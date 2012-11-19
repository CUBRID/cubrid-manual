******************
Database Migration
******************

Migrating Database
==================

To use a new version of CUBRID database, you may need to migrate an existing data to a new one. For this purpose, you can use the "Export to an ASCII text file" and "Import from an ASCII text file" features provided by CUBRID. 

The following section explains migration steps using the **cubrid unloaddb** and **cubrid loaddb** utilities.

**Recommended Scenario and Procedures**

The following steps describes migration scenario that can be applied while the existing version of CUBRID is running. For database migration, you should use the **cubrid unloaddb** and **cubrid loaddb** utilities. For details, see `Unloading Database <#admin_admin_migration_unload_htm>`_ and `Loading Database <#admin_admin_migration_load_htm>`_ .

#. Stop the existing CUBRID service

	Execute **cubrid service stop** to stop all service processes running on the existing CUBRID and then check whether all CUBRID-related processes have been successfully stopped. 

	To verify whether all CUBRID-related processes have been successfully stopped, execute **ps -ef | grep cub_** in Linux. If there is no process starting with cub\_, all CUBRID-related processes have been successfully stopped. In Windows, press the <Ctrl+Alt+Delete> key and select [Start Task Manager]. If there is no process starting with cub\_ in the [Processes] tab, all CUBRID-related processes have been successfully stopped. In Linux, when the related processes remain even after the CUBRID service has been terminated, use **kill** command to forcibly terminate them, and use **ipcs -m** command to check and release the memory shard by CUBRID broker. To forcibly terminate related processes in Windows, go to the [Processes] tab of Task Manager, right-click the image name, and then select [End Process].

#. Back up the existing database

	Perform backup of the existing version of the database by using the **cubrid backupdb** utility. The purpose of this step is to safeguard against failures that might occur during the database unload/load operations. For details on the database backup, see `Database Backup <#admin_admin_br_backup_htm>`_ .

#. Unload the existing database

	Unload the database created for the existing version of CUBRID by using the **cubrid unloaddb** utility. For details on unloading a database, see `Unloading Database <#admin_admin_migration_unload_htm>`_ .

#. Store the existing CUBRID configuration files

	Store the configurations files such as **cubrid.conf**, **cubrid_broker.conf** and **cm.conf ** in the **CUBRID/conf** directory. The purpose of this step is to conveniently apply parameter values for the existing CUBRID database environment to the new one.

#. Install a new version of CUBRID

	Once backing up and unloading of the data created by the existing version of CUBRID have been completed, delete the existing version of CUBRID and its databases and then install the new version of CUBRID. For details on installing CUBRID, see `Installing and Running on Linux <#gs_gs_install_linux_htm>`_ in "Getting Started."

#. Configure the new CUBRID environment

	Configure the new version of CUBRID by referring to configuration files of the existing database stored in the step 3, " **Store the existing CUBRID configuration files** ." For details on configuring new environment, see `Installing and Running on Windows <#gs_gs_install_windows_htm>`_ in "Getting Started."

#. Load the new database
	Create a database by using the **cubrid createdb** utility and then load the data which had previously been unloaded into the new database by using the **cubrid loaddb** utility. For details on creating a database, see `Creating Database <#admin_admin_db_create_create_htm>`_ in "Administrator's Guide." For details on loading a database, see `Loading Database <#admin_admin_migration_load_htm>`_ .
	
#. Back up the new database

	Once the data has been successfully loaded into the new database, back up the database created for the new version of CUBRID by using the **cubrid backupdb** utility. The reason for this step is because you cannot restore the data backed up in the existing version of CUBRID when using the new version. For details on backing up the database, see `Database Backup <#admin_admin_br_backup_htm>`_ .

.. note:: 

	Even if the version is identical, the 32-bit database volume and the 64-bit database volume are not compatible for backup and recovery. Therefore, it is not recommended to recover a 32-bit database backup on the 64-bit CUBRID or vice versa.

Unloading Database
==================

The purposes of loading/unloading databases are as follows:

*   To rebuild databases by volume reconfiguration
*   To migrate database in different system environments
*   To migrate database in different versions

