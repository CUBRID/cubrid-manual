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

    ALTER TABLE code DROP s_name;
    INSERT INTO code (f_name) VALUES ('Diamond');

    COMMIT WORK;

세이브포인트와 부분 롤백
------------------------

세이브포인트(savepoint)는 트랜잭션이 진행되는 중에 수립되는데, 트랜잭션에 의해 수행되는 데이터베이스 갱신을 세이브포인트 지점까지만 롤백할 수 있도록 하기 위해서이다. 이 연산을 부분 롤백(partial rollback)이라고 부른다. 부분 롤백에서는 세이브포인트 이후의 데이터베이스 연산(삽입, 삭제, 갱신 등)은 하지 않은 것으로 되고 세이브포인트 지점을 포함하여 앞서 진행된 트랜잭션의 연산은 그대로 유지된다. 부분 롤백이 실행된 후에 트랜잭션은 다른 연산을 계속 진행할 수 있다. 또는 **COMMIT WORK** 문이나 **ROLLBACK WORK** 문으로 트랜잭션을 끝낼 수도 있다. 세이브포인트는 트랜잭션에서 수행된 갱신을 커밋하는 것이 아님을 명심해야 한다.

세이브포인트는 트랜잭션의 어느 시점에서도 만들 수 있고 몇 개의 세이브포인트라도 어떤 주어진 시점에 사용될 수 있다. 특정 세이브포인트보다 앞선 세이브포인트로 부분 롤백이 수행되거나 **COMMIT WORK** 또는 **ROLLBACK WORK** 문으로 트랜잭션이 끝나면 특정 세이브포인트는 제거된다. 특정 세이브포인트 이후에 대한 부분 롤백은 여러 번 수행될 수 있다.

세이브포인트는 길고 복잡한 프로그램을 통제할 수 있도록 중간 단계를 만들고 이름을 붙일 수 있기 때문에 유용하다. 예를 들어, 많은 갱신 연산 수행 시 세이브포인트를 사용하면 실수를 했을 때 모든 문장을 다시 수행할 필요가 없다. ::

    SAVEPOINT mark;
    mark:
    _ a SQL identifier
    _ a host variable (starting with :)

같은 트랜잭션 내에 여러 개의 세이브포인트를 지정할 때 *mark* 를 같은 값으로 하면 마지막 세이브포인트만 부분 롤백에 나타난다. 그리고 앞의 세이브포인트는 제일 마지막 세이브포인트로 부분 롤백할 때까지 감춰졌다가 제일 마지막 세이브포인트가 사용된 후 없어지면 나타난다. ::

    ROLLBACK [WORK] [TO [SAVEPOINT] mark] ;
    mark:
    _ a SQL identifier
    _ a host variable (starting with :)

앞에서는 **ROLLBACK WORK** 문이 마지막 트랜잭션 이후로 입력된 모든 데이터베이스의 갱신을 제거하였다. **ROLLBACK WORK** 문은 특정 세이브포인트 이후로 트랜잭션의 갱신을 되돌리는 부분 롤백에도 사용된다.

*mark* 의 값이 주어지지 않으면 트랜잭션은 모든 갱신을 취소하면서 종료한다. 여기에는 트랜잭션에 만들어진 모든 세이브포인트도 포함한다. *mark* 가 주어지면 지정한 세이브포인트 이후의 것은 취소되고, 세이브포인트를 포함한 이전의 것은 갱신 사항이 남는다.

다음 예제는 트랜잭션의 일부를 롤백하는 방법을 보여준다.
먼저 savepoint SP1, SP2를 설정한다.

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

위에서 'Lim Jin-Suk' 을 삭제한 것은 이후에 진행되는 rollback work to SP2 명령문에 의해서 취소되었다.
다음은 SP1으로 롤백하는 경우이다.

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

.. note:: \

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

대부분의 상용 데이터베이스 시스템과 마찬가지로 CUBRID도 데이터베이스 내의 동시성(concurrency)을 위한 기본 요소인 직렬성(serializability)을 수용한다. 직렬성이란 여러 트랜잭션이 동시에 수행될 때, 마치 각각의 트랜잭션이 순차적으로 수행되는 것처럼 트랜잭션 간 간섭이 없다는 것을 의미하며, 트랜잭션의 격리 수준(isolation level)이 높을수록 보장된다. 이러한 원칙은 원자성(atomic, 트랜잭션의 모든 영향들은 커밋되거나 롤백되어야 함)을 갖는 트랜잭션이 각각 수행된다면, 데이터베이스의 동시성이 보장된다는 가정에 기초하고 있다. CUBRID에서 직렬성은 잘 알려진 2단계 잠금(two-phase locking)  기법을 통해 관리된다.

커밋하고자 하는 트랜잭션은 데이터베이스의 동시성을 보장하고, 적합한 결과를 보장해야 한다. 여러 트랜잭션이 동시에 수행 중일 때, 트랜잭션 T1 내의 이벤트는 트랜잭션 T2에 영향을 끼치지 않아야 하며, 이를 격리성(isolation)이라 한다. 즉, 트랜잭션의 격리 수준(isolation level)은 동시에 수행되는 다른 트랜잭션으로부터 간섭받는 것을 허용하는 정도의 단위이다. 격리 수준이 높을수록 트랜잭션 간 간섭이 적으며 직렬적이고, 격리 수준이 낮을수록 트랜잭션 간 간섭이 많고 병렬적이며 동시성이 높아진다. 이러한 트랜잭션의 격리 수준에 따라 데이터베이스는 테이블과 레코드에 대해 어떤 잠금을 획득할지 결정한다. 따라서, 적용하고자 하는 서비스의 특성에 따라 격리 수준을 적절히 설정함으로써 데이터베이스의 일관성(consistency)과 동시성(concurrency)을 조정할 수 있다.

트랜잭션 격리 수준 설정을 통해 트랜잭션 간 간섭을 허용할 수 있는 읽기 연산의 종류는 다음과 같다.

*   **Dirty read** (dirty read): 트랜잭션 T1이 데이터 D를 D'으로 갱신한 후 커밋을 수행하기 전에 트랜잭션 T2가 D'을 읽을 수 있다.
*   **Non-repeatable read** (non-repeatable read, unrepeatable read): 트랜잭션 T1이 데이터를 반복 조회하는 중에 다른 트랜잭션 T2가 데이터를 갱신 혹은 삭제하고 커밋하는 경우, 트랜잭션 T1은 수정된 값을 읽을 수 있다.
*   **Phantom read** (phantom read): 트랜잭션 T1에서 데이터를 여러 번 조회하는 중에 다른 트랜잭션 T2가 새로운 레코드 E를 삽입하고 커밋한 경우, 트랜잭션 T1은 E를 읽을 수 있다.

CUBRID에서 트랜잭션 격리 수준의 기본 설정은 :ref:`isolation-level-4`\이다.

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
| :ref:`isolation-level-5` (5)   | X      | X         | O      | X                    |
+--------------------------------+--------+-----------+--------+----------------------+
| :ref:`isolation-level-4` (4)   | X      | O         | O      | X                    |
+--------------------------------+--------+-----------+--------+----------------------+

