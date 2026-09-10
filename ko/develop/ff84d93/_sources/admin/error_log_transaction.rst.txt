트랜잭션 관련 오류
==================


.. _ERROR-72:

**ERROR CODE: -72, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) has been unilaterally aborted by the system.'**

- 이 메시지는 CUBRID 시스템에서 특정 트랜잭션이 시스템에 의해 일방적으로 중단되었음을 나타내는 오류입니다, 여기서 "일방적으로 중단"이라는 것은 트랜잭션을 실행한 사용자나 애플리케이션의 의도와 관계없이 시스템이 자동으로 트랜잭션을 롤백했다는 의미입니다,  이는 주로 데드락 해결, 시스템 리소스 부족, 서버 장애, 또는 기타 시스템 레벨의 문제로 인해 발생합니다.


.. _ERROR-73:

**ERROR CODE: -73, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on object %6$d|%7$d|%8$d. You are waiting for user(s) %9$s to finish.'**

- 이 메시지는 CUBRID 시스템에서 특정 트랜잭션이 특정 객체(레코드)에 대한 잠금을 획득하기 위해 기다리는 중 시간 초과가 발생했음을 나타내는 락 타임아웃(lock timeout) 오류입니다, 현재 트랜잭션이 다른 사용자가 해당 객체(레코드)에 대한 잠금을 해제하기를 기다리고 있지만, 설정된 시간 내에 잠금이 해제되지 않아 타임아웃이 발생한 상황입니다, 이는 주로 객체(레코드) 레벨의 동시성 제어 문제, 데드락 상황, 또는 장시간 실행되는 트랜잭션으로 인해 발생합니다.


.. _ERROR-74:

**ERROR CODE: -74, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on class %6$s. You are waiting for user(s) %7$s to finish.'**

- 이 메시지는 CUBRID 시스템에서 특정 트랜잭션이 테이블에서 잠금을 획득하기 위해 기다리는 중 시간 초과가 발생했음을 나타내는 락 타임아웃(lock timeout) 오류입니다, 현재 트랜잭션이 다른 사용자가 해당 테이블에 대한 잠금을 해제하기를 기다리고 있지만, 설정된 시간 내에 잠금이 해제되지 않아 타임아웃이 발생한 상황입니다, 이는 주로 테이블 레벨의 동시성 제어 문제, 데드락 상황, 또는 장시간 실행되는 트랜잭션으로 인해 발생합니다.


.. _ERROR-75:

**ERROR CODE: -75, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on instance %6$d|%7$d|%8$d of class %9$s. You are waiting for user(s) %10$s to finish.'**

- 이 메시지는 CUBRID 시스템에서 특정 트랜잭션이 테이블의 인스턴스에서 잠금을 기다리는 중 시간 초과가 발생했음을 나타내는 락 타임아웃(lock timeout) 오류입니다, 이는 주로 객체(레코드) 레벨 동시성 제어 문제, 데드락 상황, 또는 장시간 실행되는 트랜잭션으로 인해 발생합니다.


.. _ERROR-76:

**ERROR CODE: -76, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s on page %6$d|%7$d. You are waiting for user(s) %8$s to release the page lock.'**

- 이 메시지는 CUBRID 시스템에서 특정 트랜잭션이 페이지에서 작업을 수행하기 위해 페이지 잠금을 기다리는 중 시간 초과가 발생했음을 나타내는 래치 타임아웃(latch timeout) 오류입니다, 이는 주로 동시성 제어 문제, 데드락 상황, 또는 장시간 실행되는 트랜잭션으로 인해 발생합니다.


.. _ERROR-106:

**ERROR CODE: -106, 'Cannot prepare to commit the current transaction with global transaction identifier %1$d because that transaction identifier is in use by another transaction.'**

- 이 메시지는 CUBRID 시스템이 2PC(Two-Phase Commit) 분산 트랜잭션에서 현재 트랜잭션을 커밋 준비하려고 할 때, 지정된 글로벌 트랜잭션 식별자가 이미 다른 활성 트랜잭션에 의해 사용 중이어서 준비 작업을 진행할 수 없을 때 발생하는 오류입니다.


.. _ERROR-107:

**ERROR CODE: -107, 'There is no global distributed transaction associated with transaction identifier %1$d.'**

