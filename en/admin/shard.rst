************
CUBRID SHARD
************

Database Sharding
=================

**Horizontal partitioning**

Horizontal partition is a design to partition the data for which schema is identical, based on the row, into multiple tables and store them. For example, 'User Table' with identical schema can be divided into 'User Table #0' in which users less than 13 years are stored and 'User Table #1' in which users 13 or greater than 13 years old.

.. image:: /images/image38.png

**Database sharding**

Database sharding is to store data into physically separate databases through horizontal partitioning and to retrieve them. For example, it is a method to store users less than 13 years old in database 0 and store users 13 or greater than 13 years old in database 1 when 'User Table' is located through multiple database. In addition to performance, database sharding is used to distribute and save large data which cannot be saved into one database instance.

Each partitioned database is called a shard or database shard.

.. image:: /images/image39.png

**CUBRID SHARD**

The CUBRID SHARD is middleware for database sharding and its characteristics are as follows:

*   Middleware that is used to minimize changes in existing applications, allowing access to transparently sharded data by using Java Database Connectivity (JDBC), a popular choice, or CUBRID C Interface (CCI), which is CUBRID C API.
*   In this function, a hint may be added to an existing query to indicate a shard in which the query would be executed.
*   It can be configured on various backend shard DBs such as MySQL and Oracle, as well as CUBRID.
*   It guarantees the unique characteristics of certain transactions.

More details on each characteristic will be described in the next chapter.

.. _shard-terminologies:

CUBRID SHARD Terminologies
==========================

The teminologies used to describe CUBRID SHARD are as follows:

*   **shard DB** : A database that includes a split table and data and processes user requests

*   **shard metadata** : Configuration information for the operation of a CUBRID SHARD. It analyzes the requested query and includes information to select a shard DB which will execute a query and to create a database session with the shard DB.

*   **shard key (column)** : A column used as an identifier to select a shard in the sharding table
*   **shard key data** : A shard key value that corresponds to the hint to identify the shard from the query
*   **shard ID** : An identifier to identify a shard DB
*   **shard proxy** : A CUBRID middleware process that analyzes hints included in user queries and sends requests to a shard DB which will process the query, based on the analyzed hint and the shard metadata

CUBRID SHARD Main Features
==========================

Middleware Structure
--------------------

The CUBRID SHARD is middleware, positioned between an application and logically, or physically, split shards. It keeps connection with multiple applications, sends requests from applications to the appropriate shard, and returns the results to the applications.

.. image:: /images/image40.png

Generally, it uses Java Database Connectivity (JDBC) or CUBRID C Interface (CCI), an interface used to connect to the CUBRID SHARD, and processes the requests from applications. It does not require additional driver or framework, minimizing the changes in applications.

The CUBRID SHARD middleware consists of three processes (broker/proxy/cas) and the brief functionalities of each process are as follows:

.. image:: /images/image41.png

*   **shard broker**

    *   Receives the initial connection request from drivers such as JDBC/CCI and then sends the received request to the shard proxy based on the load balancing policy.
    *   Monitors and restores the status of the shard proxy process and the shard CAS process.

*   **shard proxy**

    *   Sends the user request from the driver and then returns the processing result to the application.
    *   Manages the connection between the driver and the CAS and processes transactions.

*   **shard CAS**

    *   Creates a connection with the split shard DB and processes the user request received from the shard proxy by using the connection.
    *   Processes transactions.

Selecting a Shard DB through the Shard SQL Hint
-----------------------------------------------

	**Shard SQL Hint**

	With the hints and configuration data included in a SQL hint statement, the CUBRID SHARD selects a shard DB that will process the requests from applications. The types of available SQL hints are as follows:

	+----------------------+------------------------------------------------------------------------------------------------------------------------------------+
	| SQL hint             | Description                                                                                                                        |
	+======================+====================================================================================================================================+
	| **/*+ shard_key */** | A hint to specify the position of the bind variable or literal value that corresponds to the shard key column.                     |
	+----------------------+------------------------------------------------------------------------------------------------------------------------------------+
	| **/*+ shard_val(**   | A hint to explicitly specify the shard key within the hint when there is no column that corresponds to the shard key in the query. |
	| *value*              |                                                                                                                                    |
	| **) */**             |                                                                                                                                    |
	+----------------------+------------------------------------------------------------------------------------------------------------------------------------+
	| **/*+ shard_id(**    | A hint used when a user specifies a shard DB to process queries.                                                                   |
	| *shard_id*           |                                                                                                                                    |
	| **) */**             |                                                                                                                                    |
	+----------------------+------------------------------------------------------------------------------------------------------------------------------------+

	The terms are summarized as shown below: For more information on shard terms, see :ref:`shard-terminologies`.

	*   **shard key** : A column to distinguish shard DBs. In general, this column exists in all or most tables in a shard DB and has a unique value.
	*   **shard id** : An identifier that can be used to logically distinguish shards. For example, when one DB is split into four shard DBs, there are four shard IDs.

	For more information on the query process using hints and configuration information, see :ref:`General Procedure of Executing Queries by Using Shard SQL Hint <using-shard-hint>`.

	[번역]
	
	.. note::

		* When more than one shard hints exist on a query, it works normally if shard hints indicate the same shards, or it fails if they indicates the different shards. 
		
			::
	
				SELECT * FROM student WHERE shard_key = /*+ shard_key */ 250 OR shard_key = /*+ shard_key */ 22;
	
			On the above case, it works normally if the shard keys 250 and 22 indicate the same shard, but it fails if they indicate the different shards.
	
		* On some driver functions which batches the queries with an array by binding the several values(ex. PreparedStatement.executeBatch in JDBC, cci_execute_array in CCI), if at least the one which accesses to the other shard exists, all executions of the queries fail.
	
		* Functions to run several statements at one time on shard environment(ex. Statement.executeBatch in JDBC, cci_execute_batch in CCI) will be supported later.

	**shard_key Hint**

	The **shard_key** hint is to specify the position of a bind or literal variable. This hint should be positioned in front of either of them.

	Ex) Specifies the position of a bind variable. Executes the query in the shard DB corresponding to the student_no value that would be bound when executed.

	.. code-block:: sql

		SELECT name FROM student WHERE student_no = /*+ shard_key */ ?

	Ex) Specifies the position of a literal value. Executes the query in the shard DB corresponding to the student_no value (the literal value) that is 123 when executed.

	.. code-block:: sql

		SELECT name FROM student WHERE student_no = /*+ shard_key */ 123

	**shard_val Hint**

	The **shard_val** hint is used when there is no shard column that can be used to identify the shard DB in the query. It sets the shard key column as the value of the **shard_val** hint. The **shard_val** hint can be positioned anywhere in an SQL statement.

	Ex) When the shard key is not included in the student_no or in the query, the query is performed in the shard DB in which the shard key (student_no) is 123.

	.. code-block:: sql

		SELECT age FROM student WHERE name =? /*+ shard_val(123) */

	**shard_id Hint**

	Regardless of the shard key column value, the **shard_id** hint can be used when the user specifies a shard for query execution. The **shard_id** hint can be positioned anywhere in an SQL statement.

	Ex) When the query is performed in shard DB #3, queries students whose value of age is greater than 17 in the shard DB #3.

	.. code-block:: sql

		SELECT * FROM student WHERE age > 17 /*+ shard_id(3) */

