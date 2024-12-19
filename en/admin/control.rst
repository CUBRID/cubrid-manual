
:meta-keywords: cubrid server process, cub_server, cubrid broker, cubrid gateway,cubrid cas, cubrid manager server, cubrid HA, cubrid services, cubrid logging, cubrid errors, cubrid server access, cubrid status, cubrid manager, cubrid javasp, cub_javasp
:meta-description: How to control and check CUBRID services and processes (server, broker, gateway), logging files, access, errors, CUBRID Manager and CUBRID Java SP Server.

.. _control-cubrid-processes:

Controlling CUBRID Processes
============================

CUBRID processes can be controlled by **cubrid** utility.

Controlling CUBRID Service
--------------------------

The following **cubrid** utility syntax shows how to control services registered in the configuration file. One of the following can be specified in <command>.

::

    cubrid service <command>
    <command>: {start|stop|restart|status}

*   start: start services.
*   stop: stop services.
*   restart: restart services.
*   status: check status.
   
No additional options or arguments are required. 

Controlling Database Server
---------------------------

The following **cubrid** utility syntax shows how to control database server process. 

::

    cubrid server <command> [database_name]
    <command>: {start|stop|restart|status}

One of the following can be specified in <command>: 

*   start: start a database server process.
*   stop: stop a database server process.
*   restart: restart a database server process.
*   status: check status of a database server process. 

| Every command can specify a database name (**[database_name]**) as an argument.
| If the database name is not specified, the **status** command displays the currently running database servers' information, and the commands except **status** refer to the database names in the **server** property of the **[service]** section of **cubrid.conf**.

::

    # cubrid.conf

    [service]

    ...

    server=demodb,testdb

    ...

::

    % cubrid server start

    @ cubrid server start: demodb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 11.3

    ++ cubrid server start: success

    @ cubrid server start: testdb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 11.3

    ++ cubrid server start: success

Controlling Broker
------------------

The following cubrid utility syntax shows how to control CUBRID broker process. One of the following can be specified in <command>: start is used to run services; stop is used to stop services; restart is used to restart services; status is used to check status; acl is used to limit broker access; on/off is used to enable/disable the specified broker; reset is used to reset the connection to broker; info is used to display the broker configuration information; getid is used to get the SHARD ID(SHARD database ID) with SHARD key.

And more, SHARD feature can be used only after the broker is started and "SHARD", the broker parameter, whose value in cubrid_broker.conf is set to ON.

::

    cubrid broker <command> 
    <command>: start
               |stop
               |restart
               |status [options] [broker_name_expr]
               |acl {status|reload} broker_name
               |on <broker_name> |off <broker_name>
               |reset broker_name 
               |info
               |getid -b <broker_name> [-f] shard_key

Controlling Gateway
-------------------

The following **cubrid** utility syntax shows how to control CUBRID gateway process. 

::

    cubrid gateway <command> 
    <command>: start
               |stop
               |restart
               |status [options] [gateway_name_expr]
               |acl {status|reload} gateway_name
               |on <gateway_name> |off <gateway_name>
               |reset gateway_name 
               |info

*   start: start gateway processes.
*   stop: stop gateway processes. 
*   restart: restart gateway processes. 
*   status: check status of gateway processes.  
*   acl: limit gateway access.
*   on/off: enable/disable the specified gateway.
*   reset: reset the connection to gateway.
*   info: display the gateway configuration information.

Controlling CUBRID Manager Server
---------------------------------

To use the CUBRID Manager, the Manager server must be running where database server is running. The following **cubrid** utility syntax shows how to control the CUBRID Manager processes. 

::

    cubrid manager <command>
    <command>: {start|stop|status}

*   start: start manager server processes.
*   stop: stop manager server processes.
*   status: check the status of manager processes.

Controlling CUBRID HA
---------------------

The following **cubrid heartbeat** utility syntax shows how to use CUBRID HA. One of the following can be specified in *command*.

::

    cubrid heartbeat <command>
    <command>: {start|stop|copylogdb|applylogdb|reload|status}
    
*   start: start HA-related processes.
*   stop: stop HA-related processes.
*   copylogdb: start or stop copylogdb process.
*   applylogdb: start or stop applylogdb process.
*   reload: reload information on HA configuration.
*   status: check HA status. 

For details, see :ref:`cubrid-heartbeat`.

Controlling CUBRID Java Stored Procedure Server
------------------------------------------------

The following **cubrid** utility syntax shows how to control CUBRID Java Stored Procedure server process.

::

    cubrid javasp <command> [database_name]
    <command>: {start|stop|restart|status}

One of the following can be specified in <command>: 

*   start: start a Java Stored Procedure server process.
*   stop: stop a Java Stored Procedure server process.
*   restart: restart a Java Stored Procedure server process.
*   status: check status of a Java Stored Procedure server process.

| Every command can specify a database name (**[database_name]**) as an argument.
| If the database name is not specified, the **status** command displays status information of the Java stored procedure server of every currently running database server.

::

::

    % cubrid javasp start demodb

    @ cubrid javasp start: demodb
    ++ cubrid javasp start: success

.. _control-cubrid-services:

CUBRID Services
===============

Registering Services
--------------------

You can register database servers, CUBRID brokers, CUBRID gateways, CUBRID Manager(s) or CUBRID HA as CUBRID service in the configuration file ( **cubrid.conf** ). To register services, you can input for each **server**, **broker**, **gateway**, **manager** or **heartbeat** as a parameter value, and it is possible to input several values by concatenating them in comma(,).

If you do not register any service, only master process is registered by default. It is convenient for you to view status of all related processes at a glance or start and stop the processes at once with the **cubrid** **service** utility once it is registered as CUBRID service. 

- For details on CUBRID HA configuration, see :ref:`cubrid-service-util`.

The following example shows how to register database server and broker, gateway as service in the **cubrid.conf** file and enable databases ( *demodb* and *testdb* ) to start automatically at once when CUBRID server starts running.

::

    # cubrid.conf
    ... 

    [service]

    # The list of processes to be started automatically by 'cubrid service start' command
    # Any combinations are available with server, broker, gateway, manager and heartbeat.
    service=server,broker,gateway

    # The list of database servers in all by 'cubrid service start' command.
    # This property is effective only when the above 'service' property contains 'server' keyword.
    server=demodb,testdb

Starting Services
-----------------

In Linux environment, you can enter the code below to start CUBRID after installation. If no server is registered in the configuration file, only master process (cub_master) runs by default. 

In the Windows environment, the code below is normally executed only if a user with system permission has logged in. An administrator or general user can start or stop the CUBRID server by clicking its icon on the taskbar tray. 

::

    % cubrid service start
    
    @ cubrid master start
    ++ cubrid master start: success

The following message is returned if master process is already running. 

::

    % cubrid service start
    
    @ cubrid master start
    ++ cubrid master is running.

The following message is returned if master process fails to run. The example shows that service fails to start due to conflicts of the **cubrid_port_id** parameter value specified in the cubrid.conf file. In such a case, you can resolve the problem by changing the port. If it fails to start even though no port is occupied by process, delete /tmp/CUBRID1523 file and then restart the process. ::

    % cubrid service start
    
    @ cubrid master start
    cub_master: '/tmp/CUBRID1523' file for UNIX domain socket exist.... Operation not permitted
    ++ cubrid master start: fail

After registering service as explained in :ref:`control-cubrid-services`, enter the code below to start the service. You can verify that database server process and broker, gateway as well as registered *demodb* and *testdb* are starting at once. 

::

    % cubrid service start
    
    @ cubrid master start
    ++ cubrid master start: success
    @ cubrid server start: demodb

    This may take a long time depending on the amount of restore works to do.
    CUBRID 11.3

    ++ cubrid server start: success
    @ cubrid server start: testdb

    This may take a long time depending on the amount of recovery works to do.
    CUBRID 11.3

    ++ cubrid server start: success
    @ cubrid broker start
    ++ cubrid broker start: success
    @ cubrid gateway start
    ++ cubrid gateway start: success

Stopping Services
-----------------

Enter code below to stop CUBRID service. If no services are registered by a user, only master process stops and then restarts. 

::

    % cubrid service stop
    @ cubrid master stop
    ++ cubrid master stop: success

Enter code below to stop registered CUBRID service. You can verify that server process, broker process, gateway process and master process as well as *demodb* and *testdb* stop at once. 

::

    % cubrid service stop
    @ cubrid server stop: demodb

    Server demodb notified of shutdown.
    This may take several minutes. Please wait.
    ++ cubrid server stop: success
    @ cubrid server stop: testdb
    Server testdb notified of shutdown.
    This may take several minutes. Please wait.
    ++ cubrid server stop: success
    @ cubrid broker stop
    ++ cubrid broker stop: success
    @ cubrid gateway stop
    ++ cubrid gateway stop: success
    @ cubrid master stop
    ++ cubrid master stop: success
    @ cubrid master stop
    ++ cubrid master stop: success

Restarting Services
-------------------

Enter code below to restart CUBRID service. If no services are registered by a user, only master process stops and then restarts. 

::

    % cubrid service restart
    
    @ cubrid master stop
    ++ cubrid master stop: success
    @ cubrid master start
    ++ cubrid master start: success


Enter code below to restart registered CUBRID service. You can verify that server process, broker process, gateway process and master process as well as *demodb* and *testdb* stop and then restart at once. 

::

    % cubrid service restart
    
    @ cubrid server stop: demodb
    Server demodb notified of shutdown.
    This may take several minutes. Please wait.
    ++ cubrid server stop: success
    @ cubrid server stop: testdb
    Server testdb notified of shutdown.
    This may take several minutes. Please wait.
    ++ cubrid server stop: success
    @ cubrid broker stop
    ++ cubrid broker stop: success
    @ cubrid gateway stop
    ++ cubrid gateway stop: success
    @ cubrid master stop
    ++ cubrid master stop: success
    @ cubrid master start
    ++ cubrid master start: success
    @ cubrid server start: demodb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 11.3

    ++ cubrid server start: success
    @ cubrid server start: testdb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 11.3

    ++ cubrid server start: success
    @ cubrid broker start
    ++ cubrid broker start: success
    @ cubrid gateway start
    ++ cubrid gateway start: success

Managing Service Status
-----------------------

The following example shows how to check the status of master process and database server registered. 

::

    % cubrid service status
    
    @ cubrid master status
    ++ cubrid master is running.
    @ cubrid server status

    Server testdb (rel 11.3, pid 31059)
    Server demodb (rel 11.3, pid 30950)

    @ cubrid broker status
    NAME                   PID  PORT    AS   JQ                  TPS                  QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q         ERR-Q  UNIQUE-ERR-Q  #CONNECT   #REJECT
    ===========================================================================================================================================================================================================
    * query_editor         10877 30000     5    0                    0                    0        0        0        0        0        0     0/60.0     0/60.0             0             0         0         0
    * broker1              10889 33000     5    0                    0                    0        0        0        0        0        0     0/60.0     0/60.0             0             0         0         0
    @ cubrid gateway status
    NAME                   PID  PORT    AS   JQ                  TPS                  QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q         ERR-Q  UNIQUE-ERR-Q  #CONNECT   #REJECT
    ===========================================================================================================================================================================================================
    * oracle_gateway       10903 53000     5    0                    0                    0        0        0        0        0        0     0/60.0     0/60.0             0             0         0         0

    @ cubrid manager server status
    ++ cubrid manager server is not running.
    
The following message is returned if master process has stopped.

::

    % cubrid service status
    @ cubrid master status    
    ++ cubrid master is not running.

.. _cubrid-utility-logging:
 
cubrid Utility Logging
----------------------
 
CUBRID supports a logging feature about cubrid utility's running result.
 
**Logging contents**
 
The following contents are written to the **$CUBRID/log/cubrid_utility.log** file.
 
*   All commands through cubrid utilities: only usage, version and parsing errors are not logged.
    
*   Execution results by cubrid utilities: success/failure.
 
*   An error message when failure.
 
**Log file size** 
 
A size of **cubrid_utility.log** file is expanded by the size specified by **error_log_size** parameter in **cubrid.conf**; if this size is enlarged as the specified size, it is backed up as the **cubrid_utility.log.bak** file. 

**Log format**
 
::
 
    <time> (cubrid PID) <contents>
 
The following is an example of printing the log file.
    
::
        
    13-11-19 15:27:19.426 (17724) cubrid manager stop
    13-11-19 15:27:19.430 (17724) FAILURE: ++ cubrid manager server is not running.
    13-11-19 15:27:19.434 (17726) cubrid service start
    13-11-19 15:27:19.439 (17726) FAILURE: ++ cubrid master is running.
    13-11-19 15:27:22.931 (17726) SUCCESS
    13-11-19 15:27:22.936 (17756) cubrid service restart
    13-11-19 15:27:31.667 (17756) SUCCESS
    13-11-19 15:27:31.671 (17868) cubrid service stop
    13-11-19 15:27:34.909 (17868) SUCCESS
 
However, in Windows, some **cubrid** commands are executed through a service process; therefore, a duplicated information can be displayed again.
 
::
 
    13-11-13 17:17:47.638 ( 3820) cubrid service stop
    13-11-13 17:17:47.704 ( 7848) d:\CUBRID\bin\cubrid.exe service stop --for-windows-service
    13-11-13 17:17:56.027 ( 7848) SUCCESS
    13-11-13 17:17:57.136 ( 3820) SUCCESS

And, in Windows, a process run through the service process cannot print out an error message; therefore, for error messages related to the service start, you should definitely check them in the **cubrid_utility.log** file.

.. _control-cubrid-server:

Database Server
===============

Starting Database Server
------------------------

The following example shows how to run *demodb* server.

::

    % cubrid server start demodb
    
    @ cubrid server start: demodb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 11.3

    ++ cubrid server start: success
    
If you start *demodb* server while master process has stopped, master process automatically runs at first and then a specified database server runs.

::

    % cubrid server start demodb
    
    @ cubrid master start
    ++ cubrid master start: success
    @ cubrid server start: demodb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 11.3

    ++ cubrid server start: success

The following message is returned while *demodb* server is running.

::

    % cubrid server start demodb

    @ cubrid server start: demodb
    ++ cubrid server 'demodb' is running.

**cubrid server start** runs cub_server process of a specific database regardless of HA mode configuration. To run database in HA environment, you should use **cubrid heartbeat start**.

Stopping Database Server
------------------------

The following example shows how to stop *demodb* server. 

::

    % cubrid server stop demodb
    
    @ cubrid server stop: demodb
    Server demodb notified of shutdown.
    This may take several minutes. Please wait.
    ++ cubrid server stop: success

The following message is returned while *demodb* server has stopped. 

::

    % cubrid server stop demodb
    
    @ cubrid server stop: demodb
    ++ cubrid server 'demodb' is not running.

**cubrid server stop** stops cub_server process of a specific database regardless of HA mode configuration. Be careful not to restart the database server or occur failover. To stop database in HA environment, you should use **cubrid heartbeat stop** .

Restarting Database Server
--------------------------

The following example shows how to restart *demodb* server. *demodb* server that has already run stops and the server restarts. 

::

    % cubrid server restart demodb
    
    @ cubrid server stop: demodb
    Server demodb notified of shutdown.
    This may take several minutes. Please wait.
    ++ cubrid server stop: success
    @ cubrid server start: demodb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 11.3

    ++ cubrid server start: success

.. _control-cubrid-server-status:

Checking Database Server Status
-------------------------------

The following example shows how to check the status of a database server. Names of currently running database servers are displayed. 

::

    % cubrid server status
    
    @ cubrid server status
    Server testdb (rel 11.3, pid 24465)
    Server demodb (rel 11.3, pid 24342)

The following example shows the message when master process has stopped. 

::

    % cubrid server status
    
    @ cubrid server status
    ++ cubrid master is not running.

.. _limiting-server-access:

Limiting Database Server Access
-------------------------------

To limit brokers and the CSQL Interpreter connecting to the database server, configure the parameter value of **access_ip_control** in the **cubrid.conf** file to yes and enter the path of a file in which the list of IP addresses allowed to access the **access_ip_control_file** parameter value is written. You should enter the absolute file path. If you enter the relative path, the system will search the file under the **$CUBRID/conf** directory on Linux and under the **%CUBRID%\\conf** directory on Windows.

The following example shows how to configure the **cubrid.conf** file. 

::

    # cubrid.conf
    access_ip_control=yes
    access_ip_control_file="/home1/cubrid1/CUBRID/db.access"

The following example shows the format of the **access_ip_control_file** file. 

::

    [@<db_name>]
    <ip_addr>
    ...

*   <db_name>: The name of a database in which access is allowed
*   <ip_addr>: The IP address allowed to access a database. Using an asterisk (*) at the last digit means that all IP addresses are allowed. Several lines of <ip_addr> can be added in the next line of the name of a database.

To configure several databases, it is possible to specify additional [@<db_name>] and <ip_addr>.

Accessing any IP address except localhost is blocked by server if **access_ip_control** is set to yes but **ip_control_file** is not configured. A server will not run if analyzing **access_ip_control_file** fails caused by incorrect format. 

The following example shows **access_ip_control_file**. 

::

    [@dbname1]
    10.10.10.10
    10.156.*

    [@dbname2]
    *

    [@dbname3]
    192.168.1.15

The example above shows that *dbname1* database allows the access of IP addresses starting with 10.156;
*dbname2* database allows the access of every IP address;
*dbname3* database allows the access of an IP address, 192.168.1.15, only.

For the database which has already been running, you can modify a configuration file or you can check the currently applied status by using the following commands.

To change the contents of **access_ip_control_file** and apply it to server, use the following command. 

::

    cubrid server acl reload <database_name>

