
:meta-keywords: table definition, create table, drop table, alter table, column definition, constraint definition, create table like, create table as select, rename table
:meta-description: Define tables in CUBRID database using create table, alter table, drop table and rename table statements.

***************************
TABLE DEFINITION STATEMENTS
***************************

CREATE TABLE
============

Table Definition
----------------

Use **CREATE TABLE** statement to create a table. A table with the same name as an existing view or synonym cannot be created. Regarding writing table name, see :doc:`/sql/identifier`. 

.. CUBRIDSUS-12366: from 10.0, create table if not exists ...

::

    CREATE {TABLE | CLASS} [IF NOT EXISTS] [schema_name.]table_name
    [<subclass_definition>]
    [CLASS ATTRIBUTE (<column_definition>, ...)]
    [([{<table_constraint>}... ,] <column_definition> [{, {<column_definition> | <table_constraint>}}...])]
    [INHERIT <resolution>, ...]
    [<table_options>]

        <subclass_definition> ::= {UNDER | AS SUBCLASS OF} [schema_name.]superclass_name, ...
        
        <column_definition> ::= 
            column_name <data_type> [{<default_or_shared_or_ai> | <on_update> | <column_constraint>}] [COMMENT 'column_comment_string']
        
            <data_type> ::= <column_type> [<charset_modifier_clause>] [<collation_modifier_clause>]

                <charset_modifier_clause> ::= {CHARACTER_SET|CHARSET} {<char_string_literal>|<identifier>}

                <collation_modifier_clause> ::= COLLATE {<char_string_literal>|<identifier>}
            
            <default_or_shared_or_ai> ::=
                SHARED <value_specification> | 
                DEFAULT <value_specification>  |
                AUTO_INCREMENT [(seed, increment)]

            <on_update> ::= [ON UPDATE <value_specification>]

            <column_constraint> ::= [CONSTRAINT constraint_name] { NOT NULL | UNIQUE | PRIMARY KEY | [FOREIGN KEY] [WITH <index_with_option>] <referential_definition> }

                <referential_definition> ::=
                    REFERENCES [schema_name.]referenced_table_name (column_name, ...) [<referential_triggered_action> ...]
         
                    <referential_triggered_action> ::=
                        ON UPDATE <referential_action> |
                        ON DELETE <referential_action> 

                        <referential_action> ::= CASCADE | RESTRICT | NO ACTION | SET NULL
                        
        <table_constraint> ::= 
            { 
                {KEY|INDEX} index_name <index_col_desc> |
                [CONSTRAINT [constraint_name]]
                   {
                      UNIQUE [KEY|INDEX](column_name, ...) |
                      PRIMARY KEY (column_name, ...) |
                      <referential_constraint>
                   }
            } COMMENT 'index_comment_string'
         
            <index_col_desc> ::=
              { ( {column_name | function_name (argument_list)} [ASC | DESC] [{, {column_name | function_name (argument_list)} [ASC | DESC]} ...] ) }
              [WHERE <filter_predicate>]
              [WITH <index_with_option>]
              [INVISIBLE]
              [COMMENT 'index_comment_string’]
         
            <referential_constraint> ::= FOREIGN KEY [<foreign_key_name>](column_name, ...) [WITH <index_with_option>] <referential_definition>
         
                <referential_definition> ::=
                    REFERENCES [schema_name.]referenced_table_name (column_name, ...) [<referential_triggered_action> ...]
         
                    <referential_triggered_action> ::=
                        ON UPDATE <referential_action> |
                        ON DELETE <referential_action> 
        
                        <referential_action> ::= CASCADE | RESTRICT | NO ACTION | SET NULL
     
        <resolution> ::= [CLASS] {column_name} OF [schema_name.]superclass_name [AS alias]
        <table_options> ::= <table_option> [[,] <table_option> ...] 
            <table_option> ::= REUSE_OID | DONT_REUSE_OID |
                               COMMENT [=] 'table_comment_string' |
                               [CHARSET charset_name] [COLLATE collation_name] |
                               ENCRYPT [=] [AES | ARIA] |
                               AUTO_INCREMENT = initial_value
                               
       <index_with_option> ::= {DEDUPLICATE ‘=‘ deduplicate_level }

*   IF NOT EXISTS: If an identically named table already exists, a new table will not be created without an error.
*   *schema_name*: Specifies the schema name(maximum: 31 bytes). If omitted, the schema name of the current session is used.
*   *table_name*: specifies the name of the table to be created (maximum: 222 bytes).
*   *column_name*: specifies the name of the column to be created (maximum: 254 bytes).
*   *column_type*: specifies the data type of the column.
*   [**SHARED** *value* | **DEFAULT** *value*]: specifies the initial value of the column.
*   **ON UPDATE** specifies an expression to update the field when the field's ROW gets updated. For details, see :ref:`constraint-definition`.
*   <*column_constraint*>: specifies the constraint of the column. Available constraints are **NOT NULL**, **UNIQUE**, **PRIMARY KEY** and **FOREIGN KEY**.
*   <*default_or_shared_or_ai*>: only one of DEFAULT, SHARED, AUTO_INCREMENT can be used.
    When AUTO_INCREMENT is specified, "(seed, increment)" and "AUTO_INCREMENT = initial_value" cannot be defined at the same time.
*   *table_comment_string*: specifies a table's comment
*   *column_comment_string*: specifies a column's comment.
*   *index_comment_string*: specifies an index's comment.
*   *deduplicate_level*: specifies the deduplicate level (0 to 14). For details, see :ref:`deduplicate_overview`.

.. note::

    *   **DBA** and **DBA** members can create tables in different schemas. If a user is neither **DBA** nor **DBA** member, tables can only be created in the schema of that user.

.. note::

    *   *deduplicate_level* is an integer from 0 to 14. 
    *   When this parameter is set to 0, it indicates an index configuration where data deduplication within indexes is not performed. This configuration is the same as that in CUBRID 11.2 or an earlier version.

.. code-block:: sql

    CREATE TABLE olympic2 (
        host_year        INT    NOT NULL PRIMARY KEY,
        host_nation      VARCHAR(40) NOT NULL,
        host_city        VARCHAR(20) NOT NULL,
        opening_date     DATE        NOT NULL,
        closing_date     DATE        NOT NULL,
        mascot           VARCHAR(20),
        slogan           VARCHAR(40),
        introduction     VARCHAR(1500)
    );

The below adds a comment of a table with ALTER statement.

.. code-block:: sql
    
    ALTER TABLE olympic2 COMMENT = 'this is new comment for olympic2';

The below includes an index comment when you create a table.

    CREATE TABLE tbl (a INT, index i_t_a (a) COMMENT 'index comment');

.. note:: **A CHECK constraint in the table schema**

    A CHECK constraint defined in the table schema is parsed, but ignored. The reason of being parsed is to support the compatibility when DB migration from other DBMS is done.
    
    .. code-block:: sql
    
        CREATE TABLE tbl (
            id INT PRIMARY KEY,
            CHECK (id > 0)
        )

.. _column-definition:

Column Definition
-----------------

A column is a set of data values of a particular simple type, one for each row of the table.

::

    <column_definition> ::= 
        column_name <data_type> [[<default_or_shared_or_ai>] | [<on_update>] | [<column_constraint>]] ... [COMMENT 'comment_string']
    
        <data_type> ::= <column_type> [<charset_modifier_clause>] [<collation_modifier_clause>]

            <charset_modifier_clause> ::= {CHARACTER_SET|CHARSET} {<char_string_literal>|<identifier>}

            <collation_modifier_clause> ::= COLLATE {<char_string_literal>|<identifier>}
        
        <default_or_shared_or_ai> ::=
            SHARED <value_specification> | 
            DEFAULT <value_specification>  |
            AUTO_INCREMENT [(seed, increment)]

        <on_update> ::= [ON UPDATE <value_specification>]

        <column_constraint> ::= [CONSTRAINT constraint_name] {NOT NULL | UNIQUE | PRIMARY KEY | [FOREIGN KEY] [WITH <index_with_option>] <referential_definition>}

Column Name
^^^^^^^^^^^

How to create a column name, see :doc:`/sql/identifier`. You can alter created column name by using the :ref:`rename-column` of the **ALTER TABLE** statement.

The following example shows how to create the *manager2* table that has the following two columns: *full_name* and *age*.

.. code-block:: sql

    CREATE TABLE manager2 (full_name VARCHAR(40), age INT );

.. note::

    *   The first character of a column name must be an alphabet.
    *   The column name must be unique in the table.

Setting the Column Initial Value (SHARED, DEFAULT)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

An attribute in a table can be created with an initial **SHARED** or **DEFAULT** value. You can change the value of **SHARED** and **DEFAULT** in the **ALTER TABLE** statement.

*   **SHARED** : Column values are identical in all rows. If a value different from the initial value is **INSERT**\ ed, the column value is updated to a new one in every row.
*   **DEFAULT** : The initial value set when the **DEFAULT** attribute was defined is stored even if the column value is not specified when a new row is inserted.

The pseudocolumn allows for the **DEFAULT** value as follows.

+-------------------------------+---------------+
| DEFAULT Value                 | Data Type     |
+===============================+===============+
| SYS_TIMESTAMP                 | TIMESTAMP     |
+-------------------------------+---------------+
| UNIX_TIMESTAMP()              | INTEGER       |
+-------------------------------+---------------+
| CURRENT_TIMESTAMP             | TIMESTAMP     |
+-------------------------------+---------------+
| SYS_DATETIME                  | DATETIME      |
+-------------------------------+---------------+
| CURRENT_DATETIME              | DATETIME      |
+-------------------------------+---------------+
| SYS_DATE                      | DATE          |
+-------------------------------+---------------+
| CURRENT_DATE                  | DATE          |
+-------------------------------+---------------+
| SYS_TIME                      | TIME          |
+-------------------------------+---------------+
| CURRENT_TIME                  | TIME          |
+-------------------------------+---------------+
| USER, USER()                  | STRING        |
+-------------------------------+---------------+
| TO_CHAR(date_time[, format])  | STRING        |
+-------------------------------+---------------+
| TO_CHAR(number[, format])     | STRING        |
+-------------------------------+---------------+

.. note::

    In version lower than CUBRID 9.0, the value at the time of **CREATE TABLE** has been saved when the **DATE** value of the **DATE**, **DATETIME**, **TIME**, **TIMESTAMP** column has been specified as **SYS_DATE**, **SYS_DATETIME**, **SYS_TIME**, **SYS_TIMESTAMP** while creating a table. Therefore, to enter the value at the time of data **INSERT** in version lower than CUBRID 9.0, the function should be entered to the **VALUES** clause of the **INSERT** syntax.

.. code-block:: sql

    CREATE TABLE colval_tbl
    (id INT, name VARCHAR SHARED 'AAA', phone VARCHAR DEFAULT '000-0000');
    INSERT INTO colval_tbl (id) VALUES (1), (2);
    SELECT * FROM colval_tbl;
    
::
     
               id  name                  phone
    =========================================================
                1  'AAA'                 '000-0000'
                2  'AAA'                 '000-0000'
     
.. code-block:: sql

    --updating column values on every row
    INSERT INTO colval_tbl(id, name) VALUES (3,'BBB');
    INSERT INTO colval_tbl(id) VALUES (4),(5);
    SELECT * FROM colval_tbl;
     
::

               id  name                  phone
    =========================================================
                1  'BBB'                 '000-0000'
                2  'BBB'                 '000-0000'
                3  'BBB'                 '000-0000'
                4  'BBB'                 '000-0000'
                5  'BBB'                 '000-0000'
     
.. code-block:: sql

    --changing DEFAULT value in the ALTER TABLE statement
    ALTER TABLE colval_tbl MODIFY phone VARCHAR DEFAULT '111-1111';
    INSERT INTO colval_tbl (id) VALUES (6);
    SELECT * FROM colval_tbl;
     
::

               id  name                  phone
    =========================================================
                1  'BBB'                 '000-0000'
                2  'BBB'                 '000-0000'
                3  'BBB'                 '000-0000'
                4  'BBB'                 '000-0000'
                5  'BBB'                 '000-0000'
                6  'BBB'                 '111-1111'

.. code-block:: sql

    --use DEFAULT TO_CHAR in CREATE TABLE statement
    CREATE TABLE t1(id1 INT, id2 VARCHAR(20) DEFAULT TO_CHAR(12345,'S999999'));
    INSERT INTO t1 (id1) VALUES (1);
    SELECT * FROM t1;

::    
    
              id1  id2
    ===================================
                1  ' +12345'

The **DEFAULT** value of the pseudocolumn can be specified as one or more columns.

.. code-block:: sql

    CREATE TABLE tbl (date1 DATE DEFAULT SYSDATE, date2 DATE DEFAULT SYSDATE);
    CREATE TABLE tbl (date1 DATE DEFAULT SYSDATE,
                      ts1   TIMESTAMP DEFAULT CURRENT_TIMESTAMP);
    CREATE TABLE t1(id1 INT, id2 VARCHAR(20) DEFAULT TO_CHAR(12345,'S999999'), id3 VARCHAR(20) DEFAULT TO_CHAR(SYS_TIME, 'HH24:MI:SS'));
    ALTER TABLE t1 add column id4 varchar (20) default TO_CHAR(SYS_DATETIME, 'yyyy/mm/dd hh:mi:ss'), id5 DATE DEFAULT SYSDATE;

AUTO INCREMENT
^^^^^^^^^^^^^^

You can define the **AUTO_INCREMENT** attribute for the column to automatically give serial numbers to column values. This can be defined only for **SMALLINT**, **INTEGER**, **BIGINT** and **NUMERIC**\ (*p*, 0) types.

**DEFAULT**, **SHARED** and **AUTO_INCREMENT** cannot be defined for the same column. Make sure the value entered directly by the user and the value entered by the auto increment attribute do not conflict with each other.

You can change the initial value of **AUTO_INCREMENT** by using the **ALTER TABLE** statement. For details, see :ref:`alter-auto-increment` of **ALTER TABLE**. 

::

    CREATE TABLE [schema_name.]table_name (id INT AUTO_INCREMENT[(seed, increment)]);

    CREATE TABLE [schema_name.]table_name (id INT AUTO_INCREMENT) AUTO_INCREMENT = seed ;

*   *seed* : The initial value from which the number starts. All integers (positive, negative, and zero) are allowed. The default value is **1**.
*   *increment* : The increment value of each row. Only positive integers are allowed. The default value is **1**.

When you use the **CREATE TABLE** *[schema_name.]table_name* (id INT **AUTO_INCREMENT**) **AUTO_INCREMENT** = *seed*; statement, the constraints are as follows:

*   You should define only one column with the **AUTO_INCREMENT** attribute.
*   Don't use (*seed*, *increment*) and AUTO_INCREMENT = *seed* together.

.. code-block:: sql

    CREATE TABLE auto_tbl (id INT AUTO_INCREMENT, name VARCHAR);
    INSERT INTO auto_tbl VALUES (NULL, 'AAA'), (NULL, 'BBB'), (NULL, 'CCC');
    INSERT INTO auto_tbl (name) VALUES ('DDD'), ('EEE');
    SELECT * FROM auto_tbl;
     
::

               id  name
    ===================================
                1  'AAA'
                2  'BBB'
                3  'CCC'
                4  'DDD'
                5  'EEE'
     
.. code-block:: sql

    CREATE TABLE tbl (id INT AUTO_INCREMENT, val string) AUTO_INCREMENT = 3;
    INSERT INTO tbl VALUES (NULL, 'cubrid');
     
    SELECT * FROM tbl;
    
::

               id  val
    ===================================
                3  'cubrid'
     
.. code-block:: sql

    CREATE TABLE t (id INT AUTO_INCREMENT, id2 int AUTO_INCREMENT) AUTO_INCREMENT = 5;
    
::
    
    ERROR: To avoid ambiguity, the AUTO_INCREMENT table option requires the table to  have exactly one AUTO_INCREMENT column and no seed/increment specification.
     
.. code-block:: sql

    CREATE TABLE t (i INT AUTO_INCREMENT(100, 2)) AUTO_INCREMENT = 3;
    
::

    ERROR: To avoid ambiguity, the AUTO_INCREMENT table option requires the table to  have exactly one AUTO_INCREMENT column and no seed/increment specification.

.. note::

    *   Even if a column has auto increment, the **UNIQUE** constraint is not satisfied.
    *   If **NULL** is specified in the column where auto increment is defined, the value of auto increment is stored.
    *   Even if a value is directly specified in the column where auto increment is defined, AUTO_INCREMENT value is not changed.
    *   **SHARED** or **DEFAULT** attribute cannot be specified in the column in which AUTO_INCREMENT is defined.
    *   The initial value and the final value obtained by auto increment cannot exceed the minimum and maximum values allowed in the given type.
    *   Because auto increment has no cycle, an error occurs when the maximum value of the type exceeds, and no rollback is executed. Therefore, you must delete and recreate the column in such cases.

        For example, if a table is created as below, the maximum value of A is 32767. Because an error occurs if the value exceeds 32767, you must make sure that the maximum value of the column A does not exceed the maximum value of the type when creating the initial table.

        .. code-block:: sql
          
            CREATE TABLE tb1(A SMALLINT AUTO_INCREMENT, B CHAR(5));

.. _constraint-definition:

ON UPDATE
---------

An attribute in a table can be created with an automatic update when another attribute of the row is updated. You can change the value of **ON UPDATE** in the **ALTER TABLE** statement.
The pseudocolumn allows for the **ON UPDATE** value as follows.
Including the attribute in the updated fields will not trigger an update with the specified **ON UPDATE** value.

+-------------------------------+---------------+
| DEFAULT Value                 | Data Type     |
+===============================+===============+
| SYS_TIMESTAMP                 | TIMESTAMP     |
+-------------------------------+---------------+
| UNIX_TIMESTAMP()              | INTEGER       |
+-------------------------------+---------------+
| CURRENT_TIMESTAMP             | TIMESTAMP     |
+-------------------------------+---------------+
| SYS_DATETIME                  | DATETIME      |
+-------------------------------+---------------+
| CURRENT_DATETIME              | DATETIME      |
+-------------------------------+---------------+
| SYS_DATE                      | DATE          |
+-------------------------------+---------------+
| CURRENT_DATE                  | DATE          |
+-----------------------------------------------+

.. code-block:: sql

    CREATE TABLE sales (sales_cnt INTEGER, last_sale TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, product VARCHAR(100), product_id INTEGER);
    INSERT INTO sales VALUES (0, NULL, 'bicycle', 1);

    UPDATE sales set sales_cnt = sales_cnt + 1
    WHERE product_id = 1;

.. code-block:: sql

   ALTER TABLE sales MODIFY last_sale TIMESTAMP; -- removes ON UPDATE
   UPDATE sales set sales_cnt = sales_cnt + 1
   WHERE product_id = 1; -- last_sale will remain unupdated

Constraint Definition
---------------------

You can define **NOT NULL**, **UNIQUE**, **PRIMARY KEY**, **FOREIGN KEY** as the constraints. You can also create an index by using **INDEX** or **KEY**. 

::

    <column_constraint> ::= [CONSTRAINT constraint_name] { NOT NULL | UNIQUE | PRIMARY KEY | [FOREIGN KEY] [WITH <index_with_option>] <referential_definition> }

    <table_constraint> ::=         
        { 
            {KEY|INDEX} index_name <index_col_desc> |
            [CONSTRAINT [constraint_name]]
                {
                      UNIQUE [KEY|INDEX](column_name, ...) |
                      PRIMARY KEY (column_name, ...) |
                      <referential_constraint>
                }
        }
     
        <referential_constraint> ::= FOREIGN KEY [<foreign_key_name>](column_name, ...) [WITH <index_with_option>] <referential_definition>
     
            <referential_definition> ::=
                REFERENCES [schema_name.]referenced_table_name (column_name, ...) [<referential_triggered_action> ...]
     
                <referential_triggered_action> ::=
                    ON UPDATE <referential_action> |
                    ON DELETE <referential_action> 
    
                    <referential_action> ::= CASCADE | RESTRICT | NO ACTION | SET NULL
                    
        <index_with_option> ::= {DEDUPLICATE ‘=‘ deduplicate_level}

NOT NULL Constraint
^^^^^^^^^^^^^^^^^^^

A column for which the **NOT NULL** constraint has been defined must have a certain value that is not **NULL**. The **NOT NULL** constraint can be defined for all columns. An error occurs if you try to insert a **NULL** value into a column with the **NOT NULL** constraint by using the **INSERT** or **UPDATE** statement.

In the following example, if you input NULL value on the *id* column, it occurs an error because *id* column cannot have NULL value.

.. code-block:: sql

    CREATE TABLE const_tbl1(id INT NOT NULL, INDEX i_index(id ASC), phone VARCHAR);
     
    CREATE TABLE const_tbl2(id INT NOT NULL PRIMARY KEY, phone VARCHAR);
    INSERT INTO const_tbl2 VALUES (NULL,'000-0000');
     
::

    Putting value 'null' into attribute 'id' returned: Attribute "id" cannot be made NULL.

UNIQUE Constraint
^^^^^^^^^^^^^^^^^

The **UNIQUE** constraint enforces a column to have a unique value. An error occurs if a new record that has the same value as the existing one is added by this constraint.

You can place a **UNIQUE** constraint on either a column or a set of columns. If the **UNIQUE** constraint is defined for multiple columns, the uniqueness is ensured not for each column, but the combination of multiple columns.

In the following example, the second INSERT statement fails because the value of *id* column is the same as 1 with the value of *id* column in the first INSERT statement.

.. code-block:: sql

    -- UNIQUE constraint is defined on a single column only
    CREATE TABLE const_tbl5(id INT UNIQUE, phone VARCHAR);
    INSERT INTO const_tbl5(id) VALUES (NULL), (NULL);
    INSERT INTO const_tbl5 VALUES (1, '000-0000');
    SELECT * FROM const_tbl5;

::

       id  phone
    =================
     NULL  NULL
     NULL  NULL
        1  '000-0000'
     
.. code-block:: sql

    INSERT INTO const_tbl5 VALUES (1, '111-1111');
     
::

    ERROR: Operation would have caused one or more unique constraint violations.

In the following example, if a **UNIQUE** constraint is defined on several columns, this ensures the uniqueness of the values in all the columns.

.. code-block:: sql
     
    -- UNIQUE constraint is defined on several columns
    CREATE TABLE const_tbl6(id INT, phone VARCHAR, CONSTRAINT UNIQUE (id, phone));
    INSERT INTO const_tbl6 VALUES (1, NULL), (2, NULL), (1, '000-0000'), (1, '111-1111');
    SELECT * FROM const_tbl6;

::
    
       id  phone
    ====================
        1  NULL
        2  NULL
        1  '000-0000'
        1  '111-1111'

PRIMARY KEY Constraint
^^^^^^^^^^^^^^^^^^^^^^

A key in a table is a set of column(s) that uniquely identifies each row. A candidate key is a set of columns that uniquely identifies each row of the table. You can define one of such candidate keys a primary key. That is, the column defined as a primary key is uniquely identified in each row.

By default, the index created by defining the primary key is created in ascending order, and you can define the order by specifying **ASC** or **DESC** keyword next to the column. 

.. code-block:: sql

    CREATE TABLE pk_tbl (a INT, b INT, PRIMARY KEY (a, b DESC));

    CREATE TABLE const_tbl7 (
        id INT NOT NULL,
        phone VARCHAR,
        CONSTRAINT pk_id PRIMARY KEY (id)
    );
     
    -- CONSTRAINT keyword
    CREATE TABLE const_tbl8 (
        id INT NOT NULL PRIMARY KEY,
        phone VARCHAR
    );
     
    -- primary key is defined on multiple columns
    CREATE TABLE const_tbl8 (
        host_year    INT NOT NULL,
        event_code   INT NOT NULL,
        athlete_code INT NOT NULL,
        medal        CHAR (1)  NOT NULL,
        score        VARCHAR (20),
        unit         VARCHAR (5),
        PRIMARY KEY (host_year, event_code, athlete_code, medal)
    );

.. _foreign-key-constraint:

FOREIGN KEY Constraint
^^^^^^^^^^^^^^^^^^^^^^

A foreign key is a column or a set of columns that references the primary key in other tables in order to maintain reference relationship. The foreign key and the referenced primary key must have the same data type. Consistency between two tables is maintained by the foreign key referencing the primary key, which is called referential integrity. ::

    [CONSTRAINT constraint_name] FOREIGN KEY [foreign_key_name] (<column_name_comma_list1>) [WITH <index_with_option>] REFERENCES [schema_name.]referenced_table_name (<column_name_comma_list2>) [<referential_triggered_action> ...]
     
        <referential_triggered_action> ::= ON UPDATE <referential_action> | ON DELETE <referential_action>

            <referential_action> ::= CASCADE | RESTRICT | NO ACTION  | SET NULL

        <index_with_option> ::= {DEDUPLICATE ‘=‘ deduplicate_level}

*   *constraint_name*: Specifies the name of the table to be created.
*   *foreign_key_name*: Specifies a name of the **FOREIGN KEY** constraint. You can skip the name specification. However, if you specify this value, *constraint_name* will be ignored, and the specified value will be used.

*   <*column_name_comma_list1*>: Specifies the name of the column to be defined as a foreign key after the **FOREIGN KEY** keyword. The column number of foreign keys defined and primary keys must be same.
*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *referenced_table_name*: Specifies the name of the table to be referenced.
*   <*column_name_comma_list2*>: Specifies the name of the referred primary key column after the **FOREIGN KEY** keyword.
*   <*referential_triggered_action*>: Specifies the trigger action that responds to a certain operation in order to maintain referential integrity. **ON UPDATE** or **ON DELETE** can be specified. Each action can be defined multiple times, and the definition order is not significant.

    *   **ON UPDATE**: Defines the action to be performed when attempting to update the primary key referenced by the foreign key. You can use either **NO ACTION**, **RESTRICT**, or **SET NULL** option. The default is **RESTRICT**.
    *   **ON DELETE**: Defines the action to be performed when attempting to delete the primary key referenced by the foreign key. You can use **NO ACTION**, **RESTRICT**, **CASCADE**, or **SET NULL** option. The default is **RESTRICT**.

*   <*referential_action*>: You can define an option that determines whether to maintain the value of the foreign key when the primary key value is deleted or updated.

    *   **CASCADE**: If the primary key is deleted, the foreign key is deleted as well. This option is supported only for the **ON DELETE** operation.
    *   **RESTRICT**: Prevents the value of the primary key from being deleted or updated, and rolls back any transaction that has been attempted.
    *   **SET NULL**: When a specific record is being deleted or updated, the column value of the foreign key is updated to **NULL**.
    *   **NO ACTION**: Its behavior is the same as that of the **RESTRICT** option.

*   *deduplicate_level*: specifies the deduplicate level (0 to 14). For details, see :ref:`deduplicate_overview`.

For each row R1 of the referencing table, there should be some row R2 of the referenced table such that the value of each referencing column in R1 is either **NULL** or is equal to the value of the corresponding referenced column in R2.

.. code-block:: sql

    -- creating two tables where one is referring to the other
    CREATE TABLE a_tbl (
        id INT NOT NULL DEFAULT 0 PRIMARY KEY,
        phone VARCHAR(10)
    );
     
    CREATE TABLE b_tbl (
        id INT NOT NULL,
        name VARCHAR (10) NOT NULL,
        CONSTRAINT pk_id PRIMARY KEY (id),
        CONSTRAINT fk_id FOREIGN KEY (id) REFERENCES a_tbl (id)
        ON DELETE CASCADE ON UPDATE RESTRICT
    );
     
    INSERT INTO a_tbl VALUES (1,'111-1111'), (2,'222-2222'), (3, '333-3333');
    INSERT INTO b_tbl VALUES (1,'George'),(2,'Laura'), (3,'Max');
    SELECT a.id, b.id, a.phone, b.name FROM a_tbl a, b_tbl b WHERE a.id = b.id;
     
::

       id           id                   phone                 name
    ======================================================================
        1            1                   '111-1111'            'George'
        2            2                   '222-2222'            'Laura'
        3            3                   '333-3333'            'Max'
     
.. code-block:: sql

    -- when deleting primary key value, it cascades foreign key value  
    DELETE FROM a_tbl WHERE id=3;
     
::

    1 row affected.
     
.. code-block:: sql

    SELECT a.id, b.id, a.phone, b.name FROM a_tbl a, b_tbl b WHERE a.id = b.id;
     
::

       id           id                   phone                 name
    ======================================================================
        1            1                   '111-1111'            'George'
        2            2                   '222-2222'            'Laura'

.. code-block:: sql

    -- when attempting to update primary key value, it restricts the operation
    UPDATE  a_tbl SET id = 10 WHERE phone = '111-1111';
     
::

    ERROR: Update/Delete operations are restricted by the foreign key 'fk_id'.

.. note::

    *   In a referential constraint, the name of the primary key table to be referenced and the corresponding column names are defined. If the list of column names are is not specified, the primary key of the primary key table is specified in the defined order.
    *   The number of primary keys in a referential constraint must be identical to that of foreign keys. The same column name cannot be used multiple times for the primary key in the referential constraint.
    *   The actions cascaded by reference constraints do not activate the trigger action.
    *   It is not recommended to use *referential_triggered_action* in the CUBRID HA environment. In the CUBRID HA environment, the trigger action is not supported. Therefore, if you use *referential_triggered_action*, the data between the master database and the slave database can be inconsistent. For details, see :doc:`/ha`.

KEY or INDEX
^^^^^^^^^^^^

**KEY** and **INDEX** are used interchangeably. They create an index that uses the corresponding column as a key.

.. code-block:: sql

    CREATE TABLE const_tbl4(id INT, phone VARCHAR, KEY i_key(id DESC, phone ASC));

.. note:: In versions lower than CUBRID 9.0, index name can be omitted; however, in version of CUBRID 9.0 or higher, it is no longer allowed.

Column Option
-------------

You can specify options such as **ASC** or **DESC** after the column name when defining **UNIQUE** or **INDEX** for a specific column. This keyword is specified as store the index value in ascending or descending order. 

.. code-block:: sql

    column_name [ASC | DESC]

.. code-block:: sql

    CREATE TABLE const_tbl(
        id VARCHAR,
        name VARCHAR,
        CONSTRAINT UNIQUE INDEX(id DESC, name ASC)
    );
     
    INSERT INTO const_tbl VALUES('1000', 'john'), ('1000','johnny'), ('1000', 'jone');
    INSERT INTO const_tbl VALUES('1001', 'johnny'), ('1001','john'), ('1001', 'jone');
     
    SELECT * FROM const_tbl WHERE id > '100';
    
::

      id    name    
    =================
      1001     john     
      1001     johnny     
      1001     jone     
      1000     john     
      1000     johnny     
      1000     jone

Table Option
------------

**REUSE_OID** and **DONT_REUSE_OID** are options that specify whether to be referable when creating a table. The two options cannot be used together and can be used with other options. When creating a table without the option, the **REUSE_OID** table option is used. To change the default option to **DONT_REUSE_OID**, you should change the system parameter **create_table_reuseoid** to **no**. For detail, see :ref:`stmt-type-parameters` .

::

        <table_options> ::= <table_option> [[,] <table_option> ...]
            <table_option> ::= REUSE_OID | DONT_REUSE_OID |
                               COMMENT [=] 'table_comment_string' |
                               [CHARSET charset_name] [COLLATE collation_name]

.. _reuse-oid:

REUSE_OID
^^^^^^^^^

You can specify the **REUSE_OID** option when creating a table, so that OIDs that have been deleted due to the deletion of records (**DELETE**) can be reused when a new record is inserted (**INSERT**). Such a table is called an OID reusable or a non-referable table.

OID (Object Identifier) is an object identifier represented by physical location information such as the volume number, page number and slot number. By using such OIDs, CUBRID manages the reference relationships of objects and searches, stores or deletes them. When an OID is used, accessibility is improved because the object in the heap file can be directly accessed without referring to the table. However, the problem of decreased reusability of the storage occurs when there are many **DELETE/ INSERT** operations because the object's OID is kept to maintain the reference relationship with the object even if it is deleted.

If you specify the **REUSE_OID** option when creating a table, the OID is also deleted when data in the table is deleted, so that another **INSERT**\ ed data can use it. OID reusable tables cannot be referred to by other tables, and OID values of the objects in the OID reusable tables cannot be viewed.

.. code-block:: sql

    -- creating table with REUSE_OID option specified
    CREATE TABLE reuse_tbl (a INT PRIMARY KEY) REUSE_OID, COMMENT = 'reuse oid table';
    INSERT INTO reuse_tbl VALUES (1);
    INSERT INTO reuse_tbl VALUES (2);
    INSERT INTO reuse_tbl VALUES (3);
     
    -- an error occurs when column type is a OID reusable table itself
    CREATE TABLE tbl_1 (a reuse_tbl);

::
    
    ERROR: The class 'reuse_tbl' is marked as REUSE_OID and is non-referable. Non-referable classes can't be the domain of an attribute and their instances' OIDs cannot be returned.

If you specify REUSE_OID together with the collation of table, it can be placed on before or after **COLLATE** syntax.
     
.. code-block:: sql
    
    CREATE TABLE t3(a VARCHAR(20)) REUSE_OID, COMMENT = 'reuse oid table', COLLATE euckr_bin;
    CREATE TABLE t4(a VARCHAR(20)) COLLATE euckr_bin REUSE_OID;

.. note::

    *   OID reusable tables cannot be referred to by other tables.
    *   Updatable views cannot be created for OID reusable tables.
    *   OID reusable tables cannot be specified as table column type.
    *   OID values of the objects in the OID reusable tables cannot be read.
    *   Instance methods cannot be called from OID reusable tables. Also, instance methods cannot be called if a sub class inherited from the class where the method is defined is defined as an OID reusable table.
    *   OID reusable tables are supported only by CUBRID 2008 R2.2 or above, and backward compatibility is not ensured. That is, the database in which the OID reusable table is located cannot be accessed from a lower version database.
    *   OID reusable tables can be managed as partitioned tables and can be replicated.
    *   The updating query with a view can be faced with error when the view includes any OID reusable table.

.. code-block:: sql

   CREATE TABLE t1 (c1 INT) REUSE_OID;
   CREATE VIEW v3 AS SELECT c1 FROM t1;

   insert into v3(c1) values (1);

::

   ERROR: Vclass v3 is not updatable. Please check if any of its related classes are marked as REUSE_OID.

.. _dont-reuse-oid:

DONT_REUSE_OID
^^^^^^^^^^^^^^

Specifying the **DONT_REUSE_OID** option when creating the table will create a referable table as opposite to **REUSE_OID**.

Charset and Collation
^^^^^^^^^^^^^^^^^^^^^

The charset and collation of the table and column can be designated in the **CREATE TABLE** statement.
This example specifies the character set and collation for **CREATE TABLE**.

.. code-block:: sql

    CREATE TABLE tblutf8(a int, b int) charset utf8;
    CREATE TABLE tblutf8enci(a int, b int) collate utf8_en_ci;

    CREATE TABLE tbleuckr(a int, b int) charset euckr collate euckr_bin;
    CREATE TABLE tblcoleuckr(a int, c varchar(100) collate euckr_bin) charset euckr;

    CREATE TABLE tblcol (s1 STRING COLLATE utf8_en_cs,s2 STRING COLLATE utf8_bin);
    CREATE TABLE tblcharcol (col STRING CHARSET utf8) COLLATE utf8_en_ci;
 
Please see :ref:`collation-charset-table` or :ref:`collation-charset-string` for details.

Table's COMMENT
^^^^^^^^^^^^^^^

You can write a table's comment as following.

.. code-block:: sql

    CREATE TABLE tbl (a INT, b INT) COMMENT = 'this is comment for table tbl';

To see the table's comment, run the below syntax.

.. code-block:: sql

    SHOW CREATE TABLE table_name;
    SELECT class_name, comment from db_class;
    SELECT class_name, comment from _db_class;

Or you can see the table's comment with ;sc command in the CSQL interpreter.

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc tbl

.. _create-tde-table:

Table Encryption (TDE)
^^^^^^^^^^^^^^^^^^^^^^

You can encrypt a table as follows. For more information on TDE encryption, see :ref:`tde`.

.. code-block:: sql

    CREATE TABLE enc_tbl (a INT, b INT) ENCRYPT = AES;

You can specify **AES** or **ARIA** as the encryption algorithm. If omitted as follows, the encryption algorithm specified by the system parameter **tde_default_algorithm** is used. The default value is **AES**.

.. code-block:: sql

    CREATE TABLE enc_tbl (a INT, b INT) ENCRYPT;

The encryption information is not inherited.

CREATE TABLE LIKE
-----------------

You can create a table with the same schema as an existing table by using the **CREATE TABLE ... LIKE** statement. Column attribute, table constraint, index, and encryption information are replicated from the existing table. An index name created from the existing table changes according to a new table name, but an index name defined by a user is replicated as it is. Therefore, you should be careful with a query statement that is supposed to use a specific index created by using the index hint syntax(see :ref:`index-hint-syntax`).

You cannot create the column definition because the **CREATE TABLE ... LIKE** statement replicates the schema only. 

::

    CREATE {TABLE | CLASS} [schema_name.]new_table_name LIKE [schema_name.]source_table_name;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *new_table_name*: A table name to be created
*   *source_table_name*: The name of the original table that already exists in the database. The following tables cannot be specified as original tables in the **CREATE TABLE ... LIKE** statement.

    * Partition table
    * Table that contains an **AUTO_INCREMENT** column
    * Table that uses inheritance or methods

.. code-block:: sql

    CREATE TABLE a_tbl (
      id INT NOT NULL DEFAULT 0 PRIMARY KEY,
      phone VARCHAR(10)
    );
    INSERT INTO a_tbl VALUES (1,'111-1111'), (2,'222-2222'), (3, '333-3333');
     
    -- creating an empty table with the same schema as a_tbl
    CREATE TABLE new_tbl LIKE a_tbl;
    SELECT * FROM new_tbl;

::

    There are no results.
     
    csql> ;schema a_tbl
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         a_tbl
     
     <Attributes>
     
         id                   INTEGER DEFAULT 0 NOT NULL
         phone                CHARACTER VARYING(10)
     
     <Constraints>
     
         PRIMARY KEY pk_a_tbl_id ON a_tbl (id)
     
    csql> ;schema new_tbl
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         new_tbl
     
     <Attributes>
     
         id                   INTEGER DEFAULT 0 NOT NULL
         phone                CHARACTER VARYING(10)
     
     <Constraints>
     
         PRIMARY KEY pk_new_tbl_id ON new_tbl (id)

CREATE TABLE AS SELECT
----------------------

You can create a new table that contains the result records of the **SELECT** statement by using the **CREATE TABLE...AS SELECT** statement. You can define column and table constraints for the new table. The following rules are applied to reflect the result records of the **SELECT** statement.

*   If *col_1* is defined in the new table and the same column *col_1* is specified in *select_statement*, the result record of the **SELECT** statement is stored as *col_1* value in the new table. Type casting is attempted if the column names are identical but the columns types are different.

*   If *col_1* and  *col_2* are defined in the new table, *col_1*, col_2 and *col_3* are specified in the column list of the *select_statement* and there is a containment relationship between all of them, *col_1*, *col_2* and *col_3* are created in the new table and the result data of the **SELECT** statement is stored as values for all columns. Type casting is attempted if the column names are identical but the columns types are different.

*   If columns *col_1* and *col_2* are defined in the new table and *col_1* and *col_3* are defined in the column list of *select_statement* without any containment relationship between them, *col_1*, *col_2* and *col_3* are created in the new table, the result data of the **SELECT** statement is stored only for *col_1* and *col_3* which are specified in *select_statement*, and **NULL** is stored as the value of *col_2*.

*   Column aliases can be included in the column list of *select_statement*. In this case, new column alias is used as a new table column name. It is recommended to use an alias because invalid column name is created, if an alias does not exist when a function calling or an expression is used.

*   The **REPLACE** option is valid only when the **UNIQUE** constraint is defined in a new table column (*col_1*). When duplicate values exist in the result record of *select_statement*, a **UNIQUE** value is stored for *col_1* if the **REPLACE** option has been defined, or an error message is displayed if the **REPLACE** option is omitted due to the violation of the **UNIQUE** constraint.

::

    CREATE {TABLE | CLASS} [schema_name.]table_name [([{<table_constraint>}... ,] <column_definition> [{, {<column_definition> | <table_constraint>}}...])] [REPLACE] AS <select_statement>;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name*: a name of the table to be created.
*   <*column_definition*>: defines a column. If this is omitted, the column schema of **SELECT** statement is replicated; however, the constraint or the **AUTO_INCREMENT** attribute is not replicated.
*   <*table_constraint*>: defines table constraint.
*   <*select_statement*>: a **SELECT** statement targeting a source table that already exists in the database.
*   <*select statement*> can include remote tables like tbl@srver1.

.. code-block:: sql

    CREATE TABLE a_tbl (
      id INT NOT NULL DEFAULT 0 PRIMARY KEY,
      phone VARCHAR(10)
    );
    INSERT INTO a_tbl VALUES (1,'111-1111'), (2,'222-2222'), (3, '333-3333');
     
    -- creating a table without column definition
    CREATE TABLE new_tbl1 AS SELECT * FROM a_tbl;
    SELECT * FROM new_tbl1;
     
::

       id  phone
    ===================================
        1  '111-1111'
        2  '222-2222'
        3  '333-3333'
     
.. code-block:: sql

    -- all of column values are replicated from a_tbl
    CREATE TABLE new_tbl2 (
      id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
      phone VARCHAR
    ) AS SELECT * FROM a_tbl;
    
    SELECT * FROM new_tbl2;
     
::

       id  phone
    ===================================
        1  '111-1111'
        2  '222-2222'
        3  '333-3333'
     
.. code-block:: sql

    -- some of column values are replicated from a_tbl and the rest is NULL
    CREATE TABLE new_tbl3 (
      id INT, 
      name VARCHAR
    ) AS SELECT id, phone FROM a_tbl;
    
    SELECT * FROM new_tbl3
     
::

      name                           id  phone
    =========================================================
      NULL                            1  '111-1111'
      NULL                            2  '222-2222'
      NULL                            3  '333-3333'
     
.. code-block:: sql

    -- column alias in the select statement should be used in the column definition
    CREATE TABLE new_tbl4 (
      id1 INT, 
      id2 INT
    ) AS SELECT t1.id id1, t2.id id2 FROM new_tbl1 t1, new_tbl2 t2;
    
    SELECT * FROM new_tbl4;
     
::

      id1          id2
    ==========================
        1            1
        1            2
        1            3
        2            1
        2            2
        2            3
        3            1
        3            2
        3            3
     
.. code-block:: sql

    -- REPLACE is used on the UNIQUE column
    CREATE TABLE new_tbl5 (id1 int UNIQUE) REPLACE AS SELECT * FROM new_tbl4;
    
    SELECT * FROM new_tbl5;
     
::

      id1          id2
    ==========================
        1            3
        2            3
        3            3

You can create a table by selecting data from a remote table as below statement.

.. code-block:: sql

    -- column values are replicated from remote_tbl
    CREATE TABLE new_tbl_rem (
      id INT NON NULL;
      phone VARCHAR;
      localtion VARCHAR;
    ) AS SELECT id, phone, 'remote' FROM a_tbl@server1 WHERE id < 3;

    SELECT * FROM new_tbl_rem;

::

      id  phone       location
    ===================================
       1  '111-1111'  'remote'
       2  '222-2222'  'remote'

ALTER TABLE
===========

You can modify the structure of a table by using the **ALTER** statement. You can perform operations on the target table such as adding/deleting columns, creating/deleting indexes, and type casting existing columns as well as changing table names, column names and constraints. You can also change the initial value of **AUTO_INCREMENT**. **TABLE** and **CLASS** are used interchangeably **VIEW** and **VCLASS**, and **COLUMN** and **ATTRIBUTE** as well.

::

    ALTER [TABLE | CLASS] [schema_name.]table_name <alter_clause> [, <alter_clause>] ... ;
     
        <alter_clause> ::= 
            ADD <alter_add> [INHERIT <resolution>, ...]  | 
            ADD {KEY | INDEX} <index_name> <index_col_desc> |
            ALTER [COLUMN] column_name SET DEFAULT <value_specification> |
            DROP <alter_drop> [INHERIT <resolution>, ...] |
            DROP {KEY | INDEX} index_name |
            DROP FOREIGN KEY constraint_name |
            DROP PRIMARY KEY |                   
            RENAME <alter_rename> [INHERIT <resolution>, ...] |
            CHANGE <alter_change> |
            MODIFY <alter_modify> |            
            INHERIT <resolution>, ... |
            AUTO_INCREMENT = <initial_value> |
            COMMENT [=] 'table_comment_string' |
            COMMENT ON {COLUMN | CLASS ATTRIBUTE} <column_comment_definition> [, <column_comment_definition>] ;
                           
            <alter_add> ::= 
                [ATTRIBUTE|COLUMN] [(]<class_element>, ...[)] [FIRST|AFTER old_column_name] |
                CLASS ATTRIBUTE <column_definition>, ... |
                CONSTRAINT <constraint_name> <column_constraint> (column_name) |
                QUERY <select_statement> |
                SUPERCLASS [schema_name.]superclass_name, ...
                            
                <class_element> ::= <column_definition> | <table_constraint>
     
                <column_constraint> ::= UNIQUE [KEY] | PRIMARY KEY | FOREIGN KEY
     
            <alter_drop> ::= 
                [ATTRIBUTE | COLUMN]
                {
                    column_name, ... |
                    QUERY [<unsigned_integer_literal>] |
                    SUPERCLASS [schema_name.]superclass_name, ... |
                    CONSTRAINT constraint_name
                }
                             
            <alter_rename> ::= 
                [ATTRIBUTE | COLUMN]
                {
                    old_column_name AS new_column_name |
                    FUNCTION OF column_name AS function_name
                }
                
            <alter_change> ::= 
                [COLUMN | CLASS ATTRIBUTE] old_col_name new_col_name <column_definition>
                    [FIRST | AFTER col_name]

            <alter_modify> ::= 
                [COLUMN | CLASS ATTRIBUTE] col_name <column_definition>
                    [FIRST | AFTER col_name2]
                    
            <table_option> ::=
                CHANGE [COLUMN | CLASS ATTRIBUTE] old_col_name new_col_name <column_definition>
                    [FIRST | AFTER col_name2]
              | MODIFY [COLUMN | CLASS ATTRIBUTE] col_name <column_definition>
                    [FIRST | AFTER col_name2]

            <resolution> ::= column_name OF [schema_name.]superclass_name [AS alias]

            <index_col_name> ::= column_name [(length)] [ASC | DESC]

            <column_comment_definition> ::= column_name [=] 'column_comment_string'

.. note::

    A column's comment is specified in <*column_definition*> or <*column_comment_definition*>. For <*column_definition*>, see the above :ref:`CREATE TABLE syntax<column-definition>`.

.. warning::

    The table's name can be changed only by the table owner, **DBA** and **DBA** members. The other users must be granted to change the name by the owner or **DBA** (see :ref:`granting-authorization` For details on authorization).

ADD COLUMN Clause
-----------------

You can add a new column by using the **ADD COLUMN** clause. You can specify the location of the column to be added by using the **FIRST** or **AFTER** keyword.

::

    ALTER [TABLE | CLASS] [schema_name.]table_name
    ADD [COLUMN | ATTRIBUTE] [(] <column_definition> [FIRST | AFTER old_column_name] [)];

        <column_definition> ::= 
            column_name <data_type> [[<default_or_shared_or_ai>] | [<on_update>] | [<column_constraint>]] [COMMENT 'comment_string']
        
            <data_type> ::= <column_type> [<charset_modifier_clause>] [<collation_modifier_clause>]

                <charset_modifier_clause> ::= {CHARACTER_SET|CHARSET} {<char_string_literal>|<identifier>}

                <collation_modifier_clause> ::= COLLATE {<char_string_literal>|<identifier>}
            
            <default_or_shared_or_ai> ::=
                SHARED <value_specification> | 
                DEFAULT <value_specification>  |
                AUTO_INCREMENT [(seed, increment)]

            <on_update> ::= [ON UPDATE <value_specification>]

            <column_constraint> ::= [CONSTRAINT constraint_name] {NOT NULL | UNIQUE | PRIMARY KEY | [FOREIGN KEY] [WITH <index_with_option>] <referential_definition>}

                <referential_definition> ::=
                    REFERENCES [schema_name.]referenced_table_name (column_name, ...) [<referential_triggered_action> ...]
         
                    <referential_triggered_action> ::=
                        ON UPDATE <referential_action> |
                        ON DELETE <referential_action> 

                        <referential_action> ::= CASCADE | RESTRICT | NO ACTION | SET NULL
                        
                <index_with_option> ::= {DEDUPLICATE ‘=‘ deduplicate_level}                        

*   *schema_name*: specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name*: specifies the name of a table that has a column to be added.
*   <*column_definition*>: specifies the name(max 254 bytes), data type, and constraints of a column to be added.
*   **AFTER** *oid_column_name*: specifies the name of an existing column before the column to be added.
*   *comment_string*: specifies a column's comment.
*   *deduplicate_level*: Specifies the deduplicate level (0 to 14). For details, see :ref:`deduplicate_overview`.

.. code-block:: sql

    CREATE TABLE a_tbl;
    ALTER TABLE a_tbl ADD COLUMN age INT DEFAULT 0 NOT NULL COMMENT 'age comment';
    ALTER TABLE a_tbl ADD COLUMN name VARCHAR FIRST;
    ALTER TABLE a_tbl ADD COLUMN id INT NOT NULL AUTO_INCREMENT UNIQUE FIRST;
    INSERT INTO a_tbl(age) VALUES(20),(30),(40);

    ALTER TABLE a_tbl ADD COLUMN phone VARCHAR(13) DEFAULT '000-0000-0000' AFTER name;
    ALTER TABLE a_tbl ADD COLUMN birthday VARCHAR(20) DEFAULT TO_CHAR(SYSDATE,'YYYY-MM-DD');
     
    SELECT * FROM a_tbl;
     
::

           id  name                  phone                         age  birthday
    ============================================================================================
            1  NULL                  '000-0000-0000'                20  '2017-05-24'
            2  NULL                  '000-0000-0000'                30  '2017-05-24'
            3  NULL                  '000-0000-0000'                40  '2017-05-24'
     
    --adding multiple columns
    ALTER TABLE a_tbl ADD COLUMN (age1 int, age2 int, age3 int);

The result when you add a new column depends on what constraints are added.

*   If there is a **DEFAULT** constraint on the newly added column, **DEFAULT** value is inserted.
*   If there is no **DEFAULT** constraint and there is a **NOT NULL** constraint, hard default value is inserted when a value of system parameter **add_column_update_hard_default** is **yes**; however, it returns an error when a value of **add_column_update_hard_default** is **no**. 
 
The default of add_column_update_hard_default is **no**.
 
Depending on **DEFAULT** constraint and **add_column_update_hard_default**\ 's value, if they do not violate their constraints, it is possible to add **PRIMARY KEY** constraint or **UNIQUE** constraint.
 
*   If the newly added column when there is no data on the table, or the newly added column with **NOT NULL** and **UNIQUE** data can have **PRIMARY KEY** constraint.
*   If you try to add a new column with **PRIMARY KEY** constraint when there is data on the table, it returns an error.
 
    .. code-block:: sql
    
        CREATE TABLE tbl (a INT);
        INSERT INTO tbl VALUES (1), (2);
        ALTER TABLE tbl ADD COLUMN (b int PRIMARY KEY);
 
    ::
    
        ERROR: NOT NULL constraints do not allow NULL value.
 
*   If there is data and **UNIQUE** constraint is specified on the newly added data, **NULL** is inserted when there is no **DEFAULT** constraint.
 
    .. code-block:: sql
 
        ALTER TABLE tbl ADD COLUMN (b int UNIQUE);
        SELECT * FROM tbl;
 
    ::
    
            a            b
        ==================
            1         NULL
            2         NULL
 
*   If there is data on the table and **UNIQUE** constraint is specified on the newly added column, unique violation error is returned when there is **DEFAULT** constraint.
 
    .. code-block:: sql
    
        ALTER TABLE tbl ADD COLUMN (c int UNIQUE DEFAULT 10);
        
    ::
    
        ERROR: Operation would have caused one or more unique constraint violations.
 
*   If there is data on the table and **UNIQUE** constraint is specified on the newly added column, unique violation error is returned when there is **NOT NULL** constraint and the value of system parameter  add_column_update_hard_default is yes.
 
    .. code-block:: sql
 
        SET SYSTEM PARAMETERS 'add_column_update_hard_default=yes';
        ALTER TABLE tbl ADD COLUMN (c int UNIQUE NOT NULL);
 
    ::
    
        ERROR: Operation would have caused one or more unique constraint violations.
        
For **add_column_update_hard_default** and the hard default, see :ref:`change-column`. 

ADD CONSTRAINT Clause
---------------------

You can add a new constraint by using the **ADD CONSTRAINT** clause.

By default, the index created when you add **PRIMARY KEY** constraints is created in ascending order, and you can define the key sorting order by specifying the **ASC** or **DESC** keyword next to the column name. ::

    ALTER [ TABLE | CLASS | VCLASS | VIEW ] [schema_name.]table_name
    ADD <table_constraint> ;
    
        <table_constraint> ::=             
            { 
                {KEY|INDEX} index_name (column_name, ...) |
                [CONSTRAINT [constraint_name]]
                   {
                      UNIQUE [KEY|INDEX](column_name, ...) |
                      PRIMARY KEY (column_name, ...) |
                      <referential_constraint>
                   }
            }
     
            <referential_constraint> ::= FOREIGN KEY [foreign_key_name](column_name, ...) <referential_definition>
         
                <referential_definition> ::=
                    REFERENCES [schema_name.]referenced_table_name (column_name, ...) [<referential_triggered_action> ...]
         
                    <referential_triggered_action> ::=
                        ON UPDATE <referential_action> |
                        ON DELETE <referential_action> 

                        <referential_action> ::= CASCADE | RESTRICT | NO ACTION | SET NULL

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name*: Specifies the name of a table that has a constraint to be added.
*   *constraint_name*: Specifies the name of a constraint to be added, or it can be omitted. If omitted, a name is automatically assigned(maximum: 254 bytes).
*   *foreign_key_name*: Specifies a name of the **FOREIGN KEY** constraint. You can skip the name specification. However, if you specify this value, *constraint_name* will be ignored, and the specified value will be used.
*   <*table_constraint*>: Defines a constraint for the specified table. For details, see :ref:`constraint-definition`.

.. code-block:: sql

    ALTER TABLE a_tbl ADD CONSTRAINT pk_a_tbl_id PRIMARY KEY(id); 
    ALTER TABLE a_tbl DROP CONSTRAINT pk_a_tbl_id;
    ALTER TABLE a_tbl ADD CONSTRAINT pk_a_tbl_id PRIMARY KEY(id, name DESC);
    ALTER TABLE a_tbl ADD CONSTRAINT u_key1 UNIQUE (id);

ADD INDEX Clause
----------------

You can define the index attributes for a specific column by using the **ADD INDEX** clause. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name ADD {KEY | INDEX} index_name (<index_col_name>) ;
     
        <index_col_name> ::= column_name [(length)] [ ASC | DESC ]

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of a table to be modified.
*   *index_name* : Specifies the name of an index(maximum: 254 bytes). If omitted, a name is automatically assigned.
*   *index_col_name* : Specifies the column that has an index to be defined. **ASC** or **DESC** can be specified for a column option.

.. code-block:: sql

    ALTER TABLE a_tbl ADD INDEX i1(age ASC), ADD INDEX i2(phone DESC);
    
::

    csql> ;schema a_tbl
     
    === <Help: Schema of a Class> ===
     
     <Class Name>
     
         a_tbl
     
    <Attributes>
     
         name                 CHARACTER VARYING(1073741823) DEFAULT ''
         phone                CHARACTER VARYING(13) DEFAULT '111-1111'
         age                  INTEGER
         id                   INTEGER AUTO_INCREMENT  NOT NULL
     
     <Constraints>
     
         UNIQUE u_a_tbl_id ON a_tbl (id)
         INDEX i1 ON a_tbl (age)
         INDEX i2 ON a_tbl (phone DESC)

The below is an example to include an index's comment when you add an index with ALTER statement.

.. code-block:: sql

    ALTER TABLE tbl ADD index i_t_c (c) COMMENT 'index comment c';

ALTER COLUMN ... SET DEFAULT Clause
-----------------------------------

You can specify a new default value for a column that has no default value or modify the existing default value by using the **ALTER COLUMN** ... **SET DEFAULT**. You can use the **CHANGE** clause to change the default value of multiple columns with a single statement. For details, see the :ref:`change-column`. 

::

    ALTER [TABLE | CLASS] [schema_name.]table_name ALTER [COLUMN] column_name SET DEFAULT value ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of a table that has a column whose default value is to be modified.
*   *column_name* : Specifies the name of a column whose default value is to be modified.
*   *value* : Specifies a new default value.

::

    csql> ;schema a_tbl
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         a_tbl
     
     <Attributes>
     
         name                 CHARACTER VARYING(1073741823)
         phone                CHARACTER VARYING(13) DEFAULT '000-0000-0000'
         age                  INTEGER
         id                   INTEGER AUTO_INCREMENT  NOT NULL
     
     <Constraints>
     
         UNIQUE u_a_tbl_id ON a_tbl (id)
     
     
.. code-block:: sql
     
    ALTER TABLE a_tbl ALTER COLUMN name SET DEFAULT '';
    ALTER TABLE a_tbl ALTER COLUMN phone SET DEFAULT '111-1111';
     
::

    csql> ;schema a_tbl
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         a_tbl
     
     <Attributes>
     
         name                 CHARACTER VARYING(1073741823) DEFAULT ''
         phone                CHARACTER VARYING(13) DEFAULT '111-1111'
         age                  INTEGER
         id                   INTEGER AUTO_INCREMENT  NOT NULL
     
     <Constraints>
     
         UNIQUE u_a_tbl_id ON a_tbl (id)

.. code-block:: sql         

    CREATE TABLE t1(id1 VARCHAR(20), id2 VARCHAR(20) DEFAULT '');
    ALTER TABLE t1 ALTER COLUMN id1 SET DEFAULT TO_CHAR(SYS_DATETIME, 'yyyy/mm/dd hh:mi:ss');

::

    csql> ;schema t1

    === <Help: Schema of a Class> ===


    <Class Name>

         t1

    <Attributes>

         id1                  CHARACTER VARYING(20) DEFAULT TO_CHAR(SYS_DATETIME, 'yyyy/mm/dd hh:mi:ss')
         id2                  CHARACTER VARYING(20) DEFAULT ''
         
.. _alter-auto-increment:

AUTO_INCREMENT Clause
---------------------

The **AUTO_INCREMENT** clause can change the initial value of the increment value that is currently defined. However, there should be only one **AUTO_INCREMENT** column defined. ::

    ALTER TABLE [schema_name.]table_name AUTO_INCREMENT = initial_value ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Table name
*   *initial_value* : Initial value to alter

.. code-block:: sql

    CREATE TABLE t (i int AUTO_INCREMENT);
    ALTER TABLE t AUTO_INCREMENT = 5;
     
    CREATE TABLE t (i int AUTO_INCREMENT, j int AUTO_INCREMENT);
    
    -- when 2 AUTO_INCREMENT constraints are defined on one table, below query returns an error.
    ALTER TABLE t AUTO_INCREMENT = 5;

::
    
    ERROR: To avoid ambiguity, the AUTO_INCREMENT table option requires the table to have exactly one AUTO_INCREMENT column and no seed/increment specification.

.. warning:: You must be careful not to violate constraints (such as a **PRIMARY KEY** or **UNIQUE**) due to changing the initial value of **AUTO_INCREMENT**.

.. note:: If you change the type of **AUTO_INCREMENT** column, the maximum value is changed, too. For example, if you change INT to BIGINT, the maximum value of **AUTO_INCREMENT** is changed from the maximum INT into the maximum BIGINT.

.. _change-column:

CHANGE/MODIFY Clauses
---------------------

The **CHANGE** clause changes column name, type, size, and attribute. If the existing column name and a new column name are the same, types, size, and attribute will be changed.

The **MODIFY** clause can modify type, size, and attribute of a column but cannot change its name.

If you set the type, size, and attribute to apply to a new column with the **CHANGE** clause or the **MODIFY** clause, the attribute that is currently defined will not be passed to the attribute of the new column.

When you change data types using the **CHANGE** clause or the **MODIFY** clause, the data can be modified. For example, if you shorten the length of a column, the character string may be truncated if the value of configuration parameter alter_table_change_type_strict is set to **no**. But if the parameter value is set to **yes**, the change or modify is not allowed and it returns an error.
the configuration parameter allow_truncated_string also affect the similar as alter_table_change_type_strict.

When changing the type of the column specified AUTO_INCREMENT, it cannot be changed to a column type that is not allowed to used with the AUTO_INCREMENT. For example, if you try to change the column type of "a" from INT to VARCHAR with the ALTER statement as shown below, an error occurs.

.. code-block:: sql

    CREATE a_tbl (a int AUTO_INCREMENT, b VARCHAR);
    ALTER TABLE a_tbl MODIFY COLUMN a VARCHAR;

::

    ERROR: before '  varchar; '
    The domain of the attribute 'a' having an auto increment constraint is invalid.

When changing the type of a column specified  a default value, if the default value can't be coerced to the changed type, an error occurs as shown in the example below.

.. code-block:: sql

     CREATE TABLE t_def (a bigint default 123456789012, b varchar(20));
     ALTER TABLE t_def a a int;

::

     ERROR: A domain conflict exists on attribute "a".

.. warning::

    *   **ALTER TABLE** *[schema_name.]table_name* **CHANGE** *column_name* **DEFAULT** *default_value* syntax supported in CUBRID 2008 R3.1 or earlier version is no longer supported.
    *   When converting a number type to character type, if alter_table_change_type_strict=no and the length of the string is shorter than that of the number, the string is truncated and saved according to the length of the converted character type. If alter_table_change_type_strict=yes, it returns an error.
    *   If the column attributes like a type, a collation, etc. are changed, the changed attributes are not applied into the view created with the table before the change. Therefore, if you change the attributes of a table, it is recommended to recreate the related views.

::

    ALTER [/*+ SKIP_UPDATE_NULL */] TABLE [schema_name.]tbl_name <table_options> ;
     
        <table_options> ::=
            <table_option>[, <table_option>, ...]
     
            <table_option> ::=
                CHANGE [COLUMN | CLASS ATTRIBUTE] old_col_name new_col_name <column_definition>
                         [FIRST | AFTER col_name]
              | MODIFY [COLUMN | CLASS ATTRIBUTE] col_name <column_definition>
                         [FIRST | AFTER col_name]

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *tbl_name*: specifies the name of the table including the column to change.
*   *old_col_name*: specifies the existing column name.
*   *new_col_name*: specifies the column name to change
*   <*column_definition*>: specifies the type, the size, the attribute, and the comment of the column to be changed.
*   *col_name*: specifies the location where the column to change exists.
*   **SKIP_UPDATE_NULL**: If this hint is added, CUBRID does not check the previous NULLs even if NOT NULL constraint is added. See :ref:`SKIP_UPDATE_NULL <skip-update-null>`.

.. code-block:: sql

    CREATE TABLE t1 (a INTEGER);
     
    -- changing column a's name into a1
    ALTER TABLE t1 CHANGE a a1 INTEGER;
     
    -- changing column a1's constraint
    ALTER TABLE t1 CHANGE a1 a1 INTEGER NOT NULL;
    ---- or
    ALTER TABLE t1 MODIFY a1 INTEGER NOT NULL;
     
    -- changing column col1's type - "DEFAULT 1" constraint is removed.
    CREATE TABLE t1 (col1 INT DEFAULT 1);
    ALTER TABLE t1 MODIFY col1 BIGINT;
     
    -- changing column col1's type - "DEFAULT 1" constraint is kept.
    CREATE TABLE t1 (col1 INT DEFAULT 1, b VARCHAR(10));
    ALTER TABLE t1 MODIFY col1 BIGINT DEFAULT 1;
     
    -- changing column b's size
    ALTER TABLE t1 MODIFY b VARCHAR(20);

    -- changing the name and position of a column  
    CREATE TABLE t1 (i1 INT, i2 INT);  
    INSERT INTO t1 VALUES (1,11), (2,22), (3,33);
    
    SELECT * FROM t1 ORDER BY 1;
    
::

                i1           i2
    ==========================
                 1           11
                 2           22
                 3           33
     
.. code-block:: sql

    ALTER TABLE t1 CHANGE i2 i0 INTEGER FIRST;  
    SELECT * FROM t1 ORDER BY 1;
    
::

                i0           i1
    ==========================
                11            1
                22            2
                33            3

.. code-block:: sql

    ALTER TABLE t1 MODIFY i1 VARCHAR (200) DEFAULT TO_CHAR (SYS_DATE);
    INSERT INTO t1(i0) VALUES (17);
    SELECT * FROM t1 ORDER BY 1;
    
::

               i0  i1
    ===================================
               11  '1'
               17  '05/24/2017'
               22  '2'
               33  '3'

.. code-block:: sql

    -- adding NOT NULL constraint (strict)
    SET SYSTEM PARAMETERS 'alter_table_change_type_strict=yes';
     
    CREATE TABLE t1 (i INT);
    INSERT INTO t1 VALUES (11), (NULL), (22);
     
    ALTER TABLE t1 CHANGE i i1 INTEGER NOT NULL;
     
::
     
    ERROR: Cannot add NOT NULL constraint for attribute "i1": there are existing NULL values for this attribute.

.. code-block:: sql

    -- adding NOT NULL constraint
    SET SYSTEM PARAMETERS 'alter_table_change_type_strict=no';
     
    CREATE TABLE t1 (i INT);
    INSERT INTO t1 VALUES (11), (NULL), (22);
     
    ALTER TABLE t1 CHANGE i i1 INTEGER NOT NULL;
     
    SELECT * FROM t1;
     
::

               i1
    =============
               22
                0
               11

.. code-block:: sql

    -- change the column's data type (no errors)
     
    CREATE TABLE t1 (i1 INT);
    INSERT INTO t1 VALUES (1), (-2147483648), (2147483647);
     
    ALTER TABLE t1 CHANGE i1 s1 CHAR(11);
    SELECT * FROM t1;
     
::

      s1
    ======================
      '2147483647 '
      '-2147483648'
      '1          '

.. code-block:: sql

    -- change the column's data type (errors), strict mode
    SET SYSTEM PARAMETERS 'alter_table_change_type_strict=yes';
     
    CREATE TABLE t1 (i1 INT);
    INSERT INTO t1 VALUES (1), (-2147483648), (2147483647);
     
    ALTER TABLE t1 CHANGE i1 s1 CHAR(4);

::

    ERROR: ALTER TABLE .. CHANGE : changing to new domain : cast failed, current configuration doesn't allow truncation or overflow.

.. code-block:: sql

    -- change the column's data type (errors)
    SET SYSTEM PARAMETERS 'alter_table_change_type_strict=no';
     
    CREATE TABLE t1 (i1 INT);
    INSERT INTO t1 VALUES (1), (-2147483648), (2147483647);
     
    ALTER TABLE t1 CHANGE i1 s1 CHAR(4);
    SELECT * FROM t1;
     
::

    -- hard default values have been placed instead of signaling overflow

      s1
    ======================
      '1   '
      '-214'
      '2147'

.. _skip-update-null:

.. note:: 
  
    When you change NULL constraint into NOT NULL, it takes a long time by the time updating values into **hard default**; to resolve this problem, CUBRID can skip updating values which already exists by using **SKIP_UPDATE_NULL**. However, you should consider that NULL values which do not match with the NOT NULL constraints, can exists.
  
    .. code-block:: sql 
  
        ALTER /*+ SKIP_UPDATE_NULL */ TABLE foo MODIFY col INT NOT NULL; 

Changes of Table Attributes based on Changes of Column Type
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*   Type Change : If the value of the system parameter **alter_table_change_type_strict** is set to no, then changing values to other types is allowed, but if it is set to yes then changing is not allowed. The default value of the parameter is **yes**. You can change values to all types allowed by the **CAST** operator. Changing object types is allowed only by the upper classes (tables) of the objects. Also, if the system parameter **allow_truncated_string** is **no** when changing to a string such as **char** or **varchar**, the overflowed string cannot be changed. The default value for the parameter is **no**.

*   **NOT NULL**

    *   Even though the **NOT NULL** constraint is not specified, it will not be removed from a new table if the constraint is present in the existing table. If you want to remove the constraint **NOT NULL**, specify the **NULL** in the statement.
    *   If the **NOT NULL** constraint is specified in the column to change, the result varies depending on the configuration of the system parameter, **alter_table_change_type_strict**.

        *   If **alter_table_change_type_strict** is set to yes, the column values will be checked. If **NULL** exists, an error will occur, and the change will not be executed.
        *   If the **alter_table_change_type_strict** is set to no, every existing **NULL** value will be changed to a hard default value of the type to change.

*   **DEFAULT** : Even though the **DEFAULT** attribute is not specified in the column to change, it will not be removed from a new table if the attribute is present in the existing table. If you want to remove the attribute **DEFAULT**, specify the **DEFAULT NULL** in the statement.

*   **COMMENT** : Even though the **COMMENT** attribute is not specified in the column to change, it will not be removed from a new table if the attribute is present in the existing table. If you want to remove the attribute **COMMENT**, specify the **COMMENT ''** in the statement.

*   **AUTO_INCREMENT** : Even though the **AUTO_INCREMENT** attribute is not specified in the column to change, it will not be removed from a new table if the attribute is present in the existing table.
        *   caution) The **AUTO_INCREMENT** attribute can not be removed from a new table once the attribute is set by CREATE or ALTER.

