-----------------------------
Creating a Stored Procedure
-----------------------------

Stored procedures and stored functions can be created using :ref:`create-procedure`\ and :ref:`create-function`\ respectively.
In CUBRID, there is no significant functional difference between stored procedures and stored functions.
The only difference is that stored functions define a return type, whereas stored procedures have a return type of void and always return NULL.

This chapter provides a detailed explanation of the considerations when creating stored procedures and stored functions.


.. _pl-schema:

Schema and Procedure Naming
===========================

::

    CREATE [OR REPLACE] [PROCEDURE] [schema_name.]procedure_name

    CREATE [OR REPLACE] [FUNCTION] [schema_name.]function_name

The names of stored procedures and stored functions are identifiers and must be created according to the principles outlined in :doc:`/sql/identifier`.
The name is limited to a maximum length of 222 bytes, and exceeding this length will result in an error.

When specifying the name of a stored procedure or stored function, the schema name (**schema_name**) does not need to be specified; it will be created and stored in the schema of the currently logged-in :ref:`user<user_schema>`.
Users with DBA privileges can create stored procedures and stored functions in other users' schemas, but regular users can only create them in their own schemas.

If a regular user attempts to create stored procedures and stored functions in another user's schema, an error will occur.

.. code-block:: sql
    -- Login as DBA
    CREATE USER U1;
    CREATE USER U2;
    CALL login ('U1', '') ON CLASS db_user;
    CREATE PROCEDURE u2.test_proc (arg1 INT, arg2 INT)
    AS BEGIN
        DBMS_OUTPUT.put_line (arg1 + arg2);
    END;
::

    ERROR: before ' ; '
    Only DBA, members of the DBA group, and the owner can perform CREATE PROCEDURE/FUNCTION.

.. _pl-parameter:

Specifying Parameters for Stored Procedures
========================================

Specifying parameters for stored procedures and stored functions allows for passing arguments when the procedure or function is called.
When specifying parameters, you must specify the name and data type of the parameter, and you can specify up to **64** parameters.

Stored functions receive input parameters and return a result value, so you must specify a result data type.
Stored procedures, on the other hand, are of the void type and always return NULL.
The result data type for stored procedures and stored functions is a subset of the data types supported by CUBRID SQL (:doc:`/sql/datatype`\).
For more information, refer to :ref:`pl-arg-type-restriction`.

.. code-block:: sql

    CREATE PROCEDURE test_proc (arg1 INT, arg2 INT)
    AS BEGIN
        DBMS_OUTPUT.put_line (arg1 + arg2);
    END;

    CREATE FUNCTION test_func (arg1 INT, arg2 INT) RETURN INT
    AS BEGIN
        RETURN arg1 + arg2;
    END;

    SELECT test_proc(1, 2);
    SELECT test_func(1, 2);

::

      test_proc(1, 2)
    ======================
      NULL

    <DBMS_OUTPUT>
    ====
    3

      test_func(1, 2)
    =================
                    3

.. _pl-arg-type-restriction:

Supported Data Types of Arguments and Return
---------------------------------------------

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


.. _pl-arg-mode:

Specifying the Mode of Arguments
============================================

The modes of arguments are IN, OUT, and IN OUT.
If no mode is specified when specifying arguments, they are assumed to be IN mode by default.

Stored procedures and stored functions can use the **CALL**\ statement and OUT or IN OUT arguments to return the values of arguments that have been changed in the stored procedure or stored function.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_proc (arg1 OUT INT)
    AS BEGIN
        arg1 := 5;
    END;


    CREATE OR REPLACE FUNCTION test_func (arg1 IN OUT INT) RETURN INT
    AS BEGIN
        arg1 := arg1 + 1;
        RETURN arg1 + 5;
    END;

.. code-block:: sql

    SELECT 1 into :a;
    select :a;
    CALL test_proc(:a);
    select :a;

::

    -- select :a;

               :a
    =============
                1

    -- select :a;

               :a
    =============
                5

