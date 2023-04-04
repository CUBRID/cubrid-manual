:meta-keywords: shard, cubrid shard, sharding, shard proxy, shard cas, shard log, 
:meta-description: introducing cubrid shard-relate contents

************
CUBRID SHARD
************

Database Sharding
====================

**Horizontal partitioning**

Horizontal partitioning refers to a design in which data with the same schema is stored in two or more tables based on rows. For example, 'User Table' can be divided two tables. first table contain the data which age is over 13, and second table contain the data which age is under 13. Horizontal partitioning is typically done within a single database. Horizontal partitioning reduces the size of each table's data and index. and increases work synchronization, which can be expected to improve performance. Horizontal partitioning is typically done within a single database.

.. image:: /images/image38.png

**ㅇatabase sharding**

Database sharding refers to a method of distributing and inquiring data in a horizontal partitioning method in a physically different database. For example, When "User Table" is in multiple databases, users under the age of 13 are stored in database 0 and users over the age of 13 are stored in database 1. database sharding is used to distribute and process large data that cannot be put into a single database instance, as well as for performance reasons.

Each partitioned database is called a shard or database shard.

.. image:: /images/image39.png

**CUBRID SHARD**

CUBRID SHARD is middleware for database sharding, It has the following features

*   As a middleware form to minimize changes to existing applications, Transparent sharded data can be accessed using JDBC (Java Database Connectivity) or CCI (CUBRID C Interface).
*   It is a method of selecting a shard to perform an actual query using a hint, and can be used by adding a hint to an existing query.
*   It guarantees the nature of some transactions.

.. _shard-terminologies:

CUBRID SHARD Default term 
===========================

The following are terms used to describe CUBRID SHARD in the future, and each meaning is as follows.

*   **shard DB** : A database that contains a partitioned table and data and actually handles user requests
*   **shard metadata** : Configuration information for the operation of CUBRID SHARD. It includes information for analyzing the requested query to select a shard DB to perform the actual query and information for creating a database session with shard DB.
*   **shard key (column)** : Column used as an identifier for selecting shard from sharded table
*   **shard key data** : The value of the shard key corresponding to the hint to identify the shard during the query
*   **shard ID** : Identifier for identifying shard DB
*   **proxy** : CUBRID middleware process that interprets hints contained in user queries and uses interpreted hints and shard metadata to pass requests to the shard DB to process the actual query

CUBRID SHARD Main feature
===========================

Middleware structure
-----------------------------

CUBRID SHARD is a middleware located in the middle of a physically or logically divided shard with the application, and maintains connection with multiple applications at the same time, and delivers it to the appropriate shard to process and return the results to the application.

.. image:: /images/image40.png

CUBRID SHARD can be connected by commonly used Java Database Connectivity (JDBC) or CCI (CUBRID C Interface), and changes to existing applications can be minimized because no separate driver or framework is required.

CUBRID SHARD middleware consists of three processes, broker/proxy/CAS, and the brief function of each process is as follows

.. image:: /images/image41.png

*   **broker**

    *   Receives initial connection requests from drivers, such as JDBC/CCI, and forwards the received connection requests to proxy in accordance with the load balancing policy
    *   Monitoring and recovering the health of proxy and CAS processes

*   **proxy**

    *   Forward user requests from drivers to CAS and return processed results to application
    *   Manage connection status with drivers and CAS and handle transactions

*   **CAS**

    *   Create a connection with a partitioned shard DB and use the connection to process user requests(query) received from proxy
    *   Transaction Processing

Choose shard DB using shard SQL hints
-----------------------------------------------

**shard SQL hint**

CUBRID SHARD uses hints and configuration information contained in the SQL hint syntax to select a shard DB to actually handle the query requested from the application. The types of SQL hints available are as follows.

+----------------------+---------------------------------------------------------------------------------------------------------+
| SQL hint             | description                                                                                             |
+======================+=========================================================================================================+
| **/*+ shard_key */** | Hint to specify the location of the bind variable or literal value corresponding to the shard key column|
+----------------------+---------------------------------------------------------------------------------------------------------+
| **/*+ shard_val(**   | Hint for explicitly specifying a hard key within a hint if the column                                   |
| *value*              | corresponding to the hard key does not exist in the query                                               |
| **) */**             |                                                                                                         |
+----------------------+---------------------------------------------------------------------------------------------------------+
| **/*+ shard_id(**    | Hints that users use to specify a specific shard DB                                                     |
| *shard_id*           | to process queries                                                                                      |
| **) */**             |                                                                                                         |
+----------------------+---------------------------------------------------------------------------------------------------------+

