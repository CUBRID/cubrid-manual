
:meta-keywords: procedure definition, create procedure, drop procedure, function definition, create function, drop function
:meta-description: Define functions/procedures in CUBRID database using create procedure, create function, drop procedure and drop function statements.


************************************************
STORED FUNCTION/PROCEDURE DEFINITION STATEMENTS
************************************************

.. _create-procedure:

CREATE PROCEDURE
=================

Create stored procedure using the **CREATE PROCEDURE** statement.
The other languages except Java do not support stored procedure. 
In CUBRID, only Java can implement stored procedure.
See details of how to use Stored Procedure, please refer to the :doc:`/sql/jsp`.

::

    CREATE [OR REPLACE] PROCEDURE procedure_name [(<parameter_definition> [, <parameter_definition>] ...)]
    {IS | AS} LANGUAGE JAVA <java_call_specification>
    COMMENT 'sp_comment_string';
    
        <parameter_definition> ::= parameter_name [IN|OUT|IN OUT|INOUT] sql_type [COMMENT 'param_comment_string']
        <java_call_specification> ::= NAME 'java_method_name (java_type [,java_type]...) [return java_type]'

*   *procedure_name*: Specifies the name of the stored procedure(maximum 254 bytes).
*   *parameter_name*: Specifies the name of the parameter (maximum 254 bytes).
*   *sql_type*: Specifies the data type of the parameter. See details on the data types that can be used for the parameter, refer to the :ref:`jsp-type-mapping`.
*   *param_comment_string*: Specifies comment of the parameter.
*   *sp_comment_string*: Specifies comment of the stored procedure.
*   *java_method_name*: Specifies the name of Java method name, including the name of the class it belongs to.
*   *java_type*: Specifies the Java data type. See details on the Java data types that can be used to return, refer to the :ref:`jsp-type-mapping`.

You must publish Java classes by using Java Call Specifications (<*java_call_specification*>) because it is unknown how a function in a class will be called by SQL statements or Java applications when Java classes are loaded.
See details on how to write Java Call Specifications, refer to the :ref:`call-specification`.

COMMENT of Java Stored Procedure
-----------------------------------

A comment of stored function/procedure can be written at the end of the statement as follows.

.. code-block:: sql


    CREATE FUNCTION Hello() RETURN VARCHAR
    AS LANGUAGE JAVA
    NAME 'SpCubrid.HelloCubrid() return java.lang.String'
    COMMENT 'function comment';

A comment of a paramenter can be written as follows.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test(i in number COMMENT 'arg i') 
    RETURN NUMBER AS LANGUAGE JAVA NAME 'SpTest.testInt(int) return int' COMMENT 'function test';

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


DROP PROCEDURE
==============

In CUBRID, A stored proceudre can be deleted using the **DROP PROCEDURE** statement.
Also, you can delete multiple stored procedures at the same time with several *procedure_name*\s separated by a comma (,).

::

    DROP PROCEDURE procedure_name [{ , procedure_name , ... }]

*   *procedure_name*: Specifies the name of procedure to delete

.. code-block:: sql

    DROP PROCEDURE hello, sp_int;

A stored procedure can be deleted only by the user who published it or by DBA members. 
For example, if a **PUBLIC** user published the 'sp_int' stored procedure, only the **PUBLIC** or **DBA** members can delete it.

.. _create-function:

CREATE FUNCTION
=================

Create stored function using the **CREATE FUNCTION** statement.
The other languages except Java do not support stored function. 
In CUBRID, only Java can implement stored function.
See details of how to use Stored Function, please refer to the :doc:`/sql/jsp`.

::

    CREATE [OR REPLACE] FUNCTION function_name [(<parameter_definition> [, <parameter_definition>] ...)] RETURN sql_type
    {IS | AS} LANGUAGE JAVA <java_call_specification>
    COMMENT 'sp_comment_string';
    
        <parameter_definition> ::= parameter_name [IN|OUT|IN OUT|INOUT] sql_type [COMMENT 'param_comment_string']
        <java_call_specification> ::= NAME 'java_method_name (java_type [,java_type]...) [return java_type]'

