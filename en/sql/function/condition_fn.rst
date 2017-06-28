
:meta-keywords: cubrid coalesce, cubrid decode, cubrid greatest, cubrid if, cubrid ifnull, cubrid nvl, cubrid isnull, cubrid least, cubrid nullif, cubrid nlv2

:tocdepth: 3

********************
Comparison Functions
********************

.. contents::

COALESCE
========

.. function:: COALESCE(expression [, expression ] ...)

    The **COALESCE** function has more than one expression as an argument. If the first argument is non-**NULL**, the corresponding value is returned if it is **NULL**, the second argument is returned. If all expressions which have an argument are **NULL**, **NULL** is returned. Therefore, this function is generally used to replace **NULL** with other default value.

    :param expression: Specifies more than one expression. Their types must be comparable each other.
    :rtype: determined with the type of the arguments

Operation is performed by converting the type of every argument into that with the highest priority. If there is an argument whose type cannot be converted, the type of every argument is converted into a **VARCHAR** type. The following list shows priority of conversion based on input argument type.

*   **CHAR** < **VARCHAR**
*   **BIT** < **VARBIT**
*   **SHORT** < **INT** < **BIGINT** < **NUMERIC** < **FLOAT** < **DOUBLE**
*   **DATE** < **TIMESTAMP** < **DATETIME**

For example, if a type of a is **INT**, b, **BIGINT**, c, **SHORT**, and d, **FLOAT**, then **COALESCE** (a, b, c, d) returns a **FLOAT** type. If a type of a is **INTEGER**, b, **DOULBE** , c, **FLOAT**, and d, **TIMESTAMP**, then **COALESCE** (a, b, c, d) returns a **VARCHAR** type.

**COALESCE** (*a, b*) works the same as the **CASE** expression as follows: ::

    CASE WHEN a IS NOT NULL
    THEN a
    ELSE b
    END

.. code-block:: sql

    SELECT * FROM case_tbl;
    
::

                a
    =============
                1
                2
                3
             NULL
     
.. code-block:: sql

    --substituting a default value 10.0000 for a NULL value
    SELECT a, COALESCE(a, 10.0000) FROM case_tbl;
    
::

                a  coalesce(a, 10.0000)
    ===================================
                1  1.0000
                2  2.0000
                3  3.0000
             NULL  10.0000

DECODE
======

.. function:: DECODE(expression, search, result [, search, result]* [, default])

    As well as a **CASE** expression, the **DECODE** function performs the same functionality as the **IF** ... **THEN** ... **ELSE** statement. It compares the *expression* argument with *search* argument, and returns the *result* corresponding to *search* that has the same value. It returns *default* if there is no *search* with the same value, and returns **NULL** if *default* is omitted. An expression argument and a search argument to be comparable should be same or convertible each other. The number of digits after the decimal point is determined to display all significant figures including valid number of all *result*.

    :param expression,search: expressions that are comparable with each other
    :param result: the value to be returned when matched
    :param default: the value to be retuned when no match is found 
    :rtype: determined with the type of *result* and *default*

**DECODE**\(*a*, *b*, *c*, *d*, *e, f*) has the same meaning as the **CASE** expression below. ::

    CASE WHEN a = b THEN c
    WHEN a = d THEN e
    ELSE f
    END

.. code-block:: sql

    SELECT * FROM case_tbl;
    
::

                a
    =============
                1
                2
                3
             NULL
     
.. code-block:: sql

    --Using DECODE function to compare expression and search values one by one
    SELECT a, DECODE(a, 1, 'one', 2, 'two', 'other') FROM case_tbl;
    
::

                a  decode(a, 1, 'one', 2, 'two', 'other')
    ===================================
                1  'one'
                2  'two'
                3  'other'
             NULL  'other'
     
     
.. code-block:: sql

    --result types are converted to a single type containing all of significant figures
    SELECT a, DECODE(a, 1, 1, 2, 1.2345, 1.234567890) FROM case_tbl;
    
::

                a  decode(a, 1, 1, 2, 1.2345, 1.234567890)
    ===================================
                1  1.000000000
                2  1.234500000
                3  1.234567890
             NULL  1.234567890
     
.. code-block:: sql

    --an error occurs when result types are not convertible
    SELECT a, DECODE(a, 1, 'one', 2, 'two', 1.2345) FROM case_tbl;
     
::

    ERROR: Cannot coerce 'one' to type double.

GREATEST
========

