************************************
환경 변수 설정 및 CUBRID 서비스 시작
************************************

환경 변수 설정
==============

CUBRID를 사용하기 위해서는 다음의 환경 변수들이 설정되어 있어야 한다. 필요한 환경 변수들은 CUBRID 시스템을 설치하면 자동으로 설정되나 필요에 의해서 사용자가 적절히 변경할 수도 있다.

**CUBRID 환경 변수**

* **CUBRID** : CUBRID 시스템이 설치된 위치를 지정하는 기본 환경 변수이다. CUBRID 시스템에 포함된 모든 프로그램은 이 환경 변수를 참조하므로 정확히 설정되어 있어야 한다.

* **CUBRID_DATABASES** : **databases.txt** 파일의 위치를 지정하는 환경 변수이다. CUBRID 시스템은 **$CUBRID_DATABASES/databases.txt**
  파일에 데이터베이스 볼륨들의 절대 경로를 저장 관리한다. `databases.txt 파일 <#admin_admin_db_create_file_htm>`_ 을 참고한다.

* **CUBRID_LANG** : CUBRID 시스템이 데이터베이스의 로캘(언어+문자셋)과 오류 메시지를 출력할 때 사용할 언어를 지정하는 환경 변수이다. 제품 설치 시 초기 설정 값은 **en_US** 이며, 언어 이름 뒤에 문자셋을 생략하면 ISO-8859-1(.iso88591)이 기본으로 지정된다. 자세한 내용은 `언어 설정 <#gs_gs_must_langset_htm>`_ 을 참고한다.

* **CUBRID_TMP** : Linux용 CUBRID에서 cub_master 프로세스와 cub_broker 프로세스의 유닉스 도메인 소켓 파일을 저장하는 위치를 지정하는 환경 변수로, 지정하지 않으면 cub_master 프로세스는 **/tmp** 디렉터리에, cub_broker 프로세스는 **$CUBRID/var/CUBRID_SOCK** 디렉터리에 유닉스 도메인 소켓 파일을 저장한다(Windows용 CUBRID에서는 사용되지 않는다).

**CUBRID_TMP** 의 값에는 다음과 같은 제약 사항이 있다.

* unix socket의 path의 최대 크기가 108이므로 다음과 같이 **$CUBRID_TMP** 에 108보다 긴 경로를 입력하면 에러를 출력한다. ::

	$ export CUBRID_TMP=/home1/siwankim/cubrid=/tmp/123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

	$ cubrid server start apricot

	The $CUBRID_TMP is too long. (/home1/siwankim/cubrid=/tmp/123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789)

* 상대 경로를 입력하면 에러를 출력한다. ::

	$ export CUBRID_TMP=./var $ cubrid server start apricot

	The $CUBRID_TMP should be an absolute path. (./var)

**CUBRID_TMP** 는 CUBRID가 사용하는 유닉스 도메인 소켓의 기본 경로에서 발생할 수 있는 다음 문제를 회피하기 위해 사용할 수 있다.

* **/tmp** 는 주로 Linux에서 임시 파일을 저장하는 공간으로, 시스템 관리자가 이 공간을 주기적으로 임의 삭제하는 경우 유닉스 도메인 소켓까지 삭제될 수 있다. 이러한 경우 **$CUBRID_TMP** 를 **/tmp** 가 아닌 다른 경로로 설정한다.

* 유닉스 도메인 소켓 파일의 경로 최대 길이는 108인데, CUBRID의 설치 경로가 길어서 cub_broker용 유닉스 도메인 소켓 파일을 저장하는
  **$CUBRID/var/CUBRID_SOCK** 경로의 길이가 108을 넘는 경우 브로커를 구동할 수 없다. 따라서 **$CUBRID_TMP** 를 108이 넘지 않는 경로로 설정해야 한다.

이들 환경변수는 CUBRID를 설치하면서 이미 설정되었으나, 설정을 확인하기 위해서는 다음 명령을 사용할 수 있다.

* Linux ::

	% printenv CUBRID
	% printenv CUBRID_DATABASES
	% printenv CUBRID_LANG
	% printenv CUBRID_TMP

* Windows ::

	C:\> set CUBRID

**OS 환경 변수 및 Java 환경 변수**

* **PATH** : Linux 환경에서 PATH 환경 변수에는 CUBRID 시스템의 실행 파일이 있는 디렉터리인 $CUBRID/bin이 포함되어 있어야 한다.

* **LD_LIBRARY_PATH** : Linux 환경에서는 LD_LIBRARY_PATH (혹은 SHLIB_PATH나 LIBPATH) 환경 변수에 CUBRID 시스템의 동적 라이브러리 파일(libjvm.so)이 있는 디렉터리인 $CUBRID/lib이 포함되어 있어야 한다.

