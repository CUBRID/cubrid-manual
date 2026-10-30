#!/usr/bin/env python3
"""Render a cubrid-manual change as HTML with the additions and deletions marked.

Reviewers read the built pages, not the RST diff, so this builds the manual
twice and marks the difference on the rendered page:

1. Find the ko/en documents the change touches (``BASE...HEAD``). A changed
   ``.inc`` file brings in every page that includes it.
2. Build the HEAD checkout (the worktree given by ``--repo``) and BASE (a
   temporary detached worktree) with ``sphinx-build -b html``.
3. For each changed page, diff the ``div#content`` body of BASE and HEAD with
   ``lxml.html.diff.htmldiff`` and write it into a copy of the HEAD site:
   ``<ins>`` (green) marks added text, ``<del>`` (red, struck through) removed
   text. Tables are diffed cell by cell and code blocks line by line. A new
   page is marked as added in full. A page that cannot be marked is listed as
   such, with a link to the unmarked page, and the other pages go on.
4. Write ``index.html`` listing the changed pages per language, then serve the
   site with ``--serve PORT`` (on 127.0.0.1 unless ``--bind`` says otherwise).

CI mode (the manual's GitHub Actions PR preview) takes the HEAD build that
``make html`` already made (``--head-html``, one language) and, with
``--inplace``, writes each marked page next to its original as
``<page>.diff.html`` plus a list page ``_changes.html`` and its data
``_changes.json`` in that directory, so the marked pages are published with
the normal preview and keep its links, styles and images. It exits 0 when the
language has no changed page. ``--comment-md`` then turns the ko and en
``_changes.json`` into the Markdown section of the PR comment.

Marking needs ``lxml``: run it with the Python environment that builds the
manual (Sphinx and lxml installed). ``--comment-md`` needs neither lxml, git
nor Sphinx.

Usage::

    manual_preview.py --repo <manual worktree> [--base origin/develop]
        [--lang ko,en] [--out DIR] [--sphinx-build PATH] [--serve PORT]
    manual_preview.py --repo . --base origin/develop --lang ko
        --head-html ko/_build/html --inplace [--sphinx-build PATH]
    manual_preview.py --comment-md OUT --changes-json KO.json EN.json --url-base URL
"""

from __future__ import annotations

import argparse
import copy
import difflib
import hashlib
import html
import http.server
import json
import pathlib
import re
import shutil
import subprocess
import sys
import tempfile
import traceback

try:
    import lxml.html as LH
    from lxml.html.diff import htmldiff
except ImportError:  # only marking needs lxml; main() checks before it marks
    LH = htmldiff = None

STYLE = """
ins.mp-diff, ins { background: #c8f2c8; color: inherit; text-decoration: none; }
del.mp-diff, del { background: #f8d0d0; color: #7a1f1f; text-decoration: line-through; }
@media (prefers-color-scheme: dark) {
  ins { background: #1f5130; } del { background: #5c2424; color: #f2c4c4; }
}
.mp-banner { position: sticky; top: 0; z-index: 50; margin: 0 0 1rem; padding: .5rem .75rem;
  border: 1px solid #9ac79a; border-radius: 6px; background: #f2fbf2; color: #1d3b1d; font-size: .9rem; }
@media (prefers-color-scheme: dark) { .mp-banner { background: #16281a; color: #cfe8cf; border-color: #2f6b3a; } }
.mp-banner a { text-decoration: underline; }
.mp-code pre, .mp-delblock pre { white-space: pre; overflow-x: auto; padding: .75rem 1rem; border-radius: 6px;
  background: #f6f8fa; color: #1b1b1b; font-size: .85rem; line-height: 1.5; }
.mp-code ins.mp-line, .mp-code del.mp-line { display: block; }
.mp-newblock { outline: 3px solid #5cb85c; border-radius: 6px; }
.mp-delblock pre { background: #f8d0d0; text-decoration: line-through; }
@media (prefers-color-scheme: dark) { .mp-code pre, .mp-delblock pre { background: #1e1e1e; color: #e6e6e6; } }
.mp-changed { box-shadow: -8px 0 0 #5cb85c; scroll-margin-top: 140px; }
tr.mp-changed > td:first-child { box-shadow: inset 6px 0 0 #5cb85c; }
.mp-current { outline: 3px solid #f0ad4e; outline-offset: 2px; }
.mp-nav { margin-left: .5rem; white-space: nowrap; }
.mp-nav button { padding: 0 .5rem; border: 1px solid #9ac79a; border-radius: 4px; background: #fff; cursor: pointer; }
@media (prefers-color-scheme: dark) { .mp-nav button { background: #1e1e1e; color: #cfe8cf; } }
"""


# in --out: the directory was made by this script, so a later run may replace it
MARKER = ".manual_preview"


def git(repo: pathlib.Path, *args: str) -> str:
    try:
        return subprocess.run(["git", "-C", str(repo), *args], check=True,
                              capture_output=True, text=True).stdout
    except subprocess.CalledProcessError as e:
        sys.exit(f"git {' '.join(args)} failed (exit {e.returncode}):\n{e.stderr.strip()}")


def changed_docs(repo: pathlib.Path, base: str, head: str, langs: list[str]) -> dict[str, list[str]]:
    """Map language -> docnames (path under the language dir, no suffix) touched by BASE...HEAD."""
    names = [n for n in git(repo, "diff", "-z", "--name-only", f"{base}...{head}", "--", *langs).split("\0") if n]
    out: dict[str, set[str]] = {lang: set() for lang in langs}
    for name in names:
        lang, _, rel = name.partition("/")
        if lang not in out or not rel.endswith((".rst", ".inc")):
            continue
        if rel.endswith(".inc"):
            inc = pathlib.Path(rel).name
            for rst in (repo / lang).rglob("*.rst"):
                if re.search(rf"^\s*\.\. include::\s+\S*{re.escape(inc)}\s*$",
                             rst.read_text(encoding="utf-8", errors="replace"), re.M):
                    out[lang].add(str(rst.relative_to(repo / lang).with_suffix("")))
        out[lang].add(str(pathlib.Path(rel).with_suffix("")))
    return {lang: sorted(docs) for lang, docs in out.items()}