To display the IP configuration of a server which is currently running, use the following command. 

::

    cubrid server acl status <database_name>

.. _server-logs:

Database Server Log
-------------------

Error Log
^^^^^^^^^

The following log is created in the file of a server error log if an IP address that is not allowed to access is used. 

::

    Time: 10/29/10 17:32:42.360 - ERROR *** ERROR CODE = -1022, Tran = 0, CLIENT = (unknown):(unknown)(-1), EID = 2
    Address(10.24.18.66) is not authorized.

An error log of the database server is saved into **$CUBRID/log/server** directory, and the format of the file name is *<db_name>_<yyyymmdd>_<hhmi>.err*. The extension is ".err".
 
::
 
    demodb_20130618_1655.err

.. note:: 

    For details on how to limit an access to the broker server, see :ref:`limiting-broker-access`.
    
.. _server-event-log:
 
Event Log
^^^^^^^^^
 
If an event which affects on the query performance occurs, this is saved into the event log.

The events which are saved on the event log are *SLOW_QUERY*, *MANY_IOREADS*, *LOCK_TIMEOUT*, *DEADLOCK* and *TEMP_VOLUME_EXPAND*.

This log file is saved into the **$CUBRID/log/server** directory, and the format of the file name is *<db_name>_<yyyymmdd>_<hhmi>.event*. The extension is ".event".
 
::
 
    demodb_20130618_1655.event
 
**SLOW_QUERY**
 
If a slow query occurs, this event is written. If **sql_trace_slow** parameter value of cubrid.conf is set, this event will arise. The output example is as follows.
 
::
 
    06/12/13 16:41:05.558 - SLOW_QUERY
      client: PUBLIC@testhost|csql(13173)
      sql: update [y] [y] set [y].[a]= ?:1  where [y].[a]= ?:0  using index [y].[pk_y_a](+)
      bind: 5
      bind: 200
      time: 1015
      buffer: fetch=48, ioread=2, iowrite=0
      wait: cs=1, lock=1010, latch=0
 
*   client: <DB user>@<application client host name>|<program name>(<process ID>)
*   sql: slow query
*   bind: binding value. it is printed out as the number of <num> in the sql item, "?:<num>". The value of "?:0" is 5, and the value of "?:1" is 200.
*   time: execution time(ms)
*   buffer: execution statistics in the buffer

    *   fetch: fetching pages count
    *   ioread: I/O read pages count
    *   iowrite: I/O write pages count
    
*   wait: waiting time

    *   cs: waiting time on the critical section(ms)
    *   lock: waiting time to acquire the lock(ms)
    *   latch: waiting time to acquire the latch(ms)
 
On the above example, the query execution time was 1015ms, and lock waiting time was 1010ms, so we can indicate that almost all execution time was from lock waiting.
    
**MANY_IOREADS**
 
Queries which brought many I/O reads are written on the event log. If I/O reads occurs more than **sql_trace_ioread_pages** parameter value of cubrid.conf, the event is written on the event log. The following is an output example.
 
::
 
    06/12/13 17:07:29.457 - MANY_IOREADS
      client: PUBLIC@testhost|csql(12852)
      sql: update [x] [x] set [x].[a]= ?:1  where ([x].[a]> ?:0 ) using index [x].[idx](+)
      bind: 8
      bind: 100
      time: 528
      ioreads: 15648 
 
*   client: <DB user>@<application client host name>|<process name>(<process ID>)
*   sql: an SQL which brought many I/O reads
*   bind: binding value. it is printed out as the number of <num> in the sql item, "?:<num>". The value of "?:0" is 8, and the value of "?:1" is 100.
*   time: execution time(ms)
*   ioread: I/O read pages count

**LOCK_TIMEOUT**
 
When lock timeout occurs, queries of a waiter and a blocker are written on the event log. The following is an output example.
 
::
 
    02/02/16 20:56:18.650 - LOCK_TIMEOUT
    waiter:
      client: public@testhost|csql(21529)
      lock:    X_LOCK (oid=0|650|3, table=t)
      sql: update [t] [t] set [t].[a]= ?:0  where [t].[a]= ?:1
      bind: 2
      bind: 1
 
    blocker:
      client: public@testhost|csql(21541)
      lock:    X_LOCK (oid=0|650|3, table=t)
      sql: update [t] [t] set [t].[a]= ?:0  where [t].[a]= ?:1
      bind: 3
      bind: 1
      
*   waiter: a waiting client to acquire locks.

    *   lock: lock type, table and index names
    *   sql: a waiting SQL to acquire locks.
    *   bind: binding value.
 
*   blocker: a client to have locks.

    *   lock: lock type, table and index names
    *   sql: a SQL which is acquiring locks
    *   bind: binding value
 
On the above, you can indicate the blocker which brought lock timeout and the waiter which is waiting locks.
    
**DEADLOCK**
 
When a deadlock occurs, lock information of that transaction is written into the event log. The following is an output example.
 
::
 
    02/02/16 20:56:17.638 - DEADLOCK
    client: public@testhost|csql(21541)
    hold:
      lock:    X_LOCK (oid=0|650|5, table=t)
      sql: update [t] [t] set [t].[a]= ?:0  where [t].[a]= ?:1 
      bind: 3
      bind: 1
 
      lock:    X_LOCK (oid=0|650|3, table=t)
      sql: update [t] [t] set [t].[a]= ?:0  where [t].[a]= ?:1 
      bind: 3
      bind: 1
 
    wait:
      lock:    X_LOCK (oid=0|650|4, table=t)
      sql: update [t] [t] set [t].[a]= ?:0  where [t].[a]= ?:1 
      bind: 5
      bind: 2
 
    client: public@testhost|csql(21529)
    hold:
      lock:    X_LOCK (oid=0|650|6, table=t)
      sql: update [t] [t] set [t].[a]= ?:0  where [t].[a]= ?:1 
      bind: 4
      bind: 2
 
      lock:    X_LOCK (oid=0|650|4, table=t)
      sql: update [t] [t] set [t].[a]= ?:0  where [t].[a]= ?:1 
      bind: 4
      bind: 2
 
    wait:
      lock:    X_LOCK (oid=0|650|3, table=t)
      sql: update [t] [t] set [t].[a]= ?:0  where [t].[a]= ?:1 
      bind: 6
      bind: 1
 
*   client: <DB user>@<application client host name>|<process name>(<process ID>)

    *   hold: an object which is acquiring a lock
    
        *   lock: lock type, table name
        *   sql: SQL which is acquiring locks
        *   bind: binding value
        
    *   wait: an object which is waiting a lock
    
        *   lock: lock type, table name
        *   sql: SQL which is waiting a lock
        *   bind: binding value
 
On the above output, you can check the application clients and SQLs which brought the deadlock.
      
For more details on locks, see :ref:`lockdb` and :ref:`lock-protocol`.

**TEMP_VOLUME_EXPAND**
 
When a temporary volumes are expanded, this time is written to the event log. By this log, you can check what transaction brought the expansion of a temporary volumes.
 
::
  
    06/15/13 18:55:43.458 - TEMP_VOLUME_EXPAND
      client: public@testhost|csql(17540)
      sql: select [x].[a], [x].[b] from [x] [x] where (([x].[a]< ?:0 )) group by [x].[b] order by 1
      bind: 1000
      time: 44
      pages: 24399
 
*   client: <DB user>@<application client host name>|<process name>(<process ID>)
*   sql: SQL which requires a more space for temporary data. All INSERT statement except for INSERT ... SELECT syntax, and DDL statement are not delivered to the DB server, so it is shown as EMPTY
    SELECT, UPDATE and DELETE statements are shown on this item
*   bind: binding value
*   time: the required time to create a temporary volume(ms)
*   pages: the number of available pages within new temporary volume.

.. _database-server-error:

Database Server Errors
----------------------

Database server error processes use the server error code when an error has occurred. A server error can occur in any task that uses server processes. For example, server errors may occur while using the query handling program or the **cubrid** utility.

**Checking the Database Server Error Codes**

*   Every data definition statement starting with **#define ER_** in the **$CUBRID/include/error_code.h** file indicate the server error codes.

*   All message groups under "$set 5 MSGCAT_SET_ERROR" in the **$CUBRID/msg/en_US (in Korean, ko_KR.eucKR** or **ko_KR.utf8)/cubrid.msg** $ file indicates the server error messages.

When you write a C code with CCI driver, we recommend you to write a code with an error code name than with an error code number. For example, the error code number for violating the unique key is -670 or -886, but users can easily recognize the error when it is written as **ER_BTREE_UNIQUE_FAILED** or **ER_UNIQUE_VIOLATION_WITHKEY**\.

However, when you write a JAVA code with JDBC driver, you have to use error code numbers because "dbi.h" file cannot be included into the JAVA code. For JDBC program, you can get an error number by using getErrorCode() method of SQLException class.

::

    $ vi $CUBRID/include/error_code.h

    #define NO_ERROR                                       0
    #define ER_FAILED                                     -1
    #define ER_GENERIC_ERROR                              -1
    #define ER_OUT_OF_VIRTUAL_MEMORY                      -2
    #define ER_INVALID_ENV                                -3
    #define ER_INTERRUPTED                                -4
    ...
    #define ER_LK_OBJECT_TIMEOUT_SIMPLE_MSG              -73
    #define ER_LK_OBJECT_TIMEOUT_CLASS_MSG               -74
    #define ER_LK_OBJECT_TIMEOUT_CLASSOF_MSG             -75
    #define ER_LK_PAGE_TIMEOUT                           -76
    ...
    #define ER_PT_SYNTAX                                -493
    ...
    #define ER_BTREE_UNIQUE_FAILED                      -670
    ...
    #define ER_UNIQUE_VIOLATION_WITHKEY                 -886
    ...
    #define ER_LK_OBJECT_DL_TIMEOUT_SIMPLE_MSG          -966
    #define ER_LK_OBJECT_DL_TIMEOUT_CLASS_MSG           -967
    #define ER_LK_OBJECT_DL_TIMEOUT_CLASSOF_MSG         -968
    ...

The following are some of the server error code names, error code numbers, and error messages.

+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| Error Code Name                     | Error Code Number     | Error Message                                                                                                                                            |
+=====================================+=======================+==========================================================================================================================================================+
| ER_LK_OBJECT_TIMEOUT_SIMPLE_MSG     | -73                   | Your transaction (index ?, ?@?\|?) timed out waiting on ? lock on object ?\|?\|?. You are waiting for user(s) ? to finish.                               |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| ER_LK_OBJECT_TIMEOUT_CLASS_MSG      | -74                   | Your transaction (index ?, ?@?\|?) timed out waiting on ? lock on class ?. You are waiting for user(s) ? to finish.                                      |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| ER_LK_OBJECT_TIMEOUT_CLASSOF_MSG    | -75                   | Your transaction (index ?, ?@?\|?) timed out waiting on ? lock on instance ?\|?\|? of class ?. You are waiting for user(s) ? to finish.                  |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| ER_LK_PAGE_TIMEOUT                  | -76                   | Your transaction (index ?, ?@?\|?) timed out waiting on ? on page ?|?. You are waiting for user(s) ? to release the page lock.                           |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| ER_PT_SYNTAX                        | -493                  | Syntax: ?                                                                                                                                                |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| ER_BTREE_UNIQUE_FAILED              | -670                  | Operation would have caused one or more unique constraint violations.                                                                                    |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| ER_UNIQUE_VIOLATION_WITHKEY         | -886                  | "?" caused unique constraint violation.                                                                                                                  |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| ER_LK_OBJECT_DL_TIMEOUT_SIMPLE_MSG  | -966                  | Your transaction (index ?, ?@?\|?) timed out waiting on ? lock on object ?\|?\|? because of deadlock. You are waiting for user(s) ? to finish.           |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| ER_LK_OBJECT_DL_TIMEOUT_CLASS_MSG   | -967                  | Your transaction (index ?, ?@?\|?) timed out waiting on ? lock on class ? because of deadlock. You are waiting for user(s) ? to finish.                  |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| ER_LK_OBJECT_DL_TIMEOUT_CLASSOF_MSG | -968                  | Your transaction (index ?, ?@?\|?) timed out waiting on ? lock on instance ?\|?\|? of class ? because of deadlock. You are waiting for user(s) ? to      |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+

.. _broker:

Broker
======

Starting Broker
---------------

Enter the command below to start the broker.
The SHARD function is activated when SHARD, which is the broker parameter of cubrid_broker.conf, is set to ON.

::

    $ cubrid broker start
    @ cubrid broker start
    ++ cubrid broker start: success

The following message is returned if the broker is already running. 

::

    $ cubrid broker start
    @ cubrid broker start
    ++ cubrid broker is running.

.. warning::
 
    The number of required file descriptor(fd) when starting SHARD in Linux system will be a little bit more than SHARD_MAX_CLIENTS in cubrid_broker.conf. Therefore, when you limit the number of fd by using "ulimit -n", it should be a little bit greater than the value of SHARD_MAX_CLIENTS. When the limited number of fd in Linux system is smaller than the fd number which is required in SHARD, starting SHARD fails and the required fd number is displayed in the error message.

Stopping Broker
---------------

Enter the command below to stop the broker.
If SHARD, the broker parameter of the cubrid_broker.conf, is set to ON, the SHARD function is stopped.

::

    $ cubrid broker stop
    @ cubrid broker stop
    ++ cubrid broker stop: success

The following message is returned if the broker has stopped. 

::

    $ cubrid broker stop
    @ cubrid broker stop
    ++ cubrid broker is not running.

Restarting Broker
-----------------

Enter the command below to restart the whole brokers.

::

    $ cubrid broker restart

.. _broker-status:

Checking Broker Status
----------------------

The **cubrid broker status** utility allows you to check the broker status such as number of completed jobs and the number of standby jobs by providing various options. 
The status of clients accessed SHARD or the status of SHARD can be displayed by using **-c** and **-m** options when the **SHARD** broker parameter in **cubrid_broker.conf** is set to ON. Also, by using **-S** option or **-P** option, each shard DB or each proxy information can be displayed.

::

    cubrid broker status [options] [expr]

*   *expr*: A part of the broker name or "SERVICE=ON|OFF"

Specifying *expr* performs that the status of specific brokers which include *expr* in their names is monitored; specifying no argument means that status of all brokers which are registered in the broker environment configuration file ( **cubrid_broker.conf** ) is monitored.

If "SERVICE=ON" is specified on *expr*, only the status of working brokers is displayed; if "SERVICE=OFF" is specified, only the status of stopped brokers is displayed.

The following [options] are available with the **cubrid broker status** utility. -b, -q, -c, -m, -S, -P and -f are options to define the information to print; -s, -l and -t are options to control printing; -c, -m, -S and -P are options applied when using SHARD feature. All of these are possible to use as combining each other.

.. program:: broker_status

.. option:: -b

    Displays the status information of a broker but does not display information on broker application server.

.. option:: -q

    Displays standby jobs in the job queue.
    
.. option:: -c
 
    Displays the information of clients which access the proxy when **SHARD** in cubrid_broker.conf is set to ON.

.. option:: -m

    Displays the SHARD status and the statistical information when **SHARD** in cubrid_broker.conf is set to ON.

.. option:: -S
    
    For each shard DB, it displays **-b** option's items except NAME, PID, PORT and JQ and #CONNECT; in addition, it displays ID, SHARD-Q and #REQUEST.
    
.. option:: -P

    For each proxy, it displays **-b** option's items except NAME, PID, PORT and JQ; in addition, it displays ID, SHARD-Q and #RESTART.

.. option:: -f

    Displays information of DB and host accessed by broker.

    If it is used with the **-b** option, additional information on CAS is displayed. But SELECT, INSERT, UPDATE, DELETE, OTHERS items which shown on **-b** option are excluded.

    If it is used with the **-m** option, more detailed SHARD statistics are output.

    If it is used with the **-c** option, each shard proxy additionally outputs CLIENT-ID, CLIENT-IP, CONN-TIME, LAST-REQ-TIME, LAST-RES-TIME, and LAST-REQ-CODE items.

    If it is used with the **-P** option, STMT-POOL-RATIO is additionally printed. This item shows the ratio to use statements in the pool when you are using prepared statements.
    
.. option:: -l SECOND

    The **-l** option is only used with -f option together. It specifies accumulation period (unit: sec.) when displaying the number of application servers whose client status is Waiting or Busy. If it is omitted, the default value (1 second) is specified.

.. option:: -t

    Displays results in tty mode on the screen. The output can be redirected and used as a file. 

.. option:: -s SECOND

    Regularly displays the status of broker based on specified period. It returns to a command prompt if q is entered.

If you do not specify options or arguments, the status of all brokers is displayed. 

::

    $ cubrid broker status
    @ cubrid broker status
    % query_editor
    ----------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS
    ----------------------------------------
     1 28434     0     0 50144 IDLE
     2 28435     0     0 50144 IDLE
     3 28436     0     0 50144 IDLE
     4 28437     0     0 50140 IDLE
     5 28438     0     0 50144 IDLE

    % broker1 OFF

*   % query_editor: The broker name
*   ID: Serial number of CAS within the broker
*   PID: CAS process ID within the broker
*   QPS:  The number of queries processed per second
*   LQS: The number of long-duration queries processed per second
*   PSIZE: Size of CAS
*   STATUS: The current status of CAS (BUSY, IDLE, CLIENT_WAIT, CLOSE_WAIT)
*   % broker1 OFF: broker1's SERVICE parameter is set to OFF. So, broker1 is not started.

.. note::

    ID column which is displayed when checking the SHARD status shows the composition of " (serial number of proxy) - (serial number of shard DB) - (serial number of CAS accessing shard DB)"

