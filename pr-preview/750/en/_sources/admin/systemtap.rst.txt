
:meta-keywords: install systemtap, systemtap marker, systemtap probe, systemtap event, systemtap script, connection markers, query markers, object operation markers, index operation markers, locking markers, transaction markers, I/O markers
:meta-description: SystemTap is a tool that can be used to dynamically monitor and track the process of running, to find and diagnose performance bottlenecks; learn how to use CUBRID markers in SystemTap scripts.

*********
SystemTap
*********

Overview
========

SystemTap is a tool that can be used to dynamically monitor and track the process of running. CUBRID supports SystemTap; therefore, it is possible to find the cause of a performance bottleneck.

The basic idea of ​​SystemTap script is that you can specify the name of the event and grant a handler there. Handler is script statements that specify the action to be performed each time an event occurs.

To monitor the performance of CUBRID using SystemTap, you need to install SystemTap. After installing SystemTap, you can write and run a SystemTap script which is like a C language. With this script, you can monitor the performance of the System.

SystemTap supports only on Linux.

See http://sourceware.org/systemtap/index.html for further information and installation.

Installing SystemTap
====================

Checking Installation
---------------------

1.  Check if you have group accounts, stapusr and stapdev in /etc/group file.
    If they don't exist, SystemTap may not be installed.

2.  Add CUBRID user account when you install CUBRID into stapusr and stapdev group accounts. Here let's assume the CUBRID user account as "cubrid".

    ::
    
        $ vi /etc/group
        
        stapusr:x:156:cubrid
        stapdev:x:158:cubrid

3.  To check if SystemTap is runnable, simply run the below command.

    ::

        $ stap -ve 'probe begin { log("hello world") exit() }'

Version
-------

To execute SystemTap scripts in CUBRID, you should use SystemTap 2.2 or higher.

The below is an example to install SystemTap in CentOS 6.3. Checking version and installing SystemTap can be different among Linux distributors. 

1.  Check the current version of the installed SystemTap.

    ::

        $ sudo yum list|grep  systemtap
        systemtap.x86_64                       1.7-5.el6_3.1                 @update
        systemtap-client.x86_64                1.7-5.el6_3.1                 @update
        systemtap-devel.x86_64                 1.7-5.el6_3.1                 @update
        systemtap-runtime.x86_64               1.7-5.el6_3.1                 @update
        systemtap-grapher.x86_64               1.7-5.el6_3.1                 update
        systemtap-initscript.x86_64            1.7-5.el6_3.1                 update
        systemtap-sdt-devel.i686               1.7-5.el6_3.1                 update
        systemtap-sdt-devel.x86_64             1.7-5.el6_3.1                 update
        systemtap-server.x86_64                1.7-5.el6_3.1                 update
        systemtap-testsuite.x86_64             1.7-5.el6_3.1                 update

2.  If the lower version of SystemTap than 2.2 version is installed, remove it.

    ::

        $ sudo yum remove systemtap-runtime
        $ sudo yum remove systemtap-devel
        $ sudo yum remove systemtap

3.  Install the RPM distributed package of SystemTap 2.2 or higher. You can find the RPM distributed package in http://rpmfind.net/linux/rpm2html/.

    ::

        $ sudo rpm -ivh systemtap-devel-2.3-3.el6.x86_64.rpm
        $ sudo rpm -ivh systemtap-runtime-2.3-3.el6.x86_64.rpm
        $ sudo rpm -ivh systemtap-client-2.3-3.el6.x86_64.rpm
        $ sudo rpm -ivh systemtap-2.3-3.el6.x86_64.rpm

Related Terms
=============

.. https://sourceware.org/systemtap/wiki/UsingMarkers

Marker
------

A marker placed in code provides a hook to call a function (probe) that you can provide at runtime. The function you provide is called each time the marker is executed, in the execution context of the caller. When the function provided ends its execution, it returns to the caller.

The function a user provides, the probe can be used for tracing and performance accounting.

Probe
-----

Probe is a kind of function to define the behavior when a certain event occurs; it is separated into an event and a handler.

SystemTap script defines a specific event, that is, the behavior when a marker occurs.

SystemTap script is able to have several probes; a handler of the probe is called as probe body.

SystemTap script accepts the insertion of the probing code without recompiling codes and gives more flexibility for handlers. Events are executed as triggers as handlers can be run. Handler can write data and be specified as the action to print out the data.

For CUBRID's markers, see :ref:`cubrid-marker`.

.. https://access.redhat.com/site/documentation/en-US/Red_Hat_Enterprise_Linux/5/html-single/SystemTap_Beginners_Guide/#systemtapscript-events

Asynchronous Events
-------------------

Asynchronous events are defined internally; it is not dependent on special jobs or locations on the code. These kinds of probing events are mainly counter and timer, etc.

