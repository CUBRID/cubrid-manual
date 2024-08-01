
:meta-keywords: cubrid install, cubrid compatibility, cubrid service, cubrid manager, run cubrid
:meta-description: CUBRID supported platforms, hardware and software requirements, how to install and run CUBRID engine and CUBRID manager.

.. _install-execute:

설치와 실행
===========

.. _supported:

지원 플랫폼 및 설치 권장 사양
-----------------------------

CUBRID가 지원하는 플랫폼과 설치를 위한 하드웨어/소프트웨어 요구 사항은 아래 표와 같다.

+--------------------------------------------------------------+------------------+----------------------+----------------------------+
| 지원 플랫폼                                                  | 요구되는 메모리  | 요구되는 디스크 공간 | 필요 소프트웨어            |
+==============================================================+==================+======================+============================+
| * Windows 64 Bit Windows 7 이상                              | 1G 이상          | 2G 이상(\*)          | Java 저장 프로시저를       |
|                                                              |                  |                      | 사용하는 경우 JDK 1.8 필요 |
| * Linux 계열 64 Bit(Linux kernel 2.4 및 glibc 2.3.4 이상)    |                  |                      |                            |
+--------------------------------------------------------------+------------------+----------------------+----------------------------+

(\*): 처음 설치 시 약 500MB의 디스크 공간이 필요하며, 하나의 DB를 기본 옵션으로 생성할 경우 약 1.5GB의 디스크 공간이 필요하다.

2008 R4.0부터는 CUBRID 패키지 설치 시 CUBRID 매니저 클라이언트가 같이 설치되지 않는다. 따라서 CUBRID 매니저를 사용하려면 이를 추가로 설치해야 한다. CUBRID 설치 패키지는 http://ftp.cubrid.org 에서 받을 수 있다.

CUBRID 매니저 JDBC, PHP, ODBC, OLE DB 등의 드라이버들도 http://ftp.cubrid.org 에서 받을 수 있다.

CUBRID 엔진, 사용 도구 및 드라이버에 대한 자세한 정보는 https://www.cubrid.org 를 참고한다.

버전 호환성
-----------

**응용 프로그램의 호환성**

*   2008 R4.1 또는 그 이상 버전에서 JDBC, PHP, CCI API 등을 사용하는 응용 프로그램은 CUBRID 11.3 브로커에 접근할 수 있다. 다만, JDBC, PHP, CCI 인터페이스에 추가/개선된 기능을 사용하기 위해서는 CUBRID 11.3 버전의 라이브러리를 링크하거나 드라이버를 사용해야 한다. 10.0에서 추가된 :ref:`timezone-type` 을 사용하기 위해서는 드라이버를 업그레이드 해야 한다.

*   10.2 이전 버전의 드라이버는 JSON 타입 컬럼을 Varchar 타입으로 인식한다.

*   새로운 예약어 추가 및 일부 질의에 대한 스펙 변경으로 인해 질의 결과가 과거 버전과 다를 수 있으므로 주의한다.

*   2008 R3.0 이하 버전에서 GLO 클래스를 이용하여 개발된 응용은 BLOB, CLOB 타입에 맞는 응용 및 스키마로 변환하여 사용해야 한다.

**CUBRID 매니저의 호환성**

*   CUBRID 매니저는 CUBRID 2008 R2.2 이상 버전의 서버에 대해서 하위 호환성을 보장하며, 각 서버 버전과 일치하는 CUBRID JDBC 드라이버를 사용한다. 하지만 CUBRID 매니저에서 제공하는 모든 기능을 제대로 사용하기 위해서는 CUBRID 서버 버전보다 높은 버전의 CUBRID 매니저를 사용해야 한다. CUBRID JDBC 드라이버는 CUBRID 설치 시 $CUBRID/jdbc 디렉터리에 포함되어 있다(Linux 환경에서 $CUBRID는 Windows 환경에서는 %CUBRID% 형식으로 사용됨).

