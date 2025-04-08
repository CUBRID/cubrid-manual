:tocdepth: 3

******************
11.4 Release Notes
******************

.. contents::

.. _11_4_information:

Release Notes Information
=========================

This document contains information about CUBRID 11.4 (Build Number 11.4.0.0000).
CUBRID 11.4 includes bug fixes and feature improvements found in CUBRID 11.3, as well as all bug fixes and feature enhancements applied in previous versions.

Information about CUBRID 11.3 can be found at https://www.cubrid.org/manual/ko/11.3/release_note/index.html.

Information about CUBRID 11.2 can be found at https://www.cubrid.org/manual/ko/11.2/release_note/index.html.

Information about CUBRID 11.0 can be found at https://www.cubrid.org/manual/ko/11.0/release_note/index.html.

Information about CUBRID 10.2 can be found at https://www.cubrid.org/manual/ko/10.2/release_note/index.html.

Information about CUBRID 10.1 can be found at https://www.cubrid.org/manual/ko/10.1/release_note/index.html.

Information about CUBRID 10.0 can be found at https://www.cubrid.org/manual/ko/10.0/release_note/index.html.

Information about CUBRID 9.3 can be found at https://www.cubrid.org/manual/ko/9.3/release_note/index.html.

Overview
========

CUBRID 11.4 is the latest stable version that includes new features, significant changes, and improvements.

CUBRID 11.4 introduces:

1. **PL/CSQL support for Oracle compatibility**
2. **Addition of HASH JOIN for large-scale processing**
3. **Performance improvements through optimizer and index processing enhancements**
4. **Performance boost in data recovery through parallel processing**
5. **Performance enhancement by expanding result caching**
6. **Improved data dump performance**
7. **Addition of memory monitoring features**
8. **Enhanced access control functionality**
9. **Improved user convenience for backup and recovery operations**

These updates bring not only new features but also various performance improvements across multiple areas.

Driver Compatibility
--------------------

-  The JDBC and CCI drivers of CUBRID 11.4 are compatible with database servers from CUBRID versions 11.3, 11.2, 11.1, 11.0, 10.2, 10.1, 10.0, 9.3, 9.2, 9.1, and 2008 R4.4, R4.3, or R4.1.
-  It is recommended to upgrade the drivers.

.. _11_4_changes_add_feature:

New feature
-----------

SQL
~~~

HASH JOIN Support 
^^^^^^^^^^^^^^^^^

To use HASH JOIN, join hints must be specified
``/*+ USE_HASH */``  Enables consideration of HASH JOIN.
``/*+ NO_USE_HASH */`` Disables the use of HASH JOIN.

Added SQL Syntax for Changing Serial Owner
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Previously, changing the owner of a serial was only possible using the ``call change_serial_owner()`` method. Now, an SQL statement has been added for this functionality.

- ``AS-IS``

.. code:: sql

   CALL CHANGE_SERIAL_OWNER('test_serial', 'test_user1') on class db_serial;
  
.. code:: sql

   ALTER SERIAL test_serial OWNER TO test_user1;

#. Only the DBA and DBA group members can change the serial owner.
#. An error occurs if the user lacks the necessary permissions or if the specified serial does not exist.

Added SQL Syntax for Adding Users to a Group
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. In versions prior to 11.4, adding a user to a group required executing ``DROP USER`` followed by ``CREATE USER GROUPS``.
#. In an HA environment, method call results were not synchronized.
#. Separate synchronization operations are required for the master and slave nodes. Due to these issues, a new SQL statement has been introduced.

.. code:: 

   ALTER USER user_name 
   [PASSWORD password] |
   [ADD MEMBERS user_name {, user_name}...] |
   [DROP MEMBERS user_name {, user_name}...]
   [COMMENT 'comment_string']

#. DBA/DBA group members:
   -  Can remove users from the DBA/PUBLIC group.
   -  Can add members to the DBA group.

#. General users:
   -  Can only add or remove members from their own groups.

Optimizer
~~~~~~~~~

Added LEADING Hint to Specify Join Order
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The LEADING hint allows specifying a particular table or set of tables to be used as the prefix in the execution plan, enabling more flexible control over the table join order than the ORDERED hint.

.. code:: sql

   SELECT /*+ LEADING(e j) */ *
   FROM  employees e, departments d, job_history j
   WHERE e.department_id = d.department_id
   AND e.hire_date = j.start_date;

- Conditions where the hint is ignored:

-  If the specified tables cannot be joined first due to dependencies in the join graph.
-  If multiple LEADING hints are used, only the first one is applied.
-  If an ORDERED hint is present, all LEADING hints are ignored.

Added NDV (Number of Distinct Values) for Each Column in Statistics Table
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To create more accurate execution plans, the NDV (Number of Distinct Values) for each column has been added to the statistics table.

.. code:: sql

   ;info stats t123
   /* Display NDV information for each column  */
   Attribute: col3 (INTEGER)
      Number of Distinct Values: 1
   
   Attribute: col2 (INTEGER)
      Number of Distinct Values: 501
   
   Attribute: col1 (INTEGER)
      Number of Distinct Values: 10000

Added *fetch_time* (Disk Fetch Time) to **trace info**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The functionality has been enhanced to allow viewing the time spent on disk fetch operations within the total execution time.

.. code:: sql

   Trace Statistics:
   SELECT (
        time: 840,          // total execution time
        fetch: 44408,       // Number of fetches
        fetch_time: 64,     // Time spent on fetch operations
        ioread: 0           // Number of IO reads
   )
   SCAN (table: dba.t111), (
        heap time: 681,     // Heap processing time
        fetch: 44408,       // Number of fetches
        ioread: 0,          // Number of IO reads
        readrows: 1010000,  // Number of rows read
        rows: 1010000       // Total number of rows
   )

Improved Subquery Rewrite Method for Hidden Columns in ORDER BY Clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The issue of subqueries being redundantly rewritten due to the management of hidden column properties in the ORDER BY clause has been improved.

.. code:: sql

   UPDATE /*+ recompile */ t1
   SET c2 = 1
   WHERE c1 = (SELECT c1 FROM t2 ORDER BY c2, c3 LIMIT 1);

#. Now, only a single appropriate query rewrite occurs.
#. Redundant nested subqueries are no longer created.

This improvement ensures better query performance and eliminates unnecessary complexity in query execution.

Performance
~~~~~~~~~~~

