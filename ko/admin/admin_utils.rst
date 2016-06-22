.. _cubrid-utilities:

cubrid 유틸리티
===============

cubrid 유틸리티의 사용법(구문)은 다음과 같다. ::

    cubrid utility_name
    utility_name :
        createdb [option] <database_name> <locale_name> --- 데이터베이스 생성
        deletedb [option] <database_name>   --- 데이터베이스 삭제
        installdb [option] <database-name>   --- 데이터베이스 설치
        renamedb [option] <source-database-name> <target-database-name>  --- 데이터베이스 이름 변경
        copydb [option] <source-database-name> <target-database-name>  --- 데이터베이스 복사
        backupdb [option] <database-name>  --- 데이터베이스 백업
        restoredb [option] <database-name>  --- 데이터베이스 복구
        addvoldb [option] <database-name>  --- 데이터베이스 볼륨 파일 추가
        spacedb [option] <database-name>  --- 데이터베이스 공간 정보 출력
        lockdb [option] <database-name>  --- 데이터베이스의 lock 정보 출력
        tranlist [option] <database-name>  --- 트랜잭션 확인
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
        applyinfo [option] <database-name>   --- HA 환경에서 트랜잭션 로그 반영 정보를 확인하는 도구
        synccolldb [option] <database-name>  --- DB 콜레이션을 시스템 콜레이션에 맞게 변경하는 도구
        genlocale [option] <database-name>  --- 사용하고자 하는 로캘 정보를 컴파일하는 도구
        dumplocale [option] <database-name>   --- 컴파일된 바이너리 로캘 정보를 사람이 읽을 수 있는 텍스트로 출력하는 도구
        gen_tz [option] [<database-name>]  --- 공유 라이브러리로 컴파일된 타임존 데이타를 포함한 C 소스 파일을 생성한다.
        dump_tz [option]  --- 타임존 관련 정보를 표시한다.

cubrid 유틸리티 로깅
--------------------
 
CUBRID는 cubrid 유틸리티의 수행 결과에 대한 로깅 기능을 제공하며, 자세한 내용은 :ref:`cubrid-utility-logging`\ 을 참고한다.

.. _creating-database:

.. _createdb:

createdb
--------

**cubrid createdb** 유틸리티는 CUBRID 시스템에서 사용할 데이터베이스를 생성하고 미리 만들어진 CUBRID 시스템 테이블을 초기화한다. 데이터베이스에 권한이 주어진 초기 사용자를 정의할 수도 있다. 일반적으로 데이터베이스 관리자만이 **cubrid createdb** 유틸리티를 사용한다. 로그와 데이터베이스의 위치도 지정할 수 있다. 

.. warning::

    데이터베이스를 생성할 때 데이터베이스 이름 뒤에 로캘 이름과 문자셋(예: ko_KR.utf8)을 반드시 지정해야 한다. 문자셋에 따라 문자열 타입의 크기, 문자열 비교 연산 등에 영향을 끼친다. 데이터베이스 생성 시 지정된 문자셋은 변경할 수 없으므로 지정에 주의해야 한다.
    
    문자셋, 로캘 및 콜레이션 설정과 관련된 자세한 내용은 :doc:`/sql/i18n`\ 을 참고한다.

::

    cubrid createdb [options] database_name locale_name.charset

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **createdb**: 새로운 데이터베이스를 생성하기 위한 명령이다.

*   *database_name*: 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않고, 생성하고자 하는 데이터베이스의 이름을 고유하게 부여한다. 이 때, 지정한 데이터베이스 이름이 이미 존재하는 데이터베이스 이름과 중복되는 경우, CUBRID는 기존 파일을 보호하기 위하여 데이터베이스 생성을 더 이상 진행하지 않는다.

*   *locale_name*: 데이터베이스에서 사용할 로캘 이름을 입력한다. CUBRID에서 사용 가능한 로캘 이름은 :ref:`locale-selection`\ 을 참고한다.

*   *charset*: 데이터베이스에서 사용할 문자셋을 입력한다. CUBRID에서 사용 가능한 문자셋은 iso88591, euckr, utf8이다. 
    
    *   *locale_name*\ 이 en_US이고 *charset*\ 을 생략하면 문자셋은 iso88591이 된다.
    *   *locale_name*\ 이 ko_KR이고 *charset*\ 을 생략하면 문자셋은 utf8이 된다.
    *   나머지 *locale_name*\ 은 *charset*\ 을 생략할 수 없으며, utf8만 지정 가능하다.

데이터베이스 이름의 최대 길이는 영문 17자이다.

다음은 **cubrid createdb**\에 대한 [options]이다.

.. program:: createdb

.. option:: --db-volume-size=SIZE

    데이터베이스를 생성할 때 첫 번째 데이터베이스 볼륨의 크기를 지정하는 옵션으로, 기본값은 cubrid.conf에 지정된 시스템 파라미터 **db_volume_size**\ 의 값이다. 최소값은 20M이다. K, M, G, T로 단위를 설정할 수 있으며, 각각 KB(kilobytes), MB(megabytes), GB(gigabytes), TB(terabytes)를 의미한다. 단위를 생략하면 바이트 단위가 적용된다.

    다음은 첫 번째로 생성되는 testdb의 볼륨 크기를 512MB로 지정하는 구문이다. ::

        cubrid createdb --db-volume-size=512M testdb en_US

.. option:: --db-page-size=SIZE

    데이터베이스 페이지 크기를 지정하는 옵션으로서, 최소값은 4K, 최대값은 16K(기본값)이다. K는 KB(kilobytes)를 의미한다. 데이터베이스 페이지 크기는 4K, 8K, 16K 중 하나의 값이 된다. 4K와 16K 사이의 값을 지정할 경우 지정한 값의 올림값으로 설정되며, 4K보다 작으면 4K로 설정되고 16K보다 크면 16K로 설정된다.

    다음은 testdb를 생성하고, testdb의 데이터베이스 페이지 크기를 16K로 지정하는 구문이다. ::

        cubrid createdb --db-page-size=16K testdb en_US

.. option:: --log-volume-size=SIZE 

    생성되는 데이터베이스의 로그 볼륨 크기를 지정하는 옵션으로, 기본값은 데이터베이스 볼륨 크기와 같으며 최소값은 20M이다. K, M, G, T로 단위를 설정할 수 있으며, 각각 KB(kilobytes), MB(megabytes), GB(gigabytes), TB(terabytes)를 의미한다. 단위를 생략하면 바이트 단위가 적용된다.

    다음은 *testdb*\를 생성하고, *testdb*\의 로그 볼륨 크기를 256M로 지정하는 구문이다. ::

        cubrid createdb --log-volume-size=256M testdb en_US

.. option:: --log-page-size=SIZE

    생성되는 데이터베이스의 로그 볼륨 페이지 크기를 지정하는 옵션으로, 기본값은 데이터 페이지 크기와 같다. 최소값은 4K, 최대값은 16K이다. K는 KB(kilobytes)를 의미한다.
    데이터베이스 페이지 크기는 4K, 8K, 16K 중 하나의 값이 된다. 4K와 16K 사이의 값을 지정할 경우 지정한 값의 올림값으로 설정되며, 4K보다 작으면 4K로 설정되고 16K보다 크면 16K로 설정된다.

    다음은 *testdb*\를 생성하고, *testdb*\ 의 로그 볼륨 페이지 크기를 8kbyte로 지정하는 구문이다. ::

        cubrid createdb -log-page-size=8K testdb en_US

.. option:: --comment=COMMENT

    데이터베이스의 볼륨 헤더에 지정된 주석을 포함하는 옵션으로, 문자열에 공백이 포함되면 큰 따옴표로 감싸주어야 한다.

    다음은 *testdb*\를 생성하고, 데이터베이스 볼륨에 이에 대한 주석을 추가하는 구문이다. ::

        cubrid createdb --comment "a new database for study" testdb en_US

.. option:: -F, --file-path=PATH

    새로운 데이터베이스가 생성되는 디렉터리의 절대 경로를 지정하는 옵션으로, **-F** 옵션을 지정하지 않으면 현재 작업 디렉터리에 새로운 데이터베이스가 생성된다.

    다음은 *testdb*\라는 이름의 데이터베이스를 /dbtemp/new_db라는 디렉터리에 생성하는 구문이다. ::

        cubrid createdb -F "/dbtemp/new_db/" testdb en_US

.. option:: -L, --log-path=PATH

    데이터베이스의 로그 파일이 생성되는 디렉터리의 절대 경로를 지정하는 옵션으로, **-L** 옵션을 지정하지 않으면 **-F** 옵션에서 지정한 디렉터리에 생성된다.
    **-F** 옵션과 **-L** 옵션을 둘 다 지정하지 않으면 데이터베이스와 로그 파일이 현재 작업 디렉터리에 생성된다.

    다음은 *testdb*\라는 이름의 데이터베이스를 /dbtemp/newdb라는 디렉터리에 생성하고, 로그 파일을 /dbtemp/db_log 디렉터리에 생성하는 구문이다. ::

        cubrid createdb -F "/dbtemp/new_db/" -L "/dbtemp/db_log/" testdb en_US

.. option:: -B, --lob-base-path=PATH

    **BLOB/CLOB** 데이터를 사용하는 경우 **LOB** 데이터 파일이 저장되는 디렉터리의 경로를 지정하는 옵션으로, 이 옵션을 지정하지 않으면 <*데이터베이스 볼륨이 생성되는 디렉터리*>\ **/lob** 디렉터리에 **LOB** 데이터 파일이 저장된다.

    다음은 *testdb*\ 를 현재 작업 디렉터리에 생성하고, **LOB** 데이터 파일이 저장될 디렉터리를 로컬 파일 시스템의 "/home/data1"으로 지정하는 구문이다. ::

        cubrid createdb --lob-base-path "file:/home1/data1" testdb en_US

.. option:: --server-name=HOST

    CUBRID의 클라이언트/서버 버전을 사용할 때 특정 데이터베이스에 대한 서버가 지정한 호스트 상에 구동되도록 하는 옵션이다. 이 옵션으로 지정된 서버 호스트의 정보는 데이터베이스 위치 정보 파일( **databases.txt** )에 기록된다. 이 옵션이 지정되지 않으면 기본값은 현재 로컬 호스트이다.

    다음은 *testdb*\를 *aa_host* 호스트 상에 생성 및 등록하는 구문이다. ::

        cubrid createdb --server-name aa_host testdb en_US

.. option:: -r, --replace

    **-r**\은 지정된 데이터베이스 이름이 이미 존재하는 데이터베이스 이름과 중복되더라도 새로운 데이터베이스를 생성하고, 기존의 데이터베이스를 덮어쓰도록 하는 옵션이다.

    다음은 *testdb*\ 라는 이름의 데이터베이스가 이미 존재하더라도 기존의 *testdb*\ 를 덮어쓰고 새로운 *testdb*\ 를 생성하는 구문이다. ::

        cubrid createdb -r testdb en_US

.. option:: --more-volume-file=FILE

    데이터베이스가 생성되는 디렉터리에 추가 볼륨을 생성하는 옵션으로 지정된 파일에 저장된 명세에 따라 추가 볼륨을 생성한다. 이 옵션을 이용하지 않더라도, **cubrid addvoldb** 유틸리티를 이용하여 볼륨을 추가할 수 있다.

    다음은 *testdb*\를 생성함과 동시에 vol_info.txt에 저장된 명세를 기반으로 볼륨을 추가 생성하는 구문이다. ::

        cubrid createdb --more-volume-file vol_info.txt testdb en_US

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

    *   *volname*: 추가 생성될 볼륨의 이름으로 Unix 파일 이름 규약을 따라야 하고, 디렉터리 경로를 포함하지 않는 단순한 이름이어야 한다. 볼륨명에 관한 명세는 생략할 수 있으며, 이 경우 시스템에 의해 "생성될 데이터베이스 이름_볼륨 식별자"로 볼륨명이 생성된다.

    *   *volcmnts*: 볼륨 헤더에 기록되는 주석 문장으로, 추가 생성되는 볼륨에 관한 정보를 임의로 부여할 수 있다. 볼륨 주석에 관한 명세 역시 생략할 수 있다.

    *   *volpurp*: 볼륨 저장의 목적으로, **data**, **index**, **temp**, **generic** 중 하나여야 한다. 볼륨 목적에 관한 명세는 생략할 수 있으며, 이 경우 기본값은 **generic**\이다.

    *   *volnpgs*: 추가 생성되는 볼륨의 페이지 수이다. 볼륨 페이지 수에 관한 명세는 생략할 수 없으며, 반드시 지정해야 한다.

.. option:: --user-definition-file=FILE

    생성하고자 하는 데이터베이스에 대해 권한이 있는 사용자를 추가하는 옵션으로, 파라미터로 지정된 사용자 정보 파일에 저장된 명세에 따라 사용자를 추가한다.
    **--user-definition-file** 옵션을 이용하지 않더라도 :ref:`create-user` 구문을 이용하여 사용자를 추가할 수 있다.

    다음은 *testdb*\를 생성함과 동시에 user_info.txt에 정의된 사용자 정보를 기반으로 *testdb*\에 대한 사용자를 추가하는 구문이다. ::

        cubrid createdb --user-definition-file=user_info.txt testdb en_US

    사용자 정보 파일의 구문은 아래와 같다. ::

        USER user_name [ <groups_clause> | <members_clause> ]
        
        <groups_clause>: 
            [ GROUPS <group_name> [ { <group_name> }... ] ]

        <members_clause>: 
            [ MEMBERS <member_name> [ { <member_name> }... ] ]

    *   *user_name*: 데이터베이스에 대해 권한을 가지는 사용자 이름이며, 공백이 포함되지 않아야 한다.

    *   **GROUPS** 절: 옵션이며, <group_name>은 지정된 <user_name>을 포함하는 상위 그룹의 이름이다. 이 때, <group_name>은 하나 이상이 지정될 수 있으며, **USER**\로 미리 정의되어야 한다.

    *   **MEMBERS** 절: 옵션이며, <member_name> 은 지정된 <user_name>에 포함되는 하위 멤버의 이름이다. 이 때, <member_name>은 하나 이상이 지정될 수 있으며, **USER**\로 미리 정의되어야 한다.

    사용자 정보 파일에서는 주석을 사용할 수 있으며, 주석 라인은 연속된 하이픈(--)으로 시작된다. 공백 라인은 무시된다.

    다음 예제는 그룹 *sedan*\에 *grandeur*\와 *sonata*\가, 그룹 *suv*\에 *tuscan*\이, 그룹 *hatchback*\에 *i30*\가 포함되는 것을 정의하는 사용자 정보 파일이다. 사용자 정보 파일명은 user_info.txt로 예시한다. ::

        --
        --  사용자 정보 파일의 예 1
        --
        USER sedan
        USER suv
        USER hatchback
        USER grandeur GROUPS sedan
        USER sonata GROUPS sedan
        USER tuscan GROUPS suv
        USER i30 GROUPS hatchback

    위 예제와 동일한 사용자 관계를 정의하는 파일이다. 다만, 아래 예제에서는 **MEMBERS**\ 절을 이용하였다. ::

        --
        -- 사용자 정보 파일의 예 2
        --
        USER grandeur
        USER sonata
        USER tuscan
        USER i30
        USER sedan MEMBERS sonata grandeur
        USER suv MEMBERS tuscan
        USER hatchback MEMBERS i30
        
