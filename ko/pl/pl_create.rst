-----------------------------
저장 프로시저의 생성
-----------------------------

저장 프로시저와 저장 함수는 각각 :ref:`create-procedure`\와 :ref:`create-function`\을 사용하여 등록 할 수 있다.
CUBRID에서는 저장 프로시저와 저장 함수에 대해서 기능에 특별한 차이점은 없으며, 저장 함수의 경우 반환 타입을 정의하지만, 저장 프로시저는 반환 타입은 void로 항상 NULL 값을 반환한다.
따라서 일반적인 경우 사용자의 용도에 따라 저장 프로시저는 :doc:`/sql/query/call` 구문을 사용하여 호출하고, 저장 함수는 그 외 반환 값이 필요한 질의에서 사용한다.

이 장에서는 저장 프로시저와 저장 함수를 생성할 때 고려해야 할 사항에 대하여 자세히 설명한다.

스키마와 이름 지정
======================================

::

    CREATE [OR REPLACE] [PROCEDURE] [schema_name.]procedure_name

    CREATE [OR REPLACE] [FUNCTION] [schema_name.]function_name

저장 프로시저와 저장 함수의 이름은 식별자에 해당하며 :doc:`/sql/identifier`\의 작성 원칙에 따라 작성해야 한다.
이름의 최대 222 바이트 길이로 제한되며, 이 길이를 초과하면 오류가 발생한다.

저장 프로시저와 저장 함수의 이름을 지정할 때 스키마 이름(**schema_name**)을 지정하지 않아도 현재 로그인 된 사용자의 스키마에 저장되어 생성된다. (:doc:`/sql/user_schema`\ 참고)
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

인자의 데이터 타입, 모드, 기본값 지정
======================================

저장 프로시저와 저장 함수에 인자를 지정하면 호출 시 인수를 전달할 수 있다. 인자를 지정할 때에는 인자의 이름과 데이터 타입을 지정해야 하며 최대 **64**\개의 인자를 지정할 수 있다.
저장 함수는 입력 매개 변수를 받아 결과값을 반환하므로 결과 데이터 타입을 지정해야 하며, 저장 프로시저는 결과 데이터 타입을 지정하지 않고 항상 NULL 을 반환하는 void 타입으로 간주한다.
저장 프로시저와 저장 함수의 결과 타입으로 CUBRID SQL이 지원하는 데이터 타입 (:doc:`/sql/datatype`\) 중 일부만을 지원한다. 자세한 내용은 :ref:`pl-supported_sql_type`\을 참고한다.

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

.. _pl-supported_sql_type:

지원하는 인수와 결과 데이터 타입
--------------------------------------

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

.. include:: pl_default_args.rst

.. _pl_function_overloading:

함수 오버로딩
======================================

함수 오버로딩이란 동일한 이름이지만 인자의 타입이나 개수가 다른 함수를 여러 개 정의하는 것을 의미한다.
CUBRID에서는 함수 오버로딩을 지원하지 않는다. 즉, 인수의 갯수나 데이터 타입이 다르더라도 동일한 이름의 저장 프로시저와 저장 함수를 생성할 수 없다.
따라서 **CREATE OR REPLACE** 구문을 사용했을 때, 새로운 프로시저가 생성되지 않고 기존에 등록한 프로시저를 덮어쓰기 때문에 주의가 필요하다.

::

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
