
:meta-keywords: cubrid any, cubrid some, cubrid all, cubrid between, cubrid exists, cubrid in, cubrid like, cubrid case clause
:meta-description: A comparison expression is an expression that is included in the WHERE clause of the SELECT, UPDATE and DELETE statements, and in the HAVING clause of the SELECT statement.

*********************
Comparison Expression
*********************

.. _basic-cond-expr:

Simple Comparison Expression
============================

A comparison expression is an expression that is included in the **WHERE** clause of the **SELECT**, **UPDATE** and **DELETE** statements, and in the **HAVING** clause of the **SELECT** statement. There are simple comparison, **ANY** / **SOME** / **ALL**, **BETWEEN**, **EXISTS**, **IN** / **NOT IN**, **LIKE** and **IS NULL** comparison expressions, depending on the kinds of the operators combined.

A simple comparison expression compares two comparable data values. Expressions or subqueries are specified as operands, and the comparison expression always returns **NULL** if one of the operands is **NULL**. The following table shows operators that can be used in the simple comparison expressions. For details, see :doc:`/sql/function/comparison_op`.

**Comparison Operators**

+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| Comparison Operator     | Description                                                                | Comparison Expression      | Return Value     |
+=========================+============================================================================+============================+==================+
| **=**                   | A value of left operand is the same as that of right operand.              | 1=2                        | 0                |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| **<>**                  | A value of left operand is not the same as that of right operand.          | 1<>2                       | 1                |
| ,                       |                                                                            |                            |                  |
| **!=**                  |                                                                            |                            |                  |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| **>**                   | A value of left operand is greater than that of right operand.             | 1>2                        | 0                |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| **<**                   | A value of left operand is less than that of right operand.                | 1<2                        | 1                |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| **>=**                  | A value of left operand is equal to or greater than that of right operand. | 1>=2                       | 0                |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| **<=**                  | A value of left operand is equal to or less than that of right operand.    | 1<=2                       | 1                |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+

.. _any-some-all-expr:

ANY/SOME/ALL quantifiers
========================

A comparison expression that includes quantifiers such as **ANY/SOME/ALL** performs comparison operation on one data value and on some or all values included in the list. A comparison expression that includes **ANY** or **SOME** returns **TRUE** if the value of the data on the left satisfies simple comparison with at least one of the values in the list specified as an operand on the right. A comparison expression that includes **ALL** returns **TRUE** if the value of the data on the left satisfies simple comparison with all values in the list on the right.

When a comparison operation is performed on **NULL** in a comparison expression that includes **ANY** or **SOME**, **UNKNOWN** or **TRUE** is returned as a result; when a comparison operation is performed on **NULL** in a comparison expression that includes **ALL**, **UNKNOWN** or **FALSE** is returned. ::

    expression comp_op SOME expression
    expression comp_op ANY expression
    expression comp_op ALL expression

*   *comp_op* : A comparison operator >, = or <= can be used.
*   *expression* (left): A single-value column, path expression (ex.: *tbl_name.col_name*), constant value or arithmetic function that produces a single value can be used.
*   *expression* (right): A column name, path expression, list (set) of constant values or subquery can be used. A list is a set represented within braces ({}). If a subquery is used, *expression* (left) and comparison operation on all results of the subquery execution is performed.

.. code-block:: sql

    --creating a table
     
    CREATE TABLE condition_tbl (id int primary key, name char(10), dept_name VARCHAR, salary INT);
    INSERT INTO condition_tbl VALUES(1, 'Kim', 'devel', 4000000);
    INSERT INTO condition_tbl VALUES(2, 'Moy', 'sales', 3000000);
    INSERT INTO condition_tbl VALUES(3, 'Jones', 'sales', 5400000);
    INSERT INTO condition_tbl VALUES(4, 'Smith', 'devel', 5500000);
    INSERT INTO condition_tbl VALUES(5, 'Kim', 'account', 3800000);
    INSERT INTO condition_tbl VALUES(6, 'Smith', 'devel', 2400000);
    INSERT INTO condition_tbl VALUES(7, 'Brown', 'account', NULL);
     
    --selecting rows where department is sales or devel
    SELECT * FROM condition_tbl WHERE dept_name = ANY{'devel','sales'};
    
::    
    
               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                2  'Moy       '          'sales'                   3000000
                3  'Jones     '          'sales'                   5400000
                4  'Smith     '          'devel'                   5500000
                6  'Smith     '          'devel'                   2400000
     
