
:meta-keywords: cubrid authorization, cubrid dba, cubrid user
:meta-description: CUBRID has two types of users by default: DBA and PUBLIC. DBA and DBA members can create, drop and alter users by using SQL statements.

***************
User Management
***************

Database User
=============

To know the user name's writing rule, see :doc:`identifier`.

CUBRID has two types of users by default: **DBA** and **PUBLIC**. At initial installation of the product, no password is set.

*   All users have authorization granted to the **PUBLIC** user. All users of the database are automatically the members of **PUBLIC**. Granting authorization to the **PUBLIC** means granting it all users.

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

In CUBRID, the smallest grant unit of authorization is a table. You must grant appropriate authorization to other users (groups) before allowing them to access the table you created.

You don't need to grant authorization individually because the members of the granted group have the same authorization. The access to the (virtual) table created by a **PUBLIC** user is allowed to all users. You can grant access authorization to a user by using the **GRANT** statement. ::

    GRANT operation [{, operation }] ON [schema_name.]table_name [{, [schema_name.]table_name}]
    TO user [{,user }] [WITH GRANT OPTION] ;

* *operation*: Specifies an operation that can be used when granting authorization. The following table shows operations.

    *   **SELECT**: Allows to read the table definitions and retrieve records. The most general type of permissions.
    *   **INSERT**: Allows to create records in the table.
    *   **UPDATE**: Allows to modify the records already existing in the table.
    *   **DELETE**: Allows to delete records in the table.
    *   **ALTER**: Allows to modify the table definition, rename or delete the table.
    *   **INDEX**: Allows to create indexes on columns to speed up searches.
    *   **EXECUTE**: Allows to call table methods or instance methods.
    *   **ALL PRIVILEGES**: Includes all permissions described above.

* *schema_name*: Specifies the schema name of the table or virtual table. If omitted, the schema name of the current session is used.
* *table_name*: Specifies the name of a table or virtual table to be granted.
* *user*: Specifies the name of a user (group) to be granted. Enter the login name of the database user or **PUBLIC**, a system-defined user. If **PUBLIC** is specified, all database users are granted with the permission.
* **WITH GRANT OPTION**: **WITH GRANT OPTION** allows the grantee of authorization to grant that same authorization to another user.

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

The following example shows how to grant retrieving authorization on the *record* and *history* tables to *brown*. Using **WITH GRANT OPTION** allows *brown* to grant retrieving to another users. *brown* can grant authorization to others within his authorization.

.. code-block:: sql

    GRANT SELECT ON record, history TO brown WITH GRANT OPTION;

.. note::

    *   The grantor of authorization must be the owner of all tables listed before the grant operation or have **WITH GRANT OPTION** specified.
    *   Before granting **SELECT**, **UPDATE**, **DELETE** and **INSERT** authorization for a virtual table, the owner of the virtual table must have **SELECT** and **GRANT** authorization for all the tables included in the query specification. The **DBA** user and the members of the **DBA** group are automatically granted all authorization for all tables.
    *   To execute the **TRUNCATE** statement, the **ALTER**, **INDEX**, and **DELETE** authorization is **required**.

.. _revoking-authorization:

REVOKE
======

You can revoke authorization using the **REVOKE** statement. The authorization granted to a user can be revoked anytime. If more than one authorization is granted to a user, all or part of the authorization can be revoked. In addition, if authorization on multiple tables is granted to more than one user using one **GRANT** statement, the authorization can be selectively revoked for specific users and tables.

If the authorization (**WITH GRANT OPTION**) is revoked from the grantor, the authorization granted to the grantee by that grantor is also revoked. ::

    REVOKE operation [{, operation}] ON [schema_name.]table_name [{, [schema_name.]table_name}]
    FROM user [{, user}] ;

*   *operation*: Indicates an operation that can be used when granting authorization (see **Syntax** in :ref:`granting-authorization` for details).
*   *schema_name*: Specifies the schema name of the table or virtual table. If omitted, the schema name of the current session is used.
*   *table_name*: Specifies the name of the table or virtual table to be granted.
*   *user*: Specifies the name of the user (group) to be granted.

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
