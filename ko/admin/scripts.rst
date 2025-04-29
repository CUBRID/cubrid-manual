
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


.. _restore_to_newdb_sh:


restore_to_newdb.sh 스크립트
============================


사용자 실수나 시스템 오류로 데이터 복구가 필요한 경우, “백업본”을 이용해 운영 중인 서버에 복구를 진행해야 한다.  이때 추가 계정 생성이나 엔진 설치 등 번거로운 작업이 선행되어야 하는 불편함있다.
restore_to_newdb.sh 스크립트는 별도의 추가 작업 없이, CUBRID가 설치된 기존 운영 환경에서 백업본을 사용해 신규 DB 이름으로 복구할 수 있도록 지원한다.

.. warning::

   **제약사항:**
     * Linux 환경에서만 지원된다.
     * 복구 대상 DB의 백업 파일이 존재해야 한다.
     * 기존 DB명과 복구할 DB명이 서로 달라야 한다.
     * 복구할 DB명은 $CUBRID/databases.txt에 등록되어 있으면 안된다.


기본 명령어 수행 방법이다. ::

	sh restore_to_new.sh [options] backuped-database-name new-database-name

* **restore_to_new.sh** : 백업본을 이용해 새로운 DBMS를 생성하는 스크립트이다.
* **backup-database-name** : 복구할 백업 DB의 이름이다.
* **new-database-name** : 새로 생성할 복구 DB의 이름이다.

다음은 **restore_to_new.sh** 에 대한 [options] 설명이다.

.. program:: sh restore_to_newdb.sh backuped-database-name new-database-name

.. option:: -F path

	복구할 DB의 데이터베이스 경로를 절대경로로 지정한다. 지정하지 않으면 현재 경로를 기본값으로 사용한다. ::

		sh restore_to_newdb.sh -F /home/cubrid/newdbpath backuped-database-name new-database-name

.. option:: -B path

    백업본이 위치한 경로를 절대경로로 지정한다. 지정하지 않으면 현재 경로를 기본값으로 사용한다. ::

		sh restore_to_newdb.sh -B /home/cubrid/backupdbpath backuped-database-name new-database-name

.. option:: -d date

    지정한 날짜 및 시간까지 복구를 수행한다.형식은 dd-mm-yyyy:hh:mi:ss이다. (예: 14-10-2008:14:10:00) 

    이 옵션은 cubrid restoredb의 -d 옵션과 동일하다. ::

		sh restore_to_newdb.sh backuped-database-name new-database-name -F /home/cubrid -B /home/cubrid/DB/backupdbpath -d 30-10-2025:12:20:00

.. option:: -l level

    백업 수준(0, 1, 2)을 지정하여 복구를 진행한다. cubrid restoredb의 -l 옵션과 동일하다. ::

		sh restore_to_newdb.sh -B /home/cubrid/DB/backupdbpath -l 1 backuped-database-name new-database-name

.. option:: -p

    로그 아카이브가 없는 경우, 부분 복구를 수행한다. 상세 내용은 cubrid restoredb의 -p 옵션을 참고한다. ::

		sh restore_to_newdb.sh -B /home/cubrid/DB/backupdbpath -p backuped-database-name new-database-name

.. option:: -k path

    복구 시 필요한 키 파일의 경로를 지정한다. 상세 내용은 cubrid restoredb의 -k 옵션을 참고한다. ::

		sh restore_to_newdb.sh -B /home/cubrid/DB/backupdbpath -k /home/cubdev/DB/backupdbpath/backupdb_bk1_keys backuped-database-name new-database-name


**restoredb_newdb.sh에 수행화면**

::

	sh restore_to_newdb.sh -B /home/cubrid/DB/backupdbpath backuped-database-name new-database-name

::

	Validation passed: newdb_path and vol-path are different.
	Validation passed: 'new-database-name' does not exist in /home/cubrid/CUBRID-11.4-Linux.x86_64/databases/databases.txt.

	Confirmed: Files found for pattern (/home/cubrid/DB/backupdbpath/backuped-database-name_bk0v*).
	file: /home/cubrid/DB/backupdbpath/backuped-database-name_bk0v*
	Warning: /home/cubrid/CUBRID-11.4-Linux.x86_64/databases/databases.txt already exists.
	Existing databases.txt backed up as databases.txt.bak.
	Restoring... /

	CUBRID 11.4

	Restoring... |Updated CUBRID-11.4-Linux.x86_64/databases/databases.txt
	successfully.
	database restoration completed successfully.


복구가 정상 완료되면, 복구된 DBMS를 구동한다.

::

	[cubrid ~]$ cubrid server start new-database-name
	@ cubrid server start: new-database-name

	This may take a long time depending on the amount of recovery works to do.

	CUBRID 11.4

	++ cubrid server start: success
	Calling java stored procedure is allowed

.. note::

    * 해당 스크립트를 이용한 복구 작업은 $CUBRID_DATABASES를 수정하기 때문에, 스크립트 종료 후 생성 대상 DB가 정상 등록되었는지 확인하는 것이 좋다.
    * 더 이상 사용하지 않는 복구된 DBMS는 deletedb를 통해 삭제하기를 권고한다



**restoredb_db_new.sh를 수행한 경로에 log파일 확인**


복구 작업을 수행한 경로에는 restoredb_YYYYMMDDhhmiss.log 형식의 로그 파일이 생성된다.
문제가 발생했을 경우, 이 로그 파일을 통해 복구 진행 내역을 확인할 수 있다.

::

	[cubrid ~]$ cat restoredb_20250411185128.log

	[ Database(db1) Restore (level = 0) start ]

	- restore start time: Fri Api 11 18:51:28 2025

	- restore steps: 1
	 step 1) restore using (level = 0) backup data

	- restore progress status (using level = 0 backup data)
	 -----------------------------------------------------------------------------
	 volume name                  | # of pages |  restore progress status  | done
	 -----------------------------------------------------------------------------
	 dbms_keys                     |          1 | ######################### | done
	 dbms_vinf                     |          1 | ######################### | done
	 dbms                          |      32768 | ######################### | done
	 dbms_lgar001                  |      32768 | ######################### | done
	 dbms_lgar002                  |      32768 | ######################### | done
	 dbms_lginf                    |          1 | ######################### | done
	 dbms_lgat                     |      32768 | ######################### | done
	 -----------------------------------------------------------------------------

	- restore end time: Fri Api 11 18:51:30 2025

	[ Database(db1) Restore (level = 0) end ]