*   CUBRID 매니저의 Bit 버전과 JRE의 Bit 버전은 서로 동일해야 한다. 

    예를 들어, 64Bit 버전 DB 서버라도 CUBRID Manager 32Bit 버전을 사용한다면 JRE 또는 JDK 32Bit 버전을 설치해야 한다.

*   CUBRID 2008 R2.2 이상 버전의 드라이버는 CUBRID 매니저에 기본으로 내장되어 있으며, https://www.cubrid.org 웹사이트에서 별도로 받을 수도 있다.

.. note:: 과거 버전 사용자들은 드라이버, 브로커, DB 서버 모두를 반드시 업그레이드해야 하며, 11.1 이하 버전의 DB 볼륨이 11.3과 호환되지 않으므로 반드시 데이터 마이그레이션을 해야 한다.
    업그레이드 및 데이터 마이그레이션은 :doc:`/upgrade`\ 를 참고한다.

**CUBRID DB 서버와 브로커 간 상호 운용성**

*   CUBRID DB 서버와 브로커를 분리하여 운영하는 경우, 두 장비 간 CUBRID 버전은 동일해야 한다. 그러나 패치 버전만 다른 경우는 호환된다.

    예를 들어, 2008 R4.1 Patch1의 브로커는 2008 R4.1 Patch 10의 DB 서버와 호환되지만 2008 R4.3 DB 서버와는 호환되지 않는다. 9.1 Patch 1의 브로커는 9.1 Patch 10의 DB 서버와 호환되지만 9.2 DB 서버와는 호환되지 않는다.

*   CUBRID DB 서버와 브로커 장비의 운영 체제가 서로 다르더라도 DB 서버의 Bit 버전과 브로커 서버의 Bit 버전이 서로 동일하면 상호 운용성을 보장한다. 

    예를 들어, Linux용 64Bit 버전 DB 서버는 Windows용 64Bit 버전 브로커 서버와 상호 운용이 가능하다.

    DB 서버와 브로커 서버 사이의 관계에 대한 설명은 :doc:`intro`\를 참고한다. 
    
.. _Installing-and-Running-on-Linux:
    
Linux에서의 설치와 실행
-----------------------

**설치 전 확인 사항**

Linux 버전의 CUBRID 데이터베이스를 설치하기 전에 다음 사항을 점검한다.

*   glibc 버전

    glibc 2.3.4 버전 이상만 지원한다.
    glibc 버전은 다음과 같은 방법으로 확인한다. ::
      
        % rpm -q glibc

*   64비트 
    
    10.0 이후 CUBRID는 64비트 버전만 지원한다.  Linux버전은 다음과 같은 방법으로 확인한다. ::
        
        % uname -a
        Linux host_name 2.6.32-696.20.1.el6.x86_64 #1 SMP Fri Jan 26 17:51:45 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux
    
    64비트 Linux에서는 CUBRID 64비트 버전을 설치한다. 
    
*   추가로 설치할 라이브러리
    
    *   Curses Library (rpm -q ncurses)

        CUBRID는 curses 라이브러리를 사용하여 구동되므로 시스템에 curses 라이브러리가 없는 경우 사전에 최신 ncurses-compat-libs 패키지를 설치해야 한다.

    *   gcrypt Library (rpm -q libgcrypt)
    *   stdc++ Library (rpm -q libstdc++)
    
*   /etc/hosts 파일에 호스트 이름과 IP 주소 매핑이 정상인지 확인하기

    호스트 이름과 이에 맞는 IP 주소가 비정상적으로 매핑되어 있으면 DB 서버를 구동할 수 없으므로, 정상적으로 매핑되어 있는지 확인한다.
  
**CUBRID 설치**

설치 프로그램은 바이너리를 포함한 쉘 스크립트로 되어 있어 자동으로 설치할 수 있다. 다음은 리눅스에서 "CUBRID-11.3.0.1089-bd31bd5-Linux.x86_64.sh" 파일을 이용하여 CUBRID를 설치하는 예제이다. 

