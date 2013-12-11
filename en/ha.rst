*********
CUBRID HA
*********

High Availability (HA) refers to a feature to provide uninterrupted service in the event of hardware, software, or network failure. This ability is a critical element in the network computing area where services should be provided 24/7. An HA system consists of more than two server systems, each of which provides uninterrupted services, even when a failure occurs in one of them.

CUBRID HA is an implementation of High Availability. CUBRID HA ensures database synchronization among multiple servers when providing service. When an unexpected failure occurs in the system which is operating services, this feature minimizes the service down time by allowing the other system to carry out the service automatically.

CUBRID HA is in a shared-nothing structure. To synchronize data from an active server to a standby server, CUBRID HA executes the following two steps.

#.  Transaction log multiplexing: Replicates the transaction logs created by an active server to another node in real time.
#.  Transaction log reflection: Analyzes replicated transaction logs in real time and reflects the data to a standby server.

CUBRID HA executes the steps described above in order to always maintain data synchronization between an active server and a standby server. For this reason, if an active server is not working properly because of a failure occurring in the master node that had been providing service, the standby server of the slave node provides service instead of the failed server. CUBRID HA monitors the status of the system and CUBRID in real time. It uses heartbeat messages to execute an automatic failover when a failure occurs.

.. image:: /images/image13.png

CUBRID HA Concept
=================

Nodes and Groups
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

*   **Replication log copy process (copylogdb)** : Copies all transaction logs in a group. When the replication log copy process requests a transaction log from the database server process of the target node, the database server process returns the corresponding log. The location of copied transaction logs can be configured in the **ha_copy_log_base** of **cubrid_ha.conf**. Use :ref:`cubrid-applyinfo` utility to verify the information of copied replication logs. The replication log copy process has following two modes: SYNC and ASYNC. You can configure it with the **ha_copy_sync_mode** of **cubrid_ha.conf**. For details on these modes, see :ref:`log-multiplexing`.

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

*   The following log information of cub_master process is saved on **$CUBRID/log/**\ `<hostname>` **_master.err** file. ::

        HA generic: Send changemode request to the server. (state:1[active], args:[cub_server demodb ], pid:25728).
        HA generic: Receive changemode response from the server. (state:1[active], args:[cub_server demodb ], pid:25728).

*   The following log information of cub_server is saved on **$CUBRID/log/server/**\ `<db_name>_<date>_<time>`\ **.err** file. ::

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

A broker can access a server with one of the following modes: **Read Write**, **Read Only** or **Slave Only**. This configuration value is determined by a user.

A broker finds and connects to a suitable server by trying to establish a connection in the order of server connections; this is, if it fails to establish a connection, it tries another connection to the next server defined until it reaches the last server. If no connection is made even after trying all servers, the broker fails to connect to a server.

For details on how to configure broker mode, see :ref:`ha-cubrid-broker-conf`.

**Read Write**

A broker that provides read and write services. This broker is usually connected to an active server. If no active servers exist, this broker will be connected to a standby server. For this reason, a Read Write broker can be temporarily connected to a standby server.

When the broker temporarily establishes a connection to a standby server, it will disconnect itself from the standby server at the end of every transaction so that it can attempt to find an active server at the beginning of the next transaction. When it is connected to the standby server, only read service is available. Any write requests will result in a server error.

Connecting to a DB server is influenced by **PREFERRED_HOSTS** and **CONNECT_ORDER** parameters in **cubrid_broker.conf**, and a CAS tries to connect in the below order.

#.  If **PREFERRED_HOSTS** is 

    a.  specified, a CAS tries to connect in the order in the specified hosts; if the status of the server is active, the connection is completed.
    b.  not specified; if there is a server which has been connected already, a CAS tries to connect with that server; if the status of the server is active, the connection is completed.

#.  After the failure of the connection in 1., if the value of **CONNECT_ORDER** is

    a.  specified as **SEQ** or not specified, a CAS tries to connect in the order to the hosts specified in **databases.txt**; if the status of the server is active, the connection is completed.
    b.  specified as **RANDOM**, a CAS tries to connect randomly to the hosts specified in **databases.txt**; if the status of the server is active, the connection is completed.
    
#.  After the failure of the connection in 2., if the value of **CONNECT_ORDER** is

    a.  specified as **SEQ** or not specified, a CAS tries to connect in the order to the hosts specified in **databases.txt**; if the accessible server exists, the connection is completed.
    b.  specified as **RANDOM**, a CAS tries to connect randomly to the hosts specified in **databases.txt**; if the accessible server exists, the connection is completed.

The following picture shows how a CAS connects to the host through **db-host** configuration when there are no settings about **PREFERRED_HOSTS** and **CONNECT_ORDER**.

.. image:: /images/image20.png

**Read Only**

A broker that provides the read service. This broker is connected to a standby server if possible. For this reason, the Read Only broker can be connected to an active server temporarily.

Once it establishes a connection with an active server, it will maintain that connection even if a standby server exists. To disconnect from the active server and reconnect to a standby server, you should execute the **cubrid broker reset** command. An error will occur when the Read Only broker receives write requests; therefore, only the read service will be available even if it is connected to an active server.

Connecting to a DB server is influenced by **PREFERRED_HOSTS** and **CONNECT_ORDER** parameters in **cubrid_broker.conf**, and a CAS tries to connect in the below order.

#.  If **PREFERRED_HOSTS** is 

    a.  specified, a CAS tries to connect in the order in the specified hosts; if the status of the server is standby, the connection is completed.
    b.  not specified; if there is a server which has been connected already, a CAS tries to connect with that server; if the status of the server is standby, the connection is completed.

#.  After the failure of the connection in 1., if the value of **CONNECT_ORDER** is

    a.  specified as **SEQ** or not specified, a CAS tries to connect in the order to the hosts specified in **databases.txt**; if the status of the server is standby, the connection is completed.
    b.  specified as **RANDOM**, a CAS tries to connect randomly to the hosts specified in **databases.txt**; if the status of the server is standby, the connection is completed.
    
#.  After the failure of the connection in 2., if the value of **CONNECT_ORDER** is

    a.  specified as **SEQ** or not specified, a CAS tries to connect in the order to the hosts specified in **databases.txt**; if the accessible server exists, the connection is completed.
    b.  specified as **RANDOM**, a CAS tries to connect randomly to the hosts specified in **databases.txt**; if the accessible server exists, the connection is completed.

The following picture shows how a CAS connects to the host through **db-host** configuration when there are no settings about **PREFERRED_HOSTS** and **CONNECT_ORDER**.

.. image:: /images/image21.png

**Slave Only**

A broker that provides the read service. This broker can only be connected to a standby server. If no standby server exists, no service will be provided.

Connecting to a DB server is influenced by **PREFERRED_HOSTS** and **CONNECT_ORDER** parameters in **cubrid_broker.conf**, and a CAS tries to connect in the below order.

#.  If **PREFERRED_HOSTS** is 

    a.  specified, a CAS tries to connect in the order in the specified hosts; if the status of the server is standby, the connection is completed.
    b.  not specified; if there is a server which has been connected already, a CAS tries to connect with that server; if the status of the server is standby, the connection is completed.

#.  After the failure of the connection in 1., if the value of **CONNECT_ORDER** is

    a.  specified as **SEQ** or not specified, a CAS tries to connect in the order to the hosts specified in **databases.txt**; if the status of the server is standby, the connection is completed.
    b.  specified as **RANDOM**, a CAS tries to connect randomly to the hosts specified in **databases.txt**; if the status of the server is standby, the connection is completed.
    
#.  If there is no standby status on the server, the connection is failed.

The following picture shows how a CAS connects to the host through the db-host configuration when there are no settings about **PREFERRED_HOSTS** and **CONNECT_ORDER**.

.. image:: /images/image22.png

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

To set a broker, configure the **cubrid_broker.conf** file. To set the order of failovers of a database server, configure the **databases.txt** file. For more information, see :ref:`quick-broker-config`.

The following is an example in which two Read Write (RW) brokers are configured. When the first connection broker of the application URL is set to *broker B1* and the second connection broker to *broker B2*, the application connects to *broker B2* when it cannot connect to *broker B1*. When broker B1 becomes available again, the application reconnects to *broker B1*.

.. image:: /images/image25.png

The following is an example in which the Read Write (RW) broker and the Read Only (RO) broker are configured in each piece of equipment of the master node and the slave node. First, the app1 and the app2 URL connect to *broker A1* (RW) and *broker B2* (RO), respectively. The second connection (altHosts) is made to *broker A2* (RO) and *broker B1* (RW). When equipment that includes *nodeA* fails, app1 and the app2 connect to the broker that includes *nodeB*.

.. image:: /images/image26.png

The following is an example of a configuration in which broker equipment includes one Read Write broker (master node) and two Preferred Host Read Only brokers (slave nodes). The Preferred Host Read Only brokers are connected to nodeB and nodeC to distribute the reading load.

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

The transaction log copy modes include **SYNC** and **ASYNC**. This value can be configured by the user in :ref:`cubrid-ha-conf` file.

**SYNC Mode**

When transactions are committed, the created transaction logs are copied to the slave node and stored as a file. The transaction commit is complete after receiving a notice on its success. Although the time it takes to execute commit in this mode may be longer than that in ASYNC modes, this is the safest method because the copied transaction logs are always guaranteed to be reflected to the standby server even if a failover occurs.

**ASYNC Mode**

When transactions are committed, commit is complete without verifying the transfer of transaction logs to a slave node. Therefore, it is not guaranteed that committed transactions are reflected to a slave node in a master node side.

Although **ASYNC** mode provides a better performance as it has almost no delay when executing commit, there may be data inconsistency in its nodes.

.. note::

    **SEMISYNC** mode which has existed from the previous versions operates in the same way as **SYNC** mode.

Quick Start
===========

[번역] DB 생성 시점부터 마스터 노드와 슬레이브 노드를 1:1로 구축하는 방법에 대해 간단히 설명한다. 기존 운영 중인 DB 서버에서 슬레이브를 새로 구축하려면 :ref:`build-new-slave`\ 를 참고한다.

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

    This document describes the HA configuration in CUBRID 9.2 or later versions. Note that the previous versions have different settings. For example, **cubrid_ha.conf** is only available in CUBRID 2008 R4.0 or later. **ha_make_slavedb.sh** is introduced from CUBRID 2008 R4.1 Patch 2 or later.

.. _quick-server-config:

Creating Databases and Configuring Servers
------------------------------------------

**Creating Databases**

Create databases to be included in CUBRID HA at each node of the CUBRID HA in the same manner. Modify the options for database creation as needed. ::

    [nodeA]$ cd $CUBRID_DATABASES
    [nodeA]$ mkdir testdb
    [nodeA]$ cd testdb
    [nodeA]$ mkdir log
    [nodeA]$ cubrid createdb -L ./log testdb en_US
    Creating database with 512.0M size. The total amount of disk space needed is 1.5G.
     
    CUBRID 10.0
     
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

*   Master node ::

        [nodeA]$ cubrid changemode testdb@localhost
        The server 'testdb@localhost''s current HA running mode is active.

*   Slave node ::

        [nodeB]$ cubrid changemode testdb@localhost
        The server 'testdb@localhost''s current HA running mode is standby.

**Verifying the CUBRID HA Operation**

Verify that action is properly applied to standby server of the slave node after performing write in an active server of the master node. To make a success connection via the CSQL Interpreter in HA environment, you must specify the host name to be connected after the database name like "@<*host_name*>"). If you specify a host name as localhost, it is connected to local node.

.. warning:: Ensure that primary key must exist when creating a table to have replication successfully processed.

*   Master node ::

        [nodeA]$ csql -u dba testdb@localhost -c "create table abc(a int, b int, c int, primary key(a));"
        [nodeA]$ csql -u dba testdb@localhost -c "insert into abc values (1,1,1);"
        [nodeA]$

*   Slave node ::

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

*   databases.txt ::

        #db-name        vol-path                db-host         log-path        lob-base-path
        testdb          /home1/cubrid1/CUBRID/testdb  nodeA:nodeB        /home1/cubrid1/CUBRID/testdb/log file:/home1/cubrid1/CUBRID/testdb/lob

*   cubrid_broker.conf ::

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

*   **off** : CUBRID HA is not used.
*   **on** : CUBRID HA is used. Failover is supported for its node.
*   **replica** : CUBRID HA is used. Failover is not supported for its node.

The **ha_mode** parameter can be re-configured in the **[@<database>]** section; however, only **off** can be entered in the case. An error is returned if a value other than **off** is entered in the **[@<database>]** section.

If **ha_mode** is **on**, the CUBRID HA values are configured by reading **cubrid_ha.conf**.

This parameter cannot be modified dynamically. To modify the value of this parameter, you must restart it.

.. _ha-log_max_archives:

**log_max_archives**

**log_max_archives** is a parameter used to configure the minimum number of archive log files to be archived. The minimum value is 0 and the default is **INT_MAX** (2147483647). When CUBRID has installed for the first time, this value is set to 0 in the **cubrid.conf** file. The behavior of the parameter is affected by **force_remove_log_archives**.

If the value of **force_remove_log_archives** is set to **no**, the existing archive log files to which the activated transaction refers or the archive log files of the master node not reflected to the slave node in HA environment will not be deleted. For details, see the following **force_remove_log_archives**. 

For details about **log_max_archives**, see :ref:`logging-parameters`.

.. _ha-force_remove_log_archives:

**force_remove_log_archives**

It is recommended to configure **force_remove_log_archives** to **no** so that archive logs to be used by HA-related processes always can be maintained to set up HA environment by configuring **ha_mode** to **on**.

If you configure the value for **force_remove_log_archives** to yes, the archive log files which will be used in the HA-related process can be deleted, and this may lead to an inconsistency between replicated databases. If you want to maintain free disk space even though doing this could lead to risk, you can configure the value to yes. 

For details about **force_remove_log_archives**, see :ref:`logging-parameters`.

.. note::

    From 2008 R4.3 in replica mode, it will be always deleted except for archive logs as many as specified in the **log_max_archives** parameter, regardless the **force_remove_log_archives** value specified.

**max_clients**

**max_clients** is a parameter used to configure the maximum number of clients to be connected to a database server simultaneously. The default is **100**.

Because the replication log copy and the replication log reflection processes start by default if CUBRID HA is used, you must configure the value to twice the number of all nodes in the CUBRID HA group, except the corresponding node. Furthermore, you must consider the case in which a client that is connected to another node at the time of failover attempts to connect to that node. 

For details about **max_clients**, see :ref:`connection-parameters`.

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

**ha_node_list** is a parameter used to configure the group name to be used in the CUBRID HA group and the host name of member nodes in which failover is supported. The group name is separated by @. The name before @ is for the group, and the names after @ are for host names of member nodes. A comma(,) or colon(:) is used to separate individual host names. The default is **localhost@localhost**.

.. note::

    The host name of the member nodes specified in this parameter cannot be replaced with the IP. You should use the host names which are registered in **/etc/hosts**. 

    If the host name is not specified properly, the below message is written into the server.err error log file.
    
    ::
    
        Time: 04/10/12 17:49:45.030 - ERROR *** file ../../src/connection/tcp.c, line 121 ERROR CODE = -353 Tran = 0, CLIENT = (unknown):(unknown)(-1), EID = 1 Cannot make connection to master server on host "Wrong_HOST_NAME".... Connection timed out

A node in which the **ha_mode** value is set to **on** must be specified in **ha_node_list**. The value of the **ha_node_list** of all nodes in the CUBRID HA group must be identical. When a failover occurs, a node becomes a master node in the order specified in the parameter.

This parameter can be modified dynamically. If you modify the value of this parameter, you must execute :ref:`cubrid heartbeat reload <cubrid-heartbeat>` to apply the changes.

**ha_replica_list**

**ha_replica_list** is parameter used to configure the group name, which is used in the CUBRID HA group, and the replica nodes, that is host name of member nodes in which failover is not supported. There is no need to specify if you do not construct replica nodes. The group name is separated by @. The name before @ is for the group, and the names after @ are for host names of member nodes. A comma(,) or colon(:) is used to separate individual host names. The default is **NULL**.

The group name must be identical to the name specified in **ha_replica_list**. The host names of member nodes and the host names of nodes specified in this parameter must be registered in **/etc/hosts**. A node in which the **ha_mode** value is set to **replica** must be specified in **ha_replica_list**. The **ha_replica_list** values of all nodes in the CUBRID HA group must be identical.

This parameter can be modified dynamically. If you modify the value of this parameter, you must execute :ref:`cubrid heartbeat reload <cubrid-heartbeat>` to apply the changes.

**ha_port_id**

**ha_port_id** is a parameter used to configure the UDP port number; the UDP port is used to detect failure when exchanging heartbeat messages. The default is **59,901**.

If a firewall exists in the service environment, the firewall must be configured to allow the configured port to pass through it.

**ha_ping_hosts**

**ha_ping_hosts** is a parameter used to configure the host which verifies whether or not a failover occurs due to unstable network when a failover has started in a slave node. The default is **NULL**. A comma(,) or colon(:) is used to separate individual host names.

The host name of the member nodes specified in this parameter can be replaced with the IP. When a host name is used, the name must be registered in **/etc/hosts**.

Configuring this parameter can prevent split-brain, a phenomenon in which two master nodes simultaneously exist as a result of the slave node erroneously detecting an abnormal termination of the master node due to unstable network status and then promoting itself as the new master.

**ha_copy_sync_mode**

**ha_copy_sync_mode** is a parameter used to configure the mode of storing the replication log, which is a copy of transaction log. The default is **SYNC**.

