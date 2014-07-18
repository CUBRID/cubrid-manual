.. _upgrade:

Upgrade
=======

Cautions during upgrade
-----------------------

**Saving the Existing Configuration File**

*   Save the configuration files in the $CUBRID/conf directory (cubrid.conf, cubrid_broker.conf and cm.conf) and the DB location file (databases.txt) in the $CUBRID_DATABASES directory.

**Checking New Reserved Words**

*   You can check whether reserved words are being used or not by applying the CUBRID 9.3 reserved word detection script, check_reserved.sql, which is distributed through the CUBRID installation package or http://ftp.cubrid.org/CUBRID_Engine/9.3.0//. If the reserved words are being used as identifiers, the identifiers must be modified. See :doc:`sql/identifier`.

**Configuring environment variables of CUBRID_MSG_LANG**

*   CUBRID_LANG and CUBRID_CHARSET environment variables are no more used, and the language and the charset should be configured during creating DB. CUBRID_MSG_LANG is used when displaying the messages of utilities or errors. If CUBRID_MSG_LANG is not configured, it follows the language and the charset specified when creating DB.

**Changing schema**

*   9.0 Beta or earlier version user which had used not ISO-8859-1 charset but EUC-KR charset or UTF-8 charset, should change the schema. In the earlier version of 9.0 Beta, the precision of CHAR or VARCHAR was specified as byte size. From 9.0 Beta, the precision is specified as character length.

**Adding/Keeping locales**

*   If you have locales to want to add, add them into $CUBRID/conf/cubrid_locales.txt file and run make_locale script. For more details, see :ref:`locale-setting`.
*   If you want to keep the old version's locale, add the old version's locale to $CUBRID/conf/cubrid_locales.txt file and run make_locale script, and specify the old locale when running createdb command.

**DB migration**

*   Since the DB volume of CUBRID 9.3 is compatible with the DB volume of CUBRID 9.2, DB migration is needless.
*   Since the DB volume of CUBRID 9.3 is not compatible with the DB volume of CUBRID 9.1, it should be migrated with migrate_91_to_92 utility. For more detail procedure, see :ref:`migration-from-91`.
*   Since the DB volume of CUBRID 9.0 Beta, 2008 R4.x or earlier version is not compatible with the DB volume of CUBRID 9.3, it should be migrated with cubrid unloaddb/loaddb utility. For more detail procedure, see :ref:`migration-from-41`.
*   CUBRID 2008 R3.1 and later don't support GLO and the LOB type replaces the GLO feature. For this reason, applications or schemas that use GLO must be modified to be compatible with LOB.

.. note::

    In 2008 R4.0 or before, TIMESTAMP '1970-01-01 00:00:00'(GMT) is the minimum value of TIMESTAMP, but in 2008 4.1 or later, it is recognized as zerodate and TIMESTAMP '1970-01-01 00:00:01'(GMT) is the minimum value of TIMESTAMP.

**Reconfiguring environments for replication or HA**

*   From 2008 R4.0, the replication feature is no longer supported; therefore, it is recommended to reconfigure the DB migration and HA environment for systems in which previous replication versions are used. In addition, for systems that use Linux Heartbeat-based HA feature, which is provided in CUBRID 2008 R2.0 and 2008 R2.1, you must reconfigure to DB migration and the CUBRID Heartbeat-based HA environment for better operational stability(see :ref:`ha-db-migration`).
*   To reconfigure the HA environment configuration, see :doc:`ha` in the manual.

**Java Stored Function/Procedure**

*   A user who uses Java stored function/procedure should run loadjava command to load Java classes into CUBRID. See :doc:`/sql/jsp`.

Upgrading from CUBRID 9.2 to CUBRID 9.3
---------------------------------------

Users who are using versions CUBRID 9.2 should install 9.3 in the same directory and modify parameter values in the previous environment configuration file.

DB migration
^^^^^^^^^^^^

Since the DB volume of CUBRID 9.3 is compatible with the DB volume of CUBRID 9.2, DB migration is needless.

Parameter configuration
^^^^^^^^^^^^^^^^^^^^^^^

