******
DELETE
******

You can delete records in the table by using the **DELETE** statement. You can specify delete conditions by combining the statement with the :ref:`where-clause`. You can delete one or more tables with one **DELETE** statement.

::
 
    <DELETE single table>
    DELETE [FROM] table_name [ WHERE <search_condition> ] [LIMIT row_count]
     
    <DELETE multiple tables FROM ...>
    DELETE table_name[, table_name] ... FROM <table_specifications> [ WHERE <search_condition> ]
     
    <DELETE FROM multiple tables USING ...>
    DELETE FROM table_name[, table_name] ... USING <table_specifications> [ WHERE <search_condition> ]

*   <*table_specifications*>: You can specify the statement such as **FROM** clause of the **SELECT** statement and one or more tables can be specified.

*   *table_name*: Specifies the name of a table where the data to be deleted is contained. If the number of table is one, the **FROM** keyword can be omitted.

*   *search_condition*: Deletes only data that meets *search_condition* by using :ref:`where-clause`. If it is specified, all data in the specified tables will be deleted.

*   *row_count*: Specifies the number of records to be deleted in the :ref:`limit-clause`. An integer greater than 0 can be given.

When a table to delete records is only one, :ref:`limit-clause` can be specified. You can limit the number of records by specifying the :ref:`limit-clause`.  If the number of records satisfying the :ref:`where-clause` exceeds *row_count*, only the number of records specified in *row_count* will be deleted.

.. note:: \

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