The value can be one of the following: **SYNC** and **ASYNC**. The number of values must be the same as the number of nodes specified in **ha_node_list**. They must be ordered by the specified value. You can specify multiple modes by using a comma(,) or colon(:). The replica node is always working in **ASNYC** mode regardless of this value.

For details, see :ref:`log-multiplexing`.

**ha_copy_log_base**

**ha_copy_log_base** is a parameter used to configure the location of storing the transaction log copy. The default is **$CUBRID_DATABASES**.

For details, see :ref:`log-multiplexing`.

.. _ha_copy_log_max_archives:

**ha_copy_log_max_archives**

**ha_copy_log_max_archives** is a parameter used to configure the maximum number of keeping replication log files. The default is 1. [번역]  현재 데이터베이스에 반영되지 않은 복제 로그 파일은 삭제되지 않는다. 

불필요한 디스크의 공간 낭비를 방지하기 위해 이 값을 기본값인 1로 유지할 것을 권장한다. 

.. 아래 내용은 복제 재구축 스크립트의 동작 방식이 변경되기 전까지는 틀린 내용이므로 제거. 물론 수동으로 구축한다면 복제로그를 사용할 수는 있음.

    If :ref:`rebuilding-replication` is needed by using the slave node or the replica node as a source, a newly added replication log files during rebuilding a replication should not be deleted by specifying **ha_copy_log_max_archives**. Therefore, the value of **ha_copy_log_max_archives** should be specified moderately largely. Except for this case, we recommend you to keep this value as the default value, 1, to prevent wasting disk space

**ha_db_list**

**ha_db_list** is a parameter used to configure the name of the database that will run in CUBRID HA mode. The default is **NULL**. You can specify multiple databases by using a comma (,).

**ha_apply_max_mem_size**

**ha_apply_max_mem_size** is a parameter used to configure the value of maximum memory that the replication log reflection process of CUBRID HA can use. The default and maximum values are **500** (unit: MB). When the value is larger than the size allowed by the system, memory allocation fails and the HA replication reflection process may malfunction. For this reason, you must check whether or not the memory resource can handle the specified value before setting it.

**ha_applylogdb_ignore_error_list**

**ha_applylogdb_ignore_error_list** is a parameter used to configure for proceeding replication in CUBRID HA process by ignoring an error occurrence. The error codes to be ignored are separated by a comma (,). This value has a high priority. Therefore, when this value is the same as the value of the **ha_applylogdb_retry_error_list** parameter or the error code of "List of Retry Errors," the values of the **ha_applylogdb_retry_error_list** parameter or the error code of "List of Retry Errors" are ignored and the tasks that cause the error are not retried. For "List of Retry Errors," see the description of **ha_applylogdb_retry_error_list** below.

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

**ha_replica_delay**

This parameter specifies the term of applying the replicated data between a master node and a replica node. You can set a unit as ms, s, min or h, which stands for milliseconds, seconds, minutes or hours respectively. If you omit the unit, milliseconds(ms) will be applied. The default value is 0.

**ha_replica_time_bound**

In master node, only the transactions which have been run on the specified time with this parameter are applied to the replica node. The format of this value is "YYYY-MM-DD hh:mi:ss". There is no default value.
    
.. note:: \

    The following example shows how to configure **cubrid_ha.conf**. ::

        [common]
        ha_node_list=cubrid@nodeA:nodeB
        ha_db_list=testdb
        ha_copy_sync_mode=sync:sync
        ha_apply_max_mem_size=500

.. note:: \

    The following example shows how to configure the value of /etc/hosts (a host name of a member node: nodeA, IP: 192.168.0.1). ::

        127.0.0.1 localhost.localdomain localhost
        192.168.0.1 nodeA

**ha_delay_limit** 
  
**ha_delay_limit**\은 DB가 스스로 복제 지연 상태임을 판단하는 기준 시간이고 **ha_delay_limit_delta**\는 복제 지연 시간에서 복제 지연 해제 시간을 뺀 값이다. 한번 복제 지연이라고 판단된 서버는 복제 지연 시간이 (**ha_delay_limit** - **ha_delay_limit_delta**) 이하로 낮아질 경우에 복제 지연이 해소되었다고 판단한다. 
슬레이브 노드 또는 레플리카 노드가 standby DB 서버에 해당한다. 
  
예를 들어 복제 지연 시간을 10분으로 설정하고 복제 지연 해제는 8분으로 하고 싶다면, **ha_delay_limit**\의 값은 600S(또는 10M), **ha_delay_limit_delta**\의 값은 120S(또는 2M)이다. 
  
복제 지연으로 판단되면 CAS는 현재 접속 중인 standby DB가 작업 처리에 문제가 있다고 판단하고, 다른 standby DB로 재접속을 시도한다. 

복제 지연으로 인해 우선 순위가 낮은 DB에서 연결된 CAS는 **cubrid_broker.conf**\의 :ref:`RECONNECT_TIME <reconnect_time>` 파라미터로 명시한 시간이 경과하면 복제 지연이 해소되었을 것으로 기대하여, 우선 순위가 높은 standby DB 서버에 재접속을 시도한다. 

**ha_delay_limit_delta** 
  
위의 **ha_delay_limit** 설명을 참고한다. 

.. _ha-cubrid-broker-conf:

cubrid_broker.conf
------------------

The **cubrid_broker.conf** file that has general information on configuring CUBRID broker is located in the **$CUBRID/conf** directory. This section explains the parameters of **cubrid_broker.conf** that are used by CUBRID HA.

**ACCESS_MODE**

**ACCESS_MODE** is a parameter used to configure the mode of a broker. The default is **RW**.

Its value can be one of the following: **RW** (Read Write), **RO** (Read Only), **SO** (Slave Only), or **PHRO** (Preferred Host Read Only). For details, see :ref:`broker-mode`.

**PREFERRED_HOSTS**

Specify the order to connect by listing host names. The default value is **NULL**.

You can specify multiple nodes by using a colon (:). First, it tries to connect to host in the following order: host specified in the **PREFERRED_HOSTS** parameter first and then host specified in **$CUBRID_DATABASES/databases.txt**. For details, see :ref:`broker-mode`.

The following example shows how to configure **cubrid_broker.conf**. To access localhost in a first priority, set **PREFERRED_HOSTS** as *localhost*.

::

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
    ACCESS_MODE             =RO
    PREFERRED_HOSTS         =localhost

databases.txt
-------------

The **databases.txt** file has information on the order of servers for the CAS of a broker to connect. It is located in the **$CUBRID_DATABASES** (if not specified, $CUBRID/databases) directory; the information can be configured by using **db_hosts**. You can specify multiple nodes by using a colon (:). If "**CONNECT_ORDER**=**RANDOM**", the connection order is decided as randomly. But if **PREFERRED_HOSTS** is specified, the specified hosts have the first priority of the connection order.

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
    
    If this value is **true**, an application program releases the existing connection from a broker and reconnects to the other broker which is specified on **altHosts**.

Running and Monitoring
======================

.. _cubrid-heartbeat:

cubrid heartbeat Utility
------------------------

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

If you want to deactivate CUBRID HA feature immediately, add -i option into the "cubrid heartbeat stop" command. This option is used when the speedy quitting is required because the DB server process is working improperly.

::
 
    $ cubrid heartbeat stop -i
    or
    $cubrid heartbeat stop --immediately
    
**copylogdb**

This utility is used to start or stop the **copylogdb** process that copies the transaction logs for the *db_name* of a specific peer_node in the CUBRID HA configuration. You can pause log copy for rebuilding replications in the middle of operation and then rerun it whenever you want.

Even though only the **cubrid heartbeat copylogdb start** command has succeeded, the functions of detecting and recovering the failure between the nodes are executed. Since the node is the target of failover, the slave node can be changed to the master node.

How to use this utility is as shown below. ::

    $ cubrid heartbeat copylogdb <start|stop> db_name peer_node

[번역] 명령을 수행하는 노드가 *nodeB*\ 이고, peer_node가 *nodeA*\ 라면, 다음과 같이 명령을 수행할 수 있다.
    
::
    
    [nodeB]$ cubrid heartbeat copylogdb stop testdb nodeA
    [nodeB]$ cubrid heartbeat copylogdb start testdb nodeA
    
When the **copylogdb** process is started/stopped, the configuration information of the **cubrid_ha.conf** is used. We recommend that you do not change the configuration as possible after you have set the configuration once. If you need to change it, it is recommended to restart the whole nodes.

**applylogdb**

This utility is used to start or stop the **copylogdb** process that reflects the transaction logs for the *db_name* of a specific peer_node in the CUBRID HA configuration. You can pause log copy for rebuilding replications in the middle of operation and then rerun it whenever you want.

Even though only the **cubrid heartbeat copylogdb start** command has succeeded, the functions of detecting and recovering the failure between the nodes are executed. Since the node is the target of failover, the slave node can be changed to the master node.

How to use this utility is as shown below. ::

    $ cubrid heartbeat applylogdb <start|stop> db_name peer_node

[번역] 명령을 수행하는 노드가 *nodeB*\ 이고, peer_node가 *nodeA*\ 라면, 다음과 같이 명령을 수행할 수 있다.
    
::
    
    [nodeB]$ cubrid heartbeat applylogdb stop testdb nodeA
    [nodeB]$ cubrid heartbeat applylogdb start testdb nodeA

When the **applylogdb** process is started/stopped, the configuration information of the **cubrid_ha.conf** is used. We recommend that you do not change the configuration as possible after you have set the configuration once. If you need to change it, it is recommended to restart the whole nodes.

**reload**

This utility is used to retrieve the CUBRID HA information again, and it starts or stops the CUBRID HA components according to new CUBRID HA configuration. Used to add or delete a node; it starts the HA processes which correspond to the added nodes after modification or it stops the HA processes which correspond to the deleted nodes.

How to use this utility is as shown below. ::

    $ cubrid heartbeat reload

Reconfigurable parameters are **ha_node_list** and **ha_replica_list**. Even if an error occurs on a special node during running this command, the left jobs are continued. After **reload** command is finished, check if the reconfiguration of nodes is applied well or not. If it fails, find the reason and resolve it.

**status**

This utility is used to output the information of CUBRID HA group and CUBRID HA components. How to use this utility is as shown below. ::

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

Registering HA to cubrid service
--------------------------------

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

    Outputs the information of replication reflection of a node executing cubrid applyinfo. The **-L** option is required to use this option.
    
.. option:: -L, --copied-log-path=PATH

    Configures the location of transaction logs copied from the other node. Using this option will output the information of transaction logs copied (Copied Active Info.) from the other node.
    
.. option:: -p, --pageid=ID

    Outputs the information of a specific page in the copied logs. This is available only when the  **-L** option is enabled.  The default is 0, it means the active page. 
        
.. option:: -v

    Outputs detailed information.                        

.. option:: -i, --interval=SECOND

    Outputs the copied status and applied status of transaction logs per specified seconds. To see the delayed status of the replicated log, this option is mandatory.
    
**Example**

The following example shows how to check log information (Active Info.) of the master node, the status information of log copy (Copied Active Info.) of the slave node, and the applylogdb info (Applied Info.) of the slave node by executing **applyinfo** in the slave node.

*   Applied Info.: Shows the status information after the slave node applies the replication log.
*   Copied Active Info.: Shows the status information after the slave node copies the replication log.
*   Active Info.: Shows the status information after the master node records the transaction log.
*   Delay in Copying Active Log: Shows the status information which the transaction logs' copy is delayed.
*   Delay in Applying Copied Log: Shows the status information which the transaction logs' application is delayed.

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

     *** Active Info. *** 
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

*   Applied Info.
    
    *   Committed page: The information of committed pageid and offset of a transaction reflected last through replication log reflection process. The difference between this value and the EOF LSA of "Copied Active Info. represents the amount of replication delay.
    *   Insert Count: The number of Insert queries reflected through replication log reflection process.
    *   Update Count: The number of Update queries reflected through replication log reflection process.
    *   Delete Count: The number of Delete queries reflected through replication log reflection process.
    *   Schema Count: The number of DDL statements reflected through replication log reflection process.
    *   Commit Count: The number of transactions reflected through replication log reflection process.
    *   Fail Count: The number of DML and DDL statements in which log reflection through replication log reflection process fails.
    
*   Copied Active Info.
    
    *   DB name: Name of a target database in which the replication log copy process copies logs
    *   DB creation time: The creation time of a database copied through replication log copy process
    
    *   EOF LSA: Information of pageid and offset copied at the last time on the target node by the replication log copy process. There will be a delay in copying logs as much as difference with the EOF LSA value of "Active Info." and with the Append LSA value of "Copied Active Info."
    
    *   Append LSA: Information of pageid and offset written at the last time on the disk by the replication log copy process. This value can be less than or equal to EOF LSA. There will be a delay in copying logs as much as difference between the EOF LSA value of "Copied Active Info." and this value.
    
    *   HA server state: Status of a database server process which replication log copy process receives logs from. For details on status, see :ref:`ha-server`.
    
*   Active Info.
    
    *   DB name: Name of a database whose node was configured in the **-r** option.
    *   DB creation time: Database creation time of a node that is configured in the **-r** option.
    *   EOF LSA: The last information of pageid and offset of a database transaction log of a node that is configured in the **-r** option. There will be a delay in copying logs as much as difference between the EOF LSA value of "Copied Active Info." and this value.
    
    *   Append LSA: Information of pageid and offset written at the last time on the disk by the database whose node was configured in the **-r** option.
    
    *   HA server state: The server status of a database server whose node was configured in the **-r** option.
    
*   Delay in Copying Active Log
    
    *   Delayed log page count: the count of transaction log pages which the copy is delayed.
    *   Estimated Delay: the expected time which the logs copying is completed.
    
*   Delay in Applying Copied Log

    *   Delayed log page count: the count of transaction log pages which the application is delayed.
    *   Estimated Delay: the expected time which the logs applying is completed.

When you run this command in replica node, if "ha_replica_delay=30s" is specified in cubrid.conf, the following information is printed out additionally.

::

     *** Replica-specific Info. ***
    Deliberate lag                 : 30 second(s)
    Last applied log record time   : 2013-06-20 11:20:10

Each item of the status information is as below.

*   Replica-specific Info.

    *   Deliberate lag: delayed time a user defined by ha_replica_delay parameter
    *   Last applied log record time: the time where the replication log of being applied in the replica node recently was actually applied in the master node.
    
When you run this command in replica node, if "ha_replica_delay=30s" and "ha_replica_time_bound=2013-06-20 11:31:00" are specified in cubrid.conf, "ha_replica_delay=30s" is ignored and the following information is printed out additionally.

::

     *** Replica-specific Info. ***
    Last applied log record time   : 2013-06-20 11:25:17
    Will apply log records up to   : 2013-06-20 11:31:00

Each item of the status information is as below.

*   Replica-specific Info.

    *   Last applied log record time: the time where the replication log of being applied in the replica node recently was actually applied in the master node.
    *   Will apply log records up to: the replica node will apply the master node's logs replicated up to this time.

