.. _cubrid-utilities:

cubrid Utilities
================

The following shows how to use the cubrid management utilities. ::

    cubrid utility_name
    utility_name:
        createdb [option] <database_name> <locale_name>  --- Creating a database
        deletedb [option] <database_name>   --- Deleting a database
        installdb [option] <database-name>   --- Installing a database 
        renamedb [option] <source-database-name> <target-database-name>  --- Renaming a database 
        copydb [option] <source-database-name> <target-database-name>  --- Copying a database 
        backupdb [option] <database-name>  --- Backing up a database 
        restoredb [option] <database-name>  --- Restoring a database 
        addvoldb [option] <database-name>  --- Adding a database volume file 
        spacedb [option] <database-name>  --- Displaying details of database space 
        lockdb [option] <database-name>  --- Displaying details of database lock 
        tranlist [option] <database-name>  --- Checking transactions
        killtran [option] <database-name>  --- Removing transactions 
        optimizedb [option] <database-name>  --- Updating database statistics 
        statdump [option] <database-name>  --- Dumping statistic information of database server execution 
        compactdb [option] <database-name>  --- Optimizing space by freeing unused space 
        diagdb [option] <database-name>  --- Displaying internal information 
        checkdb [option] <database-name>  --- Checking database consistency 
        alterdbhost [option] <database-name>  --- Altering database host 
        plandump [option] <database-name>  --- Displaying details of the query plan 
        loaddb [option] <database-name>  --- Loading data and schema 
        unloaddb [option] <database-name>  --- Unloading data and schema 
        paramdump [option] <database-name>  --- Checking out the parameter values configured in a database 
        changemode [option] <database-name>  --- Displaying or changing the server HA mode 
        applyinfo [option] <database-name>   --- Displaying the status of being applied transaction log to the other node in HA replication environment
        synccolldb [option] <database-name>  --- Synchronizing the DB collation with the system collation
        genlocale [option] <database-name>  --- Compiling the locale information to use
        dumplocale [option] <database-name>   --- Printing human readable text for the compiled binary locale information
    gen_tz [option] [<database-name>]  --- Generates C source file containing timezone data ready to be compiled into a shared library
    dump_tz [option]  --- Displaying timezone related information

cubrid Utility Logging
----------------------
 
CUBRID supports logging feature for the execution result of **cubrid** utilities; for details, see :ref:`cubrid-utility-logging`.

.. _creating-database:

.. _createdb:

createdb
--------

The **cubrid createdb** utility creates databases and initializes them with the built-in CUBRID system tables. It can also define initial users to be authorized in the database and specify the locations of the logs and databases. In general, the **cubrid createdb** utility is used only by DBA. 

.. warning::

    When you create database, a locale name and a charset name after a DB name must be specified(e.g. ko_KR.utf8). It affects the length of string type, string comparison operation, etc. The specified charset when creating database cannot be changed later, so you should be careful when specifying it.
    
    For charset, locale and collation setting, see :doc:`/sql/i18n`.

::

    cubrid createdb [options] database_name locale_name.charset

*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **createdb**: A command used to create a new database.

*   *database_name*: Specifies a unique name for the database to be created, without including the path name to the directory where the database will be created. If the specified database name is the same as that of an existing database name, CUBRID halts creation of the database to protect existing files.

*   *locale_name*: A locale name to use in the database should be input. For a locale name which can be used in CUBRID, refer to :ref:`locale-selection`.

*   *charset*: A characterset to use in the database should be input. A character set which can be used in CUBRID is iso88591, euckr or utf8.
    
    *   If *locale_name* is en_US and *charset* is omitted, a character set will be iso88591.
    *   If *locale_name* is ko_KR and *charset* is omitted, a character set will be utf8.
    *   All locale names except en_US and ko_KR cannot omit *charset*, and a *charset* can be specified only with utf8.

The maximum length of database name is 17 in English.

The following shows [options] available with the **cubrid** **createdb** utility.

.. program:: createdb

.. option:: --db-volume-size=SIZE

    This option specifies the size of the database volume that will be created first. The default value is  the value of the system parameter **db_volume_size**, and the minimum value is 20M. You can set units as K, M, G and T, which stand for kilobytes (KB), megabytes (MB), gigabytes (GB), and terabytes (TB) respectively. If you omit the unit, bytes will be applied.

    The following example shows how to create a database named *testdb* and assign 512 MB to its first volume. ::

        cubrid createdb --db-volume-size=512M testdb en_US

.. option:: --db-page-size=SIZE

    This option specifies the size of the database page; the minimum value is 4K and the maximum value is **16K** (default). K stands for kilobytes (KB). The value of page size is one of the following: 4K, 8K, or 16K. If a value between 4K and 16K is specified, system rounds up the number. If a value greater than 16K or less than 4K, the specified number is used.

    The following example shows how to create a database named *testdb* and configure its page size 16K. ::

        cubrid createdb --db-page-size=16K testdb en_US

.. option:: --log-volume-size=SIZE

    This option  specifies the size of the database log volume. The default value is the same as database volume size, and the minimum value is 20M. You can set units as K, M, G and T, which stand for kilobytes (KB), megabytes (MB), gigabytes (GB), and terabytes (TB) respectively. If you omit the unit, bytes will be applied. 

    The following example shows how to create a database named *testdb* and assign 256 MB to its log volume. ::

        cubrid createdb --log-volume-size=256M testdb en_US

.. option:: --log-page-size=SIZE

    This option specifies the size of the log volume page. The default value is the same as data page size. The minimum value is 4K and the maximum value is 16K. K stands for kilobytes (KB).
    The value of page size is one of the following: 4K, 8K, or 16K. If a value between 4K and 16K is specified, system rounds up the number. If a value greater than 16K or less than 4K, the specified number is used.

    The following example shows how to create  a database named *testdb* and configure its log volume page size 8K. ::

        cubrid createdb --log-page-size=8K testdb en_US

.. option:: --comment=COMMENT

    This option specifies a comment to be included in the database volume header. If the character string contains spaces, the comment must be enclosed in double quotes.

    The following example shows how to create a database named *testdb* and add a comment to the database volume. ::

        cubrid createdb --comment "a new database for study" testdb en_US

.. option:: -F, --file_path=PATH

    The **-F** option specifies an absolute path to a directory where the new database will be created. If the **-F** option is not specified, the new database is created in the current working directory.

    The following example shows how to create a database named *testdb* in the directory /dbtemp/new_db. ::

        cubrid createdb -F "/dbtemp/new_db/" testdb en_US

.. option:: -L, --log_path=PATH

    The **-L** option specifies an absolute path to the directory where database log files are created. If the **-L** option is not specified, log files are created in the directory specified by the **-F** option. 
    If neither **-F** nor **-L** option is specified, database log files are created in the current working directory.

    The following example shows how to create a database named *testdb* in the directory /dbtemp/newdb and log files in the directory /dbtemp/db_log. ::

        cubrid createdb -F "/dbtemp/new_db/" -L "/dbtemp/db_log/" testdb en_US

