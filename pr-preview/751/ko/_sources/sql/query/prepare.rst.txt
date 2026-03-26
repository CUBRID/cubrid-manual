
:meta-keywords: prepare statement, execute prepared, deallocate prepare, drop prepare

******************
PREPARED STATEMENT
******************

prepared statement 기능은 보통 JDBC, PHP, ODBC 등의 인터페이스 함수를 통해서 사용할 수 있는데, SQL 레벨에서도 직접 수행할 수 있다. prepared statement 사용을 위해 다음의 SQL 문을 제공한다.

*   실행하고자 하는 SQL 문을 준비한다. 

    ::

        PREPARE stmt_name FROM preparable_stmt

*   prepared statement를 실행한다. 

    ::

        EXECUTE stmt_name [USING value [, value] ...]

*   prepared statement를 해제한다. 

    ::

        {DEALLOCATE | DROP} PREPARE stmt_name

.. note::

    *   SQL 수준의 PREPARE 문은 CSQL 인터프리터에서만 사용할 것을 권장한다. 응용 프로그램에서 사용하는 경우 정상 동작을 보장하지 않는다.
    *   SQL 수준의 PREPARE 문은 DB 연결 당 개수가 최대 20개로 제한된다. SQL 수준의 PREPARE 문은 DB 서버의 메모리 자원을 사용하므로 DB 서버 메모리의 남용을 방지하기 위해 제한된다.
    *   인터페이스 함수의 prepared statement는 브로커 파라미터인 :ref:`MAX_PREPARED_STMT_COUNT <max-prepared-stmt-count>` 를 통해 DB 연결 당 prepared statement 개수가 제한된다. 

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
