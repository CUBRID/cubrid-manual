.. _upgrade:

Upgrade
=======

.. _up-from-90to91:

Upgrading From CUBRID 9.0 Beta To CUBRID 9.1
--------------------------------------------

Users who are using versions CUBRID 2008 R9.0 Beta should install 9.1 in the different directory and modify parameter values in the existing environment configuration file.

**cubrid.conf**

* The value of sort_buffer_size should be configured as 2G or less since the maximum value of sort_buffer_size is 2G.
    
**cubrid_broker.conf**

* In KEEP_CONNECTION parameter, OFF value should be changed as ON or AUTO since OFF setting value is no longer used. 
* SELECT_AUTO_COMMIT should be deleted since this parameter is no longer used.
* The value of APPL_SERVER_MAX_SIZE_HARD_LIMIT should be 2,097,151 or less since the maximum value of APPL_SERVER_MAX_SIZE_HARD_LIMIT is 2,097,151.

**Environment variable**

* CUBRID_LANG is removed, and now CUBRID_CHARSET is used for configuring the charset of database and CUBRID_MSG_LANG is used for configuring the charset of messages for utilities and errors.

For more details, see :ref:`changed-config91`.

* Since the DB volume of CUBRID 9.0 Beta is not compatible with the DB volume of CUBRID 9.1, it should be migrated after the installation of CUBRID 9.1.

For DB migration, see :ref:`db-migration`.

Upgrading From CUBRID 2008 R4.1/R4.3 To CUBRID 9.1
--------------------------------------------------

Users who are using versions CUBRID 2008 R4.1 or R4.3 should install 9.1 in the different directory and modify parameter values in the existing environment configuration file.

**cubrid.conf**

* The value of sort_buffer_size should be configured as 2G or less since the maximum value of sort_buffer_size is 2G.
* single_byte_compare should be deleted since this parameter is no longer used.
* intl_mbs_support should be deleted since this parameter is no longer used.
* lock_timeout_message_type should be deleted since this parameter is no longer used.

**cubrid_broker.conf**

* In KEEP_CONNECTION parameter, OFF value should be changed as ON or AUTO since OFF setting value is no longer used. 
* SELECT_AUTO_COMMIT should be deleted since this parameter is no longer used.
* The value of APPL_SERVER_MAX_SIZE_HARD_LIMIT should be 2,097,151 or less since the maximum value of APPL_SERVER_MAX_SIZE_HARD_LIMIT is 2,097,151.
    
**cubrid_ha.conf**

* Users who have configured the ha_apply_max_mem_size parameter value more than 500 must the value to 500 or less.

**Environment variable**

* CUBRID_LANG is removed, and now CUBRID_CHARSET is used for configuring the charset of database and CUBRID_MSG_LANG is used for configuring the charset of messages for utilities and errors.
    
For more details, see :ref:`changed-config91`.

For DB migration after environmental configuration, see :ref:`db-migration`.

Upgrading From CUBRID 2008 R4.0 or Earlier Versions To CUBRID 9.1
-----------------------------------------------------------------

Users who are using versions CUBRID 2008 R4.0 or earlier should install 9.1 in the different directory and modify parameter values in the existing environment configuration file.

**cubrid.conf**

* The value of sort_buffer_size should be configured as 2G or less since the maximum value of sort_buffer_size is 2G.
* single_byte_compare should be deleted since this parameter is no longer used.
* intl_mbs_support should be deleted since this parameter is no longer used.
* lock_timeout_message_type should be deleted since this parameter is no longer used.
* Because the default value of thread_stacksize has been changed from 100K to 1M, it is recommended that users who have not configured this value check memory usage of CUBRID-associative processes.
* Because the minimum value of data_buffer_size has been changed from 64K to 16M, users who have configured this value less than 16M must change the value equal to or greater than 16M.
    
**cubrid_broker.conf**

* In KEEP_CONNECTION parameter, OFF value should be changed as ON or AUTO since OFF setting value is no longer used. 
* SELECT_AUTO_COMMIT should be deleted since this parameter is no longer used.
* The value of APPL_SERVER_MAX_SIZE_HARD_LIMIT should be 2,097,151 or less since the maximum value of APPL_SERVER_MAX_SIZE_HARD_LIMIT is 2,097,151.
* The minimum value of APPL_SERVER_MAX_SIZE_HARD_LIMIT is 1024M. It is recommended that users who configure APPL_SERVER_MAX_SIZE configure this value less than the value of APPL_SERVER_MAX_SIZE_HARD_LIMIT.
* Because the default value of CCI_DEFAULT_AUTOCOMMIT has been changed to ON, users who have not configured this value should change it to OFF if they want to keep auto commit mode.

**cubrid_ha.conf**

* Users who have configured the ha_apply_max_mem_size parameter value more than 500 must the value to 500 or less.

