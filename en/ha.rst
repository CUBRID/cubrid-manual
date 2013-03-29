*********
CUBRID HA
*********

High Availability (HA) refers to a feature to provide uninterrupted service in the event of hardware, software, or network failure. This ability is a critical element in the network computing area where services should be provided 24/7. An HA system consists of more than two server systems, each of which provides uninterrupted services, even when a failure occurs in one of them.

CUBRID HA is an implementation of High Availability. CUBRID HA ensures database synchronization among multiple servers when providing service. When an unexpected failure occurs in the system which is operating services, this feature minimizes the service down time by allowing the other system to carry out the service automatically.

CUBRID HA is in a shared-nothing structure. To synchronize data from an active server to a standby server, CUBRID HA executes the following two steps.

#. Transaction log multiplexing: Replicates the transaction logs created by an active server to another node in real time.
#. Transaction log reflection: Analyzes replicated transaction logs in real time and reflects the data to a standby server.

CUBRID HA executes the steps described above in order to always maintain data synchronization between an active server and a standby server. For this reason, if an active server is not working properly because of a failure occurring in the master node that had been providing service, the standby server of the slave node provides service instead of the failed server. CUBRID HA monitors the status of the system and CUBRID in real time. It uses heartbeat messages to execute an automatic failover when a failure occurs.

.. image:: /images/image13.png

CUBRID HA Concept
=================

Groups and Nodes
----------------

A node is a logical unit that makes up CUBRID HA. It can become one of the following nodes according to its status: master node, slave node, or replica node.

*   **Master node** : A node to be replicated. It provides all services which are read, write, etc. using an active server.

*   **Slave node** : A node that has the same information as a master node. Changes made in the master node are automatically reflected to the slave node. It provides the read service using a standby server, and a failover will occur when the master node fails.

*   **Replica node** : A node that has the same information as a master node. Changes made in the master node are automatically reflected to the replica node. It provides the read service using a standby server, and no failover will occur when the master node fails.

The CUBRID HA group consists of the nodes described above. You can configure the members of this group by using the **ha_node_list** and **ha_replica_list** in the **cubrid_ha.conf** file. Nodes in a group have the same information. They exchange status checking messages periodically and a failover will occurs when the master node fails.

A node includes the master process (cub_master), the database server process (cub_server), the replication log copy process (copylogdb), the replication log reflection process (applylogdb), etc.

.. image:: /images/image14.png

Processes
---------

A CUBRID HA node consists of one master process (cub_master), one or more database server processes (cub_server), one or more replication log copy processes (copylogdb), and one or more replication log reflection processes (applylogdb). When a database is configured, database server processes, replication log copy processes, and replication log reflection processes will start. Because copy and reflection of a replication log are executed by different processes, the delay in replicating reflections does not affect the transaction that is being executed.

*   **Master process (cub_master)** : Exchanges heartbeat messages to control the internal management processes of CUBRID HA.

*   **Database server process (cub_server)** : Provides services such as read or write to the user. For details, see :ref:`ha-server`.

*   **Replication log copy process (copylogdb)** : Copies all transaction logs in a group. When the replication log copy process requests a transaction log from the database server process of the target node, the database server process returns the corresponding log. The location of copied transaction logs can be configured in the **REPL_LOG_HOME** of **cubrid-ha**. Use :ref:`cubrid-applyinfo` utility to verify the information of copied replication logs. The replication log copy process has following three modes: SYNC, SEMISYNC, and ASYNC. You can configure it with the **LW_SYNC_MODE** of **cubrid-ha**. For details on these modes, see :ref:`log-multiplexing`.

.. image:: /images/image15.png

*   **Replication log reflection process (applylogdb)** : Reflects the log that has been copied by the replication log copy process to a node. The information of reflected replications is stored in the internal catalog (db_ha_apply_info). You can use the :ref:`cubrid-applyinfo` utility to verify this information.

.. image:: /images/image16.png

.. _ha-server:

Servers
-------

Here, the word "server" is a logical representation of database server processes. Depending on its status, a server can be either an active server or a standby server.

*   **Active server** : A server that belongs to a master node; the status is active. An active server provides all services, including read, write, etc. to the user.
*   **Standby server** : A standby server that belongs to a non-master node; the status is standby. A standby server provides only the read service to the user.

The server status changes based on the status of the node. You can use the :ref:`cubrid-changemode` utility to verify server status. The maintenance mode exists for operational convenience and you can change it by using the **cubrid changemode** utility.

.. image:: /images/image17.png

*   **active** : The status of servers that run on a master node is usually active. In this status, all services including read, write, etc. are provided.
*   **standby** : The status of servers that run on a slave node or a replica node is standby. In this status, only the read service is provided.
*   **maintenance** : The status of servers can be manually changed for operational convenience is maintenance. In this status, only a csql can access and no service is provided to the user.
*   **to-be-active** : The status in which a standby server will become active for reasons such as failover, etc. is to-be-active. In this status, servers prepare to become active by reflecting transaction logs from the existing master node to its own server. The node in this status can accept only SELECT query.
*   Other : This status is internally used.

When the node status is changed, on cub_master process log and cub_server process log, following error messages are saved. But, they are saved only when the value of **error_log_level** in cubrid.conf is **error** or less.

* The following log information of cub_master process is saved on $CUBRID/log/<hostname>_master.err file. ::

    HA generic: Send changemode request to the server. (state:1[active], args:[cub_server demodb ], pid:25728).
    HA generic: Receive changemode response from the server. (state:1[active], args:[cub_server demodb ], pid:25728).

* The following log information of cub_server is saved on $CUBRID/log/server/<db_name>_<date>_<time>.err file. ::

    Server HA mode is changed from 'to-be-active' to 'active'.


heartbeat Message
-----------------

As a core element to provide HA, it is a message exchanged among master, slave, and replica nodes to monitor the status of other nodes. A master process periodically exchanges heartbeat messages with all other master processes in the group. A heartbeat message is exchanged through the UDP port configured in the **ha_port_id** parameter of **cubrid_ha.conf**. The exchange interval of heartbeat messages is determined by an internally configured value.

When the master node fails, a failover occurs to a slave node.

.. image:: /images/image18.png

failover and failback
---------------------

A failover means that the highest priority slave node automatically becomes a new master node when the original master node fails to provide services due to a failure. A master process calculates scores for all nodes in the CUBRID HA group based on the collected information, promotes slave nodes to master modes when it is necessary, and then notifies the management process of the changes it has made.

A failback means that the previously failed master node automatically becomes a master node back after the failure node is restored. The CUBRID HA does not currently support this functionality.

.. image:: /images/image19.png

If a heartbeat message fails to deliver, a failover will occur. For this reason, servers with unstable connection may experience failover even though no actual failures occur. To prevent a failover from occurring in the situation described above, configure **ha_ping_ports**. Configuring **ha_ping_ports** will send a ping message to a node specified in **ha_ping_ports** in order to verify whether the network is stable or not when a heartbeat message fails to deliver. For details on configuring **ha_ping_ports**, see :ref:`cubrid-ha-conf`.

.. _broker-mode:

Broker Mode
-----------

A broker can access a server with one of the following modes: **Read Write**, **Read Only**, **Slave Only**, or **Preferred Host Read Only**. This configuration value is determined by a user.

A broker finds and connects to a suitable server by trying to establish a connection in the order of server connections; this is, if it fails to establish a connection, it tries another connection to the next server defined until it reaches the last server. If no connection is made even after trying all servers, the broker fails to connect to a server.

For details on how to configure broker mode, see :ref:`ha-cubrid-broker-conf`.

**Read Write**

A broker that provides read and write services. This broker is usually connected to an active server. If no active servers exist, this broker will be connected to a standby server. For this reason, a Read Write broker can be temporarily connected to a standby server.

When the broker temporarily establishes a connection to a standby server, it will disconnect itself from the standby server at the end of every transaction so that it can attempt to find an active server at the beginning of the next transaction. When it is connected to the standby server, only read service is available. Any write requests will result in a server error.

The order of server connection is described below:

*   The broker tries to establish a connection to an existing server connected (if exists). The active status of the server means the connection is complete.
*   The broker tries to establish a connection to the hosts specified in the **databases.txt** file in a sequence. The active status of the server means the connection is complete.
*   The broker tries to establish a connection to the hosts specified in the **databases.txt** file in a sequence and connects to the first available host.

.. image:: /images/image20.png

**Read Only**

A broker that provides the read service. This broker is connected to a standby server if possible. For this reason, the Read Only broker can be connected to an active server temporarily.

Once it establishes a connection with an active server, it will maintain that connection even if a standby server exists. To disconnect from the active server and reconnect to a standby server, you should execute the
**cubrid_broker reset**
command. An error will occur when the Read Only broker receives write requests; therefore, only the read service will be available even if it is connected to an active server.

The order of server connection is described below:

*   The broker tries to establish a connection to an existing server connected (if exists). The standby status of the server means the connection is complete.
*   The broker tries to establish a connection to the hosts specified in the **databases.txt** file in a sequence. The standby status of the server means the connection is complete.
*   The broker tries to establish a connection to the hosts specified in the **databases.txt** file in a sequence and connects to the first available host.

.. image:: /images/image21.png

**Slave Only**

A broker that provides the read service. This broker can only be connected to a standby server. If no standby server exists, no service will be provided.

The order of server connection is described below:

*   The broker tries to establish a connection to an existing server connected (if exists). The standby status of the server means the connection is complete.
*   The broker tries to establish a connection to the hosts specified in the **databases.txt** file in a sequence. The standby status of the server means the connection is complete.

.. image:: /images/image22.png

**Preferred Host Read Only**

A broker that provides the read service. This works in the same manner as the Read Only broker except its server connection order and server selecting criteria. The server connection order and server selecting criteria can be configured in **PREFERRED_HOSTS**. For details on configuring these, see :ref:`ha-cubrid-broker-conf`.

The order of server connection is described below:

*   The broker tries to establish a connection to the hosts specified in PREFERRED_HOSTS in a sequence and connects to the first available host.
*   The broker tries to establish a connection to the hosts specified in the **databases.txt** file in a sequence. The standby status of the server means the connection is complete.
*   The broker tries to establish a connection to the hosts specified in the **databases.txt** file in a sequence and connects to the first available host.

.. image:: /images/image23.png

CUBRID HA Features
==================

Duplexing Servers
-----------------

Duplexing servers is building a system by configuring duplicate hardware equipment to provide CUBRID HA. This method will prevent any interruptions in a server in case of occurring a hardware failure.

**Server failover**

A broker defines server connection order and connects to a server according to the defined order. If the connected server fails, the broker connects to the server with the next highest priority. This requires no processing in the application side. The actions taken when the broker connects to another server may differ according to the current mode of the broker. For details on the server connection order and configuring broker mode, see :ref:`ha-cubrid-broker-conf`.

.. image:: /images/image24.png

**Server failback**

CUBRID HA does not automatically support server failback. Therefore, to manually apply failback, restore the master node that has been abnormally terminated and run it as a slave node, terminate the node that has become the master from the slave due to failover, and finally, change the role of each node again.

For example, when *nodeA* is the master and *nodeB* is the slave, *nodeB* becomes the master and *nodeA* becomes the slave after a failover. After terminating *nodeB* (**cubrid heartbeat stop**) check (**cubrid heartbeat status**) whether the status of *nodeA* has become active. Start (**cubrid heartbeat start**) *nodeB* and it will become the slave.

.. _duplexing-brokers:

Duplexing Brokers
-----------------

As a 3-tier DBMS, CUBRID has middleware called the broker which relays applications and database servers. To provide HA, the broker also requires duplicate hardware equipment. This method will prevent any interruptions in a broker in case of occurring a hardware failure.

The configuration of broker redundancy is not determined by the configuration of server redundancy; it can be user-defined. In addition, it can be separated by piece of individual equipment.

To use the failover and failback functionalities of a broker, the **altHosts** attribute must be added to the connection URL of the JDBC, CCI, or PHP. For a description of this, see JDBC Configuration, CCI Configuration and PHP Configuration.

To set a broker, configure the **cubrid_broker.conf** file. To set the order of failovers of a database server, configure the **databases.txt** file. For more information, see Broker Configuration.

The following is an example in which two Read Write (RW) brokers are configured. When the first connection broker of the application URL is set to *broker B1* and the second connection broker to *broker B2*, the application connects to *broker B2* when it cannot connect to *broker B1*. When broker B1 becomes available again, the application reconnects to *broker B1*.

.. image:: /images/image25.png

The following is an example in which the Read Write (RW) broker and the Read Only (RO) broker are configured in each piece of equipment of the master node and the slave node. First, the app1 and the app2 URL connect to *broker A1* (RW) and *broker B2* (RO), respectively. The second connection (altHosts) is made to *broker A2* (RO) and *broker B1* (RW). When equipment that includes *nodeA* fails, app1 and the app2 connect to the broker that includes *nodeB*.

.. image:: /images/image26.png

The following is an example of a configuration in which broker equipment includes one Read Write broker (master node) and two Preferred Host Read Only brokers (slave nodes). The Preferred Host Read Only brokers are connected to nodeB and nodeC to distribute read load.

.. image:: /images/image27.png

**Broker failover**

The broker failover is not automatically failed over by the settings of system parameters. It is available in the JDBC, CCI, and PHP applications only when broker hosts are configured in the **altHosts** of the connection URL. Applications connect to the broker with the highest priority. When the connected broker fails, the application connects to the broker with the next highest priority. Configuring the **altHosts** of the connection URL is the only necessary action, and it is processed in the JDBC, CCI, and PHP drivers.

**Broker failback**

If the failed broker is recovered after a failover, the connection to the existing broker is terminated and a new connection is established with the recovered broker which has the highest priority. This requires no processing in the application side as it is processed within the JDBC, CCI, and PHP drivers. Execution time of failback depends on the value configured in JDBC connection URL. For details, see :ref:`ha-jdbc-conf`.

.. _log-multiplexing:

Log Multiplexing
----------------

CUBRID HA keeps every node in the CUBRID HA group with the identical structure by copying and reflecting transaction logs to all nodes included in the CUBRID HA group. As the log copy structure of CUBRID HA is a mutual copy between the master and the slave nodes, it has a disadvantage of increasing the size of a log volume. However, it has an advantage of flexibility in terms of configuration and failure handling, comparing to the chain-type copy structure.

.. image:: /images/image28.png

The transaction log copy modes include **SYNC**, **SEMISYNC**, and **ASYNC**. This value can be configured by the user in :ref:`cubrid-ha-conf` file.

**SYNC Mode**

When transactions are committed, the created transaction logs are copied to the slave node and stored as a file. The transaction commit is complete after receiving a notice on its success. Although the time it takes to execute commit in this mode may be longer than that in other modes, this is the safest method because the copied transaction logs are always guaranteed to be reflected to the standby server even if a failover occurs.

**SEMISYNC Mode**

When transactions are committed, the created transaction logs are copied to the slave node and stored as a file according to the internally optimized interval. The transaction commit is complete after receiving a notice of its success. The committed transactions in this mode are guaranteed to be reflected to the slave node sometime in the future.

Because SEMISYNC mode does not always store replication logs as a file, the execution time of commit can decrease, comparing to the SYNC mode. However, data synchronization between nodes may be delayed because replication logs are not reflected until it is stored as a file.

**ASYNC Mode**

When transactions are committed, commit is complete without verifying the transfer of transaction logs to a slave node. Therefore, it is not guaranteed that committed transactions are reflected to a slave node in a master node side.

Although ASYNC mode provides a better performance as it has almost no delay when executing commit, there may be data inconsistency in its nodes.

Quick Start
===========

Preparation
-----------

**Structure Diagram**

The diagram below aims to help users who are new to CUBRID HA, by explaining a simple procedure of the CUBRID HA configuration.

.. image:: /images/image29.png

