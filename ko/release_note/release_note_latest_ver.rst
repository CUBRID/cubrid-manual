:tocdepth: 3

****************
11.3 릴리스 노트
****************

.. contents::

릴리스 노트 정보
================

본 문서는 CUBRID 11.3에 관한 정보를 포함한다.

CUBRID 11.3은 CUBRID 11.2에서 발견된 오류 수정 및 기능 개선과 이전 버전들에 반영된 모든 오류 수정 및 기능 개선을 포함한다.

CUBRID 11.2에 대한 정보는 https://www.cubrid.org/manual/ko/11.2/release_note/index.html 에서 확인할 수 있다.

CUBRID 11.0에 대한 정보는 https://www.cubrid.org/manual/ko/11.0/release_note/index.html 에서 확인할 수 있다.

CUBRID 10.2에 대한 정보는 https://www.cubrid.org/manual/ko/10.2/release_note/index.html 에서 확인할 수 있다.

CUBRID 10.1에 대한 정보는 https://www.cubrid.org/manual/ko/10.1/release_note/index.html 에서 확인할 수 있다.

CUBRID 10.0에 대한 정보는 https://www.cubrid.org/manual/en/10.0/release_note/index.html 에서 확인할 수 있다.

CUBRID 9.3에 대한 정보는 https://www.cubrid.org/manual/ko/9.3.0/release_note/index.html 에서 확인할 수 있다.

릴리스 개요
===========


CUBRID 11.3은 새로운 기능, 중요한 변경 사항 및 개선 사항이 포함 된 최신 안정화 버전이다.

.. TODO: UPDATE WITH DETAILS.

CUBRID 11.3은

* 데이터베이스 링크를 개선하여 연계성을 향상시켰다.
* 보다 안정적이고, 빠르고, 관리자의 편의성을 높였다. 
* 수많은 중요 버그가 수정되었다.
* 코드 재작성 및 최신화 작업을 포함한다.

CUBRID 11.3은 데이터베이스 링크를 위해 테이블 확장 형식(object@server)을 제공하여 사용자의 편리성을 제공하였고, 원격 DB에 대해 데이터 조회 뿐만 아니라 삽입, 수정 및 삭제할 수 있는 기능을 제공하여 **연계성을 향상시켰다**.

CUBRID 11.3은 **더 빨라졌다**. 이 버전은 조건절 푸시다운(predicate pushdown), 뷰 변환(view transformation) 및 불필요한 조인 테이블을 제거하는 등의 질의 최적화(query optimization) 및 aggregation 함수( min, max group_concat) 개선을 통해 성능을 더욱 향상시켰다. 또한 index(또는 foreign key) 생성 시 deduplication option을 제공하여 skewed index에 의해 발생한 성능 문제를 개선하였다. 

CUBRID 11.3은 csql, unloaddb 및 loaddb 유틸리티를 개선하여 **관리자 편의성을 개선** 하였다.

CUBRID 11.3의 데이터베이스 볼륨은 CUBRID 11.2 볼륨과 호환된다.(단, catalog view 중 db_index, db_vclass의 view query spec이 변경되어 CUBRID 11.2에 생성된 볼륨을 CUBRID 11.3에서 사용할 경우, 해당 catalog view 질의 수행시 기대하지 않는 결과를 얻을 수 있다.)

.. TODO: coming soon 

드라이버 호환성
---------------


*   CUBRID 11.3의 JDBC 및 CCI 드라이버는 CUBRID 11.2, 11.1, 11.0, 10.2, 10.1, 10.0, 9.3, 9.2, 9.1, 2008 R4.4, R4.3 또는 R4.1의 DB 서버와 호환된다.
*   드라이버 업그레이드를 권장한다.

변경 사항에 대한 자세한 내용은 :ref:`11_3_changes` 절을 참고한다. 이전 버전의 사용자는 :ref:`11_3_changes` 및 :ref:`11_3_new_cautions` 절을 확인해야 한다.

.. _11_3_changes:

11.3 변경사항 
=============

`change logs of CUBRID 11.3 <https://github.com/CUBRID/cubrid/releases/tag/v11.3>`_ 를 참고한다.