**Environment variable**

* CUBRID_LANG is removed, and now CUBRID_CHARSET is used for configuring the charset of database and CUBRID_MSG_LANG is used for configuring the charset of messages for utilities and errors.
    
For more details, see :ref:`changed-config91`.

For DB migration after environmental configuration, see :ref:`db-migration`.

Cautions During Upgrade
-----------------------

**Saving the Existing Configuration File**

* Archive the configuration files in the $CUBRID/conf directory (cubrid.conf, cubrid_broker.conf and cm.conf) and the DB location file (databases.txt) in the $CUBRID_DATABASES directory.

**Checking New Reserved Words**

* You can check whether reserved words are being used or not by applying the CUBRID 9.1 reserved word detection script, check_reserved.sql, which is distributed through the CUBRID installation package or http://ftp.cubrid.org\ . If the reserved words are being used as identifiers, the identifiers must be modified. See :doc:`sql/identifier`.

**Configuring environment variables of CUBRID_CHARSET, CUBRID_MSG_LANG**

* CUBRID_LANG environment variable is no more used, and CUBRID_CHARSET should be configured because it is used to decide the charset of database. CUBRID_MSG_LANG is used when displaying the messages of utilities or errors. If CUBRID_MSG_LANG is not configured, it follows the value of CUBRID_CHARSET.

**Changing schema**

* 9.0 Beta or earlier version user which had used not ISO-8859-1 charset but EUC-KR charset or UTF-8 charset, should change the schema. In 9.0 Beta or earlier version, the precision of CHAR or VARCHAR was specified as byte size. From 9.0 Beta, the precision is specified as character length.

**Adding system locales**

* If you have system locales to add, add them into $CUBRID/conf/cubrid_locales.txt file and run make_locale script. For more details, see :ref:`locale-setting`.

DB Migration
------------

* Since the DB volume of CUBRID 9.0 Beta is not compatible with the DB volume of CUBRID 9.1, it should be migrated with migrate_90beta_to_91 utility.
* Since the DB volume of CUBRID 2008 R4.x or earlier version is not compatible with the DB volume of CUBRID 9.1, it should be migrated with cubrid unloaddb/loaddb utility.
* CUBRID 2008 R3.1 and later don't support GLO and the LOB type replaces the GLO feature. For this reason, applications or schemas that use GLO must be modified to be compatible with LOB(see :ref:`glo-users-migration`).

Reconfiguring Environments for Replication or HA
------------------------------------------------

* From 2008 R4.0, the replication feature is no longer supported; for this reason, it is recommended to reconfigure the DB migration and HA environment for systems in which previous replication versions are used. In addition, for systems that use Linux Heartbeat-based HA feature, which is provided in CUBRID 2008 R2.0 and 2008 R2.1, you must reconfigure to DB migration and the CUBRID Heartbeat-based HA environment for better operational stability(see :ref:`ha-db-migration`).
* To reconfigure the HA environment configuration, see :doc:`/admin/ha` in the manual.

.. _db-migration:

Database Migration Procedures
=============================

.. _migration-from-90beta:

Migration from CUBRID 9.0 Beta to CUBRID 9.1
--------------------------------------------

To migrate DB from CUBRID 9.0 Beta, do the following procedures with "migrate_90beta_to_91 <db_name>" command.

