A value that specifies the optimization level. It has the following meanings.
:meta-keywords: csql, cubrid csql, cubrid commands, executing csql, csql options
:meta-description: CUBRID CSQL is an application that allows users to use SQL statements through a command-driven interface. This section briefly explains how to use the CSQL Interpreter and associated commands.

****************
CSQL Interpreter
****************

To execute SQL statements in CUBRID, you need to use either a Graphical User Interface(GUI)-based CUBRID Manager or a console-based CSQL Interpreter.

CSQL is an application that allows users to use SQL statements through a command-driven interface. This section briefly explains how to use the CSQL Interpreter and associated commands.

.. _csql-intro:

Introduction to the CSQL Interpreter
====================================

**A Tool for SQL**

The CSQL Interpreter is an application installed with CUBRID that allows you to execute in an interactive or batch mode and viewing query results. The CSQL Interpreter has a command-line interface. With this, you can store SQL statements together with their results to a file for a later use.

The CSQL Interpreter provides the best and easiest way to use CUBRID. You can develop database applications with various APIs (e.g. JDBC, ODBC, PHP, CCI, etc.; you can use the CUBRID Manager, which is a management and query tool provided by CUBRID. With the CSQL Interpreter, users can create and retrieve data in a terminal-based environment.

The CSQL Interpreter directly connects to a CUBRID database and executes various tasks using SQL statements. Using the CSQL Interpreter, you can:

*   Retrieve, update and delete data in a database by using SQL statements
*   Execute external shell commands
*   Store or display query results
*   Create and execute SQL script files
*   Select table schema
*   Retrieve or modify parameters of the database server system
*   Retrieve database information (e.g. schema, triggers, queued triggers, workspaces, locks, and statistics)

**A Tool for DBA**

A database administrator(**DBA**) performs administrative tasks by using various administrative utilities provided by CUBRID; a terminal-based interface of CSQL Interpreter is an environment where **DBA** executes administrative tasks.

It is also possible to run the CSQL Interpreter in standalone mode. In this mode, the CSQL Interpreter directly accesses database files and executes commands including server process properties. That is, SQL statements can be executed to a database without running a separate database server process. The CSQL Interpreter is a powerful tool that allows you to use the database only with a **csql** utility, without any other applications such as the database server or the brokers.

Executing CSQL
==============

.. _csql-exec-mode:

CSQL Execution Mode
-------------------

**Interactive Mode**

With CSQL Interpreter, you can enter and execute SQL statements to handle schema and data in the database. Enter statements in a prompt that appears when running the **csql** utility. After executing the statements, the results are listed in the next line. This is called the interactive mode.

**Batch Mode**

You can store SQL statements in a file and execute them later to have the **csql** utility read the file. This is called the batch mode..

**Standalone Mode**

In the standalone mode, CSQL Interpreter directly accesses database files and executes commands including server process functions. That is, SQL statements can be sent and executed to a database without a separate database server process running for the task. Since the standalone mode allows only one user access at a given time, it is suitable for management tasks by Database Administrators (**DBAs**).

**Client/Server Mode**

CSQL Interpreter usually operates as a client process and accesses the server process.

**System Administration Mode**

You can use this mode when you  run checkpoint through CSQL interpreter or exit the transaction monitoring. Also, it allows one connection on CSQL interpreter even if the server  access count exceeds the value of **max_clients** system parameter. In this mode, allowed connection count by CSQL interpreter is only one.

::

    csql -u dba --sysadm demodb

Using CSQL (Syntax)
-------------------

**Connecting to Local Host**

Execute the CSQL Interpreter using the **csql** utility. You can set options as needed. To set the options, specify the name of the database to connect to as a parameter. The following is a **csql** utility statement to access the database on a local server: ::

    csql [options] database_name

**Connecting to Remote Host**

The following is a **csql** utility syntax to access the database on a remote host: ::

    csql [options] database_name@remote_host_name

Make sure that the following conditions are met before you run the CSQL Interpreter on a remote host.

*   The CUBRID installed on the remote host must be the same version as the one on the local host.
*   The port number used by the master process on the remote host must be identical to the one on the local host.
*   You must access the remote host in client/server mode using the **-C** option.

**Example**

The following example shows how to access the **demodb** database on the remote host with the IP address 192.168.1.3 and calls the **csql** utility. ::

    csql -C demodb@192.168.1.3

CSQL Options
------------

To display the option list in the prompt, execute the **csql** utilities without specifying the database name as follows: ::

    $ csql
    A database-name is missing.
    interactive SQL utility, version 11.3
    usage: csql [OPTION] database-name[@host]

    valid options:
      -S, --SA-mode                standalone mode execution
      -C, --CS-mode                client-server mode execution
      -u, --user=ARG               alternate user name
      -p, --password=ARG           password string, give "" for none
      -e, --error-continue         don't exit on statement error
      -i, --input-file=ARG         input-file-name
      -o, --output-file=ARG        output-file-name
      -s, --single-line            single line oriented execution
      -c, --command=ARG            CSQL-commands
      -l, --line-output            display each value in a line
      -r, --read-only              read-only mode
      -t, --plain-output           display results in a script-friendly format (only works with -c and -i)
      -q, --query-output           display results in a query-friendly format (only work with -c and -i)
      -d, --loaddb-output          display results in a loaddb-friendly format (only work with -c and -i)
      -N, --skip-column-names      do not display column names in results (only works with -c and -i)
          --string-width           display each column which is a string type in this width
          --no-auto-commit         disable auto commit mode execution
          --no-pager               do not use pager
          --no-single-line         turn off single line oriented execution
          --no-trigger-action      disable trigger action
          --delimiter=ARG          delimiter between columns (only work with -q)
          --enclosure=ARG          enclosure for a result string (only work with -q)		  

    For additional information, see http://www.cubrid.org

**Options**

.. program:: csql

.. option:: -S, --SA-mode

    The following example shows how to connect to a database in standalone mode and execute the **csql** utility. If you want to use the database exclusively, use the **-S** option. If **csql** is running in standalone mode, it is impossible to use another **csql** or utility. If both **-S** and **-C** options are omitted, the **-C** option will be specified. ::

        csql -S demodb

.. option:: -C, --CS-mode

    The following example shows how to connect to a database in client/server mode and execute the **csql** utility. In an environment where multiple clients connect to the database, use the **-C** option. Even when you connect to a database on a remote host in client/server mode, the error log created during **csql** execution will be stored in the **cub.err** file on the local host. ::

        csql -C demodb

.. option:: -i, --input-file=ARG

    The following example shows how to specify the name of the input file that will be used in a batch mode with the **-i** option. In the **infile** file, more than one SQL statement is stored. Without the **-i** option specified, the CSQL Interpreter will run in an interactive mode. ::

        csql -i infile demodb

.. option:: -o, --output-file=ARG

    The following example shows how to store the execution results to the specified file instead of displaying on the screen. It is useful to retrieve the results of the query performed by the CSQL Interpreter afterwards. ::

        csql -o outfile demodb

.. option:: -u, --user=ARG

    The following example shows how to specify the name of the user that will connect to the specified database with the **-u** option. If the **-u** option is not specified, **PUBLIC** that has the lowest level of authorization will be specified as a user. If the user name is not valid, an error message is displayed and the **csql** utility is terminated. If there is a password for the user name you specify, you will be prompted to enter the password. ::

        csql -u DBA demodb

.. option:: -p, --password=ARG

    The following example shows how to enter the password of the user specified with the **-p** option. Especially since there is no prompt to enter a password for the user you specify in a batch mode, you must enter the password using the **-p** option. When you enter an incorrect password, an error message is displayed and the **csql** utility is terminated. ::

        csql -u DBA -p *** demodb

.. option:: -s, --single-line

    As an option used with the **-i** option, it executes multiple SQL statement one by one in a file with the **-s** option. This option is useful to allocate less memory for query execution and each SQL statement is separated by semicolons (;). If it is not specified, multiple SQL statements are retrieved and executed at once. ::

        csql -s -i infile demodb

.. option:: -c, --command=ARG

    The following example shows how to execute more than one SQL statement from the shell with the **-c** option. Multiple statements are separated by semicolons (;). ::

        csql -c "select * from olympic;select * from stadium" demodb

.. option:: -l, --line-output

    With **-l** option, you can display the values of SELECT lists by line. If **-l** option is omitted, all SELECT lists of the result record are displayed in one line. ::

        csql -l demodb

.. option:: -e, --error-continue 

    The following example shows how to ignore errors and keep execution even though semantic or runtime errors occur with the **-e** option. However, if any SQL statements have syntax errors, query execution stops after errors occur despite specifying the **-e** option. ::

        $ csql -e demodb

        csql> SELECT * FROM aaa;SELECT * FROM athlete WHERE code=10000;

        In line 1, column 1,

        ERROR: before ' ;SELECT * FROM athlete WHERE code=10000; '
        Unknown class "aaa".


        === <Result of SELECT Command in Line 1> ===

                 code  name                  gender                nation_code           event               
        =====================================================================================================
                10000  'Aardewijn Pepijn'    'M'                   'NED'                 'Rowing'            


        1 rows selected. (0.006433 sec) Committed.

.. option:: -r, --read-only

    You can connect to the read-only database with the **-r** option. Retrieving data is only allowed in the read-only database; creating databases and entering data are not allowed. ::

        csql -r demodb

.. option:: -t, --plain-output

    It only shows column names and values and works with **-c** or **-i** option. Each column and value is separated by a tab and a new line, a tab and a backslash which are included in results are replaced by '\n', '\t' and '\\' for each. This option is ignored when it is given with **-l** option. 
    
    ::
    
        $ csql demodb -c "select * from athlete where code between 12762 and 12765" -t

        code    name    gender  nation_code event
        12762   O'Brien Dan M   USA Athletics
        12763   O'Brien Leah    W   USA Softball
        12764   O'Brien Shaun William   M   AUS Cycling
        12765   O'Brien-Amico Leah  W   USA Softball
 
.. option:: -q, --query-output

    This option displays the result for easy use in insert queries, only show column names and values, and works with **-c** or **-i** option. Each column name and value are separated by a comma or a single character of the **\-\-delimiter** option; and all results except for numeric types are enclosed by a single quote or a single character of the **\-\-enclosure** option. If the enclosure is a single quote, the single quote in the results is replaced with two ones. It is ignored when it is given with **-l** option.

    ::

        $ csql demodb -c "select * from athlete where code between 12762 and 12765" -q

        code,name,gender,nation_code,event
        12762,'O''Brien Dan','M','USA','Athletics'
        12763,'O''Brien Leah','W','USA','Softball'
        12764,'O''Brien Shaun William','M','AUS','Cycling'
        12765,'O''Brien-Amico Leah','W','USA','Softball'

    ::

        $ csql demodb -c "select * from athlete where code between 12762 and 12765" -q --delimiter="" --enclosure="\""

        code,name,gender,nation_code,event
        12762,"O'Brien Dan","M","USA","Athletics"
        12763,"O'Brien Leah","W","USA","Softball"
        12764,"O'Brien Shaun William","M","AUS","Cycling"
        12765,"O'Brien-Amico Leah","W","USA","Softball"

.. option:: -d, --loaddb-output

    This option displays the result for easy use in loaddb utility, only show column names and values, and works with **-c** or **-i** option. Each column name and value are separated by a space; and all results except for numeric types are enclosed by a single quote. The single quote in the results is replaced with two ones and the result of the enum type is outputted by an index instead of a value. This opton is ignored when it is given with **-l** option.

    ::

        $ csql demodb -c "select * from athlete where code between 12762 and 12765" -d

        %class [ ] ([code] [name] [gender] [nation_code] [event])
        12762 'O''Brien Dan' 'M' 'USA' 'Athletics'
        12763 'O''Brien Leah' 'W' 'USA' 'Softball'
        12764 'O''Brien Shaun William' 'M' 'AUS' 'Cycling'
        12765 'O''Brien-Amico Leah' 'W' 'USA' 'Softball'

.. option:: -N, --skip-column-names
 
    It will hide column names from the results. It only works with **-c** or **-i** option and is usually used with **-t** **-q** or **-d** option. This option is ignored when it is given with **-l** option. 
 
    ::

        $ csql demodb -c "select * from athlete where code between 12762 and 12765" -d -N

        12762 'O''Brien Dan' 'M' 'USA' 'Athletics'
        12763 'O''Brien Leah' 'W' 'USA' 'Softball'
        12764 'O''Brien Shaun William' 'M' 'AUS' 'Cycling'
        12765 'O''Brien-Amico Leah' 'W' 'USA' 'Softball'

.. option:: --no-auto-commit

    The following example shows how to stop the auto-commit mode with the **\-\-no-auto-commit** option. If you don't configure **\-\-no-auto-commit** option, the CSQL Interpreter runs in an auto-commit mode by default, and the SQL statement is committed automatically at every execution. Executing the **;AUtocommit** session command after starting the CSQL Interpreter will also have the same result. ::

        csql --no-auto-commit demodb

.. option:: --no-pager

    The following example shows how to display the execution results by the CSQL Interpreter at once instead of page-by-page with the **\-\-no-pager** option. The results will be output page-by-page if **\-\-no-pager** option is not specified. ::

        csql --no-pager demodb

.. option:: --no-single-line

    The following example shows how to keep storing multiple SQL statements and execute them at once with the **;xr** or **;ru** session command. If you do not specify this option, SQL statements are executed without **;xr** or **;ru** session command. ;Singleline command will also have the same result. ::

        csql --no-single-line demodb

.. option:: --sysadm

    This option should be used together with **-u dba**. It is specified when you want to run CSQL in a system administrator's mode.

    ::

        csql -u dba --sysadm demodb

.. option:: --write-on-standby

    This option should be used together with a system administrator's mode option(**\-\-sysadm**). dba which run CSQL with this option can write directly to the standby DB (slave DB or replica DB). However, the data to be written directly to the replica DB are not replicated.

    :: 

         csql --sysadm --write-on-standby -u dba testdb@localhost 
    
    .. note::
    
        Please note that replication mismatch occurs when you write the data directly to the replica DB.

.. option:: --no-trigger-action

    If you specify this option, triggers of the queries executed in this CSQL are not triggered.

.. option:: --delimiter=ARG

    This option should be used together with **-q** and a single character is specified in the argument to separate the column name and value. If multiple characters are specified, the first character is used without displaying an error. (include special characters such as \\t and \\n) 

.. option:: --enclosure=ARG

    This option should be used together with **-q** and a single character is specified in the argument to enclose all values except for numeric types. If multiple characters are specified, the first character is used without displaying an error.

.. _csql-session-commands:

Session Commands
================

In addition to SQL statements, CSQL Interpreter provides special commands allowing you to control the Interpreter. These commands are called session commands. All the session commands must start with a semicolon (;).

Enter the **;help** command to display a list of the session commands available in the CSQL Interpreter. Note that only the uppercase letters of each session command are required to make the CSQL Interpreter to recognize it. Session commands are not case-sensitive.

"Query buffer" is a buffer to store the query before running it. If you run CSQL as giving the **\-\-no-single-line** option, the query string is kept on the buffer until running **;xr** command.

**Reading SQL statements from a file (;REAd)**

The **;REAd** command reads the contents of a file into the buffer. This command is used to execute SQL commands stored in the specified file. To view the contents of the file loaded into the buffer, use the **;List** command. ::

    csql> ;read nation.sql
    The file has been read into the command buffer.
    csql> ;list
    insert into "sport_event" ("event_code", "event_name", "gender_type", "num_player") values
    (20001, 'Archery Individual', 'M', 1);
    insert into "sport_event" ("event_code", "event_name", "gender_type", "num_player") values
    20002, 'Archery Individual', 'W', 1);
    ....

