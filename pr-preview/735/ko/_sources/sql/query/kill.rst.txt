
:meta-keywords: kill statement
:meta-description: The KILL statement terminates transactions with an option TRANSACTION or QUERY modifier.


****
KILL
****

**KILL** 구문은 **TRANSACTION** 또는 **QUERY** 수정자를 사용하여 트랜잭션을 종료한다.

::

    KILL [TRANSACTION | QUERY] tran_index, ... ;

\

* **KILL TRANSACTION** 은 **KILL** 질의문의 기본값이다. 수정자가 없는 **KILL** 과 같다. 해당 *tran_index* 와 관련된 트랜잭션을 종료한다.
* **KILL QUERY** 는 트랜잭션에서 수행 중인 질의문을 종료한다.

DBA와 DBA 그룹의 사용자는 시스템의 모든 트랜잭션을 종료할 수 있으며, DBA 외 사용자는 자신의 트랜잭션만 종료할 수 있다.

::

    KILL TRANSACTION 1;
    