.. _using-shard-hint:

	**General Procedure of Executing Queries by Using Shard SQL Hint**

	#. Executing Queries

		The following shows how a user-requested query is executed.

		.. image:: /images/image42.png

		*   An application makes a request for a query to the CUBRID SHARD through the JDBC interface. It adds the **shard_key** hint to the SQL statement to specify the shard DB from where the query will be executed.

		*   The SQL hint, like the example above, in the SQL statement, should be positioned in front of the bind variable or literal value of the column specified by the shard key.

		The shard SQL hint configured by the bind variable is as follows:

		.. image:: /images/image43.png

		The shard SQL hint specified in the literal value is as follows:

		.. image:: /images/image44.png

	#. Select a Shard DB to Analyze and Perform a Query

		Select a shard DB to analyze and perform the query by following the steps below:

		.. image:: /images/image45.png

		*   SQL queries received from users are rewritten in the format that is appropriate for internal processing.
		*   Select the shard DB that executed the query by using the SQL statement and hint requested by the user.

			*   When the SQL hint is set in the bind variable, select the shard DB which will execute the query by using the result of hashing the value of the shard_key bind variable and the configuration information.

			*   The hash function can be specified by the user. If not specified, the shard_key value is hashed by using the default hash function. Default hash functions are as follows:

			*   When the shard_key is an integer

				Default hash function (shard_key) = shard_key mod SHARD_KEY_MODULAR parameter (default value 256)

			*   When the shard_key is a string

				Default hash function (shard_key) = shard_key[0] mod  SHARD_KEY_MODULAR parameter (default value 256)

		.. note::

			When the shard_key bind variable value is 100, "Default hash function (shard_key) = 100 % 256 = 100." Therefore, the shard DB #1 (the hash result is 100) will be selected and then the user request will be sent to the selected shard DB #1.

	#. Return the Query Execution Result

		Return the query execution result as follows:

		.. image:: /images/image46.png

		*   Receives the query execution result from the shard DB #1 and then returns it to the requested application.
		
		
		.. note::
		
			[번역]
			배열로 여러 개의 값을 바인딩하여 일괄 처리하는 드라이버 함수(예: JDBC의 executeBatch, CCI의 cci_execute_array, cci_execute_batch)에서 다른 shard에 접근하는 값이 존재하면 오류 처리한다.

Various DBMSs Available
-----------------------

The CUBRID SHARD can be used on a variety of DBMSs such as CUBRID and MySQL.

	**CUBRID SHARD with CUBRID**

	The following image shows the structure of CUBRID SHARD when using three CUBRID SHARD DBs.

	.. image:: /images/image47.png

	**CUBRID SHARD with MySQL**

	The following image shows the structure of CUBRID SHARD when using three MySQL shard DBs.

	.. image:: /images/image48.png

.. note::

	It is impossible to use the different DBMSs on one CUBRID SHARD concurrently; if it is required, separate the CUBRID SHARD instances per DBMS.

Transaction Support
-------------------

	**Transaction Processing**

	The CUBRID SHARD executes an internal processing procedure to guarantee atomicity among ACID. For example, when an exception such as abnormal termination of an application occurs, the CUBRID SHARD sends a request to rollback to the shard DB which has been processing the request from the application in order to invalidate all changes in the transaction.

	The ACID, the characteristic of general transactions, is guaranteed, based on the characteristics and settings of the backend DBMS.

	**Constraints**

	2 Phase Commit (2PC) is unavailable; therefore, an error occurs when a query is executed by using several shard DBs in a single transaction.

Quick Start
===========

Configuration Example
---------------------

The CUBRID SHARD to be explained consists of four CUBRID SHARD DBs as shown below. The application uses the JDBC interface to process user requests.

	.. image:: /images/image49.png

	**Start after creating the shard DB and user account**

	As shown in the example above, after each shard DB node creates a shard DB and a user account, it starts the instance of the database.

	*   shard DB name: *shard1*
	*   shard DB user account: *shard*
	*   shard DB user password: *shard123*

	::

		sh> # Creating CUBRID SHARD DB
		sh> cubrid createdb *shard1*
		
		sh> # Creating CUBRID SHARD user account
		sh> csql -S -u dba shard1 -c "create user *shard* password '*shard123*'"
		
		sh> # Starting CUBRID SHARD DB
		sh> cubrid server start *shard1*

Changing the shard Configurations
---------------------------------

	**shard.conf**

	Change **shard.conf**, the default configuration file, as shown below:

	.. warning:: The port number and the shared memory identifier should be appropriately changed to the value which has not been assigned by the system.

	::

		[shard]
		MASTER_SHM_ID           =45501
		ADMIN_LOG_FILE          =log/broker/cubrid_broker.log
		 
		[%shard1]
		SERVICE                 =ON
		BROKER_PORT             =45511
		MIN_NUM_APPL_SERVER     =1  
		MAX_NUM_APPL_SERVER     =1  
		APPL_SERVER_SHM_ID      =45511
		LOG_DIR                 =log/broker/sql_log
		ERROR_LOG_DIR           =log/broker/error_log
		SQL_LOG                 =ON
		TIME_TO_KILL            =120
		SESSION_TIMEOUT         =300
		KEEP_CONNECTION         =ON
		MAX_PREPARED_STMT_COUNT =1024
		SHARD_DB_NAME           =shard1
		SHARD_DB_USER           =shard
		SHARD_DB_PASSWORD       =shard123
		MIN_NUM_PROXY           =1  
		MAX_NUM_PROXY           =1  
		PROXY_LOG_DIR           =log/broker/proxy_log
		PROXY_LOG               =ALL
		MAX_CLIENT              =10
		METADATA_SHM_ID         =45591
		SHARD_CONNECTION_FILE   =shard_connection.txt
		SHARD_KEY_FILE          =shard_key.txt

	For CUBRID, the server port number is not separately configured in the **shard_connection.txt** but the **cubrid_port_id** parameter of the **cubrid.conf** configuration file is used. Therefore, set the **cubrid_port_id** parameter of the **cubrid.conf** identical to the server. ::

		# TCP port id for the CUBRID programs (used by all clients).
		cubrid_port_id=41523

	**shard_key.txt**

	Set **shard_key.txt**, the shard DB mapping configuration file, for the shard key hash value as follows:

	*   [%shard_key]: Sets the shard key section
	*   Executing the query at shard #0 when the shard key hash result created by default hash function is between 0 and 63
	*   Executing the query at shard #1 when the shard key hash result created by default hash function is between 64 and 127
	*   Executing the query at shard #2 when the shard key hash result created by default hash function is between 128 and 191
	*   Executing the query at the shard #3 when the shard key hash result created by default hash function is between 192 and 255

	::

		[%shard_key]
		#min    max     shard_id
		0       63      0
		64      127     1
		128     191     2
		192     255     3

	**shard_connection.txt**

	Configure the **shard_connection.txt** file which is shard database configuration file, as follows:

	*   Real database name and connection information of shard #0
	*   Real database name and connection information of shard #1
	*   Real database name and connection information of shard #2
	*   Real database name and connection information of shard #3

	::

		# shard-id  real-db-name  connection-info
		#                         * cubrid : hostname, hostname, ...
		#                         * mysql  : hostname:port
		0           shard1        HostA
		1           shard1        HostB
		2           shard1        HostC
		3           shard1        HostD

Starting Service and Monitoring
-------------------------------

	**Starting CUBRID SHARD**

	Start the CUBRID SHARD as shown below: ::

		sh> cubrid shard start
		@ cubrid shard start
		++ cubrid shard start: success

	**Retrieving the CUBRID SHARD Status**

	Retrieve the CUBRID SHARD status as follows to check the parameter and the status of the process. ::

		sh> cubrid shard status
		@ cubrid shard status
		% shard1
		----------------------------------------------------------------
		PROXY_ID SHARD_ID   CAS_ID   PID   QPS   LQS PSIZE STATUS       
		----------------------------------------------------------------
			   1        0        1 21272     0     0 53292 IDLE         
			   1        1        1 21273     0     0 53292 IDLE         
			   1        2        1 21274     0     0 53292 IDLE         
			   1        3        1 21275     0     0 53292 IDLE
		 
		sh> cubrid shard status -f
		@ cubrid shard status
		% shard1
		----------------------------------------------------------------------------------------------------------------------------------------------------------
		PROXY_ID SHARD_ID   CAS_ID   PID   QPS   LQS PSIZE STATUS          LAST ACCESS TIME               DB             HOST   LAST CONNECT TIME    SQL_LOG_MODE
		----------------------------------------------------------------------------------------------------------------------------------------------------------
			   1        0        1 21272     0     0 53292 IDLE         2013/01/31 15:00:24    shard1@HostA           HostA 2013/01/31 15:00:25               -
			   1        1        1 21273     0     0 53292 IDLE         2013/01/31 15:00:24    shard1@HostB           HostB 2013/01/31 15:00:25               -
			   1        2        1 21274     0     0 53292 IDLE         2013/01/31 15:00:24    shard1@HostC           HostC 2013/01/31 15:00:25               -
			   1        3        1 21275     0     0 53292 IDLE         2013/01/31 15:00:24    shard1@HostD           HostD 2013/01/31 15:00:25               -

Writing a Sample
----------------

