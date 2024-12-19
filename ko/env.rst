
:meta-keywords: configure cubrid, cubrid port, cubrid language, cubrid environment, cubrid settings
:meta-description: The following environment variables need to be set in order to use the CUBRID. The necessary environment variables are automatically set when the CUBRID system is installed or can be changed, as needed, by the user.

환경 변수 설정
==============

CUBRID를 사용하기 위해서는 다음의 환경 변수들이 설정되어 있어야 한다. 필요한 환경 변수들은 CUBRID 시스템을 설치하면 자동으로 설정되나 필요에 의해서 사용자가 적절히 변경할 수도 있다.

CUBRID 환경 변수
----------------

*   **CUBRID**: CUBRID 시스템이 설치된 위치를 지정하는 기본 환경 변수이다. CUBRID 시스템에 포함된 모든 프로그램은 이 환경 변수를 참조하므로 정확히 설정되어 있어야 한다.

*   **CUBRID_DATABASES**: **databases.txt** 파일의 위치를 지정하는 환경 변수이다. CUBRID 시스템은 **$CUBRID_DATABASES/databases.txt** 파일에 데이터베이스 볼륨들의 절대 경로를 저장 관리한다. :ref:`databases-txt-file`\ 을 참고한다.

*   **CUBRID_MSG_LANG**: CUBRID 시스템이 명령어 사용법 메시지와 오류 메시지를 출력할 때 사용할 언어를 지정하는 환경 변수이다. 제품 설치 시 초기 설정 값은 없으며, 설정 값이 없으면 **en_US**로 설정된다. 환경변수로 설정할 수 있는 언어는 en_US, en_US.utf8, ko_KR.euckr 및 ko_KR.utf8 이다. 

.. note:: 

    *  CUBRID Manager 사용자는 DB 서버 노드의 **CUBRID_MSG_LANG** 환경 변수를 **en_US** 로 설정해야만 데이터베이스 관련 작업 이후 출력되는 메시지를 정상적으로 확인할 수 있다.  **CUBRID_MSG_LANG** 환경 변수가 **en_US** 가 아닌 경우 메시지는 비정상적으로 출력되지만 데이터베이스의 작업은 정상적으로 실행된다.
    *  변경한 **CUBRID_MSG_LANG** 을  적용하려면 DB 서버 노드의 CUBRID 시스템이 반드시 재시작(cubrid service stop; cubrid service start)되어야 한다.

*   **CUBRID_TMP**: CUBRID DBMS를 구동에 필요한 임시 파일을 저장하기 위한 디렉터리를 설정하는 환경 변수이다. 임시 파일은 텍스트형과 프로세스간 통신을 위한 UNIX 도메인 소켓의 두 종류가 있다. 이 환경 변수가 설정된 경우 임시 파일은 환경 변수에 지정된 디렉터리에 생성된다.

    *   **텍스트형** 임시 파일은 데이터베이스 서버, csql 이외에도 **plandump** 등과 같은 유틸리티에서 생성하며 이 환경 변수가 설정되지 않으면 다음의 디렉터리를 사용한다.

        * Linux: /tmp
        * Windows: C:\\Windows\\TEMP

    *   **UNIX 도메인 소켓 파일** 은 이 환경 변수가 설정되지 않은 경우 프로세스 별로 아래의 경로에 생성된다 (이 소켓 파일은 **Linux에만 해당** 되며 Windows에는 적용되지 않는다).
    
        *   cub_master 프로세스: **/tmp** 디렉터리
        *   cub_broker 프로세스: **$CUBRID/var/CUBRID_SOCK** 디렉터리
        *   cub_javasp 프로세스: **$CUBRID/var/CUBRID_SOCK** 디렉터리

.. note::

    * **CUBRID_TMP** 환경 변수가 설정된 경우, cub_javasp 프로세스(Java VM 내장) 구동시 사용하는  **java_stored_procedure_jvm_options** 중  임시 파일 저장 경로를 지정하는 **java.io.tmpdir** 의 설정값은 무시된다.
    * Windows에서 이 환경 변수를 설정하기 위해서는 registry에 CUBRID_TMP key를 추가해야한다 (**%CUBRID%**\\share\\windows_scripts\\cubrid_env.bat 참조).

