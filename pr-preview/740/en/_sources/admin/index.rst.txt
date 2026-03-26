
:meta-keywords: cubrid administrator, cubrid dba, cubrid tools, cubrid utilities, cubrid services, cubrid processes, cubrid config, cubrid troubleshoot, ddl audit
:meta-description: This manual chapter covers everything needed by database administrators how to use cubrid utilities, how to monitor cubrid systems, how to troubleshoot and how to fix problems.

*****************
CUBRID Management
*****************

This chapter describes how the database administrators (**DBA**) operates the CUBRID system.

*   It includes instructions on how to use the **cubrid** utility, which starts and stops various processes of the CUBRID server, the broker, java stored procedure server, and manager server. See :doc:`/admin/control`.

*   It includes instructions on the following: database management tasks (creating and deleting databases, adding volume, etc.), migration tasks (moving database to a different location or making changes so that it fits the system's version), and making back-ups and rollbacks of the database in case of failures. See :doc:`/admin/admin_utils`.

*   It includes instructions on the system configuration. See :doc:`/admin/config`.

*   It includes how to use SystemTap, which can monitors and traces the operating processes dynamically. See :doc:`/admin/systemtap`.
   
*   It includes instructions on troubleshooting. See :doc:`/admin/troubleshoot`.

The **cubrid** utilities provide features that can be used to comprehensively manage the CUBRID service. The CUBRID utilities are divided into the service management utility, which is used to manage the CUBRID service process, and the database management utility, which is used to manage the database.

The service management utilities are as follows:

*   Service utility : Operates and manages the master process.

    *   :ref:`cubrid service <control-cubrid-services>`

*   Server utility : Operates and manages the server process.

    *   :ref:`cubrid server <control-cubrid-server>`

*   Broker utility : Operates and manages the broker process and application server (CAS) process.

    *   :ref:`cubrid broker <broker>`

*   Gateway utility : Operates and manages the gateway process and application server (CAS) process.
    *   :ref:`cubrid gateway <gateway>`
	
*   Manager utility : Operates and manages the manager server process.

    *   :ref:`cubrid manager <cubrid-manager-server>`

*   HA utility : Operates and manages the HA-related processes.

    *   :ref:`cubrid heartbeat <cubrid-heartbeat>`

*   Java SP Server utility : Operates and manages the process of the Java stored procedure (Java SP) server.

    *   :ref:`cubrid javasp <cubrid-javasp-server>`

See :ref:`control-cubrid-processes` for details.

The database management utilities are as follows:

*   Creating database, adding volume, and deleting database

    *   :ref:`cubrid createdb <createdb>`
    *   :ref:`cubrid addvoldb <addvoldb>`
    *   :ref:`cubrid deletedb <deletedb>`

*   Renaming database, altering host, copying/moving database, and registering database

    *   :ref:`cubrid renamedb <renamedb>`
    *   :ref:`cubrid alterdbhost <alterdbhost>`
    *   :ref:`cubrid copydb <copydb>`
    *   :ref:`cubrid installdb <installdb>`

*   Backing up database

    *   :ref:`cubrid backupdb <backupdb>`

*   Restoring database

    *   :ref:`cubrid restoredb <restoredb>`

*   Unloading and Loading database

    *   :ref:`cubrid unloaddb <unloaddb>`
    *   :ref:`cubrid loaddb <loaddb>`
    
*   Checking and compacting database space

    *   :ref:`cubrid spacedb <spacedb>`
    *   :ref:`cubrid compactdb <compactdb>`

*   Updating statistics and checking query plan

    *   :ref:`cubrid plandump <plandump>`
    *   :ref:`cubrid optimizedb <optimizedb>`
    *   :ref:`cubrid statdump <statdump>`

*   Checking database lock, checking transaction and killing transaction

    *   :ref:`cubrid lockdb <lockdb>`
    *   :ref:`cubrid tranlist <tranlist>`
    *   :ref:`cubrid killtran <killtran>`

*   Diagnosing database and dumping parameter

    *   :ref:`cubrid checkdb <checkdb>`
    *   :ref:`cubrid diagdb <diagdb>`
    *   :ref:`cubrid paramdump <paramdump>`

*   Changing HA mode, replicating/applying logs 

    *   :ref:`cubrid changemode <cubrid-changemode>`
    *   :ref:`cubrid applyinfo <cubrid-applyinfo>`

*   Compiling/Outputting locale

    *   :ref:`cubrid genlocale <locale-command>`
    *   :ref:`cubrid dumplocale <dumplocale>`

See :ref:`cubrid-utilities` for details.

.. _utility-on-windows:

.. note::

    If you want to control the service by using **cubrid** utility on Windows Vista or later, it is recommended that you run the command prompt with an administrator account. If you use **cubrid** utility without an administrator account, the result message is not displayed even though you can run it through the User Account Control (UAC) dialog.

    To run the command prompt on Windows Vista or later with an administrator account, right-click [Start] > [All Programs] > [Accessories] > [Command Prompt] and select [Run as Administrator]. In the dialog verifying authorization, click [Yes], and then the command prompt is run as an administrator account.

.. toctree::
    :maxdepth: 3

    control.rst
    db_manage.rst
    admin_utils.rst
    config.rst
    systemtap.rst
    scripts.rst
    troubleshoot.rst
    ddl_audit.rst
