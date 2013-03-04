.. _install-execute:

설치와 실행
===========

지원 플랫폼 및 설치 권장 사양
-----------------------------

CUBRID가 지원하는 플랫폼과 설치를 위한 하드웨어/소프트웨어 요구 사항은 아래 표와 같다.

+--------------------------------------------------------------+------------------+----------------------+--------------------------+
| 지원 플랫폼                                                  | 요구되는 메모리  | 요구되는 디스크 공간 | 필요 소프트웨어          |
+==============================================================+==================+======================+==========================+
| * Windows 32/64 Bit XP, 2003, Vista, Windows 7               | 1G 이상          | 2G 이상(\*)          | JRE 또는 JDK 1.6 이상,   |
|                                                              |                  |                      | Java 저장 프로시저를     |
| * Linux 계열 32/64 Bit(Linux kernel 2.4 및 glibc 2.3.4 이상) |                  |                      | 사용하는 경우 필요       |
+--------------------------------------------------------------+------------------+----------------------+--------------------------+

(\*): 처음 설치 시 약 500MB의 디스크 공간이 필요하며, 하나의 DB를 기본 옵션으로 생성할 경우 약 1.5GB의 디스크 공간이 필요하다.

2008 R4.0부터는 CUBRID 패키지 설치 시 CUBRID 매니저 클라이언트가 같이 설치되지 않는다. 따라서 CUBRID 매니저를 사용하려면 이를 추가로 설치해야 한다. CUBRID 설치 패키지는 http://ftp.cubrid.org 에서 받을 수 있다.

CUBRID 매니저 및 CUBRID 쿼리 브라우저 설치 패키지와 JDBC, PHP, ODBC, OLE DB 등의 드라이버들도 http://ftp.cubrid.org 에서 받을 수 있다.

CUBRID 엔진, 사용 도구 및 드라이버에 대한 자세한 정보는 http://www.cubrid.org 를 참고한다.


버전 호환성
-----------

**응용 프로그램의 호환성**

*   2008 R2.0 또는 그 이상 버전에서 JDBC, PHP, CCI API 등을 사용하는 응용 프로그램은 CUBRID 9.1 DB에 접근할 수 있다. 다만, JDBC, PHP, CCI 인터페이스에 추가/개선된 기능을 사용하기 위해서는 CUBRID 9.1 버전의 라이브러리를 링크하거나 드라이버를 사용해야 한다.

*   새로운 예약어 추가 및 일부 질의에 대한 스펙 변경으로 인해 질의 결과가 이전 버전과 다를 수 있으므로 주의한다.

*   2008 R3.0 이하 버전에서 GLO 클래스를 이용하여 개발된 응용은 BLOB, CLOB 타입에 맞는 응용 및 스키마로 변환하여 사용해야 한다.

**CUBRID 매니저의 호환성**

*   CUBRID 매니저는 CUBRID 2008 R2.1 이상 버전의 서버에 대해서 하위 호환성을 보장하며, 각 서버 버전과 일치하는 CUBRID JDBC 드라이버를 사용한다. 하지만 CUBRID 매니저에서 제공하는 모든 기능을 제대로 사용하기 위해서는 CUBRID 서버 버전보다 높은 버전의 CUBRID 매니저를 사용해야 한다. CUBRID JDBC 드라이버는 CUBRID 설치 시 $CUBRID/jdbc 디렉터리에 포함되어 있다(Linux 환경에서 $CUBRID는 Windows 환경에서는 %CUBRID% 형식으로 사용됨).

*   CUBRID 매니저의 Bit 버전과 JRE의 Bit 버전은 서로 동일해야 한다. 예를 들어, 64Bit 버전 DB 서버라도 CUBRID Manager 32Bit 버전을 사용한다면 JRE 또는 JDK 32Bit 버전을 설치해야 한다.

*   CUBRID 2008 R2.2 이상 버전의 드라이버는 CUBRID 매니저에 기본으로 내장되어 있으며, cubrid.org에서 별도로 받을 수도 있다.

.. note:: 9.0 Beta 버전 사용자는 드라이버, 브로커, DB 서버 모두를 반드시 업그레이드해야 하며, DB 볼륨이 9.1과 호환되지 않으므로 반드시 데이터 마이그레이션을 해야 한다.
    업그레이드 및 데이터 마이그레이션은 :doc:`/upgrade` 를 참고한다.

상호 운용성
-----------

*   CUBRID DB 서버와 브로커 서버(또는 CUBRID SHARD 서버)를 분리하여 운영하는 경우, 서버 장비의 운영 체제가 다르더라도 상호 운용성을 보장한다. 단, DB 서버의 Bit 버전과 브로커 서버의 Bit 버전은 서로 동일해야 한다. 예를 들어, Linux용 64Bit 버전 DB 서버는 Windows용 64Bit 버전 브로커 서버와 상호 운용이 가능하지만, 32Bit 버전 브로커 서버와는 상호 운용이 불가능하다.

    DB 서버와 브로커 서버 사이의 관계에 대한 설명은 :doc:`intro` 를 참고한다. CUBRID SHARD에 대한 설명은 :doc:`admin/shard` 를 참고한다.

*   CUBRID DB 서버와 브로커 서버(또는 CUBRID SHARD 서버)를 분리하여 운영하는 경우, DB 서버와 브로커 서버의 시스템 로캘은 동일해야 한다. 예를 들어 DB 서버의 CUBRID_CHARSET이 en_US.utf8이면 브로커 서버의 CUBRID_CHARSET도 en_US.utf8이어야 한다.

Linux에서의 설치와 실행
-----------------------

**설치 시 확인 사항**

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

