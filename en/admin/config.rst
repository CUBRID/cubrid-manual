
:meta-keywords: cubrid configure, cubrid conf, cubrid parameters, cubrid settings, cubrid.conf, cubrid default parameters
:meta-description: How to configure CUBRID database behavior. Set system parameters for Connection, Memory, Disk, Concurrency/Lock, Logging, Transaction Processing, Query Execution, Utilities and High Availability.

*****************
System Parameters
*****************

This chapter provides information about configuring system parameters that can affect the system performance. System parameters determine overall performance and operation of the system. This chapter explains how to use configuration files for database server and broker as well as a description of each parameter. 

.. FIXME: For CUBRID Manager server configuration, see `CUBRID Manager Manual <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual>`_.

This chapter covers the following topics:

*   Configuring the database server
*   Configuring the broker

Configuring the Database Server
===============================

.. _scope-server-conf:

Scope of Database Server Configuration
--------------------------------------

CUBRID consists of the database server, the broker and the CUBRID Manager. Each component has its configuration file. The system parameter configuration file for the database server is **cubrid.conf** located in the **$CUBRID/conf** directory. System parameters configured in **cubrid.conf** affect overall performance and operation of the database system. Therefore, it is very important to understand the database server configuration.

The CUBRID database server has a client/server architecture. To be more specific, it is divided into a database server process linked to the server library and the broker process linked to the client library. The server process manages the database storage structure and provides concurrency and transaction functionalities. The client process prepares for query execution and manages object/schema.

System parameters for the database server, which can be set in the **cubrid.conf** file, are classified into a client parameter, a server parameter and a client/server parameter according to the range to which they are applied. A client parameter is only applied to client processes such as the broker. A server parameter affects the behaviors of the server processes. A client/server parameter must be applied to both server and client.

.. note:: **Location of cubrid.conf File and How It Works**

    The **cubrid.conf** file is located on the **$CUBRID/conf** directory. For setting by database, it divides into a section in the **cubrid.conf** file.

Changing Database Server Configuration
--------------------------------------

Editing the Configuration File
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can add/delete parameters or change parameter values by manually editing the system parameter configuration file (**cubrid.conf**) in the **$CUBRID/conf** directory.

The following parameter syntax rules are applied when configuring parameters in the configuration file:

*   Parameter names are not case-sensitive.
*   The name and value of a parameter must be entered in the same line.
*   An equal sign (=) can be to configure the parameter value. Spaces are allowed before and after the equal sign.
*   If the value of a parameter is a character string, enter the character string without quotes. However, use quotes if spaces are included in the character string.

Using SQL Statements
^^^^^^^^^^^^^^^^^^^^

You can configure a parameter value by using SQL statements in the CSQL Interpreter or CUBRID Manager's Query Editor. Note that you cannot change every parameter. For updatable parameters, see :ref:`cubrid-conf`. ::

    SET SYSTEM PARAMETERS 'parameter_name=value [{; name=value}...]'

*parameter_name* is the name of a client parameter whose value is editable. In this syntax, *value* is the value of the given parameter. You can change multiple parameter values by separating them with a semicolon(;). You should be careful when you change a parameter.

The following example shows how to retrieve the result of an index scan in OID order and configure the number of queries to be stored in the history of the CSQL Interpreter to 70.

.. code-block:: sql

    SET SYSTEM PARAMETERS 'index_scan_in_oid_order=1; csql_history_num=70';

**DEFAULT** for *value* will reset the parameter to its default value with an exception of **call_stack_dump_activation_list** parameter. 

.. code-block:: sql

    SET SYSTEM PARAMETERS 'lock_timeout=DEFAULT';
    
Using Session Commands of the CSQL Interpreter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can configure system parameter values by using session commands (;SET) in the CSQL Interpreter. Note that you cannot change every parameter. For updatable parameters, see :ref:`cubrid-conf`.

The following example shows how to configure the block_ddl_statement parameter to 1 so that execution of DDL statements is not allowed. ::

    csql> ;se block_ddl_statement=1
    === Set Param Input ===
    block_ddl_statement=1

.. _cubrid-conf:

cubrid.conf Configuration File and Default Parameters
-----------------------------------------------------

CUBRID consists of the database server, the broker and the CUBRID Manager. The name of the configuration file for each component is as follows. These files are all located in the **$CUBRID/conf** directory.

*   Database server configuration file: **cubrid.conf**
*   Broker configuration file: **cubrid_broker.conf**
*   CUBRID Manager server configuration file: **cm.conf**

**cubrid.conf** is a configuration file that sets system parameters for the CUBRID database server and determines overall performance and operation of the database system. In the **cubrid.conf** file, some important parameters needed for system installation are provided, having their default values.

Database Server System Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following are database server system parameters that can be used in the **cubrid.conf** configuration file. On the following table, "Applied" column's "client parameter" means that they are applied to CAS, CSQL, **cubrid** utilities. Its "server parameter" means that they are applied to the DB server (cub_server process).
For the scope of **client** and **server parameters**, see :ref:`scope-server-conf`.