.. code-block:: sql

    --selecting rows comparing NULL value in the ALL group conditions
    SELECT * FROM condition_tbl WHERE salary > ALL{3000000, 4000000, NULL};

::
    
    There are no results.
     
.. code-block:: sql

    --selecting rows comparing NULL value in the ANY group conditions
    SELECT * FROM condition_tbl WHERE salary > ANY{3000000, 4000000, NULL};

::
    
               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                3  'Jones     '          'sales'                   5400000
                4  'Smith     '          'devel'                   5500000
                5  'Kim       '          'account'                 3800000
     
.. code-block:: sql

    --selecting rows where salary*0.9 is less than those salary in devel department
    SELECT * FROM condition_tbl WHERE (
      (0.9 * salary) < ALL (SELECT salary FROM condition_tbl
      WHERE dept_name = 'devel')
    );

::
    
               id  name                  dept_name                  salary
    ======================================================================
                6  'Smith     '          'devel'                   2400000

.. _between-expr:

BETWEEN
=======

The **BETWEEN** makes a comparison to determine whether the data value on the left exists between two data values specified on the right. It returns **TRUE** even when the data value on the left is the same as a boundary value of the comparison target range. If **NOT** comes before the **BETWEEN** keyword, the result of a **NOT** operation on the result of the **BETWEEN** operation is returned.

*i* **BETWEEN** *g* **AND** *m* and the compound condition *i* **>= g AND** *i* <= *m* have the same effect. 

::

    expression [ NOT ] BETWEEN expression AND expression

*   *expression* : A column name, path expression (ex.: *tbl_name.col_name*), constant value, arithmetic expression or aggregate function can be used. For a character string expression, the conditions are evaluated in alphabetical order. If **NULL** is specified for at least one of the expressions, the **BETWEEN** predicate returns **UNKNOWN** as a result.

.. code-block:: sql

    --selecting rows where 3000000 <= salary <= 4000000
    SELECT * FROM condition_tbl WHERE salary BETWEEN 3000000 AND 4000000;
    SELECT * FROM condition_tbl WHERE (salary >= 3000000) AND (salary <= 4000000);
    
::
    
               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                2  'Moy       '          'sales'                   3000000
                5  'Kim       '          'account'                 3800000
     
.. code-block:: sql

    --selecting rows where salary < 3000000 or salary > 4000000
    SELECT * FROM condition_tbl WHERE salary NOT BETWEEN 3000000 AND 4000000;
    
::

               id  name                  dept_name                  salary
    ======================================================================
                3  'Jones     '          'sales'                   5400000
                4  'Smith     '          'devel'                   5500000
                6  'Smith     '          'devel'                   2400000
     
.. code-block:: sql

    --selecting rows where name starts from A to E
    SELECT * FROM condition_tbl WHERE name BETWEEN 'A' AND 'E';

::

               id  name                  dept_name                  salary
    ======================================================================
                7  'Brown     '          'account'                    NULL

.. _exists-expr:

EXISTS
======

The **EXISTS** returns **TRUE** if one or more results of the execution of the subquery specified on the right exist, and returns **FALSE** if the result of the operation is an empty set. ::

    EXISTS expression

*   *expression* : Specifies a subquery and compares to determine whether the result of the subquery execution exists. If the subquery does not produce any result, the result of the conditional expression is **FALSE**.

.. code-block:: sql

    --selecting rows using EXISTS and subquery
    SELECT 'raise' FROM db_root WHERE EXISTS(
    SELECT * FROM condition_tbl WHERE salary < 2500000);
    
::
    
      'raise'
    ======================
      'raise'
     
.. code-block:: sql

    --selecting rows using NOT EXISTS and subquery
    SELECT 'raise' FROM db_root WHERE NOT EXISTS(
    SELECT * FROM condition_tbl WHERE salary < 2500000);

::

    There are no results.

.. _in-expr:

IN
==

The **IN** compares to determine whether the single data value on the left is included in the list specified on the right. That is, the predicate returns **TRUE** if the single data value on the left is an element of the expression specified on the right. If **NOT** comes before the **IN** keyword, the result of a **NOT** operation on the result of the **IN** operation is returned. ::

    expression [ NOT ] IN expression

