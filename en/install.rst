
:meta-keywords: cubrid install, cubrid compatibility, cubrid service, cubrid manager, run cubrid
:meta-description: CUBRID supported platforms, hardware and software requirements, how to install and run CUBRID engine and CUBRID manager.

.. _install-execute:

Installing and Running CUBRID
=============================

Supported Platforms and System Requirements
-------------------------------------------

The platforms supported by CUBRID and hardware/software requirements for the installation are as follows:

+---------------------------------------------------------------------+------------------+---------------------+--------------------------------------+
| Supported Platforms                                                 | Required Memory  | Required Disk Space | Required Software                    |
+=====================================================================+==================+=====================+======================================+
| * Windows 64 Bit Windows 7                                          | 1GB or more      | 2GB or more(\*)     | JRE/JDK 1.6 or higher                |
|                                                                     |                  |                     | (Required when Java Stored Procedure |
| * Linux family 64 Bit(Linux kernel 2.4, glibc 2.3.4 or higher)      |                  |                     | is required)                         |
+---------------------------------------------------------------------+------------------+---------------------+--------------------------------------+

(\*): Requires a 500MB of free disk space on the initial installation; requires approximately 1.5GB of free disk space with a database creating with default options.

Beginning with 2008 R4.0, CUBRID Manager Client is not automatically installed when installing the CUBRID package. For this reason, if you require CUBRID Manager you must install it separately. The CUBRID can be downloaded from http://ftp.cubrid.org.

A variety of drivers such as PHP, ODBC and OLE DB can also be downloaded from http://ftp.cubrid.org.

For more information on the CUBRID engine, tools, and drivers, see http://www.cubrid.org.

Compatibility
-------------

**Application Compatibility**

*   Applications that use JDBC, PHP or CCI APIs from 2008 R4.1 or higher version of CUBRID can access the CUBRID 10.2 database. However, you must link the CUBRID 10.2 library or use the driver to use the added/improved features of JDBC, PHP or CCI interfaces. In order to use :ref:`timezone-type` which are introduced as 10.0, users should upgrade drivers.

*   Old driver versions connecting to CUBRID 10.2 server interpret a JSON type column as Varchar.

*   Note that query results may differ from those given in the earlier version because new reserved words have been added, and the specifications for some queries have been changed.

*   An application that is developed by using the GLO class can be used after it is converted to an application or schema suitable to the BLOB or CLOB type.

**CUBRID Manager Compatibility**

*   CUBRID Manager guarantees backward compatibility with the servers using CUBRID 2008 R2.2 or higher and uses the CUBRID JDBC driver that matches each server version. However, you must use a CUBRID Manager that is higher than CUBRID servers in version in order to utilize all the features of CUBRID Manager. The CUBRID JDBC driver is included in the $CUBRID/jdbc directory when CUBRID is installed($CUBRID on Linux, %CUBRID% on Windows).

*   The bit version of CUBRID Manager must be identical to the bit version of JRE.

    For example, if a 64-bit DB server uses CUBRID Manager 32-bit version, JRE or JDK 32-bit version should be installed.

*   Drivers for 2008 R2.2 and higher versions are included in CUBRID Manager by default, which you can download separately from the http://www.cubrid.org Website.

.. note:: Old version users should upgrade all of driver, broker, DB server; Data migration should be done because its DB volume is not compatible with 10.2 version.
    For upgrade and data migration, see :doc:`/upgrade`.

**Interoperability between CUBRID DB server and broker**

*   If the CUBRID DB server and its broker server are operated separately, CUBRID version between them should be the same, but if just the patch version is different, their interoperability is guaranteed.

    For example, 2008 R4.1 Patch1 broker is compatible with 2008 R4.1 Patch 10 DB server, but not compatible with 2008 R4.3 DB server. 9.1 Patch 1 broker is compatible with 9.1 Patch 10 DB server, but not compatible with 9.2 DB server.
    
*   Even if the operating systems are different, their interoperability is guaranteed if the bit version of a DB server is identical to the bit version of a broker server. 

    For example, the 64-bit DB server for Linux is interoperable with the 64-bit broker server for Windows.

    For the relation between DB server and broker, see :doc:`intro`.

.. _Installing-and-Running-on-Linux:

Installing and Running CUBRID on Linux
--------------------------------------

**Checklist before Installing**

Check the following before installing CUBRID for Linux.