A brief summary of the terms for explanation is as follows. For a more detailed description of the term, see :ref:`shard-terminologies`\

*   **shard key** : Column that identifies shard DB. Generally, it is a column that exists in all or most tables in the shard DB and has the only value in the DB.
*   **shard id** : Identifier that can logically distinguish shards. For example, if one DB is divided into four shard DBs, there are four shard IDs.

For detailed query processing procedures using hints and setup information, see :ref:`shard SQL Typical procedure by which a query is performed using hints <using-shard-hint>`\

.. note::

    *   If there more than one shard hint exists in one query, pointing to the same shard will process normally, and pointing to another shard will process error.
    
        ::

            SELECT * FROM student WHERE shard_key = /*+ shard_key */ 250 OR shard_key = /*+ shard_key */ 22;

        In the above case, if 250 and 22 point to the same shard, normal processing is performed, and if they point to another shard, error processing is performed.

    *   In a driver function (e.g., PreparedStatement.executeBatch in JDBC, cci_execute_array in CCI) that batchs queries into arrays that bind multiple values, any query that approaches another shard will fail.

    *   Functions that execute multiple sentences at once in a shard environment (e.g., Statements.executeBatch in JDBC and cci_execute_batch in CCI) will be supported later.

**shard_key hint**

**shard_key** A hint is a hint for specifying the location of a bind variable or literal value, and must precede the bind variable or literal value.

ex) Specify the location of the bind variable. Perform a query in the shard DB corresponding to the student_no value bound at run time.

.. code-block:: sql

    SELECT name FROM student WHERE student_no = /*+ shard_key */ ?;

ex) Positioning literal values. Execute query in shard DB with literal value of study_no equal to 123

.. code-block:: sql

    SELECT name FROM student WHERE student_no = /*+ shard_key */ 123;

**shard_val hint**

**shard_val** Use the hint if there is no shard key column in the query that can identify the shard DB, Set the shard key column ignored when processing the actual query to the value of the **shard_val** hint. **shard_val** Hints can be located anywhere in the SQL syntax.

ex) The shard key is not included in the student_no or query. Student_no, the shard key, performs a query in the shard DB corresponding to 123

.. code-block:: sql

    SELECT age FROM student WHERE name =? /*+ shard_val(123) */;

**shard_id hint**

**shard_id** Hints are used when a user wants to perform a query by specifying a specific shard regardless of the value of the shard key column. **shard_id** hints can be located anywhere in the SQL syntax.

ex) If you need to perform a query in shard DB #3. Query students with age greater than 17 in shard DB #3

.. code-block:: sql

    SELECT * FROM student WHERE age > 17 /*+ shard_id(3) */;

.. _using-shard-hint:

**shard SQL Typical procedure by which a query is performed using hints**

#.  execute query

    The following is a process in which a user query request is performed.

    .. image:: /images/image42.png

    *   The application requests query processing to CUBRID SHARD through the JDBC interface, and adds a **shard_key** hint within the SQL syntax to specify the shard DB where the query will actually be performed.

    *   The SQL hint should be placed within the SQL syntax immediately before the binding or literal value of the column set to hard key, as in the above example.

    The shard SQL hints set in the bind variable are as follows.

    .. image:: /images/image43.png

    The shard SQL hints specified in the literal value are as follows.

    .. image:: /images/image44.png

