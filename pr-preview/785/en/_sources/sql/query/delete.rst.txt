
:meta-keywords: delete statement
:meta-description: You can delete records in the table by using the DELETE statement.

******
DELETE
******

You can delete records in the table by using the **DELETE** statement. You can specify delete conditions by combining the statement with the :ref:`where-clause`. You can delete one or more tables with one **DELETE** statement.

::
 
    <DELETE single table>
    DELETE [FROM] [schema_name.]table_name[@[schema_name.]server_name] [<correlation>] WHERE <search_condition> ] [LIMIT row_count]
     
    <DELETE multiple tables FROM ...>
    DELETE [schema_name.]table_name[@[schema_name.]server_name] | <correlation> [{, [schema_name.]table_name[@[schema_name.]server_name] | <correlation>}]
      FROM <table_specifications> [ WHERE <search_condition> ]
     
    <DELETE FROM multiple tables USING ...>
    DELETE FROM [schema_name.]table_name[@[schema_name.]server_name] | <correlation> [{, [schema_name.]table_name[@[schema_name.]server_name] | <correlation>}]
      USING <table_specifications> [ WHERE <search_condition> ]

*   <*table_specifications*>: You can specify the same syntax as the **FROM** clause of the **SELECT** statement, and you can specify one or more tables. Starting with version 11.3, you can specify not only local tables but also remote tables.

*   *server_name*: Used when specifying a table of a remote server connected by dblink, not the current server.

*   *correlation*: alias of the specified local table or remote table

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.

*   *table_name*: Specifies the name of a table where the data to be deleted is contained. If the number of table is one, the **FROM** keyword can be omitted.

*   *search_condition*: Deletes only data that meets *search_condition* by using :ref:`where-clause`. If it is specified, all data in the specified tables will be deleted.

*   *row_count*: Specifies the number of records to be deleted in the :ref:`limit-clause`. It can be one of unsigned integer, a host variable or a simple expression.

When a table to delete records is only one, :ref:`limit-clause` can be specified. You can limit the number of records by specifying the :ref:`limit-clause`.  If the number of records satisfying the :ref:`where-clause` exceeds *row_count*, only the number of records specified in *row_count* will be deleted.

.. note::

    *   On the **DELETE** statement with multiple tables, the table alias can be defined within <*table_specifications*> only. At the outside of <*table_specifications*>, the table alias defined in <*table_specifications*> can be used.

    *   Previous versions of CUBRID 9.0 allow only one table for <*table_specifications*>.

.. code-block:: sql

    CREATE TABLE a_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));
    INSERT INTO a_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL), (5, NULL);
     
    --delete one record only from a_tbl
    DELETE FROM a_tbl WHERE phone IS NULL LIMIT 1;
    SELECT * FROM a_tbl;
    
::
    
               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                5  NULL
     
.. code-block:: sql

    --delete all records from a_tbl
    DELETE FROM a_tbl;

Below tables are created to explain **DELETE JOIN**.

.. code-block:: sql

    CREATE TABLE a_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));
    CREATE TABLE b_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));
    CREATE TABLE c_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));
     
    INSERT INTO a_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL), (5, NULL);
    INSERT INTO b_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL);
    INSERT INTO c_tbl VALUES(1,'111-1111'), (2,'222-2222'), (10, '333-3333'), (11, NULL), (12, NULL);

The below queries delete rows after joining multiple tables. They show the same result.

.. code-block:: sql

    -- Below four queries show the same result.
    --  <DELETE multiple tables FROM ...>
     
    DELETE a, b FROM a_tbl a, b_tbl b, c_tbl c
    WHERE a.id=b.id AND b.id=c.id;
     
    DELETE a, b FROM a_tbl a INNER JOIN b_tbl b ON a.id=b.id
    INNER JOIN c_tbl c ON b.id=c.id;
     
    -- <DELETE FROM multiple tables USING ...>
     
    DELETE FROM a, b USING a_tbl a, b_tbl b, c_tbl c
    WHERE a.id=b.id AND b.id=c.id;
     
    DELETE FROM a, b USING a_tbl a INNER JOIN b_tbl b ON a.id=b.id
    INNER JOIN c_tbl c ON b.id=c.id;

For more details on join syntax, see :ref:`join-query`.

You can also delete remote table data using the extended table name. The query below is for deleting remote table data.

.. code-block:: sql

    --at remote srv1
    CREATE TABLE a_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));
    INSERT INTO a_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL), (5, NULL);

    --at local
    --delete one record only from remote a_tbl
    DELETE FROM a_tbl@srv1 WHERE phone IS NULL LIMIT 1;
    SELECT * FROM a_tbl@srv1;
               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                5  NULL
    --delete all records from remote a_tbl
    DELETE FROM a_tbl@srv1;

The tables below were created to explain remote DELETE JOIN.

.. code-block:: sql

    --at remote srv1
    CREATE TABLE a_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));

    CREATE TABLE b_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));

    CREATE TABLE c_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));

    --at local
    INSERT INTO a_tbl@srv1 VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL), (5, NULL);
    INSERT INTO b_tbl@srv1 VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL);
    INSERT INTO c_tbl@srv1 VALUES(1,'111-1111'), (2,'222-2222'), (10, '333-3333'), (11, NULL), (12, NULL);

The following queries perform deletion after joining tables including remote tables (2 local tables, 1 remote table), and all show the same result.

.. code-block:: sql

    -- Below four queries show the same result.
    --  <DELETE multiple tables FROM ...>

    DELETE a, b FROM a_tbl@srv1 a, b_tbl@srv1 b, c_tbl@srv1 c
    WHERE a.id=b.id AND b.id=c.id;

    DELETE a, b FROM a_tbl@srv1 a INNER JOIN b_tbl@srv1 b ON a.id=b.id
    INNER JOIN c_tbl@srv1 c ON b.id=c.id;

    -- <DELETE FROM multiple tables USING ...>

    DELETE FROM a, b USING a_tbl@srv1 a, b_tbl@srv1 b, c_tbl@srv1 c
    WHERE a.id=b.id AND b.id=c.id;

    DELETE FROM a, b USING a_tbl@srv1 a INNER JOIN b_tbl@srv1 b ON a.id=b.id
    INNER JOIN c_tbl@srv1 c ON b.id=c.id;

Caution
==========

As shown below, DELETE JOIN queries that include local tables and remote tables and delete remote tables are not allowed. This is because it does not send local data to the remote.

.. code-block:: sql

    DELETE c FROM a_tbl a, b_tbl b, c_tbl@srv1 c
    WHERE a.id=b.id AND b.id=c.id;

    DELETE a, b, c FROM a_tbl a, b_tbl b, c_tbl@srv1 c
    WHERE a.id=b.id AND b.id=c.id;