**Specifications**

Linux and CUBRID version 2008 R2.2 or later must be installed on the equipment to be used as the master and the slave nodes. CUBRID HA does not support Windows operating system.

**Specifications of Configuring the CUBRID HA Equipment**

+------------------+---------------------------+--------+
|                  | CUBRID Version            | OS     |
+==================+===========================+========+
| For master nodes | CUBRID 2008 R2.2 or later | Linux  |
+------------------+---------------------------+--------+
| For slave nodes  | CUBRID 2008 R2.2 or later | Linux  |
+------------------+---------------------------+--------+

.. note:: 

    This document describes the HA configuration in CUBRID 2008 R4.1 Patch 2 or later versions. Note that the previous versions have different settings. For example, **cubrid_ha.conf** is only available in CUBRID 2008 R4.0 or later. **ha_make_slavedb.sh** describes CUBRID 2008 R4.1 Patch 2 or later.

.. _quick-server-config:

Creating Databases and Configuring Servers
------------------------------------------

**Creating Databases**

Create databases to be included in CUBRID HA at each node of the CUBRID HA in the same manner. Modify the options for database creation as needed. ::

    [nodeA]$ cd $CUBRID_DATABASES
    [nodeA]$ mkdir testdb
    [nodeA]$ cd testdb
    [nodeA]$ mkdir log
    [nodeA]$ cubrid createdb -L ./log testdb
    Creating database with 512.0M size. The total amount of disk space needed is 1.5G.
     
    CUBRID 9.0
     
    [nodeA]$

**cubrid.conf**

Ensure **ha_mode** of **$CUBRID/conf/cubrid.conf** in every CUBRID HA node has the same value. Especially, take caution when configuring the **log_max_archives** and **force_remove_log_archives** parameters (logging parameters) and the **ha_mode** parameter (HA parameter). ::

    # Service parameters
    [service]
    service=server,broker,manager

    # Common section
    [common]
    service=server,broker,manager

    # Server parameters
    server=testdb
    data_buffer_size=512M
    log_buffer_size=4M
    sort_buffer_size=2M
    max_clients=100
    cubrid_port_id=1523
    db_volume_size=512M
    log_volume_size=512M

    # Adds when configuring HA (Logging parameters)
    log_max_archives=100
    force_remove_log_archives=no

    # Adds when configuring HA (HA mode)
    ha_mode=on

**cubrid_ha.conf**

Ensure **ha_port_id**, **ha_node_list**, **ha_db_list** of **$CUBRID/conf/cubrid_ha.conf** in every CUBRID HA node has the same value. In the example below, we assume that the host name of a master node is *nodeA* and that of a slave node is *nodeB*. ::

    [common]
    ha_port_id=59901
    ha_node_list=cubrid@nodeA:nodeB
    ha_db_list=testdb
    ha_copy_sync_mode=sync:sync
    ha_apply_max_mem_size=500

**databases.txt**

Ensure that you must configure the host names (*nodeA:nodeB*) of master and slave nodes in db-host of **$CUBRID_DATABASES/databases.txt**; if **$CUBRID_DATABASES** is not configured, do it in **$CUBRID/databases/databases.txt**). ::

    #db-name vol-path db-host log-path lob-base-path
    testdb /home/cubrid/DB/testdb nodeA:nodeB /home/cubrid/DB/testdb/log file:/home/cubrid/DB/testdb/lob

Starting and Verifying CUBRID HA
--------------------------------

**Starting CUBRID HA**

Execute the **cubrid heartbeat** **start** at each node in the CUBRID HA group. Note that the node executing **cubrid heartbeat start** first will become a master node. In the example below, we assume that the host name of a master node is *nodeA* and that of a slave node is *nodeB*.

*   Master node ::

    [nodeA]$ cubrid heartbeat start

*   Slave node ::

    [nodeB]$ cubrid heartbeat start

**Verifying CUBRID HA Status**

Execute **cubrid heartbeat status** at each node in the CUBRID HA group to verify its configuration status. ::

    [nodeA]$ cubrid heartbeat status
    @ cubrid heartbeat list
     HA-Node Info (current nodeA-node-name, state master)
       Node nodeB-node-name (priority 2, state slave)
       Node nodeA-node-name (priority 1, state master)
     HA-Process Info (nodeA 9289, state nodeA)
       Applylogdb testdb@localhost:/home1/cubrid1/DB/testdb_nodeB.cub (pid 9423, state registered)
       Copylogdb testdb@nodeB-node-name:/home1/cubrid1/DB/testdb_nodeB.cub (pid 9418, state registered)
       Server testdb (pid 9306, state registered_and_active)
     
    [nodeA]$

Use the **cubrid changemode** utility at each node in the CUBRID HA group to verify the status of the server.

* Master node ::

    [nodeA]$ cubrid changemode testdb@localhost
    The server 'testdb@localhost''s current HA running mode is active.

* Slave node ::

    [nodeB]$ cubrid changemode testdb@localhost
    The server 'testdb@localhost''s current HA running mode is standby.

**Verifying the CUBRID HA Operation**

Verify that action is properly applied to standby server of the slave node after performing write in an active server of the master node. To make a success connection via the CSQL Interpreter in HA environment, you must specify the host name to be connected after the database name like "@<*host_name*>"). If you specify a host name as localhost, it is connected to local node.

.. warning:: Ensure that primary key must exist when creating a table to have replication successfully processed.

* Master node ::

    [nodeA]$ csql -u dba testdb@localhost -c "create table abc(a int, b int, c int, primary key(a));"
    [nodeA]$ csql -u dba testdb@localhost -c "insert into abc values (1,1,1);"
    [nodeA]$

* Slave node ::

    [nodeB]$ csql -u dba testdb@localhost -l -c "select * from abc;"
    === <Result of SELECT Command in Line 1> ===
    <00001> a: 1
            b: 1
            c: 1
    
    [nodeB]$

.. _quick-broker-config:

Configuring and Starting Broker, and Verifying the Broker Status
----------------------------------------------------------------

**Configuring the Broker**

To provide normal service during a database failover, it is necessary to configure an available database node in the **db-host** of **databases.txt**. And **ACCESS_MODE** in the **cubrid_broker.conf** file must be specified; if it is omitted, the default value is configured to Read Write mode. If you want to divide into a separate device, you must configure **cubrid_broker.conf** and **databases.txt** in the broker device.

* databases.txt ::

    #db-name        vol-path                db-host         log-path        lob-base-path
    testdb          /home1/cubrid1/CUBRID/testdb  nodeA:nodeB        /home1/cubrid1/CUBRID/testdb/log file:/home1/cubrid1/CUBRID/testdb/lob

* cubrid_broker.conf ::

    [%testdb_RWbroker]
    SERVICE                 =ON
    BROKER_PORT             =33000
    MIN_NUM_APPL_SERVER     =5
    MAX_NUM_APPL_SERVER     =40
    APPL_SERVER_SHM_ID      =33000
    LOG_DIR                 =log/broker/sql_log
    ERROR_LOG_DIR           =log/broker/error_log
    SQL_LOG                 =ON
    TIME_TO_KILL            =120
    SESSION_TIMEOUT         =300
    KEEP_CONNECTION         =AUTO
    CCI_DEFAULT_AUTOCOMMIT  =ON
     
    # broker mode parameter
    ACCESS_MODE             =RW

**Starting Broker and Verifying its Status**

A broker is used to access applications such as JDBC, CCI or PHP. Therefore, to simply test server redundancy, execute the CSQL interpreter that is directly connected to the server processes, without having to start a broker. To start a broker, execute **cubrid broker start**. To stop it, execute **cubrid broker stop**.

The following example shows how to execute a broker from the master node. ::

    [nodeA]$ cubrid broker start
    @ cubrid broker start
    ++ cubrid broker start: success
    [nodeA]$ cubrid broker status
    @ cubrid broker status
    % testdb_RWbroker
    ---------------------------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS
    ---------------------------------------------------------
     1  9532     0     0  48120  IDLE

**Configuring Applications**

Specifies the host name (*nodeA_broker*, *nodeB_broker*) and port for an application to connect in the connection URL. The **altHosts** attribute defines the broker where the next connection will be made when the connection to a broker fails. The following is an example of a JDBC program. For more information on CCI and PHP, see :ref:`ha-cci-conf` and :ref:`ha-php-conf`.

.. code-block:: java

    Connection connection = DriverManager.getConnection("jdbc:CUBRID:nodeA_broker:33000:testdb:::?charSet=utf-8&altHosts=nodeB_broker:33000", "dba", "");

.. _ha-configuration:

Environment Configuration
=========================

cubrid.conf
-----------

The **cubrid.conf** file that has general information on configuring CUBRID is located in the **$CUBRID/conf** directory. This page provides information about **cubrid.conf** parameters used by CUBRID HA.

**ha_mode**

**ha_mode** is a parameter used to configure whether to use CUBRID HA. The default value is **off**. CUBRID HA does not support Windows; it supports Linux only.

*   **off** : CUBIRD HA is not used.
*   **on** : CUBRID HA is used. Failover is supported for its node.
*   **replica** : CUBRID HA is used. Failover is not supported for its node.

The **ha_mode** parameter can be re-configured in the **[@<database>]** section; however, only **off** can be entered in the case. An error is returned if a value other than **off** is entered in the **[@<database>]** section.

If **ha_mode** is **on**, the CUBRID HA values are configured by reading **cubrid_ha.conf**.

This parameter cannot be modified dynamically. To modify the value of this parameter, you must restart it.

**log_max_archives**

**log_max_archives** is a parameter used to configure the minimum number of archive log files to be archived. The minimum value is 0 and the default is **INT_MAX** (2147483647). When CUBRID has installed for the first time, this value is set to 0 in the **cubrid.conf** file. The behavior of the parameter is affected by **force_remove_log_archives**.

The existing archive log files to which the activated transaction refers or the archive log files of the master node not reflected to the slave node in HA environment will not be deleted. For details, see the following **force_remove_log_archives**. For details about **log_max_archives**, see :ref:`logging-parameters`.

**force_remove_log_archives**

It is recommended to configure **force_remove_archives** to **no** so that archive logs to be used by HA-related processes always can be maintained to set up HA environment by configuring **ha_mode** to **on**.

If you configure the value for **force_remove_log_archives** to yes, the archive log files which will be used in the HA-related process can be deleted, and this may lead to an inconsistency between replicated databases. If you want to maintain free disk space even though doing this could lead to risk, you can configure the value to yes. For details about **force_remove_log_archives**, see :ref:`logging-parameters`.

.. note::

    From 2008 R4.3 in replica mode, it will be always deleted except for archive logs as many as specified in the **log_max_archives** parameter, regardless the **force_remove_log_archives** value specified.

**max_clients**

**max_clients** is a parameter used to configure the maximum number of clients to be connected to a database server simultaneously. The default is **100**.

Because the replication log copy and the replication log reflection processes start by default if CUBRID HA is used, you must configure the value to twice the number of all nodes in the CUBRID HA group, except the corresponding node. Furthermore, you must consider the case in which a client that is connected to another node at the time of failover attempts to connect to that node. For details about max_client, see :ref:`connection-parameters`.

**The Parameters That Must Have the Same Value for All Nodes**

*   **log_buffer_size** : The size of a log buffer. This must be same for all nodes, as it affects the protocol between **copylogdb** that duplicate the server and logs.

*   **log_volume_size** : The size of a log volume. In CUBRID HA, the format and contents of a transaction log are the same as that of the replica log. Therefore, the parameter must be same for all nodes. If each node creates its own DB, the **cubrid createdb** options (**--db-volume-size**, **--db-page-size**, **--log-volume-size**, **--log-page-size**, etc.) must be the same.

*   **cubrid_port_id** : The TCP port number for creating a server connection. It must be same for all nodes in order to connect **copylogdb** that duplicate the server and logs.

*   **HA-related parameters** : HA parameters included in **cubrid_ha.conf** must be identical by default. However, the following parameters can be set differently according to the node.

    *   The **ha_mode** parameter in replica node
    *   The **ha_copy_sync_mode** parameter
    *   The **ha_ping_hosts** parameter

**Example**

The following example shows how to configure **cubrid.conf**. Please take caution when configuring **log_max_archives** and **force_remove_log_archives** (logging-related parameters), and **ha_mode** (an HA-related parameter). ::

    # Service Parameters
    [service]
    service=server,broker,manager

    # Server Parameters
    server=testdb
    data_buffer_size=512M
    log_buffer_size=4M
    sort_buffer_size=2M
    max_clients=200
    cubrid_port_id=1523
    db_volume_size=512M
    log_volume_size=512M

    # Adds when configuring HA (Logging parameters)
    log_max_archives=100
    force_remove_log_archives=no

    # Adds when configuring HA (HA mode)
    ha_mode=on
    log_max_archives=100

.. _cubrid-ha-conf:

cubrid_ha.conf
--------------

The **cubrid_ha.conf** file that has generation information on CUBRID HA is located in the **$CUBRID/conf** directory. CUBRID HA does not support Windows; it supports Linux only.

**ha_node_list**

**ha_node_list** is a parameter used to configure the group name to be used in the CUBRID HA group and the host name of member nodes in which failover is supported. The group name is separated by @. The name before @ is for the group, and the names after @ are for host names of member nodes. A colon (:) is used to separate individual host names. The default is **localhost@localhost**.

The host name of the member nodes specified in this parameter cannot be replaced with the IP. You should use the host names which are registered in **/etc/hosts**. A node in which the **ha_mode** value is set to **on** must be specified in **ha_node_list**. The value of the **ha_node_list** of all nodes in the CUBRID HA group must be identical. When a failover occurs, a node becomes a master node in the order specified in the parameter.

This parameter can be modified dynamically. If you modify the value of this parameter, you must execute :ref:`cubrid heartbeat reload <cubrid-heartbeat>` to apply the changes.

**ha_replica_list**

**ha_replica_list** is parameter used to configure the group name to be used in the CUBRID HA group and the host name of member nodes in which failover is not supported. The group name is separated by @. The name before @ is for the group, and the names after @ are for host names of member nodes. A colon (:) is used to separate individual host names. The default is **NULL**.

The group name must be identical to the name specified in **ha_replica_list**. The host names of member nodes and the host names of nodes specified in this parameter must be registered in **/etc/hosts**. A node in which the **ha_mode** value is set to **replica** must be specified in **ha_replica_list**. The **ha_replica_list** values of all nodes in the CUBRID HA group must be identical.

This parameter can be modified dynamically. If you modify the value of this parameter, you must execute :ref:`cubrid heartbeat reload <cubrid-heartbeat>` to apply the changes.

**ha_port_id**

**ha_port_id** is a parameter used to configure the UDP port number; the UDP port is used to detect failure when exchanging heartbeat messages. The default is **59,901**.

If a firewall exists in the service environment, the firewall must be configured to allow the configured port to pass through it.

**ha_ping_hosts**

**ha_ping_hosts** is a parameter used to configure the host which verifies whether or not a failover occurs due to unstable network when a failover has started in a slave node. The default is **NULL**.

The host name of the member nodes specified in this parameter can be replaced with the IP. When a host name is used, the name must be registered in **/etc/hosts**.

Configuring this parameter can prevent split-brain, a phenomenon in which two master nodes simultaneously exist as a result of the slave node erroneously detecting an abnormal termination of the master node due to unstable network status and then promoting itself as the new master. When specifying multiple hosts, separate each host with a colon (:).

**ha_copy_sync_mode**

**ha_copy_sync_mode** is a parameter used to configure the mode of storing the transaction log copy. The default is **SYNC**.

The value can be one of the followings: **SYNC**, **SEMISYNC**, or **ASYNC**. The number of values must be the same as the number of nodes specified in **ha_node_list**. They must be ordered by the specified value. You can specify multiple nodes by using a colon (:). The replica node is always working in **ASNYC** mode regardless of this value.