**cubrid.conf**

*   The minimum size of log_buffer_size is changed from 48KB(3*1page, 16KB=1page) into 2MB(128*1page, 16KB=1page); therefore, this value should be larger than the changed minimum size.

.. _up-from-91:

Upgrading from CUBRID 9.1 to CUBRID 9.3
---------------------------------------

Users who are using versions CUBRID 9.1 should install 9.3 in the same directory and modify parameter values in the previous environment configuration file.

.. _migration-from-91:

DB migration
^^^^^^^^^^^^

To migrate DB from CUBRID 9.1, do the following procedures with "migrate_91_to_92 <db_name>" command.

+------------------------------------+-----------------------------------------------+-----------------------------------------------+
| Step                               | Linux Environment                             | Windows Environment                           |
+====================================+===============================================+===============================================+
| Step 1: Stop CUBRID Service        | % cubrid service stop                         | Stop CUBRID Service Tray.                     |
+------------------------------------+-----------------------------------------------+-----------------------------------------------+
| Step 2: Execute the reserved words | Execute the following command in the directory where the reserved word detection script       |
|          detection script.         | is located.                                                                                   |
|                                    |                                                                                               |
|                                    | Execute migration or identifier modification by checking the detection result                 |
|                                    | (For the allowable identifier).                                                               |
|                                    |                                                                                               |
|                                    |   % csql -S -u dba -i check_reserved.sql testdb                                               |
+------------------------------------+-----------------------------------------------------------------------------------------------+
| Step 3: Backup                     | Store the databases.txt file and the configuration files under the conf directory of          |
|         the earlier version DB     | the earlier version in a separate directory.  (3a)                                            |
|                                    |                                                                                               |
|                                    | Execute the cubrid backupdb utility and store the file generated                              |
|                                    | at this point in a separate directory (3b)                                                    |
|                                    |                                                                                               |
|                                    |   % cubrid backupdb -S testdb                                                                 |
|                                    +-----------------------------------------------+-----------------------------------------------+
|                                    |                                               | Uninstall the earlier version of CUBRID       |
|                                    |                                               |                                               |
|                                    | Keep the existing database.                   | At this time, keep the existing database.     |
+------------------------------------+-----------------------------------------------+-----------------------------------------------+
| Step 4: Install new version        | Install the new version on the same directory of earlier installation.                        |
|                                    | See :ref:`install-execute`.                                                                   |
|                                    | At this time, set cubrid.conf and cubrid_locales.txt as the same with 9.1 version and         |
|                                    | run make_locale script.                                                                       |
+------------------------------------+-----------------------------------------------------------------------------------------------+
| Step 5: Database migration         | Run below utility with the previous database volume.                                          |
|                                    |                                                                                               |
|                                    |   % migrate_91_to_92 testdb                                                                   |
+------------------------------------+-----------------------------------------------+-----------------------------------------------+
| Step 6: Configure the CUBRID       | Modify the configuration file. At this point, | Start the service by selecting                |
|      environment                   | partially modify the configuration files      | CUBRID Service Tray > [Service Start].        |
|      and start the CUBRID Service  | from the earlier version stored in step (3a)  |                                               |
|                                    | to fit the new version.                       | Start the database server from                |
|                                    |                                               | the command prompt.                           |
|                                    | (For configuring system parameter, see        |                                               |
|                                    | :ref:`conf-from-91` and :doc:`admin/config`)  |   % cubrid server start testdb                |
|                                    |                                               |                                               |
|                                    |   % cubrid service start                      |                                               |
|                                    |                                               |                                               |
|                                    |   % cubrid server start testdb                |                                               |
+------------------------------------+-----------------------------------------------+-----------------------------------------------+

.. _conf-from-91:

Parameter configuration
^^^^^^^^^^^^^^^^^^^^^^^

**cubrid.conf**

