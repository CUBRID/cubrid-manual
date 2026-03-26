
:meta-keywords: set statement, set system parameter, set user variable
:meta-description: The SET statement is the syntax that specifies a system parameter's value or user-defined variables.


***
SET
***

**SET** 문은 시스템 파라미터의 값을 지정하거나 사용자 정의 변수의 값을 지정하는 구문이다.

시스템 파라미터
===============

SQL 문을 이용하여 CSQL 인터프리터나 CUBRID 매니저의 질의 편집기에서 시스템 파라미터의 값을 설정할 수 있다. 단, 갱신할 수 있는 파라미터는 한정되어 있으므로 주의한다. 갱신할 수 있는 파라미터를 확인하려면 :ref:`cubrid-conf`\를 참고한다.

::

    SET SYSTEM PARAMETERS 'parameter_name=value [{; name=value}...]'

*value* : **call_stack_dump_activation_list** 파라미터를 제외하고 해당 파라미터의 *value* 를 **DEFAULT** 로 설정하면  기본값으로 재설정된다.

.. code-block:: sql

    SET SYSTEM PARAMETERS 'lock_timeout=DEFAULT';


사용자 변수
===========

사용자 정의 변수는 2가지 방법으로 생성될 수 있다. 하나는 **SET** 문을 사용하는 것이고, 다른 하나는 SQL문 내에서 사용자 정의 변수 할당 구문을 사용하는 것이다. 정의한 사용자 정의 변수는 **DEALLOCATE** 혹은 **DROP** 구문을 사용하여 삭제할 수 있다.

사용자 정의 변수는 하나의 응용 프로그램 내에서 연결을 유지하는 동안 사용되는 변수이므로 세션 변수라고도 한다. 사용자 정의 변수는 연결 세션 영역 내에서 사용되며, 하나의 응용 프로그램에 의해 정의된 사용자 변수는 다른 응용 프로그램이 볼 수 없다. 응용 프로그램이 연결을 종료하면 모든 변수는 자동으로 제거된다. 사용자 정의 변수는 응용 프로그램의 연결 세션 당 20개로 제한되어 있다. 사용자 정의 변수가 20개일 때 새 변수를 정의하고 싶으면, **DROP VARIABLE** 구문을 사용하여 사용하지 않는 일부 변수를 제거해야 한다.

대부분의 SQL 구문에서는 사용자 정의 변수를 사용할 수 있다. 한 구문에서 사용자 정의 변수를 지정하고 참조할 때에는 그 순서가 보장되지 않는다. 즉, **HAVING**, **GROUP BY** 또는 **ORDER BY** 절의 **SELECT** 리스트에 지정된 사용자 정의 변수를 참조하면 기대한 순서대로 값을 가져오지 않을 수도 있다. 또한, 사용자 정의 변수는 SQL 문 내에서 칼럼 이름이나 테이블 이름 같은 식별자로 사용할 수 없다.

사용자 정의 변수는 대소문자를 구분하지 않는다. 사용자 정의 변수의 타입은 **SHORT**, **INTEGER**, **BIGINT**, **FLOAT**, **DOUBLE**, **NUMERIC**, **CHAR**, **VARCHAR**, **BIT**, **BIT VARYING** 중 하나가 될 수 있으며, 그 밖의 타입은 **VARCHAR** 타입으로 변환된다.

.. code-block:: sql

    SET @v1 = 1, @v2=CAST(1 AS BIGINT), @v3 = '123', @v4 = DATE'2010-01-01';
     
    SELECT typeof(@v1), typeof(@v2), typeof(@v3), typeof(@v4);
     
::

       typeof(@v1)         typeof(@v2)         typeof(@v3)         typeof(@v4)
    ======================================================================================
      'integer'           'bigint'            'character (-1)'    'character varying (1073741823)

사용자 정의 변수의 타입은 사용자가 값을 지정할 때 바뀔 수 있다.

.. code-block:: sql

    SET @v = 'a'; 
    SET @v1 = 10;

    SELECT @v := 1, typeof(@v1), @v1:='1', typeof(@v1);
     
::

      @v := 1                typeof(@v1)          @v1 := '1'             typeof(@v1)
    ======================================================================================
      1                     'integer'             '1'                    'character (-1)'

::

    <set_statement>
            : <set_statement>, <udf_assignment>
            | SET <udv_assignment>
            ;
     
    <udv_assignment>
            : @<name> = <expression>
            | @<name> := <expression>
            ;
     
    {DEALLOCATE|DROP} VARIABLE <variable_name_list>
    <variable_name_list>
           : <variable_name_list> ',' @<name>

*   사용자 정의 변수의 이름은 영숫자(alphanumeric)와 언더바(_)로 정의한다.
*   SQL 문 내에서 사용자 정의 변수를 선언할 때에는 ':=' 연산자를 사용한다.

사용자 정의 변수 a를 선언하고, 값 1을 할당한다.

.. code-block:: sql

    SET @a = 1;
    SELECT @a;

::

      @a
    ======================
      1

사용자 정의 변수를 사용하여 **SELECT** 문에서 행의 개수를 카운트한다.

.. code-block:: sql

    CREATE TABLE t (i INTEGER);
    INSERT INTO t(i) VALUES(2),(4),(6),(8);
     
    SET @a = 0;
     
    SELECT @a := @a+1 AS row_no, i FROM t;

::

      row_no                          i
     ===================================
      1                               2
      2                               4
      3                               6
      4                               8
      
    4 rows selected.

사용자 정의 변수를 prepared statement에서 지정한 바인드 파라미터의 입력으로 사용한다.

.. code-block:: sql

    SET @a:=3;
     
    PREPARE stmt FROM 'SELECT i FROM t WHERE i < ?';
    EXECUTE stmt USING @a;

::

                i
    =============
                2

SQL 문 내에서 ':=' 연산자를 사용하여 사용자 정의 변수를 선언한다.

.. code-block:: sql

    SELECT @a := 1, @user_defined_variable := 'user defined variable';
    UPDATE t SET i = (@var := 1);

사용자 정의 변수 *a* 와 *user_defined_variable* 를 삭제한다.

.. code-block:: sql

    DEALLOCATE VARIABLE @a, @user_defined_variable;
    DROP VARIABLE @a, @user_defined_variable;

.. note:: \

    **SET** 문에 의해 정의되는 사용자 정의 변수는 응용 프로그램이 서버에 연결하면서 시작되어 응용 프로그램이 연결을 종료할 때까지 유지되며, 이 기간동안 유지되는 연결을 세션(session)이라고 한다. 사용자 정의 변수는 응용 프로그램이 연결을 종료하거나 일정 기간 동안 요청이 없어 세션 기간이 만료될(expired) 때 삭제된다. 세션 기간은 **cubrid.conf** 의 **session_state_timeout** 파라미터로 설정할 수 있으며, 기본값은 **21600** 초(=6시간)이다.

    세션에 의해 관리되는 데이터는 **PREPARE** 문 외에 사용자 정의 변수, 가장 마지막에 삽입한 ID(**LAST_INSERT_ID**), 가장 마지막에 실행한 문장에 의해 영향 받은 레코드의 개수(**ROW_COUNT**)를 포함한다.
