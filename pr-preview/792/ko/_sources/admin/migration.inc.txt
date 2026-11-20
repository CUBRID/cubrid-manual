.. _unloaddb:

unloaddb
--------

데이터베이스를 언로드/로드하는 목적은 다음과 같다.

*   데이터베이스 볼륨을 재구성하여 데이터베이스 재구축
*   시스템이 다른 환경에서 마이그레이션 수행
*   버전이 다른 DBMS에서 마이그레이션 수행

::

    cubrid unloaddb [options] database_name

**cubrid unloaddb**\ 가 생성하는 파일은 다음과 같다.

*   스키마 파일(*database-name*\ **_schema**): 해당 데이터베이스에 정의된 스키마 정보를 포함하는 파일이다.
*   객체 파일(*database-name*\ **_objects**): 해당 데이터베이스에 포함된 인스턴스 정보를 포함하는 파일이다.
*   인덱스 파일(*database-name*\ **_indexes**): 해당 데이터베이스에 정의된 인덱스 정보를 포함하는 파일이다.
*   트리거 파일(*database-name*\ **_trigger**): 해당 데이터베이스에 정의된 트리거 정보를 포함하는 파일이다. 만약 데이터를 로딩하는 동안 트리거가 구동되는 것을 원치 않는다면, 데이터 로딩을 완료한 후에 트리거 정의를 로딩하면 된다.
*   분할된 스키마 파일(*database-name*\ **_schema**\ **_objecttype**): **\-\-split-schema-files** 옵션을 사용할 경우 해당 데이터베이스에 정의된 스키마 정보를 오브젝트 단위로 분할하여 해당 오브젝트 정보만 가지고 있는 파일들이다. 분할된 파일 목록은 **\-\-split-schema-files** 옵션을 참고 한다.
*   스키마 목록 파일(*database-name*\ **_schema**\ **_info**): **\-\-split-schema-files** 옵션을 사용할 경우 분할된 스키마 정보 파일들의 목록을 가지고 있는 파일이다.


이러한 스키마, 객체, 인덱스, 트리거 파일은 같은 디렉터리에 생성된다.

다음은 **cubrid unloaddb** 에서 사용하는 [options]이다.

.. program:: unloaddb

::

  -u, --user=ID               데이터베이스 접근에 관한 사용자 이름
  -p, --password=PASS         사용자 패스워드
  -i, --input-class-file=FILE 테이블 이름이 기입된 파일
      --include-reference     참조되는 테이블 포함
      --input-class-only      명세한 클래스만 처리
      --estimated-size=NUMBER 인스턴스의 대략적인 개수
      --cached-pages=NUMBER   cached 페이지 개수
  -O, --output-path=PATH      출력 디렉터리 경로
  -s, --schema-only           스키마만 처리
  -d, --data-only             오브젝트만 처리
      --output-prefix=PREFIEX 출력 파일의 prefix
      --hash-file=FILE        해쉬 파일이름
      --latest-image          오브젝트 (객체)를 언로드할 때, 데이터 볼륨의 최신 이미지를 언로드합니다. 
                              커밋되지않은 데이터와 제거되었지만 배큠되지 않은 데이터가 포함될 수 있습니다
  -t, --thread-count=COUNT    사용할 쓰레드 개수를 지정; 기본값: 1 최대값: 127
      --enhanced-estimates    언로드할 테이블의 정확한 레코드 건수 수집; 기본값 : 미적용
  -v, --verbose               많은 상태 메시지를 출력
      --use-delimiter         식별자 처음과 끝에 '"' 사용
  -S, --SA-mode               독립 모드 실행
  -C, --CS-mode               클라이언트 서버 모드 실행
      --datafile-per-class    각 클래스별 오브젝트 파일 생성
      --split-schema-files    스키마 정보를 오브젝트별로 분리하여 각각의 파일로 생성
      --skip-index-detail     인덱스를 생성할 때 지정된 WITH 절 옵션을 인쇄하지 않음
      --as-dba                로그인 사용자가 DBA 그룹의 구성원인 경우 DBA와 동일한 스키마 파일을 추출합니다.


.. option:: -u, --user=ID

    언로딩할 데이터베이스의 사용자 계정을 지정한다. 옵션을 지정하지 않으면 기본값은 **DBA**\가 된다.

    ::

        cubrid unloaddb -u dba -i table_list.txt demodb

.. option:: -p, --password=PASS

    언로딩할 데이터베이스의 사용자 암호를 지정한다. 옵션을 지정하지 않으면 빈 문자열을 입력한 것으로 간주한다.

    ::

        cubrid unloaddb -u dba -p dba_pwd -i table_list.txt demodb

.. option:: -i, --input-class-file=FILE

    모든 테이블의 스키마와 인덱스를 언로드하되, 파일에 명시된 테이블의 데이터만 언로드한다. 언로딩할 사용자의 스키마와(-u 옵션 참고) 테이블의 스키마가 다른 경우 테이블 이름은 스키마 이름을 접두어로 사용해야 한다.
    
    ::

        cubrid unloaddb -i table_list.txt demodb

    다음은 입력 파일 table_list.txt의 예이다. ::

        table_1
        table_2
        ..
        table_n

    **-i** 옵션이 **\-\-input-class-only** 와 결합되면, **-i** 옵션의 입력 파일에서 지정된 테이블의 스키마, 인덱스, 데이터 파일만 언로드한다.
    
    ::

        cubrid unloaddb --input-class-only -i table_list.txt demodb

    **-i** 옵션이 **\-\-include-reference** 와 결합되면, 참조되는 테이블도 함께 언로드된다.
    
    ::
    
        cubrid unloaddb --include-reference -i table_list.txt demodb

.. option:: --include-reference

    **-i** 옵션과 함께 사용되며, 참조되는 테이블도 함께 언로드된다. 

.. option:: --input-class-only

    **-i** 옵션과 함께 사용되며, **-i** 옵션의 입력 파일에서 지정된 테이블의 스키마 파일만 생성한다.

