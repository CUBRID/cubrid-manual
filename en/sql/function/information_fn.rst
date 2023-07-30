
:meta-keywords: cubrid charset, cubrid coercibility, cubrid collation, cubrid current_user, cubrid default, cubrid last_insert_id, cubrid row_count

:tocdepth: 3

*********************
Information Functions
*********************

.. contents::

CHARSET
=======

.. function:: CHARSET(expr)

    This function returns the character set of *expr*.
    
    :param expr: Target expression to get the character set.
    
    :rtype: STRING

.. code-block:: sql
 
    SELECT CHARSET('abc');
    
::
    
    'iso88591'
    
.. code-block:: sql
 
    SELECT CHARSET(_utf8'abc');
    
::
    
    'utf8'
    
.. code-block:: sql
 
    SET NAMES utf8;
    SELECT CHARSET('abc');
    
::
    
    'utf8'
    
COERCIBILITY
============

.. function:: COERCIBILITY(expr)
    
    This function returns the collation coercibility level of *expr*. The collation coercibility level determines which collation or charset should be used when each column(expression) has different collation or charset. For more details, please see :ref:`Collation Coercibility <collation-coercibility>`.

    :param expr: Target expression to get the collation coercibility level.

    :rtype: INT
    
.. code-block:: sql

    SELECT COERCIBILITY(USER());
    
::

    7
    
.. code-block:: sql

    SELECT COERCIBILITY(_utf8'abc');
    
::
    
    10

COLLATION
=========

.. function:: COLLATION(expr)

    This function returns the collation of *expr*.
    
    :param expr: Target expression to get the collation.

    :rtype: STRING
    
.. code-block:: sql

    SELECT COLLATION('abc');
    
::

    'iso88591_bin'
    
.. code-block:: sql

    SELECT COLLATION(_utf8'abc');
    
::

    'utf8_bin'

CURRENT_USER, USER
==================

.. c:macro:: CURRENT_USER

.. c:macro:: USER

    **CURRENT_USER** and **USER** are pseudo-columns and can be used interchangeably. They return the user name that is currently logged in to the database as a string.

    Please note that :func:`SYSTEM_USER` and :func:`USER` functions return the user name with a host name where CSQL or CAS was executed.

    :rtype: STRING
    
.. code-block:: sql

    --selecting the current user on the session
    SELECT USER;
    
::

       CURRENT_USER
    ======================
      'PUBLIC'
     
.. code-block:: sql

    SELECT USER(), CURRENT_USER;
    
::

       user()                CURRENT_USER
    ============================================
      'PUBLIC@cdbs006.cub'  'PUBLIC'
     
.. code-block:: sql

    --selecting all users of the current database from the system table
    SELECT name, id, password FROM db_user;
    
::

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
    
::

       database()            schema()
    ============================================
      'demodb'              'demodb'

DBTIMEZONE
==========
      
.. function:: DBTIMEZONE()

    Prints out a timezone of database server (offset or region name) as a string. (e.g. '-05:00', or 'Europe/Vienna').

.. code-block:: sql

    SELECT DBTIMEZONE();

::
    
      dbtimezone
    ======================
      'Asia/Seoul'

.. seealso:: 

    :func:`SESSIONTIMEZONE`, :func:`FROM_TZ`, :func:`NEW_TIME`, :func:`TZ_OFFSET`


DEFAULT
=======

.. function:: DEFAULT(column_name)
.. c:macro:: DEFAULT

The **DEFAULT** and the **DEFAULT** function returns a default value defined for a column. If a default value is not specified for the column, **NULL** or an error is output. **DEFAULT** has no parameter, however, the **DEFAULT** function uses the column name as the input parameter. **DEFAULT** can be used for the input data of the **INSERT** statement and the **SET** clause of the **UPDATE** statement and the **DEFAULT** function can be used anywhere.

