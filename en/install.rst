.. _install-execute:

Installing and Running CUBRID
=============================

Supported Platforms and System Requirements
-------------------------------------------

The platforms supported by CUBRID and hardware/software requirements for the installation are as follows:

+---------------------------------------------------------------------+------------------+---------------------+--------------------------------------+
| Supported Platforms                                                 | Required Memory  | Required Disk Space | Required Software                    |
+=====================================================================+==================+=====================+======================================+
| * Windows 32/64 Bit XP, 2003, Vista, Windows 7                      | 1GB or more      | 2GB or more(\*)     | JRE/JDK 1.6 or higher                |
|                                                                     |                  |                     | (Required when Java Stored Procedure |
| * Linux family 32/64 Bit(Linux kernel 2.4, glibc 2.3.4 or higher)   |                  |                     | is required)                         |
+---------------------------------------------------------------------+------------------+---------------------+--------------------------------------+

(\*): Requires a 500 MB of free disk space on the initial installation; requires approximately 1.5 GB of free disk space with a database creating with default options.

Beginning with 2008 R4.0, CUBRID Manager Client is not automatically installed when installing the CUBRID package. For this reason, if you require CUBRID Manager you must install it separately. The CUBRID can be downloaded from http://ftp.cubrid.org.

Including CUBRID Query Browser, a variety of drivers such as PHP, ODBC and OLE DB can also be downloaded from http://ftp.cubrid.org.

For more information on the CUBRID engine, tools, and drivers, see http://www.cubrid.org.

.. note:: 
  
    If you install CUBRID SH package by using ksh in AIX OS, it fails with the following error. 
  
    :: 
  
        0403-065 An incomplete or invalid multibyte character encountered. 
  
    Therefore, it is recommended to use ksh93 or bash instead of ksh.
    
    ::
  
        $ ksh93 ./CUBRID-10.0.0.0146-AIX-ppc64.sh 
        $ bash ./CUBRID-10.0.0.0146-AIX-ppc64.sh 

Compatibility
-------------

**Application Compatibility**

*   Applications that use JDBC, PHP or CCI APIs from 2008 R4.1 or higher version of CUBRID can access the CUBRID 10.0 database. However, you must link the CUBRID 10.0 library or use the driver to use the added/improved features of JDBC, PHP or CCI interfaces.

*   Note that query results may differ from those given in the earlier version because new reserved words have been added, and the specifications for some queries have been changed.

*   An application that is developed by using the GLO class can be used after it is converted to an application or schema suitable to the BLOB or CLOB type.

**CUBRID Manager Compatibility**

*   CUBRID Manager guarantees backward compatibility with the servers using CUBRID 2008 R2.2 or higher and uses the CUBRID JDBC driver that matches each server version. However, you must use a CUBRID Manager that is higher than CUBRID servers in version in order to utilize all the features of CUBRID Manager. The CUBRID JDBC driver is included in the $CUBRID/jdbc directory when CUBRID is installed($CUBRID on Linux, %CUBRID% on Windows).

*   The bit version of CUBRID Manager must be identical to the bit version of JRE. For example, if a 64-bit DB server uses CUBRID Manager 32-bit version, JRE or JDK 32-bit version should be installed.

*   Drivers for 2008 R2.2 and higher versions are included in CUBRID Manager by default, which you can download separately from the http://www.cubrid.org Website.

.. note:: Old version user should upgrade all of driver, broker, DB server; Data migration should be done because its DB volume is not compatible with 10.0 version.
    For upgrade and data migration, see :doc:`/upgrade`.

**Interoperability between CUBRID DB server and broker**

*   If the CUBRID DB server and its broker server are operated separately, CUBRID version between them should be the same, but if just the patch version is different, their interoperability is guaranteed.

    For example, 2008 R4.1 Patch1 broker is compatible with 2008 R4.1 Patch 10 DB server, but not compatible with 2008 R4.3 DB server. 9.1 Patch 1 broker is compatible with 9.1 Patch 10 DB server, but not compatible with 9.2 DB server.
    
*   Even if the operating systems are different, their interoperability is guaranteed if the bit version of a DB server is identical to the bit version of a broker server. 

    For example, the 64-bit DB server for Linux is interoperable with the 64-bit broker server for Windows, but it is not interoperable with a 32-bit broker server.

    For the relation between DB server and broker, see :doc:`intro`.For CUBRID SHARD, see :doc:`shard`.

.. _Installing-and-Running-on-Linux:

Installation and Running CUBRID on Linux
----------------------------------------

**Details to Check when Installing**

Check the following before installing CUBRID for Linux.

*   Operating system 
    
    Only supports glibc 2.3.4 or later.
    The glibc version can be checked as follows: ::
    
        %rpm -q glibc
    
*   64-bit or 32-bit
    
    CUBRID supports both 32-bit and 64-bit Linux. You can check the version as follows: ::
    
        % uname -a
        Linux host_name 2.6.18-53.1.14.el5xen #1 SMP Wed Mar 5 12:08:17 EST 2008 x86_64 x86_64 x86_64 GNU/Linux
    
    Make sure to install the CUBRID 32-bit version on 32-bit Linux and the CUBRID 64-bit version on 64-bit Linux. The following are the libraries that should be added.
    
    *   Curses Library (rpm -q ncurses)
    *   gcrypt Library (rpm -q libgcrypt)
    *   stdc++ Library (rpm -q libstdc++)
    
*   Check if the mapping between host names and IP addresses are correct in the /etc/hosts file.

    If host names and IP addresses are matched incorrectly, DB server cannot be started normally. Therefore, check if they are correctly mapped.
    
**Installing CUBRID**

The installation program consists of shell scripts that contain binary; thus it can be installed automatically. The following example shows how to install CUBRID with the "CUBRID-10.0.0.0201-linux.x86_64.sh" file on the Linux. ::

    $ sh CUBRID-10.0.0.0201-linux.x86_64.sh
    Do you agree to the above license terms? (yes or no) : yes
    Do you want to install this software(CUBRID) to the default(/home1/cub_user/CUBRID) directory? (yes or no) [Default: yes] : yes
    Install CUBRID to '/home1/cub_user/CUBRID' ...
    In case a different version of the CUBRID product is being used in other machines, 
    please note that the CUBRID 10.0 servers are only compatible with the CUBRID 10.0 clients and vice versa.
    Do you want to continue? (yes or no) [Default: yes] : yes
    Copying old .cubrid.sh to .cubrid.sh.bak ...

    CUBRID has been successfully installed.

    demodb has been successfully created.

    If you want to use CUBRID, run the following commands
    $ . /home1/cub_user/.cubrid.sh
    $ cubrid service start

As shown in the example above, after installing the downloaded file (CUBRID-10.0.0.0201-linux.x86_64.sh), the CUBRID related environment variables must be set in order to use the CUBRID database. Such setting has been made automatically when logging in the concerned terminal. Therefore there is no need to re-set after the first installation. ::

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

You can install CUBRID by using rpm file that is created on CentOS 5. The way of installing and uninstalling CUBRID is the same as that of using general rpm utility. While CUBRID is being installed, a new system group (cubrid) and a user account (cubrid) are created. After installation is complete, you should log in with a cubrid user account to start a CUBRID service.::

    $ rpm -Uvh cubrid-10.0.0.0201-el5.x86_64.rpm

When rpm is executed, CUBRID is installed in the "cubrid" home directory (/opt/cubrid) and related configuration file (cubrid.[c]sh) is installed in the /etc/profile.d directory. Note that *demodb* is not automatically installed. Therefore, you must executed /opt/cubrid/demo/make_cubrid_demo.sh with "cubrid" Linux ID. When installation is complete, enter the code below to start CUBRID with "cubrid" Linux ID. ::

    $ cubrid service start

.. note:: \

    *   **RPM and dependency**
    
        You must check RPM dependency when installing with RPM. If you ignore (--nodeps) dependency, it may not be executed. 
        
    *   **cubrid account and DB exists even if you remove RPM package**
        
        Even if you remove RPM, user accounts and databases that are created after installing, you must remove it manually, if needed.
        
    *   **Running CUBRID automatically in Linux when the system is started**
    
        When you use SH package to install CUBRID, the cubrid script will be included in the $CUBRID/share/init.d directory. In this file, you can find the environment variable, **CUBRID_USER**. You should change this variable to the Linux account with which CUBRID has been installed and register it in /etc/init.d, then you can use service or chkconfig command to run CUBRID automatically when the Linux system is started.

        When you use RPM package to install CUBRID, the cubrid script will be included in /etc/init.d. But you still need to change the environment variable in "cubrid" script file, $CUBRID_USER into "cubrid" account.

    *   **In /etc/hosts file, check if a host name and an IP address mapping is normal**

        If a host name and an IP address is abnormally mapped, you cannot start DB server. Therefore, you should check if they are normally mapped.
        
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