*   The minimum size of log_buffer_size is changed from 48KB(3*1page, 16KB=1page) into 2MB(128*1page, 16KB=1page); therefore, this value should be larger than the changed minimum size.
*   The value of sort_buffer_size should be configured as 2G or less since the maximum value of sort_buffer_size is 2G.
*   In the following parameters, the old parameters will be deprecated and the new parameters are recommended to use. the value in the parenthesis is the unit of the value when the unit is omitted, and the new parameters can specify the unit after the value. For details, see each parameter's explanation in :doc:`/admin/config`

    +-----------------------------------------+-----------------------------------------+
    | Old parameters(unit)                    | New parameters(unit)                    |
    +=========================================+=========================================+
    | lock_timeout_in_secs(sec)               | lock_timeout(msec)                      |
    +-----------------------------------------+-----------------------------------------+
    | checkpoint_every_npages(page_count)     | checkpoint_every_size(byte)             |
    +-----------------------------------------+-----------------------------------------+
    | checkpoint_interval_in_mins(min)        | checkpoint_interval(msec)               |
    +-----------------------------------------+-----------------------------------------+
    | max_flush_pages_per_second(page_count)  | max_flush_size_per_second(byte)         |
    +-----------------------------------------+-----------------------------------------+
    | sync_on_nflush(page_count)              | sync_on_flush_size(byte)                |
    +-----------------------------------------+-----------------------------------------+
    | sql_trace_slow_msecs(msec)              | sql_trace_slow(msecs)                   |
    +-----------------------------------------+-----------------------------------------+
    
**cubrid_broker.conf**

*   In KEEP_CONNECTION parameter, OFF value should be changed as ON or AUTO since OFF setting value is no longer used. 
*   SELECT_AUTO_COMMIT should be deleted since this parameter is no longer used.
*   The value of APPL_SERVER_MAX_SIZE_HARD_LIMIT should be 2,097,151 or less since the maximum value of APPL_SERVER_MAX_SIZE_HARD_LIMIT is 2,097,151.

**Environment variable**

*   CUBRID_CHARSET is removed, and now CUBRID_CHARSET is used for configuring the charset of database and CUBRID_MSG_LANG is used for configuring the charset of messages for utilities and errors.

    .. warning::

        When you create database, a language and a charset must be specified. It affects the length of string type, string comparison operation, etc. The specified charset when creating database cannot be changed later, so you should be careful when specifying it.
        
        For charset, locale and collation setting, see :doc:`/sql/i18n`.

For more details, see :ref:`changed-config100`.

.. _up-from-41:

Upgrading From CUBRID 2008 R4.1/R4.3/R4.4 To CUBRID 9.3
-------------------------------------------------------

Users who are using a version of CUBRID 2008 R4.1, R4.3 or R4.4 should install 9.3 in the different directory and modify parameter values in the existing environment configuration file.

.. _migration-from-41:

DB migration
^^^^^^^^^^^^

The following table shows how to perform the migration using the reserved word detection script, check_reserved.sql, which is separately distributed from http://ftp.cubrid.org/CUBRID_Engine/9.3.0/Linux/ and the cubrid unloaddb/loaddb utilities. (See :ref:`unload-db` and :ref:`load-db`)

