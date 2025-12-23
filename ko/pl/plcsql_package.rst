
:meta-keywords: cubrid sql, pl/csql, pl/csql package
:meta-description: This chapter describes PL/CSQL Package Spec.

.. _plcsql_package:

*****************************
PL/CSQL 패키지
*****************************

PL/CSQL 패키지(Package)는 :ref:`PL/CSQL <stored_proc>`\의 변수, 상수, Exception, 커서, 프로시저, 함수, 레코드 타입들을
하나의 이름 아래에 모아서 선언, 정의할 수 있도록 하는 기능이다.
패키지에서 선언한 항목들은 패키지 이름과 '.' (dot) 뒤에 항목 이름을 써서 참조한다.

패키지를 사용하여 얻을 수 있는 효과는 아래와 같다.

* 관련 있는 선언과 정의를 하나의 패키지로 모듈화함으로써 유지 보수에 도움을 준다.
* 모듈의 명세(선언)와 구현(정의)을 분리하고 모듈 사용은 명세를 통하도록 함으로써 세부 구현에 대한 의존성을 줄인다.
* 패키지에 선언된 변수, 상수, 커서는 한 세션 동안 유지되는 상태를 제공한다.

패키지가 하나 이상의 변수, 상수, 커서를 갖는 경우에 그 패키지는 상태를 갖는 패키지라고 부른다.
상태를 갖는 패키지에 대해서는 각각의 로그인 세션에서 그 패키지를 최초로 참조할 때 상태 인스턴스를 한번 생성하고 초기화한다.
세션이 종료될 때에는, 열려 있는 패키지 선언 커서가 있으면 닫히고 상태 인스턴스에 사용되었던 시스템 자원은 회수된다.
패키지의 재생성(CREATE OR REPLACE PACKAGE)이나 재컴파일(ALTER PACKAGE)에 의해서 기존의 패키지 상태 인스턴스는 회수되고
이후에 패키지가 다시 참조될 때 인스턴스가 다시 초기화 된다.
서로 다른 세션 간에 패키지 상태가 공유되는 일은 없다.

패키지 정의문
=============

CREATE PACKAGE 문을 사용해서 이름과 타입 정보로 구성된 패키지 명세를 선언하고,
CREATE PACKAGE BODY 문을 사용해서 명세에 선언된 커서, 프로시저, 함수를 구현한다.
DROP PACKAGE 문을 사용해서 패키지를 삭제하고, ALTER PACKAGE 문을 사용해서 패키지를 다시 컴파일 한다.

CREATE PACKAGE
---------------

CREATE PACKAGE 문의 문법은 아래와 같다.

::

    CREATE [OR REPLACE] PACKAGE [schema_name.]<package_name> {IS|AS} <package_item> { <package_item> }... END [ <package_name> ]

        <package_item> ::=
            <variable_decl>
          | <constant_decl>
          | <exception_decl>
          | <cursor_decl_or_def>
          | <procedure_decl>
          | <function_decl>
          | <record_type_def>

            <variable_decl> ::=
                <variable_name> <type_spec> [ [ NOT NULL ] <initial_value_part> ] ;

                <type_spec> ::=
                      <builtin_type>
                    | <table>.<column>%TYPE
                    | <variable>%TYPE
                    | <table>%ROWTYPE
                    | <cursor>%ROWTYPE
                    | <record_type>

                <initial_value_part> ::= { := | DEFAULT } <expression>

            <constant_decl> ::=
                <constant_name> CONSTANT <type_spec> [ NOT_NULL ] <initial_value_part> ;

            <exception_decl> ::=
                <exception_name> EXCEPTION ;

            <cursor_decl_or_def> ::=
                CURSOR <cursor_name> [ ( <seq_of_cursor_parameters> ) ] RETURN <rowtype> ;
              | CURSOR <cursor_name> [ ( <seq_of_cursor_parameters> ) ] [ RETURN <rowtype> ] IS <select_statement> ;

                <seq_of_cursor_parameters> ::= <cursor_parameter> { , <cursor_parameter> }...
                <cursor_parameter> ::= <parameter_name> [ IN ] <type_spec>

            <procedure_decl> ::=
                PROCEDURE <procedure_name> [ ( <seq_of_parameters> ) ] ;

                <seq_of_parameters> ::= [ <parameter> { , <parameter> }... ]
                <parameter> ::= <parameter_name> [ { IN | IN OUT | INOUT | OUT } ] <type_spec> [ COMMENT 'param_comment_string' ]

            <function_decl> ::=
                FUNCTION <function_name> [ ( <seq_of_parameters> ) ] RETURN <type_spec> [ [ NOT] DETERMINISTIC ] ;

            <record_type_def> ::=
                TYPE <record_type_name> IS RECORD ( <field_decl> { , <field_decl> }... ) ;

                <field_decl> ::= <field_name> <type_spec> [ [ NOT NULL ] <initial_value_part> ]