- 이 메시지는 CUBRID 시스템이 특정 트랜잭션 식별자에 연결된 글로벌 분산 트랜잭션을 찾을 수 없을 때 발생하는 오류입니다, 즉, 2PC(Two-Phase Commit) 분산 트랜잭션 환경에서 클라이언트가 특정 트랜잭션 ID를 사용하여 분산 트랜잭션에 대한 작업을 시도했지만 해당 ID에 해당하는 글로벌 분산 트랜잭션이 존재하지 않거나 이미 완료된 경우에 발생합니다.


.. _ERROR-108:

**ERROR CODE: -108, 'Current transaction %1$d must be committed or aborted before attempting to join to the distributed transaction with global identifier %2$d.'**

- 이 메시지는 CUBRID 시스템이 2PC(Two-Phase Commit) 분산 트랜잭션에 참여하려고 할 때, 현재 활성화된 트랜잭션이 있어서 분산 트랜잭션에 참여할 수 없을 때 발생하는 오류입니다, 분산 트랜잭션에 참여하기 전에 현재 트랜잭션을 완료(커밋 또는 중단)해야 함을 의미합니다.


.. _ERROR-198:

**ERROR CODE: -198, 'Receive buffer too small, data truncated.'**

- 이 메시지는 CUBRID 데이터베이스에서 데이터를 수신하는 과정에서 할당된 버퍼의 크기가 수신하려는 데이터의 크기보다 작아서 데이터의 일부가 잘렸음을 나타냅니다, 주로 네트워크 통신이나 파일 I/O 작업에서 대용량 데이터를 처리할 때 발생하며, 데이터 무결성에 영향을 줄 수 있는 오류입니다.


.. _ERROR-440:

**ERROR CODE: -440, 'Invalid cursor position.'**

- 이 메시지는 CUBRID 데이터베이스에서 쿼리 결과를 처리하는 과정에서 커서의 위치가 유효하지 않을 때 나타나는 오류입니다, 쿼리 프로세서에서 커서 위치 검증 과정에서 커서가 유효하지 않은 위치에 있거나, 커서 상태가 올바르지 않을 때입니다.


.. _ERROR-441:

**ERROR CODE: -441, 'Invalid cursor operation.'**

- 이 메시지는 CUBRID 데이터베이스에서 OID가 포함되어 있지 않는 결과(cursor)에서 레코도의 OID를 추출하려고 할 때 발생하는 오류입니다, 쿼리 프로세서에서 커서 작업 검증 과정에서 커서가 지원하지 않는 작업을 수행하려고 하거나, 커서 상태가 해당 작업을 허용하지 않을 때 발생될 수 있습니다.


.. _ERROR-442:

**ERROR CODE: -442, 'Unknown cursor position.'**

- 이 메시지는 CUBRID 데이터베이스에서 커서의 위치가 예상치 못한 상태이거나 정의되지 않은 상태일 때 나타나는 오류입니다, 쿼리 프로세서에서 커서 위치 검증 과정에서 커서 위치가 예상치 못한 값일 때 발생할 수 있습니다.


.. _ERROR-443:

**ERROR CODE: -443, 'Tuple value index %1$d is out of range.'**

- 이 메시지는 CUBRID 데이터베이스에서 쿼리 결과의 튜플에서 특정 인덱스로 값을 가져오려고 할 때, 해당 인덱스가 유효한 범위를 벗어났을 때 나타나는 오류입니다, 쿼리 프로세서에서 튜플 값 인덱스 검증 과정에서 튜플의 컬럼 수보다 큰 인덱스나 음수 인덱스를 사용했을 때입니다.


.. _ERROR-444:

**ERROR CODE: -444, 'Invalid attribute name: "%1$s".'**

- 이 메시지는 CUBRID 데이터베이스에서 쿼리 결과의 컬럼 이름이나 애트리뷰트 이름이 유효하지 않을 때 나타나는 오류입니다, 쿼리 프로세서에서 컬럼 이름 검증 과정에서 존재하지 않는 컬럼 이름이나 애트리뷰트 이름을 참조했을 때입니다.


.. _ERROR-447:

**ERROR CODE: -447, 'Attempted to operate a query result structure already closed.'**