::

    $ sh CUBRID-11.3.0.1089-bd31bd5-Linux.x86_64.sh
    Do you agree to the above license terms? (yes or no) : yes
    Do you want to install this software(CUBRID) to the default(/home1/cub_user/CUBRID) directory? (yes or no) [Default: yes] : yes
    Install CUBRID to '/home1/cub_user/CUBRID' ...
    In case a different version of the CUBRID product is being used in other machines, 
    please note that the CUBRID 11.3 servers are only compatible with the CUBRID 11.3 clients and vice versa.
    Do you want to continue? (yes or no) [Default: yes] : yes
    Copying old .cubrid.sh to .cubrid.sh.bak ...

    CUBRID has been successfully installed.

    demodb has been successfully created.

    If you want to use CUBRID, run the following commands
    $ . /home1/cub_user/.cubrid.sh
    $ cubrid service start

위의 예제와 같이 다운로드한 파일(CUBRID-11.3.0.1089-bd31bd5-Linux.x86_64.sh)을 설치한 후, CUBRID 데이터베이스를 사용하기 위해서는 CUBRID 관련 환경 정보를 설정해야 한다. 이 설정은 해당 터미널에 로그인할 때 자동 설정되도록 지정되어 있으므로 설치 후 최초 한 번만 수행하면 된다. ::

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

CentOS 6 환경에서 생성한 RPM 파일을 사용하여 CUBRID를 설치할 수 있으며, 일반적인 RPM 유틸리티와 동일한 방법으로 설치하고 삭제할 수 있다. 설치하면 새로운 시스템 그룹(cubrid) 및 사용자 계정(cubrid)이 생성되며, 설치 후에는 cubrid 사용자 계정으로 로그인하여 CUBRID 서비스를 시작해야 한다. ::

    $ rpm -Uvh CUBRID-11.3.0.1089-bd31bd5-Linux.x86_64.rpm

RPM을 실행하면 CUBRID는 "cubrid" 홈 디렉터리(/opt/cubrid)에 설치되고, CUBRID 관련 환경 설정 파일(cubrid.[c]sh)이 /etc/profile.d 디렉터리에 설치된다. 단, demodb는 자동으로 설치되지 않으므로 "cubrid" Linux 계정으로 로그인하여 /opt/cubrid/demo/make_cubrid_demo.sh를 실행하여야 한다. CUBRID가 설치 완료되면 "cubrid" Linux 계정으로 로그인하여 CUBRID 서비스를 다음과 같이 시작한다. ::

    $ cubrid service start

.. note::

    *   **RPM과 의존성**
    
        RPM으로 설치할 때에는 의존성을 꼭 확인해야 한다. 의존성을 무시(\-\-nodeps)하고 설치하면 실행되지 않을 수 있다.

    *   **RPM 삭제 후에도 계정 및 DB는 남아 있음**

        RPM을 삭제하더라도 cubrid 사용자 계정 및 설치 후 생성한 데이터베이스는 보관되므로, 더 이상 필요하지 않은 경우 수동으로 삭제해야 한다.

    *   **Linux에서 시스템 구동 시 CUBRID 자동 구동하기**

        SH 패키지로 CUBRID를 설치했다면 $CUBRID/share/init.d 디렉터리에 cubrid라는 스크립트가 포함되어 있다. 이 파일 안의 **CUBRID_USER** 라는 환경 변수 값을 CUBRID를 설치한 Linux 계정으로 변경한 후, /etc/init.d에 등록하면 service나 chkconfig 명령을 사용하여 Linux 시스템 구동 시 CUBRID를 자동으로 구동할 수 있다.
        
        RPM 패키지로 CUBRID를 설치했다면 /etc/init.d 디렉터리에 cubrid 스크립트가 추가된다. 그러나 cubrid 스크립트 파일 안의 $CUBRID_USER 환경 변수를 cubrid 계정으로 변경하는 작업이 필요하다.
        
    *   **/etc/hosts 파일에 호스트 이름과 IP 주소 매핑이 정상인지 확인하기**

        호스트 이름과 이에 맞는 IP 주소가 비정상적으로 매핑되어 있으면 DB 서버를 구동할 수 없으므로, 정상적으로 매핑되어 있는지 확인한다.

**CUBRID 업그레이드**

