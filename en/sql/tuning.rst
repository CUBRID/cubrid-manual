******************
Query Optimization
******************

Updating Statistics
===================

With the **UPDATE STATISTICS ON** statement, you can generate internal statistics used by the query processor. Such statistics allow the database system to perform query optimization more efficiently. ::

    UPDATE STATISTICS ON { table_spec [ {, table_spec } ] | ALL CLASSES | CATALOG CLASSES } [ ; ]
    
    table_spec ::=
    single_table_spec
    | ( single_table_spec [ {, single_table_spec } ] )
    
    single_table_spec ::=
    [ ONLY ] table_name
    | ALL table_name [ ( EXCEPT table_name ) ]

*   **ALL CLASSES** : If the **ALL CLASSES** keyword is specified, the statistics on all the tables existing in the database are updated.

Checking Statistics Information
===============================

You can check the statistics Information with the session command of the CSQL Interpreter. ::

    csql> ;info stats table_name
    
*   *table_name* : Table name to check the statistics Information

.. code-block:: sql

    CREATE TABLE t1 (code INT);
    INSERT INTO t1 VALUES(1),(2),(3),(4),(5);
    CREATE INDEX i_t1_code ON t1(code);
    UPDATE STATISTICS ON t1;

::

    ;info stats t1
    CLASS STATISTICS
    ****************
     Class name: t1 Timestamp: Mon Mar 14 16:26:40 2011
     Total pages in class heap: 1
     Total objects: 5
     Number of attributes: 1
     Atrribute: code
        id: 0
        Type: DB_TYPE_INTEGER
        Mininum value: 1
        Maxinum value: 5
        B+tree statistics:
            BTID: { 0 , 1049 }
            Cardinality: 5 (5) , Total pages: 2 , Leaf pages: 1 , Height: 2

Viewing Query Plan
==================

To view a query plan for a CUBRID SQL query, change the value of the optimization level by using the **SET OPTIMIZATION** statement. You can get the current optimization level value by using the **GET OPTIMIZATION** statement. 

The CUBRID query optimizer determines whether to perform query optimization and output the query plan by referencing the optimization level value set by the user. The query plan is displayed as standard output; the following explanations are based on the assumption that the plan is used in a terminal-based program such as the CSQL Interpreter. In the CSQL query editor, you can view execution plan by executing the **;plan** command. For details, see :ref:`csql-session-commands`. For the method how to view a query plan, see `CUBRID Manager manual <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual_kr>`_. ::

    SET OPTIMIZATION LEVEL opt-level [;]
    GET OPTIMIZATION LEVEL [ { TO | INTO } variable ] [;]

*   *opt-level* : A value that specifies the optimization level. It has the following meanings.

    *   0: Does not perform query optimization. The query is executed using the simplest query plan. This value is used only for debugging.
    
    *   1: Create a query plan by performing query optimization and executes the query. This is a default value used in CUBRID, and does not have to be changed in most cases.
    
    *   2: Creates a query plan by performing query optimization. However, the query itself is not executed. In general, this value is not used; it is used together with the following values to be set for viewing query plans.
    
    *   257: Performs query optimization and outputs the created query plan. This value works for displaying the query plan by internally interpreting the value as 256+1 related with the value 1.
    
    *   258: Performs query optimization and outputs the created query plan, but does not execute the query.  That is, this value works for displaying the query plan by internally interpreting the value as 256+2 related with the value 2. This setting is useful to examine the query plan but not to intend to see the query results.
    
    *   513: Performs query optimization and outputs the detailed query plan. This value works for displaying more detailed query plan than the value 257 by internally interpreting the value as 512+1.
    
    *   514: Performs query optimization and outputs the detailed query plan. However, the query is not executed. This value works for displaying more detailed query plan than the value 258 by internally interpreting the value as 512+2.

    .. note:: If you config the optimization level as not executing the query like 2, 258, or 514, all queries(not only SELECT, but also INSERT, UPDATE, DELETE, REPLACE, TRIGGER, SERIAL, etc.) are not executed.   

The following example shows how to view query plan by using the example retrieving year when Sim Kwon Ho won medal and metal type.

.. code-block:: sql

    GET OPTIMIZATION LEVEL
          Result
    =============
                1

    SET OPTIMIZATION LEVEL 258;

    SELECT a.name, b.host_year, b.medal
    FROM athlete a, game b WHERE a.name = 'Sim Kwon Ho' AND a.code = b.athlete_code
    Query plan:
      Nested loops
            Sequential scan(game b)
            Index scan(athlete a, pk_athlete_code, a.code=b.athlete_code)
    There are no results.
    0 rows selected.

.. _sql-hint:

Using SQL Hint
==============

Using hints can affect the performance of query execution. you can allow the query optimizer to create more efficient execution plan by referring the SQL HINT. The SQL HINTs related tale join, index, and statistics information are provided by CUBRID. ::

    { CREATE | ALTER } /*+ NO_STATS */ { TABLE | CLASS } ...;
        
    { CREATE | ALTER | DROP } /*+ NO_STATS */ INDEX ...;
     
    { SELECT | UPDATE | DELETE } /*+ <hint> [ { <hint> } ... ] */ ...;

    MERGE /*+ <merge_statement_hint> [ { <merge_statement_hint> } ... ] */ INTO ...;
    
    <hint> ::=
    USE_NL [ (spec_name_comma_list) ] |
    USE_IDX [ (spec_name_comma_list) ] |
    USE_MERGE [ (spec_name_comma_list) ] |
    ORDERED |
    USE_DESC_IDX |
    NO_DESC_IDX |
    NO_COVERING_IDX |
    NO_MULTI_RANGE_OPT |
    RECOMPILE
    
    <merge_statement_hint> ::=
    USE_UPDATE_INDEX (<update_index_list>) |
    USE_DELETE_INDEX (<insert_index_list>) |
    RECOMPILE

SQL hints are specified by using plus signs to comments.

* /\*+ hint \*/
* --+ hint
* //+ hint

The hint comment must appear after the **SELECT**, **CREATE**, **ALTER**, etc. keyword, and the comment must begin with a plus sign (+), following the comment delimiter.  When you specify several hints, they are  separated by blanks.

The following hints can be specified in CREATE/ALTER TABLE statements and CREATE/ALTER/DROP INDEX statements.

    *   **NO_STATS** : Related to a statistical information hint. If it is specified, query optimizaer does not update the statistical information after running the DDL statement. Therefore, the DDL performance  is improved, but note that the query plan is not optimized.

