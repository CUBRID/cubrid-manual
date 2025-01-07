
:meta-keywords: cubrid sql, pl/csql
:meta-description: This chapter describes PL/CSQL Spec.

*****************************
Overview
*****************************

.. _stored_proc:

저장 프로시저/함수 생성
==============================

PL/CSQL은 저장 프로시저나 저장 함수를 생성하는데 사용된다.
다음 문법을 따르는 CREATE PROCEDURE 문과 CREATE FUNCTION 문의 AS (또는 IS) 키워드 뒤에 PL/CSQL 코드를 써서
생성하고자 하는 저장 프로시저/함수의 동작을 기술한다.

::

    <create_procedure> ::=
        CREATE [ OR REPLACE ] PROCEDURE <identifier> [ ( <seq_of_parameters> ) ]
        { IS | AS } [ LANGUAGE PLCSQL ] [ <seq_of_declare_specs> ] <body> ;
    <create_function> ::=
        CREATE [ OR REPLACE ] FUNCTION <identifier> [ ( <seq_of_parameters> ) ] RETURN <type_spec>
        { IS | AS } [ LANGUAGE PLCSQL ] [ <seq_of_declare_specs> ] <body> ;

위 문법에서 저장 프로시저/함수의 *body*\는 PL/CSQL 실행문들을 포함하고
그 앞의 선언부 *seq_of_declare_specs*\는 실행문들 안에서 사용될 변수, 상수, Exception 등을 선언한다.
이들 문법 요소에 대한 자세한 내용은 :ref:`선언문 <decl>`\과 :ref:`실행문 <stmt>` 절을 참고한다.

저장 프로시저/함수는 Auto Commit 기능이 언제나 비활성화 된 상태로 실행된다.
이는 호출한 세션에서 Auto Commit 기능이 활성화 되어 있어도 마찬가지이다.

저장 프로시저/함수는 :ref:`큐브리드 내장 함수 <operators-and-functions>`\와 동일한 이름을 가질 수 없다.
동일한 이름으로 선언하면 컴파일 과정에서 (CREATE 문 실행 과정에서) 에러가 발생한다.

body 내부의 실행문들 중 수행시 도달할 수 없는 실햄문이 있는 경우 컴파일 과정에서 에러를 발생한다.
다음은 도달할 수 없는 실행문이 있는 간단한 예이다.

.. code-block:: sql

    csql> CREATE OR REPLACE PROCEDURE test_unreachable_statement
    csql> AS
    csql> BEGIN
    csql>     return;
    csql>     dbms_output.put_line('Hello world');
    csql> END;

    ERROR: In line 5, column 5
    Stored procedure compile error: unreachable statement

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

CREATE PROCEDURE/FUNCTION 문을 실행하면 저장 프로시저/함수의 문법과 실행 의미에 관련된 각종 규칙들을 검사한다.
검사에서 오류가 발견되면 발생된 위치와 원인을 설명하는 오류 메세지를 출력한다.
다음은 오류를 가지고 있는 저장 프로시저가 CSQL에서 에러를 발생시키는 예이다.

.. code-block:: sql

    csql> CREATE OR REPLACE PROCEDURE athlete_code(p_name VARCHAR) AS
    csql> BEGIN
    csql>     -- 오류: Static SQL SELECT 문은 INTO 절을 가져야 함
    csql>     SELECT code
    csql>     FROM athlete a
    csql>     WHERE a.name = p_name;
    csql> END;

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
ALTER PROCEDURE/FUNCTION <name> REBUILD 문을 실행해서 재컴파일해 주어야 한다.

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
그러나, 저장 프로시저/함수의 인자 타입과 리턴 타입에는 예외적으로 정밀도와 스케일 지정이 허용되지 않는다.
내부 프로시저/함수에서도 마찬가지이다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION sf(a NUMERIC(5, 3)) RETURN VARCHAR(10) AS ...    -- Error
    CREATE OR REPLACE FUNCTION sf(a NUMERIC) RETURN VARCHAR AS ...              -- OK

그리고, 일반적으로 정밀도와 스케일이 생략된 NUMERIC은 NUMERIC(15, 0)을 의미하지만
예외적으로 인자 타입과 리턴 타입 자리에서는 임의의 정밀도와 스케일을 허용하는 것으로 동작한다
(단, 정밀도는 1 이상 38 이하,  스케일은 0 이상 정밀도 이하 범위).
또한, CHAR와 VARCHAR도 인자 타입과 리턴 타입 자리에서는 기본 스케일 값인 CHAR(1)과 VARCHAR(1073741823)를 나타내는 것이
아니라 임의의 길이를 갖는 문자열을 허용하는 것으로 동작한다
(단, CHAR 길이는 2048 이하, VARCHAR의 길이는 1073741823 이하 범위).

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_any_precision_scale(a NUMERIC) return NUMERIC
    AS
    BEGIN
        return a;
    END;

    SELECT test_any_precision_scale(1.23);      -- 결과: 1.23
    SELECT test_any_precision_scale(1.234);     -- 결과: 1.234
    SELECT test_any_precision_scale(1.2345);    -- 결과: 1.2345

    CREATE OR REPLACE FUNCTION test_any_length(a CHAR) return CHAR
    AS
    BEGIN
        return a;
    END;

    SELECT test_any_length('ab');       -- 결과: 'ab'
    SELECT test_any_length('abc');      -- 결과: 'abc'
    SELECT test_any_length('abcd');     -- 결과: 'abcd'

인자 타입과 리턴 타입을 :ref:`%TYPE <percent_type>`\을 사용해서 지정했을 때에도 참조되는 원래 타입의
정밀도, 스케일 및 길이 지정은 무시되고 대신 임의의 정밀도, 스케일, 길이를 허용하는 것으로 동작한다.

.. code-block:: sql

    CREATE TABLE tbl(p NUMERIC(3,2), q CHAR(3));

    CREATE OR REPLACE FUNCTION test_ptype_precision_scale(a tbl.p%TYPE) RETURN NUMERIC
    AS
    BEGIN
        RETURN a;
    END;

    SELECT test_ptype_precision_scale(1.23);      -- 결과: 1.23
    SELECT test_ptype_precision_scale(1.234);     -- 결과: 1.234
    SELECT test_ptype_precision_scale(1.2345);    -- 결과: 1.2345

    CREATE OR REPLACE FUNCTION test_ptype_length(a tbl.q%TYPE) RETURN tbl.q%TYPE
    AS
    BEGIN
        RETURN a;
    END;

    SELECT test_ptype_length('ab');       -- 결과: 'ab'
    SELECT test_ptype_length('abc');      -- 결과: 'abc'
    SELECT test_ptype_length('abcd');     -- 결과: 'abcd'

단, %TYPE 사용과 관련해서 한 가지 예외가 있다. 함수의 리턴 타입에 %TYPE이 사용되고 참조되는 원래 타입이
NUMERIC(p, s) 이면 원래 타입의 정밀도 p와 스케일 s가 유지된다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_return_ptype_numeric(a tbl.p%TYPE) RETURN tbl.p%TYPE
    AS
    BEGIN
        RETURN a;
    END;

    SELECT test_return_ptype_numeric(1.23);      -- 결과: 1.23
    SELECT test_return_ptype_numeric(1.234);     -- 결과: 1.23
    SELECT test_return_ptype_numeric(1.2345);    -- 결과: 1.23
    SELECT test_return_ptype_numeric(12.345);    -- Error: 스케일 2로 반올림한 값 12.34가 정밀도 3을 초과

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

   csql> select athlete_code('x');

   In line 1, column 22,

   ERROR: Stored procedure execute error:
     (line 6, column 5) no data found


   0 command(s) successfully processed.

