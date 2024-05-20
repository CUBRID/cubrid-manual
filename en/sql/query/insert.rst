
:meta-keywords: insert statement, insert set, insert select, on duplicate key update, odku
:meta-description: You can insert a new record into a table in a database by using the INSERT statement. CUBRID supports INSERT ... VALUES, INSERT ... SET and INSERT ... SELECT statements.

******
INSERT
******

You can insert a new record into a table in a database by using the **INSERT** statement. CUBRID supports **INSERT ... VALUES**, **INSERT ... SET** and **INSERT ... SELECT** statements.

**INSERT ... VALUES** and **INSERT ... SET** statements are used to insert a new record based on the value that is explicitly specified while the **INSERT ... SELECT** statement is used to insert query result records obtained from different tables. Use the **INSERT VALUES** or **INSERT ... SELECT** statement to insert multiple rows by using the single **INSERT** statement.

::

    <INSERT ... VALUES statement>
    INSERT [INTO] <table_specification> [(column_name, ...)]
        {VALUES | VALUE}({expr | DEFAULT}, ...)[,({expr | DEFAULT}, ...),...]
        [ON DUPLICATE KEY UPDATE column_name = expr, ... ]
    INSERT [INTO] <table-specification> DEFAULT [ VALUES ]

    <INSERT ... SET statement>
    INSERT [INTO] <table_specification>
        SET column_name = {expr | DEFAULT}[, column_name = {expr | DEFAULT},...]
        [ON DUPLICATE KEY UPDATE column_name = expr, ... ]

    <INSERT ... SELECT statement>
    INSERT [INTO] <table_specification> [(column_name, ...)]
        SELECT...
        [ON DUPLICATE KEY UPDATE column_name = expr, ... ]

    <table_specification> ::= [schema_name.]table_name | <remote_table_spec>
    <remote_table_spec> ::= [schema_name.]table_name@[shema_name.]server_name

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.

*   *table_name*: Specifies the name of the target table into which you want to insert a new record.

*   *server_name*: Specifies the name of the remote server to connect to with dblink.

*   *column_name*: Specifies the name of the column into which you want to insert the value. If you omit to specify the column name, it is considered that all columns defined in the table have been specified. Therefore, you must specify the values for all columns next to the **VALUES** keyword. If you do not specify all the columns defined in the table, a **DEFAULT** value is assigned to the non-specified columns; if the **DEFAULT** value is not defined, a **NULL** value is assigned.

*   *expr* | **DEFAULT**: Specifies values that correspond to the columns next to the **VALUES** keyword. Expressions or the **DEFAULT** keyword can be specified as a value. At this time, the order and number of the specified column list must correspond to the column value list. The column value list for a single record is described in parentheses.

*   **DEFAULT**: You can use the **DEFAULT** keyword to specify a default value as the column value. If you specify **DEFAULT** in the column value list next to the **VALUES** keyword, a default value column is stored for the given column: if you specify **DEFAULT** before the **VALUES** keyword, default values are stored for all columns in the table. **NULL** is stored for the column whose default value has not been defined.

*   **ON DUPLICATE KEY UPDATE**: In case constraints are violated because a duplicated value for a column where **PRIMARY KEY** or **UNIQUE** attribute is defined is inserted, the value that makes constraints violated is changed into a specific value by performing the action specified in the **ON DUPLICATE KEY UPDATE** statement.

.. code-block:: sql

    CREATE TABLE a_tbl1(
        id INT UNIQUE,
        name VARCHAR,
        phone VARCHAR DEFAULT '000-0000'
    );
     
    --insert default values with DEFAULT keyword before VALUES
    INSERT INTO a_tbl1 DEFAULT VALUES;
     
    --insert multiple rows
    INSERT INTO a_tbl1 VALUES (1,'aaa', DEFAULT),(2,'bbb', DEFAULT);
     
    --insert a single row specifying column values for all
    INSERT INTO a_tbl1 VALUES (3,'ccc', '333-3333');
     
    --insert two rows specifying column values for only
    INSERT INTO a_tbl1(id) VALUES (4), (5);
     
    --insert a single row with SET clauses
    INSERT INTO a_tbl1 SET id=6, name='eee';
    INSERT INTO a_tbl1 SET id=7, phone='777-7777';
    
    SELECT * FROM a_tbl1;
    
