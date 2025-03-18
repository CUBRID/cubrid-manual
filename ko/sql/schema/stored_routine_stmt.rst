
:meta-keywords: procedure definition, create procedure, alter procedure, drop procedure, function definition, create function, alter function, drop function
:meta-description: Define functions/procedures in CUBRID database using create procedure, create function, alter procedure, alter function, drop procedure and drop function statements.


*************************
저장 프로시저/함수 정의문
*************************

.. _create-procedure:

CREATE PROCEDURE
=================

**CREATE PROCEDURE** 문을 사용하여 저장 프로시저를 등록한다.

::

    CREATE [OR REPLACE] PROCEDURE [schema_name.]procedure_name ([<parameter_definition> [, <parameter_definition>] ...])
    [<procedure_properties>]
    {IS | AS} LANGUAGE <lang> 
      <body>
    COMMENT 'procedure_comment';
    
        <parameter_definition> ::= parameter_name [mode] sql_type [ { DEFAULT | = } <default_expr> ] [COMMENT 'parameter_comment_string']
        <lang> ::= [PLCSQL | JAVA]
        <mode> ::= IN | OUT | IN OUT | INOUT
        <procedure_properties> ::= 
          <authid> = AUTHID {DEFINER | OWNER | CALLER | CURRENT_USER}

**OR REPLACE** 구문을 사용하여 현재의 저장 함수/프로시저를 대체 혹은 새로 생성하는 문장을 작성할 수 있다.

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *procedure_name*: 생성할 저장 프로시저의 이름을 지정한다. (최대 222바이트)
*   *parameter_name*: 인자의 이름을 지정한다. (최대 254바이트)
*   *sql_type*: 인자의 데이터 타입을 지정한다.
*   *default_arg*: 인자의 기본값을 지정한다. :ref:`pl-default-argument`\를 참고한다.
*   *authid*: 저장 프로시저의 실행 권한을 지정한다. 자세한 내용은 :doc:`/pl/pl_authid`\을 참고한다.
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

    CREATE OR REPLACE FUNCTION test(i in int COMMENT 'arg i') 
    RETURN int AS LANGUAGE JAVA NAME 'SpTest.testInt(int) return int' COMMENT 'function test';

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

    sp_name               pkg_name              sp_type               return_type             arg_count  lang                  authid                is_deterministic      target                                                                                      owner    code    comment             
    ============================================================================================================================================================================================================================================================================================
    'athlete_add'         NULL                  'PROCEDURE'           'void'                          4  'JAVA'                'DEFINER'             'NO'                  'Athlete.Athlete(java.lang.String, java.lang.String, java.lang.String, java.lang.String)'   'DBA'    NULL    NULL 

.. code-block:: sql
    
    SELECT * FROM db_stored_procedure_args WHERE sp_name = 'athlete_add';
    
::

    sp_name               owner_name            pkg_name                 index_of  arg_name              data_type             mode                  is_optional           default_value         comment           
    =======================================================================================================================================================================================================
     'athlete_add'         'DBA'                 NULL                            0  'name'                'STRING'              'IN'                  'NO'                  NULL                  NULL              
     'athlete_add'         'DBA'                 NULL                            1  'gender'              'STRING'              'IN'                  'NO'                  NULL                  NULL              
     'athlete_add'         'DBA'                 NULL                            2  'nation_code'         'STRING'              'IN'                  'NO'                  NULL                  NULL              
     'athlete_add'         'DBA'                 NULL                            3  'event'               'STRING'              'IN'                  'NO'                  NULL                  NULL


.. _create-function:

CREATE FUNCTION
=================

**CREATE FUNCTION** 문을 사용하여 저장 함수를 등록한다.

::

    CREATE [OR REPLACE] FUNCTION [schema_name.]function_name ([<parameter_definition> [, <parameter_definition>] ...])
    RETURN sql_type
    [<function_properties>]
    {IS | AS} LANGUAGE <lang> 
      <body>
    COMMENT 'function_comment';
    
        <parameter_definition> ::= parameter_name [mode] sql_type [<default_arg>] [COMMENT 'param_comment_string']
            <default_arg> ::= { DEFAULT | = } <default_expr>
        <procedure_properties> ::= <authid> | <deterministic>
            <authid> = AUTHID {DEFINER | OWNER | CALLER | CURRENT_USER}
            <deterministic> = [NOT DETERMINISTIC | DETERMINISTIC]
        <lang> ::= [PLCSQL | JAVA]
        <mode> ::= IN | OUT | IN OUT | INOUT