For details, see :ref:`log-multiplexing`.

**ha_copy_log_base**

**ha_copy_log_base** is a parameter used to configure the location of storing the transaction log copy. The default is **$CUBRID_DATABASES**.

For details, see :ref:`log-multiplexing`.

**ha_db_list**

**ha_db_list** is a parameter used to configure the name of the database that will run in CUBRID HA mode. The default is **NULL**. You can specify multiple databases by using a comma (,).

**ha_apply_max_mem_size**

**ha_apply_max_mem_size** is a parameter used to configure the value of maximum memory that the replication log reflection process of CUBRID HA can use. The default and maximum values are **500** (unit: MB). When the value is larger than the size allowed by the system, memory allocation fails and the HA replication reflection process may malfunction. For this reason, you must check whether or not the memory resource can handle the specified value before setting it.

**ha_applylogdb_ignore_error_list**

**ha_applylogdb_ignore_error_lis** is a parameter used to configure for proceeding replication in CUBRID HA process by ignoring an error occurrence. The error codes to be ignored are separated by a comma (,). This value has a high priority. Therefore, when this value is the same as the value of the **ha_applylogdb_retry_error_list** parameter or the error code of "List of Retry Errors," the values of the **ha_applylogdb_retry_error_list** parameter or the error code of "List of Retry Errors" are ignored and the tasks that cause the error are not retried. For "List of Retry Errors," see the description of **ha_applylogdb_retry_error_list** below.

**ha_applylogdb_retry_error_list**

**ha_applylogdb_retry_error_list** is a parameter used to configure for retrying tasks that caused an error in the replication log reflection process of CUBRID HA until the task succeeds. When specifying errors to be retried, separate each error with a comma (,). The following table shows the default "List of Retry Errors." If these values exist in **ha_applylogdb_ignore_error_list**, the error will be overridden.

**List of Retry Errors**

+-------------------------------------+----------------+
| Error Code Name                     | Error Code     |
+=====================================+================+
| ER_LK_UNILATERALLY_ABORTED          | -72            |
+-------------------------------------+----------------+
| ER_LK_OBJECT_TIMEOUT_SIMPLE_MSG     | -73            |
+-------------------------------------+----------------+
| ER_LK_OBJECT_TIMEOUT_CLASS_MSG      | -74            |
+-------------------------------------+----------------+
| ER_LK_OBJECT_TIMEOUT_CLASSOF_MSG    | -75            |
+-------------------------------------+----------------+
| ER_LK_PAGE_TIMEOUT                  | -76            |
+-------------------------------------+----------------+
| ER_PAGE_LATCH_TIMEDOUT              | -836           |
+-------------------------------------+----------------+
| ER_PAGE_LATCH_ABORTED               | -859           |
+-------------------------------------+----------------+
| ER_LK_OBJECT_DL_TIMEOUT_SIMPLE_MSG  | -966           |
+-------------------------------------+----------------+
| ER_LK_OBJECT_DL_TIMEOUT_CLASS_MSG   | -967           |
+-------------------------------------+----------------+
| ER_LK_OBJECT_DL_TIMEOUT_CLASSOF_MSG | -968           |
+-------------------------------------+----------------+
| ER_LK_DEADLOCK_CYCLE_DETECTED       | -1021          |
+-------------------------------------+----------------+

The following example shows how to configure **cubrid_ha.conf**. ::

    [common]
    ha_node_list=cubrid@nodeA:nodeB
    ha_db_list=testdb
    ha_copy_sync_mode=sync:sync
    ha_apply_max_mem_size=500

**Remark**

The following example shows how to configure the value of /etc/hosts (a host name of a member node: nodeA, IP: 192.168.0.1). ::

    127.0.0.1 localhost.localdomain localhost
    192.168.0.1 nodeA

.. _ha-cubrid-broker-conf:

cubrid_broker.conf
------------------

The **cubrid_broker.conf** file that has general information on configuring CUBRID broker is located in the **$CUBRID/conf** directory. This section explains the parameters of **cubrid_broker.conf** that are used by CUBRID HA.

**ACCESS_MODE**

**ACCESS_MODE** is a parameter used to configure the mode of a broker. The default is **RW**.

Its value can be one of the followings: **RW** (Read Write), **RO** (Read Only), **SO** (Slave Only), or **PHRO** (Preferred Host Read Only). For details, see :ref:`broker-mode`.

**PREFERRED_HOSTS**

**PREFERRED_HOSTS** is a parameter used only when the **ACCESS_MODE** parameter value is **PHRO**. The default value is **NULL**.

You can specify multiple nodes by using a colon (:). First, it tries to connect to host in the following order: host specified in the **PREFERRED_HOSTS** parameter first and then host specified in **$CUBRID_DATABASES/databases.txt**. For details, see :ref:`broker-mode`.

The following example shows how to configure **cubrid_broker.conf**. ::

    [%PHRO_broker]
    SERVICE                 =ON
    BROKER_PORT             =33000
    MIN_NUM_APPL_SERVER     =5
    MAX_NUM_APPL_SERVER     =40
    APPL_SERVER_SHM_ID      =33000
    LOG_DIR                 =log/broker/sql_log
    ERROR_LOG_DIR           =log/broker/error_log
    SQL_LOG                 =ON
    TIME_TO_KILL            =120
    SESSION_TIMEOUT         =300
    KEEP_CONNECTION         =AUTO
    CCI_DEFAULT_AUTOCOMMIT  =ON
     
    # Broker mode setting parameter
    ACCESS_MODE             =PHRO
    PREFERRED_HOSTS         =nodeA:nodeB:nodeC

databases.txt
-------------

The **databases.txt** file that has information on servers to be connected by a broker and their order is located in the **$CUBRID_DATABASES** (if not specified, $CUBRID/databases) directory; the information can be configured by using **db_hosts**. You can specify multiple nodes by using a colon (:).

The following example shows how to configure **databases.txt**. ::

    #db-name    vol-path        db-host     log-path     lob-base-path
    testdb       /home/cubrid/DB/testdb nodeA:nodeB   /home/cubrid/DB/testdb/log  file:/home/cubrid/DB/testdb/lob

.. _ha-jdbc-conf:

JDBC Configuration
------------------

To use CUBRID HA in JDBC, you must specify the connection information of another broker (*nodeB_broker*) to be connected when a failure occurs in broker (*nodeA_broker*). The attribute configured for CUBRID HA is **altHosts** which represents information of one or more broker nodes to be connected. For details, see :ref:`jdbc-connection-conf`.

The following example shows how to configure JDBC:

.. code-block:: java

    Connection connection = DriverManager.getConnection("jdbc:CUBRID:nodeA_broker:33000:testdb:::?charSet=utf-8&altHosts=nodeB_broker:33000", "dba", "");

.. _ha-cci-conf:

CCI Configuration
-----------------

To use CUBRID HA in CCI, you must use the :c:func:`cci_connect_with_url` function which additionally allows specifying connection information in connection URL; the connection information is used when a failure occurs in broker. The attribute configured for CUBRID HA is **altHosts** which represents information of one or more broker nodes to be connected.

The following example shows how to configure CCI.

.. code-block:: c

    con = cci_connect_with_url ("cci:CUBRID:nodeA_broker:33000:testdb:::?altHosts=nodeB_broker:33000", "dba", NULL);
    if (con < 0)
    {
          printf ("cannot connect to database\n");
          return 1;
    }

.. _ha-php-conf:

PHP Configuration
-----------------

To use the functions of CUBRID HA in PHP, connect to the broker by using `cubrid_connect_with_url <http://www.php.net/manual/en/function.cubrid-connect-with-url.php>`_ , which is used to specify the connection information of the failover broker in the connection URL. The attribute specified for CUBRID HA is **altHosts**, the information on one or more broker nodes to be connected when a failover occurs.

The following example shows how to configure PHP.

.. code-block:: php

    <?php
    $con = cubrid_connect_with_url ("cci:CUBRID:nodeA_broker:33000:testdb:::?altHosts=nodeB_broker:33000", "dba", NULL);
    if ($con < 0)
    {
          printf ("cannot connect to database\n");
          return 1;
    }
    ?>

.. note:: 

    If you want to run smoothly the broker's failover in the environment which the broker's failover is enabled by setting **altHosts**, you should set the value of **disconnectOnQueryTimeout** in URL as **true**.
    
    If this value is **true**, an application program releases the existing connection from a broker and reconnect to the other broker which is specified on **altHosts**.

Running and Monitoring
======================

.. _cubrid-heartbeat:

Utilities of cubrid heartbeat
-----------------------------

**start**

This utility is used to activate CUBRID HA feature and start all processes of CUBRID HA in the node(database server process, replication log copy process, and replication log reflection process). Note that a master node or a slave node is determined based on the execution order of **cubrid heartbeat start**.

How to execute the command is as shown below. ::

    $ cubrid heartbeat start

The database server process configured in HA mode cannot be started with the **cubrid server start** command.

Specify the database name at the end of the command to run only the HA configuration processes (database server process, replication log copy process, and replication log reflection process) of a specific database in the node. For example, use the following command to run the database *testdb* only: ::

    $ cubrid heartbeat start testdb

**stop**

This utility is used to disable and stop all components of CUBRID. The node that executes this command stops and a failover occurs to the next slave node according to the CUBRID HA configuration.

How to use this utility is as shown below. ::

    $ cubrid heartbeat stop

The database server process cannot be stopped with the **cubrid server stop** command.

Specify the database name at the end of the command to stop only the HA configuration processes (database server process, replication log copy process, and replication log reflection process) of a specific database in the node. For example, use the following command to run the database *testdb* only: ::

    $ cubrid heartbeat stop testdb

**copylogdb**

This utility is used to start or stop the **copylogdb** process that copies the transaction logs for the *db_name* of a specific peer_node in the CUBRID HA configuration. You can pause log copy for rebuilding replications in the middle of operation and then rerun it whenever you want.

Even though only the **cubrid heartbeat copylogdb start** command has succeeded, the functions of detecting and recovering the failure between the nodes are executed. Since the node is the target of failover, the slave node can be changed to the master node.

How to use this utility is as shown below. ::

    $ cubrid heartbeat copylogdb <start|stop> db_name peer_node

When the **copylogdb** process is started/stopped, the configuration information of the **cubrid_ha.conf** is used. We recommend that you do not change the configuration as possible after you have set the configuration once. If you need to change it, it is recommended to restart the whole nodes.

**applylogdb**

This utility is used to start or stop the **copylogdb** process that reflect the transaction logs for the *db_name* of a specific peer_node in the CUBRID HA configuration. You can pause log copy for rebuilding replications in the middle of operation and then rerun it whenever you want.

Even though only the **cubrid heartbeat copylogdb start** command has succeeded, the functions of detecting and recovering the failure between the nodes are executed. Since the node is the target of failover, the slave node can be changed to the master node.

How to use this utility is as shown below. ::

    $ cubrid heartbeat applylogdb <start|stop> db_name peer_node

When the **applylogdb** process is started/stopped, the configuration information of the **cubrid_ha.conf** is used. We recommend that you do not change the configuration as possible after you have set the configuration once. If you need to change it, it is recommended to restart the whole nodes.

**reload**

This utility is used to retrieve the CUBRID HA information again, and it starts or stops the CUBRID HA components according to new CUBRID HA configuration. Used to add or delete a node; it starts the HA processes which correspond to the added nodes after modification or it stops the HA processes which correspond to the deleted nodes.

How to use this utility is as shown below. ::

    $ cubrid heartbeat reload

**status**

This utility is used to output the information of CUBRID HA group and CUBRID HA components.

How to use this utility is as shown below. ::

    $ cubrid heartbeat status
    @ cubrid heartbeat status
     
     HA-Node Info (current nodeB, state slave)
       Node nodeB (priority 2, state slave)
       Node nodeA (priority 1, state master)
     
     
     HA-Process Info (master 2143, state slave)
       Applylogdb testdb@localhost:/home/cubrid/DB/testdb_nodeB (pid 2510, state registered)
       Copylogdb testdb@nodeA:/home/cubrid/DB/testdb_nodeA (pid 2505, state registered)
       Server testdb (pid 2393, state registered_and_standby)

.. note:: **act**, **deact**, and **deregister** commands which were used in versions lower than CUBRID 9.0 are no longer used.

.. _cubrid-service-util:

Utilities of cubrid service
---------------------------

If you register heartbeat to CUBRID service, you can use the utilities of **cubrid service** to start, stop or check all the related processes at once. The processes specified by **service** parameter in [**service**] section in **cubrid.conf** file are registered to CUBRID service. If this parameter includes **heartbeat**, you can start/stop all the service processes and the HA-related processes by using **cubrid service start** / **stop** command.

How to configure **cubrid.conf** file is shown below. ::

    # cubrid.conf

    ...

    [service]

    ...

    service=broker,heartbeat

    ...

    [common]

    ...

    ha_mode=on

.. _cubrid-applyinfo:

cubrid applyinfo
----------------

This utility is used to check the copied and applied status of replication logs by CUBRID HA. ::

    cubrid applyinfo [option] <database-name>

*   *database-name* : Specifies the name of a server to monitor. A node name is not included.

The following shows the [options] used on **cubrid applyinfo**.

.. program:: applyinfo

.. option:: -r, --remote-host-name=HOSTNAME

    Configures the name of a target node in which transaction logs are copied. Using this option will output the information of active logs (Active Info.) of a target node.
    
.. option:: -a, --applied-info

    Outputs the information of replication reflection of a node executing cubrid applyinfo. 
    The **-L** option is required to use this option.
    
.. option:: -L, --copied-log-path=PATH

    Configures the location of transaction logs copied from the other node. Using this option will output the information of transaction logs copied (Copied Active Info.) from the other node.
    
.. option:: -p, --pageid=ID

    Outputs the information of a specific page in the copied logs. 
    This is available only when the  **-L** option is enabled.  The default is 0, it means the active page. 
        
.. option:: -v

    Outputs detailed information.                        

.. option:: -i, --interval=SECOND

    Outputs the copied status and applied status of transaction logs per specified seconds. To see the delayed status of the replicated log, this option is mandatory.
    
**Example**

The following example shows how to check log information (Active Info.) of the master node, the status information of log copy (Copied Active Info.) of the slave node, and the applylogdb info (Applied Info.) of the slave node by executing **applyinfo** in the slave node.

*   Applied Info.: Shows the status information after the slave node applies the replication log.
*   Copied Active Info.: Shows the status information after the slave node copies the replication log.
*   Active Info.: Shows the status information after the master node records the transaction log.
*    Delay in Copying Active Log: Shows the status information which the transaction logs’ copy is delayed.
*    Delay in Applying Copied Log: Shows the status information which the transaction logs’ application is delayed.

::

    [nodeB] $ cubrid applyinfo -L /home/cubrid/DB/testdb_nodeA -r nodeA -a -i 3 testdb
     
     *** Applied Info. *** 
    Insert count                   : 289492
    Update count                   : 71192
    Delete count                   : 280312
    Schema count                   : 20
    Commit count                   : 124917
    Fail count                     : 0

     *** Copied Active Info. *** 
    DB name                        : testdb
    DB creation time               : 04:29:00.000 PM 11/04/2012 (1352014140)
    EOF LSA                        : 27722 | 10088
    Append LSA                     : 27722 | 10088
    HA server state                : active

     ***  Active Info. *** 
    DB name                        : testdb
    DB creation time               : 04:29:00.000 PM 11/04/2012 (1352014140)
    EOF LSA                        : 27726 | 2512
    Append LSA                     : 27726 | 2512
    HA server state                : active

     *** Delay in Copying Active Log *** 
    Delayed log page count         : 4
    Estimated Delay                : 0 second(s)

     *** Delay in Applying Copied Log *** 
    Delayed log page count         : 1459
    Estimated Delay                : 22 second(s)