다른 버전의 CUBRID가 설치된 디렉터리를 CUBRID를 설치할 디렉터리로 지정하면, 해당 디렉터리가 존재하는 것을 알리고 덮어쓸 것인지 확인한다. **no** 를 입력하면 설치가 중단된다. ::

    Directory '/home1/cub_user/CUBRID' exist!
    If a CUBRID service is running on this directory, it may be terminated abnormally.
    And if you don't have right access permission on this directory(subdirectories or files), install operation will be failed.
    Overwrite anyway? (yes or no) [Default: no] : yes

CUBRID를 설치하고 설정 파일을 구성할 때 기존의 설정 파일을 그대로 사용할 것인지, 새 설정 파일을 사용할 것인지 확인한다. **yes** 를 입력하면 기존의 설정 파일을 확장자가 .bak인 백업 파일로 보관한다. ::

    The configuration file (.conf or .pass) already exists. Do you want to overwrite it? (yes or no) : yes

과거 버전에서 새 버전으로 데이터베이스를 업그레이드하는 방법에 대한 보다 자세한 내용은 :doc:`upgrade` 를 참고한다.

**환경 설정**

서비스 포트 등 사용자 환경에 맞춰 설정을 변경하려면 **$CUBRID/conf** 디렉터리에서 설정 파일의 파라미터를 수정한다.  자세한 내용은 :ref:`Installing-and-Running-on-Windows`\ 의 환경 설정을 참고한다.

**CUBRID 인터페이스 설치**

CCI, JDBC, PHP, ODBC, OLE DB, ADO.NET, Ruby, Python, Node.js 등의 인터페이스 모듈은 https://www.cubrid.org/downloads 에서 최신 정보를 확인할 수 있고 관련 파일을 내려받아 설치할 수 있다.

.. FIXME You can see the latest information on interface modules such as CCI, JDBC, PHP, ODBC, OLE DB, ADO.NET, Ruby, Python and Node.js and install them by downloading files from http://www.cubrid.org/downloads.  

각 드라이버에 대한 간단한 설명은 :doc:`/api/index` 를 참고한다.
    
**CUBRID 도구 설치**

CUBRID 매니저 등의 도구는 https://www.cubrid.org/downloads 에서 최신 정보를 확인할 수 있고 관련 파일을 내려받아 설치할 수 있다.

.. FIXME CUBRID 웹매니저는 CUBRID 설치 시 같이 설치된다. 자세한 설명은 `CUBRID 웹 매니저 매뉴얼 <http://www.cubrid.org/wiki_tools/entry/cubrid-web-manager-manual>`_\ 을 참고한다.
    
.. _Installing-and-Running-on-Windows:

Windows에서의 설치와 실행
-------------------------

**설치 전 확인 사항**

Windows 버전의 CUBRID 데이터베이스를 설치하기 전에 다음 사항을 점검한다.

*   64비트

    CUBRID는 64비트 버전만 지원한다. [내 컴퓨터] > [시스템 등록 정보] 창을 활성화하여 Windows 버전 비트를 확인할 수 있다. 64비트 Windows에 CUBRID 64비트 버전을 설치한다.

    .. warning:: 10.1이 32비트 Windows의 마지막 릴리스이다.

**설치 과정**

**1단계: 설치 디렉터리 지정**

**2단계: 샘플 데이터베이스 생성**
    
    샘플 데이터베이스를 생성하려면 약 1.5GB의 디스크 공간이 필요하다. 

**3단계: 설치 완료**

    우측 하단에 CUBRID Service Tray가 나타난다. 
    
.. note:: 

    CUBRID는 설치하고 나면 시스템 재구동 시 자동으로 실행하게 되어 있다. 시스템 재구동 시 자동 실행을 중단하려면 "제어판 > 시스템 및 보안 > 관리 도구 > 서비스 > CUBRIDService" 에서 더블클릭한 후 나타난 팝업 창에서 시작 유형을 수동으로 변경한다.

**설치 후 확인 사항**