The following hints can be specified in UPDATE, DELETE and SELECT statements.

    *   **USE_NL** : Related to a table join, the query optimizer creates a nested loop join execution plan with this hint.
    *   **USE_MERGE** : Related to a table join, the query optimizer creates a sort merge join execution plan with this hint.
    *   **ORDERED** : Related to a table join, the query optimizer create a join execution plan with this hint, based on the order of tables specified in the FROM clause. The left table in the FROM clause becomes the outer table; the right one becomes the inner table.
    *   **USE_IDX** : Related to a index, the query optimizer creates a index join execution plan corresponding to a specified table with this hint.
    *   **USE_DESC_IDX** : This is a hint for the scan in descending index. For more information, see :ref:`index-descending-scan`.
    *   **NO_DESC_IDX** : This is a hint not to use the descending index.
    *   **NO_COVERING_IDX** : This is a hint not to use the covering index. For details, see :ref:`covering-index`.
    *   **NO_STATS** : Related to statistics information, the query optimizer does not update statistics information. Query performance for the corresponding queries can be improved; however, query plan is not optimized because the information is not updated.
    *   **RECOMPILE** : Recompiles the query execution plan. This hint is used to delete the query execution plan stored in the cache and establish a new query execution plan.

    .. note:: If the *spec_name* is specified together with **USE_NL**, **USE_IDX** or **USE_MERGE**, the specified join method applies only to the *spec_name*. If **USE_NL** and **USE_MERGE** are specified together, the given hint is ignored. In some cases, the query optimizer cannot create a query execution plan based on the given hint. For example, if **USE_NL** is specified for a right outer join, the query is converted to a left outer join internally, and the join order may not be guaranteed.

MERGE statement can have below hints.

*   **USE_INSERT_INDEX** (<*insert_index_list*>) : An index hint which is used in INSERT clause of MERGE statement. Lists index names to *insert_index_list* to use when executing INSERT cluase. This hint is applied to  <*join_condition*> of MERGE statement.
*   **USE_UPDATE_INDEX** (<*update_index_list*>) : An index hint which is used in UPDATE clause of MERGE statement. Lists index names to *update_index_list* to use when executing UPDATE cluase. This hint is applied to <*join_condition*> and <*update_condition*> of MERGE statement.
*   **RECOMPILE** : Recompile the query execution plan. Use this hint to remove the old query plan and set the new one to the query plan cache.

The following example shows how to retrieve the years when Sim Kwon Ho won medals and the types of medals. Here, a nested loop join execution plan needs to be created which has the *athlete* table as an outer table and the *game* table as an inner table. It can be expressed by the following query. The query optimizer creates a nested loop join execution plan that has the *game* table as an outer table and the *athlete* table as an inner table.

.. code-block:: sql

    SELECT /*+ USE_NL ORDERED  */ a.name, b.host_year, b.medal
    FROM athlete a, game b 
    WHERE a.name = 'Sim Kwon Ho' AND a.code = b.athlete_code;
    
      name                    host_year  medal
    =========================================================
      'Sim Kwon Ho'                2000  'G'
      'Sim Kwon Ho'                1996  'G'
      
    2 rows selected.

The following example shows how to retrieve query execution time with **NO_STAT** hint to improve the functionality of drop partitioned table (*before_2008*); any data is not stored in the table. Assuming that there are more than 1 million data in the *participant2* table. The execution time in the example depends on system performance and database configuration.

.. code-block:: sql

    -- Not using NO_STATS hint
    ALTER TABLE participant2 DROP partition before_2008;

    SQL statement execution time: 31.684550 sec

    -- Using NO_STATS hint
    ALTER /*+ NO_STATS */ TABLE participant2 DROP partition before_2008;

    SQL statement execution time: 0.025773 sec

.. _index-hint-syntax:

Index Hint Syntax
-----------------

The index hint syntax allows the query processor to select a proper index by specifying the index in the query. You can specify the index hint by USING INDEX clause or by {USE|FORCE|IGNORE} INDEX syntax after "FROM table" clause.

USING INDEX
-----------

**USING INDEX** clause should be specified after **WHERE** clause of **SELECT**, **DELETE** or **UPDATE** statement. **USING INDEX** clause forces a sequential/index scan to be used or an index that can improve the performance to be included.

If **USING INDEX** clause is specified with the list of index names, query optimizer creates optimized execution plan by calculating the query execution cost based on the specified indexes only and comparing the index scan cost and the sequential scan cost of the specified indexes(CUBRID performs cost-based query optimization to select an execution plan).

The **USING INDEX**  clause is useful to get the results in the desired order without **ORDER BY**. When index scan is performed by CUBRID, the results are created in the order they were saved in the index. When there are more than one indexes in one table, you can use **USING INDEX** to get the query results in a given order of indexes.

::

    SELECT ... WHERE ...
    [USING INDEX { NONE | [ ALL EXCEPT ] <index_spec> [ {, <index_spec> } ...] } ] [ ; ]
    
    DELETE ... WHERE ...
    [USING INDEX { NONE | [ ALL EXCEPT ] <index_spec> [ {, <index_spec> } ...] } ] [ ; ]
    
    UPDATE ... WHERE ...
    [USING INDEX { NONE | [ ALL EXCEPT ] <index_spec> [ {, <index_spec> } ...] } ] [ ; ] 
    
    <index_spec> ::=
      [table_spec.]index_name [(+) | (-)] |
      table_spec.NONE
 

*   **NONE** : If **NONE** is specified,  a sequential scan is used on all tables.
*   **ALL EXCEPT** : All indexes except the specified indexes can be used when the query is executed.
*   *index_name*\ (+) : If (+) is specified after the index_name, it is the first priority in index selection. IF this index is not proper to run the query, it is not selected.
*   *index_name*\ (-) : If (-) is specified after the index_name, it is execluded from index selection. 
*   *table_spec*.\ **NONE** : All indexes are execluded from the selection, so sequential scan is used.

USE,FORCE,IGNORE INDEX
----------------------

Index hints can be specified through **USE**, **FORCE**, **IGNORE INDEX** syntax after table specification of **FROM** clause.

::

    FROM table_spec [ <index_hint_clause> ] ...
    
    <index_hint_clause> ::=
      { USE | FORCE | IGNORE } INDEX  ( <index_spec> [, <index_spec>  ...] )
    
    <index_spec> ::=
      [table_spec.]index_name

*    **USE INDEX** ( <*index_spec*> ): Only specified indexes are considered when choose them.
*    **FORCE INDEX** ( <*index_spec*> ): Specified indexes are choosed as the first priority.
*    **IGNORE INDEX** ( <*index_spec*> ): Specified indexes are excluded from the choice.

USE, FORCE, IGNORE INDEX syntax is automatically rewritten as the proper USING INDEX syntax by the system.

