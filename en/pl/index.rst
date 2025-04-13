:meta-keywords: cubrid pl, cubrid procedural language, cubrid server programming, cubrid pl/csql, cubrid jsp, cubrid method
:meta-description: CUBRID supports to develop stored functions and procedures. This chapter describes an introduction to CUBRID PL system

.. _sql_procedural_langauge:

***************
CUBRID PL
***************

This chapter introduces the procedural language extension features provided by CUBRID to develop stored procedures and functions.

CUBRID supports stored procedures and stored functions as procedural language extension features.
Stored procedures and stored functions help handle complex business logic that is difficult to implement with SQL alone and make data manipulation easier to perform.

They offer the following major advantages:

* Productivity and Usability: Once created, stored procedures and stored functions can be reused repeatedly. Users can call them from SQL and also invoke them easily from applications using JDBC.
* Performance: Since stored procedures and functions run on the database server, they reduce network traffic and enhance the performance of the database server.
* Security: Stored procedures can be granted execution permissions to specific users, enabling fine-grained control over data access and modification.
* Interoperability and Portability: Stored procedures and functions are designed to work across various languages and execution environments, maximizing the usability of the database.

CUBRID supports the following two procedural languages for stored procedures/functions:

    * PL/CSQL
    * Java

The main contents described in this chapter are as follows:

*   Create a Stored Procedure: Describes the creation of stored procedures and stored functions.

*   Call a Stored Procedure: Describes how to call stored procedures and stored functions.

*   Transaction Commit and Rollback Support: Describes the use of transaction commit and rollback within stored procedures.

*   Performance Tuning: Describes methods to achieve optimized performance when using stored procedures.

*   System Package: Describes the system packages supported by CUBRID.

*   PL/CSQL: Describes the overview and syntax of PL/CSQL, an extension of CUBRID's SQL procedural language.

*   Java Stored Procedure: Describes how to develop stored procedures using the Java language.

*   Method: Describes the methods, which are built-in functions of CUBRID.

.. toctree::
    :maxdepth: 2

    pl_create
    pl_call
    pl_tuning
    pl_package
    plcsql
    jsp
    method
