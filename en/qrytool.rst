Query Tools
===========

CSQL Interpreter
----------------

The CSQL Interpreter is a program used to execute the SQL statements and retrieve results in a way that CUBRID supports. The entered SQL statements and the results can be stored as a file. For more information, see :ref:`csql-intro` and :ref:`csql-exec-mode`.

CUBRID also provides a GUI-based program called "CUBRID Manager" or "CUBRID Query Browser." By using these tools, you can execute all SQL statements and retrieve results in the query editor of CUBRID Manager. For more information, see :ref:`cm-cqb`.

This section describes how to use the CSQL Interpreter on the Linux environment

**Starting the CSQL Interpreter**

You can start the CSQL program in the shell as shown below. At the initial installation, **PUBLIC** and **DBA** users are provided and the passwords of the users not set. If no user is specified while the CSQL Interpreter is executed, **PUBLIC** is used for log-in. ::

    % csql demodb

    CUBRID SQL Interpreter

    Type `;help' for help messages.

    csql> ;help

    === <Help: Session Command Summary> ===


       All session commands should be prefixed by `;' and only blanks/tabs
       can precede the prefix. Capitalized characters represent the minimum
       abbreviation that you need to enter to execute the specified command.

       ;REAd   [<file-name>]       - read a file into command buffer.
       ;Write  [<file-name>]       - (over)write command buffer into a file.
       ;APpend [<file-name>]       - append command buffer into a file.
       ;PRINT                      - print command buffer.
       ;SHELL                      - invoke shell.
       ;CD                         - change current working directory.
       ;EXit                       - exit program.

       ;CLear                      - clear command buffer.
       ;EDIT                       - invoke system editor with command buffer.
       ;LISt                       - display the content of command buffer.

       ;RUn                        - execute sql in command buffer.
       ;Xrun                       - execute sql in command buffer,
                                     and clear the command buffer.
       ;COmmit                     - commit the current transaction.
       ;ROllback                   - roll back the current transaction.
       ;AUtocommit [ON|OFF]        - enable/disable auto commit mode.
       ;REStart                    - restart database.

       ;SHELL_Cmd  [shell-cmd]     - set default shell, editor, print and pager
       ;EDITOR_Cmd [editor-cmd]      command to new one, or display the current
       ;PRINT_Cmd  [print-cmd]       one, respectively.
       ;PAger_cmd  [pager-cmd]

       ;DATE                       - display the local time, date.
       ;DATAbase                   - display the name of database being accessed.
       ;SChema class-name          - display schema information of a class.
       ;SYntax [sql-cmd-name]      - display syntax of a command.
       ;TRigger [`*'|trigger-name] - display trigger definition.
       ;Get system_parameter       - get the value of a system parameter.
       ;SEt system_parameter=value - set the value of a system parameter.
       ;PLan [simple/detail/off]   - show query execution plan.
       ;Info <command>             - display internal information.
       ;TIme [ON/OFF]              - enable/disable to display the query
                                     execution time.
       ;LINe-output [ON/OFF]       - enable/disable to display each value in a line
       ;HISTORYList                - display list of the executed queries.
       ;HISTORYRead <history_num>  - read entry on the history number into command buffer.
       ;TRAce [ON/OFF] [text/json] - enable/disable sql auto trace.
       ;HElp                       - display this help message.

**Running SQL with CSQL**

After the CSQL has been executed, you can enter the SQL into the CSQL prompt. Each SQL statement must end with a semicolon (;). Multiple SQL statements can be entered in a single line. You can find the simple usage of the session commands with the ;help command. For more information, see :ref:`csql-session-commands`. ::

    % csql demodb
    
    csql> SELECT SUM(n) FROM (SELECT gold FROM participant WHERE nation_code='KOR'
    csql> UNION ALL SELECT silver FROM participant WHERE nation_code='JPN') AS t(n);

    === <Result of SELECT Command in Line 2> ===

           sum(n)
    =============
               82

    1 rows selected. (0.106504 sec) Committed.

    csql> ;exit

.. _cm-cqb:

Management Tools
================

+--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+---------------------------------------------------------------------+
|                          | Summary of features                                                         | Downloads of the recent files                                   | Links to the latest documents                                       |
+==========================+=============================================================================+=================================================================+=====================================================================+
| CUBRID Web Manager       | Web based tool for SQL execution and DB operation.                          | `CUBRID Web Manager Download                                    | `CUBRID Web Manager Documents                                       | 
|                          |                                                                             | <http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Web_Manager>`_       | <http://www.cubrid.org/wiki_tools/entry/cubrid-web-manager>`_       |   
|                          | 1) Possible to access to DB with a web browser                              |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |   
|                          | 2) Possible to use instantly after installing CUBRID                        |                                                                 |                                                                     |
|                          |    (CUBRID 2008 R4.3 or higher)                                             |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |   
|                          | 3) Useful to manage a single host                                           |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |   
|                          | 4) DB access via CUBRID Manager server                                      |                                                                 |                                                                     |
+--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+---------------------------------------------------------------------+
| CUBRID Manager           | Java client tool for SQL execution & DB operation.                          | `CUBRID Manager Download                                        | `CUBRID Manager Documents                                           |
|                          |                                                                             | <http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Manager>`_           | <http://www.cubrid.org/wiki_tools/entry/cubrid-manager>`_           |   
|                          | 1) Java-based management tool (JRE 1.6 or higher required)                  |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |   
|                          | 2) Auto upgrade after the initial download                                  |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |   
|                          | 3) Useful to manage multiple hosts                                          |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |   
|                          | 4) DB access via CUBRID Manager server                                      |                                                                 |                                                                     |
+--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+---------------------------------------------------------------------+
| CUBRID Query Browser     | Java client tool for SQL execution only.                                    | `CUBRID Query Browser Download                                  | `CUBRID Query Browser Documents                                     |
|                          |                                                                             | <http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Query_Browser>`_     | <http://www.cubrid.org/wiki_tools/entry/cubrid-query-browser>`_     |  
|                          | 1) Java-based management tool (JRE 1.6 or higher required)                  |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |  
|                          | 2) Auto upgrade after the initial download                                  |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |  
|                          | 3) Useful to manage multiple hosts                                          |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |  
|                          | 4) Direct DB access via JDBC                                                |                                                                 |                                                                     |
+--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+---------------------------------------------------------------------+
| CUBRID Migration Toolkit | Java-based client tool to migrate schema and data from source DB            | `CUBRID Migration Toolkit Download                              | `CUBRID Migration Toolkit Documents                                 |
|                          | (MySQL, Oracle, CUBRID) to CUBRID.                                          | <http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Migration_Toolkit>`_ | <http://www.cubrid.org/wiki_tools/entry/cubrid-migration-toolkit>`_ |
|                          |                                                                             |                                                                 |                                                                     |   
|                          | 1) Java-based management tool (JRE 1.6 or higher required)                  |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |   
|                          | 2) Auto upgrade after the initial download                                  |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |   
|                          | 3) Available migration only for multiple queries results,                   |                                                                 |                                                                     |
|                          |    the reuse of migration scenario; good to batch job                       |                                                                 |                                                                     |
|                          |                                                                             |                                                                 |                                                                     |   
|                          | 4) Direct DB access with JDBC                                               |                                                                 |                                                                     |
+--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+---------------------------------------------------------------------+

