# -*- coding: utf-8 -*-

import sys, os, sphinx
from pathlib import Path

sys.path.append(str(Path('.').resolve()))

# -- General configuration -----------------------------------------------------
needs_sphinx = "7.3.1"

extensions = [
    'sphinx_simplepdf',
    'sphinx_copybutton',
    'sphinx.ext.napoleon',
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.todo',
    'sphinx.ext.viewcode',
    'sphinx.ext.intersphinx',
    'sphinxext.rediraffe',
    'sphinx_design',
    'myst_parser',
    'sphinx_togglebutton',
    'sphinx_favicon',
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

version = '11.4'
release = '11.4.0'

html_context = {
    "switcher_url": "https://ftp.cubrid.org/CUBRID_Docs/Manuals/switcher.json"
}

exclude_patterns = ['_build']

pygments_style = 'sphinx'

suppress_warnings = [
    'misc.highlighting_failure'
]

# -- Options for plantuml and simplepdf ----------------------------------------

plantuml_output_format = "svg_img"
local_plantuml_path = os.path.join(os.path.dirname(__file__), "utils", "plantuml.jar")
plantuml = f"java -Djava.awt.headless=true -jar {local_plantuml_path}"

simplepdf_vars = {
    'cover-overlay': 'rgba(150, 26, 26, 0.7)',
}

# -- Options for HTML output ---------------------------------------------------

html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    "collapse_navigation": False,
}
html_static_path = ['_static']
html_css_files = ["style.css"]
html_last_updated_fmt = '%b %d, %Y'
html_use_smartypants = False
html_show_sphinx = False
html_show_copyright = True
html_use_index = True
html_show_sourcelink = True
htmlhelp_basename = 'cubrid_doc'

# -- Options for LaTeX output --------------------------------------------------

latex_engine = 'xelatex'
latex_elements = {
    'papersize': 'a4paper',
    'pointsize': '10pt',
    'classoptions': ',oneside',
    'geometry': 'left=2cm,right=2cm,top=2cm,bottom=2cm',
    'figure_align': 'htbp',
    'fontpkg': r'''
    \usepackage{fontspec}
    \setmainfont{NanumGothic}[Path = ./_static/fonts/, Extension = .ttf]
    \setmonofont{D2Coding}[Path = ./_static/fonts/, Extension = .ttf]
    '''
}
latex_documents = [
    ('index', 'cubrid.tex', u'CUBRID Documentation', u'CUBRID Corparation', 'manual'),
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
    r"https://dev\.mysql\.com/doc/connector-odbc/en/connector-odbc-configuration-connection-parameters\.html#codbc-dsn-option-flags"
]
linkcheck_timeout = 30
linkcheck_workers = 10
linkcheck_retries = 5

# -- Setup for jQuery (Sphinx 6.0+) -------------------------------------------

def setup(app):
    app.add_js_file('version-switcher.js')

def setup_jquery(app, exception):
    if sphinx.version_info >= (5, 0, 0):
        jquery_cdn_url = "https://code.jquery.com/jquery-3.6.0.min.js"
        html_js_files = getattr(app.config, "html_js_files", [])
        html_js_files.append((
            jquery_cdn_url,
            {
                'integrity': 'sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=',
                'crossorigin': 'anonymous'
            }
        ))
        app.config.html_js_files = html_js_files