def build(src: pathlib.Path, lang: str, dest: pathlib.Path, sphinx: str) -> None:
    dest.mkdir(parents=True, exist_ok=True)
    proc = subprocess.run([sphinx, "-q", "-b", "html", "-d", str(dest.parent / f".doctrees-{lang}"),
                           str(src / lang), str(dest)], capture_output=True, text=True)
    if proc.returncode != 0:
        sys.exit(f"sphinx-build failed for {src / lang}:\n{proc.stderr[-2000:]}")


def content_div(path: pathlib.Path):
    if not path.exists():
        return None, None
    tree = LH.parse(str(path))
    found = tree.xpath('//div[@id="content"]')
    return tree, (found[0] if found else None)


def inner_html(el) -> str:
    return (el.text or "") + "".join(LH.tostring(c, encoding="unicode") for c in el)


def pull_code_blocks(div) -> dict[str, tuple[str, str]]:
    """Replace each rendered code block in ``div`` with a placeholder paragraph.

    htmldiff diffs word by word and drops the whitespace between the highlight
    spans of a ``<pre>``, which garbles even unchanged examples. Code blocks are
    therefore taken out before the diff: the placeholder carries a hash of the
    block's text, so an unchanged block yields the same placeholder on both
    sides and a changed one yields a deleted and an inserted placeholder.
    Returns placeholder key -> (block HTML, block text).
    """
    blocks: dict[str, tuple[str, str]] = {}
    for pre in div.xpath(".//pre"):
        block = pre
        for anc in pre.iterancestors():
            if anc is div:
                break
            if anc.tag == "div" and any(c.startswith("highlight-") for c in (anc.get("class") or "").split()):
                block = anc
                break
        if block.getparent() is None:
            continue  # already replaced through another <pre> in the same block
        text = pre.text_content()
        key = hashlib.sha1(text.encode()).hexdigest()[:12]
        blocks[key] = (LH.tostring(block, encoding="unicode"), text)
        ph = LH.fromstring(f'<p class="mp-ph">@@P{key}@@</p>')
        ph.tail = block.tail
        block.getparent().replace(block, ph)
    return blocks


# htmldiff may nest the marks around a placeholder, e.g. <del><ins>@@P…@@</ins></del>
PH_RE = re.compile(r'(?:<p[^>]*>)?((?:\s*<(?:ins|del)>)*)\s*@@P([0-9a-f]{12})@@(?:\s*</(?:ins|del)>)*\s*(?:</p>)?')


def code_line_diff(old: str, new: str) -> str:
    rows = []
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, old.splitlines(), new.splitlines()).get_opcodes():
        if tag in ("replace", "delete"):
            rows += [f'<del class="mp-line">{html.escape(x) or " "}</del>' for x in old.splitlines()[i1:i2]]
        if tag in ("replace", "insert"):
            rows += [f'<ins class="mp-line">{html.escape(x) or " "}</ins>' for x in new.splitlines()[j1:j2]]
        if tag == "equal":
            rows += [html.escape(x) for x in new.splitlines()[j1:j2]]
    return '<div class="mp-code"><pre>' + "\n".join(rows) + "</pre></div>"


def placeholder_kind(m: re.Match, base_blocks: dict, head_blocks: dict) -> str | None:
    """"del", "ins" or None (unchanged) for a placeholder match.

    The block's side decides, not the marks htmldiff put around it (they can be
    nested or contradict the side): a key only in BASE is deleted, only in HEAD
    added. A key on both sides keeps its mark only when the marks agree (a block
    that moved or was duplicated).
    """
    key = m.group(2)
    if key not in head_blocks:
        return "del"
    if key not in base_blocks:
        return "ins"
    marks = set(re.findall(r"<(ins|del)>", m.group(1)))
    return marks.pop() if len(marks) == 1 else None


def restore_code_blocks(marked: str, base_blocks: dict, head_blocks: dict) -> str:
    """Put code blocks back: unchanged as rendered, changed as a line diff."""
    hits = list(PH_RE.finditer(marked))
    kinds = [placeholder_kind(m, base_blocks, head_blocks) for m in hits]
    out, pos, i = [], 0, 0
    while i < len(hits):
        m, kind, key = hits[i], kinds[i], hits[i].group(2)
        out.append(marked[pos:m.start()])
        nxt = hits[i + 1] if i + 1 < len(hits) else None
        if (kind and nxt is not None and kinds[i + 1] and kinds[i + 1] != kind
                and nxt.start() - m.end() < 80):  # a changed block: htmldiff emits del/ins in either order
            old_key, new_key = (key, nxt.group(2)) if kind == "del" else (nxt.group(2), key)
            out.append(code_line_diff(base_blocks[old_key][1], head_blocks[new_key][1]))
            pos, i = nxt.end(), i + 2
            continue
        if kind == "ins":
            out.append(f'<div class="mp-newblock">{head_blocks[key][0]}</div>')
        elif kind == "del":
            out.append(f'<div class="mp-delblock"><pre>{html.escape(base_blocks[key][1])}</pre></div>')
        else:
            out.append(head_blocks[key][0])
        pos, i = m.end(), i + 1
    out.append(marked[pos:])
    return "".join(out)


