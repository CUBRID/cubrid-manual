*********
정보 함수
*********

CHARSET
=======

.. function:: CHARSET(expr)

    *expr* 의 문자셋을 반환한다.
    
    :param expr: 문자셋을 구할 대상 표현식
    
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
    
    *expr*\ 의 콜레이션 변환도(coercibility)를 반환한다. 콜레이션 변환도는 칼럼(표현식)들이 서로 다른 콜레이션과 문자셋을 가지고 있을 때 어떤 콜레이션과 문자셋으로 변환할 것인지를 결정한다. 어떤 연산을 수행하는 두 개의 칼럼(표현식)이 있을 때, 높은 변환도를 가진 인자는 더 낮은 변환도를 가진 인자의 콜레이션으로 변환된다. 이와 관련하여 :ref:`콜레이션 변환도 <collation-coercibility>`\ 를 참고한다.

    :param expr: 콜레이션 변환도를 구할 대상 표현식

    :rtype: INT
    
.. code-block:: sql

    SELECT COERCIBILITY(USER());
    7
    
    SELECT COERCIBILITY(_utf8'abc');
    10

COLLATION
=========

.. function:: COLLATION(expr)

    *expr*\ 의 콜레이션을 반환한다.
    
    :param expr: 콜레이션을 구할 대상 표현식

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

    **CURRENT_USER**\ 와 **USER** 의사 컬럼(pseudo column)은 동일하며, 현재 데이터베이스에 로그인한 사용자의 이름을 문자열로 반환한다.

    기능이 비슷한 :func:`USER` 함수와 :func:`SYSTEM_USER` 함수는 사용자 이름을 호스트 이름과 함께 반환한다.

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

    **DATABASE** 함수와 **SCHEMA** 함수는 동일하며, 현재 연결된 데이터베이스 이름을 **VARCHAR** 타입의 문자열로 반환한다.

    :rtype: STRING
    
.. code-block:: sql

    SELECT DATABASE(), SCHEMA();
    
       database()            schema()
    ============================================
      'demodb'              'demodb'

DEFAULT
=======

.. function:: DEFAULT(column_name)
.. c:macro:: DEFAULT

    **DEFAULT**\ 와 **DEFAULT** 함수는 칼럼에 정의된 기본값을 반환한다. 해당 칼럼에 기본값이 지정되지 않으면 **NULL** 또는 에러를 출력한다. **DEFAULT**\ 는 인자가 없는 반면, **DEFAULT** 함수는 칼럼 이름을 입력 인자로 하는 차이가 있다. **DEFAULT**\ 는 **INSERT** 문의 입력 데이터, **UPDATE** 문의 **SET** 절에서 사용될 수 있고, **DEFAULT** 함수는 모든 곳에서 사용될 수 있다.

    기본값이 정의되지 않은 칼럼에 어떠한 제약 조건이 정의되어 있지 않거나 **UNIQUE** 제약 조건이 정의된 경우에는 **NULL**\ 을 반환하고, 해당 칼럼에 **NOT NULL** 또는 **PRIMARY KEY** 제약 조건이 정의된 경우에는 에러를 반환한다.

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

    CUBRID 9.0 미만 버전에서는 테이블 생성 시 DATE, DATETIME, TIME, TIMESTAMP 칼럼의 DEFAULT 값을 SYS_DATE, SYS_DATETIME, SYS_TIME, SYS_TIMESTAMP로 지정하면, CREATE TABLE 시점의 값이 저장되었다. 따라서 CUBRID 9.0 미만 버전에서 데이터가 INSERT되는 시점의 값을 입력하려면 INSERT 구문의 VALUES 절에 해당 함수를 입력해야 했다.
    
INDEX_CARDINALITY
=================

.. function:: INDEX_CARDINALITY(table, index, key_pos)

    **INDEX_CARDINALITY** 함수는 테이블에서 인덱스 카디널리티(cardinality)를 반환한다. 인덱스 카디널리티는 인덱스를 정의하는 고유한 값의 개수이다. 인덱스 카디널리티는 다중 칼럼 인덱스의 부분 키에 대해서도 적용할 수 있는데, 이때 세 번째 인자로 칼럼의 위치를 지정하여 부분 키에 대한 고유 값의 개수를 나타낸다.

    :param table: 테이블 이름
    :param index: *table* 내에 존재하는 인덱스 이름
    :param key_pos: 부분 키의 위치. *key_pos* 는 0부터 시작하여 키를 구성하는 칼럼 개수보다 작은 범위를 갖는다. 즉, 첫 번째 칼럼의 *key_pos* 는 0이다. 단일 칼럼 인덱스의 경우에는 0이다. 다음 타입 중 하나가 될 수 있다.
    
        * 숫자형 타입으로 변환할 수 있는 문자열. 
        * 정수형으로 변환할 수 있는 숫자형 타입. FLOAT나 DOUBLE 타입은 ROUND 함수로 변환한 값이 된다.

    :rtype: INT
    
