***********
시스템 설정
***********

Performance Tuning

This chapter provides information about configuring system parameters that can affect the system performance. System parameters determine overall performance and operation of the system. This chapter explains how to use configuration files for database server and broker as well as a description of each parameter. For CUBRID Manager server configuration, see CUBRID Manager Manual.

This chapter covers the following topics:

*   Configuring the database server



*   Configuring the broker



**Configuring the Database Server**

**Scope of Database Server Configuration**

CUBRID consists of the database server, the broker and the CUBRID Manager. Each component has its configuration file. The system parameter configuration file for the database server is
**cubrid.conf**
located in the
**$CUBRID/conf**
directory. System parameters configured in
**cubrid.conf**
affect overall performance and operation of the database system. Therefore, it is very important to understand the database server configuration.

The CUBRID database server has a client/server architecture. To be more specific, it is divided into a database server process linked to the server library and the broker process linked to the client library. The server process manages the database storage structure and provides concurrency and transaction functionalities. The client process prepares for query execution and manages object/schema.

System parameters for the database server, which can be set in the
**cubrid.conf**
file, are classified into a client parameter, a server parameter and a client/server parameter according to the range to which they are applied. A client parameter is only applied to client processes such as the broker. A server parameter affects the behaviors of the server processes. A client/server parameter must be applied to both server and client.

**Location of cubrid.conf File and How It Works**

*   A database server process refers only to the
    **$CUBRID/conf/cubrid.conf**
    file. Database-specific configurations are distinguished by sections in the
    **cubrid.conf**
    file.



*   A client process (i) refers to the
    **$CUBRID/conf/cubrid.conf**
    file and then (ii) additionally refers to the
    **cubrid.conf**
    file in the current directory (
    **$PWD**
    ). The configuration of the file in the current directory (
    **$PWD/cubrid.conf**
    ) overwrites that of the
    **$CUBRID/conf/cubrid.conf**
    file. That is, if the same parameter configuration exists in
    **$PWD/cubrid.conf**
    and in
    **$CUBRID/conf/cubrid.conf**
    , the configuration in
    **$PWD/cubrid.conf**
    has the priority.



**Changing Database Server Configuration**

**Editing the Configuration File**

You can add/delete parameters or change parameter values by manually editing the system parameter configuration file (
**cubrid.conf**
) in the
**$CUBRID/conf**
directory.

The following parameter syntax rules are applied when configuring parameters in the configuration file:

*   Parameter names are not case-sensitive.



*   The name and value of a parameter must be entered in the same line.



*   An equal sign (=) can be used to configure the parameter value. Spaces are allowed before and after the equal sign.



*   If the value of a parameter is a character string, enter the character string without quotes. However, use quotes if spaces are included in the character string.



**Using SQL Statements**

**Description**

You can configure a parameter value by using SQL statements in the CSQL Interpreter or CUBRID Manager's Query Editor. Note that you cannot change every parameter. For updatable parameters, see 
`cubrid.conf Configuration File and Default Parameters <#pm_pm_db_setting_htm>`_
.

**Syntax**

**SET SYSTEM PARAMETERS**
'
*parameter_name*
=value [{;
*name*
=value}...]'

*parameter_name*
is the name of a client parameter whose value is editable. In this syntax, value is the value of the given parameter. You can change multiple parameter values by separating them with semicolons (;). You must take caution when you apply changes of parameter values.

**Example**

The following example shows how to retrieve the result of an index scan in OID order and configure the number of queries to be stored in the history of the CSQL Interpreter to 70.

SET SYSTEM PARAMETERS 'index_scan_in_oid_order=1; csql_history_num=70';

**Using Session Commands of the CSQL Interpreter**

**Description**

You can configure system parameter values by using session commands (;
**SET**
) in the CSQL Interpreter. Note that you cannot change every parameter. For updatable parameters, see 
`cubrid.conf Configuration File and Default Parameters <#pm_pm_db_setting_htm>`_
.

**Example**

The following example shows how to configure the
**block_ddl_statement**
parameter to 1 so that execution of DDL statements is not allowed.

csql> ;se block_ddl_statement=1

=== Set Param Input ===

block_ddl_statement=1

**cubrid.conf Configuration File and Default Parameters**

CUBRID consists of the database server, the broker and the CUBRID Manager. The name of the configuration file for each component is as follows. These files are all located in the
**$CUBRID/conf**
directory.

*   Database server configuration file:
    **cubrid.conf**



*   Broker configuration file:
    **cubrid_broker.conf**



*   CUBRID Manager server configuration file:
    **cm.conf**



**cubrid.conf**
is a configuration file that sets system parameters for the CUBRID database server and determines overall performance and operation of the database system. In the
**cubrid.conf**
file, some important parameters needed for system installation are provided, having their default values.

**Database Server System Parameters**

The following are database server system parameters that can be used in the
**cubrid.conf**
configuration file. For the scope of
**client**
and
**server parameters**
, see
`Scope of Database Server Configuration <#pm_pm_server_general_htm>`_
.

You can change the parameters that are capable of dynamically changing the setting value through the
**SET SYSTEM PARAMETERS**
statement or a session command of the CSQL Interpreter,
**;set**
dynamically. If you are a DBA, you can change parameters regardless of the applied classification. However, if you are not a DBA, you can only change client parameters.

+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| **Category**                                                     | **Parameter Name**                  | **Applied**             | **Type** | **Default Value**              | **Dynamicity** |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Connection <#pm_pm_db_classify_connect_htm>`_                   | cubrid_port_id                      | client parameter        | int      | 1523                           |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | db_hosts                            | client parameter        | string   | NULL                           | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | max_clients                         | server parameter        | int      | 100                            |                |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Memory <#pm_pm_db_classify_memory_htm>`_                        | data_buffer_size                    | server parameter        | int      | 512M                           |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | index_scan_oid_buffer_size          | server parameter        | int      | 64K                            |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | sort_buffer_size                    | server parameter        | int      | 2M                             |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | temp_file_memory_size_in_pages      | server parameter        | int      | 4                              |                |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| thread_stack_size                                                | server parameter                    | int                     | 1048576  |                                |                |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Disk <#pm_pm_db_classify_disk_htm>`_                            | db_volume_size                      | server parameter        | int      | 512M                           |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | dont_reuse_heap_file                | server parameter        | bool     | no                             |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | temp_file_max_size_in_pages         | server parameter        | int      | -1                             |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | temp_volume_path                    | server parameter        | string   | NULL                           |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | unfill_factor                       | server parameter        | float    | 0.1                            |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | volume_extension_path               | server parameter        | string   | NULL                           |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | log_volume_size                     | server parameter        | int      | 512M                           |                |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Error message <#pm_pm_db_classify_error_htm>`_                  | call_stack_dump_activation_list     | client/server parameter | string   | NULL                           | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | call_stack_dump_deactivation_list   | client/server parameter | string   | NULL                           | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | call_stack_dump_on_error            | client/server parameter | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | error_log                           | client/server parameter | string   | cub_client.err, cub_server.err |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | error_log_level                     | client/server parameter | string   | SYNTAX                         | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | error_log_warning                   | client/server parameter | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | error_log_size                      | client/server parameter | int      | 8000000                        | available      |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Concurrency/Lock <#pm_pm_db_classify_lock_htm>`_                | deadlock_detection_interval_in_secs | server parameter        | float    | 1.0                            | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | isolation_level                     | client parameter        | int      | 3                              | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | lock_escalation                     | server parameter        | int      | 100000                         |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | lock_timeout_in_secs                | client parameter        | int      | -1                             | available      |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Logging <#pm_pm_db_classify_logging_htm>`_                      | adaptive_flush_control              | server parameter        | bool     | yes                            | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | background_archiving                | server parameter        | bool     | yes                            | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | checkpoint_every_npages             | server parameter        | int      | 10000                          |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | checkpoint_interval_in_mins         | server parameter        | int      | 720                            | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | force_remove_log_archives           | server parameter        | bool     | yes                            | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | log_buffer_size                     | server parameter        | int      | 2M                             |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | log_max_archives                    | server parameter        | int      | INT_MAX                        | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | max_flush_pages_per_second          | server parameter        | int      | 10000                          | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | page_flush_interval_in_msecs        | server parameter        | int      | 0                              | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | sync_on_nflush                      | server parameter        | int      | 200                            | available      |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Transaction handling <#pm_pm_db_classify_transaction_ht_3621>`_ | async_commit                        | server parameter        | bool     | no                             |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | group_commit_interval_in_msecs      | server parameter        | int      | 0                              | available      |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Statement/Type <#pm_pm_db_classify_type_htm>`_                  | add_column_update_hard_default      | client parameter        | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | alter_table_change_type_strict      | client/server parameter | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | ansi_quotes                         | client parameter        | bool     | yes                            |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | block_ddl_statement                 | client parameter        | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | block_nowhere_statement             | client parameter        | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | compat_numeric_division_scale       | client/server parameter | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | default_week_format                 | 서버/client parameter     | int      | 0                              | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | group_concat_max_len                | server parameter        | int      | 1024                           | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | intl_check_input_string             | client parameter        | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | intl_date_lang                      | client parameter        | string   |                                | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | intl_number_lang                    | client parameter        | string   |                                | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | no_backslash_escapes                | client parameter        | bool     | yes                            |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | only_full_group_by                  | client parameter        | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | oracle_style_empty_string           | client parameter        | bool     | no                             |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | pipes_as_concat                     | client parameter        | bool     | yes                            |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | plus_as_concat                      | client parameter        | bool     | yes                            |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | require_like_escape_character       | client parameter        | bool     | no                             |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | return_null_on_function_errors      | client/server parameter | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | string_max_size_bytes               | client/server parameter | int      | 1048576                        | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | unicode_input_normalization         | client/server parameter | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | unicode_output_normalization        | client/server parameter | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Query cache <#pm_pm_db_classify_querycache_htm>`_               | max_plan_cache_entries              | client/server parameter | int      | 1000                           |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | max_filter_pred_cache_entries       | client/server parameter | int      | 1000                           |                |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Utility <#pm_pm_db_classify_utility_htm>`_                      | backup_volume_max_size_bytes        | server parameter        | int      | -1                             |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | communication_histogram             | client parameter        | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | compactdb_page_reclaim_only         | server parameter        | int      | 0                              |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | csql_history_num                    | client parameter        | int      | 50                             | available      |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `HA <#pm_pm_db_classify_ha_htm>`_                                | ha_mode                             | server parameter        | string   | off                            |                |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+
| `Others <#pm_pm_db_classify_etc_htm>`_                           | access_ip_control                   | server parameter        | bool     | no                             |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | access_ip_control_file              | server parameter        | string   |                                |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | auto_restart_server                 | server parameter        | bool     | yes                            | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | index_scan_in_oid_order             | client parameter        | bool     | no                             | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | index_unfill_factor                 | server parameter        | float    | 0.05                           |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | java_stored_procedure               | server parameter        | bool     | no                             |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | multi_range_optimization_limit      | server parameter        | int      | 100                            | available      |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | pthread_scope_process               | server parameter        | bool     | yes                            |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | server                              | server parameter        | string   |                                |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | service                             | server parameter        | string   |                                |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | session_state_timeout               | server parameter        | int      | 21600                          |                |
|                                                                  |                                     |                         |          |                                |                |
|                                                                  +-------------------------------------+-------------------------+----------+--------------------------------+----------------+
|                                                                  | use_orderby_sort_limit              | server parameter        | bool     | yes                            | available      |
|                                                                  |                                     |                         |          |                                |                |
+------------------------------------------------------------------+-------------------------------------+-------------------------+----------+--------------------------------+----------------+

**Section by Parameter**

Parameters specified in
**cubrid.conf**
have the following three sections:

*   Used when the CUBRID service starts: [service] section



*   Applied commonly to all databases: [common] section



*   Applied individually to each database: [@<
    *database*
    >] section