def diff(old: str, new: str) -> str:
    """``htmldiff``, which on an internal error prints it and returns "" (lxml 6): raise instead."""
    out = htmldiff(old, new)
    if not out and (old.strip() or new.strip()):
        raise ValueError("htmldiff failed and returned nothing (see the message printed above)")
    return out


NEST = "mp-n-"  # prefix of a tag renamed by split_nesting


def split_nesting(div) -> None:
    """Rename each element nested directly in one of the same tag (``<section>`` in ``<section>``).

    lxml 6's htmldiff miscounts a run of start or end tags of one name when it
    places a deletion, and then drops or doubles text: a changed last word of a
    subsection comes out as ``<ins>new <del>old</del> old</ins>``. The renaming
    leaves no such run; ``join_nesting`` undoes it.
    """
    for el in div.iterdescendants():
        par = el.getparent()
        if isinstance(el.tag, str) and el.tag == par.tag:
            el.tag = NEST + el.tag


def join_nesting(div) -> None:
    for el in div.iter():
        if isinstance(el.tag, str) and el.tag.startswith(NEST):
            el.tag = el.tag[len(NEST):]


def set_inner(el, markup: str) -> None:
    """Replace the content of ``el`` with the HTML fragment ``markup``."""
    for child in list(el):
        el.remove(child)
    el.text = None
    for frag in LH.fragments_fromstring(markup):
        if isinstance(frag, str):
            el.text = (el.text or "") + frag
        else:
            el.append(frag)


def pull_tables(div) -> list[tuple[str, object]]:
    """Replace each table in ``div`` with a placeholder paragraph; return [(key, table)] in page order.

    htmldiff over a whole table puts a change at the end of a cell between the
    cells or after the table, where a browser moves it above the table, and turns
    a changed cell value into an added and a deleted cell, so the row gets one
    cell too many. Tables are therefore taken out like code blocks and diffed
    cell by cell (``diff_table``). The key hashes the table's HTML, so an
    unchanged table yields the same placeholder on both sides.
    """
    tables = []
    for tbl in div.xpath(".//table[not(ancestor::table)]"):
        key = hashlib.sha1(LH.tostring(tbl, encoding="unicode", with_tail=False).encode()).hexdigest()[:12]
        ph = LH.fromstring(f'<p class="mp-ph">@@T{key}@@</p>')
        ph.tail, tbl.tail = tbl.tail, None
        tbl.getparent().replace(tbl, ph)
        tables.append((key, tbl))
    return tables


def align(old_keys: list, new_keys: list, old_words: list, new_words: list) -> list[tuple[int | None, int | None]]:
    """Pair BASE items with HEAD items in order: [(BASE index or None, HEAD index or None)].

    Items with equal keys pair first. A stretch between them that has as many
    items on both sides was edited in place and pairs one to one. In any other
    stretch the most similar two (word ratio 0.5 or more) pair, then the parts
    before and after that pair are paired the same way; what is left is
    deleted or added.
    """
    out: list[tuple[int | None, int | None]] = []

    def best(i1: int, i2: int, j1: int, j2: int) -> None:
        top, bi, bj = 0.0, -1, -1
        for i in range(i1, i2):
            for j in range(j1, j2):
                sm = difflib.SequenceMatcher(None, old_words[i], new_words[j], autojunk=False)
                if sm.real_quick_ratio() > top and sm.quick_ratio() > top and sm.ratio() > top:
                    top, bi, bj = sm.ratio(), i, j
        if top < 0.5:
            out.extend([(i, None) for i in range(i1, i2)] + [(None, j) for j in range(j1, j2)])
            return
        best(i1, bi, j1, bj)
        out.append((bi, bj))
        best(bi + 1, i2, bj + 1, j2)

    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, old_keys, new_keys, autojunk=False).get_opcodes():
        if tag == "equal" or i2 - i1 == j2 - j1:
            out.extend(zip(range(i1, i2), range(j1, j2)))
        else:
            best(i1, i2, j1, j2)
    return out


def words(el) -> list[str]:
    return " ".join(el.itertext()).split()  # text_content() would glue the texts of adjacent cells


def table_rows(tbl) -> list:
    return [r for r in tbl.iter("tr") if next(r.iterancestors("table")) is tbl]


def row_cells(row) -> list:
    return [c for c in row if c.tag in ("td", "th")]


def mark_whole(el, kind: str) -> None:
    """Mark the content of each cell of row ``el`` (or of ``el`` itself) as added ("ins") or deleted ("del")."""
    for c in row_cells(el) or [el]:
        body = inner_html(c)
        set_inner(c, diff("", body) if kind == "ins" else diff(body, ""))


def diff_table(old, new) -> str:
    """Return HEAD's table ``new`` as HTML with its differences from BASE's ``old`` marked inside the cells.

    Either side may be None (a table added or deleted). Rows pair by ``align``.
    A pair with as many cells on both sides is diffed cell by cell; otherwise,
    and for a row only in BASE, the BASE row is put back struck through before
    the HEAD row. Every mark therefore stays in a cell and every row keeps its
    own cells.
    """
    old, new = copy.deepcopy(old), copy.deepcopy(new)  # the same table can be shown twice
    if old is None or new is None:
        tbl = new if old is None else old
        for el in table_rows(tbl) + tbl.findall("caption"):
            mark_whole(el, "ins" if old is None else "del")
        return LH.tostring(tbl, encoding="unicode", with_tail=False)
    olds, news = table_rows(old), table_rows(new)

    def key(row) -> str:  # the cells only: an added row changes the row-odd/row-even class of the rows after it
        return "".join(LH.tostring(c, encoding="unicode", with_tail=False) for c in row_cells(row))

    placed, prev = 0, None  # HEAD rows done so far, and the row placed last
    for i, j in align([key(r) for r in olds], [key(r) for r in news],
                      [words(r) for r in olds], [words(r) for r in news]):
        if i is not None and j is not None and len(row_cells(olds[i])) == len(row_cells(news[j])):
            for oc, nc in zip(row_cells(olds[i]), row_cells(news[j])):
                a, b = inner_html(oc), inner_html(nc)
                if a != b:
                    set_inner(nc, diff(a, b))
        else:
            if i is not None:  # put the BASE row back, in its own section (thead/tbody) when possible
                row, section = olds[i], olds[i].getparent().tag
                mark_whole(row, "del")
                nxt = news[placed] if placed < len(news) else None
                if nxt is not None and (prev is None or nxt.getparent().tag == section):
                    nxt.addprevious(row)
                elif prev is not None:
                    prev.addnext(row)
                else:
                    new.append(row)
                prev = row
            if j is not None:
                mark_whole(news[j], "ins")
        if j is not None:
            placed, prev = placed + 1, news[j]
    old_cap, new_cap = old.find("caption"), new.find("caption")
    if new_cap is not None:
        a = inner_html(old_cap) if old_cap is not None else ""
        if a != inner_html(new_cap):
            set_inner(new_cap, diff(a, inner_html(new_cap)))
    return LH.tostring(new, encoding="unicode", with_tail=False)


