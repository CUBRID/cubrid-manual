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

CUBRID stores information on the locations of all existing databases in the **databases.txt** file. This file is called the "database location file." A database location file is used when CUBRID executes utilities for creating, renaming, deleting or replicating databases; it is also used when CUBRID runs each database. By default, this file is located in the **databases** directory under the installation directory. The directory is located through the environment variable **CUBRID_DATABASES**. 

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

    *   there are generic, data, index and temp volumes in database volumes.
    *   there are an active log, an archiving log and a background archiving log in log volumes.
    
*   In temporary volume, there is a temporary temp volume.

For more details on volumes, see :ref:`database-volume-structure`.

The following is an example of files related to the database when testdb database is operated.

+----------------+-------+-----------------+----------------+------------------------------------------------------------------------------------------------------+
| File name      | Size  | Type            | Classification | Description                                                                                          |
+================+=======+=================+================+======================================================================================================+
| testdb         | 40MB  | generic         | Database       | The firstly created volume when DB is created. This is used as **generic** volume and includes       |
|                |       |                 | volume         | the meta information of DB. The file size is 40M because "cubrid createdb" is executed after         |
|                |       |                 |                | db_volume_size in cubrid.conf is specified as 40M                                                    |
|                |       |                 |                | or the option of "cubrid createdb", --db-volume-size is specified as 40M.                            |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_x001    | 40MB  | one of generic, |                | Automatically created **generic** file or a file created by the user's command for adding a volume.  |
|                |       | data, index and |                | The size of **generic** file which was automatically created became 40MB because DB was started      |
|                |       | temp            |                | after specifying db_volume_size in cubrid.conf as 40M.                                               |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_x002    | 40MB  | one of generic, |                | Automatically created **generic** file or a file created by the user's command for adding a volume.  |
|                |       | data, index and |                |                                                                                                      |
|                |       | temp            |                |                                                                                                      |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_x003    | 40MB  | one of generic, |                | Automatically created **generic** file or a file created by the user's command for adding a volume.  |
|                |       | data, index and |                |                                                                                                      |
|                |       | temp            |                |                                                                                                      |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_x004    | 40MB  | one of generic, |                | Automatically created **generic** file or a file created by the user's command for adding a volume.  |
|                |       | data, index and |                |                                                                                                      |
|                |       | temp            |                |                                                                                                      |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_x005    | 40MB  | one of generic, |                | Automatically created **generic** file or a file created by the user's command for adding a volume.  |
|                |       | data, index and |                |                                                                                                      |
|                |       | temp            |                |                                                                                                      |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_x006    | 2GB   | one of generic, |                | Automatically created **generic** file or a file created by the user's command for adding a volume.  |
|                |       | data, index and |                | The size became 2GB because DB was restarted after specifying db_volume_size in cubrid.conf as 2G or |
|                |       | temp            |                | the option of "cubrid addvoldb", --db-volume-size is specified as 2G.                                |
+----------------+-------+-----------------+----------------+------------------------------------------------------------------------------------------------------+
| testdb_t32766  | 360MB | temporary temp  | None           | a file created temporarily when the space of **temp** volume is insufficient during running          |
|                |       |                 |                | the **temp** volume required query(e.g.: sorting, scanning, index creation).                         |
|                |       |                 |                | This is removed when DB is restarted. But, this should not be deleted arbitrarily.                   |
+----------------+-------+-----------------+----------------+------------------------------------------------------------------------------------------------------+
| testdb_lgar_t  | 40MB  | background      | Log            | A log file which is related to the background archiving feature.                                     |
|                |       | archiving       | volume         | This is used when storing the archiving log.                                                         |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_lgar224 | 40MB  | archiving       |                | Archiving logs are continuously archived and the files ending with three digits are created.         |
|                |       |                 |                | At this time, archiving logs from 001~223 seem to be removed normally by "cubrid backupdb" -r option |
|                |       |                 |                | or the setting of log_max_archives in cubrid.conf. When archiving logs are removed, you can see the  |
|                |       |                 |                | removed archiving log numbers in the REMOVE section of lginf file. See :ref:`managing-archive-logs`. |
+----------------+-------+-----------------+                +------------------------------------------------------------------------------------------------------+
| testdb_lgat    | 40MB  | active          |                | Active log file                                                                                      |
+----------------+-------+-----------------+----------------+------------------------------------------------------------------------------------------------------+

*   Database volume file

    *   In the above, testdb, testdb_x001 ~ testdb_x006 are classified as the database volume files.
    *   File size is determined by "db_volume_size" in cubrid.conf or the "--db-volume-size" option of "cubrid createdb" and "cubrid addvoldb".
    *  The type of an automatically created volume is always **generic**.
    
*   Log volume file

    *   In the above, testdb_lgar_t, testdb_lgar22 and testdb_lgat are classified as the log volume files.
    *   File size is determined by "log_volume_size" in cubrid.conf or the "--log-volume-size" option of "cubrid createdb".