설치와 실행
===========

Linux에서의 설치와 실행
-----------------------

설치시 확인 사항
^^^^^^^^^^^^^^^^

Linux 버전의 CUBRID 데이터베이스를 설치하기 전에 다음 사항을 점검한다.

* 운영체제 버전

  운영체제 버전에 상관 없이 glibc 2.3.4 버전 이상만 지원한다.
  glibc 버전은 다음과 같은 방법으로 확인한다. ::
	  
	% rpm -q glibc
		
* 64비트 여부 

  CUBRID 2008 R2.0 버전부터 32비트 버전과 64비트 버전을 각각 지원한다.
  Linux버전은 다음과 같은 방법으로 확인한다. ::
	  
	% uname -a
	Linux host_name 2.6.18-53.1.14.el5xen #1 SMP Wed Mar 5 12:08:17 EST 2008 x86_64 x86_64 x86_64 GNU/Linux

  32비트 Linux에서는 CUBRID 32비트 버전을, 64비트 Linux에서는 CUBRID 64비트 버전을 설치한다. 
  설치할 추가 라이브러리는 다음과 같다.
  
  * Curses Library (rpm -q ncurses)
  * gcrypt Library (rpm -q libgcrypt)
  * stdc++ Library (rpm -q libstdc++)

* 메모리 여유 공간 

  1GB 이상을 권장한다.

* 디스크 여유 공간

  2GB 이상을 권장한다(초기 설치 시 500MB, 기본 옵션으로 데이터베이스 생성 시 1.5GB 필요).

* 필요 소프트웨어

  CUBRID 매니저 및 자바 저장 프로시저를 사용하려면 Java Runtime Environment (JRE) 1.6 이상 버전이 설치되어 있어야 한다.

CUBRID 설치
^^^^^^^^^^^

설치 프로그램은 바이너리를 포함한 셸 스크립트로 되어 있어 자동으로 설치할 수 있다. 다음은 리눅스에서 "CUBRID-9.0.0.0470-linux.x86_64.sh" 파일을 이용하여 CUBRID를 설치하는 예제이다. ::

	$ sh CUBRID-9.0.0.0470-linux.x86_64.sh
	Do you agree to the above license terms? (yes or no) : yes
	Do you want to install this software(CUBRID) to the default(/home1/cub_user/CUBRID) directory? (yes or no) [Default: yes] : yes
	Install CUBRID to '/home1/cub_user/CUBRID' ...
	In case a different version of the CUBRID product is being used in other machines, 
	please note that the CUBRID 9.0 servers are only compatible with the CUBRID 9.0 clients and vice versa.
	Do you want to continue? (yes or no) [Default: yes] : yes
	Copying old .cubrid.sh to .cubrid.sh.bak ...

	CUBRID has been successfully installed.

	demodb has been successfully created.

	If you want to use CUBRID, run the following commands
	$ . /home1/cub_user/.cubrid.sh
	$ cubrid service start

위의 예제와 같이 다운로드한 파일(CUBRID-9.0.0.0470-linux.x86_64.sh)을 설치한 후, CUBRID 데이터베이스를 사용하기 위해서는 CUBRID 관련 환경 정보를 설정해야 한다. 이 설정은 해당 터미널에 로그인할 때 자동 설정되도록 지정되어 있으므로 설치 후 최초 한 번만 수행하면 된다. ::

	$ . /home1/cub_user/.cubrid.sh

CUBRID가 설치 완료되면 CUBRID 매니저 서버와 브로커를 다음과 같이 구동시킬 수 있다. ::

	$ cubrid service start

cubrid service를 구동시킨 후 정상적으로 구동되었는지 확인하려면 다음과 같이 grep으로 cub_* 프로세스들이 구동되어 있는지를 확인한다. ::

	$ ps -ef | grep cub_
	cub_user 15200 1 0 18:57 ? 00:00:00 cub_master
	cub_user 15205 1 0 18:57 pts/17 00:00:00 cub_broker
	cub_user 15210 1 0 18:57 pts/17 00:00:00 query_editor_cub_cas_1
	cub_user 15211 1 0 18:57 pts/17 00:00:00 query_editor_cub_cas_2
	cub_user 15212 1 0 18:57 pts/17 00:00:00 query_editor_cub_cas_3
	cub_user 15213 1 0 18:57 pts/17 00:00:00 query_editor_cub_cas_4
	cub_user 15214 1 0 18:57 pts/17 00:00:00 query_editor_cub_cas_5
	cub_user 15217 1 0 18:57 pts/17 00:00:00 cub_broker
	cub_user 15222 1 0 18:57 pts/17 00:00:00 broker1_cub_cas_1
	cub_user 15223 1 0 18:57 pts/17 00:00:00 broker1_cub_cas_2
	cub_user 15224 1 0 18:57 pts/17 00:00:00 broker1_cub_cas_3
	cub_user 15225 1 0 18:57 pts/17 00:00:00 broker1_cub_cas_4
	cub_user 15226 1 0 18:57 pts/17 00:00:00 broker1_cub_cas_5
	cub_user 15229 1 0 18:57 ? 00:00:00 cub_auto start
	cub_user 15232 1 0 18:57 ? 00:00:00 cub_js start

