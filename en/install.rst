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
  
        $ ksh93 ./CUBRID-9.2.0.0146-AIX-ppc64.sh 
        $ bash ./CUBRID-9.2.0.0146-AIX-ppc64.sh 

Compatibility
-------------

**Application Compatibility**

*   Applications that use JDBC, PHP or CCI APIs from 2008 R2.0 or higher version of CUBRID can access the CUBRID 9.2 database. However, you must link the CUBRID 9.2 library or use the driver to use the added/improved features of JDBC, PHP or CCI interfaces.

*   Note that query results may differ from those given in the earlier version because new reserved words have been added, and the specifications for some queries have been changed.

*   An application that is developed by using the GLO class can be used after it is converted to an application or schema suitable to the BLOB or CLOB type.

**CUBRID Manager Compatibility**

*   CUBRID Manager guarantees backward compatibility with the servers using CUBRID 2008 R2.1 or higher and uses the CUBRID JDBC driver that matches each server version. However, you must use a CUBRID Manager that is higher than CUBRID servers in version in order to utilize all the features of CUBRID Manager. The CUBRID JDBC driver is included in the $CUBRID/jdbc directory when CUBRID is installed($CUBRID on Linux, %CUBRID% on Windows).

*   The bit version of CUBRID Manager must be identical to the bit version of JRE. For example, if a 64-bit DB server uses CUBRID Manager 32-bit version, JRE or JDK 32-bit version should be installed.

*   Drivers for 2008 R2.2 and higher versions are included in CUBRID Manager by default, which you can download separately from the http://www.cubrid.org Website.

.. note:: Old version user should upgrade all of driver, broker, DB server; Data migration should be done because its DB volume is not compatible with 9.2 version.
    For upgrade and data migration, see :doc:`/upgrade`.

Interoperability
----------------

*   If the CUBRID DB server and its broker server are operated separately, their interoperability is guaranteed, even when the operating systems are different. However, the bit version of a DB server must be identical to the bit version of a broker server. For example, the 64-bit DB server for Linux is interoperable with the 64-bit broker server for Windows, but it is not interoperable with a 32-bit broker server.

    For the relation between DB server and broker, see :doc:`intro`.For CUBRID SHARD, see :doc:`shard`.

Installation and Running CUBRID on Linux
----------------------------------------

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

      Make sure to install the CUBRID 32-bit version on 32-bit Linux and the CUBRID 64-bit version on 64-bit Linux. The following are the libraries that should be added.

      * Curses Library (rpm -q ncurses)
      * gcrypt Library (rpm -q libgcrypt)
      * stdc++ Library (rpm -q libstdc++)
  
**Installing CUBRID**

    The installation program consists of shell scripts that contain binary; thus it can be installed automatically. The following example shows how to install CUBRID with the "CUBRID-9.2.0.0201-linux.x86_64.sh" file on the Linux. ::

        $ sh CUBRID-9.2.0.0201-linux.x86_64.sh
        Do you agree to the above license terms? (yes or no) : yes
        Do you want to install this software(CUBRID) to the default(/home1/cub_user/CUBRID) directory? (yes or no) [Default: yes] : yes
        Install CUBRID to '/home1/cub_user/CUBRID' ...
        In case a different version of the CUBRID product is being used in other machines, 
        please note that the CUBRID 9.2 servers are only compatible with the CUBRID 9.2 clients and vice versa.
        Do you want to continue? (yes or no) [Default: yes] : yes
        Copying old .cubrid.sh to .cubrid.sh.bak ...

        CUBRID has been successfully installed.

        demodb has been successfully created.

        If you want to use CUBRID, run the following commands
        $ . /home1/cub_user/.cubrid.sh
        $ cubrid service start

    As shown in the example above, after installing the downloaded file (CUBRID-9.2.0.0201-linux.x86_64.sh), the CUBRID related environment variables must be set in order to use the CUBRID database. Such setting has been made automatically when logging in the concerned terminal. Therefore there is no need to re-set after the first installation. ::

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

        $ rpm -Uvh cubrid-9.2.0.0201-el5.x86_64.rpm

    When rpm is executed, CUBRID is installed in the "cubrid" home directory (/opt/cubrid) and related configuration file (cubrid.[c]sh) is installed in the /etc/profile.d directory. Note that *demodb* is not automatically installed. Therefore, you must executed /opt/cubrid/demo/make_cubrid_demo.sh with "cubrid" Linux ID. When installation is complete, enter the code below to start CUBRID with "cubrid" Linux ID. ::

        $ cubrid service start

    .. note:: \

        *   **RPM and dependency**
        
            You must check RPM dependency when installing with RPM. If you ignore (--nodeps) dependency, it may not be executed. Even if you remove RPM, user accounts and databases that are created after installing, you must remove it manually, if needed.
        
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
    
    * 64-bit

      Since version 2008 R2.0, CUBRID supports both 32-bit and 64-bit Windows. You can check the version by selecting [My Computer] > [System Properties]. Make sure to install the CUBRID 32-bit version on 32-bit Windows and the CUBRID 64-bit version on 64-bit Windows.

    If you want to install CUBRID on Windows Vista or higher, execute the installation file with administrative privileges.

    * On the popup menu after clicking right mouse button on the CUBRID installation file, choose [Execute as an administrator (A)].