**CUBRID_TMP** 의 값에는 다음과 같은 제약 사항이 있다.

*   unix socket의 path의 최대 크기가 108이므로 다음과 같이 **$CUBRID_TMP** 에 108보다 긴 경로를 입력하면 에러를 출력한다. 

    ::

        $ export CUBRID_TMP=/home1/testusr/cubrid=/tmp/123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
        $ cubrid server start apricot

        The $CUBRID_TMP is too long. (/home1/testusr/cubrid=/tmp/123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789)

*   상대 경로를 입력하면 에러를 출력한다. 

    ::

        $ export CUBRID_TMP=./var 
        $ cubrid server start testdb

        The $CUBRID_TMP should be an absolute path. (./var)

**CUBRID_TMP** 는 CUBRID가 사용하는 유닉스 도메인 소켓의 기본 경로에서 발생할 수 있는 다음 문제를 회피하기 위해 사용할 수 있다.

*   **/tmp** 는 주로 Linux에서 임시 파일을 저장하는 공간으로, 시스템 관리자가 이 공간을 주기적으로 임의 삭제하는 경우 유닉스 도메인 소켓까지 삭제될 수 있다. 이러한 경우 **$CUBRID_TMP** 를 **/tmp** 가 아닌 다른 경로로 설정한다.
*   유닉스 도메인 소켓 파일의 경로 최대 길이는 108인데, CUBRID의 설치 경로가 길어서 cub_broker용 유닉스 도메인 소켓 파일을 저장하는 **$CUBRID/var/CUBRID_SOCK** 경로의 길이가 108을 넘는 경우 브로커를 구동할 수 없다. 따라서 **$CUBRID_TMP** 를 108이 넘지 않는 경로로 설정해야 한다.

이들 환경변수는 CUBRID를 설치하면서 이미 설정되었으나, 설정을 확인하기 위해서는 다음 명령을 사용할 수 있다.

*   Linux 

    ::

        % printenv CUBRID
        % printenv CUBRID_DATABASES
        % printenv CUBRID_MSG_LANG
        % printenv CUBRID_TMP

*   Windows 

    ::

        C:\> set CUBRID

OS 환경 변수 및 Java 환경 변수
------------------------------

*   PATH: Linux 환경에서 PATH 환경 변수에는 CUBRID 시스템의 실행 파일이 있는 디렉터리인 **$CUBRID/bin** 이 포함되어 있어야 한다.

*   LD_LIBRARY_PATH: Linux 환경에서는 **LD_LIBRARY_PATH** (혹은 **SHLIB_PATH** 나 **LIBPATH**) 환경 변수에 CUBRID 시스템의 동적 라이브러리 파일(libjvm.so)이 있는 디렉터리인 **$CUBRID/lib** 과 **$CUBRID/cci/lib** 이 포함되어 있어야 한다.

*   Path: Windows 환경에서 Path 환경 변수에는 CUBRID 시스템의 실행 파일이 있는 디렉터리인 **%CUBRID%\\bin** 과 **%CUBRID%\\cci\\bin** 이 포함되어 있어야 한다.

*   JAVA_HOME: CUBRID 시스템에서 자바 저장 프로시저 기능을 사용하기 위해서는 Java Runtime Environment (JRE) 1.6 이상 버전이 설치되어야 하고 **JAVA_HOME** 환경 변수에 해당 디렉터리가 지정되어야 한다. :ref:`cubrid-javasp-server-config` 을 참고한다.

*   JVM_PATH: CUBRID 시스템에서 자바 저장 프로시저 기능을 사용하기 위해서 **JAVA_HOME**\에서 JVM 라이브러리 (**libjvm**)을 찾는 대신 **JVM_PATH** 환경 변수를 설정하여 명시적으로 라이브러리의 경로를 지정할 수 있다. :ref:`cubrid-javasp-server-config` 을 참고한다.

환경 변수 설정
--------------

**Windows 환경인 경우**

