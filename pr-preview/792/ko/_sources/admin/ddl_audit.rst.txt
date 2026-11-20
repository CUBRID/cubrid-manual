
:meta-keywords: DDL Audit, log
:meta-description: CUBRID has capability of recording DDL (Data Definition Language) that changes the database system configuration such as create/delete/modify tables as well as changing access privilege of a table.

.. _ddl-audit:

***************
DDL Audit Log
***************

개요
========

CUBRID는 테이블 생성/삭제/수정 등 데이터베이스 시스템 구성 및 테이블 액세스 권한을 변경하는 DDL(Data Definition Language)을 기록하는 기능을 가지고있다.
CAS, csql 및 loaddb를 통해 수행된 DDL은 필요에 따라 실행 된 파일의 복사본과 함께 로그 파일에 기록 될 수 있다.

시스템 파라미터의 ddl_audit_log가 yes 이면 $CUBRID/log/ddl_audit 디렉토리에 DDL Audit log가 생성된다. 각 로그 파일의 크기는 ddl_audit_log_size 매개 변수에 지정된 값을 초과 할 수 없다. DDL Audit와 관련된 시스템 매개 변수는 CUBRID 운영의  :doc:`/admin/config` 을 참조 한다.

.. note::

    :ref:`catalog` 의 메서드를 사용하는 경우에는 DDL Audit Log는 생성되지 않는다.

DDL Audit 로그 파일 이름 규칙
================================

* cas: {broker_name}_{app_server_num}_ddl.log
* csql interactive: csql_{db_name}_ddl.log
* loaddb: loaddb_{db_name}_ddl.log

**추가 파일 이름 규칙 :**

* 파일의 csql : csql/{csql_file}_{YYYYMMDD}_{HHMMSS}_{pid}
* loaddb : loaddb/{loaddb_file}_{YYYYMMDD}_{HHMMSS}_{pid}


CAS의 DDL Audit 로그 파일 형식
================================

* [Time] [ip_addr]|[user_name]|[result]|[elapsed time]|[auto commit/rollback]|[sql_text]

	설명:

	* [Time] : DDL 실행 시작 시간 (예 : 20-12-18 12 : 08 : 32.327)
	* [ip_addr] : 애플리케이션 클라이언트의 IP 주소 (예 : 172.31.0.70)
	* [user_name] : DDL을 발행 한 데이터베이스 사용자 이름
	* [result] : 명령문 실행 결과. 성공하면 OK, 그렇지 않으면 오류 코드 (예 : ERROR : -494)
	* [elapsed time] : 문 실행 경과 시간
	* [auto commit/rollback] : 오류 코드와 함께 자동으로 커밋되거나 롤백된다.
	* [sql_text] : 실행 된 DDL 텍스트

CSQL의 DDL Audit 로그 파일 형식
================================

* [Time] [pid]|[user_name]|[result]|[elapsed time]|[auto commit/rollback]|[sql_text]

	설명:
	
	* [Time] : DDL 실행 시작 시간 (예 : 20-11-20 13 : 26 : 51.765)
	* [pid] : csql 프로세스 ID
	* [user_name] : DDL을 발행 한 데이터베이스 사용자 이름
	* [result] : 명령문 실행 결과. 성공하면 OK, 그렇지 않으면 오류 코드 (예 : ERROR : -272)
	* [elapsed time] : 문 실행 경과 시간
	* [auto commit/rollback] : 오류 코드와 함께 자동으로 커밋되거나 롤백된다.
	* [sql_text] : 실행 된 DDL 텍스트 또는 실행 된 csql 파일 이름

LOADDB의 DDL Audit 로그 형식
================================

* [Time] [pid]|[user_name]|[result]|[log contents]|[file name]

	설명:

	* [Time] : DDL 실행 시작 시간 (예 : 20-12-18 12 : 08 : 32.327)
	* [pid] : loaddb 프로세스 ID
	* [user_name] : DDL을 발행 한 데이터베이스 사용자 이름
	* [result] : loaddb 실행 결과, 성공하면 OK, 그렇지 않으면 오류 코드 (예 : ERROR : -494)
	* [log contents] : 총 문의 수 또는 오류 및 오류 행의 경우 커밋 수.
	* [file name] : loaddb에서 로드 한 파일을 복사한다. 이때 복사 된 파일의 이름이다.