::
    
               id  name                  phone
    =========================================================
             NULL  NULL                  '000-0000'
                1  'aaa'                 '000-0000'
                2  'bbb'                 '000-0000'
                3  'ccc'                 '333-3333'
                4  NULL                  '000-0000'
                5  NULL                  '000-0000'
                6  'eee'                 '000-0000'
                7  NULL                  '777-7777' 
     
.. code-block:: sql

    INSERT INTO a_tbl1 SET id=6, phone='000-0000'
    ON DUPLICATE KEY UPDATE phone='666-6666';
    SELECT * FROM a_tbl1 WHERE id=6;
    
::

               id  name                  phone
    =========================================================
                6  'eee'                 '666-6666'
     
.. code-block:: sql

    INSERT INTO a_tbl1 SELECT * FROM a_tbl1 WHERE id=7 ON DUPLICATE KEY UPDATE name='ggg';
    SELECT * FROM a_tbl1 WHERE id=7;
    
::

    
               id  name                  phone
    =========================================================
                7  'ggg'                 '777-7777'

In **INSERT ... SET** syntax, the evaluation of an assignment expression is performed from left to right. If the column value is not specified, then the default value is assigned. If there is no default value, **NULL** is assigned.
 
.. code-block:: sql

    CREATE TABLE tbl (a INT, b INT, c INT);
    INSERT INTO tbl SET a=1, b=a+1, c=b+2;
    SELECT * FROM tbl;
    
::

            a            b            c
    ===================================
            1            2            4
    
In the above example, b's value will be 2 and c's value will be 4 since a's value is 1.
 
.. code-block:: sql
 
    CREATE TABLE tbl2 (a INT, b INT, c INT);
    INSERT INTO tbl2 SET a=b+1, b=1, c=b+2;
 
In the above example, a's value will be **NULL** since b's value is not specified yet when assigning a's value.
 
.. code-block:: sql
    
    SELECT * FROM tbl2;

::
    
            a            b            c
    ===================================
         NULL            1            3
  
 
.. code-block:: sql
    
    CREATE TABLE tbl3 (a INT, b INT default 10, c INT);
    INSERT INTO tbl3 SET a=b+1, b=1, c=b+2;
 
In the above example, a's value will be 11 since b's value is not specified yet and b's default is 10.
   
.. code-block:: sql

    SELECT * FROM tbl3;
    
::

            a            b            c
    ===================================
           11            1            3

INSERT ... SELECT Statement
===========================

If you use the **SELECT** query in the **INSERT** statement, you can insert query results which satisfy the specified retrieval condition from one or many tables to the target table. 

::

    INSERT [INTO] [schema_name.]table_name [(column_name, ...)]
        SELECT...
        [ON DUPLICATE KEY UPDATE column_name = expr, ... ]

The **SELECT** statement can be used in place of the **VALUES** keyword, or be included as a subquery in the column value list next to **VALUES**. If you specify the **SELECT** statement in place of the **VALUES** keyword, you can insert multiple query result records into the column of the table at once. However, there should be only one query result record if the **SELECT** statement is specified in the column value list.

.. code-block:: sql

    --creating an empty table which schema replicated from a_tbl1
    CREATE TABLE a_tbl2 LIKE a_tbl1;
     
    --inserting multiple rows from SELECT query results
    INSERT INTO a_tbl2 SELECT * FROM a_tbl1 WHERE id IS NOT NULL;
     
    --inserting column value with SELECT subquery specified in the value list
    INSERT INTO a_tbl2 VALUES(8, SELECT name FROM a_tbl1 WHERE name <'bbb', DEFAULT);
     
    SELECT * FROM a_tbl2;
    
::

               id  name                  phone
    =========================================================
                1  'aaa'                 '000-0000'
                2  'bbb'                 '000-0000'
                3  'ccc'                 '333-3333'
                4  NULL                  '000-0000'
                5  NULL                  '000-0000'
                6  'eee'                 '000-0000'
                7  NULL                  '777-7777'
                8  'aaa'                 '000-0000'

INSERT INTO <remote-table-spec>… SELECT statement
==================================================

If a remote table of the same remote server used in the SELECT query is used in the INSERT statement, query results that satisfy specific search conditions from one or more remote tables can be inserted into the remote table. However, an error occurs when the remote table of the INSERT statement among the tables specified in the SELECT statement and the table of another server (local table or table of another remote server) are used.

