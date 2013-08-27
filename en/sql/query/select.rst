******
SELECT
******

The **SELECT** statement specifies columns that you want to retrieve from a table. ::

    SELECT [ <qualifier> ] <select_expressions>
        [ { TO | INTO } <variable_comma_list> ]
        [ FROM <extended_table_specification_comma_list> ]
        [ WHERE <search_condition> ]
        [ GROUP BY {col_name | expr} [ ASC | DESC ],...[ WITH ROLLUP ] ]
        [ HAVING  <search_condition> ]
        [ ORDER BY {col_name | expr} [ ASC | DESC ],... [ NULLS { FIRST | LAST } ] [ FOR <orderby_for_condition> ] ]
        [ LIMIT [offset,] row_count ]
        [ USING INDEX { index name [,index_name,...] | NONE }]
     
    <qualifier> ::= ALL | DISTINCT | DISTINCTROW | UNIQUE
     
    <select_expressions> ::= * | <expression_comma_list> | *, <expression_comma_list>
    
    <variable_comma_list> ::= [:] identifier, [:] identifier, ...
    
    <extended_table_specification_comma_list> ::=
    <table specification> [ {, <table specification> | <join table specification> }... ]
     
    <table_specification> ::=
     <single_table_spec> [ <correlation> ] [ WITH (<lock_hint>) ]|
     <metaclass_specification> [ <correlation> ] |
     <subquery> <correlation> |
     TABLE ( <expression> ) <correlation>
     
    <correlation> ::= [ AS ] <identifier> [ ( <identifier_comma_list> ) ]
     
    <single_table_spec> ::= [ ONLY ] <table_name> |
                          ALL <table_name> [ EXCEPT <table_name> ]
     
    <metaclass_specification> ::= CLASS <class_name>
     
    <join_table_specification> ::=
    [ INNER | { LEFT | RIGHT } [ OUTER ] ] JOIN <table specification> ON <search condition>
     
    <join_table_specification2> ::= CROSS JOIN <table_specification>
     
    <lock_hint> ::= READ UNCOMMITTED
     
    <orderby_for_condition> ::=
    ORDERBY_NUM() { BETWEEN int AND int } |
        { { = | =< | < | > | >= } int } |
        IN ( int, ...)

*   *qualifier* : A qualifier. When omitted, it is set to **ALL**.

    *   **ALL** : Retrieves all records of the table.
    *   **DISTINCT** : Retrieves only records with unique values without allowing duplicates. **DISTINCT**, **DISTINCTROW**, and **UNIQUE** are used interchangeably.

*   <*select_expressions*> :

    *   \* : By using **SELECT** * statement, you can retrieve all columns from the table specified in the **FROM** clause.
    
    *   *expression_comma_list* : *expression* can be a path expression (ex.: *tbl_name.col_name*), variable or table name. All general expressions including arithmetic operations can also be used. Use a comma (,) to separate each expression in the list. You can specify aliases by using the **AS** keyword for columns or expressions to be queried. Specified aliases are used as column names in **GROUP BY**, **HAVING**, **ORDER BY** and **FOR** clauses. The position index of a column is assigned based on the order in which the column was specified. The starting value is 1.

        As **AVG**, **COUNT**, **MAX**, **MIN**, or **SUM**, an aggregate function that manipulates the retrieved data can also be used in the *expression*. 

*   *table_name*. \*: Specifies the table name and using \* has the same effect as specifying all columns for the given table.

*   *variable_comma_list* : The data retrieved by the *select_expressions* can be stored in more than one variable.

*   [:]\ *identifier* : By using the *:identifier* after **TO** (or **INTO**), you can store the data to be retrieved in the ':identifier' variable.

*   <*single_table_spec*>

    *   If a superclass name is specified after the **ONLY** keyword, only the superclass, not the subclass inheriting from it, is selected.
    *   If a superclass name is specified after the **ALL** keyword, the superclass as well as the subclass inheriting from it are both selected.
    *   You can define the list of subclass not to be selected after the **EXCEPT** keyword.

The following example shows how to retrieve host countries of the Olympic Games without any duplicates. This example is performed on the *olympic* table of *demodb*. The **DISTINCT** or **UNIQUE** keyword makes the query result unique. For example, when there are multiple *olympic* records of which each *host_nation* value is 'Greece', you can use such keywords to display only one value in the query result.

.. code-block:: sql

    SELECT DISTINCT host_nation FROM olympic;
    
::

      host_nation
    ======================
      'Australia'
      'Belgium'
      'Canada'
      'Finland'
      'France'
    ...

