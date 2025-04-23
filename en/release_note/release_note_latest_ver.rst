:tocdepth: 3

.. raw:: html

   <style>
   h4 a:link { text-decoration: underline; font-size: 118%; }
   h5 { font-size: 100%; }
   </style>

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

#. **PL/CSQL support for Oracle compatibility**
#. **Addition of HASH JOIN for large-scale processing**
#. **Performance improvements through optimizer and index processing enhancements**
#. **Performance boost in data recovery through parallel processing**
#. **Performance enhancement by expanding result caching**
#. **Improved data dump performance**
#. **Addition of memory monitoring features**
#. **Enhanced access control functionality**
#. **Improved user convenience for backup and recovery operations**

These updates bring not only new features but also various performance improvements across multiple areas.

Driver Compatibility
--------------------

- CUBRID 11.4 ensures compatibility with earlier driver versions. To maximize the utilization of the advanced features introduced in this version, it is strongly recommended to adopt the latest driver.

.. _11_4_changes:

11.4 Changes
============

.. _11_4_changes_add_feature:

New feature
-----------

PL/CSQL
~~~~~~~

- CUBRID offers the PL/CSQL feature to ensure compatibility with ORACLE’s PL/SQL. 
- For syntax and usage details, please refer to the manual. (:ref:`sql_procedural_langauge`\)

SQL
~~~

HASH JOIN Support
^^^^^^^^^^^^^^^^^

- It supports HASH JOIN, a type of join processing method that creates a hash table based on the join condition column to perform the join.
- For the optimizer to apply a HASH JOIN, a join hint is required.

  - When ``/*+ USE_HASH */`` hint is added, HASH JOIN is considered.
  - When ``/*+ NO_USE_HASH */`` hint is added, HASH JOIN is not used.

Support for Changing Owner of SERIAL SQL Syntax
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Extended the **ALTER SERIAL** statement to support changing the owner of a SERIAL, which was previously only possible by calling the ``call change_serial_owner()`` method.

.. code:: sql 

   ALTER SERIAL serial_name OWNER TO user_name

- Only the DBA or members of the DBA group are allowed to change the owner of a SERIAL.
- An error is returned if the user does not have sufficient privileges or if the specified SERIAL does not exist.

Support for SQL Statements to Add/Remove Members from a Created Group
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Extended the **ALTER USER** statement to support adding or removing users to/from a created group, which was previously only possible by calling ``call add_member()`` and ``call drop_member()`` methods.
- In HA environments, this enhancement simplifies user management by eliminating the need for separate operations on the master and slave nodes when modifying group membership.

.. code:: sql

   ALTER USER user_name 
   [PASSWORD password] |
   [ADD MEMBERS user_name {, user_name}...] |
   [DROP MEMBERS user_name {, user_name}...]
   [COMMENT 'comment_string']

- DBA/DBA Group Members

  - Add or remove members from the DBA group
  - Add or remove members from user-defined groups, including PUBLIC

- General Users

  - Can add or remove members only from their own groups

Extended Query Cache Support
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The query cache feature, which was previously available only at the full query level, has been extended to support subquery-level caching.

- Support query chache in CTE(Common Table Expressions).

.. code:: sql

   with cte0 as (select /*+ query_cache */ count(*) from public.game where host_year > 2000),
   cte1 as (select /*+ query_cache */ count(*) from public.game where host_year = 2004)
   select * from cte1;

.. code:: sql

  Trace Statistics:
  SELECT (time: 0, fetch: 4, fetch_time: 0, ioread: 0)
    SCAN (temp time: 0, fetch: 4, ioread: 0, readrows: 1, rows: 1)
    SUBQUERY (uncorrelated)
      CTE (non_recursive_part)
        SELECT (time: 0, fetch: 0, fetch_time: 0, ioread: 0)
          RESULT CACHE (reference count: 1)
      CTE (non_recursive_part)
        SELECT (time: 0, fetch: 0, fetch_time: 0, ioread: 0)
          RESULT CACHE (reference count: 1)

- Support query cache in non-correlated subqueries.

.. code:: sql

   select count(host_year)
   from (select /*+ query_cache */ code from public.nation where code like 'K%') n, public.game g
   where n.code = g.nation_code;

