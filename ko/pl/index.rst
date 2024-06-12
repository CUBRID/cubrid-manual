:meta-keywords: cubrid pl, cubrid procedural language, cubrid server programming, cubrid pl/csql, cubrid jsp, cubrid method
:meta-description: CUBRID supports to develop stored functions and procedures.

.. _sql_procedural_langauge:

***************
CUBRID PL
***************

.. _pl-overview:

개요
==========================

저장 프로시저와 저장 함수를 통해 SQL로는 구현하기 힘든 복잡한 비즈니스 로직을 처리할 수 있으며, 데이터 조작을 간편하게 할 수 있다.
함수와 프로시저는 데이터를 조작하기 위해 실행 명령의 흐름이 있고, 쉽게 조작할 수 있고, 관리할 수 있는 블록 단위라고 할 수 있다.

저장 함수/프로시저를 사용할 때 얻을 수 있는 이점은 다음과 같다.

*   **생산성과 사용성**: Java 저장 함수/프로시저는 한번 만들어 놓으면 계속해서 사용할 수 있다. 사용자가 저장 함수와 저장 프로시저를 SQL에서도 호출하여 사용할 수 있다.
*   **뛰어난 상호 운용성과 이식성**: Java 저장 함수/프로시저는 Java 가상 머신을 사용하므로, 시스템에 Java 가상 머신을 사용할 수만 있다면 언제 어디서나 사용할 수 있다.

CUBRID는 저장 프로시저와 사용자 정의 함수를 지원하기 위해 다음의 SQL 절차적 언어 기능을 제공한다.

*   **PL/CSQL**
*   **자바 저장 프로시저**
*   **메서드**

.. _pl-csql:

PL/CSQL
===========================

PL/CSQL 프로그래밍 언어는 CUBRID SQL의 절차적 언어로의 확장이다. 

.. toctree::
    :maxdepth: 2

    pl.rst

..
    pl_structure.rst
    pl_declaration.rst
    pl_control_stmt.rst
    pl_sql_stmt.rst
    pl_porting.rst
..

.. _pl-jsp:

자바 저장 프로시저
===========================

CUBRID는 Java로 저장 함수와 프로시저를 개발할 수 있도록 지원한다. Java 저장 함수와 프로시저는 CUBRID에서 호스팅한 Java 가상 머신(JVM, Java Virtual Machine)에서 실행된다.

.. toctree::
    :maxdepth: 2

    jsp.rst

.. _pl-method:

메서드
===========================

메서드(method)는 CUBRID 데이터베이스 시스템의 내장 함수로 C로 작성된 프로그램이고, **CALL** 문에 의해 호출된다. 메서드 프로그램은 메서드가 호출되었을 때 동적 로더에 의해 실행 중인 응용과 함께 로드(load)되고 연결(link)된다. 메서드 실행 결과 생성된 리턴 값(return value)은 호출자(caller)에게 전달된다.

.. toctree::
    :maxdepth: 2

    method.rst
