
:meta-keywords: cubrid dblink
:meta-description: CUBRID supports DBLink, which can connect to an external server and search information.

***********************
CUBRID DBLink
***********************

.. _dblink-introduction:

Introduction to CUBRID DBLink
==============================================

When retrieving information from a database, it is often necessary to retrieve information from an external database.
In this way, if you use CUBRID DBLink to inquire information in an external database, you can access information in other databases.
CUBRID DBLink provides a function to inquire information in the databases of homogeneous CUBRID and heterogeneous Oracle, MySQL and MariaDB.

It has the advantage of being able to directly inquire information from an external database.
However, it is possible to set up multiple external databases, but when searching for information, it is possible to inquire information from only one other database.

.. _dblink-diagram:

CUBRID DBLink diagram
==============================================

CUBRID DBLink supports DBLink between homogeneous and heterogeneous DBLinks.

Homogeneous DBLink diagram
-----------------------------

If you look at the configuration diagram for inquiring information of a homogeneous database, you can use CCI in Database Server to connect to homogeneous brokers and inquire information from an external database.


.. image:: /images/dblink_homo.png

Heterogeneous DBLink diagram 
-----------------------------

If you look at the configuration diagram for inquiring information in heterogeneous databases, you can inquire information in heterogeneous databases through GATEWAY.
The gateway uses the ODBC (Open DataBase Connectivity) driver of the connecting database.

.. image:: /images/dblink_heter.png


.. _gateway-info:

GATEWAY
==============================================

A gateway is a middleware that mediates between the CUBRID database and heterogeneous databases and is similar to a broker. The gateway connects to a heterogeneous database server (Oracle/MySQL/MariaDB etc), retrieves data, and delivers it to the CUBRID database server.


A cubrid system including a gateway has a multi-hierarchical structure including cub_gateway and cub_cas_cgw as shown in the figure below.

.. image:: /images/gateway.png

cub_cas_cgw
----------------

cub_cas_cgw (CAS Gateway) acts as a common application server used by all the application clients that request connections. cub_cas_cgw also acts as the database server’s client and provides the connection to the database server upon the client’s request. The number of cub_cas_cgw(s) running in the service pool can be specified in the cubrid_broker.conf file, and this number is dynamically adjusted by cub_gateway.

cub_gateway
----------------

cub_gateway mediates the connection between the application client and the cub_cas_cgw. That is, when an application client requests access, the cub_broker checks the status of the cub_cas_cgw through the shared memory, and then delivers the request to an accessible cub_cas_cgws. It then returns the processing results of the request from the cub_cas_cgw to the application client.

The cub_gateway also manages the server load by adjusting the number of cub_cas_cgw (s) in the service pool and monitors and manages the status of the cub_cas_cgw. If the cub_gateway delivers the request to cub_cas_cgw but the connection to cub_cas_cgw 1 fails because of an abnormal termination, it sends an error message about the connection failure to the application client and restarts cub_cas_cgw 1. Restarted cub_cas_cgw 1 is now in a normal stand-by mode, and will be reconnected by a new request from a new application client.

Shared memory
-----------------

The status information of the cub_cas_cgw의 is stored in the shared memory, and the cub_broker refers to this information to mediate the connection to the application client. With the status information stored in the shared memory, the system manager can identify which task the cub_cas_cgw의 is currently performing or which application client’s request is currently being processed.


Start GATEWAY
-----------------------

Enter the command below to start the GATEWAY.

::

    $ cubrid gateway start


The following message is returned if the GATEWAY is already running.

::

    $ cubrid gateway start

Stopping GATEWAY

-------------------------

Enter the below command to stop the GATEWAY.

::

    $ cubrid gateway stop

The following message is returned if the GATEWAY has stopped.

::

    $ cubrid gateway stop
    @ cubrid gateway stop
    ++ cubrid gateway is not running.

Restarting GATEWAY
---------------------------

Enter the below command to restart the GATEWAY.

::

    $ cubrid gateway restart

.. _gateway-status-command:

Checking GATEWAY Status
--------------------------------

**cubrid gateway status** provides various options and allows you to check the gateway status information, including the number of completed tasks for each gateway and the number of tasks waiting to be processed.

GATEWAY status is the same as broker, so refer to :ref:`broker-status`\.

::

    cubrid gateway status [options] [expr]

Starting the GATEWAY together when the CUBRID service starts
---------------------------------------------------------------------------------

To have *GATEWAY* started upon startup of the CUBRID service (cubrid service start), configure *gateway* in the **service**  parameter of the **cubrid.conf**  file. ::

    # cubrid.conf

    [service]

    service=server,broker,gateway,manager

    ...

CUBRID DBLINK settings
======================================

The settings for using CUBRID DBLink are different from those of homogeneous DBLink and heterogeneous DBLink.


Homogeneous DBLink Setting

---------------------------------------

If you look at the homogeneous DBlink diagram above, you need to connect to the broker in the remote database, so you need to set up the broker in the remote database.
This setting is the same as the general broker setting.


Heterogeneous DBLink Setting
---------------------------------------

To connect to a heterogeneous database (Oracle/MySQL/MariaDB), cubrid_gataway.conf, unixODBC installation, and ODBC Driver information settings are required.

.. _gatewayconf-info:

Gateway configuration file
----------------------------------------

The parameters used in cubrid_gataway.conf, the default gateway configuration file created when CUBRID is installed, are almost identical to the broker parameters, and some additional parameters that must be changed are included.
Parameter values not included by default can be directly added/edited and used.
The following is the content of the cubrid_gateway.conf file that is provided by default during installation.

The following is the content of the cubrid_gateway.conf file provided by default.


::
    
 	[gateway]
	MASTER_SHM_ID           =50001
	ADMIN_LOG_FILE          =log/gateway/cubrid_gateway.log

	[%oracle_gateway]
	SERVICE                 =OFF
	SSL			=OFF
	APPL_SERVER             =CAS_CGW
	BROKER_PORT             =53000
	MIN_NUM_APPL_SERVER     =5
	MAX_NUM_APPL_SERVER     =40
	APPL_SERVER_SHM_ID      =53000
	LOG_DIR                 =log/gateway/sql_log
	ERROR_LOG_DIR           =log/gateway/error_log
	SQL_LOG                 =ON
	TIME_TO_KILL            =120
	SESSION_TIMEOUT         =300
	KEEP_CONNECTION         =AUTO
	CCI_DEFAULT_AUTOCOMMIT  =ON
	APPL_SERVER_MAX_SIZE    =256
	CGW_LINK_SERVER		=ORACLE
	CGW_LINK_SERVER_IP      =localhost
	CGW_LINK_SERVER_PORT    =1521
	CGW_LINK_ODBC_DRIVER_NAME   =Oracle_ODBC_Driver
	CGW_LINK_CONNECT_URL_PROPERTY       =


	[%mysql_gateway]
	SERVICE                 =OFF
	SSL			=OFF
	APPL_SERVER             =CAS_CGW
	BROKER_PORT             =56000
	MIN_NUM_APPL_SERVER     =5
	MAX_NUM_APPL_SERVER     =40
	APPL_SERVER_SHM_ID      =56000
	LOG_DIR                 =log/gateway/sql_log
	ERROR_LOG_DIR           =log/gateway/error_log
	SQL_LOG                 =ON
	TIME_TO_KILL            =120
	SESSION_TIMEOUT         =300
	KEEP_CONNECTION         =AUTO
	CCI_DEFAULT_AUTOCOMMIT  =ON
	APPL_SERVER_MAX_SIZE    =256
	CGW_LINK_SERVER		=MYSQL
	CGW_LINK_SERVER_IP      =localhost
	CGW_LINK_SERVER_PORT    =3306 
	CGW_LINK_ODBC_DRIVER_NAME   =MySQL_ODBC_Driver
	CGW_LINK_CONNECT_URL_PROPERTY       ="charset=utf8;PREFETCH=100;NO_CACHE=1"

	[%mariadb_gateway]
	SERVICE                 =OFF
	SSL			=OFF
	APPL_SERVER             =CAS_CGW
	BROKER_PORT             =59000
	MIN_NUM_APPL_SERVER     =5
	MAX_NUM_APPL_SERVER     =40
	APPL_SERVER_SHM_ID      =59000
	LOG_DIR                 =log/gateway/sql_log
	ERROR_LOG_DIR           =log/gateway/error_log
	SQL_LOG                 =ON
	TIME_TO_KILL            =120
	SESSION_TIMEOUT         =300
	KEEP_CONNECTION         =AUTO
	CCI_DEFAULT_AUTOCOMMIT  =ON
	APPL_SERVER_MAX_SIZE    =256
	CGW_LINK_SERVER		=MARIADB
	CGW_LINK_SERVER_IP      =localhost
	CGW_LINK_SERVER_PORT    =3306 
	CGW_LINK_ODBC_DRIVER_NAME   =MariaDB_ODBC_Driver
	CGW_LINK_CONNECT_URL_PROPERTY       =


