/*
 * Sphinx JavaScript utilities for the full-text search.
 */
"use strict";

/**
 * Simple result scoring code.
 */
if (typeof Scorer === "undefined") {
  var Scorer = {
    // Implement the following function to further tweak the score for each result
    // The function takes a result array [docname, title, anchor, descr, score, filename]
    // and returns the new score.
    /*
    score: result => {
      const [docname, title, anchor, descr, score, filename, kind] = result
      return score
    },
    */

    // query matches the full name of an object
    objNameMatch: 11,
    // or matches in the last dotted part of the object name
    objPartialMatch: 6,
    // Additive scores depending on the priority of the object
    objPrio: {
      0: 15, // used to be importantResults
      1: 5, // used to be objectResults
      2: -5, // used to be unimportantResults
    },
    //  Used when the priority is not in the mapping.
    objPrioDefault: 0,

    // query found in title
    title: 15,
    partialTitle: 7,
    // query found in terms
    term: 5,
    partialTerm: 2,
    
    // [신규] 전체 구문 일치에 대한 추가 점수
    phraseMatch: 75,  // 전체 구문이 정확히 일치할 때 추가 점수
    partialPhrase: 25, // 구문의 일부가 연속으로 일치할 때 추가 점수
    
    // [신규] 한글 조사 가중치 감소 설정
    koreanParticleWeight: 0.7, // 조사가 포함된 단어의 가중치 감소율
    koreanContentWordBonus: 1.5 // 실질 형태소(내용어) 가중치 증가율
  };
}

// [신규] 한글 조사 목록
const KOREAN_PARTICLES = new Set([
  '은', '는', '이', '가', '을', '를', '의', '에', '에서', '으로', '로',
  '와', '과', '랑', '이랑', '하고', '부터', '까지', '도', '만', '처럼',
  '보다', '한', '한테', '께', '에게', '한테서', '에게서', '로서', '로써',
  '이라고', '라는', '이란', '란', '고', '며', '면서', '며는', '면',
  '다', '습니다', 'ㅂ니다', '아', '어', '여', '게', '지', '네', '나',
  '군', '구나', '구려', '세', '소', 'ㅂ시오', '읍시오', '읍시다',
  'ㅂ시다', '자', 'ㅂ시다', '시죠', '세요', '으세요', '이세요',
  '으십시오', '십시오', '라', '으라', '너라', '거라', '마', '지마',
  '습니다', 'ㅂ니다', '았', '었', '였', '겠', '습', 'ㅂ'
]);

// [신규] 한글 실질 형태소(내용어) 확인 함수
const _isKoreanContentWord = (word) => {
  // 한글 음절로만 이루어진 단어인지 확인
  const koreanRegex = /^[가-힣]+$/;
  if (!koreanRegex.test(word)) return false;
  
  // 조사 목록에 없으면 실질 형태소로 간주
  return !KOREAN_PARTICLES.has(word);
};

// [신규] 검색어에서 조사를 제거하고 내용어만 추출
const _extractContentWords = (query) => {
  const words = query.split(/\s+/);
  const contentWords = [];
  const particles = [];
  
  words.forEach(word => {
    if (_isKoreanContentWord(word)) {
      contentWords.push(word);
    } else {
      // 조사인 경우 별도 저장 (가중치 감소용)
      particles.push(word);
    }
  });
  
  return { contentWords, particles };
};

// [신규] 조사를 고려한 단어 가중치 계산
const _calculateWordWeight = (word) => {
  if (_isKoreanContentWord(word)) {
    return Scorer.koreanContentWordBonus; // 내용어는 가중치 증가
  }
  return Scorer.koreanParticleWeight; // 조사는 가중치 감소
};

// Global search result kind enum, used by themes to style search results.
// prettier-ignore
class SearchResultKind {
  static get index() { return "index"; }
  static get object() { return "object"; }
  static get text() { return "text"; }
  static get title() { return "title"; }
}

const _removeChildren = (element) => {
  while (element && element.lastChild) element.removeChild(element.lastChild);
};

/**
 * See https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Regular_Expressions#escaping
 */