Check that the CUBRID SHARD operates normally by using a simple Java program.

	**Writing a Sample Table**

	Write a temporary table for the example in all shard DBs. ::

		sh> csql -C -u shard -p 'shard123' shard1@localhost -c "create table student (s_no int, s_name varchar, s_age int, primary key(s_no))"

	**Writing Code**

	The following example program is to enter student information from 0 to 1023 to the shard DB. Check the **shard.conf** modified in the previous procedure and then set the address/port information and the user information in the connection url.

	.. code-block:: java

		import java.sql.DriverManager;
		import java.sql.Connection;
		import java.sql.SQLException;
		import java.sql.Statement;
		import java.sql.ResultSet;
		import java.sql.ResultSetMetaData;
		import java.sql.PreparedStatement;
		import java.sql.Date;
		import java.sql.*;
		import cubrid.jdbc.driver.*;
		 
		public class TestInsert {
		 
				static  {
						try {
								Class.forName("cubrid.jdbc.driver.CUBRIDDriver");
						} catch (ClassNotFoundException e) {
								throw new RuntimeException(e);
						}
				}
		 
				public static void DoTest(int thread_id) throws SQLException {
						Connection connection = null;
		 
						try {
								connection = DriverManager.getConnection("jdbc:cubrid:localhost:45511:shard1:::?charSet=utf8", "shard", "shard123");
								connection.setAutoCommit(false);
		 
								for (int i=0; i < 1024; i++) {
										String query = "INSERT INTO student VALUES (/*+ shard_key */ ?, ?, ?)";
										PreparedStatement query_stmt = connection.prepareStatement(query);
		 
										String name="name_" + i;
										query_stmt.setInt(1, i);
										query_stmt.setString(2, name);
										query_stmt.setInt(3, (i%64)+10);
		 
										query_stmt.executeUpdate();
										System.out.print(".");
		 
										query_stmt.close();
										connection.commit();
								}
		 
								connection.close();
						} catch(SQLException e) {
								System.out.print("exception occurs : " + e.getErrorCode() + " - " + e.getMessage());
								System.out.println();
								connection.close();
						}
				}
		 
		 
				/**
				 * @param args
				 */
				public static void main(String[] args) {
						// TODO Auto-generated method stub
		 
						try {
								DoTest(1);
						} catch(Exception e){
								e.printStackTrace();
						}
				}
		}

	**Executing a Sample**

	Execute the sample program as follows: ::

		sh> javac -cp ".:$CUBRID/jdbc/cubrid_jdbc.jar" *.java
		sh> java -cp ".:$CUBRID/jdbc/cubrid_jdbc.jar" TestInsert

	**Checking the Result**

	Execute the query in each shard DB and check whether or not the partitioned information has been correctly entered.

	*   shard #0 ::

		sh> csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
		 
		         s_no  s_name                      s_age
		================================================
		            0  'name_0'                       10
		            1  'name_1'                       11
		            2  'name_2'                       12
		            3  'name_3'                       13
		            ...


	*   shard #1 ::

		sh> $ csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
		 
		         s_no  s_name                      s_age
		================================================
		           64  'name_64'                      10
		           65  'name_65'                      11
		           66  'name_66'                      12
		           67  'name_67'                      13  
		           ...

	*   shard #2 ::

		sh> $ csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
		 
		=== <Result of SELECT Command in Line 1> ===
		 
		          s_no  s_name                      s_age
		=================================================
		           128  'name_128'                     10
		           129  'name_129'                     11
		           130  'name_130'                     12
		           131  'name_131'                     13
		           ...

	*   shard #3 ::

		sh> $ csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
		 
		         s_no  s_name                      s_age
		================================================
		          192  'name_192'                     10
		          193  'name_193'                     11
		          194  'name_194'                     12
		          195  'name_195'                     13
		          ...

.. _shard-configuration:

Configuration and Setup
=======================

Configuration
-------------

The CUBRID SHARD is middleware, consisting of a shard broker, shard proxy, and shard CAS process as shown below.

.. image:: /images/image50.png

The **shard.conf** file is used for the default settings required for executing all processes in the CUBRID SHARD, and the configuration file is located in the **$CUBRID/conf** directory.

.. _default-shard-conf:

Default Configuration File, shard.conf
--------------------------------------

**shard.conf** is the default configuration file of the CUBRID SHARD, having a very similar format and content to **cubrid_broker.conf**, the configuration file of the existing CUBRID Broker/CAS.

**shard.conf** contains all the parameter settings as **cubrid_broker.conf** in an identical manner. This document describes the settings added to **shard.conf**. For more information on the :ref:`broker-configuration`.

+-------------------------------+----------+----------------------+--------------------+
| Parameter Name                | Type     | Default Value        | Dynamic Change     |
+===============================+==========+======================+====================+
| IGNORE_SHARD_HINT             | string   | OFF                  |                    |
+-------------------------------+----------+----------------------+--------------------+
| MIN_NUM_PROXY                 | int      | 1                    |                    |
+-------------------------------+----------+----------------------+--------------------+
| MAX_NUM_PROXY                 | int      | 1                    |                    |
+-------------------------------+----------+----------------------+--------------------+
| PROXY_LOG                     | string   | ERROR                | available          |
+-------------------------------+----------+----------------------+--------------------+
| PROXY_LOG_DIR                 | string   | log/broker/proxy_log |                    |
+-------------------------------+----------+----------------------+--------------------+
| PROXY_LOG_MAX_SIZE            | int      | 100000               | available          |
+-------------------------------+----------+----------------------+--------------------+
| PROXY_MAX_PREPARED_STMT_COUNT | int      | 2000                 |                    |
+-------------------------------+----------+----------------------+--------------------+
| PROXY_TIMEOUT                 | int      | 30(seconds)          |                    |
+-------------------------------+----------+----------------------+--------------------+
| MAX_CLIENT                    | int      | 10                   |                    |
+-------------------------------+----------+----------------------+--------------------+
| METADATA_SHM_ID               | int      | -                    |                    |
+-------------------------------+----------+----------------------+--------------------+
| SHARD_CONNECTION_FILE         | string   | shard_connection.txt |                    |
+-------------------------------+----------+----------------------+--------------------+
| SHARD_DB_NAME                 | string   | -                    | available          |
+-------------------------------+----------+----------------------+--------------------+
| SHARD_DB_USER                 | string   | -                    | available          |
+-------------------------------+----------+----------------------+--------------------+
| SHARD_DB_PASSWORD             | string   | -                    | available          |
+-------------------------------+----------+----------------------+--------------------+
| SHARD_KEY_FILE                | string   | shard_key.txt        |                    |
+-------------------------------+----------+----------------------+--------------------+
| SHARD_KEY_MODULAR             | int      | 256                  |                    |
+-------------------------------+----------+----------------------+--------------------+
| SHARD_KEY_LIBRARY_NAME        | string   | -                    |                    |
+-------------------------------+----------+----------------------+--------------------+
| SHARD_KEY_FUNCTION_NAME       | string   | -                    |                    |
+-------------------------------+----------+----------------------+--------------------+

*   **SHARD_DB_NAME** : The name of the shard DB, used to verify the request for connection from an application.
*   **SHARD_DB_USER** : The name of the backend shard DB user, used to connect to the backend DBMS for the shard CAS process as well as to verify the request for connection from an application. User names on all shard DBs should be identical.
*   **SHARD_DB_PASSWORD** : The user password of the backend shard DB, used to connect to the backend DBMS for the shard CAS process as well as to verify the request for connection from an application. Passwords of all shard DBs should be identical.

*   **MIN_NUM_PROXY** : The minimum number of shard proxy processes.
*   **MAX_NUM_PROXY** : The maximum number of shard proxy processes.
*   **PROXY_LOG_DIR** : The directory path where the shard proxy logs will be saved.
*   **PROXY_LOG** : The shard proxy log level. It can be set to one of the following values:

    *   **ALL** : All logs
    *   **ON** : All logs
    *   **SHARD** : Logs for selecting and processing shard DBs.
    *   **SCHEDULE** : Logs for scheduling tasks.
    *   **NOTICE** : Logs for key notices.
    *   **TIMEOUT** : Logs for timeouts.
    *   **ERROR** : Logs for errors.
    *   **NONE** : No log is recorded.
    *   **OFF** : No log is recorded.