You can modify the environment such as service ports etc. edit the parameters of a configuration file located in the **$CUBRID/conf** directory. See :ref:`Installing-and-Running-on-Windows` for more information.

[번역]
**Linux에서 시스템 구동 시 CUBRID 자동 구동하기**

SH 패키지로 CUBRID를 설치했다면 $CUBRID/share/init.d 디렉터리에 cubrid라는 스크립트가 포함되어 있다. 이 파일 안의 **CUBRID_USER** 환경 변수 값을 CUBRID를 설치한 Linux 계정으로 변경한 후, /etc/init.d에 등록하면 service나 chkconfig 명령을 사용하여 Linux 시스템 구동 시 CUBRID를 자동으로 구동할 수 있다.

RPM 패키지로 CUBRID를 설치했다면 /etc/init.d 디렉터리에 cubrid 스크립트가 추가된다. 그러나 cubrid 스크립트 파일 안의 $CUBRID_USER 환경 변수를 cubrid 계정으로 변경하는 작업이 필요하다.
        

**Installing CUBRID Interfaces**

You can see the latest information on interface modules such as CCI, JDBC, PHP, ODBC, OLE DB, ADO.NET, Ruby, Python and Node.js and install them by downloading files from `http://www.cubrid.org/wiki_apis <http://www.cubrid.org/wiki_apis>`_ .

A simple description on each driver can be found on :doc:`/api/index`.

**Installing CUBRID Tools**

You can see the latest information on tools such as CUBRID Manager and CUBRID Query Browser and install them by downloading files from `http://www.cubrid.org/wiki_tools <http://www.cubrid.org/wiki_tools>`_ .

CUBRID Web Manager is started when the CUBRID is installed, and you can see this by accessing to https://localhost:8282/ .

.. _Installing-and-Running-on-Windows:

Installing and Running CUBRID on Windows
----------------------------------------

**Details to Check when Install**

You should check the below before installing CUBRID for Windows.

*   64-bit or not
    
    CUBRID supports both 32-bit and 64-bit Windows. You can check the version by selecting [My Computer] > [System Properties]. Make sure to install the CUBRID 32-bit version on 32-bit Windows and the CUBRID 64-bit version on 64-bit Windows.

*   Run with administrative privilege

    If you want to install CUBRID on Windows Vista or higher, execute the installation file with administrative privileges. On the popup menu after clicking right mouse button on the CUBRID installation file, choose [Execute as an administrator (A)].

**Installation Process**
    
**Step 1: Specifying the directory to install**

**Step 2: Selecting Setup Type**

*   **Server and Driver Installation** : CUBRID Server, CSQL (a command line tool), interface drivers (JDBC, C API) are all installed.

**Step 3: Creating a sample database**
    
    To create a sample database, it requires 300MB disk space. 

**Step 4: Completing the installation**

    CUBRID Service Tray appears on the right bottom.

.. note:: 

    CUBRID Service is automatically started when the system is rebooted. If you want to stop the  when the system is rebooted, change the "Start parameters" of "CUBRIDService" as "Stop"; "Control Panel > Administrative Tools > Services" and double-clicking "CUBRIDService", then pop-up window will be shown.


[번역] **설치 후 확인 사항**

*   CUBRID Service Tray 구동 여부

    시스템을 시작할 때 CUBRID Service Tray가 자동으로 구동되지 않는다면 다음 사항을 확인하도록 한다.

    *   [시작 버튼] > [제어판] > [관리 도구] > [서비스]의 Task Scheduler가 시작되어 있는지 확인하고, 그렇지 않으면 Task Scheduler를 시작한다.
    *   [시작 버튼] > [모든 프로그램] > [시작프로그램]에 CUBRID Service Tray가 등록되어 있는지 확인하고, 그렇지 않으면 CUBRID Service Tray를 등록한다.
    
**Upgrading CUBRID**

To install a new version of CUBRID in an environment in which a previous version has already been installed, select [CUBRID Service Tray] > [Exit] from the menu to stop currently running services, and then remove the previous version of CUBRID. Note that when you are prompted with "Do you want to delete all the existing version of databases and the configuration files?" you must select "No" to protect the existing databases.

For more information on upgrading a database from a previous version to a new version, see :doc:`upgrade`.

**Configuring Environment**

You can change configuration such as service ports to meet the user environment by changing the parameter values of following files which are located in the **%CUBRID%\\conf** directory. If a firewall has been configured, the ports used in CUBRID need to be opened.

