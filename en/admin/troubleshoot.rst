
:meta-keywords: cubrid logging, slow query, error log, deadlock detect, cubrid fail-over, cubrid fail-back
:meta-description: Troubleshoot CUBRID database and High Availability nodes by consulting logs.

***************
Troubleshooting
***************

.. _sql-log-check:

Checking SQL Log
================

SQL log of CAS
--------------

When a specific error occurs, generally you can check SQL logs of broker application server(CAS)

One SQL log file is generated per each CAS; it is hard to find an SQL log in which an error occurred because SQL log files are many when CAS processes are many. However, SQL log file name includes CAS ID in the end part, you can find easily if you know the CAS ID in which an error occurred.

.. note:: SQL log file name in a CAS is <broker_name>_<app_server_num>.sql.log(see :ref:`broker-logs`); <app_server_num> is CAS ID.

Function getting CAS information
--------------------------------
 
:func:`cci_get_cas_info` function or :ref:`cubrid.jdbc.driver.CUBRIDConnection.toString() method in JDBC <jdbc-con-tostring>`) prints out the information including the broker host and CAS ID in which the query is executed when the query is run; with this information, you can find an SQL log file of that CAS easily.

::

    <host>:<port>,<cas id>,<cas process id> 
    e.g. 127.0.0.1:33000,1,12916 

Application log
---------------

If you specify the connection URL for printing out the log in the application, you can check the CAS ID which brought an error when an error occurs in the specific query. The following are examples that an application log is written when an error occurs.

**JDBC application log**
  
:: 
 
    Syntax: syntax error, unexpected IdName [CAS INFO - localhost:33000,1,30560],[SESSION-16],[URL-jdbc:cubrid:localhost:33000:demodb::********:?logFile=driver_1.log&logSlowQueries=true&slowQueryThresholdMillis=5]. 

**CCI application log**

:: 
 
    Syntax: syntax error, unexpected IdName [CAS INFO - 127.0.0.1:33000, 1, 30560]. 

Slow query
----------

When a slow query occurs, you should find the reason of a slow query by an application log and an SQL log of CAS.

To find where the cause exists when slow query occurs(in the application-broker section? or in the broker-DB section?), you should check application log or SQL log of CAS because CUBRID is composed of 3-tiers; application-broker-DB server.

There is a slow query in the application log but that is not printed as slow query in the SQL log of CAS; then there will be a cause to make the speed low in the application-broker section.

Some examples are as below.
 
*   Check if there is a low speed of the network between application and broker.
*   Check if there is a case that CAS was restarted by watching the broker log(exists in **$CUBRID/log/broker** directory). If it is revealed as CASes are not enough, you should enlarge the number of CASes; to do so, the value of :ref:`MAX_NUM_APPL_SERVER <max-num-appl-server>` should be enlarged properly. Also the value of :ref:`max_clients <max_clients>` should be enlarged if needed.

If application log and CAS SQL log show the slow query log together and there is almost no gab between the slow query times of application log and the CAS SQL log, the cause which the query was slow will exist between the broker and DB server. For example, the query execution in the DB server was slow.

There are examples of each application log when a slow query occurs.

**JDBC application log** 
 
:: 
 
    2013-05-09 16:25:08.831|INFO|SLOW QUERY 
    [CAS INFO] 
    localhost:33000, 1, 12916 
    [TIME] 
    START: 2013-05-09 16:25:08.775, ELAPSED: 52 
    [SQL] 
    SELECT * from db_class a, db_class b 
     
**CCI application log** 
 
:: 
 
    2013-05-10 18:11:23.023 [TID:14346] [DEBUG][CONHANDLE - 0002][CAS INFO - 127.0.0.1:33000, 1, 12916] [SLOW QUERY - ELAPSED : 45] [SQL - select * from db_class a, db_class b] 

Slow query information in an application and in a broker is stored in each file when the setting is as following.
     
*   The slow query information in application is stored in application log file when the value of **logSlowQueries** property in the connection URL is set to **yes** and the value of **slowQueryThresholdMillis** is set; it is stored to the application logfile specified with the **logFile** property (see :func:`cci_connect_with_url` and :ref:`jdbc-connection-conf`).

