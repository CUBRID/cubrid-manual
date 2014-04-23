*********************
Administrator's Guide
*********************

The "Administrator's Guide" provides the database administrators (**DBA**) with details on how to operate the CUBRID system. 

*   It includes instructions on how to use the **cubrid** utility, which starts and stops various processes of the CUBRID server, the broker and manager server.

*   It includes instructions on the following: database management tasks (creating and deleting databases, adding volume, etc.), migration tasks (moving database to a different location or making changes so that it fits the system's version), and making back-ups and rollbacks of the database in case of failures.

*   It includes instructions on the system configuration.
   
*   It includes instructions on troubleshooting.

The **cubrid** utilities provide features that can be used to comprehensively manage the CUBRID service. The CUBRID utilities are divided into the service management utility, which is used to manage the CUBRID service process, and the database management utility, which is used to manage the database.

The service management utilities are as follows:

*   Service utility : Operates and manages the master process.

    *   cubrid service

*   Server utility : Operates and manages the server process.

    *   cubrid server

*   Broker utility : Operates and manages the broker process and application server (CAS) process.

    *   cubrid broker

*   Manager utility : Operates and manages the manager server process.

    *   cubrid manager

*   HA utility : Operates and manages the HA-related processes.

    *   cubrid heartbeat

See :ref:`control-cubrid-processes` for details.

The database management utilities are as follows:

*   Creating database, adding volume, and deleting database

    *   cubrid createdb
    *   cubrid addvoldb
    *   cubrid deletedb

*   Renaming database, altering host, copying/moving database, and registering database

    *   cubrid renamedb
    *   cubrid alterdbhost
    *   cubrid copydb
    *   cubrid installdb

*   Backing up database

    *   cubrid backupdb

*   Restoring database

    *   cubrid restoredb

*   Unloading and Loading database

    *   cubrid loaddb
    *   cubrid unloaddb
    
*   Checking and compacting database space

    *   cubrid spacedb
    *   cubrid compactdb

*   Updating statistics and checking query plan

    *   cubrid plandump
    *   cubrid optimizedb
    *   cubrid statdump

*   Checking database lock, checking transaction and killing transaction

    *   cubrid lockdb
    *   cubrid tranlist
    *   cubrid killtran

*   Diagnosing database and dumping parameter

    *   cubrid checkdb
    *   cubrid diagdb
    *   cubrid paramdump

*   Changing HA mode, replicating/applying logs 

    *   cubrid changemode
    *   cubrid applyinfo

*   Compiling/Outputting locale
 

    *   cubrid genlocale
    *   cubrid dumplocale

See :ref:`cubrid-utilities` for details.

.. _utility-on-windows:

.. note::

    If you want to control the service by using **cubrid** utility on Windows Vista or later, it is recommended that you run the command prompt with an administrator account. If you use **cubrid** utility without an administrator account, the result message is not displayed even though you can run it through the User Account Control (UAC) dialog.

    To run the command prompt on Windows Vista or later with an administrator account, right-click [Start] > [All Programs] > [Accessories] > [Command Prompt] and select [Run as Administrator]. In the dialog verifying authorization, click [Yes], and then the command prompt is run as an administrator account.

.. toctree::
    :maxdepth: 3

    control.rst
    admin_utils.rst
    config.rst
    troubleshoot.rst
    systemtap.rst