*   glibc version 
    
    Only supports glibc 2.3.4 or later.
    The glibc version can be checked as follows: ::
    
        %rpm -q glibc
    
*   64-bit
    
    As 10.0, CUBRID supports only 64-bit Linux. You can check the version as follows: ::
    
        % uname -a
        Linux host_name 2.6.32-696.20.1.el6.x86_64 #1 SMP Fri Jan 26 17:51:45 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux
    
    Make sure to install the CUBRID 64-bit version on 64-bit Linux. 
    
*   The libraries that should be added.
    
    *   Curses Library (rpm -q ncurses)

        CUBRID is packaged with version 5 of Curses library. You may need to install ncurses-compat-libs package if your system has newer version and downgrade is not possible.

    *   gcrypt Library (rpm -q libgcrypt)
    *   stdc++ Library (rpm -q libstdc++)
    
*   Check if the mapping between host names and IP addresses are correct in the /etc/hosts file.

    If host names and IP addresses are matched incorrectly, DB server cannot be started normally. Therefore, check if they are correctly mapped.
    
**Installing CUBRID**

The installation program consists of shell scripts that contain binary; thus it can be installed automatically. The following example shows how to install CUBRID with the "CUBRID-10.2.0.8787-a31ea42-Linux.x86_64.sh" file on the Linux. 

::

    $ sh CUBRID-10.2.0.8787-a31ea42-Linux.x86_64.sh
    Do you agree to the above license terms? (yes or no) : yes
    Do you want to install this software(CUBRID) to the default(/home1/cub_user/CUBRID) directory? (yes or no) [Default: yes] : yes
    Install CUBRID to '/home1/cub_user/CUBRID' ...
    In case a different version of the CUBRID product is being used in other machines, 
    please note that the CUBRID 10.2 servers are only compatible with the CUBRID 10.2 clients and vice versa.
    Do you want to continue? (yes or no) [Default: yes] : yes
    Copying old .cubrid.sh to .cubrid.sh.bak ...

    CUBRID has been successfully installed.

    demodb has been successfully created.

    If you want to use CUBRID, run the following commands
    $ . /home1/cub_user/.cubrid.sh
    $ cubrid service start

As shown in the example above, after installing the downloaded file (CUBRID-10.2.0.8787-a31ea42-Linux.x86_64.sh), the CUBRID related environment variables must be set in order to use the CUBRID database. Such setting has been made automatically when logging in the concerned terminal. Therefore there is no need to re-set after the first installation. ::

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

You can install CUBRID by using rpm file that is created on CentOS 6. The way of installing and uninstalling CUBRID is the same as that of using general rpm utility. While CUBRID is being installed, a new system group (cubrid) and a user account (cubrid) are created. After installation is complete, you should log in with a cubrid user account to start a CUBRID service.::

    $ rpm -Uvh cubrid-10.2.0.8787-a31ea42-Linux.x86_64.rpm

When rpm is executed, CUBRID is installed in the "cubrid" home directory (/opt/cubrid) and related configuration file (cubrid.[c]sh) is installed in the /etc/profile.d directory. Note that *demodb* is not automatically installed. Therefore, you must executed /opt/cubrid/demo/make_cubrid_demo.sh with "cubrid" Linux ID. When installation is complete, enter the code below to start CUBRID with "cubrid" Linux ID. ::

    $ cubrid service start

.. note:: \

    *   **RPM and dependency**
    
        You must check RPM dependency when installing with RPM. If you ignore (--nodeps) dependency, it may not be executed. 

    *   **cubrid account and DB exists even if you remove RPM package**
        
        Even if you remove RPM, user accounts and databases that are created after installing, you must remove it manually, if needed.
        
    *   **Running CUBRID automatically in Linux when the system is started**
    
        When you use SH package to install CUBRID, the cubrid script will be included in the $CUBRID/share/init.d directory. In this file, you can find the environment variable, **CUBRID_USER**. You should change this variable to the Linux account with which CUBRID has been installed and register it in /etc/init.d, then you can use service or chkconfig command to run CUBRID automatically when the Linux system is started.

        When you use RPM package to install CUBRID, the cubrid script will be included in /etc/init.d. But you still need to change the environment variable, $CUBRID_USER from "cubrid" script file.

    *   **In /etc/hosts file, check if a host name and an IP address mapping is normal**

        If a host name and an IP address is abnormally mapped, you cannot start DB server. Therefore, you should check if they are normally mapped.

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

**Installing CUBRID Interfaces**

