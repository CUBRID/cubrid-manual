
---------------
개요
---------------

.. _stored_proc:

저장 프로시저/함수 생성
==============================

PL/CSQL은 저장 프로시저나 저장 함수를 생성하는데 사용된다.
다음 문법을 따르는 CREATE PROCEDURE 문과 CREATE FUNCTION 문의 AS (또는 IS) 키워드 뒤에 PL/CSQL 코드를 써서
생성하고자 하는 저장 프로시저/함수의 동작을 기술한다.

::

    <create_procedure> ::=
        CREATE [ OR REPLACE ] PROCEDURE [schema_name.]<identifier> [ ( <seq_of_parameters> ) ]
        { IS | AS } [ LANGUAGE PLCSQL ] [ <seq_of_declare_specs> ] <body> ;
    <create_function> ::=
        CREATE [ OR REPLACE ] FUNCTION [schema_name.]<identifier> [ ( <seq_of_parameters> ) ] RETURN <type_spec>
        { IS | AS } [ LANGUAGE PLCSQL ] [ <seq_of_declare_specs> ] <body> ;

위 문법에서 저장 프로시저/함수의 *body*\는 PL/CSQL 실행문들을 포함하고
그 앞의 선언부 *seq_of_declare_specs*\는 실행문들 안에서 사용될 변수, 상수, Exception 등을 선언한다.
이들 문법 요소에 대한 자세한 내용은 :doc:`선언문 <plcsql_decl>`\과 :doc:`실행문 <plcsql_stmt>` 절을 참고한다.

저장 프로시저/함수는 Auto Commit 기능이 언제나 비활성화 된 상태로 실행된다.
이는 호출한 세션에서 Auto Commit 기능이 활성화 되어 있어도 마찬가지이다.

저장 프로시저/함수는 :ref:`큐브리드 내장 함수 <operators-and-functions>`\와 동일한 이름을 가질 수 없다.
동일한 이름으로 선언하면 컴파일 과정에서 (CREATE 문 실행 과정에서) 에러가 발생한다.

다음은 PL/CSQL을 사용해서 작성한 저장 프로시저/함수의 예이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE insert_athlete(
        p_name VARCHAR,
        p_gender VARCHAR,
        p_nation_code VARCHAR,
        p_event VARCHAR)
    AS
    BEGIN
        INSERT INTO athlete (name, gender, nation_code, event)
        VALUES (p_name, p_gender, p_nation_code, p_event);

        COMMIT;
    EXCEPTION
        WHEN OTHERS THEN
            ROLLBACK;
    END;

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE delete_athlete(c INTEGER)
    AS
        n_deleted INTEGER;
    BEGIN
        DELETE FROM athlete
        WHERE code = c;

        n_deleted := SQL%ROWCOUNT;   -- number of deleted rows
        DBMS_OUTPUT.put_line(n_deleted || ' rows deleted');
    EXCEPTION
        WHEN OTHERS THEN
            DBMS_OUTPUT.put_line('exception occurred');
    END;

.. code-block:: sql

    CREATE OR REPLACE FUNCTION fibonacci(n INTEGER) RETURN INTEGER
    IS
        invalid_input EXCEPTION;
    BEGIN
        IF n <= 0 THEN
            RAISE invalid_input;
        END IF;

        IF n = 1 OR n = 2 THEN
            RETURN 1;
        ELSE
            RETURN fibonacci(n-1) + fibonacci(n-2);
        END IF;
    EXCEPTION
        WHEN invalid_input THEN
            DBMS_OUTPUT.put_line('invalid input: ' || n);
            RETURN -1;
        WHEN OTHERS THEN
            DBMS_OUTPUT.put_line('unknown exception');
            RETURN -1;
    END;

위 예제들에서 DBMS_OUTPUT.put_line() 문은 인자로 주어진 문자열을 서버의 DBMS_OUTPUT 버퍼에 저장한다.
인자가 문자열 타입이 아닐 때는 문자열로 형변환한 값을 저장한다.
DBMS_OUTPUT 버퍼에 저장된 문자열 메시지들은 CSQL에서 세션 명령어 ;server-output on을 실행해서 확인할 수 있다.
자세한 내용은 :ref:`CSQL 세션명령어 server-output <server-output>`\을 참조한다.

CREATE PROCEDURE/FUNCTION 문을 실행하면 저장 프로시저/함수의 문법과 실행 의미에 관련된 각종 규칙들을 검사한다.
검사에서 오류가 발견되면 발생된 위치와 원인을 설명하는 오류 메세지를 출력한다.
다음은 오류를 가지고 있는 저장 프로시저가 CSQL에서 에러를 발생시키는 예이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE athlete_code(p_name VARCHAR) AS
    BEGIN
         -- 오류: Static SQL SELECT 문은 INTO 절을 가져야 함
        SELECT code
        FROM athlete a
        WHERE a.name = p_name;
    END;

    ERROR: In line 4, column 5
    Stored procedure compile error: SELECT statement must have an INTO clause

    0 command(s) successfully processed.

