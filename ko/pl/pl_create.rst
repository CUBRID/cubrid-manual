-----------------------------
저장 프로시저의 생성
-----------------------------

저장 프로시저와 저장 함수는 각각 :ref:`create-procedure`\와 :ref:`create-function`\을 사용하여 생성 할 수 있다.
CUBRID에서는 저장 프로시저와 저장 함수에 대해서 기능에 특별한 차이점은 없다. 
유일한 차이점은 저장 함수의 경우 반환 타입을 정의하지만, 저장 프로시저는 반환 타입은 void로 항상 NULL 값을 반환한다.

이 장에서는 저장 프로시저와 저장 함수를 생성할 때 고려해야 할 사항에 대하여 자세히 설명한다.

.. _pl-schema:

스키마와 이름 지정
======================================

::

    CREATE [OR REPLACE] [PROCEDURE] [schema_name.]procedure_name

    CREATE [OR REPLACE] [FUNCTION] [schema_name.]function_name

저장 프로시저와 저장 함수의 이름은 식별자에 해당하며 :doc:`/sql/identifier`\의 작성 원칙에 따라 작성해야 한다.
이름의 최대 222 바이트 길이로 제한되며, 이 길이를 초과하면 오류가 발생한다.

저장 프로시저와 저장 함수의 이름을 지정할 때 스키마 이름(**schema_name**)을 지정하지 않아도 현재 로그인 된 :ref:`사용자의 스키마<user_schema>`\에 저장되어 생성된다.
DBA 권한이 있는 사용자는 다른 사용자의 스키마에 저장 프로시저와 저장 함수를 생성할 수 있지만, 일반 사용자의 경우 자신의 스키마에만 저장 프로시저와 저장 함수를 생성할 수 있다.

만약 일반 사용자가 다른 사용자의 스키마에 저장 프로시저와 저장 함수를 생성하려고 시도하면 에러가 발생한다.

.. code-block:: sql

    -- Login as DBA
    CREATE USER U1;
    CREATE USER U2;

    CALL login ('U1', '') ON CLASS db_user;

    CREATE PROCEDURE u2.test_proc (arg1 INT, arg2 INT)
    AS BEGIN
        DBMS_OUTPUT.put_line (arg1 + arg2);
    END;

::

    ERROR: before ' ; '
    DBA, members of DBA group, and owner can perform CREATE PROCEDURE/FUNCTION.

.. _pl-parameter:

저장 프로시저의 인자 지정
======================================

저장 프로시저와 저장 함수에 인자를 지정하면 호출 시 인수를 전달할 수 있다. 인자를 지정할 때에는 인자의 이름과 데이터 타입을 지정해야 하며 최대 **64**\개의 인자를 지정할 수 있다.

저장 함수는 입력 매개 변수를 받아 결과값을 반환하므로 결과 데이터 타입을 지정해야 하며, 저장 프로시저는 결과 데이터 타입을 지정하지 않고 항상 NULL 을 반환하는 void 타입으로 간주한다.
저장 프로시저와 저장 함수의 결과 타입으로 CUBRID SQL이 지원하는 (:doc:`데이터 타입 </sql/datatype>`\) 중 일부만을 지원한다. 자세한 내용은 :ref:`pl-arg-type-restriction`\을 참고한다.

.. code-block:: sql

    CREATE PROCEDURE test_proc (arg1 INT, arg2 INT)
    AS BEGIN
        DBMS_OUTPUT.put_line (arg1 + arg2);
    END;

    CREATE FUNCTION test_func (arg1 INT, arg2 INT) RETURN INT
    AS BEGIN
        RETURN arg1 + arg2;
    END;

    SELECT test_proc(1, 2);
    SELECT test_func(1, 2);

::

      test_proc(1, 2)
    ======================
      NULL

    <DBMS_OUTPUT>
    ====
    3

      test_func(1, 2)
    =================
                    3

.. _pl-arg-type-restriction:

지원하는 인자와 결과 데이터 타입
==============================================