Examples of index hint
----------------------

.. code-block:: sql

    CREATE TABLE athlete (
       code             SMALLINT PRIMARY KEY,
       name             VARCHAR(40) NOT NULL,
       gender           CHAR(1),
       nation_code      CHAR(3),
       event            VARCHAR(30)
    );
    CREATE UNIQUE INDEX athlete_idx1 ON athlete (code, nation_code);
    CREATE INDEX athlete_idx2 ON athlete (gender, nation_code);

Below two queries do the same behavior and they select index scan if the specified index, *athlete_idx2*\'s scan cost is lower than sequential scan cost.

.. code-block:: sql

    SELECT /*+ RECOMPILE */ * 
    FROM athlete USE INDEX (athlete_idx2) 
    WHERE gender='M' AND nation_code='USA';

    SELECT /*+ RECOMPILE */ * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete_idx2;

Below two queries do the same behavior and they always use *athlete_idx2*

.. code-block:: sql
    
    SELECT /*+ RECOMPILE */ * 
    FROM athlete FORCE INDEX (athlete_idx2) 
    WHERE gender='M' AND nation_code='USA';

    SELECT /*+ RECOMPILE */ * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete_idx2(+);

Below two queries do the same behavior and they always don't use *athlete_idx2*

.. code-block:: sql
    
    SELECT /*+ RECOMPILE */ * 
    FROM athlete IGNORE INDEX (athlete_idx2) 
    WHERE gender='M' AND nation_code='USA';

    SELECT /*+ RECOMPILE */ * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete_idx2(-);
    
Below query always do the sequential scan.

.. code-block:: sql

    SELECT * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX NONE;

    SELECT * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete.NONE;

Below query forces to be possible to use all indexes execept *athlete_idx2* index.

.. code-block:: sql

    SELECT * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX ALL EXCEPT athlete_idx2;
    
    
When two or more indexes have been specified in the **USING INDEX** clause, the query optimizer selects the proper one of the specified indexes.

.. code-block:: sql

    SELECT * 
    FROM athlete USE INDEX (char_idx, athlete_idx) 
    WHERE gender='M' AND nation_code='USA';

    SELECT * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX char_idx, athlete_idx;

When a query is run for several tables, you can specify a table to perform index scan by using a specific index and another table to perform sequential scan. The query has the following format.

.. code-block:: sql

    SELECT * 
    FROM tab1, tab2 
    WHERE ... 
    USING INDEX tab1.idx1, tab2.NONE;

When executing a query with the index hint syntax, the query optimizer considers all available indexes on the table for which no index has been specified. For example, when the *tab1* table includes *idx1* and *idx2* and the *tab2* table includes *idx3*, *idx4*, and *idx5*, if indexes for only *tab1* are specified but no indexes are specified for *tab2*, the query optimizer considers the indexes of *tab2*.

.. code-block:: sql

    SELECT ... 
    FROM tab1, tab2 USE INDEX(tab1.idx1) 
    WHERE ... ;
    
    SELECT ... 
    FROM tab1, tab2 
    WHERE ... USING INDEX tab1.idx1;

The above query select the scan method of table *tab1* after comparing the cost between the sequential scan of the table *tab1* and the index scan of the index *idx1*, and select the scan method of table *tab2* after comparing the cost between the sequential scan of the table *tab2* and the index scan of the indexes *idx3*, *idx4*, *idx5*.
    
Special Indexes
===============

.. _filtered-index:

Filtered Index
--------------

The filtered index is used to sort, search, or operate a well-defined partials set for one table. It is called the partial index since only some data that satisfy the condition are kept in that index. ::


    CREATE /*+ hints */ INDEX index_name
    ON table_name (col1, col2, ...) 
    WHERE <filter_predicate>;
     
    ALTER  /*+ hints */ INDEX index_name
    [ ON table_name (col1, col2, ...) 
    [ WHERE <filter_predicate> ] ]
    REBUILD;
     
    <filter_predicate> ::= <filter_predicate> AND <expression> | <expression>
    
*   <*filter_predicate*>: Condition to compare the column and the constant. When there are several conditions, filtering is available only when they are connected by using **AND**. The filter conditions can include most of the operators and functions supported by CUBRID. However, the date/time function that shows the current date/time (ex: :func:`SYS_DATETIME`) or random functions (ex: :func:`RAND`), which outputs different results for one input are not allowed.

If you apply the filtered index, that filtered index must be specified by **USING INDEX** clause or **USE INDEX** syntax.

.. code-block:: sql

    SELECT * 
    FROM blogtopic 
    WHERE postDate>'2010-01-01' 
    USING INDEX my_filter_index;

The following example shows a bug tracking system that maintains bugs/issues. After a specified period of development, the bugs table records bugs. Most of the bugs have already been closed. The bug tracking system makes queries on the table to find new open bugs. In this case, the indexes on the bug table do not need to know the records on closed bugs. Then the filtered indexes allow indexing of open bugs only.

.. code-block:: sql

    CREATE TABLE bugs
    (
        bugID BIGINT NOT NULL,
        CreationDate TIMESTAMP,
        Author VARCHAR(255),
        Subject VARCHAR(255),
        Description VARCHAR(255),
        CurrentStatus INTEGER,
        Closed SMALLINT
    );

Indexes for open bugs can be created by using the following sentence:

.. code-block:: sql

    CREATE INDEX idx_open_bugs ON bugs(bugID) WHERE Closed = 0;

To process queries that are interested in open bugs, specify the index as a index hint. It will allow to create query results by accessing less index pages through filtered indexes.

.. code-block:: sql

    SELECT * 
    FROM bugs
    WHERE Author = 'madden' AND Subject LIKE '%fopen%' AND Closed = 0;
    USING INDEX idx_open_bugs;
     
    SELECT * 
    FROM bugs  USE INDEX (idx_open_bugs)
    WHERE CreationDate > CURRENT_DATE - 10 AND Closed = 0;

.. warning::

    If you execute queries by specifying indexes with index hint syntax, you may have incorrect query results as output even though the conditions of creating filtered indexes does not meet the query conditions.

**Constraints**

Only generic indexes are allowed as filtered indexes. For example, the filtered unique index is not allowed. 

The following cases are not allowed as filtering conditions.

*   Functions, which output different results with the same input, such as date/time function or random function

    .. code-block:: sql
      
        CREATE INDEX idx ON bugs(creationdate) WHERE creationdate > SYS_DATETIME;
         
        ERROR: before ' ; '
        'sys_datetime ' is not allowed in a filter expression for index.
         
        CREATE INDEX idx ON bugs(bugID) WHERE bugID > RAND();
         
        ERROR: before ' ; '
        'rand ' is not allowed in a filter expression for index.
    