**CUBRID 설치**

    설치 프로그램은 바이너리를 포함한 셸 스크립트로 되어 있어 자동으로 설치할 수 있다. 다음은 리눅스에서 "CUBRID-9.1.0.0201-linux.x86_64.sh" 파일을 이용하여 CUBRID를 설치하는 예제이다. ::

        $ sh CUBRID-9.1.0.0201-linux.x86_64.sh
        Do you agree to the above license terms? (yes or no) : yes
        Do you want to install this software(CUBRID) to the default(/home1/cub_user/CUBRID) directory? (yes or no) [Default: yes] : yes
        Install CUBRID to '/home1/cub_user/CUBRID' ...
        In case a different version of the CUBRID product is being used in other machines, 
        please note that the CUBRID 9.1 servers are only compatible with the CUBRID 9.1 clients and vice versa.
        Do you want to continue? (yes or no) [Default: yes] : yes
        Copying old .cubrid.sh to .cubrid.sh.bak ...

        CUBRID has been successfully installed.

        demodb has been successfully created.

        If you want to use CUBRID, run the following commands
        $ . /home1/cub_user/.cubrid.sh
        $ cubrid service start

    위의 예제와 같이 다운로드한 파일(CUBRID-9.1.0.0201-linux.x86_64.sh)을 설치한 후, CUBRID 데이터베이스를 사용하기 위해서는 CUBRID 관련 환경 정보를 설정해야 한다. 이 설정은 해당 터미널에 로그인할 때 자동 설정되도록 지정되어 있으므로 설치 후 최초 한 번만 수행하면 된다. ::

        $ . /home1/cub_user/.cubrid.sh

    CUBRID가 설치 완료되면 CUBRID 매니저 서버와 브로커를 다음과 같이 구동시킬 수 있다. ::

        $ cubrid service start

    cubrid service를 구동시킨 후 정상적으로 구동되었는지 확인하려면 Linux에서는 다음과 같이 grep으로 cub_* 프로세스들이 구동되어 있는지를 확인한다. ::

        $ ps -ef | grep cub_
        cub_user 15200 1 0 18:57   00:00:00 cub_master
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
        cub_user 15229 1 0 18:57   00:00:00 cub_auto start
        cub_user 15232 1 0 18:57   00:00:00 cub_js start

**RPM으로 CUBRID 설치**

    CentOS5 환경에서 생성한 RPM 파일을 사용하여 CUBRID를 설치할 수 있으며, 일반적인 RPM 유틸리티와 동일한 방법으로 설치하고 삭제할 수 있다. 설치하면 새로운 시스템 그룹(cubrid) 및 사용자 계정(cubrid)이 생성되며, 설치 후에는 cubrid 사용자 계정으로 로그인하여 CUBRID 서비스를 시작해야 한다. ::

        $ rpm -Uvh cubrid-9.1.0.0201-el5.x86_64.rpm

    RPM을 실행하면 CUBRID는 "cubrid" 홈 디렉터리(/opt/cubrid)에 설치되고, CUBRID 관련 환경 설정 파일(cubrid.[c]sh)이 /etc/profile.d 디렉터리에 설치된다. 단, demodb는 자동으로 설치되지 않으므로 "cubrid" Linux 계정으로 로그인하여 /opt/cubrid/demo/make_cubrid_demo.sh를 실행하여야 한다. CUBRID가 설치 완료되면 "cubrid" Linux 계정으로 로그인하여 CUBRID 서비스를 다음과 같이 시작한다. ::

        $ cubrid service start

    .. note:: \

        **RPM과 의존성**
        
            RPM으로 설치할 때에는 의존성을 꼭 확인해야 한다. 의존성을 무시(--nodeps)하고 설치하면 실행되지 않을 수 있다.
            RPM을 삭제하더라도 cubrid 사용자 계정 및 설치 후 생성한 데이터베이스는 보관되므로, 더 이상 필요하지 않은 경우 수동으로 삭제해야 한다.


        **Linux에서 시스템 구동 시 CUBRID 자동 구동하기**
        
            SH 패키지나 RPM 패키지로 CUBRID를 설치했다면 $CUBRID/share/init.d 디렉터리에 cubrid라는 스크립트가 포함되어 있다. 이 파일 안의 **CUBRID_USER** 라는 환경 변수 값을 CUBRID를 설치한 Linux 계정으로 변경한 후, /etc/init.d에 등록하면 service나 chkconfig 명령을 사용하여 Linux 시스템 구동 시 CUBRID를 자동으로 구동할 수 있다.

**Fedora/CentOS에서 CUBRID 설치**

    yum 명령어를 사용하여 CUBRID를 설치하려면, CUBRID 패키지의 위치를 알아야 한다. 운영체제에 따라 다음 주소로 이동하여 자신의 운영체제에 맞는 파일을 선택한다.    `http://www.cubrid.org/yum_repository <http://www.cubrid.org/yum_repository>`_

    예를 들어, 운영체제가 Fedora 16이면 다음과 같은 명령을 실행한다. fc16은 Fedora 16을 의미한다. ::

        $ rpm -i http://yumrepository.cubrid.org/cubrid_repo_settings/9.0.0/cubridrepo-9.0.0-1.fc16.noarch.rpm

    운영체제가 CentOS 6.2이면 다음과 같은 명령을 실행한다. el6.2는 CentOS 6.2를 의미한다. ::

        $ rpm -i http://yumrepository.cubrid.org/cubrid_repo_settings/9.0.0/cubridrepo-9.0.0-1.el6.2.noarch.rpm

    위의 명령을 실행하면 원하는 CUBRID 패키지를 설치할 수 있다. CUBRID 최신 버전을 설치하려면 다음 명령을 실행한다. ::

        $ yum install cubrid

    이전 버전을 설치하려면 다음과 같이 명령에 버전을 포함해야 한다. ::

        $ yum install cubrid-8.4.3

    설치를 완료하면 CUBRID 경로를 포함한 환경 변수들을 설정하고, 이를 시스템에 적용한다.

**Ubuntu에서 CUBRID 설치**

    Ubuntu에서 apt-get 명령어를 사용하여 CUBRID를 설치하려면, 먼저 CUBRID 저장소를 추가하고, apt 인덱스를 업데이트한다. ::

        $ sudo add-apt-repository ppa:cubrid/cubrid
        $ sudo apt-get update

    CUBRID 최신 버전을 설치하려면 다음 명령을 실행한다. ::

        $ sudo apt-get install cubrid

    이전 버전을 설치하려면 다음과 같이 명령에 버전을 포함해야 한다. ::

        $ sudo apt-get install cubrid-8.4.3

    설치를 완료하면 CUBRID 경로를 포함한 환경 변수들을 설정하고, 이를 시스템에 적용한다.