+------------------------------------+-----------------------------------------------+---------------------------------------------+
| Step                               | Linux Environment                             | Windows Environment                         |
+====================================+===============================================+=============================================+
| Step 1: Stop CUBRID Service        | % cubrid service stop                         | Stop CUBRID Service Tray.                   |
+------------------------------------+-----------------------------------------------+---------------------------------------------+
| Step 2: Execute the reserved words | Execute the following command in the directory where the reserved word detection script     |
|          detection script.         | is located.                                                                                 |
|                                    |                                                                                             |
|                                    | Execute migration or identifier modification by checking the detection result               |
|                                    | (For the allowable identifier).                                                             |
|                                    |                                                                                             |
|                                    |   % csql -S -u dba -i check_reserved.sql testdb                                             |
+------------------------------------+---------------------------------------------------------------------------------------------+
| Step 3: Backup                     | Store the databases.txt file and the configuration files under the conf directory of        |
|         the earlier version DB     | the earlier version in a separate directory.  (3a)                                          |
|                                    |                                                                                             |
|                                    | Execute the cubrid backupdb utility and store the file generated                            |
|                                    | at this point in a separate directory (3b)                                                  |
|                                    |                                                                                             |
|                                    |   % cubrid backupdb -S testdb                                                               |
|                                    +-----------------------------------------------+---------------------------------------------+
|                                    |                                               | Uninstall the earlier version of CUBRID     |
|                                    |                                               |                                             |
|                                    | Keep the existing database.                   | At this time, keep the existing database.   |
+------------------------------------+-----------------------------------------------+---------------------------------------------+
| Step 4: Install new version        | Install new version on the same directory of earlier installation.                          |
|                                    | See :ref:`install-execute`.                                                                 |
+------------------------------------+---------------------------------------------------------------------------------------------+
| Step 5: Database migration         | Run below utility with previous database volume.                                            |
|                                    |                                                                                             |
|                                    |   % migrate_90beta_to_91 testdb                                                             |
+------------------------------------+-----------------------------------------------+---------------------------------------------+
| Step 6: Configure the CUBRID       | Modify the configuration file. At this point, | Start the service by selecting              |
|      environment                   | partially modify the configuration files      | CUBRID Service Tray > [Service Start].      |
|      and start the CUBRID Service  | from the earlier version stored in step (3a)  |                                             |
|                                    | to fit the new version.                       | Start the database server from              |
|                                    |                                               | the command prompt.                         |
|                                    | (For configuring system parameter,            |                                             |
|                                    |                                               |   % cubrid server start testdb              |
|                                    | see :ref:`upgrade` and :doc:`admin/config`)   |                                             |
|                                    |                                               |                                             |
|                                    |   % cubrid service start                      |                                             |
|                                    |                                               |                                             |
|                                    |   % cubrid server start testdb                |                                             |
+------------------------------------+-----------------------------------------------+---------------------------------------------+

.. _migration-from-4x-or-earlier:

Migration from CUBRID 9.0 Beta or earlier to CUBRID 9.1
-------------------------------------------------------

If you are using CUBRID 2008 R3.0 Beta or less and GLO classes, you have to do the additional works(see :ref:`glo-users-migration`).

The following table shows how to perform the migration using the reserved word detection script, check_reserved.sql, which is separately distributed from http://ftp.cubrid.org and the cubrid unloaddb/loaddb utilities. See :doc:`/admin/migration`)

+------------------------------------+---------------------------------------------+---------------------------------------------+
| Step                               | Linux Environment                           | Windows Environment                         |
+====================================+=============================================+=============================================+
| Step C1: Stop CUBRID Service       | % cubrid service stop                       | Stop CUBRID Service Tray.                   |
+------------------------------------+---------------------------------------------+---------------------------------------------+
| Step C2: Execute the reserved      | Execute the following command in the directory where the reserved word detection          |
|         words detection script     | script is located.                                                                        |
|                                    |                                                                                           |
|                                    | Execute migration or identifier modification by checking the detection result             |
|                                    | (For the allowable identifier).                                                           |
|                                    |                                                                                           |
|                                    |   % csql -S -u dba -i check_reserved.sql testdb                                           |
+------------------------------------+-------------------------------------------------------------------------------------------+
| Step C3: Unload the earlier        | Store the databases.txt file and the configuration files under the conf directory         |
|          version of the DB         | of the earlier version in a separate directory (C3a).                                     |
|                                    |                                                                                           |
|                                    | Execute the cubrid unloaddb utility and store the file generated at this point in a       |
|                                    | separate directory(C3b).                                                                  |
|                                    |                                                                                           |
|                                    |   % cubrid unloaddb -S testdb                                                             |
|                                    |                                                                                           |
|                                    | Delete the existing database (C3c).                                                       |
|                                    |                                                                                           |
|                                    |   % cubrid deletedb testdb                                                                |
|                                    +---------------------------------------------+---------------------------------------------+
|                                    |                                             | Uninstall the earlier version of CUBRID.    |
+------------------------------------+---------------------------------------------+---------------------------------------------+
| Step C4: Install new version       | See :ref:`install-execute`                                                                |
+------------------------------------+-------------------------------------------------------------------------------------------+
| Step C5: Database creation and     | Go to the directory where you want to create a database, and create one. (C5a)            |
|          data loading              |                                                                                           |
|                                    |   % cd $CUBRID/databases/testdb                                                           |
|                                    |                                                                                           |
|                                    |   % cubrid createdb testdb                                                                |
|                                    |                                                                                           |
|                                    | Execute the cubrid loaddb utility with the stored files in (C3b). (C5b)                   |
|                                    |                                                                                           |
|                                    |   % cubrid loaddb -s testdb_schema -d testdb_objects -i testdb_indexes testdb             |
+------------------------------------+-------------------------------------------------------------------------------------------+
| Step C6: Back up the new version   |   % cubrid backupdb -S testdb                                                             |
|          of the DB                 |                                                                                           |
+------------------------------------+---------------------------------------------+---------------------------------------------+
| Step C7: Configure the CUBRID      | Modify the configuration file.              | Start the service by selecting              |
|          environment and start     | At this point, partially modify             | CUBRID Service Tray > [Service Start].      |
|          the CUBRID Service        | the configuration files from the earlier    |                                             |
|                                    | version stored in step (C3a) to fit the new |                                             |
|                                    | version(For system parameter settings,      |                                             |
|                                    | see the cautions).                          | Start the database server from the          |
|                                    |                                             | command prompt.                             |
|                                    | (For configuring system paramater,          |                                             |
|                                    |                                             |                                             |
|                                    | see :ref:`upgrade` and :doc:`admin/config`) |   % cubrid server start testdb              |
|                                    |                                             |                                             |
|                                    |   % cubrid service start                    |                                             |
|                                    |                                             |                                             |
|                                    |   % cubrid server start testdb              |                                             |
+------------------------------------+---------------------------------------------+---------------------------------------------+

