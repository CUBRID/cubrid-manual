
:meta-keywords: cubrid sql, database sql, cubrid statements
:meta-description: This chapter describes SQL syntax such as data types, functions and operators, data retrieval or table manipulation. You can also find SQL statements used for index, trigger, partition, serial and changing user information.

**********
CUBRID SQL
**********

This chapter describes SQL syntax such as data types, functions and operators, data retrieval or table manipulation. You can also find SQL statements used for index, trigger, partition, serial and changing user information.

The main topics covered in this chapter are as follows:

*   Writing Rules

    *   Identifier: Describes how to write, the identifier, string allowed to be used as a name of a table, index, and column.
    *   Reserved words: Lists reserved words in CUBRID. To use a reserved word as an identifier, enclose the identifier by using double quotes, backticks (`), or brackets ([]).
    *   Comment
    *   Literal: Describes how to write constant values.
    
*   Data types: Describes the data types, the format to store data.

*   Data Definition Statements: Describes how to create, alter, drop, and rename a table, an index, a view and a serial.

*   Operators and Functions: Describes the operators and functions used for query statements.

*   Data Manipulation Statements: Describes the SELECT, INSERT, UPDATE, and DELETE statements.

*   Query Optimization: Describes the query optimization by using the index, hint, and the index hint syntax.

*   Partitioning: Describes how to partition one table into several independent logical units.

*   Trigger: Describes how to create, alter, drop, and rename a trigger that is automatically executed in response to certain events.

*   Java Stored Functions/Procedures: Describes how to create a Java method and call it in the query statement.

*   Method: Describes the method, a built-in function of the CUBRID database system.

*   Class Inheritance: Describes how to inherit the attribute from the parent to the child table (class).

*   Database Administration: Describes about user management, SET and SHOW statements.

*   User Schema: Describes a logical collection of objects in a database that is created with users.

*   System Catalog: Describes the CUBRID system catalog, the internal information of the CUBRID database.

.. toctree::
    :maxdepth: 3
    
    syntax.rst
    datatype_index.rst
    schema/index.rst
    function/index.rst
    query/index.rst
    tuning_index.rst
    partition_index.rst
    i18n_index.rst
    transaction_index.rst
    trigger.rst
    jsp.rst
    dblink.rst
    method.rst
    oodb.rst
    db_admin.rst
    user_schema.rst
    catalog.rst
    