- 이 메시지는 CUBRID 데이터베이스에서 이미 닫힌 쿼리 결과 구조(query result structure)에 대해 어떤 작업을 수행하려고 했을 때 나타나는 오류입니다, 쿼리 프로세서에서 쿼리 결과 구조의 상태를 검증하는 과정에서 발생합니다, 애플리케이션이 쿼리 결과를 명시적으로 닫았거나, 트랜잭션 종료 등으로 인해 내부적으로 닫힌 쿼리 결과에 다시 접근하려 할 때 발생될 수 있습니다.


.. _ERROR-449:

**ERROR CODE: -449, 'Unknown query identifier: %1$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 존재하지 않는 쿼리 식별자가 참조되었을 때 나타나는 오류입니다, 쿼리에서 쿼리 식별자 검증 과정에서 발생합니다, 즉, 정의되지 않은 쿼리 ID나 잘못된 쿼리 식별자가 사용된 경우입니다.


.. _ERROR-452:

**ERROR CODE: -452, 'Invalid XASL tree node content.'**

- 이 메시지는 CUBRID 데이터베이스에서 XASL(eXtensible Abstract Syntax Language) 트리 노드의 내용이 유효하지 않을 때 나타나는 오류입니다, 쿼리에서 XASL 트리 노드 검증 과정에서 발생합니다, 즉, XASL 트리 노드의 구조나 내용이 올바르지 않은 경우입니다.


.. _ERROR-460:

**ERROR CODE: -460, 'Too few input host variables provided.'**

- 이 메시지는 CUBRID 데이터베이스에서 쿼리 실행에 필요한 호스트 변수 수보다 적은 수의 호스트 변수가 제공되었을 때 나타나는 오류입니다.


.. _ERROR-550:

**ERROR CODE: -550, 'Unknown savepoint name %1$s'**

- 이 메시지는 CUBRID 데이터베이스에서 존재하지 않는 savepoint 이름을 참조할 때 나타나는 오류입니다, 주로 트랜잭션의 부분 롤백(partial rollback)을 수행할 때 지정된 savepoint가 존재하지 않을 때 발생합니다.


.. _ERROR-609:

**ERROR CODE: -609, 'Your transaction cannot be rolled back since logging was ignored. Your database may be corrupted.'**

- 이 메시지는 CUBRID 데이터베이스에서 트랜잭션 롤백을 시도할 때 발생합니다, 로깅이 무시된 상태에서는 트랜잭션을 롤백 시도하거나 트랜잭션 복구가 불가능하여 데이터베이스가 손상될 가능성이 있습니다.


.. _ERROR-640:

**ERROR CODE: -640, 'System Error: a savepoint cannot be added when the transaction is not active (e.g., during commit or rollback).'**

- 이 메시지는 CUBRID 데이터베이스에서 트랜잭션이 활성화되지 않은 상태에서 세이브포인트를 추가하려고 할 때 발생하는 시스템 오류입니다, 트랜잭션이 커밋(commit) 또는 롤백(rollback) 중이거나, 아직 시작되지 않았거나, 이미 종료된 상태에서는 세이브포인트를 생성할 수 없습니다, 이는 트랜잭션의 생명주기 관리와 관련된 내부 시스템 오류를 나타냅니다.


.. _ERROR-641:

**ERROR CODE: -641, 'A name must be given for a savepoint.'**

- 이 메시지는 CUBRID 데이터베이스에서 세이브포인트를 생성할 때 이름이 제공되지 않았을 때 발생하는 오류입니다, 세이브포인트는 트랜잭션 내에서 특정 지점을 저장하는 기능으로, 반드시 고유한 이름이 필요합니다, 이는 세이브포인트 생성 시 필수 파라미터인 이름이 누락되었을 때 발생하는 검증 오류입니다.


.. _ERROR-642:

**ERROR CODE: -642, ' System Error: there is not an active top system operation for current transaction.'**

- 이 메시지는 CUBRID 데이터베이스에서 현재 트랜잭션에 대한 활성화된 최상위 시스템 작업이 없을 때 발생하는 시스템 오류입니다, 시스템 작업을 종료하려고 할 때 해당 작업이 활성화되어 있지 않을 때 발생합니다, 이는 시스템 작업의 상태 관리와 관련된 내부 시스템 오류를 나타냅니다.


.. _ERROR-643:

**ERROR CODE: -643, 'May be a system error: Committing/Aborting transaction = %1$d (index = %2$d) which has permanent operations\n attached to it. Will attach those system operations to the transaction'**

- 이 메시지는 CUBRID 데이터베이스에서 트랜잭션을 커밋하거나 중단할 때 영구적인 시스템 작업이 연결되어 있을 때 발생하는 경고입니다, 트랜잭션에 시스템 작업이 연결된 상태에서 커밋이나 중단을 시도할 때 발생합니다, 이는 시스템 오류일 가능성이 있지만, CUBRID가 자동으로 해당 시스템 작업들을 트랜잭션에 연결하여 처리합니다.


.. _ERROR-674:

**ERROR CODE: -674, 'There is not enough active threads to continue current server execution. Current number of active threads is %1$d. You may need up to %2$d threads for clean server execution in your environment. Transaction index %3$d has been timed out to continue server execution.'**

- 이 메시지는 CUBRID 데이터베이스 서버에서 작업 스레드들이 전부‘대기(suspend) 상태로 묶여 있어 당장 일을 처리할‘활성(Active) 스레드가 모자라 보인다는 의미의 경고 메시지입니다.


.. _ERROR-735:

**ERROR CODE: -735, 'Unknown transaction index %1$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 유효하지 않은 트랜잭션 인덱스가 사용되었을 때 나타나는 오류입니다, 트랜잭션 인덱스는 CUBRID에서 각 트랜잭션을 고유하게 식별하기 위한 내부 식별자입니다, 이 메시지는 시스템이 존재하지 않거나 잘못된 트랜잭션 인덱스를 참조하려고 할 때 발생합니다, 즉, 트랜잭션 관리 시스템의 내부 상태 불일치나 메모리 손상으로 인해 발생할 수 있습니다.


.. _ERROR-830:

**ERROR CODE: -830, 'Cannot allocate query entry any more. Maximum allocatable entries are %1$d.'**

- 이 메시지는 CUBRID 데이터베이스의 쿼리 관리 모듈에서 새로운 쿼리를 실행하기 위한 쿼리 엔트리를 할당할 때 발생합니다, 즉, 트랜잭션당 최대 쿼리 엔트리 수를 초과하여 새로운 쿼리 엔트리를 할당할 수 없을 때 발생하는 오류 메시지입니다.


.. _ERROR-836:

**ERROR CODE: -836, 'LATCH ON PAGE(%1$d|%2$d) TIMEDOUT'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 페이지에 대한 래치(동기화 잠금) 획득 시도가 지정된 시간 내에 완료되지 않아 타임아웃이 발생했을 때 나타납니다, 래치(LATCH)는 데이터베이스 내부에서 페이지의 동시 접근을 제어하기 위한 경량 잠금 메커니즘으로, 다수의 트랜잭션이 동시에 같은 페이지에 접근할 때 경합이 심하거나, 장기 점유로 인해 래치 획득이 지연될 경우 발생할 수 있습니다, 주로 대량의 동시 트랜잭션, 비효율적인 쿼리, 시스템 부하, 내부 데드락, 버그 등으로 인해 래치 경합이 심해질 때 나타며, 이 영향은 I/O 병목 현상에 의해 원하는 페이지를 파일로 부터 가지고 오지 못하는 경우나, btree를 분할 또는 병합 수행 중 dead-latch가 발생하는 경우입니다.


.. _ERROR-842:

**ERROR CODE: -842, 'Cannot cache lock of object %1$d|%2$d|%3$d.'**

- 이 메시지는 클라이언트 모듈에 캐시되어 있는 락(LOCK)을 확인 시 NULL LOCK 이거나 LOCK 변경 오류로 여러 객체(레코드)들에 대해 락을 획득하는 과정에서, 특정 객체(레코드)에 대한 락 획득이 실패했거나 락 캐시 작업이 실패했을 때 발생하는 오류 메시지입니다.


.. _ERROR-843:

**ERROR CODE: -843, 'Wrong arguments in %1$s, a %2$s was passed.'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager) 함수들에서 잘못된 인자가 전달되었을 때 발생합니다, 주로 애플리케이션 코드에서 내부적으로 락 관리자 함수를 호출할 때 매개 변수가 NULL인 경우 발생합니다.


.. _ERROR-844:

**ERROR CODE: -844, 'Unknown isolation(%1$d) is found. The transaction index is %2$d.'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 트랜잭션의 격리 수준을 확인할 때, 시스템이 인식할 수 없는 잘못된 격리 수준 값을 발견했을 때 발생합니다, CUBRID는 `TRAN_READ_COMMITTED`, `TRAN_REPEATABLE_READ`, `TRAN_SERIALIZABLE`과 같은 정의된 격리 수준을 지원합니다. 이 범위를 벗어나는 값이 발견되면 이 오류가 발생합니다. 이는 일반적으로 메모리 손상, 동시성 제어 문제, 또는 트랜잭션 관리자의 내부 상태 불일치로 인해 발생하는 오류입니다.


.. _ERROR-845:

**ERROR CODE: -845, 'Invalid object type(%1$d) is found. The lockable object is %2$d|%3$d|%4$d.'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 트랜잭션 hold 리스트에서 락 엔트리를 삭제하는 과정에서 발생합니다, CUBRID에서 지원하는 범위를 벗어나는 값이 발견되면 이 오류가 발생합니다. 이는 일반적으로 메모리 손상, 동시성 제어 문제, 또는 락 관리자의 내부 상태 불일치로 인해 발생하는 오류입니다.


.. _ERROR-846:

**ERROR CODE: -846, '%1$s of tran(%2$d) is not found in the holder list of an object(%3$d|%4$d|%5$d).'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 테이블 락 강등(demotion) 과정에서 발생합니다, 즉, 특정 객체(레코드)의 holder 리스트에서 트랜잭션의 락 엔트리를 찾으려고 할 때, 해당 엔트리가 holder 리스트에 존재하지 않는 비정상적인 상태를 발견했을 때 발생하는 오류 메시지입니다, 이런 상황은 일반적으로 CUBRID 내부의 락 관리 로직에 문제가 있거나, 메모리 손상, 또는 동시성 제어 문제로 인해 락 엔트리의 상태 정보가 일관되지 않게 되었을 때 나타납니다. 이 오류가 동일 시점 반복적으로 발생될 경우 데이터베이스 서비스 안정성에 영향을 줄 수 있습니다.


.. _ERROR-847:

**ERROR CODE: -847, '%1$s on a %2$s(%3$d|%4$d|%5$d) is not found in the transaction(%6$d) hold list(cnt=%7$d).'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 트랜잭션의 hold 리스트 관리 과정에서 발생합니다, 즉, 특정 트랜잭션의 hold 리스트에서 락 엔트리를 삭제하려고 할 때, 해당 엔트리가 리스트에 존재하지 않는 비정상적인 상태를 발견했을 때 발생하는 오류 메시지입니다, 이런 상황은 일반적으로 CUBRID 내부의 락 관리 로직에 문제가 있거나, 메모리 손상, 또는 동시성 제어 문제로 인해 락 엔트리의 상태 정보가 일관되지 않게 되었을 때 나타납니다. 이 오류가 동일 시점 반복적으로 발생될 경우 데이터베이스 서비스 안정성에 영향을 줄 수 있습니다.


.. _ERROR-849:

**ERROR CODE: -849, 'Trying to abort a transaction(%1$d) twice.'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 데드락 해결 과정에서 발생합니다, 즉, 데드락으로 인해 트랜잭션을 중단(abort)하려고 할 때, 해당 트랜잭션이 이미 중단 중인 상태에서 다시 중단을 시도하는 비정상적인 상황을 발견했을 때 발생하는 오류 메시지입니다, 이런 상황은 일반적으로 데드락 감지 및 해결 과정에서 동시성 제어 문제나 트랜잭션 상태 관리의 불일치로 인해 발생합니다.


.. _ERROR-850:

**ERROR CODE: -850, 'Tran(%1$d) is neither lock holder nor lock waiter of an object(%2$d|%3$d|%4$d).'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 락을 해제할 때 발생합니다, 즉, 특정 객체(레코드)에 대한 락을 해제하려고 할 때, 해당 트랜잭션이 그 객체(Object)의 락을 소유한 holder도 아니고 락을 기다리는 waiter도 아닌 비정상적인 상태를 발견했을 때 발생하는 오류 메시지입니다, 이런 상황은 일반적으로 CUBRID 내부의 락 관리 로직에 문제가 있거나, 메모리 손상, 또는 동시성 제어 문제로 인해 락 리소스의 상태 정보가 일관되지 않게 되었을 때 나타납니다.