+------------------------------------+-----------------------------------------------+-----------------------------------------------+
| Step                               | Linux Environment                             | Windows Environment                           |
+====================================+===============================================+===============================================+
| Step C1: Stop CUBRID Service       | % cubrid service stop                         | Stop CUBRID Service Tray.                     |
+------------------------------------+-----------------------------------------------+-----------------------------------------------+
| Step C2: Execute the reserved      | Execute the following command in the directory where the reserved word detection              |
|         words detection script     | script is located.                                                                            |
|                                    |                                                                                               |
|                                    | Execute migration or identifier modification by checking the detection result                 |
|                                    | (For the allowable identifier).                                                               |
|                                    |                                                                                               |
|                                    |   % csql -S -u dba -i check_reserved.sql testdb                                               |
+------------------------------------+-----------------------------------------------------------------------------------------------+
| Step C3: Unload the earlier        | Store the databases.txt file and the configuration files under the conf directory             |
|          version of the DB         | of the earlier version in a separate directory (C3a).                                         |
|                                    |                                                                                               |
|                                    | Execute the cubrid unloaddb utility and store the file generated at this point in a           |
|                                    | separate directory(C3b).                                                                      |
|                                    |                                                                                               |
|                                    |   % cubrid unloaddb -S testdb                                                                 |
|                                    |                                                                                               |
|                                    | Delete the existing database (C3c).                                                           |
|                                    |                                                                                               |
|                                    |   % cubrid deletedb testdb                                                                    |
|                                    +-----------------------------------------------+-----------------------------------------------+
|                                    |                                               | Uninstall the earlier version of CUBRID.      |
+------------------------------------+-----------------------------------------------+-----------------------------------------------+
| Step C4: Install new version       | See :ref:`install-execute`                                                                    |
+------------------------------------+-----------------------------------------------------------------------------------------------+
| Step C5: Database creation and     | Go to the directory where you want to create a database, and create one.                      |
|          data loading              | At this time, be cautious about locale setting(\*). (c5a)                                     |
|                                    |                                                                                               |
|                                    |   % cd $CUBRID/databases/testdb                                                               |
|                                    |                                                                                               |
|                                    |   % cubrid createdb testdb en_US                                                              |
|                                    |                                                                                               |
|                                    | Execute the cubrid loaddb utility with the stored files in (C3b). (C5b)                       |
|                                    |                                                                                               |
|                                    |   % cubrid loaddb -s testdb_schema -d testdb_objects -i testdb_indexes testdb                 |
+------------------------------------+-----------------------------------------------------------------------------------------------+
| Step C6: Back up the new version   |   % cubrid backupdb -S testdb                                                                 |
|          of the DB                 |                                                                                               |
+------------------------------------+-----------------------------------------------+-----------------------------------------------+
| Step C7: Configure the CUBRID      | Modify the configuration file.                | Start the service by selecting                |
|          environment and start     | At this point, partially modify               | CUBRID Service Tray > [Service Start].        |
|          the CUBRID Service        | the configuration files from the earlier      |                                               |
|                                    | version stored in step (C3a) to fit the new   | Start the database server from the            |
|                                    | version.                                      | command prompt.                               |
|                                    |                                               |                                               |
|                                    | (For configuring system parameter, see        |   % cubrid server start testdb                |
|                                    | :ref:`conf-from-41` and :doc:`admin/config`)  |                                               |
|                                    |                                               |                                               |
|                                    |   % cubrid service start                      |                                               |
|                                    |                                               |                                               |
|                                    |   % cubrid server start testdb                |                                               |
+------------------------------------+-----------------------------------------------+-----------------------------------------------+

(\*): The user which uses CUBRID 2008 R4.x or before should be cautious for determining a locale(language and charset). For example, when the user which used the language as ko_KR(Korean) and the charset as utf8 processes DB migration, the locale should be set as "cubrid createdb testdb ko_KR.utf8". If the locale is not built-in locale, you should run make_locale(.sh) command first. For more details, see :ref:`locale-setting`. 

*   You should be careful about the change of the space for storing about the multibyte character. For example, in 2008 R4.3, CHAR(6) means CHAR type with 6 bytes size, but in 9.3, CHAR(6) means CHAR type with 6 characters. In utf8 charset, Korean uses 3 bytes per 1 character, so CHAR(6) has 18 bytes. Therefore, more disk space is required. 

*   If you used utf8 charset in CUBRID 2008 R4.x or before, you should set the charset as utf8 when you run "cubrid createdb". If not, retrieval queries or string functions are unable to work properly.

.. _conf-from-41:

Parameter configuration
^^^^^^^^^^^^^^^^^^^^^^^

**cubrid.conf**