If any of constraints is not defined or the **UNIQUE** constraint is defined for the column where a default value is not defined, **NULL** is returned. If **NOT NULL** or **PRIMARY KEY** constraint is defined, an error is returned.

.. code-block:: sql

    CREATE TABLE info_tbl(id INT DEFAULT 0, name VARCHAR);
    INSERT INTO info_tbl VALUES (1,'a'),(2,'b'),(NULL,'c');
     
    SELECT id, DEFAULT(id) FROM info_tbl;
    
::

               id   default(id)  
    =============================
                1             0
                2             0  
             NULL             0   
     
.. code-block:: sql

    UPDATE info_tbl SET id = DEFAULT WHERE id IS NULL;
    DELETE FROM info_tbl WHERE id = DEFAULT(id);
    INSERT INTO info_tbl VALUES (DEFAULT,'d');

.. note::

    In version lower than CUBRID 9.0, the value at the time of CREATE TABLE has been saved when the value of the DATE, DATETIME, TIME, TIMESTAMP column has been specified as SYS_DATE, SYS_DATETIME, SYS_TIME, SYS_TIMESTAMP while creating a table. Therefore, to enter the value at the time of data INSERT in version lower than CUBRID 9.0, the function should be entered to the VALUES clause of the INSERT syntax.
    
.. _disk_size:
	
DISK_SIZE
=========

.. function:: DISK_SIZE(expr)

    This function returns the size in bytes required to store the value of *expr* after evaluation. Main usage is to get necessary size for storing values in database heap file.
    
    :param expr: Target expression to get the size.

    :rtype: INTEGER
    
.. code-block:: sql

     SELECT DISK_SIZE('abc'), DISK_SIZE(1);
   
::

       disk_size('abc')   disk_size(1)
    ==================================
                      7              4
                      

The size depends on the actual content of value, :ref:`string compression<string_compression>` is also taken into account:
    
.. code-block:: sql

     CREATE TABLE t1(s1 VARCHAR(10), s2 VARCHAR(300), c1 CHAR(10), c2 CHAR(300));
     INSERT INTO t1 VALUES(REPEAT('a', 10), REPEAT('b', 300), REPEAT('c', 10), REPEAT('d', 300));
     INSERT INTO t1 VALUES('a', 'b', 'c', 'd');
     SELECT DISK_SIZE(s1), DISK_SIZE(s2), DISK_SIZE(c1), DISK_SIZE(c2) FROM t1;
    
::

       disk_size(s1)   disk_size(s2)   disk_size(c1)   disk_size(c2)
    ================================================================
                  12              24              10             300
                   4               4              10             300
                   
    
INDEX_CARDINALITY
=================

.. function:: INDEX_CARDINALITY(table, index, key_pos)

    The **INDEX_CARDINALITY** function returns the index cardinality in a table. The index cardinality is the number of unique values defining the index. The index cardinality can be applied even to the partial key of the multiple column index and displays the number of the unique value for the partial key by specifying the column location with the third parameter. Note that this value is an approximate value.

    If you want the updated result from this function, you should run **UPDATE STATISTICS** statement.
    
    :param table: Table name
    :param index: Index name that exists in the *table*
    :param key_pos: Partial key location. It *key_pos* starts from 0 and has a range that is smaller than the number of columns consisting of keys; that is, the *key_pos* of the first column is 0. For the single column index, it is 0. It can be one of the following types.
    
        *   Character string that can be converted to a numeric type.
        *   Numeric type that can be converted to an integer type. The **FLOAT** or the **DOUBLE** types will be the value converted by the **ROUND** function.

    :rtype: INT

The return value is 0 or a positive integer and if any of the input parameters is **NULL**, **NULL** is returned. If tables or indexes that are input parameters are not found, or *key_pos* is out of range, **NULL** is returned.

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
     
    UPDATE STATISTICS ON t1;
    SELECT INDEX_CARDINALITY('t1','i_t1_i1_s1',0);
    