#.  Analyze queries and select shard DB to handle actual requests

    The process of analyzing the query and selecting a shard DB to actually process the request is as follows.

    .. image:: /images/image45.png

    *   The SQL query received from the user is rewritten in the form of internal processing (query rewrite).
    *   Select the shard DB that performed the actual query using the SQL syntax and hints requested by the user.

        *   When SQL hints are set in the bind variable, the shard DB to which the actual query will be performed is selected using the result of hash of the value substituted for the shard_key bind variable during execute and the setting information.

        *   The hash function can be specified separately by the user, and if not, the shard_key value is hashed using the built-in hash function. The default embedded hash function is as follows.

        *   If shard_key is an integer

            ::

                default hash function (shard_key) = shard_key mod SHARD_KEY_MODULAR parameter(default value 256)

        *   If shard_key is an string

            ::

                default hash function (shard_key) = shard_key[0] mod SHARD_KEY_MODULAR parameter(default value 256)

    .. note::

        If the value of the shard_key bind variable is 100, "default hash function (shard_key) = 100% 256 = 100", then the shard DB #1 corresponding to the hash result 100 is selected by the setting, and the user request is delivered to the selected shard DB #1.

#.  Return query execution results

    The process of returning the result of performing the query is as follows.

    .. image:: /images/image46.png

    *   The processing result performed in shard DB #1 is received, and the result is returned to the requested application.
        
.. note::

    If a driver function (e.g., executeBatch on JDBC, cci_execute_array on CCI, cci_execute_batch) that batch-processes a query into an array that binds multiple values, it will fail.

Transaction Support
-----------------------

**Transaction Processing**

    CUBRID SHARD carries out internal processing procedures to ensure atomicity among ACID. For example, if an exception such as an abnormal termination of an application occurs during a transaction, a rollback request is delivered to the shard DB that was processing the query of the application to invalidate all changes made during the transaction.

    In addition, ACID, which is a characteristic of a general transaction, is guaranteed according to the characteristics and settings of the backend DBMS.

**Constraint**

    2PC(2 Phase commit) is impossible, and for this reason, if a query is performed with multiple shard DBs in one transaction, an error is processed.

Quick start
=============

Configuration Example
------------------------

The CUBRID SHARD, which will be described as an example, consists of four CUBRID SHARD DBs, as shown below, and the application processes user requests using the JDBC interface.

.. image:: /images/image49.png

**Create and start shard DB and user accounts**

As in the example of the above configuration, after creating a shard DB and a user account on each shard DB node, the database starts an instance.

*   shard DB name : *shard1*
*   shard DB user account : *shard*
*   shard DB user password : *shard123*

::

    sh> # create CUBRID SHARD DB
    sh> cubrid createdb shard1 en_US

    sh> # create CUBRID SHARD user account
    sh> csql -S -u dba shard1 -c "create user shard password 'shard123'"

    sh> # start CUBRID SHARD DB
    sh> cubrid server start shard1

change shard setting
------------------------

**cubrid_broker.conf**

Refer to **cubrid_broker.conf.shard**\ and change **cubrid_broker.conf**\ as follows

.. warning:: The port number and shared memory identifier should be appropriately changed to values not used by the current system.

::

    [broker]
    MASTER_SHM_ID           =30001
    ADMIN_LOG_FILE          =log/broker/cubrid_broker.log
     
    [%shard1]
    SERVICE                 =ON
    BROKER_PORT             =36000
    MIN_NUM_APPL_SERVER     =20  
    MAX_NUM_APPL_SERVER     =40  
    APPL_SERVER_SHM_ID      =36000
    LOG_DIR                 =log/broker/sql_log
    ERROR_LOG_DIR           =log/broker/error_log
    SQL_LOG                 =ON
    TIME_TO_KILL            =120
    SESSION_TIMEOUT         =300
    KEEP_CONNECTION         =ON
    MAX_PREPARED_STMT_COUNT =1024
    SHARD                   =ON
    SHARD_DB_NAME           =shard1
    SHARD_DB_USER           =shard
    SHARD_DB_PASSWORD       =shard123
    SHARD_NUM_PROXY         =1  
    SHARD_PROXY_LOG_DIR     =log/broker/proxy_log
    SHARD_PROXY_LOG         =ERROR
    SHARD_MAX_CLIENTS       =256
    SHARD_PROXY_SHM_ID      =36090
    SHARD_CONNECTION_FILE   =shard_connection.txt
    SHARD_KEY_FILE          =shard_key.txt

