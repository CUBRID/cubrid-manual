.. _install-execute:

Installing and Running on Linux
===============================

Overview
--------

Supported Platforms and System Requirements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The platforms supported by CUBRID and hardware/software requirements for the installation are as follows:

+---------------------------------------------------------------------+------------------+---------------------+--------------------------------------+
| Supported Platforms                                                 | Required Memory  | Required Disk Space | Required Software                    |
+=====================================================================+==================+=====================+======================================+
| * Windows 32/64 Bit XP, 2003, Vista, Windows 7                      | 1GB or more      | 2GB or more(\*)     | JRE/JDK 1.6 or higher                |
|                                                                     |                  |                     | (Required when Java Stored Procedure |
| * Linux family 32/64 Bit(Linux kernel 2.4 및 glibc 2.3.4 or higher) |                  |                     | is required)                         |
+---------------------------------------------------------------------+------------------+---------------------+--------------------------------------+

(\*): Requires a 500 MB of free disk space on the initial installation; requires approximately 1.5 GB of free disk space with a database creating with default options.

Beginning with 2008 R4.0, CUBRID Manager Client is not automatically installed when installing the CUBRID package. For this reason, if you require CUBRID Manager you must install it separately. The CUBRID can be downloaded from http://ftp.cubrid.org.

Including CUBRID Query Browser, a variety of drivers such as PHP, ODBC, OLE, and DB can also be downloaded from http://ftp.cubrid.org.

For more information on the CUBRID engine, tools, and drivers, see http://www.cubrid.org.


Version Compatibility and Operability
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Application Compatibility**

*   Applications that use JDBC, PHP or CCI APIs from 2008 R2.0 or higher version of CUBRID can access the CUBRID 2008 R4.1 database. However, you must link the CUBRID 2008 R4.1 library or use the driver to use the added/improved features of JDBC, PHP or CCI interfaces.

*   Note that query results may differ from those given in the earlier version because new reserved words have been added, and the specifications for some queries have been changed.

*   An application that is developed by using the GLO class can be used after it is converted to an application or schema suitable to the BLOB or CLOB type.

**CUBRID Manager Compatibility**

*   CUBRID Manager guarantees backward compatibility with the servers using CUBRID 2008 R2.1 or higher and uses the CUBRID JDBC driver that matches each server version. However, you must use a CUBRID Manager that is higher than CUBRID servers in version in order to utilize all the features of CUBRID Manager. The CUBRID JDBC driver is included in the $CUBRID/jdbc directory when CUBRID is installed.($CUBRID on Linux, %CUBRID% on Windows).

*   The bit version of CUBRID Manager must be identical to the bit version of JRE. For example, if a 64-bit DB server uses CUBRID Manager 32-bit version, JRE or JDK 32-bit version should be installed.

*   Drivers for 2008 R2.2 and higher versions are included in CUBRID Manager by default, which you can download separately from the http://www.cubrid.org Website.

.. note:: 9.0 Beta version user should upgrade all of driver, broker, DB server; Data migration should be done because it's DB volume is not compatible with 9.1 version.
    For upgrade and data migration, see :doc:`/upgrade`.

Interoperability
^^^^^^^^^^^^^^^^

*   If the CUBRID DB server and its broker server are operated separately, their interoperability is guaranteed, even when the operating systems are different. However, the bit version of a DB server must be identical to the bit version of a broker server. For example, the 64-bit DB server for Linux is interoperable with the 64-bit broker server for Windows, but it is not interoperable with a 32-bit broker server.

    For the relation between DB server and broker, see :doc:`intro`.For CUBRID SHARD, see :doc:`admin/shard`.

*   If the CUBRID DB server and its broker server are operated separately, their system locales should be the same. For example, if CUBRID_CHARSET of DB server is en_US.utf8, CUBRID_CHARSET of broker server should be en_US.utf8, too.

Installing and Running on Linux
-------------------------------