*   **READ COMMITTED**\는 Drity read를 불허하며 반복할 수 없는 읽기(unrepeatable read), 유령 읽기(phantom read)를 허용한다.
*   **REPEATABLE READ**\는 Dirty read, Non-repeatable read를 불허하며 Phantom read를 허용한다.
*   **SERIALIZABLE**\은 읽기 연산 시 트랜잭션 간 간섭을 불허한다.


.. _mvcc-snapshot:

다중 버전 동시성 제어 (Multiversion Concurrency Control)
========================================================

CUBRID 10 이전의 하위 버전들은 격리(isolation) 수준을 잘 알려진 2단계 잠금(2 Phase Locking) 프로토콜을 사용하여 관리했다. 이 프로토콜에서는, 트랜잭션은 객체를 읽기 전에 공유 잠금(shared lock)을 획득하고, 객체를 변경하기 전에는 독점 잠금(exclusive lock)을 획득하여, 충돌이 발생하는 두 연산(operation) 이 동시에 실행되지 못하도록 한다. 만약 트랜잭션 T1이 잠금을 요청하면 시스템은 요청된 잠금이 기존 잠금과 충돌이 발생하는 지를 체크한다. 만약 잠금 충돌이 발생하면,  T1 은 대기 상태에 들어가 잠금 연산이 지연된다. 만일 그 잠금을 잡고 있던 다른 트랜잭션 T2 가 그 잠금을 해제하면, 트랜잭션 T1은 재시작하여 그 잠금을 획득한다. 일단 잠금이 해제되면, 그 트랜잭션은 더 이상 그 잠금을 요청하지 않는다. 

CUBRID 10.0 은 2단계 잠금 프로토콜을 대신하여 다중버전 동시성 제어(MVCC) 프로토콜을 도입했다. 2단계 잠금 프로토콜과는 달리, MVCC 프로토콜은 병행수행되는 다른 트랜잭션이 변경하고 있는 객체를 접근하고자 하는 reader 트랜잭션을 대기시키지 않는다. 대신, MVCC 는 그 레코드를 복제함으로써 각 변경에 대한 다중 버전을 생성한다. reader 트랜잭션을 대기시키지 않는 것이 아주 중요한데, 특히, read 중심의 업무 시나리오에서 공통적으로 사용되는, 대부분의 값 읽기를 포함하는 워크로드에 아주 중요하다. 물론, 객체를 변경하기 전에는 여전히 독점 잠금이 필요하다. 

MVCC 는 또한 특정 시점의 데이터베이스의 일관성 있는 관점을 제공하며, 다른 동시성 제어 기법보다 더 적은 성능 비용으로 진정한  snapshot isolation을 제공하는 것으로 유명하다. 

다중 버전, 가시성, 스냅샷
=========================

MVCC는 각 데이터베이스 레코드에 대해 다중 버전을 유지한다. 레코드의 각 버전은 그 레코드의 삽입자 또는 삭제자에 의해 MVCCID 로 표시된다. MVCCID 는 각 변경 트랜잭션을 유일하게 구분하는 ID 이다. 이런 식별자는 각 변경자를 식별하는 데 아주 유용하고 변경에 대한 시점을 지정하는 데 유용하다. 

트랜잭션 T1이 새 레코드를 삽입할 때, 그 트랜잭션은 그 레코드의 첫 버전을 생성하고 그것에 대한 유일한 식별자 MVCCID1 를 삽입ID 로 설정한다. 이  MVCCID는 레코드 헤더의 메타 데이터로 저장된다. 

.. image:: /images/transaction_inserted_record.png

트랜잭션 T1 이 완료(commit)할 때까지는 다른 트랜잭션은 이 레코드의 새 값을 보지 못한다. MVCCID가 그 레코드의 변경자를 식별하는 데 도움을 주며, 변경 시점을 지정하게 해준다. 이렇게 함으로써 다른 트랜잭션이 그 변경에 대해서 타당한 지 아닌 지 알수 있도록 해준다. 이러한 경우에, 이 레코드를 체크하는 임의의 트랜잭션은 MVCCID1 을 발견하게 되고, 그 변경자가 여전히 활성 상태인 것을 알게 됨으로써, 그 변경은 여전히 자신이 참조할 수 있는 상태가 아니라는 것을 알게된다. 

트랜잭션 T1 이 완료한 이후에는, 새 트랜잭션 T2가 T1이 변경했던 레코드를 참조할 수 있고, 삭제하기로 결정한다. T2는 실제로 그 레코드를 삭제하지는 않음으로써 다른 트랜잭션이 참조하도록 허용한다. 대신에, 그 레코드에 대한 독점 잠금을 획득하여 다른 트랜잭션이 변경하지 못하도록 하고, 삭제되었음을 그 레코드에 표시한다. 이 트랜잭션은 삭제 시 또 다른 MVCCID를 부여함으로써 다른 트랜잭션들이 그 레코드의 삭제자를 식별할 수 있도록 한다. 

.. image:: /images/transaction_deleted_record.png

만일 트랜재션 T2가 한 레코드 값을 변경하고자 하면, 기존 버전과 유사한 새 버전을 생성해야 한다. 두 버전은 트랜잭션 T2의 MVCCID로 표시되며, 기존 버전은 삭제용, 새 버전은 삽입용으로 표시된다. 기존 버전은 또한 새 버전에 대한 링크를 저장한다. 이러한 레코드의 관계는 다음 그림과 같이 표현된다. 

.. image:: /images/transaction_updated_record.png

현재, T2만이 레코드의 새 버전을 볼 수 있으며, 반면에 다른 모든 트랜잭션들은 기존의 첫 버전만을 참조하게 될 것이다. 실행 중인 트랜잭션에 의해서 해당 버전을 볼 수 있느냐 없는냐 하는 특성은 가시성(visibility)이라 한다. 이 가시성은 각 트랜잭션에 상대적이며, 일부 트랜잭션은 그 버전을 볼 수 있고 다른 트랜잭션들은 볼 수 없게 된다. 

트랜잭션 T2가 레코드를 변경한 후 완료하지는 않은 상태에서 시작하는 트랜잭션 T3는 변경된 레코드의 새 버전을 보지 못한다. 심지어 T2가 완료된 이후에도 마찬가지다. 한 버전의 T3에 대한 가시성은 T3가 시작할 때 그 버전의 삽입자와 삭제자의 상태에 달려있다. 또한, T3의 생존기간 동안에 동일하게 유지된다. 

사실상, 데이터베이스의 모든 버전의 가시성은 트랜잭션이 시작한 이후에 발생한 변경에 의존적이지는 않다. 더구나, 추가된 새 버젼은 그 트랜잭션에게는 무시된다. 결과적으로, 데이터베이스에서 모든 가시적인 버전의 집합은 변하지 않고 남아있게 되며 그 트랜잭션에 대한 스냅샷을 형성한다.  그러므로, 이 MVCC에 의해서 스냅샷 격리(snapshot isolation)가 제공되며 각 트랜잭션에서 수행되는 모든 read 질의문이 데이터베이스 일관성있는 뷰를 보는 것이 보장된다. 