GATEWAY Parameter
------------------------

This parameter is set to use a heterogeneous database and DBLink.

The meaning of each parameter is slightly different for each heterogeneous database.

+-------------------------------+-------------+------------------------------------------------------------+
| Parameter Name                | Type        | Value                                                      |
+===============================+=============+============================================================+
| APPL_SERVER                   | string      |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
| CGW_LINK_SERVER               | string      |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
| CGW_LINK_SERVER_IP            | string      |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
| CGW_LINK_SERVER_PORT          | int         |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
| CGW_LINK_ODBC_DRIVER_NAME     | string      |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
| CGW_LINK_CONNECT_URL_PROPERTY | string      |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
     
  
**APPL_SERVER**

    **APPL_SERVER** is the part that sets the application server name of the gateway and must be set as CAS_CGW.

**CGW_LINK_SERVER**

    **CGW_LINK_SERVER** should set the name of the heterogeneous database to be used by connecting to CAS_CGW. Currently, supported databases are Oracle, MySQL and MariaDB.



**CGW_LINK_SERVER_IP**

    **CGW_LINK_SERVER_IP** should set the IP address of the heterogeneous database to be connected with CAS_CGW.

.. note::
    
    *   In case of Oracle, net_service_name of tnsnames.ora is used, so this parameter is not used.
    *   For details, refer to :ref:`Setting Connection Information for Connection to Oracle Database <tnsnames-info>`\ .

**CGW_LINK_SERVER_PORT**

    **CGW_LINK_SERVER_PORT** should set the port number of databaseto be connected with CAS_CGW.

.. note::
    
    *   In case of Oracle, net_service_name of tnsnames.ora is used, so this parameter is not used.
    *   For details, refer to :ref:`Setting Connection Information for Connection to Oracle Database <tnsnames-info>`\ .

**CGW_LINK_ODBC_DRIVER_NAME**

    **CGW_LINK_ODBC_DRIVER_NAME** must set the ODBC Driver name provided by the heterogeneous database when connecting with CAS_CGW.

.. note::
    
    *   In Windows, if the ODBC driver of the heterogeneous database is installed, the driver name can be checked through the ODBC data source manager.
    *   In Linux, the driver name must be specified directly in odbcinit.ini.
    *   For details, refer to :ref: `ODBC Driver Information Settings <odbcdriver-info>`\.

**CGW_LINK_CONNECT_URL_PROPERTY**

    **CGW_LINK_CONNECT_URL_PROPERTY** sets the connection property used for the connection string for heterogeneous database connection.


.. note::
    
    *   Connection properties are different for each database, so refer to the site below.
    *   Oracle : https://docs.oracle.com/cd/B19306_01/server.102/b15658/app_odbc.htm#UNXAR418
    *   MySQL : https://dev.mysql.com/doc/connector-odbc/en/connector-odbc-configuration-connection-parameters.html#codbc-dsn-option-flags
    *   MariaDB : https://mariadb.com/kb/en/about-mariadb-connector-odbc/#general-connection-parameters