.. _static_sql:

Static SQL
==================

SQL 구문 중 다음 구문들은 PL/CSQL 실행문으로 직접 사용할 수 있으며, 이를 Static SQL 문이라고 부른다.

* SELECT (CTE, UNION, INTERSECT, MINUS 포함)
* INSERT, UPDATE, DELETE, MERGE, REPLACE
* COMMIT, ROLLBACK
* TRUNCATE

이들의 자세한 문법과 의미는 :ref:`CUBRID SQL <cubrid_sql>`\을 참고하도록 한다.
위 목록에 포함되지 않는 다른 SQL 문들은 직접 사용할 수는 없으나,
아래에서 설명하는 Dynamic SQL 문을 써서 실행할 수 있다.

SELECT 문은 실행문으로 사용될 뿐만 아니라 :ref:`커서를 선언 <cursor_decl>`\할 때나
:ref:`OPEN-FOR <cursor_manipulation>` 문에도 사용된다.
SELECT 문의 INTO 절에 프로그램의 변수나 OUT 인자를 사용하여 조회 결과를 담을 수 있다.
이 때 조회 결과 값들의 개수는 INTO 절 안의 변수나 OUT 인자의 개수와 일치해야 하며
값들은 대응되는 변수나 OUT 인자에 대입 가능한 타입을 가져야 한다.
SELECT 문을 실행문으로 사용할 때는 INTO 절을 반드시 포함해야 하는 반면
SELECT 문을 :ref:`커서 선언 <cursor_decl>`\이나 :ref:`OPEN-FOR <cursor_manipulation>` 문에서
사용할 때는 INTO 절을 포함하지 않아야 한다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_into_clause_1
    AS
        h int;
        s varchar(10);
        CURSOR c IS SELECT host_year, score INTO h, s FROM history;     -- Error: INTO clause
    BEGIN
        ...
    END;

    CREATE OR REPLACE PROCEDURE test_into_clause_2
    AS
        h int;
        s varchar(10);
        r SYS_REFCURSOR;
    BEGIN
        OPEN r FOR SELECT host_year, score INTO h, s FROM history;      -- Error: INTO clause
        ...
    END;

    CREATE OR REPLACE PROCEDURE test_into_clause_3
    AS
    BEGIN
        SELECT host_year, score FROM history WHERE event_code = 20023;  -- Error: no INTO clause
        ...
    END;

INTO 절을 포함안 SELECT 문의 조회 결과는 한 건의 결과 레코드여야 하며, 두건 이상의 결과를 가져오는 경우는 TOO_MANY_ROWS Exception이 발생한다.
결과가 없을 경우에는 NO_DATA_FOUND Exception이 발생한다.

Static SQL 문의 WHERE 절이나 VALUES 절 안에서처럼 값을 필요로 하는 자리에
PL/CSQL에서 선언한 변수, 상수, 프로시저/함수 인자를 쓸 수 있다.
단, 이들은 BOOLEAN이나 SYS_REFCURSOR 타입을 가져서는 안된다. :ref:`SQL 데이터타입 <datatype_index>`\이
이들을 포함하지 않기 때문이다.

다음은 Static SQL 사용 예이다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION get_medal_count(p_name VARCHAR, p_medal CHAR) RETURN INTEGER
    AS
        n INTEGER;
    BEGIN
        -- 일반 실행문으로서의 SELECT 문
        SELECT COUNT(medal)
        INTO n
        FROM athlete a, record r
        WHERE a.code = r.athlete_code   /* 조인 조건 */
        AND a.name = p_name AND r.medal = p_medal;    /* 필터 조건 */

        RETURN n;
    END;

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE athlete_history(p_name VARCHAR)
    AS
    BEGIN
        -- For 루프 안에서의 SELECT 문
        FOR r IN (SELECT host_year, score FROM history WHERE athlete = p_name) LOOP
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);
        END LOOP;
    END;

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE athlete_history(p_name VARCHAR)
    AS
        -- 커서 정의에서의 SELECT 문
        CURSOR my_cursor IS
        SELECT host_year, score
        FROM history
        WHERE athlete = p_name;
    BEGIN
        FOR r IN my_cursor LOOP
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);
        END LOOP;
    END;

Static SQL 실행 중에 에러가 나면 SQL_ERROR Exception이 발생한다.

.. _dyn_sql:

Dynamic SQL
==================

Dynamic SQL은 실행 시간에 SQL 구문에 해당하는 문자열을 만들어
:ref:`EXECUTE IMMEDIATE <exec_imme>` 문으로 실행하는 방식이다.
Dynamic SQL은 주로 다음 두 가지 경우에 필요하다.

* 실행하려는 SQL 구문을 프로그램 작성 시에 결정하는 것이 어렵거나 불가능한 경우
* DDL 문처럼 Static SQL이 지원하지 않는 구문을 실행해야 할 경우