위에서 위치 (1, 22)는 SELECT 문 안에서의 위치를 나타내고, (6, 5)는 athlete_code()를 선언한 CREATE 문 안에서의
위치를 나타낸다.

서버 설정 적용
==========================

Static/Dynamic SQL 문의 동작은 :ref:`서버 설정 파라미터 <system_config>` 전체의 영향을 동일하게 받는다.

Static/Dynamic SQL 제외한 PL/CSQL 문에서는 다음 4개 서버 설정 파라미터만이 유효하다.

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

이들 설정의 자세한 의미는 :ref:`서버 설정 파라미터 <system_config>`\를 참조할 수 있다.

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

.. _decl:

******************
선언문
******************

프로시저/함수 선언문, 그리고 Block 실행문에는 선언부 *seq_of_declare_specs*\가 존재한다.
선언부에서는 아래 문법에서 정의하는 바와 같이 변수, 상수, Exception, 커서,
내부 프로시저/함수를 선언할 수 있다.
선언된 각 항목들은 선언부를 뒤따르는 *body* 안에서 참조할 수 있다.

::

    <seq_of_declare_specs> ::= <declare_spec> [ <declare_spec> ... ]
    <declare_spec> ::=
          <variable_decl>
        | <constant_decl>
        | <exception_decl>
        | <cursor_decl>
        | <inner_procedure_decl>
        | <inner_function_decl>

선언 가능한 각 항목에 대한 설명은 아래 내용을 참고한다.

:ref:`내부 프로시저/함수 선언 <local_routine_decl>`\과 :ref:`Block 실행문 <block_stmt>`\은
자신만의 선언부와 실행부를 가지면서 중첩된 scope들을 이룬다.
이 때 안쪽 scope 선언부에서 바깥에서 선언한 항목과 동일한 이름을 가진 다른 항목을 선언하면
안쪽 scope에서 그 이름은 새로 정의한 항목을 가리키며 바깥쪽의 동일 이름은 가려진다.
단, 안쪽 scope 밖에서 그 이름은 여전히 바깥 항목을 가리킨다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE hidden_variable
    AS
        a INT := 3;
        b VARCHAR(10);

        -- 내부 프로시저
        PROCEDURE inner_proc
        AS
            a INT := 5;
            b FLOAT;
        BEGIN
            -- 여기서 a = 5, b는 FLOAT 타입
        END;

    BEGIN
        -- 여기서 a = 3, b는 VARCHAR(10) 타입

        -- Block 실행문
        DECLARE
            a INT := 7;
            b DATETIME;
        BEGIN
            -- 여기서 a = 7, b는 DATETIME 타입
        END;

        -- 다시 a = 3, b는 VARCHAR(10) 타입
    END;

이러한 "이름 가림"(Name Hiding)은 다른 종류의 항목(상수, 프로시저/함수 인자, Exception, 커서, 내부 프로시저/함수)들에
대해서도 동일하게 적용된다.

단, 가려지는 항목이 동일 선언부 위쪽에서 다른 변수나 상수의 초기값 표현식에 사용되었다면 컴파일 과정에서 에러가 발생한다.
다음은 그 간단한 예이다. 프로시저 poo의 인자 a를 내부 프로시저 inner 안에서 선언한 변수 i의 초기값으로 사용하고
그 아래쪽에서 a 이름으로 다시 변수를 선언하였다. 이런 경우에는 '... already been used ... in the same declaration block'
이라는 메시지의 에러가 발생한다.

.. code-block:: sql

    csql> CREATE OR REPLACE PROCEDURE poo(a INT) AS
    csql>
    csql>     PROCEDURE inner AS
    csql>         i INT := a;
    csql>         a NUMERIC;
    csql>     BEGIN
    csql>         ...
    csql>     END;
    csql>
    csql> BEGIN
    csql>     ...
    csql> END;

    ERROR: In line 5, column 9
    Stored procedure compile error: name A has already been used at line 4 and column 18 in the same declaration block



변수 선언
=========

::

    <variable_decl> ::=
        <identifier> <type_spec> [ [ NOT NULL ] <initial_value_part> ] ;

    <type_spec> ::=
          <builtin_type>
        | <table>.<column>%TYPE
        | <variable>%TYPE
        | <table>%ROWTYPE
        | <cursor>%ROWTYPE
    <initial_value_part> ::= { := | DEFAULT } <expression>

* *builtin_type*: :ref:`데이터 타입 <types>` 절에서 설명한 시스템 제공 타입

변수 선언에 선택적으로 NOT NULL 조건과 초기값을 지정할 수 있다.
NOT NULL 조건이 지정된 경우에는 반드시 NULL이 아닌 초기값이 함께 지정되어야 한다.
선언할 때 초기값이 지정되지 않은 변수는 묵시적으로 NULL 값을 갖게 된다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_variable
    AS
        a INT NOT NULL := 3;
        b VARCHAR(1) := 's';
        c FLOAT;        -- c = NULL
    BEGIN
        --
        NULL;
    END;

상수 선언
=========
::

    <constant_decl> ::=
        <identifier> CONSTANT <type_spec> [ NOT_NULL ] <value_part> ;

    <type_spec> ::=
          <builtin_type>
        | <table>.<column>%TYPE
        | <variable>%TYPE
        | <table>%ROWTYPE
        | <cursor>%ROWTYPE
    <value_part> ::= { := | DEFAULT } <expression>

* *builtin_type*: :ref:`데이터 타입 <types>` 절에서 설명한 시스템 제공 타입

상수 선언에는 필수적으로 값 지정이 포함되어야 한다.
NOT NULL 조건이 지정된 경우, 이 값은 NULL이 아니어야 한다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_constant
    AS
        a CONSTANT INT NOT NULL := 3;
        b CONSTANT VARCHAR := 's';
        --c CONSTANT FLOAT;        -- 에러
    BEGIN
        ...
    END;

.. _exception_decl:

Exception 선언
==============

::

    <exception_decl> ::=
        <identifier> EXCEPTION ;

사용자가 원하는 이름의 Exception을 선언할 수 있다.
이렇게 선언된 Exception을 :ref:`RAISE <raise>` 문과 Exception 처리의 :ref:`WHEN <block_stmt>` 절에서 사용할 수 있다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION text_exception(n INT) RETURN INT
    AS
        negative_argument EXCEPTION;
        too_big_argument EXCEPTION;
    BEGIN
        IF n < 0 THEN
            RAISE negative_argument;
        ELSIF n > 100 THEN
            RAISE too_big_argument;
        ELSIF n = 0 THEN
            RETURN 0;
        END IF;
        ...
    EXCEPTION
        WHEN negative_argument THEN
            DBMS_OUTPUT.put_line('error: negative argument ' || n);
            return -1;
        WHEN too_big_argument THEN
            DBMS_OUTPUT.put_line('error: too big argument ' || n);
            return -2;
    END;

.. _cursor_decl:

커서 선언
=========
::

    <cursor_decl> ::=
        CURSOR <identifier> [ ( <seq_of_cursor_parameters> ) ] IS <select_statement> ;

    <seq_of_cursor_parameters> ::= <cursor_parameter> [, <cursor_parameter>, ...]
    <cursor_parameter> ::= <identifier> [ IN ] <type_spec>
    <type_spec> ::=
          <builtin_type>
        | <table>.<column>%TYPE
        | <variable>%TYPE
        | <table>%ROWTYPE
        | <cursor>%ROWTYPE

* *builtin_type*: :ref:`데이터 타입 <types>` 절에서 설명한 시스템 제공 타입