*   **cm.conf**
    
    A configuration file for CUBRID Manager. The port that the Manager server process uses is called  **cm_port** and its default value is **8001** . Two ports are used and the port number is determined by the **cm_port** parameter. If 8001 is specified, 8001 and 8002 (configured number plus 1) ports will be used. For details, see `CUBRID Manager Manual <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual>`_ .
    
*   **cm_ext.conf**
    
    A configuration file for CUBRID Web Manager. **listen** is the port to be used in the web manager server process, and its default value is **8282**. For more details, see `CUBRID Web Manager Manual <http://www.cubrid.org/wiki_tools/entry/cubrid-web-manager-manual>`_ .
    
*   **cubrid.conf**
    
    A configuration file for server. You can use it to configure the following values: database memory, the number threads based on the number of concurrent users, communication port between broker and server, etc.  The port that a master process uses is called cubrid_port_id and its default value is 1523. For details, see :ref:`cubrid-conf-default-parameters`.
    
*   **cubrid_broker.conf**
    
    A configuration file for broker. You can use it to configure the following values: broker port, the number of application servers (CAS), SQL LOG, etc. The port that a broker uses is called **BROKER_PORT**. A port you see in the drivers such as JDBC is its corresponding broker's port. **APPL_SERVER_PORT** is a port that a broker application server (CAS) uses and it is added only in Windows. The default value is  **BROKER_PORT** +1. The number of ports used is the same as the number of CAS, starting from the specified port's number plus 1. For details, see :ref:`parameter-by-broker`.
    For example, if the value of **APPL_SERVER_PORT** is 35000 and the maximum number of CASes by **MAX_NUM_APPL_SERVER** is 50, then listening ports on CASes are 35000, 35001, ..., 35049.
    For more details, see :ref:`parameter-by-broker`. 

    The **CCI_DEFAULT_AUTOCOMMIT** broker parameter is supported since 2008 R4.0. The default value in the version is **OFF** and it is later changed to **ON** .  Therefore, users who have upgraded from 2008 R4.0 to 2008 R4.1 or later versions should change this value to **OFF** or configure the auto-commit mode to **OFF** .

**Installing CUBRID Interfaces**

You can see the latest information on interface modules such as JDBC, PHP, ODBC, and OLE DB and install them by downloading files from `http://www.cubrid.org/wiki_apis`.

A simple description on each driver can be found on :doc:`/api/index`.

**Installing CUBRID Tools**

You can see the latest information on tools such as CUBRID Manager and CUBRID Query Browser and install them by downloading files from `http://www.cubrid.org/wiki_tools`.

CUBRID Web Manager is started when the CUBRID is installed, and you can see this by accessing to https://localhost:8282/.

[번역]

압축 파일로 설치하기
--------------------

Linux에서 tar.gz 파일로 CUBRID 설치
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**설치 시 확인 사항**

Linux 버전의 CUBRID 데이터베이스를 설치하기 전에 다음 사항을 점검한다.

*   glibc 버전

    glibc 2.3.4 버전 이상만 지원한다.
    glibc 버전은 다음과 같은 방법으로 확인한다. ::
      
        % rpm -q glibc

*   64비트 여부 
    
    CUBRID 2008 R2.0 버전부터 32비트 버전과 64비트 버전을 각각 지원한다.
    Linux 버전은 다음과 같은 방법으로 확인한다. ::
        
        % uname -a
        Linux host_name 2.6.18-53.1.14.el5xen #1 SMP Wed Mar 5 12:08:17 EST 2008 x86_64 x86_64 x86_64 GNU/Linux
    
    32비트 Linux에서는 CUBRID 32비트 버전을, 64비트 Linux에서는 CUBRID 64비트 버전을 설치한다. 
    설치할 추가 라이브러리는 다음과 같다.
    
    * Curses Library (rpm -q ncurses)
    * gcrypt Library (rpm -q libgcrypt)
    * stdc++ Library (rpm -q libstdc++)
    
*   /etc/hosts 파일에 호스트 이름과 IP 주소 매핑이 정상인지 확인하기

    호스트 이름과 이에 맞는 IP 주소가 비정상적으로 매핑되어 있으면 DB 서버를 구동할 수 없으므로, 정상적으로 매핑되어 있는지 확인한다.