**Details to Check when Installing**

    Check the following before installing CUBRID for Linux.

    * Operating system 

      Only supports glibc 2.3.4 or later.
      The glibc version can be checked as follows: ::
      
        %rpm -q glibc

    * 64-bit

      Since version 2008 R2.0, CUBRID supports both 32-bit and 64-bit Linux. You can check the version as follows: ::
      
        % uname -a
        Linux host_name 2.6.18-53.1.14.el5xen #1 SMP Wed Mar 5 12:08:17 EST 2008 x86_64 x86_64 x86_64 GNU/Linux

      Make sure to install the CUBRID 32-bit version on 32-bit Linux and the CUBRID 64-bit version on 64-bit Linux. The followings are the libraries that should be added.

      * Curses Library (rpm -q ncurses)
      * gcrypt Library (rpm -q libgcrypt)
      * stdc++ Library (rpm -q libstdc++)
  
**Installing CUBRID**

    The installation program consists shell scripts that contain binary; thus it can be installed automatically. The following example shows how to install CUBRID with the "CUBRID-9.1.0.0201-linux.x86_64.sh" file on the Linux. ::

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

    As shown in the example above, after installing the downloaded file (CUBRID-9.1.0.0201-linux.x86_64.sh), the CUBRID related environment variables must be set in order to use the CUBRID database. Such setting has been made automatically when logging in the concerned terminal. Therefore there is no need to re-set after the first installation. ::

        $ . /home1/cub_user/.cubrid.sh

    After CUBRID is installed, you can start CUBRID Manager server and CUBRID broker as follows. ::

        $ cubrid service start

    When you want to check whether CUBRID Manager server and CUBRID broker works well, you can use **grep** command in Linux as follows. ::

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

**Installing CUBRID (rpm File)**

    You can install CUBRID by using rpm file that is created on CentOS5. The way of installing and uninstalling CUBRID is the same as that of using general rpm utility. While CUBRID is being installed, a new system group (cubrid) and a user account (cubrid) are created. After installation is complete, you should log in with a cubrid user account to start a CUBRID service.::

        $ rpm -Uvh cubrid-9.1.0.0201-el5.x86_64.rpm

    When rmp is executed, CUBRID is installed in the "cubrid" home directory (/opt/cubrid) and related configuration file (cubrid.[c]sh) is installed in the /etc/profile.d directory. Note that *demodb* is not automatically installed. Therefore, you must executed /opt/cubrid/demo/make_cubrid_demo.sh with "cubrid" Linux ID. When installation is complete, enter the code below to start CUBRID with "cubrid" Linux ID. ::

        $ cubrid service start

    .. note:: \

        **RPM and dependency**
        
        You must check RPM dependency when installing with RPM. If you ignore (--nodeps) dependency, it may not be executed. Even if you remove RPM, user accounts and databases that are created after installing, you must remove it manually, if needed.
        
        **Running CUBRID automatically in Linux when the system is started**
        
        How to use service or chkconfig command If you use SH or RPM package to install CUBRID, the cubrid script will be included in the $CUBRID/share/init.d directory. In this file, you can find the environment variable, **CUBRID_USER**. If you change this variable to the Linux account with which CUBRID has been installed and register it in /etc/init.d, then you can use service or chkconfig command to run CUBRID automatically when the Linux system is started.
    
**Installing CUBRID on Fedora/CentOS**

    To install CUBRID using the yum command, you should know where the CUBRID package is located. Choose appropriate location based on your operating system.

    *   `http://www.cubrid.org/yum_repository <http://www.cubrid.org/yum_repository>`_

    For example, if you are using Fedora 16, execute the command below. In the example, fc16 refers to Fedora 16. ::

        $ rpm -i http://yumrepository.cubrid.org/cubrid_repo_settings/9.0.0/cubridrepo-9.0.0-1.fc16.noarch.rpm

    If you are using CentOS 6.2, execute the command below. In this example, el6.2 refers to CentOS. ::

        $ rpm -i http://yumrepository.cubrid.org/cubrid_repo_settings/9.0.0/cubridrepo-9.0.0-1.el6.2.noarch.rpm

    You can install the CUBRID package you have desired based on the command you execute. To install the latest version, execute the command below. ::

        $ yum install cubrid

    To install the earlier version, you should include version information in the command. ::

        $ yum install cubrid-8.4.3

    After installation is complete, configure environment variables including installation path of CUBRID and then apply them to system.