::

	cubrid unloaddb [options] database_name

**cubrid unloaddb** utility creates the following files:

*   Schema file (*database-name* **_schema**): A file that contains information on the schema defined in the database.
*   Object file (*database-name* **_objects**): A file that contains information on the records in the database.
*   Index file (*database-name* **_indexes**): A file that contains information on the indexes defined in the database.
*   Trigger file (*database-name* **_trigger**): A file that contains information on the triggers defined in the database. If you don't want triggers to be running while loading the data, load the trigger definitions after the data loading has completed.

The schema, object, index, and trigger files are created in the same directory.

The following is [options] used in **cubrid unloaddb**.

.. program:: unloaddb

.. option:: -i, --input-class-file FILE

	This option specifies the name of the file which stored the names of classes to unload. ::

		cubrid unloaddb -i table_list.txt demodb

	The following example shows an input file (table_list.txt). ::

		table_1
		table_2
		..
		table_n

	This option can be used together with the **--input-class-only** option that creates the schema file related to only those tables included in the input file. ::

		cubrid unloaddb --input-class-only -i table_list.txt demodb

	This option can be used together with the **--include-reference** option that creates the object reference as well.	::

		cubrid unloaddb --include-reference -i table_list.txt demodb

.. option:: --include-reference

	This option is used together with the **-i** option, and also creates the object reference.

.. option:: --input-class-only

	This option is used together with the **-i** option, and creates only the file of the schemas which are specified by the file of the **-i** option.

.. option:: -O, --output-path PATH

	This option specifies the directory in which to create schema and object files. If this is not specified, files are created in the current directory. ::

		cubrid unloaddb -O ./CUBRID/Databases/demodb demodb

	If the specified directory does not exist, the following error message will be displayed. ::

		unloaddb: No such file or directory.

.. option:: -s, --schema-only

	This option specifies that only the schema file will be created from amongst all the output files which can be created by the unload operation.	::
	
		cubrid unloaddb -s demodb

.. option:: -d, --data-only

	This option specifies that only the data file will be created from amongst all of the output files which can be created by the unload operation. ::

		cubrid unloaddb -d demodb

.. option:: --lo-count

	This option specifies the number of large object (LO) data files to be created in a single. The default value is 0.

.. option:: --estimated-size=NUMBER

	This option allows you to assign hash memory to store records of the database to be unloaded. If the **--estimated-size** option is not specified, the number of records of the database is determined based on recent statistics information. This option can be used if the recent statistics information has not been updated or if a large amount of hash memory needs to be assigned. Therefore, if the number given as the argument for the option is too small, the unload performance deteriorates due to hash conflicts. ::

		cubrid unloaddb --estimated-size=1000 demodb

.. option:: --hash-file

.. option:: --datafile-per-class

	This option specifies that the output file generated through unload operation creates a data file per each table. The file name is generated as *<Database Name>_<Table Name>*\_**objects** for each table. However, all column values in object types are unloaded as NULL and %id class_name class_id part is not written in the unloaded file (see `How to Write a File to Load Database <#admin_admin_migration_file_htm>`_ ). ::

		cubrid unloaddb -d demodb

.. option:: -v, --verbose

	The **-v** option displays detailed information on the database tables and records being unloaded while the unload operation is under way. ::

		cubrid unloaddb -v demodb

.. option:: -S, --SA-mode

	The **-S** option performs the unload operation by accessing the database in standalone mode. ::
	
		cubrid unloaddb -S demodb

.. option:: -C, --CS-mode

	The **-C** option performs the unload operation by accessing the database in client/server mode. ::

		cubrid unloaddb -C demodb

.. option:: --cached-pages

	The **--cached-pages** option specifies the number of pages of tables to be cached in the memory. Each page is 4,096 bytes. The administrator can configure the number of pages taking into account the memory size and speed. If this option is not specified, the default value is 100 pages. ::

		cubrid unloaddb --cached-pages 500 demodb