::

    $ cubrid broker status
    @ cubrid broker status
    % shard1
    ----------------------------------------------------------------
        ID      PID     QPS   LQS PSIZE STATUS
    ----------------------------------------------------------------
        1-1-1  2580     100     3 55968 IDLE
        1-2-1  2581     200     4 55968 IDLE

The following shows the detail status of broker for 5 seconds. The display will reset per 5 seconds as the new status information. To escape the display of the status, press <Q>.

::

    $ cubrid broker status -b -s 5
    @ cubrid broker status

     NAME                    PID  PORT   AS   JQ    TPS    QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q   ERR-Q  UNIQUE-ERR-Q  #CONNECT  #REJECT
    =======================================================================================================================================================================
    * query_editor         13200 30000    5    0      0      0        0        0        0        0        0     0/60.0     0/60.0       0             0         0        0
    * broker1              13269 33000    5    0     70     60       10       20       10       10       10     0/60.0     0/60.0      30            10       213        1

*   NAME: The broker name
*   PID: Process ID of the broker
*   PORT: Port number of the broker
*   AS: The number of CAS
*   JQ: The number of standby jobs in the job queue
*   TPS: The number of transactions processed per second (calculated only when the option is configured to "-b -s <sec>")
*   QPS: The number of queries processed per second (calculated only when the option is configured to "-b -s <sec>")
*   SELECT: The number of SELECT queries after staring of the broker. When there is an option of "-b -s <sec>", it is updated every time with the number of SELECTs which have been executed during the seconds specified by this option.
*   INSERT: The number of INSERT queries after staring of the broker. When there is an option of "-b -s <sec>", it is updated every time with the number of INSERTs which have been executed during the seconds specified by this option.
*   UPDATE: The number of UPDATE queries after staring of the broker. When there is an option of "-b -s <sec>", it is updated every time with the number of UPDATEs which have been executed during the seconds specified by this option.
*   DELETE: The number of DELETE queries after staring of the broker. When there is an option of "-b -s <sec>", it is updated every time with the number of DELETEs which have been executed during the seconds specified by this option.
*   OTHERS: The number of queries like CREATE and DROP except for SELECT, INSERT, UPDATE, DELETE. When there is an option of "-b -s <sec>", it is updated every time with the number of queries which have been executed during the seconds specified by this option.
*   LONG-T: The number of transactions which exceed LONG_TRANSACTION_TIME. / the value of the LONG_TRANSACTION_TIME parameter. When there is an option of "-b -s <sec>", it is updated every time with the number of transactions which have been executed during the seconds specified by this option.
*   LONG-Q: The number of queries which exceed LONG_QUERY_TIME. / the value of the LONG_QUERY_TIME parameter. When there is an option of "-b -s <sec>", it is updated every time with the number of queries which have been executed during the seconds specified by this option.
*   ERR-Q: The number of queries with errors found. When there is an option of "-b -s <sec>", it is updated every time with the number of errors which have occurred during the seconds specified by this option. 
*   UNIQUE-ERR-Q: The number of queries with unique key errors found. When there is an option of "-b -s <sec>", it is updated every time with the number of unique key errors which have occurred during the seconds specified by this option.
*   #CONNECT: The number of connections that an application client accesses to CAS after starting the broker. 
*   #REJECT: The count that an application client excluded from ACL IP list is rejected to access a CAS. Regarding ACL setting, see :ref:`limiting-broker-access`.

The following checks the status of broker whose name includes broker1 and job status of a specific broker in the job queue with the **-q** option. If you do not specify broker1 as an argument, list of jobs in the job queue for all brokers is displayed. 

::

    % cubrid broker status -q broker1
    @ cubrid broker status
    % broker1
    ----------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS
    ----------------------------------------
     1 28444     0     0 50144 IDLE
     2 28445     0     0 50140 IDLE
     3 28446     0     0 50144 IDLE
     4 28447     0     0 50144 IDLE
     5 28448     0     0 50144 IDLE

The following monitors the status of a broker whose name includes broker1 with the **-s** option. If you do not specify broker1 as an argument, monitoring status for all brokers is performed regularly. It returns to a command prompt if q is not entered. 

::

    % cubrid broker status -s 5 broker1
    % broker1
    ----------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS
    ----------------------------------------
     1 28444     0     0 50144 IDLE
     2 28445     0     0 50140 IDLE
     3 28446     0     0 50144 IDLE
     4 28447     0     0 50144 IDLE
     5 28448     0     0 50144 IDLE

With the **-t** option, it display information of TPS and QPS to a file. To cancel displaying, press <Ctrl+C> to stop program.

::

    % cubrid broker status -b -t -s 1 > log_file

The following views information of server/database accessed by broker, the last access times of applications, the IP addresses accessed to CAS and the versions of drivers etc.  with the **-f** option.

::

    $ cubrid broker status -f broker1
    @ cubrid broker status
    % broker1 
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS         LAST ACCESS TIME      DB       HOST   LAST CONNECT TIME       CLIENT IP   CLIENT VERSION    SQL_LOG_MODE   TRANSACTION STIME  #CONNECT  #RESTART
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     1 26946     0     0 51168 IDLE         2011/11/16 16:23:42  demodb  localhost 2011/11/16 16:23:40      10.0.1.101     9.2.0.0062              NONE 2011/11/16 16:23:42         0         0
     2 26947     0     0 51172 IDLE         2011/11/16 16:23:34      -          -                   -          0.0.0.0                                -                   -         0         0
     3 26948     0     0 51172 IDLE         2011/11/16 16:23:34      -          -                   -          0.0.0.0                                -                   -         0         0
     4 26949     0     0 51172 IDLE         2011/11/16 16:23:34      -          -                   -          0.0.0.0                                -                   -         0         0
     5 26950     0     0 51172 IDLE         2011/11/16 16:23:34      -          -                   -          0.0.0.0                                -                   -         0         0
    
Meaning of each column in code above is as follows:

*   LAST ACCESS TIME: Time when CAS runs or the latest time when an application client accesses CAS
*   DB: Name of a database which CAS accesses most recently    
*   HOST: Name of a which CAS accesses most recently
*   LAST CONNECT TIME: Most recent time when CAS accesses a database
*   CLIENT IP: IP of an application clients currently being connected to an application server(CAS). If no application client is connected, 0.0.0.0 is displayed.
*   CLIENT VERSION: A driver's version of an application client currently being connected to a CAS
*   SQL_LOG_MODE: SQL logging mode of CAS. If the mode is same as the mode configured in the broker, "-" is displayed. For details, see :ref:`SQL_LOG <sql-log>` .
*   TRANSACTION STIME: Transaction start time
*   #CONNECT: The number of connections that an application client accesses to CAS after starting the broker
*   #RESTART: The number of connection that CAS is re-running after starting the broker

.. _as-detail:

Enter the command below with the **-b** and **-f** options to display AS(T W B Ns-W Ns-B) and CANCELED additionally.

::

    // The -f option is added upon execution of broker status information. Configuring Ns-W and Ns-B are displayed as long as N seconds by using the -l.
    % cubrid broker status -b -f -l 2
    @ cubrid broker status
    NAME          PID    PSIZE PORT  AS(T W B 2s-W 2s-B) JQ TPS QPS LONG-T LONG-Q  ERR-Q UNIQUE-ERR-Q CANCELED ACCESS_MODE SQL_LOG  #CONNECT #REJECT
    ================================================================================================================================================
    query_editor 16784 56700 30000      5 0 0     0   0   0  16  29 0/60.0 0/60.0      1            1        0          RW     ALL         4       1

Meaning of added columns in code above is as follows:

*   AS(T): Total number of CAS being executed
*   AS(W): The number of CAS in the status of Waiting
*   AS(B): The number of CAS in the status of Busy
*   AS(Ns-W): The number of CAS that the client belongs to has been waited for N seconds.
*   AS(Ns-B): The number of CAS that the client belongs to has been Busy for N seconds.
*   CANCELED: The number of queries have cancelled by user interruption since the broker starts (if it is used with the **-l** *N* option, it specifies the number of accumulations for *N* seconds).
*   ACCESS_MODE: The mode in which the broker connects to the DB. Broker mode is classified into three types: Read Write, Read Only, and Standby Only. For details, see :ref:`broker-mode`.
*   SQL_LOG: SQL logging mode of CAS. SQL LOG is classified into five types: ALL, OFF, ERROR, NOTICE, and TIMEOUT. For details, see :ref:`SQL_LOG <sql-log>` .

Use the **-m** option to display SHARD status and statistics information. For details on the parameter of **cubrid_broker.conf**, see :ref:`broker-configuration`. 

::

    $ cubrid broker status -m
    @ cubrid broker status
    % shard1
    MODULAR : 256, LIBRARY_NAME : NOT DEFINED, FUNCTION_NAME : NOT DEFINED
    ACTIVE-PROXY : 1, NUM-NO-HINT-ERR-Q : 0

    SHARD STATISTICS
    ID  NUM-KEY-Q   NUM-ID-Q   NUM-NO-HINT-Q             SUM
    ------------------------------------------------------------
        0       1281          0               0            1281
        1       1281          0               0            1281
        2       1281          0               0            1281
        3       1281          0               0            1281

    NUM_SHARD_Q
    PROXY_ID      1
    SHARD_ID
    ------------------
    0                1
    1                0
    2                0
    3                2


The description of each column is as follows.

*   shard1: The broker name
*   MODULAR: The **SHARD_KEY_MODULR** parameter value of **cubrid_broker.conf**
*   LIBRARY_NAME: The **SHARD_KEY_LIBRARY_NAME** parameter value of **cubrid_broker.conf**
*   FUNCTION_NAME: The **SHARD_KEY_FUNCTION_NAME** parameter value of **cubrid_broker.conf**
*   ACTIVE-PROXY: The number of proxy processes which are running
*   NUM-NO-HINT-ERR-Q: The number of errored queries because of no shard hints
*   SHARD STATISTICS: The shard ID query information

    *   ID: The shard DB serial number (shard ID)
    *   NUM-KEY-Q: The number of query requests which include the shard key
    *   NUM-ID-Q: The number of query requests which include the shard ID
    *   NUM-NO-HINT-Q: The number of requests handled by load balancing without hint when **SHARD_IGNORE_HINT** is configured
    *   SUM: NUM-KEY-Q + NUM-ID-Q
    
*   NUM_SHARD_Q: The number of requests to run queries waiting on SHARD-Q

    *   PROXY_ID: The proxy serial number
    *   SHARD_ID: The shard DB serial number

.. _shard-q:

SHARD-Q is an abbreviation of "Shard Waiting Queue". If proxy process requested to run the query but there was no CAS process to run this, then this request is waiting on SHARD-Q for a while. If the value of SHARD-Q is larger, it means that waiting cases are more. Therefore, you can consider to enlarge the value of MAX_NUM_APPL_SERVER.

Use the **-m -f** option to display more detailed SHARD statistics information. For details on the parameter of **cubrid_broker.conf**, see :ref:`broker-configuration`. 

::

    $ cubrid broker status -m -f
    @ cubrid broker status
    % shard1 
    MODULAR : 256, LIBRARY_NAME : NOT DEFINED, FUNCTION_NAME : NOT DEFINED
    SHARD : 0 [HostA] [shard1], 1 [HostB] [shard1], 2 [HostC] [shard1], 3 [HostD] [shard1]
    ACTIVE-PROXY : 1, NUM-NO-HINT-ERR-Q : 0

    SHARD STATISTICS
           ID  NUM-KEY-Q   NUM-ID-Q   NUM-NO-HINT-Q             SUM
        ------------------------------------------------------------
            0       2309          0               0            2309
            1       2309          0               0            2309
            2       2309          0               0            2309
            3       2309          0               0            2309

    NUM_SHARD_Q
       PROXY_ID      1
    SHARD_ID
    ------------------
    0                1
    1                0
    2                0
    3                2

    RANGE STATISTICS : user_no
          MIN ~   MAX :      SHARD     NUM-Q
        ------------------------------------
            0 ~    31 :          0      1157
           32 ~    63 :          1      1157
           64 ~    95 :          2      1157
           96 ~   127 :          3      1157
          128 ~   159 :          0      1152
          160 ~   191 :          1      1152
          192 ~   223 :          2      1152
          224 ~   255 :          3      1152

    DB Alias : shard1 [USER : shard, PASSWD : shard123]

A description of the added column is as follows.

*   SHARD: About shard DB within proxy

    *   0: shard DB serial number (shard ID)
    *   [HostA]: shard Connection Information
    *   [shard1]: Physical DB Name

*   RANGE STATISTICS: shard key query information

    *   user_no: The shard key name
    *   MIN: The minimum range of a shard key
    *   MAX: The maximum range of a shard key
    *   SHARD: The shard DB serial number (shard ID)
    *   NUM-Q: The number of query requests which include the shard key

The below displays the information of clients which access the proxy by using the **-c** option.

::

    $ cubrid broker status -c
    @ cubrid broker status
    % shard1(0), MAX-CLIENT : 50, CUR-CLIENT : 0
    % shard1(1), MAX-CLIENT : 50, CUR-CLIENT : 0

*   MAX-CLIENT: Maximum number of application clients that can access that proxy
*   CUR-CLIENT: Number of application clients connected to that proxy
    
Adding the **-f** option to the **-c** option will output more detailed client information.

::

    $ cubrid broker status -c -f
    @ cubrid broker status
    % shardqa(0), MAX-CLIENT : 50, CUR-CLIENT : 0
    ---------------------------------------------------------------------------------------------------------------
     CLIENT-ID           CLIENT-IP             CONN-TIME         LAST-REQ-TIME         LAST-RES-TIME  LAST-REQ-CODE
    ---------------------------------------------------------------------------------------------------------------
             2           127.0.0.1   2014/01/21 18:07:29   2014/01/21 18:07:56   2014/01/21 18:07:56              2
    % shardqa(1), MAX-CLIENT : 50, CUR-CLIENT : 0
    ---------------------------------------------------------------------------------------------------------------
     CLIENT-ID           CLIENT-IP             CONN-TIME         LAST-REQ-TIME         LAST-RES-TIME  LAST-REQ-CODE
    ---------------------------------------------------------------------------------------------------------------
    
A description of the added column is as follows.

*   CLIENT-ID: Client serial numbers assigned sequentially within the proxy
*   CLIENT-IP: Client IP address
*   CONN-TIME: Time of access to proxy
*   LAST-REQ-TIME: Last time a query was requested from proxy
*   LAST-RES-TIME: Last time a response was received from proxy
*   LAST-REQ-CODE: Code of the last function performed. The functions corresponding to the main code are as follows.

    *   0:  end_tran(Termination of transaction)
    *   1:  prepare
    *   2:  execute
    *   7:  fetch
    
The following display the information for each shard DB with  **-S** option.

::
    
    $ cubrid broker status -S
    @ cubrid broker status
    % shard1
      SHARD_ID    AS SHARD-Q     TPS      QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q   ERR-Q  UNIQUE-ERR-Q  #REQUEST
    =============================================================================================================================================
             0     2       0    3200     3772      956      960      928      928        0     0/60.0     0/60.0     700             0      6978
             1     2       0    3200     3776      960      960      928      928        0     0/60.0     0/60.0     704             0      6983
             2     2       0    3200     3762      960      960      928      914        0     0/60.0     0/60.0     690             0      6968
             3     2       0    3200     3776      960      960      928      928        0     0/60.0     0/60.0     704             0      6983

The following explains the additional columns.

*   SHARD_ID: The index of a shard(starting from 0).
*   SHARD-Q: The number of queries waiting on SHARD-Q(see :ref:`SHARD-Q <shard-q>`) for each shard.
*   #REQUEST: The total number of requests which a CAS belonging to the shard get from application clients(requests includes not only a query-execution request, but also a connection request and etc.)
             
If **-f** option is added to **-S** option, AS items are divided into (T W B 1s-W 1s-B) and displayed in detail. Regarding AS items, see :ref:`AS <as-detail>`.

The below displays the information for each proxy with **-P** option.

::

    $ cubrid broker status -P
    % shard1
      PROXY_ID    AS SHARD-Q     TPS      QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q   ERR-Q  UNIQUE-ERR-Q  #CONNECT  #REJECT  #RESTART
    ================================================================================================================================================================
             1     4       0   22174    26160    26160        0        0        0        0     0/60.0     0/60.0    5256             0       165        0         0
             2     4       0   35257    37903    23599     5152     4576     4576        0     0/60.0     0/60.0    4300             0       264        1         0
                                                                                                                                                                                   
The additional displayed items with **-P** option compared to **-b** option are as below.

*   PROXY_ID: The index of a proxy(starting from 1)
*   SHARD-Q: The number of queries waiting on SHARD-Q(see :ref:`SHARD-Q <shard-q>`) for each proxy.
*   #CONNECT: The count that application clients tried accessing a proxy
*   #REJECT: The count that an application client excluded from ACL IP list is rejected to access a proxy. Regarding ACL setting, see :ref:`limiting-broker-access`.
*   #RESTART: The count that a proxy is restarted.

If **-f** option is added to **-S** option, AS items are divided into (T W B 1s-W 1s-B) and displayed in detail and STMT-POOL-RATIO item is added. Regarding AS items, see :ref:`AS <as-detail>`.

::

    $ cubrid broker status -P -f
    % shard1
      PROXY_ID  AS(T      W      B   1s-W  1s-B) SHARD-Q   TPS   QPS   LONG-T   LONG-Q  ERR-Q  UNIQUE-ERR-Q  #CONNECT  #REJECT  #RESTART  STMT-POOL-RATIO (%)
    ==========================================================================================================================================================
             1     4      0      0      0      0       0     0     0   0/60.0   0/60.0      0             0       165        0         0                    -
             2     4      0      0      0      0       0     0     0   0/60.0   0/60.0      0             0       264        1         0                    -
                                                                                                                                               