Change REDO Recovery from Single-Threaded to Parallel Processing for Performance Improvement
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Currently, CUBRID’s REDO recovery runs in a single-threaded manner. However, since REDO logs applied to different data pages do not need to be synchronized, parallelizing this process can enhance performance.
#. If the proportion of REDO is high and the parallelism index is favorable, the improvement effect will be significant.

Performance Improvement Using Query Result Cache for Correlated Scalar Subqueries
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. A correlated subquery is executed individually for each row of the main query to retrieve values. If there are many duplicate input values, the same query may be executed repeatedly, leading to performance degradation.
#. To resolve this issue, a result cache is applied to eliminate unnecessary repeated executions.

Extend Query Result Cache Support for CTE
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Extend the existing result cache mechanism to apply caching to Common Table Expression (CTE) subqueries.
#. Improve performance by caching the results of CTE subqueries.

-  When the ``/*+ QUERY_CACHE */`` hint is applied to a CTE query, the caching mechanism is extended from applying a single result cache per query to also caching the results of CTE subqueries.

Extend Query Cache Support to Uncorrelated Subqueries
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Extend the result cache applied to CTE queries to also support uncorrelated subqueries.

Display Result Cache Reference in Trace Information
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Support displaying the reference count in trace information when executing queries that utilize the result cache.

.. code:: sql

   Trace Statistics:  
     SELECT (time: 23, fetch: 156, fetch_time: 0, ioread: 32)  
       SCAN (table: public.game), (heap time: 17, fetch: 124, ioread: 31, readrows: 8653, rows: 5676)  
       SCAN (hash temp(m), build time: 0, time: 2, fetch: 0, ioread: 0, readrows: 359, rows: 340)  
       UNION (time: 0, fetch: 8, fetch_time: 0, ioread: 0)  
         SELECT (time: 0, fetch: 0, fetch_time: 0, ioread: 0)  
           RESULT CACHE (reference count : 1)  
         SELECT (time: 0, fetch: 0, fetch_time: 0, ioread: 0)  
           RESULT CACHE (reference count : 1)

PL/CSQL, JAVA SP
~~~~~~~~~~~~~~~~

CUBRID offers the PL/CSQL feature to ensure compatibility with ORACLE’s PL/SQL. For syntax and usage details, please refer to the manual. (PL/CSQL)

Prevent Reloading of JNI-Loading Classes in Java SP Server by Adding a New ClassLoader (Add ``loadjava`` Option: *-j* or *--jni*)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. A new ClassLoader has been introduced to prevent the reloading of classes that load JNI in the Java SP server.
#. Additionally, the ``loadjava`` command now supports the *-j* or *--jni* option. 

Utility
~~~~~~~

Improve ``unloaddb`` Performance
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``unloaddb`` utility, previously operating in a single-threaded manner, has been improved to support multi-threaded execution, significantly reducing data export time.

#. ``unloaddb`` Options

   -  ``--thread-count``: Specifies the number of concurrent execution threads (0~127).
   -  ``enhanced-estimates``: Provides an accurate record count estimation (verbose mode only).

#. Performance monitoring
   - ``Elapsed``: Total execution time
   - ``Fetch``: Server fetch time/count
   - ``Write``: File write time
   - ``Add/Get L``: List operation wait time/count
   - ``Add/Get Q``: Queue operation wait time/count
   - ``to obj``: DB_VALUE conversion time
   - ``to str``: Plain text conversion time

Add Memory Monitoring Feature
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. The ``cubrid memmon`` utility outputs the heap memory usage allocated to the server process.
#. When the system parameter **enable_memory_monitoring** is set to *yes*, the server memory monitoring module tracks detailed memory allocation information based on total heap memory usage and the source code and line number where memory allocation occurred.
#. This allows you to check the server heap memory usage at the time the utility is executed.
#. For usage and detailed information, please refer to the manual.

Add ``info ndv`` CSQL Session Command to Print NDV (Number of Distinct Values) for Each Column of a Table
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A new CSQL session command has been added to print the Number of Distinct Values (NDV) for specific columns of a table.

#. Executing the ``info ndv <table_name>`` command will output the number of distinct values (NDV) for each column of the specified table.
#. Internally, the following query is executed to calculate the NDV,

   .. code:: sql

      SELECT /*+ SAMPLING_SCAN */
         COUNT(DISTINCT host_year), COUNT(DISTINCT event_code), COUNT(DISTINCT athlete_code),
         COUNT(DISTINCT stadium_code), COUNT(DISTINCT nation_code), COUNT(DISTINCT medal),
         COUNT(DISTINCT game_date),COUNT(*)
      FROM public.game;

#. Example of Execution

   .. code:: sql

      csql> ;info ndv public.game 

      Query : SELECT /*+ SAMPLING_SCAN */ count(distinct [host_year]), count(distinct [event_code]), count(distinct [athlete_code]), count(distinct [stadium_code]), count(distinct [nation_code]), count(distinct [medal]), count(distinct [game_date]), count(*) FROM [public.game] 

      Number of Distinct Values 
      **************** 
      Class name: public.game 
      host_year (5) 
      event_code (393) 
      athlete_code (6677) 
      stadium_code (116) 
      nation_code (115) 
      medal (3) 
      game_date (84) 

      total count : 8653 

      Committed.

Add Fence Key Information to ``SHOW INDEX CAPACITY`` and ``diagdb``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``SHOW ALL INDEXES CAPACITY OF [schema_name.]table_name;`` command has been updated to include the ``Num_fence_key`` field in the output.

Changes:
- The ``Num_fence_key`` is now displayed in the results of the ``SHOW ALL INDEXES CAPACITY`` command.
- Previously, the ``Num_fence_key`` value was included in the capacity calculation, but it will no longer be considered in the calculation.

This change provides more accurate index capacity information by excluding the fence key in the capacity calculation.

Add Volume Creation Time to ``diagdb`` Utility Output
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``diagdb`` utility has been updated to display the volume creation time. This includes the creation time information for the following volumes:
   -  Database volume
   -  Active log volume
   -  Archive log volume

Now, when running the ``diagdb`` utility, the creation time for each volume will be included in the output.

-  before

   .. code:: shell

      $ cat diag.txt | grep "creation time"
       Database creation time = Mon Aug 12 20:46:32 2024
       Database creation time = Mon Aug 12 20:46:32 2024
       Database creation time = Mon Aug 12 20:46:32 2024
       Database creation time = Mon Aug 12 20:46:32 2024

