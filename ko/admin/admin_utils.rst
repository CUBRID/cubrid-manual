*****************
데이터베이스 관리
*****************

.. _cubrid-utilities:

CUBRID 관리 유틸리티
====================

CUBRID 관리 유틸리티의 사용법(구문)은 다음과 같다. ::

	cubrid utility_name
	utility_name :
		createdb [option] <database_name>   --- 데이터베이스 생성
		deletedb [option] <database_name>   --- 데이터베이스 삭제
		installdb [option] <database-name>   --- 데이터베이스 설치
		renamedb [option] <source-database-name> <target-database-name>  --- 데이터베이스 이름 변경
		copydb [option] <source-database-name> <target-database-name>  --- 데이터베이스 복사
		backupdb [option] <database-name>  --- 데이터베이스 백업
		restoredb [option] <database-name>  --- 데이터베이스 복구
		addvoldb [option] <database-name>  --- 데이터베이스 볼륨 파일 추가
		spacedb [option] <database-name>  --- 데이터베이스 공간 정보 출력
		lockdb [option] <database-name>  --- 데이터베이스의 lock 정보 출력
		killtran [option] <database-name>  --- 트랜잭션 제거
		optimizedb [option] <database-name>  --- 데이터베이스 통계 정보 갱신
		statdump [option] <database-name>  --- 데이터베이스 서버 실행 통계 정보 출력
		compactdb [option] <database-name>  --- 사용되지 않는 영역을 해제, 공간 최적화
		diagdb [option] <database-name>  --- 내부 정보 출력
		checkdb [option] <database-name>  --- 데이터베이스 일관성 검사
		alterdbhost [option] <database-name>  --- 데이터베이스 호스트 변경
		plandump [option] <database-name>  --- 쿼리 플랜 캐시 정보 출력
		loaddb [option] <database-name>  --- 데이터 및 스키마 가져오기(로드)
		unloaddb [option] <database-name>  --- 데이터 및 스키마 내보내기(언로드)
		paramdump [option] <database-name>  --- 데이터베이스의 설정된 파라미터 값 확인
		changemode [option] <database-name>  --- 서버의 HA 모드 출력 또는 변경
		copylogdb [option] <database-name>  --- HA구성을 위해 트랜잭션 로그를 다중화하는 도구
		applylogdb [option] <database-name>  --- HA 구성을 위해 트랜잭션 로그에서 복제 로그를 읽고 적용하는 도구
	  
데이터베이스 사용자
===================

CUBRID 데이터베이스 사용자는 동일한 권한을 갖는 멤버를 가질 수 있다. 사용자에게 권한 **A** 가 부여되면, 상기 사용자에게 속하는 모든 멤버에게도 권한 **A** 가 동일하게 부여된다. 이와 같이 데이터베이스 사용자와 그에 속한 멤버를 '그룹'이라 하고, 멤버가 없는 사용자를 '사용자'라 한다.

CUBRID는 **DBA** 와 **PUBLIC** 이라는 사용자를 기본으로 제공한다.

*   **DBA** 는 모든 사용자의 멤버가 되며 데이터베이스의 모든 객체에 접근할 수 있는 최고 권한 사용자이다. 또한, **DBA** 만이 데이터베이스 사용자를 추가, 편집, 삭제할 수 있는 권한을 갖는다.

*   **DBA** 를 포함한 모든 사용자는 **PUBLIC** 의 멤버가 되므로 모든 데이터베이스 사용자는 **PUBLIC** 에 부여된 권한을 가진다. 예를 들어, **PUBLIC** 사용자에 권한 **B** 를 추가하면 데이터베이스의 모든 사용자에게 일괄적으로 권한 **B** 가 부여된다.

.. _databases-txt-file:

databases.txt 파일
==================

CUBRID에 존재하는 모든 데이터베이스의 위치 정보는 **databases.txt** 파일에 저장하는데, 이를 데이터베이스 위치 정보 파일이라 한다. 이러한 데이터베이스 위치 정보 파일은 데이터베이스의 생성, 이름 변경, 삭제 및 복사에 관한 유틸리티를 수행하거나 각 데이터베이스를 구동할 때에 사용되며, 기본으로는 설치 디렉터리의 **databases** 디렉터리에 위치하고, **CUBRID_DATABASES** 환경 변수로 디렉터리 위치를 지정할 수 있다.

::

	db_name db_directory server_host logfile_directory

데이터베이스 위치 정보 파일의 라인별 형식은 구문에 정의된 바와 같으며, 데이터베이스 이름, 데이터베이스 경로, 서버 호스트 및 로그 파일의 경로에 관한 정보를 저장한다. 다음은 데이터베이스 위치 정보 파일의 내용을 확인한 예이다.

::

	% more databases.txt
	
	dist_testdb /home1/user/CUBRID/bin d85007 /home1/user/CUBRID/bin
	dist_demodb /home1/user/CUBRID/bin d85007 /home1/user/CUBRID/bin
	testdb /home1/user/CUBRID/databases/testdb d85007 /home1/user/CUBRID/databases/testdb
	demodb /home1/user/CUBRID/databases/demodb d85007 /home1/user/CUBRID/databases/demodb

데이터베이스 위치 정보 파일의 저장 디렉터리는 기본적으로 설치 디렉터리의 **databases** 디렉터리로 지정되며, 시스템 환경 변수 **CUBRID_DATABASES** 의 설정을 변경하여 기본 디렉터리를 변경할 수 있다. 데이터베이스 위치 정보 파일의 저장 디렉터리 경로가 유효해야 데이터베이스 관리를 위한 **cubrid** 유틸리티가 데이터베이스 위치 정보 파일에 접근할 수 있게 된다. 이를 위해서 사용자는 디렉터리 경로를 정확하게 입력해야 하고, 해당 디렉터리 경로에 대해 쓰기 권한을 가지는지 확인해야 한다. 다음은 **CUBRID_DATABASES** 환경 변수에 설정된 값을 확인하는 예이다.

::

	% set | grep CUBRID_DATABASES
	CUBRID_DATABASES=/home1/user/CUBRID/databases

만약 **CUBRID_DATABASES** 환경 변수에서 유효하지 않은 디렉터리 경로가 설정되는 경우에는 에러가 발생하며, 설정된 디렉터리 경로는 유효하나 데이터베이스 위치 정보 파일이 존재하지 않는 경우에는 새로운 위치 정보 파일을 생성한다. 또한, **CUBRID_DATABASES** 환경 변수가 아예 설정되지 않은 경우에는 현재 작업 디렉터리에서 위치 정보 파일을 검색한다.

.. _creating-database:

데이터베이스 생성
=================