.. code:: sql

  Trace Statistics:
  SELECT (time: 218, fetch: 872, fetch_time: 2, ioread: 0)
    SCAN (temp time: 0, fetch: 4, ioread: 0, readrows: 7, rows: 7)
      SCAN (table: public.game), (heap time: 215, fetch: 868, ioread: 0, readrows: 60571, rows: 386)
    SUBQUERY (uncorrelated)
      SELECT (time: 0, fetch: 0, fetch_time: 0, ioread: 0)
        RESULT CACHE (reference count: 3)

- Support displaying the reference count in trace information when executing queries that utilize the result cache by **RESULT CHACHE (reference count: number)**


Added **LEADING** Hint to Specify Join Order
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- The **LEADING** hint allows specifying a particular table or set of tables to be used as the prefix in the execution plan, enabling more flexible control over the table join order than the **ORDERED** hint.

.. code:: sql

   SELECT /*+ LEADING(e j) */ *
   FROM  employees e, departments d, job_history j
   WHERE e.department_id = d.department_id
   AND e.hire_date = j.start_date;

- Conditions for Ignoring the LEADING Hint

  - Ignored when the ORDERED hint is present.
  - Multiple LEADING hints are used, only the first one is applied.
  - Ignored if join graph dependencies prevent early joining of specified tables.

Utility
~~~~~~~

Added Option to Delete Specific Plans in ``cubrid plandump``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Option: *-s*

Added Support for Separate loadjava Option for JNI-Based JavaSP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Option: *-j* or *--jni*
- Without using the new ``loadjava -j`` option will now result in an error when JNI-Based JavaSP executed.

Added Memory Monitoring Featured utility
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- The memory monitoring utility ``cubrid memmon`` retrieves and displays the following heap memory information from the server process (*cub_server*) memory management module.

  - Total heap memory usage allocated and currently in use by the server process.
  -  Detailed memory allocation information based on the source code and line number where the memory allocation request was made.
   
- To use the server process memory management module, the system parameter enable_memory_monitoring must be set to "yes", and the server needs to be restarted.
- For detailed usage and instructions, please refer to the manual. (:ref:`memmon`\)

Supports ``diagdb -d 9`` to dump only specific classe(table)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Option: -n class-name 
- In the case of partitioned tables, all partitions are dumped. For sub-partitioned tables, only the corresponding partition table is dumped.

Broker,CAS,CMS
~~~~~~~~~~~~~~

Added Parameter to Set the Size of Data Sent to Clients in Broker
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- A new parameter, **NET_BUF_SIZE**, has been introduced to the Broker to control the size of the data transmitted to clients.
- The values that can be set for **NET_BUF_SIZE**

  - 16K(default)
  - 32K
  - 48K
  - 64K

Added Parameter to Set ACL(Access Control) for Each Broker
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- When using the broker's ACL (ACCESS_CONTROL=ON), the ACCESS_CONTROL_DEFAULT_POLICY parameter allows you to set the default action policy for brokers that are not specified in the ACCESS_CONTROL_FILE.

- Set the value of ACCESS_CONTROL_DEFAULT_POLICY to **DENY** or **ALLOW** in the [broker] section of the ``cubrid_broker.conf`` file (default value is **DENY**).

   - If the broker is not specified in ACCESS_CONTROL_FILE, and the default policy is ALLOW, all clients are allowed to connect; if the policy is DENY, connections are denied.

.. code:: shell

   $ cubrid broker acl status 
   ACCESS_CONTROL=ON 
   ACCESS_CONTROL_DEFAULT_POLICY=DENY
   ACCESS_CONTROL_FILE=cubrid_acl.conf 

   [%query_editor] 
   testdb:dba:acl_ip_list.conf 

   CLIENT IP LAST ACCESS TIME 
   ========================================== 
   172.29.80.1 
   192.168.0.31 
   172.31.0.175 

   [%broker1] 

   ++ cubrid broker acl: success

- **[%broker1]** does not have any client access information specified, all client connections are denied according to the default policy **DENY**.

Others
~~~~~~

Support for Automatic Restart of Non-HA DB Server After Abnormal Termination
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- When operating a non-HA DB server, a new feature enables automatic restart if the DB server is abnormally terminated, such as by the OOM killer.

- Notes

  - The server does not automatically restart after a normal shutdown.
  - This feature can be enabled or disabled using the auto_restart_server parameter.
  - Automatic restart is disabled if the server repeatedly terminates abnormally within a short period or fails to start successfully after a certain number of attempts.

