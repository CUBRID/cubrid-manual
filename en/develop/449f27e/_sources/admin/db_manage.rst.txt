
:meta-keywords: cubrid databases.txt, cubrid users, cubrid volume
:meta-description: How to manage CUBRID Databases, Users and Volumes.

Database Management
===================

Database Users
--------------

A CUBRID database user can have members with the same authorization. If authorization **A** is granted to a user, the same authorization is also granted to all members belonging to the user. A database user and its members are called a "group."; a user who has no members is called a "user."

CUBRID provides **DBA** and **PUBLIC** users by default.

*   **DBA** can access every object in the database, that is, it has authorization at the highest level. Only **DBA** has sufficient authorization to add, alter and delete the database users.

*   All users including **DBA** are members of **PUBLIC**. Therefore, all database users have the authorization granted to **PUBLIC** . For example, if authorization **B** is added to **PUBLIC** group, all database members will automatically have the **B** authorization.

.. _databases-txt-file:

databases.txt File
------------------

CUBRID stores information on the locations of all existing databases in the **databases.txt** file. This file is called the "database location file". A database location file is used when CUBRID executes utilities for creating, renaming, deleting or replicating databases; it is also used when CUBRID runs each database. By default, this file is located in the **databases** directory under the installation directory. The directory is located through the environment variable **CUBRID_DATABASES**. 

::

    db_name db_directory server_host logfile_directory

The format of each line of a database location file is the same as defined by the above syntax; it contains information on the database name, database path, server host and the path to the log files. The following example shows how to check the contents of a database location file.

::

    % more databases.txt

    dist_testdb /home1/user/CUBRID/bin d85007 /home1/user/CUBRID/bin
    dist_demodb /home1/user/CUBRID/bin d85007 /home1/user/CUBRID/bin
    testdb /home1/user/CUBRID/databases/testdb d85007 /home1/user/CUBRID/databases/testdb
    demodb /home1/user/CUBRID/databases/demodb d85007 /home1/user/CUBRID/databases/demodb

By default, the database location file is stored in the **databases** directory under the installation directory. You can change the default directory by modifying the value of the **CUBRID_DATABASES** environment variable. The path to  the database location file must be valid so that the **cubrid** utility for database management can access the file properly. You must enter the directory path correctly and check if you have write permission on the file. The following example shows how to check the value configured in the **CUBRID_DATABASES** environment variable.

::

    % set | grep CUBRID_DATABASES
    CUBRID_DATABASES=/home1/user/CUBRID/databases

An error occurs if an invalid directory path is set in the **CUBRID_DATABASES** environment variable. If the directory path is valid but the database location file does not exist, a new location information file is created. If the **CUBRID_DATABASES** environment variable has not been configured at all, CUBRID retrieves the location information file in the current working directory.

.. _database-volume:

Database Volume
----------------

The volumes of CUBRID database are classified as permanent volume, temporary volume and backup volume.

*   In the permanent volumes,

    *   there are data volumes that usually store permanent data, but can also store temporary data.
    *   there is the active log volume that includes late changes of database.
    
*   In the temporary volumes,

    *   there are temporary temp volumes that store temporary data.
    *   there are archive log volumes that store exhausted logs from the active log.
    *   there is one background archive log volume that stores archive log in the background.

For more details on volumes, see :ref:`database-volume-structure`.

The following is an example of files related to the database when *testdb* database operates.