**Installation Process**
    
    **Step 1: Specifying the directory to install**
    
    **Step 2: Selecting Setup Type**

    *   **Server and Driver Installation** : CUBRID Server, CSQL (a command line tool), interface drivers (JDBC, C API) are all installed.

    *   **Driver Installation** : Only the interface drivers (JDBC, C API) are  installed. You can select this type of installation if development or operation is performed by remote connection to the computer in which the CUBRID database server is installed.

    **Step 3: Creating a sample database**
        
        To create a sample database, it requires 300MB disk space. 
    
    **Step 4: Completing the installation**
    
        CUBRID Service Tray appears on the right bottom.

    .. note:: 
    
        CUBRID Service is automatically started when the system is rebooted. If you want to stop the  when the system is rebooted, change the "Start parameters" of "CUBRIDService" as "Stop"; "Control Panel > Administrative Tools > Services" and double-clicking "CUBRIDService", then pop-up window will be shown.

**Upgrading CUBRID**

    To install a new version of CUBRID in an environment in which a previous version has already been installed, select [CUBRID Service Tray] > [Exit] from the menu to stop currently running services, and then remove the previous version of CUBRID. Note that when you are prompted with "Do you want to delete all the existing version of databases and the configuration files?" you must select "No" to protect the existing databases.

    For more information on upgrading a database from a previous version to a new version, see :doc:`upgrade`.

**Configuring Environment**

    You can change configuration such as service ports to meet the user environment by changing the parameter values of following files which are located in the **%CUBRID%\\conf** directory. If a firewall has been configured, the ports used in CUBRID need to be opened.

    * **cm.conf**

      A configuration file for CUBRID Manager. The port that the Manager server process uses is called  **cm_port** and its default value is **8001** . Two ports are used and the port number is determined by the **cm_port** parameter. If 8001 is specified, 8001 and 8002 (configured number plus 1) ports will be used. For details, see `CUBRID Manager Manual <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual>`_ .

    * **cm_ext.conf**
     
      A configuration file for CUBRID Web Manager. **listen** is the port to be used in the web manager server process, and its default value is **8282**. For more details, see `CUBRID Web Manager Manual <http://www.cubrid.org/wiki_tools/entry/cubrid-web-manager-manual>`_ .

    * **cubrid.conf**

      A configuration file for server. You can use it to configure the following values: database memory, the number threads based on the number of concurrent users, communication port between broker and server, etc.  The port that a master process uses is called cubrid_port_id and its default value is 1523. For details, see :ref:`cubrid-conf-default-parameters`.

    * **cubrid_broker.conf**

      A configuration file for broker. You can use it to configure the following values: broker port, the number of application servers (CAS), SQL LOG, etc. The port that a broker uses is called **BROKER_PORT**. A port you see in the drivers such as JDBC is its corresponding broker's port. **APPL_SERVER_PORT** is a port that a broker application server (CAS) uses and it is added only in Windows. The default value is  **BROKER_PORT** +1. The number of ports used is the same as the number of CAS, starting from the specified port's number plus 1. For details, see :ref:`parameter-by-broker`.

      For example, if the value of **APPL_SERVER_PORT** is 35000 and the maximum number of CASes by **MAX_NUM_APPL_SERVER** is 50, then listening ports on CASes are 35000, 35001, ..., 35049.
      For more details, see :ref:`parameter-by-broker`. 
      
      The **CCI_DEFAULT_AUTOCOMMIT** broker parameter is supported since 2008 R4.0. The default value in the version is **OFF** and it is later changed to **ON** .  Therefore, users who have upgraded from 2008 R4.0 to 2008 R4.1 or later versions should change this value to **OFF** or configure the auto-commit mode to **OFF** .

**Installing CUBRID Interfaces**

    You can see the latest information on interface modules such as JDBC, PHP, ODBC, and OLE DB and install them by downloading files from `http://www.cubrid.org/wiki_apis <http://www.cubrid.org/wiki_apis>`_ .

    A simple description on each driver can be found on :doc:`/api/index`.

**Installing CUBRID Tools**

    You can see the latest information on tools such as CUBRID Manager and CUBRID Query Browser and install them by downloading files from `http://www.cubrid.org/wiki_tools <http://www.cubrid.org/wiki_tools>`_ .
    
    CUBRID Web Manager is started when the CUBRID is installed, and you can see this by accessing to `https://localhost:8282/ <https://localhost:8282/>`_.


            