* *..._name*: :ref:`PL/CSQL 작성 규칙 <plcsql_identifier>`\에 설명된 식별자
* *builtin_type*: :ref:`PL/CSQL 데이터 타입 <types>` 절에서 설명한 시스템 제공 타입
* *...%TYPE*: :ref:`PL/CSQL %TYPE <percent_type>` 참고
* *...%ROWTYPE*: :ref:`PL/CSQL %ROWTYPE <percent_rowtype>` 참고
* *expression*: :ref:`PL/CSQL 표현식 <plcsql_expression>` 참고

CREATE PACKAGE 문은 패키지 명세를 선언한다. 패키지 명세는 사용자가 참조할 수 있는 항목들로 이루어진다.
항목들의 이름과 타입 정보들을 제공하는 것이 주된 기능이지만 변수의 초기값이나 커서의 실행문처럼 구현에 관련된 정보도 일부 포함할 수 있다.

OR REPLACE\가 명시되어 있고 지정된 이름으로 패키지가 이미 있으면 기존의 패키지를 대체한다.
OR REPLACE\가 명시되어 있지 않고 지정된 이름으로 패키지가 이미 있으면 컴파일 에러가 발생한다.

패키지에 포함할 수 있는 항목은 변수, 상수, Exception, 커서, 프로시저, 함수, 레코드 타입 7가지이다.

변수 선언에 초기값 지정은 선택이고 상수 선언에 초기값 지정은 필수이다.
이 초기값은 세션별로 패키지 상태 인스턴스를 초기화할 때 대입된다.
초기값이 지정되지 않은 변수는 NULL로 초기화된다.
변수와 상수 모두 NOT NULL 수식어를 지정할 수 있다. NOT NULL이 지정된 변수나 상수에 NULL 값을 대입하면 실행시간 에러가 발생한다.

커서 선언에 실행할 SELECT 문을 지정했을 때는 RETURN 절을 포함할 수도 있고 하지 않을 수도 있다.
SELECT 문을 지정하지 않았을 때는 RETURN 절을 포함하여 커서가 반환하는 레코드의 타입을 명시해야 하고
이후에 CREATE PACKAGE BODY 문에서 SELECT 문을 지정해야 한다.
커서에 IN 인자를 지정할 수 있다. 그러나, OUT 또는 INOUT 인자를 지정할 수는 없다.

프로시저나 함수 선언은 이름과 타입 정보(인자 타입, 리턴 타입)를 제공하지만 구현(body)을 제공할 수는 없다.
구현은 아래에서 설명할 CREATE PACKAGE BODY 문을 이용해서 제공한다.
함수의 경우에는 DETERMINISTIC 혹은 NOT DETERMINISTIC 수식어를 사용하여 :ref:`결정적 함수 <pl-deterministic>`\인지 여부를 지정할 수 있다.
둘 중 아무 것도 지정하지 않았을 때는 NOT DETERMINISTIC을 지정한 것처럼 동작한다.

레코드 타입은 주로 커서의 리턴 타입을 지정하기 위해 필요하다.
즉, 커서가 리턴하는 레코드가 어떤 이름과 타입의 컬럼들로 이루어져 있는지 나타내기 위해 주로 사용된다.
레코드 타입은 :ref:`%ROWTYPE <percent_rowtype>`\으로 사용할 수 있지만 위 문법의 *record_type_def*\대로 사용자가 정의해서 사용할 수도 있다.
레코드 컬럼을 나타내는 필드는 각각 NOT NULL 수식어와 디폴트값을 가질 수 있다.

커서의 리턴 타입 뿐만 아니라 PL/CSQL 문에서 타입 지정이 필요한 모든 위치에 레코드 타입을 쓸 수 있다.

CREATE PACKAGE BODY
--------------------