The following example shows how to define an alias to a column to be queried and sort the result record by using the column alias in the **ORDER BY** clause. At this time, the number of the result records is limited to 5 by using the **LIMIT** clause and FOR **ORDERBY_NUM()**.

.. code-block:: sql

    SELECT host_year as col1, host_nation as col2 FROM olympic ORDER BY col2 LIMIT 5;
    
::
    
             col1  col2
    ===================================
             2000  'Australia'
             1956  'Australia'
             1920  'Belgium'
             1976  'Canada'
             1948  'England'
     
.. code-block:: sql

    SELECT CONCAT(host_nation, ', ', host_city) AS host_place FROM olympic
    ORDER BY host_place FOR ORDERBY_NUM() BETWEEN 1 AND 5;
    
::
    
      host_place
    ======================
      'Australia,  Melbourne'
      'Australia,  Sydney'
      'Belgium,  Antwerp'
      'Canada,  Montreal'
      'England,  London'

FROM Clause
===========

The **FROM** clause specifies the table in which data is to be retrieved in the query. If no table is referenced, the **FROM** clause can be omitted. Retrieval paths are as follows:

*   Single table
*   Subquery
*   Derived table

::

    SELECT [ <qualifier> ] <select_expressions>
    [ FROM <table_specification> [ {, <table specification>
    | <join table specification> }... ]]
     
     
    <select_expressions> ::= * | <expression_comma_list> | *, <expression_comma_list>
     
    <table_specification> ::=
     <single_table_spec> [ <correlation> ] [ WITH (<lock_hint>) ] |
     <metaclass_specification> [ <correlation> ] |
     <subquery> <correlation> |
     TABLE ( <expression> ) <correlation>
     
    <correlation> ::= [ AS ] <identifier> [ ( <identifier_comma_list> ) ]
     
    <single_table_spec> ::= [ ONLY ] <table_name> |
                          ALL <table_name> [ EXCEPT <table_name> ]
     
    <metaclass_specification> ::= CLASS <class_name>
     
    <lock_hint> ::= READ UNCOMMITTED
    
*   <*select_expressions*> : One or more columns or expressions to query is specified. Use * to query all columns in the table. You can also specify an alias for a column or an expression to be queried by using the AS keyword. This keyword can be used in **GROUP BY**, **HAVING**, **ORDER BY** and **FOR** clauses. The position index of the column is given according to the order in which the column was specified. The starting value is 1.

*   <*table_specification*> : At least one table name is specified after the **FROM** clause. Subqueries and derived tables can also be used in the **FROM** clause. For details on subquery derived tables, see :ref:`subquery-derived-table`.

*   <*lock_hint*> : You can set **READ UNCOMMITTED** for the table isolation level. **READ UNCOMMITTED** is a level where dirty reads are allowed; see :ref:`transaction-isolation-level` For details on the CUBRID transaction isolation level.

.. code-block:: sql

    --FROM clause can be omitted in the statement
    SELECT 1+1 AS sum_value;
    
::

        sum_value
    =============
                2
     
.. code-block:: sql

    SELECT CONCAT('CUBRID', '2008' , 'R3.0') AS db_version;
    
::

      db_version
    ======================
      'CUBRID2008R3.0'

Derived Table
-------------

In the query statement, subqueries can be used in the table specification of the **FROM** clause. Such subqueries create derived tables where subquery results are treated as tables. A correlation specification must be used when a subquery that creates a derived table is used.

Derived tables are also used to access the individual element of an attribute that has a set value. In this case, an element of the set value is created as an instance in the derived table.

.. _subquery-derived-table:

Subquery Derived Table
----------------------

Each instance in the derived table is created from the result of the subquery in the **FROM** clause. A derived table created form a subquery can have any number of columns and records. ::

    FROM (subquery) [[ AS ] derived_table_name [( column_name [ {, column_name } ... ] )]]

*   The number of *column_name* and the number of columns created by the *subquery* must be identical.
*   *derived_table_name* can be omitted.

The following example shows how to retrieve the sum of the number of gold (*gold*) medals won by Korea and that of silver medals won by Japan. This example shows a way of getting an intermediate result of the subquery and processing it as a single result, by using a derived table. The query returns the sum of the *gold* values whose *nation_code* is 'KOR' and the *silver* values whose *nation_code* column is 'JPN'.

.. code-block:: sql

    SELECT SUM (n) 
    FROM (SELECT gold FROM participant WHERE nation_code = 'KOR'
          UNION ALL 
          SELECT silver FROM participant WHERE nation_code = 'JPN') AS t(n);

