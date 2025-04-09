:tocdepth: 3

****************
11.4 릴리스 노트
****************

.. contents::

릴리스 노트 정보
================

본 문서는 CUBRID 11.4(빌드번호 11.4.0.0000)에 관한 정보를 포함한다.
CUBRID 11.4은 CUBRID 11.3 버전에서 발견된 오류 수정 및 기능 개선과 이전 버전들에 반영된 모든 오류 수정 및 기능 개선을 포함한다.

CUBRID 11.3에 대한
정보는 https://www.cubrid.org/manual/ko/11.3/release_note/index.html 에서 확인할 수 있다.

CUBRID 11.2에 대한
정보는 https://www.cubrid.org/manual/ko/11.2/release_note/index.html 에서 확인할 수 있다.

CUBRID 11.0에 대한
정보는 https://www.cubrid.org/manual/ko/11.0/release_note/index.html 에서 확인할 수 있다.

CUBRID 10.2에 대한
정보는 https://www.cubrid.org/manual/ko/10.2/release_note/index.html 에서 확인할 수 있다.

CUBRID 10.1에 대한
정보는 https://www.cubrid.org/manual/ko/10.1/release_note/index.html 에서 확인할 수 있다.

CUBRID 10.0에 대한
정보는 https://www.cubrid.org/manual/en/10.0/release_note/index.html 에서 확인할 수 있다.

CUBRID 9.3에 대한
정보는 https://www.cubrid.org/manual/ko/9.3.0/release_note/index.html 에서 확인할 수 있다.

Release 개요
============

CUBRID 11.4은 새로운 기능, 중요한 변경 사항 및 개선 사항이 포함 된 최신 안정화 버전이다.

CUBRID 11.4는

1. **오라클 호환성을 위한 PL/CSQL 지원**
2. **대용량 처리 지원을 위한 HASH JOIN 추가**
3. **옵티마이저 및 인덱스 처리 방식 개선을 통한 성능 향상**
4. **데이타 복구시 병렬 처리로 인한 성능 향상**
5. **결과 캐시 확장을 통한 성능 개선**
6. **데이타 덤프 성능 향상**
7. **메모리 모니터링 기능 추가**
8. **접근 제어 기능 향상**
9. **백업/복구 운영 사용자 편의 향상**

을 통해서 새로운 기능과 더불어 다양한 부분에서의 성능 개선이 이루어 졌다.

드라이버 호환성
---------------

-  CUBRID 11.4의 JDBC 및 CCI 드라이버는 CUBRID 11.3, 11.2, 11.1, 11.0, 10.2, 10.1, 10.0, 9.3, 9.2, 9.1, 2008 R4.4, R4.3 또는 R4.1의 DB 서버와 호환된다.
-  드라이버 업그레이드를 권장한다.

.. _11_4_changes:

11.4 변경사항
=============

.. _11_4_changes_add_feature:

기능추가
--------

SQL
~~~

HASH JOIN 지원
^^^^^^^^^^^^^^

Optimizer가 HASH JOIN을 사용하기 위해서는 조인 힌트를 사용해야만 한다.
``/*+ USE_HASH */``  힌트가 추가되면 HASH JOIN 고려함.
``/*+ NO_USE_HASH */`` 힌트가 추가되면 HASH JOIN 사용하지 않음.
HASH JOIN을 사용한 경우와 다른 조인 방법을 사용하는 경우 결과가 동일하여야 한다.

serial의 소유자를 변경하는 SQL 구문을 추가
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``call change_serial_owner()`` 메소드만으로 serial 소유자를 변경 가능하던 것을 SQL문으로 소유자 변경 기능 추가함.

- ``AS-IS`` 
   
.. code:: sql 

   CALL CHANGE_SERIAL_OWNER('test_serial', 'test_user1') on class db_serial; 

- ``TO-BE`` 

.. code:: sql 

   ALTER SERIAL test_serial OWNER TO test_user1; 

#. DBA와 DBA 그룹 멤버만 serial 소유자 변경이 가능 함. 
#. 권한이 없을 경우나 serial이 없을 경우에는 에러가 발생함.

사용자를 그룹에 구성원을 추가하는 SQL 구문을 추가
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. 11.4 이전 버전에서 사용자를 그룹에 추가하기 위해서는 DROP USER 후CREATE USER GROUPS를 실행하는 것이 필요함.
#. HA 환경에서 메소드 호출 결과가 동기화 되지 않음.
#. 마스터와 슬레이브에서 별도 작업 필요 위와 같은 이유로 새로운 SQL구문이 필요하여 다음과 같이 구현함.

.. code:: 

   ALTER USER user_name 
   [PASSWORD password] |
   [ADD MEMBERS user_name {, user_name}...] |
   [DROP MEMBERS user_name {, user_name}...]
   [COMMENT 'comment_string']

#. DBA/DBA 그룹 구성원
   -  DBA/PUBLIC 그룹 삭제
   -  DBA 그룹에 구성원 추가

#. 일반 사용자
   - 자신의 그룹에만 구성원 추가/삭제

Optimizer
~~~~~~~~~

조인 순서를 지정하는 LEADING 힌트추가
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Opitimizer에게 특정 테이블들을 실행 계획의 prefix로 사용하게 하여 ORDERED 힌트보다 유연한 테이블 조인 순서를 제어 가능하도록 함.

.. code:: sql

   SELECT /*+ LEADING(e j) */ *
   FROM  employees e, departments d, job_history j
   WHERE e.department_id = d.department_id
   AND e.hire_date = j.start_date;

- 힌트 무시 조건:

- 조인 그래프의 종속성으로 인해 지정된 테이블들을 먼저 조인할 수 없는 경우
- 여러 LEADING 힌트가 있는 경우 첫 번째만 적용
- ORDERED 힌트가 있으면 모든 LEADING 힌트 무시

통계 정보에 테이블의 각 컬럼 NDV(Number of Distinct Values, 고유 값 개수)를 추가
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Opitimizer가 더 정확한 실행 계획을 수립할 수 있도록 상세한 통계 정보가 제공되도록 기능을 추가함.

.. code:: sql

   ;info stats t123
   /* 각 컬럼별 NDV 정보 표시 */
   Attribute: col3 (INTEGER)
      Number of Distinct Values: 1

   Attribute: col2 (INTEGER)
      Number of Distinct Values: 501

   Attribute: col1 (INTEGER)
      Number of Distinct Values: 10000

**trace info** 에 디스크 패치 시간 *fetch_time* 항목 추가(전체 처리 시간에서 디스크 패치 시간 확인 가능)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

전체 실행 시간 중 디스크 패치 작업이 차지하는 시간을 확인할 수 있도록 개선함.

.. code:: sql

   Trace Statistics:
   SELECT (
        time: 840,          -- 전체 실행 시간
        fetch: 44408,       -- fetch 횟수
        fetch_time: 64,     -- fetch에 소요된 시간
        ioread: 0           -- IO 읽기 횟수
   )
   SCAN (table: dba.t111), (
        heap time: 681,     -- 힙 처리 시간
        fetch: 44408,       -- fetch 횟수
        ioread: 0,          -- IO 읽기 횟수
        readrows: 1010000,  -- 읽은 행 수
        rows: 1010000       -- 전체 행 수
   )

Hidden Column 관련 서브 쿼리 재작성 방법 개선(ORDER BY 절)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ORDER BY절 컬럼의 Hidden Column 속성 관리 문제로 서브 쿼리가 중복으로 재작성 되는 것을 개선함.

.. code:: sql

   UPDATE /*+ recompile */ t1 
   SET c2 = 1 
   WHERE c1 = (SELECT c1 FROM t2 ORDER BY c2, c3 LIMIT 1);

#. 한 번의 적절한 쿼리 재작성만 발생
#. 불필요한 중첩 서브쿼리가 생성되지 않음

.. _11_4_changes_add_feature_perf:

Performance
~~~~~~~~~~~

redo recovery 처리를 단일 스레드에서 병렬 처리로 변경하여 성능 향상
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. CUBRID의 REDO 복구는 현재 단일 스레드로 실행되지만, 서로 다른 데이터 페이지에 적용되는 REDO 로그는 동기적으로 적용할 필요가 없어서 이 과정을 병렬화하여 성능 개선.
#. REDO의 비중이 크고 병렬 지수가 좋으면 개선 효과가 큼.