CREATE PACKAGE BODY 문의 문법은 아래와 같다.

::

    CREATE [OR REPLACE] PACKAGE BODY [schema_name.]<package_name> {IS|AS} <pb_declare_section> [ <pb_initialize_section> ] END [ <package_name> ]

        <pb_declare_section> ::= <package_body_item> { <package_body_item> }...

            <package_body_item> ::=
                <variable_decl>
              | <constant_decl>
              | <exception_decl>
              | <cursor_def>
              | <procedure_def>
              | <function_def>
              | <record_type_def>

                <variable_decl> ::=
                    <variable_name> <type_spec> [ [ NOT NULL ] <initial_value_part> ] ;

                    <type_spec> ::=
                          <builtin_type>
                        | <table>.<column>%TYPE
                        | <variable>%TYPE
                        | <table>%ROWTYPE
                        | <cursor>%ROWTYPE
                        | <record_type>

                    <initial_value_part> ::= { := | DEFAULT } <expression>

                <constant_decl> ::=
                    <constant_name> CONSTANT <type_spec> [ NOT_NULL ] <initial_value_part> ;

                <exception_decl> ::=
                    <exception_name> EXCEPTION ;

                <cursor_def> ::=
                    CURSOR <cursor_name> [ ( <seq_of_cursor_parameters> ) ] [ RETURN <rowtype> ] IS <select_statement> ;

                    <seq_of_cursor_parameters> ::= <cursor_parameter> { , <cursor_parameter> }...
                    <cursor_parameter> ::= <parameter_name> [ IN ] <type_spec>

                <procedure_def> ::=
                    PROCEDURE <procedure_name> [ ( <seq_of_parameters> ) ] { IS | AS } [ <seq_of_declare_specs> ] <body> ;

                    <seq_of_parameters> ::= [ <parameter> { , <parameter> }... ]
                    <parameter> ::= <parameter_name> [ { IN | IN OUT | INOUT | OUT } ] <type_spec> [ COMMENT 'param_comment_string' ]
                    <seq_of_declare_specs> ::= <declare_spec> { <declare_spec> }...

                <function_def> ::=
                    FUNCTION <function_name> [ ( <seq_of_parameters> ) ] RETURN <type_spec> [ [ NOT] DETERMINISTIC ] { IS | AS } [ <seq_of_declare_specs> ] <body>  ;

                <record_type_def> ::=
                    TYPE <record_type_name> IS RECORD ( <field_decl> { , <field_decl> }... ) ;

                    <field_decl> ::= <field_name> <type_spec> [ [ NOT NULL ] <initial_value_part> ]

        <pb_initialize_seaction> ::=
            BEGIN <statement> { <statement> }... [ EXCEPTION <exception_handler> { <exception_handler> }... ]

* *..._name*: :ref:`PL/CSQL 작성 규칙 <plcsql_identifier>`\에 설명된 식별자
* *builtin_type*: :ref:`PL/CSQL 데이터 타입 <types>` 절에서 설명한 시스템 제공 타입
* *...%TYPE*: :ref:`PL/CSQL %TYPE <percent_type>` 참고
* *...%ROWTYPE*: :ref:`PL/CSQL %ROWTYPE <percent_rowtype>` 참고
* *expression*: :ref:`PL/CSQL 표현식 <plcsql_expression>` 참고
* *body*: :ref:`PL/CSQL BLOCK <block_stmt>` 참고
* *statement*: :ref:`PL/CSQL 실행문 <plcsql_statement>` 참고
* *exception_handler*: :ref:`PL/CSQL BLOCK <block_stmt>` 참고

CREATE PACKAGE BODY 문의 기능은 아래와 같다.

* CREATE PACKAGE 문에서 선언한 프로시저와 함수에 대해서 구현(body)을 제공
* CREATE PACKAGE 문에서 커서 선언에 SELECT 문을 포함하지 않았을 때 이를 제공
* 패키지 상태 인스턴스 초기화 코드를 제공 (위 문법에서 *pb_initialize_section*)
* 패키지 구현 내부에서만 사용하는 (사용자가 참조할 수 없는) 패키지 private 항목들 정의

CREATE PACKAGE 문에서 선언한 커서, 프로시저, 함수에 대해서 구현을 제공할 때에는 선언부까지의 텍스트가 동일해야 한다.
그렇지 않으면 컴파일 에러이다.