**Storing SQL statements into a file (;Write)**

The **;Write** command stores the contents of the query buffer into a file. This command is used to store queries that you entered or modified in the CSQL Interpreter. ::

    csql> ;write outfile
    Command buffer has been saved.

**Appending to a file (;APpend)**

This command appends the contents of the current query buffer to an **outfile** file. ::

    csql> ;append outfile
    Command buffer has been saved.

**Executing a shell command (;SHELL)**

The **;SHELL** session command calls an external shell. Starts a new shell in the environment where the CSQL Interpreter is running. It returns to the CSQL Interpreter when the shell terminates. If the shell command to execute with the **;SHELL_Cmd** command has been specified, it starts the shell, executes the specified command, and returns to the CSQL Interpreter. ::

    csql> ;shell
    % ls -al
    total 2088
    drwxr-xr-x 16 DBA cubrid   4096 Jul 29 16:51 .
    drwxr-xr-x  6 DBA cubrid   4096 Jul 29 16:17 ..
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 02:49 audit
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 16:17 bin
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 16:17 conf
    drwxr-xr-x  4 DBA cubrid   4096 Jul 29 16:14 cubridmanager
    % exit
    csql>

**Registering a shell command (;SHELL_Cmd)**

The **;SHELL_Cmd** command registers a shell command to execute with the **SHELL** session command. As shown in the example below, enter the **;shell** command to execute the registered command. ::

    csql> ;shell_c ls -la
    csql> ;shell
    total 2088
    drwxr-xr-x 16 DBA cubrid   4096 Jul 29 16:51 .
    drwxr-xr-x  6 DBA cubrid   4096 Jul 29 16:17 ..
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 02:49 audit
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 16:17 bin
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 16:17 conf
    drwxr-xr-x  4 DBA cubrid   4096 Jul 29 16:14 cubridmanager
    csql>

