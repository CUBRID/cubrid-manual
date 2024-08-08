.. _unloaddb:
    
unloaddb
--------

The purposes of loading/unloading databases are as follows:

*   To rebuild databases by volume reconfiguration
*   To migrate database in different system environments
*   To migrate database in different versions

::

    cubrid unloaddb [options] database_name

**cubrid unloaddb** utility creates the following files:

*   Schema file(*database-name*\ **_schema**): A file that contains information on the schema defined in the database.
*   Object file(*database-name*\ **_objects**): A file that contains information on the records in the database.
*   Index file(*database-name*\ **_indexes**): A file that contains information on the indexes defined in the database.
*   Trigger file(*database-name*\ **_trigger**): A file that contains information on the triggers defined in the database. If you don't want triggers to be running while loading the data, load the trigger definitions after the data loading has completed.
*   Split schema files (*database-name*\ **_schema**\ **_objecttype**): When using the **\-\-split-schema-files** option, the schema information defined in the database is split into object units and These files contain only object information. For a list of split files, see the **\-\-split-schema-files** option.
*   Schema list file (*database-name*\ **_schema**\ **_info**): When using the **\-\-split-schema-files** option, this file contains a list of split schema information files.

The schema, object, index, and trigger files are created in the same directory.

The following is [options] used in **cubrid unloaddb**.

.. program:: unloaddb

::

  -u, --user=ID               user ID for database access
  -p, --password=PASS         user password
  -i, --input-class-file=FILE input FILE of table names
      --include-reference     include referenced tables
      --input-class-only      include specified class only
      --estimated-size=NUMBER estimated NUMBER of instances
      --cached-pages=NUMBER   NUMBER of cached pages
  -O, --output-path=PATH      PATH of output directory
  -s, --schema-only           dump schema only
  -d, --data-only             dump objects only
      --output-prefix=PREFIEX PREFIX for output files
      --hash-file=FILE        FILE for hash
      --latest-image          when dumping objects, it dumps the latest image of objects in data volumes.
  -t, --thread-count=COUNT    number of threads; default: 1 maximum: 127
      --enhanced-estimates    obtain the exact number of records in the table to unload; default: disabled
  -v, --verbose               enable verbose status messages
      --use-delimiter         use '"' where an identifier begins and ends
  -S, --SA-mode               stand-alone mode execution
  -C, --CS-mode               client-server mode execution
      --datafile-per-class    create a object file for each class
      --split-schema-files    split schema information by object and generate each file
      --skip-index-detail     Do not print the specified WITH clause options when creating the index
      --as-dba                If the login user is a member of the DBA group, extract the same schema file as the DBA.


.. option:: -u, --user=ID

    Specify a user account of a database to be unloaded. If this is not specified, the default is **DBA**.

    ::

        cubrid unloaddb -u dba -i table_list.txt demodb

.. option:: -p, --password=PASS

    Specify a user's password of a database to be unloaded. If this is not specified, it is regarded as the empty string is entered.

    ::

        cubrid unloaddb -u dba -p dba_pwd -i table_list.txt demodb

.. option:: -i, --input-class-file FILE

    Unload all schema and index of all tables; however, only the data of specified tables in this file are unloaded. If the to-be-unloaded schema (See the -u option) of the user and the schema of the table are different, the table name must be prefixed with the schema name.
    
    ::

        cubrid unloaddb -i table_list.txt demodb

    The following example shows an input file (table_list.txt). ::

        table_1
        table_2
        ..
        table_n

    If this option is used together with the **\-\-input-class-only** option, it creates schema, index, and data files of tables only specified in the input file of **-i** option.
    
    ::

        cubrid unloaddb --input-class-only -i table_list.txt demodb

    If this option is used together with the **\-\-include-reference** option, it unloads the referenced tables as well.
    
    ::

        cubrid unloaddb --include-reference -i table_list.txt demodb

.. option:: --include-reference

    This option is used together with the **-i** option, and also unloads the referenced tables.

.. option:: --input-class-only

    This option is used together with the **-i** option, and creates only a schema file of tables specified in the input file of **-i** option.

