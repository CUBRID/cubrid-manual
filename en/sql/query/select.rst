
:meta-keywords: select statement, select from clause, select where clause, group by, having clause, limit clause, join query, subquery, select for update
:meta-description: The SELECT statement specifies columns that you want to retrieve from a table.

******
SELECT
******

The **SELECT** statement specifies columns that you want to retrieve from a table. ::

    SELECT [ <qualifier> ] <select_expressions>
        [{TO | INTO} <variable_comma_list>]
        [FROM <extended_table_specification_comma_list>]
        [WHERE <search_condition>]
        [GROUP BY {col_name | expr} [ASC | DESC], ...[WITH ROLLUP]]
        [HAVING  <search_condition> ]
        [USING INDEX { index_name [,index_name, ...] | NONE }]
        [ORDER BY {col_name | expr} [ASC | DESC], ... [NULLS {FIRST | LAST}]
        [LIMIT [offset,] row_count]        
        [FOR UPDATE [OF <spec_name_comma_list>]]
        
        <qualifier> ::= ALL | DISTINCT | DISTINCTROW | UNIQUE
    
        <select_expressions> ::= * | <expression_comma_list> | *, <expression_comma_list>
     
        <variable_comma_list> ::= [:] identifier, [:] identifier, ...
    
        <extended_table_specification_comma_list> ::=
            <table_specification>   [   
                                        {, <table_specification> } ... |
                                        <join_table_specification> ... |
                                        <join_table_specification2> ...
                                    ]
     
    <table_specification> ::=
        <single_table_spec> [<correlation>] |
        <metaclass_specification> [ <correlation> ] |
        <subquery> <correlation> |
        TABLE ( <expression> ) <correlation>

    <correlation> ::= [AS] <identifier> [(<identifier_comma_list>)]
     
    <single_table_spec> ::= [ONLY] [schema_name.]table_name |
                          ALL [schema_name.]table_name [ EXCEPT [schema_name.]table_name ]
     
    <metaclass_specification> ::= CLASS [schema_name.]class_name
     
    <join_table_specification> ::=
        {
            [INNER | {LEFT | RIGHT} [OUTER]] JOIN 

        } <table_specification> ON <search_condition>
     
    <join_table_specification2> ::= 
        { 
            CROSS JOIN | 
            NATURAL [ LEFT | RIGHT ] JOIN 
        } <table_specification>
    

*   *qualifier*: A qualifier. When omitted, it is set to **ALL**.

    *   **ALL**: Retrieves all records of the table.
    *   **DISTINCT**: Retrieves only records with unique values without allowing duplicates. **DISTINCT**, **DISTINCTROW**, and **UNIQUE** are used interchangeably.

*   <*select_expressions*>

    *   \*: By using **SELECT** * statement, you can retrieve all columns from the table specified in the **FROM** clause.

    *   *expression_comma_list*: *expression* can be a path expression (ex.: *tbl_name.col_name*), variable or table name. All general expressions including arithmetic operations can also be used. Use a comma (,) to separate each expression in the list. You can specify aliases by using the **AS** keyword for columns or expressions to be queried. Specified aliases are used as column names in **GROUP BY**, **HAVING** and **ORDER BY** clauses. The position index of a column is assigned based on the order in which the column was specified. The starting value is 1.

        As **AVG**, **COUNT**, **MAX**, **MIN**, or **SUM**, an aggregate function that manipulates the retrieved data can also be used in the *expression*. 

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.

*   *table_name*.\*: Specifies the table name and using \* has the same effect as specifying all columns for the given table.

*   *variable_comma_list*: The data retrieved by the *select_expressions* can be stored in more than one variable.

*   [:]\ *identifier*: By using the *:identifier* after **TO** (or **INTO**), you can store the data to be retrieved in the ':identifier' variable.

*   <*single_table_spec*>

    *   If a superclass name is specified after the **ONLY** keyword, only the superclass, not the subclass inheriting from it, is selected.
    *   If a superclass name is specified after the **ALL** keyword, the superclass as well as the subclass inheriting from it are both selected.
    *   You can define the list of subclass not to be selected after the **EXCEPT** keyword.

The following example shows how to retrieve host countries of the Olympic Games without any duplicates. This example is performed on the *olympic* table of *demodb*. The **DISTINCT** or **UNIQUE** keyword makes the query result unique. For example, when there are multiple *olympic* records of which each *host_nation* value is 'Greece', you can use such keywords to display only one value in the query result.

.. code-block:: sql

    SELECT DISTINCT host_nation 
    FROM olympic;

::

      host_nation
    ======================
      'Australia'
      'Belgium'
      'Canada'
      'Finland'
      'France'
    ...

The following example shows how to define an alias to a column to be queried and sort the result record by using the column alias in the **ORDER BY** clause. At this time, the number of the result records is limited to 5 by using the **LIMIT** clause.

.. code-block:: sql

    SELECT host_year as col1, host_nation as col2 
    FROM olympic 
    ORDER BY col2 LIMIT 5;
    
::
    
             col1  col2
    ===================================
             2000  'Australia'
             1956  'Australia'
             1920  'Belgium'
             1976  'Canada'
             1948  'England'
     
.. code-block:: sql

    SELECT CONCAT(host_nation, ', ', host_city) AS host_place 
    FROM olympic
    ORDER BY host_place LIMIT 5;
    
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
*   Remote table

::

    SELECT [<qualifier>] <select_expressions>
    [
        FROM <table_specification> [ {, <table_specification> | <join_table_specification> }... ]
    ]
     
    <select_expressions> ::= * | <expression_comma_list> | *, <expression_comma_list>
     
    <table_specification> ::=
        <single_table_spec> [<correlation>] |
        <metaclass_specification> [<correlation>] |
        <subquery> <correlation> |
        TABLE (<expression>) <correlation> |
        DBLINK (<dblink_expr>) <dblink_identifier_col_attrs>
     
    <correlation> ::= [AS] <identifier> [(<identifier_comma_list>)]
     
    <single_table_spec> ::= [ONLY] [schema_name.]table_name |
                          ALL [schema_name.]table_name [EXCEPT [schema_name.]table_name]
     
    <metaclass_specification> ::= CLASS [schema_name.]class_name
     

*   <*select_expressions*>: One or more columns or expressions to query is specified. Use * to query all columns in the table. You can also specify an alias for a column or an expression to be queried by using the AS keyword. This keyword can be used in **GROUP BY**, **HAVING** and **ORDER BY** clauses. The position index of the column is given according to the order in which the column was specified. The starting value is 1.

*   <*table_specification*>: At least one table name is specified after the **FROM** clause. Subqueries and derived tables can also be used in the **FROM** clause. For details on subquery derived tables, see :ref:`subquery-derived-table`.

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

Each instance in the derived table is created from the result of the subquery in the **FROM** clause. A derived table created form a subquery can have any number of columns and records. 

::

    FROM (subquery) [AS] [derived_table_name [(column_name [{, column_name } ... ])]]

*   The number of *column_name* and the number of columns created by the *subquery* must be identical.
*   *derived_table_name* can be omitted.

The following example shows how to retrieve the sum of the number of gold (*gold*) medals won by Korea and that of silver medals won by Japan. This example shows a way of getting an intermediate result of the subquery and processing it as a single result, by using a derived table. The query returns the sum of the *gold* values whose *nation_code* is 'KOR' and the *silver* values whose *nation_code* column is 'JPN'.

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


.. _dblink-clause:

Remote Table
------------

A remote table can be specified in the FROM clause, and when specifying a remote table, '@' is used and a table extension such as table_name@server_name is used. The remote server can use not only CUBRID but also other DBMS (ORACLE, MySQL, and MariaDB). The query for remote table is recreated and executed using the DBLINK statement while going through an optimization step.

.. code-block:: sql

   -- at remote-side, "remote_server"
   CREATE TABLE remote_tbl (
     id INT,
     name VARCHAR(32)
   );

   INSERT INTO remote_tbl VALUES (1, 'Kim');
   INSERT INTO remote_tbl VALUES (2, 'Lee');
   INSERT INTO remote_tbl VALUES (3, 'Park');

::

   -- at local-side
   SELECT *
   FROM remote_tbl@remote_server rem
   WHERE id < 3;

::

       id       name
   ===================
        1       Kim
        2       Lee

The query rewritten with the DBLINK statement for the remote table in the optimization stage is as follows.

.. code-block:: sql

   SELECT *
   FROM DBLINK(remote_server, 'SELECT id, name FROM remote_tbl WHERE id < 3') AS dbl (id INT, name VARCHAR(32));

.. note::

    Objects allowed for table extensions include general tables, synonyms, and views. The example below shows three types of table extensions.

.. code-block:: sql

    -- at remote-side
    CREATE TABLE remote_table (
      id INT,
      phone VARCHAR(12)
    };

    CREATE SYNONYM a_remote_tbl FOR user_a.remote_table
    CREATE VIEW v_remote_tbl(r_phone) AS SELECT phone FROM remote_tble WHERE id > 10;

    -- at local-side

    -- remote-table
    SELECT phone FROM user_a.remote_table@server1 WHERE id > 10;

    -- remote-synonym
    SELECT phone FROM a_remote_tbl@server1 WHERE id > 10;

    -- remote-view
    SELECT r_phone FROM v_remote_tbl@server1;

All three queries above return the same result.

DBLINK
--------

The result can be obtained by executing a query in a separate DBMS located at a remote location. The result is a kind of subquery that is created as :ref:`Subquery Derived Table <subquery-derived-table>`\.

::

    FROM DBLINK (<dblink_expr>) [AS] <dblink_identifier_col_attrs> 

        <dblink_expr> ::= <dblink_conn>,  remote_query_sting  
        <dblink_conn> ::= server_name | dblink_conn_string
            
        <dblink_identifier_col_attrs> ::= dblink_table_alias ( <dblink_column_definition_list> ) 
        <dblink_column_definition_list> ::= dblink_column_alias <primitive_type> [{, dblink_column_alias <primitive_type>} ...]

*   *remote_query_sting*: Only SELECT query can be specified as the query to be transmitted to the remote DBMS.
*   *server_name*: The name of the server created using:doc:`/sql/schema/server_stmt`\.
*   *dblink_conn_string*: Remote access information expressed as a string.
*   *dblink_table_alias*: The name of a derived table created using DBLINK.
*   *dblink_column_alias*: Virtual column name corresponding to the select list of *remote_query_string* in DBLINK.

.. note::

    The attributes of the column supported by DBLINK are as follows.
    
    * INT, BIGINT, SHORT, FLOAT, DOUBLE, MONETARY, NUMERIC
    * VARCHAR, CHAR
    * DATE, TIME, TIMESTAMP, DATETIME
    * DATETIMETZ, DATETIMELTZ, TIMESTAMPTZ, TIMESTAMPLTZ

.. warning::

    DBLINK does not support columns with the following data types.
    
    * COLLECTION TYPE ( SET, MULTISET, SEQUENCE )
    * OBJECT
    * CLOB / BLOB
    * ENUM
    * BIT / BIT VARYING
    * JSON

.. note::

    *dblink_conn_string* is composed of the following structure.
    Each content is information corresponding to HOST, PORT, DBNAME, USER, PASSWOED, and PROPERTIES in the :doc:`/sql/schema/server_stmt` syntax.
    Each item is separated by the character ':'.
        
    
    <broker-host>:<port#>:<db_name>:<db_user>:<db_password>:[?<properties>]
    
    To prevent password exposure, it is recommended to use *server_name* rather than *dblink_conn_string*.
    
  
.. code-block:: sql

    CREATE SERVER remote_srv1 ( HOST='127.0.0.1', PORT=3300, DBNAME=demodb, USER=cub, PASSWORD='cub-password');    
    SELECT * FROM DBLINK (remote_srv1, 'SELECT col1 FROM remote_t') AS t(col1 int);
    
    SELECT * FROM DBLINK ('127.0.0.1:3300:demodb:cub:cub-password:','SELECT col1, col2 FROM remote_t') AS t(col1 int, col2 varchar(32));

  
In the example above, the two SELECT statements perform the same function.
  



.. _where-clause:

WHERE Clause
============

In a query, a column can be processed based on conditions. The **WHERE** clause specifies a search condition for data. ::

    WHERE <search_condition>

        <search_condition> ::=
            <comparison_predicate>
            <between_predicate>
            <exists_predicate>
            <in_predicate>
            <null_predicate>
            <like_predicate>
            <quantified_predicate>
            <set_predicate>

The **WHERE** clause specifies a condition that determines the data to be retrieved by *search_condition* or a query. Only data for which the condition is true is retrieved for the query results. (**NULL** value is not retrieved for the query results because it is evaluated as unknown value.)

*   *search_condition*: It is described in detail in the following sections.

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

By SQL standard, you cannot specify a column (hidden column) not defined in the **GROUP BY** clause to the SELECT column list. However, by using extended CUBRID grammars, you can specify the hidden column to the SELECT column list. If you do not use the extended CUBRID grammars, the **only_full_group_by** parameter should be set to **yes**. For details, see :ref:`stmt-type-parameters`. 

::

    SELECT ...
    GROUP BY {col_name | expr | position} [ASC | DESC], ...
              [WITH ROLLUP] [HAVING <search_condition>]

*   *col_name* | *expr* | *position*: Specifies one or more column names, expressions, aliases or column location. Items are separated by commas. Columns are sorted on this basis.

*   [**ASC** | **DESC**]: Specifies the **ASC** or **DESC** sorting option after the columns specified in the **GROUP BY** clause. If the sorting option is not specified, the default value is **ASC**.

*   <*search_condition*>: Specifies the search condition in the **HAVING** clause. In the **HAVING** clause, you can refer to columns and aliases specified in the **GROUP BY** clause, or columns used in aggregate functions.

    .. note:: Even the hidden columns not specified in the **GROUP BY** clause can be referred to, if the value of the **only_full_group_by** parameter is set to **yes**. At this time, the HAVING condition does not affect to the query result.

*   **WITH ROLLUP**: If you specify the **WITH ROLLUP** modifier in the **GROUP BY** clause, the aggregate information of the result value of each GROUPed BY column is displayed for each group, and the total of all result rows is displayed at the last row. When a **WITH ROLLUP** modifier is defined in the **GROUP BY** clause, the result value for all rows of the group is additionally displayed. In other words, total aggregation is made for the value aggregated by group. When there are two columns for Group By, the former is considered as a large unit and the latter is considered as a small unit, so the total aggregation row for the small unit and the total aggregation row for the large unit are added. For example, you can check the aggregation of the sales result per department and salesperson through one query.

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
    ORDER BY {col_name | expr | position} [ASC | DESC], ...] [NULLS {FIRST | LAST}]

