*********************
Information Functions
*********************

CHARSET
=======

.. function:: CHARSET(expr)

    This function returns the character set of *expr* .
    
    :param expr: Target expression to get the character set.
    
    :rtype: STRING

.. code-block:: sql

    SELECT CHARSET('abc');
    'iso88591'
    
    SELECT CHARSET(_utf8'abc');
    'utf8'
    
    SET NAMES utf8;
    SELECT CHARSET('abc');
    'utf8'
    
COERCIBILITY
============
        
.. function:: COERCIBILITY(expr)
    
    This function returns the collation coercibility level of *expr*. The collation coercibility level determines which collation or charset should be used when each column(expression) has different collation or charset. For more details, please refer :ref:`Collation Coercibility <collation-coercibility>`.

    :param expr: Target expression to get the collation coercibility level.

    :rtype: INT
    
.. code-block:: sql

    SELECT COERCIBILITY(USER());
    7
    
    SELECT COERCIBILITY(_utf8'abc');
    10

COLLATION
=========

.. function:: COLLATION(expr)

    This function returns the collation of *expr*.
    
    :param expr: Target expression to get the collation.

    :rtype: STRING
    
.. code-block:: sql

    SELECT COLLATION('abc');
    'iso88591_bin'
    
    SELECT COLLATION(_utf8'abc');
    'utf8_bin'

CURRENT_USER, USER
==================

.. c:macro:: CURRENT_USER

.. c:macro:: USER

    **CURRENT_USER** and **USER** are pseudo-columns and can be used interchangeably. They return the user name that is currently logged in to the database as a string.

    Please note that :func:`USER` and :func:`SYSTEM_USER` functions return the user name with a host name.

    :rtype: STRING
    
.. code-block:: sql

    --selecting the current user on the session
    SELECT USER;
    
       CURRENT_USER
    ======================
      'PUBLIC'
     
    SELECT USER(), CURRENT_USER;
    
       user()                CURRENT_USER
    ============================================
      'PUBLIC@cdbs006.cub'  'PUBLIC'
     
    --selecting all users of the current database from the system table
    SELECT name, id, password FROM db_user;
    
      name                           id  password
    =========================================================
      'DBA'                        NULL  NULL
      'PUBLIC'                     NULL  NULL
      'SELECT_ONLY_USER'           NULL  db_password
      'ALMOST_DBA_USER'            NULL  db_password
      'SELECT_ONLY_USER2'          NULL  NULL

DATABASE, SCHEMA
================

.. function:: DATABASE()
.. function:: SCHEMA()

    The functions **DATABASE** and **SCHEMA** are used interchangeably. They return the name of currently-connected database as a **VARCHAR** type.

    :rtype: STRING
    
.. code-block:: sql

    SELECT DATABASE(), SCHEMA();
    
       database()            schema()
    ============================================
      'demodb'              'demodb'

DEFAULT
=======

.. function:: DEFAULT (column_name)
.. c:macro:: DEFAULT

The **DEFAULT** and the **DEFAULT** function returns a default value defined for a column. If a default value is not specified for the column, **NULL** or an error is output. **DEFAULT** has no parameter, however, the **DEFAULT** function uses the column name as the input parameter. **DEFAULT** can be used for the input data of the **INSERT** statement and the **SET** clause of the **UPDATE** statement and the **DEFAULT** function can be used anywhere.

If any of constraints is not defined or the **UNIQUE** constraint is defined for the column where a default value is not defined, **NULL** is returned. If **NOT NULL** or **PRIMARY KEY** constraint is defined, an error is returned.

.. code-block:: sql

    CREATE TABLE info_tbl(id INT DEFAULT 0, name VARCHAR)
    INSERT INTO info_tbl VALUES (1,'a'),(2,'b'),(NULL,'c');
     
    3 rows affected.
     
    SELECT id, DEFAULT(id) FROM info_tbl;
    
               id   default(id)  
    =============================
                1             0
                2             0  
             NULL             0   
     
    UPDATE info_tbl SET id = DEFAULT WHERE id IS NULL;
    DELETE FROM info_tbl WHERE id = DEFAULT(id);
    INSERT INTO info_tbl VALUES (DEFAULT,'d');

.. note::
    In version lower than CUBRID 9.0, the value at the time of CREATE TABLE has been saved when the DATE value of the DATE, DATETIME, TIME, TIMESTAMP column has been specified to SYS_DATE, SYS_DATETIME, SYS_TIME, SYS_TIMESTAMP while creating a table. Therefore, to enter the value at the time of data INSERT in version lower than CUBRID 9.0, the function should be entered to the VALUES clause of the INSERT syntax.
    
