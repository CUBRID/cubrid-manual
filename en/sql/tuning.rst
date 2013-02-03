******************
Query Optimization
******************

Updating Statistics
===================

With the **UPDATE STATISTICS ON** statement, you can generate internal statistics used by the query processor. Such statistics allow the database system to perform query optimization more efficiently. ::

	UPDATE STATISTICS ON { table_spec [ {, table_spec } ] | ALL CLASSES | CATALOG CLASSES } [ ; ]
	table_spec :
	single_table_spec
	( single_table_spec [ {, single_table_spec } ] )
	single_table_spec :
	[ ONLY ] table_name
	| ALL table_name [ ( EXCEPT table_name ) ]

*   **ALL CLASSES** : If the **ALL CLASSES** keyword is specified, the statistics on all the tables existing in the database are updated.

Checking Statistics Information
===============================

You can check the statistics Information with the session command of the CSQL Interpreter. ::

	csql> ;info stats <table_name>
	
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

Using SQL Hint
==============

Using hints can affect the performance of query execution. you can allow the query optimizer to create more efficient execution plan by referring the SQL HINT. The SQL HINTs related tale join, index, and statistics information are provided by CUBRID. ::

	CREATE /*+ NO_STATS */ [TABLE | CLASS] ...;
	ALTER /*+ NO_STATS */ [TABLE | CLASS] ...;
	 
	CREATE /*+ NO_STATS */ INDEX ...;
	ALTER /*+ NO_STATS */ INDEX ...;
	DROP /*+ NO_STATS */ INDEX ...;
	 
	SELECT /*+ hint [ { hint } ... ] */
	SELECT --+ hint [ { hint } ... ]
	SELECT //+ hint [ { hint } ... ]
	 
	hint :
	USE_NL[(spec-name[{, spec-name}...])]
	USE_IDX[(spec-name[{, spec-name}...])]
	USE_MERGE[(spec-name[{, spec-name}...])]
	ORDERED
	USE_DESC_IDX
	NO_DESC_IDX
	NO_COVERING_IDX

SQL hints are specified by using plus signs and comments. CUBRID interprets this comment as a list of hints separated by blanks. The hint comment must appear after the **SELECT**, **CREATE**, or **ALTER** keyword, and the comment must begin with a plus sign (+), following the comment delimiter.

*   *hint* : The following hints can be specified.

    *   **USE_NL** : Related to a table join, the query optimizer creates a nested loop join execution plan with this hint.
    *   **USE_MERGE** : Related to a table join, the query optimizer creates a sort merge join execution plan with this hint.
    *   **ORDERED** : Related to a table join, the query optimizer create a join execution plan with this hint, based on the order of tables specified in the FROM clause. The left table in the FROM clause becomes the outer table; the right one becomes the inner table.
    *   **USE_IDX** : Related to a index, the query optimizer creates a index join execution plan corresponding to a specified table with this hint.
    *   **USE_DESC_IDX** : This is a hint for the scan in descending index. For more information, see :ref:`index-descending-scan`.
    *   **NO_DESC_IDX** : This is a hint not to use the descending index.
    *   **NO_COVERING_IDX** : This is a hint not to use the covering index. For details, see :ref:`covering-index`.
    *   **NO_STATS** : Related to statistics information, the query optimizer does not update statistics information. Query performance for the corresponding queries can be improved; however, query plan is not optimized because the information is not updated.
    *   **RECOMPILE** : Recompiles the query execution plan. This hint is used to delete the query execution plan stored in the cache and establish a new query execution plan.

*   *spec_name* : If the *spec_name* is specified together with **USE_NL**, **USE_IDX** or **USE_MERGE**, the specified join method applies only to the *spec_name*. If **USE_NL** and **USE_MERGE** are specified together, the given hint is ignored. In some cases, the query optimizer cannot create a query execution plan based on the given hint. For example, if **USE_NL** is specified for a right outer join, the query is converted to a left outer join internally, and the join order may not be guaranteed.

The following example shows how to retrieve the years when Sim Kwon Ho won medals and the types of medals. Here, a nested loop join execution plan needs to be created which has the *athlete* table as an outer table and the *game* table as an inner table. It can be expressed by the following query. The query optimizer creates a nested loop join execution plan that has the *game* table as an outer table and the *athlete* table as an inner table.