When applylogdb stops the replication after the time of being specified by **ha_replica_time_bound**,  the error message which is printed out in the file, **$CUBRID/log/**\ *db-name*\ **@**\ *local-node-name*\ **_applylogdb_**\ *db-name*\ **_**\ *remote-node-name*\ **.err** is as below.

::

    Time: 06/20/13 11:51:05.549 - ERROR *** file ../../src/transaction/log_applier.c, line 7913 ERROR CODE = -1040 Tran = 1, EID = 3
    HA generic: applylogdb paused since it reached a log record committed on master at 2013-06-20 11:31:00 or later.
    Adjust or remove ha_replica_time_bound and restart applylogdb to resume.
    
.. _cubrid-changemode:

cubrid changemode
-----------------

This utility is used to check and change the server status of CUBRID HA. ::

    cubrid changemode [options] <database-name@node-name>

*   *database-name@node-name* : Specifies the name of a server to be checked or changed and separates each node name by using @.

The following shows [options] used in **cubrid changemode**.

.. program:: changemode

.. option:: -m, --mode=MODE

    Changes the server status. You can enter one of the following:                                                                                                       
    
    **standby**, **maintenance** or **active**.
    
.. option:: -f, --force

    Configures whether or not to forcibly change the server status. This option must be configured if you want to change the server status from to-be-active to active.   |
    
    If it is not configured, the status will not be changed to active. 
    Forcibly change may cause data inconsistency among replication nodes; so it is not recommended.                                                                       |

.. option:: -t, --timeout=SECOND
    
    The default is 5(seconds). Configures the waiting time for the normal completion of the transaction that is being processed when the node status switches from **standby** to **maintenance**. 
    
    If the transaction is still in progress beyond the configured time, it will be forced to terminate and switch to **maintenance** status; if all transactions have completed normally within the configured time, it will switch to **maintenance** status immediately. 

**Status Changeable**

This table shows changeable modes depending on current status.

+------------------------------------+-----------------------------------------------+
|                                    | **Changeable**                                |
|                                    +---------------+---------------+---------------+
|                                    | active        | standby       | maintenance   |
+--------------------+---------------+---------------+---------------+---------------+
| **Current Status** | standby       | X             | O             | O             |
|                    +---------------+---------------+---------------+---------------+
|                    | to-be-standby | X             | X             | X             |
|                    +---------------+---------------+---------------+---------------+
|                    | active        | O             | X             | X             |
|                    +---------------+---------------+---------------+---------------+
|                    | to-be-active  | O*            | X             | X             |
|                    +---------------+---------------+---------------+---------------+
|                    | maintenance   | X             | O             | O             |
+--------------------+---------------+---------------+---------------+---------------+

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

Structures of HA
================

There are four possible structures for CUBRID HA: The default structure, multiple-slave node structure, load balancing structure, and multiple-standby server structure. In the table below, M stands for a master node, S for a slave node, and R for a replica node.

+-----------------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Structure                         | Node structure (M:S:R) | Characteristic                                                                                                                        |
+===================================+========================+=======================================================================================================================================+
| Default Structure                 | 1:1:0                  | The most basic structure of CUBRID HA consists of one master node and one slave node and provides availability                        |
|                                   |                        | which is a unique feature of CUBRID HA.                                                                                               |
+-----------------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Multiple-Slave Node Structure     | 1:N:0                  | This is a structure in which availability is increased by several slave nodes. However,                                               |
|                                   |                        | note that there may be a situation in which data is inconsistent in the CUBRID HA group when multiple failures occur.                 |
+-----------------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Load Balancing Structure          | 1:1:N                  | Several replica nodes are added in the basic structure. Read service load can be distributed, and the HA load is reduced,             |
|                                   |                        | comparing to a multiple-slave node structure. Note that replica nodes do not failover.                                                |
+-----------------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------+
| Multiple-Standby Server Structure | 1:1:0                  | Basically, this structure is the same as the basic structure. However, several slave nodes are installed on a single physical server. |
+-----------------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------+

[번역] 이하에서 설명할 각 구성의 설정 전에, 각 노드에는 데이터가 없는 상태의 testdb 데이터베이스를 생성했다고 가정한다. 이미 데이터가 존재하는 데이터베이스를 복제하여 HA를 구성하기 위해서는 :ref:`rebuilding-replication`\ 를 참고한다.

Basic Structure of HA
---------------------

The most basic structure of CUBRID HA consists of one master node and one slave node.

The default configuration is one master node and one slave node. To distribute the write load, a multi-slave node or load-distributed configuration is recommended. In addition, to access a specific node such as a slave node or replica node in read-only mode, configure the Read Only broker or the Preferred Host Read Only broker. For details about broker configuration, see :ref:`duplexing-brokers`.

**An Example of Node Configuration**

.. image:: /images/image30.png

You can configure each node in the basic structure of HA as shown below:

*   **node A** (master node)

    *   Configure the **ha_mode** of the **cubrid.conf** file to **on**. ::

            ha_mode=on

    *   The following example shows how to configure **cubrid_ha.conf**: ::

            ha_port_id=59901
            ha_node_list=cubrid@nodeA:nodeB
            ha_db_list=testdb

*   **node B** (slave node): Configure this node in the same manner as *node A*.

For the **databases.txt** file of a broker node, it is necessary to configure the list of hosts configured as HA in **db-host** according to their priority. The following example shows the **databases.txt** file. ::

    #db-name    vol-path                  db-host       log-path                   lob-base-path
    testdb     /home/cubrid/DB/testdb     nodeA:nodeB   /home/cubrid/DB/testdb/log file:/home/cubrid/DB/testdb/lob

The **cubrid_broker.conf** file can be set in a variety of ways according to configuration of the broker. It can also be configured as separate equipment with the **databases.txt** file.

The example below shows that the RW broker is set in each node, and *node A* and *node B* have the same value. ::

    [%RW_broker]
    ...
     
    # Broker mode setting parameter
        ACCESS_MODE             =RW

**Connection Configuration of Applications**

See :ref:`ha-jdbc-conf`, :ref:`ha-cci-conf`, and :ref:`ha-php-conf` in Environment Configuration.

**Remark**

The moving path of a transaction log in these configurations is as follows:

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

    *   Configure the **ha_mode** of the **cubrid.conf** file to **on**. ::

            ha_mode=on

    *   The following example shows how to configure **cubrid_ha.conf**: ::

            ha_port_id=59901
            ha_node_list=cubrid@nodeA:nodeB:nodeC
            ha_db_list=testdb

*   **node B**, **node C** (slave node): Configure this node in the same manner as *node A*.

You must enter the list of hosts configured in HA in order of priority in the **databases.txt** file of a broker node. The following is an example of the **databases.txt** file. ::

    #db-name   vol-path                   db-host             log-path                   lob-base-path
    testdb     /home/cubrid/DB/testdb     nodeA:nodeB:nodeC   /home/cubrid/DB/testdb/log file:/home/cubrid/DB/testdb/lob

The **cubrid_broker.conf** file can be set in a variety of ways according to configuration of the broker. It can also be configured as separate equipment with the **databases.txt** file. In this example, the RW broker is configured in *node A*, *node B*, and *node C*.

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

.. note::

    The data in the CUBRID HA group may lose integrity when there are multiple failures in this structure and the example is shown below.

    *   n a situation where a failover occurs in the first slave node while replication in the second slave node is being delayed due to restart
    *   In a situation where a failover re-occurs before replication reflection of a new master node is not complete due to frequent failover

    In addition, if the mode of replication log copy process is ASYNC, the data in the CUBRID HA group may lose integrity.

    If the data in the CUBRID HA group loses integrity for any of the reasons above, you can fix it by using :ref:`rebuilding-replication`.

**Remark**

The moving path of a transaction log in these configurations is as follows:

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

    *   Configure the **ha_mode** of the **cubrid.conf** file to **on**. ::

            ha_mode=on

    *   The following example shows how to configure **cubrid_ha.conf**: ::

            ha_port_id=59901
            ha_node_list=cubrid@nodeA:nodeB 
            ha_replica_list=cubrid@nodeC:nodeD:nodeE
            ha_db_list=testdb

*   **node B** (slave node): Configure this node in the same manner as *node A*.

*   **node C**, **node D**, **node E** (replica node)

    *   Configure the **ha_mode** of the **cubrid.conf** file to **replica**. ::

            ha_mode=replica

    *   You can configure the **cubrid_ha.conf** file in the same manner as *node A*.

The **cubrid_broker.conf** can be set in a variety of ways according to configuration of the broker. It can also be configured as separate equipment with the **databases.txt** file.

In this example, broker and DB server exist on the same machine; the RW broker is configured in *node A* and *node B*; the SO broker with "CONNECT_ORDER=RANDOM" and "PREFERRED_HOSTS=localhost" is configured in *node C*, *node D* and *node E*. each of *node C*, *node D* or *node E* tries to connect to local DB server first because they are set as "PREFERRED_HOSTS=localhost". When it is failed to connect to localhost, it tries to connect to one of **db-hosts** in **databases.txt** randomly because they are set as "CONNECT_ORDER=RANDOM".

The following is an example of **cubrid_broker.conf** in *node A* and *node B*.

::

    [%RW_broker]
    ...
     
    # Broker mode setting parameter
    ACCESS_MODE             =RW

The following is an example **cubrid_broker.conf** in *node C*, *node D* and *node E*. ::

    [%PHRO_broker]
    ...
     
    # Broker mode setting parameter
    ACCESS_MODE             =SO
    PREFERRED_HOSTS         =localhost

You must enter the list of DB server hosts in the order so that each broker can be connected appropriate HA or load balancing server in the **databases.txt** file of a broker node.

The following is an example of the **databases.txt** file in *node A* and *node B*. ::

    #db-name    vol-path                  db-host       log-path             lob-base-path
    testdb     /home/cubrid/DB/testdb   nodeA:nodeB   /home/cubrid/DB/testdb/log file:/home/cubrid/CUBRID/testdb/lob

The following is an example of the **databases.txt** file in *node C*, *node D* and *node E*. ::

    #db-name    vol-path                db-host             log-path                    lob-base-path
    testdb     /home/cubrid/DB/testdb   nodeC:nodeD:nodeE   /home/cubrid/DB/testdb/log  file:/home/cubrid/CUBRID/testdb/lob

**Connection Configuration of Applications**

Connect the application to access in read/write mode to the broker of *node A* or *node B*. The following is an example of a JDBC application.

.. code-block:: java

    Connection connection = DriverManager.getConnection(
        "jdbc:CUBRID:nodeA:33000:testdb:::?charSet=utf-8&altHosts=nodeB:33000", "dba", "");

Connect the application to access in read-only mode to the broker of *node C*, *node D* or *node E*. The following is an example of a JDBC application. Configure "**loadBalance**\ =true" on the URL to connect randomly to the main host and hosts which are specified by **altHosts**.

.. code-block:: java

    Connection connection = DriverManager.getConnection(
        "jdbc:CUBRID:nodeC:33000:testdb:::?charSet=utf-8&loadBalance=true&altHosts=nodeD:33000,nodeE:33000", "dba", "");

For details, see :ref:`ha-jdbc-conf`, :ref:`ha-cci-conf`, and :ref:`ha-php-conf` in Environment Configuration.

**Remark**

The path of a transaction log in these configurations is as follows:

.. image:: /images/image35.png

Multiple-Standby Server Structure
---------------------------------

Although its node structure has a single master node and a single slave node, many slave nodes from different services are physically configured in a single server.

This structure is for very small services in which the reading load of slave nodes is light. It is strictly for the availability of the CUBRID service. For this reason, when a master node that failed after a failover has been restored, the load must be moved back to the original master node to minimize the load of the server with multiple-slave nodes.

.. image:: /images/image36.png

**An Example of Node Configuration**

You can configure each node in the basic structure of HA as shown below:

*   **node AM**, **node AS** : Configure them in the same manner.

    *   Configure the **ha_mode** of the **cubrid.conf** file to **on**. 
    
        ::

            ha_mode=on

    *   The following example shows how to configure **cubrid_ha.conf**. 
    
        ::

            ha_port_id=10000
            ha_node_list=cubridA@Host1:Host5
            ha_db_list=testdbA1,testdbA2

*   **node BM**, **node BS** : Configure them in the same manner.

    *   Configure the **ha_mode** of the **cubrid.conf** file to **on**. 
    
        ::

            ha_mode=on

    *   The following example shows how to configure **cubrid_ha.conf**. 
    
        ::

            ha_port_id=10001
            ha_node_list=cubridB@Host2:Host5
            ha_db_list=testdbB1,testdbB2

*   **node CM**, **node CS** : Configure them in the same manner.

    *   Configure the **ha_mode** of the **cubrid.conf** file to **on**. 
    
        ::

            ha_mode=on

    *   The following example shows how to configure **cubrid_ha.conf**. 
    
        ::

            ha_port_id=10002
            ha_node_list=cubridC@Host3:Host5
            ha_db_list=testdbC1,testdbC2

*   **node DM**, **node DS** : Configure them in the same manner.

    *   Configure the **ha_mode** of the **cubrid.conf** file to **on**. 
    
        ::

            ha_mode=on

    *   The following is an example of the **cubrid_ha.conf** configuration. 
    
        ::

            ha_port_id=10003
            ha_node_list=cubridD@Host4:Host5
            ha_db_list=testdbD1,testdbD2

HA Constraints
==============

**Supported Platforms**

Currently, CUBRID HA is supported by Linux only. All nodes within CUBRID HA groups must be configured on the same platforms.

**Table Primary Key**

CUBRID HA synchronizes data among nodes with the following method (as known as transaction log shipping): It replicates the primary key-based replication logs generated from the server of a master node to a slave node and then reflects the replication logs to the slave node.

If data of the specific table within CUBRID HA groups is not synchronized, you should check whether the appropriate primary key has specified for the table.

On the partitioned table, the table which has promoted some partitions by the **PROMOTE** statement replicates all data to the slave. However, since the table does not have the primary key, the data changes on the table made by the master are not applied to the slave.

**Java Stored Procedure**

Because using java stored procedures in CUBRID HA cannot be replicated, java stored procedures should be configured to all nodes. For more details, see :ref:`jsp-environment-configuration`.

**Method**

CUBRID HA synchronizes data among nodes within CUBRID HA groups based on replication logs, So using method that does not generate replication logs may cause data inconsistency among nodes within CUBRID HA groups.

Therefore, in CUBRID HA environment, it is not recommended to use method.

**UPDATE STATISTICS Statement**

The **UPDATE STATISTICS** statement which updates statistics is not replicated to the slave node.

**Standalone Mode**

The replication logs are not generated as for tasks performed in standalone mode. For this reason, data inconsistency among nodes within CUBRID HA groups may occur when performing tasks in standalone mode.

**Serial Cache**

To enhance performance, a serial cache does not access Heap and does not generate replication logs when retrieving or updating serial information. Therefore, if you use a serial cache, the current values of serial caches will be inconsistent among the nodes within CUBRID HA groups.

**cubrid backupdb -r**

This command is used to back up a specified database. If the **-r** option is used, logs that are not required for recovery will be deleted. This deletion may result in data inconsistency among nodes within CUBRID HA groups. Therefore, you must not use the **-r** option.

**On-line backup**

If you want to perform on-line backup in HA environment, add @\ *hostname*\ after the database name. *hostname*\ is a name defined in $CUBRID_DATABASES/databases.txt. Specify "@localhost" because you generally perform on-line backup on the local database.

::

    cubrid backupdb -C -D ./ -l 0 -z testdb@localhost

Backup during running database may occur disk I/O load. Therefore, it is recommended to run backup on slave DB than master DB.

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

This scenario assumes that the database has been created by below commands. When you run createdb, a locale name and a charset should be the same between a master node and a slave node.

::

    export CUBRID_DATABASES=/home/cubrid/DB
    mkdir $CUBRID_DATABASES/testdb
    mkdir $CUBRID_DATABASES/testdb/log
    cd $CUBRID_DATABASES/testdb
    cubrid createdb testdb -L $CUBRID_DATABASES/testdb/log en_US.utf8

At this time, the backup file is saved in the $CUBRID_DATABASES/testdb directory by default if the location is not specified.

Using the above instructions, build a new slave node by following these steps, in the order specified.

#.  Stop the master node service. 

    ::

        [nodeA]$ cubrid service stop

#. Set the master node HA and the slave node HA.

    *   Set the **$CUBRID/conf/cubrid.conf** as identical for both the master node and the slave node. 
    
        ::

            [service]
            service=server,broker,manager
                        
            # Add the database name to run when starting the service
            server=testdb

            [common]
            ...

            # Add when configuring the HA (Logging parameters)
            log_max_archives=100
            force_remove_log_archives=no

            # Add when configuring the HA (HA mode)
            ha_mode=on

    *   Set the **$CUBRID/conf/cubrid_ha.conf** as identical for both the master node and the slave node. 
    
        ::

            [common]
            ha_port_id=59901
            
            # cubrid is a group name of HA system, nodeA and nodeB are host names.
            ha_node_list=cubrid@nodeA:nodeB
            
            ha_db_list=testdb
            ha_copy_sync_mode=sync:sync
            ha_apply_max_mem_size=500

    *   Set the **$CUBRID_DATABASES/databases.txt** as identical for both the master node and the slave node. 
    
        ::

            #db-name    vol-path        db-host     log-path     lob-base-path
            testdb       /home/cubrid/DB/testdb nodeA:nodeB   /home/cubrid/DB/testdb/log  file:/home/cubrid/DB/testdb/lob

    *   Set locale library as identical for both the master node and the slave node.
    
        ::
        
            [nodeA]$ scp $CUBRID/conf/cubrid_locales.txt cubrid_usr@nodeB:/$CUBRID/conf/.
            
            [nodeB]$ make_locale.sh -t 64bit
            
    *   Create a database directory to the slave node. 
    
        ::
    
            [nodeB]$ cd $CUBRID_DATABASES
            [nodeB]$ mkdir testdb

    *   Create the log directory to the slave node(same location with the master node). 
    
        ::

            [nodeB]$ cd $CUBRID_DATABASES/testdb
            [nodeB]$ mkdir log

#.  Back up the database of the master node and copy the backup file to the slave node. If the location where the backup file will be saved in the master node is not specified, the location is set as the log directory of *testdb* by default. Copy the backup file to the same location in the slave node. *testdb* _bk0v000 is the backup volume file and *testdb* _bkvinf is the backup volume information file. 

    ::

        [nodeA]$ cubrid backupdb -z -S testdb
        Backup Volume Label: Level: 0, Unit: 0, Database testdb, Backup Time: Thu Apr 19 16:05:18 2012
        [nodeA]$ cd $CUBRID_DATABASES/testdb/log
        [nodeA]$ scp testdb_bk* cubrid_usr@nodeB:/home/cubrid_usr/CUBRID/databases/testdb/log
        cubrid_usr@nodeB's password:
        testdb_bk0v000                            100% 6157KB   6.0MB/s   00:00
        testdb_bkvinf                             100%   66     0.1KB/s   00:00

#.  Recover the database in the slave node. At this time, the volume path of the master node must be identical to that of the slave node. 

    ::

        [nodeB]$ cubrid restoredb -B $CUBRID_DATABASES/testdb/log demodb
    
#.  Start the master node 

    ::

        [nodeA]$ cubrid heartbeat start

#.  After confirming that the master node has started, start the slave node. If *nodeA* is changed from to-be-master to master, it means that the master node has been successfully started. 

    ::

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

#.  Confirm that the HA configurations of the master node and the slave node are successfully running 

    ::

        [nodeA]$ csql -u dba testdb@localhost -c"create table tbl(i int primary key);insert into tbl values (1),(2),(3)"
         
        [nodeB]$ csql -u dba testdb@localhost -c"select * from tbl"
         
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

+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
|   General Operation                          |   Scenario                                              |   Consideration                                                                                                    |
+==============================================+=========================================================+====================================================================================================================+
| Online Backup                                | Operation task is performed at each master node and     | Note that there may be a delay in the transaction of master node due to the operation task.                        |
|                                              | slave node each during operation.                       |                                                                                                                    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| Schema change (excluding basic key change),  | When an operation task occurs at a master node, it is   | Because replication log is copied and reflected to a slave node after an operation task is completed in a master   |
| index change, authorization change           | automatically replication reflected to a slave node.    | node, operation task time is doubled. Changing schema must be processed without any failover.                      |
|                                              |                                                         | Index change and authority change other than the schema change can be performed by stopping each node and          |
|                                              |                                                         | executing standalone mode (ex: the **-S** option of the **csql** utility) when the operation time is important.    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| Add volume                                   | Operation task is performed at each DB regardless of    | Note that there may be a delay in the transaction of master node due to the operation task.                        |
|                                              | HA structure.                                           | If operation task time is an issue, operation task can be performed by stopping each node and executing standalone |
|                                              |                                                         | mode (ex: the **-S**  of the **cubrid addvoldb**  utility).                                                        |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| Failure node server replacement              | It can be replaced without restarting the CUBRID HA     | The failure node must be registered in the ha_node_list of CUBRID HA group, and the node name must not be changed  |
|                                              | group when a failure occurs.                            | during replacement.                                                                                                |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| Failure broker server replacement            | It can be replaced without restarting the broker when   | The connection to a broker replaced at a client can be made by rcTime which is configured in URL string.           |
|                                              | a failure occurs.                                       |                                                                                                                    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| DB server expansion                          | You can execute **cubrid heartbeat reload** in each     | Starts or stops the **copylogdb/applylogdb**                                                                       |
|                                              | node after configuration change (ha_node_list,          | processes which were added or deleted by loading changed configuration information.                                |
|                                              | ha_replica_list) without restarting the previously      |                                                                                                                    |
|                                              | configured CUBRID HA group.                             |                                                                                                                    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| Broker server expansion                      | Run additional brokers without restarting               | Modify the URL string to connect to a broker where a client is added.                                              |
|                                              | existing brokers.                                       |                                                                                                                    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+

**When Failover Occurs**

You must stop nodes in CUBRID HA group and complete operation before performing the following operations. 

+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
|   General Operation                          |   Scenario                                              |   Consideration                                                                                                    |
+==============================================+=========================================================+====================================================================================================================+
| DB server configuration change               | A node whose configuration is changed is restarted when |                                                                                                                    |
|                                              | the configuration in  **cubrid.conf** is changed.       |                                                                                                                    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| Change broker configuration, add broker      | A broker whose configuration is changed is restarted    |                                                                                                                    |
| , and delete broker                          | when the configuration in **cubrid_broker.conf**        |                                                                                                                    |
|                                              | is changed.                                             |                                                                                                                    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| DBMS version patch                           | Restart nodes and brokers in HA group after version     | Version patch means there is no change in the internal protocol, volume, and log of CUBRID.                        |
|                                              | patch.                                                  |                                                                                                                    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+

Operation Scenario during Read Service
--------------------------------------

The operation scenario written in this page is only applied to read service. It is required to allow read service only or dynamically change mode configuration of broker to Read Only. There can be two types of operation scenarios in which failover occurs or it does not occur.

**When Failover Does Not Occur**

You can perform the following operations without stopping and restarting nodes in CUBRID HA groups.

+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
|   General Operation                          |   Scenario                                              |   Consideration                                                                                                    |
+==============================================+=========================================================+====================================================================================================================+
| Schema change (primary key change)           | When an operation task is performed at the master node, | In order to change the primary key, the existing key must be deleted and a new one added.  For this reason,        |
|                                              | it is automatically reflected to the slave node.        | replication reflection may not occur due to the HA internal structure which reflects primary key-based replication |
|                                              |                                                         | logs. Therefore, operation tasks must be performed during the read service.                                        |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| Schema change (excluding basic key change),  | When an operation task is performed at the master node, | Because replication log is copied and reflected to a slave node after an operation task is completed in a master   |
| index change, authorization change           | it is automatically reflected to the slave node.        | node, operation task time is doubled. Changing schema must be processed without any failover. Index change         |
|                                              |                                                         | and authority change other than the schema change can be performed by stopping each node and executing             |
|                                              |                                                         | standalone mode(ex: the span class="nkeyword">-S option of **csql**) when the operation time is important.         |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+

**When Failover Occurs**

You must stop nodes in CUBRID HA group and complete operation before performing the following operations. 

+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
|   General Operation                          |   Scenario                                              |   Consideration                                                                                                    |
|                                              |                                                         |                                                                                                                    |
+==============================================+=========================================================+====================================================================================================================+
| DBMS version upgrade                         | Restart each node and broker in the CUBRID HA group     | A version upgrade means that there have been changed in the internal protocol, volume, or log of CUBRID.           |
|                                              | after they are upgraded.                                | Because there are two different versions of the protocols, volumes, and logs of a broker and server during an      |
|                                              |                                                         | upgrade, an operation task must be performed to make sure that each client and broker (before/after upgrade) are   |
|                                              |                                                         | connected to the corresponding counterpart in the same version.                                                    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| Massive data processing                      | Stop the node that must be changed, perform an          | This processes massive data that cannot be segmented.                                                              |
| (INSERT/UPDATE/DELETE)                       | operation task, and then execute the node.              |                                                                                                                    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+

Operation Scenario after Service Stop
-------------------------------------

You must stop all nodes in CUBRID HA group before performing the following operation.

+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
|   General Operation                          |   Scenario                                              |   Consideration                                                                                                    |
|                                              |                                                         |                                                                                                                    |
+==============================================+=========================================================+====================================================================================================================+
| Changing the host name and IP of a DB server | Stop all nodes in the CUBRID HA group, and restart      | When a host name has been changed, change the **databases.txt** file of each broker and reset the broker           |
|                                              | them after the operation task.                          | connection with **cubrid broker reset**.                                                                           |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+

Setting Replica Replication Delay
---------------------------------

This scenario delays the replication of the master node data in replica node, and stops the replication of the master node data at the specific time, to detect the case which someone deleted the data by mistake and stop the replication at the specified time.

+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+
| General Operation                            | Scenario                                                | Consideration                                                                                                      |
+==============================================+=========================================================+====================================================================================================================+
| Setting the delay of replication             | Specify the term of replicated delay to replica node,   | specify **ha_replica_delay** and ha_replica_time_bound in cubrid.conf                                              |
| in replica node                              | and let the replication stop on the specified time      |                                                                                                                    |
+----------------------------------------------+---------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+

Detection of Replication Mismatch and Rebuild
=============================================

Detection of Replication Mismatch
---------------------------------

Replication mismatch between replication nodes, indicating that data of the master node and the slave node is not identical, can be detected to some degree by the following process. However, please note that there is no more accurate way to detect a replication mismatch than by directly comparing the data of the master node to the data of the slave node. If it is determined that there has been a replication mismatch, you should rebuild the database of the master node to the slave node (see :ref:`rebuilding-replication`.)

*   On the slave node, execute **cubrid applyinfo** to check the "Fail count" value. If the "Fail count" is 0, it can be determined that no transaction has failed in replication (see :ref:`cubrid-applyinfo`.) ::

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

*   To check whether copying replication logs has been delayed or not on the slave node, execute **cubrid applyinfo** and compare the "Append LSA" value of "Copied Active Info." to the "Append LSA" value of "Active Info.". If there is a big difference between the two values, it means that delay has occurred while copying the replication logs to the slave node (see :ref:`cubrid-applyinfo`.) ::

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

*   If a delay seems to occur when copying the replication logs, check whether the network line speed is slow, whether there is sufficient free disk space, disk I/O is normal, etc.

*   To check the delay in applying the replication log in the slave node, execute **cubrid applyinfo** and compare the "Committed page" value of "Applied Info." to the "EOF LSA" value of "Copied Active Info.". If there is a big difference between the two values, it means that a delay has occurred while applying the replication logs to the slave database (see :ref:`cubrid-applyinfo`.) ::

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

*   If the delay in applying the replication logs is too long, it may be due to a transaction with a long execution time. If the transaction is performed normally, a delay in applying the replication logs may normally occur. To determine whether it is normal or abnormal, continuously execute **cubrid applyinfo** and check whether applylogdb continuously applies replication logs to the slave node or not.

*   Check the error log message created by the copylogdb process and the applylogdb process (see the error message).

*   Compare the number of records on the master database table to that on the slave database table.

.. _ha-error:

HA Error Messages
-----------------

CAS 프로세스(cub_cas)
^^^^^^^^^^^^^^^^^^^^^

[번역] CAS 프로세스의 오류 메시지는  **$CUBRID/log/broker/error_log**\ `<broker_name>_<app_server_num>`\ **.err**\ 에 기록된다. 아래는 HA 환경에서 접속 시 발생하는 오류 메시지를 별도로 정리한 것이다.

**브로커와 DB 서버 간 handshake 오류 메시지**

+-------+-----------------------------------------------------+--------------+------------------------------------------------------+----------------------------------------------------------------------+
| 오류  | 오류 메시지                                         | severity     | 설명                                                 | 조치 사항                                                            |
| 코드  |                                                     |              |                                                      |                                                                      |
+=======+=====================================================+==============+======================================================+======================================================================+
| -1139 | Handshake error (peer host ?): incompatible         | error        | 브로커 ACCESS_MODE와 서버의 상태 (active/standby)    |                                                                      |
|       | read/write mode. (client: ?, server: ?)             |              | 불일치(:ref:`broker-mode` 참고)                      |                                                                      |
+-------+-----------------------------------------------------+--------------+------------------------------------------------------+----------------------------------------------------------------------+
| -1140 | Handshake error (peer host ?):                      | error        | ha_delay_limit을 설정한 서버에서 복제 지연 발생      |                                                                      |
|       | HA replication delayed.                             |              |                                                      |                                                                      |
+-------+-----------------------------------------------------+--------------+------------------------------------------------------+----------------------------------------------------------------------+
| -1141 | Handshake error (peer host ?): replica-only         | error        | 레플리카만 접속 가능한 브로커(CAS)가 레플리카가      |                                                                      |
|       | client to non-replica server.                       |              | 아닌 서버에 접속 시도(:ref:`broker-mode` 참고)       |                                                                      |
+-------+-----------------------------------------------------+--------------+------------------------------------------------------+----------------------------------------------------------------------+
| -1142 | Handshake error (peer host ?): remote access to     | error        | HA maintenance 모드인 서버에 원격                    |                                                                      |
|       | server not allowed.                                 |              | (:ref:`ha-server` 참고)                              |                                                                      |
+-------+-----------------------------------------------------+--------------+------------------------------------------------------+----------------------------------------------------------------------+
| -1143 | Handshake error (peer host ?): unidentified         | error        | 서버 버전 알 수 없음                                 |                                                                      |
|       | server version.                                     |              |                                                      |                                                                      |
+-------+-----------------------------------------------------+--------------+------------------------------------------------------+----------------------------------------------------------------------+

**연결 오류 메시지**

+-------+-----------------------------------------------------+--------------+------------------------------------------------------+----------------------------------------------------------------------+
| 오류  | 오류 메시지                                         | severity     | 설명                                                 | 조치 사항                                                            |
| 코드  |                                                     |              |                                                      |                                                                      |
+=======+=====================================================+==============+======================================================+======================================================================+
| -353  | Cannot make connection to master server on host ?.  | error        | cub_master 프로세스 down                             |                                                                      |
+-------+-----------------------------------------------------+--------------+------------------------------------------------------+----------------------------------------------------------------------+
| -1144 | Timed out attempting to connect to ?.               | error        | 장비 down                                            |                                                                      |
|       | (timeout: ? sec(s))                                 |              |                                                      |                                                                      |
+-------+-----------------------------------------------------+--------------+------------------------------------------------------+----------------------------------------------------------------------+

Replication Log Copy Process (copylogdb)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    The error messages from the replication log copy process are stored in **$CUBRID/log/db-name@remote-node-name_copylogdb.err**. The severity levels of error messages found in the replication log copy process are as follows: fatal, error, and notification. The default level is error. Therefore, to record notification error messages, it is necessary to change the value of **error_log_level** in the **cubrid.conf** file. For details, see :ref:`error-parameters`.

**Initialization Error Messages**

The error messages that can be found in initialization stage of replication log copy process are as follows:

+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Error | Error Message                                        | severity     | Description                                         | Solution                                                             |
| Code  |                                                      |              |                                                     |                                                                      |
+=======+======================================================+==============+=====================================================+======================================================================+
| -10   | Cannot mount the disk volume ?.                      | error        | Fails to open a replication log file.               | Check if replication logs exist. For the location of replication     |
|       |                                                      |              |                                                     | logs, see `Default Environment Configuration                         |
|       |                                                      |              |                                                     | <#admin_admin_ha_conf_ha_htm>`_.                                     |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -78   | Internal error: an I/O error occurred while          | fatal        | Fails to read a replication log.                    | Check the replication log by using the cubrid applyinfo utility.     |
|       | reading logical log page ? (physical page ?) of ?    |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -81   | Internal error: logical log page ? may be            | fatal        | A replication log page error, in which the          | Check the error log of the database server process to which the      |
|       | corrupted.                                           |              | replication log copy process has been copied from   | replication log copy process is connected.                           |
|       |                                                      |              | the connected database server process.              | This error log can be found in $CUBRID/log/server.                   |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1039 | log writer: log writer has been started. mode: ?     | error        | The replication log copy process has been           | No action is required because this error message is recorded to      |
|       |                                                      |              | successfully initialized and started.               | display the start information of the replication log copy process.   |
|       |                                                      |              |                                                     | Ignore any error messages which are displayed between the start of   |
|       |                                                      |              |                                                     | replication log copy process and output of this error message since  |
|       |                                                      |              |                                                     | there is a possibility that an error message is shown up even in     |
|       |                                                      |              |                                                     | normal situation.                                                    |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+

**Replication Log Request and Reception Error Messages**

The replication log copy process requests a replication log from the connected database server and receives the corresponding replication log. Error messages that can be found in this stage are as follows:

+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Error | Error Message                                        | severity     | Description                                         | Solution                                                             |
| Code  |                                                      |              |                                                     |                                                                      |
+=======+======================================================+==============+=====================================================+======================================================================+
| -89   | Log ? is not included in the given database.         | error        | The previously replicated log and the log to be     | Check information of the database server/host to which the           |
|       |                                                      |              | replication do not match.                           | replication log copy process is connected. If you need to change the |
|       |                                                      |              |                                                     | database server/host information, reinitialize it by deleting the    |
|       |                                                      |              |                                                     | existing replication log and then restarting.                        |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -186  | Data receiver error from the server                  | error        | Incorrect information has been received from the    | It will be internally recovered.                                     |
|       |                                                      |              | database server to which the replication log copy   |                                                                      |
|       |                                                      |              | process is connected.                               |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -199  | The server is not responding.                        | error        | The connection to the database server has been      | It will be internally recovered.                                     |
|       |                                                      |              | terminated.                                         |                                                                      |
|       |                                                      |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+

**Replication Log Write Error Messages**

The replication log copy process copies the replication log that was received from the connected database server process to the location (**ha_copy_log_base**) specified in the **cubrid_ha.conf** file. Error messages that can be found in this stage are as follows:

+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Error | Error Message                                        | severity     | Description                                         | Solution                                                             |
| Code  |                                                      |              |                                                     |                                                                      |
+=======+======================================================+==============+=====================================================+======================================================================+
| -10   | Cannot mount the disk volume ?.                      | error        | Fails to open a replication log file.               | Check if replication logs exist.                                     |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -79   | Internal error: an I/O error occurred while          | fatal        | Fails to write a replication log.                   | It will be internally recovered.                                     |
|       | writing logical log page ? (physical page ?)         |              |                                                     |                                                                      |
|       | of ?.                                                |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -80   | An error occurred due to insufficient space in       | fatal        | Fails to write a replication log due to             | Check if there is sufficient space left in the disk partition.       |
|       | operating system device while writing logical        |              | insufficient file system space.                     |                                                                      |
|       | log page ?(physical page ?) of ?.                    |              |                                                     |                                                                      |
|       | Up to ? bytes in size are allowed.                   |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+

**Replication Log Archive Error Messages**

The replication log copy process periodically archives the replication logs that have been received from the connected database server process. Error messages that can be found in this stage are as follows:

+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Error | Error Message                                        | severity     | Description                                         | Solution                                                             |
| Code  |                                                      |              |                                                     |                                                                      |
+=======+======================================================+==============+=====================================================+======================================================================+
| -78   | Internal error: an I/O error occurred while          | fatal        | Fails to read a replication log during archiving.   | Check the replication log by using the cubrid applyinfo utility.     |
|       | reading logical log page ?                           |              |                                                     |                                                                      |
|       | (physical page ?) of ?.                              |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -79   | Internal error: an I/O error occurred while          | fatal        | Fails to write an archive log.                      | It will be internally recovered.                                     |
|       | writing logical log page ?                           |              |                                                     |                                                                      |
|       | (physical page ?) of ?.                              |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -81   | Internal error: logical log page ? may be            | fatal        | Found an error on the replication log during        | Check the replication log by using the cubrid applyinfo utility.     |
|       | corrupted.                                           |              | archiving.                                          |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -98   | Cannot create an archive log ? to archive pages      | fatal        | Fails to create an archive log file.                | Check if there is sufficient space left in the disk partition.       |
|       | from ? to ?.                                         |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -974  | An archive log ? to archive pages from ? to ? has    | notification | Information on an archive log file                  | No action is required because this error message is recorded to keep |
|       | been created.                                        |              |                                                     | information on newly created archive.                                |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+

**Stop and Restart Error Messages**

Error messages that can be found in this stage are as follows:

+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Error | Error Message                                        | severity     | Description                                         | Solution                                                             |
| Code  |                                                      |              |                                                     |                                                                      |
+=======+======================================================+==============+=====================================================+======================================================================+
| -1037 | log writer: log writer is terminated by signal.      | error        | The copylogdb process has been terminated by a      | It will be internally recovered.                                     |
|       |                                                      |              | specified signal.                                   |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+

Replication Log Reflection Process (applylogdb)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The error messages from the replication log reflection process are stored in **$CUBRID/log/**\ *db-name*\ **@**\ *local-node-name*\ **_applylogdb_**\ *db-name*\ **_**\ *remote-node-name*\ **.err**. The severity levels of error message found in the replication log reflection process are as follow: fatal, error, and notification. The default level is error. Therefore, to record notification error messages, it is necessary to change the value of **error_log_level** in the **cubrid.conf** file. For details, see :ref:`error-parameters`.

**Initialization Error Messages**

The error messages that can be found in initialization stage of replication log reflection process are as follows:

+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Error | Error Message                                        | severity     | Description                                         | Solution                                                             |
| Code  |                                                      |              |                                                     |                                                                      |
+=======+======================================================+==============+=====================================================+======================================================================+
| -10   | Cannot mount the disk volume ?.                      | error        | An applylogdb that is trying to reflect the same    | Check if there is an applylogdb process that is trying to reflect    |
|       |                                                      |              | replication log is already running.                 | the same replication log.                                            |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1038 | log applier: log applier has been started.           | error        | It will be started normally after initialization of | No action is required because this error is recorded to display the  |
|       | required LSA: ?|?. last committed LSA: ?|?.          |              | applylogdb succeeds.                                | start information of  the replication log reflection process.        |
|       | last committed repl LSA: ?|?                         |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+

**Log Analysis Error Messages**

The replication log reflection process reads, analyzes, and reflects the replications logs that have been copied by the replication log copy process. The error message that can be found in this stage are as follows:

+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Error | Error Message                                        | severity     | Description                                         | Solution                                                             |
| Code  |                                                      |              |                                                     |                                                                      |
+=======+======================================================+==============+=====================================================+======================================================================+
| -13   | An I/O error occurred while reading page ?           | error        | Fails to read a log page to be reflected.           | Check the replication log by using the cubrid applyinfo utility.     |
|       | in volume ?.                                         |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -17   | Internal error: Trying to read page ? of the         | fatal        | Trying to read a log page that does not exist in    | Check the replication log by using the cubrid applyinfo utility.     |
|       | volume ? which has been already released.            |              | the replication log.                                |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -81   | Internal error: logical log page ? may be            | fatal        | There is an inconsistency between an old log under  | Check the replication log by using the cubrid applyinfo utility.     |
|       | corrupted.                                           |              | replication reflection and the current log, or      |                                                                      |
|       |                                                      |              | there is a replication log record error.            |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -82   | Cannot mount the disk volume/file ?.                 | error        | No replication log file exists.                     | Check if replication logs exist.                                     |
|       |                                                      |              |                                                     | Check the replication log by using the cubrid applyinfo utility.     |
|       |                                                      |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -97   | Internal error: unable to find log page ? in         | error        | No log page exists in the replication log.          | Check the replication log by using the cubrid applyinfo utility.     |
|       | log archives.                                        |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -897  | Decompression failure                                | error        | Fails to decompress the log record.                 | Check the replication log by using the cubrid applyinfo utility.     |
|       |                                                      |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1028 | log applier: Unexpected EOF log record exists        | error        | Incorrect log record exists in the archive log.     | Check the replication log by using the cubrid applyinfo utility.     |
|       | in the Archive log. LSA: ?|?.                        |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1029 | log applier: Incorrect log page/offset. page         | error        | Incorrect log record exists.                        | Check the replication log by using the cubrid applyinfo utility.     |
|       | HDR: ?|?, final: ?|?, append LSA: ?|?, EOF LSA:      |              |                                                     |                                                                      |
|       | ?|?, ha file status: ?, is end-of-log: ?.            |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1030 | log applier: Incorrect log record. LSA: ?|?,         | error        | Log record header error                             | Check the replication log by using the cubrid applyinfo utility.     |
|       | forw LSA: ?|?, backw LSA: ?|?, Trid: ?, prev         |              |                                                     |                                                                      |
|       | tran LSA: ?|?, type: ?.                              |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+

**Replication Log Reflection Error Messages**

The replication log reflection process reads, analyzes, and reflects the replication logs that have been copied by the replication log copy process. Error messages that can be found in this stage are as follows:

+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Error | Error Message                                        | severity     | Description                                         | Solution                                                             |
| Code  |                                                      |              |                                                     |                                                                      |
+=======+======================================================+==============+=====================================================+======================================================================+
| -72   | The transaction (index ?, ?@?|?) has been            | error        | Fails to reflect replication due to deadlock, etc.  | It will be recovered internally.                                     |
|       | cancelled by system.                                 |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -111  | Your transaction has been cancelled due to           | error        | Fails to reflect replication because the database   | It will be recovered internally.                                     |
|       | server failure or a mode change.                     |              | server process in which replication is supposed     |                                                                      |
|       |                                                      |              | to be reflected has been terminated or its mode     |                                                                      |
|       |                                                      |              | has been changed.                                   |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -191  | Cannot connect to server ? on ?.                     | error        | The connection to the database server process in    | It will be recovered internally.                                     |
|       |                                                      |              | which replication is supposed to be reflected has   |                                                                      |
|       |                                                      |              | been terminated.                                    |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -195  | Server communication error: ?.                       | error        | The connection to the database server process in    | It will be recovered internally.                                     |
|       |                                                      |              | which replication is supposed to be reflected has   |                                                                      |
|       |                                                      |              | been terminated.                                    |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -224  | The database has not been resumed.                   | error        | The connection to the database server process in    | It will be recovered internally.                                     |
|       |                                                      |              | which replication is supposed to be reflected has   |                                                                      |
|       |                                                      |              | been terminated.                                    |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1027 | log applier: Failed to change the reflection         | error        | Fails to change of replication reflection.          | It will be recovered internally.                                     |
|       | status from ? to ?.                                  |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1031 | log applier: Failed to reflect the Schema            | error        | Fails to reflect SCHEMA replication.                | Check the consistency of the replication. If it is inconsistent,     |
|       | replication log. class: ?, schema: ?, internal       |              |                                                     | reconfigure the HA replication.                                      |
|       | error: ?.                                            |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1032 | log applier: Failed to reflect the Insert            | error        | Fails to reflect INSERT replication.                | Check the consistency of the replication. If it is inconsistent,     |
|       | replication log. class: ?, key: ?, internal          |              |                                                     | reconfigure the HA replication.                                      |
|       | error: ?.                                            |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1033 | log applier: Failed to reflect the Update            | error        | Fails to reflect UPDATE replication.                | Check the consistency of the replication. If it is inconsistent,     |
|       | replication log. class: ?, key: ?, internal          |              |                                                     | reconfigure the HA replication.                                      |
|       | error: ?.                                            |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1034 | log applier: Failed to reflect the Delete            | error        | Fails to reflect DELETE replication.                | Check the consistency of the replication. If it is inconsistent,     |
|       | replication log. class: ?, key: ?, internal          |              |                                                     | reconfigure the HA replication.                                      |
|       | error: ?.                                            |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1040 | HA generic: ?.                                       | notification | Changes the last record of the archive log or       | No action is required because this error message is recorded to      |
|       |                                                      |              | replication reflection status.                      | provide general information.                                         |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+

**Stop and Restart Error Messages**

The error messages that can be found in this stage are as follows:

+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Error | Error Message                                        | severity     | Description                                         | Solution                                                             |
| Code  |                                                      |              |                                                     |                                                                      |
+=======+======================================================+==============+=====================================================+======================================================================+
| -1035 | log applier: The memory size (? MB) of the log       | error        | The replication log reflection process has been     | It will be recovered internally.                                     |
|       | applier is larger than the maximum memory size       |              | restarted due to reaching the maximum memory size   |                                                                      |
|       | (? MB), or is doubled the starting memory size       |              | limit.                                              |                                                                      |
|       | (? MB) or more. required LSA: ?|?. last              |              |                                                     |                                                                      |
|       | committed LSA: ?|?.                                  |              |                                                     |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
| -1036 | log applier: log applier is terminated by signal.    | error        | The replication log reflection process has been     | It will be recovered internally.                                     |
|       |                                                      |              | terminated by a specified signal.                   |                                                                      |
+-------+------------------------------------------------------+--------------+-----------------------------------------------------+----------------------------------------------------------------------+
                                                                                                                         
.. _rebuilding-replication:

Rebuilding Replication
----------------------

Replication rebuilding is required in CUBRID HA when data in the CUBRID HA group is inconsistent because of multiple failures in multiple-slave node structure, or because of a generic error. Rebuilding replications in CUBRID HA is perform done through a **ha_make_slavedb.sh** script. With the **cubrid applyinfo** utility, you can check the replication progress; however replication inconsistency is not detected. If you want to determine whether replication is inconsistent correctly, you must examine data of the master and slave nodes yourself.

For rebuilding replications, the following environment must be the same in master, slave and replica nodes.

*   CUBRID version
*   Environmental variable (**$CUBRID**, **$CUBRID_DATABASES**, **$LD_LIBRARY_PATH, $PATH**)
*   The paths of database volume, log, and replication
*   Username and password of the Linux server
*   HA-related parameters except for **ha_mode** and **ha_copy_sync_mode**, **ha_ping_hosts**

[번역] 복제 재구축은 ha_make_slavedb.sh 스크립트를 실행하여 가능하며, 재구축이 아닌 신규 구축인 경우 cubrid.conf, cubrid_ha.conf, databases.txt의 파일들이 설정되어야 한다.

먼저 복제 재구축에 필요한 ha_make_slavedb.sh 스크립트를 살펴보고, 마스터에서 슬레이브 구축, 슬레이브에서 레플리카 구축, 레플리카에서 레플리카 구축에 대해 알아본다.

참고로, 다중 슬레이브 노드를 구축하고자 하는 경우 ha_make_slavedb.sh 스크립트를 사용할 수 없다.

ha_make_slavedb.sh Script
^^^^^^^^^^^^^^^^^^^^^^^^^

To rebuild replications, use the **ha_make_slavedb.sh** script. This script is located in **$CUBRID/share/scripts/ha**. Before rebuilding replications, the following items must be configured for the environment of the user. This script is supported since the version 2008 R2.2 Patch 9 and its configuration is different from 2008 R4.1 Patch 2 or earlier. This document describes it in CUBRID 2008 R4.1 Patch 2 or later.

*   **target_host** : The host name of the source node (master node in general) for rebuilding replication. It should be registered in **/etc/hosts**. A slave node can rebuild replication by using the master node or the replica node as the source. A replica node can rebuild replication by using the slave node or another replica node as the source.

    ..  아래 내용은 복제 재구축 스크립트 변경 전까지는 불필요

        If you want rebuild replication by using the slave node or the replica node as the source, moderately large value of :ref:`ha_copy_log_max_archives <ha_copy_log_max_archives>` parameter in cubrid_ha.conf should be specified.