correlated scalar subquery의 query result-cache를 사용하여 성능개선
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

상관 서브쿼리는 메인 쿼리의 각 행에 대해 개별적으로 실행되어 값을 가져오므로, 중복된 입력 값이 많을 경우 동일한 쿼리가 반복 실행되어 성능 저하를 유발할 수 있음. 이를 해결하기 위해 결과 캐시(Result Cache)를 적용하여 불필요한 반복 실행을 제거함.

CTE에서 query result-cache를 사용할 수 있도록 확장
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. 기존 결과 캐시(result cache) 적용 방식을 확장하여 CTE(Common Table Expression) 서브 쿼리에도 캐시를 적용.
#. CTE 서브 쿼리의 결과를 캐시하여 성능을 개선.

-  ``/*+ QUERY_CACHE */`` 힌트를 CTE 쿼리에 적용하면 기존에는 하나의 쿼리에 하나의 결과 캐시만 적용되던 방식에서, CTE의 서브 쿼리에도 캐시가 적용됨.

uncorrelated subqueries에서 query cache를 사용할 수 있도록 확장
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CTE 쿼리에 적용된 결과 캐시(result cache)를 비상관 서브쿼리(uncorrelated subqueries)에도 확장 적용.

결과 캐시가 참조되는지 여부를 trace 정보에 표시
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

결과 캐시(result cache)를 참조하는 쿼리를 실행할 때, trace 정보에 참조 횟수(reference count)가 표시되도록 지원.

.. code:: sql

   Trace Statistics:  
   SELECT (time: 23, fetch: 156, fetch_time: 0, ioread: 32)  
      SCAN (table: public.game), (heap time: 17, fetch: 124, ioread: 31, readrows: 8653, rows: 5676)  
      SCAN (hash temp(m), build time: 0, time: 2, fetch: 0, ioread: 0, readrows: 359, rows: 340)  
   UNION (time: 0, fetch: 8, fetch_time: 0, ioread: 0)  
      SELECT (time: 0, fetch: 0, fetch_time: 0, ioread: 0)  
      RESULT CACHE (reference count : 1)  
      SELECT (time: 0, fetch: 0, fetch_time: 0, ioread: 0)  
      RESULT CACHE (reference count : 1)

PL/CSQL, JAVA SP
~~~~~~~~~~~~~~~~

ORACLE의 PL/SQL과의 호환성을 위해서 CUBRID PL/CSQL 기능을 제공. 문법 및 사용법은 메뉴얼 참조 바람.

Java SP 서버에서 JNI를 로드하는 클래스를 다시 로드하지 않도록 새 ClassLoader 추가( loadjava 옵션추가 : *-j* 또는 *–jni* )
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Utility
~~~~~~~

unloaddb 성능 향상
^^^^^^^^^^^^^^^^^^

단일 스레드로 동작되던 unloaddb utility를 멀티 스레드 방식으로 변경함으로써 데이타 익스포트 시간을 현저하게 줄일 수 있도록 기능을 개선함.

#. unloaddb 옵션

   - ``--thread-count`` : 동시 실행 스레드 수(0~127)
   - ``enhanced-estimates`` : 정확한 레코드 수 예측(verbose 모드 전용)

#. 성능 모니터링

   - ``Elapsed`` : 전체 실행 시간 
   - ``Fetch`` : 서버 fetch 시간/횟수 
   - ``Write`` : 파일 쓰기 시간 
   - ``Add/Get L`` : List 작업 대기 시간/횟수 
   - ``Add/Get Q`` : Queue 작업 대기 시간/횟수 
   - ``to obj`` : DB_VALUE 변환 시간
   - ``to str`` : plain text 변환 시간 

메모리 모니터링 기능 추가
^^^^^^^^^^^^^^^^^^^^^^^^^

#. cubrid memmon utility는 서버 프로세스에 할당된 힙 메모리 사용량을 출력.
#. 시스템 파라미터 **enable_memory_monitoring** 가 yes로 설정된 경우, 서버 메모리 모니터링 모듈은 힙 메모리 총 사용량과 메모리 할당이 발생한 소스 코드 및 라인 정보를 기준으로 세부적인 메모리 할당 정보를 추적 관리.
#. 이를 통해 utility를 실행하는 시점의 서버 힙 메모리 사용 현황을 확인.
#. 사용 방법 및 상세 내용은 메뉴얼 참조.

테이블의 각 컬럼의 NDV(Number of Distinct Values)를 인쇄하는 “info ndv” CSQL 세션 명령을 추가
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

테이블의 특정 컬럼에 대한 NDV(Number of Distinct Values, 고유값 개수)를 출력하는 CSQL 세션 명령 추가.

#. ``info ndv <table_name>`` 명령어를 실행하면, 해당 테이블의 컬럼별 고유값 개수(NDV)를 출력.
#. 내부적으로 다음과 같은 쿼리를 실행하여 NDV를 계산.

   .. code:: sql

      SELECT /*+ SAMPLING_SCAN */ 
         COUNT(DISTINCT host_year), COUNT(DISTINCT event_code), COUNT(DISTINCT athlete_code),
         COUNT(DISTINCT stadium_code), COUNT(DISTINCT nation_code), COUNT(DISTINCT medal),
         COUNT(DISTINCT game_date),COUNT(*)
      FROM public.game;

#. 실행 예시

   .. code:: sql

      csql> ;info ndv public.game 

      Query : SELECT /*+ SAMPLING_SCAN */ count(distinct [host_year]), count(distinct [event_code]), count(distinct [athlete_code]), count(distinct [stadium_code]), count(distinct [nation_code]), count(distinct [medal]), count(distinct [game_date]), count(*) FROM [public.game] 

      Number of Distinct Values 
      **************** 
      Class name: public.game 
      host_year (5) 
      event_code (393) 
      athlete_code (6677) 
      stadium_code (116) 
      nation_code (115) 
      medal (3) 
      game_date (84) 

      total count : 8653 

      Committed.

SHOW INDEX CAPACITY 및 diagdb에 fence key 정보 추가
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``SHOW ALL INDEXES CAPACITY OF [schema_name.]table_name;`` 를 사용한 결과 항목에 ``Num_fence_key``\ 를 추가. 기존에는
``Num_fence_key``\ 값을 포함하여 용량을 계산하였으나 이를 포함하지 않도록 함. 

볼륨 생성 시간이 새롭게 추가됨에 따라 diagdb Utility 사용 시 볼륨 생성 시간을 추가로 출력하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

디스크 볼륨 헤더, 활성로그 볼륨, 보관로그 볼륨에 각 볼륨들의 생성시간을 의미하는 정보 추가.

- before

  .. code:: shell

     $ cat diag.txt | grep "creation time"
        Database creation time = Mon Aug 12 20:46:32 2024
        Database creation time = Mon Aug 12 20:46:32 2024
        Database creation time = Mon Aug 12 20:46:32 2024
        Database creation time = Mon Aug 12 20:46:32 2024

- after

  .. code:: shell

     $ cat diag.txt | grep "creation time"
        Database creation time = Mon Aug 12 20:46:32 2024
        Volume creation time = Mon Aug 12 20:46:37 2024
        Database creation time = Mon Aug 12 20:46:32 2024
        Volume creation time = Mon Aug 12 20:46:37 2024
        Database creation time = Mon Aug 12 20:46:32 2024
        Volume creation time = Mon Aug 12 20:46:37 2024
        Database creation time = Mon Aug 12 20:46:32 2024
        Volume creation time = Mon Aug 12 20:46:37 2024

single server 환경(HA가 아닌 환경)에서 비정상 종료 시 cub_server 프로세스를 자동 재시작 할 수 있도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. 현재 HA를 구성하여 서비스중이 아닌 single server로 운영중일 경우, cub_server가 비정상 종료되면 수동으로 재시작해야 함.
#. 이를 자동으로 재시작 할 수 있는 기능 추가.

-  특이 사항