*   In case of using the **OR** operator

    .. code-block:: sql

        CREATE INDEX IDX ON bugs(bugID) WHERE bugID > 10 OR bugID = 3;
         
        In line 1, column 62,
         
        ERROR: before ' ; '
        ' or ' is not allowed in a filter expression for index.

*   In case of including functions like :func:`INCR`, :func:`DECR` functions, which modify the data of a table.
    
*   In case of Serial-related functions and including pseudo columns.
    
*   In case of including aggregate functions such as :func:`MIN`, :func:`MAX`, :func:`STDDEV`
    
*   In case of using the types where indexes cannot be created

    -   The operators and functions where an argument is the **SET** type
    -   The functions to use LOB file(:func:`CHAR_TO_BLOB`, :func:`CHAR_TO_CLOB`, :func:`BIT_TO_BLOB`, :func:`BLOB_FROM_FILE`, :func:`CLOB_FROM_FILE`)

*   The **IS NULL** operator can be used only when at least one column of an index is not NULL.

  .. code-block:: sql
  
    CREATE TABLE t (a INT, b INT);
     
    -- IS NULL cannot be used with expressions
    CREATE INDEX idx ON t (a) WHERE (not a) IS NULL;
    
    ERROR: before ' ; '
    Invalid filter expression (( not t.a<>0) is null ) for index.
     
    CREATE INDEX idx ON t (a) WHERE (a+1) IS NULL;
    
    ERROR: before ' ; '
    Invalid filter expression ((t.a+1) is null ) for index.
     
    -- At least one attribute must not be used with IS NULL
    CREATE INDEX idx ON t(a,b) WHERE a IS NULL ;
    
    ERROR: before '  ; '
    Invalid filter expression (t.a is null ) for index.
     
    CREATE INDEX idx ON t(a,b) WHERE a IS NULL and b IS NULL;
    
    ERROR: before ' ; '
    Invalid filter expression (t.a is null  and t.b is null ) for index.
     
    CREATE INDEX idx ON t(a,b) WHERE a IS NULL and b IS NOT NULL;

*   Index Skip Scan (ISS) is not allowed for the filtered indexes.
*   The length of condition string used for the filtered index is limited to 128 characters.

    .. code-block:: sql

        CREATE TABLE t(VeryLongColumnNameOfTypeInteger INT);
        1 command(s) successfully processed.
         
        CREATE INDEX idx ON t(VeryLongColumnNameOfTypeInteger) WHERE VeryLongColumnNameOfTypeInteger > 3 AND VeryLongColumnNameOfTypeInteger < 10 AND sqrt(VeryLongColumnNameOfTypeInteger) < 3 AND SQRT(VeryLongColumnNameOfTypeInteger) < 10;
        ERROR: before ' ; '
        The maximum length of filter predicate string must be 128.

.. _function-index:

Function-based Index
--------------------

Function-based index is used to sort or find the data based on the combination of values of table rows by using a specific function. For example, to find the space-ignored string, it can be used to optimize the query by using the function that provides the feature. In addition, it is useful to search the non-case-sensitive names. ::

    CREATE /*+ hints */ INDEX index_name
    ON table_name (function_name (argument_list));
    
    ALTER /*+ hints */ INDEX index_name
    [ ON table_name (function_name (argument_list)) ]
    REBUILD;
    
After the following indexes have been created, the **SELECT** query automatically uses the function-based index.

.. code-block:: sql

    CREATE INDEX idx_trim_post ON posts_table(TRIM(keyword));
    
    SELECT * 
    FROM posts_table 
    WHERE TRIM(keyword) = 'SQL';

If a function-based index is created by using the **LOWER** function, it can be used to search the non-case-sensitive names.

.. code-block:: sql

    CREATE INDEX idx_last_name_lower ON clients_table(LOWER(LastName));
    SELECT * FROM clients_table WHERE LOWER(LastName) = LOWER('Timothy');

To make an index selected while creating a query plan, the function used for the index should be used for the query condition in the same way. The **SELECT** query above uses the last_name_lower index created above. However, this index is not used for the following condition:

.. code-block:: sql

    SELECT * 
    FROM clients_table
    WHERE LOWER(CONCAT('Mr. ', LastName)) = LOWER('Mr. Timothy');

In addition, to make the function-based index used by force, use the **USING INDEX** syntax.

.. code-block:: sql

    CREATE INDEX i_tbl_first_four ON tbl(LEFT(col, 4));
    SELECT *
    FROM clients_table 
    WHERE LEFT(col, 4) = 'CAT5' 
    USING INDEX i_tbl_first_four;

.. _allowed-function-in-function-index:

Functions with the function-based indexes are as follows:

    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | ABS               | ACOS              | ADD_MONTHS        | ADDDATE           | ASIN              |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | ATAN              | ATAN2             | BIT_COUNT         | BIT_LENGTH        | CEIL              |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | CHAR_LENGTH       | CHR               | COS               | COT               | DATE              |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | DATE_ADD          | DATE_FORMAT       | DATE_SUB          | DATEDIFF          | DAY               |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | DAYOFMONTH        | DAYOFWEEK         | DAYOFYEAR         | DEGREES           | EXP               |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | FLOOR             | FORMAT            | FROM_DAYS         | FROM_UNIXTIME     | GREATEST          |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | HOUR              | IFNULL            | INET_ATON         | INET_NTOA         | INSTR             |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | LAST_DAY          | LEAST             | LEFT              | LN                | LOCATE            |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | LOG               | LOG10             | LOG2              | LOWER             | LPAD              |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | LTRIM             | MAKEDATE          | MAKETIME          | MD5               | MID               |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | MINUTE            | MOD               | MONTH             | MONTHS_BETWEEN    | NULLIF            |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | NVL               | NVL2              | OCTET_LENGTH      | POSITION          | POWER             |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | QUARTER           | RADIANS           | REPEAT            | REPLACE           | REVERSE           |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | RIGHT             | ROUND             | RPAD              | RTRIM             | SECOND            |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | SECTOTIME         | SIN               | SQRT              | STR_TO_DATE       | STRCMP            |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | SUBDATE           | SUBSTR            | SUBSTRING         | SUBSTRING_INDEX   | TAN               |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | TIME              | TIME_FORMAT       | TIMEDIFF          | TIMESTAMP         | TIMETOSEC         |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | TO_CHAR           | TO_DATE           | TO_DATETIME       | TO_DAYS           | TO_NUMBER         |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | TO_TIME           | TO_TIMESTAMP      | TRANSLATE         | TRIM              | TRUNC             |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | UNIX_TIMESTAMP    | UPPER             | WEEK              | WEEKDAY           | YEAR              |
    +-------------------+-------------------+-------------------+-------------------+-------------------+