*   **repl_log_home** : Specifies the home directory of the replication log of the master node. It is usually the same as **$CUBRID_DATABASES**. You must enter an absolute path and should not use a symbolic link. You also cannot use a slash (/) after the path.

The following are optional items:

*   **db_name** : Specifies the name of the database to be replicated. If not specified, the first name that appears in **ha_db_list** in **$CUBRID/conf/cubrid_ha.conf** is used.

*   **backup_dest_path** : Specifies the path in which the backup volume is created when executing **backupdb** in source node for rebuilding replication.

*   **backup_option** : Specifies necessary options when executing **backupdb** in source node in which replication will be rebuilt.

*   **restore_option** : Specifies necessary options when executing **restoredb** in target node in which replication will be rebuilt.

*   **scp_option** : Specifies the **scp** option which enables backup of source node in which replication is rebuilt to copy into the target node. The default option is **-l 131072**, which does not impose an overload on network (limits the transfer rate to 16 MB).

Once the script has been configured, execute the **ha_make_slavedb.sh** script in target node in which replication will be rebuilt. When the script is executed, rebuilding replication happens in a number of phases. To move to the next stage, the user must enter an appropriate value. The following are the descriptions of available values.

*   **yes** : Keeps going.

*   **no** : Does not move forward with any stages from now on.