const _escapeRegExp = (string) =>
  string.replace(/[.*+\-?^${}()|[\]\\]/g, "\\$&"); // $& means the whole matched string

const _escapeHTML = (text) => {
  return text
    .replaceAll("&", "&amp;")
    .replaceAll("<", "&lt;")
    .replaceAll(">", "&gt;")
    .replaceAll('"', "&quot;")
    .replaceAll("'", "&apos;");
};

// [수정됨] 전체 구문 일치 점수를 계산하는 함수 (조사 가중치 적용)
const _calculatePhraseScore = (text, originalQuery, searchTerms) => {
  if (!text || !originalQuery) return 0;
  
  const textLower = text.toLowerCase();
  const queryLower = originalQuery.toLowerCase().trim();
  
  // 조사를 제거한 내용어만으로 구문 일치 확인
  const { contentWords } = _extractContentWords(queryLower);
  const contentPhrase = contentWords.join(' ');
  
  // 1. 내용어 구문이 정확히 일치하는 경우 (조사 제외)
  if (contentWords.length > 1 && textLower.includes(contentPhrase)) {
    return Scorer.phraseMatch;
  }
  
  // 2. 전체 구문이 정확히 일치하는 경우 (조사 포함)
  if (textLower.includes(queryLower)) {
    return Math.round(Scorer.phraseMatch * 0.8); // 조사 포함은 약간 낮은 점수
  }
  
  // 3. 구문의 연속된 내용어들이 일치하는지 확인
  const queryWords = queryLower.split(/\s+/);
  let maxConsecutiveMatches = 0;
  let currentConsecutive = 0;
  
  const textWords = textLower.split(/\s+/);
  
  for (let i = 0; i < textWords.length; i++) {
    for (let j = 0; j < queryWords.length; j++) {
      // 단어 비교 시 조사 가중치 적용
      const wordWeight = _calculateWordWeight(queryWords[j]);
      
      if (textWords[i] === queryWords[j]) {
        currentConsecutive = wordWeight;
        let k = 1;
        while (i + k < textWords.length && j + k < queryWords.length && 
               textWords[i + k] === queryWords[j + k]) {
          const nextWordWeight = _calculateWordWeight(queryWords[j + k]);
          currentConsecutive += nextWordWeight;
          k++;
        }
        maxConsecutiveMatches = Math.max(maxConsecutiveMatches, currentConsecutive);
      }
    }
  }
  
  if (maxConsecutiveMatches >= 2) {
    const totalPossibleWeight = queryWords.reduce((sum, w) => 
      sum + _calculateWordWeight(w), 0);
    const phraseRatio = maxConsecutiveMatches / totalPossibleWeight;
    return Math.round(Scorer.partialPhrase * phraseRatio);
  }
  
  return 0;
};

// [신규] 텍스트 내용을 가져오는 함수 (비동기)
const _fetchDocumentContent = async (docName, anchor) => {
  const docBuilder = DOCUMENTATION_OPTIONS.BUILDER;
  const docFileSuffix = DOCUMENTATION_OPTIONS.FILE_SUFFIX;
  const contentRoot = document.documentElement.dataset.content_root;
  
  let requestUrl;
  if (docBuilder === "dirhtml") {
    let dirname = docName + "/";
    if (dirname.match(/\/index\/$/))
      dirname = dirname.substring(0, dirname.length - 6);
    else if (dirname === "index/") dirname = "";
    requestUrl = contentRoot + dirname;
  } else {
    requestUrl = contentRoot + docName + docFileSuffix;
  }
  
  try {
    const response = await fetch(requestUrl);
    const html = await response.text();
    return Search.htmlToText(html, anchor);
  } catch (error) {
    console.warn("Failed to fetch document content:", error);
    return "";
  }
};