.. option:: --estimated-size=NUMBER

    언로드할 데이터베이스의 레코드 저장을 위한 해시 메모리를 사용자 임의로 할당하기 위한 옵션이다. 만약 **\-\-estimated-size** 옵션이 지정되지 않으면 최근의 통계 정보를 기반으로 데이터베이스의 레코드 수를 결정하게 되는데, 만약 최근 통계 정보가 갱신되지 않았거나 해시 메모리를 크게 할당하고 싶은 경우 이 옵션을 이용할 수 있다. 따라서, 옵션의 인수로 너무 적은 레코드 개수를 정의한다면 해시 충돌로 인해 언로드 성능이 저하된다.
    
    ::

        cubrid unloaddb --estimated-size=1000 demodb
        
.. option:: --cached-pages=NUMBER

    메모리에 캐시되는 테이블의 페이지 수를 지정하기 위한 옵션이다. 각 페이지는 4,096 바이트이며, 관리자는 메모리의 크기와 속도를 고려하여 캐시되는 페이지 수를 지정할 수 있다. 만약, 이 옵션이 지정되지 않으면 기본값은 100페이지가 된다.
    
    ::

        cubrid unloaddb --cached-pages 500 demodb
    
.. option:: -O, --output-path=PATH

    스키마와 객체 파일이 생성될 디렉터리를 지정한다. 옵션이 지정되지 않으면 현재 디렉터리에 생성된다.
    
    ::

        cubrid unloaddb -O ./CUBRID/Databases/demodb demodb

    지정된 디렉터리가 존재하지 않는 경우 다음과 같은 에러 메시지가 출력된다.
    
    ::

        unloaddb: No such file or directory.

.. option:: -s, --schema-only

    언로드 작업을 통해 생성되는 출력 파일 중 스키마 파일만 생성되도록 지정하는 옵션이다.
    
    ::

        cubrid unloaddb -s demodb

.. option:: -d, --data-only

    언로드 작업을 통해 생성되는 출력 파일 중 데이터 파일만 생성되도록 지정하는 옵션이다.
    
    ::

        cubrid unloaddb -d demodb

.. option:: --output-prefix=PREFIX

    언로드 작업에 의해 생성되는 스키마 파일과 객체 파일의 이름 앞에 붙는 prefix를 지정하기 위한 옵션이다. 예제를 수행하면 스키마 파일명은 *abcd_schema* 가 되고, 객체 파일명은 *abcd_objects* 가 된다. 만약, **\-\-output-prefix** 옵션을 지정하지 않으면 언로드할 데이터베이스 이름이 prefix로 사용된다.
    
    ::

        cubrid unloaddb --output-prefix abcd demodb

.. option:: --hash-file=FILE

    해시 파일의 이름을 지정한다.
    
.. option:: --latest-image

    인스턴스들을 언로드할 때 현재 데이터 볼륨의 가장 마지막 이미지에서 언로드한다. 커밋되지 않은 데이터나 삭제되었지만 배큠되지 않은 데이터가 포함될 수 있다. 이 옵션을 지정하면 MVCC 버전은 무시되며 로그 볼륨을 참조하지 않는다.

.. option:: -t, --thread-count=COUNT

    사용할 쓰레드 개수를 지정하는 옵션으로, COUNT 개수 만큼의 쓰레드를 이용해서 병렬로 수행하도록 하며 COUNT는 0 이상, 127 이하의 범위여야 한다.
    설정을 생략하면 1로 지정한 것과 같고, 만약 0으로 지정되면 쓰레드 방식으로 동작하지 않는다.
    쓰레드 방식을 지정하더라도 테이블에 Object 타입 또는, Object 타입을 내재하는 Set, MultiSet, Sequence(List) 타입이 있는 경우에는 해당 테이블에 대해서는 쓰레드 방식으로 동작하지 않는다.
    
    ::

        cubrid unloaddb -t 4 demodb

.. option:: --enhanced-estimates

    언로드할 테이블의 정확한 레코드 개수를 수집하는 옵션으로, -v 옵션을 함께 지정해서 사용해야 하며, -v 옵션을 사용하지 않는 경우 무시된다.
    작업 대상 테이블의 레코드 수를 파악 할 때 통계 정보 대신 실제 값을 구한다. 이 옵션이 지정되면 레코드 수를 파악하기 위한 수행 시간이 추가되므로 언로드 수행시간이 길어질 수 있으므로 유의해서 사용해야 한다.
    
    ::

        cubrid unloaddb --enhanced-estimates demodb

.. option:: -v, --verbose

    언로드 작업이 진행되는 동안 언로드되는 데이터베이스의 테이블 및 인스턴스에 관한 상세 정보를 화면에 출력하는 옵션이다.
    
    ::

        cubrid unloaddb -v demodb

