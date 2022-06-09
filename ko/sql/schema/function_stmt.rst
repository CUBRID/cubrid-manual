
:meta-keywords: function definition, create function, drop function
:meta-description: Define functions in CUBRID database using create function, drop function statements.


****************
저장 함수 정의문
****************

.. _create-function:

CREATE FUNCTION
=================

**CREATE FUNCTION** 문을 사용하여 저장 함수를 등록한다.
CUBRID는 Java를 제외한 다른 언어에서는 저장 함수를 지원하지 않는다. CUBRID에서 저장 함수는 오직 Java로만 구현 가능하다.
등록한 저장 함수의 사용 방법은 :doc:`/sql/jsp`\를 참고한다.

::

    CREATE [OR REPLACE] FUNCTION function_name [(<parameter_definition> [, <parameter_definition>] ...)] RETURN sql_type
    {IS | AS} LANGUAGE JAVA <java_call_specification>
    COMMENT 'sp_comment_string';
    
        <parameter_definition> ::= parameter_name [IN|OUT|IN OUT|INOUT] sql_type [COMMENT 'param_comment_string']
        <java_call_specification> ::= NAME 'java_method_name (java_type [,java_type]...) [return java_type]'

*   *function_name*: 생성할 저장 함수의 이름을 지정한다(최대 254바이트).
*   *parameter_name*: 인자의 이름을 지정한다(최대 254바이트).
*   *sql_type*: 인자 또는 리턴 값의 데이터 타입을 지정한다. 지정할 수 있는 데이터 타입은 :ref:`jsp-type-mapping`\을 참고한다.
*   *param_comment_string*: 인자 커멘트 문자열을 지정한다.
*   *sp_comment_string*: 저장 프로시저의 커멘트 문자열을 지정한다.
*   *java_method_name*: 자바의 클래스 이름을 포함하여 자바의 메소드 이름을 지정한다.
*   *java_type*: 자바의 데이터 타입을 지정한다. 지정할 수 있는 데이터 타입은 :ref:`jsp-type-mapping`\을 참고한다.

Java 클래스를 로딩했을 때 SQL 문이나 Java 응용 프로그램에서 클래스 내의 함수를 어떻게 호출할지 모르기 때문에 Call Specification를 사용하여 등록해야 한다.
Call Specification 작성 방법에 대해서는 :ref:`jsp-call-specification`\을 참조한다.

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
    
    sp_name     sp_type   return_type    arg_count
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


DROP FUNCTION
==============

CUBRID에서는 등록한 저장 함수를 **DROP FUNCTION** 구문을 사용하여 삭제할 수 있다.
이 때, 여러 개의 *function_name* 을 콤마(,)로 구분하여 한꺼번에 여러 개의 저장 함수를 삭제할 수 있다.

::

    DROP FUNCTION function_name [{ , function_name , ... }];

*   *function_name*: 제거할 함수의 이름을 지정한다.

.. code-block:: sql

    DROP FUNCTION hello, sp_int;

저장 함수의 삭제는 함수를 등록한 사용자와 DBA의 구성원만 삭제할 수 있다.
예를 들어 'sp_int' 저장 함수를 **PUBLIC** 이 등록했다면, **PUBLIC** 또는 **DBA** 의 구성원만이 'sp_int' 저장 함수를 삭제할 수 있다.