**CUBRID 업그레이드**

    다른 버전의 CUBRID가 설치된 디렉터리를 CUBRID를 설치할 디렉터리로 지정하면, 해당 디렉터리가 존재하는 것을 알리고 덮어쓸 것인지 확인한다. **no** 를 입력하면 설치가 중단된다. ::

        Directory '/home1/cub_user/CUBRID' exist!
        If a CUBRID service is running on this directory, it may be terminated abnormally.
        And if you don't have right access permission on this directory(subdirectories or files), install operation will be failed.
        Overwrite anyway? (yes or no) [Default: no] : yes

    CUBRID를 설치하고 설정 파일을 구성할 때 기존의 설정 파일을 그대로 사용할 것인지, 새 설정 파일을 사용할 것인지 확인한다. **yes** 를 입력하면 기존의 설정 파일을 확장자가 .bak인 백업 파일로 보관한다. ::

        The configuration file (.conf or .pass) already exists. Do you want to overwrite it? (yes or no) : yes

    이전 버전에서 새 버전으로 데이터베이스를 업그레이드하는 방법에 대한 보다 자세한 내용은 :doc:`upgrade` 를 참고한다.

**환경 설정**

    서비스 포트 등 사용자 환경에 맞춰 설정을 변경하려면 **$CUBRID/conf** 디렉터리에서 설정 파일의 파라미터를 수정한다. 
    자세한 내용은 :ref:`Installin-and-Running-on-Windows` 의 환경 설정을 참고한다.

**CUBRID 인터페이스 설치**

    CCI, JDBC, PHP, ODBC, OLE DB, ADO.NET, Ruby, Python, Node.js 등의 인터페이스 모듈은 `http://www.cubrid.org/wiki_apis <http://www.cubrid.org/wiki_apis>`_ 에서 최신 정보를 확인할 수 있고 관련 파일을 내려받아 설치할 수 있다.

    각 드라이버에 대한 간단한 설명은 :doc:`/api/index` 를 참고한다.
    
**CUBRID 도구 설치**

    CUBRID 매니저, CUBRID 쿼리 브라우저 등의 도구는 `http://www.cubrid.org/wiki_tools <http://www.cubrid.org/wiki_tools>`_
    에서 최신 정보를 확인할 수 있고 관련 파일을 내려받아 설치할 수 있다.

    CUBRID 웹매니저는 CUBRID 설치 시 구동되며 `https://localhost:8282/ <https://localhost:8282/>`_ 에서 확인할 수 있다.
        
.. _Installin-and-Running-on-Windows:

Windows에서의 설치와 실행
-------------------------

**설치 시 확인 사항**

    Windows 버전의 CUBRID 데이터베이스를 설치하기 전에 다음 사항을 점검한다.

    * 64비트 여부

      CUBRID 2008 R2.0 버전부터 32비트 버전과 64비트 버전을 각각 지원한다. [내 컴퓨터] > [시스템 등록 정보] 창을 활성화하여 Windows 버전 비트를 확인할 수 있다. 32비트 Windows에서는 CUBRID 32비트 버전을 설치하고, 64비트 Windows에서는 CUBRID 64비트 버전을 설치한다.

    Windows Vista 이상 버전에서 CUBRID를 설치하려면 설치 파일을 관리자 권한으로 실행하도록 한다.

    * CUBRID 설치 파일에 대고 마우스 오른쪽 버튼을 클릭하여 나타난 팝업 메뉴에서 [관리자 권한으로 실행(A)]을 선택한다.
      
    시스템을 시작할 때 CUBRID Service Tray가 자동으로 구동되지 않는다면 다음 사항을 확인하도록 한다.

    * [제어판] > [관리 도구] > [서비스]의 Task Scheduler가 시작되어 있는지 확인하고, 그렇지 않으면 Task Scheduler를 시작한다.
    * [제어판] > [관리 도구] > [작업 스케줄러]에 CUBRID Service Tray가 등록되어 있는지 확인하고, 그렇지 않으면 CUBRID Service Tray를 등록한다.

**설치 과정**

    **1단계: 설치 디렉터리 지정**
    
    **2단계: 설치 유형 선택**

    * **전체 설치** : CUBRID 서버와 명령행 도구 및 인터페이스 드라이버(OLEDB Provider, ODBC, JDBC, C API)가 모두 설치된다.

    * **인터페이스 드라이버 설치** : 인터페이스 드라이버(OLEDB Provider, ODBC, JDBC, C API)만 설치된다. CUBRID 데이터베이스 서버가 설치된 컴퓨터에 원격 접근하여 개발하는 경우, 이 설치 유형을 선택할 수 있다.
          
    **3단계: 샘플 데이터베이스 생성**
        
        샘플 데이터베이스를 생성하려면 약 300MB의 디스크 공간이 필요하다. 
    
    **4단계: 설치 완료**
    
        우측 하단에 CUBRID Service Tray가 나타난다. 
        
    .. note:: 
    
        CUBRID는 설치하고 나면 시스템 재구동 시 자동으로 실행하게 되어 있다. 시스템 재구동 시 자동 실행을 중단하려면 "제어판 > 시스템 및 보안 > 관리 도구 > 서비스 > CUBRIDService" 에서 더블클릭한 후 나타난 팝업 창에서 시작 유형을 수동으로 변경한다.

    
**CUBRID 업그레이드**

    이전 버전의 CUBRID가 이미 설치된 환경에 새로운 버전의 CUBRID를 설치하는 경우, 시스템 트레이에서 [CUBRID Service Tray] > [Exit]를 선택하여 운영 중인 서비스를 종료한 후 이전 버전의 CUBRID를 제거해야 한다. "데이터베이스와 설정 파일을 모두 삭제하겠습니까?"라고 묻는 대화 상자가 나타나면, 이전 버전의 데이터베이스가 삭제되지 않도록?[아니오]를 클릭한다.

    이전 버전에서 새 버전으로 데이터베이스를 업그레이드하는 방법에 대한 보다 자세한 내용은 :doc:`upgrade` 를 참고한다.

.. _Configuring-Environment-on-Windows:

**환경 설정**

    서비스 포트 등 사용자 환경에 맞춰 설정을 변경하려면 **%CUBRID%\\conf** 디렉터리에서 다음 설정 파일의 파라미터 값을 변경한다. 방화벽이 설정되어 있다면 CUBRID에서 사용하는 포트들을 열어두어야(open) 한다. CUBRID가 사용하는 포트에 대한 자세한 내용은 :ref:`connect-to-cubrid-server` 를 참고한다.

    * **cm.conf**

      CUBRID 매니저용 설정 파일이다. **cm_port** 는 매니저 서버 프로세스가 사용하는 포트로 기본값은 **8001** 이며, 설정된 포트와 설정된 포트 번호+1 두 개의 포트가 사용된다. 즉, 8001 포트를 설정하면 8001, 8002 두 개의 포트가 사용된다. 자세한 내용은 `CUBRID 매니저 매뉴얼 <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual_kr>`_ 을 참고한다.

    * **cm_httpd.conf**
     
      CUBRID 웹 매니저용 설정 파일이다. **listen**\은 웹 매니저 서버 프로세스가 사용하는 포트로 기본값은 **8282** 이다. 자세한 내용은 `CUBRID 웹 매니저 매뉴얼 <http://www.cubrid.org/wiki_tools/entry/cubrid-web-manager-manual>`_ 을 참고한다.
      
    * **cubrid.conf**

      서버 설정용 파일로, 운영하려는 데이터베이스의 메모리, 동시 사용자 수에 따른 스레드 수, 브로커와 서버 사이의 통신 포트 등을 설정한다. **cubrid_port_id** 는 마스터 프로세스가 사용하는 포트로, 기본값은 *1523* 이다. 자세한 내용은 :ref:`cubrid-conf-default-parameters` 를 참조한다.

    * **cubrid_broker.conf**

      브로커 설정용 파일로, 운영하려는 브로커가 사용하는 포트, 응용서버(CAS) 수, SQL LOG 등을 설정한다. **BROKER_PORT** 는 브로커가 사용하는 포트이며, 실제 JDBC와 같은 드라이버에서 보는 포트는 해당 브로커의 포트이다. **APPL_SERVER_PORT** 는 Windows에서만 추가하는 파라미터로, 브로커 응용 서버(CAS)가 사용하는 포트이다. 기본값은 **BROKER_PORT** + 1이다. **APPL_SERVER_PORT** 값을 기준으로 1씩 더한 포트들이 CAS 개수만큼 사용된다.
      예를 들어 **APPL_SERVER_PORT** 값이 35000이고 **MAX_NUM_APPL_SERVER** 값에 의한 CAS의 최대 개수가 50이면 CAS에서 listen하는 포트는 35000, 35001, ..., 35049이다.
      자세한 내용은 :ref:`parameter-by-broker` 를 참조한다. 
      
      **CCI_DEFAULT_AUTOCOMMIT** 브로커 파라미터는 2008 R4.0부터 지원하기 시작했고, 이때 기본값은 **OFF** 였다가 2008 R4.1부터는 기본값이 **ON** 으로 바뀌었다. 따라서 2008 R4.0에서 2008 R4.1 이상 버전으로 업그레이드하는 사용자는 이 값을 OFF로 바꾸거나, 응용 프로그램의 함수에서 자동 커밋 모드를 OFF로 설정해야 한다.

**CUBRID 인터페이스 설치**

    JDBC, PHP, ODBC, OLE DB 등 인터페이스 모듈은 `http://www.cubrid.org/wiki_apis <http://www.cubrid.org/wiki_apis>`_ 에서 최신 정보를 확인할 수 있고 관련 파일을 내려받아 설치할 수 있다.
    
    각 드라이버에 대한 간단한 설명은 :doc:`/api/index` 를 참고한다.

**CUBRID 도구 설치**

    CUBRID 매니저, CUBRID 쿼리 브라우저 등의 도구는 `http://www.cubrid.org/wiki_tools <http://www.cubrid.org/wiki_tools>`_ 에서 최신 정보를 확인할 수 있고 관련 파일을 내려받아 설치할 수 있다.

    CUBRID 웹매니저는 CUBRID 설치 시 구동되며 https://localhost:8282/ 에서 확인할 수 있다.
    
.. _connect-to-cubrid-server:

CUBRID 서버에 연결하기
======================

포트가 개방되어 있지 않은 환경에서 사용하는 경우, CUBRID가 사용하는 포트들을 개방해야 한다.

다음은 CUBRID가 사용하는 포트에 대해 하나의 표로 정리한 것이다. 각 포트는 상대방의 접속을 대기하는 listener 쪽에서 개방되어야 한다.

Linux 방화벽에서 특정 프로세스에 대한 포트를 개방하려면 해당 방화벽 프로그램의 설명을 따른다.

Windows에서 임의의 가용 포트를 사용하는 경우는 어떤 포트를 개방할 지 알 수 없으므로  Windows 메뉴의 "제어판" 검색창에서  "방화벽"을 입력한 후, "Windows 방화벽 > Windows 방화벽을 통해 프로그램 또는 기능 허용"에서 포트 개방을 원하는 프로그램을 추가한다. 

Windows에서 특정 포트를 지정하기 번거로운 경우에도 이 방법을 사용할 수 있다. 일반적으로 Windows 방화벽에서 특정 프로그램을 지정하지 않고 포트를 여는 것보다 허용되는 프로그램 목록에 프로그램을 추가하는 것이 보다 안전하므로 이 방식을 권장한다.

* cub_broker에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_broker.exe"를 추가한다.
* CAS에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_cas.exe"를 추가한다.
* cub_master에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_master.exe"를 추가한다.
* cub_server에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_server.exe"를 추가한다.
* CUBRID Manager에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_cmserver.exe"를 추가한다.
* CUBRID Web Manager에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_cmhttpd.exe"를 추가한다.
    