# a table placeholder and the marks htmldiff put around it (groups as in PH_RE)
TABLE_RE = re.compile(r'(?:<p class="mp-ph">)?((?:\s*<(?:ins|del)>)*)\s*@@T([0-9a-f]{12})@@'
                      r'(?:\s*</(?:ins|del)>)*\s*(?:</p>)?')


def restore_tables(marked: str, base_tables: list, head_tables: list) -> str:
    """Put tables back: unchanged as rendered, changed ones marked by ``diff_table``.

    A changed table leaves a BASE-only and a HEAD-only key. ``align`` pairs such
    keys by the tables' words, whatever htmldiff did around the placeholders,
    and the pair is shown where HEAD has the table.
    """
    base, head = dict(base_tables), dict(head_tables)
    gone = [k for k in dict.fromkeys(k for k, _ in base_tables) if k not in head]
    added = [k for k in dict.fromkeys(k for k, _ in head_tables) if k not in base]
    partner = {added[j]: gone[i] for i, j in align(gone, added, [words(base[k]) for k in gone],
                                                     [words(head[k]) for k in added])
               if i is not None and j is not None}
    paired = set(partner.values())

    def put(m: re.Match) -> str:
        key = m.group(2)
        if key in partner:
            return diff_table(base[partner[key]], head[key])
        if key in paired:
            return ""  # shown, marked, where HEAD has the table
        kind = placeholder_kind(m, base, head)
        if kind is None:
            return LH.tostring(head[key], encoding="unicode", with_tail=False)
        return diff_table(base[key], None) if kind == "del" else diff_table(None, head[key])

    return TABLE_RE.sub(put, marked)


BLOCK_TAGS = {"p", "li", "tr", "dt", "dd", "h1", "h2", "h3", "h4", "h5", "h6", "blockquote", "pre"}
MARK_DIVS = {"mp-code", "mp-newblock", "mp-delblock"}

NAV_JS = """
(function () {
  var m = [].slice.call(document.querySelectorAll('[data-mp]')), i = -1;
  function show(k) {
    // scroll directly: the theme's smooth scrolling does not settle reliably on table rows
    var r = (m[k].tagName === 'TR' ? m[k].cells[0] : m[k]).getBoundingClientRect();
    window.scrollTo({top: window.scrollY + r.top - window.innerHeight / 3, behavior: 'instant'});
  }
  function go(k) {
    if (!m.length) return;
    m.forEach(function (e) { e.classList.remove('mp-current'); });
    i = k; m[i].classList.add('mp-current'); show(i);
    var c = document.getElementById('mp-pos'); if (c) c.textContent = (i + 1) + ' / ' + m.length;
  }
  window.mpStep = function (d) { go((i + d + m.length) % m.length); };
  document.addEventListener('keydown', function (e) {
    if (/INPUT|TEXTAREA/.test(e.target.tagName) || e.ctrlKey || e.metaKey || e.altKey) return;
    if (e.key === 'n') mpStep(1); else if (e.key === 'p') mpStep(-1);
  });
  window.addEventListener('load', function () {
    var h = /^#mp-(\\d+)$/.exec(location.hash);
    if (!h || !m.length) return;
    go(Math.min(+h[1], m.length) - 1);
    // fonts and the theme's scripts still change heights after load: keep the block in view while
    // they do, for 3 s at most, and stop as soon as the reader scrolls, clicks or presses a key
    var k = i, live = true, evs = ['wheel', 'touchstart', 'pointerdown', 'keydown'];
    var ro = window.ResizeObserver && new ResizeObserver(function () { if (live) show(k); });
    function stop() {
      live = false;
      if (ro) ro.disconnect();
      evs.forEach(function (e) { window.removeEventListener(e, stop, true); });
    }
    evs.forEach(function (e) { window.addEventListener(e, stop, true); });
    if (ro) ro.observe(document.body);
    if (document.fonts) document.fonts.ready.then(function () { if (live) show(k); });
    setTimeout(stop, 3000);
  });
})();
"""