You can change the parameters that are capable of changing dynamically the setting value through the **SET SYSTEM PARAMETERS** statement or a session command of the CSQL Interpreter, **;set** while running the DB. If you are a DBA, you can change parameters regardless of the applied classification. However, if you are not a DBA, you can only change "session" parameters. (on the below table, a parameter of which "session" item's value is O.)

On the below table, if "Applied" is "server parameter", that parameter affects to cub_server process; If "client parameter", that parameter affects to CAS, CSQL or "cubrid" utilities which run on client/server mode (\-\-CS-mode). "Client/server parameter" affects to all of cub_server, CAS, CSQL and "cubrid" utilities.

"Dynamic Change" and "Session or not" are marked on the below table. The affected range of the parameter which "Dynamic Change" is "available" depends on "Applied" and "Session" items.

*   If "Dynamic Change" is "available" and "Applied" is "server parameter", that parameter's changed value is applied to DB server. Then applications use the changed value of the parameter until the DB server is restarted.

*   If "Dynamic Change" is "available" and "Applied" is "client parameter", this belongs to the "session" parameter and that parameter's changed value is applied only to that DB session. In other words, the changed value is only applied to the application which requested to change that value. For example, if **block_ddl_statement** parameter's value is changed into **yes**, then only the application requested to change that parameter cannot use DDL statements.

*   If "Dynamic Change" is "available", "Applied" is "client parameter" and;

    *   this belongs to the "session" parameter, that parameter's changed value is applied only to that DB session. In other words, the changed value is only applied to the application requested to change that value. For example, if **add_column_update_hard_default** parameter's value is changed into **yes**, then only the application requested to change that parameter lets the newly added column with NOT NULL constraint have hard default value.
    
    *   this does not belong to the "session" parameter, the values of "client" side and "server" side are changed. For example, **error_log_level** parameter is applied to each of "server" side and "client" side; if this value is changed from "ERROR" into "WARNING", this is applied only to "server" (cub_server process) and "client" (CAS or CSQL) which requested to change this value. Other "clients" keeps the value of "ERROR".

.. note:: If you want to change the value of a parameter permanently, restart all of DB server and broker after changing configuration values of cubrid.conf.

+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| Category                      | Parameter Name                      | Applied                 | Session | Type     | Default Value                  | Dynamic Change        |
+===============================+=====================================+=========================+=========+==========+================================+=======================+
| :ref:`connection-parameters`  | cubrid_port_id                      | client parameter        |         | int      | 1,523                          |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | check_peer_alive                    | client/server parameter | O       | string   | both                           | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | db_hosts                            | client parameter        | O       | string   | NULL                           | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | max_clients                         | server parameter        |         | int      | 100                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | tcp_keepalive                       | client/server parameter |         | bool     | yes                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | use_user_hosts                      | client/server parameter |         | bool     | off                            |                       |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`memory-parameters`      | data_buffer_size                    | server parameter        |         | byte     | 32,768 *                       |                       |
|                               |                                     |                         |         |          | :ref:`db_page_size <dpg>`      |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | index_scan_oid_buffer_size          | server parameter        |         | byte     | 4 *                            |                       |
|                               |                                     |                         |         |          | :ref:`db_page_size <dpg>`      |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | max_agg_hash_size                   | server parameter        |         | byte     | 2,097,152(2M)                  |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | max_hash_list_scan_size             | server parameter        |         | byte     | 8,388,608(8M)                  |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | max_subquery_cache_size             | server parameter        |         | byte     | 2,097,152(2M)                  | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | sort_buffer_size                    | server parameter        |         | byte     | 128 *                          |                       |
|                               |                                     |                         |         |          | :ref:`db_page_size <dpg>`      |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | temp_file_memory_size_in_pages      | server parameter        |         | int      | 4                              |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | thread_stacksize                    | server parameter        |         | byte     | 1,048,576                      |                       |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`disk-parameters`        | db_volume_size                      | server parameter        |         | byte     | 512M                           |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | dont_reuse_heap_file                | server parameter        |         | bool     | no                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | log_volume_size                     | server parameter        |         | byte     | 512M                           |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | temp_file_max_size_in_pages         | server parameter        |         | int      | -1                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | temp_volume_path                    | server parameter        |         | string   | NULL                           |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | unfill_factor                       | server parameter        |         | float    | 0.1                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | volume_extension_path               | server parameter        |         | string   | NULL                           |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | double_write_buffer_size            | server parameter        |         | byte     | 2M                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | data_file_os_advise                 | server parameter        |         | int      | 0                              |                       |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`error-parameters`       | call_stack_dump_activation_list     | client/server parameter |         | string   | DEFAULT                        | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | call_stack_dump_deactivation_list   | client/server parameter |         | string   | NULL                           | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | call_stack_dump_on_error            | client/server parameter |         | bool     | no                             | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | error_log                           | client/server parameter |         | string   | cub_client.err, cub_server.err |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | error_log_level                     | client/server parameter |         | string   | NOTIFICATION                   | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | error_log_warning                   | client/server parameter |         | bool     | no                             | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | error_log_size                      | client/server parameter |         | int      | 512M                           | DBA only              |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`lock-parameters`        | deadlock_detection_interval_in_secs | server parameter        |         | float    | 1.0                            | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | isolation_level                     | client parameter        | O       | int      | 4                              | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | lock_escalation                     | server parameter        |         | int      | 100,000                        |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | lock_timeout                        | client parameter        | O       | msec     | -1                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | rollback_on_lock_escalation         | server parameter        |         | bool     | no                             | DBA only              |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`logging-parameters`     | adaptive_flush_control              | server parameter        |         | bool     | yes                            | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | background_archiving                | server parameter        |         | bool     | yes                            | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | checkpoint_every_size               | server parameter        |         | byte     | 100,000 *                      |                       |
|                               |                                     |                         |         |          | :ref:`log_page_size <lpg>`     |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | checkpoint_interval                 | server parameter        |         | msec     | 6min                           | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | checkpoint_sleep_msecs              | server parameter        |         | msec     | 1                              | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | force_remove_log_archives           | server parameter        |         | bool     | yes                            | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | log_buffer_size                     | server parameter        |         | byte     | 16k *                          |                       |
|                               |                                     |                         |         |          | :ref:`log_page_size <lpg>`     |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | log_max_archives                    | server parameter        |         | int      | INT_MAX                        | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | log_trace_flush_time                | server parameter        |         | msec     | 0                              | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | max_flush_size_per_second           | server parameter        |         | byte     | 10,000 *                       | DBA only              |
|                               |                                     |                         |         |          | :ref:`db_page_size <dpg>`      |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | remove_log_archive_interval_in_secs | server parameter        |         | sec      | 0                              | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | sync_on_flush_size                  | server parameter        |         | byte     | 200 *                          | DBA only              |
|                               |                                     |                         |         |          | :ref:`db_page_size <dpg>`      |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | ddl_audit_log                       | client parameter        |         | bool     | no                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | ddl_audit_log_size                  | client parameter        |         | byte     | 10M                            |                       |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`transaction-parameters` | async_commit                        | server parameter        |         | bool     | no                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | group_commit_interval_in_msecs      | server parameter        |         | msec     | 0                              | DBA only              |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`stmt-type-parameters`   | add_column_update_hard_default      | client/server parameter | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | alter_table_change_type_strict      | client/server parameter | O       | bool     | yes                            | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | allow_truncated_string              | client/server parameter | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | ansi_quotes                         | client parameter        |         | bool     | yes                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | block_ddl_statement                 | client parameter        | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | block_nowhere_statement             | client parameter        | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | compat_numeric_division_scale       | client/server parameter | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | create_table_reuseoid               | client parameter        | O       | bool     | yes                            | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | cte_max_recursions                  | client/server parameter | O       | int      | 2000                           | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | default_week_format                 | client/server parameter | O       | int      | 0                              | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | group_concat_max_len                | server parameter        | O       | byte     | 1,024                          | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | intl_check_input_string             | client parameter        | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | intl_collation                      | client parameter        | O       | string   |                                | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | intl_date_lang                      | client parameter        | O       | string   |                                | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | intl_number_lang                    | client parameter        | O       | string   |                                | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | json_max_array_idx                  | server parameter        | O       | string   | 65,536                         | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | no_backslash_escapes                | client parameter        |         | bool     | yes                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | only_full_group_by                  | client parameter        | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | oracle_compat_number_behavior       | server parameter        |         | bool     | no                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | oracle_style_empty_string           | client parameter        |         | bool     | no                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | pipes_as_concat                     | client parameter        |         | bool     | yes                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | plus_as_concat                      | client parameter        |         | bool     | yes                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | require_like_escape_character       | client parameter        |         | bool     | no                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | return_null_on_function_errors      | client/server parameter | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | string_max_size_bytes               | client/server parameter | O       | byte     | 1,048,576                      | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | unicode_input_normalization         | client parameter        | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | unicode_output_normalization        | client parameter        | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | update_use_attribute_references     | client parameter        | O       | bool     | no                             | available             |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`thread-parameters`      | thread_connection_pooling           | server parameter        |         | bool     | yes                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | thread_connection_timeout_seconds   | server parameter        |         | int      | 300                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | thread_worker_pooling               | server parameter        |         | bool     | yes                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | thread_core_count                   | server parameter        |         | int      | # of system cores              |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | thread_worker_timeout_seconds       | server parameter        |         | int      | 300                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | loaddb_worker_count                 | server parameter        |         | int      | 8                              |                       |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`timezone-parameters`    | server_timezone                     | server parameter        |         | string   | OS timezone                    | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | timezone                            | client/server parameter | O       | string   | the value of server_timezone   | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | tz_leap_second_support              | server parameter        |         | bool     | no                             | available             |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`plan-cache-parameters`  | max_plan_cache_entries              | client/server parameter |         | int      | 1,000                          |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | max_plan_cache_clones               | server parameter        |         | int      | 1,000                          |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | xasl_cache_time_threshold_in_minutes| client/server parameter |         | int      | 360                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | max_filter_pred_cache_entries       | client/server parameter |         | int      | 1,000                          |                       |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`query-cache-parameters` | max_query_cache_entries             | server parameter        |         | int      | 0                              | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | query_cache_size_in_pages           | server parameter        |         | int      | 0                              | available             |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`utility-parameters`     | backup_volume_max_size_bytes        | server parameter        |         | byte     | 0                              |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | communication_histogram             | client parameter        | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | compactdb_page_reclaim_only         | server parameter        |         | int      | 0                              |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | csql_history_num                    | client parameter        | O       | int      | 50                             | available             |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`ha-parameters`          | ha_mode                             | server parameter        |         | string   | off                            |                       |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
| :ref:`other-parameters`       | access_ip_control                   | server parameter        |         | bool     | no                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | access_ip_control_file              | server parameter        |         | string   |                                |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | agg_hash_respect_order              | client parameter        | O       | bool     | yes                            | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | auto_restart_server                 | server parameter        | O       | bool     | yes                            | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | enable_string_compression           | client/server parameter |         | bool     | yes                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | index_scan_in_oid_order             | client parameter        | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | index_unfill_factor                 | server parameter        |         | float    | 0.05                           |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | java_stored_procedure               | server parameter        |         | bool     | no                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | java_stored_procedure_port          | server parameter        |         | int      | 0                              |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | java_stored_procedure_uds           | server parameter        |         | bool     | yes                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | java_stored_procedure_jvm_options   | server parameter        |         | string   |                                |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | multi_range_optimization_limit      | server parameter        | O       | int      | 100                            | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | optimizer_enable_merge_join         | client parameter        | O       | bool     | no                             | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | use_stat_estimation                 | server parameter        |         | bool     | no                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | server                              | server parameter        |         | string   |                                |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | service                             | server parameter        |         | string   |                                |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | session_state_timeout               | server parameter        |         | sec      | 21,600                         |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | sort_limit_max_count                | client parameter        | O       | int      | 1,000                          | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | sql_trace_slow                      | server parameter        | O       | msec     | -1                             | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | sql_trace_execution_plan            | server parameter        | O       | bool     | no                             | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | use_orderby_sort_limit              | server parameter        | O       | bool     | yes                            | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | vacuum_prefetch_log_mode            | server parameter        |         | int      | 1                              | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | vacuum_prefetch_log_buffer_size     | server parameter        |         | int      | 3200 *                         | DBA only              |
|                               |                                     |                         |         |          | :ref:`log_page_size <lpg>`     |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | data_buffer_neighbor_flush_pages    | server parameter        |         | int      | 8                              | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | data_buffer_neighbor_flush_nondirty | server parameter        |         | bool     | no                             | DBA only              |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | tde_keys_file_path                  | server parameter        |         | string   | NULL                           |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | tde_default_algorithm               | server parameter        |         | string   | AES                            |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | recovery_progress_logging_interval  | server parameter        |         | int      | 0                              |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | supplemental_log                    | client/server parameter |         | int      | 0                              |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | regexp_engine                       | client/server parameter |         | string   | re2                            | available             |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | vacuum_ovfp_check_threshold         | server parameter        |         | int      | 1000                           |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | vacuum_ovfp_check_duration          | server parameter        |         | int      | 45000                          |                       |
+                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | deduplicate_key_level               | client/server parameter |         | int      | -1                             |                       |
|                               +-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+
|                               | print_index_detail                  | client/server parameter |         | bool     | no                             |                       |
+-------------------------------+-------------------------------------+-------------------------+---------+----------+--------------------------------+-----------------------+

.. _lpg:

*   **log_page_size**: A log volume page size specified by **\-\-log-page-size** option when you are :ref:`creating database<creating-database>`. Default: 16KB. log page related parameter's value is rounded off by page unit. 
    For example, the value of checkpoint_every_size is divided by 16KB and its decimal point is dropped, then it is multiplied by 16KB.

.. _dpg:

*   **db_page_size**: A DB volume page size specified by **\-\-db-page-size** option when you are :ref:`creating database<creating-database>`. Default: 16KB. DB page related parameter's value is rounded off by page unit. 
    For example, the value of data_buffer_size is divided by 16KB and its decimal point is dropped, then it is multiplied by 16KB.

Section by Parameter
^^^^^^^^^^^^^^^^^^^^

Parameters specified in **cubrid.conf** have the following four sections:

*   Used when the CUBRID service starts: [service] section
*   Applied commonly to all databases: [common] section
*   Applied individually to each database: [@<*database*>] section
*   Used only when the cubrid utilities are run with stand-alone mode(\-\-SA-mode): [standalone] section

Where <*database*> is the name of the database to which each parameter applies. If a parameter configured in [common] is the same as the one configured in [@<*database*>], the one configured in [@<*database*>] is applied.

::

    ..... 
    [common] 
    ..... 
    sort_buffer_size=2M 
    ..... 
    [standalone] 
  
    sort_buffer_size=256M 
    ..... 

Configuration defined in [standalone] is used only when cubrid utilities started with "cubrid" are run with stand-alone mode.
For example, on the above configuration, if DB is started with \-\-CS-mode(default)(cubrid databases start db_name), "sort_buffer_size=2M" is applied. However, if DB is stopped and "cubrid loaddb \-\-SA-mode" is executed, "sort_buffer_size=256M" is applied. If you run "cubrid loaddb \-\-SA-mode", bigger size of sort buffer will be required during index creation; therefore, increasing sort buffer size will be better for the performance of "loaddb" execution.

Default Parameters
^^^^^^^^^^^^^^^^^^

**cubrid.conf**, a default database configuration file created during the CUBRID installation, includes some default database server parameters that must be changed. You can change the value of a parameter that is not included as a default parameter by manually adding or editing one.

The following is the content of the **cubrid.conf** file. ::

    # Copyright (C) 2008 Search Solution Corporation. All rights reserved by Search Solution.
    #
    # $Id$
    #
    # cubrid.conf#
     
    # For complete information on parameters, see the CUBRID
    # Database Administration Guide chapter on System Parameters
     
    # Service section - a section for 'cubrid service' command
    [service]
     
    # The list of processes to be started automatically by 'cubrid service start' command
    # Any combinations are available with server, broker and manager.
    service=server,broker,manager
     
    # The list of database servers in all by 'cubrid service start' command.
    # This property is effective only when the above 'service' property contains 'server' keyword.
    #server=demodb,testdb
    
    # Common section - properties for all databases
    # This section will be applied before other database specific sections.
    [common]
     
    # Read the manual for detailed description of system parameters
    # Manual > System Configuration > Database Server Configuration > Default Parameters
     
    # Size of data buffer are using K, M, G, T unit
    data_buffer_size=512M
     
    # Size of log buffer are using K, M, G, T unit
    log_buffer_size=256M
     
    # Size of sort buffer are using K, M, G, T unit
    # The sort buffer should be allocated per thread.
    # So, the max size of the sort buffer is sort_buffer_size * max_clients.
    sort_buffer_size=2M
     
    # The maximum number of concurrent client connections the server will accept.
    # This value also means the total # of concurrent transactions.
    max_clients=100
     
    # TCP port id for the CUBRID programs (used by all clients).
    cubrid_port_id=1523

If you want to set **data_buffer_size** as 128M and **max_clients** as 10 only on *testdb*, set as follows. ::

    [service]
     
    service=server,broker,manager
     
    [common]
     
    data_buffer_size=512M
    log_buffer_size=256M
    sort_buffer_size=2M
    max_clients=100
     
    # TCP port id for the CUBRID programs (used by all clients).
    cubrid_port_id=1523

    [@testdb]
    data_buffer_size=128M
    max_clients=10

.. _connection-parameters:

Connection-Related Parameters
-----------------------------

The following are parameters related to the database server. The type and value range for each parameter are as follows:

+---------------------------------+--------+----------+----------+----------+
| Parameter Name                  | Type   | Default  | Min      | Max      |
+=================================+========+==========+==========+==========+
| cubrid_port_id                  | int    | 1,523    | 1        |          |
+---------------------------------+--------+----------+----------+----------+
| check_peer_alive                | string | both     |          |          |
+---------------------------------+--------+----------+----------+----------+
| db_hosts                        | string | NULL     |          |          |
+---------------------------------+--------+----------+----------+----------+
| max_clients                     | int    | 100      | 10       | 4,000    |
+---------------------------------+--------+----------+----------+----------+
| tcp_keepalive                   | bool   | yes      |          |          |
+---------------------------------+--------+----------+----------+----------+
| use_user_hosts                  | bool   | off      |          |          |
+---------------------------------+--------+----------+----------+----------+

**cubrid_port_id**

    **cubrid_port_id** is a parameter to configure the port to be used by the master process. The default value is **1,523**. If the port 1,523 is already being used on the server where CUBRID is installed or it is blocked by a firewall, an error message, which means the master server is not connected because the master process cannot be running properly, is displayed. If such port conflict occurs, the administrator must change the value of **cubrid_port_id** considering the server environment.

.. _check_peer_alive:

**check_peer_alive**

    **check_peer_alive** is a parameter to decide whether you execute the function checking that the client/server processes work well. The default is **both**. 

    The client processes connecting with a server process are the broker application server(cub_cas) process, the process copying replication logs(copylogdb), the process applying replication logs. (applylogdb), CSQL interpreter(csql), etc. The server process and the client process which connected with it wait each other's response. But if one of them cannot get the data for a long time(example: exceeding 5 sec), it will check or not if the other works well based on the configuration of check_peer_alive parameter. During this processes, if it is judged  that the process doesn't work properly, it disconnect forcibly.

    The values and the working methods are as follows.

    *   **both**: As the server process accesses to the client process by ECHO(7) port, it checks if the client process works well. The client process also does the same thing to the server process(The default value).
    *   **server_only**: Only the server process checks whether the client process works well.
    *   **client_only**: Only the client process checks whether the server process works well.
    *   **none**: None of the server and client processes check whether the other process works well.

    Specially, if ECHO(7) port is blocked by the firewall configuration, each process can mistake that the other process  was exited. Therefore, you should avoid this problem by setting this parameter's value as none.

**db_hosts**

    **db_hosts** is a parameter to configure a list of the database server hosts to which clients can connect, and the connection order. The server host list consists of multiple server host names, and host names are separated by spaces or colons (:). Duplicate or non-existent names are ignored.

    The following example shows the values of the **db_hosts** parameter. In this example, connections are attempted in the order of **host1** > **host2** > **host3**. ::

        db_hosts="hosts1:hosts2:hosts3"

    To connect to the server, the client first tries to connect to the specified server host referring to the database location file (**databases.txt**). If the connection fails, the client then tries to connect to the first one of the secondarily specified server hosts by referring to the value of the **db_hosts** parameter in the database configuration file (**cubrid.conf**).

.. _max_clients:

**max_clients**

    **max_clients** is a parameter to configure the maximum number of clients (usually broker application processes (CAS)) which allow concurrent connections to the database server. The **max_clients** parameter refers to the number of concurrent transactions per database server process. The default value is **100**.

    To guarantee performance while increasing the number of concurrent users in CUBRID environment, you need to make the appropriate value of the **max_clients** (**cubrid.conf**) parameter and the :ref:`MAX_NUM_APPL_SERVER <max-num-appl-server>` (**cubrid_broker.conf**) parameter. That is, you are required to configure the number of concurrent connections allowed by databases with the **max_clients** parameter. You should also configure the number of concurrent connections allowed by brokers with the **MAX_NUM_APPL_SERVER** parameter.

    For example, in the **cubrid_broker.conf** file, two node of a broker where the **MAX_NUM_APPL_SERVER** value of [%query_editor] is 50 and the **MAX_NUM_APPL_SERVER** value of [%BROKER1] is 50 is trying to connect one database server, the concurrent connections (**max_clients** value) allowed by the database server can be configured as follows:

    *   (the maximum number of 100 by each node of a broker) * (two node of a broker) + (10 spare for database server connections of internal CUBRID process such as database server connection of CSQL Interpreter or HA log replication process) = 210

    Especially, in HA environment, the value must be greater than the sum specified in **MAX_NUM_APPL_SERVER** of every broker node which connects to the same database.

    Note that the memory usage is affected by the value specified in **max_clients**. That is, if the number of value is high, the memory usage will increase regardless of whether or not the clients actually access the database.

    .. note::
        
        In Linux system, max_clients parameter is related to "ulimit -n" command, which specifies the maximum number of file descriptors which a process can use. File descriptor includes not only a file, but also a network socket. Therefore, the number of "ulimit -n" should be greater than the number of max_clients.

**tcp_keepalive** 
  
    **tcp_keepalive** is a parameter which specifies if you apply SO_KEEPALIVE option to TCP network protocol or not. The default is **yes**. If this value is **no**, DB server-side connection can be disconnected when transaction logs are not copied for a long time in the firewall environment between master and slave.
 

**use_user_hosts** 

    **use_user_hosts** is a system parameter that is used to select the host look up between **hostname** and **IP address**, required by the CUBRID service from the services below. The default value is **OFF**.

    * The host/IP address look-up library that **OS** provides. (**glibc**, Linux)
    * **The host/IP address look-up library** that **CUBRID** provides.

    use_user_hosts=off (default)

    * Looks up between IP address and hostname using the system library.
    * In general, provides /etc/hosts host look-up, DNS Query commonly

    use_user_hosts=on

    * Looks up between IP address and hostname using CUBRID host look-up library.
    * Uses **$CUBRID/conf/cubrid_hosts.conf** file to look up between IP address and hostname.
    * executes regardless of the read permission of /etc/hosts, /etc/nsswitch.conf file.

    .. warning::

        If **use_user_hosts** parameter is changed during service operation, CUBRID service cannot terminate normally. So, the user must change the parameter after CUBRID service terminates.

    *   The format of **$CUBRID/conf/cubrid_hosts.conf** is same as **/etc/hosts** but there are some restrictions as follows.

        * Allow **IPv4** format address only. (Not allow **IPv6** format address)
        * Not allow **alias**. ::

           172.31.0.1 host1 alias1 alias2

        * If there are multiple IP addresses, the top of information ,{ip, hostname}, is used except others. ::

           172.31.0.1 host1
           172.31.0.1 alias1
           172.31.0.1 alias2

        * If you use the same hostname for more than one IP, only the topmost IP will be applied. ::

            172.31.0.1 host1
            178.31.0.2 host1

    * The following is an example of $CUBRID/conf/cubrid_hosts.conf. ::

            #
            # hosts file for CUBRID user specific host service
            #
            127.0.0.1       localhost
            172.31.0.1      node1
            192.168.0.31    node4.kr         # Seoul
            192.168.2.31    node5.gov.or.kr  # Daejeon


.. warning::

    You must change $CUBRID/conf/cubrid_hosts.conf after terminating all CUBRID processes, and **the changes will be applied after restarting.** In addition, you must write including **localhost** and **'hostname'** (The output of hostname command by among Linux commands) in the cubrid_hosts.conf.

.. warning::

    The hostname adheres the following format in the $CUBRID/conf/cubrid_hosts.conf (The Linux hostname naming rule).

    * Only English letters, numbers (0 to 9), hyphen ('-'), and dot (".") characters can be used for the hostname.
    * The first character of the hostname must be an English letter.
    * The last character of the hostname must be an English letter and a number.
    * FQDN (Fully Qualified Domain Name) format hostname can be used (Example: www.cubrid.com). 

    Allow the following hostname.

    ::

      cubrid-dev1
      CUB2.dev

.. _memory-parameters:

Memory-Related Parameters
-------------------------

The following are parameters related to the memory used by the database server or client. The type and value range for each parameter are as follows:

+--------------------------------+--------+---------------------------+---------------------------+---------------------------+
| Parameter Name                 | Type   | Default                   | Min                       | Max                       |
+================================+========+===========================+===========================+===========================+
| data_buffer_size               | byte   | 32,768 *                  | 1,024 *                   | 2G(32bit),                |
|                                |        | :ref:`db_page_size <dpg>` | :ref:`db_page_size <dpg>` | INT_MAX *                 |
|                                |        |                           |                           | :ref:`db_page_size <dpg>` |
|                                |        |                           |                           | (64bit)                   |
+--------------------------------+--------+---------------------------+---------------------------+---------------------------+
| index_scan_oid_buffer_size     | byte   | 4 *                       | 0.05 *                    | 16 *                      |
|                                |        | :ref:`db_page_size <dpg>` | :ref:`db_page_size <dpg>` | :ref:`db_page_size <dpg>` |
+--------------------------------+--------+---------------------------+---------------------------+---------------------------+
| max_agg_hash_size              | byte   | 2,097,152(2M)             | 32,768(32K)               | 134,217,728(128MB)        |
+--------------------------------+--------+---------------------------+---------------------------+---------------------------+
| max_hash_list_scan_size        | byte   | 8,388,608(8M)             | 0                         | 128MB                     |
+--------------------------------+--------+---------------------------+---------------------------+---------------------------+
| max_subquery_cache_size        | byte   | 2,097,152(2M)             | 0                         | 16,777,216(16M)           |
+--------------------------------+--------+---------------------------+---------------------------+---------------------------+
| sort_buffer_size               | byte   | 128 *                     | 1 *                       | 2G(32bit),                |
|                                |        | :ref:`db_page_size <dpg>` | :ref:`db_page_size <dpg>` | INT_MAX *                 |
|                                |        |                           |                           | :ref:`db_page_size <dpg>` |
|                                |        |                           |                           | (64bit)                   |
+--------------------------------+--------+---------------------------+---------------------------+---------------------------+
| temp_file_memory_size_in_pages | int    | 4                         | 0                         | 20                        |
+--------------------------------+--------+---------------------------+---------------------------+---------------------------+
| thread_stacksize               | byte   | 1,048,576                 | 65,536                    |                           |
+--------------------------------+--------+---------------------------+---------------------------+---------------------------+

**data_buffer_size**

    **data_buffer_size** is a parameter to configure the size of data buffer to be cached in the memory by the database server. You can set a unit as B, K, M, G or T, which stands for bytes, kilobytes(KB), megabytes(MB), gigabytes(GB) or terabytes(TB) respectively. If you omit the unit, bytes will be applied. The default value is 32,768 * :ref:`db_page_size <dpg>` (**512M** when db_page_size is 16K), and the minimum value is 1,024 * :ref:`db_page_size <dpg>` (**16M** when db_page_size is 16K). The maximum value in 64-bit CUBRID is INT_MAX * :ref:`db_page_size <dpg>`. Note that the maximum value in 32-bit CUBRID is **2G**.
    
    The greater the value of the **data_buffer_size** parameter, the more data pages to be cached in the buffer, thus providing the advantage of decreased disk I/O cost. However, if this parameter is too large, the buffer pool can be swapped out by the operating system because the system memory is excessively occupied. It is recommended to configure the **data_buffer_size** parameter in a way the required memory size is less than two-thirds of the system memory size.

    *   Required memory size = data buffer size (**data_buffer_size**)

**index_scan_oid_buffer_size**

    **index_scan_oid_buffer_size** is a parameter to configure the size of buffer where the OID list is to be temporarily stored during the index scan. You can set unit K, which stands for KB (kilobytes). If you omit the unit, bytes will be applied. The default value is  4 * :ref:`db_page_size <dpg>` (**64K** when db_page_size is 16K), and the minimum value is 0.05 * :ref:`db_page_size <dpg>` (about **1K** when db_page_size is 16K).

    The size of the OID buffer tends to vary in proportion to the value of the **index_scan_oid_buffer_size** parameter and the page size set when the database was created. In addition, the bigger the size of such OID buffer, the more the index scan cost. You can set the value of the **index_scan_oid_buffer_size** by considering these factors.

.. _max_agg_hash_size:

**max_agg_hash_size**

    **max_agg_hash_size** is a parameter to configure the maximum memory per transaction allocated for hashing the tuple groups in a query containing aggregation. The default is **2,097,152**\ (2M), the minimum size is 32,768(32K), and the maximum size is  134,217,728(128MB). 
    
    If :ref:`NO_HASH_AGGREGATE <no-hash-aggregate>` hint is specified, hash aggregate evaluation will not be used. As a reference, see :ref:`agg_hash_respect_order <agg_hash_respect_order>`.

.. _max_hash_list_scan_size:

**max_hash_list_scan_size**

    **max_hash_list_scan_size** is a parameter to configure the maximum memory per transaction allocated for building hash table in a query containing subquerys. The default is 8MB, the minimum size is 0, and the maximum size is 128MB.

    If this parameter is set to 0 or If :ref:`NO_HASH_LIST_SCAN <no-hash-list-scan>` hint is specified, hash list scan will not be used.

**max_subquery_cache_size**

    **max_subquery_cache_size** is a parameter used to set the size of the subquery cache (correlated). You can set a unit as B, K or M, which stand for bytes, kilobytes (KB), and megabytes (MB), respectively. If you omit the unit, bytes will be applied. The default value is **2,097,152** (2M) bytes, the minimum value is **0**, and the maximum value is **16,777,216** (16M) bytes. The subquery cache is allocated for the number of subqueries in a query and is deallocated when the main query is completed. 
    
    If max_subquery_cache_size is set to 0, the :ref:`NO_SUBQUERY_CACHE <correlated-subquery-cache>` hint is specified, or there is insufficient memory space, the subquery cache optimization is not enabled.

**sort_buffer_size**

    **sort_buffer_size** is a parameter to configure the size of buffer to be used when a query is processing sorting. The server assigns one sort buffer for each client's sorting-request, and releases the assigned buffer memory when sorting is complete. A sorting query includes not only SELECT sorting query, but also index-creating query.

    You can set a unit as B, K, M, G or T, which stand for bytes, kilobytes (KB), megabytes (MB), gigabytes (GB), and terabytes (TB) respectively. If you omit the unit, bytes will be applied. The default value is 128 * :ref:`db_page_size <dpg>` (**2M** when db_page_size is 16K), and the minimum value is 1 * :ref:`db_page_size <dpg>` (**16K** when db_page_size is 16K).
    
**temp_file_memory_size_in_pages**

    **temp_file_memory_size_in_pages** is a parameter to configure the number of buffer pages to cache temporary result of a query. The default value is **4** and the maximum value is 20.

    *   Required memory size = the number of temporary memory buffer pages (**temp_file_memory_size_in_pages** \* **page size**)
    *   The number of temporary memory buffer pages = the value of the **temp_file_memory_size_in_pages** parameter
    *   Page size = the value of the page size specified by the **-s** option of the **cubrid createdb** utility during the database creation

    The spaces to store the temporary result are as follows.
    
    *   Cache buffer to store the temporary result (acquired by **temp_file_memory_size_in_pages** parameter)
    *   Permanent volumes with the purpose of storing temporary data.
    *   Temporary volumes
    
    If the previous space is exhausted, then the next space is used as the following order: Cache buffer for storing temporary result -> Permanent volumes -> Temporary volumes.

**thread_stacksize**

    **thread_stacksize** is a parameter to configure the stack size of a thread. The default value is **1048576** bytes. The value of the **thread_stacksize** parameter must not exceed the stack size allowed by the operating system.

.. _disk-parameters:

Disk-Related Parameters
-----------------------

The following are disk-related parameters for defining database volumes and storing files. The type and value range for each parameter are as follows:

+------------------------------------------+--------+----------+----------+----------+
| Parameter Name                           | Type   | Default  | Min      | Max      |
+==========================================+========+==========+==========+==========+
| db_volume_size                           | byte   | 512M     | 0        | 20G      |
+------------------------------------------+--------+----------+----------+----------+
| dont_reuse_heap_file                     | bool   | no       |          |          |
+------------------------------------------+--------+----------+----------+----------+
| log_volume_size                          | byte   | 512M     | 20M      | 4G       |
+------------------------------------------+--------+----------+----------+----------+
| temp_file_max_size_in_pages              | int    | -1       |          |          |
+------------------------------------------+--------+----------+----------+----------+
| temp_volume_path                         | string | NULL     |          |          |
+------------------------------------------+--------+----------+----------+----------+
| unfill_factor                            | float  | 0.1      | 0.0      | 0.3      |
+------------------------------------------+--------+----------+----------+----------+
| volume_extension_path                    | string | NULL     |          |          |
+------------------------------------------+--------+----------+----------+----------+
| double_write_buffer_size                 | byte   | 2M       | 0        | 32M      |
+------------------------------------------+--------+----------+----------+----------+
| data_file_os_advise                      | int    | 0        | 0        | 6        |
+------------------------------------------+--------+----------+----------+----------+

**db_volume_size**

    **db_volume_size** is a parameter to configure the following values. You can set a unit as B, K, M, G or T, which stand for bytes, kilobytes (KB), megabytes (MB), gigabytes (GB), and terabytes (TB) respectively. If you omit the unit, bytes will be applied. The default value is **512M**.

    *   The default database volume size when **cubrid createdb** and **cubrid addvoldb** utility is used without **\-\-db-volume-size** option.
    *   The default size of volume that is maximum size of an automatically added volume when the database is full. (The auto-added volume is incrementally increased by the needed amount, and another volume is created when the volume exceeds db_volume_size.)

.. note::

    The actual volume size will always be rounded up to a multiple of the size of 64 sectors. Sector size depends on page size, therefore 64 sectors size is 16M, 32M or 64M for page size 4k, 8k or 16k respectively.

**dont_reuse_heap_file**

    **dont_reuse_heap_file** is a parameter to configure whether or not heap files, which are deleted when deleting the table (**DROP TABLE**), are to be reused when creating a new table (**CREATE TABLE**). If this parameter is set to no, the deleted heap files can be reused; if it is set to yes, the deleted heap files are not used when creating a new table. The default value is **no**.

**log_volume_size**

    **log_volume_size** is a parameter to configure the default size of log volume file when the **cubrid createdb** utility is used without **\-\-log-volume-size** option. You can set a unit as B, K, M, G or T, which stand for bytes, kilobytes (KB), megabytes (MB), gigabytes (GB) and terabytes (TB) respectively. If you omit the unit, bytes will be applied. The default value is **512M**.

**temp_file_max_size_in_pages**

    **temp_file_max_size_in_pages** is a parameter to configure the maximum number of pages to which temporary volumes can be extended. By default, this value is **-1**, which means that temporary volumes can occupy an unlimited disk space. A positive value will set a limit to these values (exceeding it may show an error and cancel some big queries).
    
    If the parameter is configured to **0**, temporary volumes are not created automatically; the administrator must create permanent volumes with the purpose of storing temporary data by using the **cubrid addvoldb** utility.
    
    For more details see :ref:`temporary-volumes`

**temp_volume_path**

    **temp_volume_path** is a parameter to configure the directory in which to create temporary volumes used for the execution of complex queries or sorting. The default value is the volume location configured during the database creation.

**unfill_factor**

    **unfill_factor** is a parameter to configure the rate of disk space to be allocated in a heap page for data updates. The default value is **0.1**. That is, the rate of free space is configured to 10%. In principle, data in the table is inserted in physical order. However, if the size of the data increases due to updates and there is not enough space for storage in the given page, performance may degrade because updated data must be relocated to another page. To prevent such a problem, you can configure the rate of space for a heap page by using the **unfill_factor** parameter. The allowable maximum value is 0.3 (30%). In a database where data updates rarely occur, you can configure this parameter to 0.0 so that space will not be allocated in a heap page for data updates. If the value of the **unfill_factor** parameter is negative or greater than the maximum value, the default value (**0.1**) is used.

**volume_extension_path**

    **volume_extension_path** is a parameter to configure the directory where automatically extended volumes are to be created. The default value is the volume location configured during the database creation.

**double_write_buffer_size**

    **double_write_buffer_size** is a parameter to configure the memory and disk size of double writer buffer. Double write buffer protection against partial I/O writes can be disabled by setting this size to zero. By default, it is enabled and its size is 2M.

**data_file_os_advise**

    **data_file_os_advise** is a UNIX-only parameter that may be used to boost I/O performance. \
    \The parameter value is converted into a *posix_fadvise()* flag \
    \(for details about the flags `see here
    <https://linux.die.net/man/2/posix_fadvise>`_).

    +-----------------------------------+-------------------------------------------+
    | Parameter Value                   | posix_fadvise flag                        |
    +===================================+===========================================+
    | 0                                 | 0                                         |
    +-----------------------------------+-------------------------------------------+
    | 1                                 | POSIX_FADV_NORMAL                         |
    +-----------------------------------+-------------------------------------------+
    | 2                                 | POSIX_FADV_SEQUENTIAL                     |
    +-----------------------------------+-------------------------------------------+
    | 3                                 | POSIX_FADV_RANDOM                         |
    +-----------------------------------+-------------------------------------------+
    | 4                                 | POSIX_FADV_NOREUSE                        |
    +-----------------------------------+-------------------------------------------+
    | 5                                 | POSIX_FADV_WILLNEED                       |
    +-----------------------------------+-------------------------------------------+
    | 6                                 | POSIX_FADV_DONTNEED                       |
    +-----------------------------------+-------------------------------------------+

    .. warning::

        Make sure posix_fadvise flags and how data is accessed are perfectly understood. \
        \The parameter can help improve performance but it can also degrade it if misused. \
        \In most scenarios it is best to use the default value.

.. _error-parameters:

Error Message-Related Parameters
--------------------------------

The following are parameters related to processing error messages recorded by CUBRID. The type and value range for each parameter are as follows:

+-----------------------------------+----------+--------------------------------+
| Parameter Name                    | Type     | Default                        |
+===================================+==========+================================+
| call_stack_dump_activation_list   | string   | DEFAULT                        |
+-----------------------------------+----------+--------------------------------+
| call_stack_dump_deactivation_list | string   | NULL                           |
+-----------------------------------+----------+--------------------------------+
| call_stack_dump_on_error          | bool     | no                             |
+-----------------------------------+----------+--------------------------------+
| error_log                         | string   | cub_client.err, cub_server.err |
+-----------------------------------+----------+--------------------------------+
| error_log_level                   | string   | NOTIFICATION                   |
+-----------------------------------+----------+--------------------------------+
| error_log_warning                 | bool     | no                             |
+-----------------------------------+----------+--------------------------------+
| error_log_size                    | int      | 512M                           |
+-----------------------------------+----------+--------------------------------+

**call_stack_dump_activation_list**

    **call_stack_dump_activation_list** is a parameter to configure a certain error number for which a call stack is to be dumped to a server error log (located in $CUBRID/log/server directory) as an exception even when you configure that a call stack will not be dumped for any errors. Therefore, the **call_stack_dump_activation_list** parameter is effective only when **call_stack_dump_on_error=no**.
    
    If this value is not configured, the default value is "DEFAULT" keyword. This keyword includes below errors. "DEFAULT" keyword can be used together with other error numbers.

    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | Error Number | Error Message                                                                                                                                   |
    +==============+=================================================================================================================================================+
    | -2           | Internal system failure: no more specific information is available.                                                                             |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -7           | Trying to format disk volume xxx with an incorrect value xxx for number of pages.                                                               |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -13          | An I/O error occurred while reading page xxx of volume xxx.                                                                                     |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -14          | An I/O error occurred while writing page xxx of volume xxx.                                                                                     |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -17          | Internal error: fetching deallocated pageid xxx of volume xxx.                                                                                  |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -19          | Internal error: pageptr = xxx of page xxx of volume xxx is not fixed.                                                                           |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -21          | Internal error: unknown sector xxx of volume xxx.                                                                                               |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -22          | Internal error: unknown page xxx of volume xxx.                                                                                                 |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -45          | Slot xxx on page xxx of volume xxx is allocated to an anchored record. A new record cannot be inserted here.                                    |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -46          | Internal error: slot xxx on page xxx of volume xxx is not allocated.                                                                            |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -48          | Accessing deleted object xxx|xxx|xxx.                                                                                                           |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -50          | Internal error: relocation record of object xxx|xxx|xxx may be corrupted.                                                                       |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -51          | Internal error: object xxx|xxx|xxx may be corrupted.                                                                                            |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -52          | Internal error: object overflow address xxx|xxx|xxx may be corrupted.                                                                           |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -76          | Your transaction (index xxx, xxx\@xxx|xxx) timed out waiting on xxx on page xxx|xxx. You are waiting for user(s) xxx to release the page lock.  |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -78          | Internal error: an I/O error occurred while reading logical log page xxx (physical page xxx) of xxx.                                            |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -79          | Internal error: an I/O error occurred while writing logical log page xxx (physical page xxx) of xxx.                                            |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -81          | Internal error: logical log page xxx may be corrupted.                                                                                          |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -90          | Redo logging is always a page level logging operation. A data page pointer must be given as part of the address.                                |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -96          | Media recovery may be needed on volume xxx.                                                                                                     |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -97          | Internal error: unable to find log page xxx in log archives.                                                                                    |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -313         | Object buffer underflow while reading.                                                                                                          |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -314         | Object buffer overflow while writing.                                                                                                           |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -407         | Unknown key xxx referenced in B+tree index {vfid: (xxx, xxx), rt_pgid: xxx, key_type: xxx}.                                                     |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -414         | Unknown class identifier: xxx|xxx|xxx.                                                                                                          |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -415         | Invalid class identifier: xxx|xxx|xxx.                                                                                                          |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -416         | Unknown representation identifier: xxx.                                                                                                         |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -417         | Invalid representation identifier: xxx.                                                                                                         |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -583         | Trying to allocate an invalid number (xxx) of pages.                                                                                            |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -603         | Internal Error: Sector/page table of file VFID xxx|xxx seems corrupted.                                                                         |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -836         | LATCH ON PAGE(xxx|xxx) TIMEDOUT                                                                                                                 |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -859         | LATCH ON PAGE(xxx|xxx) ABORTED                                                                                                                  |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -890         | Partition failed.                                                                                                                               |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -891         | Appropriate partition does not exist.                                                                                                           |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -976         | Internal error: Table size overflow (allocated size: xxx, accessed size: xxx) at file table page xxx|xxx(volume xxx)                            |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -1040        | HA generic: xxx.                                                                                                                                |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+
    | -1075        | Descending index scan aborted because of lower priority on B+tree with index identifier: (vfid = (xxx, xxx), rt_pgid: xxx).                     |
    +--------------+-------------------------------------------------------------------------------------------------------------------------------------------------+

    The following example shows how to make error numbers only -115 and -116, perform call-stack dump. ::

        call_stack_dump_on_error= no
        call_stack_dump_activation_list=-115,-116

    The following example shows how to make error numbers -115, -116 and "DEFAULT" error numbers, perform call-stack dump. ::

        call_stack_dump_on_error= no
        call_stack_dump_activation_list=-115,-116, DEFAULT

**call_stack_dump_deactivation_list**

    **call_stack_dump_deactivation_list** is a parameter to configure a certain error number for which a call stack is not to be dumped when you configure that a call stack will be dumped for any errors. Therefore, the **call_stack_dump_deactivation_list** parameter is effective only when **call_stack_dump_on_error** is set to **yes**.

    The following example shows how to configure the parameter so that call stacks will be dumped for any errors, except the ones whose numbers are -115 and -116. ::

        call_stack_dump_on_error= yes
        call_stack_dump_deactivation_list=-115,-116

**call_stack_dump_on_error**

    **call_stack_dump_on_error** is a parameter to configure whether or not to dump a call stack when an error occurs in the database server. If this parameter is set to **no**, a call stack for any errors is not dumped. If it is set to **yes**, a call stack for all errors is dumped. The default value is **no**.

**error_log**

    **error_log** is a server/client parameter to configure the name of the error log file when an error occurs in the database server. The name of the error log file must be in the form of *<database_name>_<date>_<time>.err*. However, the naming rule of the error log file does not apply to errors for which the system cannot find the database server information. Therefore, error logs are recorded in the **cubrid.err** file. The error log file **cubrid.err** is stored in the **$CUBRID/log/server** directory.

**error_log_level**

    **error_log_level** is a server parameter to configure an error message to be stored based on severity. There are five different levels which range from **WARNING** (lowest level), to **FATAL** (highest level). The inclusion relation in messages is **FATAL** < **ERROR** < **SYNTAX** < **NOTIFICATION** < **WARNING**. The default is **NOTIFICATION**. If severity of error is **NOTIFICATION**, error messages with **NOTIFICATION**, **SYNTAX**, **ERROR** and **FATAL** levels are written to the log file.

**error_log_warning**

    **error_log_warning** is a parameter to configure whether or not error messages with a severity level of **WARNING** are to be displayed. Its default value is **no**. For this reason, you must set **error_log_warning** to **yes** to store **WARNING** messages to an error log file.

**error_log_size**

    **error_log_size** is a parameter to configure the maximum size per an error log file. The default value is **512M**. If it reaches up to the specified size, the *<database_name>_<date>_<time>.err.bak* file is created. 

.. _lock-parameters:

Concurrency/Lock-Related Parameters
-----------------------------------

The following are parameters related to concurrency control and locks of the database server. The type and value range for each parameter are as follows:

+-------------------------------------+--------+-------------+-------------+-------------+
| Parameter Name                      | Type   | Default     | Min         | Max         |
+=====================================+========+=============+=============+=============+
| deadlock_detection_interval_in_secs | float  | 1.0         | 0.1         |             |
+-------------------------------------+--------+-------------+-------------+-------------+
| isolation_level                     | int    | 4           | 4           | 6           |
+-------------------------------------+--------+-------------+-------------+-------------+
| lock_escalation                     | int    | 100,000     | 5           |             |
+-------------------------------------+--------+-------------+-------------+-------------+
| lock_timeout                        | msec   | -1(inf)     | 0(no wait)  | INT_MAX     |
+-------------------------------------+--------+-------------+-------------+-------------+
| rollback_on_lock_escalation         | bool   | no          |             |             |
+-------------------------------------+--------+-------------+-------------+-------------+

**deadlock_detection_interval_in_secs**

    **deadlock_detection_interval_in_secs** is a parameter to configure the interval (in seconds) in which deadlocks are detected for stopped transactions. If a deadlock occurs, CUBRID resolves the problem by rolling back one of the transactions. The default value is 1 second and the minimum value is 0.1 second. This value is rounded up by 0.1 sec. unit. For example, if an input value is 0.12 seconds, the value is rounded up to 0.2 seconds. Note that deadlocks cannot be detected if the detection interval is too long.

**isolation_level**

    **isolation_level** is a parameter to configure the isolation level of a transaction. The higher the isolation level, the less concurrency and the less interruption by other concurrent transactions. The **isolation_level** parameter can be configured to an integer value from 4 to 6, which represent isolation levels, or character strings. The default value is **READ COMMITTED**. For details about each isolation level and parameter values, see :ref:`transaction-isolation-level` and the following table.

    +----------------------------+-------------------------------------------------------------------------------------------+
    | Isolation Level            | isolation_level Parameter Value                                                           |
    +============================+===========================================================================================+
    | SERIALIZABLE               | "TRAN_SERIALIZABLE" or 6                                                                  |
    +----------------------------+-------------------------------------------------------------------------------------------+
    | REPEATABLE READ            | "TRAN_REP_CLASS_REP_INSTANCE" or "TRAN_REP_READ" or 5                                     |
    +----------------------------+-------------------------------------------------------------------------------------------+
    | READ COMMITTED             | "TRAN_REP_CLASS_COMMIT_INSTANCE" or "TRAN_READ_COMMITTED" or "TRAN_CURSOR_STABILITY" or 4 |
    +----------------------------+-------------------------------------------------------------------------------------------+

    *   **TRAN_SERIALIZABLE** : This isolation level ensures the highest level of consistency. For details, see :ref:`isolation-level-6`.

    *   **TRAN_REP_READ** : This isolation level prevents dirty reads, non-repeatable reads, and phantom reads due to snapshot isolation. For details, see :ref:`isolation-level-5`.

    *   **TRAN_READ_COMMITTED** : This isolation level can incur unrepeatable read. For details, see :ref:`isolation-level-4`.

    .. note::
    
        9.3 or less version supports the below levels additionally. From 10.0, concurrency can be guaranteed more because MVCC method is applied when multiple concurrent transactions are processed; therefore, the below isolation levels are not used anymore.
        
            *   **TRAN_REP_CLASS_UNCOMMIT_INSTANCE** : This isolation level can incur dirty read.
            
            *   **TRAN_COMMIT_CLASS_COMMIT_INSTANCE** : This isolation level can incur unrepeatable read. It allows modification of table schema by current transactions while data is being retrieved.

            *   **TRAN_COMMIT_CLASS_UNCOMMIT_INSTANCE** : This isolation level can incur dirty read. It allows modification of table schema by current transactions while data is being retrieved.

**lock_escalation**

    **lock_escalation** is a parameter to configure the maximum number of locks permitted before row level locking is extended to table level locking. The default value is **100,000**. If the value of the **lock_escalation** parameter is small, the overhead by memory lock management is small as well; however, the concurrency decreases. On the other hand, if the configured value is large, the overhead is large as well; however, the concurrency increases.

**lock_timeout**

    **lock_timeout** is a client parameter to configure the lock waiting time. If the lock is not permitted within the specified time period, the given transaction is canceled, and an error message is returned. If the parameter is configured to **-1**, which is the default value, the waiting time is infinite until the lock is permitted. If it is configured to **0**, there is no waiting for locks.

    You can set a unit as s, min or h, which stands for seconds, minutes or hours respectively. If you omit the unit, milliseconds(ms) will be applied, and it is rounded up to seconds. For example, 1ms will be 1s, and 1001ms will be 2s.

**rollback_on_lock_escalation**
  
    It specifies rolling back the transaction or not when the lock escalation occurs. The default is **no**. 
  
    If this parameter is specified with **yes**, the error log is written without lock escalation on the lock-escalating time and this lock escalation request is failed with rolling back the transaction.
    If it is specified with **no**, the lock escalation is performed and the transaction is continued.

    When the lock escalation occurs, record locks are transformed into a table lock and lock-releasing time can take long, so other transaction's access to the table can be impossible.
    However, if you specify the value of **lock_escalation** parameter(it specifies the number of record locks occurring the lock escalation) bigger, the system can overuse the memory resource.
    
.. _logging-parameters:

Logging-Related Parameters
--------------------------

The following are parameters related to logs used for database backup and restore. The types and value range for each parameter are as follows:

+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| Parameter Name                      | Type   | Default                    | Min                        | Max                        |
+=====================================+========+============================+============================+============================+
| adaptive_flush_control              | bool   | yes                        |                            |                            |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| background_archiving                | bool   | yes                        |                            |                            |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| checkpoint_every_size               | byte   | 10,000 *                   | 10  *                      |                            |
|                                     |        | :ref:`log_page_size <lpg>` | :ref:`log_page_size <lpg>` | :ref:`log_page_size <lpg>` |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| checkpoint_interval                 | msec   | 6min                       | 1min                       | 35,791,394min              |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| checkpoint_sleep_msecs              | msec   | 1                          | 0                          |                            |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| force_remove_log_archives           | bool   | yes                        |                            |                            |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| log_buffer_size                     | byte   | 16k *                      | 128 *                      | INT_MAX *                  |
|                                     |        | :ref:`log_page_size <lpg>` | :ref:`log_page_size <lpg>` | :ref:`log_page_size <lpg>` |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| log_max_archives                    | int    | INT_MAX                    | 0                          | INT_MAX                    |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| log_trace_flush_time                | int    | 0                          | 0                          | INT_MAX                    |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| max_flush_size_per_second           | byte   | 10,000 *                   | 1 *                        | INT_MAX *                  |
|                                     |        | :ref:`db_page_size <dpg>`  | :ref:`db_page_size <dpg>`  | :ref:`db_page_size <dpg>`  |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| remove_log_archive_interval_in_secs | sec    | 0                          | 0                          |                            |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| sync_on_flush_size                  | byte   | 200 *                      | 1 *                        | INT_MAX *                  |
|                                     |        | :ref:`db_page_size <dpg>`  | :ref:`db_page_size <dpg>`  | :ref:`db_page_size <dpg>`  |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| ddl_audit_log                       | bool   | no                         |                            |                            |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+
| ddl_audit_log_size                  | byte   | 10M                        | 10M                        | 2G                         |
+-------------------------------------+--------+----------------------------+----------------------------+----------------------------+

**adaptive_flush_control**

    **adaptive_flush_control** is a parameter used automatically to adjust the flush capacity at every 50 ms depending on the current status of the flushing operation. The default value is **yes**. That is, this capacity is increased if a large number of **INSERT** or **UPDATE** operations are concentrated at a certain point of time and the number of flushed pages reaches the **max_flush_size_per_second** parameter value; and is decreased otherwise. In the same way, you can distribute the I/O load by adjusting the flush capacity on a regular basis depending on the workload.

**background_archiving**

    **background_archiving** is a parameter used to create temporary archive logs periodically at a specific time. It is useful when balancing disk I/O load which has been caused by archiving logs. The default is **yes**.

**checkpoint_every_size**

    **checkpoint_every_size** is a parameter to configure checkpoint interval by log page. You can set a unit as B, K, M, G or T, which stands for bytes, kilobytes(KB), megabytes(MB), gigabytes(GB) or terabytes(TB) respectively. If you omit the unit, bytes will be applied. The default value is **10,000** * :ref:`log_page_size <lpg>` (**156.25M** when log_page_size is 16K).
   
    You can distribute disk I/O overload at the checkpoint by specifying lower size in the **checkpoint_every_size** parameter, especially in the environment where **INSERT** / **UPDATE** are heavily loaded at a specific time.

    Checkpoint is a job to record every modified page except for the temp page in data buffers to database volumes (disk) at a specific point. Checkpoint can shrink a restore time after the database failure because this makes the transaction logs which have been generated previously than the checkpoint time needless when the restore is processed.
    However, an efficient checkpoint interval should be properly considered because this job can occur a lot of disk I/O.
    
    .. note::
    
        There are three ways to run checkpoint in CUBRID. The following two ways are provided by the setting of cubrid.conf.
        
        *   **checkpoint_interval**: After the previous checkpoint is done, checkpoint is periodically processed at every time after the term of this parameter value.
        *   **checkpoint_every_size**: Checkpoint is periodically processed at every time after the transaction log size of this parameter value.
        
        If one condition on the above parameters is satisfied, checkpoint is processed.
        
        The following two ways are provided by a user's command.
        
        *   If you run ";checkpoint" command in the CSQL interpreter, which is run with a "DBA" user, checkpoint is processed.
                
        As a reference, if you run backup command during checkpoint, backup command is blocked until checkpoint is ended.

**checkpoint_interval**

    **checkpoint_interval** is a parameter to configure execution period of checkpoint. You can set a unit as ms, s, min or h, which stands for milliseconds, seconds, minutes or hours respectively. If you omit the unit, milliseconds(ms) will be applied, and it is rounded up to seconds. For example, 1ms will be 1s, and 1001ms will be 2s. The default value is **6min** and the minimum value is 1min.

**checkpoint_sleep_msecs**

    **checkpoint_sleep_msecs** is a parameter to let the job which flushes a buffer's data into a disk process slowly. The default is **1** (millisecond).

**force_remove_log_archives**

    **force_remove_log_archives** is a parameter to configure whether to allow the deletion of the files other than the recent log archive files whose number is specified by **log_max_archives**. The default value is **yes**.

    If the value is set to **yes**, the files will be deleted other than the recent log archive files for which the number is specified by **log_max_archives**. 

    If it is set to **no**, the log archive files will not be deleted. Exceptionally, if **ha_mode** is set to **on**, the files other than the log archive files required for the HA-related processes and the recent log archive files of which the number is specified by **log_max_archives** will be deleted.

    For setting up the CUBRID HA environment, see :ref:`ha-configuration`.
    
**log_buffer_size**

    **log_buffer_size** is a parameter to configure the size of log buffer to be cached in the memory. You can set a unit as B, K, M, G or T, which stands for bytes, kilobytes(KB), megabytes(MB), gigabytes(GB) or terabytes(TB) respectively. If you omit the unit, bytes will be applied. The default value is 16k * :ref:`log_page_size <dpg>` (**256M** when log_page_size is 16K).

    If the value of the **log_buffer_size** parameter is large, performance can be improved (due to the decrease in disk I/O) in an environment where transactions are long and numerous. Moreover, CUBRID Multiversion Concurrency Control system relies on log to access previous row versions and to vacuum invisible versions from database. It is recommended to configure an appropriate value considering the memory size and operations of the system where CUBRID is installed.

    *   Required memory size = the size of log buffer (**log_buffer_size**)

.. _log_max_archives: 

**log_max_archives**

    **log_max_archives** is a parameter to configure the maximum number of archive log files. The minimum value is 0 and default value is **INT_MAX** (2,147,483,647). Its operations can differ depending on the configuration of **force_remove_log_archives**. For example, when **log_max_archives** is 3 and **force_remove_log_archives** is **yes** in the cubrid.conf file, the most recent three archive log files are recorded and when a fourth archiving log file is generated, the oldest archive log file is automatically deleted; the information about the deleted archive logs are recorded in the ***_lginf** file.

    However, if an active transaction still refers to an existing archive log file, the archive log file will not be deleted. That is, if a transaction starts at the point that the first archive log file is generated, and it is still active until the fifth archive log is generated, the first archive log file cannot be deleted.

    Also, if the information of archive logs is not applied to database volumes, these are not deleted. (Archive logs after which a checkpoint has occurred keep the information of modified pages of a data buffer; therefore, they are required to restore a database.)

    If you change the value of **log_max_archives** dynamically during database operation, changed value will be applied when a new log archive file is created. For example, if you change this value from 10 to 5, old 5 files will be deleted when a new log archive file is created.
    
    For setting up the CUBRID HA environment, see :ref:`ha-configuration`.

    .. note::
    
        In 2008 R4.3 or lower and in 9.1, **log_max_archives** was also used for specifying the maximum number of keeping replication log files in HA environment. From 2008 R4.4 and 9.2, :ref:`ha_copy_log_max_archives <ha_copy_log_max_archives>` of  cubrid_ha.conf is in charge of this role.

**log_trace_flush_time** 
  
    When the log flushing time takes more time than the time you set in this parameter, this event is recorded in the log of the database server.

    The example of written information is as below.
      
    :: 
      
        03/18/14 10:20:45.889 - LOG_FLUSH_THREAD_WAIT 
          total flush count: 1 page(s) 
          total flush time: 310 ms 
          time waiting for log writer: 308 ms 
          last log writer info 
            client: DBA@cdbs037.cub|copylogdb(15312) 
            time spent by log writer: 308 ms 
      
    *   LOG_FLUSH_THREAD_WAIT: Event name
    *   total flush count: The number of flushed pages when the event occurs
    *   total flush time: Total spent time when the log is flushed
    *   time waiting for log writer: The time LFT(Log Flushing Thread) has been waiting for LWT(Log Writer Thread)
    *   last log writer info 
      
        *   DBA@cdbs037.cub|copylogdb(15312): copylogdb information related to LWT which let LFT wait <user@host_name|client_name(pid)> 
        *   time spent by log writer: Time spend by LWT measured in LWT; in general, this value is the same as the value of "time waiting for log writer")

