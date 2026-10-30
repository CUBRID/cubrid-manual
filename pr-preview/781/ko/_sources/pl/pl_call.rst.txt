-----------------------------
저장 프로시저의 호출
-----------------------------

등록된 저장 프로시저와 저장 함수는 :doc:`/sql/query/call` 구문을 사용하거나 SQL 문에서 호출할 수 있다.
일반적인 경우 사용자의 용도에 따라 저장 프로시저는 :doc:`/sql/query/call` 구문을 사용하여 호출하고, 저장 함수는 그 외 반환 값이 필요한 SQL 질의문에서 사용한다.

이 장에서는 저장 프로시저와 저장 함수를 호출과 관련하여 고려해야 할 사항에 대하여 자세히 설명한다.

.. _pl_call_stmt:

CALL 문
========

:doc:`/sql/query/call` 구문을 사용하여 다음과 같이 CSQL 인터프리터에서 저장 프로시저를 호출할 수 있다.

.. code-block:: sql

    -- csql에서 실행
    CREATE PROCEDURE hello ()
    AS
    BEGIN
        DBMS_OUTPUT.put_line('Hello, CUBRID!');
    END;

    ;server-output on

    -- 호출
    CALL hello();

::
    
      Result              
    ======================
      NULL                

    <DBMS_OUTPUT>
    ====
    Hello, CUBRID!

.. _pl_call_sql:

SQL 문에서 호출
===================

SQL 문에서 호출하는 경우 일반적으로 저장 함수를 사용하여 값을 반환한다. 
다음은 저장 함수 사용의 예시로 특정 국가의 메달 수를 계산하는 예시를 보여준다.

.. code-block:: sql

    CREATE FUNCTION count_medals(nation STRING) RETURN INT
    AS
    cnt INT;
    BEGIN
        SELECT COUNT(*) INTO cnt
        FROM game
        WHERE nation_code = nation;
        RETURN cnt;
    END;

    SELECT count_medals('USA');
    SELECT count_medals('KOR');

::

      count_medals('USA')
    =====================
                     1118

      count_medals('KOR')
    =====================
                      316

.. _pl_nested_call_limits:

프로시저 호출 시 중첩 호출 제한사항
====================================

저장 프로시저와 저장 루틴의 허용된 최대 중첩 호출 수는 **16**\으로 제한된다.
중첩 호출이란 프로시저 내부에서 다른 프로시저를 호출하는 것을 의미한다.
이 때 질의문을 통한 호출이 아닌 재귀 호출의 경우에는 제한하지 않는다.

다음은 중첩 호출 제한 사항에 대한 예시이다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_factorial_query(n BIGINT) RETURN BIGINT
    AS
    k BIGINT;
    BEGIN
        IF n = 0 THEN
            RETURN 1;
        ELSE
            SELECT test_factorial_query(n - 1) INTO k;
            RETURN n * k;
        END IF;
    END;

    SELECT test_factorial_query(15);
    SELECT test_factorial_query(16);

::

      test_factorial_query(15)
    ==========================
                 1307674368000


    -- SELECT test_factorial_query(16);
    ERROR: Stored procedure execute error:
      ...
      (line 8, column 13) Stored procedure execute error:
      (line 8, column 13) Stored procedure execute error: Too many nested stored procedure call.

다음은 질의문을 통하지 않고 재귀 호출을 하는 예시이다. 이 경우 중첩 호출 제한 사항이 적용되지 않는다.
재귀 호출을 통하여 다음과 같이 팩토리얼의 값이 BIGINT 값을 넘어 오버플로우가 발생하거나, 시스템 자원을 과도하게 사용하는 경우 질의가 실패할 수 있으므로 주의한다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_factorial_constant(n BIGINT) RETURN BIGINT
    AS
    k BIGINT;
    BEGIN
        IF n = 0 THEN
            RETURN 1;
        ELSE
            RETURN n * test_factorial_constant(n - 1);
        END IF;
    END;

    SELECT test_factorial_constant(16);
    SELECT test_factorial_constant(25);

::

      test_factorial_constant(16)
    =============================
                   20922789888000

    -- SELECT test_factorial_constant(25);
    ERROR: Stored procedure execute error: 
      (line 8, column 20) data overflow in multiplication of BIGINT values