In the case of CUBRID, the **cubrid_port_id** parameter in the **cubrid.conf** setup file is used without setting the server's port number separately in **shard_connection.txt**\, so set the **cubrid_port_id** parameter in **cubrid.conf** to the same as the server. ::

    # TCP port id for the CUBRID programs (used by all clients).
    cubrid_port_id=41523

**shard_key.txt**

Set the **shard_key.txt** file, which is the shard DB mapping configuration file for the shard key hash value, as follows.

*   [%shard_key] : shard key section setting
*   Perform a query on shard #0 if the shard key hash result by the default hash function is 0-63
*   Perform a query on shard #1 if the shard key hash result by the default hash function is 64-127
*   Perform a query on shard #2 if the shard key hash result by the default hash function is 128~191
*   Perform a query on shard #3 if the shard key hash result by the default hash function is 192~255

::

    [%shard_key]
    #min    max     shard_id
    0       63      0
    64      127     1
    128     191     2
    192     255     3

**shard_connection.txt**

Set the shard configuration database settings file, **shard_connection.txt**, as follows.

*   Physical database name and connection information for shard #0
*   Physical database name and connection information for shard #1
*   Physical database name and connection information for shard #2
*   Physical database name and connection information for shard #3

::

    # shard-id  real-db-name  connection-info
    #                         * cubrid : hostname, hostname, ...
    0           shard1        HostA
    1           shard1        HostB
    2           shard1        HostC
    3           shard1        HostD

Starting and monitoring services
-------------------------------------

**start CUBRID SHARD**

To use the CUBRID SHARD function, run the broker as shown below. ::

    sh> cubrid broker start
    @ cubrid broker start
    ++ cubrid broker start: success

**CUBRID SHARD status query**

The state of the CUBRID SHARD is inquired as follows, and the state of the set parameter and process is checked. ::

    sh> cubrid broker status
    @ cubrid broker status
    % shard1
    ----------------------------------------------------------------
    ID     PID     QPS   LQS PSIZE STATUS       
    ----------------------------------------------------------------
    1-0-1  21272     0     0 53292 IDLE         
    1-1-1  21273     0     0 53292 IDLE         
    1-2-1  21274     0     0 53292 IDLE         
    1-3-1  21275     0     0 53292 IDLE
     
    sh> cubrid broker status -f
    @ cubrid broker status
    % shard1
    ----------------------------------------------------------------------------------------------------------------------------------------------------------
    ID     PID     QPS   LQS PSIZE STATUS          LAST ACCESS TIME               DB             HOST   LAST CONNECT TIME    SQL_LOG_MODE
    ----------------------------------------------------------------------------------------------------------------------------------------------------------
    1-0-1  21272     0     0 53292 IDLE         2013/01/31 15:00:24    shard1@HostA           HostA 2013/01/31 15:00:25               -
    1-1-1  21273     0     0 53292 IDLE         2013/01/31 15:00:24    shard1@HostB           HostB 2013/01/31 15:00:25               -
    1-2-1  21274     0     0 53292 IDLE         2013/01/31 15:00:24    shard1@HostC           HostC 2013/01/31 15:00:25               -
    1-3-1  21275     0     0 53292 IDLE         2013/01/31 15:00:24    shard1@HostD           HostD 2013/01/31 15:00:25               -

Creating an application example program
------------------------------------------

Using a simple Java program, verify that the CUBRID SHARD function operates normally.

**Creating an example table**

All shard DBs create temporary tables for example programs as follows. ::

    sh> csql -C -u shard -p 'shard123' shard1@localhost -c "create table student (s_no int, s_name varchar, s_age int, primary key(s_no))"    

**Creating an Example Program**

The following is an example program for inputting student information from 0 to 1023 times into the shard DB. Check the **cubrid_broker.conf** modified in the previous procedure to set the address/port and user information to the connection URL.

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
                            connection = DriverManager.getConnection("jdbc:cubrid:localhost:36000:shard1:::?charSet=utf8", "shard", "shard123");
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

**Execute an example program**

Do the example program written above as follows. ::

    sh> javac -cp ".:$CUBRID/jdbc/cubrid_jdbc.jar" *.java
    sh> java -cp ".:$CUBRID/jdbc/cubrid_jdbc.jar" TestInsert

**Check the results**