브로커 장비 또는 DB 서버 장비에서 Linux용 CUBRID를 사용한다면 Linux 포트가 모두 개방되어 있어야 한다.
브로커 장비 또는 DB 서버 장비에서 Windows용 CUBRID를 사용한다면 Windows 포트가 모두 개방되어 있거나, 관련 프로세스들이 모두 Windows 방화벽에서 허용되는 목록에 추가되어 있어야 한다.
     
    +---------------+--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    | 구분          | listener     | requester     | Linux 포트     | Windows 포트                                        | 방화벽 포트 설정         | 설명         |
    +===============+==============+===============+================+=====================================================+==========================+==============+
    | 기본 사용     | cub_broker   | application   | BROKER_PORT    | BROKER_PORT                                         | 개방(open)               | 일회성 연결  |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | CAS          | application   | BROKER_PORT    | APPL_SERVER_PORT ~ (APP_SERVER_PORT + CAS 개수 - 1) | 개방                     | 연결 유지    |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | cub_master   | CAS           | cubrid_port_id | cubrid_port_id                                      | 개방                     | 일회성 연결  |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | cub_server   | CAS           | cubrid_port_id | 임의의 가용 포트                                    | Linux: 개방              | 연결 유지    |
    |               |              |               |                |                                                     |                          |              |
    |               |              |               |                |                                                     | Windows: 프로그램        |              |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | 클라이언트   | cub_server    | ECHO(7)        | ECHO(7)                                             | 개방                     | 주기적 연결  |
    |               | 장비(*)      |               |                |                                                     |                          |              |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | 서버         | CAS, CSQL     | ECHO(7)        | ECHO(7)                                             | 개방                     | 주기적 연결  |
    |               | 장비(**)     |               |                |                                                     |                          |              |
    +---------------+--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    | HA 사용       | cub_broker   | application   | BROKER_PORT    | 미지원                                              | 개방                     | 일회성 연결  |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | CAS          | application   | BROKER_PORT    | 미지원                                              | 개방                     | 연결 유지    |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | cub_master   | CAS           | cubrid_port_id | 미지원                                              | 개방                     | 일회성 연결  |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | cub_master   | cub_master    | ha_port_id     | 미지원                                              | 개방                     | 주기적 연결, |
    |               |              |               |                |                                                     |                          | heartbeat    |
    |               | (slave)      | (master)      |                |                                                     |                          | 확인         |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | cub_master   | cub_master    | ha_port_id     | 미지원                                              | 개방                     | 주기적 연결, |
    |               |              |               |                |                                                     |                          | heartbeat    |
    |               | (master)     | (slave)       |                |                                                     |                          | 확인         |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | cub_server   | CAS           | cubrid_port_id | 미지원                                              | 개방                     | 연결 유지    |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | 클라이언트   | cub_server    | ECHO(7)        | 미지원                                              | 개방                     | 주기적 연결  |
    |               | 장비(*)      |               |                |                                                     |                          |              |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | 서버         | CAS, CSQL,    | ECHO(7)        | 미지원                                              | 개방                     | 주기적 연결  |
    |               | 장비(**)     | copylogdb,    |                |                                                     |                          |              |
    |               |              | applylogdb    |                |                                                     |                          |              |
    +---------------+--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    | SHARD 사용    | shard_broker | application   | BROKER_PORT    | BROKER_PORT                                         | 개방                     | 일회성 연결  |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | shard_proxy  | application   | BROKER_PORT    | BROKER_PORT + 1 ~ (BROKER_PORT + MAX_NUM_PROXY)     | 개방                     | 연결 유지    |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | shard_proxy  | shard CAS     | 없음           | (BROKER_PORT + MAX_NUM_PROXY + 1) ~                 | 불필요                   | 연결 유지    |
    |               |              |               |                | (BROKER_PORT + MAX_NUM_PROXY * 2)                   |                          |              |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | cub_master   | shard CAS     | cubrid_port_id | cubrid_port_id                                      | 개방                     | 일회성 연결  |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | cub_server   | shard CAS     | cubrid_port_id | 임의의 가용 포트                                    | Linux: 개방              | 연결 유지    |
    |               |              |               |                |                                                     |                          |              |
    |               |              |               |                |                                                     | Windows: 프로그램        |              |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | 클라이언트   | cub_server    | ECHO(7)        | ECHO(7)                                             | 개방                     | 주기적 연결  |
    |               | 장비(**)     |               |                |                                                     |                          |              |
    |               +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    |               | 서버         | CAS, CSQL     | ECHO(7)        | ECHO(7)                                             | 개방                     | 주기적 연결  |
    |               | 장비(\*\*\*) |               |                |                                                     |                          |              |
    +---------------+--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    | Manager,      | Manager      | application   | 8001, 8002     | 8001, 8002                                          | 개방                     |              |
    |               | 서버         |               |                |                                                     |                          |              |
    | Web Manager   +--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    | 사용          | Web Manager  | application   | 8282           | 8282                                                | 개방                     |              |
    |               | 서버         |               |                |                                                     |                          |              |
    +---------------+--------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    
각 구분 별 상세 설명은 아래와 같다.

.. _cubrid-basic-ports:

CUBRID 기본 사용 포트
---------------------

접속 요청을 기다리는(listening) 프로세스들을 기준으로 각 OS 별로 필요한 포트를 정리하면 다음과 같으며, 각 포트는 listener 쪽에서 개방되어야 한다.

+------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| listener   | requester     | Linux port     | Windows port                                        | 방화벽 포트 설정         | 설명         |
+============+===============+================+=====================================================+==========================+==============+
| cub_broker | application   | BROKER_PORT    | BROKER_PORT                                         | 개방(open)               | 일회성 연결  |
+------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| CAS        | application   | BROKER_PORT    | APPL_SERVER_PORT ~ (APP_SERVER_PORT + CAS 개수 - 1) | 개방                     | 연결 유지    |
+------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| cub_master | CAS           | cubrid_port_id | cubrid_port_id                                      | 개방                     | 일회성 연결  |
+------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| cub_server | CAS           | cubrid_port_id | 임의의 가용 포트                                    | Linux: 개방              | 연결 유지    |
|            |               |                |                                                     |                          |              |
|            |               |                |                                                     | Windows: 프로그램        |              |
+------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| 클라이언트 | cub_server    | ECHO(7)        | ECHO(7)                                             | 개방                     | 주기적 연결  |
| 장비(*)    |               |                |                                                     |                          |              |
+------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| 서버       | CAS, CSQL     | ECHO(7)        | ECHO(7)                                             | 개방                     | 주기적 연결  |
| 장비(**)   |               |                |                                                     |                          |              |
+------------+---------------+----------------+-----------------------------------------------------+--------------------------+--------------+
    
(*): CAS 또는 CSQL 프로세스가 존재하는 장비

(**): cub_server가 존재하는 장비
    
.. note:: Windows에서는 CAS가 cub_server에 접근할 때 사용할 포트를 임의로 정하므로 개방할 포트를 정할 수 없다. 따라서 "Windows 방화벽 >  허용되는 프로그램"에 "%CUBRID%\\bin\\cub_server.exe"을 추가해야 한다.
    