*   *function_name*: Specifies the name of the stored function(maximum 254 bytes).
*   *parameter_name*: Specifies the name of the parameter (maximum 254 bytes).
*   *sql_type*: Specifies the data type of the parameter or of the return value. See details on the data types that can be used for the parameter, refer to the :ref:`jsp-type-mapping`.
*   *param_comment_string*: Specifies comment of the parameter.
*   *sp_comment_string*: Specifies comment of the stored function.
*   *java_method_name*: Specifies the name of Java method name, including the name of the class it belongs to.
*   *java_type*: Specifies the Java data type. See details on the Java data types that can be used to return, refer to the :ref:`jsp-type-mapping`.

You must publish Java classes by using Java Call Specifications (<*java_call_specification*>) because it is unknown how a function in a class will be called by SQL statements or Java applications when Java classes are loaded.
See details on how to write Java Call Specifications, refer to the :ref:`call-specification`.

COMMENT of Java Stored Function
----------------------------------

A comment of stored function/procedure can be written at the end of the statement as follows.

.. code-block:: sql

    CREATE FUNCTION Hello() RETURN VARCHAR
    AS LANGUAGE JAVA
    NAME 'SpCubrid.HelloCubrid() return java.lang.String'
    COMMENT 'function comment';

A comment of a paramenter can be written as follows.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test(i in number COMMENT 'arg i') 
    RETURN NUMBER AS LANGUAGE JAVA NAME 'SpTest.testInt(int) return int' COMMENT 'function test';

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


DROP FUNCTION
==============

In CUBRID, A stored function can be deleted using the **DROP FUNCTION** statement.
Also, you can delete multiple stored functions at the same time with several *function_name*\s separated by a comma (,).

::

    DROP FUNCTION function_name [{ , function_name , ... }];

*   *function_name*: Specifies the name of function to delete

.. code-block:: sql

    DROP FUNCTION hello, sp_int;

A stored function can be deleted only by the user who published it or by DBA members. 
For example, if a **PUBLIC** user published the 'sp_int' stored function, only the **PUBLIC** or **DBA** members can delete it.

.. _call-specification:

Java Call Specification
==========================

You must publish Java classes by using call specifications because it is not known how a function in a class will be called by SQL statements or Java applications when Java classes are loaded.

With call specifications, Java function names, parameter types, return values and their types can be accessed by SQL statements or Java applications.
To write call specifications, use :ref:`create-function` or :ref:`create-procedure` statement. 

* Java stored function/procedure names are not case sensitive. 
* The maximum number of characters a Java stored function/procedure can have is 254 bytes.
* The maximum number of parameters a Java stored function/procedure can have is 64. 

If the parameter of a Java stored function/procedure is set to **OUT**, it will be passed as a one-dimensional array whose length is 1.
Therefore, a Java method must store its value to pass in the first space of the array.

.. code-block:: sql

    CREATE PROCEDURE test_out(x OUT STRING)
    AS LANGUAGE JAVA
    NAME 'SpCubrid.outTest(java.lang.String[] o)';

.. _jsp-type-mapping:

Data Type Mapping
------------------

When a Java stored function/procedure is published, it is not checked whether the return definition of the Java stored function/procedure coincides with the one in the declaration of the Java file.
Therefore, the Java stored function/procedure follows the return definition (SQL Type) provided at the time of registration. The return definition in the declaration is significant only as user-defined information.

In call specifications, the data types of SQL must correspond to the data types of Java parameter and return value.
In addition, when implementing a Java stored function/procedure, the data types of Java must match the data types of query result (ResultSet).
The following table shows SQL/Java data types allowed in CUBRID.