아래 예제에서 새 테이블 이름은 프로시저 인자를 포함하므로 프로그램 작성 시에 결정할 수 없고
프로그램 실행 시간에야 결정되는 값이다.
그리고, DROP TABLE 문과 CREATE TABLE 문은 Static SQL 기능에서 지원하지 않는 DDL 문이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE collect_athlete_history(p_name VARCHAR)
    AS
        new_table VARCHAR := p_name || '_history';
    BEGIN
        EXECUTE IMMEDIATE 'drop table if exists ' || new_table;
        EXECUTE IMMEDIATE 'create table ' || new_table || ' like history';
        EXECUTE IMMEDIATE 'insert into ' || new_table || ' select * from history where athlete = ?'
        USING p_name;
    END;

작성 규칙
==================

식별자, 예약어, 주석, 리터럴을 작성할 때 :ref:`Static <static_sql>`/:ref:`Dynamic <dyn_sql>`
SQL 안에서는 :ref:`SQL의 작성 규칙 <lexical_rules>`\을 따른다.

Static/Dynamic SQL 밖의 PL/CSQL 문 작성 규칙도 대체로 같은 규칙을 따르지만 다음 몇 가지 예외가 있다.

* SQL과 달리 식별자에 '#'을 쓸 수 없다. 즉, 식별자는 영문 대소문자, 한글, 숫자, '_'(underscore)로만 이루어져야 한다.
* 큰따옴표, 대괄호, 백틱 부호로 둘러싸더라도 식별자에 특수 문자를 쓸 수 없다.
  영문 대소문자, 한글, 숫자, '_'(underscore)만 사용 가능하다.
* 비트열 리터럴을 사용할 수 없다.

.. rubric:: 허용되는 식별자의 예

::

    a
    a_b
    athleteName2
    "select"        -- " "로 둘러싸인 예약어

.. rubric:: 허용되지 않는 식별자의 예

::

    1a              -- 숫자로 시작
    a@b             -- 특수문자
    athlete-name-2  -- 특수문자
    [a@b]           -- [ ]로 둘러싸더라도 특수문자 불가
    select          -- 예약어