*   **PROXY_LOG_MAX_SIZE** : The maximum size of the shard proxy log file in KB. The maximum value is 1,000,000.

.. _proxy-max-prepared-stmt-count:

*   **PROXY_MAX_PREPARED_STMT_COUNT** : The maximum size of statement pool managed by shard proxy
*   **PROXY_TIMEOUT** : The maximum waiting time by which the statement is prepared or shard(cas) is available to use. The default value is 30(seconds). If this value is 0, the waiting time is decided by the value of the query_timeout system paramater; if the value of query_timeout is also 0, the waiting time is infinite. IF the value PROXY_TIMEOUT is larger than 0, the maximum value between query_timeout and PROXY_TIMEOUT decides the waiting time.
*   **MAX_CLIENT** : The number of applications that can be concurrently connected by using the shard proxy.
*   **METADATA_SHM_ID** : Shared memory identifier of the shard metadata storage.

*   **SHARD_CONNECTION_FILE** : The path of the shard connection configuration file. The shard connection configuration file should be located in **$CUBRID/conf**. For more information, see the :ref:`shard connection configuration file <shard-connection-configuration-file>`.

*   **SHARD_KEY_FILE** : The path of the shard key configuration file. The shard key configuration file should be located in **$CUBRID/conf**. For more information, see the :ref:`shard key configuration file <shard-key-configuration-file>`.

*   **SHARD_KEY_MODULAR** : The parameter to specify the range of results of the default shard key hash function. The result of the function is shard_key(integer) % SHARD_KEY_MODULAR. For related issues, see :ref:`shard key configuration file <shard-key-configuration-file>` and :ref:`setting-user-defined-hash-function`.

*   **SHARD_KEY_LIBRARY_NAME** : Specify the library path loadable at runtime to specify the user hash function for the shard key. If the **SHARD_KEY_LIBRARY_NAME** parameter is set, the **SHARD_KEY_FUNCTION_NAME** parameter should also be set. For more information, see :ref:`setting-user-defined-hash-function`.

*   **SHARD_KEY_FUNCTION_NAME** : The parameter to specify the name of the user hash function for shard key. For more information, see :ref:`setting-user-defined-hash-function`.

*   **IGNORE_SHARD_HINT** : When this value is **ON**, the hint provided to connect to a specific shard is ignored and the database to connect is selected based on the defined rule. The default value is **OFF**. It can be used to balance the read load while all databases are copied with the same data. For example, to give the load of an application to only one node among several replication nodes, the shard proxy automatically determines the node (database) with one connection to a specific shard.

Setting Shard Metadata
----------------------

In addition to **shard.conf**, the CUBRID SHARD has a configuration file for shard key and the shard connection configuration file for connection with the shard DB.

.. _shard-connection-configuration-file:

	**Shard Connection Configuration File (SHARD_CONNECTION_FILE)**

	To connect to the backend shard DB, the CUBRID SHARD loads the shard connection configuration file specified in the **SHARD_CONNECTION_FILE** parameter of **shard.conf**, the default configuration file. If **SHARD_CONNECTION_FILE** is not specified in **shard.conf**, it loads the **shard_connection.txt** file by default.

	**Format**

		The basic example and format of a shard connection configuration file are as follows: ::

			#
			# shard-id      real-db-name    connection-info
			#                               * cubrid : hostname, hostname, ...
			#                               * mysql  : hostname:port
			 
			# CUBRID
			0               shard1          HostA  
			1               shard1          HostB
			2               shard1          HostC
			3               shard1          HostD
			 
			# mysql
			#0              shard1         HostA:3306
			#1              shard1         HostB:3306
			#2              shard1         HostC:3306
			#3              shard1         HostD:3306

		.. note:: As shown in the general CUBRID settings, the content after # is converted to comment.

	**CUBRID**

	When the backend shard DB is CUBRID, the format of the connection configuration file is as follows: ::

		# CUBRID
		# shard-id      real-db-name            connection-info
		# shard identifier( >0 )        The real name of backend shard DB    host name

		0           shard_db_1          host1
		1           shard_db_2          host2
		2           shard_db_3          host3
		3           shard_db_4          host4

	For CUBRID, a separate backend shard DB port number is not specified in the above configuration file, but the **CUBRID_PORT_ID** parameter in the **cubrid.conf** file (the default configuration file of CUBRID) is used. The **cubrid.conf** file is by default located in the **$CUBRID/conf**. ::

		$ vi cubrid.conf

		...

		# TCP port id for the CUBRID programs (used by all clients).
		cubrid_port_id=41523

	**MySQL**

	When the backend shard DB is MySQL, the format of the connection configuration file is as follows: ::

		# mysql
		# shard-id      real-db-name            connection-info
		# shard identifier (>0 )        Actual name of each backend shard DB    Host name: port number

		0           shard_db_1          host1:1234
		1           shard_db_2          host2:1234
		2           shard_db_3          host3:1234
		3           shard_db_4          host4:1234

.. _shard-key-configuration-file:

	**Configuration File for Shard Key (SHARD_KEY_FILE)**

	The CUBRID SHARD loads the shard key configuration file specified in the **SHARD_KEY_FILE** parameter of **shard.conf**, the default configuration file, to determine which backend shard DB should process the user requests.

	If **SHARD_KEY_FILE** is not specified in **shard.conf**, it loads the **shard_key.txt** file by default.

	**Format**

	The example and format of a shard key configuration file are as follows: ::

		[%student_no]
		#min    max     shard_id
		0       31      0   
		32      63      1   
		64      95      2   
		96      127     3   
		128     159     0
		160     191     1
		192     223     2
		224     255     3
		 
		#[%another_key_column]
		#min    max     shard_id
		#0      127     0   
		#128    255     1

	*   [%shard_key_name]: Specifies the name of the shard key.
	*   min: The minimum value range of the shard key hash results.
	*   max: The maximum range of the shard key hash results.
	*   shard_id: The shard identifier

	.. note:: As shown in the general CUBRID settings, the content after # is converted to comment.

.. warning::

	*   min of the shard key should always start from 0.
	*   max should be up to 255.
	*   No empty value between min and max is allowed.
	*   The default hash function should not exceed the value of the **SHARD_KEY_MODULAR** parameter.
	*   The result of shard key hashing should be within a range from 0 to (**SHARD_KEY_MODULAR** -1).

.. _setting-user-defined-hash-function:

Setting User-Defined Hash Function
----------------------------------