The items shown by each status are as follows:

*    Applied Info.
    
    *    Committed page: The information of committed pageid and offset of a transaction reflected last through replication log reflection process. The difference between this value and the EOF LSA of "Copied Active Info. represents the amount of replication delay.
    *    Insert Count: The number of Insert queries reflected through replication log reflection process.
    *    Update Count: The number of Update queries reflected through replication log reflection process.
    *    Delete Count: The number of Delete queries reflected through replication log reflection process.
    *    Schema Count: The number of DDL statements reflected through replication log reflection process.
    *    Commit Count: The number of transactions reflected through replication log reflection process.
    *    Fail Count: The number of DML and DDL statements in which log reflection through replication log reflection process fails.
    
*    Copied Active Info.
    
    *    DB name: Name of a target database in which the replication log copy process copies logs
    *    DB creation time: The creation time of a database copied through replication log copy process
    *    EOF LSA: Information of pageid and offset copied at the last time on the target node by the replication log copy process. There will be a delay in copying logs as much as difference with the EOF LSA value of "Active Info." and with the Append LSA value of "Copied Active Info."
    *    Append LSA: Information of pageid and offset written at the last time on the disk by the replication log copy process. This value can be less than or equal to EOF LSA. There will be a delay in copying logs as much as difference between the EOF LSA value of "Copied Active Info." and this value.
    *    HA server state: Status of a database server process which replication log copy process receives logs from. For details on status, see :ref:`ha-server`.
    
*    Active Info.
    
    *    DB name: Name of a database of which node was configured in the **-r** option.
    *    DB creation time: Database creation time of a node that is configured in the **-r** option.
    *    EOF LSA: The last information of pageid and offset of a database transaction log of a node that is configured in the **-r** option. There will be a delay in copying logs as much as difference between the EOF LSA value of "Copied Active Info." and this value.
    *    Append LSA: Information of pageid and offset written at the last time on the disk by the database of which node was configured in the **-r** option.
    *    HA server state: The server status of a database server of which node was configured in the **-r** option.
    
*    Delay in Copying Active Log
    
    *    Delayed log page count: the count of transaction log pages which the copy is delayed.
    *    Estimated Delay: the expected time which the logs copying is completed.
    
*    Delay in Applying Copied Log

    *    Delayed log page count: the count of transaction log pages which the application is delayed.
    *    Estimated Delay: the expected time which the logs applying is completed.
    
.. _cubrid-changemode:

cubrid changemode
-----------------

This utility is used to check and change the server status of CUBRID HA. ::

    cubrid changemode [option] <database-name@node-name>

*   *database-name@node-name* : Specifies the name of a server to be checked or changed and separates each node name by using @.


.. program:: changemode

.. option:: -m, --mode=MODE

    Changes the server status. You can enter one of the followings:                                                                                                       
    
    **standby**, **maintenance** or **active**.
    
.. option:: -f, --force

    Configures whether or not to forcibly change the server status. This option must be configured if you want to change the server status from to-be-active to active.   |
    
    If it is not configured, the status will not be changed to active. 
    Forcibly change may cause data inconsistency among replication nodes; so it is not recommended.                                                                       |

.. option:: -t, --timeout=SECOND
    
    The default is 5(seconds). 

    Configures the waiting time for the normal completion of the transaction that is being processed when the node status switches from **standby** to **maintenance**. 
    
    If the transaction is still in progress beyond the configured time, it will be forced to terminate and switch to **maintenance** status; if all transactions have completed normally within the configured time, it will switch to **maintenance** status immediately. 

**Status Changeable**

This table shows changeable modes depending on current status.

+------------------------------------+----------------------------------------+
|                                    | **Changeable**                         |
|                                    +----------------+---------+-------------+
|                                    | active         | standby | maintenance |
+--------------------+---------------+----------------+---------+-------------+
| **Current Status** | standby       | X              | O       | O           |
|                    +---------------+----------------+---------+-------------+
|                    | to-be-standby | X              | X       | X           |
|                    +---------------+----------------+---------+-------------+
|                    | active        | O              | X       | X           |
|                    +---------------+----------------+---------+-------------+
|                    | to-be-active  | O*             | X       | X           |
|                    +---------------+----------------+---------+-------------+
|                    | maintenance   | X              | O       | O           |
+--------------------+---------------+----------------+---------+-------------+

\* When the server status is to-be-active, forcibly change may cause data inconsistency among replication nodes. It is not recommended if you are not skilled enough.

**Example**

The following example shows how to switch the *testdb* server status in the localhost node to maintenance. The waiting time for all transactions in progress to complete normally is 5 seconds, which is the default value for the **-t** option. If all transactions are complete within this time limit, the status will be switched immediately. However, if there are transactions still being processed after this time limit, they will be rolled back before changing the status. ::

    $ cubrid changemode -m maintenance testdb@localhost
    The server 'testdb@localhost''s current HA running mode is maintenance.

The following example shows how to retrieve status of the *testdb* server in the localhost node. ::

    $ cubrid changemode testdb@localhost
    The server 'testdb@localhost''s current HA running mode is active.

Monitoring CUBRID Manager HA
----------------------------

CUBRID Manager is a dedicated CUBRID database management tool that provides the CUBRID database management and query features in a GUI environment. CUBRID Manager provides the HA dashboard, which shows the relationship diagram for the CUBRID HA group and server status. For details, see CUBRID Manager manual.

Configuration
=============

There are four possible structures for CUBRID HA: The default structure, multiple-slave node structure, load balancing structure, and multiple-standby server structure. In the table below, M stands for a master node, S for a slave node, and R for a replica node.

+-----------------------------------+----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| Structure                         | Node structure (M:S:R)     | Characteristic                                                                                                                                         |
+===================================+============================+========================================================================================================================================================+
| Default Structure                 | 1:1:0                      | The most basic structure of CUBRID HA consists of one master node and one slave node and provides availability which is a unique feature of CUBRID HA. |
+-----------------------------------+----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| Multiple-Slave Node Structure     | 1:N:0                      | This is a structure in which availability is increased by several slave nodes. However,                                                                |
|                                   |                            | note that there may be a situation in which data is inconsistent in the CUBRID HA group when multiple failures occur.                                  |
+-----------------------------------+----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| Load Balancing Structure          | 1:1:N                      | Several replica nodes are added in the basic structure. Read service load can be distributed, and the HA load is reduced,                              |
|                                   |                            | comparing to a multiple-slave node structure. Note that replica nodes do not failover.                                                                 |
+-----------------------------------+----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| Multiple-Standby Server Structure | 1:1:0                      | Basically, this structure is the same as the basic structure. However, several slave nodes are installed on a single physical server.                  |
+-----------------------------------+----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+

Default Structure of HA
-----------------------

The most basic structure of CUBRID HA consists of one master node and one slave node.

The default configuration is one master node and one slave node. To distribute the write load, a multi-slave node or load-distributed configuration is recommended. In addition, to access a specific node such as a slave node or replica node in read-only mode, configure the Read Only broker or the Preferred Host Read Only broker. For details about broker configuration, see :ref:`duplexing-brokers`.

**An Example of Node Configuration**

.. image:: /images/image30.png

You can configure each node in the basic structure of HA as shown below:

*   **node A** (master node)

    * Configure the **ha_mode** of the **cubrid.conf** file to **on**. ::

        ha_mode=on

    * The following example shows how to configure **cubrid_ha.conf**: ::

        ha_port_id=59901
        ha_node_list=cubrid@nodeA:nodeB
        ha_db_list=testdb

*   **node B** (slave node): Configure this node in the same manner as *node A*.

For the **databases.txt** file of a broker node, it is necessary to configure the list of hosts configured as HA in **db-host** according to their priority. The following example shows the **databases.txt** file. ::

    #db-name    vol-path                  db-host       log-path       lob-base-path
    testdb     /home/cubrid/DB/testdb1   nodeA:nodeB   /home/cubrid/DB/testdb/log file:/home/cubrid/DB/testdb/lob

The **cubrid_broker.conf** file can be set in a variety of ways according to configuration of the broker. It can also be configured as separate equipment with the **databases.txt** file.

The example below shows that the RW broker is set in each node, and *node A* and *node B* have the same value. ::

    [%RW_broker]
    ...
     
    # Broker mode setting parameter
        ACCESS_MODE             =RW

**Connection Configuration of Applications**

See :ref:`ha-jdbc-conf`, :ref:`ha-cci-conf`, and :ref:`ha-php-conf` in Environment Configuration.

**Remark**

The path of a transaction log in these configurations is as follows:

.. image:: /images/image31.png

Multiple-Slave Node Structure
-----------------------------

In multiple-slave node structure, there is one master node and several slave nodes to improve the service availability of CUBRID.

Because replication log copy process and replication log reflection process are running at all nodes in the CUBRID HA group, a load of copying replication log occurs. Therefore, all nodes in the CUBRID HA group have high network and disk usage.

Because there are many nodes with HA enabled, read and write services never fail as long as a single node is alive.

In the multiple-slave node structure, the node becoming a master node when failover occurs is determined by the order specified in **ha_node_list**. If the value of **ha_node_list** is node1:node2:node3 and the master node is *node A*, *node B* will become a new master node when the master node fails.

**An Example of Node Configuration**

.. image:: /images/image32.png

You can configure each node in the basic structure of HA as shown below:

*   **node A** (master node)

    * Configure the **ha_mode** of the **cubrid.conf** file to **on**. ::

        ha_mode=on

    * The following example shows how to configure **cubrid_ha.conf**: ::

        ha_port_id=59901
        ha_node_list=cubrid@nodeA:nodeB:nodeC
        ha_db_list=testdb

*   **node B** (slave node): Configure this node in the same manner as *node A*.

*   **node C** (slave node): Configure this node in the same manner as *node A*.

You must enter the list of hosts configured in HA in order of priority in the **databases.txt** file of a broker node. The following is an example of the **databases.txt** file. ::

    #db-name    vol-path                  db-host             log-path       lob-base-path
    testdb     /home/cubrid/DB/testdb1   nodeA:nodeB:nodeC   /home/cubrid/DB/testdb/log file:/home/cubrid/DB/testdb/lob

The **cubrid_broker.conf** file can be set in a variety of ways according to configuration of the broker. It can also be configured as separate equipment with the **databases.txt** file.

In this example, the RW broker is configured in *node A*, *node B*, and *node C*.

The following is an example of the **databases.txt** file in *node A*, *node B*, and *node C*. ::

    [%RW_broker]
    ...
     
    # Broker mode setting parameter
    ACCESS_MODE             =RW

**Connection Configuration of Applications**

Connect the application to access to the broker of *node A*, *node B*, or *node C*.

.. code-block:: java

    Connection connection = DriverManager.getConnection(
        "jdbc:CUBRID:nodeA:33000:testdb:::?charSet=utf-8&altHosts=nodeB:33000,nodeC:33000", "dba", "");

For details, see :ref:`ha-jdbc-conf`, :ref:`ha-cci-conf`, and :ref:`ha-php-conf` in Environment Configuration.

**Remark**

The data in the CUBRID HA group may lose integrity when there are multiple failures in this structure and the example is shown below.

*   n a situation where a failover occurs in the first slave node while replication in the second slave node is being delayed due to restart
*   In a situation where a failover re-occurs before replication reflection of a new master node is not complete due to frequent failover

In addition, if the mode of replication log copy process is ASYNC, the data in the CUBRID HA group may lose integrity.

If the data in the CUBRID HA group loses integrity for any of the reasons above, you can fix it by using :ref:`rebuilding-replication`.

**Remark**

The path of a transaction log in these configurations is as follows:

.. image:: /images/image33.png

Load Balancing Structure
------------------------

The load balancing structure increases the availability of the CUBRID service by placing several nodes in the HA configuration (one master node and one slave node) and distributes read-load.

Because the replica nodes receive replication logs from the nodes in the HA configuration and maintain the same data, and because the nodes in the HA configuration do not receive replication logs from the replica nodes, its network and disk usage rate is lower than that of the multiple-slave structure.

Because replica nodes are not included in the HA structure, they provide read service without failover, even when all other nodes in the HA structure fail.

**An Example of Node Configuration**

.. image:: /images/image34.png

You can configure each node in load balancing structure as shown below:

*   **node A** (master node)

    * Configure the **ha_mode** of the **cubrid.conf** file to **on**. ::

        ha_mode=on

    * The following example shows how to configure **cubrid_ha.conf**: ::

        ha_port_id=59901
        ha_node_list=cubrid@nodeA:nodeB 
        ha_replica_list=cubrid@nodeC:nodeD
        ha_db_list=testdb

*   **node B** (slave node): Configure this node in the same manner as *node A*.

*   **node C** (replica node)

    * Configure the **ha_mode** of the **cubrid.conf** file to **replica**. ::

        ha_mode=replica

    * You can configure the **cubrid_ha.conf** file in the same manner as *node A*.

*   **node D** (replica node): Configure this node in the same manner as *node C*.

You must enter the list of DB server hosts in the order so that each broker can be connected appropriate HA or load balancing server in the **databases.txt** file of a broker node.

The following is an example of the **databases.txt** file in *node A* and *node B*. ::

    #db-name    vol-path                  db-host       log-path             lob-base-path
    testdb     /home/cubrid/DB/testdb1   nodeA:nodeB   /home/cubrid/DB/testdb/log file:/home/cubrid/CUBRID/testdb/lob

The following is an example of the **databases.txt** file in *node C*. ::

    #db-name    vol-path                  db-host       log-path             lob-base-path
    testdb     /home/cubrid/DB/testdb   nodeC   /home/cubrid/DB/testdb/log        file:/home/cubrid/CUBRID/testdb/lob

The following is an example the **databases.txt** in *node D*. ::

    #db-name    vol-path                  db-host       log-path             lob-base-path
    testdb     /home/cubrid/DB/testdb   nodeD   /home/cubrid/DB/testdb/log file:/home/cubrid/CUBRID/testdb/lob

The **cubrid_broker.conf** can be set in a variety of ways according to configuration of the broker. It can also be configured as separate equipment with the **databases.txt** file.

In this example, the RW broker is configured in *node A* and *node B* and the PHRO broker is configured in *node C* and *node D*.

The following is an example of **cubrid_broker.conf** in *node A* and *node B*. ::

    [%RW_broker]
    ...
     
    # Broker mode setting parameter
    ACCESS_MODE             =RW

The following is an example **cubrid_broker.conf** in *node C*. ::

    [%PHRO_broker]
    ...
     
    # Broker mode setting parameter
    ACCESS_MODE             =PHRO
    PREFERRED_HOSTS         =nodeC:nodeD

The following is an example **cubrid_broker.conf** in *node D*. ::

    [%PHRO_broker]
    ...
     
    # Broker mode setting parameter
    ACCESS_MODE             =PHRO
    PREFERRED_HOSTS         =nodeD:nodeC

**Connection Configuration of Applications**

Connect the application to access in read/write mode to the broker of *node A* or *node B*. The following is an example of a JDBC application.

.. code-block:: java

    Connection connection = DriverManager.getConnection(
        "jdbc:CUBRID:nodeA:33000:testdb:::?charSet=utf-8&altHosts=nodeB:33000", "dba", "");

Connect the application to access in read-only mode to the broker of *node C* or *node D*. The following is an example of a JDBC application.

.. code-block:: java

    Connection connection = DriverManager.getConnection(
        "jdbc:CUBRID:nodeC:33000:testdb:::?charSet=utf-8&altHosts=nodeD:33000", "dba", "");

For details, see :ref:`ha-jdbc-conf`, :ref:`ha-cci-conf`, and :ref:`ha-php-conf` in Environment Configuration.

**Remark**

The path of a transaction log in these configurations is as follows:

.. image:: /images/image35.png

Multiple-Standby Server Structure
---------------------------------