Subquery derived tables can be useful when combined with outer queries. For example, a derived table can be used in the **FROM** clause of the subquery used in the **WHERE** clause.
The following example shows *nation_code*, *host_year* and *gold* records whose number of gold medals is greater than average sum of the number of silver and bronze medals when one or more silver or bronze medals were won. In this example, the query (the outer **SELECT** clause) and the subquery (the inner **SELECT** clause) share the *nation_code* attribute.

.. code-block:: sql

    SELECT nation_code, host_year, gold
    FROM participant p
    WHERE gold > (SELECT AVG(s)
                  FROM (SELECT silver + bronze
                        FROM participant
                        WHERE nation_code = p.nation_code
                        AND silver > 0
                        AND bronze > 0)
                       AS t(s));
              
::

      nation_code      host_year      gold
    =========================================
      'JPN'                2004         16
      'CHN'                2004         32
      'DEN'                1996          4
      'ESP'                1992         13
    
.. _where-clause:

WHERE Clause
============

In a query, a column can be processed based on conditions. The **WHERE** clause specifies a search condition for data. ::

    WHERE search_condition

    search_condition :
    • comparison_predicate
    • between_predicate
    • exists_predicate
    • in_predicate
    • null_predicate
    • like_predicate
    • quantified predicate
    • set_predicate

The **WHERE** clause specifies a condition that determines the data to be retrieved by *search_condition* or a query. Only data for which the condition is true is retrieved for the query results. (**NULL** value is not retrieved for the query results because it is evaluated as unknown value.)

*   *search_condition* : It is described in detail in the following sections.

    *   :ref:`basic-cond-expr`
    *   :ref:`between-expr`
    *   :ref:`exists-expr`
    *   :ref:`in-expr`
    *   :ref:`is-null-expr`
    *   :ref:`like-expr`
    *   :ref:`any-some-all-expr`

The logical operator **AND** or **OR** can be used for multiple conditions. If **AND** is specified, all conditions must be true. If **OR** is specified, only one needs to be true. If the keyword **NOT** is preceded by a condition, the meaning of the condition is reserved. The following table shows the order in which logical operators are evaluated.

+--------------+--------------+---------------------------------------------------------------+
| Priority     | Operator     | Function                                                      |
+==============+==============+===============================================================+
| 1            | **()**       | Logical expressions in parentheses are evaluated first.       |
+--------------+--------------+---------------------------------------------------------------+
| 2            | **NOT**      | Negates the result of the logical expression.                 |
+--------------+--------------+---------------------------------------------------------------+
| 3            | **AND**      | All conditions in the logical expression must be true.        |
+--------------+--------------+---------------------------------------------------------------+
| 4            | **OR**       | One of the conditions in the logical expression must be true. |
+--------------+--------------+---------------------------------------------------------------+

.. _group-by-clause:

GROUP BY ... HAVING Clause
==========================

The **GROUP BY** clause is used to group the result retrieved by the **SELECT** statement based on a specific column. This clause is used to sort by group or to get the aggregation by group using the aggregation function. Herein, a group consists of records that have the same value for the column specified in the **GROUP BY** clause.

You can also set a condition for group selection by including the **HAVING** clause after the **GROUP BY** clause. That is, only groups satisfying the condition specified by the **HAVING** clause are queried out of all groups that are grouped by the **GROUP BY** clause.

By SQL standard, you cannot specify a column (hidden column) not defined in the **GROUP BY** clause to the SELECT column list. However, by using extended CUBRID grammars, you can specify the hidden column to the SELECT column list. If you do not use the extended CUBRID grammars, the **only_full_group_by** parameter should be set to **yes**. For details, see :ref:`stmt-type-parameters`. ::

    SELECT ...
    GROUP BY { col_name | expr | position } [ ASC | DESC ],...
              [ WITH ROLLUP ][ HAVING <search_condition> ]

*   *col_name* | *expr* | *position* : Specifies one or more column names, expressions, aliases or column location. Items are separated by commas. Columns are sorted on this basis.

*   [ **ASC** | **DESC** ] : Specifies the **ASC** or **DESC** sorting option after the columns specified in the **GROUP BY** clause. If the sorting option is not specified, the default value is **ASC**.

*   <*search_condition*> : Specifies the search condition in the **HAVING** clause. In the **HAVING** clause you can refer to the hidden columns not specified in the **GROUP BY** clause as well as to columns and aliases specified in the **GROUP BY** clause and columns used in aggregate functions.