The OS environment variable(CUBRID_CSQL_SHELL or SHELL; CUBRID_CSQL_SHELL is used if both environment variables are set) may be registered and used in advance. ::
  
   $ export CUBRID_CSQL_SHELL='ls -la'
   or
   $ export SHELL='ls -la'

**Registering a pager command (;PAger_cmd)**

The ;PAger_cmd command registers a pager command to display the query result. The way of displaying is decided by the registered command. The default is **more**. Also **cat** and **less** can be used. But ;Pager_cmd command works well only on Linux.

When you register pager command as more, the query result shows by page and wait until you press the space key. ::

    csql>;pager more
    
When you register pager command as cat, the query result shows all in one display without paging. ::

    csql>;pager cat

When you redirect the output with a file, the total query result will be written on the file. ::

    csql>;pager cat > output.txt

If you register pager command as less, you can forward, backward the query result. Also pattern matching on the query result is possible. ::

    csql>;pager less
    
The keyboard commands used on the **less** are as follows.

* Page UP, b: go up to one page. (backwording)

* Page Down, Space: go down to one page (forwarding)

* /string: find a sting on the query results

* n: find the next string

* N: find the previous string

* q: quit the paging mode.
    
**Changing the current working directory (;CD)**

This command changes the current working directory where the CSQL Interpreter is running to the specified directory. If you don't specify the path, the directory will be changed to the home directory. ::

    csql> ;cd /home1/DBA/CUBRID
    Current directory changed to  /home1/DBA/CUBRID.

**Exiting the CSQL Interpreter (;EXit)**

This command exits the CSQL Interpreter. ::

    csql> ;ex

**Clearing the query buffer (;CLear)**

The **;CLear** session command clears the contents of the query buffer. ::

    csql> ;clear
    csql> ;list

**Displaying the contents of the query buffer (;List)**

The **;List** session command lists the contents of the query buffer that have been entered or modified. The query buffer can be modified by **;READ** or **;Edit** command. ::

    csql> ;list

**Executing SQL statements (;RUn)**

This command executes SQL statements in the query buffer. Unlike the **;Xrun** session command described below, the buffer will not be cleared even after the query execution. ::

    csql> ;run

**Clearing the query buffer after executing the SQL statement (;Xrun)**

This command executes SQL statements in the query buffer. The buffer will be cleared after the query execution. ::

    csql> ;xrun

**Committing transaction (;COmmit)**

This command commits the current transaction. You must enter a commit command explicitly if it is not in auto-commit mode. In auto-commit mode, transactions are automatically committed whenever SQL is executed. ::

    csql> ;commit
    Execute OK. (0.000192 sec)
    
**Rolling back transaction (;ROllback)**

This command rolls back the current transaction. Like a commit command (**;COmmit**), it must enter a rollback command explicitly if it is not in auto-commit mode (**OFF**). ::

    csql> ;rollback
    Execute OK. (0.000166 sec)

**Setting the auto-commit mode (;AUtocommit)**

This command sets auto-commit mode to **ON** or **OFF**. If any value is not specified, current configured value is applied by default. The default value is **ON**. ::

    csql> ;autocommit off
    AUTOCOMMIT IS OFF

**CHeckpoint Execution (;CHeckpoint)**

This command executes the checkpoint within the CSQL session. This command can only be executed when a DBA group member, who is specified for the custom option (**-u** *user_name*), connects to the CSQL Interpreter in system administrator mode (**\-\-sysadm**).

**Checkpoint** is an operation of flushing all dirty pages except for temp pages within the current data buffer to disks. You can also change the checkpoint interval using a command (**;set** *parameter_name* value) to set the parameter values in the CSQL session. You can see the examples of the parameter related to the checkpoint execution interval (**checkpoint_interval** and **checkpoint_every_size**). For more information, see :ref:`logging-parameters`. ::

    sysadm> ;checkpoint
    Checkpoint has been issued.

**Transaction Monitoring Or Termination (;Killtran)**

This command checks the transaction status information or terminates a specific transaction in the CSQL session. This command prints out the status information of all transactions on the screen if a parameter is omitted it terminates the transaction if a specific transaction ID is specified for the parameter. It can only be executed when a DBA group member, who is specified for the custom option (**-u** *user_name*), connects to the CSQL Interpreter in system administrator mode (**\-\-sysadm**). ::

    sysadm> ;killtran
    Tran index      User name      Host name      Process id      Program name
    -------------------------------------------------------------------------------
          1(+)            dba      myhost             664           cub_cas
          2(+)            dba      myhost            6700              csql
          3(+)            dba      myhost            2188           cub_cas
          4(+)            dba      myhost             696              csql
          5(+)         public      myhost            6944              csql
     
    sysadm> ;killtran 3
    The specified transaction has been killed.

**Restarting database (;REStart)**

A command that tries to reconnect to the target database in a CSQL session. Note that when you execute the CSQL Interpreter in CS (client/server) mode, it will be disconnected from the server. When the connection to the server is lost due to a HA failure and failover to another server occurs, this command is particularly useful in connecting to the switched server while maintaining the current session. ::

    csql> ;restart
    The database has been restarted.

**Displaying the current date (;DATE)**

The **;DATE** command displays the current date and time in the CSQL Interpreter. ::

    csql> ;date
         Tue July 29 18:58:12 KST 2008

**Displaying the database information (;DATAbase)**

This command displays the database name and host name where the CSQL Interpreter is working. If the database is running, the HA mode (one of those following: active, standby, or maintenance) will be displayed as well.  ::

    csql> ;database
         demodb@cubridhost (active)

**Displaying schema information of a class (;SChema)**

The **;SChema** session command displays schema information of the specified table. The information includes the table name, its column name and constraints. ::

    csql> ;schema event
    === <Help: Schema of a Class> ===
     <Class Name>
         event
     <Attributes>
         code           INTEGER NOT NULL
         sports         CHARACTER VARYING(50)
         name           CHARACTER VARYING(50)
         gender         CHARACTER(1)
         players        INTEGER
     <Constraints>
         PRIMARY KEY pk_event_event_code ON event (code)

**Displaying the trigger (;TRigger)**

This command searches and displays the trigger specified. If there is no trigger name specified, all the triggers defined will be displayed. ::

    csql> ;trigger
    === <Help: All Triggers> ===
        trig_delete_contents

**Checking the parameter value(;Get)**

You can check the parameter value currently set in the CSQL Interpreter using the **;Get** session command. An error occurs if the parameter name specified is incorrect. ::

    csql> ;get isolation_level
    === Get Param Input ===
    isolation_level="tran_rep_class_commit_instance"

**Setting the parameter value (;SET)**

You can use the **;Set** session command to set a specific parameter value. Note that changeable parameter values are only can be changed. To change the server parameter values, you must have DBA authorization. For information on list of changeable parameters, see :ref:`broker-configuration`. ::

    csql> ;set block_ddl_statement=1
    === Set Param Input ===
    block_ddl_statement=1

    -- Dynamically change the log_max_archives value in the csql accessed by dba account
    csql> ;set log_max_archives=5