**max_flush_size_per_second**

    **max_flush_size_per_second** is a parameter to configure the maximum flush capacity when the flushing operation is performed from a buffer to a disk. You can set a unit as B, K, M, G or T, which stands for bytes, kilobytes(KB), megabytes(MB), gigabytes(GB) or terabytes(TB) respectively. If you omit the unit, bytes will be applied. The default value is 10,000 * :ref:`db_page_size <dpg>` (**156.25M** when db_page_size is 16K).
    That is, you can prevent concentration of I/O load at a certain point of time by configuring this parameter to control the maximum flush capacity per second.

    If a large number of **INSERT** or **UPDATE** operations are concentrated at a certain point of time, and the flush capacity reaches the maximum capacity set by this parameter, only log pages are flushed to the disk, and data pages are no longer flushed. Therefore, you must set an appropriate value for this parameter considering the workload of the service environment.

**remove_log_archive_interval_in_secs**

    Archive logs which exceed the specified number in the **log_max_archives** are removed when checkpoint occurs. By the way, many archive logs can be removed at once frequently after being piled up when jobs such as data migration or big data batch processing are performed. If files are removed at once like these, I/O overhead of database server is rapidly risen; therefore, we need to decrease this burden.
    
    **remove_log_archive_interval_in_secs** parameter lets archive logs delete slowly to shrink this burden. The default is **0** (second). In the situations which jobs like big data batch processing occur frequently, it is recommended to set the deletion interval as a 60 seconds if the disk space is enough.