**Installing CUBRID on Ubuntu**

    To install CUBRID using the apt-get command on Ubuntu, add the CUBRID storage first and then update the apt index. ::

        $ sudo add-apt-repository ppa:cubrid/cubrid
        $ sudo apt-get update

    To install the latest version, execute the command below. ::

        $ sudo apt-get install cubrid

    To install the earlier version, you should include version information in the command. ::

        $ sudo apt-get install cubrid-8.4.3

    After installation is complete, configure environment variables including installation path of CUBRID and then apply them to system.

**Upgrading CUBRID**

    When you specify an installation directory where the previous version of CUBRID is already installed, a message which asks to overwrite files in the directory will appear. Entering **no** will stop the installation. ::

        Directory '/home1/cub_user/CUBRID' exist!
        If a CUBRID service is running on this directory, it may be terminated abnormally.
        And if you don't have right access permission on this directory(subdirectories or files), install operation will be failed.
        Overwrite anyway? (yes or no) [Default: no] : yes

    Choose whether to overwrite the existing configuration files during the CUBRID installation. Entering **yes** will overwrite and back up them as extension .bak files. ::

        The configuration file (.conf or .pass) already exists. Do you want to overwrite it? (yes or no) : yes

    For more information on upgrading a database from a previous version to a new version, see :doc:`upgrade`.

**Configuring Environment**

    You can modify the environment such as service ports etc. edit the parameters of a configuration file located in the **$CUBRID/conf** directory. See :ref:`Installin-and-Running-on-Windows` for more information.

**Installing CUBRID Interfaces**

    You can see the latest information on interface modules such as CCI, JDBC, PHP, ODBC, OLE DB, ADO.NET, Ruby, Python and Node.js and install them by downloading files from `http://www.cubrid.org/wiki_apis <http://www.cubrid.org/wiki_apis>`_ .

    A simple description on each driver can be found on :doc:`/api/index`.

**Installing CUBRID Tools**

    You can see the latest information on tools such as CUBRID Manager and CUBRID Query Browser and install them by downloading files from `http://www.cubrid.org/wiki_tools <http://www.cubrid.org/wiki_tools>`_ .

    CUBRID Web Manager is started when the CUBRID is installed, and you can see this by accessing to https://localhost:8282/ .

.. _Installin-and-Running-on-Windows:

Installing and Running on Windows
---------------------------------

**Details to Check when Install**

    You should check belows before installing CUBRID for Windows.
    
    * 64-bit

      Since version 2008 R2.0, CUBRID supports both 32-bit and 64-bit Windows. You can check the version by selecting [My Computer] > [System Properties]. Make sure to install the CUBRID 32-bit version on 32-bit Windows and the CUBRID 64-bit version on 64-bit Windows.

    If you want to install CUBRID on Windows Vista or higher, execute the installation file with administrative privileges.

    * On the popup menu after clicking right mouse button on the CUBRID installation file, choose [Execute as an administrator (A)].

**Installation Process**
    
    **Step 1: Specifying the directory to install**
    
    **Step 2: Selecting Setup Type**

    *   **Server and Driver Installation** : CUBRID Server, CSQL (a command line tool), interface drivers (OLE DB Provider, ODBC, JDBC, C API) are all installed.

    *   **Driver Installation** : Only the interface drivers (OLE DB Provider, ODBC, JDBC, C API) are  installed. You can select this type of installation if development or operation is performed by remote connection to the computer in which the CUBRID database server is installed.

    **Step 3: Creating a sample database**
        
        To craete a sample database, it requires 300MB disk space. 
    
    **Step 4: Completing the installation**
    
        CUBRID Service Tray appears on the right bottom.

    .. note:: 
    
        CUBRID Service is automatically started when the system is rebooted. If you want to stop the  when the system is rebooted, change the "Start parameters" of "CUBRIDService" as "Stop"; "Control Panel > Adminstrative Tools > Services" and double-clicking "CUBRIDService", then pop-up window will be shown.

**Upgrading CUBRID**

    To install a new version of CUBRID in an environment in which a previous version has already been installed, select [CUBRID Service Tray] > [Exit] from the menu to stop currently running services, and then remove the previous version of CUBRID. Note that when you are prompted with "Do you want to delete all the existing version of databases and the configuration files?" you must select "No" to protect the existing databases.

    For more information on upgrading a database from a previous version to a new version, see :doc:`upgrade`.

    .. _Configuring-Environment-on-Windows:

**Configuring Environment**

    You can change configuration such as service ports to meet the user environment by changing the parameter values of following files which are located in the **%CUBRID%\\conf** directory. If a firewall has been configured, the ports used in CUBRID need to be opened.

    * **cm.conf**

      A configuration file for CUBRID Manager. The port that the Manager server process uses is called  **cm_port** and its default value is **8001** . Two ports are used and the port number is determined by the **cm_port** parameter. If 8001 is specified, 8001 and 8002 (configured number plus 1) ports will be used. For details, see `CUBRID Manager Manual <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual>`_ .

    * **cm_httpd.conf**
     
      A configuration file for CUBRID Web Manager. **listen** is the port to be used in the web manager server process, and it's default value is **8282**. For more details, see `CUBRID Web Manager Manual <http://www.cubrid.org/wiki_tools/entry/cubrid-web-manager-manual>`_ .

    * **cubrid.conf**

      A configuration file for server. You can use it to configure the following values: database memory, the number threads based on the number of concurrent users, communication port between broker and server, etc.  The port that a master process uses is called cubrid_port_id and its default value is 1523. For details, see :ref:`cubrid-conf-default-parameters`.

    * **cubrid_broker.conf**

      A configuration file for broker. You can use it to configure the following values: broker port, the number of application servers (CAS), SQL LOG, etc. The port that a broker uses is called **BROKER_PORT**. A port you see in the drivers such as JDBC is its corresponding broker's port. **APPL_SERVER_PORT** is a port that a broker application server (CAS) uses and it is added only in Windows. The default value is  **BROKER_PORT** +1. The number of ports used is the same as the number of CAS, starting from the specified port's number plus 1. For details, see :ref:`parameter-by-broker`.

      For example, if the value of **APPL_SERVER_PORT** is 35000 and the maximum number of CASs by **MAX_NUM_APPL_SERVER** is 50, then listening ports on CASs are 35000, 35001, ..., 35049.
      For more details, see :ref:`parameter-by-broker`. 
      
      The **CCI_DEFAULT_AUTOCOMMIT** broker parameter is supported since 2008 R4.0. The default value in the version is **OFF** and it is later changed to **ON** .  Therefore, users who have upgraded from 2008 R4.0 to 2008 R4.1 or later versions should change this value to **OFF** or configure the auto-commit mode to **OFF** .

**Installing CUBRID Interfaces**

    You can see the latest information on interface modules such as JDBC, PHP, ODBC, and OLE DB and install them by downloading files from `http://www.cubrid.org/wiki_apis <http://www.cubrid.org/wiki_apis>`_ .

    A simple description on each driver can be found on :doc:`/api/index`.

**Installing CUBRID Tools**

    You can see the latest information on tools such as CUBRID Manager and CUBRID Query Browser and install them by downloading files from `http://www.cubrid.org/wiki_tools <http://www.cubrid.org/wiki_tools>`_ .
    
    CUBRID Web Manager is started when the CUBRID is installed, and you can see this by accessing to `https://localhost:8282/ <https://localhost:8282/>`_.

[번역]

.. _connect-to-cubrid-server:

CUBRID 서버에 연결하기
======================

포트가 개방되어 있지 않은 환경에서 사용하는 경우, CUBRID가 사용하는 포트들을 개방해야 한다.

다음은 CUBRID가 사용하는 포트에 대해 하나의 표로 정리한 것이다. 각 포트는 상대방의 접속을 대기하는 listener 쪽에서 개방되어야 한다.

Linux 방화벽에서 특정 프로세스에 대한 포트를 개방하려면 해당 방화벽 프로그램의 설명을 따른다.

Windows에서 임의의 가용 포트를 사용하는 경우는 어떤 포트를 개방할 지 알 수 없으므로  Windows 메뉴의 "제어판" 검색창에서  "방화벽"을 입력한 후, "Windows 방화벽 > Windows 방화벽을 통해 프로그램 또는 기능 허용"에서 포트 개방을 원하는 프로그램을 추가한다. 
=>
Windows에서 임의의 가용 포트를 사용하는 경우는 어떤 포트를 개방할 지 알 수 없으므로, type firewall in the search box of "Control Panel", click "Windows Firewall > Allow a program through Windows Firewall" and select the check box next to the program you want to allow, and then click OK.

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

