****************
Index Definition
****************

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

*   **UNIQUE** : Creates an index with unique values.
*   *index_name* : Specifies the name of the index to be created. The index name must be unique in the table.
*   *prefix_length* : When you specify an index for character- or bit string-type column, you can create an index by specifying the beginning part of the column name as a prefix. You can specify the length of the prefix as the number of characters in parentheses next to the column name. You cannot specify *prefix_length* in a multiple column index or a **UNIQUE** index. It is impossible to create an index by specifying *prefix_length* as a host variable. If you want to guarantee the query result order in the index in which *prefix_length* is specified, you must specify the **ORDER BY** clause.

*   *table_name* : Specifies the name of the table where the index is to be created.
*   *column_name* : Specifies the name of the column where the index is to be applied. To create a composite index, specify two or more column names.
*   **ASC** | **DESC** : Specifies the sorting order of columns. 

*   <*filter_predicate*>: Defines the conditions to create filtered indexes. When there are several comparison conditions between a column and a constant, filtering is available only when the conditions are connected by using **AND**.

*   *function_name* (*argument_list*): Defines the conditions to create function-based indexes.

.. warning:: In versions lower than CUBRID 9.0, index names can be deleted; however, from the CUBRID 9.0 version, it is no longer supported.

The following example shows how to create a descending index.

.. code-block:: sql

    CREATE INDEX gold_index ON participant(gold DESC);

The following example shows how to create a multiple column index.

.. code-block:: sql

    CREATE INDEX name_nation_idx ON athlete(name, nation_code);

The following example shows how to create a prefix index. In this example, 1-byte long prefix is specified for the *nation_code* column when creating an index.

.. code-block:: sql

    CREATE INDEX idx_game_nation_code ON game(nation_code(1));

ALTER INDEX
===========

The **ALTER INDEX** statement rebuilds an index. In other words, it drops and rebuilds an index. If a table name and a column name are added at the end of the **ON** clause, a new index is re-created with the table and column names. ::

    ALTER [ UNIQUE ] INDEX index_name
    ON { ONLY } table_name <index_col_desc> REBUILD [ ; ]
     
    <index_col_desc> ::=
        ( column_name[ {, column_name} ...] ) [ WHERE <filter_predicate> ]
        | (function_name (argument_list) )

*   **UNIQUE** : Creates an index with unique values.
*   *index_name* : Specifies the name of the index to be recreated. The index name must be unique in the table.
*   *table_name* : Specifies the name of the table where the index is recreated.
*   *column_name* : Specifies the name of the column where the index is applied. To create a multiple column index, specify two or more column names.

*   <*filter_predicate*>: Defines the conditions to create filtered indexes. When there are several comparison conditions between a column and a constant, filtering is available only when the conditions are connected by using **AND**.

*   *function_name* (*argument_list*): Defines the conditions to create function-based indexes.

.. warning:: In versions lower than CUBRID 9.0, index names can be deleted; however, from the CUBRID 9.0 version, it is no longer supported.

The following is an example of re-creating indexes in various ways:

.. code-block:: sql

    ALTER INDEX i_game_medal ON game(medal) REBUILD;
    ALTER INDEX game_date_idx REBUILD;
    ALTER INDEX char_idx ON athlete(gender, nation_code) WHERE gender='M' AND nation_code='USA' REBUILD;

DROP INDEX
==========

Use the **DROP INDEX** statement to drop an index. ::

    DROP [ UNIQUE ] INDEX index_name
    [ON table_name] [ ; ]

*   **UNIQUE** : Specifies that the index to be dropped is a unique index. This also can be dropped with **DROP CONSTRAINT** clause.
*   *index_name* : Specifies the name of the index to be dropped. If omitted, a name is automatically assigned as *i_<table_name>_<column_names>*.
*   *table_name* : Specifies the name of the table whose index is dropped.

The following is an example of dropping an index:

.. code-block:: sql

    DROP INDEX game_date_idx ON game;