Running SQL with CUBRID Web Manager
-----------------------------------

Because CUBRID 2008 R4.3 or higher version includes Web Manager on the installation package, you can use the Web Manager instantly after the installation of CUBRID DBMS.

#.  Configure the value of **support_web_manager** in cm.conf as "YES".

#.  Start CUBRID Service. Web Manager works normally only when CUBRID Manager server is started. For more information, see :ref:`cubrid-manager-server`. 

    ::

        C:\CUBRID>cubrid service start
        ++ cubrid service is running.

#.  Access to https://localhost:8001/ which is written on the address bar. Note that the header of address is not http, but https.
    
#.  First, log-in to the host. To access to the host, you should perform the CUBRID Manager server user (=the host user)'s authentication primarily. The default user ID/password is admin/admin.

    .. image:: /images/gs_manager_login.png

#.  Connect to the DB server. In the tree on the left, you can see the list of databases which have been generated within the corresponding host. Click the DB name that you want to access and perform the DB user authentication (default ID/password: dba/pressing enter key)
    
    .. image:: /images/gs_manager_db.png
    
#.  Run the SQL on the access DB and confirm the result. On the left side, the list of connected databases are displayed. You can edit, run, and find the result on the SQL tab.

    .. image:: /images/gs_manager_screen.png

For more information, see http://www.cubrid.org/wiki_tools/entry/cubrid-web-manager-manual\ .

Running SQL with CUBRID Manager Client
--------------------------------------

CUBRID Manager is the client tool that you should download and run. It is a Java application which requires JRE or JDK 1.6 or higher.

#.  Download and install the latest CUBRID Manager file. CUBRID Manager is compatible with CUBRID DB engine 2008 R2.2 or higher version. It is recommended to upgrade to the latest version periodically; it supports the auto-update feature.
    (CUBRID FTP: http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Manager )

#.  Start CUBRID service on the server. CUBRID Manager server should be started for CUBRID Manager client to access to DB. For more information, see :ref:`cubrid-manager-server`.

    ::

        C:\CUBRID>cubrid service start
        ++ cubrid service is running.
    
#.  After the installation of CUBRID Manager, register host information on the [File > Add Host] menu. To register the host, you should enter host address, connection port (default: 8001), and CUBRID Manager user name/password and install the JDBC driver of the same version with DB engine (supporting auto-driver-search/auto-update).

#.  Choose the host on the left tress and perform the CUBRID Manager user (=host user) authentication. The default ID/password is admin/admin.

#.  Create a new database as clicking the right mouse button on the database node, or try to connect as choosing the existing database on the bottom of the host node. At this time, do the DB user's login. The default db user is "dba", and there is no password.

#. Run SQL on the access DB and confirm the result. The host, DB and table list are displayed on the left side, and the query editor and the result window is shown on the right side. You can reuse the SQLs which have been succeeded with [SQL History] tab and compare the multiple results of several DBs as adding the DBs for the comparison of the result with [Multiple Query] tab.

    .. image:: /images/gs_manager_sql.png