Windows 환경에서 CUBRID 시스템을 설치한 경우는 설치 프로그램이 필요한 환경 변수를 자동으로 설정한다. [시스템 등록 정보] 대화 상자의 [고급] 탭에서 [환경 변수]를 클릭하면 나타나는 [환경 변수] 대화 상자에서 확인할 수 있으며, [편집] 버튼을 통해 변경할 수 있다. Windows 환경에서 환경 변수를 변경하는 방법에 대한 상세한 정보는 Windows 도움말을 참고한다.

.. image:: /images/image4.jpg

**Linux 환경인 경우**

Linux 환경에서 CUBRID 시스템을 설치한 경우는 설치 프로그램이 **.cubrid.sh** 혹은 **.cubrid.csh** 파일을 자동으로 생성하고 설치 계정의 셸 로그인 스크립트에서 자동으로 호출하도록 구성한다. 다음은 sh이나 bash 등을 사용하는 환경에서 생성된 **.cubrid.sh** 파일의 환경 변수 설정 내용이다.

::

    CUBRID=/home1/cub_user/CUBRID
    CUBRID_DATABASES=/home1/cub_user/CUBRID/databases
    ld_lib_path=`printenv LD_LIBRARY_PATH`
    
    if [ "$ld_lib_path" = "" ]
    then
        LD_LIBRARY_PATH=$CUBRID/lib:$CUBRID/cci/lib
    else
        LD_LIBRARY_PATH=$CUBRID/lib:$CUBRID/cci/lib:$LD_LIBRARY_PATH
    fi

    SHLIB_PATH=$LD_LIBRARY_PATH
    LIBPATH=$LD_LIBRARY_PATH
    PATH=$CUBRID/bin:$CUBRID/cubridmanager:$PATH
    
    export CUBRID
    export CUBRID_DATABASES
    export LD_LIBRARY_PATH
    export SHLIB_PATH
    export LIBPATH
    export PATH

.. _language-setting:

언어 및 문자셋 설정
-------------------

CUBRID 데이터베이스 관리 시스템은 사용할 언어와 문자셋을 DB 생성 시 DB 이름 뒤에 지정한다(예: cubrid createdb testdb ko_KR.utf8). 현재 언어와 문자셋으로 설정될 수 있는 값은 다음과 같다.

*   **en_US.iso88591**: 영어 ISO-8859-1 인코딩 (.iso88591 생략 가능)
*   **ko_KR.euckr**: 한국어 EUC-KR 인코딩
*   **ko_KR.utf8**: 한국어 UTF-8 인코딩(.utf8 생략 가능)
*   **de_DE.utf8**: 독일어 UTF-8 인코딩
*   **es_ES.utf8**: 스페인어 UTF-8 인코딩
*   **fr_FR.utf8**: 프랑스어 UTF-8 인코딩
*   **it_IT.utf8**: 이태리어 UTF-8 인코딩
*   **ja_JP.utf8**: 일본어 UTF-8 인코딩
*   **km_KH.utf8**: 캄보디아어 UTF-8 인코딩
*   **tr_TR.utf8**: 터키어 UTF-8 인코딩(.utf8 생략 가능)
*   **vi_VN.utf8**: 베트남어 UTF-8 인코딩
*   **zh_CN.utf8**: 중국어 UTF-8 인코딩
*   **ro_RO.utf8**: 루마니아어 UTF-8 인코딩

CUBRID의 언어와 문자셋 설정은 데이터를 쓰거나 읽을 때 영향을 미치며, 프로그램들이 출력하는 메시지에도 해당 언어가 사용된다.

문자셋, 로캘 및 콜레이션 설정과 관련된 자세한 내용은 :doc:`sql/i18n` 을 참고한다.

.. _connect-to-cubrid-server:

포트 설정
=========

포트가 개방되어 있지 않은 환경에서 사용하는 경우, CUBRID가 사용하는 포트들을 개방해야 한다.

다음은 CUBRID가 사용하는 포트에 대해 하나의 표로 정리한 것이다. 각 포트는 상대방의 접속을 대기하는 listener 쪽에서 개방되어야 한다.

Linux 방화벽에서 특정 프로세스에 대한 포트를 개방하려면 해당 방화벽 프로그램의 설명을 따른다.

