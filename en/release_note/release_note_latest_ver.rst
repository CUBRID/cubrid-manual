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

- HASH JOIN is now supported—a type of join processing method that creates a hash table based on the join condition column to efficiently execute the join.
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
  - If multiple LEADING hints are used, only the first one is applied.
  - The hint is ignored if join graph dependencies prevent early joining of specified tables.

Utility
~~~~~~~

Added Option to Delete Specific Plans in ``cubrid plandump``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Option: *-s*

Added Support for Separate loadjava Option for JNI-Based JavaSP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Option: *-j* or *--jni*
- Not using the new ``loadjava -j`` option will now result in an error when a JNI-based JavaSP is executed.

Added Memory Monitoring Featured utility
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- The memory monitoring utility ``cubrid memmon`` retrieves and displays the following heap memory information from the server process (*cub_server*) memory management module.

  - Total heap memory usage allocated and currently in use by the server process.
  - Detailed memory allocation information based on the source code and line number where the memory allocation request was made.
   
- To use the server process memory management module, the system parameter enable_memory_monitoring must be set to "yes", and the server needs to be restarted.
- For detailed usage and instructions, please refer to the manual. (:ref:`memmon`\)

Added support for the ``diagdb -d 9`` option to dump data from a specific class (table)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Option: -n class-name 
- In the case of partitioned tables, all partitions are dumped. For sub-partitioned tables, only the corresponding partition table is dumped.

Broker,CAS,CMS
~~~~~~~~~~~~~~

Added Parameter to Set the Size of Data Sent to Clients in the Broker
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

   - If a broker is not specified in the ACCESS_CONTROL_FILE, and the default policy is ALLOW, all clients are allowed to connect; if the policy is DENY, connections are denied.

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

- When data recovery from a backup is needed on a production server, the ``restore_to_newdb.sh`` script allows restoring the backup as a new database without the need for additional steps such as creating new accounts or installing extra engine instances. For detailed usage and instructions, please refer to the manual (:ref:`restore_to_newdb_sh`\)

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

  - Index scans are now supported.
  - Unnecessary joins have been eliminated.
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

- Multi-threading has been implemented to significantly reduce data extraction time and improve overall performance.

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

Improvement to CMS getlogfileinfo() API to Return SQL Log File Information Only Once
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

Improvement in User Awareness by Adding Fence Key Information to **SHOW INDEX CAPACITY** and ``diagdb``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Information regarding the fence key used during leaf node key compression is now displayed separately from general key information, improving user awareness.

.. _11_4_changes_bugfix:

Bug Fixes
---------

SQL
~~~

Fixed an issue where referencing a remote server object without specifying the user schema could fail to resolve to the correct object if an object with the same name exists in another user.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Hidden Column Handling in Scalar Subqueries with ORDER BY
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed an issue where View Merging of Inline Views and Scalar Subqueries Containing ORDER BY Clauses, Where ROWNUM Was Incorrectly Rewritten as ORDER_BY_NUM()
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Corrected an Issue When **oracle_style_empty_string=yes** is Set and The Result of The Expression ‘null || string’ Was Incorrectly Returned as NULL
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed NULL Output in REPLACE Function When **oracle_style_empty_string=yes**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Incorrect Results in Covered Index Scans Using Function Indexes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Segment Fault Errors in CTE Queries with Always False (or NULL) Conditions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Segment Fault Errors in UNION ALL Queries Using Multiple Tables and Serial Next Values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Segment Fault Errors in Queries Using Constants Over 255 Characters Without Aliases
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Errors During View Merging with OUTER JOIN
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Displaying Both User and Public Views With the Same Name in **SHOW CREATE VIEW**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Missing Error Message and NULL Return in repeat() Function Exceeding string_max_size_bytes Setting
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Incorrect Table Positioning During INNER JOIN Removal in Query Optimization
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Errors with Host Variables in PREPARE Statements Using Certain Built-in Functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Missing User Information Deletion in System Catalog Tables When Dropping Users by The ``DROP USER`` Clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Oracle-Style LEFT OUTER JOIN Not Rewriting to INNER JOIN When Using Host Variables in SP Call or WHERE Clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Incorrect Results in Subquery with **ORDERBY_NUM()** During View Merging
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Tables Without Join Relations Being Recognized as Join Tables During Query Optimization
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed ROWNUM Values Being Printed as 0 When Using Equal Condition (=) in WHERE Clause and the Same Column in ORDER BY
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Errors That Occur When INNER JOIN and Oracle-Style OUTER JOIN Are Used Together and JOIN Condition Placement in WHERE Clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Corrected Incorrect Results in **INSERT INTO tbl … SELECT … FROM View … ON DUPLICATE KEY UPDATEI** Query
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Corrected Incorrect Column Order Numbers in Analytical Functions Inside Views
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Resolved a Case Where Range Conditions Were Not Recognized as Range Items When Using The Pipe Operator (||)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed An Error That Occurred When Handling NULL in Host Variables of DBLINK Statements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Issue Where Korean Characters Were Corrupted When Inserting via Host Variables Using DBLINK on EUC-KR Oracle DB
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

