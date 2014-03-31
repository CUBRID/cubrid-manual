*****
Index
*****

CREATE INDEX
============

Creates an index to a specified table by using the **CREATE INDEX** statement. For how to write index name, :doc:`/sql/identifier`.

For how to use indexes on the **SELECT** statement like Using SQL Hint, Descending Index, Covering Index, Index Skip Scan, **ORDER BY** Optimization and **GROUP BY** Optimization, and how to create Filtered Index and Function-based Index, see :doc:`/sql/tuning`.

::

    CREATE [ UNIQUE ] INDEX index_name ON table_name <index_col_desc> ;
     
        <index_col_desc> ::=
            ( column_name [ASC | DESC] [ {, column_name [ASC | DESC]} ...] ) [ WHERE <filter_predicate> ]
            | (function_name (argument_list) )

*   **UNIQUE**: Creates an index with unique values.
*   *index_name*: Specifies the name of the index to be created. The index name must be unique in the table.
*   *table_name*: Specifies the name of the table where the index is to be created.
*   *column_name*: Specifies the name of the column where the index is to be applied. To create a composite index, specify two or more column names.
*   **ASC** | **DESC**: Specifies the sorting order of columns. 

*   <*filter_predicate*>: Defines the conditions to create filtered indexes. When there are several comparison conditions between a column and a constant, filtering is available only when the conditions are connected by using **AND**. Regarding this, definitely watch :ref:`filtered-index`.

*   *function_name* (*argument_list*): Defines the conditions to create function-based indexes. Regarding this, definitely watch :ref:`function-index`.

..  note::

    *   From CUBRID 9.0, the index name should not be omitted.

    *   prefix index feature is deprecated, so it is not recommended anymore.

The following example shows how to create a descending index.

.. code-block:: sql

    CREATE INDEX gold_index ON participant(gold DESC);

The following example shows how to create a multiple column index.

.. code-block:: sql

    CREATE INDEX name_nation_idx ON athlete(name, nation_code);

.. _alter-index:

ALTER INDEX
===========

The **ALTER INDEX** statement rebuilds an index or renames an index name. Rebuilding an index is a job which drops and recreates an index.

The following is a syntax of rebuilding an index.

::

    ALTER INDEX index_name ON table_name REBUILD ;
     
*   *index_name*: Specifies the name of the index to be recreated. The index name must be unique in the table.
*   *table_name*: Specifies the name of the table where the index is recreated.
*   **REBUILD**:  Recreate an index with the same structure as the one already created.

.. warning:: 

    From CUBRID 10.0, table name should not be omitted.

.. note:: 

    From CUBRID 10.0, even if you add column names at the end of a table name, these will be ignored and recreated with the same columns with the previous index.

.. note::

    *   From CUBRID 9.0, the index name should not be omitted.

    *   From CUBRID 10.0, table name should not be omitted.
    
    *   From CUBRID 10.0, even if you add column names at the end of a table name, these will be ignored and recreated with the same columns with the previous index.

    *   prefix index feature is deprecated, so it is not recommended anymore.
    
The following is an example of recreating indexes in various ways:

.. code-block:: sql

    CREATE INDEX i_game_medal ON game(medal);
    ALTER INDEX i_game_medal ON game REBUILD;

The following is a syntax of renaming an index.

:: 

    ALTER INDEX old_index_name ON table_name RENAME TO new_index_name ;
     
An index name can be changed by not only ALTER INDEX statement, but also :ref:`rename-index`.

The following is an example of changing an index name:

.. code-block:: sql 

    ALTER INDEX i_game_medal ON game RENAME TO i_new_game_medal; 

DROP INDEX
==========

Use the **DROP INDEX** statement to drop an index. An index also can be dropped with **DROP CONSTRAINT** clause.

::

    DROP INDEX index_name ON table_name ;

*   *index_name*: Specifies the name of the index to be dropped.
*   *table_name*: Specifies the name of the table whose index is dropped.

.. warning:: 

    From the CUBRID 10.0 version, table name cannot be omitted.

The following is an example of dropping an index:

.. code-block:: sql

    DROP INDEX i_game_medal ON game;