**Setting the output width of string (;STring-width)** 

You can use the **;STring-width** command to set the output width of character string or BIT string.

**;string-width** session command without a length shows the current setting length. When it is set to 0, the columns will be displayed as it is. If it sets greater than 0, the string typed columns will be displayed with the specified length. 

::

    csql> SELECT name FROM NATION WHERE NAME LIKE 'Ar%';
      'Arab Republic of Egypt'
      'Aruba'
      'Armenia'
      'Argentina'

    csql> ;string-width 5
    csql>  SELECT name FROM NATION WHERE NAME LIKE 'Ar%';
      'Arab '
      'Aruba'
      'Armen'
      'Argen'

    csql> ;string-width
    STRING-WIDTH : 5

**Setting the output width of the column (;COLumn-width)**

You can use the **;COLumn-width** command to set the output width regardless of its data types.
If you don't give a value after **;COL** command, it shows the current setting length. When it sets to 0, the columns will be displayed as it is. If it sets to greater than 0, the columns will be displayed with the specified length. ::

    csql> CREATE TABLE tbl(a BIGINT, b BIGINT);
    csql> INSERT INTO tbl VALUES(12345678890, 1234567890)
    csql> ;column-width a=5
    csql> SELECT * FROM tbl;
          12345            1234567890
    csql> ;column-width
    COLUMN-WIDTH a : 5

**Setting the view level of executing query plan (;PLan)**

You can use the **;PLan** session command to set the view level of executing query plan the level is composed of **simple**, **detail**, and **off**. Each command refers to the following:

*   **off**: Not displaying the query execution plan (OPT LEVEL=1)
*   **simple**: Displaying the query execution plan in simple version (OPT LEVEL=257)
*   **detail**: Displaying the query execution plan in detailed version (OPT LEVEL=513)

For more information. see :ref:`viewing-query-plan`.

.. _set-autotrace:
 
**Setting SQL trace(;trace)**
 
The **;trace** session command specifies if SQL trace result is printed out together with query result or not.
When you set SQL trace ON by using this command, the result of query profiling is automatically shown even if you do not run "**SHOW TRACE**;" syntax.

For more information, see :ref:`query-profiling`.
 
The command format is as follows.
 
::
 
    ;trace {on | off} [{text | json}]
 
*   **on**: set on SQL trace.
*   **off**: set off SQL trace.
*   **text**: print out as a general TEXT format. If you omit OUTPUT clause, TEXT format is specified.
*   **json**: print out as a JSON format.

.. note:: CSQL interpreter which is run in the standalone mode(use -S option) does not support SQL trace feature.
    
**Displaying information (;Info)**

The **;Info** session command allows you to check information such as schema, triggers, the working environment, locks and statistics. ::

    csql> ;info lock
    *** Lock Table Dump ***
     Lock Escalation at = 100000, Run Deadlock interval = 1
    Transaction (index  0, unknown, unknown@unknown|-1)
    Isolation COMMITTED READ
    State TRAN_ACTIVE
    Timeout_period -1
    ......

.. _csql-execution-statistics:

**Dumping CSQL execution statistics information(;.Hist)**

This command is a CSQL session command for starting to collect the statistics information in CSQL. The information is collected only for the currently connected CSQL after "**;.Hist on**" command is entered. Following options are provided for this session command.

*   **on**: Starts collecting statistics information for the current connection.
*   **off**: Stops collecting statistics information of server.

This command is executable while the **communication_histogram** parameter in the **cubrid.conf** file is set to **yes**. You can also view this information by using the **cubrid statdump** utility. 

After running "**;.Hist on**", the execution commands such as **;.dump_hist** or **;.x** must be entered to output the statistics information. After **;.dump_hist** or **;.x**, all accumulated data are dumped and initiated.

As a reference, you should use **cubrid statdump** utility to check all queries' statistics information of a database server.

This example shows the server statistics information for current connection. For information on dumped specific items or **cubrid statdump** command, see :ref:`statdump`.

