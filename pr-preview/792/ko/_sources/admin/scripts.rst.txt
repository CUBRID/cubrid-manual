
:meta-keywords: scripts
:meta-description: Various CUBRID scripts support CUBRID Managements

***************
cubrid 스크립트
***************

.. _cubrid_scripts_unloaddb_sh:

unloaddb.sh 스크립트
====================

**unloaddb** 의 실행 시간을 단축하기 위해서 서로 다른 테이블을 언로드 하기 위한 **cubrid unloaddb** 명령어를 동시에 여러 개 실행할 수 있다. 
**unloaddb.sh** 은 이러한 과정을 자동화해주는 shell script이다.

다수의 자식 process에게 균등하게 unloaddb의 부하를 배분하여 실행시키기 위해서 unloadb.sh은 catalog에서 언로드 대상 테이블들의 크기 정보를 가져온다 (DBA 권한 필요). 

테이블 크기를 기준으로 각 자식 프로세스들에게 언로드할 테이블들이 할당되며, 각 자식 프로세스들은 할당된 테이블들의 unloaddb를 동시에 수행한다.

.. warning::

	**제약사항:**
	  * Linux 환경
	  * DBA 권한이 있는 사용자만 실행 가능


다음은 **unloaddb.sh** 에 대한 [options]이다.

.. program:: sh unloaddb.sh

.. option:: -u user

    언로딩할 데이터베이스의 사용자 계정을 지정한다. 옵션을 지정하지 않으면 기본값은 DBA가 된다 (**user** 는 **DBA 권한** 이 있어야 한다). ::

        sh unloaddb.sh -u dba demodb

.. option:: -t no-process

    동시에 실행될 자식 프로세스의 개수 (no-process, 기본: 8개, 최대 16개 ) ::

        sh unloaddb.sh -t 4 demodb

.. option:: -i input-class-file

    입력 파일에서 지정된 테이블들만 언로드 한다. (기본: 지정된 데이터베이스 전체) ::

        sh unloaddb.sh -i table_list.txt demodb

    다음은 입력 파일 table_list.txt의 예이다. ::

        public.table_1
        public.table_2
        ..
        public.table_n

.. option:: -D output-directory

    스키마와 객체 파일이 생성될 디렉터리를 지정한다. 옵션이 지정되지 않으면 현재 디렉터리에 생성된다. ::

        sh unloaddb.sh -D /tmp demodb

.. option:: -s

    언로드 작업을 통해 생성되는 출력 파일 중 스키마 파일만 생성되도록 지정하는 옵션이다 (**schema only**). ::

        sh unloaddb.sh -s demodb

.. option:: -d

    언로드 작업을 통해 생성되는 출력 파일 중 데이터 파일만 생성되도록 지정하는 옵션이다 (**data only**). ::

        sh unloaddb.sh -d demodb

.. option:: -v

    언로드 작업이 진행되는 동안 데이터베이스의 테이블 크기 등의 부가 정보를 화면에 출력하는 옵션이다 (**verbose mode**). ::

        sh unloaddb.sh -v demodb


**unloadbdb.sh에 의해 생성되는 파일**

* 언로드 대상 테이블 전체에 대한 schema 파일
* 자식 프로세스 숫자 만큼의 object 파일
* 로그 파일: 디렉터리 {데이터베이스 이름}_unloaddb.log 가 생성되며, 여기에 로그 파일들이 생성된다.

  unloaddb 실행 예 ::

	sh unloaddb.sh -t 4 demodb

  파일 생성 예 ::

     * demodb_schema: 언로드된 테이블의 스키마 파일
     * demodb_0_objects: 1번째 자식 프로세스가 언로드한 오브젝프 파일
     * demodb_1_objects: 2번째 자식 프로세스가 언로드한 오브젝프 파일
     * demodb_2_objects: 3번째 자식 프로세스가 언로드한 오브젝프 파일
     * demodb_3_objects: 4번째 자식 프로세스가 언로드한 오브젝프 파일
     * demodb_unloaddb.log/demodb_0.files: 첫번째 자식 프로세스에 의해서 언로드된 테이블 이름
     * demodb_unloaddb.log/demodb_0.pid: 첫번째 자식 프로세스의 프로세스 번호
     * demodb_unloaddb.log/demodb_0_unloaddb.log: 첫번째 자식 프로세스가 실행한 'cubrid unloaddb'의 log
     * demodb_unloaddb.log/demodb_0.status: 첫번째 자식 프로세스의 실행 결과 성공 여부

.. note::

   * unloaddb.sh의 실행이 완전히 종료되기 이전에 터미널에서 인터럽트 키 (**CTRL-C**)를 입력하면, 언로드가 진행중인 unloaddb object 파일들은 삭제된다 (언도드가 완료된 object 파일들은 삭제되지 않는다).