.. option:: --output-prefix

	The **--output-prefix** option specifies the prefix for the names of schema and object files created by the unload operation. Once the example is executed, the schema file name becomes *abcd_schema* and the object file name becomes *abcd_objects*. If the **--output-prefix** option is not specified, the name of the database to be unloaded is used as the prefix. ::

		cubrid unloaddb --output-prefix abcd demodb

Loading Database
================

You can load a database by using the **cubrid loaddb** utility in the following situations:

*   Migrating previous version of CUBRID database to new version of CUBRID database
*   Migrating a third-party DBMS database to a CUBRID database
*   Inserting massive amount of data faster than using the **INSERT** statement

In general, the **cubrid loaddb** utility uses files (schema definition, object input, and index definition files) created by the **cubrid unloaddb** utility.

	cubrid loaddb [options] database_name

**Input Files**

*   Schema file (*database-name* **_schema**): A file generated by the unload operation; it contains schema information defined in the database.
*   Object file (*database-name* **_objects**): A file created by an unload operation. It contains information on the records in the database.
*   Index file (*database-name* **_indexes**): A file created by an unload operation. It contains information on the indexes defined in the database.
*   Trigger file (*database-name* **_trigger**): A file created by an unload operation. It contains information on the triggers defined in the database.
*   User-defined object file (*user_defined_object_file*): A file in table format written by the user to enter mass data. (For details, see `How to Write Files to Load Database <#admin_admin_migration_file_htm>`_.)

The following table shows options available with the **cubrid loaddb** utility (options are case sensitive).

.. program:: loaddb

.. option:: -u, --user

	The **-u** option specifies the user account of a database where records are loaded. If the option is not specified, the default value is **PUBLIC**. ::

		cubrid loaddb -u admin -d demodb_objects newdb

.. option:: -p, --password

	The **-p** option specifies the password of a database user who will load records. If the option is not specified, you will be prompted to enter the password. ::

		cubrid loaddb -p admin -d demodb_objects newdb

.. option:: -l, --load-only

	The **-l** option loads data directly without checking the syntax for the data to be loaded. The following example shows how to load data included in demodb_objects to newdb.

	If the **-l** option is used, loading speed increases because data is loaded without checking the syntax included in demodb_objects, but an error might occur. ::

		cubrid loaddb -l -d demodb_objects newdb

.. option:: -v, --verbose

	The following example shows how to display detailed information on the tables and records of the database being loaded while the database loading operation is performed. You can check the detailed information such as the progress level, the class being loaded and the number of records entered by using the **-v** option. ::

		cubrid loaddb -v -d demodb_objects newdb

.. option:: -c, --periodic-commit

	The following command performs commit regularly every time 100 records are entered into the newdb by using the **-c** option. If the **-c** option is not specified, all records included in demodb_objects are loaded to newdb before the transaction is committed. If the **-c** option is used together with the **-s** or **-i** option, commit is performed regularly every time 100 DDL statements are loaded. The recommended commit interval varies depending on the data to be loaded. It is recommended that the parameter of the **-c** option be configured to 50 for schema loading, 1,000 for record loading, and 1 for index loading. ::

		cubrid loaddb -c 100 -d demodb_objects newdb

.. option:: -s, --schema-file

	The following statement loads the schema information defined in *demodb* into the newly created newdb database. demodb_schema is a file created by the unload operation and contains the schema information of the unloaded database. You can load the actual records after loading the schema information first by using the **-s** option. ::

		cubrid loaddb -u dba -s demodb_schema newdb

		Start schema loading.
		Total       86 statements executed.
		Schema loading from demodb_schema finished.
		Statistics for Catalog classes have been updated.

	The following satement loads the triggers defined in *demodb* into the newly created newdb database. demodb_trigger is a file created by the unload operation and contains the trigger information of the unloaded database. It is recommended to load the schema information after loading the records. ::

		cubrid loaddb -u dba -s demodb_trigger newdb

.. option:: -i, --index-file

	The following command loads the index information defined in *demodb* into the newly created newdb database. demo_indexes is a file created by the unload operation and contains the index information of the unloaded database. You can create indexes after loading records by using the **-i** option together with the **-d** option. ::

		cubrid loaddb -u dba -i demodb_indexes newdb