* **Path** : Windows 환경에서 Path 환경 변수에는 CUBRID 시스템의 실행 파일이 있는 디렉터리인 %CUBRID%\bin이 포함되어 있어야 한다.

* **JAVA_HOME** : CUBRID 시스템에서 자바 저장 프로시저 기능을 사용하기 위해서는 Java Runtime Environment (JRE) 1.6 이상 버전이 설치되어야 하고 JAVA_HOME 환경 변수에 해당 디렉터리가 지정되어야 한다. `Java 저장 함수/프로시저 사용을 위한 환경 설정 <#syntax_syntax_javasp_settings_ht_8446>`_ 을 참고한다.

**환경 변수 설정**

**Windows 환경인 경우**

Windows 환경에서 CUBRID 시스템을 설치한 경우는 설치 프로그램이 필요한 환경 변수를 자동으로 설정한다. [시스템 등록 정보] 대화 상자의 [고급] 탭에서 [환경 변수]를 클릭하면 나타나는 [환경 변수] 대화 상자에서 확인할 수 있으며, [편집] 버튼을 통해 변경할 수 있다. Windows?환경에서 환경 변수를 변경하는 방법에 대한 상세한 정보는 Windows?도움말을 참고한다.

.. image:: /images/image4.jpg

**Linux 환경인 경우**

Linux 환경에서 CUBRID 시스템을 설치한 경우는 설치 프로그램이 **.cubrid.sh** 혹은 **.cubrid.csh** 파일을 자동으로 생성하고 설치 계정의 셸 로그인 스크립트에서 자동으로 호출하도록 구성한다. 다음은 sh이나 bash 등을 사용하는 환경에서 생성된 **.cubrid.sh** 파일의 환경 변수 설정 내용이다. ::

	CUBRID=/home1/cub_user/CUBRID
	CUBRID_DATABASES=/home1/cub_user/CUBRID/databases
	CUBRID_LANG=en_US
	ld_lib_path=`printenv LD_LIBRARY_PATH`
	
	if [ "$ld_lib_path" = "" ]
	then
	    LD_LIBRARY_PATH=$CUBRID/lib
	else
	    LD_LIBRARY_PATH=$CUBRID/lib:$LD_LIBRARY_PATH
	fi
	
	SHLIB_PATH=$LD_LIBRARY_PATH
	LIBPATH=$LD_LIBRARY_PATH
	PATH=$CUBRID/bin:$CUBRID/cubridmanager:$PATH
	
	export CUBRID
	export CUBRID_DATABASES
	export CUBRID_LANG
	export LD_LIBRARY_PATH
	export SHLIB_PATH
	export LIBPATH
	export PATH

언어 설정
^^^^^^^^^

CUBRID 데이터베이스 관리 시스템은 사용할 언어를 **CUBRID_LANG** 환경 변수로 지정한다. 현재 **CUBRID_LANG** 환경 변수에 설정될 수 있는 값의 예는 다음과 같다.

*   **en_US** : 영어(기본값)
*   **ko_KR.euckr** : 한국어 EUC-KR 인코딩
*   **ko_KR.utf8** : 한국어 UTF-8 인코딩
*   **de_DE.utf8** : 독일어 UTF-8 인코딩
*   **es_ES.utf8** : 스페인어 UTF-8 인코딩
*   **fr_FR.utf8** : 프랑스어 UTF-8 인코딩
*   **it_IT.utf8** : 이태리어 UTF-8 인코딩
*   **ja_JP.utf8** : 일본어 UTF-8 인코딩
*   **km_KH.utf8** : 캄보디아어 UTF-8 인코딩
*   **tr_TR.utf8** : 터키어 UTF-8 인코딩
*   **vi_VN.utf8** : 베트남어 UTF-8 인코딩
*   **zh_CN.utf8** : 중국어 UTF-8 인코딩

CUBRID의 언어와 문자셋 설정은 데이터를 쓰거나 읽을 때 영향을 미치며, 프로그램들이 출력하는 메시지에도 해당 언어가 사용된다. 제품 설치 시
**CUBRID_LANG** 의 기본값은 **en_US** 이다.

문자셋, 로캘 및 콜레이션 설정과 관련된 자세한 내용은 `관리자 안내서 > 다국어 지원 <#admin_admin_i18n_intro_htm>`_ 을 참고한다.

CUBRID 서비스 시작
------------------

환경 변수 및 언어 설정을 완료한 후, CUBRID 서비스를 시작한다. 이에 대한 자세한 설명은 `서비스 등록 <#admin_admin_service_conf_registe_6298>`_
및 `서비스 구동 및 종료 <#admin_admin_service_conf_start_h_3702>`_ 를 참고한다.

**셸 명령어**