.. option:: --use-delimiter

    식별자의 시작과 끝에 겹따옴표(")를 기록한다. 기본 설정은 식별자의 시작과 끝에 겹따옴표를 기록하지 않는다.
        
.. option:: -S, --SA-mode

    독립 모드에서 데이터베이스를 언로드한다.
    
    ::
    
        cubrid unloaddb -S demodb

.. option:: -C, --CS-mode

    클라이언트/서버 모드에서 데이터베이스를 언로드한다.
    
    ::

        cubrid unloaddb -C demodb

.. option:: --datafile-per-class

    언로드 작업으로 생성되는 데이터 파일을 각 테이블별로 생성되도록 지정하는 옵션이다. 파일 이름은 *<데이터베이스 이름>_<테이블 이름>*\ **_objects** 로 생성된다. 단, 객체 타입의 칼럼 값은 모두 **NULL** 로 언로드되며, 언로드된 파일에는 %id class_name class_id 부분이 작성되지 않는다. 자세한 내용은 :ref:`howtowrite-loadfile` 을 참고한다.
    
    ::

        cubrid unloaddb --datafile-per-class demodb

.. option:: --split-schema-files

    스키마 정보를 오브젝트 단위로 분할 생성하고, 분할된 파일 목록을 기록한 파일도 생성한다. 자세한 사용방법은 :ref:`loaddb` 의 --schema-file-list=FILE 를 참조한다.
    이 옵션을 지정하지 않으면 모든 오브젝트가 포함된 하나의 스키마 파일이 생성된다.
    ::

        cubrid unloaddb --split-schema-files demodb

    스키마 정보를 오브젝트 단위로 분할하여 생성한 파일 목록 ::

        demodb_schema_user
        demodb_schema_class
        demodb_schema_vclass
        demodb_schema_synonym
        demodb_schema_serial
        demodb_schema_procedure
        demodb_schema_server
        demodb_schema_pk
        demodb_schema_fk
        demodb_schema_uk
        demodb_schema_grant
        demodb_schema_vclass_query_spec

    분할된 스키마 파일들의 목록을 가지고 있는 파일명
    
    ::

        demodb_schema_info

.. option:: --as-dba

	사용자가 DBA 그룹의 구성원인 경우, DBA 사용자인 경우와 동일한 내용으로 스키마 파일을 생성한다.
    
    ::

        cubrid unloaddb -u u1_dba_group --as-dba demodb

.. option:: --skip-index-detail

	이 옵션은 인덱스 생성시 지정된 WITH 절 옵션들을 출력하지 않는다. unloaddb는 **print_index_detail** 설정값에 상관없이 항상 WITH 절의 옵션들을 출력하는 것이 기본적인 동작이다. 
        이 옵션이 지정되면 WITH 절의 옵션들은 생략하고 출력하게 된다. 영향을 받는 출력 파일은 스키마파일(*database-name*\_schema)과 인덱스파일(*database-name*\_indexes)이다. 
        이 옵션이 생략되면 *deduplicate level*\이 **0**\이더라도 반드시 명시적으로 출력된다.
    
    ::

        cubrid unloaddb --skip-index-detail demodb

    아래 예시에서 이 옵션의 지정 여부에 따른 차이를 확인해 볼 수 있다.
    
    스키마파일
    
    +----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
    | -–skip-index-detail 미지정 | ALTER CLASS [dba].[ts] ADD CONSTRAINT [fk_ts_fid] FOREIGN KEY([fid]) WITH DEDUPLICATE=0 REFERENCES [dba].[tm] ON DELETE RESTRICT ON UPDATE RESTRICT ;  |
    +----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
    | -–skip-index-detail 지정   | ALTER CLASS [dba].[ts] ADD CONSTRAINT [fk_ts_fid] FOREIGN KEY([fid]) REFERENCES [dba].[tm] ON DELETE RESTRICT ON UPDATE RESTRICT ;                     |
    +----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
    
    인덱스파일
    
    +----------------------------+-------------------------------------------------------------------+
    | -–skip-index-detail 미지정 | CREATE INDEX [idx_val] ON [dba].[ts] ([val]) WITH DEDUPLICATE=0;  |
    +----------------------------+-------------------------------------------------------------------+
    | -–skip-index-detail 지정   | CREATE INDEX [idx_val] ON [dba].[ts] ([val]);                     |
    +----------------------------+-------------------------------------------------------------------+


.. note::

   언로드 시간을 단축하기 위해서 서로 다른 테이블에 대한 언로드는 동시에 실행 가능하며 **unloaddb.sh** 스크립트를 이용하면 이러한 과정을 쉽게 할 수 있다 (:ref:`cubrid_scripts_unloaddb_sh` 참고).

.. _loaddb:

loaddb
------

데이터베이스 로드는 다음과 같은 경우에 **cubrid loaddb** 유틸리티를 이용하여 수행된다.

*   예전 버전의 CUBRID 데이터베이스를 새로운 버전의 데이터베이스로 마이그레이션하는 경우
*   타 DBMS의 데이터베이스를 CUBRID 데이터베이스로 마이그레이션하는 경우
*   **INSERT** 구문 실행보다 빠른 성능으로 대용량 데이터를 입력하는 경우

일반적으로 **cubrid loaddb** 유틸리티는 **cubrid unloaddb** 유틸리티가 생성한 파일(스키마 정의 파일, 객체 입력 파일, 인덱스 정의 파일)을 사용한다.

::

    cubrid loaddb [options] database_name

**입력 파일**

*   스키마 파일(*database-name*\ **_schema**): 언로드 작업에 의해 생성된 파일로서, 데이터베이스에 정의된 스키마 정보를 포함하는 파일이다.
*   객체 파일(*database-name*\ **_objects**): 언로드 작업에 의해 생성된 파일로서, 데이터베이스에 포함된 레코드 정보를 포함하는 파일이다.
*   인덱스 파일(*database-name*\ **_indexes**): 언로드 작업에 의해 생성된 파일로서, 데이터베이스에 정의된 인덱스 정보를 포함하는 파일이다.
*   트리거 파일(*database-name*\ **_trigger**): 언로드 작업에 의해 생성된 파일로서, 데이터베이스에 정의된 트리거 정보를 포함하는 파일이다.
*   사용자 정의 객체 파일(*user_defined_object_file*) : 대용량 데이터 입력을 위해 사용자가 테이블 형식으로 작성한 입력 파일이다(:ref:`howtowrite-loadfile` 참고).
*   스키마 목록 파일(*database-name*\ **_schema_info**): **\-\-split-schema-files** 옵션과 함께 언로드 작업에 의해 생성된 파일로서, 데이터베이스에 정의된 스키마 정보를 오브첵트 단위로 분할된 오브젝트 파일들의 목록을 가지고 있는 파일이다.(:option:`unloaddb --split-schema-files` 참고)

**입력 파일 처리 순서**

**cubrid loaddb** 유틸리티는 아래와 같이 특정 순서 따라서 파싱을 하며 입력 파일을 처리한다. 이것은 loaddb의 정확성과 일관성을 보장한다.

1. 스키마 파일 로드 (기본키 포함)
2. 오브젝트 파일 로드
3. 인덱스 파일 로드 (보조키와 외래키)
4. 트리거 파일 로드

**로딩 모드**

**cubrid loaddb** 유틸리티는 데이터를 데이터베이스에 로드하기 위해서 두가지 모드를 제공한다.

*   **Stand-alone** 모드에서는 단일 스레드 환경에서 오프라인으로 데이터를 로드한다. CUBRID 10.1과 그 이하의 버전에서는 이 모드 만이 제공된다.
*   **Client-server** 모드에서는 데이터베이스 서버에 접속하여 온라인으로 데이터를 로드하며 병렬로 처리된다. **Client-server** 모드는 **stand-alone** 모드보다 훨씬 빠르지만 오브젝트의 참조와 클래스/공유 속성은 지원하지 않는다.

다음은 **cubrid loaddb** 에서 사용하는 [options]이다. 

.. program:: loaddb

::

  -u, --user=ID                  데이터베이스 접근에 관한 사용자 이름
  -p, --password=PASS            사용자 패스워드
  -S, --SA-mode                  독립 모드 실행
  -C, --CS-mode                  클라이언트 서버 모드 실행
      --data-file-check-only     데이터 파일을 로딩하지 않고 신택스만 검사
  -l, --load-only                신택스 체크없이 데이터 파일 로딩
      --estimated-size=NUMBER    인스턴스의 대략적인 개수
  -v, --verbose                  많은 상태 정보를 출력
  -c, --periodic-commit=COUNT    주기적인 커밋에 대한 카운트
      --no-oid                   OID 사용 안 함
      --no-statistics            로딩 후에 통계 정보 갱신하지 않음
  -s, --schema-file=FILE[:LINE]  파일[:라인] 정보로 스키마 생성
  -i, --index-file=FILE[:LINE]   파일[:라인] 정보로 인덱스 생성
  -d, --data-file=FILE           적재할 데이터 파일
  -t, --table=TABLE              데이터를 적재할 테이블 이름
      --error-control-file=FILE  적재 시 발생하는 에러에 대한 제어 파일
      --ignore-class-file=FILE   적재하지 않을 클래스 이름이 있는 파일
      --trigger-file=FILE[:LINE] 파일[:라인] 정보로 트리거 생성
      --no-user-specified-name   소유자 이름 없이 객체 이름으로 CLASS, SERIAL 및 TRIGGER를 찾음.
      --schema-file-list=FILE    이 파일의 내용은 loaddb에서 사용할 스키마 파일 이름 목록이다.


.. option:: -u, --user=ID

    레코드를 로딩할 데이터베이스의 사용자 계정을 지정한다. 옵션을 지정하지 않으면 기본값은 **PUBLIC** 이 된다. ::

        cubrid loaddb -u admin -d demodb_objects newdb

.. option:: -p, --password=PASS

    레코드를 로딩할 데이터베이스의 사용자 암호를 지정한다. 옵션을 지정하지 않으면 암호 입력을 요청하는 프롬프트가 출력된다. ::

        cubrid loaddb -u dba -p pass -d demodb_objects newdb
        
.. option:: -S, --SA-MODE

    데이터를 **stand-alone** 모드로 데이터베이스에 로드한다. 이것은 **loaddb** 의 기본 로드 모드이며 CUBRID 10.1 이하의 버전과 완벽히 호환된다. ::

         cubrid loaddb -S -u dba -d demodb_objects newdb

.. option:: -C, --CS-MODE

    이 옵션에서는 **loaddb** 가 가동중인 데이터베이스 서버 프로세스에 접속한다. **stand-alone** 모드와 다르게 **client-server** 모드에서는 여러 개의 스레드를 사용하여 데이터를 로드할 수있다. 이 모드에서는 동시에 여러 개의 **loaddb** 세션을 연결하며, 대용량의 데이터를 로드할 때 특히 우수한 성능을 발휘한다. 이 모드에서는 오브젝트 참조나 클래스/공유 속성은 제공하지 않는다. ::

         cubrid loaddb -C -u dba -d demodb_objects newdb

.. note::

    **HA** (High Availability)환경의 **client-server** 모드(-C, \-\-CS-MODE)에서는 -d 옵션(data loading)만 사용 가능하다.

.. option:: --data-file-check-only

     demodb_objects에 포함된 데이터의 구문이 정상인지 확인만 하며 데이터를 데이터베이스에 로딩하지 않는다. ::

        cubrid loaddb --data-file-check-only -d demodb_objects newdb
        
.. option:: -l, --load-only

    **Stand-alone** 모드는 기본적으로 데이터베이스에 로드하기 전에 전체 데이터에 대해 사전 구문 검사가 실행된다. 오류가 발생하면 데이터의 로드는 실행되지 않고 오류가 보고된다.

    **-l** 옵션을 이용하면 로드 전에 전체 데이타에 대해 사전 구문 검사 실행이 생략되어 속도가 빠르다. 그러나, 각 레코드를 데이터베이스에 로드하기 전에 구문 검사가 진행된다. 이 과정에서 구문 오류가 발생하는 경우 로드는 중지되며, 결과적으로 데이터의 일부분만 로드되는 경우가 발생할 수도 있다. ::

        cubrid loaddb -l -d demodb_objects newdb

.. option:: --estimated-size=NUMBER

    언로드할 레코드의 수가 기본값인 5,000개보다 많은 경우 로딩 성능 향상을 위해 사용할 수 있다. 이 옵션을 통해 레코드 저장을 위한 해시 메모리를 크게 할당함으로써 로드 성능을 향상시킬 수 있다. ::

        cubrid loaddb --estimated-size 8000 -d demodb_objects newdb

.. option:: -v, --verbose

    데이터베이스 로딩 작업이 진행되는 동안, 로딩되는 데이터베이스의 테이블 및 레코드에 관한 상세 정보를 화면에 출력한다. 진행 단계, 로딩되는 클래스, 입력된 레코드의 개수와 같은 상세 정보를 확인할 수 있다. ::

        cubrid loaddb -v -d demodb_objects newdb

.. option:: -c, --periodic-commit=COUNT

    지정된 개수의 레코드가 데이터베이스에 입력될 때마다 주기적으로 커밋을 실행한다. **-c** 옵션이 지정되지 않은 경우, 데이터 파일에 포함된 모든 레코드가 데이터베이스로 로드된 후에 트랜잭션이 커밋된다. 또한, **-c** 옵션이 **-s** 옵션이나 **-i** 옵션과 함께 사용하는 경우에는 100개의 DDL문이 로드될 때마다 주기적으로 커밋을 실행한다.

    권장되는 커밋 주기는 로딩되는 데이터에 따라 다른데, 스키마 로딩의 경우에는 **-c**\ 의 인수를 50으로, 인덱스 로딩의 경우는 인수를 1로 설정하는 것이 권고된다. 레코드 로딩의 경우는 10,240이 권고되며 기본값이다.

    CUBRID 10.1 또는 그 이하의 버전에서 **\-\-periodic-commit** 의 기본값은 없으며 따라서 전체 데이터를 로드한 후에 트랜잭션 커밋을 수행할 것이다. 현재 상태에서, 이전과 같이 동작하기를 원한다면 **\-\-periodic-commit** 을 매우 큰 값으로 설정하라. 그러면 모든 데이터가 로드된 후에 커밋이 실행될 것이다. ::

        cubrid loaddb -c 100 -d demodb_objects newdb

.. option:: --no-oid

    demodb_objects에 포함된 OID를 무시하고 레코드를 newdb로 로딩하는 명령이다. ::

        cubrid loaddb --no-oid -d demodb_objects newdb

.. option:: --no-statistics

    demodb_objects를 로딩한 후 newdb의 통계 정보를 갱신하지 않는 명령이다. 특히, 대상 데이터베이스의 데이터 용량에 비해 매우 적은 데이터만 로딩할 경우 이 옵션을 이용하여 로드 성능을 향상시킬 수 있다. ::

        cubrid loaddb --no-statistics -d demodb_objects newdb

.. option:: -s, --schema-file=FILE[:LINE]

    스키마 파일의 LINE번째부터 정의된 스키마 정보를 새로 생성한 newdb에 로딩하는 구문이다. **-s** 옵션을 이용하여 스키마 정보를 먼저 로딩한 후, 실제 레코드를 로딩할 수 있다. 

    다음 예제에서 demodb_schema 파일은 언로드 작업에 의해 생성된 파일이며 언로드된 데이터베이스의 스키마 정보를 포함한다.

    ::

        cubrid loaddb -u dba -s demodb_schema newdb

        Start schema loading.
        Total       86 statements executed.
        Schema loading from demodb_schema finished.
        Statistics for Catalog classes have been updated.

    다음은 demodb에 정의된 트리거 정보를 새로 생성한 newdb에 로딩하는 구문이다. demodb_trigger 파일은 언로드 작업에 의해 생성된 파일이며, 언로드된 데이터베이스의 트리거 정보를 포함한다. 레코드를 모두 로딩한 후, -s 옵션을 이용하여 트리거를 생성할 것을 권장한다. ::

        cubrid loaddb -u dba -s demodb_trigger newdb

.. option:: -i, --index-file=FILE[:LINE]

    인덱스 파일의 LINE번째부터 정의된 인덱스 정보를 데이터베이스에 로딩하는 명령이다. 다음 예제에서, demo_indexes 파일은 언로드 작업에 의해 생성된 파일이며 언로드된 데이터베이스의 인덱스 정보를 포함한다. **-d** 옵션을 이용하여 레코드를 로딩한 후, **-i** 옵션을 이용하여 인덱스를 생성할 수 있다. ::

        cubrid loaddb -c 100 -d demodb_objects newdb
        cubrid loaddb -u dba -i demodb_indexes newdb

.. option:: -d, --data-file=FILE

    **-d** 옵션을 이용하여 데이터 파일 또는 사용자 정의 객체 파일을 지정함으로써 레코드 정보를 newdb로 로딩하는 명령이다. demodb_objects 파일은 언로드 작업에 의해 생성된 객체 파일이거나, 사용자가 대량의 데이터 로딩을 위하여 작성한 사용자 정의 객체 파일 중 하나이다. ::
    
        cubrid loaddb -u dba -d demodb_objects newdb

.. option:: -t, --table=TABLE

    로딩할 데이터 파일에 테이블 이름 헤더가 생략되어 있는 경우, 이 옵션 뒤에 테이블 이름을 지정한다. 테이블 이름은 스키마 이름을 접두사로 사용해야 한다. ::

        cubrid loaddb -u dba -d demodb_objects -t tbl_name newdb

.. option:: --error-control-file=FILE

    데이터베이스 로드 작업 중에 발생하는 에러 중 특정 에러를 처리하는 방식에 관해 명세한 파일을 지정하는 옵션이다. ::

        cubrid loaddb --error-control-file=error_test -d demodb_objects newdb

    서버 에러 코드 이름은 **$CUBRID/include/dbi.h** 파일을 참고하도록 한다.

    에러 코드(에러 번호) 별 에러 메시지는 **$CUBRID/msg/**\ *<문자셋 이름>*\ **/cubrid.msg** 파일의 $set 5 MSGCAT_SET_ERROR 이하에 있는 번호들을 참고하도록 한다. ::

        vi $CUBRID/msg/en_US/cubrid.msg
         
        $set 5 MSGCAT_SET_ERROR
        1 Missing message for error code %1$d.
        2 Internal system failure: no more specific information is available.
        3 Out of virtual memory: unable to allocate %1$ld memory bytes.
        4 Has been interrupted.
        ...
        670 Operation would have caused one or more unique constraint violations.
        ...

    특정 에러 명세 파일의 형식은 다음과 같다.

    *   -<에러 코드> : <에러 코드>에 해당하는 에러를 무시하도록 설정 (**loaddb** 수행 중 해당 에러가 발생해도 계속 수행)

    *   +<에러 코드> : <에러 코드>에 해당하는 에러를 무시하지 않도록 설정 (**loaddb** 수행 중 해당 에러가 발생하면 작업을 종료함)

    *   +DEFAULT : 24번부터 33번까지의 에러를 무시하지 않도록 설정

    **\-\-error-control-file** 옵션으로 에러 명세 파일을 설정하지 않을 경우, **loaddb** 유틸리티는 기본적으로 24번부터 33번까지의 에러를 무시하도록 설정되어 있다. 이들은 데이터베이스 볼륨의 여유 공간이 얼마 남지 않았다는 경고성 에러로서, 이후 할당된 데이터베이스 볼륨의 여유 공간이 없어지면 자동으로 범용 볼륨(generic volume)을 생성하게 된다.

    다음은 에러 명세 파일을 작성한 예이다.

    * +DEFAULT를 설정하여, 24번부터 33번까지의 DB 볼륨 여유 공간 경고성 에러는 무시되지 않는다.

    * 앞에서 -2를 설정했으나, 뒤에서 +2를 설정했기 때문에 2번 에러 코드는 무시되지 않는다.

    * -670을 설정하여, 670번 에러인 고유성 위반 에러(unique violation error)는 무시된다.

    * #-115는 앞에 #이 있어 커멘트 처리되었다. ::

        vi error_file
         
        +DEFAULT
        -2
        -670
        #-115 --> comment
        +2

.. option:: --ignore-class-file=FILE

    로딩 작업 중 무시할 클래스 목록을 명세한 파일을 지정한다. 지정된 파일에 포함된 클래스를 제외한 나머지 클래스의 레코드만 로딩된다. ::

        cubrid loaddb --ignore-class-file=skip_class_list -d demodb_objects newdb

.. option:: --trigger-file=FILE[:LINE]

    트리거 파일의 LINE번째부터 정의된 트리거 정보를 새로 생성한 newdb에 로딩하는 구문이다. ::

        cubrid loaddb --trigger-file=demodb_triggers -d demodb_objects newdb

.. option:: --no-user-specified-name

    11.2 이전 버전에서 언로드된 파일은 **\-\-no-user-specified-name** 옵션을 사용하는 경우에만 11.2 버전에서 로드할 수 있다.

    이 옵션은 -u 옵션과 함께 데이터베이스의 사용자 계정이 **DBA**\로 지정된 경우에만 동작한다.
    
    ::

        cubrid loaddb -u dba -s demodb_schema -d demodb_objects --no-user-specified-name demodb

.. warning::

    **\-\-no-user-specified-name** 옵션은 11.2버전 또는 11.2 이후 버전에서 언로드된 파일을 로드할 때 사용하면 안 된다.


.. option:: --schema-file-list=FILE

    목록에 포함된 모든 스키마 파일들을 한번에 로딩 하는 옵션이다. 스키마 파일 목록이 있어야 하며, 이 목록 파일의 맨 위 스키마 파일부터 아래로 순차적으로 로딩된다. 이 옵션과 -s 옵션은 함께 사용할 수 없다.

    이 목록 파일은 unloaddb에서 **\-\-split-schema-files** 옵션을 사용할 때 자동으로 생성된다.
	
    ::

        cubrid loaddb -u dba --schema-file-list=demodb_schema_info

.. warning::

    이 파일에 작성된 스키마 파일의 순서를 변경할 수 있으나 순서가 잘못된 경우 로딩 오류가 발생할 수 있다. 예를 들어, CLASS 정보가 있는 파일이 로드되지 않은 상태에서 PK 파일을 먼저 로드하면 오류가 발생할 수 있다. 따라서 목록에서 CLASS 정보를 로드하기 위한 파일명이 PK를 로드하기 위한 파일보다 위에 있어야 한다.


.. warning::

    **\-\-no-logging** 옵션을 사용하면 **loaddb** 를 수행하면서 트랜잭션 로그를 저장하지 않으므로 데이터 파일을 빠르게 로드할 수 있다. 그러나 로드 도중 파일 형식이 잘못되거나 시스템이 다운되는 등의 문제가 발생했을 때 데이터를 복구할 수 없으므로 데이터베이스를 새로 구축해야 한다. 즉, 데이터를 복구할 필요가 없는 새로운 데이터베이스를 구축하는 경우를 제외하고는 사용하지 않도록 주의한다. 이 옵션을 사용하면 고유 위반 등의 오류를 검사하지 않아 기본 키에 값이 중복되는 경우 등이 발생할 수 있으므로, 사용 시 이러한 문제를 반드시 감안해야 한다.

.. warning::

    **loaddb** 실행 시 대량의 데이터 로딩에 대한 성능 보장과 로딩 데이터 간의 의존성을 최소화하기 위해 외래키 제약 조건에 대한 검사를 수행하지 않는다. 그러므로, HA 환경에서 **-C** 옵션을 이용하여 데이터를 로딩할 경우 슬레이브 또는 레플리카 노드 복제 과정에서 외래키 제약 조건 검사가 수행 되어 마스터 노드와 데이터 불일치가 발생할 수 있다.

.. _howtowrite-loadfile:

가져오기용 파일 작성 방법
^^^^^^^^^^^^^^^^^^^^^^^^^

**cubrid loaddb** 유틸리티에서 사용되는 객체 입력 파일을 직접 작성하여 사용하면 데이터베이스에 대량의 데이터를 보다 신속하게 추가할 수 있다. 객체 입력 파일은 간단한 테이블 모양의 형식으로 구성되며 주석, 명령 라인, 데이터 라인으로 이루어진 텍스트 파일이다.

주석
++++

CUBRID에서는 주석은 두 개의 연속된 하이픈(\-\-)을 이용하여 처리한다. ::

    -- This is a comment!

.. _howtowrite-loadfile-command-line:

명령 라인
+++++++++

명령 라인은 퍼센트(%) 문자로 시작하며, 명령어로는 클래스를 정의하는 **%class** 명령어와, 클래스 식별을 위해 사용하는 별칭(alias)이나 식별자(identifier)를 정의하는 **%id** 명령어가 있다.

.. _assign-id-to-class:

*   클래스에 식별자 부여

    **%id** 를 이용하여 참조 관계에 있는 클래스에 식별자를 부여할 수 있다.
    
    ::

        %id [schema_name.]class_name class_id
        schema_name:
            identifier
        class_name:
            identifier
        class_id:
            integer

    **%id** 명령어에 의해 명시된 *class_name*\은 해당 데이터베이스에 정의된 클래스 이름이다. *class_name*\은 *schema_name*\을 접두사로 사용하며, *schema_name*\은 클래스가 정의된 스키마 이름이다.
    *schema_name*\은 생략할 수 있으며, *schema_name*\을 생략하면 -u 옵션과 함께 지정된 데이터베이스의 사용자 이름이 스키마 이름으로 사용된다.

    ::

        %id [employee] 21
        %id [office] 22
        %id [project] 23
        %id [phone] 24

    *schema_name*\과 *class_name*\은 대괄호([ ])로 묶어야 한다. *schema_name*\과 *class_name*\의 각각을 대괄호로 묶거나, 함께 대괄호로 묶을 수 있다.

    ::

        %id [public].[employee] 21
        %id [public].[office] 22
        %id [public].[project] 23
        %id [public].[phone] 24

    ::

        %id [public.employee] 21
        %id [public.office] 22
        %id [public.project] 23
        %id [public.phone] 24

*   클래스 및 속성 명시

    **%class** 명령어를 이용하여 데이터가 로딩될 클래스(테이블) 및 속성(칼럼)을 명시하며, 명시된 속성의 순서에 따라 데이터 라인이 작성되어야 한다. **cubrid loaddb** 유틸리티를 실행할 때 **-t** 옵션으로 클래스 이름을 제공하는 경우에는 데이터 파일에 클래스 및 속성을 명시하지 않아도 된다. 단, 데이터가 작성되는 순서는 클래스 생성 시의 속성 순서를 따라야 한다.
    
    ::

        %class [schema_name.]class_name (attr_name [attr_name...])
        schema_name:
            identifier
        class_name:
            identifier
        attr_name:
            identifier

    데이터를 로딩하고자 하는 데이터베이스에는 클래스가 이미 정의되어 있어야 한다.

    **%class** 명령어에 의해 명시된 *class_name*\은 해당 데이터베이스에 정의된 클래스 이름이며, *schema_name*\은 클래스가 정의된 스키마 이름이다. *attr_name*\는 정의된 속성 이름을 의미한다. *schema_name*\과 *class_name*\의 작성 방법은 위와 동일하다.

    다음은 *employee*\라는 클래스에 데이터를 입력하기 위하여 **%class** 명령으로 클래스 및 3개의 속성을 명시한 예제이다. **%class** 명령 다음에 나오는 데이터 라인에서는 3개의 데이터가 입력되어야 하며, 이는 :ref:`참조 관계 설정 <conf-reference-relation>`\을 참조한다.
    
    ::

        %class [public].[employee] (name age department)

데이터 라인
+++++++++++

데이터 라인은 **%class** 명령 라인 다음에 위치하며, 입력되는 데이터는 **%class** 명령에 의해 명시된 클래스 속성과 타입이 일치해야 한다. 만약, 명시된 속성과 타입이 일치하지 않으면 데이터 로드 작업은 중지된다.

또한, 각각의 속성에 대응되는 데이터는 적어도 하나의 공백에 의해 분리되어야 하며, 한 라인에 작성되는 것이 원칙이다. 다만, 입력되는 데이터가 많은 경우에는 첫 번째 데이터 라인의 맨 마지막 데이터 다음에 플러스 기호(+)를 명시하여 다음 라인에 데이터를 연속적으로 입력할 수 있다. 이 때, 맨 마지막 데이터와 플러스 기호 사이에는 공백이 허용되지 않음을 유의한다.

*   인스턴스 입력

    다음과 같이 명시된 클래스 속성과 타입이 일치하는 인스턴스를 입력할 수 있다. 각각의 데이터는 적어도 하나의 공백에 의해 구분된다.

    ::

        %class [public].[employee] (name)
        'jordan' 
        'james'  
        'garnett'
        'malone'

*   인스턴스 번호 부여

    데이터 라인의 처음에 '번호:'의 형식으로 해당 인스턴스에 대한 번호를 부여할 수 있다. 인스턴스 번호는 명시된 클래스 내에서 유일한 양수이며, 번호와 콜론(:) 사이에는 공백이 허용되지 않는다. 이와 같이 인스턴스 번호를 부여하는 이유는 추후 객체 참조 관계를 설정하기 위함이다.

    ::

        %class [public].[employee] (name)
        1: 'jordan' 
        2: 'james'  
        3: 'garnett' 
        4: 'malone' 

.. _conf-reference-relation:
    
*   참조 관계 설정

    **@** 다음에 참조하는 클래스를 명시하고, 수직바(|) 다음에 참조하는 인스턴스의 번호를 명시하여 객체 참조 관계를 설정할 수 있다.
    
    ::

        @class_ref | instance_no
        class_ref:
             class_name
             class_id

    **@** 다음에는 클래스명 또는 클래스 id를 명시하고, 수직바(|) 다음에는 인스턴스 번호를 명시한다. 수직바(|)의 양쪽에는 공백을 허용하지 않는다. *class_ref*\의 작성 방법은 :ref:`명령 라인 <howtowrite-loadfile-command-line>`\의 *schema_name*\과 *class_name*\의 작성 방법과 동일하다. 하지만 *schema_name*\과 *class_name* 각각을 대괄호([ ])로 묶는 것은 허용하지 않는다.

    다음은 *paycheck* 클래스에 인스턴스를 입력하는 예제이며, *name* 속성은 *employee* 클래스의 인스턴스를 참조한다. 마지막 라인과 같이 앞에서 정의되지 아니한 인스턴스 번호를 이용하여 참조 관계를 설정하는 경우 해당 데이터는 **NULL** 로 입력된다.
    
    ::

        %class [public].[paycheck] (name department salary)
        @[public.employee]|1   'planning'   8000000   
        @[public.employee]|2   'planning'   6000000  
        @[public.employee]|3   'sales'   5000000  
        @[public.employee]|4   'development'   4000000
        @[public.employee]|5   'development'   5000000

    :ref:`클래스에 식별자 부여 <assign-id-to-class>`\ 에서 **%id** 명령어로 *employee* 클래스에 21이라는 식별자를 부여했으므로, 위의 예를 다음과 같이 작성할 수 있다.
    
    ::

        %class [public].[paycheck] (name department salary)
        @21|1   'planning'   8000000   
        @21|2   'planning'   6000000  
        @21|3   'sales'   5000000  
        @21|4   'development'   4000000
        @21|5   'development'   5000000

데이터베이스 마이그레이션
^^^^^^^^^^^^^^^^^^^^^^^^^

신규 버전의 CUBRID 데이터베이스를 사용하기 위해서는 기존 버전의 CUBRID 데이터베이스를 신규 버전의 CUBRID 데이터베이스로 이전하는 작업을 진행해야 할 경우가 있다. 이때 CUBRID에서 제공하는 텍스트 파일로 내보내기와 텍스트 파일에서 가져오기 기능을 활용할 수 있다.

**cubrid unloaddb** 및 **cubrid loaddb** 유틸리티를 이용하는 마이그레이션 절차를 설명한다.

**권장 시나리오 및 절차**

기존 버전의 CUBRID가 운영 중인 상태에서 적용할 수 있는 마이그레이션 시나리오를 설명한다. 데이터베이스 마이그레이션을 위해서는 **cubrid unloaddb**\ 와 **cubrid loaddb** 유틸리티를 사용한다. 자세한 내용은 :ref:`unloaddb` 및 :ref:`loaddb` 를 참조한다.

#. 기존 CUBRID 서비스 종료

   **cubrid service stop** 을 실행하여 기존 CUBRID로 운영되는 모든 서비스 프로세스를 종료한 후, CUBRID 관련 프로세스들이 모두 정상 종료되었는지 확인한다.

   CUBRID 관련 프로세스들이 모두 정상 종료되었는지 확인하려면, Linux에서는 **ps -ef|grep cub_**\ 를 실행한다. cub_로 시작하는 프로세스가 없으면 정상적으로 종료된 것이다. Windows에서는 <Ctrl + Alt + Delete> 키를 누른 후 [작업 관리자 시작]을 선택한다. [프로세스] 탭에 cub_로 시작하는 프로세스가 없으면 정상적으로 종료된 것이다. CUBRID 서비스 종료 후에도 관련 프로세스가 존재하면 Linux에서는 **kill** 명령으로 강제 종료한 후 **ipcs -m** 명령으로 CUBRID 브로커가 사용 중이던 공유 메모리를 확인하고 삭제한다. Windows에서는 작업 관리자의 [프로세스] 탭에서 해당 이미지 이름을 마우스 오른쪽 버튼으로 클릭하고 [프로세스 끝내기]를 선택하여 강제 종료한다.

#. 기존 데이터베이스 백업

   **cubrid backupdb** 유틸리티를 이용하여 기존 버전의 데이터베이스 백업을 수행한다. 그 이유는 데이터베이스 언로드/로드 작업 중 발생 가능한 장애에 대비하기 위함이다. 데이터베이스 백업에 관한 자세한 내용은 :ref:`backupdb`\를 참조한다.

#. 기존 데이터베이스 언로드

   **cubrid unloaddb** 유틸리티를 이용하여 기존 버전의 CUBRID에서 생성된 데이터베이스를 언로드한다. 데이터베이스 언로드에 관한 자세한 내용은 :ref:`unloaddb` 를 참조한다.

#. 기존 CUBRID의 환경 설정 파일 보관

   **CUBRID/conf** 디렉터리 아래의    **cubrid.conf**, **cubrid_broker.conf**, **cm.conf** 등의 환경 설정 파일을 보관한다. 이는 기존 CUBRID 데이터베이스 환경에 적용된 파라미터 설정값을 신규 CUBRID 데이터베이스 환경에서 편리하게 적용할 수 있기 때문이다.

#. 신규 버전의 CUBRID 설치

   기존 버전의 CUBRID에서 생성된 데이터의 백업 및 언로드 작업이 완료되었으므로, 기존 버전의 CUBRID 및 데이터베이스를 삭제하고 신규 버전의 CUBRID를 설치한다. CUBRID 설치에 대한 자세한 내용은 :doc:`/start`\ 을 참조한다.

#. 신규 CUBRID의 환경 설정

   **기존 CUBRID의 환경 설정 파일 보관하기** 에서 보관한 기존 데이터베이스의 환경 설정 파일을 참고하여 신규 버전의 CUBRID 환경을 설정할 수 있다. 환경 설정에 대한 자세한 내용은 "CUBRID 시작"의 :doc:`/install`\ 을 참조한다.

#. 신규 데이터베이스 로드

   **cubrid createdb** 유틸리티를 이용하여 데이터베이스를 생성하고, **cubrid loaddb** 유틸리티를 이용하여 언로드한 데이터를 해당 데이터베이스에 로드한다. 데이터베이스 생성에 대한 자세한 내용은 "관리자 안내서"의 :ref:`creating-database` 을 참조하고, 데이터베이스 로드에 대한 자세한 내용은 :ref:`loaddb`\ 를 참조한다.

#. 신규 데이터베이스 백업

   신규 데이터베이스에 데이터 로딩이 완료되면, **cubrid backupdb**    유틸리티를 이용하여 신규 버전의 CUBRID 환경에서 생성된 데이터베이스를 백업한다. 그 이유는 기존 버전의 CUBRID 환경에서 백업한 데이터를 신규 버전의 CUBRID 환경에서 복구할 수 없기 때문이다. 데이터베이스 백업에 대한 자세한 내용은 :ref:`backupdb`\를 참고한다.

.. warning:: 

    같은 버전이라 하더라도 백업 및 복구 시 32비트 데이터베이스 볼륨과 64비트 데이터베이스 볼륨 간에는 상호 호환을 보장하지 않는다. 따라서 32비트 CUBRID에서 백업한 데이터베이스를 64비트 CUBRID에서 복구하거나, 이와 반대로 작업하는 것을 권장하지 않는다.

.. warning::
 
    CUBRID 11버전에서 TDE 기능을 사용할 경우 하위호환성을 제공하지 않아, 언로드된 파일로 더 낮은 버전에서 로드할 수 없다.
