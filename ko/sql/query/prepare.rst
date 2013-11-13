******************
PREPARED STATEMENT
******************

prepared statement 기능은 보통 JDBC, PHP, ODBC 등의 인터페이스 함수를 통해서 사용할 수 있는데, SQL 레벨에서도 직접 수행할 수 있다. prepared statement 사용을 위해 다음의 SQL 문을 제공한다.

* 실행하고자 하는 SQL 문을 준비한다. ::

    PREPARE stmt_name FROM preparable_stmt

* prepared statement를 실행한다. ::

    EXECUTE stmt_name [USING value [, value] ...]

* prepared statement를 해제한다. ::

    {DEALLOCATE | DROP} PREPARE stmt_name

.. note::

    *   SQL 수준의 PREPARE 문은 CSQL 인터프리터에서만 사용할 것을 권장한다. 응용 프로그램에서 사용하는 경우 정상 동작을 보장하지 않는다.
    *   SQL 수준의 PREPARE 문은 DB 연결 당 개수가 최대 20개로 제한된다. SQL 수준의 PREPARE 문은 DB 서버의 메모리 자원을 사용하므로 DB 서버 메모리의 남용을 방지하기 위해 제한된다.
    *   인터페이스 함수의 prepared statement는 브로커 파라미터인 :ref:`MAX_PREPARED_STMT_COUNT <max-prepared-stmt-count>` 를 통해 DB 연결 당 prepared statement 개수가 제한된다. CUBRID SHARD 를 사용하는 경우 shard proxy 파라미터인 :ref:`SHARD_MAX_PREPARED_STMT_COUNT <shard-max-prepared-stmt-count>` 를 통해 shard proxy 하나 당 prepared statement 개수가 제한된다.

PREPARE 문
==========

**PREPARE** 문은 **FROM** 절의 *preparable_stmt* 에 지정된 질의문을 준비하고, 이후에 해당 SQL 문을 참조할 때 사용될 이름을 *stmt_name* 에 할당한다. 예제는 :ref:`execute-statement` 을 참고한다. ::

    PREPARE stmt_name FROM preparable_stmt

*   *stmt_name* : prepared statement의 이름을 할당한다. 해당 클라이언트 세션에 이미 동일한 *stmt_name* 을 가지는 SQL 문이 존재하면, 기존 prepared statement을 해제한 후 새로운 SQL 문을 준비한다. 주어진 SQL 문의 오류로 인해 **PREPARE** 문이 정상 수행되지 않는 경우, 해당 SQL 문에 할당된 *stmt_name* 은 존재하지 않는 것으로 처리된다.

*   *preparable_stmt* : 반드시 단일 SQL 문이어야 하며, 여러 개의 SQL 문을 지정할 수 없다. *preparable_stmt* 인자에 바인드 파라미터(?)를 사용할 수 있으며, 이를 따옴표로 감싸지 않아야 한다.

.. note:: \

    **PREPARE** 문은 응용 프로그램이 서버에 연결하면서 시작되며 응용 프로그램이 연결을 종료하거나 세션 기간이 만료되기 전까지 유지된다. 세션 기간은 시스템 파라미터의 **session_state_timeout** 파라미터로 설정할 수 있으며, 기본값은 **21600** 초(=6시간)이다. 

    세션에 의해 관리되는 데이터는 **PREPARE** 문 외에 사용자 정의 변수, 가장 마지막에 삽입한 ID(**LAST_INSERT_ID**), 가장 마지막에 실행한 문장에 의해 영향 받은 레코드의 개수(**ROW_COUNT**)를 포함한다.

.. _execute-statement:

EXECUTE 문
==========

**EXECUTE** 문은 prepared statement을 실행하며, prepared statement에 바인드 파라미터(?)를 포함하면 **USING** 절 뒤에 데이터 값을 바인딩할 수 있다. **USING** 절에서는 세션 변수뿐만이 아니라 리터럴, 입력 파라미터와 같은 값도 지정할 수 있다. ::

    EXECUTE stmt_name [USING value [, value] ...]

*   *stmt_name* : 실행하고자 하는 prepared statement에 부여된 이름을 지정한다. *stmt_name* 이 유효하지 않거나 prepared statement가 존재하지 않는 경우 에러가 출력된다.

*   *value* : 바인드 파라미터가 prepared statement에 있는 경우 바인딩할 데이터를 입력한다. 바인드 파라미터와 데이터의 개수 및 순서가 대응되어야 한다. 그렇지 않으면 에러가 출력된다.

.. code-block:: sql

    PREPARE st FROM 'SELECT 1 + ?';
    EXECUTE st USING 4;
    
::

       1+ ?:0
    ==========================
       5
     
.. code-block:: sql

    PREPARE st FROM 'SELECT 1 + ?';
    SET @a=3;
    EXECUTE st USING @a;
    
::

       1+ ?:0
    ==========================
       4
     
.. code-block:: sql

    PREPARE st FROM 'SELECT ? + ?';
    EXECUTE st USING 1,3;
    
::

       ?:0 + ?:1
    ==========================
       4
     
.. code-block:: sql

    PREPARE st FROM 'SELECT ? + ?';
    EXECUTE st USING 'a','b';
    
::

       ?:0 + ?:1
    ==========================
       'ab'
     
.. code-block:: sql

    PREPARE st FROM 'SELECT FLOOR(?)';
    EXECUTE st USING '3.2';
    
::

       floor( ?:0 )
    ==========================
       3.000000000000000e+000

DEALLOCATE PREPARE 문, DROP PREPARE 문
======================================

**DEALLOCATE PREPARE** 문과 **DROP PREPARE** 문은 동일하며, prepared statement를 해제한다. **DEALLOCATE PREPARE** 문 또는 **DROP PREPARE** 문을 수행하지 않더라도 클라이언트 세션이 종료되면, 서버에 의해 모든 prepared statement가 자동 해제된다. ::

    {DEALLOCATE | DROP} PREPARE stmt_name

*   *stmt_name* : 해제하고자 하는 prepared statement에 부여된 이름을 지정한다. *stmt_name* 이 유효하지 않거나 prepared statement가 존재하지 않으면 에러가 출력된다.

.. code-block:: sql

    DEALLOCATE PREPARE stmt1;

***
SET
***

**SET** 문은 사용자 정의 변수를 지정하는 구문이며, 사용자가 값을 저장하는 방법이다.

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
      1                     'integer'            '1'                    'character (-1)'
  
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

**
DO
**

**DO** 문은 지정된 연산식을 실행하지만 결과 값을 리턴하지 않는다. 지정된 연산식이 문법에 맞게 쓰여지지 않으면 에러를 반환하므로, 연산식의 문법이 올바른지 여부를 확인하는 데 사용할 수 있다. **DO** 문은 데이터베이스 서버에서 연산 결과 또는 에러를 반환하지 않기 때문에, 일반적으로 **SELECT** 문보다 수행 속도가 빠르다. ::

    DO expression

*   *expression* : 임의의 연산식을 지정한다.

.. code-block:: sql

    DO 1+1;
    DO SYSDATE + 1;
    DO (SELECT count(*) FROM athlete);