Where <
*database*
> is the name of the database to which each parameter applies. If a parameter configured in [common] is the same as the one configured in [@<
*database*
>], the one configured in [@<
*database*
>] is applied.

**Default Parameters**

**cubrid.conf**
, a default database configuration file created during the CUBRID installation, includes some default database server parameters that must be changed. You can change the value of a parameter that is not included as a default parameter by manually adding or editing one.

The following is the content of the
**cubrid.conf**
file.

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

#server=server, broker, manager

 

# Common section - properties for all databases

# This section will be applied before other database specific sections.

[common]

 

# Read the manual for detailed description of system parameters

# Manual > Performance Tuning > Database Server Configuration > Default Parameters

 

# Size of data buffer are using K, M, G, T unit

data_buffer_size=512M

 

# Size of log buffer are using K, M, G, T unit

log_buffer_size=4M

 

# Size of sort buffer are using K, M, G, T unit

# The sort buffer should be allocated per thread.

# So, the max size of the sort buffer is sort_buffer_size * max_clients.

sort_buffer_size=2M

 

# The maximum number of concurrent client connections the server will accept.

# This value also means the total # of concurrent transactions.

max_clients=100

 

# TCP port id for the CUBRID programs (used by all clients).

cubrid_port_id=1523

**Connection-Related Parameters**

The following are parameters related to the database server. The type and value range for each parameter are as follows:

+--------------------+----------+-------------------+---------+---------+
| **Parameter Name** | **Type** | **Default Value** | **Min** | **Max** |
|                    |          |                   |         |         |
+--------------------+----------+-------------------+---------+---------+
| cubrid_port_id     | int      | 1523              | 1       |         |
|                    |          |                   |         |         |
+--------------------+----------+-------------------+---------+---------+
| db_hosts           | string   | NULL              |         |         |
|                    |          |                   |         |         |
+--------------------+----------+-------------------+---------+---------+
| max_clients        | int      | 100               | 10      | 10000   |
|                    |          |                   |         |         |
+--------------------+----------+-------------------+---------+---------+

**cubrid_port_id**

**cubrid_port_id**
is a parameter used to configure the port to be used by the master process. The default value is
**1,523**
. If the port 1,523 is already being used on the server where CUBRID is installed or it is blocked by a firewall, an error message, which means the master server is not connected because the master process cannot be running properly, is displayed. If such port conflict occurs, the administrator must change the value of
**cubrid_port_id**
considering the server environment.

**db_hosts**

**db_hosts**
is a parameter used to configure a list of the database server hosts to which clients can connect, and the connection order. The server host list consists of multiple server host names, and host names are separated by spaces or colons (:). Duplicate or non-existent names are ignored.

The following example shows the values of the
**db_hosts**
parameter. In this example, connections are attempted in the order of
**host1**
>
**host2**
>
**host3**
.

db_hosts="hosts1:hosts2:hosts3"

To connect to the server, the client first tries to connect to the specified server host referring to the database location file (
**databases.txt**
). If the connection fails, the client then tries to connect to the first one of the secondarily specified server hosts by referring to the value of the
**db_hosts**
parameter in the database configuration file (
**cubrid.conf**
).

**max_clients**

**max_clients**
is a parameter used to configure the maximum number of clients (usually broker application processes (CAS)) which allow concurrent connections to the database server. The
**max_clients**
parameter refers to the number of concurrent transactions. The default value is
**100**
.

To grantee performance while increasing the number of concurrent users in CUBRID environment, you need to make the appropriate value of the
**max_clients**
(
**cubrid.conf**
) parameter and the
`MAX_NUM_APPL_SERVER <#pm_pm_broker_one_htm_max_num_app_7692>`_
(
**cubrid_broker.conf**
) parameter. That is, you are required to configure the number of concurrent connections allowed by databases with the
**max_clients**
parameter. You should also configure the number of concurrent connections allowed by brokers with the
**MAX_NUM_APPL_SERVER**
parameter.

For example, in the
**cubrid_broker.conf**
file, two node of a broker where the
**MAX_NUM_APPL_SERVER**
value of [%query_editor] is 50 and the 
**MAX_NUM_APPL_SERVER**
value of [%BROKER1] is 50 is trying to connect one database server, the concurrent connections (
**max_clients**
value) allowed by the database server can be configured as follows:

*   (the maximum number of 100 by each node of a broker) * (two node of a broker) + (10 spare for database server connections of internal CUBRID process such as database server connection of CSQL Interpreter or HA log replication process) = 210



Especially, in HA environment, the value must be greater than the sum specified in
**MAX_NUM_APPL_SERVER**
of every broker node which connects to the same database.

Note that the memory usage is affected by the value specified in
**max_clients**
. That is, if the number of value is high, the memory usage will increase regardless of whether or not the clients actually access the database.

**Memory-Related Parameters**

The following are parameters related to the memory used by the database server or client. The type and value range for each parameter are as follows:

+--------------------------------+----------+-------------------+---------+-------------+
| **Parameter Name**             | **Type** | **Default Value** | **Min** | **Max**     |
|                                |          |                   |         |             |
+--------------------------------+----------+-------------------+---------+-------------+
| data_buffer_size               | int      | 512M              | 16M     | 2G (32 bit) |
|                                |          |                   |         |             |
+--------------------------------+----------+-------------------+---------+-------------+
| index_scan_oid_buffer_size     | int      | 64K               | 1K      | 256K        |
|                                |          |                   |         |             |
+--------------------------------+----------+-------------------+---------+-------------+
| sort_buffer_size               | int      | 2M                | 64K     |             |
|                                |          |                   |         |             |
+--------------------------------+----------+-------------------+---------+-------------+
| temp_file_memory_size_in_pages | int      | 4                 | 0       | 20          |
|                                |          |                   |         |             |
+--------------------------------+----------+-------------------+---------+-------------+
| thread_stacksize               | int      | 1048576           | 65536   |             |
|                                |          |                   |         |             |
+--------------------------------+----------+-------------------+---------+-------------+

**data_buffer_size**

**data_buffer_size**
is a parameter used to configure the size of data buffer to be cached in the memory by the database server. You can set units as K, M, G and T, which stand for kilobytes (KB), megabytes(MB), gigabytes (GB), and terabytes (TB) respectively. If you omit the unit, bytes will be applied. The default value is 512M, and the minimum value is 16M. Note that the maximum value of 32-bit CUBRID is 2 GB.

The greater the value of the
**data_buffer_size**
parameter, the more data pages to be cached in the buffer, thus providing the advantage of decreased disk I/O cost. However, if this parameter is too large, the buffer pool can be swapped out by the operating system because the system memory is excessively occupied. It is recommended to configure the
**data_buffer_size**
parameter in a way the required memory size is less than two-thirds of the system memory size.

*   Required memory size = data buffer size (
    **data_buffer_size**
    )



**index_scan_oid_buffer_size**

**index_scan_oid_buffer_size**
is a parameter used to configure the size of buffer where the OID list is to be temporarily stored during the index scan.  You can set units as K, M, G and T, which stand for KB (kilobytes), MB (megabytes), GB (gigabytes) and TB (terabytes), respectively. If you omit the unit, bytes will be applied. The default value is 64K, the minimum value is 1K, and the maximum value is 256K.

The size of the OID buffer tends to vary in proportion to the value of the
**index_scan_oid_buffer_size**
parameter and the page size set when the database was created. In addition, the bigger the size of such OID buffer, the more the index scan cost. You can set the value of the
**index_scan_oid_buffer_size**
by considering these factors.

**sort_buffer_size**

**sort_buffer_size**
is a parameter used to configure the size of buffer to be used when sorting. You can set units as K, M, G and T, which stand for kilobytes (KB), megabytes (MB), gigabytes (GB), and terabytes (TB) respectively. If you omit the unit, bytes will be applied. The default value is 2M, and the minimum value is 64K.

The server assigns one sort buffer for each client request, and releases the assigned buffer memory when sorting is complete.

**temp_file_memory_size_in_pages**

**temp_file_memory_size_in_pages**
is a parameter used to configure the number of buffer pages to cache temporary result of a query. The default value is
**4**
and the maximum value is 20.

*   Required memory size = the number of temporary memory buffer pages (
    **temp_file_memory_size_in_pages**
    *
    **page size**
    )



*   The number of temporary memory buffer pages = the value of the
    **temp_file_memory_size_in_pages**
    parameter



*   Page size = the value of the page size specified by the
    **-s**
    option of the
    **cubrid createdb**
    utility during the database creation



**thread_stacksize**

**thread_stacksize**
is a parameter used to configure the stack size of a thread. The default value is
**1048576**
bytes. The value of the
**thread_stacksize**
parameter must not exceed the stack size allowed by the operating system.

**Disk-Related Parameters**

The following are disk-related parameters for defining database volumes and storing files. The type and value range for each parameter are as follows:

+-----------------------------+----------+-------------------+----------+----------+
| **Parameter Name**          | **Type** | **Default Value** | **Min.** | **Max.** |
|                             |          |                   |          |          |
+-----------------------------+----------+-------------------+----------+----------+
| db_volume_size              | int      | 512M              | 20M      | 20G      |
|                             |          |                   |          |          |
+-----------------------------+----------+-------------------+----------+----------+
| dont_reuse_heap_file        | bool     | no                |          |          |
|                             |          |                   |          |          |
+-----------------------------+----------+-------------------+----------+----------+
| log_volume_size             | int      | 512M              | 20M      | 4G       |
|                             |          |                   |          |          |
+-----------------------------+----------+-------------------+----------+----------+
| temp_file_max_size_in_pages | int      | -1                |          |          |
|                             |          |                   |          |          |
+-----------------------------+----------+-------------------+----------+----------+
| temp_volume_path            | string   | NULL              |          |          |
|                             |          |                   |          |          |
+-----------------------------+----------+-------------------+----------+----------+
| unfill_factor               | float    | 0.1               | 0.0      | 0.3      |
|                             |          |                   |          |          |
+-----------------------------+----------+-------------------+----------+----------+
| volume_extension_path       | string   | NULL              |          |          |
|                             |          |                   |          |          |
+-----------------------------+----------+-------------------+----------+----------+

**db_volume_size**

**db_volume_size**
is a parameter used to configure the following values. The default value is
**512M**
.

*   The default database volume size when
    **cubrid createdb**
    and
    **cubrid addvoldb**
    utility is used without
    **--db-volume-size**
    option.



*   The default size of generic volume that is added automatically when database volume is full.



**dont_reuse_heap_file**

**dont_reuse_heap_file**
is a parameter used to configure whether or not heap files, which are deleted when deleting the table (DROP TABLE), are to be reused when creating a new table (CREATE TABLE). If this parameter is set to 0, the deleted heap files can be reused; if it is set to 1, the deleted heap files are not used when creating a new table. The default value is
**0**
.

**log_volume_size**

**log_volume_size**
is a parameter used to configure the default size of log volume file when the
**cubrid createdb**
utility is used without --log-volume-size option. You can set units as K, M, G and T, which stand for kilobytes (KB), megabytes (MB), gigabytes (GB) and terabytes (TB) respectively. If you omit the unit, bytes will be applied. The default value is
**512M**
.

**temp_file_max_size_in_pages**

**temp_file_max_size_in_pages**
is a parameter used to configure the maximum number of pages to store temporary volumes in the disk, which are used for the execution of complex queries or sorting; the default value is
**-1**
. If this parameter is configured to the default value, unlimited number of temporary temp volumes are created and stored in the directory specified by the
**temp_volume_path**
parameter. If it is configured to 0, the administrator must create permanent temp volumes manually by using the
**cubrid addvoldb**
utility because temporary temp volumes are not created automatically.

**temp_volume_path**

**temp_volume_path**
is a parameter used to configure the directory in which to create temporary temp volumes used for the execution of complex queries or sorting. The default value is the volume location configured during the database creation.

**unfill_factor**