리턴 값은 0 또는 양의 정수이며, 입력 인자 중 하나라도 **NULL** 이면 **NULL** 을 반환한다. 입력 인자인 테이블이나 인덱스가 발견되지 않거나 *key_pos* 가 지정된 범위를 벗어나면 **NULL** 을 리턴한다.

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

    **INET_ATON** 함수는 IPv4 주소의 문자열을 입력받아 이에 해당하는 숫자를 반환한다. 'a.b.c.d' 형식의 IP 주소 문자열을 입력하면 "a * 256 ^ 3 + b * 256 ^ 2 + c * 256 + d"가 반환된다. 반환 타입은 **BIGINT** 이다.

    :param ip_string: IPv4 주소 문자열
    :rtype: BIGINT

다음 예제에서 192.168.0.10은 "192 * 256 ^ 3 + 168 * 256 ^ 2 + 0 * 256 + 10"으로 계산된다.

.. code-block:: sql

    SELECT INET_ATON('192.168.0.10');
     
       inet_aton('192.168.0.10')
    ============================
                      3232235530

INET_NTOA
=========

.. function:: INET_NTOA( expr )

    **INET_NTOA** 함수는 숫자를 입력받아 IPv4 주소 형식의 문자열을 반환한다. 반환 타입은 **VARCHAR** 이다.

    :param expr: 숫자 표현식
    :rtype: STRING

.. code-block:: sql

    SELECT INET_NTOA(3232235530);
     
       inet_ntoa(3232235530)
    ======================
      '192.168.0.10'

LAST_INSERT_ID
==============

.. function:: LAST_INSERT_ID()

    **LAST_INSERT_ID** 함수는 하나의 **INSERT** 문에 의해 **AUTO_INCREMENT** 칼럼에 가장 최근에 삽입된 값을 반환한다.
    
    :rtype: BIGINT
    
**LAST_INSERT_ID** 함수가 반환하는 값은 다음의 특징을 가진다. 

*   성공적으로 **INSERT** 된 값이 없을 때에는 가장 최근에 성공한 값이 유지된다.
*   수행 중인 SQL 문은 **LAST_INSERT_ID** () 값에 영향을 주지 않는다.
*   다중 행 **INSERT** 문(예: INSERT INTO tbl VALUES (), (), ..., ())에서 **LAST_INSERT_ID** ()는 첫 번째로 입력된 **AUTO_INCREMENT** () 값을 반환한다.
*   롤백해도 **LAST_INSERT_ID** () 값은 트랜잭션 이전의 **LAST_INSERT_ID** () 값으로 복구되지 않는다.
*   트리거 내에서 사용한 **LAST_INSERT_ID** () 값은 트리거 밖에서 확인할 수 없다.
*   **LAST_INSERT_ID** 는 각 응용 클라이언트의 연결마다 독립적으로 유지된다.

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

    **LIST_DBS** 함수는 디렉터리 파일(**$CUBRID_DATABASES/databases.txt**)에 존재하는 모든 데이터베이스 리스트를 공백 문자로 구분하여 출력한다.

    :rtype: STRING
        
.. code-block:: sql

    SELECT LIST_DBS();
    
      list_dbs()
    ======================
      'testdb demodb'

ROW_COUNT
=========

.. function:: ROW_COUNT()

    **ROW_COUNT** 함수는 가장 마지막에 수행된 **UPDATE**, **INSERT**, **DELETE**, **REPLACE** 문에 영향을 받는 행의 개수를 정수로 반환한다. 
    
    **INSERT ... ON DUPLICATE KEY UPDATE** 문에 의해 INSERT가 수행되면 1, UPDATE가 수행되면 2를 반환한다.
    
    **REPLACE** 문을 수행하면 DELETE와 INSERT를 합한 개수를 반환한다. 
    
    **UPDATE**, **INSERT**, **DELETE** 문에 의해 호출되는 트리거에는 영향을 받지 않으며, 트리거 내에 **UPDATE**, **INSERT**, **DELETE** 문이 포함되어 있어도 영향을 받지 않는다.

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

    **USER** 함수와 **SYSTEM_USER** 함수는 동일하며, 사용자 이름을 호스트 이름과 함께 반환한다.

    기능이 비슷한 :c:macro:`USER`\ 와 :c:macro:`CURRENT_USER` 의사컬럼은 현재 데이터베이스에 로그인한 사용자의 이름을 문자열로 반환한다.

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

    CUBRID 서버 버전을 나타내는 버전 문자열을 반환한다.

    :rtype: STRING

.. code-block:: sql

    SELECT VERSION();
    
       version()
    =====================
      '9.1.0.0203'