*   *schema_name*: 스키마 이름을 지정한다(최대 31바이트). 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *function_name*: 생성할 저장 함수의 이름을 지정한다(최대 222바이트).
*   *parameter_name*: 인자의 이름을 지정한다(최대 254바이트).
*   *sql_type*: 인자 또는 리턴 값의 데이터 타입을 지정한다. 지정할 수 있는 데이터 타입은 :ref:`pl-supported_sql_type`\을 참고한다.
*   *default_arg*: 인자의 기본값을 지정한다. :ref:`pl-default-argument`\를 참고한다.
*   *param_comment_string*: 인자 커멘트 문자열을 지정한다.
*   *authid*: 저장 함수의 실행 권한을 지정한다. 자세한 내용은 :doc:`/pl/pl_authid`\을 참고한다.
*   *deterministic*: 저장 함수가 결정적 함수인지 여부를 지정한다. 자세한 내용은 :ref:`pl-deterministic`\을 참고한다.
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

    CREATE OR REPLACE FUNCTION test(i in int COMMENT 'arg i') 
    RETURN int AS LANGUAGE JAVA NAME 'SpTest.testInt(int) return int' COMMENT 'function test';

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

    sp_name               pkg_name              sp_type               return_type             arg_count  lang                  authid                is_deterministic      target                                              owner      code      comment             
    ======================================================================================================================================================================================================================================================
    'hello'               NULL                  'FUNCTION'            'STRING'                        0  'JAVA'                'DEFINER'             'NO'                  'SpCubrid.HelloCubrid() return java.lang.String'    'DBA'      NULL      NULL                
    'sp_int'              NULL                  'FUNCTION'            'INTEGER'                       1  'JAVA'                'DEFINER'             'NO'                  'SpCubrid.SpInt(int) return int'                    'DBA'      NULL      NULL  

.. code-block:: sql
    
    SELECT * FROM db_stored_procedure_args WHERE sp_name = 'sp_int';
    
::

    sp_name               owner_name            pkg_name                 index_of  arg_name              data_type             mode                  is_optional           default_value         comment           
    =======================================================================================================================================================================================================
     'sp_int'              'DBA'                 NULL                            0  'i'                   'INTEGER'             'IN'                  'NO'                  NULL                  NULL    


CREATE FUNCTION DETERMINISTIC
------------------------------------------

NOT DETERMINISTIC 키워드는 저장 함수가 동일한 입력값에 대해 다른 결과를 반환하는 함수이다.
NOT DETERMINISTIC으로 설정된 함수는 부질의 결과 캐시 최적화의 대상에서 제외되며, 매 호출 시 결과가 재계산된다.
기본값은 NOT DETERMINISTIC이다.

DETERMINISTIC 키워드는 저장 함수가 동일한 입력값에 대해 항상 동일한 결과를 반환하는 함수이다. 
DETERMINISTIC으로 설정된 함수는 상관 부질의(correlated subquery) 사용 시, 질의 최적화기가 해당 함수를 부질의 결과 캐시 최적화의 대상으로 처리한다.

상관 부질의 캐시 동작 방식에 대한 자세한 내용은 :ref:`correlated-subquery-cache`\을 참고한다.

다음은 DETERMINISTIC을 사용한 저장 함수의 예시이다. 이 예시에서는 상관 부질의를 사용할 때 결과를 캐시하여 성능을 최적화하는 과정을 보여준다.

.. code-block:: sql

    CREATE TABLE dummy_tbl (col1 INTEGER);
    INSERT INTO dummy_tbl VALUES (1), (2), (1), (2);

    CREATE OR REPLACE FUNCTION pl_csql_not_deterministic (n INTEGER) RETURN INTEGER AS
    BEGIN
      return n + 1;
    END;

    CREATE OR REPLACE FUNCTION pl_csql_deterministic (n INTEGER) RETURN INTEGER DETERMINISTIC AS
    BEGIN
      return n + 1;
    END;

    SELECT sp_name, owner, sp_type, is_deterministic from db_stored_procedure;