커서에도 프로시저/함수와 유사하게 인자를 선언할 수 있지만 오직 IN 인자만 선언할 수 있다는 차이가 있다.
이 인자를 *select_statement* 문 안에서 참조할 수 있다.
커서를 :ref:`OPEN <cursor_manipulation>` 할 때 이 인자에 실제 선언된 개수와 타입이 일치하도록
인자값을 채워 해당 SELECT 문을 실행한다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_cursor(p_name VARCHAR, p_year INTEGER)
    AS
        CURSOR my_cursor(a VARCHAR, y INTEGER) IS
        SELECT host_year, score
        FROM history
        WHERE athlete = a AND host_year >= y;

        target_year INT;
        target_score VARCHAR(10);
    BEGIN
        OPEN my_cursor(p_name, p_year);
        LOOP
            FETCH my_cursor INTO target_year, target_score;
            EXIT WHEN my_cursor%NOTFOUND;
            DBMS_OUTPUT.put_line('host_year: ' || target_year || ' score: ' || target_score);
        END LOOP;
        CLOSE my_cursor;
    END;

커서는 위 예제처럼 명시적으로 OPEN, FETCH, CLOSE 실행문을 통해 이용할 수 있다.
반면, 아래 예제처럼 OPEN, FETCH, CLOSE 동작이 묵시적으로 이루어지는 For-Loop 문을 통해서 커서를 이용할 수도 있다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_cursor_loop(p_name VARCHAR, p_year INTEGER)
    AS
        CURSOR my_cursor(a VARCHAR, y INTEGER) IS
        SELECT host_year, score
        FROM history
        WHERE athlete = a AND host_year >= y;
    BEGIN
        FOR r IN my_cursor(p_name, p_year) LOOP
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);
        END LOOP;
    END;

커서를 선언할 때 사용하는 SELECT 문에는 INTO 절을 쓸 수 없다.

.. _local_routine_decl:

내부 프로시저/함수 선언
========================

정의 중인 저장 프로시저/함수 안에서만 사용할 내부 프로시저/함수를 다음 문법에 따라 정의할 수 있다.
어느 정도 규모를 이루거나 두 번 이상 반복되는 실행 과정을 내부 프로시저/함수로 묶어 모듈화하면
프로그램 가독성이 높아지고 모듈화한 부분의 코드 재사용성이 높아진다.

::

    <inner_procedure_decl> ::=
        PROCEDURE <identifier> [ ( <seq_of_parameters> ) ] { IS | AS } [ <seq_of_declare_specs> ] <body> ;
    <inner_function_decl> ::=
        FUNCTION <identifier> [ ( <seq_of_parameters> ) ] RETURN <type_spec> { IS | AS } [ <seq_of_declare_specs> ] <body> ;

    <seq_of_parameters> ::= [ <parameter> [, <parameter> ...] ]
    <parameter> ::= <identifier> [ { IN | IN OUT | INOUT | OUT } ] <type_spec>
    <type_spec> ::=
          <builtin_type>
        | <table>.<column>%TYPE
        | <variable>%TYPE
        | <table>%ROWTYPE
        | <cursor>%ROWTYPE
    <body> ::= BEGIN <seq_of_statements> [ EXCEPTION <seq_of_handlers> ] END [ <label_name> ]
    <seq_of_declare_specs> ::= <declare_spec> [ <declare_spec> ... ]
    <seq_of_statements> ::= <statement> ; [ <statement> ; ... ]
    <seq_of_handlers> ::= <handler> [ <handler> ... ]
    <handler> ::= WHEN <exception_name> [ OR <exeption_name> OR ... ] THEN <seq_of_statements>
    <exception_name> ::= identifier | OTHERS

* *parameter*: 인자는 IN, IN OUT, INOUT, OUT 네 가지 경우로 선언할 수 있다. IN OUT과 INOUT은 동일한 효과를 갖는다.
* *builtin_type*: :ref:`데이터 타입 <types>` 절에서 설명한 시스템 제공 타입
* *body*: 필수적으로 하나 이상의 실행문과 선택적으로 몇 개의 Exception 핸들러로 구성된다.
* *label_name*: 프로시저/함수 이름과 일치해야 한다.
* *declare_spec*: 변수, 상수, Exception, 커서, 내부 프로시저/함수 선언 중 하나
* *statement*: 아래 :ref:`실행문 <stmt>` 절 참조
* *handler*: 지정된 Exception이 발생했을 때 실행할 실행문들을 지정한다.
* *exception_name*: Exception 이름 *identifier*\는 :ref:`시스템 Exception <exception>`\이거나 :ref:`사용자가 선언 <exception_decl>`\한 것이어야 한다. OTHERS는 아직까지 매치되지 않은 모든 Exception에 매치되며 OR로 다른 exception 이름과 연결할 수 없다.

내부 프로시저/함수는 :ref:`저장 프로시저/함수 <stored_proc>`\와 달리
:ref:`큐브리드 내장 함수 <operators-and-functions>`\와 동일한 이름을 가질 수 있다.
이 때 내장 함수는 내부 프로시저/함수가 선언된 scope 안에서 가려진다.

함수의 경우에는  *body*\에서 RETURN 문으로 선언된 리턴 타입에 맞는 값을 반환해야 한다.
함수가 *body* 끝에 도달할 때까지 RETURN 문을 만나지 못하는 실행경로가 존재하면 컴파일 과정에서 에러가 발생한다.
프로시저의 경우에는 RETURN 문에 반환값을 지정할 수 없다.

프로시저/함수는 자기 자신을 실행부에서 참조할 수 있다. 즉, 재귀 호출이 가능하다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION choose(m INT, n INT) RETURN INT
    AS
        invalid_argument EXCEPTION;

        -- 내부 함수 선언
        FUNCTION factorial(n INT) RETURN INT
        AS
        BEGIN
            IF n < 0 THEN
                RAISE invalid_argument;
            ELSIF n <= 1 THEN
                RETURN 1;
            ELSE
                RETURN n * factorial(n - 1);    -- 재귀 호출
            END IF;
        END;
    BEGIN
        IF n > m OR n < 0 THEN
            RAISE invalid_argument;
        ELSE
            RETURN factorial(m) / factorial(n) / factorial(m - n);
        END IF;
    END;

동일한 선언부에서 선언된 내부 프로시저/함수끼리는 상호 재귀 호출도 가능하다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE ping_pong(cnt INT)
    AS
        PROCEDURE ping(n INT)
        AS
        BEGIN
            IF n <= 0 THEN
                DBMS_OUTPUT.put_line('-- end --');
            ELSE
                DBMS_OUTPUT.put_line('ping ->');
                pong(n - 1);     -- 상호 재귀 호출
            END IF;
        END;

        PROCEDURE pong(n INT)
        AS
        BEGIN
            IF n <= 0 THEN
                DBMS_OUTPUT.put_line('-- end --');
            ELSE
                DBMS_OUTPUT.put_line('      <- pong');
                ping(n - 1);     -- 상호 재귀 호출
            END IF;
        END;
    BEGIN
        ping(cnt);
    END;

재귀 호출을 사용할 때는 무한 루프에 빠지지 않도록 종료 조건을 적절히 주어야 한다.

.. _stmt:

******************
실행문
******************

현재 PL/CSQL은 다음과 같이 14가지 종류의 실행문을 제공한다.
::

    <statement> ::=
          <block>
        | <sql_statement>
        | <cursor_manipulation>
        | <raise_application_error>
        | <execute_immediate>
        | <assignment_statement>
        | <continue_statement>
        | <exit_statement>
        | <null_statement>
        | <raise_statement>
        | <return_statement>
        | <procedure_call>
        | <if_statement>
        | <loop_statement>
        | <case_statement>

.. _block_stmt:

BLOCK
=====
BLOCK 문은 실행문들 중간에 중첩 scope을 만들어 그 안에서 새로운 변수, 상수 등을 선언하고 사용할 수 있게 한다.
BLOCK은 프로시저/함수와 마찬가지로 Exception 처리 구조를 가질 수 있다.
::

    <block> ::=
        [ DECLARE <seq_of_declare_specs> ] <body>

    <body> ::= BEGIN <seq_of_statements> [ EXCEPTION <seq_of_handlers> ] END [ <label_name> ]
    <seq_of_declare_specs> ::= <declare_spec> [ <declare_spec> ... ]
    <seq_of_statements> ::= <statement> ; [ <statement> ; ... ]
    <seq_of_handlers> ::= <handler> [ <handler> ... ]
    <handler> ::= WHEN <exception_name> [ OR <exeption_name> OR ... ] THEN <seq_of_statements>
    <exception_name> ::= identifier | OTHERS


* *body*: 필수적으로 하나 이상의 실행문과 선택적으로 몇 개의 Exception 핸들러로 구성된다.
* *declare_spec*: 변수, 상수, Exception, 커서, 내부 프로시저/함수 선언. (참조: :ref:`선언문 <decl>`)
* *handler*:  지정된 Exception이 발생했을 때 실행할 실행문들을 지정한다.
* *exception_name*: Exception 이름 *identifier*\는 :ref:`시스템 Exception <exception>`\이거나 :ref:`사용자가 선언 <exception_decl>`\한 것이어야 한다. OTHERS는 선언되지 않은 모든 Exception에 매치되며 OR로 다른 exception 이름과 연결할 수 없다.


BLOCK 안에서 선언된 아이템들은 그 BLOCK을 벗어나면 참조할 수 없다.
BLOCK에서 선언된 아이템이 바깥 scope에서 선언된 다른 아이템과 이름이 겹칠 경우
바깥 아이템은 그 BLOCK 안에서 참조할 수 없게 된다 (가려진다).

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_block
    IS
        a INT := 3;
        b INT := 3;
        c INT := 3;
    BEGIN
        DECLARE
            a INT := 5;
            b INT := 5;
        BEGIN
            DECLARE
                a INT := 7;
            BEGIN
                DBMS_OUTPUT.put_line(a || b || c);  -- '753'
            END;

            DBMS_OUTPUT.put_line(a || b || c);      -- '553'
        END;

        DBMS_OUTPUT.put_line(a || b || c);          -- '333'
    END;

Static SQL
==========

:ref:`Static SQL <static_sql>` 절에서 설명한대로 SQL 문 중에서 SELECT, INSERT, UPDATE, DELETE, MERGE, REPLACE,
COMMIT, ROLLBACK, TRUNCATE 문은 프로그램의 실행문으로서 직접 사용 가능하다.

.. _cursor_manipulation:

커서 조작문
===========
커서 조작문은 아래와 같이 4 가지 종류가 있다.
::

    <cursor_manipulation> ::=
          <open_statement>
        | <fetch_statement>
        | <close_statement>
        | <open_for_statement>

    <open_statement> ::= OPEN <cursor> [ <function_argument> ]
    <fetch_statement> ::= FETCH <cursor_expression> INTO <identifier> [ , <identifier>, ... ]
    <close_statement> ::= CLOSE <cursor_expression>

    <open_for_statement> ::= OPEN <identifier> FOR <select_statement>

* *cursor_expression*: 계산 결과로 커서나 SYS_REFCURSOR 변수를 갖는 표현식
* *open_statement*: 커서를 연다. SYS_REFCURSOR 변수가 아닌 커서에 대해서만 사용가능함에 주의하자. 인자를 갖도록 선언된 커서에 대해서는 선언된 인자 개수와 타입에 맞는 값을 주면서 열어야 한다. 이미 열려 있는 커서를 다시 열려고 시도하면 CURSOR_ALREADY_OPEN Exception이 발생한다.
* *fetch_statement*: 커서로부터 하나의 row를 가져와 지정된 변수나 OUT 인자에 대입한다. row 안의 컬럼 개수는 지정된 변수나 OUT 인자 개수와 일치해야 하고 각각의 컬럼값은 해당 변수나 OUT 인자에 대입 가능한 타입을 가져야 한다. 열려 있지 않은 커서로부터 FETCH를 시도하면 INVALID_CURSOR Exception이 발생한다.
* *close_statement*: 커서를 닫는다. 열려 있지 않은 커서를 닫으려고 시도하면 INVALID_CURSOR Exception이 발생한다.
* *open_for_statement*: *identifier*\는 SYS_REFCURSOR 타입으로 선언된 변수이어야 한다. 지정된 *select_statement*\를 실행하는 커서를 내부적으로 열어서 지정된 변수에 할당한다. *select_statement*\가 INTO 절을 포함하면 컴파일 과정에서 에러가 발생한다.

다음은 OPEN, FETCH, CLOSE 문의 사용 예이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_cursor(p_name VARCHAR, p_year INTEGER)
    AS
        CURSOR my_cursor(a VARCHAR, y INTEGER) IS
        SELECT host_year, score
        FROM history
        WHERE athlete = a AND host_year >= y;

        target_year INT;
        target_score VARCHAR(10);
    BEGIN

        OPEN my_cursor(p_name, p_year);
        LOOP
            FETCH my_cursor INTO target_year, target_score;
            EXIT WHEN my_cursor%NOTFOUND;
            DBMS_OUTPUT.put_line('host_year: ' || target_year || ' score: ' || target_score);
        END LOOP;
        CLOSE my_cursor;
    END;

다음 예제는 SYS_REFCURSOR를 OUT 인자로 갖는 내부 프로시저와 OPEN-FOR 문을 이용해서 특정 SELECT 문을
SYS_REFCURSOR 변수에 연결하고 SELECT 문의 결과를 조회하는 예제이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_ref_cursor(p_name VARCHAR)
    AS
        my_refcursor SYS_REFCURSOR;

        target_year INT;
        target_score VARCHAR(10);

        PROCEDURE open_refcursor(athlete_name VARCHAR, rc OUT SYS_REFCURSOR)
        AS
            refcursor SYS_REFCURSOR;
        BEGIN
            OPEN refcursor FOR SELECT host_year, score FROM history WHERE athlete = athlete_name;
            rc := refcursor;
        END;
    BEGIN
        open_refcursor(p_name, my_refcursor);
        LOOP
            FETCH my_refcursor INTO target_year, target_score;
            EXIT WHEN my_refcursor%NOTFOUND;
            DBMS_OUTPUT.put_line('host_year: ' || target_year || ' score: ' || target_score);
        END LOOP;
        CLOSE my_refcursor;
    END;

.. _raise_application_error:

RAISE_APPLICATION_ERROR
=========================