.. option:: --estimated-size=NUMBER

    This option allows you to assign hash memory to store records of the database to be unloaded. If the **\-\-estimated-size** option is not specified, the number of records of the database is determined based on recent statistics information. This option can be used if the recent statistics information has not been updated or if a large amount of hash memory needs to be assigned. Therefore, if the number given as the argument for the option is too small, the unload performance deteriorates due to hash conflicts.
    
    ::

        cubrid unloaddb --estimated-size=1000 demodb

.. option:: --cached-pages=NUMBER

    The **\-\-cached-pages** option specifies the number of pages of tables to be cached in the memory. Each page is 4,096 bytes. The administrator can configure the number of pages taking into account the memory size and speed. If this option is not specified, the default value is 100 pages.
    
    ::

        cubrid unloaddb --cached-pages 500 demodb

.. option:: -O, --output-path=PATH

    This option specifies the directory in which to create schema and object files. If this is not specified, files are created in the current directory.
    
    ::

        cubrid unloaddb -O ./CUBRID/Databases/demodb demodb

    If the specified directory does not exist, the following error message will be displayed.
    
    ::

        unloaddb: No such file or directory.

.. option:: -s, --schema-only

    This option specifies that only the schema file will be created from amongst all the output files which can be created by the unload operation.
    
    ::

        cubrid unloaddb -s demodb

.. option:: -d, --data-only

    This option specifies that only the data file will be created from amongst all of the output files which can be created by the unload operation.
    
    ::

        cubrid unloaddb -d demodb

.. option:: --output-prefix=PREFIX

    This option specifies the prefix for the names of schema and object files created by the unload operation. Once the example is executed, the schema file name becomes *abcd_schema* and the object file name becomes *abcd_objects*. If the **\-\-output-prefix** option is not specified, the name of the database to be unloaded is used as the prefix.
    
    ::

        cubrid unloaddb --output-prefix abcd demodb

.. option:: --hash-file=FILE
    
    This option specifies the name of the hash file.

.. option:: --latest-image
   
    This option makes it to unload the latest image of objects in data volumes when unloading instances (the MVCC version is ignored and it does not refer to the log volumes). It may include uncommitted data and deleted data that has not been vacuumed.

.. option:: -t, --thread-count=COUNT

    This option sets the number of threads to be used in the unload process. It is performed in parallel using as many threads as the specified COUNT value, which must be in the range of 0 to 127.
    If this setting is omitted, it is the same as specifying COUNT as 1. If specified as 0, unloaddb does not operate in thread mode.
    Even if thread mode is specified, if a table contains an Object type or a Set, MultiSet, or Sequence (List) type that contains an Object type, it will not operate in thread mode for that table.
    
    ::

        cubrid unloaddb -t 4 demodb

.. option:: --enhanced-estimates

    The -v option must be specified together.
    When determining the number of records in the target table, this option obtains the actual number of records instead of relying on statistical information. Using this option may increase the execution time of unloaddb because doing so adds the time needed to calculate the number of records. Therefore, this option should be used with caution.
    
    ::

        cubrid unloaddb --enhanced-estimates demodb


.. option:: -v, --verbose

    This option displays detailed information on the database tables and records being unloaded while the unload operation is under way.
    
    ::

        cubrid unloaddb -v demodb

.. option:: --use-delimiter

    This option writes the double quote(") on the beginning and end of an identifier. The default is not to write the double quote(").
    
.. option:: -S, --SA-mode

    The **-S** option performs the unload operation by accessing the database in standalone mode.
    
    ::
    
        cubrid unloaddb -S demodb

.. option:: -C, --CS-mode

    The **-C** option performs the unload operation by accessing the database in client/server mode.
    
    ::

        cubrid unloaddb -C demodb

.. option:: --datafile-per-class

    This option specifies that the output file generated through unload operation creates a data file per each table. The file name is generated as *<Database Name>_<Table Name>*\_ **objects** for each table. However, all column values in object types are unloaded as NULL and %id class_name class_id part is not written in the unloaded file (see :ref:`howtowrite-loadfile`).
    
    ::

        cubrid unloaddb --datafile-per-class demodb


