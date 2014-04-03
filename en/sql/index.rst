****************
CUBRID SQL Guide
****************

This chapter describes SQL syntax such as data types, functions and operators, data retrieval or table manipulation. You can also find SQL statements used for index, trigger, partition, serial and changing user information.

The main topics covered in this chapter are as follows:

*   Identifier: Describes how to write, the identifier, string allowed to be used as a name of a table, index, and column.
*   Reserved words: Lists reserved words in CUBRID. To use a reserved word as an identifier, enclose the identifier by using double quotes, backticks (`), or brackets ([]).
*   Comment
*   Data types: Describes the data types, the format to store data.
*   Tables: Describes how to create, alter, drop, and rename a table.
*   Index: Describes how to create, alter, and drop an index.
*   VIEW: Describes how to create, alter, drop, and rename VIEW, a virtual table.
*   SERIAL: Describes how to create, alter, and drop serial numbers managed by a database.
*   Operators and functions: Describes the operators and functions used for query statements.
*   Data retrieval and manipulation: Describes the SELECT, INSERT, UPDATE, and DELETE statements.
*   Query optimization: Describes the query optimization by using the index, hint, and the index hint syntax.
*   Partitioning: Describes how to partition one table into several independent logical units.
*   Trigger: Describes how to create, alter, drop, and rename a trigger that is automatically executed in response to certain events.
*   Java stored functions/procedures: Describes how to create a Java method and call it in the query statement.
*   Method: Describes the method, a built-in function of the CUBRID database system.
*   Class inheritance: Describes how to inherit the attribute from the parent to the child table (class).
*   Class conflict resolution: Describes how to resolve table (class) conflicts between attributes or methods related through inheritance.
*   CUBRID system catalog: Describes the CUBRID system catalog, the internal information of the CUBRID database.

.. toctree::
    :maxdepth: 3

    identifier.rst
    keyword.rst
    comment.rst
    datatype.rst
    schema/table.rst
    schema/index.rst
    schema/view.rst
    schema/serial.rst
    authorization.rst
    function/index.rst
    query/index.rst
    tuning.rst
    partition.rst
    i18n.rst
    transaction.rst
    trigger.rst
    jsp.rst
    method.rst
    oodb.rst
    catalog.rst
    