::

       index_cardinality('t1', 'i_t1_i1_s1', 0)
    ===========================================
                                              2
     
.. code-block:: sql

    SELECT INDEX_CARDINALITY('t1','i_t1_i1_s1',1);
    
::

       index_cardinality('t1', 'i_t1_i1_s1', 1)
    ===========================================
                                              3
     
.. code-block:: sql

    SELECT INDEX_CARDINALITY('t1','i_t1_i1_s1',2);
    
::

       index_cardinality('t1', 'i_t1_i1_s1', 2)
    ===========================================
                                           NULL
     
.. code-block:: sql

    SELECT INDEX_CARDINALITY('t123','i_t1_i1_s1',1);
    
::

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
     
::

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
     
::

       inet_ntoa(3232235530)
    ======================
      '192.168.0.10'

LAST_INSERT_ID
==============

.. function:: LAST_INSERT_ID()

    The **LAST_INSERT_ID** function returns the value that has been most recently inserted to the **AUTO_INCREMENT** column by a single **INSERT** statement. 
    
    :rtype: BIGINT
    
The value returned by the **LAST_INSERT_ID** function has the following characteristics.

*   The latest **LAST_INSERT_ID** value which was INSERTed successfully will be maintained. If it fails to INSERT, there is no change for **LAST_INSERT_ID**\() value, but **AUTO_INCREMENT** value is internally increased. Therefore, **LAST_INSERT_ID**\() value after the next **INSERT** statement's success reflects the internally increased **AUTO_INCREMENT** value.

    .. code-block:: sql

        CREATE TABLE tbl(a INT PRIMARY KEY AUTO_INCREMENT, b INT UNIQUE);
        INSERT INTO tbl VALUES (null, 1);
        INSERT INTO tbl VALUES (null, 1);
        
    ::

        ERROR: Operation would have caused one or more unique constraint violations.

    .. code-block:: sql

        INSERT INTO tbl VALUES (null, 1);
        
    ::
    
        ERROR: Operation would have caused one or more unique constraint violations.

    .. code-block:: sql

        SELECT LAST_INSERT_ID();
        
    ::
    
        1

        -- In 2008 R4.x or before, above value is 3.

    .. code-block:: sql

        INSERT INTO tbl VALUES (null, 2);
        SELECT LAST_INSERT_ID();
        
    ::
    
        4
        
*   In the Multiple-rows **INSERT** statement(INSERT INTO tbl VALUES (), (), ..., ()), **LAST_INSERT_ID**\ () returns the firstly inserted **AUTO_INCREMENT** value. In other words, from the second row, there is no change on **LAST_INSERT_ID**\ () value even if the next rows are inserted.

    .. code-block:: sql
    
        INSERT INTO tbl VALUES (null, 11), (null, 12), (null, 13);    
        SELECT LAST_INSERT_ID();
        
    ::
    
        5
    
    .. code-block:: sql

        INSERT INTO tbl VALUES (null, 21);
        SELECT LAST_INSERT_ID();
        
    ::
    
        8
        
*   If **INSERT** statement succeeds to execute, **LAST_INSERT_ID** () value is not recovered to its previous value even if the transaction is rolled back.

    .. code-block:: sql

        -- csql> ;autocommit off
        CREATE TABLE tbl2(a INT PRIMARY KEY AUTO_INCREMENT, b INT UNIQUE);
        INSERT INTO tbl2 VALUES (null, 1);
        COMMIT;
        
        SELECT LAST_INSERT_ID();
        
    ::
    
        1
        
    .. code-block:: sql
    
        INSERT INTO tbl2 VALUES (null, 2);
        INSERT INTO tbl2 VALUES (null, 3);
        
        ROLLBACK;
        
        SELECT LAST_INSERT_ID();
        
    ::
    
        3

*   **LAST_INSERT_ID**\ () value used from the inside of a trigger cannot be identified from the outside of the trigger.