.. code-block:: sql

	SELECT /*+ USE_NL ORDERED  */ a.name, b.host_year, b.medal
	FROM athlete a, game b WHERE a.name = 'Sim Kwon Ho' AND a.code = b.athlete_code;
	  name                    host_year  medal
	=========================================================
	  'Sim Kwon Ho'                2000  'G'
	  'Sim Kwon Ho'                1996  'G'
	2 rows selected.

.. note::
	For how to specify the index to use in the query, see :ref:`index-hint-syntax`.

The following example shows how to retrieve query execution time with **NO_STAT** hint to improve the functionality of drop partitioned table (*before_2008*); any data is not stored in the table. Assuming that there are more than 1 million data in the *participant2* table. The execution time in the example depends on system performance and database configuration.

.. code-block:: sql

	-- Not using NO_STATS hint
	ALTER TABLE participant2 DROP partition before_2008;

	SQL statement execution time: 31.684550 sec

	-- Using NO_STATS hint
	ALTER /*+ NO_STATS */ TABLE participant2 DROP partition before_2008;

	SQL statement execution time: 0.025773 sec

Viewing Query Plan
==================

To view a query plan for a CUBRID SQL query, change the value of the optimization level by using the **SET OPTIMIZATION** statement. You can get the current optimization level value by using the **GET OPTIMIZATION** statement. 

The CUBRID query optimizer determines whether to perform query optimization and output the query plan by referencing the optimization level value set by the user. The query plan is displayed as standard output; the following explanations are based on the assumption that the plan is used in a terminal-based program such as the CSQL Interpreter. In the CSQL query editor, you can view execution plan by executing the **;plan** command. See :ref:`csql-session-commands`. For information on how to view a query plan, see the CUBRID Manager. ::

	SET OPTIMIZATION LEVEL opt-level [;]
	GET OPTIMIZATION LEVEL [ { TO | INTO } variable ] [;]

*   *opt-level* : A value that specifies the optimization level. It has the following meanings.

    *   0: Does not perform query optimization. The query is executed using the simplest query plan. This value is used only for debugging.
    *   1: Create a query plan by performing query optimization and executes the query. This is a default value used in CUBRID, and does not have to be changed in most cases.
    *   2: Creates a query plan by performing query optimization. However, the query itself is not executed. In generall, this value is not used; it is used together with the following values to be set for viewing query plans.
	
    *   257: Performs query optimization and outputs the created query plan. This value works for displaying the query plan by internally interpreting the value as 256+1 related with the value 1.
	
    *   258: Performs query optimization and outputs the created query plan. The difference from the value 257 is that the query is not executed. That is, this value works for displaying the query plan by internally interpreting the value as 256+2 related with the value 2. This setting is useful to examine the query plan but not to intend to see the query results.
	
    *   513: Performs query optimization and outputs the detailed query plan. This value works for displaying more detailed query plan than the value 257 by internally interpreting the value as 512+1.
	
    *   514: Performs query optimization and outputs the detailed query plan. However, the query is not executed. This value works for displaying more detailed query plan than the value 258 by internally interpreting the value as 512+2.

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

.. _tuning-index:
	
Using Indexes
=============

.. _index-hint-syntax:

Index Hint Syntax
-----------------

The index hint syntax allows the query processor to select a proper index by specifying the index in the query.

{USE|FORCE|IGNORE} INDEX syntax is specified after "FROM table" clause.

::

	SELECT ... FROM ...
	  USE INDEX  (index_spec [, index_spec  ...] ) 
	| FORCE INDEX ( index_spec [, index_spec ...] ) 
	| IGNORE INDEX ( index_spec [, index_spec ...] )
	WHERE ...
	
	index_spec :
	 [table_name.]index_name

*	**USE INDEX** ( *index_spec*, *index_spec*, ... ): forces to use only one index among specified indexes.
*	**FORCE INDEX** ( *index_spec*, *index_spec*, ... ): works like **USING INDEX** clause, but it assumes that a cost of sequential scanning cost is very expensive. In other words, sequential scanning is executed only if there is no method to use the specified indexes to find the rows on the table.
*	**IGNORE INDEX** ( *index_spec*, *index_spec*, ... ): forces not to use the specified indexes when scanning.

The **USING INDEX** *index_name* clause should be specified after the **WHERE** clause of the **SELECT** statement; it works like **USE INDEX** (*index_name*). If (+) is specified after the index name, it works like **FORCE INDEX**;if (-) is specified after the index name, it works like **IGNORE INDEX**.

**USING INDEX NONE** syntax forces not to use the all indexes.

**USING ALL EXCEPT** syntax forces not to use only the specified indexes.

