:meta-keywords: cubrid pl introduction, cubrid pl system
:meta-description: This chapter describes an introduction to CUBRID PL system

*****************************
Overview
*****************************

Stored procedures and stored functions help handle complex business logic that is difficult to implement with SQL alone and make data manipulation easier to perform.
They offer the following major advantages:

* Productivity and Usability: Once created, stored procedures and stored functions can be reused repeatedly. Users can call them from SQL, and also invoke them easily from applications using JDBC.
* Performance: Since stored procedures and functions run on the database server, they reduce network traffic and enhance the performance of the database server.
* Security: Stored procedures can be granted execution permissions to specific users, enabling fine-grained control over data access and modification.
* Interoperability and Portability: Stored procedures and functions are designed to work across various languages and execution environments, maximizing the usability of the database.

CUBRID supports the following two procedural languages for stored procedures/functions:

    * PL/CSQL
    * Java

Creating a Stored Procedure
===========================

.. _pl-supported_sql_type:

Supported Argument and Return Data Types
----------------------------------------

Only a subset of the data types supported by CUBRID SQL can be specified for stored procedure/function arguments and return values.
The following table shows the data types supported by language extensions:

+----------------+-------------------------------------+----------+--------------+
|                |                                     | Support Status (O, X)   |
+ Type           + Data Type                           +----------+--------------+
|                |                                     | PL/CSQL  | Java SP      |
+================+=====================================+==========+==============+
| Numeric        | SHORT, SMALLINT                     | O        | O            |
+                +-------------------------------------+----------+--------------+
|                | INTEGER, INT                        | O        | O            |
+                +-------------------------------------+----------+--------------+
|                | BIGINT                              | O        | O            |
+                +-------------------------------------+----------+--------------+
|                | NUMERIC, DECIMAL                    | O        | O            |
+                +-------------------------------------+----------+--------------+
|                | FLOAT, REAL                         | O        | O            |
+                +-------------------------------------+----------+--------------+
|                | DOUBLE, DOUBLE PRECISION            | O        | O            |
+----------------+-------------------------------------+----------+--------------+
| Date/Time      | DATE, TIME, TIMESTAMP, DATETIME     | O        | O            |
+                +-------------------------------------+----------+--------------+
|                | TIMESTAMPLTZ, TIMESTAMPTZ           | X        | X            |
|                | DATETIMELTZ, DATETIMETZ             |          |              |
+----------------+-------------------------------------+----------+--------------+
| String         | CHAR, VARCHAR, STRING, CHAR VARYING | O        | O            |
+----------------+-------------------------------------+----------+--------------+
| Collection     | SET, MULTISET, LIST, SEQUENCE       | X        | O            |
+----------------+-------------------------------------+----------+--------------+
| Other          | BIT, BIT VARYING                    | X        | X            |
+                +-------------------------------------+----------+--------------+
|                | ENUM                                | X        | X            |
+                +-------------------------------------+----------+--------------+
|                | BLOB/CLOB                           | X        | X            |
+                +-------------------------------------+----------+--------------+
|                | JSON                                | X        | X            |
+                +-------------------------------------+----------+--------------+
|                | CURSOR                              | X        | O*           |
+----------------+-------------------------------------+----------+--------------+

\* In Java SP, only return types are supported for CURSOR, not argument types.

If an unsupported data type is used when creating a stored procedure, the following errors occur:

.. code-block:: sql

        CREATE FUNCTION unsupported_json() RETURN JSON
        AS BEGIN RETURN NULL; END;

        CREATE PROCEDURE unsupported_args (arg TIMESTAMPLTZ)
        AS BEGIN NULL; END;

::

        ERROR: Unsupported return type 'json' of the stored procedure

        ERROR: before ' )
        AS BEGIN NULL; END; '
        Unsupported argument type 'timestampltz' of the stored procedure

