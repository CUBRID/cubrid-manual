****************
Table Definition
****************

CREATE TABLE
============

To create a table, use the **CREATE TABLE** statement. ::

    CREATE {TABLE | CLASS} <table_name>
                       [ <subclass_definition> ]
                       [ ( <column_definition> [,<table_constraint>]... ) ]
                       [ AUTO_INCREMENT = initial_value ] ]
                       [ CLASS ATTRIBUTE ( <column_definition_comma_list> ) ]
                       [ METHOD <method_definition_comma_list> ]
                       [ FILE <method_file_comma_list> ]
                       [ INHERIT <resolution_comma_list> ]
                       [ REUSE_OID ]
    <column_definition> ::=
    column_name column_type [[ <default_or_shared> ] | [ <column_constraint> ]]...
     
    <default_or_shared> ::=
    {SHARED <value_specification> | DEFAULT <value_specification> } |
    AUTO_INCREMENT [(seed, increment)]
     
    <column_constraint> ::=
    NOT NULL | UNIQUE | PRIMARY KEY | FOREIGN KEY <referential definition>
     
    <table_constraint> ::=
    [ CONSTRAINT [ <constraint_name> ] ] UNIQUE [ KEY | INDEX ]( column_name_comma_list ) |
    [ { KEY | INDEX } [ <constraint_name> ]( column_name_comma_list ) |
    [ PRIMARY KEY ( column_name_comma_list )] |
    [ <referential_constraint> ]
     
    <referential_constraint> ::=
    FOREIGN KEY [ <foreign_key_name> ]( column_name_comma_list ) <referential definition>
     
    <referential definition> ::=
    REFERENCES [ referenced_table_name ] ( column_name_comma_list )
    [ <referential_triggered_action> ... ]
     
    <referential_triggered_action> ::=
    { ON UPDATE <referential_action> } |
    { ON DELETE <referential_action> }
     
    <referential_action> ::=
    CASCADE | RESTRICT | NO ACTION | SET NULL
     
    <subclass_definition> ::=
    { UNDER | AS SUBCLASS OF } table_name_comma_list
     
    <method_definition> ::=
    [ CLASS ] method_name
    [ ( [ argument_type_comma_list ] ) ]
    [ result_type ]
    [ FUNCTION  function_name ]
     
    <resolution> ::=
    [ CLASS ] { column_name | method_name } OF superclass_name
    [ AS alias ]

*   *table_name* : Specifies the name of the table to be created (maximum: 254 bytes).
*   *column_name* : Specifies the name of the column to be created (maximum: 254 bytes).
*   *column_type* : Specifies the data type of the column.
*   [**SHARED** *value* | **DEFAULT** *value*] : Specifies the initial value of the column.
*   *column_constraints* : Specifies the constraint of the column. Available constraints are **NOT NULL**, **UNIQUE**, **PRIMARY KEY** and **FOREIGN KEY** (see :ref:`constraint-definition` For details).

.. code-block:: sql

    CREATE TABLE olympic (
       host_year        INT    NOT NULL PRIMARY KEY,
       host_nation      VARCHAR(40) NOT NULL,
       host_city        VARCHAR(20) NOT NULL,
       opening_date     DATE        NOT NULL,
       closing_date     DATE        NOT NULL,
       mascot           VARCHAR(20) ,
       slogan           VARCHAR(40) ,
       introduction     VARCHAR(1500)
    );

Column Definition
-----------------

A column is a set of data values of a particular simple type, one for each row of the table. ::

    <column_definition> ::=
    column_name column_type [ [ <default_or_shared> ] | [ <column_constraint> ] ]...
     
    <default_or_shared> ::=
    { SHARED <value_specification> | DEFAULT <value_specification> } |
    AUTO_INCREMENT [ (seed, increment) ]
     
    <column_constraint> ::=
    NOT NULL | UNIQUE | PRIMARY KEY | FOREIGN KEY <referential definition>

**Column Name**

    How to create a column name, see :doc:`/sql/identifier`. You can alter created column name by using the **RENAME COLUMN** clause of the **ALTER TABLE** statement (see :ref:`rename-column`).

    The following example shows how to create the *manager2* table that has the following two columns: *full_name* and *age*.

    .. code-block:: sql

        CREATE TABLE manager2 (full_name VARCHAR(40), age INT );

    .. warning::

        *   The first character of a column name must be an alphabet.
        *   The column name must be unique in the table.

**Setting the Column Initial Value (SHARED, DEFAULT)**

    **SHARED** and **DEFAULT** are attributes related to the initial value of the column. You can change the value of **SHARED** and **DEFAULT** in the **ALTER TABLE** statement.

    *   **SHARED** : Column values are identical in all rows. If a value different from the initial value is **INSERT** ed, the column value is updated to a new one in every row.
    *   **DEFAULT** : The initial value set when the **DEFAULT** attribute was defined is stored even if the column value is not specified when a new row is inserted.

    The pseudocolumn (a special function which has no element) allows for the **DEFAULT** value as follows.

    +-------------------+---------------+
    | DEFAULT Value     | Data Type     |
    +===================+===============+
    | SYS_TIMESTAMP     | TIMESTAMP     |
    +-------------------+---------------+
    | SYS_DATETIME      | DATETIME      |
    +-------------------+---------------+
    | SYS_DATE          | DATE          |
    +-------------------+---------------+
    | SYS_TIME          | TIME          |
    +-------------------+---------------+
    | USER, USER()      | STRING        |
    +-------------------+---------------+

    .. note::

        In version lower than CUBRID 9.0, the value at the time of **CREATE TABLE** has been saved when the **DATE** value of the **DATE**, **DATETIME**, **TIME**, **TIMESTAMP** column has been specified to **SYS_DATE**, **SYS_DATETIME**, **SYS_TIME**, **SYS_TIMESTAMP** while creating a table. Therefore, to enter the value at the time of data **INSERT** in version lower than CUBRID 9.0, the function should be entered to the **VALUES** clause of the **INSERT** syntax.

    .. code-block:: sql

        CREATE TABLE colval_tbl
        ( id INT, name VARCHAR SHARED 'AAA', phone VARCHAR DEFAULT '000-0000');
        INSERT INTO colval_tbl(id) VALUES (1),(2);
        SELECT * FROM colval_tbl;
         
                   id  name                  phone
        =========================================================
                    1  'AAA'                 '000-0000'
                    2  'AAA'                 '000-0000'
         
        --updating column values on every row
        INSERT INTO colval_tbl(id, name) VALUES (3,'BBB');
        INSERT INTO colval_tbl(id) VALUES (4),(5);
        SELECT * FROM colval_tbl;
         
                   id  name                  phone
        =========================================================
                    1  'BBB'                 '000-0000'
                    2  'BBB'                 '000-0000'
                    3  'BBB'                 '000-0000'
                    4  'BBB'                 '000-0000'
                    5  'BBB'                 '000-0000'
         
        --changing DEFAULT value in the ALTER TABLE statement
        ALTER TABLE colval_tbl CHANGE phone DEFAULT '111-1111'
        INSERT INTO colval_tbl(id) VALUES (6);
        SELECT * FROM colval_tbl;
         
                   id  name                  phone
        =========================================================
                    1  'BBB'                 '000-0000'
                    2  'BBB'                 '000-0000'
                    3  'BBB'                 '000-0000'
                    4  'BBB'                 '000-0000'
                    5  'BBB'                 '000-0000'
                    6  'BBB'                 '111-1111'

    The **DEFAULT** value of the pseudocolumn can be specified to one or more columns.

    .. code-block:: sql

        CREATE TABLE t (date1 DATE DEFAULT SYSDATE, date2 DATE DEFAULT SYSDATE);
        CREATE TABLE t (date1 DATE DEFAULT SYSDATE,
                        ts1   TIMESTAMP DEFAULT CURRENT_TIMESTAMP);

**AUTO INCREMENT**

    You can define the **AUTO_INCREMENT** attribute for the column to automatically give serial numbers to column values. This can be defined only for **SMALLINT**, **INTEGER**, **BIGINT**, and **NUMERIC** (*p*, 0) types.

    **DEFAULT**, **SHARED**, and **AUTO_INCREMENT** cannot be defined for the same column. Make sure the value entered directly by the user and the value entered by the auto increment attribute do not conflict with each other.

    You can change the initial value of **AUTO_INCREMENT** by using the **ALTER TABLE** statement. For details, see :ref:`alter-auto-increment` of **ALTER TABLE**. 
    
    .. code-block:: sql
    
        CREATE TABLE table_name (id int AUTO_INCREMENT[(seed, increment)]);
        
        CREATE TABLE table_name (id int AUTO_INCREMENT) AUTO_INCREMENT = seed;


    *   *seed* : The initial value from which the number starts. All integers (positive, negative, and zero) are allowed. The default value is **1**.
    *   *increment* : The increment value of each row. Only positive integers are allowed. The default value is **1**.

    When you use the **CREATE TABLE** *table_name* (id int **AUTO_INCREMENT**) **AUTO_INCREMENT** = *seed*; statement, the constraints are as follows:

    *   You should define only one column with the **AUTO_INCREMENT** attribute.
    *   Don't use (*seed*, *increment*) and AUTO_INCREMENT = *seed* together.

    .. code-block:: sql

        CREATE TABLE auto_tbl(id INT AUTO_INCREMENT, name VARCHAR);
        INSERT INTO auto_tbl VALUES(NULL, 'AAA'),(NULL, 'BBB'),(NULL, 'CCC');
        INSERT INTO auto_tbl(name) VALUES ('DDD'),('EEE');
        SELECT * FROM auto_tbl;
         
                   id  name
        ===================================
                    1  'AAA'
                    2  'BBB'
                    3  'CCC'
                    4  'DDD'
                    5  'EEE'
         
        CREATE TABLE tbl (id int AUTO_INCREMENT, val string) AUTO_INCREMENT = 3;
        INSERT INTO tbl VALUES (NULL,'cubrid');
         
        SELECT * FROM tbl;
        
                   id  val
        ===================================
                    3  'cubrid'
         
        CREATE TABLE t (id int AUTO_INCREMENT, id2 int AUTO_INCREMENT) AUTO_INCREMENT = 5;
        ERROR: To avoid ambiguity, the AUTO_INCREMENT table option requires the table to  have exactly one AUTO_INCREMENT column and no seed/increment specification.
         
        CREATE TABLE t (i int AUTO_INCREMENT(100, 2)) AUTO_INCREMENT = 3;
        ERROR: To avoid ambiguity, the AUTO_INCREMENT table option requires the table to  have exactly one AUTO_INCREMENT column and no seed/increment specification.

    .. note::

        *   Even if a column has auto increment, the **UNIQUE** constraint is not satisfied.
        *   If **NULL** is specified in the column where auto increment is defined, the value of auto increment is stored.
        *   **SHARED** or **DEFAULT** attribute cannot be specified in the column in which AUTO_INCREMENT is defined.
        *   The initial value and the final value obtained by auto increment cannot exceed the minimum and maximum values allowed in the given type.
        *   Because auto increment has no cycle, an error occurs when the maximum value of the type exceeds, and no rollback is executed. Therefore, you must delete and recreate the column in such cases.

        For example, if a table is created as below, the maximum value of A is 32767. Because an error occurs if the value exceeds 32767, you must make sure that the maximum value of the column A does not exceed the maximum value of the type when creating the initial table.

            .. code-block:: sql
              
                CREATE TABLE tb1(A SMALLINT AUTO_INCREMENT, B CHAR(5));

.. _constraint-definition:

Constraint Definition
---------------------

You can define **NOT NULL**, **UNIQUE**, **PRIMARY KEY**, **FOREIGN KEY** as the constraints. You can also create an index by using **INDEX** or **KEY**. 

::

    <column_constraint> ::=
    NOT NULL | UNIQUE | PRIMARY KEY | FOREIGN KEY <referential definition>
     
    <table_constraint> ::=
    [ CONSTRAINT [ <constraint_name> ] ] UNIQUE [ KEY | INDEX ]( column_name_comma_list ) |
    [ { KEY | INDEX } <constraint_name> ( column_name_comma_list ) |
    [ PRIMARY KEY ( column_name_comma_list )] |
    [ <referential_constraint> ]
     
    <referential_constraint> ::=
    FOREIGN KEY ( column_name_comma_list ) <referential definition>
     
    <referential definition> ::=
    REFERENCES [ referenced_table_name ] ( column_name_comma_list )
    [ <referential_triggered_action> ... ]
     
    <referential_triggered_action> ::=
    { ON UPDATE <referential_action> } |
    { ON DELETE <referential_action> }
     
    <referential_action> ::=
    CASCADE | RESTRICT | NO ACTION  | SET NULL

**NOT NULL Constraint**

    A column for which the **NOT NULL** constraint has been defined must have a certain value that is not **NULL**. The **NOT NULL** constraint can be defined for all columns. An error occurs if you try to insert a **NULL** value into a column with the **NOT NULL** constraint by using the **INSERT** or **UPDATE** statement.

    In the following example, if you input NULL value on the *id* column, it occurs an error because *id* column cannot have NULL value.

    .. code-block:: sql

        CREATE TABLE const_tbl1(id INT NOT NULL, INDEX i_index(id ASC), phone VARCHAR);
         
        CREATE TABLE const_tbl2(id INT NOT NULL PRIMARY KEY, phone VARCHAR);
        INSERT INTO const_tbl2 (NULL,'000-0000');
         
        In line 2, column 25,
         
        ERROR: syntax error, unexpected Null

**UNIQUE Constraint**

    The **UNIQUE** constraint enforces a column to have a unique value. An error occurs if a new record that has the same value as the existing one is added by this constraint.

    You can place a **UNIQUE** constraint on either a column or a set of columns. If the **UNIQUE** constraint is defined for multiple columns, the uniqueness is ensured not for each column, but the combination of multiple columns.

    In the following example, the second INSERT statement fails because the value of *id* column is the same as 1 with the value of *id* column in the fist INSERT statement.

    .. code-block:: sql

        --UNIQUE constraint is defined on a single column only
        CREATE TABLE const_tbl5(id INT UNIQUE, phone VARCHAR);
        INSERT INTO const_tbl5(id) VALUES (NULL), (NULL);
        INSERT INTO const_tbl5 VALUES (1, '000-0000');
        SELECT * FROM const_tbl5;
         
                   id  phone
        ===================================
                 NULL  NULL
                 NULL  NULL
                    1  '000-0000'
         
        INSERT INTO const_tbl5 VALUES (1, '111-1111');
         
        ERROR: Operation would have caused one or more unique constraint violations.
     
    In the following example, if a **UNIQUE** constraint is defined on several columns, this ensures the uniqueness of the values in all the columns.

    .. code-block:: sql

        CREATE TABLE const_tbl6(id INT, phone VARCHAR, CONSTRAINT UNIQUE(id,phone));
        INSERT INTO const_tbl6 VALUES (1,NULL), (2,NULL), (1,'000-0000'), (1,'111-1111');
        SELECT * FROM const_tbl6;
         
                   id  phone
        ===================================
                    1  NULL
                    2  NULL
                    1  '000-0000'
                    1  '111-1111'

**PRIMARY KEY Constraint**

    A key in a table is a set of column(s) that uniquely identifies each row. A candidate key is a set of columns that uniquely identifies each row of the table. You can define one of such candidate keys a primary key. That is, the column defined as a primary key is uniquely identified in each row.

    By default, the index created by defining the primary key is created in ascending order, and you can define the order by specifying **ASC** or **DESC** keyword next to the column. 
    
    .. code-block:: sql

        CREATE TABLE pk_tbl (a INT, b INT, PRIMARY KEY (a, b DESC));

    .. code-block:: sql

        CREATE TABLE const_tbl7(
        id INT NOT NULL,
        phone VARCHAR,
        CONSTRAINT pk_id PRIMARY KEY(id));
         
        --CONSTRAINT keyword
        CREATE TABLE const_tbl8(
        id INT NOT NULL PRIMARY KEY,
        phone VARCHAR);
         
        --primary key is defined on multiple columns
        CREATE TABLE const_tbl8 (
        host_year    INT NOT NULL,
        event_code   INT NOT NULL,
        athlete_code INT NOT NULL,
        medal        CHAR(1)  NOT NULL,
        score        VARCHAR(20),
        unit         VARCHAR(5),
        PRIMARY KEY(host_year, event_code, athlete_code, medal)
        );

**FOREIGN KEY Constraint**

    A foreign key is a column or a set of columns that references the primary key in other tables in order to maintain reference relationship. The foreign key and the referenced primary key must have the same data type. Consistency between two tables is maintained by the foreign key referencing the primary key, which is called referential integrity. ::

        [ CONSTRAINT < constraint_name > ]
        FOREIGN KEY [ <foreign_key_name> ] ( column_name_comma_list1 )
        REFERENCES [ referenced_table_name ] ( column_name_comma_list2 )
        [ <referential_triggered_action> ]
         
        <referential_triggered_action> :
        ON UPDATE <referential_action>
        [ ON DELETE <referential_action> ]
         
        <referential_action> :
        CASCADE | RESTRICT | NO ACTION | SET NULL

    *   *constraint_name* : Specifies the name of the table to be created.
    *   *foreign_key_name* : Specifies a name of the **FOREIGN KEY** constraint. You can skip the name specification. However, if you specify this value, *constraint_name* will be ignored, and the specified value will be used.

    *   *column_name_comma_list1* : Specifies the name of the column to be defined as a foreign key after the **FOREIGN KEY** keyword. The column number of foreign keys defined and primary keys must be same.
    *   *referenced_table_name* : Specifies the name of the table to be referenced.
    *   *column_name_comma_list2* : Specifies the name of the referred primary key column after the **FOREIGN KEY** keyword.

    *   *referential_triggered_action* : Specifies the trigger action that responds to a certain operation in order to maintain referential integrity. **ON UPDATE** or **ON DELETE** can be specified. Each action can be defined multiple times, and the definition order is not significant.

    *   **ON UPDATE** : Defines the action to be performed when attempting to update the primary key referenced by the foreign key. You can use either **NO ACTION**, **RESTRICT**, or **SET NULL** option. The default is **RESTRICT**.

    *   **ON DELETE** : Defines the action to be performed when attempting to delete the primary key referenced by the foreign key. You can use **NO ACTION**, **RESTRICT**, **CASCADE**, or **SET NULL** option. The default is **RESTRICT**.

    *   *referential_ action* : You can define an option that determines whether to maintain the value of the foreign key when the primary key value is deleted or updated.
    *   **CASCADE** : If the primary key is deleted, the foreign key is deleted as well. This option is supported only for the **ON DELETE** operation.
    *   **RESTRICT** : Prevents the value of the primary key from being deleted or updated, and rolls back any transaction that has been attempted.
    *   **SET NULL** : When a specific record is being deleted or updated, the column value of the foreign key is updated to **NULL**.
    *   **NO ACTION** : Its behavior is the same as that of the **RESTRICT** option.

    .. code-block:: sql

        --creaing two tables where one is referencing the other
        CREATE TABLE a_tbl(
          id INT NOT NULL DEFAULT 0 PRIMARY KEY,
          phone VARCHAR(10)
        );
         
        CREATE TABLE b_tbl(
          id INT NOT NULL,
          name VARCHAR(10) NOT NULL,
          CONSTRAINT pk_id PRIMARY KEY(id),
          CONSTRAINT fk_id FOREIGN KEY(id) REFERENCES a_tbl(id)
          ON DELETE CASCADE ON UPDATE RESTRICT
        );
         
        INSERT INTO a_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333');
        INSERT INTO b_tbl VALUES(1,'George'),(2,'Laura'),(3,'Max');
        SELECT a.id, b.id, a.phone, b.name FROM a_tbl a, b_tbl b WHERE a.id=b.id;
         
           id           id                   phone                 name
        ======================================================================
            1            1                   '111-1111'            'George'
            2            2                   '222-2222'            'Laura'
            3            3                   '333-3333'            'Max'
         
        --when deleting primay key value, it cascades foreign key value  
        DELETE FROM a_tbl WHERE id=3;
         
        1 rows affected.
         
        SELECT a.id, b.id, a.phone, b.name FROM a_tbl a, b_tbl b WHERE a.id=b.id;
         
           id           id                   phone                 name
        ======================================================================
            1            1                   '111-1111'            'George'
            2            2                   '222-2222'            'Laura'
         
        --when attempting to update primay key value, it restricts the operation
        UPDATE  a_tbl SET id = 10 WHERE phone = '111-1111';
         
        In the command from line 1,
         
        ERROR: Update/Delete operations are restricted by the foreign key 'fk_id'.
         
        0 command(s) successfully processed.

    .. note::

        *   In a referential constraint, the name of the primary key table to be referenced and the corresponding column names are defined. If the list of column names are is not specified, the primary key of the primary key table is specified in the defined order.
        *   The number of primary keys in a referential constraint must be identical to that of foreign keys. The same column name cannot be used multiple times for the primary key in the referential constraint.
        *   The actions cascaded by reference constraints do not activate the trigger action.
        *   It is not recommended to use *referential_triggered_action* in the CUBRID HA environment. In the CUBRID HA environment, the trigger action is not supported. Therefore, if you use *referential_triggered_action*, the data between the master database and the slave database can be inconsistent. For details, see :doc:`/admin/ha`.

**KEY or INDEX**

    **KEY** and **INDEX** are used interchangeably. They create an index that uses the corresponding column as a key.

    .. note:: In versions lower than CUBRID 9.0, index name can be omitted; however, in version of CUBRID 9.0 or higher, it is no longer supported.

    .. code-block:: sql

        CREATE TABLE const_tbl4(id INT, phone VARCHAR, KEY i_key(id DESC, phone ASC));

Column Option
-------------

You can specify options such as **ASC** or **DESC** after the column name when defining **UNIQUE** or **INDEX** for a specific column. This keyword is specified to store the index value in ascending or descending order. ::

    column_name [ASC|DESC]

.. code-block:: sql

    CREATE TABLE const_tbl(
    id VARCHAR,
    name VARCHAR,
    CONSTRAINT UNIQUE INDEX(id DESC, name ASC)
    );
     
    INSERT INTO const_tbl VALUES('1000', 'john'), ('1000','johnny'), ('1000', 'jone');
    INSERT INTO const_tbl VALUES('1001', 'johnny'), ('1001','john'), ('1001', 'jone');
     
    SELECT * FROM const_tbl WHERE id > '100';
    
      id    name    
    =================
      1001     john     
      1001     johnny     
      1001     jone     
      1000     john     
      1000     johnny     
      1000     jone

Table Option (REUSE_OID)
------------------------

You can specify the **REUSE_OID** option when creating a table, so that OIDs that have been deleted due to the deletion of records (**DELETE**) can be reused when a new record is inserted (**INSERT**). Such a table is called an OID reusable or a non-referable table.

OID (Object Identifier) is an object identifier represented by physical location information such as the volume number, page number and slot number. By using such OIDs, CUBRID manages the reference relationships of objects and searches, stores or deletes them. When an OID is used, accessibility is improved because the object in the heap file can be directly accessed without referring to the table. However, the problem of decreased reusability of the storage occurs when there are many **DELETE/ INSERT** operations because the object's OID is kept to maintain the reference relationship with the object even if it is deleted.

If you specify the **REUSE_OID** option when creating a table, the OID is also deleted when data in the table is deleted, so that another **INSERT** ed data can use it. OID reusable tables cannot be referred to by other tables, and OID values of the objects in the OID reusable tables cannot be viewed.

.. code-block:: sql

    --creating table with REUSE_OID option specified
    CREATE TABLE reuse_tbl (a INT PRIMARY KEY) REUSE_OID;
    INSERT INTO reuse_tbl VALUES (1);
    INSERT INTO reuse_tbl VALUES (2);
    INSERT INTO reuse_tbl VALUES (3);
     
    --an error occurs when column type is a OID reusable table itself
    CREATE TABLE tbl_1 ( a reuse_tbl);
     
    ERROR: The class 'reuse_tbl' is marked as REUSE_OID and is non-referable. Non-referable classes can't be the domain of an attribute and their instances' OIDs cannot be returned.
     
    --an error occurs when a table references a OID reusable table

If you specify REUSE_OID together with the collation of table, it can be placed on before or after **COLLATE** syntax.
     
.. code-block:: sql
    
    CREATE TABLE t3(a VARCHAR(20)) REUSE_OID COLLATE euckr_bin;
    CREATE TABLE t4(a VARCHAR(20)) COLLATE euckr_bin REUSE_OID;

.. note::

    *   OID reusable tables cannot be referred to by other tables.
    *   Updatable views cannot be created for OID reusable tables.
    *   OID reusable tables cannot be specified as table column type.
    *   OID values of the objects in the OID reusable tables cannot be read.
    *   Instance methods cannot be called from OID reusable tables. Also, instance methods cannot be called if a sub class inherited from the class where the method is defined is defined as an OID reusable table.
    *   OID reusable tables are supported only by CUBRID 2008 R2.2 or above, and backward compatibility is not ensured. That is, the database in which the OID reusable table is located cannot be accessed from a lower version database.
    *   OID reusable tables can be managed as partitioned tables and can be replicated.

CREATE TABLE LIKE
-----------------

You can create a table that has the same schema as an existing table by using the **CREATE TABLE...LIKE** statement. Column attribute, table constraint, and index are replicated from the existing table. An index name created from the existing table changes according to a new table name, but an index name defined by a user is replicated as it is. Therefore, you should be careful at a query statement that is supposed to use a specific index created by using the index hint syntax(see ref:`index-hint-syntax`).

You cannot create the column definition because the **CREATE TABLE ... LIKE** statement replicates the schema only. ::

    CREATE {TABLE | CLASS} <new_table_name> LIKE <old_table_name>;

*   *new_table_name* : A table name to be created
*   *old_table_name* : The name of the original table that already exists in the database. The following tables cannot be specified as original tables in the **CREATE TABLE … LIKE** statement.
    * Partition table
    * Table that contains an **AUTO_INCREMENT** column
    * Table that uses inheritance or methods

.. code-block:: sql

    CREATE TABLE a_tbl(
      id INT NOT NULL DEFAULT 0 PRIMARY KEY,
      phone VARCHAR(10)
    );
    INSERT INTO a_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333');
     
    --creating an empty table with the same schema as a_tbl
    CREATE TABLE new_tbl LIKE a_tbl;
    SELECT * FROM new_tbl;
     
    There are no results.
     
    ;schema a_tbl
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         a_tbl
     
     <Attributes>
     
         id                   INTEGER DEFAULT 0 NOT NULL
         phone                CHARACTER VARYING(10)
     
     <Constraints>
     
         PRIMARY KEY pk_a_tbl_id ON a_tbl (id)
     
    Current transaction has been committed.
     
    ;schema new_tbl
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         new_tbl
     
     <Attributes>
     
         id                   INTEGER DEFAULT 0 NOT NULL
         phone                CHARACTER VARYING(10)
     
     <Constraints>
     
         PRIMARY KEY pk_new_tbl_id ON new_tbl (id)
     
     
    Current transaction has been committed.

CREATE TABLE AS SELECT
----------------------

You can create a new table that contains the result records of the **SELECT** statement by using the **CREATE TABLE...AS SELECT** statement. You can define column and table constraints for the new table. The following rules are applied to reflect the result records of the **SELECT** statement.

*   If *col_1* is defined in the new table and the same column *col_1* is specified in *select_statement*, the result record of the **SELECT** statement is stored as *col_1* value in the new table. Type casting is attempted if the column names are identical but the columns types are different.

*   If *col_1* and  *col_2* are defined in the new table, *col_1*, col_2 and *col_3* are specified in the column list of the *select_statement* and there is a containment relationship between all of them, *col_1*, *col_2* and *col_3* are created in the new table and the result data of the **SELECT** statement is stored as values for all columns. Type casting is attempted if the column names are identical but the columns types are different.

*   If columns *col_1* and *col_2* are defined in the new table and *col_1* and *col_3* are defined in the column list of *select_statement* without any containment relationship between them, *col_1*, *col_2* and *col_3* are created in the new table, the result data of the **SELECT** statement is stored only for *col_1* and *col_3* which are specified in *select_statement*, and **NULL** is stored as the value of *col_2*.

*   Column aliases can be included in the column list of *select_statement*. In this case, new column alias is used as a new table column name. It is recommended to use an alias because invalid column name is created, if an alias does not exist when a function calling or an expression is used.

*   The **REPLACE** option is valid only when the **UNIQUE** constraint is defined in a new table column (*col_1*). When duplicate values exist in the result record of *select_statement*, a **UNIQUE** value is stored for *col_1* if the **REPLACE** option has been defined, or an error message is displayed if the **REPLACE** option is omitted due to the violation of the **UNIQUE** constraint.

::

    CREATE {TABLE | CLASS} <table_name> [( <column_definition> [,<table_constraint>]... )]
    [REPLACE] AS <select_statement>

*   *table_name* : A name of the table to be created.
*   *column_definition* : Defines a column. If it is omitted, the column schema of **SELECT** statement is replicated; however, the constraint or the **AUTO_INCREMENT** attribute is not replicated.
*   *table_constraint* : Defines table constraint.
*   *select_statement* : A **SELECT** statement targeting a source table that already exists in the database.

.. code-block:: sql

    CREATE TABLE a_tbl(
      id INT NOT NULL DEFAULT 0 PRIMARY KEY,
      phone VARCHAR(10));
    INSERT INTO a_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333');
     
    --creating a table without column definition
    CREATE TABLE new_tbl1 AS SELECT * FROM a_tbl;
    SELECT * FROM new_tbl1;
     
       id  phone
    ===========================
        1  '111-1111'
        2  '222-2222'
        3  '333-3333'
     
    --all of column values are replicated from a_tbl
    CREATE TABLE new_tbl2(
      id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
      phone VARCHAR
    ) AS SELECT * FROM a_tbl;
    
    SELECT * FROM new_tbl2;
     
       id  phone
    ===========================
        1  '111-1111'
        2  '222-2222'
        3  '333-3333'
     
    --some of column values are replicated from a_tbl and the rest is NULL
    CREATE TABLE new_tbl3 (
      id INT, 
      name VARCHAR
    ) AS SELECT id, phone FROM a_tbl;
    
    SELECT * FROM new_tbl3
     
      name                           id  phone
    =========================================================
      NULL                            1  '111-1111'
      NULL                            2  '222-2222'
      NULL                            3  '333-3333'
     
    --column alias in the select statement should be used in the column definition
    CREATE TABLE new_tbl4 (
      id1 int,
      id2 int
    ) AS SELECT t1.id id1, t2.id id2 FROM new_tbl1 t1, new_tbl2 t2;
    
    SELECT * FROM new_tbl4;
     
      id1          id2
    ==================
        1            1
        1            2
        1            3
        2            1
        2            2
        2            3
        3            1
        3            2
        3            3
     
    --REPLACE is used on the UNIQUE column
    CREATE TABLE new_tbl5(id1 int UNIQUE) REPLACE AS SELECT * FROM new_tbl4;
    
    SELECT * FROM new_tbl5;
     
      id1          id2
    ==================
        1            3
        2            3
        3            3


ALTER TABLE
===========

You can modify the structure of a table by using the **ALTER** statement. You can perform operations on the target table such as adding/deleting columns, creating/deleting indexes, and type casting existing columns as well as changing table names, column names and constraints. **TABLE** and **CLASS** are used interchangeably **VIEW** and **VCLASS**, and **COLUMN** and **ATTRIBUTE** as well.

You can also change the initial value of **AUTO_INCREMENT**. ::

    ALTER [ <class_type> ] <table_name> <alter_clause> ;
     
    <class_type> ::= TABLE | CLASS | VCLASS | VIEW
     
    <alter_clause> ::= ADD <alter_add> [ INHERIT <resolution_comma_list> ] | 
                       ADD { KEY | INDEX } <index_name> (<index_col_name>) |
                       ALTER [ COLUMN ] column_name SET DEFAULT <value_specifiation> |
                       DROP <alter_drop> [ INHERIT <resolution_comma_list> ] |
                       DROP { KEY | INDEX } index_name |
                       DROP FOREIGN KEY constraint_name |
                       DROP PRIMARY KEY |                   
                       RENAME <alter_rename> [ INHERIT <resolution_comma_list> ] |
                       CHANGE <alter_change> |
                       INHERIT <resolution_comma_list>
                       AUTO_INCREMENT = <initial_value>
     
    <alter_add> ::= [ ATTRIBUTE | COLUMN ] [(]<class_element_comma_list>[)] [ FIRST | AFTER old_column_name ] |
                    CLASS ATTRIBUTE <column_definition_comma_list> |
                    CONSTRAINT < constraint_name > <column_constraint> ( column_name )|
                    FILE <file_name_comma_list> |
                    METHOD <method_definition_comma_list> |
                    QUERY <select_statement> |
                    SUPERCLASS <class_name_comma_list>
     
    <alter_change> ::= FILE <file_path_name> AS <file_path_name> |
                       METHOD <method_definition_comma_list> |
                       QUERY [ <unsigned_integer_literal> ] <select_statement> |
                       <column_name> DEFAULT <value_specifiation>
     
    <alter_drop> ::= [ ATTRIBUTE | COLUMN | METHOD ]
                     <column_name_comma_list> |
                     FILE <file_name_comma_list> |
                     QUERY [ <unsigned_integer_literal> ] |
                     SUPERCLASS <class_name_comma_list> |
                     CONSTRAINT <constraint_name>
     
    <alter_rename> ::= [ ATTRIBUTE | COLUMN | METHOD ]
                       <old_column_name> AS <new_column_name> |
                       FUNCTION OF <column_name> AS <function_name>
                       FILE <file_path_name> AS <file_path_name>
     
    <resolution> ::= { column_name | method_name } OF <superclass_name>
                     [ AS alias ]
     
    <class_element> ::= <column_definition> | <table_constraint>
     
    <column_constraint> ::= UNIQUE [ KEY ] | PRIMARY KEY | FOREIGN KEY
     
    <index_col_name> ::=
    column_name [(length)] [ ASC | DESC ]

.. warning::

    The table name can be changed only by the table owner, **DBA** and **DBA** members. The other users must be granted to change the name by the owner or **DBA** (see :ref:`granting-authorization` For details on authorization).

ADD COLUMN Clause
-----------------

You can add a new column by using the **ADD COLUMN** clause. You can specify the location of the column to be added by using the **FIRST** or **AFTER** keyword.

If the newly added column has the **NOT NULL** constraint but no **DEFAULT** constraint, it will have the hard default when the database server configuration parameter, **add_column_update_hard_default** is set to yes. However, when the parameter is set to no, the column will have **NULL** even with the **NOT NULL** constraint.

If the newly added column has the **PRIMARY KEY** or **UNIQUE** constraints, an error will be returned when the database server configuration parameter **add_column_update_hard_default** is set to yes. When the parameter is set to no, all data will have **NULL**. The default value of **add_column_update_hard_default** is **no**.

For **add_column_update_hard_default** and the hard default, see :ref:`change-column`. ::

    ALTER [ TABLE | CLASS | VCLASS | VIEW ] table_name
    ADD [ COLUMN | ATTRIBUTE ] [(]<column_definition>[)] [ FIRST | AFTER old_column_name ]
     
    column_definition ::=
    column_name column_type
        { [ NOT NULL | NULL ] |
          [ { SHARED <value_specification> | DEFAULT <value_specification> }
              | AUTO_INCREMENT [(seed, increment)] ] |
          [ UNIQUE [ KEY ] |
              [ PRIMARY KEY | FOREIGN KEY REFERENCES
                  [ referenced_table_name ]( column_name_comma_list )
                  [ <referential_triggered_action> ... ]
              ]
          ] } ...
     
    <referential_triggered_action> ::=
    { ON UPDATE <referential_action> } |
    { ON DELETE <referential_action> }
     
    <referential_action> ::=
    CASCADE | RESTRICT | NO ACTION | SET NULL

*   *table_name* : Specifies the name of a table that has a column to be added.
*   *column_definition* : Specifies the name, data type, and constraints of a column to be added.
*   **AFTER** *oid_column_name* : Specifies the name of an existing column before the column to be added.

.. code-block:: sql

    CREATE TABLE a_tbl;
    ALTER TABLE a_tbl ADD COLUMN age INT DEFAULT 0 NOT NULL;
    INSERT INTO a_tbl(age) VALUES(20),(30),(40);
    ALTER TABLE a_tbl ADD COLUMN name VARCHAR FIRST;
    ALTER TABLE a_tbl ADD COLUMN id INT NOT NULL AUTO_INCREMENT UNIQUE;
    ALTER TABLE a_tbl ADD COLUMN phone VARCHAR(13) DEFAULT '000-0000-0000' AFTER name;
     
    SELECT * FROM a_tbl;
     
      name                  phone                         age           id
    ======================================================================
      NULL                  '000-0000-0000'                20         NULL
      NULL                  '000-0000-0000'                30         NULL
      NULL                  '000-0000-0000'                40         NULL
     
    --adding multiple columns
    ALTER TABLE a_tbl ADD COLUMN (age1 int, age2 int, age3 int);

ADD CONSTRAINT Clause
---------------------

You can add a new constraint by using the **ADD CONSTRAINT** clause.

By default, the index created when you add **PRIMARY KEY** constraints is created in ascending order, and you can define the key sorting order by specifying the **ASC** or **DESC** keyword next to the column name. ::

    ALTER [ TABLE | CLASS | VCLASS | VIEW ] table_name
    ADD CONSTRAINT < constraint_name > column_constraint ( column_name_comma_list )
     
    column_constraint ::=
    UNIQUE [ KEY ] |
    PRIMARY KEY |
    FOREIGN KEY [ <foreign_key_name> ] REFERENCES [referenced_table_name]( column_name_comma_list )
                           [ <referential_triggered_action> ... ]
     
    <referential_triggered_action> ::=
    { ON UPDATE <referential_action> } |
    { ON DELETE <referential_action> }
     
    <referential_action> ::=
    CASCADE | RESTRICT | NO ACTION | SET NULL

*   *table_name* : Specifies the name of a table that has a constraint to be added.
*   *constraint_name* : Specifies the name of a constraint to be added, or it can be omitted. If omitted, a name is automatically assigned(maximum: 254 bytes).
*   *foreign_key_name* : Specifies a name of the **FOREIGN KEY** constraint. You can skip the name specification. However, if you specify this value, *constraint_name* will be ignored, and the specified value will be used.
*   *column_constraint* : Defines a constraint for the specified column. For details, see :ref:`constraint-definition`.

.. code-block:: sql

    ALTER TABLE a_tbl ADD CONSTRAINT PRIMARY KEY(id); 
    ALTER TABLE a_tbl ADD CONSTRAINT PRIMARY KEY(id, no DESC);
    ALTER TABLE a_tbl ADD CONSTRAINT UNIQUE u_key1(id);

ADD INDEX Clause
----------------

You can define the index attributes for a specific column by using the **ADD INDEX** clause. ::

    ALTER [ TABLE | CLASS ] table_name ADD { KEY | INDEX } index_name (<index_col_name>)
     
    <index_col_name> ::=
    column_name [(length)] [ ASC | DESC ]

*   *table_name* : Specifies the name of a table to be modified.
*   *index_name* : Specifies the name of an index(maximum: 254 bytes). If omitted, a name is automatically assigned.
*   *index_col_name* : Specifies the column that has an index to be defined. **ASC** or **DESC** can be specified for a column option; *prefix_length* of an index key also can be specified for a column option.

.. code-block:: sql

    ALTER TABLE a_tbl ADD INDEX i1(age ASC), ADD INDEX i2(phone DESC);
    ;schema a_tbl
     
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
     
    Current transaction has been committed.

ALTER COLUMN ... SET DEFAULT Clause
-----------------------------------

You can specify a new default value for a column that has no default value or modify the existing default value by using the **ALTER COLUMN** … **SET DEFAULT**. You can use the **CHANGE** clause to change the default value of multiple columns with a single statement. For details, see the :ref:`change-column`. ::

    ALTER [ TABLE | CLASS ] table_name ALTER [COLUMN] column_name SET DEFAULT value

*   *table_name* : Specifies the name of a table that has a column whose default value is to be modified.
*   *column_name* : Specifies the name of a column whose default value is to be modified.
*   *value* : Specifies a new default value.

.. code-block:: sql

    ;schema a_tbl
     
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
     
     
    Current transaction has been committed.
     
     
    ALTER TABLE a_tbl ALTER COLUMN name SET DEFAULT '';
    ALTER TABLE a_tbl ALTER COLUMN phone SET DEFAULT '111-1111';
     
    ;schema a_tbl
     
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


.. _alter-auto-increment:

AUTO_INCREMENT Clause
---------------------

The **AUTO_INCREMENT** clause can change the initial value of the increment value that is currently defined. However, there should be only one **AUTO_INCREMENT** column defined. ::

    ALTER TABLE table_name AUTO_INCREMENT = initial_value;
    
*   *table_name* : Table name
*   *initial_value* : Initial value to alter

**Example**

.. code-block:: sql

    CREATE TABLE t (i int AUTO_INCREMENT);
    ALTER TABLE t AUTO_INCREMENT = 5;
     
    -- when 2 AUTO_INCREMENT constraints are defined on one table, it returns error.
    CREATE TABLE t (i int AUTO_INCREMENT, j int AUTO_INCREMENT);
    ALTER TABLE t AUTO_INCREMENT = 5;
     
    ERROR: To avoid ambiguity, the AUTO_INCREMENT table option requires the table to have exactly one AUTO_INCREMENT column and no seed/increment specification.

.. warning:: You must be careful not to violate constraints (such as a **PRIMARY KEY** or **UNIQUE**) due to changing the initial value of **AUTO_INCREMENT**.

.. _change-column:

CHANGE/MODIFY Clauses
---------------------

The **CHANGE** clause changes column name, type, size, and attribute. If the existing column name and a new column name are the same, types, size, and attribute will be changed.

The **MODIFY** clause can modify type, size, and attribute of a column but cannot change its name.

If you set the type, size, and attribute to apply to a new column with the **CHANGE** clause or the **MODIFY** clause, the attribute that is currently defined will not be passed to the attribute of the new column.

When you change data types using the **CHANGE** clause or the **MODIFY** clause, the data can be modified. For example, if you shorten the length of a column, the character string may be truncated.

.. warning::

    * **ALTER TABLE** <table_name> **CHANGE** <column_name> **DEFAULT** <default_value> syntax supported in CUBRID 2008 R3.1 or earlier version is no longer supported.
    * When converting a number type to character type, if the length of the string is shorter than that of the number, the string is truncated and saved according to the length of the converted character type.
    * If the column attributs like a type, a collation, etc. are changed, the changed attributes are not applied into the view created with the table before the change. Therefore, if you change the attributes of a table, it is recommended to recreate the related views.

::

    ALTER TABLE tbl_name table_options;
     
    table_options :
         table_option[, table_option]
     
    table_option :
        CHANGE [COLUMN | CLASS ATTRIBUTE ] old_col_name new_col_name column_definition
                 [FIRST | AFTER col_name]
      | MODIFY [COLUMN | CLASS ATTRIBUTE] col_name column_definition
                 [FIRST | AFTER col_name]

*   *tbl_name* : Specifies the name of the table including the column to change.
*   *old_col_name* : Specifies the existing column name.
*   *new_col_name* : Specifies the column name to change
*   *column_definition* : Specifies the type, size, and attribute of the column to change.
*   *col_name* : Specifies the column name to which the type, size, and attribute of the column to apply changes.

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

.. code-block:: sql

    -- changing the name and position of a column  
    CREATE TABLE t1(i1 int,i2 int);  
    INSERT INTO t1 VALUE (1,11),(2,22),(3,33);  
    SELECT * FROM t1 ORDER BY 1;
                i1           i2
    ==========================
                 1           11
                 2           22
                 3           33
     
    ALTER TABLE t1 CHANGE i2 i0 INTEGER FIRST;  
    SELECT * FROM t1 ORDER BY 1;
                i0           i1
    ==========================
                11            1
                22            2
                33            3

.. code-block:: sql

    -- adding NOT NULL constraint (strict)
    -- alter_table_change_type_strict=yes
     
    CREATE TABLE t1(i int);
    INSERT INTO t1 values (11),(NULL),(22);
     
    ALTER TABLE t1 change i i1 integer not null;
     
    In the command from line 1,
     
    ERROR: Cannot add NOT NULL constraint for attribute "i1": there are existing NULL values for this attribute.

.. code-block:: sql

    -- adding NOT NULL constraint
    -- alter_table_change_type_strict=no
     
    CREATE TABLE t1(i int);
    INSERT INTO t1 VALUES (11),(NULL),(22);
     
    ALTER TABLE t1 CHANGE i i1 INTEGER NOT NULL;
     
    SELECT * FROM t1;
     
               i1
    =============
               22
                0
               11

.. code-block:: sql

    -- change the column's data type (no errors)
     
    CREATE TABLE t1 (i1 int);
    INSERT INTO t1 VALUES (1),(-2147483648),(2147483647);
     
    ALTER TABLE t1 CHANGE i1 s1 CHAR(11);
     
    SELECT * FROM t1;
     
      s1
    ======================
      '2147483647 '
      '-2147483648'
      '1          '

.. code-block:: sql

    -- change the column's data type (errors), strict mode
    -- alter_table_change_type_strict=yes
     
    CREATE TABLE t1 (i1 int);
    INSERT INTO t1 VALUES (1),(-2147483648),(2147483647);
     
    ALTER TABLE t1 CHANGE i1 s1 CHAR(4);
     
    In the command from line 1,
     
    ERROR: ALTER TABLE .. CHANGE : changing to new domain : cast failed, current configuration doesn't allow truncation or overflow.
     
    -- change the column's data type (errors)
    -- alter_table_change_type_strict=no
     
    CREATE TABLE t1 (i1 INT);
    INSERT INTO t1 VALUES (1),(-2147483648),(2147483647);
     
    ALTER TABLE t1 CHANGE i1 s1 CHAR(4);
     
    SELECT * FROM t1;
     
      s1
    ======================
      '    '
      '    '
      '1   '
     
    -- hard default values have been placed instead of signaling overflow

**Changes of Table Attributes based on Changes of Column Type**

    *   Type Change : If the value of the system parameter **alter_table_change_type_strict** is set to no, then changing values to other types is allowed, but if it is set to yes then changing is not allowed. The default value of the parameter is **no**. You can change values to all types allowed by the **CAST** operator. Changing object types is allowed only by the upper classes (tables) of the objects.

    *   **NOT NULL**

        *   If the **NOT NULL** constraint is not specified, it will be removed from a new table even though it is present in the existing table.
        *   If the **NOT NULL** constraint is specified in the column to change, the result varies depending on the configuration of the system parameter, **alter_table_change_type_strict**.

            *   If **alter_table_change_type_strict** is set to yes, the column values will be checked. If **NULL** exists, an error will occur, and the change will not be executed.
            *   If the **alter_table_change_type_strict** is set to no, every existing **NULL** value will be changed to a hard default value of the type to change.

    *   **DEFAULT** : If the **DEFAULT** attribute is not specified in the column to change, it will be removed from a new table even though the attribute is present in the existing table.

    *   **AUTO_INCREMENT** : If the **AUTO_INCREMENT** attribute is not specified in the column to change, it will be removed from a new table even though the attribute is present in the existing table.

    *   **FOREIGN KEY** : You cannot change the column with the foreign key constraint that is referred to or refers to.

    *   Single Column **PRIMARY KEY**

        *   If the **PRIMARY KEY** constraint is specified in the column to change, a **PRIMARY KEY** is re-created only in which a **PRIMARY KEY** constraint exists in the existing column and the type is upgraded.
        *   If the **PRIMARY KEY** constraint is specified in the column to change but doesn't exist in the existing column, a **PRIMARY KEY** will be created.
        *   If a **PRIMARY KEY** constraint exists but is not specified in the column to change, the **PRIMARY KEY** will be maintained.

    *   Multicolumn **PRIMARY KEY** : If the **PRIMARY KEY** constraint is specified and the type is upgraded, a **PRIMARY KEY** will be re-created.

    *   Single Column **UNIQUE KEY**

        *   If the type is upgraded, a **UNIQUE KEY** will be re-created.
        *   If a **UNIQUE KEY** exists in the existing column and it is not specified in the column to change, it will be maintained.
        *   If a **UNIQUE KEY** exists in the existing column to change, it will be created.

    *   Multicolumn **UNIQUE KEY** : If the column type is changed, an index will be re-created.
    *   Column with a Non-unique Index : If the column type is changed, an index will be re-created.
    *   Partition Column: If a table is partitioned by a column, the column cannot be changed. Partitions cannot be added.

    *   Column with a Class Hierarchy : You can only change the tables that do not have a lower class. You cannot change the lower class that inherits from an upper class. You cannot change the inherited attributes.

    *   Trigger and View : You must redefine triggers and views directly because they are not changed according to the definition of the column to change.
    *   Column Sequence : You can change the sequence of columns.
    *   Name Change : You can change names as long as they do not conflict.

.. note:: \

    **Changes of Values based on Changes of Column Type**

    The **alter_table_change_type_strict** parameter determines whether the value conversion is allowed according to the type change. If the value is no, it can be changed when you change a column type or add a **NOT NULL** constraint. The default value is **no**.

    When the value of the parameter, **alter_table_change_type_strict** is no, it will operate depending on the conditions as follows:

    *   Overflow occurred while converting numbers or character strings to Numbers: It is determined based on symbol of the result type. If it is negative value, it is specified as a minimum value or positive value, specified as the maximum value and a warning message for records where overflow occurred is recorded in the log. For strings, it will follow the rules stated above after it is converted to **DOUBLE** type.

    *   Character strings to convert to shorter ones: The record will be updated to the hard default value of the type that is defined and the warning message will be recorded in a log.

    *   Conversion failure due to other reasons: The record will be updated to the hard default value of the type that is defined and the warning message will be recorded in a log.

    If the value of the **alter_table_change_type_strict** parameter is yes, an error message will be displayed and the changes will be rolled back.

    The **ALTER CHANGE** statement checks the possibility of type conversion before updating a record but the type conversion of specific values may fail. For example, if the value format is not correct when you convert **VARCHAR** to **DATE**, the conversion may fail. In this case, the hard default value of the **DATE** type will be assigned.

    The hard default value is a value that will be used when you add columns with the **ALTER TABLE ... ADD  COLUMN** statement, add or change by converting types with the **ALTER TABLE ... CHANGE/MODIFY** statement. The operation will vary depending on the system parameter, **add_column_update_hard_default** in the **ADD COLUMN** statement.

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
    | MONETARY  | Yes                                 | 0                                       |
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
    | BIT       | Yes                                 |                                         |
    +-----------+-------------------------------------+-----------------------------------------+
    | VARBIT    | No                                  |                                         |
    +-----------+-------------------------------------+-----------------------------------------+
    | OBJECT    | No                                  |                                         |
    +-----------+-------------------------------------+-----------------------------------------+
    | BLOB      | No                                  |                                         |
    +-----------+-------------------------------------+-----------------------------------------+
    | CLOB      | No                                  |                                         |
    +-----------+-------------------------------------+-----------------------------------------+
    | ELO       | No                                  |                                         |
    +-----------+-------------------------------------+-----------------------------------------+

.. _rename-column:

RENAME COLUMN Clause
--------------------

You can change the name of the column by using the **RENAME COLUMN** clause. ::

    ALTER [ TABLE | CLASS | VCLASS | VIEW ] table_name
    RENAME [ COLUMN | ATTRIBUTE ] old_column_name { AS | TO } new_column_name

*   *table_name* : Specifies the name of a table that has a column to be renamed.
*   *old_column_name* : Specifies the name of a column.
*   *new_column_name* : Specifies a new column name after the **AS** keyword(maximum: 254 bytes).

.. code-block:: sql

    ALTER TABLE a_tbl RENAME COLUMN name AS name1;

DROP COLUMN Clause
------------------

You can delete a column in a table by using the **DROP COLUMN** clause. You can specify multiple columns to delete simultaneously by separating them with commas (,). ::

    ALTER [ TABLE | CLASS | VCLASS | VIEW ] table_name
    DROP [ COLUMN | ATTRIBUTE ] column_name, ...
    
*   *table_name* : Specifies the name of a table that has a column to be deleted.
*   *column_ name* : Specifies the name of a column to be deleted. Multiple columns can be specified by separating them with commas (,).

.. code-block:: sql

    ALTER TABLE a_tbl DROP COLUMN age1,age2,age3;

DROP CONSTRAINT Clause
----------------------

You can drop the constraints pre-defined for the table, such as **UNIQUE**, **PRIMARY KEY** and **FOREIGN KEY** by using the **DROP CONSTRAINT** clause. In this case, you must specify a constraint name. You can check these names by using the CSQL command (**;schema table_name**). ::

    ALTER [ TABLE | CLASS ] table_name
    DROP CONSTRAINT constraint_name

*   *table_name* : Specifies the name of a table that has a constraint to be dropped.
*   *constraint_name* : Specifies the name of a constraint to be dropped.  

.. code-block:: sql

    ALTER TABLE a_tbl DROP CONSTRAINT pk_a_tbl_id;
    ALTER TABLE a_tbl DROP CONSTRAINT fk_a_tbl_id;
    ALTER TABLE a_tbl DROP CONSTRAINT u_a_tbl_id;

DROP INDEX Clause
-----------------

You can delete an index defined for a column by using the **DROP INDEX** clause. ::

    ALTER [ TABLE | CLASS ] table_name DROP [ UNIQUE ] INDEX index_name

*   **UNIQUE** : Specifies that the index to be dropped is a unique index. The unique index can be dropped by using the **DROP CONSTRAINT** statement.
*   *table_name* : Specifies the name of a table of which constraints will be deleted.
*   *index_name* : Specifies the name of an index to be deleted.

.. code-block:: sql

    ALTER TABLE a_tbl DROP INDEX i_a_tbl_age;

DROP PRIMARY KEY Clause
-----------------------

You can delete a primary key constraint defined for a table by using the **DROP PRIMARY KEY** clause. You do have to specify the name of the primary key constraint because only one primary key can be defined by table. ::

    ALTER [ TABLE | CLASS ] table_name DROP PRIMARY KEY

*   *table_name* : Specifies the name of a table that has a primary key constraint to be deleted.

.. code-block:: sql

    ALTER TABLE a_tbl DROP PRIMARY KEY;

DROP FOREIGN KEY Clause
-----------------------

You can drop a foreign key constraint defined for a table using the **DROP FOREIGN KEY** clause. ::

    ALTER [ TABLE | CLASS ] table_name DROP FOREIGN KEY constraint_name

*   *table_name* : Specifies the name of a table whose constraint is to be deleted.
*   *constraint_name* : Specifies the name of foreign key constraint to be deleted.

.. code-block:: sql

    ALTER TABLE a_tbl DROP FOREIGN KEY fk_a_tbl_id;

DROP TABLE
==========

You can drop an existing table by the **DROP** statement. Multiple tables can be dropped by a single **DROP** statement. All rows of table are also dropped. If you use it together with the **IF EXISTS** statement, you can prevent errors from occurring and specify multiple tables in one statement. ::

    DROP [ TABLE | CLASS ] [ IF EXISTS ] <table_specification_comma_list>
     
    <table_specification_comma_list> ::=
    <single_table_spec> | ( <table_specification_comma_list> )
     
    <single_table_spec> ::=
    |[ ONLY ] table_name
    | ALL table_name [ ( EXCEPT table_name, ... ) ]

*   *table_name* : Specifies the name of the table to be dropped. You can delete multiple tables simultaneously by separating them with commas.

*   If a super class name is specified after the **ONLY** keyword, only the super class, not the sub classes inheriting from it, is deleted. If a super class name is specified after the **ALL** keyword, the super classes as well as the sub classes inheriting from it are all deleted. You can specify the list of sub classes not to be deleted after the **EXCEPT** keyword.

*   If sub classes that inherit from the super class specified after the **ALL** keyword are specified after the **EXCEPT** keyword, they are not deleted.

*   Specifies the list of subclasses which are not to be deleted after the **EXCEPT** keyword.

.. code-block:: sql

    DROP TABLE history ;
    CREATE TABLE t (i INT);
     
    -- DROP TABLE IF EXISTS
    DROP TABLE IF EXISTS history, t;
    2 command(s) successfully processed.
     
    SELECT * FROM t;
    In line 1, column 10, ERROR: Unknown class "t".

RENAME TABLE
============

You can change the name of a table by using the **RENAME TABLE** statement and specify a list of the table name to change the names of multiple tables. ::

    RENAME  [ TABLE | CLASS | VIEW | VCLASS ] old_table_name { AS | TO } new_table_name [, old_table_name { AS | TO } new_table_name, ... ]

*   *old_table_name* : Specifies the old table name to be renamed.
*   *new_table_name* : Specifies a new table name(maximum: 254 bytes).

.. code-block:: sql

    RENAME TABLE a_tbl AS aa_tbl;
    RENAME TABLE a_tbl TO aa_tbl, b_tbl TO bb_tbl;

.. warning::

    The table name can be changed only by the table owner, **DBA** and **DBA** members. The other users must be granted to change the name by the owner or **DBA** (see :ref:`granting-authorization` For details on authorization).
