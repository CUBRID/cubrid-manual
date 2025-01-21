------------------
선언문
------------------

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

     CREATE OR REPLACE PROCEDURE poo(a INT) AS
    
         PROCEDURE inner AS
             i INT := a;
             a NUMERIC;
         BEGIN
             ...
         END;
    
     BEGIN
         ...
     END;

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
    <parameter> ::= <identifier> [ { IN | IN OUT | INOUT | OUT } ] <type_spec> [ COMMENT 'param_comment_string' ]
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
* *param_comment_string*: 인자 커멘트 문자열을 지정한다.
* *builtin_type*: :ref:`데이터 타입 <types>` 절에서 설명한 시스템 제공 타입
* *body*: 필수적으로 하나 이상의 실행문과 선택적으로 몇 개의 Exception 핸들러로 구성된다.
* *label_name*: 프로시저/함수 이름과 일치해야 한다.
* *declare_spec*: 변수, 상수, Exception, 커서, 내부 프로시저/함수 선언 중 하나
* *statement*: 아래 :doc:`실행문 <plcsql_stmt>` 절 참조
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