#. 정상 종료시에는 자동으로 재시작 되지 않음.
#. 시스템 파라미터를 통해 기능 ON/OFF 가능.
#. 짧은 시간 내 반복적으로 종료될 경우, 일정 횟수 이상 실패시 재시작 하지 않음.

Broker, CAS, CMS
~~~~~~~~~~~~~~~~

클라이언트로 전달하는 fetch size를 설정하는 NET_BUF_SIZE broker 파라미터 추가
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Broker에 새로운 파라미터인 NET_BUF_SIZE를 추가 도입.

#. 설정 가능한 값 

   - 16K(기본값)
   - 32K
   - 48K
   - 64K

브로커별로 ACL을 설정할 수 있는 파라미터 추가
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Broker 마다 ACL 설정 값을 추가하여 ACCESS_CONTROL이 ON 인 경우에도Broker에 접속하는 클라이언트를 모두 허용하는 방법을 제공.

#. cubrid_broker.conf의 [broker] 섹션에서 ACCESS_CONTROL이 ON일 때, 각 브로커에 대해 새로운 ACL 파라미터인 ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER를 설정
#. 파라미터는 DENY 또는 ALLOW 값을 가질 수 있으며, 기본 값은 DENY

.. code:: shell

   $ cubrid broker acl status 
   ACCESS_CONTROL=ON 
   ACCESS_CONTROL_FILE=cubrid_acl.conf 

   [%query_editor] 
   ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER=DENY 
   testdb:dba:acl_ip_list.conf 

   CLIENT IP LAST ACCESS TIME 
   ========================================== 
   172.29.80.1 
   192.168.0.31 
   172.31.0.175 

   [%broker1] 
   ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER=ALLOW 

   ++ cubrid broker acl: success

- ``%query_editor`` 브로커는 ``DENY`` 로 설정되어 특정 IP만 허용
- ``%broker1`` 은 ``ALLOW`` 로 설정되어 모든 IP의 접속을 허용

TLS v1.2 클라이언트 이상을 지원하도록 CMS SSL 프로파일 변경
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CUBRID Manager Server(**CMS**)와 CM/CA는 보안상의 이유로 **SSL**\ 을 사용하여 통신하는데 TLS v1.0에서 TLS v1.2를 지원하도록 변경

Others
~~~~~~

백업본을 기반으로 신규 DB명으로 복구하는 스크립트 제공
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. 데이터 복구가 필요할 때 ``백업본``\ 을 운영 서버에 복구하려면 추가 계정 생성, 추가 엔진 설치 등 번거로운 작업이 필요
#. 이러한 불편을 해소하기 위해, 기존 CUBRID 설치 계정에서 ``백업본``\ 을 복구하고 새로운 DB명으로 변경할 수 있는 스크립트를 제공

- 사용법: ``sh rename_to_newdb.sh [OPTION] ASIS_DBNAME TOBE_DBNAME``
- 옵션:

  - ``-F``: 새 데이터베이스가 생성될 디렉토리의 절대 경로 지정
  - ``-B``: 백업 파일이 있는 디렉토리의 절대 경로 지정 (미지정 시 현재 작업 디렉토리에서 검색)
  - ``-d``: 지정된 날짜까지의 데이터베이스 상태로 복구
  - ``-l``: 복구할 백업 레벨 지정
  - ``-p``: 로그 아카이브가 없는 경우 부분 복구 수행
  - ``-k``: TDE 복구 시 사용할 키 파일(_keys)의 경로

사용자 편의를 위해 cubrid_host.conf 파일에 “0.0.0.0 your_hostname” 추가와 에러메시지 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

cubrid_host.conf의 사용자 호스트에 대해 대소문자 구분없는 hostname 구현
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

db_serial의 att_name 컬럼명을 attr_name으로 변경( 변경전: att_name)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

정렬 단계에서 TDE에 대한 불필요한 고정 페이지 헤더 제거
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

영어, 한국어 이외의 언어로 작성된 메시지 삭제
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

demodb_schema 파일에 dont_reuse_oid table option 제거
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _11_4_changes_spec:

스펙변경
--------

SQL
~~~

CHAR 타입의 최대 문자 개수를 2048(문자개수를 2048개)로 제한( 변경 전 최대 : 268435456 )
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. CHAR 타입의 최대 길이가 현재 256MB로 타 DBMS (oracle, mysql, postgres) 대비 상당히 큰 편으로, 문자셋이 utf8인 경우 메모리 할당이 최대 1GB에 도달.
#. 최대 길이의 CHAR타입으로 설정된 컬럼이 2개 이상인 경우, INSERT 구문에서 2GB가 넘는 메모리가 할당되어 메모리 오버플로우가 발생

이러한 메모리 할당 관련 문제와 서버로 전송 효율 관련 성능 문제를 해결하기 위해 CHAR타입에 대한 스펙을 변경:

  - CHAR타입의 최대 길이를 **256MB에서 2048** 로 축소함.

LOB컬럼 값의 LOB 로케이터를 절대 경로에서 상대 경로로 변경
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

현재 LOB file directory의 위치가 변경되는 경우 database 내의 모든 LOB column에 대한 검색자 (locator)를 변경해야 한다는 단점.

**수정내용:**

#.  locator 저장 방식 변경: 절대 경로에서 상대 경로로 변경 
#.  LOB 컬럼에는 상대 경로로 locator를 보관 
#.  LOB file의 내용을 참조하는 경우, LOB Base Path와 상대 경로를 붙여서 검색 

   - **LOB Base path:** /cubrid/demo/lob
   - **Locator:** ces_272/public.t1.00001720143587746537_3683 
   - **참조되는 FILE:** ``(LOB Base path) + locator``
       /cubrid/demo/lob/ces_272/public.t1.00001720143587746537_3683

Utility
~~~~~~~

csql 실행 상태에서 SQL문 또는 PL/CSQL 문장(create문, body문 등) 내에서 csql 세션 명령어가 인식되도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``cubrid plandump -s`` 옵션으로 특정 plan 삭제 가능. ``cubrid plandump -d`` 처리 변경 ( 변경전 : plan을 출력후 삭제, 변경후 : plan을 삭제 )
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**개선사항**

``plandump`` 유틸리티에서 SQL 실행 계획을 출력하고, 특정 플랜 캐시를 삭제하는 기능을 추가.

**신규기능**

#. ``plandump`` 실행 시 SQL 실행 계획을 항상 출력하도록 개선
#. ``-s`` 옵션을 추가하여 특정 플랜 캐시를 삭제할 수 있도록 기능 확장

**사용방법**

#. **실행 계획 출력**

   .. code:: shell

      $ cubrid plandump demodb
      ...
      Entries: XASL_ID = { sha1 = { 6290f2b1 44f088e5 d37d6f0f c8303155 9ef2096a }, time_stored = 1715925895 sec, 58124 usec }
      ...
      sql plan text = Sequential scan(public.game dba.game)
      ...

#.  **특정 플랜 캐시 삭제 후 플랜 캐시 목록 출력**

    .. code:: shell

       $ cubrid plandump -s '6290f2b1 44f088e5 d37d6f0f c8303155 9ef2096a' demodb 
       ... 
       /* delete specific plan cache and display current plan caches. */ 
       ...

교착 상태(deadlock) 발생 시 DB명_latest.event 로그파일에 기록되는 잠금관련 정보를 간소화 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. 교착 상태(Deadlock) 발생 시, 서버 로그 파일(``db명_latest.event``)에 기록되는 로그를 개선하여 **교착 상태에 직접 기여한 잠금 정보만 출력** 하도록 변경.
#. 기존 방식은 트랜잭션이 보유한 모든 잠금 정보를 출력하여 분석이 어려웠으나, **대기가 발생하는 잠금 정보** 만 순차적으로 기록하는 방식으로 개선.
#. **교착 상태의 피해자(Victim) 트랜잭션** 을 명확히 표시하여 원인 분석이 용이하도록 개선.

Broker, CAS, CMS
~~~~~~~~~~~~~~~~

