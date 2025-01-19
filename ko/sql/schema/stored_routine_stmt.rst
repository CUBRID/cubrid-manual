
:meta-keywords: procedure definition, create procedure, drop procedure, function definition, create function, drop function
:meta-description: Define functions/procedures in CUBRID database using create procedure, create function, drop procedure and drop function statements.


*************************
저장 함수/프로시저 정의문
*************************

.. _create-procedure:

CREATE PROCEDURE
=================

**CREATE PROCEDURE** 문을 사용하여 저장 프로시저를 등록한다.

::

    CREATE [OR REPLACE] PROCEDURE [schema_name.]procedure_name ([<parameter_definition> [, <parameter_definition>] ...])
    {IS | AS} LANGUAGE <lang> 
      <body>
    COMMENT 'procedure_comment';
    
        <parameter_definition> ::= parameter_name [mode] sql_type [ { DEFAULT | = } <default_expr> ] [COMMENT 'parameter_comment_string']
        <lang> ::= [PLCSQL | JAVA]
        <mode> ::= IN | OUT | IN OUT | INOUT

* **OR REPLACE** 구문을 사용하여 현재의 저장 함수/프로시저를 대체 혹은 새로 생성하는 문장을 작성할 수 있다.

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *procedure_name*: 생성할 저장 프로시저의 이름을 지정한다. (최대 222바이트)
*   *parameter_name*: 인자의 이름을 지정한다. (최대 254바이트)
*   *sql_type*: 인자의 데이터 타입을 지정한다.
*   *parameter_comment_string*: 인자 커멘트 문자열을 지정한다.
*   *body*: 저장 프로시저의 본문을 지정한다.
*   *procedure_comment*: 저장 프로시저의 커멘트 문자열을 지정한다.

저장 프로시저의 커멘트
----------------------------------

저장 프로시저의 커멘트를 다음과 같이 제일 뒤에 지정할 수 있다. 

.. code-block:: sql


    CREATE FUNCTION Hello() RETURN VARCHAR
    AS LANGUAGE JAVA
    NAME 'SpCubrid.HelloCubrid() return java.lang.String'
    COMMENT 'function comment';

저장 프로시저의 인자 뒤에는 다음과 같이 지정할 수 있다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test(i in number COMMENT 'arg i') 
    RETURN NUMBER AS LANGUAGE JAVA NAME 'SpTest.testInt(int) return int' COMMENT 'function test';

저장 프로시저의 커멘트는 다음 구문을 실행하여 확인할 수 있다.

.. code-block:: sql

    SELECT sp_name, comment FROM db_stored_procedure; 

저장 프로시저 인자의 커멘트는 다음 구문을 실행하여 확인할 수 있다.

.. code-block:: sql
          
    SELECT sp_name, arg_name, comment FROM db_stored_procedure_args;


등록된 저장 프로시저의 정보 확인
------------------------------------------

등록된 저장 프로시저의 정보는 **db_stored_procedure** 시스템 가상 클래스와 **db_stored_procedure_args** 시스템 가상 클래스에서 확인할 수 있다. 
**db_stored_procedure** 시스템 가상 클래스에서는 저장 프로시저의 이름과 타입, 인자의 수, Java 클래스에 대한 명세, 저장 프로시저의 소유자에 대한 정보를 확인할 수 있다.
**db_stored_procedure_args** 시스템 가상 클래스에서는 저장 프로시저에서 사용하는 인자에 대한 정보를 확인할 수 있다.

.. code-block:: sql

    SELECT * FROM db_stored_procedure WHERE sp_type = 'PROCEDURE';
    
::
    
    sp_name               sp_type               return_type             arg_count  lang target                owner
    ================================================================================
    'athlete_add'         'PROCEDURE'           'void'                          4  'JAVA''Athlete.Athlete(java.lang.String, java.lang.String, java.lang.String, java.lang.String)'  'DBA'

.. code-block:: sql
    
    SELECT * FROM db_stored_procedure_args WHERE sp_name = 'athlete_add';
    
::
    
    sp_name   index_of  arg_name  data_type      mode
    =================================================
     'athlete_add'                   0  'name'                'STRING'              'IN'
     'athlete_add'                   1  'gender'              'STRING'              'IN'
     'athlete_add'                   2  'nation_code'         'STRING'              'IN'
     'athlete_add'                   3  'event'               'STRING'              'IN'

.. _create-function:

CREATE FUNCTION
=================

**CREATE FUNCTION** 문을 사용하여 저장 함수를 등록한다.

