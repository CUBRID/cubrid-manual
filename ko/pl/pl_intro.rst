:meta-keywords: cubrid pl introduction, cubrid pl system
:meta-description: This chapter describes an introduction to CUBRID PL system

*****************************
개요
*****************************


따라서 동일한 스키마 내에서는 동일한 이름을 가진 저장 프로시저와 저장 함수를 생성할 수 없으며, 다른 스키마 간에는 동일한 이름을 가진 저장 프로시저와 저장 함수를 생성할 수 있다.
스키마 이름을 통해 권한이 있다면 다른 사용자가 생성한 저장 프로시저와 저장 함수를 호출 할 수 있다.
그러나 구현하는 일련의 루틴에서 권한이 없는 객체를 참조하는 경우에는 컴파일 또는 실행 시 오류가 발생할 수 있다.
저장 프로시저 또는 그 내부에서 접근하는 객체에 대한 권한을 부여하고 해지하는 방법에 대한 자세한 사항은 :ref:`granting-authorization`\과 :ref:`revoking-authorization`\을 각각 참고한다.

저장 프로시저에서 기본 트랜잭션 동작에 대한 설명과 COMMIT과 ROLLBACK 구문을 사용하여 트랜잭션을 제어하는 방법은 :doc:`/pl/pl_tcl`\을 참고한다.