Install unixODBC
-------------------------------

The unixODBC Driver Manager is an open source ODBC driver manager that can be used with ODBC drivers on Linux and UNIX operating systems.
To use ODBC in the gateway, unixODBC must be installed.

.. note::
    
        *   In Winodws, you can use Microsoft® ODBC Data Source Manager, which is installed by default.

How to install unixODBC

::
    
	$ wget http://www.unixodbc.org/unixODBC-2.3.9.tar.gz
	$ tar xvf unixODBC-2.3.9.tar.gz
	$ cd unixODBC-2.3.9
	$ ./configure
	$ make
	$ make install
	
.. note::
    
    For instructions on how to install the unixODBC driver manager, refer to the url below.

    unixODBC website: http://www.unixodbc.org/


ODBC Driver information setting
------------------------------------------------

After unixODBC is installed, the ODBC Driver information of the database to be connected must be registered.

ODBC Driver information is registered by directly modifying odbcinst.ini.

The following is an example of setting MySQL, Oracle and MariaDB ODBC Driver information.


::
		
	[MySQL ODBC 8.0 Unicode Driver]
	Description = MySQL ODBC driver v8.0
	Driver=/usr/lib64/libmyodbc8w.so

	[Oracle 11g ODBC driver]
	Description = Oracle ODBC driver v11g
	Driver = /home/user/oracle/instantclient/libsqora.so.11.1

	[mariadb odbc 3.1.13 driver]
	Description= mariadb odbc driver 3.1.13
	Driver=/home/user/mariadb-odbc-3.1.13/lib64/mariadb/libmaodbc.so	

.. note::
    
        For reference, in the example above, the driver names are "MySQL ODBC 8.0 Unicode Driver", "Oracle 11g ODBC driver" and "mariadb odbc 3.1.13 driver" respectively.



Oracle Setting for DBLink
==============================================

Oracle Environment Configuration
----------------------------------------------

To use Oracle in DBLink, you must install and configure Oracle Instant Client, set connection information, set Oracle Database environment variables, and set gateway.

**Install Oracle Instant Client ODBC**

Download ODBC Package and Basic Package from the Oracle Instant Client download site and extract them to the same directory.

::
    
	unzip instantclient-basic-linux.x64-11.2.0.4.0.zip
	unzip instantclient-odbc-linux.x64-11.2.0.4.0.zip

Oracle Instant Client Download Site: https://www.oracle.com/database/technologies/instant-client/downloads.html


**Oracle Instant Client Environment Variable Settings**


::

	export ORACLE_INSTANT_CLIENT=/home/user/oracle/instantclient  
	export PATH=$ORACLE_INSTANT_CLIENT:$PATH
	export LD_LIBRARY_PATH=$ORACLE_INSTANT_CLIENT:$LD_LIBRARY_PATH


.. _tnsnames-info:

**Set connection information for connection to Oracle Database**

In order to connect to Oracle Database, the tnsnames.ora file containing connection information must be modified.
Connection information should be written in these three items: HOST, PORT, and SERVICE_NAME in the basic format below.
For the tnsnames.ora file in which connection information is created, the directory path must be set in the TNS_ADMIN environment variable.
For how to set TNS_ADMIN, refer to "Setting TNS_ADMIN Environment Variable".


Default format of the tnsnames.ora file

::
	
	net_service_name =
	  (DESCRIPTION=
		(ADDRESS = (PROTOCOL = TCP)(HOST = xxx.xxx.xxx.xxx)(PORT = 1521)
	  )
	  (CONNECT_DATA =
		(SERVICE_NAME=service_name)
	  )
	)


* net_service_name: The name of the net service for database connection, and the name used for the db_name of the connection url.
* HOST: IP address or server name to connect to the database.
* PORT: Port required for connection. In most cases, the default port is 1521.
* SERVICE_NAME: The name of the database to connect to.

