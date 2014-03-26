.. _control-cubrid-processes:

Controlling CUBRID Processes
============================

CUBRID processes can be controlled by **cubrid** utility.

Controlling CUBRID Service
--------------------------

The following **cubrid** utility syntax shows how to control services registered in the configuration file. One of the following can be specified in <command>: 
**start**, **stop**, **restart**, or **status**; **start** is used to run services; **stop** is used to stop services; **restart** is used to restart services; **status** is used to check status. No additional options or arguments are required. 

::

    cubrid service <command>
    <command>: {start|stop|restart|status}

Controlling Database Server
---------------------------

The following **cubrid** utility syntax shows how to control database server process. 
One of the following can be specified in <command>: **start** is used to run services; **stop** is used to stop services; **restart** is used to restart services; **status** is used to check status. Every command except **status** must have a database name as an argument. 

::

    cubrid server <command> [database_name]
    <command>: {start|stop|restart|status}

Controlling Broker
------------------

The following **cubrid** utility syntax shows how to control CUBRID broker process. 
One of the following can be specified in <command>: 
**start** is used to run services; **stop** is used to stop services; 
**restart** is used to restart services; **status** is used to check status;  
**acl** is used to limit broker access; 
**on**/**off** is used to enable/disable the specified broker; 
**reset** is used to reset the connection to broker; 
**info** is used to display the broker configuration information; 
**getid** is used to get the SHARD ID(SHARD database ID) with SHARD key.

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

Controlling CUBRID Manager Server
---------------------------------

To use the CUBRID Manager, the Manager server must be running where database server is running. The following **cubrid** utility syntax shows how to control the CUBRID Manager processes. One of the following can be specified in *command*: **start**, **stop**, or **status**; **start** is used to run services; **stop** is used to stop services; **status** is used to check status. 

::

    cubrid manager <command>
    <command>: {start|stop|status}


Controlling CUBRID HA
---------------------

The following **cubrid heartbeat** utility syntax shows how to use CUBRID HA. One of the following can be specified in *command*: 
**start**, **stop**, **copylogdb**, **applylogdb**, **reload** or **status**; **start** is used to run HA-related processes; **stop** is used to stop them; **copylogdb** is used to start or stop copylogdb process; **applylogdb** is used to start or stop applylogdb process; ; **reload** is used to reload information on HA configuration; **status** is used to check HA status. For details, see :ref:`cubrid-heartbeat`. 

::

    cubrid heartbeat <command>
    <command>: {start|stop|copylogdb|applylogdb|reload|status}
    
.. _control-cubrid-services:

CUBRID Services
===============

Registering Services
--------------------

You can register database servers, CUBRID brokers, CUBRID Manager(s) or CUBRID HA as CUBRID service in the configuration file ( **cubrid.conf** ). To register services, you can input for each **server**, **broker**, **manager** or **heartbeat** as a parameter value, and it is possible to input several values by concatenating them in comma(,).

If you do not register any service, only master process is registered by default. 
It is convenient for you to view status of all related processes at a glance or start and stop the processes at once with the **cubrid** **service** utility once it is registered as CUBRID service. For details on CUBRID HA configuration, see :ref:`cubrid-service-util`.

The following example shows how to register database server and broker as service in the **cubrid.conf** file and enable databases ( *demodb* and *testdb* ) to start automatically at once when CUBRID server starts running.

::

    # cubrid.conf
    ... 

    [service]

    # The list of processes to be started automatically by 'cubrid service start' command
    # Any combinations are available with server, broker, manager and heartbeat.
    service=server,broker

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

The following message is returned if master process is already running. 

::

    % cubrid service start
    
    @ cubrid master start
    ++ cubrid master is running.

The following message is returned if master process fails to run. The example shows that service fails to start due to conflicts of the **cubrid_port_id** parameter value specified in the cubrid.conf file. In such a case, you can resolve the problem by changing the port. If it fails to start even though no port is occupied by process, delete /tmp/CUBRID1523 file and then restart the process. ::

    % cubrid service start
    
    @ cubrid master start
    cub_master: '/tmp/CUBRID1523' file for UNIX domain socket exist.... Operation not permitted
    ++ cubrid master start: fail

After registering service as explained in :ref:`control-cubrid-services`, enter the code below to start the service. You can verify that database server process and broker as well as registered *demodb* and *testdb* are starting at once. 

::

    % cubrid service start
    
    @ cubrid master start
    ++ cubrid master start: success
    @ cubrid server start: demodb

    This may take a long time depending on the amount of restore works to do.
    CUBRID 9.2

    ++ cubrid server start: success
    @ cubrid server start: testdb

    This may take a long time depending on the amount of recovery works to do.
    CUBRID 9.2

    ++ cubrid server start: success
    @ cubrid broker start
    ++ cubrid broker start: success

Stopping Services
-----------------

Enter code below to stop CUBRID service. If no services are registered by a user, only master process stops and then restarts. 

::

    % cubrid service stop
    @ cubrid master stop
    ++ cubrid master stop: success

Enter code below to stop registered CUBRID service. You can verify that server process, broker process, and master process as well as *demodb* and *testdb* stop at once. 

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


Enter code below to restart registered CUBRID service. You can verify that server process, broker process, and master process as well as *demodb* and *testdb* stop and then restart at once. 

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
    @ cubrid master stop
    ++ cubrid master stop: success
    @ cubrid master start
    ++ cubrid master start: success
    @ cubrid server start: demodb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 9.2

    ++ cubrid server start: success
    @ cubrid server start: testdb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 9.2

    ++ cubrid server start: success
    @ cubrid broker start
    ++ cubrid broker start: success

Managing Service Status
-----------------------

The following example shows how to check the status of master process and database server registered. 

::

    % cubrid service status
    
    @ cubrid master status
    ++ cubrid master is running.
    @ cubrid server status

    Server testdb (rel 9.2, pid 31059)
    Server demodb (rel 9.2, pid 30950)

    @ cubrid broker status
    % query_editor
    ----------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS
    ----------------------------------------
     1 15465     0     0 48032 IDLE
     2 15466     0     0 48036 IDLE
     3 15467     0     0 48036 IDLE
     4 15468     0     0 48036 IDLE
     5 15469     0     0 48032 IDLE

    % broker1 OFF

    @ cubrid manager server status
    ++ cubrid manager server is not running.
    
The following message is returned if master process has stopped.

::

    % cubrid service status
    @ cubrid master status    
    ++ cubrid master is not running.

.. _cubrid-utility-logging:
 
cubrid Utility Logging
----------------------
 
CUBRID supports a logging feature about cubrid utility's running result.
 
**Logging contents**
 
The following contents are written to the $CUBRID/log/cubrid_utility.log file.
 
*   All commands through cubrid utilities: only usage, version and parsing errors are not logged.
    
*   Execution results by cubrid utilities: success/failure.
 
*   An error message when failure.
 
**Log file size** 
 
A size of cubrid_utility.log file is expanded by the size specified by **error_log_size** parameter in  cubrid.conf; if this size is enlarged as the specified size, it is backed up as the cubrid_utility.log.bak file. 

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
 
However, in Windows, some cubrid commands are executed through a service process; therefore, a duplicated information can be displayed again.
 
::
 
    13-11-13 17:17:47.638 ( 3820) cubrid service stop
    13-11-13 17:17:47.704 ( 7848) d:\CUBRID\bin\cubrid.exe service stop --for-windows-service
    13-11-13 17:17:56.027 ( 7848) SUCCESS
    13-11-13 17:17:57.136 ( 3820) SUCCESS

And, in Windows, a process run through the service process cannot print out an error message; therefore, for error messages related to the service start, you should definitely check them in the cubrid_utility.log file.

Database Server
===============

Starting Database Server
------------------------

The following example shows how to run *demodb* server.

::

    % cubrid server start demodb
    
    @ cubrid server start: demodb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 9.2

    ++ cubrid server start: success
    
If you start *demodb* server while master process has stopped, master process automatically runs at first and then a specified database server runs.

::

    % cubrid server start demodb
    
    @ cubrid master start
    ++ cubrid master start: success
    @ cubrid server start: demodb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 9.2

    ++ cubrid server start: success

The following message is returned while *demodb* server is running.

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

The following message is returned while *demodb* server has stopped. 

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

    CUBRID 9.2

    ++ cubrid server start: success

Checking Database Server Status
-------------------------------

The following example shows how to check the status of a database server. Names of currently running database servers are displayed. 

::

    % cubrid server status
    
    @ cubrid server status
    Server testdb (rel 9.2, pid 24465)
    Server demodb (rel 9.2, pid 24342)

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

An error log of the database server is saved into $CUBRID/log/server directory, and the format of the file name is  <db_name>_<yyyymmdd>_<hhmi>.err. The extension is ".err".
 
::
 
    demodb_20130618_1655.err

.. note:: 

    For details on how to limit an access to the broker server, see :ref:`limiting-broker-access`.
    
.. _server-event-log:
 
Event Log
^^^^^^^^^
 
If an event which affects on the query performance occurs, this is saved into the event log.

The events which are saved on the event log are SLOW_QUERY, MANY_IOREADS, LOCK_TIMEOUT, DEADLOCK and TEMP_VOLUME_EXPAND.

This log file is saved into the $CUBRID/log/server directory, and the format of the file name is  <db_name>_<yyyymmdd>_<hhmi>.event. The extension is ".event".
 
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
 
    06/13/13 20:56:18.650 - LOCK_TIMEOUT
    waiter:
      client: public@testhost|csql(21529)
      lock:   NX_LOCK (oid=-532|540|16386, table=y, index=pk_y_a)
      sql: update [y] [y] set [a]=400 where ([y].[a]= ?:0 ) using index [y].[pk_y_a](+)
      bind: 1
 
    blocker:
      client: public@testhost|csql(21541)
      lock:   NX_LOCK (oid=-532|540|16386, table=y, index=pk_y_a)
      sql: update [y] [y] set [a]=100 where ([y].[a]= ?:0 ) using index [y].[pk_y_a](+)
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
 
    06/13/13 20:56:17.638 - DEADLOCK
    client: public@testhost|csql(21541)
    hold:
      lock:   NX_LOCK (oid=-532|540|16385, table=y, index=pk_y_a)
      sql: update [y] [y] set [a]=100 where ([y].[a]= ?:0 ) using index [y].[pk_y_a](+)
      bind: 1
 
      lock:   NX_LOCK (oid=-532|540|16386, table=y, index=pk_y_a)
      sql: update [y] [y] set [a]=100 where ([y].[a]= ?:0 ) using index [y].[pk_y_a](+)
      bind: 1
 
      lock:    X_LOCK (oid=0|540|1, table=y)
      sql: update [y] [y] set [a]=100 where ([y].[a]= ?:0 ) using index [y].[pk_y_a](+)
      bind: 1
 
    wait:
      lock:   NX_LOCK (oid=-532|540|16390, table=y, index=pk_y_a)
      sql: update [y] [y] set [a]=300 where ([y].[a]= ?:0 ) using index [y].[pk_y_a](+)
      bind: 5
 
    client: public@testhost|csql(21529)
    hold:
      lock:   NX_LOCK (oid=-532|540|16389, table=y, index=pk_y_a)
      sql: update [y] [y] set [a]=200 where ([y].[a]= ?:0 ) using index [y].[pk_y_a](+)
      bind: 5
 
      lock:   NX_LOCK (oid=-532|540|16390, table=y, index=pk_y_a)
      sql: update [y] [y] set [a]=200 where ([y].[a]= ?:0 ) using index [y].[pk_y_a](+)
      bind: 5
 
      lock:    X_LOCK (oid=0|540|5, table=y)
      sql: update [y] [y] set [a]=200 where ([y].[a]= ?:0 ) using index [y].[pk_y_a](+)
      bind: 5
 
    wait:
      lock:   NX_LOCK (oid=-532|540|16386, table=y, index=pk_y_a)
      sql: update [y] [y] set [a]=400 where ([y].[a]= ?:0 ) using index [y].[pk_y_a](+)
      bind: 1
 
*   client: <DB user>@<application client host name>|<process name>(<process ID>)

    *   hold: an object which is acquiring a lock
    
        *   lock: lock type, table and index names
        *   sql: SQL which is acquiring locks
        *   bind: binding value
        
    *   wait: an object which is waiting a lock
    
        *   lock: lock type, table and index names
        *   sql: SQL which is waiting a lock
        *   bind: binding value
 
On the above output, you can check the application clients and SQLs which brought the deadlock.
      
For more details on locks, see :ref:`lockdb` and :ref:`lock-protocol`.

**TEMP_VOLUME_EXPAND**
 
When a temporary temp volume is expanded, this time is written to the event log. By this log, you can check what transaction brought the expansion of a temporary temp volume.
 
::
  
    06/15/13 18:55:43.458 - TEMP_VOLUME_EXPAND
      client: public@testhost|csql(17540)
      sql: select [x].[a], [x].[b] from [x] [x] where (([x].[a]< ?:0 )) group by [x].[b] order by 1
      bind: 1000
      time: 44
      pages: 24399
 
*   client: <DB user>@<application client host name>|<process name>(<process ID>)
*   sql: SQL which requires a temporary temp volume. All INSERT statement except for INSERT ... SELECT syntax, and DDL statement are not delivered to the DB server, so it is shown as EMPTY
    SELECT, UPDATE and DELETE statements are shown on this item
*   bind: binding value
*   time: a required time to create a temporary temp volume(ms)
*   pages: a required number of pages to create a temporary temp volume

.. _database-server-error:

Database Server Errors
----------------------

Database server error processes use the server error code when an error has occurred. A server error can occur in any task that uses server processes. For example, server errors may occur while using the query handling program or the **cubrid** utility.

**Checking the Database Server Error Codes**

*   Every data definition statement starting with **#define ER_** in the **$CUBRID/include/dbi.h** file indicate the serer error codes.

*   All message groups under "$set 5 MSGCAT_SET_ERROR" in the **CUBRID/msg/en_US (in Korean, ko_KR.eucKR** or **ko_KR.utf8)/cubrid.msg** $ file indicates the server error messages.

When you write code, it is recommended to use the error code name rather than the error code number. For example, the error code number for violating the unique key is -670 or -886. However, users can easily recognize the error when it is written as **ER_BTREE_UNIQUE_FAILED** or **ER_UNIQUE_VIOLATION_WITHKEY** .

::

    $ vi $CUBRID/include/dbi.h

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
    #define ER_LK_DEADLOCK_CYCLE_DETECTED               -1021
    #define ER_LK_DEADLOCK_SPECIFIC_INFO                -1083
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
| ER_LK_DEADLOCK_CYCLE_DETECTED       | -1021                 | A deadlock cycle is detected. ?.                                                                                                                         |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+
| ER_LK_DEADLOCK_SPECIFIC_INFO        | -1083                 | Specific information about deadlock.                                                                                                                     |
+-------------------------------------+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------+

.. _broker:

Broker
======

Starting Broker
---------------

Enter the command below to start the broker. SHARD feature is activated when the broker parameter **SHARD** in cubrid_broker.conf is set to ON.

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

Enter the command below to stop the broker. SHARD feature is stopped when the broker parameter SHARD in cubrid_broker.conf is set to ON.

::

    $ cubrid broker stop
    @ cubrid broker stop
    ++ cubrid broker stop: success

The following message is returned if the broker has stopped. 

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
The status of clients accessed SHARD or the status of SHARD can be displayed by using **-c** and **-m** options when the **SHARD** broker parameter in **cubrid_broker.conf** is set to ON.
Also, by using **-S** option or **-P** option, each shard DB or each proxy information can be displayed.

::

    cubrid broker status [options] [expr]
    
Specifying [expr] performs that the status of specific brokers which include [expr] in their names is monitored; specifying no argument means that status of all brokers which are registered in the broker environment configuration file ( **cubrid_broker.conf** ) is monitored.  

The following [options] are available with the **cubrid broker status** utility. -b, -q, -c, -m, -S, -P and -f are options to define the information to print; -s, -l and -t are options to control printing; -c, -m, -S and -P are options applied when using SHARD feature. All of these are possible to use as combining each other.

.. program:: broker_status

.. option:: -b

    Displays the status information of a broker but does not display information on broker application server. the name of a broker is displayed until 20 characters; if the length is over 20, by adding "..." after the name.

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
    
    If it is used with the **-m** option, more detailed SHARD statistics information is displayed.
    
    If it is used with the **-c** option, for each shard proxy, CLIENT-ID, CLIENT-IP, CONN-TIME, LAST-REQ-TIME, LAST-RES-TIME and LAST-REQ-CODE items are additionally printed.

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
*   QPS:  The number of queries processed per second
*   LQS: The number of long-duration queries processed per second
*   PSIZE: Size of CAS
*   STATUS: The current status of CAS (BUSY, IDLE, CLIENT_WAIT, CLOSE_WAIT)
*   % broker1 OFF: broker1's SERVICE parameter is set to OFF. So, broker1 is not started.

.. note:: 

    ID column which is displayed when checking the SHARD status shows the composition of " (serial number of proxy) – (serial number of shard DB) – (serial number of CAS accessing shard DB)"

::

    $ cubrid broker status
    @ cubrid broker status
    % shard1
    ----------------------------------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS
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
*   ERR-Q: The number of queries with errors found. When there is an option of "-b -s <sec>", it is updated every time with the number of errors which have occurred during the seconds specified by this option. If the SHARD parameter in cubrid_broker.conf is set to ON, the value of ERR-Q is increased even if an error at proxy occurs.
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
*   SQL_LOG_MODE: SQL logging mode of CAS. If the mode is same as the mode configured in the broker, "-" is displayed.
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
*   CANCELED: The number of queries have canceled by user interruption since the broker starts (if it is used with the **-l** *N* option, it specifies the number of accumulations for *N* seconds).

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

The below explains what each column means.

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
    ------------------
    SHARD_ID
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

The below explains the added columns.

*   SHARD: The shard DB information in the proxy

    *   0: The shard DB serial number (shard ID)
    *   [HostA]: The shard access information
    *   [shard1]: The actual DB name

*   RANGE STATISTICS: The shard key query information

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

*   MAX-CLIENT: The maximum number of application clients which can connect to a proxy.
*   CUR-CLIENT: The number of application clients connecting to a proxy.
    
If **-f** option is added when **-c** option is used, more detail client information is displayed.

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

The below explains the added columns.

*   CLIENT-ID: The client serial number sequentially given in the proxy
*   CLIENT-IP: The client IP address
*   CONN-TIME: The time that the proxy has been accessed
*   LAST-REQ-TIME: The time when the last request had been made to the proxy
*   LAST-RES-TIME: The time when the last response has been received from the proxy
*   LAST-REQ-CODE: The code from which the last execution has been completed. Functions related to the main codes are as follows.

    *   0:  end_tran(end transaction)
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

The following explains for the added item.

*   STMT-POOL-RATIO: The ratio to use statements in the pool when you are using prepared statements.

If you use **-b**, **-S** and **-P** options together, it displays as follows.

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
The default value of the **ACCESS_CONTROL** broker parameter is **OFF**. The **ACCESS_CONTROL** and **ACCESS_CONTROL_FILE** parameters must be written under [broker] which common parameters are specified.

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
 
The format of the ip_list_file is as follows:  

::

    <ip_addr>
    ... 

*   <ip_addr>: An IP address that is allowed to access the server. If the last digit of the address is specified as \*, all IP addresses in that rage are allowed to access the broker server.

If a value for **ACCESS_CONTROL** is set to ON and a value for **ACCESS_CONTROL_FILE** is not specified, the broker will only allow the access requests from the localhost. 

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

*   When a user logging into *dbname1* with a *dbuser1* account connects from IPs registered in READIP.txt
*   When a user logging into *dbname1* with a *dbuser2* account connects from IPs registered in WRITEIP1.txt and WRITEIP2.txt
*   When a user logging into every database with a **DBA** account connects from IPs registered in READIP.txt, WRITEIP1.txt, and WRITEIP2.txt

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

Managing a Specific Broker
--------------------------

Enter the code below to run *broker1* only. Note that *broker1* should have already been configured in the shared memory. 

::

    % cubrid broker on broker1

The following message is returned if *broker1* has not been configured in the shared memory. 

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

    broker_changer broker_name [cas_id] parameters value

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

Checking CUBRID SHARD ID
------------------------
**cubrid broker getid** prints out SHARD ID to know in what DB a specific key is included. :: 

    cubrid broker getid -b <broker-name> [-f] shard-key
    
*   -b <*broker-name*>: broker name
*   -f: prints detail information
*   *shard-key*: shard key

The following shows how to print out the SHARD ID for the key 1 within the shard1 broker.

::

    $ cubrid broker getid -b shard1 1
    @ cubrid broker getid
    % shard1
     SHARD_ID : 0, SHARD_KEY: 1

The following shows how to print the detail information using the **-f** option.

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

Connection Test Between a Broker and a DB
----------------------------------------- 

**cubrid broker test** is a command to run the user-defined query to the DBs connected with a specified broker. If a SHARD feature is enabled, you can try to run a query to all SHARD DBs. After running this query, the transaction is rolled back. If you run a query to all SHARD DBs connected with a specified broker by this command, it is possible to confirm the success of a query for each SHARD DB; if you specify the SHARD HASH function, it is possible to confirm what SHARD DB is used for the query.

:: 

    cubrid broker test <broker_name> [-D <db_name>] [-u <db_user>] [-p <db_password>] {-c <query> | -i <input_file>} [-o <output_file>] [-s] [-v] 

* db_name: DB name
* db_user: DB user's account
* db_password: DB user's password
* query: query statement
* input_file: a file which stores input queries
* output_file: a file in which to save the results

The following options are available with the **cubrid broker test** utility.

.. program:: broker_test 

.. option:: -D DB_NAME 
     
    Specify the DB name of the test target. When this option is omitted, the value of SHARD_DB_NAME parameter is used if the value of a SHARD parameter in cubrid_broker.conf is ON. If the value of a SHARD parameter is OFF, an error occurs.
     
.. option:: -u DB_USER 

    Specify the DB account of the test target. When this option is omitted, the value of SHARD_DB_USER parameter is used if the value of a SHARD parameter in cubrid_broker.conf is ON. If the value of a SHARD parameter is OFF, "public" is input in CUBRID, or "root" is input in MySQL.
     
.. option:: -p DB_PASSWORD 

    Specify the DB password of the test target. When this option is omitted, the value of SHARD_DB_PASSWORD parameter is used if the value of a SHARD parameter in cubrid_broker.conf is ON. If the value of a SHARD parameter is OFF, an empty string("") is input in CUBRID and MySQL.
     
.. option:: -c QUERY 

    Specify the query string. **-c** or **-i** option can be used to specify a query. If they are omitted, only the connection information between a broker and a DB is printed.
     
.. option:: -i FILE_NAME 

    Specify the file where you saved the queries to input. **-c** or **-i** option can be used to specify a query.  If they are omitted, only the connection information between a broker and a DB is printed. 
     
.. option:: -o FILE_NAME 

    Specify the file name to save the execution result to be displayed to the console. If this is omitted, the execution result is output only to the console.
     
.. option:: -s 

    A query which includes a SHARD hint is performed only in the SHARD DB. If this is omitted, a query is performed on all SHARD DBs.
     
    If the value of a SHARD parameter is OFF, this option does not affect to the query execution.

.. option:: -v 

    Output an error message and a result set of a SELECT query with the following information.
     
    *   RESULT: Whether an error is returned after the execution of a query. Output [OK | FAIL].
    *   SHARD_ID: an ID of SHAR DB which a query is performed(no output when a value of SHARD parameter is OFF).
    *   ROW COUNT: A number of affected rows by DML, or a number of rows if SELECT query. -1 is returned when a query error.
    *   EXECUTION TIME: an execution time of a query.
    *   QUERY: a query by a user's input.
     
    If this option is omitted, only "RESULT, SHARD_ID, ROW COUNT, EXECUTION TIME, QUERY" are output.

The following are examples to use the above options.

*   Query to DB

    **When the value of SHARD parameter in cubrid_broker.conf is OFF** 
     
    Check if a DB is accessible.

    :: 

        $ cubrid broker test shard1 -D shard -u shard -p shard123 -c "select 1 from db_root where charset = 3" 
     
        @ cubrid broker test 
        @ [OK] CONNECT broker1 DB [demodb] USER [shard] 

        @ SHARD OFF 

        RESULT ROW COUNT EXECUTION TIME QUERY 
        ======================================================== 
        OK 1 0.011341 sec select 1,'a' from db_root where charset = 3 
        @ [OK] QUERY TEST 
         
    **When the value of SHARD parameter in cubrid_broker.conf is ON**

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

*   Check a user's authority

    **When there is no INSERT authority on one of SHARD DBs** 
     
    RESULT is output as FAIL if there is no INSERT authority.
     
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

    RESULT is output as FAIL if there is no UPDATE authority.
    
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

*   Check whether SHARD HASH works well

    Check if a hashing function works well on a special key.

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
         
*   Whether to use -v option or not

    **When using -v option**
     
    When SELECT query succeeds, it returns a resultset; if it fails, it returns an error message.
     
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
         
    **When not using -v option**

    It just returns a success or a failure of a query.
     
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
         
*   Check the value of SHARD key

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

**Checking the Access Log**

    The access log file records information on the application client and is stored to **$CUBRID/log/broker/**\ `<broker_name>`\ **.access** file. If the **LOG_BACKUP** parameter is configured to **ON** in the broker configuration file, when the broker stops properly, the access log file is stored with the date and time that the broker has stopped. For example, if broker1 stopped at 12:27 P.M. on June 17, 2008, an access file named broker1.access.20080617.1227 is generated in the **log/broker** directory. The following example shows an access log.

    The following example and description show an access log file created in the log directory: 
    
    ::

        1 192.168.1.203 - - 972523031.298 972523032.058 2008/06/17 12:27:46~2008/06/17 12:27:47 7118 - -1
        2 192.168.1.203 - - 972523052.778 972523052.815 2008/06/17 12:27:47~2008/06/17 12:27:47 7119 ERR 1025
        1 192.168.1.203 - - 972523052.778 972523052.815 2008/06/17 12:27:49~2008/06/17 12:27:49 7118 - -1

    *   1: ID assigned to the application server of the broker
    *   192.168.1.203: IP address of the application client
    *   972523031.298: UNIX timestamp value when the client's request processing started
    *   2008/06/17 12:27:46: Time when the client's request processing started
    *   972523032.058: UNIX timestamp value when the client's request processing finished
    *   2008/06/17 12:27:47: Time when the client's request processing finished
    *   7118: Process ID of the application server
    *   -1: No error occurred during the request processing
    *   ERR 1025: Error occurred during the request processing. Error information exists in offset=1025 of the error log file

**Checking the Error Log**

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

**Managing the SQL Log**

    The SQL log file records SQL statements requested by the application client and is stored with the name of *<broker_name>_<app_server_num>*. sql.log. The SQL log is generated in the log/broker/sql_log directory when the SQL_LOG parameter is set to ON. Note that the size of the SQL log file to be generated cannot exceed the value set for the SQL_LOG_MAX_SIZE parameter. CUBRID offers the **broker_log_top**, **broker_log_converter**, and **broker_log_runner** utilities to manage SQL logs. Each utility should be executed in a directory where the corresponding SQL log exists.

    The following examples and descriptions show SQL log files: 
    
    ::
    
        13-06-11 15:07:39.282 (0) STATE idle
        13-06-11 15:07:44.832 (0) CLIENT IP 192.168.10.100
        13-06-11 15:07:44.835 (0) CLIENT VERSION 9.2.0.0062
        13-06-11 15:07:44.835 (0) session id for connection 0
        13-06-11 15:07:44.836 (0) connect db demodb user dba url jdbc:cubrid:192.168.10.200:30000:demodb:dba:********: session id 12
        13-06-11 15:07:44.836 (0) DEFAULT isolation_level 3, lock_timeout -1
        13-06-11 15:07:44.840 (0) end_tran COMMIT
        13-06-11 15:07:44.841 (0) end_tran 0 time 0.000
        13-06-11 15:07:44.841 (0) *** elapsed time 0.004
        
        13-06-11 15:07:44.844 (0) check_cas 0
        13-06-11 15:07:44.848 (0) set_db_parameter lock_timeout 1000
        13-06-11 15:09:36.299 (0) check_cas 0
        13-06-11 15:09:36.303 (0) get_db_parameter isolation_level 3
        13-06-11 15:09:36.375 (1) prepare 0 CREATE TABLE unique_tbl (a INT PRIMARY key);
        13-06-11 15:09:36.376 (1) prepare srv_h_id 1
        13-06-11 15:09:36.419 (1) set query timeout to 0 (no limit)
        13-06-11 15:09:36.419 (1) execute srv_h_id 1 CREATE TABLE unique_tbl (a INT PRIMARY key);
        13-06-11 15:09:38.247 (1) execute 0 tuple 0 time 1.827
        13-06-11 15:09:38.247 (0) auto_commit
        13-06-11 15:09:38.344 (0) auto_commit 0
        13-06-11 15:09:38.344 (0) *** elapsed time 1.968
        
        13-06-11 15:09:54.481 (0) get_db_parameter isolation_level 3
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
        
        13-06-11 15:19:04.593 (0) get_db_parameter isolation_level 3
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

    The **broker_log_top** utility analyzes the SQL logs which are generated for a specific period. As a result, the information of SQL statements and time execution are displayed in files by order of the longest execution time; the results of SQL statements are stored in **log.top.q** and those of execution time are stored in **log.top.res**, respectively.

    The **broker_log_top** utility is useful to analyze a long running query. The syntax is as follows: 
        
    ::

        broker_log_top [options] sql_log_file_list

    * *sql_log_file_list*: names of log files to analyze.

    The following is [options] used on **broker_log_top**.

    .. program:: broker_log_top

    .. option:: -t

        The result is displayed in transaction unit.

    .. option:: -F DATE

        This option specifies the execution start date of the SQL statements to be analyzed. The input format is YY[-MM[-DD[ hh[:mm[:ss[.msec]]]]]], and the part enclosed by [] can be omitted. If you omit the value, it is regarded as that 01 is input for MM and DD, and 0 is input for hh, mm, ss and msec.

    :: option:: -T DATE

        This option specifies the execution end date of the SQL statements to be analyzed. The input format is the same with the *DATE* in the **-F** options.

    All logs are displayed by SQL statement if any option is not specified.
        
    The following sets the search range to milliseconds 
    
    ::

        broker_log_top -F "01/19 15:00:25.000" -T "01/19 15:15:25.180" log1.log
        
    The part where the time format is omitted is set to 0 by default. This means that -F "01/19 00:00:00.000" -T "01/20 00:00:00.000" is input. 
    
    ::

        broker_log_top -F "01/19" -T "01/20" log1.log

    The following logs are the results of executing the broker_log_top utility; logs are generated from Nov. 11th to Nov. 12th, and it is displayed in the order of the longest execution of SQL statements. Each month and day are separated by a slash (/) when specifying period. Note that "\*.sql.log" is not recognized so the SQL logs should be separated by a white space on Windows. 
    
    ::

        --Execution broker_log_top on Linux
        % broker_log_top -F "11/11" -T "11/12" -t *.sql.log

        query_editor_1.sql.log
        query_editor_2.sql.log
        query_editor_3.sql.log
        query_editor_4.sql.log
        query_editor_5.sql.log

        --Executing broker_log_top on Windows
        % broker_log_top -F "11/11" -T "11/12" -t query_editor_1.sql.log query_editor_2.sql.log query_editor_3.sql.log query_editor_4.sql.log query_editor_5.sql.log

    The **log.top.q** and **log.top.res** files are generated in the same directory where the analyzed logs are stored when executing the example above; 
    In the **log.top.q** file, you can see each SQL statement, and its line number. In the **log.top.res** file, you can see the minimum execution time, the maximum execution time, the average execution time, and the number of execution queries for each SQL statement. 
    
    ::

        --log.top.q file
        [Q1]-------------------------------------------
        broker1_6.sql.log:137734
        11/11 18:17:59.396 (27754) execute_all srv_h_id 34 select a.int_col, b.var_col from dml_v_view_6 a, dml_v_view_6 b, dml_v_view_6 c , dml_v_view_6 d, dml_v_view_6 e where a.int_col=b.int_col and b.int_col=c.int_col and c.int_col=d.int_col and d.int_col=e.int_col order by 1,2;
        11/11 18:18:58.378 (27754) execute_all 0 tuple 497664 time 58.982
        .
        .
        [Q4]-------------------------------------------
        broker1_100.sql.log:142068
        11/11 18:12:38.387 (27268) execute_all srv_h_id 798 drop table list_test;
        11/11 18:13:08.856 (27268) execute_all 0 tuple 0 time 30.469

        --log.top.res file

                      max       min        avg   cnt(err)
        -----------------------------------------------------
        [Q1]        58.982    30.371    44.676    2 (0)
        [Q2]        49.556    24.023    32.688    6 (0)
        [Q3]        35.548    25.650    30.599    2 (0)
        [Q4]        30.469     0.001     0.103 1050 (0)

    To store SQL logs created in log/broker/sql_log under the installation directory to a separate file, the **broker_log_converter** utility is executed. The syntax of the **broker_log_converter** utility is as follows. The example shows how to store queries in the query_editor_1.sql.log file to the query_convert.in file. 
    
    ::

        broker_log_converter [option] SQL_log_file output_file
        
    * *SQL_log_file*: SQL log file located in $CUBRID/log/broker/sql_log directory. It only saves SQL log when the application sends a query through only a driver, and does not save SQL log when the query is executed through CSQL interpreter.
    * *output_file*: an output file which the input format is followed by **broker_log_runner**

    The following is the [option] used in **broker_log_converter**.

    .. program:: broker_log_converter

    .. option:: -i
        
        It prints QUERY_ID comment in front of the query.
    
    If QUERY_ID comment exists on the query, tracing a query is easier since QUERY_ID information is printed to the SQL log(located in $CUBRID/log/broker/sql_log) of the broker when replaying the queries of **output_file**.
    
    The following example shows how to convert the query in the query_editor_1.sql.log file into the query_convert.in file. 
    
    ::

        % cd CUBRID/log/broker/sql_log
        % broker_log_converter query_editor_1.sql.log query_convert.in

    To re-execute queries stored in the query file which has been created by the **broker_log_converter** utility, execute the **broker_log_runner** utility.
    
    The syntax of the **broker_log_runner** utility is as follows: 
    
    ::

        broker_log_runner -I broker_host -P broker_port -d dbname [options] exec_script_file 
        
    *   *broker_host*: IP address or host name of the CUBRID broker
    *   *broker_port*: Port number of the CUBRID broker
    *   *dbname*: Name of the database against which queries are to be executed  
    *   *exec_script_file*: Name of the file where execution results are to be stored.

    The following is [options] used in **broker_log_runner**.

    .. program:: broker_log_runner

    .. option:: -u NAME

        Database user name (default: **PUBLIC**)
        
    .. option:: -p PASSWORD

        Database password
        
    .. option:: -t NUMBER    

        The number of threads(default: 1)
        
    .. option:: -r COUNT

        The number of times that the query is to be executed (default value: 1)

    .. option:: -o FILE

        Name of the file where execution results are to be stored 
        
    .. option:: -Q
        
        Stores the query plan in the FILE specified in the **-o** option.

    .. option:: -s
        
        Prints the information by the "cubrid statdump" command per each query. See :ref:`statdump`.
        
    .. option:: -a

        Sets autocommit mode as ON.
        
    The following example re-executes the queries saved on *query_convert.in* on *demodb*, and it assumes that the broker IP is specified in 192.168.1.10, and broker port is specified in 30000. 
    
    ::

        % broker_log_runner -I 192.168.1.10  -P 30000 -d demodb -t 2 query_convert.in
        broker_ip = 192.168.1.10
        broker_port = 30000
        num_thread = 2
        repeat = 1
        dbname = demodb
        dbuser = public
        dbpasswd =
        exec_time : 0.001
        exec_time : 0.000
        0.000500 0.000500

    The following example saves the query plan only without running the query. 
    
    ::
        
        % broker_log_runner -I 192.168.1.10 -P 30000 -d demodb -o result -Q query_convert.in
        ... 
        %cat result.0
        -------------- query -----------------
        SELECT * FROM athlete where code=10099;
        cci_prepare exec_time : 0.000
        cci_execute_exec_time : 0.000
        cci_execute:1
        ---------- query plan --------------
        Join graph segments (f indicates final):
        seg[0]: [0]
        seg[1]: code[0] (f)
        seg[2]: name[0] (f)
        seg[3]: gender[0] (f)
        seg[4]: nation_code[0] (f)
        seg[5]: event[0] (f)
        Join graph nodes:
        node[0]: athlete athlete(6677/107) (sargs 0)
        Join graph terms:
        term[0]: (athlete.code=10099) (sel 0.000149768) (sarg term) (not-join eligible) (indexable code[0]) (loc 0)

        Query plan:

        iscan
            class: athlete node[0]
            index: pk_athlete_code term[0]
            cost:  0 card 1

        Query stmt:

        select athlete.code, athlete.[name], athlete.gender, athlete.nation_code, athlete.event from athlete athlete where (athlete.code=  :0 )

        ---------- query result --------------
        10099|Andersson Magnus|M|SWE|Handball|
        -- 1 rows ----------------------------

        cci_end_tran exec_time : 0.000

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
| CAS_ER_UNKNOWN_U_TYPE(-10008)                    |  Invalid T_CCI_U_TYPE value / Parameter binding error               |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_DB_VALUE(-10009)                          |  Cannot make DB_VALUE                                               |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_TYPE_CONVERSION(-10010)                   |  Type conversion error                                              |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_PARAM_NAME(-10011)                        |  Invalid T_CCI_DB_PARAM value / Invalid database parameter name     | The name of the system parameter is not valid.                                                                       |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NO_MORE_DATA(-10012)                      |  Invalid cursor position / No more data                             |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_OBJECT(-10013)                            |  Invalid oid / Object is not valid                                  |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_OPEN_FILE(-10014)                         |  Cannot open file / File open error                                 |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_SCHEMA_TYPE(-10015)                       |  Invalid T_CCI_SCH_TYPE value / Invalid schema type                 |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_VERSION(-10016)                           |  Version mismatch                                                   | The DB server version does not compatible with the client (CAS) version.                                             |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_FREE_SERVER(-10017)                       |  Cannot process the request. Try again later                        | The CAS which handles connection request of applications cannot be assigned.                                         |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NOT_AUTHORIZED_CLIENT(-10018)             |  Authorization error                                                | Access is denied.                                                                                                    |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_QUERY_CANCEL(-10019)                      |  Cannot cancel the query                                            |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NOT_COLLECTION(-10020)                    |  The attribute domain must be the set type                          |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_COLLECTION_DOMAIN(-10021)                 |  Heterogeneous set is not supported /                               |                                                                                                                      |
|                                                  |  The domain of a set must be the same data type                     |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NO_MORE_RESULT_SET(-10022)                |  No More Result                                                     |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_INVALID_CALL_STMT(-10023)                 |  Illegal CALL statement                                             |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_STMT_POOLING(-10024)                      |  Invalid plan                                                       |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_DBSERVER_DISCONNECTED(-10025)             |  Cannot communicate with DB Server                                  |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_MAX_PREPARED_STMT_COUNT_EXCEEDED(-10026)  |  Cannot prepare more than MAX_PREPARED_STMT_COUNT statements        |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_HOLDABLE_NOT_ALLOWED(-10027)              |  Holdable results may not be updatable or sensitive                 |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_HOLDABLE_NOT_ALLOWED_KEEP_CON_OFF(-10028) |  Holdable results are not allowed while KEEP_CONNECTION is off      |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_NOT_IMPLEMENTED(-10100)                   |  None / Attempt to use a not supported service                      |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+
| CAS_ER_IS(-10200)                                |  None / Authentication failure                                      |                                                                                                                      |
+--------------------------------------------------+---------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------+

.. _cubrid-manager-server:

CUBRID Manager Server
=====================

Starting the CUBRID Manager Server
----------------------------------

The following example shows how to start the CUBRID Manager server. 

::

    % cubrid manager start

The following message is returned if the CUBRID Manager server is already running. 

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

*   cub_auto.access.log: Access log of a client that has successfully logged into and out of the CUBRID Manager server
*   cub_auto.error.log: Access log of a client that failed to log into or out of the CUBRID Manager Server
*   cub_js.access.log: Job log processed by the CUBRID Manager server
*   cub_js.error.log: Error log that occurred while the CUBRID Manager server has been processing jobs

Configuring CUBRID Manager Server
---------------------------------

The configuration file name for the CUBRID Manager server is **cm.conf** and located in the **$CUBRID/conf** directory.
In the CUBRID Manager server configuration file, where parameter names and values are stored, comments are prefaced by "#." Parameter names and values are separated by spaces or an equal sign (=). 
    
This page describes parameters that are specified in the **cm.conf** file.

**cm_port**

    **cm_port** is a parameter used to configure a communication port for the connection between the CUBRID Manager server and the client.
    The default value is **8001** . It is used by **cub_auto** and **cm_js** automatically adds 1 to the value specified by **cub_auto** . For example, if **cm_port**
    is set to 8001, **cub_auto** uses the port 8001, and **cub_js** uses 8002. Therefore, to run the CUBRID Manager in an environment where a firewall has been installed, you must open two ports which will be actually used.

**monitor_interval**

    **monitor_interval** is a parameter used to configure the monitoring interval of **cub_auto** in seconds. The default value is **5** .

**allow_user_multi_connection**

    **allow_user_multi_connection** is a parameter used to have multiple client connections allowed to the CUBRID Manager server. The default value is **YES** . Therefore, more than one CUBRID Manager client can connect to the CUBRID Manager server, even with the same user name.

**server_long_query_time**

    **server_long_query_time** is a parameter used to configure delay reference time in seconds when configuring **slow_query** which is one of server diagnostics items. The default value is **10** . If the execution time of the query performed on the server exceeds this parameter value, the number of the **slow_query** parameters will increase.

**cm_target**

    **cm_target** is a parameter used to display appropriate menus of the CUBRID Manager depending on the service being provided where the broker and the database server have been separated. The default value means the environment where both broker and database server have been installed. You can set required values as follows:

    *   **cm_target broker, server**: Both broker and database server exist.
    *   **cm_target broker**: Only broker exists.
    *   **cm_target server**: Only database server exists.

    If you set broker only, broker-related menus will be shown; if you set database server only, server-related menus will be displayed.

    If you right-click the host in the navigation tree and then select [Properties], you can check the setting information under [Host Information].

    .. image:: /images/image10.jpg

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
        *   The permission to create a database. For now, this authority is only given to the **admin** user.
        *   The permission to monitor status

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

        Specifies the authority to create a database which will be granted to a new CM user.

        You can use **none** (default) and **admin** as *AUTHORITY*.

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
        You can use **admin**, **none**, and **monitor** as *AUTHORITY* .

        The following example shows how to change the broker authority of a CM user named *testcm* to monitor. 
        
        ::
        
            cm_admin changeuserauth -b monitor testcm    
        
    .. option:: -c, --dbcreate

        Specifies the authority to create a database which will be granted to a CM user.
        You can use **admin** and **none** as *AUTHORITY* .

        The following example shows how to change the database creation authority of a CM user named *testcm* to admin. 
        
        ::

            cm_admin changeuserauth -c admin testcm

    .. option:: -m, --monitor 

        Specifies the authority to monitor status which will be granted to a CM user.
        You can use **admin**, **none**, and **monitor** as *AUTHORITY* .

        The following example shows how to change the monitoring authority of a CM user named *testcm* to admin. 
        
        ::

            cm_admin changeuserauth -m admin testcm

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

        The following example shows how to add a database whose name is *testdb* and user ID is *uid* to a CM user named *testcm*. 
        
        ::

            cm_admin adddbinfo -u uid testcm testdb
        
    .. option:: -h, --host IP

        Specifies the host IP of a broker used when clients access a database. The default value is **localhost**.

        The following example shows how to add a database whose name is *testdb* and the host IP of is *127.0.0.1* to a CM user named *testcm*. ::

            cm_admin adddbinfo -h 127.0.0.1 testcm testdb

    .. option:: -p, --port

        Specifies the port number of a broker used when clients access a database. The default value: **30000** .

**Deleting database information from CM Users**

    The **cm_admin deldbinfo** utility deletes database information of a specified CM user. ::

        cm_admin deldbinfo cmuser-name database-name

    *   **cm_admin**: An integrated utility to manage CUBRID Manager
    *   **deldbinfo**: A command to delete database information of a CM user
    *   *cmuser-name*: CM user name
    *   *database-name*: The name of a database to be deleted

    The following example shows how to delete database information whose name is *testdb* from a CM user named *testcm* . ::

        cm_admin deldbinfo  testcm testdb

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

        The following example shows how to update user ID information to *uid* in the *testdb* database which belongs to a CM user named *testcm* . ::
        
            cm_admin changedbinfo -u uid testcm testdb
        
    .. option:: -h, --host IP

        Specifies the host of a broker used when clients access a database.

        The following example shows how to update host IP information to *10.34.63.132* in the *testdb* database which belongs to a CM user named *testcm* . ::

            cm_admin changedbinfo -h 10.34.63.132 testcm testdb

    .. option:: -p, --port NUMBER

        Specifies the port number of a broker used when clients access a database.

        The following example shows how to update broker port information to *33000* in the *testdb* database which belongs to a CM user named *testcm* . ::

            cm_admin changedbinfo -p 33000 testcm testdb
