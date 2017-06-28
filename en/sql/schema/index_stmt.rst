
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

    CREATE [UNIQUE] INDEX index_name ON table_name <index_col_desc> ;
     
        <index_col_desc> ::=
            { ( column_name [ASC | DESC] [ {, column_name [ASC | DESC]} ...] ) [ WHERE <filter_predicate> ] | 
            (function_name (argument_list) ) } 
                [COMMENT 'index_comment_string']

*   **UNIQUE**: creates an index with unique values.
*   *index_name*: specifies the name of the index to be created. The index name must be unique in the table.

*   *table_name*: specifies the name of the table where the index is to be created.
*   *column_name*: specifies the name of the column where the index is to be applied. To create a composite index, specify two or more column names.
*   **ASC** | **DESC**: specifies the sorting order of columns. 

*   <*filter_predicate*>: defines the conditions to create filtered indexes. When there are several comparison conditions between a column and a constant, filtering is available only when the conditions are connected by using **AND**. Refer to :ref:`filtered-index` for more details.
*   *function_name* (*argument_list*): Defines the conditions to create function-based indexes. Regarding this, definitely watch :ref:`function-index`.

*   *index_comment_string*: specifies a comment of an index.

..  note::

    *   From CUBRID 9.0, the index name should not be omitted.

    *   Prefix index feature is deprecated, so it is not recommended anymore.
    
    *   The session and server timezone (:ref:`timezone-parameters`) should not be changed if database contains indexes or function index on columns of type TIMESTAMP, TIMESTAMP WITH LOCAL TIME ZONE or DATETIME WITH LOCAL TIME ZONE.
    
    *   The leap second support parameter (:ref:`timezone-parameters`) should not be changed if database contains indexes or function index on columns of type TIMESTAMP or TIMESTAMP WITH LOCAL TIME ZONE. 

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

.. _alter-index:

ALTER INDEX
===========

The **ALTER INDEX** statement rebuilds an index or adds/changes the comment of an index. Rebuilding an index is a job which drops and recreates an index.

The following is a syntax of rebuilding an index.

::

    ALTER INDEX index_name ON table_name REBUILD;

*   *index_name*: specifies the name of the index to be recreated. The index name must be unique in the table.
*   *table_name*: specifies the name of the table where the index is recreated.
*   **REBUILD**:  recreate an index with the same structure as the one already created.
*   *index_comment_string*: specifies a comment of an index.

.. note::

    *   From CUBRID 9.0, the index name should not be omitted.

    *   From CUBRID 10.0, table name should not be omitted.
    
    *   From CUBRID 10.0, even if you add column names at the end of a table name, these will be ignored and recreated with the same columns with the previous index.

    *   Prefix index feature is deprecated, so it is not recommended anymore.

The following is an example of recreating index.

.. code-block:: sql

    CREATE INDEX i_game_medal ON game(medal);
    ALTER INDEX i_game_medal ON game COMMENT 'rebuild index comment' REBUILD ;

If you want to add or change a comment of the index without rebuilding an index, add a **COMMENT** clause and remove **REBUILD** keyword as follows:

::

    ALTER INDEX index_name ON table_name COMMENT 'index_comment_string' ;

The below is a syntax to only add or change a comment without rebuilding an index.

.. code-block:: sql
    
    ALTER INDEX i_game_medal ON game COMMENT 'change index comment' ;

The following is a syntax of renaming an index.

:: 

    ALTER INDEX old_index_name ON table_name RENAME TO new_index_name [COMMENT 'index_comment_string'] ;


DROP INDEX
==========

Use the **DROP INDEX** statement to drop an index. An index also can be dropped with **DROP CONSTRAINT** clause.

::

    DROP INDEX index_name ON table_name ;

*   *index_name*: specifies the name of the index to be dropped.
*   *table_name*: specifies the name of the table whose index is dropped.

.. warning::

    From the CUBRID 10.0 version, table name cannot be omitted.

The following is an example of dropping an index:

.. code-block:: sql

    DROP INDEX i_game_medal ON game;