RPM으로 CUBRID 설치
^^^^^^^^^^^^^^^^^^^

CentOS5 환경에서 생성한 RPM?파일을 사용하여 CUBRID를 설치할 수 있으며, 일반적인 RPM?유틸리티와 동일한 방법으로 설치하고 삭제할 수 있다. 설치하면 새로운 시스템 그룹(cubrid) 및 사용자 계정(cubrid)이 생성되며, 설치 후에는 cubrid 사용자 계정으로 로그인하여 CUBRID 서비스를 시작해야 한다. ::

	$ rpm -Uvh CUBRID-9.0.0.0470-el5.x86_64.rpm

RPM을 실행하면 CUBRID는 cubrid 홈 디렉터리(/opt/cubrid)에 설치되고, CUBRID 관련 환경 설정 파일(cubrid.[c]sh)이 /etc/profile.d 디렉터리에 설치된다. 단, demodb는 자동으로 설치되지 않으므로 /opt/cubrid/demo/make_cubrid_demo.sh를 실행하여야 한다. CUBRID가 설치 완료되면 cubrid 사용자 계정으로 로그인하여 CUBRID 서비스를 다음과 같이 시작한다. ::

	$ cubrid service start

.. note::

	RPM으로 설치할 때에는 의존성을 꼭 확인해야 한다. 의존성을 무시(--nodeps)하고 설치하면 실행되지 않을 수 있다.
	RPM을 삭제하더라도 cubrid 사용자 계정 및 설치 후 생성한 데이터베이스는 보관되므로, 더 이상 필요하지 않은 경우 수동으로 삭제해야 한다.

.. note::

	**service나 chkconfig 명령을 사용하는 방법**
	
	SH 패키지나 RPM 패키지로 CUBRID를 설치했다면 $CUBRID/share/init.d 디렉터리에 cubrid라는 스크립트가 포함되어 있다. 이 파일 안의 **CUBRID_USER** 라는 환경 변수 값을 CUBRID를 설치한 Linux 계정으로 변경한 후, /etc/init.d에 등록하면 service나 chkconfig 명령을 사용할 수 있다.

Fedora/CentOS에서 CUBRID 설치
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

yum 명령어를 사용하여 CUBRID를 설치하려면, CUBRID 패키지의 위치를 알아야 한다. 운영체제에 따라 다음 주소로 이동하여 자신의 운영체제에 맞는 파일을 선택한다.	`http://www.cubrid.org/yum_repository <http://www.cubrid.org/yum_repository>`_

예를 들어, 운영체제가 Fedora 16이면 다음과 같은 명령을 실행한다. fc16은 Fedora 16을 의미한다. ::

	$ rpm -i http://yumrepository.cubrid.org/cubrid_repo_settings/9.0.0/cubridrepo-9.0.0-1.fc16.noarch.rpm

운영체제가 CentOS 6.2이면 다음과 같은 명령을 실행한다. el6.2는 CentOS 6.2를 의미한다. ::

	$ rpm -i http://yumrepository.cubrid.org/cubrid_repo_settings/9.0.0/cubridrepo-9.0.0-1.el6.2.noarch.rpm

위의 명령을 실행하면 원하는 CUBRID 패키지를 설치할 수 있다. CUBRID 최신 버전을 설치하려면 다음 명령을 실행한다. ::

	$ yum install cubrid

이전 버전을 설치하려면 다음과 같이 명령에 버전을 포함해야 한다. ::

	$ yum install cubrid-9.0.0

설치를 완료하면 CUBRID 경로를 포함한 환경 변수들을 설정하고, 이를 시스템에 적용한다.

Ubuntu에서 CUBRID 설치
^^^^^^^^^^^^^^^^^^^^^^

Ubuntu에서 apt-get 명령어를 사용하여 CUBRID를 설치하려면, 먼저 CUBRID 저장소를 추가하고, apt 인덱스를 업데이트한다. ::

	$ sudo add-apt-repository ppa:cubrid/cubrid
	$ sudo apt-get update

CUBRID 최신 버전을 설치하려면 다음 명령을 실행한다. ::

	$ sudo apt-get install cubrid

이전 버전을 설치하려면 다음과 같이 명령에 버전을 포함해야 한다. ::

	$ sudo apt-get install cubrid-9.0.0

설치를 완료하면 CUBRID 경로를 포함한 환경 변수들을 설정하고, 이를 시스템에 적용한다.