CUBRID 10.0 에서, 스냅샷은 모든 타당하지 못한 MVCCID에 대한 필터(filter)이다. 스냅샷이 지정되기 전에 완료되지 않은 트랜잭션의 MVCCID는 모두 타당하지 못하다. 새 트랜잭션이 시작할 때마다 스냅샷 필터를 변경하는 것을 피하기 위해서, 스냅샷은 두 개의 경계 MVCCID로 결정된다: 즉, 활성 트랜잭션중 가장 적은 MVCCID와 완료된 트랜잭션중 가장 큰 MVCCID로 결정된다. 경계 사이에 존재하는 활성 MVCCID 의 목록만이 저장된다. 완료 트랜잭션 중 가장 큰 MVCCID 보다 큰 MVCCID를 갖도록 스냅샷이 보장된 이후에 시작한 트랜잭션은 자동적으로 타당하지 못한 것으로 결정된다. 가장 작은 활성 MVCCID보다 적은 임의의 MVCCID는 자동적으로 타당한 것으로 결정된다. 

버전 가시성을 결정하는 스냅샷 필터 알고리즘은 삽입과 삭제에 사용된 MVCCID 표시자를 질의하여 처리한다. 

+--------------------------------------+-----------------------------------------------------+
|                                      | Delete MVCCID                                       |
|                                      +--------------------------+--------------------------+
|                                      | Valid                    | Invalid                  |
+--------------------+-----------------+--------------------------+--------------------------+
| Insert MVCCID      | Valid           | Not visible              | Visible                  |
|                    +-----------------+--------------------------+--------------------------+
|                    | Invalid         | Impossible               | Not visible              |
+--------------------+-----------------+--------------------------+--------------------------+

Table 설명:

* **Valid Insert MVCCID, valid delete MVCCID:** 스냅샷 이전에 버전 삽입/삭제 (그리고 완료됨), 그러므로 참조 가능.
* **Valid Insert MVCCID, invalid delete MVCCID:** 버전 삽입 및 완료, 그러나 삭제 안됨 또는 최근 삭제됨, 그러므로 가시적.
* **Invalid Insert MVCCID, invalid delete MVCCID:** 스냅샷 이전에 삽입자 완료 안한 상태, 레코드 삭제 안됨 또는 완료 안됨, 그러므로 가시적이지 않음.
* **invalid Insert MVCCID, valid delete MVCCID:** 삽입자는 완료 안함, 삭제자는 완료함 - 불가능 경우. 만일 삭제자가 삽입자와 같지 않다면, 버전 보지 못함. 만일 같다면, 삽입/삭제 MVCCID 는 타당 또는 타당하지 않음.

이제 스냅샷이 어떻게 동작하는 지 확인해 보자 (**REPEATABLE READ** 격리 수준을 사용하여 전체 트랜잭션 동안의 동일 스냅샷 유지)

**예제1: 새 레코드 추가**

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

**예제 2: 레코드 삭제**

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

**예제3: 레코드 변경**

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

**예제4: 서로 다른 버전은 서로 다른 트랜잭션에 보일 수 있음**

+-------------------------------------------------------------------+----------------------------------------+----------------------------------------+
| session 1                                                         | session 2                              | session 3                              |
+===================================================================+========================================+========================================+
| .. code-block:: sql                                               |                                        |                                        |
|                                                                   |                                        |                                        |
|   csql> INSERT INTO tbl VALUES (2008, 'AUS');                     |                                        |                                        |
|   csql> COMMIT WORK;                                              |                                        |                                        |
|                                                                   |                                        |                                        |
+-------------------------------------------------------------------+----------------------------------------+----------------------------------------+
| .. code-block:: sql                                               | .. code-block:: sql                    |                                        |
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

각 변경에 대한 새 버전 생성과 삭제에 대한 기존 버전을 유지하는 것은 데이터베이스 크기를 무제한으로 필요로 하게 될 것이고, 데이터베이스에 대한 아주 중요한 이슈가 될 수 있다. 그러므로, 클린업 시스템이 필요하며, 오래된 불필요한 데이터를 제거하고, 그 데이터가 차지하고 있던 공간을 회수해야 한다. 

각각의 레코드 버전은 다음과 같은 동일한 단계를 거치게 된다. 

1. 새로 삽입되었으나 아직 완료안된 상태: 삽입자에게만 보임
2. 삽입 완료 상태: 이전 트랜잭션에는 보이지 않고, 이후 트랜잭션에는 보임
3. 삭제되었으나 완료안된 상태: 다른 트랜잭션에 보이고, 삭제가 자신에게는 보이지 않음 
4. 삭제 완료 상태: 이전 트랜잭션에 여전히 보이고, 이후 트랜잭션에 보이지 않음
5. 모든 활성 트랜재션에 보이지 않는 상태
6. 데이터베이스에서 제거되는 상태

이 클린업 시스템의 역할은 단계 5와 6에서 버전을 제거한다. 이 클린업 시스템을 CUBRID에서는 VACCUM 이라 불린다. 

이 **VACCUM** 시스템은 세 가지 원칙 하에서 개발되었다. 

* **VACCUM** 은 정확하고 완변해야 한다. 일부 트랜잭션에 보이는 데이터를 절대 제거하면 안되며, 오래된 불필요한 데이터의 제거를 놓치면 안된다. 
* **VACCUM** 은 이산적(discrete) 이어야 한다. 클린업 프로세스는 데이터베이스의 상태를 변경하는 것이기 때문에, 활성 트랜잭션의 실행을 일정 부분 간섭이 발생할 수 있다. 그러나, 이 간섭이 가능한 한 최소화되어야 한다. 
* **VACCUM** 은 빠르고 효율적이어야 한다. 너무 느리거나 지지부진하면, 데이터베이스의 상태가 악화될 수 있고, 이로 인해 전체적인 성능이 영향받을 수 있다. 

이러한 세 가지 원칙을 염두에 두고서, **VACCUM** 은 기존 복구 로깅을 사용하여 구현된다. 이유는 다음과 같다. 

. 힙(heap)과 인덱스의 변경에 대해서, 데이터 주소가 복구 데이터에 유지된다. 이로 인해서, VACCUM은 데이터베이스를 스캔하는 대신에 직접 해당 객체에 접근할 수 있다.
. 로그 데이터를 처리하는 것은 활성 작업자와의 간섭이 드물다. 

로그 데이터에 MVCCID를 추가함으로써, 로그 복구가 VACCUM에 적용되었다. MVCCID에 기반해서, VACCUM은 로그 엔트리가 처리 준비가 되었는 지 결정할 수 있다. 여전히 활성 트랜잭션에 보이는 MVCCID는 처리할 수 없다. 적절한 시점에, 각 MVCCID는 충분히 오래되어 불필요하게 되고, 그 MVCCID를 사용한 모든 변경이 참조할 수 없게 된다. 

