
:meta-keywords: cubrid sql, pl/csql
:meta-description: This chapter describes PL/CSQL Spec.

*****************************
Overview
*****************************

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

저장 프로시저/함수 안에서 실행되는 COMMIT, ROLLBACK 문의 의미는
그 저장 프로시저/함수가 Autonomous Transaction으로 설정되어 있는가 아닌가에 따라 달라진다.
관련 내용은 :ref:`Autonomous Transaction 선언 <auto_tran>`\을 참고한다.

다음은 PL/CSQL을 사용해서 작성한 저장 프로시저/함수의 예이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE insert_athlete(
        p_name VARCHAR,
        p_gender VARCHAR,
        p_nation_code VARCHAR,
        p_event VARCHAR)
    AS
        PRAGMA AUTONOMOUS_TRANSACTION;
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

.. _static_sql:

Static SQL
==================

SQL 구문 중에 다음에 해당하는 것들을 PL/CSQL 실행문으로 직접 사용할 수 있으며,
그러한 경우를 Static SQL 문이라고 부른다.

* SELECT (CTE, UNION, INTERSECT, MINUS 포함)
* INSERT, UPDATE, DELETE, MERGE, REPLACE
* COMMIT, ROLLBACK
* TRUNCATE

위 목록에 포함되지 않는 다른 SQL 문들은 직접 사용할 수는 없으나,
아래에서 설명하는 Dynamic SQL 문을 써서 실행할 수 있다.

SELECT 문은 실행문으로 사용될 뿐만 아니라 커서를 선언할 때나 OPEN-FOR 문에도 사용된다.

Static SQL 문의 WHERE 절이나 VALUES 절 안에서처럼 값을 필요로 하는 자리에
프로그램에서 선언한 변수나 프로시저/함수 파라미터를 쓸 수 있다.
그리고, SELECT 문의 INTO 절에 프로그램의 변수나 OUT 파라미터를 써서 조회 결과를 담을 수 있다.

SQL 구문의 문법과 의미는 CUBRID 매뉴얼 중
`CUBRID SQL <https://www.cubrid.org/manual/ko/11.2/sql/index.html>`_\을 참고하도록 한다.

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

식별자, 예약어, 주석, 리터럴을 작성할 때 Static/Dynamic SQL 안에서는
`SQL의 작성 규칙 <https://www.cubrid.org/manual/ko/11.2/sql/syntax.html>`_\을 따른다.

Static/Dynamic SQL 밖의 PL/CSQL 문 작성 규칙도 대체로 같은 규칙을 따르지만 다음 몇 가지 예외가 있다.

* SQL과 달리 식별자에 '#'을 쓸 수 없다. 즉, 식별자는 영문 대소문자, 한글, 숫자, '_'(underscore)로만 이루어져야 한다.
* 큰따옴표, 대괄호, 백틱 부호로 둘러싸더라도 식별자에 특수 문자를 쓸 수 없다.
  영문 대소문자, 한글, 숫자, '_'(underscore)만 사용 가능하다.
* no_backslash_escapes 설정 파라미터 값과 관계 없이 backslash 문자는 escape 문자로 사용되지 않는다.
* oracle_style_empty_string 설정 파라미터 값과 관계 없이 빈 문자열을 NULL과 동일시하지 않는다.
* 비트열 리터럴을 사용할 수 없다. Static/Dynamic SQL 밖의 PL/CSQL 문에서는 비트열 타입을 지원하지 않는다.

.. rubric:: 허용되는 식별자의 예

::

    a
    a_b
    athleteName2
    "select"        // " "로 둘러싸인 예약어

.. rubric:: 허용되지 않는 식별자의 예

::

    1a              // 숫자로 시작
    a@b             // 특수문자
    athlete-name-2  // 특수문자
    [a@b]           // [ ]로 둘러싸더라도 특수문자 불가
    select          // 예약어

