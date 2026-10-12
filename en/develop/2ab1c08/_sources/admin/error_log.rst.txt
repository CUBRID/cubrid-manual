:meta-keywords: cubrid.msg, "$set 5 MSGCAT_SET_ERROR" log info
:meta-description: This section contains almost all runtime errors in CUBRID and consists of messages that inform users of various error situations that may occur during database operations.

.. _error_log:

Database Error Logs
===================

**Overview of CUBRID Error Messages**

CUBRID error messages are provided in both Korean and English in the `$CUBRID/msg` directory. From the perspective of a CUBRID user (DBA), these error messages constitute a core error catalog that systematically classifies major error situations that the CUBRID database may encounter during runtime, such as system issues, transaction/lock problems, log/backup issues, DDL/DML operations, indexes, constraints, authorization, SQL execution, and HA/replication.

The composition of CUBRID error message files and user guidance messages is as follows.

* cubrid.msg: Runtime errors and system messages of the CUBRID database engine
* csql.msg: Usage instructions, commands, and error messages of the CSQL (CUBRID SQL Interpreter) client tool
* utils.msg: Messages and usage instructions of CUBRID administrative utility tools


**CUBRID Data Model Terminology**

CUBRID was developed as an object-relational database (ORDB) and partially supports object concepts such as class (table) inheritance. Because of this, object-based terminology may be used in error messages or internal concepts.
Therefore, it is necessary to understand traditional relational database (RDB) terminology together with object-relational database (ORDB) terminology, as shown below.

+--------------------------+---------------------------+
| Relational (RDB) Term    | Object-Relational (ORDB)  |
+==========================+===========================+
| Table                    | Class                     |
+--------------------------+---------------------------+
| Column                   | Attribute                 |
+--------------------------+---------------------------+
| Record                   | Instance                  |
+--------------------------+---------------------------+
| Data Type                | Domain                    |
+--------------------------+---------------------------+


**Classification of CUBRID Error Messages**

Major database error messages are classified into six categories: system management, SQL queries, database administration, database files, transactions, and CUBRID HA. For each error code and message, descriptions and service impact are organized together.


.. toctree::
    :maxdepth: 4

    error_log_system
    error_log_sql
    error_log_admin
    error_log_volume
    error_log_transaction
    error_log_ha