각 트랜잭션은 참조할 수 있는 가장 오래된 활성 MVCCID를 유지한다. 모든 실행 트랜잭션이 활성이라고 보는 가장 오래된 MVCCID가 모든 트랜잭션의 가장 오래된 MVCCID로 결정된다. 이 값 이하의 어떤 MVCCID도 더 이상 참조될 수 없는 상태가 되며 VACCUM이 클린업할 수 있는 상태가 된다. 

VACCUM 병행 실행
++++++++++++++++

VACCUM의 세번째 원칙에 따르면, VACCUM은 빨라야 하며 활성 작업자에 뒤쳐져서는 안된다. 명확한 것은 시스템 워크로드가 심하면 한 쓰레드가 모든 VACCUM 작업을 처리할 수 없다. 그러므로, 병행 처리되어야 한다. 

병행 처리가 되려면, 로드 데이터는 고정 길이의 블럭으로 분할되어야 한다. 각 블럭은 적절한 시점에 하나의 VACCUM 작업을 생성한다. 적절한 시점이란, 그 블럭에 로그된 모든 연산이 클린업될 수 있는 상태를 의미하는, 즉, 가장 최근의 MVCCID가 클린업될 수 있는 시점을 말한다. VACCUM 작업자(쓰레드)는 로그 블럭에서 발견된 연관된 로그 엔트리에 기반하여 데이터베이스를 클린업할 수 있으며, 여러 개의 VACCUM 작업자가 VACCUM 작업을 픽업하여 처리한다. 로그 블럭의 추적과 VACCUM 작업을 생성하는 것은 VACCUM Master에 의해서 처리된다. 

VACUUM 데이터 
+++++++++++++

로그 블럭에서 집계된 데이터는 VACCUM 데이터 파일에 저장된다. 연산별로 실행되는 VACCUM 작업은 시점적으로 나중에 실행될 수 있기 때문에, 그 데이터는 그 작업이 실행될 수 있을 때까지 저정되어야 한다. 설사 서버에 장애가 발생하더라도 그 데이터는 남아 있어야 한다. 어떤 연산도 leak 이 발생하거나 클린업되지 못하면 안된다. 만일 서버에 장애가 발생하면, 그 작업은 두번 실행될 수도 있으나, 전혀 실행되지 않는 것보다 낫다고 할 수 있다. 

클린업 작업이 성공적으로 수행된 후에는, 처리된 로그 블럭에 집계된 데이터는 제거된다. 

집계된 로그 블럭 데이터는 VACCUM 데이터로 바로 추가되지는 않는다. latch-free 버퍼를 사용함으로써, (로그 블럭과 집계된 데이터를 생성하는) 활성 작업 쓰레드가 VUCCUM 시스템과 동기화하는 것을 피하도록 해준다. VACCUM Master 가 주기적으로 깨어나서, 버퍼에 있는 모든 데이터를 VACCUM 데이터로 덤프하고, 이미 처리된 데이터를 제거하고, (자원이 이용 가능하면) 새 작업을 생성한다. 

VACUUM 작업 
+++++++++++

VACCUM 작업 실행 단계는 다음과 같다. 

1. 로그 사전 페치 : VACUUM Master 또는 작업 쓰레드가 그 작업에 의해 처리될 로그 페이지를 사전 페치한다. 
2. 각 로그 레코드에 대해 다음 단계를 반복한다. 

	1. 로그 레코드 판독(read)
	2. 삭제된 파일 체크. 만일 로그 레코드가 삭제된 파일을 가리키고 있으면, 다음 로그 레코드로 진행
	3. 인덱스 클린업을 실행하고 heap OID를 수집한다. 

		. 만일 로그 레코드가 인덱스에 속하면, 즉시 클린업을 실행한다. 
		. 만일 로그 레코드가 heap에 속하면, 나중에 클린업될 OID를 수집한다. 

3. 수집된 OID를 기반으로 heap 클린업을 수행한다. 
4. 작업을 완료한다. 해당 작업을 완료한 것으로 VACCUM 데이터에 표시한다.

로그 페이지 판독을 용이하게 하고 클린업 작업 식행을 최적화하기 위해서 여러 조치가 취해졌다. 

삭제된 파일 추적
++++++++++++++++

트랜잭션이 테이블이나 인덱스를 삭제할 때, 일상적으로 영향받는 테이블에 잠금을 걸어서 다른 트랜잭션이 접근하는 것을 방지한다. 활성 작업자들과는 반대로, VACCUM 작업자는 최소한으로 유지되어야 하고, VACCUM 시스템은 청소할 데이터가 남아 있는 한 멈추어서는 안된다. 게다가, VACCUM은 청소가 필요한 데이터를 어떤 것도 간과해서는 안된다. 

1. **VACCUM** 은 삭제된 테이블이나 인덱스에 속하는 파일에 대해 삭제 트랜잭션이 완료할 때 까지는 청소하는 것을 멈추어서는 안된다. 설사 트랜잭션이 하나의 테이블을 삭제하더라도, 그 파일은 바로 삭제되지 않고 여전히 접근될 수 있다. 실제적인 삭제는 그 트랜잭션의 완료 이후로 연기된다. 
2. 실제적인 파일 삭제 전에는 **VACCUM** 시스템에 알려져야 한다. 삭제자는 **VACCUM** 시스템에 알림 경보를 보내고 확인될 때 까지 대기해야 한다. VACCUM 시스템은 아주 짧은 반복 작업을 하고 새로 삭제된 파일을 비번하게 체크한다. 그러므로, 삭제자는 길게 대기하지 않는다. 

파일이 삭제된 이후에, **VACCUM** 은 그 파일에 속하는 모든 발견된 로그 엔트리들을 무시할 것이다. 그 파일 식별자는 삭제 순간을 표시하는 MVCCID 와 쌍을 이루어 파일에 저장되어 있게 되고, **VACCUM** 시스템이 그 파일을 제거하는 것이 안전하다고 결정할 때 까지 유지된다. 그 제거 안전성은 아직 청소되지 않은 MVCCID 중 가장 적은 것에 기반하여 결정된다. 

.. _lock-protocol:

잠금 프로토콜
=============

CUBRID는 동시성 제어를 위해 2단계 잠금 프로토콜(2-phase locking protocol, 2PL)을 사용하여 트랜잭션 스케줄을 관리한다. 이는 트랜잭션이 사용하는 자원, 즉 객체에 대해 상호 배제 기능을 제공하는 기법이다. 확장 단계(growing phase)에서는 트랜잭션들이 잠금 연산만 수행할 수 있고 잠금 해제(unlock) 연산은 수행할 수 없다. 축소 단계(shrinking phase)에서는 트랜잭션들이 잠금 해제(unlock) 연산만 수행할 수 있고 잠금 연산은 수행할 수 없다. 즉, 트랜잭션 T1이 특정 객체에 대해 읽기 또는 갱신 연산을 수행하기 전에 반드시 잠금 연산을 먼저 수행하고, T1을 종료하기 전에 잠금 해제 연산을 수행해야 한다.

잠금의 단위
-----------