To select a shard that will perform queries, the CUBRID SHARD uses the results of hashing the shard key and the metadata configuration information. For this, users can use the default hash function or define a hash function.

	**Default Hash Function**

	When the **SHARD_KEY_LIBRARY_NAME** and **SHARD_KEY_FUNCTION_NAME** parameters of **shard.conf** are not set, the shard key is hashed by using the default hash function. The default hash function is as follows:

	*   When the shard_key is an integer

		Default hash function (shard_key) = shard_key mod SHARD_KEY_MODULAR parameter (default value: 256)

	*   When the shard_key is a string

		Default hash function (shard_key) = shard_key[0] mod SHARD_KEY_MODULAR parameter (default value: 256)

	**Setting User-Defined Hash Function**

	The CUBRID SHARD can hash the shard key by using the user-defined hash function, in addition to the default hash function.

	**Implementing and Creating a Library**

	The user-defined hash function must be implemented as a **.so** library loadable at runtime. Its prototype is as shown below:

	.. code-block:: c

		94 /*
		95    return value :
		96         success - shard key id(>0)
		97         fail    - invalid argument(ERROR_ON_ARGUMENT), shard key id make fail(ERROR_ON_MAKE_SHARD_KEY)
		98    type         : shard key value type
		99    val          : shard key value
		100 */
		101 typedef int (*FN_GET_SHARD_KEY) (const char *shard_key, T_SHARD_U_TYPE type,
		102                                    const void *val, int val_size);

	*   The return value of the hash function should be within the range of the hash results of the **shard_key.txt** configuration file.
	*   To build a library, the **$CUBRID/include/shard_key.h** file of the CUBRID source must be included. The file lets you know the details such as error code that can be returned.

	**Changing the shard.conf Configuration File**

	To apply a user-defined hash function, the **SHARD_KEY_LIBRARY_NAME** and **SHARD_KEY_FUNCTION_NAME** parameters of **shard.conf** should be set according to the implementation.

	*   **SHARD_KEY_LIBRARY_NAME** : The (absolute) path of the user-defined hash library.
	*   **SHARD_KEY_FUNCTION_NAME** : The name of the user-defined hash function.

	**Example**

	The following example shows how to use a user-defined hash.

	First, check the **shard_key.txt** configuration file. ::

		[%student_no]
		#min    max     shard_id
		0       31      0   
		32      63      1   
		64      95      2   
		96      127     3   
		128     159     0
		160     191     1
		192     223     2
		224     255     3

	To set the user-defined hash function, implement a **.so** shared library that is loadable at runtime. The result of the hash function should be within the range of hash function results defined in the **shard_key.txt** configuration file. The following example shows a simple implementation.

	*   When the shard_key is an integer

		*   Select shard #0 when the shard_key is an odd number
		*   Select shard #1 when the shard_key is an even number

	*   When the shard_key is a string

		*   Select shard #0 when the shard_key string starts with 'a' or 'A'.
		*   Select shard #1 when the shard_key string starts with 'b' or 'B'.
		*   Select shard #2 when the shard_key string starts with 'c' or 'C'.
		*   Select shard #3 when the shard_key string starts with 'd' or 'D'.

	.. code-block:: c
		
		// <shard_key_udf.c>
		 
		1 #include <string.h>
		2 #include <stdio.h>
		3 #include <unistd.h>
		4 #include "shard_key.h"
		5
		6 int
		7 fn_shard_key_udf (const char *shard_key, T_SHARD_U_TYPE type,
		8                   const void *value, int value_len)
		9 {
		10   unsigned int ival;
		11   unsigned char c;
		12
		13   if (value == NULL)
		14     {
		15       return ERROR_ON_ARGUMENT;
		16     }
		17
		18   switch (type)
		19     {
		20     case SHARD_U_TYPE_INT:
		21       ival = (unsigned int) (*(unsigned int *) value);
		22       if (ival % 2)
		23         {
		24           return 32;            // shard #1
		25         }
		26       else
		27         {
		28           return 0;             // shard #0
		29         }
		30       break;
		31
		32     case SHARD_U_TYPE_STRING:
		33       c = (unsigned char) (((unsigned char *) value)[0]);
		34       switch (c)
		36         case 'a':
		37         case 'A':
		38           return 0;             // shard #0
		39         case 'b':
		40         case 'B':
		41           return 32;            // shard #1
		42         case 'c':
		43         case 'C':
		44           return 64;            // shard #2
		45         case 'd':
		46         case 'D':
		47           return 96;            // shard #3
		48         default:
		49           return ERROR_ON_ARGUMENT;
		50         }
		51
		52       break;
		53
		54     default:
		55       return ERROR_ON_ARGUMENT;
		56     }
		57   return ERROR_ON_MAKE_SHARD_KEY;
		58 }

	Build the user-defined function as a shared library. The following example is Makefile for building a hash function. ::

		# Makefile
		 
		CC = gcc
		LIBS = $(LIB_FLAG)
		CFLAGS = $(CFLAGS_COMMON) -fPIC -I$(CUBRID)/include –I$(CUBRID_SRC)/src/broker
		 
		SHARD_CC = gcc -g -shared -Wl,-soname,shard_key_udf.so
		SHARD_KEY_UDF_OBJS = shard_key_udf.o
		 
		all:$(SHARD_KEY_UDF_OBJS)
				$(SHARD_CC) $(CFLAGS) -o shard_key_udf.so $(SHARD_KEY_UDF_OBJS) $(LIBS)
		 
		clean:
				-rm -f *.o core shard_key_udf.so

	To include the user-defined hash function, modify the **SHARD_KEY_LIBRARY_NAME** and **SHARD_KEY_FUNCTION_NAME** parameters as shown in the above implementation. ::

		[%student_no]
		SHARD_KEY_LIBRARY_NAME =$CUBRID/conf/shard_key_udf.so
		SHARD_KEY_FUNCTION_NAME =fn_shard_key_udf

Running and Monitoring
======================

By using the CUBRID SHARD utility, CUBRID SHARD can be started or stopped and various status information can be retrieved.

Starting CUBRID SHARD
---------------------

	To start the CUBRID SHARD, enter the following: ::

		% cubrid shard start
		@ cubrid shard start
		++ cubrid shard start: success

	If the CUBRID SHARD has already been started, the following message will appear: ::

		% cubrid shard start
		@ cubrid shard start
		++ cubrid shard is running.

	While executing **cubrid shard start**, the information of the CUBRID SHARD configuration file (**shard.conf**) are read to start all components of the configuration. All metadata DBs and shard DBs should be started before starting the CUBRID SHARD because it accesses them.

	CUBRID SHARD cannot be started even if one thing like DB connection is failed among all configured items; you can find the cause of failure through the SHARD error logs written on the $CUBRID/log/broker/ directory.

Stopping CUBRID SHARD
---------------------

	Enter the following to stop the CUBRID SHARD. ::

		% cubrid shard stop
		@ cubrid shard stop
		++ cubrid shard stop: success

	If the CUBRID SHARD has already been stopped, the following message will appear: ::

		$ cubrid shard stop
		@ cubrid shard stop
		++ cubrid shard is not running.

Dynamic change of CUBRID SHARD parameters
-----------------------------------------

	**Description**

	You can configure the parameters related to running CUBRID SHARD in the environment configuration file (**shard.conf**). Additionally, you can some CUBRID SHARD parameters while it is running by using the **shard_broker_changer** utility. For details about configuration of CUBRID SHARD parameters and dynamically changeable parameters see :ref:`shard-configuration`.

	**Syntax**

	The **shard_broker_changer** syntax used to change parameter while CUBRID SHARD is running is as follows: Enter the name of CUBRID SHARD running in *shard-name* and enter dynamically changeable parameters in *parameter*. *value* must be specified based on the parameter to be modified. You can apply changes in a specific CUBRID SHARD by specifying an identifier of CUBRID SHARD. *proxy-number* represents PROXY-ID displayed in the **cubrid shard status** command. ::

		shard_broker_changer shard-name [proxy-number] parameter value

	**Example**

	Even though SQL logs are recorded in CUBRID SHARD which is running, you need to enter as follows to configure the **SQL_LOG** parameter to ON so that SQL logs are recorded in CUBRID SHARD running. Such dynamic parameter change is effective only while CUBRID SHARD is running. ::

		% shard_broker_changer shard1 sql_log on
		OK

Checking CUBRID SHARD configuration information
-----------------------------------------------

**cubrid shard info** dumps the currently "working" shard parameters' configuration information(cubrid_shard.conf). Shard parameters' information can be dynamically changed by **shard_broker_changer** command; with **cubrid shard info** command, you can see the configuration information of the working shard. ::

	% cubrid shard info

As a reference, to see the configuration information of the currently "working" system(cubrid.conf), use **cubrid paramdump** *database_name* command. By **SET SYSTEM PARAMETERS** syntax, the configuration information of the system parameters can be changed dynamically; with **cubrid broker info** command, you can see the configuration information of the system parameters.

Checking CUBRID SHARD ID
------------------------
**cubrid shard getid** prints SHARD ID to know in what DB a specific key is included. :: 

	cubrid shard getid -b <broker-name> [-f] shard-key
	
* -b <*broker-name*>: shard broker name
* -f: prints detail information
* *shard-key* : shard key

The following shows how to print the SHARD ID for the key 1 within the shard1 shard broker.

::

	$ cubrid shard getid -b shard1 1
	@ cubrid shard getid
	% shard1
	 SHARD_ID : 0, SHARD_KEY: 1

The following shows how to print the detail information using the **-f** option.

::
	
	$ cubrid shard getid -b shard1 -f 1
	@ cubrid shard getid
	% shard1
	 SHARD_ID : 0, SHARD_KEY : 1, KEY_COLUMN : student_no
	 MODULAR : 256, LIBRARY_NAME : NOT DEFINED, FUNCTION_NAME : NOT DEFINED
	 RANGE STATISTICS : student_no
	      MIN ~   MAX :      SHARD
	    ---------------------------
	        0 ~    31 :          0

	 SHARD CONNECTION :
	    SHARD_ID          DB NAME          CONNECTION_INFO
	    ---------------------------------------------------
	           0           shard1                192.168.10.1
	           1           shard4                192.168.10.2
	           2           shard2                192.168.10.3
	           3           shard3                192.168.10.4