서버 프로세스(cub_server)와 이에 접속하는 클라이언트 프로세스들(CAS, CSQL) 사이에서 상대 노드가 정상 동작하는지 ECHO(7) 포트를 통해 서로 확인하므로, 방화벽 존재 시 ECHO(7) 포트를 개방해야 한다. ECHO 포트를 서버와 클라이언트 양쪽 다 개방할 수 없는 상황이라면 cubrid.conf의 **check_peer_alive** 파라미터 값을 none으로 설정한다.

다음은 각 프로세스 간 연결 관계를 나타낸 것이다.

::

     application - cub_broker
                 -> CAS  -  cub_master
                         -> cub_server

* application: 응용 프로세스
* cub_broker: 브로커 서버 프로세스. application이 연결할 CAS를 선택하는 역할을 수행.
* CAS: 브로커 응용 서버 프로세스. application과 cub_server를 중계.
* cub_master: 마스터 프로세스. CAS가 연결할 cub_server를 선택하는 역할을 수행.
* cub_server: DB 서버 프로세스
    
프로세스 간 관계 기호 및 의미는 다음과 같다.

* \- 기호: 최초 한 번만 연결됨을 나타낸다.
* ->, <- 기호: 연결이 유지됨을 나타내며, -> 의 오른쪽 또는 <-의 왼쪽이 화살을 받는 쪽이다. 화살을 받는 쪽이 처음에 상대 프로세스의 접속을 기다리는(listening) 쪽을 나타낸다.
* (master): HA 구성에서 master 노드를 나타낸다.
* (slave): HA 구성에서 slave 노드를 나타낸다.

다음은 응용 프로그램과 DB 사이의 연결 과정을 순서대로 나열한 것이다.

#. application이 cubrid_broker.conf에 설정된 브로커 포트(BROKER_PORT)를 통해 cub_broker와 연결을 시도한다.
#. cub_broker는 연결 가능한 CAS를 선택한다.
#. application과 CAS가 연결된다. 

   Linux에서는 application이 유닉스 도메인 소켓을 통해 CAS와 연결되므로 BROKER_PORT를 사용한다. Windows에서는 유닉스 도메인 소켓을 사용할 수 없으므로 각 CAS마다 cubrid_broker.conf에 설정된 APPL_SERVER_PORT 값을 기준으로 CAS ID를 더한 포트를 통해 연결된다. APPL_SERVER_PORT의 값이 설정되지 않으면 첫번째 CAS와 연결하는 포트 값은 BROKER_PORT + 1이 된다.

   예를 들어 Windows에서 BROKER_PORT가 33000이고 APPL_SERVER_PORT 가 설정되지 않았으면 application과 CAS 사이에 사용하는 포트는 다음과 같다.
    
   * application이 CAS(1)과 접속하는 포트 : 33001
   * application이 CAS(2)와 접속하는 포트 : 33002
   * application이 CAS(3)와 접속하는 포트 : 33003
                
#. CAS는 cubrid.conf에 설정된 cubrid_port_id 포트를 통해 cub_master에게 cub_server로의 연결을 요청한다.
#. CAS와 cub_server가 연결된다. 

   Linux에서는 CAS가 유닉스 도메인 소켓을 통해 cub_server와 연결되므로 cubrid_port_id 포트를 사용한다. Windows에서는 유닉스 도메인 소켓을 사용할 수 없으므로 임의의 가용 포트를 통해 cub_server와 연결된다. Windows에서 DB server를 운용한다면 브로커 장비와 DB 서버 장비 사이에서는 임의의 가용 포트를 사용하므로, 두 장비 사이에서 방화벽이 해당 프로세스에 대한 포트를 막게 되면 정상 동작을 보장할 수 없게 된다는 점에 주의한다.
  
#. 이후 CAS는 application이 종료되어도 CAS가 재시작되지 않는 한 cub_server와 연결을 유지한다.

.. _cubrid-ha-ports: 

CUBRID HA 사용 포트
-------------------

CUBRID HA는 Linux 환경에서만 지원한다.

접속 요청을 기다리는(listening) 프로세스들을 기준으로 각 OS 별로 필요한 포트를 정리하면 다음과 같으며, 각 포트는 listener 쪽에서 개방되어야 한다.

+------------+---------------+----------------+--------------------------+--------------+
| listener   | requester     | Linux port     | 방화벽 포트 설정         | 설명         |
+============+===============+================+==========================+==============+
| cub_broker | application   | BROKER_PORT    | 개방(open)               | 일회성 연결  |
+------------+---------------+----------------+--------------------------+--------------+
| CAS        | application   | BROKER_PORT    | 개방                     | 연결 유지    |
+------------+---------------+----------------+--------------------------+--------------+
| cub_master | CAS           | cubrid_port_id | 개방                     | 일회성 연결  |
+------------+---------------+----------------+--------------------------+--------------+
| cub_master | cub_master    | ha_port_id     | 개방                     | 주기적 연결, |
|            |               |                |                          | heartbeat    |
| (slave)    | (master)      |                |                          | 확인         |
+------------+---------------+----------------+--------------------------+--------------+
| cub_master | cub_master    | ha_port_id     | 개방                     | 주기적 연결, |
|            |               |                |                          | heartbeat    |
| (master)   | (slave)       |                |                          | 확인         |
+------------+---------------+----------------+--------------------------+--------------+
| cub_server | CAS           | cubrid_port_id | 개방                     | 연결 유지    |
+------------+---------------+----------------+--------------------------+--------------+
| 클라이언트 | cub_server    | ECHO(7)        | 개방                     | 주기적 연결  |
| 장비(*)    |               |                |                          |              |
+------------+---------------+----------------+--------------------------+--------------+
| 서버       | CAS, CSQL,    | ECHO(7)        | 개방                     | 주기적 연결  |
| 장비(**)   | copylogdb,    |                |                          |              |
|            | applylogdb    |                |                          |              |
+------------+---------------+----------------+--------------------------+--------------+
    
(*): CAS, CSQL, copylogdb, 또는 applylogdb 프로세스가 존재하는 장비

(**): cub_server가 존재하는 장비

서버 프로세스(cub_server)와 이에 접속하는 클라이언트 프로세스들(CAS, CSQL, copylogdb, applylogdb 등) 사이에서 상대 노드가 정상 동작하는지 ECHO(7) 포트를 통해 서로 확인하므로, 방화벽 존재 시 ECHO(7) 포트를 개방해야 한다. ECHO 포트를 서버와 클라이언트 양쪽 다 개방할 수 없는 상황이라면 cubrid.conf의 **check_peer_alive** 파라미터 값을 none으로 설정한다.