*   *col_name* | *expr* | *position*: Specifies a column name, expression, alias, or column location. One or more column names, expressions or aliases can be specified. Items are separated by commas. A column that is not specified in the list of **SELECT** columns can be specified.

*   [**ASC** | **DESC**]: **ASC** means sorting in ascending order, and **DESC** is sorting in descending order. If the sorting option is not specified, the default value is **ASC**.

*   [**NULLS** {**FIRST** | **LAST**}]: **NULLS FIRST** sorts NULL at first, **NULLS LAST** sorts NULL at last. If this syntax is omitted, **ASC** sorts NULL at first, **DESC** sorts NULL at last.

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
    LIMIT 3;
    
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

.. note::

    **Translation of GROUP BY alias**

    .. code-block:: sql

        CREATE TABLE t1(a INT, b INT, c INT);
        INSERT INTO t1 VALUES(1,1,1);
        INSERT INTO t1 VALUES(2,NULL,2);
        INSERT INTO t1 VALUES(2,2,2);

        SELECT a, NVL(b,2) AS b 
        FROM t1 
        GROUP BY a, b;  -- Q1

    When you run the above SELECT query, "GROUP BY a, b" is translated as:

    *   "GROUP BY a, NVL(b, 2)"(alias name b) in 9.2 or before. The result is the same as Q2's result as below.

        .. code-block:: sql
        
            SELECT a, NVL(b,2) AS bxxx 
            FROM t1 
            GROUP BY a, bxxx;  -- Q2

        ::

                    a            b
            ======================
                    1            1
                    2            2

	*   "GROUP BY a, b"(column name b) in 9.3 or higher. The result is the same as Q3's result as below.

        .. code-block:: sql
        
            SELECT a, NVL(b,2) AS bxxx
            FROM t1 
            GROUP BY a, b;  -- Q3

        ::

                    a            b
            ======================
                    1            1
                    2            2
                    2            2

