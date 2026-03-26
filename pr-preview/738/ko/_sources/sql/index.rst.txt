
:meta-keywords: cubrid sql, database sql, cubrid statements
:meta-description: This chapter describes SQL syntax such as data types, functions and operators, data retrieval or table manipulation. You can also find SQL statements used for index, trigger, partition, serial and changing user information.

**********
CUBRID SQL
**********

이 장에서는 CUBRID에서 사용되는 데이터 타입, 함수와 연산자, 데이터 조회나 테이블 조작 등의 SQL 구문에 대해 설명한다. 인덱스나 트리거, 분할, 시리얼 및 사용자 정보 변경 등의 작업을 위한 SQL 구문도 찾아볼 수 있다.

주요 내용은 다음과 같다.

*   작성 규칙

    *   식별자: 테이블, 인덱스, 칼럼의 이름으로 허용되는 문자열인 식별자 작성 방법을 설명한다.
    *   예약어: 질의문을 구성하기 위한 문법적인 용도로 사용하는 예약어들을 나열한다. 예약어를 식별자로 사용하려면 식별자를 겹따옴표("), 백틱(`) 혹은 대괄호([])로 반드시 감싸야 한다.
    *   주석
    *   리터럴: 상수 값 작성 방법을 설명한다.
    
*   데이터 타입: 데이터를 저장하는 형태인 데이터 타입에 대해 설명한다.

*   데이터 정의문: 테이블, 인덱스, 뷰, 시리얼을 생성, 변경, 삭제하는 방법을 설명한다.
   
*   연산자와 함수: 질의문에서 사용되는 연산자와 함수에 대해 설명한다.

*   데이터 조작문: SELECT, INSERT, UPDATE, DELETE 구문에 대해 설명한다.

*   질의 최적화: 인덱스와 힌트, 인덱스 힌트 구문을 이용한 질의 최적화에 대해 설명한다.

*   분할: 하나의 테이블을 여러 독립적인 논리적 단위로 분할하는 방법을 설명한다.

*   트리거(trigger): 특정 질의 수행 시 특정 기능이 같이 수행되도록 하는 트리거의 생성, 변경, 삭제 방법을 설명한다.

*   Java 저장 함수/프로시저: Java 메서드를 별도로 생성하여 질의문 내에서 호출할 수 있는 방법을 설명한다.

*   메서드(method): CUBRID 데이터베이스 시스템의 내장 함수인 메서드에 대해 설명한다.

*   클래스 상속: 부모와 자식 테이블(클래스) 사이에 속성을 상속하는 방법을 설명한다.

*   데이터베이스 관리: 사용자 관리, SET, SHOW 문에 대해 설명한다.

*   시스템 카탈로그: CUBRID 데이터베이스의 내부 정보인 시스템 카탈로그에 대해 설명한다.

.. toctree::
    :maxdepth: 3

    syntax.rst
    datatype_index.rst
    schema/index.rst
    function/index.rst
    query/index.rst
    tuning_index.rst
    partition_index.rst
    i18n_index.rst
    transaction_index.rst
    trigger.rst
    jsp.rst
    method.rst
    oodb.rst
    db_admin.rst
    catalog.rst
    