Windows에서 임의의 가용 포트를 사용하는 경우는 어떤 포트를 개방할 지 알 수 없으므로  Windows 메뉴의 "제어판" 검색창에서  "방화벽"을 입력한 후, "Windows 방화벽 > Windows 방화벽을 통해 프로그램 또는 기능 허용"에서 포트 개방을 원하는 프로그램을 추가한다. 

Windows에서 특정 포트를 지정하기 번거로운 경우에도 이 방법을 사용할 수 있다. 일반적으로 Windows 방화벽에서 특정 프로그램을 지정하지 않고 포트를 여는 것보다 허용되는 프로그램 목록에 프로그램을 추가하는 것이 보다 안전하므로 이 방식을 권장한다.

*   cub_broker에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_broker.exe"를 추가한다.
*   CAS에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_cas.exe"를 추가한다.
*   cub_master에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_master.exe"를 추가한다.
*   cub_server에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_server.exe"를 추가한다.
*   CUBRID 매니저에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_cmserver.exe"를 추가한다.
*   CUBRID 자바 저장 프로시저 서버에 대한 모든 포트를 개방하려면 "%CUBRID%\\bin\\cub_javasp.exe"를 추가한다.
    
브로커 장비 또는 DB 서버 장비에서 Linux용 CUBRID를 사용한다면 Linux 포트가 모두 개방되어 있어야 한다.
브로커 장비 또는 DB 서버 장비에서 Windows용 CUBRID를 사용한다면 Windows 포트가 모두 개방되어 있거나, 관련 프로세스들이 모두 Windows 방화벽에서 허용되는 목록에 추가되어 있어야 한다.
     
+---------------+--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
| 구분          | listener     | requester     | Linux 포트                 | Windows 포트                                        | 방화벽 포트 설정         | 설명         |
+===============+==============+===============+============================+=====================================================+==========================+==============+
| 기본 사용     | cub_broker   | application   | BROKER_PORT                | BROKER_PORT                                         | 개방(open)               | 일회성 연결  |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | CAS          | application   | BROKER_PORT                | APPL_SERVER_PORT ~ (APP_SERVER_PORT + CAS 개수 - 1) | 개방                     | 연결 유지    |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | cub_master   | CAS           | cubrid_port_id             | cubrid_port_id                                      | 개방                     | 일회성 연결  |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | cub_server   | CAS           | cubrid_port_id             | 임의의 가용 포트                                    | Linux: 개방              | 연결 유지    |
|               |              |               |                            |                                                     |                          |              |
|               |              |               |                            |                                                     | Windows: 프로그램        |              |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | 클라이언트   | cub_server    | ECHO(7)                    | ECHO(7)                                             | 개방                     | 주기적 연결  |
|               | 장비(*)      |               |                            |                                                     |                          |              |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | 서버         | CAS, CSQL     | ECHO(7)                    | ECHO(7)                                             | 개방                     | 주기적 연결  |
|               | 장비(**)     |               |                            |                                                     |                          |              |
+---------------+--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
| HA 사용       | cub_broker   | application   | BROKER_PORT                | 미지원                                              | 개방                     | 일회성 연결  |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | CAS          | application   | BROKER_PORT                | 미지원                                              | 개방                     | 연결 유지    |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | cub_master   | CAS           | cubrid_port_id             | 미지원                                              | 개방                     | 일회성 연결  |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | cub_master   | cub_master    | ha_port_id                 | 미지원                                              | 개방                     | 주기적 연결, |
|               |              |               |                            |                                                     |                          | heartbeat    |
|               | (slave)      | (master)      |                            |                                                     |                          | 확인         |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | cub_master   | cub_master    | ha_port_id                 | 미지원                                              | 개방                     | 주기적 연결, |
|               |              |               |                            |                                                     |                          | heartbeat    |
|               | (master)     | (slave)       |                            |                                                     |                          | 확인         |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | cub_server   | CAS           | cubrid_port_id             | 미지원                                              | 개방                     | 연결 유지    |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | 클라이언트   | cub_server    | ECHO(7)                    | 미지원                                              | 개방                     | 주기적 연결  |
|               | 장비(*)      |               |                            |                                                     |                          |              |
|               +--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
|               | 서버         | CAS, CSQL,    | ECHO(7)                    | 미지원                                              | 개방                     | 주기적 연결  |
|               | 장비(**)     | copylogdb,    |                            |                                                     |                          |              |
|               |              | applylogdb    |                            |                                                     |                          |              |
+---------------+--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
| Manager 사용  | Manager      | application   | 8001                       | 8001                                                | 개방                     |              |
|               | 서버         |               |                            |                                                     |                          |              |
+---------------+--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+
| Java SP 사용  | cub_javasp   | cub_server    | java_stored_procedure_port | java_stored_procedure_port                          | 개방                     | 연결 유지    |
+---------------+--------------+---------------+----------------------------+-----------------------------------------------------+--------------------------+--------------+