PL/CSQL의 예약어는 아래 표에 나열되어 있다.
Static/Dynamic SQL 밖의 PL/CSQL 문에서 아래 표의 단어들을 변수, 상수, Exception, 내부 프로시저/함수
등의 이름을 나타내는 식별자로 쓸 수 없다.
단, SQL 문에서처럼 큰따옴표(" "), 대괄호([ ]), 백틱(\` \`)으로 감싸면 식별자로 쓸 수 있다.

+---------------------------------------------------------------------------------------+
|   AND, AS, AUTONOMOUS_TRANSACTION                                                     |
+---------------------------------------------------------------------------------------+
|   BEGIN, BETWEEN, BIGINT, BOOLEAN, BY                                                 |
+---------------------------------------------------------------------------------------+
|   CASE, CHAR, CHARACTER, CLOSE, COMMENT, COMMIT, CONSTANT, CONTINUE, CREATE, CURSOR   |
+---------------------------------------------------------------------------------------+
|   DATE, DATETIME, DATETIMELTZ, DATETIMETZ, DBMS_OUTPUT, DEC, DECIMAL, DECLARE,        |
|   DEFAULT, DELETE, DIV, DOUBLE                                                        |
+---------------------------------------------------------------------------------------+
|   ELSE, ELSIF, END, ESCAPE, EXCEPTION, EXECUTE, EXIT                                  |
+---------------------------------------------------------------------------------------+
|   FALSE, FETCH, FLOAT, FOR, FUNCTION                                                  |
+---------------------------------------------------------------------------------------+
|   IF, IMMEDIATE, IN, INOUT, INSERT, INT, INTEGER, INTO, IS                            |
+---------------------------------------------------------------------------------------+
|   LANGUAGE, LIKE, LIST, LOOP                                                          |
+---------------------------------------------------------------------------------------+
|   MERGE, MOD, MULTISET                                                                |
+---------------------------------------------------------------------------------------+
|   NOT, NULL, NUMERIC                                                                  |
+---------------------------------------------------------------------------------------+
|   OF, OPEN, OR, OUT                                                                   |
+---------------------------------------------------------------------------------------+
|   PLCSQL, PRAGMA, PRECISION, PROCEDURE                                                |
+---------------------------------------------------------------------------------------+
|   RAISE, REAL, REPLACE, RETURN, REVERSE, ROLLBACK                                     |
+---------------------------------------------------------------------------------------+
|   SEQUENCE, SELECT, SET, SETEQ, SETNEQ, SHORT, SMALLINT, SQL, SQLCODE, SQLERRM,       |
|   STRING, SUBSET, SUBSETEQ, SUPERSET, SUPERSETEQ, SYS_REFCURSOR                       |
+---------------------------------------------------------------------------------------+
|   THEN, TIME, TIMESTAMP, TIMESTAMPLTZ, TIMESTAMPTZ, TRUE, TRUNCATE                    |
+---------------------------------------------------------------------------------------+
|   UPDATE, USING                                                                       |
+---------------------------------------------------------------------------------------+
|   VARCHAR, VARYING                                                                    |
+---------------------------------------------------------------------------------------+
|   WHEN, WHILE, WITH, WORK                                                             |
+---------------------------------------------------------------------------------------+
|   XOR                                                                                 |
+---------------------------------------------------------------------------------------+

위에서 AUTONOMOUS_TRANSACTION은 향후 추가할 기능을 위해서 미리 포함되어 있는 예약어이다.

.. _types:

데이터 타입
==================

Static/Dynamic SQL에서는 SQL에서 제공하는 모든 :ref:`데이터 타입 <datatype_index>`\을 사용할 수 있다.

반면, Static/Dynamic SQL 밖의 PL/CSQL 문에서 사용할 수 있는 데이터 타입은
BOOLEAN, SYS_REFCURSOR와 SQL에서 제공하는 데이터 타입 중 일부이다.

* BOOLEAN: TRUE, FALSE, NULL을 값으로 가질 수 있다.
  SQL에서 BOOLEAN 타입을 지원하지 않기 때문에 CREATE PROCEDURE/FUNCTION 문에서 인자 타입이나 리턴 타입으로 BOOLEAN을 사용할 수는 없다.
  단, :ref:`내부 프로시저/함수 <local_routine_decl>`\를 선언할 때는 인자 타입이나 리턴 타입으로
  BOOLEAN을 사용할 수 있다.
* SYS_REFCURSOR: 커서 변수를 선언할 때 사용한다.
  커서 변수의 용도는 :ref:`OPEN-FOR <cursor_manipulation>` 문을 참고한다.
  BOOLEAN과 마찬가지로 CREATE PROCEDURE/FUNCTION 문에서 인자 타입이나 리턴 타입으로 SYS_REFCURSOR를 사용할 수 없고
  :ref:`내부 프로시저/함수 <local_routine_decl>`\에는 사용할 수 있다.

SQL에서 제공하는 데이터 타입 중 PL/CSQL에서 지원하는 것과 지원하지 않는 것은 다음과 같다.
(단, 위에서 언급한 대로 Static/Dynamic SQL에서는 SQL에서 제공하는 모든 데이터 타입을 쓸 수 있다.)

+----------------+-------------------------------------+----------------------------------+
| 유형           | 지원                                | 미지원                           |
+================+=====================================+==================================+
| 수치           | SHORT, SMALLINT,                    |                                  |
+                +-------------------------------------+                                  +
|                | INTEGER, INT,                       |                                  |
+                +-------------------------------------+                                  +
|                | BIGINT,                             |                                  |
+                +-------------------------------------+                                  +
|                | NUMERIC, DECIMAL,                   |                                  |
+                +-------------------------------------+                                  +
|                | FLOAT, REAL,                        |                                  |
+                +-------------------------------------+                                  +
|                | DOUBLE, DOUBLE PRECISION,           |                                  |
+----------------+-------------------------------------+----------------------------------+
| 날짜/시간      | DATE, TIME, TIMESTAMP, DATETIME,    | | TIMESTAMPLTZ, TIMESTAMPTZ,     |
|                |                                     | | DATETIMELTZ, DATETIMETZ        |
+----------------+-------------------------------------+----------------------------------+
| 문자열         | CHAR, VARCHAR, STRING, CHAR VARYING |                                  |
+----------------+-------------------------------------+----------------------------------+
| 컬렉션         |                                     | SET, MULTISET, LIST, SEQUENCE    |
+----------------+-------------------------------------+----------------------------------+
| 기타           |                                     | BIT, BIT VARYING,                |
+                +                                     +----------------------------------+
|                |                                     | ENUM,                            |
+                +                                     +----------------------------------+
|                |                                     | BLOB/CLOB,                       |
+                +                                     +----------------------------------+
|                |                                     | JSON                             |
+----------------+-------------------------------------+----------------------------------+

.. _percent_type:

%TYPE
======================

테이블 컬럼 이름 뒤에 '%TYPE'을 덧붙여 해당 컬럼의 타입을 나타낼 수 있다.
아래는 %TYPE을 사용하는 예제이다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION get_athlete_name(p_code athlete.code%TYPE) RETURN athlete.name%TYPE
    AS
        name athlete.name%TYPE;
    BEGIN
        SELECT a.name
        INTO name
        FROM athlete a
        WHERE a.code = p_code;

        RETURN name;
    END;

<table>.<column>%TYPE은 CREATE PROCEDURE/FUNTION 문을 실행하는 시점에 지정된 테이블 컬럼의 타입을 나타내지만,
나중에 그 컬럼의 타입이 변경되어도 자동으로 <table>.<column>%TYPE을 사용한 저장 프로시저/함수의 동작에 반영되지는 않는다.
그러므로, %TYPE을 적용한 테이블 컬럼의 타입이 변경되었을 때는 그 %TYPE을 사용한 저장 프로시저/함수에 대해서 모두
ALTER PROCEDURE/FUNCTION <name> COMPILE 문을 실행해서 재컴파일해 주어야 한다.

테이블 컬럼 뿐만 아니라 프로시저/함수의 인자나 변수 이름 뒤에 %TYPE을 덧붙여 그 인자나 변수의 타입을 나타낼 수 있다.

.. code-block:: sql

   ...
   a VARCHAR(10);
   a_like a%TYPE;   -- 변수 a와 동일한 타입으로 변수 a_like 을 선언
   ...

.. _percent_rowtype:

%ROWTYPE
======================

테이블 이름 뒤에 %ROWTYPE을 덧붙여서 그 테이블 컬럼들의 이름과 타입을 갖는 필드들로 이루어진 레코드 타입을 나타낼 수 있다.
예를 들어, 다음과 같이 선언된 테이블 tbl에 대해서

.. code-block:: sql

   CREATE TABLE tbl(a INT, b CHAR, c VARCHAR);

변수 r을 tbl%ROWTYPE 타입으로 선언하면

.. code-block:: sql

   r tbl%ROWTYPE;

r의 값은 필드 a, b, c를 갖는 레코드가 되고 r.a, r.b, r.c는 각각 INT, CHAR, VARCHAR 타입을 갖는다.

커서 이름 뒤에도 %ROWTYPE을 덧붙일 수 있다.
이 때는 커서 정의에 주어진 SELECT 문의 결과에 해당하는 레코드 타입을 나타내게 된다.

.. code-block:: sql

   CURSOR c IS SELECT a, b from tbl;
   p c%ROWTYPE;     -- p.a, p.b는 각각 INT, CHAR 타입

레코드 변수의 선언문에서 초기값을 주지 않았을 때 그 변수는 모든 필드가 NULL인 '빈레코드'로 초기화 된다.

.. code-block:: sql

   r tbl%ROWTYPE;   -- r.a, r.b, r.c 모두 NULL. 그러나 r은 NULL 아닌 빈레코드

레코드 변수에 NULL을 대입하면 각 필드가 NULL로 초기화 되지만 레코드 변수 값 자체가 NULL이 되지는 않는다.
즉, 레코드 변수는 선언 이후로 NULL 값을 갖는 일이 없다.

동일한 타입의 레코드끼리는 =와 != 연산자로 비교할 수 있다.
여기서 동일 타입 레코드란 하나의 테이블로부터 얻어진 레코드 타입만을 의미하는 것이 아니라
다른 테이블이라도 대응하는 필드들의 이름과 타입이 일치하는 경우까지 포함하는 것이다.
두 레코드에 대한 = 연산의 결과는 대응하는 필드끼리 <=> 연산을 한 결과가 모두 TRUE일 때 TRUE이고 그렇지 않으면 FALSE이다.
!= 연산의 결과는 = 연산 결과의 반대이다.
다른 타입의 레코드에 =와 != 연산자를 사용했을 때는 컴파일 과정에서 에러가 발생한다.

.. code-block:: sql

    create table tblA(a INT, b CHAR, c VARCHAR);
    create table tblB(a INT, b CHAR, c VARCHAR);        -- tblA%ROWTYPE과 tblB%ROWTYPE은 동일 타입
    create table tblB(aa INT, bb CHAR, cc VARCHAR);     -- tblA%ROWTYPE과 tblC%ROWTYPE은 동일 타입 아님

    CREATE OR REPLACE PROCEDURE test_record_equality AS
        r1 tblA%ROWTYPE;
        r2 tblB%ROWTYPE;
        r3 tblC%ROWTYPE;
    BEGIN
        ...
        if (r1 = r2) then       -- OK
        ...
        if (r1 = r3) then       -- Error
        ...
    END;

=와 != 아닌 다른 비교 연산자 <=>, <, >, <=, >= 들은 레코드 비교에 적용할 수 없다.

다음 조건이 만족되는 경우, 레코드 변수 s로부터 다른 레코드 변수 t로의 대입이 가능하다.

* s와 t의 필드 개수가 동일함.
* 각각의 필드 순번 i에 대해서, s와 t의 i번째 필드들의 타입을 S\ :sub:`i`\와 T\ :sub:`i`\라고 할 때, S\ :sub:`i`\에서 T\ :sub:`i`\로 대입 가능함.

레코드 변수 사이에 대입이 가능하기 위해서 같은 순번의 필드명이 동일할 필요는 없다.

.. code-block:: sql

    create table tblAA(a NUMERIC, b DATETIME);
    create table tblBB(m INT, n VARCHAR);
    create table tblCC(x INT, y TIME);

    CREATE OR REPLACE PROCEDURE test_record_assign AS
        r1 tblAA%ROWTYPE;
        r2 tblBB%ROWTYPE;
        r3 tblCC%ROWTYPE;
    BEGIN
        ...
        r1 := r2;   -- OK
        r1 := r3;   -- Error: TIME에서 DATETIME으로 대입 불가 (형변환 불가)
    END;

%ROWTYPE은 내부 프로시저/함수의 인자 타입과 리턴 타입으로 사용할 수 있다.
그러나, SQL문에서 레코드 타입을 지원하지 않기 때문에 저장 프로시저/함수의 인자 타입과 리턴 타입으로는 사용할 수 없다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE sp(a tbl%ROWTYPE) AS    -- Error

        PROCEDURE inner(b tbl%ROWTYPE) AS               -- OK
        BEGIN
            ...
        END;
    BEGIN
        ...
    END;

Static/Dynamic SQL SELECT 문과 FETCH 문의 INTO 절에 레코드 변수를 사용할 수 있으며, 레코드 변수를 사용할 경우 다른 변수와 함께 사용할 수는 없다.
그리고, 조회 결과와 레코드 변수의 컬럼명은 동일할 필요는 없지만, 조회 컬럼 개수와 레코드 변수의 컬럼 개수는 동일해야 하며, 조회 컬럼 타입과 레코드 변수의 컬럼 타입이 동일하거나 호환 가능해야 한다.

.. code-block:: sql

   CURSOR c IS SELECT a, b from tbl;
   whole tbl%ROWTYPE;
   part c%ROWTYPE;

   -- Static SQL
   SELECT * INTO whole from tbl;

   -- Dynamic SQL
   EXECUTE IMMEDIATE 'SELECT * from tbl' INTO whole;
   EXECUTE IMMEDIATE 'SELECT a, b from tbl' INTO part;

   -- Fetch
   FETCH c INTO part;

Static SQL INSERT/REPLACE 문의 VALUES 절에 레코드 변수를 사용할 수 있으며, 레코드 변수를 사용할 경우 다른 변수와 함께 사용할 수 없다.
그리고, 대입 컬럼명과 레코드 변수의 컬럼명은 동일할 필요는 없지만, 대입 컬럼 개수와 레코드 변수의 컬럼 개수는 동일해야 하며, 대입 컬럼 타입과 레코드 변수의 컬럼 타입이 동일하거나 호환 가능해야 한다.

.. code-block:: sql

   INSERT INTO tbl VALUES whole;
   INSERT INTO tbl(a, b) VALUES part;

이 때 다음과 같은 형태도 가능하다.

.. code-block:: sql

   INSERT INTO tbl SET ROW = whole;
   INSERT INTO tbl(a, b) SET ROW = part;


Static SQL UPDATE 문에도 다음과 같이 'SET ROW = <record>' 구문을 사용하여 레코드 변수를 사용할 수 있다.
단, 단일 테이블 갱신에만 사용되며 각각의 레코드 필드로부터 동일 순번의 테이블 컬럼으로 대입 가능해야 한다.

.. code-block:: sql

   UPDATE tbl SET ROW = whole WHERE a % 2 = 0;


정밀도와 스케일 지정 예외
==============================

:ref:`PL/CSQL에서 지원하는 데이터 타입 <datatype_index>` 중에 NUMERIC은 정밀도와 스케일을,
CHAR와 VARCHAR는 길이를 지정할 수 있다.
그러나, 저장 프로시저/함수의 인자 타입과 리턴 타입에는 정밀도와 스케일 지정이 허용되지 않는다.
내부 프로시저/함수에서도 마찬가지이다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION sf(a NUMERIC(5, 3)) RETURN VARCHAR(10) AS ...    -- Error
    CREATE OR REPLACE FUNCTION sf(a NUMERIC) RETURN VARCHAR AS ...              -- OK

그리고, 일반적으로 정밀도와 스케일이 생략된 NUMERIC은 NUMERIC(15, 0)을 의미하지만
예외적으로 인자 타입 자리에서는 임의의 정밀도와 스케일을 허용함을 의미하고
(단, 정밀도는 1 이상 38 이하,  스케일은 0 이상 정밀도 이하 범위),
리턴 타입 자리에서는 NUMERIC(p, s)를 의미한다.
여기서 p와 s는 :ref:`시스템 설정 파라미터 <system_config>` stored_procedure_return_numeric_size에서
지정하는 정밀도와 스케일을 나타낸다. p와 s의 디폴트 값은 각각 38과 15이다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_any_precision_scale(a NUMERIC) return NUMERIC
    AS
    BEGIN
        return a;
    END;

    SELECT test_any_precision_scale(1.23);      -- 결과: 1.230000000000000
    SELECT test_any_precision_scale(1.234);     -- 결과: 1.234000000000000
    SELECT test_any_precision_scale(1.2345);    -- 결과: 1.234500000000000

또한, CHAR와 VARCHAR도 인자 타입과 리턴 타입 자리에서는
다른 자리에서처럼 CHAR(1)과 VARCHAR(1073741823)를 나타내는 것이 아니라
임의의 길이를 갖는 문자열을 허용하는 것으로 동작한다
(단, CHAR 길이는 2048 이하, VARCHAR의 길이는 1073741823 이하 범위).

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_any_length(a CHAR) return CHAR
    AS
    BEGIN
        return a;
    END;

    SELECT test_any_length('ab');       -- 결과: 'ab'
    SELECT test_any_length('abc');      -- 결과: 'abc'
    SELECT test_any_length('abcd');     -- 결과: 'abcd'

인자 타입과 리턴 타입을 :ref:`%TYPE <percent_type>`\을 사용해서 지정했을 때에도 참조되는 원래 타입의
정밀도, 스케일 및 길이 지정은 무시되고 위에 기술한 방식으로 동작한다.

.. code-block:: sql

    CREATE TABLE tbl(p NUMERIC(3,2), q CHAR(3));

    CREATE OR REPLACE FUNCTION test_ptype_precision_scale(a tbl.p%TYPE) RETURN tbl.p%TYPE
    AS
    BEGIN
        RETURN a;
    END;

    SELECT test_ptype_precision_scale(1.23);      -- 결과: 1.230000000000000
    SELECT test_ptype_precision_scale(1.234);     -- 결과: 1.234000000000000
    SELECT test_ptype_precision_scale(1.2345);    -- 결과: 1.234500000000000

    CREATE OR REPLACE FUNCTION test_ptype_length(a tbl.q%TYPE) RETURN tbl.q%TYPE
    AS
    BEGIN
        RETURN a;
    END;

    SELECT test_ptype_length('ab');       -- 결과: 'ab'
    SELECT test_ptype_length('abc');      -- 결과: 'abc'
    SELECT test_ptype_length('abcd');     -- 결과: 'abcd'

연산자와 함수
==================

Static/Dynamic SQL에서는 SQL에서 제공하는 모든 연산자와 함수를 쓸 수 있다.
반면, Static/Dynamic SQL 밖의 PL/CSQL 문에서는 SQL에서 제공하는 모든 연산자와 함수를
대부분 동일하게 쓸 수 있으나 다음 몇 가지 예외가 있다.

* 지원하지 않는 타입(BIT, ENUM, BLOB/CLOB, JSON, 등)의 값을 인자나 결과로 갖는 연산자와 함수는 쓸 수 없다.
* 나머지 연산자 %를 쓸 수 없다. 단, 동일한 의미의 MOD를 대신 쓸 수 있다.
* 논리 연산자 &&, ||, ! 들을 쓸 수 없다. 단, 각각 동일한 의미의 AND, OR, NOT을 대신 쓸 수 있다.

다음 예제는 문자열 함수 locate과 substr, 그리고 문자열 병합 연산자 ||를 Static/Dynamic SQL 밖의
PL/CSQL 실행문에서도 사용할 수 있음을 보여준다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE family_name_to_last
    AS
        delim INTEGER;
        family_name VARCHAR;
        given_name VARCHAR;
    BEGIN
        FOR r IN (SELECT a.name FROM athlete a limit 5,5) LOOP
            delim := locate(' ', r.name);                   -- 함수 locate
            family_name := substr(r.name, 1, delim - 1);    -- 함수 substr
            given_name := substr(r.name, delim + 1);        -- 함수 substr
            DBMS_OUTPUT.put_line(given_name || ' ' || family_name);     -- 문자열 병합 연산자 ||
        END LOOP;
    END;

.. _exception:

Exception
======================

PL/CSQL은 다른 많은 프로그래밍 언어와 마찬가지로 Exception 핸들러를 통한 에러 처리를 지원한다
(참고: :ref:`Block 실행문 <block_stmt>`).
사용자가 프로그램 선언부에서 자신만의 Exception을 정의하고 실행부에서 사용할 수 있다
(참고: :ref:`Exception 선언 <exception_decl>`).
그리고, 주요 에러 상황에 대해서는 다음과 같이 시스템 Exception들이 미리 정의되어 있다.


+---------------------+---------+------------------------------------------------------------------+
| 이름                | SQLCODE | 설명                                                             |
+=====================+=========+==================================================================+
| CASE_NOT_FOUND      | 0       | CASE 문에서 조건이 참인 WHEN 절이 없고 ELSE 절도 없음            |
+---------------------+---------+------------------------------------------------------------------+
| CURSOR_ALREADY_OPEN | 1       | 이미 열려 있는 커서에 다시 열기 시도                             |
+---------------------+---------+------------------------------------------------------------------+
| INVALID_CURSOR      | 2       | 허용되지 않는 커서 조작 (예: 열려 있지 않은 커서를 닫으려고 함)  |
+---------------------+---------+------------------------------------------------------------------+
| NO_DATA_FOUND       | 3       | SELECT INTO 문 실행 결과 0개의 Row가 반환됨                      |
+---------------------+---------+------------------------------------------------------------------+
| PROGRAM_ERROR       | 4       | 시스템 내부 에러                                                 |
+---------------------+---------+------------------------------------------------------------------+
| STORAGE_ERROR       | 5       | 메모리 부족으로 인한 할당 실패                                   |
+---------------------+---------+------------------------------------------------------------------+
| SQL_ERROR           | 6       | Static/Dynamic SQL 실행 실패                                     |
+---------------------+---------+------------------------------------------------------------------+
| TOO_MANY_ROWS       | 7       | SELECT INTO 문 실행 결과 2개 이상의 Row가 반환됨                 |
+---------------------+---------+------------------------------------------------------------------+
| VALUE_ERROR         | 8       | 잘못된 값에 의한 에러                                            |
+---------------------+---------+------------------------------------------------------------------+
| ZERO_DIVIDE         | 9       | 0으로 나누기 시도                                                |
+---------------------+---------+------------------------------------------------------------------+

위에서 각 Exception의 SQLCODE는 :ref:`OTHERS Exception 핸들러 block <block_stmt>` 안에서
Exception의 종류를 식별하는데 사용할 수 있다.

* 999 이하의 SQLCODE 값들은 시스템 Exception을 위해서 예약되어 있다.
* :ref:`사용자가 선언한 Exception <exception_decl>`\은 SQLCODE 1000 값을 갖는다.
* :ref:`RAISE_APPLICATION_ERROR <raise_application_error>`\의 첫번째 인자로 지정하는 SQLCODE는 1000보다 큰 값을 가져야 한다.

다음은 Static SQL SELECT 문을 실행할 때 발생할 수 있는 시스템 Exception NO_DATA_FOUND와 TOO_MANY_ROWS를
처리하는 간단한 예제이다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION athlete_code(p_name VARCHAR) RETURN integer
    AS
        c INTEGER;
    BEGIN
        -- SELECT INTO 문은 단 하나, 그리고 오직 하나의 Row를 결과로 가져야 함
        SELECT code
        INTO c
        FROM athlete a
        WHERE a.name = p_name;

        RETURN c;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.put_line('error: no rows found for athlete name ' || p_name);
            RETURN -1;
        WHEN TOO_MANY_ROWS THEN
            DBMS_OUTPUT.put_line('error: more than one rows found for athlete name ' || p_name);
            RETURN -1;
    END;

발생한 Exception에 대한 처리를 명시적 (WHEN ... THEN ... 절)으로 처리하지 않은 경우에는
코드상의 Exception 발생 위치와 에러메시지가 출력된다.
예를 들어, 위 athlete_code()에서 Exception 처리절들을 삭제하고

.. code-block:: sql

    CREATE OR REPLACE FUNCTION athlete_code(p_name VARCHAR) RETURN integer
    AS
        c INTEGER;
    BEGIN
        -- SELECT INTO 문은 단 하나, 그리고 오직 하나의 Row를 결과로 가져야 함
        SELECT code
        INTO c
        FROM athlete a
        WHERE a.name = p_name;

        RETURN c;
    END;

CSQL에서 athlete 테이블에 존재하지 않는 이름을 인자로 주어 NO_DATA_FOUND Exception을 일으켰을 때 결과는 다음과 같다.

.. code-block::

    select athlete_code('x');

   In line 1, column 22,

   ERROR: Stored procedure execute error:
     (line 6, column 5) no data found


   0 command(s) successfully processed.

위에서 위치 (1, 22)는 SELECT 문 안에서의 위치를 나타내고, (6, 5)는 athlete_code()를 선언한 CREATE 문 안에서의
위치를 나타낸다.

시스템 설정 적용
==========================

Static/Dynamic SQL 문의 동작은 :ref:`시스템 설정 파라미터 <system_config>` 전체의 영향을 동일하게 받는다.

Static/Dynamic SQL 제외한 PL/CSQL 문에서는 다음 4개 시스템 설정 파라미터만이 유효하다.

* compat_numeric_division_scale
* oracle_compat_number_behavior
* oracle_style_empty_string
* timezone

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_system_config
    AS
    BEGIN
        -- compat_numeric_division_scale가 no일 때 0.125000000, yes일 때 0.1
        dbms_output.put_line(1.0 / 8.0);

        -- oracle_compat_number_behavior가 no일 때 1, yes일 때 2
        dbms_output.put_line(3 / 2);

        -- oracle_style_empty_string가 no일 때 'false', yes일 때 'true'
        if '' IS NULL THEN
            dbms_output.put_line('true');
        ELSE
            dbms_output.put_line('false');
        END IF;
    END;

이들 설정의 자세한 의미는 :ref:`시스템 설정 파라미터 <system_config>`\를 참조할 수 있다.

위 4개 외 다른 설정은 Static/Dynamic SQL 제외한 PL/CSQL 문에서 유효하지 않다. 특히,

* no_backslash_escapes 설정 파라미터값과 상관없이 backslash 문자는 escape 문자로 사용되지 않는다.
* pipes_as_concat 설정 파라미터값과 상관없이 ||는 논리합(OR) 연산자로 사용되지 않는다.
* plus_as_concat 설정 파라미터값과 상관없이 +는 문자열에 적용되었을 때 병합 연산자로 사용된다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_system_config_2
    AS
    BEGIN
        -- no_backslash_escapes 값에 상관없이 'Hello\nworld'
        dbms_output.put_line('Hello\nworld');

        -- pipes_as_concat 값에 상관없이 'ab'
        dbms_output.put_line('a' || 'b');

        -- plus_as_concat 값에 상관없이 '12'
        dbms_output.put_line('1' + '2');
    END;