PL(JavaSP)
~~~~~~~~~~

Resolved an Issue With Using DATETIMELTZ as a DATETIME Parameter in JavaSP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed An Error That Occured Due to Space Between Declared Java Method Name and Opening Parenthesis in JavaSP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed an Error that Occurred When Using Columns of Tables with Unsupported Types as Arguments in JavaSP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed The **Cannot allocate query entry any more** Error in JavaSP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Utility
~~~~~~~

Fixed The Exit Code Not Being Set to 3 When an Error Occurs During ``loaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Incorrect **count(*)** Value Returned After Executing ``loaddb`` with **--no-logging**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed Error That Occurred When Processing Serials Without Schema Names During ``loaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed an Issue Where loaddb Would Continue Execution Even After an Error Occurred
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Resolved an Error That Occurred When Processing a Synonym for System Tables During ``loaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Corrected an Issue Where the Schema Name Was Incorrectly Stored or Missing for Trigger System-Table Columns (condition, action_definition) During ``loaddb`` Execution Using unloaddb Files From Version 11.4 or Earlier
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Addressed a Case Where Reverse Unique-Index Comments Were Missing During ``unloaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Corrected an Issue Where the **auto_increment** Value Was Reset to 1 for PK Columns With **auto_increment** Set During ``unloaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Resolved an Issue Where the **dbname_schema_uk** File Was Created Even When No Unique Index Existed During ``unloaddb`` **--split-schema-files** Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Addressed a Case Where the **ALTER SERIAL** Statement Was Missing When Executing ``unloaddb`` With the **-i (--input-class-only)** Option
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Resolved an Error That Occurred When current_val and max_val of a Serial Were Identical During ``unloaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Corrected an Issue Where an Infinite Loop Occurred When Processing JSON Data Over 1MB During ``unloaddb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Addressed a Case Where, When Running ``unloaddb`` as a Regular User, Schema Names Identical to the User Were Not Removed From the query_spec of Views  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Resolved a Case Where Schema Names for Serials and Triggers Were Missing When Executing ``unloaddb`` as **DBA** User
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Corrected an Issue That Occured When Running ``unloaddb`` as a DBA User Without the --as-dba Option, Where Procedure Privileges Granted by Other Users (e.g., GRANT ... ON PROCEDURE) Were Incorrectly Included in the Output
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Corrected an Issue That Occured When Running ``unloaddb`` as a Regular User, Where Schema Names Identical to the User Were Not Removed From the Condition and action_definition of Triggers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed for Unnecessary Archive Log Volumes Being Created During ``backupdb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fixed an Issue Where ``flashback`` Did Not Terminate Properly on Forced Exit (Ctrl+C)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Resolved a Case Where ``backupdb`` Could Hang in Certain Situations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Broker,CAS, CMS
~~~~~~~~~~~~~~~

Fixed a Potential Memory Leak In the Processing of `addBatch()` and `executeBatch()` in Broker/CAS/CMS
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Corrected an Incorrect Output From the `getTransactionInfo()` Function Call
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