주의사항
========

.. _11_3_new_cautions:

신규 주의 사항
--------------

질의 최적화기의 통계 정보는 DDL 문이 수행될 때 자동으로 갱신되지 않고, 사용자가 직접 UPDATE STATISTICS 문을 실행하여 통계정보를 갱신해야 함 (:doc:`/sql/tuning` 참조)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ALTER TABLE 구문에서 AUTO_INCREMENT 속성 갖는 컬럼 또는 default 값을 가지고 있는 컬럼의 타입 변경시, AUTO_INCREMENT 속성을 사용할 수 없는 타입으로 변경하거나 기존 default 값을 변환할 수 없는 타입으로 변경할 때, 에러가 발생함  (:ref:`change-column` 참조)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

서버측 JDBC에서 cubrid.jdbc.driver.CUBRIDDriver.getDefaultConnection()을 통해 Connection 객체를 가져올 수 없음. 대신, DriverManager.getConnection("jdbc\:default\:connection\:");을 사용해야 함 (:ref:`jsp-server-side-jdbc-connection` 참조)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

USE INDEX(USING INDEX) 구문에서 유효하지 않은 인덱스명이나 테이블명을 지정하는 경우 에러 처리하지 않고 무시함  (해당 사항을 log 파일에 남기려면 error_log_warning 설정값을 yes로 설정하면 됨)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CREATE INDEX 생성시 COMMENT 절의 위치가 WITH 절 또는 INVISIBLE 절 뒤인 구문 마지막 위치로 변경됨 (:doc:`/sql/schema/index_stmt` 참조)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

기존 주의 사항
--------------

User schema 개념 도입으로 사용자 별로 동일한 객체명을 사용할 수 있으며, 지원에 따라 다음 동작이 변경됨
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* 객체명에 "."(dot)을 허용하지 않는다.
* 질의 또는 유틸리티 명령어 사용 시 "[사용자명].객체명"으로 사용해야 한다. (단, 로그인 된 사용자의 객체를 질의하는 경우에는 사용자명을 생략할 수 있음) (:doc:`/sql/user_schema` 참조)
* info schema, show full tables 결과에 사용자명 포함되도록 변경되었다. (:doc:`/sql/query/show` 참조)
* 11.2 이전 loaddb 파일은 11.2에서 수행할 수 있도록 user명.table명으로 수정하거나 \-\-no-user-specified-name 옵션을 설정하여 loaddb를 수행할 수 있다. (:ref:`loaddb` 참조)

JavaSP의 "jdbc\:default\:connection\:" 또는 getDefaultConnection()  사용 시 다음 함수 및 동작 변경됨 (:ref:`JavaSP 주의 사항 <jsp-caution>` 참조)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* java.sql.DatabaseMetaData의 모든 function 지원하지 않는다.
* java.sql.Connection의 createClob(), createBlob() 지원하지 않는다.
* java.sql.Statement의 addBatch(), clearBatch(), executeBatch(), setMaxRows(), cancel() 지원하지 않는다.
* 하나의 prepare (또는 execute)에 multiple sql을지원하지 않는다.
* cursor는 non-holdable로 변경되었다.
* ResultSet은 non-updatable, non-scrollable, non-sensitive로 변경되었다.

Truncate table 수행 시 FK의 set null 또는 cascade가 있는 경우 동작이 변경됨 (:doc:`/sql/query/truncate` 참조)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Alter change/modify 시 기입하지 않은 컬럼 속성은 유지되게 변경되었으며, alter 구문으로 auto_increment, on update property를 제거할 수 없게 변경됨 (:ref:`change-column` 참조)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
조건절에 컬럼명만 존재하는 경우 오류 처리되도록 변경됨
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* UPDATE t1 SET c1 = 9 WHERE c1; 형태로 사용하면 오류 발생한다.

