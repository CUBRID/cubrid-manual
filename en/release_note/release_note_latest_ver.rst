:tocdepth: 3

******************
11.3 Release Notes
******************

.. contents::

Release Notes Information
=========================

This document includes information on CUBRID 11.3.

CUBRID 11.3 includes all of the fixed errors and improved features that were detected in the CUBRID 11.2 and were applied to the previous versions.

For CUBRID 11.2, please find https://www.cubrid.org/manual/en/11.2/release_note/index.html.

For CUBRID 11.0, please find https://www.cubrid.org/manual/en/11.0/release_note/index.html.

For CUBRID 10.2, please find https://www.cubrid.org/manual/en/10.2/release_note/index.html.

For CUBRID 10.1, please find https://www.cubrid.org/manual/en/10.1/release_note/index.html.

For CUBRID 10.0, please find https://www.cubrid.org/manual/en/10.0/release_note/index.html.

For CUBRID 9.3, please find https://www.cubrid.org/manual/en/9.3.0/release_note/index.html.

Overview
========

CUBRID 11.3 is the latest stable version that includes new features, significant changes and enhancements.

.. TODO: UPDATE WITH DETAILS.

CUBRID 11.3

* improves connectivity by enhancing database links.
* is more stable, faster, and more convenient for administrators.
* fixes a large number of critical bugs.
* includes code refactoring and modernization.

CUBRID 11.3 provided user convenience by providing a table extension style (object@server) for database links, and **improved connectivity** by providing the ability to insert, modify, and delete data as well as retrieve data for remote DB.

CUBRID 11.3 is **faster**. This version further improves performance through improve aggregation functions (min, max group_concat) and query optimization, such as predicate pushdown, view transformation, and removal of unnecessary join tables. Additionally, performance problems caused by skewed indexes were improved by providing a deduplication option when creating an index (or foreign key).

CUBRID 11.3 **improved administrator convenience** by improving the csql, unloaddb, and loaddb utilities.

The database volume of CUBRID 11.3 is compatible with the CUBRID 11.2 volume. (However, when a volume created in CUBRID 11.2 is used in CUBRID 11.3 due to a change in view query spec of db_index and db_vclass among catalog views, unexpected results may be obtained when executing a query related the catalog view.)

.. TODO: coming soon 

Driver Compatibility
--------------------

*   The JDBC and CCI driver of CUBRID 11.3 are compatible with the DB server of CUBRID 11.2, 11.1, 11.0, 10.2, 10.1, 10.0, 9.3, 9.2, 9.1, 2008 R4.4, R4.3 or R4.1.
*   To upgrade drivers are highly recommended.

For more details on changes, see the :ref:`11_3_changes`. Users of previous versions should check the :ref:`11_3_changes` and :ref:`11_3_new_cautions` sections.

.. _11_3_changes:

11.3 Changes
============

Please refer to `change logs of CUBRID 11.3 <https://github.com/CUBRID/cubrid/releases/tag/v11.3>`_.


Cautions
========

.. _11_3_new_cautions:

New Cautions
------------

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

Existing Cautions
-----------------

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
System catalog information changed or added due to the addition of new features (see :doc:`/sql/catalog`)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
When creating a table without an option, it is created as a reuse_oid table
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The maximum length of the CHAR data type has been changed to 256M character string
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Modified to occur error when the input string length is longer than the set length of the string data type
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Modified to recognize the space character at the end of the string, it is recognized as a different character string according to the space character at the end of the string
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Due to the change in the statistics collection method, it is necessary to perform periodic statistics collection
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Locale(language and charset) is specified when creating DB
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

It is changed as locale is specified when creating DB.
   
CUBRID_CHARSET environment variable is removed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As locale(language and charset) is specified when creating DB from 9.2 version, CUBRID_CHARSET is not used anymore.

.. 4.4new

[JDBC] Change zero date of TIMESTAMP into '1970-01-01 00:00:00'(GST) from '0001-01-01 00:00:00' when the value of zeroDateTimeBehavior in the connection URL is "round"(CUBRIDSUS-11612)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
From 2008 R4.4, when the value of the property "zeroDateTimeBehavior" in the connection URL is "round", the  zero date value of TIMESTAMP is changed into '1970-01-01 00:00:00'(GST) from '0001-01-01 00:00:00'. You should be cautious when using zero date in your application.