*   **skip**: Skips to the next stage. This input value is used to ignore a stage that has not necessarily been executed when retrying the script after it has failed.

.. warning::

    *   ha_make_slavedb.sh script executes the connection command into the remote node by **expect** and **ssh**; therefore, **expect** command should be installed('yum install expect' in root account) and **ssh** should be possible in remote connection.

.. note::

    *   Backup volumes are required for rebuilding replication
        
        To replicate, you must copy the physical image of the database volume in the target node to the database of the node to be replicated. However, **cubrid unloaddb** backs up only logical images so replication using **cubrid unloaddb** and **cubrid loaddb** is unavailable. Because **cubrid backupdb** backs up physical images, replication is possible by using this utility. The **ha_make_slavedb.sh** script performs replication by using **cubrid backupdb**.
   
    *   Online-backup of a source node and restoration of a rebuilding node during rebuilding replication.

        [번역] **ha_make_slavedb.sh** 스크립트는 수행 도중 원본 노드에 대해 온라인 백업을 수행하고, 재구축되는 노드에 복구를 수행한다. 백업을 시작한 이후 추가로 진행된 트랜잭션들을 복구되는 노드에 반영하기 위해 **ha_make_slavedb.sh** 스크립트는 "마스터" 노드의 보관 로그를 복사해서 사용한다(슬레이브에서 레플리카를 구축할 때, 레플리카에서 레플리카를 구축할 때, 그리고 레플리카에서 슬레이브를 구축할 때 모두 "마스터"의 보관 로그를 사용).

        따라서 온라인 백업이 진행되는 동안 원본 노드에 추가되는 보관 로그가 삭제되지 않도록 마스터 노드에서 **cubrid.conf**\의 **force_remove_log_max_archives**\, **log_max_archives**\ 를 적절히 설정해야 한다. 자세한 내용은 아래의 구축 예들을 참고한다.

            .. 아래 내용은 스크립트 변경 전까지 위와 같이 작성됨.
            
                따라서 온라인 백업이 진행되는 동안 원본 노드에 추가되는 보관 로그와 복제 로그가 삭제되지 않도록 **cubrid.conf**\의 *force_remove_log_max_archives**\, **log_max_archives** 그리고 **cubrid_ha.conf**\의 **ha_copy_log_max_archives**\를 적절히 설정해야 한다. 자세한 내용은 아래의 구축 예들을 참고한다.

    *   **Error while executing the rebuilding replication script**
    
        The rebuilding replication script is not automatically rolled back to its previous stage even when an error occurs during the execution. This is because the slave node cannot provide normal service before rebuilding replication script is executed. To return to the phase before rebuilding replication script is executed, you must back up the existing replication logs and **db_ha_apply_info** information which is internal catalog of the master and slave nodes before building replication is executed.
        
[번역]
        
마스터에서 슬레이브 구축
^^^^^^^^^^^^^^^^^^^^^^^^

The following example shows how to configure an original node for rebuilding replications as a master mode and rebuild a slave node from the master node. [번역] 새로 구축하는 경우 Master:Slave는 1:0에서 1:1이 된다.


.. image:: /images/image37.png

*   The host name in master node: *nodeA*
*   The host name in slave node: *nodeB*

Rebuilding replications can be performed while the system is running, however, to minimize replication delay, it is recommended to execute this when there are just a few transactions per hour.

Before starting to execute the **ha_make_slavedb.sh** script, run the following.

#.  Stop the HA service of *nodeB*. If you build the slave node newly, this is not necessary.

    ::
    
        [nodeB]$ cubrid heartbeat stop

#.  [번역] *nodeB*\ 를 새로 구축한다면 :ref:`ha-configuration`\ 을 참고하여 *nodeA*\ 와 *nodeB*\ 의 **cubrid.conf**, **cubrid_ha.conf**, **cubrid_broker.conf**, **databases.txt**\를 설정한다.

    *   *nodeA*\ 와 *nodeB*\ 의 cubrid.conf에 설정

        ::
        
            ha_mode=on
            force_remove_log_archives=no
            
    *   *nodeA*\에 DB의 재구동 없이 **log_max_archives** 적용

        슬레이브 구축 시간 동안 수행되는 트랜잭션을 보관할 수 있는 보관 로그(archive logs) 개수를 10이라고 가정한다. "SET SYSTEM PARAMETERS" 문을 사용하여 "10"으로 설정하며, **cubrid.conf**\의 **log_max_archives**\는 구축/재구축 이후 기존 설정을 사용할 것이므로 변경하지 않는다.
    
        ::
        
            [nodeA]$ csql -u dba -c "SET SYSTEM PARAMETERS 'force_remove_log_archives=no'" testdb@localhost
            [nodeA]$ csql -u dba -c "SET SYSTEM PARAMETERS 'log_max_archives=10'" testdb@localhost

    *   *nodeA*\의 **cubrid_ha.conf**\에 설정
    
        **ha_node_list**\에 `nodeB`\를 추가한 후 마스터 노드에서 HA 서비스를 구동해 놓으면 슬레이브 노드가 구동되기 전까지 마스터 노드에서 쓰기 작업을 할 수 없게 되므로 `nodeA`\만 추가한다.
        
        ::
        
            ha_node_list=cubrid@nodeA
    
    *   **databases.txt**\의 **db-host** 설정
    
        ::
        
            nodeA:nodeB
            
        하지만 **databases.txt**\의 **db-host** 부분은 브로커에서 DB 서버에 접속하는 순서와 관련된 설정 파일이므로 원하는대로 변경이 가능하다.
    
#.  *nodeB*\ 에서 **ha_make_slavedb.sh** 스크립트를 설정해야 한다. **target_host**\에는 복사 원본, 즉 마스터 노드의 호스트 이름(*nodeA*)을 설정하고, **repl_log_home**\에는 복제 로그의 홈 디렉터리(기본값: **$CUBRID_DATABASES**)를 설정한다. 

    ::
         
        [nodeB]$ cd $CUBRID/share/scripts/ha
        [nodeB]$ vi ha_make_slavedb.sh
        target_host=nodeA