A query is performed in each shard DB to check whether the divided information is accurately input as intended.

*   shard #0 

    ::

        sh> csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
         
                 s_no  s_name                      s_age
        ================================================
                    0  'name_0'                       10
                    1  'name_1'                       11
                    2  'name_2'                       12
                    3  'name_3'                       13
                    ...

*   shard #1 

    ::

        sh> $ csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
         
                 s_no  s_name                      s_age
        ================================================
                   64  'name_64'                      10
                   65  'name_65'                      11
                   66  'name_66'                      12
                   67  'name_67'                      13  
                   ...

*   shard #2 

    ::

        sh> $ csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
         
                  s_no  s_name                      s_age
        =================================================
                   128  'name_128'                     10
                   129  'name_129'                     11
                   130  'name_130'                     12
                   131  'name_131'                     13
                   ...

*   shard #3 

    ::

        sh> $ csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
         
                 s_no  s_name                      s_age
        ================================================
                  192  'name_192'                     10
                  193  'name_193'                     11
                  194  'name_194'                     12
                  195  'name_195'                     13
                  ...

.. _shard-configuration:

Configuration and Settings
============================

Configuration
----------------

CUBRID SHARD is middleware and consists of broker, proxy, and CAS processes as shown below.

.. image:: /images/image50.png

.. _default-shard-conf:

Settings
-----------

To use the CUBRID SHARD function, you must set the parameters required to run SHARD-related processes in the **cubrid_broker.conf** file, and set the SHARD_CONNECTION_FILE and the SHARD_KEY_FILE file.

cubrid_broker.conf
^^^^^^^^^^^^^^^^^^^^^^

**cubrid_broker.conf** is used to set up the CUBRID SHARD feature. See **cubrid_broker.conf.shard**\ for setup, and see :ref:`broker-configuration`\ for more information about **cubrid_broker.conf**\.

.. _shard-connection-file:

shard connection file(SHARD_CONNECTION_FILE)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When driving a broker, CUBRID SHARD loads the shard connection setup file specified in the **SHARD_CONNECTION_FILE** parameter of **cubrid_broker.conf**\ to perform a connection with the backend shard DB.

The maximum number of shard DBs that can be set is 256.

If **cubrid_broker.conf**\ is not specified separately **SHARD_CONNECTION_FILE**\, load the default **shard_connection.txt** file.

**Default Formats**

The basic examples and formats of the shard connection setup file are as follows. ::

    #
    # shard-id      real-db-name    connection-info
    #                               * cubrid : hostname, hostname, ...
     
    # CUBRID
    0               shard1          HostA  
    1               shard1          HostB
    2               shard1          HostC
    3               shard1          HostD

.. note:: Like a typical CUBRID setting, # and subsequent content are treated as comments.

**CUBRID**

When the backend shard DB is CUBRID, the format of the connection setup file is as follows. ::

    # CUBRID
    # shard-id      real-db-name            connection-info
    # shard identifier ( >0 )        The physical name of each backend shard DB    host name
     
    0           shard_db_1          host1
    1           shard_db_2          host2
    2           shard_db_3          host3
    3           shard_db_4          host4

For CUBRID, use **CUBRID_PORT_ID** parameter of **cubrid.conf**\ without specifying the port number of a separate backend shard DB in the above configuration file. The **cubrid.conf** file is located in the **$CUBRID/conf** directory by default. ::

    $ vi cubrid.conf

    ...
    
    # TCP port id for the CUBRID programs (used by all clients).
    cubrid_port_id=41523

.. _shard-key-configuration-file:

shard key file (SHARD_KEY_FILE)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CUBRID SHARD loads the shard key configuration file specified in the **SHARD_KEY_FILE** parameter of **cubrid_broker.conf**, which is the default configuration file, at startup to determine which backend shard DB should handle the user request.

If **cubrid_broker.conf** does not specify **SHARD_KEY_FILE** separately, load the default **shard_key.txt** file.

**Formats**

Examples and formats of the shard key configuration file are as follows. ::

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

*   [%shard_key_name] : Specify a name for the shard key
*   min : shard key minimum range of hash results
*   max : shard key maximum range of hash results
*   shard_id : shard identifier