*   **ON UPDATE** : Even though the **ON UPDATE** attribute is not specified in the column to change, it will not be removed from a new table if the attribute is present in the existing table.
        *   caution) The **ON UPDATE** attribute can not be removed from a new table once the attribute is set by CREATE or ALTER.

*   **FOREIGN KEY** : You cannot change the column with the foreign key constraint that is referred to or refers to.

*   Single Column **PRIMARY KEY**

    *   If the **PRIMARY KEY** constraint is specified in the column to change, a **PRIMARY KEY** is re-created only in which a **PRIMARY KEY** constraint exists in the existing column and the type is upgraded.
    *   If the **PRIMARY KEY** constraint is specified in the column to change but doesn't exist in the existing column, a **PRIMARY KEY** will be created.
    *   If a **PRIMARY KEY** constraint exists but is not specified in the column to change, the **PRIMARY KEY** will be maintained.

*   Multicolumn **PRIMARY KEY**: If the **PRIMARY KEY** constraint is specified and the type is upgraded, a **PRIMARY KEY** will be re-created.

*   Single Column **UNIQUE KEY**

    *   If the type is upgraded, a **UNIQUE KEY** will be re-created.
    *   If a **UNIQUE KEY** exists in the existing column and it is not specified in the column to change, it will be maintained.
    *   If a **UNIQUE KEY** exists in the existing column to change, it will be created.