저장 프로시저와 저장 함수의 인수와 결과 데이터 타입으로 CUBRID SQL이 지원하는 데이터 타입 중 일부 데이터 타입을 명시할 수 있다.
다음의 표는 언어 확장에서 지원하는 데이터 타입이다.

+----------------+-------------------------------------+----------+----------+
|                |                                     | 지원 여부 (O, X)    |
+ 유형           + 타입                                +----------+----------+
|                |                                     | PL/CSQL  | Java SP  |
+================+=====================================+==========+==========+
| 수치           | SHORT, SMALLINT                     | O        | O        |
+                +-------------------------------------+----------+----------+
|                | INTEGER, INT                        | O        | O        |
+                +-------------------------------------+----------+----------+
|                | BIGINT                              | O        | O        |
+                +-------------------------------------+----------+----------+
|                | NUMERIC, DECIMAL                    | O        | O        |
+                +-------------------------------------+----------+----------+
|                | FLOAT, REAL                         | O        | O        |
+                +-------------------------------------+----------+----------+
|                | DOUBLE, DOUBLE PRECISION            | O        | O        |
+----------------+-------------------------------------+----------+----------+
| 날짜/시간      | DATE, TIME, TIMESTAMP, DATETIME     | O        | O        |
+                +-------------------------------------+----------+----------+
|                | TIMESTAMPLTZ, TIMESTAMPTZ           | X        | X        |
|                | DATETIMELTZ, DATETIMETZ             |          |          |
+----------------+-------------------------------------+----------+----------+
| 문자열         | CHAR, VARCHAR, STRING, CHAR VARYING | O        | O        |
+----------------+-------------------------------------+----------+----------+
| 컬렉션         | SET, MULTISET, LIST, SEQUENCE       | X        | O        |
+----------------+-------------------------------------+----------+----------+
| 기타           | BIT, BIT VARYING                    | X        | X        |
+                +-------------------------------------+----------+----------+
|                | ENUM                                | X        | X        |
+                +-------------------------------------+----------+----------+
|                | BLOB/CLOB                           | X        | X        |
+                +-------------------------------------+----------+----------+
|                | JSON                                | X        | X        |
+                +-------------------------------------+----------+----------+
|                | CURSOR                              | X        | O*       |
+----------------+-------------------------------------+----------+----------+

* Java SP에서는 CURSOR 타입에 대해서 반환 타입만을 지원하며, 인수 타입으로는 지원하지 않는다.

지원하지 않는 데이터 타입을 사용해서 저장 프로시저를 생성하면 다음과 같은 오류가 발생한다.

.. code-block:: sql
        
        CREATE FUNCTION unsupported_json() RETURN JSON 
        AS BEGIN RETURN NULL; END;

        CREATE PROCEDURE unsupproted_args (arg TIMESTAMPLTZ) 
        AS BEGIN NULL; END;

::

        ERROR: Unsupported return type 'json' of the stored procedure

        ERROR: before ' ) 
        AS BEGIN NULL; END; '
        Unsupported argument type 'timestampltz' of the stored procedure

.. _pl-arg-mode:

인자의 모드 지정
============================================

인자의 모드에는 IN, OUT, IN OUT 모드가 있다. 인자를 지정할 때 모드를 지정하지 않으면 기본적으로 IN 모드로 간주한다.

저장 프로시저와 저장 함수에서 **CALL**\ 구문과 OUT 또는 IN OUT 인자를 사용하여 저장 프로시저와 저장 함수에서 변경한 인자의 값을 다시 반환할 수 있다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_proc (arg1 OUT INT)
    AS BEGIN
        arg1 := 5;
    END;


    CREATE OR REPLACE FUNCTION test_func (arg1 IN OUT INT) RETURN INT
    AS BEGIN
        arg1 := arg1 + 1;
        RETURN arg1 + 5;
    END;

.. code-block:: sql

    SELECT 1 into :a;
    select :a;
    CALL test_proc(:a);
    select :a;

::

    -- select :a;

               :a
    =============
                1

    -- select :a;

               :a
    =============
                5