::

	SELECT ... FROM . . . WHERE . . .
	  USING INDEX { [table_name.]NONE | [ ALL EXCEPT ] index_spec [ {, index_spec } ...] }  

	index_spec :
	 [table_name.]index_name [{(+)|(-)}]

*   **NONE** : All indexes are not used and sequential scanning is executed.
*   **ALL EXCEPT** : All indexes except the specified index can be used on query execution.
*   (+) : When (+) is specified after the index name, the possibility to use that index is increased.
*   (-) : When (-) is specified after the index name, that index is not used on the query execution.

The following example is creating an index based on the table creation statement of the *athlete* table.

.. code-block:: sql

	CREATE TABLE athlete (
	   code             SMALLINT    NOT NULL PRIMARY KEY,
	   name             VARCHAR(40) NOT NULL,
	   gender           CHAR(1)     ,
	   nation_code      CHAR(3)     ,
	   event            VARCHAR(30)
	   );
	   
	CREATE UNIQUE INDEX athlete_idx ON athlete(code, nation_code);
	CREATE INDEX char_idx ON athlete(gender, nation_code);

For the following query, the query optimizer can select the index scan that uses the *athlete_idx* index.

.. code-block:: sql

	SELECT * FROM athlete WHERE gender='M' AND nation_code='USA';

	
If the index scanning cost is lower than the sequantial scanning cost, the index scanning is executed.
Below two queries do the same behavior and they use always char_idx index to execute.

.. code-block:: sql

	SELECT /*+ RECOMPILE */ * FROM athlete USE INDEX (char_idx) WHERE gender='M' AND nation_code='USA';

	SELECT /*+ RECOMPILE */ * FROM athlete WHERE gender='M' AND nation_code='USA'
	USING INDEX char_idx;

Below two queries do the same behavior and they don't use char_idx index to execute.

.. code-block:: sql
	
	SELECT /*+ RECOMPILE */ * FROM athlete IGNORE INDEX (char_idx) WHERE gender='M' AND nation_code='USA';

	SELECT /*+ RECOMPILE */ * FROM athlete WHERE gender='M' AND nation_code='USA'
	USING INDEX char_idx(-);

Below query always forces to do the sequential scanning.

.. code-block:: sql

	SELECT * FROM athlete WHERE gender='M' AND nation_code='USA'
	USING INDEX NONE;

Below query forces to be possible to use all indexes execept char_idex index.

.. code-block:: sql

	SELECT * FROM athlete WHERE gender='M' AND nation_code='USA'
	USING INDEX ALL EXCEPT char_idx;

	
	
When two or more indexes have been specified in the **USING INDEX** clause, the query optimizer selects the proper one of the specified indexes.

.. code-block:: sql

	SELECT * FROM athlete USE INDEX (char_idx, athlete_idx) WHERE gender='M' AND nation_code='USA';

	SELECT * FROM athlete WHERE gender='M' AND nation_code='USA'
	USING INDEX char_idx, athlete_idx;

When a query is made for several tables, you can specify a table to perform index scan by using a specific index and another table to perform sequential scan. The query has the following format.

.. code-block:: sql

	SELECT ... FROM tab1, tab2 WHERE ... USING INDEX tab1.idx1, tab2.NONE;

When executing a query with the index hint syntax, the query optimizer considers all available indexes on the table for which no index has been specified. For example, when the *tab1* table includes *idx1* and *idx2* and the *tab2* table includes *idx3*, *idx4*, and *idx5*, if indexes for only *tab1* are specified but no indexes are specified for *tab2*, the query optimizer considers the indexes of *tab2*.

.. code-block:: sql

	SELECT ... FROM tab1, tab2 USE INDEX(tab1.idx1) WHERE ... ;
	SELECT ... FROM tab1, tab2 WHERE ... USING INDEX tab1.idx1;

*   The sequential scan of table *tab1* and *idx1* index scan are compared, and the optimal query plan is selected.
*   The sequential scan of table *tab2* and *idx3*, *idx4*, and *idx5* index scan are compared, and the optimal query plan is selected.

To perform index scan for only the *tab2* table and sequential scan for the *tab1* table, specify *tab1*.NONE not to perform index scan for the *tab1* table.

.. code-block:: sql

	SELECT * from tab1,tab2 WHERE tab1.id > 2 and tab2.id < 3 USING index i_tab2_id, tab1.NONE;

Filtered Index
--------------