CUBRID는 잠금의 개수를 줄이기 위해서 단위 잠금(granularity locking) 프로토콜을 사용한다. 단위 잠금 프로토콜에서는 잠금 단위의 크기에 따라 계층으로 모델화되며, 행 잠금(row lock), 테이블 잠금(table lock), 데이터베이스 잠금(database lock)이 있다. 이때, 단위가 큰 잠금은 작은 단위의 잠금을 내포한다.

잠금을 설정하고 해제하는 과정에서 발생하는 성능 손실을 잠금 비용(overhead)이라고 하는데, 큰 단위보다 작은 단위의 잠금을 수행할 때 이러한 잠금 비용이 높아지고 대신 트랜잭션 동시성은 향상된다. 따라서, CUBRID는 잠금 비용과 트랜잭션 동시성을 고려하여 잠금 단위를 결정한다. 예를 들어, 한 트랜잭션이 테이블의 모든 행들을 조회하는 경우 행 단위로 잠금을 설정/해제하는 비용이 너무 높으므로 차라리 해당 테이블에 잠금을 설정한다. 이처럼 테이블에 잠금이 설정되면 트랜잭션 동시성이 저하되므로, 동시성을 보장하려면 풀 스캔(full scan)이 발생하지 않도록 적절한 인덱스를 사용해야 할 것이다.

이와 같은 잠금 관리를 위해 CUBRID는 잠금 에스컬레이션(lock escalation) 기법을 사용하여 설정 가능한 단위 잠금의 수를 제한한다. 예를 들어, 한 트랜잭션이 행 단위에서 특정 개수 이상의 잠금을 가지고 있으면 시스템은 계층적으로 상위 단위인 테이블에 대해 잠금을 요청하기 시작한다. 단, 상위 단위로 잠금 에스컬레이션을 수행하기 위해서는 어떤 트랜잭션도 상위 단위 객체에 대한 잠금을 가지고 있지 않아야 한다. 그래야만 잠금 변환에 따른 교착 상태(deadlock)를 예방할 수 있다. 이때, 작은 단위에서 허용하는 잠금 개수는 시스템 파라미터 **lock_escalation** 을 통해 설정할 수 있다.

.. _lock-mode:

잠금 모드의 종류와 호환성
-------------------------

CUBRID는 트랜잭션이 수행하고자 하는 연산의 종류에 따라 획득하고자 하는 잠금 모드를 결정하며, 다른 트랜잭션에 의해 이미 선점된 잠금 모드의 종류에 따라 잠금 공유 여부를 결정한다. 이와 같은 잠금에 대한 결정은 시스템이 자동으로 수행하며, 사용자에 의한 수동 지정은 허용되지 않는다. CUBRID의 잠금 정보를 확인하기 위해서는 **cubrid lockdb** *db_name* 명령어를 사용하며, 자세한 내용은 :ref:`lockdb` 을 참고한다.

*   **Shared lock (shared lock, S_LOCK, no longer used with MVCC protocol)** 

    객체에 대해 읽기 연산을 수행하기 전에 획득하며, 여러 트랜잭션이 동일 객체에 대해 획득할 수 있는 잠금이다.

    트랜잭션 T1이 특정 객체에 대해 읽기 연산을 수행하기 전에 공유 잠금을 먼저 획득한다. 이때, 트랜잭션 T2, T3은 동시에 그 객체에 대해 읽기 연산을 수행할 수 있으나 갱신 연산을 수행할 수 없다.
    
    .. note::

        *   Shared locks are rarely used in CUBRID 10.0, because of MVCC. It is still used, mostly in internal database operati     ons, to protect rows or index keys from being modified.

*   **배타 잠금(Exclusive lock, X_LOCK)**

    객체에 대해 갱신 연산을 수행하기 전에 획득하며, 하나의 트랜잭션만 획득할 수 있는 잠금이다.

    트랜잭션 T1이 특정 객체 X에 대해 갱신 연산을 수행하기 전에 배타 잠금을 먼저 획득하고, 갱신 연산을 완료하더라도 트랜잭션 T1이 커밋될 때까지 배타 잠금을 해제하지 않는다. 따라서, 트랜잭션 T2, T3은 트랜잭션 T1이 배타 잠금을 해제하기 전까지는 X에 대한 읽기 연산도 수행할 수 없다.

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

        DDL(ALTER/CREATE/DROP)을 실행하는 동안 획득되며 다른 트랜잭션이 수정된 스키마에 접근하는 것을 방지한다.

    Some DDL operations like **ALTER**, **CREATE INDEX** do not acquire **SCH_M_LOCK** directly. For example, CUBRID operates type checking about filtering expression when you create a filtered index; during this term, the lock which is kept to the target table is **SCH_S_LOCK** like other type checking operations. The lock is then upgraded to **SIX_LOCK** (other transactions are prevented from modifying target table rows, but they can continue reading them), and finally **SCH_M_LOCK** is requested to change the table schema. The method has a strength to increase the concurrency by allowing other transaction's operation during DDL operation's compilation and execution.

    하지만 이 방식은 같은 테이블에 동시에 DDL 연산을 수행할 때 교착 상태를 회피할 수 없다는 단점 또한 존재한다. 인덱스를 로딩함으로 인한 교착 상태의 예는 다음과 같다.

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
   
.. note:: 잠금에 대해 요약하면 다음과 같다.

    *   잠금 대상 객체에는 행(instance), 키(key), 스키마(class)가 있다. 잠금 대상 객체를 기준으로 잠금의 종류를 나누면 다음과 같다.

        *   행 잠금: **S_LOCK**, **X_LOCK**
        
        *   intension/schema 잠금: **IX_LOCK**, **IS_LOCK**, **SIX_LOCK**, **SCH_S_LOCK**, **SCH_M_LOCK**
        
    *   행 잠금과 intension/schema 잠금은 서로에게 영향을 끼친다.
        
위에서 설명한 잠금들의 호환 관계(lock compatibility)를 정리하면 아래의 표와 같다. 호환된다는 것은 잠금 보유자(lock holder)가 특정 객체에 대해 획득한 잠금과 중복하여 잠금 요청자(lock requester)가 잠금을 획득할 수 있다는 의미이다.

**잠금 호환성**

*   **NULL**\: lock이 존재하는 상태.

(O: TRUE, X: FALSE)

+----------------------------------+-----------------------------------------------------------------------------------------------+
|                                  | **Lock holder**                                                                               |
|                                  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                                  | **NULL**  | **SCH-S** | **IS**    | **S**     | **IX**    | **SIX**   | **X**     | **SCH-M** |
+----------------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
| **Lock requester**   | **NULL**  | O         | O         | O         | O         | O         | O         | O         | O         |
|                      |           |           |           |           |           |           |           |           |           |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SCH-S** | O         | O         | O         | O         | O         | O         | O         | O         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **IS**    | O         | O         | O         | O         | O         | O         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **S**     | O         | O         | O         | O         | X         | X         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **IX**    | O         | O         | O         | X         | O         | X         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SIX**   | O         | O         | O         | X         | X         | X         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **X**     | O         | O         | X         | X         | X         | X         | X         | X         |
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SCH-M** | O         | X         | X         | X         | X         | X         | X         | X         |
+----------------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+