만약 저장 프로시저와 함수의 어떠한 인자에 OUT 또는 IN OUT 모드를 지정하는 경우, **CALL**\이 아닌 질의문 (**SELECT**, **UPDATE**, **DELETE** 등) 내에서 호출할 수 없으며 에러를 반환한다.

.. code-block:: sql

    SELECT test_func(1);

::

    In the command from line 1,
    ERROR: Semantic: Stored procedure/function 'u1.test_func' has OUT or IN OUT arguments

.. _pl-arg-default:

기본값 인자 지정
============================================

저장 프로시저와 저장 함수의 인자에 기본값을 지정할 수 있다.

* **:=** 또는 **DEFAULT** 키워드를 사용하여 기본값을 지정할 수 있다.
* 기본값을 지정하면 저장 프로시저와 저장 함수의 인수를 생략하고 호출할 수 있다. 생략된 인수는 기본값으로 대체된다.
* 기본값은 리터럴 값으로 지정할 수 있으며, 255 바이트 이하의 문자열 값으로 저장한다. 이 때 크기를 초과하면 오류가 발생한다.
* 기본값에 리터럴 값 외에 허용하는 함수는 다음과 같다.

+-------------------------------+---------------+
| 기본값                        | 데이터 타입   |
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

    위의 표에 명시되지 않은 함수를 **DEFAULT** 값으로 사용하는 경우, 프로시저 생성 시점에 평가된 결과를 **DEFAULT** 값으로 저장한다.
    따라서 **INSERT** 시 해당 칼럼의 기본 값은 **INSERT** 수행시 함수 수행 값이 아닌, 프로시저 생성 시점에 수행된 결과 값을 사용한다.
    저장 함수인 경우 **DEFAULT** 값으로 사용할 수 없으므로 오류를 반환한다.

.. code-block:: sql

    CREATE FUNCTION fn_int (a int) return int as begin return a; end;
    
    -- error
    CREATE FUNCTION default_val_func_test (id INT DEFAULT fn_int (1)) return int as begin return id; end;

::

    ERROR: before ' ); '
    'fn_int(1)' function can not be used in DEFAULT clause.

다음은 기본값에 리터럴 값을 지정하는 간단한 예시이다.

.. code-block:: sql

    CREATE FUNCTION default_args (
            a INT := 1, 
            b INT DEFAULT 2
    ) RETURN INT
    AS BEGIN RETURN a + b; END;

    SELECT default_args(); 
    SELECT default_args(3);
    SELECT default_args(3, 4);

::

      default_args()
    ================
                    3

      default_args(3)
    =================
                    5

      default_args(3, 4)
    ====================
                    7

다음은 기본값에 함수를 지정하는 예시이다.

.. code-block:: sql

    CREATE FUNCTION get_age (
        birth DATE DEFAULT DATE'2000-01-01',
        today DATE DEFAULT SYS_DATE
    ) RETURN INT
    AS
    BEGIN
        RETURN YEAR(today) - YEAR(birth)
            - CASE WHEN TO_CHAR(today, 'MMDD') < TO_CHAR(birth, 'MMDD') THEN 1 ELSE 0 END;
    END;

    SELECT get_age();
    SELECT get_age(DATE'2000-05-10');
    SELECT get_age(DATE'2000-05-10', DATE'2025-03-24');

::        

    get_age()
    =============
               25

    get_age(date '2000-05-10')
    ============================
                              24

    get_age(date '2000-05-10', date '2025-03-24')
    ===============================================
                                                 24

.. _pl_function_overloading:

함수 오버로딩
======================================

함수 오버로딩이란 동일한 이름이지만 인자의 타입이나 개수가 다른 함수를 여러 개 정의하는 것을 의미한다.
CUBRID에서는 **함수 오버로딩을 지원하지 않는다.** 즉, 인수의 갯수나 데이터 타입이 다르더라도 동일한 이름의 저장 프로시저와 저장 함수를 생성할 수 없다.
따라서 **CREATE OR REPLACE** 구문을 사용했을 때, 새로운 프로시저가 생성되지 않고 기존에 등록한 프로시저를 덮어쓰기 때문에 주의가 필요하다.

