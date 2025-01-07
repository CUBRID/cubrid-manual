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
저장 함수/프로시저는 한번 생성한 후 SQL에서 지속적으로 사용할 수 있으므로 높은 생산성과 사용성을 갖는다.

CUBRID는 저장 프로시저/함수를 위해 다음 세가지 절차적 언어를 지원한다.

*   **PL/CSQL**
*   **Java**
*   **C**

.. _pl-csql:

PL/CSQL 저장 프로시저/함수
===========================

CUBRID는 PL/CSQL(Procedural Language extension of CUBRID SQL)로 저장 함수와 프로시저를 개발할 수 있도록 지원한다.
PL/CSQL은 CUBRID SQL의 절차적 언어로의 확장으로서, 선언적 언어인 SQL만으로 구현하기 어려운
조건문, 반복문, 변수, 에러 처리, 내부 프로시저/함수를 통한 모듈화 같은 절차적 언어의 특성을 지원한다.
물론, SQL 문을 PL/CSQL 프로그램 안에 포함시켜 함께 사용할 수 있다.

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

Java 저장 프로시저/함수
===========================

CUBRID는 Java로 저장 함수와 프로시저를 개발할 수 있도록 지원한다. Java 저장 함수와 프로시저는 CUBRID에서 호스팅한 Java 가상 머신(JVM, Java Virtual Machine)에서 실행된다.
Java 저장 함수/프로시저는 Java 가상머신을 사용하므로 뛰어난 상호 운용성과 이식성을 갖는다. 

.. toctree::
    :maxdepth: 2

    jsp.rst

.. _pl-method:

C 메서드
===========================

메서드(method)는 CUBRID 데이터베이스 시스템의 내장 함수로 C로 작성된 프로그램이고, **CALL** 문에 의해 호출된다. 메서드 프로그램은 메서드가 호출되었을 때 동적 로더에 의해 실행 중인 응용과 함께 로드(load)되고 연결(link)된다. 메서드 실행 결과 생성된 리턴 값(return value)은 호출자(caller)에게 전달된다.

.. toctree::
    :maxdepth: 2

    method.rst