Multiple SQL는 반드시 세미콜론으로 구분하도록 변경됨
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
CUBRID 패키지내의 CCI Driver 디렉토리가 $CUBRID/lib, $CUBRID/include에서 $CUBRID/cci/lib, $CUBRID/cci/include로 각각 변경됨 (:ref:`CCI 개요 <cci-overview>` 참조)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* CCI 사용 시 환경변수 LD_LIBRARY_PATH에 $CUBRID/cci/lib를 추가해야 한다.

백업 시 압축(-z, \-\-compress) 옵션이 기본으로 설정되도록 변경됨 (:ref:`backupdb` 참조)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
새로운 기능 추가로 인해 시스템 카탈로그 정보가 변경되거나 추가됨 (:doc:`/sql/catalog` 참조)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
옵션 없이 테이블 생성 시 reuse_oid 테이블로 생성
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
CHAR 데이터 타입의 최대 길이 256M 문자열로 변경
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
문자열 데이터 타입에 길이보다 큰 문자열이 들어가는 경우 오류 발생
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
문자열 끝의 공백 문자를 인식하게 수정하여, 문자열 끝의 공백 문자에 따라 다른 문자열로 인식
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
통계 수집 방식의 변경으로 주기적인 통계 수집 수행 필요
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
DB를 생성할 때 로케일(언어 및 문자 집합)을 지정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

DB를 생성할 때 로케일을 지정하도록 변경되었다.
   
CUBRID_CHARSET 환경 변수 제거
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

9.2 버전 이후 DB를 생성할 때 로케일(언어 및 문자 집합)을 지정하므로 CUBRID_CHARSET는 더 이상 사용하지 않는다.

.. 4.4new

[JDBC] 연결 URL의 zeroDateTimeBehavior 값이 "round"일 때 TIMESTAMP의 zero date가 '0001-01-01 00:00:00'에서 '1970-01-01 00:00:00'(GST)으로 변경 (CUBRIDSUS-11612)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
2008 R4.4부터 연결 URL의 "zeroDateTimeBehavior" 속성 값이 "round"일 때 TIMESTAMP의 zero date가 '0001-01-01 00:00:00'에서 '1970-01-01 00:00:00'(GST)으로 변경되었므로, 응용 프로그램에서 zero date를 사용하는 경우 주의해야 한다.


AIX에서 CUBRID SH 패키지 설치 시 권장 사항 (CUBRIDSUS-12251)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

AIX OS에서 ksh를 사용하여 CUBRID SH 패키지를 설치하는 경우 다음 오류와 함께 실패한다.
  
:: 
  
    0403-065 An incomplete or invalid multibyte character encountered. 
  
따라서 ksh 대신 ksh93 또는 bash를 사용할 것을 권장한다.
  
:: 
  
    $ ksh93 ./CUBRID-9.2.0.0146-AIX-ppc64.sh 
    $ bash ./CUBRID-9.2.0.0146-AIX-ppc64.sh 

CUBRID_LANG 제거, CUBRID_MSG_LANG 추가
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

9.1 버전부터 CUBRID_LANG 환경 변수를 더 이상 사용하지 않는다.
유틸리티 메시지 및 오류 메시지를 출력할 때는 CUBRID_MSG_LANG 환경 변수를 사용한다. 


CCI 응용 프로그램에서 여러 개의 질의를 한 번에 수행한 결과의 배열에 대한 오류 처리 방식 수정 (CUBRIDSUS-9364)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CCI 응용에서 여러 개의 질의를 한 번에 수행할 때 2008 R3.0부터 2008 R4.1 버전까지는 cci_execute_array 함수나 cci_execute_batch 함수에 의한 질의 수행 결과들 중 하나만 오류가 발생해도 해당 질의의 오류 코드를 반환했으나, 2008 R4.3 버전 및 9.1 버전부터는 전체 질의 개수를 반환하고 CCI_QUERY_RESULT_* 매크로를 통해 개별 질의에 대한 오류를 확인할 수 있도록 수정했다.

수정 이전 버전에서는 오류가 발생한 경우에도 배열 내 각각의 질의들의 성공 실패 여부를 알 수 없으므로, 이를 판단해야 한다.

