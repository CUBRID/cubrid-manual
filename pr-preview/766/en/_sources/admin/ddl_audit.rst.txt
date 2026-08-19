
:meta-keywords: DDL Audit, log
:meta-description: CUBRID has the capability of recording DDL (Data Definition Language) that changes the database system configuration such as create/delete/modify tables as well as changing the access privilege of a table.

.. _ddl-audit:

***************
DDL Audit Log
***************

Overview
==========

CUBRID has the capability of recording DDL (Data Definition Language) that changes the database system configuration such as create/delete/modify tables as well as changing the access privilege of a table.
DDLs issued through CAS, csql, and loaddb could be recorded in log files in addition to the copy of the files executed if required.

The DDL Audit log will be created in the $CUBRID/log/ddl_audit directory when ddl_audit_log of the system parameter is turned on. Note that the size of each log file cannot exceed the value specified in the ddl_audit_log_size parameter. For parameters related to DDL audit, refer to system parameters of CUBRID Managment :doc:`/admin/config` .

.. note::

    If you use a method from the :ref:`catalog` , the DDL Audit Log will not be created.

DDL Audit Log file name convention
======================================

* cas: {broker_name}_{app_server_num}_ddl.log
* csql interactive: csql_{db_name}_ddl.log
* loaddb: loaddb_{db_name}_ddl.log

**Additional file name convention:**

* csql from file: csql/{csql_file}_{YYYYMMDD}_{HHMMSS}_{pid}
* loaddb: loaddb/{loaddb_file}_{YYYYMMDD}_{HHMMSS}_{pid}

DDL Audit Logfile format of CAS
======================================

* [Time] [ip_addr]|[db_name]|[user_name]|[result]|[elapsed time]|[auto commit/rollback]|[sql_text]

	Description:
	
	* [Time]: Time starting execution of the DDL (e.g. 20-12-18 12:08:32.327)
	* [db_name]: Database name where DDL was executed
	* [ip_addr]: An IP address of an application client (e.g. 172.31.0.70)
	* [user_name]: the database user name who issued DDL
	* [result]: statement execution result. OK if successful, otherwise error code (e.g. ERROR:-494)
	* [elapsed time]: Elapsed time of statement execution
	* [auto commit/rollback]: Automatically committed or rolled back, with the error code of it
	* [sql_text]: executed DDL text

DDL Audit Logfile format of CSQL
======================================

* [Time] [pid]|[user_name]|[result]|[elapsed time]|[auto commit/rollback]|[sql_text]

	Description:
	
	* [Time]: Time starting execution of the DDL (e.g. 20-11-20 13:26:51.765)
	* [pid]: csql process id
	* [user_name]: the database user name who issued DDL
	* [result]: statement execution result. OK if successful, otherwise error code  (e.g. ERROR:-272)
	* [elapsed time]: Elapsed time of statement execution
	* [auto commit/rollback]: Automatically committed or rolled back, with the error code
	* [sql_text]: executed DDL text or executed csql filename

DDL Audit Log format of LOADDB
======================================

* [Time] [pid]|[user_name]|[result]|[log contents]|[file name]

	Description:
	
	* [Time]: Time starting execution of the DDL (e.g. 20-12-18 12:08:32.327)
	* [pid]: loaddb process id
	* [user_name]: the database user name who issued DDL
	* [result]: Result of loaddb execution, OK if successful, otherwise error code (e.g. ERROR:-494)
	* [log contents]: The number of total statements, or the number of commit in case of error and error line
	* [file name]: Copy the file loaded from loaddb. At this time, it is the name of the copied file