PL/CSQL의 예약어는 아래 표에 나열되어 있다.
Static/Dynamic SQL 밖의 PL/CSQL 문에서 아래 표의 단어들을 변수, 상수, Exception, 내부 프로시저/함수
등의 이름을 나타내는 식별자로 쓸 수 없다.
단, SQL 문에서처럼 큰따옴표(" "), 대괄호([ ]), 백틱(\` \`)으로 감싸면 식별자로 쓸 수 있다.

+---------------------------------------------------------------------------------------+
|   AND, AS, AUTONOMOUS_TRANSACTION                                                     |
+---------------------------------------------------------------------------------------+
|   BEGIN, BETWEEN, BIGINT, BOOLEAN, BY                                                 |
+---------------------------------------------------------------------------------------+
|   CASE, CHAR, CHARACTER, CLOSE, COMMIT, CONSTANT, CONTINUE, CREATE, CURSOR            |
+---------------------------------------------------------------------------------------+
|   DATE, DATETIME, DBMS_OUTPUT, DEC, DECIMAL, DECLARE, DEFAULT, DELETE, DIV, DOUBLE    |
+---------------------------------------------------------------------------------------+
|   ELSE, ELSIF, END, ESCAPE, EXCEPTION, EXECUTE, EXIT                                  |
+---------------------------------------------------------------------------------------+
|   FALSE, FETCH, FLOAT, FOR, FUNCTION                                                  |
+---------------------------------------------------------------------------------------+
|   IF, IMMEDIATE, IN, INOUT, INSERT, INT, INTEGER, INTO, IS                            |
+---------------------------------------------------------------------------------------+
|   LANGUAGE, LIKE, LOOP                                                                |
+---------------------------------------------------------------------------------------+
|   MERGE, MOD                                                                          |
+---------------------------------------------------------------------------------------+
|   NOT, NULL, NUMERIC                                                                  |
+---------------------------------------------------------------------------------------+
|   OF, OPEN, OR, OUT                                                                   |
+---------------------------------------------------------------------------------------+
|   PLCSQL, PRAGMA, PRECISION, PROCEDURE                                                |
+---------------------------------------------------------------------------------------+
|   RAISE, REAL, REPLACE, RETURN, REVERSE, ROLLBACK                                     |
+---------------------------------------------------------------------------------------+
|   SELECT, SHORT, SMALLINT, SQL, SQLCODE, SQLERRM, STRING, SYS_REFCURSOR               |
+---------------------------------------------------------------------------------------+
|   THEN, TIME, TIMESTAMP, TRUE, TRUNCATE                                               |
+---------------------------------------------------------------------------------------+
|   UPDATE, USING                                                                       |
+---------------------------------------------------------------------------------------+
|   VARCHAR, VARYING                                                                    |
+---------------------------------------------------------------------------------------+
|   WHEN, WHILE, WITH, WORK                                                             |
+---------------------------------------------------------------------------------------+
|   XOR                                                                                 |
+---------------------------------------------------------------------------------------+

..
    (TODO) examples on comments and literals
..

.. _types:

데이터 타입
==================

Static/Dynamic SQL에서는 SQL에서 제공하는 모든
`데이터 타입 <https://www.cubrid.org/manual/ko/11.2/sql/datatype_index.html>`_\을 쓸 수 있다.

반면, Static/Dynamic SQL 밖의 PL/CSQL 문에서 사용할 수 있는 데이터 타입은
SQL에서 제공하는 데이터 타입 중 일부와 BOOLEAN, SYS_REFCURSOR이다.

* BOOLEAN: TRUE, FALSE, NULL을 값으로 가질 수 있다.
  CREATE PROCEDURE/FUNCTION 문에서 파라미터 타입이나 리턴 타입으로 BOOLEAN을 사용할 수는 없다.
  왜냐하면 SQL에 BOOLEAN 타입이 정의되어 있지 않기 때문이다.
  단, :ref:`내부 프로시저/함수 <local_routine_decl>`\를 선언할 때에는 파라미터 타입이나 리턴 타입으로
  BOOLEAN을 사용할 수 있다.
* SYS_REFCURSOR: 커서 변수를 선언할 때 사용한다.
  커서 변수의 용도는 :ref:`OPEN-FOR <cursor_manipulation>` 문을 참고한다.
  BOOLEAN과 마찬가지로 CREATE PROCEDURE/FUNCTION 문에서 파라미터 타입이나 리턴 타입으로 SYS_REFCURSOR를 사용할 수 없고,
  :ref:`내부 프로시저/함수 <local_routine_decl>`\에는 사용할 수 있다.

SQL에서 제공하는 데이터 타입 중 PL/CSQL에서 지원하는 것과 지원하지 않는 것은 다음과 같다.

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

Static/Dynamic SQL 밖의 PL/CSQL문에서 문자열 타입 CHAR와 VARCHAR를 사용할 때,
타 DBMS와의 호환성과 향후 확장성을 위해 길이를 지정하는 CHAR(n), VARCHAR(n) 형태를 문법적으로 지원한다.
하지만, 현재까지의 구현에서는 프로그램 동작 중에는 길이 지정 부분 '(n)'가 무시된다.
예를 들어, 아래 예제에서 VARCHAR(40)은 VARCHAR라고 쓴 것과 동일하게 동작한다.

현재, PL/CSQL은 사용자 정의 타입을 지원하지 않는다.

변수, 상수, 프로시저/함수, 커서를 선언할 때 위에서 열거된 지원 타입을 쓸 수 있다.
혹은, 변수나 테이블 컬럼 이름 뒤에 '%TYPE'을 덧붙여 해당 변수나 컬럼의 선언 타입을 나타낼 수도 있다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION get_athlete_name(p_code INTEGER) RETURN VARCHAR(40)
    AS
        name athlete.name%TYPE;
    BEGIN
        SELECT a.name
        INTO name
        FROM athlete a
        WHERE a.code = p_code;

        RETURN name;
    END;

연산자와 함수
==================

Static/Dynamic SQL에서는 SQL에서 제공하는 모든 연산자와 함수를 쓸 수 있다.
그리고, 연산자 ||와 +의 의미도 기존 SQL과 동일하게 서버 설정 파라미터 pipes_as_concat 값과 plus_as_concat 값을 따른다.
(참고: `연산자와 함수 <https://www.cubrid.org/manual/ko/11.2/sql/function/index.html>`_\,
`구문/타입 관련 파라미터 <https://www.cubrid.org/manual/ko/11.2/admin/config.html#stmt-type-parameters>`_)

반면, Static/Dynamic SQL 밖의 PL/CSQL 문에서는 SQL에서 제공하는 모든 연산자와 함수를
대부분 동일하게 쓸 수 있으나 다음의 몇 가지 예외들은 쓸 수 없다.

* 지원하지 않는 타입(BIT, ENUM, BLOB/CLOB, JSON, 등)의 값을 인자나 결과로 갖는 연산자와 함수
* 나머지 연산자 %

  + 단, 동일한 의미의 MOD를 대신 쓸 수 있음

* 논리 연산자 &&, ||, !

  + 단, 각각 동일한 의미의 AND, OR, NOT을 대신 쓸 수 있음
  + 특히, ||는 서버 설정 파라미터 pipes_as_concat 값이 no일지라도 논리합 연산자로 쓰이지 않음

* 서버 설정 파라미터 plus_as_concat 값이 yes일지라도 +가 문자열 병합 연산자로 쓰이지 않음

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

..
    ******************
    예외 처리
    ******************
..

시스템 Exception
======================

PL/CSQL은 다른 많은 프로그래밍 언어와 마찬가지로 Exception 핸들러를 통한 에러 처리를 지원한다
(참고: :ref:`Block 실행문 <block_stmt>`).
사용자가 프로그램 선언부에서 자신만의 Exception을 정의할 수 있지만,
주요 예외 상황에 대해서는 다음과 같이 시스템 Exception들이 미리 정의되어 있다.

+---------------------+------------------------------------------------------------------+
| CASE_NOT_FOUND      | CASE 문에서 조건이 참인 WHEN 절이 없고 ELSE 절도 없음            |
+---------------------+------------------------------------------------------------------+
| CURSOR_ALREADY_OPEN | 이미 열려 있는 커서에 다시 열기 시도                             |
+---------------------+------------------------------------------------------------------+
| INVALID_CURSOR      | 허용되지 않는 커서 조작 (예: 열려 있지 않은 커서를 닫으려고 함)  |
+---------------------+------------------------------------------------------------------+
| NO_DATA_FOUND       | SELECT INTO 문 실행 결과 0개의 Row가 반환됨                      |
+---------------------+------------------------------------------------------------------+
| PROGRAM_ERROR       | 시스템 내부 에러                                                 |
+---------------------+------------------------------------------------------------------+
| STORAGE_ERROR       | 메모리 부족으로 인한 할당 실패                                   |
+---------------------+------------------------------------------------------------------+
| SQL_ERROR           | Static/Dynamic SQL 실행 실패                                     |
+---------------------+------------------------------------------------------------------+
| TOO_MANY_ROWS       | SELECT INTO 문 실행 결과 2개 이상의 Row가 반환됨                 |
+---------------------+------------------------------------------------------------------+
| VALUE_ERROR         | 잘못된 값에 의한 에러                                            |
+---------------------+------------------------------------------------------------------+
| ZERO_DIVIDE         | 0으로 나누기 시도                                                |
+---------------------+------------------------------------------------------------------+

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

서버 설정 적용 예외
==========================

Static/Dynamic SQL 문의 동작은 각종 `서버 설정 <https://www.cubrid.org/manual/ko/11.2/admin/config.html#id2>`_\의 영향을 받는다.

그러나, Static/Dynamic SQL 밖에서 PL/CSQL 문의 동작은 서버 설정 파라미터 적용에 몇 가지 예외가 있다.

* no_backslash_escapes 설정 파라미터값과 관계 없이 backslash 문자는 escape 문자로 사용되지 않는다.
* oracle_style_empty_string 설정 파라미터값과 관계 없이 빈 문자열을 NULL과 동일시하지 않는다.
* pipes_as_concat 설정 파라미터값과 상관없이 ||는 논리합(OR) 연산자로 사용할 수 없다.
* plus_as_concat 설정 파라미터값과 상관없이 +는 문자열 병합 연산자로 사용할 수 없다.

위 네 가지 파라미터에 대한 자세한 내용은
`구문/타입 관련 파라미터 <https://www.cubrid.org/manual/ko/11.2/admin/config.html#stmt-type-parameters>`_\를 참고한다.

.. _decl:

******************
선언문
******************

프로시저/함수 선언문, 그리고 Block 실행문에는 선언부 *seq_of_declare_specs*\가 존재한다.
선언부에서는 아래 문법에서 정의하는 바와 같이 변수, 상수, Exception, 커서,
내부 프로시저/함수, Autonomous Transaction 여부를 선언할 수 있다.
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
        | <autonomous_transaction_decl>

선언 가능한 각 항목에 대한 설명은 아래 내용을 참고한다.

변수 선언
=========

::

    <variable_decl> ::=
        <identifier> <type_spec> [ [ NOT NULL ] <initial_value_part> ] ;

    <type_spec> ::=
          <builtin_type>
        | <variable>%TYPE
        | <table>.<column>%TYPE
    <initial_value_part> ::= { := | DEFAULT } <expression>

* *builtin_type*: :ref:`데이터 타입 <types>` 절에서 설명한 시스템 제공 타입

변수 선언에 선택적으로 NOT NULL 조건과 초기값을 지정할 수 있다.
NOT NULL 조건이 지정된 경우 반드시 NULL이 아닌 초기값이 함께 지정되어야 한다.
선언할 때 초기값이 지정되지 않은 변수는 암묵적으로 NULL 값을 갖게 된다.

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

내부 프로시저/함수 선언이나 Block 실행문은 자신만의 선언부와 실행부를 가지면서 중첩된 scope을 이룬다.
안쪽 scope에서 바깥에서 선언한 변수와 동일한 이름의 변수를 선언하면 안쪽에서는 바깥쪽의 동일 이름이 가려진다.
이러한 "이름 가림"은 다른 종류의 이름(변수, 상수, 프로시저/함수 파라미터, Exception, 커서, 내부 프로시저/함수)들에
대해서도 마찬가지로 적용된다.
중첩된 scope에서 선언된  이름들은 그 scope이 끝나면 사라진다.

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
        -- 여기서 a = 3, b는 VARCHAR 타입

        -- Block 실행문
        DECLARE
            a INT := 7;
            b DATETIME;
        BEGIN
            -- 여기서 a = 7, b는 DATETIME 타입
        END;

        -- 다시 a = 3, b는 VARCHAR 타입
    END;

상수 선언
=========
::

    <constant_decl> ::=
        <identifier> CONSTANT <type_spec> [ NOT_NULL ] <value_part> ;

    <type_spec> ::=
          <builtin_type>
        | <variable>%TYPE
        | <table>.<column>%TYPE
    <value_part> ::= { := | DEFAULT } <expression>

* *builtin_type*: :ref:`데이터 타입 <types>` 절에서 설명한 시스템 제공 타입

상수 선언에는 필수적으로 값 지정이 포함되어야 한다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_constant
    AS
        a CONSTANT INT NOT NULL := 3;
        b CONSTANT VARCHAR := 's';
        --c CONSTANT FLOAT;        -- 에러
    BEGIN
        ...
    END;

Exception 선언
==============

::

    <exception_decl> ::=
        <identifier> EXCEPTION ;

사용자가 원하는 이름의 Exception을 선언할 수 있다.
이렇게 선언된 Exception을 :ref:`RAISE <raise>` 문과 예외처리의 :ref:`WHEN <block_stmt>` 구에서 사용할 수 있다.

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
        | <variable>%TYPE
        | <table>.<column>%TYPE

* *builtin_type*: :ref:`데이터 타입 <types>` 절에서 설명한 시스템 제공 타입

커서에도 프로시저/함수와 유사하게 파라미터를 선언할 수 있지만 오직 IN 파라미터만 선언할 수 있다는 차이가 있다.
이 파라미터를 *select_statement* 문 안에서 참조할 수 있다.
커서를 :ref:`OPEN <cursor_manipulation>` 할 때 이 파라미터에 실제 선언된 갯수와 타입이 일치하도록
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
반면, 아래 예제처럼 OPEN, FETCH, CLOSE 동작이 암묵적으로 이루어지는 For-Loop 문을 통해서 커서를 이용할 수도 있다.
이 경우에는 사용자가 명시적으로 커서를 닫아줄 필요가 없다.

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

.. _local_routine_decl:

내부 프로시저/함수 선언
========================

정의 중인 저장 프로시저/함수 안에서만 사용할 내부 프로시저/함수를 다음 문법에 따라 정의할 수 있다.
어느 정도 규모를 이루거나 두 번 이상 반복되는 연관된 실행 과정을 내부 프로시저/함수로 묶어 모듈화하면
프로그램 가독성이 높아지고 유지 보수에 도움이 된다.

::

    <inner_procedure_decl> ::=
        PROCEDURE <identifier> [ ( <seq_of_parameters> ) ] { IS | AS } [ <seq_of_declare_specs> ] <body> ;
    <inner_function_decl> ::=
        FUNCTION <identifier> [ ( <seq_of_parameters> ) ] RETURN <type_spec> { IS | AS } [ <seq_of_declare_specs> ] <body> ;

    <seq_of_parameters> ::= [ <parameter> [, <parameter> ...] ]
    <parameter> ::= <identifier> [ { IN | IN OUT | INOUT | OUT } ] <type_spec>
    <type_spec> ::=
          <builtin_type>
        | <variable>%TYPE
        | <table>.<column>%TYPE
    <body> ::= BEGIN <seq_of_statements> [ EXCEPTION <seq_of_handlers> ] END [ <label_name> ]
    <seq_of_declare_specs> ::= <declare_spec> [ <declare_spec> ... ]
    <seq_of_statements> ::= <statement> ; [ <statement> ; ... ]
    <seq_of_handlers> ::= <handler> [ <handler> ... ]
    <handler> ::= WHEN <exception_name> [ OR <exeption_name> OR ... ] THEN <seq_of_statements>
    <exception_name> ::= OTHERS | identifier

* *parameter*: 파라미터는 IN, IN OUT, INOUT, OUT 네 가지 경우로 선언할 수 있다.
* *builtin_type*: :ref:`데이터 타입 <types>` 절에서 설명한 시스템 제공 타입
* *body*: 필수적으로 하나 이상의 실행문과 선택적으로 몇 개의 Exception 핸들러로 구성된다.
* *declare_spec*: 변수, 상수, Exception, 커서, Autonomous Transaction, 내부 프로시저/함수 선언 중 하나
* *statement*: 아래 :ref:`실행문 <stmt>` 절 참조
* *handler*: 지정된 Exception이 발생했을 때 실행할 실행문들을 지정한다.
* *exception_name*: OTHERS인 경우 아직까지 매치되지 않은 모든 Exception에 매치되며 OR로 다른 exception 이름과 연결할 수 없다.  OTHERS가 아닌 경우는 시스템 Exception이거나 사용자 정의 Exception을 나타낸다.

함수 *body*\에서는 RETURN 절에 지정된 타입에 맞는 값을 반환해야 한다.
함수가 *body* 끝에 도달할 때까지 RETURN 문을 만나지 못하면 에러가 발생한다.
프로시저의 RETURN 문에 반환값을 지정하면 에러이다.

프로시저/함수를 선언하면 자기 자신을 실행부에서 참조할 수 있다. 즉, 재귀 호출이 가능하다.

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

.. _auto_tran:

Autonomous Transaction 선언
===========================
::

    <autonomous_transaction_decl> ::=
        PRAGMA AUTONOMOUS_TRANSACTION ;

이 선언문을 포함한 저장 프로시저/함수는 호출한 쪽의 트랜잭션에 포함되는 것이 아니라,
독자적인 자신만의 트랜잭션 안에서 실행된다.
이 경우에 저장 프로시저/함수 실행 도중에 DB 변경이 있었는데도 COMMIT이나 ROLLBACK이 실행되지 않으면 에러이다.
그리고, COMMIT이나 ROLLBACK을 실행해도 호출한 쪽의 트랜잭션의 진행에는 영향을 미치지 않는다.

Autonomous Transaction으로 선언되지 않은 저장 프로시저/함수는 호출한 쪽의 트랜잭션 안에 포함된다.
이 경우에는 저장 프로시저/함수 안에서 호출한 COMMIT이나 ROLLBACK은 호출한 쪽의 트랜잭션에 대해서 동작한다.

이 선언문은 최상위 선언부에서만 사용할 수 있다.
즉, 내부 프로시저/함수나 BLOCK 실행문의 선언부에서는 사용할 수 없다.

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
BLOCK은 프로시저/함수와 마찬가지로 예외처리 구조를 가질 수 있다.
::

    <block> ::=
        [ DECLARE <seq_of_declare_specs> ] <body>

    <body> ::= BEGIN <seq_of_statements> [ EXCEPTION <seq_of_handlers> ] END [ <label_name> ]
    <seq_of_declare_specs> ::= <declare_spec> [ <declare_spec> ... ]
    <seq_of_statements> ::= <statement> ; [ <statement> ; ... ]
    <seq_of_handlers> ::= <handler> [ <handler> ... ]
    <handler> ::= WHEN <exception_name> [ OR <exeption_name> OR ... ] THEN <seq_of_statements>
    <exception_name> ::= OTHERS | identifier


* *body*: 필수적으로 하나 이상의 실행문과 선택적으로 몇 개의 Exception 핸들러로 구성된다.
* *declare_spec*: 변수, 상수, Exception, 커서, Autonomous Transaction, 내부 프로시저/함수 선언. (참조: :ref:`선언문 <decl>`)
* *handler*:  지정된 Exception이 발생했을 때 실행할 실행문들을 지정한다.
* *exception_name*: OTHERS인 경우 아직까지 매치되지 않은 모든 Exception에 매치된다. 아닌 경우는 시스템 Exception이거나 사용자 정의 Exception을 나타낸다.

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

    <open_statement> ::= OPEN <cursor_expression> [ <function_argument> ]
    <fetch_statement> ::= FETCH <cursor_expression> INTO <identifier> [ , <identifier>, ... ]
    <close_statement> ::= CLOSE <cursor_expression>

    <open_for_statement> ::= OPEN <identifier> FOR <select_statement>

* *cursor_expression*: 계산 결과로 커서나 SYS_REFCURSOR 변수를 갖는 표현식
* *open_statement*: 커서를 연다. 파라미터를 갖도록 선언된 커서에 대해서는 선언된 파라미터 갯수와 타입에 맞는 인자를 주면서 열어야 한다.
* *fetch_statement*: 커서로부터 하나의 row를 가져와 지정된 변수나 OUT 파라미터에 대입한다. row 안의 컬럼 갯수는 지정된 변수나 OUT 파라미터 갯수와 일치해야 하고 각각의 컬럼값은 해당 변수나 OUT 파라미터에 대입 가능한 타입을 가져야 한다.
* *close_statement*: 커서를 닫는다.
* *open_for_statement*: *identifier*\는 SYS_REFCURSOR 타입으로 선언된 변수이어야 한다. 지정된 *select_statement*\를 실행하는 커서를 내부적으로 열어서 지정된 변수에 할당한다.

다음은 OPEN, FETCH, CLOSE 문의 사용예이다.

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

다음 예제는 SYS_REFCURSOR를 OUT 파라미터로 갖는 내부 프로시저와 OPEN-FOR 문을 이용해서 특정 SELECT 문을
SYS_REFCURSOR 변수에 연결하고 그 SELECT 문의 결과를 조회해 오는 예제이다.

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

.. _exec_imme:

EXECUTE IMMEDIATE
=================

:ref:`Dynamic SQL <dyn_sql>` 절에서 설명한 바와 같이
실행 시간에 임의의 SQL을 문자열로 구성하여 EXECUTE IMMDIATE 문을 통해 실행할 수 있다.
USING 절을 써서 프로그램 상의 어떤 값을 SQL문의 호스트 변수 자리에 채우는 것이 가능하고,
INTO 절을 써서 SELECT 문의 조회 결과를 프로그램의 변수나 OUT 파라미터에 담아오는 것도 가능하다.

::

    <execute_immediate> ::=
        EXECUTE IMMEDIATE <dynamic_sql> { [ <into_clause> ] [ <using_clause> ] | <using_clause> <into_clause> }
        <using_clause> ::= USING <using_element> [ , <using_element>, ... ]
        <using_element> ::= [ { IN | IN OUT | INOUT | OUT } ] <expression>
        <into_clause> ::= INTO <identifier> [ , <identifier>, ... ]


* *dynamic_sql*: 문자열 타입을 갖는 표현식. 표현식은 SQL 규약에 맞는 SQL 구문 문자열을 계산 결과로 가져야 한다.
  SQL 구문 중간중간 값을 필요로 하는 자리에 ?(물음표)를 대신 쓸 수 있으며 이러한 ?의 갯수와 *using_clause*\에
  포함된 표현식의 갯수는 일치해야 한다.
* *using_clause*: *dynamic_sql*\을 실행할 때 문자열의 ? 자리에 채워질 값들을 지정한다.  IN, IN OUT, INOUT, OUT 네 가지 타입으로 지정할 수 있다.
* *into_clause*: *dynamic_sql*\이 SELECT문을 나타내는 경우에 조회 결과를 담을 변수나 OUT 파라미터를 지정한다.

다음은 EXECUTE IMMEDIATE의 사용예이다.

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

* *identifier*: 변수이거나 OUT 파라미터이어야 한다.
* *expression*: 대입될 값을 계산하는 표현식. 아래 표현식 절 참조

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

Exception을 일으킨다.
Exception 이름 *identifier*\가 생략되는 경우는 RAISE 문의 위치가 예외처리 구조의 THEN 절 안에 있을 때 뿐이다.
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
현재 루틴이 함수인 경우에는 그 함수의 리턴 타입에 맞는 반환값 *expression*\을 지정해야 한다.
현재 루틴이 함수가 아닌 프로시저인 경우에는 반환값을 지정하면 에러이다.

프로시저 호출문
===============
::

    <procedure_call> ::=
        <identifier> [ <function_argument> ]
    <function_argument> ::= ( [ <expression> [ , <expression>, ... ] ] )

이름 *identifier*\로 지정된 프로시저를 인자 *function_argument*\를 주어 호출한다.
인자 갯수와 각각의 타입은 해당 프로시저의 선언과 일치해야 한다.
호출되는 프로시저의 OUT 파라미터에 주어질 인자들은 프로시저 호출 결과로 변경이 될 것이므로
대입이 가능한 변수나 다른 OUT 파라미터이어야 한다.

IF
==
::

    <if_statement> ::=
        IF <expression> THEN <seq_of_statements> [ <elsif_part> [ <elsif_part> ... ] ] [ <else_part> ] END IF
    <elsif_part> ::= ELSIF <expression> THEN <seq_of_statements>
    <else_part> ::= ELSE <seq_of_statements>

일반적인 프로그래밍 언어가 제공하는 If-Then-Else 문을 제공한다.

.. _loop:

LOOP
====
PL/CSQL이 제공하는 루프문은 아래와 같이 여섯 가지 형태가 있다.
앞의 세 가지는 일반적인 프로그래밍 언어에서 제공하는 루프문과 유사하다.
뒤의 세 가지는 SELECT 문의 조회 결과를 순회하는 용도로 사용한다.
::

    <loop_statement> ::=
          <label_declaration>? LOOP <seq_of_statements> END LOOP                          # basic-loop
        | <label_declaration>? WHILE <expression> LOOP <seq_of_statements> END LOOP       # while-loop
        | <label_declaration>? FOR <iterator> LOOP <seq_of_statements> END LOOP           # for-iter-loop
        | <label_declaration>? FOR <for_cursor> LOOP <seq_of_statements> END LOOP         # for-cursor-loop
        | <label_declaration>? FOR <for_static_sql> LOOP <seq_of_statements> END LOOP     # for-static-sql-loop
        | <label_declaration>? FOR <for_dynamic_sql> LOOP <seq_of_statements> END LOOP    # for-dynamic-sql-loop

    <label_declaration> ::= '<<' <identifier> '>>'

    <iterator> ::= <identifier> IN [ REVERSE ] <lower_bound> .. <upper_bound> [ BY <step> ]

    <for_cursor>      ::= <record> IN <cursor_expression> [ <function_argument> ]
    <for_static_sql>  ::= <record> IN ( <select_statement> )
    <for_dynamic_sql> ::= <record> IN ( EXECUTE IMMEDIATE <dynamic_sql> [ <using_clause> ] )

* *label_declaration*: 오직 루프문 시작 부분에서만 라벨 선언을 할 수 있다. 이 라벨은 루프 바디 안 쪽의 CONTINUE 문이나 EXIT 문이 분기 기준이 될 루프를 지정하는데 사용된다.
* *for-iter-loop* 형태의 루프에서 *lower_bound*, *upper_bound*, *step*\은 모두 INTEGER 타입을 갖는다. step은 1보다 크거나 같아야 한다. REVERSE가 지정되지 않은 경우, *identifier*\는 *lower_bound*\로 초기화 된 후 *upper_bound*\보다 작거나 같다는 조건을 만족하면 루프 바디를 한번 실행하고 그 이후는 *step* 만큼 증가한 값이 *upper_bound*\보다 작거나 같다는 조건을 만족하는 한 반복한다.  REVERSE가 지정된 경우에는, *identifier*\는 *upper_bound*\로 초기화 된 후 *lower_bound*\보다 크거나 같다는 조건을 만족하면 루프 바디를 한번 실행하고 그 이후는 *step*\만큼 감소한 값이 *lower_bound*\보다 크거나 같다는 조건을 만족하는 한 반복한다. 루프 변수 *identifier*\는 루프 바디 안에서 INTEGER 타입 변수로 사용될 수 있다.
* *for-cursor-loop*, *for-static-sql-loop*, *for-dynamic-sql-loop* 형태의 FOR 루프는 *record* IN 다음에 기술하는 SELECT 문의 조회 결과들을 순회하기 위해 사용된다. 매 iteration 마다 조회 결과가 한 row 씩 *record*\에 할당된 상태로 루프 바디가 실행된다. 이 때, 결과 row의 각 컬럼들은 루프 바디 안에서 *record*. *column* 모양으로 참조할 수 있다.
* *for-dynamic-sql-loop* 문 안에서의 *using_clause*\는 EXECUTE IMMEDIATE 문에서와는 달리 OUT 키워드를 지정할 수 없다.

다음은 For-Iterator Loop 구문의 사용예를 보여준다.

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

다음은 동일한 SELECT 문을 세 가지 다른 형태의 For Loop으로 조회하는 예를 보여준다.

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

        -- For-Dynamic-SQL Loop
        FOR r IN (EXECUTE IMMEDIATE 'SELECT host_year, score FROM history WHERE athlete = ?' USING p_name) LOOP
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
* 두 형태 모두 선택적으로 ELSE 절을 가질 수 있다. 이는 조건을 만족하는 WHEN 이후 표현식을 찾지 못했을 경우에 실행할 실행문들을 지정한다. 조건을 만족하는 WHEN 절이 없고 ELSE 절도 없을 때는 CASE_NOT_FOUND라는 시스템 예외가 발생한다.

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
        | { [ <literal> [, <literal> ... ] ] }
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
리터럴에는 날짜/시간(DATE, TIME, TIMESTAMP, DATETIME), 숫자, 문자열, 컬렉션, NULL, TRUE, FALSE 값이 있다.
비트열을 사용할 수 없다는 점을 제외하고 `SQL 리터럴 <https://www.cubrid.org/manual/ko/11.2/sql/literal.html#>`_\과 동일하다.

식별자
=================
Static/Dynamic SQL 밖의 PL/CSQL 문에서 사용할 수 있는 식별자에는 다음 세 가지 종류가 있다.

* 선언부에서 선언된 변수, 상수, 커서, Exception, 내부 프로시저/함수
* 프로시저/함수의 파라미터
* 암묵적으로 선언된 :ref:`For 루프<loop>`\의 iterator - integer와 record

명시적 혹은 암묵적 선언 없이 식별자를 사용하면 컴파일 에러가 발생한다.

Static SQL 결과 크기
====================
SQL%ROWCOUNT는 Static SQL을 실행한 직후에 결과 크기를 나타내는 표현식이다.

* 커서와 연관되지 않은 SELECT 문의 경우 반드시 INTO 절을 포함하게 되고 조회 결과는 1개이어야 한다. 따라서, 이 SELECT 문이 정상적으로 수행되었을 때 SQL%ROWCOUNT의 값은 1이다. 조회 결과 크기가 0이거나 1을 초과해서 실행시간 에러가 발생했을 때에는 SQL%ROWCOUNT의 값은 정의되지 않는다.
* INSERT, UPDATE, DELETE, MERGE, REPLACE, TRUNCATE 문의 경우 영향 받은 레코드 갯수가 된다.
* COMMIT, ROLLBACK 문에 대해서는 0이 된다.

커서 속성
=================

커서나 SYS_REFCURSOR 변수를 계산 결과로 갖는 표현식 *cursor_expression*\에
%ISOPEN, %FOUND, %NOTFOUND, %ROWCOUNT 기호를 덧붙여 그 커서의 네 가지 속성을 조회할 수 있다.

* %ISOPEN: 커서가 열려 있는지 여부 (BOOLEAN)
* %FOUND: 첫 번째 FETCH 이전이면 NULL. 아니면 마지막 FETCH가 1개의 ROW를 결과로 갖는지 여부 (BOOLEAN)
* %NOTFOUND: 첫 번째 FETCH 이전이면 NULL. 아니면 마지막 FETCH가 0개의 ROW를 결과로 갖는지 여부 (BOOLEAN)
* %ROWCOUNT: 첫 번째 FETCH 이전이면 NULL. 아니면 현재까지 FETCH된 ROW의 갯수 (BIGINT)

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
| +, -, ||                                                           | 더하기, 빼기, 문자열 병합           |
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
* &&, !은 Static/Dynamic SQL 밖에서는 AND, NOT과 동일한 의미의 논리 연산자로 사용할 수 없다.
* ||는 서버 설정 파라미터 pipes_as_concat 값과 상관없이 Static/Dynamic SQL 밖에서는 논리합(OR) 연산자로 사용할 수 없다.
* +는 서버 설정 파라미터 plus_as_concat 값과 상관없이 Static/Dynamic SQL 밖에서는 문자열 병합 연산자로 사용할 수 없다.
* Static/Dynamic SQL 밖에서의 문자열은 DB 설정과 관계 없이 UTF8 encoding을 따르며
  이들 문자열들 사이의 비교는 해당 Unicode 배열들 사이의 사전식 비교법을 따른다.
  Static/Dynamic SQL 안에서의 문자열의 encoding과 비교는 DB와 테이블 설정을 따른다.

명시적으로 연산 순서를 지정하기 위해 괄호를 사용할 수 있다.

레코드 필드 참조
=================

PL/CSQL에서는 명시적인 레코드 타입과 레코드 변수 선언을 지원하지 않지만,
FOR 문에서 SELECT 결과를 순회하기 위해 암묵적으로 선언되는 레코드 변수를 사용할 수 있다.
즉, FOR 문 iterator에 SELECT 결과 컬럼 이름을 덧붙여 해당 컬럼값을 레코드 필드 참조하듯이 사용할 수 있다.

.. code-block:: sql

    CREATE PROCEDURE athlete_history(p_name VARCHAR)
    AS
        CURSOR my_cursor IS
        SELECT host_year, score
        FROM history
        WHERE athlete = p_name;
    BEGIN
        FOR r IN my_cursor LOOP     -- r: 암묵적으로 선언됨
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);    -- r.<column-name>
        END LOOP;
    END;