CUBRID 업그레이드
^^^^^^^^^^^^^^^^^

다른 버전의 CUBRID가 설치된 디렉터리를 CUBRID를 설치할 디렉터리로 지정하면, 해당 디렉터리가 존재하는 것을 알리고 덮어쓸 것인지 확인한다.
**no** 를 입력하면 설치가 중단된다. ::

	Directory '/home1/cub_user/CUBRID' exist!
	If a CUBRID service is running on this directory, it may be terminated abnormally.
	And if you don't have right access permission on this directory(subdirectories or files), install operation will be failed.
	Overwrite anyway? (yes or no) [Default: no] : yes

CUBRID를 설치하고 설정 파일을 구성할 때 기존의 설정 파일을 그대로 사용할 것인지, 새 설정 파일을 사용할 것인지 확인한다.
**yes** 를 입력하면 기존의 설정 파일을 확장자가 .bak인 백업 파일로 보관한다. ::

	The configuration file (.conf or .pass) already exists. Do you want to overwrite it? (yes or no) : yes

환경 설정
^^^^^^^^^

서비스 포트 등 사용자 환경에 맞춰 설정을 변경하려면 **$CUBRID/conf** 디렉터리에서 설정 파일의 파라미터를 수정한다. 
자세한 내용은 `Windows에서의 설치와 실행 <#gs_gs_install_windows_htm>`_ 의 환경 설정을 참고한다.

CUBRID 인터페이스 설치
^^^^^^^^^^^^^^^^^^^^^^

CCI, JDBC, PHP, ODBC, OLE DB, ADO.NET, Ruby, Python 등의 인터페이스 모듈은 `http://www.cubrid.org/wiki_apis <http://www.cubrid.org/wiki_apis>`_ 에서 최신 정보를 확인할 수 있고 관련 파일을 내려받아 설치할 수 있다.

CUBRID 도구 설치
^^^^^^^^^^^^^^^^

CUBRID 매니저, CUBRID 쿼리 브라우저 등의 도구는 `http://www.cubrid.org/wiki_tools <http://www.cubrid.org/wiki_tools>`_
에서 최신 정보를 확인할 수 있고 관련 파일을 내려받아 설치할 수 있다.

Windows에서의 설치와 실행
-------------------------

설치 시 확인 사항
^^^^^^^^^^^^^^^^^

Windows 버전의 CUBRID 데이터베이스를 설치하기 전에 다음 사항을 점검한다.

* 64비트 여부

  CUBRID 2008 R2.0 버전부터 32비트 버전과 64비트 버전을 각각 지원한다. [내 컴퓨터] > [시스템 등록 정보] 창을 활성화하여 Windows 버전 비트를 확인할 수 있다. 32비트 Windows에서는 CUBRID 32비트 버전을 설치하고, 64비트 Windows에서는 CUBRID 64비트 버전을 설치한다.
	  
* 메모리 여유 공간 

  1GB 이상을 권장한다.
	 
* 디스크 여유 공간 

  2GB 이상을 권장한다(초기 설치 시 500MB, 기본 옵션으로 데이터베이스 생성 시 1.5GB 필요).
	  
* 필요 소프트웨어

  CUBRID 매니저 및 자바 저장 프로시저를 사용하려면 Java Runtime Environment (JRE) 1.6 이상 버전이 설치되어 있어야 한다.

시스템을 시작할 때 CUBRID Service Tray가 자동으로 구동되지 않는다면 다음 사항을 확인하도록 한다.

* [제어판] > [관리 도구] > [서비스]의 Task Scheduler가 시작되어 있는지 확인하고, 그렇지 않으면 Task Scheduler를 시작한다.

* [제어판] > [관리 도구] > [작업 스케줄러]에 CUBRID Service Tray가 등록되어 있는지 확인하고, 그렇지 않으면 CUBRID Service Tray를 등록한다.

설치 유형 선택
^^^^^^^^^^^^^^

* **전체 설치**

  CUBRID 서버와 명령행 도구 및 인터페이스 드라이버(OLEDB Provider, ODBC, JDBC, C API)가 모두 설치된다.

* **인터페이스 드라이버 설치**

  인터페이스 드라이버(OLEDB Provider, ODBC, JDBC, C API)만 설치된다. CUBRID 데이터베이스 서버가 설치된 컴퓨터에 원격 접근하여 개발하는 경우, 이 설치 유형을 선택할 수 있다.

CUBRID 업그레이드
^^^^^^^^^^^^^^^^^

이전 버전의 CUBRID가 이미 설치된 환경에 새로운 버전의 CUBRID를 설치하는 경우, 시스템 트레이에서 [CUBRID Service Tray] > [Exit]를 선택하여 운영 중인 서비스를 종료한 후 이전 버전의 CUBRID를 제거해야 한다. "데이터베이스와 설정 파일을 모두 삭제하겠습니까?"라고 묻는 대화 상자가 나타나면, 이전 버전의 데이터베이스가 삭제되지 않도록?[아니오]를 클릭한다.

