
:meta-keywords: cubrid transaction, database transaction, cubrid locking, database locking, cubrid concurrency, multiversion concurrency control, mvcc, isolation level, database recovery
:meta-description: This chapter covers issues relating to concurrency (MVCC) and restore, as well as how to commit or rollback transactions in CUBRID database.

.. _database-transaction:

데이터베이스 트랜잭션
=====================

데이터베이스 트랜잭션은 CUBRID 질의문을 일관성(다중 사용자 환경에서 유효한 결과를 만들어내는 것)과 복구(시스템 실패와 같은 어떤 장애에도 커밋된 트랜잭션의 결과를 유지하는 것과, 어떤 고장에도 불구하고 중단된 트랜잭션은 데이터베이스로부터 무효화되는 것을 보장하는 것)의 단위로 그룹화한다. 하나의 트랜잭션은 데이터베이스에 접근하고 갱신하는 하나의 질의문 또는 여러 질의문으로 구성된다.

CUBRID는 많은 사용자가 동시에 데이터베이스에 접근하도록 하고 데이터베이스의 불일치를 방지하기 위하여 사용자 간 접근과 갱신을 관리한다. 예를 들어 데이터가 한 사용자에 의해 갱신되었을 때 그 트랜잭션에 관련된 데이터의 변화는 갱신이 커밋될 때까지 다른 사용자나 데이터베이스에서 일어나는 다른 트랜잭션에 보이지 않는다. 트랜잭션이 커밋되지 않고 롤백될 수 있기 때문에 이 원칙은 중요하다.

트랜잭션 처리 결과를 확신할 때까지, 데이터베이스에 영구적으로 갱신하는 것을 연기할 수 있다. 또한 트랜잭션 처리 과정에서 응용 프로그램이나 컴퓨터 시스템에서 만족할 수 없는 결과나 실패가 발생하면 데이터베이스의 모든 갱신을 제거(**ROLLBACK**)할 수 있다. 트랜잭션의 끝은 **COMMIT WORK** 또는 **ROLLBACK WORK** 문으로 결정된다. **COMMIT WORK** 문은 데이터베이스의 모든 갱신을 영구적으로 만드는 반면에 **ROLLBACK WORK** 문은 트랜잭션에서 입력된 모든 갱신을 무효화시킨다.

트랜잭션 커밋
-------------

데이터베이스에서 일어난 갱신들은 **COMMIT WORK** 문이 주어지기 전까지 영구히 저장되지 않는다. "영구히(permanently)" 저장된다는 것은 디스크에 저장이 완료되는 것을 의미한다. 키워드 **WORK** 는 생략이 가능하다. 추가로 데이터베이스의 다른 사용자는 변경이 영구히 반영되기 전까지는 변경 사항을 볼 수 없다. 예를 들어 테이블에 새로운 행을 삽입했을 때 데이터베이스 트랜잭션이 커밋되기 전까지 그 행에 접근할 수 있는 것은 그 행을 삽입한 사용자뿐이다(**UNCOMMITTED INSTANCES** 격리 수준을 사용하면 다른 사용자가 일관성이 없는 커밋되지 않은 갱신을 볼 수도 있다).

트랜잭션이 커밋된 후에는 트랜잭션에서 획득한 모든 잠금이 해제된다. ::

    COMMIT [WORK];

.. code-block:: sql

    -- ;autocommit off
    -- AUTOCOMMIT IS OFF
    
    SELECT name, seats
    FROM stadium WHERE code IN (30138, 30139, 30140);

::

        name                                seats
    =============================================
        'Athens Olympic Tennis Centre'      3200
        'Goudi Olympic Hall'                5000
        'Vouliagmeni Olympic Centre'        3400

다음 **UPDATE** 문은 3개의 stadium의 seats 칼럼 값을 변경한다. 결과를 검토하기 위해 갱신이 일어나기 전에 현재의 값과 이름을 검색한다. 기본적으로 csql은 자동으로 autocommit으로 작동되므로, 예제에서는 autocommit 모드를 off로 설정한 후 동작을 시험한다.

.. code-block:: sql

    UPDATE stadium
    SET seats = seats + 1000
    WHERE code IN (30138, 30139, 30140);
     
    SELECT name, seats FROM stadium WHERE code in (30138, 30139, 30140);
    
::

        name                                seats
    ============================================
        'Athens Olympic Tennis Centre'      4200
        'Goudi Olympic Hall'                6000
        'Vouliagmeni Olympic Centre'        4400

만약 갱신이 제대로 이루어 졌다면 변경을 영구적으로 만들 수 있다. 이때 아래처럼 **COMMIT WORK** 문을 사용한다.

.. code-block:: sql

    COMMIT [WORK];

.. note:: CUBRID에서는 트랜잭션 처리 시 기본적으로 자동 커밋 모드로 지정된다.

자동 커밋 모드는 모든 SQL 문을 자동으로 커밋 또는 롤백하는 모드로서, 해당 SQL 문이 정상 수행되면 해당 트랜잭션을 자동 커밋하고, 오류가 발생하면 트랜잭션을 롤백한다.

이러한 자동 커밋 모드는 모든 인터페이스에서 지원되며, CCI, PHP, ODBC, OLE DB 인터페이스는 브로커 파라미터인 **CCI_DEFAULT_AUTOCOMMIT** 을 통해 응용 프로그램 시작 시의 자동 커밋 모드를 설정할 수 있다. 브로커 파라미터 설정이 생략될 경우 기본값은 **ON** 이다. CCI 인터페이스는 **cci_set_autocommit** (), PHP 인터페이스는 **cubrid_set_autocommit** () 함수를 이용하여 응용 프로그램 내에서 자동 커밋 모드 설정 여부를 변경할 수 있다. 

CSQL 인터프리터에서 자동 커밋 모드를 설정하는 세션 명령어(**;AUtocommit**)에 대해서는 :ref:`csql-session-commands` 를 참조한다.

트랜잭션 롤백
-------------

**ROLLBACK WORK** 문은 마지막 트랜잭션 이후의 모든 데이터베이스의 갱신을 제거한다. **WORK** 키워드는 생략 가능하다. 이것은 데이터베이스에 영구적으로 입력하기 전에 부정확하고 불필요한 갱신을 무효화할 수 있다. 트랜잭션 동안 획득한 모든 잠금은 해제된다. ::

    ROLLBACK [WORK];

다음 예제는 동일한 테이블의 정의와 행을 수정하는 두 개의 명령을 보여주고 있다.

.. code-block:: sql

    -- csql> ;autocommit off
    CREATE TABLE code2 (
        s_name  CHAR(1),
        f_name  VARCHAR(10)
    );
    COMMIT;
    
    ALTER TABLE code2 DROP s_name;
    INSERT INTO code2 (s_name, f_name) VALUES ('D','Diamond');
 
::

    ERROR: s_name is not defined.

*code* 테이블의 정의에서 *s_name* 칼럼이 이미 제거되었기 때문에 **INSERT** 문의 실행은 실패한다. *code* 테이블에 입력하려고 했던 데이터는 틀리지 않으나 테이블에서 칼럼이 잘못 제거되었다. 이 시점에서 *code* 테이블의 원래 정의를 복원하기 위해서 **ROLLBACK WORK** 문을 사용할 수 있다.

.. code-block:: sql

    ROLLBACK WORK;

이후에 **ALTER CLASS** 명령을 다시 입력하여 *s_name* 칼럼을 제거하며, **INSERT** 문을 수정한다. 트랜잭션이 중단되었기 때문에 **INSERT** 명령은 다시 입력되어야 한다. 데이터베이스 갱신이 의도한 대로 이루어졌으면 변경을 영구화하기 위해 트랜잭션을 커밋한다.

.. code-block:: sql

    ALTER TABLE code2 DROP s_name;
    INSERT INTO code2 (f_name) VALUES ('Diamond');

    COMMIT WORK;

세이브포인트와 부분 롤백
------------------------

세이브포인트(savepoint)는 트랜잭션이 진행되는 중에 수립되는데, 트랜잭션에 의해 수행되는 데이터베이스 갱신을 세이브포인트 지점까지만 롤백할 수 있도록 하기 위해서이다. 이 연산을 부분 롤백(partial rollback)이라고 부른다. 부분 롤백에서는 세이브포인트 이후의 데이터베이스 연산(삽입, 삭제, 갱신 등)은 하지 않은 것으로 되고 세이브포인트 지점을 포함하여 앞서 진행된 트랜잭션의 연산은 그대로 유지된다. 부분 롤백이 실행된 후에 트랜잭션은 다른 연산을 계속 진행할 수 있다. 또는 **COMMIT WORK** 문이나 **ROLLBACK WORK** 문으로 트랜잭션을 끝낼 수도 있다. 세이브포인트는 트랜잭션에서 수행된 갱신을 커밋하는 것이 아님을 명심해야 한다.

세이브포인트는 트랜잭션의 어느 시점에서도 만들 수 있고 몇 개의 세이브포인트라도 어떤 주어진 시점에 사용될 수 있다. 특정 세이브포인트보다 앞선 세이브포인트로 부분 롤백이 수행되거나 **COMMIT WORK** 또는 **ROLLBACK WORK** 문으로 트랜잭션이 끝나면 특정 세이브포인트는 제거된다. 특정 세이브포인트 이후에 대한 부분 롤백은 여러 번 수행될 수 있다.

세이브포인트는 길고 복잡한 프로그램을 통제할 수 있도록 중간 단계를 만들고 이름을 붙일 수 있기 때문에 유용하다. 예를 들어, 많은 갱신 연산 수행 시 세이브포인트를 사용하면 실수를 했을 때 모든 문장을 다시 수행할 필요가 없다. ::

    SAVEPOINT <mark>;

    <mark>:
    - a SQL identifier
    - a host variable (starting with :)

