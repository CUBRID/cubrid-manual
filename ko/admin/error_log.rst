
:meta-keywords: cubrid.msg, "$set 5 MSGCAT_SET_ERROR" log info
:meta-description: This section contains almost all runtime errors in CUBRID and consists of messages that inform users of various error situations that may occur during database operations. 

.. _error_log:

데이터베이스 오류 로그
======================


**CUBRID 오류 메시지 개요**

CUBRID 에러 메시지는 $CUBRID/msg 디렉터리에 한글 및 영문으로 제공된다, CUBRID 사용자(DBA) 관점에서 이러한 오류 메시지는 시스템, 트랜잭션/락, 로그/백업, DDL/DML, 인덱스, 제약조건, 권한, SQL 실행, HA/복제 등 CUBRID 데이터베이스가 런타임 중 직면할 수 있는 주요 오류 상황을 체계적으로 분류한 핵심 에러 카탈로그를 구성한다

CUBRID 오류 메시지 파일별 구성 및 사용자 안내 메시지는 다음과 같다.

* cubrid.msg: CUBRID 데이터베이스 엔진의 런타임 에러 및 시스템 메시지
* csql.msg: CSQL(CUBRID SQL 인터프리터) 클라이언트 도구의 사용법, 명령어, 에러 메시지
* utils.msg: CUBRID 관리 유틸리티 도구들의 메시지 및 사용법
* esql.msg: ESQL/C (Embedded SQL in C) 프리컴파일러의 에러 및 경고 메시지


**CUBRID 데이터 모델 용어 비교**

CUBRID는 객체-관계 데이터베이스(ORDB)로 개발되어 클래스(테이블) 상속과 같은 객체 개념을 일부 지원하고 있다, 이로 인해 오류 메시지나 내부 개념에서는 객체(Object) 기반 용어가 사용되기도 한다.
따라서 아래와 같이 전통적인 관계형 데이터베이스(RDB) 용어와 객체-관계 데이터베이스(ORDB) 용어를 함께 이해할 필요가 있다.


+--------------------------+---------------------------+
| 관계형(RDB) 용어         | 객체-관계형(ORDB) 용어    |
+==========================+===========================+
| 테이블 (table)           | 클래스 (class)            |
+--------------------------+---------------------------+
| 칼럼 (column)            | 속성 (attribute)          |
+--------------------------+---------------------------+
| 레코드 (record)          | 인스턴스 (instance)       |
+--------------------------+---------------------------+
| 데이터 타입 (data type)  | 도메인 (domain)           |
+--------------------------+---------------------------+


**CUBRID 오류 메시지 카테고리**

데이터베이스 주요 오류 메시지는 시스템 관리, SQL 질의, 데이터베이스 관리, 데이터베이스 파일, 트랜잭션, CUBRID HA 등 여섯 범주로 분류하였으며, 각 오류 코드와 메시지별로 설명과 서비스 영향도를 함께 정리하였다.


.. toctree::
    :maxdepth: 4

    error_log_system
    error_log_sql
    error_log_admin
    error_log_volume
    error_log_transaction
    error_log_ha

