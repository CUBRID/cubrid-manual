
:meta-keywords: procedure definition, create procedure, drop procedure, function definition, create function, drop function
:meta-description: Define functions/procedures in CUBRID database using create procedure, create function, drop procedure and drop function statements.


************************************************
STORED FUNCTION/PROCEDURE DEFINITION STATEMENTS
************************************************

.. _create-procedure:

CREATE PROCEDURE
=================

Create stored procedure using the **CREATE PROCEDURE** statement.

::

    CREATE [OR REPLACE] PROCEDURE [schema_name.]procedure_name ([<parameter_definition> [, <parameter_definition>] ...])
    [<procedure_properties>]
    {IS | AS} LANGUAGE <lang> 
      <body>
    COMMENT 'procedure_comment';
    
        <parameter_definition> ::= parameter_name [mode] sql_type [ { DEFAULT | = } <default_expr> ] [COMMENT 'parameter_comment_string']
        <lang> ::= [PLCSQL | JAVA]
        <mode> ::= IN | OUT | IN OUT | INOUT
        <procedure_properties> ::= 
          <authid> = AUTHID {DEFINER | OWNER | CALLER | CURRENT_USER}

You can use the **OR REPLACE** clause to replace or create a new stored function/procedure.

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *procedure_name*: Specifies the name of the stored procedure to be created (maximum 222 bytes).
*   *parameter_name*: Specifies the name of the parameter (maximum 254 bytes).
*   *sql_type*: Specifies the data type of the parameter.
*   *default_arg*: Specifies the default value of the parameter. Refer to :doc:`/pl/pl_default_args`.
*   *authid*: Specifies the execution authority of the stored procedure. For more details, refer to :doc:`/pl/pl_authid`.
*   *parameter_comment_string*: Specifies the comment string for the parameter.
*   *body*: Specifies the body of the stored procedure.
*   *procedure_comment*: Specifies the comment string for the stored procedure.

COMMENT of Stored Procedure
-----------------------------------

A comment of stored function/procedure can be written at the end of the statement as follows.

.. code-block:: sql


    CREATE FUNCTION Hello() RETURN VARCHAR
    AS LANGUAGE JAVA
    NAME 'SpCubrid.HelloCubrid() return java.lang.String'
    COMMENT 'function comment';

A comment of a paramenter can be written as follows.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test(i in int COMMENT 'arg i') 
    RETURN int AS LANGUAGE JAVA NAME 'SpTest.testInt(int) return int' COMMENT 'function test';

A comment of a stored function/procedure can be shown by running the following syntax.

.. code-block:: sql

    SELECT sp_name, comment FROM db_stored_procedure; 

A comment for a parameter of a function can be shown by running the following syntax.

.. code-block:: sql
          
    SELECT sp_name, arg_name, comment FROM db_stored_procedure_args;


Checking the Published Java Stored Procedure Information
-------------------------------------------------------------------

You can check the information on the published Java stored procedure.
The **db_stored_procedure** system virtual table provides the information on stored names and types, return types, number of parameters, Java class specifications, and the owner.
The **db_stored_procedure_args** system virtual table provides the information on parameters used in the stored function/procedure.

.. code-block:: sql

    SELECT * FROM db_stored_procedure WHERE sp_type = 'PROCEDURE';
    
::
    
    sp_name               sp_type               return_type             arg_count  lang target                owner
    ================================================================================
    'athlete_add'         'PROCEDURE'           'void'                          4  'JAVA''Athlete.Athlete(java.lang.String, java.lang.String, java.lang.String, java.lang.String)'  'DBA'

.. code-block:: sql
    
    SELECT * FROM db_stored_procedure_args WHERE sp_name = 'athlete_add';
    
::
    
    sp_name   index_of  arg_name  data_type      mode
    =================================================
     'athlete_add'                   0  'name'                'STRING'              'IN'
     'athlete_add'                   1  'gender'              'STRING'              'IN'
     'athlete_add'                   2  'nation_code'         'STRING'              'IN'
     'athlete_add'                   3  'event'               'STRING'              'IN'


.. _create-function:

CREATE FUNCTION
=================

Create stored function using the **CREATE FUNCTION** statement.