**잠금 변환 테이블**

*   **NULL**\: The status that any lock exists.

+----------------------------------+-----------------------------------------------------------------------------------------------+
|                                  | **Granted lock mode**                                                                         |
|                                  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                                  | **NULL**  | **SCH-S** | **IS**    | **S**     | **IX**    | **SIX**   | **X**     | **SCH-M** |
+----------------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
| **Requested lock**   | **NULL**  | NULL      | SCH-S     | IS        | S         | IX        | SIX       | X         | SCH-M     |    
| **mode**             +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SCH-S** | SCH-S     | SCH-S     | IS        | S         | IX        | SIX       | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **IS**    | IS        | IS        | IS        | S         | IX        | SIX       | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **S**     | S         | S         | S         | S         | SIX       | SIX       | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **IX**    | IX        | IX        | IX        | SIX       | IX        | SIX       | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SIX**   | SIX       | SIX       | SIX       | SIX       | SIX       | SIX       | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **X**     | X         | X         | X         | X         | X         | X         | X         | SCH-M     |    
|                      +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
|                      | **SCH-M** | SCH-M     | SCH-M     | SCH-M     | SCH-M     | SCH-M     | SCH-M     | SCH-M     | SCH-M     |    
+----------------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+

lock을 사용한 예
++++++++++++++++

다음 몇개의 예에 걸쳐서 REPEATABLE READ(5) 격리 수준이 사용될 것이다. READ COMMITTED는 열을 갱신하는데 다른 원칙을 가지고 있으며 다음 장에서 다루기로 한다 (여기를 참조)
다음 예제들은 기존의 lock을 보여주기 위해서 lockdb 유틸리티를 사용할 것이다.

**Lock 예:**
다음의 예에서 REPEATABLE READ(5)이 사용될 것이며, 이것은 같은 열에 대해서 읽기와 쓰기가 블록되지 않는다는 것을 증명할 것이다. 그리고 충돌하는 갱신이 시도될 것인데, 두번째 갱신자가 블록된다. 트랜잭션 T1이 커밋될때, T2는 블럭에서 해제된다 하지만 격리 수준의 제약 때문에 갱신은 허용되지 않는다. 만약 T1이 롤백할 수 있는데, 이 경우 T2는 갱신을 진행할 수 있다.
 
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
| T1                                                      | T2                                                      | Description                                                                |
+=========================================================+=========================================================+============================================================================+
| .. code-block :: sql                                    | .. code-block :: sql                                    | AUTOCOMMIT OFF and REPEATABLE READ                                         |
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
| .. code-block :: sql                                    |                                                         | a = 10인 첫번째 버전의 열이 잠기고 갱신됨. a = 90 인 열의 새로운 버전이    |
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
|                                                         | .. code-block :: sql                                    | 트랜잭션 T2가 모든 열을 읽음, a <= 20. T1이 갱신에 대해 커밋을 하지 않았기 |
|                                                         |                                                         | 때문에 T2는 a = 10인 열을 보려고 계속 시도하지만 잠금을 하지는 않음 .::    |
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
|                                                         | .. code-block :: sql                                    | 트랜잭션 T2 a <= 20인 모든 열을 갱신을 시도한다. 이것은                    |
|                                                         |                                                         | T2의 잠금 클래스를 IX_LOCK로 업그레이드하고, 첫번째 잠금으로써             |
|                                                         |                                                         | row = 10의 갱신을 시도하는 것을 의미한다. 하지만, T1 이 이미 잠근          |
|                                                         |   csql> UPDATE tbl                                      | 상태이고, 따라서 T2 가 블럭될 것이다. ::                                   |
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
|                                                         | ::                                                      | T2가 블럭에서 해제되어 T1이 이미 갱신한 개체의 갱신을 시도한다.            |
|                                                         |                                                         | REPEATABLE READ 격리 수준에서 이것은 허용되지 않고                         |
|                                                         |     ERROR: 동시 갱신의 충돌로                           | 오류가 전송된.                                                             |
|                                                         |     직렬성 위반                                         |                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+

고유한 제약을 보호하기 위한 잠금
--------------------------------

이전 버전의 CUBRID에서 사용한 2단계 잠금 프로토콜 (2PL)은 고유한 제약 조건을 유지하고 높은 격리 제약을 위해서 인덱스 키를 잠그는데 사용되었다. CUBRID 10.0에서 키 잠금은 제거되었다. 격리 수준의 제약은 다중 버전 동시성 제어 (MVCC) 스냅샷으로 해결되었다, 하지만 고유한 제약은 여전히 어떤 형태의 보호를 필요로 한다.

다중 버전 동시정 제어 (MVCC)를 이용하여, 고유한 인덱스는, 열과 같은 형태로, 동시에 여러 개의 버전을 유지할 수 있으며, 각각이 서로 다른 트랜잭션에 보이게 할 수있다. 하나는 최종 버전이다, 반면 다른 버전들은 보이지 않게 되어서 **VACUUM**에 의해 제거될 수 있으며 이때까지는 임시적으로 유지될 수 있다. 고유한 제약 조건을 유지하기 위한 조건은 어떤 키를 수정하고자 하는 모든 트랜잭션은 해당 키의 존재하는 마지막 버전의 잠금을 획득해야만 한다는 것이다.

아래의 예는 **REPEATABLE READ** 격리 수준을 사용하였는데 이는 잠금이 고유한 제약을 위반하는 방법을 보여주기 위함이다.

+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
| T1                                                      | T2                                                      | Description                                                                |
+=========================================================+=========================================================+============================================================================+
| .. code-block :: sql                                    | .. code-block :: sql                                    | AUTOCOMMIT OFF and REPEATABLE READ                                         |
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
| .. code-block :: sql                                    |                                                         | T1이 새로운 열을 테이블에 삽입하고 그것을 잠금. 키 20은 따라서             |
|                                                         |                                                         | 보호.                                                                      |
|   csql> INSERT INTO tbl VALUES (20, 20);                |                                                         |                                                                            |
+---------------------------------------------------------+---------------------------------------------------------+----------------------------------------------------------------------------+
|                                                         | .. code-block :: sql                                    | T2 또한 테이블에 새로운 열을 삽입하고 그것을 잠금. 하지만, T2가            |
|                                                         |                                                         | 주키에 그것을 삽입하려고 할 때, 키 20이 이미 있는 것이 발견된다. T2는      |
|                                                         |    INSERT INTO tbl VALUES (20, 120);                    | T1ㅣ 삽입한 기존의 오브젝트의 잠금을 시도하고, T1이                        |
|                                                         |                                                         | 커밋할 때까지 블럭된다. ::                                                 |
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
|                                                         | ::                                                      | T2 가 해제된다, 커밋된 키를 찾지만                                         |
|                                                         |                                                         | 고유키 제약이 발생된다.                                                    |
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