Although its node structure has a single master node and a single slave node, many slave nodes from different services are physically configured in a single server.

This structure is for very small services in which the read load of slave nodes are light. It is strictly for the availability of the CUBRID service. For this reason, when a master node that failed after a failover has been restored, the load must be moved back to the original master node to minimize the load of the server with multiple-slave nodes.

.. image:: /images/image36.png

**An Example of Node Configuration**

You can configure each node in the basic structure of HA as shown below:

*   **node AM**, **node AS** : Configure them in the same manner.

    * Configure the **ha_mode** of the **cubrid.conf** file to **on**. ::

        ha_mode=on

    * The following example shows how to configure **cubrid_ha.conf**. ::

        ha_port_id=10000
        ha_node_list=cubridA@Host1:Host5
        ha_db_list=testdbA1,testdbA2

*   **node BM**, **node BS** : Configure them in the same manner.

    * Configure the **ha_mode** of the **cubrid.conf** file to **on**. ::

        ha_mode=on

    * The following example shows how to configure **cubrid_ha.conf**. ::

        ha_port_id=10001
        ha_node_list=cubridB@Host2:Host5
        ha_db_list=testdbB1,testdbB2

*   **node CM**, **node CS** : Configure them in the same manner.

    * Configure the **ha_mode** of the **cubrid.conf** file to **on**. ::

        ha_mode=on

    * The following example shows how to configure **cubrid_ha.conf**. ::

        ha_port_id=10002
        ha_node_list=cubridC@Host3:Host5
        ha_db_list=testdbC1,testdbC2

*   **node DM**, **node DS** : Configure them in the same manner.

    * Configure the **ha_mode** of the **cubrid.conf** file to **on**. ::

        ha_mode=on

    * The following is an example of the **cubrid_ha.conf** configuration. ::

        ha_port_id=10003
        ha_node_list=cubridD@Host4:Host5
        ha_db_list=testdbD1,testdbD2

Constraints
===========

**Supported Platforms**

Currently, CUBRID HA is supported by Linux only. All nodes within CUBRID HA groups must be configured on the same platforms.

**Table Primary Key**

CUBRID HA synchronizes data among nodes with the following method (as known as transaction log shipping): It replicates the primary key-based replication logs generated from the server of a master node to a slave node and then reflects the replication logs to the slave node.

If data of the specific table within CUBRID HA groups is not synchronized, you should check whether the appropriate primary key has specified for the table.

On the partitioned table, the table which has promoted some partitions by the **PROMOTE** statement replicates all data to the slave. However, since the table does not have the primary key, the data changes on the table made by the master are not applied to the slave.

**Java Stored Procedure**

Because using java stored procedures in CUBRID HA cannot be replicated, java stored procedures should be configured to all nodes. For more details, see :ref:`jsp-environment-configuration`.

**Method and CUBRID Manager**

CUBRID HA synchronizes data among nodes within CUBRID HA groups based on replication logs. Therefore, using method that does not generate replication logs or configuring **NOT NULL** through CUBRID Manager may cause data inconsistency among nodes within CUBRID HA groups. Therefore, in CUBRID HA environment, it is not recommended to use method and other menus of CUBRID Manager except for the query processor.

**UPDATE STATISTICS Statement**

The **UPDATE STATISTICS** statement which updates statistics is not replicated to the slave node.

**Standalone Mode**

The replication logs are not generated as for tasks performed in standalone mode. For this reason, data inconsistency among nodes within CUBRID HA groups may occur when performing tasks in standalone mode.

**Serial Cache**

To enhance performance, a serial cache does not access Heap and does not generate replication logs when retrieving or updating serial information. Therefore, if you use a serial cache, the current values of serial caches will be inconsistent among the nodes within CUBRID HA groups.

**cubrid backupdb -r**

This command is used to back up a specified database. If the **-r** option is used, logs that are not required for recovery will be deleted. This deletion may result in data inconsistency among nodes within CUBRID HA groups. Therefore, you must not use the **-r** option.

**INCR/DECR Functions**

If you use **INCR** / **DECR** (click counter functions) in a slave node of HA configuration, an error is returned.

**LOB (BLOB/CLOB) Type**

In a CUBRID HA environment, the meta data (Locator) of a **LOB** column is replicated and **LOB** data is not replicated. Therefore, if storage of a **LOB** type is located on the local machine, no tasks corresponding to columns are allowed in slave nodes or master nodes after failover.

.. note::

    On previous version of CUBRID 9.1, using triggers in CUBRID HA can cause duplicate executions. This may cause data inconsistency among nodes within CUBRID HA groups. Therefore, you should not use triggers on the previous version of 9.1.

Operational Scenarios
=====================

Scenario of Building New Slave Node
-----------------------------------

This scenario involves building a new slave node while operating a single master node, making a 1:1 master-slave scheme. Please note that only tables with a default key can be replicated. In addition, all of the volume directories of the master node and the slave node must be identical.

This scenario assumes that the database has been created using the **cubrid createdb testdb -L $CUBRID_DATABASES/testdb/log** command. At this time, the backup file is saved in the $CUBRID_DATABASES/testdb directory by default if the location is not specified.

Using the above instructions, build a new slave node by following these steps, in the order specified.

#. Stop the master node service. ::

    [nodeA]$ cubrid service stop

#. Set the master node HA and the slave node HA.

    * Set the **$CUBRID/conf/cubrid.conf** as identical for both the master node and the slave node. ::

        ...

        [common]
        service=server,broker,manager

        # Add the database name to run when starting the service
        server=testdb

        ...

        # Add when configuring the HA (Logging parameters)
        log_max_archives=100
        force_remove_log_archives=no

        # Add when configuring the HA (HA mode)
        ha_mode=on

    * Set the **$CUBRID/conf/cubrid_ha.conf** as identical for both the master node and the slave node. ::

        [common]
        ha_port_id=59901
        ha_node_list=cubrid@nodeA:nodeB
        ha_db_list=testdb
        ha_copy_sync_mode=sync:sync
        ha_apply_max_mem_size=500

    * Set the **$CUBRID_DATABASES/databases.txt** as identical for both the master node and the slave node. ::

        #db-name    vol-path        db-host     log-path     lob-base-path
        testdb       /home/cubrid/DB/testdb nodeA:nodeB   /home/cubrid/DB/testdb/log  file:/home/cubrid/DB/testdb/lob

        
    * Create a database directory to the slave node. ::
    
        [nodeB]$ cd $CUBRID_DATABASES
        [nodeB]$ mkdir testdb

    * Create the log directory to the slave node(same location with the master node). ::

        [nodeB]$ cd $CUBRID_DATABASES/testdb
        [nodeB]$ mkdir log

#. Back up the database of the master node and copy the backup file to the slave node. If the location where the backup file will be saved in the master node is not specified, the location is set as the log directory of *testdb* by default. Copy the backup file to the same location in the slave node. *testdb* _bk0v000 is the backup volume file and *testdb* _bkvinf is the backup volume information file. ::

    [nodeA]$ cubrid backupdb -z -S testdb
    Backup Volume Label: Level: 0, Unit: 0, Database testdb, Backup Time: Thu Apr 19 16:05:18 2012
    [nodeA]$ cd $CUBRID_DATABASES/testdb/log
    [nodeA]$ scp testdb_bk* cubrid_usr@nodeB:/home/cubrid_usr/CUBRID/databases/testdb/log
    cubrid_usr@nodeB's password:
    testdb_bk0v000                            100% 6157KB   6.0MB/s   00:00
    testdb_bkvinf                             100%   66     0.1KB/s   00:00

#. Recover the database in the slave node. At this time, the volume path of the master node must be identical to that of the slave node. ::

    [nodeB]$ cubrid restoredb -B $CUBRID_DATABASES/testdb/log demodb
    
#. Start the master node ::

    [nodeA]$ cubrid heartbeat start

#. After confirming that the master node has started, start the slave node. If *nodeA* is changed from to-be-master to master, it means that the master node has been successfully started. ::

    [nodeA]$ cubrid heartbeat status
    @ cubrid heartbeat status
     
     HA-Node Info (current nodeA, state master)
       Node nodeB (priority 2, state unknown)
       Node nodeA (priority 1, state master)
     
     HA-Process Info (master 123, state master)
     
       Applylogdb testdb@localhost:/home1/cubrid/DB/tdb01_nodeB (pid 234, state registered)
       Copylogdb testdb@nodeB:/home1/cubrid/DB/tdb01_nodeB (pid 345, state registered)
       Server tdb01 (pid 456, state registered_and_to_be_active)
     
    [nodeB]$ cubrid heartbeat start

#. Confirm that the HA configurations of the master node and the slave node are successfully running ::

    [nodeA]$ csql -u dba testdb@localhost -c"create table tbl(i int primary key);insert into tbl values (1),(2),(3)"
     
    [nodeB]$ csql -u dba testdb@localhost -c"select * from tbl"
     
    === <Result of SELECT Command in Line 1> ===
     
                i
    =============
                1
                2
                3

Operation Scenario during Read/Write Service
--------------------------------------------

The operation scenario written in this page is not affected by read/write services. Therefore, its impact on the services caused by CUBRID operation is very limited. There can be two types of operation scenarios in which failover occurs or it does not occur.

**When Failover Does Not Occur**

You can perform the following operations without stopping and restarting nodes in CUBRID HA groups.

+----------------------------------------------+-------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| **General Operation**                        | **Scenario**                                                            | **Consideration**                                                                                                                                      |
|                                              |                                                                         |                                                                                                                                                        |
+==============================================+=========================================================================+========================================================================================================================================================+
| Online Backup                                | Operation task is performed at each master node and slave node          | Note that there may be a delay in the transaction of master node due to the operation task.                                                            |
|                                              | each during operation.                                                  |                                                                                                                                                        |
+----------------------------------------------+-------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| Schema change (excluding basic key change),  | When an operation task occurs at a master node, it is automatically     | Because replication log is copied and reflected to a slave node after an operation task is completed in a master node, operation task time is doubled. |
| index change, authorization change           | replication reflected to a slave node.                                  | Changing schema must be processed without any failover.                                                                                                |
|                                              |                                                                         | Index change and authority change other than the schema change can be performed by stopping each node and executing standalone mode (ex: the           |
|                                              |                                                                         | **-S**                                                                                                                                                 |
|                                              |                                                                         | option of the                                                                                                                                          |
|                                              |                                                                         | **csql**                                                                                                                                               |
|                                              |                                                                         | utility) when the operation time is important.                                                                                                         |
|                                              |                                                                         |                                                                                                                                                        |
+----------------------------------------------+-------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| Add volume                                   | Operation task is performed at each DB regardless of HA structure.      | Note that there may be a delay in the transaction of master node due to the operation task.                                                            |
|                                              |                                                                         | If operation task time is an issue, operation task can be performed by stopping each node and executing standalone mode (ex: the                       |
|                                              |                                                                         | **-S**                                                                                                                                                 |
|                                              |                                                                         | of the                                                                                                                                                 |
|                                              |                                                                         | **cubrid addvoldb**                                                                                                                                    |
|                                              |                                                                         | utility).                                                                                                                                              |
|                                              |                                                                         |                                                                                                                                                        |
+----------------------------------------------+-------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| Failure node server replacement              | It can be replaced without restarting the CUBRID HA group when          | The failure node must be registered in the ha_node_list of CUBRID HA group, and the node name must not be changed during replacement.                  |
|                                              | a failure occurs.                                                       |                                                                                                                                                        |
+----------------------------------------------+-------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| Failure broker server replacement            | It can be replaced without restarting the broker when a failure occurs. | The connection to a broker replaced at a client can be made by rcTime which is configured in URL string.                                               |
|                                              |                                                                         |                                                                                                                                                        |
+----------------------------------------------+-------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| DB server expansion                          | You can execute                                                         | Starts or stops the                                                                                                                                    |
|                                              | **cubrid heartbeat reload**                                             | **copylogdb/applylogdb**                                                                                                                               |
|                                              | in each node after configuration change (ha_node_list, ha_replica_list) | processes which were added or deleted by loading changed configuration information.                                                                    |
|                                              | without restarting the previously configured CUBRID HA group.           |                                                                                                                                                        |
+----------------------------------------------+-------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
| Broker server expansion                      | Run additional brokers without restarting existing brokers.             | Modify the URL string to connect to a broker where a client is added.                                                                                  |
|                                              |                                                                         |                                                                                                                                                        |
+----------------------------------------------+-------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+

**When Failover Occurs**

You must stop nodes in CUBRID HA group and complete operation before performing the following operations. 

+------------------------------------------------------------+--------------------------------------------------------------------------------+---------------------------------------------------------------------------------------------+
| **General Operation**                                      | **Scenario**                                                                   | **Consideration**                                                                           |
|                                                            |                                                                                |                                                                                             |
+============================================================+================================================================================+=============================================================================================+
| DB server configuration change                             | A node whose configuration is changed is restarted when the configuration in   |                                                                                             |
|                                                            | **cubrid.conf**                                                                |                                                                                             |
|                                                            | is changed.                                                                    |                                                                                             |
|                                                            |                                                                                |                                                                                             |
+------------------------------------------------------------+--------------------------------------------------------------------------------+---------------------------------------------------------------------------------------------+
| Change broker configuration, add broker, and delete broker | A broker whose configuration is changed is restarted when the configuration in |                                                                                             |
|                                                            | **cubrid_broker.conf**                                                         |                                                                                             |
|                                                            | is changed.                                                                    |                                                                                             |
|                                                            |                                                                                |                                                                                             |
+------------------------------------------------------------+--------------------------------------------------------------------------------+---------------------------------------------------------------------------------------------+
| DBMS version patch                                         | Restart nodes and brokers in HA group after version patch.                     | Version patch means there is no change in the internal protocol, volume, and log of CUBRID. |
|                                                            |                                                                                |                                                                                             |
+------------------------------------------------------------+--------------------------------------------------------------------------------+---------------------------------------------------------------------------------------------+

Operation Scenario during Read Service
--------------------------------------

The operation scenario written in this page is only applied to read service. It is required to allow read service only or dynamically change mode configuration of broker to Read Only. There can be two types of operation scenarios in which failover occurs or it does not occur.

**When Failover Does Not Occur**

You can perform the following operations without stopping and restarting nodes in CUBRID HA groups.

+--------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **General Operation**                                                          | **Scenario**                                                                                             | **Consideration**                                                                                                                                                                                                                                                                                         |
|                                                                                |                                                                                                          |                                                                                                                                                                                                                                                                                                           |
+================================================================================+==========================================================================================================+===========================================================================================================================================================================================================================================================================================================+
| Schema change (primary key change)                                             | When an operation task is performed at the master node, it is automatically reflected to the slave node. | In order to change the primary key, the existing key must be deleted and a new one added. For this reason, replication reflection may not occur due to the HA internal structure which reflects primary key-based replication logs. Therefore, operation tasks must be performed during the read service. |
|                                                                                |                                                                                                          |                                                                                                                                                                                                                                                                                                           |
+--------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Schema change (excluding basic key change), index change, authorization change | When an operation task is performed at the master node, it is automatically reflected to the slave node. | Because replication log is copied and reflected to a slave node after an operation task is completed in a master node, operation task time is doubled.                                                                                                                                                    |
|                                                                                |                                                                                                          | Changing schema must be processed without any failover.                                                                                                                                                                                                                                                   |
|                                                                                |                                                                                                          | Index change and authority change other than the schema change can be performed by stopping each node and executing standalone mode (ex: the span class="nkeyword">-S option of                                                                                                                           |
|                                                                                |                                                                                                          | **csql**                                                                                                                                                                                                                                                                                                  |
|                                                                                |                                                                                                          | ) when the operation time is important.                                                                                                                                                                                                                                                                   |
|                                                                                |                                                                                                          |                                                                                                                                                                                                                                                                                                           |
+--------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

