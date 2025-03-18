------------------
실행문
------------------

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
* *declare_spec*: 변수, 상수, Exception, 커서, 내부 프로시저/함수 선언. (참조: :doc:`선언문 <plcsql_decl>`)
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

body 내부의 실행문들 중 수행시 도달할 수 없는 실햄문이 있는 경우 컴파일 과정에서 에러를 발생한다.
다음은 도달할 수 없는 실행문이 있는 간단한 예이다.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_unreachable_statement
    AS
    BEGIN
        RETURN;
        DBMS_OUTPUT.put_line('Hello world');
    END;

    ERROR: In line 5, column 5
    Stored procedure compile error: unreachable statement

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
        CASE i MOD 2
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
            WHEN i MOD 2 = 0 THEN
                DBMS_OUTPUT.put_line('Even');
            WHEN i MOD 2 = 1 THEN
                DBMS_OUTPUT.put_line('Odd');
            ELSE
                DBMS_OUTPUT.put_line('Null');
        END CASE;
    END;