*   The minimum size of log_buffer_size is changed from 48KB(3*1page, 16KB=1page) into 2MB(128*1page, 16KB=1page); therefore, this value should be larger than the changed minimum size.
*   The value of sort_buffer_size should be configured as 2G or less since the maximum value of sort_buffer_size is 2G.
*   single_byte_compare should be deleted since this parameter is no longer used.
*   intl_mbs_support should be deleted since this parameter is no longer used.
*   lock_timeout_message_type should be deleted since this parameter is no longer used.
*   In the following parameters, the old parameters will be deprecated and the new parameters are recommended to use. the value in the parenthesis is the unit of the value when the unit is omitted, and the new parameters can specify the unit after the value. For details, see each parameter's explanation in :doc:`/admin/config`

    +-----------------------------------------+-----------------------------------------+
    | Old parameters(unit)                    | New parameters(unit)                    |
    +=========================================+=========================================+
    | lock_timeout_in_secs(sec)               | lock_timeout(msec)                      |
    +-----------------------------------------+-----------------------------------------+
    | checkpoint_every_npages(page_count)     | checkpoint_every_size(byte)             |
    +-----------------------------------------+-----------------------------------------+
    | checkpoint_interval_in_mins(min)        | checkpoint_interval(msec)               |
    +-----------------------------------------+-----------------------------------------+
    | max_flush_pages_per_second(page_count)  | max_flush_size_per_second(byte)         |
    +-----------------------------------------+-----------------------------------------+
    | sync_on_nflush(page_count)              | sync_on_flush_size(byte)                |
    +-----------------------------------------+-----------------------------------------+
    | sql_trace_slow_msecs(msec)              | sql_trace_slow(msecs)                   |
    +-----------------------------------------+-----------------------------------------+

**cubrid_broker.conf**

*   In KEEP_CONNECTION parameter, OFF value should be changed as ON or AUTO since OFF setting value is no longer used. 
*   SELECT_AUTO_COMMIT should be deleted since this parameter is no longer used.
*   The value of APPL_SERVER_MAX_SIZE_HARD_LIMIT should be 2,097,151 or less since the maximum value of APPL_SERVER_MAX_SIZE_HARD_LIMIT is 2,097,151.
    
**cubrid_ha.conf**

*   Users who have configured the ha_apply_max_mem_size parameter value more than 500 must the value to 500 or less.

**Environment variable**

*   CUBRID_LANG is removed; now the language and the charset of database is set when creating DB, and CUBRID_MSG_LANG is used for configuring the charset of messages for utilities and errors.

    .. warning::

        When you create database, the language and the charset of database should be specified. It affects the length of string type, string comparison operation, etc. The specified charset when creating database cannot be changed later, so you should be careful when specifying it.
        
        For charset, locale and collation setting, see :doc:`/sql/i18n`.

For more details, see :ref:`changed-config100`.

.. _up-from-40:

Upgrading From CUBRID 2008 R4.0 or Earlier Versions To CUBRID 9.3
-----------------------------------------------------------------

Users who are using versions CUBRID 2008 R4.0 or earlier should install 9.3 in the different directory and modify parameter values in the existing environment configuration file.

DB migration
^^^^^^^^^^^^

Do the same procedures with :ref:`migration-from-41` in :ref:`up-from-41`. If you use GLO classes, you must modify applications and schema in order to use BLOB or CLOB types, since GLO classes are not supported in 2008 R3.1. If this modification is not easy, it is not recommended to perform the migration.

Parameter configuration
^^^^^^^^^^^^^^^^^^^^^^^

**cubrid.conf**

*   The minimum size of log_buffer_size is changed from 48KB(3*1page, 16KB=1page) into 2MB(128*1page, 16KB=1page); therefore, this value should be larger than the changed minimum size.
*   The value of sort_buffer_size should be configured as 2G or less since the maximum value of sort_buffer_size is 2G.
*   single_byte_compare should be deleted since this parameter is no longer used.
*   intl_mbs_support should be deleted since this parameter is no longer used.
*   lock_timeout_message_type should be deleted since this parameter is no longer used.
*   Because the default value of thread_stacksize has been changed from 100K to 1M, it is recommended that users who have not configured this value check memory usage of CUBRID-associative processes.
*   Because the minimum value of data_buffer_size has been changed from 64K to 16M, users who have configured this value less than 16M must change the value equal to or greater than 16M.
*   In the following parameters, the old parameters will be deprecated and the new parameters are recommended to use. the value in the parenthesis is the unit of the value when the unit is omitted, and the new parameters can specify the unit after the value. For details, see each parameter's explanation in :doc:`/admin/config`

    +-----------------------------------------+-----------------------------------------+
    | Old parameters(unit)                    | New parameters(unit)                    |
    +=========================================+=========================================+
    | lock_timeout_in_secs(sec)               | lock_timeout(msec)                      |
    +-----------------------------------------+-----------------------------------------+
    | checkpoint_every_npages(page_count)     | checkpoint_every_size(byte)             |
    +-----------------------------------------+-----------------------------------------+
    | checkpoint_interval_in_mins(min)        | checkpoint_interval(msec)               |
    +-----------------------------------------+-----------------------------------------+
    | max_flush_pages_per_second(page_count)  | max_flush_size_per_second(byte)         |
    +-----------------------------------------+-----------------------------------------+
    | sync_on_nflush(page_count)              | sync_on_flush_size(byte)                |
    +-----------------------------------------+-----------------------------------------+