**SQL LOG 작성 시점 변경** - SQL LOG에 먼저 작성한 다음 prepare/execute 실행하여 core 발생시에 sql log로 확인 가능하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. CAS가 prepare/execute 실행 중 core dump로 다운될 경우, 해당 SQL 문이 SQL 로그에 기록되지 않는 문제가 발생함.
#. CAS가 prepare/execute 완료된 후에야 로그를 기록하기 때문에 문제를 유발한 쿼리를 찾기 어려움.
#. prepare/execute 실행 전에 SQL 로그를 먼저 기록하게 순서를 변경하여 CAS가 다운되더라도 실행된 SQL을 추적할 수 있도록 개선.

.. _11_4_changes_improvements:

개선사항
--------

SQL
~~~

테이블에 X_LOCK이 발생할 경우, 해당 테이블에 대해서 select가 수행되지 않는 현상 개선( 불필요한 행에 X-LOCK이 설정되지 않도록 개선)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

LOCK이 생성되는 시점이 너무 이르기 때문에 불필요한 LOCK일 발생하고 있어서 모든 조건들이 평가된 후에 불필요한 LOCK을 해제하도록 개선함.

Where 조건절에서 like문의 조건 값으로 Javasp 함수를 사용하는 경우에 인덱스 스캔을 수행하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: sql

   CREATE OR REPLACE FUNCTION stringTest(x String) RETURN String AS LANGUAGE JAVA NAME 'SpTest.typeteststring(java.lang.String) return java.lang.String';

   CREATE TABLE tbl (ord INT, col_int INT, col_char char(1));
   CREATE INDEX i_tbl ON tbl (ord);
   CREATE INDEX i_tbl_char ON tbl (col_char);

   INSERT INTO tbl VALUES (1,10,'a');
   INSERT INTO tbl VALUES (2,10,'b');
   INSERT INTO tbl VALUES (3,10,'c');
   INSERT INTO tbl VALUES (4,10,'d');
   INSERT INTO tbl VALUES (5,10,'e');

   SELECT count(*) AS "like" FROM tbl WHERE col_char LIKE (SELECT stringTest('a') FROM dual);

Where 조건절에서 ``<=`` 와 ``>=`` 조건 사용시에  조건 컬럼에 함수 인덱스가 있는 경우에 범위 스캔을 수행하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. 함수 인덱스와 함께 <= 및 >= 조건을 사용할 때 발생하는 실행계획 최적화 문제로 플랜 생성기가 범위 스캔을 수행하지 않고 인덱스가 처음부터 <= 조건까지 스캔하는 계획을 생성.
#. 이는 함수 인덱스를 사용하는 범위 검색 쿼리의 성능에 중대한 영향을 미치는 최적화 문제로 개선함.

시스템 테이블과 시스템 뷰 테이블(_db_class, db_root, dual 등)에 대해서 ``for update`` 구문을 사용 못하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

뷰 생성 시 데이터 타입의 정합성을 체크하지 않도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. 이전에는 뷰를 생성할 때, 각 컬럼의 타입 변환 가능 여부를 즉시 체크함.
#. 생성 시점에 타입 체크를 않고 실행 시점에 타입 변환이 불가능한 경우 에러 처리함.
#. 타입 변환이 불가능한 경우, 뷰 생성 시 에러 → 뷰 실행 시 에러 발생으로 수정.

``CREATE VIEW`` 의 select절에 NULL 값 사용이 가능하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: sql

   CREATE VIEW a_view( col1 ) AS select NULL as col1 from a_tbl;


**ALTER TABLE MODIFY** 를 통해 테이블 컬럼 속성 변경시 ``AUTO_INCREMENT`` 와 ``DEFAULT`` 을 공존할 수 없도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. **CREATE TABLE** 및 **ALTER COLUMN**  구문 수행 시 ``AUTO_INCREMENT`` 및 ``DEFAULT`` 속성을 동시에 사용할 경우 아래와 같은 에러가 발생
#. **ALTER TABLE MODIFY** 구문을 사용하여 ``AUTO_INCREMENT`` 및 ``DEFAULT`` 속성을 개별적으로 실행하면 해당 컬럼에 두 속성이 공존. 이 문제로 인해 ``AUTO_INCREMENT`` 및 ``DEFAULT`` 속성이 있는 테이블을Unload 한 다음 Load 하면 테이블이 생성되지 않는 문제가 발생하여 이를 개선

DB볼륨 및 로그의 생성 시간을 확인할 수 있도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

‘SHOW VOLUME HEADER’, ‘SHOW LOG HEADER’, ’SHOW ARCHIVE LOG HEADER’의 ’Creation_time’도 볼륨 생성 시간을 출력하도록 수정

필터 인덱스를 생성할 때 where 절의 길이 제한을 개선
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. filtered 인덱스 사용 시 where 절의 길이 제한이 매뉴얼과 다른 문제 발생
#. 사용자가 입력한 문자열 길이가 아닌, 재작성된 문자열의 길이를 측정하여 발생하는 문제(255자 제한)
#. 길이 제한을 제거하여 발생되는 문제를 개선함

Limit 절에 바인드 변수와 계산식이 있는 경우에 sort limit 최적화 적용되게 개선
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: sql

   drop table if exists tbl1;
   create table tbl1 (col1 int, col2 int);
   insert into tbl1 select rownum, random() % 100000 +1 from db_class a, db_class b, db_class c, db_class d limit 100000;
   create index idx on tbl1(col1);

   prepare stmt from
   'SELECT a.col1, a.col2
   FROM tbl1 a
      LEFT JOIN tbl1 b ON a.col1 = b.col1
      LEFT JOIN tbl1 c ON a.col1 = c.col1
   ORDER BY a.col2,a.col1
   LIMIT ?*10,?';

   execute stmt using 10,10;

Optimizer
~~~~~~~~~

데이터 삭제될 때 테이블 통계 정보의 “Total objects” 값이 업데이트 되지 않는 것을 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

데이터 삭제 시 통계 정보의 객체 수가 업데이트 되지 않아서 실행 계획을생성할 때 부정확한 통계 정보를 사용하게 되어 성능에 영향을 미칠 수 있는 문제를 개선함.

“;info stats 테이블명” 명령의 “Total objects”의 값을 unique index의 key 개수를 사용
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

통계 정보를 업데이트 할 때 전체 데이타 스캔으로 인헤 성능 저하가 발생하는 문제를 줄이기 위해, unique index의 key 개수를 사용하도록 개선함.

b-tree 통계 정보를 수집 시 정확도를 높이기 위해서 샘플링 페이지 수를 늘림
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. B-tree 통계 수집 시 샘플링 페이지 수를 증가시켜 정확성과 성능을 개선
#. 동시에 불필요한 로직(동일한 페이지 처리 로직, 셈플링 페이지 선택 확률 계산 루틴)을 삭제하여 성능 개선

데이터(heap)에서 추출한 NDV(Number of Distinct Values)가 먼저 사용하여 정확성을 높임
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. **Heap에서 NDV(고유 값 수)를 우선적으로 사용** 

   -  **NDV(Number of Distinct Values)**: 특정 컬럼에서 서로 다른 고유한 값의 개수.
   -  먼저 Heap에서 NDV 값을 추출해 사용하고, Heap에서 얻을 수 없으면 B-tree에서 추출한 NDV를 사용하도록 변경.

#. **예외 처리**

   -  문자열 타입(``char`` 등)의 길이가 4000자를 초과하면 Heap에서 NDV를 추출하지 않음.(긴 문자열의 경우 성능이나 정확도 문제 때문)

#. **객체 수 세는 방법 변경**

   -  샘플링 스캔 힌트를 사용해 객체 수(count)를 계산하도록 변경

Heap에서 얻은 NDV를 우선적으로 활용하여 선택도를 설정하고, 성능 및 정확도를 높이기 위해 긴 문자열 예외 처리와 샘플링 기반 카운트 방식을 도입.

규칙 기반 최적화( rule base optimization )를 최소화하고 경험적 요소( 통계추정, Join-첫번째 테이블 선택, PK에 대한 비용 없는 계산 등)를 제거하여 정확도를 높임
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. **RBO(규칙 기반 최적화) 관련 요소 제거**

   - Heuristic Factor(경험적 가중치) 제거
   - 통계 기반 추정치 제거
   - First Node 우선 처리 루틴 제거
   - 계산식에 있던 휴리스틱 제거
   - 고유 인덱스(unique index)의 비용을 0으로 처리하던 로직 제거
   - 선택도, 데이터 버퍼, 내부 페이지 수 기반의 IO 카운트 계산 로직 제거