The filtered index is used to sort, search, or operate a well-defined partials set for one table. It is called the partial index since only some of indexes that satisfy the condition are used. To guarantee using the filtered indexes, the **USING INDEX** syntax must be added as follows:

.. code-block:: sql

	SELECT * FROM blogtopic WHERE postDate>'2010-01-01' USING INDEX my_filter_index;

::

	CREATE /* hints */ INDEX index_name
			ON table_name (col1, col2, ...) WHERE <filter_predicate>;
	 
	ALTER  /* hints */ INDEX index_name
			[ ON table_name (col1, col2, ...) [ WHERE <filter_predicate> ] ]
			REBUILD;
	 
	<filter_predicate> ::= <filter_predicate> AND <expression> | <expression>

*   <*filter_predicate*>: Condition to compare the column and the constant. When there are several conditions, filtering is available only when they are connected by using **AND**. The filter conditions can include most of the operators and functions supported by CUBRID. However, the date/time function that shows the current date/time (ex: :func:`SYS_DATETIME`) or random functions (ex: :func:`RAND`), which outputs different results for one input are not allowed.

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

To process queries that are interested in open bugs, specify the index in the USING INDEX statement. It will allow to create query results by accessing less index pages through filtered indexes.

.. code-block:: sql

	SELECT * FROM bugs
	WHERE Author = 'madden' AND Subject LIKE '%fopen%' AND Closed = 0;
	USING INDEX idx_open_bugs;
	 
	SELECT * FROM bugs
	WHERE CreationDate > CURRENT_DATE - 10 AND Closed = 0;
	USING INDEX idx_open_bugs;

.. warning::

	If you execute queries by specifying indexes with index hint syntax, you may have incorrect query results as output even though the conditions of creating filtered indexes does not meet the query conditions.

**Constraints**

Only generic indexes are allowed as filtered indexes. For example, the filtered unique index is not allowed. The following cases are not allowed as filtering conditions.

* Functions, which output different results with the same input, such as date/time function or random function

  .. code-block:: sql
  
	CREATE INDEX idx ON bugs(creationdate) WHERE creationdate > SYS_DATETIME;
	 
	ERROR: before ' ; '
	'sys_datetime ' is not allowed in a filter expression for index.
	 
	CREATE INDEX idx ON bugs(bugID) WHERE bugID > RAND();
	 
	ERROR: before ' ; '
	'rand ' is not allowed in a filter expression for index.
	
* When the **OR** operator is used

  .. code-block:: sql

	CREATE INDEX IDX ON bugs(bugID) WHERE bugID > 10 OR bugID = 3;
	 
	In line 1, column 62,
	 
	ERROR: before ' ; '
	' or ' is not allowed in a filter expression for index.

* **INCR** () function and **DECR** () function
* Serial-related functions
* Aggregate functions such as **MIN** (), **MAX** (), and **STDDEV** ()
* Conditions for types where indexes cannot be created

  * The operators and functions where parameter is the **SET** type
  * **IS NULL** operator can be used only when at least one column among the columns of the index is not **NULL**.

* The **IS NULL** operator can be used only when at least one column of an index is not NULL.

  .. code-block:: sql
  
	CREATE TABLE t (a INT, b INT);
	Current transaction has been committed.
	 
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
	Current transaction has been committed.

* Index Skip Scan (ISS) is not allowed for the filtered indexes.
* The length of condition string used for the filtered index is limited to 128 characters.

  .. code-block:: sql

	CREATE TABLE t(VeryLongColumnNameOfTypeInteger INT);
	1 command(s) successfully processed.
	 
	CREATE INDEX idx ON t(VeryLongColumnNameOfTypeInteger) WHERE VeryLongColumnNameOfTypeInteger > 3 AND VeryLongColumnNameOfTypeInteger < 10 AND sqrt(VeryLongColumnNameOfTypeInteger) < 3 AND SQRT(VeryLongColumnNameOfTypeInteger) < 10;
	ERROR: before ' ; '
	The maximum length of filter predicate string must be 128.

Function-based Index
--------------------

Function-based index is used to sort or find the data based on the combination of values of table rows by using a specific function. For example, to find the space-ignored string, it can be used to optimize the query by using the function that provides the feature. In addition, it is useful to search the non-case-sensitive names. ::

	CREATE /* hints */ [REVERSE] [UNIQUE] INDEX index_name
			ON table_name (function_name (argument_list));
	ALTER /* hints */ [REVERSE] [UNIQUE] INDEX index_name
			[ ON table_name (function_name (argument_list)) ]
			REBUILD;

