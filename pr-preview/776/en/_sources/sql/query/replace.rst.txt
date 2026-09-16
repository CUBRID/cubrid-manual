
:meta-keywords: replace statement
:meta-description: The REPLACE statement works like INSERT, but the difference is that it inserts a new record after deleting the existing record without displaying the error when a duplicate value is to be inserted into a column for which PRIMARY KEY or UNIQUE constraints have defined.

*******
REPLACE
*******

The **REPLACE** statement works like :doc:`insert`, but the difference is that it inserts a new record after deleting the existing record without displaying the error when a duplicate value is to be inserted into a column for which **PRIMARY KEY** or **UNIQUE** constraints have defined. You must have both **INSERT** and **DELETE** authorization to use the **REPLACE** statement, because it performs insertion or insertion after deletion operations. Please see :ref:`granting-authorization` for more information about authorization.

::

    <REPLACE ... VALUES statement>
    REPLACE [INTO] <table_specification> [(column_name, ...)]
        {VALUES | VALUE}({expr | DEFAULT}, ...)[,({expr | DEFAULT}, ...),...]
     
    <REPLACE ... SET statement>
    REPLACE [INTO] <table_specification>
        SET column_name = {expr | DEFAULT}[, column_name = {expr | DEFAULT},...]
     
    <REPLACE ... SELECT statement>
    REPLACE [INTO] <table_specification> [column_name, ...)]
        SELECT...

    <table_specfication> ::= [schema_name.]table_name
                           | [schema_name.]table_name@[schema_name.]server_name

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.

*   *table_name*: Specifies the name of the target table into which you want to insert a new record.

*   *server_name*: Specifies the remote server name of the target table. For local tables, do not specify *server_name*.

*   *column_name*: Specifies the name of the column into which you want to insert the value. If you omit to specify the column name, it is considered that all columns defined in the table have been specified. Therefore, you must specify the value for the column next to **VALUES**. If you do not specify all the columns defined in the table, a **DEFAULT** value is assigned to the non-specified columns; if the **DEFAULT** value is not defined, a **NULL** value is assigned.

*   *expr* | **DEFAULT**: Specifies values that correspond to the columns after **VALUES**. Expressions or the **DEFAULT** keyword can be specified as a value. At this time, the order and number of the specified column list must correspond to the column value list. The column value list for a single record is described in parentheses.

The **REPLACE** statement determines whether a new record causes the duplication of **PRIMARY KEY** or **UNIQUE** index column values. Therefore, for performance reasons, it is recommended to use the **INSERT** statement for a table for which a **PRIMARY KEY** or **UNIQUE** index has not been defined. 

.. code-block:: sql

    --creating a new table having the same schema as a_tbl1
    CREATE TABLE a_tbl4 LIKE a_tbl1;
    INSERT INTO a_tbl4 SELECT * FROM a_tbl1 WHERE id IS NOT NULL and name IS NOT NULL;
    
    SELECT * FROM a_tbl4;
    
::

               id  name                  phone
    =========================================================
                1  'aaa'                 '000-0000'
                2  'bbb'                 '000-0000'
                3  'ccc'                 '333-3333'
                6  'eee'                 '000-0000'
     
.. code-block:: sql

    --insert duplicated value violating UNIQUE constraint
    REPLACE INTO a_tbl4 VALUES(1, 'aaa', '111-1111'),(2, 'bbb', '222-2222');
    REPLACE INTO a_tbl4 SET id=6, name='fff', phone=DEFAULT;
     
    SELECT * FROM a_tbl4;
    
::

               id  name                  phone
    =========================================================
                3  'ccc'                 '333-3333'
                1  'aaa'                 '111-1111'
                2  'bbb'                 '222-2222'
                6  'fff'                 '000-0000'