**unfill_factor**
is a parameter used to configure the rate of disk space to be allocated in a heap page for data updates. The default value is
**0.1**
. That is, the rate of free space is configured to 10%. In principle, data in the table is inserted in physical order. However, if the size of the data increases due to updates and there is not enough space for storage in the given page, performance may degrade because updated data must be relocated to another page. To prevent such a problem, you can configure the rate of space for a heap page by using the
**unfill_factor**
parameter. The allowable maximum value is 0.3 (30%). In a database where data updates rarely occur, you can configure this parameter to 0.0 so that space will not be allocated in a heap page for data updates. If the value of the
**unfill_factor**
parameter is negative or greater than the maximum value, the default value (
**0.1**
) is used.

**volume_extension_path**

**volume_extension_path**
is a parameter used to configure the directory where automatically extended volumes are to be created. The default value is the volume location configured during the database creation.

**Error Message-Related Parameters**

The following are parameters related to processing error messages recorded by CUBRID. The type and value range for each parameter are as follows:

+-----------------------------------+----------+--------------------------------+
| **Parameter Name**                | **Type** | **Default Value**              |
|                                   |          |                                |
+-----------------------------------+----------+--------------------------------+
| call_stack_dump_activation_list   | string   | NULL                           |
|                                   |          |                                |
+-----------------------------------+----------+--------------------------------+
| call_stack_dump_deactivation_list | string   | NULL                           |
|                                   |          |                                |
+-----------------------------------+----------+--------------------------------+
| call_stack_dump_on_error          | bool     | no                             |
|                                   |          |                                |
+-----------------------------------+----------+--------------------------------+
| error_log                         | string   | cub_client.err, cub_server.err |
|                                   |          |                                |
+-----------------------------------+----------+--------------------------------+
| error_log_level                   | string   | SYNTAX                         |
|                                   |          |                                |
+-----------------------------------+----------+--------------------------------+
| error_log_warning                 | bool     | no                             |
|                                   |          |                                |
+-----------------------------------+----------+--------------------------------+
| error_log_size                    | int      | 8000000                        |
|                                   |          |                                |
+-----------------------------------+----------+--------------------------------+

**call_stack_dump_activation_list**

**call_stack_dump_activation_list**
is a parameter used to configure a certain error number for which a call stack is to be dumped as an exception even when you configure that a call stack will not be dumped for any errors. Therefore, the
**call_stack_dump_activation_list**
parameter is effective only when
**call_stack_dump_on_error=no**
.

The following errors are included in
**call_stack_dump_activation_list**
.

+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| **Error Number** | **Error Message**                                                                                                                             |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -2               | Internal system failure: no more specific information is available.                                                                           |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -7               | Trying to format disk volume xxx with an incorrect value xxx for number of pages.                                                             |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -13              | An I/O error occurred while reading page xxx of volume xxx.                                                                                   |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -14              | An I/O error occurred while writing page xxx of volume xxx.                                                                                   |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -17              | Internal error: fetching deallocated pageid xxx of volume xxx.                                                                                |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -19              | Internal error: pageptr = xxx of page xxx of volume xxx is not fixed.                                                                         |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -21              | Internal error: unknown sector xxx of volume xxx.                                                                                             |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -22              | Internal error: unknown page xxx of volume xxx.                                                                                               |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -45              | Slot xxx on page xxx of volume xxx is allocated to an anchored record. A new record cannot be inserted here.                                  |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -46              | Internal error: slot xxx on page xxx of volume xxx is not allocated.                                                                          |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -48              | Accessing deleted object xxx|xxx|xxx.                                                                                                         |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -50              | Internal error: relocation record of object xxx|xxx|xxx may be corrupted.                                                                     |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -51              | Internal error: object xxx|xxx|xxx may be corrupted.                                                                                          |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -52              | Internal error: object overflow address xxx|xxx|xxx may be corrupted.                                                                         |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -76              | Your transaction (index xxx, xxx@xxx|xxx) timed out waiting on xxx on page xxx|xxx. You are waiting for user(s) xxx to release the page lock. |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -78              | Internal error: an I/O error occurred while reading logical log page xxx (physical page xxx) of xxx.                                          |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -79              | Internal error: an I/O error occurred while writing logical log page xxx (physical page xxx) of xxx.                                          |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -81              | Internal error: logical log page xxx may be corrupted.                                                                                        |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -90              | Redo logging is always a page level logging operation. A data page pointer must be given as part of the address.                              |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -96              | Media recovery may be needed on volume xxx.                                                                                                   |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -97              | Internal error: unable to find log page xxx in log archives.                                                                                  |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -313             | Object buffer underflow while reading.                                                                                                        |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -314             | Object buffer overflow while writing.                                                                                                         |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -407             | Unknown key xxx referenced in B+tree index {vfid: (xxx, xxx), rt_pgid: xxx, key_type: xxx}.                                                   |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -414             | Unknown class identifier: xxx|xxx|xxx.                                                                                                        |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -415             | Invalid class identifier: xxx|xxx|xxx.                                                                                                        |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -416             | Unknown representation identifier: xxx.                                                                                                       |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -417             | Invalid representation identifier: xxx.                                                                                                       |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -583             | Trying to allocate an invalid number (xxx) of pages.                                                                                          |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -603             | Internal Error: Sector/page table of file VFID xxx|xxx seems corrupted.                                                                       |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -836             | LATCH ON PAGE(xxx|xxx) TIMEDOUT                                                                                                               |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -859             | LATCH ON PAGE(xxx|xxx) ABORTED                                                                                                                |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -890             | Partition failed.                                                                                                                             |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -891             | Appropriate partition does not exist.                                                                                                         |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -976             | Internal error: Table size overflow (allocated size: xxx, accessed size: xxx) at file table page xxx|xxx(volume xxx)                          |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -1040            | HA generic: xxx.                                                                                                                              |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| -1075            | Descending index scan aborted because of lower priority on B+tree with index identifier: (vfid = (xxx, xxx), rt_pgid: xxx).                   |
|                  |                                                                                                                                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+

If error numbers are configured in
**call_stack_dump_activation_list**
of
**cubrid.conf**
, it is working like configuration done including errors above.

The following example shows how to make error numbers -115, -116, and error numbers perform call-stack dump.

call_stack_dump_on_error= no

call_stack_dump_activation_list=-115,-116

**call_stack_dump_deactivation_list**

**call_stack_dump_deactivation_list**
is a parameter used to configure a certain error number for which a call stack is not to be dumped when you configure that a call stack will be dumped for any errors. Therefore, the
**call_stack_dump_deactivation_list**
parameter is effective only when
**call_stack_dump_on_error**
is set to
**yes**
.

The following example shows how to configure the parameter so that call stacks will be dumped for any errors, except the ones whose numbers are -115 and -116.

call_stack_dump_on_error= yes

call_stack_dump_deactivation_list=-115,-116

**call_stack_dump_on_error**

**call_stack_dump_on_error**
is a parameter used to configure whether or not to dump a call stack when an error occurs in the database server. If this parameter is set to no, a call stack for any errors is not dumped. If it is set to yes, a call stack for all errors is dumped. The default value is
**no**
.

**error_log**

**error_log**
is a server/client parameter used to configure the name of the error log file when an error occurs in the database server. The name of the error log file must be in the form of <
*database_name*
>_<
*date*
>_<
*time*
>.
**err**
. However, the naming rule of the error log file does not apply to errors for which the system cannot find the database server information. Therefore, error logs are recorded in the
**cubrid.err**
file. The error log file
**cubrid.err**
is stored in the
**$CUBRID/log/server**
directory.

**error_log_level**

**error_log_level**
is a server parameter used to configure an error message to be stored based on severity. There are five different levels which ranges from
**NOTIFICATION**
(lowest level),
**WARNING**
,
**SYNTAX**
,
**ERROR**
, and
**SYNTAX**
(highest level). An error message with
**SYNTAX**
,
**ERROR**
, and FATAL levels are stored in the log file if severity of error is
**SYNTAX**
, default value.

**error_log_warning**

**error_log_warning**
is a parameter used to configure whether or not error messages with a severity level of
**WARNING**
are to be displayed. Its default value is no. Therefore, only error messages with levels other than
**WARNING**
will be stored even when
**error_log_level**
is set to
**NOTIFICATION**
. For this reason, you must set
**error_log_warning**
to
**yes**
to store WARNING messages to an error log file.

**error_log_size**

**error_log_size**
is a parameter used to configure the maximum number of lines per an error log file. The default value is
**8,000,000**
. If it reaches up the specified number, the <
*database_name*
>_<
*date*
>_<
*time*
>.
**err**
.
**bak**
file is created. 

**Concurrency/Lock-Related Parameters**

The following are parameters related to concurrency control and locks of the database server. The type and value range for each parameter are as follows:

+-------------------------------------+----------+-------------------+---------+---------+
| **Parameter Name**                  | **Type** | **Default Value** | **Min** | **Max** |
|                                     |          |                   |         |         |
+-------------------------------------+----------+-------------------+---------+---------+
| deadlock_detection_interval_in_secs | float    | 1.0               | 0.1     |         |
|                                     |          |                   |         |         |
+-------------------------------------+----------+-------------------+---------+---------+
| isolation_level                     | int      | 3                 | 1       | 6       |
|                                     |          |                   |         |         |
+-------------------------------------+----------+-------------------+---------+---------+
| lock_escalation                     | int      | 100000            | 5       |         |
|                                     |          |                   |         |         |
+-------------------------------------+----------+-------------------+---------+---------+
| lock_timeout_in_secs                | int      | -1                | -1      |         |
|                                     |          |                   |         |         |
+-------------------------------------+----------+-------------------+---------+---------+

**deadlock_detection_interval_in_secs**

**deadlock_detection_interval_in_secs**
is a parameter used to configure the interval (in seconds) in which deadlocks are detected for stopped transactions. If a deadlock occurs, CUBRID resolves the problem by rolling back one of the transactions. The default value is 1 second and the minimum value is 0.1 second. This value is rounded up by 0.1 sec. unit. For example, if an input value is 0.12 seconds, the value is rounded up to 0.2 seconds. Note that deadlocks cannot be detected if the detection interval is too long.

**isolation_level**

**isolation_level**
is a parameter used to configure the isolation level of a transaction. The higher the isolation level, the less concurrency and the less interruption by other concurrent transactions. The
**isolation_level**
parameter can be configured to an integer value from 1 to 6, which represent isolation levels, or character strings. The default value is
**TRAN_REP_CLASS_UNCOMMIT_INSTANCE**
. For details about each isolation level and parameter values, see
`Setting Isolation Level <#syntax_syntax_tran_isolation_set_4219>`_
and the following table.

+--------------------------------------------------------------------------+-------------------------------------------------------------------------------------------+
| **Isolation Level**                                                      | **isolation_level Parameter Value**                                                       |
|                                                                          |                                                                                           |
+--------------------------------------------------------------------------+-------------------------------------------------------------------------------------------+
| SERIALIZABLE                                                             | "TRAN_SERIALIZABLE" or 6                                                                  |
|                                                                          |                                                                                           |
+--------------------------------------------------------------------------+-------------------------------------------------------------------------------------------+
| REPEATABLE READ CLASS with REPEATABLE READ INSTANCES                     | "TRAN_REP_CLASS_REP_INSTANCE" or "TRAN_REP_READ" or 5                                     |
|                                                                          |                                                                                           |
+--------------------------------------------------------------------------+-------------------------------------------------------------------------------------------+
| REPEATABLE READ CLASS with READ COMMITTED INSTANCES(or CURSOR STABILITY) | "TRAN_REP_CLASS_COMMIT_INSTANCE" or "TRAN_READ_COMMITTED" or "TRAN_CURSOR_STABILITY" or 4 |
|                                                                          |                                                                                           |
+--------------------------------------------------------------------------+-------------------------------------------------------------------------------------------+
| REPEATABLE READ CLASS with READ UNCOMMITTED INSTANCES                    | "TRAN_REP_CLASS_UNCOMMIT_INSTANCE" or "TRAN_READ_UNCOMMITTED" or 3                        |
|                                                                          |                                                                                           |
+--------------------------------------------------------------------------+-------------------------------------------------------------------------------------------+
| READ COMMITTED CLASS with READ COMMITTED INSTANCES                       | "TRAN_COMMIT_CLASS_COMMIT_INSTANCE" or 2                                                  |
|                                                                          |                                                                                           |
+--------------------------------------------------------------------------+-------------------------------------------------------------------------------------------+
| READ COMMITTED CLASS with READ UNCOMMITTED INSTANCES                     | "TRAN_COMMIT_CLASS_UNCOMMIT_INSTANCE" or 1                                                |
|                                                                          |                                                                                           |
+--------------------------------------------------------------------------+-------------------------------------------------------------------------------------------+