CREATE PACKAGE 문에서 선언한 항목들에 대해서 구현을 제공하는 것이 아니라 같은 이름으로 다시 선언하는 경우에는 컴파일 에러이다.

DROP PACKAGE
-------------

DROP PACKAGE 문의 문법은 아래와 같다.

::

    DROP PACKAGE [ BODY ] [schema_name.]<package_name> ;

등록된 패키지 전체나 패키지 Body를 삭제한다.

ALTER PACKAGE
--------------

ALTER PACKAGE 문의 문법은 아래와 같다.

::

    ALTER PACKAGE [schema_name.]<package_name> COMPILE { PACKAGE | SPECIFICATION | BODY } ;

ALTER PACKAGE 문을 이용해서 패키지 전체 혹은 선언이나 구현만 재컴파일 할 수 있다.
패키지에서 참조하고 있는 테이블의 스키마가 바뀌는 등의 경우에 재컴파일이 필요하다.

SQL 문에서 패키지 참조 제약
============================

PL/CSQL 문에서는 패키지에서 선언한 모든 항목을 사용할 수 있는 반면,
일반 SQL 문에서는 패키지 선언 항목 중에 프로시저와 함수만 사용 가능하며
그 중에서도 인자 타입과 리턴 타입이 SQL에서 유효한 경우로만 사용이 제약된다.
이는 SQL의 실행 엔진과 PL/CSQL의 실행 엔진이 분리되어 있기 때문에 발생하는 제약이다.
예를 들어, 패키지 안에서 선언된 함수가 리턴 타입으로 레코드 타입을 갖는 경우에는 일반 SQL 문에서 그 함수를 호출할 수 없다.

시스템 패키지
==============

CUBRID 에서는 사용자 편의를 위해 시스템 패키지를 제공한다.
향후 버전에서 더 많은 패키지가 추가되어 CUBRID의 기능이 확장될 예정이며 현재 버전에서는 DBMS_OUTPUT 패키지만을 제공하고 있다.

.. _dbms_output:

DBMS_OUTPUT
------------

DBMS_OUTPUT 패키지는 문자열 메시지를 DBMS_OUTPUT 버퍼에 저장하고 읽어오기 위한 기능을 제공한다.
저장 프로시저/함수 개발자는 이 패키지의 PUT_LINE이나 PUT 함수로 원하는 메시지를 DBMS_OUTPUT 버퍼에 쌓을 수 있다.
CSQL이나 DBeaver 같은 클라이언트 도구들은 이 패키지의 ENABLE, DISABLE, GET_LINE, GET_LINES 함수들을 사용하여
메시지 저장 기능을 활성화/비활성화하고 버퍼에 쌓인 메시지들을 가져온다.
개발자는 프로그램 진행 상황 확인이나 디버깅을 위해 이 메시지들을 유용하게 사용할 수 있다.

이 섹션에서는 DBMS_OUTPUT 패키지의 사용법과 활용 예시를 설명한다.
DBMS_OUTPUT 패키지의 함수는 다음과 같다:

        * :ref:`dbms-output-enable`
        * :ref:`dbms-output-disable`
        * :ref:`dbms-output-put`
        * :ref:`dbms-output-put_line`
        * :ref:`dbms-output-new_line`
        * :ref:`dbms-output-get_line`
        * :ref:`dbms-output-get_lines`

.. _dbms-output-enable:

DBMS_OUTPUT.ENABLE
^^^^^^^^^^^^^^^^^^^

.. function:: DBMS_OUTPUT.ENABLE (size)

        DBMS_OUTPUT 메시지 버퍼를 활성화하고, 메시지를 저장할 버퍼의 크기를 설정한다.

        :param size: 버퍼의 크기를 지정하며, 이 값은 바이트 단위로 지정한다. 최대 크기는 32767 바이트이며 이 값을 초과하면 오류가 발생한다.

.. note::

        CSQL 인터프리터에서 **;server-output on** 을 호출하면 내부적으로 기본값인  DBMS_OUTPUT.ENABLE(20000)\을 호출한 것과 동일하다.
        자세한 내용은 :ref:`CSQL 세션명령어 server-output <server-output>`\을 참고한다.

.. _dbms-output-disable:

DBMS_OUTPUT.DISABLE
^^^^^^^^^^^^^^^^^^^^