.. _limit-clause:

LIMIT Clause
============

The **LIMIT** clause can be used to limit the number of records displayed. You can specify a very big integer for *row_count* to display to the last row, starting from a specific row. The **LIMIT** clause can be used as a prepared statement. In this case, the bind parameter (?) can be used instead of an argument.

**INST_NUM** () and **ROWNUM** cannot be included in the **WHERE** clause in a query that contains the **LIMIT** clause. Also, **LIMIT** cannot be used together with **HAVING GROUPBY_NUM** (). 

::

    LIMIT {[offset,] row_count | row_count [OFFSET offset]}

    <offset> ::= <limit_expression>
    <row_count> ::= <limit_expression>

    <limit_expression> ::= <limit_term> | <limit_expression> + <limit_term> | <limit_expression> - <limit_term>
    <limit_term> ::= <limit_factor> | <limit_term> * <limit_factor> | <limit_term> / <limit_factor>
    <limit_factor> ::= <unsigned int> | <input_hostvar> | ( <limit_expression> )

*   *offset*: Specifies the offset of the starting row to be displayed. The offset of the starting row of the result set is 0; it can be omitted and the default value is **0**. It can be one of unsigned int, a host variable or a simple expression.
*   *row_count*: Specifies the number of records to be displayed. It can be one of unsigned integer, a host variable or a simple expression.