RAISE_APPLICATION_ERROR는 사용자가 원하는 :ref:`코드와 에러메시지 <sqlcode>`\로 :ref:`Exception <exception>`\을
발생하고자 할 때 사용한다.
RAISE_APPLICATION_ERROR의 사용 형태는 Built-in 프로시저 호출처럼 보이지만 내부적으로는 PL/CSQL 실행문이다.
첫번째 인자로 주는 코드는 1000보다 큰 INTEGER 값을 가져야 한다. 1000 이하의 값은 시스템을 위해 예약되어 있기 때문이다.
두번째 인자로 주는 에러메시지는 임의의 문자열이 가능하다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_raise_app_err(i INT)
    AS
    BEGIN
        CASE i
        WHEN 1 THEN
            RAISE_APPLICATION_ERROR(1001, 'my error 1');
        WHEN 2 THEN
            RAISE_APPLICATION_ERROR(1002, 'my error 2');
        WHEN 3 THEN
            RAISE_APPLICATION_ERROR(1003, 'my error 3');
        END CASE;
    EXCEPTION
        WHEN OTHERS THEN
            dbms_output.put_line('code=' || SQLCODE || ', message=''' || SQLERRM || '''');
    END;

    CALL test_raise_app_err(1);     -- 출력: code=1001, message='my error 1'
    CALL test_raise_app_err(2);     -- 출력: code=1002, message='my error 2'
    CALL test_raise_app_err(3);     -- 출력: code=1003, message='my error 3'

.. _exec_imme:

EXECUTE IMMEDIATE
=================

:ref:`Dynamic SQL <dyn_sql>` 절에서 설명한 바와 같이
실행 시간에 임의의 SQL을 문자열로 구성하여 EXECUTE IMMDIATE 문을 통해 실행할 수 있다.
USING 절을 써서 프로그램의 어떤 값을 SQL문의 호스트 변수 자리에 채우는 것이 가능하다.
INTO 절을 써서 SELECT 문의 조회 결과를 프로그램의 변수나 OUT 인자에 담아오는 것도 가능하다.
이 때 조회 결과 값들의 개수는 INTO 절 안의 변수나 OUT 인자의 개수와 일치해야 하고
값들은 대응되는 변수나 OUT 인자에 대입 가능한 타입을 가져야 한다.

SQL 문 실행 중에 에러가 나면 SQL_ERROR Exception이 발생한다.
INTO 절을 포함한 경우 SELECT 문의 조회 결과는 단 한 건의 결과 레코드를 가져야 한다.
결과가 없을 때는 NO_DATA_FOUND Exception이 발생하고 결과가 두 건 이상일 때는 TOO_MANY_ROWS Exception이 발생한다.

::

    <execute_immediate> ::=
        EXECUTE IMMEDIATE <dynamic_sql> { [ <into_clause> ] [ <using_clause> ] | <using_clause> <into_clause> }
        <using_clause> ::= USING <using_element> [ , <using_element>, ... ]
        <using_element> ::= [ IN ] <expression>
        <into_clause> ::= INTO <identifier> [ , <identifier>, ... ]


* *dynamic_sql*: 문자열 타입을 갖는 표현식. 표현식은 SQL 규약에 맞는 SQL 구문 문자열을 계산 결과로 가져야 한다.
  SQL 구문 내부에서 값을 필요로 하는 자리에 ?(물음표)를 대신 쓸 수 있으며 사용한 ?의 개수와 *using_clause*\에
  포함된 표현식의 개수는 일치해야 한다.
* *using_clause*: *dynamic_sql*\을 실행할 때 문자열의 ? 자리에 채워질 값들을 지정한다. BOOLEAN이나 SYS_REFCURSOR 타입을 갖는 표현식을 가질 수 없다. :ref:`%ROWTYPE <percent_rowtype>`\으로 선언된 레코드 타입 값이나 커서도 표현식 자리에 올 수 없다.
* *into_clause*: *dynamic_sql*\이 SELECT문을 나타내는 경우에 조회 결과를 담을 변수나 OUT 인자를 지정한다. *dynamic_sql*\이 SELECT문을 나타내는데 INTO 절이 없거나 *dynamic_sql*\이 SELECT문을 나타내지 않는데 INTO 절이 있으면 SQL_ERROR Exception이 발생한다.

다음은 EXECUTE IMMEDIATE의 사용 예이다.

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

대입문
======
::

    <assignment_statement> ::=
        <identifier> := <expression>

* *identifier*: 변수이거나 OUT 인자이어야 한다.
* *expression*: 대입될 값을 계산하는 표현식. 아래 표현식 절 참조

*expression*\의 타입은 *identifier*\의 타입과 같거나 *identifier*\의 타입으로 형변환이 가능해야 한다.
그렇지 않으면 컴파일 과정에서 에러가 발생한다.

CONTINUE, EXIT
===============
::

    <continue_statement> ::=
        CONTINUE [ <label_name> ] [ WHEN <expression> ]

::

    <exit_statement> ::=
        EXIT [ <label_name> ] [ WHEN <expression> ]


CONTINUE와 EXIT 문은 루프문 안에서만 사용할 수 있다.
CONTINUE 문은 아래쪽으로의 실행 흐름을 멈추고 루프의 처음으로 분기해서 다음 iteration을 실행하도록 한다.
EXIT 문은 아래쪽으로의 실행 흐름을 멈추고 루프를 빠져나가 그 루프 다음 실행문으로 분기한다.
*label_name*\이 없는 경우 그 CONTINUE/EXIT 문을 포함하는 가장 안쪽의 루프를 재시작한다/빠져나간다.
*label_name*\이 있는 경우 그 CONTINUE/EXIT 문을 포함하는 루프들 중 하나에 선언된 것이어야 한다.
아니면 컴파일 과정에서 에러가 발생한다.
루프가 여럿 중첩된 경우 *label_name*\을 지정하여 분기할 루프를 지정할 수 있다.
WHEN 절이 있는 경우 BOOLEAN 타입의 *expression*\이 TRUE로 계산될 경우에만 분기한다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_continue_exit
    AS
        i INT := 0;
    BEGIN
        LOOP
            DBMS_OUTPUT.put_line(i);            -- 0, 1, 2, 3, 4, 5
            i := i + 1;
            CONTINUE WHEN i < 3;
            DBMS_OUTPUT.put_line(i);            -- 3, 4, 5
            EXIT WHEN i = 5;
        END LOOP;

        DBMS_OUTPUT.put_line(i);                -- 5
    END;

NULL
====
::

    <null_statement> ::=
        NULL

아무 일도 하지 않는다는 것을 명시적으로 표시하고 싶을 경우,
혹은 나중에 구현할 실행문 자리를 임시로 채워 넣고 싶을 경우 등에 NULL을 사용할 수 있다.
문법상 실행문 자리를 채우기 위한 place holder 구문이다.

.. _raise:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_null(medal CHAR)
    AS
    BEGIN
        CASE medal
            WHEN 'G' THEN
                DBMS_OUTPUT.put_line('Gold');
            WHEN 'S' THEN
                DBMS_OUTPUT.put_line('Silver');
            WHEN 'B' THEN
                DBMS_OUTPUT.put_line('Bronze');
            ELSE
                NULL;
        END CASE;
    END;

RAISE
=====
::

    <raise_statement> ::=
        RAISE [ <identifier> ]

Exception을 발생시킨다.
Exception 이름 *identifier*\는 :ref:`시스템 Exception <exception>`\이거나
:ref:`사용자가 선언 <exception_decl>`\한 것이어야 한다.
Exception의 THEN 절 안의 RAISE는 Exception 이름을 생략할 수 있다.
이 경우, 현재 처리 중인 Exception을 일으키는 것으로 동작한다.

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
            RAISE;      -- 현재 처리 중인 invalid_input을 다시 일으킴
        WHEN OTHERS THEN
            DBMS_OUTPUT.put_line('unknown exception');
            RAISE;      -- 현재 처리 중인 Exception을 다시 일으킴
    END;

RETURN
======
::

    <return_statement> ::=
        RETURN [ <expression> ]

현재 루틴을 호출한 호출문 다음으로 분기한다.
현재 루틴이 함수인 경우에는 그 함수의 리턴 타입으로 변환 가능한 반환값 *expression*\을 지정해야 한다.
현재 루틴이 함수가 아닌 프로시저인 경우에는 반환값을 지정하면 에러이다.

프로시저 호출문
===============
::

    <procedure_call> ::=
        <identifier> [ <function_argument> ]
    <function_argument> ::= ( [ <expression> [ , <expression>, ... ] ] )

이름 *identifier*\로 지정된 프로시저를 인자 *function_argument*\를 주어 호출한다.
인자 개수와 각각의 타입은 해당 프로시저의 선언과 일치해야 한다.
호출되는 프로시저의 OUT 인자에 주어질 인자들은 프로시저 호출 결과로 변경이 될 것이므로
대입이 가능한 변수나 다른 OUT 인자이어야 한다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE callee(o OUT INT)
    AS
    BEGIN
        ...
    END;

    CREATE OR REPLACE PROCEDURE caller(i INT, o OUT INT)
    AS
        v INT;
        c CONSTANT INT := 0;
    BEGIN
        callee(i);   -- Error: IN 인자
        callee(o);   -- OK: OUT 인자
        callee(v);   -- OK: 변수
        callee(c);   -- Error: 상수
    END;

호출되는 프로시저는 저장 프로시저이거나 내부 프로시저이다.
다른 저장 프로시저 호출문 실행 중에 문제가 발생했을 때는 SQL_ERROR Exception이 발생한다.

IF
==
::

    <if_statement> ::=
        IF <expression> THEN <seq_of_statements> [ <elsif_part> [ <elsif_part> ... ] ] [ <else_part> ] END IF
    <elsif_part> ::= ELSIF <expression> THEN <seq_of_statements>
    <else_part> ::= ELSE <seq_of_statements>

일반적인 프로그래밍 언어가 제공하는 If-Then-Else 문을 제공한다.
IF와 ELSIF 다음의 *expression*\는 BOOLEAN 타입이어야 한다.

.. _loop:

LOOP
====
PL/CSQL이 제공하는 루프문은 아래와 같이 다섯 가지 형태가 있다.
앞의 세 가지는 일반적인 프로그래밍 언어에서 제공하는 루프문과 유사하다.
뒤의 두 가지는 SELECT 문의 조회 결과를 순회하는 용도로 사용한다.
::

    <loop_statement> ::=
          <label_declaration>? LOOP <seq_of_statements> END LOOP                          # basic-loop
        | <label_declaration>? WHILE <expression> LOOP <seq_of_statements> END LOOP       # while-loop
        | <label_declaration>? FOR <iterator> LOOP <seq_of_statements> END LOOP           # for-iter-loop
        | <label_declaration>? FOR <for_cursor> LOOP <seq_of_statements> END LOOP         # for-cursor-loop
        | <label_declaration>? FOR <for_static_sql> LOOP <seq_of_statements> END LOOP     # for-static-sql-loop

    <label_declaration> ::= '<<' <identifier> '>>'

    <iterator> ::= <identifier> IN [ REVERSE ] <lower_bound> .. <upper_bound> [ BY <step> ]

    <for_cursor>      ::= <record> IN <cursor> [ <function_argument> ]
    <for_static_sql>  ::= <record> IN ( <select_statement> )

* *label_declaration*: 오직 루프문 시작 부분에서만 라벨 선언을 할 수 있다. 이 라벨은 루프 바디 안 쪽의 CONTINUE 문이나 EXIT 문이 분기 기준이 될 루프를 지정하는데 사용된다.
* *while-loop* 형태의 루프에서 조건 *expression*\은 BOOLEAN 타입이어야 한다.
* *for-iter-loop* 형태의 루프에서 *lower_bound*, *upper_bound*, *step*\은 모두 INTEGER로 변환가능한 타입을 가져야 한다. 실행시간에 step 값이 0 이하이면 VALUE_ERROR Exception이 발생한다. REVERSE가 지정되지 않은 경우, *identifier*\는 *lower_bound*\로 초기화 된 후 *upper_bound*\보다 작거나 같다는 조건을 만족하면 루프 바디를 한번 실행하고 그 이후는 *step* 만큼 증가한 값이 *upper_bound*\보다 작거나 같다는 조건을 만족하는 한 반복한다.  REVERSE가 지정된 경우에는, *identifier*\는 *upper_bound*\로 초기화 된 후 *lower_bound*\보다 크거나 같다는 조건을 만족하면 루프 바디를 한번 실행하고 그 이후는 *step*\만큼 감소한 값이 *lower_bound*\보다 크거나 같다는 조건을 만족하는 한 반복한다. 루프 변수 *identifier*\는 루프 바디 안에서 INTEGER 타입 변수로 사용될 수 있다.
* *for-cursor-loop*, *for-static-sql-loop* 형태의 FOR 루프는 *record* IN 다음에 기술하는 커서나 SELECT 문의 조회 결과들을 순회하기 위해 사용된다. 이 때 사용되는 SELECT 문에 INTO 절이 있으면 컴파일 과정에서 에러가 발생한다. 매 iteration 마다 조회 결과가 한 row 씩 *record*\에 할당된 상태로 루프 바디가 실행된다. 이 때, 결과 row의 각 컬럼들은 루프 바디 안에서 *record*. *column* 모양으로 참조할 수 있다.

기본 형태 LOOP는 보통 아래와 같이 반복 종료를 위한 조건을 내부에 가지게 된다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE print_nation_athletes(nation CHAR)
    AS
        code INT;
        name VARCHAR(40);
        CURSOR c IS SELECT code, name from athlete where nation_code = nation;
    BEGIN
        OPEN c;
        LOOP
            FETCH c INTO code, name;
            EXIT WHEN c%NOTFOUND;
            DBMS_OUTPUT.PUT_LINE('code: ' || code || ' name: ' || name);
        END LOOP;
        CLOSE c;
    END;

다음은 While Loop 구문의 간단한 사용 예를 보여준다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION sum_upto(n INT) RETURN INT
    AS
        sum INT := 0;
        i INT := 1;
    BEGIN
        WHILE i <= n LOOP
            sum := sum + i;
            i := i + 1;
        END LOOP;

        RETURN sum;
    END;


다음은 For-Iterator Loop 구문의 사용 예를 보여준다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE mult_tables
    AS
    BEGIN
        FOR i IN 2 .. 9 LOOP
            DBMS_OUTPUT.put_line('table ' || i);

            FOR j IN 1 .. 9 LOOP
                DBMS_OUTPUT.put_line(i || ' x ' || j || ' = ' || i*j);
            END LOOP;

            DBMS_OUTPUT.put_line('');
        END LOOP;
    END;

다음은 동일한 SELECT 문을 두 가지 다른 형태의 For Loop으로 조회하는 예를 보여준다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE athlete_history(p_name VARCHAR)
    AS
        CURSOR my_cursor IS
        SELECT host_year, score
        FROM history
        WHERE athlete = p_name;
    BEGIN
        -- For-Cursor Loop
        FOR r IN my_cursor LOOP
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);
        END LOOP;

        -- For-Select Loop
        FOR r IN (SELECT host_year, score FROM history WHERE athlete = p_name) LOOP
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);
        END LOOP;
    END;

.. _case_stmt:

CASE 실행문
===========
CASE 문은 여러 개의 조건을 순차적으로 검사해서 가장 처음 만족하는 조건에 연관되어 있는 실행문들을 실행한다.

::

    <case_statement> ::=
          CASE <expression> { WHEN <expression> THEN <seq_of_statements> }... [ ELSE <seq_of_statements> ] END CASE
        | CASE { WHEN <expression> THEN <seq_of_statements> }... [ ELSE <seq_of_statements> ] END CASE

CASE 문은 두 가지 형태가 있다.

* 첫번째 형태는 CASE 키워드 직후에 표현식을 갖는다. 우선 이 최초 표현식을 계산한 다음, 이후 WHEN 절의 표현식을 하나씩 차례로 계산해서 최초 표현식과 일치하는 값을 찾고, 해당 THEN 절의 실행문들을 실행한다. 최초 표현식은 단 한번 계산된다.
* 두번째 형태는 CASE 키워드 직후에 표현식을 갖지 않는다. CASE 키워드 이후 여러 개의 WHEN 절의 표현식은 BOOLEAN 타입을 가져야 한다. 이들 표현식을 하나씩 차례로 계산하다가 처음으로 TRUE 값이 되는 표현식이 발견되면 해당 THEN 절의 실행문을 실행한다.

두 형태 모두 선택적으로 ELSE 절을 가질 수 있다. 이는 조건을 만족하는 WHEN 이후 표현식을 찾지 못했을 경우에 실행할 실행문들을 지정한다. 조건을 만족하는 WHEN 절이 없고 ELSE 절도 없을 때는 CASE_NOT_FOUND라는 시스템 Exception이 발생한다.

다음은 첫 번째 형태의 CASE 문 예제이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE print_even_odd(i INTEGER)
    AS
    BEGIN
        CASE i % 2
            WHEN 0 THEN
                DBMS_OUTPUT.put_line('Even');
            WHEN 1 THEN
                DBMS_OUTPUT.put_line('Odd');
            ELSE
                DBMS_OUTPUT.put_line('Null');
        END CASE;
    END;

다음은 유사한 동작을 하는 두 번째 형태의 CASE 문 예제이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE print_even_odd(i INTEGER)
    AS
    BEGIN
        CASE
            WHEN i % 2 = 0 THEN
                DBMS_OUTPUT.put_line('Even');
            WHEN i % 2 = 1 THEN
                DBMS_OUTPUT.put_line('Odd');
            ELSE
                DBMS_OUTPUT.put_line('Null');
        END CASE;
    END;

******************
표현식
******************
PL/CSQL의 표현식의 종류는 다음 문법으로 요약할 수 있다.
::

    <expression> ::=
          <literal>                                 # 상수
        | <identifier>                              # 식별자
        | SQL%ROWCOUNT                              # Static SQL 결과 크기
        | <cursor_expression> <cursor_attribute>    # 커서 속성
        | <expression> <binary_op> <expression>     # 이항 연산
        | <unary_op> <expression>                   # 단항 연산
        | ( <expression> )                          # 괄호
        | <identifier>.<identifier>                 # 레코드 필드 참조
        | <identifier> <function_argument>          # 함수 호출
        | <case_expression>                         # CASE 표현식
        | SQLCODE                                   # Exception 코드
        | SQLERRM                                   # Exception 메시지
        | <expression> IS [ NOT ] NULL              # IS NULL 표현식
        | <expression> [ NOT ] BETWEEN <expression> AND <expression>        # BETWEEN 표현식
        | <expression> [ NOT ] IN ( <expression> [ , <expression>, ... ] )  # IN 표현식
        | <expression> [ NOT ] LIKE <expression> [ ESCAPE <expression> ]    # LIKE 표현식

    <literal> ::=
          DATE <quoted_string>
        | TIME <quoted_string>
        | DATETIME <quoted_string>
        | TIMESTAMP <quoted_string>
        | <numeric>
        | <quoted_string>
        | NULL
        | TRUE
        | FALSE

    <numeric> ::= UNSIGNED_INTEGER | FLOATING_POINT_NUM

    <cursor_attribute> ::= { %ISOPEN | %FOUND | %NOTFOUND | %ROWCOUNT }

    <binary_op> ::=
          AND | XOR | OR
        | = | <=> | != | <> | <= | >= | < | >
        | * | / | + | -
        | >> | << | & | ^ | '|'
        | ||

    <unary_op> ::= + | - | NOT | ~

    <case_expression> ::=
          CASE <expression> <case_expression_when_part>... [ ELSE <expression> ] END
        | CASE <case_expression_when_part>... [ ELSE <expression> ] END

    <case_expression_when_part> ::= WHEN <expression> THEN <expression>

리터럴
=================
리터럴에는 날짜/시간, 숫자, 문자열, NULL, TRUE, FALSE 값이 있다.
비트열과 컬렉션을 사용할 수 없다는 점을 제외하고 리터럴 관련 규칙은 :ref:`SQL 리터럴 <sql_literal>`\과 동일하다.

식별자
=================
Static/Dynamic SQL 밖의 PL/CSQL 문에서 사용할 수 있는 식별자에는 다음 세 가지 종류가 있다.

* 선언부에서 선언된 변수, 상수, 커서, Exception, 내부 프로시저/함수
* 프로시저/함수의 인자
* 묵시적으로 선언된 :ref:`For 루프<loop>`\의 iterator. integer 타입이거나 record 타입

명시적 혹은 묵시적 선언 없이 식별자를 사용하면 컴파일 에러가 발생한다.

Static SQL 결과 크기
====================
SQL%ROWCOUNT는 Static SQL을 실행한 직후에 결과 크기를 나타내는 표현식이다.

* 커서와 연관되지 않은 SELECT 문의 경우 반드시 INTO 절을 사용해야 하며 조회 결과는 1개이어야 한다. 따라서, 이 SELECT 문이 정상적으로 수행되었을 때 SQL%ROWCOUNT의 값은 1이다. 조회 결과 크기가 0이거나 1을 초과해서 실행시간 에러가 발생했을 때는 SQL%ROWCOUNT의 값은 정의되지 않는다.
* INSERT, UPDATE, DELETE, MERGE, REPLACE, TRUNCATE 문의 경우 영향 받은 레코드 개수가 된다.
* COMMIT, ROLLBACK 문에 대해서는 0이 된다.

커서 속성
=================

커서나 SYS_REFCURSOR 변수를 계산 결과로 갖는 표현식 *cursor_expression*\에
%ISOPEN, %FOUND, %NOTFOUND, %ROWCOUNT 기호를 덧붙여 그 커서의 네 가지 속성을 조회할 수 있다.

* %ISOPEN: 커서가 열려 있는지 여부 (BOOLEAN)
* %FOUND: 첫 번째 FETCH 이전이면 NULL. 아니면 마지막 FETCH가 1개의 ROW를 결과로 갖는지 여부 (BOOLEAN). 열려 있지 않은 커서에 대해서 조회하면 INVALID_CURSOR Exception 발생.
* %NOTFOUND: 첫 번째 FETCH 이전이면 NULL. 아니면 마지막 FETCH가 0개의 ROW를 결과로 갖는지 여부 (BOOLEAN). 열려 있지 않은 커서에 대해서 조회하면 INVALID_CURSOR Exception 발생.
* %ROWCOUNT: 첫 번째 FETCH 이전이면 NULL. 아니면 현재까지 FETCH된 ROW의 개수 (BIGINT). 열려 있지 않은 커서에 대해서 조회하면 INVALID_CURSOR Exception 발생.

아래 예제에서 내부 함수 iterate_cursor()는 커서 속성을 사용하여 레코드들을 순회하고 전체 레코드 개수를 리턴한다.
인자로 넘겨 받은 커서가 열려 있지 않을 때는 (커서의 %ISOPEN 속성이 False일 때는) -1을 리턴한다.
더 이상 조회할 레코드가 없는지는 FETCH 후 커서의 %NOTFOUND 속성을 검사해서 알아낸다.
커서의 %ROWCOUNT 속성은 FETCH 문으로 조회된 레코드 각각마다 1씩 증가하다가
FETCH 반복문이 종료된 후에는 조회된 전체 레코드 개수를 나타내게 된다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE cursor_attributes AS
        ...

        FUNCTION iterate_cursor(rc SYS_REFCURSOR) RETURN INT
        AS
            v VARCHAR;
        BEGIN
            IF rc%ISOPEN THEN
                LOOP
                    FETCH rc INTO v;
                    EXIT WHEN rc%NOTFOUND;

                    -- do something with v
                    ...

                END LOOP;

                RETURN rc%ROWCOUNT;     -- number of records
            ELSE
                RETURN -1;              -- error
            END IF;
        END;
    begin
        ...

    end;

*cursor_expression*\의 계산 결과가 NULL이면 INVALID_CURSOR Exception이 발생한다.

이항 연산, 단항 연산, 괄호
==========================

PL/CSQL은 다음과 같이 연산자 우선 순위를 갖는다.

+--------------------------------------------------------------------+-------------------------------------+
| 연산자                                                             | 연산                                |
+====================================================================+=====================================+
| +, -, ~                                                            | 부호, 비트역 (단항)                 |
+--------------------------------------------------------------------+-------------------------------------+
| \*, /, DIV, MOD                                                    | 곱하기, 나누기, 정수 나누기, 나머지 |
+--------------------------------------------------------------------+-------------------------------------+
| +, -                                                               | 더하기, 빼기                        |
+--------------------------------------------------------------------+-------------------------------------+
| ||                                                                 | 문자열 병합                         |
+--------------------------------------------------------------------+-------------------------------------+
| <<, >>                                                             | 비트 이동                           |
+--------------------------------------------------------------------+-------------------------------------+
| &                                                                  | 비트곱                              |
+--------------------------------------------------------------------+-------------------------------------+
| ^                                                                  | 배타적 비트합                       |
+--------------------------------------------------------------------+-------------------------------------+
| \|                                                                 | 비트합                              |
+--------------------------------------------------------------------+-------------------------------------+
| IS NULL                                                            | NULL 테스트                         |
+--------------------------------------------------------------------+-------------------------------------+
| LIKE                                                               | 문자열 패턴 테스트                  |
+--------------------------------------------------------------------+-------------------------------------+
| BETWEEN                                                            | 값 범위 테스트                      |
+--------------------------------------------------------------------+-------------------------------------+
| IN                                                                 | 값 포함 테스트                      |
+--------------------------------------------------------------------+-------------------------------------+
| =, <=>, <, >, <=, >=, <>, !=,                                      | 비교                                |
+--------------------------------------------------------------------+-------------------------------------+
| NOT                                                                | 논리역                              |
+--------------------------------------------------------------------+-------------------------------------+
| AND                                                                | 논리곱                              |
+--------------------------------------------------------------------+-------------------------------------+
| XOR                                                                | 배타적 논리합                       |
+--------------------------------------------------------------------+-------------------------------------+
| OR                                                                 | 논리합                              |
+--------------------------------------------------------------------+-------------------------------------+

* %는 Static/Dynamic SQL 밖에서는 MOD와 동일한 의미의 나머지 연산자로 사용할 수 없다.
* &&, ||, !은 Static/Dynamic SQL 밖에서는 AND, OR, NOT과 동일한 의미의 논리 연산자로 사용할 수 없다.
* Static/Dynamic SQL 밖에서의 문자열은 DB 설정과 상관없이 UTF8 encoding을 따르며
  이들 문자열들 사이의 비교는 해당 Unicode 배열들 사이의 사전식 비교법을 따른다.
  Static/Dynamic SQL 안에서의 문자열의 encoding과 비교는 DB와 테이블 설정을 따른다.

명시적으로 연산 순서를 지정하기 위해 괄호를 사용할 수 있다.

레코드 필드 참조
=================

PL/CSQL에서는 다음 두 가지 경우에 레코드 변수를 사용할 수 있다.

* FOR 문에서 SELECT 결과를 순회하기 위해 묵시적으로 선언되는 레코드 변수
* %ROWTYPE으로 선언된 레코드 변수

레코드 변수에 대해서 필드 이름을 덧붙여 레코드 필드를 참조할 수 있다.

.. code-block:: sql

    CREATE PROCEDURE athlete_history(p_name VARCHAR)
    AS
        CURSOR my_cursor IS
        SELECT host_year, score
        FROM history
        WHERE athlete = p_name;
    BEGIN
        FOR r IN my_cursor LOOP     -- r: 묵시적으로 선언됨
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);    -- r.<column-name>
        END LOOP;
    END;

함수 호출
=================

함수 호출 표현식에서 인자 개수와 각각의 타입은 해당 함수의 선언과 일치해야 한다.
호출되는 함수의 OUT 인자에 주어질 인자들은 호출 결과 변경이 일어나게 되므로
대입이 가능한 변수나 다른 OUT 인자이어야 한다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION callee(o OUT INT) RETURN INT
    AS
    BEGIN
        ...
    END;

    CREATE OR REPLACE PROCEDURE caller(i INT, o OUT INT)
    AS
        v INT;
        c CONSTANT INT := 0;
    BEGIN
        ... callee(i) ...   -- Error: IN 인자
        ... callee(o) ...   -- OK: OUT 인자
        ... callee(v) ...   -- OK: 변수
        ... callee(c) ...   -- Error: 상수
    END;

호출되는 함수는 저장 함수, 내부 함수, 빌트인 함수 이렇게 세 가지 종류이다.
이 중에서 빌트인 함수는 :ref:`연산자와 함수 <operators-and-functions>` 장에 나열된 큐브리드 내장 함수들을 말한다.
단, 빌트인 함수들 중에서 PL/CSQL 문법과 충돌을 일으키는 :ref:`IF <func_if>`\는 사용할 수 없다.

다른 저장 함수나 빌트인 함수 호출문 실행 중에 에러가 나면 SQL_ERROR Exception이 발생한다.

CASE 표현식
=================

CASE 표현식은 여러 개의 조건을 순차적으로 검사해서 가장 처음 만족하는 조건에 연관되어 있는 값을 갖는다.

CASE 표현식은 :ref:`CASE 실행문 <case_stmt>`\(Statement)과 마찬가지로 CASE 키워드 직후에 표현식을 갖는 형태와 갖지 않는 형태가 있다.

* CASE 키워드 직후에 표현식을 갖는 형태에서는 우선 이 최초 표현식을 계산한 다음, WHEN 절들의 표현식을 하나씩 차례로 계산해서 최초 표현식과 일치하는 값을 찾고, 해당 THEN 절의 표현식을 계산해서 CASE문의 최종값으로 한다. 최초 표현식은 단 한번 계산된다.
* CASE 키워드 직후에 표현식을 갖지 않는 형태에서는 CASE 키워드 이후 여러 개의 WHEN 절의 표현식은 BOOLEAN 타입을 가져야 한다. 이들 표현식을 하나씩 차례로 계산하다가 처음으로 TRUE 값이 되는 표현식이 발견되면 해당 THEN 절의 표현식을 계산해서 CASE문의 최종값으로 한다.

두 형태 모두 선택적으로 ELSE 절을 가질 수 있다. 이는 조건을 만족하는 WHEN 이후 표현식을 찾지 못했을 경우에 값으로 가질 표현식을  지정한다. 조건을 만족하는 WHEN 절이 없고 ELSE 절도 없을 때 전체 CASE 표현식은 NULL 값을 갖는다.

다음은 첫 번째 형태의 CASE 표현식 예제이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE print_even_odd(i INTEGER)
    AS
        s VARCHAR(5);
    BEGIN
        s := CASE i % 2
            WHEN 0 THEN 'Even'
            WHEN 1 THEN 'Odd'
            ELSE 'NULL'
        END;

        DBMS_OUTPUT.put_line(s);
    END;

다음은 유사한 동작을 하는 두 번째 형태의 CASE 표현식 예제이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE print_even_odd(i INTEGER)
    AS
        s VARCHAR(5);
    BEGIN
        s := CASE
            WHEN i % 2 = 0 THEN 'Even'
            WHEN i % 2 = 1 THEN 'Odd'
            ELSE 'NULL'
        END;

        DBMS_OUTPUT.put_line(s);
    END;

.. _sqlcode:

SQLCODE, SQLERRM
=================

Exception 처리 block 안에서 SQLCODE와 SQLERRM은 각각 현재 처리 중인 Exception의 코드(INTEGER 타입)와
에러메시지(STRING 타입)를 나타낸다.
Exception 처리 block 밖에서 SQLCODE와 SQLERRM은 각각 0과 'no error' 값을 갖는다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_sql_code_errm
    AS
    BEGIN
        ...
    EXCEPTION
        WHEN OTHERS THEN
            DBMS_OUTPUT.put_line('code=' || SQLCODE);
            DBMS_OUTPUT.put_line('error message' || SQLERRM);
    END;

