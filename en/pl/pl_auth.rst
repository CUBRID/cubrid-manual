--------------------------------
Stored Procedure Authorization
--------------------------------

This chapter explains the permissions required to call a stored procedure and how to grant call permissions to other users.
It also details the difference between Owner's Rights and Caller's Rights, specifying under which user's permissions the call is made.

.. _pl-call-authorization:

Granting Procedure Call Permission
============================================

When a stored procedure is created, it belongs to the schema of the user who created it, and only users with **owner** and **DBA** permissions can call it.
If the owner of the stored procedure grants call permission to another user, the user granted permission can call the stored procedure of the other user.

The important thing to note is that if a routine that is not authorized is referenced in the implementation, it may result in a compilation or execution error.
Therefore, it is necessary to check whether the stored procedure references authorized objects.

CUBRID allows you to grant call permission to a stored procedure using the :ref:`GRANT <granting-authorization>` statement.

.. code-block:: sql

    GRANT EXECUTE ON PROCEDURE procedure_name TO user_name;

Conversely, you can revoke the execution privilege on a procedure by using the :ref:`REVOKE <revoking-authorization>` statement.

.. code-block:: sql

    REVOKE EXECUTE ON PROCEDURE procedure_name FROM user_name;

.. note::
        
    Stored procedures and stored functions do not support the **EXECUTE ON PROCEDURE** permission with the **WITH GRANT OPTION** option.
    Therefore, the owner of the stored procedure cannot grant the permission to another user.

.. _pl_authid:

The Difference Between Owner's Rights and Caller's Rights
=================================================================

When a stored procedure or function has call permission, it can specify which user's rights to execute it.
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