#. **RBO 자체 제거**

   - 원래는 비용 차이가 1.x 이내일 때만 RBO 사용.
   - 이제 RBO를 완전히 제거하고, 비용 기반 최적화(CBO)에 집중.
   - 인덱스 스캔이 가능한 경우에도 순차 스캔(seq_scan) 플랜 생성 허용. (기존 RBO에서는 인덱스 스캔을 우선시함.)

#. **기타**

   - 부분 탐색(partial search) 시 플랜 비용 확인하는 루틴 추가
   - LIMIT 처리 시, 다음 컬럼의 누적 선택도(selectivity)를 잘못 사용하는 버그 수정

RBO(규칙 기반 최적화)를 제거하고, 비용 기반 최적화(CBO)를 강화하는 방향으로 최적화 로직 개선

실제 실행 시간을 기반으로 일부 비용 공식을 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

조인 순서 벤치마크(Join Order Benchmark) 결과를 반영해 실제 실행 시간에 맞춰 비용 계산식을 개선하고, 몇 가지 최적화 기능 및 보완 작업 진행

#. 비용 계산식(cost formula) 수정

   - **인덱스 필터 스캔 선택도 반영**: 인덱스 스캔 시 데이터 I/O 비용 계산 시 선택도를 고려하도록 변경.
   - **‘NOT LIKE’** 연산자 선택도 추가.
   - **함수 기반 인덱스(function-based index)** 선택도 추가.
   - **샘플링 페이지 수** 5000으로 증가 - 통계 정확도 향상.
   - **NDV(고유 값 수)** 중복 많을 때 가중치 조정 - 샘플링 데이터의 중복도가 1% 이하로 높을 경우, 통계 가중치(weight) 조정.
   - **SSCAN_DEFAULT_CARD** 도입: **NL JOIN(중첩 루프 조인)** 시 카디널리티(결과 건수) 추정이 너무 낮으면 비효율적인 플랜 방지.

#. 추가 개선

   - trace 정보에 **fetch_time** 추가로 실행 시간 추적 강화.
   - ``index_ss`` 힌트 없이도 **인덱스 스킵 스캔(index skip scan)** 선택되도록 개선.
   - **Sort Merge Join** 활성화: 옵티마이저에서 Sort Merge Join 방식을 사용할 수 있도록 변경.

비용 계산식을 실제 실행 시간 기반으로 조정해 정확도를 높이고, 인덱스 활용 및 조인 방식 등 최적화를 추가한 개선 작업

LIMIT 절에 대한 비용 공식을 추가
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**LIMIT** 절을 고려한 비용 계산식을 추가하고, LIMIT가 있는 경우 실행 계획 선택 방식(최적화 기준)을 개선

#. LIMIT 절 관련 비용(cost) 및 카디널리티(cardinality) 설정:

   - LIMIT 절이 있는 서브쿼리의 카디널리티를 **LIMIT 값** 으로 설정 
   - 예) ``LIMIT 10`` 이면, 해당 서브쿼리의 결과 건수 추정을 10으로 설정.

#. LIMIT 있을 때 규칙 기반(Rule-based) 최적화 강화:

   - LIMIT이 붙은 경우, 첫 몇 개의 행(row)을 빠르게 가져올 수 있는 실행 계획에 우선순위 부여
   - **ORDER BY SKIP** 사용. 
   - **SORT-LIMIT** 최적화 적용. 
   - 전체 데이터를 읽는 것보다 빠르게일부만 가져오는 플랜을 선호하도록 변경. 

#. SSCAN_DEFAULT_CARD 값 변경 (기본 카디널리티):

   - 1000 → 100으로 변경 - 시퀀셜 스캔(SSCAN)이 너무 불리한 플랜이 선택되지 않도록 함.

LIMIT 절을 고려한 비용 및 카디널리티 설정을 도입하고, 소량 데이터 조회 성능을 높이기 위해 **LIMIT 최적화** 및 기본 카디널리티 값을 조정

이미 논리적으로 확인된 조인 조건을 제거하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

조인(predicate) 조건 중 논리적으로 이미 평가된 중복 조건을 제거하여 최적화 하는 개선 사항

.. code:: sql

   a.col1 = b.col1 AND b.col1 = c.col1 AND a.col1 = c.col1;

위와 같은 조건은 하나만 있어도 충분하지만, 3개의 조건이 모두 남아있어 불필요한 필터(term)가 발생.

쿼리 최적화 단계에서 불필요한 ’INNER JOIN 제거’와 조인 유형을 제거
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

INNER JOIN 제거 최적화(Eliminate INNER JOIN optimization)에 대한 동작 조건 및 예외 사항

#. **INNER JOIN 제거 최적화** 

   - 불필요한 INNER JOIN을 제거하는 최적화
   - 예를 들어, 테이블이 하나만 있는 경우 JOIN 자체가 무의미하므로 제거함.

#. **최적화 동작 방식(조건)** 

   - 첫 번째 테이블에 대해 INNER JOIN 제거 최적화가 수행됨.
   - 두 번째 테이블의 조인 유형(join type)이 INNER JOIN이면 함께 제거됨.

#. **예외 조건** 

   - 두 번째 테이블의 조인 유형이 OUTER JOIN이면 이 최적화가 수행되지 않음.
   - OUTER JOIN은 결과에 영향을 줄 수 있으므로 무조건 유지.

#. 코드 예시

   - AS-IS

     .. code:: sql

        SELECT /*+ ORDERED */ c.c2 FROM t2_parent p INNER JOIN t2_child c ON c.id = p.id

   - TO-BE (INNER JOIN이 제거됨, 불필요한 JOIN 구문이 사라지고 테이블만 남음)

     .. code:: sql

        SELECT /*+ ORDERED */ c.c2 FROM t2_child c
        
불필요한 INNER JOIN을 제거하는 최적화를 수행하되, 두 번째 테이블이 OUTER JOIN이면 이 최적화를 적용하지 않도록 동작을 명확히 정의

Performance
~~~~~~~~~~~

저장 프로시저의 플랜과 실행자를 재설계
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

현재 저장 프로시저에서 불필요한 **메서드 변환(Method Transformation)**  을 제거하려는 작업 이며 기존 방식에서는 저장 프로시저의 인자가 테이블 컬럼과 연관되면, 이를 인라인 뷰처럼 변환하고 **Lateral Join** 구조를 생성하였지만 이 방식은 성능 저하(비효율적인 인덱스 스캔, 잘못된 외부 조인 생성 등)를 초래하므로, 새로운 실행 계획을 도입하여 해결.

#. ``where`` 조건절 비교값으로 SP를 사용하는 경우 index scan 가능.
#. rewrite 시에 SP 호출을 테이블 스캔과 같이 변환 안되도록 수정.

기존 방식에서 불필요한 변환을 제거하고 성능을 개선하는 방향으로 저장 프로시저 실행 계획을 최적화하는 작업

다중 컬럼 인덱스 노드에서 레코드를 읽을 때 필요한 midxkey.buf 크기를 계산하는 프로세스를 개선하여 성능 향상
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

기존에는 다중 컬럼 인덱스에서 키 길이를 반복 계산해야 했지만, 각 컬럼의 ``OFFSET`` 을 추가하여 직접 참조하도록 개선. 이를 통해 **바이너리 검색, 키 필터링, DML 실행 시** 불필요한 연산을 줄여 성능 향상.

B-Tree 인덱스에서 키 읽기 개선
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. **Range-Scan 시 키 읽기 및 처리 방식 수정**:

   Range-Scan 과정에서 키를 읽고 처리하는 방식을 최적화하여 불필요한 연산을 줄임.

#. **통계 정보 갱신 및 용량 계산 시 키 읽기 최적화**:

   통계 정보 업데이트 및 인덱스 용량 계산 시 키 읽기 방식을 최적화.

#. **Range-Scan 시 ``upper_key`` 비교 횟수 감소**:

   Range-Scan 과정에서 ``upper_key``\ 와의 불필요한 비교를 줄여 성능 향상.

