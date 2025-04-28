
:meta-keywords: scripts
:meta-description: Various CUBRID scripts support CUBRID Managements

***************
cubrid scripts
***************

.. _cubrid_scripts_unloaddb_sh:

unloaddb.sh script
==================

To shorten the execution time of **unloaddb** of a large size database, it is possible to run **cubrid unloaddb** processes concurrently over groups of different tables.
**unloaddb.sh** is a shell script that automates this process.

To evenly distribute unloaddb load over multiple child processes, the **unloadb.sh** gets the size information of the tables to be unloaded from the system catalog (this requires dba privileges).

Tables to be unloaded are allocated to each child process based on the table size, and each child process simultaneously unloads the allocated tables.


.. warning::

	**restrictions:**
	  * Require Linux Environments
	  * Only users with **DBA privileges** can run this script

The following is [options] used in **unloaddb.sh**.

.. program:: sh unloaddb.sh

.. option:: -u user

    Specify a user account of a database to be unloaded. If this is not specified, the default is **DBA** (The **user** must have **DBA privileges**). ::

        sh unloaddb.sh -u dba demodb

.. option:: -t no-process

    Number of child processes to run concurrently (default: 8, maximum 16) ::

        sh unloaddb.sh -t 4 demodb

.. option:: -i input-class-file

    Unload schemas, indexes and data of specified tables in the file. (default: all tables in the database) ::

        sh unloaddb.sh -i table_list.txt demodb

    The following example shows an input file (table_list.txt). ::

        public.table_1
        public.table_2
        ..
        public.table_n

.. option:: -D output-directory

    Specifies the directory in which to create schema and object files. If it is not specified, files are created in the current directory. ::

        sh unloaddb.sh -D /tmp demodb

.. option:: -s

    Specifies only the schema file will be created (**schema only**). ::

        sh unloaddb.sh -s demodb

.. option:: -d

    Specifies only the data file will be created (**data only**). ::

        sh unloaddb.sh -d demodb

.. option:: -v

    Verbose mode. Show additional information during process (for example size of tables) (**verbose mode**). ::

        sh unloaddb.sh -v demodb


**Files/Directory created by unloaddb.sh**

* single schema file for all tables unloaded: **{database name}_schema**
* object files (as many as the number of child processes, suffix starts from 0)
    * {database name}_0_objects, {database name}_1_objects, ..., {database name}_n-1_objects
* LOG files / Directory:
    * **{database name}_unloaddb.log** will be created.
    * Various log-files will be created here.

  **unloaddb.sh example** ::

	sh unloaddb.sh -t 4 demodb

  **Files/Directory** created by the result of this command: ::

     * demodb_schema: schema file for all tables unloaded
     * demodb_0_objects: object files unloaded by the 1st child process
     * demodb_1_objects: object files unloaded by the 2nd child process
     * demodb_2_objects: object files unloaded by the 3rd child process
     * demodb_3_objects: object files unloaded by the 4th child process
     * demodb_unloaddb.log/demodb_0.files: table names unloaded by 1st child process
     * demodb_unloaddb.log/demodb_0.pid: process id of 1st child process
     * demodb_unloaddb.log/demodb_0_unloaddb.log: 'cubrid unloaddb' log of the 1st child process
     * demodb_unloaddb.log/demodb_0.status: execution result of the 1st child process (success/fail)

.. note::

   * If the interrupt key (**CTRL-C**) is pressed before termination of **unloaddb.sh**, all in-progress unloaddb **object files** will be **deleted** (object files that unload completed will not be deleted).


.. _restore_to_newdb_sh:


restore_to_newdb.sh script
============================


When data recovery is required due to user errors or system failures, the recovery process typically involves restoring a backup to the production server. This often requires additional steps such as creating new user accounts or installing database engines, making the process cumbersome. \

The restore_to_newdb.sh script allows you to restore a backup and create a new database instance under a new database name without any additional setup, using the existing CUBRID environment.

.. warning::

   **Restrictions:**
     * Only available on Linux environments.
     * The backup files for the target database must exist.
     * The new database name must differ from the original database name.
     * The new database name must not already be registered in $CUBRID/databases.txt.


**Basic Usage** ::

	sh restore_to_new.sh [options] backuped-database-name new-database-name

* **restore_to_new.sh** : A script that restores a backup and creates a new DBMS instance.
* **backup-database-name** : Name of the backed-up database to be restored.
* **new-database-name** : Name of the newly created database after restoration.


**[Options] for restore_to_new.sh**