**When Failover Occurs**

You must stop nodes in CUBRID HA group and complete operation before performing the following operations. 

+------------------------------------------------+-------------------------------------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **General Operation**                          | **Scenario**                                                                              | **Consideration**                                                                                                                                                                                                                                                                              |
|                                                |                                                                                           |                                                                                                                                                                                                                                                                                                |
+================================================+===========================================================================================+================================================================================================================================================================================================================================================================================================+
| DBMS version upgrade                           | Restart each node and broker in the CUBRID HA group after they are upgraded.              | A version upgrade means that there have been changed in the internal protocol, volume, or log of CUBRID.                                                                                                                                                                                       |
|                                                |                                                                                           | Because there are two different versions of the protocols, volumes, and logs of a broker and server during an upgrade, an operation task must be performed to make sure that each client and broker (before/after upgrade) are connected to the corresponding counterpart in the same version. |
|                                                |                                                                                           |                                                                                                                                                                                                                                                                                                |
+------------------------------------------------+-------------------------------------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Massive data processing (INSERT/UPDATE/DELETE) | Stop the node that must be changed, perform an operation task, and then execute the node. | This processes massive data that cannot be segmented.                                                                                                                                                                                                                                          |
|                                                |                                                                                           |                                                                                                                                                                                                                                                                                                |
+------------------------------------------------+-------------------------------------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

Operation Scenario after Service Stop
-------------------------------------

You must stop all nodes in CUBRID HA group before performing the following operation.

+----------------------------------------------+-----------------------------------------------------------------------------------+----------------------------------------------------------+
| **General Operation**                        | **Scenario**                                                                      | **Consideration**                                        |
|                                              |                                                                                   |                                                          |
+==============================================+===================================================================================+==========================================================+
| Changing the host name and IP of a DB server | Stop all nodes in the CUBRID HA group, and restart them after the operation task. | When a host name has been changed, change the            |
|                                              |                                                                                   | **databases.txt**                                        |
|                                              |                                                                                   | file of each broker and reset the broker connection with |
|                                              |                                                                                   | **cubrid broker reset**                                  |
|                                              |                                                                                   | .                                                        |
|                                              |                                                                                   |                                                          |
+----------------------------------------------+-----------------------------------------------------------------------------------+----------------------------------------------------------+

Detection of Replication Mismatch and Rebuild
=============================================

Detection of Replication Mismatch
---------------------------------

Replication mismatch between replication nodes, indicating that data of the master node and the slave node is not identical, can be detected to some degree by the following process. However, please note that there is no more accurate way to detect a replication mismatch than by directly comparing the data of the master node to the data of the slave node. If it is determined that there has been a replication mismatch, you should rebuild the database of the master node to the slave node (see :ref:`rebuilding-replication`.)

* On the slave node, execute **cubrid applyinfo** to check the "Fail count" value. If the "Fail count" is 0, it can be determined that no transaction has failed in replication (see :ref:`cubrid-applyinfo`.) ::

    [nodeB]$ cubrid applyinfo -L /home/cubrid/DB/testdb_nodeA -r nodeA -a testdb
     
     *** Applied Info. ***
    Committed page                 : 1913 | 2904
    Insert count                   : 645
    Update count                   : 0
    Delete count                   : 0
    Schema count                   : 60
    Commit count                   : 15
    Fail count                     : 0
    ...

* To check whether copying replication logs has been delayed or not on the slave node, execute **cubrid applyinfo** and compare the "Append LSA" value of "Copied Active Info." to the "Append LSA" value of "Active Info.". If there is a big difference between the two values, it means that delay has occurred while copying the replication logs to the slave node (see :ref:`cubrid-applyinfo`.) ::

    [nodeB]$ cubrid applyinfo -L /home/cubrid/DB/testdb_nodeA -r nodeA -a testdb
 
    ...
     
     *** Copied Active Info. ***
    DB name                        : testdb
    DB creation time               : 11:28:00.000 AM 12/17/2010  (1292552880)
    EOF LSA                        : 1913 | 2976
    Append LSA                     : 1913 | 2976
    HA server state                : active
     
     ***  Active Info. ***
    DB name                        : testdb
    DB creation time               : 11:28:00.000 AM 12/17/2010  (1292552880)
    EOF LSA                        : 1913 | 2976
    Append LSA                     : 1913 | 2976
    HA server state                : active

* If a delay seems to occur when copying the replication logs, check whether the network line speed is slow, whether there is sufficient free disk space, disk I/O is normal, etc.

* To check the delay in applying the replication log in the slave node, execute **cubrid applyinfo** and compare the "Committed page" value of "Applied Info." to the "EOF LSA" value of "Copied Active Info.". If there is a big difference between the two values, it means that a delay has occurred while applying the replication logs to the slave database (see :ref:`cubrid-applyinfo`.) ::

    [nodeB]$ cubrid applyinfo -L /home/cubrid/DB/testdb_nodeA -r nodeA -a testdb
 
     *** Applied Info. ***
    Committed page                 : 1913 | 2904
    Insert count                   : 645
    Update count                   : 0
    Delete count                   : 0
    Schema count                   : 60
    Commit count                   : 15
    Fail count                     : 0
     
     *** Copied Active Info. ***
    DB name                        : testdb
    DB creation time               : 11:28:00.000 AM 12/17/2010  (1292552880)
    EOF LSA                        : 1913 | 2976
    Append LSA                     : 1913 | 2976
    HA server state                : active
    ...


* If the delay in applying the replication logs is too long, it may be due to a transaction with a long execution time. If the transaction is performed normally, a delay in applying the replication logs may normally occur. To determine whether it is normal or abnormal, continuously execute **cubrid applyinfo** and check whether applylogdb continuously applies replication logs to the slave node or not.

* Check the error log message created by the copylogdb process and the applylogdb process (see the error message).
* Compare the number of records on the master database table to that on the slave database table.

Error Messages
--------------

**Replication Log Copy Process (copylogdb)**

    The error messages from the replication log copy process are stored in **$CUBRID/log/db-name@remote-node-name_copylogdb.err**. The severity levels of error messages found in the replication log copy process are as follows: fatal, error, and notification. The default level is error. Therefore, to record notification error messages, it is necessary to change the value of **error_log_level** in the **cubrid.conf** file. For details, see :ref:`error-parameters`.

**Initialization Error Messages**

The error messages that can be found in initialization stage of replication log copy process are as follows:

+----------------+-----------------------------------------------------------------------------------------------+--------------+-------------------------------------------------------------------------------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **Error Code** | **Error Message**                                                                             | **Severity** | **Description**                                                                                                                     | **Solution**                                                                                                                                                                                                              |
|                |                                                                                               |              |                                                                                                                                     |                                                                                                                                                                                                                           |
+================+===============================================================================================+==============+=====================================================================================================================================+===========================================================================================================================================================================================================================+
| 10             | Cannot mount the disk volume ?.                                                               | error        | Fails to open a replication log file.                                                                                               | Check if replication logs exist. For the location of replication logs, see                                                                                                                                                |
|                |                                                                                               |              |                                                                                                                                     | `Default Environment Configuration <#admin_admin_ha_conf_ha_htm>`_                                                                                                                                                        |
|                |                                                                                               |              |                                                                                                                                     | .                                                                                                                                                                                                                         |
|                |                                                                                               |              |                                                                                                                                     |                                                                                                                                                                                                                           |
+----------------+-----------------------------------------------------------------------------------------------+--------------+-------------------------------------------------------------------------------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 78             | Internal error: an I/O error occurred while reading logical log page ? (physical page ?) of ? | fatal        | Fails to read a replication log.                                                                                                    | Check the replication log by using the cubrid applyinfo utility.                                                                                                                                                          |
|                |                                                                                               |              |                                                                                                                                     |                                                                                                                                                                                                                           |
+----------------+-----------------------------------------------------------------------------------------------+--------------+-------------------------------------------------------------------------------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 81             | Internal error: logical log page ? may be corrupted.                                          | fatal        | A replication log page error, in which the replication log copy process has been copied from the connected database server process. | Check the error log of the database server process to which the replication log copy process is connected.                                                                                                                |
|                |                                                                                               |              |                                                                                                                                     | This error log can be found in $CUBRID/log/server.                                                                                                                                                                        |
|                |                                                                                               |              |                                                                                                                                     |                                                                                                                                                                                                                           |
+----------------+-----------------------------------------------------------------------------------------------+--------------+-------------------------------------------------------------------------------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1039           | log writer: log writer has been started. mode: ?                                              | error        | The replication log copy process has been successfully initialized and started.                                                     | No action is required because this error message is recorded to display the start information of the replication log copy process.                                                                                        |
|                |                                                                                               |              |                                                                                                                                     | Ignore any error messages which are displayed between the start of replication log copy process and output of this error message since there is a possibility that an error message is shown up even in normal situation. |
|                |                                                                                               |              |                                                                                                                                     |                                                                                                                                                                                                                           |
+----------------+-----------------------------------------------------------------------------------------------+--------------+-------------------------------------------------------------------------------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

**Replication Log Request and Reception Error Messages**

The replication log copy process requests a replication log from the connected database server and receives the corresponding replication log. Error messages that can be found in this stage are as follows:

+----------------+----------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **Error Code** | **Error Message**                            | **Severity** | **Description**                                                                                                          | **Solution**                                                                                                                                                                                                                                    |
|                |                                              |              |                                                                                                                          |                                                                                                                                                                                                                                                 |
+================+==============================================+==============+==========================================================================================================================+=================================================================================================================================================================================================================================================+
| 89             | Log ? is not included in the given database. | error        | The previously replicated log and the log to be replication do not match.                                                | Check information of the database server/host to which the replication log copy process is connected. If you need to change the database server/host information, reinitialize it by deleting the existing replication log and then restarting. |
|                |                                              |              |                                                                                                                          |                                                                                                                                                                                                                                                 |
+----------------+----------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 186            | Data receiver error from the server          | error        | Incorrect information has been received from the database server to which the replication log copy process is connected. | It will be internally recovered.                                                                                                                                                                                                                |
|                |                                              |              |                                                                                                                          |                                                                                                                                                                                                                                                 |
+----------------+----------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 199            | The server is not responding.                | error        | The connection to the database server has been terminated.                                                               | It will be internally recovered.                                                                                                                                                                                                                |
|                |                                              |              |                                                                                                                          |                                                                                                                                                                                                                                                 |
+----------------+----------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

**Replication Log Write Error Messages**

The replication log copy process copies the replication log that was received from the connected database server process to the location (**ha_copy_log_base**) specified in the **cubrid_ha.conf** file. Error messages that can be found in this stage are as follows:

+----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------+-------------------------------------------------------------------------+----------------------------------------------------------------+
| **Error Code** | **Error Message**                                                                                                                                                  | **Severity** | **Description**                                                         | **Solution**                                                   |
|                |                                                                                                                                                                    |              |                                                                         |                                                                |
+================+====================================================================================================================================================================+==============+=========================================================================+================================================================+
| 10             | Cannot mount the disk volume ?.                                                                                                                                    | error        | Fails to open a replication log file.                                   | Check if replication logs exist.                               |
|                |                                                                                                                                                                    |              |                                                                         |                                                                |
+----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------+-------------------------------------------------------------------------+----------------------------------------------------------------+
| 79             | Internal error: an I/O error occurred while writing logical log page ? (physical page ?) of ?.                                                                     | fatal        | Fails to write a replication log.                                       | It will be internally recovered.                               |
|                |                                                                                                                                                                    |              |                                                                         |                                                                |
+----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------+-------------------------------------------------------------------------+----------------------------------------------------------------+
| 80             | An error occurred due to insufficient space in operating system device while writing logical log page ? (physical page ?) of ?. Up to ? bytes in size are allowed. | fatal        | Fails to write a replication log due to insufficient file system space. | Check if there is sufficient space left in the disk partition. |
|                |                                                                                                                                                                    |              |                                                                         |                                                                |
+----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------+-------------------------------------------------------------------------+----------------------------------------------------------------+

**Replication Log Archive Error Messages**

The replication log copy process periodically archives the replication logs that have been received from the connected database server process. Error messages that can be found in this stage are as follows:

+----------------+------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------+------------------------------------------------------------------------------------------------------------+
| **Error Code** | **Error Message**                                                                              | **Severity** | **Description**                                         | **Solution**                                                                                               |
|                |                                                                                                |              |                                                         |                                                                                                            |
+================+================================================================================================+==============+=========================================================+============================================================================================================+
| 78             | Internal error: an I/O error occurred while reading logical log page ? (physical page ?) of ?. | fatal        | Fails to read a replication log during archiving.       | Check the replication log by using the cubrid applyinfo utility.                                           |
|                |                                                                                                |              |                                                         |                                                                                                            |
+----------------+------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------+------------------------------------------------------------------------------------------------------------+
| 79             | Internal error: an I/O error occurred while writing logical log page ? (physical page ?) of ?. | fatal        | Fails to write an archive log.                          | It will be internally recovered.                                                                           |
|                |                                                                                                |              |                                                         |                                                                                                            |
+----------------+------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------+------------------------------------------------------------------------------------------------------------+
| 81             | Internal error: logical log page ? may be corrupted.                                           | fatal        | Found an error on the replication log during archiving. | Check the replication log by using the cubrid applyinfo utility.                                           |
|                |                                                                                                |              |                                                         |                                                                                                            |
+----------------+------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------+------------------------------------------------------------------------------------------------------------+
| 98             | Cannot create an archive log ? to archive pages from ? to ?.                                   | fatal        | Fails to create an archive log file.                    | Check if there is sufficient space left in the disk partition.                                             |
|                |                                                                                                |              |                                                         |                                                                                                            |
+----------------+------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------+------------------------------------------------------------------------------------------------------------+
| 974            | An archive log ? to archive pages from ? to ? has been created.                                | notification | Information on an archive log file                      | No action is required because this error message is recorded to keep information on newly created archive. |
|                |                                                                                                |              |                                                         |                                                                                                            |
+----------------+------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------+------------------------------------------------------------------------------------------------------------+

**Stop and Restart Error Messages**

Error messages that can be found in this stage are as follows:

+----------------+-------------------------------------------------+--------------+------------------------------------------------------------------+----------------------------------+
| **Error Code** | **Error Message**                               | **Severity** | **Description**                                                  | **Solution**                     |
|                |                                                 |              |                                                                  |                                  |
+================+=================================================+==============+==================================================================+==================================+
| 1037           | log writer: log writer is terminated by signal. | error        | The copylogdb process has been terminated by a specified signal. | It will be internally recovered. |
|                |                                                 |              |                                                                  |                                  |
+----------------+-------------------------------------------------+--------------+------------------------------------------------------------------+----------------------------------+

**Replication Log Reflection Process (applylogdb)**