::

    csql> ;set communication_histogram=yes
    csql> ;.hist on
    csql> SELECT name FROM nation LIMIT 5;
    csql> ;.x
    Histogram of client requests:
    Name                            Rcount   Sent size  Recv size , Server time
    LC_FETCHALL                        2 X         72+         0 b,   0.000000 s
    LC_DOESEXIST                       3 X         84+        36 b,   0.000142 s
    LC_FETCH_LOCKHINT_CLASSES          1 X         56+         0 b,   0.000000 s
    BTREE_GET_KEY_TYPE                 5 X        180+        60 b,   0.000111 s
    QM_QUERY_DROP_ALL_PLANS            1 X        224+        40 b,   0.000080 s
    QM_QUERY_DUMP_PLANS                1 X         72+       308 b,   0.000355 s
    ---------------------------------------------------------------------------
    Totals:                           13 X        688+       444 b    0.001415 s

    Average server response time = 0.000109 secs
    Average time between client requests = 0.000000 secs

    *** CLIENT EXECUTION STATISTICS ***
    System CPU (sec)              =          0
    User CPU (sec)                =          0
    Elapsed (sec)                 =         20

    *** SERVER EXECUTION STATISTICS ***
    Num_file_creates              =          0
    Num_file_removes              =          0
    Num_file_ioreads              =          0
    Num_file_iowrites             =          0
    Num_file_iosynches            =          0
    The timer values for file_iosync_all are:
    Num_file_iosync_all           =          0
    Total_time_file_iosync_all    =          0
    Max_time_file_iosync_all      =          0
    Avg_time_file_iosync_all      =          0
    Num_file_page_allocs          =          0
    Num_file_page_deallocs        =          0
    Num_data_page_fetches         =         58
    Num_data_page_dirties         =          0
    Num_data_page_ioreads         =          0
    Num_data_page_iowrites        =          0
    Num_data_page_flushed         =          0
    Num_data_page_private_quota   =        551
    Num_data_page_private_count   =          8
    Num_data_page_fixed           =          1
    Num_data_page_dirty           =          0
    Num_data_page_lru1            =         22
    Num_data_page_lru2            =         14
    Num_data_page_lru3            =          8
    Num_data_page_victim_candidate =          8
    Num_log_page_fetches          =          0
    Num_log_page_ioreads          =          0
    Num_log_page_iowrites         =          0
    Num_log_append_records        =          0
    Num_log_archives              =          0
    Num_log_start_checkpoints     =          0
    Num_log_end_checkpoints       =          0
    Num_log_wals                  =          0
    Num_log_page_iowrites_for_replacement =          0
    Num_log_page_replacements     =          0
    Num_page_locks_acquired       =          0
    Num_object_locks_acquired     =          4
    Num_page_locks_converted      =          0
    Num_object_locks_converted    =          2
    Num_page_locks_re-requested   =          0
    Num_object_locks_re-requested =         11
    Num_page_locks_waits          =          0
    Num_object_locks_waits        =          0
    Num_object_locks_time_waited_usec =          0
    Num_tran_commits              =          1
    Num_tran_rollbacks            =          0
    Num_tran_savepoints           =          0
    Num_tran_start_topops         =          0
    Num_tran_end_topops           =          0
    Num_tran_interrupts           =          0
    Num_tran_postpone_cache_hits  =          0
    Num_tran_postpone_cache_miss  =          0
    Num_tran_topop_postpone_cache_hits =          0
    Num_tran_topop_postpone_cache_miss =          0
    Num_btree_inserts             =          0
    Num_btree_deletes             =          0
    Num_btree_updates             =          0
    Num_btree_covered             =          0
    Num_btree_noncovered          =          0
    Num_btree_resumes             =          0
    Num_btree_multirange_optimization =          0
    Num_btree_splits              =          0
    Num_btree_merges              =          0
    Num_btree_get_stats           =          0
    The timer values for btree_online_load are:
    Num_btree_online_load         =          0
    Total_time_btree_online_load  =          0
    Max_time_btree_online_load    =          0
    Avg_time_btree_online_load    =          0
    The timer values for btree_online_insert_task are:
    Num_btree_online_insert_task  =          0
    Total_time_btree_online_insert_task =          0
    Max_time_btree_online_insert_task =          0
    Avg_time_btree_online_insert_task =          0
    The timer values for btree_online_prepare_task are:
    Num_btree_online_prepare_task =          0
    Total_time_btree_online_prepare_task =          0
    Max_time_btree_online_prepare_task =          0
    Avg_time_btree_online_prepare_task =          0
    The timer values for btree_online_insert_leaf are:
    Num_btree_online_insert_leaf  =          0
    Total_time_btree_online_insert_leaf =          0
    Max_time_btree_online_insert_leaf =          0
    Avg_time_btree_online_insert_leaf =          0
    Num_btree_online_inserts      =          0
    Num_btree_online_inserts_same_page_hold =          0
    Num_btree_online_inserts_retry =          0
    Num_btree_online_inserts_retry_nice =          0
    Num_query_selects             =          1
    Num_query_inserts             =          0
    Num_query_deletes             =          0
    Num_query_updates             =          0
    Num_query_sscans              =          1
    Num_query_iscans              =          0
    Num_query_lscans              =          0
    Num_query_setscans            =          0
    Num_query_methscans           =          0
    Num_query_nljoins             =          0
    Num_query_mjoins              =          0
    Num_query_objfetches          =          0
    Num_query_holdable_cursors    =          0
    Num_sort_io_pages             =          0
    Num_sort_data_pages           =          0
    Num_network_requests          =         14
    Num_adaptive_flush_pages      =          0
    Num_adaptive_flush_log_pages  =          0
    Num_adaptive_flush_max_pages  =          0
    Num_prior_lsa_list_size       =          0
    Num_prior_lsa_list_maxed      =          0
    Num_prior_lsa_list_removed    =          0
    Time_ha_replication_delay     =          0
    Num_plan_cache_add            =          0
    Num_plan_cache_lookup         =          2
    Num_plan_cache_hit            =          2
    Num_plan_cache_miss           =          0
    Num_plan_cache_full           =          0
    Num_plan_cache_delete         =          0
    Num_plan_cache_invalid_xasl_id =          0
    Num_plan_cache_entries        =          1
    Num_vacuum_log_pages_vacuumed =          0
    Num_vacuum_log_pages_to_vacuum =          0
    Num_vacuum_prefetch_requests_log_pages =          0
    Num_vacuum_prefetch_hits_log_pages =          0
    Num_heap_home_inserts         =          0
    Num_heap_big_inserts          =          0
    Num_heap_assign_inserts       =          0
    Num_heap_home_deletes         =          0
    Num_heap_home_mvcc_deletes    =          0
    Num_heap_home_to_rel_deletes  =          0
    Num_heap_home_to_big_deletes  =          0
    Num_heap_rel_deletes          =          0
    Num_heap_rel_mvcc_deletes     =          0
    Num_heap_rel_to_home_deletes  =          0
    Num_heap_rel_to_big_deletes   =          0
    Num_heap_rel_to_rel_deletes   =          0
    Num_heap_big_deletes          =          0
    Num_heap_big_mvcc_deletes     =          0
    Num_heap_home_updates         =          0
    Num_heap_home_to_rel_updates  =          0
    Num_heap_home_to_big_updates  =          0
    Num_heap_rel_updates          =          0
    Num_heap_rel_to_home_updates  =          0
    Num_heap_rel_to_rel_updates   =          0
    Num_heap_rel_to_big_updates   =          0
    Num_heap_big_updates          =          0
    Num_heap_home_vacuums         =          0
    Num_heap_big_vacuums          =          0
    Num_heap_rel_vacuums          =          0
    Num_heap_insid_vacuums        =          0
    Num_heap_remove_vacuums       =          0
    The timer values for heap_insert_prepare are:
    Num_heap_insert_prepare       =          0
    Total_time_heap_insert_prepare =          0
    Max_time_heap_insert_prepare  =          0
    Avg_time_heap_insert_prepare  =          0
    The timer values for heap_insert_execute are:
    Num_heap_insert_execute       =          0
    Total_time_heap_insert_execute =          0
    Max_time_heap_insert_execute  =          0
    Avg_time_heap_insert_execute  =          0
    The timer values for heap_insert_log are:
    Num_heap_insert_log           =          0
    Total_time_heap_insert_log    =          0
    Max_time_heap_insert_log      =          0
    Avg_time_heap_insert_log      =          0
    The timer values for heap_delete_prepare are:
    Num_heap_delete_prepare       =          0
    Total_time_heap_delete_prepare =          0
    Max_time_heap_delete_prepare  =          0
    Avg_time_heap_delete_prepare  =          0
    The timer values for heap_delete_execute are:
    Num_heap_delete_execute       =          0
    Total_time_heap_delete_execute =          0
    Max_time_heap_delete_execute  =          0
    Avg_time_heap_delete_execute  =          0
    The timer values for heap_delete_log are:
    Num_heap_delete_log           =          0
    Total_time_heap_delete_log    =          0
    Max_time_heap_delete_log      =          0
    Avg_time_heap_delete_log      =          0
    The timer values for heap_update_prepare are:
    Num_heap_update_prepare       =          0
    Total_time_heap_update_prepare =          0
    Max_time_heap_update_prepare  =          0
    Avg_time_heap_update_prepare  =          0
    The timer values for heap_update_execute are:
    Num_heap_update_execute       =          0
    Total_time_heap_update_execute =          0
    Max_time_heap_update_execute  =          0
    Avg_time_heap_update_execute  =          0
    The timer values for heap_update_log are:
    Num_heap_update_log           =          0
    Total_time_heap_update_log    =          0
    Max_time_heap_update_log      =          0
    Avg_time_heap_update_log      =          0
    The timer values for heap_vacuum_prepare are:
    Num_heap_vacuum_prepare       =          0
    Total_time_heap_vacuum_prepare =          0
    Max_time_heap_vacuum_prepare  =          0
    Avg_time_heap_vacuum_prepare  =          0
    The timer values for heap_vacuum_execute are:
    Num_heap_vacuum_execute       =          0
    Total_time_heap_vacuum_execute =          0
    Max_time_heap_vacuum_execute  =          0
    Avg_time_heap_vacuum_execute  =          0
    The timer values for heap_vacuum_log are:
    Num_heap_vacuum_log           =          0
    Total_time_heap_vacuum_log    =          0
    Max_time_heap_vacuum_log      =          0
    Avg_time_heap_vacuum_log      =          0
    The timer values for heap_stats_sync_bestspace are:
    Num_heap_stats_sync_bestspace =          0
    Total_time_heap_stats_sync_bestspace =          0
    Max_time_heap_stats_sync_bestspace =          0
    Avg_time_heap_stats_sync_bestspace =          0
    Num_heap_stats_bestspace_entries =          0
    Num_heap_stats_bestspace_maxed =          0
    The timer values for bestspace_add are:
    Num_bestspace_add             =          0
    Total_time_bestspace_add      =          0
    Max_time_bestspace_add        =          0
    Avg_time_bestspace_add        =          0
    The timer values for bestspace_del are:
    Num_bestspace_del             =          0
    Total_time_bestspace_del      =          0
    Max_time_bestspace_del        =          0
    Avg_time_bestspace_del        =          0
    The timer values for bestspace_find are:
    Num_bestspace_find            =          0
    Total_time_bestspace_find     =          0
    Max_time_bestspace_find       =          0
    Avg_time_bestspace_find       =          0
    The timer values for heap_find_page_bestspace are:
    Num_heap_find_page_bestspace  =          0
    Total_time_heap_find_page_bestspace =          0
    Max_time_heap_find_page_bestspace =          0
    Avg_time_heap_find_page_bestspace =          0
    The timer values for heap_find_best_page are:
    Num_heap_find_best_page       =          0
    Total_time_heap_find_best_page =          0
    Max_time_heap_find_best_page  =          0
    Avg_time_heap_find_best_page  =          0
    The timer values for bt_fix_ovf_oids are:
    Num_bt_fix_ovf_oids           =          0
    Total_time_bt_fix_ovf_oids    =          0
    Max_time_bt_fix_ovf_oids      =          0
    Avg_time_bt_fix_ovf_oids      =          0
    The timer values for bt_unique_rlocks are:
    Num_bt_unique_rlocks          =          0
    Total_time_bt_unique_rlocks   =          0
    Max_time_bt_unique_rlocks     =          0
    Avg_time_bt_unique_rlocks     =          0
    The timer values for bt_unique_wlocks are:
    Num_bt_unique_wlocks          =          0
    Total_time_bt_unique_wlocks   =          0
    Max_time_bt_unique_wlocks     =          0
    Avg_time_bt_unique_wlocks     =          0
    The timer values for bt_leaf are:
    Num_bt_leaf                   =          0
    Total_time_bt_leaf            =          0
    Max_time_bt_leaf              =          0
    Avg_time_bt_leaf              =          0
    The timer values for bt_traverse are:
    Num_bt_traverse               =          5
    Total_time_bt_traverse        =        162
    Max_time_bt_traverse          =         51
    Avg_time_bt_traverse          =         32
    The timer values for bt_find_unique are:
    Num_bt_find_unique            =          0
    Total_time_bt_find_unique     =          0
    Max_time_bt_find_unique       =          0
    Avg_time_bt_find_unique       =          0
    The timer values for bt_find_unique_traverse are:
    Num_bt_find_unique_traverse   =          5
    Total_time_bt_find_unique_traverse =        165
    Max_time_bt_find_unique_traverse =         53
    Avg_time_bt_find_unique_traverse =         33
    The timer values for bt_range_search are:
    Num_bt_range_search           =          0
    Total_time_bt_range_search    =          0
    Max_time_bt_range_search      =          0
    Avg_time_bt_range_search      =          0
    The timer values for bt_range_search_traverse are:
    Num_bt_range_search_traverse  =          0
    Total_time_bt_range_search_traverse =          0
    Max_time_bt_range_search_traverse =          0
    Avg_time_bt_range_search_traverse =          0
    The timer values for bt_insert are:
    Num_bt_insert                 =          0
    Total_time_bt_insert          =          0
    Max_time_bt_insert            =          0
    Avg_time_bt_insert            =          0
    The timer values for bt_insert_traverse are:
    Num_bt_insert_traverse        =          0
    Total_time_bt_insert_traverse =          0
    Max_time_bt_insert_traverse   =          0
    Avg_time_bt_insert_traverse   =          0
    The timer values for bt_delete_obj are:
    Num_bt_delete_obj             =          0
    Total_time_bt_delete_obj      =          0
    Max_time_bt_delete_obj        =          0
    Avg_time_bt_delete_obj        =          0
    The timer values for bt_delete_obj_traverse are:
    Num_bt_delete_obj_traverse    =          0
    Total_time_bt_delete_obj_traverse =          0
    Max_time_bt_delete_obj_traverse =          0
    Avg_time_bt_delete_obj_traverse =          0
    The timer values for bt_mvcc_delete are:
    Num_bt_mvcc_delete            =          0
    Total_time_bt_mvcc_delete     =          0
    Max_time_bt_mvcc_delete       =          0
    Avg_time_bt_mvcc_delete       =          0
    The timer values for bt_mvcc_delete_traverse are:
    Num_bt_mvcc_delete_traverse   =          0
    Total_time_bt_mvcc_delete_traverse =          0
    Max_time_bt_mvcc_delete_traverse =          0
    Avg_time_bt_mvcc_delete_traverse =          0
    The timer values for bt_mark_delete are:
    Num_bt_mark_delete            =          0
    Total_time_bt_mark_delete     =          0
    Max_time_bt_mark_delete       =          0
    Avg_time_bt_mark_delete       =          0
    The timer values for bt_mark_delete_traverse are:
    Num_bt_mark_delete_traverse   =          0
    Total_time_bt_mark_delete_traverse =          0
    Max_time_bt_mark_delete_traverse =          0
    Avg_time_bt_mark_delete_traverse =          0
    The timer values for bt_undo_insert are:
    Num_bt_undo_insert            =          0
    Total_time_bt_undo_insert     =          0
    Max_time_bt_undo_insert       =          0
    Avg_time_bt_undo_insert       =          0
    The timer values for bt_undo_insert_traverse are:
    Num_bt_undo_insert_traverse   =          0
    Total_time_bt_undo_insert_traverse =          0
    Max_time_bt_undo_insert_traverse =          0
    Avg_time_bt_undo_insert_traverse =          0
    The timer values for bt_undo_delete are:
    Num_bt_undo_delete            =          0
    Total_time_bt_undo_delete     =          0
    Max_time_bt_undo_delete       =          0
    Avg_time_bt_undo_delete       =          0
    The timer values for bt_undo_delete_traverse are:
    Num_bt_undo_delete_traverse   =          0
    Total_time_bt_undo_delete_traverse =          0
    Max_time_bt_undo_delete_traverse =          0
    Avg_time_bt_undo_delete_traverse =          0
    The timer values for bt_undo_mvcc_delete are:
    Num_bt_undo_mvcc_delete       =          0
    Total_time_bt_undo_mvcc_delete =          0
    Max_time_bt_undo_mvcc_delete  =          0
    Avg_time_bt_undo_mvcc_delete  =          0
    The timer values for bt_undo_mvcc_delete_traverse are:
    Num_bt_undo_mvcc_delete_traverse =          0
    Total_time_bt_undo_mvcc_delete_traverse =          0
    Max_time_bt_undo_mvcc_delete_traverse =          0
    Avg_time_bt_undo_mvcc_delete_traverse =          0
    The timer values for bt_vacuum are:
    Num_bt_vacuum                 =          0
    Total_time_bt_vacuum          =          0
    Max_time_bt_vacuum            =          0
    Avg_time_bt_vacuum            =          0
    The timer values for bt_vacuum_traverse are:
    Num_bt_vacuum_traverse        =          0
    Total_time_bt_vacuum_traverse =          0
    Max_time_bt_vacuum_traverse   =          0
    Avg_time_bt_vacuum_traverse   =          0
    The timer values for bt_vacuum_insid are:
    Num_bt_vacuum_insid           =          0
    Total_time_bt_vacuum_insid    =          0
    Max_time_bt_vacuum_insid      =          0
    Avg_time_bt_vacuum_insid      =          0
    The timer values for bt_vacuum_insid_traverse are:
    Num_bt_vacuum_insid_traverse  =          0
    Total_time_bt_vacuum_insid_traverse =          0
    Max_time_bt_vacuum_insid_traverse =          0
    Avg_time_bt_vacuum_insid_traverse =          0
    The timer values for vacuum_master are:
    Num_vacuum_master             =          0
    Total_time_vacuum_master      =          0
    Max_time_vacuum_master        =          0
    Avg_time_vacuum_master        =          0
    The timer values for vacuum_job are:
    Num_vacuum_job                =          0
    Total_time_vacuum_job         =          0
    Max_time_vacuum_job           =          0
    Avg_time_vacuum_job           =          0
    The timer values for vacuum_worker_process_log are:
    Num_vacuum_worker_process_log =          0
    Total_time_vacuum_worker_process_log =          0
    Max_time_vacuum_worker_process_log =          0
    Avg_time_vacuum_worker_process_log =          0
    The timer values for vacuum_worker_execute are:
    Num_vacuum_worker_execute     =          0
    Total_time_vacuum_worker_execute =          0
    Max_time_vacuum_worker_execute =          0
    Avg_time_vacuum_worker_execute =          0
    Time_get_snapshot_acquire_time =          7
    Count_get_snapshot_retry      =          0
    Time_tran_complete_time       =          7
    The timer values for compute_oldest_visible are:
    Num_compute_oldest_visible    =          0
    Total_time_compute_oldest_visible =          0
    Max_time_compute_oldest_visible =          0
    Avg_time_compute_oldest_visible =          0
    Count_get_oldest_mvcc_retry   =          0
    Data_page_buffer_hit_ratio    =     100.00
    Log_page_buffer_hit_ratio     =       0.00
    Vacuum_data_page_buffer_hit_ratio =       0.00
    Vacuum_page_efficiency_ratio  =       0.00
    Vacuum_page_fetch_ratio       =       0.00
    Data_page_fix_lock_acquire_time_msec =       0.00
    Data_page_fix_hold_acquire_time_msec =       0.00
    Data_page_fix_acquire_time_msec =       0.25
    Data_page_allocate_time_ratio =     100.00
    Data_page_total_promote_success =       0.00
    Data_page_total_promote_fail  =       0.00
    Data_page_total_promote_time_msec =       0.00
    Num_unfix_void_to_private_top =          0
    Num_unfix_void_to_private_mid =          0
    Num_unfix_void_to_shared_mid  =          0
    Num_unfix_lru1_private_to_shared_mid =          0
    Num_unfix_lru2_private_to_shared_mid =          0
    Num_unfix_lru3_private_to_shared_mid =          0
    Num_unfix_lru2_private_keep   =          0
    Num_unfix_lru2_shared_keep    =          0
    Num_unfix_lru2_private_to_top =          0
    Num_unfix_lru2_shared_to_top  =          0
    Num_unfix_lru3_private_to_top =          4
    Num_unfix_lru3_shared_to_top  =          0
    Num_unfix_lru1_private_keep   =          3
    Num_unfix_lru1_shared_keep    =         36
    Num_unfix_void_to_private_mid_vacuum =          0
    Num_unfix_lru1_any_keep_vacuum =          0
    Num_unfix_lru2_any_keep_vacuum =          0
    Num_unfix_lru3_any_keep_vacuum =          0
    Num_unfix_void_aout_found     =          0
    Num_unfix_void_aout_not_found =          0
    Num_unfix_void_aout_found_vacuum =          0
    Num_unfix_void_aout_not_found_vacuum =          0
    Num_data_page_hash_anchor_waits =          0
    Time_data_page_hash_anchor_wait =          0
    The timer values for flush_collect are:
    Num_flush_collect             =          0
    Total_time_flush_collect      =          0
    Max_time_flush_collect        =          0
    Avg_time_flush_collect        =          0
    The timer values for flush_flush are:
    Num_flush_flush               =          0
    Total_time_flush_flush        =          0
    Max_time_flush_flush          =          0
    Avg_time_flush_flush          =          0
    The timer values for flush_sleep are:
    Num_flush_sleep               =          0
    Total_time_flush_sleep        =          0
    Max_time_flush_sleep          =          0
    Avg_time_flush_sleep          =          0
    The timer values for flush_collect_per_page are:
    Num_flush_collect_per_page    =          0
    Total_time_flush_collect_per_page =          0
    Max_time_flush_collect_per_page =          0
    Avg_time_flush_collect_per_page =          0
    The timer values for flush_flush_per_page are:
    Num_flush_flush_per_page      =          0
    Total_time_flush_flush_per_page =          0
    Max_time_flush_flush_per_page =          0
    Avg_time_flush_flush_per_page =          0
    Num_data_page_writes          =          0
    Num_data_page_dirty_to_post_flush =          0
    Num_data_page_skipped_flush   =          0
    Num_data_page_skipped_flush_need_wal =          0
    Num_data_page_skipped_flush_already_flushed =          0
    Num_data_page_skipped_flush_fixed_or_hot =          0
    The timer values for compensate_flush are:
    Num_compensate_flush          =          0
    Total_time_compensate_flush   =          0
    Max_time_compensate_flush     =          0
    Avg_time_compensate_flush     =          0
    The timer values for assign_direct_bcb are:
    Num_assign_direct_bcb         =          0
    Total_time_assign_direct_bcb  =          0
    Max_time_assign_direct_bcb    =          0
    Avg_time_assign_direct_bcb    =          0
    The timer values for wake_flush_waiter are:
    Num_wake_flush_waiter         =          0
    Total_time_wake_flush_waiter  =          0
    Max_time_wake_flush_waiter    =          0
    Avg_time_wake_flush_waiter    =          0
    The timer values for alloc_bcb are:
    Num_alloc_bcb                 =          0
    Total_time_alloc_bcb          =          0
    Max_time_alloc_bcb            =          0
    Avg_time_alloc_bcb            =          0
    The timer values for alloc_bcb_search_victim are:
    Num_alloc_bcb_search_victim   =          0
    Total_time_alloc_bcb_search_victim =          0
    Max_time_alloc_bcb_search_victim =          0
    Avg_time_alloc_bcb_search_victim =          0
    The timer values for alloc_bcb_cond_wait_high_prio are:
    Num_alloc_bcb_cond_wait_high_prio =          0
    Total_time_alloc_bcb_cond_wait_high_prio =          0
    Max_time_alloc_bcb_cond_wait_high_prio =          0
    Avg_time_alloc_bcb_cond_wait_high_prio =          0
    The timer values for alloc_bcb_cond_wait_low_prio are:
    Num_alloc_bcb_cond_wait_low_prio =          0
    Total_time_alloc_bcb_cond_wait_low_prio =          0
    Max_time_alloc_bcb_cond_wait_low_prio =          0
    Avg_time_alloc_bcb_cond_wait_low_prio =          0
    Num_alloc_bcb_prioritize_vacuum =          0
    Num_victim_use_invalid_bcb    =          0
    The timer values for alloc_bcb_get_victim_search_own_private_list are:
    Num_alloc_bcb_get_victim_search_own_private_list =          0
    Total_time_alloc_bcb_get_victim_search_own_private_list =          0
    Max_time_alloc_bcb_get_victim_search_own_private_list =          0
    Avg_time_alloc_bcb_get_victim_search_own_private_list =          0
    The timer values for alloc_bcb_get_victim_search_others_private_list are:
    Num_alloc_bcb_get_victim_search_others_private_list =          0
    Total_time_alloc_bcb_get_victim_search_others_private_list =          0
    Max_time_alloc_bcb_get_victim_search_others_private_list =          0
    Avg_time_alloc_bcb_get_victim_search_others_private_list =          0
    The timer values for alloc_bcb_get_victim_search_shared_list are:
    Num_alloc_bcb_get_victim_search_shared_list =          0
    Total_time_alloc_bcb_get_victim_search_shared_list =          0
    Max_time_alloc_bcb_get_victim_search_shared_list =          0
    Avg_time_alloc_bcb_get_victim_search_shared_list =          0
    Num_victim_assign_direct_vacuum_void =          0
    Num_victim_assign_direct_vacuum_lru =          0
    Num_victim_assign_direct_flush =          0
    Num_victim_assign_direct_panic =          0
    Num_victim_assign_direct_adjust_lru =          0
    Num_victim_assign_direct_adjust_lru_to_vacuum =          0
    Num_victim_assign_direct_search_for_flush =          0
    Num_victim_shared_lru_success =          0
    Num_victim_own_private_lru_success =          0
    Num_victim_other_private_lru_success =          0
    Num_victim_shared_lru_fail    =          0
    Num_victim_own_private_lru_fail =          0
    Num_victim_other_private_lru_fail =          0
    Num_victim_all_lru_fail       =          0
    Num_victim_get_from_lru       =          0
    Num_victim_get_from_lru_was_empty =          0
    Num_victim_get_from_lru_fail  =          0
    Num_victim_get_from_lru_bad_hint =          0
    Num_lfcq_prv_get_total_calls  =          0
    Num_lfcq_prv_get_empty        =          0
    Num_lfcq_prv_get_big          =          0
    Num_lfcq_shr_get_total_calls  =          0
    Num_lfcq_shr_get_empty        =          0
    The timer values for DWB_flush_block are:
    Num_DWB_flush_block           =          0
    Total_time_DWB_flush_block    =          0
    Max_time_DWB_flush_block      =          0
    Avg_time_DWB_flush_block      =          0
    The timer values for DWB_file_sync_helper are:
    Num_DWB_file_sync_helper      =          0
    Total_time_DWB_file_sync_helper =          0
    Max_time_DWB_file_sync_helper =          0
    Avg_time_DWB_file_sync_helper =          0
    The timer values for DWB_flush_block_cond_wait are:
    Num_DWB_flush_block_cond_wait =          0
    Total_time_DWB_flush_block_cond_wait =          0
    Max_time_DWB_flush_block_cond_wait =          0
    Avg_time_DWB_flush_block_cond_wait =          0
    The timer values for DWB_flush_block_sort are:
    Num_DWB_flush_block_sort      =          0
    Total_time_DWB_flush_block_sort =          0
    Max_time_DWB_flush_block_sort =          0
    Avg_time_DWB_flush_block_sort =          0
    The timer values for DWB_decache_pages_after_write are:
    Num_DWB_decache_pages_after_write =          0
    Total_time_DWB_decache_pages_after_write =          0
    Max_time_DWB_decache_pages_after_write =          0
    Avg_time_DWB_decache_pages_after_write =          0
    The timer values for DWB_wait_flush_block are:
    Num_DWB_wait_flush_block      =          0
    Total_time_DWB_wait_flush_block =          0
    Max_time_DWB_wait_flush_block =          0
    Avg_time_DWB_wait_flush_block =          0
    The timer values for DWB_wait_file_sync_helper are:
    Num_DWB_wait_file_sync_helper =          0
    Total_time_DWB_wait_file_sync_helper =          0
    Max_time_DWB_wait_file_sync_helper =          0
    Avg_time_DWB_wait_file_sync_helper =          0
    The timer values for DWB_flush_force are:
    Num_DWB_flush_force           =          0
    Total_time_DWB_flush_force    =          0
    Max_time_DWB_flush_force      =          0
    Avg_time_DWB_flush_force      =          0
    The timer values for Log_LZ4_compress are:
    Num_Log_LZ4_compress          =          0
    Total_time_Log_LZ4_compress   =          0
    Max_time_Log_LZ4_compress     =          0
    Avg_time_Log_LZ4_compress     =          0
    The timer values for Log_LZ4_decompress are:
    Num_Log_LZ4_decompress        =          0
    Total_time_Log_LZ4_decompress =          0
    Max_time_Log_LZ4_decompress   =          0
    Avg_time_Log_LZ4_decompress   =          0
    Num_alloc_bcb_wait_threads_high_priority =          0
    Num_alloc_bcb_wait_threads_low_priority =          0
    Num_flushed_bcbs_wait_for_direct_victim =          0
    Num_lfcq_big_private_lists    =          0
    Num_lfcq_private_lists        =          1
    Num_lfcq_shared_lists         =          0
    Num_data_page_avoid_dealloc   =          0
    Num_data_page_avoid_victim    =          0
    Num_data_page_fix_ext:
    WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =          8
    WORKER,PAGE_VOLHEADER,OLD_PAGE_IN_PB    ,READ ,UNCOND      =         30
    WORKER,PAGE_VOLBITMAP,OLD_PAGE_IN_PB    ,READ ,UNCOND      =         15
    WORKER,PAGE_BTREE_R  ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =          5
    Num_data_page_promote_ext:
    Num_data_page_promote_time_ext:
    Num_data_page_unfix_ext:
    WORKER,PAGE_HEAP     ,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =          8
    WORKER,PAGE_VOLHEADER,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =         30
    WORKER,PAGE_VOLBITMAP,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =         15
    WORKER,PAGE_BTREE_R  ,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =          5
    Time_data_page_lock_acquire_time:
    Time_data_page_hold_acquire_time:
    Time_data_page_fix_acquire_time:
    WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =               27
    WORKER,PAGE_VOLHEADER,OLD_PAGE_IN_PB    ,READ ,UNCOND      =              167
    WORKER,PAGE_VOLBITMAP,OLD_PAGE_IN_PB    ,READ ,UNCOND      =               40
    WORKER,PAGE_BTREE_R  ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =               23
    Num_mvcc_snapshot_ext:
    DELETE  ,INS_VACUUMED      ,VISIBLE   =                5
    SNAPSHOT,INS_VACUUMED      ,VISIBLE   =                5
    Time_obj_lock_acquire_time:
    Thread_stats_counters_timers:
    Thread_pgbuf_daemon_stats_counters_timers:
    Num_dwb_flushed_block_volumes:
    Thread_loaddb_stats_counters_timers:
    
    csql> ;.hist off