**cubrid createdb** 유틸리티는 CUBRID 시스템에서 사용할 데이터베이스를 생성하고 미리 만들어진 CUBRID 시스템 테이블을 초기화한다. 데이터베이스에 권한이 주어진 초기 사용자를 정의할 수도 있다. 일반적으로 데이터베이스 관리자만이 **cubrid createdb** 유틸리티를 사용한다. 로그와 데이터베이스의 위치도 지정할 수 있다. ::

	cubrid createdb [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **createdb**: 새로운 데이터베이스를 생성하기 위한 명령이다.

*   *database_name*: 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않고, 생성하고자 하는 데이터베이스의 이름을 고유하게 부여한다. 이 때, 지정한 데이터베이스 이름이 이미 존재하는 데이터베이스 이름과 중복되는 경우, CUBRID는 기존 파일을 보호하기 위하여 데이터베이스 생성을 더 이상 진행하지 않는다.

데이터베이스 이름의 최대 길이는 영문 17자이다.

다음은 **cubrid createdb** 에 대한 [options]이다.

.. program:: createdb
	
.. option:: --db-volume-size=SIZE

	데이터베이스를 생성할 때 첫 번째 데이터베이스 볼륨의 크기를 지정하는 옵션으로, 기본값은 cubrid.conf에 지정된 시스템 파라미터 **db_volume_size** 의 값이다. 최소값은 20M이다. K, M, G, T로 단위를 설정할 수 있으며, 각각 KB(kilobytes), MB(megabytes), GB(gigabytes), TB(terabytes)를 의미한다. 단위를 생략하면 바이트 단위가 적용된다.

	다음은 첫 번째로 생성되는 testdb의 볼륨 크기를 512MB로 지정하는 구문이다. ::

		cubrid createdb --db-volume-size=512M testdb
	
.. option:: --db-page-size=SIZE

	데이터베이스 페이지 크기를 지정하는 옵션으로서, 최소값은 4K, 최대값은 16K(기본값)이다. K는 KB(kilobytes)를 의미한다. 데이터베이스 페이지 크기는 4K, 8K, 16K 중 하나의 값이 된다. 4K와 16K 사이의 값을 지정할 경우 지정한 값의 올림값으로 설정되며, 4K보다 작으면 4K로 설정되고 16K보다 크면 16K로 설정된다.

	다음은 testdb를 생성하고, testdb의 데이터베이스 페이지 크기를 16K로 지정하는 구문이다. ::

		cubrid createdb --db-page-size=16K testdb 

.. option:: --log-volume-size=SIZE 

	생성되는 데이터베이스의 로그 볼륨 크기를 지정하는 옵션으로, 기본값은 데이터베이스 볼륨 크기와 같으며 최소값은 20M이다. K, M, G, T로 단위를 설정할 수 있으며, 각각 KB(kilobytes), MB(megabytes), GB(gigabytes), TB(terabytes)를 의미한다. 단위를 생략하면 바이트 단위가 적용된다.

	다음은 *testdb* 를 생성하고, *testdb* 의 로그 볼륨 크기를 256M로 지정하는 구문이다. ::

		cubrid createdb --log-volume-size=256M testdb

.. option:: --log-page-size=SIZE

	생성되는 데이터베이스의 로그 볼륨 페이지 크기를 지정하는 옵션으로, 기본값은 데이터 페이지 크기와 같다. 최소값은 4K, 최대값은 16K이다. K는 KB(kilobytes)를 의미한다.
	데이터베이스 페이지 크기는 4K, 8K, 16K 중 하나의 값이 된다. 4K와 16K 사이의 값을 지정할 경우 지정한 값의 올림값으로 설정되며, 4K보다 작으면 4K로 설정되고 16K보다 크면 16K로 설정된다.

	다음은 *testdb* 를 생성하고, *testdb* 의 로그 볼륨 페이지 크기를 8kbyte로 지정하는 구문이다. ::

		cubrid createdb -log-page-size=8K testdb

.. option:: --comment=COMMENT

	데이터베이스의 볼륨 헤더에 지정된 주석을 포함하는 옵션으로, 문자열에 공백이 포함되면 큰 따옴표로 감싸주어야 한다.

	다음은 *testdb* 를 생성하고, 데이터베이스 볼륨에 이에 대한 주석을 추가하는 구문이다. ::

		cubrid createdb --comment "a new database for study" testdb

	
.. option:: -F, --file-path=PATH

	새로운 데이터베이스가 생성되는 디렉터리의 절대 경로를 지정하는 옵션으로, **-F** 옵션을 지정하지 않으면 현재 작업 디렉터리에 새로운 데이터베이스가 생성된다.

	다음은 *testdb* 라는 이름의 데이터베이스를 /dbtemp/new_db라는 디렉터리에 생성하는 구문이다. ::

		cubrid createdb -F "/dbtemp/new_db/" testdb

.. option:: -L, --log-path=PATH

	데이터베이스의 로그 파일이 생성되는 디렉터리의 절대 경로를 지정하는 옵션으로, **-L** 옵션을 지정하지 않으면 **-F** 옵션에서 지정한 디렉터리에 생성된다.
	**-F** 옵션과 **-L** 옵션을 둘 다 지정하지 않으면 데이터베이스와 로그 파일이 현재 작업 디렉터리에 생성된다.

	다음은 *testdb*	라는 이름의 데이터베이스를 /dbtemp/newdb라는 디렉터리에 생성하고, 로그 파일을 /dbtemp/db_log 디렉터리에 생성하는 구문이다. ::

		cubrid createdb -F "/dbtemp/new_db/" -L "/dbtemp/db_log/" testdb
	
.. option:: -B, --lob-base-path=PATH
	
	**BLOB** / **CLOB**	데이터를 사용하는 경우,	**LOB**	데이터 파일이 저장되는 디렉터리의 경로를 지정하는 옵션으로, 이 옵션을 지정하지 않으면 <	*데이터베이스 볼륨이 생성되는 디렉터리*	> **/lob** 디렉터리에 **LOB** 데이터 파일이 저장된다.

	다음은 *testdb*	를 현재 작업 디렉터리에 생성하고, **LOB** 데이터 파일이 저장될 디렉터리를 로컬 파일 시스템의 "/home/data1" 로 지정하는 구문이다. ::

		cubrid createdb --lob-base-path "file:/home1/data1" testdb
	
.. option:: --server-name=HOST

	CUBRID의 클라이언트/서버 버전을 사용할 때 특정 데이터베이스에 대한 서버가 지정한 호스트 상에 구동되도록 하는 옵션이다. 이 옵션으로 지정된 서버 호스트의 정보는 데이터베이스 위치 정보 파일(	**databases.txt** )에 기록된다. 이 옵션이 지정되지 않으면 기본값은 현재 로컬 호스트이다.

	다음은 *testdb* 를 *aa_host* 호스트 상에 생성 및 등록하는 구문이다. ::

		cubrid createdb --server-name aa_host testdb

.. option:: -r, --replace

	**-r** 은 지정된 데이터베이스 이름이 이미 존재하는 데이터베이스 이름과 중복되더라도 새로운 데이터베이스를 생성하고, 기존의 데이터베이스를 덮어쓰도록 하는 옵션이다.

	다음은 *testdb* 라는 이름의 데이터베이스가 이미 존재하더라도 기존의 *testdb* 를 덮어쓰기하고 새로운 *testdb* 를 생성하는 구문이다. ::

		cubrid createdb -r testdb

.. option:: --more-volume-file=FILE

	데이터베이스가 생성되는 디렉터리에 추가 볼륨을 생성하는 옵션으로 지정된 파일에 저장된 명세에 따라 추가 볼륨을 생성한다. 이 옵션을 이용하지 않더라도, 	**cubrid addvoldb**	유틸리티를 이용하여 볼륨을 추가할 수 있다.

	다음은 *testdb* 를 생성함과 동시에 vol_info.txt에 저장된 명세를 기반으로 볼륨을 추가 생성하는 구문이다. ::

		cubrid createdb --more-volume-file vol_info.txt testdb

	다음은 위 구문으로 vol_info.txt에 저장된 추가 볼륨에 관한 명세이다. 각 볼륨에 관한 명세는 라인 단위로 작성되어야 한다. ::

		#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
		# NAME volname COMMENTS volcmnts PURPOSE volpurp NPAGES volnpgs
		NAME data_v1 COMMENTS "데이터 정보 볼륨" PURPOSE data NPAGES 1000
		NAME data_v2 COMMENTS "데이터 정보 볼륨" PURPOSE data NPAGES 1000
		NAME data_v3 PURPOSE data NPAGES 1000
		NAME index_v1 COMMENTS "인덱스 정보 볼륨" PURPOSE index NPAGES 500
		NAME temp_v1 COMMENTS "임시 정보 볼륨" PURPOSE temp NPAGES 500
		NAME generic_v1 COMMENTS "일반 정보 볼륨" PURPOSE generic NPAGES 500
		#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

	예제 파일에서와 같이 각 볼륨에 관한 명세는 다음과 같이 구성된다. ::

		NAME volname COMMENTS volcmnts PURPOSE volpurp NPAGES volnpgs

	* *volname*: 추가 생성될 볼륨의 이름으로 Unix 파일 이름 규약을 따라야 하고, 디렉터리 경로를 포함하지 않는 단순한 이름이어야 한다. 볼륨명에 관한 명세는 생략할 수 있으며, 이 경우 시스템에 의해 "생성될 데이터베이스 이름_볼륨 식별자"로 볼륨명이 생성된다.

	* *volcmnts*: 볼륨 헤더에 기록되는 주석 문장으로, 추가 생성되는 볼륨에 관한 정보를 임의로 부여할 수 있다. 볼륨 주석에 관한 명세 역시 생략할 수 있다.

	* *volpurp*: 볼륨 저장의 목적으로, **data**, **index**, **temp**, **generic**	중 하나여야 한다. 볼륨 목적에 관한 명세는 생략할 수 있으며, 이 경우 기본값은 **generic**	이다.

	* *volnpgs*: 추가 생성되는 볼륨의 페이지 수이다. 볼륨 페이지 수에 관한 명세는 생략할 수 없으며, 반드시 지정해야 한다.
	
.. option:: --user-definition-file=FILE

	생성하고자 하는 데이터베이스에 대해 권한이 있는 사용자를 추가하는 옵션으로, 파라미터로 지정된 사용자 정보 파일에 저장된 명세에 따라 사용자를 추가한다.
	**--user-definition-file** 옵션을 이용하지 않더라도 :ref:`create-user` 구문을 이용하여 사용자를 추가할 수 있다.

	다음은 *testdb* 를 생성함과 동시에 user_info.txt에 정의된 사용자 정보를 기반으로 *testdb* 에 대한 사용자를 추가하는 구문이다. ::

		cubrid createdb --user-definition-file=user_info.txt testdb

	사용자 정보 파일의 구문은 아래와 같다. ::

		USER user_name [ <groups_clause> | <members_clause> ]
		
		<groups_clause>: 
			[ GROUPS <group_name> [ { <group_name> }... ] ]

		<members_clause>: 
			[ MEMBERS <member_name> [ { <member_name> }... ] ]

	*   *user_name*: 데이터베이스에 대해 권한을 가지는 사용자 이름이며, 공백이 포함되지 않아야 한다.

	*   **GROUPS** 절: 옵션이며, <group_name> 은 지정된	<user_name>을 포함하는 상위 그룹의 이름이다. 이 때, <group_name>은 하나 이상이 지정될 수 있으며, 	**USER** 로 미리 정의되어야 한다.

	*   **MEMBERS**	절: 옵션이며, <member_name> 은 지정된 <user_name>에 포함되는 하위 멤버의 이름이다. 이 때, <member_name>은 하나 이상이 지정될 수 있으며,		**USER** 로 미리 정의되어야 한다.

	사용자 정보 파일에서는 주석을 사용할 수 있으며, 주석 라인은 연속된 하이픈(--)으로 시작된다. 공백 라인은 무시된다.

	다음 예제는 그룹 *sedan* 에 *granduer* 와 *sonata* 가, 그룹 *suv* 에 *tuscan* 이, 그룹 *hatchback* 에 *i30* 가 포함되는 것을 정의하는 사용자 정보 파일이다. 사용자 정보 파일명은 user_info.txt로 예시한다. ::

		--
		--	사용자 정보 파일의 예1
		--
		USER sedan
		USER suv
		USER hatchback
		USER granduer GROUPS sedan
		USER sonata GROUPS sedan
		USER tuscan GROUPS suv
		USER i30 GROUPS hatchback

	위 예제와 동일한 사용자 관계를 정의하는 파일이다. 다만, 아래 예제에서는	**MEMBERS**	절을 이용하였다. ::

		--
		-- 사용자 정보 파일의 예2
		--
		USER granduer
		USER sonata
		USER tuscan
		USER i30
		USER sedan MEMBERS sonata granduer
		USER suv MEMBERS tuscan
		USER hatchback MEMBERS i30
		
.. option::	--csql-initialization-file=FILE

	생성하고자 하는 데이터베이스에 대해 CSQL 인터프리터에서 구문을 실행하는 옵션으로, 파라미터로 지정된 파일에 저장된 SQL 구문에 따라 스키마를 생성할 수 있다.

	다음은 *testdb* 를 생성함과 동시에 table_schema.sql에 정의된 SQL 구문을 CSQL 인터프리터에서 실행시키는 구문이다. ::

		cubrid createdb --csql-initialization-file table_schema.sql testdb

.. option:: -o, --output-file=FILE

	데이터베이스 생성에 관한 메시지를 파라미터로 지정된 파일에 저장하는 옵션이며, 파일은 데이터베이스와 동일한 디렉터리에 생성된다.
	**-o** 옵션이 지정되지 않으면 메시지는 콘솔 화면에 출력된다. **-o**	옵션은 데이터베이스가 생성되는 중에 출력되는 메시지를 지정된 파일에 저장함으로써 특정 데이터베이스의 생성 과정에 관한 정보를 활용할 수 있게 한다.

	다음은 *testdb* 를 생성하면서 이에 관한 유틸리티의 출력을 콘솔 화면이 아닌 db_output 파일에 저장하는 구문이다. ::
	
		cubrid createdb -o db_output testdb

.. option::  -v, --verbose

	데이터베이스 생성 연산에 관한 모든 정보를 화면에 출력하는 옵션으로서, **-o** 옵션과 마찬가지로 특정 데이터베이스 생성 과정에 관한 정보를 확인하는데 유용하다. 따라서, **-v** 옵션과 **-o** 옵션을 함께 지정하면, **-o** 옵션의 파라미터로 지정된 출력 파일에 **cubrid createdb** 유틸리티의 연산 정보와 생성 과정에 관한 출력 메시지를 저장할 수 있다.

	다음은 *testdb* 를 생성하면서 이에 관한 상세한 연산 정보를 화면에 출력하는 구문이다. ::

		cubrid createdb -v testdb


.. note::

	**temp_file_max_size_in_pages** 는 복잡한 질의문이나 정렬 수행에 사용되는 일시적 임시 볼륨(temporary temp volume)을 디스크에 저장하는 데에 할당되는 페이지의 최대 개수를 설정하는 파라미터이다. 
	기본값은 **-1** 로, **temp_volume_path** 파라미터가 지정한 디스크의 여유 공간까지 일시적 임시 볼륨(temporary temp volume)이 커질 수 있다. 0이면 일시적 임시 볼륨이 생성되지 않으므로 :ref:`cubrid addvoldb <adding-database-volume>` 유틸리티를 이용하여 영구적 임시 볼륨(permanent temp volume)을 충분히 추가해야 한다.
	볼륨을 효율적으로 관리하려면 용도별로 볼륨을 추가하는 것을 권장한다.
	
	:ref:`cubrid spacedb <spacedb>` 유틸리티를 사용하여 각 용도별 볼륨의 남은 공간을 검사할 수 있으며, :ref:`cubrid addvoldb <adding-database-volume>` 유틸리티를 사용하여 데이터베이스 운영 중에도 필요한 만큼 볼륨을 추가할 수 있다. 데이터베이스 운영 중에 볼륨을 추가하려면 가급적 시스템 부하가 적은 상태에서 추가할 것을 권장한다. 해당 용도의 볼륨 공간이 모두 사용되면 범용(**generic**) 볼륨이 생성되므로 여유 공간이 부족할 것으로 예상되는 용도의 볼륨을 미리 추가해 놓을 것을 권장한다.

다음은 데이터베이스를 생성하고 볼륨 용도를 구분하여 데이터(**data**), 인덱스(**index**), 임시(**temp**) 볼륨을 추가하는 예이다. ::

	cubrid createdb --db-volume-size=512M --log-volume-size=256M cubriddb
	cubrid addvoldb -p data -n cubriddb_DATA01 --db-volume-size=512M cubriddb
	cubrid addvoldb -p data -n cubriddb_DATA02 --db-volume-size=512M cubriddb
	cubrid addvoldb -p index -n cubriddb_INDEX01 cubriddb --db-volume-size=512M cubriddb
	cubrid addvoldb -p temp -n cubriddb_TEMP01 cubriddb --db-volume-size=512M cubriddb

.. _adding-database-volume:	
	
데이터베이스 볼륨 추가
======================

데이터베이스 볼륨을 추가한다. ::

	cubrid addvoldb [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **addvoldb**: 지정된 데이터베이스에 지정된 페이지 수만큼 새로운 볼륨을 추가하기 위한 명령이다.

*   *database_name*: 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않고, 볼륨을 추가하고자 하는 데이터베이스의 이름을 지정한다.

		
다음은 데이터베이스를 생성하고 볼륨 용도를 구분하여 데이터(**data**), 인덱스(**index**), 임시(**temp**) 볼륨을 추가하는 예이다. ::

	cubrid createdb --db-volume-size=512M --log-volume-size=256M cubriddb
	cubrid addvoldb -p data -n cubriddb_DATA01 --db-volume-size=512M cubriddb
	cubrid addvoldb -p data -n cubriddb_DATA02 --db-volume-size=512M cubriddb
	cubrid addvoldb -p index -n cubriddb_INDEX01 cubriddb --db-volume-size=512M cubriddb
	cubrid addvoldb -p temp -n cubriddb_TEMP01 cubriddb --db-volume-size=512M cubriddb

다음은 cubrid addvoldb에 대한 [options]이다.

.. program:: addvoldb

.. option:: --db-volume-size=SIZE

	추가되는 데이터베이스 볼륨의 크기를 지정하는 옵션으로, 기본값은 **cubrid.conf** 에 지정된 시스템 파라미터 **db_volume_size** 의 값이다. K, M, G, T로 단위를 설정할 수 있으며, 각각 KB(kilobytes), MB(megabytes), GB(gigabytes), TB(terabytes)를 의미한다. 단위를 생략하면 바이트 단위가 적용된다.

	다음은 *testdb* 에 데이터 볼륨을 추가하며 볼륨 크기를 256MB로 지정하는 구문이다. ::

		cubrid addvoldb -p data --db-volume-size=256M testdb

.. option:: -n, --volume-name=NAME

	지정된 데이터베이스에 대하여 추가될 볼륨의 이름을 지정하는 옵션이다. 볼륨명은 운영체제의 파일 이름 규약을 따라야 하고, 디렉터리 경로나 공백을 포함하지 않는 단순한 이름이어야 한다.
	
	**-n** 옵션을 생략하면 추가되는 볼륨의 이름은 시스템에 의해 "데이터베이스 이름_볼륨 식별자"로 자동 부여된다. 예를 들어, 데이터베이스 이름이
	*testdb* 이면 자동 부여된 볼륨명은 *testdb_x001* 이 된다.

	다음은 독립모드(standalone) 상태에서 *testdb* 라는 데이터베이스에 256MB 볼륨을 추가하는 구문이며, 생성되는 볼륨명은	*testdb_v1*	이 된다. ::

		cubrid addvoldb -S -n testdb_v1 --db-volume-size=256M testdb

		
.. option::  -F, --file-path=PATH

	지정된 데이터베이스에 대하여 추가될 볼륨이 저장되는 디렉터리 경로를 지정하는 옵션이다.
	**-F** 옵션을 생략하면, 시스템 파라미터인 **volume_extension_path** 의 값이 기본값으로 사용된다.

	다음은 독립모드(standalone) 상태에서 *testdb* 라는 데이터베이스에 256MB 볼륨을 추가하는 구문이며, 추가 볼륨은 /dbtemp/addvol 디렉터리에 생성된다. 볼륨명에 관한 **-n** 옵션을 지정하지 않았으므로, 생성되는 볼륨명은 *testdb_x001* 이 된다. ::

		cubrid addvoldb -S -F /dbtemp/addvol/ --db-volume-size=256M testdb

.. option:: --comment=COMMENT

	추가된 볼륨에 관한 정보 검색을 쉽게 하기 위하여 볼륨에 관한 정보를 주석으로 처리하는 옵션이다. 이때 주석의 내용은 볼륨을 추가하는
	**DBA** 의 이름이나 볼륨 추가의 목적을 포함하는 것이 바람직하며, 큰따옴표로 감싸야 한다.
	
	다음은 독립모드(standalone) 상태에서 *testdb* 라는 데이터베이스에 256MB 볼륨을 추가하는 구문이며, 해당 볼륨에 관한 정보를 주석으로 남긴다. ::

		cubrid addvoldb -S --comment "데이터 볼륨 추가_김철수" --db-volume-size=256M testdb

.. option:: -p, --purpose=PURPOSE

	추가할 볼륨의 사용 목적에 따라 볼륨의 종류를 지정하는 옵션이다. 이처럼 볼륨의 사용 목적에 맞는 볼륨을 지정해야 볼륨 종류별로 디스크 드라이브에 분리 저장할 수 있어 I/O 성능을 높일 수 있다.
	
	**-p** 옵션의 파라미터로 가능한 값은 **data**, **index**, **temp**,	**generic**	중 하나이며, 기본값은 **generic** 이다. 각 볼륨 용도에 관해서는 :ref:`database-volume-structure` 를 참조한다.

	다음은 독립모드(standalone) 상태에서 *testdb* 라는 데이터베이스에 256MB 인덱스 볼륨을 추가하는 구문이다. ::

		cubrid addvoldb -S -p index --db-volume-size=256M testdb

.. option:: -S, --SA-mode

	서버 프로세스를 구동하지 않고 데이터베이스에 접근하는 독립 모드(standalone)로 작업하기 위해 지정되며, 인수는 없다.
	**-S** 옵션을 지정하지 않으면, 시스템은 클라이언트/서버 모드로 인식한다. ::

		cubrid addvoldb -S --db-volume-size=256M testdb

.. option:: -C, --CS-mode

	서버 프로세스와 클라이언트 프로세스를 각각 구동하여 데이터베이스에 접근하는 클라이언트/서버 모드로 작업하기 위한 옵션이며, 인수는 없다. 
	**-C** 옵션을 지정하지 않더라도 시스템은 기본적으로 클라이언트/서버 모드로 인식한다. ::

		cubrid addvoldb -C --db-volume-size=256M testdb

.. option:: --max_writesize-in-sec=SIZE

	데이터베이스에 볼륨을 추가할 때 디스크 출력량을 제한하여 시스템 운영 영향을 줄이도록 하는 옵션이다. 이 옵션을 통해 1초당 쓸 수 있는 최대 크기를 지정할 수 있으며, 단위는 K(kilobytes), M(megabytes)이다. 최소값은 160K이며, 이보다 작게 값을 설정하면 160K로 바뀐다. 단, 클라이언트/서버 모드(-C)에서만 사용 가능하다.
	
	다음은 2GB 볼륨을 초당 1MB씩 쓰도록 하는 예이다. 소요 시간은 35분( = (2048MB / 1MB)  / 60초 )  정도가 예상된다. ::
	
		cubrid addvoldb -C --db-volume-size=2G --max-writesize-in-sec=1M testdb

데이터베이스 삭제
=================

**cubrid deletedb** 는 데이터베이스를 삭제하는 유틸리티이다. 데이터베이스가 몇 개의 상호 의존적 파일들로 만들어지기 때문에, 데이터베이스를 제거하기 위해 운영체제 파일 삭제 명령이 아닌 **cubrid deletedb** 유틸리티를 사용해야 한다.

**cubrid deletedb** 유틸리티는 데이터베이스 위치 파일( **databases.txt** )에 지정된 데이터베이스에 대한 정보도 같이 삭제한다. **cubrid deletedb** 유틸리티는 오프라인 상에서 즉, 아무도 데이터베이스를 사용하지 않는 상태에서 독립 모드로 사용해야 한다. ::

	cubrid deletedb [options] database_name 

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **deletedb**: 데이터베이스 및 관련 데이터, 로그, 백업 파일을 전부 삭제하기 위한 명령으로, 데이터베이스 서버가 구동 정지 상태인 경우에만 정상적으로 수행된다.

*   *database_name*: 디렉터리 경로명을 포함하지 않고, 삭제하고자 하는 데이터베이스의 이름을 지정한다

다음은 **cubrid deletedb** 에 대한 [options]이다.

.. program:: deletedb

.. option:: -o, --output-file=FILE

	데이터베이스를 삭제하면서 출력되는 메시지를 인자로 지정한 파일에 기록하는 명령이다. **cubrid deletedb** 유틸리티를 사용하면 데이터베이스 위치 정보 파일( **databases.txt** )에 기록된 데이터베이스 정보가 함께 삭제된다. ::

		cubrid deletedb -o deleted_db.out testdb

	만약, 존재하지 않는 데이터베이스를 삭제하는 명령을 입력하면 다음과 같은 메시지가 출력된다. ::

		cubrid deletedb testdb
		Database "testdb" is unknown, or the file "databases.txt" cannot be accessed.

.. option:: -d, --delete-backup

	데이터베이스를 삭제하면서 백업 볼륨 및 백업 정보 파일도 함께 삭제할 수 있다. -**d** 옵션을 지정하지 않으면 백업 볼륨 및 백업 정보 파일은 삭제되지 않는다. ::

		cubrid deletedb -d testdb

데이터베이스 이름 변경
======================

**cubrid renamedb** 유틸리티는 존재하는 데이터베이스의 현재 이름을 변경한다. 정보 볼륨, 로그 볼륨, 제어 파일들이 새로운 이름과 일치되게 이름을 변경한다.

이에 비해 **cubrid alterdbhost** 유틸리티는 지정된 데이터베이스의 호스트 이름을 설정하거나 변경한다. 즉, **databases.txt** 에 있는 호스트 이름을 변경한다. ::

	cubrid renamedb [options] src_database_name dest_database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **renamedb**: 현재 존재하는 데이터베이스의 이름을 새로운 이름으로 변경하기 위한 명령으로, 데이터베이스가 구동 정지 상태인 경우에만 정상적으로 수행된다. 관련된 정보 볼륨, 로그 볼륨, 제어 파일도 함께 새로 지정된 이름으로 변경된다.

*   *src_database_name*: 이름을 바꾸고자 하는 현재 존재하는 데이터베이스의 이름이며, 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않는다.

*   *dest_database_name*: 새로 부여하고자 하는 데이터베이스의 이름이며, 현재 존재하는 데이터베이스 이름과 중복되어서는 안 된다. 이 역시, 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않는다.

다음은 **cubrid renamedb** 에 대한 [options]이다.

.. program:: renamedb

.. option:: -E, --extented-volume-path=PATH

	확장 볼륨의 이름을 변경한 후 새 디렉터리 경로로 이동하는 명령으로서, **-E** 옵션을 이용하여 변경된 이름을 가지는 확장 볼륨을 이동시킬 새로운 디렉터리 경로(예: /dbtemp/newaddvols/)를 지정한다.

	**-E** 옵션을 주지 않으면, 확장 볼륨은 기존 위치에서 이름만 변경된다. 이때, 기존 데이터베이스 볼륨의 디스크 파티션 외부에 있는 디렉터리 경로 또는 유효하지 않은 디렉터리 경로가 지정되는 경우 데이터베이스 이름 변경 작업은 수행되지 않으며, **-i** 옵션과 병행될 수 없다. ::

		cubrid renamedb -E /dbtemp/newaddvols/ testdb testdb_1

.. option::	-i, --control-file FILE

	각 볼륨 또는 파일에 대하여 일괄적으로 데이터베이스 이름을 변경하면서 디렉터리 경로를 상이하게 지정하기 위해 디렉터리 정보가 저장된 입력 파일을 지정하는 명령으로서, **-i** 옵션을 이용한다. 
	이때, **-i** 옵션은 **-E** 옵션과 병행될 수 없다. ::

		cubrid renamedb -i rename_path testdb testdb_1

	다음은 개별적 볼륨들의 이름과 현재 디렉터리 경로, 그리고 변경된 이름의 볼륨들이 저장될 디렉터리 경로를 포함하는 파일의 구문 및 예시이다. ::

		volid   source_fullvolname   dest_fullvolname

*   *volid*: 각 볼륨을 식별하기 위한 정수이며, 데이터베이스 볼륨 정보 제어 파일(database_name_vinf)를 통해 확인할 수 있다.

*   *source_fullvolname*: 각 볼륨에 대한 현재 디렉터리 경로이다.

*   *dest_fullvolname*: 이름이 변경된 새로운 볼륨이 이동될 목적지 디렉터리 경로이다. 만약, 목적지 디렉터리가 유효하지 않은 경우 데이터베이스 이름 변경 작업은 수행되지 않는다. 

::

	  -5  /home1/user/testdb_vinf    /home1/CUBRID/databases/testdb_1_vinf   
	  -4  /home1/user/testdb_lginf   /home1/CUBRID/databases/testdb_1_lginf
	  -3  /home1/user/testdb_bkvinf   /home1/CUBRID/databases/testdb_1_bkvinf
	  -2  /home1/user/testdb_lgat   /home1/CUBRID/databases/testdb_1_lgat
	   0  /home1/user/testdb   /home1/CUBRID/databases/testdb_1
	   1  /home1/user/backup/testdb_x001   /home1/CUBRID/databases/backup/testdb_1_x001
   
.. option::	-d, --delete-backup

	데이터베이스의 이름을 변경하면서 데이터베이스와 와 동일 위치에 있는 모든 백업 볼륨 및 백업 정보 파일을 함께 강제 삭제하는 명령이다. 일단, 데이터베이스 이름이 변경되면 이전 이름의 백업 파일은 이용할 수 없으므로 주의해야 한다. 만약, **-d** 옵션을 지정하지 않으면 백업 볼륨 및 백업 정보 파일은 삭제되지 않는다. ::

		cubrid renamedb -d testdb testdb_1

데이터베이스 호스트 변경
========================

**cubrid alterdbhost** 유틸리티는 지정된 데이터베이스의 호스트 이름을 설정하거나 변경한다. 즉, **databases.txt** 에 있는 호스트 이름을 변경한다.

	cubrid alterdbhost [<option>] database_name 

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **alterdbhost**: 현 데이터베이스의 호스트 이름을 새로운 이름으로 변경하기 위한 명령이다.

**cubrid alterdbhost** 에서 사용하는 옵션은 다음과 같다.	
	
.. program:: alterdbhost

.. option:: -h, --host=HOST

    뒤에 변경할 호스트 이름을 지정하며, 옵션을 생략하면 호스트 이름으로 localhost를 지정한다.



데이터베이스 복사/이동
======================

**cubrid copydb** 유틸리티는 데이터베이스를 한 위치에서 다른 곳으로 복사 또는 이동하며, 인자로 원본 데이터베이스 이름과 새로운 데이터베이스 이름이 지정되어야 한다. 이때, 새로운 데이터베이스 이름은 원본 데이터베이스 이름과 다른 이름으로 지정되어야 하고, 새로운 데이터베이스에 대한 위치 정보는 **databases.txt** 에 등록된다.

**cubrid copydb** 유틸리티는 원본 데이터베이스가 정지 상태일 때(오프라인)에만 실행할 수 있다. ::

	cubrid copydb [<options>] src-database-name dest-database-name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **copydb**: 원본 데이터베이스를 새로운 위치로 이동 또는 복사하는 명령이다.

*   *src-database-name*: 복사 또는 이동하고자 하는 원본 데이터베이스 이름이다.

*   *dest-database-name*: 새로운 데이터베이스 이름이다.

[options]를 생략하면 원본 데이터베이스를 현재 작업 디렉터리에 복사한다.

**cubrid copydb** 에 대한 [options]는 다음과 같다.

.. program:: copydb

.. option:: --server-name=HOST

	새로운 데이터베이스의 서버 호스트 이름을 명시하며, 이는 **databases.txt** 의 **db-host** 항목에 등록된다. 이 옵션을 생략하면, 로컬 호스트가 등록된다. ::

		cubrid copydb --server-name=cub_server1 demodb new_demodb

.. option:: -F, --file-path=PATH

	새로운 데이터베이스 볼륨이 저장되는 특정 디렉터리 경로를 지정할 수 있다. 절대 경로로 지정해야 하며, 존재하지 않는 디렉터리를 지정하면 에러를 출력한다. 이 옵션을 생략하면 현재 작업 디렉터리에 새로운 데이터베이스의 볼륨이 생성된다. 이 경로는 **databases.txt** 의 **vol-path** 항목에 등록된다. ::
	
		cubrid copydb -F /home/usr/CUBRID/databases demodb new_demodb

.. option:: -L, --log-path=PATH

	새로운 데이터베이스 로그 볼륨이 저장되는 특정 디렉터리 경로를 지정할 수 있다. 절대 경로로 지정해야 하며, 존재하지 않는 디렉터리를 지정하면 에러를 출력한다. 이 옵션을 생략하면 새로운 데이터베이스 볼륨이 저장되는 경로에 로그 볼륨도 함께 생성된다. 이 경로는 **databases.txt** 의 **log-path** 항목에 등록된다. ::
	
		cubrid copydb -L /home/usr/CUBRID/databases/logs demodb new_demodb

.. option:: -E, --extended-volume-path=PATH

	새로운 데이터베이스의 확장 정보 볼륨이 저장되는 특정 디렉터리 경로를 지정할 수 있다. 이 옵션을 생략하면 새로운 데이터베이스 볼륨이 저장되는 경로 또는 제어 파일에 등록된 경로에 확장 정보 볼륨이 저장된다. **-i** 옵션과 병행될 수 없다. ::

		cubrid copydb -E home/usr/CUBRID/databases/extvols demodb new_demodb

.. option:: -i, --control-file=FILE

	대상 데이터베이스에 대한 복수 개의 볼륨들을 각각 다른 디렉터리에 복사 또는 이동하기 위해서, 원본 볼륨의 경로 및 새로운 디렉터리 경로 정보를 포함하는 입력 파일을 지정할 수 있다. 이때, **-i** 옵션은 **-E** 옵션과 병행될 수 없다. 아래 예제에서는 copy_path라는 입력 파일을 예로 사용했다. ::

		cubrid copydb -i copy_path demodb new_demodb

	다음은 각 볼륨들의 이름과 현재 디렉터리 경로, 그리고 새로 복사할 디렉터리 및 새로운 볼륨 이름을 포함하는 입력 파일의 예시이다. ::

		# volid   source_fullvolname   dest_fullvolname
		0 /usr/databases/demodb        /drive1/usr/databases/new_demodb
		1 /usr/databases/demodb_data1  /drive1/usr/databases/new_demodb new_data1
		2 /usr/databases/ext/demodb index1 /drive2//usr/databases/new_demodb new_index1
		3 /usr/ databases/ext/demodb index2  /drive2/usr/databases/new_demodb new_index2

	*   volid : 각 볼륨을 식별하기 위한 정수이며, 데이터베이스 볼륨 정보 제어 파일( **database_name_vinf** )를 통해 확인할 수 있다.

	*   source_fullvolname : 원본 데이터베이스의 각 볼륨이 존재하는 현재 디렉터리 경로이다.

	*   dest_fullvolname : 새로운 데이터베이스의 각 볼륨이 저장될 디렉터리 경로이며, 유효한 디렉터리를 지정해야 한다.

.. option:: -r, --replace

	새로운 데이터베이스 이름이 기존 데이터베이스 이름과 중복되더라도 에러를 출력하지 않고 덮어쓴다. ::

		cubrid copydb -r -F /home/usr/CUBRID/databases demodb new_demodb

.. option:: -d 또는 --delete-source

	새로운 데이터베이스로 복사한 후, 원본 데이터베이스를 제거한다. 이 옵션이 주어지면 데이터베이스 복사 후 **cubrid deletedb** 를 수행하는 것과 동일하다. 단, 원본 데이터베이스에 **LOB** 데이터를 포함하는 경우, 원본 데이터베이스 대한 **LOB** 파일 디렉터리 경로가 새로운 데이터베이스로 복사되어 **databases.txt** 의 **lob-base-path** 항목에 등록된다. ::

		cubrid copydb -d -F /home/usr/CUBRID/databases demodb new_demodb

.. option:: --copy-lob-path=PATH

	원본 데이터베이스에 대한 **LOB** 파일 디렉터리 경로를 새로운 데이터베이스의 **LOB** 파일 경로로 복사하고, 원본 데이터베이스를 복사한다. 이 옵션을 생략하면, **LOB** 파일 디렉터리 경로를 복사하지 않으므로, **databases.txt** 파일의 **lob-base-path** 항목을 별도로 수정해야 한다. **-B** 옵션과 병행할 수 없다. ::

		cubrid copydb --copy-lob-path=/home/usr/CUBRID/databases/new_demodb/lob demodb new_demodb


.. option::	-B, --lob-base-path=PATH

	**-B** 옵션을 사용하여 특정 디렉터리를 새로운 데이터베이스에 대한 **LOB** 파일 디렉터리 경로를 지정하면서 원본 데이터베이스를 복사한다.
	**--copy-lob-path** 옵션과 병행할 수 없다. ::

		cubrid copydb -B /home/usr/CUBRID/databases/new_lob demodb new_demodb

데이터베이스 등록
=================

**cubrid installdb** 유틸리티는 데이터베이스 위치 정보를 저장하는 **databases.txt** 에 새로 설치된 데이터베이스 정보를 등록한다. 이 유틸리티의 실행은 등록 대상 데이터베이스의 동작에 영향을 끼치지 않는다.

::

	cubrid installdb [<options>] database_name	

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **installdb**: 이동 또는 복사된 데이터베이스의 정보를 **databases.txt** 에 등록하는 명령이다.

*   *database_name*: **databases.txt** 에 등록하고자 하는 데이터베이스의 이름이다.

[options]를 생략하는 경우, 해당 데이터베이스가 존재하는 디렉터리에서 명령을 수행해야 한다.

**cubrid installdb** 에 대한 [options]는 다음과 같다.

.. program:: installdb

.. option:: --server-name=HOST

	대상 데이터베이스의 서버 호스트 정보를 지정된 호스트 명으로 **databases.txt** 에 등록한다. 이 옵션을 생략하면, 현재의 호스트 정보가 등록된다.  ::

		cubrid installdb --server-name=cub_server1 testdb

.. option:: -F, --file-path=PATH
		
	대상 데이터베이스 볼륨의 디렉터리 경로를 **databases.txt** 에 등록한다. 이 옵션을 생략하면 기본값인 현재 디렉터리 경로가 등록된다.  ::

		cubrid installdb -F /home/cubrid/CUBRID/databases/testdb testdb

.. option:: -L, --log-path=PATH

	대상 데이터베이스 로그 볼륨의 디렉터리 경로를 **databases.txt** 에 등록한다. 이 옵션을 생략하면 데이터베이스 볼륨의 디렉터리 경로가 등록된다.  ::
	
		cubrid installdb -L /home/cubrid/CUBRID/databases/logs/testdb testdb

.. _spacedb:

사용 공간 확인
==============

**cubrid spacedb** 유틸리티는 사용 중인 데이터베이스 볼륨의 공간을 확인하기 위해서 사용된다.
**cubrid spacedb** 유틸리티는 데이터베이스에 있는 모든 영구 데이터 볼륨의 간략한 설명을 보여준다. **cubrid spacedb** 유틸리티에 의해 반환되는 정보는 볼륨 ID와 이름, 각 볼륨의 목적, 각 볼륨과 관련된 총(total) 공간과 빈(free) 공간이다. 

::

	cubrid spacedb [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **spacedb**: 대상 데이터베이스에 대한 공간을 확인하는 명령으로 데이터베이스 서버가 구동 정지 상태인 경우에만 정상적으로 수행된다.

*   *database_name*: 공간을 확인하고자 하는 데이터베이스의 이름이며, 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않는다.


다음은 **cubrid spacedb** 에 대한 [options]이다.

.. program:: spacedb

.. option:: -o FILE

	데이터베이스의 공간 정보에 대한 결과를 지정한 파일에 저장한다. ::
	
		cubrid spacedb -o db_output testdb

.. option:: -S, --SA-mode
	
	서버 프로세스를 구동하지 않고 데이터베이스에 접근하는 독립 모드(standalone)로 작업하기 위해 지정되며, 인수는 없다. **-S** 옵션을 지정하지 않으면, 시스템은 클라이언트/서버 모드로 인식한다. ::

		cubrid spacedb --SA-mode testdb

.. option:: -C, --CS-mode

	**-C** 옵션은 서버 프로세스와 클라이언트 프로세스를 각각 구동하여 데이터베이스에 접근하는 클라이언트/서버 모드로 작업하기 위한 옵션이며, 인수는 없다.
	**-C** 옵션을 지정하지 않더라도 시스템은 기본적으로 클라이언트/서버 모드로 인식한다. ::

		cubrid spacedb --CS-mode testdb

.. option:: --size-unit={PAGE|M|G|T|H}

	데이터베이스 볼륨의 공간을 지정한 크기 단위로 출력하기 위한 옵션이며, 기본값은 H이다.
	단위를 PAGE, M, G, T, H로 설정할 수 있으며, 각각 페이지, MB(megabytes), GB(gigabytes), TB(terabytes), 자동 지정을 의미한다. 자동 지정을 의미하는 H로 설정하면 데이터베이스 크기가 1MB 이상 1024MB 미만일 때 MB 단위로, 1GB 이상 1024GB 미만일 때 GB 단위로 결정된다. ::

		cubrid spacedb --size_unit=M testdb
		cubrid spacedb --size_unit=H testdb

.. option:: -s, --summarize

	데이터 볼륨(DATA), 인덱스 볼륨(INDEX), 범용 볼륨(GENERIC), 임시 볼륨(TEMP), 일시적 임시 볼륨(TEMP TEMP)별로 전체 공간(total_pages), 사용 공간(used_pages), 빈 공간(free_pages)을 합산하여 출력한다. ::

		cubrid spacedb -s testdb

사용 공간 정리
==============

**cubrid compactdb** 유틸리티는 데이터베이스 볼륨 중에 사용되지 않는 공간을 확보하기 위해서 사용된다. 데이터베이스 서버가 정지된 경우(offline)에는 독립 모드(stand-alone mode)로, 데이터베이스가 구동 중인 경우(online)에는 클라이언트 서버 모드(client-server mode)로 공간 정리 작업을 수행할 수 있다.

**cubrid compactdb** 유틸리티는 삭제된 객체들의 OID와 클래스 변경에 의해 점유되고 있는 공간을 확보한다. 객체를 삭제하면 삭제된 객체를 참조하는 다른 객체가 있을 수 있기 때문에 삭제된 객체에 대한 OID는 바로 사용 가능한 빈 공간이 될 수 없다.

**cubrid compactdb** 유틸리티를 수행하면 삭제된 객체에 대한 참조를 **NULL** 로 표시하는데, 이렇게 **NULL** 로 표시된 공간은 OID가 재사용할 수 있는 공간임을 의미한다. ::

	cubrid compactdb [<options>] database_name [ class_name1, class_name2, ...]

*   **cubrid**: 큐브리드 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **compactdb**: 대상 데이터베이스에 대하여 삭제된 데이터에 할당되었던 OID가 재사용될 수 있도록 공간을 정리하는 명령으로서, 데이터베이스가 구동 정지 상태인 경우에만 정상적으로 수행된다.

*   *database_name*: 공간을 정리할 데이터베이스의 이름이며, 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않는다.

*   *class_name_list*: 공간을 정리할 테이블 이름 리스트를 데이터베이스 이름 뒤에 직접 명시할 수 있으며,
    **-i** 옵션과 함께 사용할 수 없다. 클라이언트/서버 모드에서만 명시할 수 있다.

클라이언트/서버 모드에서만 **-I**, **-i**, **-c**, **-d**, **-p** 옵션을 사용할 수 있다.
	
다음은 **cubrid compactdb** 에 대한 [options]이다.
	
.. program:: compactdb

.. option:: -v, --verbose

	어느 클래스가 현재 정리되고 있는지, 얼마나 많은 인스턴스가 그 클래스를 위하여 처리되었는지를 알리는 메시지를 화면에 출력할 수 있다. ::

		cubrid compactdb -v testdb

.. option:: -S, --SA-mode

	데이터베이스 서버가 구동 중단된 상태에서 독립 모드(standalone)로 공간 정리 작업을 수행하기 위해 지정되며, 인수는 없다.
	**-S** 옵션을 지정하지 않으면, 시스템은 클라이언트/서버 모드로 인식한다. ::

		cubrid compactdb --SA-mode testdb

.. option:: -C, --CS-mode

	**-C** 옵션은 데이터베이스 서버가 구동 중인 상태에서 클라이언트/서버 모드로 공간 정리 작업을 수행하기 위해 지정되며, 인수는 없다. **-C** 옵션이 생략되더라도 시스템은 기본적으로 클라이언트/서버 모드로 인식한다. 클라이언트/서버 모드에서만 -I, -i, -c, -d, -p 옵션을 사용할 수 있다.

다음은 클라이언트/서버 모드에서만 사용할 수 있는 옵션이다.
	
.. option:: -i, --input-class-file=FILE

	대상 테이블 이름을 포함하는 입력 파일 이름을 지정할 수 있다. 라인 당 하나의 테이블 이름을 명시하며, 유효하지 않은 테이블 이름은 무시된다. 이 옵션을 지정하는 경우, 데이터베이스 이름 뒤에 대상 테이블 이름 리스트를 직접 명시할 수 없으므로 주의한다.

.. option:: -p, --pages-commited-once=NUMBER

	한 번에 커밋할 수 있는 최대 페이지 수를 지정한다. 기본값은 **10** 이며, 최소 값은 1, 최대 값은 10이다. 옵션 값이 작으면 클래스/인스턴스에 대한 잠금 비용이 작으므로 동시성은 향상될 수 있으나 작업 속도는 저하될 수 있고, 옵션 값이 크면 동시성은 저하되나 작업 속도는 향상될 수 있다.  ::

		cubrid compactdb --CS-mode -p 10 testdb tbl1, tbl2, tbl5

.. option:: -d, --delete-old-repr

	카탈로그에서 과거 테이블 표현(스키마 구조)을 삭제할 수 있다. **ALTER** 문에 의해 칼럼이 추가되거나 삭제되는 경우 기존의 레코드에 대해 과거의 스키마를 참조하고 있는 상태로 두면, 스키마를 업데이트하는 비용을 들이지 않기 때문에 평소에는 과거의 테이블 표현을 유지하는 것이 좋다.

.. option:: -I, --Instance-lock-timeout=NUMBER

	인스턴스 잠금 타임아웃 값을 지정할 수 있다. 기본값은 **2** (초)이며, 최소 값은 1, 최대 값은 10이다. 설정된 시간동안 잠금 인스턴스를 대기하므로, 옵션 값이 작을수록 작업 속도는 향상될 수 있으나 처리 가능한 인스턴스 개수가 적어진다. 반면, 옵션 값이 클수록 작업 속도는 저하되나 더 많은 인스턴스에 대해 작업을 수행할 수 있다.

.. option::-c, --class-lock-timeout=NUMBER

	클래스 잠금 타임아웃 값을 지정할 수 있다. 기본값은 **10**(초)이며, 최소값은 1, 최대 값은 10이다. 설정된 시간동안 잠금 테이블을 대기하므로, 옵션 값이 작을수록 작업 속도는 향상될 수 있으나 처리 가능한 테이블 개수가 적어진다. 반면, 옵션 값이 클수록 작업 속도는 저하되나 더 많은 테이블에 대해 작업을 수행할 수 있다.

통계 정보 갱신
==============

CUBRID의 질의 최적화기가 사용하는 테이블에 있는 객체들의 수, 접근하는 페이지들의 수, 속성 값들의 분산 같은 통계 정보를 갱신한다. ::

	cubrid optimizedb [option] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **optimizedb**: 대상 데이터베이스에 대하여 비용 기반 질의 최적화에 사용되는 통계 정보를 업데이트한다. 옵션을 지정하는 경우, 지정한 클래스에 대해서만 업데이트한다.

*   *database_name*: 비용기반 질의 최적화용 통계 자료를 업데이트하려는 데이터베이스 이름이다.

다음은 *cubrid optimizedb* 에 대한 [option]이다.

.. program:: optimizedb

.. option:: -n, --class-name

	**-n** 옵션을 이용하여 해당 클래스의 질의 통계 정보를 업데이트하는 명령이다. ::

		cubrid optimizedb -n event_table testdb
	
다음은 대상 데이터베이스의 전체 클래스의 질의 통계 정보를 업데이트하는 명령이다. ::

	cubrid optimizedb testdb

.. _statdump:

데이터베이스 서버 실행 통계 정보 출력
=====================================

**cubrid statdump** 유틸리티를 이용해 CUBRID 데이터베이스 서버가 실행한 통계 정보를 확인할 수 있으며, 통계 정보 항목은 크게 File I/O 관련, 페이지 버퍼 관련, 로그 관련, 트랜잭션 관련, 동시성 관련, 인덱스 관련, 쿼리 수행 관련, 네트워크 요청 관련으로 구분된다. 

단, 유틸리티 실행 전에 **cubrid.conf** 파일에 **communication_histogram** 파라미터를 **yes** 로 설정해야 한다. 또한, csql에서 세션 명령어( **;.h on** )을 이용하여 서버의 통계 정보를 확인할 수 있다. ::
	
	cubrid statdump [options] database_name
	
*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **statdump**: 대상 데이터베이스 서버 실행 통계 정보를 출력하는 명령어이다. 데이터베이스가 동작 중일 때에만 정상 수행된다.

*   *database_name*: 통계 자료를 확인하고자 하는 대상 데이터베이스 이름이다.

다음은 **cubrid statdump** 에 대한 [options]이다.

.. program:: statdump

.. option:: -i, --interval=SECOND

	실행 통계 정보를 초 단위로 주기적으로 출력한다.
	
	::

		cubrid statdump -i 5 testdb
		 
		Thu April 07 23:10:08 KST 2011
		 
		 *** SERVER EXECUTION STATISTICS ***
		Num_file_creates              =          0
		Num_file_removes              =          0
		Num_file_ioreads              =          0
		Num_file_iowrites             =          0
		Num_file_iosynches            =          0
		Num_data_page_fetches         =          0
		Num_data_page_dirties         =          0
		Num_data_page_ioreads         =          0
		Num_data_page_iowrites        =          0
		Num_data_page_victims         =          0
		Num_data_page_iowrites_for_replacement =          0
		Num_log_page_ioreads          =          0
		Num_log_page_iowrites         =          0
		Num_log_append_records        =          0
		Num_log_archives              =          0
		Num_log_checkpoints           =          0
		Num_log_wals                  =          0
		Num_page_locks_acquired       =          0
		Num_object_locks_acquired     =          0
		Num_page_locks_converted      =          0
		Num_object_locks_converted    =          0
		Num_page_locks_re-requested   =          0
		Num_object_locks_re-requested =          0
		Num_page_locks_waits          =          0
		Num_object_locks_waits        =          0
		Num_tran_commits              =          0
		Num_tran_rollbacks            =          0
		Num_tran_savepoints           =          0
		Num_tran_start_topops         =          0
		Num_tran_end_topops           =          0
		Num_tran_interrupts           =          0
		Num_btree_inserts             =          0
		Num_btree_deletes             =          0
		Num_btree_updates             =          0
		Num_btree_covered             =          0
		Num_btree_noncovered          =          0
		Num_btree_resumes             =          0
		Num_btree_multirange_optimization =      0
		Num_query_selects             =          0
		Num_query_inserts             =          0
		Num_query_deletes             =          0
		Num_query_updates             =          0
		Num_query_sscans              =          0
		Num_query_iscans              =          0
		Num_query_lscans              =          0
		Num_query_setscans            =          0
		Num_query_methscans           =          0
		Num_query_nljoins             =          0
		Num_query_mjoins              =          0
		Num_query_objfetches          =          0
		Num_network_requests          =          1
		Num_adaptive_flush_pages      =          0
		Num_adaptive_flush_log_pages  =          0
		Num_adaptive_flush_max_pages  =        900
		 
		 *** OTHER STATISTICS ***
		Data_page_buffer_hit_ratio    =       0.00

	다음은 위의 데이터베이스 서버 실행 통계 정보에 대한 각 항목 설명이다.

	+-------------+----------------------------------------+-----------------------------------------------------------------------------+
	| 분류        | 항목                                   | 설명                                                                        |
	+=============+========================================+=============================================================================+
	| File I/O    | Num_file_removes                       | 삭제한 파일 개수                                                            |
	| 관련        |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_file_creates                       | 생성한 파일 개수                                                            |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_file_ioreads                       | 디스크로부터 읽은 횟수                                                      |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_file_iowrites                      | 디스크로 저장한 횟수                                                        |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_file_iosynches                     | 디스크와 동기화를 수행한 횟수                                               |
	|             |                                        |                                                                             |
	+-------------+----------------------------------------+-----------------------------------------------------------------------------+
	| 페이지 버퍼 | Num_data_page_fetches                  | 가져오기(fetch)한 페이지 수                                                 |
	| 관련        |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_data_page_dirties                  | 더티 페이지 수                                                              |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_data_page_ioreads                  | 읽은 페이지 수                                                              |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_data_page_iowrites                 | 저장한 페이지 수                                                            |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_data_page_victims                  | 데이터 페이지에서 디스크로 내려갈 후보(victim) 데이터를 정하는 횟수         |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_data_page_iowrites_for_replacement | 후보로 선정되어 디스크로 쓰여진 데이터 페이지 수                            |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_adaptive_flush_pages               | 데이터 버퍼로부터 디스크로 내려 쓰기(flush)한 데이터 페이지 수              |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_adaptive_flush_log_pages           | 로그 버퍼로부터 디스크로 내려 쓰기(flush)한 로그 페이지 수                  |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_adaptive_flush_max_pages           | 데이터 및 로그 버퍼로부터 디스크로 내려 쓰기(flush)를 허용하는 최대         |
	|             |                                        | 페이지 수                                                                   |
	+-------------+----------------------------------------+-----------------------------------------------------------------------------+
	| 로그 관련   | Num_log_page_ioreads                   | 읽은 로그 페이지의 수                                                       |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_log_page_iowrites                  | 저장한 로그 페이지의 수                                                     |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_log_append_records                 | 추가(append)한 로그 레코드의 수                                             |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_log_archives                       | 보관 로그의 개수                                                            |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_log_checkpoints                    | 체크포인트 수행 횟수                                                        |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_log_wals                           | 현재 사용하지 않음                                                          |
	|             |                                        |                                                                             |
	+-------------+----------------------------------------+-----------------------------------------------------------------------------+
	| 트랜잭션    | Num_tran_commits                       | 커밋한 횟수                                                                 |
	| 관련        |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_tran_rollbacks                     | 롤백한 횟수                                                                 |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_tran_savepoints                    | 세이브포인트 횟수                                                           |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_tran_start_topops                  | 시작한 top operation의 개수                                                 |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_tran_end_topops                    | 종료한 top peration의 개수                                                  |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_tran_interrupts                    | 인터럽트 개수                                                               |
	|             |                                        |                                                                             |
	+-------------+----------------------------------------+-----------------------------------------------------------------------------+
	| 동시성/잠금 | Num_page_locks_acquired                | 페이지 잠금을 획득한 횟수                                                   |
	| 관련        |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_object_locks_acquired              | 오브젝트 잠금을 획득한 횟수                                                 |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_page_locks_converted               | 페이지 잠금 타입을 변환한 횟수                                              |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_object_locks_converted             | 오브젝트 잠금 타입을 변환한 횟수                                            |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_page_locks_re-requested            | 페이지 잠금을 재요청한 횟수                                                 |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_object_locks_re-requested          | 오브젝트 잠금을 재요청한 횟수                                               |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_page_locks_waits                   | 잠금을 대기하는 페이지 개수                                                 |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_object_locks_waits                 | 잠금을 대기하는 오브젝트 개수                                               |
	|             |                                        |                                                                             |
	+-------------+----------------------------------------+-----------------------------------------------------------------------------+
	| 인덱스 관련 | Num_btree_inserts                      | 삽입된 항목의 개수                                                          |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_btree_deletes                      | 삭제된 항목의 개수                                                          |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_btree_updates                      | 갱신된 항목의 개수                                                          |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_btree_covered                      | 질의 시 인덱스가 데이터를 모두 포함한 경우의 개수                           |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_btree_noncovered                   | 질의 시 인덱스가 데이터를 일부분만 포함하거나 전혀 포함하지 않은 경우의     |
	|             |                                        | 개수                                                                        |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_btree_resumes                      | index_scan_oid_buffer_pages를 초과한 인덱스 스캔 횟수                       |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_btree_multirange_optimization      | WHERE … IN … LIMIT 조건 질의문에 대해 다중 범위                             |
	|             |                                        | 최적화(multi-range optimization)를 수행한 횟수                              |
	+-------------+----------------------------------------+-----------------------------------------------------------------------------+
	| 쿼리 관련   | Num_query_selects                      | SELECT 쿼리의 수행 횟수                                                     |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_inserts                      | INSERT 쿼리의 수행 횟수                                                     |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_deletes                      | DELETE 쿼리의 수행 횟수                                                     |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_updates                      | UPDATE 쿼리의 수행 횟수                                                     |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_sscans                       | 순차 스캔(풀 스캔) 횟수                                                     |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_iscans                       | 인덱스 스캔 횟수                                                            |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_lscans                       | LIST 스캔 횟수                                                              |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_setscans                     | SET 스캔 횟수                                                               |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_methscans                    | METHOD 스캔 횟수                                                            |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_nljoins                      | Nested Loop 조인 횟수                                                       |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_mjoins                       | 병합 조인 횟수                                                              |
	|             |                                        |                                                                             |
	|             +----------------------------------------+-----------------------------------------------------------------------------+
	|             | Num_query_objfetches                   | 객체를 가져오기(fetch)한 횟수                                               |
	|             |                                        |                                                                             |
	+-------------+----------------------------------------+-----------------------------------------------------------------------------+
	| 네트워크    | Num_network_requests                   | 네트워크 요청 횟수                                                          |
	| 요청 관련   |                                        |                                                                             |
	+-------------+----------------------------------------+-----------------------------------------------------------------------------+
	| 버퍼 히트율 | Data_page_buffer_hit_ratio             | 페이지 버퍼의 Hit Ratio                                                     |
	| 관련        |                                        | (Num_data_page_fetches - Num_data_page_ioreads)*100 / Num_data_page_fetches |
	|             |                                        |                                                                             |
	+-------------+----------------------------------------+-----------------------------------------------------------------------------+

.. option:: -o, --output-file=FILE

	대상 데이터베이스 서버의 실행 통계 정보를 지정된 파일에 저장한다. ::

		cubrid statdump -o statdump.log testdb

.. option:: -c, --cumulative

	**-c** 옵션을 이용하여 대상 데이터베이스 서버의 누적된 실행 통계 정보를 출력할 수 있다.

	**-i** 옵션과 결합하면, 지정된 시간 간격(interval)마다 실행 통계 정보를 확인할 수 있다.

::

		cubrid statdump -i 5 -c testdb

.. option:: -s, --substr=STRING

	**-s** 옵션 뒤에 문자열을 지정하면, 항목 이름 내에 해당 문자열을 포함하는 통계 정보만 출력할 수 있다.

	다음 예는 항목 이름 내에 "data"를 포함하는 통계 정보만 출력한다.

	::
	
		cubrid statdump -s data testdb

		*** SERVER EXECUTION STATISTICS ***
		Num_data_page_fetches         =        135
		Num_data_page_dirties         =          0
		Num_data_page_ioreads         =          0
		Num_data_page_iowrites        =          0
		Num_data_page_victims         =          0
		Num_data_page_iowrites_for_replacement =          0
		 
		 *** OTHER STATISTICS ***
		Data_page_buffer_hit_ratio    =     100.00


.. note ::

	각 상태 정보는 64비트 **INTEGER** 로 구성되어 있으며, 누적된 값이 한도를 넘으면 해당 실행 통계 정보가 유실될 수 있다.

.. _lockdb:

잠금(Lock) 상태 확인
====================

**cubrid lockdb** 는 대상 데이터베이스에 대하여 현재 트랜잭션에서 사용되고 있는 잠금 정보를 확인하는 유틸리티이다. ::

	cubrid lockdb [<option>] database_name
	
*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **lockdb**: 대상 데이터베이스에 대하여 현재 트랜잭션에서 사용되고 있는 잠금 정보를 확인하는 명령이다.

*   *database_name*: 현재 트랜잭션의 잠금 정보를 확인하는 데이터베이스 이름이다.

다음 예는 옵션 없이 testdb 데이터베이스의 잠금 정보를 화면에 출력한다.

::

	cubrid lockdb testdb

다음은 **cubrid lockdb** 에 대한 [option]이다.
	
.. program:: lockdb

.. option:: -o, --output-file=FILE

	데이터베이스의 잠금 정보를 output.txt로 출력한다. ::

		cubrid lockdb -o output.txt testdb

출력 내용
---------

**cubrid lockdb** 의 출력 내용은 논리적으로 3개의 섹션으로 나뉘어져 있다.

	* 서버에 대한 잠금 설정

	* 현재 데이터베이스에 접속한 클라이언트들

	* 객체 잠금 테이블의 내용

**서버에 대한 잠금 설정**

**cubrid lockdb** 출력 내용의 첫 번째 섹션은 데이터베이스 서버에 대한 잠금 설정이다.

::

	*** Lock Table Dump ***
	 Lock Escalation at = 100000, Run Deadlock interval = 0

위에서 잠금 에스컬레이션 레벨은 100000레코드로, 교착 상태 탐지 간격은 0초로 설정되어 있다.

관련 시스템 파라미터인 **lock_escalation** 과 **deadlock_detection_interval** 에 대한 설명은 :ref:`lock-parameters` 를 참고한다.

**현재 데이터베이스에 접속한 클라이언트들**

**cubrid lockdb** 출력 내용의 두 번째 섹션은 데이터베이스에 연결된 모든 클라이언트의 정보를 포함한다. 이 정보에는 각각의 클라이언트에 대한 트랜잭션 인덱스, 프로그램 이름, 사용자 ID, 호스트 이름, 프로세스 ID, 고립 수준, 그리고 잠금 타임아웃 설정이 포함된다.

::

	Transaction (index 1, csql, dba@cubriddb|12854)
	Isolation READ COMMITTED CLASSES AND READ UNCOMMITTED INSTANCES
	Timeout_period -1

위에서 트랜잭션 인덱스는 1이고, 프로그램 이름은 csql, 사용자 이름은 dba, 호스트 이름은 cubriddb, 클라이언트 프로세스 식별자는 12854, 고립 수준은 READ COMMITTED CLASSES AND READ UNCOMMITTED INSTANCES, 그리고 잠금 타임아웃은 무제한이다.

트랜잭션 인덱스가 0인 클라이언트는 내부적인 시스템 트랜잭션이다. 이것은 데이터베이스의 체크포인트 수행과 같이 특정한 시간에 잠금을 획득할 수 있지만 대부분의 경우 이 트랜잭션은 어떤 잠금도 획득하지 않을 것이다.

**cubrid lockdb** 유틸리티는 잠금 정보를 가져오기 위해 데이터베이스에 접속하기 때문에 **cubrid lockdb** 자체가 하나의 클라이언트이고 따라서 클라이언트의 하나로 출력된다.

**객체 잠금 테이블**

**cubrid lockdb** 출력 내용의 세 번째 섹션은 객체 잠금 테이블의 내용을 포함한다. 이것은 어떤 객체에 대해서 어떤 클라이언트가 어떤 모드로 잠금을 가지고 있는지, 어떤 객체에 대해서 어떤 클라이언트가 어떤 모드로 기다리고 있는지를 보여준다. 객체 잠금 테이블 결과물의 첫 부분에는 얼마나 많은 객체가 잠금되었는지가 출력된다. 

::

	Object lock Table:
		Current number of ojbects which are locked = 2001

**cubrid lockdb** 는 잠금을 획득한 각각의 객체에 대한 객체의 OID와 Object type, 테이블 이름을 출력한다. 추가적으로 객체에 대해서 잠금을 보유하고 있는 트랜잭션의 개수(Num holders), 잠금을 보유하고 있지만 상위 잠금으로 변환(예를 들어 U_LOCK에서 X_LOCK으로 잠금 변환)하지 못해 차단된 트랜잭션의 개수(Num blocked-holders), 객체의 잠금을 기다리는 다른 트랜잭션의 개수(Num waiters)가 출력된다. 그리고 잠금을 보유하고 있는 클라이언트 트랜잭션, 차단된 클라이언트 트랜잭션, 기다리는 클라이언트 트랜잭션의 리스트가 출력된다.

다음 예는 Object type이 instance of class, 즉 레코드인 경우, OID( 2| 50| 1)인 객체에 대해서 트랜잭션 2가 S_LOCK을 가지고 있고, 트랜잭션 1이 U_LOCK을 획득하고 있지만 트랜잭션 2가 S_LOCK을 획득하고 있기 때문에 X_LOCK으로 변환하지 못해 차단되었음을 보여준다. 그리고 트랜잭션 3은 S_LOCK을 대기하고 있지만 트랜잭션 2가 X_LOCK을 대기하고 있기 때문에 차단되었음을 보여준다.

::

	OID = 2| 50| 1
	Object type: instance of class ( 0| 62| 5) = athlete
	Num holders = 1, Num blocked-holders= 1, Num waiters = 1
	LOCK HOLDERS :
		Tran_index = 2, Granted_mode = S_LOCK, Count = 1
	BLOCKED LOCK HOLDERS :
		Tran_index = 1, Granted_mode = U_LOCK, Count = 3
		Blocked_mode = X_LOCK
						Start_waiting_at = Fri May 3 14:44:31 2002
						Wait_for _nsecs = -1
	LOCK WAITERS :
		Tran_index = 3, Blocked_mode = S_LOCK
						Start_waiting_at = Fri May 3 14:45:14 2002
						Wait_for_nsecs = -1

Object type이 Index key of class, 즉 인덱스 키인 경우 테이블의 인덱스에 대한 잠금 정보를 출력한다.

::

	OID = -662|   572|-32512
	Object type: Index key of class ( 0|   319|  10) = athlete.
	Index name: pk_athlete_code
	Total mode of holders =   NX_LOCK, Total mode of waiters = NULL_LOCK.
	Num holders=  1, Num blocked-holders=  0, Num waiters=  0
	LOCK HOLDERS:
		Tran_index =   1, Granted_mode =  NX_LOCK, Count =   1
		
Granted_mode는 현재 획득한 잠금의 모드를 의미하고 Blocked_mode는 차된된 잠금의 모드를 의미한다. Starting_waiting_at은 잠금을 요청한 시간을 의미하고 Wait_for_nsecs는 잠금을 기다리는 시간을 의미한다. Wait_for_nsecs의 값은 lock_timeout_in_secs 시스템 파라미터에 의해 설정된다.

Object type이 Class, 즉 테이블인 경우 Nsubgranules가 출력되는데 이것은 해당 테이블 내의 특정 트랜잭션이 획득하고 있는 레코드 잠금과 키 잠금을 합한 개수이다.

::

	OID = 0| 62| 5
	Object type: Class = athlete
	Num holders = 2, Num blocked-holders= 0, Num waiters= 0
	LOCK HOLDERS:
	Tran_index = 3, Granted_mode = IS_LOCK, Count = 2, Nsubgranules = 0
	Tran_index = 1, Granted_mode = IX_LOCK, Count = 3, Nsubgranules = 1
	Tran_index = 2, Granted_mode = IS_LOCK, Count = 2, Nsubgranules = 1

데이터베이스 일관성 확인
========================

**cubrid checkdb** 유틸리티는 데이터베이스를 확인하기 위해 사용된다. **cubrid checkdb** 유틸리티를 사용하면 인덱스와 다른 데이터 구조를 확인하기 위해 데이터와 로그 볼륨의 내부적인 물리적 일치를 확인할 수 있다. 만일 **cubrid checkdb** 유틸리티의 실행 결과가 불일치로 나온다면 --**repair** 옵션으로 자동 수정을 시도해 보아야 한다.

::

	cubrid checkdb [options] database_name [table_name1 table_name2 ...]

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티

*   **checkdb**: 대상 데이터베이스에 대하여 데이터의 일관성(consistency)을 확인하는 명령

*   *database_name*: 일관성을 확인하거나 복구하려는 데이터베이스 이름

*    *table_name1 table_name2*: 일관성을 확인하거나 복구하려는 테이블 이름을 나열한다.

다음은 **cubrid checkdb** 에 대한 [options]이다.

.. program:: checkdb

.. option:: -S, --SA-mode

	서버 프로세스를 구동하지 않고 데이터베이스에 접근하는 독립 모드(standalone)로 작업하기 위해 지정되며, 인수는 없다.
	**-S** 옵션을 지정하지 않으면, 시스템은 클라이언트/서버 모드로 인식한다. ::

		cubrid checkdb -S testdb


.. option:: -C, --CS-mode
	서버 프로세스와 클라이언트 프로세스를 각각 구동하여 데이터베이스에 접근하는 클라이언트/서버 모드로 작업하기 위한 옵션이며, 인수는 없다.
	**-C** 옵션을 지정하지 않더라도 시스템은 기본적으로 클라이언트/서버 모드로 인식한다. ::

		cubrid checkdb -C testdb

.. option:: -r, --repair

	데이터베이스의 일관성에 문제가 발견되었을 때 복구를 수행한다. ::

		cubrid checkdb -r testdb

.. option:: -i, --input-class-file=FILE

	**-i** *FILE* 옵션을 지정하거나, 데이터베이스 이름 뒤에 테이블의 이름을 나열하여 일관성 확인 또는 복구 대상을 한정할 수 있다. 두 가지 방법을 같이 사용할 수도 있으며, 대상을 지정하지 않으면 전체 데이터베이스를 대상으로 일관성을 확인하거나 복구를 수행한다. 특정 대상이 지정되지 않으면 전체 데이터베이스가 일관성 확인  또는 복구의 대상이 된다. ::

		cubrid checkdb testdb tbl1 tbl2
		cubrid checkdb -r testdb tbl1 tbl2
		cubrid checkdb -r -i table_list.txt testdb tbl1 tbl2

	**-i** 옵션으로 지정하는 테이블 목록 파일은 공백, 탭, 줄바꿈, 쉼표로 테이블 이름을 구분한다. 다음은 테이블 목록 파일의 예로, t1부터 t10까지를 모두 일관성 확인 또는 복구를 위한 테이블로 인식한다. ::

		t1 t2 t3,t4 t5
		t6, t7 t8   t9
		 
			 t10

.. _killtran:

데이터베이스 트랜잭션 제거
==========================

**cubrid killtran** 은 대상 데이터베이스의 트랜잭션을 확인하거나 특정 트랜잭션을 강제 종료하는 유틸리티로서, **DBA** 사용자만 수행할 수 있다. ::

	cubrid killtran [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **killtran**: 지정된 데이터베이스에 대해 트랜잭션을 관리하는 명령어이다.

*   *database_name*: 대상 데이터베이스의 이름이다.

[options]에 따라 특정 트랜잭션을 지정하여 제거하거나, 현재 활성화된 트랜잭션을 화면 출력할 수 있다. 옵션이 지정되지 않으면, **-d** 옵션이 기본으로 적용되어 모든 트랜잭션을 화면 출력한다.

::

	cubrid killtran testdb 
	 
	Tran index      User name   Host name      Process id      Program name
	-------------------------------------------------------------------------------
		  1(+)            dba      myhost             664           cub_cas
		  2(+)            dba      myhost            6700              csql
		  3(+)            dba      myhost            2188           cub_cas
		  4(+)            dba      myhost             696              csql
		  5(+)         public      myhost            6944              csql
	-------------------------------------------------------------------------------


다음은 **cubrid killtran** 에 대한 [options]이다.

.. program:: killtran

.. option:: -i, --kill-transation-index=INDEX

	지정한 인덱스에 해당하는 트랜잭션을 제거한다.  ::

		cubrid killtran -i 1 testdb
		 
		Ready to kill the following transactions:
		 
		Tran index      User name      Host name      Process id      Program name
		-------------------------------------------------------------------------------
			  1(+)            dba      myhost            4760              csql
		-------------------------------------------------------------------------------
		Do you wish to proceed ? (Y/N)y
		Killing transaction associated with transaction index 1

.. option:: --kill-user-name=ID

	지정한 OS 사용자 ID에 해당하는 트랜잭션을 제거한다. ::

		cubrid killtran --kill-user-name=os_user_id testdb

.. option:: --kill-host-name=HOST
	
	지정한 클라이언트 호스트의 트랜잭션을 제거한다. ::

		cubrid killtran --kill-host-name=myhost testdb

.. option:: --kill-program-name=NAME
		
	지정한 이름의 프로그램에 해당하는 트랜잭션을 제거한다. ::

		cubrid killtran --kill-program-name=cub_cas testdb

.. option:: -p PASSWORD

	**-p** 옵션 뒤에 오는 값은 **DBA** 의 암호이며 생략하면 프롬프트에서 입력해야 한다.

.. option:: -d, --display

	기본 지정되는 옵션으로 모든 트랜잭션의 정보를 출력한다. 
		
	::
	
		cubrid killtran -d testdb
		 
		Tran index      User name      Host name      Process id      Program name
		-------------------------------------------------------------------------------
			  2(+)            dba      myhost            6700              csql
			  3(+)            dba      myhost            2188           cub_cas
			  4(+)            dba      myhost             696              csql
			  5(+)         public      myhost            6944              csql
		-------------------------------------------------------------------------------

.. option:: -q, --query-exec-info

	트랜잭션의 질의 수행 상태를 출력한다. 상태 정보를 출력하는 예는 다음과 같다.

	::

		cubrid killtran --query-exec-info testdb
		 
		Tran index Process id Program name Query time Tran time  Wait for lock holder   SQL Text
		---------------------------------------------------------------------------------------------
			  1(+)       8536    b1_cub_cas_1    0.00      0.00  -1                     *** empty ***
			  2(+)       8538    b1_cub_cas_3    0.00      0.00  -1                     *** empty ***
			  3(+)       8537    b1_cub_cas_2    0.00      0.00  -1                     *** empty ***
			  4(+)       8543    b1_cub_cas_4    1.80      1.80  3, 2, 1                update [ta] [ta] set [a]=5 wher
			  5(+)       8264    b1_cub_cas_5    0.00      0.60  -1                     *** empty ***
			  6(+)       8307    b1_cub_cas_6    0.00      0.00  -1                     select [a].[index_name], ( cast
			  7(+)       8308    b1_cub_cas_7    0.00      0.20  -1                     select [a].[index_name], ( cast
			  .....
		 
		---------------------------------------------------------------------------------------------

	* Tran index : 트랜잭션 인덱스
	* Process id : 클라이언트 프로세스 ID
	* Program name : 클라이언트 프로그램 이름
	* Query time : 수행중인 질의의 총 수행 시간(단위: 초)
	* Tran time : 현재 트랜잭션의 총 수행 시간(단위: 초)
	* Wait for lock holder : 현재 트랜잭션이 락 대기중이면 해당 락을 소유하고 있는 트랜잭션의 리스트
	* SQL Text : 수행중인 질의문(최대 30자)

	위와 같이 트랜잭션 전체 정보가 출력된 후, 잠금 대기를 유발한 질의문이 다음과 같이 출력된다.

	::

		Tran index : 4
		update [ta] [ta] set [a]=5 where (([ta].[a]> ?:0 ))
		Tran index : 5, 6, 7
		select [a].[index_name], ( cast(case when [a].[is_unique]=0 then 'NO' else 'YES' end as varchar(3))), ( cast(case when [a].[is_reverse]=0 then 'NO' else 'YES' end as varchar(3))), [a].[class_of].[class_name], [a].[key_count], ( cast(case when [a].[is_primary_key]=0 then 'NO' else 'YES' end as varchar(3))), ( cast(case when [a].[is_foreign_key]=0 then 'NO' else 'YES' end as varchar(3))), [b].[index_name], ( cast(case when [b].[is_unique]=0 then 'NO' else 'YES' end as varchar(3))), ( cast(case when [b].[is_reverse]=0 then 'NO' else 'YES' end as varchar(3))), [b].[class_of].[class_name], [b].[key_count], ( cast(case when [b].[is_primary_key]=0 then 'NO' else 'YES' end as varchar(3))), ( cast(case when [b].[is_foreign_key]=0 then 'NO' else 'YES' end as varchar(3))) from [_db_index] [a], [_db_index] [b] where (( CURRENT_USER ='DBA' or {[a].[class_of].[owner].[name]} subseteq (select set{ CURRENT_USER }+coalesce(sum(set{[t].[g].[name]}), set{}) from [db_user] [u], table([u].[groups]) [t] ([g]) where ([u].[name]= CURRENT_USER )) or {[a].[class_of]} subseteq (select sum(set{[au].[class_of]}) from [_db_auth] [au] where ({[name]} subseteq (select set{ CURRENT_USER }+coalesce(sum(set{[t].[g].[name]}), set{}) from [db_user] [u], table([u].[groups]) [t] ([g]) where ([u].[name]= CURRENT_USER )) and [au].[auth_type]= ?:0 ))) and ( CURRENT_USER ='DBA' or {[b].[class_of].[owner].[name]} subseteq (select set{ CURRENT_USER }+coalesce(sum(set{[t].[g].[name]}), set{}) from [db_user] [u], table([u].[groups]) [t] ([g]) where ([u].[name]= CURRENT_USER )) or {[b].[class_of]} subseteq (select sum(set{[au].[class_of]}) from [_db_auth] [au] where ({[name]} subseteq (select set{ CURRENT_USER }+coalesce(sum(set{[t].[g].[name]}), set{}) from [db_user] [u], table([u].[groups]) [t] ([g]) where ([u].[name]= CURRENT_USER )) and [au].[auth_type]= ?:1 ))))

	화면에 출력되는 질의문은 질의 계획 캐시에 저장되어 있는 것으로, 응용 프로그램에서 수행되는 INSERT 문이나 질의 계획이 캐시되지 않는 질의문은 출력되지 않는다. 또한 출력 형태도 질의 파싱이 완료된 후의 질의문이 출력되므로, 사용자가 입력한 질의문과는 다른 형태일 수 있다.

	예를 들어 사용자가 아래와 같은 질의문을 수행하면 ::

		UPDATE ta SET a=5 WHERE a > 0

	출력되는 질의문은 다음과 같다. ::

		update [ta] [ta] set [a]=5 where (([ta].[a]> ?:0 ))

.. option:: -f, --force

	중지할 트랜잭션을 확인하는 프롬프트를 생략한다. ::

		cubrid killtran -f -i 1 testdb

질의 수행 계획 캐시 확인
========================

**cubrid plandump** 유틸리티를 사용해서 서버에 저장(캐시)되어 있는 질의 수행 계획들의 정보를 출력할 수 있다. ::

	cubrid plandump [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **plandump**: 대상 데이터베이스에 대하여 현재 캐시에 저장되어 있는 질의 수행 계획을 출력하는 명령이다.

*   *database_name*: 데이터베이스 서버 캐시로부터 질의 수행 계획을 확인 또는 제거하고자 하는 데이터베이스 이름이다

옵션 없이 사용하면 캐시에 저장된 질의 수행 계획을 확인한다. ::

	cubrid plandump testdb

다음은 **cubrid plandump** 에 대한 [options]이다.

.. program:: plandump

.. option:: -d, --drop

	캐시에 저장된 질의 수행 계획을 제거한다. ::

		cubrid plandump -d testdb

.. option:: -o, --output-file=FILE
		
	캐시에 저장된 질의 수행 계획 결과 파일에 저장 ::

		cubrid plandump -o output.txt testdb

데이터베이스 내부 정보 출력
===========================

**cubrid diagdb** 유틸리티를 이용해 다양한 데이터베이스 내부 정보를 확인할 수 있다. **cubrid diagdb** 유틸리티가 제공하는 정보들은 현재 데이터베이스의 상태를 진단하거나 문제를 파악하는데 도움이 된다.

::

	cubrid diagdb [option] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **diagdb**: CUBRID에 저장되는 바이너리 형태의 파일 정보를 텍스트 형태로 출력하여 현재의 데이터베이스 저장 상태를 확인하고자 할 때 사용하는 명령어이다. 데이터베이스가 구동 정지 상태인 경우에만 정상적으로 수행된다. 전체를 확인하거나 옵션을 사용하여 파일 테이블, 파일 용량, 힙 용량, 클래스 이름, 디스크 비트맵을 선택해 확인할 수 있다.

*   *database_name*: 내부 정보를 확인하려는 데이터베이스 이름이다.

다음은 **cubrid diagdb** 에서 사용하는 [option]이다.

.. program:: diagdb

.. option:: -d, --dump-type=TYPE

	testdb라는 데이터베이스의 전체 파일에 대한 기록 상태를 출력할 때 출력 범위를 지정한다. 생략하면 기본값인 1이 지정된다. ::

		cubrid diagdb -d 1 myhost testdb

	**-d** 옵션에 적용되는 타입은 모두 9가지로, 그 종류는 다음과 같다.

	+--------+------------------------------------+
	| 타입   | 설명                               |
	+========+====================================+
	| -1     | 전체 데이터베이스 정보를 출력한다. |
	+--------+------------------------------------+
	| 1      | 파일 테이블 정보를 출력한다.       |
	+--------+------------------------------------+
	| 2      | 파일 용량 정보를 출력한다.         |
	+--------+------------------------------------+
	| 3      | 힙 용량 정보를 출력한다.           |
	+--------+------------------------------------+
	| 4      | 인덱스 용량 정보를 출력한다.       |
	+--------+------------------------------------+
	| 5      | 클래스 이름 정보를 출력한다.       |
	+--------+------------------------------------+
	| 6      | 디스크 비트맵 정보를출력한다.      |
	+--------+------------------------------------+
	| 7      | 카탈로그 정보를 출력한다.          |
	+--------+------------------------------------+
	| 8      | 로그 정보를 출력한다.              |
	+--------+------------------------------------+
	| 9      | 힙(heap) 정보를 출력한다.          |
	+--------+------------------------------------+

백업 및 복구
============

데이터베이스 관리자(**DBA**)는 시스템의 장애에 대비하여 데이터베이스를 일정 시점의 데이터베이스로 복구할 수 있도록 주기적으로 백업을 수행해야 한다. 상세한 내용은 :ref:`db-backup` 을 참조한다.

내보내기와 가져오기
===================

신규 버전의 CUBRID 데이터베이스를 사용하기 위해서는 기존 버전의 CUBRID 데이터베이스를 신규 버전의 CUBRID 데이터베이스로 이전하는 작업을 진행해야 할 경우가 있다. 이때 CUBRID에서 제공하는 텍스트 파일로 내보내기와 텍스트 파일에서 가져오기 기능을 활용할 수 있다. 내보내기와 가져오기에 대한 보다 자세한 설명은 :doc:`/admin/migration` 을 참고한다.

서버/클라이언트에서 사용하는 파라미터 출력
==========================================

**cubrid paramdump** 유틸리티는 서버/클라이언트 프로세스에서 사용하는 파라미터 정보를 출력한다. ::

	cubrid paramdump [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **paramdump**: 서버/클라이언트 프로세스에서 사용하는 파라미터 정보를 출력하는 명령이다.

*   *database_name*: 파라미터 정보를 출력할 데이터베이스 이름이다.

다음은 **cubrid paramdump** 에서 사용하는 [options]이다.

.. program:: paramdump

.. option:: -o --output-file=FILE

	데이터베이스의 서버/클라이언트 프로세스에서 사용하는 파라미터 정보를 지정된 파일에 저장하는 옵션이며, 파일은 현재 디렉터리에 생성된다.
	**-o** 옵션이 지정되지 않으면 메시지는 콘솔 화면에 출력한다. ::

		cubrid paramdump -o db_output testdb

.. option:: -b, --both

	데이터베이스의 서버/클라이언트 프로세스에서 사용하는 파라미터 정보를 콘솔 화면에 출력하는 옵션이며, **-b** 옵션을 사용하지 않으면 서버 프로세스의 파라미터 정보만 출력한다. ::

		cubrid paramdump -b testdb

.. option:: -S, --SA-mode

	독립 모드에서 서버 프로세스의 파라미터 정보를 출력한다. ::

		cubrid paramdump -S testdb

.. option:: -C, --CS-mode

	클라이언트-서버 모드에서 서버 프로세스의 파라미터 정보를 출력한다. ::

		cubrid paramdump -C testdb

로캘 컴파일/출력
================

**cubrid genlocale** 유틸리티는 사용하고자 하는 로캘(locale) 정보를 컴파일하는 유틸리티이다. 이 유틸리티는 **make_locale.sh** (Windows는 **.bat**) 스크립트 내에서 실행된다.

**cubrid dumplocale** 유틸리티는 컴파일된 바이너리 로캘 파일을 사람이 읽을 수 있는 형태로 콘솔에 출력한다. 출력 값이 매우 클 수 있으므로, 리다이렉션을 이용하여 특정 파일로 저장할 것을 권장한다.

자세한 사용법은 :ref:`locale-setting` 을 참고한다.