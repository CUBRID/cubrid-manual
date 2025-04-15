
:meta-keywords: cubrid authorization, cubrid dba, cubrid user
:meta-description: CUBRID has two types of users by default: DBA and PUBLIC. DBA and DBA members can create, drop and alter users by using SQL statements.

***************
User Management
***************

Database User
=============

To know the user name's writing rule, see :doc:`identifier`.

CUBRID has two types of users by default: **DBA** and **PUBLIC**. At initial installation of the product, no password is set for the users.

*   All users of the database are automatically the members of **PUBLIC**, so all users have authorization granted to the **PUBLIC** user. Granting authorization to the **PUBLIC** means granting it all users.

*   The **DBA** user has the authorization of the database administrator. The **DBA** automatically becomes the member of all users and groups. That is, the **DBA** is granted the access for all tables. Therefore, there is no need to grant authorization explicitly to the **DBA** and **DBA** members. Each database user has a unique name. The database administrator can create multiple users simultaneously using the **cubrid createdb** utility (see :ref:`cubrid-utilities` for details). A database user cannot have a member who already has the same authorization. If authorization is granted to a user, all members of the user is automatically granted the same authorization.

.. note::

    From version 11.3 of CUBRID, **DBA** members can also query objects of all users in the system catalog virtual class.

.. _create-user:

CREATE USER
===========

You can create a user using the CREATE USER statement. The default DBA, PUBLIC user is created without a password. ::

    CREATE USER user_name
    [PASSWORD password]
    [GROUPS user_name [{, user_name } ... ]]
    [MEMBERS user_name [{, user_name } ... ]] 
    [COMMENT 'comment_string'];

*   *user_name*: specifies the user name to create.
*   *password*: specifies the user password to create.
*   *comment_string*: specifies the user comment to create.

.. note::

    Only **DBA** and **DBA** members can create users using the CREATE USER statement.

The following example creates a user test_user1 and specifies a password.

.. code-block:: sql

    CREATE USER test_user1 PASSWORD 'password';

The following example shows how to create a user and add member to the user. By the following statement, *company* becomes a group that has *engineering*, *marketing* and *design* as its members. *marketing* becomes a group with members *smith* and *jones*, *design* becomes a group with a member *smith*, and *engineering* becomes a group with a member *brown*.

.. code-block:: sql

    CREATE USER company;
    CREATE USER engineering GROUPS company;
    CREATE USER marketing GROUPS company;
    CREATE USER design GROUPS company;
    CREATE USER smith GROUPS design, marketing;
    CREATE USER jones GROUPS marketing;  
    CREATE USER brown GROUPS engineering;

The following example shows how to create the same groups as above but use the **MEMBERS** keyword instead of **GROUPS**.

.. code-block:: sql

    CREATE USER smith;
    CREATE USER brown;
    CREATE USER jones;
    CREATE USER engineering MEMBERS brown;
    CREATE USER marketing MEMBERS smith, jones;
    CREATE USER design MEMBERS smith;
    CREATE USER company MEMBERS engineering, marketing, design;

User's COMMENT
--------------

The following example creates a user test_user1 and adds a password and a comment.

.. code-block:: sql

    CREATE USER test_user1 PASSWORD 'password' COMMENT 'new user';
    
You can see a comment for a user with this syntax.

.. code-block:: sql

    SELECT name, comment FROM db_user;

To change user comment, refer to the description of the ALTER USER statement.

.. _alter-user:

ALTER USER
==========

You can use the ALTER USER statement to change the password, members and comment of a created user. ::

    ALTER USER user_name 
    [PASSWORD password] |
    [ADD MEMBERS user_name [{, user_name } ... ]] |
    [DROP MEMBERS user_name [{, user_name } ... ]]
    [COMMENT 'comment_string'];

*   *user_name*: specifies the user name to change.
*   *password*: specifies the user password to change.
*   *comment_string*: specifies the user comment to change.

.. note::

    **DBA** and **DBA** members can use the ALTER USER statement to change the password, members and comment of **all users**.

    **General users** can change **their own** password, member and comment using the ALTER USER statement.

The following example creates a user test_user1 and changes the password.

.. code-block:: sql

    CREATE USER test_user1;
    ALTER USER test_user1 PASSWORD '1234';

The following example creates a user and adds members to the created user. This example does the same thing as the example in CREATE USER .. MEMBERS ..

.. code-block:: sql

    CREATE USER company;
    CREATE USER engineering;
    CREATE USER marketing;
    CREATE USER design;
    CREATE USER smith;
    CREATE USER jones;
    CREATE USER brown;

    ALTER USER engineering ADD MEMBERS brown;
    ALTER USER marketing ADD MEMBERS smith, jones;
    ALTER USER design ADD MEMBERS smith;
    ALTER USER company ADD MEMBERS engineering, marketing, design;