**cubrid_broker.conf**

*   In KEEP_CONNECTION parameter, OFF value should be changed as ON or AUTO since OFF setting value is no longer used. 
*   SELECT_AUTO_COMMIT should be deleted since this parameter is no longer used.
*   The value of APPL_SERVER_MAX_SIZE_HARD_LIMIT should be 2,097,151 or less since the maximum value of APPL_SERVER_MAX_SIZE_HARD_LIMIT is 2,097,151.
*   The minimum value of APPL_SERVER_MAX_SIZE_HARD_LIMIT is 1024M. It is recommended that users who configure APPL_SERVER_MAX_SIZE configure this value less than the value of APPL_SERVER_MAX_SIZE_HARD_LIMIT.
*   Because the default value of CCI_DEFAULT_AUTOCOMMIT has been changed to ON, users who have not configured this value should change it to OFF if they want to keep auto commit mode.

**cubrid_ha.conf**

*   Users who have configured the ha_apply_max_mem_size parameter value more than 500 must the value to 500 or less.

**Environment variable**

*   CUBRID_LANG is removed; now the language and the charset of database is set when creating DB, and CUBRID_MSG_LANG is used for configuring the charset of messages for utilities and errors.

    .. warning::

        When you create database, the language and the charset of database should be specified. It affects the length of string type, string comparison operation, etc. The specified charset when creating database cannot be changed later, so you should be careful when specifying it.
        
        For charset, locale and collation setting, see :doc:`/sql/i18n`.

For more details, see :ref:`changed-config100`.

.. _ha-db-migration:

Database Migration under HA Environment
=======================================

HA migration from CUBRID 2008 R2.2 or higher to CUBRID 9.3
----------------------------------------------------------

In the scenario described below, the current service is stopped to perform an upgrade in an environment in which a broker, a master DB and a slave DB are operating on different servers.

+------------------------------------------------------+-----------------------------------------------------------------------------------------------------------+
| Step                                                 | Description                                                                                               |
+======================================================+===========================================================================================================+
| Steps H1~H6: Perform :ref:`migration-from-91` or     | Run the CUBRID upgrade and database migration in the master node, and back up the new version's database. |
| steps C1-C6 of :ref:`migration-from-41`              | on the master node.                                                                                       |
|                                                      |                                                                                                           |
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

HA Migration from CUBRID 2008 R2.0/R2.1 to CUBRID 9.3
-----------------------------------------------------

If you are using the HA feature of CUBRID 2008 R2.0 or 2008 R2.1, you must upgrade the server version, migrate the database, set up a new HA environment, and then change the Linux Heartbeat auto start setting used in 2008 R2.0 or 2008 R2.1. If the Linux Heartbeat package is not needed, delete it.

Perform steps H1~H10 above, then perform step H11 below:

+-----------------------------------------------------+-------------------------------------------------------------------------------+
| Step                                                | Description                                                                   |
+=====================================================+===============================================================================+
| Step H11: Change the previous Linux heartbeat       | Perform the following task in the master and slave nodes from a root account. |
|           auto start settings                       |                                                                               |
|                                                     |   [root\ @master ~]# chkconfig --del heartbeat                                |
|                                                     |   // Performing the same job in the slave node                                |
+-----------------------------------------------------+-------------------------------------------------------------------------------+