-  after

   .. code:: shell

      $ cat diag.txt | grep "creation time"
       Database creation time = Mon Aug 12 20:46:32 2024
       Volume creation time = Mon Aug 12 20:46:37 2024
       Database creation time = Mon Aug 12 20:46:32 2024
       Volume creation time = Mon Aug 12 20:46:37 2024
       Database creation time = Mon Aug 12 20:46:32 2024
       Volume creation time = Mon Aug 12 20:46:37 2024
       Database creation time = Mon Aug 12 20:46:32 2024
       Volume creation time = Mon Aug 12 20:46:37 2024

Automatically Restart ``cub_server`` Process in Single Server Environment on Abnormal Termination
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. When single-server setup (not using HA), if the ``cub_server`` process terminates abnormally, it previously required a manual restart. This has now been updated to support automatic restarts.
#. The functionality to automatically restart the ``cub_server`` process after an abnormal termination has been added.

.. important::

   #. The server will not automatically restart if it terminates normally. 
   #. The feature can be enabled or disabled via a system parameter. 
   #. If the server terminates repeatedly within a short period, it will stop attempting to restart after a certain number of failures.

Broker, CAS, CMS
~~~~~~~~~~~~~~~~

Add Parameter to Set the Size of Data Sent to Clients in Broker
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A new parameter, ``NET_BUF_SIZE``, has been introduced to the Broker to control the size of the data transmitted to clients.

Configurable Values: 
   - 16K(default)
   - 32K
   - 48K
   - 64K

Add Parameter to Set ACL for Each Broker
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A new parameter has been added to allow setting ACL for each broker, providing a way to allow clients to connect to the broker even when ACCESS_CONTROL is ON.

#. n the ``cubrid_broker.conf`` file, under the ``[broker]`` section, when ACCESS_CONTROL is ON, you can set the new ACL parameter ``ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER`` for each broker.
#. The parameter can have two possible values: ``DENY`` (default) or ``ALLOW``.

.. code:: shell

   $ cubrid broker acl status 
   ACCESS_CONTROL=ON 
   ACCESS_CONTROL_FILE=cubrid_acl.conf 

   [%query_editor] 
   ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER=DENY 
   testdb:dba:acl_ip_list.conf 

   CLIENT IP LAST ACCESS TIME 
   ========================================== 
   172.29.80.1 
   192.168.0.31 
   172.31.0.175 

   [%broker1] 
   ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER=ALLOW 

   ++ cubrid broker acl: success

-  The ``%query_editor`` broker is set to ``DENY``, allowing only specific IPs to connect.
-  The ``%broker1`` broker is set to ``ALLOW``, permitting connections from all IPs.

Change CMS(CUBRID Manager Server) SSL Profile to Support TLS v1.2 and Above
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

For security reasons, CUBRID Manager Server (**CMS**) and CUBRID Manager(**CM**)/CUBRID Admin(**CA**) communication has been updated to support **TLS v1.2** instead of the previous **TLS v1.0**. This change ensures enhanced security by enforcing the use of more secure encryption protocols.

Others
~~~~~~

Provide Script for Restoring a Backup with a New DB Name
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When data recovery is required, restoring a backup to an operational server typically involves tasks such as creating additional accounts and installing extra engines, which can be cumbersome. To simplify this process, a script has been provided to restore a backup and change the database name without additional setup.

- Usage: ``sh rename_to_newdb.sh [OPTION] ASIS_DBNAME TOBE_DBNAME``
- Options: 

  - ``-F``: Specify the absolute path of the directory where the new database will be created. 
  - ``-B``: Specify the absolute path of the directory containing the backup files (if not provided, it searches the current working directory). 
  - ``-d``: Restore the database to the state as of the specified date.
  - ``-l``: Specify the backup level to restore. 
  - ``-p``: Perform partial recovery if no log archive is available. 
  - ``-k``: Specify the path to the key file (_keys) for TDE recovery.

Add “0.0.0.0 your_hostname” to ``cubrid_host.conf`` File and Modify Error Messages
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To improve user convenience, the following changes have been made:

#. **Add the entry “0.0.0.0 your_hostname”** to the ``cubrid_host.conf`` file for easier configuration. This ensures the proper hostname binding.
#. **Modify error messages** for better clarity, providing more useful information to users when encountering issues.

These changes aim to simplify the setup process and enhance error handling for a better user experience.

Modify ``cubrid_host.conf`` to Be Case-Insensitive for User Hostnames
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``cubrid_host.conf`` file has been updated to treat hostnames as **case-insensitive**. This change ensures that the hostname lookup does not differentiate between uppercase and lowercase letters, making it more flexible and user-friendly.

Change the ``att_name`` Column Name to ``attr_name`` in ``db_serial``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Remove Unnecessary Fixed Page Headers for TDE during Sorting
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In the sorting process, the **unnecessary fixed page headers** for **Transparent Data Encryption (TDE)** have been removed. This change helps improve performance by eliminating redundant data that was previously included during the sorting phase

Remove Messages Written in Languages Other than English and Korean
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Messages written in languages other than **English** and **Korean** have been deleted. This ensures that only the relevant languages are included in the system, improving clarity and consistency.

Remove ``dont_reuse_oid`` Table Option from ``demodb_schema`` File
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``dont_reuse_oid`` table option has been removed from the ``demodb_schema`` file. This change simplifies the schema configuration by eliminating this option, which was previously used to prevent the reuse of object IDs.

.. _11_4_changes_spec:

Specification Changes
---------------------

SQL
~~~

Limit the maximum number of characters for the CHAR type to 2048 (2048 Korean characters) (Previous maximum: 268,435,456)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. The current maximum length of the CHAR type is 256MB, which is significantly larger compared to other DBMS (Oracle, MySQL, PostgreSQL). When using the UTF-8 character set, memory allocation can reach up to 1GB.
#. If there are two or more columns defined with the maximum CHAR type length, an **INSERT** statement can allocate more than 2GB of memory, leading to a memory overflow issue.

To resolve these memory allocation issues and improve server transmission efficiency, the specification for the CHAR type has been modified:

   - The maximum length of the CHAR type has been reduced from **256MB to 2048**.

Change LOB column value locators from absolute path to relative path
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If the **LOB file directory** location changes, all **LOB column locators** in the database must be updated.

**Modification Details:**

#.  Change the locator storage method from **absolute path** to **relative path**.
#.  Store locators in **relative path format** within LOB columns.
#.  When referencing LOB file contents, concatenate the **LOB Base Path** with the relative locator path. 

  - **LOB Base Path:** ``/cubrid/demo/lob``
  - **Locator:** ``ces_272/public.t1.00001720143587746537_3683`` 
  - **Referenced File:** ``(LOB Base Path) + locator`` 
      /cubrid/demo/lob/ces_272/public.t1.00001720143587746537_3683