Arguments of functions which can be used in the function-based indexes, only column names and constants are allowed; nested expressions are not allowed. For example, a statement below will cause an error.

.. code-block:: sql

    CREATE INDEX my_idx ON tbl (TRIM(LEFT(col, 3)));
    CREATE INDEX my_idx ON tbl (LEFT(col1, col2 + 3));

However, implicit cast is allowed. In the example below, the first argument type of the **LEFT** () function should be **VARCHAR** and the second argument type should be **INTEGER**; it works normally.

.. code-block:: sql

    CREATE INDEX my_idx ON tbl (LEFT(int_col, str_col));

Function-based indexes cannot be used with filtered indexes. The example will cause an error.

.. code-block:: sql

    CREATE INDEX my_idx ON tbl ( TRIM(col) ) WHERE col > 'SQL';

Function-based indexes cannot become multiple-columns indexes. The example will cause an error.
.. code-block:: sql

    CREATE INDEX my_idx ON tbl ( TRIM(col1), col2, LEFT(col3, 5) );


.. _tuning-index:

Optimization using indexes
========================== 

.. _covering-index:

Covering Index
--------------

The covering index is the index including the data of all columns in the **SELECT** list and the **WHERE**, **HAVING**, **GROUP BY**, and **ORDER BY** clauses.

You only need to scan the index pages, as the covering index contains all the data necessary for executing a query, and it also reduces the I/O costs as it is not necessary to scan the data storage any further. To increase data search speed, you can consider creating a covering index but you should be aware that the **INSERT** and the **DELETE** processes may be slowed down due to the increase in index size.

The rules about the applicability of the covering index are as follows:

*   If the covering index is applicable, you should use the CUBRID query optimizer first.
*   For the join query, if the index includes columns of the table in the **SELECT** list, use this index.
*   You cannot use the covering index if an index cannot be used.

.. code-block:: sql

    CREATE TABLE t (col1 INT, col2 INT, col3 INT);
    CREATE INDEX i_t_col1_col2_col3 ON t (col1,col2,col3);
    INSERT INTO t VALUES (1,2,3),(4,5,6),(10,8,9);

The following example shows that the index is used as a covering index because columns of both **SELECT** and **WHERE** condition exist within the index.

.. code-block:: sql

    csql> ;plan simple
    SELECT * FROM t WHERE col1 < 6;
     
    Query plan:
     Index scan(t t, i_t_col1_col2_col3, [(t.col1 range (min inf_lt t.col3))] (covers))
             col1         col2         col3
    =======================================
                1            2            3
                4            5            6

.. warning::

    If the covering index is applied when you get the values from the **VARCHAR** type column, the empty strings that follow will be truncated. If the covering index is applied to the execution of query optimization, the resulting query value will be retrieved. This is because the value will be stored in the index with the empty string being truncated.

    If you don't want this, use the **NO_COVERING_IDX** hint, which does not use the covering index function. If you use the hint, you can get the result value from the data area rather than from the index area.

    The following is a detailed example of the above situation. First, create a table with columns in **VARCHAR** types, and then **INSERT** the value with the same start character string value but the number of empty characters. Next, create an index in the column.

    .. code-block:: sql

        CREATE TABLE tab(c VARCHAR(32));
        INSERT INTO tab VALUES('abcd'),('abcd    '),('abcd ');
        CREATE INDEX i_tab_c ON tab(c);

    If you must use the index (the covering index applied), the query result is as follows:

    .. code-block:: sql

        csql>;plan simple
        SELECT * FROM tab where c='abcd    ' USING INDEX i_tab_c(+);
         
        Query plan:
         Index scan(tab tab, i_tab_c, (tab.c='abcd    ') (covers))
         
         c
        ======================
        'abcd'
        'abcd'
        'abcd'

    The following is the query result when you don't use the index.

    .. code-block:: sql

        SELECT * FROM tab WHERE c='abcd    ' USING INDEX tab.NONE;
         
        Query plan:
         Sequential scan(tab tab)
         
         c
        ======================
        'abcd'
        'abcd    '
        'abcd '

    As you can see in the above comparison result, the value in the **VARCHAR** type retrieved from the index will appear with the following empty string truncated when the covering index has been applied.
    
.. note:: If covering index optimization is available to be applied, the I/O performance can be improved because the disk I/O is decreased. Buf if you don't want covering index optimization in a special condition, specify a **NO_COVERING_IDX** hint to the query. For how to add a query, refer :ref:`sql-hint`.

.. _order-by-skip-optimization:

Optimizing ORDER BY Clause
--------------------------

The index including all columns in the **ORDER BY** clause is referred to as the ordered index.
 Optimizing the query with **ORDER BY** clause is no need for the additional sorting process(skip order by), because the query results are searched by the ordered index. In general, for an ordered index, the columns in the **ORDER BY** clause should be located at the front of the index.

.. code-block:: sql

    SELECT * 
    FROM tab 
    WHERE col1 > 0 
    ORDER BY col1, col2

*   The index consisting of *tab* (*col1*, *col2*) is an ordered index.
*   The index consisting of *tab* (*col1*, *col2*, *col3*) is also an ordered index. This is because the *col3*, which is not referred by the **ORDER BY** clause comes after *col1* and *col2* .
*   The index consisting of *tab* (*col1*) is not an ordered index.
*   You can use the index consisting of *tab* (*col3*, *col1*, *col2*) or *tab* (*col1*, *col3*, *col2*) for optimization. This is because *col3* is not located at the back of the columns in the **ORDER BY** clause.

Although the columns composing an index do not exist in the **ORDER BY** clause, you can use an ordered index if the column condition is a constant.

.. code-block:: sql

    SELECT * 
    FROM tab 
    WHERE col2=val 
    ORDER BY col1,col3;

If the index consisting of *tab* (*col1*, *col2*, *col3*) exists and the index consisting of *tab* (*col1*, *col2*) do not exist when executing the above query, the query optimizer uses the index consisting of *tab* (*col1*, *col2*, *col3*) as an ordered index. You can get the result in the requested order when you execute an index scan, so you don't need to sort records.

If you can use the sorted index and the covering index, use the latter first. If you use the covering index, you don't need to retrieve additional data, because the data result requested is included in the index page, and you won't need to sort the result if you are satisfied with the index order.

If the query doesn't include any conditions and uses an ordered index, the ordered index will be used under the condition that the first column meets the **NOT NULL** condition.