const _displayScore = (score, phraseBonus = 0, contentWordRatio = 0) => {
  const scoreBadge = document.createElement("span");
  scoreBadge.classList.add("search-score-badge");
  
  const totalScore = score + phraseBonus;
  
  // [신규] 내용어 비율 표시
  const contentInfo = contentWordRatio > 0 ? `, 내용어: ${Math.round(contentWordRatio * 100)}%` : '';
  
  scoreBadge.textContent = `점수: ${totalScore} (기본: ${score}, 구문: +${phraseBonus}${contentInfo})`;
  
  scoreBadge.style.marginLeft = "10px";
  scoreBadge.style.fontSize = "0.8em";
  
  // 점수에 따른 색상 변화
  if (totalScore >= 80) {
    scoreBadge.style.color = "#fff";
    scoreBadge.style.backgroundColor = "#28a745"; // 높은 점수: 녹색
  } else if (totalScore >= 50) {
    scoreBadge.style.backgroundColor = "#ffc107"; // 중간 점수: 노란색
    scoreBadge.style.color = "#000";
  } else {
    scoreBadge.style.backgroundColor = "#f0f0f0"; // 낮은 점수: 회색
    scoreBadge.style.color = "#666";
  }
  
  scoreBadge.style.padding = "2px 6px";
  scoreBadge.style.borderRadius = "4px";
  scoreBadge.style.fontWeight = "bold";
  return scoreBadge;
};

// [수정됨] 모든 결과를 모아서 점수를 계산하고 정렬하여 표시
const _displayItemsWithScores = async (items, searchTerms, highlightTerms, originalQuery) => {
  Search.output.innerHTML = ''; // 기존 결과 초기화
  
  // 각 아이템에 대한 점수 정보를 저장할 배열
  const itemsWithScores = [];
  
  // 검색어에서 내용어 추출
  const { contentWords, particles } = _extractContentWords(originalQuery);
  const contentWordRatio = contentWords.length / (contentWords.length + particles.length);
  
  // 모든 아이템의 문서 내용을 비동기적으로 가져오기
  const fetchPromises = items.map(async (item) => {
    const [docName, title, anchor, descr, baseScore, filename, kind] = item;
    
    // 문서 내용 가져오기
    const content = await _fetchDocumentContent(docName, anchor);
    
    // 구문 점수 계산 (조사 가중치 적용)
    const phraseBonus = _calculatePhraseScore(content, originalQuery, searchTerms);
    const totalScore = baseScore + phraseBonus;
    
    return {
      item,
      baseScore,
      phraseBonus,
      totalScore,
      title,
      content,
      contentWordRatio
    };
  });
  
  // 모든 Promise가 완료될 때까지 대기
  const itemsWithScoreData = await Promise.all(fetchPromises);
  
  // 총점(totalScore) 기준으로 내림차순 정렬
  itemsWithScoreData.sort((a, b) => {
    if (a.totalScore !== b.totalScore) {
      return b.totalScore - a.totalScore; // 높은 점수가 먼저
    }
    // 점수가 같으면 제목 알파벳 순
    const titleA = a.title.toLowerCase();
    const titleB = b.title.toLowerCase();
    return titleA.localeCompare(titleB);
  });
  
  // 정렬된 순서대로 화면에 표시
  itemsWithScoreData.forEach((itemData) => {
    const [docName, title, anchor, descr, baseScore, filename, kind] = itemData.item;
    
    const listItem = document.createElement("li");
    listItem.classList.add(`kind-${kind}`);
    
    const docBuilder = DOCUMENTATION_OPTIONS.BUILDER;
    const docFileSuffix = DOCUMENTATION_OPTIONS.FILE_SUFFIX;
    const docLinkSuffix = DOCUMENTATION_OPTIONS.LINK_SUFFIX;
    const showSearchSummary = DOCUMENTATION_OPTIONS.SHOW_SEARCH_SUMMARY;
    const contentRoot = document.documentElement.dataset.content_root;
    
    let requestUrl;
    let linkUrl;
    if (docBuilder === "dirhtml") {
      let dirname = docName + "/";
      if (dirname.match(/\/index\/$/))
        dirname = dirname.substring(0, dirname.length - 6);
      else if (dirname === "index/") dirname = "";
      requestUrl = contentRoot + dirname;
      linkUrl = requestUrl;
    } else {
      requestUrl = contentRoot + docName + docFileSuffix;
      linkUrl = docName + docLinkSuffix;
    }
    
    let linkEl = listItem.appendChild(document.createElement("a"));
    linkEl.href = linkUrl + anchor;
    linkEl.dataset.score = baseScore;
    linkEl.innerHTML = _escapeHTML(title);
    
    // 점수 표시 (내용어 비율 포함)
    linkEl.appendChild(_displayScore(baseScore, itemData.phraseBonus, itemData.contentWordRatio));
    
    if (descr) {
      listItem.appendChild(document.createElement("span")).innerHTML =
        ` (${_escapeHTML(descr)})`;
      if (SPHINX_HIGHLIGHT_ENABLED)
        highlightTerms.forEach((term) =>
          _highlightText(listItem, term, "highlighted"),
        );
    } else if (showSearchSummary) {
      // 요약 정보 표시 (이미 가져온 content 사용)
      if (itemData.content) {
        listItem.appendChild(
          Search.makeSearchSummaryWithText(itemData.content, searchTerms, anchor)
        );
        if (SPHINX_HIGHLIGHT_ENABLED)
          highlightTerms.forEach((term) =>
            _highlightText(listItem, term, "highlighted"),
          );
      }
    }
    
    Search.output.appendChild(listItem);
  });
  
  _finishSearch(items.length);
};