.. code-block:: sql

    CREATE FUNCTION test_func (arg1 INT) RETURN INT AS
    BEGIN
        RETURN arg1;
    END;

    SELECT test_func (1);

    CREATE OR REPLACE FUNCTION test_func (arg1 INT, arg2 INT) RETURN INT AS
    BEGIN
        RETURN arg1 + arg2;
    END;

    SELECT test_func (1); -- error
    SELECT test_func (1, 2); -- 3

::
    
    ERROR: Parameter count is invalid. expected: 2, actual: 1

      test_func(1, 2)
    =================
                    3

.. _pl_object_dependencies:

객체 의존성
======================================

저장 프로시저와 저장 함수가 내부적으로 참조하는 데이터베이스 객체에 대한 의존성은 PL/CSQL에서 등록 시에만 확인한다.
따라서 PL/CSQL 프로시저에서 참조하는 테이블, 뷰, 시퀀스, 인덱스 등의 객체가 존재하지 않는 경우 프로시저 등록 시 오류가 발생한다.

.. code-block:: sql

    DROP TABLE IF EXISTS test_tbl;

    CREATE OR REPLACE FUNCTION test_func () RETURN INT
        k INT;
    AS
    BEGIN
        SELECT id INTO k FROM test_tbl LIMIT 1;
        RETURN k;
    END;

::

    ERROR: Stored procedure compile error: Syntax: before '  LIMIT 1'
    Unknown class "dba.test_tbl". select id from [dba.test_tbl] limit 1

다음 예제에서 test_tbl 테이블을 생성하는 경우 올바르게 생성된다.

.. code-block:: sql

    DROP TABLE IF EXISTS test_tbl;
    CREATE TABLE test_tbl (id INT);
    INSERT INTO test_tbl VALUES (1);

    CREATE OR REPLACE FUNCTION test_func () RETURN INT
    AS
        k INT;
    BEGIN
        SELECT id INTO k FROM test_tbl LIMIT 1;
        RETURN k;
    END;

    SELECT test_func();

::

      test_func()
    =============
                1

단, PL/CSQL의 Dynamic SQL 이나 Java SP의 경우 저장된 문자열을 런타임에 실행하기 때문에 
프로시저 등록 시 확인하지 않고, 프로시저 호출 시 확인된다.

.. code-block:: sql

    DROP TABLE IF EXISTS test_tbl;

    CREATE OR REPLACE FUNCTION test_func () RETURN INT
    AS
        k INT;
    BEGIN
        execute immediate 'SELECT id FROM test_tbl LIMIT 1' INTO k;
        RETURN k;
    END;

    SELECT test_func();

::

    ERROR: Stored procedure execute error: 
      (line 5, column 9) Syntax: before '  LIMIT 1'
    Unknown class "dba.test_tbl". select id from [dba.test_tbl] limit 1

저장 프로시저와 저장 함수가 내부적으로 참조하는 객체가 삭제되는 경우에는 시스템에서 의존성을 확인하지 않으므로
올바르게 등록되더라도 프로시저 호출 시 오류가 발생할 수 있다.
따라서 저장 프로시저와 저장 함수가 참조하는 객체가 삭제되지 않도록 주의해야 한다.

.. code-block:: sql

    DROP TABLE IF EXISTS test_tbl;
    CREATE TABLE test_tbl (id INT);
    INSERT INTO test_tbl VALUES (1);

    CREATE OR REPLACE FUNCTION test_func () RETURN INT
    AS
        k INT;
    BEGIN
        SELECT id INTO k FROM test_tbl LIMIT 1;
        RETURN k;
    END;

    DROP TABLE IF EXISTS test_tbl;
    SELECT test_func();

::

    ERROR: Stored procedure execute error: 
      (line 5, column 9) Syntax: Unknown class "dba.test_tbl". select [dba.test_tbl].id from [dba.test_tbl] [dba.test_tbl] ...