*   CUBRID Service Tray 구동 여부

    시스템을 시작할 때 CUBRID Service Tray가 자동으로 구동되지 않는다면 다음 사항을 확인하도록 한다.

    *   [시작 버튼] > [제어판] > [관리 도구] > [서비스]의 Task Scheduler가 시작되어 있는지 확인하고, 그렇지 않으면 Task Scheduler를 시작한다.
    *   [시작 버튼] > [모든 프로그램] > [시작프로그램]에 CUBRID Service Tray가 등록되어 있는지 확인하고, 그렇지 않으면 CUBRID Service Tray를 등록한다.
        
**CUBRID 업그레이드**

과거 버전의 CUBRID가 이미 설치된 환경에 새로운 버전의 CUBRID를 설치하는 경우, 시스템 트레이에서 [CUBRID Service Tray] > [Exit]를 선택하여 운영 중인 서비스를 종료한 후 과거 버전의 CUBRID를 제거해야 한다. "데이터베이스와 설정 파일을 모두 삭제하겠습니까?"라고 묻는 대화 상자가 나타나면, 과거 버전의 데이터베이스가 삭제되지 않도록[아니오]를 클릭한다.

과거 버전에서 새 버전으로 데이터베이스를 업그레이드하는 방법에 대한 보다 자세한 내용은 :doc:`upgrade` 를 참고한다.

**환경 설정**

서비스 포트 등 사용자 환경에 맞춰 설정을 변경하려면 **%CUBRID%\\conf** 디렉터리에서 다음 설정 파일의 파라미터 값을 변경한다. 방화벽이 설정되어 있다면 CUBRID에서 사용하는 포트들을 열어두어야(open) 한다. CUBRID가 사용하는 포트에 대한 자세한 내용은 :ref:`connect-to-cubrid-server`\ 을 참고한다.

*   **cm.conf**

    CUBRID 매니저용 설정 파일이다. **cm_port** 는 매니저 서버 프로세스, 매니저 서버 프로세스가 사용하는 포트로 기본값은 **8001** 이다. 

    .. FIXME: For details, see `CUBRID Manager Manual <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual>`_.
    
*   **cubrid.conf**

    서버 설정용 파일로, 운영하려는 데이터베이스의 메모리, 동시 사용자 수에 따른 스레드 수, 브로커와 서버 사이의 통신 포트 등을 설정한다. **cubrid_port_id** 는 마스터 프로세스가 사용하는 포트로, 기본값은 *1523* 이다. 자세한 내용은 :ref:`cubrid-conf`\ 를 참조한다.

*   **cubrid_broker.conf**

    브로커 설정용 파일로, 운영하려는 브로커가 사용하는 포트, 응용서버(CAS) 수, SQL LOG 등을 설정한다. **BROKER_PORT** 는 브로커가 사용하는 포트이며, 실제 JDBC와 같은 드라이버에서 보는 포트는 해당 브로커의 포트이다. **APPL_SERVER_PORT** 는 Windows에서만 추가하는 파라미터로, 브로커 응용 서버(CAS)가 사용하는 포트이다. 기본값은 **BROKER_PORT** + 1이다. **APPL_SERVER_PORT** 값을 기준으로 1씩 더한 포트들이 CAS 개수만큼 사용된다.
    예를 들어 **APPL_SERVER_PORT** 값이 35000이고 **MAX_NUM_APPL_SERVER** 값에 의한 CAS의 최대 개수가 50이면 CAS에서 listen하는 포트는 35000, 35001, ..., 35049이다.
    자세한 내용은 :ref:`parameter-by-broker`\ 를 참조한다. 
    
    **CCI_DEFAULT_AUTOCOMMIT** 브로커 파라미터는 2008 R4.0부터 지원하기 시작했고, 이때 기본값은 **OFF** 였다가 2008 R4.1부터는 기본값이 **ON** 으로 바뀌었다. 따라서 2008 R4.0에서 2008 R4.1 이상 버전으로 업그레이드하는 사용자는 이 값을 OFF로 바꾸거나, 응용 프로그램의 함수에서 자동 커밋 모드를 OFF로 설정해야 한다.