# Banner and list-page texts per language; "changes" is formatted with n and s (English plural).
TEXT = {
    "ko": {"preview": "변경 미리보기", "added": "추가", "deleted": "삭제", "list": "변경된 페이지 목록",
           "unmarked": "표시 없이 보기", "changes": "변경 {n}곳", "prev": "◀ 이전", "next": "다음 ▶",
           "keys": "(단축키 n / p)", "title": "매뉴얼 변경 미리보기", "page": "페이지", "blocks": "변경 블록",
           "new": "새 페이지", "nohtml": "head 에 HTML 페이지 없음", "failed": "변경 표시 실패",
           "none": "이 언어({lang})에는 바뀐 페이지가 없다.",
           "intro": "페이지 안에서 초록 배경은 추가, 빨간 취소선은 삭제다. 페이지 링크는 첫 번째 변경 위치로 열리고, "
                    "페이지 위 배너의 이전·다음 버튼(단축키 n / p)으로 바뀐 곳을 차례로 옮겨 다닌다. "
                    "바뀐 블록 왼쪽에는 초록 막대가 있다. \"변경 블록\"은 바뀐 문단·목록 항목·표 행·코드 블록의 수이고, 추가·삭제는 표시된 구간의 수다."},
    "en": {"preview": "Change preview", "added": "added", "deleted": "deleted", "list": "list of changed pages",
           "unmarked": "view without marks", "changes": "{n} change{s}", "prev": "◀ prev", "next": "next ▶",
           "keys": "(keys n / p)", "title": "Manual change preview", "page": "Page", "blocks": "Changed blocks",
           "new": "new page", "nohtml": "no HTML page in head", "failed": "could not be marked",
           "none": "No page changed in this language ({lang}).",
           "intro": "On a page, a green background marks added text and red strikethrough deleted text. A page link "
                    "opens at the first change; the prev/next buttons in the banner (keys n / p) step through the "
                    "changes, and a green bar marks the left of each changed block. \"Changed blocks\" counts the changed paragraphs, list items, table rows and code "
                    "blocks; added and deleted count the marked spans."},
}


def drop_invisible_marks(div) -> None:
    """Unmark changes with no text, e.g. a heading's permalink whose generated id moved."""
    for el in list(div.iter("ins", "del")):
        if el.text_content().strip() or el.getparent() is None:
            continue
        if el.tag == "del":
            el.drop_tree()  # keeps its tail text
        else:
            el.drop_tag()


def number_changes(div) -> int:
    """Number the blocks that hold a change (``data-mp``, anchors ``#mp-N``) for the nav bar."""
    drop_invisible_marks(div)
    blocks = []
    for el in div.iter("ins", "del", "div"):
        classes = set((el.get("class") or "").split())
        if el.tag == "div":
            if not classes & MARK_DIVS:
                continue
            blk = el
        else:
            anc = list(el.iterancestors())
            if any(set((a.get("class") or "").split()) & (MARK_DIVS | {"mp-banner"}) for a in anc):
                continue  # inside a code diff (already its own block) or the banner
            inside = anc[:anc.index(div)] if div in anc else anc
            # a table row or list item is one change, not one per cell or paragraph
            blk = el
            for wanted in ({"tr"}, {"li"}, BLOCK_TAGS):
                found = next((a for a in inside if a.tag in wanted), None)
                if found is not None:
                    blk = found
                    break
        if blk not in blocks and not any(b in blk.iterancestors() for b in blocks):
            blocks.append(blk)
    for n, blk in enumerate(blocks, 1):
        blk.set("data-mp", str(n))
        blk.set("class", ((blk.get("class") or "") + " mp-changed").strip())
        holder = blk
        if blk.tag == "tr":  # header rows have th cells only
            holder = next((c for c in blk if c.tag in ("td", "th")), blk)
        anchor = LH.fromstring(f'<a id="mp-{n}" class="mp-anchor"></a>')
        anchor.tail, holder.text = holder.text, None
        holder.insert(0, anchor)
    return len(blocks)


def mark_page(base_page: pathlib.Path, head_page: pathlib.Path, out_page: pathlib.Path,
              banner: str, t: dict[str, str]) -> tuple[int, int, int]:
    """Write HEAD's page with BASE->HEAD differences marked; return (#ins, #del, #changed blocks)."""
    head_tree, head_div = content_div(head_page)
    if head_div is None:
        raise ValueError(f"{head_page} has no div#content")
    _, base_div = content_div(base_page)
    if base_div is None:
        marked = f"<ins>{inner_html(head_div)}</ins>"
    else:
        base_blocks, head_blocks = pull_code_blocks(base_div), pull_code_blocks(head_div)
        split_nesting(base_div)
        split_nesting(head_div)
        base_tables, head_tables = pull_tables(base_div), pull_tables(head_div)
        marked = diff(inner_html(base_div), inner_html(head_div))
        marked = restore_tables(marked, base_tables, head_tables)
        marked = re.sub(r"\s*Link: [^\s<]+", "", marked)  # htmldiff spells out changed hrefs
        # htmldiff also leaves marks around nothing but whitespace, e.g. <ins></ins> next to a
        # wholly deleted cell; drop them.
        marked = re.sub(r"<(ins|del)>(\s*)</\1>", r"\2", marked)
        marked = restore_code_blocks(marked, base_blocks, head_blocks)
    set_inner(head_div, banner + marked)
    join_nesting(head_div)
    n_blocks = number_changes(head_div)
    n_ins = sum(1 for _ in head_div.iter("ins")) - banner.count("<ins")  # the banner's legend
    n_del = sum(1 for _ in head_div.iter("del")) - banner.count("<del")
    nav = LH.fromstring(f'<span class="mp-nav">{t["changes"].format(n=n_blocks, s="" if n_blocks == 1 else "s")} '
                        f'<button type="button" onclick="mpStep(-1)">{t["prev"]}</button> '
                        f'<span id="mp-pos">– / {n_blocks}</span> '
                        f'<button type="button" onclick="mpStep(1)">{t["next"]}</button> {t["keys"]}</span>')
    head_div.find('div[@class="mp-banner"]').append(nav)
    root = head_tree.getroot()
    root.find("head").append(LH.fromstring(f"<style>{STYLE}</style>"))
    script = LH.Element("script")
    script.text = NAV_JS
    root.find("body").append(script)
    out_page.parent.mkdir(parents=True, exist_ok=True)
    out_page.write_bytes(LH.tostring(head_tree, encoding="utf-8", doctype="<!DOCTYPE html>"))
    return n_ins, n_del, n_blocks