*   **TRAN_SERIALIZABLE**
    : This isolation level ensures the highest level of consistency. For details, see
    `SERIALIZABLE <#syntax_syntax_tran_isolation_ser_6285>`_
    .



*   **TRAN_REP_CLASS_REP_INSTANCE**
    : This isolation level can occur phantom read. For details, see
    `REPEATABLE READ CLASS with REPEATABLE READ INSTANCES <#syntax_syntax_tran_isolation_rep_7879>`_
    .



*   **TRAN_REP_CLASS_COMMIT_INSTANCE**
    : This isolation level can occur unrepeatable read. For details, see
    `REPEATABLE READ CLASS with READ COMMITTED INSTANCES <#syntax_syntax_tran_isolation_rep_8779>`_
    .



*   **TRAN_REP_CLASS_UNCOMMIT_INSTANCE**
    : This isolation level can occur dirty read. For details, see
    `REPEATABLE READ CLASS with READ UNCOMMITTED INSTANCES <#syntax_syntax_tran_isolation_rep_4346>`_
    .



*   **TRAN_COMMIT_CLASS_COMMIT_INSTANCE**
    : This isolation level can occur unrepeatable read. It allows modification of table schema by current transactions while data is being retrieved. For details, see
    `READ COMMITTED CLASS with READ COMMITTED INSTANCES <#syntax_syntax_tran_isolation_rea_875>`_
    .



*   **TRAN_COMMIT_CLASS_UNCOMMIT_INSTANCE**
    : This isolation level can occur dirty read. It allows modification of table schema by current transactions while data is being retrieved. For details, see
    `READ COMMITTED CLASS with READ UNCOMMITTED INSTANCES <#syntax_syntax_tran_isolation_rea_9641>`_
    .



**lock_escalation**

**lock_escalation**
is a parameter used to configure the maximum number of locks permitted before row level locking is extended to table level locking. The default value is
**100,000**
. If the value of the
**lock_escalation**
parameter is small, the overhead by memory lock management is small as well; however, the concurrency decreases. On the other hand, if the configured value is large, the overhead is large as well; however, the concurrency increases.

**lock_timeout_in_secs**

**lock_timeout_in_secs**
is a client parameter used to configure the lock waiting time. If the lock is not permitted within the specified time period, the given transaction is canceled, and an error message is returned. If the parameter is configured to
**-1**
, which is the default value, the waiting time is infinite until the lock is permitted. If it is configured to 0, there is no waiting for locks.

**Logging-Related Parameters**

The following are parameters related to logs used for database backup and restore. The types and value range for each parameter are as follows:

+------------------------------+----------+-------------------+----------+----------+
| **Parameter Name**           | **Type** | **Default Value** | **Min.** | **Max.** |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| adaptive_flush_control       | bool     | yes               |          |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| background_archiving         | bool     | yes               |          |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| checkpoint_every_npages      | int      | 10000             | 10       |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| checkpoint_interval_in_mins  | int      | 720               | 1        |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| force_remove_log_archives    | bool     | yes               |          |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| log_buffer_size              | int      | 2 MB              | 192 KB   |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| log_max_archives             | int      | INT_MAX           | 0        |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| max_flush_pages_per_second   | int      | 10000             | 1        | INT_MAX  |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| page_flush_interval_in_msecs | int      | 0                 | -1       |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| sync_on_nflush               | int      | 200               | 1        | INT_MAX  |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+

**adaptive_flush_control**

**adaptive_flush_control**
is a parameter used automatically to adjust the flush capacity at every 50 ms depending on the current status of the flushing operation. Its default value is
**yes**
. That is, this capacity is increased if a large number of
**INSERT**
or
**UPDATE**
operations are concentrated at a certain point of time and the number of flushed pages reaches the
**max_flush_pages_per_second**
parameter value; and is decreased otherwise. In the same way, you can distribute the I/O load by adjusting the flush capacity on a regular basis depending on the workload.

**background_archiving**

**background_archiving**
is a parameter used to create temporary archive logs periodically at a specific time. It is useful when balancing disk I/O load which has been caused by archiving logs. The default is
**yes**
.

**checkpoint_every_npages**

**checkpoint_every_npages**
is a parameter used to configure checkpoint interval by log page. The default value is
**10,000**
.

You can distribute disk I/O overload at the checkpoint by specifying lower number in the
**checkpoint_every_npages**
parameter, especially  in the environment where
**INSERT**
/
**UPDATE**
are heavily loaded at a specific time.

Checkpoint is a job to record every modified page in data buffers to database volumes (disk) at a specific point. It can restore data back to the latest checkpoint if database failure occurs. It is important to choose efficient checkpoint interval because large increase of log files stored in a disk may affect database operation, causing unnecessary disk I/O.

The
**checkpoint_interval_in_mins**
and
**checkpoint_every_npages**
parameters are related to setting checkpoint cycle. The checkpoint is periodically executed whenever the time specified in
**checkpoint_interval_in_mins**
parameter has elapsed or the number of log pages specified in
**checkpoint_every_npages**
parameter has reached.

**checkpoint_interval_in_mins**

**checkpoint_interval_in_mins**
is a parameter used to configure execution period of checkpoint in minutes. The default value is
**720**
.

**force_remove_log_archives**

**force_remove_log_archives**
is a parameter used to configure whether to allow the deletion of the files other than the recent log archive files of which the number is specified by
**log_max_archives**
. The default value is
**yes**
.

If the value is set to yes, the files will be deleted other than the recent log archive files for which the number is specified by
**log_max_archives**
. If it is set to no, the log archive files will not be deleted. Exceptionally, if
**ha_mode**
is set to on, the files other than the log archive files required for the HA-related processes and the recent log archive files of which the number is specified by
**log_max_archives**
will be deleted.

If you want to build the CUBRID HA environment, see
`Configuration <#admin_admin_ha_conf_cubrid_htm>`_
.

**log_buffer_size**

**log_buffer_size**
is a parameter used to configure the size of log buffer to be cached in the memory. There are four types of unit available: K, M, G, and T; K stands for kilobytes (KB), M stands for megabytes (MB), G stands for gigabytes (GB), and T stands for terabytes (TB). If unit is omitted, byte-unit is applied and the default value is
**2M**
.

If the value of the
**log_buffer_size**
parameter is large, performance can be improved (due to the decrease in disk I/O) in an environment where transactions are long and numerous. It is recommended to configure an appropriate value considering the memory size and operations of the system where CUBRID is installed.

*   Required memory size = the size of log buffer (
    **log_buffer_size**
    )



**log_max_archives**

**log_max_archives**
is a parameter used to configure the maximum number of archive log files. The minimum value is 0 and default value is
**INT_MAX**
(2147483647). It is set to 0 in the
**cubrid.conf**
file when CUBRID has installed. Its operations can differ depending on the configuration of
**force_remove_log_archives**
. For example, when
**log_max_archives**
is 3 and
**force_remove_log_archives**
is
**yes**
in the cubrid.conf file, the most recent three archive log files are recorded and when a fourth archiving log file is generated, the oldest archive log file is automatically deleted; the information about the deleted archive logs are recorded in the
***_lginf**
file.

However, if an active transaction still refers to an existing archive log file, the archive log file will not be deleted. That is, if a transaction starts at the point that the first archive log file is generated, and it is still active until the fifth archive log is generated, the first archive log file cannot be deleted.

For how to set up the CUBRID HA environment, see
`Administrator > CUBRID HA > cubrid.conf <#admin_admin_ha_conf_cubrid_htm>`_
.

**max_flush_pages_per_second**

**max_flush_pages_per_second**
is a parameter used to configure the maximum flush capacity when the flushing operation is performed from a buffer to a disk. Its default value is
**10,000**
. That is, you can prevent concentration of I/O load at a certain point of time by configuring this parameter to control the maximum flush capacity per second.

If a large number of
**INSERT**
or
**UPDATE**
operations are concentrated at a certain point of time, and the flush capacity reaches the maximum capacity set by this parameter, only log pages are flushed to the disk, and data pages are no longer flushed. Therefore, you must set an appropriate value for this parameter considering the workload of the service environment.

**page_flush_interval_in_msecs**

**page_flush_interval_in_msecs**
is a parameter used to configure the interval in milliseconds (msec.) at which dirty pages in a data buffer are flushed to a disk. Its default value is
**0**
. When the minimum value is set to -1, it work as that is set to 0. This is a parameter that is related to I/O load and buffer concurrency. For this reason, you must set its value in consideration of the workload of the service environment.

**sync_on_nflush**

**sync_on_nflush**
is a parameter used to configure the interval in pages between after data and log pages are flushed from buffer and before they are synchronized with FILE I/O of operating system. Its default value is
**200**
. That is, the CUBRID Server performs synchronization with the FILE I/O of the operating system whenever 200 pages have been flushed. This is also a parameter related to I/O load.

**Transaction Processing-Related Parameters**

The following are parameters for improving transaction commit performance. The type and value range for each parameter are as follows:

+--------------------------------+----------+-------------------+---------+---------+
| **Parameter Name**             | **Type** | **Default Value** | **Min** | **Max** |
|                                |          |                   |         |         |
+--------------------------------+----------+-------------------+---------+---------+
| async_commit                   | bool     | no                |         |         |
|                                |          |                   |         |         |
+--------------------------------+----------+-------------------+---------+---------+
| group_commit_interval_in_msecs | int      | 0                 | 0       |         |
|                                |          |                   |         |         |
+--------------------------------+----------+-------------------+---------+---------+

**async_commit**

**async_commit**
is a parameter used to activate the asynchronous commit functionality. If the parameter is set to no, which is the default value, the asynchronous commit is not performed; if it is set to yes, the asynchronous commit is executed. The asynchronous commit is a functionality that improves commit performance by completing the commit for the client before commit logs are flushed on the disk and having the log flush thread (LFT) perform log flushing in the background. Note that already committed transactions cannot be restored if a failure occurs on the database server before log flushing is performed.

**group_commit_interval_in_msecs**

**group_commit_interval_in_msecs**
is a parameter used to configure the interval (in milliseconds), at which the group commit is to be performed. If the parameter is configured to
**0**
, which is the default value, the group commit is not performed. The group commit is a functionality that improves commit performance by combining multiple commits that occurred in the specified time period into a group so that commit logs are flushed on the disk at once.

**Statement/Type-Related Parameters**

The following are parameters related to SQL statements and data types supported by CUBRID. The type and value range for each parameter are as follows:

+--------------------------------+----------+-------------------+----------------+----------------+
| **Parameter Name**             | **Type** | **Default Value** | **Min. Value** | **Max. Value** |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| add_column_update_hard_default | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| alter_table_change_type_strict | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| ansi_quotes                    | bool     | yes               |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| block_ddl_statement            | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| block_nowhere_statement        | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| compat_numeric_division_scale  | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| default_week_format            | int      | 0                 |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| group_concat_max_len           | int      | 1024              | 4              | 33554432       |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| intl_check_input_string        | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| intl_date_lang                 | string   |                   |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| intl_number_lang               | string   |                   |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| no_backslash_escapes           | bool     | yes               |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| only_full_group_by             | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| oracle_style_empty_string      | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| pipes_as_concat                | bool     | yes               |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| plus_as_concat                 | bool     | yes               |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| require_like_escape_character  | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| return_null_on_function_errors | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| string_max_size_bytes          | int      | 1048576           | 64             | 33554432       |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| unicode_input_normalization    | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+
| unicode_output_normalization   | bool     | no                |                |                |
|                                |          |                   |                |                |
+--------------------------------+----------+-------------------+----------------+----------------+