Checking CUBRID SHARD status Information
----------------------------------------

	**cubrid shard status**

	provides a variety of options to check the status information of each shard broker, shard proxy, and shard cas. In addition, it is possible to check the metadata information and the information on the client who has accessed the shard proxy. ::

		cubrid shard status [options] [<expr>]
		options : -b | -f [-l sec] | -t | -c | -m | -s <sec>

	When <*expr*> is given, the status monitoring is performed for the corresponding CUBRID SHARD. When it is omitted, status monitoring is performed for all CUBRID SHARDs registered to the CUBRID SHARD configuration file (**shard.conf**).

	**Options**

	The following table shows options that can be used together with cubrid broker status.

	+------------+-------------------------------------------------------------------------------------------------------------------------+
	| Option     | Description                                                                                                             |
	+============+=========================================================================================================================+
	| <          | Displays the status information for the CUBRID SHARD whose name includes <                                              |
	| *expr*     | *expr*                                                                                                                  |
	| >          | >. If the name is not specified, displays the status information for all CUBRID SHARDs.                                 |
	+------------+-------------------------------------------------------------------------------------------------------------------------+
	| **-b**     | Displays the status information for the CUBRID broker excluding the information on the CUBRID proxy or the CUBRID CAS.  |
	+------------+-------------------------------------------------------------------------------------------------------------------------+
	| **-c**     | Displays the information on the client which has accessed the CUBRID proxy.                                             |
	+------------+-------------------------------------------------------------------------------------------------------------------------+
	| **-m**     | Displays the metadata information.                                                                                      |
	+------------+-------------------------------------------------------------------------------------------------------------------------+
	| **-t**     | Displays in tty mode. The output content can be redirected to a file.                                                   |
	+------------+-------------------------------------------------------------------------------------------------------------------------+
	| **-f**     | Displays more detailed information on the CUBRID SHARD.                                                                 |
	| [          |                                                                                                                         |
	| **-l**     |                                                                                                                         |
	| *secs*     |                                                                                                                         |
	| ]          |                                                                                                                         |
	+------------+-------------------------------------------------------------------------------------------------------------------------+
	| **-s**     | Periodically displays the status information for the CUBRID SHARD at a specified time. Returns to the command prompt if |
	| *secs*     | **q**                                                                                                                   |
	|            | is entered.                                                                                                             |
	+------------+-------------------------------------------------------------------------------------------------------------------------+

	**Example**

	If no options or parameters are given to check the status of all CUBRID SHARDs, the following will be displayed as a result: ::

		$ cubrid shard status
		@ cubrid shard status
		% test_shard  - shard_cas [2576,45000] /home/CUBRID/log/broker/test_shard.err
		 JOB QUEUE:0, AUTO_ADD_APPL_SERVER:ON, SQL_LOG_MODE:ALL:100000
		 LONG_TRANSACTION_TIME:60.00, LONG_QUERY_TIME:60.00, SESSION_TIMEOUT:10
		 KEEP_CONNECTION:AUTO, ACCESS_MODE:RW
		----------------------------------------------------------------
		PROXY_ID SHARD_ID   CAS_ID   PID   QPS   LQS PSIZE STATUS
		----------------------------------------------------------------
			   1        1        1  2580     100     3 55968 IDLE
			   1        2        1  2581     200     4 55968 IDLE

	*   % test_shard: The proxy name
	*   shard_cas: The application server format [shard_cas | shard_cas_myqsl]
	*   [2576, 45000]: The proxy process ID and the proxy access port number
	*   /home/CUBRID/log/broker/test_shard.err: The error log file of test_shard
	*   JOB QUEUE: The number of standing by jobs in the job queue

	*   SQL_LOG_MODE: The **SQL_LOG** parameter value of the **shard.conf** file has been set to **ALL** in order to log in all SQL.
	*   SLOW_LOG: The **SLOW_LOG** parameter value of the **shard.conf** file has been set to **ON** in order to log the query where any long-duration execution query or any error has occurred to the SLOW SQL LOG file.

	*   LONG_TRANSACTION_TIME: The execution time of a transaction to be considered as a long-duration transaction. When the execution time of a transaction exceeds 60 seconds, it is considered as a long-duration transaction.

	*   LONG_QUERY_TIME: The execution time of a query to be considered as a long-duration query. When the execution time of a query exceeds 60 seconds, it is considered as a long-duration query.

	*   SESSION_TIMEOUT: The timeout value to terminate a CAS session that has made no requests without any commit or rollback after starting the transaction. When this time is expired in this status, the connection between the application client and the application server (CAS) is terminated. The **SESSION_TIMEOUT** parameter value of the **shard.conf** is 300 (secs).

	*   ACCESS_MODE: The shard broker operation mode. The RW mode allows modification of the database as well as retrieval.
	*   PROXY_ID: The serial number of a proxy which has been sequentially given in the shard broker
	*   SHARD_ID: The serial number of a shard DB set in the proxy
	*   CAS_ID: The serial number of an application server (CAS) which accesses the shard DB
	*   PID: The ID of an application server (CAS) process which accesses the shard DB
	*   QPS: The number of queries processed per second
	*   LQS: The number of long-duration queries processed per second
	*   PSIZE: The size of the application server process
	*   STATUS: The current status of the application server, such as BUSY/IDLE/CLIENT_WAIT/CLOSE_WAIT/CON_WAIT.

	To check the status of the shard broker, enter the following: ::

		$ cubrid shard status -b
		@ cubrid shard status
		  NAME           PID  PORT  Active-P  Active-C      REQ  TPS  QPS  K-QPS NK-QPS    LONG-T    LONG-Q  ERR-Q
		==========================================================================================================
		* test_shard    3548 45000         1         2        0    0    0      0      0    0/60.0    0/60.0      0

	*   NAME: The proxy name
	*   PID: The process ID of the proxy
	*   PORT: The proxy port number
	*   Active-P: The number of proxy
	*   Active-C: The number of application servers (CASs)
	*   REQ: The number of client requests processed by the proxy
	*   TPS: The number of transactions processed per second (calculated only when the option is **-b -s** <*sec*>)
	*   QPS: The number of queries processed per second (calculated only when the option is **-b -s** <*sec*>)
	*   K-QPS: QPS for the queries which include a shard key
	*   NK-QPS: QPS for the queries which do not include a shard key
	*   LONG-T: The number of transactions that exceed the **LONG_TRANSACTION_TIME** time / **LONG_TRANSACTION_TIME** parameter value
	*   LONG-Q: The number of queries that exceeds the **LONG_QUERY_TIME** time / **LONG_QUERY_TIME** parameter value
	*   ERR-Q: The number of queries where errors have occurred

	To check details on the status of the shard broker, enter as follows: ::

		$ cubrid shard status -b -f
		@ cubrid shard status
		NAME           PID  PSIZE  PORT  Active-P  Active-C      REQ  TPS  QPS  K-QPS (H-KEY   H-ID H-ALL) NK-QPS    LONG-T    LONG-Q  ERR-Q  CANCELED  ACCESS_MODE  SQL_LOG
		======================================================================================================================================================================
		* test_shard 3548 100644 45000         1         2        0    0    0      0      0      0      0      0    0/60.0    0/60.0      0         0           RW      ALL

	*   NAME: The proxy name
	*   PID: The process ID of the proxy
	*   PSIZE: The process size of the proxy
	*   PORT: The proxy port number
	*   Active-P: The number of proxies
	*   Active-C: The number of application servers (CASs)
	*   REQ: The number of client requests processed by the proxy
	*   TPS: The number of transactions processed per second (calculated only when the option is **-b -s** <*sec*>)
	*   QPS: The number of queries processed per second (calculated only when the option is **-b -s** <*sec*>)
	*   K-QPS: QPS for the queries which include a shard key
	*   H-KEY: QPS for the queries which include the shard_key hint
	*   H-ID: QPS for the queries which include the shard_id hint
	*   H-ALL: QPS for the queries which include the shard_all hint
	*   NK-QPS: QPS for the queries which do not include a shard key
	*   LONG-T: The number of transactions that exceeds the **LONG_TRANSACTION_TIME** time / **LONG_TRANSACTION_TIME** parameter value
	*   LONG-Q: The number of queries that exceeds the **LONG_QUERY_TIME** time / **LONG_QUERY_TIME** parameter value
	*   ERR-Q: The number of queries where errors have occurred
	*   CANCELED: The number of queries which have been canceled due to user interruption after the shard broker had been started (the number accumulations for *N* seconds in case of using with the **-l** *N* option)
	*   ACCESS_MODE: The shard broker operation mode. The RW mode allows modification of the database as well as retrieval.
	*   SQL_LOG: The **SQL_LOG** parameter value of the **shard.conf** file is ALL in order to leave the SQL log.

	By using the **-s** option, enter the monitoring interval of the shard broker which includes test_shard, and then enter the following to monitor the shard broker status periodically. If test_shard is not entered as a parameter,the status monitoring is periodically made for all shard brokers. If **q** is entered, the monitoring screen returns to the command prompt. ::

		$ cubrid shard status -b test_shard -s 1 -t
		@ cubrid shard status
		  NAME           PID  PORT  Active-P  Active-C      REQ  TPS  QPS  K-QPS NK-QPS    LONG-T    LONG-Q  ERR-Q
		==========================================================================================================
		* test_shard    3548 45000         1         2        0    0    0      0      0    0/60.0    0/60.0      0

	Output TPS and QPS information to a file by using the **-t** option. To stop the output as a file, press <Ctrl+C> to stop the program. ::

		% cubrid shard status -b -s 1 -t  > log_file

	Output the metadata information by using the **-m** option. For details on the parameter of **shard.conf**, see :ref:`default-shard-conf`. ::

		$ cubrid shard status -m
		@ cubrid shard status
		% test_shard [299009]
		MODULAR : 256, LIBRARY_NAME : NOT DEFINED, FUNCTION_NAME : NOT DEFINED
		SHARD STATISTICS
				   ID  NUM-KEY-Q  NUM-ID-Q   NUM-NO-HINT-Q       SUM
				-----------------------------------------------------
					0          0         0               0         0
					1          0         0               0         0
					2          0         0               0         0
					3          0         0               0         0

	*   test_shard: The proxy name
	*   [299009]: The decimal value of the **METADATA_SHM_ID** parameter of **shard.conf**
	*   MODULAR: The **SHARD_KEY_MODULR** parameter value of **shard.conf**
	*   LIBRARY_NAME: The **SHARD_KEY_LIBRARY_NAME** parameter value of **shard.conf**
	*   FUNCTION_NAME: The **SHARD_KEY_FUNCTION_NAME** parameter value of **shard.conf**
	*   SHARD STATISTICS: The shard ID query information

		*   ID: The shard DB serial number (shard ID)
		*   NUM-KEY-Q: The number of query requests which include the shard key
		*   NUM-ID-Q: The number of query requests which include the shard ID
		*   NUM-NO-HINT-Q: The number of requests handled by load balancing without hint when **IGNORE_SHARD_HINT** is configured
		*   SUM: NUM-KEY-Q + NUM-ID-Q

	Use the **-m -f** option to display more detailed metadata information. For details on the parameter of **shard.conf**, see :ref:`default-shard-conf`. ::

		$ cubrid shard status –m -f
		@ cubrid shard status
		% test_shard [299009]
		MODULAR : 256, LIBRARY_NAME : NOT DEFINED, FUNCTION_NAME : NOT DEFINED
		SHARD : 0 [HostA] [shard1], 1 [HostB] [shard1], 2 [HostC] [shard1], 3 [HostD] [shard1]
		SHARD STATISTICS
				   ID  NUM-KEY-Q  NUM-ID-Q   NUM-NO-HINT-Q       SUM
				-----------------------------------------------------
					0          0         0               0         0
					1          0         0               0         0
					2          0         0               0         0
					3          0         0               0         0
		 
		RANGE STATISTICS : user_no
				  MIN ~   MAX :      SHARD     NUM-Q
				------------------------------------
					0 ~    31 :          0         0
				   32 ~    63 :          1         0
				   64 ~    95 :          2         0
				   96 ~   127 :          3         0
				  128 ~   159 :          0         0
				  160 ~   191 :          1         0
				  192 ~   223 :          2         0
				  224 ~   255 :          3         0
		DB Alias : shard1 [USER : shard, PASSWD : shard123]

	*   test_shard: The proxy name
	*   [299009]: The decimal value of the **METADATA_SHM_ID** parameter of **shard.conf**
	*   MODULAR: The **SHARD_KEY_MODULR** parameter value of **shard.conf**
	*   LIBRARY_NAME: The **SHARD_KEY_LIBRARY_NAME** parameter value of **shard.conf**
	*   FUNCTION_NAME: The **SHARD_KEY_FUNCTION_NAME** parameter value of **shard.conf**
	*   SHARD: The shard DB information in the proxy

		*   0: The shard DB serial number (shard ID)
		*   [HostA]: The shard access information
		*   [shard1]: The actual DB name

	*   ID: The shard DB serial number (shard ID)
	*   NUM-KEY-Q: The number of query requests which include a shard key
	*   NUM-ID-Q: The number of query requests which include a shard ID
	*   SUM: NUM-KEY-Q + NUM-ID-Q
	*   RANGE STATISTICS: The shard key query information

		*   user_no: The shard key name
		*   MIN: The minimum range of a shard key
		*   MAX: The maximum range of a shard key
		*   SHARD: The shard DB serial number (shard ID)
		*   NUM-Q: The number of query requests which include the shard key

	Displays the information on the client that has accessed the shard proxy by using the **-c** option. ::

		$ cubrid shard status -c
		@ cubrid shard status
		% test_shard(0), MAX-CLIENT : 10000
		------------------------------------------------------------------------------------------------
		 CLIENT-ID           CLIENT-IP             CONN-TIME            L-REQ-TIME            L-RES-TIME
		------------------------------------------------------------------------------------------------
				 0         10.24.18.68   2011/12/15 16:33:31   2011/12/15 16:33:31   2011/12/15 16:33:31

	*   CLIENT-ID: The client serial number sequentially given in the proxy
	*   CLIENT-IP: The client IP address
	*   CONN-TIME: The time that the proxy has been accessed
	*   L-REQ-TIME: The time at which the last request had been made to the proxy
	*   L-RES-TIME: The time at which the last response has been received from the proxy