.. code-block:: sql

    -- LIMIT clause can be used in prepared statement
    PREPARE stmt FROM 'SELECT * FROM sales_tbl LIMIT ?, ?';
    EXECUTE stmt USING 0, 10;
     
.. code-block:: sql

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

.. code-block:: sql

    -- LIMIT clause allows simple expressions for both offset and row_count
    SELECT * 
    FROM sales_tbl 
    WHERE sales_amount > 100 
    LIMIT ? * ?, (? * ?) + ?;
    
.. _join-query:
              
Join Query
==========

A join is a query that combines the rows of two or more tables or virtual tables (views). In a join query, a condition that compares the columns that are common in two or more tables is called a join condition. Rows are retrieved from each joined table, and are combined only when they satisfy the specified join condition.

A join query using an equality operator (=) is called an equi-join, and one without any join condition is called a cartesian product. Meanwhile, joining a single table is called a self join. In a self join, table **ALIAS** is used to distinguish columns, because the same table is used twice in the **FROM** clause.

A join that outputs only rows that satisfy the join condition from a joined table is called an inner or a simple join, whereas a join that outputs both rows that satisfy and do not satisfy the join condition from a joined table is called an outer join. 

An outer join is divided into a left outer join which outputs all rows of the left table as a result(outputs NULL when the right table's columns don't match conditions), a right outer join which outputs all rows of the right table as a result(outputs NULL when the left table's columns don't match conditions) and a full outer join which outputs all rows of both tables. If there is no column value that corresponds to a table on one side in the result of an outer join query, all rows are returned as **NULL**.