The error messages from the replication log reflection process are stored in **$CUBRID/log/**\ *db-name*\ **@**\ *local-node-name*\ **_applylogdb_**\ *db-name*\ **_**\ *remote-node-name*\ **.err**. The severity levels of error message found in the replication log reflection process are as follow: fatal, error, and notification. The default level is error. Therefore, to record notification error messages, it is necessary to change the value of **error_log_level** in the **cubrid.conf** file. For details, see :ref:`error-parameters`.

**Initialization Error Messages**

The error messages that can be found in initialization stage of replication log reflection process are as follows:

+----------------+----------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------+
| **Error Code** | **Error Message**                                                                      | **Severity** | **Description**                                                                      | **Solution**                                                                                                                     |
|                |                                                                                        |              |                                                                                      |                                                                                                                                  |
+================+========================================================================================+==============+======================================================================================+==================================================================================================================================+
| 10             | Cannot mount the disk volume ?.                                                        | error        | An applylogdb that is trying to reflect the same replication log is already running. | Check if there is an applylogdb process that is trying to reflect the same replication log.                                      |
|                |                                                                                        |              |                                                                                      |                                                                                                                                  |
+----------------+----------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------+
| 1038           | log applier: log applier has been started. required LSA: ?|?. last committed LSA: ?|?. | error        | It will be started normally after initialization of applylogdb succeeds.             | No action is required because this error is recorded to display the start information of the replication log reflection process. |
|                |                                                                                        |              |                                                                                      |                                                                                                                                  |
+----------------+----------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------+

**Log Analysis Error Messages**

The replication log reflection process reads, analyzes, and reflects the replications logs that have been copied by the replication log copy process. The error message that can be found in this stage are as follows:

+----------------+----------------------------------------------------------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------+
| **Error Code** | **Error Message**                                                                                                                      | **Severity** | **Description**                                                                                                                            | **Solution**                                                     |
|                |                                                                                                                                        |              |                                                                                                                                            |                                                                  |
+================+========================================================================================================================================+==============+============================================================================================================================================+==================================================================+
| 13             | An I/O error occurred while reading page ? in volume ?.                                                                                | error        | Fails to read a log page to be reflected.                                                                                                  | Check the replication log by using the cubrid applyinfo utility. |
|                |                                                                                                                                        |              |                                                                                                                                            |                                                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------+
| 17             | Internal error: Trying to read page ? of the volume ? which has been already released.                                                 | fatal        | Trying to read a log page that does not exist in the replication log.                                                                      | Check the replication log by using the cubrid applyinfo utility. |
|                |                                                                                                                                        |              |                                                                                                                                            |                                                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------+
| 81             | Internal error: logical log page ? may be corrupted.                                                                                   | fatal        | There is an inconsistency between an old log under replication reflection and the current log, or there is a replication log record error. | Check the replication log by using the cubrid applyinfo utility. |
|                |                                                                                                                                        |              |                                                                                                                                            |                                                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------+
| 82             | Cannot mount the disk volume/file ?.                                                                                                   | error        | No replication log file exists.                                                                                                            | Check if replication logs exist.                                 |
|                |                                                                                                                                        |              |                                                                                                                                            | Check the replication log by using the cubrid applyinfo utility. |
|                |                                                                                                                                        |              |                                                                                                                                            |                                                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------+
| 97             | Internal error: unable to find log page ? in log archives.                                                                             | error        | No log page exists in the replication log.                                                                                                 | Check the replication log by using the cubrid applyinfo utility. |
|                |                                                                                                                                        |              |                                                                                                                                            |                                                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------+
| 897            | Decompression failure                                                                                                                  | error        | Fails to decompress the log record.                                                                                                        | Check the replication log by using the cubrid applyinfo utility. |
|                |                                                                                                                                        |              |                                                                                                                                            |                                                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------+
| 1028           | log applier: Unexpected EOF log record exists in the Archive log. LSA: ?|?.                                                            | error        | Incorrect log record exists in the archive log.                                                                                            | Check the replication log by using the cubrid applyinfo utility. |
|                |                                                                                                                                        |              |                                                                                                                                            |                                                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------+
| 1029           | log applier: Incorrect log page/offset. page HDR: ?|?, final: ?|?, append LSA: ?|?, EOF LSA: ?|?, ha file status: ?, is end-of-log: ?. | error        | Incorrect log record exists.                                                                                                               | Check the replication log by using the cubrid applyinfo utility. |
|                |                                                                                                                                        |              |                                                                                                                                            |                                                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------+
| 1030           | log applier: Incorrect log record. LSA: ?|?, forw LSA: ?|?, backw LSA: ?|?, Trid: ?, prev tran LSA: ?|?, type: ?.                      | error        | Log record header error                                                                                                                    | Check the replication log by using the cubrid applyinfo utility. |
|                |                                                                                                                                        |              |                                                                                                                                            |                                                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------+--------------+--------------------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------+

**Replication Log Reflection Error Messages**

The replication log reflection process reads, analyzes, and reflects the replication logs that have been copied by the replication log copy process. Error messages that can be found in this stage are as follows:

+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| **Error Code** | **Error Message**                                                                                  | **Severity** | **Description**                                                                                                                                                     | **Solution**                                                                                     |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+================+====================================================================================================+==============+=====================================================================================================================================================================+==================================================================================================+
| 72             | The transaction (index ?, ?@?|?) has been cancelled by system.                                     | error        | Fails to reflect replication due to deadlock, etc.                                                                                                                  | It will be recovered internally.                                                                 |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 111            | Your transaction has been cancelled due to server failure or a mode change.                        | error        | Fails to reflect replication because the database server process in which replication is supposed to be reflected has been terminated or its mode has been changed. | It will be recovered internally.                                                                 |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 191            | Cannot connect to server ? on ?.                                                                   | error        | The connection to the database server process in which replication is supposed to be reflected has been terminated.                                                 | It will be recovered internally.                                                                 |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 195            | Server communication error: ?.                                                                     | error        | The connection to the database server process in which replication is supposed to be reflected has been terminated.                                                 | It will be recovered internally.                                                                 |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 224            | The database has not been resumed.                                                                 | error        | The connection to the database server process in which replication is supposed to be reflected has been terminated.                                                 | It will be recovered internally.                                                                 |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 1027           | log applier: Failed to change the reflection status from ? to ?.                                   | error        | Fails to change of replication reflection.                                                                                                                          | It will be recovered internally.                                                                 |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 1031           | log applier: Failed to reflect the Schema replication log. class: ?, schema: ?, internal error: ?. | error        | Fails to reflect SCHEMA replication.                                                                                                                                | Check the consistency of the replication. If it is inconsistent, reconfigure the HA replication. |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 1032           | log applier: Failed to reflect the Insert replication log. class: ?, key: ?, internal error: ?.    | error        | Fails to reflect INSERT replication.                                                                                                                                | Check the consistency of the replication. If it is inconsistent, reconfigure the HA replication. |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 1033           | log applier: Failed to reflect the Update replication log. class: ?, key: ?, internal error: ?.    | error        | Fails to reflect UPDATE replication.                                                                                                                                | Check the consistency of the replication. If it is inconsistent, reconfigure the HA replication. |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 1034           | log applier: Failed to reflect the Delete replication log. class: ?, key: ?, internal error: ?.    | error        | Fails to reflect DELETE replication.                                                                                                                                | Check the consistency of the replication. If it is inconsistent, reconfigure the HA replication. |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 1040           | HA generic: ?.                                                                                     | notification | Changes the last record of the archive log or replication reflection status.                                                                                        | No action is required because this error message is recorded to provide general information.     |
|                |                                                                                                    |              |                                                                                                                                                                     |                                                                                                  |
+----------------+----------------------------------------------------------------------------------------------------+--------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+

**Stop and Restart Error Messages**

The error messages that can be found in this stage are as follows:

+----------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------+----------------------------------------------------------------------------------------------------------+----------------------------------+
| **Error Code** | **Error Message**                                                                                                                                                                                        | **Severity** | **Description**                                                                                          | **Solution**                     |
|                |                                                                                                                                                                                                          |              |                                                                                                          |                                  |
+================+==========================================================================================================================================================================================================+==============+==========================================================================================================+==================================+
| 1035           | log applier: The memory size (? MB) of the log applier is larger than the maximum memory size (? MB), or is doubled the starting memory size (? MB) or more. required LSA: ?|?. last committed LSA: ?|?. | error        | The replication log reflection process has been restarted due to reaching the maximum memory size limit. | It will be recovered internally. |
|                |                                                                                                                                                                                                          |              |                                                                                                          |                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------+----------------------------------------------------------------------------------------------------------+----------------------------------+
| 1036           | log applier: log applier is terminated by signal.                                                                                                                                                        | error        | The replication log reflection process has been terminated by a specified signal.                        | It will be recovered internally. |
|                |                                                                                                                                                                                                          |              |                                                                                                          |                                  |
+----------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------------+----------------------------------------------------------------------------------------------------------+----------------------------------+

.. _rebuilding-replication:

Rebuilding Replication
----------------------

Replication rebuilding is required in CUBRID HA when data in the CUBRID HA group is inconsistent because of multiple failures in multiple-slave node structure, or because of a generic error. Rebuilding replications in CUBRID HA is perform done through a **ha_make_slavedb.sh** script. With the **cubrid applyinfo** utility, you can check the replication progress; however replication inconsistency is not detected. If you want to determine whether replication is inconsistent correctly, you must examine data of the master and slave nodes yourself.

For rebuilding replications, the following environment must be the same in the slave, master, and replica nodes.

*   CUBRID version
*   Environmental variable (**$CUBRID**, **$CUBRID_DATABASES**, **$LD_LIBRARY_PATH, $PATH**)
*   The paths of database volume, log, and replication
*   Username and password of the Linux server
*   HA-related parameters except for **ha_mode** and **ha_copy_sync_mode**, **ha_ping_hosts**

**ha_make_slavedb.sh Script**

To rebuild replications, use the **ha_make_slavedb.sh** script. This script is located in **$CUBRID/share/scripts/ha**. Before rebuilding replications, the following items must be configured for the environment of the user. This script is supported since the version 2008 R2.2 Patch 9 and its configuration is different from 2008 R4.1 Patch 2 or earlier. This document describes it in CUBIRD 2008 R4.1 Patch 2 or later.

*   **target_host** : The host name of the source node (master node in general) for rebuilding replication. It should be registered in **/etc/hosts**. A slave node can be replicated as the master node or the replica node. A replica node can be replicated and rebuilt as another replica node.

*   **repl_log_home** : Specifies the home directory of the replication log of the master node. It is usually the same as **$CUBRID_DATABASES**. You must enter an absolute path and should not use a symbolic link. You also cannot use a slash (/) after the path.

The following are optional items:

*   **db_name** : Specifies the name of the database to be replicated. If not specified, the first name that appears in **ha_db_list** in **$CUBRID/conf/cubrid_ha.conf** is used.
*   **backup_dest_path** : Specifies the path in which the backup volume is created when executing **backupdb** in source node for rebuilding replication.
*   **backup_option** : Specifies necessary options when executing **backupdb** in source node in which replication will be rebuilt.
*   **restore_option** : Specifies necessary options when executing **restoredb** in slave node in which replication will be rebuilt.
*   **scp_option** : Specifies the **scp** option which enables backup of source node in which replication is rebuilt to copy into the slave node. The default option is **-l 131072**, which does not impose an overload on network (limits the transfer rate to 16 MB).

Once the script has been configured, execute the **ha_make_slavedb.sh** script in slave node in which replication will be rebuilt. When the script is executed, rebuilding replication happens in a number of phases. To move to the next stage, the user must enter an appropriate value. The following are the descriptions of available values.

*   **yes** : Keeps going.
*   **no** : Does not move forward with any stages from now on.
*   **skip**: Skips to the next stage. This input value is used to ignore a stage that has not necessarily been executed when retrying the script after it has failed.

**Constraints**

*   Remote ssh connection must be available when using the script because it executes connection commands in the remote node by using expect and ssh.

*   **Online backup of rebuilding replication node** : Existing backup of the replica or slave nodes cannot be used for rebuilding replication. Therefore, you must use the online backup of the master node that is automatically created by the script.

*   **Error while executing the rebuilding replication script** : The rebuilding replication script is not automatically rolled back to its previous stage even when an error occurs during the execution. This is because the slave node cannot provide normal service before rebuilding replication script is executed. To return to the phase before rebuilding replication script is executed, you must back up the existing replication logs and **db_ha_apply_info** information which is internal catalog of the master and slave nodes before building replication is executed.

**Remark**

To replicate, you must copy the physical image of the database volume in the target node to the database of the node to be replicated. However, **cubrid unloaddb** backs up only logical images so replication using **cubrid unloaddb** and **cubrid loaddb** is unavailable. Because **cubrid backupdb** backs up physical images, replication is possible by using this utility. The **ha_make_slavedb.sh** script performs replication by using **cubrid backupdb**.

**Example**

The following example shows how to configure an original node for rebuilding replications as a master mode and rebuild a slave node from the master node.

.. image:: /images/image37.png

*   The host name in master node: *nodeA*
*   The host name in slave node: *nodeB*

Rebuilding replications can be performed while the master node is running, however, it is recommended to execute this when there are just a few transactions per hour in order to minimize replication delay.

Before starting to rebuild replications by executing the **ha_make_slavedb.sh** script, stop the HA service of the slave node and configure the **ha_make_slavedb.sh** script as shown below. Configure the host name of the master node to replicate (*nodeA*) to target_host and configure the home directory of the replication log (default value: $CUBRID_DATABASES) to repl_log_home. ::

    [nodeB]$ cubrid heartbeat stop
     
    [nodeB]$ cd $CUBRID/share/scripts/ha
    [nodeB]$ vi ha_make_slavedb.sh
    target_host=nodeA

After configuration, execute the **ha_make_slavedb.sh** script on the slave node. ::

    [nodeB]$ cd $CUBRID/share/scripts/ha
    [nodeB]$ ./ha_make_slavedb.sh

When any error occurs while executing the script in step-by-step order, or if the script should be restarted before being stopped by entering n, you can enter s for the steps which have been succeeded and go to the next step.

1. At this step, enter the password of a Linux account and password of **DBA**, the CUBRID database account, for HA rebuilding replication. Enter y to the question.
 
  ::

    ##### step 1 ###################################################################
    #
    # get HA/replica user password and DBA password
    #
    #  * warning !!!
    #   - Because ha_make_slavedb.sh use expect (ssh, scp) to control HA/replica node,
    #     the script has to know these passwords.
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y

  Enter the password of a Linux account of the HA node and the password of **DBA**, the CUBRID database account. 
  If you have not changed the password of **DBA** after installing CUBRID, press the <Enter> key without entering the password of **DBA**. 
  
  ::

        HA/replica cubrid_usr's password :
        HA/replica cubrid_usr's password :

        testdb's DBA password :
        Retype testdb's DBA password :

2. At this step, check whether the environment variables of the slave node are correct. Enter y to the question. 

  ::

    ##### step 2 ###################################################################
    #
    #  ha_make_slavedb.sh is the script for making slave database more easily
    #
    #  * environment
    #   - db_name           : testdb
    #
    #   - master_host       : nodeA
    #   - slave_host        : nodeB
    #   - replica_hosts     :
    #
    #   - current_host      : nodeB
    #   - current_state     : slave
    #
    #   - target_host       : nodeA
    #   - target_state      : master
    #
    #   - repl_log_home     : /home/cubrid_usr/CUBRID/databases
    #   - backup_dest_path  : /home/cubrid_usr/.ha/backup
    #   - backup_option     :
    #   - restore_option    :
    #
    #  * warning !!!
    #   - environment on slave must be same as master
    #   - database and replication log on slave will be deleted
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y

3. At this step, copy the HA-related scripts of the slave node to the master node. Enter y to the question. Then the password will be asked for when you access the master node in every step. In addition, the password will be asked for when you send a file by using the scp command. 

  ::

    ##### step 3 ###################################################################
    #
    #  copy scripts to master node
    #
    #  * details
    #   - scp scripts to '~/.ha' on nodeA(master).
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    [nodeB]$ tar -zcf ha.tgz ha
    [nodeA]$ rm -rf /home/cubrid_usr/.ha
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.
    [nodeB]$ scp -l 131072 -r ./../ha.tgz cubrid_usr@nodeA:/home1/cubrid_usr
    cubrid_usr@nodeA's password:
    ha.tgz                    100%   10KB  10.4KB/s   00:00
    [nodeA]$ tar -zxf ha.tgz
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.
    [nodeA]$ mv ha /home/cubrid_usr/.ha
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.
    [nodeA]$ mkdir /home/cubrid_usr/.ha/backup
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.

    To skip the password entry while executing the scp command, configure the secret key of the scp to the slave node and the public key to the master node, as shown below. 
    For more details, see How to Use ssh-keygen for Linux. 

    #. Execute **ssh-keygen -t rsa** to check that .ssh/id_rsa file and .ssh/id_rsa.pub file have been created under the home directory of the Linux user account.
    #. Copy the id_rsa.pub file as a file named authorized_keys under the home directory of the Linux user account in master node.
    #. Execute a test to check that the file is copied without asking for the password (scp test.txt cubrid_usr@:/home/cubrid_usr/).