.. code-block:: sql

    CREATE TABLE tab (i INT, j INT, k INT);
    CREATE INDEX i_tab_j_k on tab (j,k);
    INSERT INTO tab VALUES (1,2,3),(6,4,2),(3,4,1),(5,2,1),(1,5,5),(2,6,6),(3,5,4);

The following example shows that indexes consisting of *tab* (*j*, *k*) become sorted indexes and no separate sorting process is required because **GROUP BY** is executed by *j* and *k* columns.

.. code-block:: sql

    SELECT i,j,k 
    FROM tab 
    WHERE j > 0 
    ORDER BY j,k;
     
    --  the  selection from the query plan dump shows that the ordering index i_tab_j_k was used and sorting was not necessary
    --  (/* --> skip ORDER BY */)
    Query plan:
    iscan
        class: tab node[0]
        index: i_tab_j_k term[0]
        sort:  2 asc, 3 asc
        cost:  1 card 0
    Query stmt:
    select tab.i, tab.j, tab.k from tab tab where ((tab.j> ?:0 )) order by 2, 3
    /* ---> skip ORDER BY */
     
                i            j            k
    =======================================
                5            2            1
                1            2            3
                3            4            1
                6            4            2
                3            5            4
                1            5            5
                2            6            6

The following example shows that *j* and *k* columns execute **ORDER BY** and the index including all columns are selected so that indexes consisting of *tab* (*j*, *k*) are used as covering indexes; no separate process is required because the value is selected from the indexes themselves.

.. code-block:: sql

    SELECT /*+ RECOMPILE */ j,k 
    FROM tab 
    WHERE j > 0 
    ORDER BY j,k;
     
    --  in this case the index i_tab_j_k is a covering index and also respects the orderind index property.
    --  Therefore, it is used as a covering index and sorting is not performed.
     
    Query plan:
    iscan
        class: tab node[0]
        index: i_tab_j_k term[0] (covers)
        sort:  1 asc, 2 asc
        cost:  1 card 0
     
    Query stmt: select tab.j, tab.k from tab tab where ((tab.j> ?:0 )) order by 1, 2
    /* ---> skip ORDER BY */
     
                j            k
    ==========================
                2            1
                2            3
                4            1
                4            2
                5            4
                5            5
                6            6

The following example shows that *i* column exists, **ORDER BY** is executed by *j* and *k* columns, and columns that perform **SELECT** are *i*, *j*, and *k*. Therefore, indexes consisting of *tab* (*i*, *j*, *k*) are used as covering indexes; separate sorting process is required for **ORDER BY** *j*, *k* even though the value is selected from the indexes themselves.

.. code-block:: sql

    CREATE INDEX i_tab_j_k ON tab (i,j,k);
    SELECT /*+ RECOMPILE */ i,j,k 
    FROM tab WHERE i > 0 
    ORDER BY j,k;
     
    -- since an index on (i,j,k) is now available, it will be used as covering index. However, sorting the results according to
    -- the ORDER BY  clause is needed.
    Query plan:
    temp(order by)
        subplan: iscan
                     class: tab node[0]
                     index: i_tab_i_j_k term[0] (covers)
                     sort:  1 asc, 2 asc, 3 asc
                     cost:  1 card 1
        sort:  2 asc, 3 asc
        cost:  7 card 1
     
    Query stmt: select tab.i, tab.j, tab.k from tab tab where ((tab.i> ?:0 )) order by 2, 3
     
                i            j            k
    =======================================
                5            2            1
                1            2            3
                3            4            1
                6            4            2
                3            5            4
                1            5            5
                2            6            6

.. note::
    Even if the type of a column in the ORDER BY clause is converted by using :func:`CAST`, ORDER BY optimization is executed when the sorting order is the same as before.
    
        +---------------------------------+
        | Before         | After          |
        +================+================+
        | numeric type   | numeric type   |
        +----------------+----------------+
        | string type    | string type    |
        +----------------+----------------+
        | DATETIME       | TIMESTAMP      |
        +----------------+----------------+
        | TIMESTAMP      | DATETIME       |
        +----------------+----------------+
        | DATETIME       | DATE           |
        +----------------+----------------+
        | TIMESTAMP      | DATE           |
        +----------------+----------------+
        | DATE           | DATETIME       |
        +----------------+----------------+

.. _index-descending-scan:

Index Scan in Descending Order
------------------------------

When a query is executed by sorting in descending order as follows, it usually creates a descending index. In this way, you do not have to go through addition procedure.

.. code-block:: sql

    SELECT * 
    FROM tab 
    [WHERE ...] 
    ORDER BY a DESC

However, if you create an ascending index and an descending index in the same column, the possibility of deadlock increases. In order to decrease the possibility of such case, CUBRID supports the descending scan only with ascending index. Users can use the **USE_DESC_IDX** hint to specify the use of the descending scan. If the hint is not specified, the following three query executions should be considered, provided that the columns listed in the **ORDER BY** clause can use the index.

*   Sequential scan + Sort in descending order
*   Scan in general ascending order + sort in descending
*   Scan in descending order that does not require a separate scan

Although the **USE_DESC_IDX** hint is omitted for the scan in descending order, the query optimizer decides the last execution plan of the three listed for an optimal plan.

.. note:: The **USE_DESC_IDX** hint is not supported for the join query.

.. code-block:: sql

    CREATE TABLE di (i INT);
    CREATE INDEX i_di_i on di (i);
    INSERT INTO di VALUES (5),(3),(1),(4),(3),(5),(2),(5);

The query will be executed as an ascending scan without **USE_DESC_IDX** hint.

.. code-block:: sql

    -- The same query, without the hint, will have a different output, since descending scan is not used.
     
    SELECT  * FROM di WHERE i > 0 LIMIT 3;
     
    Query plan:
     
    Index scan(di di, i_di_i, (di.i range (0 gt_inf max) and inst_num() range (min inf_le 3)) (covers))
     
                i
    =============
                1
                2
                3
    

If you add **USE_DESC_IDX** hint to the above query, a different result will be shown by descending scan.

.. code-block:: sql

    -- We now run the following query, using the ''use_desc_idx'' SQL hint:
     
    SELECT /*+ USE_DESC_IDX */ * FROM di WHERE i > 0 LIMIT 3;
     
    Query plan:
     Index scan(di di, i_di_i, (di.i range (0 gt_inf max) and inst_num() range (min inf_le 3)) (covers) (desc_index))
     
                i
    =============
                5
                5
                5

The following example requires descending order by **ORDER BY** clause. In this case, there is no **USE_DESC_IDX** but do the descending scan.

