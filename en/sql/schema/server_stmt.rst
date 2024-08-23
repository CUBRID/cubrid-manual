
:meta-keywords: server definition, create server, drop server, alter server, rename server
:meta-description: Define servers in CUBRID database using create server, alter server, drop server and rename server statements.


******************************
SERVER DEFINITION STATEMENTS
******************************

CREATE SERVER
=============

Server Definition
------------------

Create remote access information using the **CREATE SERVER** statement.
The created server is used to designate a remote server when executing a :doc:`/sql/query/select` query using **DBLINK**.
For how to use the prepared server, refer to :ref:`dblink-clause`.

::

    CREATE SERVER <dblink_server_name> (<connect_info>) ;
   
        <dblink_server_name> ::= [owner_name.] server_name
		
        <connect_info> ::=
                <connect_info>, <connect_item>
                | <connect_item>						   
        <connect_item> ::= 
                HOST = host_string
                | PORT = port_number 
                | DBNAME = db_name 
                | USER = user_name
                | PASSWORD = [password_string]
                | PROPERTIES = [properties_string] 
                | COMMENT = [server_comment_string]
      
*   *owner_name*: Specifies the name of the owner of the server to be created.
*   *server_name*: Specifies the name of the server to be created. (up to 254 bytes)
*   <*connect_info*>: **HOST**, **PORT**, **DBNAME**, and **USER** are mandatory items in the <connect_item> list of access information.
*   <*connect_item*>: It consists of HOST, PORT, DBNAME, USER, PASSWOED, PROPERTIES, and COMMENT items, and the same item cannot be duplicated.
	
    *   *host_string*: It is the hose name or IP address of the broker server that has DBMS information to be accessed remotely.
    *   *port_number*: The port number of the broker server that has DBMS information to be accessed remotely.
    *   *db_name*: The database name to connect to remotely.
    *   *user_name*: The user name to use when connecting to the database to be accessed remotely.
    *   *password_string*: Password string for *user_name* used to connect to the database to be accessed remotely.
    *   *properties_string*: A string of property information (up to 2047 bytes) used when connecting to a broker (or gateway) for remote database usage. For detailed attribute information, see :ref:`cci-connect-with-url`.
    *   *server_comment_string*: Specifies comments about server information. (up to 1023 bytes)

.. note::

    db_name and user_name can be written in the form of identifier or string literal.

    
    * Example of identifier format
             t123db, "123db", `123db`, [124db]
    * Example of string format
             't123db', '123db'

**Example 1**
The following basic example has the required fields (HOST,PORT,DBNAME,USER) and also contains the PROPERTIES and COMMENT fields.

.. code-block:: sql

    CREATE SERVER dblink_srv1 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dba,
	   PASSWORD='password1234',
	   PROPERTIES='?rcTime=600',
	   COMMENT='this is dblink_srv1'	   
    );

**Example 2**
The following is an example that includes minimal information when creating a server.
It indicates that the remote demodb will be connected to the dev1 account without a password.
srv1, srv2, and srv3 have the same information.

.. code-block:: sql

    CREATE SERVER srv1 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dev1	 
    );
    
   CREATE SERVER srv2 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dev1,
	   PASSWORD=       	 
    );
    
    CREATE SERVER srv3 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dev1,
	   PASSWORD=''       	 
    );

**Example 3**    
The following example specifies the owner when creating the server.
In case of CREATE without designating the owner, the current user becomes the owner.
Later, you can change the owner using the ALTER SERVER statement.
In the example below, the two servers have the same name as *srv2*, but have different owners as *dba* and *cub*, respectively.

.. code-block:: sql

    -- When the current account is dba
    CREATE SERVER srv2 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dev1,
	   PASSWORD='dev1-password',
	   COMMENT='The owner of this server is dba'
    );
    
   CREATE SERVER cub.srv2 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dev2,
	   PASSWORD='dev2-password',
	   COMMENT='The owner of this server is cub.'
    );

**Example 4**
The following is an example of setting up for failover of a remote database by using the altHosts attribute in PROPERTIES when using a remote database configured in an HA environment.
For more information, see :ref:`cci-connect-with-url`.

.. code-block:: sql

    CREATE SERVER dblink_srv1 (
       HOST='192.168.1.8',
       PORT=3300,
       DBNAME=demodb,
       USER=dba,
       PASSWORD='password1234',
       PROPERTIES='?altHosts=192.168.1.9:33000',
       COMMENT='this is dblink_srv1'
    );