(*): CAS, CSQL, copylogdb, 또는 applylogdb 프로세스가 존재하는 장비

(**): cub_server가 존재하는 장비

각 구분 별 상세 설명은 아래와 같다.

.. _cubrid-basic-ports:

CUBRID 기본 사용 포트
---------------------

접속 요청을 기다리는(listening) 프로세스들을 기준으로 각 OS 별로 필요한 포트를 정리하면 다음과 같으며, 각 포트는 listener 쪽에서 개방되어야 한다.

+---------------+---------------+----------------+-----------------------------------------------------+--------------------------+------------------------+
| listener      | requester     | Linux port     | Windows port                                        | 방화벽 포트 설정         | 설명                   |
+===============+===============+================+=====================================================+==========================+========================+
| cub_broker    | application   | BROKER_PORT    | BROKER_PORT                                         | 개방(open)               | 일회성 연결            |
+---------------+---------------+----------------+-----------------------------------------------------+--------------------------+------------------------+
| CAS           | application   | BROKER_PORT    | APPL_SERVER_PORT ~ (APP_SERVER_PORT + CAS 개수 - 1) | 개방                     | 연결 유지              |
+---------------+---------------+----------------+-----------------------------------------------------+--------------------------+------------------------+
| cub_master    | CAS           | cubrid_port_id | cubrid_port_id                                      | 개방                     | 일회성 연결            |
+---------------+---------------+----------------+-----------------------------------------------------+--------------------------+------------------------+
| cub_server    | CAS           | cubrid_port_id | 임의의 가용 포트                                    | Linux: 개방              | 연결 유지              |
|               |               |                |                                                     |                          |                        |
|               |               |                |                                                     | Windows: 프로그램        |                        |
+---------------+---------------+----------------+-----------------------------------------------------+--------------------------+------------------------+
| 클라이언트    | cub_server    | ECHO(7)        | ECHO(7)                                             | 개방                     | 주기적 연결            |
| 장비(*)       |               |                |                                                     |                          |                        |
+---------------+---------------+----------------+-----------------------------------------------------+--------------------------+------------------------+
| 서버          | CAS, CSQL     | ECHO(7)        | ECHO(7)                                             | 개방                     | 주기적 연결            |
| 장비(**)      |               |                |                                                     |                          |                        |
+---------------+---------------+----------------+-----------------------------------------------------+--------------------------+------------------------+

(*): CAS 또는 CSQL 프로세스가 존재하는 장비

(**): cub_server가 존재하는 장비

.. note:: Windows에서는 CAS가 cub_server에 접근할 때 사용할 포트를 임의로 정하므로 개방할 포트를 정할 수 없다. 따라서 "Windows 방화벽 >  허용되는 프로그램"에 "%CUBRID%\\bin\\cub_server.exe"을 추가해야 한다.
    
서버 프로세스(cub_server)와 이에 접속하는 클라이언트 프로세스들(CAS, CSQL) 사이에서 상대 노드가 정상 동작하는지 ECHO(7) 포트를 통해 서로 확인하므로, 방화벽 존재 시 ECHO(7) 포트를 개방해야 한다. ECHO 포트를 서버와 클라이언트 양쪽 다 개방할 수 없는 상황이라면 cubrid.conf의 :ref:`check_peer_alive <check_peer_alive>` 파라미터 값을 none으로 설정한다.

다음은 각 프로세스 간 연결 관계를 나타낸 것이다.

::

     application - cub_broker
                 -> CAS  -  cub_master
                         -> cub_server