.. code-block:: sql

    --at remote-side
    --creating an empty table which schema replicated from a_tbl1
    CREATE TABLE a_tbl2 LIKE a_tbl1;

    --at local-side
    --inserting multiple rows from SELECT query results
    INSERT INTO a_tbl2@server1 SELECT * FROM a_tbl1@server1 WHERE id IS NOT NULL;

    --inserting column value with SELECT subquery specified in the value list
    INSERT INTO a_tbl2@server1 VALUES(8, SELECT name FROM a_tbl1@server1 WHERE name <'bbb', DEFAULT);

    SELECT * FROM a_tbl2@server1;

::

               id  name                  phone
    =========================================================
                1  'aaa'                 '000-0000'
                2  'bbb'                 '000-0000'
                3  'ccc'                 '333-3333'
                4  NULL                  '000-0000'
                5  NULL                  '000-0000'
                6  'eee'                 '000-0000'
                7  NULL                  '777-7777'
                8  'aaa'                 '000-0000'

Data from a remote table can be inserted into a local table, but queries that insert data from a local table into a remote table cannot be used, so care must be taken when using it. The query below throws an error.

.. code-block:: sql

    --inserting multiple rows from SELECT query results
    INSERT INTO a_tbl2@server1 SELECT * FROM a_tbl1 WHERE id IS NOT NULL;

::

    dblink: local mixed remote DML is not allowed

Also, if the server of the remote table used in the INSERT statement and the SELECT statement are different, query execution is not allowed. The query below throws an error.

.. code-block:: sql

    --inserting multiple rows from SELECT query results
    INSERT INTO a_tbl2@server1 SELECT * FROM a_tbl1@server2 WHERE id IS NOT NULL;

::

    dblink: multi-remote DML is not allowed

ON DUPLICATE KEY UPDATE Clause
==============================

In a situation in which a duplicate value is inserted into a column for which the **UNIQUE** index or the **PRIMARY KEY** constraint has been set, you can update to a new value by specifying the **ON DUPLICATE KEY UPDATE** clause in the **INSERT** statement.

.. note::

    *   If **PRIMARY KEY** and **UNIQUE** or multiple **UNIQUE** constraints exist on a table together, constraint violation can happen by one of them; so in this case, **ON DUPLICATE KEY UPDATE** clause is not recommended.
    *   Even if **UPDATE** is executed after failing executing **INSERT**, **AUTO_INCREMENT** value which is increased once cannot be rolled back into the previous value.

::

    <INSERT ... VALUES statement>
    <INSERT ... SET statement>
    <INSERT ... SELECT statement>
        INSERT ...
        [ON DUPLICATE KEY UPDATE column_name = expr, ... ]

*   *column_name* = *expr*: Specifies the name of the column whose value you want to change next to **ON DUPLICATE KEY UPDATE** and a new column value by using the equal sign.

.. code-block:: sql

    --creating a new table having the same schema as a_tbl1
    CREATE TABLE a_tbl3 LIKE a_tbl1;
    INSERT INTO a_tbl3 SELECT * FROM a_tbl1 WHERE id IS NOT NULL and name IS NOT NULL;
    SELECT * FROM a_tbl3;
    
::

               id  name                  phone
    =========================================================
                1  'aaa'                 '000-0000'
                2  'bbb'                 '000-0000'
                3  'ccc'                 '333-3333'
                6  'eee'                 '000-0000'
     
.. code-block:: sql

    --insert duplicated value violating UNIQUE constraint
    INSERT INTO a_tbl3 VALUES(2, 'bbb', '222-2222');
     
::

    ERROR: Operation would have caused one or more unique constraint violations.

With ON DUPLICATE KEY UPDATE, "affected rows" value per row will be 1 if a new row is inserted, and 2 if an existing row is updated.

.. code-block:: sql
    
    --insert duplicated value with specifying ON DUPLICATED KEY UPDATE clause
    INSERT INTO a_tbl3 VALUES(2, 'ggg', '222-2222')
    ON DUPLICATE KEY UPDATE name='ggg', phone = '222-2222';
     
    SELECT * FROM a_tbl3 WHERE id=2;
    
::

               id  name                  phone
    =========================================================
                2  'ggg'                 '222-2222'

    2 rows affected.