.. option::    --csql-initialization-file=FILE

    생성하고자 하는 데이터베이스에 대해 CSQL 인터프리터에서 구문을 실행하는 옵션으로, 파라미터로 지정된 파일에 저장된 SQL 구문에 따라 스키마를 생성할 수 있다.

    다음은 *testdb*\를 생성함과 동시에 table_schema.sql에 정의된 SQL 구문을 CSQL 인터프리터에서 실행시키는 구문이다. ::

        cubrid createdb --csql-initialization-file table_schema.sql testdb en_US

.. option:: -o, --output-file=FILE

    데이터베이스 생성에 관한 메시지를 파라미터로 지정된 파일에 저장하는 옵션이며, 파일은 데이터베이스와 동일한 디렉터리에 생성된다.
    **-o** 옵션이 지정되지 않으면 메시지는 콘솔 화면에 출력된다. **-o** 옵션은 데이터베이스가 생성되는 중에 출력되는 메시지를 지정된 파일에 저장함으로써 특정 데이터베이스의 생성 과정에 관한 정보를 활용할 수 있게 한다.

    다음은 *testdb*\를 생성하면서 이에 관한 유틸리티의 출력을 콘솔 화면이 아닌 db_output 파일에 저장하는 구문이다. ::

        cubrid createdb -o db_output testdb en_US

.. option::  -v, --verbose

    데이터베이스 생성 연산에 관한 모든 정보를 화면에 출력하는 옵션으로서, **-o** 옵션과 마찬가지로 특정 데이터베이스 생성 과정에 관한 정보를 확인하는데 유용하다. 따라서, **-v** 옵션과 **-o** 옵션을 함께 지정하면, **-o** 옵션의 파라미터로 지정된 출력 파일에 **cubrid createdb** 유틸리티의 연산 정보와 생성 과정에 관한 출력 메시지를 저장할 수 있다.

    다음은 *testdb*\를 생성하면서 이에 관한 상세한 연산 정보를 화면에 출력하는 구문이다. ::

        cubrid createdb -v testdb en_US

.. note::

    *  **temp_file_max_size_in_pages**\는 복잡한 질의문이나 정렬 수행에 사용되는 일시적 임시 볼륨(temporary temp volume)을 디스크에 저장하는 데에 할당되는 페이지의 최대 개수를 설정하는 파라미터이다. 기본값은 **-1**\로, **temp_volume_path** 파라미터가 지정한 디스크의 여유 공간까지 일시적 임시 볼륨(temporary temp volume)이 커질 수 있다. 0이면 일시적 임시 볼륨이 생성되지 않으므로 :ref:`cubrid addvoldb <adding-database-volume>` 유틸리티를 이용하여 영구적 임시 볼륨(permanent temp volume)을 충분히 추가해야 한다. 볼륨을 효율적으로 관리하려면 용도별로 볼륨을 추가하는 것을 권장한다.
    
    *  :ref:`cubrid spacedb <spacedb>` 유틸리티를 사용하여 각 용도별 볼륨의 남은 공간을 검사할 수 있으며, :ref:`cubrid addvoldb <adding-database-volume>` 유틸리티를 사용하여 데이터베이스 운영 중에도 필요한 만큼 볼륨을 추가할 수 있다. 데이터베이스 운영 중에 볼륨을 추가하려면 가급적 시스템 부하가 적은 상태에서 추가할 것을 권장한다. 해당 용도의 볼륨 공간이 모두 사용되면 범용(**generic**) 볼륨이 생성되므로 여유 공간이 부족할 것으로 예상되는 용도의 볼륨을 미리 추가해 놓을 것을 권장한다.

다음은 데이터베이스를 생성하고 볼륨 용도를 구분하여 데이터(**data**), 인덱스(**index**), 임시(**temp**) 볼륨을 추가하는 예이다. ::

    cubrid createdb --db-volume-size=512M --log-volume-size=256M cubriddb en_US
    cubrid addvoldb -S -p data -n cubriddb_DATA01 --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p data -n cubriddb_DATA02 --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p index -n cubriddb_INDEX01 cubriddb --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p temp -n cubriddb_TEMP01 cubriddb --db-volume-size=512M cubriddb

.. _adding-database-volume:    

.. _addvoldb:

addvoldb
--------

전체 **generic** 볼륨의 여유 공간이 :ref:`disk-parameters`\ 에 속한 **generic_vol_prealloc_size** 파라미터에서 지정한 크기(기본값: 50M)보다 작아지면 자동으로 **generic** 볼륨이 추가된다. 볼륨 자동 추가는 새로운 페이지 할당 요청이 있을 때 이루어지며, SELECT만 수행되는 경우 볼륨이 확장되지 않는다.

CUBRID의 볼륨은 데이터 저장, 인덱스 저장, 임시 결과 저장 등 용도에 따라 구분되는데, **generic** 볼륨은 데이터 및 인덱스 저장 용도로 사용될 수 있다.

각 볼륨의 종류(용도)에 대해서는 :ref:`database-volume-structure`\ 를 참고한다.

이에 비해, 사용자에 의해 수동으로 데이터베이스 볼륨을 추가하는 명령은 다음과 같다.