.. code-block:: c

    ...
    char *query = "INSERT INTO test_data (id, ndata, cdata, sdata, ldata) VALUES (?, ?, 'A', 'ABCD', 1234)";
    ...
    req = cci_prepare (con, query, 0, &cci_error);
    ...
    error = cci_bind_param_array_size (req, 3);
    ...
    error = cci_bind_param_array (req, 1, CCI_A_TYPE_INT, co_ex, null_ind, CCI_U_TYPE_INT);
    ...
    n_executed = cci_execute_array (req, &result, &cci_error);

    if (n_executed < 0)
      {
        printf ("execute error: %d, %s\n", cci_error.err_code, cci_error.err_msg);

        for (i = 1; i <= 3; i++)
          {
            printf ("query %d\n", i);
            printf ("result count = %d\n", CCI_QUERY_RESULT_RESULT (result, i));
            printf ("error message = %s\n", CCI_QUERY_RESULT_ERR_MSG (result, i));
            printf ("statement type = %d\n", CCI_QUERY_RESULT_STMT_TYPE (result, i));
          }
      }
    ...

수정된 버전부터는 오류가 발생하면 전체 질의가 실패한 것이며, 오류가 발생하지 않은 경우에 대해 배열 내 각 질의의 성공 여부를 판단한다.