.. option:: --split-schema-files

    Schema information is splited and created in object units, and a file containing a list of splited files is also created. For details, see to :ref:`loaddb` 's --schema-file-list=FILE.
    If this option is not specified, a single schema file containing all objects is created.

    ::

        cubrid unloaddb --split-schema-files demodb

    List of files created by spliting schema information into object units ::
	
        demodb_schema_user
        demodb_schema_class
        demodb_schema_vclass
        demodb_schema_synonym
        demodb_schema_serial
        demodb_schema_procedure
        demodb_schema_server
        demodb_schema_pk
        demodb_schema_fk
        demodb_schema_uk
        demodb_schema_grant
        demodb_schema_vclass_query_spec

    Filename containing a list of splited schema files
    
    ::

        demodb_schema_info

.. option:: --as-dba

    If the user is a member of the DBA group, create a schema file with the same contents as for the DBA user.
    
    ::

        cubrid unloaddb -u u1_dba_group --as-dba demodb

.. option:: --skip-index-detail

	This option doesn't output of the WITH clause options specified when creating the index. The default operation of unloaddb is to always print the WITH clause options regardless of the **print_index_detail** setting. If this option is specified, options of indexes in the WITH clause are omitted and printed. Affected output files are schema file (*database-name*\_schema) and index file (*database-name*\_indexes). If this option is omitted, even if *deduplicate level* is **0**, it will always be output explicitly.
    
    ::

        cubrid unloaddb --skip-index-detail demodb

    In the example below, you can see the difference whether this option is specified or not.
    
    Schema file
    
    +-----------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
    | -–skip-index-detail not specified | ALTER CLASS [dba].[ts] ADD CONSTRAINT [fk_ts_fid] FOREIGN KEY([fid]) WITH DEDUPLICATE=0 REFERENCES [dba].[tm] ON DELETE RESTRICT ON UPDATE RESTRICT ;  |
    +-----------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
    | -–skip-index-detail specified     | ALTER CLASS [dba].[ts] ADD CONSTRAINT [fk_ts_fid] FOREIGN KEY([fid]) REFERENCES [dba].[tm] ON DELETE RESTRICT ON UPDATE RESTRICT ;                     |
    +-----------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+
    
    Index file
    
    +-----------------------------------+-------------------------------------------------------------------+
    | -–skip-index-detail not specified | CREATE INDEX [idx_val] ON [dba].[ts] ([val]) WITH DEDUPLICATE=0;  |
    +-----------------------------------+-------------------------------------------------------------------+
    | -–skip-index-detail specified     | CREATE INDEX [idx_val] ON [dba].[ts] ([val]);                     |
    +-----------------------------------+-------------------------------------------------------------------+

.. note::

   To shorten the execution time of the unloaddb, it is possible to run multiple **'cubrid unloaddb'** processes concurrently for groups of different tables. (or you can use **unloaddb.sh**, refer :ref:`cubrid_scripts_unloaddb_sh`).

.. _loaddb:

loaddb
------

You can load a database by using the **cubrid loaddb** utility in the following situations:

*   Migrating previous version of CUBRID database to new version of CUBRID database
*   Migrating a third-party DBMS database to a CUBRID database
*   Inserting massive amount of data faster than using the **INSERT** statement

In general, the **cubrid loaddb** utility uses files (schema definition, object input, and index definition files) created by the **cubrid unloaddb** utility.
::

    cubrid loaddb [options] database_name

**Input Files**

*   Schema file(*database-name*\ **_schema**): A file generated by the unload operation; it contains schema information defined in the database.
*   Object file(*database-name*\ **_objects**): A file created by an unload operation. It contains information on the records in the database.
*   Index file(*database-name*\ **_indexes**): A file created by an unload operation. It contains information on the indexes defined in the database.
*   Trigger file(*database-name*\ **_trigger**): A file created by an unload operation. It contains information on the triggers defined in the database.
*   User-defined object file(*user_defined_object_file*): A file in table format written by the user to enter mass data. (For details, see :ref:`howtowrite-loadfile` .)
*   Schema list file (*database-name*\ **_schema_info**): A file created by unloading with the **\-\-split-schema-files** option. This is a file that has a list of files in which schema information defined in the database is splited into object units.(For details, see :option:`unloaddb --split-schema-files`.)