A description of the added column is as follows.

*   STMT-POOL-RATIO: Percentage of use of statements in the pool when using prepare statements

When the **-b**, **-S** and **-P** options are used at the same time, the output is as follows.

::

    $ cubrid broker status -b -S -P

    @ cubrid broker status
      NAME          PID  PORT    AS   JQ       TPS      QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q   ERR-Q  UNIQUE-ERR-Q  #CONNECT #REJECT  
    ====================================================================================================================================================================
    * shard1      10204 56001     8    0     57431    64063    49759     5152     4576     4576        0     0/60.0     0/60.0    9556             0       429       0  
    * shard2      10221 56002     8    0     51913    58979    49844        0     4687     4448        0     0/60.0     0/60.0    9862             0       429       2  
    % broker1 OFF                                                                                                                                

    <SHARD INFO>
    % shard1
      SHARD_ID    AS SHARD-Q     TPS       QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q         ERR-Q  UNIQUE-ERR-Q    #REQUEST
    ======================================================================================================================================================
             0     2       0   14464     16165    12613     1312     1120     1120        0     0/60.0     0/60.0          2437             0       30645
             1     2       0   14464     15926    12310     1248     1184     1184        0     0/60.0     0/60.0          2198             0       30403
             2     2       0   14464     16347    12795     1312     1120     1120        0     0/60.0     0/60.0          2619             0       30824
             3     2       0   14039     15625    12041     1280     1152     1152        0     0/60.0     0/60.0          2302             0       29681
    % shard2
      SHARD_ID    AS SHARD-Q     TPS       QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q         ERR-Q  UNIQUE-ERR-Q    #REQUEST
    ======================================================================================================================================================
             0     2       0   13085     14884    12580        0     1184     1120        0     0/60.0     0/60.0          2503             0       27985
             1     2       0   13056     14808    12507        0     1181     1120        0     0/60.0     0/60.0          2456             0       27878
             2     2       0   13056     14743    12453        0     1170     1120        0     0/60.0     0/60.0          2391             0       27812
             3     2       0   12716     14544    12304        0     1152     1088        0     0/60.0     0/60.0          2512             0       27273

    % broker1 OFF

    <PROXY INFO>
    % shard1
      PROXY_ID    AS SHARD-Q      TPS     QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q  ERR-Q  UNIQUE-ERR-Q  #CONNECT  #REJECT  #RESTART
    ===============================================================================================================================================================
             1     4       0    22174   26160    26160        0        0        0        0     0/60.0     0/60.0   5256             0       165        0         0
             2     4       0    35257   37903    23599     5152     4576     4576        0     0/60.0     0/60.0   4300             0       264        1         0
    % shard2                                                                                                                        
      PROXY_ID    AS SHARD-Q      TPS     QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q  ERR-Q  UNIQUE-ERR-Q  #CONNECT  #REJECT  #RESTART
    ===============================================================================================================================================================
             1     4       0    21590   25586    25586        0        0        0        0     0/60.0     0/60.0   5266             0       165        0         0
             2     4       0    30323   33393    24258        0     4687     4448        0     0/60.0     0/60.0   4596             0       264        1         0
    % broker1 OFF

.. _limiting-broker-access:

Limiting Broker Access
----------------------

To limit the client applications accessing the broker, set to **ON** for the **ACCESS_ CONTROL** parameter in the **cubrid_broker.conf** file, and enter a name of the file in which the users and the list of databases and IP addresses allowed to access the **ACCESS_CONTROL_FILE** parameter value are written. 
The default value of the **ACCESS_CONTROL** broker parameter is **OFF**. All access to brokers not listed in **ACCESS_CONTROL_FILE** is restricted. Even if not listed in **ACCESS_CONTROL_FILE**, you can allow access to a specific broker by setting **ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER** to **ALLOW** for that broker.
The **ACCESS_CONTROL** and **ACCESS_CONTROL_FILE** parameters must be written under the [broker] section where common parameters are specified. On the other hand, the **ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER** parameter must be specified for each broker.

The format of **ACCESS_CONTROL_FILE** is as follows: 

::

    [%<broker_name>]
    <db_name>:<db_user>:<ip_list_file>
    ... 

*   <broker_name>: A broker name. It is the one of broker names specified in **cubrid_broker.conf** .
*   <db_name>: A database name. If it is specified as \*, all databases are allowed to access the broker server.
*   <db_user>: A database user ID. If it is specified as \*, all database user IDs are allowed to access the broker server.
*   <ip_list_file>: Names of files in which the list of accessible IPs are stored. Several files such as ip_list_file1, ip_list_file2, ... can be specified by using a comma (,).

[%<*broker_name*>] and <*db_name*>:<*db_user*>:<*ip_list_file*> can be specified separately for each broker. A separated line can be specified for the same <*db_name*> and the same <*db_user*>.
List of IPs can be written up to the maximum of 256 lines per <*db_name*>:<*db_user*> in a broker.
 
.. note::
    When using an absolute path name in Windows, the DRIVE name must be specified (for example, C:\\CUBRID\\CONF\\CUBRID_ACL.CONF).

The format of the ip_list_file is as follows:  

::

    <ip_addr>
    ... 

*   <ip_addr>: An IP address that is allowed to access the server. If the last digit of the address is specified as \*, all IP addresses in that rage are allowed to access the broker server.

If a value for **ACCESS_CONTROL** is set to ON and a value for **ACCESS_CONTROL_FILE** is not specified, the broker will only allow the access requests from the localhost. 
However, if **ACCESS_CONTROL_FILE** is not specified, all requests are allowed for brokers with **ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER** set to **ALLOW**.

Broker access restrictions not specified in **ACCESS_CONTROL_FILE**.

* Allow access only from localhost. (default)
* If **ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER** is set to **ALLOW**, all access is allowed.

If the analysis of **ACCESS_CONTROL_FILE** and ip_list_file fails when starting a broker, the broker will not be run.  

::

    # cubrid_broker.conf
    [broker]
    MASTER_SHM_ID           =30001
    ADMIN_LOG_FILE          =log/broker/cubrid_broker.log
    ACCESS_CONTROL   =ON
    ACCESS_CONTROL_FILE     =/home1/cubrid/access_file.txt
    [%QUERY_EDITOR]
    SERVICE                 =ON
    BROKER_PORT             =30000
    ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER = ALLOW
    ......

The following example shows the content of **ACCESS_CONTROL_FILE**. The * symbol represents everything, and you can use it when you want to specify database names, database user IDs and IPs in the IP list file which are allowed to access the broker server.  

::

    [%QUERY_EDITOR]
    dbname1:dbuser1:READIP.txt
    dbname1:dbuser2:WRITEIP1.txt,WRITEIP2.txt
    *:dba:READIP.txt
    *:dba:WRITEIP1.txt
    *:dba:WRITEIP2.txt
     
    [%BROKER2]
    dbname:dbuser:iplist2.txt
     
    [%BROKER3]
    dbname:dbuser:iplist2.txt
     
    [%BROKER4]
    dbname:dbuser:iplist2.txt

The brokers specified above are QUERY_EDITOR, BROKER2, BROKER3, and BROKER4.

The QUERY_EDITOR broker only allows the following application access requests.

*   When a user logging into *dbname1* with a *dbuser1* account connects from IPs registered in READIP.txt
*   When a user logging into *dbname1* with a *dbuser2* account connects from IPs registered in WRITEIP1.txt and WRITEIP2.txt
*   When a user logging into every database with a **DBA** account connects from IPs registered in READIP.txt, WRITEIP1.txt, and WRITEIP2.txt

The following example shows how to specify the IPs allowed in ip_list_file.  

::

    192.168.1.25
    192.168.*
    10.*
    *

The descriptions for the IPs specified in the example above are as follows:

*   The first line setting allows an access from 192.168.1.25.
*   The second line setting allows an access from all IPs starting with 192.168.
*   The third line setting allows an access from all IPs starting with 10.
*   The fourth line setting allows an access from all IPs.

For the broker which has already been running, you can modify the configuration file or check the currently applied status of configuration by using the following commands.

To configure databases, database user IDs and IPs allowed to access the broker and then apply the modified configuration to the server, use the following command.  ::

    cubrid broker acl reload [<BR_NAME>]

*   <BR_NAME>: A broker name. If you specify this value, you can apply the changes only to specified brokers. If you omit it, you can apply the changes to all brokers.

.. note:: Existing connections are not affected by reconfiguration of the access control list using the acl reload command. It means, it is applied only to new connections. To take effect changes on the connected connections, the broker must be restarted.

To display the databases, database user IDs and IPs that are allowed to access the broker in running on the screen, use the following command.  

::

    cubrid broker acl status [<BR_NAME>]

*   <BR_NAME>: A broker name. If you specify the value, you can display the specified broker configuration. If you omit it, you can display all broker configurations.

The below is an example of displaying results.

:: 
  
    $ cubrid broker acl status 
    ACCESS_CONTROL=ON 
    ACCESS_CONTROL_FILE=access_file.txt 
  
    [%broker1] 
    ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER=DENY
    demodb:dba:iplist1.txt 
           CLIENT IP LAST ACCESS TIME 
    ========================================== 
        10.20.129.11 
      10.113.153.144 2013-11-07 15:19:14 
      10.113.153.145 
      10.113.153.146 
             10.64.* 2013-11-07 15:20:50 
  
    testdb:dba:iplist2.txt 
           CLIENT IP LAST ACCESS TIME 
    ========================================== 
                   * 2013-11-08 10:10:12 
    
    [%broker2]
    ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER=ALLOW
	
    ++ cubrid broker acl: success
	
**Broker Logs**

    If you try to access brokers through IP addresses that are not allowed, the following logs will be created.

    *   ACCESS_LOG 

    ::

        1 192.10.10.10 - - 1288340944.198 1288340944.198 2010/10/29 17:29:04 ~ 2010/10/29 17:29:04 14942 - -1 db1 dba : rejected

    *   SQL LOG 

    ::

        10/29 10:28:57.591 (0) CLIENT IP 192.10.10.10 10/29 10:28:57.592 (0) connect db db1 user dba url jdbc:cubrid:192.10.10.10:30000:db1::: - rejected

.. note:: 

    For details on how to limit an access to the database server, see :ref:`limiting-server-access`.

.. _encrypted_connections:

Packet Encryption
-----------------

In an unencrypted communication environment, someone can monitor and interpret all the traffic between clients and a database server, and collected information could be used illegally. In order to access information in an unsafe communication environment while avoiding such an information leakage, data transmitted and received must be encrypted. CUBRID Broker can be configured in safe mode. In this case, all data transmitted and received between the database server and the client are encrypted.

CUBRID supports encrypted connections between clients and the server using TLS (Transport Layer Security) protocol. TLS provides data encryption mechanism as well as detecting data tampering, loss, hence ensures providing secure and trusted communication channel between clients and the server. CUBRID provides these TLS functions using `OpenSSL <https://www.openssl.org>`_.

CUBRID Broker can be configured for encrypted mode (**SSL = ON**) or non-encrypted mode (**SSL = OFF**) using **SSL** parameter in **cubrid_broker.conf**. A Broker must be restarted when the encryption parameter is changed. When a Broker is configured in encryption mode, clients such as **jdbc client** must connect in encryption mode, otherwise the connection to the broker will be rejected. The opposite is also true. That is, a connection request of clients using encryption mode to non-secure broker will be refused.

When SSL parameter is not specified in cubrid_broker.conf, that broker will be started in non-encrypted mode (**'SSL = OFF'** is the default). The following is an example of setting the Broker **'query_editor'** in **encrypted mode** (cubrid_broker.conf).

::

    # cubrid_broker.conf
    [query_editor]
    SERVICE                 =ON
    SSL                     =ON
    BROKER_PORT             =30000
    ....

**Certificate and Private Key**

In order to exchange an encrypted **symmetric session key** which will be used in a secure communication session, a public key and a private key are required in the server.

The public key used by the server is included in the certificate **'cas_ssl_cert.crt'**, and the private key is included in **'cas_ssl_cert.key'**. The certificate and private key are located in the **$CUBRID/conf** directory.

This certificate, **'self-signed'** certificate, was created with the OpenSSL command tool utility, and can be replaced with another certificate issued by a public **CA** (Certificate Authorities, for example **IdenTrust** or **DigiCert**) if desired. Or, existing certificate/private key can be replaced by generating new one using OpenSSL command utility as shown below.

::

    $ openssl genrsa -out my_cert.key 2048                                               # create 2048 bit size RSA private key
    $ openssl req -new -key my_cert.key -out my_cert.csr                                 # create CSR (Certificate Signing Request)
    $ openssl x509 -req -days 365 -in my_cert.csr -signkey my_cert.key -out my_cert.crt  # create a certificate valid for 1 year.

And replace **my_cert.key** and **my_cert.crt** with $CUBRID/conf/cas_ssl_cert.key and $CUBRID/conf/cas_ssl_cert.crt respectively.

.. _managing_specific_broker:

Managing a Specific Broker
--------------------------

Enter the code below to run *broker1* only. Note that *broker1* should have already been configured in the shared memory. 

::

    % cubrid broker on broker1

The following message is returned if *broker1* has not been configured in the shared memory. 

::

    % cubrid broker on broker1
    Cannot open shared memory

Enter the code below to stop *broker1* only. Note that service pool of *broker1* can also be removed. 

::

    % cubrid broker off broker1

The broker reset feature enables broker application servers (CAS) to disconnect the existing connection and reconnect when the servers are connected to unwanted databases due to failover, etc. in HA. For example, once Read Only broker is connected to active servers, it is not automatically connected to standby servers although standby servers are available. Connecting to standby servers is allowed only with the **cubrid broker reset** command.

Enter the code below to reset broker1. 

::

    % cubrid broker reset broker1

.. _changing-broker-parameter:

Dynamically Changing Broker Parameters
--------------------------------------

You can configure the parameters related to running the broker in the configuration file ( **cubrid_broker.conf** ). You can also modify some broker parameters temporarily while the broker is running by using the **broker_changer** utility. For details, see :ref:`broker-configuration`.

The syntax for the **broker_changer** utility, which is used to change broker parameters while the broker is running, is as follows. Enter the name of the currently running broker for the *broker_name* . The *parameters* can be used only for dynamically modifiable parameters. The *value* must be specified based on the parameter to be modified. You can specify the broker CAS identifier ( *cas_id* ) to apply the changes to the specific broker CAS. 

.. note::
    
    When CUBRID SHARD feature is activated(SHARD=ON in cubrid_broker.conf), you cannot apply the changes to the specific broker CAS by specifying the broker CAS identifier(cas_id).

*cas_id* is an ID to be output by **cubrid broker status** command.

::

    broker_changer <broker_name> [<cas_id>] <conf-name> <conf-value>

Enter the following to configure the **SQL_LOG** parameter to **ON** so that SQL logs can be written to the currently running broker. Such dynamic parameter change is effective only while the broker is running. 

::

    % broker_changer query_editor sql_log on
    OK

Enter the following to change the **ACCESS_MODE** to **Read Only** and automatically reset the broker in HA environment. 

::

    % broker_changer broker_m access_mode ro
    OK

.. note::

    If you want to control the service using cubrid utilities on Windows Vista or the later versions of Window, you are recommended to open the command prompt window as an administrator. For details, see the notes of :ref:`CUBRID Utilities <utility-on-windows>`.

.. _broker-configuration-info:

Broker configuration information
--------------------------------

**cubrid broker info** dumps the currently "working" broker parameters' configuration information(cubrid_broker.conf). broker parameters' information can be dynamically changed by **broker_changer** command; with **cubrid broker info** command, you can see the configuration information of the working broker. 

::

    % cubrid broker info

As a reference, to see the configuration information of the currently "working" system(cubrid.conf), use **cubrid paramdump** *database_name* command. By **SET SYSTEM PARAMETERS** syntax, the configuration information of the system parameters can be changed dynamically; with **cubrid broker info** command, you can see the configuration information of the system parameters.

Check CUBRID SHARD ID
--------------------------
**cubrid broker getid** prints out SHARD ID to know in what DB a specific key is included. :: 

    cubrid broker getid -b <broker-name> [-f] shard-key
    
*   -b <*broker-name*>: broker name
*   -f: print detail information
*   *shard-key*: shard key

The following is an example of outputting the SHARD ID for key 1 from the shard1 broker.

::

    $ cubrid broker getid -b shard1 1
    @ cubrid broker getid
    % shard1
     SHARD_ID : 0, SHARD_KEY: 1

The following is an example of outputting detailed information using the -f option.

::
    
    $ cubrid broker getid -b shard1 -f 1
    @ cubrid broker getid
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

.. _broker-test: 

Broker to DB Connection Test 
------------------------------

**cubrid broker test**\ is a command to perform user-defined queries on a DB that connects to a specified broker. When the shard function is activated, query all SHARD DBs. After performing the query, the transaction is rolled back. This command allows you to check the success of the query for each SHARD DB by performing a query to all SHARD DBs accessing the specified broker, and if you set the SHARD HASH function, you can check on which SHARD DB the query you entered was performed.

::

    cubrid broker test <broker_name> [-D <db_name>] [-u <db_user>] [-p <db_password>] {-c <query> | -i <input_file>} [-o <output_file>] [-s] [-v] 

*   db_name: DB name 
*   db_user: DB user account 
*   db_password: DB user password 
*   query: query 
*   input_file: File that stores the query you want to enter
*   output_file: File to save the results

The following options are available with the **cubrid broker test** utility.

.. program:: broker_test 