**CUBRID 인터페이스 설치**

https://www.cubrid.org/downloads 에서 CCI, JDBC, PHP, ODBC, OLE DB, ADO.NET, Ruby, Python 및 Node.js와 같은 인터페이스 모듈을 다운로드할 수 있다.

.. FIXME: You can see the latest information on interface modules such as JDBC, PHP, ODBC, and OLE DB and install them by downloading files from `<http://www.cubrid.org/wiki_apis>`_.

각 드라이버에 대한 간단한 설명은 :doc:`/api/index`\ 를 참고한다.

**CUBRID 도구 설치**

https://www.cubrid.org/downloads 에서 CUBRID Manager 및 CUBRID Migration Toolkit을 비롯한 다양한 도구를 다운로드할 수 있다.

.. FIXME: You can see the latest information on interface modules such as JDBC, PHP, ODBC, and OLE DB and install them by downloading files from `<http://www.cubrid.org/wiki_apis>`_.

압축 파일로 설치하기
--------------------

Linux에서 tar.gz 파일로 CUBRID 설치
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**설치 전 확인 사항**

Linux 버전의 CUBRID 데이터베이스를 설치하기 전에 다음 사항을 점검한다.

*   glibc 버전

    glibc 2.3.4 버전 이상만 지원한다.
    glibc 버전은 다음과 같은 방법으로 확인한다. ::
      
        % rpm -q glibc

*   64비트 여부 
    
    10.0 이후CUBRID는 64비트 버전만 지원한다.  Linux 버전은 다음과 같은 방법으로 확인한다. ::
        
        % uname -a
        Linux host_name 2.6.18-53.1.14.el5xen #1 SMP Wed Mar 5 12:08:17 EST 2008 x86_64 x86_64 x86_64 GNU/Linux
    
    64비트 Linux에서는 CUBRID 64비트 버전을 설치한다. 
    
*   추가로 설치할 라이브러리
    
    * Curses Library (rpm -q ncurses)

      CUBRID는 Curses 라이브러리 버전 5와 함께 패키지된다. 시스템에 최신 버전이 있고 다운 그레이드할 수 없는 경우 ncurses-compat-libs 패키지를 설치해야할 수 있다.

    * gcrypt Library (rpm -q libgcrypt)
    * stdc++ Library (rpm -q libstdc++)
    
*   /etc/hosts 파일에 호스트 이름과 IP 주소 매핑이 정상인지 확인하기

    호스트 이름과 이에 맞는 IP 주소가 비정상적으로 매핑되어 있으면 DB 서버를 구동할 수 없으므로, 정상적으로 매핑되어 있는지 확인한다.

**설치 과정**

    **설치 디렉터리 지정**

    *   압축 파일을 설치하려는 경로에 풀어 놓는다.

        ::
        
            tar xvfz CUBRID-11.3.0.1089-bd31bd5-Linux.x86_64.tar.gz /home1/cub_user/

        /home1/cub_user/ 이하에 CUBRID 디렉터리가 생기고 그 이하에 파일이 생성된다.

    **환경 변수 설정**

    #.  사용자의 홈 디렉터리(/home1/cub_user) 이하에서 자동으로 실행되는 쉘 스크립트에 아래의 환경 변수를 추가한다.
    
        **$CUBRID_DATABASES** 변수에 설정된 디렉토리 생성이 필요하다. 적절한 권한이 있는 임의의 디렉토리를 지정할 수 있다.

        다음은 bash 쉘로 수행하는 경우 .bash_profile에 다음을 추가하는 예이다.

        ::
        
            export CUBRID=/home1/cub_user/CUBRID
            export CUBRID_DATABASES=$CUBRID/databases
            
    #.  **CLASSPATH** 환경 변수에  CUBRID JDBC 라이브러리 파일 이름을 추가한다.
    
        ::
        
            export CLASSPATH=$CUBRID/jdbc/cubrid_jdbc.jar:$CLASSPATH
            
    #.  **PATH** 환경 변수에 CUBRID bin 디렉터리를 추가한다.
      
        ::
        
            export PATH=$CUBRID/bin:$PATH
                
    **DB 생성**
        
    *   콘솔 창에서 DB를 생성할 디렉터리로 이동해서 DB를 직접 생성한다.

        ::
        
            cd $CUBRID_DATABASES
            mkdir testdb
            cd testdb
            cubrid createdb --db-volume-size=128M --log-volume-size=128M testdb en_US

    **부팅 시 자동 시작**

    *   **$CUBRID/share/init.d** 디렉터리에 cubrid라는 스크립트가 포함되어 있다. 이 파일 안의 **CUBRID_USER** 환경 변수 값을 CUBRID를 설치한 Linux 계정으로 변경한 후, /etc/init.d에 등록하면 service나 chkconfig 명령을 사용하여 Linux 시스템 구동 시 CUBRID를 자동으로 구동할 수 있다.
            
    **DB 자동 구동**    

    *   부팅 시 생성한 DB가 구동되게 하려면 **$CUBRID/conf/cubrid.conf** 에서 다음을 수정한다.

        ::
            
            [service]
            service=server, broker, manager
            server=testdb

    *   service 파라미터에는 자동으로 구동할 프로세스들을 지정한다.
    *   server 파라미터에는 자동으로 구동할 DB 이름을 지정한다.
        