이외에도 ECHO(7) 포트의 개방이 필요하다. ECHO 포트 개방과 관련된 설명은 :ref:`cubrid-basic-ports` 절을 참고한다.

다음은 각 프로세스 간 연결 관계를 나타낸 것이다.

::

    application - cub_broker
                -> CAS  -  cub_master(master) <-> cub_master(slave)
                        -> cub_server(master)     cub_server(slave) <- applylogdb(slave)
                                              <----------------------- copylogdb(slave)
                                              
* cub_master(master): CUBRID HA 구성에서 master 노드에 있는 마스터 프로세스. 상대 노드가 살아있는지 확인하는 역할을 수행.
* cub_master(slave): CUBRID HA 구성에서 slave 노드에 있는 마스터 프로세스. 상대 노드가 살아있는지 확인하는 역할을 수행.
* copylogdb(slave): CUBRID HA 구성에서 slave 노드에 있는 복제 로그 복사 프로세스
* applylogdb(slave): CUBRID HA 구성에서 slave 노드에 있는 복제 로그 반영 프로세스

master 노드에서 slave 노드로의 복제 과정 파악이 용이하게 하기 위해 위에서 master 노드의 applylogdb, copylogdb와 slave 노드의 CAS는 생략했다.

프로세스 간 관계 기호 및 의미는 다음과 같다.

* \- 기호: 최초 한 번만 연결됨을 나타낸다.
* ->, <- 기호: 연결이 유지됨을 나타내며, -> 의 오른쪽 또는 <-의 왼쪽이 화살을 받는 쪽이다. 화살을 받는 쪽이 처음에 상대 프로세스의 접속을 기다리는(listening) 쪽을 나타낸다.
* (master): HA 구성에서 master 노드를 나타낸다.
* (slave): HA 구성에서 slave 노드를 나타낸다.
    
응용 프로그램과 DB 사이의 연결 과정은 1. CUBRID 기본 사용 포트와 동일하다. 여기에서는 CUBRID HA에 의해 1:1로 master DB와 slave DB를 구성할 때 master 노드와 slave 노드 사이의 연결 과정에 대해서만 설명한다.

#. cub_master(master)와 cub_master(slave) 사이에는 cubrid_ha.conf에 설정된 ha_port_id를 사용한다.
#. copylogdb(slave)는 slave 노드에 있는 cubrid.conf의 cubrid_port_id에 설정된 포트를 통해 cub_master(master)에게 master DB로의 연결을 요청하여, 최종적으로 cub_server(master)와 연결하게 된다.
#. applylogdb(slave)는 slave 노드에 있는 cubrid.conf의 cubrid_port_id에 설정된 포트를 통해 cub_master(slave)에게 slave DB로의 연결을 요청하여, 최종적으로 cub_server(slave)와 연결하게 된다.

master 노드에서도 applylogdb와 copylogdb가 동작하는데, master 노드가 절체로 인해 slave 노드로 변경될 때를 대비하기 위함이다.

.. _cubrid-shard-ports:

CUBRID SHARD 사용 포트
----------------------

접속 요청을 기다리는(listening) 프로세스들을 기준으로 각 OS 별로 필요한 포트를 정리하면 다음과 같으며, 각 포트는 listener 쪽에서 개방되어야 한다.

+---------------+--------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| listener      | requester    | Linux port     | Windows port                                        | 방화벽 포트 설정         | 설명         |
+===============+==============+================+=====================================================+==========================+==============+
| shard_broker  | application  | BROKER_PORT    | BROKER_PORT                                         | 개방(open)               | 일회성 연결  |
+---------------+--------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| shard_proxy   | application  | BROKER_PORT    | BROKER_PORT + 1 ~ (BROKER_PORT + MAX_NUM_PROXY)     | 개방                     | 연결 유지    |
+---------------+--------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| shard_proxy   | shard CAS    | 없음           | (BROKER_PORT + MAX_NUM_PROXY + 1) ~                 | 불필요(*)                | 연결 유지    |
|               |              |                | (BROKER_PORT + MAX_NUM_PROXY * 2)                   |                          |              |
+---------------+--------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| cub_master    | shard CAS    | cubrid_port_id | cubrid_port_id                                      | 개방                     | 일회성 연결  |
+---------------+--------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| cub_server    | shard CAS    | cubrid_port_id | 임의의 가용 포트                                    | Linux: 개방              | 연결 유지    |
|               |              |                |                                                     |                          |              |
|               |              |                |                                                     | Windows: 프로그램        |              |
+---------------+--------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| 클라이언트    | cub_server   | ECHO(7)        | ECHO(7)                                             | 개방                     | 주기적 연결  |
| 장비(**)      |              |                |                                                     |                          |              |
+---------------+--------------+----------------+-----------------------------------------------------+--------------------------+--------------+
| 서버          | CAS, CSQL    | ECHO(7)        | ECHO(7)                                             | 개방                     | 주기적 연결  |
| 장비(\*\*\*)  |              |                |                                                     |                          |              |
+---------------+--------------+----------------+-----------------------------------------------------+--------------------------+--------------+

(*): shard CAS와 shard_proxy는 물리적으로 서로 분리되지 않으므로 방화벽에서 포트 개방을 설정하지 않아도 된다. Linux에서 두 프로세스 간 접속은 유닉스 도메인 소켓을 사용한다.

(**): CAS 또는 CSQL 프로세스가 존재하는 장비

(\*\*\*): cub_server가 존재하는 장비
    
.. note:: Windows에서는 CAS가 cub_server에 접근할 때 사용할 포트를 임의로 정하므로 개방할 포트를 정할 수 없다.  따라서 "Windows 방화벽 >  허용되는 프로그램"에 "%CUBRID%\\bin\\cub_server.exe"을 추가해야 한다.
    
서버 프로세스(cub_server)와 이에 접속하는 클라이언트 프로세스들(CAS, CSQL) 사이에서 상대 노드가 정상 동작하는지 ECHO(7) 포트를 통해 서로 확인하므로, 방화벽 존재 시 ECHO(7) 포트를 개방해야 한다. ECHO 포트를 서버와 클라이언트 양쪽 다 개방할 수 없는 상황이라면 cubrid.conf의 **check_peer_alive** 파라미터 값을 none으로 설정한다.