The following example deletes the members of a created user group. The *marketing* member is deleted from the *company* group through the following sentence and the *marketing* group deletes *smith* and *jones* from the member.

.. code-block:: sql

    ALTER USER company DROP MEMBERS marketing;
    ALTER USER marketing DROP MEMBERS smith, jones;

User's COMMENT Change
---------------------

The following example changes the comment for the created user.

.. code-block:: sql

    CREATE USER test_user1 COMMENT 'new user';
    ALTER USER test_user1 COMMENT 'old user';

.. _drop-user:

DROP USER
=========

You can delete a user using the DROP USER statement. Users who own objects in table, view, trigger, stored function/procedure, serial, synonym, and server cannot be dropped. ::

    DROP USER user_name;

*   *user_name*: specifies the user name to delete.

.. note::

    Only **DBA** and **DBA** members can delete users using the DROP USER statement.

The following example shows how to create a user (*test_user1*), change a password, and delete the user.

.. code-block:: sql

    CREATE USER test_user1;
    ALTER USER test_user1 PASSWORD '1234';
    DROP USER test_user1; 

.. _granting-authorization:

GRANT
=====

In CUBRID, the following database objects can be granted permissions:

*  **Table**
*  **View**
*  **Procedure**

To allow other users (groups) to access the database objects you created, you must grant them the appropriate permissions. All members of a group with granted permissions have the same permissions, so there is no need to grant permissions individually to each member. However, members of a group with the WITH GRANT OPTION permission, who are not part of the DBA or owner group, cannot grant the received permissions to other users.

Database objects created by the **PUBLIC** user are accessible to all users.

You can grant access permissions to a user using the following **GRANT** statement: ::

    (1) Table and View: 
        GRANT operation [ { ,operation } ... ] ON [schema_name.]object_name [ { , [schema_name.]object_name } ... ] 
        TO user [ { ,user } ... ] [ WITH GRANT OPTION ];

    (2) Stored Procedure and Function: 
        GRANT EXECUTE ON PROCEDURE [schema_name.]object_name
        TO user [ { ,user } ... ];

* *operation*: Specifies an operation that can be used when granting authorization. The following table shows operations.

    * (1) Table and View

        *   **SELECT**: Allows reading the table definitions and retrieving instances. The most common type of permission.
        *   **INSERT**: Allows creating instances in the table.
        *   **UPDATE**: Allows modifying instances already existing in the table.
        *   **DELETE**: Allows deleting instances in the table.
        *   **ALTER**: Allows modifying the table definition, renaming, or deleting the table.
        *   **INDEX**: Allows creating indexes on columns to improve search speed.
        *   **EXECUTE**: Allows calling table methods or instance methods.
        *   **ALL PRIVILEGES**: Includes all the above-mentioned permissions.

    * (2) Stored Procedure and Function

        *   **EXECUTE ON PROCEDURE**: Allows calling a stored procedure or function.

* *schema_name*: Specifies the schema name of the table or virtual table. If omitted, the schema name of the current session is used.
* *table_name*: Specifies the name of a table or virtual table to be granted.
* *user*: Specifies the name of a user (group) to be granted. Enter the login name of the database user or **PUBLIC**, a system-defined user. If **PUBLIC** is specified, all database users are granted with the permission.
* **WITH GRANT OPTION**
    
    * The **WITH GRANT OPTION** allows the grantee of authorization to grant that same authorization to another user.
    * The **EXECUTE ON PROCEDURE** permission for stored procedures and functions does not support the **WITH GRANT OPTION**.

The following example shows how to grant the **SELECT** authorization for the *olympic* table to *smith* (including his members).

.. code-block:: sql

    GRANT SELECT ON olympic TO smith;

The following example shows how to grant the **SELECT**, **INSERT**, **UPDATE** and **DELETE** authorization on the *nation* and *athlete* tables to *brown* and *jones* (including their members).

.. code-block:: sql

    GRANT SELECT, INSERT, UPDATE, DELETE ON nation, athlete TO  brown, jones;

The following example shows how to grant every authorization on the *tbl1* and *tbl2* tables to all users(public).

.. code-block:: sql

    CREATE TABLE tbl1 (a INT);
    CREATE TABLE tbl2 (a INT);
    GRANT ALL PRIVILEGES ON tbl1, tbl2 TO public;