#.  설정 후에는 다음과 같이 슬레이브에서 **ha_make_slavedb.sh** 스크립트를 실행한다.

    ::

        [nodeB]$ cd $CUBRID/share/scripts/ha
        [nodeB]$ ./ha_make_slavedb.sh

.. _scp-setting:
        
.. note:: **ha_make_slavedb.sh** 스크립트를 수행할 때 scp 명령을 사용하게 되는데, scp 수행 시 암호 요청 과정을 생략하려면 다음과 같이 scp의 개인키를 대상 노드에, 공개키를 원본 노드에 설정하면 된다. 보다 자세한 내용은 Linux의 ssh-keygen 사용법을 참고한다.

    #.  대상 노드에 **ssh-keygen -t rsa** 를 실행하여 리눅스 사용자 계정의 홈 디렉터리 이하에 .ssh/id_rsa와 .ssh/id_rsa.pub 파일이 생성되었음을 확인한다.

    #.  대상 노드의 id_rsa.pub을 원본 노드 Linux 사용자 계정의 홈 디렉터리/.ssh 디렉터리 이하에 authorized_keys라는 파일명으로 복사한다.

    #.  authorized_keys 파일의 속성을 640, .ssh 디렉터리의 속성을 700으로 수정한다. (chmod 640 authorized_keys; cd ..; chmod 700 .ssh)

    #.  테스트를 통해 암호 요청 없이 파일이 복사되는지 확인한다. (scp test.txt cubrid_usr@:/home/cubrid_usr/.)
    
    
**ha_make_slavedb.sh** 스크립트를 각 단계의 순서대로 실행하는 도중 오류가 발생하거나 n을 입력하여 실행을 중단한 이후에 재실행할 경우, 그 이전까지 실행에 성공한 단계에 대해서는 s를 입력하여 다음 단계로 넘어가도 된다.
   
1.  At this step, enter the password of a Linux account and password of **DBA**, the CUBRID database account, for HA rebuilding replication. Enter y to the question.
 
    ::

        ##### step 1 ###################################################################
        #
        # get HA/replica user password and DBA password
        #
        #  * warning !!!
        #   - Because ha_make_slavedb.sh uses expect (ssh, scp) to control HA/replica node,
        #     the script has to know these passwords.
        #
        ################################################################################
         
           continue ? ([y]es / [n]o / [s]kip) : y

    Enter the password of a Linux account of the HA node and the password of **DBA**, the CUBRID database account. If you have not changed the password of **DBA** after installing CUBRID, press the <Enter> key without entering the password of **DBA**. 

2.  At this step, check whether the environment variables of the slave node are correct. Enter y to the question. 

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

3.  At this step, copy the HA-related scripts of the slave node to the master node. Enter y to the question. Then the password will be asked for if you did not set the public key with ssh-keygen when you sent a file with **scp** command. 

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
         
4.  At this step, copy the HA-related scripts to the replica node. In this scenario, if there is no replica node, skip this step and go to the next step by entering. 

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
         
        There is no replication server to copy scripts to.

5.  At this step, check whether the environment variables of all nodes are correct. Enter y to the question. 

    [번역] expect 명령이 설치(root 계정에서 yum install expect)되어 있지 않으면 이 단계에서 오류가 발생할 수 있다.
    
    ::

        ##### step 5 ###################################################################
        #
        #  check environment of all ha nodes
        #
        #  * details
        #   - test $CUBRID == /home1/cubrid_usr/CUBRID
        #   - test $CUBRID_DATABASES == /home1/cubrid_usr/CUBRID/database
        #   - test -d /home1/cubrid_usr/CUBRID/database/testdb
        #
        ################################################################################
         
           continue ? ([y]es / [n]o / [s]kip) : y

6.  At this step, stop replication of the master node. Enter y to the question. 

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

7.  At this step, delete the old replication log from the slave node and initialize the HA meta information table of the master node. Enter y to the question. 

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

8.  At this step, initialize the HA meta information table of replica node. In this scenario, if there is no replica node, skip this step and go to the next step by entering s. 

    ::

        ##### step 8 ###################################################################
        #
        #  remove old copy log of slave and init db_ha_apply_info on replications
        #
        #  * details
        #   - remove old copy log of slave
        #   - init db_ha_apply_info on replications
        #
        ################################################################################
         
           continue ? ([y]es / [n]o / [s]kip) : y
         
        There is no replication server to init ha_info

9.  At this step, create a backup volume from the master node (**target_host**) for HA replication rebuilding. You can skip this step and go to the next step by entering s if there is an existing backup volume. There are some constraints for rebuilding replication by using the existing backup volume, which are as follows:

    *   The archive logs, including the transaction being executed during backup, must be in the master node (**target_host**); this means that a backup volume created long ago cannot be used.

    *   The backup status information file must be created by using the **-o** option during backup. At this time, the path must be identical to the path of the backup volume file. The file name must be `db_name`\ **.bkup.output** format. If the file name is not identical with the format, change the file name according to the format before executing the script.

    *   The path of the existing backup volume and the status information file must be specified in the **backup_dest_path** parameter in the script. In other words, specify the absolute path of the directory containing the backup volume on the master node (**target_host**) to this parameter.

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

10. At this step, copy the database backup of the master node to the slave node. Enter y to the question. 

    ::

        ##### step 10 ###################################################################
        #
        #  copy testdb databases backup to current host
        #
        #  * details
        #   - scp databases.txt from target host if there's no testdb info on current host
        #   - remove old database and replication log if exists
        #   - make new database volume and replication path
        #   - scp  database backup to current host
        #
        ################################################################################
         
           continue ? ([y]es / [n]o / [s]kip) : y

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

15. At this step, the result of building the slave node is printed to check whether it was successful or failed.

    ::

        ##### step 15 ##################################################################
        #
        #  completed
        #
        ################################################################################
        .

[번역]  **ha_make_slavedb.sh** 스크립트의 수행이 완료되면 다음 작업을 수행한다.
        
*   슬레이브를 새로 구축하는 경우 다음과 같이 마스터와 슬레이브의 **ha_node_list**\에 *nodeB*\를 추가한다.

    *   마스터와 슬레이브의 **cubrid_ha.conf**\에 "ha_node_list=cubrid\@nodeA:nodeB"를 설정한다.
    *   마스터에서 **cubrid heartbeat reload** 명령을 실행하여 **ha_node_list** 정보를 다시 읽는다.
    *   다음과 같이 마스터의 보관 로그 개수인 **log_max_archives** 설정을 **cubrid.conf**\에 지정된 값으로 재설정한다.

        ::
        
            $ csql -u dba -c "SET SYSTEM PARAMETERS 'log_max_archives=0'" testdb@localhost
    
*   슬레이브 노드에서 HA 상태를 확인하고, **cubrid heartbeat start** 명령으로 HA를 구동한다. 

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
        
        CUBRID 10.0
        
        ++ cubrid server start: success
        @ copylogdb start
        ++ copylogdb start: success
        @ applylogdb start
        ++ applylogdb start: success
        ++ HA processes start: success
        ++ cubrid heartbeat start: success
        
        [nodeB]$ cubrid heartbeat status
        @ cubrid heartbeat status
        
        HA-Node Info (current nodeB, state slave)
        Node nodeB (priority 2, state slave)
        Node nodeA (priority 1, state master)
        
        HA-Process Info (master 26611, state slave)
        Applylogdb testdb@localhost:/home/cubrid_usr/CUBRID/databases/testdb_nodeA (pid 26831, state registered)
        Copylogdb testdb@nodeA:/home/cubrid_usr/CUBRID/databases/testdb_nodeA (pid 26829, state registered)
        Server testdb (pid 26617, state registered_and_standby)

:ref:`ha-cubrid-broker-conf`\ 를 참고하여 브로커를 설정하고, **cubrid broker restart** 명령으로 브로커를 구동한다.

슬레이브에서 레플리카 구축
^^^^^^^^^^^^^^^^^^^^^^^^^^

다음은 복제 재구축 원본 노드를 슬레이브 노드로 하여, 슬레이브 노드에서 레플리카 노드를 새로 구축하거나 재구축하는 예이다.
새로 구축하는 경우 Master:Slave:Replica는 1:1:0에서 1:1:1이 된다.

.. image:: /images/s2r.png

*   마스터 노드 호스트 명: *nodeA*
*   슬레이브 노드 호스트 명: *nodeB*
*   레플리카 노드 호스트 명: *nodeC*

복제 재구축은 시스템이 운영 중인 상태에서도 수행할 수 있으나, 복제 지연을 최소화하기 위해 시간 당 트랜잭션 수가 적을 때 수행하는 것을 권장한다.

**ha_make_slavedb.sh** 스크립트를 실행하기 전에 다음을 수행한다.

#.  *nodeC*\ 의 HA 서비스를 종료한다. *nodeC*\ 를 새로 구축한다면 이 과정은 불필요하다.

    ::

        [nodeC]$ cubrid heartbeat stop
        [nodeC]$ cubrid service stop

#.  *nodeC*\를 새로 구축한다면 :ref:`ha-configuration`\을 참고하여 *nodeA*, *nodeB*\, *nodeC*\의 **cubrid.conf**, **cubrid_ha.conf**, **cubrid_broker.conf**, **databases.txt**\를 설정한다.

    *   *nodeC*\ 의 **cubrid.conf**\에 설정
    
        ::
        
            ha_mode=replica
            
    *   모든 노드의 **cubrid_ha.conf**\에 설정

        ::
        
            ha_node_list=cubrid@nodeA:nodeB
            ha_replica_list=cubrid@nodeC
            
    *   모든 노드에서 **databases.txt**\의 **db-host** 설정
    
        ::
        
            nodeA:nodeB:nodeC
            
        하지만 **databases.txt**\의 **db-host** 부분은 브로커에서 DB 서버에 접속하는 순서와 관련된 설정 파일이므로 원하는대로 변경이 가능하다.

    .. 스크립트 수정 전까지는 아래 내용 적용 안 됨.
    
        #.  **cubrid_ha.conf**\의 :ref:`ha_copy_log_max_archives <ha_copy_log_max_archives>` 값을 변경한다.

            *nodeC*\ 의 복제 재구축이 진행될 때 원본 노드인 *nodeB*\ 의 복제 로그(*nodeA*\ 로부터 복제된 로그. 보통 복제된 로그가 *nodeB* DB에 반영될 때 사용됨)가 사용되므로, 복제 재구축이 끝날 때까지 *nodeB*\ 의 복제 로그가 삭제되지 않을 정도로 **ha_copy_log_max_archives** 값이 변경되어야 한다. 이 값을 10이라고 가정하고 아래와 같이 설정한다.

            *   *nodeB*\ 의 **cubrid_ha.conf**\에 설정
            
                ::
                    
                    ha_copy_log_max_archives=10
            
            *   *nodeB*\ 에서 다음을 수행하여 **ha_copy_log_max_archives**\의 변경된 설정 값을 적용
            
                ::
                    
                    [nodeB]$ cubrid heartbeat applylogdb stop testdb nodeA
                    [nodeB]$ cubrid heartbeat applylogdb start testdb nodeA

#.  *nodeB*\에 DB의 재구동 없이 **log_max_archives**\의 변경된 값을 적용한다.

    레플리카 구축 시간 동안 수행되는 트랜잭션을 보관할 수 있는 보관 로그(archive logs) 개수를 10이라고 가정한다. "SET SYSTEM PARAMETERS" 문을 사용하여 "10"으로 설정하며, **cubrid.conf**\의 **log_max_archives**\는 *nodeC*\ 의 구축/재구축 이후 기존 설정을 사용할 것이므로 변경하지 않는다.
    
    ::
    
        [nodeB]$ csql -u dba -c "SET SYSTEM PARAMETERS 'force_remove_log_archives=no'" testdb@localhost
        [nodeB]$ csql -u dba -c "SET SYSTEM PARAMETERS 'log_max_archives=10'" testdb@localhost
            
#.  *nodeC*\ 의 **ha_make_slavedb.sh** 스크립트를 설정해야 한다. **target_host**\에는 복사할 원본, 즉 *nodeB*\ 를 설정하고, **repl_log_home**\에는 복제 로그의 홈 디렉터리(기본값: **$CUBRID_DATABASES**)를 설정한다. 

    ::
         
        [nodeC]$ cd $CUBRID/share/scripts/ha
        [nodeC]$ vi ha_make_slavedb.sh
        target_host=nodeB

#.  설정 후에는 *nodeC*\ 에서 **ha_make_slavedb.sh** 스크립트를 실행한다 

    ::

        [nodeC]$ cd $CUBRID/share/scripts/ha
        [nodeC]$ ./ha_make_slavedb.sh

.. note::   스크립트를 수행할 때 scp 명령을 사용하게 되는데, scp 수행 시 암호 요청 과정을 생략하려면 scp의 개인키를 재구축하려는 레플리카 노드(*nodeC*)에, 공개키를 복제 원본에 해당하는 슬레이브 노드(*nodeB*)에 설정하면 된다. 이와 관련하여 :ref:`scp 설정하기 <scp-setting>`\ 를 참고한다.

**ha_make_slavedb.sh** 스크립트를 각 단계의 순서대로 실행하는 도중 오류가 발생하거나 n을 입력하여 실행을 중단한 이후에 재실행할 경우, 그 이전까지 실행에 성공한 단계에 대해서는 s를 입력하여 다음 단계로 넘어가도 된다.

1.  HA 복제 재구축을 위한 Linux 계정의 암호와, CUBRID DB 계정인 **dba** 의 암호를 입력하는 단계이다. 질문에 y를 입력한다. 

    ::

        ##### step 1 ###################################################################
        #
        # get HA/replica user password and DBA password
        #
        #  * warning !!!
        #   - Because ha_make_slavedb.sh uses expect (ssh, scp) to control HA/replica node,
        #     the script has to know these passwords.
        #
        ################################################################################
         
           continue ? ([y]es / [n]o / [s]kip) : y
   
    HA 노드의 Linux 계정 암호와, CUBRID DB 계정인 **dba** 의 암호를 입력한다. 처음 CUBRID 설치 후 **dba** 암호를 변경하지 않았을 경우, **dba** 암호의 입력 없이 <Enter> 키를 누르면 된다. 
   
    ::
    
        HA/replica cubrid_usr's password :
        HA/replica cubrid_usr's password :
         
        testdb's DBA password :
        Retype testdb's DBA password :

2.  레플리카 노드의 환경 변수 설정이 올바른지 확인하는 단계이다. 질문에 y를 입력한다. 

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
        #   - replica_hosts     : nocdC
        #
        #   - current_host      : nodeC
        #   - current_state     : replica
        #
        #   - target_host       : nodeB
        #   - target_state      : slave
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

3.  레플리카 노드의 HA 관련 스크립트들을 마스터 노드에 복사하는 단계이다. 질문에 y를 입력한다. 이후 모든 단계에서도 마스터 노드에 접속 시 암호를 요구하게 되며, scp 명령을 이용하여 필요한 파일을 전송할 때에도 암호를 요구한다. 

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

4.  레플리카 노드의 HA 관련 스크립트들을 슬레이브 노드에 복사하는 단계이다.

    ::

        ##### step 4 #####################################
        #
        #  copy scripts to slave node
        #
        #  * details
        #   - scp scripts to '~/.ha' on nodeB().
        #
        ##################################################
         
           continue ? ([y]es / [n]o / [s]kip) : y
         
5.  모든 노드의 환경 변수 설정이 올바른지 확인하는 단계이다. 질문에 y를 입력한다. 

    expect 명령이 설치(root 계정에서 yum install expect)되어 있지 않으면 이 단계에서 오류가 발생할 수 있다.

    ::

        ##### step 5 ###################################################################
        #
        #  check environment of all ha nodes
        #
        #  * details
        #   - test $CUBRID == /home1/cubrid_usr/CUBRID
        #   - test $CUBRID_DATABASES == /home1/cubrid_usr/CUBRID/database
        #   - test -d /home1/cubrid_usr/CUBRID/database/testdb
        #
        ################################################################################
         
           continue ? ([y]es / [n]o / [s]kip) : y
           
6.  HA 복제 재구축을 위해 슬레이브 노드(**target_host**)로부터 백업 볼륨을 생성하는 단계로서, 기존에 이미 생성한 백업 볼륨이 있다면 s를 입력하여 다음 단계로 넘어갈 수 있다. 기존 백업 볼륨을 사용하여 복제 재구축을 수행하기 위해서는 다음과 같은 제약 조건이 있다.
   
    *   백업 시작 이후 생성된 보관 로그(archive logs), 원본 DB에 미반영된 복제 로그와 복제 재구축 이후 진행 중인 트랜잭션에 대한 복제 로그가 반드시 슬레이브 노드(**target_host**) 내에 존재해야 한다(즉, 오래 전에 생성한 백업 볼륨은 사용할 수 없음).

    *   백업 시 -o 옵션을 사용하여 백업 상태 정보 파일을 남겨야 한다. 이 때 저장되는 경로는 백업 볼륨 파일의 경로와 같아야 한다. 파일 이름은 `db_name`\ **.bkup.output** 형식이어야 하며, 파일 이름이 다른 경우 스크립트 수행 전에 형식에 맞게 변경한다.

    *   기존 백업 볼륨 및 상태 정보 파일의 경로는 스크립트 내의 **backup_dest_path** 파라미터에 지정해야 한다. 즉, 슬레이브 노드(**target_host**)에 존재하는 백업 볼륨이 포함된 디렉터리의 절대 경로를 이 파라미터에 지정하면 된다.

    ::
    
        ##### step 6 ###################################################################
        #
        #  online backup database  on slave
        #
        #  * details
        #   - run 'cubrid backupdb -C -D ... -o ... testdb@localhost' on slave
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

