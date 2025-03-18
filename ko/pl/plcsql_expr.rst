------------------
표현식
------------------

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
        s := CASE i MOD 2
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
            WHEN i MOD 2 = 0 THEN 'Even'
            WHEN i MOD 2 = 1 THEN 'Odd'
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