Support for Script to Restore Backup as a New Database
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- When data recovery from a backup is needed on a production server, the ``rename_to_newdb.sh`` script allows restoring the backup as a new database without the need for additional steps such as creating new accounts or installing extra engine instances. For detailed usage and instructions, please refer to the manual.

.. _11_4_changes_spec:

Specification Changes
---------------------

SQL
~~~

Limit Maximum Number of Characters for CHAR Type to 2,048 (Previous Limit: 268,435,456)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- To prevent potential issues such as memory allocation errors, the maximum number of characters allowed for the CHAR type has been reduced from 268,435,456 to 2,048.
- When migrating databases from versions prior to 11.4, any CHAR type columns exceeding 2,048 characters must be converted to VARCHAR.
 
LOB Locator Path Changed to Relative Path
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- To improve manageability and ease of moving LOB data files, the locator path for LOBs has been changed from an absolute path to a relative path.
- If the LOB directory location for a DB listed in databases.txt is changed, all existing LOB files in the previous directory must be moved or copied to the new LOB directory.

Restriction on Use of Analytic Functions in UPDATE JOIN Queries
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- To prevent unexpected behavior, the use of analytic functions in UPDATE JOIN queries involving two or more tables is now restricted.
- Queries using this pattern that were written in versions prior to 11.4 must be revised, as they will cause errors in version 11.4.

Restrict Use of **for update** clause on System Tables and Views
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- To prevent lock waits caused by X-LOCKs on system tables, the use of the **for update** clause on system tables and views now results in an error.

Data Type Consistency Check Disabled During View Creation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- To improve Oracle compatibility, data type checks during view creation are no longer performed. Type consistency is now checked at runtime.
- If type conversion is not possible, an error will occur when the view is executed.

Allow Use of NULL in SELECT Clause of View Query Specification
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- To enhance Oracle compatibility, the use of NULL values in the SELECT clause of a view query specification is now allowed.

.. code:: sql

   CREATE VIEW a_view( col1 ) AS select NULL as col1 from a_tbl;  

Disallow Use of Both **AUTO_INCREMENT** and **DEFAULT** in TABLE Modification and Creation.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- When executing **CREATE TABLE** or **ALTER COLUMN** statements, an error will now be raised if both **AUTO_INCREMENT** and **DEFAULT** attributes are specified together.

Raise Error When Adding New Columns in **ALTER INDEX … REBUILD**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- An error will now be raised if new columns are added during an ALTER INDEX … REBUILD operation, as adding columns is not supported in this context.
- Queries written in versions prior to 11.4 using this pattern will cause errors in version 11.4 and must be revised.

Raise Error When ROWNUM Value Exceeds NUMERIC Type Range
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Utility
~~~~~~~

Recognize Session Commands While Writing SQL or PL/CSQL Statements(like create, body and etc) in `csql` Interactive Mode
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Suppress Plan Information Output Before Deletion in ``cubrid plandump -d``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

HA
~~

Raise Error When ``ha_mode`` is **on** and **myhost** is Included in **ha_node_list**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- In previous versions, this configuration would automatically switch the server to replica mode.

Others
~~~~~~

Column Name Changed in System Table db_serial (att_name → attr_name)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Queries that explicitly reference att_name in versions prior to 11.4 must be updated, as they will result in errors in version 11.4.

.. _11_4_changes_perf_improvements:

Improvements (Including Performance Enhancements)
-------------------------------------------------

SQL
~~~

Performance Improvement with Caching for Correlated Subqueries
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Correlated subqueries are executed individually for each row of the main query. When there are many repeated condition values, the same subquery can be executed multiple times, leading to performance degradation. By implementing caching, repeated executions of the same subquery for identical condition values are prevented, improving performance.

Enhancement of Sort-Limit Optimization When Bind Variables or Expressions Are Used in the LIMIT Clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

PL(incl JavaSP)
~~~~~~~~~~~~~~~

Improved Automatic Restart of JavaSP Server When JNI Program Encounters Segmentation Fault
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Enhanced String Functionality to Support Multiple Character Encodings (euckr, utf8)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- The string handling functionality has been improved to support various character encodings, such as euckr and utf8, by processing the encoding scheme of stored procedure strings as byte arrays.

Index
~~~~~