*   **WITH ROLLUP** : If you specify the **WITH ROLLUP** modifier in the **GROUP BY** clause, the aggregate information of the result value of each GROUPed BY column is displayed for each group, and the total of all result rows is displayed at the last row. When a **WITH ROLLUP** modifier is defined in the **GROUP BY** clause, the result value for all rows of the group is additionally displayed. In other words, total aggregation is made for the value aggregated by group. When there are two columns for Group By, the former is considered as a large unit and the latter is considered as a small unit, so the total aggregation row for the small unit and the total aggregation row for the large unit are added. For example, you can check the aggregation of the sales result per department and salesperson through one query.

.. code-block:: sql

    -- creating a new table
    CREATE TABLE sales_tbl
    (dept_no INT, name VARCHAR(20), sales_month INT, sales_amount INT DEFAULT 100, PRIMARY KEY (dept_no, name, sales_month));
    
    INSERT INTO sales_tbl VALUES
    (201, 'George' , 1, 450), (201, 'George' , 2, 250), (201, 'Laura'  , 1, 100), (201, 'Laura'  , 2, 500),
    (301, 'Max'    , 1, 300), (301, 'Max'    , 2, 300),
    (501, 'Stephan', 1, 300), (501, 'Stephan', 2, DEFAULT), (501, 'Chang'  , 1, 150),(501, 'Chang'  , 2, 150),
    (501, 'Sue'    , 1, 150), (501, 'Sue'    , 2, 200);
     
    -- selecting rows grouped by dept_no
    SELECT dept_no, avg(sales_amount) 
    FROM sales_tbl
    GROUP BY dept_no;
    
::

          dept_no         avg(sales_amount)
    =======================================
              201     3.250000000000000e+02
              301     3.000000000000000e+02
              501     1.750000000000000e+02
    
.. code-block:: sql

    -- conditions in WHERE clause operate first before GROUP BY
    SELECT dept_no, avg(sales_amount) 
    FROM sales_tbl
    WHERE sales_amount > 100 
    GROUP BY dept_no;
    
::

          dept_no         avg(sales_amount)
    =======================================
              201     4.000000000000000e+02
              301     3.000000000000000e+02
              501     1.900000000000000e+02
     
.. code-block:: sql

    -- conditions in HAVING clause operate last after GROUP BY
    SELECT dept_no, avg(sales_amount) 
    FROM sales_tbl
    WHERE sales_amount > 100 
    GROUP BY dept_no HAVING avg(sales_amount) > 200;
    
::

          dept_no         avg(sales_amount)
    =======================================
              201     4.000000000000000e+02
              301     3.000000000000000e+02
     
.. code-block:: sql

    -- selecting and sorting rows with using column alias
    SELECT dept_no AS a1, avg(sales_amount) AS a2 
    FROM sales_tbl
    WHERE sales_amount > 200 GROUP 
    BY a1 HAVING a2 > 200 
    ORDER BY a2;
    
::

               a1                        a2
    =======================================
              301     3.000000000000000e+02
              501     3.000000000000000e+02
              201     4.000000000000000e+02
     
.. code-block:: sql

    -- selecting rows grouped by dept_no, name with WITH ROLLUP modifier
    SELECT dept_no AS a1, name AS a2, avg(sales_amount) AS a3 
    FROM sales_tbl
    WHERE sales_amount > 100 
    GROUP BY a1, a2 WITH ROLLUP;
    
::

               a1  a2                                          a3
    =============================================================
              201  'George'                 3.500000000000000e+02
              201  'Laura'                  5.000000000000000e+02
              201  NULL                     4.000000000000000e+02
              301  'Max'                    3.000000000000000e+02
              301  NULL                     3.000000000000000e+02
              501  'Chang'                  1.500000000000000e+02
              501  'Stephan'                3.000000000000000e+02
              501  'Sue'                    1.750000000000000e+02
              501  NULL                     1.900000000000000e+02
             NULL  NULL                     2.750000000000000e+02

.. _order-by-clause:

ORDER BY Clause
===============

The **ORDER BY** clause sorts the query result set in ascending or descending order. If you do not specify a sorting option such as **ASC** or **DESC**, the result set in ascending order by default. If you do not specify the **ORDER BY** clause, the order of records to be queried may vary depending on query. ::

    SELECT ...
    ORDER BY { col_name | expr | position } [ ASC | DESC ], ...] [ NULLS { FIRST | LAST } ]
        [ FOR <orderby_for_condition> ] ]
     
    <orderby_for_condition> ::=
    ORDERBY_NUM() { BETWEEN bigint AND bigint } |
        { { = | =< | < | > | >= } bigint } |
        IN (bigint, ...)