INDEX_CARDINALITY
=================

.. function:: INDEX_CARDINALITY(table, index, key_pos)

    The **INDEX_CARDINALITY** function returns the index cardinality in a table. The index cardinality is the number of unique values defining the index. The index cardinality can be applied even to the partial key of the multiple column index and displays the number of the unique value for the partial key by specifying the column location with the third parameter.

    :param table: Table name
    :param index: Index name that exists in the *table*
    :param key_pos: Partial key location. It *key_pos* starts from 0 and has a range that is smaller than the number of columns consisting of keys; that is, the *key_pos* of the first column is 0. For the single column index, it is 0. It can be one of the following types.
    
        * Character string that can be converted to a numeric type.
        * Numeric type that can be converted to an integer type. The **FLOAT** or the **DOUBLE** types will be the value converted by the **ROUND** function.

    :rtype: INT

The return value is 0 or a positive integer and if any of the input parameters is **NULL**, **NULL** is returned. If tables or indexes that are input parameters are not found, or *key_pos* is out of range, **NULL** is returned.

For the table and the index names which are the first and the second input parameters, they cannot be passed as **NCHAR** or **VARNCHAR** types.

.. code-block:: sql

    CREATE TABLE t1( i1 INTEGER ,
    i2 INTEGER not null,
    i3 INTEGER unique,
    s1 VARCHAR(10),
    s2 VARCHAR(10),
    s3 VARCHAR(10) UNIQUE);
      
    CREATE INDEX i_t1_i1 ON t1(i1 DESC);
    CREATE INDEX i_t1_s1 ON t1(s1(7));
    CREATE INDEX i_t1_i1_s1 on t1(i1,s1);
    CREATE UNIQUE INDEX i_t1_i2_s2 ON t1(i2,s2);
     
    INSERT INTO t1 VALUES (1,1,1,'abc','abc','abc');
    INSERT INTO t1 VALUES (2,2,2,'zabc','zabc','zabc');
    INSERT INTO t1 VALUES (2,3,3,'+abc','+abc','+abc');
     
    SELECT INDEX_CARDINALITY('t1','i_t1_i1_s1',0);
    
       index_cardinality('t1', 'i_t1_i1_s1', 0)
    ===========================================
                                              2
     
    SELECT INDEX_CARDINALITY('t1','i_t1_i1_s1',1);
    
       index_cardinality('t1', 'i_t1_i1_s1', 1)
    ===========================================
                                              3
     
    SELECT INDEX_CARDINALITY('t1','i_t1_i1_s1',2);
    
       index_cardinality('t1', 'i_t1_i1_s1', 2)
    ===========================================
                                           NULL
     
    SELECT INDEX_CARDINALITY('t123','i_t1_i1_s1',1);
    
      index_cardinality('t123', 'i_t1_i1_s1', 1)
    ============================================
                                           NULL

INET_ATON
=========

.. function:: INET_ATON( ip_string )

    The **INET_ATON** function receives the string of an IPv4 address and returns a number. When an IP address string such as 'a.b.c.d' is entered, the function returns "a * 256 ^ 3 + b * 256 ^ 2 + c * 256 + d". The return type is **BIGINT**.

    :param ip_string: IPv4 address string
    :rtype: BIGINT

In the following example, 192.168.0.10 is calculated as "192 * 256 ^ 3 + 168 * 256 ^ 2 + 0 * 256 + 10".

.. code-block:: sql

    SELECT INET_ATON('192.168.0.10');
     
       inet_aton('192.168.0.10')
    ============================
                      3232235530

INET_NTOA
=========

.. function:: INET_NTOA( expr )

    The **INET_NTOA** function receives a number and returns an IPv4 address string. The return type is VARCHAR.

    :param expr: Numeric expression
    :rtype: STRING

.. code-block:: sql

    SELECT INET_NTOA(3232235530);
     
       inet_ntoa(3232235530)
    ======================
      '192.168.0.10'

LAST_INSERT_ID
==============

.. function:: LAST_INSERT_ID()

    The **LAST_INSERT_ID** function returns the value that has been most recently inserted to the **AUTO_INCREMENT** column by a single **INSERT** statement. 
    
    :rtype: BIGINT
    
The value returned by the **LAST_INSERT_ID** function has the following characteristics.