**Execution order of input files**

The **cubrid loaddb** utility follows a specific order of parsing and executing input files, ensuring both correctness and optimized performance. The order is as follows:

1. Load schema file (including primary keys)
2. Load object file
3. Load index file (secondary keys and foreign keys)
4. Load trigger file

**Loading modes**

The **cubrid loaddb** utility provides two modes of loading the data into a database.

*   **Stand-alone** mode loads the data offline, within a single thread environment. CUBRID 10.1 and older versions run only in this mode.
*   **Client-server** mode connects to a server and loads the data online and in parallel. **Client-server** mode can be much faster, but object references and class/shared attributes are not supported in this mode and they can only be loaded in **stand-alone** mode


The following table shows [options] available with the **cubrid loaddb** utility.

.. program:: loaddb

::

  -u, --user=ID                  user ID for database access
  -p, --password=PASS            user password
  -S, --SA-mode                  standalone mode execution
  -C, --CS-mode                  client-server mode execution
      --data-file-check-only     check syntax for data file without load
  -l, --load-only                load data file without prior syntax checking
      --estimated-size=NUMBER    estimated NUMBER of instances
  -v, --verbose                  enable verbose status messages
  -c, --periodic-commit=COUNT    insertion COUNT for periodic commit
      --no-oid                   don't use OID
      --no-statistics            do not update the statistics while loading
  -s, --schema-file=FILE[:LINE]  create schema from LINE with FILE
  -i, --index-file=FILE[:LINE]   create index from LINE with FILE
  -d, --data-file=FILE           load data with FILE
  -t, --table=TABLE              table name that is substituted for missing class header in data file
      --error-control-file=FILE  FILE to control error(s) during loading
      --ignore-class-file=FILE   input file of class names that skip load
      --trigger-file=FILE[:LINE] create trigger from LINE with FILE
      --no-user-specified-name   Find classes, serials, and triggers by their object names without their owner names.
      --schema-file-list=FILE    The content of this file is a list of schema file names to be used in loaddb.


.. option:: -u, --user=ID

    This option specifies the user account of a database where records are loaded. If the option is not specified, the default value is **PUBLIC**. ::

        cubrid loaddb -u admin -d demodb_objects newdb

.. option:: -p, --password=PASS

    This option specifies the password of a database user who will load records. If the option is not specified, you will be prompted to enter the password. ::

        cubrid loaddb -u dba -p pass -d demodb_objects newdb

.. option:: -S, --SA-MODE

    This option loads data into a database in **stand-alone** mode. This is the default loading mode of **loaddb** and it is fully compatible with CUBRID 10.1 and older database versions. ::

        cubrid loaddb -S -u dba -d demodb_objects newdb

.. option:: -C, --CS-MODE

    This option connects to a running server process. Unlike the **stand-alone** mode, the **client-server** mode loads the data using multiple threads, leading to greater performance for large amounts of data. Multiple **loaddb** sessions may run at the same time. Object references and class/shared attributes are not supported in this mode. ::

        cubrid loaddb -C -u dba -d demodb_objects newdb

.. note::

    When in **client-server** mode (-C, \-\-CS-MODE) on the **HA** (High Availability), only the -d option (data loading) is available.

.. option:: --data-file-check-only

    This option checks only the syntax of the data contained in demodb_objects, and does not load the data to the database. ::

        cubrid loaddb --data-file-check-only -d demodb_objects newdb

.. option:: -l, --load-only

    **Stand-alone** mode does, by default, one prior syntax check on the whole data file before actually loading the data into the database. If a syntax error occurs, no data is loaded and the error is reported.
    
    Using the **-l** option, the prior syntax check is skipped, thus increasing the loading speed. Syntax is still checked before loading each row into the database. Loading is stopped if a syntax error occurs, but data may be partially loaded.
    
    **Client-server mode** doesn't do the prior syntax check, so this option has no effect. ::

        cubrid loaddb -l -d demodb_objects newdb