Recommendation for installing CUBRID SH package in AIX(CUBRIDSUS-12251)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you install CUBRID SH package by using ksh in AIX OS, it fails with the following error. 
  
:: 
  
    0403-065 An incomplete or invalid multibyte character encountered. 
  
Therefore, it is recommended to use ksh93 or bash instead of ksh.
  
:: 
  
    $ ksh93 ./CUBRID-9.2.0.0146-AIX-ppc64.sh 
    $ bash ./CUBRID-9.2.0.0146-AIX-ppc64.sh 

CUBRID_LANG is removed, CUBRID_MSG_LANG is added
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

From version 9.1, CUBRID_LANG environment variable is no longer used.
To output the utility message and the error message, the CUBRID_MSG_LANG environment variable is used. 


Modify how to process an error for the array of the result of executing several queries at once in the CCI application(CUBRIDSUS-9364)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When executing several queries at once in the CCI application, if an error has occurs from at least one query among the results of executing queries by using the cci_execute_array function, the cci_execute_batch function, the error code of the corresponding query was returned from 2008 R3.0 to 2008 R4.1. This problem has been fixed to return the number of the entire queries and check the error of each query by using the CCI_QUERY_RESULT_* macros from 2008 R4.3 and 9.1.

In earlier versions of this modification, there is no way to know whether each query in the array is success or failure when an error occurs; therefore, it it requires certain conditions.

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

From the modified version, entire queries are regarded as failure if an error occurs. In case that no error occurred, it is determined whether each query in the array succeeds or not.

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

Current CUBRID does not support ResultSet.HOLD_CURSORS_OVER_COMMIT in java.sql.XAConnection interface.

From 9.0, STRCMP behaves case-sensitively
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Until the previous version of 9.0, STRCMP did not distinguish an uppercase and a lowercase. From 9.0, it compares the strings case-sensitively.
To make STRCMP case-insensitive, you should use case-insensitive collation(e.g.: utf8_en_ci).

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

The default value for the CCI_DEFAULT_AUTOCOMMIT broker parameter, which affects the auto commit mode for applications developed with CCI interface, has been changed to ON since CUBRID 2008 R4.1. As a result of this change, CCI and CCI-based interface (PHP, ODBC, OLE DB etc.) users should check whether or not the application's auto commit mode is suitable for this.

From the 2008 R4.0 version, the options and parameters that use the unit of pages were changed to use the unit of volume size(CUBRIDSUS-5136)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The options (-p, -l, -s), which use page units to specify the database volume size and log volume size of the cubrid createdb utility, will be removed. Instead, the new options, added after 2008 R4.0 Beta (\-\-db-volume-size, \-\-log-volume-size, \-\-db-page-size, \-\-log-page-size), are used.

To specify the database volume size of the cubrid addvoldb utility, use the newly-added option (\-\-db-volume-size) after 2008 R4.0 Beta instead of using the page unit.
It is recommended to use the new system parameters in bytes because the page-unit system parameters will be removed. For details on the related system parameters, see the below.

Be cautious when setting db volume size if you are a user of a version before 2008 R4.0 Beta(CUBRIDSUS-4222)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

From the 2008 R4.0 Beta version, the default value of data page size and log page size in creating the database was changed from 4 KB to 16 KB. If you specify the database volume to the page count, the byte size of the volume may differ from your expectations. If you did not set any options, 100MB-database volume with 4KB-page size was created in the previous version. However, starting from the 2008 R4.0, 512MB-database volume with 16KB-page size is created.

In addition, the minimum size of the available database volume is limited to 20 MB. Therefore, a database volume less than this size cannot be created.

The change of the default value of some system parameters of the versions before 2008 R4.0(CUBRIDSUS-4095)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Starting from 2008 R4.0, the default values of some system parameters have been changed.

Now, the default value of max_clients, which specifies the number of concurrent connections allowed by a DB server, and the default value of index_unfill_factor that specifies the ratio of reserved space for future updates while creating an index page, have been changed. Furthermore, the default values of the system parameters in bytes now use more memory when they exceed the default values of the previous system parameters per page.

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