.. option:: -d, --data-file

	The following command loads the record information into newdb by specifying the data file or the user-defined object file with the **-d** option. demodb_objects is either an object file created by the unload operation or a user-defined object file written by the user for mass data loading. ::

		cubrid loaddb -u dba -d demodb_objects newdb

.. option:: -t, --table

	The following command specifies the table name after this option if a table name header is omitted in the data file to be loaded. ::

		cubrid loaded -u dba -d demodb_objects -t tbl_name newdb

.. option:: --data-file-check-only

	The following is a command that checks the statements for the data contained in demodb_objects by using the **--data-file-check-only** option. Therefore, the execution of the command below does not load records. ::

		cubrid loaddb --data-file-check-only -d demodb_objects newdb

.. option:: --estimated-size

	The **--estimated-size** option can be used to improve loading performance when the number of records to be unloaded exceeds the default value of 5,000. That is, you can improve the load performance by assigning large hash memory for record storage with this option. ::

		cubrid loaddb --estimated-size 8000 -d demodb_objects newdb

.. option:: --no-oid

	The following is a command that loads records into newdb ignoring the OIDs in demodb_objects. ::
	
		cubrid loaddb --no-oid -d demodb_objects newdb

.. option:: --no-statistics

	The following is a command that does not update the statistics information of newdb after loading demodb_objects. It is useful especially when small data is loaded to a relatively big database; you can improve the load performance by using this command. ::

		cubrid loaddb --no-statistics -d demodb_objects newdb

.. option:: --ignore-class-file

	You can specify a file that lists classes to be ignored during loading records. All records of classes except ones specified in the file will be loaded. ::

		cubrid loaddb --ignore-class-file=skip_class_list -d demodb_objects newdb

.. option:: --error-control-file

	This option specifies the file that describes how to handle specific errors occurring during database loading. ::

		cubrid loaddb --error-control-file=error_test -d demodb_objects newdb

	For the server error code name, see the **$CUBRID/include/dbi.h** file.

	For error messages by error code (error number), see the number under $set 5 MSGCAT_SET_ERROR in th **$CUBRID/msg/** *<character set name>* **/cubrid.msg** file. ::

		vi $CUBRID/msg/en_US/cubrid.msg
		 
		$set 5 MSGCAT_SET_ERROR
		1 Missing message for error code %1$d.
		2 Internal system failure: no more specific information is available.
		3 Out of virtual memory: unable to allocate %1$ld memory bytes.
		4 Has been interrupted.
		...
		670 Operation would have caused one or more unique constraint violations.
		...

	The format of a file that details specific errors is as follows:
	
	*   -<error code>: Configures to ignore the error that corresponds to the <error code> (**loaddb** is continuously executed even when an error occurs while it is being executed).

	*   +<error code>: Configures not to ignore the error that corresponds to the <error code> (**loaddb** is stopped when an error occurs while it is being executed).

	*   +DEFAULT: Configures not to ignore errors from 24 to 33.

	If the file that details errors is not specified by using the **--error-control-file** option, the **loaddb** utility is configured to ignore errors from 24 to 33 by default. As a warning error, it indicates that there is no enough space in the database volume. If there is no space in the assigned database volume, a generic volume is automatically created.

	The following example shows a file that details errors.

	*   The warning errors from 24 to 33 indicating DB volume space is insufficient are not ignored by configuring +DEFAULT.
	*   The error code 2 is not ignored because +2 has been specified later, even when -2 has been specified first.
	*   -670 has been specified to ignore the error code 670, which is a unique violation error.
	*   #-115 has been processed as a comment since # is added. ::

		vi error_file
		 
		+DEFAULT
		-2
		-670
		#-115 --> comment
		+2

**Remark**

The **--no-logging** option enables to load data file quickly when **loaddb** is executed by not storing transaction logs; however, it has risk, which data cannot be recovered in case of errors occurred such as incorrect file format or system failure. In this case, you must rebuild database to solve the problem. Thus, in general, it is not recommended to use this option exception of building a new database which does not require data recovery.

**How to Write Files to Load Database**