LIST_STYLE = """body{font-family:system-ui,sans-serif;margin:2rem auto;max-width:860px;padding:0 16px;background:#fff;color:#1b1b1b}
table{border-collapse:collapse;width:100%}td,th{border-bottom:1px solid #ddd;padding:.4rem .5rem;text-align:left}
@media (prefers-color-scheme: dark){body{background:#121212;color:#e6e6e6}td,th{border-color:#333}a{color:#8ab4f8}}"""


def write_changes(path: pathlib.Path, lang: str, rows: list, base_sha: str, head_sha: str) -> None:
    """Write ``--inplace``'s list of one language's changed pages (links relative to ``path``)
    and, next to it, the same list as ``_changes.json`` for ``--comment-md``."""
    t = TEXT[lang]
    items, pages = [], []
    for _, doc, rel, n_ins, n_del, n_blk, is_new, failed in rows:
        if not (path.parent / rel).exists():  # a page deleted in head
            items.append(f'<tr><td>{html.escape(doc)} ({t["nohtml"]})</td><td></td><td></td><td></td><td></td></tr>')
            continue  # no page to link to, so not in the JSON either
        pages.append({"doc": doc, "blocks": n_blk, "ins": n_ins, "del": n_del, "new": is_new, "error": failed})
        new = f' ({t["new"]})' if is_new else ""
        unmarked = f'<td><a href="{html.escape(rel)}">{t["unmarked"]}</a></td>'
        if failed:  # no .diff.html was written: link the page itself
            items.append(f'<tr><td><a href="{html.escape(rel)}">{html.escape(doc)}</a>{new} ({t["failed"]})</td>'
                         f'<td></td><td></td><td></td>{unmarked}</tr>')
            continue
        diff = html.escape(f"{doc}.diff.html") + ("#mp-1" if n_blk else "")
        items.append(f'<tr><td><a href="{diff}">{html.escape(doc)}</a>{new}</td><td>{n_blk}</td><td>{n_ins}</td>'
                     f'<td>{n_del}</td>{unmarked}</tr>')
    shas = f'base <code>{base_sha[:9]}</code> → head <code>{head_sha[:9]}</code>.'
    if rows:
        body = (f'<p>{shas} {t["intro"]}</p>\n<table><tr><th>{t["page"]}</th><th>{t["blocks"]}</th>'
                f'<th>{t["added"].capitalize()}</th><th>{t["deleted"].capitalize()}</th>'
                f'<th>{t["unmarked"].capitalize()}</th></tr>\n' + "\n".join(items) + "\n</table>")
    else:
        body = f'<p>{shas} {t["none"].format(lang=lang)}</p>'
    path.write_text(f"""<!doctype html><html lang="{lang}"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1"><title>{t["title"]} ({lang})</title>
<style>{LIST_STYLE}</style>
</head><body><h1>{t["title"]} ({lang})</h1>
{body}
</body></html>""", encoding="utf-8")
    path.with_suffix(".json").write_text(json.dumps(
        {"lang": lang, "base": base_sha, "head": head_sha, "pages": sorted(pages, key=lambda x: x["doc"])},
        ensure_ascii=False, indent=1) + "\n", encoding="utf-8")


# a docname as the comment may show it: path characters only, so a forged entry cannot add Markdown,
# a link, a table cell or an @mention
DOC_RE = re.compile(r"[A-Za-z0-9_][A-Za-z0-9_./-]*")


def well_formed(p) -> bool:
    """Whether a ``_changes.json`` page entry is fit for the comment (the file comes from the PR's build)."""
    return (isinstance(p, dict) and isinstance(p.get("doc"), str) and DOC_RE.fullmatch(p["doc"]) is not None
            and ".." not in p["doc"] and all(type(p.get(k)) is int and p[k] >= 0 for k in ("blocks", "ins", "del"))
            and type(p.get("new")) is bool and type(p.get("error", False)) is bool)


def comment_md(changes: dict[str, dict | None], url: str) -> str:
    """Render the PR comment's changed-page section from each language's ``_changes.json``.

    None for a language means its file is missing or unreadable. Page entries
    that are not well formed are left out with a note on stderr.
    """
    pages: dict[str, dict] = {}
    for lang, data in changes.items():
        entries = [] if data is None else data.get("pages", [])
        if not isinstance(entries, list):
            print(f"{lang}: \"pages\" is not a list; ignored", file=sys.stderr)
            entries = []
        pages[lang] = {}
        for p in entries:
            if well_formed(p):
                pages[lang][p["doc"]] = p
            else:
                print(f"{lang}: skipped a malformed page entry {json.dumps(p)[:200]}", file=sys.stderr)
    gone = " and ".join(lang.upper() for lang, data in changes.items() if data is None)
    docs = sorted(set(pages["ko"]) | set(pages["en"]))
    if not docs:
        if not gone:
            return "**🔍 Changed pages** — none.\n"
        rest = " and ".join(lang.upper() for lang, data in changes.items() if data is not None)
        return f"**🔍 Changed pages** — change list unavailable for {gone}" + (f"; none in {rest}" if rest else "") + ".\n"

    def cell(lang: str, doc: str) -> str:
        if changes[lang] is None:
            return "?"
        p = pages[lang].get(doc)
        if p is None:
            return "—"
        if p.get("error"):  # no .diff.html: link the page itself
            return f"⚠️ [not marked]({url}/{lang}/{doc}.html)"
        n = p["blocks"]
        href = f"{url}/{lang}/{doc}.diff.html" + ("#mp-1" if n else "")
        return ("🆕 " if p["new"] else "") + f"[{n} change{'' if n == 1 else 's'}]({href})"

    rows = []
    for doc in docs:
        ko, en = pages["ko"].get(doc), pages["en"].get(doc)
        warn = " ⚠️" if ko is None or en is None or ko["blocks"] != en["blocks"] else ""
        rows.append(f"| {doc}{warn} | {cell('ko', doc)} | {cell('en', doc)} |")
    table = "| Page | KO | EN |\n| --- | --- | --- |\n" + "\n".join(rows)
    if len(docs) > 15:
        table = f"<details><summary>Show {len(docs)} changed pages</summary>\n\n{table}\n\n</details>"
    lists = " · ".join(f"[{lang.upper()}]({url}/{lang}/_changes.html)" for lang, data in changes.items()
                       if data is not None)
    return (f"**🔍 Changed pages ({len(docs)})** — Each link opens the page at its first change. "
            "Additions are green and deletions are struck through in red. Press `n` / `p` to move between "
            f"changes. Full lists: {lists}" + (f". Change list unavailable for {gone}." if gone else "")
            + f"\n\n{table}\n")