*   The slow query information in broker is stored in the $CUBRID/log/broker/sql_log directory when **SLOW_LOG** of :ref:`broker-configuration` is set to ON and **LONG_QUERY_TIME** is set.

Server Error Log
================

You can get various information from the server error log by setting  **error_log_level** parameter in cubrid.conf. The default of **error_log_level** is **NOTIFICATION**. For how to set this parameter, see :ref:`error-parameters`.

.. 4957

.. 10703 

Detecting Overflow Keys or Overflow Pages
------------------------------------------

When overflow keys or overflow pages occur, **NOTIFICATION** messages are written to the server error log. Through this message, users can detect DB performance became slow because of overflow keys or overflow pages. If possible, overflow keys or overflow pages should not appear. That is, it is better not to use the index on the big size column, and not to define the record size largely.

::

    Time: 06/14/13 19:23:40.485 - NOTIFICATION *** file ../../src/storage/btree.c, line 10617 CODE = -1125 Tran = 1, CLIENT = testhost:csql(24670), EID = 6 
    Created the overflow key file. INDEX idx(B+tree: 0|131|540) ON CLASS hoo(CLASS_OID: 0|522|2). key: 'z ..... '(OID: 0|530|1). 
    ........... 

    Time: 06/14/13 19:23:41.614 - NOTIFICATION *** file ../../src/storage/btree.c, line 8785 CODE = -1126 Tran = 1, CLIENT = testhost:csql(24670), EID = 9 
    Created a new overflow page. INDEX i_foo(B+tree: 0|149|580) ON CLASS foo(CLASS_OID: 0|522|3). key: 1(OID: 0|572|578). 
    ........... 

    Time: 06/14/13 19:23:48.636 - NOTIFICATION *** file ../../src/storage/btree.c, line 5562 CODE = -1127 Tran = 1, CLIENT = testhost:csql(24670), EID = 42 
    Deleted an empty overflow page. INDEX i_foo(B+tree: 0|149|580) ON CLASS foo(CLASS_OID: 0|522|3). key: 1(OID: 0|572|192).

.. 9620

Detecting log recovery time
---------------------------

When DB sever is started or backup volume is restored, you can check the duration of the log recovery by printing out the **NOTIFICATION** messages, the starting time and the ending time of the log recovery, to the server error log or an error log file of restoredb. In these messages, the number of logs and the number of log pages to redo are written together.

:: 
  
    Time: 06/14/13 21:29:04.059 - NOTIFICATION *** file ../../src/transaction/log_recovery.c, line 748 CODE = -1128 Tran = -1, EID = 1 
    Log recovery is started. The number of log records to be applied: 96916. Log page: 343 ~ 5104. 
    ..... 
    Time: 06/14/13 21:29:05.170 - NOTIFICATION *** file ../../src/transaction/log_recovery.c, line 843 CODE = -1129 Tran = -1, EID = 4 
    Log recovery is finished.

.. 6128

Detecting a Deadlock
--------------------

Locks related information is written to the server error log.

::

    demodb_20160202_1811.err
    
          ...
          
    Your transaction (index 1, public@testhost|csql(21541)) timed out waiting on    X_LOCK lock on instance 0|650|3 of class t because of deadlock. You are waiting for user(s) public@testhost|csql(21529) to finish.
    
          ...

          
Detecting the change of HA status
================================= 
  
Detecting the change of HA status can be checked in the cub_master process log. This log file is stored in the **$CUBRID/log** directory as named in *<host_name>.cub_master.err*.
  
Detecting HA split-brain
------------------------
  
When there is an abnormal status that two or more nodes are in charge of master role in HA environment, we call it "split-brain".

To resolve the split-brain status, one of two is dead for itself; cub_master log file of this node includes the following information.
 