Utility
~~~~~~~

Modify **CSQL** to recognize session commands when writing SQL or PL/CSQL statements (e.g., ``CREATE`` statements, body statements, etc.)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Additionally, fix the issue where **csql** remained unresponsive when encountering unmatched single quotes.

``"cubrid plandump -s"`` option allows deleting a specific plan. ``"cubrid plandump -d"`` behavior changed (Previous: output plan before deletion, Now: delete plan directly)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


**Enhancements in ``plandump`` Utility**

The ``plandump`` utility has been improved to always display the SQL execution plan and to allow specific plan cache deletion.

**New Features**

#. Improved ``plandump`` to always output the SQL execution plan when executed.
#. Added the ``-s`` option to enable the deletion of specific plan caches.

**Usage Examples**

#. **Displaying SQL Execution Plan**

   .. code:: shell

      $ cubrid plandump demodb 
      ... 
      Entries: XASL_ID = { sha1 = { 6290f2b1 44f088e5 d37d6f0f c8303155 9ef2096a }, time_stored = 1715925895 sec, 58124 usec } 
      ... 
      sql plan text = Sequential scan(public.game dba.game) 
      ...

#. **Deleting a Specific Plan Cache and Displaying Remaining Plans** 

   .. code:: shell

      $ cubrid plandump -s '6290f2b1 44f088e5 d37d6f0f c8303155 9ef2096a' demodb 
      ... 
      /* delete specific plan cache and display current plan caches. */ 
      ...

Simplified Lock Information Logged in ``DBName_latest.event`` File During Deadlock Occurrences
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Improved the server log file (``DBName_latest.event``) to log only the **lock information directly contributing to the deadlock** instead of all held locks.
#. Previously, all locks held by transactions were logged, making analysis difficult. Now, only **locks causing wait conditions** are recorded sequentially for better readability.
#. Clearly mark the **deadlock victim transaction** to facilitate root cause analysis.

Broker, CAS, CMS
~~~~~~~~~~~~~~~~

**Change SQL Log Writing Timing** – Modify to log SQL first, then execute prepare/execute, so that the SQL can be traced in the log even in case of a core dump during execution.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. When CAS crashes during ``prepare/execute``, the corresponding SQL statement was not logged, causing difficulty in identifying the problematic SQL.
#. Logs were recorded only after ``prepare/execute`` completion, making it hard to pinpoint the query that caused the issue.
#. By changing the order to log SQL **before** ``prepare/execute``, it allows tracing the executed SQL even if CAS crashes, thereby improving the ability to debug.

.. _11_4_changes_improvements:

Improvements
------------

SQL
~~~

Improvement of the issue where SELECT does not execute when X_LOCK occurs on the table (preventing unnecessary X_LOCK on rows)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The issue arises because locks are being created too early, causing unnecessary locks. The improvement ensures that unnecessary locks are released after all conditions are evaluated.

Modify the system to perform an index scan when a JavaSP function is used as a condition value in the ``WHERE`` clause with the ``LIKE`` operator
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: sql

   CREATE OR REPLACE FUNCTION stringTest(x String) RETURN String AS LANGUAGE JAVA NAME 'SpTest.typeteststring(java.lang.String) return java.lang.String';

   CREATE TABLE tbl (ord INT, col_int INT, col_char char(1));
   CREATE INDEX i_tbl ON tbl (ord);
   CREATE INDEX i_tbl_char ON tbl (col_char);

   INSERT INTO tbl VALUES (1,10,'a');
   INSERT INTO tbl VALUES (2,10,'b');
   INSERT INTO tbl VALUES (3,10,'c');
   INSERT INTO tbl VALUES (4,10,'d');
   INSERT INTO tbl VALUES (5,10,'e');

   SELECT count(*) AS "like" FROM tbl WHERE col_char LIKE (SELECT stringTest('a') FROM dual);

Improvement of the issue where the plan generator does not perform a **range scan** when a function index is used with ``<=`` and ``>=`` conditions, and instead generates a plan that scans the index from the beginning to the end
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modified to prevent the use of the ``FOR UPDATE`` clause on system tables and system view tables (such as \_db_class, db_root, dual, etc.)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modified to not check data type consistency when creating view
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Previously, the type compatibility of each column was checked immediately when creating a view.
#. Now, instead of checking the type at creation time, errors will be handled when the view is executed if type conversion is not possible.
#. The error handling was changed from generating an error during view creation to generating an error during view execution if type conversion is not possible.

Modified to allow the use of NULL values in the SELECT clause of ``CREATE VIEW``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: sql

   CREATE VIEW a_view( col1 ) AS select NULL as col1 from a_tbl;

Modified so that ``AUTO_INCREMENT`` and ``DEFAULT`` cannot coexist when changing table column properties using **ALTER TABLE MODIFY**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. When executing **CREATE TABLE** or **ALTER COLUMN** statements, an error occurs if the ``AUTO_INCREMENT`` and ``DEFAULT`` properties are used together.
#. Using the **ALTER TABLE MODIFY** statement to execute ``AUTO_INCREMENT`` and ``DEFAULT`` properties separately allows both properties to coexist on the same column. This issue caused a problem where tables with both ``AUTO_INCREMENT`` and ``DEFAULT`` properties could not be reloaded after unloading, resulting in the table not being created.

Modified to allow checking the creation time of DB volumes and logs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``SHOW VOLUME HEADER``, ``SHOW LOG HEADER``, and ``SHOW ARCHIVE LOG HEADER`` commands have been updated to display the volume creation time in the 'Creation_time' field
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Improvement of the length limitation for the WHERE clause when creating filtered indexes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. An issue occurred where the length limitation of the WHERE clause
when using a filtered index did not match the manual.
#. The problem arose from measuring the length of the rewritten string rather than the user-input string (with a 255-character limit).
#. The issue has been resolved by removing the length restriction.

Improvement to apply sort limit optimization when there are bind variables and expressions in the LIMIT clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: sql

   drop table if exists tbl1;
   create table tbl1 (col1 int, col2 int);
   insert into tbl1 select rownum, random() % 100000 +1 from db_class a, db_class b, db_class c, db_class d limit 100000;
   create index idx on tbl1(col1);

   prepare stmt from
   'SELECT a.col1, a.col2
   FROM tbl1 a
      LEFT JOIN tbl1 b ON a.col1 = b.col1
      LEFT JOIN tbl1 c ON a.col1 = c.col1
   ORDER BY a.col2,a.col1
   LIMIT ?*10,?';

   execute stmt using 10,10;

