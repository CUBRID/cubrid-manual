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

.. _pl-call-permission:

프로시저의 호출 권한
============================================

프로시저를 생성하면 프로시저를 생성한 사용자의 스키마에 속하며, 오직 **소유자**와 **DBA** 권한을 가진 사용자만이 프로시저를 호출할 수 있다.
프로시저의 소유자가 다른 사용자에게 프로시저에 대한 호출 권한을 부여하면, 해당 사용자는 권한을 부여받은 다른 사용자의 프로시저를 호출할 수 있다.

이 때 주의해야할 점은 구현하는 일련의 루틴에서 권한이 없는 객체를 참조하는 경우에는 컴파일 또는 실행 시 오류가 발생할 수 있다.
따라서 저장 프로시저에서 참조하는 객체에 대해서도 권한이 있는지 확인하는 것이 필요하다.

CUBRID에서는 :ref:`GRANT <granting-authorization>` 문을 사용하여 프로시저에 대한 호출 권한을 부여할 수 있다.

.. code-block:: sql

    GRANT EXECUTE ON PROCEDURE procedure_name TO user_name;

반대로 :ref:`REVOKE <revoking-authorization>` 문을 사용하여 프로시저에 대한 호출 권한을 해지할 수 있다.

.. code-block:: sql

    REVOKE EXECUTE ON PROCEDURE procedure_name FROM user_name;

.. note::
        
    저장 프로시저와 저장 함수에 대한 **EXECUTE ON PROCEDURE** 권한은 **WITH GRANT OPTION** 을 지원하지 않는다.
    따라서 프로시저의 소유자가 부여한 권한을 다른 사용자에게 부여할 수 없다.

.. _pl_authid:

소유자 권한과 호출자 권한
============================================

저장 프로시저와 저장 함수는 **소유자** 권한 또는 **호출자** 권한으로 실행될 수 있다. 실행 권한은 저장 프로시저 또는 저장함수 생성시 둘 중 하나를 선택하여 생성할 수 있으며, 지정한 권한으로 실행된다. 

다음은 각 권한에 대한 설명과 차이점을 설명한다.

* **소유자 권한** (Owner's Rights 또는 Definer's Rights)
        * 저장 프로시저를 만든 사용자의 권한으로 실행된다.
        * 저장 프로시저를 만든 사용자가 다른 사용자에게 권한을 부여하면 부여된 사용자도 저장 프로시저를 실행할 수 있다.
        * 저장 프로시저의 소유자의 모든 권한을 사용 가능하므로 별도의 권한 부여 없이 소유자가 접근할 수 있는 데이터베이스 객체에 접근할 수 있다. **DBA**\의 소유자 권한으로 저장 프로시저를 만들면 모든 데이터베이스 객체에 접근할 수 있으므로 특히 동적 SQL(Dynamic SQL)을 사용할 때 SQL 인젝션(SQL Injection)과 같은 보안 문제에 주의해야 한다.
        * 공통적인 작업이나 데이터 접근을 필요로 하는데 적합하다.

반면에 호출자 권한은 다음과 같다.

* **호출자 권한** (Caller's Rights 또는 Invoker's Rights)
        * 저장 프로시저를 만든 사용자의 권한이 아닌 호출자의 권한으로 실행된다.
        * 호출자 권한으로 저장 프로시저를 만들고 다른 유저에게 권한을 부여하면, 저장 프로시저를 호출하는 사용자의 권한으로 실행된다.
        * 호출자 권한으로 저장 프로시저를 만들면 저장 프로시저를 만든 사용자의 권한이 아닌 호출자의 권한 수준에서 실행되므로 호출자의 권한으로 데이터베이스 객체에 접근할 수 있다. 따라서 호출자 권한으로 저장 프로시저를 만들 때는 호출자의 권한 수준을 고려하여 저장 프로시저를 만들어야 한다.
        * 호출자별로 권한을 다르게 적용해야할 경우에 적합하다.

저장 프로시저 생성 시 소유자 권한과 호출자 권한의 차이를 이해하고 저장 프로시저를 활용하면 보다 효율적으로 데이터베이스를 관리하는 것이 필요하다.

저장 프로시저 또는 저장 함수 생성 시 **AUTHID** 속성을 지정하여 **소유자 권한** 또는 **호출자 권한** 으로의 동작 여부를 지정할 수 있다. 정의문에 대한 자세한 내용은 :doc:`/sql/schema/stored_routine_stmt`\를 참고한다. 

**AUTHID** 속성을 다음과 같이 지정할 수 있으며, **DEFINER**\와 **OWNER** 그리고 **CURRENT_USER**\와 **CALLER**\는 동의어이다.

* **소유자 권한**: AUTHID DEFINER 또는 AUTHID OWNER
* **호출자 권한**: AUTHID CURRENT_USER 또는 AUTHID CALLER

속성을 지정하지 않으면 기본적으로 **소유자 권한**\으로 동작한다.

다음은 DBA 사용자로 로그인하여 :ref:`fn_current_user` 를 반환하는 DBA 소유자 권한의 저장 함수를 만들고 U1 사용자에서 호출하는 예이다.

.. code-block:: sql

        -- DBA로 로그인
        CREATE USER U1;

        CREATE OR REPLACE FUNCTION fn_current_user() RETURN STRING AUTHID DEFINER AS
        BEGIN
                RETURN CURRENT_USER;
        END;

        GRANT EXECUTE ON PROCEDURE fn_current_user TO U1;

        CALL login ('U1', '') ON CLASS db_user;

        SELECT dba.fn_current_user();

::

        dba.fn_current_user()
        ======================
        'DBA@<host>'    

.. warning::

        * CUBRID의 PL/CSQL에서는 현재 **소유자 권한** 만을 지원하고 있으며, **호출자 권한**\은 지원하지 않는다.
        * **호출자 권한**\을 사용하기 위해서는 Java SP를 사용하여 저장 프로시저를 작성해야 한다.

.. note::

        * **소유자 권한**\은 CUBRID 11.4 버전부터 지원한다.