*   Multicolumn **UNIQUE KEY**: If the column type is changed, an index will be re-created.

*   Column with a Non-unique Index : If the column type is changed, an index will be re-created.

*   Partition Column: If a table is partitioned by a column, the column cannot be changed. Partitions cannot be added.

*   Column with a Class Hierarchy : You can only change the tables that do not have a lower class. You cannot change the lower class that inherits from an upper class. You cannot change the inherited attributes.

*   Trigger and View : You must redefine triggers and views directly because they are not changed according to the definition of the column to change.

*   Column Sequence : You can change the sequence of columns.

*   Name Change : You can change names as long as they do not conflict.

Changes of Values based on Changes of Column Type
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The **alter_table_change_type_strict** parameter determines whether the value conversion is allowed according to the type change. If the value is no, it can be changed when you change a column type or add a **NOT NULL** constraint. The default value is **yes**.

When the value of the parameter, **alter_table_change_type_strict** is no, it will operate depending on the conditions as follows:

*   Overflow occurred while converting numbers or character strings to Numbers: It is determined based on symbol of the result type. If it is negative value, it is specified as a minimum value or positive value, specified as the maximum value and a warning message for records where overflow occurred is recorded in the log. For strings, it will follow the rules stated above after it is converted to **DOUBLE** type. Overflow can also be returned by the parameter **allow_truncated_string** setting to **no** if the converted string does not fit the length of the target string type.