.. _ERROR-851:

**ERROR CODE: -851, 'Failed to allocate lock resource (%1$s).'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 락 엔트리(lock entry)를 할당할 때 발생합니다, 즉, 새로운 락을 획득하거나 락 상태를 관리하기 위해 필요한 메모리 자원을 할당하는 과정에서 실패했을 때 발생하는 오류 메시지입니다, 이런 상황은 일반적으로 시스템 메모리 부족, 락 관리자의 내부 자원 고갈, 또는 메모리 할당자(memory allocator)의 문제로 인해 발생합니다. 이는 데이터베이스의 락 관리 기능이 정상적으로 동작하지 못하게 하는 오류입니다.


.. _ERROR-852:

**ERROR CODE: -852, 'Total holders mode(%1$d) of an empty lock holder list is not NULL_LOCK.'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 데드락 감지 과정에서 발생합니다, 즉, 락 리소스를 순회하면서 데드락을 감지할 때, 락을 소유한 holder는 없지만 NULL_LOCK이 아닌 비정상적인 상태를 발견했을 때 발생하는 오류 메시지입니다.


.. _ERROR-854:

**ERROR CODE: -854, 'WARNING: No lock holder, but lock waiters exist. refer to %1$s.'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 데드락 감지 과정에서 발생합니다, 즉, 락 리소스를 순회하면서 데드락을 감지할 때, 락을 소유한 holder는 없지만 락을 기다리는 waiter는 존재하는 비정상적인 상태를 발견했을 때 발생하는 경고 메시지입니다, 이런 상황은 일반적으로 CUBRID 내부의 락 관리 로직에 문제가 있거나, 메모리 손상, 또는 동시성 제어 문제로 인해 락 리소스의 상태 정보가 일관되지 않게 되었을 때 나타납니다.


.. _ERROR-855:

**ERROR CODE: -855, 'WARNING: Two or more threads are lock-waiting for one transaction(%1$d).'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 락을 요청하는 과정에서 발생합니다, 이런 상황은 일반적으로 하나의 트랜잭션이 여러 스레드에서 동시에 실행되거나, 트랜잭션 내에서 중첩된 락 요청이 발생할 때 나타납니다. 



.. _ERROR-856:

**ERROR CODE: -856, 'WARNING: Strange lockwait state(%1$d|%2$d), thread(%3$d|%4$lld) and transaction(%5$d).'**

- 이 메시지는 CUBRID 데이터베이스의 락 관리자(lock manager)에서 락 대기(lockwait) 상태를 확인하는 도중, 예상치 못한 비정상적인 상태를 감지했을 때 발생하는 경고 메시지입니다, 이런 상황은 일반적으로 CUBRID 내부의 락 관리 로직에 문제가 있거나, 메모리 손상, 또는 동시성 제어 문제로 인해 락 대기 상태 정보가 일관되지 않게 되었을 때 나타납니다.


.. _ERROR-859:

**ERROR CODE: -859, 'LATCH ON PAGE(%1$d|%2$d) ABORTED'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 페이지에 대한 래치(동기화 잠금) 획득 시도가 시스템 내부 정책이나 예외 상황(예: 데드락, 강제 중단, 트랜잭션 롤백 등)으로 인해 중단(Abort)되었을 때 발생합니다, 주로 동시성 제어 과정에서 데드락이 감지되거나, 장기 점유로 인한 강제 중단, 시스템 내부 오류, 트랜잭션 강제 종료 등으로 인해 래치 획득이 정상적으로 완료되지 못할 때 나타며 이 영향은 I/O 병목 현상에 의해 원하는 페이지를 file로 부터 가지고 오지 못하는 경우나, btree를 분할 또는 병합 수행 중 dead-latch가 발생하는 경우입니다.


.. _ERROR-860:

**ERROR CODE: -860, 'Cannot make current transaction as a part of a global transaction because its state is '%1$s'.'**