**sync_on_flush_size**

    **sync_on_flush_size** is a parameter to configure the interval in pages between after data and log pages are flushed from buffer and before they are synchronized with FILE I/O of operating system. The default value is 200 * :ref:`db_page_size <dpg>` (**3.125M** when db_page_size is 16K). That is, the CUBRID Server performs synchronization with the FILE I/O of the operating system whenever 200 pages have been flushed. This is also a parameter related to I/O load.

**ddl_audit_log**
	**ddl_audit_log** is a parameter to turn on/off DDL logging facility. The default value is no.
	If this value is set to yes, all DDL executed will be logged into the logfile. The path of log files is $CUBRID/log/ddl_audit, and refer to :doc:/admin/ddl_audit for each DDL AUDIT log file name and description of log files in detail.

**ddl_audit_log_size**
	**ddl_audit_log_size** specifies the maximum size of the DDL AUDIT log file. If the ddl audit log file is larger than the specified size, that ddl audit log file is backed up with the name of .bak appended to the ddl audit log file, and new recording will be started with the file from the beginning of the file. You can set the size with a size unit as B, K, M, or G, which stand for bytes, kilobytes (KB), megabytes (MB), and gigabytes (GB) respectively. If you omit the size unit, bytes will be applied. The default is 10M, and it can be set up to 2G.
	
.. _transaction-parameters:

Transaction Processing-Related Parameters
-----------------------------------------

The following are parameters for improving transaction commit performance. The type and value range for each parameter are as follows:

+---------------------------------+--------+------------+------------+------------+
| Parameter Name                  | Type   | Default    | Min        | Max        |
+=================================+========+============+============+============+
| async_commit                    | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| group_commit_interval_in_msecs  | msec   | 0          | 0          |            |
+---------------------------------+--------+------------+------------+------------+

**async_commit**

    **async_commit** is a parameter used to activate the asynchronous commit functionality. If the parameter is set to **no**, which is the default value, the asynchronous commit is not performed; if it is set to **yes**, the asynchronous commit is executed. The asynchronous commit is a functionality that improves commit performance by completing the commit for the client before commit logs are flushed on the disk and having the log flush thread (LFT) perform log flushing in the background. Note that already committed transactions cannot be restored if a failure occurs on the database server before log flushing is performed.

**group_commit_interval_in_msecs**

    **group_commit_interval_in_msecs** is a parameter to configure the interval (in milliseconds), at which the group commit is to be performed. If the parameter is configured to **0**, which is the default value, the group commit is not performed. The group commit is a functionality that improves commit performance by combining multiple commits that occurred in the specified time period into a group so that commit logs are flushed on the disk at once.

.. _stmt-type-parameters:

Statement/Type-Related Parameters
---------------------------------

The following are parameters related to SQL statements and data types supported by CUBRID. The type and value range for each parameter are as follows:

+---------------------------------+--------+------------+------------+------------+
| Parameter Name                  | Type   | Default    | Min        | Max        |
+=================================+========+============+============+============+
| add_column_update_hard_default  | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| alter_table_change_type_strict  | bool   | yes        |            |            |
+---------------------------------+--------+------------+------------+------------+
| allow_truncated_string          | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| ansi_quotes                     | bool   | yes        |            |            |
+---------------------------------+--------+------------+------------+------------+
| block_ddl_statement             | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| block_nowhere_statement         | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| compat_numeric_division_scale   | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| create_table_reuseoid           | bool   | yes        |            |            |
+---------------------------------+--------+------------+------------+------------+
| cte_max_recursions              | int    | 2,000      | 2          | 1,000,000  |
+---------------------------------+--------+------------+------------+------------+
| default_week_format             | int    | 0          |            |            |
+---------------------------------+--------+------------+------------+------------+
| group_concat_max_len            | byte   | 1,024      | 4          | INT_MAX    |
+---------------------------------+--------+------------+------------+------------+
| intl_check_input_string         | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| intl_collation                  | string |            |            |            |
+---------------------------------+--------+------------+------------+------------+
| intl_date_lang                  | string |            |            |            |
+---------------------------------+--------+------------+------------+------------+
| intl_number_lang                | string |            |            |            |
+---------------------------------+--------+------------+------------+------------+
| json_max_array_idx              | int    | 65,536     | 1,024      | 1,048,576  |
+---------------------------------+--------+------------+------------+------------+
| no_backslash_escapes            | bool   | yes        |            |            |
+---------------------------------+--------+------------+------------+------------+
| only_full_group_by              | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| oracle_compat_number_behavior   | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| oracle_style_empty_string       | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| pipes_as_concat                 | bool   | yes        |            |            |
+---------------------------------+--------+------------+------------+------------+
| plus_as_concat                  | bool   | yes        |            |            |
+---------------------------------+--------+------------+------------+------------+
| require_like_escape_character   | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| return_null_on_function_errors  | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| string_max_size_bytes           | byte   | 1,048,576  | 64         | 33,554,432 |
+---------------------------------+--------+------------+------------+------------+
| unicode_input_normalization     | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| unicode_output_normalization    | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+
| update_use_attribute_references | bool   | no         |            |            |
+---------------------------------+--------+------------+------------+------------+

**add_column_update_hard_default**

    **add_column_update_hard_default** is a parameter to configure whether or not to provide the hard default value as the input value for a column when you add a new column to the **ALTER TABLE ... ADD COLUMN** clause.

    When there is **NOT NULL** constraint and no **DEFAULT** constraint, if a value of this parameter is set to **yes**, the value of newly added column will be inserted as hard default value; if it is set to **no**, CUBRID returns an error. For the hard default for each type, see the :ref:`change-column` of the **ALTER TABLE** statement.

    .. code-block:: sql
                    
        SET SYSTEM PARAMETERS 'add_column_update_hard_default=yes';
         
        CREATE TABLE tbl (i int);
        INSERT INTO tbl VALUES (1),(2);
        ALTER TABLE tbl ADD COLUMN j INT NOT NULL;
         
        SELECT * FROM tbl;
         
    ::     
    
                    i          j
        =========================
                    1          0
                    2          0

    .. code-block:: sql

        SET SYSTEM PARAMETERS 'add_column_update_hard_default=no';
         
        CREATE TABLE tbl (i INT);
        INSERT INTO tbl VALUES (1),(2);
        ALTER TABLE tbl ADD COLUMN j INT NOT NULL;

    ::
    
        ERROR: Cannot add NOT NULL constraint for attribute "j": there are existing NULL values for this attribute.

**alter_table_change_type_strict**

    **alter_table_change_type_strict** is a parameter to configure whether to allow the conversion of column values according to the type change, and the default value is **yes**. If a value for this parameter is set to **no**, the value may be changed when you change the column types or when you add **NOT NULL** constraints; if it is set to **yes**, the value does not change. For details, see CHANGE Clause in the :ref:`change-column`.

**allow_truncated_string**

    **allow_truncated_string** is a parameter to configure whether to allow the truncation of string values according to the string manipulation operations used in insert or update query, and the default value is **no**. If the value for this parameter is set to **no**, the string value is not allowed to be truncated when you do operation for any string related to insert or update query; however the string related to select query may be truncated regardless of this configuration. If it is set to **yes**, the string value may be truncated regardless of the type of (INSERT/UPDATE/SELECT) query.

**ansi_quotes**

    **ansi_quotes** is a parameter used to enclose symbols and character string to handle identifiers. The default value is **yes**. If this parameter value is set to **yes**, double quotations are handled as identifier symbols and single quotations are handled as character string symbols. If it is set to **no**, both double and single quotations are handled as character string symbols.

.. _block_ddl_statement:

**block_ddl_statement**

    **block_ddl_statement** is a parameter used to limit the execution of DDL (Data Definition Language) statements by the client. If the parameter is set to **no**, the given client is allowed to execute DDL statements. If it is set to **yes**, the client is not permitted to execute DDL statements. The default value is **no**.

.. _block_nowhere_statement:

**block_nowhere_statement**

    **block_nowhere_statement** is a parameter used to limit the execution of **UPDATE** / **DELETE** statements without a condition clause (**WHERE**) by the client. If the parameter is set to **no**, the given client is allowed to execute **UPDATE** / **DELETE** statements without a condition clause. If it is set to **yes**, the client is not permitted to execute **UPDATE** / **DELETE** statements without a condition clause. The default value is **no**.

**compat_numeric_division_scale**

    **compat_numeric_division_scale** is a parameter to configure the scale to be displayed in the result (quotient) of a division operation. If the parameter is set to **no**, the scale of the quotient is 9, if it is set to **yes**, the scale is determined by that of the operand. The default value is **no**.

**create_table_reuseoid**

   **create_table_reuseoid** is a parameter to specify whether to use the **REUSE_OID** or **DONT_REUSE_OID** option when creating a table without table option. If it is set to **yes**, the table is created with **REUSE_OID** option. The default value is **yes**.

   For detail, see :ref:`reuse-oid` and :ref:`dont-reuse-oid` .

**cte_max_recursions**

    **cte_max_recursions** is a parameter to limit the maximum number of iterations when executing the recursive part of the CTE (Common Table Expressions) statement. This avoids infinite loop and potential issues produced by the size of temporary lists.

**default_week_format**

    **default_week_format** is a parameter to configure default value for the *mode* attribute of the :func:`WEEK` function. The default value is **0**. For details, see :func:`WEEK`.

**intl_check_input_string**

    **intl_check_input_string** is a parameter to determine whether or not to check that string entered is correctly corresponded to character set used. The default value is **no**. If this value is **no** and character set is UTF-8 and incorrect data is enter which violate UTF-8 byte sequence, it can show abnormal behavior or database server and applications can be terminated abnormally. However, if it is guaranteed this problem does not happen, it has advantage in performance not to do it.

    UTF-8 and EUC-KR can be checked; ISO-8859-1 is one-byte encoding so it does not have to be checked because every byte is valid.

**group_concat_max_len**

    **group_concat_max_len** is a parameter used to limit the return value size of the :func:`GROUP_CONCAT` function.
    You can set a unit as B, K, M, G or T, which stands for bytes, kilobytes(KB), megabytes(MB), gigabytes(GB) or terabytes(TB) respectively. If you omit the unit, bytes will be applied. The default value is **1,024**. The minimum value is 4 and the maximum value is INT_MAX (about 2G) bytes. 
    
    This function is affected by **string_max_size_bytes** parameter; if the value of **group_concat_max_len** is greater than the value **string_max_size_bytes** and the result size of **GROUP_CONCAT** exceeds the value of **string_max_size_bytes**, an error occurs.

**intl_check_input_string**

    **intl_check_input_string** is a parameter to determine whether or not to check that string entered is correctly corresponded to character set used. The default value is **no**. If this value is no and character set is UTF-8 and incorrect data is enter which violate UTF-8 byte sequence, it can show abnormal behavior or database server and applications can be terminated abnormally. However, if it is guaranteed this problem does not happen, it has advantage in performance not to do it.

    UTF-8 and EUC-KR can be checked; ISO-8859-1 is one-byte encoding so it does not have to be checked because every byte is valid.

**intl_collation**

    **intl_collation** is a parameter which specifies a collation name about a specific application client. Specifying this parameter is the same as changing a collation of application client by using "SET NAMES" statement. Specifying a collation includes a charset.
    
    The following two statements behave the same.

    .. code-block:: sql

        SET NAMES utf8;
        SET SYSTEM PARAMETERS 'intl_collation=utf8_bin';

    For an available value of **intl_collation**, see :ref:`collation-setting`.
    
**intl_date_lang**

    **intl_date_lang** is a parameter used to input/output the values of **TIME**, **DATE**, **DATETIME**, and **TIMESTAMP**. If language name is omitted, it specifies a locale format of string of localized calendar (month, weekday, and AM/PM).

    The values allowed are as follows: Note that to use all values, locale library should be configured except built-in locale. For configuring locale, see :ref:`locale-setting`.

    +--------------+-----------------------------+
    | Language     | Locale Name of Language     |
    +==============+=============================+
    | English      | en_US                       |
    +--------------+-----------------------------+
    | German       | de_DE                       |
    +--------------+-----------------------------+
    | Spanish      | es_ES                       |
    +--------------+-----------------------------+
    | French       | fr_FR                       |
    +--------------+-----------------------------+
    | Italian      | it_IT                       |
    +--------------+-----------------------------+
    | Japanese     | ja_JP                       |
    +--------------+-----------------------------+
    | Cambodian    | km_KH                       |
    +--------------+-----------------------------+
    | Korean       | ko_KR                       |
    +--------------+-----------------------------+
    | Turkish      | tr_TR                       |
    +--------------+-----------------------------+
    | Vietnamese   | vi_VN                       |
    +--------------+-----------------------------+
    | Chinese      | zh_CN                       |
    +--------------+-----------------------------+
    | Romanian     | ro_RO                       |
    +--------------+-----------------------------+

    The function recognizing input string based on calendar format of specified language is as follows:

    *   :func:`TO_DATE`
    *   :func:`TO_TIME`
    *   :func:`TO_DATETIME`
    *   :func:`TO_TIMESTAMP`
    *   :func:`STR_TO_DATE`

    The function outputting string based on calendar format of specified language is as follows:

    *   :func:`TO_CHAR`
    *   :func:`DATE_FORMAT`
    *   :func:`TIME_FORMAT`

**intl_number_lang**

    **intl_number_lang** is a parameter used to specify locale applied when numeric format is assigned to input/output string in the function where a string is converted to number or number is converted to string. A delimiter and decimal symbol are used for numeric localization. In general, a comma and period are used; however, it can be changeable based on locale. For example, while number 1000.12 is written as 1,000.12 in most locale, it is written as 1.000,12 in tr_TR locale.

    The function recognizing input string based on calendar format of specified language is as follows:

    *   :func:`TO_NUMBER`

    The function outputting string based on calendar format of specified language is as follows:

    *   :func:`FORMAT`
    *   :func:`TO_CHAR`

**json_max_array_idx**

    **json_max_array_idx** is a parameter used by JSON functions to set a \
    \limit on the size of arrays after changes.

    If a very large index is fed to functions like **JSON_ARRAY_INSERT** \
    \by accident, the JSON document will occupy a lot of memory and disk \
    \space. Use this parameter to limit the risks.

    The limit is not applied to arrays generated by parsing strings.

**no_backslash_escapes**

    **no_backslash_escapes** is a parameter to configure whether or not to use backslash (\\) as an escape character, and the default value is **yes**. If a value for this parameter is set to **no**, backslash (\\) will be used as an escape character; if it is set to **yes**, backslash (\\) will be used as a normal character. For example, if this value is set to **no**, "\\n" means a newline character. For details, see :ref:`escape-characters`.

**only_full_group_by**

    **only_full_group_by** is a parameter to configure whether or not to use extended syntax about using **GROUP BY** statement.

    If this parameter value is set to **no**, an extended syntax is applied thus, a column that is not specified in the **GROUP BY** statement can be specified in the **SELECT** column list. If it is set to yes, a column that is only specified in the **GROUP BY** statement can be the **SELECT** column list.

    The default value is **no**. Therefore, specify the **only_full_group_by** parameter value to **yes** to execute queries by SQL standards. Because the extended syntax is not applied in this case, an error below is displayed. ::

        ERROR: Attributes exposed in aggregate queries must also appear in the group by clause.

.. _oracle_compat_number_behavior:

**oracle_compat_number_behavior**

    **oracle_compat_number_behavior** is a parameter used to improve compatibility with other DBMS (Database Management Systems); for NUMERIC type, DOUBLE and FLOAT types, it does not display decimal point trailing 0, and in case of DOUBLE and FLOAT, it does not display the exponent. For example, if this parameter setting value is **no**, the result of a query that searches the a_double table composed of DOUBLE types is displayed in the form of an exponent as shown below, but if the parameter setting value is **yes**, only decimal point is displayed.

    .. code-block:: sql

        csql> ;get oracle_compat_number_behavior

        === Get Param Input ===

        oracle_compat_number_behavior=n

        SELECT a FROM a_double;

    ::

                                 a
        ==========================
             1.234567890123457e+19
             1.234567890123457e-08
             1.230000000000000e-08
             1.000000000000000e+00
             1.200000000000000e+00
            -4.939030300000000e-03
            -1.293934894993939e+18
            -1.938943893939394e+16

    ::

        csql> ;get oracle_compat_number_behavior

        === Get Param Input ===

        oracle_compat_number_behavior=y

        SELECT a FROM a_double;

    ::

                                 a
        ==========================
              12345678901234570000
          0.00000001234567890123457
                      0.0000000123
                                 1
                               1.2
                     -0.0049390303
              -1293934894993939000
                -19389438939393940

    Also, when **oracle_compat_number_behavior** is set to **yes**, the type of division operation for integer numbers becomes a real number type (that is, a NUMERIC type) not an integer type. In the example below, if this parameter is **yes**, affected integer types are INT, SHORT and BIGINT.

    .. code-block:: sql

        csql> ;get oracle_compat_number_behavior

        === Get Param Input ===

        oracle_compat_number_behavior=y

        SELECT 1/2;

    ::

               1/2
        ==========
               0.5

    .. note:: 

        The oracle_compat_number_behavior is only applied, when reading NUMERIC, DOUBLE, and FLOAT type data in string format in JDBC/CCI.  The related functions of JDBC/CCI are as follows.

        * JDBC : getString(int columnIndex), getString(String columnLabel), getObject(int columnIndex) , getObject(String columnLabel)

        * CCI :  cci_get_data (CCI_A_TYPE_STR as type), Example) cci_get_data(req, i, CCI_A_TYPE_STR, &data, &ind)
		 
.. _oracle_style_empty_string:

**oracle_style_empty_string**

    **oracle_style_empty_string** is a parameter used to improve compatibility with other DBMS (Database Management Systems); it specifies to process empty string and **NULL** as the same value. The default is **no**. If the **oracle_style_empty_string** parameter is set to **no**, the character string is processed as a valid string; if it is set to **yes**, according to each function, the empty string is processed as **NULL** or **NULL** is processed as the empty string.

    .. note:: 

        Other functions except below functions are not affected by **oracle_style_empty_string** parameter.
        
        *   Functions processing an empty string and NULL into NULL when **oracle_style_empty_string=yes**.

            *   :func:`ASCII`
            *   :func:`CONCAT_WS`
            *   :func:`ELT`
            *   :func:`FIELD`
            *   :func:`FIND_IN_SET`
            *   :func:`FROM_BASE64`
            *   :func:`INSERT`
            *   :func:`INSTR`
            *   :func:`LOWER`
            *   :func:`LEFT`
            *   :func:`LOCATE`
            *   :func:`LPAD`
            *   :func:`LTRIM`
            *   :func:`MID`
            *   :func:`POSITION`
            *   :func:`REPEAT`
            *   :func:`REVERSE`
            *   :func:`RIGHT`
            *   :func:`RPAD`
            *   :func:`RTRIM`
            *   :func:`SPACE`
            *   :func:`STRCMP`
            *   :func:`SUBSTR`
            *   :func:`SUBSTRING`
            *   :func:`SUBSTRING_INDEX`
            *   :func:`TO_BASE64`
            *   :func:`TRANSLATE`
            *   :func:`TRIM`
            *   :func:`UPPER`
            
        *   Functions processing an empty string and NULL into an empty string when **oracle_style_empty_string=yes**.
        
            *   :func:`CONCAT`
            *   :func:`REPLACE`

            
    .. note::
    
        :func:`REPLACE` function has the different behavior in the previous versions of 10.0 when **oracle_style_empty_string=yes**.
        
        .. code-block:: sql
        
            SELECT REPLACE ('abc', 'a', '');
        
        In the above query, the version of 10.0 or more return 'bc' because it processes the input of an empty string as an empty string; the previous version of 10.0 returns NULL because it processes the input of an empty string as NULL.

**pipes_as_concat**

    **pipes_as_concat** is a parameter to configure how to handle a double pipe symbol. The default value is **yes**. If this parameter value is set to **yes**, a double pipe symbol is handled as a concatenation operator if **no**, it is handled as the **OR** operator.

**plus_as_concat**

    **plus_as_concat** is a parameter to configure the plus (+) operator, and the default value is **yes**. If a value for this parameter is set to **yes**, the plus (+) operator will be interpreted as a concatenation operator; if it is set to **no**, the operator will be interpreted as a numeric operator.

    .. code-block:: sql

        -- plus_as_concat = yes
        SELECT '1'+'1';
        
    ::
    
                 '1'+'1'
        ======================
                 '11'  

    .. code-block:: sql
                 
        SELECT '1'+'a';
        
    ::
         
                 '1'+'a'
        ======================
                 '1a'

    .. code-block:: sql
                 
        -- plus_as_concat = no
        SELECT '1'+'1';
        
    ::
    
                        '1'+'1'
        ==========================
         2.000000000000000e+000
    
    .. code-block:: sql
    
        SELECT '1'+'a';
    
    ::
    
        ERROR: Cannot coerce 'a' to type double.

**require_like_escape_character**

    **require_like_escape_character** is parameter to configure whether or not to use an ESCAPE character in the **LIKE** clause, and the default value is **no**. If a value for this parameter is set to **yes** and a value for **no_backslash_escapes** is set to **no**, backslash (\\) will be used as an ESCAPE character in the strings of the LIKE clause, otherwise you should specify an ESCAPE character by using the **LIKE ... ESCAPE** clause. For details, see :ref:`like-expr`.

**return_null_on_function_errors**

    **return_null_on_function_errors** is a parameter used to define actions when errors occur in some SQL functions, and the default value is **no**. If a value for this parameter is set to **yes**, **NULL** is returned; if it is set to **no**, an error is returned when the error occurs in functions, and the related message is displayed.

    The following SQL functions are affected by this system parameter.

    **Date/Time functions**
    
    *   :func:`ADDDATE`
    *   :func:`ADDTIME`
    *   :func:`DATEDIFF`
    *   :func:`DAY`
    *   :func:`DAYOFMONTH`
    *   :func:`DAYOFWEEK`
    *   :func:`DAYOFYEAR`
    *   :func:`FROM_DAYS`
    *   :func:`FROM_UNIXTIME`
    *   :func:`HOUR`
    *   :func:`LAST_DAY`
    *   :func:`MAKEDATE`
    *   :func:`MAKETIME`
    *   :func:`MINUTE`
    *   :func:`MONTH`
    *   :func:`QUARTER`
    *   :func:`SEC_TO_TIME`
    *   :func:`SECOND`
    *   :func:`TIME`
    *   :func:`TIME_TO_SEC`
    *   :func:`TIMEDIFF`
    *   :func:`TO_DAYS`
    *   :func:`WEEK`
    *   :func:`WEEKDAY`
    *   :func:`YEAR`

    **String functions**
    
    *   :func:`ASCII`
    *   :func:`BIN`
    *   :func:`BIT_LENGTH`
    *   :func:`CHR`
    
    **Numeric functions**
    
    *   :func:`ABS`
    *   :func:`ACOS`
    *   :func:`ASIN`
    *   :func:`ATAN`
    *   :func:`ATAN2`
    *   :func:`CEIL`
    *   :func:`CONV`
    *   :func:`COS`
    *   :func:`COT`
    *   :func:`DEGREES`
    *   :func:`EXP`
    *   :func:`FLOOR`
    *   :func:`LN`
    *   :func:`LOG2`
    *   :func:`LOG10`
    *   :func:`MOD`
    *   :func:`POW`
    *   :func:`RADIANS`
    *   :func:`SIGN`
    *   :func:`SIN`
    *   :func:`SQRT`
    *   :func:`TAN`
    *   :func:`TRUNC`
    *   :func:`WIDTH_BUCKET`
    
    .. code-block:: sql

        SET SYSTEM PARAMETERS 'return_null_on_function_errors=no';         
        SELECT YEAR('12:34:56');
        
    ::
    
        ERROR: Conversion error in time format.
    
    .. code-block:: sql
    
        SET SYSTEM PARAMETERS 'return_null_on_function_errors=yes';         
        SELECT YEAR('12:34:56');
        
    ::
    
           year('12:34:56')
        ======================
           NULL

**string_max_size_bytes**

    **string_max_size_bytes** is a parameter to define the maximum byte allowable in string functions or operators. 
    You can set a unit as B, K, M, G or T, which stands for bytes, kilobytes(KB), megabytes(MB), gigabytes(GB) or terabytes(TB) respectively. If you omit the unit, bytes will be applied. The default value is **1,048,576**\ (1M). The minimum value is 64 and the maximum value is 33,554,432(32M).

    The functions and operators affected by this parameter are as follows:

    *   :func:`SPACE`
    *   :func:`CONCAT`
    *   :func:`CONCAT_WS`
    *   '**+**': Operand of string
    *   :func:`REPEAT`
    *   :func:`GROUP_CONCAT`\: This function is affected not only by **string_max_size_bytes** parameter, but also by **group_concat_max_len**.
    *   :func:`INSERT` function

.. _unicode_input_normalization:

**unicode_input_normalization**

    **unicode_input_normalization** is a parameter to determine whether or not to input unicode stored in system level or not. The default value is **no**.

    In general, unicode text can be stored in "fully composed" or "fully decomposed". When character 'Ä' has 00C4 (if it is encoded in UTF-8, it becomes 2 bytes of C3 84) which is only one code point. In "fully decomposed" mode, it has two code points/characters. It is 0041 (character "A") and 0308(COMBINING DIAERESIS). In case of UTF-8 encoding, it becomes 3 bytes of 41 CC 88.

    CUBRID can work with fully composed unicode. For clients which have fully decomposed texts, configure the value of **unicode_input_normalization** to yes so that it can be converted to fully composed mode; and then it can be reverted to fully decomposed mode. For normalization of unicode encapsulation of CUBRID, compatibility equivalence is not applied. In general, normalization of unicode is not possible to revert after composition, CUBRID supports revert for characters as many as possible, and it applies normalization of unicode encapsulation. The characteristics of CUBRID normalization are as follows:

    *   In case of language specific, normalization does not depend on locale. 
    
        If one or more locale can be used, this means every CAS/CSQL process, not CUBRID server. The **unicode_input_normalization** system parameter determines whether composition of input codes by normalization in system level. The **unicode_output_normalization** system parameter determines whether composition of output codes by normalization in system level.

    *   Collation and normalization does not have direct relationship. 
    
        Even though the value of **unicode_input_normalization** is no, the string of extensible collation (utf8_de_exp, utf8_jap_exp, utf8_km_exp) is properly sorted fully decomposed mode, it is not intended; it is side-effect of UCA(Unicode Collation Algorithm). The extensible collation is implemented only with fully composed texts.

    *   In CUBRID, composition and decomposition for normalization does not work separately. 
    
        It is generally used when **unicode_input_normalization** and **unicode_output_normalization** are yes. In this case, codes entered from clients are stored in composed mode and output in decomposed mode.

    If the application client sends the decomposed text data into CUBRID, let CUBRID deal with the composed code, by setting **unicode_input_normalization** as **yes**.
    
    If the application client can deal with the decomposed text data only, let CUBRID always send the decomposed code, by setting **unicode_output_normalization** as **yes**.

    If the application client knows both of input and output, leave the setting **unicode_input_normalization** and **unicode_output_normalization** as **no**.

    For more details, see :doc:`/sql/i18n`.

**unicode_output_normalization**

    **unicode_output_normalization** is a parameter to determine whether or not to output unicode stored in system level. The default value is **no**. For details, see the above **unicode_input_normalization** description.

.. _update_use_attribute_references:

**update_use_attribute_references** 

    **update_use_attribute_references** is a parameter whether a value X of a column to be updated in an **UPDATE** statement affects to another column's update which uses X. The default is **no**.
    
    The below result of an **UPDATE** statement is dependent on the value of **update_use_attribute_references** parameter.
      
    .. code-block:: sql 

        CREATE TABLE tbl(a INT, b INT); 
        INSERT INTO tbl values (10, NULL); 

        UPDATE tbl SET a=1, b=a; 
    
    If this parameter's value is **yes**, the updated value of the column "b" will be "1" which is affected by "a=1". 

    .. code-block:: sql 
      
        SELECT * FROM tbl; 

    :: 
      
        1, 1 
          
    If this parameter's value is **no**, the updated value of the column "b" will be "NULL", which is affected by the value of "a" stored in the record, not by "a=1". 

    .. code-block:: sql 
      
        SELECT * FROM tbl; 
          
    ::
      
        1, NULL

.. _thread-parameters:

Thread-Related Parameters
-------------------------

Thread management can be configured by threads parameters. The type and value range for each parameter are as follows:

+---------------------------------------+--------+-------------------+----------+----------+
| Parameter Name                        | Type   | Default           | Min      | Max      |
+=======================================+========+===================+==========+==========+
| thread_connection_pooling             | bool   | true              |          |          |
+---------------------------------------+--------+-------------------+----------+----------+
| thread_connection_timeout_seconds     | int    | 300               | -1       | 3600     |
+---------------------------------------+--------+-------------------+----------+----------+
| thread_worker_pooling                 | bool   | true              |          |          |
+---------------------------------------+--------+-------------------+----------+----------+
| thread_core_count                     | int    | # of system core  | 1        | 1024     |
+---------------------------------------+--------+-------------------+----------+----------+
| thread_worker_timeout_seconds         | int    | 300               | -1       | 3600     |
+---------------------------------------+--------+-------------------+----------+----------+
| loaddb_worker_count                   | bool   | 8                 | 2        | 64       |
+---------------------------------------+--------+-------------------+----------+----------+

**thread_connection_pooling**

    If **thread_connection_pooling** parameter is true, all threads used for client connection management are pooled on server boot.

**thread_connection_timeout_seconds**

    **thread_connection_timeout_seconds** is a parameter that configures \
    \wait time before stopping for threads handling connection management. \
    \After closing a connection, the thread will wait the value of the \
    \parameter expressed in seconds to be assigned a new connection. If no \
    \connection is assigned and the wait time expires, the thread stops. \
    \Another thread may be started the next time a connection comes. \
    \If parameter value is **-1**, threads never stop. They sleep until \
    \they are given a new assignment.

**thread_worker_pooling**

    If **thread_worker_pooling** parameter is true, all threads used for client requests execution are pooled on server boot.

**thread_core_count**

    The number of groups of pooled threads is configured according to the **thread_core_count** parameter. The default value is set to the number of system cores.
    If the number of threads in a group does not reach 3 or more according to the parameter value, the system adjusts this value so that at least 3 threads belong to each group.

**thread_worker_timeout_seconds**

    **thread_worker_timeout_seconds** is a parameter that configures \
    \wait time before stopping for threads handling client requests. \
    \After executing a request, the thread will wait the value of the \
    \parameter expressed in seconds to be assigned a request. If no \
    \client request is assigned and the wait time expires, the thread stops. \
    \Another thread may be started the next time a client request comes. \
    \If parameter value is **-1**, threads never stop. They sleep until \
    \they are given a new assignment.

**loaddb_worker_count**

    **loaddb_worker_count** is a parameter that configures the maximum \
    \number of threads that can be dedicated for **loaddb** sessions. \
    \If a single **loaddb** session runs, it may use all threads. \
    \If multiple **loaddb** sessions run concurrently, the total number \
    \of threads of all sessions cannot exceed the parameter's value.

.. _timezone-parameters:

Timezone Parameter
------------------

The following are the parameters related to timezone. The type and the value range for each parameter are as follows:

+-------------------------------+--------+-------------------+----------+----------+
| Parameter Name                | Type   | Default           | Min      | Max      |
+===============================+========+===================+==========+==========+
| server_timezone               | string | OS timezone       |          |          |
+-------------------------------+--------+-------------------+----------+----------+
| timezone                      | string | server_timezone   |          |          |
+-------------------------------+--------+-------------------+----------+----------+
| tz_leap_second_support        | bool   | no                |          |          |
+-------------------------------+--------+-------------------+----------+----------+

*   **timezone**

    Specifies a timezone for a session. The default is a value of **server_timezone**. This value can be specified by a timezone offset (e.g. +01:00, +02) or a timezone region name (e.g. Asia/Seoul). This value can be changed during operating database.

*   **server_timezone**

    Specifies a timezone for a server. The default is a OS timezone. To apply the changed value, database should be restarted.
    The timezone of operating system is read depending on the operating system and information found in operating system configuration files:
	
     - on Windows, the API tzset() function and tzname[0] variable are used to retrieve an Windows style timezone name. This name is translated into IANA/CUBRID style name using the CUBRID mapping data (the mapping file is %CUBRID%\\timezones\\tzdata\\windowsZones.xml).
     - on Linux, CUBRID attempts to read and parse the file "/etc/sysconfig/clock". If this file is not available, then the value of link "/etc/localtime" is read and used.
     - on AIX, the value of "TZ" operating system environment variable is used.
	 
    On all operating systems, if the server_timezone is not specified, and the value for timezone from operating system cannot be read, then "Asia/Seoul" zone is used as server timezone.

    
*   **tz_leap_second_support**

    Specifies to support a leap second or not as yes or no. The default is **no**. 
    
    A leap second is a one-second adjustment that is occasionally applied to Coordinated Universal Time (UTC) in order to keep its time of day close to the mean solar time.
    
    To apply the changed value, database should be restarted.

.. _plan-cache-parameters:

Query Plan Cache-Related Parameters
-----------------------------------

The following are parameters related to the query plan cache functionality. The type and value range for each parameter are as follows:

+--------------------------------------+--------+---------+---------+---------+
| Parameter Name                       | Type   | Default | Min     | Max     |
+======================================+========+=========+=========+=========+
| max_plan_cache_entries               | int    | 1,000   |         |         |
+--------------------------------------+--------+---------+---------+---------+
| max_plan_cache_clones                | int    | 1,000   |         |         |
+--------------------------------------+--------+---------+---------+---------+
| xasl_cache_time_threshold_in_minutes | int    | 360     |         |         |
+--------------------------------------+--------+---------+---------+---------+
| max_filter_pred_cache_entries        | int    | 1,000   |         |         |
+--------------------------------------+--------+---------+---------+---------+

**max_plan_cache_entries**

    **max_plan_cache_entries** is a parameter to configure the maximum number of query plans to be cached in the memory. If the **max_plan_cache_entries** parameter is configured to -1 or 0, generated query plans are not stored in the memory cache; if it is configured to an integer value equal to 0 or greater than 1, a specified number of query plans are cached in the memory.

    The following example shows how to cache up to 1,000 queries. ::

        max_plan_cache_entries=1000

**max_plan_cache_clones**
     The plan cache stores the XASL for a query in a serializable form. When executing the query, the XASL is converted back into its deserialized form, populated with values, and then executed.
     The clone cache stores deserialized XASL to reuse it instead of discarding. The **max_plan_cache_clones** is a parameter that sets the maximum number of cloned cache entries a single plan can hold, and its default value is set to 1000. When a plan is deleted, its clones are also deleted.

**xasl_cache_time_threshold_in_minutes**
    It is a parameter that determines the time threshold for deciding whether to recompile (clean-up) a cached plan. It is also used when searching for a candidate plan to remove when there is no space in the plan cache. This parameter can be set in minutes, with a default value of 360 minutes.

**max_filter_pred_cache_entries**

    **max_filter_pred_cache_entries** is a parameter used to specify the maximum number of filtered index expressions. The filtered index expressions are stored with them complied and can be immediately used in server. If it is not stored in cache, the process is required which filtered index expressions are fetched from database schema and interpreted.

.. _query-cache-parameters:

Query Cache-Related Parameters
-----------------------------------

The following are the parameters related to the query cache functionality. The type and value range for each parameter are as follows:

+-------------------------------+--------+----------+----------+----------+
| Parameter Name                | Type   | Default  | Min      | Max      |
+===============================+========+==========+==========+==========+
| max_query_cache_entries       | int    | 0        | 0        | INT_MAX  |
+-------------------------------+--------+----------+----------+----------+
| query_cache_size_in_pages     | int    | 0        | 0        | INT_MAX  |
+-------------------------------+--------+----------+----------+----------+

If one of the parameters is set to 0 or negative value, the query cache is disabled regardless using the query hint **QUERY_CACHE**.

**max_query_cache_entries**

    **max_query_cache_entries** is a parameter to configure the maximum number of query to be cached. If it is configured to an integer value equal to 0 or greater than 1, a specified number of queries are cached with the result.

    The following example shows how to cache up to 500 queries. ::

        max_query_cache_entries=500