const _finishSearch = (resultCount) => {
  Search.stopPulse();
  Search.title.innerText = _("Search Results");
  
  if (!resultCount) {
    Search.status.innerText = Documentation.gettext(
      "Your search did not match any documents. Please make sure that all words are spelled correctly and that you've selected enough categories.",
    );
  } else {
    const statusText = Documentation.ngettext(
      "Search finished, found one page matching the search query.",
      "Search finished, found ${resultCount} pages matching the search query.",
      resultCount,
    ).replace("${resultCount}", resultCount);
    
    Search.status.innerText = `${statusText}`; // (점수 순으로 정렬됨, 한글 조사 가중치 조정 적용);
  }
};

const _orderResultsByScoreThenName = (a, b) => {
  const leftScore = a[4];
  const rightScore = b[4];
  if (leftScore === rightScore) {
    const leftTitle = a[1].toLowerCase();
    const rightTitle = b[1].toLowerCase();
    if (leftTitle === rightTitle) return 0;
    return leftTitle > rightTitle ? 1 : -1;
  }
  return leftScore > rightScore ? -1 : 1; // 높은 점수가 먼저
};

if (typeof splitQuery === "undefined") {
  var splitQuery = (query) =>
    query
      .split(/[^\p{Letter}\p{Number}_\p{Emoji_Presentation}]+/gu)
      .filter((term) => term);
}

/**
 * Search Module
 */