.. option:: --estimated-size=NUMBER

    This option can be used to improve loading performance when the number of records to be unloaded exceeds the default value of 5,000. You can improve the load performance by assigning large hash memory for record storage with this option. ::

        cubrid loaddb --estimated-size 8000 -d demodb_objects newdb

.. option:: -v, --verbose

    This option shows how to display detailed information on the tables and records of the database being loaded while the database loading operation is performed. You can check the detailed information such as the progress, the class being loaded and the number of records to be entered. ::

        cubrid loaddb -v -d demodb_objects newdb

.. option:: -c, --periodic-commit=COUNT

    This option commits periodically every time COUNT records are entered into the database. If this option is not specified, all records included in demodb_objects are loaded to the database before the transaction is committed. If this option is used together with the **-s** or **-i** option, commit is performed periodically every time 100 DDL statements are loaded. 
    
    The recommended commit interval varies depending on the data to be loaded. It is recommended that the parameter of the **-c** option be configured to 50 for schema loading and 1 for index loading. For record loading, the recommended and default value is 10240.
    
    The previous versions of **CUBRID**, like 10.1 or older, did not have a default value for **\-\-periodic-commit** and would commit the transation only after loading the whole data. With the current approach, if you want to use the replicate the same behavior, you have to set the **\-\-periodic-commit** argument to a very high value, so that the commit is done after loading all the data.

::

        cubrid loaddb -c 100 -d demodb_objects newdb

.. option:: --no-oid

    The following is a command that loads records into newdb ignoring the OIDs in demodb_objects. ::
    
        cubrid loaddb --no-oid -d demodb_objects newdb

.. option:: --no-statistics

    The following is a command that does not update the statistics information of newdb after loading demodb_objects. It is useful especially when small data is loaded to a relatively big database; you can improve the load performance by using this command. ::

        cubrid loaddb --no-statistics -d demodb_objects newdb

.. option:: -s, --schema-file=FILE[:LINE]

    This option loads the schema information defined in the schema file, from the LINE-th. You can load the actual records after loading the schema information first by using the **-s** option.

    On the following example, demodb_schema is a file created by the unload operation and contains the schema information of the unloaded database.
    
   ::

        cubrid loaddb -u dba -s demodb_schema newdb

        Start schema loading.
        Total       86 statements executed.
        Schema loading from demodb_schema finished.
        Statistics for Catalog classes have been updated.

    The following loads the triggers defined in *demodb* into the newly created newdb database. demodb_trigger is a file created by the unload operation and contains the trigger information of the unloaded database. It is recommended to load the schema information after loading the records. ::

        cubrid loaddb -u dba -s demodb_trigger newdb

.. option:: -i, --index-file=FILE[:LINE]

    The following loads the index information defined in the index file, from the LINE-th. On the following example, demo_indexes is a file created by the unload operation and contains the index information of the unloaded database. You can create indexes with the **-i** option, after loading records with the **-d** option. ::

        cubrid loaddb -c 100 -d demodb_objects newdb
        cubrid loaddb -u dba -i demodb_indexes newdb

.. option:: -d, --data-file=FILE

    This option loads the record information into newdb by specifying the data file or the user-defined object file. demodb_objects is either an object file created by the unload operation or a user-defined object file written by the user for mass data loading. ::

        cubrid loaddb -u dba -d demodb_objects newdb

.. option:: -t, --table=TABLE

    This option specifies the table name if a table name header is omitted in the data file to be loaded. The table name must be prefixed with the schema name. ::

        cubrid loaddb -u dba -d demodb_objects -t tbl_name newdb