.. note:: Like a typical CUBRID setting, # and subsequent content are treated as comments.

.. warning::

    *   The min of the hard key should always start with 0.
    *   Max should be set up to 255.
    *   There should not be an empty value between min and max.
    *   **SHARD_KEY_MODULAR** parameter value (minimum 1, maximum 256) cannot be exceeded if the built-in hash function is used.
    *   The hard key hash result must be in the range of 0 to (**SHARD_KEY_MODULAR** - 1).

.. _setting-user-defined-hash-function:

User-defined hash function
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CUBRID SHARD uses the result of hashing the shard key and metadata configuration information to select the shard to perform the query. To this end, a built-in hash function may be used, or a hash function may be defined separately by the user.

**Embedded default hash function**

**SHARD_KEY_LIBRARY_NAME**, **SHARD_KEY_FUNCTION_NAME** If you do not set the SHARD_KEY_FUNCTION_NAME** parameter, the SHARD key is hashed using the built-in hash function, and the contents of the basic hash function are as follows.

*   If shard_key is an integer

    ::

        Default hash function (shard_key) = shard_key mod SHARD_KEY_MODULAR parameter (default 256)

*   If shard_key is a String 

    ::

        Default has function (shard_key) = shard_key[0] mod SHARD_KEY_MODULAR parameter (default 256)

**Setting user hash function**

CUBRID SHARD can hash the shard key contained in the query using a user-defined hash function in addition to the built-in hash function.

    **Implementing and creating libraries**

    The user-defined hash function should be implemented as a library in the form of **.so** that can be loaded at runtime, and the prototype is shown below.

    .. code-block:: c

        /*
           return value :
                success - shard key id(>0)
                fail    - invalid argument(ERROR_ON_ARGUMENT), shard key id make fail(ERROR_ON_MAKE_SHARD_KEY)
           type         : shard key value type
           val          : shard key value
        */
        typedef int (*FN_GET_SHARD_KEY) (const char *shard_key, T_SHARD_U_TYPE type,
                                           const void *val, int val_size);

    *   The return value of the hash function must be included in the hash result range of the **shard_key.txt** setup file.
    *   The **$CUBRID/include/shard_key.h** file must be included to build the library. Details such as returnable error codes can also be found in this file.


    **Change cubrid_broker.conf setting file**

    To reflect the created user-defined hash function, the **SHARD_KEY_LIBRARY_NAME** and **SHARD_KEY_FUNCTION_NAME** parameters of **cubrid_broker.conf**\ must be set according to the implementation.

    *   **SHARD_KEY_LIBRARY_NAME** : The (absolute) path to the user-defined hash library
    *   **SHARD_KEY_FUNCTION_NAME** : Name of the user-defined hash function

    **Example**

    The following is an example of using a user-defined hash function. First, check the **shard_key.txt** configuration file. ::

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

    To set up a custom hash function, you must first implement a shared library in the form of **.so** that can be loaded at runtime. The result of the hash function should be a value within the range of the hash result defined in the **shard_key.txt** setup file identified in the previous process. The following is a simple implementation.

    *   If shard_key is an integer

        *   Select shard #0 if shard_key is odd
        *   Select shard #1 if shard_key is even

    *   If shard_key is a String

        *   Select shard #0 if the shard_key string begins with 'a', 'A'
        *   Select shard #1 if the shard_key string begins with 'b', 'B'
        *   Select shard #2 if the shard_key string begins with 'c', 'C'
        *   Select shard #3 if the shard_key string begins with 'd', 'D'

    .. code-block:: c
        
        // <shard_key_udf.c>
        
        #include <string.h>
        #include <stdio.h>
        #include <unistd.h>
        #include "shard_key.h"
        
        int
        fn_shard_key_udf (const char *shard_key, T_SHARD_U_TYPE type,
                          const void *value, int value_len)
        {
          unsigned int ival;
          unsigned char c;
        
          if (value == NULL)
            {
              return ERROR_ON_ARGUMENT;
            }
        
          switch (type)
            {
            case SHARD_U_TYPE_INT:
              ival = (unsigned int) (*(unsigned int *) value);
              if (ival % 2)
                {
                  return 32;            // shard #1
                }
              else
                {
                  return 0;             // shard #0
                }
              break;
        
            case SHARD_U_TYPE_STRING:
              c = (unsigned char) (((unsigned char *) value)[0]);
              switch (c)
                {
                case 'a':
                case 'A':
                  return 0;             // shard #0
                case 'b':
                case 'B':
                  return 32;            // shard #1
                case 'c':
                case 'C':
                  return 64;            // shard #2
                case 'd':
                case 'D':
                  return 96;            // shard #3
                default:
                  return ERROR_ON_ARGUMENT;
                }
        
              break;
        
            default:
              return ERROR_ON_ARGUMENT;
            }
          return ERROR_ON_MAKE_SHARD_KEY;
        }

    Build custom hash functions in the form of shared libraries. The following is an example of a Makefile for building hash functions. ::

        # Makefile
         
        CC = gcc
        LIBS = $(LIB_FLAG)
        CFLAGS = $(CFLAGS_COMMON) -fPIC -I$(CUBRID)/include -I$(CUBRID_SRC)/src/broker
         
        SHARD_CC = gcc -g -shared -Wl,-soname,shard_key_udf.so
        SHARD_KEY_UDF_OBJS = shard_key_udf.o
         
        all:$(SHARD_KEY_UDF_OBJS)
                $(SHARD_CC) $(CFLAGS) -o shard_key_udf.so $(SHARD_KEY_UDF_OBJS) $(LIBS)
         
        clean:
                -rm -f *.o core shard_key_udf.so

    Modify **SHARD_KEY_LIBRARY_NAME**, **SHARD_KEY_FUNCTION_NAME** parameters to match the above implementation to include user-defined hash functions. ::

        [%student_no]
        SHARD_KEY_LIBRARY_NAME =$CUBRID/conf/shard_key_udf.so
        SHARD_KEY_FUNCTION_NAME =fn_shard_key_udf

    .. note:: 
    
        *   When defining user hash functions in an application, 16 bit(short), 32 bit(int), and 64 bit(INT64) integer can be used as input values for the hard key.
        *   If VARCHAR must be used, the user must define a hash function. 