*   *col_name* | *expr* | *position* : Specifies a column name, expression, alias, or column location. One or more column names, expressions or aliases can be specified. Items are separated by commas. A column that is not specified in the list of **SELECT** columns can be specified.

*   [ **ASC** | **DESC** ] : **ASC** means sorting in ascending order, and **DESC** is sorting in descending order. If the sorting option is not specified, the default value is **ASC**.

*   [ **NULLS** { **FIRST** | **LAST** } ] : **NULLS FIRST** sorts NULL at first, **NULLS LAST** sorts NULL at last. If this syntax is omitted, **ASC** sorts NULL at first, **DESC** sorts NULL at last.

.. code-block:: sql

    -- selecting rows sorted by ORDER BY clause
    SELECT * 
    FROM sales_tbl
    ORDER BY dept_no DESC, name ASC;
    
::

          dept_no  name                  sales_month  sales_amount
    ==============================================================
              501  'Chang'                         1           150
              501  'Chang'                         2           150
              501  'Stephan'                       1           300
              501  'Stephan'                       2           100
              501  'Sue'                           1           150
              501  'Sue'                           2           200
              301  'Max'                           1           300
              301  'Max'                           2           300
              201  'George'                        1           450
              201  'George'                        2           250
              201  'Laura'                         1           100
              201  'Laura'                         2           500
     
.. code-block:: sql

    -- sorting reversely and limiting result rows by LIMIT clause
    SELECT dept_no AS a1, avg(sales_amount) AS a2 
    FROM sales_tbl
    GROUP BY a1
    ORDER BY a2 DESC
    LIMIT 0, 3;
    
::

               a1           a2
    =======================================
              201     3.250000000000000e+02
              301     3.000000000000000e+02
              501     1.750000000000000e+02
     
.. code-block:: sql

    -- sorting reversely and limiting result rows by FOR clause
    SELECT dept_no AS a1, avg(sales_amount) AS a2 
    FROM sales_tbl
    GROUP BY a1
    ORDER BY a2 DESC FOR ORDERBY_NUM() BETWEEN 1 AND 3;
    
::

               a1           a2
    =======================================
              201     3.250000000000000e+02
              301     3.000000000000000e+02
              501     1.750000000000000e+02

The following is an example how to specify the NULLS FIRST or NULLS LAST after ORDER BY clause.

.. code-block:: sql

    CREATE TABLE tbl (a INT, b VARCHAR);

    INSERT INTO tbl VALUES
    (1,NULL), (2,NULL), (3,'AB'), (4,NULL), (5,'AB'), 
    (6,NULL), (7,'ABCD'), (8,NULL), (9,'ABCD'), (10,NULL);

.. code-block:: sql
    
    SELECT * FROM tbl ORDER BY b NULLS FIRST;

::

                a  b
    ===================================
                1  NULL
                2  NULL
                4  NULL
                6  NULL
                8  NULL
               10  NULL
                3  'ab'
                5  'ab'
                7  'abcd'
                9  'abcd'
    
.. code-block:: sql

    SELECT * FROM tbl ORDER BY b NULLS LAST;

::

                a  b
    ===================================
                3  'ab'
                5  'ab'
                7  'abcd'
                9  'abcd'
                1  NULL
                2  NULL
                4  NULL
                6  NULL
                8  NULL
               10  NULL

.. _limit-clause:

LIMIT Clause
============

The **LIMIT** clause can be used to limit the number of records displayed. You can specify a very big integer for *row_count* to display to the last row, starting from a specific row. The **LIMIT** clause can be used as a prepared statement. In this case, the bind parameter (?) can be used instead of an argument.

**INST_NUM** () and **ROWNUM** cannot be included in the **WHERE** clause in a query that contains the **LIMIT** clause. Also, **LIMIT** cannot be used together with FOR **ORDERBY_NUM** () or **HAVING GROUPBY_NUM** (). 

::

    LIMIT { [offset,] row_count | row_count [ OFFSET offset ] }

*   *offset* : Specifies the offset value of the starting row to be displayed. The offset value of the starting row of the result set is 0; it can be omitted and the default value is **0**.
*   *row_count* : Specifies the number of records to be displayed. You can specify an integer greater than 0.

... code-block:: sql

    -- LIMIT clause can be used in prepared statement
    PREPARE stmt FROM 'SELECT * FROM sales_tbl LIMIT ?, ?';
    EXECUTE stmt USING 0, 10;
     
    -- selecting rows with LIMIT clause
    SELECT * 
    FROM sales_tbl
    WHERE sales_amount > 100
    LIMIT 5;
    
