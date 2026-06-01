
:meta-keywords: truncate statement
:meta-description: You can delete all records in the specified table by using the TRUNCATE statement.

********
TRUNCATE
********

You can delete all records in the specified table by using the **TRUNCATE** statement.

It has the following advantages over using the **DELETE FROM** *[schema_name.]table_name* statement without a **WHERE** clause.

* It's way faster due to deleting all indexes and constraints in advance and deleting records at once.
* There is no vacuum cost.
* It generates way lesser log records so that it is better in terms of HA replication, Recovery, and Rollback.

.. note:: 

    * The **DELETE** trigger is disabled while deleting records with the **TRUNCATE** statement.
    * The **TRUNCATE** statement initializes the **AUTO INCREMENT** column of the table. Therefore, if data is inserted, the **AUTO INCREMENT** column value increases from the initial value. 
    * To execute the **TRUNCATE** statement, the authorization of **ALTER**, **INDEX**, and **DELETE** is required on the table. For granting authorization, see :ref:`granting-authorization`.
    * If a table is created with **DONT_REUSE_OID** option (:ref:`dont-reuse-oid`), other tables can refer to it as a column. When the specified table is referred to like this, it is impossible to truncate the table and it's done with **DELETE FROM** statement internally.

::

    TRUNCATE [ TABLE ] [schema_name.]table_name [ CASCADE ]

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of the table that contains the data to be deleted.
*   **CASCADE** : Deletes all records in all tables referring to the specified table with a foreign key. This is propagated to all tables in the foreign key relationship. A **PRIMARY KEY** constraint has to be defined in the table and this has to be referred to by one or more **FOREIGN KEY**, and the foreign key action has to be **ON DELETE**. It fails without this option when a foreign key referring to the specified table is defined. It also fails when even one **ON DELETE** action is not **CASCADE** in all foreign key relationships. See :ref:`foreign-key-constraint` for more information about the foreign key constraint. 

.. code-block:: sql

    CREATE TABLE a_tbl(A INT AUTO_INCREMENT(3,10) PRIMARY KEY);
    INSERT INTO a_tbl VALUES (NULL),(NULL),(NULL);
    SELECT * FROM a_tbl;
    
::

                a
    =============
                3
                13
                23

.. code-block:: sql

    --AUTO_INCREMENT column value increases from the initial value after truncating the table
    TRUNCATE TABLE a_tbl;
    INSERT INTO a_tbl VALUES (NULL);
    SELECT * FROM a_tbl;
    
::

                a
    =============
                3

The following example uses the **CASCADE** option. Note that even a record refers to nothing, but the propagated **TRUNCATE** deletes all records.                 

.. code-block:: sql
    
    -- a_tbl <- b_tbl <- c_tbl
    --       <- d_tbl
    CREATE TABLE a_tbl(a1 INT PRIMARY KEY);
    CREATE TABLE b_tbl(b1 INT PRIMARY KEY, b2 INT FOREIGN KEY REFERENCES a_tbl (a1) ON DELETE CASCADE);
    CREATE TABLE c_tbl(c1 INT PRIMARY KEY, c2 INT FOREIGN KEY REFERENCES b_tbl (b1) ON DELETE CASCADE);
    CREATE TABLE d_tbl(d1 INT PRIMARY KEY, d2 INT FOREIGN KEY REFERENCES a_tbl (a1) ON DELETE CASCADE);
    INSERT INTO a_tbl VALUES (1);
    INSERT INTO b_tbl VALUES (2, 1);
    INSERT INTO c_tbl VALUES (3, 2);
    INSERT INTO d_tbl VALUES (5, 1);
    INSERT INTO d_tbl VALUES (4, NULL); -- not refer to any record in a_tbl, but will be deleted.
    SELECT * FROM a_tbl;
    SELECT * FROM b_tbl;
    SELECT * FROM c_tbl;
    SELECT * FROM d_tbl;

    TRUNCATE a_tbl CASCADE;
    SELECT * FROM a_tbl, b_tbl, c_tbl, d_tbl;

::

               a1
    =============
                1

               b1           b2
    ==========================
                2            1

               c1           c2
    ==========================
                3            2

               d1           d2
    ==========================
                5            1
                4         NULL
    
    -- after TRUNCATE a_tbl CASCADE;
    There are no results.