.. _shard-start-monitoring:

Drive and monitor
====================

The CUBRID SHARD function can be operated or stopped using the cubrid broker utility, and various status information can be inquired.
For more information, see :ref:`broker`\. 

Setting test
================

It is possible to test whether the configuration is operating normally using the cubrid broker test command. For more information, see :ref:`broker-test`\.

.. _shard-logs:

CUBRID SHARD log
====================

Logs related to SHARD operation include access logs, proxy logs, SQL logs, and error logs. Changes to the storage directory of each log can be set through the **LOG_DIR**, **ERROR_LOG_DIR**, and **PROXY_LOG_DIR** parameters in the SHARD configuration file (**cubrid_broker.conf**).

SHARD PROXY log
-------------------

**Connection log**

*   Parameter: **ACCESS_LOG**
*   Description: Logging the client's connection (existing brokers leave logs on the CAS).
*   Default storage directory: $CUBRID/log/broker/
*   File name: <broker_name>_<proxy_index>.access
*   Log format: Same access log and all strings except cas_index left on CAS

::

    10.24.18.67 - - 1340243427.828 1340243427.828 2012/06/21 10:50:27 ~ 2012/06/21 10:50:27 23377 - -1 shard1     shard1
    10.24.18.67 - - 1340243427.858 1340243427.858 2012/06/21 10:50:27 ~ 2012/06/21 10:50:27 23377 - -1 shard1     shard1
    10.24.18.67 - - 1340243446.791 1340243446.791 2012/06/21 10:50:46 ~ 2012/06/21 10:50:46 23377 - -1 shard1     shard1
    10.24.18.67 - - 1340243446.821 1340243446.821 2012/06/21 10:50:46 ~ 2012/06/21 10:50:46 23377 - -1 shard1     shard1

**Proxy log**

*   parameter: **SHARD_PROXY_LOG_DIR**
*   Description: Log the operation inside the proxy.
*   Default storage directory: $CUBRID/log/broker/proxy_log
*   File name: <broker_name>_<proxy_index>.log