*   application: 응용 프로세스
*   cub_broker: 브로커 서버 프로세스. application이 연결할 CAS를 선택하는 역할을 수행.
*   CAS: 브로커 응용 서버 프로세스. application과 cub_server를 중계.
*   cub_master: 마스터 프로세스. CAS가 연결할 cub_server를 선택하는 역할을 수행.
*   cub_server: DB 서버 프로세스

프로세스 간 관계 기호 및 의미는 다음과 같다.

*   \- 기호: 최초 한 번만 연결됨을 나타낸다.
*   ->, <- 기호: 연결이 유지됨을 나타내며, -> 의 오른쪽 또는 <-의 왼쪽이 화살을 받는 쪽이다. 화살을 받는 쪽이 처음에 상대 프로세스의 접속을 기다리는(listening) 쪽을 나타낸다.
*   (master): HA 구성에서 master 노드를 나타낸다.
*   (slave): HA 구성에서 slave 노드를 나타낸다.

다음은 응용 프로그램과 DB 사이의 연결 과정을 순서대로 나열한 것이다.

#.  application이 cubrid_broker.conf에 설정된 브로커 포트(BROKER_PORT)를 통해 cub_broker와 연결을 시도한다.
#.  cub_broker는 연결 가능한 CAS를 선택한다.
#.  application과 CAS가 연결된다. 

    Linux에서는 application과 브로커의 네트워크 연결을 브로커가 CAS에 그대로 전달한다. 따라서 application이 CAS와 연결을  위한 별도의 네트워크 포트는 필요하지 않다. Windows에서는 Application이 브로커에 연결되면 브로커가 가용한 CAS에 접속하기 위한 네트워크 포트 번호를 application에 전달한다. Application은 브로커와의 연결을 종료한 후, 전달 받은 네트워크 포트 번호로 CAS와 접속하게 된다. APPL_SERVER_PORT의 값이 설정되지 않으면 첫번째 CAS가 사용하는 네트워크 포트는 BROKER_PORT + 1이 된다.

    예를 들어 Windows에서 BROKER_PORT가 33000이고 APPL_SERVER_PORT 가 설정되지 않았으면 application과 CAS 사이에 사용되는 포트는 다음과 같다.
    
    *   application이 CAS(1)과 접속하는 포트 : 33001
    *   application이 CAS(2)와 접속하는 포트 : 33002
    *   application이 CAS(3)와 접속하는 포트 : 33003
                
#.  CAS는 cubrid.conf에 설정된 cubrid_port_id 포트를 통해 cub_master에게 cub_server로의 연결을 요청한다.
#.  CAS와 cub_server가 연결된다. 
    
    Linux에서는 CAS가 유닉스 도메인 소켓을 통해 cub_server와 연결되므로 cubrid_port_id 포트를 사용한다. Windows에서는 유닉스 도메인 소켓을 사용할 수 없으므로 임의의 가용 포트를 통해 cub_server와 연결된다. Windows에서 DB server를 운용한다면 브로커 장비와 DB 서버 장비 사이에서는 임의의 가용 포트를 사용하므로, 두 장비 사이에서 방화벽이 해당 프로세스에 대한 포트를 막게 되면 정상 동작을 보장할 수 없게 된다는 점에 주의한다.
    
#.  이후 CAS는 application이 종료되어도 CAS가 재시작되지 않는 한 cub_server와 연결을 유지한다.

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

서버 프로세스(cub_server)와 이에 접속하는 클라이언트 프로세스들(CAS, CSQL, copylogdb, applylogdb 등) 사이에서 상대 노드가 정상 동작하는지 ECHO(7) 포트를 통해 서로 확인하므로, 방화벽 존재 시 ECHO(7) 포트를 개방해야 한다. ECHO 포트를 서버와 클라이언트 양쪽 다 개방할 수 없는 상황이라면 cubrid.conf의 ref:`check_peer_alive <check_peer_alive>` 파라미터 값을 none으로 설정한다.

다음은 각 프로세스 간 연결 관계를 나타낸 것이다.

::

    application - cub_broker
                -> CAS  -  cub_master(master) <-> cub_master(slave)
                        -> cub_server(master)     cub_server(slave) <- applylogdb(slave)
                                              <----------------------- copylogdb(slave)
                                              