**Displaying query execution time (;TIme)**

The **;TIme** session command can be set to display the elapsed time to execute the query. It can be set to **ON** or **OFF**. The current setting is displayed if there is no value specified. The default value is **ON**.

The **SELECT** query includes the time of outputting the fetched records. Therefore, to check the execution time of complete output of all records in the **SELECT** query, use the **\-\-no-pager** option while executing the CSQC interpreter. ::

    $ csql -u dba --no-pager demodb
    csql> ;time ON
    csql> ;time
    TIME IS ON

**Displaying a column of result record in one line(;LINe-output)**

If this value is set to ON, it makes the record display in several lines by column. The default value is OFF, which makes one record display in one line. ::

    csql> ;line-output ON
    csql> select * from athlete;
     
    === <Result of SELECT Command in Line 1> ===
     
    <00001> code       : 10999
            name       : 'Fernandez Jesus'
            gender     : 'M'
            nation_code: 'ESP'
            event      : 'Handball'
    <00002> code       : 10998
            name       : 'Fernandez Jaime'
            gender     : 'M'
            nation_code: 'AUS'
            event      : 'Rowing'
    ...

**Displaying query history (;HISTORYList)**

This command displays the list that contains previously executed commands (input) and their history numbers. ::

    csql> ;historylist
    ----< 1 >----
    select * from nation;
    ----< 2 >----
    select * from athlete;

