# -*- coding: utf-8 -*-

import sys, os, sphinx

from pathlib import Path

from dataclasses import asdict
from sphinx.application import Sphinx
from sphinx.util.docfields import Field
from sphinxawesome_theme import ThemeOptions, __version__
from sphinxawesome_theme.postprocess import Icons

sys.path.append(str(Path('.').resolve()))

# -- General configuration -----------------------------------------------------
needs_sphinx = "7.3.1"

extensions = [
    'sphinx_simplepdf',
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    "sphinx.ext.extlinks",
    'sphinx.ext.autosummary',
    'sphinx.ext.viewcode',
    'sphinxext.rediraffe',
    'sphinx_design',
]

templates_path = ['_templates']

source_suffix = {
    '.rst': 'restructuredtext',
    '.inc': 'restructuredtext',
    '.md': 'markdown',
}

master_doc = 'index'

project = u'CUBRID'
copyright = u'2016, CUBRID Corparation'

version = '11.0'
release = '11.0'

language = 'ko'
html_search_language = "ko"
html_use_index = True

html_title = f"CUBRID {version} User Manual"

html_context = {
    "switcher_url": "https://ftp.cubrid.org/CUBRID_Docs/Manuals/switcher.json",
    "show_version_switcher": True,
}

exclude_patterns = ['_build']

pygments_style = "nord"
pygments_style_dark = "github-dark"

suppress_warnings = [
    'misc.highlighting_failure'
]

html_favicon = "_static/logo_only.png"

# -- Options for plantuml and simplepdf ----------------------------------------
plantuml_output_format = "svg_img"
local_plantuml_path = os.path.join(os.path.dirname(__file__), "utils", "plantuml.jar")
plantuml = f"java -Djava.awt.headless=true -jar {local_plantuml_path}"

simplepdf_vars = {
    'cover-overlay': 'rgba(150, 26, 26, 0.7)',
}

# -- Options for HTML output ---------------------------------------------------
html_theme = 'sphinxawesome_theme'
html_theme_options = {
  "logo_light": "_static/logo.svg",
  "logo_dark": "_static/logo.svg",
  "show_prev_next": "true",
  "show_scrolltop": "true",
  "awesome_headerlinks": "false",
  "main_nav_links": {
    "Release Note" : "release_note/index",
    "Index": "./genindex",
    "Q&A": "https://www.cubrid.com/qna",
  },
  "extra_header_link_icons": {
    "GitHub": {
      "link": "https://github.com/CUBRID",
      "icon": '<div class="inline-flex items-center justify-center px-0 text-sm font-medium transition-colors rounded-md hover:bg-accent hover:text-accent-foreground h-9 w-9"><svg fill="currentColor" height="26px" style="margin-top:-2px;display:inline" viewBox="0 0 45 44" xmlns="http://www.w3.org/2000/svg"><path clip-rule="evenodd" d="M22.477.927C10.485.927.76 10.65.76 22.647c0 9.596 6.223 17.736 14.853 20.608 1.087.2 1.483-.47 1.483-1.047 0-.516-.019-1.881-.03-3.693-6.04 1.312-7.315-2.912-7.315-2.912-.988-2.51-2.412-3.178-2.412-3.178-1.972-1.346.149-1.32.149-1.32 2.18.154 3.327 2.24 3.327 2.24 1.937 3.318 5.084 2.36 6.321 1.803.197-1.403.759-2.36 1.379-2.903-4.823-.548-9.894-2.412-9.894-10.734 0-2.37.847-4.31 2.236-5.828-.224-.55-.969-2.759.214-5.748 0 0 1.822-.584 5.972 2.226 1.732-.482 3.59-.722 5.437-.732 1.845.01 3.703.25 5.437.732 4.147-2.81 5.967-2.226 5.967-2.226 1.185 2.99.44 5.198.217 5.748 1.392 1.517 2.232 3.457 2.232 5.828 0 8.344-5.078 10.18-9.916 10.717.779.67 1.474 1.996 1.474 4.021 0 2.904-.027 5.247-.027 5.96 0 .58.392 1.256 1.493 1.044C37.981 40.375 44.2 32.24 44.2 22.647c0-11.996-9.726-21.72-21.722-21.72" fill="currentColor" fill-rule="evenodd"></path></svg></div>',
      "icon_pack": ""
    },
  },
#    "breadcrumbs_separator": "/",
}
html_static_path = ['_static']
html_css_files = ["style.css"]

# -- Awesome Shinx Theme
html_permalinks_icon = Icons.permalinks_icon

extlinks = {
    "sphinxdocs": ("https://www.sphinx-doc.org/en/master/%s", "%s"),
}

html_sidebars = {
#  "**": [], -- Hide sidebar
#  "**": ["sidebar_main_nav_links.html", "sidebar_toc.html"],
#  "/": ["sidebar_main_nav_links.html"],
}

html_last_updated_fmt = '%b %d, %Y'
html_use_smartypants = False
html_show_sphinx = False
html_show_copyright = True
html_use_index = True
html_show_sourcelink = True
htmlhelp_basename = 'cubrid_doc'

sd_custom_directives = {
    "dropdown-syntax": {
        "inherit": "dropdown",
        "argument": "Syntax",
        "options": {
            "color": "primary",
            "icon": "code",
        },
    }
}

# -- Options for LaTeX output --------------------------------------------------
latex_engine = 'xelatex'
latex_elements = {
    'papersize': 'a4paper',
    'pointsize': '9pt',
    'classoptions': ',oneside',
    'geometry': 'left=2cm,right=2cm,top=2cm,bottom=2cm',
    'figure_align': 'htbp',
    'fontpkg': r'''
    \usepackage{fontspec}
    \setmainfont{Pretendard Variable}[Path = ./_static/fonts/, Extension = .ttf]
    \setmonofont{D2Coding}[Path = ./_static/fonts/, Extension = .ttf]
    '''
}
latex_documents = [
    ('index', 'cubrid.tex', u'CUBRID Manual', u'CUBRID Corparation', 'manual'),

]
latex_domain_indices = True

# -- Options for manual page output --------------------------------------------
man_pages = [
    ('index', 'cubrid', u'CUBRID Documentation', [u'CUBRID Corparation'], 1)
]

# -- Options for Texinfo output ------------------------------------------------
texinfo_documents = [
    ('index', 'cubrid', u'CUBRID Documentation',
     u'CUBRID Corparation', 'cubrid', 'One line description of project.',
     'Miscellaneous'),
]
texinfo_domain_indices = True

# -- Options for linkcheck builder ---------------------------------------------
linkcheck_ignore = [
    r'https://github.com/CUBRID/cubrid/.*',
    r'http://jira.cubrid.org/browse/.*',
    r'https://www.apachelounge.com/download/win64/binaries/.*',
    r'https://linux.die.net/man/2/posix_fadvise',
    r"https://dev\.mysql\.com/downloads/repo/yum/",
    r"https://dev\.mysql\.com/doc/connector-odbc/en/connector-odbc-installation-binary-yum\.html",
    r"https://dev\.mysql\.com/doc/connector-odbc/en/connector-odbc-configuration-connection-parameters\.html#codbc-dsn-option-flags",
    r"https://metacpan.org/dist/DBI/",
    r"https://www\.gnu\.org/software/libc/manual/html_node/Malloc-Tunable-Parameters",
]
linkcheck_timeout = 30
linkcheck_workers = 10
linkcheck_retries = 5

html_js_files = ['version-switcher.js', 'theme-toggle.js']