Optimizer
~~~~~~~~~

Fix the issue where the ``Total objects`` value in the table statistics is not updated when data is deleted
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When data is deleted, the object count in the statistics is not updated, which can cause inaccurate statistics to be used when generating the execution plan, potentially affecting performance. This issue has been improved.

Use the count of unique index keys for the ``Total objects`` value in the ``;info stats table_name`` command
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To reduce performance degradation caused by a full data scan when updating statistics, the count of unique index keys will now be used for the ``Total objects`` value.

Increase the number of sampling pages to improve the accuracy of B-tree statistics collection
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Increase the number of sampling pages during B-tree statistics collection to improve both accuracy and performance.
#. At the same time, remove unnecessary logic (such as the same page processing logic and the sampling page selection probability calculation routine) to improve performance.

Use NDV (Number of Distinct Values) extracted from the heap first to improve accuracy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. **Prioritize using NDV (Number of Distinct Values) from the Heap**

   -  **NDV (Number of Distinct Values)**: The count of distinct unique values in a specific column.
   - First, extract the NDV value from the heap and use it; if it cannot be obtained from the heap, fall back to using the NDV extracted from the B-tree.

#. **Exception Handling**

   -  If the length of string types (e.g., ``char``) exceeds 4000 characters, NDV will not be extracted from the heap. (This is to avoid performance or accuracy issues with long strings.)

#. **Change in Object Counting Method**

   -  Change the counting method to use sampling scan hints to calculate the object count.

By prioritizing NDV obtained from the heap, the selectivity is set more accurately, and performance and accuracy are improved by handling long strings as exceptions and adopting a sampling-based count method
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Minimize rule-based optimization (RBO) and heuristic factor elements to improve accuracy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. **Remove RBO-related elements**

   - Remove **Heuristic Factor** (empirical weights) 
   - Remove **statistics-based estimates** 
   - Remove **First Node priority processing routine** 
   - Remove heuristics from expressions 
   - Remove logic that treated the cost of unique indexes as 0
   - Remove IO count calculation logic based on selectivity, data buffers, and internal page count

#. **Completely remove RBO** 

   - Previously, RBO was used only when the cost difference was within 1.x. 
   - Now, RBO is fully removed, and focus is shifted entirely to **Cost-Based Optimization (CBO)**. 
   - Even when index scan is possible, allow the creation of a sequential scan (seq_scan) plan (RBO used to prioritize index scans).

#. **Other improvements**

   - Add a routine to check the plan cost during **partial search**. 
   - Fix the bug where **cumulative selectivity** of the next column was incorrectly used during **LIMIT** processing.

This improves the optimization logic by removing RBO and strengthening CBO.

Adjust some cost formulas based on actual execution time
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Reflecting the results from the **Join Order Benchmark**, we improve cost calculation formulas to match actual execution times and carry out several optimization enhancements and improvements.

#. **Cost formula adjustments**
   
   - Reflect **index filter scan selectivity**: Modify the data I/O cost calculation for index scans to consider selectivity. 
   - Add selectivity for the **'NOT LIKE'** operator. 
   - Add selectivity for **function-based indexes**. 
   - Increase **sampling page count** to 5000 to improve statistical accuracy. 
   - Adjust weights when **NDV (Number of Distinct Values)** has a high degree of duplication: If the sample data has a duplication rate below 1%, adjust the statistical weight.
   - Introduce **SSCAN_DEFAULT_CARD**: To prevent inefficient plans when **NL JOIN(Nested Loop Join)** estimates cardinality (result count) too low.

#. **Additional improvements** 
  
   - Add **fetch_time** to trace information to enhance execution time tracking. 
   - Enable automatic selection of **index skip scan** without the ``index_ss`` hint. 
   - Activate **Sort Merge Join**: Allow the optimizer to use the Sort Merge Join method.

This improvement enhances the accuracy of cost calculation formulas based on actual execution time and adds optimizations like index usage and join methods.

Add cost formulas for the LIMIT clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We introduce cost formulas that consider the **LIMIT** clause and improve the execution plan selection approach (optimization criteria) when a LIMIT is present.

#. **Cost and cardinality settings for the LIMIT clause**

   - Set the cardinality of a subquery with a LIMIT clause to the **LIMIT value**.
   - For example, if ``LIMIT 10`` is used, estimate the result count of the subquery to be 10.

#. **Enhance rule-based optimization when a LIMIT is present**

   - When LIMIT is used, give priority to execution plans that can quickly fetch the first few rows. 
   - Use **ORDER BY SKIP**
   - Apply **SORT-LIMIT** optimization
   - Prefer plans that retrieve a subset of the data faster than reading the entire dataset.

#. **Change the value of SSCAN_DEFAULT_CARD (default cardinality)**

   - Change from 1000 to 100 to avoid selecting inefficient plans for sequential scans (SSCAN).

This introduces cardinality and cost adjustments for the **LIMIT** clause, optimizing the performance for small data retrievals and adjusting the default cardinality value for more efficient plans.

Remove logically redundant join conditions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This improvement removes logically redundant join conditions that have already been evaluated, optimizing the query.

.. code:: sql

   a.col1 = b.col1 AND b.col1 = c.col1 AND a.col1 = c.col1;

In this case, having all three conditions is unnecessary, as just one condition is sufficient. Keeping all three results in redundant filters (terms), which should be removed.

This change improves query optimization by eliminating duplicate conditions that don’t contribute to the logic.

Eliminate unnecessary ``INNER JOIN`` and join types during query optimization
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Conditions and exceptions for the “Eliminate INNER JOIN” optimization:

#. **INNER JOIN Removal Optimization**
   
   - This optimization removes unnecessary INNER JOINs
   - For example, if only one table exists, the JOIN itself is meaningless, and it is removed

2. **Optimization Behavior (Conditions)** 
   
   - The INNER JOIN removal optimization is applied to the first table. 
   - If the join type for the second table is INNER JOIN, it will also be removed.

#. **Exception Conditions** 
   
   - If the join type for the second table is OUTER JOIN, this optimization is not applied.
   - OUTER JOIN must be retained as they can affect the result.

#. Example
  
   - AS-IS
     
   .. code:: sql

      SELECT /*+ ORDERED */ c.c2 FROM t2_parent p INNER JOIN t2_child c ON c.id = p.id

   - TO-BE (The INNER JOIN is removed. The unnecessary JOIN clause is eliminated, leaving just the table.)

   .. code:: sql

      SELECT /*+ ORDERED */ c.c2 FROM t2_child c


