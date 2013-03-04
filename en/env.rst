Configuring Environment Variables
=================================

The following environment variables need to be set in order to use the CUBRID. The necessary environment variables are automatically set when the CUBRID system is installed or can be changed, as needed, by the user.

CUBRID Environment Variables
----------------------------

*   **CUBRID**: The default environment variable that designates the location where the CUBRID is installed. This variable must be set accurately since all programs included in the CUBRID system uses this environment variable as reference.

*   **CUBRID_DATABASES**: The environment variable that designates the location of the **databases.txt** file. The CUBRID system stores the absolute path of database volumes in the **$CUBRID_DATABASES/databases.txt** file. See :ref:`databases-txt-file`.

*   **CUBRID_CHARSET**: The environment variable that specifies database locale (language+character set) in CUBRID. The initial value upon CUBRID installation is **en_US**. If character set is omitted after language name, ISO-8859-1(.iso88591) will be specified by default. For more information, see :ref:`Language Setting <language-setting>`.

*   **CUBRID_MSG_LANG**: The environment variable that specifies usage messages and error messages in CUBRID. The initial value upon start is not defined. If it's not defined, it follows the value of **CUBRID_CHARESET**. If character set is omitted after **en_US**, ISO-8859-1(.iso88591) will be specified by default. For more information, see :ref:`Language Setting <language-setting>`.

*   **CUBRID_TMP**: The environment variable that specifies the location where the cub_master process and the cub_broker process store the UNIX domain socket file in CUBRID for Linux. If it is not specified, the cub_master process stores the UNIX domain socket file under the **/tmp** directory and the cub_broker process stores the UNIX domain socket file under the **$CUBRID/var/CUBRID_SOCK** directory (not used in CUBRID for Windows).

**CUBRID_TMP** value has some constraints, which are as follows:

* Since the maximum length of the UNIX socket path is 108, when a path longer than 108 is entered in **$CUBRID_TMP**, an error is displayed. ::

    $ export CUBRID_TMP=/home1/siwankim/cubrid=/tmp/123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

    $ cubrid server start apricot

    The $CUBRID_TMP is too long. (/home1/siwankim/cubrid=/tmp/123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789)

* When the relative path is entered, an error is displayed. ::

    $ export CUBRID_TMP=./var $ cubrid server start apricot

    The $CUBRID_TMP should be an absolute path. (./var)

**CUBRID_TMP** can be used to avoid the following problems that can occur at the default path of the UNIX domain socket that CUBRID uses.

* **/tmp** is used to store the temporary files in Linux. If the system administrator periodically and voluntarily cleans the space, the UNIX domain socket may be removed. In this case, configure **$CUBRID_TMP** to another path, not **/tmp**.
* The maximum length of the UNIX socket path is 108. When the installation path of CUBRID is too long and the **$CUBRID/var/CUBRID_SOCK** path that store the UNIX socket path for cub_broker exceeds 108 characters, the broker cannot be executed. Therefore, the path of **$CUBRID_TMP** must not exceed 1008 characters.

The above mentioned environment variables are set when the CUBRID is installed. However, the following commands can be used to verify the setting.

For Linux : ::

    % printenv CUBRID
    % printenv CUBRID_DATABASES
    % printenv CUBRID_CHARSET
    % printenv CUBRID TMP

In Windows : ::

    C:\> set CUBRID

OS Environment and Java Environment Variables
---------------------------------------------

*   PATH: In the Linux environment, the directory **$CUBRID/bin**, which includes a CUBRID system executable file, must be included in the PATH environment variable.

*   LD_LIBRARY_PATH: In the Linux environment, **$CUBRID/lib**, which is the CUBRID system’s dynamic library file (libjvm.so), must be included in the **LD_LIBRARY_PATH** (or **SHLIB_PATH** or **LIBPATH**) environment variable.

*   Path: In the Windows environment, the **%CUBRID%\\bin**, which is a directory that contains CUBRID system’s execution file, must be included in the **Path** environment variable.

*   JAVA_HOME: To use the Java stored procedure in the CUBRID system, the Java Virtual Machine (JVM) version 1.6 or later must be installed, and the **JAVA_HOME** environment variable must designate the concerned directory.
    See the :ref:`jsp_environment-configuration`.

**Configuring the Environment Variable**

**For Windows**

If the CUBRID system has been installed on Windows, then the installation program automatically sets the necessary environment variable. Select [Systems Properties] in [My Computer] and select the [Advanced] tab. Click the [Environment Variable] button and check the setting in the [System Variable]. The settings can be changed by clicking on the [Edit] button. See the Windows help for more information on how to change the environment variable on Windows.

.. image:: /images/image4.png

**For Linux**

