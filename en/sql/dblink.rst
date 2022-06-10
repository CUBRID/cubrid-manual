
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
CUBRID DBLink provides a function to inquire information in the databases of homogeneous CUBRID and heterogeneous Oracle and MySQL.
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
GATWAY uses ODBC (Open DataBase Connectivity).

.. image:: /images/dblink_heter.png


.. _gateway:

GATEWAY
==============================================

A gateway is a middleware that relays to connect to an external database server and is similar to a broker. The gateway connects the CUBRID Database Server to an external server which is Oracle/MySQL, to retrieve information from the external server and deliver it to the CUBRID Database Server.

A cubrid system including a gateway has a multi-hierarchical structure including cubrid_gateway, cub_gateway, and cub_cas_cgw as shown in the figure below.

.. image:: /images/gateway.png

cub_cas_cgw
----------------

cub_cas_cgw(CAS Gateway)는 CUBRID Database Server에서 외부의 Database의 연결을 요청하는 공용 서버 역할을 한다. 또한, cub_cas_cgw는 데이터베이스 서버의 클라이언트로 동작하여 CUBRID Database Server의 요청에 의해 외부 데이터베이스 서버와 연결을 제공한다. 서비스 풀(service pool) 내에서 구동되는 cub_cas_cgw의 개수는 cubrid_gateway.conf 설정 파일에 지정할 수 있으며, cub_gateway에 의해 동적으로 조정된다.
cub_cas_cgw (CUBRID Common Application Server and broker application server (CAS in short)) acts as a common application server used by all the application clients that request connections. cub_cas_cgw also acts as the database server’s client and provides the connection to the database server upon the client’s request. The number of cub_cas_cgw(s) running in the service pool can be specified in the cubrid_broker.conf file, and this number is dynamically adjusted by cub_gateway.

cub_gateway
----------------

cub_broker relays the connection between the application client and the cub_cas_cgw. That is, when an application client requests access, the cub_broker checks the status of the cub_cas_cgw through the shared memory, and then delivers the request to an accessible cub_cas_cgw . It then returns the processing results of the request from the cub_cas_cgw to the application client.

The cub_gateway는 also manages the server load by adjusting the number of cub_cas_cgw (s) in the service pool and monitors and manages the status of the cub_cas_cgw. If the cub_gateway는 delivers the request to cub_cas_cgw but the connection to cub_cas_cgw 1 fails because of an abnormal termination, it sends an error message about the connection failure to the application client and restarts cub_cas_cgw 1. Restarted cub_cas_cgw 1 is now in a normal stand-by mode, and will be reconnected by a new request from a new application client.

공유 메모리
-----------------

The status information of the cub_cas_cgw의 is stored in the shared memory, and the cub_broker refers to this information to relay the connection to the application client. With the status information stored in the shared memory, the system manager can identify which task the cub_cas_cgw의 is currently performing or which application client’s request is currently being processed.


Start GATEWAY
-----------------------

Enter the command below to start the GATEWAY.

::

    $ cubrid gateway start
    @ cubrid gateway start
    ++ cubrid gateway start: success

The following message is returned if the GATEWAY is already running.

::

    cubrid gateway start
    @ cubrid gateway start
    ++ cubrid gateway is running.

Stpping GATEWAY
-------------------------

Enter the below command to stop the GATEWAY.

::

    $ cubrid gateway stop
    @ cubrid gateway stop
    ++ cubrid gateway stop: success

The following message is returned if the GATEWAY has stopped.

::

    $ cubrid gateway stop
    @ cubrid gateway stop
    ++ cubrid gateway is not running.

Restarting GATEWAY
---------------------------

Enter the command below to restart the whole GATEWAY.

::

    $ cubrid gateway restart

.. _gateway-status:

Checking GATEWAY Status
--------------------------------

**cubrid gateway status**  The cubrid gateway status utility allows you to check the gateway status such as number of completed jobs and the number of standby jobs by providing various options.
GATEWAY status is the same as broker, so refer to :ref:`broker-status`\.

::

    cubrid gateway status [options] [expr]


CUBRID DBLINK settings
==============================================

The settings for using CUBRID DBLink are different from those of homogeneous DBLink and heterogeneous DBLink.

Homogeneous DBLink Setting
-------------------------------------

If you look at the Homogeneous configuration diagram above, you need to connect to the broker of the external database, so you need to set up the broker for the external database.
This setting is the same as the general broker setting.

Heterogeneous DBLink Setting
---------------------------------------

It is necessary to set the information to connect to a heterogeneous type (Oracle/MySQL), and the heterogeneous setting value must be written in GATEWAY.
GATEWAY can be configured through the parameters of **cubrid_gateway.conf** .


GATEWAY Parameter
------------------------

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

    **APPL_SERVER** is the part that sets the application server name of GATEWAY. To connect to an external server, it must be set to CAS_CGW.

**CGW_LINK_SERVER**

    **CGW_LINK_SERVER** should set the name of the external databaseto be used by connecting to CAS_CGW. Currently supported databases are Oracle and MySQL.

**CGW_LINK_SERVER_IP**

    **CGW_LINK_SERVER_IP** should set the IP address of the external database to be connected with CAS_CGW.


**CGW_LINK_SERVER_PORT**

    **CGW_LINK_SERVER_PORT** should set the port number of databaseto be connected with CAS_CGW.


