
:meta-keywords: index definition, unique index, create index, alter index, drop index
:meta-description: Define table indexes using create index, alter index, drop index statements.

***************************
INDEX DEFINITION STATEMENTS
***************************

CREATE INDEX
============

Creates an index to a specified table by using the **CREATE INDEX** statement. Regarding writing index name, see :doc:`/sql/identifier`.

For how to use indexes on the **SELECT** statement like Using SQL Hint, Descending Index, Covering Index, Index Skip Scan, **ORDER BY** Optimization and **GROUP BY** Optimization, and how to create Filtered Index and Function-based Index, see :doc:`/sql/tuning`.

::

    CREATE [UNIQUE] INDEX index_name ON [schema_name.]table_name <index_col_desc2> ;
     
        <index_col_desc2> ::=
                  {
                     (  {column_name | function_name (argument_list)} [ASC | DESC]
                       [{, {column_name | function_name (argument_list)} [ASC | DESC]} ...] )
                  }
                  [WHERE <filter_predicate> ]
                  [WITH <index_with_clause> [{, <index_with_clause>]} ...]
                  [INVISIBLE]
                  [COMMENT 'index_comment_string’]

             <index_with_clause> ::= {ONLINE [PARALLEL parallel_count]} | <index_with_option>
             <index_with_option> ::= {DEDUPLICATE ‘=‘ deduplicate_level}

*   **UNIQUE**: creates an index with unique values.
*   *index_name*: specifies the name of the index to be created. The index name must be unique in the table.

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name*: specifies the name of the table where the index is to be created.
*   *column_name*: specifies the name of the column where the index is to be applied. To create a composite index, specify two or more column names.
*   **ASC** | **DESC**: specifies the sorting order of columns.
*   *deduplicate_level*: specifies the deduplicate level (0 to 14). For details, see `DEDUPLICATE`_.

.. note::

    *   *deduplicate_level* is an integer from 0 to 14. 
    *   When this parameter is set to 0, it indicates an index configuration where data deduplication within indexes is not performed. This configuration is the same as that in CUBRID 11.2 or an earlier version.

*   <*filter_predicate*>: defines the conditions to create filtered indexes. When there are several comparison conditions between a column and a constant, filtering is available only when the conditions are connected by using **AND**. Refer to :ref:`filtered-index` for more details.
*   *function_name* (*argument_list*): Defines the conditions to create function-based indexes. Regarding this, definitely watch :ref:`function-index`.

*   **WITH ONLINE**: creates the index while allowing changes of table data from other transactions. If **PARALLEL** is not specified, the index is created using the transaction thread. <*parallel_count*> is the number of threads to be used for creating the index and it must be a integer between 1 and 16.

*   **INVISIBLE**: creates the index with its status set to **INVISIBLE**, meaning queries executed will not take into account the index. If **INVISIBLE** is omitted, the index will be created with its status set to **NORMAL_INDEX**.

*   *index_comment_string*: specifies a comment of an index.

..  note::

    *   From CUBRID 9.0, the index name should not be omitted.

    *   Prefix index feature is deprecated, so it is not recommended anymore.
    
    *   The session and server timezone (:ref:`timezone-parameters`) should not be changed if database contains indexes or function index on columns of type TIMESTAMP, TIMESTAMP WITH LOCAL TIME ZONE or DATETIME WITH LOCAL TIME ZONE.
    
    *   The leap second support parameter (:ref:`timezone-parameters`) should not be changed if database contains indexes or function index on columns of type TIMESTAMP or TIMESTAMP WITH LOCAL TIME ZONE. 
    
    *   The number of threads from PARALLEL option applies only to index loading step. The heap scan step is performed by the transaction thread (single thread). During this process, the rows are collected into batches; when a batch is full (reached 16M in size), it is dispatched to a index loading thread (or pushed in the queue), the collecting process continues. Batch size computing consists of index key size and the row identifier size (8 bytes).

    *   Under stand-alone mode, the WITH ONLINE option is ignored (normal index is created).

    *   The creation of online index is performed in three stages:
    
        * Adding index into schema with status INDEX IS IN ONLINE BUILDING. This is performed with a SCH_M_LOCK (like all schema changes) on table. After this, the lock is demoted to IX_LOCK.

        * Populating the index : scanning the heap file into batches, sorting the batches and adding the keys to index. During this step, other transactions can modify table data (index is also updated with data changes from other committed transactions).

        * Updating the index status as NORMAL INDEX; this is performed after promoting the table lock back to SCH_M_LOCK (promotion is guaranteed).
    
    *   The online index being built is displayed by SHOW statements from other transactions. It is not visible from other transactions in :ref:`-db-index` system table due to MVCC snapshot (other transactions can only see committed entries in this table).

    *   Transactions running in parallel with online index building which performs operations causing unique violations in index are allowed to commit. The online index will continue to progress and check before final step (setting NORMAL INDEX status in schema) the validity of unique constraint. The index creation will be aborted in case of unique violation. The user needs to restart the operation after making sure the unique constraint is ensured.

The following example shows how to create a descending index.

.. code-block:: sql

    CREATE INDEX gold_index ON participant(gold DESC);

The following example shows how to create a multiple column index.

.. code-block:: sql

    CREATE INDEX name_nation_idx ON athlete(name, nation_code) COMMENT 'index comment';

COMMENT of Index
----------------

You can write a comment of an index as following.

.. code-block:: sql

    CREATE TABLE tbl (a int default 0, b int, c int);

    CREATE INDEX i_tbl_b on tbl (b) COMMENT 'index comment for i_tbl_b';

    CREATE TABLE tbl2 (a INT, index i_tbl_a (a) COMMENT 'index comment', b INT);

    ALTER TABLE tbl2 ADD INDEX i_tbl2_b (b) COMMENT 'index comment b';

A specified comment of an index can be shown by running these statements.

.. code-block:: sql

    SHOW CREATE TABLE table_name;
    SELECT index_name, class_name, comment from db_index where class_name ='classname';
    SHOW INDEX FROM table_name;

Or you can see the index comments with ;sc command in the CSQL interpreter.

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc tbl

Online index creation
---------------------

You can create the index while still allowing other transactions to insert or update the table. 

.. code-block:: sql

    CREATE TABLE t1 (i1 int, i2 int);

    CREATE INDEX i_t1_i1 on t1 (i1) WITH ONLINE PARALLEL 10;


Displaying online index from other transactions
-----------------------------------------------

Other transactions may see the online index with schema related statements: 

.. code-block:: sql

       csql> show index in t1;
       
       === <Result of SELECT Command in Line 1> ===
       
         Table                  Non_unique  Key_name              Seq_in_index  Column_name           Collation             Cardinality     Sub_part  Packed                Null                  Index_type            Func                  Comment               Visible
       =================================================================================================================================================================================================================================================================================
         't1'                            1  'i_t1'                           1  'i1'                  'A'                             0         NULL  NULL                  'YES'                 'BTREE'               NULL                  NULL                  'NO'
       
       1 row selected. (0.020779 sec) Committed.
       
       1 command(s) successfully processed.
       csql> desc t1;
       
       === <Result of SELECT Command in Line 1> ===
       
         Field                 Type                  Null                  Key                   Default               Extra
       ====================================================================================================================================
         'i1'                  'INTEGER'             'YES'                 'MUL'                 NULL                  ''
         'i2'                  'INTEGER'             'YES'                 ''                    NULL                  ''

       csql> ;schema t1
       
       === <Help: Schema of a Class> ===
       
       
        <Class Name>
       
            t1
       
        <Attributes>
       
            i1                   INTEGER
            i2                   INTEGER
       
        <Constraints>
       
            INDEX i_t1 ON t1 (i1) IN PROGRESS



Online unique index while other transactions inserts violates uniqueness
------------------------------------------------------------------------

+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| session 1                                                         | session 2                                                                         |
+===================================================================+===================================================================================+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   CREATE TABLE t1 (i1 int, i2 int);                               |                                                                                   |
|                                                                   |                                                                                   |
|   COMMIT WORK;                                                    |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   INSERT INTO t1 VALUES (1, 10);                                  |                                                                                   |
|                                                                   |                                                                                   |
|   CREATE UNIQUE INDEX i_t1_i1 on t1 (i1) WITH ONLINE;             |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                   | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|                                                                   |    csql> ;schema t1                                                               |
|                                                                   |                                                                                   |
|                                                                   |    === <Help: Schema of a Class> ===                                              |
|                                                                   |                                                                                   |
|                                                                   |                                                                                   |
|                                                                   |     <Class Name>                                                                  |
|                                                                   |                                                                                   |
|                                                                   |         t1                                                                        |
|                                                                   |                                                                                   |
|                                                                   |     <Attributes>                                                                  |
|                                                                   |                                                                                   |
|                                                                   |         i1                   INTEGER                                              |
|                                                                   |         i2                   INTEGER                                              |
|                                                                   |                                                                                   |
|                                                                   |     <Constraints>                                                                 |
|                                                                   |                                                                                   |
|                                                                   |         UNIQUE i_t1 ON t1 (i1) IN PROGRESS                                        |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                   | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|                                                                   |  INSERT INTO t1 VALUES (1, 20);                                                   |
|                                                                   |                                                                                   |
|                                                                   |  COMMIT WORK;                                                                     |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|  COMMIT WORK;                                                     |                                                                                   |
|                                                                   |                                                                                   |
|   ERROR: Operation would have caused one or more unique constraint|                                                                                   |
|                                                                   |                                                                                   |
|   violations. INDEX i_t1(B+tree: 0|3456|3457) ON                  |                                                                                   |
|                                                                   |                                                                                   |
|   CLASS t1(CLASS_OID: 0|202|7). key: *UNKNOWN-KEY*.               |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+

.. _deduplicate_overview:

DEDUPLICATE 
-----------

If you use the **DEDUPLICATE** option, you can improve performance of modifying skewed index. The value of this option can be adjusted to mitigate the overflow page's linked list for a specific key value from being made too long, thereby improving insert/delete/update and vacuum performance. However, since an index column hidden by the system is added, the number of leaf nodes in the index and the height of the b-tree may increase, increasing the size of the index and affecting search performance. In particular, if the index data is uniformly distributed with respect to key values, you are careful to use this option because only the size of the index may increase without improving performance.

If the value of *deduplicate level* is specified as 1 or higher, a hidden index column used internally by the system is added when creating the index. The value of column is used to reduce redundancy so that it is not biased toward a specific key value.  A higher *deduplicate level* value, it will be mitigated redundancy and a smaller overflow page for a specific  key.

.. note::

    * The overflow page have a disadvantage in add, delete, and update performance, but are beneficial in search performance. Therefore, it is better to adjust the length of the overflow page for one key to be within tens or hundreds of pages, rather than setting the *deduplication level* high so that overflow pages are not created when creating an index with that option.


There are two ways to specify *deduplicate level* when creating an index.

Implicit method

    This method automatically specifies *deduplicate level* when there is no explicit DEDUPLICATE option specified in the SQL statement. This method is affected by the set value of the system parameter **deduplicate_key_level**.
    If **deduplicate_key_level** is greater than or equal to 1, *deduplicate level* is automatically set to **deduplicate_key_level**. 

Explicit method

    This is a method in which the user explicitly specifies the **DEDUPLICATE** option in the SQL statement. Regardless of the **deduplicate_key_level** set value, the *deduplicate level* specified by the user is applied.
    As in the example below, specify the DEDUPLICATE statement directly.
    
    .. code-block:: sql
    
        CREATE TABLE tbl (a int default 0, b int, c int);
        CREATE INDEX i_tbl_b on tbl (b) WITH DEDUPLICATE=3 COMMENT 'for deduplicate level 3';
        CREATE INDEX i_tbl_b_c on tbl (b,c) WITH DEDUPLICATE=7 COMMENT 'for deduplicate level 1';

.. warning::

    * If **deduplicate_key_level** is **\-1**\, it is internally ignored and not applied even if it is specified explicitly. That is, in this case, all indexes are created with *deduplicate level* as **0**.
    * A column name that begins with "**_dedup_**" cannot be created.

.. note::

    * When creating an index, if the composition of the key field is guaranteed to be UNIQUE, the DEDUPLICATE specified by the user is ignored and *deduplicate level* is created as **0**.
        * If the columns of index includes all columns of Primary Key or Unique Index.
        * However, except if columns of Primary key or Unique index used as an argument of a function index.

Allowing multiple indexes on the same column in the same order
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
    Multiple indexes can be defined with all key fields and filter conditions identical except for *deduplicate level*\.

    .. code-block:: sql
    
        CREATE TABLE tbl (a int primary key, b int, c int);
        CREATE INDEX idx1 ON tbl(b, c) WITH DEDUPLICATE=3;
        CREATE INDEX idx2 ON tbl(b, c) WITH DEDUPLICATE=5;
        CREATE UNIQUE INDEX idx_uk ON tbl(b); 
        CREATE INDEX idx3 ON tbl(b, c) WITH DEDUPLICATE=7;

    In the example above, idx1 and idx2 have the specified *deduplicate level*. However, since idx3 is guaranteed by idx_uk that column b is unique, it is created *duplicate level* as **0** and ignore user's specified option.
        
.. note::

    * If an FK already exists with the same index column that differs only in *deduplication level*, it can't be duplicated.
    * If columns of index includes a primary key or unique index, indexes that differ only in the deduplicate level can't be created. (Even if duplicate indexes are allowed.)
    * You cannot change the *deduplicate level* of an index with the ALTER INDEX REBUILD statement. If necessary, delete the index and recreate it.


.. _deduplicate_overflow_page:

OVERFLOW PAGE
~~~~~~~~~~~~~~~

    * What is the overflow page?
       An index consists of non-leaf nodes and leaf nodes, and a leaf node is composed of a set of index key information.
       At this time, one index key information is a pair of a key value and a set of record's OIDs corresponding to the key value.
       If there are many records with a specific key value, the OIDs of all the records can't be stored in the leaf node, so they are separated and managed in a separate page, which is called an overflow page.
       Also, when even overflow pages are full, new overflow pages are created and these pages are maintained as a linked list.

.. _alter-index:

ALTER INDEX
===========

The **ALTER INDEX** statement changes the properties of an index. Index is rebuilt unless only comment or status is changed. Rebuilding an index is a job which drops and recreates an index.

The following is a syntax of rebuilding an index.

::

    ALTER INDEX index_name ON [schema_name.]table_name {{[COMMENT index_comment_string] REBUILD} | {COMMENT index_comment_string} | VISIBLE | INVISIBLE};

*   *index_name*: specifies the name of the index to be altered or rebuilt. The index name must be unique in the table.
*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name*: specifies the name of the table where the index is rebuilt.
*   **REBUILD**:  rebuild an index with the same structure as the one already created.
*   *index_comment_string*: specifies a comment of an index.

.. note::

    *   From CUBRID 9.0, the index name should not be omitted.

    *   From CUBRID 10.0, table name should not be omitted.
    
    *   From CUBRID 10.0, even if you add column names at the end of a table name, these will be ignored and recreated with the same columns with the previous index.

    *   From CUBRID 11.4, an error occurs if you add a column name after the table name.
    

The following is an example of recreating index.

.. code-block:: sql

    CREATE INDEX i_game_medal ON game(medal);
    ALTER INDEX i_game_medal ON game COMMENT 'rebuild index comment' REBUILD ;

If you want to add or change a comment of the index without rebuilding an index, add a **COMMENT** clause and remove **REBUILD** keyword as follows:

.. code-block:: sql

    ALTER INDEX index_name ON table_name COMMENT 'index_comment_string' ;

The below is a syntax to only add or change a comment without rebuilding an index.

.. code-block:: sql
    
    ALTER INDEX i_game_medal ON game COMMENT 'change index comment' ;

The following is a syntax to change the status of an index to **INVISIBLE**/**VISIBLE**. When an index is set as **INVISIBLE**, queries will be executed as like the index does not exist. In this way, the performance of the index may be tested and the impact of its removal be evaluated without actually dropping the index.

.. code-block:: sql
    
    CREATE INDEX i_game_medal ON game(medal);
    ALTER INDEX i_game_medal ON game VISIBLE;
    ALTER INDEX i_game_medal ON game INVISIBLE;


DROP INDEX
==========

Use the **DROP INDEX** statement to drop an index. An index also can be dropped with **DROP CONSTRAINT** clause.

::

    DROP INDEX index_name ON [schema_name.]table_name ;

*   *index_name*: specifies the name of the index to be dropped.
*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name*: specifies the name of the table whose index is dropped.

.. warning::

    From the CUBRID 10.0 version, table name cannot be omitted.

The following is an example of dropping an index:

.. code-block:: sql

    DROP INDEX i_game_medal ON game;