.. note::

		For reference, no error occurs even if net_service_name is duplicated. However, since it may be connected to a different server than the intended one, net_service_name must be set so that it does not overlap.


**Oracle Database Environment Variable Settings**

The following environment variables must be set in the Oracle database server.

::
	
	export ORACLE_SID=XE
	export ORACLE_BASE=/u01/app/oracle
	export ORACLE_HOME=$ORACLE_BASE/product/11.2.0/xe
	export PATH=$ORACLE_HOME/bin:$PATH


* ORACLE_SID is the system identifier.
* ORACLE_BASE is the Oracle base directory.
* ORACLE_HOME is the path where the Oracle database is installed.


.. _tns_admin-info:

**TNS_ADMIN environment variable setting**

TNS_ADMIN points to the directory path where the tnsnames.ora file is located.
If there is a tnsnames.ora file in /home/user/myconfigs, you can settings it as follows.

::
	
	export TNS_ADMIN=/home/user/myconfigs


**Configuring cubrid_gataway.conf for Oracle**

In order to connect to Oracle from the gateway, several settings are required as follows.

For details, refer to :ref:`Gateway configuration file <gatewayconf-info>`\.

Because the gateway uses the information in tnsnames.ora to connect to oracle, it is not necessary to write CGW_LINK_SERVER_IP and CGW_LINK_SERVER_PORT. Even if the corresponding information is written, the gateway does not refer to it.

	
::
    
	APPL_SERVER              	=CAS_CGW
			.
			.
			.	
	CGW_LINK_SERVER		        =ORACLE
	CGW_LINK_ODBC_DRIVER_NAME   =Oracle 12c ODBC driver
	CGW_LINK_CONNECT_URL_PROPERTY =


MySQL Configuration for DBLink
===========================================

MySQL Environment Configuration
-------------------------------------------

**Install MySQL ODBC Driver**

MySQL Unicode ODBC Driver is required to connect to MySQL from the gateway.
The following is how to install MYySQL ODBC Driver.