.. function:: GREATEST(expression [, expression] ...)

    The **GREATEST** function compares more than one expression specified as parameters and returns the greatest value. If only one expression has been specified, the expression is returned because there is no expression to be compared with.

    Therefore, more than one expression that is specified as parameters must be of the type that can be compared with each other. If the types of the specified parameters are identical, so are the types of the return values; if they are different, the type of the return value becomes a convertible common data type.

    That is, the **GREATEST** function compares the values of column 1, column 2 and column 3 in the same row and returns the greatest value while the **MAX** function compares the values of column in all result rows and returns the greatest value.

    :param expression: Specifies more than one expression. Their types must be comparable each other. One of the arguments is **NULL**, **NULL** is returned.
    :rtype: same as that of the argument
    
The following example shows how to retrieve the number of every medals and the highest number that Korea won in the *demodb* database.

.. code-block:: sql

    SELECT gold, silver , bronze, GREATEST (gold, silver, bronze) 
    FROM participant
    WHERE nation_code = 'KOR';
    
::

             gold       silver       bronze  greatest(gold, silver, bronze)
    =======================================================================
                9           12            9                              12
                8           10           10                              10
                7           15            5                              15
               12            5           12                              12
               12           10           11                              12

IF
==

.. function:: IF(expression1, expression2, expression3)

    The **IF** function returns *expression2* if the value of the arithmetic expression specified as the first parameter is **TRUE**, or *expression3* if the value is **FALSE** or **NULL**. *expression2* and *expression3* which are returned as a result must be the same or of a convertible common type. If one is explicitly **NULL**, the result of the function follows the type of the non-**NULL** parameter.

    :param expression1: comparison expression
    :param expression2: the value to be returned when *expression1* is true
    :param expression3: the value to be returned when *expression1* is not true
    :rtype: type of *expression2* or *expression3*

**IF**\(*a*, *b*, *c*) has the same meaning as the **CASE** expression in the following example: ::

    CASE WHEN a IS TRUE THEN b
    ELSE c
    END

.. code-block:: sql

    SELECT * FROM case_tbl;
    
::

                a
    =============
                1
                2
                3
             NULL
     
.. code-block:: sql

    --IF function returns the second expression when the first is TRUE
    SELECT a, IF(a=1, 'one', 'other') FROM case_tbl;
    
::

                a   if(a=1, 'one', 'other')
    ===================================
                1  'one'
                2  'other'
                3  'other'
             NULL  'other'
     
.. code-block:: sql

    --If function in WHERE clause
    SELECT * FROM case_tbl WHERE IF(a=1, 1, 2) = 1;
    
::

                a
    =============
                1

IFNULL, NVL
===========

.. function:: IFNULL(expr1, expr2)
.. function:: NVL(expr1, expr2)

    The **IFNULL** function is working like the **NVL** function; however, only the **NVL** function supports collection type as well. The **IFNULL** function (which has two arguments) returns *expr1* if the value of the first expression is not **NULL** or returns *expr2*, otherwise.

    :param expr1: expression
    :param expr2: the value to be returned when *expr1* is **NULL**
    :rtype: determined with the type of *expr1* and *expr2*

Operation is performed by converting the type of every argument into that with the highest priority. If there is an argument whose type cannot be converted, the type of every argument is converted into a **VARCHAR** type. The following list shows priority of conversion based on input argument type.

*   **CHAR** < **VARCHAR**
*   **BIT** < **VARBIT**
*   **SHORT** < **INT** < **BIGINT** < **NUMERIC** < **FLOAT** < **DOUBLE**
*   **DATE** < **TIMESTAMP** < **DATETIME**

For example, if a type of a is **INT** and b is **BIGINT**, then **IFNULL** (a, b) returns a **BIGINT** type. If a type of a is **INTEGER** and b is **TIMESTAMP**, then **IFNULL** (a, b) returns a **VARCHAR** type.

**IFNULL**\(*a*, *b*) or **NVL**\(*a*, *b*) has the same meaning as the **CASE** expression below. ::

    CASE WHEN a IS NULL THEN b
    ELSE a
    END

.. code-block:: sql

    SELECT * FROM case_tbl;
    
::

                a
    =============
                1
                2
                3
             NULL
     
.. code-block:: sql

    --returning a specific value when a is NULL
    SELECT a, NVL(a, 10.0000) FROM case_tbl;
    
::

                a  nvl(a, 10.0000)
    ===================================
                1  1.0000
                2  2.0000
                3  3.0000
             NULL  10.0000
     
.. code-block:: sql

    --IFNULL can be used instead of NVL and return values are converted to the string type
    SELECT a, IFNULL(a, 'UNKNOWN') FROM case_tbl;
    
::

                a   ifnull(a, 'UNKNOWN')
    ===================================
                1  '1'
                2  '2'
                3  '3'
             NULL  'UNKNOWN'

ISNULL
======