::

    application - shard broker
                -> shard proxy <- shard CAS - cub_master
                                            -> cub_server

    * shard broker: CUBRID SHARD 브로커 프로세스. application과 shard proxy를 중계
    * shard proxy: CUBRID SHARD 프록시 프로세스. 어떤 shard DB를 선택할 지 결정하는 역할을 수행
    * shard CAS: CUBRID SHARD CAS 프로세스. shard proxy와 cub_server를 중계

프로세스 간 관계 기호 및 의미는 다음과 같다.

* \- 기호: 최초 한 번만 연결됨을 나타낸다.
* ->, <- 기호: 연결이 유지됨을 나타내며, -> 의 오른쪽 또는 <-의 왼쪽이 화살을 받는 쪽이다. 화살을 받는 쪽이 처음에 상대 프로세스의 접속을 기다리는(listening) 쪽을 나타낸다.

다음은 CUBRID SHARD 구성에서 application과 DB server 사이의 연결 과정에 대해 나열한 것이다. shard CAS와 shard proxy는 CUBRID SHARD를 구동(cubrid shard start)하는 시점에 이미 연결된 상태이다.

#. application이 shard.conf에 설정된 BROKER_PORT를 통해 shard broker에 연결을 시도한다.

#. shard broker는 연결 가능한 shard proxy를 선택한다. 

#. application과 shard proxy가 연결된다. shard proxy의 최소, 최대 개수는 shard.conf의 MIN_NUM_PROXY와 MAX_NUM_PROXY에 의해 설정된다.

   Linux에서는 application이 유닉스 도메인 소켓을 통해 shard proxy와 연결된다. Windows에서는 유닉스 도메인 소켓을 사용할 수 없으므로 각 shard proxy마다 shard.conf에 설정된 BROKER_PORT와 MAX_NUM_PROXY를 가지고 계산된 포트를 통해 연결된다.

   예를 들어 Linux에서 BROKER_PORT가 45000이고 MAX_NUM_PROXY가 3일 때 사용하는 포트는 45000 하나면 된다.
   
   * application이 shard proxy(1)과 접속하는 포트: 45000, shard CAS가 shard proxy(1)과 접속하는 포트 : 없음
   * application이 shard proxy(2)와 접속하는 포트: 45000, shard CAS가 shard proxy(2)와 접속하는 포트 : 없음
   * application이 shard proxy(3)과 접속하는 포트: 45000, shard CAS가 shard proxy(3)와 접속하는 포트 : 없음
   
   반면, Windows에서 BROKER_PORT가 45000이고 MAX_NUM_PROXY가 3이면 사용하는 포트는 다음과 같다.
   
   * application이 shard proxy(1)과 접속하는 포트: 45001, shard CAS가 shard proxy(1)과 접속하는 포트 : 45004
   * application이 shard proxy(2)와 접속하는 포트: 45002, shard CAS가 shard proxy(2)와 접속하는 포트 : 45005
   * application이 shard proxy(3)과 접속하는 포트: 45003, shard CAS가 shard proxy(3)와 접속하는 포트 : 45006
   
   .. note:: 현재 버전에서 MIN_NUM_PROXY는 사용되지 않고 MAX_NUM_PROXY만 사용된다.
 
#. shard CAS와 shard proxy는 CUBRID SHARD를 구동(cubrid shard start)하는 시점에 이미 연결된 상태이다. 또한, 각 프로세스는 항상 한 장비 내에 존재하므로 원격 접속이 불필요하다.

   shard CAS가 shard proxy로 연결할 때 Linux에서는 유닉스 도메인 소켓을 사용하지만 Windows에서는 유닉스 도메인 소켓이 없어 포트를 사용한다(위의 예 참고). shard proxy 하나 당 여러 개의 shard CAS가 연결될 수 있다. shard CAS의 최소, 최대 개수는 shard.conf의 MIN_NUM_APPL_SERVER, MAX_NUM_APPL_SERVER에 의해 설정된다. shard proxy 하나가 동시에 연결 가능한 shard CAS의 최대 개수는 shard.conf의 MAX_CLIENT에 의해 설정된다.
  
#. shard CAS는 cubrid.conf에 설정된 cubrid_port_id 포트를 통해 cub_master에게 DB 서버로의 연결을 요청한다.

#. shard CAS와 DB 서버가 연결된다. Linux에서는 CAS가 유닉스 도메인 소켓을 통해 cub_server와 연결되므로 cubrid_port_id 포트를 사용한다. Windows에서는 유닉스 도메인 소켓을 사용할 수 없으므로 임의의 가용 포트를 통해 cub_server와 연결된다. Windows에서 DB server를 운용한다면 브로커 장비와 DB 서버 장비 사이에서는 임의의 가용 포트를 사용하므로, 두 장비 사이에서 방화벽이 해당 프로세스에 대한 포트를 막게 되면 정상 동작을 보장할 수 없게 된다는 점에 주의한다.

#. 이후 shard CAS는 application이 종료되어도 shard CAS가 재시작되지 않는 한 cub_server와 연결을 유지한다.


.. _cwm-cm-ports:

CUBRID Web Manager, CUBRID Manager 서버 사용 포트
-------------------------------------------------

접속 요청을 기다리는(listening) 프로세스들을 기준으로 CUBRID Web Manager, CUBRID Manager 서버가 사용하는 포트는 다음과 같으며, 이들은 OS의 종류와 관계없이 동일하다.

+--------------------------+--------------+----------------+--------------------------+
| listener                 | requester    | port           | 방화벽 존재 시 포트 설정 |
+==========================+==============+================+==========================+
| Manager server           | application  | 8001, 8002     | 개방(open)               |
+--------------------------+--------------+----------------+--------------------------+
| Web Manager server       | application  | 8282           | 개방                     |
+--------------------------+--------------+----------------+--------------------------+

* CUBRID Manager 클라이언트가 CUBRID Manager 서버 프로세스에 접속할 때 사용하는 포트는 cm.conf의 **cm_port**\와 **cm_port** + 1이며 **cm_port**\의 기본값은 8001이다.
* CUBRID Web Manager 클라이언트가 CUBRID Web Manager 서버 프로세스에 접속할 때 사용하는 포트는 cm_httpd.conf의 **listen**\이며 기본값은 8282이다.

            