This optimization removes unnecessary INNER JOIN, but it ensures that the optimization is not applied when the second table has an OUTER JOIN.

Performance
~~~~~~~~~~~

Redesigning the plan and executor for stored procedure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We are working on eliminating unnecessary **method transformations** in stored procedures. In the current approach, when a stored procedure parameter is associated with a table column, it is transformed into an inline view and a **lateral join** structure is created. However, this approach leads to performance degradation
(e.g., inefficient index scans, incorrect outer joins). To resolve this issue, a new execution plan is being introduced.

#. Enabling index scans when an stored procedure(SP) is used as a comparison value in the ``WHERE`` clause.
#. Modifying the rewrite process to prevent stored procedure(SP) calls from being transformed as part of a table scan.

This optimization aims to remove unnecessary transformations from the existing approach and improve the performance of stored procedure execution plans.

Improving the process of calculating ``midxkey.buf`` size when reading records from multi-column index nodes for performance enhancement
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Previously, the key length in a multi-column index had to be recalculated repeatedly. This has been improved by adding an ``OFFSET`` for each column, allowing direct reference. This optimization reduces unnecessary operations during **binary search, key filtering, and DML execution**, leading to improved performance.

Improving key reading in B-Tree indexes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. **Optimizing key reading and processing during range scans**:

   Reduces unnecessary operations by optimizing the way keys are read and processed during range scans.

#. **Optimizing key reading for statistics updates and capacity calculations**:

   Enhances performance by refining the key reading process when updating statistics and calculating index capacity.

#. **Reducing ``upper_key`` comparison frequency during range scans**: 

   Minimizes unnecessary comparisons with ``upper_key`` to improve performance. 

#. **Reducing redundant index column ID comparisons**: 

   Optimizes operations by decreasing repetitive comparisons of index column ID.

Storing the common prefix in ``BTREE_NODE_HEADER`` for compressed leaf nodes to reduce redundant calculations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Currently, the **common prefix** is recalculated every time an index scan occurs. However, since the common prefix remains unchanged unless the node is split or merged, recalculating it repeatedly is inefficient. To determine the common prefix, the **lower fence key** and **upper fence key** within a leaf node must be checked and compared to identify how many columns are shared.

This improvement aims to **enhance binary search performance in compressed leaf nodes** by skipping redundant common prefix calculations.

#. In the improved structure, the common prefix is stored in ``BTREE_NODE_HEADER``, allowing immediate reference without additional computations.

#. This modification improves index scan performance.

Performance improvement for ``time_format()`` and ``date_format()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Optimized performance by eliminating the use of string concatenation functions in the ``STRCHCAT`` macro, reducing function call overhead.

Improving query performance degradation when ``TRACE`` is enabled
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When executing queries with ``set trace on``, performance drops significantly compared to running the same query with ``set trace off``. To minimize performance degradation, high-cost operations in TRACE statistics collection have been optimized to reduce overhead as much as possible.

Removing unnecessary length checks for string types
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The order of size checks for string types has been changed, and unnecessary steps have been reduced to improve overall query performance.

PL/CSQL, JAVA SP
~~~~~~~~~~~~~~~~

Modify to restart Java SP on JNI segmentation fault
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In case of a segmentation fault (segfault) in JNI, the system has been modified to restart the Java stored procedure (SP).

Enhancing string functionality to support multiple character encodings (euckr, utf8)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In Java programs before version 11.2, the string encoding was affected by the file encoding, which could lead to incorrect character set conversions and string corruption. Since version 11.2, string handling has been standardized to UTF-8 encoding. However, many databases still use encodings other than UTF-8, so support for these encodings is necessary.

The improvement involves processing strings as byte arrays to support various character encodings.

Setting ``java.io.tmpdir`` in Java SP when ``CUBRID_TMP`` is configured (defaults to ``$CUBRID/tmp`` if not set)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When the ``java.io.File.createTempFile()`` function tries to create a temporary file, an error occurs. Junixsocket internally uses this function to create temporary files and then deletes them immediately. By default, ``createTempFile()`` creates files in the ``/tmp`` directory, but the file creation location can be changed using the ``-Djava.io.tmpdir`` option.

#. If ``CUBRID_TMP`` is set, the value of ``CUBRID_TMP`` is reflected in ``java.io.tmpdir`` even if the ``java_stored_procedure_jvm_options="-Djava.io.tmpdir=<path>"`` option is set.

#. The user-defined values are displayed in the ``cubrid javasp status`` command output, and the ``java.io.tmpdir`` setting is reflected even if set through ``java_stored_procedure_jvm_options``.

#. If ``CUBRID_TMP`` is not set, ``$CUBRID/tmp`` is used as the value for ``java.io.tmpdir``.

#. The ``java.io.tmpdir`` path is initialized only when the Java SP server starts, and changing the path requires restarting the Java SP server.

Utility
~~~~~~~

Renaming environment variables in CSQL (e.g., ``FORMATTER`` → ``CUBRID_CSQL_FORMATTER``)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There was an issue where the environment variables used in the CSQL environment, such as ``FORMATTER``, ``EDITOR``, and ``SHELL``, were not clearly related to CUBRID. To resolve this, the variable names have been updated to include the ``CUBRID_CSQL`` prefix to explicitly indicate that they are related to CUBRID.

- **Changes**: 

  -  EDITOR → CUBRID_CSQL_EDITOR
  -  SHELL → CUBRID_CSQL_SHELL
  -  FORMATTER → CUBRID_CSQL_FORMATTER

The original variable names are still supported, ensuring compatibility with existing systems.

Modify to release allocated lock resources immediately when exceeding the ``lock_escalation`` setting
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There was an issue where memory allocated for locks was not released after the lock was freed once the ``lock_escalation`` environment variable value was exceeded. To resolve this, the system has been modified to immediately return memory when the number of locks exceeds the configured limit, ensuring that memory is freed when locks are released.

Modify ``diagdb -d 9`` to dump only specific classes(tables)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instead of dumping the entire heap file, a feature has been added to allow dumping only a specified class.

- **Usage:**

  -  The ``diagdb -d 9`` command can now accept the class (table) name, including the user name, as a parameter.
  -  ``diagdb -d 9 -n class-name`` → Dumps only the specified class.
  -  For partitioned tables: Dumps all partitions.
  -  For specific partitions: Dumps only the specified partition.