Limit shard proxy access
------------------------

To limit the applications to access shard proxy, the **ACCESS_CONTROL** of the **cubrid_shard.conf** should set to ON and enter a file name where the list of users, databases, and IPs of which access is access is permitted ot the **ACCESS_CONTROL_FILE** parameter is stored. The default value of **ACCESS_CONTROL** parameter is OFF.

The **ACCESS_CONTROL** and **ACCESS_CONTROL_FILE** parameters should be written under [shard] which are located in common parameters.

The format of **ACCESS_CONTROL_FILE** is as follows: ::

	[%<shard_name>]
	<db_name>:<db_user>:<ip_list_file>

	...

*   <*shard_name*>: Shard proxy name. It is one of shared proxies specified by **cubrid_broker.conf**.
*   <*db_name*>: Database name. If it is specified as \*, every database can be permitted.
*   <*db_user*>: The user ID of the database. If it is specified as \*, the user ID of every database is permitted.
*   <*ip_list_file*>: The file name where the list of IPs accessable is stored. You can use a comman to separate each file such as ip_list_file1, ip_list_file2, ….

You can additionally specify [%<*broker_name*>] and <*db_name*>:<*db_user*>:<*ip_list_file*> for each shard proxy and separate line can be added for the same <*db_name*> and <*db_user*>.

The format of writing ip_list_file is as follows: ::

	<ip_addr>

	...

*   <*ip_addr*>: The name of IP of which access is permitted. If * is enterd in back part, it means every IP is permitted.

While the value of **ACCESS_CONTROL** is ON and **ACCESS_CONTROL_FILE** is not specified, shard proxy allows access request from localhost. When running shard proxy and if it analysis of **ACCESS_CONTROL_FILE** and ip_list_file is faled, shard proxy allows access request only from localhost.

When running shard proxy and if it analysis of **ACCESS_CONTROL_FILE** and ip_list_file is faled, shard proxy does not run. ::

	# cubrid_broker.conf
	[broker]
	MASTER_SHM_ID           =30001
	ADMIN_LOG_FILE          =log/broker/cubrid_broker.log
	ACCESS_CONTROL   =ON
	ACCESS_CONTROL_FILE     =/home1/cubrid/access_file.txt
	[%QUERY_EDITOR]
	SERVICE                 =ON
	BROKER_PORT             =30000
	......