#. **인덱스 컬럼 ID 중복 비교 감소**:

   인덱스 컬럼 ID를 반복적으로 비교하는 작업을 줄여 연산 최적화.

리프 노드가 압축된 경우 공통 접두사를 BTREE_NODE_HEADER에저장하여, 반복 계산을 감소하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

현재 인덱스를 스캔할 때마다 **공통 접두사(common prefix)** 를 계산하고 있습니다. 하지만 노드가 분할(split)되거나 병합(merge)되지 않는 한 공통 접두사는 변하지 않으므로, 매번 계산하는 것은 비효율적임. 공통 접두사를 계산하려면 리프(leaf) 노드 내에서 **하한 펜스 키(lower fence key)** 와 **상한 펜스 키(upper fence key)** 가 존재하는지 확인하고, 두 키를 비교하여 몇 개의 열(column)이 공통인지 판단해야 함. 

이 개선작업의 목적은 **압축된 리프 노드에서 이진 검색(binary search)** 시 공통 접두사를 건너뛰어 성능을 향상시키는 것.

#. O-BE 구조에서는 공통 접두사를 ``BTREE_NODE_HEADER`` 에 저장하여, 추가적인 계산 없이 즉시 참조할 수 있도록 수정.

#. 수정한 방식으로 인덱스 스캔 성능 향상.

time_format() 및 date_format()의 성능 향상
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``STRCHCAT`` 매크로에서 사용하는 문자열 추가 연산 함수를 사용하지 않도록 하여 함수 호출에 발생하는 비용을 줄여 성능을 개선.

TRACE 활성화 시 쿼리 성능 개선
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``set trace on`` 상태에서 쿼리를 실행하면, ``set trace off`` 상태에서 동일한 쿼리를 실행할 때보다 성능이 급격히 떨어짐. TRACE의 통계 정보 수집에서 발생되는 오버헤드의 원인이 되는 고비용의 operation을 최대한 줄여 성능저하가 최소로 발생하도록 개선.

문자열 유형에 대한 불필요한 길이 검사를 제거
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

문자열 유형에 대한 크기를 확인하는 순서를 변경하고 단계를 축소하여 전체적인 조회 성능을 개선.

PL/CSQL, JAVA SP
~~~~~~~~~~~~~~~~

JNI의 세그폴트 오류 시 java SP 재 구동하게 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

여러 종류의 문자 인코딩(euckr, utf8)을 지원하도록 문자열 기능 보완
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

11.2 이전의 Java 프로그램에서는 문자열의 인코딩이 파일의 인코딩에 영향을 받았고 이로 인해 잘못된 문자셋 변환으로 문자열이 손상이 될 수 있었음. 11.2 이후, 문자열 처리는 UTF-8 인코딩으로 통일 되었고 여전히
많은 데이타베이스에서 UTF-8이 아닌 다른 인코딩을 사용하고 있어 이에 대한 지원이 필요. 

문자열을 byte 배열로 처리하여 다양한 문자 인코딩을 지원하도록 개선.

환경변수 CUBRID_TMP가 설정된 경우에 Java SP에서 java.io.tmpdir 설정 (CUBRID_TMP가 설정 안된 경우 $CUBRID/tmp로 설정)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``java.io.File.createTempFile()`` 함수가 임시 파일을 생성하려고 할 때 오류가 발생함. Junixsocket이 내부적으로 이 함수를 사용하여 임시 파일을 생성한 후, 파일을 곧바로 삭제함. 기본적으로 ``createTempFile()`` 함수는 ``/tmp`` 디렉토리에 파일을 생성하며, ``-Djava.io.tmpdir`` 옵션을 통해 파일이 생성되는 위치를 변경할 수 있음.

#. ``CUBRID_TMP``\ 가 설정되어 있으면 ``java_stored_procedure_jvm_options="-Djava.io.tmpdir=<path>"`` 옵션을 설정하더라도 ``CUBRID_TMP`` 값이 ``java.io.tmpdir``\ 에 반영.

#. ``cubrid javasp status`` 명령어에서 사용자 설정 값이 출력되며, ``java_stored_procedure_jvm_options``\ 를 통해 ``java.io.tmpdir``\ 을 설정한 경우에도 반영.

#. ``CUBRID_TMP``\ 가 설정되지 않으면, ``$CUBRID/tmp``\ 가 ``java.io.tmpdir``\ 으로 설정.

#. ``java.io.tmpdir`` 경로는 Java SP 서버 시작 시에만 초기화되며, 경로를 변경하려면 Java SP 서버를 재시작 해야 함.

Utility
~~~~~~~

csql의 환경 변수 이름 변경( 예 : FORMATTER => CUBRID_CSQL_FORMATTER )
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CSQL 환경에서 사용되는 환경 변수 FORMATTER, EDITOR, SHELL이 CUBRID와 직접적인 관련이 있는지 명확하지 않다는 문제가 있음. 이를 해결하기 위해 변수명 앞에 CUBRID_CSQL 접두사를 추가하여 CUBRID 관련 변수임을
명확히 표시.

- **변경 사항**:

  - EDITOR → CUBRID_CSQL_EDITOR
  - SHELL → CUBRID_CSQL_SHELL
  - FORMATTER → CUBRID_CSQL_FORMATTER

기존 변수명도 그대로 사용할 수 있어 기존 시스템과의 호환성을 유지.

lock_escalation 설정 값 만큼만 사용된 lock 리소스를 유지하도로 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``lock_escalation`` 환경 변수 설정 값이 초과되면 잠금이 해제되어도 할당된 메모리가 해제되지 않는 문제. 설정된 개수를 초과하면, 잠금이 해제될 때 즉시 메모리를 반환하도록 수정.

diagdb -d 9 에서 클래스(테이블)만 덤프하도록 수정 ( 예시: “diagdb -d 9 -n class-name” )
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

전체 힙 파일을 덤프하는 대신, 특정 클래스만 지정하여 덤프할 수 있는 기능을 추가.

- **사용방법**:

  - ``diagdb -d 9`` 명령어에서 클래스(테이블) 이름(사용자명 포함)을 파라미터로 받을 수 있도록 수정
  - ``diagdb -d 9 -n class-name`` → 해당 클래스만 덤프
  - 파티션 테이블인 경우: 모든 파티션을 덤프
  - 파티션의 일부인 경우: 해당 파티션만 덤프

cubrid.conf 설정 파일의 use_user_hosts=true 인 경우에 cubrid_hosts.conf 파일의 유효성 검증( IP 주소, Hostname, cubrid_hosts.conf )
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``cubrid_hosts.conf`` 파일의 **유효성을 CUBRID 시작 시점에 검사**\ 하여, 오류 발생 시 적절한 메시지를 출력하고 실행을 중지하도록 개선.

- **유효성 체크 항목**

  #. **파일 존재 여부**: 파일이 없으면 오류 발생
  #. **IP주소 검사**: IPv4 형식만 지원
  #. **Hostname 검사**

     - 최소 1자 ~ 최대 63자
     - 알파벳(A-Z, a-z), 숫자(0-9), 하이픈(-)만 허용
     - 시작과 끝에 하이픈(-) 불가
     - 예: ``cubrid``, ``node-1``, ``www.cubrid.com``

  #. **FQDN(Fully Qualified Domain Name) 검사**

     - 최대 255자 허용
     - 여러 개의 레이블을 점(``.``)으로 구분
     - 각 레이블은 Hostname 규칙을 따라야 함
     - 예: ``mail.server.cubrid.com``

- **제약사항**

  - Alias 미지원
  - ``#``\ 으로 시작하는 줄은 주석 처리

- **유효성 체크 적용 범위**
   
  - CUBRID 서비스 및 모든 관리 유틸리티(``service``, ``server``, ``broker``, ``createdb``, ``backupdb``, ``vacuumdb`` 등)

``cubrid spacedb`` 결과에서 HEAP, SYSTEM 결과 값 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

현재 데이터베이스의 파일 유형(``FILE_TYPE``)을 ``SPACEDB_FILE_TYPE``\ 으로 매핑하는 코드에서 일부 파일 유형이 잘못 분류되어 이전 버전과의 spacedb 결과와 다름. 매핑 문제를 올바르게 해결하여 “show heap capacity of” 명령의 heap page는 HEAP으로 계산, overflow page는 SYSTEM으로 계산되도록 수정.