4. At this step, copy the HA-related scripts to the replica node. In this scenario, if there is no replica node, skip this step and go to the next step by entering. 

  ::

    ##### step 4 #####################################
    #
    #  copy scripts to replication node
    #
    #  * details
    #   - scp scripts to '~/.ha' on replication node.
    #
    ##################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    There is no replication server to copy scripts.

5. At this step, check whether the environment variables of all nodes are correct. Enter y to the question. 

  ::

    ##### step 5 ###################################################################
    #
    #  check environment of all ha node
    #
    #  * details
    #   - test $CUBRID == /home1/cubrid_usr/CUBRID
    #   - test $CUBRID_DATABASES == /home1/cubrid_usr/CUBRID/database
    #   - test -d /home1/cubrid_usr/CUBRID/database/testdb
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y

6. At this step, stop replication of the master node. Enter y to the question. 

  ::

    ##### step 6 ###################################################################
    #
    #  suspend copylogdb/applylogdb on master if running
    #
    #  * details
    #   - deregister copylogdb/applylogdb on nodeA(master).
    #
    ################################################################################
    
        continue ? ([y]es / [n]o / [s]kip) : y
     
    [nodeA]$ sh /home/cubrid_usr/.ha/functions/ha_repl_suspend.sh -l /home/cubrid_usr/CUBRID/databases -d testdb -h nodeB -o /home/cubrid_usr/.ha/repl_utils.output
    cubrid_usr@nodeA's password:
    [nodeA]$ cubrid heartbeat deregister 9408
    suspend: (9408) cub_admin copylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB -m sync testdb@nodeB
    [nodeA]$ cubrid heartbeat deregister 9410
    suspend: (9410) cub_admin applylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB --max-mem-size=300 testdb@localhost
     
     
    3. heartbeat status on nodeA(master).
     
    [nodeA]$ cubrid heartbeat list
    @ cubrid heartbeat list
     
     HA-Node Info (current nodeA, state master)
       Node nodeB (priority 2, state unknown)
       Node nodeA (priority 1, state master)
     
     
     HA-Process Info (master 8362, state master)
       Copylogdb testdb@nodeB:/home/cubrid_usr/CUBRID/databases/testdb_nodeB (pid 9408, state deregistered)
       Server testdb (pid 9196, state registered_and_active)
     
    Connection to nodeA closed.
    Wait for 60s to deregister coppylogdb/applylogdb.
    ............................................................

7. At this step, delete the old replication log from the slave node and initialize the HA meta information table of the master node. Enter y to the question. 

  ::

    ##### step 7 ###################################################################
    #
    #  remove old copy log of slave and init db_ha_apply_info on master
    #
    #  * details
    #   - remove old copy log of slave
    #   - init db_ha_apply_info on master
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    - 1. remove old copy log.
     
    [nodeA]$ rm -rf /home/cubrid_usr/CUBRID/databases/testdb_nodeB/*
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.
     
    - 2. init db_ha_apply_info.
     
    [nodeA]$ csql -C -u dba  --sysadm testdb@localhost -c "delete from db_ha_apply_info where db_name='testdb'"
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.
    [nodeA]$ csql -C -u dba  --sysadm testdb@localhost -c "select * from db_ha_apply_info where db_name='testdb'"
    cubrid_usr@nodeA's password:
     
    === <Result of SELECT Command in Line 1> ===
     
    There are no results.
    Connection to nodeA closed.

8. At this step, initialize the HA meta information table of replica node. In this scenario, if there is no replica node, skip this step and go to the next step by entering s. 

  ::

    ##### step 8 ###################################################################
    #
    #  remove old copy log of slave and init db_ha_apply_info on replications
    #
    #  * details
    #   - remove old copy log of replica
    #   - init db_ha_apply_info on master
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    There is no replication server to init ha_info

9. At this step, create a backup volume from the master node (target_host) for HA replication rebuilding. You can skip this step and go to the next step by entering s if there is an existing backup volume. There are some constraints for rebuilding replication by using the existing backup volume, which are as follows:

    *   The archive log, including the transaction being executed during backup, must be in the master node (target_host); this means that a backup volume created long ago cannot be used.
    *   The backup status information file must be created by using the **-o** option during backup. 
        At this time, the path must be identical to the path of the backup volume file. 
        The file name must be the db_name.bkup.output format. If the file name is not identical with the format, change the file name according to the format before executing the script.
    *   The path of the existing backup volume and the status information file must be specified in the backup_dest_path parameter in the script.
        In other words, specify the absolute path of the directory containing the backup volume on the master node (target_host) to this parameter.

  ::

    ##### step 9 ###################################################################
    #
    #  online backup database  on master
    #
    #  * details
    #   - run 'cubrid backupdb -C -D ... -o ... testdb@localhost' on master
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    [nodeA]$ cubrid backupdb  -C -D /home/cubrid_usr/.ha/backup -o /home/cubrid_usr/.ha/backup/testdb.bkup.output testdb@localhost
    cubrid_usr@nodeA's password:
    Backup Volume Label: Level: 0, Unit: 0, Database testdb, Backup Time: Thu Apr 19 18:52:03 2012
    Connection to nodeA closed.
    [nodeA]$ cat /home/cubrid_usr/.ha/backup/testdb.bkup.output
    cubrid_usr@nodeA's password:
    [ Database(testdb) Full Backup start ]
     
    - num-threads: 2
     
    - compression method: NONE
     
    - backup start time: Thu Apr 19 18:52:03 2012
     
    - number of permanent volumes: 1
     
    - HA apply info: testdb 1334739766 715 8680
     
    - backup progress status
     
    -----------------------------------------------------------------------------
     volume name                  | # of pages | backup progress status    | done
    -----------------------------------------------------------------------------
     testdb_vinf                  |          1 | ######################### | done
     testdb                       |       6400 | ######################### | done
     testdb_lgar000               |       6400 | ######################### | done
     testdb_lgar001               |       6400 | ######################### | done
     testdb_lginf                 |          1 | ######################### | done
     testdb_lgat                  |       6400 | ######################### | done
    -----------------------------------------------------------------------------
     
    # backup end time: Thu Apr 19 18:52:06 2012
     
    [ Database(testdb) Full Backup end ]
    Connection to nodeA closed.

10. At this step, copy the database backup of the master node to the slave node. Enter y to the question. 

  ::

    ##### step 10 ###################################################################
    #
    #  copy testdb databases backup to current host
    #
    #  * details
    #   - scp databases.txt from target host if there's no testdb info on current host
    #   - remove old database and replication log if exist
    #   - make new database volume and replication path
    #   - scp  database backup to current host
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
     
     - 1. check if the databases information is already registered.
     
     
     - there is already testdb information in /home/cubrid_usr/CUBRID/databases/databases.txt
    [nodeB]$ grep testdb /home/cubrid_usr/CUBRID/databases/databases.txt
    testdb          /home/cubrid_usr/CUBRID/databases/testdb        nodeA:nodeB /home/cubrid_usr/CUBRID/databases/testdb/log file:/home/cubrid_usr/CUBRID/databases/testdb/lob
     
     - 2. get db_vol_path and db_log_path from databases.txt.
     
     
     - 3. remove old database and replication log.
     
    [nodeB]$ rm -rf /home/cubrid_usr/CUBRID/databases/testdb/log
    [nodeB]$ rm -rf /home/cubrid_usr/CUBRID/databases/testdb
    [nodeB]$ rm -rf /home/cubrid_usr/CUBRID/databases/testdb_*
     
     - 4. make new database volume and replication log directory.
     
    [nodeB]$ mkdir -p /home/cubrid_usr/CUBRID/databases/testdb
    [nodeB]$ mkdir -p /home/cubrid_usr/CUBRID/databases/testdb/log
    [nodeB]$ mkdir -p /home/cubrid_usr/.ha
    [nodeB]$ rm -rf /home/cubrid_usr/.ha/backup
    [nodeB]$ mkdir -p /home/cubrid_usr/.ha/backup
     
     - 5. copy backup volume and log from target host
     
    cubrid_usr@nodeA's password:
    testdb_bkvinf              100%   49     0.1KB/s   00:00
    cubrid_usr@nodeA's password:
    testdb_bk0v000             100% 1540MB   7.8MB/s   03:18
    testdb.bkup.output         100% 1023     1.0KB/s   00:00

11. At this step, restore the copied database backup to the slave node. Enter y to the question. 

  ::

    ##### step 11 ###################################################################
    #
    #  restore database testdb on current host
    #
    #  * details
    #   - cubrid restoredb -B ... testdb current host
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    [nodeB]$ cubrid restoredb -B /home/cubrid_usr/.ha/backup  testdb

12. At this step, configure the HA meta information table value of the slave node. Enter y to the question. 

  ::

    ##### step 12 ###################################################################
    #
    #  set db_ha_apply_info on slave
    #
    #  * details
    #   - insert db_ha_apply_info on slave
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
     
     
    1. get db_ha_apply_info from backup output(/home1/cubrid_usr/.ha/backup/testdb.bkup.output).
     
     - db_name       : testdb
     - db_creation   : 1349426614
     - pageid        : 86
     - offset        : 8800
     - log_path      : /home1/cubrid_usr/CUBRID/databases/testdb_nodeA
     
     
     
    2. select old db_ha_apply_info.
     
    [nodeA]$ csql -u DBA -S testdb -l -c "SELECT db_name, db_creation_time, copied_log_path, committed_lsa_pageid, committed_lsa_offset, committed_rep_pageid, committed_rep_offset, required_lsa_pageid, required_lsa_offset FROM db_ha_apply_info WHERE db_name='testdb'"
     
    === <Result of SELECT Command in Line 1> ===
     
    There are no results.
     
     
     
    3. insert new db_ha_apply_info on slave.
     
    [nodeB]$ csql --sysadm -u dba -S testdb -c "DELETE FROM db_ha_apply_info WHERE db_name='testdb'"
    [nodeB]$ csql --sysadm -u DBA -S testdb -c "INSERT INTO  db_ha_apply_info VALUES (       'testdb',       datetime '10/05/2012 17:43:34',         '/home1/cubrid_usr/DB/testdb_nodeA',         86, 8800,       86, 8800,       86, 8800,       86, 8800,       86, 8800,       86, 8800,       NULL,   NULL,   NULL,   0,      0,      0,      0,      0,      0,      0,      NULL )"
    [nodeB]$ csql -u DBA -S testdb -l -c "SELECT db_name, db_creation_time, copied_log_path, committed_lsa_pageid, committed_lsa_offset, committed_rep_pageid, committed_rep_offset, required_lsa_pageid, required_lsa_offset FROM db_ha_apply_info WHERE db_name='testdb'"
     
    === <Result of SELECT Command in Line 1> ===
     
    <00001> db_name             : 'testdb'
            db_creation_time    : 05:43:34.000 PM 10/05/2012
            copied_log_path     : '/home1/cubrid_usr/CUBRID/databases/testdb_nodeA'
            committed_lsa_pageid: 86
            committed_lsa_offset: 8800
            committed_rep_pageid: 86
            committed_rep_offset: 8800
            required_lsa_pageid : 86
            required_lsa_offset : 8800

13. At this step, initial the replication log of the master node and then copy the storage log of the master node to the slave node. Enter y to the question. 

  ::

    ##### step 13 ###################################################################
    #
    #  make initial replication active log on master, and copy archive logs from
    #  master
    #
    #  * details
    #   - remove old replication log on master if exist
    #   - start copylogdb to make replication active log
    #   - copy archive logs from master
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
     
     - 1. remove old replication log.
     
    [nodeB]$ rm -rf /home/cubrid_usr/CUBRID/databases/testdb_nodeA
    [nodeB]$ mkdir -p /home/cubrid_usr/CUBRID/databases/testdb_nodeA
     
     - 2. start copylogdb to initiate active log.
     
     
     - cubrid service stop
    [nodeB]$ cubrid service stop >/dev/null 2>&1
     
     - start cub_master
    [nodeB]$ cub_master >/dev/null 2>&1
     
     - start copylogdb and wait until replication active log header to be initialized
    [nodeB]$ cub_admin copylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeA -m 3 testdb@nodeA >/dev/null 2>&1 &
     
    ...
     
     - cubrid service stop
    [nodeB]$ cubrid service stop >/dev/null 2>&1
     
     - check copied active log header
    [nodeB]$  cubrid applyinfo -L /home/cubrid_usr/CUBRID/databases/testdb_nodeA testdb | grep -wqs "DB name"
     
     - 3. copy archive log from target.
     
    cubrid_usr@nodeA's password:
    testdb_lgar000             100%  512MB   3.9MB/s   02:11

14. At this step, restart the copylogdb process and the applylogdb process of the master node. Enter y to the question. 

  ::

    ##### step 14 ###################################################################
    #
    #  restart copylogdb/applylogdb on master
    #
    #  * details
    #   - restart copylogdb/applylogdb
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    [nodeA]$ sh /home/cubrid_usr/.ha/functions/ha_repl_resume.sh -i /home/cubrid_usr/.ha/repl_utils.output
    cubrid_usr@nodeA's password:
    [nodeA]$ cub_admin copylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB -m sync testdb@nodeB >/dev/null 2>&1 &
    resume: cub_admin copylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB -m sync testdb@nodeB
    [nodeA]$ cub_admin applylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB --max-mem-size=300 testdb@localhost >/dev/null 2>&1 &
    resume: cub_admin applylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB --max-mem-size=300 testdb@localhost
     
     - check heartbeat list on (master).
     
    [nodeA]$ cubrid heartbeat status
    @ cubrid heartbeat status
     
     HA-Node Info (current nodeA, state master)
       Node nodeB (priority 2, state unknown)
       Node nodeA (priority 1, state master)
     
     HA-Process Info (master 11847, state master)
       Server testdb (pid 11853, state registered_and_active)
     
     
    Connection to nodeA closed.

15. At this step, the result of building the slave node is printed to check whether it was successful or failed.

  ::

    ##### step 15 ##################################################################
    #
    #  completed
    #
    ################################################################################
    .

After the **ha_make_slavedb.sh** script has been stopped, check the HA status from the slave node and then run the HA. 

  ::

    [NodeB]$ cubrid heartbeat status
    @ cubrid heartbeat status
    ++ cubrid master is not running.
    [NodeB]$ cubrid heartbeat start
    @ cubrid heartbeat start
    @ cubrid master start
    ++ cubrid master start: success
     
    @ HA processes start
    @ cubrid server start: testdb
     
    This may take a long time depending on the amount of recovery works to do.
     
    CUBRID 9.0
     
    ++ cubrid server start: success
    @ copylogdb start
    ++ copylogdb start: success
    @ applylogdb start
    ++ applylogdb start: success
    ++ HA processes start: success
    ++ cubrid heartbeat start: success
    [nodeB ha]$ cubrid heartbeat status
    @ cubrid heartbeat status
     
     HA-Node Info (current nodeB, state slave)
       Node nodeB (priority 2, state slave)
       Node nodeA (priority 1, state master)
     
     HA-Process Info (master 26611, state slave)
       Applylogdb testdb@localhost:/home/cubrid_usr/CUBRID/databases/testdb_nodeA (pid 26831, state registered)
       Copylogdb testdb@nodeA:/home/cubrid_usr/CUBRID/databases/testdb_nodeA (pid 26829, state registered)
       Server testdb (pid 26617, state registered_and_standby)
