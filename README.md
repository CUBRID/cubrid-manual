# cubrid-manual
Documentation for CUBRID RDBMS

1. Extension of original files of this manual 
    *.rst
    *.inc

2. Precaution to edit files.

    Please don't use tab character. Change your editor setting that "change tab as spaces". 
    1 tab should be 4 spaces.
    
3. How to build manual

   1) Install sphinx package by following the instruction of sphinx_install.txt
   2) go to the manual directory (cd ko for Korean; or cd en for English)
   3) make html
   4) now you can see the built html files

4. Make commands
    make html: make html files.
    make clean: clean built files.

5. Mainly used tags on sphinx document(.rst file).

Basically, indent is very important when you use tags.
For details, see http://sphinx-doc.org/

1) Titles

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

2) bold & italic

```
**bold text**

*italic text*
```

3) escaping with \

```
*italic text*\ _  will be "italic text_" : blank is ignored.
```

4) dot list

```
*   text
-   text
```

5) boxes

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

6) footnote

```
.. rubric:: Footnotes

.. [#f1] word: explanation
```
    
7) function

```
.. function:: function_name (arg1, arg2)
```

8) option

```
.. program:: program_name

.. option:: option_description

    explanation
```

9) links
9.1) function link

```
    :func:`func_name`
```

9.2) footnote link

```
    [#f1]_
```

9.3) internal link

```
    .. _link_position:

    :ref:`link_positon` or :ref:`title <link_positon>`
    
    :doc:`doc_path_name`
```

9.4)external link

```
    `title <http address>`_
```    