You can download interface modules such as CCI, JDBC, PHP, ODBC, OLE DB, ADO.NET, Ruby, Python and Node.js from http://www.cubrid.org/downloads.

.. FIXME You can see the latest information on interface modules such as CCI, JDBC, PHP, ODBC, OLE DB, ADO.NET, Ruby, Python and Node.js and install them by downloading files from http://www.cubrid.org/downloads.

A simple description on each driver can be found on :doc:`/api/index`.

**Installing CUBRID Tools**

You can download various tools including CUBRID Manager and CUBRID Migration Toolkit from http://www.cubrid.org/downloads.

.. FIXME You can see the latest information on tools such as CUBRID Manager and install them by downloading files from http://www.cubrid.org/downloads.

.. _Installing-and-Running-on-Windows:

Installing and Running CUBRID on Windows
----------------------------------------

**Checklist before Installing**

You should check the below before installing CUBRID for Windows.

*   64-bit
    
    CUBRID supports only 64-bit Windows. You can check the version by selecting [My Computer] > [System Properties]. Make sure to install a CUBRID 64-bit version on 64-bit Windows.

    .. warning:: 10.1 would be the last release of 32-bit Windows.

**Installation Process**
    
**Step 1: Specifying the directory to install**

**Step 2: Creating a sample database**
    
    To create a sample database, it requires about 1.5GB disk space. 

**Step 3: Completing the installation**

    CUBRID Service Tray appears on the right bottom.

.. note:: 

    CUBRID Service is automatically started when the system is rebooted. If you want to stop the  when the system is rebooted, change the "Start parameters" of "CUBRIDService" as "Stop"; "Control Panel > Administrative Tools > Services" and double-clicking "CUBRIDService", then pop-up window will be shown.

**Checklist After Installation**

*   Whether the start of CUBRID Service Tray or not

    If CUBRID Service Tray is not automatically started when starting a system, confirm the following.

    *   Check if Task Scheduler is started in [Start button] > [Control panel] > [Administrative Tools] > [Services]; if not, start Task Scheduler.
    *   Check if CUBRID Service Tray is registered in [Start button] > [All Programs] > [Startup]; if not, register CUBRID Service Tray.

**Upgrading CUBRID**

To install a new version of CUBRID in an environment in which a previous version has already been installed, select [CUBRID Service Tray] > [Exit] from the menu to stop currently running services, and then remove the previous version of CUBRID. Note that when you are prompted with "Do you want to delete all the existing version of databases and the configuration files?" you must select "No" to protect the existing databases.

For more information on upgrading a database from a previous version to a new version, see :doc:`upgrade`.

**Configuring Environment**

You can change configuration such as service ports to meet the user environment by changing the parameter values of following files which are located in the **%CUBRID%\\conf** directory. If a firewall has been configured, the ports used in CUBRID need to be opened.

*   **cm.conf**
    
    A configuration file for CUBRID Manager. The port that the Manager server process uses is called  **cm_port** and its default value is **8001**. 

    .. FIXME: For details, see `CUBRID Manager Manual <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual>`_. 

*   **cubrid.conf**
    
    A configuration file for server. You can use it to configure the following values: database memory, the number threads based on the number of concurrent users, communication port between broker and server, etc.  The port that a master process uses is called cubrid_port_id and its default value is 1523. For details, see :ref:`cubrid-conf`.
    
*   **cubrid_broker.conf**
    
    A configuration file for broker. You can use it to configure the following values: broker port, the number of application servers (CAS), SQL LOG, etc. The port that a broker uses is called **BROKER_PORT**. A port you see in the drivers such as JDBC is its corresponding broker's port. **APPL_SERVER_PORT** is a port that a broker application server (CAS) uses and it is added only in Windows. The default value is  **BROKER_PORT** +1. The number of ports used is the same as the number of CAS, starting from the specified port's number plus 1. For details, see :ref:`parameter-by-broker`.
    For example, if the value of **APPL_SERVER_PORT** is 35000 and the maximum number of CASes by **MAX_NUM_APPL_SERVER** is 50, then listening ports on CASes are 35000, 35001, ..., 35049.
    For more details, see :ref:`parameter-by-broker`. 

    The **CCI_DEFAULT_AUTOCOMMIT** broker parameter is supported since 2008 R4.0. The default value in the version is **OFF** and it is later changed to **ON**.  Therefore, users who have upgraded from 2008 R4.0 to 2008 R4.1 or later versions should change this value to **OFF** or configure the auto-commit mode to **OFF**.