7.  슬레이브 노드의 데이터베이스 백업본을 레플리카 노드에 복사하는 단계이다. 질문에 y를 입력한다. 
           
    ::       
           
        ##### step 7 ###################################################################
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

       
8.  복사한 데이터베이스 백업본을 레플리카 노드에 복구하는 단계이다. 질문에 y를 입력한다. 

    ::

        ##### step 8 ###################################################################
        #
        #  restore database testdb on current host
        #
        #  * details
        #   - cubrid restoredb -B ... testdb current host
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

9.  레플리카 노드의 HA 메타 정보 테이블 값을 설정하는 단계이다. 질문에 y를 입력한다. 

    ::

        ##### step 9 ###################################################################
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
   

10. 슬레이브에 있는 복제 로그를 초기화하고, 슬레이브 노드의 저장 로그를 레플리카 노드에 복사하는 단계이다. 질문에 y를 입력한다. 

    ::
       
        ##### step 10 ###################################################################
        #
        #  make initial replication active log on slave, and copy archive logs from
        #  slave
        #
        #  * details
        #   - remove old replication log on slave if exists
        #   - start copylogdb to make replication active log
        #   - copy archive logs from slave
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

11. 레플리카 노드 구축이 정상적으로 완료되었는지 여부를 출력하는 단계이다. 

    ::

        ##### step 11 ##################################################################
        #
        #  completed
        #
        ################################################################################

**ha_make_slavedb.sh** 스크립트의 수행이 완료되면 다음 작업을 수행한다.


..  아래 내용은 복제 재구축 스크립트 변경 전까진 불필요.
        
    *   *nodeC*\ 에서 예상되는 복제 지연 정도에 따라 *nodeA*\ 와 *nodeB*\ 에서 **cubrid_ha.conf**\의 **ha_copy_log_max_archives** 값을 변경한 후, 다음을 수행하여 변경된 설정 값을 적용한다.

        ::
            
            [nodeA]$ cubrid heartbeat applylogdb stop testdb nodeB
            [nodeA]$ cubrid heartbeat applylogdb start testdb nodeB
            
        ::
        
            [nodeB]$ cubrid heartbeat applylogdb stop testdb nodeA
            [nodeB]$ cubrid heartbeat applylogdb start testdb nodeA

*   레플리카 노드를 새로 구축하는 경우 *nodeA*\ 와 *nodeB*\ 에서 변경된 **cubrid_ha.conf**\의 **ha_node_list, ha_replica_list**\를 적용한다.

    ::
    
        [nodeA]$ cubrid heartbeat reload
        
    ::
    
        [nodeB]$ cubrid heartbeat reload
    
*   슬레이브 노드에서 **log_max_archives** 설정을 **cubrid.conf**\에 지정된 값으로 재설정한다.

    ::
    
        $ csql -u dba -c "SET SYSTEM PARAMETERS 'log_max_archives=0'" testdb@localhost

*   레플리카 노드에서 HA 상태를 확인하고, **cubrid heartbeat start** 명령으로 HA를 구동한다. 

    ::
    
        [nodeC]$ cubrid hb status
        @ cubrid heartbeat status

         HA-Node Info (current nodeC, state unknown)


         HA-Process Info (master 5214, state unknown)

        [nodeC]$ cubrid hb start
        @ cubrid heartbeat start
        @ cubrid master start
        ++ cubrid master is running.
        @ HA processes start
        @ cubrid server start: testdb

        This may take a long time depending on the amount of recovery works to do.

        CUBRID 10.0

        ++ cubrid server start: success
        @ copylogdb start
        ++ copylogdb start: success
        @ applylogdb start
        ++ applylogdb start: success
        ++ HA processes start: success
        ++ cubrid heartbeat start: success
        
        [nodeC]$ cubrid hb status
        @ cubrid heartbeat status

         HA-Node Info (current nodeC, state replica)
           Node nodeC (priority 32767, state replica)
           Node nodeB (priority 2, state slave)
           Node nodeA (priority 1, state master)


         HA-Process Info (master 5214, state slave)
           Applylogdb testdb@localhost:/home1/cubrid_usr/CUBRID/databases/testdb_nodeB (pid 5816, state registered)
           Applylogdb testdb@localhost:/home1/cubrid_usr/CUBRID/databases/testdb_nodeA (pid 5814, state registered)
           Copylogdb testdb@test-dhlee002.ncl:/home1/cubrid_usr/CUBRID/databases/testdb_nodeB (pid 5812, state registered)
           Copylogdb testdb@dhtest001.ncl:/home1/cubrid_usr/CUBRID/databases/testdb_nodeA (pid 5810, state registered)
           Server testdb (pid 5590, state registered_and_standby)

*   :ref:`ha-cubrid-broker-conf`\ 를 참고하여 브로커를 설정하고, **cubrid broker restart** 명령으로 브로커를 구동한다.

레플리카에서 레플리카 구축
^^^^^^^^^^^^^^^^^^^^^^^^^^

다음은 복제 재구축 원본 노드를 레플리카 노드로 하여, 레플리카 노드로부터 레플리카 노드를 새로 구축하거나 재구축하는 예이다.
새로 구축하는 경우 Master:Slave:Replica는 1:1:1에서 1:1:2가 된다.

.. image:: /images/r2r.png

*   마스터 노드 호스트 명: *nodeA*
*   슬레이브 노드 호스트 명: *nodeB*
*   레플리카 노드 호스트 명: *nodeC*
*   레플리카 노드 호스트 명: *nodeD*

복제 재구축은 마스터 노드가 운영 중인 상태에서도 수행할 수 있으나, 복제 지연을 최소화하기 위해 시간 당 트랜잭션 수가 적을 때 수행하는 것을 권장한다.

**ha_make_slavedb.sh** 스크립트를 실행하기 전에 다음을 수행한다.

#.  *nodeD*\ 의 HA 서비스를 종료한다. *nodeD*\ 를 새로 구축한다면 이 과정은 불필요하다. 

    ::

        [nodeD]$ cubrid heartbeat stop
        [nodeD]$ cubrid service stop

#.  *nodeD*\ 를 새로 구축한다면 :ref:`ha-configuration`\ 을 참고하여 마스터, 슬레이브, 복제 원본 레플리카, 복제 대상 레플리카의 **cubrid.conf, cubrid_ha.conf, cubrid_broker.conf, databases.txt**\를 설정한다.

    *   *nodeC*, *nodeD*\의 **cubrid.conf**\에 설정
    
        ::
        
            ha_mode=replica
            
    *   모든 노드의 **cubrid_ha.conf**\에 설정
    
        ::
        
            ha_node_list=cubrid@nodeA:nodeB
            ha_replica_list=cubrid@nodeC:nodeD
            
    *   모든 노드에서 **databases.txt**\의 **db-host** 설정
    
        ::
        
            nodeA:nodeB:nodeC:nodeD
            
        하지만 **databases.txt**\의 **db-host** 부분은 브로커에서 DB 서버에 접속하는 순서와 관련된 설정 파일이므로 원하는대로 변경이 가능하다.

        
        
    ..  아래 내용은 복제 재구축 스크립트 변경 전까진 불필요.
            
        #.  **cubrid_ha.conf**\의 :ref:`ha_copy_log_max_archives <ha_copy_log_max_archives>` 값을 변경한다.

            *   *nodeD*\ 의 복제 재구축이 진행될 때 원본 노드인 *nodeC*\ 의 복제 로그(*nodeA*\ 로부터 복제된 로그. 보통 복제된 로그가 *nodeC* DB에 반영될 때 사용됨)가 사용되므로, 복제 재구축이 끝날 때까지 *nodeC*\의 복제 로그가 삭제되지 않을 정도로 **ha_copy_log_max_archives** 값이 변경되어야 한다. 이 값을 10이라고 가정하고 아래와 같이 설정한다.

            *   *nodeC*\의 **cubrid_ha.conf**\에 설정
            
                ::
                
                    ha_copy_log_max_archives=10        
            
            *   *nodeC*\에서 다음을 수행하여 ha_copy_log_max_archives의 변경된 설정 값을 적용
            
                ::
                
                    [nodeC]$ cubrid heartbeat applylogdb stop testdb nodeA
                    [nodeC]$ cubrid heartbeat applylogdb start testdb nodeA

#.  *nodeC*\에 DB의 재구동 없이 **log_max_archives**\의 변경된 값을 적용한다.

    레플리카 구축 시간 동안 수행되는 트랜잭션을 보관할 수 있는 보관 로그(archive logs) 개수를 10이라고 가정한다. "SET SYSTEM PARAMETERS" 문을 사용하여 "10"으로 설정하며, **cubrid.conf**\의 **log_max_archives**\는 *nodeD*\의 구축/재구축 이후 기존 설정을 사용할 것이므로 변경하지 않는다.
    
    ::
    
        [nodeC]$ csql -u dba -c "SET SYSTEM PARAMETERS 'force_remove_log_archives=no'" testdb@localhost
        [nodeC]$ csql -u dba -c "SET SYSTEM PARAMETERS 'log_max_archives=10'" testdb@localhost

#.  *nodeD*\의 **ha_make_slavedb.sh** 스크립트를 설정해야 한다. **target_host**\ 에는 복사할 원본, 즉 *nodeC*\ 를 설정하고, **repl_log_home**\에는 복제 로그의 홈 디렉터리(기본값: **$CUBRID_DATABASES**)를 설정한다. 

    ::
         
        [nodeD]$ cd $CUBRID/share/scripts/ha
        [nodeD]$ vi ha_make_slavedb.sh
        target_host=nodeC
        
#.  설정 후에는 *nodeD*\ 에서 **ha_make_slavedb.sh** 스크립트를 실행한다 

    ::

        [nodeD]$ cd $CUBRID/share/scripts/ha
        [nodeD]$ ./ha_make_slavedb.sh
        
.. note::   스크립트를 수행할 때 scp 명령을 사용하게 되는데, scp 수행 시 암호 요청 과정을 생략하려면 scp의 개인키를 재구축하려는 레플리카 노드(*nodeD*)에, 공개키를 복제 원본 레플리카 노드(*nodeC*)에 설정하면 된다. 이와 관련하여 :ref:`scp 설정하기 <scp-setting>`\ 를 참고한다.

**ha_make_slavedb.sh** 스크립트를 각 단계의 순서대로 실행하는 도중 오류가 발생하거나 n을 입력하여 실행을 중단한 이후에 재실행할 경우, 그 이전까지 실행에 성공한 단계에 대해서는 s를 입력하여 다음 단계로 넘어가도 된다.

1.  HA 복제 재구축을 위한 Linux 계정의 암호와, CUBRID DB 계정인 **dba** 의 암호를 입력하는 단계이다. 질문에 y를 입력한다. 

    ::

        ##### step 1 ###################################################################
        #
        # get HA/replica user password and DBA password
        #
        #  * warning !!!
        #   - Because ha_make_slavedb.sh uses expect (ssh, scp) to control HA/replica node,
        #     the script has to know these passwords.
        #
        ################################################################################
         
           continue ? ([y]es / [n]o / [s]kip) : y
   
    HA 노드의 Linux 계정 암호와, CUBRID DB 계정인 **dba** 의 암호를 입력한다. 처음 CUBRID 설치 후 **dba** 암호를 변경하지 않았을 경우, **dba** 암호의 입력 없이 <Enter> 키를 누르면 된다. 
   
    ::
    
        HA/replica cubrid_usr's password :
        HA/replica cubrid_usr's password :
         
        testdb's DBA password :
        Retype testdb's DBA password :

2.  레플리카 노드의 환경 변수 설정이 올바른지 확인하는 단계이다. 질문에 y를 입력한다. 

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
        #   - replica_hosts     : nodeC nodeD
        #
        #   - current_host      : nodeD
        #   - current_state     : replica
        #
        #   - target_host       : nodeC
        #   - target_state      : replica
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

3.  대상 레플리카 노드의 HA 관련 스크립트들을 마스터 노드에 복사하는 단계이다. 질문에 y를 입력한다. 이후 모든 단계에서도 마스터 노드에 접속 시 암호를 요구하게 되며, scp 명령을 이용하여 필요한 파일을 전송할 때에도 암호를 요구한다. 

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

4.  대상 레플리카 노드의 HA 관련 스크립트들을 슬레이브 노드에 복사하는 단계이다.

    ::

        ##### step 4 #####################################
        #
        #  copy scripts to slave node
        #
        #  * details
        #   - scp scripts to '~/.ha' on nodeB().
        #
        ##################################################
         
           continue ? ([y]es / [n]o / [s]kip) : y

5.  대상 레플리카 노드의 HA 관련 스크립트들을 원본 레플리카 노드에 복사하는 단계이다.

    ::
    
        ##### step 5 ###################################################################
        #
        #  copy scripts to target node
        #
        #  * details
        #   - scp scripts to '~/.ha' on nodeC(replica).
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

           
6.  모든 노드의 환경 변수 설정이 올바른지 확인하는 단계이다. 질문에 y를 입력한다.

    expect 명령이 설치(root 계정에서 yum install expect)되어 있지 않으면 이 단계에서 오류가 발생할 수 있다.

    ::

        ##### step 6 ###################################################################
        #
        #  check environment of all ha nodes
        #
        #  * details
        #   - test $CUBRID == /home1/cubrid_usr/CUBRID
        #   - test $CUBRID_DATABASES == /home1/cubrid_usr/CUBRID/database
        #   - test -d /home1/cubrid_usr/CUBRID/database/testdb
        #
        ################################################################################
         
           continue ? ([y]es / [n]o / [s]kip) : y
   
7.  HA 복제 재구축을 위해 원본 노드(**target_host**)로부터 백업 볼륨을 생성하는 단계로서, 기존에 이미 생성한 백업 볼륨이 있다면 s를 입력하여 다음 단계로 넘어갈 수 있다. 기존 백업 볼륨을 사용하여 복제 재구축을 수행하기 위해서는 다음과 같은 제약 조건이 있다.
   
    *   백업 시작 이후 생성된 보관 로그(archive logs), 원본 DB에 미반영된 복제 로그와 복제 재구축 이후 진행 중인 트랜잭션에 대한 복제 로그가 반드시 원본 레플리카 노드(**target_host**) 내에 존재해야 한다(즉, 오래 전에 생성한 백업 볼륨은 사용할 수 없음).

    *   백업 시 -o 옵션을 사용하여 백업 상태 정보 파일을 남겨야 한다. 이 때 저장되는 경로는 백업 볼륨 파일의 경로와 같아야 한다. 파일 이름은 `db_name`\ **.bkup.output** 형식이어야 하며, 파일 이름이 다른 경우 스크립트 수행 전에 형식에 맞게 변경한다.

    *   기존 백업 볼륨 및 상태 정보 파일의 경로는 스크립트 내의 **backup_dest_path** 파라미터에 지정해야 한다. 즉, 원본 노드(**target_host**)에 존재하는 백업 볼륨이 포함된 디렉터리의 절대 경로를 이 파라미터에 지정하면 된다.
    
    ::
    
        ##### step 7 ###################################################################
        #
        #  online backup database  on replica
        #
        #  * details
        #   - run 'cubrid backupdb -C -D ... -o ... testdb@localhost' on replica
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

8.  원본 레플리카 노드의 데이터베이스 백업본을 대상 레플리카 노드에 복사하는 단계이다. 질문에 y를 입력한다. 
           
    ::       
           
        ##### step 8 ###################################################################
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

       
9.  복사한 데이터베이스 백업본을 대상 레플리카 노드에 복구하는 단계이다. 질문에 y를 입력한다. 

    ::

        ##### step 9 ###################################################################
        #
        #  restore database testdb on current host
        #
        #  * details
        #   - cubrid restoredb -B ... testdb current host
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

10. 복제 원본 레플리카 노드에서 수행 중인 copylogdb/applylogdb 프로세스가 있으면 종료시키는 단계이다. 질문에 y를 입력한다.

    ::
           
        ##### step 10 ###################################################################
        #
        #  suspend copylogdb/applylogdb on target server if running
        #
        #  * details
        #   - deregister copylogdb/applylogdb on nodeC(replica).
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

11. 원본 레플리카 노드에 있는 복제 로그를 초기화하고, 원본 레플리카 노드의 저장 로그를 레플리카 노드에 복사하는 단계이다. 질문에 y를 입력한다. 

    ::
       
        ##### step 11 ###################################################################
        #
        #  make initial replication active log on target, and copy archive logs from
        #  target
        #
        #  * details
        #   - remove old replication log on target if exists
        #   - start copylogdb to make replication active log
        #   - copy archive logs from target
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

12. 복제 원본 레플리카 노드의 copylogdb/applylogdb 프로세스를 재구동한다.

    ::

        ##### step 12 ###################################################################
        #
        #  restart copylogdb/applylogdb on target
        #
        #  * details
        #   - restart copylogdb/applylogdb
        #
        ################################################################################

13. 레플리카 노드 구축이 정상적으로 완료되었는지 여부를 출력하는 단계이다. 

    ::

        ##### step 13 ##################################################################
        #
        #  completed
        #
        ################################################################################

**ha_make_slavedb.sh** 스크립트의 수행이 완료되면 다음 작업을 수행한다.