.. program:: sh restore_to_newdb.sh backuped-database-name new-database-name

.. option:: -F path

	Specifies the absolute path for the new database. If not specified, the current path is used by default. ::

		sh restore_to_newdb.sh -F /home/cubrid/newdbpath backuped-database-name new-database-name

.. option:: -B path

    Specifies the absolute path where the backup files are located. If not specified, the current path is used by default. ::

		sh restore_to_newdb.sh -B /home/cubrid/backupdbpath backuped-database-name new-database-name

.. option:: -d date

	Restores the database up to a specific point in time.  The format must be `dd-mm-yyyy:hh:mi:ss` (e.g., `14-10-2008:14:10:00`).
	This option works the same as the `-d` option in `cubrid restoredb`. ::

		sh restore_to_newdb.sh backuped-database-name new-database-name -F /home/cubrid -B /home/cubrid/DB/backupdbpath -d 30-10-2025:12:20:00

.. option:: -l level

    Specifies the backup level (0, 1, or 2) to restore from. This option works the same as the `-l` option in `cubrid restoredb`. ::

		sh restore_to_newdb.sh -B /home/cubrid/DB/backupdbpath -l 1 backuped-database-name new-database-name

.. option:: -p

    Performs a partial restore when log archives are unavailable. Refer to the `-p` option in `cubrid restoredb` for more details. ::

		sh restore_to_newdb.sh -B /home/cubrid/DB/backupdbpath -p backuped-database-name new-database-name

.. option:: -k path

    Specifies the path of the key file required for restoration. Refer to the `-k` option in `cubrid restoredb` for more details. ::

		sh restore_to_newdb.sh -B /home/cubrid/DB/backupdbpath -k /home/cubdev/DB/backupdbpath/backupdb_bk1_keys backuped-database-name new-database-name

**Example of Running restore_to_newdb.sh**

::

	sh restore_to_newdb.sh -B /home/cubrid/DB/backupdbpath backuped-database-name new-database-name

::

	Validation passed: newdb_path and vol-path are different.
	Validation passed: 'new-database-name' does not exist in /home/cubrid/CUBRID-11.3.1.1139-6aaf4dd-Linux.x86_64/databases/databases.txt.

	Confirmed: Files found for pattern (/home/cubrid/DB/backupdbpath/backuped-database-name_bk0v*).
	file: /home/cubrid/DB/backupdbpath/backuped-database-name_bk0v*
	Warning: /home/cubrid/CUBRID-11.3.1.1139-6aaf4dd-Linux.x86_64/databases/databases.txt already exists.
	Existing databases.txt backed up as databases.txt.bak.
	Restoring... /

	CUBRID 11.3

	Restoring... |Updated CUBRID-11.3.1.1139-6aaf4dd-Linux.x86_64/databases/databases.txt
	successfully.
	database restoration completed successfully.


Once the database is successfully restored, start the new DBMS as follows:

::

	[cubrid ~]$ cubrid server start new-database-name
	@ cubrid server start: new-database-name

	This may take a long time depending on the amount of recovery works to do.

	CUBRID 11.3

	++ cubrid server start: success
	Calling java stored procedure is allowed

.. note::
* After the restoration, it is essential to clean up and verify the DBMS environment.
* Since the restoration modifies $CUBRID_DATABASES, ensure that the database list is properly updated.



**Checking the Log File After Running restore_to_newdb.sh**

After the restoration, a log file named restoredb_YYYYMMDDhhmiss.log will be created in the same directory where the script was executed.
If any issues occur, you can check this log file to review the restoration history.

::

	[cubrid ~]$ cat restoredb_20250411185128.log

	[ Database(db1) Restore (level = 0) start ]

	- restore start time: Fri Api 11 18:51:28 2025

	- restore steps: 1
	 step 1) restore using (level = 0) backup data

	- restore progress status (using level = 0 backup data)
	 -----------------------------------------------------------------------------
	 volume name                  | # of pages |  restore progress status  | done
	 -----------------------------------------------------------------------------
	 dbms_keys                     |          1 | ######################### | done
	 dbms_vinf                     |          1 | ######################### | done
	 dbms                          |      32768 | ######################### | done
	 dbms_lgar001                  |      32768 | ######################### | done
	 dbms_lgar002                  |      32768 | ######################### | done
	 dbms_lginf                    |          1 | ######################### | done
	 dbms_lgat                     |      32768 | ######################### | done
	 -----------------------------------------------------------------------------

	- restore end time: Fri Api 11 18:51:30 2025

	[ Database(db1) Restore (level = 0) end ]