HA
~~

Addressed a Case Where Replication Logs Were Not Deleted on a Replica Node When `ha_replica_delay` Was Set to 60 Seconds or More in an HA Environment With Master, Slave, and Replica Nodes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Resolved an Issue Where the SQL Log File Was Not Automatically Deleted Under Certain Conditions During ``applylogdb`` Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _11_4_cautions:

Cautions
========

.. _new_cautions:

New Cautions
------------

The database volume of CUBRID 11.4 is not compatible with that of CUBRID 11.3 and earlier versions.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*  Due to the addition of new features, system catalog information has been modified or added (:ref:`catalog`). For other new precautions, please refer to :ref:`11_4_changes_spec`.

Existing Cautions
-----------------

.. _11_3_cautions:

11.3
~~~~

The query optimizer's statistical information is not automatically updated when DDL statement is executed, the user must manually execute UPDATE STATISTICS statement to update the statistical information. (see :doc:`/sql/tuning`)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When changing the type of a column included the AUTO_INCREMENT property or default value in the ALTER TABLE statement, an error occurs if the changed type cannot have the AUTO_INCREMENT property or is a type whose existing default value cannot be changed. (see :ref:`change-column`)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Unable to get Connection object through cubrid.jdbc.driver.CUBRIDDriver.getDefaultConnection() from server-side JDBC. Instead, you should use DriverManager.getConnection("jdbc\:default\:connection\:"). (see :ref:`jsp-server-side-jdbc-connection`)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If an invalid index name or table name is used in a USE INDEX (USING INDEX) statement, it will be ignored rather than occuring an error. (To leave it in the log file, set the error_log warning parameter to yes.)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When writing a CREATE INDEX statement, the position of COMMENT was changed to the end of the statement after the WITH or INVISIBLE clause. (see :doc:`/sql/schema/index_stmt`)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _11_2_cautions:

11.2
~~~~

The database volume of CUBRID 11.2 is not compatible with that of CUBRID 11.1 and earlier versions.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By introducing the concept of user schema, the same object name can be used for each user, and the behavior is changed as follows
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

 * "." (dot) is not allowed in the object name.
 * When using a query or utility command, it must be used as "[user name].object name". (However, the user name can be omitted when querying the object of the logged-in user) (see :doc:`/sql/user_schema`)
 * Changed to include user name in info schema and show full tables results. (see :doc:`/sql/query/show`)
 * The loaddb file prior to 11.2 must be modified to "user name.table name" so that it can be executed in 11.2, or loaddb can be executed by setting the \-\-no-user-specified-name option. (see :ref:`loaddb`)

The following functions and behavior changed when using "jdbc\:default\:connection\:" or calling getDefaultConnection() in JavaSP. (see :ref:`JavaSP Caution <jsp-caution>`)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

 * All functions of java.sql.DatabaseMetaData are not supported.
 * createClob() and createBlob() of java.sql.Connection are not supported.
 * addBatch(), clearBatch(), executeBatch(), setMaxRows() and cancel() of java.sql.Statement are not supported.
 * Multiple SQL is not supported for one prepare (or execute).
 * The cursor is changed to non-holdable.
 * The ResultSet is changed to non-scrollable, non-sensitive and non-updatable.

The behavior of the TRUNCATE TABLE changed if there is set null or cascade of FK (see :doc:`/sql/query/truncate`)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Column properties not written during alter change/modify are changed to be maintained, and auto_increment and on update properties cannot be removed with the alter statement (see :ref:`change-column`)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Changed to handle an error if only the column name exists in the where clause
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

 * If used in the form of UPDATE t1 SET c1 = 9 WHERE c1; , an error occurs.

Multiple SQL must be separated by semicolons
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The CCI Driver directory in the CUBRID package is changed from $CUBRID/lib and $CUBRID/include to $CUBRID/cci/lib and $CUBRID/cci/include, respectively (see :ref:`CCI Overview <cci-overview>`)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* When using CCI, $CUBRID/cci/lib must be added to LD_LIBRARY_PATH in the environment variable.