**query_cache_size_in_pages**

    **query_cache_size_in_pages** is a parameter to configure the maximum page of result to be cached. If it is configured to an integer value equal to 0 or greater than 1, specified pages in results are cached as temp files.

    The following example shows how to cache up to 4,000 pages. ::

        query_cache_size_in_pages=4000

.. _utility-parameters:

Utility-Related Parameters
--------------------------

The following are parameters related to utilities used in CUBRID. The type and value range for each parameter are as follows:

+-------------------------------+--------+----------+----------+----------+
| Parameter Name                | Type   | Default  | Min      | Max      |
+===============================+========+==========+==========+==========+
| backup_volume_max_size_bytes  | byte   | 0        | 32K      |          |
+-------------------------------+--------+----------+----------+----------+
| communication_histogram       | bool   | no       |          |          |
+-------------------------------+--------+----------+----------+----------+
| compactdb_page_reclaim_only   | int    | 0        |          |          |
+-------------------------------+--------+----------+----------+----------+
| csql_history_num              | int    | 50       | 1        | 200      |
+-------------------------------+--------+----------+----------+----------+

**backup_volume_max_size_bytes**

    **backup_volume_max_size_bytes** is a parameter to configure the size of the backup volume file created by the **cubrid backupdb** utility in byte unit. 
    You can set a unit as B, K, M, G or T, which stands for bytes, kilobytes(KB), megabytes(MB), gigabytes(GB) or terabytes(TB) respectively. If you omit the unit, bytes will be applied. The default value is **0**, and the minimum value is 32K.    
    
    If the parameter is configured to **0**, which is the default value, the created backup volume is not partitioned; if it is configured to a value greater than 0, the backup volume is partitioned as much as it is specified size.
    
**communication_histogram**

    **communication_histogram** is a parameter associated with :ref:`csql-session-commands` "**;.h**" of the CSQL Interpreter and the default value is **no**. For details, see :ref:`Dumping CSQL execution statistics information <csql-execution-statistics>`.

**compactdb_page_reclaim_only**

    **compactdb_page_reclaim_only** is a parameter to configure the **compactdb** utility, which compacts the storage of already deleted objects to reuse OIDs of the already assigned storage. Storage optimization with the **compactdb** utility can be divided into three steps. The optimization steps can be selected through the **compactdb_page_reclaim_only** parameter. If the parameter is configured to **0**, which is the default value, step 1, 2 and 3 are all performed, so the storage is optimized in data, table and file units. If it is configured to 1, step 1 is skipped to have the storage optimized in table and file units. If it is configured to 2, steps 1 and 2 are skipped to have the storage optimized only in file units.

    *   Step 1: Optimizes the storage only in data unit.
    *   Step 2: Optimizes the storage in table unit.
    *   Step 3: Optimizes the storage in file (heap file) unit.

**csql_history_num**

    **csql_history_num** is a parameter to configure the CSQL Interpreter and the number of SQL statements to be stored in the history of the CSQL Interpreter. The default value is **50**.

.. _ha-parameters:

HA-Related Parameters
---------------------

The following are HA-related parameters. The type and value range for each parameter are as follows:

+--------------------+----------+-------------------+
| Parameter Name     | Type     | Default Value     |
+====================+==========+===================+
| ha_mode            | string   | off               |
+--------------------+----------+-------------------+

**ha_mode**

    The **ha_mode** parameter is used to set CUBRID HA, and the default value is **off**.

    *   off : CUBRID HA is not used.
    *   on : CUBRID HA is used using the configured node as a node for failover.
    *   replica : CUBRID HA is used without using the configured node as a node for failover.

    To use the CUBRID HA feature, you should set HA-related parameters in the **cubrid_ha.conf** file in addition to the **ha_mode** parameter. For details, see :doc:`/ha`.

.. _other-parameters:

Other Parameters
----------------

The following are other parameters. The type and value range for each parameter are as follows:

+-------------------------------------+--------+----------------+----------------+----------------+
| Parameter Name                      | Type   | Default        | Min            | Max            |
+=====================================+========+================+================+================+
| access_ip_control                   | bool   | no             |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| access_ip_control_file              | string |                |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| agg_hash_respect_order              | bool   | yes            |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| auto_restart_server                 | bool   | yes            |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| enable_string_compression           | bool   | yes            |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| index_scan_in_oid_order             | bool   | no             |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| index_unfill_factor                 | float  | 0.05           | 0              | 0.5            |
+-------------------------------------+--------+----------------+----------------+----------------+
| java_stored_procedure               | bool   | no             |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| java_stored_procedure_port          | int    | 0              | 0              | 65535          |
+-------------------------------------+--------+----------------+----------------+----------------+
| java_stored_procedure_uds           | bool   | yes            |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| java_stored_procedure_jvm_options   | string |                |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| multi_range_optimization_limit      | int    | 100            | 0              | 10,000         |
+-------------------------------------+--------+----------------+----------------+----------------+
| optimizer_enable_merge_join         | bool   | no             |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| use_stat_estimation                 | bool   | no             |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| server                              | string |                |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| service                             | string |                |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| session_state_timeout               | sec    | 21,600         | 60(1 minute)   | 31,536,000     |
|                                     |        | (6 hours)      |                | (1 year)       |
+-------------------------------------+--------+----------------+----------------+----------------+
| sort_limit_max_count                | int    | 1000           | 0              | INT_MAX        |
+-------------------------------------+--------+----------------+----------------+----------------+
| sql_trace_slow                      | msec   | -1(inf)        | 0              | 86,400,000     |
|                                     |        |                |                | (24 hours)     |
+-------------------------------------+--------+----------------+----------------+----------------+
| sql_trace_execution_plan            | bool   | no             |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| use_orderby_sort_limit              | bool   | yes            |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| vacuum_prefetch_log_mode            | int    | 1              | 0              | 1              |
+-------------------------------------+--------+----------------+----------------+----------------+
| vacuum_prefetch_log_buffer_size     | int    | 50M            | 25M            | INT_MAX        |
+-------------------------------------+--------+----------------+----------------+----------------+
| data_buffer_neighbor_flush_pages    | int    | 8              | 0              | 32             |
+-------------------------------------+--------+----------------+----------------+----------------+
| data_buffer_neighbor_flush_nondirty | bool   | no             |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| tde_keys_file_path                  | string | NULL           |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| tde_default_algorithm               | string | AES            |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| recovery_progress_logging_interval  | int    | 0 (off)        | 0              | 3600           |
+-------------------------------------+--------+----------------+----------------+----------------+
| supplemental_log                    | int    | 0 (off)        | 0              | 2              |
+-------------------------------------+--------+----------------+----------------+----------------+
| regexp_engine                       | string | re2            |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+
| vacuum_ovfp_check_threshold         | int    | 1000           | 2              | INT_MAX        |
+-------------------------------------+--------+----------------+----------------+----------------+
| vacuum_ovfp_check_duration          | int    | 45000          | 1              | 600000         |
+-------------------------------------+--------+----------------+----------------+----------------+
| deduplicate_key_level               | int    | -1             | -1             | 14             |
+-------------------------------------+--------+----------------+----------------+----------------+
| print_index_detail                  | bool   | no             |                |                |
+-------------------------------------+--------+----------------+----------------+----------------+

**access_ip_control**

    **access_ip_control** is a parameter to configure whether to use feature limiting the IP addresses that allow server access. The default value is **no**. For details, see :ref:`limiting-server-access`.

**access_ip_control_file**

    **access_ip_control_file** is a parameter to configure the file name in which the list of IP addresses allowed by servers is stored. If **access_ip_control** value is set to **yes**, database server allows the list of IP addresses only stored in the file specified by this parameter. For details, see :ref:`limiting-server-access`.

.. _agg_hash_respect_order:
    
**agg_hash_respect_order**

    **agg_hash_respect_order** is a parameter to configure whether the groups in an aggregate function will be returned ordered or not. The default is **yes**. As a reference, see :ref:`max_agg_hash_size <max_agg_hash_size>`.
    
    If all the groups (keys and accumulators) can fit into hash memory, then "agg_hash_respect_order=no" will skip sorting them before writing to output, so it is fair to assume that the order cannot be guaranteed in this case. However, when overflows occur, then a sort step must be performed and you will get the results in-order even with "agg_hash_respect_order=no". 

**auto_restart_server**

    **auto_restart_server** is a parameter to configure whether to restart the process when it stops due to fatal errors being occurred in database server process. If **auto_restart_server** value is set to **yes**, the server process automatically restarts when it has stopped due to errors; it does not restart in case it stops by following normal process (by using **STOP** command).
	
.. _enable_string_compression:
	
**enable_string_compression**

    **enable_string_compression** is a parameter to configure whether string compression should be used when storing variable string type value into heap, index or list. If **enable_string_compression** value is set to **yes**, and the string is at least 255 bytes in size and the compressed string requires less size than original string, then the string is stored in compressed form.

**index_scan_in_oid_order**

    **index_scan_in_oid_order** is a parameter to configure the result data to be retrieved in OID order after the index scan. If the parameter is set to **no**, which is the default value, results are retrieved in data order; if it is set to **yes**, they are retrieved in OID order.

**index_unfill_factor**

    If there is no free space because index pages are full when the **INSERT** or **UPDATE** operation is executed after the first index is created, the split of index page nodes occurs. This substantially affects the performance by increasing the operation time. **index_unfill_factor** is a parameter to configure the percent of free space defined for each index page node when an index is created. The **index_unfill_factor** value is applied only when an index is created for the first time. The percent of free space defined for the page is not maintained dynamically. Its value ranges between 0 and 0.5. The default value is **0.05**.

    If an index is created without any free space for the index page node (**index_unfill_factor** is set to 0), the split of index page nodes occurs every time an additional insertion is made. This may degrade the performance.

    If the value of **index_unfill_factor** is large, a large amount of free space is available when an index is created. Therefore, better performance can be obtained because the split of index nodes does not occur for a relatively long period of time until the free space for the nodes is filled after the first index is created.

    If this value is small, the amount of free space for the nodes is small when an index is created. Therefore, it is likely that the index nodes are spilt by **INSERT** or **UPDATE** because free space for the index nodes is filled in a short period of time.

**java_stored_procedure**

    **java_stored_procedure** is a parameter to configure whether to use Java stored procedures by running the Java Virtual Machine (JVM). If the parameter is set to **no**, which is the default value, JVM is not executed; if it is set to **yes**, JVM is executed so you can use Java stored procedures. Therefore, configure the parameter to yes if you plan to use Java stored procedures.

**java_stored_procedure_port**

    **java_stored_procedure_port** is a parameter to configure the port number receiving a request that calls the java stored procedures from database server. the value must be unique and smaller than 65,535. The default value of **java_stored_procedure_port** is **0** which means the port number is automatically allocated, typically from an ephemeral port range. The value configured in this parameter affects only **java_stored_procedure** is set to **yes**. Note that an error occurs if the parameter is configured in [common]. ::

        ..... 
        [common] 
        ..... 
        # an error occurs. remove the following line.
        java_stored_procedure_port=4333
        .....
        [@testdb]
        .....
        # the parameter is configured successfully for testdb
        java_stored_procedure_port=4334
        .....

**java_stored_procedure_uds**

    **java_stored_procedure_uds** is a parameter to connect between the cub_javasp process and the cub_server process through a Unix domain socket instead of TCP when calling a Java stored procedure. The default value of **java_stored_procedure_uds** is **yes**. For Windows, regardless of the value of the parameter, TCP connection is used.

    .. note::

        For the **CUBRID_TMP** environment variable that specifies the UNIX domain socket file path of *cub_javasp** processes, see :doc:`/env`.

**java_stored_procedure_jvm_options**

    **java_stored_procedure_jvm_options** is a parameter to configure Java Virtual Machine (JVM) and Java options on which Java stored procedures are executed. Each option string should be separated by spaces. For JVM options, there are three types of options; standard, non-standard and advanced options. non-standard and advanced options are not guaranteed to be supported on all VM implementations. The default is an empty string. If the parameter value configured in [@<database>], it overwrites the value specified in [common]. ::

        ..... 
        [common] 
        ..... 
        java_stored_procedure_jvm_options="-Xms1024m -Xmx1024m -XX:PermSize=512m -XX:MaxPermSize=512m"
        .....
        [@testdb]
        .....
        java_stored_procedure=yes

        # Note that -XX:PermSize=512m and -XX:MaxPermSize=512m will not be applied for testdb, Even though they specified in [common] section.
        java_stored_procedure_jvm_options="-Xms2048m -Xmx2048m"
        .....

**multi_range_optimization_limit**

    If the number of rows specified by the **LIMIT** clause in the query, which has multiple ranges (col IN (?, ?, ... ,?)) and is available to use an index, is within the number specified in the **multi_range_optimization_limit** parameter, the optimization for the way of index sorting will be performed. The default value is **100**.

    For example, if a value for this parameter is set to 50, LIMIT 10 means that it is within the value specified by this parameter, so that the values that meet the conditions will be sorted to produce the result. If LIMIT is 60, it means that it exceeds the parameter configuration value, so that it gets and sorts out all values that meet the conditions.

    Depending on the setting value, the differences are made between collecting the result with on-the-fly sorting of the intermediate values and sorting the result values after collecting them, and the bigger value could make more unfavorable performance.

**optimizer_enable_merge_join**

    **optimizer_enable_merge_join** is a parameter to specify whether to include sort merge join plan as a candidate of query plans or not. The default is **no**. Regarding sort merge join, see :ref:`sql-hint`.

**use_stat_estimation**

    **use_stat_estimation** is a parameter to specify whether to use the estimated information in calculating statistics or not. The default is no. The estimated information generated by the heap manager while processing DML is associated with the number of added objects. it is relatively accurate for the number of total objects, NOT for the number of distinct values.

**server**

    **server** is a parameter used to register the name of database server process which will run automatically when CUBRID server starts.

**service**

    **service** is a parameter to configure process that starts automatically when the CUBRID service starts. There are four types of processes: **server**, **broker**, **manager**, and **heartbeat**. Three processes are usually registered as in **service=server,broker,manager**.

    *   If the parameter is set to **server**, the database process specified by the **@server** parameter gets started.
    *   If the parameter is set to **broker**, the broker process gets started.
    *   If the parameter is set to **manager**, the manager process gets started.
    *   If the parameter is set to **heartbeat**, the HA-related processes get started.

**session_state_timeout**

    **session_state_timeout** is a parameter used to define how long the CUBRID session data will be kept. The session data will be deleted when the driver terminates the connection or the session time expires. The session time will expire after the specified time if a client terminates abnormally.

    Custom variables defined by **SET** and **PREPARE** statements can be deleted by **DROP** / **DEALLOCATE** statements before session timeout.

    The default value is **21,600** seconds(6 hours), and this parameter's unit is second.

**sort_limit_max_count**

    Regarding SORT-LIMIT optimization which can be applied when top-N rows are sorted by "ORDER BY ... LIMIT *N*\ " statement, this parameter specifies the LIMIT count to limit applying this optimization. When the value of *N* is smaller than a value of **sort_limit_max_count**, SORT-LIMIT optimization is applied. The default is 1000, the minimum is 0(which means that this optimization is disabled), and the maximum is INT_MAX.
    
    For more details, see :ref:`sort-limit-optimization`.

**sql_trace_slow**

    **sql_trace_slow** is a parameter to configure the execution time of a query which will be judged as a long time execution. 
    You can set a unit as ms, s, min or h, which stands for milliseconds, seconds, minutes or hours respectively. If you omit the unit, milliseconds(ms) will be applied. The default value is -1 and the maximum value is 86,400,000 msec (24 hour). -1 means that the infinite time, so any queries will not be judged as a long duration query. For details, see the below **sql_trace_execution_plan**.
    
    .. note::

        The system parameter **sql_trace_slow** judges the query execution time based on the server, but the broker parameter **MAX_QUERY_TIMEOUT** judges the query execution time based on the broker.
        
**sql_trace_execution_plan**

    **sql_trace_execution_plan** is a parameter to configure if the query plan of the long running query is written to the log or not. The default value is **no**.

    If it is set to yes, a long running SQL, a query plan and the output of cubrid statdump command  are written to the server error log file(located on $CUBRID/log/server directory) and CAS log file(located on $CUBRID/log/broker/sql_log directory) .

    If it is set to no, only a long running SQL is written to the server error log file and CAS log file.

    For example, if you want to write the execution plan of the slow query to the log file, and specify the query which executes more than 5 seconds as the slow query, then configure the value of the **sql_trace_slow** parameter as 5000(ms) and configure the value of the **sql_trace_execution_plan** parameter as yes.

    But, on the server error log file, the related information is written only when the value of error_log_level is NOTIFICATION. 

**use_orderby_sort_limit**

    **use_orderby_sort_limit** is a parameter to configure whether to keep the intermediate result of sorting and merging process in the statement including the **ORDER BY ... LIMIT** *row_count* clause as many as *row_count*. If it is set to **yes**, you can decrease unnecessary comparing and merging processes because as many as intermediate results will be kept as the value of *row_count*. The default value is **yes**.

**vacuum_prefetch_log_mode**

    **vacuum_prefetch_log_mode** is a parameter to configure the prefetch mode of log pages on behalf of vacuum.
	
	In mode 0, the vacuum master thread prefetch the required log pages in a shared buffer. In mode 1 (default), each vacuum worker prefetches the required log pages in its own buffer. Mode 0 also requires that **vacuum_prefetch_log_buffer_size** system parameter is configured, in mode 0 this parameter is ignored and each vacuum worker prefetches an entire vacuum log block (default 32 log pages).
	
**vacuum_prefetch_log_buffer_size**

    **vacuum_prefetch_log_buffer_size** is a parameter to configure the log prefetch buffer size of vacuum (it is used only if **vacuum_prefetch_log_mode** is set to 0).

**data_buffer_neighbor_flush_pages**
    
	**data_buffer_neighbor_flush_pages** is a parameter to control the number of neighbor pages to be flushed with background flush (victim candidates flushing). When is less or equal to 1, the neighbor flush feature is considered deactivated.

**data_buffer_neighbor_flush_nondirty**
    
	**data_buffer_neighbor_flush_nondirty** is a parameter to control the flushing of non-dirty neighbor pages. When victim candidates pages are flushed, and neighbor flush is activated (**data_buffer_neighbor_flush_pages** is greater than 1), than single non-dirty pages which completes a chain of neighbor (dirty) pages are also flushed.

**tde_keys_file_path**

    **tde_keys_file_path** is a parameter to configure the path of the key file for TDE. The key file's name is fixed as <database_name>_keys, and the directory where the key file exists is designated. If this system parameter is not set, the key file is searched in the same location as the database volume. For a detailed description of the key file, see :ref:`tde-file-based-key`.

**tde_default_algorithm**

    **tde_default_algorithm** is a parameter that configures the default algorithm used when creating the TDE encryption table. Log and temporary data are always encrypted using the algorithm set with this parameter when they have to be encrypted. **AES** or **ARIA** can be set. For more information on encryption algorithms, refer to :ref:`tde-algorithm`.

**recovery_progress_logging_interval**
    
    **recovery_progress_logging_interval** is a parameter to decide whether the details of recovery are printed and configure its period in seconds. If it is set bigger than 0, the total works and remained works to do of the three phases of recovery: Analysis, Redo and Undo are printed. When this is set smaller than 5, it is set to 5.

**supplemental_log**

    **supplemental_log** is a parameter to determine whether information needed to support the CDC (Change Data Capture) or :ref:`flashback` is written to the log volume. CDC and flashback must be able to see how transactions logically changed the database through the physical logs. Any additional information required to interpret the physical logs is saved as **supplemental_log** . Setting this parameter bigger than 0 affects performance and log space because more logs are created and stored in addition to the existing transaction logs. If this parameter is set to 1, the information necessary to interpret DML and DDL executed by the user is logged. If it is set to 2, only information necessary to interpret the DML is logged.

**regexp_engine**

    **regexp_engine** is a parameter to choose a library in which regular expression operators and functions will perform. **cppstd** or **re2** can be set and the default value is **re2**. For more information on regular expression functionalities, refer to :doc:`/sql/function/regex_fn`.

**vacuum_ovfp_check_threshold**

 **vacuum_ovfp_check_threshold** collects index information when the number of leaf's overflow pages to be read is greater than the value, when index vacuum is performed. The default is 1000 pages.

**vacuum_ovfp_check_duration**

**vacuum_ovfp_check_duration** specifies the duration for which data related to the count of index overflow pages, gathered by vacuum threads, is retained. Data that remains unchanged within the specified duration will be automatically removed. The unit of it's value is minutes.

**deduplicate_key_level**

 **deduplicate_key_level** determines the automatic inclusion and value setting of the WITH DEDUPLICATE statement within the index creation statement. For details on DEDUPLICATE, see :ref:`deduplicate_overview`. The default is -1(which means that the WITH DEDUPLICATE is not included implicitly).
 