- 이 메시지는 CUBRID 데이터베이스에서 2단계 커밋(2PC, Two-Phase Commit) 프로토콜을 사용하여 글로벌 트랜잭션을 시작하려고 할 때 발생합니다, 즉, 현재 트랜잭션이 활성 상태가 아니거나 적절한 상태가 아닌데, 글로벌 트랜잭션의 일부로 만들려고 시도할 때 발생하는 오류입니다, 이는 2PC 프로토콜의 올바른 사용과 트랜잭션 상태 관리의 무결성을 보장하기 위한 보호성 오류입니다.


.. _ERROR-861:

**ERROR CODE: -861, 'Cannot prepare to commit the current transaction with transaction identifier %1$d because that transaction is not made as a part of a global transaction.'**

- 이 메시지는 CUBRID 데이터베이스에서 2단계 커밋(2PC, Two-Phase Commit) 프로토콜을 사용하여 트랜잭션을 prepare하려고 할 때 발생합니다, 즉, 현재 트랜잭션이 글로벌 트랜잭션의 일부로 시작되지 않았는데, 2PC prepare를 시도할 때 발생하는 오류입니다, 이는 2PC 프로토콜의 올바른 사용 순서를 보장하고, 글로벌 트랜잭션의 무결성을 보호하기 위한 보호성 오류입니다.


.. _ERROR-862:

**ERROR CODE: -862, 'Cannot set the user information of the global transaction with the identifier %1$d because its state is '%2$s'.'**

- 이 메시지는 CUBRID 데이터베이스에서 2단계 커밋(2PC, Two-Phase Commit) 프로토콜을 사용하는 글로벌 트랜잭션의 사용자 정보를 설정하려고 할 때 발생합니다, 즉, 글로벌 트랜잭션이 이미 2PC 프로토콜의 중간 단계에 있어서 사용자 정보를 변경할 수 없는 상태일 때 발생하는 오류입니다, 이런 상황은 일반적으로 분산 트랜잭션 환경에서 여러 데이터베이스 간의 트랜잭션 일관성을 보장하기 위해 2PC 프로토콜을 사용할 때 발생합니다, 이는 트랜잭션의 상태 관리와 데이터 일관성을 보호하기 위한 보호성 오류입니다.


.. _ERROR-966:

**ERROR CODE: -966, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on object %6$d|%7$d|%8$d because of deadlock. You are waiting for user(s) %9$s to finish.'**

- 이 메시지는 CUBRID 데이터베이스에서 트랜잭션이 특정 객체(레코드)에 대한 락을 획득하려고 대기하는 동안 데드락 상황이 발생하여 타임아웃이 발생했음을 나타냅니다, 데드락은 두 개 이상의 트랜잭션이 서로가 보유한 리소스의 락을 기다리면서 무한 대기 상태에 빠지는 상황을 의미하며, CUBRID는 이를 감지하고 해결하기 위해 일부 트랜잭션을 중단시킵니다.


.. _ERROR-967:

**ERROR CODE: -967, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on class %6$s because of deadlock. You are waiting for user(s) %7$s to finish.'**

- 이 메시지는 CUBRID 데이터베이스에서 트랜잭션이 특정 테이블에 대한 락을 획득하려고 대기하는 동안 데드락 상황이 발생하여 타임아웃이 발생했음을 나타냅니다, 데드락은 두 개 이상의 트랜잭션이 서로가 보유한 리소스의 락을 기다리면서 무한 대기 상태에 빠지는 상황을 의미합니다. CUBRID는 데드락을 감지하고 이를 해결하기 위해 일부 트랜잭션을 중단시키는데, 이 경우 해당 트랜잭션이 데드락으로 인해 타임아웃되었음을 알려줍니다.



.. _ERROR-968:

**ERROR CODE: -968, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on instance %6$d|%7$d|%8$d of class %9$s because of deadlock. You are waiting for user(s) %10$s to finish.'**

- 이 메시지는 데드락 상황에서 특정 객체(레코드)에 대한 잠금을 획득하지 못하고 타임아웃이 발생할 때 나타납니다. 


.. _ERROR-1002:

**ERROR CODE: -1002, 'The current transaction could not acquire a database lock it requires to continue processing.'**

- 이 메시지는 트랜잭션이 필요한 리소스에 대한 잠금을 획득하지 못했을 때 발생합니다.


.. _ERROR-1021:

**ERROR CODE: -1021, 'A deadlock cycle is detected. %1$s.'**