.. option:: --error-control-file=FILE

    This option specifies the file that describes how to handle specific errors occurring during database loading. ::

        cubrid loaddb --error-control-file=error_test -d demodb_objects newdb

    For the server error code name, see the **$CUBRID/include/dbi.h** file.

    For error messages by error code (error number), see the number under $set 5 MSGCAT_SET_ERROR in the **$CUBRID/msg/**\ *<character set name>*\ **/cubrid.msg** file. ::

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

    If the file that details errors is not specified by using the **\-\-error-control-file** option, the **loaddb** utility is configured to ignore errors from 24 to 33 by default. As a warning error, it indicates that there is no enough space in the database volume. If there is no space in the assigned database volume, a generic volume is automatically created.

    The following example shows a file that details errors.

    *  The warning errors from 24 to 33 indicating DB volume space is insufficient are not ignored by configuring +DEFAULT.

    *  The error code 2 is not ignored because +2 has been specified later, even when -2 has been specified first.

    *  -670 has been specified to ignore the error code 670, which is a unique violation error.

    *  #-115 has been processed as a comment since # is added. ::

        vi error_file
         
        +DEFAULT
        -2
        -670
        #-115 --> comment
        +2

.. option:: --ignore-class-file=FILE

    You can specify a file that lists classes to be ignored during loading records. All records of classes except ones specified in the file will be loaded. ::

        cubrid loaddb --ignore-class-file=skip_class_list -d demodb_objects newdb

.. option:: --trigger-file=FILE[:LINE]

    This option loads the trigger information defined in the trigger file, from the LINE-th. ::

        cubrid loaddb --trigger-file=demodb_triggers -d demodb_objects newdb

.. option:: --no-user-specified-name

    Files unloaded in a version older than 11.2 can be loaded in version 11.2 only if the **\-\-no-user-specified-name** option is used.

    This option works if **DBA** is specified as the user account of the database with the -u option.
    
    ::

        cubrid loaddb -u dba -s demodb_schema -d demodb_objects --no-user-specified-name demodb

.. warning::

    The **\-\-no-user-specified-name** option must not be used to load a file unloaded in version 11.2 or after version 11.2.

.. option:: --schema-file-list=FILE

    This option loads all schema files included in the list at once. There must be a list of schema files, and they are loaded sequentially from the top schema file to the bottom of this list file. This option and the -s option cannot be used together.

    This list file is automatically created when using the **\-\-split-schema-files** option in unloaddb.
	
    ::

        cubrid loaddb -u dba --schema-file-list=demodb_schema_info

.. warning::

    You can change the order of schema files written in this file, but loading errors may occur if the order is wrong. For example, an error may occur if the PK file is loaded first while the file with CLASS information is not loaded. Therefore, in the list, the file name for loading CLASS information must be higher than the file for loading PK.

.. warning::

    The **\-\-no-logging** option enables to load data file quickly when **loaddb** is executed by not storing transaction logs; however, it has risk, which data cannot be recovered in case of errors occurred such as incorrect file format or system failure. In this case, you must rebuild database to solve the problem. Thus, in general, it is not recommended to use this option exception of building a new database which does not require data recovery. If you use this option, **loaddb** does not check the errors like unique violation. To use this option, you should consider these issues.

.. warning::

    When running **loaddb**, foreign key constraints are not checked to ensure performance for loading large amounts of data and to minimize dependencies between loaded data. So, the data inconsistencies with the master node may occur because foreign key constraints are checked during the replication process of the slave or replica node in the HA environment if the **-C** option is used.

.. _howtowrite-loadfile:

How to Write Files to Load Database
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can add mass data to the database more rapidly by writing the object input file used in the **cubrid loaddb** utility. An object input file is a text file in simple table form that consists of comments and command/data lines.

Comment
+++++++

In CUBRID, a comment is represented by two hyphens (\-\-). ::

    -- This is a comment!

.. _howtowrite-loadfile-command-line:

Command Line
++++++++++++

A command line begins with a percent character (%) and consists of **%class** and **%id** commands; the former defines classes, and the latter defines aliases and identifiers used for class identification.

.. _assign-id-to-class:

*   Assigning an Identifier to a Class

    You can assign an identifier to class reference relations by using the **%id** command.
    
    ::

        %id [schema_name.]class_name class_id
        schema_name:
            identifier
        class_name:
            identifier
        class_id:
            integer

    The *class_name* specified by the **%id** command is the class name defined in the database, and *class_id* is the numeric identifier which is assigned for object reference. The *class_name* uses the *schema_name* as a prefix and the *schema_name* is the name of the schema in which the class is defined.
    The *schema_name* can be omitted, and if the *schema_name* is omitted, the user name of the database specified with the -u option is used as the schema name.

    ::

        %id [employee] 21
        %id [office] 22
        %id [project] 23
        %id [phone] 24

    The *schema_name* and *class_name* must be enclosed in square brackets ([ ]). Each of *schema_name* and *class_name* or together can be enclosed in square brackets.

    ::

        %id [public].[employee] 21
        %id [public].[office] 22
        %id [public].[project] 23
        %id [public].[phone] 24

    ::

        %id [public.employee] 21
        %id [public.office] 22
        %id [public.project] 23
        %id [public.phone] 24

*   Specifying the Class and Attribute

    You can specify the classes (tables) and attributes (columns) upon loading data by using the **%class** command. The data line should be written based on the order of attributes specified. When a class name is provided by using the **-t** option while executing the **cubrid loaddb** utility, you don't have to specify the class and attribute in the data file. However, the order of writing data must comply with the order of the attribute defined when creating a class.
    
    ::

        %class [schema_name.]class_name (attr_name [attr_name...])
        schema_name:
            identifier
        class_name:
            identifier
        attr_name:
            identifier

    The class must be pre-defined in the database to be loaded.

    The *class_name* specified by the **%class** command is the class name defined in the database and the *schema_name* is the name of the schema in which the class is defined. The *attr_name* is the name of the attribute defined. The method of writing *schema_name* and *class_name* is the same as above.

    The following example shows how to specify a class and three attributes by using the **%class** command to enter data into a class named *employee*. Three pieces of data should be entered on the data lines after the **%class** command. For this, see :ref:`Configuring Reference Relation <conf-reference-relation>`.
    
    ::

        %class [public].[employee] (name age department)

Data Line
+++++++++

A data line comes after the **%class** command line. Data loaded must have the same type as the class attributes specified by the **%class** command. The data loading operation stops if these two types are different.

Data for each attribute must be separated by at least one space and be basically written as a single line. However, if the data to be loaded takes more than one line, you should specify the plus sign (+) at the end of the first data line to enter data continuously on the following line. Note that no space is allowed between the last character of the data and the plus sign.

*   Loading an Instance

    As shown below, you can load an instance that has the same type as the specified class attribute. Each piece of data is separated by at least one space.

    ::

        %class [public].[employee] (name)
        'jordan' 
        'james'  
        'garnett'
        'malone'

*   Assigning an Instance Number

    You can assign a number to a given instance at the beginning of the data line. An instance number is a unique positive number in the specified class. Spaces are not allowed between the number and the colon (:). Assigning an instance number is used to configure the reference relation for later.

    ::

        %class [public].[employee] (name)
        1: 'jordan' 
        2: 'james'  
        3: 'garnett' 
        4: 'malone' 

.. _conf-reference-relation:
    
*   Configuring Reference Relation

    You can configure the object reference relation by specifying the reference class after an "at sign (**@**)" and the instance number after the "vertical line (|)."
    
    ::

        @class_ref | instance_no
        class_ref:
            [schema_name.]class_name
            class_id

    Specify a class name or a class id after the **@** sign, and an instance number after a vertical line (|). Spaces are not allowed before and after a vertical line (|). The method of wirting *class_ref* is the same as the method of wirting *schema_name* and *class_name* in the :ref:`Command Line <howtowrite-loadfile-command-line>`. However, enclosing each of *schema_name* and *class_name* in square brackets ([ ]) is not allowed.

    The following example shows how to load class instances into the *paycheck* class. The *name* attribute references an instance of the *employee* class. As in the last line, data is loaded as **NULL** if you configure the reference relation by using an instance number not specified earlier.
    
    ::

        %class [public].[paycheck] (name department salary)
        @[public.employee]|1   'planning'   8000000   
        @[public.employee]|2   'planning'   6000000  
        @[public.employee]|3   'sales'   5000000  
        @[public.employee]|4   'development'   4000000
        @[public.employee]|5   'development'   5000000

    Since the id 21 was assigned to the *employee* class by using the **%id** command in the :ref:`Assigning an Identifier to a Class <assign-id-to-class>` section, the above example can be written as follows:
    
    ::

        %class [public].[paycheck] (name department salary)
        @21|1   'planning'   8000000   
        @21|2   'planning'   6000000  
        @21|3   'sales'   5000000  
        @21|4   'development'   4000000
        @21|5   'development'   5000000