Examples of asynchronous events are as follows.

*   begin
    
    The start of SystemTap session. 
    
    e.g. The moment the SystemTap starts.
    
    
*   end

    The end of SystemTap.
    
*   timer events

    The event specifying the handler works periodically
    
    e.g. The below prints "hello world" per 5 seconds.
    
    ::
    
        probe timer.s(5)
        {
          printf("hello world\n")
        }

Using SystemTap in CUBRID
=========================

Building CUBRID source
----------------------

SystemTap can be used only on Linux.

To use SystemTap by building CUBRID source, **ENABLE_SYSTEMTAP** is **ON** which is set by default.

This option is already included in the release build, a user not building the CUBRID source but installing CUBRID with the installation package can also use SystemTap script.

The below is an example of building the CUBRID source.

::

    build.sh -m release

Running SystemTap script
------------------------

Examples of SystemTap scripts in CUBRID are located in $CUBRID/share/systemtap directory.

The below is an example of running buffer_access.stp file.

::

    cd $CUBRID/share/systemtap/tapset/scripts
    stap -k buffer_access.stp -o result.txt

Printing Results
----------------

When you run a certain script, it displays the requested result to the console by the script code. With "-o *filename*" option, it writes the requested result to the *filename*.

The below is the result of the above example.

::

    Page buffer hit count: 172
    Page buffer miss count: 172
    Miss ratio: 50

.. _cubrid-marker:

CUBRID markers
==============

A very useful feature of SystemTap is the possibility of placing markers in the user source code (CUBRID code) and writing probes that triggers when these markers are reached. Below is the list of CUBRID markers and their meaning.

Connection markers
------------------

We might be interested in gathering information helpful for an analysis related to connection activity (number of connections, duration of individual connections, average duration of a connection, maximum number of connections achieved etc.) during a period of time. In order for such monitoring scripts to be written, we must provide at least two helpful markers: connection-start and connection-end.

.. function:: conn_start(connection_id, user)

    This marker is triggered when the query execution process on the server has begun.

    :param connection_id: an integer containing the connection ID.
    :param user: The username used by this connection.
    
.. function:: conn_end(connection_id, user)

    This marker is triggered when the query execution process on the server has ended.
    
    :param connection_id: an integer containing the connection ID.
    :param user: The username used by this connection.

Query markers
-------------

Markers for query execution related events can prove very useful in monitor tasks, although they do not contain global information related to the entire system. At least two markers are essential: those corresponding to the start of the execution of a query and the end of the execution.

.. function:: query_exec_start(query_string, query_id, connection_id, user)

    This marker is triggered after the query execution has begun on the server.

    :param query_string: string representing the query to be executed
    :param query_id: Query identifier
    :param connection_id: Connection ID
    :param user: The username used by this connection
    
.. function:: query_exec_end(query_string, query_id, connection_id, user, status)

    This marker is triggered when the query execution process on the server has ended.
    
    :param query_string: string representing the query to be executed
    :param query_id: Query identifier
    :param connection_id: Connection ID
    :param user: The username used by this connection
    :param status: The status returned by the query execution (Success, Error)

Object operation markers
------------------------

Operations involving the storage engine are critical and probing updates in a table at object level can greatly help monitor database activity. Markers will be triggered for each object inserted/updated/deleted, which may bring performance drawbacks on both the monitoring scripts and the server.

.. function:: obj_insert_start(table)

    This marker is triggered before an object is inserted.

    :param table: Target table of the operation
    
.. function:: obj_insert_end(table, status)

    This marker is triggered after an object has been inserted.
    
    :param table: Target table of the operation
    :param status: Value showing whether the operation ended with success or not
    
.. function:: obj_update_start(table)

    This marker is triggered before an object is updated.
    
    :param table: Target table of the operation

.. function:: obj_update_end(table, status)

    This marker is triggered after an object has been updated
    
    :param table: Target table of the operation
    :param status: Value showing whether the operation ended with success or not
    
.. function:: obj_deleted_start(table)

    This marker is triggered before an object is deleted.

    :param table: Target table of the operation

.. function:: obj_delete_end(table, status)

    This marker is triggered after an object has been deleted.
    
    :param table: Target table of the operation
    :param status: Value showing whether the operation ended with success or not
    
Index operation markers
-----------------------

The object operation markers presented above are table-related, but below are index-related markers. 

Indexes and their misuse can be the cause of many problems in a system and the possibility of monitoring them can be very helpful. The proposed markers are similar to those used for tables, since indexes support the same operations.

.. function:: idx_insert_start(classname, index_name) 

    This marker is triggered before an insertion in the B-Tree.

    :param classname: Name of the class having the target index
    :param index_name: Target index of the operation
    