HA
~~

ha_node_list 및 ha_replica_list를 로드할 때 발생할 수 있는 오류 메시지를 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

현재 ``ha_node_list`` 또는 ``ha_replica_list``\ 에 잘못된 정보가 포함되었을 때, ``master.err`` 로그에 일관된 오류 메시지만 출력되어, 원인을 파악하기 어려움. 오류 유형별로 명확한 오류 메시지를 출력하도록 수정.

- **AS-IS**

  -  모든 오류가 **“ha_node_list”: Unknown system parameter or bad value.** 로 출력됨

- **TO-BE**

  +--------------------------------------+-------------------------------+
  | 오류 메시지                          | 오류 유형                     |
  +======================================+===============================+
  | ha_node_list is empty                | ``ha_node_list``\ 가          |
  |                                      | ``NULL``\ 이거나 빈 문자열인  |
  |                                      | 경우                          |
  +--------------------------------------+-------------------------------+
  | cannot                               | ``ha_mode``\ 가 ``on``\ 인데  |
  | find (myhost) in the ha_node_list    | ``ha_node_list``\ 에          |
  |                                      | ``myhost``\ 가 없는 경우      |
  +--------------------------------------+-------------------------------+
  | group id of (ha_node_list, ha_replic | ``ha_node_list``\ 와          |
  | a_list) is different                 | ``ha_replica_list``\ 의 그룹  |
  |                                      | ID가 다른 경우                |
  +--------------------------------------+-------------------------------+
  | In replica node, (myhost) must not   | ``ha_mode``\ 가               |
  | be specified in the ha_node_list     | ``replica``\ 인데             |
  |                                      | ``ha_node_list``\ 에          |
  |                                      | ``myhost``\ 가 포함된 경우    |
  +--------------------------------------+-------------------------------+
  | ha_replica_list is empty             | ``ha_mode``\ 가               |
  |                                      | ``replica``\ 이고             |
  |                                      | ``ha_replica_list``\ 가       |
  |                                      | ``NULL``\ 이거나 빈 문자열인  |
  |                                      | 경우                          |
  +--------------------------------------+-------------------------------+
  | In replica node, (myhost) must be    | ``ha_mode``\ 가               |
  | specified in the ha_replica_list     | ``replica``\ 인데             |
  |                                      | ``ha_replica_list``\ 에       |
  |                                      | ``myhost``\ 가 없는 경우      |
  +--------------------------------------+-------------------------------+
  | In not replica mode,                 | ``ha_mode``\ 가 ``on``\ 인데  |
  | (myhost) must not be specified in    | ``ha_node_list``\ 에          |
  | the ha_replica_list                  | ``myhost``\ 가 포함된 경우    |
  +--------------------------------------+-------------------------------+

- **추가 변경 사항**

  - ``ha_mode`` 가 ``on`` 이면서 ``ha_node_list`` 에 ``myhost`` 가 있으면 **에러를 발생** 하도록 수정 (기존에는 replica 모드로 변경) 

HA Failover와 Failback에 수행시에 작성되는 에러 메시지를 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- **현재**

  - 현재 HA Failover/Fallback 시 불명확한 로그 메시지로 인해 원인 분석이 어려움.
  - 일부 케이스에서는 로그가 남지 않음.
  - 기존 메시지는 원인과 결과가 명확하지 않음 

- **개선사항**

  - **Failover (Slave → Master)** 와 **Failback (Master → Slave)** 을 명확히 구분
  - **진단(Diagnosis) 메시지** 와 **결과(Result) 메시지** 를 구분하여 출력
  - 로그 태그 추가: ``[Failover]``, ``[Failback]``, ``[Diagnosis]``, ``[Cancelled]``, ``[Success]`` 
  - **Failover 메시지** 는 **Slave 노드** 에 기록, **Failback 메시지** 는 **Master 노드** 에 기록 
  - 기존 메시지 누락 문제 해결 (Failback 시 로그 미출력, Ping check 관련 로그 추가 등)

로그 볼륨 헤더에 볼륨 생성 시간이 새롭게 추가됨에 따라 applyinfo 유틸리티 사용 시 볼륨 생성 시간을 추가로 출력하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

로그 볼륨 헤더에 볼륨 **생성시간** ( ``Vol creation time`` ) 정보가 추가됨에 따라 HA환경에서 **활성 로그볼륨** 과 **보관 로그볼륨**  정보를 출력하는 ``applyinfo`` 유틸리티 또한 볼륨 **생성시간** ( ``Vol creation time`` ) 정보를 추가로 출력하도록 수정

Broker, CAS, CMS
~~~~~~~~~~~~~~~~

연결된 CAS 연결에 broker ACL 변경 사항 반영하여 reload 시에 기존 연결을 그대로 사용 안되게 변경
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ACL을 변경하고 ``cubrid broker acl reload`` 명령어로 재로드 하면 새로운 연결 시에는 변경된 ACL 규칙이 적용되지만, 기존에 연결된 CAS(CUBRID Application Server) 세션은 변경된 ACL 규칙을 적용하지 않고 계속 유지되는 현상 발생. ACL 재로드 후 이미 연결된 CAS 연결은 트랜잭션이 완료된 후, 변경된 ACL을 적용하여 접근을 다시 제어하도록 수정.

변경된 conf (cubrid_broker.conf, cubrid_gateway.conf)이용해서 필요한 디렉토리를 생성하게 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. CUBRID Broker/Gateway는 여러 종류의 로그를 저장하기 위해 각기 다른 디렉터리를 사용.
#. 현재 CUBRID Broker와 CUBRID Gateway는 로그 디렉터리를 두 번 생성
#. 이로 인해 설정된 디렉터리와 실제 디렉터리의 불일치가 발생할 수 있음.
#. 로그 디렉터리 설정이 두 번 생성되지 않도록 수정하여 설정 파일에 정의된 값에 따라 정확한 디렉터리만 생성.

cubrid broker info 결과의 브로커 LOG 디렉토리가 항상 절대 경로로 표시하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``cubrid broker info`` 명령어는 브로커의 설정 정보를 보여주는데 이 명령에서 디렉터리 관련 정보(``LOG_DIR``, ``ERROR_LOG_DIR`` 등)는 다음과 동작.

- **초기 CUBRID 설치 및 기동 시**

  -  ``cubrid_broker.conf`` 파일에서 상대 경로로 설정한 경우, 브로커 관련 디렉터리 경로가 **상대 경로** 로 표시.

- **CUBRID 서비스를 종료 후 재기동 시**

  -  서비스 재기동 후에는 상대 경로에 ``$CUBRID`` 경로가 추가된 **절대 경로** 로 디렉터리 정보가 표시

- **개선사항**

  - CUBRID 설치 후 기동/재기동 여부와 관계없이 항상 **브로커 관련 디렉터리**들을 **절대 경로** 로 표시되도록 수정

‘cubrid broker info’ 결과를 확장하여ADMIN_LOG_FILE(broker 구동에 관한 시간 기록 파일 위치 파라미터)가 표시되도록 수정 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``cubrid broker info`` 명령어에 출력되는 매개변수 목록에
``ADMIN_LOG_FILE``\ 을 추가하여, 이 매개변수도 함께 출력 되도록 확장.

cub_manager에 의해 생성된 broker/cas 프로세스의 ppid를 1로 설정하여 좀비 프로세스를 방지
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CMS getlogfileinfo() API를 호출하면 동일한 SQL LOGFILE의 정보를 2번 리턴하는 것을 1번만 처리하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CMS ha_status() API가 Master,Slave,Replica 구성된 HA환경에서 Replica Node의 상태가 출력되도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Others
~~~~~~

Windows 빌드에서 vsnprintf()를 지원하도록 cub_vsnprintf() 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

오류수정
--------

SQL
~~~

``ALTER INDEX … REBUILD`` 실행 시 컬럼 추가에 대한 오류 메시지 출력 개선 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ROWNUM 연산 값이 NUMERIC 타입 범위를 초과할 경우 오류 메시지 출력하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ORDER BY를 포함하는 인라인 뷰와 스칼라 서브쿼리의 뷰 머징 시ROWNUM이 ORDER_BY_NUM()으로 잘못 변경되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