.. _glo-users-migration:

Migration for GLO Class Users
-----------------------------

If you use GLO classes, you must modify applications and schema in order to use BLOB or CLOB types, since GLO classes are not supported in 2008 R3.1. If this modification is not easy, it is not recommended to perform the migration.

.. _ha-db-migration:

Database Migration Procedures under HA Environment
==================================================

HA migration from CUBRID 2008 R2.2 or higher to CUBRID 9.1
----------------------------------------------------------

In the scenario described below, the current service is stopped to perform an upgrade in an environment in which a broker, a master DB and a slave DB are operating on different servers.

+------------------------------------------------------+-----------------------------------------------------------------------------------------------------------+
| Step                                                 | Description                                                                                               |
+======================================================+===========================================================================================================+
| Steps H1~H6: Perform steps C1-C6 on the master node. | Run the CUBRID upgrade and database migration in the master node, and back up the new version's database. |
+------------------------------------------------------+-----------------------------------------------------------------------------------------------------------+
| Step H7: Install new version in the slave node       | Delete the previous version of the database from the slave node and install a new version.                |
|                                                      |                                                                                                           |
|                                                      | For more information, see :ref:`install-execute`.                                                         |
+------------------------------------------------------+-----------------------------------------------------------------------------------------------------------+
| Step H8: Restore the backup copy of the master node  | Restore the new database backup copy (testdb_bk*) of the master node, which is created in step H6         |
|          in the slave node                           | , to the slave node.                                                                                      |
|                                                      |                                                                                                           |
|                                                      |   % scp user1\ @master:$CUBRID/databases/databases.txt $CUBRID/databases/.                                |
|                                                      |                                                                                                           |
|                                                      |   % cd ~/DB/testdb                                                                                        |
|                                                      |                                                                                                           |
|                                                      |   % scp user1\ @master:~/DB/testdb/testdb_bk0v000 .                                                       |
|                                                      |                                                                                                           |
|                                                      |   % scp user1\ @master:~/DB/testdb/testdb_bkvinf .                                                        |
|                                                      |                                                                                                           |
|                                                      |   % cubrid restoredb testdb                                                                               |
+------------------------------------------------------+-----------------------------------------------------------------------------------------------------------+
| Step H9: Reconfigure HA environment and start        | In the master node and the slave node, set the CUBRID environment configuration file (cubrid.conf)        |
|          HA mode                                     | and the HA environment configuration file(cubrid_ha.conf)                                                 |
|                                                      | See :ref:`quick-server-config`.                                                                           |
+------------------------------------------------------+-----------------------------------------------------------------------------------------------------------+
| Step H10: Install new version in the broker server,  | For more information about installation, see :ref:`install-execute`.                                      |
|           and start the broker                       |                                                                                                           |
|                                                      | Start the broker in the Broker server. See :ref:`quick-broker-config`.                                    |
|                                                      |                                                                                                           |
|                                                      |   % cubrid broker start                                                                                   |
+------------------------------------------------------+-----------------------------------------------------------------------------------------------------------+

HA Migration from CUBRID 2008 R2.0/R2.1 to CUBRID 9.1
-----------------------------------------------------

If you are using the HA feature of CUBRID 2008 R2.0 or 2008 R2.1, you must upgrade the server version, migrate the database, set up a new HA environment, and then change the Linux Heartbeat auto start setting used in 2008 R2.0 or 2008 R2.1. If the Linux Heartbeat package is not needed, delete it.

Perform steps H1–H10 above, then perform step H11 below:

+-----------------------------------------------------+-------------------------------------------------------------------------------+
| Step                                                | Description                                                                   |
+=====================================================+===============================================================================+
| Step H11: Change the previous Linux heartbeat       | Perform the following task in the master and slave nodes from a root account. |
|           auto start settings                       |                                                                               |
|                                                     |   [root\ @master ~]# chkconfig --del heartbeat                                |
|                                                     |   // Performing the same job in the slave node                                |
+-----------------------------------------------------+-------------------------------------------------------------------------------+
