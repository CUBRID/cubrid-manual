# cubrid-manual
Documentation for CUBRID RDBMS

1. Extension of original files of this manual 

   ```
    *.rst
    *.inc
   ```

2. Precaution to edit files.

    Please don't use tab character. Change your editor setting that "change tab as spaces". 1 tab should be 4 spaces.

3. Prerequisites

    To build the documentation locally, you need:
    - **Python 3.12** (recommended)
    - **pip** (Python package manager)
    - **make** (for running build commands)

    ```
    pip3 install -r requirements.txt
    ```

4. Additional System Packages Required

    The following system packages are required to support advanced features such as PDF rendering, Korean localization, and Sphinx extensions. These are **in addition to basic build tools** like Python, pip, make, and gcc.

    | Package Name          | Purpose                                                                |
    |-----------------------|------------------------------------------------------------------------|
    | `glibc-locale-source` | Generates custom locales (e.g., `ko_KR.utf8`)                          |
    | `glibc-langpack-ko`   | Korean language pack for UTF-8 support                                 |
    | `openssl-devel`       | Required for SSL-related Python packages                               |
    | `bzip2-devel`         | Supports compression features used in PDF generation                   |
    | `libffi-devel`        | Enables building Python C extensions                                   |
    | `zlib-devel`          | Provides compression support                                           |
    | `pango`               | Text layout engine used by PDF renderers like `weasyprint`             |
    | `cairo`               | Graphics library for rendering PDFs and images                         |
    | `gdk-pixbuf2`         | Image processing and rendering support                                 |
    | `git`                 | Version control and external documentation integration                 |
    
5. How to build manual

   1. go to the manual directory (cd ko for Korean; or cd en for English)
   1. make html
   1. now you can see the built html files from _build/

6. Make commands

    ```
    make clean
    ```

    ```
    make html
    ```

    ```
    make pdf
    ```

7. Mainly used tags on sphinx document(.rst file).

   Basically, indent is very important when you use tags. For details, see http://sphinx-doc.org/

    1. Titles
    
    ```
    *****
    First
    *****
    
    Second
    ======
    
    Third
    -----
    
    Fourth
    ^^^^^^
    
    Fifth
    +++++
    ```
    
    2. bold & italic
    
    ```
    **bold text**
    
    *italic text*
    ```
    
    3. escaping with \
    
    ```
    *italic text*\_  will be "italic text_" : blank is ignored.
    ```
    
    4. dot list
    
    ```
    *   text
    -   text
    ```
    
    5. boxes
    
    ```
    ::
    
        text
    ```
    
    ```
    .. code-block:: sql
    
        SELECT 1;
    ```
    
    ```
    .. note::
    
        note this.
    ```
    
    ```
    .. warning::
    
        warning this.
    ```
    
    6. footnote
    
    ```
    .. rubric:: Footnotes
    
    .. [#f1] word: explanation
    ```
        
    7. function
    
    ```
    .. function:: function_name (arg1, arg2)
    ```
    
    8. option
    
    ```
    .. program:: program_name
    
    .. option:: option_description
    
        explanation
    ```
    
    9. links
    
        1. function link
    
           ```
               :func:`func_name`
           ```
    
        1. footnote link
    
           ```
               [#f1]_
           ```
    
        1. internal link
    
           ```
               .. _link_position:
    
               :ref:`link_positon` or :ref:`title <link_positon>`
        
               :doc:`doc_path_name`
           ```
    
        1. external link
    
           ```
               `title <http address>`_
           ```    