**설치 과정**

    **설치 디렉터리 지정**

    *   압축 파일을 설치하려는 경로에 풀어 놓는다.

        ::
        
            tar xvfz CUBRID-10.0.0.0181-linux.x86_64.tar.gz /home1/cub_user/

        /home1/cub_user/ 이하에 CUBRID 디렉터리가 생기고 그 이하에 파일이 생성된다.

    **환경 변수 설정**

    #.  사용자의 홈 디렉터리(/home1/cub_user) 이하에서 자동으로 실행되는 셸 스크립트에 아래의 환경 변수를 추가한다.
    
        다음은 bash 셸로 수행하는 경우 .bash_profile에 다음을 추가하는 예이다.

       ::
        
            export CUBRID=/home1/cub_user/CUBRID
            export CUBRID_DATABASES=$CUBRID/databases
            
    #.  CLASSPATH 환경 변수에  CUBRID JDBC 라이브러리 파일 이름을 추가한다.
    
        ::
        
            export CLASSPATH=$CUBRID/jdbc/cubrid_jdbc.jar:$CLASSPATH
            
    #.  Path 시스템 변수에 CUBRID bin 디렉터리를 추가한다.
      
        ::
        
            export PATH=$CUBRID/bin:$PATH
                
    **DB 생성**
        
    *   콘솔 창에서 DB를 생성할 디렉터리로 이동해서 DB를 직접 생성한다.

        ::
        
            cd $CUBRID_DATABASES
            mkdir testdb
            cd testdb
            cubrid createdb --db-volume-size=100M --log-volume-size=100M testdb en_US

    **부팅 시 자동 시작**

    *   $CUBRID/share/init.d 디렉터리에 cubrid라는 스크립트가 포함되어 있다. 이 파일 안의 **CUBRID_USER** 환경 변수 값을 CUBRID를 설치한 Linux 계정으로 변경한 후, /etc/init.d에 등록하면 service나 chkconfig 명령을 사용하여 Linux 시스템 구동 시 CUBRID를 자동으로 구동할 수 있다.
            
    **DB 자동 구동**    

    *   부팅 시 생성한 DB가 구동되게 하려면 C:\\CUBRID\\conf\\cubrid.conf에서 다음을 수정한다.

        ::
            
            [service]
            service=server, broker, manager
            server=testdb

    *   service 파라미터에는 자동으로 구동할 프로세스들을 지정한다.
    *   server 파라미터에는 자동으로 구동할 DB 이름을 지정한다.
        
CUBRID 설치 이후 환경 설정, 도구 설치, 인터페이스 설치 등은 :ref:`Installing-and-Running-on-Linux`\을 확인하도록 한다.
            
Windows에서 zip 파일로 CUBRID 설치
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**설치 시 확인 사항**

Windows 버전의 CUBRID 데이터베이스를 설치하기 전에 다음 사항을 점검한다.

*   64비트 여부

    CUBRID는 32비트 버전과 64비트 버전을 각각 지원한다. [내 컴퓨터] > [시스템 등록 정보] 창을 활성화하여 Windows 버전 비트를 확인할 수 있다. 32비트 Windows에서는 CUBRID 32비트 버전을 설치하고, 64비트 Windows에서는 CUBRID 64비트 버전을 설치한다.
    
**설치 과정**

    **설치 디렉터리 지정**

    *   압축 파일을 설치하려는 경로에 풀어 놓는다.

        ::
        
            C:\CUBRID

    **환경 변수 설정**

    #.  내 컴퓨터(오른쪽 마우스 클릭) -> 속성 -> 고급 -> 환경변수를 선택한다.
    #.  시스템 변수 항목에 새로 만들기를 클릭한 후 아래와 같이 시스템 변수를 추가한다.
    
        ::
        
            CUBRID = C:\CUBRID
            CUBRID_DATABASES = %CUBRID%\databases
            
    #.  CLASSPATH 시스템 변수에  CUBRID JDBC 라이브러리 파일 이름을 추가한다.
    
        ::
        
            %CUBRID%\jdbc\cubrid_jdbc.jar       
            
    #.  Path 시스템 변수에 CUBRID bin 디렉터리를 추가한다.
      
        ::
        
            %CUBRID%\bin
                
    **DB 생성**
        
    *   cmd 명령으로 콘솔 창을 띄운 후 DB를 생성할 디렉터리로 이동해서 DB를 직접 생성한다.

        ::
        
            cd C:\CUBRID\databases
            md testdb
            cd testdb
            c:\CUBRID\databases\testdb>cubrid createdb --db-volume-size=100M --log-volume-size=100M testdb en_US
    
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
            