이전 버전들과 달리 CUBRID 10.0은 더 이상 인덱스를 읽고 쓰는데 인덱스 키 잠금을 사용하지 않는다, 따라서 교착상태의 발생이 현저하게 줄어들었다. 교착상태가 자주 발생하지 않는 또 하나의 이유는 이전의 CUBRID 버전들이 범위의 인덱스를 읽는 것에 높은 격리 수준을 사용하는데 이것이 많은 오브젝트들을 잠글 수 있기 때문이다, 반면 이러한 경우 CUBRID 10.0은 더 이상 잠금을 사용하지 않는다.

하지만, 교착상태는 같은 오브젝트들을 서로 다른 순서로 갱신하는 두개의 트랜잭션에서 아직도 발생할 수 있다.

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

즉, 위의 잠금 에러 메시지는 "다른 트랜잭션들이 *tbl* 테이블의 특정 행에 잠금을 점유하고 있으므로, *host1* 호스트에서 수행된 트랜잭션은 다른 트랜잭션들이 종료되기를 기다리다가 타임아웃 시간이 경과되어 롤백되었다."로 해석할 수 있다. 만약, 에러 메시지에 명시된 트랜잭션의 잠금 정보를 확인하고자 한다면, **cubrid lockdb** 유틸리티를 통해 현재 잠금을 점유 중인 클라이언트의 트랜잭션 ID 값, 클라이언트 프로그램 이름, 프로세스 ID(PID)를 확인할 수 있다. 이에 관한 상세한 설명은 :ref:`lockdb` 을 참고한다.

이처럼 트랜잭션의 잠금 정보를 확인한 후에는 SQL 로그를 통해 커밋되지 않은 질의문을 확인하여 트랜잭션을 정리할 수 있다. SQL 로그를 확인하는 방법은 :ref:`broker-logs`\ 를 참고한다.

또한, **cubrid killtran** 유틸리티를 통해 문제가 되는 트랜잭션을 강제 종료할 수 있으며, 이에 관한 상세한 설명은 :ref:`killtran` 를 참고한다.

.. _transaction-isolation-level:

트랜잭션 격리 수준
==================

트랜잭션의 격리 수준은 트랜잭션이 동시에 진행 중인 다른 트랜잭션에 의해 간섭받는 정도를 의미하며, 트랜잭션 격리 수준이 높을수록 트랜잭션 간 간섭이 적으며 직렬적이고, 트랜잭션 격리 수준이 낮을수록 트랜잭션 간 간섭은 많으나 높은 동시성을 보장한다. 사용자는 적용하고자 하는 서비스의 특성에 따라 격리 수준을 적절히 설정함으로써 데이터베이스의 일관성(consistency)과 동시성(concurrency)을 조정할 수 있다.

.. note:: 지원되는 모든 격리 수준에서 트랜잭션은 복구 가능하다. 이는 트랜잭션이 끝나기 전에는 갱신을 커밋하지 않기 때문이다.

.. _set-transaction-isolation-level:

격리 수준 설정
--------------

You can set the level of transaction isolation by using **isolation_level** and the **SET TRANSACTION** statement in the **$CUBRID/conf/cubrid.conf**. The level of **REPEATABLE READ CLASS** and **READ COMMITTED INSTANCES** are set by default, which indicates the level 4 through level 4 to 6 (levels 1 to 3 were used by older versions of CUBRID and are now obsolete). For details, see :ref:`database-concurrency`. ::

    SET TRANSACTION ISOLATION LEVEL isolation_level_spec ;
    
    isolation_level_spec:
        SERIALIZABLE | 6
        REPETABLE READ | 5
        READ COMMITTED | 4

**예제 1** ::

    vi $CUBRID/conf/cubrid.conf
    ...

    isolation_level = 4
    ...

    -- or 

    isolation_level = "TRAN_READ_COMMITTED"

**예제 2** ::

    SET TRANSACTION ISOLATION LEVEL 4;
    -- 또는
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;

**CUBRID가 지원하는 격리 수준**

+-----------------------+-----------------------------------------------------------------------------------------------------------------------------------+
| 격리 수준 이름        | 설명                                                                                                                              |
+=======================+===================================================================================================================================+
| READ COMMITTED (4)    | 트랜잭션 T1이 테이블 A를 조회하는 중에 다른 트랜잭션 T2가 테이블 A의 스키마를 갱신할 수 없다.                                     |
|                       | 트랜잭션 T1이 레코드 R을 여러 번 조회하는 중에, 다른 트랜잭션 T2가 갱신하고 커밋한 R' 읽기(반복 불가능한 읽기)를 경험할 수 있다.  |
+-----------------------+-----------------------------------------------------------------------------------------------------------------------------------+
| REPEATABLE READ (5)   | 트랜잭션 T1이 테이블 A를 조회하는 중에 다른 트랜잭션 T2가 테이블 A의 스키마를 갱신할 수 없다.                                     |
|                       | 트랜잭션 T1이 특정 레코드를 여러 번 조회하는 중에, 다른 트랜잭션 T2가 삽입한 레코드 R에 대한 유령 읽기를 경험할 수 있다.          |
+-----------------------+-----------------------------------------------------------------------------------------------------------------------------------+
| SERIALIZABLE (6)      | 동시성 관련한 모든 문제들(더티 읽기, 반복 불가능한 읽기, 유령 읽기)이 발생하지 않는다                                             |
+-----------------------+-----------------------------------------------------------------------------------------------------------------------------------+

응용 프로그램에서 트랜잭션 수행 중에 격리 수준이 변경되면, 수행 중인 트랜잭션의 남은 부분부터 변경된 격리 수준이 적용된다. 따라서, 트랜잭션 수행 중 객체에 대해 이미 획득한 일부 잠금이 새로운 격리 수준이 적용되는 동안 해제될 수도 있다. 이처럼 설정된 격리 수준이 하나의 트랜잭션 전체에 적용되는 것이 아니라 트랜잭션 중간에 변경되어 적용될 수 있기 때문에, 트랜잭션 격리 수준은 트랜잭션 시작 시점(커밋, 롤백, 또는 시스템 재시작 이후)에 변경하는 것이 하는 것이 바람직하다.

트랜잭션 격리 수준 값 확인
--------------------------

**GET TRANSACTION** 문을 이용하여 현재 클라이언트에 설정된 격리 수준 값을 출력하거나 *variable* 에 할당할 수 있다. 아래는 격리 수준을 확인하기 위한 구문이다. ::

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

상대적으로 낮은 수준 (4). 더티 리드는 일어나지 않는다, 하지만 비 반복적이거나 허구의 읽기는 발생할 수 있다. 즉, 트랜잭션 *T2*가 삽입이나 갱신이 허용되고 반면 *T1*은 반복적으로 한 개체를 읽는 경우 *T1*은 다른 값을 읽을 수 있다.

다음은 이 격리 수준의 규칙이다:

*   트랜잭션 *T1*은 다른 트랜잭션 *T2*에 의해서 삽입된 레코드를 읽거나 갱신할 수 없다. 이런 레코드는 읽거나 갱신하지 않고 차라리 무시된다.
*   트랜잭션 *T1*은 또 다른 트랜잭션 *T2*에 의해서 갱신된 레코드를 읽을 수 있지만 그 레코드의 마지막 커밋된 버전 만을 볼 수 있다 (*T1*은 커밋되지 않은 다른 버전들은 볼 수 없다).
*   트랜잭션 *T1*은 다른 트랜잭션 *T2*에 의해서 갱신중인 레코드를 수정할 수 없다. *T1*은 *T2*가 커밋하기를 기다린후, 레코드의 값을 다시 계산한다. 재 계산 검사를 통과하면 T1은 그 레코드를 수정한다, 통과하지 못하면 그 레코드를 무시한다.
*   트랜잭션 *T1*은 다른 트랜잭션 *T2*가 보고 있는 레코드를 수정할 수 있다.
*   트랜잭션 *T1*은 다른 트랜잭션 *T2*가 보고 있는 테이블에 레코드를 갱신/삽입할 수 있다.
*   트랜잭션 *T1*은 다른 트랜잭션 *T2*가 보고 있는 테이블의 스키마를 변경할 수 없다.
*   트랜잭션 *T1*은 실행된 문장에 대해서 새로운 스냅샷을 생성한다, 따라서 허구나 비 반복적인 읽기가 발생할 수 있다.

이 격리 수준은 배타적인 잠금을 위해서 MVCC 잠금 프로토콜을 따른다. 하나의 열에 대해서 공유된 잠금은 필요하지 않다; 그러나, 한 테이블에 대한 잠금 의향은 그 스키마에 대한 반복적인 읽기를 확보하기 위해서 트랜잭션이 종료되었을 때 해제된다.

*예:*

다음의 예는 팬텀 읽기나 비 반복적인 읽기가 발생할 수 있음을 보여준다. 왜냐하면, 트랜잭션의 동시성 수준이 **READ COMMITTED**이고 한 트랜잭션이 객체의 읽기를 수행하나 테이블 스키마 갱신에 대해서 반복적인 읽기가 보장되었을때, 다른 트랜잭션이 레코드의 추가나 갱신을 수행할 수 있기 때문이다.

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

**READ COMMITTED** 격리는 높은 격리 수준과 달르게 열을 동시에 갱신하는 것을 다르게 처리한다. 높은 격리 수준에서는, 만약 *T2*가 동시에 실행중인 트랜잭션 *T1*에 의해서 이미 갱신된 열을 수정하려고 하면, *T2*는 *T1*이 커밋이나 롤백할 때까지 블록된다, 만일 *T1*이 커밋하면 *T2*는 직렬화 오류를 방지하기 위해서 문장의 수행을 중단한다. **READ COMMITTED** 격리 수준에서, *T1*이 카밋후에, *T2*는 문장의 실행을 즉시 중단하지는 않는다 반면 새로운 버전을 다시 계산한다, 그것은 커밋된 것으로 간주되지 않고 띠리사 이 격리 수준의 제약을 위반하지 않을 수도 있다. 이전 버전의 select를 위해서 사용된 술부가 아직도 참이라면, *T2*는 계속 진행하여 새로운 버전을 수정한다. 만일 술부가 참이 아니라면, *T2*는 술부가 만족한적이 없는 것처럼 단순히 레코드를  무시한다.

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
|                                                                         |    *   T2 releases the lock on object and ingores it.                            |
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

비교적 높은 수준 (5). **snapshot isolation**으로 인해서 더티 리드, 비 반복적 읽기, 팬텀 읽기는 일어나지 않는다. 하지만, 이것이 진정한 **직렬성**은 아니다, 트랜잭션의 실행은 *동시에 실행중인 트랜잭션이 없는 것처럼* 계획될 수 없다. 쓰기 왜곡과 같은, **직렬화 스냅샷 격리*리 수준이 허용하지 않는 더욱 이상한 현상들이 발생할 수 있다.

쓰기 이상 현상에서, 중복되는 데이터 셋에서 동시에 읽기를 수행하고 중복된 데이터 셋에 대해서 독립적인 갱신을 트랜잭션들은 상대방에 의해서 실행된 갱신을 보는 경우가 없게 된다. 직렬화가능 시스템에서, 한 트랜잭션은 첫번째에 발생하고 두번째 트랜잭션은 첫번째 트랜잭션의 결과를 볼수 있어야 하기 때문에 이러한 이상 현상은 불가능할 것이다.

다음은 아 격리 수준의 규칙이다:

*   트랜잭션 *T1* 은 또 다른 트랜잭션 *T2*에 의해서 삽입된 레코드를 읽거나 수정할 수 없다. 이런 레코드는 차라리 무시된다.
*   트랜잭션 *T1* 은 또 다른 트랜잭션 *T2*에 의해서 갱신된 레코드를 읽을 수 있지만 *T1*은 그 레코드가 최종으로 커밋된 버전만을 볼 수 있다.
*   트랜잭션 *T1* 은 또 다른 트랜잭션 *T2*에 의해서 수정된 레코드를 수정할 수 없다.
*   트랜잭션 *T1* 은 또 다른 트랜잭션 *T2*가 보고 있는 레코드를 수정할 수 있다.
*   트랜잭션 *T1* 은 또 다른 트랜잭션 *T2*가 보고 있는 테이블에 레코드를 삽입하거나 갱신할 수 있다.
*   트랜잭션 *T1* 은 또 다른 트랜잭션 *T2*가 보고 있는 테이블의 스키마를 변경할 수 없다.
*   트랜잭션 *T1* 은 트랜잭션의 전체 기간에 걸쳐서 유효한 고유한 스냅샷을 생성한다.

**예제**

다음의 예는 **snapshot isolation** 때문에 비 반복적인 읽기와 팬텀 읽기가 발생하지 않는 것을 보여준다. 하지만, 쓰기 이상은 가능한데, 이것은 격리가 바로 **직렬화가능**은 아니라는 것을 의미한다.

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
|                                                                            |   /* phantom read connot occur due to snapshot isolation */                 |
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
----------------------------

CUBRID 10.0 **SERIALIZABLE** 격리 수준은 **REPEATABLE READ** 격리 수준과 같다. :ref:`isolation-level-5` 장에서 설명한 것과 같이, **SNAPSHOT** 격리 수준이 비 반복적인 읽기와 팬텀 읽기 이상 현상이 발생이지 않는다는 것을 보증하더라도, 쓰기 이상은 여전히 가능하다. 쓰기 이상으로부터 보호를 위해서, 읽기를 위한 인덱스 키 잠금이 사용될 것이다. 그 대신에 잠재적으로 격리 충돌이 발생할 수 있는 트랜잭션들을 중지시킴으로써, **SERIALIZABLE SNAPSHOT ISOLATION**을 제공하기 위한 복잡한 시스템을 기술하는 많은 작업들이 있다. 그러한 시스템이 추후 CUBRID 버전으로 공급될 것이다.

핵심 주제어는 과거의 버전과의 호환성 이유로 제거되지 않았다, 하지만 기억하라, 이것은 **REPEATABLE READ**와 비슷하다.

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