::

    FROM <table_specification> [{, <table_specification> 
        | { <join_table_specification> | <join_table_specification2> } ...]

    <table_specification> ::=
        <single_table_spec> [<correlation>] |
        <metaclass_specification> [<correlation>] |
        <subquery> <correlation> |
        TABLE (<expression>) <correlation>
        
    <join_table_specification> ::=
        {
            [INNER | {LEFT | RIGHT} [OUTER]] JOIN 

         } <table_specification> ON <search_condition>
     
    <join_table_specification2> ::=
		{
            CROSS JOIN |
            NATURAL [ LEFT | RIGHT ] JOIN 
        } <table_specification>

*   <*join_table_specification*>

    *   [**INNER**] **JOIN**: Used for inner join and requires join conditions.

    *   {**LEFT** | **RIGHT**} [**OUTER**] **JOIN**: **LEFT** is used for a left outer join query, and **RIGHT** is for a right outer join query.

   
*   <*join_table_specification2*>

    *   **CROSS JOIN**: Used for cross join and requires no join conditions.
    *   **NATURAL** [**LEFT** | **RIGHT**] **JOIN**: Used for natural join and join condition is not used. It operates in the equivalent same way to have a condition between columns equivalent of the same name.

Inner Join
----------

The inner join requires join conditions. The **INNER JOIN** keyword can be omitted. When it is omitted, the table is separated by a comma (,). The **ON** join condition can be replaced with the **WHERE** condition.

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

Outer Join
----------

CUBRID does not support full outer joins; it supports only left and right joins. Path expressions that include subqueries and sub-columns cannot be used in the join conditions of an outer join.

Join conditions of an outer join are specified in a different way from those of an inner join. In an inner join, join conditions can be expressed in the **WHERE** clause; in an outer join, they appear after the **ON** keyword within the **FROM** clause. Other retrieval conditions can be used in the **WHERE** or **ON** clause, but the retrieval result depends on whether the condition is used in the **WHERE** or **ON** clause.

The table execution order is fixed according to the order specified in the **FROM** clause. Therefore, when using an outer join, you should create a query statement in consideration of the table order. It is recommended to use standard statements using { **LEFT** | **RIGHT** } [ **OUTER** ] **JOIN**, because using an Oracle-style join query statements by specifying an outer join operator **(+)** in the **WHERE** clause, even if possible, might lead the execution result or plan in an unwanted direction.

The following example shows how to retrieve the years and host countries of the Olympic Games since 1950 where a world record has been set, but including the Olympic Games where any world records haven't been set in the result. This example can be expressed in the following right outer join query. In this example, all instances whose values of the *host_year* column in the *history* table are not greater than 1950 are also retrieved. All instances of *host_nation* are included because this is a right outer join. *host_year* that does not have a value is represented as **NULL**.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_year, o.host_nation
    FROM history h RIGHT OUTER JOIN olympic o ON h.host_year = o.host_year 
    WHERE o.host_year > 1950;
    
::

        host_year    host_year  host_nation
    ================================================
             NULL         1952  'Finland'
             NULL         1956  'Australia'
             NULL         1960  'Italy'
             NULL         1964  'Japan'
             NULL         1972  'Germany'
             NULL         1976  'Canada'
             1968         1968  'Mexico'
             1980         1980  'USSR'
             1984         1984  'USA'
             1988         1988  'Korea'
             1992         1992  'Spain'
             1996         1996  'USA'
             2000         2000  'Australia'
             2004         2004  'Greece'

A right outer join query can be converted to a left outer join query by switching the position of two tables in the **FROM** clause. The right outer join query in the previous example can be expressed as a left outer join query as follows:

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_year, o.host_nation
    FROM olympic o LEFT OUTER JOIN history h ON h.host_year = o.host_year 
    WHERE o.host_year > 1950;
     
::

        host_year    host_year  host_nation
    ================================================
             NULL         1952  'Finland'
             NULL         1956  'Australia'
             NULL         1960  'Italy'
             NULL         1964  'Japan'
             NULL         1972  'Germany'
             NULL         1976  'Canada'
             1968         1968  'Mexico'
             1980         1980  'USSR'
             1984         1984  'USA'
             1988         1988  'Korea'
             1992         1992  'Spain'
             1996         1996  'USA'
             2000         2000  'Australia'
             2004         2004  'Greece'

Outer joins can also be represented by using **(+)** in the **WHERE** clause. The above example is a query that has the same meaning as the example using the **LEFT** **OUTER** **JOIN**. The **(+)** syntax is not ISO/ANSI standard, so it can lead to ambiguous situations. It is recommended to use the standard syntax **LEFT** **OUTER** **JOIN** (or **RIGHT** **OUTER** **JOIN**) if possible.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_year, o.host_nation 
    FROM history h, olympic o
    WHERE o.host_year = h.host_year(+) AND o.host_year > 1950;
     
::

        host_year    host_year  host_nation
    ================================================
             NULL         1952  'Finland'
             NULL         1956  'Australia'
             NULL         1960  'Italy'
             NULL         1964  'Japan'
             NULL         1972  'Germany'
             NULL         1976  'Canada'
             1968         1968  'Mexico'
             1980         1980  'USSR'
             1984         1984  'USA'
             1988         1988  'Korea'
             1992         1992  'Spain'
             1996         1996  'USA'
             2000         2000  'Australia'
             2004         2004  'Greece'

In the above examples, *h.host_year=o.host_year* is an outer join condition, and *o.host_year > 1950* is a search condition. If the search condition is not written in the **WHERE** clause but in the **ON** clause, the meaning and the result will be different. The following query also includes instances whose values of *o.host_year* are not greater than 1950.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_year, o.host_nation
    FROM olympic o LEFT OUTER JOIN history h ON h.host_year = o.host_year AND o.host_year > 1950;
     
::

        host_year    host_year  host_nation
    ================================================
             NULL         1896  'Greece'
             NULL         1900  'France'
             NULL         1904  'USA'
             NULL         1908  'United Kingdom'
             NULL         1912  'Sweden'
             NULL         1920  'Belgium'
             NULL         1924  'France'
             NULL         1928  'Netherlands'
             NULL         1932  'USA'
             NULL         1936  'Germany'
             NULL         1948  'England'
             NULL         1952  'Finland'
             NULL         1956  'Australia'
             NULL         1960  'Italy'
             NULL         1964  'Japan'
             NULL         1972  'Germany'
             NULL         1976  'Canada'
             1968         1968  'Mexico'
             1980         1980  'USSR'
             1984         1984  'USA'
             1988         1988  'Korea'
             1992         1992  'Spain'
             1996         1996  'USA'
             2000         2000  'Australia'
             2004         2004  'Greece'

In the above example, **LEFT OUTER JOIN** should attach all rows to the result rows even if the left table's rows do not match to the condition; therefore, the left table's condition, "AND o.host_year > 1950" is ignored. But "WHERE o.host_year > 1950" is applied after the join operation is completed. Please consider that a condition after **ON** clause and a condition after **WHERE** clause can be applied differently in **OUTER JOIN**.

Cross Join
----------

The cross join is a cartesian product, meaning that it is a combination of two tables, without any condition. For the cross join, the **CROSS JOIN** keyword can be omitted. When it is omitted, the table is separated by a comma (,).

The following example shows how to write cross join.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h CROSS JOIN olympic o;
     
    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h, olympic o;

The above two queries output the same results.

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

    144 rows selected. (1.283548 sec) Committed.


Natural Join
------------

When column names to be joined to each table are the same, that is, when you want to grant equivalent conditions between each column with the same name, a natural join, which can replace inner/outer join, can be used.

.. code-block:: sql

    CREATE TABLE t1 (a int, b1 int); 
    CREATE TABLE t2 (a int, b2 int);

    INSERT INTO t1 values(1,1);
    INSERT INTO t1 values(3,3);
    INSERT INTO t2 values(1,1);
    INSERT INTO t2 values(2,2);

The below is an example of running **NATURAL JOIN**.

.. code-block:: sql
    
    SELECT /*+ RECOMPILE*/ * 
    FROM t1 NATURAL JOIN t2;

Running the above query is the same as running the below query, and they display the same result.

.. code-block:: sql

    SELECT /*+ RECOMPILE*/ * 
    FROM t1 INNER JOIN t2 ON t1.a=t2.a;

::


            a           b1            a           b2
    ================================================
            1            1            1            1

The below is an example of running **NATURAL LEFT JOIN**.
    
.. code-block:: sql

    SELECT /*+ RECOMPILE*/ * 
    FROM t1 NATURAL LEFT JOIN t2;
    
Running the above query is the same as running the below query, and they display the same result.

.. code-block:: sql

    SELECT /*+ RECOMPILE*/ * 
    FROM t1 LEFT JOIN t2 ON t1.a=t2.a;

::

                a           b1            a           b2
    ====================================================
                1            1            1            1
                3            3         NULL         NULL

The below is an example of running **NATURAL RIGHT JOIN**.

.. code-block:: sql

    SELECT /*+ RECOMPILE*/ * 
    FROM t1 NATURAL RIGHT JOIN t2;

Running the above query is the same as running the below query, and they display the same result.

.. code-block:: sql

    SELECT /*+ RECOMPILE*/ * 
    FROM t1 RIGHT JOIN t2 ON t1.a=t2.a;
    
::

                a           b1            a           b2
    ====================================================
                1            1            1            1
             NULL         NULL            2            2




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

The **VALUES** clause can be used to express the **UNION ALL** query, which consists of constant values in a simpler way. For example, the following query can be executed.

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
    SELECT 4, 'fourth';

The following example shows use of the **VALUES** clause with multiple rows in the **INSERT** statement.

.. code-block:: sql

    INSERT INTO athlete (code, name, gender, nation_code, event)
    VALUES ('21111', 'Jang Mi-Ran ', 'F', 'KOR', 'Weight-lifting'),
           ('21112', 'Son Yeon-Jae ', 'F', 'KOR', 'Rhythmic gymnastics');

The following example shows how to use subquery in the **FROM** statement.

.. code-block:: sql
    
    SELECT a.*
    FROM athlete a, (VALUES ('Jang Mi-Ran', 'F'), ('Son Yeon-Jae', 'F')) AS t(name, gender)
    WHERE a.name=t.name AND a.gender=t.gender;

::

             code  name                gender   nation_code        event
    =====================================================================================================
            21111  'Jang Mi-Ran'       'F'      'KOR'              'Weight-lifting'
            21112  'Son Yeon-Jae'      'F'      'KOR'              'Rhythmic gymnastics'

FOR UPDATE
==========

The **FOR UPDATE** clause can be used in **SELECT** statements for locking rows returned by the statement for a later **UPDATE/DELETE**.

:: 

    SELECT ... [FOR UPDATE [OF <spec_name_comma_list>]]

        <spec_name_comma_list> ::= <spec_name> [, <spec_name>, ... ]
            <spec_name> ::= [schema_name.]table_name | [schema_name.]view_name 
         
* <*spec_name_comma_list*>: A list of table/view names referenced from the **FROM** clause.

Only table/view referenced in <*spec_name_comma_list*> will be locked. If the <*spec_name_comma_list*> is missing but **FOR UPDATE** is present then we assume that all tables/views from the **FROM** clause of the **SELECT** statement are referenced. Rows are locked using **X_LOCK**.

.. note:: Restrictions

    *   It cannot be used in subqueries (but it can reference subqueries). 
    *   It cannot be used in a statement that has **GROUP BY**, **DISTINCT** or aggregate functions. 
    *   It cannot reference **UNION**\s. 

The following shows how to use **SELECT ... FOR UPDATE** statements.

.. code-block:: sql 


    CREATE TABLE t1(i INT); 
    INSERT INTO t1 VALUES (1), (2), (3), (4), (5); 

    CREATE TABLE t2(i INT); 
    INSERT INTO t2 VALUES (1), (2), (3), (4), (5); 
    CREATE INDEX idx_t2_i ON t2(i); 

    CREATE VIEW v12 AS SELECT t1.i AS i1, t2.i AS i2 FROM t1 INNER JOIN t2 ON t1.i=t2.i; 

    SELECT * FROM t1 ORDER BY 1 FOR UPDATE; 
    SELECT * FROM t1 ORDER BY 1 FOR UPDATE OF t1; 
    SELECT * FROM t1 INNER JOIN t2 ON t1.i=t2.i ORDER BY 1 FOR UPDATE OF t1, t2; 

    SELECT * FROM t1 INNER JOIN (SELECT * FROM t2 WHERE t2.i > 0) r ON t1.i=r.i WHERE t1.i > 0 ORDER BY 1 FOR UPDATE; 

    SELECT * FROM v12 ORDER BY 1 FOR UPDATE; 
    SELECT * FROM t1, (SELECT * FROM v12, t2 WHERE t2.i > 0 AND t2.i=v12.i1) r WHERE t1.i > 0 AND t1.i=r.i ORDER BY 1 FOR UPDATE OF r;