*   cub_master(master): CUBRID HA 구성에서 master 노드에 있는 마스터 프로세스. 상대 노드가 살아있는지 확인하는 역할을 수행.
*   cub_master(slave): CUBRID HA 구성에서 slave 노드에 있는 마스터 프로세스. 상대 노드가 살아있는지 확인하는 역할을 수행.
*   copylogdb(slave): CUBRID HA 구성에서 slave 노드에 있는 복제 로그 복사 프로세스
*   applylogdb(slave): CUBRID HA 구성에서 slave 노드에 있는 복제 로그 반영 프로세스

master 노드에서 slave 노드로의 복제 과정 파악이 용이하게 하기 위해 위에서 master 노드의 applylogdb, copylogdb와 slave 노드의 CAS는 생략했다.

프로세스 간 관계 기호 및 의미는 다음과 같다.

*   \- 기호: 최초 한 번만 연결됨을 나타낸다.
*   ->, <- 기호: 연결이 유지됨을 나타내며, -> 의 오른쪽 또는 <-의 왼쪽이 화살을 받는 쪽이다. 화살을 받는 쪽이 처음에 상대 프로세스의 접속을 기다리는(listening) 쪽을 나타낸다.
*   (master): HA 구성에서 master 노드를 나타낸다.
*   (slave): HA 구성에서 slave 노드를 나타낸다.
    
응용 프로그램과 DB 사이의 연결 과정은 :ref:`cubrid-basic-ports`\ 와 동일하다. 여기에서는 CUBRID HA에 의해 1:1로 master DB와 slave DB를 구성할 때 master 노드와 slave 노드 사이의 연결 과정에 대해서만 설명한다.

#.  cub_master(master)와 cub_master(slave) 사이에는 cubrid_ha.conf에 설정된 ha_port_id를 사용한다.
#.  copylogdb(slave)는 slave 노드에 있는 cubrid.conf의 cubrid_port_id에 설정된 포트를 통해 cub_master(master)에게 master DB로의 연결을 요청하여, 최종적으로 cub_server(master)와 연결하게 된다.
#.  applylogdb(slave)는 slave 노드에 있는 cubrid.conf의 cubrid_port_id에 설정된 포트를 통해 cub_master(slave)에게 slave DB로의 연결을 요청하여, 최종적으로 cub_server(slave)와 연결하게 된다.

master 노드에서도 applylogdb와 copylogdb가 동작하는데, master 노드가 절체(failover)로 인해 slave 노드로 변경될 때를 대비하기 위함이다.

.. _cwm-cm-ports:

CUBRID 매니저 서버 사용 포트
----------------------------

다음 표는 접속 요청을 기다리는(listening) 프로세스들을 기준으로 CUBRID Manager 서버가 사용하는 포트이며, 이것은 OS의 종류와 상관없이 동일하다.

+--------------------------+--------------+----------------+--------------------------+
| listener                 | requester    | port           | 방화벽 존재 시 포트 설정 |
+==========================+==============+================+==========================+
| Manager server           | application  | 8001           | 개방(open)               |
+--------------------------+--------------+----------------+--------------------------+

*   CUBRID 매니저 클라이언트가 CUBRID 매니저 서버 프로세스에 접속할 때 사용하는 포트는 cm.conf의 **cm_port**\이며 기본값은 8001이다.

CUBRID 자바 저장 프로시저 서버 사용 포트
---------------------------------------------

다음 표는 CUBRID 자바 저장 프로시저 서버가 사용하는 포트이며, 이것은 OS 종류에 관계없이 동일하다.

+---------------+--------------+----------------------------+--------------------------+
| Listener      | Requester    | Port                       | 방화벽 존재 시 포트 설정 |
+===============+==============+============================+==========================+
| cub_javasp    | cub_server   | java_stored_procedure_port | 개방(open)               |
+---------------+--------------+----------------------------+--------------------------+

*   CUBRID 자바 저장 프로시저 서버 (cub_javasp)가 cub_server 와 통신할 때 사용하는 포트는 **cubrid.conf**\의 **java_stored_procedure_port**\이며 기본값은 0으로, 사용 가능한 임의의 가용 포트가 할당됨을 의미한다.