**Data Type Mapping**

    +------------------------+--------------------------+-------------------------------------------------------------------------+
    | Category               | SQL Type                 | Java Type                                                               |
    +========================+==========================+=========================================================================+
    | Numeric Types          | SHORT, SMALLINT          | short, java.lang.Short                                                  |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | INT, INTEGER             | int, java.lang.Integer                                                  |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | BIGINT                   | long, java.lang.Long                                                    |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | NUMERIC, DECIMAL         | java.math.BigDecimal                                                    |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | FLOAT, REAL              | float, java.lang.Float                                                  |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | DOUBLE, DOUBLE PRECISION | double, java.lang.Double                                                |
    +------------------------+--------------------------+-------------------------------------------------------------------------+
    | Date/Time Types        | DATE                     | java.sql.Date                                                           |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | TIME                     | java.sql.Time                                                           |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | TIMESTAMP                | java.sql.Timestamp                                                      |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | DATETIME                 | java.sql.Timestamp                                                      |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | TIMESTAMPLTZ             | X (not supported)                                                       |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | TIMESTAMPTZ              | X (not supported)                                                       |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | DATETIMELTZ              | X (not supported)                                                       |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | DATETIMETZ               | X (not supported)                                                       |
    +------------------------+--------------------------+-------------------------------------------------------------------------+
    | Bit String  Types      | BIT                      | X (not supported)                                                       |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | VARBIT                   | X (not supported)                                                       |
    +------------------------+--------------------------+-------------------------------------------------------------------------+
    | Character String Types | CHAR                     | java.lang.String                                                        |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | VARCHAR                  | java.lang.String                                                        |
    +------------------------+--------------------------+-------------------------------------------------------------------------+
    | Enum Type              | ENUM                     | X (not supported)                                                       |
    +------------------------+--------------------------+-------------------------------------------------------------------------+
    | LOB Types              | CLOB, BLOB               | X (not supported)                                                       |
    +------------------------+--------------------------+-------------------------------------------------------------------------+
    | Collection Types       | SET, MULTISET, SEQUENCE  | java.lang.Object[], java primitive type array, java wrapper class array |
    +------------------------+--------------------------+-------------------------------------------------------------------------+
    | Special Types          | JSON                     | X (not supported)                                                       |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | OBJECT, OID              | cubrid.sql.CUBRIDOID <interface>                                        |
    |                        +--------------------------+-------------------------------------------------------------------------+
    |                        | CURSOR                   | java.sql.ResultSet <interface>                                          |
    +------------------------+--------------------------+-------------------------------------------------------------------------+

**Implicit Data Type Conversion**

If the data type of SQL and the data type of Java do not correspond as shown in the table above, CUBRID implicitly attempts data type conversion according to the following table.
Please note that implicit data conversion may result in data loss.

    +-------------------------+----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+
    |                         | **Java Data Types**                                                                                                                                                                        |
    |                         +----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+
    |                         | byte,          | short,          | int,              | long,           | float,          | double,          |                      |                  |               |                    |
    | **SQL Data Types**      | java.lang.Byte | java.lang.Short | java.lang.Integer | java.lang.Long  | java.lang.Float | java.lang.Double | java.math.BigDecimal | java.lang.String | java.sql.Time | java.sql.Timestamp |
    +=========================+================+=================+===================+=================+=================+==================+======================+==================+===============+====================+
    | **SHORT, SMALLINT**     | O              | O               | O                 | O               | O               | O                | O                    | O                | X             | X                  |
    +-------------------------+----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+
    | **INT, INTEGER**        | O              | O               | O                 | O               | O               | O                | O                    | O                | X             | X                  |
    +-------------------------+----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+
    | **BIGINT**              | O              | O               | O                 | O               | O               | O                | O                    | O                | X             | X                  |
    +-------------------------+----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+
    | **NUMERIC, DECIMAL**    | O              | O               | O                 | O               | O               | O                | O                    | O                | X             | X                  |
    +-------------------------+----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+
    | **FLOAT, REAL**         | O              | O               | O                 | O               | O               | O                | O                    | O                | X             | X                  |
    +-------------------------+----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+
    | **DOUBLE**              | O              | O               | O                 | O               | O               | O                | O                    | O                | X             | X                  |
    | **DOUBLE PRECISION**    |                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    +-------------------------+----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+
    | **DATE**                | X              | X               | X                 | X               | X               | X                | X                    | O                | O             | O                  |
    +-------------------------+                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    | **TIME**                |                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    +-------------------------+                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    | **TIMESTAMP**           |                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    +-------------------------+                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    | **DATETIME**            |                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    +-------------------------+----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+
    | **CHAR**                | O              | O               | O                 | O               | O               | O                | O                    | O                | O             | O                  |
    +-------------------------+                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    | **VARCHAR**             |                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    +-------------------------+----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+
    | **SET**                 | X              | X               | X                 | X               | X               | X                | X                    | X                | X             | X                  |
    +-------------------------+                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    | **MULTISET**            |                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    +-------------------------+                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    | **SEQUENCE**            |                |                 |                   |                 |                 |                  |                      |                  |               |                    |
    +-------------------------+----------------+-----------------+-------------------+-----------------+-----------------+------------------+----------------------+------------------+---------------+--------------------+

    - X: Conversion not allowed
    - O: Implicit conversion