The following **GRANT** statement is example shows how to grant retrieving authorization on the *record* and *history* tables to *brown*. Using **WITH GRANT OPTION** allows *brown* to grant retrieving to another users. *brown* can grant authorization to others within his authorization.

.. code-block:: sql

    GRANT SELECT ON record, history TO brown WITH GRANT OPTION;

.. note::

    *   The grantor of authorization must be the owner of all tables listed before the grant operation or have **WITH GRANT OPTION** specified.
    *   Before granting **SELECT**, **UPDATE**, **DELETE** and **INSERT** authorization for a virtual table, the owner of the virtual table must have **SELECT** and **GRANT** authorization for all the tables included in the query specification. The **DBA** user and the members of the **DBA** group are automatically granted all authorization for all tables.
    *   To execute the **TRUNCATE** statement, the **ALTER**, **INDEX**, and **DELETE** authorization is **required**.

The following example shows how to grant the permission to execute the *my_sp* procedure to *smith*.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE my_sp ()
    IS
    BEGIN
             DBMS_OUTPUT.put_line('grant test');
    END;
    GRANT EXECUTE ON PROCEDURE my_sp TO smith;

.. note::

    *   The **GRANT** statement can only grant permission to one object at a time for stored procedures and functions.
    *   The **EXECUTE ON PROCEDURE** operation is used for both stored procedures and functions.
    *   Currently, the **WITH GRANT OPTION** is not supported for granting permissions on stored procedures and functions, and will return a syntax error if used.

.. _revoking-authorization:

REVOKE
======

You can revoke authorization using the **REVOKE** statement. The authorization granted to a user can be revoked at any time. If more than one type of authorization is granted to a user, all or part of the authorization can be revoked. Additionally, even if a single **GRANT** statement is used to grant authorization on multiple database objects to multiple users, selective revocation of authorization for specific users and specific database objects is possible.

If the authorization is revoked from the grantor (**WITH GRANT OPTION**), the authorization granted to the grantee by that grantor is also revoked.

The following **REVOKE** statement can be used to revoke the authorization granted to a user. ::

    (1) For tables and views: 
        REVOKE operation [ { ,operation } ... ] ON [schema_name.]object_name [ { , [schema_name.]object_name } ... ] 
        FROM user [ { ,user } ... ];

    (2) For stored procedures and functions: 
        REVOKE EXECUTE ON PROCEDURE [schema_name.]object_name
        FROM user [ { ,user } ... ];

*   *operation*: Indicates the type of operation that can be revoked (see :ref:`granting-authorization` for details).
*   *schema_name*: Specifies the schema name of the database object. If omitted, the schema name of the current session is used.
*   *object_name*: Specifies the name of the database object for which the authorization is to be revoked.
*   *user*: Specifies the name of the user or group from which the authorization is to be revoked.

The following example shows how to grant **SELECT**, **INSERT**, **UPDATE** and **DELETE** authorization to *smith* and *jones* so that they can perform on the *nation* and *athlete* tables.

.. code-block:: sql

    GRANT SELECT, INSERT, UPDATE, DELETE ON nation, athlete TO smith, jones;

The following example shows how to execute the **REVOKE** statement; this allows *jones* to have only **SELECT** authorization. If *jones* has granted authorization to another user, the user is also allowed to execute **SELECT** only.

.. code-block:: sql

    REVOKE INSERT, UPDATE, DELETE ON nation, athlete FROM jones;

The following example shows how to execute the **REVOKE** statement revoking all authorization that has granted to *smith*. *smith* is not allowed to execute any operations on the *nation* and *athlete* tables once this statement is executed.

.. code-block:: sql

    REVOKE ALL PRIVILEGES ON nation, athlete FROM smith;

.. _change-owner:

ALTER ... OWNER
===============

Database Administrator (**DBA**) or a member of the **DBA** group can change the owner of table, view, trigger, stored functions/procedures, and serial by using the following query. ::

    ALTER (TABLE | CLASS | VIEW | VCLASS | TRIGGER | PROCEDURE | FUNCTION | SERIAL) [schema_name.]name OWNER TO user_id;

*   *schema_name*: Specifies the schema of the object. If omitted, the schema name of the current session is used.
*   *name*: The name of schema object of which owner is to be changed
*   *user_id*: User ID

.. code-block:: sql

    ALTER TABLE test_tbl OWNER TO public;
    ALTER VIEW test_view OWNER TO public;
    ALTER TRIGGER test_trigger OWNER TO public;
    ALTER FUNCTION test_function OWNER TO public;
    ALTER PROCEDURE test_procedure OWNER TO public;
    ALTER SERIAL test_serial OWNER TO public;