.. code-block:: sql

    -- We also run the same query, this time asking that the results are displayed in descending order. 
    -- However, no hint is given. 
    -- Since ORDER BY...DESC clause exists, CUBRID will use descending scan, even though the hint is not given, 
    -- thus avoiding to sort the records.
     
    SELECT * 
    FROM di 
    WHERE i > 0 
    ORDER BY i DESC LIMIT 3;
     
    Query plan:
     Index scan(di di, i_di_i, (di.i range (0 gt_inf max)) (covers) (desc_index))
     
                i
    =============
                5
                5
                5

.. _group-by-skip-optimization:

Optimizing GROUP BY Clause
--------------------------

**GROUP BY** caluse optimization works on the premise that if all columns in the **GROUP BY** clause are included in an index, you can use the index upon executing a query, so you don't execute a separate sorting job. The columns in the **GROUP BY** clause must exist in front side of the column forming the index.

.. code-block:: sql

    SELECT * 
    FROM tab 
    WHERE col1 > 0 
    GROUP BY col1,col2

*   You can use the index consisting of tab(col1, col2) for optimization.
*   The index consisting of tab(col1, col2, col3) can be used because col3 no referred by **GROUP BY** comes after col1 and col2.
*   You cannot use the index consisting of tab(col1) for optimization.
*   You also cannot use the index consisting of tab(col3, col1, col2) or tab(col1, col3, col2), because col3 is not located at the back of the column in the **GROUP BY** clause.

You can use the index if the column condition is a constant although the column consisting of the idex doesn't exist in the **GROUP BY** clause.

.. code-block:: sql

    SELECT * 
    FROM tab 
    WHERE col2=val 
    GROUP BY col1,col3

If there is any index that consists of tab(col1, col2, col3) in the above example, use the index for optimizing **GROUP BY**.

Row sorting by **GROUP BY** is not required, because you can get the result as the requested order on the index scan.

If the index consisting of the **GROUP BY** column and the first column of the index is **NOT NULL**, even though there is no **WHERE** clause, the **GROUP BY** optimization will be applied.

**GROUP BY** optimization is applied only when **MIN** () or **MAX** () are used in an aggregate function, and to use the two aggregate functions together, an identical column must be used.

.. code-block:: sql

    CREATE INDEX i_T_a_b_c ON T(a, b, c);
    SELECT a, MIN(b), c, MAX(b) FROM T WHERE a > 18 GROUP BY a, b;

**Example**

.. code-block:: sql

    CREATE TABLE tab (i INT, j INT, k INT);
    CREATE INDEX i_tab_j_k ON tab (j, k);
    INSERT INTO tab VALUES (1,2,3),(6,4,2),(3,4,1),(5,2,1),(1,5,5),(2,6,6),(3,5,4);

The following example shows that indexes consisting of tab(j,k) are used and no separate sorting process is required because **GROUP BY** is executed by j and k columns.

.. code-block:: sql

    SELECT i,j,k 
    FROM tab 
    WHERE j > 0 
    GROUP BY j,k;
     
    --  the  selection from the query plan dump shows that the index i_tab_j_k was used and sorting was not necessary
    --  (/* ---> skip GROUP BY */)
     
    Query plan:
    iscan
        class: tab node[0]
        index: i_tab_j_k term[0]
        sort:  2 asc, 3 asc
        cost:  1 card 0
     
    Query stmt:
    select tab.i, tab.j, tab.k from tab tab where ((tab.j> ?:0 )) group by tab.j, tab.k
    /* ---> skip GROUP BY */
                i            j            k
                5            2            1
                1            2            3
                3            4            1
                6            4            2
                3            5            4
                1            5            5
                2            6            6

The following example shows that an index consisting of tab(j,k) is used and no separate sorting process is required while **GROUP BY** is executed by j and k columns, no condition exists for j, and j column has **NOT NULL** attribute.

.. code-block:: sql

    ALTER TABLE tab CHANGE COLUMN j j INT NOT NULL;
    SELECT * 
    FROM tab 
    GROUP BY j,k;
     
    --  the  selection from the query plan dump shows that the index i_tab_j_k was used (since j has the NOT NULL constraint )
    --  and sorting was not necessary (/* ---> skip GROUP BY */)
    Query plan:
    iscan
        class: tab node[0]
        index: i_tab_j_k
        sort:  2 asc, 3 asc
        cost:  1 card 0
     
    Query stmt: select tab.i, tab.j, tab.k from tab tab group by tab.j, tab.k
    /* ---> skip GROUP BY */
    === <Result of SELECT Command in Line 1> ===
                i            j            k
    =======================================
                5            2            1
                1            2            3
                3            4            1
                6            4            2
                3            5            4
                1            5            5
                2            6            6
                
.. _multi-key-range-opt:

[번역]

다중 키 범위 최적화
-------------------

대부분의 질의가 **LIMIT** 절을 포함하고 있기 때문에 **LIMIT** 절을 최적화하는 것이 질의 성능에 매우 중요한데, 이에 해당하는 대표적인 최적화가 다중 키 범위 최적화(multiple key range optimization)이다. 다중 키 범위 최적화는 결과 생성에 필요한 인덱스 범위 전체를 스캔하지 않고, 인덱스 내의 일부 키 범위만 스캔하면서 Top N 정렬 방식을 통해 질의 결과를 생성한다. Top N 정렬은 전체 결과를 생성한 후에 이를 정렬하여 결과를 얻는 것이 아니라, 항상 최적의 N 개의 결과를 유지하는 방식으로 질의를 처리하기 때문에 매우 뛰어난 성능을 보인다.

예를 들어 내 친구들이 쓴 글 중에서 가장 최근 글을 10 개만 검색하는 경우, 내 전체 친구가 쓴 글을 모두 찾아서 정렬한 후에 결과를 찾는 방법 보다는 각 친구가 쓴 최근 글 10 개씩만을 찾아서 정렬을 유지하는 방식으로 인덱스를 스캔하는 CUBRID만의 최적화 기법이다.


다중 키 범위 최적화를 사용할 수 있는 예는 다음과 같다. 

.. code-block:: sql

    CREATE TABLE t (a int, b int); 
    CREATE INDEX i_t_a_b ON t (a,b);
    
    -- Multiple key range optimization
    SELECT * 
    FROM t 
    WHERE a IN (1,2,3) 
    ORDER BY b 
    LIMIT 2; 

    Query plan: 
    iscan 
    class: t node[0] 
    index: i_t_a_b term[0] (covers) (multi_range_opt) 
    sort: 1 asc, 2 asc 
    cost: 1 card 0 

단일 테이블에서는 다음과 같은 조건들이 만족되었을 경우에 다중 키 범위 최적화가 수행된다. 