::

          dept_no  name                  sales_month  sales_amount
    ==============================================================
              201  'George'                        1           450
              201  'George'                        2           250
              201  'Laura'                         2           500
              301  'Max'                           1           300
              301  'Max'                           2           300
     
.. code-block:: sql

    -- LIMIT clause can be used in subquery
    SELECT t1.*
    FROM (SELECT * FROM sales_tbl AS t2 WHERE sales_amount > 100 LIMIT 5) AS t1
    LIMIT 1,3;
    
    -- above query and below query shows the same result
    SELECT t1.*
    FROM (SELECT * FROM sales_tbl AS t2 WHERE sales_amount > 100 LIMIT 5) AS t1
    LIMIT 3 OFFSET 1;
    
::

          dept_no  name                  sales_month  sales_amount
    ==============================================================
              201  'George'                        2           250
              201  'Laura'                         2           500
              301  'Max'                           1           300

.. _join-query:
              
Join Query
==========

A join is a query that combines the rows of two or more tables or virtual tables (views). In a join query, a condition that compares the columns that are common in two or more tables is called a join condition. Rows are retrieved from each joined table, and are combined only when they satisfy the specified join condition.

A join query using an equality operator (=) is called an equi-join, and one without any join condition is called a cartesian product. Meanwhile, joining a single table is called a self join. In a self join, table **ALIAS** is used to distinguish columns, because the same table is used twice in the **FROM** clause.