After the following indexes have been created, the **SELECT** query automatically uses the function-based index.

.. code-block:: sql

	CREATE INDEX idx_trim_post ON posts_table(TRIM(keyword));
	SELECT * FROM posts_table WHERE TRIM(keyword) = 'SQL';

If a function-based index is created by using the **LOWER** function, it can be used to search the non-case-sensitive names.

.. code-block:: sql

	CREATE INDEX idx_last_name_lower ON clients_table(LOWER(LastName));
	SELECT * FROM clients_table WHERE LOWER(LastName) = LOWER('Timothy');

To make an index selected while creating a query plan, the function used for the index should be used for the query condition in the same way. The **SELECT** query above uses the last_name_lower index created above. However, this index is not used for the following condition:

.. code-block:: sql

	SELECT * FROM clients_table
	WHERE LOWER(CONCAT('Mr. ', LastName)) = LOWER('Mr. Timothy');

In addition, to make the function-based index used by force, use the **USING INDEX** syntax.

.. code-block:: sql

	CREATE INDEX i_tbl_first_four ON tbl(LEFT(col, 4));
	SELECT * FROM clients_table WHERE LEFT(col, 4) = 'CAT5' USING INDEX i_tbl_first_four;

**Constraints**

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

.. _allowed-function-in-function-index:

Functions which can be used on the function-based indexes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

Optimizing ORDER BY Clause
--------------------------

The index including all columns in the **ORDER BY** clause is referred to as the ordered index.
 Optimizing the query with ORDER BY clause is no need for the additional sorting process(skip order by), because the query results are searched by the ordered index. In general, for an ordered index, the columns in the **ORDER BY** clause should be located at the front of the index.

.. code-block:: sql

	SELECT * FROM tab WHERE col1 > 0 ORDER BY col1, col2

*   The index consisting of *tab* (*col1*, *col2*) is an ordered index.
*   The index consisting of *tab* (*col1*, *col2*, *col3*) is also an ordered index. This is because the *col3*, which is not referred by the **ORDER BY** clause comes after *col1* and *col2* .
*   The index consisting of *tab* (*col1*) is not an ordered index.
*   You can use the index consisting of *tab* (*col3*, *col1*,*col2*) or *tab* (*col1*, *col3*, *col2*) for optimization. This is because *col3* is not located at the back of the columns in the **ORDER BY** clause.

Although the columns composing an index do not exist in the **ORDER BY** clause, you can use an ordered index if the column condition is a constant.

.. code-block:: sql

	SELECT * FROM tab WHERE col2=val ORDER BY col1,col3;

If the index consisting of *tab* (*col1*, *col2*, *col3*) exists and the index consisting of *tab* (*col1*, *col2*) do not exist when executing the above query, the query optimizer uses the index consisting of *tab* (*col1*, *col2*, *col3*) as an ordered index. You can get the result in the requested order when you execute an index scan, so you don't need to sort records.

If you can use the sorted index and the covering index, use the latter first. If you use the covering index, you don't need to retrieve additional data, because the data result requested is included in the index page, and you won't need to sort the result if you are satisfied with the index order.

If the query doesn't include any conditions and uses an ordered index, the ordered index will be used under the condition that the first column meets the **NOT NULL** condition.

.. code-block:: sql

	CREATE TABLE tab (i INT, j INT, k INT);
	CREATE INDEX i_tab_j_k on tab (j,k);
	INSERT INTO tab VALUES (1,2,3),(6,4,2),(3,4,1),(5,2,1),(1,5,5),(2,6,6),(3,5,4);

The following example shows that indexes consisting of *tab* (*j*, *k*) become sorted indexes and no separate sorting process is required because **GROUP BY** is executed by *j* and *k* columns.

.. code-block:: sql

	SELECT i,j,k FROM tab WHERE j > 0 ORDER BY j,k;
	 
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

	SELECT /*+ RECOMPILE */ j,k FROM tab WHERE j > 0 ORDER BY j,k;
	 
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
	SELECT /*+ RECOMPILE */ i,j,k FROM tab WHERE i > 0 ORDER BY j,k;
	 
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
	Even if the type of a column in the ORDER BY clause is converted, ORDER BY optimization is executed when the sorting order is the same as before.
	
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

When a query is executed by sorting in descending order as follows, it usually creates a reverse index. In this way, you do not have to go through addition procedure.

.. code-block:: sql

	SELECT * FROM tab [WHERE ...] ORDER BY a DESC

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