def main(argv: list[str] | None = None) -> int:
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--repo", default=".", help="manual worktree at HEAD (default: cwd)")
    p.add_argument("--base", default="origin/develop")
    p.add_argument("--head", default="HEAD")
    p.add_argument("--lang", default="ko,en")
    p.add_argument("--out", default=None,
                   help=f"output dir (default: a temp dir); an existing one must be empty or from an earlier run "
                        f"(holding {MARKER}), as it is replaced")
    p.add_argument("--sphinx-build", default=shutil.which("sphinx-build")
                   or str(pathlib.Path(sys.executable).with_name("sphinx-build")))
    p.add_argument("--head-html", metavar="DIR",
                   help="HEAD's HTML already built for the one language in --lang (HEAD is not rebuilt)")
    p.add_argument("--inplace", action="store_true",
                   help="write <page>.diff.html next to each page in --head-html and _changes.html there")
    p.add_argument("--serve", type=int, metavar="PORT", help="serve the marked site on this port")
    p.add_argument("--bind", default="127.0.0.1", help="address --serve listens on (default: 127.0.0.1)")
    p.add_argument("--comment-md", metavar="OUT",
                   help="write the PR comment's changed-page section (Markdown) from --changes-json")
    p.add_argument("--changes-json", nargs=2, metavar=("KO_JSON", "EN_JSON"),
                   help="the _changes.json of ko and en; a missing or unreadable file is reported as unavailable")
    p.add_argument("--url-base", metavar="URL", help="URL holding ko/ and en/ of the preview")
    a = p.parse_args(argv)

    if a.comment_md:
        if not (a.changes_json and a.url_base):
            p.error("--comment-md needs --changes-json KO_JSON EN_JSON and --url-base")
        changes: dict[str, dict | None] = {}
        for lang, f in zip(("ko", "en"), map(pathlib.Path, a.changes_json)):
            try:
                data = json.loads(f.read_text(encoding="utf-8"))
            except (OSError, ValueError) as e:  # e.g. the language's preview step was skipped or failed
                print(f"{f}: {'missing' if not f.exists() else e}; change list for {lang} unavailable", file=sys.stderr)
                data = None
            if data is not None and not isinstance(data, dict):
                print(f"{f}: not a JSON object; change list for {lang} unavailable", file=sys.stderr)
                data = None
            if data is not None and data.get("lang") != lang:
                p.error(f"{f} holds lang {data.get('lang')!r}, expected {lang} (order: KO_JSON EN_JSON)")
            changes[lang] = data
        pathlib.Path(a.comment_md).write_text(comment_md(changes, a.url_base.rstrip("/")), encoding="utf-8")
        return 0
    if LH is None:
        sys.exit("manual_preview.py needs lxml; run it with the manual venv's python")

    langs = [x for x in a.lang.split(",") if x]
    if not langs or set(langs) - TEXT.keys():
        p.error("--lang takes ko, en or ko,en")
    if a.head_html and len(langs) != 1:
        p.error("--head-html needs exactly one language in --lang")
    if a.inplace and not a.head_html:
        p.error("--inplace needs --head-html")
    if a.inplace and a.serve:
        p.error("--serve cannot be used with --inplace")
    head_html = pathlib.Path(a.head_html).resolve() if a.head_html else None
    if head_html and not (head_html / "index.html").is_file():
        p.error(f"--head-html {a.head_html}: no index.html (build it first)")
    given = pathlib.Path(a.out) if a.out else None
    if given and given.exists() and not (given / MARKER).is_file() and (not given.is_dir() or any(given.iterdir())):
        p.error(f"--out {a.out}: not empty and not made by this script (no {MARKER}); give a new or empty directory")

    repo = pathlib.Path(git(pathlib.Path(a.repo), "rev-parse", "--show-toplevel").strip())
    base_sha = git(repo, "merge-base", a.base, a.head).strip()
    head_sha = git(repo, "rev-parse", a.head).strip()
    if git(repo, "rev-parse", "HEAD").strip() != head_sha:
        sys.exit("--repo must be checked out at --head (build HEAD from its worktree)")
    docs = changed_docs(repo, a.base, a.head, langs)
    if not any(docs.values()):
        if a.inplace:  # e.g. a PR that changes only the other language: not an error in CI
            write_changes(head_html / "_changes.html", langs[0], [], base_sha, head_sha)
            print(f"no {langs[0]} .rst/.inc changes in {a.base}...{a.head}; wrote {head_html / '_changes.html'}")
            return 0
        sys.exit(f"no ko/en .rst/.inc changes in {a.base}...{a.head}")

    out = given or pathlib.Path(tempfile.mkdtemp(prefix="manual-preview-"))
    if (out / MARKER).is_file():
        shutil.rmtree(out)  # an earlier run's output (checked above)
    out.mkdir(parents=True, exist_ok=True)
    (out / MARKER).write_text("made by manual_preview.py; a later run with this --out replaces the directory\n",
                              encoding="utf-8")
    site = out / "site"
    base_wt = out / "base-src"
    heads = {lang: head_html or out / "head" / lang for lang in langs}
    git(repo, "worktree", "add", "--detach", str(base_wt), base_sha)
    try:
        for lang in langs:
            if head_html is None:
                print(f"building {lang}: head …", flush=True)
                build(repo, lang, heads[lang], a.sphinx_build)
            print(f"building {lang}: base {base_sha[:9]} …", flush=True)
            build(base_wt, lang, out / "base" / lang, a.sphinx_build)
            if not a.inplace:
                shutil.copytree(heads[lang], site / lang)
    finally:
        git(repo, "worktree", "remove", "--force", str(base_wt))

    rows = []
    for lang in langs:
        t = TEXT[lang]
        for doc in docs[lang]:
            rel = f"{doc}.html"
            is_new = not (out / "base" / lang / rel).exists()
            if not (heads[lang] / rel).exists():  # deleted in head: listed without a link
                rows.append((lang, doc, rel, 0, 0, 0, is_new, False))
                continue
            if a.inplace:  # the marked page sits next to the original; links stay relative to it
                out_page = head_html / f"{doc}.diff.html"
                links = (f'<a href="{"../" * rel.count("/")}_changes.html">{t["list"]}</a>'
                         f' · <a href="{html.escape(pathlib.PurePosixPath(rel).name)}">{t["unmarked"]}</a>')
            else:
                out_page = site / lang / rel
                links = f'<a href="{"../" * (rel.count("/") + 1)}index.html">{t["list"]}</a>'
            banner = (f'<div class="mp-banner">{t["preview"]} — <ins>{t["added"]}</ins> · <del>{t["deleted"]}</del>'
                      f' · base <code>{base_sha[:9]}</code> → head <code>{head_sha[:9]}</code> · {links}</div>')
            try:
                n_ins, n_del, n_blk = mark_page(out / "base" / lang / rel, heads[lang] / rel, out_page, banner, t)
                failed = False
            except Exception:  # one page must not cost the others, nor fail the build
                traceback.print_exc()
                print(f"{lang}/{rel}: could not be marked; listed with a link to the unmarked page", file=sys.stderr)
                n_ins = n_del = n_blk = 0
                failed = True
            rows.append((lang, doc, rel, n_ins, n_del, n_blk, is_new, failed))

    if a.inplace:
        write_changes(head_html / "_changes.html", langs[0], rows, base_sha, head_sha)
        if a.out is None:
            shutil.rmtree(out, ignore_errors=True)
        print(f"changes: {head_html / '_changes.html'}")
        for lang, doc, rel, i, d, b, new, failed in rows:
            print(f"  {lang}/{doc}.diff.html: " + ("could not be marked" if failed else
                                                   f"{b} block(s), +{i} -{d}{' (new)' if new else ''}"))
        return 0

    items = "\n".join(  # a page that could not be marked is the unmarked copy in the site
        f'<tr><td>{lang}</td><td><a href="{lang}/{html.escape(rel)}{"#mp-1" if b else ""}">{html.escape(doc)}</a>'
        f'{" (새 페이지)" if new else ""}{" (변경 표시 실패)" if failed else ""}</td><td>{b}</td><td>{i}</td><td>{d}</td></tr>'
        if (site / lang / rel).exists() else
        f'<tr><td>{lang}</td><td>{html.escape(doc)} (head 에 HTML 페이지 없음)</td><td></td><td></td><td></td></tr>'
        for lang, doc, rel, i, d, b, new, failed in rows)
    (site / "index.html").write_text(f"""<!doctype html><html lang="ko"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1"><title>매뉴얼 변경 미리보기</title>
<style>{LIST_STYLE}</style>
</head><body><h1>매뉴얼 변경 미리보기</h1>
<p>base <code>{base_sha[:9]}</code> → head <code>{head_sha[:9]}</code>. 페이지 안에서 초록 배경은 추가, 빨간 취소선은 삭제다.
페이지 링크는 첫 번째 변경 위치로 열리고, 페이지 위 배너의 이전·다음 버튼(단축키 n / p)으로 바뀐 곳을 차례로 옮겨 다닌다. 바뀐 블록 왼쪽에는 초록 막대가 있다.
"변경 블록"은 바뀐 문단·목록 항목·표 행·코드 블록의 수이고, 추가·삭제는 표시된 구간의 수다. 바뀌지 않은 페이지는 <a href="ko/index.html">ko</a> · <a href="en/index.html">en</a> 에서 그대로 볼 수 있다.</p>
<table><tr><th>언어</th><th>페이지</th><th>변경 블록</th><th>추가</th><th>삭제</th></tr>
{items}
</table></body></html>""", encoding="utf-8")
    print(f"site: {site}")
    for lang, doc, rel, i, d, b, new, failed in rows:
        print(f"  {lang}/{rel}: " + ("could not be marked" if failed else
                                     f"{b} block(s), +{i} -{d}{' (new)' if new else ''}"))

    if a.serve:
        handler = lambda *x, **k: http.server.SimpleHTTPRequestHandler(*x, directory=str(site), **k)
        srv = http.server.ThreadingHTTPServer((a.bind, a.serve), handler)
        print(f"serving on http://{a.bind}:{a.serve}/  (Ctrl-C to stop)", flush=True)
        try:
            srv.serve_forever()
        except KeyboardInterrupt:
            pass
    return 0


if __name__ == "__main__":
    sys.exit(main())