A join that outputs only rows that satisfy the join condition from a joined table is called an inner or a simple join, whereas a join that outputs both rows that satisfy and do not satisfy the join condition from a joined table is called an outer join. An outer join is divided into a left outer join which outputs all rows of the left table as a result, a right outer join which outputs all rows of the right table as a result and a full outer join which outputs all rows of both tables. If there is no column value that corresponds to a table on one side in the result of an outer join query, all rows are returned as **NULL**. ::

    FROM table_specification [{, table_specification | { join_table_specification | join_table_specification2 }...]
     
    table_specification :
    table_specification [ correlation ]
    CLASS table_name [ correlation ]
    subquery correlation
    TABLE (expression) correlation
     
    join_table_specification :
    [ INNER | {LEFT | RIGHT} [ OUTER ] ] JOIN table_specification ON search_condition
     
    join_table_specification2 :
    CROSS JOIN table_specification

*   *join_table_specification*

    *   [ **INNER** ] **JOIN** : Used for inner join and requires join conditions.

    *   { **LEFT** | **RIGHT** } [ **OUTER** ] **JOIN** : **LEFT** is used for a left outer join query, and **RIGHT** is for a right outer join query.

    *   **CROSS JOIN** : Used for cross join and requires no join conditions.

The inner join requires join conditions. The **INNER JOIN** keyword can be omitted. When it is omitted, the table is separated by a comma (,). The **ON** join condition can be replaced with the **WHERE** condition.

CUBRID does not support full outer joins; it supports only left and right joins. Path expressions that include subqueries and sub-columns cannot be used in the join conditions of an outer join.

Join conditions of an outer join are specified in a different way from those of an inner join. In an inner join, join conditions can be expressed in the **WHERE** clause; in an outer join, they appear after the **ON** keyword within the **FROM** clause. Other retrieval conditions can be used in the **WHERE** or **ON** clause, but the retrieval result depends on whether the condition is used in the **WHERE** or **ON** clause.

The table execution order is fixed according to the order specified in the **FROM** clause. Therefore, when using an outer join, you should create a query statement in consideration of the table order. It is recommended to use standard statements using { **LEFT** | **RIGHT** } [ **OUTER** ] **JOIN**, because using an Oracle-style join query statements by specifying an outer join operator (**+**) in the **WHERE** clause, even if possible, might lead the execution result or plan in an unwanted direction.

The cross join is a cartesian product, meaning that it is a combination of two tables, without any condition. For the cross join, the **CROSS JOIN** keyword can be omitted. When it is omitted, the table is separated by a comma (,).

The following example shows how to retrieve the years and host countries of the Olympic Games since 1950 where a world record has been set. The following query retrieves instances whose values of the *host_year* column in the *history* table are greater than 1950. The following two queries output the same result.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h INNER JOIN olympic o ON h.host_year = o.host_year AND o.host_year > 1950;
     
    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h, olympic o
    WHERE h.host_year = o.host_year AND o.host_year > 1950;
     
::

        host_year  host_nation
    ===================================
             1968  'Mexico'
             1980  'U.S.S.R.'
             1984  'United States of America'
             1988  'Korea'
             1992  'Spain'
             1996  'United States of America'
             2000  'Australia'
             2004  'Greece'

The following example shows how to retrieve the years and host countries of the Olympic Games since 1950 where a world record has been set, but including the Olympic Games where any world records haven't been set in the result. This example can be expressed in the following right outer join query. In this example, all instances whose values of the *host_year* column in the *history* table are not greater than 1950 are also retrieved. All instances of *host_nation* are included because this is a right outer join. *host_year* that does not have a value is represented as **NULL**.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_nation
    FROM history h RIGHT OUTER JOIN olympic o ON h.host_year=o.host_year 
    WHERE o.host_year>1950;
     
::

        host_year  host_nation
    ===================================
             NULL  'Australia'
             NULL  'Canada'
             NULL  'Finland'
             NULL  'Germany'
             NULL  'Italy'
             NULL  'Japan'
             1968  'Mexico'
             1980  'U.S.S.R.'
             1984  'United States of America'
             1988  'Korea'
             1992  'Spain'
             1996  'United States of America'
             2000  'Australia'
             2004  'Greece'

A right outer join query can be converted to a left outer join query by switching the position of two tables in the **FROM** clause. The right outer join query in the previous example can be expressed as a left outer join query as follows:

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_nation
    FROM olympic o LEFT OUTER JOIN history h ON h.host_year=o.host_year 
    WHERE o.host_year>1950;
     
::

        host_year  host_nation
    ===================================
             NULL  'Australia'
             NULL  'Canada'
             NULL  'Finland'
             NULL  'Germany'
             NULL  'Italy'
             NULL  'Japan'
             1968  'Mexico'
             1980  'U.S.S.R.'
             1984  'United States of America'
             1988  'Korea'
             1992  'Spain'
             1996  'United States of America'
             2000  'Australia'
             2004  'Greece'

In this example, *h.host_year=o.host_year* is an outer join condition, and *o.host_year > 1950* is a search condition. If the search condition is used not in the **WHERE** clause but in the **ON** clause, the meaning and the result will be different. The following query also includes instances whose values of *o.host_year* are not greater than 1950.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_nation
    FROM olympic o LEFT OUTER JOIN history h ON h.host_year=o.host_year AND o.host_year>1950;
     
::

        host_year  host_nation
    ===================================
             NULL  'Australia'
             NULL  'Belgium'
             NULL  'Canada'
    ...
             1996  'United States of America'
             2000  'Australia'
             2004  'Greece'

Outer joins can also be represented by using **(+)** in the **WHERE** clause. The above example is a query that has the same meaning as the example using the **LEFT** **OUTER** **JOIN**. The **(+)** syntax is not ISO/ANSI standard, so it can lead to ambiguous situations. It is recommended to use the standard syntax **LEFT** **OUTER** **JOIN** (or **RIGHT** **OUTER** **JOIN**) if possible.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h, olympic o
    WHERE o.host_year=h.host_year(+) AND o.host_year>1950;
     
::

        host_year  host_nation
    ===================================
             NULL  'Australia'
             NULL  'Canada'
             NULL  'Finland'
             NULL  'Germany'
             NULL  'Italy'
             NULL  'Japan'
             1968  'Mexico'
             1980  'U.S.S.R.'
             1984  'United States of America'
             1988  'Korea'
             1992  'Spain'
             1996  'United States of America'
             2000  'Australia'
             2004  'Greece'

The following example shows how to write cross join. The following two queries will output the same results.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h CROSS JOIN olympic o;
     
    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h, olympic o;
     
::

    host_year  host_nation
    ===================================
             1968  'Australia'
             1968  'Belgium'
             1968  'Canada'
             1968  'England'
             1968  'Finland'
             1968  'France'
             1968  'Germany'
    ...
             2004  'Spain'
             2004  'Sweden'
             2004  'USA'
             2004  'USSR'
             2004  'United Kingdom'

Subquery
========

A subquery can be used wherever expressions such as **SELECT** or **WHERE** clause can be used. If the subquery is represented as an expression, it must return a single column; otherwise it can return multiple rows. Subqueries can be divided into single-row subquery and multiple-row subquery depending on how they are used.

Single-Row Subquery
-------------------

A single-row subquery outputs a row that has a single column. If no row is returned by the subquery, the subquery expression has a **NULL** value. If the subquery is supposed to return more than one row, an error occurs.

The following example shows how to retrieve the *history* table as well as the host country where a new world record has been set. This example shows a single-row subquery used as an expression. In this example, the subquery returns *host_nation* values for the rows whose values of the *host_year* column in the *olympic* table are the same as those of the *host_year* column in the *history* table. If there are no values that meet the condition, the result of the subquery is **NULL**.

.. code-block:: sql

    SELECT h.host_year, (SELECT host_nation FROM olympic o WHERE o.host_year=h.host_year) AS host_nation,
           h.event_code, h.score, h.unit 
    FROM history h;
    
::

        host_year  host_nation            event_code  score                 unit
    ============================================================================================
        2004       'Greece'               20283       '07:53.0'             'time'
        2004       'Greece'               20283       '07:53.0'             'time'
        2004       'Greece'               20281       '03:57.0'             'time'
        2004       'Greece'               20281       '03:57.0'             'time'
        2004       'Greece'               20281       '03:57.0'             'time'
        2004       'Greece'               20281       '03:57.0'             'time'
        2004       'Greece'               20326       '210'                 'kg'
        2000       'Australia'            20328       '225'                 'kg'
        2004       'Greece'               20331       '237.5'               'kg'
    ...

Multiple-Row Subquery
---------------------

The multiple-row subquery returns one or more rows that contain the specified column. The result of the multiple-row subquery can create **SET**, **MULTISET** and **LIST**) by using an appropriate keyword.

