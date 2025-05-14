:meta-keywords: cubrid pl, cubrid procedural language, cubrid server programming, cubrid pl/csql, cubrid jsp, cubrid method
:meta-description: CUBRID supports to develop stored functions and procedures. This chapter describes an introduction to CUBRID PL system

.. _sql_procedural_langauge:

***************
CUBRID 절차적 언어
***************

이 장에서는 저장 프로시저와 저장 함수를 사용할 수 있도록 CUBRID 에서 제공하는 절차적 언어 확장 기능을 소개한다.

CUBRID 는 절차적 언어 확장 기능으로 저장 프로시저와 저장 함수를 지원한다. 저장 프로시저와 저장 함수를 사용하면, SQL 만으로 구현하기 어려운 복잡한 비즈니스 로직을 효율적으로 처리하고 데이터 조작을 간편하게 수행할 수 있다.

다음의 주요한 장점을 가진다.

* 생산성과 사용성: 저장 프로시저와 저장 함수는 한 번 만들어 두면 여러 번 재사용할 수 있다. 사용자가 SQL 문이나 JDBC를 통해 응용 프로그램에서도 쉽게 호출할 수 있다.
* 성능: 저장 프로시저와 저장 함수는 데이터베이스 서버에서 실행되기 때문에 네트워크 트래픽을 줄이고 데이터베이스 서버의 성능을 향상시킨다.
* 보안: 저장 프로시저는 특정 사용자에게 실행 권한을 부여할 수 있으므로, 데이터 접근 및 수정 권한을 세밀하게 제어할 수 있다.
* 상호 운용성과 이식성: 저장 프로시저와 함수는 다양한 언어 및 실행 환경으로 동작할 수 있도록 설계되어 데이터베이스의 활용도를 극대화하였다. CUBRID는 저장 프로시저/함수를 위해 다음 두 가지 절차적 언어를 지원한다.

        * PL/CSQL
        * Java

이 장에서 설명하는 주요 내용은 다음과 같다.

*   저장 프로시저의 생성: 저장 프로시저와 저장 함수의 생성에 대하여 설명한다.

*   저장 프로시저의 호출: 저장 프로시저와 저장 함수를 호출에 대하여 설명한다.

*   저장 프로시저의 권한: 저장 프로시저에 대한 호출 권한 부여와, 소유자 권한과 호출자 권한의 차이에 대하여 설명한다.

*   트랜잭션 커밋과 롤백: 저장 프로시저 내에서 트랜잭션 커밋과 롤백 사용에 대하여 설명한다.

*   성능 최적화: 저장 프로시저를 사용할 때 최적화된 성능을 얻기 위한 방법에 대해 설명한다.

*   시스템 패키지: CUBRID에서 지원하는 시스템 패키지에 대해 설명한다.

*   PL/CSQL: CUBRID의 SQL 절차적 언어의 확장인 PL/CSQL의 개요와 문법에 대해 설명한다.

*   자바 저장 프로시저: 자바 언어를 사용하여 저장 프로시저를 개발하는 방법에 대해 설명한다.

*   메서드: CUBRID의 내장 함수인 메서드에 대해 설명한다.

.. toctree::
    :maxdepth: 2

    pl_create
    pl_call
    pl_auth
    pl_tcl
    pl_tuning
    pl_package
    plcsql
    jsp
    method