If you specify the OUT or IN OUT mode for any argument in a stored procedure or stored function, 
it can not be called within a query statement (**SELECT**, **UPDATE**, **DELETE**, etc.); it will return an error.

.. code-block:: sql

    SELECT test_func(1);

::

    In the command from line 1,
    ERROR: Semantic: Stored procedure/function 'u1.test_func' has OUT or IN OUT arguments

.. _pl-default-args:

Using Default Arguments
=======================

You can specify default values for the arguments of stored procedures and stored functions.

* You can specify default values using the **:=** or **DEFAULT** keywords.
* When a default value is specified, you can omit the argument when calling the stored procedure or function. The omitted argument will be replaced with the default value.
* Default values can be specified as literal values and are stored as string values up to 255 bytes. An error will occur if this size is exceeded.
* The following functions are allowed as default values in addition to literal values:

+-------------------------------+---------------+
| Default Value                 | Data Type     |
+===============================+===============+
| SYS_TIMESTAMP                 | TIMESTAMP     |
+-------------------------------+---------------+
| UNIX_TIMESTAMP()              | INTEGER       |
+-------------------------------+---------------+
| CURRENT_TIMESTAMP             | TIMESTAMP     |
+-------------------------------+---------------+
| SYS_DATETIME                  | DATETIME      |
+-------------------------------+---------------+
| CURRENT_DATETIME              | DATETIME      |
+-------------------------------+---------------+
| SYS_DATE                      | DATE          |
+-------------------------------+---------------+
| CURRENT_DATE                  | DATE          |
+-------------------------------+---------------+
| SYS_TIME                      | TIME          |
+-------------------------------+---------------+
| CURRENT_TIME                  | TIME          |
+-------------------------------+---------------+
| USER, USER()                  | STRING        |
+-------------------------------+---------------+
| TO_CHAR(date_time[, format])  | STRING        |
+-------------------------------+---------------+
| TO_CHAR(number[, format])     | STRING        |
+-------------------------------+---------------+

.. note::

    If a function that is not listed in the table above is used as a **DEFAULT** value, 
    the result evaluated at the time of procedure creation is stored as a **DEFAULT** value.
    Therefore, the default value of the column will be the result value of the function at the time of **INSERT** execution, 
    not the result value of the function at the time of procedure creation.
    Stored functions cannot be used as **DEFAULT** values, so an error will be returned.

.. code-block:: sql

    CREATE FUNCTION fn_int (a int) return int as begin return a; end;
    
    -- error
    CREATE FUNCTION default_val_func_test (id INT DEFAULT fn_int (1)) return int as begin return id; end;

::

    ERROR: before ' ); '
    'fn_int(1)' function can not be used in DEFAULT clause.

Here is a simple example of specifying a literal value as a default:

.. code-block:: sql

        CREATE FUNCTION default_args (
                a INT := 1, 
                b INT DEFAULT 2
        ) RETURN INT
        AS BEGIN RETURN a + b; END;

        SELECT default_args(); 
        SELECT default_args(3);
        SELECT default_args(3, 4);

::

          default_args()
        ================
                        3

          default_args(3)
        =================
                        5

          default_args(3, 4)
        ====================
                        7

Here is an example of specifying a function as a default value:

.. code-block:: sql

        CREATE FUNCTION get_age (
            birth DATE DEFAULT DATE'2000-01-01',
            today DATE DEFAULT SYS_DATE
        ) RETURN INT
        AS
        BEGIN
            RETURN YEAR(today) - YEAR(birth)
                - CASE WHEN TO_CHAR(today, 'MMDD') < TO_CHAR(birth, 'MMDD') THEN 1 ELSE 0 END;
        END;

        SELECT get_age();
        SELECT get_age(DATE'2000-05-10');
        SELECT get_age(DATE'2000-05-10', DATE'2025-03-24');

::        

        get_age()
        =============
                   25

        get_age(date '2000-05-10')
        ============================
                                  24

        get_age(date '2000-05-10', date '2025-03-24')
        ===============================================
                                                     24