Removed Length Limitation on WHERE Clause When Creating Filter Indexes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Improved Index Scan Performance by Optimizing midxkey.buf Size Calculation for Multi-Column Indexes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- In multi-column indexes, the previous method of repeatedly calculating the key length has been improved. Now, each column's **OFFSET** is directly referenced without needing calculations, which improves performance during **binary search, key filtering, and DML operations**.

Optimized Index Scan Performance by Reducing Unnecessary Calculations and Comparisons
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- The process of reading and processing keys during Range-Scan has been optimized to reduce unnecessary operations.
- Unnecessary comparisons with *upper_key* during Range-Scan have been reduced, enhancing performance.
- Repetitive comparisons of index column IDs have been minimized, optimizing operations.
- Common prefix information has been added to leaf node headers, optimizing the processing of compressed leaf nodes by reducing repetitive comparisons and calculations.

Optimizer
~~~~~~~~~

Performance Improvement Through Overall Optimizer Enhancements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- The number of sampling pages has been increased to 5000 to improve the accuracy of collected statistics.
- Enhanced the method of collecting NDV(Number of Distinct Values), leading to more accurate statistics.
- Minimized Rule-Based Optimization (RBO) for Better Accuracy

  - Reduced the use of empirical weights (Heuristic Factor).
  - Removed RBO for cost differences within 1.x, transitioning to Cost-Based Optimization (CBO) for improved accuracy.

- Improved Cost Formula Calculation

  - **Reflected Index Filter Scan Selectivity**: Adjusted I/O cost calculation during index scan to account for selectivity.
  - Added selectivity for the *NOT LIKE* operator.
  - Added selectivity for **function-based indexes**.
  - Adjusted weight for **NDV**\(Distinct Value Count) when duplicates are high: If the duplication rate in sampling data exceeds 1%, statistics weight is adjusted.
  - Introduced **SSCAN_DEFAULT_CARD**: Prevents inefficient plans in **NL JOIN**\(Nested Loop Join) when cardinality estimation is too low.
  - Introduced cost and cardinality settings for the LIMIT clause to improve performance for small data retrievals, with **optimized LIMIT** and adjusted default cardinality values.

- Elimination of Redundant Conditions

  - Removed logically redundant predictable join conditions that were already evaluated.
  - Improved the removal of unnecessary INNER JOIN.
   
-  Additional Improvements

  - Enhanced trace information by adding **fetch_time** for better execution time tracking.
  - Improved **index skip scan** to be selected even without the ``index_ss`` hint.
  - Enhanced the optimizer to choose a better index over the Primary Key.

Improvement in Stored Procedure Execution Plans
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- The execution plan processing for stored procedures has been modified to align with the execution plan of built-in functions.
- Improvements

  - Index scans are now possible.
  - Unnecessary joins have been removed.
  - **Result caching** is now available when using stored procedures in **correlated subqueries**.

Improvement in Concurrency by Preventing Unnecessary X-LOCKs on Unneeded Rows
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- In certain queries, locks were being created too early, causing unnecessary locks. The system has been improved to release unnecessary locks after all conditions have been evaluated.

Improvement in Index Scanning for LIKE Queries Using JavaSP Functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

Improvement in Range Index Scanning for Queries with ``<=`` and ``>=`` Conditions on Function Indexes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Utility
~~~~~~~

Improvement to ``unloaddb`` in Databases Without Tables and Views to Extract Other Schemas(user, serial, sp, server, synonym, grant)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Performance Improvement in unloaddb
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- To significantly reduce data extraction time, multi-threading has been implemented to improve performance.

- unloaddb options

  - **--thread-count**: Specifies the number of concurrent threads to execute (0–127).
  - **enhanced-estimates**\: Provides more accurate record count estimation (for verbose mode only).

Improvement in Clarity of ``csql`` Environment Variable Names
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- To improve the clarity of CSQL environment variable names, the prefix CUBRID_CSQL has been added to indicate that these environment variables are related to CUBRID.

- **Changes**

  - EDITOR → CUBRID_CSQL_EDITOR
  - SHELL → CUBRID_CSQL_SHELL
  - FORMATTER → CUBRID_CSQL_FORMATTER

- For backward compatibility, the original environment variable names can still be used.