..  아래 내용은 복제 재구축 스크립트 변경 전까진 불필요.

    
    *   *nodeC*, *nodeD*\에서 예상되는 복제 지연 정도에 따라 모든 노드에서 **cubrid_ha.conf**\의 **ha_copy_log_max_archives** 값을 변경한 후, 다음을 수행하여 변경된 설정 값을 적용한다.

        ::
            
            [nodeA]$ cubrid heartbeat applylogdb stop testdb nodeB
            [nodeA]$ cubrid heartbeat applylogdb start testdb nodeB
            
        ::
        
            [nodeB]$ cubrid heartbeat applylogdb stop testdb nodeA
            [nodeB]$ cubrid heartbeat applylogdb start testdb nodeA

            
        ::
        
            [nodeC]$ cubrid heartbeat applylogdb stop testdb nodeA
            [nodeC]$ cubrid heartbeat applylogdb start testdb nodeA
            [nodeC]$ cubrid heartbeat applylogdb stop testdb nodeB
            [nodeC]$ cubrid heartbeat applylogdb start testdb nodeB        
        
*   *nodeD*\ 를 새로 구축하는 경우 *nodeA*, *nodeB*, *nodeC*\ 에서 변경된 **cubrid_ha.conf**\의 **ha_node_list, ha_replica_list**\를 적용한다.

    ::
    
        [nodeA]$ cubrid heartbeat reload
        
    ::
    
        [nodeB]$ cubrid heartbeat reload

    ::
    
        [nodeC]$ cubrid heartbeat reload
        
*   *nodeC*\에서 **log_max_archives** 설정을 **cubrid.conf**\에 지정된 값으로 재설정한다.

    ::
    
        [nodeC]$ csql -u dba -c "SET SYSTEM PARAMETERS 'log_max_archives=0'" testdb@localhost

*   *nodeD*\에서 HA 상태를 확인하고, **cubrid heartbeat start** 명령으로 HA를 구동한다. 



    ::
    
        [nodeD]$ cubrid hb status
        @ cubrid heartbeat status

         HA-Node Info (current nodeD, state unknown)


         HA-Process Info (master 5214, state unknown)

        [nodeD]$ cubrid hb start
        @ cubrid heartbeat start
        @ cubrid master start
        ++ cubrid master is running.
        @ HA processes start
        @ cubrid server start: testdb

        This may take a long time depending on the amount of recovery works to do.

        CUBRID 10.0

        ++ cubrid server start: success
        @ copylogdb start
        ++ copylogdb start: success
        @ applylogdb start
        ++ applylogdb start: success
        ++ HA processes start: success
        ++ cubrid heartbeat start: success
        
        [nodeD]$ cubrid hb status
        @ cubrid heartbeat status

         HA-Node Info (current nodeD, state replica)
           Node nodeD (priority 32767, state replica)
           Node nodeC (priority 32767, state replica)
           Node nodeB (priority 2, state slave)
           Node nodeA (priority 1, state master)


         HA-Process Info (master 5214, state slave)
           Applylogdb testdb@localhost:/home1/cubrid_usr/CUBRID/databases/testdb_nodeB (pid 5816, state registered)
           Applylogdb testdb@localhost:/home1/cubrid_usr/CUBRID/databases/testdb_nodeA (pid 5814, state registered)
           Copylogdb testdb@test-dhlee002.ncl:/home1/cubrid_usr/CUBRID/databases/testdb_nodeB (pid 5812, state registered)
           Copylogdb testdb@dhtest001.ncl:/home1/cubrid_usr/CUBRID/databases/testdb_nodeA (pid 5810, state registered)
           Server testdb (pid 5590, state registered_and_standby)

*   :ref:`ha-cubrid-broker-conf`\ 를 참고하여 브로커를 설정하고, "cubrid broker restart" 명령으로 브로커를 구동한다.

*   CUBRID 서비스를 구동한다.

    ::
    
        [nodeD]$ cubrid service start

레플리카에서 슬레이브 재구축
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

다음은 복제 재구축 원본 노드를 레플리카 노드로 하여, 레플리카 노드에서 슬레이브 노드를 재구축하는 예이다. **ha_make_slavedb.sh** 스크립트를 사용하는 경우, 재구축 이후에 슬레이브 노드는 하나만 존재해야 한다.

.. image:: /images/r2s.png

*   마스터 노드 호스트 명: *nodeA*
*   슬레이브 노드 호스트 명: *nodeB*
*   레플리카 노드 호스트 명: *nodeC*
*   레플리카 노드 호스트 명: *nodeD*

복제 재구축은 시스템이 운영 중인 상태에서도 수행할 수 있으나, 복제 지연을 최소화하기 위해 시간 당 트랜잭션 수가 적을 때 수행하는 것을 권장한다.

**ha_make_slavedb.sh** 스크립트를 실행하기 전에 다음을 수행한다.

#.  *nodeB*\ 의 HA 서비스를 종료한다. *nodeB*\ 를 기존의 장비를 제거하고 새로운 장비에서 구축한다면 이 과정은 불필요하다.

    ::

        [nodeB]$ cubrid heartbeat stop
        [nodeB]$ cubrid service stop
        
#.  *nodeB*\를 새로운 장비에서 재구축한다면 :ref:`ha-configuration`\을 참고하여 *nodeB*\의 **cubrid.conf**, **cubrid_ha.conf**, **cubrid_broker.conf**, **databases.txt**\를 설정해야 한다. 이때 *nodeA*, *nodeC*, *nodeD*\에는 *nodeB*\가 이미 설정되어 있다고 가정한다.

    *   *nodeB*\ 의 **cubrid.conf**\에 설정
    
        ::
        
            ha_mode=on
            
    *   모든 노드의 **cubrid_ha.conf**\에 설정

        ::
        
            ha_node_list=cubrid@nodeA:nodeB
            ha_replica_list=cubrid@nodeC:nodeD
            
    *   모든 노드에서 **databases.txt**\의 **db-host** 설정
    
        ::
        
            nodeA:nodeB:nodeC:nodeD
            
        하지만 **databases.txt**\의 **db-host** 부분은 브로커에서 DB 서버에 접속하는 순서와 관련된 설정 파일이므로 원하는대로 변경이 가능하다.

    ..  아래 내용은 복제 재구축 스크립트 변경 전까진 불필요.
    
        #.  **cubrid_ha.conf**\의 :ref:`ha_copy_log_max_archives <ha_copy_log_max_archives>` 값을 변경한다.

            *nodeB*\ 의 복제 재구축이 진행될 때 원본 노드인 *nodeC*\ 의 복제 로그(*nodeA*\ 로부터 복제된 로그. 보통 복제된 로그가 *nodeC* DB에 반영될 때 사용됨)가 사용되므로, 복제 재구축이 끝날 때까지 *nodeC*\ 의 복제 로그가 삭제되지 않을 정도로 **ha_copy_log_max_archives** 값이 변경되어야 한다. 이 값을 10이라고 가정하고 아래와 같이 설정한다.

            *   *nodeC*\ 의 **cubrid_ha.conf**\에 설정
            
                ::
                    
                    ha_copy_log_max_archives=10
            
            *   *nodeC*\ 에서 다음을 수행하여 **ha_copy_log_max_archives**\의 변경된 설정 값을 적용
            
                ::
                    
                    [nodeC]$ cubrid heartbeat applylogdb stop testdb nodeA
                    [nodeC]$ cubrid heartbeat applylogdb start testdb nodeA

#.  *nodeC*\에 DB의 재구동 없이 **log_max_archives**\의 변경된 값을 적용한다.

    레플리카 구축 시간 동안 수행되는 트랜잭션을 보관할 수 있는 보관 로그(archive logs) 개수를 10이라고 가정한다. "SET SYSTEM PARAMETERS" 문을 사용하여 "10"으로 설정하며, **cubrid.conf**\의 **log_max_archives**\는 *nodeB*\ 의 재구축 이후 기존 설정을 사용할 것이므로 변경하지 않는다.
    
    ::
    
        [nodeC]$ csql -u dba -c "SET SYSTEM PARAMETERS 'force_remove_log_archives=no'" testdb@localhost
        [nodeC]$ csql -u dba -c "SET SYSTEM PARAMETERS 'log_max_archives=10'" testdb@localhost

#.  *nodeB*\ 의 **ha_make_slavedb.sh** 스크립트를 설정해야 한다. **target_host**\에는 복사할 원본, 즉 *nodeB*\ 를 설정하고, **repl_log_home**\에는 복제 로그의 홈 디렉터리(기본값: **$CUBRID_DATABASES**)를 설정한다. 

    ::
         
        [nodeB]$ cd $CUBRID/share/scripts/ha
        [nodeB]$ vi ha_make_slavedb.sh
        target_host=nodeC

#.  설정 후에는 *nodeB*\ 에서 **ha_make_slavedb.sh** 스크립트를 실행한다 

    ::

        [nodeB]$ cd $CUBRID/share/scripts/ha
        [nodeB]$ ./ha_make_slavedb.sh
        
.. note::   스크립트를 수행할 때 scp 명령을 사용하게 되는데, scp 수행 시 암호 요청 과정을 생략하려면 scp의 개인키를 재구축하려는 슬레이브 노드(*nodeB*)에, 공개키를 복제 원본에 해당하는 레플리카 노드(*nodeC*)에 설정하면 된다. 이와 관련하여 :ref:`scp 설정하기 <scp-setting>`\ 를 참고한다.

**ha_make_slavedb.sh** 스크립트를 각 단계의 순서대로 실행하는 도중 오류가 발생하거나 n을 입력하여 실행을 중단한 이후에 재실행할 경우, 그 이전까지 실행에 성공한 단계에 대해서는 s를 입력하여 다음 단계로 넘어가도 된다.


1.  HA 복제 재구축을 위한 Linux 계정의 암호와, CUBRID DB 계정인 **dba** 의 암호를 입력하는 단계이다. 질문에 y를 입력한다. 

    ::

        ##### step 1 ###################################################################
        #
        # get HA/replica user password and DBA password
        #
        #  * warning !!!
        #   - Because ha_make_slavedb.sh uses expect (ssh, scp) to control HA/replica node,
        #     the script has to know these passwords.
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

    HA 노드의 Linux 계정 암호와, CUBRID DB 계정인 **dba** 의 암호를 입력한다. 처음 CUBRID 설치 후 **dba** 암호를 변경하지 않았을 경우, **dba** 암호의 입력 없이 <Enter> 키를 누르면 된다. 

    
2.  슬레이브 노드의 환경 변수 설정이 올바른지 확인하는 단계이다. 질문에 y를 입력한다. 

    ::    

        ##### step 2 ###################################################################
        #
        #  ha_make_slavedb.sh is the script for making slave database more easily
        #
        #  * environment
        #   - db_name           : testdb
        #
        #   - master_host       : dhtest001.ncl
        #   - slave_host        : test-dhlee002.ncl
        #   - replica_hosts     : test-dhlee004.ncl test-dhlee005.ncl
        #
        #   - current_host      : test-dhlee002.ncl
        #   - current_state     : slave
        #
        #   - target_host       : test-dhlee004.ncl
        #   - target_state      : replica
        #
        #   - repl_log_home     : /home1/cubrid/CUBRID/databases
        #   - backup_dest_path  : /home1/cubrid/.ha/backup
        #   - backup_option     :
        #   - restore_option    :
        #
        #  * warning !!!
        #   - environment on slave must be same as master
        #   - database and replication log on slave will be deleted
        #
        ################################################################################

            continue ? ([y]es / [n]o / [s]kip) : y

3.  슬레이브 노드의 HA 관련 스크립트들을 마스터 노드에 복사하는 단계이다. 질문에 y를 입력한다. 이후 scp 명령을 이용하여 필요한 파일을 전송할 때 ssh-keygen으로 공개 키를 설정하지 않았으면 암호를 요구한다. 

    ::

        ##### step 3 ###################################################################
        #
        #  copy scripts to master node
        #
        #  * details
        #   - scp scripts to '~/.ha' on dhtest001.ncl(master).
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

4.  HA 관련 스크립트들을 레플리카 노드에 복사하는 단계이다. 질문에 y를 입력한다. 

    ::   


        ##### step 4 ###################################################################
        #
        #  copy scripts to replication node
        #
        #  * details
        #   - scp scripts to '~/.ha' on replication node.
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y


5.  모든 노드의 환경 변수 설정이 올바른지 확인하는 단계이다. 질문에 y를 입력한다. 

    expect 명령이 설치(root 계정에서 yum install expect)되어 있지 않으면 이 단계에서 오류가 발생할 수 있다.

    ::

        ##### step 5 ###################################################################
        #
        #  check environment of all ha nodes
        #
        #  * details
        #   - test $CUBRID == /home1/cubrid/CUBRID
        #   - test $CUBRID_DATABASES == /home1/cubrid/CUBRID/databases
        #   - test -d /home1/cubrid/CUBRID/databases
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

6.  마스터 노드의 복제 진행을 멈추도록 하는 단계이다. 질문에 y를 입력한다. 

    ::

        ##### step 6 ###################################################################
        #
        #  suspend copylogdb/applylogdb on master if running
        #
        #  * details
        #   - deregister copylogdb/applylogdb on dhtest001.ncl(master).
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

7.  이전에 슬레이브 노드에 존재했던 오래된 복제 로그를 삭제하고 마스터 노드의 HA 메타 정보 테이블을 초기화하는 단계이다. 질문에 y를 입력한다. 

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

8.  레플리카 노드의 HA 메타 정보 테이블을 초기화하는 단계이다. 이 시나리오에서는 레플리카 노드가 없으므로 질문에 s를 입력하여 넘어가도 된다. 

    ::
    
        ##### step 8 ###################################################################
        #
        #  remove old copy log of slave and init db_ha_apply_info on replications
        #
        #  * details
        #   - remove old copy log of slave
        #   - init db_ha_apply_info on replications
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

9.  HA 복제 재구축을 위해 레플리카 노드(**target_host**)로부터 백업 볼륨을 생성하는 단계로서, 기존에 이미 생성한 백업 볼륨이 있다면 s를 입력하여 다음 단계로 넘어갈 수 있다. 기존 백업 볼륨을 사용하여 복제 재구축을 수행하기 위해서는 다음과 같은 제약 조건이 있다.
   
    *   백업 시 실행 중인 트랜잭션을 포함하는 보관 로그(archive logs)가 반드시 레플리카  노드(**target_host**) 내에 존재해야 한다(즉, 오래 전에 생성한 백업 볼륨은 사용할 수 없음).

    *   백업 시 -o 옵션을 사용하여 백업 상태 정보 파일을 남겨야 한다. 이 때 저장되는 경로는 백업 볼륨 파일의 경로와 같아야 한다. 파일 이름은 `db_name`\ **.bkup.output** 형식이어야 하며, 파일 이름이 다른 경우 스크립트 수행 전에 형식에 맞게 변경한다.

    *   기존 백업 볼륨 및 상태 정보 파일의 경로는 스크립트 내의 **backup_dest_path** 파라미터에 지정해야 한다. 즉, 레플리카 노드(**target_host**)에 존재하는 백업 볼륨이 포함된 디렉터리의 절대 경로를 이 파라미터에 지정하면 된다.

    ::
   
        ##### step 9 ###################################################################
        #
        #  online backup database  on replica
        #
        #  * details
        #   - run 'cubrid backupdb -C -D ... -o ... testdb@localhost' on replica
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

10. 레플리카 노드의 데이터베이스 백업본을 슬레이브 노드에 복사하는 단계이다. 질문에 y를 입력한다. 

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

11. 복사한 데이터베이스 백업본을 슬레이브 노드에 복구하는 단계이다. 질문에 y를 입력한다. 

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

12. 마스터에 있는 복제 로그를 초기화하고, 마스터 노드의 저장 로그를 슬레이브 노드에 복사하는 단계이다. 질문에 y를 입력한다. 

    ::

        ##### step 12 ###################################################################
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

13. 마스터 노드의 copylogdb, applylogdb 프로세스를 재시작하는 단계이다. 질문에 y를 입력한다. 

    ::

        ##### step 13 ###################################################################
        #
        #  restart copylogdb/applylogdb on master
        #
        #  * details
        #   - restart copylogdb/applylogdb
        #
        ################################################################################

           continue ? ([y]es / [n]o / [s]kip) : y

14. 슬레이브 노드 구축이 정상적으로 완료되었는지 여부를 출력하는 단계이다. 

    ::
   
        ##### step 14 ##################################################################
        #
        #  completed
        #
        ################################################################################

**ha_make_slavedb.sh** 스크립트의 수행이 완료되면 다음 작업을 수행한다.
        
*   슬레이브 노드에서 HA 상태를 확인하고, **cubrid heartbeat start** 명령으로 HA를 구동한다. 

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

        CUBRID 10.0

        ++ cubrid server start: success
        @ copylogdb start
        ++ copylogdb start: success
        @ applylogdb start
        ++ applylogdb start: success
        ++ HA processes start: success
        ++ cubrid heartbeat start: success
        
        [NodeB]$ cubrid hb status
        @ cubrid heartbeat status

         HA-Node Info (current test-dhlee002.ncl, state slave)
           Node test-dhlee005.ncl (priority 32767, state replica)
           Node test-dhlee004.ncl (priority 32767, state replica)
           Node test-dhlee002.ncl (priority 2, state slave)
           Node dhtest001.ncl (priority 1, state master)


         HA-Process Info (master 27221, state slave)
           Applylogdb testdb@localhost:/home1/cubrid/CUBRID/databases/testdb_nodeA (pid 27455, state registered)
           Copylogdb testdb@nodeA:/home1/cubrid/CUBRID/databases/testdb_nodeA (pid 27453, state registered)
           Server testdb (pid 27228, state registered_and_standby)
    
:ref:`ha-cubrid-broker-conf`\ 를 참고하여 브로커를 설정하고, **cubrid broker restart** 명령으로 브로커를 구동한다.