You can add mass data to the database more rapidly by writing the object input file used in the **cubrid loaddb** utility. An object input file is a text file in simple table form that consists of comments and command/data lines.

**Comment**

In CUBRID, a comment is represented by two hyphens (--). ::

	-- This is a comment!

**Command Line**

A command line begins with a percent character (%) and consists of **%class** and **%id** commands; the former defines classes, and the latter defines aliases and identifiers used for class identification.

**Assigning an Identifier to a Class**

You can assign an identifier to class reference relationships by using the **%id** command. ::

	%id class_name class_id
	class_name:
		identifier
	class_id:
		integer

The *class_name* specified by the **%id** command is the class name defined in the database, and *class_id* is the numeric identifier which is assigned for object reference.

**Example 1** ::

	%id employee 2
	%id office 22
	%id project 23
	%id phone 24

**Specifying the Class and Attribute**

You can specify the classes (tables) and attributes (columns) upon loading data by using the **%class** command. The data line should be written based on the order of attributes specified. When a class name is provided by using the **-t** option while executing the **cubrid loadd** utility, you don't have to specify the class and attribute in the data file. However, the order of writing data must comply with the order of the attribute defined when creating a class. ::

	%class class_name ( attr_name [attr_name... ] )

The schema must be pre-defined in the database to be loaded.

The *class_name* specified by the **%class** command is the class name defined in the database and the *attr_name* is the name of the attribute defined.

**Example 2**

The following example shows how to specify a class and three attributes by using the **%class** command to enter data into a class named *employee*. Three pieces of data should be entered on the data lines after the **%class** command. For this, see `Example 3 <#admin_admin_migration_file_htm_e_5652>`_ in the "Configuring Reference Relationship" section. ::

	%class employee (name age department)

**Data Line**

A data line comes after the **%class** command line. Data loaded must have the same type as the class attributes specified by the **%class** command. The data loading operation stops if these two types are different.

Data for each attribute must be separated by at least one space and be basically written as a single line. However, if the data to be loaded takes more than one line, you should specify the plus sign (+) at the end of the first data line to enter data continuously on the following line. Note that no space is allowed between the last character of the data and the plus sign.

**Loading an Instance**

As shown below, you can load an instance that has the same type as the specified class attribute. Each piece of data is separated by at least one space.

**Example 1** ::

	%class employee (name)
	'jordan' 
	'james'  
	'garnett'
	'malone'

**Assigning an Instance Number**

You can assign a number to a given instance at the beginning of the data line. An instance number is a unique positive number in the specified class. Spaces are not allowed between the number and the colon (:). Assigning an instance number is used to configure the reference relationship for later.

**Example 2** ::

	%class employee (name)
	1: 'jordan' 
	2: 'james'  
	3: 'garnett' 
	4: 'malone' 

**Configuring Reference Relationship**

You can configure the object reference relationship by specifying the reference class after an "at sign (**@**)" and the instance number after the "vertical line (|)." ::

	@class_ref | instance_no
	class_ref:
		 class_name
		 class_id

Specify a class name or a class id after the **@** sign, and an instance number after a vertical line (|). Spaces are not allowed before and after a vertical line (|).

**Example 3**

The following example shows how to load class instances into the *paycheck* class. The *name* attribute references an instance of the *employee* class. As in the last line, data is loaded as **NULL** if you configure the reference relationship by using an instance number not specified earlier. ::

	%class paycheck(name department salary)
	@employee|1   'planning'   8000000   
	@employee|2   'planning'   6000000  
	@employee|3   'sales'   5000000  
	@employee|4   'development'   4000000
	@employee|5   'development'   5000000

**Example 4**

Since the id 21 was assigned to the *employee* class by using the **%id** command in the `Assigning an Identifier to a Class <#admin_admin_migration_file_htm_i_3325>`_ section, Example 3 can be written as follows: ::

	%class paycheck(name department salary)
	@21|1   'planning'   8000000   
	@21|2   'planning'   6000000  
	@21|3   'sales'   5000000  
	@21|4   'development'   4000000
	@21|5   'development'   5000000