If the CUBRID system has been installed on Linux, the installation program automatically creates the **.cubrid.sh** or **.cubrid.csh** file and makes configurations so that the files are automatically called from the installation account’s
shell log-in script. The following is the contents of . **cubrid.sh** environment variable configuration that was created in an environment that uses sh, bash, etc. ::

    CUBRID=/home1/cub_user/CUBRID
    CUBRID_DATABASES=/home1/cub_user/CUBRID/databases
    CUBRID_CHARSET=en_US
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
    export CUBRID_CHARSET
    export LD_LIBRARY_PATH
    export SHLIB_PATH
    export LIBPATH
    export PATH

.. _language-setting:

Language Setting
----------------

The language that will be used in the CUBRID DBMS can be designated with the **CUBRID_CHARSET** environment variable. The following are examples of values that can currently be set in the **CUBRID_CHARSET** environment variable.

*   **en_US**: English (Default)
*   **ko_KR.euckr**: Korean EUC-KR encoding
*   **ko_KR.utf8**: Korean UTF-8 encoding
*   **de_DE.utf8**: German UTF-8 encoding
*   **es_ES.utf8**: Spanish UTF-8 encoding
*   **fr_FR.utf8**: French UTF-8 encoding
*   **it_IT.utf8**: Italian UTF-8 encoding
*   **ja_JP.utf8**: Japanese UTF-8 encoding
*   **km_KH.utf8**: Cambodian UTF-8 encoding
*   **tr_TR.utf8**: Turkish UTF-8 encoding
*   **vi_VN.utf8**: Vietnamese UTF-8 encoding
*   **zh_CN.utf8**: Chinese UTF-8 encoding

Language and charset setting of CUBRID affects read and write data. The language is used for messages displayed by the program. The default value of **CUBRID_CHARSET** is **en_US** while installing the product.

For more details related to charset, locale and collation settings, see :doc:`admin/i18n`.

[번역]

.. _connect-to-cubrid-server:

포트 설정
=========

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

Starting the CUBRID Service
===========================

Configure environment variables and language, and then start the CUBRID service. For more information on configuring environment variables and language, see :ref:`control-cubrid-services`.

Shell Command
-------------

The following shell command can be used to start the CUBRID service and the *demodb* included in the installation package. ::

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

CUBRIDService or CUBRID Service Tray
------------------------------------

On the Windows environment, you can start or stop a service as follows:

*   Go to [Control Panel] > [Performance and Maintenance] > [Administrator Tools] > [Services] and select the CUBRIDService to start or stop the service.

    .. image:: /images/image5.png

*   In the system tray, right-click the CUBRID Service Tray. To start CUBRID, select [Service Start]; to stop it, select [Service Stop]. Selecting [Service Start] or [Service Stop] menu would be like executing cubrid service start or cubrid service stop in a command prompt; this command runs or stops the processes configured in service parameters of cubrid.conf.

*   If you click [Exit] while CUBRID is running, all the services and process in the server stop.

.. note::

    An administrator level (SYSTEM) authorization is required to start/stop CUBRID processes through the CUBRID Service tray; a login level user authorization is required to start/stop them with shell commands. If you cannot control the CUBRID processes on the Windows Vista or later version environment, select [Execute as an administrator (A)] in the [Start] > [All Programs] > [Accessories] > [Command Prompt]) or execute it by using the CUBRID Service Tray. When all processes of CUBRID Server stops, an icon on the CUBRID Service tray turns out gray.

Creating Databases
------------------

You can create databases by using the **cubrid createdb** utility and execute it where database volumes and log volumes are located. If you do not specify additional options such as **--db-volume-size** or **--log-volume-size**, 1.5 GB volume files are created by default (generic volume is set to 512 MB, active log is set to 512 MB, and background archive log is set to 512 MB). ::

    % cd testdb
    % cubrid createdb testdb
    % ls -l

    -rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb
    -rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb_lgar_t
    -rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb_lgat
    -rw------- 1 cubrid dbms       176 Jan 11 15:04 testdb_lginf
    -rw------- 1 cubrid dbms       183 Jan 11 15:04 testdb_vinf

In the above, *testdb* represents a generic volume file, testdb_lgar_t represents a background archive log file, testdb_lgat represents an active log file, testdb_lginf represents a log information file, and testdb_vinf represents a volume information file.

For details on volumes, see :ref:`database-volume-structure` . For details on creating volumes, see :ref:`creating-database`. It is recommended to classify and add volumes based on its purpose by using the **cubrid addvoldb** utility. For details, see :ref:`adding-database-volume`.

Starting Database
-----------------

You can start a database process by using the **cubrid server** utility. ::

    % cubrid server start testdb

To have *testdb* started upon startup of the CUBRID service (cubrid service start), configure *testdb* in the **server**  parameter of the **cubrid.conf**  file. ::

    % vi cubrid.conf

    [service]

    service=server,broker,manager
    server=testdb

    ...