Linux 환경 또는 Windows 환경에서 아래와 같은 셸 명령어로 CUBRID 서비스를 시작하고, 설치 패키지에 포함된 demodb를 구동할 수 있다. ::

	% cubrid service start

	@ cubrid master start
	++ cubrid master start: success

	@ cubrid broker start
	++ cubrid broker start: success

	@ cubrid manager server start
	++ cubrid manager server start: success

	% cubrid server start demodb

	@ cubrid server start: demodb

	This may take a long time depending on the amount of recovery works to do.

	CUBRID 9.0

	++ cubrid server start: success

	@ cubrid server status

	Server demodb (rel 9.0, pid 31322)

**CUBRIDService 또는 CUBRID Service Tray**

Windows 환경에서는 다음과 같은 방법으로 CUBRID 서비스를 시작하거나 중지할 수 있다.

*   [제어판] > [성능 및 유지 관리] > [관리도구] > [서비스]에 등록된 CUBRIDService를 선택하여 시작하거나 중지한다.

	.. image:: /images/image5.jpg

*   시스템 트레이에서 CUBRID Service Tray를 마우스 오른쪽 버튼으로 클릭한 후, CUBRID를 시작하려면 [Service Start]를 선택하고 중지하려면 [Service Stop]을 선택한다. 시스템 트레이에서 [Service Start]/[Service Stop] 메뉴를 선택하면, 명령어 프롬프트 창에서
    **cubrid service start** / **cubrid service stop** 을 실행했을 때와 같은 동작을 수행하며, **cubrid.conf** 의 **service** 파라미터에 설정한 프로세스들을 구동/중지한다.

*   CUBRID가 실행 중일 때 CUBRID 서비스 트레이에서 [Exit]를 선택하면, 해당 서버에서 실행 중인 모든 서비스와 프로세스가 중지되므로 주의한다.

.. note::

	CUBRID 서비스 트레이를 통해 CUBRID 관련 프로세스를 시작/종료하는 작업은 관리자 권한(SYSTEM)으로 수행되고, 셸?명령어로?시작/종료하는 작업은 로그인한 사용자 권한으로 수행된다. Windows Vista 이상 버전의 환경에서 셸?명령어로 CUBRID 프로세스가 제어되지 않는 경우, 명령 프롬프트 창을 관리자 권한으로 실행([시작] > [모든 프로그램] > [보조 프로그램] > [명령 프롬프트]를 마우스 오른쪽 버튼으로 클릭하여 [관리자 권한으로 실행] 선택)하거나 CUBRID 서비스 트레이를 이용해서 해당 작업을 수행할 수 있다.
	CUBRID 서버 프로세스가 모두 중단되면, CUBRID Service Tray 아이콘이 회색으로 변한다.

**데이터베이스 생성**

데이터베이스 볼륨 및 로그 볼륨이 위치할 디렉터리에서 **cubrid createdb** 유틸리티를 실행하여 데이터베이스를 생성할 수 있다.
**--db-volume-size**, **--log-volume-size** 와 같은 별도의 옵션을 지정하지 않으면 기본적으로 범용 볼륨(generic volume) 512MB, 활성 로그(active log) 512MB, 백그라운드 보관 로그(backgroud archive log) 512MB, 총 1.5GB의 볼륨 파일이 생성된다. ::

	% cd testdb
	% cubrid createdb testdb
	% ls -l

	-rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb
	-rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb_lgar_t
	-rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb_lgat
	-rw------- 1 cubrid dbms       176 Jan 11 15:04 testdb_lginf
	-rw------- 1 cubrid dbms       183 Jan 11 15:04 testdb_vinf

위에서 testdb는 범용 볼륨 파일, testdb_lgar_t는 백그라운드 보관 로그 파일, testdb_lgat는 활성 로그 파일, testdb_lginf는 로그 정보 파일, testdb_vinf는 볼륨 정보 파일이다.

볼륨에 대한 자세한 정보는 `데이터베이스 볼륨 구조 <#intro_intro_arch_volume_htm>`_ 를 참고하고, 볼륨 생성에 대한 자세한 정보는
`데이터베이스 볼륨 생성 <#admin_admin_db_create_create_htm>`_ 을 참고한다. 볼륨을 생성할 때에는 **cubrid addvoldb** 유틸리티를 이용하여 용도별로 볼륨을 추가하는 것을 권장하며, 이에 대한 자세한 정보는 `데이터베이스 볼륨 추가 <#admin_admin_db_addvol_htm>`_ 를 참고한다.

**데이터베이스 시작**

데이터베이스 프로세스를 시작하려면 **cubrid** 명령어를 이용한다. ::

	% cubrid server start testdb

앞에서 설명한 CUBRID 서비스 시작(**cubrid service start**) 시 *testdb* 가 같이 시작되게 하려면, **cubrid.conf** 파일의 **server** 파라미터에 *testdb* 를 설정한다. ::

	% vi cubrid.conf

	[service]

	service=server,broker,manager
	server=testdb

	...