.. option:: -D DB_NAME 
     
    Specifies the DB name of the test target. When this option is omitted, the value of SHARD_DB_NAME parameter is used if the value of a SHARD parameter in cubrid_broker.conf is ON. If the value of a SHARD parameter is OFF, an error occurs.
     
.. option:: -u DB_USER 

    Specifies the DB account of the test target. When this option is omitted, the value of SHARD_DB_USER parameter is used if the value of a SHARD parameter in cubrid_broker.conf is ON. If the value of a SHARD parameter is OFF, "public" is input in CUBRID.
     
.. option:: -p DB_PASSWORD 

    Specifies the DB password of the test target. When this option is omitted, the value of SHARD_DB_PASSWORD parameter is used if the value of a SHARD parameter in cubrid_broker.conf is ON. If the value of a SHARD parameter is OFF, an empty string("") is input in CUBRID.
     
.. option:: -c QUERY 

    Specifies the query string. **-c** or **-i** option can be used to specify a query. If they are omitted, only the connection information between a broker and a DB is printed.
     
.. option:: -i FILE_NAME 

    Specifies the file where you saved the queries to input. **-c** or **-i** option can be used to specify a query.  If they are omitted, only the connection information between a broker and a DB is printed. 
     
.. option:: -o FILE_NAME 

    Specifies the file name to save the execution result to be displayed to the console. If this is omitted, the execution result is output only to the console.
     
.. option:: -s 

    A query which includes a SHARD hint is performed only in the SHARD DB. If this is omitted, a query is performed on all SHARD DBs.
     
    If the value of a SHARD parameter is OFF, this option does not affect to the query execution.

.. option:: -v 

    The error message and the result set of the SELECT are output together with the following information.
     
    *   RESULT: Error returned after performing query. output [OK | FAIL] 
    *   SHARD_ID: ID of the SHARD DB where the query was performed (if the SHARD parameter value is OFF, the item is not output)
    *   ROW COUNT: Number of rows affected by DML, or number of rows in case of SELECT query. Outputs -1 in case of query execution error 
    *   EXECUTION TIME: Time the query was performed 
    *   QUERY: Queries entered by the user
     
    If this option is omitted, only "RESULT, SHARD_ID, ROW COUNT, EXECUTION TIME, QUERY" will be output.

The following is an example of using the above options.

*   Query to DB 

    **When the SHARD parameter value of the cubrid_broker.conf is OFF** 
     
    Check if the DB connection is good.

    :: 

        $ cubrid broker test shard1 -D shard -u shard -p shard123 -c "select 1 from db_root where charset = 3" 
     
        @ cubrid broker test 
        @ [OK] CONNECT broker1 DB [demodb] USER [shard] 

        @ SHARD OFF 

        RESULT ROW COUNT EXECUTION TIME QUERY 
        ======================================================== 
        OK 1 0.011341 sec select 1,'a' from db_root where charset = 3 
        @ [OK] QUERY TEST 
         
    **When the SHARD parameter value of the cubrid_broker.conf is ON** 

    Check if all SHARD DBs are accessible.

    :: 
     
        $ cubrid broker test shard1 -D shard -u shard -p shard123 -c "select 1 from db_root where charset = 3" 

        @ cubrid broker test 
        @ [OK] CONNECT shard1 DB [shard] USER [shard] 

        @ SHARD ON 

        RESULT SHARD_ID ROW COUNT EXECUTION TIME QUERY 
        ================================================================== 
        OK 0 1 0.003436 sec select 1 from db_root where charset = 3 
        OK 1 1 0.003010 sec select 1 from db_root where charset = 3 
        OK 2 1 0.003039 sec select 1 from db_root where charset = 3 
        OK 3 1 0.002916 sec select 1 from db_root where charset = 3 
        @ [OK] QUERY TEST 