CUBRID 설치 이후 환경 설정, 도구 설치, 인터페이스 설치 등은 :ref:`Installing-and-Running-on-Linux`\을 확인하도록 한다.
            
Windows에서 zip 파일로 CUBRID 설치
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**설치 전 확인 사항**

Windows 버전의 CUBRID 데이터베이스를 설치하기 전에 다음 사항을 점검한다.

*   64비트 여부

    CUBRID는 64비트 버전만 지원한다. [내 컴퓨터] > [시스템 등록 정보] 창을 활성화하여 Windows 버전 비트를 확인할 수 있다. 64비트 Windows에 CUBRID 64비트 버전을 설치한다.

    .. warning:: 10.1이 32비트 Windows의 마지막 릴리스이다.

**설치 과정**

    **설치 디렉터리 지정**

    *   압축 파일을 설치하려는 경로에 풀어 놓는다.

        ::
        
            C:\CUBRID
    *   **$CUBRID_DATABASES** 변수에 설정된 디렉토리 생성이 필요하다. 적절한 권한이 있는 임의의 디렉터리를 지정할 수 있다.

    **환경 변수 설정**

    #.  [시작 버튼] > [컴퓨터] > (오른쪽 마우스 버튼 클릭) > [속성] -> [고급 시스템 설정] > [환경변수]를 선택한다.
    #.  시스템 변수 항목에 [새로 만들기]를 클릭한 후 아래와 같이 시스템 변수를 추가한다.
    
        ::
        
            CUBRID = C:\CUBRID
            CUBRID_DATABASES = %CUBRID%\databases
            
    #.  **CLASSPATH** 시스템 변수에  CUBRID JDBC 라이브러리 파일 이름을 추가한다.
    
        ::
        
            %CUBRID%\jdbc\cubrid_jdbc.jar       
            
    #.  **Path** 시스템 변수에 CUBRID bin 과 CUBRID cci\bin 디렉터리를 추가한다.
      
        ::

            %CUBRID%\bin;%CUBRID%\cci\bin

	.. note:: 
	
	    CUBRID를 사용하기 위한 환경변수 설정은 아래의 Batch file(cubrid_env.bat)을 이용하여 설정을 편리하게 할 수 있다.
	    C:\\CUBRID\\shard\\windows_scripts\\cubrid_env.bat


    **DB 생성**
        
    *   cmd 명령으로 콘솔 창을 띄운 후 DB를 생성할 디렉터리로 이동해서 DB를 직접 생성한다.

        ::
        
            cd C:\CUBRID\databases
            md testdb
            cd testdb
            c:\CUBRID\databases\testdb>cubrid createdb --db-volume-size=128M --log-volume-size=128M testdb en_US
    
    **부팅 시 자동 시작**
    
    *   설치한 CUBRID가 Windows 시스템 부팅 시 자동으로 시작되게 하려면 CUBRID 서비스가 먼저 Windows 서비스에 등록되어야 한다. 
        
        #.  CUBRID 서비스를 Windows 서비스에 등록한다.

            ::
            
                C:\CUBRID\bin\ctrlService.exe -i C:\CUBRID\bin
            
        #.  CUBRID 서비스를 구동/정지하는 방법은 아래와 같다.
        
            ::
            
                C:\CUBRID\bin\ctrlService.exe -start/-stop
            
    **DB 자동 구동**    

    *   Windows 부팅 시 DB가 구동되게 하려면 C:\\CUBRID\conf\\cubrid.conf에서 다음을 수정한다.

        ::
            
            [service]
            service=server, broker, manager
            server=testdb

        *   service 파라미터에는 자동으로 구동할 프로세스들을 지정한다.
        *   server 파라미터에는 자동으로 구동할 DB 이름을 지정한다.

    **서비스에서 제거**

    *   등록한 CUBRID Service를 제거하려면 다음을 수행한다.

        ::
        
            C:\CUBRID\bin\ctrlService.exe -u