The following example shows how to execute queries by using the **USE_DESC_IDX** hint.

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

Even though the example below is the same as that above, the output result may be different because it cannot be scanned in descending order; which is caused by not using the **USE_DESC_IDX** hint.

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

The following example shows how to sort in descending order by using **ORDER BY DESC**; the example below is the same as that above. There is no **USE_DESC_IDX** hint in the following example; however it is scanned in descending order and the result is the same as the example 1.

.. code-block:: sql

	-- We also run the same query , this time asking that the results are displayed in descending order. However, no hint will be given. Since the
	-- ORDER BY...DESC clause is present, CUBRID will use descending scan, even if the hint is  was not given, thus avoiding to sort the records.
	 
	SELECT * FROM di WHERE i > 0 ORDER BY i DESC LIMIT 3;
	 
	Query plan:
	 Index scan(di di, i_di_i, (di.i range (0 gt_inf max)) (covers) (desc_index))
	 
				i
	=============
				5
				5
				5

Optimizing GROUP BY Clause
--------------------------

**GROUP BY** caluse optimization works on the premise that if all columns in the **GROUP BY** clause are included in an index, you can use the index upon executing a query, so you don't execute a separate sorting job. The columns in the **GROUP BY** clause must exist in front side of the column forming the index.

.. code-block:: sql

	SELECT * FROM tab WHERE col1 > 0 GROUP BY col1,col2

*   You can use the index consisting of tab(col1, col2) for optimization.
*   The index consisting of tab(col1, col2, col3) can be used because col3 no referred by **GROUP BY** comes after col1 and col2.
*   You cannot use the index consisting of tab(col1) for optimization.
*   You also cannot use the index consisting of tab(col3, col1, col2) or tab(col1, col3, col2), because col3 is not located at the back of the column in the **GROUP BY** clause.

You can use the index if the column condition is a constant although the column consisting of the idex doesn't exist in the **GROUP BY** clause.

.. code-block:: sql

	SELECT * FROM tab WHERE col2=val GROUP BY col1,col3

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
	CREATE INDEX i_tab_j_k ON tab (j,k);
	INSERT INTO tab VALUES (1,2,3),(6,4,2),(3,4,1),(5,2,1),(1,5,5),(2,6,6),(3,5,4);

The following example shows that indexes consisting of tab(j,k) are used and no separate sorting process is required because **GROUP BY** is executed by j and k columns.

.. code-block:: sql

	SELECT i,j,k FROM tab WHERE j > 0 GROUP BY j,k;
	 
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
	SELECT * FROM tab GROUP BY j,k;
	 
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

Index Skip Scan
---------------

Index Skip Scan (also known as ISS) is an optimization method that allows ignoring the first column of an index when the first column of the index is not included in the condition but the following column is included in the condition (in most cases, =).

Generally, ISS should consider several columns (C1, C2, ..., Cn). Here, a query has the conditions for the consecutive columns and the conditions are started from the second column (C2) of the index.

.. code-block:: sql

	INDEX (C1, C2, ..., Cn);
	 
	SELECT ... WHERE C2 = x and C3 = y and ... and Cp = z; -- p <= n
	SELECT ... WHERE C2 < x and C3 >= y and ... and Cp BETWEEN (z and w); -- other conditions than equal

The query optimizer eventually determines whether ISS is the most optimum access method based on the cost. ISS is applied under very specific situations, such as when the first column of an index has a very small number of **DISTINCT** values compared to the number of records. In addition, ISS should provide higher performance compared to Index Full Scan. For example, when the first column of index columns has very low cardinality, such as the value of men/women or hundreds of thousands of records with the value of 1~100, it may be inefficient to perform index scan by using the first column value. So ISS is useful in this case.

ISS skips reading most of the index pages in the disk and uses range search which is dynamically readjusted. Generally, ISS can be applied to a specific scenario when the number of **DISTINCT** values in the first column is very small. If ISS is applied to this case, ISS provides significantly higher performance than the index full scan.

.. code-block:: sql

	CREATE TABLE t (name string, gender char (1), birthday datetime);
	 
	CREATE INDEX idx_t_gen_name on t (gender, name);
	-- Note that gender can only have 2 values, 'M' and 'F' (low cardinality)
	 
	-- this would qualify to use Index Skip Scanning:
	SELECT * FROM t WHERE name = 'SMITH';

ISS is not applied in the following cases:

*   Filtered index
*   The first column of an index is a range filter or key filter
*   Hierarchical query
*   Aggregate function included