*   Character strings to convert to shorter ones: The record will be updated to the hard default value of the type that is defined and the warning message will be recorded in a log. Converting to shorter ones is not allowed when the **allow_truncated_string** is set to **no**.

*   Conversion failure due to other reasons: The record will be updated to the hard default value of the type that is defined and the warning message will be recorded in a log.

If the value of the **alter_table_change_type_strict** parameter is yes or **allow_truncated_string** is set to no, an error message will be displayed and the changes will be rolled back.

The **ALTER CHANGE** statement checks the possibility of type conversion before updating a record but the type conversion of specific values may fail. For example, if the value format is not correct when you convert **VARCHAR** to **DATE**, the conversion may fail. In this case, the hard default value of the **DATE** type will be assigned.

The hard default value is a value that will be used when you add columns with the **ALTER TABLE ... ADD COLUMN** statement, add or change by converting types with the **ALTER TABLE ... CHANGE/MODIFY** statement. The operation will vary depending on the system parameter, **add_column_update_hard_default** in the **ADD COLUMN** statement.

**Hard Default Value by Type**

+-----------+-------------------------------------+-----------------------------------------+
| Type      | Existence of Hard Default Value     | Hard Default Value                      |
+===========+=====================================+=========================================+
| INTEGER   | Yes                                 | 0                                       |
+-----------+-------------------------------------+-----------------------------------------+
| FLOAT     | Yes                                 | 0                                       |
+-----------+-------------------------------------+-----------------------------------------+
| DOUBLE    | Yes                                 | 0                                       |
+-----------+-------------------------------------+-----------------------------------------+
| SMALLINT  | Yes                                 | 0                                       |
+-----------+-------------------------------------+-----------------------------------------+
| DATE      | Yes                                 | date'01/01/0001'                        |
+-----------+-------------------------------------+-----------------------------------------+
| TIME      | Yes                                 | time'00:00'                             |
+-----------+-------------------------------------+-----------------------------------------+
| DATETIME  | Yes                                 | datetime'01/01/0001 00:00'              |
+-----------+-------------------------------------+-----------------------------------------+
| TIMESTAMP | Yes                                 | timestamp'00:00:01 AM 01/01/1970' (GMT) |
+-----------+-------------------------------------+-----------------------------------------+
| NUMERIC   | Yes                                 | 0                                       |
+-----------+-------------------------------------+-----------------------------------------+
| CHAR      | Yes                                 | ''                                      |
+-----------+-------------------------------------+-----------------------------------------+
| VARCHAR   | Yes                                 | ''                                      |
+-----------+-------------------------------------+-----------------------------------------+
| SET       | Yes                                 | {}                                      |
+-----------+-------------------------------------+-----------------------------------------+
| MULTISET  | Yes                                 | {}                                      |
+-----------+-------------------------------------+-----------------------------------------+
| SEQUENCE  | Yes                                 | {}                                      |
+-----------+-------------------------------------+-----------------------------------------+
| BIGINT    | Yes                                 | 0                                       |
+-----------+-------------------------------------+-----------------------------------------+
| BIT       | NO                                  |                                         |
+-----------+-------------------------------------+-----------------------------------------+
| VARBIT    | No                                  |                                         |
+-----------+-------------------------------------+-----------------------------------------+
| OBJECT    | No                                  |                                         |
+-----------+-------------------------------------+-----------------------------------------+
| BLOB      | No                                  |                                         |
+-----------+-------------------------------------+-----------------------------------------+
| CLOB      | No                                  |                                         |
+-----------+-------------------------------------+-----------------------------------------+