Migrating Database
^^^^^^^^^^^^^^^^^^

To use a new version of CUBRID database, you may need to migrate an existing data to a new one. For this purpose, you can use the "Export to an ASCII text file" and "Import from an ASCII text file" features provided by CUBRID. 

The following section explains migration steps using the **cubrid unloaddb** and **cubrid loaddb** utilities.

**Recommended Scenario and Procedures**

The following steps describes migration scenario that can be applied while the existing version of CUBRID is running. For database migration, you should use the **cubrid unloaddb** and **cubrid loaddb** utilities. For details, see :ref:`unloaddb` and :ref:`loaddb`.

#. Stop the existing CUBRID service

    Execute **cubrid service stop** to stop all service processes running on the existing CUBRID and then check whether all CUBRID-related processes have been successfully stopped. 

    To verify whether all CUBRID-related processes have been successfully stopped, execute **ps -ef | grep cub\_** in Linux. If there is no process starting with cub\_, all CUBRID-related processes have been successfully stopped. In Windows, press the <Ctrl+Alt+Delete> key and select [Start Task Manager]. If there is no process starting with cub\_ in the [Processes] tab, all CUBRID-related processes have been successfully stopped. In Linux, when the related processes remain even after the CUBRID service has been terminated, use **kill** command to forcibly terminate them, and use **ipcs -m** command to check and release the memory shard by CUBRID broker. To forcibly terminate related processes in Windows, go to the [Processes] tab of Task Manager, right-click the image name, and then select [End Process].

#. Back up the existing database

    Perform backup of the existing version of the database by using the **cubrid backupdb** utility. The purpose of this step is to safeguard against failures that might occur during the database unload/load operations. For details on the database backup, see :ref:`backupdb`.

#. Unload the existing database

    Unload the database created for the existing version of CUBRID by using the **cubrid unloaddb** utility. For details on unloading a database, see :ref:`unloaddb` .

#. Store the existing CUBRID configuration files

    Store the configurations files such as **cubrid.conf**, **cubrid_broker.conf** and **cm.conf ** in the **CUBRID/conf** directory. The purpose of this step is to conveniently apply parameter values for the existing CUBRID database environment to the new one.

#. Install a new version of CUBRID

    Once backing up and unloading of the data created by the existing version of CUBRID have been completed, delete the existing version of CUBRID and its databases and then install the new version of CUBRID. For details on installing CUBRID, see :doc:`/start`.

#. Configure the new CUBRID environment

    Configure the new version of CUBRID by referring to configuration files of the existing database stored in the step 3, " **Store the existing CUBRID configuration files** ." For details on configuring new environment, see :doc:`/install` in "Getting Started."

#. Load the new database

    Create a database by using the **cubrid createdb** utility and then load the data which had previously been unloaded into the new database by using the **cubrid loaddb** utility. Please see :ref:`creating-database` for creating a database and :ref:`loaddb` for loading a database.

#. Back up the new database

    Once the data has been successfully loaded into the new database, back up the database created for the new version of CUBRID by using the **cubrid backupdb** utility. The reason for this step is because you cannot restore the data backed up in the existing version of CUBRID when using the new version. For details on backing up the database, see :ref:`backupdb` .

.. warning:: 

    Even if the version is identical, the 32-bit database volume and the 64-bit database volume are not compatible for backup and recovery. Therefore, it is not recommended to recover a 32-bit database backup on the 64-bit CUBRID or vice versa.

.. warning::

    When using the TDE feature in CUBRID 11, it does not provide backward compatibility. So, the unloaded file in CUBRID 11 cannot be loaded into a lower version if more the one TDE table exists.