+----------------+-------+-----------------+----------------+------------------------------------------------------------------------------------------------------+
| File name      | Size  | Purpose         | Classification | Description                                                                                          |
+================+=======+=================+================+======================================================================================================+
| testdb         | 512MB | | permanent     | | Database     | | The firstly created volume when DB is created.                                                     |
|                |       | | data          | | volume       | | This volume stores permanent data (system, heap and index files).                                  |
|                |       |                 |                | | This volume includes database meta information.                                                    |
|                |       |                 |                | | **cubrid createdb** uses by default the size specified by **db_volume_size** in **cubrid.conf**.   |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_perm    | 512MB | | permanent     |                | | Manually added volume using **cubrid addvoldb** utility                                            |
|                |       | | data          |                | | This volume stores permanent data (system, heap and index files).                                  |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_temp    | 512MB | | temporary     |                | | Manually added volume using **cubrid addvoldb** utility                                            |
|                |       | | data          |                | | This volume stores temporary data (query results, list files, sort files, join object hashes).     |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_x003    | 512MB | | permanent     |                | | Automatically created when database requires more space.                                           |
|                |       | | data          |                | | This volume stores permanent data (system, heap and index files).                                  |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_x004    | 512MB | | permanent     |                | | Automatically created when database requires more space.                                           |
|                |       | | data          |                | | This volume stores permanent data (system, heap and index files).                                  |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_x005    | 512MB | | permanent     |                | | Automatically created when database requires more space.                                           |
|                |       | | data          |                | | This volume stores permanent data (system, heap and index files).                                  |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_x006    | 64MB  | | permanent     |                | | Automatically created when database requires more space.                                           |
|                |       | | data          |                | | This volume stores permanent data (system, heap and index files).                                  |
|                |       |                 |                | | The size of volume is not maximized (yet).                                                         |
+----------------+-------+-----------------+----------------+------------------------------------------------------------------------------------------------------+
| testdb_t32766  | 512MB | | temporary     | | Temporary    | | Automatically created when database requires more space.                                           |
|                |       | | data          | | Volume       | | This volume stores temporary data (query results, list files, sort files, join object hashes).     |
+----------------+-------+-----------------+----------------+------------------------------------------------------------------------------------------------------+
| testdb_lgar_t  | 512MB | | background    | | Log          | | A log file which is related to the background archiving feature.                                   |
|                |       | | archiving     | | volume       | | This is used when storing the archiving log.                                                       |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_lgar224 | 512MB | | archive       |                | | Archiving logs are continuously archived and the files ending with three digits are created.       |
|                |       |                 |                | | At this time, archiving logs from 001~223 seem to be removed normally by **cubrid backupdb** -r    |
|                |       |                 |                | | option or the setting of **log_max_archives** in **cubrid.conf**. When archiving logs are removed, |
|                |       |                 |                | | you can see the removed archiving log numbers in the REMOVE section of lginf file.                 |
|                |       |                 |                | | See :ref:`managing-archive-logs`.                                                                  |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_lgat    | 512MB | | active        |                | | Active log file                                                                                    |
+----------------+-------+-----------------+----------------+------------------------------------------------------------------------------------------------------+
| testdb_dwb     |  1MB  | | temporary     | | Double write | Double write buffer storage file, where flushed pages are written first.                             |
|                |       | | data          | | buffer       |                                                                                                      |
+----------------+-------+-----------------+----------------+------------------------------------------------------------------------------------------------------+

*   Database volume file

    *  In the table above, *testdb*, *testdb_perm*, *testdb_temp*, *testdb_x003* ~ *testdb_x006* are classified as the database volume files.
    *  File size is determined by **db_volume_size** in **cubrid.conf** or the **\-\-db-volume-size** option of **cubrid createdb** and **cubrid addvoldb**.
    *  When database remains out of space, it automatically expands existing volumes and creates new volumes.

*   Temporary volume

    *  Temporary volumes are usually used to store temporary data. They are automatically created and destroyed by database.
    *  File size is determined by **db_volume_size** in **cubrid.conf**.
    
*   Log volume file

    *   In the above, *testdb_lgar_t*, *testdb_lgar224* and *testdb_lgat* are classified as the log volume files.
    *   File size is determined by **log_volume_size** in **cubrid.conf** or the **\-\-log-volume-size** option of **cubrid createdb**.

*   Double write buffer file
    *   Double write buffer file is a storage area used to protect against I/O errors (partial writes).
    *   Every data page write is first written into the buffer and then flushed to its location in the permanent data volumes.
    *   During database reboot, partially written page is detected and replaced with the counterpart page in double write buffer.
    *   The file size is determined by **double_write_buffer_size** in **cubrid.conf**. If set to zero, no file is created and double write buffer is disabled.

.. note::

    Any data that has to be persistent over database restart and crash is stored in the database volumes created for permanent data purpose. The volumes store table rows (heap files), indexes (b-tree files) and several system files.

    Intermediate and final results of query processing and sorting need only temporary storage. Based on the size of required temporary data, it will be first stored in memory (the space size is determined by the system parameter **temp_file_memory_size_in_pages** specified in **cubrid.conf**). Exceeding data has to be stored on disk.

    Database will usually create and use temporary volumes to allocate disk space for temporary data. Administrator may however assign permanent database volumes with the purpose of storing temporary data using by running **cubrid addvoldb -p temp** command. If such volumes exist, they will have priority over temporary volumes when disk space is allocated for temporary data.

    The examples of queries that can use temporary data are as follows:

       *   Queries creating the resultset like **SELECT**
       *   Queries including **GROUP BY** or **ORDER BY**
       *   Queries including a subquery
       *   Queries executing sort-merge join
       *   Queries including the **CREATE INDEX** statement

    To have complete control on the disk space used for temporary data and to prevent it from consuming all system disk space, our recommendation is to:
    
       *   create permanent database volumes in advance to secure the required space for temporary data
       *   limit the size of the space used in the temporary volumes when a queries are executed by setting **temp_file_max_size_in_pages** parameter in **cubrid.conf** (there is no limit by default).
    