::
    
    sp_name                      owner           sp_type               is_deterministic    
 ========================================================================================
    'pl_csql_not_deterministic'  'DBA'           'FUNCTION'            'NO'                
    'pl_csql_deterministic'      'DBA'           'FUNCTION'            'YES' 

위 예시에서 pl_csql_not_deterministic 함수는 NOT DETERMINISTIC이므로 상관 부질의에서 캐시를 사용하지 않는다.
반면, pl_csql_deterministic 함수는 DETERMINISTIC 키워드가 지정되어 있으므로 상관 부질의 결과를 캐시하여 성능을 최적화할 수 있다.

.. code-block:: sql
    
    ;trace on
    SELECT (SELECT pl_csql_not_deterministic (t1.col1) FROM dual) AS results FROM dummy_tbl t1;

::

      results
 =============
            2
            3
            2
            3
 
 === Auto Trace ===
    ...
    Trace Statistics:
      SELECT (time: 3, fetch: 44, fetch_time: 0, ioread: 0)
        SCAN (table: dba.dummy_tbl), (heap time: 0, fetch: 20, ioread: 0, readrows: 4, rows: 4)
        SUBQUERY (correlated)
          SELECT (time: 3, fetch: 24, fetch_time: 0, ioread: 0)
            SCAN (table: dual), (heap time: 0, fetch: 16, ioread: 0, readrows: 4, rows: 4)

pl_csql_not_deterministic 함수는 NOT DETERMINISTIC이므로 부질의 결과를 캐시하지 않는다.

.. code-block:: sql
    
    ;trace on
    SELECT (SELECT pl_csql_deterministic (t1.col1) FROM dual) AS results FROM dummy_tbl t1;

::

      results
 =============
            2
            3
            2
            3

 === Auto Trace ===
    ...
    Trace Statistics:
      SELECT (time: 3, fetch: 36, fetch_time: 0, ioread: 0)
        SCAN (table: dba.dummy_tbl), (heap time: 0, fetch: 20, ioread: 0, readrows: 4, rows: 4)
        SUBQUERY (correlated)
          SELECT (time: 3, fetch: 16, fetch_time: 0, ioread: 0)
            SCAN (table: dual), (heap time: 0, fetch: 8, ioread: 0, readrows: 2, rows: 2)
            SUBQUERY_CACHE (hit: 2, miss: 2, size: 150808, status: enabled)

pl_csql_deterministic 함수의 Trace 결과에서는 SUBQUERY_CACHE 항목이 표시되며(hit: 2, miss: 2, size: 150808, status: enabled), 첫 번째 결과 (2), (3)은 캐시에서 miss되었고, 이후 동일한 결과부터는 캐시에서 hit된 것을 확인할 수 있다.

ALTER PROCEDURE
================

**ALTER PROCEDURE** 문을 사용하여 저장 프로시저를 재컴파일할 수 있다.
저장 프로시저와 연관된 테이블의 스키마가 변경되더라도 자동으로 재컴파일되지 않으므로, 변경 사항을 반영하려면 사용자가 직접 재컴파일해야 한다.

::

    ALTER PROCEDURE [schema_name.]procedure_name COMPILE;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *procedure_name*: 재컴파일할 프로시저의 이름을 지정한다.

.. note::

    소유자를 변경하는 경우, 변경된 소유자로 저장 프로시저를 자동으로 재컴파일한다. 
    소유자를 변경하기 위해서는 :ref:`ALTER … OWNER<change-owner>`\을 참고한다.

다음은 테이블 스키마 변경 후 PL/CSQL을 재컴파일하여 정상적으로 실행할 수 있게 만드는 예이다.  

PL/CSQL에 Static SQL을 사용하는 저장 프로시저를 생성한 후 정상적으로 실행되는지 확인한다. 

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE proc_stadium_code() AS
      n INTEGER;
    BEGIN
      SELECT code INTO n FROM stadium LIMIT 1;
      DBMS_OUTPUT.put_line('code :' || n);
    END;
    
    ;server-output on
    CALL proc_stadium_code();