**add_column_update_hard_default**

**add_column_update_hard_default**
is a parameter used to configure whether or not to provide the hard_default value as the input value for a column when you add a new column to the
**ALTER TABLE … ADD COLUMN**
clause.

If a value for this parameter is set to yes, enter a new input value of a column as a hard default value when you have
**NOT NULL**
constraints but no
**DEFAULT**
constraints. If the parameter value is set to no, enter
**NULL**
, even if
**NOT NULL**
constraints exist. If a value for this parameter is set to yes and there is no hard default value for the column type to add, an error message will be displayed and a roll-back occurs. For the hard default for each type, see the
`CHANGE Clause <#syntax_syntax_def_alttable_chang_3554>`_
of the
**ALTER TABLE**
statement.

-- add_column_update_hard_default=no

 

CREATE TABLE tbl (i INT);

INSERT INTO tbl VALUES (1),(2);

ALTER TABLE tbl ADD COLUMN j INT NOT NULL;

 

SELECT * FROM TBL;

 

            i          j

========================

            2       NULL

            1       NULL

 

-- add_column_update_hard_default=yes

 

CREATE TABLE tbl (i int);

INSERT INTO tbl VALUES (1),(2);

ALTER TABLE tbl ADD COLUMN j INT NOT NULL;

 

SELECT * FROM tbl;

 

            i          j

=========================

            2          0

            1          0

**alter_table_change_type_strict**

**alter_table_change_type_strict**
is a parameter used to configure whether or not to allow the conversion of column values according to the type change, and the default value is
**no**
. If a value for this parameter is set to no, the value may be changed when you change the column types or when you add
**NOT NULL**
constraints; if it is set to yes, the value is not changed. For details, see CHANGE Clause in the
`CHANGE/MODIFY Clause <#syntax_syntax_def_alttable_chang_3554>`_
.

**ansi_quotes**

**ansi_quotes**
is a parameter used to enclose symbols and character string to handle identifiers. The default value is
**yes**
. If this parameter value is set to
**yes**
, double quotations are handled as identifier symbols and single quotations are handled as character string symbols. If it is set to
**no**
, both double and single quotations are handled as character string symbols.

**block_ddl_statement**

**block_ddl_statement**
is a parameter used to limit the execution of DDL (Data Definition Language) statements by the client. If the parameter is set to no, the given client is allowed to execute DDL statements. If it is set to yes, the client is not permitted to execute DDL statements. The default value is
**no**
.

**block_nowhere_statement**

**block_nowhere_statement**
is a parameter used to limit the execution of
**UPDATE**
/
**DELETE**
statements without a condition clause (
**WHERE**
) by the client. If the parameter is set to no, the given client is allowed to execute
**UPDATE**
/
**DELETE**
statements without a condition clause. If it is set to yes, the client is not permitted to execute
**UPDATE**
/
**DELETE**
statements without a condition clause. The default value is
**no**
.

**compat_numeric_division_scale**

**compat_numeric_division_scale**
is a parameter used to configure the scale to be displayed in the result (quotient) of a division operation. If the parameter is set to no, the scale of the quotient is 9 if it is set to yes, the scale is determined by that of the operand. The default value is
**no**
.

**default_week_format**

**default_week_format**
is a parameter used to configure default value for the
*mode*
attribute of the WEEK function. The default value is
**0**
. For details, see
`WEEK Function <#syntax_syntax_operator_datefunc__6582>`_
.

**intl_check_input_string**

**intl_check_input_string**
is a parameter used to whether to check that string entered is correctly corresponded to character set used. The default value is
**no**
. If this value is no and character set is UTF-8 and incorrect data is enter which violate UTF-8 byte sequence, it can show abnormal behavior or database server and applications can be termminated abnormally. However, if it is guaranteed this problem does not happen, it has advantage in performance not to do it.

UTF-8 and EUC-KR can be checked; ISO-8859-1 is one-byte encoding so it does not have to be checked because every byte is valid.

**group_concat_max_len**

**group_concat_max_len**
is a parameter used to limit the return value size of the
**GROUP_CONCAT**
function. The default value is
**1024**
bytes, the minimum value is 4 bytes, and the maximum value is 33,554,432 bytes. If the return value of the
**GROUP_CONCAT**
function exceeds the limitation,
**NULL**
will be returned.

**intl_check_input_string**

**intl_check_input_string**
is a parameter used to whether to check that string entered is correctly corresponded to character set used. The default value is
**no**
. If this value is no and character set is UTF-8 and incorrect data is enter which violate UTF-8 byte sequence, it can show abnormal behavior or database server and applications can be termminated abnormally. However, if it is guaranteed this problem does not happen, it has advantage in performance not to do it.

UTF-8 and EUC-KR can be checked; ISO-8859-1 is one-byte encoding so it does not have to be checked because every byte is valid.

**intl_date_lang**

**intl_date_lang**
is a parameter used to input/output the values of
**TIME**
,
**DATE**
,
**DATETIME**
, and
**TIMESTAMP**
. If language name is omitted, it specifies a locale format of string of localized calendar (month, weekday, and AM/PM).

The values allowed are as follows: Note that to use all values, locale library should be configured except built-in locale. For configuring locale, see
`Administrator Guide > Locale Setting <#admin_admin_i18n_locale_htm>`_
.

+--------------+-----------------------------+
| **Language** | **Locale Name of Language** |
|              |                             |
+--------------+-----------------------------+
| English      | en_US                       |
|              |                             |
+--------------+-----------------------------+
| German       | de_DE                       |
|              |                             |
+--------------+-----------------------------+
| Spanish      | es_ES                       |
|              |                             |
+--------------+-----------------------------+
| French       | fr_FR                       |
|              |                             |
+--------------+-----------------------------+
| Italian      | it_IT                       |
|              |                             |
+--------------+-----------------------------+
| Japanese     | ja_JP                       |
|              |                             |
+--------------+-----------------------------+
| Cambodian    | km_KH                       |
|              |                             |
+--------------+-----------------------------+
| Korean       | ko_KR                       |
|              |                             |
+--------------+-----------------------------+
| Turkish      | tr_TR                       |
|              |                             |
+--------------+-----------------------------+
| Vietnamese   | vi_VN                       |
|              |                             |
+--------------+-----------------------------+
| Chinese      | zh_CN                       |
|              |                             |
+--------------+-----------------------------+

The function recognizing input string based on calendar format of specified language is as follows:

*   **TO_DATE**



*   **TO_TIME**



*   **TO_DATETIME**



*   **TO_TIMESTAMP**



*   **STR_TO_DATE**



The function outputting string based on calendar format of specified language is as follows:

*   **TO_CHAR(date)**



*   **DATE_FORMAT**



*   **TIME_FORMAT**



**intl_number_lang**

**intl_number_lane**
 is a parameter used to specify locale applied when numeric format is assiged to input/output string in the function where a string is converted to number or number is converted to string. A delimiter and decimal symbol are used for numeric localization. In general, a comma and period are used; however, it can be changeable based on locale. For example, while number 1000.12 is used as 1,000.12 in most locale, it used as 1.000,12 in , tr_TR locale.

The function recognizing input string based on calendar format of specified language is as follows:

*   **TO_NUMBER**



The function outputting string based on calendar format of specified language is as follows:

*   **FORMAT**



*   **TO_CHAR(number)**



**no_backslash_escapes**

**no_backslash_escapes**
is a parameter used to configure whether or not to use backslash (\) as an escape character, and the default value is
**yes**
. If a value for this parameter is set to no, backslash (\) will be used as an escape character; if it is set to yes, backslash (\) will be used as a normal character. For details, see
`Escape Special Characters <#syntax_syntax_datatype_string_es_323>`_
.

**only_full_group_by**

**only_full_group_by**
is a parameter used to configure whether to use extended syntax about using
**GROUP BY**
statement.

If this parameter value is set to
**no**
, an extended syntax is applied thus, a column that is not specified in the
**GROUP BY**
statement can be specified in the
**SELECT**
column list. If it is set to yes, a column that is only specified in the
**GROUP BY**
statement can be the
**SELECT**
column list.

The default value is
**no**
. Therefore, specify the
**only_full_group_by**
parameter value to
**yes**
to execute queries by SQL standards. Because the extended syntax is not applied in this case, an error below is displayed.

ERROR: Attributes exposed in aggregate queries must also appear in the group by clause.

**oracle_style_empty_string**

**oracle_style_empty_string**
is a parameter used to improve compatibility with other DBMS (Database Management Systems) and specifies whether or not to process empty strings as
**NULL**
as in Oracle DBMS. If the
**oracle_style_empty_string**
parameter is set to no, the character string is processed as a valid string if it is set to yes, the empty string is processed as
**NULL**
.

**pipes_as_concat**

**pipes_as_concat**
is a parameter used to configure how to handle a double pipe symbol. The default value is
**yes**
. If this parameter value is set to
**yes**
, a double pipe symbol is handled as a concatenation operator if no, it is handled as the
**OR**
operator.

**plus_as_concat**

**plus_as_concat**
is a parameter used to configure the plus (+) operator, and the default value is
**yes**
. If a value for this parameter is set to yes, the plus (+) operator will be interpreted as a concatenation operator; if it is set to no, the operator will be interpreted as a numeric operator.

-- plus_as_concat = yes

SELECT '1'+'1';

         '1'+'1'

======================

         '11'  SELECT '1'+'a';

 

         '1'+'a'

======================

         '1a'

 

-- plus_as_concat = no

SELECT '1'+'1';

                '1'+'1'

==========================

 2.000000000000000e+000

 

SELECT '1'+'a';

 

ERROR: Cannot coerce 'a' to type double.

**require_like_escape_character**

**require_like_escape_character**
is parameter used to configure whether or not to use an ESCAPE character in the
**LIKE**
clause, and the default value is
**no**
. If a value for this parameter is set to yes and a value for
**no_backslash_escapes**
is set to no, backslash (\) will be used as an ESCAPE character in the strings of the LIKE clause, otherwise you should specify an ESCAPE character by using the
**LIKE… ESCAPE**
clause. For details, see
`LIKE Predicate <#syntax_syntax_operator_where_lik_9691>`_
.

**return_null_on_function_errors**

**return_null_on_function_errors**
is a parameter used to define actions when errors occur in some SQL functions, and the default value is
**no**
. If a value for this parameter is set to yes,
**NULL**
is returned; if it is set to no, an error is returned when the error occurs in functions, and the related message is displayed.

The following SQL functions are affected by this system parameter.

*   ADDDATE



*   ADDTIME



*   DATEDIFF



*   DAY



*   DAYOFMONTH



*   DAYOFWEEK



*   DAYOFYEAR



*   FROM_DAYS



*   FROM_UNIXTIME



*   HOUR



*   LAST_DAY



*   MAKEDATE



*   MAKETIME



*   MINUTE



*   MONTH



*   QUARTER



*   SEC_TO_TIME



*   SECOND



*   TIME



*   TIME_TO_SEC



*   TIMEDIFF



*   TO_DAYS



*   WEEK



*   WEEKDAY



*   YEAR



-- return_null_on_function_errors=no

 