Use the MySQL Yum repository to provide the Connector/ODBC RPM package. The MySQL Yum repository should be in your system's list of repositories,
If not, select the package for your platform from the MySQL Yum repository download page ( https://dev.mysql.com/downloads/repo/yum/ ) and download it.

Install the downloaded release package.

::
    
	$ sudo yum install mysql80-community-release-el6-{version-number}.noarch.rpm


Update the repository using yum.

::
    
	$ sudo yum update mysql-community-release

Install Connector/ODBC with the command below.

::
    
	$ sudo yum install mysql-connector-odbc

For detailed installation instructions, refer to https://dev.mysql.com/doc/connector-odbc/en/connector-odbc-installation-binary-yum.html.


**Configuring cubrid_gataway.conf for MySQL**

In order to connect to MySQL from the gateway, several settings are required as below.

For details, refer to :ref:`gateway configuration file <gatewayconf-info>`\.


::
    
	APPL_SERVER                  =CAS_CGW
			.
			.
			.	
	CGW_LINK_SERVER		         =MYSQL
	CGW_LINK_SERVER_IP           =localhost
	CGW_LINK_SERVER_PORT         =3306 
	CGW_LINK_ODBC_DRIVER_NAME    =MySQL ODBC 8.0 Unicode Driver
	CGW_LINK_CONNECT_URL_PROPERTY ="charset=utf8;PREFETCH=100;NO_CACHE=1"


MariaDB Configuration for DBLink
=======================================

MariaDB Environment Configuration
---------------------------------------
 
**Install MariaDB ODBC Driver**

MariaDB ODBC Driver is required to connect to MariaDB from the gateway.

The following is how to install MariaDB ODBC Driver.

MariaDB Connector/ODBC package can be downloaded by selecting the version from the page below.

https://mariadb.com/downloads/connectors/


Extract the files from the downloaded tarball package. Then, install the driver's shared library in an appropriate location in the system.
The installed driver must register driver information in odbcinst.ini. For settings method, refer to :ref: `ODBC Driver Information Settings <odbcdriver-info>`\.

::
    
	$ mariadb-connector-odbc-3.1.13-centos7-amd64.tar.gz -C mariadb-odbc-3.1.13

For detailed installation instructions, refer to  https://mariadb.com/kb/en/about-mariadb-connector-odbc/#installing-mariadb-connectorodbc-on-linux.


**Configuring cubrid_gataway.conf for MariaDB**

In order to connect to MariaDB from the gateway, several settings are required as below.

For details, refer to :ref:`gateway configuration file <gatewayconf-info>`\.

  
::
    
	APPL_SERVER                  =CAS_CGW
			.
			.
			.	
	CGW_LINK_SERVER		         =MARIADB
	CGW_LINK_SERVER_IP           =localhost
	CGW_LINK_SERVER_PORT         =3306 
	CGW_LINK_ODBC_DRIVER_NAME    =mariadb odbc 3.1.13 driver



How to use Cubrid DBLink
==============================================

If the information on brokers of CUBRID to be connected to use DBLink has been completed or gateway settings for heterogeneous databases have been completed, let's learn how to write a query statement using DBLink.


There are two ways to write DBLINK Query statement for data inquiry.

**First**, how to query information from other databases by writing DBLINK syntax in the FROM clause

The Query statement below is a Query statement that inquires the remote_t table information of another database of IP 192.xxx.xxx.xxx.

::
    
	SELECT * FROM DBLINK ('192.xxx.xxx.xxx:53000:testdb:user:password:','SELECT col1, col2 FROM remote_t') AS t(col1 int, col2 varchar(32));

.. note::
    
	In case of Oracle, ip and port are gateway connection information among remote connection information, and net_service_name of tnsnames.ora must be entered in the db_name field. If net_service_name is ora_test, write as follow

	SELECT * FROM DBLINK ('192.xxx.xxx.xxx:53000:ora_test:user:password:','SELECT col1, col2 FROM remote_t') AS t(col1 int, col2 varchar(32));
	

**Second**, use the CREATE SERVER statement to protect user information (id, password) and the hassle of writing connection information every time you write a Query. If the CREATE SERVER statement is used, the Query statement becomes concise and helps to protect user information.


::
    
    CREATE SERVER remote_srv1 ( HOST='192.xxx.xxx.xxx', PORT=53000, DBNAME=testdb, USER=user, PASSWORD='password');
    SELECT * FROM DBLINK (remote_srv1, 'SELECT col1 FROM remote_t') AS t(col1 int);


.. note::

    DBLINK can set additional connection properties in the connection URL. For detailed attribute details, refer to :ref:`CCI driver's cci_connect_with_url function <cci_connect_with_url>`\
   
    If the DBLINK target database is configured as an HA environment, you can use the altHosts attribute to set it as shown in the example below.
	
    192.168.0.1:53000:testdb:user:password::?altHosts=192.168.0.2:33000,192.168.0.3:33000

    In the example, if the 192.168.0.1 server is an active database and cannot connect to the server, it is the setting to request a connection to the 192.168.0.2 server. As in the example above, multiple altHosts can be specified, and connections are attempted in the order listed.

    For detailed DBLink SQL syntax, refer to :doc:`/sql/query/select` and :doc:`/sql/schema/server_stmt`.




Restrictions
==============================================

*   DBLink for heterogeneous databases only supports utf-8.
*   Only Unicode ODBC Driver must be used in the gateway.
*   The maximum string length of one column is supported up to 16M.
*   When using cache in Mysql, it is recommended to use PREFETCH, NO_CACHE=1 because the memory usage of the gateway cub_cas_cgw increases.
*   ODBC non-supported types are SQL_INTERVAL, SQL_GUID, SQL_BIT, SQL_BINARY, SQL_VARBINARY, SQL_LONGVARBINARY.
*   When using DBLink with heterogeneous types (Oracle/MySQL/MariaDB), you must use Oracle/MySQL/MariaDB's Unicode ODBC driver.
*   When performing a query that includes the repeat() function in MySQL/MariaDB, part of the string may be cut off or the string may not be read.