Changed Compression (-z, \-\-compress) option to default on backup (see :ref:`backupdb`)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* System catalog information changed or added due to the addition of new features (see :doc:`/sql/catalog`)

.. _11_0_cautions:

11.0
~~~~


The database volume of CUBRID 11.0 is not compatible with that of CUBRID 10.2 and earlier versions.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When creating a table without an option, it is created as a reuse_oid table.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The maximum length of the CHAR data type has been changed to 256M character string.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modified to occur error when the input string length is longer than the set length of the string data type.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modified to recognize the space character at the end of the string, it is recognized as a different character string according to the space character at the end of the string.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Due to the change in the statistics collection method, it is necessary to perform periodic statistics collection.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _10_2_cautions:

10.2
~~~~

The database volume of CUBRID 10.2 is not compatible with that of CUBRID 10.1 and earlier versions.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _10_1_and_old_cautions:

10.1
~~~~

The database volume of CUBRID 10.1 is not compatible with that of CUBRID 10.0 and earlier versions.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Version 10.0 and earlier
~~~~~~~~~~~~~~~~~~~~~~~~

The database volume of CUBRID 10.0 is not compatible with that of CUBRID 9.3 and earlier versions.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Locale(language and charset) is specified when creating DB
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* It is changed as locale is specified when creating DB.
   
CUBRID_CHARSET environment variable is removed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* As locale(language and charset) is specified when creating DB from 9.2 version, CUBRID_CHARSET is not used anymore.

.. 4.4new

[JDBC] Change zero date of TIMESTAMP into '1970-01-01 00:00:00'(GST) from '0001-01-01 00:00:00' when the value of zeroDateTimeBehavior in the connection URL is "round"(CUBRIDSUS-11612)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
* From 2008 R4.4, when the value of the property "zeroDateTimeBehavior" in the connection URL is "round", the  zero date value of TIMESTAMP is changed into '1970-01-01 00:00:00'(GST) from '0001-01-01 00:00:00'. You should be cautious when using zero date in your application.


Recommendation for installing CUBRID SH package in AIX(CUBRIDSUS-12251)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* If you install CUBRID SH package by using ksh in AIX OS, it fails with the following error. 
  
:: 
  
    0403-065 An incomplete or invalid multibyte character encountered. 
  
* Therefore, it is recommended to use ksh93 or bash instead of ksh.
  
:: 
  
    $ ksh93 ./CUBRID-9.2.0.0146-AIX-ppc64.sh 
    $ bash ./CUBRID-9.2.0.0146-AIX-ppc64.sh 

CUBRID_LANG is removed, CUBRID_MSG_LANG is added
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* From version 9.1, CUBRID_LANG environment variable is no longer used.

* To output the utility message and the error message, the CUBRID_MSG_LANG environment variable is used. 


Modify how to process an error for the array of the result of executing several queries at once in the CCI application(CUBRIDSUS-9364)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* When executing several queries at once in the CCI application, if an error has occurs from at least one query among the results of executing queries by using the cci_execute_array function, the cci_execute_batch function, the error code of the corresponding query was returned from 2008 R3.0 to 2008 R4.1. This problem has been fixed to return the number of the entire queries and check the error of each query by using the CCI_QUERY_RESULT_* macros from 2008 R4.3 and 9.1.

* In earlier versions of this modification, there is no way to know whether each query in the array is success or failure when an error occurs; therefore, it it requires certain conditions.

.. code-block:: c

    ...
    char *query = "INSERT INTO test_data (id, ndata, cdata, sdata, ldata) VALUES (?, ?, 'A', 'ABCD', 1234)";
    ...
    req = cci_prepare (con, query, 0, &cci_error);
    ...
    error = cci_bind_param_array_size (req, 3);
    ...
    error = cci_bind_param_array (req, 1, CCI_A_TYPE_INT, co_ex, null_ind, CCI_U_TYPE_INT);
    ...
    n_executed = cci_execute_array (req, &result, &cci_error);

    if (n_executed < 0)
      {
        printf ("execute error: %d, %s\n", cci_error.err_code, cci_error.err_msg);

        for (i = 1; i <= 3; i++)
          {
            printf ("query %d\n", i);
            printf ("result count = %d\n", CCI_QUERY_RESULT_RESULT (result, i));
            printf ("error message = %s\n", CCI_QUERY_RESULT_ERR_MSG (result, i));
            printf ("statement type = %d\n", CCI_QUERY_RESULT_STMT_TYPE (result, i));
          }
      }
    ...