::

    06/21 10:50:46.822 [SRD] ../../src/broker/shard_proxy_io.c(1045): New socket io created. (fd:50).
    06/21 10:50:46.822 [SRD] ../../src/broker/shard_proxy_io.c(2517): New client connected. client(client_id:3, is_busy:Y, fd:50, ctx_cid:3, ctx_uid:4).
    06/21 10:50:46.825 [DBG] ../../src/broker/shard_proxy_io.c(3298): Shard status. (num_cas_in_tran=1, shard_id=2).
    06/21 10:50:46.827 [DBG] ../../src/broker/shard_proxy_io.c(3385): Shard status. (num_cas_in_tran=0, shard_id=2).

**Proxy log**

*   Parameter: **SHARD_PROXY_LOG**
*   Proxy log level policy: Setting the higher level leaves all the lower logs.

    *   Example) When SCHEDULE is set, ERROR | TIMEOUT | NOTICE | SHARD | SCHEDULE log is left.

*   Proxy log level list

    *   NONE or OFF: Don't leave a log.
    *   ERROR(default): If an error occurs internally and cannot be processed normally
    *   TIMEOUT: Timeout. ex) session timeout, query timeout
    *   NOTICE: Queries without hints and other errors
    *   SHARD: Scheduling such as which shard the client's request went to which CAS and whether it was responded to again
    *   SCHEDULE: Hint Shard processing such as getting a shard key id through parsing and hash
    *   ALL: All log

SHARD CAS log
--------------------

**SQL log**

*   Parameter: **SQL_LOG**
*   Description: Query such as prepare/execute/fetch and other CAS information are logged.
*   Default storage directory: $CUBRID/log/broker/sql_log
*   File name: <broker_name>_<proxy_index>_<shard_index>_<cas_index>.sql.log

::

    13-06-21 10:13:00.005 (0) STATE idle
    13-06-21 10:13:01.035 (0) CAS TERMINATED pid 31595
    13-06-21 10:14:20.198 (0) CAS STARTED pid 23378
    13-06-21 10:14:21.227 (0) connect db shard1@HostA user dba url shard1 session id 3
    13-06-21 10:14:21.227 (0) DEFAULT isolation_level 3, lock_timeout -1
    13-06-21 10:50:28.259 (1) prepare srv_h_id 1
    13-06-21 10:50:28.259 (0) auto_rollback
    13-06-21 10:50:28.259 (0) auto_rollback 0

**Error log**

*   Parameter: **ERROR_LOG_DIR**
*   Description: For CUBRID, log the EID and error string to the file in the cs library.
*   Default storage directory: $CUBRID/log/broker/error_log
*   File name: <broker_name>_<proxy_index>_<shard_index>_<cas_index>.err

::

    Time: 06/21/13 10:50:27.776 - DEBUG *** file ../../src/transaction/boot_cl.c, line 1409
    trying to connect 'shard1@localhost'
    Time: 06/21/13 10:50:27.776 - DEBUG *** file ../../src/transaction/boot_cl.c, line 1418
    ping server with handshake
    Time: 06/21/13 10:50:27.777 - DEBUG *** file ../../src/transaction/boot_cl.c, line 966
    boot_restart_client: register client { type 4 db shard1 user dba password (null) program cubrid_cub_cas_1 login cubrid_user host HostA pid 23270 }

Constraints
=================

**Linux only support**

The CUBRID SHARD function can be used only in Linux.

**One transaction can only be performed in one shard DB**

One transaction must be performed on only one shard DB, so the following restrictions exist.

*   It is impossible to change the data of several shard DBs due to the change of the shard key (**UPDATE**), and **DELETE** / **INSERT** is used if necessary.
*   It does not support queries for two or more shards (join, sub-query, or, union, group by, between, like, in, exist, any/some/all, etc.).

**Session information is valid only within each shard DB**

Since session information is valid only within each shard DB, the results of session-related functions such as:func:`LAST_INSERT_ID`\ may differ from what was intended.

**SET NAMES statement not supported**

Use of SET NAMES statements is not recommended in SHARD configuration environments. because they may not work properly.

**Auto increment is valid only within each shard DB**

Since the value of the auto increment attribute or SERIAL is valid only within each shard DB, a value different from the intended one can be returned.
