
:meta-keywords: cubrid introduction, cubrid glossary, cubrid manual conventions, cubrid versions
:meta-description: The contents of the CUBRID Database Management System (CUBRID DBMS) product manual, Glossary, Manual Conventions, Version Name and Version String Conventions.

======================
Introduction to Manual
======================

Manual Contents
---------------

The contents of the CUBRID Database Management System (CUBRID DBMS) product manual are as follows:

*   :doc:`intro`: This chapter provides a description of the structure and characteristics of the CUBRID DBMS.

*   :doc:`start`: The "Getting Started with CUBRID" provides users with a brief explanation on what to do when first starting CUBRID. The chapter contains information on how to install and execute the system, used ports on accessing to CUBRID and provides simple explanations on the CUBRID query tools. 

*   :doc:`csql`: CSQL is an application that allows you to use SQL statements through a command-driven interface. This chapter explains how to use the CSQL Interpreter and associated commands.

*   :doc:`sql/index`: This chapter describes SQL syntaxes such as data types, functions and operators, data retrieval or table manipulation. The chapter also provides SQL syntaxes used for indexes, triggers, partitioning, serial and user information changes, etc.

*   :doc:`admin/index`: This chapter provides instructions on how to create, drop, back up, restore and migrate a database, configuring globalization, and executing CUBRID HA. Also it includes instructions on how to use the  **cubrid** utility, which starts and stops the server, broker, and CUBRID Manager server, etc. Also, this chapter provides instructions on setting system parameters that may influence the performance. It provides information on how to use the configuration file for the server and broker, and describes the meaning of each parameter.

*   :doc:`security`: This chapter describes the CUBRID security features such as packet encryption, ACL(Access Control List), authorization, and TDE(Transparent Data Encryption).

*   :doc:`api/index`: The "Performance Tuning" chapter provides instructions on setting system parameters that may influence the performance. This chapter provides information on how to use the configuration file for the server and broker, and describes the meaning of each parameter.

*   :doc:`release_note/index`: This chapter provides a description of additions, changes, improvements and bug fixes.

Glossary
--------

CUBRID is an object-relational database management system (ORDBMS), which supports object-oriented concepts such as inheritance. In this manual, relational database terminologies are also used along with object-oriented terminologies for better understanding. Object-oriented terminologies such as class, instance and attribute is used to describe concepts including inheritance, and relational database terminologies are mainly used to describe common SQL syntax.

+-------------------------+-------------------+
| Relational Database     | CUBRID            |
+=========================+===================+
| table                   | class, table      |
+-------------------------+-------------------+
| column                  | attribute, column |
+-------------------------+-------------------+
| record                  | instance, record  |
+-------------------------+-------------------+
| data type               | domain, data type |
+-------------------------+-------------------+

Manual Conventions
------------------

The following table provides conventions on definitions used in the CUBRID Database Management System product manual to identify "statements," "commands" and "reference within texts."

+--------------------+---------------------------------------------------------+----------------------+
| Convention         | Description                                             | Example              |
|                    |                                                         |                      |
+====================+=========================================================+======================+
| *Italics*          | *Italics*                                               | *persistent*         |
|                    | type represents variable names and user-defined values  | :                    |
|                    | (system, database, table, column and file) in examples. | *stringVariableName* |
+--------------------+---------------------------------------------------------+----------------------+
| **Boldface**       | **Boldface** type represents names such as the member   | **fetch**            |
|                    | function name, class name, constants, CUBRID keyword    | ( ) member function  |
|                    | or names such as other required characters.             |                      |
+--------------------+---------------------------------------------------------+----------------------+
| Constant Width     | Constant Width type represents segments of code         | csql database_name   |
|                    | example or describes a command's execution and results. |                      |
+--------------------+---------------------------------------------------------+----------------------+
| UPPER-CASE         | UPPER-CASE represents the CUBRID keyword                | **SELECT**           |
|                    | (see **Boldface**).                                     |                      |
+--------------------+---------------------------------------------------------+----------------------+
| Single Quotes      | Single quotes (' ') are used with braces and brackets   | {'{'                 |
| (' ')              | and represent the necessary sections of a syntax.       | *const_list*         |
|                    | Single quotes are also used to enclose strings.         | '}'}                 |
+--------------------+---------------------------------------------------------+----------------------+
| Brackets           | Brackets ([ ]) represents optional parameters or        | [                    |
| ([ ])              | keywords.                                               | **ONLY**             |
|                    |                                                         | ]                    |
+--------------------+---------------------------------------------------------+----------------------+
| Vertical bar       | Vertical bar (|) represents that one or another         | [                    |
| ( | )              | option can be specified.                                | **COLUMN**           |
|                    |                                                         | |                    |
|                    |                                                         | **ATTRIBUTE**        |
|                    |                                                         | ]                    |
+--------------------+---------------------------------------------------------+----------------------+
| A parameter        | A parameter enclosed by braces represents that one      | **CREATE**           |
| enclosed           | of those parameters must be specified in a statement    | {                    |
| by braces ({ })    | syntax.                                                 | **TABLE**            |
|                    |                                                         | |                    |
|                    |                                                         | **CLASS**            |
|                    |                                                         | }                    |
+--------------------+---------------------------------------------------------+----------------------+
| A value enclosed   | A value enclosed by braces an element consisting of     | {2, 4, 6}            |
| by braces ({ })    | collection.                                             |                      |
+--------------------+---------------------------------------------------------+----------------------+
| Braces with        | Braces before an ellipsis represents that a parameter   | {,                   |
| ellipsis ({ }...)  | can be repeated.                                        | *class_name*         |
|                    |                                                         | }...                 |
+--------------------+---------------------------------------------------------+----------------------+
| Angle brackets     | Angle brackets represent a single key or a series of    | <Ctrl+n>             |
| (< >)              | key strokes.                                            | }...                 |
+--------------------+---------------------------------------------------------+----------------------+

Version Name and Version String Conventions
-------------------------------------------

Rules for version naming and string since CUBRID 10.0 are as follows:

*  Version name: CUBRID M.m Patch p (Major version, Minor version, Patch version if necessary)
   CUBRID 10.1 Patch 1 (CUBRID 10.1 P1 in short)

*  Version string: M.m.p.build_number (Major version, Minor version, Patch version, Build number)
   10.2.0.8787-a31ea42

   Build number consists of two parts which are separated by a hyphen. The former is the number of changes from the base revision, which monotonically increases. The later is the SHA-1 hash of the build built.
   
Rules for version naming and string since CUBRID 9.0 are as follows:

*  Version name: CUBRID M.m Patch p (Major version, Minor version, Patch version if necessary)
   CUBRID 9.2 Patch 1 (CUBRID 9.2 P1 in short)

*  Version string: M.m.p.build_number (Major version, Minor version, Patch version, Build number)
   9.2.1.0012
   
Rules for version naming and string before CUBRID 9.0 are as follows:

*  Version name: CUBRID 2008 RM.m Patch p (2008 for Major version, Minor version, Patch version, Build number)
   CUBRID 2008 R4.1 Patch 1
   
*  Version string: 8.m.p.build_number (Major version, Minor version, Patch version, Build number)
   8.4.1.1001