:: 

    Time: 05/31/13 17:38:29.138 - ERROR *** file ../../src/executables/master_heartbeat.c, line 714 ERROR CODE = -988 Tran = -1, EID = 19 
    Node event: More than one master detected and local processes and cub_master will be terminated. 
  
    Time: 05/31/13 17:38:32.337 - ERROR *** file ../../src/executables/master_heartbeat.c, line 4493 ERROR CODE = -988 Tran = -1, EID = 20 
    Node event:HA Node Information 
    ================================================================================ 
     * group_id : hagrp host_name : testhost02 state : unknown 
    -------------------------------------------------------------------------------- 
    name priority state score missed heartbeat 
    -------------------------------------------------------------------------------- 
    testhost03 3 slave 3 0 
    testhost02 2 master 2 0 
    testhost01 1 master -32767 0 
    ================================================================================ 

Above example is the information to print out into the cub_master log when testhost02 server detects split-brain status and it is dead for itself.    
     
Detecting Fail-over, Fail-back
------------------------------
  
If fail-over or fail-back occurs, a node changes its role.
  
The following is the log file of the cub_master that is changed as slave node after fail-back or master node after fail-over; it includes the following node information.
  
:: 
  
    Time: 06/04/13 15:23:28.056 - ERROR *** file ../../src/executables/master_heartbeat.c, line 957 ERROR CODE = -988 Tran = -1, EID = 25 
    Node event: Failover completed. 
  
    Time: 06/04/13 15:23:28.056 - ERROR *** file ../../src/executables/master_heartbeat.c, line 4484 ERROR CODE = -988 Tran = -1, EID = 26 
    Node event: HA Node Information 
    ================================================================================ 
     * group_id : hagrp host_name : testhost02 state : master 
    -------------------------------------------------------------------------------- 
    name priority state score missed heartbeat 
    -------------------------------------------------------------------------------- 
    testhost03 3 slave 3 0 
    testhost02 2 to-be-master -4094 0 
    testhost01 1 unknown 32767 0 
    ================================================================================ 
  
Above example is an information which is printed out to the cub_master log; it is the process that the 'testhost02' host changes the role from slave to master because of the fail-over.

Failure on HA Start
===================

The following is examples that replicated DB volumes' restoration is impossible without user intervention.

*   When logs to copy in copylogdb process are deleted from a source node.

*   When archive logs to apply from active server are already deleted.

*   When a restoration of server is failed.

When replicated DB volumes' restoration is impossible like above cases, **"cubrid heartbeat start"** command is failed; for each case, you should fix it properly.


Typical Unrestorable Failure
----------------------------

If server process is the cause of the cases that automatic restoration of DB volumes without user intervention is impossible, that cases will be very various, so descriptions for those are omitted.
The following describes the error messages when **copylogdb** or **applylogdb** process is the cause.

*   When **copylogdb** process is the cause

+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| Cause                                                         | Error message                                                                                    |
+===============================================================+==================================================================================================+
| A log not copied yet is already deleted from the target node. | log writer: failed to get log page(s) starting from page id 80.                                  |
+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| Detected as the other DB's log.                               | Log \"/home1/cubrid/DB/tdb01_cdbs037.cub/tdb01_lgat\" does not belong to the given database.     |
+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+

*   When **applylogdb** process is the cause

+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| Cause                                                         | Error message                                                                                    |
+===============================================================+==================================================================================================+
| Archive logs including logs to apply in replication           | Internal error: unable to find log page 81 in log archives.                                      |
| are already deleted.                                          |                                                                                                  |
|                                                               | Internal error: logical log page 81 may be corrupted.                                            |
+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| Different between db_ha_apply_info catalog time and           | HA generic: Failed to initialize db_ha_apply_info.                                               |
| DB creation time in the current replication logs.             |                                                                                                  |
| That is, it's not the previous log to be being applied.       |                                                                                                  |
+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| Different database locale.                                    | Locale initialization: Active log file(/home1/cubrid/DB/tdb01_cdbs037.cub/tdb01_lgat) charset    |
|                                                               | is not valid (iso88591), expecting utf8.                                                         |
+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+

How to fix when a Failure on HA start
-------------------------------------

================================================================= ==================================================================================================
Status                                                            How to fix                                                         
================================================================= ==================================================================================================
When the source node, the cause of failure, is in master status.  Rebuild replication.
When the source node, the cause of failure, is in slave status.   Initialize replicated logs and db_ha_apply_info catalog then restart.
================================================================= ==================================================================================================
