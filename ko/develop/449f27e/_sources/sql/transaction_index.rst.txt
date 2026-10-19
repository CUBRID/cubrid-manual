
:meta-keywords: cubrid transaction, database transaction, cubrid locking, database locking, cubrid concurrency, multiversion concurrency control, mvcc, isolation level, database recovery
:meta-description: This chapter covers issues relating to concurrency (MVCC) and restore, as well as how to commit or rollback transactions in CUBRID database.


***************
트랜잭션과 잠금
***************

이 장에서는 트랜잭션을 커밋하거나 롤백하는 방법뿐만 아니라 동시성과 복구 이슈에 대하여 논의한다.

다중 사용자 환경에서 데이터베이스의 무결성을 보호하고 사용자의 트랜잭션이 항상 정확하고 일관된 데이터를 확보할 수 있도록 보장하기 위해서는 접근과 갱신의 조정이 필수적이다. 충분한 조정이 없다면 데이터는 타당하지 않거나 순서에 어긋나게 갱신될 수 있다.

동일한 데이터에 대한 병렬 연산을 통제하는 방법은 트랜잭션이 진행되는 동안 데이터를 잠그고 트랜잭션이 끝나기 전까지 다른 트랜잭션에서 용납되지 않은 데이터 접근을 허용하지 않는 것이다. 게다가 특정 테이블에 대한 갱신이 있을 때 이것을 커밋하기 전까지 다른 이들이 볼 수 없도록 한다. 만약 갱신을 커밋하지 않기로 결정했다면 마지막으로 갱신을 커밋하거나 롤백한 이후부터 입력된 모든 질의문을 무효화할 수 있다.

CUBRID는 다중 버전 동시성 제어(Multiversion Concurrency Control)을 사용하여 데이터 액세스를 최적화한다. 수정 중인 데이터를 덮어쓰지 않는 대신 이전 데이터를 삭제됨으로 표시하고 새 버전을 다른 곳에 추가한다. 즉, 다른 트랜잭션에서 이전 버전을 잠그지 않은 상태로 계속 읽을 수 있다. 각 트랜잭션은 트랜잭션 또는 질의문 실행 시작 시 정의된 자신의 데이터베이스 스냅샷 정보를 확인할 뿐 아니라, 스냅샷에 자체 변경 사항만 적용하고 수행 중 일관성을 유지한다.

여기에서 소개하는 모든 예제는 csql로 실행한 것이다.

.. toctree::
    :maxdepth: 2

    transaction.rst