::

    CREATE [OR REPLACE] FUNCTION [schema_name.]function_name ([<parameter_definition> [, <parameter_definition>] ...])
    RETURN sql_type
    {IS | AS} LANGUAGE <lang> 
      <body>
    COMMENT 'function_comment';
    
        <parameter_definition> ::= parameter_name [mode] sql_type [ { DEFAULT | = } <default_expr> ] [COMMENT 'param_comment_string']
        <lang> ::= [PLCSQL | JAVA]
        <mode> ::= IN | OUT | IN OUT | INOUT

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *function_name*: 생성할 저장 함수의 이름을 지정한다(최대 222바이트).
*   *parameter_name*: 인자의 이름을 지정한다(최대 254바이트).
*   *sql_type*: 인자 또는 리턴 값의 데이터 타입을 지정한다. 지정할 수 있는 데이터 타입은 :ref:`jsp-type-mapping`\을 참고한다.
*   *param_comment_string*: 인자 커멘트 문자열을 지정한다.
*   *body*: 저장 함수의 본문을 지정한다.
*   *function_comment*: 저장 함수의 커멘트 문자열을 지정한다.

저장 함수의 커멘트
----------------------------------

저장 함수의 커멘트를 다음과 같이 제일 뒤에 지정할 수 있다. 

.. code-block:: sql

    CREATE FUNCTION Hello() RETURN VARCHAR
    AS LANGUAGE JAVA
    NAME 'SpCubrid.HelloCubrid() return java.lang.String'
    COMMENT 'function comment';

저장 함수의 인자 뒤에는 다음과 같이 지정할 수 있다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test(i in number COMMENT 'arg i') 
    RETURN NUMBER AS LANGUAGE JAVA NAME 'SpTest.testInt(int) return int' COMMENT 'function test';

저장 함수의 커멘트는 다음 구문을 실행하여 확인할 수 있다.

.. code-block:: sql

    SELECT sp_name, comment FROM db_stored_procedure; 

함수 인자의 커멘트는 다음 구문을 실행하여 확인할 수 있다.

.. code-block:: sql
          
    SELECT sp_name, arg_name, comment FROM db_stored_procedure_args;

등록된 저장 함수의 정보 확인
------------------------------------------

등록된 저장 함수의 정보는 **db_stored_procedure** 시스템 가상 클래스와 **db_stored_procedure_args** 시스템 가상 클래스에서 확인할 수 있다. 
**db_stored_procedure** 시스템 가상 클래스에서는 저장 함수의 이름과 타입, 반환 타입, 인자의 수, Java 클래스에 대한 명세, 저장 함수의 소유자에 대한 정보를 확인할 수 있다. 
**db_stored_procedure_args** 시스템 가상 클래스에서는 저장 함수에서 사용하는 인자에 대한 정보를 확인할 수 있다.

.. code-block:: sql

    SELECT * FROM db_stored_procedure WHERE sp_type = 'FUNCTION';
    
::
    
    sp_name               sp_type               return_type             arg_count  lang target                owner
    ================================================================================
    'hello'               'FUNCTION'            'STRING'                        0  'JAVA''SpCubrid.HelloCubrid() return java.lang.String'  'DBA'
     
    'sp_int'              'FUNCTION'            'INTEGER'                       1  'JAVA''SpCubrid.SpInt(int) return int'  'DBA'

.. code-block:: sql
    
    SELECT * FROM db_stored_procedure_args WHERE sp_name = 'sp_int';
    
::
    
    sp_name   index_of  arg_name  data_type      mode
    =================================================
     'sp_int'                        0  'i'                   'INTEGER'             'IN'

DROP PROCEDURE
==============

CUBRID에서는 등록한 저장 프로시저를 **DROP PROCEDURE** 구문을 사용하여 삭제할 수 있다.
이 때, 여러 개의 *procedure_name* 을 콤마(,)로 구분하여 한꺼번에 여러 개의 저장 프로시저를 삭제할 수 있다.

::

    DROP PROCEDURE [schema_name.]procedure_name [{, [schema_name.]procedure_name} ... ];

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *procedure_name*: 제거할 프로시저의 이름을 지정한다.

.. code-block:: sql

    DROP PROCEDURE hello, sp_int;

저장 프로시저의 삭제는 프로시저를 등록한 사용자와 DBA의 구성원만 삭제할 수 있다.
예를 들어 'sp_int' 저장 프로시저를 **PUBLIC** 이 등록했다면, **PUBLIC** 또는 **DBA** 의 구성원만이 'sp_int' 저장 프로시저를 삭제할 수 있다.

DROP FUNCTION
==============

CUBRID에서는 등록한 저장 함수를 **DROP FUNCTION** 구문을 사용하여 삭제할 수 있다.
이 때, 여러 개의 *function_name* 을 콤마(,)로 구분하여 한꺼번에 여러 개의 저장 함수를 삭제할 수 있다.

::

    DROP FUNCTION [schema_name.]function_name [{, [schema_name.]function_name} ... ];

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *function_name*: 제거할 함수의 이름을 지정한다.

.. code-block:: sql

    DROP FUNCTION hello, sp_int;

저장 함수의 삭제는 함수를 등록한 사용자와 DBA의 구성원만 삭제할 수 있다.
예를 들어 'sp_int' 저장 함수를 **PUBLIC** 이 등록했다면, **PUBLIC** 또는 **DBA** 의 구성원만이 'sp_int' 저장 함수를 삭제할 수 있다.