.. _pl_function_overloading:

Function Overloading
======================================

Function overloading is the process of defining multiple functions with the same name but different argument types or numbers.
**CUBRID does not support function overloading**. This means that you cannot create multiple stored procedures or stored functions with the same name if they have different argument types or numbers.
Therefore, when using the **CREATE OR REPLACE** statement, the new procedure is not created and the existing procedure is overwritten, so caution is required.

.. code-block:: sql

    CREATE FUNCTION test_func (arg1 INT) RETURN INT AS
    BEGIN
        RETURN arg1;
    END;

    SELECT test_func (1);

    CREATE OR REPLACE FUNCTION test_func (arg1 INT, arg2 INT) RETURN INT AS
    BEGIN
        RETURN arg1 + arg2;
    END;

    SELECT test_func (1); -- error
    SELECT test_func (1, 2); -- 3

::
    
    ERROR: Parameter count is invalid. expected: 2, actual: 1

      test_func(1, 2)
    =================
                    3

.. _pl_object_dependencies:

Object Dependencies
======================================

The dependencies of stored procedures and stored functions on the database objects they internally reference are only checked when registering in PL/CSQL.
Therefore, if the table, view, sequence, or index objects referenced in the PL/CSQL procedure do not exist, an error will occur when registering the procedure.

.. code-block:: sql

    DROP TABLE IF EXISTS test_tbl;

    CREATE OR REPLACE FUNCTION test_func () RETURN INT
        k INT;
    AS
    BEGIN
        SELECT id INTO k FROM test_tbl LIMIT 1;
        RETURN k;
    END;

::

    ERROR: Stored procedure compile error: Syntax: before '  LIMIT 1'
    Unknown class "dba.test_tbl". select id from [dba.test_tbl] limit 1

The following example will create the test_tbl table correctly.

.. code-block:: sql

    DROP TABLE IF EXISTS test_tbl;
    CREATE TABLE test_tbl (id INT);
    INSERT INTO test_tbl VALUES (1);

    CREATE OR REPLACE FUNCTION test_func () RETURN INT
    AS
        k INT;
    BEGIN
        SELECT id INTO k FROM test_tbl LIMIT 1;
        RETURN k;
    END;

    SELECT test_func();

::

      test_func()
    =============
                1

In the case of PL/CSQL Dynamic SQL or Java SP, the stored string is executed at runtime, 
so it is not checked when registering the procedure, but it is checked when calling the procedure.

.. code-block:: sql

    DROP TABLE IF EXISTS test_tbl;

    CREATE OR REPLACE FUNCTION test_func () RETURN INT
    AS
        k INT;
    BEGIN
        execute immediate 'SELECT id FROM test_tbl LIMIT 1' INTO k;
        RETURN k;
    END;

    SELECT test_func();

::

    ERROR: Stored procedure execute error: 
      (line 5, column 9) Syntax: before '  LIMIT 1'
    Unknown class "dba.test_tbl". select id from [dba.test_tbl] limit 1

If the object referenced internally by the stored procedure or stored function is deleted, 
the system does not check the dependency, so even if it is registered correctly, an error may occur when the procedure is called.
Therefore, you should be careful so that the object referenced by the stored procedure or stored function is not deleted.

.. code-block:: sql

    DROP TABLE IF EXISTS test_tbl;
    CREATE TABLE test_tbl (id INT);
    INSERT INTO test_tbl VALUES (1);

    CREATE OR REPLACE FUNCTION test_func () RETURN INT
    AS
        k INT;
    BEGIN
        SELECT id INTO k FROM test_tbl LIMIT 1;
        RETURN k;
    END;

    DROP TABLE IF EXISTS test_tbl;
    SELECT test_func();

::

    ERROR: Stored procedure execute error: 
      (line 5, column 9) Syntax: Unknown class "dba.test_tbl". select [dba.test_tbl].id from [dba.test_tbl] [dba.test_tbl] ...