::
    
    Result              
    ======================
      NULL                

    <DBMS_OUTPUT>
    ====
    code :30140

stadium 테이블의 code 컬럼 타입을 INTEGER에서 VARCHAR로 변경한 후 저장 프로시저를 실행하면 아래와 같은 에러가 발생한다.

.. code-block:: sql

    ALTER TABLE public.stadium MODIFY code VARCHAR;

    CALL proc_stadium_code();

::

    ERROR: Stored procedure execute error: 
      (line 4, column 3) internal server error

컬럼 타입 변경 정보가 기존에 컴파일된 PL/CSQL의 실행코드에 반영되지 않았기 때문에, 저장 프로시저를 재컴파일해야 정상적으로 실행할 수 있다.

.. code-block:: sql

    ALTER PROCEDURE proc_stadium_code COMPILE;

    CALL proc_stadium_code();

::

    Result              
    ======================
      NULL                

    <DBMS_OUTPUT>
    ====
    code :30140

ALTER FUNCTION
===============

**ALTER FUNCTION** 문을 사용하여 저장 함수를 재컴파일할 수 있다.
저장 함수와 연관된 테이블의 스키마가 변경되더라도 자동으로 재컴파일되지 않으므로, 변경 사항을 반영하려면 사용자가 직접 재컴파일해야 한다.

::

    ALTER FUNCTION [schema_name.]function_name COMPILE;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *function_name*: 재컴파일할 함수의 이름을 지정한다.

.. note::

    소유자를 변경하는 경우, 변경된 소유자로 저장 함수를 자동으로 재컴파일한다.
    소유자를 변경하기 위해서는 :ref:`ALTER … OWNER<change-owner>`\을 참고한다.

다음은 테이블 스키마 변경 후 PL/CSQL을 재컴파일하여 정상적으로 실행할 수 있게 만드는 예이다. 

PL/CSQL에 Static SQL을 사용하는 저장 함수를 생성한 후 정상적으로 실행되는지 확인한다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION func_stadium_code() RETURN INTEGER AS
      n INTEGER;
    BEGIN
      SELECT code INTO n FROM stadium LIMIT 1;
      RETURN n;
    END;
    
    CALL func_stadium_code();

::
    
    Result
    =============
    30140

stadium 테이블의 code 컬럼 타입을 INTEGER에서 VARCHAR로 변경한 후 저장 함수를 실행하면 아래와 같은 에러가 발생한다.

.. code-block:: sql

    ALTER TABLE public.stadium MODIFY code VARCHAR;

    CALL func_stadium_code();

::

    ERROR: Stored procedure execute error: 
      (line 4, column 3) internal server error

컬럼 타입 변경 정보가 기존에 컴파일된 PL/CSQL의 실행코드에 반영되지 않았기 때문에, 저장 함수를 재컴파일을 수행해야 정상적으로 실행할 수 있다.

.. code-block:: sql

    ALTER FUNCTION func_stadium_code COMPILE;

    CALL func_stadium_code();

::
    
    Result
    =============
    30140

DROP PROCEDURE
==============

CUBRID에서는 등록한 저장 프로시저를 **DROP PROCEDURE** 구문을 사용하여 삭제할 수 있다.
이 때, 여러 개의 *procedure_name* 을 콤마(,)로 구분하여 한꺼번에 여러 개의 저장 프로시저를 삭제할 수 있다.

::

    DROP PROCEDURE [schema_name.]procedure_name [{ , [schema_name.]procedure_name , ... }];

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *procedure_name*: 제거할 프로시저의 이름을 지정한다.

.. code-block:: sql

    DROP PROCEDURE hello, public.sp_int;

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

    DROP FUNCTION hello, public.sp_int;

저장 함수의 삭제는 함수를 등록한 사용자와 DBA의 구성원만 삭제할 수 있다.
예를 들어 'sp_int' 저장 함수를 **PUBLIC** 이 등록했다면, **PUBLIC** 또는 **DBA** 의 구성원만이 'sp_int' 저장 함수를 삭제할 수 있다.