*   *expression* (left): A single-value column, path expression (ex.: *tbl_name.col_name*), constant value or arithmetic function that produces a single value can be used.
*   *expression* (right): A column name, path expression, list (set) of constant values or subquery can be used. A list is a set represented within parentheses (()) or braces ({}). If a subquery is used, comparison with expression(left) is performed for all results of the subquery execution.

.. code-block:: sql

    --selecting rows where department is sales or devel
    SELECT * FROM condition_tbl WHERE dept_name IN {'devel','sales'};
    SELECT * FROM condition_tbl WHERE dept_name = ANY{'devel','sales'};
    
::
    
               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                2  'Moy       '          'sales'                   3000000
                3  'Jones     '          'sales'                   5400000
                4  'Smith     '          'devel'                   5500000
                6  'Smith     '          'devel'                   2400000
     
.. code-block:: sql

    --selecting rows where department is neither sales nor devel
    SELECT * FROM condition_tbl WHERE dept_name NOT IN {'devel','sales'};
    
::

               id  name                  dept_name                  salary
    ======================================================================
                5  'Kim       '          'account'                 3800000
                7  'Brown     '          'account'                    NULL

.. _is-null-expr:

IS NULL
=======

The **IS NULL** compares to determine whether the expression specified on the left is **NULL**, and if it is **NULL**, returns **TRUE** and it can be used in the conditional expression. If **NOT** comes before the **NULL** keyword, the result of a **NOT** operation on the result of the **IS NULL** operation is returned.

    expression IS [ NOT ] NULL

*   *expression* : A single-value column, path expression (ex.: *tbl_name.col_name*), constant value or arithmetic function that produces a single value can be used. 

.. code-block:: sql

    --selecting rows where salary is NULL
    SELECT * FROM condition_tbl WHERE salary IS NULL;
    
::
    
               id  name                  dept_name                  salary
    ======================================================================
                7  'Brown     '          'account'                    NULL
     
.. code-block:: sql

    --selecting rows where salary is NOT NULL
    SELECT * FROM condition_tbl WHERE salary IS NOT NULL;
    
::

               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                2  'Moy       '          'sales'                   3000000
                3  'Jones     '          'sales'                   5400000
                4  'Smith     '          'devel'                   5500000
                5  'Kim       '          'account'                 3800000
                6  'Smith     '          'devel'                   2400000
     
.. code-block:: sql

    --simple comparison operation returns NULL when operand is NULL
    SELECT * FROM condition_tbl WHERE salary = NULL;
    
::

    There are no results.

.. _like-expr:

LIKE
====

The **LIKE** compares patterns between character string data, and returns **TRUE** if a character string whose pattern matches the search word is found. Pattern comparison target types are **CHAR**, **VARCHAR** and **STRING**. The **LIKE** search cannot be performed on an **BIT** type. If **NOT** comes before the **LIKE** keyword, the result of a **NOT** operation on the result of the **LIKE** operation is returned.