For more information, see http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual_kr\ .

Running SQL with CUBRID Query Browser
-------------------------------------

CUBRID Query Browser (hereafter CQB) is the development tool only for SQL execution, light-weight version of CUBRID Manager (hereafter CM). The differences with CM are as follows:

*   CQB can access DB via JDBC only, without CM server.
    
*   As a result, DB/broker operating and monitoring features are not supported.
    
*   As a result, CQB only logs in DB user and CM user login is unnecessary.
    
*   Running CUBRID Manager server on the server side is unnecessary.

CQB client tool also needs to be downloaded and installed separately from the CUBRID installation package. It is executed on a Java application which requires JRE or JDK 1.6 version or later.

#.  Install the latest CQB file after download. It is compatible with any versions of the engine if you just add the same version's JDBC driver with the DB server. It is recommended to upgrade to the latest version periodically because it supports the auto-update feature.
    (CUBRID ftp: http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Query_Browser )

#.  Register DB access information on the [File > New Connection] menu after installing CQB. In this case, broker address, broker access port (default: 33,000), DB user, and password should be entered and the JDBC driver which has the same version with DB server should be installed (supporting auto-driver-search/auto-update).
    
#.  Try to access as choosing DB. In this case, perform DB authentication (default: dba/pressing enter key).
    
#.  Run SQL on the access DB and confirm the result. The lists of Host, DB, and table are displayed on the left side, and the query editor/result window are shown on the right side. You can reuse the SQLs which have been succeeded with [SQL History] tab and compare the multiple results of several DBs as adding the DBs for the comparison of the result with [Multiple Query] tab.

    .. image:: /images/gs_manager_qb.png

For more information, see http://www.cubrid.org/wiki_tools/entry/cubrid-query-browser-manual_kr\ .

Migrating schema/data with CUBRID Migration Toolkit
---------------------------------------------------

CUBRID Migration Toolkit is a tool to migrate the data and the schema from the source DB (MySQL, Oracle, and CUBRID) to the target DB (CUBRID). It is also Java applications which require JRE or JDK 1.6 or later. You should download separately.
(CUBRID FTP: http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Migration_Toolkit )

It is useful in case of switching from other DB into CUBRID, in case of migrating into other hardware, in case of migrating some schema and data from the operating DB, in case of upgrading CUBRID version, and in case of running the batch jobs. The main features are as follows:

*   Supports the tools/some schema and data migration
    
*   Available to migrate only the desired data as running several SQLs
    
*   Executable without suspending of operation as supporting online migration through JDBC
    
*   Available offline migration with CSV, SQL, CUBRID loaddb format data
    
*   Available to run directly on the target server as extracting the run-script of migration
    
*   Shorten the batch job time as reusing the migration run-script.

.. image:: /images/gs_manager_migration.png

For more information, see http://www.cubrid.org/wiki_tools/entry/cubrid-migration-toolkit-manual\ .

Drivers
=======

The drivers supported by CUBRID are as follows:

*   :doc:`CUBRID JDBC driver <api/jdbc>` (`Downloads JDBC <http://www.cubrid.org/?mid=downloads&item=jdbc_driver>`_)

*   :doc:`CUBRID CCI driver <api/cci>` (`Downloads CCI <http://www.cubrid.org?mid=downloads&item=cci_driver>`_)

*   :doc:`CUBRID PHP driver <api/php>` (`Downloads PHP <http://www.cubrid.org/?mid=downloads&item=php_driver&driver_type=phpdr>`_)

*   :doc:`CUBRID PDO driver <api/pdo>` (`Downloads PDO <http://www.cubrid.org/?mid=downloads&item=php_driver&driver_type=pdo>`_)

*   :doc:`CUBRID ODBC driver <api/odbc>` (`Downloads ODBC <http://www.cubrid.org/?mid=downloads&item=odbc_driver>`_)

*   :doc:`CUBRID OLE DB driver <api/oledb>` (`Downloads OLE DB <http://www.cubrid.org/?mid=downloads&item=oledb_driver>`_)

*   :doc:`CUBRID ADO.NET driver <api/adodotnet>` (`Downloads ADO.NET <http://www.cubrid.org/?mid=downloads&item=ado_dot_net_driver>`_)

*   :doc:`CUBRID Perl driver <api/perl>` (`Downloads Perl <http://www.cubrid.org/?mid=downloads&item=perl_driver>`_)

*   :doc:`CUBRID Python driver <api/python>` (`Downloads Python <http://www.cubrid.org/?mid=downloads&item=python_driver>`_)

*   :doc:`CUBRID Ruby driver <api/ruby>` (`Downloads Ruby <http://www.cubrid.org/?mid=downloads&item=ruby_driver>`_)

*   :doc:`CUBRID Node.js driver <api/node_js>` (`Downloads Node.js <http://www.cubrid.org/?mid=downloads&item=nodejs_driver>`_)

Among above drivers, JDBC and CCI drivers are automatically downloaded while CUBRID is being installed. Thus, you do not have to download them manually.