**Installing CUBRID Interfaces**

You can download interface modules such as CCI, JDBC, PHP, ODBC, OLE DB, ADO.NET, Ruby, Python and Node.js from http://www.cubrid.org/downloads.

.. FIXME: You can see the latest information on interface modules such as JDBC, PHP, ODBC, and OLE DB and install them by downloading files from `<http://www.cubrid.org/wiki_apis>`_.

A simple description on each driver can be found on :doc:`/api/index`.

**Installing CUBRID Tools**

You can download various tools including CUBRID Manager and CUBRID Migration Toolkit from http://www.cubrid.org/downloads.

.. FIXME: You can see the latest information on tools such as CUBRID Manager and install them by downloading files from `<http://www.cubrid.org/wiki_tools>`_.

Installing with a Compressed Package
------------------------------------

Installing CUBRID with tar.gz on Linux
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Checklist before Installing**

Check the following before installing CUBRID for Linux.

*   glibc version 
    
    Only supports glibc 2.3.4 or later.
    The glibc version can be checked as follows: ::
    
        %rpm -q glibc
    
*   64-bit 
    
    As 10.0, CUBRID supports only 64-bit Linux. You can check the version as follows: ::
    
        % uname -a
        Linux host_name 2.6.18-53.1.14.el5xen #1 SMP Wed Mar 5 12:08:17 EST 2008 x86_64 x86_64 x86_64 GNU/Linux
    
    Make sure to install the CUBRID 64-bit version on 64-bit Linux. 
    
*   The libraries that should be added.
    
    *   Curses Library (rpm -q ncurses)

        CUBRID is packaged with version 5 of Curses library. You may need to install ncurses-compat-libs package if your system has newer version and downgrade is not possible.

    *   gcrypt Library (rpm -q libgcrypt)
    *   stdc++ Library (rpm -q libstdc++)
    
*   Check if the mapping between host names and IP addresses are correct in the /etc/hosts file.

    If host names and IP addresses are matched incorrectly, DB server cannot be started normally. Therefore, check if they are correctly mapped.

**Installation Process**

    **Specifying the Directory to Install**

    *   Decompress the compressed file to the directory to install.

        ::
        
            tar xvfz CUBRID-10.2.0.8787-a31ea42-Linux.x86_64.tar.gz /home1/cub_user/

        CUBRID directory is created under /home1/cub_user/ and files are created under CUBRID directory. 

    **Specifying Environment Variables**

    #.  Add below environment variables to a shell script which is run automatically and located under the home directory of a user.
    
        You may have to create a directory for **$CUBRID_DATABASES**. You can designate any directory you have enough permission.

        The below is an example to add environment variables to .bash_profile when you run on the bash shell.

        ::
        
            export CUBRID=/home1/cub_user/CUBRID
            export CUBRID_DATABASES=$CUBRID/databases
            
    #.  Add CUBRID JDBC library file name to the **CLASSPATH** environment variable.
    
        ::
        
            export CLASSPATH=$CUBRID/jdbc/cubrid_jdbc.jar:$CLASSPATH
            
    #.  Add CUBRID bin directory to **PATH** environment variables.
      
        ::
        
            export PATH=$CUBRID/bin:$PATH
                
    **Creating DB**
        
    *   Move to the directory to create DB on the console and create DB.

        ::
        
            cd $CUBRID_DATABASES
            mkdir testdb
            cd testdb
            cubrid createdb --db-volume-size=128M --log-volume-size=128M testdb en_US

    **Auto-starting when Booting**

    *   "cubrid" script is included in the **$CUBRID/share/init.d** directory. Change the value of **$CUBRID_USER** environment variable into the Linux account which installed CUBRID and register this script to **/etc/init.d**; then you can start automatically by using "service" or "chkconfig" command.
            
    **Auto-starting DB**    

    *   To start DB automatically when you booting a system, change the below in **$CUBRID/conf/cubrid.conf**.

        ::
            
            [service]
            service=server, broker, manager
            server=testdb

    *   In the "service" parameter, processes to be auto-started are specified.
    *   In the "server" parameter, DB name to be auto-started is specified.
        
For environment setting, tools installation and interfaces installation after CUBRID installation,  see :ref:`Installing-and-Running-on-Linux`.
            
Installing CUBRID with zip on Windows
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Checklist before Installing**