함수 호출
=================

함수는 프로시저와 달리 반환값이 있으므로 표현식으로 쓸 수 있다.
인자 갯수와 각각의 타입은 해당 함수의 선언과 일치해야 한다.
호출되는 함수의 OUT 파라미터에 주어질 인자들은 호출 결과 변경이 일어나게 되므로
대입이 가능한 변수나 다른 OUT 파라미터이어야 한다.

CASE 표현식
=================

CASE 표현식은 여러 개의 조건을 순차적으로 검사해서 가장 처음 만족하는 조건에 연관되어 있는 값을 갖는다.

CASE 표현식은 :ref:`CASE 실행문 <case_stmt>`\(Statement)과 마찬가지로 CASE 키워드 직후에 표현식을 갖는 형태와 갖지 않는 형태가 있다.

* CASE 키워드 직후에 표현식을 갖는 형태에서는 우선 이 최초 표현식을 계산한 다음, WHEN 절들의 표현식을 하나씩 차례로 계산해서 최초 표현식과 일치하는 값을 찾고, 해당 THEN 절의 표현식을 계산해서 CASE문의 최종값으로 한다. 최초 표현식은 단 한번 계산된다.
* CASE 키워드 직후에 표현식을 갖지 않는 형태에서는 CASE 키워드 이후 여러 개의 WHEN 절의 표현식은 BOOLEAN 타입을 가져야 한다. 이들 표현식을 하나씩 차례로 계산하다가 처음으로 TRUE 값이 되는 표현식이 발견되면 해당 THEN 절의 표현식을 계산해서 CASE문의 최종값으로 한다.
* 두 형태 모두 선택적으로 ELSE 절을 가질 수 있다. 이는 조건을 만족하는 WHEN 이후 표현식을 찾지 못했을 경우에 값으로 가질 표현식을  지정한다. 조건을 만족하는 WHEN 절이 없고 ELSE 절도 없을 때 전체 CASE 표현식은 NULL 값을 갖는다.

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

SQLCODE, SQLERRM
=================

예외 처리 블럭 안에서 SQLCODE와 SQLERRM은 각각 현재 처리 중인 예외의 코드(INTEGER 타입)와 메시지(STRING 타입)를 나타낸다. 예외 처리 블럭 밖에서 SQLCODE와 SQLERRM은 각각 0과 'no error' 값을 갖는다.

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