In addition, when a database is created using cubrid createdb, the minimum value of the data page size and the log page size has been changed from 1K to 4K.
 
Changed so that database services, utilities, and applications cannot be executed when the system parameter is incorrectly configured(CUBRIDSUS-5375)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

It has been changed so that now the related database services, utilities, and applications are not executed when configuring system parameters that are not defined in cubrid.conf or cubrid_ha.conf, when the value of system parameters exceed the threshold, or when the system parameters per page and the system parameters in bytes are used simultaneously.

Database fails to start if the data_buffer_size is configured with a value that exceeds 2G in CUBRID 32-bit version(CUBRIDSUS-5349)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In the CUBRID 32-bit version, if the value of data_buffer_size exceeds 2G, the running database fails. Note that the configuration value cannot exceed 2G in the 32-bit version because of the OS limit.

Recommendations for controlling services with the CUBRID Utility in Windows Vista and higher(CUBRIDSUS-4186)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To control services using cubrid utility from Windows Vista and higher, it is recommended to start the command prompt window with administrative privileges.

If you don't start the command prompt window with administrative privileges and use the cubrid utility, you can still execute it with administrative privileges through the User Account Control (UAC) dialog box, but you will not be able to verify the resulting messages.

The procedures for starting the command prompt window as an administrator in Windows Vista and higher are as follows:

* Right-click [Start > All Programs > Accessories > Command Prompt].
* When [Execute as an administrator (A)] is selected, a dialog box to verify the privilege escalation is activated. Click “YES" to start with administrative privileges.
    
A manager server process-related error occurs in the execution of the CUBRID source after its build(CUBRIDSUS-3553)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    
If users want to build the CUBRID source and install it themselves, they must build and install CUBRID and the CUBRID Manager respectively. If you check out only CUBRID source and run cubrid service start or cubrid manager start after build, the error "cubrid manager server is not installed" will occur.


GLO class which is used in 2008 r3.0 or before is not supported any longer(CUBRIDSUS-3826)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CUBRID 2008 R3.0 and earlier versions processed Large Objects with the Generalized Large Object glo class, but the glo class has been removed from CUBRID 2008 R3.1 and later versions. Instead, they support BLOB and CLOB (LOB from this point forward) data types. (See :ref:`blob-clob` for more information about LOB data types).

glo class users are recommended to carry out tasks as follows:

* After saving GLO data as a file, modify to not use GLO in any application and DB schema.
* Implement DB migration by using the unloaddb and loaddb utilities.
* Perform tasks to load files into LOB data according to the modified application.
* Verify the application that you modified operates normally.

For reference, if the cubrid loaddb utility loads a table that inherits the GLO class or has the GLO class type, it stops the data from loading by displaying an error message, "Error occurred during schema loading."

With the discontinued support of GLO class, the deleted functions for each interface are as follows:

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

Because the communication protocol between a master process (cub_master) and a server process (cub_server) has been changed, the master process of CUBRID 2008 R3.0 or later cannot communicate with the server process of a lower version, and the master process of a lower version cannot communicate with a server process of 2008 R3.0 version or later. Therefore, if you run two versions of CUBRID at the same time by adding a new version in an environment where a lower version has already been installed, you should modify the cubrid_port_id system parameter of cubrid.conf so that different ports are used by the different versions.

Specifying a question mark when entering connection information as a URL string in JDBC(CUBRIDSUS-3217)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When entering connection information as a URL string in JDBC, property information was applied even if you did not enter a question mark (?) in the earlier version. However, you must specify a question mark depending on syntax in this CUBRID 2008 R3.0 version. If not, an error is displayed. In addition, you must specify colon (:) even if there is no username or password in the connection information. ::

    URL=jdbc:CUBRID:127.0.0.1:31000:db1:::altHosts=127.0.0.2:31000,127.0.0.3:31000 -- Error
    URL=jdbc:CUBRID:127.0.0.1:31000:db1:::?altHosts=127.0.0.2:31000,127.0.0.3:31000 -- Normal

Not allowed to include @ in a database name(CUBRIDSUS-2828)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If @ is included in a database name, it can be interpreted that a host name has been specified. To prevent this, a revision has been made so that @ cannot be included in a database name when running cubrid createdb, cubrid renamedb and cubrid copydb utilities.