Column's COMMENT
----------------

A column's comment is specified in <*column_definition*> or <*column_comment_definition*>. <*column_definition*> is located at the end of ADD/MODIFY/CHANGE syntax and <*column_comment_definition*> is located at the end of COMMENT ON COLUMN syntax. To see the meaning of <*column_definition*>, refer to :ref:`CREATE TABLE syntax<column-definition>` on the above.

In the COMMENT ON COLUMN syntax, you can change column comments by specifying one or more columns.
The following example shows how to change a column comment using the COMMENT ON COLUMN statement.

.. code-block:: sql

    ALTER TABLE t1 COMMENT ON COLUMN c1 = 'changed table column c1 comment';
    ALTER TABLE t1 COMMENT ON COLUMN c2 = 'changed table column c2 comment', c3 = 'changed table column c3 comment';

The below is a syntax to show a column's comment.

.. code-block:: sql

    SHOW CREATE TABLE t1 /* table_name */ ;

    SELECT attr_name, class_name, comment 
    FROM db_attribute
    WHERE class_name = t1 /* lowercase_table_name */ ;

    SHOW FULL COLUMNS FROM t1 /* table_name */ ;

You can see this comment with the ";sc table_name" command in the CSQL interpreter.

::

    $ csql -u dba demodb
    
    csql> ;sc table_name