The following is an example of **ACCESS_CONTROL_FILE**. * means everything; it can be used when you specifying the database name, database user ID, and the list of IP list file. ::

	[%QUERY_EDITOR]
	dbname1:dbuser1:READIP.txt
	dbname1:dbuser2:WRITEIP1.txt,WRITEIP2.txt
	*:dba:READIP.txt
	*:dba:WRITEIP1.txt
	*:dba:WRITEIP2.txt
	 
	[%SHARD2]
	dbname:dbuser:iplist2.txt
	 
	[%SHARD3]
	dbname:dbuser:iplist2.txt
	 
	[%SHARD4]
	dbname:dbuser:iplist2.txt

The shard proxy specified above is QUERY_EDITOR, SHARD2, and SHARD3, SHARD4.

The QUERY_EDITOR shard proxy allows only access of the same applications.

*   A user logging in with dbuser1 to dbname1 accesses IP registered in READIP.txt
*   A user logging in with dbuser1 to dbname1 accesses IP registered in WRITEIP1.txt or WRITEIP2.txt
*   A user logging in with DBA to every database accesses IP registered in READIP.txt, WRITEIP1.txt, or WRITEIP2.txt

The following shows how to configure IPs accessible in ip_list_file. ::

	192.168.1.25
	192.168.*
	10.*
	*

The IPs specified above are as follows:

*   The configuration of the first line allows 192.168.1.25.
*   The configuration of the second line allows every IP starting with 192.168.
*   The configuration of the third line allows every IP starting with 10.
*   The configuration of the fourth line allows every IP.

For shard proxy which has been running, you can re-apply configuration by using the following command or check the current status.

To apply changes to server after database, database user ID, and IP allowed in shard proxy is configured, use the following command. ::

	cubrid shard acl reload [<SP_NAME>]
	
*   *SP_NAME* : shard proxy name. If this value is specified, changes are applied to specific shard proxy; if it is omitted, changes are applied to every shard proxy.

To output IP configuration of which database, database user ID, and IP allowed in shard proxy to screen, use the following command. ::

	cubrid shard acl status [<SP_NAME>]

*   *SP_NAME* : shard proxy name. If this value is specified, changes are applied to specific shard proxy; if it is omitted, changes are applied to every shard proxy.

.. note:: For details, see :ref:`limiting-server-access`.

Managing specific shard
-----------------------

Enter the following to run shard1. ::

	$ cubrid shard on shard1

If shard1 is not configured in shared memory, the following message will output. ::

	% cubrid shard on shard1
	Cannot open shared memory

To exit shard1, enter the following. ::

	$ cubrid shard off shard1

To restart shard1, enter the following. ::

	$ cubrhd shard restart shard1

The shard proxy reset feature disconnects exiting connection and makes new connection when shard proxy is connected unwanted database server due to failover in HA. If **SHARD_DB_NAME**, **SHARD_DB_USER**, **SHARD_DB_PASSWORD** is changed dynamically, it will try to connect with the changed value. ::

	% cubrid shard reset shard1

CUBRID SHARD Log
================

There are four types of logs that relate to starting the shard: access, proxy, error and SQL logs. Changing the directory of each log is available through **LOG_DIR**, **ERROR_LOG_DIR**, and **PROXY_LOG_DIR** parameter of the shard configuration file (**shard.conf**).

SHARD PROXY Log
---------------

	**Access Log**

	*   Parameter: **ACCESS_LOG**
	*   Description: Log the client access (the existing broker logs at the cas).
	*   Default directory: $CUBRID/log/broker/
	*   File name: <broker_name>_<proxy_index>.access
	*   Log type: All strings, except the access log and the cas_index at the cas, are identical

	::

		10.24.18.67 - - 1340243427.828 1340243427.828 2012/06/21 10:50:27 ~ 2012/06/21 10:50:27 23377 - -1 shard1     shard1
		10.24.18.67 - - 1340243427.858 1340243427.858 2012/06/21 10:50:27 ~ 2012/06/21 10:50:27 23377 - -1 shard1     shard1
		10.24.18.67 - - 1340243446.791 1340243446.791 2012/06/21 10:50:46 ~ 2012/06/21 10:50:46 23377 - -1 shard1     shard1
		10.24.18.67 - - 1340243446.821 1340243446.821 2012/06/21 10:50:46 ~ 2012/06/21 10:50:46 23377 - -1 shard1     shard1

	**Proxy Log Level**

	*   Parameter: **PROXY_LOG**
	*   Proxy log level policy: When the upper level is set, all logs of the lower level will be left.

		*   Ex) Set SCHEDULE and then all ERROR | TIMEOUT | NOTICE | SHARD | SCHEDULE logs will be left.

	*   Proxy Log Levell Item

		*   NONE or OFF: No log is left.
		*   ERROR (default): An internal error occurs and logging is not successfully processed
		*   TIMEOUT: Timeout such as session timeout or query timeout
		*   NOTICE: When the error is not a query without hint or other errors
		*   SHARD: Scheduling that shows which shard and which cas the client request have sent to and whether the request has responded to the client or not
		*   SCHEDULE: Shard processing such as getting the shard key ID through parsing the hit or hashing
		*   ALL: All logs

SHARD CAS Log
-------------

	**SQL Log**

	*   Parameter: **SQL_LOG**
	*   Description: Log queries such as prepare/execute/fetch and other cas information.
	*   Default directory: $CUBRID/log/broker/sql_log
	*   File name: %broker_name%_%proxy_index%_%shard_index%_%as_index%.sql.log

	::

		06/21 10:13:00.005 (0) STATE idle
		06/21 10:13:01.035 (0) CAS TERMINATED pid 31595
		06/21 10:14:20.198 (0) CAS STARTED pid 23378
		06/21 10:14:21.227 (0) connect db shard1@HostA user dba url shard1 session id 3
		06/21 10:14:21.227 (0) DEFAULT isolation_level 3, lock_timeout -1
		06/21 10:50:28.259 (1) prepare srv_h_id 1
		06/21 10:50:28.259 (0) auto_rollback
		06/21 10:50:28.259 (0) auto_rollback 0

	**Error log**

	*   Parameter: **ERROR_LOG_DIR**
	*   Description: For CUBRID, the cs library logs EID and error strings to the corresponding file. For cas4o/m, the cas logs errors to the corresponding file.
	*   Default directory: $CUBRID/log/broker/error_log
	*   File name: %broker_name%_%proxy_index%_%shard_index%_%cas_index%.err

	::

		Time: 06/21/12 10:50:27.776 - DEBUG *** file ../../src/transaction/boot_cl.c, line 1409
		trying to connect 'shard1@localhost'
		Time: 06/21/12 10:50:27.776 - DEBUG *** file ../../src/transaction/boot_cl.c, line 1418
		ping server with handshake
		Time: 06/21/12 10:50:27.777 - DEBUG *** file ../../src/transaction/boot_cl.c, line 966
		boot_restart_client: register client { type 4 db shard1 user dba password (null) program cubrid_cub_cas_1 login cubrid_user host HostA pid 23270 }

Constraints
===========

	**Changing or retrieving data in several shard DBs within one transaction**

	One transaction should be performed within only one shard DB, so the following constraints exist.

	*   It is unavailable to change data in several shard DBs through changing the shard key (**UPDATE**). If necessary, use **DELETE** / **INSERT**.

	*   When a query, such as join, sub-query, or, union, group by, between, like, in, exist, or any/some/all, for several shard DB data, a result different from the intended one may be returned.

	**Session**

	Session information is valid within each shard DB only. Therefore, the results from session-related functions such as **last_insert_id** () may be different from the intended result.

	**auto increment**

	The auto increment attribute or SERIAL is valid within each shard DB only. So a result different from the intended result may be returned.

[번역]

	**Windows용 SHARD DB와 응용 드라이버 사이의 접속**
	
	Windows용 SHARD DB 서버는 같은 버전의 드라이버를 사용하는 응용 프로그램만 접속이 가능하다. Linux용 SHARD DB 서버는 다른 버전의 드라이버를 사용하는 응용 프로그램과도 접속이 가능하다.
	