Validating the ``cubrid_hosts.conf`` file when ``use_user_hosts=true``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When ``use_user_hosts=true``, the ``cubrid_hosts.conf`` file will be validated at the CUBRID startup. If an error is found, an appropriate message will be displayed, and the execution will be halted.

- **Validation checks**

  #. **File existence**: If the file does not exist, an error will occur.
  #. **IP address check**: Only IPv4 addresses are supported.
  #. **Hostname check**

     -  Length: Between 1 and 63 characters.
     -  Allowed characters: Alphabets (A-Z, a-z), numbers (0-9), and hyphens (-).
     -  Hyphens are not allowed at the beginning or end.
     -  Examples: ``cubrid``, ``node-1``, ``www.cubrid.com``

  #. **FQDN (Fully Qualified Domain Name) check**

     -  Maximum length: 255 characters.
     -  Multiple labels separated by periods (``.``)
     -  Each label must follow the hostname rules.
     -  Example: ``mail.server.cubrid.com``.

- **Restrictions**

  -  Aliases are not supported.
  -  Lines starting with ``#`` are treated as comments.

- **Scope of validation** 
  
  - The validation applies to CUBRID services and all management utilities (e.g., ``service``, ``server``, ``broker``, ``createdb``, ``backupdb``, ``vacuumdb``, etc.).

Fixing HEAP and SYSTEM results in ``cubrid spacedb`` output
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In the current implementation, the file types of the database(``FILE_TYPE``) were incorrectly mapped to ``SPACEDB_FILE_TYPE``, causing discrepancies in the ``spacedb`` output compared to previous versions.

The mapping issue has been corrected so that:

-  The **heap pages** in the ``show heap capacity of`` command are correctly calculated as **HEAP**.
-  The **overflow pages** are correctly calculated as **SYSTEM**.

This fix ensures that the ``spacedb`` output now matches the expected values.

HA
~~

Fixing error messages when loading ``ha_node_list`` and ``ha_replica_list``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Currently, when ``ha_node_list`` or ``ha_replica_list`` contains invalid information, the ``master.err`` log only prints a generic error message, making it difficult to identify the root cause. This update aims to provide clear error messages based on the specific error type.

- **AS-IS**

  -  All errors are logged as **“ha_node_list”: Unknown system parameter or bad value.**

- **TO-BE**

  +-----------------------------------+----------------------------------+
  | 오류 메시지                       | 오류 유형                        |
  +===================================+==================================+
  | ha_node_list is empty             | When ``ha_node_list`` is         |
  |                                   | ``NULL`` or an empty string      |
  +-----------------------------------+----------------------------------+
  | cannot fi                         | When ``ha_mode`` is ``on`` but   |
  | nd (myhost) in the ha_node_list   | ``myhost`` is not listed in      |
  |                                   | ``ha_node_list``                 |
  +-----------------------------------+----------------------------------+
  | group id of (ha_node_list         | When ``ha_node_list`` and        |
  | , ha_replica_list) is different   | ``ha_replica_list`` have         |
  |                                   | different group ID               |
  +-----------------------------------+----------------------------------+
  | In                                | When ``ha_mode`` is ``replica``, |
  | replica node, (myhost) must not b | but ``myhost`` is present in     |
  | e specified in the ha_node_list   | ``ha_node_list``                 |
  +-----------------------------------+----------------------------------+
  | ha_replica_list is empty          | When ``ha_mode`` is ``replica``  |
  |                                   | and ``ha_replica_list`` is       |
  |                                   | ``NULL`` or empty                |
  +-----------------------------------+----------------------------------+
  | In replica node, (myhost) must be | When ``ha_mode`` is ``replica``, |
  | specified in the ha_replica_list  | but ``myhost`` is missing from   |
  |                                   | ``ha_replica_list``              |
  +-----------------------------------+----------------------------------+
  | In not rep                        | When ``ha_mode`` is ``on``, but  |
  | lica mode, (myhost) must not be s | ``myhost`` is present in         |
  | pecified in the ha_replica_list   | ``ha_replica_list``              |
  +-----------------------------------+----------------------------------+

- **Additional Changes**

  - When `ha_mode` is `on` and `myhost` is present in `ha_node_list`, it will now trigger an **error** instead of switching to replica mode.

Modifying error messages for HA failover and failback
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- **Current Issues**
 
  - Unclear log messages during HA Failover/Failback make root cause analysis difficult. 
  - In some cases, no logs are recorded. 
  - Existing messages do not clearly indicate the cause and result.

- **Improvements**

  - Clearly distinguish between **Failover (Slave → Master)** and **Failback (Master → Slave)**. 
  - Separate **Diagnosis messages** and **Result messages**. 
  - Add log tags: ``[Failover]``, ``[Failback]``, ``[Diagnosis]``, ``[Cancelled]``, ``[Success]``. 
  - Log **Failover messages** on the **Slave node** and **Failback messages** on the **Master node**. 
  - Fix missing log issues (e.g., missing logs during Failback, add logs related to Ping checks).

Modify ``applyinfo`` utility to display volume creation time
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

With the addition of **Volume Creation Time** (``Vol creation time``) to the log volume header, the ``applyinfo`` utility will be updated to include this information when displaying **active log volumes** and **archive log volumes** in an HA environment.

Broker, CAS, CMS
~~~~~~~~~~~~~~~~

Modify broker ACL reload to apply changes to existing CAS connections
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Currently, when reloading the ACL using the ``cubrid broker acl reload`` command, the new ACL rules apply only to **new connections**, while **existing CAS (CUBRID Application Server) sessions** continue without the updated rules.

- **Improvements**
 
  - After reloading the ACL, **existing CAS connections** will not persist indefinitely under the old rules. 
  - Once a transaction is completed, **the updated ACL rules will be enforced** for subsequent access control.

Modify conf (cubrid_broker.conf, cubrid_gateway.conf) to create necessary directories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- **Issues**

  - CUBRID Broker/Gateway uses separate directories for different types of logs.
  - Currently, both **CUBRID Broker** and **CUBRID Gateway** create log directories **twice**.
  - This can cause discrepancies between the **configured** and **actual** directories.

- **Improvements**
 
  - Prevent duplicate creation of log directories. 
  - Ensure that only the directories defined in the configuration files are created.

Modify ``cubrid broker info`` to always display broker log directory in absolute path
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``cubrid broker info`` command displays broker configuration details, including directory-related information (``LOG_DIR``, ``ERROR_LOG_DIR``, etc.). However, the behavior varies based on system state:

- **After initial CUBRID installation and startup**
  
  - If the directories are set with relative path in ``cubrid_broker.conf``, they are displayed as **relative path** 