::

    SELECT /*+ hints */ … 
    FROM table
    WHERE col_1 = ? AND col_2 = ? AND … AND col(j-1) = ?
    AND col_(j) IN (?, ?, … )
    AND col_(j+1) = ? AND … AND col_(p-1) = ?
    AND key_filter_terms
    ORDER BY col_(p) [ASC|DESC], col_(p+1) [ASC|DESC],… col_(p+k-1) [ASC|DESC]
    FOR orderbynum_pred | LIMIT n;

먼저 *orderbynum_pred* 조건이 명시되었다면 이 조건은 유효해야 하고, **ORDERBY_NUM** 또는 **LIMIT**\ 를 통해서 지정된 최종 결과의 상한 크기(*n*)이 multi_range_optimization_limit 시스템 파라미터 값보다 크지 않아야 한다.

또한 다중 키 범위 최적화에 적합한 인덱스가 필요한데, **ORDER BY** 절에 명시된 모든 *k* 개의 컬럼을 커버해야 한다. 즉, 인덱스 상에서 **ORDER BY** 절에 명시된 컬럼들을 모두 포함해야 하고, 컬럼들의 순서와 정렬 방향이 일치해야 한다. 또한 **WHERE** 절에서 사용되는 모든 칼럼을 포함해야 한다.

인덱스를 구성하는 칼럼들 중 

* 범위 조건(예를 들어, IN 조건) 앞의 칼럼들은 동일(=) 조건으로 표현된다.
* 범위 조건을 가진 칼럼이 하나만 존재한다. 
* 범위 조건 이후의 칼럼들은 키 필터로 존재한다. 
* 데이터 필터 조건이 없어야 한다. 다시 말해, 인덱스는 **WHERE** 절에서 사용되는 모든 칼럼을 포함해야 한다.
* 키 필터 이후의 칼럼들은 **ORDER BY** 절에 존재한다. 
* 키 필터 조건의 칼럼들은 반드시 **ORDER BY** 절의 칼럼이 아니어야 한다.
* 상관 부질의(correlated subquery)를 포함한 키 필터 조건이 포함되어 있다면, 이에 연관된 컬럼은 범위 조건이 아닌 조건으로 WHERE 절에 포함되어야 한다. 

다음과 같은 예에 다중 키 범위 최적화가 수행된다. 

.. code-block:: sql

    CREATE TABLE t (a INT, b INT, c INT, d INT, e INT); 
    CREATE INDEX i_t_a_b ON t (a,b,c,d,e); 
    
    SELECT * 
    FROM t 
    WHERE a = 1 AND b = 3 AND c IN (1,2,3) AND d = 3 
    ORDER BY e 
    LIMIT 2; 

다중 테이블을 포함하는 JOIN 질의에서는 다음의 경우 최적화가 수행된다. 

::

    SELECT /*+ hints */ ...
    FROM table_1, table_2, ... table_(sort), ...
    WHERE col_1 = ? AND col_2 = ? AND ...
    AND col_(j) IN (?, ?, ... )
    AND col_(j+1) = ? AND ... AND col_(p-1) = ?
    AND key_filter_terms
    AND join_terms
    ORDER BY col_(p) [ASC|DESC], col_(p+1) [ASC|DESC], ... col_(p+k-1) [ASC|DESC]
    FOR ordbynum_pred | LIMIT n;

JOIN 질의에 대해서 다중 키 범위 최적화가 적용되기 위해서는 다음과 같은 조건을 만족해야 한다.

* ORDER BY 절에 존재하는 칼럼들은 하나의 테이블에만 존재하는 칼럼들이며, 이 테이블은 단일 테이블 질의에서 다중 키 범위 최적화에 의해 요구되는 조건을 모두 만족해야 한다. 이 테이블을 정렬 테이블(sort table)이라고 하자. 
*  정렬 테이블과 외부 테이블들(outer tables) 간의 JOIN 조건에 명시된 정렬 테이블의 칼럼들은 모두 인덱스에 포함되어야 한다. 즉, 데이터 필터링 조건이 없어야 한다. 
*  정렬 테이블과 내부 테이블들(inner tables) 간의 JOIN 조건에 명시된 정렬 테이블의 칼럼들은 범위 조건이 아닌 조건으로 WHERE 절에 포함되어야 한다. 


.. note:: 다중 키 범위 최적화가 적용될 수 있는 대부분의 경우에 다중 키 범위 최적화가 가장 좋은 성능을 보장하지만, 특정한 상황에서 최적화를 원하지 않는다면 질의에 **NO_MULTI_RANGE_OPT** 힌트를 명시하면 된다. 힌트를 지정하는 방법은 :ref:`sql-hint`\ 를 참고하면 된다.

.. _index-skip-scan:

Index Skip Scan
---------------

Index Skip Scan (also known as ISS) is an optimization method that allows ignoring the first column of an index when the first column of the index is not included in the condition but the following column is included in the condition (in most cases, =).
Generally, ISS should consider several columns (C1, C2, ..., Cn). Here, a query has the conditions for the consecutive columns and the conditions are started from the second column (C2) of the index.

.. code-block:: sql

    INDEX (C1, C2, ..., Cn);
     
    SELECT ... WHERE C2 = x and C3 = y and ... and Cp = z; -- p <= n
    SELECT ... WHERE C2 < x and C3 >= y and ... and Cp BETWEEN (z and w); -- other conditions than equal
    
The query optimizer eventually determines whether ISS is the most optimum access method based on the cost. ISS is applied under very specific situations, such as when the first column of an index has a very small number of **DISTINCT** values compared to the number of records. In this case, ISS provides higher performance compared to Index Full Scan. For example, when the first column of index columns has very low cardinality, such as the value of men/women or millions of records with the value of 1~100, it may be inefficient to perform index scan by using the first column value. So ISS is useful in this case.

ISS skips reading most of the index pages in the disk and uses range search which is dynamically readjusted. Generally, ISS can be applied to a specific scenario when the number of **DISTINCT** values in the first column is very small. If ISS is applied to this case, ISS provides significantly higher performance than the index full scan. However, it means improper index creation that ISS is applied to a lot queries. So DBA should consider whether readjusting the indexes or not.

.. code-block:: sql

    CREATE TABLE t (name string, gender char (1), birthday datetime);
     
    CREATE INDEX idx_t_gen_name on t (gender, name);
    -- Note that gender can only have 2 values, 'M' and 'F' (low cardinality)
     
    -- this would qualify to use Index Skip Scanning:
    SELECT * 
    FROM t 
    WHERE name = 'SMITH';

ISS is not applied in the following cases:

*   Filtered index
*   The first column of an index is a range filter or key filter
*   Hierarchical query
*   Aggregate function included