Improved the output of the ``cubrid spacedb`` command
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Improved the classification of overflow pages so that pages used by users are categorized as either INDEX or HEAP based on their purpose, instead of being incorrectly classified as SYSTEM pages.

Broker,CAS,CMS
~~~~~~~~~~~~~~

Improvement in cubrid spacedb Output
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Directory names used in configuration settings are now displayed as absolute paths.
- **ADMIN_LOG_FILE** display has been added.

Error Occurrence for Incorrect ACL Settings for Improved User Awareness
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Improvement to Allow TLS v1.2 Clients to Connect to CMS(CUBRID Manager Server)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Improvement to Prevent Zombie Processes When Broker/CAS Processes are Terminated Through CMS
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Improvement to cms getlogfileinfo() API to return SQL log file information only once
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Improvement to cms ha_status() API to Display Replica Node Status in HA Environments with Master, Slave, and Replica Configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

HA
~~

Clear Error Message Display for Incorrect Settings in **ha_node_list** and **ha_replica_list** for Improved User Awareness
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Clear Error Message Display for Failover and Failback Occurrences to Improve User Awareness
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Improvement to Prevent Incorrect Data Mismatch Error Message from Appearing When **restoreslave** is Executed on Slave Node
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Others
~~~~~~

Improvements to User Hosts (cubrid_host.conf) for User Convenience
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Added "0.0.0.0 your_hostname" entry.
- Improved functionality to allow hostnames to be used without case sensitivity.
- Validation has been added for IP addresses and hostnames; errors will occur if invalid entries are detected.

Improvements to DDL Audit Logs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Added the 'DB Name' to the DDL audit log file generated by CAS, enabling identification of which database the operation was performed on in environments with multiple databases.
- Modified the DDL audit log to record an ABORT log even if the transaction ends without a commit or rollback.
- Improved handling of multiple DDL statements executed in a single transaction with SetAutocommit(false) by ensuring a commit or rollback statement is recorded between commands.

Performance Improvement in REDO Recovery with Parallel Threads
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Applied parallel threads for page-by-page recovery during the REDO recovery process, which does not require synchronization for data consistency, improving recovery performance.
- The improvement in performance is most noticeable when the REDO operation takes up a significant portion of the recovery process and the parallel index is high.

Simplified Lock Information Logged in **dbname_latest.event** File During **Deadlock**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- When a deadlock occurs, the server log file (dbname_latest.event) now **only logs the lock information directly contributing to the deadlock**\, simplifying the output.

Performance Improvement in time_format() and date_format()
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Removed unnecessary string operation function calls to improve performance.

Query Performance Improvement with TRACE Enabled
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- When executing a query with ``set trace on``, the performance was significantly worse compared to executing the same query with ``set trace off``. This has been improved by reducing high-cost operations caused by TRACE's statistics gathering overhead, minimizing the performance degradation.

Performance Improvement by Removing Unnecessary Length Checks on String Types
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Memory Usage Improvement by Limiting Lock Resource Retention to **lock_escalation** Setting
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Improved User Awareness with Error Output for Invalid **optimization_level** Setting
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Improvement in Checking DB Volume and Log Creation Times
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- The ``SHOW VOLUME HEADER``, ``SHOW LOG HEADER``, and ``SHOW ARCHIVE LOG HEADER`` commands have been enhanced to display the **Creation_time**\, allowing users to easily check the volume creation time.
- The ``diagdb`` utility has been improved to allow users to view the creation times of each volume.
- Modification to applyinfo Utility to Display added Volume Creation Time in Log Volume Header

  - The ``applyinfo`` utility, which displays information about **Active Log Volumes** and **Archived Log Volumes** in an HA environment, has been modified to also display the creation time of each volume by adding a **volume creation time(Vol creation time)** to the log volume header.

Improvement in User Awareness by Adding Fence Key Information to **SHOW INDEX CAPACITY** and ``diagdb``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Information regarding the fence key used during leaf node key compression is now displayed separately from general key information, improving user awareness.

.. _11_4_changes_bugfix:

Bug Fixes
---------

SQL
~~~