같은 트랜잭션 내에 여러 개의 세이브포인트를 지정할 때 *mark* 를 같은 값으로 하면 마지막 세이브포인트만 부분 롤백에 나타난다. 그리고 앞의 세이브포인트는 제일 마지막 세이브포인트로 부분 롤백할 때까지 감춰졌다가 제일 마지막 세이브포인트가 사용된 후 없어지면 나타난다. ::

    ROLLBACK [WORK] [TO [SAVEPOINT] <mark> ;

    <mark>:
    - a SQL identifier
    - a host variable (starting with :)

앞에서는 **ROLLBACK WORK** 문이 마지막 트랜잭션 이후로 입력된 모든 데이터베이스의 갱신을 제거하였다. **ROLLBACK WORK** 문은 특정 세이브포인트 이후로 트랜잭션의 갱신을 되돌리는 부분 롤백에도 사용된다.

*mark* 의 값이 주어지지 않으면 트랜잭션은 모든 갱신을 취소하면서 종료한다. 여기에는 트랜잭션에 만들어진 모든 세이브포인트도 포함한다. *mark* 가 주어지면 지정한 세이브포인트 이후의 것은 취소되고, 세이브포인트를 포함한 이전의 것은 갱신 사항이 남는다.

다음 예제는 트랜잭션의 일부를 롤백하는 방법을 보여준다.
먼저 savepoint *SP1*, *SP2* 를 설정한다.

.. code-block:: sql

    -- csql> ;autocommit off
    
    CREATE TABLE athlete2 (name VARCHAR(40), gender CHAR(1), nation_code CHAR(3), event VARCHAR(30));
    INSERT INTO athlete2(name, gender, nation_code, event)
    VALUES ('Lim Kye-Sook', 'W', 'KOR', 'Hockey');
    SAVEPOINT SP1;
     
    SELECT * from athlete2;
    INSERT INTO athlete2(name, gender, nation_code, event)
    VALUES ('Lim Jin-Suk', 'M', 'KOR', 'Handball');
     
    SELECT * FROM athlete2;
    SAVEPOINT SP2;
     
    RENAME TABLE athlete2 AS sportsman;
    SELECT * FROM sportsman;
    ROLLBACK WORK TO SP2;

위에서 *athlete2* 테이블의 이름 변경은 위의 부분 롤백에 의해서 롤백된다. 다음의 문장은 원래의 이름으로 질의를 수행하여 이것을 검증하고 있다.

.. code-block:: sql

    SELECT * FROM athlete2;
    DELETE FROM athlete2 WHERE name = 'Lim Jin-Suk';
    SELECT * FROM athlete2;
    ROLLBACK WORK TO SP2;

위에서 'Lim Jin-Suk' 을 삭제한 것은 이후에 진행되는 rollback work to *SP2* 명령문에 의해서 취소되었다.
다음은 *SP1* 으로 롤백하는 경우이다.

.. code-block:: sql

    SELECT * FROM athlete2;
    ROLLBACK WORK TO SP1;
    SELECT * FROM athlete2;
    COMMIT WORK;

.. _cursor-holding:

커서 유지
=========

응용 프로그램이 명시적인 커밋 혹은 자동 커밋 이후에도 **SELECT** 질의 결과의 레코드셋을 유지하여 다음 레코드를 읽을(fetch) 수 있도록 하는 것을 커서 유지(cursor holdability)라고 한다. 각 응용 프로그램에서 연결 수준(connection level) 또는 문장 수준(statement level)으로 커서 유지 기능을 설정할 수 있으며, 설정을 명시하지 않으면 기본으로 커서가 유지된다.

다음 코드는 JDBC에서 커서 유지를 설정하는 예이다.

.. code-block:: java

    // set cursor holdability at the connection level
    conn.setHoldability(ResultSet.HOLD_CURSORS_OVER_COMMIT);
     
    // set cursor holdability at the statement level which can override the connection
    PreparedStatement pStmt = conn.prepareStatement(sql,
                                        ResultSet.TYPE_SCROLL_SENSITIVE,
                                        ResultSet.CONCUR_UPDATABLE,
                                        ResultSet.HOLD_CURSORS_OVER_COMMIT);

커밋 시점에 커서를 유지하지 않고 커서를 닫도록 설정하고 싶으면, 위의 예제에서 **ResultSet.HOLD_CURSORS_OVER_COMMIT** 대신 **ResultSet.CLOSE_CURSORS_AT_COMMIT** 를 설정한다.

CCI 로 개발된 응용 프로그램 역시 커서 유지가 기본 동작이며, 연결 수준에서 커서를 유지하지 않도록 설정한 경우 질의를 prepare할 때 **CCI_PREPARE_HOLDABLE** 플래그를 명시하면 해당 질의 수준에서 커서를 유지한다. CCI로 개발된 드라이버(PHP, PDO, ODBC, OLE DB, ADO.NET, Perl, Python, Ruby) 역시 커서 유지가 기본 동작이며, 커서 유지 여부의 설정을 지원하는지에 대해서는 해당 드라이버의 **PREPARE** 함수를 참고한다.

.. note::

    *   CUBRID 9.0 미만 버전까지는 커서 유지를 지원하지 않으며, 커밋이 발생하면 커서가 자동으로 닫히는 것이 기본 동작이다.
    *   CUBRID는 현재 java.sql.XAConnection 인터페이스에서 ResultSet.HOLD_CURSORS_OVER_COMMIT을 지원하지 않는다.

**트랜잭션 종료 시의 커서 관련 동작**

트랜잭션이 커밋되면 커서 유지로 설정되어 있더라도 모든 잠금은 해제된다.

트랜잭션이 롤백되면 결과 셋이 닫힌다. 이것은 커서 유지가 설정되어 현재 트랜잭션에서 유지되던 결과 셋이 닫힌다는 것을 의미한다.

.. code-block:: java

    rs1 = stmt.executeQuery(sql1);
    conn.commit();
    rs2 = stmt.executeQuery(sql2);
    conn.rollback();  // 결과 셋 rs2와 rs1이 닫히게 되어 둘 다 사용하지 못하게 됨.

**결과 셋이 종료되는 경우**

커서가 유지되는 결과 셋은 다음의 경우에 닫힌다.

*   드라이버에서 결과 셋을 닫는 경우(예: rs.close() 등)
*   드라이버에서 statement를 닫는 경우(예: stmt.close() 등)
*   드라이버 연결 종료
*   트랜잭션을 롤백하는 경우(예: 자동 커밋 OFF 모드에서 사용자의 명시적인 롤백 호출, 자동 커밋 ON 모드에서 질의 실행 오류 발생 등)

**CAS와의 관계**

응용 프로그램에서 커서 유지로 설정되어 있다고 해도 응용 프로그램과 CAS와의 연결이 끊기면 결과 셋은 자동으로 닫힌다. 브로커 파라미터인 **KEEP_CONNECTION** 의 설정 값은 결과 셋의 커서 유지에 영향을 미친다.

*   KEEP_CONNECTION = ON: 커서 유지에 영향을 주지 않음.
*   KEEP_CONNECTION = AUTO: 커서 유지되는 결과 셋이 열려 있는 동안 CAS가 재시작될 수 없음.

.. warning:: 결과 셋을 닫지 않은 상태로 유지하는 만큼 메모리 사용량이 늘어날 수 있으므로 사용을 마친 결과 셋은 반드시 닫아야 한다.

.. note:: CUBRID 9.0 미만 버전까지는 커서 유지를 지원하지 않으며, 커밋이 발생하면 커서가 자동으로 닫힌다. 즉, **SELECT** 질의 결과의 레코드셋을 유지하지 않는다.


.. _database-concurrency:

데이터베이스 동시성
===================

다수의 사용자들이 데이터베이스에서 읽고 쓰는 권한을 가질 때, 한 명 이상의 사용자가 동시에 같은 데이터에 접근할 가능성이 있다. 데이터베이스의 무결성을 보호하고, 사용자와 트랜잭션이 항상 정확하고 일관된 데이터를 지니기 위해서는 다중 사용자 환경에서의 접근과 갱신에 대한 통제가 필수적이다. 적정한 통제가 없으면 데이터는 어긋난 순서로 부정확하게 갱신될 수 있다.

트랜잭션은 데이터베이스 동시성을 보장해야하며 각 트랜잭션은 적절한 결과를 보장해야한다. 한 번에 여러 트랜잭션이 실행될 때 트랜잭션 *T1* 의 이벤트가 트랜잭션 *T2* 의 이벤트에 영향을 미치지 않아야 한다. 이것은 격리를 의미한다. 트랜잭션 격리 수준은 트랜잭션이 다른 모든 동시 트랜잭션과 분리되는 정도이다. 격리 수준이 높으면 다른 트랜잭션의 간섭이 적음을 의미한다. 격리 수준이 낮으면 동시성이 높다는 것을 의미한다. 데이터베이스는 격리 레벨에 따라 어떤 잠금(lock)이 테이블과 레코드에 적용할 것인지 판별한다. 따라서 적절한 격리 수준을 설정하여 서비스 고유의 일관성 및 동시성 수준을 제어 할 수 있다.

트랜잭션 격리 수준 설정을 통해 트랜잭션 간 간섭을 허용할 수 있는 읽기 연산의 종류는 다음과 같다.

*   **더티 읽기 (Dirty read)** : 트랜잭션 *T1* 이 데이터 *D* 를 *D'* 으로 갱신한 후 커밋을 수행하기 전에 트랜잭션 *T2* 가 *D'* 을 읽을 수 있다.
*   **반복할 수 없는 읽기 (Non-repeatable read)** : 트랜잭션 *T1* 이 데이터를 반복 조회하는 중에 다른 트랜잭션 *T2* 가 데이터를 갱신 혹은 삭제하고 커밋하는 경우, 트랜잭션 *T1* 은 수정된 값을 읽을 수 있다.
*   **유령 읽기 (Phantom read)** : 트랜잭션 *T1* 에서 데이터를 여러 번 조회하는 중에 다른 트랜잭션 *T2* 가 새로운 레코드 *E* 를 삽입하고 커밋한 경우, 트랜잭션 *T1* 은 *E* 를 읽을 수 있다.

이러한 간섭을 기반으로 SQL 표준은 트랜잭션 격리 수준을 네 가지로 정의한다.

*   **READ UNCOMMITTED** 는 더티 읽기(dirty read), 반복할 수 없는 읽기(unrepeatable read), 유령 읽기(phantom read)를 허용한다.
*   **READ COMMITTED** 는 더티 읽기를 허용하지 않으며 반복할 수 없는 읽기와 유령 읽기를 허용한다.
*   **REPEATABLE READ** 는 더티 읽기와 반복할 수 없는 읽기를 허용하지 않으며 유령 읽기를 허용한다.
*   **SERIALIZABLE** 은 읽기 연산 시 트랜잭션 간 간섭을 허용하지 않는다.


**CUBRID가 제공하는 격리 수준**

아래 표에서 격리 수준 옆에 있는 괄호 안의 숫자는 격리 수준을 설정할 때 격리 수준 명칭 대신 사용할 수 있는 번호이다.

사용자는 :ref:`set-transaction-isolation-level` 문을 사용하거나 CUBRID가 지원하는 동시성/잠금 파라미터를 이용하여 격리 수준을 설정할 수 있는데, 이에 관한 설명은 :ref:`lock-parameters`\ 를 참조한다.

(O: YES, X: NO)

+--------------------------------+--------+-----------+--------+----------------------+
| CUBRID 격리 수준               | 더티   | 반복할 수 | 유령   | 조회 중인 테이블에   |
| (isolation_level)              | 읽기   | 없는 읽기 | 읽기   | 대한 스키마 갱신     |
+================================+========+===========+========+======================+
| :ref:`isolation-level-6` (6)   | X      | X         | X      | X                    |
+--------------------------------+--------+-----------+--------+----------------------+
| :ref:`isolation-level-5` (5)   | X      | X         | X      | X                    |
+--------------------------------+--------+-----------+--------+----------------------+
| :ref:`isolation-level-4` (4)   | X      | O         | O      | X                    |
+--------------------------------+--------+-----------+--------+----------------------+

CUBRID 격리 수준의 기본값은 :ref:`isolation-level-4` 이다.

.. _mvcc-snapshot:

다중 버전 동시성 제어(Multiversion Concurrency Control)
=======================================================

이전 CUBRID는 잘 알려진 2단계 잠금 프로토콜을 사용하여 격리 수준을 관리했다. 이 프로토콜에서는 동시에 연산 충돌이 발생하지 않도록 트랜잭션이 객체를 읽기 전에 공유 잠금을 획득하고, 갱신하기 전에 배타 잠금을 획득한다. 트랜잭션 *T1* 에 잠금이 필요한 경우 시스템에서 요청된 잠금이 기존 잠금과 충돌하는지 확인한다. 충돌이 발생하면 트랜잭션 *T1* 은 대기 상태가 되고 잠금이 지연된다. 다른 트랜잭션 *T2* 가 잠금을 해제하면 트랜잭션 *T1* 이 다시 시작되어 잠금을 획득한다. 잠금이 해제되면 해당 트랜잭션에서 새로운 잠금을 획득할 필요가 없다.

CUBRID 10.0에서는 2단계 잠금 프로토콜이 Multiversion Concurrency Control(MVCC) 프로토콜로 대체되었다. 2단계 잠금 프로토콜과 달리, MVCC는 동시 트랜잭션에서 수정 중인 객체에 액세스하여 읽는 것을 허용한다. MVCC는 행을 중복하여 갱신될 때마다 여러 버전을 생성한다. 주로 데이터베이스에서 읽기 연산이 많은 시나리오에 대해서는 읽기 연산을 허용하는 것이 중요하다. 객체를 갱신하기 전에는 여전히 배타 잠금이 필요하다.

MVCC는 데이터베이스의 일관된 시점을 제공하며, 특히 다른 동시성 방법보다 적은 성능 비용으로 진정한 **snapshot isolation** 을 구현할 수 있다.

버전 관리, 가시성 및 스냅샷
---------------------------

MVCC는 각 데이터베이스 행에 대해 여러 버전을 유지한다. 각 버전마다 MVCCID(쓰기 트랜잭션에 대한 고유 식별자)로 삽입자(inserter) 및 삭제자(deleter)가 표시된다. 이러한 마커(Marker)는 변경한 사용자를 파악하고 타임라인에 변경 사항을 표시하는 데 유용하다.

트랜잭션 *T1* 이 새로운 행을 삽입하면 첫 번째 버전이 생성되고 고유 식별자 *MVCCID1* 이 삽입 ID로 설정된다. MVCCID는 레코드 헤더에 메타데이터로 저장된다.

+------------------+-------------+---------------+
| OTHER META-DATA  | MVCCID1     | RECORD DATA   |
+------------------+-------------+---------------+

*T1* 이 커밋할 때까지 다른 트랜잭션에서는 *T1* 이 삽입한 행을 볼 수 없어야 한다. MVCCID는 데이터베이스 변경 사항의 작성자를 식별하고 타임 라인에 배치하여 다른 트랜잭션이 변경 사항의 유효성을 알 수 있도록 한다. 이 경우 이 행을 검사하는 모든 트랜잭션은 *MVCCID1* 을 찾고 소유자가 여전히 활성 상태이므로 이 행을 볼 수 없다.

*T1* 이 커밋 된 후 새로운 트랜잭션 *T2* 가 행을 찾아 제거한다. *T2* 는 배타 잠금을 획득하는 대신, 다른 트랜잭션이 액세스할 수 있도록 해당 버전을 제거하지 않은 상태로 두고, 다른 트랜잭션이 변경할 수 없도록 버전을 삭제됨으로 표시한다.  다른 MVCCID를 추가하여 다른 트랜잭션이 삭제자를 식별 할 수 있도록 한다.

+------------------+-------------+---------------+---------------+
| OTHER META-DATA  | MVCCID1     | MVCCID2       | RECORD DATA   |
+------------------+-------------+---------------+---------------+

*T2* 가 레코드 값 중 하나를 갱신하기로 결정하면 행을 신규 버전으로 갱신하고 이전 버전을 로그에 저장해야 한다. 새로운 행은 새로운 데이터, 트랜잭션 MVCCID(삽입 MVCCID) 및 이전 버전이 저장된 로그 엔트리의 주소로 구성된다. 행에 표시되는 내용은 다음과 같다.

*HEAP* 파일은 OID에 의해 식별되는 단일 행을 포함한다.

+------------------+-------------+--------------------+---------------+
| OTHER META-DATA  | MVCCID_INS1 | PREV_VERSION_LSA1  |  RECORD DATA  |
+------------------+-------------+--------------------+---------------+

LOG 파일에는 로그 엔트리 체인이 있고, 각 로그 엔트리의 언두 부분은 수정되기 전의 힙 레코드를 포함한다.

+----------------------+------------------+-------------+--------------------+---------------+
| LOG ENTRY META-DATA  | OTHER META-DATA  | MVCCID_INS2 | PREV_VERSION_LSA2  |  RECORD DATA  |
+----------------------+------------------+-------------+--------------------+---------------+

+----------------------+------------------+-------------+--------------------+---------------+
| LOG ENTRY META-DATA  | OTHER META-DATA  | MVCCID_INS3 | NULL               |  RECORD DATA  |
+----------------------+------------------+-------------+--------------------+---------------+

다른 트랜잭션은 각 레코드의 삽입 및 삭제 MVCCID 값에 따라 결정되는 가시성 조건을 만족하는 레코드가 나올 때까지 다중 로그 레코드의 이전 버전 LSA의 로그 체인을 조사할 필요가 있다.

    .. note::

         *   예전 버전(10.0)에서는 갱신된 행의 이전 및 새 버전을 저장하는 데 힙(다른 OID)이 사용되었다. 실제로 이전 버전은 변경되지 않은 행이었으며, 여기에는 새 버전에 대한 OID 링크가 추가되었다. 새 버전과 이전 버전이 둘 다 힙에 저장되었다.

*T2* 만 갱신된 행을 볼 수 있으며, 다른 트랜잭션은 힙 행에서 획득한 LSA를 통해 로그 페이지에 있는 행 버전에 액세스할 수 있다. 실행 중인 트랜잭션에 의해 보이거나 보이지 않는 버전 속성을 **가시성 (visibility)** 이라 한다. 가시성 속성은 각 트랜잭션과 관련이 있고, 일부 트랜잭션은 이를 true로 간주하지만, 나머지 트랜잭션은 false로 간주할 수 있다.

*T2* 가 행 갱신을 수행한 후 커밋하기 전, 트랜잭션 *T3* 가 시작한 경우 *T2* 가 커밋한 후에도 *T3* 는 새 버전을 볼 수 없다. *T3* 의 버전 가시성은 *T3* 가 시작할 때 삽입자 및 삭제자의 상태에 따라 결정되며, *T3* 의 트랜잭션이 수행되는 동안 해당 상태가 유지된다.

사실상, 트랜잭션에 대한 모든 버전의 가시성은 트랜잭션이 시작된 이후에 발생하는 변경 사항의 영향을 받지 않는다. 또한 새로 추가되는 버전도 무시된다.  결과적으로 트랜잭션에서 보여진 버전 셋은 변경되지 않고 트랜잭션의 스냅샷으로 구성된다. 따라서 MVCC에 의해 제공된 **snapshot isolation** 는 트랜잭션의 모든 읽기 질의에 대해 일관된 뷰를 보장한다.

CUBRID에서 **스냅샷 (snapshot)** 은 모든 유효하지 않은 MVCCID의 필터이다.  스냅샷이 만들어지기 전에 커밋되지 않으면 MVCCID는 유효하지 않다. 새 트랜잭션을 시작할 때마다 스냅샷 필터가 갱신되지 않기 위해 두 경계(가장 낮은 활성 MVCCID와 가장 높은 커밋 MVCCID)를 통해 스냅샷이 정의된다. 이 경계 안에 있는 활성 MVCCID 값 목록만 저장된다. 스냅샷 이후 시작된 트랜잭션은 가장 높은 커밋 MVCCID보다 큰 MVCCID를 가지므로 자동으로 무효화된다. 가장 낮은 활성 MVCCID보다 낮은 MVCCID는 이미 커밋되었으므로 자동으로 유효하다.

버전 가시성을 결정하는 스냅샷 필터 알고리즘은 삽입 및 삭제에 사용되는 MVCCID 마커를 사용한다. 스냅샷은 힙에 저장된 마지막 버전을 검사하고 결과에 따라 힙에서 버전을 가져오거나 로그에서 이전 버전을 가져올 수 있으며 행을 무시할 수 도 있다.

+--------------------+--------------------------+---------------------+--------------------------------------------------------+
| 삽입MVCCID         | 이전 버전 LSA            | 삭제 MVCCID         | 스냅샷 테스트 결과                                     |
+====================+==========================+=====================+========================================================+
| Not visible        | NULL                     | None 또는           | | 버전이 *최신* 이며 가시적이지 않다.                  |
|                    |                          | not visible         | | 이전 버전이 없으므로 행이 무시된다.                  |
|                    +--------------------------+---------------------+--------------------------------------------------------+
|                    | LSA                      | None 또는           | | 버전이 *최신* 이며 가시적이지 않다.                  |
|                    |                          | not visible         | | 이전 버전이 있어 스냅샷은 행의 LSA를 확인해야한다.   |
+--------------------+--------------------------+---------------------+--------------------------------------------------------+
| None or visible    | LSA or NULL              | None 또는           | | 버전이 가시적이며 행의 데이터가 패치(fetch)된다.     |
|                    |                          | not visible         | | 이전 버전이 있는지 여부는 중요하지 않다.             |
|                    |                          +---------------------+--------------------------------------------------------+
|                    |                          | Visible             | | 버전이 너무 오래되고, 삭제되어 가시적이지 않다.      |
|                    |                          |                     | | 이전 버전이 있는지 여부는 중요하지 않다.             |
+--------------------+--------------------------+---------------------+--------------------------------------------------------+

버전이 최신이지만 로그에 저장된 이전 버전이 있는 경우 이전 버전에서도 동일한 확인 과정이 반복된다. 더 이상 이전 버전이 없거나(해당 트랜잭션에 대한 행 체인 전부가 최신인 경우) 가시적인 버전을 발견하면 확인이 중지된다.

스냅샷의 작동 원리는 다음과 같다(전체 트랜잭션에서 동일한 스냅샷을 유지하기 위해 **REPEATABLE READ** 격리 수준 사용).

**예제 1: 새로운 행 삽입**

+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| session 1                                                         | session 2                                                                         |
+===================================================================+===================================================================================+
| .. code-block:: sql                                               | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|   csql> ;autocommit off                                           |   csql> ;autocommit off                                                           |
|                                                                   |                                                                                   |
|   AUTOCOMMIT IS OFF                                               |   AUTOCOMMIT IS OFF                                                               |
|                                                                   |                                                                                   |
|   csql> set transaction isolation level REPEATABLE READ;          |   csql> set transaction isolation level REPEATABLE READ;                          |
|                                                                   |                                                                                   |
|   Isolation level set to:                                         |   Isolation level set to:                                                         |
|   REPEATABLE READ                                                 |   REPEATABLE READ                                                                 |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   csql> CREATE TABLE tbl(host_year integer, nation_code char(3)); |                                                                                   |
|   csql> COMMIT WORK;                                              |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   -- insert a row without committing                              |                                                                                   |
|   csql> INSERT INTO tbl VALUES (2008, 'AUS');                     |                                                                                   |
|                                                                   |                                                                                   |
|   -- current transaction sees its own changes                     |                                                                                   |
|   csql> SELECT * FROM tbl;                                        |                                                                                   |
|                                                                   |                                                                                   |
|       host_year  nation_code                                      |                                                                                   |
|   ===================================                             |                                                                                   |
|            2008  'AUS'                                            |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                   | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|                                                                   |   -- this snapshot should not see uncommitted row                                 |
|                                                                   |   csql> SELECT * FROM tbl;                                                        |
|                                                                   |                                                                                   |
|                                                                   |   There are no results.                                                           |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   csql> COMMIT WORK;                                              |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                   | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|                                                                   |   -- even though inserter did commit, this snapshot still can't see the row       |
|                                                                   |   csql> SELECT * FROM tbl;                                                        |
|                                                                   |                                                                                   |
|                                                                   |   There are no results.                                                           |
|                                                                   |                                                                                   |
|                                                                   |   -- commit to start a new transaction with a new snapshot                        |
|                                                                   |   csql> COMMIT WORK;                                                              |
|                                                                   |                                                                                   |
|                                                                   |   -- the new snapshot should see committed row                                    |
|                                                                   |   csql> SELECT * FROM tbl;                                                        |
|                                                                   |                                                                                   |
|                                                                   |       host_year  nation_code                                                      |
|                                                                   |   ===================================                                             |
|                                                                   |            2008  'AUS'                                                            |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+

**예제 2: 행 삭제**

+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| session 1                                                         | session 2                                                                         |
+===================================================================+===================================================================================+
| .. code-block:: sql                                               | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|   csql> ;autocommit off                                           |   csql> ;autocommit off                                                           |
|                                                                   |                                                                                   |
|   AUTOCOMMIT IS OFF                                               |   AUTOCOMMIT IS OFF                                                               |
|                                                                   |                                                                                   |
|   csql> set transaction isolation level REPEATABLE READ;          |   csql> set transaction isolation level REPEATABLE READ;                          |
|                                                                   |                                                                                   |
|   Isolation level set to:                                         |   Isolation level set to:                                                         |
|   REPEATABLE READ                                                 |   REPEATABLE READ                                                                 |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   csql> CREATE TABLE tbl(host_year integer, nation_code char(3)); |                                                                                   |
|   csql> INSERT INTO tbl VALUES (2008, 'AUS');                     |                                                                                   |
|   csql> COMMIT WORK;                                              |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   -- delete the row without committing                            |                                                                                   |
|   csql> DELETE FROM tbl WHERE nation_code = 'AUS';                |                                                                                   |
|                                                                   |                                                                                   |
|   -- this transaction sees its own changes                        |                                                                                   |
|   csql> SELECT * FROM tbl;                                        |                                                                                   |
|                                                                   |                                                                                   |
|   There are no results.                                           |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                   | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|                                                                   |   -- delete was not committed, so the row is visible to this snapshot             |
|                                                                   |   csql> SELECT * FROM tbl;                                                        |
|                                                                   |                                                                                   |
|                                                                   |       host_year  nation_code                                                      |
|                                                                   |   ===================================                                             |
|                                                                   |            2008  'AUS'                                                            |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   csql> COMMIT WORK;                                              |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                   | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|                                                                   |   -- delete was committed, but the row is still visible to this snapshot          |
|                                                                   |   csql> SELECT * FROM tbl;                                                        |
|                                                                   |                                                                                   |
|                                                                   |       host_year  nation_code                                                      |
|                                                                   |   ===================================                                             |
|                                                                   |            2008  'AUS'                                                            |
|                                                                   |                                                                                   |
|                                                                   |   -- commit to start a new transaction with a new snapshot                        |
|                                                                   |   csql> COMMIT WORK;                                                              |
|                                                                   |                                                                                   |
|                                                                   |   -- the new snapshot can no longer see deleted row                               |
|                                                                   |   csql> SELECT * FROM tbl;                                                        |
|                                                                   |                                                                                   |
|                                                                   |   There are no results.                                                           |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+

**예제 3: 행 갱신**

+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| session 1                                                         | session 2                                                                         |
+===================================================================+===================================================================================+
| .. code-block:: sql                                               | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|   csql> ;autocommit off                                           |   csql> ;autocommit off                                                           |
|                                                                   |                                                                                   |
|   AUTOCOMMIT IS OFF                                               |   AUTOCOMMIT IS OFF                                                               |
|                                                                   |                                                                                   |
|   csql> set transaction isolation level REPEATABLE READ;          |   csql> set transaction isolation level REPEATABLE READ;                          |
|                                                                   |                                                                                   |
|   Isolation level set to:                                         |   Isolation level set to:                                                         |
|   REPEATABLE READ                                                 |   REPEATABLE READ                                                                 |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   csql> CREATE TABLE tbl(host_year integer, nation_code char(3)); |                                                                                   |
|   csql> INSERT INTO tbl VALUES (2008, 'AUS');                     |                                                                                   |
|   csql> COMMIT WORK;                                              |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   -- delete the row without committing                            |                                                                                   |
|   csql> UPDATE tbl SET host_year = 2012 WHERE nation_code = 'AUS';|                                                                                   |
|                                                                   |                                                                                   |
|   -- this transaction sees new version, host_year = 2012          |                                                                                   |
|   csql> SELECT * FROM tbl;                                        |                                                                                   |
|                                                                   |                                                                                   |
|       host_year  nation_code                                      |                                                                                   |
|   ===================================                             |                                                                                   |
|            2012  'AUS'                                            |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                   | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|                                                                   |   -- update was not committed, so this snapshot sees old version                  |
|                                                                   |   csql> SELECT * FROM tbl;                                                        |
|                                                                   |                                                                                   |
|                                                                   |       host_year  nation_code                                                      |
|                                                                   |   ===================================                                             |
|                                                                   |            2008  'AUS'                                                            |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   csql> COMMIT WORK;                                              |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                   | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|                                                                   |   -- update was committed, but this snapshot still sees old version               |
|                                                                   |   csql> SELECT * FROM tbl;                                                        |
|                                                                   |                                                                                   |
|                                                                   |       host_year  nation_code                                                      |
|                                                                   |   ===================================                                             |
|                                                                   |            2008  'AUS'                                                            |
|                                                                   |                                                                                   |
|                                                                   |   -- commit to start a new transaction with a new snapshot                        |
|                                                                   |   csql> COMMIT WORK;                                                              |
|                                                                   |                                                                                   |
|                                                                   |   -- the new snapshot can see new version, host_year = 2012                       |
|                                                                   |   csql> SELECT * FROM tbl;                                                        |
|                                                                   |                                                                                   |
|                                                                   |       host_year  nation_code                                                      |
|                                                                   |   ===================================                                             |
|                                                                   |            2012  'AUS'                                                            |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+

**예제 4: 다양한 트랜잭션에서 각각 다른 버전이 보임**

+-------------------------------------------------------------------+----------------------------------------+----------------------------------------+
| session 1                                                         | session 2                              | session 3                              |
+===================================================================+========================================+========================================+
| .. code-block:: sql                                               | ..  code-block:: sql                   | .. code-block:: sql                    |
|                                                                   |                                        |                                        |
|   csql> ;autocommit off                                           |   csql> ;autocommit off                |   csql> ;autocommit off                |
|                                                                   |                                        |                                        |
|   AUTOCOMMIT IS OFF                                               |   AUTOCOMMIT IS OFF                    |   AUTOCOMMIT IS OFF                    |
|                                                                   |                                        |                                        |
|   csql> set transaction isolation level REPEATABLE READ;          |   csql> set transaction isolation      |   csql> set transaction isolation      |
|                                                                   |   level REPEATABLE READ;               |   level REPEATABLE READ;               |
|                                                                   |                                        |                                        |
|   Isolation level set to:                                         |   Isolation level set to:              |   Isolation level set to:              |
|   REPEATABLE READ                                                 |   REPEATABLE READ                      |   REPEATABLE READ                      |
|                                                                   |                                        |                                        |
+-------------------------------------------------------------------+----------------------------------------+----------------------------------------+
| .. code-block:: sql                                               |                                        |                                        |
|                                                                   |                                        |                                        |
|   csql> CREATE TABLE tbl(host_year integer, nation_code char(3)); |                                        |                                        |
|   csql> INSERT INTO tbl VALUES (2008, 'AUS');                     |                                        |                                        |
|   csql> COMMIT WORK;                                              |                                        |                                        |
|                                                                   |                                        |                                        |
+-------------------------------------------------------------------+----------------------------------------+----------------------------------------+
| .. code-block:: sql                                               | ..  code-block:: sql                   |                                        |
|                                                                   |                                        |                                        |
|   -- update row                                                   |                                        |                                        |
|   csql> UPDATE tbl SET host_year = 2012 WHERE nation_code = 'AUS';|                                        |                                        |
|                                                                   |                                        |                                        |
|   csql> SELECT * FROM tbl;                                        |   csql> SELECT * FROM tbl;             |                                        |
|                                                                   |                                        |                                        |
|       host_year  nation_code                                      |       host_year  nation_code           |                                        |
|   ===================================                             |   ===================================  |                                        |
|            2012  'AUS'                                            |            2008  'AUS'                 |                                        |
|                                                                   |                                        |                                        |
+-------------------------------------------------------------------+----------------------------------------+----------------------------------------+
| .. code-block:: sql                                               |                                        |                                        |
|                                                                   |                                        |                                        |
|   csql> COMMIT WORK;                                              |                                        |                                        |
|                                                                   |                                        |                                        |
+-------------------------------------------------------------------+----------------------------------------+----------------------------------------+
| .. code-block:: sql                                               |  .. code-block:: sql                   |  .. code-block:: sql                   |
|                                                                   |                                        |                                        |
|   csql> UPDATE tbl SET host_year = 2016 WHERE nation_code = 'AUS';|                                        |                                        |
|                                                                   |                                        |                                        |
|   csql> SELECT * FROM tbl;                                        |   csql> SELECT * FROM tbl;             |   csql> SELECT * FROM tbl;             |
|                                                                   |                                        |                                        |
|       host_year  nation_code                                      |       host_year  nation_code           |       host_year  nation_code           |
|   ===================================                             |   ===================================  |   ===================================  |
|            2016  'AUS'                                            |            2008  'AUS'                 |            2012  'AUS'                 |
|                                                                   |                                        |                                        |
+-------------------------------------------------------------------+----------------------------------------+----------------------------------------+


VACUUM
------

각 갱신에 대해 새 버전을 생성하고 삭제 시 이전 버전을 유지하면 데이터베이스 크기가 무한으로 증가하여 데이터베이스에 큰 이슈가 발생할 수 있다.  따라서 이전 데이터를 제거하고 점유된 공간을 재사용하기 위한 회수(Cleanup) 시스템이 필요하다.

각 행의 버전은 다음과 같은 동일한 단계를 거친다.

  1. 새로 삽입되었으나 커밋되지 않으면, 삽입자만 볼 수 있음
  2. 커밋되었으면, 이전 트랜잭션에서는 볼 수 없으나 이후 트랜잭션에서는 볼 수 있음
  3. 삭제되었으나 커밋되지 않으면, 다른 트랜잭션에서는 볼 수 있으나 삭제자는 볼 수 없음
  4. 커밋되면, 이전 트랜잭션에서는 볼 수 있으나 이후 트랜잭션에서는 볼 수 없음
  5. 모든 활성 트랜잭션에서 볼 수 없음
  6. 데이터베이스에서 제거됨

회수 시스템의 역할은 5~6단계에서 회수할 버전을 획득하는 것이다. CUBRID에서는 이 시스템을 **VACUUM** 이라고 부른다.

**VACUUM** 시스템은 세 가지 원칙에 따라 개발되었다.

*   **VACUUM** 은 정확하고 완전해야 한다. **VACUUM** 은 일부 사용자가 계속 볼 수 있는 데이터는 제거하지 않으며 이전 데이터는 하나도 놓치지 않는다.
*   **VACUUM** 은 신중해야 한다. 회수 프로세스는 데이터베이스의 내용을 변경하기 때문에 수행 중인 트랜잭션의 동작에 간섭을 일으킬 수 있지만 이러한 가능성을 최소화해야 한다.
*   **VACUUM** 은 빠르고 효율적이어야 한다. **VACUUM** 이 너무 느리거나 지연되기 시작하면 데이터베이스 상태가 악화되어 전체 성능에 영향을 미칠 수 있다.

이러한 원칙에 따라 **VACUUM** 구현은 다음과 같은 이유로 기존 복구 로깅을 사용했다.

*   복구 로깅에는 힙과 인덱스 변경 사항에 대한 복구 데이터의 주소가 유지된다. 그래서 데이터베이스를 스캔하지 않고 **VACUUM** 이 대상으로 바로 이동할 수 있다.
*   로그 데이터의 처리는 활성 Worker의 작업에 거의 간섭을 일으키지 않는다.

MVCCID 정보를 로깅된 데이터에 추가함으로써 **VACUUM** 요구 사항에 맞게 복구 로깅을 조정했다. 로그 엔트리를 처리할 준비가 되면 MVCCID에 따라 **VACUUM** 이 결정된다. 활성 트랜잭션에서 볼 수 있는 MVCCID는 처리되지 않는다. 시간이 지나면 각 MVCCID를 사용한 변경 사항을 모두 볼 수 없게 된다.

각 트랜잭션은 활성 상태로 간주되는 가장 오래된 MVCCID를 유지한다.  활성으로 간주되는 가장 오래된 MVCCID는 모든 트랜잭션 중 가장 오래된 MVCCID에 의해 결정된다. 이 값보다 오래된 것은 보이지 않으므로 **VACUUM** 이 회수할 수 있다.

VACUUM 병렬 수행
++++++++++++++++

**VACUUM** 은 세 번째 원칙에 따라 빨라야 하며 활성 Worker보다 뒤처지면 안 된다. 시스템 작업 부하가 많은 경우 하나의 스레드에서 모든 **VACUUM** 작업을 처리할 수 없기 때문에 병렬 처리해야 한다.

병렬 처리를 수행하기 위해 로그 데이터를 고정 크기 블록으로 분할했다.  적절한 시기(최신의 MVCCID를 vacuum 처리 할 수 있음. 이는 블록에 기록된 모든 작업을 vacuum 할 수 있음을 의미함)가 되면 각 블록마다 하나의 vacuum 작업을 생성한다. vacuum 작업은 로그 블록에 있는 관련 로그 항목을 기반으로 데이터 공간을 회수하는 **VACUUM Worker** 들에 의해 선택된다. 로그 블록의 추적 및 vacuum 작업의 생성은 **VACUUM Master** 가 수행한다.

VACUUM 데이터 
+++++++++++++

로그 블록에서 수집된 데이터는 vacuum 데이터 파일에 저장된다. 생성된 vacuum 작업은 나중에 수행되므로 vacuum 작업을 수행할 수 있을 때까지 데이터가 저장되어야 하며 서버가 비정상 종료하는 경우에도 유지되어야 한다. vacuum 작업은 빠짐없이 수행되어야 한다. 서버의 비정상 종료로 인해 전혀 수행되지 않는 경우를 피하기 위해 작업이 두 번 수행되기도 한다.

작업이 성공적으로 수행된 후 처리된 로그 블록의 수집된 데이터가 제거된다.

수집된 로그 블록 데이터는 vacuum 데이터에 바로 추가되지 않는다. vacuum 시스템에서 작업 중인 스레드(로그 블록 및 수집 데이터 생성)들의 간섭을 피하기 위해 래치-프리(latch-free) 버퍼가 사용된다. **VACUUM Master** 가 주기적으로 활성화되어 버퍼에 있는 모든 내용을 vacuum 데이터로 저장하고, 이미 처리된 데이터를 제거한 후 새로운 작업(사용 가능한 경우)을 생성한다.

VACUUM 작업 
+++++++++++

VACUUM 작업 실행 단계는 다음과 같다.

  1. **로그 프리패치** : vacuum Master 또는 Worker가 작업으로 처리할 로그 페이지를 프리패치한다.
  2. **각 로그 레코드에 대해 다음 작업을 반복한다**.

    1. 로그 레코드를 **읽는다**.
    2. **삭제된 파일을 확인한다.** : 로그 레코드가 삭제된 파일을 가리키면 다음 로그 레코드로 이동한다.
    3. **인덱스 vacuum을 수행하고 힙 OID를 수집한다.**

      * 로그 레코드가 인덱스에 속해 있는 경우 바로 vacuum을 수행한다.
      * 로그 레코드가 힙에 속해 있는 경우 나중에 vacuum을 수행할 OID를 수집한다.

  3. 수집된 OID에 따라 **힙 vacuum을 수행** 한다.
  4. **작업을 완료한다.** vacuum 데이터에서 작업을 완료됨으로 표시한다.

로그 페이지 읽기를 쉽게 하고 vacuum 수행을 최적화하기 위해 여러 가지 방법이 수행되었다.

삭제된 파일 추적
++++++++++++++++

트랜잭션에서 테이블 또는 인덱스가 삭제되면 일반적으로 해당 테이블을 잠궈 다른 트랜잭션이 액세스하지 못하도록 차단한다. 그러나 활성 트랜잭션과 달리, **VACUUM** worker는 활성 트랜잭션에 대한 간섭을 최소화해야 하고, 회수할 데이터가 있는 경우 **VACUUM** 시스템은 절대로 중지되면 안되므로 잠금 시스템을 사용하지 않는다.  또한, **VACUUM** 은 회수가 필요한 데이터를 건너뛸 수 없다.  이에 따른 두 가지 결과는 다음과 같다.

  1. **VACUUM** 은 삭제자(dropper)가 커밋할 때까지 삭제된 테이블 또는 삭제된 인덱스에 속한 파일을 삭제하지 않는다. 트랜잭션에서 테이블을 삭제한 경우에도 해당 파일이 즉시 삭제되지 않고 계속 액세스 할 수 있다. 실질적인 삭제는 커밋한 이후로 연기된다.
  2. 실제 파일이 삭제되기 전에 **VACUUM** 시스템에 알려야 한다. 삭제자(dropper)가 **VACUUM** 시스템에 알림을 보내고 확인을 기다린다. **VACUUM** 작업의 반복 주기는 매우 짧고 새롭게 삭제된 파일이 있는지 자주 확인하므로 삭제자(dropper)가 오랫동안 기다리지 않아도 된다.

파일이 삭제된 후에는 **VACUUM** 은 해당 파일에 속한 모든 로그 엔트리를 무시한다. **VACUUM** 에서 제거해도 된다고 결정할 때까지(아직 vacuum되지 않은 가장 오래된 MVCCID에 따라 결정됨) 삭제 시점이 표시된 MVCCID와 함께 파일 식별자가 영구 파일에 저장된다.

.. _lock-protocol:

잠금 프로토콜
=============

2단계 잠금 프로토콜에서는 동시에 연산 충돌이 발생하지 않도록 트랜잭션이 객체를 읽기 전에 공유 잠금을 획득하고, 갱신하기 전에 배타 잠금을 획득한다. 현재 CUBRID에서 사용하고 있는 MVCC 잠금 프로토콜은 행을 읽기 전에 공유 잠금이 필요하지 않다. 그러나 테이블 객체에 의도 공유 잠금은 해당 행을 읽을 때 계속 사용된다. 트랜잭션 *T1* 에 잠금이 필요한 경우 CUBRID에서 요청된 잠금이 기존 잠금과 충돌하는지 확인한다. 충돌이 발생하면 트랜잭션 *T1* 은 대기 상태가 되고 잠금이 지연된다. 다른 트랜잭션 *T2* 가 잠금을 해제하면 트랜잭션 *T1* 이 다시 시작되어 잠금을 획득한다. 잠금이 해제되면 해당 트랜잭션에서 새로운 잠금을 획득할 필요가 없다.

잠금의 단위
-----------

CUBRID는 잠금의 개수를 줄이기 위해서 단위 잠금(granularity locking) 프로토콜을 사용한다. 단위 잠금 프로토콜에서는 잠금 단위의 크기에 따라 계층으로 모델화되며, 행 잠금(row lock), 테이블 잠금(table lock), 데이터베이스 잠금(database lock)이 있다. 이때, 단위가 큰 잠금은 작은 단위의 잠금을 내포한다.

잠금을 설정하고 해제하는 과정에서 발생하는 성능 손실을 잠금 비용(overhead)이라고 하는데, 큰 단위보다 작은 단위의 잠금을 수행할 때 이러한 잠금 비용이 높아지고 대신 트랜잭션 동시성은 향상된다. 따라서, CUBRID는 잠금 비용과 트랜잭션 동시성을 고려하여 잠금 단위를 결정한다. 예를 들어, 한 트랜잭션이 테이블의 모든 행들을 조회하는 경우 행 단위로 잠금을 설정/해제하는 비용이 너무 높으므로 차라리 해당 테이블에 잠금을 설정한다. 이처럼 테이블에 잠금이 설정되면 트랜잭션 동시성이 저하되므로, 동시성을 보장하려면 풀 스캔(full scan)이 발생하지 않도록 적절한 인덱스를 사용해야 할 것이다.

이와 같은 잠금 관리를 위해 CUBRID는 잠금 에스컬레이션(lock escalation) 기법을 사용하여 설정 가능한 단위 잠금의 수를 제한한다. 예를 들어, 한 트랜잭션이 행 단위에서 특정 개수 이상의 잠금을 가지고 있으면 시스템은 계층적으로 상위 단위인 테이블에 대해 잠금을 요청하기 시작한다. 단, 상위 단위로 잠금 에스컬레이션을 수행하기 위해서는 어떤 트랜잭션도 상위 단위 객체에 대한 잠금을 가지고 있지 않아야 한다. 그래야만 잠금 변환에 따른 교착 상태(deadlock)를 예방할 수 있다. 이때, 작은 단위에서 허용하는 잠금 개수는 시스템 파라미터 **lock_escalation** 을 통해 설정할 수 있다.

.. _lock-mode:

잠금 모드의 종류와 호환성
-------------------------

CUBRID는 트랜잭션이 수행하고자 하는 연산의 종류에 따라 획득하고자 하는 잠금 모드를 결정하며, 다른 트랜잭션에 의해 이미 선점된 잠금 모드의 종류에 따라 잠금 공유 여부를 결정한다. 이와 같은 잠금에 대한 결정은 시스템이 자동으로 수행하며, 사용자에 의한 수동 지정은 허용되지 않는다. CUBRID의 잠금 정보를 확인하기 위해서는 **cubrid lockdb** *db_name* 명령어를 사용하며, 자세한 내용은 :ref:`lockdb` 을 참고한다.

*   **공유 잠금(shared lock, S_LOCK, MVCC 프로토콜에서는 더 이상 사용 안 함)**

    객체에 대해 읽기 연산을 수행하기 전에 획득하며, 여러 트랜잭션이 동일 객체에 대해 획득할 수 있는 잠금이다.

    트랜잭션 T1이 특정 객체에 대해 읽기 연산을 수행하기 전에 공유 잠금을 먼저 획득한다. 이때, 트랜잭션 *T2* , *T3* 은 동시에 그 객체에 대해 읽기 연산을 수행할 수 있으나 갱신 연산을 수행할 수 없다.
    
    .. note::

        *   CUBRID 10.0에서는 MVCC를 사용하므로 공유 잠금은 거의 사용되지 않는다. 현재는 내부 데이터베이스 연산에서 행 또는 인덱스 키가 수정되는 것을 방지하는 데 주로 사용된다.

*   **배타 잠금(Exclusive lock, X_LOCK)**

    객체에 대해 갱신 연산을 수행하기 전에 획득하며, 하나의 트랜잭션만 획득할 수 있는 잠금이다.

    트랜잭션 *T1* 이 특정 객체 X에 대해 갱신 연산을 수행하기 전에 배타 잠금을 먼저 획득하고, 갱신 연산을 완료하더라도 트랜잭션 *T1* 이 커밋될 때까지 배타 잠금을 해제하지 않는다. 따라서, 트랜잭션 *T2*, *T3* 은 트랜잭션 *T1* 이 배타 잠금을 해제하기 전까지는 X에 대한 읽기 연산도 수행할 수 없다.

*   **의도 잠금(내재된 잠금, Intent lock)**

    특정 단위의 객체에 걸리는 잠금을 보호하기 위하여 이 객체보다 상위 단위의 객체에 내재적으로 설정하는 잠금을 의미한다.

    예를 들어, 특정 행에 공유 잠금이 요청되면 이보다 계층적으로 상위에 있는 테이블에도 의도 공유 잠금을 함께 설정하여 다른 트랜잭션에 의해 테이블이 잠금되는 것을 예방한다. 따라서, 의도 잠금은 계층적으로 가장 낮은 단위인 행에 대해서는 설정되지 않으며, 이보다 높은 단위의 객체에 대해서만 설정된다. 의도 잠금의 종류는 다음과 같다.

    *   **의도 공유 잠금(Intent shared lock, IS_LOCK)**

        특정 행에 공유 잠금이 설정됨에 따라 상위 객체인 테이블에 의도 공유 잠금이 설정되면, 다른 트랜잭션은 칼럼을 추가하거나 테이블 이름을 변경하는 등의 테이블 스키마를 변경할 수 없고, 모든 행을 갱신하는 작업을 수행할 수 없다. 그러나 일부 행을 갱신하는 작업이나, 모든 행을 조회하는 작업은 허용된다.

    *   **의도 배타 잠금(Intent exclusive lock, IX_LOCK)** 
    
        특정 행에 배타 잠금이 설정됨에 따라 상위 객체인 테이블에 의도 배타 잠금이 설정되면, 다른 트랜잭션은 테이블 스키마를 변경할 수 없고, 모든 행을 갱신하는 작업은 물론, 모든 행을 조회하는 작업은 수행할 수 없다. 그러나, 일부 행을 갱신하는 작업은 허용된다.

    *   **공유 의도 배타 잠금(shared with intent exclusive lock, SIX_LOCK)** 
    
        계층적으로 더 낮은 모든 객체에 설정된 공유 잠금을 보호하고, 계층적으로 더 낮은 일부 객체에 대한 의도 배타 잠금을 보호하기 위하여 상위 객체에 내재적으로 설정되는 잠금이다.

        테이블에 공유 의도 배타 잠금이 설정되면, 다른 트랜잭션은 테이블 스키마를 변경할 수 없고, 모든 행/일부 행을 갱신할 수 없으며, 모든 행을 조회할 수 없다. 그러나, 일부 행을 조회하는 작업은 허용된다.

*   **스키마 잠금**
    
    DDL 작업을 수행할 때 스키마 잠금을 획득한다.
    
    *   **스키마 안정 잠금(schema stability lock, SCH_S_LOCK)**

        질의 컴파일을 수행하는 동안 획득되며 질의에 포함된 스키마가 다른 트랜잭션에 의해 수정되지 않음을 보장한다. 

    *   **스키마 수정 잠금(schema modification lock, SCH_M_LOCK)**

        DDL(**ALTER**/**CREATE**/**DROP**)을 실행하는 동안 획득되며 다른 트랜잭션이 수정된 스키마에 접근하는 것을 방지한다.

    **ALTER**, **CREATE INDEX** 등 일부 DDL 연산은 **SCH_M_LOCK** 을 직접 획득하지 않는다. 예를 들어 필터링된 인덱스를 생성할 때, CUBRID는 필터링 표현식에 대한 타입 검사를 수행한다. 이 기간 동안, 대상 테이블에 유지되는 잠금은 다른 타입 검사 연산의 경우처럼 **SCH_S_LOCK** 이다. 그런 다음 잠금이 **SIX_LOCK** 으로 업그레이드되고(다른 트랜잭션이 대상 테이블 행을 수정할 수 없지만 계속 읽을 수는 있음), 마지막으로 테이블 스키마를 변경하기 위해 **SCH_M_LOCK** 이 요청된다. 이러한 방식은 DDL 연산이 컴파일되고 수행되는 동안 다른 트랜잭션이 연산을 수행하는 것을 허용하여, 동시성을 높일 수 있다는 이점이 있다.

    하지만 이 방식은 같은 테이블에 동시에 DDL 연산을 수행할 때 교착 상태를 회피할 수 없다는 단점 또한 존재한다. 인덱스를 로딩함으로 인한 교착 상태의 예는 다음과 같다.
    그러나 이 방식은 같은 테이블에 동시에 DDL 연산을 수행할 때 교착 상태를 회피할 수 없다는 단점 또한 존재한다. 인덱스 로딩으로 인한 교착 상태의 예는 다음과 같다.

    +---------------------------------------------------------------+---------------------------------------------------------------+
    | T1                                                            | T2                                                            |
    +===============================================================+===============================================================+
    | .. code-block:: sql                                           | .. code-block :: sql                                          |
    |                                                               |                                                               |
    |  CREATE INDEX i_t_i on t(i) WHERE i > 0;                      |   CREATE INDEX i_t_j on t(j) WHERE j > 0;                     |
    +---------------------------------------------------------------+---------------------------------------------------------------+
    | "i > 0" 경우의 타입 검사중에 SCH_S_LOCK.                      |                                                               |
    +---------------------------------------------------------------+---------------------------------------------------------------+
    |                                                               |  "j > 0" case."j > 0" 타입 검사중에 SCH_S_LOCK                |
    +---------------------------------------------------------------+---------------------------------------------------------------+
    | 인덱스 로딩 중에 SIX_LOCK.                                    |                                                               |
    +---------------------------------------------------------------+---------------------------------------------------------------+
    |                                                               | SIX_LOCK을 요구하나 T1이 SIX_LOCK의 반환을 대기               |
    +---------------------------------------------------------------+---------------------------------------------------------------+
    | SCH_M_LOCK을 요구하나 T2가 SCH_S_LOCK의 반환을 대기           |                                                               |
    +---------------------------------------------------------------+---------------------------------------------------------------+
   
*   **특수 잠금**

    **CUBRID 10.2** 시스템 내부적으로 특수한 오퍼레이션에서 사용하는 새로운 유형의 잠금을 소개한다.

    *   **대량갱신잠금(Bulk update lock, BU_LOCK)**

        이 잠금은 데이터베이스에 대량의 데이타를 삽입하는 경우에 사용하도록 설계되었다. **CUBRID 10.2** 를 기준으로, 이 잠금은 테이블에 행을 load하는 동안 :ref:`loaddb` 과정에서 배타적으로 사용된다. **BU_LOCK** 은 자기 자신과 **SCH_S_LOCK** 에 호환된다. 결과적으로 이 잠금은 다른 어떤 **SELECT/DML/DDL** 구문도 동일한 테이블에 접근하지 못한다는 것을 보장한다. 그러나 둘 이상의 **loaddb** 가 하나의 테이블에 행을 load할 수는 있다. **BU_LOCK** 잠금 보유자는 행 잠금을 요구하진 않는다.

.. note:: 잠금에 대해 요약하면 다음과 같다.

    *   잠금 대상 객체에 대해 행(인스턴스)과 스키마(클래스)가 있다. 사용된 객체 종류를 기준으로 잠금을 나누면 다음과 같다.

        *   행 잠금: **S_LOCK**, **X_LOCK**
    
        *   의도/스키마 잠금: **IX_LOCK**, **IS_LOCK**, **SIX_LOCK**, **SCH_S_LOCK**, **SCH_M_LOCK**
        
        *   특수 잠금: **BU_LOCK**
        
    *   모든 유형의 잠금은 서로 영향을 미친다.
     
위에서 설명한 잠금들의 호환 관계(lock compatibility)를 정리하면 아래의 표와 같다. 호환된다는 것은 잠금 보유자(lock holder)가 특정 객체에 대해 획득한 잠금과 중복하여 잠금 요청자(lock requester)가 잠금을 획득할 수 있다는 의미이다.

**잠금 호환성**

*   **NULL**\: lock이 존재하는 상태.

(O: TRUE, X: FALSE)

+----------------------------------+-----------------------------------------------------------------------------------------------------------+
|                                  | **잠금 보유자(Lock holder)**                                                                              |
|                                  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                                  | **NULL**  | **SCH-S** | **IS**    | **S**     | **IX**    | **BU**    | **SIX**   | **X**     | **SCH-M** |
+----------------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
| **잠금 요청자**      | **NULL**  | O         | O         | O         | O         | O         | O         | O         | O         | O         |
| **(Lock requester)** |           |           |           |           |           |           |           |           |           |           |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SCH-S** | O         | O         | O         | O         | O         | O         | O         | O         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **IS**    | O         | O         | O         | O         | O         | X         | O         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **S**     | O         | O         | O         | O         | X         | X         | X         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **IX**    | O         | O         | O         | X         | O         | X         | X         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **BU**    | O         | O         | X         | X         | X         | O         | X         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SIX**   | O         | O         | O         | X         | X         | X         | X         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **X**     | O         | O         | X         | X         | X         | X         | X         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SCH-M** | O         | X         | X         | X         | X         | X         | X         | X         | X         |
+----------------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+

**잠금 변환 테이블**

*   **NULL**\: 아무 잠금도 없는 상태 

+----------------------------------+-----------------------------------------------------------------------------------------------------------+
|                                  | **획득 잠금 모드(Granted lock mode)**                                                                     |
|                                  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                                  | **NULL**  | **SCH-S** | **IS**    | **S**     | **IX**    | **BU**    | **SIX**   | **X**     | **SCH-M** |
+----------------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
| **요청 잠금 모드**   | **NULL**  | NULL      | SCH-S     | IS        | S         | IX        | BU        | SIX       | X         | SCH-M     |    
| **(Requested lock**  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
| **mode)**            | **SCH-S** | SCH-S     | SCH-S     | IS        | S         | IX        | BU        | SIX       | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **IS**    | IS        | IS        | IS        | S         | IX        | X         | SIX       | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **S**     | S         | S         | S         | S         | SIX       | X         | SIX       | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **IX**    | IX        | IX        | IX        | SIX       | IX        | X         | SIX       | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **BU**    | BU        | BU        | BU        | X         | BU        | BU        | BU        | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SIX**   | SIX       | SIX       | SIX       | SIX       | SIX       | X         | SIX       | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **X**     | X         | X         | X         | X         | X         | X         | X         | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SCH-M** | SCH-M     | SCH-M     | SCH-M     | SCH-M     | SCH-M     | SCH-M     | SCH-M     | SCH-M     | SCH-M     |    
+----------------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+

잠금 사용 예제
++++++++++++++

다음 예제에서는 REPEATABLE READ(5) 격리 수준이 사용된다. READ COMMITTED의 행 갱신 규칙은 다양하며, 다음 섹션에서 설명한다.
예제에서는 기존 잠금을 보여주기 위해 lockdb 유틸리티를 사용한다.

**잠금 예제:**
다음 예제에서는 REPEATABLE READ(5) 격리 수준이 사용되며 동일한 행에서 읽기 및 쓰기가 차단되지 않는다는 것을 보여준다. 갱신 충돌이 시도되고 두 번째 갱신자(updater)가 차단된다. 트랜잭션 T1이 커밋될 때 T2의 차단이 해제되지만 격리 수준의 제약 사항으로 인해 갱신은 허용되지 않는다. T1이 롤백하는 경우 T2가 갱신을 진행할 수 있다.
 
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
| T1                                                      | T2                                                      | 설명                                                                       |
+=========================================================+=========================================================+============================================================================+
| .. code-block :: sql                                    | .. code-block :: sql                                    | AUTOCOMMIT OFF, REPEATABLE READ 로 설정                                    |
|                                                         |                                                         |                                                                            |
|   csql> ;au off                                         |   csql> ;au off                                         |                                                                            |
|   csql> SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;|   csql> SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;|                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
| .. code-block :: sql                                    |                                                         |                                                                            |
|                                                         |                                                         |                                                                            |
|   csql> CREATE TABLE tbl(a INT PRIMARY KEY, b INT);     |                                                         |                                                                            |
|                                                         |                                                         |                                                                            |
|   csql> INSERT INTO tbl                                 |                                                         |                                                                            |
|         VALUES (10, 10),                                |                                                         |                                                                            |
|                (30, 30),                                |                                                         |                                                                            |
|                (50, 50),                                |                                                         |                                                                            |
|                (70, 70);                                |                                                         |                                                                            |
|   csql> COMMIT;                                         |                                                         |                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
| .. code-block :: sql                                    |                                                         | a = 10인 열의 첫번째 버전이 잠기고 갱신됨. a = 90 인 열의 새로운 버전이    |
|                                                         |                                                         | 생성되고 잠김 ::                                                           |
|   csql> UPDATE tbl SET a = 90 WHERE a = 10;             |                                                         |                                                                            |
|                                                         |                                                         |   cubrid lockdb:                                                           |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |   OID =  0|   623|   4                                                     |
|                                                         |                                                         |   Object type: Class = tbl.                                                |
|                                                         |                                                         |   Total mode of holders =   IX_LOCK,                                       |
|                                                         |                                                         |        Total mode of waiters = NULL_LOCK.                                  |
|                                                         |                                                         |   Num holders=  1, Num blocked-holders=  0,                                |
|                                                         |                                                         |       Num waiters=  0                                                      |
|                                                         |                                                         |   LOCK HOLDERS:                                                            |
|                                                         |                                                         |       Tran_index =   1, Granted_mode =  IX_LOCK                            |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |   OID =  0|   650|   5                                                     |
|                                                         |                                                         |   Object type: Instance of class ( 0|   623|   4) = tbl.                   |
|                                                         |                                                         |   MVCC info: insert ID = 5, delete ID = missing.                           |
|                                                         |                                                         |   Total mode of holders =    X_LOCK,                                       |
|                                                         |                                                         |       Total mode of waiters = NULL_LOCK.                                   |
|                                                         |                                                         |   Num holders=  1, Num blocked-holders=  0,                                |
|                                                         |                                                         |       Num waiters=  0                                                      |
|                                                         |                                                         |   LOCK HOLDERS:                                                            |
|                                                         |                                                         |       Tran_index =   1, Granted_mode =   X_LOCK                            |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |   OID =  0|   650|   1                                                     |
|                                                         |                                                         |   Object type: Instance of class ( 0|   623|   4) = tbl.                   |
|                                                         |                                                         |   MVCC info: insert ID = 4, delete ID = 5.                                 |
|                                                         |                                                         |   Total mode of holders =    X_LOCK,                                       |
|                                                         |                                                         |       Total mode of waiters = NULL_LOCK.                                   |
|                                                         |                                                         |   Num holders=  1, Num blocked-holders=  0,                                |
|                                                         |                                                         |       Num waiters=  0                                                      |
|                                                         |                                                         |   LOCK HOLDERS:                                                            |
|                                                         |                                                         |       Tran_index =   1, Granted_mode =   X_LOCK                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
|                                                         | .. code-block :: sql                                    | T2는 a<=20 인 조건을 만족하는 모든 열을 읽음. 갱신을 수행한 T1이 커밋을    |
|                                                         |                                                         | 하지 않았기 때문에 T2는 a=10인 열을 계속 보게되고 잠금을 하지는 않음 ::    |
|                                                         |   csql> SELECT * FROM tbl WHERE a <= 20;                |                                                                            |
|                                                         |                                                         |   cubrid lockdb:                                                           |
|                                                         |                                                         |                                                                            |
|                                                         |               a            b                            |   OID =  0|   623|   4                                                     |
|                                                         |    ==========================                           |   Object type: Class = tbl.                                                |
|                                                         |               10           10                           |   Total mode of holders =   IX_LOCK,                                       |
|                                                         |                                                         |       Total mode of waiters = NULL_LOCK.                                   |
|                                                         |                                                         |   Num holders=  2, Num blocked-holders=  0,                                |
|                                                         |                                                         |       Num waiters=  0                                                      |
|                                                         |                                                         |   LOCK HOLDERS:                                                            |
|                                                         |                                                         |       Tran_index =   1, Granted_mode =  IX_LOCK                            |
|                                                         |                                                         |       Tran_index =   2, Granted_mode =  IS_LOCK                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
|                                                         | .. code-block :: sql                                    | T2는 a <= 20인 모든 열에 대해 갱신을 시도한다.                             |
|                                                         |                                                         | T2가 클래스의 잠금을 IX_LOCK로 업그레이드하고, 이미 잠긴 a = 10인 열의     |
|                                                         |                                                         | 갱신을 시도하지만, T1에 의해 이미 잠긴 상태이므로 T2는 차단된다. ::        |
|                                                         |   csql> UPDATE tbl                                      |                                                                            |
|                                                         |         SET a = a + 100                                 |                                                                            |
|                                                         |         WHERE a <= 20;                                  |                                                                            |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |   cubrid lockdb:                                                           |
|                                                         |                                                         |   OID =  0|   623|   4                                                     |
|                                                         |                                                         |   Object type: Class = tbl.                                                |
|                                                         |                                                         |   Total mode of holders =   IX_LOCK,                                       |
|                                                         |                                                         |       Total mode of waiters = NULL_LOCK.                                   |
|                                                         |                                                         |   Num holders=  2, Num blocked-holders=  0,                                |
|                                                         |                                                         |       Num waiters=  0                                                      |
|                                                         |                                                         |   LOCK HOLDERS:                                                            |
|                                                         |                                                         |       Tran_index =   1, Granted_mode =  IX_LOCK                            |
|                                                         |                                                         |       Tran_index =   2, Granted_mode =  IX_LOCK                            |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |   OID =  0|   650|   5                                                     |
|                                                         |                                                         |   Object type: Instance of class ( 0|   623|   4) = tbl.                   |
|                                                         |                                                         |   MVCC info: insert ID = 5, delete ID = missing.                           |
|                                                         |                                                         |   Total mode of holders =   X_LOCK,                                        |
|                                                         |                                                         |       Total mode of waiters = NULL_LOCK.                                   |
|                                                         |                                                         |   Num holders=  1, Num blocked-holders=  0,                                |
|                                                         |                                                         |       Num waiters=  0                                                      |
|                                                         |                                                         |   LOCK HOLDERS:                                                            |
|                                                         |                                                         |       Tran_index =   1, Granted_mode =   X_LOCK                            |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |   OID =  0|   650|   1                                                     |
|                                                         |                                                         |   Object type: Instance of class ( 0|   623|   4) = tbl.                   |
|                                                         |                                                         |   MVCC info: insert ID = 4, delete ID = 5.                                 |
|                                                         |                                                         |   Total mode of holders =    X_LOCK,                                       |
|                                                         |                                                         |       Total mode of waiters =    X_LOCK.                                   |
|                                                         |                                                         |   Num holders=  1, Num blocked-holders=  0,                                |
|                                                         |                                                         |       Num waiters=  1                                                      |
|                                                         |                                                         |   LOCK HOLDERS:                                                            |
|                                                         |                                                         |       Tran_index =   1, Granted_mode =   X_LOCK                            |
|                                                         |                                                         |   LOCK WAITERS:                                                            |
|                                                         |                                                         |       Tran_index =   2, Blocked_mode =   X_LOCK                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
| .. code-block :: sql                                    |                                                         | T1의 잠금이 해제되었다.                                                    |
|                                                         |                                                         |                                                                            |
|   csql> COMMIT;                                         |                                                         |                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
|                                                         | ::                                                      | T2가 차단에서 해제되어 T1이 이미 갱신한 개체의 갱신을 시도한다.            |
|                                                         |                                                         | REPEATABLE READ 격리 수준에서 이것은 허용되지 않고                         |
|                                                         |     ERROR: Serializable conflict due                    | 오류가 전송됨                                                              | 
|                                                         |     to concurrent updates                               |                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+

unique 제약 조건을 보호하기 위한 잠금
-------------------------------------

이전 CUBRID 버전의 2단계 잠금 프로토콜은 unique 제약 조건과 상위 격리 제한을 보호하기 위해 인덱스 키 잠금을 사용했다. CUBRID 10.0에서는 키 잠금이 제거되었다. 격리 수준 제한은 MVCC 스냅샷으로 해결되었지만 unique 제약 조건에는 여전히 특정 유형의 보호가 필요했다.

MVCC는 행처럼 고유 인덱스가 동시에 여러 버전을 유지하므로 각각 다른 트랜잭션에서 볼 수 있다. 여러 버전 중 하나는 마지막 버전이고, 나머지 버전들은 비가시화된 후 **VACUUM** 에 의해 제거될 때까지 일시적으로 유지된다. unique 제약 조건을 보호하는 규칙은 키를 수정하려는 모든 트랜잭션이 키의 마지막 버전을 잠그는 것이다.

아래 예제는 잠금을 통해 unique 제약 조건 위반 방지를 위해 REPEATABLE READ 격리 수준을 사용한다.

+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
| T1                                                      | T2                                                      | 설명                                                                       |
+=========================================================+=========================================================+============================================================================+
| .. code-block :: sql                                    | .. code-block :: sql                                    | AUTOCOMMIT OFF, REPEATABLE READ 로 설정                                    |
|                                                         |                                                         |                                                                            |
|   csql> ;au off                                         |   csql> ;au off                                         |                                                                            |
|   csql> SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;|   csql> SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;|                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
| .. code-block :: sql                                    |                                                         |                                                                            |
|                                                         |                                                         |                                                                            |
|   csql> CREATE TABLE tbl(a INT PRIMARY KEY, b INT);     |                                                         |                                                                            |
|                                                         |                                                         |                                                                            |
|   csql> INSERT INTO tbl                                 |                                                         |                                                                            |
|         VALUES (10, 10),                                |                                                         |                                                                            |
|                (30, 30),                                |                                                         |                                                                            |
|                (50, 50),                                |                                                         |                                                                            |
|                (70, 70);                                |                                                         |                                                                            |
|   csql> COMMIT;                                         |                                                         |                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
| .. code-block :: sql                                    |                                                         | T1이 테이블에 새로운 열을 삽입하고 잠금으로써 기본키 20은 보호된다.        |
|                                                         |                                                         |                                                                            |
|   csql> INSERT INTO tbl VALUES (20, 20);                |                                                         |                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
|                                                         | .. code-block :: sql                                    | T2는 테이블에 새로운 열을 삽입하고 잠금을 요청한다.                        |
|                                                         |                                                         | 하지만, T2가 기본키에 새로운 열을 삽입하려고 할 때,                        |
|                                                         |    INSERT INTO tbl VALUES (20, 120);                    | 이미 기본키 20이 존재하기 때문에 T2는 T1이 커밋할 때까지 차단된다. ::      |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |   cubrid lockdb:                                                           |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |   OID =  0|   623|   4                                                     |
|                                                         |                                                         |   Object type: Class = tbl.                                                |
|                                                         |                                                         |   Total mode of holders =   IX_LOCK,                                       |
|                                                         |                                                         |       Total mode of waiters = NULL_LOCK.                                   |
|                                                         |                                                         |   Num holders=  2, Num blocked-holders=  0,                                |
|                                                         |                                                         |       Num waiters=  0                                                      |
|                                                         |                                                         |   LOCK HOLDERS:                                                            |
|                                                         |                                                         |       Tran_index =   1, Granted_mode =  IX_LOCK                            |
|                                                         |                                                         |       Tran_index =   2, Granted_mode =  IX_LOCK                            |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |   OID =  0|   650|   5                                                     |
|                                                         |                                                         |   Object type: Instance of class ( 0|   623|   4) = tbl.                   |
|                                                         |                                                         |   MVCC info: insert ID = 5, delete ID = missing.                           |
|                                                         |                                                         |   Total mode of holders =   X_LOCK,                                        |
|                                                         |                                                         |       Total mode of waiters =    X_LOCK.                                   |
|                                                         |                                                         |   Num holders=  1, Num blocked-holders=  0,                                |
|                                                         |                                                         |       Num waiters=  1                                                      |
|                                                         |                                                         |   LOCK HOLDERS:                                                            |
|                                                         |                                                         |       Tran_index =   1, Granted_mode =   X_LOCK                            |
|                                                         |                                                         |   LOCK WAITERS:                                                            |
|                                                         |                                                         |       Tran_index =   1, Blocked_mode =   X_LOCK                            |
|                                                         |                                                         |                                                                            |
|                                                         |                                                         |   OID =  0|   650|   6                                                     |
|                                                         |                                                         |   Object type: Instance of class ( 0|   623|   4) = tbl.                   |
|                                                         |                                                         |   MVCC info: insert ID = 6, delete ID = missing.                           |
|                                                         |                                                         |   Total mode of holders =    X_LOCK,                                       |
|                                                         |                                                         |       Total mode of waiters = NULL_LOCK.                                   |
|                                                         |                                                         |   Num holders=  1, Num blocked-holders=  0,                                |
|                                                         |                                                         |       Num waiters=  0                                                      |
|                                                         |                                                         |   LOCK HOLDERS:                                                            |
|                                                         |                                                         |       Tran_index =   2, Granted_mode =   X_LOCK                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
| .. code-block :: sql                                    |                                                         | T1의 잠금이 해제된다.                                                      |
|                                                         |                                                         |                                                                            |
|   COMMIT;                                               |                                                         |                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
|                                                         | ::                                                      | 차단이 해제된 T2는 T1이 커밋한 키로 인해 unique 제약 위반 오류가 발생한다. |
|                                                         |                                                         |                                                                            |
|                                                         |    ERROR: Operation would have caused                   |                                                                            |
|                                                         |    one or more unique constraint violations.            |                                                                            |
|                                                         |    INDEX pk_tbl_a(B+tree: 0|186|640)                    |                                                                            |
|                                                         |    ON CLASS tbl(CLASS_OID: 0|623|4).                    |                                                                            |
|                                                         |    key: 20(OID: 0|650|6).                               |                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+

트랜잭션 교착 상태(deadlock)
----------------------------

교착 상태(deadlock)는 둘 이상의 트랜잭션이 서로 맞물려 상대방의 잠금이 해제되기를 기다리는 상태이다. 이러한 교착 상태에서는 서로가 상대방의 작업 수행을 차단하기 때문에 CUBRID는 트랜잭션 중 하나를 롤백시켜 교착 상태를 해결한다. 롤백되는 트랜잭션은 일반적으로 가장 적은 갱신을 수행한 것인데 보통 가장 최근에 시작된 트랜잭션이다. 시스템에 의해 트랜잭션이 롤백되자마자 그 트랜잭션이 가지고 있던 잠금이 해제되고 교착 상태에 있던 다른 트랜잭션이 진행되도록 허가된다.

이러한 교착 상태 발생은 예측할 수 없지만 가급적 교착 상태가 발생하지 않도록 하려면, 인덱스를 설정하여 잠금이 설정되는 범위를 줄이거나 트랜잭션을 짧게 만들거나 트랜잭션 격리 수준(isolation level)을 낮게 설정하는 것이 좋다.

에러 심각성 수준을 설정하는 시스템 파라미터인 **error_log_level** 의 값을 NOTIFICATION으로 설정하면 교착 상태 발생 시 서버 에러 로그 파일에 잠금 관련 정보가 기록된다.

이전 버전에 비해 CUBRID 10.0은 더 이상 인덱스 키 잠금을 사용하여 인덱스를 읽고 쓰지 않으므로 교착 상태 발생이 현저히 줄어들었다. 교착 상태가 자주 발생하지 않는 또 다른 이유는 이전 CUBRID 버전은 일정 범위의 인덱스를 읽을 때 높은 격리 수준으로 인해 여러 객체들을 잠궜지만, CUBRID 10.0은 더 이상 잠금을 사용하지 않기 때문이다.

그러나 서로 다른 두 개의 트랜잭션에서 동일한 객체를 다른 순서로 갱신할 경우 여전히 교착 상태가 발생할 가능성이 남아 있다.

**예제**

+----------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+
| session 1                                                                                          | session 2                                                                                          |
+====================================================================================================+====================================================================================================+
| .. code-block :: sql                                                                               | .. code-block :: sql                                                                               |
|                                                                                                    |                                                                                                    |
|   csql> ;autocommit off                                                                            |   csql> ;autocommit off                                                                            |
|                                                                                                    |                                                                                                    |
|   AUTOCOMMIT IS OFF                                                                                |   AUTOCOMMIT IS OFF                                                                                |
|                                                                                                    |                                                                                                    |
|   csql> set transaction isolation level REPEATABLE READ;                                           |   csql> set transaction isolation level REPEATABLE READ;                                           |
|                                                                                                    |                                                                                                    |
|   Isolation level set to:                                                                          |   Isolation level set to:                                                                          |
|   REPEATABLE READ                                                                                  |   REPEATABLE READ                                                                                  |
+----------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+
| .. code-block :: sql                                                                               |                                                                                                    |
|                                                                                                    |                                                                                                    |
|   csql> CREATE TABLE lock_tbl(host_year INTEGER,                                                   |                                                                                                    |
|                               nation_code CHAR(3));                                                |                                                                                                    |
|   csql> INSERT INTO lock_tbl VALUES (2004, 'KOR');                                                 |                                                                                                    |
|   csql> INSERT INTO lock_tbl VALUES (2004, 'USA');                                                 |                                                                                                    |
|   csql> INSERT INTO lock_tbl VALUES (2004, 'GER');                                                 |                                                                                                    |
|   csql> INSERT INTO lock_tbl VALUES (2008, 'GER');                                                 |                                                                                                    |
|   csql> COMMIT;                                                                                    |                                                                                                    |
|                                                                                                    |                                                                                                    |
+----------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+
| .. code-block :: sql                                                                               | .. code-block :: sql                                                                               |
|                                                                                                    |                                                                                                    |
|   csql> DELETE FROM lock_tbl WHERE nation_code = 'KOR';                                            |   csql> DELETE FROM lock_tbl WHERE nation_code = 'GER';                                            |
|                                                                                                    |                                                                                                    |
|   /* The two transactions lock different objects                                                   |                                                                                                    |
|    * and they do not block each-other.                                                             |                                                                                                    |
|    */                                                                                              |                                                                                                    |
+----------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+
| .. code-block :: sql                                                                               |                                                                                                    |
|                                                                                                    |                                                                                                    |
|   csql> DELETE FROM lock_tbl WHERE host_year=2008;                                                 |                                                                                                    |
|                                                                                                    |                                                                                                    |
|   /* T1 want's to modify a row locked by T2 and is blocked */                                      |                                                                                                    |
|                                                                                                    |                                                                                                    |
+----------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+
|                                                                                                    | .. code-block :: sql                                                                               |
|                                                                                                    |                                                                                                    |
|                                                                                                    |   csql> DELETE FROM lock_tbl WHERE host_year = 2004;                                               |
|                                                                                                    |                                                                                                    |
|                                                                                                    |   /* T2 now want to delete the row blocked by T1                                                   |
|                                                                                                    |    * and a deadlock is created.                                                                    |
|                                                                                                    |    */                                                                                              |
|                                                                                                    |                                                                                                    |
+----------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+
| ::                                                                                                 | ::                                                                                                 |
|                                                                                                    |                                                                                                    |
|   ERROR: Your transaction (index 1, dba@ 090205|4760)                                              |   /* T2 is unblocked and proceeds on modifying its rows. */                                        |
|          has been unilaterally aborted by the system.                                              |                                                                                                    |
|                                                                                                    |                                                                                                    |
|   /* System rolled back the transaction 1 to resolve a deadlock */                                 |                                                                                                    |
|                                                                                                    |                                                                                                    |
+----------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+

트랜잭션 잠금 타임아웃
----------------------

CUBRID는 트랜잭션 잠금 설정이 허용될 때까지 잠금을 대기하는 시간을 설정하는 잠금 타임아웃(lock timeout) 기능을 제공한다.

만약 설정된 잠금 타임아웃 시간 이내에 잠금이 허용되지 않으면, 잠금 타임아웃 시간이 경과된 시점에 해당 트랜잭션을 롤백시키고 에러를 출력한다. 또한, 잠금 타임아웃 시간 이내에 트랜잭션 교착 상태가 발생하면, CUBRID는 교착 상태에 있는 여러 트랜잭션 중 대기시간이 타임아웃 시간에 가까운 트랜잭션을 롤백시킨다.

**잠금 타임아웃 값 설정**

**$CUBRID/conf/cubrid.conf** 파일 내의 시스템 파라미터 **lock_timeout** 또는 **SET TRANSACTION** 구문을 통해 응용 프로그램이 잠금을 대기하는 타임아웃 시간(초 단위)을 설정하며, 설정된 시간이 경과된 이후에는 해당 트랜잭션을 롤백시키고 에러를 출력한다. **lock_timeout** 파라미터의 기본값은 **-1** 이며, 이는 트랜잭션 잠금이 허용되는 시점까지 무한정 대기한다는 의미이다. 따라서, 사용자는 응용 프로그램의 트랜잭션 패턴에 맞게 이 값을 변경할 수 있다. 만약, 잠금 타임아웃 값이 0으로 설정되면 잠금이 발생하는 즉시 에러 메시지가 출력될 것이다. ::

    SET TRANSACTION LOCK TIMEOUT timeout_spec [ ; ]
    timeout_spec:
    - INFINITE
    - OFF
    - unsigned_integer
    - variable

*   **INFINITE** : 트랜잭션 잠금이 허용될 때까지 무한정 대기한다. 시스템 파라미터 **lock_timeout**\ 을 -1로 설정한 것과 같다.
*   **OFF** : 잠금을 대기하지 않고, 해당 트랜잭션을 롤백시킨 후 에러 메시지를 출력한다. 시스템 파라미터 **lock_timeout**\ 을 0으로 설정한 것과 같다.
*   *unsigned_integer* : 초 단위로 설정되며, 설정된 시간만큼 트랜잭션 잠금을 대기한다.
*   *variable* : 변수를 지정할 수 있으며, 변수에 저장된 값만큼 트랜잭션 잠금을 대기한다.

**예제 1** ::

    vi $CUBRID/conf/cubrid.conf
    ...
    lock_timeout = 10s
    ...

**예제 2** ::

    SET TRANSACTION LOCK TIMEOUT 10;

**잠금 타임아웃 값 확인**

**GET TRANSACTION** 문을 이용하여 현재 응용 프로그램이 설정된 잠금 타임아웃 값을 확인할 수 있고, 이 값을 변수에 저장할 수도 있다. ::

    GET TRANSACTION LOCK TIMEOUT [ { INTO | TO } variable ] [ ; ]

**예제** ::

    GET TRANSACTION LOCK TIMEOUT;
    
             Result
    ===============
      1.000000e+001

**잠금 타임아웃 에러 메시지 확인과 조치 방법**

다른 트랜잭션의 잠금이 해제되기를 대기하던 트랜잭션에 대해 잠금 타임아웃이 발생하면, 아래와 같은 에러 메시지를 출력한다. ::

    Your transaction (index 2, user1@host1|9808) timed out waiting on IX_LOCK lock on class tbl. You are waiting for
    user(s) user1@host1|csql(9807), user1@host1|csql(9805) to finish.

*   Your transaction(index 2 ...) : 잠금을 대기하다가 타임아웃으로 롤백된 트랜잭션의 인덱스가 2라는 의미이다. 트랜잭션 인덱스는 클라이언트가 데이터베이스 서버에 접속하였을 때 순차적으로 할당되는 번호이다. 이는 **cubrid lockdb** 유틸리티 실행을 통해서도 확인할 수 있다.

*   (... user1\@host1|9808) : *user1* 는 클라이언트의 로그인 아이디이고, @의 뒷 부분은 클라이언트가 수행된 호스트 이름이다. 또한 | 의 뒷 부분은 클라이언트의 프로세스 ID(PID)이다.

*   IX_LOCK : 특정 행에 배타 잠금이 설정됨에 따라 상위 객체인 테이블에 의도 배타 잠금이 설정된다. 이에 관한 상세한 설명은 :ref:`lock-mode` 을 참고한다.

*   user1@host1|csql(9807), user1@host1|csql(9805) : **IX_LOCK** 잠금을 설정하기 위해 종료되기를 기다리는 다른 트랜잭션들이다.

즉, 위의 잠금 에러 메시지는 "다른 트랜잭션들이 *participant* 테이블의 특정 행에 **X_LOCK** 을 점유하고 있으므로, *host1* 호스트에서 수행된 트랜잭션 3은 잠금이 해제되기를 기다리다가 제한 시간이 초과되었다."로 해석할 수 있다.  만약, 에러 메시지에 명시된 트랜잭션의 잠금 정보를 확인하고자 한다면, **cubrid lockdb** 유틸리티를 통해 현재 **X_LOCK** 이 설정되어 있는 특정 행의 OID 값(예: 0|636|34)을 검색하여 현재 잠금을 점유 중인 트랜잭션 ID, 클라이언트 프로그램명 및 프로세스 ID(PID)를 확인할 수 있다. 이에 관한 상세한 설명은 :ref:`lockdb` 상태 확인 을 참고한다. CUBRID Manager에서 트랜잭션 잠금 정보를 확인할 수도 있다.

이처럼 트랜잭션의 잠금 정보를 확인한 후에는 SQL 로그를 통해 커밋되지 않은 질의문을 확인하여 트랜잭션을 정리할 수 있다. SQL 로그를 확인하는 방법은 :ref:`broker-logs`\ 를 참고한다.

또한, **cubrid killtran** 유틸리티를 통해 문제가 되는 트랜잭션을 강제 종료할 수 있으며, 이에 관한 상세한 설명은 :ref:`killtran` 를 참고한다.

.. _transaction-isolation-level:

트랜잭션 격리 수준
==================

트랜잭션의 격리 수준은 트랜잭션이 동시에 진행 중인 다른 트랜잭션에 의해 간섭받는 정도를 의미하며, 트랜잭션 격리 수준이 높을수록 트랜잭션 간 간섭이 적으며 직렬적이고, 트랜잭션 격리 수준이 낮을수록 트랜잭션 간 간섭은 많으나 높은 동시성을 보장한다. 사용자는 적용하고자 하는 서비스의 특성에 따라 격리 수준을 적절히 설정함으로써 데이터베이스의 일관성(consistency)과 동시성(concurrency)을 조정할 수 있다.

.. note:: 지원되는 모든 격리 수준에서 트랜잭션은 복구 가능하다. 이는 트랜잭션이 끝나기 전에는 갱신을 커밋하지 않기 때문이다.

.. _set-transaction-isolation-level:

트랜잭션 격리 수준 설정
-----------------------

**$CUBRID/conf/cubrid.conf** 의 **isolation_level** 및 **SET TRANSACTION** 문을 사용하여 트랜잭션 격리 수준을 설정할 수 있다. 기본적으로 **READ COMMITTED** 수준이 설정되어 있으며, 4~6 수준 중에서 4 수준에 해당한다(1~3 수준은 CUBRID의 이전 버전에서 사용되었으며 더 이상 사용하지 않음). 이에 관한 상세한 설명은 :ref:`database-concurrency` 을 참고한다. ::

    SET TRANSACTION ISOLATION LEVEL isolation_level_spec ;
    
    isolation_level_spec:
        SERIALIZABLE | 6
        REPETABLE READ | 5
        READ COMMITTED | CURSOR STABILITY | 4

**예제 1** ::

    vi $CUBRID/conf/cubrid.conf
    ...

    isolation_level = 4
    ...

    -- or 

    isolation_level = "TRAN_READ_COMMITTED"

**예제 2** ::

    SET TRANSACTION ISOLATION LEVEL 4;
    -- or 
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;

**CUBRID가 지원하는 격리 수준**

+-----------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 격리 수준 이름        | 설명                                                                                                                                                                                |
+=======================+=====================================================================================================================================================================================+
| READ COMMITTED (4)    | 트랜잭션 T1이 테이블 A를 조회하는 동안 다른 트랜잭션T2는 테이블A의 스키마를 갱신 할 수 없다.                                                                                        |
|                       | 트랜잭션 T1은 여러번 R를 조회하는 중에 트랜잭션 T2가 갱신하고 커밋한 R'를 읽기(반복 불가능한 읽기)를 경험할 수 있다.                                                                |
+-----------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| REPEATABLE READ (5)   | 트랜잭션 T1이 테이블 A를 조회하는 동안 다른 트랜잭션T2는 테이블 A의 스키마를 갱신 할 수 없다.                                                                                       |
+-----------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| SERIALIZABLE (6)      | 지원 예정  - 상세한 사항은 다음을 참고한다.  :ref:`isolation-level-6`                                                                                                               |
+-----------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

응용 프로그램에서 트랜잭션 수행 중에 격리 수준이 변경되면, 수행 중인 트랜잭션의 남은 부분부터 변경된 격리 수준이 적용된다. 이처럼 설정된 격리 수준이 하나의 트랜잭션 전체에 적용되는 것이 아니라 트랜잭션 중간에 변경되어 적용될 수 있기 때문에, 트랜잭션 격리 수준은 트랜잭션 시작 시점(커밋, 롤백, 또는 시스템 재시작 이후)에 변경하는 것이 바람직하다.

트랜잭션 격리 수준 값 확인
--------------------------

**GET TRANSACTION ISOLATION LEVEL** 문을 이용하여 현재 클라이언트에 설정된 격리 수준 값을 출력하거나 *variable* 에 할당할 수 있다. 아래는 격리 수준을 확인하기 위한 구문이다. ::

    GET TRANSACTION ISOLATION LEVEL [ { INTO | TO } variable ] [ ; ]

.. code-block:: sql

    GET TRANSACTION ISOLATION LEVEL;
    
::

           Result
    =============
      READ COMMITTED

.. _isolation-level-4:

READ COMMITTED 격리 수준
------------------------

비교적 낮은 격리 수준(4)으로서 더티 읽기는 발생하지 않지만, 반복 불가능한 읽기와 유령 읽기는 발생할 수 있다. 즉, 트랜잭션 *T1* 이 하나의 객체를 반복하여 조회하는 동안 다른 트랜잭션 *T2* 에서의 삽입 또는 갱신이 허용되어, 트랜잭션 *T1* 이 다른 값을 읽을 수 있다는 의미이다.

다음과 같은 규칙이 적용된다. 

*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 삽입되는 레코드를 읽거나 수정할 수 없다. 대신 레코드가 무시된다.
*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 갱신하는 레코드를 읽을 수 있으며, 레코드의 마지막 커밋된 버전을 확인한다(커밋되지 않은 버전은 볼 수 없음).
*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 갱신 중인 레코드를 수정할 수 없다. *T1* 은 *T2* 가 커밋되기를 기다린 후 커밋이 되면 레코드 값을 다시 평가한다. 재평가시 적합하면 *T1* 은 레코드를 수정하고 적합하지 않으면 무시한다.
*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 조회 중인 레코드를 수정할 수 있다.
*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 조회 중인 테이블에 레코드를 갱신/삽입할 수 있다.
*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 조회 중인  테이블의 스키마를 변경할 수 없다.
*   트랜잭션 *T1* 이 수행된 각각의 질의문에 새로운 스냅샷을 생성하므로 유령 읽기 또는 반복할 수 없는 읽기는 발생할 수 있다.

이 격리 수준은 배타 잠금에 대해 MVCC 잠금 프로토콜을 따른다. 행에 공유 잠금은 필요하지 않지만 테이블에 대한 의도 잠금은 스키마에 대한 반복 가능한 읽기를 보장하기 위하여 트랜잭션이 종료될 때 해제된다.

*예:*

다음 예제는 트랜잭션 격리 수준이 **READ COMMITTED** 인 경우, 한 트랜잭션이 객체 읽기를 수행하는 동안 다른 트랜잭션이 레코드를 추가하거나 갱신할 수 있기때문에 유령 또는 반복 불가능한 읽기가 발생할 수 있음을 보여주고 테이블 스키마 갱신에 대해서는 반복 가능한 읽기가 보장된다는 것을 보여준다.

+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| session 1                                                               | session 2                                                                        |
+=========================================================================+==================================================================================+
| .. code-block :: sql                                                    | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|   csql> ;autocommit off                                                 |   csql> ;autocommit off                                                          |
|                                                                         |                                                                                  |
|   AUTOCOMMIT IS OFF                                                     |   AUTOCOMMIT IS OFF                                                              |
|                                                                         |                                                                                  |
|   csql> SET TRANSACTION ISOLATION LEVEL READ COMMITTED;                 |   csql> SET TRANSACTION ISOLATION LEVEL READ COMMITTED;                          |
|                                                                         |                                                                                  |
|   Isolation level set to:                                               |   Isolation level set to:                                                        |
|   READ COMMITTED                                                        |   READ COMMITTED                                                                 |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| .. code-block :: sql                                                    |                                                                                  |
|                                                                         |                                                                                  |
|   csql> CREATE TABLE isol4_tbl(host_year integer, nation_code char(3)); |                                                                                  |
|                                                                         |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (2008, 'AUS');                     |                                                                                  |
|                                                                         |                                                                                  |
|   csql> COMMIT;                                                         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |       host_year  nation_code                                                     |
|                                                                         |   ===================================                                            |
|                                                                         |            2008  'AUS'                                                           |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| .. code-block :: sql                                                    |                                                                                  |
|                                                                         |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (2004, 'AUS');                     |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (2000, 'NED');                     |                                                                                  |
|   csql> COMMIT;                                                         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|                                                                         |   /* phantom read occurs because tran 1 committed */                             |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |       host_year  nation_code                                                     |
|                                                                         |   ===================================                                            |
|                                                                         |            2008  'AUS'                                                           |
|                                                                         |            2004  'AUS'                                                           |
|                                                                         |            2000  'NED'                                                           |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| .. code-block :: sql                                                    |                                                                                  |
|                                                                         |                                                                                  |
|   csql> UPDATE isol4_tbl                                                |                                                                                  |
|   csql> SET nation_code = 'KOR'                                         |                                                                                  |
|   csql> WHERE host_year = 2008;                                         |                                                                                  |
|   csql> COMMIT;                                                         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|                                                                         |   /* unrepeatable read occurs because tran 1 committed */                        |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |       host_year  nation_code                                                     |
|                                                                         |   ===================================                                            |
|                                                                         |            2008  'KOR'                                                           |
|                                                                         |            2004  'AUS'                                                           |
|                                                                         |            2000  'NED'                                                           |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| .. code-block :: sql                                                    |                                                                                  |
|                                                                         |                                                                                  |
|   csql> ALTER TABLE isol4_tbl ADD COLUMN gold INT;                      |                                                                                  |
|                                                                         |                                                                                  |
|   /* unable to alter the table schema until tran 2 committed */         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|                                                                         |   /* repeatable read is ensured while                                            |
|                                                                         |    * tran_1 is altering table schema                                             |
|                                                                         |    */                                                                            |
|                                                                         |                                                                                  |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |       host_year  nation_code                                                     |
|                                                                         |   ===================================                                            |
|                                                                         |            2008  'KOR'                                                           |
|                                                                         |            2004  'AUS'                                                           |
|                                                                         |            2000  'NED'                                                           |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|                                                                         |   csql> COMMIT;                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |   /* unable to access the table until tran_1 committed */                        |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| .. code-block :: sql                                                    |                                                                                  |
|                                                                         |                                                                                  |
|   csql> COMMIT;                                                         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|                                                                         |   host_year  nation_code  gold                                                   |
|                                                                         |   ===================================                                            |
|                                                                         |     2008  'KOR'           NULL                                                   |
|                                                                         |     2004  'AUS'           NULL                                                   |
|                                                                         |     2000  'NED'           NULL                                                   |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+

READ COMMITTED UPDATE RE-EVALUATION
+++++++++++++++++++++++++++++++++++

**READ COMMITTED** 격리 수준은 동시에 발생하는 행 갱신에 대해 높은 격리 수준과 다르게 처리한다. 높은 격리 수준에서는 동시 트랜잭션 *T1* 에서 이미 갱신한 행을 *T2* 가 수정하려고 시도하면 *T1* 이 커밋 또는 롤백할 때까지 차단되며, *T1* 이 커밋되면 *T2* 가 질의 수행을 중단하고 직렬화 오류가 표시된다. **READ COMMITTED** 격리 수준에서는 *T1* 이 커밋되면 *T2* 가 질의 수행을 바로 중단하지 않고 최신 버전을 재 평가한다. 이전 버전을 선택하는 데 사용된 조건값이 최신 버전에 대해서도 계속 적용되면 *T2* 는 최신 버전을 수정한다. 조건값이 더 이상 만족하지 않으면 *T2* 는 해당 레코드의 수정을 무시한다.

*예:*

+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| session 1                                                               | session 2                                                                        |
+=========================================================================+==================================================================================+
| .. code-block :: sql                                                    | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|   csql> ;autocommit off                                                 |   csql> ;autocommit off                                                          |
|                                                                         |                                                                                  |
|   AUTOCOMMIT IS OFF                                                     |   AUTOCOMMIT IS OFF                                                              |
|                                                                         |                                                                                  |
|   csql> SET TRANSACTION ISOLATION LEVEL 4;                              |   csql> SET TRANSACTION ISOLATION LEVEL 4;                                       |
|                                                                         |                                                                                  |
|   Isolation level set to:                                               |   Isolation level set to:                                                        |
|   READ COMMITTED                                                        |   READ COMMITTED                                                                 |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| .. code-block :: sql                                                    |                                                                                  |
|                                                                         |                                                                                  |
|   csql> CREATE TABLE isol4_tbl(host_year integer, nation_code char(3)); |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (2000, 'KOR');                     |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (2004, 'USA');                     |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (2004, 'GER');                     |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (2008, 'GER');                     |                                                                                  |
|   csql> COMMIT;                                                         |                                                                                  |
|                                                                         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| .. code-block :: sql                                                    |                                                                                  |
|                                                                         |                                                                                  |
|   csql> UPDATE isol4_tbl                                                |                                                                                  |
|   csql> SET host_year = host_year - 4                                   |                                                                                  |
|   csql> WHERE nation_code = 'GER';                                      |                                                                                  |
|                                                                         |                                                                                  |
|   /* T1 locks and modifies (2004, 'GER') to (2000, 'GER') */            |                                                                                  |
|   /* T1 locks and modifies (2008, 'GER') to (2004, 'GER') */            |                                                                                  |
|                                                                         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|                                                                         |   csql> UPDATE isol4_tbl                                                         |
|                                                                         |   csql> SET host_year = host_year + 4                                            |
|                                                                         |   csql> WHERE host_year >= 2004;                                                 |
|                                                                         |                                                                                  |
|                                                                         |   /* T2 snapshot will try to modify three records:                               |
|                                                                         |    * (2004, 'USA'), (2004, 'GER'), (2008, 'GER')                                 |
|                                                                         |    *                                                                             |
|                                                                         |    * T2 locks and modifies (2004, 'USA') to (2008, 'USA')                        |
|                                                                         |    * T2 is blocked on lock on (2004, 'GER').                                     |
|                                                                         |    */                                                                            |
|                                                                         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| .. code-block :: sql                                                    |                                                                                  |
|                                                                         |                                                                                  |
|   csql> COMMIT;                                                         |                                                                                  |
|                                                                         |                                                                                  |
|   /* T1 releases locks on modified rows. */                             |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|                                                                         |   /* T2 is unblocked and will do the next steps:                                 |
|                                                                         |    *                                                                             |
|                                                                         |    *   T2 finds (2004, 'GER') has a new version (2000, 'GER')                    |
|                                                                         |    *   that doesn't satisfy predicate anymore.                                   |
|                                                                         |    *   T2 releases the lock on object and ignores it.                            |
|                                                                         |    *                                                                             |
|                                                                         |    *   T2 finds (2008, 'GER') has a new version (2004, 'GER')                    |
|                                                                         |    *   that still satisfies the predicate.                                       |
|                                                                         |    *   T2 keeps the lock and changes row to (2008, 'GER')                        |
|                                                                         |    */                                                                            |
|                                                                         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | .. code-block :: sql                                                             |
|                                                                         |                                                                                  |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |       host_year  nation_code                                                     |
|                                                                         |   ===================================                                            |
|                                                                         |            2000  'KOR'                                                           |
|                                                                         |            2000  'GER'                                                           |
|                                                                         |            2008  'USA'                                                           |
|                                                                         |            2008  'GER'                                                           |
|                                                                         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+

.. _isolation-level-5:

REPEATABLE READ 격리 수준
-------------------------

REPEATABLE READ 격리 수준(5)은 **snapshot isolation** 때문에 더티 읽기, 반복 불가능한 읽기 및 유령 읽기가 발생하지 않는다. 하지만 완벽하게 **serializable** 하지는 않으므로, *동시에 실행 중인 다른 트랜잭션이 없는* 트랜잭션 수행이라고 말할 수 없으며, **serializable snapshot isolation** 수준이 허용하지 않는 스큐 쓰기(write skew)와 같은 복잡한 예외가 발생할 수 있다.

스큐 쓰기는 두 개의 트랜잭션이 동시에 겹치는 데이터 셋을 읽고 겹친 각각 다른 영역의 갱신을 수행하여 다른 사용자가 수행한 갱신을 확인할 수 없는 현상이다. 직렬화 시스템에서는 한 트랜잭션이 먼저 발생하고 두 번째 트랜잭션이 첫 번째 트랜잭션의 갱신을 확인하기 때문에 이러한 현상이 발생하지 않는다.

다음과 같은 규칙이 적용된다. 

*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 삽입되는 레코드를 읽거나 수정할 수 없으며 대신 해당 레코드를 무시한다.
*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 갱신 중인 레코드를 읽을 수 있으며, 레코드의 마지막 커밋된 버전을 확인한다.
*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 갱신 중인 레코드를 수정할 수 없다.
*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 조회 중인 레코드를 수정할 수 있다.
*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 조회 중인 테이블에 레코드를 갱신/삽입할 수 있다.
*   트랜잭션 *T1* 은 다른 트랜잭션 *T2* 에서 조회 중인 테이블의 스키마를 변경할 수 없다.
*   트랜잭션 *T1* 은 트랜잭션의 수행 동안에 유효한 고유 스냅샷을 생성한다.

**예제**

다음 예제는 **snapshot isolation** 으로 인해 반복 불가능한 읽기 및 유령 읽기가 발생하지 않는 것을 보여준다. 그러나 격리 수준이 **serializable** 되어 있지 않기 때문에 스큐 쓰기는 발생할 수 있다.


+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| session 1                                                                  | session 2                                                                   |
+============================================================================+=============================================================================+
| .. code-block :: sql                                                       | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|   csql> ;autocommit off                                                    |   csql> ;autocommit off                                                     |
|                                                                            |                                                                             |
|   AUTOCOMMIT IS OFF                                                        |   AUTOCOMMIT IS OFF                                                         |
|                                                                            |                                                                             |
|   csql> SET TRANSACTION ISOLATION LEVEL 5;                                 |   csql> SET TRANSACTION ISOLATION LEVEL 5;                                  |
|                                                                            |                                                                             |
|   Isolation level set to:                                                  |   Isolation level set to:                                                   |
|   REPEATABLE READ                                                          |   REPEATABLE READ                                                           |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| .. code-block :: sql                                                       |                                                                             |
|                                                                            |                                                                             |
|   csql> CREATE TABLE isol5_tbl(host_year integer, nation_code char(3));    |                                                                             |
|   csql> CREATE UNIQUE INDEX isol5_u_idx                                    |                                                                             |
|             on isol5_tbl(nation_code, host_year);                          |                                                                             |
|   csql> INSERT INTO isol5_tbl VALUES (2008, 'AUS');                        |                                                                             |
|   csql> INSERT INTO isol5_tbl VALUES (2004, 'AUS');                        |                                                                             |
|                                                                            |                                                                             |
|   csql> COMMIT;                                                            |                                                                             |
|                                                                            |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code='AUS';                    |
|                                                                            |                                                                             |
|                                                                            |       host_year  nation_code                                                |
|                                                                            |   ===================================                                       |
|                                                                            |            2004  'AUS'                                                      |
|                                                                            |            2008  'AUS'                                                      |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| .. code-block :: sql                                                       |                                                                             |
|                                                                            |                                                                             |
|   csql> INSERT INTO isol5_tbl VALUES (2004, 'KOR');                        |                                                                             |
|   csql> INSERT INTO isol5_tbl VALUES (2000, 'AUS');                        |                                                                             |
|                                                                            |                                                                             |
|   /* able to insert new rows  */                                           |                                                                             |
|   csql> COMMIT;                                                            |                                                                             |
|                                                                            |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code='AUS';                    |
|                                                                            |                                                                             |
|                                                                            |   /* phantom read cannot occur due to snapshot isolation */                 |
|                                                                            |                                                                             |
|                                                                            |       host_year  nation_code                                                |
|                                                                            |   ===================================                                       |
|                                                                            |            2004  'AUS'                                                      |
|                                                                            |            2008  'AUS'                                                      |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| .. code-block :: sql                                                       |                                                                             |
|                                                                            |                                                                             |
|   csql> UPDATE isol5_tbl                                                   |                                                                             |
|   csql> SET host_year = 2012                                               |                                                                             |
|   csql> WHERE nation_code = 'AUS' and                                      |                                                                             |
|   csql> host_year=2008;                                                    |                                                                             |
|                                                                            |                                                                             |
|   /* able to update rows viewed by T2 */                                   |                                                                             |
|   csql> COMMIT;                                                            |                                                                             |
|                                                                            |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code = 'AUS';                  |
|                                                                            |                                                                             |
|                                                                            |   /* non-repeatable read cannot occur due to snapshot isolation */          |
|                                                                            |                                                                             |
|                                                                            |       host_year  nation_code                                                |
|                                                                            |   ===================================                                       |
|                                                                            |            2004  'AUS'                                                      |
|                                                                            |            2008  'AUS'                                                      |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|                                                                            |   csql> COMMIT;                                                             |
|                                                                            |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| .. code-block :: sql                                                       | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|   csql> SELECT * FROM isol5_tbl WHERE host_year >= 2004;                   |   csql> SELECT * FROM isol5_tbl WHERE nation_code = 'AUS';                  |
|                                                                            |                                                                             |
|       host_year  nation_code                                               |       host_year  nation_code                                                |
|   ===================================                                      |   ===================================                                       |
|            2004  'AUS'                                                     |            2000  'AUS'                                                      |
|            2004  'KOR'                                                     |            2004  'AUS'                                                      |
|            2012  'AUS'                                                     |            2012  'AUS'                                                      |
|                                                                            |                                                                             |
|   csql> UPDATE isol5_tbl                                                   |   csql> UPDATE isol5_tbl                                                    |
|   csql> SET nation_code = 'USA'                                            |   csql> SET nation_code = 'NED'                                             |
|   csql> WHERE nation_code = 'AUS' and                                      |   csql> WHERE nation_code = 'AUS' and                                       |
|   csql> host_year = 2004;                                                  |   csql> host_year = 2012;                                                   |
|                                                                            |                                                                             |
|   csql> COMMIT;                                                            |   csql> COMMIT;                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| .. code-block :: sql                                                                                                                                     |
|                                                                                                                                                          |
|   /* T1 and T2 first have selected each 3 throws and rows (2004, 'AUS'), (2012, 'AUS') overlapped.                                                       |
|    * Then T1 modified (2004, 'AUS'), while T2 modified (2012, 'AUS'), without blocking each other.                                                       |
|    * In a serial execution, the result of select query for T1 or T2, whichever executes last, would be different.                                        |
|    */                                                                                                                                                    |
|                                                                                                                                                          |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code = 'AUS';                  |
|                                                                            |                                                                             |
|                                                                            |       host_year  nation_code                                                |
|                                                                            |   ===================================                                       |
|                                                                            |            2000  'AUS'                                                      |
|                                                                            |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| .. code-block :: sql                                                       |                                                                             |
|                                                                            |                                                                             |
|   csql> ALTER TABLE isol5_tbl ADD COLUMN gold INT;                         |                                                                             |
|                                                                            |                                                                             |
|   /* unable to alter the table schema until tran 2 committed */            |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|                                                                            |   /* repeatable read is ensured while tran_1 is altering                    |
|                                                                            |    * table schema                                                           |
|                                                                            |    */                                                                       |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code = 'AUS';                  |
|                                                                            |                                                                             |
|                                                                            |       host_year  nation_code                                                |
|                                                                            |   ===================================                                       |
|                                                                            |            2000  'AUS'                                                      |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|                                                                            |   csql> COMMIT;                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code = 'AUS';                  |
|                                                                            |                                                                             |
|                                                                            |   /* unable to access the table until tran_1 committed */                   |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| .. code-block :: sql                                                       |                                                                             |
|                                                                            |                                                                             |
|   csql> COMMIT;                                                            |                                                                             |
|                                                                            |                                                                             |
|                                                                            |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | .. code-block :: sql                                                        |
|                                                                            |                                                                             |
|                                                                            |   host_year  nation_code  gold                                              |
|                                                                            |   ===================================                                       |
|                                                                            |     2000  'AUS'           NULL                                              |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+

.. _isolation-level-6:

SERIALIZABLE 격리 수준
----------------------

CUBRID 10.0의 **SERIALIZABLE** 격리 수준은 **REPEATABLE READ** 격리 수준과 동일하다. :ref:`isolation-level-5` 섹션에 설명된 대로, SNAPSHOT 격리 수준으로 인해 반복 불가능한 읽기 및 유령 읽기 현상은 발생하지 않지만 스큐 쓰기 현상은 여전히 발생할 수 있다. 스큐 쓰기를 방지하기 위해서 읽기에 대해 인덱스 키 잠금을 사용하거나, 격리 충돌을 발생할 수 있는 트랜잭션을 중단시키는 등의 다양한 SERIALIZABLE SNAPSHOT ISOLATION 방법이 있다. 향후 CUBRID 버전에서는 이런 방법 중 하나를 제공될 예정이다.

즉, 기존 버전과의 호환성을 위해 키워드는 제거되지 않았지만 SERIALIZABLE은 **REPEATABLE READ** 와 유사하다.

.. _dirty-record-flush:

CUBRID에서 더티 레코드를 다루는 방법
------------------------------------

CUBRID는 다양한 상황에서 클라이언트의 버퍼에 존재하는 더티 데이터(또는 더티 레코드)를 데이터베이스 서버로 내려쓰기(flush)한다. 아래에 명시된 상황이 아닌 경우에도 내려쓰기가 발생할 수 있다.

*   트랜잭션 커밋이 수행될 때 더티 데이터는 서버로 내려쓰기된다.
*   클라이언트의 버퍼에 적재된 데이터가 많은 경우, 일부 더티 데이터는 서버로 내려쓰기된다.
*   테이블 *A* 의 더티 데이터는 테이블 *A* 의 스키마가 갱신될 때 서버로 내려쓰기된다.
*   테이블 *A* 의 더티 데이터는 테이블 *A* 가 조회( **SELECT** )될 때 서버로 내려쓰기된다.
*   더티 데이터의 일부는 서버 함수가 호출될 때 내려쓰기될 수 있다.

트랜잭션 종료와 복구
====================

CUBRID에서 복구 프로세스를 사용하면 소프트웨어 또는 하드웨어에 오류가 발생하더라도 데이터베이스에는 영향을 미치지 않도록 할 수 있다. CUBRID에서 모든 읽기와 갱신 명령문은 원자성(atomic)을 보장한다. 이것은 명령문들이 커밋되어 데이터베이스가 갱신되거나, 커밋되지 않아 갱신이 무효화되어야 함을 의미한다. 원자성의 개념은 트랜잭션을 구성하는 연산의 집합으로 확장된다. 트랜잭션은 커밋을 성공하여 모든 영향이 데이터베이스에 영구화 되거나 아니면 롤백되어 트랜잭션의 모든 영향이 제거되어야 한다. 트랜잭션의 원자성을 보장하기 위해서 CUBRID는 모든 트랜잭션의 갱신이 디스크에 쓰여지지 않은 채 오류가 발생할 때마다 커밋된 트랜잭션의 영향을 다시 적용시킨다. 또한 CUBRID는 사이트가 실패(몇몇 트랜잭션이 커밋되지 못했거나 응용 프로그램이 트랜잭션 취소를 요청했을 수 있다)할 때마다 데이터베이스에서 부분적으로 커밋된 트랜잭션의 영향을 제거한다. 이러한 복구 기능은 응용 프로그램이 시스템 오류에 따라 어떻게 데이터베이스를 일관성 있는 상태로 되돌릴 지에 대한 부담을 덜어준다. CUBRID에서 사용되는 복구 기능은 언두/리두 로깅 기법을 기반으로 한다.

CUBRID는 하드웨어와 소프트웨어 오류가 발생하는 동안 트랜잭션의 원자성을 유지하기 위해서 자동 복구 기법을 제공한다. CUBRID의 복구 기능은 응용 프로그램 또는 컴퓨터 시스템의 오류가 발생하더라도 데이터베이스를 항상 일관된 상태로 되돌려놓기 때문에 사용자는 복구에 대한 책임을 가질 필요가 없다. 이것을 위해 CUBRID는 시스템이나 응용 프로그램의 실패 또는 사용자의 명시적 요청에 따라 커밋된 트랜잭션의 일부를 자동적으로 롤백한다. 예를 들어 **COMMIT WORK** 문이 수행되는 동안 발생한 시스템 오류는 트랜잭션이 아직 커밋되지 않았다면(사용자의 연산이 커밋되었다는 확인을 받지 못한다) 중단해야 할 것이다. 자동 중단은 커밋되지 않은 갱신을 취소함으로써 데이터베이스에 원하지 않은 변경을 야기하는 오류를 방지한다.

데이터베이스 재구동
-------------------

CUBRID는 시스템과 매체(디스크)에 오류가 발생했을 때 커밋되었거나 커밋되지 않은 트랜잭션을 복구하기 위해 로그 볼륨/파일과 데이터베이스 백업을 이용한다. 로그는 사용자가 지정한 롤백을 지원하는데도 사용된다. 로그는 CUBRID가 생성한 순차적인 파일의 모음으로 구성된다. 가장 최근의 로그를 활성 로그(active log)라고 부르며, 나머지 로그를 보관 로그(archive log)라고 부른다. 로그 파일은 활성 로그와 보관 로그 전체를 가리키는데 사용된다.

데이터베이스에 대한 모든 갱신은 로그에 기록된다. 실제로 갱신에 대한 2개의 복사본이 기록되는데, 첫 번째 복사본은 before 이미지(UNDO log)라고 불리며 사용자가 명시한 **ROLLBACK WORK** 문이 수행되는 동안이나 매체 또는 시스템에 오류가 발생했을 때 데이터를 복원하는데 사용된다. 두 번째 복사본은 after 이미지(REDO log)인데 매체 또는 시스템에 오류가 발생했을 때 갱신을 다시 적용시키는데 사용된다.

CUBRID는 활성 로그가 꽉 차면 보관 로그로 복사하여 디스크에 보존한다. 보관 로그는 시스템 장애가 발생했을 때 데이터베이스 복구를 위해 필요하다.

**정상적인 종료 또는 오류**

데이터베이스가 정상적인 종료나 장비의 오류로 다시 시작되면 CUBRID는 자동적으로 데이터베이스를 복구한다. 복구 프로세스는 데이터베이스에 빠져있는 커밋된 변화를 다시 적용하고 데이터베이스에 저장되어 있는 커밋되지 않은 변경을 제거한다. 데이터베이스 시스템의 일반적인 연산은 복구가 끝나고 난 후 재개된다. 이러한 복구 프로세스는 어떠한 보관 로그나 데이터베이스 백업도 사용하지 않는다.

데이터베이스는 **cubrid server** 유틸리티를 이용해 재구동할 수 있다.

**매체 오류**

매체에 오류가 발생 한 후에 데이터베이스를 다시 구동시키는 데는 사용자의 개입이 다소 필요하다. 첫 번째 단계는 좋은 상태로 알려진 데이터베이스의 백업을 설치하여 데이터베이스를 복원하는 것이다. CUBRID에서 가장 최근의 로그 파일(마지막 백업 이후의 것)을 설치하는 것을 필요로 한다. 이 특정 로그(보관, 활성)는 데이터베이스의 백업 복사본에 적용된다. 복원이 커밋된 후 데이터베이스는 일반적인 종료의 경우와 마찬가지로 재구동할 수 있다.

.. note::

    데이터베이스의 정보를 잃어버릴 가능성을 줄이기 위해서 보관 로그가 디스크에서 삭제되기 전에 보관 로그의 스냅샷을 만들고 이를 백업 장치에 보관할 것을 권장한다. DBA는 **cubrid backupdb**, **cubrid restoredb** 유틸리티를 사용하여 데이터베이스를 백업하고 복원할 수 있다. 이 유틸리티에 대한 상세한 내용을 보려면 :ref:`backupdb`\를 참고한다.