::

    CREATE [OR REPLACE] FUNCTION [schema_name.]function_name ([<parameter_definition> [, <parameter_definition>] ...])
    RETURN sql_type
    [<function_properties>]
    {IS | AS} LANGUAGE <lang> 
      <body>
    COMMENT 'function_comment';
    
        <parameter_definition> ::= parameter_name [mode] sql_type [<default_arg>] [COMMENT 'param_comment_string']
            <default_arg> ::= { DEFAULT | = } <default_expr>
        <procedure_properties> ::= <authid> | <deterministic>
            <authid> = AUTHID {DEFINER | OWNER | CALLER | CURRENT_USER}
            <deterministic> = [NOT DETERMINISTIC | DETERMINISTIC]
        <lang> ::= [PLCSQL | JAVA]
        <mode> ::= IN | OUT | IN OUT | INOUT

*   *schema_name*: Specifies the schema name (up to 31 bytes). If omitted, the schema name of the current session is used.
*   *function_name*: Specifies the name of the stored function to be created (up to 222 bytes).
*   *parameter_name*: Specifies the name of the parameter (up to 254 bytes).
*   *sql_type*: Specifies the data type of the parameter or return value. For available data types, refer to :ref:`pl-supported_sql_type`.
*   *default_arg*: Specifies the default value of the parameter. Refer to :doc:`/pl/pl_default_args`.
*   *param_comment_string*: Specifies the comment string for the parameter.
*   *authid*: Specifies the execution authority of the stored function. For more details, refer to :doc:`/pl/pl_authid`.
*   *deterministic*: Specifies whether the stored function is deterministic. For more details, refer to :ref:`pl-deterministic`.
*   *body*: Specifies the body of the stored function.
*   *function_comment*: Specifies the comment string for the stored function.

COMMENT of Stored Function
----------------------------------

A comment of stored function/procedure can be written at the end of the statement as follows.

.. code-block:: sql

    CREATE FUNCTION Hello() RETURN VARCHAR
    AS LANGUAGE JAVA
    NAME 'SpCubrid.HelloCubrid() return java.lang.String'
    COMMENT 'function comment';

A comment of a paramenter can be written as follows.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test(i in int COMMENT 'arg i') 
    RETURN int AS LANGUAGE JAVA NAME 'SpTest.testInt(int) return int' COMMENT 'function test';

A comment of a stored function/procedure can be shown by running the following syntax.

.. code-block:: sql

    SELECT sp_name, comment FROM db_stored_procedure; 

A comment for a parameter of a function can be shown by running the following syntax.

.. code-block:: sql
          
    SELECT sp_name, arg_name, comment FROM db_stored_procedure_args;

Checking the Published Java Stored Function Information
---------------------------------------------------------

You can check the information on the published Java stored function.
The **db_stored_procedure** system virtual table provides the information on stored names and types, return types, number of parameters, Java class specifications, and the owner.
The **db_stored_procedure_args** system virtual table provides the information on parameters used in the stored function/procedure.

.. code-block:: sql

    SELECT * FROM db_stored_procedure WHERE sp_type = 'FUNCTION';
    
::
    
    sp_name               sp_type               return_type             arg_count  lang target                owner
    ================================================================================
    'hello'               'FUNCTION'            'STRING'                        0  'JAVA''SpCubrid.HelloCubrid() return java.lang.String'  'DBA'
     
    'sp_int'              'FUNCTION'            'INTEGER'                       1  'JAVA''SpCubrid.SpInt(int) return int'  'DBA'

.. code-block:: sql
    
    SELECT * FROM db_stored_procedure_args WHERE sp_name = 'sp_int';
    
::
    
    sp_name   index_of  arg_name  data_type      mode
    =================================================
     'sp_int'                        0  'i'                   'INTEGER'             'IN'


DROP PROCEDURE
==============

In CUBRID, A stored proceudre can be deleted using the **DROP PROCEDURE** statement.
Also, you can delete multiple stored procedures at the same time with several *procedure_name*\s separated by a comma (,).

::

    DROP PROCEDURE procedure_name [{ , procedure_name , ... }];

*   *procedure_name*: Specifies the name of procedure to delete

.. code-block:: sql

    DROP PROCEDURE hello, sp_int;

A stored procedure can be deleted only by the user who published it or by DBA members. 
For example, if a **PUBLIC** user published the 'sp_int' stored procedure, only the **PUBLIC** or **DBA** members can delete it.

DROP FUNCTION
==============

In CUBRID, A stored function can be deleted using the **DROP FUNCTION** statement.
You can also delete multiple stored functions at the same time by specifying several *function_name*\s separated by commas (,).

::

    DROP FUNCTION function_name [{ , function_name , ... }];

*   *function_name*: Specifies the name of function to delete

.. code-block:: sql

    DROP FUNCTION hello, sp_int;

A stored function can be deleted only by the user who published it or by DBA members. 
For example, if a **PUBLIC** user published the 'sp_int' stored function, only the **PUBLIC** or **DBA** members can delete it.