oracle_style_empty_string=yes 환경에서 ‘NULL \|\| 문자열’ 연산결과 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

oracle_style_empty_string=yes 환경에서 REPLACE 함수 결과가 NULL로 출력되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

함수 인덱스를 사용한 Covered Index 쿼리에서 발생하는 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

항상 false 또는 NULL 조건을 가진 CTE 쿼리(예: WHERE 0=1)에서 발생하는 코어 덤프 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

여러 테이블과 serial의 next_value를 UNION ALL 구문으로 사용하는 쿼리에서 발생하는 코어 덤프 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SELECT 쿼리에서 컬럼과 상수값을 연결할 때, 상수값 길이가 255자를 넘고 컬럼의 alias가 없을 경우 발생하는 코어 덤프 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

OUTER JOIN을 포함한 쿼리에서 뷰머징 처리 시 발생하는 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

2개의 계정에 동일 이름의 원격 서버가 생성된 경우, user schema를 생략할 때 원격 서버 찾기 동작 변경
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

‘SHOW CREATE VIEW’ 명령 실행 시 현재 사용자의 뷰와 public 사용자의 동일명 뷰가 함께 출력되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

string_max_size_bytes를 초과할 때 오류 메시지가 출력되지 않고 NULL이 반환되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

쿼리 최적화 실행 시 INNER JOIN 제거 과정에서 테이블 위치가 잘못 처리되는 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Prepared Statements 구문에서 호스트 변수 인자를 casting 함수에 사용할 때 발생하는 구문 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``DROP USER 사용자`` 실행 시 ``db_authorization`` 및 ``_db_auth`` 카탈로그 테이블의 사용자 정보도 함께 삭제되도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

2개 테이블 이상의 UPDATE JOIN 구문에서 분석 함수를 사용하지 못하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Oracle 스타일 LEFT OUTER JOIN이 SP 호출 및 WHERE 절에 호스트 변수(바인드 변수)가 사용될 때 INNER JOIN으로 재작성되지 않는 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ORDERBY_NUM()이 포함된 하위 쿼리가 뷰 머징 시 잘못 출력되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

조인 관계가 없는 테이블이 잘못 조인되어 잘못된 결과가 출력되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

optimization_level 설정값에 잘못된 값을 입력 시 오류가 출력되도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

WHERE 절의 동등 조건(=)과 ORDER BY 절에서 사용된 컬럼이 동일할 때 ROWNUM 값이 0으로 출력되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

WHERE 절에서 INNER JOIN과 Oracle 스타일 OUTER JOIN을 함께 사용하고, OUTER JOIN 조인 조건의 위치에 따라 발생하는 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

‘INSERT INTO tbl … SELECT … FROM 뷰테이블 … ON DUPLICATE KEY UPDATE’ 쿼리 실행 결과 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

뷰에 분석 함수가 포함될 때, 분석 함수 내부의 컬럼 순서 번호가 잘못되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

파이프 연산자(||)를 사용하는 범위 조건이 공통 범위 항목으로 축소되지 않는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Optimizer
~~~~~~~~~

PK(Primary Key)보다 더 좋은 인덱스를 선택하지 않는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Performance
~~~~~~~~~~~

SQL Rewrite 시 호스트 변수 개수가 변경되더라도 Result Cache 사용 가능하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Subquery Cache를 처리할 수 없는 경우, 제외하도록 처리
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

WITH 절을 참조하는 UPDATE / DELETE 쿼리에서 서브쿼리 캐시가 안되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

PREPARE 구문을 사용할 때 쿼리 캐시가 처리되도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Correlated Subquery에서 SP 함수를 사용할 때 쿼리 캐시가 가능하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

PL/CSQL, JAVA SP
~~~~~~~~~~~~~~~~

DATETIME 유형의 Java SP 매개변수에 DATETIMELTZ 값을 전달하는 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Java SP에서 호출되는 대상 Java 메서드 이름과 여는 괄호 사이의 공백으로 발생하는 NoSuchMethod 예외를 방지하기 위해 공백 제거
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Java SP에서 연관되지 않은 컬럼에 대해 지원하지 않는 인수 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Java SP에서 ‘Cannot allocate query entry any more’ 오류가 발생하지 않도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

loaddb 실행 중 오류 발생 시 종료 코드가 3으로 설정되도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

loaddb를 no-logging 옵션으로 실행한 이후 count(*) 값 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

unloaddb 실행 시 Reverse Unique Index의 주석이 누락되지 않도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

PK 및 auto_increment 설정된 컬럼을 unloaddb 실행 시 auto_increment 값이 1로 초기화되는 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

unloaddb –split-schema-files 실행 시 Unique Index가 없는 경우 ‘db명_schema_uk’ 파일이 생성되지 않도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

unloaddb에서 -i 및 –input-class-only 옵션을 함께 사용할 때 관련없는 ALTER SERIAL 문이 출력되지 않도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

serial의 current_val이 max_val과 같을 때 unloaddb/loaddb 실행 시 발생하는 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

loaddb에 오류 발생 시 멈추지 않고 계속 실행되는 문제를 오류 메시지 출력 후 멈추도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1MB 이상의 JSON 타입 데이터 처리 시 unloaddb에서 무한 루프가 발생하는 현상 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

일반 사용자가 뷰를 unload할 때 query_spec 소유자의 user_schema를 제거
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

테이블과 뷰가 없는 DB에서 unloaddb를 실행할 때 그 외의 schema(user, serial, sp, server, synonym, grant)가 추출될 수 있도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

트리거 생성 및 스키마가 지원되지 않는 버전의 unload 파일을 loaddb 실행 시, db_trigger 카탈로그의 condition, action_definition 컬럼에 [user_schema]가 잘못 저장되거나 누락되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

서로 다른 계정에서 동일한 이름의 serial이 있을 때 unloaddb/loaddb에서 발생하는 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

DBA 사용자로 serial 및 trigger를 unloaddb 시 schema명이 누락되지 않도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

시스템 테이블을 synonym으로 사용하는 경우, unloaddb 후 loaddb 실행시 발생하는 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

DBA 멤버가 –as-dba 옵션 없이 unloaddb를 실행할 때, 다른 사용자가 부여한 PROCEDURE 권한(GRANT … ON PROCEDURE)이 출력되는 문제 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

일반 사용자가 트리거를 unload 수행 시, condition과action_definition에서 소유자가 본인인 경우 [user_schema] 제거하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Backupdb 수행 시 만들어진 보관 로그 볼륨이 삭제되지 않는 현상 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

unloaddb에서 잘못된 serial 값이 발생하는 현상 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

HA
~~

Master node, Slave node, Replica node로 구성된 HA 환경에서 ha_replica_delay를 60초 이상으로 설정할 경우, Replica node에서 slave로부터 복제해온 보관 로그가 삭제되지 않는 현상 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

cub_commdb utility의 -D 옵션 usage message 변경
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

restoreslave 수행 시 master 노드와의 데이터 불일치 발생 가능한 상황 방지하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

applylogdb 프로세스가 DB에 반영하면서 생성된 sql log의 id가 UINT_MAX를 초과하여 0부터 다시 시작하는 경우를 고려하지 않아 sql log 파일이 자동으로 삭제되지 않는 이슈 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Broker, CAS, CMS
~~~~~~~~~~~~~~~~

Java 개발환경에서 addBatch()와 executeBatch() 사용 시 발생할 수 있는 메모리 누수(memory leak) 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

잘못된 ACL 구성에 대한 브로커 오류 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CAS의 DDL Audit 로그 파일 내용에 ’DB Name’을 추가하여 여러 개 DB를 운영하는 환경에서 어느 DB에 처리한 것인지 확인 가능하게 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

커밋/롤백 없이 트랜잭션이 종료되어도 ABORT 로그를 ddl_audit 로그에 기록하도록 수정
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SetAutocommit(false)에서 여러 DDL문을 한 번에 실행할 때 명령문 사이에 ‘commit’ 또는 ‘rollback’ 명령문이 있는 경우 개선 - commit 및 rollback도 ddl_audit.log에 작성
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