*   If no values are **INSERT** ed successfully, the latest successful value will be maintained.
*   SQL statement on execution does not affect the **LAST_INSERT_ID** () value.
*   The **LAST_INSERT_ID** () will return the first entered **AUTO_INCREMENT** () value in the **INSERT** statement with multiple rows (ex. INSERT INTO tbl VALUES (), (), …, ()).
*   The **LAST_INSERT_ID** () value will not be back to the state in the transaction began even though rollback is performed.
*   The **LAST_INSERT_ID** () value used within the trigger cannot be verified outside trigger.
*   Each **LAST_INSERT_ID** is working independently for applications.

.. code-block:: sql

    CREATE TABLE ss (id INT AUTO_INCREMENT NOT NULL PRIMARY KEY, text VARCHAR(32));
    INSERT INTO ss VALUES (NULL, 'cubrid');
    SELECT LAST_INSERT_ID ();
     
         last_insert_id()
    =======================
                         1
     
    INSERT INTO ss VALUES (NULL, 'database'), (NULL, 'manager');
    SELECT LAST_INSERT_ID ();
     
         last_insert_id()
    =======================
                         2

.. code-block:: sql

    CREATE TABLE tbl (id INT AUTO_INCREMENT);
    INSERT INTO tbl values (500), (NULL), (NULL);
    SELECT LAST_INSERT_ID();
     
         last_insert_id()
    =======================
                         1
     
    INSERT INTO tbl VALUES (500), (NULL), (NULL);
    SELECT LAST_INSERT_ID();
     
         last_insert_id()
    =======================
                         3
     
    SELECT * FROM tbl;
     
                        id
    =======================
                       500
                         1
                         2
                       500
                         3
                         4

LIST_DBS
========

.. function:: LIST_DBS()

    The **LIST_DBS** function outputs the list of all databases in the directory file(**$CUBRID_DATABASES/databases.txt**), separated by blanks. 

    :rtype: STRING
        
.. code-block:: sql

    SELECT LIST_DBS();
    
      list_dbs()
    ======================
      'testdb demodb'

ROW_COUNT
=========

.. function:: ROW_COUNT()

    The **ROW_COUNT** function returns the number of rows updated (**UPDATE**, **INSERT**, **DELETE**, **REPLACE**) by the previous statement. 
    
    ROW_COUNT returns 1 for each inserted row and 2 for each updatead row for **INSERT ON DUPLICATE KEY UPDATE** statement. It returns the sum of number of deleted and inserted rows for **REPLACE** statement.
    
    Statements triggered by trigger will not affect the ROW_COUNT for the statement.
        
    :rtype: INT
    
.. code-block:: sql

    CREATE TABLE rc (i int);
    INSERT INTO rc VALUES (1),(2),(3),(4),(5),(6),(7);
    SELECT ROW_COUNT();
    
       row_count()
    ===============
                  7
     
    UPDATE rc SET i = 0 WHERE i >  3;
    SELECT ROW_COUNT();
    
       row_count()
    ===============
                  4
     
    DELETE FROM rc WHERE i = 0;
    SELECT ROW_COUNT();
    
       row_count()
    ===============
                  4

USER, SYSTEM_USER
=================

.. function:: USER()

.. function:: SYSTEM_USER()

    The functions **USER** and **SYSTEM_USER** are identical and they return the user name together with the host name. The :c:macro:`USER` and :c:macro:`CURRENT_USER` pseudo-columns return the user names who has logged on to the current database as character strings.

    :rtype: STRING

.. code-block:: sql

    --selecting the current user on the session
    SELECT SYSTEM_USER ();
    
       user()
    ======================
      'PUBLIC@cubrid_host'
      
    SELECT USER(), CURRENT_USER;
    
       user()                CURRENT_USER
    ============================================
      'PUBLIC@cubrid_host'  'PUBLIC'
     
    --selecting all users of the current database from the system table
    SELECT name, id, password FROM db_user;
    
      name                           id  password
    =========================================================
      'DBA'                        NULL  NULL
      'PUBLIC'                     NULL  NULL
      'SELECT_ONLY_USER'           NULL  db_password
      'ALMOST_DBA_USER'            NULL  db_password
      'SELECT_ONLY_USER2'          NULL  NULL

VERSION
=======

.. function:: VERSION()

    The **VERSION** function returns the version character string representing the CUBRID server version.

    :rtype: STRING

.. code-block:: sql

    SELECT VERSION();
    
       version()
    =====================
      '9.1.0.0203'