**CGW_LINK_ODBC_DRIVER_NAME**

    **CGW_LINK_ODBC_DRIVER_NAME** must set the ODBC Driver name provided by the external database when connecting with CAS_CGW.

.. note::
    
        *   For Windows, if the ODBC Driver is installed, the driver name can be found through the ODBC Data Source Manager.
        *   For Linux, the driver name must be written directly in odbcinit.ini.

**CGW_LINK_CONNECT_URL_PROPERTY**

    **CGW_LINK_CONNECT_URL_PROPERTY** creates a property used in the connection string when connecting CAS_CGW to an external 

.. note::
    
        Property is different for each database, so refer to the site below.
	
        Oracle : https://docs.oracle.com/cd/B19306_01/server.102/b15658/app_odbc.htm#UNXAR418
	
        MySQL : https://dev.mysql.com/doc/connector-odbc/en/connector-odbc-configuration-connection-parameters.html#codbc-dsn-option-flags


cubrid_gateway.conf file
------------------------------------------------

The cubrid_gateway.conf file, the default gateway configuration file created when installing CUBRID, includes some parameters that must be modified by default. If you want to modify the values of parameters that are not included in the configuration file by default, you can add or modify one yourself.

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


*    GATEWAY SETTINGS FOR CONNECTION TO Oracle
	
    ::
    
	APPL_SERVER              	=CAS_CGW
	CGW_LINK_SERVER		        =ORACLE
	CGW_LINK_SERVER_IP      	=localhost
	CGW_LINK_SERVER_PORT    	=1521
	CGW_LINK_ODBC_DRIVER_NAME   =Oracle 12c ODBC driver
	CGW_LINK_CONNECT_URL_PROPERTY =


*     GATEWAY SETTINGS FOR CONNECTION TO MySQL
    
    ::
    
	APPL_SERVER                  =CAS_CGW
	CGW_LINK_SERVER		         =MYSQL
	CGW_LINK_SERVER_IP           =localhost
	CGW_LINK_SERVER_PORT         =3306 
	CGW_LINK_ODBC_DRIVER_NAME    =MySQL ODBC 8.0 Unicode Driver
	CGW_LINK_CONNECT_URL_PROPERTY ="charset=utf8;PREFETCH=100;NO_CACHE=1"




Install ODBC Driver
------------------------------------------------

You need to download and install Oracle/MySQL ODBC Driver from the site below.

Oracle ODBC Driver download site:

*   https://www.oracle.com/database/technologies/instant-client/downloads.html

MySQL ODBC Driver download site:

*   https://dev.mysql.com/downloads/connector/odbc/




Check and set ODBC Driver Name
------------------------------------------------

In case of Linux, after installing unixODBC to set Oracle and MySQL ODBC Driver Name
Driver name must be written in /etc/odbcinit.ini file.

*   Install unixODBC

unixODBC Driver Manager is an open source ODBC driver manager that can be used with ODBC drivers on Linux and UNIX operating systems.
For instructions on how to install the unixODBC driver manager, refer to the url below.
unixODBC website: http://www.unixodbc.org/



*   Setting ondbcinst.ini

    ::
		
	[MySQL ODBC 8.0 Unicode Driver]
	Driver=/usr/lib64/libmyodbc8w.so

	[Oracle 12c ODBC driver]
	Description = Oracle ODBC driver for Oracle 12c
	Driver = /usr/lib64/instantclient_12_2/libsqora.so.12.1
	

.. note::
    
        For reference, in the ondbcinst.ini setting, the driver names are MySQL ODBC 8.0 Unicode Driver and Oracle 12c ODBC driver, respectively.


How to use Cubrid DBLink
==============================================

If you set up homogeneous brokers and heterogeneous gateways, let's look at how to write Query statements to inquire about database information.

There are two ways to write DBLINK Query statement for data inquiry.

First, how to query information from other databases by writing DBLINK syntax in the FROM clause
The Query statement below is a Query statement that inquires the remote_t table information of another database of IP 192.168.0.1.

::
    
	SELECT * FROM DBLINK (192.168.0.1:53000:demodb:user:password:','SELECT col1, col2 FROM remote_t') AS t(col1 int, col2 varchar(32));


Second, if you look at the above DBLINK Query, information for accessing other databases is the most basic information. Therefore, there is a risk that user information (id, password) may be exposed to the outside and the inconvenience of having to write each time a Query is written.
If you use the CREATE SERVER statement for such trouble and information protection, it is simpler than the Query statement and helps to protect user information.



::
    
    CREATE SERVER remote_srv1 ( HOST='192.168.0.1', PORT=53000, DBNAME=demodb, USER=user, PASSWORD='password');
    SELECT * FROM DBLINK (remote_srv1, 'SELECT col1 FROM remote_t') AS t(col1 int);




.. note::
    
        For detailed DBLink SQL syntax, refer to :doc:`/sql/query/select` and :doc:`/sql/schema/server_stmt`.




Restrictions
==============================================

*   CUBRID DBLink only supports utf-8.
*   The maximum string length of one column is supported up to 16M.
*	In the case of Mysql, it is recommended to use PREFETCH, NO_CACHE=1 because the memory usage of Gateway CAS increases when cache is used for large tables.
*	ODBC non-supported types are SQL_INTERVAL, SQL_GUID, SQL_BIT, SQL_BINARY, SQL_VARBINARY, SQL_LONGVARBINARY.