.. function:: ISNULL(expression)

    The **ISNULL** function performs a comparison to determine if the result of the expression specified as an argument is **NULL**. The function returns 1 if it is **NULL** or 0 otherwise. You can check if a certain value is **NULL**. This function is working like the **ISNULL** expression.

    :param expression: An arithmetic function that has a single-value column, path expression (ex.: *tbl_name.col_name*), constant value is specified.
    :rtype: INT

.. code-block:: sql

    --Using ISNULL function to select rows with NULL value
    SELECT * FROM condition_tbl WHERE ISNULL(salary);
        
::

               id  name                  dept_name                  salary
    ======================================================================
                7  'Brown     '          'account'                    NULL

LEAST
=====

.. function:: LEAST(expression [, expression] ...)

    The **LEAST** function compares more than one expression specified as parameters and returns the smallest value. If only one expression has been specified, the expression is returned because there is no expression to be compared with.

    Therefore, more than one expression that is specified as parameters must be of the type that can be compared with each other. If the types of the specified parameters are identical, so are the types of the return values; if they are different, the type of the return value becomes a convertible common data type.

    That is, the **LEAST** function compares the values of column 1, column 2 and column 3 in the same row and returns the smallest value while the :func:`MIN` compares the values of column in all result rows and returns the smallest value. 

    :param expression: Specifies more than one expression. Their types must be comparable each other. One of the arguments is **NULL**, **NULL** is returned.
    :rtype: same as that of the argument

The following example shows how to retrieve the number of every medals and the lowest number that Korea won in the *demodb* database.

.. code-block:: sql

    SELECT gold, silver , bronze, LEAST(gold, silver, bronze) FROM participant
    WHERE nation_code = 'KOR';
    
::

             gold       silver       bronze  least(gold, silver, bronze)
    ====================================================================
                9           12            9                            9
                8           10           10                            8
                7           15            5                            5
               12            5           12                            5
               12           10           11                           10

NULLIF
======

.. function:: NULLIF(expr1, expr2)

    The **NULLIF** function returns **NULL** if the two expressions specified as the parameters are identical, and returns the first parameter value otherwise.

    :param expr1: expression to be compared with *expr2*
    :param expr2: expression to be compared with *expr1*
    :rtype: type of *expr1*

**NULLIF** (*a*, *b*) is the same of the **CASE** expression. ::

    CASE
    WHEN a = b THEN NULL
    ELSE a
    END

.. code-block:: sql

    SELECT * FROM case_tbl;
    
::

                a
    =============
                1
                2
                3
             NULL
     
.. code-block:: sql

    --returning NULL value when a is 1
    SELECT a, NULLIF(a, 1) FROM case_tbl;
    
::

                a  nullif(a, 1)
    ===========================
                1          NULL
                2             2
                3             3
             NULL          NULL
     
.. code-block:: sql

    --returning NULL value when arguments are same
    SELECT NULLIF (1, 1.000)  FROM db_root;
    
::

      nullif(1, 1.000)
    ======================
      NULL
     
.. code-block:: sql

    --returning the first value when arguments are not same
    SELECT NULLIF ('A', 'a')  FROM db_root;
    
::

      nullif('A', 'a')
    ======================
      'A'

NVL2
====

.. function:: NVL2(expr1, expr2, expr3)

    Three parameters are specified for the **NVL2** function. The second expression (*expr2*) is returned if the first expression (*expr1*) is not **NULL**; the third expression (*expr3*) is returned if it is **NULL**.

    :param expr1: expression
    :param expr2: the value to be returned when *expr1* is not **NULL**
    :param expr3: the value to be returned when *expr1* is **NULL**
    :rtype: determined with the type of *expr1*, *expr2* and *expr3*

Operation is performed by converting the type of every argument into that with the highest priority. If there is an argument whose type cannot be converted, the type of every argument is converted into a **VARCHAR** type. The following list shows priority of conversion based on input argument type.

*   **CHAR** < **VARCHAR**
*   **BIT** < **VARBIT**
*   **SHORT** < **INT** < **BIGINT** < **NUMERIC** < **FLOAT** < **DOUBLE**
*   **DATE** < **TIMESTAMP** < **DATETIME**

For example, if a type of a is **INT**, b, **BIGINT**, and c, **SHORT**, then **NVL2** (a, b, c) returns a **BIGINT** type. If a type of a is **INTEGER**, b, **DOUBLE**, and c, **TIMESTAMP**, then **NVL2** (a, b, c) returns a **VARCHAR** type.

.. code-block:: sql

    SELECT * FROM case_tbl;
    
::

                a
    =============
                1
                2
                3
             NULL
     
.. code-block:: sql

    --returning a specific value of INT type
    SELECT a, NVL2(a, a+1, 10.5678) FROM case_tbl;
    
::

                a  nvl2(a, a+1, 10.5678)
    ====================================
                1                      2
                2                      3
                3                      4
             NULL                     11