SELECT YEAR('12:34:56)'

 

ERROR: Conversion error in time format.

 

-- return_null_on_function_errors=yes

 

SELECT YEAR('12:34:56);

 

     year('12:34:56')

=====================

   NULL

**string max_size_bytes**

**string_max_size_bytes**
is a parameter used to define the maximum byte allowable in string functions or operators. The default value is
**1048576**
(1 MB). The minimum value is 64 bytes and the maximum value is 33,554,432 bytes (32 MB).

The functions and operators affected by this parameter are as follows:

*   **SPACE**



*   **CONCAT**



*   **CONCAT_WS**



*   '
    **+**
    ': Operand of string



*   **REPEAT**



*   **GROUP_CONCAT**
    : This function is affected not only by 
    **string_max_size_bytes**
    parameter but also by
    **group_concat_max_len**
    .



*   **INSERT**
    function



**unicode_input_normalization**

**unicode_input_normalization**
is a parameter used to whether to input unicode stored in system level. The default value is
**no**

In gernal, unicode text can be stored in "fully composed" or "fully decomposed". When character 'Ä' has 00C4 (if it is encoded in UTF-8, it becomes 2 bytes of C3 84) which is only one code point. In "fully decomposed" mode, it has tow code points/characters. It is 0041 (character "A" and 0308(COMBINING DIAERESIS). In case of UTF-8 encoding, it becomes 3 bytes of 41 CC 88.

CUBRID can work with fully composed unicode. For clients which have fully decomposed texts, configure the value of
**unicode_input_normalization**
to yes so that it can be converted to fully composed mode; and then it can be reverted to fully decomposed mode. For normalization of unicode encapsulation of CUBRID, compatibility equivalence is not applied. In general, normalization of unicode is not possible to revert after composition, CUBRID supports revert for characters an many as possible, it applies normalization of unicode encapsulation. The characteristics of CUBRID normalization are as follows:

*   In case of language specific, normalization does not depend on locale. If one or more locale cana be used, this means every CAS/CSQL process, not CUBRID server. The
    **unicode_input_normalization**
    system parameter determines whether composition of input codes by normalization in system level. The
    **unicode_output_normalization**
    system parameter determines whether composition of output codes by normalization in system level.



*   Collation and normalization does not have direct relationship. Even though the value of
    **unicode_input_normalization**
    is no, the string of extensible collation (utf8_de_exp, utf8_jap_exp, utf8_km_exp) is properly sorted fully decomposed mode, it is not intended; it is side-effect of UCA(Unicode Collation Algorithm). The extensible collation is implemented only with fully composed texts.



*   In CUBRID, composition and decomposition for normalization does not work separately. It is generally used when
    **unicode_input_normalization**
    and  
    **unicode_output_normalization**
    are yes. In this case, codes entered from clients are stored in composed mode and and output in decomposed mode.



For details, see
`Administrator Guide > Globalization > Overview <#admin_admin_i18n_intro_htm>`_
.

**unicode_output_normalization**

**unicode_output_normalization**
is a parameter used to whether to output unicode stored in system level. The default value is
**no**
. For details, see the
**unicode_input_normalization**
description above.

**Query Cache-Related Parameters**

The following are parameters related to the query cache functionality that provides execution results cached for the same
**SELECT**
statement. The type and value range for each parameter are as follows:

+-------------------------------+----------+-------------------+---------+---------+
| **Parameter Name**            | **Type** | **Default Value** | **Min** | **Max** |
|                               |          |                   |         |         |
+-------------------------------+----------+-------------------+---------+---------+
| max_plan_cache_entries        | int      | 1,000             |         |         |
|                               |          |                   |         |         |
+-------------------------------+----------+-------------------+---------+---------+
| max_filter_pred_cache_entries | in       | 1,000             |         |         |
|                               |          |                   |         |         |
+-------------------------------+----------+-------------------+---------+---------+

**max_plan_cache_entries**

**max_plan_cache_entries**
is a parameter used to configure the maximum number of query plans to be cached in the memory. If the
**max_plan_cache_entries**
parameter is configured to -1 or 0, generated query plans are not stored in the memory cache; if it is configured to an integer value equal to or greater than 1, a specified number of query plans are cached in the memory.

The following example shows how to cache up to 1,000 queries.

max_plan_cache_entries=1000

**max_filter_pred_cache_entries**

**max_filter_pred_cache_entries**
is a parameter used to specify the maximum number of of filtered index expressions. The filtered index expressions are stored with them complied and can be immediately used in server. If it is not stored in cache, the process is required which filtered index expressions are fetched from database schema and interpreted.

**Utility-Related Parameters**

The following are parameters related to utilities used in CUBRID. The type and value range for each parameter are as follows:

+------------------------------+----------+-------------------+----------+----------+
| **Parameter Name**           | **Type** | **Default Value** | **Min.** | **Max.** |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| backup_volume_max_size_bytes | int      | -1                | 1024*32  |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| communication_histogram      | bool     | no                |          |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| compactdb_page_reclaim_only  | int      | 0                 |          |          |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+
| csql_history_num             | int      | 50                | 1        | 200      |
|                              |          |                   |          |          |
+------------------------------+----------+-------------------+----------+----------+

**backup_volume_max_size_bytes**

**backup_volume_max_size_bytes**
is a parameter used to configure the size of the backup volume file created by the
**cubrid backupdb**
utility in byte unit. If the parameter is configured to
**-1**
, which is the default value, the created backup volume is not partitioned; otherwise, the backup volume is partitioned as much as it is specified size.

**communication_histogram**

**communication_histogram**
is a parameter used to configure the
**cubrid statdump**
 utility. It is related to
`Session Commands <#csql_csql_sessioncommand_htm>`_
"
**;.h**
" of the CSQL Interpreter and the default value is
**no**
. For details, see
`Outputting Statistics Information of Server <#admin_admin_db_statdump_htm>`_
.

**compactdb_page_reclaim_only**

**compactdb_page_reclaim_only**
is a parameter used to configure the
**compactdb**
utility, which compacts the storage of already deleted objects to reuse OIDs of the already assigned storage. Storage optimization with the
**compactdb**
utility can be divided into three steps. The optimization steps can be selected through the
**compactdb_page_reclaim_only**
parameter. If the parameter is configured to
**0**
, which is the default value, step 1, 2 and 3 are all performed, so the storage is optimized in data, table and file units. If it is configured to 1, step 1 is skipped to have the storage optimized in table and file units. If it is configured to 2, steps 1 and 2 are skipped to have the storage optimized only in file units.

*   Step 1: Optimizes the storage only in data unit.



*   Step 2: Optimizes the storage in table unit.



*   Step 3: Optimizes the storage in file (heap file) unit.



**csql_history_num**

**csql_history_num**
is a parameter used to configure the CSQL Interpreter and the number of SQL statements to be stored in the history of the CSQL Interpreter. The default value is
**50**
.

**HA-Related Parameters**

The following are HA-related parameters. The type and value range for each parameter are as follows:

+--------------------+----------+-------------------+
| **Parameter Name** | **Type** | **Default Value** |
|                    |          |                   |
+--------------------+----------+-------------------+
| ha_mode            | string   | off               |
|                    |          |                   |
+--------------------+----------+-------------------+

**ha_mode**

The
**ha_mode**
parameter is used to set CUBRID HA, and the default value is
**off**
.

*   off : CUBRID HA is not used.



*   on : CUBRID HA is used using the configured node as a node for failover.



*   replica : CUBRID HA is used without using the configured node as a node for failover.



To use the CUBRID HA feature, you should set HA-related parameters in the
**cubrid_ha.conf**
file in addition to the
**ha_mode**
parameter. For details, see
`CUBRID HA <#admin_admin_ha_intro_htm>`_
.

**Other Parameters**

The following are other parameters. The type and value range for each parameter are as follows:

+--------------------------------+----------+-------------------+---------------+-------------------+
| **Parameter Name**             | **Type** | **Default Value** | **Min.**      | **Max.**          |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| access_ip_control              | bool     | no                |               |                   |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| access_ip_control_file         | string   |                   |               |                   |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| auto_restart_server            | bool     | yes               |               |                   |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| index_scan_in_oid_order        | bool     | no                |               |                   |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| index_unfill_factor            | float    | 0.05              | 0             | 0.5               |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| java_stored_procedure          | bool     | no                |               |                   |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| multi_range_optimization_limit | int      | 100               | 0             | 10000             |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| pthread_scope_process          | bool     | yes               |               |                   |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| server                         | string   |                   |               |                   |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| service                        | string   |                   |               |                   |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| session_state_timeout          | int      | 21600 (6 hours)   | 60 (1 minute) | 31536000 (1 year) |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+
| use_orderby_sort_limit         | bool     | yes               |               |                   |
|                                |          |                   |               |                   |
+--------------------------------+----------+-------------------+---------------+-------------------+

**access_ip_control**

**access_ip_control**
is a parameter used to configure whether to use feature limiting the IP addresses that allow server access. The default value is
**no**
. For details, see
`Limiting Database Server Access <#admin_admin_service_server_acces_3933>`_
.

**access_ip_control_file**

**access_ip_control_file**
is a parameter used to configure the file name in which the list of IP addresses allowed by servers is stored. If
**access_ip_control**
value is set to
**yes**
, database server allows the list of IP addresses only stored in the file specified by this parameter. For details, see 
`Limiting Database Server Access <#admin_admin_service_server_acces_3933>`_
.

**auto_restart_server**

**auto_restart_server**
is a parameter used to configure whether to restart the process when it stops due to fatal errors being occurred in database server process. If
**auto_restart_server**
value is set to
**yes**
, the server process automatically restarts when it has stopped due to errors; it does not restart in case it stops by following normal process (by using
**STOP**
command).

**index_scan_in_oid_order**

**index_scan_in_oid_order**
is a parameter used to configure the result data to be retrieved in OID order after the index scan. If the parameter is set to
**no**
, which is the default value, results are retrieved in data order; if it is set to
**yes**
, they are retrieved in OID order.

**index_unfill_factor**

If there is no free space because index pages are full when the
**INSERT**
or
**UPDATE**
operation is executed after the first index is created, the split of index page nodes occurs. This substantially affects the performance by increasing the operation time.
**index_unfill_factor**
is a parameter used to configure the percent of free space defined for each index page node when an index is created. The
**index_unfill_factor**
value is applied only when an index is created for the first time. The percent of free space defined for the page is not maintained dynamically. Its value ranges between 0 and 0.5. The default value is
**0.05**
.

If an index is created without any free space for the index page node (
**index_unfill_factor**
is set to 0), the split of index page nodes occurs every time an additional insertion is made. This may degrade the performance.

If the value of
**index_unfill_factor**
is large, a large amount of free space is available when an index is created. Therefore, better performance can be obtained because the split of index nodes does not occur for a relatively long period of time until the free space for the nodes is filled after the first index is created.

If this value is small, the amount of free space for the nodes is small when an index is created. Therefore, it is likely that the index nodes are spilt by
**INSERT**
or
**UPDATE**
because free space for the index nodes is filled in a short period of time.

**java_stored_procedure**

**java_stored_procedure**
is a parameter used to configure whether to use Java stored procedures by running the Java Virtual Machine (JVM). If the parameter is set to
**no**
, which is the default value, JVM is not executed; if it is set to
**yes**
, JVM is executed so you can use Java stored procedures. Therefore, configure the parameter to yes if you plan to use Java stored procedures.

**multi_range_optimization_limit**

If the number of rows specified by the
**LIMIT**
clause in the query, which has multiple ranges (col IN (?, ?, …,?)) and is available to use an index, is within the number specified in the
**multi_range_optimization_limit**
parameter, the optimization for the way of index sorting will be performed. The default value is
**100**
.

For example, if a value for this parameter is set to 50, LIMIT 10 means that it is within the value specified by this parameter, so that the values that meet the conditions will be sorted to produce the result. If LIMIT is 60, it means that it exceeds the parameter configuration value, so that it gets and sorts out all values that meet the conditions.

Depending on the setting value, the differences are made between collecting the result with on-the-fly sorting of the intermediate values and sorting the result values after collecting them, and the bigger value could make more unfavorable performance.

**pthread_scope_process**

**pthread_scope_process**
is a parameter used to configure the contention scope of threads. It only applies to AIX systems. If the parameter is set to
**no**
, the contention scope becomes
**PTHREAD_SCOPE_SYSTEM**
; if it is set to
**yes**
, it becomes
**PTHREAD_SCOPE_PROCESS**
. The default value is
**yes**
.

**server**

**server**
is a parameter used to register the name of database server process which will run automatically when CUBRID server starts.

**service**

**service**
is a parameter used to configure process that starts automatically when the CUBRID service starts. There are four types of processes:
**server**
,
**broker**
,
**manager**
, and
**heartbeat**
. Three processes are usually registered as in
**service=server,broker,manager**
.

*   If the parameter is set to
    **server**
    , the database process specified by the
    **@server**
    parameter gets started.



*   If the parameter is set to
    **broker**
    , the broker process gets started.



*   If the parameter is set to
    **manager**
    , the manager process gets started.



*   If the parameter is set to
    **heartbeat**
    , the HA-related processes get started.



**session_state_timeout**

**session_state_timeout**
is a parameter used to define how long the CUBRID session data will be kept. The session data will be deleted when the driver terminates the connection or the session time is expired. The session time will expire if a client does not have any requests until a value specified in
**session_state_timeout**
.

Custom variables defined by
**SET**
and
**PREPARE**
statements can be deleted by
**DROP**
/
**DEALLOCATE**
statements before session timeout.

The default value is
**21600**
seconds (6 hours).

**use_orderby_sort_limit**

**use_orderby_sort_limit**
is a parameter used to configure whether to keep the intermediate result of sorting and merging process in the statement including the
**ORDER BY … LIMIT**
*row_count*
clause as many as
*row_count*
. If it is set to
**yes**
, you can decrease unnecessary comparing and merging processes because as many as intermediate results will be kept as the value of
*row_count*
. The default value is
**yes**
.

**Broker Configuration**

**cubrid_broker.conf Configuration File and Default Parameters**

**Broker System Parameters**

The following table shows the broker parameters available in the broker configuration file (
**cubrid_broker.conf**
). For details, see
`Common Parameters <#pm_pm_broker_common_htm>`_
and
`Parameter by Broker <#pm_pm_broker_one_htm>`_
. You can temporarily change the parameter of which configuration values can be dynamically changed by using the
**broker_changer**
utility. To apply configuration values even after restarting all brokers with
**cubrid broker restart**
, you should change the values in the
**cubrid_broker.conf**
file.

+-------------------------------------------------+---------------------------------+----------+------------------------------+---------------------+
| **Category**                                    | **Parameter Name**              | **Type** | **Default Value**            | **Dynamic Changes** |
|                                                 |                                 |          |                              |                     |
+-------------------------------------------------+---------------------------------+----------+------------------------------+---------------------+
| `Common Parameters <#pm_pm_broker_common_htm>`_ | ACCESS_CONTROL                  | bool     | no                           |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | ACCESS_CONTROL_FILE             | string   |                              |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | ADMIN_LOG_FILE                  | string   | log/broker/cubrid_broker.log |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | MASTER_SHM_ID                   | int      | 30001                        |                     |
|                                                 |                                 |          |                              |                     |
+-------------------------------------------------+---------------------------------+----------+------------------------------+---------------------+
| `Parameter by Broker <#pm_pm_broker_one_htm>`_  | ACCESS_LIST                     | string   | -                            |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | ACCESS_LOG                      | string   | ON                           | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | ACCESS_MODE                     | string   | RW                           | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | APPL_SERVER                     | string   | CAS                          |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | APPL_SERVER_MAX_SIZE            | int      | 32-bit Windows: 40           | available           |
|                                                 |                                 |          | 64-bit Windows: 80           |                     |
|                                                 |                                 |          | Linux: 0                     |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | APPL_SERVER_MAX_SIZE_HARD_LIMIT | int      | 1024                         | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | APPL_SERVER_PORT                | int      | BROKER_PORT+1                |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | APPL_SERVER_SHM_ID              | int      | 30000                        |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | AUTO_ADD_APPL_SERVER            | string   | ON                           |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | BROKER_PORT                     | int      | 30000 (max.: 65535)          |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | CCI_DEFAULT_AUTOCOMMIT          | string   | ON                           |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | ERROR_LOG_DIR                   | string   | log/broker/error_log         |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | KEEP_CONNECTION                 | string   | AUTO                         | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | LOG_BACKUP                      | string   | OFF                          | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | LOG_DIR                         | string   | log/broker/sql_log           |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | LONG_QUERY_TIME                 | int      | 60                           | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | LONG_TRANSACTION_TIME           | int      | 60                           | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | MAX_NUM_APPL_SERVER             | int      | 40                           |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | MIN_NUM_APPL_SERVER             | int      | 5                            |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | MAX_PREPARED_STMT_COUNT         | int      | 2000 (min.: 1)               |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | MAX_QUERY_TIMEOUT               | int      | 0 (max.: 86400 (sec.))       | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | MAX_STRING_LENGTH               | int      | -1                           |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | PREFERRED_HOSTS                 | string   | -                            |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | SERVICE                         | string   | ON                           |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | SESSION_TIMEOUT                 | int      | 300                          |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | SLOW_LOG                        | string   | ON                           | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | SLOW_LOG_DIR                    | string   | log/broker/sql_log           |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | SOURCE_ENV                      | string   | cubrid.env                   |                     |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | SQL_LOG                         | string   | ON                           | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | SQL_LOG_MAX_SIZE                | int      | 100000                       | available           |
|                                                 |                                 |          |                              |                     |
|                                                 +---------------------------------+----------+------------------------------+---------------------+
|                                                 | STATEMENT_POOLING               | string   | ON                           | available           |
|                                                 |                                 |          |                              |                     |
+-------------------------------------------------+---------------------------------+----------+------------------------------+---------------------+
| TIME_TO_KILL                                    | int                             | 120      | available                    |                     |
|                                                 |                                 |          |                              |                     |
+-------------------------------------------------+---------------------------------+----------+------------------------------+---------------------+

**Default Parameters**

The
**cubrid_broker.conf**
file, the default broker configuration file created when installing CUBRID, includes some parameters that must be modified by default. If you want to modify the values of parameters that are not included in the configuration file by default, you can add or modify one yourself.

The following is the content of the
**cubrid_broker.conf**
file provided by default.

[broker]

MASTER_SHM_ID           =30001

ADMIN_LOG_FILE          =log/broker/cubrid_broker.log

 

[%query_editor]

SERVICE                 =ON

BROKER_PORT             =30000

MIN_NUM_APPL_SERVER     =5

MAX_NUM_APPL_SERVER     =40

APPL_SERVER_SHM_ID      =30000

LOG_DIR                 =log/broker/sql_log

ERROR_LOG_DIR           =log/broker/error_log

SQL_LOG                 =ON

TIME_TO_KILL            =120

SESSION_TIMEOUT         =300

KEEP_CONNECTION         =AUTO

 

[%BROKER1]

SERVICE                 =ON

BROKER_PORT             =33000

MIN_NUM_APPL_SERVER     =5

MAX_NUM_APPL_SERVER     =40

APPL_SERVER_SHM_ID      =33000

LOG_DIR                 =log/broker/sql_log

ERROR_LOG_DIR           =log/broker/error_log

SQL_LOG                 =ON

TIME_TO_KILL            =120

SESSION_TIMEOUT         =300

KEEP_CONNECTION         =AUTO

**Broker Configuration File Related Environment Variables**

You can specify the location of broker configuration file (
**cubrid_broker.conf**
) file by using the
**CUBRID_BROKER_CONF_FILE**
variable. The variable is used when executing several brokers with different configuration.

**Common Parameters**

The following are parameters commonly applied to entire brokers; it is written under [broker] section.

**ACCESS_CONTROL**

**ACCESS_CONTROL**
is a parameter used to limit applications which are trying to connect a broker. The default value is
**OFF**
. For details, see
`Broker Server Access Limitation <#admin_admin_service_broker_acces_9795>`_
.

**ACCESS_CONTROL_FILE**

**ACCESS_CONTROL_FILE**
is a parameter used to configure the name of a file in which a database name, database user ID, and the list of IPs are stored. For details, see
`Broker Server Access Limitation <#admin_admin_service_broker_acces_9795>`_
.

**ADMIN_LOG_FILE**

**ADMIN_LOG_FILE**
is a parameter used to configure the file in which time of running CUBRID broker is stored. The default value is a
**log/broker/cubrid_broker.log**
file.

**MASTER_SHM_ID**

**MASTER_SHM_ID**
is a parameter used to specify the identifier of shared memory which is used to manage the CUBRID broker. Its value must be unique in the system. The default value is
**30001**
.

**Parameter by Broker**

The following describes parameters to configure the environment variables of brokers; each parameter is located under [%
*broker_name*
].

**ACCESS_LIST**

**ACCESS_LIST**
is a parameter used to configure the name of a file where the list of IP addresses of an application which allows access to the CUBRID broker is stored. To allow access by IP addresses access 210.192.33.* and 210.194.34.*, store them to a file (ip_lists.txt) and then assign the file name with the value of this parameter.

**ACCESS_LOG**

**ACCESS_LOG**
is a parameter used to configure whether to store the access log of the broker. The default value is
**ON**
. The name of the access log file for the broker is
*broker_name_id*
.
**access**
and the file is stored under 
**$CUBRID/log/broker**
directory.

**ACCESS_MODE**

**ACCESS_MODE**
is a parameter used to configure default mode of the broker. The default value is
**RW**
. For details, see
`cubrid_broker.conf <#admin_admin_ha_conf_broker_htm>`_
of "Administrator's Guide".

**APPL_SERVER**

**APPL_SERVER**
is a parameter used to configure types of CAS generated and managed by the CUBRID broker. The default value is
**CAS**
.

**APPL_SERVER_MAX_SIZE**

**APPL_SERVER_MAX_SIZE**
is a parameter used to configure the maximum size of the process memory usage handled by CAS; the unit is MB.

Specifying this parameter makes transactions terminate (commit or rollback) only when it is executed by a user. In contrast to this, specifying
**APPL_SERVER_MAX_SIZE_HARD_LIMIT**
 makes transactions forcibly terminate (rollback) and restart CAS.

Note that the default values of Windows and Linux from each other.

For 32-bit Windows, the default value is
**40**
MB; for 64-bit Windows, it is
**80**
 MB. At the time when current process size exceeds the value of
**APPL_SERVER_MAX_SIZE**
, broker restarts the corresponding CAS.

For Linux, the default value of
**APPL_SERVER_MAX_SIZE**
is
**0**
; CAS restarts in the following conditions.

*   **APPL_SERVER_MAX_SIZE**
    is zero or negative: At the point when current process size becomes twice as large as initial memory



*   **APPL_SERVER_MAX_SIZE**
    is positive: At the point when it exceeds the value specified in
    **APPL_SERVER_MAX_SIZE**



**Note**
Be careful not to make the value too small because application severs may restart frequently and unexpectedly. In general, the value of
**APPL_SERVER_MAX_SIZE_HARD_LIMIT**
is greater than that of
**APPL_SERVER_MAX_SIZE**
. For details, see description of
**APPL_SERVER_MAX_SIZE_HARD_LIMIT**
.

**APPL_SERVER_MAX_SIZE_HARD_LIMIT**

**APPL_SERVER_MAX_SIZE_HARD_LIMIT**
is a parameter used to configure the maximum size of process memory usage handled by CAS; the unit is MB and default value is
**1024**
MB.

Specifying this parameter makes transactions being processed forcibly terminate (rollback) and restart CAS. In contrast to this, specifying
**APPL_SERVER_MAX_SIZE**
makes transactions terminate only when it is executed by a user. 

**Note**
Be careful not to make the value too small because application severs may restart frequently and unexpectedly.
When restarting CAS,
**APPL_SERVER_MAX_SIZE**
is specified to wait for normal termination of transactions although memory usage increases;
**APPL_SERVER_MAX_SIZE_HARD_LIMIT**
is specified to forcibly terminate transactions if memory usage exceeds the maximum value allowed. Therefore, in general, the value of
**APPL_SERVER_MAX_SIZE_HARD_LIMIT**
is greater than that of
**APPL_SERVER_MAX_SIZE**
.

**APPL_SERVER_PORT**

**APPL_SERVER_PORT**
is a parameter used to configure the connection port of CAS that communicates with application clients; it is used only in Windows. In Linux, the application clients and CAS use the UNIX domain socket for communication; therefore,
**APPL_SERVER_PORT**
is not used. The default value is determined by adding plus 1 to the
**BROKER_PORT**
parameter value. The number of ports used is the same as the number of CAS, starting from the specified port's number plus 1. For example, when the value of
**BROKER_PORT**
is 30,000 and the
**APPL_SERVER_PORT**
parameter value has been configured, and if the
**MIN_NUM_APPL_SERVER**
value is 5, five CASs uses the ports numbering between 30,001 and 30,005, respectively. The maximum number of CAS specified in the
**MAX_NUM_APPL_SERVER**
parameter in
**cubrid_broker_conf**
; therefore, the maximum number of connection ports is also determined by the value of
**MAX_NUM_APPL_SERVER**
parameter.

On the Windows system, if firewall exists between an application and the CUBRID broker, the communication port specified in
**BROKER_PORT**
and
**APPL_SERVER_PORT**
must be open.

**Note**
For the
**CUBRID_TMP**
environment variable that specifies the UNIX domain socket file path of
**cub_master**
and
**cub_broker**
processes, see
`Configuring the Environment Variable <#gs_gs_must_envar_htm>`_
.

**APPL_SERVER_SHM_ID**

**APPL_SERVER_SHM_ID**
is a parameter used to configure the ID of shared memory used by CAS; the value must be unique within system. The default value is the same as the port value of the broker.

**AUTO_ADD_APPL_SERVER**

**AUTO_ADD_APPL_SERVER**
is a parameter used to configure whether CAS increase automatically to the value specified in
**MAX_NUM_APPL_SERVER**
in case of needed; the value will be either
**ON**
or
**OFF**
(default:
**ON**
).  

**BROKER_PORT**

**BROKER_PORT**
is a parameter used to configure the port number of the broker; the value must be unique and smaller than 65,535. The default port value of
**query_editor**
' broker is
**30,000**
and the port value of the
**broker1**
is
**33,000**
.

**CCI_DEFAULT_AUTOCOMMIT**

**CCI_DEFAULT_AUTOCOMMIT**
is a parameter used to configure whether to make application implemented in CCI interface or CCI-based interface such as PHP, ODBC, OLE DB, Perl, Python, and Ruby commit automatically. The default value is
**ON**
. This parameter does not affect applications implemented in JDBC. In case of using ODBC, malfunction can occur if this parameter is
**ON**
; you must set it to
**OFF**
, in this case.

If the
**CCI_DEFAULT_AUTOCOMMIT**
parameter value is
**OFF**
, the broker application server (CAS) process is occupied until the transaction is terminated. Therefore, it is recommended to execute commit after completing fetch when executing the 
**SELECT**
statement.

**Note**
The
**CCI_DEFAULT_AUTOCOMMIT**
parameter has been supported from 2008 R4.0, and the default value is
**OFF**
for the version. Therefore, if you use CUBRID 2008 R4.1 or later versions and want to keep the configuration
**OFF**
, you should manually change it to
**OFF**
to avoid auto-commit of unexpected transaction.

**ERROR_LOG_DIR**

**ERROR_LOG_DIR**
is a parameter used to configure default directory in which error logs about broker is stored. The default value is
**log/broker/error_log**
. The log file name for the broker error is
*broker_ name_id*
.
**err**
.

**KEEP_CONNECTION**

**KEEP_CONNECTION**
is a parameter used to configure the way of connection between CAS and application clients; it is set to one of the followings:
**ON**
,
**OFF**
or
**AUTO**
. If this value is
**OFF**
, clients are connected to servers in transaction unit; for
**ON**
, it is connected in connection unit. If it is
**AUTO**
and the number of servers is more than that of clients, transaction unit is used; in the reverse case, connection unit is used. The default value is
**AUTO**
.

**LOG_BACKUP**

**LOG_BACKUP**
is a parameter used to configure whether to back up access and error log files of the broker when CUBRID stops. The default value is set to 
**OFF**
. An access log file (
*broker_name*
.
**access**
) in the
**$CUBRID/log/broker**
directory is deleted when CUBRID stops. If the value is set to 
**ON**
, an access log file is stored (backed up) as
*broker_name*
.
**access**
.
*yyyymmdd.hhmm*
when CUBRID stops.

**LOG_DIR**

**LOG_DIR**
is a parameter used to configure the directory where SQL logs are stored. The default value is
**log/broker/sql_log**
. The file name of the SQL logs is
*broker_name_id*
.
**sql.log**
.

**LONG_QUERY_TIME**

**LONG_QUERY_TIME**
is a parameter used to configure execution time of query which is evaluated as long-duration query. The default value is
**60**
(seconds) and can be value in msec. with a decimal separator.
** **
For example, the value should be configured into 0.5 to configure 500 msec. Note that a parameter value is configured to 0, it is not evaluated as a long-duration query.

**LONG_TRANSACTION_TIME**

**LONG_TRANSACTION_TIME**
is a parameter used to configure execution time of query which is evaluated as long-duration transaction. The default value is
**60**
(seconds) and can be value in msec. with a decimal separator.
** **
For example, the value should be configured into 0.5 to configure 500 msec. Note that a parameter is configured to 0, it is not evaluated as a long-duration transaction.

**MAX_NUM_APPL_SERVER**

**MAX_NUM_APPL_SERVER**
is a parameter used to configure the maximum number of simultaneous connections allowed. The default value is
**40**
.

In the environment where connection pool is maintained by using middleware such as DBCP or WAS, the value of
**MAX_NUM_APPL_SERVER**
parameter and the number of connection pools should be same.

**MIN_NUM_APPL_SERVER**

**MIN_NUM_APPL_SERVER**
is a parameter used to configure the minimum number of CAS even if any request to connect the broker has not been made. The default value is
**5**
.

**MAX_PREPARED_STMT_COUNT**

**MAX_PREPARED_STMT_COUNT**
is a parameter used to limit the number of prepared statements by user (application) access. The default value is
**2,000**
and the minimum value is 1. The problem in which prepared statement exceeding allowed memory is mistakenly generated by system can be prohibited by making users specify the parameter value.

**MAX_QUERY_TIMEOUT**

**MAX_ QUERY_TIMEOUT**
is a parameter used to configure timeout value of query execution. When time exceeds a value specified in this parameter after starting query execution, the query being executed stops and rolls back.

The default value is
**0**
(seconds) and it means infinite wait. The value range is available from 8 to 86,400 seconds (one day). The smallest value (except 0) between the
**MAX_QUERY_TIMEOUT**
value and query timeout value of an application is applied if query timeout is configured in an application.

**Note**
See the
**cci_connect_with_url**
and
**cci_set_query_timeout**
functions to configure query timeout of CCI applications. For configuring query timeout of JDBC applications, see the
**setQueryTimeout**
method.

**MAX_STRING_LENGTH**

**MAX_STRING_LENGTH**
is a parameter used to configure the maximum string length for bit, varbit, char, varchar, nchar, nchar varying data types. If the value is
**-1**
, which is the default value, the length defined in the database is used. If the value is
**100**
, the value acts like 100 being applied even when a certain attribute is defined as varchar(1000).

**PREFERRED_HOSTS**

**PREFERRED_HOSTS**
is a parameter that must be configured if the broker mode is set to PHRO. The default value is
**NULL**
. FOR details, see
`cubrid_broker.conf <#admin_admin_ha_conf_broker_htm>`_
of "Administrator's Guide."

**SERVICE**

**SERVICE**
is a parameter used to configure whether to run the broker. It can be either
**ON**
or
**OFF**
. The default value is
**ON**
. The broker can run only when this value is configured to
**ON**
.

**SESSION_TIMEOUT**

**SESSION_TIMEOUT**
is a parameter used to configure timeout value for the session of the broker. If there is no response to the job request for the specified time period, session will be terminated. If a value exceeds the value specified in this parameter without any action taken after starting transaction, the connections are terminated. The default value is
**300**
(seconds).

**SLOW_LOG**

**SLOW_LOG**
is a parameter used to configure whether to log. The default value is
**ON**
. If the value is
**ON**
, long transaction query which exceeds the time specified in
**LONG_QUERY_TIME**
or query where an error occurred is stored in the
**SLOW SQL**
log file. The name of file created is
*broker_name_id*
.
**slow.log**
and it is located under
**SLOW_LOG_DIR**
.

**SLOW_LOG_DIR**

**SLOW_LOG_DIR**
is a parameter used to configure the location of directory where the log file is generated. The default value is
**log/broker/sql_log**
.

**SOURCE_ENV**

**SOURCE_ENV**
is a parameter used to determine the file where the operating system variable for each broker is configured. The extension of the file must be
**env**
. All parameters specified in
**cubrid.conf**
can also be configured by environment variables. For example, the
**lock_timeout_in_secs**
parameter in
**cubrid.conf**
can also be configured by the
**CUBRID_LOCK_TIMEOUT_IN_SECS**
environment variable. As another example, to block execution of DDL statements on broker1, you can configure
**CUBRID_BLOCK_DDL_STATEMENT**
to 1 in the file specified by
**SOURCE_ENV**
.

An environment variable, if exists, has priority over
**cubrid.conf**
. The default value is
**cubrid.env**
.

**SQL_LOG**

**SQL_LOG**
is a parameter used to configure whether to leave logs for SQL statements processed by CAS when CAS handles requests from a client. The default value is
**ON**
. When this parameter is configured to
**ON**
, all logs are stored. The log file name becomes
*broker_name_id.*
**sql**
.
**log**
. The file is created in the
**log/broker/sql_log**
directory under the installation directory. The parameter values are as follows:

*   **OFF**
    : Does not leave any logs.



*   **ERROR**
    : Stores logs for queries which occur an error. only queries where an error occurs.



*   **NOTICE**
    : Stores logs for the long-duration execution queries which exceeds the configured time/transaction, or leaves logs for queries which occur an error.



*   **TIMEOUT**
    : Stores logs for the long-duration execution queries which exceeds the configured time/transaction.



*   **ON**
    /
    **ALL**
    : Stores all logs.



**SQL_LOG_MAX_SIZE**

**SQL_LOG_MAX_SIZE**
is a parameter used to configure the maximum size of the SQL log file. The default value is
**100,000**
(KB). If the size of the SQL log file, which is created when the
**SQL_LOG**
parameter is configured to
**ON**
, reaches the value configured by the parameter,
*broker_name_id*
.
**sql.log.bak**
is created.

**STATEMENT_POOLING**

**STATEMENT_POOLING**
is a parameter used to configure whether to use statement pool feature. The default value is
**ON**
.

CUBRID closes all handles of prepared statement in the corresponding client sessions when transaction commit or rollback is made. If the value of
**STATEMENT_POOLING**
is set to
**ON**
, the handles are reusable because they are maintained in the pool. Therefore, in an environment where libraries, such as general applications reusing prepared statement or DBCP where statement pooling is implemented, are applied, the default configuration (
**ON**
) should be maintained.

If the prepared statement is executed after transaction commit or termination while
**STATEMENT_POOLING**
is set to
**OFF**
, the following message will be displayed.

Caused by: cubrid.jdbc.driver.CUBRIDException: Attempt to access a closed Statement.

**TIME_TO_KILL**

**TIME_TO_KILL**
is a parameter used to configure the time to remove CAS in idle state among CAS added dynamically. The default value is
**120**
(seconds). An idle state is one in which the server is not involved in any jobs. If this state continues exceeding the value specified in
**TIME_TO_KILL**
, CAS is removed.

The value configured in this parameter affects only CAS added dynamically, so it applies only when the
**AUTO_ADD_APPL_SERVER**
parameter is configured to
**ON**
. Note that times to add or remove CAS will be increased more if the
**TIME_TO_KILL**
value is so small.