::

    cubrid addvoldb [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **addvoldb**: 지정된 데이터베이스에 지정된 페이지 수만큼 새로운 볼륨을 추가하기 위한 명령이다.

*   *database_name*: 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않고, 볼륨을 추가하고자 하는 데이터베이스의 이름을 지정한다.

다음은 데이터베이스를 생성하고 볼륨 용도를 구분하여 데이터(**data**), 인덱스(**index**), 임시(**temp**) 볼륨을 추가하는 예이다. ::

    cubrid createdb --db-volume-size=512M --log-volume-size=256M cubriddb en_US
    cubrid addvoldb -S -p data -n cubriddb_DATA01 --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p data -n cubriddb_DATA02 --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p index -n cubriddb_INDEX01 cubriddb --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p temp -n cubriddb_TEMP01 cubriddb --db-volume-size=512M cubriddb

다음은 cubrid addvoldb에 대한 [options]이다.

.. program:: addvoldb

.. option:: --db-volume-size=SIZE

    추가되는 데이터베이스 볼륨의 크기를 지정하는 옵션으로, 기본값은 **cubrid.conf**\에 지정된 시스템 파라미터 **db_volume_size**\ 의 값이다. K, M, G, T로 단위를 설정할 수 있으며, 각각 KB(kilobytes), MB(megabytes), GB(gigabytes), TB(terabytes)를 의미한다. 단위를 생략하면 바이트 단위가 적용된다.

    다음은 *testdb*\에 데이터 볼륨을 추가하며 볼륨 크기를 256MB로 지정하는 구문이다. ::

        cubrid addvoldb -p data --db-volume-size=256M testdb

.. option:: -n, --volume-name=NAME

    지정된 데이터베이스에 대하여 추가될 볼륨의 이름을 지정하는 옵션이다. 볼륨명은 운영체제의 파일 이름 규약을 따라야 하고, 디렉터리 경로나 공백을 포함하지 않는 단순한 이름이어야 한다.
    **-n** 옵션을 생략하면 추가되는 볼륨의 이름은 시스템에 의해 "데이터베이스 이름_볼륨 식별자"로 자동 부여된다. 예를 들어, 데이터베이스 이름이 *testdb*\ 이면 자동 부여된 볼륨명은 *testdb_x001*\ 이 된다.

    다음은 독립모드(standalone) 상태에서 *testdb*\ 라는 데이터베이스에 256MB 볼륨을 추가하는 구문이며, 생성되는 볼륨명은 *testdb_v1*\ 이 된다. ::

        cubrid addvoldb -S -n testdb_v1 --db-volume-size=256M testdb

.. option::  -F, --file-path=PATH

    지정된 데이터베이스에 대하여 추가될 볼륨이 저장되는 디렉터리 경로를 지정하는 옵션이다. **-F** 옵션을 생략하면, 시스템 파라미터인 **volume_extension_path**\ 의 값이 기본값으로 사용된다.

    다음은 독립모드(standalone) 상태에서 *testdb*\ 라는 데이터베이스에 256MB 볼륨을 추가하는 구문이며, 추가 볼륨은 /dbtemp/addvol 디렉터리에 생성된다. 볼륨명에 관한 **-n** 옵션을 지정하지 않았으므로, 생성되는 볼륨명은 *testdb_x001*\이 된다. ::

        cubrid addvoldb -S -F /dbtemp/addvol/ --db-volume-size=256M testdb

.. option:: --comment=COMMENT

    추가된 볼륨에 관한 정보 검색을 쉽게 하기 위하여 볼륨에 관한 정보를 주석으로 처리하는 옵션이다. 이때 주석의 내용은 볼륨을 추가하는 **DBA**\ 의 이름이나 볼륨 추가의 목적을 포함하는 것이 바람직하며, 큰따옴표로 감싸야 한다.
    
    다음은 독립모드(standalone) 상태에서 *testdb*\ 라는 데이터베이스에 256MB 볼륨을 추가하는 구문이며, 해당 볼륨에 관한 정보를 주석으로 남긴다. ::

        cubrid addvoldb -S --comment "데이터 볼륨 추가_김철수" --db-volume-size=256M testdb

.. option:: -p, --purpose=PURPOSE

    추가할 볼륨의 사용 목적에 따라 볼륨의 종류를 지정하는 옵션이다. 이처럼 볼륨의 사용 목적에 맞는 볼륨을 지정해야 볼륨 종류별로 디스크 드라이브에 분리 저장할 수 있어 I/O 성능을 높일 수 있다.
    **-p** 옵션의 파라미터로 가능한 값은 **data**, **index**, **temp**,    **generic** 중 하나이며, 기본값은 **generic**\이다. 각 볼륨 용도에 관해서는 :ref:`database-volume-structure` 를 참조한다.

    다음은 독립모드(standalone) 상태에서 *testdb*\라는 데이터베이스에 256MB 인덱스 볼륨을 추가하는 구문이다. ::

        cubrid addvoldb -S -p index --db-volume-size=256M testdb

.. option:: -S, --SA-mode

    서버 프로세스를 구동하지 않고 데이터베이스에 접근하는 독립 모드(standalone)로 작업하기 위해 지정되며, 인수는 없다. **-S** 옵션을 지정하지 않으면, 시스템은 클라이언트/서버 모드로 인식한다. ::

        cubrid addvoldb -S --db-volume-size=256M testdb

.. option:: -C, --CS-mode

    서버 프로세스와 클라이언트 프로세스를 각각 구동하여 데이터베이스에 접근하는 클라이언트/서버 모드로 작업하기 위한 옵션이며, 인수는 없다. **-C** 옵션을 지정하지 않더라도 시스템은 기본적으로 클라이언트/서버 모드로 인식한다. ::

        cubrid addvoldb -C --db-volume-size=256M testdb

.. option:: --max_writesize-in-sec=SIZE

    데이터베이스에 볼륨을 추가할 때 디스크 출력량을 제한하여 시스템 운영 영향을 줄이도록 하는 옵션이다. 이 옵션을 통해 1초당 쓸 수 있는 최대 크기를 지정할 수 있으며, 단위는 K(kilobytes), M(megabytes)이다. 최소값은 160K이며, 이보다 작게 값을 설정하면 160K로 바뀐다. 단, 클라이언트/서버 모드(-C)에서만 사용 가능하다.
    
    다음은 2GB 볼륨을 초당 1MB씩 쓰도록 하는 예이다. 소요 시간은 35분( = (2048MB / 1MB)  / 60초 )  정도가 예상된다. ::
    
        cubrid addvoldb -C --db-volume-size=2G --max-writesize-in-sec=1M testdb

.. _deleting-database:

.. _deletedb:

deletedb
--------

**cubrid deletedb**\는 데이터베이스를 삭제하는 유틸리티이다. 데이터베이스가 몇 개의 상호 의존적 파일들로 만들어지기 때문에, 데이터베이스를 제거하기 위해 운영체제 파일 삭제 명령이 아닌 **cubrid deletedb** 유틸리티를 사용해야 한다.

**cubrid deletedb** 유틸리티는 데이터베이스 위치 파일( **databases.txt** )에 지정된 데이터베이스에 대한 정보도 같이 삭제한다. **cubrid deletedb** 유틸리티는 오프라인 상에서 즉, 아무도 데이터베이스를 사용하지 않는 상태에서 독립 모드로 사용해야 한다. ::

    cubrid deletedb [options] database_name 

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **deletedb**: 데이터베이스 및 관련 데이터, 로그, 백업 파일을 전부 삭제하기 위한 명령으로, 데이터베이스 서버가 구동 정지 상태인 경우에만 정상적으로 수행된다.

*   *database_name*: 디렉터리 경로명을 포함하지 않고, 삭제하고자 하는 데이터베이스의 이름을 지정한다

다음은 **cubrid deletedb**\에 대한 [options]이다.

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

.. _renamedb:

renamedb
--------

**cubrid renamedb** 유틸리티는 존재하는 데이터베이스의 현재 이름을 변경한다. 정보 볼륨, 로그 볼륨, 제어 파일들이 새로운 이름과 일치되게 이름을 변경한다.

이에 비해 **cubrid alterdbhost** 유틸리티는 지정된 데이터베이스의 호스트 이름을 설정하거나 변경한다. 즉, **databases.txt**\에 있는 호스트 이름을 변경한다. ::

    cubrid renamedb [options] src_database_name dest_database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **renamedb**: 현재 존재하는 데이터베이스의 이름을 새로운 이름으로 변경하기 위한 명령으로, 데이터베이스가 구동 정지 상태인 경우에만 정상적으로 수행된다. 관련된 정보 볼륨, 로그 볼륨, 제어 파일도 함께 새로 지정된 이름으로 변경된다.

*   *src_database_name*: 이름을 바꾸고자 하는 현재 존재하는 데이터베이스의 이름이며, 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않는다.

*   *dest_database_name*: 새로 부여하고자 하는 데이터베이스의 이름이며, 현재 존재하는 데이터베이스 이름과 중복되어서는 안 된다. 이 역시, 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않는다.

다음은 **cubrid renamedb**\에 대한 [options]이다.

.. program:: renamedb

.. option:: -E, --extented-volume-path=PATH

    확장 볼륨의 이름을 변경한 후 새 디렉터리 경로로 이동하는 명령으로서, **-E** 옵션을 이용하여 변경된 이름을 가지는 확장 볼륨을 이동시킬 새로운 디렉터리 경로(예: /dbtemp/newaddvols/)를 지정한다.

    **-E** 옵션을 주지 않으면, 확장 볼륨은 기존 위치에서 이름만 변경된다. 이때, 기존 데이터베이스 볼륨의 디스크 파티션 외부에 있는 디렉터리 경로 또는 유효하지 않은 디렉터리 경로가 지정되는 경우 데이터베이스 이름 변경 작업은 수행되지 않으며, **-i** 옵션과 병행될 수 없다. ::

        cubrid renamedb -E /dbtemp/newaddvols/ testdb testdb_1

.. option::    -i, --control-file FILE

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

.. option::    -d, --delete-backup

    데이터베이스의 이름을 변경하면서 데이터베이스와 와 동일 위치에 있는 모든 백업 볼륨 및 백업 정보 파일을 함께 강제 삭제하는 명령이다. 일단, 데이터베이스 이름이 변경되면 이보다 앞선 이름의 백업 파일은 이용할 수 없으므로 주의해야 한다. 만약, **-d** 옵션을 지정하지 않으면 백업 볼륨 및 백업 정보 파일은 삭제되지 않는다. ::

        cubrid renamedb -d testdb testdb_1

.. _alterdbhost:

alterdbhost
-----------

**cubrid alterdbhost** 유틸리티는 지정된 데이터베이스의 호스트 이름을 설정하거나 변경한다. 즉, **databases.txt** 에 있는 호스트 이름을 변경한다.

    cubrid alterdbhost [<option>] database_name 

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **alterdbhost**: 현 데이터베이스의 호스트 이름을 새로운 이름으로 변경하기 위한 명령이다.

**cubrid alterdbhost**\에서 사용하는 옵션은 다음과 같다.

.. program:: alterdbhost

.. option:: -h, --host=HOST

    뒤에 변경할 호스트 이름을 지정하며, 옵션을 생략하면 호스트 이름으로 localhost를 지정한다.

.. _copydb:

copydb
------

**cubrid copydb** 유틸리티는 데이터베이스를 한 위치에서 다른 곳으로 복사 또는 이동하며, 인자로 원본 데이터베이스 이름과 새로운 데이터베이스 이름이 지정되어야 한다. 이때, 새로운 데이터베이스 이름은 원본 데이터베이스 이름과 다른 이름으로 지정되어야 하고, 새로운 데이터베이스에 대한 위치 정보는 **databases.txt**\에 등록된다.

**cubrid copydb** 유틸리티는 원본 데이터베이스가 정지 상태일 때(오프라인)에만 실행할 수 있다. ::

    cubrid copydb [<options>] src-database-name dest-database-name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **copydb**: 원본 데이터베이스를 새로운 위치로 이동 또는 복사하는 명령이다.

*   *src-database-name*: 복사 또는 이동하고자 하는 원본 데이터베이스 이름이다.

*   *dest-database-name*: 새로운 데이터베이스 이름이다.

[options]를 생략하면 원본 데이터베이스를 현재 작업 디렉터리에 복사한다.

**cubrid copydb**\에 대한 [options]는 다음과 같다.

.. program:: copydb

.. option:: --server-name=HOST

    새로운 데이터베이스의 서버 호스트 이름을 명시하며, 이는 **databases.txt**\ 의 **db-host** 항목에 등록된다. 이 옵션을 생략하면, 로컬 호스트가 등록된다. ::

        cubrid copydb --server-name=cub_server1 demodb new_demodb

.. option:: -F, --file-path=PATH

    새로운 데이터베이스 볼륨이 저장되는 특정 디렉터리 경로를 지정할 수 있다. 절대 경로로 지정해야 하며, 존재하지 않는 디렉터리를 지정하면 에러를 출력한다. 이 옵션을 생략하면 현재 작업 디렉터리에 새로운 데이터베이스의 볼륨이 생성된다. 이 경로는 **databases.txt**\ 의 **vol-path** 항목에 등록된다. ::
    
        cubrid copydb -F /home/usr/CUBRID/databases demodb new_demodb

.. option:: -L, --log-path=PATH

    새로운 데이터베이스 로그 볼륨이 저장되는 특정 디렉터리 경로를 지정할 수 있다. 절대 경로로 지정해야 하며, 존재하지 않는 디렉터리를 지정하면 에러를 출력한다. 이 옵션을 생략하면 새로운 데이터베이스 볼륨이 저장되는 경로에 로그 볼륨도 함께 생성된다. 이 경로는 **databases.txt**\ 의 **log-path** 항목에 등록된다. ::
    
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

    새로운 데이터베이스로 복사한 후, 원본 데이터베이스를 제거한다. 이 옵션이 주어지면 데이터베이스 복사 후 **cubrid deletedb**\를 수행하는 것과 동일하다. 단, 원본 데이터베이스에 **LOB** 데이터를 포함하는 경우, 원본 데이터베이스 대한 **LOB** 파일 디렉터리 경로가 새로운 데이터베이스로 복사되어 **databases.txt**\ 의 **lob-base-path** 항목에 등록된다. ::

        cubrid copydb -d -F /home/usr/CUBRID/databases demodb new_demodb

.. option:: --copy-lob-path=PATH

    원본 데이터베이스에 대한 **LOB** 파일 디렉터리 경로를 새로운 데이터베이스의 **LOB** 파일 경로로 복사하고, 원본 데이터베이스를 복사한다. 이 옵션을 생략하면, **LOB** 파일 디렉터리 경로를 복사하지 않으므로, **databases.txt** 파일의 **lob-base-path** 항목을 별도로 수정해야 한다. 이 옵션은 **-B** 옵션과 병행할 수 없다. ::

        cubrid copydb --copy-lob-path=/home/usr/CUBRID/databases/new_demodb/lob demodb new_demodb

.. option:: -B, --lob-base-path=PATH

    **-B** 옵션을 사용하여 특정 디렉터리를 새로운 데이터베이스에 대한 **LOB** 파일 디렉터리 경로를 지정한다. 원본 데이터베이스에 있던 **LOB** 파일들은 사용자가 직접 이 디렉터리에 복사해야 한다. 이 옵션은 **--copy-lob-path** 옵션과 병행할 수 없다. ::

        cubrid copydb -B /home/usr/CUBRID/databases/new_lob demodb new_demodb

.. _installdb:

installdb
---------

**cubrid installdb** 유틸리티는 데이터베이스 위치 정보를 저장하는 **databases.txt**\에 새로 설치된 데이터베이스 정보를 등록한다. 이 유틸리티의 실행은 등록 대상 데이터베이스의 동작에 영향을 끼치지 않는다.

::

    cubrid installdb [<options>] database_name    

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **installdb**: 이동 또는 복사된 데이터베이스의 정보를 **databases.txt**\에 등록하는 명령이다.

*   *database_name*: **databases.txt**\에 등록하고자 하는 데이터베이스의 이름이다.

[options]를 생략하는 경우, 해당 데이터베이스가 존재하는 디렉터리에서 명령을 수행해야 한다.

**cubrid installdb**\에 대한 [options]는 다음과 같다.

.. program:: installdb

.. option:: --server-name=HOST

    대상 데이터베이스의 서버 호스트 정보를 지정된 호스트 명으로 **databases.txt**\에 등록한다. 이 옵션을 생략하면, 현재의 호스트 정보가 등록된다.  ::

        cubrid installdb --server-name=cub_server1 testdb

.. option:: -F, --file-path=PATH
        
    대상 데이터베이스 볼륨 디렉터리의 절대 경로를 **databases.txt**\에 등록한다. 이 옵션을 생략하면 기본값인 현재 디렉터리 경로가 등록된다.  ::

        cubrid installdb -F /home/cubrid/CUBRID/databases/testdb testdb

.. option:: -L, --log-path=PATH

    대상 데이터베이스 로그 볼륨 디렉터리의 절대 경로를 **databases.txt**\에 등록한다. 이 옵션을 생략하면 데이터베이스 볼륨의 디렉터리 경로가 등록된다.  ::
    
        cubrid installdb -L /home/cubrid/CUBRID/databases/logs/testdb testdb

.. include:: backup.inc

.. include:: migration.inc

.. _spacedb:

spacedb
-------

**cubrid spacedb** 유틸리티는 사용 중인 데이터베이스 볼륨의 공간을 확인하기 위해서 사용된다.
**cubrid spacedb** 유틸리티는 데이터베이스에 있는 모든 영구 데이터 볼륨의 간략한 설명을 보여준다. **cubrid spacedb** 유틸리티에 의해 반환되는 정보는 볼륨 ID와 이름, 각 볼륨의 목적, 각 볼륨과 관련된 총(total) 공간과 빈(free) 공간이다. 

::

    cubrid spacedb [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **spacedb**: 대상 데이터베이스에 대한 공간을 확인하는 명령으로 데이터베이스 서버가 구동 정지 상태인 경우에만 정상적으로 수행된다.

*   *database_name*: 공간을 확인하고자 하는 데이터베이스의 이름이며, 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않는다.

다음은 **cubrid spacedb**\에 대한 [options]이다.

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

        $ cubrid spacedb --size-unit=M testdb
        $ cubrid spacedb --size-unit=H testdb

        Space description for database 'testdb' with pagesize 16.0K. (log pagesize: 16.0K)

        Volid  Purpose    total_size   free_size  Vol Name

            0   GENERIC       20.0 M      17.0 M  /home1/cubrid/testdb
            1      DATA       20.0 M      19.5 M  /home1/cubrid/testdb_x001
            2     INDEX       20.0 M      19.6 M  /home1/cubrid/testdb_x002
            3      TEMP       20.0 M      19.6 M  /home1/cubrid/testdb_x003
            4      TEMP       20.0 M      19.9 M  /home1/cubrid/testdb_x004
        -------------------------------------------------------------------------------
            5                100.0 M      95.6 M
        Space description for temporary volumes for database 'testdb' with pagesize 16.0K.

        Volid  Purpose    total_size   free_size  Vol Name

        LOB space description file:/home1/cubrid/lob

.. option:: -s, --summarize

    데이터 볼륨(DATA), 인덱스 볼륨(INDEX), 범용 볼륨(GENERIC), 임시 볼륨(TEMP), 일시적 임시 볼륨(TEMP TEMP)별로 전체 공간(total_pages), 사용 공간(used_pages), 빈 공간(free_pages)을 합산하여 출력한다. ::

        $ cubrid spacedb -s testdb

        Summarized space description for database 'testdb' with pagesize 16.0K. (log pagesize: 16.0K)

        Purpose     total_size   used_size   free_size  volume_count
        -------------------------------------------------------------
              DATA      20.0 M       0.5 M      19.5 M          1
             INDEX      20.0 M       0.4 M      19.6 M          1
           GENERIC      20.0 M       3.0 M      17.0 M          1
              TEMP      40.0 M       0.5 M      39.5 M          2
         TEMP TEMP       0.0 M       0.0 M       0.0 M          0
        -------------------------------------------------------------
             TOTAL     100.0 M       4.4 M      95.6 M          5

.. option:: -p, --purpose

    사용 중인 디스크 공간을 data_size, index_size, temp_size로 구분하여 출력한다.

    ::
    
        Space description for database 'testdb' with pagesize 16.0K. (log pagesize: 16.0K)

        Volid  Purpose    total_size   free_size   data_size  index_size   temp_size  Vol Name

            0   GENERIC       20.0 M      17.0 M       2.1 M       0.9 M       0.0 M  /home1/cubrid/testdb
            1      DATA       20.0 M      19.5 M       0.4 M       0.0 M       0.0 M  /home1/cubrid/testdb_x001
            2     INDEX       20.0 M      19.6 M       0.0 M       0.4 M       0.0 M  /home1/cubrid/testdb_x002
            3      TEMP       20.0 M      19.6 M       0.0 M       0.0 M       0.3 M  /home1/cubrid/testdb_x003
            4      TEMP       20.0 M      19.9 M       0.0 M       0.0 M       0.1 M  /home1/cubrid/testdb_x004
        ----------------------------------------------------------------------------------------------------
            5                100.0 M      95.6 M       2.5 M       1.2 M       0.4 M
        Space description for temporary volumes for database 'testdb' with pagesize 16.0K.

        Volid  Purpose    total_size   free_size   data_size  index_size   temp_size  Vol Name

        LOB space description file:/home1/cubrid/lob

.. note::

    **-p**\ 와 **-s**\ 를 함께 사용하는 경우, 요약 정보를 출력할 때 사용 중인 디스크 공간을 data_size, index_size, temp_size로 구분하여 출력한다.

    ::

        $ cubrid spacedb -s -p testdb
        Summarized space description for database 'testdb' with pagesize 16.0K. (log pagesize: 16.0K)

        Purpose     total_size   used_size   free_size   data_size  index_size   temp_size  volume_count
        -------------------------------------------------------------------------------------------------
              DATA      20.0 M       0.5 M      19.5 M       0.4 M       0.0 M       0.0 M          1
             INDEX      20.0 M       0.4 M      19.6 M       0.0 M       0.4 M       0.0 M          1
           GENERIC      20.0 M       3.0 M      17.0 M       2.1 M       0.9 M       0.0 M          1
              TEMP      40.0 M       0.5 M      39.5 M       0.0 M       0.0 M       0.4 M          2
         TEMP TEMP       0.0 M       0.0 M       0.0 M       0.0 M       0.0 M       0.0 M          0
        -------------------------------------------------------------------------------------------------
             TOTAL     100.0 M       4.4 M      95.6 M       2.5 M       1.2 M       0.4 M          5

.. _compactdb:

compactdb
---------

**cubrid compactdb** 유틸리티는 데이터베이스 볼륨 중에 사용되지 않는 공간을 확보하기 위해서 사용된다. 데이터베이스 서버가 정지된 경우(offline)에는 독립 모드(stand-alone mode)로, 데이터베이스가 구동 중인 경우(online)에는 클라이언트 서버 모드(client-server mode)로 공간 정리 작업을 수행할 수 있다.

.. note::

    **cubrid compactdb** 유틸리티는 삭제된 객체들의 OID와 클래스 변경에 의해 점유되고 있는 공간을 확보한다. 객체를 삭제하면 삭제된 객체를 참조하는 다른 객체가 있을 수 있기 때문에 삭제된 객체에 대한 OID는 바로 사용 가능한 빈 공간이 될 수 없다.
    
    따라서 OID 재사용을 위해 테이블 생성 시에는 아래 예와 같이 REUSE_OID 옵션을 사용할 것을 권장한다.
    
    .. code-block:: sql
    
        CREATE TABLE tbl REUSE_OID
        (
            id INT PRIMARY KEY, 
            b VARCHAR
        );
    
    단, REUSE_OID 옵션을 사용한 테이블은 다른 테이블이 참조할 수 없다. 즉, 다른 테이블의 타입으로 사용될 수 없다.
    
    .. code-block:: sql
    
        CREATE TABLE reuse_tbl (a INT PRIMARY KEY) REUSE_OID;
        CREATE TABLE tbl_1 ( a reuse_tbl);
    
    ::
    
        ERROR: The class 'reuse_tbl' is marked as REUSE_OID and is non-referable. Non-referable classes can't be the domain of an attribute and their instances' OIDs cannot be returned.
 
    REUSE_OID에 대한 자세한 설명은 :ref:`reuse-oid`\를 참고한다.

**cubrid compactdb** 유틸리티를 수행하면 삭제된 객체에 대한 참조를 **NULL**\ 로 표시하는데, 이렇게 **NULL**\ 로 표시된 공간은 OID가 재사용할 수 있는 공간임을 의미한다. ::

    cubrid compactdb [<options>] database_name [ class_name1, class_name2, ...]

*   **cubrid**: 큐브리드 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **compactdb**: 대상 데이터베이스에 대하여 삭제된 데이터에 할당되었던 OID가 재사용될 수 있도록 공간을 정리하는 명령으로서, 데이터베이스가 구동 정지 상태인 경우에만 정상적으로 수행된다.

*   *database_name*: 공간을 정리할 데이터베이스의 이름이며, 데이터베이스가 생성될 디렉터리 경로명을 포함하지 않는다.

*   *class_name_list*: 공간을 정리할 테이블 이름 리스트를 데이터베이스 이름 뒤에 직접 명시할 수 있으며, **-i** 옵션과 함께 사용할 수 없다. 클라이언트/서버 모드에서만 명시할 수 있다.

클라이언트/서버 모드에서만 **-I**, **-i**, **-c**, **-d**, **-p** 옵션을 사용할 수 있다.

다음은 **cubrid compactdb**\에 대한 [options]이다.

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

    한 번에 커밋할 수 있는 최대 페이지 수를 지정한다. 기본값은 **10**\이며, 최소 값은 1, 최대 값은 10이다. 옵션 값이 작으면 클래스/인스턴스에 대한 잠금 비용이 작으므로 동시성은 향상될 수 있으나 작업 속도는 저하될 수 있고, 옵션 값이 크면 동시성은 저하되나 작업 속도는 향상될 수 있다.  ::

        cubrid compactdb --CS-mode -p 10 testdb tbl1, tbl2, tbl5

.. option:: -d, --delete-old-repr

    카탈로그에서 과거 테이블 표현(스키마 구조)을 삭제할 수 있다. **ALTER** 문에 의해 칼럼이 추가되거나 삭제되는 경우 기존의 레코드에 대해 과거의 스키마를 참조하고 있는 상태로 두면, 스키마를 업데이트하는 비용을 들이지 않기 때문에 평소에는 과거의 테이블 표현을 유지하는 것이 좋다.

.. option:: -I, --Instance-lock-timeout=NUMBER

    인스턴스 잠금 타임아웃 값을 지정할 수 있다. 기본값은 **2** (초)이며, 최소 값은 1, 최대 값은 10이다. 설정된 시간동안 잠금 인스턴스를 대기하므로, 옵션 값이 작을수록 작업 속도는 향상될 수 있으나 처리 가능한 인스턴스 개수가 적어진다. 반면, 옵션 값이 클수록 작업 속도는 저하되나 더 많은 인스턴스에 대해 작업을 수행할 수 있다.

.. option:: -c, --class-lock-timeout=NUMBER

    클래스 잠금 타임아웃 값을 지정할 수 있다. 기본값은 **10** (초)이며, 최소값은 1, 최대 값은 10이다. 설정된 시간동안 잠금 테이블을 대기하므로, 옵션 값이 작을수록 작업 속도는 향상될 수 있으나 처리 가능한 테이블 개수가 적어진다. 반면, 옵션 값이 클수록 작업 속도는 저하되나 더 많은 테이블에 대해 작업을 수행할 수 있다.

.. _optimizedb:

optimizedb
----------

CUBRID의 질의 최적화기가 사용하는 테이블에 있는 객체들의 수, 접근하는 페이지들의 수, 속성 값들의 분산 같은 통계 정보를 갱신한다. ::

    cubrid optimizedb [option] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **optimizedb**: 대상 데이터베이스에 대하여 비용 기반 질의 최적화에 사용되는 통계 정보를 업데이트한다. 옵션을 지정하는 경우, 지정한 클래스에 대해서만 업데이트한다.

*   *database_name*: 비용기반 질의 최적화용 통계 자료를 업데이트하려는 데이터베이스 이름이다.

다음은 *cubrid optimizedb*\에 대한 [option]이다.

.. program:: optimizedb

.. option:: -n, --class-name

    **-n** 옵션을 이용하여 해당 클래스의 질의 통계 정보를 업데이트하는 명령이다. ::

        cubrid optimizedb -n event_table testdb

다음은 대상 데이터베이스의 전체 클래스의 질의 통계 정보를 업데이트하는 명령이다. ::

    cubrid optimizedb testdb

.. _plandump:

plandump
--------

**cubrid plandump** 유틸리티를 사용해서 서버에 저장(캐시)되어 있는 질의 수행 계획들의 정보를 출력할 수 있다. ::

    cubrid plandump [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **plandump**: 대상 데이터베이스에 대하여 현재 캐시에 저장되어 있는 질의 수행 계획을 출력하는 명령이다.

*   *database_name*: 데이터베이스 서버 캐시로부터 질의 수행 계획을 확인 또는 제거하고자 하는 데이터베이스 이름이다

옵션 없이 사용하면 캐시에 저장된 질의 수행 계획을 확인한다. ::

    cubrid plandump testdb

다음은 **cubrid plandump**\에 대한 [options]이다.

.. program:: plandump

.. option:: -d, --drop

    캐시에 저장된 질의 수행 계획을 제거한다. ::

        cubrid plandump -d testdb

.. option:: -o, --output-file=FILE
        
    캐시에 저장된 질의 수행 계획 결과 파일에 저장 ::

        cubrid plandump -o output.txt testdb

.. _statdump:

statdump
--------

**cubrid statdump** 유틸리티를 이용해 CUBRID 데이터베이스 서버가 실행한 통계 정보를 확인할 수 있으며, 통계 정보 항목은 크게 File I/O 관련, 페이지 버퍼 관련, 로그 관련, 트랜잭션 관련, 동시성 관련, 인덱스 관련, 쿼리 수행 관련, 네트워크 요청 관련으로 구분된다. 

CSQL의 해당 연결에 대해서만 통계 정보를 확인하려면 CSQL의 세션 명령어를 이용할 수 있으며 :ref:`CSQL 실행 통계 정보 출력 <csql-execution-statistics>`\ 를 참고한다.

::

    cubrid statdump [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **statdump**: 대상 데이터베이스 서버 실행 통계 정보를 출력하는 명령어이다. 데이터베이스가 동작 중일 때에만 정상 수행된다.

*   *database_name*: 통계 자료를 확인하고자 하는 대상 데이터베이스 이름이다.

다음은 **cubrid statdump**\에 대한 [options]이다.

.. program:: statdump

.. option:: -i, --interval=SECOND

    지정한 초 단위로 주기적으로 출력한다. **-i** 옵션이 주어질 때만 정보가 갱신된다.
    
    다음은 1초마다 누적된 정보 값을 출력한다. ::
    
        cubrid statdump -i 1 -c demodb
        
    다음은 1초 마다 0으로 리셋하고 1초 동안 누적된 값을 출력한다. ::
    
        cubrid statdump -i 1 demodb
        
    다음은 -i 옵션으로 가장 마지막에 실행한 값을 출력한다. ::
    
        cubrid statdump demodb
        
    다음은 위와 같은 결과를 출력한다. **-c** 옵션은 **-i** 옵션과 같이 쓰이지 않으면 옵션을 설정하지 않은 것과 동일하다.
    
        cubrid statdump -c demodb

    다음은 5초마다 결과를 출력한다. 
    
    ::

        $ cubrid statdump -i 5 -c testdb
         

        Thu January 07 16:46:05 GTB Standard Time 2016 

         *** SERVER EXECUTION STATISTICS ***
        Num_file_creates              =          0
        Num_file_removes              =          0
        Num_file_ioreads              =          0
        Num_file_iowrites             =          2
        Num_file_iosynches            =          2
        Num_file_page_allocs          =          0
        Num_file_page_deallocs        =          0
        Num_data_page_fetches         =       1742 
        Num_data_page_dirties         =         60
        Num_data_page_ioreads         =          0
        Num_data_page_iowrites        =          0
        Num_data_page_victims         =          0
        Num_data_page_iowrites_for_replacement =          0
        Num_data_page_hash_anchor_waits =          0
        Time_data_page_hash_anchor_wait =          0
        Num_data_page_fixed           =          0
        Num_data_page_dirty           =         15
        Num_data_page_lru1            =          0
        Num_data_page_lru2            =          0
        Num_data_page_ain             =        128
        Num_data_page_avoid_dealloc   =          0
        Num_data_page_avoid_victim    =          0
        Num_data_page_victim_cand     =          0
        Num_log_page_fetches          =          0
        Num_log_page_fetch_ioreads    =          0
        Num_log_page_ioreads          =          2
        Num_log_page_iowrites         =         45
        Num_log_append_records        =       2798
        Num_log_archives              =          0
        Num_log_start_checkpoints     =          3
        Num_log_end_checkpoints       =          3
        Num_log_wals                  =          3
        Num_log_page_iowrites_for_replacement =          0
        Num_page_locks_acquired       =          0
        Num_object_locks_acquired     =         65
        Num_page_locks_converted      =          0
        Num_object_locks_converted    =         10 
        Num_page_locks_re-requested   =          0
        Num_object_locks_re-requested =         46
        Num_page_locks_waits          =          0
        Num_object_locks_waits        =          0
        Num_object_locks_time_waited_usec =          0
        Num_tran_commits              =          3
        Num_tran_rollbacks            =          1
        Num_tran_savepoints           =          2
        Num_tran_start_topops         =          6
        Num_tran_end_topops           =          6
        Num_tran_interrupts           =          0
        Num_btree_inserts             =          3
        Num_btree_deletes             =          0
        Num_btree_updates             =          0
        Num_btree_covered             =          0
        Num_btree_noncovered          =          0
        Num_btree_resumes             =          0
        Num_btree_multirange_optimization =          0
        Num_btree_splits              =          0
        Num_btree_merges              =          0
        Num_btree_get_stats           =          0
        Num_heap_stats_sync_bestspace =          0
        Num_query_selects             =          2
        Num_query_inserts             =          0
        Num_query_deletes             =          0
        Num_query_updates             =          2
        Num_query_sscans              =          2
        Num_query_iscans              =          0
        Num_query_lscans              =          1
        Num_query_setscans            =          0
        Num_query_methscans           =          0
        Num_query_nljoins             =          1
        Num_query_mjoins              =          0
        Num_query_objfetches          =          0
        Num_query_holdable_cursors    =          0
        Num_sort_io_pages             =          0
        Num_sort_data_pages           =          0
        Num_network_requests          =         79 
        Num_adaptive_flush_pages      =          0
        Num_adaptive_flush_log_pages  =          2
        Num_adaptive_flush_max_pages  =     116610 
        Num_prior_lsa_list_size       =          5
        Num_prior_lsa_list_maxed      =          0
        Num_prior_lsa_list_removed    =          2 
        Num_heap_stats_bestspace_entries =       5
        Num_heap_stats_bestspace_maxed =          0
        Time_ha_replication_delay     =          0
        Num_plan_cache_add            =          1
        Num_plan_cache_lookup         =          2
        Num_plan_cache_hit            =          0
        Num_plan_cache_miss           =          2
        Num_plan_cache_full           =          0
        Num_plan_cache_delete         =          0
        Num_plan_cache_invalid_xasl_id =          0
        Num_plan_cache_query_string_hash_entries =          5
        Num_plan_cache_xasl_id_hash_entries =          5
        Num_plan_cache_class_oid_hash_entries =          10
        Num_vacuum_log_pages_vacuumed =          0
        Num_vacuum_log_pages_to_vacuum =          0
        Num_vacuum_prefetch_requests_log_pages =          0
        Num_vacuum_prefetch_hits_log_pages =          0
        Num_heap_home_inserts         =          0
        Num_heap_big_inserts          =          0
        Num_heap_assign_inserts       =          4
        Num_heap_home_deletes         =          0
        Num_heap_home_mvcc_deletes    =          0
        Num_heap_home_to_rel_deletes  =          0
        Num_heap_home_to_big_deletes  =          0
        Num_heap_rel_deletes          =          0
        Num_heap_rel_mvcc_deletes     =          0
        Num_heap_rel_to_home_deletes  =          0
        Num_heap_rel_to_big_deletes   =          0
        Num_heap_rel_to_rel_deletes   =          0
        Num_heap_big_deletes          =          0
        Num_heap_big_mvcc_deletes     =          0
        Num_heap_new_ver_inserts      =          0
        Num_heap_home_updates         =          6
        Num_heap_home_to_rel_updates  =          0
        Num_heap_home_to_big_updates  =          0
        Num_heap_rel_updates          =          0
        Num_heap_rel_to_home_updates  =          0
        Num_heap_rel_to_rel_updates   =          0
        Num_heap_rel_to_big_updates   =          0
        Num_heap_big_updates          =          0
        Num_heap_home_vacuums         =          0
        Num_heap_big_vacuums          =          0
        Num_heap_rel_vacuums          =          0
        Num_heap_insid_vacuums        =          0
        Num_heap_remove_vacuums       =          0
        Num_heap_next_ver_vacuums     =          0
        Time_heap_insert_prepare      =       1962
        Time_heap_insert_execute      =      10007
        Time_heap_insert_log          =         44
        Time_heap_delete_prepare      =          0
        Time_heap_delete_execute      =          0
        Time_heap_delete_log          =          0
        Time_heap_update_prepare      =        497
        Time_heap_update_execute      =        972
        Time_heap_update_log          =        267
        Time_heap_vacuum_prepare      =          0
        Time_heap_vacuum_execute      =          0
        Time_heap_vacuum_log          =          0
        Num_bt_find_unique            =          2
        Num_btrange_search            =          0
        Num_bt_insert_obj             =          3
        Num_bt_delete_obj             =          0
        Num_bt_mvcc_delete            =          0
        Num_bt_mark_delete            =          0
        Num_bt_update_sk_cnt          =          0
        Num_bt_undo_insert            =          0
        Num_bt_undo_delete            =          0
        Num_bt_undo_mvcc_delete       =          0
        Num_bt_undo_update_sk         =          0
        Num_bt_vacuum                 =          0
        Num_bt_vacuum_insid           =          0
        Num_bt_vacuum_update_sk       =          0
        Num_bt_fix_ovf_oids_cnt       =          0
        Num_bt_unique_rlocks_cnt      =          0
        Num_bt_unique_wlocks_cnt      =          0
        Time_bt_find_unique           =         17
        Time_bt_range_search          =          0
        Time_bt_insert                =       1845
        Time_bt_delete                =          0
        Time_bt_mvcc_delete           =          0
        Time_bt_mark_delete           =          0
        Time_bt_update_sk             =          0
        Time_bt_undo_insert           =          0
        Time_bt_undo_delete           =          0
        Time_bt_undo_mvcc_delete      =          0
        Time_bt_undo_update_sk        =          0
        Time_bt_vacuum                =          0
        Time_bt_vacuum_insid          =          0
        Time_bt_vacuum_update_sk      =          0
        Time_bt_traverse              =       1616
        Time_bt_find_unique_traverse  =        716
        Time_bt_range_search_traverse =          0
        Time_bt_insert_traverse       =        900
        Time_bt_delete_traverse       =          0
        Time_bt_mvcc_delete_traverse  =          0
        Time_bt_mark_delete_traverse  =          0
        Time_bt_update_sk_traverse    =          0
        Time_bt_undo_insert_traverse  =          0
        Time_bt_undo_delete_traverse  =          0
        Time_bt_undo_mvcc_delete_traverse =          0
        Time_bt_undo_update_sk_traverse =          0
        Time_bt_vacuum_traverse       =          0
        Time_bt_vacuum_insid_traverse =          0
        Time_bt_vacuum_update_sk_traverse =          0
        Time_bt_fix_ovf_oids          =          0
        Time_bt_unique_rlocks         =          0
        Time_bt_unique_wlocks         =          0
        Time_vacuum_master            =     152858
        Time_vacuum_worker_process_log =          0
        Time_vacuum_worker_execute    =          0

         *** OTHER STATISTICS ***
        Data_page_buffer_hit_ratio    =     100.00
        Log_page_buffer_hit_ratio     =       0.00
        Vacuum_data_page_buffer_hit_ratio =       0.00
        Vacuum_page_efficiency_ratio  =       0.00
        Vacuum_page_fetch_ratio       =       0.00
        Data_page_fix_lock_acquire_time_msec =       0.00
        Data_page_fix_hold_acquire_time_msec =       0.00
        Data_page_fix_acquire_time_msec =      11.80
        Data_page_allocate_time_ratio =     100.00
        Data_page_total_promote_success =       3.00
        Data_page_total_promote_fail  =       0.00
        Data_page_total_promote_time_msec =       0.00
        Num_data_page_fix_ext:
        WORKER,PAGE_FTAB     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =         17
        WORKER,PAGE_FTAB     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =          2
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,READ ,COND        =        194
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =          9
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,WRITE,COND        =         18
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =          2
        WORKER,PAGE_VOLHEADER,OLD_PAGE_IN_PB    ,READ ,COND        =          8
        WORKER,PAGE_VOLHEADER,OLD_PAGE_IN_PB    ,READ ,UNCOND      =        914
        WORKER,PAGE_VOLBITMAP,OLD_PAGE_IN_PB    ,READ ,COND        =          4
        WORKER,PAGE_VOLBITMAP,OLD_PAGE_IN_PB    ,READ ,UNCOND      =        457
        WORKER,PAGE_XASL     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =          2
        WORKER,PAGE_XASL     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =          1
        WORKER,PAGE_CATALOG  ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =         75
        WORKER,PAGE_CATALOG  ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =         10
        WORKER,PAGE_BTREE_R  ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =         29
        Num_data_page_promote_ext:
        WORKER,PAGE_BTREE_R  ,SHARED_READER,READ ,SUCCESS =          3
        Num_data_page_promote_time_ext:
        WORKER,PAGE_BTREE_R  ,SHARED_READER,READ ,SUCCESS =          3
        Num_data_page_unfix_ext:
        WORKER,PAGE_FTAB     ,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =         16
        WORKER,PAGE_FTAB     ,BUF_NON_DIRTY,HOLDER_DIRTY    ,WRITE =          2
        WORKER,PAGE_FTAB     ,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =          1
        WORKER,PAGE_HEAP     ,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =        185
        WORKER,PAGE_HEAP     ,BUF_NON_DIRTY,HOLDER_DIRTY    ,WRITE =          9
        WORKER,PAGE_HEAP     ,BUF_NON_DIRTY,HOLDER_DIRTY    ,MIXED =          2
        WORKER,PAGE_HEAP     ,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =         14
        WORKER,PAGE_HEAP     ,BUF_DIRTY    ,HOLDER_NON_DIRTY,WRITE =          4
        WORKER,PAGE_HEAP     ,BUF_DIRTY    ,HOLDER_DIRTY    ,WRITE =          3
        WORKER,PAGE_HEAP     ,BUF_DIRTY    ,HOLDER_DIRTY    ,MIXED =          6
        WORKER,PAGE_VOLHEADER,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =         14
        WORKER,PAGE_VOLHEADER,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =        908
        WORKER,PAGE_VOLBITMAP,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =        461
        WORKER,PAGE_XASL     ,BUF_NON_DIRTY,HOLDER_DIRTY    ,WRITE =          1
        WORKER,PAGE_XASL     ,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =          2
        WORKER,PAGE_CATALOG  ,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =         47
        WORKER,PAGE_CATALOG  ,BUF_NON_DIRTY,HOLDER_DIRTY    ,WRITE =          1
        WORKER,PAGE_CATALOG  ,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =         28
        WORKER,PAGE_CATALOG  ,BUF_DIRTY    ,HOLDER_NON_DIRTY,WRITE =          1
        WORKER,PAGE_CATALOG  ,BUF_DIRTY    ,HOLDER_DIRTY    ,WRITE =          8
        WORKER,PAGE_BTREE_R  ,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =         18
        WORKER,PAGE_BTREE_R  ,BUF_NON_DIRTY,HOLDER_DIRTY    ,MIXED =          3
        WORKER,PAGE_BTREE_R  ,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =          8
        Time_data_page_lock_acquire_time:
        Time_data_page_hold_acquire_time:
	Time_data_page_fix_acquire_time:
        WORKER,PAGE_FTAB     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =               65
        WORKER,PAGE_FTAB     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =               12
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,READ ,COND        =              617
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =               42
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,WRITE,COND        =               81
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =                9
        WORKER,PAGE_VOLHEADER,OLD_PAGE_IN_PB    ,READ ,COND        =               36
        WORKER,PAGE_VOLHEADER,OLD_PAGE_IN_PB    ,READ ,UNCOND      =             3277
        WORKER,PAGE_VOLBITMAP,OLD_PAGE_IN_PB    ,READ ,COND        =               18
        WORKER,PAGE_VOLBITMAP,OLD_PAGE_IN_PB    ,READ ,UNCOND      =             1533
        WORKER,PAGE_XASL     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =                5
        WORKER,PAGE_XASL     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =             5644
        WORKER,PAGE_CATALOG  ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =              260
        WORKER,PAGE_CATALOG  ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =               43
        WORKER,PAGE_BTREE_R  ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =              164
        Num_mvcc_snapshot_ext:
        DELETE  ,INS_VACUUMED      ,VISIBLE   =                7
        DIRTY   ,INS_VACUUMED      ,VISIBLE   =                3
        DIRTY   ,INS_CURR          ,VISIBLE   =                2
        SNAPSHOT,INS_VACUUMED      ,VISIBLE   =               87
        SNAPSHOT,DELETED_COMMITED  ,INVISIBLE =                1
        Time_obj_lock_acquire_time:
        Time_get_snapshot_acquire_time:
        WORKER =               14
        Count_get_snapshot_retry:
        WORKER =               11
        Time_tran_complete_time:
        WORKER =               19
        Time_get_oldest_mvcc_acquire_time:
        SYSTEM =           112110
        Count_get_oldest_mvcc_retry:
        WORKER =                1
       
    다음은 위의 데이터베이스 서버 실행 통계 정보에 대한 설명이다.

    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 분류             | 항목                                     | 설명                                                                                   |
    +==================+==========================================+========================================================================================+
    | File I/O         | Num_file_creates                         | 생성한 파일 개수                                                                       |
    | 관련             +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_file_removes                         | 삭제한 파일 개수                                                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_file_ioreads                         | 디스크로부터 읽은 횟수                                                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_file_iowrites                        | 디스크로 저장한 횟수                                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_file_iosynches                       | 디스크와 동기화를 수행한 횟수                                                          |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 페이지 버퍼      | Num_data_page_fetches                    | 데이터 페이지를 페치(fetch)한 횟수                                                     |
    | 관련             +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_dirties                    | 데이터 페이지를 변경(dirty)한 횟수                                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_ioreads                    | 데이터 페이지를 디스크에서 읽은 횟수                                                   |
    |                  |                                          | (이 값이 클수록 덜 효율적이며, 히트율이 낮은 것과 상관됨)                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_iowrites                   | 데이터 페이지를 디스크에 기록한 횟수(이 값이 클수록 덜 효율적임)                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_victims                    | 데이터 버퍼에서 디스크로 내려 쓰기(flush)하는 스레드가 깨어나는 회수                   |
    |                  |                                          | (내려 쓰기되는 페이지의 또는 희생자(victim)의 개수가 아님)                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_iowrites_for_replacement   | 후보로 선정되어 디스크로 쓰여진 데이터 페이지 수                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_hash_anchor_waits          | The number of instances any hash anchor had to wait for mutex acquisition              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_data_page_hash_anchor_wait          | The total time in microseconds any hash anchor had to wait for mutex acquisition       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_fixed                      | 데이타 버퍼의 고정 페이지 수 (snapshot counter)                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_dirty                      | 데이타 버퍼의 dirty page 수  (snapshot counter)                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_lru1                       | 데이타 버퍼의 LRU1 수     (snapshot counter)                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_lru2                       | 데이타 버퍼의 LRU2 수     (snapshot counter)                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_ain                        | 데이타 버퍼의 AIN 존 수   (snapshot counter)                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_avoid_dealloc              | 데이타 버퍼의 avoid_dealloc_cnt 가 0보다 큰 페이지 수 (snapshot counter)               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_avoid_victim               | 데이타 버퍼의 avoid_victim 플래그 세트를 가지고 있는 페이지 수 (snapshot counter)      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_victim_cand                | 데이타 버퍼의 victim 후보 페이지 수 (snapshot counter)                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_adaptive_flush_pages                 | 데이터 버퍼로부터 디스크로 내려 쓰기(flush)한 데이터 페이지 수                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_adaptive_flush_log_pages             | 로그 버퍼로부터 디스크로 내려 쓰기(flush)한 로그 페이지 수                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_adaptive_flush_max_pages             | 데이터 및 로그 버퍼로부터 디스크로 내려 쓰기(flush)를 허용하는 최대                    |
    |                  |                                          | 페이지 수                                                                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_prior_lsa_list_size                  | prior LSA(Log Sequence Address) list의 현재 크기.                                      |
    |                  |                                          | prior LSA list에는 로그 버퍼에서 디스크에 쓰기 작업을 수행하기 전 미리 쓰기 순서를     |
    |                  |                                          | LSA로 기록하는데, 디스크에 쓰는 작업으로 인한 트랜잭션의 대기 시간을 줄여서 동시성을   |
    |                  |                                          | 높이기 위해 사용된다.                                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_prior_lsa_list_maxed                 | prior LSA list의 최대 크기에 도달한 회수.                                              |
    |                  |                                          | prior LSA list의 최대 크기는 log_buffer_size * 2이다.                                  |
    |                  |                                          | 이 값이 크면 로그 쓰기 작업이 동시에 많이 발생한다고 볼 수 있다.                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_prior_lsa_list_removed               | prior LSA list에서 로그 버퍼로 이동한 회수.                                            |
    |                  |                                          | 이 값과 비슷한 회수의 커밋이 발생했다고 볼 수 있다.                                    |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 로그 관련        | Num_log_page_fetches                     | 페치된 로그 페이지의 개수                                                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_page_fetch_ioreads               | 페치 시 로그 페이지의 I/O 읽기 회수                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_page_ioreads                     | 로그 페이지의 읽기 I/O 회수                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_page_iowrites                    | 로그 페이지의 쓰기 I/O 회수                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_append_records                   | 추가(append)한 로그 레코드의 수                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_archives                         | 보관 로그의 개수                                                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_start_checkpoints                | 체크포인트 시작 횟수                                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_end_checkpoints                  | 체크포인트 종료 횟수                                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_wals                             | 현재 사용하지 않음                                                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_page_iowrites_for_replacement    | 페이지 교체로 인해 로그 페이지 버퍼로부터 버려진 로그 데이터 페이지의 개수             |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 동시성/잠금      | Num_page_locks_acquired                  | 페이지 잠금을 획득한 횟수                                                              |
    | 관련             +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_object_locks_acquired                | 오브젝트 잠금을 획득한 횟수                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_page_locks_converted                 | 페이지 잠금 타입을 변환한 횟수                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_object_locks_converted               | 오브젝트 잠금 타입을 변환한 횟수                                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_page_locks_re-requested              | 페이지 잠금을 재요청한 횟수                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_object_locks_re-requested            | 오브젝트 잠금을 재요청한 횟수                                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_page_locks_waits                     | 잠금을 대기하는 페이지 개수                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_object_locks_waits                   | 잠금을 대기하는 오브젝트 개수                                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_object_locks_time_waited_usec        | The time in microseconds spent on waiting for all object locks                         |    
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 트랜잭션         | Num_tran_commits                         | 커밋한 횟수                                                                            |
    | 관련             +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_tran_rollbacks                       | 롤백한 횟수                                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_tran_savepoints                      | 세이브포인트 횟수                                                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_tran_start_topops                    | 시작한 top operation의 개수                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_tran_end_topops                      | 종료한 top operation의 개수                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_tran_interrupts                      | 인터럽트 개수                                                                          |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 인덱스 관련      | Num_btree_inserts                        | 삽입된 항목의 개수                                                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_deletes                        | 삭제된 항목의 개수                                                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_updates                        | 갱신된 항목의 개수                                                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_covered                        | 질의 시 인덱스가 데이터를 모두 포함한 경우의 개수                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_noncovered                     | 질의 시 인덱스가 데이터를 일부분만 포함하거나 전혀 포함하지 않은 경우의 개수           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_resumes                        | index_scan_oid_buffer_pages를 초과한 인덱스 스캔 횟수                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_multirange_optimization        | WHERE ... IN ... LIMIT 조건 질의문에 대해 다중 범위                                    |
    |                  |                                          | 최적화(multi-range optimization)를 수행한 횟수                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_splits                         | B-tree 노드 분할 연산 회수                                                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_merges                         | B-tree 노드 합병 연산 회수                                                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_find_unique                       | B-tree 노드 'find-unique' 연산 횟수                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btrange_search                       | B-tree 노드 'range-search' 연산 횟수                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_insert_obj                        | B-tree 노드 'insert object' 연산 횟수                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_delete_obj                        | B-tree 노드 'physical delete object' 연산 횟수                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_mvcc_delete                       | B-tree 노드 'mvcc delete' 연산 횟수                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_mark_delete                       | B-tree 노드 'mark delete' 연산 횟수                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_update_sk_cnt                     | B-tree 노드 'update same key' 연산 횟수                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_undo_insert                       | B-tree 노드 'undo insert' 연산 횟수                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_undo_delete                       | B-tree 노드 'undo physical delete' 연산 횟수                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_undo_mvcc_delete                  | B-tree 노드 'undo mvcc delete' 연산 횟수                                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_undo_update_sk                    | B-tree 노드 'undo update samekey' 연산 횟수                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_vacuum                            | B-tree 노드 'vacuum deleted object' 연산 횟수                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_vacuum_insid                      | B-tree 노드 'vacumm insert id' 연산 횟수The                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_vacuum_update_sk                  | B-tree 노드 'vacumm update samekey' 연산 횟수                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_fix_ovf_oids_cnt                  | B-tree 노드 오버플로우 페이지 수정                                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_unique_rlocks_cnt                 | 유니크 인덱스에 대한 블록된  읽기 수                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_unique_wlocks_cnt                 | 유니크 인덱스에 대한 블록된 쓰기  수                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_find_unique                      | B-tree 노드의 'find-unique' 연산에 걸린 시간                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_range_search                     | B-tree 노드의 'ranage search' 연산에 걸린 시간                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_insert                           | B-tree 노드의 'insert object' 연산에 걸린 시간                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_delete                           | B-tree 노드의 'physical delete' 연산에 걸린 시간                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_mvcc_delete                      | B-tree 노드의 'mvcc delete' 연산에 걸린 시간                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_mark_delete                      | B-tree 노드의 'mark delete' 연산에 걸린 시간                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_update_sk                        | B-tree 노드의 'update same key' 연산에 걸린 시간                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_insert                      | B-tree 노드의 'undo insert' 연산에 걸린 시간                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_delete                      | B-tree 노드의 'undo physical delete' 연산에 걸린 시간                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_mvcc_delete                 | B-tree 노드의 'undo mvcc delete' 연산에 걸린 시간                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_update_sk                   | B-tree 노드의 'undo update same key' 연산에 걸린 시간                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum                           | B-tree 노드의 'vacuum deleted object' 연산에 걸린 시간                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum_insid                     | B-tree 노드의 'vacuum insert id' 연산에 걸린 시간Time                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum_update_sk                 | B-tree 노드의 'vacuum update same key' 연산에 걸린 시간                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_traverse                         | B-tree 노드의 'traverse' 연산에 걸린 시간                                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_find_unique_traverse             | B-tree 노드의 'find unique traverse' 연산에 걸린 시간                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_range_search_traverse            | B-tree 노드의 'range search traverse' 연산에 걸린 시간                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_insert_traverse                  | B-tree 노드의 'insert travers' 연산에 걸린 시간                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_delete_traverse                  | B-tree 노드의 'physical delete travers' 연산에 걸린 시간                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_mvcc_delete_traverse             | B-tree 노드의 'mvcc delete traverse' 연산에 걸린 시간                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_mark_delete_traverse             | B-tree 노드의 'mark delete traverse' 연산에 걸린 시간                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_update_sk_traverse               | B-tree 노드의 'update same key traverse' 연산에 걸린 시간                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_insert_traverse             | B-tree 노드의 'undo insert traverse' 연산에 걸린 시간Time                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_delete_traverse             | B-tree 노드의 'undo delete traverse' 연산에 걸린 시간Time                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_mvcc_delete_traverse        | B-tree 노드의 'undo mvcc delete traverse' 연산에 걸린 시간                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_update_sk_traverse          | B-tree 노드의 'undo update sk traverse' 연산에 걸린 시간                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum_traverse                  | B-tree 노드의 'vacumm traverse ' 연산에 걸린 시간                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum_insid_traverse            | B-tree 노드의 'vacuum insid traverse' 연산에 걸린 시간                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum_update_sk_traverse        | B-tree 노드의 'vacuum update sk traverse' 연산에 걸린 시간                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_fix_ovf_oids                     | B-tree 노드의  오퍼플로우 페이지 수정에 걸린 시간                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_unique_rlocks                    | 유니크 인덱스에 대한 블록된 읽기에 걸린 시간                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_unique_wlocks                    | 유니크 인덱스에 대한 블록된 쓰기에 걸린 시간                                           |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 쿼리 관련        | Num_query_selects                        | SELECT 쿼리의 수행 횟수                                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_inserts                        | INSERT 쿼리의 수행 횟수                                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_deletes                        | DELETE 쿼리의 수행 횟수                                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_updates                        | UPDATE 쿼리의 수행 횟수                                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_sscans                         | 순차 스캔(풀 스캔) 횟수                                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_iscans                         | 인덱스 스캔 횟수                                                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_lscans                         | LIST 스캔 횟수                                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_setscans                       | SET 스캔 횟수                                                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_methscans                      | METHOD 스캔 횟수                                                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_nljoins                        | Nested Loop 조인 횟수                                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_mjoins                         | 병합 조인 횟수                                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_objfetches                     | 객체를 가져오기(fetch)한 횟수                                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_holdable_cursors               | 현재 서버에서 유지 중인 커서(holdable cursor)의 개수                                   |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 정렬 관련        | Num_sort_io_pages                        | 정렬하는 동안 디스크에서 페치한 페이지 개수(이 값이 클수록 덜 효율적임)                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_sort_data_pages                      | 정렬하는 동안 페이지 버퍼에서 발견된 페이지 개수(이 값이 클수록 더 효율적임)           |

    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 네트워크         | Num_network_requests                     | 네트워크 요청 횟수                                                                     |
    | 요청 관련        |                                          |                                                                                        |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 힙 관련          | Num_heap_stats_bestspace_entries         | "베스트 페이지" 목록에 저장된 베스트 페이지의 개수                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_stats_bestspace_maxed           | "베스트 페이지" 목록에 저장 가능한 베스트 페이지의 최대 개수                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_stats_sync_bestspace            | "베스트 페이지" 목록의 갱신 횟수.                                                      |
    |                  |                                          |                                                                                        |
    |                  |                                          | "베스트 페이지"는 INSERT와 DELETE가 반복되는 환경에서 데이터를 가지고 있는 페이지의    |
    |                  |                                          | 여유 공간이 30% 이상인 페이지들을 의미하며, 이 페이지들의 일부 정보만 "베스트 페이지   |
    |                  |                                          | 목록"을 만들어 저장함. "베스트 페이지"에는 한번에 최대 100만개의 페이지 정보를 기록함. |
    |                  |                                          | INSERT 시 이 목록을 탐색하여 이 레코드를 저장할 여유 공간을 가진 페이지가 없으면       |
    |                  |                                          | "베스트 페이지 목록"을 갱신함. 수 차례 목록을 갱신해도 해당 목록에서 여유 공간을 가진  |
    |                  |                                          | 페이지가 여전히 없으면 새 페이지에 레코드를 저장함.                                    |
    |                  |                                          |                                                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_inserts                    | HOME 타입 레코드 힙에 대한 insert 수                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_big_inserts                     | BIG 타입 레코드 힙에 대한 insert 수                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_assign_inserts                  | ASSIGN 타입 레코드 힙에 대한 insert 수                                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_deletes                    | MVCC 모드가 아닌 HOME 타입 레코드에 대한 delete 수                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_mvcc_deletes               | MVCC 모드 HOME 타입 레코드로부터 delete 수                                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_to_rel_deletes             | MVCC 모드 HOME 타입 레코드로부터 RELOCATION 타입 레코드의 delete 수                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_to_big_deletes             | MVCC 모드 HOME 타입 레코드로부터 BIG 타입 레코드의 delete 수                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_deletes                     | MVCC 모드가 아닌 RELOCATION 타입 레코드에 대한 delete 수                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_mvcc_deletes                | MVCC 모드 RELOCATION 타입 레코드로부터 delete 수                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_home_deletes             | MVCC 모드 RELOCATION 타입 레코드로부터 HOME 타입 레코드의 delete 수                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_big_deletes              | MVCC 모드 RELOCATION 타입 레코드로부터 BIG 타입 레코드의 delete 수                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_rel_deletes              | MVCC 모드 RELOCATION 타입 레코드로부터 RELOCATION 타입 레코드의 delete 수              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_big_deletes                     | MVCC 모드가 아닌 BIG 타입 레코드에 대한 delete 수                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_big_mvcc_deletes                | MVCC 모드 BIG 타입 레코드로부터 HOME 타입 레코드의 mvcc  delete 수                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_new_ver_inserts                 | MVCC 모드에서 같은 객체에 대한 새로운 버전의 insert 수                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_updates                    | MVCC 모드가 아닌 HEAP HOME 타입 레코드에서 update 수                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_to_rel_updates             | MVCC 모드가 아닌  HOME 타입 레코드로부터 RELOCATION 타입 레코드의 update 수            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_to_big_updates             | MVCC 모드가 아닌 HOME 타입 레코드로부터 BIG 타입 레코드의 update  수                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_updates                     | MVCC 모드가 아닌 RELOCATION 타입 레코드의 update 수                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_home_updates             | MVCC 모드가 아닌  RELOCATION 타입 레코드로부터 HOME 타입 레코드의 update 수            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_rel_updates              | MVCC 모드가 아닌  RELOCATION 타입 레코드로부터 RELOCATION 타입 레코드의 update 수      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_big_updates              | MVCC 모드가 아닌  RELOCATION 타입 레코드로부터 BIG 타입 레코드의 update 수             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_big_updates                     | MVCC 모드가 아닌 HEAP BIG 타입 레코드에서 update 수                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_vacuums                    | HOME 타입 레코드의 vacuumed HEAP 수                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_big_vacuums                     | BIG 타입 레코드의 vacuumed HEAP 수                                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_vacuums                     | RELOCATION 타입 레코드의 vacuumed HEAP 수                                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_insid_vacuums                   | 새롭게 추가된 vacuumed heap 의 수                                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_remove_vacuums                  | 버전정보를 제거하고 다음 버전정보를 보관하지 않는 vacuum 연산의 수                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_next_ver_vacuums                | 버전정보를 제거하고 다음 버전정보를 보관하는 vacuum 연산의 수                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_insert_prepare                 | 힙 insert 연산을 준비하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_insert_execute                 | 힙 insert 연산을 실행하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_insert_log                     | 힙 insert 연산을 기록하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_delete_prepare                 | 힙 delete 연산을 준비하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_delete_execute                 | 힙 delete 연산을 실행하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_delete_log                     | 힙 delete 연산을 기록하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_update_prepare                 | 힙 update 연산을 준비하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_update_execute                 | 힙 update 연산을 실행하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_update_log                     | 힙 update 연산을 기록하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_vacuum_prepare                 | 힙 vacuum 연산을 준비하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_vacuum_execute                 | 힙 vacuum 연산을 실행하는 시간                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_vacuum_log                     | 힙 vacuum 연산을 기록하는 시간                                                         |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 질의 계획        | Num_plan_cache_add                       | 캐시 엔트리(entry)가 새로 추가된 횟수                                                  |
    | 캐시 관련        +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_lookup                    | 특정 키를 사용하여 룩업(lookup)을 시도한 횟수                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_hit                       | 질의 문자열 해시 테이블에서 엔트리를 찾은(hit) 횟수                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_miss                      | 질의 문자열 해시 테이블에서 엔트리를 찾지 못한(miss) 횟수                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_full                      | 캐시 엔트리의 개수가 허용된 최대 개수를 넘어 희생자(victim) 탐색을 시도한 횟수         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_delete                    | 캐시 엔트리가 삭제된(victimized) 횟수                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_invalid_xasl_id           | xasl_id 해시 테이블에서 엔트리를 찾지 못한(miss) 횟수.                                 |
    |                  |                                          | 서버에서 특정 엔트리가 제거(victimized)되었는데, 해당 엔트리를 클라이언트에서          |
    |                  |                                          | 요청했을 때 발생하는 에러 횟수                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_query_string_hash_entries | 질의 문자열 해시 테이블의 현재 엔트리 개수                                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_xasl_id_hash_entries      | xasl id 해시 테이블의 현재 엔트리 개수                                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_class_oid_hash_entries    | class oid 해시 테이블의 현재 엔트리 개수                                               |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | HA 관련          | Time_ha_replication_delay                | 복제 지연 시간(초)                                                                     |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Vacuuming        | Num_vacuum_log_pages_vacuumed            | vacuum 작업자에 의해 정리되는(vacuumed) 로그 페이지의 개수.                            |
    | 관련             |                                          | 실시간으로 업데이트되지 않음.                                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_vacuum_log_pages_to_vacuum           | vacuum 작업자에 의해 정리될(to be vacuumed) 로그 페이지의 개수                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_vacuum_prefetch_requests_log_pages   | vacuum 로그 페이지 prefetch 요청 수                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_vacuum_prefetch_hits_log_pages       | vacuum 로그 페이지 prefetch hit 수                                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_vacuum_master                       | vacuum 마스터 쓰레드의 사용시간                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_vacuum_worker_process_log           | vacuum 로그 워커 쓰레드의 사용시간                                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_vacuum_worker_execute               | vacuum 워커 쓰레드의 실행 시간                                                         |    
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | 기타             | Data_page_buffer_hit_ratio               | 데이터 페이지 버퍼의 히트율                                                            |
    |                  |                                          | (Num_data_page_fetches - Num_data_page_ioreads)*100 / Num_data_page_fetches            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Log_page_buffer_hit_ratio                | 로그 페이지 버퍼의 히트율                                                              |
    |                  |                                          | (Num_log_page_fetches - Num_log_page_fetch_ioreads)*100 / Num_log_page_fetches         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Vacuum_data_page_buffer_hit_ratio        | vacuuming되는 데이터 페이지 버퍼의 히트율                                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Vacuum_page_efficiency_ratio             | vacuuming될 때 더티 플래그를 가진 페이지 언픽스 개수와 전체 페이지 언픽스 사이의 비율. |
    |                  |                                          | 이상적인 경우는 사용않는 모든 레코드를 정리하기 때문에 쓰기 작업만 수행되는 것임.      |
    |                  |                                          | vacuuming이 최적화되었더라도, 실제로 100%는 될 수 없음.                                |
    |                  |                                          |                                                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Vacuum_page_fetch_ratio                  | vacuum 모듈과 전체 페이지 언픽스 간의 비율.                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_fix_lock_acquire_time_msec     | 페이지 잠금 확득에 대한 누적 시간                                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_fix_hold_acquire_time_msec     | 페이지 유지(hold) 획득에 대한 누적 시간                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_fix_acquire_time_msec          | 페이지 픽스(fix) 획득에 대한 누적 시간                                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_allocate_time_ratio            | 페이지 할당에 소요된 누적 시간의 비율                                                  |
    |                  |                                          | (Data_page_fix_acquire_time_msec - Data_page_fix_hold_acquire_time_msec -              |
    |                  |                                          | Data_page_fix_lock_acquire_time_msec)*100 / Data_page_fix_acquire_time_msec            |
    |                  |                                          | 이 값이 클수록 I/O가 주요 병목의 원인이며, 작을수록 동시성 처리가 주요 병목의 원인임   |
    |                  |                                          |                                                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_total_promote_success          | latch promote 가 성공한 누적 수                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_total_promote_fail             | latch promote 가 실패한 누적 수                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_total_promote_time_msec        | latch promote 의 누적 시간                                                             |    
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_fix_ext:                   | 모듈, 페이지 타입, 그리고 페이지 버퍼에서 발견된 페이지 타입의 신구 여부에 따른        |
    |                  |                                          | 페이지 픽스 개수.                                                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_unfix_ext:                 | 모듈, 페이지 타입, 그리고 페이지의 더티 여부에 따른 페이지의 언픽스 개수.              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_data_page_lock_acquire_time:        | 모듈, 페이지 타입, 페이지 모드, 래치 모드, 그리고 컨디션 모드에 따른 잠금 획득 시간.   |
    |                  |                                          |                                                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_data_page_hold_acquire_time:        | 모듈, 페이지 타입, 페이지 모드, 그리고 래치 모드에 따른 유지(hold) 획득 시간.          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_data_page_fix_acquire_time:         | 모듈, 페이지 타입, 페이지 모드, 래치 모드, 그리고 컨디션 모드에 따른 페이지 픽스(fix)  |
    |                  |                                          | 획득 시간.                                                                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_mvcc_snapshot_ext:                   | 스냅 샷 유효성 검사 기능의 수                                                          |
    |                  |                                          | (스냅 샷 유형, 레코드 유형, 유효성 검사시 시정 결과에 의해 분할)                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_obj_lock_acquire_time:              | 객체 잠금 획득하는 데 필요한시간  (모듈잠금 타입으로 파티션됨 )                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_get_snapshot_acquire_time:          | 스냅 샷 유효성 검사 기능에 필요한 시간 (partitioned by snapshot type,                  |
    |                  |                                          | record type, visibility result upon validation).                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Count_get_snapshot_retry:                | The number of retries to acquire MVCC snapshot (partitioned by module)                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_tran_complete_time:                 | 스냅샷 무효화 및  MVCCID 트랜잭션 커밋/롤백 시간                                       |
    |                  |                                          | (partitioned by module)                                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_get_oldest_mvcc_acquire_time:       | 가장 오랜된 MVCCID 를 획득 소용 시간  (partitioned by module)                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Count_get_oldest_mvcc_retry:             | 가장 오래된 MVCCID 를 획득하는 재시도 횟수   (partitioned by module)                   |    
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+

.. Note::  

    (*) : These statistics measure the non-MVCC operations or MVCC operations which are performed in-place (decided internally)


.. option:: -o, --output-file=FILE

    대상 데이터베이스 서버의 실행 통계 정보를 지정된 파일에 저장한다. ::

        cubrid statdump -o statdump.log testdb

.. option:: -c, --cumulative

    **-c** 옵션을 이용하여 대상 데이터베이스 서버의 누적된 실행 통계 정보를 출력할 수 있다.
    
    Num_data_page_fix_ext, Num_data_page_unfix_ext, Time_data_page_hold_acquire_time, Time_data_page_fix_acquire_time 정보는 이 옵션을 켜야만 출력되는데, 해당 정보들은 CUBRID 엔진 개발자를 위한 정보이므로 설명을 생략한다.
    
    **-i** 옵션과 결합하면, 지정된 시간 간격(interval)마다 실행 통계 정보를 확인할 수 있다. ::

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

.. note::

    각 상태 정보는 64비트 **INTEGER**\로 구성되어 있으며, 누적된 값이 한도를 넘으면 해당 실행 통계 정보가 유실될 수 있다.

.. _lockdb:

lockdb
------

**cubrid lockdb**\는 대상 데이터베이스에 대하여 현재 트랜잭션에서 사용되고 있는 잠금 정보를 확인하는 유틸리티이다. ::

    cubrid lockdb [<option>] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **lockdb**: 대상 데이터베이스에 대하여 현재 트랜잭션에서 사용되고 있는 잠금 정보를 확인하는 명령이다.

*   *database_name*: 현재 트랜잭션의 잠금 정보를 확인하는 데이터베이스 이름이다.

다음 예는 옵션 없이 testdb 데이터베이스의 잠금 정보를 화면에 출력한다. ::

    cubrid lockdb testdb

다음은 **cubrid lockdb**\에 대한 [option]이다.
    
.. program:: lockdb

.. option:: -o, --output-file=FILE

    데이터베이스의 잠금 정보를 output.txt로 출력한다. ::

        cubrid lockdb -o output.txt testdb

출력 내용
^^^^^^^^^

**cubrid lockdb**\ 의 출력 내용은 논리적으로 3개의 섹션으로 나뉘어져 있다.

*   서버에 대한 잠금 설정

*   현재 데이터베이스에 접속한 클라이언트들

*   객체 잠금 테이블의 내용

**서버에 대한 잠금 설정**

**cubrid lockdb** 출력 내용의 첫 번째 섹션은 데이터베이스 서버에 대한 잠금 설정이다.

::

    *** Lock Table Dump ***
     Lock Escalation at = 100000, Run Deadlock interval = 0

위에서 잠금 에스컬레이션 레벨은 100000레코드로, 교착 상태 탐지 간격은 0초로 설정되어 있다.

관련 시스템 파라미터인 **lock_escalation**\과 **deadlock_detection_interval**\에 대한 설명은 :ref:`lock-parameters` 를 참고한다.

**현재 데이터베이스에 접속한 클라이언트들**

**cubrid lockdb** 출력 내용의 두 번째 섹션은 데이터베이스에 연결된 모든 클라이언트의 정보를 포함한다. 이 정보에는 각각의 클라이언트에 대한 트랜잭션 인덱스, 프로그램 이름, 사용자 ID, 호스트 이름, 프로세스 ID, 격리 수준, 그리고 잠금 타임아웃 설정이 포함된다.

::

    Transaction (index 1, csql, dba@cubriddb|12854)
    Isolation COMMITTED READ
    Timeout_period : Infinite wait

위에서 트랜잭션 인덱스는 1이고, 프로그램 이름은 csql, 사용자 이름은 dba, 호스트 이름은 cubriddb, 클라이언트 프로세스 식별자는 12854, 격리 수준은 READ COMMITTED CLASSES AND READ UNCOMMITTED INSTANCES, 그리고 잠금 타임아웃은 무제한이다.

트랜잭션 인덱스가 0인 클라이언트는 내부적인 시스템 트랜잭션이다. 이것은 데이터베이스의 체크포인트 수행과 같이 특정한 시간에 잠금을 획득할 수 있지만 대부분의 경우 이 트랜잭션은 어떤 잠금도 획득하지 않을 것이다.

**cubrid lockdb** 유틸리티는 잠금 정보를 가져오기 위해 데이터베이스에 접속하기 때문에 **cubrid lockdb** 자체가 하나의 클라이언트이고 따라서 클라이언트의 하나로 출력된다.

**객체 잠금 테이블**

**cubrid lockdb** 출력 내용의 세 번째 섹션은 객체 잠금 테이블의 내용을 포함한다. 이것은 어떤 객체에 대해서 어떤 클라이언트가 어떤 모드로 잠금을 가지고 있는지, 어떤 객체에 대해서 어떤 클라이언트가 어떤 모드로 기다리고 있는지를 보여준다. 객체 잠금 테이블 결과물의 첫 부분에는 얼마나 많은 객체가 잠금되었는지가 출력된다. 

::

    Object lock Table:
        Current number of ojbects which are locked = 2001

**cubrid lockdb** 는 잠금을 획득한 각각의 객체에 대한 객체의 OID와 Object type, 테이블 이름을 출력한다. 추가적으로 객체에 대해서 잠금을 보유하고 있는 트랜잭션의 개수(Num holders), 잠금을 보유하고 있지만 상위 잠금으로 변환(예를 들어 **SCH_S_LOCK** 에서 **SCH_M_LOCK** 으로 잠금 변환)하지 못해 차단된 트랜잭션의 개수(Num blocked-holders), 객체의 잠금을 기다리는 다른 트랜잭션의 개수(*Num waiters*)가 출력된다. 그리고 잠금을 보유하고 있는 클라이언트 트랜잭션, 차단된 클라이언트 트랜잭션, 기다리는 클라이언트 트랜잭션의 리스트가 출력된다. Class에 대해서는 아니지만 Row에 관해서 MVCC정보 역시 출력된다. 

다음 예는 Object type이 instance of class, 즉 레코드인 경우, OID( O| 62| 5)인 객체에 대해서 트랜잭션 2가 **IX_LOCK** 을 가지고 있고, 트랜잭션 1이 **SCH_S_LOCK** 을 획득하고 있지만 트랜잭션 2가 **SCH_M_LOCK** 을 획득하고 있기 때문에 **SCH_M_LOCK** 으로 변환하지 못해 차단되었음을 보여준다. 그리고 트랜잭션 3은 **SCH_S_LOCK** 을 대기하고 있지만 트랜잭션 2가 **SCH_M_LOCK** 을 대기하고 있기 때문에 차단되었음을 보여준다.

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
                        Wait_for_secs = -1
    LOCK WAITERS :
        Tran_index = 3, Blocked_mode = S_LOCK
                        Start_waiting_at = Fri May 3 14:45:14 2002
                        Wait_for_secs = -1

다음 예는 **X_LOCK** 을 보유한 트랜잭션 1에 의해서 삽입된 객체 아이디가 ( 2 | 50 | 1)인 클래스의 인스턴스를 보여준다.  트랙잰션 1이 끝날때까지 차단된 트랜잭션 2 에 의해서 수정된 이 클래스는 유일한 인덱스와 키 값을 가진다.

::

   OID = 2| 50| 1
    Object type: instance of class ( 0| 62| 5) = athlete.
    MVCC info: insert ID = 6, delete ID = missing.
    Num holders = 1, Num blocked-holders= 1, Num waiters = 1
    LOCK HOLDERS :
        Tran_index =   1, Granted_mode =   X_LOCK, Count =   1
    LOCK WAITERS :
        Tran_index =   2, Blocked_mode = X_LOCK
                          Start_waiting_at = Wed Feb 3 14:45:14 2016
                          Wait_for_secs = -1

*Granted_mode* 는 현재 획득한 잠금의 모드를 의미하고 *Blocked_mode* 는 차단된 잠금의 모드를 의미한다. *Starting_waiting_at* 은 잠금을 요청한 시간을 의미하고 *Wait_for_secs* 는 잠금을 기다리는 시간을 의미한다. *Wait_for_secs* 의 값은 **lock_timeout** 시스템 파라미터에 의해 설정된다. 

Object type이 Class, 즉 테이블인 경우 *Nsubgranules* 가 출력되는데 이것은 해당 테이블 내의 특정 트랜잭션이 획득하고 있는 레코드 잠금과 키 잠금을 합한 개수이다.

::

    OID = 0| 62| 5
    Object type: Class = athlete
    Num holders = 2, Num blocked-holders= 0, Num waiters= 0
    LOCK HOLDERS:
    Tran_index = 3, Granted_mode = IS_LOCK, Count = 2, Nsubgranules = 0
    Tran_index = 1, Granted_mode = IX_LOCK, Count = 3, Nsubgranules = 1
    Tran_index = 2, Granted_mode = IS_LOCK, Count = 2, Nsubgranules = 1

.. _tranlist:

tranlist
--------

**cubrid tranlist**\는 대상 데이터베이스의 트랜잭션 정보를 확인하는 유틸리티로서, DBA 또는 DBA그룹 사용자만 수행할 수 있다. ::

    cubrid tranlist [options] database_name

옵션을 생략하면 각 트랜잭션에 대한 전체 정보를 출력한다. 

"cubrid tranlist demodb"는 "cubrid killtran -q demodb"와 비슷한 결과를 출력하나, 후자에 비해 "User name"과 "Host name"을 더 출력한다.
"cubrid tranlist -s demodb"는 "cubrid killtran -d demodb"와 동일한 결과를 출력한다.

다음은 tranlist 출력 결과의 예이다. 

::

    $ cubrid tranlist demodb

    Tran index          User name      Host name      Process id    Program name              Query time    Tran time       Wait for lock holder      SQL_ID       SQL Text
    --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
       1(ACTIVE)           public     test-server           1681    broker1_cub_cas_1               0.00         0.00                       -1     *** empty ***  
       2(ACTIVE)           public     test-server           1682    broker1_cub_cas_2               0.00         0.00                       -1     *** empty ***  
       3(ACTIVE)           public     test-server           1683    broker1_cub_cas_3               0.00         0.00                       -1     *** empty ***  
       4(ACTIVE)           public     test-server           1684    broker1_cub_cas_4               1.80         1.80                  3, 2, 1     e5899a1b76253   update ta set a = 5 where a > 0
       5(ACTIVE)           public     test-server           1685    broker1_cub_cas_5               0.00         0.00                       -1     *** empty ***  
    --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    SQL_ID: e5899a1b76253
    Tran index : 4
    update ta set a = 5 where a > 0

위의 예는 3개의 트랜잭션이 각각 INSERT문을 실행 중일 때 또 다른 트랜잭션에서 UPDATE문의 실행을 시도한다. 위에서 Tran index가 4인 update문은 3,2,1(Wait for lock holder)번의 트랜잭션이 종료되기를 대기하고 있다.

화면에 출력되는 질의문(SQL Text)은 질의 계획 캐시에 저장되어 있는 것을 보여준다. 질의 수행이 완료되면 **empty**\로 표시된다.

각 칼럼의 의미는 다음과 같다.

    *   Tran index: 트랜잭션 인덱스
    *   User name: 데이터베이스 사용자 이름
    *   Host name: 해당 트랜잭션이 수행되는 CAS의 호스트 이름
    *   Process id: 클라이언트 프로세스 ID
    *   Program name: 클라이언트 프로그램 이름
    *   Query time: 수행중인 질의의 총 수행 시간(단위: 초)
    *   Tran time: 현재 트랜잭션의 총 수행 시간(단위: 초)
    *   Wait for lock holder: 현재 트랜잭션이 락 대기중이면 해당 락을 소유하고 있는 트랜잭션의 리스트
    *   SQL_ID: SQL Text에 대한 ID. cubrid killtran 명령의 --kill-sql-id 옵션에서 사용될 수 있다.
    *   SQL Text: 수행중인 질의문(최대 30자)

"Tran index"에 보여지는 transaction 상태 메시지는 다음과 같다.
    
    *   ACTIVE : 활성
    *   RECOVERY : 복구중인 트랜잭션
    *   COMMITTED : 커밋완료되어 종료될 트랜잭션
    *   COMMITTING : 커밋중인 트랜잭션
    *   ABORTED : 롤백되어 종료될 트랜잭션
    *   KILLED : 서버에 의해 강제 종료 중인 트랜잭션

다음은 **cubrid tranlist**\ 에 대한 [options]이다.

.. program:: tranlist

.. option:: -u, --user=USER

    로그인할 사용자 ID. DBA및 DBA그룹 사용자만 허용한다.(기본값 : DBA)
    
.. option:: -p, --password=PASSWORD

    사용자 비밀번호
    
.. option:: -s, --summary

    요약 정보만 출력한다(질의 수행 정보 또는 잠금 관련 정보를 생략).
       
    ::
    
        $ cubrid tranlist -s demodb
        
        Tran index          User name      Host name      Process id      Program name
        -------------------------------------------------------------------------------
           1(ACTIVE)           public     test-server           1681 broker1_cub_cas_1
           2(ACTIVE)           public     test-server           1682 broker1_cub_cas_2
           3(ACTIVE)           public     test-server           1683 broker1_cub_cas_3
           4(ACTIVE)           public     test-server           1684 broker1_cub_cas_4
           5(ACTIVE)           public     test-server           1685 broker1_cub_cas_5
        -------------------------------------------------------------------------------

.. option:: --sort-key=NUMBER
 
    해당 NUMBER 위치의 칼럼에 대해 오름차순으로 정렬된 값을 출력한다.
    칼럼의 타입이 숫자인 경우는 숫자로 정렬되고, 그렇지 않은 경우 문자열로 정렬된다. 생략되면 "Tran index"에 대한 정렬값을 보여준다.

    다음은 네번째 칼럼인 "Process id"를 지정하여 정렬한 정보를 출력하는 예이다.
     
    ::
     
        $ cubrid tranlist --sort-key=4 demodb
     
        Tran index          User name      Host name      Process id    Program name              Query time    Tran time       Wait for lock holder      SQL_ID       SQL Text
        --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
           1(ACTIVE)           public     test-server           1681    broker1_cub_cas_1               0.00         0.00                       -1     *** empty ***
           2(ACTIVE)           public     test-server           1682    broker1_cub_cas_2               0.00         0.00                       -1     *** empty ***
           3(ACTIVE)           public     test-server           1683    broker1_cub_cas_3               0.00         0.00                       -1     *** empty ***
           4(ACTIVE)           public     test-server           1684    broker1_cub_cas_4               1.80         1.80                  3, 1, 2     e5899a1b76253   update ta set a = 5 where a > 0
           5(ACTIVE)           public     test-server           1685    broker1_cub_cas_5               0.00         0.00                       -1     *** empty ***
        --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        SQL_ID: e5899a1b76253
        Tran index : 4
        update ta set a = 5 where a > 0
        
.. option:: --reverse
 
    역순으로 정렬된 값을 출력한다.
 
    다음은 "Tran index"의 역순으로 정렬한 정보를 출력하는 예이다.
     
    ::
     
        Tran index          User name      Host name      Process id    Program name              Query time    Tran time     Wait for lock holder      SQL_ID       SQL Text
        ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
           5(ACTIVE)           public     test-server           1685    broker1_cub_cas_5               0.00         0.00                     -1     *** empty ***
           4(ACTIVE)           public     test-server           1684    broker1_cub_cas_4               1.80         1.80                3, 2, 1     e5899a1b76253   update ta set a = 5 where a > 0
           3(ACTIVE)           public     test-server           1683    broker1_cub_cas_3               0.00         0.00                     -1     *** empty ***
           2(ACTIVE)           public     test-server           1682    broker1_cub_cas_2               0.00         0.00                     -1     *** empty ***
           1(ACTIVE)           public     test-server           1681    broker1_cub_cas_1               0.00         0.00                     -1     *** empty ***
        ------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        SQL_ID: e5899a1b76253
        Tran index : 4
        update ta set a = 5 where a > 0

.. _killtran:

killtran
--------

**cubrid killtran**\은 대상 데이터베이스의 트랜잭션을 확인하거나 특정 트랜잭션을 강제 종료하는 유틸리티로서, **DBA** 사용자만 수행할 수 있다. ::

    cubrid killtran [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **killtran**: 지정된 데이터베이스에 대해 트랜잭션을 관리하는 명령어이다.

*   *database_name*: 대상 데이터베이스의 이름이다.

[options]에 따라 특정 트랜잭션을 지정하여 제거하거나, 현재 활성화된 트랜잭션을 화면 출력할 수 있다. 옵션이 지정되지 않으면, **-d** 옵션이 기본으로 적용되어 모든 트랜잭션을 화면 출력하며, cubrid tranlist 명령에 **-s** 옵션을 준 것과 동일하다.

::

    $ cubrid killtran demodb 
     
    Tran index      User name   Host name      Process id      Program name
    -------------------------------------------------------------------------------
       1(ACTIVE)          dba      myhost             664           cub_cas
       2(ACTIVE)          dba      myhost            6700              csql
       3(ACTIVE)          dba      myhost            2188           cub_cas
       4(ACTIVE)          dba      myhost             696              csql
       5(ACTIVE)       public      myhost            6944              csql
    -------------------------------------------------------------------------------

다음은 **cubrid killtran**\에 대한 [options]이다.

.. program:: killtran

.. option:: -i, --kill-transaction-index=ID1,ID2,ID3

    지정한 인덱스에 해당하는 트랜잭션을 제거한다.  쉼표(,)로 구분하여 제거하고자 하는 트랜잭션 ID 여러 개를 지정할 수 있다. 제거할 트랜잭션 리스트에 유효하지 않은 트랜잭션 ID가 지정되면 무시된다.::

        $ cubrid killtran -i 1,2 demodb
        Ready to kill the following transactions:

        Tran index          User name      Host name      Process id      Program name
        -------------------------------------------------------------------------------
           1(ACTIVE)              DBA         myhost           15771              csql
           2(ACTIVE)              DBA         myhost            2171              csql
        -------------------------------------------------------------------------------
        Do you wish to proceed ? (Y/N)y
        Killing transaction associated with transaction index 1
        Killing transaction associated with transaction index 2

.. option:: --kill-user-name=ID

    지정한 OS 사용자 ID에 해당하는 트랜잭션을 제거한다. ::

        cubrid killtran --kill-user-name=os_user_id demodb

.. option:: --kill-host-name=HOST

    지정한 클라이언트 호스트의 트랜잭션을 제거한다. ::

        cubrid killtran --kill-host-name=myhost demodb

.. option:: --kill-program-name=NAME

    지정한 이름의 프로그램에 해당하는 트랜잭션을 제거한다. ::

        cubrid killtran --kill-program-name=cub_cas demodb

.. option:: --kill-sql-id=SQL_ID
        
    지정한 SQL ID에 해당하는 트랜잭션을 제거한다. ::

        cubrid killtran --kill-sql-id=5377225ebc75a demodb

.. option:: -p, --dba-password=PASSWORD

    이 옵션 뒤에 오는 값은 **DBA**\ 의 암호이며 생략하면 프롬프트에서 입력해야 한다.

.. option:: -q, --query-exec-info

    cubrid tranlist 명령에서 "User name" 칼럼과 "Host name" 칼럼이 출력되지 않는다는 점만 다르다. :ref:`tranlist`\ 를 참고한다.

.. option:: -d, --display

    기본 지정되는 옵션으로 트랜잭션의 요약 정보를 출력한다. cubrid tranlist 명령의 -s 옵션을 지정하여 실행한 것과 동일한 결과를 출력한다. :option:`tranlist -s`\ 를 참고한다.

.. option:: -f, --force

    중지할 트랜잭션을 확인하는 프롬프트를 생략한다. ::

        cubrid killtran -f -i 1 demodb

.. _checkdb:

checkdb
-------

**cubrid checkdb** 유틸리티는 데이터베이스를 확인하기 위해 사용된다. **cubrid checkdb** 유틸리티를 사용하면 인덱스와 다른 데이터 구조를 확인하기 위해 데이터와 로그 볼륨의 내부적인 물리적 일치를 확인할 수 있다. 만일 **cubrid checkdb** 유틸리티의 실행 결과가 불일치로 나온다면 --**repair** 옵션으로 자동 수정을 시도해 보아야 한다.

::

    cubrid checkdb [options] database_name [table_name1 table_name2 ...]

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티

*   **checkdb**: 대상 데이터베이스에 대하여 데이터의 일관성(consistency)을 확인하는 명령

*   *database_name*: 일관성을 확인하거나 복구하려는 데이터베이스 이름

*   *table_name1 table_name2*: 일관성을 확인하거나 복구하려는 테이블 이름을 나열한다.

다음은 **cubrid checkdb**\에 대한 [options]이다.

.. program:: checkdb

.. option:: -S, --SA-mode

    서버 프로세스를 구동하지 않고 데이터베이스에 접근하는 독립 모드(standalone)로 작업하기 위해 지정되며, 인수는 없다. **-S** 옵션을 지정하지 않으면, 시스템은 클라이언트/서버 모드로 인식한다. ::

        cubrid checkdb -S demodb

.. option:: -C, --CS-mode

    서버 프로세스와 클라이언트 프로세스를 각각 구동하여 데이터베이스에 접근하는 클라이언트/서버 모드로 작업하기 위한 옵션이며, 인수는 없다. **-C** 옵션을 지정하지 않더라도 시스템은 기본적으로 클라이언트/서버 모드로 인식한다. ::

        cubrid checkdb -C demodb

.. option:: -r, --repair

    데이터베이스의 일관성에 문제가 발견되었을 때 복구를 수행한다. ::

        cubrid checkdb -r demodb

.. option:: --check-prev-link 
  
    인덱스의 앞을 가리키는 링크(previous link)에 오류가 있는지를 검사한다. 
     
    :: 
  
        $ cubrid checkdb --check-prev-link demodb 
  
.. option:: --repair-prev-link 
  
    인덱스의 앞을 가리키는 링크(previous link)에 오류가 있으면 복구한다. 
     
    :: 
  
        $ cubrid checkdb --repair-prev-link demodb

.. option:: -i, --input-class-file=FILE

    **-i** *FILE* 옵션을 지정하거나, 데이터베이스 이름 뒤에 테이블의 이름을 나열하여 일관성 확인 또는 복구 대상을 한정할 수 있다. 두 가지 방법을 같이 사용할 수도 있으며, 대상을 지정하지 않으면 전체 데이터베이스를 대상으로 일관성을 확인하거나 복구를 수행한다. 특정 대상이 지정되지 않으면 전체 데이터베이스가 일관성 확인  또는 복구의 대상이 된다. ::

        cubrid checkdb demodb tbl1 tbl2
        cubrid checkdb -r demodb tbl1 tbl2
        cubrid checkdb -r -i table_list.txt demodb tbl1 tbl2

    **-i** 옵션으로 지정하는 테이블 목록 파일은 공백, 탭, 줄바꿈, 쉼표로 테이블 이름을 구분한다. 다음은 테이블 목록 파일의 예로, t1부터 t10까지를 모두 일관성 확인 또는 복구를 위한 테이블로 인식한다. ::

        t1 t2 t3,t4 t5
        t6, t7 t8   t9
         
             t10

.. option:: --check-file-tracker 

    파일 트랙커(tracker)의 모든 파일의 페이지에 검사를 수행한다. 

.. option:: --check-heap 

    모든 힙 파일에 대해 검사를 수행한다. 

.. option:: --check-catalog 

    카탈로그 정보에 대한 일관성 검사를 수행한다.

.. option:: --check-btree 

    모든 B-트리 인덱스에 대한 유효 검사를 수행한다. 

.. option:: --check-class-name 

    클래스 이름 해시 테이블과 힙 파일로 부터 가져온 클래스 정보(class oid)의 일치 여부 검사를 수행한다. 

.. option:: --check-btree-entries 

    모든 B-트리 영역(entry)의 일관성 검사를 수행한다. 

.. option:: -I, --index-name=INDEX_NAME

    검사 대상 테이블에 대해 해당 옵션으로 명시한 인덱스가 유효한지 검사한다. 이 옵션을 사용하면 힙 파일에 대한 유효 검사는 하지 않는다. 
    이 옵션을 사용할 때는 하나의 테이블과 하나의 인덱스만을 허용하며, 테이블 이름을 입력하지 않거나 두 개 이상의 테이블 이름을 입력하면 에러를 반환한다. 

.. _diagdb:

diagdb
------

**cubrid diagdb** 유틸리티를 이용해 다양한 데이터베이스 내부 정보를 확인할 수 있다. **cubrid diagdb** 유틸리티가 제공하는 정보들은 현재 데이터베이스의 상태를 진단하거나 문제를 파악하는데 도움이 된다. ::

    cubrid diagdb [option] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **diagdb**: CUBRID에 저장되는 바이너리 형태의 파일 정보를 텍스트 형태로 출력하여 현재의 데이터베이스 저장 상태를 확인하고자 할 때 사용하는 명령어이다. 데이터베이스가 구동 정지 상태인 경우에만 정상적으로 수행된다. 전체를 확인하거나 옵션을 사용하여 파일 테이블, 파일 용량, 힙 용량, 클래스 이름, 디스크 비트맵을 선택해 확인할 수 있다.

*   *database_name*: 내부 정보를 확인하려는 데이터베이스 이름이다.

다음은 **cubrid diagdb**\에서 사용하는 [option]이다.

.. program:: diagdb

.. option:: -d, --dump-type=TYPE

    데이터베이스의 전체 파일에 대한 기록 상태를 출력할 때 출력 범위를 지정한다. 생략하면 기본값인 -1이 지정된다. ::

        cubrid diagdb -d 1 demodb

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
    | 6      | 디스크 비트맵 정보를 출력한다.     |
    +--------+------------------------------------+
    | 7      | 카탈로그 정보를 출력한다.          |
    +--------+------------------------------------+
    | 8      | 로그 정보를 출력한다.              |
    +--------+------------------------------------+
    | 9      | 힙(heap) 정보를 출력한다.          |
    +--------+------------------------------------+

.. _paramdump:

paramdump
---------

**cubrid paramdump** 유틸리티는 서버/클라이언트 프로세스에서 사용하는 파라미터 정보를 출력한다. ::

    cubrid paramdump [options] database_name

*   **cubrid**: CUBRID 서비스 및 데이터베이스 관리를 위한 통합 유틸리티이다.

*   **paramdump**: 서버/클라이언트 프로세스에서 사용하는 파라미터 정보를 출력하는 명령이다.

*   *database_name*: 파라미터 정보를 출력할 데이터베이스 이름이다.

다음은 **cubrid paramdump**\에서 사용하는 [options]이다.

.. program:: paramdump

.. option:: -o, --output-file=FILE

    데이터베이스의 서버/클라이언트 프로세스에서 사용하는 파라미터 정보를 지정된 파일에 저장하는 옵션이며, 파일은 현재 디렉터리에 생성된다. **-o** 옵션이 지정되지 않으면 메시지는 콘솔 화면에 출력한다. ::

        cubrid paramdump -o db_output demodb

.. option:: -b, --both

    데이터베이스의 서버/클라이언트 프로세스에서 사용하는 파라미터 정보를 콘솔 화면에 출력하는 옵션이며, **-b** 옵션을 사용하지 않으면 서버 프로세스의 파라미터 정보만 출력한다. ::

        cubrid paramdump -b demodb

.. option:: -S, --SA-mode

    독립 모드에서 서버 프로세스의 파라미터 정보를 출력한다. ::

        cubrid paramdump -S demodb

.. option:: -C, --CS-mode

    클라이언트-서버 모드에서 서버 프로세스의 파라미터 정보를 출력한다. ::

        cubrid paramdump -C demodb

HA 명령어
---------

**cubrid changemode** 유틸리티는 서버의 HA 모드 출력 또는 변경하는 유틸리티이다. 

**cubrid applyinfo** 유틸리티는 HA 환경에서 트랜잭션 로그 반영 정보를 확인하는 유틸리티이다.

자세한 사용법은 :ref:`cubrid-service-util` 을 참고한다.

.. _locale-command:

로캘 명령어
-----------

**cubrid genlocale** 유틸리티는 사용하고자 하는 로캘(locale) 정보를 컴파일하는 유틸리티이다. 이 유틸리티는 **make_locale.sh** (Windows는 **.bat**) 스크립트 내에서 실행된다.

**cubrid dumplocale** 유틸리티는 컴파일된 바이너리 로캘 파일을 사람이 읽을 수 있는 형태로 콘솔에 출력한다. 출력 값이 매우 클 수 있으므로, 리다이렉션을 이용하여 특정 파일로 저장할 것을 권장한다.

**cubrid synccolldb** 유틸리티는 데이터베이스와 로캘 라이브러리 사이의 콜레이션 불일치 여부를 체크하고, 불일치하는 경우 동기화한다.

자세한 사용법은 :ref:`locale-setting` 을 참고한다.

타임존 명령어
-----------------

**cubrid gen_tz** 유틸리티는 C 소스 코드에 tzdata 폴더에 포함 된 IANA 시간대 정보를 컴파일한다. 이 유틸리티는 ** make_tz.sh ** 스크립트 (**.bat**  Windows 용)에서 실행됩니다.

**cubrid dump_tz** 유틸리티는 콘솔에 사람이 읽을 수있는 형식으로 컴파일 된 CUBRID 시간대 라이브러리 파일을 덤프한다. 파일로 출력을 재지정하여 저장하는 것이 좋다.