.. warning:: 

    When the owner changes, privileges granted by the previous owner are transferred to the new owner.
    **However, privileges on the object that were previously granted to the new owner by the previous owner are automatically revoked.**

.. _authorization-method:

User Authorization Management METHOD
====================================

The database administrator (**DBA**) can check and modify user authorization by calling authorization-related methods defined in **db_user** where information about database user is stored, or **db_authorizations** (the system authorization class). The administrator can specify **db_user** or **db_authorizations** depending on the method to be called, and store the return value of a method to a variable. In addition, some methods can be called only by **DBA** or members of **DBA** group.

.. note:: Note that method call made by the master node is not applied to the slave node in the HA environment.

::

    CALL method_definition ON CLASS auth_class [ TO variable ] [ ; ]
    CALL method_definition ON variable [ ; ]

**login( ) method**

As a class method of **db_user** class, this method is used to change the users who are currently connected to the database. The name and password of a new user to connect are given as arguments, and they must be string type. If there is no password, a blank character ('') can be used as the argument. **DBA** and **DBA** members can call the **login( )** method without a password.

.. code-block:: sql

    -- Connect as DBA user who has no password
    CALL login ('dba', '') ON CLASS db_user;

    -- Connect as a user_1 whose password is cubrid
    CALL login ('user_1', 'cubrid') ON CLASS db_user;

**add_user( ) method**

As a class method of **db_user** class, this method is used to add a new user. The name and password of a new user to add are given as arguments, and they must be string type. At this time, the new user name should not duplicate any user name already registered in a database. The **add_user( )** can be called only by **DBA** or members of **DBA** group.

.. code-block:: sql

    -- Add user_2 who has no password
    CALL add_user ('user_3', '') ON CLASS db_user;

    -- Add user_3 who has no password, and store the return value of a method into an admin variable
    CALL add_user ('user_2', '') ON CLASS db_user to admin;

**drop_user( ) method**

As a class method of **db_user** class, this method is used to drop an existing user. Only the user name to be dropped is given as an argument, and it must be a string type. However, the owner of a class cannot be dropped thus **DBA** needs to specify a new owner of the class before dropping the user. The **drop_user( )** method can be also called only by **DBA** or members of **DBA**.

.. code-block:: sql

    -- Delete user_2
    CALL drop_user ('user_2') ON CLASS db_user;

**find_user( ) method**

As a class method of **db_user** class, this method is used to find a user who is given as an argument. The name of a user to be found is given as an argument, and the return value of the method is stored into a variable that follows 'to'. The stored value can be used in a next query execution.

.. code-block:: sql

    -- Find user_2 and store it into a variable called 'admin'
    CALL find_user ('user_2') ON CLASS db_user to admin;

**set_password( ) method**

This method is an instance method that can call each user instance, and it is used to change a user's password. The new password of a specified user is given as an argument. General users other than **DBA** and **DBA** group members can only change their own passwords.

.. code-block:: sql

    -- Add user_4 and store it into a variable called user_common
    CALL add_user ('user_4','') ON CLASS db_user to user_common;

    -- Change the password of user_4 to 'abcdef'
    CALL set_password('abcdef') on user_common;

**change_owner() method**

As a class method of **db_authorizations** class, this method is used to change the owner of a class. The name of a class for which you want to change the owner, and the name of a new owner are given as arguments. The table name must be prefixed with the schema name. If omitted, the schema name of the current session is used. At this time, the class and owner that are specified as an argument must exist in a database. Otherwise, an error occurs. **change_owner( )** can be called only by **DBA** or members of **DBA** group. The **ALTER ... OWNER** query has the same role as the method. See :ref:`change-owner`.

.. code-block:: sql

    -- Change the owner of table_1 owned by user_1 to user_4
    CALL change_owner ('user_1.table_1', 'user_4') ON CLASS db_authorizations;

The following example shows a **CALL** statement that calls the find_user method defined in the system table **db_user**. It is called to determine whether the database user entered as the **find_user** exists. The first statement calls the table method defined in the **db_user** class. The name (**db_user** in this case) is stored in x if the user is registered in the database. Otherwise, **NULL** is stored.

The second statement outputs the value stored in the variable x. In this query statement, the **DB_ROOT** is a system class that can have only one record. It can be used to output the value of sys_date or other registered variables. For this purpose, the **DB_ROOT** can be replaced by another table having only one record.

.. code-block:: sql

    CALL find_user('dba') ON CLASS db_user to x;
    
::

    Result
    ======================
    db_user
     
.. code-block:: sql

    SELECT x FROM db_root;
    
::

    x
    ======================
    db_user

With **find_user**, you can determine if the user exists in the database depending on whether the return value is **NULL** or not.