.. function:: idx_insert_end(classname, index_name, status)

    This marker is triggered after an insertion in the B-Tree.

    :param classname: Name of the class having the target index
    :param index_name: Target index_name of the operation
    :param status: Value showing whether the operation ended with success or not
    
.. function:: idx_update_start(classname, index_name)

    This marker is triggered before an update in the B-Tree.

    :param classname: Name of the class having the target index
    :param index_name: Target index_name of the operation
    
.. function:: idx_update_end(classname, index_name, status)

    This marker is triggered after an update in the B-Tree.
    
    :param classname: Name of the class having the target index
    :param index_name: Target index_name of the operation
    :param status: Value showing whether the operation ended with success or not
    
.. function:: idx_delete_start(classname, index_name)

    This marker is triggered before a deletion in the B-Tree.

    :param classname: Name of the class having the target index
    :param index_name: Target index_name of the operation
    
.. function:: idx_delete_end(classname, index_name, status)

    This marker is triggered after a deletion in the B-Tree.

    :param classname: Name of the class having the target index
    :param index_name: Target index_name of the operation
    :param status: Value showing whether the operation ended with success or not
    
Locking markers
---------------

Markers that involve locking events are perhaps the most important for global monitoring tasks. The locking system has a deep impact on the server performance and a comprehensive analysis on lock waiting times and count, number of deadlocks and aborted transactions is very useful in finding problems.

.. function:: lock_acquire_start(OID, table, type)

    This marker is triggered before a lock is requested.

    :param OID: Target object of the lock request.
    :param table: Table holding the object
    :param type: Lock type (X_LOCK, S_LOCK etc.)
    
.. function:: lock_acquire_end(OID, table, type, status)

    This marker is triggered after a lock request has been completed.

    :param OID: Target object of the lock request.
    :param table: Table holding the object
    :param type: Lock type (X_LOCK, S_LOCK etc.)
    :param status: Value showing whether the request has been granted or not.
    
.. function:: lock_release_start(OID, table, type)

    This marker is triggered before a lock is released.

    :param OID: Target object of the lock request.
    :param table: Table holding the object
    :param type: Lock type (X_LOCK, S_LOCK etc.)
    
.. function:: lock_release_end(OID, table, type)

    This marker is triggered after a lock release operation has been completed.

    :param OID: Target object of the lock request
    :param table: Table holding the object
    :param type: Lock type(X_LOCK, S_LOCK etc)
    
Transaction markers
-------------------

Another interesting measure in server monitoring is transaction activity. A simple example: the number of transactions aborted is closely related to the number of deadlocks occurred, a very important performance indicator. Another straightforward use of such markers is the availability of a simple method of gathering system performance statistics such as TPS by using a simple SystemTap script.

.. function:: tran_commit(tran_id)

    This marker is triggered after a transaction completes successfully.

    :param tran_id: Transaction identifier.
    
.. function:: tran_abort(tran_id, status)

    This marker is triggered after a transaction has been aborted.

    :param tran_id: Transaction identifier.
    :param status: Exit status.

.. function:: tran_start(tran_id)

    This marker is triggered after a transaction is started.

    :param tran_id: Transaction identifier.
    
.. function:: tran_deadlock()

    This marker is triggered when a deadlock has been detected.

I/O markers
-----------

I/O access is the main bottleneck of a RDBMS and we should provide markers that allow the monitoring of I/O performance. The markers should be placed in a manner that will make it possible for user scripts to measure I/O page access time and aggregate various and complex statistics based on this measure.

.. function:: pgbuf_hit() 

    This marker is triggered when a requested page was found in the page buffer and there is no need to retrieve it from disk.
    
.. function:: pgbuf_miss()

    This marker is triggered when a requested page was not found in the page buffer and it must be retrieved from disk.

.. function:: io_write_start (query_id)

    This marker is triggered when a the process of writing a page onto disk has begun.

    :param query_id: Query identifier

.. function:: io_write_end(query_id, size, status)

    This marker is triggered when a the process of writing a page onto disk has ended.

    :param query_id: Query identifier
    :param size: number of bytes written
    :param status: Value showing whether the operation ended successfully or not

.. function:: io_read_start(query_id)

    This marker is triggered when a the process of reading a page from disk has begun.

    :param query_id: Query identifier
    
.. function:: io_read_end (query_id, size, status)

    This marker is triggered when a the process of reading a page from disk has ended.

    :param query_id: Query identifier
    :param size: number of bytes read
    :param status: Value showing whether the operation ended successfully or not

Other markers
-------------

.. function:: sort_start ()

    This marker is triggered when a sort operation is started.
    
.. function:: sort_end (nr_rows, status)

    This marker is triggered when a sort operation has been completed.

    :param nr_rows: number of rows sorted
    :param status: Value showing whether the operation ended successfully or not