*   **LAST_INSERT_ID**\ is independently kept by a session of each application.

.. code-block:: sql

    CREATE TABLE ss (id INT AUTO_INCREMENT NOT NULL PRIMARY KEY, text VARCHAR(32));
    INSERT INTO ss VALUES (NULL, 'cubrid');
    SELECT LAST_INSERT_ID ();
     
::

         last_insert_id()
    =======================
                         1
     
.. code-block:: sql

    INSERT INTO ss VALUES (NULL, 'database'), (NULL, 'manager');
    SELECT LAST_INSERT_ID ();
     
::

         last_insert_id()
    =======================
                         2

.. code-block:: sql

    CREATE TABLE tbl (id INT AUTO_INCREMENT);
    INSERT INTO tbl values (500), (NULL), (NULL);
    SELECT LAST_INSERT_ID();
     
::

         last_insert_id()
    =======================
                         1
     
.. code-block:: sql

    INSERT INTO tbl VALUES (500), (NULL), (NULL);
    SELECT LAST_INSERT_ID();
     
::

         last_insert_id()
    =======================
                         3
     
.. code-block:: sql

    SELECT * FROM tbl;
     
::

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
    
::

      list_dbs()
    ======================
      'testdb demodb'

ROW_COUNT
=========

.. function:: ROW_COUNT()

    The **ROW_COUNT** function returns the number of rows updated (**UPDATE**, **INSERT**, **DELETE**, **REPLACE**) by the previous statement. 
    
    ROW_COUNT returns 1 for each inserted row and 2 for each updated row for **INSERT ON DUPLICATE KEY UPDATE** statement. It returns the sum of number of deleted and inserted rows for **REPLACE** statement.
    
    Statements triggered by trigger will not affect the ROW_COUNT for the statement.
        
    :rtype: INT
    
.. code-block:: sql

    CREATE TABLE rc (i int);
    INSERT INTO rc VALUES (1),(2),(3),(4),(5),(6),(7);
    SELECT ROW_COUNT();
    
::

       row_count()
    ===============
                  7
    
.. code-block:: sql

    UPDATE rc SET i = 0 WHERE i >  3;
    SELECT ROW_COUNT();
    
::

       row_count()
    ===============
                  4
     
.. code-block:: sql

    DELETE FROM rc WHERE i = 0;
    SELECT ROW_COUNT();
    
::

       row_count()
    ===============
                  4

SESSIONTIMEZONE
===============

.. function:: SESSIONTIMEZONE()

    Prints out a timezone of session (offset or region name) as a string. (e.g. '-05:00', or 'Europe/Vienna').

.. code-block:: sql

    SELECT SESSIONTIMEZONE();

::

      sessiontimezone
    ======================
      'Asia/Seoul'

.. seealso:: 

    :func:`DBTIMEZONE`, :func:`FROM_TZ`, :func:`NEW_TIME`, :func:`TZ_OFFSET`

USER, SYSTEM_USER
=================

.. function:: USER()

.. function:: SYSTEM_USER()

    The functions **USER** and **SYSTEM_USER** are identical and they return the user name together with the host name where CSQL or CAS was executed. 
    
    The :c:macro:`USER` and :c:macro:`CURRENT_USER` pseudo-columns return the user names who has logged on to the current database as character strings.

    :rtype: STRING

.. code-block:: sql

    --selecting the current user on the session
    SELECT SYSTEM_USER ();
    
::

       user()
    ======================
      'PUBLIC@cubrid_host'
     
.. code-block:: sql

    SELECT USER(), CURRENT_USER;
    
::

       user()                CURRENT_USER
    ============================================
      'PUBLIC@cubrid_host'  'PUBLIC'
     
.. code-block:: sql

    --selecting all users of the current database from the system table
    SELECT name, id, password FROM db_user;
    
::

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
    
::

       version()
    =====================
      '9.1.0.0203'