.. _rename-column:

RENAME COLUMN Clause
--------------------

You can change the name of the column by using the **RENAME COLUMN** clause. ::

    ALTER [TABLE | CLASS | VCLASS | VIEW] [schema_name.]table_name
    RENAME [COLUMN | ATTRIBUTE] old_column_name {AS | TO} new_column_name ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of a table that has a column to be renamed.
*   *old_column_name* : Specifies the name of a column.
*   *new_column_name* : Specifies a new column name after the **AS** keyword(maximum: 254 bytes).

.. code-block:: sql

    CREATE TABLE a_tbl (id INT, name VARCHAR(50));
    ALTER TABLE a_tbl RENAME COLUMN name AS name1;

DROP COLUMN Clause
------------------

You can delete a column in a table by using the **DROP COLUMN** clause. You can specify multiple columns to delete simultaneously by separating them with commas (,). ::

    ALTER [TABLE | CLASS | VCLASS | VIEW] [schema_name.]table_name
    DROP [COLUMN | ATTRIBUTE] column_name, ... ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of a table that has a column to be deleted.
*   *column_ name* : Specifies the name of a column to be deleted. Multiple columns can be specified by separating them with commas (,).

.. code-block:: sql

    ALTER TABLE a_tbl DROP COLUMN age1,age2,age3;

