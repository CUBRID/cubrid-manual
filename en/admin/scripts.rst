
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