* From the modified version, entire queries are regarded as failure if an error occurs. In case that no error occurred, it is determined whether each query in the array succeeds or not.

.. code-block:: c

    ...
    char *query = "INSERT INTO test_data (id, ndata, cdata, sdata, ldata) VALUES (?, ?, 'A', 'ABCD', 1234)";
    ...
    req = cci_prepare (con, query, 0, &cci_error);
    ...
    error = cci_bind_param_array_size (req, 3);
    ...
    error = cci_bind_param_array (req, 1, CCI_A_TYPE_INT, co_ex, null_ind, CCI_U_TYPE_INT);
    ...
    n_executed = cci_execute_array (req, &result, &cci_error);
    if (n_executed < 0)
      {
        printf ("execute error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
      }
    else
      {
        for (i = 1; i <= 3; i++)
          {
            printf ("query %d\n", i);
            printf ("result count = %d\n", CCI_QUERY_RESULT_RESULT (result, i));
            printf ("error message = %s\n", CCI_QUERY_RESULT_ERR_MSG (result, i));
            printf ("statement type = %d\n", CCI_QUERY_RESULT_STMT_TYPE (result, i));
          }
      }
    ...

In java.sql.XAConnection interface, HOLD_CURSORS_OVER_COMMIT is not supported(CUBRIDSUS-10800)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Current CUBRID does not support ResultSet.HOLD_CURSORS_OVER_COMMIT in java.sql.XAConnection interface.

From 9.0, STRCMP behaves case-sensitively
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Until the previous version of 9.0, STRCMP did not distinguish an uppercase and a lowercase. From 9.0, it compares the strings case-sensitively.

* To make STRCMP case-insensitive, you should use case-insensitive collation(e.g.: utf8_en_ci).

.. code-block:: sql

    -- In previous version of 9.0 STRCMP works case-insensitively
    SELECT STRCMP ('ABC','abc');
    0
    
    -- From 9.0 version, STRCMP distinguish the uppercase and the lowercase when the collation is case-sensitive.
    export CUBRID_CHARSET=en_US.iso88591
    
    SELECT STRCMP ('ABC','abc');
    -1
    
    -- If the collation is case-insensitive, it distinguish the uppercase and the lowercase.
    export CUBRID_CHARSET=en_US.iso88591

    SELECT STRCMP ('ABC' COLLATE utf8_en_ci ,'abc' COLLATE utf8_en_ci);
    0

Since the 2008 R4.1 version, the Default value of CCI_DEFAULT_AUTOCOMMIT has been ON(CUBRIDSUS-5879)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* The default value for the CCI_DEFAULT_AUTOCOMMIT broker parameter, which affects the auto commit mode for applications developed with CCI interface, has been changed to ON since CUBRID 2008 R4.1. As a result of this change, CCI and CCI-based interface (PHP, ODBC, OLE DB etc.) users should check whether or not the application's auto commit mode is suitable for this.

From the 2008 R4.0 version, the options and parameters that use the unit of pages were changed to use the unit of volume size(CUBRIDSUS-5136)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* The options (-p, -l, -s), which use page units to specify the database volume size and log volume size of the cubrid createdb utility, will be removed. Instead, the new options, added after 2008 R4.0 Beta (--db-volume-size, --log-volume-size, --db-page-size, --log-page-size), are used.

* To specify the database volume size of the cubrid addvoldb utility, use the newly-added option (--db-volume-size) after 2008 R4.0 Beta instead of using the page unit.

* It is recommended to use the new system parameters in bytes because the page-unit system parameters will be removed. For details on the related system parameters, see the below.

Be cautious when setting db volume size if you are a user of a version before 2008 R4.0 Beta(CUBRIDSUS-4222)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* From the 2008 R4.0 Beta version, the default value of data page size and log page size in creating the database was changed from 4 KB to 16 KB. If you specify the database volume to the page count, the byte size of the volume may differ from your expectations. If you did not set any options, 100MB-database volume with 4KB-page size was created in the previous version. However, starting from the 2008 R4.0, 512MB-database volume with 16KB-page size is created.

* In addition, the minimum size of the available database volume is limited to 20 MB. Therefore, a database volume less than this size cannot be created.

The change of the default value of some system parameters of the versions before 2008 R4.0(CUBRIDSUS-4095)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Starting from 2008 R4.0, the default values of some system parameters have been changed.

* Now, the default value of max_clients, which specifies the number of concurrent connections allowed by a DB server, and the default value of index_unfill_factor that specifies the ratio of reserved space for future updates while creating an index page, have been changed. Furthermore, the default values of the system parameters in bytes now use more memory when they exceed the default values of the previous system parameters per page.

+-----------------------------+----------------------------+----------------------+--------------------+ 
| Previous System             | Added System               | Previous Default     | Changed Default    | 
| Parameter                   | Parameter                  | Value                | Value (unit: byte) |
|                             |                            |                      |                    | 
+=============================+============================+======================+====================+ 
| max_clients                 | None                       | 50                   | 100                | 
+-----------------------------+----------------------------+----------------------+--------------------+ 
| index_unfill_factor         | None                       | 0.2                  | 0.05               | 
+-----------------------------+----------------------------+----------------------+--------------------+
| data_buffer_pages           | data_buffer_size           | 100M(page size=4K)   | 512M               | 
+-----------------------------+----------------------------+----------------------+--------------------+
| log_buffer_pages            | log_buffer_size            | 200K(page size=4K)   | 4M                 | 
|                             |                            |                      |                    |
+-----------------------------+----------------------------+----------------------+--------------------+
| sort_buffer_pages           | sort_buffer_size           | 64K(page size=4K)    | 2M                 | 
|                             |                            |                      |                    | 
+-----------------------------+----------------------------+----------------------+--------------------+
| index_scan_oid_buffer_pages | index_scan_oid_buffer_size | 16K(page size=4K)    | 64K                | 
|                             |                            |                      |                    | 
+-----------------------------+----------------------------+----------------------+--------------------+

* In addition, when a database is created using cubrid createdb, the minimum value of the data page size and the log page size has been changed from 1K to 4K.
 
Changed so that database services, utilities, and applications cannot be executed when the system parameter is incorrectly configured(CUBRIDSUS-5375)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* It has been changed so that now the related database services, utilities, and applications are not executed when configuring system parameters that are not defined in cubrid.conf or cubrid_ha.conf, when the value of system parameters exceed the threshold, or when the system parameters per page and the system parameters in bytes are used simultaneously.

Database fails to start if the data_buffer_size is configured with a value that exceeds 2G in CUBRID 32-bit version(CUBRIDSUS-5349)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* In the CUBRID 32-bit version, if the value of data_buffer_size exceeds 2G, the running database fails. Note that the configuration value cannot exceed 2G in the 32-bit version because of the OS limit.

Recommendations for controlling services with the CUBRID Utility in Windows Vista and higher(CUBRIDSUS-4186)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* To control services using cubrid utility from Windows Vista and higher, it is recommended to start the command prompt window with administrative privileges.

* If you don't start the command prompt window with administrative privileges and use the cubrid utility, you can still execute it with administrative privileges through the User Account Control (UAC) dialog box, but you will not be able to verify the resulting messages.

The procedures for starting the command prompt window as an administrator in Windows Vista and higher are as follows:

* Right-click [Start > All Programs > Accessories > Command Prompt].
* When [Execute as an administrator (A)] is selected, a dialog box to verify the privilege escalation is activated. Click “YES" to start with administrative privileges.
    
A manager server process-related error occurs in the execution of the CUBRID source after its build(CUBRIDSUS-3553)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    
* If users want to build the CUBRID source and install it themselves, they must build and install CUBRID and the CUBRID Manager respectively. If you check out only CUBRID source and run cubrid service start or cubrid manager start after build, the error "cubrid manager server is not installed" will occur.


GLO class which is used in 2008 r3.0 or before is not supported any longer(CUBRIDSUS-3826)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* CUBRID 2008 R3.0 and earlier versions processed Large Objects with the Generalized Large Object glo class, but the glo class has been removed from CUBRID 2008 R3.1 and later versions. Instead, they support BLOB and CLOB (LOB from this point forward) data types. (See :ref:`blob-clob` for more information about LOB data types).

* glo class users are recommended to carry out tasks as follows:

* After saving GLO data as a file, modify to not use GLO in any application and DB schema.
* Implement DB migration by using the unloaddb and loaddb utilities.
* Perform tasks to load files into LOB data according to the modified application.
* Verify the application that you modified operates normally.

* For reference, if the cubrid loaddb utility loads a table that inherits the GLO class or has the GLO class type, it stops the data from loading by displaying an error message, "Error occurred during schema loading."

* With the discontinued support of GLO class, the deleted functions for each interface are as follows:

+------------+----------------------------+
| Interface  | Deleted Functions          |
+============+============================+
| CCI        | cci_glo_append_data        |
|            |                            |
|            | cci_glo_compress_data      |
|            |                            |
|            | cci_glo_data_size          |
|            |                            |
|            | cci_glo_delete_data        |
|            |                            |
|            | cci_glo_destroy_data       |
|            |                            |
|            | cci_glo_insert_data        |
|            |                            |
|            | cci_glo_load               |
|            |                            |
|            | cci_glo_new                |
|            |                            |
|            | cci_glo_read_data          |
|            |                            |
|            | cci_glo_save               |
|            |                            |
|            | cci_glo_truncate_data      |
|            |                            |
|            | cci_glo_write_data         |
|            |                            |
+------------+----------------------------+
| JDBC       | CUBRIDConnection.getNewGLO |
|            |                            |
|            | CUBRIDOID.loadGLO          |
|            |                            |
|            | CUBRIDOID.saveGLO          |
|            |                            |
+------------+----------------------------+
| PHP        | cubrid_new_glo             |
|            |                            |
|            | cubrid_save_to_glo         |
|            |                            |
|            | cubrid_load_from_glo       |
|            |                            |
|            | cubrid_send_glo            |
|            |                            |
+------------+----------------------------+

Port configuration is required if the protocol between the master and server processes is changed, or if two versions are running at the same time(CUBRIDSUS-3564)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Because the communication protocol between a master process (cub_master) and a server process (cub_server) has been changed, the master process of CUBRID 2008 R3.0 or later cannot communicate with the server process of a lower version, and the master process of a lower version cannot communicate with a server process of 2008 R3.0 version or later. Therefore, if you run two versions of CUBRID at the same time by adding a new version in an environment where a lower version has already been installed, you should modify the cubrid_port_id system parameter of cubrid.conf so that different ports are used by the different versions.

Specifying a question mark when entering connection information as a URL string in JDBC(CUBRIDSUS-3217)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* When entering connection information as a URL string in JDBC, property information was applied even if you did not enter a question mark (?) in the earlier version. However, you must specify a question mark depending on syntax in this CUBRID 2008 R3.0 version. If not, an error is displayed. In addition, you must specify colon (:) even if there is no username or password in the connection information. ::

    URL=jdbc:CUBRID:127.0.0.1:31000:db1:::altHosts=127.0.0.2:31000,127.0.0.3:31000 -- Error
    URL=jdbc:CUBRID:127.0.0.1:31000:db1:::?altHosts=127.0.0.2:31000,127.0.0.3:31000 -- Normal

Not allowed to include @ in a database name(CUBRIDSUS-2828)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* If @ is included in a database name, it can be interpreted that a host name has been specified. To prevent this, a revision has been made so that @ cannot be included in a database name when running cubrid createdb, cubrid renamedb and cubrid copydb utilities.