.. note::

    *   If **deduplicate_key_level** is set to **-1**, even if the *deduplicate level* is explicitly specified in the CREATE INDEX statement, it is ignored.


**print_index_detail**

 It specifies whether option information in the **WITH** clause is displayed when index syntax information is displayed, such as in the SHOW CREATE TABLE statement. Default is NO. However, the unloaddb tool is not affected by this setting.


.. _broker-configuration:

Broker Configuration
====================

cubrid_broker.conf Configuration File and Default Parameters
------------------------------------------------------------

Broker System Parameters
^^^^^^^^^^^^^^^^^^^^^^^^

The following table shows the broker parameters available in the broker configuration file (**cubrid_broker.conf**). For details, see :ref:`broker-common-parameters` and :ref:`parameter-by-broker`. You can temporarily change the parameter of which configuration values can be dynamically changed by using the **broker_changer** utility. To apply configuration values even after restarting all brokers with **cubrid broker restart**, you should change the values in the **cubrid_broker.conf** file.

+---------------------------------+-------------------------+-----------------------------------------+--------+------------------------------+-----------+
| Category                        | Use                     | Parameter Name                          | Type   | Default Value                | Dynamic   |
|                                 |                         |                                         |        |                              | Changes   |
+=================================+=========================+=========================================+========+==============================+===========+
| :ref:`broker-common-parameters` | Access                  | ACCESS_CONTROL                          | bool   | no                           |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | ACCESS_CONTROL_FILE                     | string |                              |           |
|                                 +-------------------------+-----------------------------------------+--------+------------------------------+-----------+
|                                 | Logging                 | ADMIN_LOG_FILE                          | string | log/broker/cubrid_broker.log |           |
|                                 +-------------------------+-----------------------------------------+--------+------------------------------+-----------+
|                                 | Broker server           | MASTER_SHM_ID                           | int    | 30,001                       |           |
|                                 | (cub_broker)            |                                         |        |                              |           |
+---------------------------------+-------------------------+-----------------------------------------+--------+------------------------------+-----------+
| :ref:`parameter-by-broker`      | Access                  | ACCESS_LIST                             | string |                              |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | ACCESS_MODE                             | string | RW                           | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | BROKER_PORT                             | int    | 30,000(max : 65,535)         |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | CONNECT_ORDER                           | string | SEQ                          | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | ENABLE_MONITOR_HANG                     | string | OFF                          |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | KEEP_CONNECTION                         | string | AUTO                         | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | MAX_NUM_DELAYED_HOSTS_LOOKUP            | int    | -1                           |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | PREFERRED_HOSTS                         | string |                              | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | RECONNECT_TIME                          | sec    | 600                          | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | REPLICA_ONLY                            | string | OFF                          |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER | bool   | DENY                         |           |
|                                 +-------------------------+-----------------------------------------+--------+------------------------------+-----------+
|                                 | Broker App. Server(CAS) | APPL_SERVER_MAX_SIZE                    | MB     | Windows 32bit: 40,           | available |
|                                 |                         |                                         |        | Windows 64bit: 80,           |           |
|                                 |                         |                                         |        | Linux: 0(max: 2,097,151)     |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | APPL_SERVER_MAX_SIZE_HARD_LIMIT         | MB     | 1,024                        | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | APPL_SERVER_PORT                        | int    | BROKER_PORT+1                |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | APPL_SERVER_SHM_ID                      | int    | 30,000                       |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | AUTO_ADD_APPL_SERVER                    | string | ON                           |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | MAX_NUM_APPL_SERVER                     | int    | 40                           |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | MIN_NUM_APPL_SERVER                     | int    | 5                            |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | TIME_TO_KILL                            | sec    | 120                          | available |
|                                 +-------------------------+-----------------------------------------+--------+------------------------------+-----------+
|                                 | Transaction & Query     | CCI_DEFAULT_AUTOCOMMIT                  | string | ON                           |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | LONG_QUERY_TIME                         | sec    | 60(max: 86,400)              | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | LONG_TRANSACTION_TIME                   | sec    | 60(max: 86,400)              | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | MAX_PREPARED_STMT_COUNT                 | int    | 2,000(min: 1)                | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | MAX_QUERY_TIMEOUT                       | sec    | 0(max: 86,400)               | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SESSION_TIMEOUT                         | sec    | 300                          | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | STATEMENT_POOLING                       | string | ON                           | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | JDBC_CACHE                              | string | OFF                          | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | JDBC_CACHE_HINT_ONLY                    | string | OFF                          | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | JDBC_CACHE_LIFE_TIME                    | sec    | 1000                         | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | TRIGGER_ACTION                          | string | ON                           | available |
|                                 +-------------------------+-----------------------------------------+--------+------------------------------+-----------+
|                                 | Logging                 | ACCESS_LOG                              | string | OFF                          | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | ACCESS_LOG_DIR                          | string | log/broker                   |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | ACCESS_LOG_MAX_SIZE                     | KB     | 10M(max: 2G)                 | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | ERROR_LOG_DIR                           | string | log/broker/error_log         | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | LOG_DIR                                 | string | log/broker/sql_log           | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SLOW_LOG                                | string | ON                           | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SLOW_LOG_DIR                            | string | log/broker/sql_log           | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SQL_LOG                                 | string | ON                           | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SQL_LOG_MAX_SIZE                        | KB     | 10,000                       | available |
|                                 +-------------------------+-----------------------------------------+--------+------------------------------+-----------+
|                                 | Shard                   | SHARD                                   | string | OFF                          |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_CONNECTION_FILE                   | string | shard_connection.txt         |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_DB_NAME                           | string |                              |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_DB_PASSWORD                       | string |                              |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_DB_USER                           | string |                              |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_IGNORE_HINT                       | string | OFF                          |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_KEY_FILE                          | string | shard_key.txt                |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_KEY_FUNCTION_NAME                 | string |                              |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_KEY_LIBRARY_NAME                  | string |                              |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_KEY_MODULAR                       | int    | 256                          |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_MAX_CLIENTS                       | int    | 256                          |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_MAX_PREPARED_STMT_COUNT           | int    | 10,000                       |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_NUM_PROXY                         | int    | 1                            |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_PROXY_CONN_WAIT_TIMEOUT           | sec    | 8h                           |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_PROXY_LOG                         | string | ERROR                        | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_PROXY_LOG_DIR                     | string | log/broker/proxy_log         |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_PROXY_LOG_MAX_SIZE                | KB     | 100,000                      | available |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_PROXY_SHM_ID                      | int    |                              |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SHARD_PROXY_TIMEOUT                     | sec    | 30(second)                   |           |
|                                 +-------------------------+-----------------------------------------+--------+------------------------------+-----------+
|                                 | Etc                     | MAX_STRING_LENGTH                       | int    | -1                           |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SERVICE                                 | string | ON                           |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SSL                                     | string | OFF                          |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | SOURCE_ENV                              | string | cubrid.env                   |           |
|                                 |                         +-----------------------------------------+--------+------------------------------+-----------+
|                                 |                         | NET_BUF_SIZE                            | KB     | 16K                          |           |
+---------------------------------+-------------------------+-----------------------------------------+--------+------------------------------+-----------+

Default Parameters
^^^^^^^^^^^^^^^^^^

The **cubrid_broker.conf** file, the default broker configuration file created when installing CUBRID, includes some parameters that must be modified by default. If you want to modify the values of parameters that are not included in the configuration file by default, you can add or modify one yourself.

The following is the content of the **cubrid_broker.conf** file provided by default. ::

    [broker]
    MASTER_SHM_ID           =30001
    ADMIN_LOG_FILE          =log/broker/cubrid_broker.log
     
    [%query_editor]
    SERVICE                 =ON
    BROKER_PORT             =30000
    MIN_NUM_APPL_SERVER     =5
    MAX_NUM_APPL_SERVER     =40
    APPL_SERVER_SHM_ID      =30000
    LOG_DIR                 =log/broker/sql_log
    ERROR_LOG_DIR           =log/broker/error_log
    SQL_LOG                 =ON
    TIME_TO_KILL            =120
    SESSION_TIMEOUT         =300
    KEEP_CONNECTION         =AUTO
     
    [%BROKER1]
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

Broker Configuration File Related Environment Variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can specify the location of broker configuration file (**cubrid_broker.conf**) file by using the **CUBRID_BROKER_CONF_FILE** variable. The variable is used when executing several brokers with different configuration.

.. _broker-common-parameters:

Common Parameters
-----------------

The following are parameters commonly applied to entire brokers; it is written under [broker] section.

Access
^^^^^^

**ACCESS_CONTROL**

    **ACCESS_CONTROL** is a parameter used to limit applications which are trying to connect a broker. The default value is **OFF**. For details, see :ref:`limiting-broker-access`.

**ACCESS_CONTROL_FILE**

    **ACCESS_CONTROL_FILE** is a parameter to configure the name of a file in which a database name, database user ID, and the list of IPs are stored. List of IPs can be written up to the maximum of 256 lines per <*db_name*>:<*db_user*> in a broker. For details, see :ref:`limiting-broker-access`.

Logging
^^^^^^^

**ADMIN_LOG_FILE**

    **ADMIN_LOG_FILE** is a parameter to configure the file in which time of running CUBRID broker is stored. The default value is a **log/broker/cubrid_broker.log** file.

Broker Server(cub_broker)
^^^^^^^^^^^^^^^^^^^^^^^^^

**MASTER_SHM_ID**

    **MASTER_SHM_ID** is a parameter used to specify the identifier of shared memory which is used to manage the CUBRID broker. Its value must be unique in the system. The default value is **30001**.

.. _parameter-by-broker:

Parameter by Broker
-------------------

The following describes parameters to configure the environment variables of brokers; each parameter is located under *[%broker_name]*. The maximum length of *broker_name* is 63 characters in English.

Access
^^^^^^

**ACCESS_LIST**

    **ACCESS_LIST** is a parameter to configure the name of a file where the list of IP addresses of an application which allows access to the CUBRID broker is stored. To allow access by IP addresses access 210.192.33.* and 210.194.34.*, store them to a file (ip_lists.txt) and then assign the file name with the value of this parameter.

**ACCESS_MODE**

    **ACCESS_MODE** is a parameter to configure default mode of the broker. The default value is **RW**. For details, see :ref:`ha-cubrid-broker-conf`.

**BROKER_PORT**

    **BROKER_PORT** is a parameter to configure the port number of the broker; the value must be unique and smaller than 65,535. The default port value of **query_editor**' broker is **30,000** and the port value of the **broker1** is **33,000**.

**CONNECT_ORDER**

    **CONNECT_ORDER** is a parameter to specify whether a CAS tries to connect to one of hosts in the order or randomly in **$CUBRID_DATABASES/databases.txt**, when a CAS decides the order of connecting to a host.
    The default is **SEQ**; a CAS tries to connect to a host in the order. if this value is **RANDOM**, a CAS tries to connect to a host randomly. If **PREFERRED_HOSTS** parameter is specified, firstly a CAS tries to connect to one of hosts specified in **PREFERRED_HOSTS**, then uses db-host values in **$CUBRID_DATABASES/databases.txt** only when the connection is failed.
    
**ENABLE_MONITOR_HANG**

    **ENABLE_MONITOR_HANG** is a parameter to configure whether to block the access from the application to the broker or not, when more than a certain ratio of CASes on that broker are hung. If the **ENABLE_MONITOR_HANG** parameter value is **ON**, blocking feature is processed. The default value is **OFF**. If it is **OFF**, don't do the behavior.
    
    The broker process judges the CAS as hung if the hanging status of the CAS keeps more than one minute, then block the access from applications to that broker; it brings the behavior which the applications try to access to the alternative hosts(altHosts) configured by the connection URL.

**KEEP_CONNECTION**

    **KEEP_CONNECTION** is a parameter to configure the way of connection between CAS and application clients; it is set to one of the following: **ON** or **AUTO**. If this value is **ON**, it is connected in connection unit. If it is **AUTO** and the number of servers is more than that of clients, transaction unit is used; in the reverse case, connection unit is used. The default value is **AUTO**.

**MAX_NUM_DELAYED_HOSTS_LOOKUP**

    When almost all DB servers have the delay of replication in the HA environment where multiple DB servers on db-host of databases.txt are specified, check if the connection is established or not until the number of delayed replication servers; the number is specified in **MAX_NUM_DELAYED_HOSTS_LOOKUP** (whether the delay of replication in the DB server is judged only with the standby hosts; it is determined by the setting of ref:`ha_delay_limit <ha_delay_limit>`). See :ref:`MAX_NUM_DELAYED_HOSTS_LOOKUP <MAX_NUM_DELAYED_HOSTS_LOOKUP>` for further information.

**PREFERRED_HOSTS**

    **PREFERRED_HOSTS** is a parameter to specify the order of a host to which a CAS tries to connect in a first priority. If the connection is failed after trying connection in the order specified in **PREFERRED_HOSTS**, a CAS tries to connect to the one of hosts specified in **$CUBRID_DATABASES/databases.txt**. The default value is **NULL**. For details, see :ref:`ha-cubrid-broker-conf`.

.. _reconnect_time:
    
**RECONNECT_TIME** 
  
    If the time specified by **RECONNECT_TIME** is elapsed in a certain status, CAS will try to reconnect to the other DB server. The default of this parameter is **600s(10min)**. You can set a unit as ms, s, min or h, which stands for milliseconds, seconds, minutes or hours respectively. If you omit the unit, s will be applied.
    
    a certain status which CAS tries to reconnect is as follows.
     
    *   when CAS is connected to not a DB server in **PREFERRED_HOSTS**, but the DB server of db-host in databases.txt.
    *   when CAS with "ACCESS_MODE=RO"(Read Only) is connected to not the standby DB server, but the active DB server.
    *   when CAS is connected to the DB server of which replication is delayed.
    
    When **RECONNECT_TIME** is 0, CAS does not try to reconnect.
    
.. _replica_only: 

**REPLICA_ONLY**
  
    If a value of **REPLICA_ONLY** is **ON**, CAS is connected only to replicas. The default is **OFF**. Even though the value of **REPLICA_ONLY** is **ON**, when a value of **ACCESS_MODE** is  **RW**, it is possible to write directly to the replica DB. However, the data to be written directly to the replica DB are not replicated.
    
    .. note::
    
        Please note that replication mismatch occurs when you write the data directly to the replica DB.

.. _access_control_behavior_for_emptybroker:

**ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER**	
	
    If no broker is specified in **ACCESS_CONTROL_FILE** and the value of **ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER** is **ALLOW** , all access to the broker are allowed. The default is **DENY**. For more information, see :ref:`limiting-broker-access`.

    .. note::
    
       The settings value ALLOW or DENY for **ACCESS_CONTROL_BEHAVIOR_FOR_EMPTYBROKER** is valid only when **ACCESS_CONTROL** is set to **ON**. If it is set to **OFF**, the setting value is not applicable.
	
Broker App. Server(CAS)
^^^^^^^^^^^^^^^^^^^^^^^

**APPL_SERVER_MAX_SIZE**

    **APPL_SERVER_MAX_SIZE** is a parameter to configure the maximum size of the process memory usage handled by CAS. You can set a unit as B, K, M or G, which stands for bytes, kilobytes(KB), megabytes(MB) or gigabytes(GB) respectively. If you omit the unit, M will be applied.    

    Specifying this parameter makes transactions terminate (commit or rollback) only when it is executed by a user. In contrast to this, specifying **APPL_SERVER_MAX_SIZE_HARD_LIMIT** makes transactions forcibly terminate (rollback) and restart CAS.

    Note that the default values of Windows and Linux are different from each other.

    For 32-bit Windows, the default value is **40** MB; for 64-bit Windows, it is **80** MB. The maximum value is the same on Windows and Linux as 2,097,151 MB. When current process size exceeds the value of **APPL_SERVER_MAX_SIZE**, broker restarts the corresponding CAS.

    For Linux, the default value of **APPL_SERVER_MAX_SIZE** is **0**; CAS restarts in the following conditions.

    *   **APPL_SERVER_MAX_SIZE** is zero or negative: At the point when current process size becomes twice as large as initial memory
    *   **APPL_SERVER_MAX_SIZE** is positive: At the point when it exceeds the value specified in **APPL_SERVER_MAX_SIZE**

    .. note::

        Be careful not to make the value too small because application servers may restart frequently and unexpectedly. In general, the value of **APPL_SERVER_MAX_SIZE_HARD_LIMIT** is greater than that of **APPL_SERVER_MAX_SIZE**. For details, see description of **APPL_SERVER_MAX_SIZE_HARD_LIMIT**.

**APPL_SERVER_MAX_SIZE_HARD_LIMIT**

    **APPL_SERVER_MAX_SIZE_HARD_LIMIT** is a parameter to configure the maximum size of process memory usage handled by CAS. You can set a unit as B, K, M or G, which stands for bytes, kilobytes(KB), megabytes(MB) or gigabytes(GB) respectively. If you omit the unit, M will be applied. The default value is **1,024** (MB), and the maximum value is 2,097,151 (MB).

    Specifying this parameter makes transactions being processed forcibly terminate (rollback) and restart CAS. In contrast to this, specifying **APPL_SERVER_MAX_SIZE** makes transactions terminate only when it is executed by a user. 

    .. note::

        Be careful not to make the value too small because application servers may restart frequently and unexpectedly. When restarting CAS, **APPL_SERVER_MAX_SIZE** is specified to wait for normal termination of transactions although memory usage increases; **APPL_SERVER_MAX_SIZE_HARD_LIMIT** is specified to forcibly terminate transactions if memory usage exceeds the maximum value allowed. Therefore, in general, the value of **APPL_SERVER_MAX_SIZE_HARD_LIMIT** is greater than that of **APPL_SERVER_MAX_SIZE**.

**APPL_SERVER_PORT**

    **APPL_SERVER_PORT** is a parameter to designate the TCP ports of CAS that communicates with application clients (**Windows only**).

    On Linux, an established TCP connection between the BROKER and the client will be passed to the CAS. Therefore, the client can communicate with the CAS without additional TCP connection.

    On the other hand, in Windows, all available CASes are waiting for a connection on an independent TCP port, for example, 33001/tcp, 33002/tcp. In these circumstances, when a client connects to a BROKER, the BROKER delivers the TCP port number, for example, 33001/tcp, for connecting to an available CAS to the client. In sequence, the client terminates the current network connection with the BROKER and establishes a new connection with the CAS using the port number received from the BROKER.

    If the **APPL_SERVER_PORT** parameter is not additionally specified, this value is the value obtained by adding 1 to the value of **BROKER_PORT**. For example, if the value of **BROKER_PORT** is 30,000 and the **APPL_SERVER_PORT** parameter has not been specified, and if the **MIN_NUM_APPL_SERVER** value is 5, then five CASes use the TCP ports between 30,001 and 30,005, respectively. On the other hand, if the value of **APPL_SERVER_PORT** is 35,000 under the same conditions, 5 CASes use TCP ports from 35,000 to 35,004. The maximum number of CAS specified in the **MAX_NUM_APPL_SERVER** parameter in **cubrid_broker_conf**; therefore, the maximum number of connection ports is also determined by the value of **MAX_NUM_APPL_SERVER** parameter.

    On the Windows system, if a firewall system exists between an application and a CUBRID broker, all TCP ports specified in **BROKER_PORT** and **APPL_SERVER_PORT** must be opened.

    .. note::

        For the **CUBRID_TMP** environment variable that specifies the UNIX domain socket file path of **cub_master** and **cub_broker** processes, see :doc:`/env`.

**APPL_SERVER_SHM_ID**

    **APPL_SERVER_SHM_ID** is a parameter to configure the ID of shared memory used by CAS; the value must be unique within system. The default value is the same as the port value of the broker.

**AUTO_ADD_APPL_SERVER**

    **AUTO_ADD_APPL_SERVER** is a parameter to configure whether CAS increase automatically to the value specified in **MAX_NUM_APPL_SERVER** in case of needed; the value will be either **ON** or **OFF** (default: **ON**).  

.. _max-num-appl-server:

**MAX_NUM_APPL_SERVER**

    **MAX_NUM_APPL_SERVER** is a parameter to configure the maximum number of simultaneous connections allowed. The default value is **40**.

**MIN_NUM_APPL_SERVER**

    **MIN_NUM_APPL_SERVER** is a parameter to configure the minimum number of CAS even if any request to connect the broker has not been made. The default value is **5**.

**TIME_TO_KILL**

    **TIME_TO_KILL** is a parameter to configure the time to remove CAS in idle state among CAS added dynamically. You can set a unit as ms, s, min or h, which stands for milliseconds, seconds, minutes or hours respectively. If you omit the unit, s will be applied. The default value is **120** (seconds). 
    
    An idle state is one in which the server is not involved in any jobs. If this state continues exceeding the value specified in **TIME_TO_KILL**, CAS is removed.

    The value configured in this parameter affects only CAS added dynamically, so it applies only when the **AUTO_ADD_APPL_SERVER** parameter is configured to **ON**. Note that times to add or remove CAS will be increased more if the **TIME_TO_KILL** value is so small.