**Reading input with the specified history number into the buffer (;HISTORYRead)**

You can use **;HISTORYRead** session command to read input with history number in the **;HISTORYList** list into the command buffer. You can enter **;run** or **;xrun** directly because it has the same effect as when you enter SQL statements directly. ::

    csql> ;historyread 1

**Calling the default editor (;EDIT)**

This command calls the specified editor. The default editor is **vi** on Linux **Notepad** on Windows environment. Use **;EDITOR_Cmd** command to specify a different editor.

.. option:: format / fmt

The **format** or **fmt** option allows you to edit after formatting the SQL statement. Formatter registration uses the **;FOrmatter_cmd** session command. ::

    csql> ;edit
    SELECT * FROM t1

    csql> ;edit format
    SELECT
     *
    FROM
     t1

**Specifying the formatter (;FOrmatter_cmd)**

This command specifies the formatter to be used with **;EDIT** session command. The user can set it up and use it as shown in the example, and the OS environment variable(CUBRID_CSQL_FORMATTER or FORMATTER; CUBRID_CSQL_FORMATTER is used if both environment variables are set) can be configured in advance for use. ::

    csql> ;formatter_cmd /usr/local/bin/fsqlf

    $ export CUBRID_CSQL_FORMATTER=/home/cubrid/bin/fsqlf
    or
    $ export FORMATTER=/home/cubrid/bin/fsqlf