Fixed for referencing a remote server object without specifying the user schema could fail to resolve to the correct object when an object with the same name exists in another user account
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Hidden Column Handling in Scalar Subqueries with ORDER BY
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for View Merging of Inline Views and Scalar Subqueries Containing ORDER BY clauses, the ROWNUM was Incorrectly Rewritten as ORDER_BY_NUM()
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for **oracle_style_empty_string=yes** is set, the result of the expression ‘NULL || string’ was incorrectly returned as NULL
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for NULL Output in REPLACE Function When **oracle_style_empty_string=yes**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Incorrect Results in Covered Index Scans Using Function Indexes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Segment Fault Errors in CTE Queries with Always False (or NULL) Conditions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Segment Fault Errors in UNION ALL Queries Using Multiple Tables and Serial Next Values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Segment Fault Errors in Queries Using Constants Over 255 Characters Without Aliases
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Errors During View Merging with OUTER JOIN
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Displaying Both User and Public Views with the Same Name in **SHOW CREATE VIEW**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Missing Error Message and NULL Return in repeat() Function Exceeding string_max_size_bytes Setting
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Incorrect Table Positioning During INNER JOIN Removal in Query Optimization
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Errors with Host Variables in PREPARE Statement Using Certain Built-in Functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Missing User Information Deletion in System Catalog Tables When Dropping Users by ``DROP USER`` clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Oracle-Style LEFT OUTER JOIN Not Rewriting to INNER JOIN When Using Host Variables in SP Call or WHERE Clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Incorrect Results in Subquery with **ORDERBY_NUM()** During View Merging
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Tables Without Join Relations Being Recognized as Join Tables During Query Optimization
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for ROWNUM Value Being Printed as 0 When Using Equal Condition (=) in WHERE Clause and the Same Column in ORDER BY
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Errors with INNER JOIN and Oracle-Style OUTER JOIN Used Together and JOIN Condition Placement in WHERE Clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Incorrect Results in **INSERT INTO tbl … SELECT … FROM View … ON DUPLICATE KEY UPDATEI** Query
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Incorrect Column Order Numbers in Analytical Functions Inside Views
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Not Recognizing Range Conditions as Range Items When Using Pipe Operator (||)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

PL(JavaSP)
~~~~~~~~~~

Fixed for Error When Using DATETIMELTZ as a DATETIME Parameter in JavaSP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Error Due to Space Between Declared Java Method Name and Opening Parenthesis in JavaSP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Error When Using Columns of Tables with Unsupported Types as Arguments in JavaSP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Error **Cannot allocate query entry any more** in JavaSP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Utility
~~~~~~~

Fixed for Exit Code Not Being Set to 3 When an Error Occurs During ``loaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Incorrect **count(*)** Value After Executing ``loaddb`` with **--no-logging**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Error Occurring When Processing Serial Without Schema Name During ``loaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for ``loaddb`` Continuing Execution Even After an Error Occurs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Error Occurring When Processing Synonym for System Tables During ``loaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Schema Name Being Incorrectly Stored or Missing for Trigger System Table Columns(condition, action_definition) During ``loaddb`` Execution Using unloaddb Files from Version 11.4 and Below
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Missing Reverse Unique Index Comments During ``unloaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for **auto_increment** Value Being Reset to 1 for PK Columns with auto_increment Set During ``unloaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for **dbname_schema_uk** File Being Created When No Unique Index Exists During ``unloaddb`` **--split-schema-files** Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Missing ALTER SERIAL Statement When Executing ``unloaddb`` with **-i (--input-class-only)** Option
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Error Occurring When current_val and max_val of a Serial Are Identical During ``unloaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Infinite Loop When Processing JSON Data Over 1MB During ``unloaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for executing unloaddb as a regular user, schema names identical to the user were not removed from the query_spec of views
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Missing Schema Names for Serial and Trigger When Executing ``unloaddb`` as **DBA** User
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for unloaddb as a DBA user without the --as-dba option, procedure privileges granted by other users (e.g., GRANT ... ON PROCEDURE) were incorrectly included in the output
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for unloaddb as a regular user, schema names identical to the user were not removed from the condition and action_definition of triggers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Unnecessary Archive Log Volumes Being Created During ``backupdb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Broker,CAS, CMS
~~~~~~~~~~~~~~~

Fixed for Potential Memory Leak When Processing addBatch() and executeBatch()
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

HA
~~

Fixed for Replication Logs Not Being Deleted on Replica Node When ha_replica_delay Is Set to 60 Seconds or More in an HA Environment with Master, Slave, and Replica Nodes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for SQL Log File Not Being Automatically Deleted Under Certain Conditions During ``applylogdb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
