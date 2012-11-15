*********************
Administrator's Guide
*********************

The "Administrator's Guide" provides the database administrators (**DBA**) with details on how to operate the CUBRID system. The guide includes instructions on the following: database management tasks (creating and deleting databases, adding volume, etc.), migration tasks (moving database to a different location or making changes so that it fits the system's version), and making back-ups and rollbacks of the database in case of failures.

It also includes instructions on how to use the **cubrid** utility, which starts and stops various processes of the CUBRID server, the broker and manager server.

This chapter contains the following:

*   How to use CUBRID utilities

*   How to control the CUBRID (service, database server, broker, manager server)

*   How to use the database administrative utilities

*   Database migration

*   Database backup and restore

*   CUBRID HA

**CUBRID Utilities**

The CUBRID utilities provide features that can be used to comprehensively manage the CUBRID service. The CUBRID utilities are divided into the service management utility, which is used to manage the CUBRID service process, and the database management utility, which is used to manage the database.

The service management utilities are as follows:

*   Service utility : Operates and manages the master process.

	*   cubrid service

*   Server utility : Operates and manages the server process.

	*   cubrid server

*   Broker utility : Operates and manages the broker process and application server (CAS) process.

	*   cubrid broker

*   Manager utility : Operates and manages the manager server process.

	*   cubrid manager

*   HA utility : Operates and manages the HA-related processes.

	*   cubrid heartbeat

See `Registering Services <#admin_admin_service_conf_registe_6298>`_ for details.

The database management utilities are as follows:

*   Creating database, adding volume, and deleting datbase

	*   cubrid createdb

	*   cubrid addvoldb

	*   cubrid deletedb

*   Renaming database, altering host, copying database, and installing database

	*   cubrid renamedb

	*   cubrid alterdbhost

	*   cubrid copydb

	*   cubrid installdb

*   Checking and compacting database space

	*   cubrid spacedb

	*   cubrid compactdb

*   Checking query plan and optimizing database

	*   cubrid plandump

	*   cubrid optimizedb

	*   cubrid statdump

*   Checking database lock, killing transaction, and checking consistency

	*   cubrid lockdb

	*   cubrid killtran

	*   cubrid checkdb

*   Diagnosing database and dumping parameter

	*   cubrid diagdb

	*   cubrid paramdump

*   Loading and unloading database

	*   cubrid loaddb

	*   cubrid unloaddb

*   Backing up and restoring database

	*   cubrid backupdb

	*   cubrid restoredb

*   HA utilities

	*   cubrid changemode

	*   cubrid copylogdb

	*   cubrid applylogdb

	*   cubrid applyinfo

*   Locale utilities

	*   cubrid genlocale

	*   cubrid dumplocale

See `How to Use the CUBRID Management Utilities (Syntax) <#admin_admin_service_server_comma_427>`_ for details.

The following result is displayed if **cubrid** is entered in a prompt.

::

	% cubrid
	 
	cubrid utility, version 9.0
	usage: cubrid <utility-name> [args]
	Type 'cubrid <utility-name>' for help on a specific utility.
	 
	Available service's utilities:
		service
		server
		broker
		manager
		heartbeat
	 
	Available administrator's utilities:
		addvoldb
		alterdbhost
		backupdb
		checkdb
		compactdb
		copydb
		createdb
		deletedb
		diagdb
		installdb
		killtran
		loaddb
		lockdb
		optimizedb
		plandump
		renamedb
		restoredb
		spacedb
		unloaddb
		paramdump
		statdump
		changemode
		copylogdb
		applylogdb
		applyinfo
	 
	cubrid is a tool for DBMS.

.. note::

	If you want to control the service by using **cubrid** utility on Windows Vista or later, it is recommended that you run the command prompt with an administrator account. If you use **cubrid** utility without an administrator account, the result message is not displayed even though you can run it through the User Account Control (UAC) dialog.

	To run the command prompt on Windows Vista or later with an administrator account, right-click [Start] > [All Programs] > [Accessories] > [Command Prompt] and select [Run as Administrator]. In the dialog verifying authorization, click [Yes]; then the command prompt runs with an administrator account.

.. toctree::
	:maxdepth: 2

	control.rst
	admin_utils.rst
	migration.rst
	backup.rst
	i18n.rst
	ha.rst
	shard.rst
	config.rst