**CUBRID Service Tray 등록**
    
zip 파일로 CUBRID를 설치하는 경우 CUBRID Service Tray가 자동으로 등록되지 않으므로, 이를 사용하려면 수동으로 등록하는 절차가 필요하다.
    
#.  C:\\CUBRID\\bin\\CUBRID_Service_Tray.exe 파일의 바로 가기를 시작 > 모든프로그램 > 시작프로그램에 생성한다.

#.  시작 > 보조 프로그램 > 실행 창에서 regedit를 입력하면 레지스트리 편집기가 실행된다.

#.  컴퓨터 > HKEY_LOCAL_MACHINE > SOFTWARE에 CUBRID 폴더를 생성한다.

#.  생성한 CUBRID 폴더에 cmclient 폴더를 생성(새로 만들기 > 키)하고 아래의 항목을 추가(새로 만들기 > 문자열 값)한다.

    ::
    
        이름          종류      데이터

        ROOT_PATH     REG_SZ     C:\CUBRID\cubridmanager
        
#.  생성한 CUBRID 폴더에 cmserver 폴더를 생성(새로 만들기 > 키)하고 아래의 항목을 추가(새로 만들기 > 문자열 값)한다.

    ::
    
        이름          종류      데이터

        ROOT_PATH     REG_SZ     C:\CUBRID

#.  생성한 CUBRID 폴더에 CUBRID 폴더를 생성(새로 만들기 > 키)하고 아래의 항목을 추가(새로 만들기 > 문자열 값)한다.


    ::
    
        이름          종류      데이터

        ROOT_PATH     REG_SZ     C:\CUBRID

#.  Windows를 재부팅하면 CUBRID Service Tray가 오른쪽 하단에 생긴다.
    
**설치 후 확인 사항**

*   CUBRID Service Tray 구동 여부

    시스템을 시작할 때 CUBRID Service Tray가 자동으로 구동되지 않는다면 다음 사항을 확인하도록 한다.

    *   [시작 버튼] > [제어판] > [관리 도구] > [서비스]의 Task Scheduler가 시작되어 있는지 확인하고, 그렇지 않으면 Task Scheduler를 시작한다.

    *   [시작 버튼] > [모든 프로그램] > [시작프로그램]에 CUBRID Service Tray가 등록되어 있는지 확인하고, 그렇지 않으면 CUBRID Service Tray를 등록한다.

CUBRID 설치 이후 환경 설정, 도구 설치, 인터페이스 설치 등은 :ref:`Installing-and-Running-on-Windows`\을 확인하도록 한다.

.. note:: 

    zip 파일로 CUBRID를 설치한 경우, CUBRID 실행에 필요한 DLL 이 없어 실행이 되지 않을 수 있다.
    이런 경우 Microsoft Visual C++ Redistributable 를 설치해야 한다.
    
    *   **Microsoft Visual C++ Redistributable**
    
        https://visualstudio.microsoft.com/ko/downloads/
        