A wild card string corresponding to any character or character string can be included in the search word on the right of the **LIKE** operator. % (percent) and _ (underscore) can be used. .% corresponds to any character string whose length is 0 or greater, and _ corresponds to one character. An escape character is a character that is used to search for a wild card character itself, and can be specified by the user as another character (**NULL**, alphabet, or number whose length is 1. See below for an example of using a character string that includes wild card or escape characters. ::

    expression [ NOT ] LIKE pattern [ ESCAPE char ]

*   *expression*\ : Specifies the data type column of the character string. Pattern comparison, which is case-sensitive, starts from the first character of the column.
*   *pattern*\ : Enters the search word. A character string with a length of 0 or greater is required. Wild card characters (% or _) can be included as the pattern of the search word. The length of the character string is 0 or greater.
*   **ESCAPE** *char* : **NULL**, alphabet, or number is allowed for *char*. If the string pattern of the search word includes "_" or "%" itself, an ESCAPE character must be specified. For example, if you want to search for the character string "10%" after specifying backslash (\\) as the ESCAPE character, you must specify "10\%" for *pattern*. If you want to search for the character string "C:\\", you can specify "C:\\" for *pattern*.

For details about character sets supported in CUBRID, see :ref:`char-data-type`.

Whether to detect the escape characters of the LIKE conditional expression is determined depending on the configuration of **no_backslash_escapes** and **require_like_escape_character** in the **cubrid.conf** file. For details, see :ref:`stmt-type-parameters`.

.. note::

    *   To execute string comparison operation for data entered in the multibyte charset environment such as UTF-8, the parameter setting (**single_byte_compare** = yes) which compares strings by 1 byte should be added to the **cubrid.conf** file for a successful search result. 

    *   Versions after CUBRID 9.0 support Unicode charset, so the **single_byte_compare** parameter is no longer used.

.. code-block:: sql

    --selection rows where name contains lower case 's', not upper case
    SELECT * FROM condition_tbl WHERE name LIKE '%s%';
    
::

               id  name                  dept_name                  salary
    ======================================================================
                3  'Jones     '          'sales'                   5400000
     
.. code-block:: sql

    --selection rows where second letter is 'O' or 'o'
    SELECT * FROM condition_tbl WHERE UPPER(name) LIKE '_O%';
    
::

               id  name                  dept_name                  salary
    ======================================================================
                2  'Moy       '          'sales'                   3000000
                3  'Jones     '          'sales'                   5400000
     
.. code-block:: sql

    --selection rows where name is 3 characters
    SELECT * FROM condition_tbl WHERE name LIKE '___';
    
::

               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                2  'Moy       '          'sales'                   3000000
                5  'Kim       '          'account'                 3800000

.. _case-expr:

CASE
====

The **CASE** expression uses the SQL statement to perform an **IF** ... **THEN** statement. When a result of comparison expression specified in a **WHEN** clause is true, a value specified in **THEN** clause is returned. A value specified in an **ELSE** clause is returned otherwise. If no **ELSE** clause exists, **NULL** is returned. ::

    CASE control_expression simple_when_list
    [ else_clause ]
    END
     
    CASE searched_when_list
    [ else_clause ]
    END
     
    simple_when :
    WHEN expression THEN result
     
    searched_when :
    WHEN search_condition THEN result
     
    else_clause :
    ELSE result
     
    result :
    expression | NULL

**The CASE** expression must end with the END keyword. A *control_expression* argument and an *expression argument* in *simple_when* expression should be comparable data types. The data types of *result* specified in the **THEN** ... **ELSE** statement should all same, or they can be convertible to common data type.

The data type for a value returned by the **CASE** expression is determined based on the following rules.

*   If data types for result specified in the **THEN** statement are all same, a value with the data type is returned.
*   If data types can be convertible to common data type even though they are not all same, a value with the data type is returned.
*   If any of values for *result* is a variable length string, a value data type is a variable length string. If values for *result* are all a fixed length string, the longest character string or bit string is returned.
*   If any of values for result is an approximate numeric data type, a value with a numeric data type is returned. The number of digits after the decimal point is determined  to display all significant figures.

.. code-block:: sql

    --creating a table
    CREATE TABLE case_tbl( a INT);
    INSERT INTO case_tbl VALUES (1);
    INSERT INTO case_tbl VALUES (2);
    INSERT INTO case_tbl VALUES (3);
    INSERT INTO case_tbl VALUES (NULL);
     
    --case operation with a search when clause
    SELECT a,
           CASE WHEN a=1 THEN 'one'
                WHEN a=2 THEN 'two'
                ELSE 'other'
           END
    FROM case_tbl;
    
::

                a  case when a=1 then 'one' when a=2 then 'two' else 'other' end
    ===================================
                1  'one'
                2  'two'
                3  'other'
             NULL  'other'
     
.. code-block:: sql

    --case operation with a simple when clause
    SELECT a,
           CASE a WHEN 1 THEN 'one'
                  WHEN 2 THEN 'two'
                  ELSE 'other'
           END
    FROM case_tbl;
    
::

                a  case a when 1 then 'one' when 2 then 'two' else 'other' end
    ===================================
                1  'one'
                2  'two'
                3  'other'
             NULL  'other'
     
.. code-block:: sql

    --result types are converted to a single type containing all of significant figures
    SELECT a,
           CASE WHEN a=1 THEN 1
                WHEN a=2 THEN 1.2345
                ELSE 1.234567890
           END
    FROM case_tbl;
    
::

                a  case when a=1 then 1 when a=2 then 1.2345 else 1.234567890 end
    ===================================
                1  1.000000000
                2  1.234500000
                3  1.234567890
             NULL  1.234567890
     
.. code-block:: sql

    --an error occurs when result types are not convertible
    SELECT a,
           CASE WHEN a=1 THEN 'one'
                WHEN a=2 THEN 'two'
                ELSE 1.2345
           END
    FROM case_tbl;
    
::

    ERROR: Cannot coerce 'one' to type double.