**1. CUBRID 기본 사용 포트**

    접속 요청을 기다리는(listening) 프로세스 들을 기준으로 각 OS 별로 필요한 포트를 정리하면 다음과 같으며, 각 포트는 listener 쪽에서 개방되어야 한다.
    
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
        
    .. note:: Windows에서는 CAS가 cub_server에 접근할 때 사용할 포트를 임의로 정하므로 개방할 포트를 정할 수 없다. 따라서 "Windows 방화벽"에서 "허용되는 프로그램"을 설정해야 한다.
        
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
    
    
**2. CUBRID HA 사용 포트**

    CUBRID HA는 Linux 환경에서만 지원한다.

    접속 요청을 기다리는(listening) 프로세스 들을 기준으로 각 OS 별로 필요한 포트를 정리하면 다음과 같으며, 각 포트는 listener 쪽에서 개방되어야 한다.

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
        
    (*): CAS, CSQL, copplogdb, 또는 applylogdb 프로세스가 존재하는 장비
    
    (**): cub_server가 존재하는 장비
    
    서버 프로세스(cub_server)와 이에 접속하는 클라이언트 프로세스들(CAS, CSQL, copylogdb, applylogdb 등) 사이에서 상대 노드가 정상 동작하는지 ECHO(7) 포트를 통해 서로 확인하므로, 방화벽 존재 시 ECHO(7) 포트를 개방해야 한다. ECHO 포트를 서버와 클라이언트 양쪽 다 개방할 수 없는 상황이라면 cubrid.conf의 **check_peer_alive** 파라미터 값을 none으로 설정한다.
    
    이외에도 ECHO(7) 포트의 개방이 필요하다. ECHO 포트 개방과 관련된 설명은 "1. CUBRID 기본 사용 포트"를 참고한다.

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
    
**3. CUBRID SHARD 사용 포트**

    접속 요청을 기다리는(listening) 프로세스 들을 기준으로 각 OS 별로 필요한 포트를 정리하면 다음과 같으며, 각 포트는 listener 쪽에서 개방되어야 한다.

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
        
    .. note:: Windows에서는 CAS가 cub_server에 접근할 때 사용할 포트를 임의로 정하므로 개방할 포트를 정할 수 없다. 따라서 "Windows 방화벽"에서 "허용되는 프로그램"을 설정해야 한다.
        
    서버 프로세스(cub_server)와 이에 접속하는 클라이언트 프로세스들(CAS, CSQL) 사이에서 상대 노드가 정상 동작하는지 ECHO(7) 포트를 통해 서로 확인하므로, 방화벽 존재 시 ECHO(7) 포트를 개방해야 한다. ECHO 포트를 서버와 클라이언트 양쪽 다 개방할 수 없는 상황이라면 cubrid.conf의 **check_peer_alive** 파라미터 값을 none으로 설정한다.

    ::
    
        application - shard broker
                    -> shard proxy <- shard CAS - cub_master
                                                -> cub_server
    
        * shard broker: CUBRID SHARD 브로커 프로세스. apllication과 shard proxy를 중계
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

**4. CUBRID Web Manager, CUBRID Manager 서버 사용 포트**
    
    접속 요청을 기다리는(listening) 프로세스 들을 기준으로 CUBRID Web Manager, CUBRID Manager 서버가 사용하는 포트는 다음과 같으며, 이들은 OS의 종류와 관계없이 동일하다.
    
    +--------------------------+--------------+----------------+--------------------------+
    | listener                 | requester    | port           | 방화벽 존재 시 포트 설정 |
    +==========================+==============+================+==========================+
    | Manager server           | application  | 8001, 8002     | 개방(open)               |
    +--------------------------+--------------+----------------+--------------------------+
    | Web Manager server       | application  | 8282           | 개방                     |
    +--------------------------+--------------+----------------+--------------------------+
    
    * CUBRID Manager 클라이언트가 CUBRID Manager 서버 프로세스에 접속할 때 사용하는 포트는 cm.conf의 **cm_port**\와 **cm_port** + 1이며 **cm_port**\의 기본값은 8001이다.
    * CUBRID Web Manager 클라이언트가 CUBRID Web Manager 서버 프로세스에 접속할 때 사용하는 포트는 cm_httpd.conf의 **listen**\이며 기본값은 8282이다.

            