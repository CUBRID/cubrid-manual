-----------------------------
Call a Stored Procedure
-----------------------------

Registered stored procedures and stored functions can be called using the :doc:`/sql/query/call` statement or in SQL statements.
In general, stored procedures are called using the :doc:`/sql/query/call` statement, and stored functions are used in SQL statements where a return value is needed.

This chapter provides a detailed explanation of the considerations when calling stored procedures and stored functions.

.. _pl_call_stmt:

CALL Statement
==============

You can call a stored procedure using the :doc:`/sql/query/call` statement as follows in the CSQL interpreter.

.. code-block:: sql

    -- Execute in csql
    CREATE PROCEDURE hello ()
    AS
    BEGIN
        DBMS_OUTPUT.put_line('Hello, CUBRID!');
    END;

    ;server-output on

    -- Call
    CALL hello();

::
    
      Result              
    ======================
      NULL                

    <DBMS_OUTPUT>
    ====
    Hello, CUBRID!

.. _pl_call_sql:

Calling in SQL Statements
===========================

In the case of calling in SQL statements, stored functions are typically used to return values. 
The following example shows how to calculate the number of medals for a specific country using a stored function.

.. code-block:: sql

    CREATE FUNCTION count_medals(nation STRING) RETURN INT
    AS
    cnt INT;
    BEGIN
        SELECT COUNT(*) INTO cnt
        FROM game
        WHERE nation_code = nation;
        RETURN cnt;
    END;

    SELECT count_medals('USA');
    SELECT count_medals('KOR');

::

      count_medals('USA')
    =====================
                     1118

      count_medals('KOR')
    =====================
                      316

.. _pl_nested_call_limits:

Limitations on Nested Procedure Calls
=======================================

The maximum allowed nested procedure call limit is **16**.
Nested procedure call refers to calling another procedure from within a procedure.
In the case of recursive calls, the limit is not applied.

The following is an example of the limitations on nested procedure calls.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_factorial_query(n BIGINT) RETURN BIGINT
    AS
    k BIGINT;
    BEGIN
        IF n = 0 THEN
            RETURN 1;
        ELSE
            SELECT test_factorial_query(n - 1) INTO k;
            RETURN n * k;
        END IF;
    END;

    SELECT test_factorial_query(15);
    SELECT test_factorial_query(16);

::

      test_factorial_query(15)
    ==========================
                 1307674368000


    -- SELECT test_factorial_query(16);
    ERROR: Stored procedure execute error:
      ...
      (line 8, column 13) Stored procedure execute error:
      (line 8, column 13) Stored procedure execute error: Too many nested stored procedure call.

The following is an example of a recursive call that does not go through a query. In this case, the nested call limit is not applied.
Recursively calling the following example may result in an overflow of the BIGINT value or excessive use of system resources, so be careful.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_factorial_constant(n BIGINT) RETURN BIGINT
    AS
    k BIGINT;
    BEGIN
        IF n = 0 THEN
            RETURN 1;
        ELSE
            RETURN n * test_factorial_constant(n - 1);
        END IF;
    END;

    SELECT test_factorial_constant(16);
    SELECT test_factorial_constant(25);

::

      test_factorial_constant(16)
    =============================
                   20922789888000

    -- SELECT test_factorial_constant(25);
    ERROR: Stored procedure execute error: 
      (line 8, column 20) data overflow in multiplication of BIGINT values

.. _pl-call-authorization:

Procedure Execution Authorization
============================================

When a stored procedure is created, it belongs to the schema of the user who created it, and only users with **owner** and **DBA** permissions can call it.
If the owner of the stored procedure grants call permission to another user, the user granted permission can call the stored procedure of the other user.

The important thing to note is that if a routine that is not authorized is referenced in the implementation, it may result in a compilation or execution error.
Therefore, it is necessary to check whether the stored procedure references authorized objects.

CUBRID allows you to grant call permission to a stored procedure using the :ref:`GRANT <granting-authorization>` statement.

.. code-block:: sql

    GRANT EXECUTE ON PROCEDURE procedure_name TO user_name;

반대로 :ref:`REVOKE <revoking-authorization>` 문을 사용하여 프로시저에 대한 호출 권한을 해지할 수 있다.

.. code-block:: sql

    REVOKE EXECUTE ON PROCEDURE procedure_name FROM user_name;

.. note::
        
    Stored procedures and stored functions do not support the **EXECUTE ON PROCEDURE** permission with the **WITH GRANT OPTION** option.
    Therefore, the owner of the stored procedure cannot grant the permission to another user.

.. _pl_authid:

Owner's Rights and Caller's Rights
============================================

Stored procedures and functions can be executed with either **Owner's Rights** or **Caller's Rights**. The execution rights can be chosen during the creation of the stored procedure or function, and it will be executed with the specified rights.

The following explains each type of rights and their differences:

* **Owner's Rights** (also known as Definer's Rights)
        * Executed with the rights of the user who created the stored procedure.
        * If the creator grants rights to another user, the granted user can also execute the stored procedure.
        * All rights of the owner of the stored procedure take effect, allowing it to access database objects that the owner has permission to access without requiring additional grants. If a stored procedure is created with the **DBA**'s owner's rights, it can access all database objects, so special attention should be paid to security issues such as SQL Injection when using dynamic SQL.
        * Suitable for common tasks or data access needs.

In contrast, Caller's Rights are as follows:

* **Caller's Rights** (also known as Invoker's Rights)
        * Executed with the rights of the caller, not the creator of the stored procedure.
        * If a stored procedure is created with Caller's Rights and rights are granted to another user, it will be executed with the rights of the user calling the stored procedure.
        * When a stored procedure is created with Caller's Rights, it is executed at the caller's rights level, not the creator's, allowing access to database objects at the caller's rights level. Therefore, when creating a stored procedure with Caller's Rights, the caller's rights level should be considered.
        * Suitable when different rights need to be applied per caller.

Understanding the difference between Owner's Rights and Caller's Rights when creating stored procedures and utilizing them can lead to more efficient database management.

When creating a stored procedure or function, the **AUTHID** attribute can be specified to determine whether it operates with **Owner's Rights** or **Caller's Rights**. For detailed definitions, refer to :doc:`/sql/schema/stored_routine_stmt`.

The **AUTHID** attribute can be specified as follows, with **DEFINER** and **OWNER** being synonyms, as well as **CURRENT_USER** and **CALLER**.

* **Owner's Rights**: AUTHID DEFINER or AUTHID OWNER
* **Caller's Rights**: AUTHID CURRENT_USER or AUTHID CALLER

If the attribute is not specified, it defaults to **Owner's Rights**.

The following is an example of logging in as a DBA user, creating a stored function with DBA Owner's Rights that returns :ref:`fn_current_user`, and calling it from user U1.

.. code-block:: sql

        -- Login as DBA
        CREATE USER U1;

        CREATE OR REPLACE FUNCTION fn_current_user() RETURN STRING AUTHID DEFINER AS
        BEGIN
                RETURN CURRENT_USER;
        END;

        GRANT EXECUTE ON PROCEDURE fn_current_user TO U1;

        CALL login ('U1', '') ON CLASS db_user;

        SELECT dba.fn_current_user();

::

        dba.fn_current_user()
        ======================
        'DBA@<host>'    

.. warning::

        * Currently, CUBRID's PL/CSQL only supports **Owner's Rights**, and does not support **Caller's Rights**.
        * To use **Caller's Rights**, stored procedures must be written using Java SP.

.. note::

        * **Owner's Rights** are supported from CUBRID version 11.4.