Transaction & Query
^^^^^^^^^^^^^^^^^^^

.. _cci_default_autocommit:

**CCI_DEFAULT_AUTOCOMMIT**

    **CCI_DEFAULT_AUTOCOMMIT** is a parameter to configure whether to make application implemented in CCI interface or CCI-based interface such as PHP, OLE DB, Perl, Python, and Ruby commit automatically. The default value is **ON**. This parameter does not affect applications implemented in JDBC.

    If the **CCI_DEFAULT_AUTOCOMMIT** parameter value is **OFF**, the broker application server (CAS) process is occupied until the transaction is terminated. Therefore, it is recommended to execute commit after completing fetch when executing the **SELECT** statement.

    .. note::

        The **CCI_DEFAULT_AUTOCOMMIT** parameter has been supported from 2008 R4.0, and the default value is **OFF** for the version. Therefore, if you use CUBRID 2008 R4.1 or later versions and want to keep the configuration **OFF**, you should manually change it to **OFF** to avoid auto-commit of unexpected transaction.

    .. warning::

        In ODBC driver, the setting of **CCI_DEFAULT_AUTOCOMMIT** is ignored and worked as **ON**; therefore, you should set the autocommit on or off in the program directly.

**LONG_QUERY_TIME**

    **LONG_QUERY_TIME** is a parameter to configure execution time of query which is evaluated as long-duration query. You can set a unit as ms, s, min or h, which stands for milliseconds, seconds, minutes or hours respectively. If you omit the unit, milliseconds(ms) will be applied. The default value is **60** (seconds) and the maximum value is 86,400(1 day). When you run a query and this query's running time takes more than the specified time, a value of LONG-Q, which is printed out from "cubrid broker status" command, is increased 1; this SQL is written to SLOW log file ($CUBRID/log/broker/sql_log/\*.slow.log) of CAS. See :ref:`SLOW_LOG <slow-log>`.

    This value can be valued in milliseconds with a decimal separator. For example, the value can be configured into 0.5 to configure 500 msec. 
    
    Note that if a parameter value is configured to **0**, a long-duration query is not evaluated.

**LONG_TRANSACTION_TIME**

    **LONG_TRANSACTION_TIME** is a parameter to configure execution time of query which is evaluated as long-duration transaction. The default value is **60** (seconds) and the maximum value is 86,400(1 day). 
    
    This value can be valued in milliseconds with a decimal separator. For example, the value can be configured into 0.5 to configure 500 msec. 
    
    Note that if a parameter value is configured to **0**, a long-duration transaction is not evaluated.

.. _max-prepared-stmt-count:

**MAX_PREPARED_STMT_COUNT**

    **MAX_PREPARED_STMT_COUNT** is a parameter used to limit the number of prepared statements by user (application) access. The default value is **2,000** and the minimum value is 1. The problem in which prepared statement exceeding allowed memory is mistakenly generated by system can be prohibited by making users specify the parameter value.
    
    .. note:: When you want to change the value of **MAX_PREPARED_STMT_COUNT** dynamically by **broker_changer** command, this can be changed only when this is bigger than the existing value; this cannot be changed when this is smaller than the existing value.

**MAX_QUERY_TIMEOUT**

    **MAX_ QUERY_TIMEOUT** is a parameter to configure timeout value of query execution. When time exceeds a value specified in this parameter after starting query execution, the query being executed stops and rolls back.
    You can set a unit as ms, s, min or h, which stands for milliseconds, seconds, minutes or hours respectively. If you omit the unit, milliseconds(ms) will be applied. The default value is **0** (seconds) and it means infinite waiting. The value range is available from 0 to 86,400 seconds(one day). 
    
    The smallest value (except 0) between the **MAX_QUERY_TIMEOUT** value and query timeout value of an application is applied if query timeout is configured in an application.

    .. note::

        See the :c:func:`cci_connect_with_url` and :c:func:`cci_set_query_timeout` functions to configure query timeout of CCI applications. For configuring query timeout of JDBC applications, see the **setQueryTimeout** method.

**SESSION_TIMEOUT**

    **SESSION_TIMEOUT** is a parameter to configure timeout value for the session of the broker.
    You can set a unit as ms, s, min or h, which stands for milliseconds, seconds, minutes or hours respectively. If you omit the unit, milliseconds(ms) will be applied. The default value is **300** (seconds).

    If there is no response to the job request for the specified time period, session will be terminated. If a value exceeds the value specified in this parameter without any action taken after starting transaction, the connections are terminated.

**STATEMENT_POOLING**

    **STATEMENT_POOLING** is a parameter to configure whether to use statement pool feature. The default value is **ON**.

    CUBRID closes all handles of prepared statement in the corresponding client sessions when transaction commit or rollback is made. If the value of **STATEMENT_POOLING** is set to **ON**, the handles are reusable because they are maintained in the pool. Therefore, in an environment where libraries, such as general applications reusing prepared statement or DBCP where statement pooling is implemented is applied, the default configuration (**ON**) should be maintained.

    If the prepared statement is executed after transaction commit or termination while **STATEMENT_POOLING** is set to **OFF**, the following message will be displayed. ::

        Caused by: cubrid.jdbc.driver.CUBRIDException: Attempt to access a closed Statement.

**JDBC_CACHE**

    **JDBC_CACHE** is a parameter to configure whether to use result-cache fetaure. The default value is **OFF**.

	If the parameter is **ON**, all of SELECT queries from JDBC is cached at client for life time which is configured by **JDBC_CACHE_LIFE_TIME**

**JDBC_CACHE_HINT_ONLY**

    **JDBC_CACHE_HINT_ONLY** is a parameter to configure whether to use result-cache feature only by query hint /\*+ JDBC_CACHE \*/.

	It works as if the parameter is **ON** when the query hint is given.

**JDBC_CACHE_LIFE_TIME**

    **JDBC_CACHE_HINT_ONLY** is a parameter to configure JDBC client's result-cache life time. The default value is 1000 (sec).

	For only cache life time, the result-cache is available. After the cache lifetime expired, the prior cached results are no more available and a new result is cached.

	The cache life time works only when the paramter **JDBC_CACHE** or **JDBC_CACHE_HINT_ONLY** is configured to "ON".

    **WARNING**

    **JDBC_CACHE**, **JDBC_CACHE_HINT_ONLY**, and **JDBC_CACHE_LIFE_TIME** parameters are meaningless

	when the system parameter both of **max_query_cache_entries** and **query_cache_size_in_pages** are not set to positive value.

	For result cache working, the SELECT query must include query hint /\*+ QUERY_CACHE \*/ together with these JDBC related paramter setting.

.. _trigger_action:

**TRIGGER_ACTION**

    Turn on or off of the trigger's action about the broker which specified this parameter. Specify **ON** or **OFF** as a value; The default is **ON**. 

Logging
^^^^^^^

**ACCESS_LOG**

    **ACCESS_LOG** is a parameter to configure whether to store the access log of the broker. The default value is **OFF**. The name of the access log file for the broker is *broker_name*\ **.access**  and the file is stored under **$CUBRID/log/broker** directory.

**ACCESS_LOG_DIR** 
     
    **ACCESS_LOG_DIR** specifies the directory for broker access logging files(**ACCESS_LOG**) to be created. The default is **log/broker**. 

**ACCESS_LOG_MAX_SIZE**

    **ACCESS_LOG_MAX_SIZE** specifies the maximum size of broker access logging files(**ACCESS_LOG**); if a broker access logging file is bigger than a specified size, this file is backed up into  the name of *broker_name*\ **.access.**\ *YYYYMMDDHHMISS*, then logging messages are written to the new file(`broker_name`.\ **access**). The default is 10M and the maximum is 2G. It can be dynamically changed during operating a broker.

**ERROR_LOG_DIR**

    **ERROR_LOG_DIR** is a parameter to configure default directory in which error logs about broker is stored. The default value is **log/broker/error_log**. The log file name for the broker error is *broker_ name_id.err*.

**LOG_DIR**

    **LOG_DIR** is a parameter to configure the directory where SQL logs are stored. The default value is **log/broker/sql_log**. The file name of the SQL logs is *broker_name_id.sql.log*.

.. _slow-log:

**SLOW_LOG**

    **SLOW_LOG** is a parameter to configure whether to log. The default value is **ON**. If the value is **ON**, long transaction query which exceeds the time specified in **LONG_QUERY_TIME** or query where an error occurred is stored in the **SLOW SQL** log file. The name of file created is *broker_name_id.slow.log* and it is located under **SLOW_LOG_DIR**.

**SLOW_LOG_DIR**

    **SLOW_LOG_DIR** is a parameter to configure the location of directory where the log file is generated. The default value is **log/broker/sql_log**.

.. _sql-log:

**SQL_LOG**

    **SQL_LOG** is a parameter to configure whether to leave logs for SQL statements processed by CAS when CAS handles requests from a client. The default value is **ON**. When this parameter is configured to **ON**, all logs are stored. The log file name becomes *broker_name_id.sql.log*. The file is created in the **log/broker/sql_log** directory under the installation directory. The parameter values are as follows:

    *   **OFF** : Does not leave any logs.
    *   **ERROR** : Stores logs for queries which occur an error. only queries where an error occurs.
    *   **NOTICE** : Stores logs for the long-duration execution queries which exceeds the configured time/transaction, or leaves logs for queries which occur an error.
    *   **TIMEOUT** : Stores logs for the long-duration execution queries which exceeds the configured time/transaction.
    *   **ON** / **ALL** : Stores all logs.

**SQL_LOG_MAX_SIZE**

    **SQL_LOG_MAX_SIZE** is a parameter to configure the maximum size of the SQL log file. 
    You can set a unit as B, K, M or G, which stands for bytes, kilobytes(KB) or megabytes(MB) or gigabytes(GB) respectively. If you omit the unit, M will be applied. The default value is **10,000*** (KB).
    
    *   If the size of the SQL log file, which is created when the **SQL_LOG** parameter is configured to **ON**, reaches to the size configured by the parameter, *broker_name_id.sql.log.bak* is created.
    *   If the size of the SLOW SQL log file, which is created when the **SLOW_LOG** parameter is configured to **ON**, reaches to the size configured by the parameter, *broker_name_id.slow.log.bak* is created.

SHARD
^^^^^

To use SHARD feature, configure the below parameters in **cubrid_broker.conf** as referring to **cubrid_broker.conf.shard**.

**SHARD**

    It specifies to activate/deactivate SHARD feature. You can set this value as **ON** or **OFF**. The default is **OFF**.

**SHARD_CONNECTION_FILE**

    The path of the shard connection file. The shard connection file should be located in **$CUBRID/conf**. For more information, see the :ref:`shard connection file <shard-connection-file>`.

**SHARD_DB_NAME**

    The name of the shard DB, used to verify the request for connection from an application.
    
**SHARD_DB_PASSWORD**

    The user password of the backend shard DB, used to connect to the backend DBMS for the CAS process as well as to verify the request for connection from an application. Passwords of all shard DBs should be identical.
    
    The environment variable can be used when you don't want to expose **SHARD_DB_PASSWORD** to cubrid_broker.conf. The format of this environment variable name is <*broker_name*>\ **_SHARD_DB_PASSWORD**, and <*broker_name*> always should be changed as upper cases. For example, if the name of broker is *shard1*, the name of an environment variable which configures the shard DB password will be **SHARD1_SHARD_DB_PASSWORD**. But, if the SHARD feature is restarted by "cubrid broker restart" command, the environment variable of **SHARD_DB_PASSWORD** or the value of **SHARD_DB_PASSWORD** in cubrid_broker.conf must be configured.
    
    ::

        export SHARD1_SHARD_DB_PASSWORD=shard123

    .. note:: SHARD_DB_USER/SHARD_DB_PASSWORD parameters is deprecated. Therefore, it is recommended to deliver the connection information in an application.
    
**SHARD_DB_USER**

    The name of the backend shard DB user, used to connect to the backend DBMS for the CAS process as well as to verify the request for connection from an application. User names on all shard DBs should be identical.

    .. note:: SHARD_DB_USER/SHARD_DB_PASSWORD parameters is deprecated. Therefore, it is recommended to deliver the connection information in an application.

**SHARD_IGNORE_HINT**

    When this value is **ON**, the hint provided to connect to a specific shard is ignored and the database to connect is selected based on the defined rule. The default value is **OFF**. It can be used to balance the read load while all databases are copied with the same data. For example, to give the load of an application to only one node among several replication nodes, the proxy automatically determines the node (database) with one connection to a specific shard.
    
**SHARD_KEY_FILE**

    The path of the shard key configuration file. The shard key configuration file should be located in **$CUBRID/conf**. For more information, see the :ref:`shard key configuration file <shard-key-configuration-file>`.
    
**SHARD_KEY_FUNCTION_NAME**

    The parameter to specify the name of the user hash function for shard key. For more information, see :ref:`setting-user-defined-hash-function`.

**SHARD_KEY_LIBRARY_NAME**

    Specify the library path loadable at runtime to specify the user hash function for the shard key. If the **SHARD_KEY_LIBRARY_NAME** parameter is set, the **SHARD_KEY_FUNCTION_NAME** parameter should also be set. For more information, see :ref:`setting-user-defined-hash-function`.

**SHARD_KEY_MODULAR**

    The parameter to specify the range of results of the default shard key hash function. The result of the function is shard_key(integer) % SHARD_KEY_MODULAR.  The minimum value is 1, and the maximum value is 256. For related issues, see :ref:`shard key configuration file <shard-key-configuration-file>` and :ref:`setting-user-defined-hash-function`.

**SHARD_MAX_CLIENTS**

    The number of applications that can be concurrently connected by using the proxy. The default value is 256 and the maximum value is 10,000 per proxy.
    
.. _shard-max-prepared-stmt-count:

**SHARD_MAX_PREPARED_STMT_COUNT**

    The maximum size of statement pool managed by proxy. The default is 10,000.
    
**SHARD_NUM_PROXY**

    The number of proxy processes.

**SHARD_PROXY_CONN_WAIT_TIMEOUT**

    If there is no request anymore during the time specified in this parameter, CAS disconnect with DB. The default is **8h**. You can set a unit as ms, s, min or h, which stands for milliseconds, seconds, minutes or hours respectively. If you omit the unit, second(s) will be applied.
    CAS which has a previous password should be exit because it cannot be used anymore; this feature protects that CAS is still kept unnecessarily.

**SHARD_PROXY_LOG**

    The proxy log level. It can be set to one of the following values:

    Proxy log level policy: Setting the higher level leaves all the lower logs.

    * Example) When SCHEDULE is set, ERROR | TIMEOUT | NOTICE | SHARD | SCHEDULE log is left.

    *   **ALL** : All logs
    *   **ON** : All logs
    *   **SHARD** : Logs for selecting and processing shard DBs.
    *   **SCHEDULE** : Logs for scheduling tasks.
    *   **NOTICE** : Logs for key notices.
    *   **TIMEOUT** : Logs for timeouts.
    *   **ERROR** : Logs for errors.
    *   **NONE** : No logs recorded.
    *   **OFF** : No logs recorded.

**SHARD_PROXY_LOG_DIR**

    The directory path where the proxy logs will be saved.

**SHARD_PROXY_LOG_MAX_SIZE**

    The maximum size of the proxy log file. You can set a unit as B, K, M or G, which stands for bytes, kilobytes(KB) or megabytes(MB) or gigabytes(GB) respectively. If you omit the unit, K will be applied. The maximum value is 1,000,000(KB).

**SHARD_PROXY_SHM_ID**

    A parameter to configure the ID of shared memory used by proxy
    
**SHARD_PROXY_TIMEOUT**

    The maximum waiting time by which the statement is prepared or CAS is available to use. The default value is 30(seconds). If this value is 0, the waiting time is decided by the value of the query_timeout system parameter; if the value of query_timeout is also 0, the waiting time is infinite. IF the value SHARD_PROXY_TIMEOUT is larger than 0, the maximum value between query_timeout and SHARD_PROXY_TIMEOUT decides the waiting time. You can set a unit as ms, s, min or h, which stands for milliseconds, seconds, minutes or hours respectively. If you omit the unit, s will be applied.

.. note:: **Required parameters for configuring proxy**

    To configure CUBRID proxy, you should specify SHARD_MAX_CLIENTS, MAX_NUM_APPL_SERVER and SHARD_NUM_PROXY.
     
    *   In Linux, the number of file descriptors(fd) per proxy process is limited as follows.

        *   "((SHARD_MAX_CLIENTS + MAX_NUM_APPL_SERVER) / SHARD_NUM_PROXY) + 256" <= 10,000
     
    The following are detail descriptions on above formulas.
     
    *   SHARD_MAX_CLIENTS is the maximum number of applications which access the SHARD system.
    *   MAX_NUM_APPL_SERVER is the maximum number of all CASes which can access proxy system.
    *   SHARD_NUM_PROXY is the maximum number of proxy processes which can use on the SHARD system.
    *   "SHARD_MAX_CLIENTS / SHARD_NUM_PROXY" is the maximum number of applications which can access per proxy process.
    *   "MAX_NUM_APPL_SERVER / SHARD_NUM_PROXY" is the maximum number of CASes which can access per proxy process.
    *   256 is the number of file descriptors which are used internally per process on Linux.

    As an example of configuring SHARD parameters in Linux system, if you specify the maximum concurrent access number of applications (SHARD_MAX_CLIENTS) as 5,000, the maximum number of CASes(MAX_NUM_APPL_SERVER) as 200 and the maximum number of proxy process(SHARD_NUM_PROXY) as 1, then file descriptors per proxy process becomes (5,000 + 200)/1 + 256 = 5,456, and it is less than 10,000; it is possible configuration.
     
    Regarding above, the following is the connection-relationship between each process. "proxy" intermediates a connection between "app. client" and "CAS".

    In the below, [] indicates a process, and -> indicates the requesting direction.

    ::
     
        [app. client]   --(initial access request)-----------> [broker] (select proxy)
                        <--(announce proxy to access)---
                        -------------------------------------> [proxy] --(select CAS)--> [CAS] ---> [DB server]
                        <-------------------------------------         <----------------       <--- 
                        <--(now use the same proxy)---------->         <--------------->       <-->

    broker is used only once when the application client requires the initial access, and then CUBRID keep connections among "[app. client] - [proxy] - [CAS] - [DB Server]".

    Also, CAS keeps connection with DB server after DB connection is completed.


Etc
^^^

**MAX_STRING_LENGTH**

    **MAX_STRING_LENGTH** is a parameter to configure the maximum string length for BIT, VARBIT, CHAR and VARCHAR data types. If the value is **-1**, which is the default value, the length defined in the database is used. If the value is **100**, the value acts like 100 being applied even when a certain attribute is defined as VARCHAR(1000).

**SERVICE**

    **SERVICE** is a parameter to configure whether to run the broker. It can be either **ON** or **OFF**. The default value is **ON**. The broker can run only when this value is configured to **ON**.

**SSL**

    **SSL** is a parameter to configure whether to apply packet encryption (**SSL**) to the broker. It can be either **ON** or **OFF**. The default value is **OFF**. When this value is configured to **ON**, packet encryption using **TLS** will be applied to the broker/CAS.

    .. warning::

        When the broker is configured to do **TLS** (**SSL=ON**), clients such as **jdbc** client must connect in encryption mode, otherwise the connection request to the broker will be rejected. The opposite is also true. The connection request of SSL clients to the non-SSL broker will be rejected.

**SOURCE_ENV**

    **SOURCE_ENV** is a parameter used to determine the file where the operating system variable for each broker is configured. The extension of the file must be **env**. All parameters specified in **cubrid.conf** can also be configured by environment variables. For example, the **lock_timeout** parameter in **cubrid.conf** can also be configured by the **CUBRID_LOCK_TIMEOUT** environment variable. As another example, to block execution of DDL statements on broker1, you can configure **CUBRID_BLOCK_DDL_STATEMENT** to 1 in the file specified by **SOURCE_ENV**.

    An environment variable, if exists, has priority over **cubrid.conf**. The default value is **cubrid.env**.

**NET_BUF_SIZE**

    **NET_BUF_SIZE** is a parameter used to determine the network buffer size used by **CAS** to transmit results to the client. The result sets will be buffered into the **CAS** network buffer, and are sent to the client when the buffer is full. **NET_BUF_SIZE** is the size of the packet that CAS transmits to the client, and it may affect transmission efficiency.

HA Configuration
================

Regarding HA configuration, see :ref:`ha-configuration`.

SHARD Configuration
===================

Regarding SHARD configuration, see :ref:`default-shard-conf`.