The following example shows how to retrieve nations, capitals and host cities for Olympic Game all together in the *nation* table. In this example, the subquery result is used to create a **List** from the values of the *host_city* column in the *olympic* table. This query returns *name* and *capital* value for *nation* table, as well as a set that contains *host_city* values of the *olympic* table with *host_nation* value. If the *name* value is an empty set in the query result, it is excluded. If there is no *olympic* table that has the same value as the *name*, an empty set is returned.

.. code-block:: sql

    SELECT name, capital, list(SELECT host_city FROM olympic WHERE host_nation = name) AS host_cities
    FROM nation;
    
::

      name                      capital                 host_cities
    ==================================================================
      'Somalia'                   'Mogadishu'           {}
      'Sri Lanka'                 'Sri Jayewardenepura Kotte' {}
      'Sao Tome & Principe'       'Sao Tome'            {}
      ...
      'U.S.S.R.'                  'Moscow'              {'Moscow'}
      'Uruguay'                   'Montevideo'          {}
      'United States of America'  'Washington.D.C'      {'Atlanta ', 'St. Louis', 'Los Angeles', 'Los Angeles'}
      'Uzbekistan'                'Tashkent'            {}
      'Vanuatu'                   'Port Vila'           {}

Such multiple-row subquery expressions can be used anywhere a collection-type value expression is allowed. However, they cannot be used where a collection-type constant value is required as in the **DEFAULT** specification in the class attribute definition.

If the **ORDER BY** clause is not used explicitly in the subquery, the order of the multiple-row query result is not set. Therefore, the order of the multiple-row subquery result that creates **LIST** must be specified by using the **ORDER BY** clause.

VALUES
======

The **VALUES** clause prints out the values of rows defined in the expression. In most cases, the **VALUES** clause is used for creating a constant table, however, the clause itself can be used. When one or more rows are specified in the **VALUES** clause, all rows should have the same number of the elements.

    VALUES (expression[, ...])[, ...]
    
*   *expression* : An expression enclosed within parentheses stands for one row in a table.

The **VALUES** clause can be used to express the **UNION** query, which consists of constant values in a simpler way. For example, the following query can be executed.

.. code-block:: sql

    VALUES (1 AS col1, 'first' AS col2), (2, 'second'), (3, 'third'), (4, 'fourth');

The above query prints out the following result.

.. code-block:: sql

    SELECT 1 AS col1, 'first' AS col2
    UNION ALL
    SELECT 2, 'second'
    UNION ALL
    SELECT 3, 'third'
    UNION ALL
    SELECT 4, 'forth';

The following example shows use of the **VALUES** clause with multiple rows in the **INSERT** statement.

.. code-block:: sql

    INSERT INTO athlete (code, name, gender, nation_code, event)
    VALUES ('21111', 'Jang Mi-Ran ', 'F', 'KOR', 'Weight-lifting'),
           ('21112', 'Son Yeon-Jae ', 'F', 'KOR', 'Rhythmic gymnastics');

The following example shows how to use subquery in the **FROM** statement.

.. code-block:: sql
    
    SELECT a.*
    FROM athlete a, (VALUES ('Miran Jang', 'F'), ('Yeonjae Son', 'F')) AS t(name, gender)
    WHERE a.name=t.name AND a.gender=t.gender;
     
::

             code  name                gender   nation_code        event
    =====================================================================================================
            21111  'Jang Mi-ran'       'F'      'KOR'              'Weight-lifting'
            21112  'Son Yeon-Jae'      'F'      'KOR'              'Rhythmic gymnastics'