*   Check User Permissions

    **User do not have INSERT privileges on one of the SHARD DBs** 
     
    If user you do not have INSERT privileges, RESULT is displayed as FAIL.
     
    :: 
     
        $ cubrid broker test shard1 -c "insert into foo values (1,"a") " -v 
         
        @ cubrid broker test 
        @ [OK] CONNECT shard1 DB [shard] USER [shard] 

        @ SHARD ON 

        RESULT SHARD_ID ROW COUNT EXECUTION TIME QUERY 
        ================================================================== 
        OK 0 1 0.001322 sec insert into foo values(1,'a') 
        FAIL(-494) -1 -1 0.001608 sec insert into foo values(1,'a') 
        <Error> 
        ERROR CODE : -494 
        Semantic: INSERT is not authorized on foo. insert into foo foo (foo.a, foo.b) values (1, cast('a' as v...[CAS INFO - 127.0.0.1:52002, 1, 18145]. 

        OK 2 1 0.001334 sec insert into foo values(1,'a') 
        OK 3 1 0.001325 sec insert into foo values(1,'a') 
        @ [FAIL] QUERY TEST 
     
    **When there is no UPDATE authority on one of DBs which access a broker** 
     
    If DB do not have UPDATE privileges, RESULT is displayed as FAIL. 
     
    :: 
     
        $ vi dml.txt 
     
        #query 
        select a from foo 
        insert into foo(b) values(3) 
        update foo set c = 2 where b = 3 
        delete foo where b = 3 
     
    :: 
     
        $ cubrid broker test broker1 -D demodb -u shard -p shard123 -i dml.txt -v 

        @ cubrid broker test 
        @ [OK] CONNECT broker1 DB [demodb] USER [shard] 

        @ SHARD OFF 

        RESULT ROW COUNT EXECUTION TIME QUERY 
        ======================================================== 
        OK 1 0.001612 sec select a from foo 
        <Result of SELECT Command> 
          a 
        ------------ 
          1 

        OK 1 0.001215 sec insert into foo(b) values(3) 
        FAIL(-494) -1 0.001291 sec update foo set c = 2 where b = 3 
        <Error> 
        ERROR CODE : -494 
        Semantic: UPDATE is not authorized on foo. update foo foo set foo.c=2 where foo.b=3[CAS INFO - 127.0.0.1:52001, 1, 18139]. 

        OK 0 0.001534 sec delete foo where b = 3 
        @ [FAIL] QUERY TEST 

*   Check SHARD HASH normal operation 

    Check if the hashing is good for a specific key. 

    :: 
     
        $ vi test_query.txt 
         
        #query 
        select number from demo_db where key = /*+ shard_key */ 14 
        select number from demo_db where key = /*+ shard_key */ 50 
        select number from demo_db where key = /*+ shard_key */ 80 
        select number from demo_db where key = /*+ shard_key */ 120 
        .. 

    :: 
     
        $ cubrid broker test shard1 -D shard -u shard -p shard123 -i shard_key.txt -v -s 

        @ cubrid broker test 
        @ [OK] CONNECT shard1 DB [shard] USER [shard] 

        @ SHARD ON 

        RESULT SHARD_ID ROW COUNT EXECUTION TIME QUERY 
        ================================================================== 
        OK 0 1 0.002225 sec select * from foo where a = /*+ shard_key */ 10 
        <Result of SELECT Command> 
          a b 
        ---------------------------------- 
          10 'aaaa' 

        OK 1 1 0.001870 sec select * from foo where a = /*+ shard_key */ 40 
        <Result of SELECT Command> 
          a b 
        ---------------------------------- 
          40 'bbb' 

        OK 2 1 0.002004 sec select * from foo where a = /*+ shard_key */ 70 
        <Result of SELECT Command> 
          a b 
        ---------------------------------- 
          70 'cccc' 

        OK 3 1 0.002025 sec select * from foo where a = /*+ shard_key */ 100 
        <Result of SELECT Command> 
          a b 
        ---------------------------------- 
          100 'dddd' 

        @ [OK] QUERY TEST 
         
*   Whether to using the -v option 

    **When using -v option** 
     
    When the SELECT query is successful, the result set is output, and when the SELECT query is unsuccessful, an error message is output.
     
    :: 
     
        $ cubrid broker test broker1 -D demodb -u shard -p shard123 -i dml.txt -v 
        @ cubrid broker test 
        @ [OK] CONNECT broker1 DB [demodb] USER [shard] 

        @ SHARD OFF 

        RESULT ROW COUNT EXECUTION TIME QUERY 
        OK 1 0.001311 sec select a from foo 
        <Result of SELECT Command> 
          a 
        ------------ 
          1 

        OK 1 0.001083 sec insert into foo(b) values(3) 
        FAIL(-494) -1 0.001166 sec update foo set c = 2 where b = 3 
        <Error> 
        ERROR CODE : -494 
        Semantic: UPDATE is not authorized on foo. update foo foo set foo.c=2 where foo.b=3[CAS INFO - 127.0.0.1:52001, 1, 18139]. 

        OK 0 0.001399 sec delete foo where b = 3 
        @ [FAIL] QUERY TEST 
         
    **When not using the -v option** 

    It only prints the success or failure of the query. 
     
    :: 
     
        $ cubrid broker test broker1 -D demodb -u shard -p shard123 -i dml.txt 
         
        @ cubrid broker test 
        @ [OK] CONNECT broker1 DB [demodb] USER [shard] 

        @ SHARD OFF 

        RESULT ROW COUNT EXECUTION TIME QUERY 
        OK 1 0.001485 sec select a from foo 
        OK 1 0.001123 sec insert into foo(b) values(3) 
        FAIL(-494) -1 0.001180 sec update foo set c = 2 where b = 3 
        OK 0 0.001393 sec delete foo where b = 3 
        @ [FAIL] QUERY TEST 
         
*   Check SHARD key value 

    When SHARD key hint is given with the -s option, query to its SHARD DB, and outputs the result. With SHARD_ID, you can confirm that the query is executed from any SHARD DB.
     
    :: 
     
        $ cubrid broker test shard1 -i shard_key.txt -s -v 
         
        @ cubrid broker test 
        @ [OK] CONNECT shard1 DB [shard1] USER [shard] 

        @ SHARD ON 

        RESULT SHARD_ID ROW COUNT EXECUTION TIME QUERY 
        OK 0 1 0.144730 sec select * from foo where a = /*+ shard_key */ 10 
        <Result of SELECT Command> 
          a b 
        ---------------------------------- 
          10 'aaaa' 

        OK 1 1 0.001870 sec select * from foo where a = /*+ shard_key */ 40 
        <Result of SELECT Command> 
          a b 
        ---------------------------------- 
          40 'bbb' 

        OK 2 1 0.002004 sec select * from foo where a = /*+ shard_key */ 70 
        <Result of SELECT Command> 
          a b 
        ---------------------------------- 
          70 'cccc' 

        OK 3 1 0.002025 sec select * from foo where a = /*+ shard_key */ 100 
        <Result of SELECT Command> 
          a b 
        ---------------------------------- 
          100 'dddd' 

        @ [OK] QUERY TEST


.. _broker-logs:

Broker Logs
-----------

There are three types of logs that relate to starting the broker: access, error and SQL logs. Each log can be found in the log directory under the installation directory. You can change the directory where these logs are to be stored through **LOG_DIR** and **ERROR_LOG_DIR** parameters of the broker configuration file (**cubrid_broker.conf**).

When **SHARD** = ON, the log directory of CUBRID proxy can be configured by using the **SHARD_PROXY_LOG_DIR** parameter. 

Checking the Access Log
^^^^^^^^^^^^^^^^^^^^^^^

The access log file records information on the application client and is stored to **$CUBRID/log/broker/**\ `<broker_name>`\ **.access** file. If the **ACCESS_LOG** parameter is configured to **ON** in the broker configuration file, when the broker stops properly, the access log file is stored. 

The maximum size of the ACCESS_LOG file can be specified through the ACCESS_LOG_MAX_SIZE parameter. When the ACCESS_LOG file is larger than the specified size, it is backed up in the name of broker_name.access.YYYYMMDDHHMISS, and the log is recorded in a new file (broker_name.access).

The record of denied access is recorded in broker_name.access.denied. It is backed up with the same rules as the ACCESS_LOG file.

The following example and description show an access log file created in the log directory: 

::

    1 192.168.56.4 2020/11/10 14:41:55 testdb dba NEW 6

*   1: ID assigned to the application server of the broker
*   192.168.56.4: IP address of the application client
*   2020/11/10 14:41:55: Time when the client's request processing started
*   testdb: The name of the database that the client requested to connect to
*   dba: The user name of the database that the client requested to connect to
*   NEW: Connection type

    *   NEW: New connection
    *   OLD: Change client or reconnection of existing connection due to CAS restart
    *   REJ: Connction denied (Recorded only in access.denied file)

*   6: session-id (session-id assgined by server)

Checking the Error Log
^^^^^^^^^^^^^^^^^^^^^^

The error log file records information on errors that occurred during the client's request processing and is stored to **$CUBRID/log/broker/error_log**\ `<broker_name>_<app_server_num>`\ **.err** file. For error codes and error messages, see :ref:`cas-error`.

The following example and description show an error log: 

::

    Time: 02/04/09 13:45:17.687 - SYNTAX ERROR *** ERROR CODE = -493, Tran = 1, EID = 38
    Syntax: Unknown class "unknown_tbl". select * from unknown_tbl

*   Time: 02/04/09 13:45:17.687: Time when the error occurred
*   - SYNTAX ERROR: Type of error (e.g. SYNTAX ERROR, ERROR, etc.)
*   \*\*\* ERROR CODE = -493: Error code
*   Tran = 1: Transaction ID. -1 indicates that no transaction ID is assigned.
*   EID = 38: Error ID. This ID is used to find the SQL log related to the server or client logs when an error occurs during SQL statement processing.
*   Syntax ...: Error message (An ellipsis ( ... ) indicates omission.)

.. _sql-log-manage:

Managing the SQL Log
^^^^^^^^^^^^^^^^^^^^

The SQL log file records SQL statements requested by the application client and is stored with the name of *<broker_name>_<app_server_num>*. sql.log. The SQL log is generated in the log/broker/sql_log directory when the SQL_LOG parameter is set to ON. Note that the size of the SQL log file to be generated cannot exceed the value set for the SQL_LOG_MAX_SIZE parameter. CUBRID offers the **broker_log_top** and **cubrid_replay** utilities to manage SQL logs. Each utility should be executed in a directory where the corresponding SQL log exists.

The following examples and descriptions show SQL log files: 

::

    13-06-11 15:07:39.282 (0) STATE idle
    13-06-11 15:07:44.832 (0) CLIENT IP 192.168.10.100
    13-06-11 15:07:44.835 (0) CLIENT VERSION 11.3.0.1089
    13-06-11 15:07:44.835 (0) session id for connection 0
    13-06-11 15:07:44.836 (0) connect db demodb user dba url jdbc:cubrid:192.168.10.200:30000:demodb:dba:********: session id 12
    13-06-11 15:07:44.836 (0) DEFAULT isolation_level 4, lock_timeout -1
    13-06-11 15:07:44.840 (0) end_tran COMMIT
    13-06-11 15:07:44.841 (0) end_tran 0 time 0.000
    13-06-11 15:07:44.841 (0) *** elapsed time 0.004
    
    13-06-11 15:07:44.844 (0) check_cas 0
    13-06-11 15:07:44.848 (0) set_db_parameter lock_timeout 1000
    13-06-11 15:09:36.299 (0) check_cas 0
    13-06-11 15:09:36.303 (0) get_db_parameter isolation_level 4
    13-06-11 15:09:36.375 (1) prepare 0 CREATE TABLE unique_tbl (a INT PRIMARY key);
    13-06-11 15:09:36.376 (1) prepare srv_h_id 1
    13-06-11 15:09:36.419 (1) set query timeout to 0 (no limit)
    13-06-11 15:09:36.419 (1) execute srv_h_id 1 CREATE TABLE unique_tbl (a INT PRIMARY key);
    13-06-11 15:09:38.247 (1) execute 0 tuple 0 time 1.827
    13-06-11 15:09:38.247 (0) auto_commit
    13-06-11 15:09:38.344 (0) auto_commit 0
    13-06-11 15:09:38.344 (0) *** elapsed time 1.968
    
    13-06-11 15:09:54.481 (0) get_db_parameter isolation_level 4
    13-06-11 15:09:54.484 (0) close_req_handle srv_h_id 1
    13-06-11 15:09:54.484 (2) prepare 0 INSERT INTO unique_tbl VALUES (1);
    13-06-11 15:09:54.485 (2) prepare srv_h_id 1
    13-06-11 15:09:54.488 (2) set query timeout to 0 (no limit)
    13-06-11 15:09:54.488 (2) execute srv_h_id 1 INSERT INTO unique_tbl VALUES (1);
    13-06-11 15:09:54.488 (2) execute 0 tuple 1 time 0.001
    13-06-11 15:09:54.488 (0) auto_commit
    13-06-11 15:09:54.505 (0) auto_commit 0
    13-06-11 15:09:54.505 (0) *** elapsed time 0.021
    
    ...
    
    13-06-11 15:19:04.593 (0) get_db_parameter isolation_level 4
    13-06-11 15:19:04.597 (0) close_req_handle srv_h_id 2
    13-06-11 15:19:04.597 (7) prepare 0 SELECT * FROM unique_tbl  WHERE ROWNUM BETWEEN 1 AND 5000;
    13-06-11 15:19:04.598 (7) prepare srv_h_id 2 (PC)
    13-06-11 15:19:04.602 (7) set query timeout to 0 (no limit)
    13-06-11 15:19:04.602 (7) execute srv_h_id 2 SELECT * FROM unique_tbl  WHERE ROWNUM BETWEEN 1 AND 5000;
    13-06-11 15:19:04.602 (7) execute 0 tuple 1 time 0.001
    13-06-11 15:19:04.607 (0) end_tran COMMIT
    13-06-11 15:19:04.607 (0) end_tran 0 time 0.000
    13-06-11 15:19:04.607 (0) *** elapsed time 0.009

*   13-06-11 15:07:39.282: Time when the application sent the request

*   (1): Sequence number of the SQL statement group. If prepared statement pooling is used, it is uniquely assigned to each SQL statement in the file.

*   CLIENT IP: An IP of an application client

*   CLIENT VERSION: A driver's version of an application client

*   prepare 0: Whether or not it is a prepared statement

*   prepare srv_h_id 1: Prepares the SQL statement as srv_h_id 1.

*   (PC): It is displayed if the data in the plan cache is used.

*   Execute 0 tuple 1 time 0.001: One row is executed. The time spent is 0.001 seconds.

*   auto_commit/auto_rollback: Automatically committed or rolled back. The second auto_commit/auto_rollback is an error code. 0 indicates that the transaction has been completed without an error.

.. _broker_log_top:

broker_log_top
""""""""""""""

The **broker_log_top** utility analyzes the SQL logs which are generated for a specific period. As a result, the information of SQL statements and time execution are displayed in files by order of the longest execution time; the results of SQL statements are stored in **log.top.q** and those of execution time are stored in **log.top.res**, respectively.

The **broker_log_top** utility is useful to analyze a long running query. The syntax is as follows: 
    
::

    broker_log_top [options] sql_log_file_list

* *sql_log_file_list*: names of log files to analyze.

The following is [options] used on **broker_log_top**.

.. program:: broker_log_top

.. option:: -t

    The result is displayed in transaction unit.

.. option:: -F DATETIME

    Specifies the execution start date and time of the SQL statements to be analyzed. The input format is YY-MM-DD[ hh[:mm[:ss[.msec]]]], and the part enclosed by [] can be omitted. If you omit the value, it is regarded as that 0 is input for hh, mm, ss and msec.

.. option:: -T DATETIME

    Specifies the execution end date and time of the SQL statements to be analyzed. The input format is the same with the *DATE* in the **-F** options.

All logs are displayed by SQL statement if any option is not specified.
    
The following sets the search range to milliseconds 

::

    broker_log_top -F "13-01-19 15:00:25.000" -T "13-01-19 15:15:25.180" log1.log
    
The part where the time format is omitted is set to 0 by default. This means that -F "13-01-19 00:00:00.000" -T "13-01-20 00:00:00.000" is input. 

::

    broker_log_top -F "13-01-19" -T "13-01-20" log1.log

The following logs are the results of executing the broker_log_top utility; logs are generated from Nov. 11th to Nov. 12th 2013, and it is displayed in the order of the longest execution of SQL statements. Each month and day are separated by a hyphen (-) when specifying period. Note that "\*.sql.log" is not recognized so the SQL logs should be separated by a white space on Windows. 

::

    --Execution broker_log_top on Linux
    % broker_log_top -F "13-11-11" -T "13-11-12" -t *.sql.log

    query_editor_1.sql.log
    query_editor_2.sql.log
    query_editor_3.sql.log
    query_editor_4.sql.log
    query_editor_5.sql.log

    --Executing broker_log_top on Windows
    % broker_log_top -F "13-11-11" -T "13-11-12" -t query_editor_1.sql.log query_editor_2.sql.log query_editor_3.sql.log query_editor_4.sql.log query_editor_5.sql.log

The **log.top.q** and **log.top.res** files are generated in the same directory where the analyzed logs are stored when executing the example above; 
In the **log.top.q** file, you can see each SQL statement, and its line number. In the **log.top.res** file, you can see the minimum execution time, the maximum execution time, the average execution time, and the number of execution queries for each SQL statement. 

::

    --log.top.q file
    [Q1]-------------------------------------------
    broker1_6.sql.log:137734
    13-11-11 18:17:59.396 (27754) execute_all srv_h_id 34 select a.int_col, b.var_col from dml_v_view_6 a, dml_v_view_6 b, dml_v_view_6 c , dml_v_view_6 d, dml_v_view_6 e where a.int_col=b.int_col and b.int_col=c.int_col and c.int_col=d.int_col and d.int_col=e.int_col order by 1,2;
    13-11-11 18:18:58.378 (27754) execute_all 0 tuple 497664 time 58.982
    .
    .
    [Q4]-------------------------------------------
    broker1_100.sql.log:142068
    13-11-11 18:12:38.387 (27268) execute_all srv_h_id 798 drop table list_test;
    13-11-11 18:13:08.856 (27268) execute_all 0 tuple 0 time 30.469

    --log.top.res file

                  max       min        avg   cnt(err)
    -----------------------------------------------------
    [Q1]        58.982    30.371    44.676    2 (0)
    [Q2]        49.556    24.023    32.688    6 (0)
    [Q3]        35.548    25.650    30.599    2 (0)
    [Q4]        30.469     0.001     0.103 1050 (0)

.. _cubrid_replay:

cubrid_replay 
""""""""""""" 
  
**cubrid_replay** utility replays the SQL log in the broker and outputs the results sorted in order from the large difference(from the slower query than the existing one) by comparing the difference in the execution time of playback and the existing execution time.

This utility plays back the queries that are logged in the SQL log, but does not execute the queries to change the data. If any options are not given, only SELECT queries are run; if **-r** option is given, it changes the UPDATE and DELETE queries into SELECT queries and runs them.

This utility can be used to compare the performance between two different hosts; for example, there can be a performance difference for a same query between master and slave even if their h/w specs are the same.

:: 
  
    cubrid_replay -I <broker_host> -P <broker_port> -d <db_name> [options] <sql_log_file> <output_file> 
     
*   *broker_host*: IP address or host name of the CUBRID broker
*   *broker_port*: Port number of the CUBRID broker
*   *db_name*: The name of database to run the query
*   *sql_log_file*: SQL log file of the CUBRID broker($CUBRID/log/broker/sql_log/\*.log, \*.log.bak) 
*   *output_file*: File name to save the execution result
  
The following is [options] used in **cubrid_replay**.

.. program:: cubrid_replay 
  
.. option:: -u DB_USER 
  
    Specifies the DB account(default: public).

.. option:: -p DB_PASSWORD 
  
    Specifies database password
    
.. option:: -r 
  
    Changes UPDATE and DELETE queries into SELECT queries
  
.. option:: -h SECOND 
  
    Specifies the term to wait between queries to run(default: 0.01 sec)
  
.. option:: -D SECOND
  
    The queries are output to *output_file* only when the specified term is bigger than (replayed execution time - previous execution time)(default: 0.01 sec).

.. option:: -F DATETIME 
  
    Specifies the execution start date and time of the SQL statements to be replayed. The input format is YY[-MM[-DD[ hh[:mm[:ss[.msec]]]]]], and the part enclosed by [] can be omitted. If you omit the value, it is regarded as that 01 is input for MM and DD, and 0 is input for hh, mm, ss and msec.

.. option:: -T DATETIME 
  
    Specifies the execution end date and time of the SQL statements to be replayed. The input format is the same with the *DATE* in the **-F** options.

:: 
  
    $ cubrid_replay -I testhost -P 33000 -d testdb -u dba -r testdb_1_11_1.sql.log.bak output.txt 
  
If you run the above command, the summary of execution result is displayed on the console.
  
:: 
     
    ------------------- Result Summary -------------------------- 
    * Total queries : 153103 
    * Skipped queries (see skip.sql) : 5127 
    * Error queries (see replay.err) : 30 
    * Slow queries (time diff > 0.000 secs) : 89987 
    * Max execution time diff : 0.016 
    * Avg execution time diff : -0.001 
     
    cubrid_replay run time : 245.308417 sec 
  
*   Total queries: Number of total queries within the specified date and time. They include DDL and DML
*   Skipped queries: Number of queries which cannot be changed from UPDATE/DELETE into SELECT when **-r** option is specified. These queries are saved into skip.sql
*   Slow queries: Number of queries of which execution time difference is bigger than the specified value by **-D** option(the replayed execution time is slower than the previous execution time plus the specified value). If you omit the **-D** option, this option value is specified as 0.01 second
*   Max execution time diff: The biggest value among the differences of the execution time(unit: sec)
*   Avg execution time diff: Average value of the differences of the execution time(unit: sec)
*   cubrid_replay run time: Execution time of this utility

"Skipped queries" are the cases which query-transform from UPDATE/DELETE to SELECT is impossible by the internal reason; the queries which are written to skip.sql are needed to check separately.

Also, you should consider that the execution time of the transformed queries does not include the data modification time.

In the *output.txt* file, SQLs that the replayed SQL execution time is slower than the SQL execution time in SQL log are written. That is, {(the replayed SQL execution time) - {(the execution time in SQL log) + (the specified time by **-D** option)} is sorted in descending order. Because **-r** option is used, UPDATE/DELETE is rewritten into SELECT and run.

:: 
  
    EXEC TIME (REPLAY / SQL_LOG / DIFF): 0.003 / 0.001 / 0.002 
    SQL: UPDATE NDV_QUOTA_INFO SET last_mod_date = now() , used_quota = ( SELECT IFNULL(sum(file_size),0) FROM NDV_RECYCLED_FILE_INFO WHERE user_id = ? ) + ( SELECT IFNULL(sum(file_size),0) FROM NDV_FILE_INFO WHERE user_id = ? ) WHERE user_id = ? /+shard_val(6900403)/ /* SQL : NDVMUpdResetUsedQuota */ 
    REWRITE SQL: select NDV_QUOTA_INFO, class NDV_QUOTA_INFO, cast( SYS_DATETIME as datetime), cast((select ifnull(sum(NDV_RECYCLED_FILE_INFO.file_size), 0) from NDV_RECYCLED_FILE_INFO NDV_RECYCLED_FILE_INFO where (NDV_RECYCLED_FILE_INFO.user_id= ?:0 ))+(select ifnull(sum(NDV_FILE_INFO.file_size), 0) from NDV_FILE_INFO NDV_FILE_INFO where (NDV_FILE_INFO.user_id= ?:1 )) as bigint) from NDV_QUOTA_INFO NDV_QUOTA_INFO where (NDV_QUOTA_INFO.user_id= ?:2 ) 
    BIND 1: 'babaemo' 
    BIND 2: 'babaemo' 
    BIND 3: 'babaemo' 
  
*   EXEC TIME: (replay time / execution time in the SQL log / difference between the two execution times) 
*   SQL: The original SQL which exists in the SQL log of the broker
*   REWRITE SQL: Transformed SELECT queries from UPDATE/DELETE queries by **-r** option.

.. note:: broker_log_runner is deprecated from 9.3. Therefore, instead of broker_log_runner, use cubrid_replay.

.. _cas-error:
        
CAS Error
---------

CAS error is an error which occurs in broker application server(CAS), and it can happen on all applications which access to CAS with drivers.

Below shows the CAS error code table. CCI and JDBC's error messages can be different each other on the same CAS error code.
If there is only one message, they are the same, but if there are two messages, then the first one is CCI error message and the second one is JDBC error message.

+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| Error Code Name(Error Number)                    | Error Message (CCI / JDBC)                                          | Note                                                                                                                 |
+==================================================+=====================================================================+======================================================================================================================+
| CAS_ER_INTERNAL(-10001)                          |                                                                     |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NO_MORE_MEMORY(-10002)                    |  Memory allocation error                                            |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_COMMUNICATION(-10003)                     |  Cannot receive data from client / Communication error              |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_ARGS(-10004)                              |  Invalid argument                                                   |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_TRAN_TYPE(-10005)                         |  Invalid transaction type argument / Unknown transaction type       |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_SRV_HANDLE(-10006)                        |  Server handle not found / Internal server error                    |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NUM_BIND(-10007)                          |  Invalid parameter binding value argument / Parameter binding error | The number of data to be bound does not match with the number of data to be transferred.                             |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_UNKNOWN_U_TYPE(-10008)                    |  Invalid T_CCI_U_TYPE value / Parameter binding error               |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_DB_VALUE(-10009)                          |  Cannot make DB_VALUE                                               |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_TYPE_CONVERSION(-10010)                   |  Type conversion error                                              |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_PARAM_NAME(-10011)                        |  Invalid T_CCI_DB_PARAM value / Invalid database parameter name     | The name of the system parameter is not valid.                                                                       |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NO_MORE_DATA(-10012)                      |  Invalid cursor position / No more data                             |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_OBJECT(-10013)                            |  Invalid oid / Object is not valid                                  |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_OPEN_FILE(-10014)                         |  Cannot open file / File open error                                 |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_SCHEMA_TYPE(-10015)                       |  Invalid T_CCI_SCH_TYPE value / Invalid schema type                 |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_VERSION(-10016)                           |  Version mismatch                                                   | The DB server version does not compatible with the client (CAS) version.                                             |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_FREE_SERVER(-10017)                       |  Cannot process the request. Try again later                        | The CAS which handles connection request of applications cannot be assigned.                                         |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NOT_AUTHORIZED_CLIENT(-10018)             |  Authorization error                                                | Access is denied.                                                                                                    |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_QUERY_CANCEL(-10019)                      |  Cannot cancel the query                                            |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NOT_COLLECTION(-10020)                    |  The attribute domain must be the set type                          |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_COLLECTION_DOMAIN(-10021)                 |  Heterogeneous set is not supported /                               |                                                                                                                      |
|                                                  |  The domain of a set must be the same data type                     |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NO_MORE_RESULT_SET(-10022)                |  No More Result                                                     |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_INVALID_CALL_STMT(-10023)                 |  Illegal CALL statement                                             |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_STMT_POOLING(-10024)                      |  Invalid plan                                                       |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_DBSERVER_DISCONNECTED(-10025)             |  Cannot communicate with DB Server                                  |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_MAX_PREPARED_STMT_COUNT_EXCEEDED(-10026)  |  Cannot prepare more than MAX_PREPARED_STMT_COUNT statements        |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_HOLDABLE_NOT_ALLOWED(-10027)              |  Holdable results may not be updatable or sensitive                 |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_HOLDABLE_NOT_ALLOWED_KEEP_CON_OFF(-10028) |  Holdable results are not allowed while KEEP_CONNECTION is off      |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NOT_IMPLEMENTED(-10100)                   |  None / Attempt to use a not supported service                      |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_SSL_TYPE_NOT_ALLOWED(-10103)              |  None / The requested SSL mode is not permitted                     |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_IS(-10200)                                |  None / Authentication failure                                      |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+

.. _gateway:

Gateway
=======

Starting Gateway
----------------

Enter the command below to start the gateway.

::

    $ cubrid gateway start
    @ cubrid gateway start
    ++ cubrid gateway start: success

The following message is returned if the gateway is already running. 

::

    $ cubrid gateway start
    @ cubrid gateway start
    ++ cubrid gateway is running.

Stopping Gateway
----------------

Enter the command below to stop the gateway.

::

    $ cubrid gateway stop
    @ cubrid gateway stop
    ++ cubrid gateway stop: success

The following message is returned if the gateway has stopped. 

::

    $ cubrid gateway stop
    @ cubrid gateway stop
    ++ cubrid gateway is not running.

Restarting Gateway
------------------

Enter the command below to restart the whole gateways.

::

    $ cubrid gateway restart

.. _gateway-status:

Checking Gateway Status
-----------------------

The **cubrid gateway status** utility allows you to check the gateway status such as number of completed jobs and the number of standby jobs by providing various options. 

::

    cubrid gateway status [options] [expr]

*   *expr*: A part of the gateway name or "SERVICE=ON|OFF"

Specifying *expr* performs that the status of specific gateways which include *expr* in their names is monitored; specifying no argument means that status of all gateways which are registered in the gateway environment configuration file ( **cubrid_gateway.conf** ) is monitored.

If "SERVICE=ON" is specified on *expr*, only the status of working gateways is displayed; if "SERVICE=OFF" is specified, only the status of stopped gateways is displayed.

The following [options] are available with the **cubrid gateway status** utility. -b, -q, -c, -m, -S, -P and -f are options to define the information to print; -s, -l and -t are options to control printing. All of these are possible to use as combining each other.

.. program:: gateway_status

.. option:: -b

    Displays the status information of a gateway but does not display information on gateway application server.

.. option:: -q

    Displays standby jobs in the job queue.

.. option:: -f

    Displays information of DB and host accessed by gateway.

    If it is used with the **-b** option, additional information on CAS is displayed. But SELECT, INSERT, UPDATE, DELETE, OTHERS items which shown on **-b** option are excluded.
  
    If it is used with the **-P** option, STMT-POOL-RATIO is additionally printed. This item shows the ratio to use statements in the pool when you are using prepared statements.

.. option:: -l SECOND

    The **-l** option is only used with -f option together. It specifies accumulation period (unit: sec.) when displaying the number of application servers whose client status is Waiting or Busy. If it is omitted, the default value (1 second) is specified.

.. option:: -t

    Displays results in tty mode on the screen. The output can be redirected and used as a file. 

.. option:: -s SECOND

    Regularly displays the status of gateway based on specified period. It returns to a command prompt if q is entered.

If you do not specify options or arguments, the status of all gateways is displayed. 

::

    $ cubrid gateway status
    @ cubrid gateway status
    % oracle_gateway
    ----------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS
    ----------------------------------------
     1 28434     0     0 50144 IDLE
     2 28435     0     0 50144 IDLE
     3 28436     0     0 50144 IDLE
     4 28437     0     0 50140 IDLE
     5 28438     0     0 50144 IDLE

*   % oracle_gateway: The gateway name
*   ID: Serial number of CAS within the gateway
*   PID: CAS process ID within the gateway
*   QPS:  The number of queries processed per second
*   LQS: The number of long-duration queries processed per second
*   PSIZE: Size of CAS
*   STATUS: The current status of CAS (BUSY, IDLE, CLIENT_WAIT, CLOSE_WAIT)

The following shows the detail status of gateway for 5 seconds. The display will reset per 5 seconds as the new status information. To escape the display of the status, press <Q>.

::

    $ cubrid gateway status -b -s 5
    @ cubrid gateway status

     NAME                    PID  PORT   AS   JQ    TPS    QPS   SELECT   INSERT   UPDATE   DELETE   OTHERS     LONG-T     LONG-Q   ERR-Q  UNIQUE-ERR-Q  #CONNECT  #REJECT
    =======================================================================================================================================================================
    * oracle_gateway         13200 30000    5    0      0      0        0        0        0        0        0     0/60.0     0/60.0       0             0         0        0

*   NAME: The gateway name
*   PID: Process ID of the gateway
*   PORT: Port number of the gateway
*   AS: The number of CAS
*   JQ: The number of standby jobs in the job queue
*   TPS: The number of transactions processed per second (calculated only when the option is configured to "-b -s <sec>")
*   QPS: The number of queries processed per second (calculated only when the option is configured to "-b -s <sec>")
*   SELECT: The number of SELECT queries after staring of the gateway. When there is an option of "-b -s <sec>", it is updated every time with the number of SELECTs which have been executed during the seconds specified by this option.
*   INSERT: The number of INSERT queries after staring of the gateway. When there is an option of "-b -s <sec>", it is updated every time with the number of INSERTs which have been executed during the seconds specified by this option.
*   UPDATE: The number of UPDATE queries after staring of the gateway. When there is an option of "-b -s <sec>", it is updated every time with the number of UPDATEs which have been executed during the seconds specified by this option.
*   DELETE: The number of DELETE queries after staring of the gateway. When there is an option of "-b -s <sec>", it is updated every time with the number of DELETEs which have been executed during the seconds specified by this option.
*   OTHERS: The number of queries like CREATE and DROP except for SELECT, INSERT, UPDATE, DELETE. When there is an option of "-b -s <sec>", it is updated every time with the number of queries which have been executed during the seconds specified by this option.
*   LONG-T: The number of transactions which exceed LONG_TRANSACTION_TIME. / the value of the LONG_TRANSACTION_TIME parameter. When there is an option of "-b -s <sec>", it is updated every time with the number of transactions which have been executed during the seconds specified by this option.
*   LONG-Q: The number of queries which exceed LONG_QUERY_TIME. / the value of the LONG_QUERY_TIME parameter. When there is an option of "-b -s <sec>", it is updated every time with the number of queries which have been executed during the seconds specified by this option.
*   ERR-Q: The number of queries with errors found. When there is an option of "-b -s <sec>", it is updated every time with the number of errors which have occurred during the seconds specified by this option. 
*   UNIQUE-ERR-Q: The number of queries with unique key errors found. When there is an option of "-b -s <sec>", it is updated every time with the number of unique key errors which have occurred during the seconds specified by this option.
*   #CONNECT: The number of connections that an application client accesses to CAS after starting the gateway. 
*   #REJECT: The count that an application client excluded from ACL IP list is rejected to access a CAS. Regarding ACL setting, see :ref:`limiting-broker-access`.

The following checks the status of gateway whose name includes oracle_gateway and job status of a specific gateway in the job queue with the **-q** option. If you do not specify oracle_gateway as an argument, list of jobs in the job queue for all gateways is displayed. 

::

    % cubrid gateway status -q oracle_gateway
    @ cubrid gateway status
    % oracle_gateway
    ----------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS
    ----------------------------------------
     1 28444     0     0 50144 IDLE
     2 28445     0     0 50140 IDLE
     3 28446     0     0 50144 IDLE
     4 28447     0     0 50144 IDLE
     5 28448     0     0 50144 IDLE

The following monitors the status of a gateway whose name includes oracle_gateway with the **-s** option. If you do not specify oracle_gateway as an argument, monitoring status for all gateways is performed regularly. It returns to a command prompt if q is not entered. 

::

    % cubrid gateway status -s 5 oracle_gateway
    % oracle_gateway
    ----------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS
    ----------------------------------------
     1 28444     0     0 50144 IDLE
     2 28445     0     0 50140 IDLE
     3 28446     0     0 50144 IDLE
     4 28447     0     0 50144 IDLE
     5 28448     0     0 50144 IDLE

With the **-t** option, it display information of TPS and QPS to a file. To cancel displaying, press <Ctrl+C> to stop program.

::

    % cubrid gateway status -b -t -s 1 > log_file

The following views information of server/database accessed by gateway, the last access times of applications, the IP addresses accessed to CAS and the versions of drivers etc.  with the **-f** option.

::

    $ cubrid gateway status -f oracle_gateway
    @ cubrid gateway status
    % oracle_gateway 
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS         LAST ACCESS TIME      DB       HOST   LAST CONNECT TIME       CLIENT IP   CLIENT VERSION    SQL_LOG_MODE   TRANSACTION STIME  #CONNECT  #RESTART
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     1 26946     0     0 51168 IDLE         2011/11/16 16:23:42  demodb  localhost 2011/11/16 16:23:40      10.0.1.101     9.2.0.0062              NONE 2011/11/16 16:23:42         0         0
     2 26947     0     0 51172 IDLE         2011/11/16 16:23:34      -          -                   -          0.0.0.0                                -                   -         0         0
     3 26948     0     0 51172 IDLE         2011/11/16 16:23:34      -          -                   -          0.0.0.0                                -                   -         0         0
     4 26949     0     0 51172 IDLE         2011/11/16 16:23:34      -          -                   -          0.0.0.0                                -                   -         0         0
     5 26950     0     0 51172 IDLE         2011/11/16 16:23:34      -          -                   -          0.0.0.0                                -                   -         0         0

Meaning of each column in code above is as follows:

*   LAST ACCESS TIME: Time when CAS runs or the latest time when an application client accesses CAS
*   DB: Name of a database which CAS accesses most recently
*   HOST: Name of a which CAS accesses most recently
*   LAST CONNECT TIME: Most recent time when CAS accesses a database
*   CLIENT IP: IP of an application clients currently being connected to an application server(CAS). If no application client is connected, 0.0.0.0 is displayed.
*   CLIENT VERSION: A driver's version of an application client currently being connected to a CAS
*   SQL_LOG_MODE: SQL logging mode of CAS. If the mode is same as the mode configured in the gateway, "-" is displayed.
*   TRANSACTION STIME: Transaction start time
*   #CONNECT: The number of connections that an application client accesses to CAS after starting the gateway
*   #RESTART: The number of connection that CAS is re-running after starting the gateway

.. _gaweway-detail:

Enter the command below with the **-b** and **-f** options to display AS(T W B Ns-W Ns-B) and CANCELED additionally.

::

    // The -f option is added upon execution of gateway status information. Configuring Ns-W and Ns-B are displayed as long as N seconds by using the -l.
    % cubrid gateway status -b -f -l 2
    @ cubrid gateway status
    NAME          PID    PSIZE PORT  AS(T W B 2s-W 2s-B) JQ TPS QPS LONG-T LONG-Q  ERR-Q UNIQUE-ERR-Q CANCELED ACCESS_MODE SQL_LOG  #CONNECT #REJECT
    ================================================================================================================================================
    oracle_gateway 16784 56700 30000      5 0 0     0   0   0  16  29 0/60.0 0/60.0      1            1        0          RW     ALL         4       1

Meaning of added columns in code above is as follows:

*   AS(T): Total number of CAS being executed
*   AS(W): The number of CAS in the status of Waiting
*   AS(B): The number of CAS in the status of Busy
*   AS(Ns-W): The number of CAS that the client belongs to has been waited for N seconds.
*   AS(Ns-B): The number of CAS that the client belongs to has been Busy for N seconds.
*   CANCELED: The number of queries have cancelled by user interruption since the gateway starts (if it is used with the **-l** *N* option, it specifies the number of accumulations for *N* seconds).

.. note::

    Gateway is almost similar to broker, see below for more information on gateway

    *   :ref:`limiting-server-access`
    *   :ref:`encrypted_connections`
    *   :ref:`managing_specific_broker`
    *   :ref:`broker-configuration-info`
    *   :ref:`broker-logs`

.. _cubrid-manager-server:

CUBRID Manager Server
=====================

Starting the CUBRID Manager Server
----------------------------------

The following example shows how to start the CUBRID Manager server. 

::

    % cubrid manager start

The following message is returned if the CUBRID Manager server is already running. 

::

    % cubrid manager start
    @ cubrid manager server start
    ++ cubrid manager server is running.

Stopping the CUBRID Manager Server
----------------------------------

The following example shows how to stop the CUBRID Manager server. 

::

    % cubrid manager stop
    @ cubrid manager server stop
    ++ cubrid manager server stop: success

CUBRID Manager Server Log
-------------------------

The logs of CUBRID Manager server are stored in the log/manager directory under the installation directory. There are four types of log files depending on server process of CUBRID Manager.

*   auto_backupdb.log: Backup log about the backup-automated jobs which was reserved by the CUBRID Manager Client
*   auto_execquery.log: Execution log about the query-automated jobs which was reserved by the CUBRID Manager Client
*   cub_js.access.log: Access log regarding the successful logins and tasks in CUBRID Manager Server.
*   cub_js.error.log: Access log regarding the failed logins and tasks in CUBRID Manager Server.

Configuring CUBRID Manager Server
---------------------------------

The configuration file name for the CUBRID Manager server is **cm.conf** and located in the **$CUBRID/conf** directory.
In the CUBRID Manager server configuration file, where parameter names and values are stored, comments are prefaced by "#." Parameter names and values are separated by spaces or an equal sign (=). 
    
This page describes parameters that are specified in the **cm.conf** file.

**cm_port**

    **cm_port** is a parameter used to configure a communication port for the connection between the CUBRID Manager server and the client. The default value is **8001** .

**monitor_interval**

    **monitor_interval** is a parameter used to configure the monitoring interval of **cub_auto** in seconds. The default value is **5** .

**allow_user_multi_connection**

    **allow_user_multi_connection** is a parameter used to have multiple client connections allowed to the CUBRID Manager server. The default value is **YES** . Therefore, more than one CUBRID Manager client can connect to the CUBRID Manager server, even with the same user name.

**server_long_query_time**

    **server_long_query_time** is a parameter used to configure delay reference time in seconds when configuring **slow_query** which is one of server diagnostics items. The default value is **10** . If the execution time of the query performed on the server exceeds this parameter value, the number of the **slow_query** parameters will increase.

**auto_job_timeout**

    **auto_job_timeout** is a parameter used to configure timeout of auto job for cub_auto. The default value is 43200 (12 hour).
 
**mon_cub_auto**

    **mon_cub_auto** is a parameter used to allow cub_js to restart cub_auto process when cub_auto is not running or not. The default value is NO.
 
**token_active_time**

    **token_active_time** is a parameter used to configure timeout of token. The default value is 7200 (2 hour).
 
**support_mon_statistic**

    **support_mon_statistic** is a parameter used to configure monitoring statistic of system or not. The default value is NO.
 
**cm_process_monitor_interval**

    **cm_process_monitor_interval** is an interval time for collecting statistics. The default and the minimum value is 5 (5 minutes).

CUBRID Manager User Management Console
--------------------------------------

The account and password of CUBRID Manager user are used to access the CUBRID Manager server when starting the CUBRID Manager client, distinguishing this user from the database user. CUBRID Manager Administrator (cm_admin) is a CLI tool that manages user information and it executes commands in the console window to manage users. This utility only supports Linux OS.

The following shows how to use the CUBRID Manager (hereafter, CM) Administrator utilities. The utilities can be used through GUI on the CUBRID Manager client. 

::

    cm_admin <utility_name>
    <utility_name>:
        adduser [<option>] <cmuser-name> <cmuser-password>   --- Adds a CM user
        deluser <cmuser-name>   --- Deletes a CM user
        viewuser [<cmuser-name>]   --- Displays CM user information
        changeuserauth [<option>] <cmuser-name>  --- Changes the CM user authority
        changeuserpwd [<option>] <cmuser-name>  --- Changes the CM user password
        adddbinfo [<option>] <cmuser-name> <database-name>  --- Adds database information of the CM user
        deldbinfo <cmuser-name> <database-name>  --- Deletes database information of the CM user
        changedbinfo [<option>] <database-name> number-of-pages --- Changes database information of the CM user

**CM Users**

    Information about CM users consists of the following:

    *   CM user authority: Includes the following information.

        *   The permission to configure broker
        *   The permission to create a database. For now, this authority is only given to the **admin** user.
        *   The permission to monitor status

    *   Database information: A database that a CM user can use
    *   CM user password

    The default user authority of CUBRID Manager is **admin** and its password is admin. Users who has **admin** authority have full administrative controls.

**Adding CM Users**

    The **cm_admin adduser** utility creates a CM user who has been granted a specific authority and has database information. The permissions to configure broker, create a database, and monitor status can be granted to the CM user. 
    
    ::

        cm_admin adduser [options] cmuser-name cmuser-password

    *   **cm_admin**: An integrated utility to manage CUBRID Manager
    *   **adduser**: A command to create a new CM user
    *   *cmuser-name*: Specifies a unique name to a CM user. Usable characters are 0~9, A~Z, a~z and _. Minimum length is 4 and maximum length is 32. If the specified name in *cmuser-name* is identical to the existing one, **cm_admin** will stop creating a new CM user.
    *   *cmuser-password*: A password of a CM user. Usable characters are 0~9, A~Z, a~z and _. Minimum length is 4 and maximum length is 32.

    The following is [options] of **cm_admin adduser**.

    .. program:: cm_admin_adduser

    .. option:: -b, --broker AUTHORITY

        Specifies the broker authority which will be granted to a new CM user.

        You can use **admin**, **none** (default), and **monitor** as *AUTHORITY*

        The following example shows how to create a CM user whose name is *testcm* and password is *testcmpwd* and then configure broker authority to monitor. ::
        
            cm_admin adduser -b monitor testcm testcmpwd
        
    .. option:: -c, --dbcreate AUTHORITY

        Specifies the authority to create a database which will be granted to a new CM user.

        You can use **none** (default) and **admin** as *AUTHORITY*.

        The following example shows how to create a CM user whose name is *testcm* and password is *testcmpwd* and then configure database creation authority to admin.  ::

            cm_admin adduser -c admin testcm testcmpwd

    .. option:: -m, --monitor AUTHORITY

        Specifies the authority to monitor status which will be granted to a new CM user. You can use **admin**, **none** (default), and **monitor** as *AUTHORITY*

        The following example shows how to create a CM user whose name is *testcm* and password is *testcmpwd* and then configure monitoring authority to admin. 

        ::

            cm_admin adduser -m admin testcm testcmpwd

    .. option:: -d, --dbinfo INFO_STRING

        Specifies database information of a new CM user. The format of *INFO_STRING* must be "<dbname>;<uid>;<broker_ip>,<broker_port>".
        The following example shows how to add database information "testdb;dba;localhost,30000" to a CM user named *testcm* .

        ::

            cm_admin adduser -d "testdb;dba;localhost,30000" testcm testcmpwd

**Deleting CM Users**

    The **cm_admin deluser** utility deletes a CM user. ::

        cm_admin deluser cmuser-name

    *   **cm_admin**: An integrated utility to manage CUBRID Manager
    *   **deluser**: A command to delete an existing CM user
    *   *cmuser-name*: The name of a CM user to be deleted

    The following example shows how to delete a CM user named *testcm*. ::

        cm_admin deluser testcm

**Displaying CM User information**

    The **cm_admin viewuser** utility displays information of a CM user. 

    ::

        cm_admin viewuser cmuser-name

    *   **cm_admin**: An integrated utility to manage CUBRID Manager
    *   **viewuser**: A command to display the CM user information
    *   *cmuser-name*: A CM user name. If this value is entered, information only for the specified user is displayed; if it is omitted, information for all CM users is displayed.

    The following example shows how to display information of a CM user named *testcm* . 
    
    ::

        cm_admin viewuser testcm

    The information will be displayed as follows: 

    ::

        CM USER: testcm
          Auth info:
            broker: none
            dbcreate: none
            statusmonitorauth: none
          DB info:
            ==========================================================================================
             DBNAME                                           UID               BROKER INFO             
            ==========================================================================================
             testdb                                           dba               localhost,30000  

**Changing the Authority of CM Users**

    The **cm_admin changeuserauth** utility changes the authority of a CM user. ::

        cm_admin changeuserauth options cmuser-name

    *   **cm_admin**: An integrated utility to manage CUBRID Manager
    *   **changeuserauth**: A command to change the authority of a CM user
    *   *cmuser-name*: The name of a CM user whose authority to be changed

    The following is [options] of **cm_admin changeuserauth**.

    .. program:: cm_admin_changeuserauth

    .. option:: -b, --broker AUTHORITY

        Specifies the broker authority that will be granted to a CM user.
        You can use **admin**, **none**, and **monitor** as *AUTHORITY*.

        The following example shows how to change the broker authority of a CM user named *testcm* to monitor. 
        
        ::
        
            cm_admin changeuserauth -b monitor testcm    
        
    .. option:: -c, --dbcreate

        Specifies the authority to create a database which will be granted to a CM user.
        You can use **admin** and **none** as *AUTHORITY* .

        The following example shows how to change the database creation authority of a CM user named *testcm* to admin. 

        ::

            cm_admin changeuserauth -c admin testcm

    .. option:: -m, --monitor 

        Specifies the authority to monitor status which will be granted to a CM user.
        You can use **admin**, **none**, and **monitor** as *AUTHORITY*.

        The following example shows how to change the monitoring authority of a CM user named *testcm* to admin. 

        ::

            cm_admin changeuserauth -m admin testcm

**Changing the CM User Password**

    The **cm_admin changeuserpwd** utility changes the password of a CM user. 

    ::

        cm_admin changeuserpwd [options] cmuser-name  

    *   **cm_admin**: An integrated utility to manage CUBRID Manager
    *   **changeuserpwd**: A command to change the password of a CM user
    *   *cmuser-name*: The name of a CM user whose password to be changed

    The following is [options] of **cm_admin changeuserpwd**.

    .. option:: -o, --oldpass PASSWORD

        Specifies the existing password of a CM user.

        The following example shows how to change a password of a CM user named *testcm* . 

        ::

            cm_admin changeuserpwd -o old_password -n new_password testcm
        
    .. option:: --adminpass PASSWORD

        The password of an admin user can be specified instead of old CM user's password that you don't know. 

        The following example shows how to change a password of a CM user named *testcm* by using an admin password. 

        ::

            cm_admin changeuserauth --adminpass admin_password -n new_password testcm
        
    .. option:: -n, --newpass PASSWORD

        Specifies a new password of a CM user.

**Adding Database Information to CM Users**

    The **cm_admin adddbinfo** utility adds database information (database name, UID, broker IP, and broker port) to a CM user. ::

        cm_admin adddbinfo options cmuser-name database-name

    *   **cm_admin**: An integrated utility to manage CUBRID Manager
    *   **adddbinfo**: A command to add database information to a CM user
    *   *cmuser-name*: CM user name
    *   *database-name*: The name of a database to be added

    The following example shows how to add a database without specifying any user-defined values to a CM user named *testcm* . 
    
    ::

        cm_admin adddbinfo testcm testdb

    The following is [options] of **cm_admin adddbinfo**.

    .. program:: cm_admin_adddbinfo

    .. option:: -u, --uid ID

        Specifies the ID of a database user to be added. The default value is **dba**.

        The following example shows how to add a database whose name is *testdb* and user ID is *cubriduser* to a CM user named *testcm*. 
        
        ::

            cm_admin adddbinfo -u cubriduser testcm testdb
        
    .. option:: -h, --host IP

        Specifies the host IP of a broker used when clients access a database. The default value is **localhost**.

        The following example shows how to add a database whose name is *testdb* and the host IP of is *127.0.0.1* to a CM user named *testcm*. ::

            cm_admin adddbinfo -h 127.0.0.1 testcm testdb

    .. option:: -p, --port NUMBER

        Specifies the port number of a broker used when clients access a database. The default value: **30000**.

**Deleting database information from CM Users**

    The **cm_admin deldbinfo** utility deletes database information of a specified CM user. ::

        cm_admin deldbinfo cmuser-name database-name

    *   **cm_admin**: An integrated utility to manage CUBRID Manager
    *   **deldbinfo**: A command to delete database information of a CM user
    *   *cmuser-name*: CM user name
    *   *database-name*: The name of a database to be deleted

    The following example shows how to delete database information whose name is *testdb* from a CM user named *testcm*.

    ::

        cm_admin deldbinfo testcm testdb

**Changing Database Information of a CM user**

    The **cm_admin changedbinfo** utility changes database information of a specified CM user. 

    ::

        cm_admin changedbinfo [options] cmuser-name database-name

    *   **cm_admin**: An integrated utility to manage CUBRID Manager
    *   **changedbinfo**: A command to change database information of a CM user
    *   *cmuser-name*: CM user name
    *   *database-name*: The name of a database to be changed

    The following is [options] of **cm_admin changedbinfo**.

    .. program:: cm_admin_changedbinfo

    .. option:: -u, --uid ID

        Specifies the ID of a database user.

        The following example shows how to update user ID information to *uid* in the *testdb* database which belongs to a CM user named *testcm* . 

        ::

            cm_admin changedbinfo -u uid testcm testdb

    .. option:: -h, --host IP

        Specifies the host of a broker used when clients access a database.

        The following example shows how to update host IP information to *10.34.63.132* in the *testdb* database which belongs to a CM user named *testcm* . 

        ::

            cm_admin changedbinfo -h 10.34.63.132 testcm testdb

    .. option:: -p, --port NUMBER

        Specifies the port number of a broker used when clients access a database.

        The following example shows how to update broker port information to *33000* in the *testdb* database which belongs to a CM user named *testcm* .
        
        ::

            cm_admin changedbinfo -p 33000 testcm testdb

.. _cubrid-javasp-server:

CUBRID Java Stored Procedure Server
===================================

Starting CUBRID Java SP Server
------------------------------

The following example shows how to start CUBRID Java SP server for *demodb*.

To start the Java SP server, the **java_stored_procedure parameter** in the CUBRID configuration file (**cubrid.conf**) must set to yes.

::

    % cubrid javasp start demodb

    @ cubrid javasp start: demodb
    ++ cubrid javasp start: success

The following message is returned if CUBRID Java SP server is already running. 

::

    % cubrid javasp start demodb

    @ cubrid javasp start: demodb
    ++ cubrid javasp 'demodb' is running.

For details on other types of errors that may occur when starting the server, see :ref:`cubrid-javasp-server-errors`.

Stopping CUBRID Java SP Server
------------------------------

The following example shows how to stop CUBRID Java SP server for *demodb*. 

::

    % cubrid javasp stop demodb

    @ cubrid javasp stop: demodb
    ++ cubrid javasp stop: success

The following message is returned when CUBRID Java SP server has been stopped already.

::

    % cubrid javasp stop demodb

    @ cubrid javasp stop: demodb
    ++ cubrid javasp 'demodb' is not running.
    ++ cubrid javasp stop: fail

Restarting CUBRID Java SP Server
--------------------------------

The following example shows how to restart CUBRID Java SP server for *demodb*. the server that has already run stops and the server restarts. 

::

    % cubrid javasp restart demodb
    
    @ cubrid javasp stop: demodb
    ++ cubrid javasp stop: success
    @ cubrid javasp start: demodb
    ++ cubrid javasp start: success

Checking CUBRID Java SP Server Status
-------------------------------------

The following example shows how to check the status of a CUBRID Java SP server for *demodb*. 
The database name of Java SP server, which currently running, *demodb* is displayed.
Additionally, The server's PID, port number, and the applied JVM option are shown together.

::

    % cubrid javasp status demodb
    
    @ cubrid javasp status: demodb
    Java Stored Procedure Server (demodb, pid 9220, port 38408)
    Java VM arguments :
    -------------------------------------------------
    -Djava.util.logging.config.file=/path/to/CUBRID/java/logging.properties
    -Xrs
    -------------------------------------------------


.. _cubrid-javasp-with-server:

Starting the CUBRID Java SP Server together when the database server starts
------------------------------------------------------------------------------------------

| If **java_stored_procedure** is set to yes for the corresponding database, 
| When the database server starts/stops, the Java stored procedure server is started/stopped.
| The following is an example of Java stored procedure server and database server both are started simultaneously.

::

    # cubrid.conf

    ...

    [@demodb]
    java_stored_procedure=yes
    
    [@testdb]
    java_stored_procedure=no

    ...

::

    -- demodb's java_stored_procedure is set to yes
    % cubrid server start demodb
    
    @ cubrid server start: demodb

    This may take a long time depending on the amount of restore works to do.
    CUBRID 11.3

    Calling java stored procedure is allowed

::

    -- testdb's java_stored_procedure is set to no
    % cubrid server start testdb
    
    @ cubrid server start: testdb

    This may take a long time depending on the amount of restore works to do.
    CUBRID 11.3

    java_stored_procedure system parameter is not enabled
    Calling java stored procedure is not allowed


.. _cubrid-javasp-server-config:

Configuring for CUBRID Java SP Server
-------------------------------------

.. _cubrid-javasp-environment-configuration:

Environment Configuration for Java Stored Function/Procedure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To use Java-stored functions/procedures in CUBRID, you must have JDK (Java Development Kit) 1.8 64bit installed in the environment where the CUBRID server is installed. 
You can download JDK at the following links

* `OpenJDK 8 <https://openjdk.java.net/projects/jdk8/>`_
* `Oracle JDK 8 <https://www.oracle.com/java/technologies/javase/javase8-archive-downloads.html>`_

Execute the following command to check the JRE version if you have it already installed in the system. ::

    % java -version
    openjdk version "1.8.0_302"
    OpenJDK Runtime Environment (build 1.8.0_302-b08)
    OpenJDK 64-Bit Server VM (build 25.302-b08, mixed mode)

**Windows Environment**

For Windows, CUBRID loads the **jvm.dll** file to run the Java Virtual Machine. CUBRID first locates the **jvm.dll** file from the **PATH** environment variable and then loads it. If it cannot find the file, it uses the Java runtime information registered in the system registry.

You can configure the **JAVA_HOME** environment variable and add the directory in which the Java executable file is located to **Path**, by executing the command as follows: For information on configuring environment variables using GUI, see Installing and Configuring JDBC.

*   An example of configuring the JDK 1.8 environment variables ::

    % set JAVA_HOME=C:\jdk1.8.0
    % set PATH=%PATH%;%JAVA_HOME%\jre\bin\server

If you want to specify the path of Java Virtual Machine (JVM) explicitly including cases to use other vendor's implementation instead of Sun's JVM, add the path of the **jvm.dll** file to the **JVM_PATH** variable during the installation.
CUBRID first looks for the **jvm.dll** file in the **JVM_PATH** variable. if **JVM_PATH** is not set or if the file cannot be loaded, it looks for the file in the **JAVA_HOME** variable as described above.

*   An example of configuring the **JVM_PATH** environment variable ::
    
    % set JVM_PATH=C:\jdk1.8.0\jre\bin\server\libjvm.dll

**Linux/UNIX Environment**

For Linux/UNIX environment, CUBRID loads the **libjvm.so** file to run the Java Virtual Machine. CUBRID first locates the **libjvm.so** file from the **LD_LIBRARY_PATH** environment variable and then loads it. If it cannot find the file, it uses the **JAVA_HOME** environment variable. For Linux, glibc 2.3.4 or later versions are supported. The following example shows how to configure the Linux environment variable (e.g., **.profile**, **.cshrc**, **.bashrc**, **.bash_profile**, etc.).

*   An example of installing JDK 1.8 and configuring the environment variables in a bash shell ::

    % JAVA_HOME=/usr/java/jdk1.8.0
    % LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/amd64:$JAVA_HOME/jre/lib/amd64/server:$LD_LIBRARY_PATH
    % export JAVA_HOME
    % export LD_LIBRARY_PATH

*   An example of installing JDK 1.8 and configuring the environment variables in a csh shell ::

    % setenv JAVA_HOME /usr/java/jdk1.8.0
    % setenv LD_LIBRARY_PATH $JAVA_HOME/jre/lib/amd64:$JAVA_HOME/jre/lib/amd64/server:$LD_LIBRARY_PATH
    % set path=($path $JAVA_HOME/bin .)

If you want to specify the path of Java Virtual Machine (JVM) explicitly including cases to use other vendor's implementation instead of Sun's JVM, add the path of the **libjvm.so** file to the **JVM_PATH** variable during the installation.
The path of the **libjvm.so** file can be different depending on the platform. For example, the path is the **$JAVA_HOME/jre/lib/sparc** directory in a SUN Sparc machine.
CUBRID first looks for the **libjvm.so** file in the **JVM_PATH** variable. if **JVM_PATH** is not set or if the file cannot be loaded, it looks for the file in the **JAVA_HOME** variable as described above.

*   An example of configuring the **JVM_PATH** environment variable ::
    
    % JVM_PATH=/usr/java/jdk1.8.0/jre/lib/amd64/server/libjvm.so
    % export JVM_PATH

.. _cubrid-javasp-system-parameter:

Java SP Server System Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following table shows the server paramters related to Java SP server available in the configuration file (**cubrid.conf**)

+-------------------------------------+--------+----------------+-----+-------+
| Parameter Name                      | Type   | Default        | Min | Max   |
+-------------------------------------+--------+----------------+-----+-------+
| java_stored_procedure               | bool   | no             |     |       |
+-------------------------------------+--------+----------------+-----+-------+
| java_stored_procedure_port          | int    | 0              | 0   | 65535 |
+-------------------------------------+--------+----------------+-----+-------+
| java_stored_procedure_jvm_options   | string |                |     |       |
+-------------------------------------+--------+----------------+-----+-------+

For more details on these paramters, see :ref:`cubrid-conf`.

.. _cubrid-javasp-server-log:

CUBRID Java SP Server Log
-------------------------

The logs of CUBRID Java SP server are stored in the **log/** directory under the installation directory. The following log files are created for CUBRID Java Stored Procedure Server per database.

*   Error Log ($CUBRID/log/[db_name]_java.err)
*   Java Log ($CUBRID/log/[db_name]_java.log)

Error Log
^^^^^^^^^

An error log of the Java SP server for each database is saved into **$CUBRID/log** directory, and and the format of the file name is **<db_name>_java.err**. The extension is **.err**.

::

    demodb_java.err

If any error occurs during starting the Java SP server, the error message is saved into the error log file.

::

    Time: 11/11/20 18:17:15.438 - ERROR *** file ../../src/jsp/jsp_sr.c, line 501 ERROR CODE = -900, Tran = -1, EID = 1
    Java VM library is not found: 
        Failed to get 'JVM_PATH' environment variable.
        Failed to load libjvm from 'JAVA_HOME' environment variable:
            /jre/lib/amd64/server/libjvm.so: cannot open shared object file: No such file or directory
            /lib/server/libjvm.so: cannot open shared object file: No such file or directory.

.. note::

    For more details on what errors can be occured, see :ref:`cubrid-javasp-server-errors`.


Java Log
^^^^^^^^^

An Java log of the JVM in the Java SP server is saved into **$CUBRID/log** directory, and the format of the file name is **<db_name>_java.log**. The extension is **.log**.

::

    demodb_java.log

If any exception during performing java stored procedure/function occurs from JVM, the exception string is saved into the java log.

::

    SEVERE:
    java.lang.NullPointerException
    at Test.testFunction(Test.java:50)
    ...
    at com.cubrid.jsp.StoredProcedure.invoke(StoredProcedure.java:263)
    at com.cubrid.jsp.ExecuteThread.run(ExecuteThread.java:197)

.. _cubrid-javasp-server-errors:

CUBRID Java SP Server Errors
----------------------------

The following are error messages about the errors which can be occurred in starting Java SP server.
Error messages are written to **$CUBRID/log**/\ *<db_name>_java*\ **.err**.

+-------+----------------------------------+-----------------------------------------------------+-----------------------------------------------------------------------------------+
| Error | Error Message                    | Description                                         | Solution                                                                          |
| Code  |                                  |                                                     |                                                                                   |
+=======+==================================+=====================================================+===================================================================================+
| -900  | Java VM library is not found: ?  | CUBRID can't find the JVM library                   | Make sure JAVA_HOME or JVM_PATH variable is set properly.                         |
|       |                                  | from the JAVA_HOME or JVM_PATH variables            | see :ref:`cubrid-javasp-environment-configuration`.                               |
+-------+----------------------------------+-----------------------------------------------------+-----------------------------------------------------------------------------------+
| -901  | Java VM can not be started: ?    | Unexpected internal error occured in JVM library.   | Try installing the JRE again. If you keep getting the error,                      |
|       |                                  | The JVM library may be broken, or there may be a    | try installing a different version of the JRE.                                    |
|       |                                  | problem with the $CUBRID/java/jspserver.jar file.   | Try replacing it with the same CUBRID version of $CUBRID/java/jspserver.jar file. |
+-------+----------------------------------+-----------------------------------------------------+-----------------------------------------------------------------------------------+

The following are error messages about the errors which can be occurred when there is a problem with the connection to Java SP server including the case it is not started.
Error messages are written to **$CUBRID/log/broker/error_log**/\ *<broker_name>_<app_server_num>*\ **.err**.

+-------+----------------------------------+-----------------------------------------------------+------------------------------------------------------------------------------------+
| Error | Error Message                    | Description                                         | Solution                                                                           |
| Code  |                                  |                                                     |                                                                                    |
+=======+==================================+=====================================================+====================================================================================+
| -902  | Java VM is not running.          | Java SP server is not started                       | Start Java SP server by **cubrid javasp start <db_name>** command.                 |
|       |                                  |                                                     | see :ref:`cubrid-javasp-server`.                                                   |
+-------+----------------------------------+-----------------------------------------------------+------------------------------------------------------------------------------------+
| -903  | Can't connect Java VM: ?         | Java SP server cannot be connected from CAS.        | Restart the Java SP server. If the restart fails,                                  |
|       |                                  | This can happen for many reasons.                   | try to shutdown **cub_javasp <db_name>** process forcibly                          |
|       |                                  | For example, the Java SP server is unstable,        | with the Linux **kill** command. and restart the server again.                     |
|       |                                  | the server is unreachable from CAS,                 |                                                                                    |
|       |                                  | or the server is killed unexpectedly.               | Check if the port of the Java SP server through **cubrid javasp status <db_name>** |
|       |                                  |                                                     | is reachable from CAS.                                                             |
|       |                                  |                                                     | It could be that a firewall forbids the port. Open the port in the firewall.       |
|       |                                  |                                                     | If required, set **java_stored_procedure_port** and restart the Java SP server     |
|       |                                  |                                                     | see :ref:`connect-to-cubrid-server`.                                               |
+-------+----------------------------------+-----------------------------------------------------+------------------------------------------------------------------------------------+
| -905  | Networking with JVM failed: ?    | CAS received invalid packet from the Java SP server |                                                                                    |
+-------+----------------------------------+-----------------------------------------------------+------------------------------------------------------------------------------------+