- 이 메시지는 데이터베이스 시스템에서 두 개 이상의 트랜잭션이 서로가 점유하고 있는 잠금(Lock)이 해제되기를 기다리면서 더 이상 진행하지 못하는 순환 대기 상태, 즉 교착 상태(Deadlock)가 발생했음을 나타냅니다, CUBRID의 잠금 관리자(Lock Manager)가 이러한 교착 상태를 감지하고 시스템 전체의 멈춤을 방지하기 위해 관련된 트랜잭션 중 하나를 강제로 종료(롤백)시키면서 이 오류를 보고합니다.


.. _ERROR-1154:

**ERROR CODE: -1154, 'MVCC row %1$d|%2$d|%3$d does not satisfies reevaluation.'**

- 이 메시지는 CUBRID 데이터베이스에서 MVCC(Multi-Version Concurrency Control) 환경에서 특정 레코드에 대한 조건 재평가(reevaluation)가 실패했을 때 발생합니다, 즉, 트랜잭션이 특정 레코드에 접근하려고 할 때, 해당 레코드가 현재 트랜잭션의 MVCC 조건을 만족하지 않음을 시스템이 감지했을 때 발생하는 오류입니다, 이런 상황은 일반적으로 동시성 제어 과정에서 레코드의 버전이 변경되었거나, 트랜잭션 격리 수준에 따른 가시성 조건이 만족되지 않을 때 발생합니다, 이는 데이터베이스의 일관성과 동시성 제어를 위한 보호성 오류입니다.(단, isolation level에 따라 재평가 달라짐)


.. _ERROR-1156:

**ERROR CODE: -1156, 'MVCC can't get snapshot'**

- 이 메시지는 CUBRID 데이터베이스의 MVCC(Multi-Version Concurrency Control) 시스템에서 트랜잭션의 스냅샷을 생성할 수 없을 때 발생하는 오류를 나타냅니다. MVCC 스냅샷은 트랜잭션이 시작될 때 데이터베이스의 일관된 상태를 캡처하는 중요한 메커니즘으로, 이를 통해 트랜잭션은 일관된 데이터 뷰를 유지할 수 있습니다.


.. _ERROR-1157:

**ERROR CODE: -1157, 'Isolation level value in MVCC must be 'read committed', 'repeatable read' or 'serializable'.'**

- 이 메시지는 CUBRID 데이터베이스에서 MVCC 환경에서 트랜잭션 격리 수준(Isolation Level)을 설정할 때, 지원되지 않는 값이 지정되었을 때 발생합니다, 즉, CUBRID의 MVCC 모드에서는 'read committed', 'repeatable read', 'serializable' 세 가지 격리 수준만 지원하는데, 이 범위를 벗어나는 값이 설정되려고 할 때 발생하는 검증 오류입니다, 이런 상황은 일반적으로 트랜잭션 격리 수준을 변경하는 SET TRANSACTION ISOLATION LEVEL 문이나 내부 API 호출에서 잘못된 값이 전달될 때 발생합니다, 이는 데이터베이스의 동시성 제어 메커니즘을 보호하기 위한 검증성 오류입니다.


.. _ERROR-1176:

**ERROR CODE: -1176, 'Latch promotion for page %1$d of volume %2$d failed.'**

- 이 메시지는 CUBRID 데이터베이스 시스템에서 특정 페이지(b-tree)에 대한 래치(latch) 승격(promote) 작업이 실패했음을 나타냅니다. 래치는 데이터베이스 내부에서 메모리 상의 데이터 구조(예: 페이지 버퍼의 페이지)에 대한 동시성 제어를 위해 사용되는 경량 잠금 메커니즘입니다., "래치 승격"은 일반적으로 공유 래치(shared latch)를 배타적 래치(exclusive latch)로 업그레이드하거나, 페이지에 대한 쓰기 작업을 수행하기 위해 더 높은 수준의 래치를 획득하려는 시도를 의미합니다,  이 실패는 페이지 버퍼 관리, 파일 관리 또는 B-트리 작업 중 발생할 수 있으며, 페이지의 일관성 유지에 필수적인 래치 획득에 문제가 생겼음을 시사합니다. 이는 동시성 문제, 페이지 상태 불일치 또는 내부적인 오류로 인해 발생할 수 있으며, 트랜잭션 실패나 데이터베이스 불안정으로 이어질 수 있습니다.