const Search = {
  _index: null,
  _queued_query: null,
  _pulse_status: -1,
  _originalQuery: null,

  htmlToText: (htmlString, anchor) => {
    const htmlElement = new DOMParser().parseFromString(
      htmlString,
      "text/html",
    );
    for (const removalQuery of [".headerlink", "script", "style"]) {
      htmlElement.querySelectorAll(removalQuery).forEach((el) => {
        el.remove();
      });
    }
    if (anchor) {
      const anchorContent = htmlElement.querySelector(
        `[role="main"] ${anchor}`,
      );
      if (anchorContent) return anchorContent.textContent;

      console.warn(
        `Anchored content block not found. Sphinx search tries to obtain it via DOM query '[role=main] ${anchor}'. Check your theme or template.`,
      );
    }

    const docContent = htmlElement.querySelector('[role="main"]');
    if (docContent) return docContent.textContent;

    console.warn(
      "Content block not found. Sphinx search tries to obtain it via DOM query '[role=main]'. Check your theme or template.",
    );
    return "";
  },

  init: () => {
    const query = new URLSearchParams(window.location.search).get("q");
    document
      .querySelectorAll('input[name="q"]')
      .forEach((el) => (el.value = query));
    if (query) Search.performSearch(query);
  },

  loadIndex: (url) =>
    (document.body.appendChild(document.createElement("script")).src = url),

  setIndex: (index) => {
    Search._index = index;
    if (Search._queued_query !== null) {
      const query = Search._queued_query;
      Search._queued_query = null;
      Search.query(query);
    }
  },

  hasIndex: () => Search._index !== null,

  deferQuery: (query) => (Search._queued_query = query),

  stopPulse: () => (Search._pulse_status = -1),

  startPulse: () => {
    if (Search._pulse_status >= 0) return;

    const pulse = () => {
      Search._pulse_status = (Search._pulse_status + 1) % 4;
      Search.dots.innerText = ".".repeat(Search._pulse_status);
      if (Search._pulse_status >= 0) window.setTimeout(pulse, 500);
    };
    pulse();
  },

  performSearch: (query) => {
    Search._originalQuery = query;
    
    const searchText = document.createElement("h2");
    searchText.textContent = _("Searching");
    const searchSummary = document.createElement("p");
    searchSummary.classList.add("search-summary");
    searchSummary.innerText = "";
    const searchList = document.createElement("ul");
    searchList.setAttribute("role", "list");
    searchList.classList.add("search");

    const out = document.getElementById("search-results");
    Search.title = out.appendChild(searchText);
    Search.dots = Search.title.appendChild(document.createElement("span"));
    Search.status = out.appendChild(searchSummary);
    Search.output = out.appendChild(searchList);

    const searchProgress = document.getElementById("search-progress");
    if (searchProgress) {
      searchProgress.innerText = _("Preparing search...");
    }
    Search.startPulse();

    if (Search.hasIndex()) Search.query(query);
    else Search.deferQuery(query);
  },

  _parseQuery: (query) => {
    const stemmer = new Stemmer();
    const searchTerms = new Set();
    const excludedTerms = new Set();
    const highlightTerms = new Set();
    const objectTerms = new Set(splitQuery(query.toLowerCase().trim()));
    
    splitQuery(query.trim()).forEach((queryTerm) => {
      const queryTermLower = queryTerm.toLowerCase();

      if (stopwords.has(queryTermLower) || queryTerm.match(/^\d+$/)) return;

      let word = stemmer.stemWord(queryTermLower);
      if (word[0] === "-") excludedTerms.add(word.substr(1));
      else {
        searchTerms.add(word);
        highlightTerms.add(queryTermLower);
      }
    });

    if (SPHINX_HIGHLIGHT_ENABLED) {
      localStorage.setItem(
        "sphinx_highlight_terms",
        [...highlightTerms].join(" "),
      );
    }

    return [query, searchTerms, excludedTerms, highlightTerms, objectTerms];
  },

  _performSearch: (
    query,
    searchTerms,
    excludedTerms,
    highlightTerms,
    objectTerms,
  ) => {
    const filenames = Search._index.filenames;
    const docNames = Search._index.docnames;
    const titles = Search._index.titles;
    const allTitles = Search._index.alltitles;
    const indexEntries = Search._index.indexentries;

    const normalResults = [];
    const nonMainIndexResults = [];

    _removeChildren(document.getElementById("search-progress"));

    const queryLower = query.toLowerCase().trim();
    
    // Title 검색
    for (const [title, foundTitles] of Object.entries(allTitles)) {
      if (
        title.toLowerCase().trim().includes(queryLower)
        && queryLower.length >= title.length / 2
      ) {
        for (const [file, id] of foundTitles) {
          const score = Math.round(
            (Scorer.title * queryLower.length) / title.length,
          );
          const boost = titles[file] === title ? 1 : 0;
          normalResults.push([
            docNames[file],
            titles[file] !== title ? `${titles[file]} > ${title}` : title,
            id !== null ? "#" + id : "",
            null,
            score + boost,
            filenames[file],
            SearchResultKind.title,
          ]);
        }
      }
    }

    // Index 검색
    for (const [entry, foundEntries] of Object.entries(indexEntries)) {
      if (entry.includes(queryLower) && queryLower.length >= entry.length / 2) {
        for (const [file, id, isMain] of foundEntries) {
          const score = Math.round((100 * queryLower.length) / entry.length);
          const result = [
            docNames[file],
            titles[file],
            id ? "#" + id : "",
            null,
            score,
            filenames[file],
            SearchResultKind.index,
          ];
          if (isMain) {
            normalResults.push(result);
          } else {
            nonMainIndexResults.push(result);
          }
        }
      }
    }

    // Object 검색
    objectTerms.forEach((term) =>
      normalResults.push(...Search.performObjectSearch(term, objectTerms)),
    );

    // Terms 검색
    normalResults.push(
      ...Search.performTermsSearch(searchTerms, excludedTerms),
    );

    // 사용자 정의 점수 함수 적용
    if (Scorer.score) {
      normalResults.forEach((item) => (item[4] = Scorer.score(item)));
      nonMainIndexResults.forEach((item) => (item[4] = Scorer.score(item)));
    }

    // 기본 점수로 1차 정렬
    normalResults.sort(_orderResultsByScoreThenName);
    nonMainIndexResults.sort(_orderResultsByScoreThenName);

    // 결과 결합
    let results = [...nonMainIndexResults, ...normalResults];

    // 중복 제거
    let seen = new Set();
    results = results.reverse().reduce((acc, result) => {
      let resultStr = result
        .slice(0, 4)
        .concat([result[5]])
        .map((v) => String(v))
        .join(",");
      if (!seen.has(resultStr)) {
        acc.push(result);
        seen.add(resultStr);
      }
      return acc;
    }, []);

    return results.reverse();
  },

  query: (query) => {
    const [
      searchQuery,
      searchTerms,
      excludedTerms,
      highlightTerms,
      objectTerms,
    ] = Search._parseQuery(query);
    
    const results = Search._performSearch(
      searchQuery,
      searchTerms,
      excludedTerms,
      highlightTerms,
      objectTerms,
    );

    _displayItemsWithScores(results, searchTerms, highlightTerms, query);
  },

  performObjectSearch: (object, objectTerms) => {
    const filenames = Search._index.filenames;
    const docNames = Search._index.docnames;
    const objects = Search._index.objects;
    const objNames = Search._index.objnames;
    const titles = Search._index.titles;

    const results = [];

    const objectSearchCallback = (prefix, match) => {
      const name = match[4];
      const fullname = (prefix ? prefix + "." : "") + name;
      const fullnameLower = fullname.toLowerCase();
      if (fullnameLower.indexOf(object) < 0) return;

      let score = 0;
      const parts = fullnameLower.split(".");

      if (fullnameLower === object || parts.slice(-1)[0] === object)
        score += Scorer.objNameMatch;
      else if (parts.slice(-1)[0].indexOf(object) > -1)
        score += Scorer.objPartialMatch;

      const objName = objNames[match[1]][2];
      const title = titles[match[0]];

      const otherTerms = new Set(objectTerms);
      otherTerms.delete(object);
      if (otherTerms.size > 0) {
        const haystack = `${prefix} ${name} ${objName} ${title}`.toLowerCase();
        if (
          [...otherTerms].some((otherTerm) => haystack.indexOf(otherTerm) < 0)
        )
          return;
      }

      let anchor = match[3];
      if (anchor === "") anchor = fullname;
      else if (anchor === "-") anchor = objNames[match[1]][1] + "-" + fullname;

      const descr = objName + _(", in ") + title;

      if (Scorer.objPrio.hasOwnProperty(match[2]))
        score += Scorer.objPrio[match[2]];
      else score += Scorer.objPrioDefault;

      results.push([
        docNames[match[0]],
        fullname,
        "#" + anchor,
        descr,
        score,
        filenames[match[0]],
        SearchResultKind.object,
      ]);
    };
    
    Object.keys(objects).forEach((prefix) =>
      objects[prefix].forEach((array) => objectSearchCallback(prefix, array)),
    );
    return results;
  },

  performTermsSearch: (searchTerms, excludedTerms) => {
    const terms = Search._index.terms;
    const titleTerms = Search._index.titleterms;
    const filenames = Search._index.filenames;
    const docNames = Search._index.docnames;
    const titles = Search._index.titles;

    const scoreMap = new Map();
    const fileMap = new Map();

    searchTerms.forEach((word) => {
      const files = [];
      const arr = [
        {
          files: terms.hasOwnProperty(word) ? terms[word] : undefined,
          score: Scorer.term,
        },
        {
          files: titleTerms.hasOwnProperty(word) ? titleTerms[word] : undefined,
          score: Scorer.title,
        },
      ];
      
      if (word.length > 2) {
        const escapedWord = _escapeRegExp(word);
        if (!terms.hasOwnProperty(word)) {
          Object.keys(terms).forEach((term) => {
            if (term.match(escapedWord))
              arr.push({ files: terms[term], score: Scorer.partialTerm });
          });
        }
        if (!titleTerms.hasOwnProperty(word)) {
          Object.keys(titleTerms).forEach((term) => {
            if (term.match(escapedWord))
              arr.push({ files: titleTerms[term], score: Scorer.partialTitle });
          });
        }
      }

      if (arr.every((record) => record.files === undefined)) return;

      arr.forEach((record) => {
        if (record.files === undefined) return;

        let recordFiles = record.files;
        if (recordFiles.length === undefined) recordFiles = [recordFiles];
        files.push(...recordFiles);

        recordFiles.forEach((file) => {
          if (!scoreMap.has(file)) scoreMap.set(file, new Map());
          const fileScores = scoreMap.get(file);
          fileScores.set(word, record.score);
        });
      });

      files.forEach((file) => {
        if (!fileMap.has(file)) fileMap.set(file, [word]);
        else if (fileMap.get(file).indexOf(word) === -1)
          fileMap.get(file).push(word);
      });
    });

    const results = [];
    for (const [file, wordList] of fileMap) {
      const filteredTermCount = [...searchTerms].filter(
        (term) => term.length > 2,
      ).length;
      
      if (
        wordList.length !== searchTerms.size
        && wordList.length !== filteredTermCount
      )
        continue;

      if (
        [...excludedTerms].some(
          (term) =>
            terms[term] === file
            || titleTerms[term] === file
            || (terms[term] || []).includes(file)
            || (titleTerms[term] || []).includes(file),
        )
      )
        continue;

      const score = Math.max(...wordList.map((w) => scoreMap.get(file).get(w)));
      results.push([
        docNames[file],
        titles[file],
        "",
        null,
        score,
        filenames[file],
        SearchResultKind.text,
      ]);
    }
    return results;
  },

  makeSearchSummary: (htmlText, keywords, anchor) => {
    const text = Search.htmlToText(htmlText, anchor);
    if (text === "") return null;

    const textLower = text.toLowerCase();
    const actualStartPosition = [...keywords]
      .map((k) => textLower.indexOf(k.toLowerCase()))
      .filter((i) => i > -1)
      .slice(-1)[0];
    const startWithContext = Math.max(actualStartPosition - 120, 0);

    const top = startWithContext === 0 ? "" : "...";
    const tail = startWithContext + 240 < text.length ? "..." : "";

    let summary = document.createElement("p");
    summary.classList.add("context");
    summary.textContent =
      top + text.substr(startWithContext, 240).trim() + tail;

    return summary;
  },
  
  // 미리 가져온 텍스트로 요약 생성
  makeSearchSummaryWithText: (text, keywords, anchor) => {
    if (text === "") return null;

    const textLower = text.toLowerCase();
    const actualStartPosition = [...keywords]
      .map((k) => textLower.indexOf(k.toLowerCase()))
      .filter((i) => i > -1)
      .slice(-1)[0];
    const startWithContext = Math.max(actualStartPosition - 120, 0);

    const top = startWithContext === 0 ? "" : "...";
    const tail = startWithContext + 240 < text.length ? "..." : "";

    let summary = document.createElement("p");
    summary.classList.add("context");
    summary.textContent =
      top + text.substr(startWithContext, 240).trim() + tail;

    return summary;
  },
};

_ready(Search.init);