Check below list before installing CUBRID database of Windows version.

*   64bit 

    CUBRID supports only 64-bit Windows. You can check the version by selecting [My Computer] > [System Properties]. Make sure to install a CUBRID 64-bit version on 64-bit Windows.
    
    .. warning:: 10.1 would be the last release of 32-bit Windows.

**Installation Process**

    **Specifying the Directory to Install**

    *   Decompress the compressed file to the directory to install.

        ::
        
            C:\CUBRID
    *   You may have to create a directory for **$CUBRID_DATABASES**. You can designate any directory you have enough permission.

    **Specifying Environment Variables**

    #.  Select [Start button] > [Computer] > (click right mouse button) > [Properties] > [Advanced system settings] > [Environment Variables].
    #.  Click [New ...] under the system variables and add system variables as below.
    
        ::
        
            CUBRID = C:\CUBRID
            CUBRID_DATABASES = %CUBRID%\databases
            
    #.  Add CUBRID JDBC library name to **CLASSPATH** system variable.
    
        ::
        
            %CUBRID%\jdbc\cubrid_jdbc.jar       
            
    #.  Add CUBRID bin directory to **Path** system variable.
      
        ::
        
            %CUBRID%\bin
                
    **Creating DB**
        
    *   Run **cmd** command and open the console; move to the directory to create DB and create DB.

        ::
        
            cd C:\CUBRID\databases
            md testdb
            cd testdb
            c:\CUBRID\databases\testdb>cubrid createdb --db-volume-size=128M --log-volume-size=128M testdb en_US
    
    **Auto-starting when Booting**
    
    *   To start CUBRID automatically when booting the Windows system, CUBRID Service should be registered to Windows Service.
        
        #.  Register CUBRID Service to Windows Service.

            ::
            
                C:\CUBRID\bin\ctrlService.exe -i C:\CUBRID\bin
            
        #.  The below shows how to start/stop CUBRID Service.
        
            ::
            
                C:\CUBRID\bin\ctrlService.exe -start/-stop
            
    **Auto-starting DB**    

    *   To start DB when booting on Windows, change below in C:\\CUBRID\\conf\\cubrid.conf.

        ::
            
            [service]
            service=server, broker, manager
            server=testdb

        *   Specify the processes to start automatically on the "service" parameter.
        *   Specify the DB name to start automatically on the "server" parameter.

    **Removing from Service**

    *   To remove registered CUBRID Service, run the following.

        ::
        
            C:\CUBRID\bin\ctrlService.exe -u

**Registering CUBRID Service Tray**
    
Since CUBRID Service Tray is not automatically registered when installing CUBRID with zip file, it is required to register manually if you want CUBRID Service Tray.
    
#.  Create a link of C:\\CUBRID\\bin\\CUBRID_Service_Tray.exe in [Start button] > [All Programs] > [Startup].

#.  Input "regedit" in [Start button] > [Accessories] > [Run] to run a registry editor.

#.  Create CUBRID folder under [Computer] > [HKEY_LOCAL_MACHINE] > [SOFTWARE].

#.  Create [cmclient] folder under [CUBRID] folder(Edit > New > Key) and add below items(Edit > New > String Value).

    ::
    
        Name          Type       Data

        ROOT_PATH     REG_SZ     C:\CUBRID\cubridmanager
        
#.  Create [cmserver] folder under [CUBRID] folder(Edit > New > Key) and add below items(Edit > New > String Value).

    ::
    
        Name          Type       Data

        ROOT_PATH     REG_SZ     C:\CUBRID

#.  Create [CUBRID] folder under [CUBRID] folder(Edit > New > Key) and add below items(Edit > New > String Value).


    ::
    
        Name          Type       Data

        ROOT_PATH     REG_SZ     C:\CUBRID

#.  When rebooting Windows, CUBRID Service Tray is created under right side.
    
**Checklist After Installation**

*   Whether the start of CUBRID Service Tray or not

    If CUBRID Service Tray is not automatically started when starting a system, confirm the following.

    *   Check if Task Scheduler is started in [Start button] > [Control panel] > [Administrative Tools] > [Services]; if not, start Task Scheduler.
    
    *   Check if CUBRID Service Tray is registered in [Start button] > [All Programs] > [Startup]; if not, register CUBRID Service Tray.

For environment setting, tools installation and interfaces installation after CUBRID installation,  see :ref:`Installing-and-Running-on-Windows`.
