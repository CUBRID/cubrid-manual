*****
Index
*****

CREATE INDEX
============

Creates an index to a specified table by using the **CREATE INDEX** statement. For how to write index name, :doc:`/sql/identifier`.

For how to use indexes on the **SELECT** statement like Using SQL Hint, Descending Index, Covering Index, Index Skip Scan, **ORDER BY** Optimization and **GROUP BY** Optimization, and how to create Filtered Index and Function-based Index, see :doc:`/sql/tuning`.

::

    CREATE [ UNIQUE ] INDEX index_name
    ON table_name <index_col_desc>
     
    <index_col_desc> ::=
        ( column_name[(prefix_length)] [ASC | DESC] [ {, column_name[(prefix_length)] [ASC | DESC]} ...] ) [ WHERE <filter_predicate> ]
        | (function_name (argument_list) )

*   **UNIQUE**: Creates an index with unique values.
*   *index_name*: Specifies the name of the index to be created. The index name must be unique in the table.
*   *table_name*: Specifies the name of the table where the index is to be created.
*   *column_name*: Specifies the name of the column where the index is to be applied. To create a composite index, specify two or more column names.
*   *prefix_length*: When you specify an index for character- or bit string-type column, you can create an index by specifying the beginning part of the column name as a prefix. You can specify the length of the prefix as the number of characters in parentheses next to the column name. You cannot specify *prefix_length* in a multiple column index or a **UNIQUE** index. It is impossible to create an index by specifying *prefix_length* as a host variable. If you want to guarantee the query result order in the index in which *prefix_length* is specified, you must specify the **ORDER BY** clause.
*   **ASC** | **DESC**: Specifies the sorting order of columns. 
*   <*filter_predicate*>: Defines the conditions to create filtered indexes. When there are several comparison conditions between a column and a constant, filtering is available only when the conditions are connected by using **AND**.
*   *function_name* (*argument_list*): Defines the conditions to create function-based indexes.

.. warning:: In versions lower than CUBRID 9.0, index name can be omitted; however, from the CUBRID 9.0 version, this should not be omitted.

The following example shows how to create a descending index.

.. code-block:: sql

    CREATE INDEX gold_index ON participant(gold DESC);

The following example shows how to create a multiple column index.

.. code-block:: sql

    CREATE INDEX name_nation_idx ON athlete(name, nation_code);

The following example shows how to create a prefix index. In this example, 1-byte long prefix is specified for the *nation_code* column when creating an index.

.. code-block:: sql

    CREATE INDEX idx_game_nation_code ON game(nation_code(1));

.. _alter-index:

ALTER INDEX
===========

The **ALTER INDEX** statement rebuilds an index or renames an index name. Rebuilding an index is a job which drops and recreates an index. If column names are added at the end of a table name, a new index is recreated with the table and column names. 

The following is a syntax to rebuild an index.

::

    ALTER [ UNIQUE ] INDEX index_name
    ON table_name [<index_col_desc>] REBUILD
    
    <index_col_desc> ::=
        ( column_name[(prefix_length)] [ASC | DESC] [ {, column_name[(prefix_length)] [ASC | DESC]} ...] ) [ WHERE <filter_predicate> ]
        | (function_name (argument_list) )
     
*   **UNIQUE**: Specifies that an index to be recreated is an unique index.
*   *index_name*: Specifies the name of the index to be recreated. The index name must be unique in the table.
*   *table_name*: Specifies the name of the table where the index is recreated.
*   *column_name*: Specifies the name of the column where the index is to be applied. To create a composite index, specify two or more column names.
*   *prefix_length*: When you specify an index for character- or bit string-type column, you can create an index by specifying the beginning part of the column name as a prefix. You can specify the length of the prefix as the number of characters in parentheses next to the column name. You cannot specify *prefix_length* in a multiple column index or a **UNIQUE** index. It is impossible to create an index by specifying *prefix_length* as a host variable. If you want to guarantee the query result order in the index in which *prefix_length* is specified, you must specify the **ORDER BY** clause.
*   **ASC** | **DESC**: Specifies the sorting order of columns. 
*   <*filter_predicate*>: Defines the conditions to create filtered indexes. When there are several comparison conditions between a column and a constant, filtering is available only when the conditions are connected by using **AND**.
*   *function_name* (*argument_list*): Defines the conditions to create function-based indexes.

.. warning:: From the CUBRID 10.0 version, table name should not be omitted.

The following is an example of re-creating indexes in various ways:

.. code-block:: sql

    CREATE INDEX i_game_medal ON game(medal);
    ALTER INDEX i_game_medal ON game REBUILD;
    ALTER INDEX i_game_medal ON game(nation_code) REBUILD;
    ALTER INDEX char_idx ON athlete(gender, nation_code) WHERE gender='M' AND nation_code='USA' REBUILD;


DROP INDEX
==========

Use the **DROP INDEX** statement to drop an index. ::

    DROP [ UNIQUE ] INDEX index_name
    ON table_name

*   **UNIQUE**: Specifies that the index to be dropped is a unique index. This also can be dropped with **DROP CONSTRAINT** clause.
*   *index_name*: Specifies the name of the index to be dropped.
*   *table_name*: Specifies the name of the table whose index is dropped.

.. warning:: From the CUBRID 10.0 version, table name should not be omitted.

The following is an example of dropping an index:

.. code-block:: sql

    DROP INDEX i_game_medal ON game;