.. option:: -B, --lob-base-path=PATH

    This option specifies a directory where **LOB** data files are stored when **BLOB/CLOB** data is used. If the **--lob-base-path** option is not specified, LOB data files are store in <*location of database volumes created*>/**lob** directory. 

    The following example shows how to create a database named *testdb* in the working directory and specify /home/data1 of local file system as a location of LOB data files. ::

        cubrid createdb --lob-base-path "file:/home1/data1" testdb en_US

.. option:: --server-name=HOST

    This option enables the server of a specific database to run in the specified host when CUBRID client/server is used. The information of a host specified is stored in the **databases.txt** file. If this option is not specified, the current localhost is specified by default. 
    
    The following example shows how to create a database named *testdb* and register it on the host *aa_host*. ::

        cubrid createdb --server-name aa_host testdb en_US

.. option:: -r, --replace

    This option creates a new database and overwrites an existing database if one with the same name exists. 
    
    The following example shows how to create a new database named *testdb* and overwrite the existing database with the same name. ::

        cubrid createdb -r testdb en_US

.. option:: --more-volume-file=FILE

    This option creates an additional volume based on the specification contained in the file specified by the option. The volume is created in the same directory where the database is created. Instead of using this option, you can add a volume by using the **cubrid addvoldb** utility. 

    The following example shows how to create a database named *testdb* as well as an additional volume based on the specification stored in the **vol_info.txt** file. ::

        cubrid createdb --more-volume-file vol_info.txt testdb en_US

    The following is a specification of the additional volume contained in the **vol_info.txt** file. The specification of each volume must be written on a single line. ::

        #xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        # NAME volname COMMENTS volcmnts PURPOSE volpurp NPAGES volnpgs
        NAME data_v1 COMMENTS "data information volume" PURPOSE data NPAGES 1000
        NAME data_v2 COMMENTS "data information volume" PURPOSE data NPAGES 1000
        NAME data_v3 PURPOSE data NPAGES 1000
        NAME index_v1 COMMENTS "index information volume" PURPOSE index NPAGES 500
        NAME temp_v1 COMMENTS "temporary information volume" PURPOSE temp NPAGES 500
        NAME generic_v1 COMMENTS "generic information volume" PURPOSE generic NPAGES 500
        #xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

    As shown in the example, the specification of each volume consists following. ::

        NAME volname COMMENTS volcmnts PURPOSE volpurp NPAGES volnpgs

    *   *volname*: The name of the volume to be created. It must follow the UNIX file name conventions and be a simple name not including the directory path. The specification of a volume name can be omitted. If it is, the "database name to be created by the system_volume identifier" becomes the volume name.

    *   *volcmnts*: Comment to be written in the volume header. It contains information on the additional volume to be created. The specification of the comment on a volume can also be omitted.

    *   *volpurp*: It must be one of the following types: **data**, **index**, **temp**, or **generic** based on the purpose of storing volumes. The specification of the purpose of a volume can be omitted in which case the default value is **generic**.

    *   *volnpgs*: The number of pages of the additional volume to be created. The specification of the number of pages of the volume cannot be omitted; it must be specified.

.. option:: --user-definition-file=FILE

    This option adds users who have access to the database to be created. It adds a user based on the specification contained in the user information file specified by the parameter. 
    Instead of using the **--user-definition-file** option, you can add a user by using the **CREATE USER** statement (for details, see :ref:`create-user`).

    The following example shows how to create a database named *testdb* and add users to *testdb* based on the user information defined in the **user_info.txt** file. ::

        cubrid createdb --user-definition-file=user_info.txt testdb en_US

    The syntax of a user information file is as follows: ::

        USER user_name [ <groups_clause> | <members_clause> ]
        
        <groups_clause>: 
            [ GROUPS <group_name> [ { <group_name> }... ] ]

        <members_clause>: 
            [ MEMBERS <member_name> [ { <member_name> }... ] ]

    *   The *user_name* is the name of the user who has access to the database. It must not include spaces.

    *   The **GROUPS** clause is optional. The *group_name* is the upper level group that contains the *user_name* . Here, the *group_name* can be multiply specified and must be defined as **USER** in advance.

    *   The **MEMBERS** clause is optional. The *member_name* is the name of the lower level member that belongs to the *user_name* . Here, the *member_name* can be multiply specified and must be defined as **USER** in advance.

    Comments can be used in a user information file. A comment line must begin with a consecutive hyphen lines (--). Blank lines are ignored.

    The following example shows a user information in which *grandeur* and *sonata* are included in *sedan* group, *tuscan* is included in *suv* group, and *i30* is included in *hatchback* group. The name of the user information file is **user_info.txt**. ::

        --
        -- Example 1 of a user information file
        --
        USER sedan
        USER suv
        USER hatchback
        USER grandeur GROUPS sedan
        USER sonata GROUPS sedan
        USER tuscan GROUPS suv
        USER i30 GROUPS hatchback

    The following example shows a file that has the same user relationship information as the file above. The difference is that the **MEMBERS** statement is used in the file below. ::

        --
        -- Example 2 of a user information file
        --
        USER grandeur
        USER sonata
        USER tuscan
        USER i30
        USER sedan MEMBERS sonata grandeur
        USER suv MEMBERS tuscan
        USER hatchback MEMBERS i30
    
.. option:: --csql-initialization-file=FILE

    This option executes an SQL statement on the database to be created by using the CSQL Interpreter. A schema can be created based on the SQL statement contained in the file specified by the parameter.

    The following example shows how to create a database named *testdb* and execute the SQL statement defined in table_schema.sql through the CSQL Interpreter. ::

        cubrid createdb --csql-initialization-file table_schema.sql testdb en_US

.. option:: -o, --output-file=FILE

    This option stores messages related to the database creation to the file given as a parameter. The file is created in the same directory where the database was created. 
    If the **-o** option is not specified, messages are displayed on the console screen. The **-o** option allows you to use information on the creation of a certain database by storing messages, generated during the database creation, to a specified file.

    The following example shows how to create a database named *testdb* and store the output of the utility to the **db_output** file instead of displaying it on the console screen. ::

        cubrid createdb -o db_output testdb en_US

.. option:: -v, --verbose

    This option displays all information on the database creation operation onto the screen. Like the **-o** option, this option is useful in checking information related to the creation of a specific database. Therefore, if you specify the **-v** option together with the **-o** option, you can store the output messages in the file given as a parameter; the messages contain the operation information about the **cubrid createdb** utility and database creation process.

    The following example shows how to create a database named *testdb* and display detailed information on the operation onto the screen. ::

        cubrid createdb -v testdb en_US

.. note::

    *   **temp_file_max_size_in_pages** is a parameter used to configure the maximum number of pages assigned to store the temporary temp volume - used for complicated queries or storing arrays - on the disk. While the default value is **-1**, the temporary temp volume may be increased up to the amount of extra space on the disk specified by the **temp_volume_path** parameter. If the value is 0, the temporary temp volume cannot be created. In this case, the permanent temp volume should be added by using the :ref:`cubrid addvoldb <adding-database-volume>` utility. For the efficient management of the volume, it is recommended to add a volume for each usage. 
        
    *   By using the :ref:`cubrid spacedb <spacedb>` utility, you can check the reaming space of each volume. By using the :ref:`cubrid addvoldb <adding-database-volume>` utility, you can add more volumes as needed while managing the database. When adding a volume while managing the database, you are advised to do so when there is less system load. Once the assigned volume for a usage is completely in use, a **generic** volume will be created, so it is suggested to add extra volume for a usage that is expected to require more space.

The following example shows how to create a database, classify volume usage, and add volumes such as **data**, **index**, and **temp**. ::

    cubrid createdb --db-volume-size=512M --log-volume-size=256M cubriddb en_US
    cubrid addvoldb -S -p data -n cubriddb_DATA01 --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p data -n cubriddb_DATA02 --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p index -n cubriddb_INDEX01 cubriddb --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p temp -n cubriddb_TEMP01 cubriddb --db-volume-size=512M cubriddb

.. _adding-database-volume:

.. _addvoldb:

addvoldb
--------

When the total free space size of the **generic** volumes has become smaller than the size which is specified at the system parameter **generic_vol_prealloc_size** (default: 50M) in :ref:`disk-parameters`, **generic** volume is added automatically. Automatically adding a volume is done when a new page is required; The volume is not expanded when only a SELECT queries are executed.

CUBRID volumes are separated by the purpose of the usage such as data storage, index storage, temporary result storage; **generic** volume can be used for data and index storage.

For the each type(purpose) of volumes, see :ref:`database-volume-structure`.

In comparison, the command for adding a database volume manually is as follows.

::

    cubrid addvoldb [options] database_name

*   **cubrid**: An integrated utility for CUBRID service and database management.
    
*   **addvoldb**: A command that adds a specified number of pages of the new volume to a specified database.
    
*   *database_name*: Specifies the name of the database to which a volume is to be added without including the path name to the directory where the database is to be created.

The following example shows how to create a database, classify volume usage, and add volumes such as **data**, **index**, and **temp**. ::

    cubrid createdb --db-volume-size=512M --log-volume-size=256M cubriddb en_US
    cubrid addvoldb -S -p data -n cubriddb_DATA01 --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p data -n cubriddb_DATA02 --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p index -n cubriddb_INDEX01 cubriddb --db-volume-size=512M cubriddb
    cubrid addvoldb -S -p temp -n cubriddb_TEMP01 cubriddb --db-volume-size=512M cubriddb

The following shows [options] available with the **cubrid addvoldb** utility.

.. program:: addvoldb

.. option:: --db-volume-size=SIZE

    **--db-volume-size** is an option that specifies the size of the volume to be added to a specified database. If the **--db-volume-size** option is omitted, the value of the system parameter **db_volume_size** is used by default. You can set units as K, M, G and T, which stand for kilobytes (KB), megabytes (MB), gigabytes (GB), and terabytes (TB) respectively. If you omit the unit, bytes will be applied.

    The following example shows how to add a volume for which 256 MB are assigned to the *testdb* database. ::

        cubrid addvoldb -p data --db-volume-size=256M testdb

.. option:: -n, --volume-name=NAME

    This option specifies the name of the volume to be added to a specified database. The volume name must follow the file name protocol of the operating system and be a simple one without including the directory path or spaces. 
    If the **-n** option is omitted, the name of the volume to be added is configured by the system automatically as "database name_volume identifier". For example, if the database name is *testdb*, the volume name *testdb_x001* is automatically configured.
    
    The following example shows how to add a volume for which 256 MB are assigned to the *testdb* database in standalone mode. The volume name *testdb_v1* will be created. ::

        cubrid addvoldb -S -n testdb_v1 --db-volume-size=256M testdb

.. option::  -F, --file-path=PATH

    This option specifies the directory path where the volume to be added will be stored. If the **-F** option is omitted, the value of the system parameter **volume_extension_path** is used by default.

    The following example shows how to add a volume for which 256 MB are assigned to the *testdb* database in standalone mode. The added volume is created in the /dbtemp/addvol directory. Because the **-n** option is not specified for the volume name, the volume name *testdb_x001* will be created. ::

        cubrid addvoldb -S -F /dbtemp/addvol/ --db-volume-size=256M testdb

.. option:: --comment COMMENT

    This option facilitates to retrieve information on the added volume by adding such information in the form of comments. It is recommended that the contents of a comment include the name of **DBA** who adds the volume, or the purpose of adding the volume. The comment must be enclosed in double quotes.  
    
    The following example shows how to add a volume for which 256 MB are assigned to the *testdb* database in standalone mode and inserts a comment about the volume. ::

        cubrid addvoldb -S --comment "data volume added_cheolsoo kim" --db-volume-size=256M testdb

.. option:: -p, --purpose=PURPOSE

    This option specifies the purpose of the volume to be added. The reason for specifying the purpose of the volume is to improve the I/O performance by storing volumes separately on different disk drives according to their purpose. 
    Parameter values that can be used for the **-p** option are **data**, **index**, **temp** and **generic**. The default value is **generic**. For the purpose of each volume, see :ref:`database-volume-structure`.

    The following example shows how to add a volume for which 256 MB are assigned to the *testdb* database in standalone mode. ::

        cubrid addvoldb -S -p index --db-volume-size=256M testdb

.. option:: -S, --SA-mode

    This option accesses the database in standalone mode without running the server process. This option has no parameter. If the **-S** option is not specified, the system assumes to be in client/server mode. ::

        cubrid addvoldb -S --db-volume-size=256M testdb

.. option:: -C, --CS-mode

    This option accesses the database in client/server mode by running the server and the client separately. There is no parameter. Even when the **-C** option is not specified, the system assumes to be in client/server mode by default. ::

        cubrid addvoldb -C --db-volume-size=256M testdb

.. option:: --max_writesize-in-sec=SIZE

    The --max_writesize-in-sec is used to limit the impact of  system operating when you add a volume to the database. This can limit the maximum writing size per second. The unit of this option is K(kilobytes) and M(megabytes). The minimum value is 160K. If you set this value as less than 160K, it is changed as 160K. It can be used only in client/server mode.
    
    The below is an example to limit the writing size of the 2GB volume as 1MB. Consuming time will be about 35 minutes(= (2048MB/1MB) /60 sec.). ::
    
        cubrid addvoldb -C --db-volume-size=2G --max-writesize-in-sec=1M testdb

.. _deleting-database:

.. _deletedb:

deletedb
--------

The **cubrid deletedb** utility is used to delete a database. You must use the **cubrid deletedb** utility to delete a database, instead of using the file deletion commands of the operating system; a database consists of a few interdependent files. 

The **cubrid deletedb** utility also deletes the information on the database from the database location file (**databases.txt**). The **cubrid deletedb** utility must be run offline, that is, in standalone mode when nobody is using the database. ::

    cubrid deletedb  [options] database_name

*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **deletedb**: A command to delete a database, its related data, logs and all backup files. It can be executed successfully only when the database is in a stopped state.

*   *database_name*: Specifies the name of the database to be deleted without including the path name.

The following shows [options] available with the **cubrid deletedb** utility.

.. program:: deletedb

.. option:: -o, --output-file=FILE

    This option specifies the file name for writing messages::

        cubrid deletedb -o deleted_db.out testdb

    The **cubrid** **deletedb** utility also deletes the database information contained in the database location file (**databases.txt**). The following message is returned if you enter a utility that tries to delete a non-existing database. ::

        cubrid deletedb testdb
        Database "testdb" is unknown, or the file "databases.txt" cannot be accessed.

.. option:: -d, --delete-backup

    This option deletes database volumes, backup volumes and backup information files simultaneously. If the -**d** option is not specified, backup volume and backup information files are not deleted. ::
    
        cubrid deletedb -d testdb

.. _renamedb:

renamedb
--------

The **cubrid renamedb** utility renames a database. The names of information volumes, log volumes and control files are also renamed to conform to the new database one.

In contrast, the **cubrid alterdbhost** utility configures or changes the host name of the specified database. In other words, it changes the host name configuration in the **databases.txt** file. ::

    cubrid renamedb [options] src_database_name dest_database_name

*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **renamedb**: A command that changes the existing name of a database to a new one. It executes successfully only when the database is in a stopped state. The names of related information volumes, log volumes and control files are also changed to new ones accordingly.

*   *src_database_name*: The name of the existing database to be renamed. The path name to the directory where the database is to be created must not be included.

*   *dest_database_name*: The new name of the database. It must not be the same as that of an existing database. The path name to the directory where the database is to be created must not be included.

The following shows [options] available with the **cubrid deletedb** utility.

.. program:: renamedb

.. option:: -E, --extended-volume-path=PATH

    This option renames an extended volume created in a specific directory path (e.g. /dbtemp/addvol/), and then moves the volume to a new directory. This specifies a new directory path (e.g. /dbtemp/newaddvols/) where the renamed extended volume will be moved. 

    If it is not specified, the extended volume is only renamed in the existing path without being moved. If a directory path outside the disk partition of the existing database volume or an invalid one is specified, the rename operation is not executed. This option cannot be used together with the **-i** option. ::

        cubrid renamedb -E /dbtemp/newaddvols/ testdb testdb_1

.. option:: -i, --control-file=FILE

    The option specifies an input file in which directory information is stored to change all database name of volumes or files and assign different directory at once. To perform this work, the **-i** option is used. 
    The **-i** option cannot be used together with the **-E** option. ::

        cubrid renamedb -i rename_path testdb testdb_1

    The following are the syntax and example of a file that contains the name of each volume, the current directory path and the directory path where renamed volumes will be stored. ::

        volid source_fullvolname dest_fullvolname

    *   *volid*: An integer that is used to identify each volume. It can be checked in the database volume control file (database_name_vinf).

    *   *source_fullvolname*: The current directory path to each volume.

    *   *dest_fullvolname*: The target directory path where renamed volumes will be moved. If the target directory path is invalid, the database rename operation is not executed.

    ::

        -5  /home1/user/testdb_vinf       /home1/CUBRID/databases/testdb_1_vinf
        -4  /home1/user/testdb_lginf      /home1/CUBRID/databases/testdb_1_lginf
        -3  /home1/user/testdb_bkvinf     /home1/CUBRID/databases/testdb_1_bkvinf
        -2  /home1/user/testdb_lgat       /home1/CUBRID/databases/testdb_1_lgat
         0  /home1/user/testdb            /home1/CUBRID/databases/testdb_1
         1  /home1/user/backup/testdb_x001/home1/CUBRID/databases/backup/testdb_1_x001

.. option:: -d, --delete-backup

    This option renames the *testdb* database and at once forcefully delete all backup volumes and backup information files that are in the same location as *testdb*. Note that you cannot use the backup files with the old names once the database is renamed. If the **-d** option is not specified, backup volumes and backup information files are not deleted. ::

        cubrid renamedb -d testdb testdb_1

.. _alterdbhost:

alterdbhost
-----------

The **cubrid alterdbhost** utility sets or changes the host name of the specified database. It changes the host name set in the **databases.txt** file. ::

    cubrid alterdbhost [option] database_name

*   **cubrid**: An integrated utility for the CUBRID service and database management

*   **alterdbhost**: A command used to change the host name of the current database

The following shows the option available with the **cubrid alterdbhost** utility.

.. program:: alterdbhost

.. option:: -h, --host=HOST

    The *-h* option specifies the host name to be changed. When this option is omitted, specifies the host name to localhost.

.. _copydb:

copydb
------

The **cubrid copydb** utility copy or move a database to another location. As arguments, source and target name of database must be given. A target database name must be different from a source database name. When the target name argument is specified, the location of target database name is registered in the **databases.txt** file. 

The **cubrid copydb** utility can be executed only offline (that is, state of a source database stop). ::

    cubrid copydb [options] src-database-name dest-database-name

*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **copydb**: A command that copy or move a database from one to another location.

*   *src-database-name*: The names of source and target databases to be copied or moved.

*   *dest-database-name*: A new (target) database name.

If options are omitted, a target database is copied into the same directory of a source database.

The following shows [options] available with the **cubrid copydb** utility.

.. program:: copydb

.. option:: --server-name=HOST

    The *--server-name* option specifies a host name of new database. The host name is registered in the **databases.txt** file. If this option is omitted, a local host is registered. ::
    
        cubrid copydb --server-name=cub_server1 demodb new_demodb

.. option:: -F, --file-path=PATH

    The *-F* option specifies a specific directory path where a new database volume is stored with an **-F** option. It represents specifying an absolute path. If the specified directory does not exist, an error is displayed. If this option is omitted, a new database volume is created in the current working directory. And this information is specified in **vol-path** of the **databases.txt** file. ::

        cubrid copydb -F /home/usr/CUBRID/databases demodb new_demodb

.. option:: -L, --log-path=PATH

    The *-L* option specifies a specific directory path where a new database volume is stored with an **-L** option. It represents specifying an absolute path. If the specified directory does not exist, an error is displayed. If this option is omitted, a new database volume is created in the current working directory. And this information is specified in **log-path** of the **databases.txt** file. ::

        cubrid copydb -L /home/usr/CUBRID/databases/logs demodb new_demodb

.. option:: -E, --extended-volume-path=PATH

    The *-E* option specifies a specific directory path where a new database extended volume is stored with an **-E**. If this option is omitted, a new database extended volume is created in the location of a new database volume or in the registered path of controlling file. The **-i** option cannot be used with this option. ::

        cubrid copydb -E home/usr/CUBRID/databases/extvols demodb new_demodb

.. option:: -i, --control_file=FILE

    The **-i** option specifies an input file where a new directory path information and a source volume are stored to copy or move multiple volumes into a different directory, respectively. This option cannot be used with the **-E** option. An input file named copy_path is specified in the example below. ::

        cubrid copydb -i copy_path demodb new_demodb

    The following is an example of input file that contains each volume name, current directory path, and new directory and volume names. ::

        # volid   source_fullvolname   dest_fullvolname
        0 /usr/databases/demodb        /drive1/usr/databases/new_demodb
        1 /usr/databases/demodb_data1  /drive1/usr/databases/new_demodb new_data1
        2 /usr/databases/ext/demodb index1 /drive2//usr/databases/new_demodb new_index1
        3 /usr/ databases/ext/demodb index2  /drive2/usr/databases/new_demodb new_index2

    *   *volid*: An integer that is used to identify each volume. It can be checked in the database volume control file (**database_name_vinf**).

    *   *source_fullvolname*: The current directory path to each source database volume.

    *   *dest_fullvolname*: The target directory path where new volumes will be stored. You should specify a valid path.  

.. option:: -r, --replace

    If the **-r** option is specified, a new database name overwrites the existing database name if it is identical, instead of outputting an error. ::

        cubrid copydb -r -F /home/usr/CUBRID/databases demodb new_demodb

.. option:: -d, --delete-source

    If the **-d** option is specified, a source database is deleted after the database is copied. This execution brings the same the result as executing **cubrid deletedb** utility after copying a database. Note that if a source database contains LOB data, LOB file directory path of a source database is copied into a new database and it is registered in the **lob-base-path** of the **databases.txt** file. ::

        cubrid copydb -d -F /home/usr/CUBRID/databases demodb new_demodb

.. option:: --copy-lob-path=PATH

    If the **--copy-lob-path** option is specified, a new directory path for LOB files is created and a source database is copied into a new directory path. If this option is omitted, the directory path is not created. Therefore, the **lob-base-path** of the **databases.txt** file should be modified separately. This option cannot be used with the **-B** option. ::

        cubrid copydb --copy-lob-path demodb new_demodb

.. option:: -B, --lob-base-path=PATH

    If the **-B** option is specified, a specified directory is specified as for LOB files of a new database and a source database is copied. This option cannot be used with the **--copy-lob-path** option. ::

        cubrid copydb -B /home/usr/CUBRID/databases/new_lob demodb new_demodb

.. _installdb:

installdb
---------

The **cubrid installdb** utility is used to register the information of a newly installed database to **databases.txt**, which stores database location information. The execution of this utility does not affect the operation of the database to be registered. 

::

    cubrid installdb [options] database_name 
    
*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **installdb**: A command that registers the information of a moved or copied database to **databases.txt**.

*   *database_name*: The name of database to be registered to **databases.txt**.

If no [options] are used, the command must be executed in the directory where the corresponding database exists.

The following shows [options] available with the **cubrid installdb** utility.

.. program:: installdb

.. option:: --server-name=HOST

    This option registers the server host information of a database to **databases.txt** with a specific host name. If this is not specified, the current host information is registered. ::

        cubrid installdb --server-name=cub_server1 testdb

.. option::-F, --file-path=PATH

    This option registers the absolute directory path of a database volume to **databases.txt** by using the **-F** option. If this option is not specified, the path of a current directory is registered as default. ::

        cubrid installdb -F /home/cubrid/CUBRID/databases/testdb testdb

.. option:: -L, --log-path=PATH

    This option registers the absolute directory path of a database log volume to **databases.txt** by using the **-L** option. If this option is not specified, the directory path of a volume is registered. ::

        cubrid installdb -L /home/cubrid/CUBRID/databases/logs/testdb testdb

.. include:: backup.inc

.. include:: migration.inc

.. _spacedb:

spacedb
-------

The **cubrid spacedb** utility is used to check how much space of database volumes is being used. 
It shows a brief description of all permanent data volumes in the database. Information returned by the **cubrid spacedb** utility includes the ID, name, purpose and total/free space of each volume. You can also check the total number of volumes and used/unused database pages. 

::

    cubrid spacedb [options] database_name

*   **cubrid** : An integrated utility for the CUBRID service and database management.

*   **spacedb** : A command that checks the space in the database. It executes successfully only when the database is in a stopped state.

*   *database_name* : The name of the database whose space is to be checked. The path-name to the directory where the database is to be created must not be included.

The following shows [options] available with the **cubrid spacedb** utility.

.. program:: spacedb

.. option:: -o FILE

    This option stores the result of checking the space information of *testdb* to a file named *db_output*. ::

        cubrid spacedb -o db_output testdb

.. option:: -S, --SA-mode

    This option is used to access a database in standalone, which means it works without processing server; it does not have an argument. If **-S** is not specified, the system recognizes that a database is running in client/server mode. ::

        cubrid spacedb --SA-mode testdb

.. option:: -C, --CS-mode

    This option is used to access a database in client/server mode, which means it works in client/server process respectively; it does not have an argument. 
    If **-C** is not specified, the system recognize that a database is running in client/server mode by default. ::

        cubrid spacedb --CS-mode testdb

.. option:: --size-unit={PAGE|M|G|T|H}

    This option specifies the size unit of the space information of the database to be one of PAGE, M(MB), G(GB), T(TB), H(print-friendly). The default value is **H**. 
    If you set the value to H, the unit is automatically determined as follows: M if 1 MB = DB size < 1024 MB, G if 1 GB = DB size < 1024 GB. ::

        $ cubrid spacedb --size-unit=M testdb
        $ cubrid spacedb --size-unit=H testdb

        Space description for database 'testdb' with pagesize 16.0K. (log pagesize: 16.0K)

        Volid  Purpose    total_size   free_size  Vol Name

            0   GENERIC       20.0 M      17.0 M  /home1/cubrid/testdb
            1      DATA       20.0 M      19.5 M  /home1/cubrid/testdb_x001
            2     INDEX       20.0 M      19.6 M  /home1/cubrid/testdb_x002
            3      TEMP       20.0 M      19.6 M  /home1/cubrid/testdb_x003
            4      TEMP       20.0 M      19.9 M  /home1/cubrid/testdb_x004
        -------------------------------------------------------------------------------
            5                100.0 M      95.6 M
        Space description for temporary volumes for database 'testdb' with pagesize 16.0K.

        Volid  Purpose    total_size   free_size  Vol Name

        LOB space description file:/home1/cubrid/lob

.. option:: -s, --summarize

    This option aggregates total_pages, used_pages and free_pages by DATA, INDEX, GENERIC, TEMP and TEMP TEMP, and outputs them. ::

        $ cubrid spacedb -s testdb

        Summarized space description for database 'testdb' with pagesize 16.0K. (log pagesize: 16.0K)

        Purpose     total_size   used_size   free_size  volume_count
        -------------------------------------------------------------
              DATA      20.0 M       0.5 M      19.5 M          1
             INDEX      20.0 M       0.4 M      19.6 M          1
           GENERIC      20.0 M       3.0 M      17.0 M          1
              TEMP      40.0 M       0.5 M      39.5 M          2
         TEMP TEMP       0.0 M       0.0 M       0.0 M          0
        -------------------------------------------------------------
             TOTAL     100.0 M       4.4 M      95.6 M          5

.. option:: -p, --purpose

    This option separates the used space as data_size, index_size and temp_size, and outputs them.

    ::
    
        Space description for database 'testdb' with pagesize 16.0K. (log pagesize: 16.0K)

        Volid  Purpose    total_size   free_size   data_size  index_size   temp_size  Vol Name

            0   GENERIC       20.0 M      17.0 M       2.1 M       0.9 M       0.0 M  /home1/cubrid/testdb
            1      DATA       20.0 M      19.5 M       0.4 M       0.0 M       0.0 M  /home1/cubrid/testdb_x001
            2     INDEX       20.0 M      19.6 M       0.0 M       0.4 M       0.0 M  /home1/cubrid/testdb_x002
            3      TEMP       20.0 M      19.6 M       0.0 M       0.0 M       0.3 M  /home1/cubrid/testdb_x003
            4      TEMP       20.0 M      19.9 M       0.0 M       0.0 M       0.1 M  /home1/cubrid/testdb_x004
        ----------------------------------------------------------------------------------------------------
            5                100.0 M      95.6 M       2.5 M       1.2 M       0.4 M
        Space description for temporary volumes for database 'testdb' with pagesize 16.0K.

        Volid  Purpose    total_size   free_size   data_size  index_size   temp_size  Vol Name

        LOB space description file:/home1/cubrid/lob

.. note::

    If you use **-p** and **-s** together, the summarized information of the used space will be separated as data_size, index_size and temp_size.

    ::

        $ cubrid spacedb -s -p testdb
        Summarized space description for database 'testdb' with pagesize 16.0K. (log pagesize: 16.0K)

        Purpose     total_size   used_size   free_size   data_size  index_size   temp_size  volume_count
        -------------------------------------------------------------------------------------------------
              DATA      20.0 M       0.5 M      19.5 M       0.4 M       0.0 M       0.0 M          1
             INDEX      20.0 M       0.4 M      19.6 M       0.0 M       0.4 M       0.0 M          1
           GENERIC      20.0 M       3.0 M      17.0 M       2.1 M       0.9 M       0.0 M          1
              TEMP      40.0 M       0.5 M      39.5 M       0.0 M       0.0 M       0.4 M          2
         TEMP TEMP       0.0 M       0.0 M       0.0 M       0.0 M       0.0 M       0.0 M          0
        -------------------------------------------------------------------------------------------------
             TOTAL     100.0 M       4.4 M      95.6 M       2.5 M       1.2 M       0.4 M          5

.. _compactdb:

compactdb
---------

The **cubrid compactdb** utility is used to secure unused space of the database volume. In case the database server is not running (offline), you can perform the job in standalone mode. In case the database server is running, you can perform it in client-server mode.

.. note::

    The **cubrid compactdb** utility secures the space being taken by OIDs of deleted objects and by class changes. When an object is deleted, the space taken by its OID is not immediately freed because there might be other objects that refer to the deleted one. 

    Therefore, when you make a table to reuse OIDs, it is recommended to use a REUSE_OID option as below.
    
    .. code-block:: sql
    
        CREATE TABLE tbl REUSE_OID
        (
            id INT PRIMARY KEY, 
            b VARCHAR
        );
    
    However, a table with a REUSE_OID option cannot be referred by the other table. That is, this table cannot be used as a type of the other table.
    
    .. code-block:: sql
    
        CREATE TABLE reuse_tbl (a INT PRIMARY KEY) REUSE_OID;
        CREATE TABLE tbl_1 ( a reuse_tbl);
    
    ::
    
        ERROR: The class 'reuse_tbl' is marked as REUSE_OID and is non-referable. Non-referable classes can't be the domain of an attribute and their instances' OIDs cannot be returned.
 
    To see details of REUSE_OID, please refer to :ref:`reuse-oid`.

Reference to the object deleted during compacting is displayed as **NULL**, which means this can be reused by OIDs. ::

    cubrid compactdb [options] database_name [class_name], class_name2, ...]

*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **compactdb**: A command that compacts the space of the database so that OIDs assigned to deleted data can be reused.

*   *database_name*: The name of the database whose space is to be compacted. The path name to the directory where the database is to be created must not be included.

*   *class_name_list*: You can specify the list of tables names that you want to compact space after a database name; the **-i** option cannot be used together. It is used in client/server mode only.

**-I**, **-i**, **-c**, **-d**, **-p** options are applied in client/server mode only.

The following shows [options] available with the **cubrid compactdb** utility.

.. program:: compactdb

.. option:: -v, --verbose

    You can output messages that shows which class is currently being compacted and how many instances have been processed for the class by using the **-v** option. ::

        cubrid compactdb -v testdb

.. option:: -S, --SA-mode

    This option specifies to compact used space in standalone mode while database server is not running; no argument is specified.  
    If the **-S** option is not specified, system recognizes that the job is executed in client/server mode. ::

        cubrid compactdb --SA-mode testdb

.. option:: -C, --CS-mode

    This option specifies to compact used space in client/server mode while database server is running; no argument is specified. Even though this option is omitted, system recognizes that the job is executed in client/server mode. 

The following options can be used in client/server mode only.

.. option:: -i, --input-class-file=FILE

    You can specify an input file name that contains the table name with this option. Write one table name in a single line; invalid table name is ignored. Note that you cannot specify the list of the table names after a database name in case of you use this option.

.. option:: -p, --pages-commited-once=NUMBER

    You can specify the number of maximum pages that can be committed once with this option. The default value is 10, the minimum value is 1, and the maximum value is 10. The less option value is specified, the more concurrency is enhanced because the value for class/instance lock is small; however, it causes slowdown on operation, and vice versa. ::

        cubrid compactdb --CS-mode -p 10 testdb tbl1, tbl2, tbl5

.. option:: -d, --delete-old-repr

    You can delete an existing table representation (schema structure) from catalog with this option. Generally you'd better keep the existing table representation because schema updating cost will be saved when you keep the status as referring to the past schema for the old records.

.. option:: -I, --Instance-lock-timeout=NUMBER 

    You can specify a value of instance lock timeout with this option. The default value is 2 (seconds), the minimum value is 1, and the maximum value is 10. The less option value is specified, the more operation speeds up. However, the number of instances that can be processed becomes smaller, and vice versa.

.. option:: -c, --class-lock-timeout=NUMBER

    You can specify a value of instance lock timeout with this option. The default value is 10 (seconds), the minimum value is 1, and the maximum value is 10. The less option value is specified, the more operation speeds up. However, the number of tables that can be processed becomes smaller, and vice versa. 

.. _optimizedb:

optimizedb
----------

Updates statistical information such as the number of objects, the number of pages to access, and the distribution of attribute values. ::

    cubrid optimizedb [option] database_name

*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **optimizedb**: Updates the statistics information, which is used for cost-based query optimization of the database. If the option is specified, only the information of the specified class is updated.

*   *database_name*: The name of the database whose cost-based query optimization statistics are to be updated.

The following shows [option] available with the **cubrid optimizedb** utility.

.. program :: optimizedb

.. option:: -n, --class-name

    The following example shows how to update the query statistics information of the given class by using the **-n** option. ::

        cubrid optimizedb -n event_table testdb

The following example shows how to update the query statistics information of all classes in the database. ::

    cubrid optimizedb testdb

.. _plandump:

plandump
--------

The **cubrid plandump** utility is used to display information on the query plans stored (cached) on the server. ::

    cubrid plandump [options] database_name 

*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **plandump**: A utility that displays the query plans stored in the current cache of a specific database.

*   *database_name*: The name of the database where the query plans are to be checked or dropped from its server cache.

If no option is used, it checks the query plans stored in the cache. ::

    cubrid plandump testdb

The following shows [options] available with the **cubrid plandump** utility.

.. program :: plandump

.. option:: -d, --drop
 
    This option drops the query plans stored in the cache. ::

        cubrid plandump -d testdb

.. option:: -o, --output-file=FILE

    This option stores the results of the query plans stored in the cache to a file. ::

        cubrid plandump -o output.txt testdb

.. _statdump:

statdump
--------

**cubrid statdump** utility checks statistics information processed by the CUBRID database server. The statistics information mainly consists of the following: File I/O, Page buffer, Logs, Transactions, Concurrency/Lock, Index, and Network request.

You can also use CSQL's session commands to check the statistics information only about the CSQL's connection. For details, see :ref:`Dumping CSQL execution statistics information <csql-execution-statistics>`.

::

    cubrid statdump [options] database_name

*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **installdb**: A command that dumps the statistics information on the database server execution.

*   *database_name*: The name of database which has the statistics data to be dumped.

The following shows [options] available with the **cubrid statdump** utility.

.. program:: statdump

.. option:: -i, --interval=SECOND

    This option specifies the periodic number of Dumping statistics as seconds.

    The following outputs the accumulated values per second. ::
    
        cubrid statdump -i 1 -c demodb
        
    The following outputs the accumulated values during 1 second, as starting with 0 value per every 1 second. ::
    
        cubrid statdump -i 1 demodb
        
    The following outputs the last values which were executed with **-i** option. ::
    
        cubrid statdump demodb
        
    The following outputs the same values with the above. **-c** option doesn't work if it is not used with **-i** option together.
    
        cubrid statdump -c demodb

    The following outputs the values per every 5 seconds. 
    
    ::

        $ cubrid statdump -i 5 -c testdb
         

        Thu January 07 16:46:05 GTB Standard Time 2016

         *** SERVER EXECUTION STATISTICS ***
        Num_file_creates              =          0
        Num_file_removes              =          0
        Num_file_ioreads              =          0
        Num_file_iowrites             =          2
        Num_file_iosynches            =          2
        Num_file_page_allocs          =          0
        Num_file_page_deallocs        =          0
        Num_data_page_fetches         =       1742
        Num_data_page_dirties         =         60
        Num_data_page_ioreads         =          0
        Num_data_page_iowrites        =          0
        Num_data_page_victims         =          0
        Num_data_page_iowrites_for_replacement =          0
        Num_data_page_hash_anchor_waits =          0
        Time_data_page_hash_anchor_wait =          0
        Num_data_page_fixed           =          0
        Num_data_page_dirty           =         15
        Num_data_page_lru1            =          0
        Num_data_page_lru2            =          0
        Num_data_page_ain             =        128
        Num_data_page_avoid_dealloc   =          0
        Num_data_page_avoid_victim    =          0
        Num_data_page_victim_cand     =          0
        Num_log_page_fetches          =          0
        Num_log_page_fetch_ioreads    =          0
        Num_log_page_ioreads          =          0
        Num_log_page_iowrites         =          2
        Num_log_append_records        =         45
        Num_log_archives              =          0
        Num_log_start_checkpoints     =          0
        Num_log_end_checkpoints       =          0
        Num_log_wals                  =          0
        Num_log_page_iowrites_for_replacement =          0
        Num_page_locks_acquired       =          0
        Num_object_locks_acquired     =         65
        Num_page_locks_converted      =          0
        Num_object_locks_converted    =         10
        Num_page_locks_re-requested   =          0
        Num_object_locks_re-requested =         46
        Num_page_locks_waits          =          0
        Num_object_locks_waits        =          0
        Num_object_locks_time_waited_usec =          0
        Num_tran_commits              =          3
        Num_tran_rollbacks            =          1
        Num_tran_savepoints           =          2
        Num_tran_start_topops         =          6
        Num_tran_end_topops           =          6
        Num_tran_interrupts           =          0
        Num_btree_inserts             =          3
        Num_btree_deletes             =          0
        Num_btree_updates             =          0
        Num_btree_covered             =          0
        Num_btree_noncovered          =          0
        Num_btree_resumes             =          0
        Num_btree_multirange_optimization =          0
        Num_btree_splits              =          0
        Num_btree_merges              =          0
        Num_btree_get_stats           =          0
        Num_heap_stats_sync_bestspace =          0
        Num_query_selects             =          2
        Num_query_inserts             =          0
        Num_query_deletes             =          0
        Num_query_updates             =          2
        Num_query_sscans              =          2
        Num_query_iscans              =          0
        Num_query_lscans              =          1
        Num_query_setscans            =          0
        Num_query_methscans           =          0
        Num_query_nljoins             =          1
        Num_query_mjoins              =          0
        Num_query_objfetches          =          0
        Num_query_holdable_cursors    =          0
        Num_sort_io_pages             =          0
        Num_sort_data_pages           =          0
        Num_network_requests          =         79
        Num_adaptive_flush_pages      =          0
        Num_adaptive_flush_log_pages  =          2
        Num_adaptive_flush_max_pages  =     116610
        Num_prior_lsa_list_size       =          5
        Num_prior_lsa_list_maxed      =          0
        Num_prior_lsa_list_removed    =          2
        Num_heap_stats_bestspace_entries =          5
        Num_heap_stats_bestspace_maxed =          0
        Time_ha_replication_delay     =          0
        Num_plan_cache_add            =          1
        Num_plan_cache_lookup         =          2
        Num_plan_cache_hit            =          0
        Num_plan_cache_miss           =          2
        Num_plan_cache_full           =          0
        Num_plan_cache_delete         =          0
        Num_plan_cache_invalid_xasl_id =          0
        Num_plan_cache_query_string_hash_entries =          5
        Num_plan_cache_xasl_id_hash_entries =          5
        Num_plan_cache_class_oid_hash_entries =         10
        Num_vacuum_log_pages_vacuumed =          0
        Num_vacuum_log_pages_to_vacuum =          0
        Num_vacuum_prefetch_requests_log_pages =          0
        Num_vacuum_prefetch_hits_log_pages =          0
        Num_heap_home_inserts         =          0
        Num_heap_big_inserts          =          0
        Num_heap_assign_inserts       =          4
        Num_heap_home_deletes         =          0
        Num_heap_home_mvcc_deletes    =          0
        Num_heap_home_to_rel_deletes  =          0
        Num_heap_home_to_big_deletes  =          0
        Num_heap_rel_deletes          =          0
        Num_heap_rel_mvcc_deletes     =          0
        Num_heap_rel_to_home_deletes  =          0
        Num_heap_rel_to_big_deletes   =          0
        Num_heap_rel_to_rel_deletes   =          0
        Num_heap_big_deletes          =          0
        Num_heap_big_mvcc_deletes     =          0
        Num_heap_new_ver_inserts      =          0
        Num_heap_home_updates         =          6
        Num_heap_home_to_rel_updates  =          0
        Num_heap_home_to_big_updates  =          0
        Num_heap_rel_updates          =          0
        Num_heap_rel_to_home_updates  =          0
        Num_heap_rel_to_rel_updates   =          0
        Num_heap_rel_to_big_updates   =          0
        Num_heap_big_updates          =          0
        Num_heap_home_vacuums         =          0
        Num_heap_big_vacuums          =          0
        Num_heap_rel_vacuums          =          0
        Num_heap_insid_vacuums        =          0
        Num_heap_remove_vacuums       =          0
        Num_heap_next_ver_vacuums     =          0
        Time_heap_insert_prepare      =       1962
        Time_heap_insert_execute      =      10007
        Time_heap_insert_log          =         44
        Time_heap_delete_prepare      =          0
        Time_heap_delete_execute      =          0
        Time_heap_delete_log          =          0
        Time_heap_update_prepare      =        497
        Time_heap_update_execute      =        972
        Time_heap_update_log          =        267
        Time_heap_vacuum_prepare      =          0
        Time_heap_vacuum_execute      =          0
        Time_heap_vacuum_log          =          0
        Num_bt_find_unique            =          2
        Num_btrange_search            =          0
        Num_bt_insert_obj             =          3
        Num_bt_delete_obj             =          0
        Num_bt_mvcc_delete            =          0
        Num_bt_mark_delete            =          0
        Num_bt_update_sk_cnt          =          0
        Num_bt_undo_insert            =          0
        Num_bt_undo_delete            =          0
        Num_bt_undo_mvcc_delete       =          0
        Num_bt_undo_update_sk         =          0
        Num_bt_vacuum                 =          0
        Num_bt_vacuum_insid           =          0
        Num_bt_vacuum_update_sk       =          0
        Num_bt_fix_ovf_oids_cnt       =          0
        Num_bt_unique_rlocks_cnt      =          0
        Num_bt_unique_wlocks_cnt      =          0
        Time_bt_find_unique           =         17
        Time_bt_range_search          =          0
        Time_bt_insert                =       1845
        Time_bt_delete                =          0
        Time_bt_mvcc_delete           =          0
        Time_bt_mark_delete           =          0
        Time_bt_update_sk             =          0
        Time_bt_undo_insert           =          0
        Time_bt_undo_delete           =          0
        Time_bt_undo_mvcc_delete      =          0
        Time_bt_undo_update_sk        =          0
        Time_bt_vacuum                =          0
        Time_bt_vacuum_insid          =          0
        Time_bt_vacuum_update_sk      =          0
        Time_bt_traverse              =       1616
        Time_bt_find_unique_traverse  =        716
        Time_bt_range_search_traverse =          0
        Time_bt_insert_traverse       =        900
        Time_bt_delete_traverse       =          0
        Time_bt_mvcc_delete_traverse  =          0
        Time_bt_mark_delete_traverse  =          0
        Time_bt_update_sk_traverse    =          0
        Time_bt_undo_insert_traverse  =          0
        Time_bt_undo_delete_traverse  =          0
        Time_bt_undo_mvcc_delete_traverse =          0
        Time_bt_undo_update_sk_traverse =          0
        Time_bt_vacuum_traverse       =          0
        Time_bt_vacuum_insid_traverse =          0
        Time_bt_vacuum_update_sk_traverse =          0
        Time_bt_fix_ovf_oids          =          0
        Time_bt_unique_rlocks         =          0
        Time_bt_unique_wlocks         =          0
        Time_vacuum_master            =     152858
        Time_vacuum_worker_process_log =          0
        Time_vacuum_worker_execute    =          0
        
         *** OTHER STATISTICS ***
        Data_page_buffer_hit_ratio    =     100.00
        Log_page_buffer_hit_ratio     =       0.00
        Vacuum_data_page_buffer_hit_ratio =       0.00
        Vacuum_page_efficiency_ratio  =       0.00
        Vacuum_page_fetch_ratio       =       0.00
        Data_page_fix_lock_acquire_time_msec =       0.00
        Data_page_fix_hold_acquire_time_msec =       0.00
        Data_page_fix_acquire_time_msec =      11.80
        Data_page_allocate_time_ratio =     100.00
        Data_page_total_promote_success =       3.00
        Data_page_total_promote_fail  =       0.00
        Data_page_total_promote_time_msec =       0.00
        Num_data_page_fix_ext:
        WORKER,PAGE_FTAB     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =         17
        WORKER,PAGE_FTAB     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =          2
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,READ ,COND        =        194
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =          9
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,WRITE,COND        =         18
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =          2
        WORKER,PAGE_VOLHEADER,OLD_PAGE_IN_PB    ,READ ,COND        =          8
        WORKER,PAGE_VOLHEADER,OLD_PAGE_IN_PB    ,READ ,UNCOND      =        914
        WORKER,PAGE_VOLBITMAP,OLD_PAGE_IN_PB    ,READ ,COND        =          4
        WORKER,PAGE_VOLBITMAP,OLD_PAGE_IN_PB    ,READ ,UNCOND      =        457
        WORKER,PAGE_XASL     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =          2
        WORKER,PAGE_XASL     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =          1
        WORKER,PAGE_CATALOG  ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =         75
        WORKER,PAGE_CATALOG  ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =         10
        WORKER,PAGE_BTREE_R  ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =         29
        Num_data_page_promote_ext:
        WORKER,PAGE_BTREE_R  ,SHARED_READER,READ ,SUCCESS =          3
        Num_data_page_promote_time_ext:
        WORKER,PAGE_BTREE_R  ,SHARED_READER,READ ,SUCCESS =          3
        Num_data_page_unfix_ext:
        WORKER,PAGE_FTAB     ,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =         16
        WORKER,PAGE_FTAB     ,BUF_NON_DIRTY,HOLDER_DIRTY    ,WRITE =          2
        WORKER,PAGE_FTAB     ,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =          1
        WORKER,PAGE_HEAP     ,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =        185
        WORKER,PAGE_HEAP     ,BUF_NON_DIRTY,HOLDER_DIRTY    ,WRITE =          9
        WORKER,PAGE_HEAP     ,BUF_NON_DIRTY,HOLDER_DIRTY    ,MIXED =          2
        WORKER,PAGE_HEAP     ,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =         14
        WORKER,PAGE_HEAP     ,BUF_DIRTY    ,HOLDER_NON_DIRTY,WRITE =          4
        WORKER,PAGE_HEAP     ,BUF_DIRTY    ,HOLDER_DIRTY    ,WRITE =          3
        WORKER,PAGE_HEAP     ,BUF_DIRTY    ,HOLDER_DIRTY    ,MIXED =          6
        WORKER,PAGE_VOLHEADER,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =         14
        WORKER,PAGE_VOLHEADER,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =        908
        WORKER,PAGE_VOLBITMAP,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =        461
        WORKER,PAGE_XASL     ,BUF_NON_DIRTY,HOLDER_DIRTY    ,WRITE =          1
        WORKER,PAGE_XASL     ,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =          2
        WORKER,PAGE_CATALOG  ,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =         47
        WORKER,PAGE_CATALOG  ,BUF_NON_DIRTY,HOLDER_DIRTY    ,WRITE =          1
        WORKER,PAGE_CATALOG  ,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =         28
        WORKER,PAGE_CATALOG  ,BUF_DIRTY    ,HOLDER_NON_DIRTY,WRITE =          1
        WORKER,PAGE_CATALOG  ,BUF_DIRTY    ,HOLDER_DIRTY    ,WRITE =          8
        WORKER,PAGE_BTREE_R  ,BUF_NON_DIRTY,HOLDER_NON_DIRTY,READ  =         18
        WORKER,PAGE_BTREE_R  ,BUF_NON_DIRTY,HOLDER_DIRTY    ,MIXED =          3
        WORKER,PAGE_BTREE_R  ,BUF_DIRTY    ,HOLDER_NON_DIRTY,READ  =          8
        Time_data_page_lock_acquire_time:
        Time_data_page_hold_acquire_time:
        Time_data_page_fix_acquire_time:
        WORKER,PAGE_FTAB     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =               65
        WORKER,PAGE_FTAB     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =               12
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,READ ,COND        =              617
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =               42
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,WRITE,COND        =               81
        WORKER,PAGE_HEAP     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =                9
        WORKER,PAGE_VOLHEADER,OLD_PAGE_IN_PB    ,READ ,COND        =               36
        WORKER,PAGE_VOLHEADER,OLD_PAGE_IN_PB    ,READ ,UNCOND      =             3277
        WORKER,PAGE_VOLBITMAP,OLD_PAGE_IN_PB    ,READ ,COND        =               18
        WORKER,PAGE_VOLBITMAP,OLD_PAGE_IN_PB    ,READ ,UNCOND      =             1533
        WORKER,PAGE_XASL     ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =                5
        WORKER,PAGE_XASL     ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =             5644
        WORKER,PAGE_CATALOG  ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =              260
        WORKER,PAGE_CATALOG  ,OLD_PAGE_IN_PB    ,WRITE,UNCOND      =               43
        WORKER,PAGE_BTREE_R  ,OLD_PAGE_IN_PB    ,READ ,UNCOND      =              164
        Num_mvcc_snapshot_ext:
        DELETE  ,INS_VACUUMED      ,VISIBLE   =                7
        DIRTY   ,INS_VACUUMED      ,VISIBLE   =                3
        DIRTY   ,INS_CURR          ,VISIBLE   =                2
        SNAPSHOT,INS_VACUUMED      ,VISIBLE   =               87
        SNAPSHOT,DELETED_COMMITED  ,INVISIBLE =                1
        Time_obj_lock_acquire_time:
        Time_get_snapshot_acquire_time:
        WORKER =               14
        Count_get_snapshot_retry:
        WORKER =               11
        Time_tran_complete_time:
        WORKER =               19
        Time_get_oldest_mvcc_acquire_time:
        SYSTEM =           112110
        Count_get_oldest_mvcc_retry:
        WORKER =                1

    The following are the explanation about the above statistical information.

    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Category         | Item                                     | Description                                                                            |
    +==================+==========================================+========================================================================================+
    | File I/O         | Num_file_removes                         | The number of files removed                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_file_creates                         | The number of files created                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_file_ioreads                         | The number of files read                                                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_file_iowrites                        | The number of files stored                                                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_file_iosynches                       | The number of file synchronization                                                     |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Page buffer      | Num_data_page_fetches                    | The number of fetched pages                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_dirties                    | The number of dirty pages                                                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_ioreads                    | The number of pages read from disk                                                     |
    |                  |                                          | (more means less efficient, it correlates with lower hit ratio)                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_iowrites                   | The number of pages write to disk (more means less efficient)                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_victims                    | The number of times the flushing thread is wake up                                     |
    |                  |                                          | (NOT the number of victims or flushed pages)                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_iowrites_for_replacement   | The number of the written data pages specified as victim                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_hash_anchor_waits          | The number of instances any hash anchor had to wait for mutex acquisition              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_data_page_hash_anchor_wait          | The total time in microseconds any hash anchor had to wait for mutex acquisition       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_fixed                      | The number of fixed pages in data buffer (snapshot counter)                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_dirty                      | The number of dirty pages in data buffer (snapshot counter)                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_lru1                       | The number of pages in LRU1 zone in data buffer (snapshot counter)                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_lru2                       | The number of pages in LRU2 zone in data buffer (snapshot counter)                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_ain                        | The number of pages in AIN zone in data buffer (snapshot counter)                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_avoid_dealloc              | The number of pages in data buffer having 'avoid_dealloc_cnt > 0' (snapshot counter)   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_avoid_victim               | The number of pages in data buffer having 'avoid_victim' flag set (snapshot counter)   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_victim_cand                | The number of pages in data buffer which are victim candidates (snapshot counter)      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_victim_cand                | The number of pages in data buffer which are victim candidates (snapshot counter)      |    
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_adaptive_flush_pages                 | The number of data pages flushed from the data buffer to the disk                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_adaptive_flush_log_pages             | The number of log pages flushed from the log buffer to the disk                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_adaptive_flush_max_pages             | The maximum number of pages allowed to flush from data and the log buffer              |
    |                  |                                          | to the disk                                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_prior_lsa_list_size                  | Current size of the prior LSA(Log Sequence Address) list.                              |
    |                  |                                          | CUBRID write the order of writing into the prior LSA list, before writing operation    |
    |                  |                                          | from the log buffer to the disk; this list is used to raise up the concurrency         |
    |                  |                                          | by reducing the waiting time of the transaction from writing to disk                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_prior_lsa_list_maxed                 | The count of the prior LSA list being reached at the maximum size.                     |
    |                  |                                          | The maximum size of the prior LSA list is log_buffer_size * 2. If this value is big,   |
    |                  |                                          | we can assume that log writing jobs happen a lot at the same time                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_prior_lsa_list_removed               | The count of LSA being moved from prior LSA list into log buffer.                      |
    |                  |                                          | We can assume that the commits have happened at the similar count with this value      |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Logs             | Num_log_page_fetches                     | The number of fetched log pages                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_page_fetch_ioreads               | The number of I/O reads of fetched log pages                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_page_ioreads                     | The number of log pages read                                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_page_iowrites                    | The number of log pages stored                                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_append_records                   | The number of log records appended                                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_archives                         | The number of logs archived                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_start_checkpoints                | The number of started checkpoints                                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_end_checkpoints                  | The number of ended checkpoints                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_wals                             | Not used                                                                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_log_page_iowrites_for_replacement    | The number of log data pages discarded from log page buffer due to page replacements   |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Concurrency/lock | Num_page_locks_acquired                  | The number of locked pages acquired                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_object_locks_acquired                | The number of locked objects acquired                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_page_locks_converted                 | The number of locked pages converted                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_object_locks_converted               | The number of locked objects converted                                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_page_locks_re-requested              | The number of locked pages requested                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_object_locks_re-requested            | The number of locked objects requested                                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_page_locks_waits                     | The number of locked pages waited                                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_object_locks_waits                   | The number of locked objects waited                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_object_locks_time_waited_usec        | The time in microseconds spent on waiting for all object locks                         |    
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Transactions     | Num_tran_commits                         | The number of commits                                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_tran_rollbacks                       | The number of rollbacks                                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_tran_savepoints                      | The number of savepoints                                                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_tran_start_topops                    | The number of top operations started                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_tran_end_topops                      | The number of top operations stopped                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_tran_interrupts                      | The number of interruptions                                                            |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Index            | Num_btree_inserts                        | The number of nodes inserted                                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_deletes                        | The number of nodes deleted                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_updates                        | The number of nodes updated                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_covered                        | The number of cases in which an index includes all data upon query execution           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_noncovered                     | The number of cases in which an index includes some or no data upon query execution    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_resumes                        | The exceeding number of index scan specified in index_scan_oid_buffer_pages            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_multirange_optimization        | The number of executions on multi-range optimization for the WHERE ... IN ...          |
    |                  |                                          | LIMIT condition query statement                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_splits                         | The number of B-tree split-operations                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btree_merges                         | The number of B-tree merge-operations                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_find_unique                       | The number of B-tree 'find-unique' operations                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_btrange_search                       | The number of B-tree 'range-search' operations                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_insert_obj                        | The number of B-tree 'insert object' operations                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_delete_obj                        | The number of B-tree 'physical delete object' operations                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_mvcc_delete                       | The number of B-tree 'mvcc delete' operations                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_mark_delete                       | The number of B-tree mark delete operations                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_update_sk_cnt                     | The number of B-tree 'update same key' operations                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_undo_insert                       | The number of B-tree 'undo insert' operations                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_undo_delete                       | The number of B-tree 'undo physical delete' operations                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_undo_mvcc_delete                  | The number of B-tree 'undo mvcc delete' operations                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_undo_update_sk                    | The number of B-tree 'undo update same key' operations                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_vacuum                            | The number of B-tree vacuum deleted object operations                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_vacuum_insid                      | The number of vacuum operations on B-tree 'insert id'                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_vacuum_update_sk                  | The number of vacuum operations on B-tree 'update same key'                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_fix_ovf_oids_cnt                  | The number of B-tree overflow page fixes                                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_unique_rlocks_cnt                 | The number of blocked read locks on unique indexes                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_bt_unique_wlocks_cnt                 | The number of blocked write locks on unique indexes                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_find_unique                      | Time consumed on B-tree 'find-unique' operations                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_range_search                     | Time consumed on B-tree 'range search' operations                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_insert                           | Time consumed on B-tree insert object operations                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_delete                           | Time consumed on B-tree physical delete operations                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_mvcc_delete                      | Time consumed on B-tree mvcc delete operations                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_mark_delete                      | Time consumed on B-tree mark delete operations                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_update_sk                        | Time consumed on B-tree 'update same key' operations                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_insert                      | Time consumed on B-tree 'undo insert' operations                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_delete                      | Time consumed on B-tree 'undo physical delete' operations                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_mvcc_delete                 | Time consumed on B-tree 'undo mvcc delete' operations                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_update_sk                   | Time consumed on B-tree 'undo update same key' operations                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum                           | Time consumed on B-tree vacuum deleted object operations                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum_insid                     | Time consumed on B-tree vacuum operations of 'insert id'                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum_update_sk                 | Time consumed on B-tree vacuum operations of 'update same key'                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_traverse                         | Time consumed on B-tree traverse operations                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_find_unique_traverse             | Time consumed on B-tree traverse operations for 'find unique'                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_range_search_traverse            | Time consumed on B-tree traverse operations for 'range search'                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_insert_traverse                  | Time consumed on B-tree traverse operations for 'insert'                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_delete_traverse                  | Time consumed on B-tree traverse operations for 'physical delete'                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_mvcc_delete_traverse             | Time consumed on B-tree traverse operations for 'mvcc delete'                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_mark_delete_traverse             | Time consumed on B-tree traverse operations for 'mark delete'                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_update_sk_traverse               | Time consumed on B-tree traverse operations for 'update same key'                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_insert_traverse             | Time consumed on B-tree traverse operations for 'undo physical insert'                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_delete_traverse             | Time consumed on B-tree traverse operations for 'undo physical delete'                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_mvcc_delete_traverse        | Time consumed on B-tree traverse operations for 'undo delete'                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_undo_update_sk_traverse          | Time consumed on B-tree traverse operations for 'undo update same key'                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum_traverse                  | Time consumed on B-tree traverse operations for vacuum deleted object                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum_insid_traverse            | Time consumed on B-tree traverse operations for vacuum 'insert id'                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_vacuum_update_sk_traverse        | Time consumed on B-tree traverse operations for vacuum 'update same key'               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_fix_ovf_oids                     | Time consumed on B-tree overflow pages fix                                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_unique_rlocks                    | Time consumed on read locks on unique indexes                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_bt_unique_wlocks                    | Time consumed on write locks on unique indexes                                         |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Query            | Num_query_selects                        | The number of SELECT query execution                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_inserts                        | The number of INSERT query execution                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_deletes                        | The number of DELETE query execution                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_updates                        | The number of UPDATE query execution                                                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_sscans                         | The number of sequential scans (full scan)                                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_iscans                         | The number of index scans                                                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_lscans                         | The number of LIST scans                                                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_setscans                       | The number of SET scans                                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_methscans                      | The number of METHOD scans                                                             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_nljoins                        | The number of nested loop joins                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_mjoins                         | The number of parallel joins                                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_objfetches                     | The number of fetch objects                                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_query_holdable_cursors               | The number of holdable cursors in the current server.                                  |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Sort             | Num_sort_io_pages                        | The number of pages fetched on the disk during sorting(more means less efficient)      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_sort_data_pages                      | The number of pages found on the page buffer during sorting(more means more efficient) |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Network request  | Num_network_requests                     | The number of network requested                                                        |
    |                  |                                          |                                                                                        |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Heap             | Num_heap_stats_bestspace_entries         | The number of best pages which are saved on the "best page" list                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_stats_bestspace_maxed           | The maximum number of best pages which can be saved on the "best page" list            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_stats_sync_bestspace            | The updated number of the "best page" list.                                            |
    |                  |                                          |                                                                                        |
    |                  |                                          | "Best pages" means that the data pages of which the free space is more than 30%        |
    |                  |                                          | in the environment of multiple INSERTs and DELETEs. Only some information of these     |
    |                  |                                          | pages are saved as the "best page" list. In the "best page" list, the information of   |
    |                  |                                          | a million pages is saved at once. This list is searched when INSERTing a record, and   |
    |                  |                                          | then this list is updated when there are no free space to store this record on         |
    |                  |                                          | the pages. If there are still no free space to store this record even this list is     |
    |                  |                                          | updated for several times, this recored is stored into a new page.                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_inserts                    | The number of inserts in heap HOME type records                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_big_inserts                     | The number of inserts in heap BIG type records                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_assign_inserts                  | The number of inserts in heap ASSIGN type records                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_deletes                    | The number of deletes from heap HOME type records in non-MVCC mode                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_mvcc_deletes               | The number of deletes from heap HOME type records in MVCC mode                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_to_rel_deletes             | The number of deletes from heap HOME to RELOCATION type records in MVCC mode           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_to_big_deletes             | The number of deletes from heap HOME to BIG type records in MVCC mode                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_deletes                     | The number of deletes from heap RELOCATION type records in non-MVCC mode               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_mvcc_deletes                | The number of deletes from heap RELOCATION type records in MVCC mode                   |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_home_deletes             | The number of deletes from heap RELOCATION to HOME type records in MVCC mode           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_big_deletes              | The number of deletes from heap RELOCATION to BIG type records in MVCC mode            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_rel_deletes              | The number of deletes from heap RELOCATION to RELOCATION type records in MVCC mode     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_big_deletes                     | The number of deletes from heap BIG type records in non-MVCC mode                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_big_mvcc_deletes                | The number of deletes from heap BIG type records in MVCC mode                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_new_ver_inserts                 | The number of inserts of new versions of the same object in MVCC mode                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_updates                    | The number of updates in place of heap HOME type records in non-MVCC mode(*)           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_to_rel_updates             | The number of updates of heap HOME to RELOCATION type records in non-MVCC mode(*)      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_to_big_updates             | The number of updates of heap HOME to BIG type records in non-MVCC mode(*)             |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_updates                     | The number of updates of heap RELOCATION type records in non-MVCC mode(*)              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_home_updates             | The number of updates of heap RELOCATION to HOME type records in non-MVCC mode(*)      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_rel_updates              | The number of updates of heap RELOCATION to RELOCATION type records in non-MVCC mode(*)|
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_to_big_updates              | The number of updates of heap RELOCATION to BIG type records in non-MVCC mode(*)       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_big_updates                     | The number of updates of heap BIG type records in non-MVCC mode(*)                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_home_vacuums                    | The number of vacuumed heap HOME type records                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_big_vacuums                     | The number of vacuumed heap BIG type records                                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_rel_vacuums                     | The number of vacuumed heap RELOCATION type records                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_insid_vacuums                   | The number of vacuumed heap newly inserted records                                     |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_remove_vacuums                  | The number of vacuum operations that remove version and don’t keep next version        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_heap_next_ver_vacuums                | The number of vacuum operations that remove version and keep their next version        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_insert_prepare                 | The time spend on preparing heap insert operation                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_insert_execute                 | The time spend on executing heap insert operation                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_insert_log                     | The time spend on logging heap insert operation                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_delete_prepare                 | The time spend on preparing heap delete operation                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_delete_execute                 | The time spend on executing heap delete operation                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_delete_log                     | The time spend on logging heap delete operation                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_update_prepare                 | The time spend on preparing heap update operation                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_update_execute                 | The time spend on executing heap update operation                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_update_log                     | The time spend on logging heap update operation                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_vacuum_prepare                 | The time spend on preparing heap vacuum operation                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_vacuum_execute                 | The time spend on executing heap vacuum operation                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_heap_vacuum_log                     | The time spend on logging heap vacuum operation                                        |        
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Query plan cache | Num_plan_cache_add                       | The number of newly added cache entry                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_lookup                    | The number of lookup try with a special key                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_hit                       | The number of the hit entries in the query string hash table                           |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_miss                      | The number of the missed entries in the query string hash table                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_full                      | The number of the victim retrieval by the full plan cache                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_delete                    | The number of victimized cache entries                                                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_invalid_xasl_id           | The number of missed entries in the xasl_id hash table.                                |
    |                  |                                          | The number of errors occurred when some entries are requested in the client            |
    |                  |                                          | during those entries are victimized in the server                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_query_string_hash_entries | The current entry number of the query string hash table                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_xasl_id_hash_entries      | The current entry number of xasl id hash table                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_plan_cache_class_oid_hash_entries    | The current entry number of class oid hash table                                       |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | HA               | Time_ha_replication_delay                | Replication latency time (sec.)                                                        |
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Vacuuming        | Num_vacuum_log_pages_vacuumed            | The number of data pages vacuumed by vacuum workers.                                   |
    |                  |                                          | This counter is not update in real-time.                                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_vacuum_log_pages_to_vacuum           | The number of data pages to be vaccumed by vacuum workers                              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_vacuum_prefetch_requests_log_pages   | The number of requests to prefetch buffer for log pages from vacuum                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_vacuum_prefetch_hits_log_pages       | The number of hits to prefetch buffer for log pages from vacuum                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_vacuum_master                       | Time consumed by vacuum master thread                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_vacuum_worker_process_log           | Time consumed by vacuum worker thread for logging                                      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_vacuum_worker_execute               | Time consumed by vacuum worker thread for execution                                    |    
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+
    | Other            | Data_page_buffer_hit_ratio               | Hit ratio of data page buffers                                                         |
    |                  |                                          | (Num_data_page_fetches - Num_data_page_ioreads)*100 / Num_data_page_fetches            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Log_page_buffer_hit_ratio                | Hit ratio of log page buffers                                                          |
    |                  |                                          | (Num_log_page_fetches - Num_log_page_fetch_ioreads)*100 / Num_log_page_fetches         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Vacuum_data_page_buffer_hit_ratio        | Hit ratio of vacuuming data page buffers                                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Vacuum_page_efficiency_ratio             | Ratio between number of page unfix of vacuum with dirty flag and total number of       |
    |                  |                                          | page unfix of vacuum. Ideally, the vacuum process performs only write operations since |
    |                  |                                          | it cleans up all unused records. Even with an optimized vacuum process,                |
    |                  |                                          | 100% eficiency is not possible.                                                        |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Vacuum_page_fetch_ratio                  | Ratio (percentage) of page unfix from vacuum module and total page unfix.              |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_fix_lock_acquire_time_msec     | Cumulated time to acquire page lock                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_fix_hold_acquire_time_msec     | Cumulated time to acquire page hold                                                    |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_fix_acquire_time_msec          | Cumulated time to acquire fix                                                          |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_allocate_time_ratio            | Ratio of cumulated time necessary for page allocation                                  |
    |                  |                                          | (Data_page_fix_acquire_time_msec - Data_page_fix_hold_acquire_time_msec -              |
    |                  |                                          | Data_page_fix_lock_acquire_time_msec)*100 / Data_page_fix_acquire_time_msec            |
    |                  |                                          | Large values indicate the IO as main bottleneck, small value indicate concurrency as   |
    |                  |                                          | main bottleneck (page hold and lock times).                                            |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_total_promote_success          | Cumulated number of successfully latch promote                                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_total_promote_fail             | Cumulated number of failed latch promote                                               |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Data_page_total_promote_time_msec        | Cumulated time to promote latch                                                        |        
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_fix_ext:                   | The number of data page fix by module, page type, and if page is new old or if is      |
    |                  |                                          | found in page buffer.                                                                  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_data_page_unfix_ext:                 | The number of data page unfix by module, page type, and if page was dirtied or clean.  |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_data_page_lock_acquire_time:        | time to acquire page lock (partitioned by module,page type and mode, latch mode,       |
    |                  |                                          | condition mode).                                                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_data_page_hold_acquire_time:        | Time to acquire page hold (partitioned by module,page type and mode, latch mode).      |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_data_page_fix_acquire_time:         | Time to acquire page fix (partitioned by module,page type and mode, latch mode,        |
    |                  |                                          | condition mode).                                                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Num_mvcc_snapshot_ext:                   | The number of snapshot validation functions are called (partitioned by snapshot type,  |
    |                  |                                          | record type, visibility result upon validation).                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_obj_lock_acquire_time:              | Time required to acquire object lock (partitioned by module, lock type)                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_get_snapshot_acquire_time:          | Time required by snapshot validation functions (partitioned by snapshot type,          |
    |                  |                                          | record type, visibility result upon validation).                                       |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Count_get_snapshot_retry:                | The number of retries to acquire MVCC snapshot (partitioned by module)                 |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_tran_complete_time:                 | Time spent to invalidate snapshot and MVCCID on transaction commit/rollback            |
    |                  |                                          | (partitioned by module)                                                                |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Time_get_oldest_mvcc_acquire_time:       | Time spend to acquire "oldest MVCC ID" (partitioned by module)                         |
    |                  +------------------------------------------+----------------------------------------------------------------------------------------+
    |                  | Count_get_oldest_mvcc_retry:             | The number of retries to acquire "oldest MVCC ID"  (partitioned by module)             |    
    +------------------+------------------------------------------+----------------------------------------------------------------------------------------+

.. Note::  

    (*) : These statistics measure the non-MVCC operations or MVCC operations which are performed in-place (decided internally)

    
.. option:: -o, --output-file=FILE

    **-o** options is used to store statistics information of server processing for the database to a specified file.  ::

        cubrid statdump -o statdump.log testdb

.. option:: -c, --cumulative

    You can display the accumulated operation statistics information of the target database server by using the **-c** option. 
    
    Num_data_page_fix_ext, Num_data_page_unfix_ext, Time_data_page_hold_acquire_time, Time_data_page_fix_acquire_time information can be output only when this option is specified; however, these informations will be omitted because they are for CUBRID Engine developers.

    By combining this with the **-i** option, you can check the operation statistics information at a specified interval.  ::

        cubrid statdump -i 5 -c testdb

.. option:: -s, --substr=STRING

    You can display statistics about items, the names of which include the specified string by using **-s** option. 

    The following example shows how to display statistics about items, the names of which include "data".

    ::
    
        cubrid statdump -s data testdb

        *** SERVER EXECUTION STATISTICS ***
        Num_data_page_fetches         =        135
        Num_data_page_dirties         =          0
        Num_data_page_ioreads         =          0
        Num_data_page_iowrites        =          0
        Num_data_page_victims         =          0
        Num_data_page_iowrites_for_replacement =          0
         
         *** OTHER STATISTICS ***
        Data_page_buffer_hit_ratio    =     100.00

.. note::

    Each status information consists of 64-bit INTEGER data and the corresponding statistics information can be lost if the accumulated value exceeds the limit.

.. _lockdb:

lockdb
------

The **cubrid lockdb** utility is used to check the information on the lock being used by the current transaction in the database. ::

    cubrid lockdb [options] database_name

*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **lockdb**: A command used to check the information on the lock being used by the current transaction in the database.

*   *database_name*: The name of the database where lock information of the current transaction is to be checked.

The following example shows how to display lock information of the *testdb* database on a screen without any option. ::

    cubrid lockdb testdb

The following shows [options] available with the **cubrid statdump** utility.
    
.. program:: lockdb

.. option:: -o, --output-file=FILE

    The **-o** option displays the lock information of the *testdb* database as a output.txt. ::

        cubrid lockdb -o output.txt testdb

Output Contents
^^^^^^^^^^^^^^^

The output contents of **cubrid lockdb** are divided into three logical sections.

*   Server lock settings

*   Clients that are accessing the database

*   The contents of an object lock table

**Server lock settings**

The first section of the output of **cubrid lockdb** is the database lock settings.

::

    *** Lock Table Dump ***
     Lock Escalation at = 100000, Run Deadlock interval = 0

The lock escalation level is 100,000 records, and the interval to detect deadlock is set to 0 seconds.

For a description of the related system parameters, **lock_escalation** and **deadlock_detection_interval**, see :ref:`lock-parameters`.

**Clients that are accessing the database**

The second section of the output of **cubrid lockdb** includes information on all clients that are connected to the database. This includes the transaction index, program name, user ID, host name, process ID, isolation level and lock timeout settings of each client.

::

    Transaction (index 1, csql, dba@cubriddb|12854)
    Isolation COMMITTED READ
    Timeout_period : Infinite wait

Here, the transaction index is 1, the program name is csql, the user ID is dba, the host name is cubriddb, the client process identifier is 12854, the isolation level is COMMITTED READ and the lock timeout is unlimited.

A client for which transaction index is 0 is the internal system transaction. It can obtain the lock at a specific time, such as the processing of a checkpoint by a database. In most cases, however, this transaction will not obtain any locks.

Because **cubrid lockdb** utility accesses the database to obtain the lock information, the **cubrid lockdb** is an independent client and will be output as such.

**Object lock table**

The third section of the output of the **cubrid lockdb** includes the contents of the object lock table. It shows which client has the lock for which object in which mode, and which client is waiting for which object in which mode. The first part of the result of the object lock table shows how many objects are locked.

::

    Object lock Table:
        Current number of objects which are locked = 2001

**cubrid lockdb** outputs the OID, object type and table name of each object that obtained lock. In addition, it outputs the number of transactions that hold lock for the object (*Num holders*), the number of transactions (*Num blocked-holders*) that hold lock but are blocked since it could not convert the lock to the upper lock (e.g., conversion from **SCH_S_LOCK** to **SCH_M_LOCK**), and the number of different transactions that are waiting for the lock of the object (*Num waiters*). It also outputs the list of client transactions that hold lock, blocked client transactions and waiting client transactions. For rows, but not class, MVCC information is also shown.

The example below shows an object in which the object type is a class, that will be blocked because the class OID( 0| 62| 5 ) that has **IX_LOCK** for transaction 1 and **SCH_S_LOCK** for transaction 2 cannot be converted into **SCH_M_LOCK**. It also shows that transaction 3 is blocked because transaction 2 is waiting for **SCH_M_LOCK** even when transaction 3 is only waiting for **SCH_S_LOCK**.

::

    OID = 0| 62| 5
    Object type: Class = athlete.
    Num holders = 1, Num blocked-holders= 1, Num waiters = 1
    LOCK HOLDERS :
        Tran_index = 1, Granted_mode = IX_LOCK, Count = 1, Nsubgranules = 1
    BLOCKED LOCK HOLDERS :
        Tran_index = 2, Granted_mode = SCH_S_LOCK, Count = 1, Nsubgranules = 0
        Blocked_mode = SCH_M_LOCK
                        Start_waiting_at = Wed Feb 3 14:44:31 2016
                        Wait_for_secs = -1
    LOCK WAITERS :
        Tran_index = 3, Blocked_mode = SCH_S_LOCK
                        Start_waiting_at = Wed Feb 3 14:45:14 2016
                        Wait_for_secs = -1
                        
The next example shows an instance of class, object OID( 2| 50| 1 ), that was inserted by transaction 1 which holds **X_LOCK** on the object. The class has a unique index and the key of inserted instance is about to be modified by transaction 2, which is blocked until transaction 1 is completed.

::

    OID = 2| 50| 1
    Object type: instance of class ( 0| 62| 5) = athlete.
    MVCC info: insert ID = 6, delete ID = missing.
    Num holders = 1, Num blocked-holders= 1, Num waiters = 1
    LOCK HOLDERS :
        Tran_index =   1, Granted_mode =   X_LOCK, Count =   1
    LOCK WAITERS :
        Tran_index =   2, Blocked_mode = X_LOCK
                          Start_waiting_at = Wed Feb 3 14:45:14 2016
                          Wait_for_secs = -1

*Granted_mode* refers to the mode of the obtained lock, and *Blocked_mode* refers to the mode of the blocked lock. *Starting_waiting_at refers* to the time at which the lock was requested, and *Wait_for_secs* refers to the waiting time of the lock. The value of *Wait_for_secs* is determined by **lock_timeout**, a system parameter.

When the object type is a class (table), *Nsubgranules* is displayed, which is the sum of the record locks and the key locks obtained by a specific transaction in the table.

::

    OID = 0| 62| 5
    Object type: Class = athlete.
    Num holders = 2, Num blocked-holders= 0, Num waiters= 0
    LOCK HOLDERS:
    Tran_index = 3, Granted_mode = IX_LOCK, Count = 2, Nsubgranules = 0
    Tran_index = 1, Granted_mode = IX_LOCK, Count = 3, Nsubgranules = 1
    Tran_index = 2, Granted_mode = IX_LOCK, Count = 2, Nsubgranules = 1
    
.. _tranlist:

tranlist
--------

The **cubrid tranlist** is used to check the transaction information of the target database. Only DBA or DBA group can use this utility. ::

    cubrid tranlist [options] database_name

If you omit the [options], it displays the total information about each transaction.

"cubrid tranlist demodb" outputs the similar result with "cubrid killtran -q demodb", but tranlist outputs more items; "User name" and "Host name".
"cubrid tranlist -s demodb" outputs the same result with "cubrid killtran -d demodb".

The following shows what information is displayed when you run "cubrid tranlist demodb".

::

    $ cubrid tranlist demodb

    Tran index          User name      Host name      Process id    Program name              Query time    Tran time       Wait for lock holder      SQL_ID       SQL Text
    --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
       1(ACTIVE)           public     test-server           1681    broker1_cub_cas_1               0.00         0.00                       -1     *** empty ***  
       2(ACTIVE)           public     test-server           1682    broker1_cub_cas_2               0.00         0.00                       -1     *** empty ***  
       3(ACTIVE)           public     test-server           1683    broker1_cub_cas_3               0.00         0.00                       -1     *** empty ***  
       4(ACTIVE)           public     test-server           1684    broker1_cub_cas_4               1.80         1.80                  3, 2, 1     e5899a1b76253   update ta set a = 5 where a > 0
       5(ACTIVE)           public     test-server           1685    broker1_cub_cas_5               0.00         0.00                       -1     *** empty ***  
    --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    SQL_ID: e5899a1b76253
    Tran index : 4
    update ta set a = 5 where a > 0

In the above example, when each three transaction is running INSERT statement, UPDATE statement is tried to run in the other transaction. In the above, UPDATE statement with "Tran index" 4 waits for the transactions 3,2,1, which are found in "Wait for lock holder", to be ended.

"SQL Text" is SQLs which are stored into the query plan cache; this is printed out as **empty** when this query's execution is terminated.

Each column's meaning is as following.

    *   Tran index : the index of transaction
    *   User name: database user's name
    *   Host name: host name of CAS which running this transaction
    *   Process id :  client's process id
    *   Program name : program name of a client
    *   Query time : total execution time for the running query (unit: second)
    *   Tran time : total run time for the current transaction (unit: second)
    *   Wait for lock holder : the list of transactions which own the lock when the current transaction is waiting for a lock
    *   SQL_ID: an ID for SQL Text
    *   SQL Text : running  SQL text (maximum 30 characters)

Transaction status messages, which are shown on "Tran index", are as follows.
    
    *   ACTIVE : active state
    *   RECOVERY : recovering transaction
    *   COMMITTED : transaction which is already committed and will be ended soon.
    *   COMMITTING : transaction which is committing
    *   ABORTED : transaction which is rolled back and will be ended soon. 
    *   KILLED : transaction which is forcefully killed by the server.

The following shows [options] available with the **cubrid tranlist** utility.

.. program:: tranlist

.. option:: -u, --user=USER

    *USER* is DB user's ID to log-in. It only allows DBA and DBA group users.(The default: DBA)
    
.. option:: -p, --password=PASSWORD

    *PASSWORD* is DB user's password.
    
.. option:: -s, --summary

    This option outputs only summarized information(it omits query execution information or locking information).

    ::
    
        $ cubrid tranlist -s demodb
        
        Tran index          User name      Host name      Process id      Program name
        -------------------------------------------------------------------------------
           1(ACTIVE)           public     test-server           1681 broker1_cub_cas_1
           2(ACTIVE)           public     test-server           1682 broker1_cub_cas_2
           3(ACTIVE)           public     test-server           1683 broker1_cub_cas_3
           4(ACTIVE)           public     test-server           1684 broker1_cub_cas_4
           5(ACTIVE)           public     test-server           1685 broker1_cub_cas_5
        -------------------------------------------------------------------------------

.. option:: --sort-key=NUMBER
 
    This option outputs the ascending values sorted by the NUMBERth column.
    If the type of the column is the number, it is sorted by the number; if not, it is sorted by the string. If this option is omitted, the output is sorted by "Tran index".

    The following is an example which outputs the sorted information by specifying the "Process id", the 4th column.
     
    ::
     
        $ cubrid tranlist --sort-key=4 demodb
     
        Tran index          User name      Host name      Process id    Program name              Query time    Tran time       Wait for lock holder      SQL_ID       SQL Text
        --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
           1(ACTIVE)           public     test-server           1681    broker1_cub_cas_1               0.00         0.00                       -1     *** empty ***
           2(ACTIVE)           public     test-server           1682    broker1_cub_cas_2               0.00         0.00                       -1     *** empty ***
           3(ACTIVE)           public     test-server           1683    broker1_cub_cas_3               0.00         0.00                       -1     *** empty ***
           4(ACTIVE)           public     test-server           1684    broker1_cub_cas_4               1.80         1.80                  3, 1, 2     e5899a1b76253   update ta set a = 5 where a > 0
           5(ACTIVE)           public     test-server           1685    broker1_cub_cas_5               0.00         0.00                       -1     *** empty ***
        --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        SQL_ID: e5899a1b76253
        Tran index : 4
        update ta set a = 5 where a > 0
        
.. option:: --reverse
 
    This option outputs the reversely sorted values.
 
    The following is an example which outputs the reversely sorted values by the "Tran index".
     
    ::
     
        Tran index          User name      Host name      Process id    Program name              Query time    Tran time     Wait for lock holder      SQL_ID       SQL Text
        ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
           5(ACTIVE)           public     test-server           1685    broker1_cub_cas_5               0.00         0.00                     -1     *** empty ***
           4(ACTIVE)           public     test-server           1684    broker1_cub_cas_4               1.80         1.80                3, 2, 1     e5899a1b76253   update ta set a = 5 where a > 0
           3(ACTIVE)           public     test-server           1683    broker1_cub_cas_3               0.00         0.00                     -1     *** empty ***
           2(ACTIVE)           public     test-server           1682    broker1_cub_cas_2               0.00         0.00                     -1     *** empty ***
           1(ACTIVE)           public     test-server           1681    broker1_cub_cas_1               0.00         0.00                     -1     *** empty ***
        ------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        SQL_ID: e5899a1b76253
        Tran index : 4
        update ta set a = 5 where a > 0

.. _killtran:

killtran
--------

The **cubrid killtran** is used to check transactions or abort specific transaction. Only a DBA can execute this utility. ::

    cubrid killtran [options] database_name

*   **cubrid**: An integrated utility for the CUBRID service and database management

*   **killtran**: A utility that manages transactions for a specified database

*   *database_name*: The name of database whose transactions are to be killed

Some [options] refer to killing specified transactions; others refer to print active transactions. If no option is specified, **-d** is specified by default so all transactions are displayed on the screen.

::

    cubrid killtran demodb 
     
    Tran index      User name   Host name      Process id      Program name
    -------------------------------------------------------------------------------
       1(ACTIVE)          dba      myhost             664           cub_cas
       2(ACTIVE)          dba      myhost            6700              csql
       3(ACTIVE)          dba      myhost            2188           cub_cas
       4(ACTIVE)          dba      myhost             696              csql
       5(ACTIVE)       public      myhost            6944              csql
    -------------------------------------------------------------------------------

The following shows [options] available with the **cubrid killtran** utility.

.. program:: killtran

.. option:: -i, --kill-transaction-index=ID1,ID2,ID3

    This option kills transactions in a specified index. Several transaction indexes can be specified by separating with comma(,). If there is an invalid transaction ID among several IDs, it is ignored. ::

        $ cubrid killtran -i 1 demodb
        Ready to kill the following transactions:
         
        Tran index          User name      Host name      Process id      Program name
        -------------------------------------------------------------------------------
           1(ACTIVE)              DBA         myhost           15771              csql
           2(ACTIVE)              DBA         myhost            2171              csql
        -------------------------------------------------------------------------------
        Do you wish to proceed ? (Y/N)y
        Killing transaction associated with transaction index 1
        Killing transaction associated with transaction index 2

.. option:: --kill-user-name=ID

    This option kills transactions for a specified OS user ID. ::

        cubrid killtran --kill-user-name=os_user_id demodb

.. option::  --kill- host-name=HOST

    This option kills transactions of a specified client host. ::

        cubrid killtran --kill-host-name=myhost demodb

.. option:: --kill-program-name=NAME

    This option kills transactions for a specified program.  ::

        cubrid killtran --kill-program-name=cub_cas demodb

.. option:: --kill-sql-id=SQL_ID
        
    This option kills transactions for a specified SQL ID. ::

        cubrid killtran --kill-sql-id=5377225ebc75a demodb

.. option:: -p PASSWORD

    A value followed by the -p option is a password of the **DBA**, and should be entered in the prompt.

.. option:: -q, --query-exec-info

    The difference with the output of "cubrid tranlist" command is that there are no "User name" column and "Host name" column. See :ref:`tranlist`.

.. option:: -d, --display

    This is the default option and it displays the summary of transactions. Its output is the same as the output of "cubrid tranlist" with **-s** option. See :option:`tranlist -s`
        
.. option:: -f, --force

    This option omits a prompt to check transactions to be stopped. ::

        cubrid killtran -f -i 1 demodb

.. _checkdb:

checkdb
-------

The **cubrid checkdb** utility is used to check the consistency of a database. You can use **cubrid checkdb** to identify data structures that are different from indexes by checking the internal physical consistency of the data and log volumes. If the **cubrid checkdb** utility reveals any inconsistencies, you must try automatic repair by using the --**repair** option.

::

    cubrid checkdb [options] database_name [table_name1 table_name2 ...]

*   **cubrid**: An integrated utility for CUBRID service and database management.

*   **checkdb**: A utility that checks the data consistency of a specific database.

*   *database_name*: The name of the database whose consistency status will be either checked or restored.

*   *table_name1 table_name2*: List the table names for consistency check or recovery

The following shows [options] available with the **cubrid checkdb** utility.

.. program:: checkdb

.. option:: -S, --SA-mode

    This option is used to access a database in standalone, which means it works without processing server; it does not have an argument. If **-S** is not specified, the system recognizes that a database is running in client/server mode. ::

        cubrid checkdb -S demodb

.. option:: -C, --CS-mode

    This option is used to access a database in client/server mode, which means it works in client/server process respectively; it does not have an argument. If **-C** is not specified, the system recognize that a database is running in client/server mode by default. ::

        cubrid checkdb -C demodb

.. option:: -r, --repair

    This option is used to restore an issue if a consistency error occurs in a database. ::

        cubrid checkdb -r demodb

.. option:: --check-prev-link 
  
    This option is used to check if there are errors on previous links of an index.
     
    :: 
  
        $ cubrid checkdb --check-prev-link demodb 

.. option:: --repair-prev-link 
  
    This option is used to restore if there are errors on previous links of an index.
     
    :: 
  
        $ cubrid checkdb --repair-prev-link demodb

.. option:: -i, --input-class-file=FILE

    You can specify tables to check the consistency or to restore, by specifying the **-i** *FILE* option or listing the table names after a database name. Both ways can be used together. If a target is not specified, entire database will be a target of consistency check or restoration. ::

        cubrid checkdb demodb tbl1 tbl2
        cubrid checkdb -r demodb tbl1 tbl2
        cubrid checkdb -r -i table_list.txt demodb tbl1 tbl2

    Empty string, tab, carriage return and comma are separators among table names in the table list file specified by **-i** option. The following example shows the table list file; from t1 to t10, it is recognized as a table for consistency check or restoration. ::

        t1 t2 t3,t4 t5
        t6, t7 t8   t9
         
             t10

.. option:: --check-file-tracker 

    Check about all pages of all files in file-trackers.

.. option:: --check-heap 

    Check about all heap-files. 

.. option:: --check-catalog 

    Check the consistency about catalog information.

.. option:: --check-btree 

    Check the validity about all B-tree indexes.

.. option:: --check-class-name 

    Check the identical between the hash table of a class name and the class information(oid) brought from a heap file. 

.. option:: --check-btree-entries 

    Check the consistency of all B-tree entries.

.. option:: -I, --index-name=INDEX_NAME

    Check if the index specified with this option about checking table. If you use this option, there is no heap validation check.
    Only one table and one index are permitted when you use this option; if you don't input a table name or input two tables, an error occurs.

.. _diagdb:

diagdb
------

You can check various pieces of internal information on the database with the **cubrid diagdb** utility. Information provided by **cubrid diagdb** is helpful in diagnosing the current status of the database or figuring out a problem. ::

    cubrid diagdb options database_name

*   **cubrid**: An integrated utility for the CUBRID service and database management.

*   **diagdb**: A command that is used to check the current storage state of the database by Dumping the information contained in the binary file managed by CUBRID in text format. It normally executes only when the database is in a stopped state. You can check the whole database or the file table, file size, heap size, class name or disk bitmap selectively by using the provided option.
    
*   *database_name*: The name of the database whose internal information is to be diagnosed.

The following shows [options] available with the **cubrid diagdb** utility.

.. program:: diagdb

.. option:: -d, --dump-type=TYPE

    This option specifies the output range when you display the information of all files in the *demodb* database. If any option is not specified, the default value of -1 is used. ::

        cubrid diagdb -d 1 demodb

    The utility has 9 types of **-d** options as follows:

    +------+--------------------------------------+
    | Type | Description                          |
    +------+--------------------------------------+
    | -1   | Displays all database information.   |
    +------+--------------------------------------+
    | 1    | Displays file table information.     |
    +------+--------------------------------------+
    | 2    | Displays file capacity information.  |
    +------+--------------------------------------+
    | 3    | Displays heap capacity information.  |
    +------+--------------------------------------+
    | 4    | Displays index capacity information. |
    +------+--------------------------------------+
    | 5    | Displays class name information.     |
    +------+--------------------------------------+
    | 6    | Displays disk bitmap information.    |
    +------+--------------------------------------+
    | 7    | Displays catalog information.        |
    +------+--------------------------------------+
    | 8    | Displays log information.            |
    +------+--------------------------------------+
    | 9    | Displays heap information.           |
    +------+--------------------------------------+

.. _paramdump:

paramdump
---------

The **cubrid paramdump** utility outputs parameter information used in the server/client process. ::

    cubrid paramdump [options] database_name

*   **cubrid**: An integrated utility for the CUBRID service and database management
    
*   **paramdump**: A utility that outputs parameter information used in the server/client process
   
*   *database_name*: The name of the database in which parameter information is to be displayed.

The following shows [options] available with the **cubrid paramdump** utility.

.. program:: paramdump

.. option:: -o, --output-file=FILE

    The **-o** option is used to store information of the parameters used in the server/client process of the database into a specified file. The file is created in the current directory. If the **-o** option is not specified, the message is displayed on a console screen. ::

        cubrid paramdump -o db_output demodb

.. option:: -b, --both

    The **-b** option is used to display parameter information used in server/client process on a console screen. If the **-b** option is not specified, only server-side information is displayed. ::
     
        cubrid paramdump -b demodb

.. option:: -S, --SA-mode

    This option displays parameter information of the server process in standalone mode. ::

        cubrid paramdump -S demodb

.. option:: -C, --CS-mode

    This option displays parameter information of the server process in client/server mode. ::

        cubrid paramdump -C demodb

HA Commands
-----------

**cubrid changemode** utility prints or changes the HA mode.

**cubrid applyinfo** utility prints the information of applied transaction logs in the HA environment.

For more details, see :ref:`cubrid-service-util`.

.. _locale-command:

Locale Commands
---------------

**cubrid genlocale** utility compiles the locale information to use. This utility is executed in the **make_locale.sh** script ( **.bat** for Windows).

**cubrid dumplocale** utility dumps the compiled binary locale (CUBRID locale library) file as a human-readable format on the console. It is better to save the output as a file by output redirection.

**cubrid synccolldb** utility checks if the collations between database and locale library are consistent or not, and synchronize them.

For more detailed usage, see :ref:`locale-setting`.


Timezone Commands
-----------------

**cubrid gen_tz** utility compiles the IANA timezone information included in tzdata folder into a C source code. This utility is executed in the **make_tz.sh** script ( **.bat** for Windows).

**cubrid dump_tz** utility dumps the compiled CUBRID timezone library file as a human-readable format on the console. It is better to save the output as a file by output redirection.