.. note::

        The use of Free SQL Formatter is recommended.

        Download URL: https://github.com/CUBRID/fsqlf/releases

**Specifying the editor (;EDITOR_Cmd)**

This command specifies the editor to be used with **;EDIT** session command. As shown in the example below, you can specify other editor (ex: emacs) which is installed in the system. ::

    csql> ;editor_cmd emacs
    csql> ;edit

The OS environment variable(CUBRID_CSQL_EDITOR or EDITOR; CUBRID_CSQL_EDITOR is used if both environment variables are set) may be registered and used in advance. ::

    export CUBRID_CSQL_EDITOR=emacs
    or
    export EDITOR=emacs

**Specifying the single line mode (;SIngleline)**

This command sets single line mode to **ON** or **OFF** (default value is **ON**). If you type semi-colon(;) and ENTER key in the single line ON mode, SQL statements are executed. If single line mode is set to **OFF**, multiple SQL statements are retrieved and executed at once with the **;xr** or **;ru** session command. If any value is not specified, current configured value is applied by default. ::

    csql> ;singleline off
    SINGLE IS OFF
    csql> ;singleline
    SINGLE IS OFF

**Switching csql session(;Connect)**

This is a session command to switch the connection to another user without terminating CSQL. The command format is as follows.

* user: Username to connect
* database: Database name to connect (if omitted, currently connected database)
* host: Hostname to connect to (if omitted, localhost) ::

     csql> ;connect public
     csql> ;connect dba

     csql> ;connect public testdb
     csql> ;connect dba demodb

     csql> ;connect Peter testdb@192.168.0.1
     csql> ;connect public demodb@localhost

.. warning::

     #. When ';connect' session command executes, the current CSQL session connection is disconnect regardless of session transition success or not. 
     #. When CSQL is in system administrator mode, ';connect' session command cannot be used (When entering CSQL with an csql -u dba \-\-sysadm demodb form command).