- **After stopping and restarting the CUBRID service** 
   
  - Upon restart, the directories are displayed as **absolute path**, with ``$CUBRID`` prepended.

- **Improvement**
 
  - Ensure that **broker-related directories** are **always displayed as absolute path**, regardless of whether CUBRID has been restarted.

Expand ``cubrid broker info`` output to display ``ADMIN_LOG_FILE``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- **Improvement**
 
  - Modify the ``cubrid broker info`` command to include ``ADMIN_LOG_FILE`` in its output. 
  - This parameter records timestamps related to **broker operations**, making it easier to track startup and execution details.

Set ppid of broker/cas processes created by ``cub_manager`` to 1 to prevent zombie processes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modify ``getlogfileinfo()`` API to return SQL logfile info only once
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Currently, when the ``CMS getlogfileinfo()`` API is called, it returns the same SQL **logfile information** twice.
Modify the ``getlogfileinfo()`` API to ensure that **SQL logfile information** is returned **only once**. This will eliminate the redundancy and improve the efficiency of the API response.

Modify ``ha_status()`` API to display Replica Node status in HA environment with Master, Slave, Replica
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Others
~~~~~~

Modify ``cub_vsnprintf()`` to support ``vsnprintf()`` on Windows
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _11_4_changes_bug_fixes:

Bug Fixes
---------

SQL
~~~

Modify *ALTER INDEX … REBUILD* to display an drror when adding columns
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modify rownum calculation to display an error when exceeding numeric type range
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where rownum in scalar subquery is incorrectly replaced with order_by_num() during view merging
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modify the result of 'NULL \|\| string' operation when oracle_style_empty_string=yes is enabled
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where REPLACE function returns NULL when oracle_style_empty_string=yes is enabled
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue with errors in covered index queries using function indexes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix core dump issue in CTE queries with always false or null conditions (e.g., where 0=1)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix core dump issue when using union all with multiple tables and serial next_value in a single query
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix core dump issue in select queries with concatenation of column and constant value exceeding 255 characters without alias
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix error in view merging when processing queries with outer join
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modify remote server lookup behavior when user schema is omitted and identical remote servers exist for two accounts
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where ``show create view`` displays both current user's view and public user's view with the same name
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where exceeding string_max_size_bytes does not trigger an error and instead returns null
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix error in query optimization when inner join is removed, causing incorrect table positioning
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix syntax error when using host variables as arguments for casting functions in prepared statements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where 'drop user' does not remove user information from db_authorization and db_auth catalog table
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Prevent use of analytical functions in update join queries with two or more tables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where oracle style left outer join is not rewritten as inner join when host variables are used in SP call and where clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where subquery with orderby_num() is incorrectly output during view merge
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where tables without join relationships are incorrectly joined, producing wrong results
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue to trigger error when invalid value is provided for optimization_level setting
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where rownum is displayed as 0 when columns used in where clause for equality condition (=) and order by clause are the same
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix error caused by using inner join and oracle style outer join together in where clause with different outer join condition placement
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix error in 'insert into tbl … select … from view_table … on duplicate key update' query execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where column sequence numbers inside analytical functions in views are incorrect
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where range conditions using pipe operator (||) are not reduced to common range items
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Optimizer
~~~~~~~~~

Fix issue where better indexes are not chosen over primary key (PK)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Performance
~~~~~~~~~~~

Allow result cache usage even when the number of host variables changes during sql rewrite
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Exclude subquery from cache when it cannot be processed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where subquery cache is not used in update / delete queries referencing with clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue to enable query cache handling when using prepare statement
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Allow query caching for SP functions used in correlated subqueries
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

PL/CSQL, JAVA SP
~~~~~~~~~~~~~~~~

Fix issue when passing datetimeltz value to datetime type java sp parameter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix NoSuchMethod exception by removing space between method name and opening parenthesis in java sp method calls
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix unsupported argument error for unrelated columns in java sp
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Prevent 'cannot allocate query entry any more' error in java sp
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Utility
~~~~~~~

Modify loaddb to exit with code 3 when an error occurs during execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix incorrect count(*) value after running loaddb with no-logging option
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Ensure reverse unique index comments are not omitted when running unloaddb
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where auto_increment value resets to 1 when unloading PK and auto_increment columns with unloaddb
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Prevent creation of 'dbname_schema_uk' file when no unique index exists during unloaddb –split-schema-files execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Prevent unrelated ``alter serial`` statements from being output when using -i and –input-class-only options in unloaddb
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix error occurring during unloaddb/loaddb when serial ``current_val`` equals ``max_val``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modify loaddb to stop execution after error with error message displayed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix infinite loop issue in unloaddb when handling json data larger than 1MB
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Remove user schema of ``query_spec`` owner when a regular user unloads a view
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Allow extraction of other schemas (user, serial, sp, server, synonym, grant) when running unloaddb on a database without tables and views
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where [user_schema] is incorrectly stored or omitted in db_trigger catalog’s condition and action_definition columns when executing loaddb with unload files from unsupported versions of trigger creation and schema
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix error in unloaddb/loaddb when identical serial names exist across different accounts
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where schema name is omitted when unloading Serial and Trigger as DBA user using unloaddb
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix error occurring during loaddb after unloaddb when using system tables as synonyms
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where dba members without –as-dba option cause grant on procedure from other users to be output during unloaddb
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Remove [user_schema] from condition and action_definition when a regular user unloads a Trigger and the owner is themselves
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where archived log volumes created during backupdb are not deleted
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where incorrect serial values are generated during unloaddb
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

HA
~~

Fix issue where archived logs replicated from slave node are not deleted on replica node when ``ha_replica_delay`` is set to 60 seconds or more in ha environment
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

update usage message for -d option in ``cub_commdb`` utility
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue to prevent data mismatch with master node during restoreslave execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix issue where SQL log files are not automatically deleted when SQL log ID exceeds UINT_MAX and resets to 0 during applylogdb process
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Broker, CAS, CMS
~~~~~~~~~~~~~~~~

Fix memory leak issue when using ``addbatch()`` and ``executebatch()`` in java development environment
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fix broker error due to incorrect acl configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Add ``db name`` to cas ddl audit log to identify the database in multi-db environments
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Record ABORT log in ``ddl_audit`` log even when transaction ends without ``commit/rollback``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Improve handling of ``commit`` or ``rollback`` statements between multiple ddl executions when using ``setautocommit(false)`` - log commit and rollback in ``ddl_audit.log``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