**Example 5**
The following is an example of a poor use of the server name (<dblink_server_name>).

.. code-block:: sql
    
    CREATE SERVER srv1 ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    CREATE SERVER "srv 1" ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    CREATE SERVER "srv.1" ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    CREATE SERVER cub.srv1 ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    
    CREATE SERVER "cub"."srv 2" ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    CREATE SERVER [cub].[srv.2] ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
        

In the above example, creation with *"srv.1"* and *[cub].[srv.2]* names fails.
In the case of *cub.srv1*, *cub* will be recognized as the user name and the server name will be recognized as *srv1*.
        
.. note::

    A dot (.'.) cannot be used for the server name. You cannot use a dot ('.') in the server name, even if it is enclosed in quotation marks or [].



RENAME SERVER
=============

You can change the server name using the **RENAME SERVER** syntax.

::

    RENAME SERVER [owner_name.] old_server_name {AS | TO} new_server_name ;
            
        
*   *owner_name*: Specify the owner name of the target server to be renamed.
*   *old_server_name*: Specify the name of the server to be renamed.
*   *new_server_name*: Specifies the name of the server to be newly assigned. (up to 254 bytes)

.. note::

    Only the owner of the server or members of the ownership group can change the information.
    In particular, **DBA** or members of **DBA** can change all server information.
    
    Even after the change, the owners remain the same. To change the owner, refer to :ref:`ALTER SERVER syntax<owner_to>`.


.. code-block:: sql

    -- When the current account is dba
    RENAME SERVER srv1 AS srv2;
    RENAME SERVER dev1.srv1 AS srv3;

..

Even if the above example is performed under the *dba* account, the owner of *srv2* is not changed and is maintained as *cub*, the owner of the *srv1* server.
Also, the owner of the *srv3* server remains *dev1*.


DROP SERVER
===========

Existing servers can be removed using **DROP SERVER** syntax. If the **IF EXISTS** clause is used together, no error occurs even if the server does not exist.

::

    DROP SERVER [IF EXISTS] [owner_name.] server_name  ;
    
*   *owner_name*: Specify the owner name of the server to be removed..
*   *server_name*: Specify the name of the server to be removed.


.. code-block:: sql

    DROP SERVER srv1;
    DROP SERVER cub.srv1;
    DROP SERVER IF EXISTS srv2;
    
    

.. Warning::

    When deleting a user with the DROP USER statement, if there is a server owned by the user, an error is processed and the user is not deleted.
    First, remove the server owned by the user with the DROP SERVER statement, and then delete the user account.
        
::
   
    -- When the current account is dba   
    csql> create user cub;
    Execute OK. (0.000371 sec) Committed.

    1 command(s) successfully processed.
    csql> create server cub.tsrv (HOST='localhost', PORT=3300, DBNAME=demdb, USER=dev1);
    Execute OK. (0.000761 sec) Committed.

    1 command(s) successfully processed.
    csql> drop user cub;

    In the command from line 1,

    ERROR: Cannot drop the user who owns database objects(class/trigger/serial/server etc).

    0 command(s) successfully processed.
    csql> drop server cub.tsrv;
    Execute OK. (0.000761 sec) Committed.

    1 command(s) successfully processed.
    csql> drop user cub;
    Execute OK. (0.001650 sec) Committed.

    1 command(s) successfully processed.
    csql>


..

In the example above, you can see that the drop user *cub* statement is failing while the *tsrv* server owned by the *cub* account is created.
After removing the *cub.tsrv* server, you can see that the *cub* account could be deleted normally..     



.. _alter-server:

ALTER SERVER
=============

You can change the server information by using the **ALTER** statement. You can change the owner of the target server, or update information about HOST, PORT, DBNAME, USER, PASSWOED, PROPERTIES, and COMMENT.

::

    ALTER SERVER <dblink_server_name> <alter_server_list> ;
     
        <dblink_server_name> ::=  [owner_name.] server_name 
        
        <alter_server_list> ::=
                <alter_server_list>, <alter_server_item>
                | <alter_server_item>						   
        <alter_server_item> ::= 
                OWNER TO owner_name
                | CHANGE <connect_item>
        <connect_item> ::= 
                HOST = host_string
                | PORT = port_number 
                | DBNAME = db_name 
                | USER = user_name
                | PASSWORD = [password_string]
                | PROPERTIES = [properties_string] 
                | COMMENT = [server_comment_string]
                

.. note::

    Only the owner of the server or members of the ownership group can change the information.
    In particular, **DBA** or **DBA** members can change all server information.

.. warning::

    It is not possible to update to remove values for HOST, PORT, DBNAME, and USER.


.. _owner_to:

OWNER TO clause
----------------

You can change the owner of the server using the **OWNER TO** clause.

::

    ALTER SERVER [owner_name.] server_name  OWNER TO new_owner_name ;
    
*   *owner_name*: Specifies the owner name of the target server whose owner is to be changed.
*   *server_name*: Specifies the name of the target server whose owner is to be changed.
*   *new_owner_name*: Specifies the new owner name.

.. warning::
    
    *   There is no OWNER TO clause in an ALTER SERVER clause, or it must be specified only once.


.. code-block:: sql
    
    CREATE SERVER srv1 (HOST='broker-server-name', PORT=3300, DBNAME=demodb, USER=dev1);
    ALTER SERVER srv1 OWNER TO usr1;    
    ALTER SERVER usr1.srv1 OWNER TO usr2;    


.. _change-server:

CHANGE clause
----------------

The **CHANGE** section is used to change the values of HOST, PORT, DBNAME, USER, PASSWOED, PROPERTIES, and COMMENT items.

.. warning::
    
    *   Multiple CHANGE clauses in one ALTER SERVER statement can be listed by separating them with commas (,). However, at this time, only one CHANGE clause for the same item should be specified.
    *   Items that were not mentioned when performing the ALTER SERVER syntax do not initialize or delete the value, but retain the existing value. 

::

     ALTER SERVER  [owner_name.] server_name CHANGE <connect_item> [, CHANGE <connect_item>] ... ;

        <connect_item> ::= 
                HOST = host_string
                | PORT = port_number 
                | DBNAME = db_name 
                | USER = user_name
                | PASSWORD = [password_string]
                | PROPERTIES = [properties_string] 
                | COMMENT = [server_comment_string]

*   *owner_name*: Specifies the user name of the server to be created.
*   *server_name*: Specifies the name of the server to be created. (up to 254 bytes)
*   *host_string*: It is the hose name or IP address of the broker server that has DBMS information to be accessed remotely.
*   *port_number*: The port number of the broker server that has DBMS information to be accessed remotely.
*   *db_name*: The database name to connect to remotely.
*   *user_name*: he user name to use when connecting to the database to be accessed remotely.
*   *password_string*: Password string for *user_name* used to connect to the database to be accessed remotely.
*   *properties_string*: Property information string used when connecting to the database to be accessed remotely. (up to 2047 bytes)	
*   *server_comment_string*: Specifies comments about server information. (up to 1023 bytes)


.. code-block:: sql

    CREATE SERVER srv1 ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    
    ALTER SERVER srv1 CHANGE HOST='127.0.0.1';
    ALTER SERVER srv1 CHANGE HOST='127.0.0.1', OWNER TO usr1;
    ALTER SERVER srv1 CHANGE USER=dev2, CHANGE PASSWORD='dev2-pawword', CHANGE PORT=3500;

..

The above example shows that the **CHANGE** clause can be used to list multiple items at once, or it can be used together with the **OWNER TO** clause. 


.. code-block:: sql
     
    ALTER SERVER srv1 CHANGE PORT=;    
    ALTER SERVER srv1 CHANGE DBNAME=;    
    ALTER SERVER srv1 CHANGE USER=;
    ALTER SERVER srv1 CHANGE HOST=;
    ALTER SERVER srv1 CHANGE HOST='';
    
..

All of the above examples are examples of not supported cases. Since HOST, PORT, DBNAME, and USER, which are essential elements in the configuration of server information, must have values, setting changes that delete values are not supported. In particular, in the case of HOST, setting it to an empty string is also not allowed.
    
    
.. code-block:: sql
    
    ALTER SERVER srv1 CHANGE PASSWORD=;
    ALTER SERVER srv1 CHANGE PASSWORD='';
    
    ALTER SERVER srv1 CHANGE PROPERTIES=;
    ALTER SERVER srv1 CHANGE PROPERTIES='';
    
    ALTER SERVER srv1 CHANGE COMMENT=;
    ALTER SERVER srv1 CHANGE COMMENT='';
    
..

The above examples are all supported examples. PASSWORD, PROPERTIES, COMMENT, which are not essential elements in the configuration of server information, do not necessarily have a value, so it is possible to change the setting to delete the value. 
    
  