.. function:: DBMS_OUTPUT.DISABLE ()

        현재 버퍼에 저장된 메시지를 제거하고 버퍼를 비활성화한다. 따라서 DBMS_OUTPUT 패키지 내의 다른 프로시저를 호출하더라도 아무런 출력이 나타나지 않는다.

.. note::

        CSQL 인터프리터에서 **;server-output off** 을 호출하면 내부적으로 DBMS_OUTPUT.DISABLE()\을 호출한 것과 같다.
        자세한 내용은 :ref:`CSQL 세션명령어 server-output <server-output>`\을 참고한다.

.. _dbms-output-put:

DBMS_OUTPUT.PUT
^^^^^^^^^^^^^^^^

.. function:: DBMS_OUTPUT.PUT (str VARCHAR)

        지정된 문자열을 줄바꿈 없이 버퍼에 저장한다.

        :param str: 저장할 문자열을 지정한다. 저장할 문자열이 NULL이면 아무런 동작도 하지 않는다.

.. _dbms-output-put_line:

DBMS_OUTPUT.PUT_LINE
^^^^^^^^^^^^^^^^^^^^^

.. function:: DBMS_OUTPUT.PUT_LINE (line VARCHAR)

        지정된 문자열을 버퍼에 저장하고 줄바꿈을 추가한다.

        :param line: 저장할 문자열을 지정한다. 저장할 문자열이 NULL이면 아무런 동작도 하지 않는다.

.. _dbms-output-new_line:

DBMS_OUTPUT.NEW_LINE
^^^^^^^^^^^^^^^^^^^^^

.. function:: DBMS_OUTPUT.NEW_LINE ()

        버퍼에 줄바꿈 문자를 추가한다. PUT 함수로 문자열을 추가한 후 NEW_LINE 함수를 호출하여 GET_LINE에서 줄 단위로 읽어올 수 있다.

.. _dbms-output-get_line:

DBMS_OUTPUT.GET_LINE
^^^^^^^^^^^^^^^^^^^^^

.. function:: DBMS_OUTPUT.GET_LINE (line OUT VARCHAR, status OUT INTEGER)

        버퍼에 저장된 문자열 메시지를 중 첫 번째 줄을 읽어온다. 읽어온 줄은 버퍼에서 삭제된다.

        :param line: 버퍼로 부터 읽어온 문자열을 저장한다.
        :param status: 문자열을 성공적으로 읽어왔을 경우 0을, 그렇지 않을 경우 1을 저장한다.

.. _dbms-output-get_lines:

DBMS_OUTPUT.GET_LINES
^^^^^^^^^^^^^^^^^^^^^^

.. function:: DBMS_OUTPUT.GET_LINES (lines OUT VARCHAR, num_lines IN OUT INTEGER)

        버퍼에 저장된 문자열 메시지를 지정된 줄 수만큼 읽어온다. 읽어온 줄은 버퍼에서 삭제된다.

        :param lines: 버퍼로 부터 읽어온 문자열을 저장한다.
        :param num_lines: 읽어올 줄의 수를 지정한다.


활용 예시
^^^^^^^^^^

다음은 CSQL 인터프리터로 DBMS_OUTPUT 패키지를 사용한 단순한 예시이다.
PUT_LINE 함수는 저장 함수 개발자가 사용하고 ENABLE, DISABLE, GET_LINE 함수들은 CSQL 인터프리터가
기능 구현을 위해 내부적으로 사용한다.

.. code-block:: sql

        ;server-output on   -- CSQL이 내부적으로 DBMS_OUTPUT.ENABLE 호출

        CREATE OR REPLACE FUNCTION test() RETURN VARCHAR
        AS
        BEGIN
                DBMS_OUTPUT.PUT_LINE('Hello, World!');
                DBMS_OUTPUT.PUT_LINE('Hello, CUBRID!');
                DBMS_OUTPUT.PUT_LINE('Hello, DBMS_OUTPUT!');
                RETURN 'Success';
        END;

        SELECT test();

        ;server-output off  -- CSQL이 내부적으로 DBMS_OUTPUT.DISABLE 호출

::

        test ()
        =======
        'Success'

        <DBMS_OUTPUT>       <-- CSQL이 출력할 메시지를 가져오기 위해 내부적으로 DBMS_OUTPUT.GET_LINE 여러 번 호출
        ====
        Hello, World!
        Hello, CUBRID!
        Hello, DBMS_OUTPUT!