DROP CONSTRAINT Clause
----------------------

You can drop the constraints pre-defined for the table, such as **UNIQUE**, **PRIMARY KEY** and **FOREIGN KEY** by using the **DROP CONSTRAINT** clause. In this case, you must specify a constraint name. You can check these names by using the CSQL command (**;schema table_name**). ::

    ALTER [TABLE | CLASS] [schema_name.]table_name
    DROP CONSTRAINT constraint_name ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of a table that has a constraint to be dropped.
*   *constraint_name* : Specifies the name of a constraint to be dropped.

.. code-block:: sql

    CREATE TABLE a_tbl (
      id INT NOT NULL DEFAULT 0 PRIMARY KEY,
      phone VARCHAR(10)
    );
     
    CREATE TABLE b_tbl (
      ID INT NOT NULL,
      name VARCHAR (10) NOT NULL,
      CONSTRAINT u_name UNIQUE (name), 
      CONSTRAINT pk_id PRIMARY KEY (id),
      CONSTRAINT fk_id FOREIGN KEY (id) REFERENCES a_tbl (id)
      ON DELETE CASCADE ON UPDATE RESTRICT
    );
    
    ALTER TABLE b_tbl DROP CONSTRAINT pk_id;
    ALTER TABLE b_tbl DROP CONSTRAINT fk_id;
    ALTER TABLE b_tbl DROP CONSTRAINT u_name;