이전 버전의 데이터베이스를 새로운 버전으로 마이그레이션하는 작업은 `데이터베이스 마이그레이션 <#admin_admin_migration_migration__1472>`_
을 참고한다.

환경 설정
^^^^^^^^^

서비스 포트 등 사용자 환경에 맞춰 설정을 변경하려면 **%CUBRID%\conf** 디렉터리에서 다음 설정 파일의 파라미터 값을 변경한다. 방화벽이 설정되어 있다면 CUBRID에서 사용하는 포트들을 열어두어야(open) 한다.

+------------------------+---------------------------------------------------------------------------------------------------------------+
| **구분**               | **내용**                                                                                                      |
+------------------------+---------------------------------------------------------------------------------------------------------------+
| **cm.conf**            | CUBRID 매니저용 설정 파일이다.                                                                                |
|                        | **cm_port** 는 매니저 서버 프로세스가 사용하는 포트로 기본값은 **8001**                                       |
|                        | 이며, 설정된 포트와 설정된 포트 번호+1 두 개의 포트가 사용된다.                                               |
|                        | 즉, 8001 포트를 설정하면 8001, 8002 두 개의 포트가 사용된다. 자세한 내용은                                    |
|                        | `cubrid 매니저 매뉴얼 <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual_kr>`_                     |
|                        | 을 참고한다.                                                                                                  |
+------------------------+---------------------------------------------------------------------------------------------------------------+
| **cubrid.conf**        | 서버 설정용 파일로, 운영하려는 데이터베이스의 메모리, 동시 사용자 수에 따른 스레드 수,                        |
|                        | 브로커와 서버 사이의 통신 포트 등을 설정한다.                                                                 |
|                        | **cubrid_port_id** 는 마스터 프로세스가 사용하는 포트로, 기본값은 *1523** 이다.                               |
|                        | 자세한 내용은 `시스템 파라미터 <#pm_pm_db_setting_htm>`_ 를 참조한다.                                         |
+------------------------+---------------------------------------------------------------------------------------------------------------+
| **cubrid_broker.conf** | 브로커 설정용 파일로, 운영하려는 브로커가 사용하는 포트, 응용서버(CAS) 수, SQL LOG 등을 설정한다.             |
|                        | **BROKER_PORT**                                                                                               |
|                        | 는 브로커가 사용하는 포트이며, 실제 JDBC와 같은 드라이버에서 보는 포트는 해당 브로커의 포트이다.              |
|                        | **APPL_SERVER_PORT**                                                                                          |
|                        | 는 Windows에서만 추가하는 파라미터로, 브로커 응용 서버(CAS)가 사용하는 포트이다. 기본값은                     |
|                        | **BROKER_PORT**                                                                                               |
|                        | +1이다.                                                                                                       |
|                        | **APPL_SERVER_PORT**                                                                                          |
|                        | 값을 기준으로 1씩 더한 포트들이 CAS 개수만큼 사용된다. 자세한 내용은                                          |
|                        | `브로커별 파라미터 <#pm_pm_broker_one_htm>`_                                                                  |
|                        | 를 참조한다.                                                                                                  |
|                        | **CCI_DEFAULT_AUTOCOMMIT**                                                                                    |
|                        | 브로커 파라미터는 2008 R4.0부터 지원하기 시작했고, 이때 기본값은                                              |
|                        | **OFF**                                                                                                       |
|                        | 였다가 2008 R4.1부터는 기본값이                                                                               |
|                        | **ON**                                                                                                        |
|                        | 으로 바뀌었다. 따라서 2008 R4.0에서 2008 R4.1 이상 버전으로 업그레이드하는 사용자는 이 값을 OFF로 바꾸거나,   | 
|                        | 응용 프로그램의 함수에서 자동 커밋 모드를 OFF로 설정해야 한다.                                                |
+------------------------+---------------------------------------------------------------------------------------------------------------+

CUBRID 인터페이스 설치
^^^^^^^^^^^^^^^^^^^^^^

JDBC, PHP, ODBC, OLE DB 등 인터페이스 모듈은 `http://www.cubrid.org/wiki_apis <http://www.cubrid.org/wiki_apis>`_
에서 최신 정보를 확인할 수 있고 관련 파일을 내려받아 설치할 수 있다.

CUBRID 도구 설치
^^^^^^^^^^^^^^^^

CUBRID 매니저, CUBRID 쿼리 브라우저 등의 도구는 `http://www.cubrid.org/wiki_tools <http://www.cubrid.org/wiki_tools>`_
에서 최신 정보를 확인할 수 있고 관련 파일을 내려받아 설치할 수 있다.