.. code-block:: c

    ...
    char *query = "INSERT INTO test_data (id, ndata, cdata, sdata, ldata) VALUES (?, ?, 'A', 'ABCD', 1234)";
    ...
    req = cci_prepare (con, query, 0, &cci_error);
    ...
    error = cci_bind_param_array_size (req, 3);
    ...
    error = cci_bind_param_array (req, 1, CCI_A_TYPE_INT, co_ex, null_ind, CCI_U_TYPE_INT);
    ...
    n_executed = cci_execute_array (req, &result, &cci_error);
    if (n_executed < 0)
      {
        printf ("execute error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
      }
    else
      {
        for (i = 1; i <= 3; i++)
          {
            printf ("query %d\n", i);
            printf ("result count = %d\n", CCI_QUERY_RESULT_RESULT (result, i));
            printf ("error message = %s\n", CCI_QUERY_RESULT_ERR_MSG (result, i));
            printf ("statement type = %d\n", CCI_QUERY_RESULT_STMT_TYPE (result, i));
          }
      }
    ...

java.sql.XAConnection 인터페이스에서 HOLD_CURSORS_OVER_COMMIT을 지원하지 않음 (CUBRIDSUS-10800)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

현재 CUBRID는 java.sql.XAConnection 인터페이스에서 ResultSet.HOLD_CURSORS_OVER_COMMIT를 지원하지 않는다.

9.0부터 STRCMP가 대소문자를 구분하여 동작
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

9.0 이전 버전까지는 STRCMP가 대소문자를 구분하지 않았지만 9.0부터는 문자열에서 대소문자를 비교하여 구분한다.
STRCMP가 대소문자를 구분하지 않도록 하려면 대소문자를 구분하지 않는 콜레이션(예: utf8_en_ci)을 사용해야 한다.

.. code-block:: sql

    -- In previous version of 9.0 STRCMP works case-insensitively
    SELECT STRCMP ('ABC','abc');
    0
    
    -- From 9.0 version, STRCMP distinguish the uppercase and the lowercase when the collation is case-sensitive.
    export CUBRID_CHARSET=en_US.iso88591
    
    SELECT STRCMP ('ABC','abc');
    -1
    
    -- If the collation is case-insensitive, it distinguish the uppercase and the lowercase.
    export CUBRID_CHARSET=en_US.iso88591

    SELECT STRCMP ('ABC' COLLATE utf8_en_ci ,'abc' COLLATE utf8_en_ci);
    0

2008 R4.1 버전부터 CCI_DEFAULT_AUTOCOMMIT의 기본값이 ON으로 변경됨 (CUBRIDSUS-5879)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CCI 인터페이스로 개발한 응용 프로그램의 자동 커밋 모드에 영향을 미치는 CCI_DEFAULT_AUTOCOMMIT 브로커 파라미터의 기본값이 CUBRID 2008 R4.1부터 ON으로 변경되었다. 이 변경의 결과로 CCI 및 CC 기반 인터페이스(PHP, ODBC, OLE DB 등) 사용자는 응용 프로그램의 자동 커밋 모드가 이에 대해 적합한지 확인해야 한다.

2008 R4.0 버전부터 페이지 단위를 사용하는 옵션 및 파라미터가 볼륨 크기 단위를 사용하도록 변경됨 (CUBRIDSUS-5136)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

데이터베이스 볼륨 크기와 cubrid createdb 유틸리티의 로그 볼륨 크기를 지정하기 위해 페이지 단위를 사용하는 옵션(-p, -l, -s)은 제거되므로, 2008 R4.0 베타 이후 새로 추가된 옵션(\-\-db-volume-size, \-\-log-volume-size, \-\-db-page-size, \-\-log-page-size)을 사용한다.

cubrid addvoldb 유틸리티의 데이터베이스 볼륨 크기를 지정하려면 페이지 단위를 사용하지 말고 2008 R4.0 베타 이후 새로 추가된 옵션(\-\-db-volume-size)을 사용한다.
페이지 단위 시스템 파라미터가 제거되므로 바이트 형식의 새 시스템 파라미터 사용을 권장한다. 관련 시스템 파라미터에 대한 자세한 내용은 아래를 참고한다.

2008 R4.0 베타 이전 버전 사용자는 db 볼륨 크기를 설정할 때 주의할 것 (CUBRIDSUS-4222)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

2008 R4.0 베타 버전부터 데이터베이스를 생성할 때 데이터 페이지 크기 및 로그 페이지 크기의 기본값이 4KB에서 16KB로 변경되었다. 페이지 수로 데이터베이스 볼륨을 지정하는 경우 볼륨의 바이트 크기는 예상과 다를 수 있다. 어떠한 옵션도 선택하지 않은 경우 이전 버전에서는 4KB 페이지 크기의 100MB 데이터베이스 볼륨이 생성되었다. 그러나 2008 R4.0부터는 16KB 페이지 크기의 512MB 데이터베이스 볼륨이 생성된다.

또한 사용 가능한 데이터베이스 볼륨의 최소 크기는 20MB로 제한된다. 따라서, 이 크기보다 작은 데이터베이스 볼륨을 생성할 수 없다.

2008 R4.0 이전 버전의 일부 시스템 파라미터의 기본값 변경 (CUBRIDSUS-4095)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

2008 R4.0부터 일부 시스템 파라미터의 기본값이 변경되었다.

max_clients의 기본값(DB 서버에서 허용되는 동시 연결 수 지정)과 index_unfill_factor의 기본값(인덱스 페이지 생성 시 향후 갱신을 위한 예약 공간의 비율 지정)이 변경되었으며, 바이트 단위의 시스템 파라미터의 기본값이 페이지 단위의 이전 시스템 파라미터의 기본값을 초과하는 경우 더 많은 메모리를 사용하게 되었다.

+-----------------------------+----------------------------+----------------------+--------------------+ 
| 기존 시스템                 | 추가된 시스템              | 기존 기본값          | 변경된 기본값      | 
| 파라미터                    | 파라미터                   |                      | (단위 :바이트)     |
|                             |                            |                      |                    | 
+=============================+============================+======================+====================+ 
| max_clients                 | 없음                       | 50                   | 100                | 
+-----------------------------+----------------------------+----------------------+--------------------+ 
| index_unfill_factor         | 없음                       | 0.2                  | 0.05               | 
+-----------------------------+----------------------------+----------------------+--------------------+
| data_buffer_pages           | data_buffer_size           | 100M(page size=4K)   | 512M               | 
+-----------------------------+----------------------------+----------------------+--------------------+
| log_buffer_pages            | log_buffer_size            | 200K(page size=4K)   | 4M                 | 
|                             |                            |                      |                    |
+-----------------------------+----------------------------+----------------------+--------------------+
| sort_buffer_pages           | sort_buffer_size           | 64K(page size=4K)    | 2M                 | 
|                             |                            |                      |                    | 
+-----------------------------+----------------------------+----------------------+--------------------+
| index_scan_oid_buffer_pages | index_scan_oid_buffer_size | 16K(page size=4K)    | 64K                | 
|                             |                            |                      |                    | 
+-----------------------------+----------------------------+----------------------+--------------------+

또한, cubrid createdb를 사용하여 데이터베이스를 생성할 때 데이터 페이지 크기 및 로그 페이지 크기의 최소 값이 1K에서 4K로 변경되었다.
 
시스템 파라미터가 잘못 설정된 경우 데이터베이스 서비스, 유틸리티 및 응용 프로그램을 수행할 수 없도록 변경됨 (CUBRIDSUS-5375)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

cubrid.conf 또는 cubrid_ha.conf에 정의되지 않은 시스템 파라미터를 설정하거나, 시스템 파라미터의 값이 임계값을 초과하거나, 페이지 단위 시스템 파라미터 및 바이트 단위 시스템 파라미터가 동시에 사용되는 경우 관련 서비스, 유틸리티 및 응용 프로그램이 수행되지 않도록 변경되었다.

CUBRID 32비트 버전에서 2G를 초과하는 값을 사용하여 data_buffer_size를 설정할 경우 데이터베이스 구동에 실패함 (CUBRIDSUS-5349)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CUBRID 32비트 버전에서 data_buffer_size의 값이 2G를 초과하는 경우 데이터베이스 구동에 실패한다. 이 설정 값은 OS 제한 때문에 32비트 버전에서 2G를 초과할 수 없다.

Windows Vista 및 그 이후 버전에서 CUBRID 유틸리티를 사용한 서비스 제어 시 권장 사항 (CUBRIDSUS-4186)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Windows Vista 및 그 이후 버전에서 cubrid 유틸리티를 사용하여 서비스를 제어하려면 명령 프롬프트 창을 관리자 권한으로 구동한 후 사용하는 것을 권장한다.

명령 프롬프트 창을 관리자 권한으로 구동하지 않고 cubrid 유틸리티를 사용하는 경우 UAC(User Account Control) 대화 상자를 통하여 관리자 권한으로 수행할 수 있으나 수행 결과의 메시지를 확인할 수 없다.

Windows Vista 및 그 이후 버전에서 명령 프롬프트 창을 관리자 권한으로 구동하는 방법은 다음과 같다.:

* [시작 > 모든 프로그램 > 보조프로그램 > 명령 프롬프트]를 마우스 오른쪽 버튼을 클릭한다.
* [관리자로 수행(A)]을 선택하면 권한 상승을 확인하는 대화 상자가 활성화되고, “예"를 클릭하여 관리자 권한으로 구동한다.
    
CUBRID 소스 빌드 후 수행 시, 매니저 서버 프로세스 관련 오류 발생 (CUBRIDSUS-3553)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

사용자가 CUBRID 소스를 직접 빌드하고 설치하는 경우, CUBRID와 CUBRID Manager를 각각 빌드하여 설치해야 한다. CUBRID 소스만 체크 아웃하고 빌드 후 cubrid service start 또는 cubrid manager start를 실행하면 "cubrid manager server is not installed"라는 오류가 발생한다.


2008 r3.0 또는 그 이전 버전에서 사용하던 GLO 클래스 지원 중단 (CUBRIDSUS-3826)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CUBRID 2008 R3.0 및 그 이전 버전은 glo(Generalized Large Object) 클래스를 사용하여 Large Object를 처리했지만 glo 클래스는 CUBRID 2008 R3.1 및 그 이후 버전에서 제거되었다. 대신, BLOB 및 CLOB(이후 LOB) 데이터 타입이 지원된다. LOB 데이터 타입에 대한 자세한 내용은 :ref:`blob-clob` 절을 참고한다.

glo 클래스 사용자는 다음 작업을 수행할 것을 권장한다.:

* GLO 데이터를 파일로 저장한 후에 다른 응용 프로그램 및 DB 스키마에서 GLO를 사용하지 않도록 수정한다.
* unloaddb 및 loaddb 유틸리티를 사용하여 DB 마이그레이션을 수행한다.
* 수정된 응용 프로그램에 따라 파일을 LOB 데이터로 로드하는 작업을 수행한다.
* 수정한 응용 프로그램이 정상적으로 동작하는지 확인한다.

예를 들어, cubrid loaddb 유틸리티가 GLO 클래스를 상속하거나 GLO 데이터 타입이 있는 테이블을 로드하는 경우 "Error occurred during schema loading." 오류 메시지와 함께 데이터 로딩을 중지한다.

GLO 클래스의 지원이 중단됨에 따라 각 인터페이스에 대해 삭제된 함수는 다음과 같다.:

+------------+----------------------------+
| 인터페이스 | 삭제한 함수                |
+============+============================+
| CCI        | cci_glo_append_data        |
|            |                            |
|            | cci_glo_compress_data      |
|            |                            |
|            | cci_glo_data_size          |
|            |                            |
|            | cci_glo_delete_data        |
|            |                            |
|            | cci_glo_destroy_data       |
|            |                            |
|            | cci_glo_insert_data        |
|            |                            |
|            | cci_glo_load               |
|            |                            |
|            | cci_glo_new                |
|            |                            |
|            | cci_glo_read_data          |
|            |                            |
|            | cci_glo_save               |
|            |                            |
|            | cci_glo_truncate_data      |
|            |                            |
|            | cci_glo_write_data         |
|            |                            |
+------------+----------------------------+
| JDBC       | CUBRIDConnection.getNewGLO |
|            |                            |
|            | CUBRIDOID.loadGLO          |
|            |                            |
|            | CUBRIDOID.saveGLO          |
|            |                            |
+------------+----------------------------+
| PHP        | cubrid_new_glo             |
|            |                            |
|            | cubrid_save_to_glo         |
|            |                            |
|            | cubrid_load_from_glo       |
|            |                            |
|            | cubrid_send_glo            |
|            |                            |
+------------+----------------------------+

마스터와 서버 프로세스 사이의 프로토콜이 변경된 경우 또는 두 버전이 동시에 실행되는 경우 포트 설정 필요 (CUBRIDSUS-3564)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

마스터 프로세스(cub_master)와 서버 프로세스(cub_server) 사이의 통신 프로토콜이 변경되었으므로 CUBRID 2008 R3.0 또는 그 이후 버전의 마스터 프로세스는 이전 버전의 서버 프로세스와 통신할 수 없고 이전 버전의 마스터 프로세스는 2008 R3.0 또는 그 이후 버전과 통신할 수 없다. 따라서 이전 버전이 이미 설치된 환경에 새 버전을 추가하여 동시에 두 버전의 CUBRID를 실행하는 경우 버전별로 다른 포트가 사용되도록 cubrid.conf의 cubrid_port_id 시스템 파라미터를 수정해야 한다.

JDBC에서 URL 문자열로서 연결 정보를 입력할 때 물음표 명시 (CUBRIDSUS-3217)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

JDBC에서 URL 문자열로서 연결 정보를 입력할 때 이전 버전에서 물음표(?)를 입력하지 않은 경우에도 속성 정보가 지정되었다. 그러나 이 CUBRID 2008 R3.0 버전에서는 구문에 따라 물음표를 명시해야 한다. 그렇지 않은 경우 오류가 표시된다. 또한, 연결 정보에 사용자명 또는 암호가 없는 경우에도 콜론(:)을 명시해야 한다. ::

    URL=jdbc:CUBRID:127.0.0.1:31000:db1:::altHosts=127.0.0.2:31000,127.0.0.3:31000 -- 에러처리 
    URL=jdbc:CUBRID:127.0.0.1:31000:db1:::?altHosts=127.0.0.2:31000,127.0.0.3:31000 -- 정상처리

데이터베이스명에 @을 포함할 수 없음 (CUBRIDSUS-2828)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

데이터베이스명에 @이 포함되면 호스트명이 지정된 것으로 해석될 수 있다. 이를 방지하기 위해 cubrid createdb, cubrid renamedb 및 cubrid copydb 유틸리티를 실행할 때는 데이터베이스에 @이 포함될 수 없도록 수정되었다.