DROP INDEX Clause
-----------------


You can delete an index defined for a column by using the **DROP INDEX** clause. A unique index can be dropped with a **DROP CONSTRAINT** clause.

::

    ALTER [TABLE | CLASS] [schema_name.]table_name DROP INDEX index_name ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of a table of which constraints will be deleted.
*   *index_name* : Specifies the name of an index to be deleted.

.. code-block:: sql

    ALTER TABLE a_tbl DROP INDEX i_a_tbl_age;

DROP PRIMARY KEY Clause
-----------------------

You can delete a primary key constraint defined for a table by using the **DROP PRIMARY KEY** clause. You do have to specify the name of the primary key constraint because only one primary key can be defined by table. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name DROP PRIMARY KEY ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of a table that has a primary key constraint to be deleted.

.. code-block:: sql

    ALTER TABLE a_tbl DROP PRIMARY KEY;

DROP FOREIGN KEY Clause
-----------------------

You can drop a foreign key constraint defined for a table using the **DROP FOREIGN KEY** clause. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name DROP FOREIGN KEY constraint_name ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of a table whose constraint is to be deleted.
*   *constraint_name* : Specifies the name of foreign key constraint to be deleted.

.. code-block:: sql

    ALTER TABLE b_tbl ADD CONSTRAINT fk_id FOREIGN KEY (id) REFERENCES a_tbl (id);
    ALTER TABLE b_tbl DROP FOREIGN KEY fk_id;

DROP TABLE
==========

You can drop an existing table by the **DROP** statement. Multiple tables can be dropped with a single **DROP** statement. All rows of table are also dropped. If you also specify **IF EXISTS** clause, no error will be happened even if a target table does not exist. 

::

    DROP [TABLE | CLASS] [IF EXISTS] <table_specification_comma_list> [CASCADE CONSTRAINTS] ;

        <table_specification_comma_list> ::= 
            <single_table_spec> | (<table_specification_comma_list>) 

            <single_table_spec> ::= 
                | [ONLY] [schema_name.]table_name 
                | ALL [schema_name.]table_name [( EXCEPT [schema_name.]table_name, ... )] 

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of the table to be dropped. You can delete multiple tables simultaneously by separating them with commas.
*   If a super class name is specified after the **ONLY** keyword, only the super class, not the sub classes inheriting from it, is deleted. If a super class name is specified after the **ALL** keyword, the super classes as well as the sub classes inheriting from it are all deleted. You can specify the list of sub classes not to be deleted after the **EXCEPT** keyword.
*   If sub classes that inherit from the super class specified after the **ALL** keyword are specified after the **EXCEPT** keyword, they are not deleted.
*   Specifies the list of subclasses which are not to be deleted after the **EXCEPT** keyword.
*	**CASCADE CONSTRAINTS**: The table is dropped and also foreign keys of other tables which refer this table are dropped.

.. code-block:: sql

    CREATE TABLE b_tbl (i INT);
    CREATE TABLE a_tbl (i INT);
     
    -- DROP TABLE IF EXISTS
    DROP TABLE IF EXISTS b_tbl, a_tbl;
     
    SELECT * FROM a_tbl;
    
::

    ERROR: Unknown class "a_tbl".

*   If **CASCADE CONSTRAINTS** is specified, the specified table is dropped even if some tables refer the dropping table's primary key; foreign keys of other tables which refer this table are also dropped. However, the data of tables which are referred are not deleted.

The below shows to drop a_parent table which b_child table refers. A foreign key of b_child table also dropped, and the data of b_child table are kept.

.. code-block:: sql 

    CREATE TABLE a_parent ( 
        id INTEGER PRIMARY KEY, 
        name VARCHAR(10) 
    ); 
    CREATE TABLE b_child ( 
        id INTEGER PRIMARY KEY, 
        parent_id INTEGER, 
        CONSTRAINT fk_parent_id FOREIGN KEY(parent_id) REFERENCES a_parent(id) ON DELETE CASCADE ON UPDATE RESTRICT 
    ); 

    DROP TABLE a_parent CASCADE CONSTRAINTS;

RENAME TABLE
============

You can change the name of a table by using the **RENAME TABLE** statement and specify a list of the table name to change the names of multiple tables. ::

    RENAME  [TABLE | CLASS] [schema_name.]old_table_name {AS | TO} [schema_name.]new_table_name [{, [schema_name.]old_table_name {AS | TO} [schema_name.]new_table_name}];

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used. The schema of the table to be changed and the schema of the new table must be the same.
*   *old_table_name* : Specifies the old table name to be renamed.
*   *new_table_name* : Specifies a new table name(maximum: 254 bytes).

.. code-block:: sql

    RENAME TABLE a_tbl AS aa_tbl;
    RENAME TABLE aa_tbl TO a1_tbl, b_tbl TO b1_tbl;

.. note::

    The table name can be changed only by the table owner, **DBA** and **DBA** members. The other users must be granted to change the name by the owner or **DBA** (see :ref:`granting-authorization` for details about authorization).
