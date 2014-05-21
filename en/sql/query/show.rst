****
SHOW
****

DESC, DESCRIBE
==============

It shows the column information of a table, and it's like a **SHOW COLUMNS** statement. For more details, see :ref:`show-columns-statement`.

::

    DESC tbl_name;
    DESCRIBE tbl_name;
    
EXPLAIN
=======

It shows the column information of a table, and it's like a **SHOW COLUMNS** statement. For more details, see :ref:`show-columns-statement`.

::

    EXPLAIN tbl_name;

.. _show-tables-statement:

SHOW TABLES
===========

It shows the list of all table names within a database. The name of the result column will be *tables_in_<database name>* and it will have one column. If you use the **LIKE** clause, you can search the table names matching this and if you use the **WHERE** clause, you can search table names with more general terms. **SHOW FULL TABLES** displays the second column, *table_type* together. The table must have the value, **BASE TABLE** and the view has the value, **VIEW**. ::

    SHOW [FULL] TABLES [LIKE 'pattern' | WHERE expr];

The following shows the examples of this syntax.

.. code-block:: sql

    SHOW TABLES;

::
    
    Tables_in_demodb
    ======================
      'athlete'
      'code'
      'event'
      'game'
      'history'
      'nation'
      'olympic'
      'participant'
      'record'
      'stadium'
     
.. code-block:: sql

    SHOW FULL TABLES;
    
::

      Tables_in_demodb     Table_type
    ============================================
      'athlete'             'BASE TABLE'
      'code'                'BASE TABLE'
      'event'               'BASE TABLE'
      'game'                'BASE TABLE'
      'history'             'BASE TABLE'
      'nation'              'BASE TABLE'
      'olympic'             'BASE TABLE'
      'participant'         'BASE TABLE'
      'record'              'BASE TABLE'
      'stadium'             'BASE TABLE'
     
.. code-block:: sql

    SHOW FULL TABLES LIKE '%c%';
    
::

      Tables_in_demodb      Table_type
    ============================================
      'code'                'BASE TABLE'
      'olympic'             'BASE TABLE'
      'participant'         'BASE TABLE'
      'record'              'BASE TABLE'
     
.. code-block:: sql

    SHOW FULL TABLES WHERE table_type = 'BASE TABLE' and TABLES_IN_demodb LIKE '%co%';
    
::

      Tables_in_demodb      Table_type
    ============================================
      'code'                'BASE TABLE'
      'record'              'BASE TABLE'

.. _show-columns-statement:

SHOW COLUMNS
============

It shows the column information of a table. You can use the **LIKE** clause to search the column names matching it. If you use the **WHERE** clause, you can search column names with more general terms like, "General Considerations for All **SHOW** Statements.".

::

    SHOW [FULL] COLUMNS {FROM | IN} tbl_name [LIKE 'pattern' | WHERE expr];

If a **FULL** keyword is used, It shows the additional information, collation.

**SHOW FIELDS** is the same statement as **SHOW COLUMNS**.

The **DESCRIBE** (abbreviated **DESC**) statement and the **EXPLAIN** statement provide the same information with **SHOW COLUMNS**, but they don't support LIKE clause or WHERE clause.

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Field                               VARCHAR         Column name
Type                                VARCHAR         Column data type
Null                                VARCHAR         If you can store **NULL**, the value is YES; if not, it is NO
Key                                 VARCHAR         Whether a column has an index or not. If there is more than one key value in the given column of a table, this displays only the one that appears first in the order of PRI, UNI and MUL.
                                                        *   If the key is a space, the column doesn't have an index, it is not the first column in the multiple column index or the index is non-unique.
                                                        *   If the value is PRI, it is a primary key or the primary key of multiple columns.
                                                        *   If the value is UNI, it is a unique index. (The unique index allows multiple NULL values but you can also set a NOT NULL constraint.)
                                                        *   If the value is MUL, it is the first column of the non-unique index that allows the given value to be displayed in the column several times. If the column composes a composite unique index, the value will be MUL. The combination of column values can be unique but the value of each column can appear several times.
Default                             VARCHAR         Default value defined in the column
Extra                               VARCHAR         Additional information available on the given column. For the column with **AUTO_INCREMENT** constraint, it shows the 'auto_increment'.
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    SHOW COLUMNS FROM athlete;
    
::

      Field                 Type                  Null       Key          Default               Extra
    ================================================================================================================
      'code'                'INTEGER'             'NO'       'PRI'        NULL                  'auto_increment'
      'name'                'VARCHAR(40)'         'NO'       ''           NULL                  ''
      'gender'              'CHAR(1)'             'YES'      ''           NULL                  ''
      'nation_code'         'CHAR(3)'             'YES'      ''           NULL                  ''
      'event'               'VARCHAR(30)'         'YES'      ''           NULL                  ''
     
.. code-block:: sql

    SHOW COLUMNS FROM athlete WHERE field LIKE '%c%';
    
::

      Field                 Type                  Null       Key          Default               Extra
    ================================================================================================================
      'code'                'INTEGER'             'NO'       'PRI'        NULL                  'auto_increment'
      'nation_code'         'CHAR(3)'             'YES'      ''           NULL                  ''
     
.. code-block:: sql

    SHOW COLUMNS FROM athlete  WHERE "type" = 'INTEGER' and "key"='PRI' AND extra='auto_increment';
    
::

      Field                 Type                  Null       Key          Default               Extra
    ================================================================================================================
      'code'                'INTEGER'             'NO'       'PRI'        NULL                  'auto_increment'
    
.. code-block:: sql

    SHOW COLUMNS FROM athlete WHERE field LIKE '%c%';
    
::

      Field                 Type                  Collation             Null      Key         Default               Extra
    ====================================================================================================================================
      'code'                'INTEGER'             NULL                  'NO'      'PRI'       NULL                  'auto_increment'
      'nation_code'         'CHAR(3)'             'iso88591_bin'        'YES'     ''          NULL                  ''

.. _show-index-statement:

SHOW INDEX
==========

It shows the index information. 

::

    SHOW {INDEX | INDEXES | KEYS } {FROM | IN} tbl_name;

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Table                               VARCHAR         Table name
Non_unique                          INTEGER         Unique or not
                                                        * 0: Duplicated value is not allowed
                                                        * 1: Duplicated value is allowed
Key_name                            VARCHAR         Index name
Seq_in_index                        INTEGER         Serial number of the column in the index. Starts from 1.
Column_name                         VARCHAR         Column name
Collation                           VARCHAR         Method of sorting columns in the index. 'A' means ascending and **NULL** means not sorted.
Cardinality                         INTEGER         The number of values measuring the unique values in the index. Higher cardinality increases the opportunity of using an index.
                                                    This value is updated every time **SHOW INDEX** is executed. Note that this is an approximate value.
Sub_part                            INTEGER         The number of bytes of the indexed characters if the columns are indexed partially. **NULL** if all columns are indexed.
Packed                                              Shows how keys are packed. If they are not packed, it will be **NULL**. Currently no support.
Null                                VARCHAR         YES if a column can include **NULL**, NO if not.
Index_type                          VARCHAR         Index to be used (currently, only the BTREE is supported.)
Func                                VARCHAR         A function which is used in a function-based index
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    SHOW INDEX IN athlete;
    
::

       Table     Non_unique   Key_name       Seq_in_index  Column_name    Collation     Cardinality   Sub_part  Packed   Null   Index_type  Func
    =============================================================================================================================================
     'athlete'     0      'pk_athlete_code'     1          'code'           'A'           6677         NULL     NULL    'NO'      'BTREE'   NULL
     
.. code-block:: sql

    CREATE TABLE tbl1 (i1 INTEGER , i2 INTEGER NOT NULL, i3 INTEGER UNIQUE, s1 VARCHAR(10), s2 VARCHAR(10), s3 VARCHAR(10) UNIQUE);
     
    CREATE INDEX i_tbl1_i1 ON tbl1 (i1 DESC);
    CREATE INDEX i_tbl1_s1 ON tbl1 (s1 (7));
    CREATE INDEX i_tbl1_i1_s1 ON tbl1 (i1, s1);
    CREATE UNIQUE INDEX i_tbl1_i2_s2 ON tbl1 (i2, s2);
     
    SHOW INDEXES FROM tbl1;
    
::

      Table  Non_unique  Key_name       Seq_in_index  Column_name  Collation  Cardinality     Sub_part  Packed  Null    Index_type   Func
    =====================================================================================================================================
      'tbl1'          1  'i_tbl1_i1'               1  'i1'         'D'                  0         NULL  NULL    'YES'   'BTREE'      NULL
      'tbl1'          1  'i_tbl1_i1_s1'            1  'i1'         'A'                  0         NULL  NULL    'YES'   'BTREE'      NULL
      'tbl1'          1  'i_tbl1_i1_s1'            2  's1'         'A'                  0         NULL  NULL    'YES'   'BTREE'      NULL
      'tbl1'          0  'i_tbl1_i2_s2'            1  'i2'         'A'                  0         NULL  NULL    'NO'    'BTREE'      NULL
      'tbl1'          0  'i_tbl1_i2_s2'            2  's2'         'A'                  0         NULL  NULL    'YES'   'BTREE'      NULL
      'tbl1'          1  'i_tbl1_s1'               1  's1'         'A'                  0            7  NULL    'YES'   'BTREE'      NULL
      'tbl1'          0  'u_tbl1_i3'               1  'i3'         'A'                  0         NULL  NULL    'YES'   'BTREE'      NULL
      'tbl1'          0  'u_tbl1_s3'               1  's3'         'A'                  0         NULL  NULL    'YES'   'BTREE'      NULL

.. _show-collation-statement:
 
SHOW COLLATION
==============

It lists collations supported by the database. If LIKE clause is present, it indicates which collation names to match. 

::

    SHOW COLLATION [ LIKE 'pattern' ];

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Collation                           VARCHAR         Collation name
Charset                             CHAR(1)         Charset name
Id                                  INTEGER         Collation ID
Built_in                            CHAR(1)         Built-in collation or not. Built-in collations are impossible to add or remove because they are hard-coded.
Expansions                          CHAR(1)         Collation with expansion or not. For details, see :ref:`expansion`.
Strength                            CHAR(1)         The number of levels to be considered in comparison, and the character order can be different by this number. 
                                                    For details, see :ref:`collation-properties`.
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    SHOW COLLATION;

::

      Collation             Charset                        Id  Built_in              Expansions            Strength
    ===========================================================================================================================
      'euckr_bin'           'euckr'                         8  'Yes'                 'No'                  'Not applicable'
      'iso88591_bin'        'iso88591'                      0  'Yes'                 'No'                  'Not applicable'
      'iso88591_en_ci'      'iso88591'                      3  'Yes'                 'No'                  'Not applicable'
      'iso88591_en_cs'      'iso88591'                      2  'Yes'                 'No'                  'Not applicable'
      'utf8_bin'            'utf8'                          1  'Yes'                 'No'                  'Not applicable'
      'utf8_de_exp'         'utf8'                         76  'No'                  'Yes'                 'Tertiary'
      'utf8_de_exp_ai_ci'   'utf8'                         72  'No'                  'Yes'                 'Primary'
      'utf8_en_ci'          'utf8'                          5  'Yes'                 'No'                  'Not applicable'
      'utf8_en_cs'          'utf8'                          4  'Yes'                 'No'                  'Not applicable'
      'utf8_es_cs'          'utf8'                         85  'No'                  'No'                  'Quaternary'
      'utf8_fr_exp_ab'      'utf8'                         94  'No'                  'Yes'                 'Tertiary'
      'utf8_gen'            'utf8'                         32  'No'                  'No'                  'Quaternary'
      'utf8_gen_ai_ci'      'utf8'                         37  'No'                  'No'                  'Primary'
      'utf8_gen_ci'         'utf8'                         44  'No'                  'No'                  'Secondary'
      'utf8_ja_exp'         'utf8'                        124  'No'                  'Yes'                 'Tertiary'
      'utf8_ja_exp_cbm'     'utf8'                        125  'No'                  'Yes'                 'Tertiary'
      'utf8_km_exp'         'utf8'                        132  'No'                  'Yes'                 'Quaternary'
      'utf8_ko_cs'          'utf8'                          7  'Yes'                 'No'                  'Not applicable'
      'utf8_ko_cs_uca'      'utf8'                        133  'No'                  'No'                  'Quaternary'
      'utf8_tr_cs'          'utf8'                          6  'Yes'                 'No'                  'Not applicable'
      'utf8_tr_cs_uca'      'utf8'                        205  'No'                  'No'                  'Quaternary'
      'utf8_vi_cs'          'utf8'                        221  'No'                  'No'                  'Quaternary'

.. code-block:: sql

    SHOW COLLATION LIKE '%_ko_%';
    
::

      Collation             Charset                        Id  Built_in              Expansions            Strength
    ===========================================================================================================================
      'utf8_ko_cs'          'utf8'                          7  'Yes'                 'No'                  'Not applicable'
      'utf8_ko_cs_uca'      'utf8'                        133  'No'                  'No'                  'Quaternary'

.. _show-grants-statement:

SHOW GRANTS
===========

It shows the permissions associated with the database user accounts. ::

    SHOW GRANTS FOR 'user';

The following shows the examples of this syntax.

.. code-block:: sql

    CREATE TABLE testgrant (id INT);
    CREATE USER user1;
    GRANT INSERT,SELECT ON testgrant TO user1;
     
    SHOW GRANTS FOR user1;
    
::

      Grants for USER1
    ======================
      'GRANT INSERT, SELECT ON testgrant TO USER1'

.. _show-create-table-statement:

SHOW CREATE TABLE
=================

When a table name is specified, It shows the **CREATE TABLE** statement of the table. ::

    SHOW CREATE TABLE table_name;
    
.. code-block:: sql

    SHOW CREATE TABLE nation;
     
::

      TABLE                 CREATE TABLE
    ============================================
      'nation'               'CREATE TABLE [nation] ([code] CHARACTER(3) NOT NULL, 
    [name] CHARACTER VARYING(40) NOT NULL, [continent] CHARACTER VARYING(10), 
    [capital] CHARACTER VARYING(30),  CONSTRAINT [pk_nation_code] PRIMARY KEY  ([code])) 
    COLLATE iso88591_bin'

**SHOW CREATE TABLE** statement does not display as the user's written syntax. For example, the comment that user wrote is not displayed, and table names and column names are always displayed as lower case letters.

.. _show-create-view-statement:

SHOW CREATE VIEW
================

It shows the corresponding **CREATE VIEW** statement if view name is specified. ::

    SHOW CREATE VIEW view_name;

The following shows the examples of this syntax.

.. code-block:: sql

    SHOW CREATE VIEW db_class;
     
::

      View              Create View
    ========================================
      'db_class'       'SELECT c.class_name, CAST(c.owner.name AS VARCHAR(255)), CASE c.class_type WHEN 0 THEN 'CLASS' WHEN 1 THEN 'VCLASS' ELSE
                       'UNKNOW' END, CASE WHEN MOD(c.is_system_class, 2) = 1 THEN 'YES' ELSE 'NO' END, CASE WHEN c.sub_classes IS NULL THEN 'NO'
                       ELSE NVL((SELECT 'YES' FROM _db_partition p WHERE p.class_of = c and p.pname IS NULL), 'NO') END, CASE WHEN
                       MOD(c.is_system_class / 8, 2) = 1 THEN 'YES' ELSE 'NO' END FROM _db_class c WHERE CURRENT_USER = 'DBA' OR {c.owner.name}
                       SUBSETEQ (  SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{t.g.name}), SET{})  FROM db_user u, TABLE(groups) AS t(g)  WHERE
                       u.name = CURRENT_USER) OR {c} SUBSETEQ (  SELECT SUM(SET{au.class_of})  FROM _db_auth au  WHERE {au.grantee.name} SUBSETEQ
                       (  SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{t.g.name}), SET{})  FROM db_user u, TABLE(groups) AS t(g)  WHERE u.name =
                       CURRENT_USER) AND  au.auth_type = 'SELECT')'

SHOW ACCESS STATUS 
================== 
  
**SHOW ACCESS STATUS** statement displays login information regarding database accounts. Only database's DBA account can use this statement.
  
:: 
  
    SHOW ACCESS STATUS [LIKE 'pattern' | WHERE expr] ; 

This statement displays the following columns.

=================== =========== =================================================================== 
Column name         Type        Description
=================== =========== =================================================================== 
user_name           VARCHAR(32) DB user's account
last_access_time    DATETIME    Last time that the database user accessed
last_access_host    VARCHAR(32) Lastly accessed host
program_name        VARCHAR(32) The name of client program(broker_cub_cas_1, csql ..) 
=================== =========== =================================================================== 
  
The following shows the result of running this statement.
  
.. code-block:: sql 
  
    SHOW ACCESS STATUS; 
  
:: 
  
      user_name last_access_time last_access_host program_name 
    ============================================================================= 
      'DBA' 08:19:31.000 PM 02/10/2014 127.0.0.1 'csql' 
      'PUBLIC' NULL NULL NULL

.. note::

    The above login information which **SHOW ACCESS STATUS** shows is initialized when the database is restarted, and this query is not replication in HA environment; therefore, each node shows the different result.

.. _show-exec-statistics-statement:

SHOW EXEC STATISTICS
====================

It shows statistics information of executing query.

*   To start collecting **@collect_exec_stats** statistics information, configure the value of session variable **@collect_exec_stats** to 1; to stop, configure it to 0.

*   It outputs the result of collecting statistics information.

    *   The **SHOW EXEC STATISTICS** statement outputs four part of data page statistics information; data_page_fetches, data_page_dirties, data_page_ioreads, and data_page_iowrites. The result columns consist of variable column (name of statistics name) and value column (value of statistics value). Once the **SHOW EXEC STATISTICS** statement is executed, the statistics information which has been accumulated is initialized.

    *   The **SHOW EXEC STATISTICS ALL** statement outputs all items of statistics information.

For details, see :ref:`statdump`.

::

    SHOW EXEC STATISTICS [ALL];

The following shows the examples of this syntax.

.. code-block:: sql

    -- set session variable @collect_exec_stats as 1 to start collecting the statistical information.
    SET @collect_exec_stats = 1;
    SELECT * FROM db_class;
     
    -- print the statistical information of the data pages.
    SHOW EXEC STATISTICS;
    
::

    variable                value
    ===============================
    'data_page_fetches'     332
    'data_page_dirties'     85
    'data_page_ioreads'     18
    'data_page_iowrites'    28
     
.. code-block:: sql

    SELECT * FROM db_index;
    
    -- print all of the statistical information.
    SHOW EXEC STATISTICS ALL;

::
    
    variable                                value
    ============================================
    'file_creates'                          0
    'file_removes'                          0
    'file_ioreads'                          6
    'file_iowrites'                         0
    'file_iosynches'                        0
    'data_page_fetches'                     548
    'data_page_dirties'                     34
    'data_page_ioreads'                     6
    'data_page_iowrites'                    0
    'data_page_victims'                     0
    'data_page_iowrites_for_replacement'    0
    'log_page_ioreads'                      0
    'log_page_iowrites'                     0
    'log_append_records'                    0
    'log_checkpoints'                       0
    'log_wals'                              0
    'page_locks_acquired'                   13
    'object_locks_acquired'                 9
    'page_locks_converted'                  0
    'object_locks_converted'                0
    'page_locks_re-requested'               0
    'object_locks_re-requested'             8
    'page_locks_waits'                      0
    'object_locks_waits'                    0
    'tran_commits'                          3
    'tran_rollbacks'                        0
    'tran_savepoints'                       0
    'tran_start_topops'                     6
    'tran_end_topops'                       6
    'tran_interrupts'                       0
    'btree_inserts'                         0
    'btree_deletes'                         0
    'btree_updates'                         0
    'btree_covered'                         0
    'btree_noncovered'                      2
    'btree_resumes'                         0
    'btree_multirange_optimization'         0
    'query_selects'                         4
    'query_inserts'                         0
    'query_deletes'                         0
    'query_updates'                         0
    'query_sscans'                          2
    'query_iscans'                          4
    'query_lscans'                          0
    'query_setscans'                        2
    'query_methscans'                       0
    'query_nljoins'                         2
    'query_mjoins'                          0
    'query_objfetches'                      0
    'network_requests'                      88
    'adaptive_flush_pages'                  0
    'adaptive_flush_log_pages'              0
    'adaptive_flush_max_pages'              0
    'network_requests'                      88
    'adaptive_flush_pages'                  0
    'adaptive_flush_log_pages'              0
    'adaptive_flush_max_pages'              0

SHOW VOLUME HEADER
==================

It shows the volume header of the specified volume in one row.

::

    SHOW VOLUME HEADER OF volume_id;
    
This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Volume_id                           INT             Volume identifier
Magic_symbol                        VARCHAR(100)    Magic value for for a volume file
Io_page_size                        INT             Size of DB volume
Purpose                             VARCHAR(32)     Volume purposes, purposes type: DATA, INDEX, GENERIC, TEMP TEMP, TEMP
Sector_size_in_pages                INT             Size of sector in pages
Num_total_sectors                   INT             Total number of sectors
Num_free_sectors                    INT             Number of free sectors
Hint_alloc_sector                   INT             Hint for next sector to be allocated
Num_total_pages                     INT             Total number of pages
Num_free_pages                      INT             Number of free pages
Sector_alloc_table_size_in_pages    INT             Size of sector allocation table in page
Sector_alloc_table_first_page       INT             First page of sector allocation table
Page_alloc_table_size_in_pages      INT             Size of page allocation table in page
Page_alloc_table_first_page         INT             First page of page allocation table
Last_system_page                    INT             Last system page
Creation_time                       DATETIME        Database creation time
Num_max_pages                       INT             max page count of this volume, this is not equal to the total_pages,if this volume is auto extended
Num_used_data_pages                 INT             allocated pages for DATA purpose
Num_used_index_pages                INT             allocated pages for INDEX purpose
Checkpoint_lsa                      VARCHAR(64)     Lowest log sequence address to start the recovery process of this volume
Boot_hfid                           VARCHAR(64)     System Heap file for booting purposes and multi volumes
Full_name                           VARCHAR(255)    The full path of volume
Next_vol_full_name                  VARCHAR(255)    The full path of next volume
Remarks                             VARCHAR(64)     
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- csql> ;line on
    SHOW VOLUME HEADER OF 0;
    
::

    <00001> Volume_id                       : 0
            Magic_symbol                    : 'MAGIC SYMBOL = CUBRID/Volume at disk location = 32'
            Io_page_size                    : 16384
            Purpose                         : 'Permanent GENERIC Volume'
            Sector_size_in_pages            : 10
            Num_total_sectors               : 640
            Num_free_sectors                : 550
            Hint_alloc_sector               : 94
            Num_total_pages                 : 6400
            Num_free_pages                  : 6025
            Sector_alloc_table_size_in_pages: 1
            Sector_alloc_table_first_page   : 1
            Page_alloc_table_size_in_pages  : 1
            Page_alloc_table_first_page     : 2
            Last_system_page                : 2
            Creation_time                   : 06:09:27.000 PM 02/27/2014
            Num_max_pages                   : 6400
            Num_used_data_pages             : 192
            Num_used_index_pages            : 180
            Checkpoint_lsa                  : '(0|12832)'
            Boot_hfid                       : '(0|41|50)'
            Full_name                       : '/home1/brightest/CUBRID/databases/demodb/demodb'
            Next_vol_full_name              : ''
            Remarks                         : ''

SHOW LOG HEADER
===============

It shows the header information of an active log file.

::

    SHOW LOG HEADER [OF file_name];
    
If you omit **OF** *file_name*, it shows the header information of a memory; if you include **OF** *file_name*, it shows the header information of *file_name*.

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Magic_symbol                        VARCHAR(32)     Magic value for log file
Magic_symbol_location               INT             Magic symbol location from log page
Creation_time                       DATETIME        Database creation time
Release                             VARCHAR(32)     CUBRID Release version
Compatibility_disk_version          VARCHAR(32)     Compatibility of the database against the current release of CUBRID
Db_page_size                        INT             Size of pages in the database
Log_page_size                       INT             Size of log pages in the database
Shutdown                            INT             Was the log shutdown
Next_trans_id                       INT             Next transaction identifier
Num_avg_trans                       INT             Number of average transactions
Num_avg_locks                       INT             Average number of object locks
Num_active_log_pages                INT             Number of pages in the active log portion
Db_charset                          INT             charset number of database
First_active_log_page               BIGINT          Logical pageid at physical location 1 in active log
Current_append                      VARCHAR(64)     Current append location
Checkpoint                          VARCHAR(64)     Lowest log sequence address to start the recovery process
Next_archive_page_id                BIGINT          Next logical page to archive
Active_physical_page_id             INT             Physical location of logical page to archive
Next_archive_num                    INT             Next log archive number
Last_archive_num_for_syscrashes     INT             Last log archive needed for system crashes
Last_deleted_archive_num            INT             Last deleted archive number
Backup_lsa_level0                   VARCHAR(64)     LSA of backup level 0
Backup_lsa_level1                   VARCHAR(64)     LSA of backup level 1
Backup_lsa_level2                   VARCHAR(64)     LSA of backup level 2
Log_prefix                          VARCHAR(256)    Log prefix name
Has_logging_been_skipped            INT             Whether or not logging skipped
Perm_status                         VARCHAR(64)     Reserved for future expansion
Backup_info_level0                  VARCHAR(128)    detail information of backup level 0. currently only backup start-time is used
Backup_info_level1                  VARCHAR(128)    detail information of backup level 1. currently only backup start-time is used
Backup_info_level2                  VARCHAR(128)    detail information of backup level 2. currently only backup start-time is used
Ha_server_state                     VARCHAR(32)     current ha state, one of flowing value: na, idle, active, to-be-active, standby, to-be-standby, maintenance, dead
Ha_file                             VARCHAR(32)     ha replication status, one of following value: clear, archived, sync
Eof_lsa                             VARCHAR(64)     
Smallest_lsa_at_last_checkpoint     VARCHAR(64)     
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- csql> ;line on
    SHOW LOG HEADER;
    
::

    <00001> Volume_id                      : -2
            Magic_symbol                   : 'CUBRID/LogActive'
            Magic_symbol_location          : 16
            Creation_time                  : 04:42:28.000 PM 12/11/2013
            Release                        : '10.0.0'
            Compatibility_disk_version     : '9.2'
            Db_page_size                   : 16384
            Log_page_size                  : 16384
            Shutdown                       : 0
            Next_trans_id                  : 607149
            Num_avg_trans                  : 0
            Num_avg_locks                  : 0
            Num_active_log_pages           : 1279
            Db_charset                     : 5
            First_active_log_page          : 66508
            Current_append                 : '(66637|14672)'
            Checkpoint                     : '(66637|14280)'
            Next_archive_page_id           : 66456
            Active_physical_page_id        : 1228
            Next_archive_num               : 52
            Last_archive_num_for_syscrashes: 52
            Last_deleted_archive_num       : -1
            Backup_lsa_level0              : '(66636|5240)'
            Backup_lsa_level1              : '(-1|-1)'
            Backup_lsa_level2              : '(-1|-1)'
            Log_prefix                     : 'demodb'
            Has_logging_been_skipped       : 0
            Perm_status                    : 'LOG_PSTAT_CLEAR'
            Backup_info_level0             : 'time: Mon Dec 16 14:33:17 2013'
            Backup_info_level1             : 'time: none'
            Backup_info_level2             : 'time: none'
            Ha_server_state                : 'idle'
            Ha_file                        : 'unknown'
            Eof_lsa                        : '(66637|14672)'
            Smallest_lsa_at_last_checkpoint: '(66637|14280)'

.. code-block:: sql
            
    SHOW LOG HEADER OF 'demodb_lgat';

::

    <00001> Volume_id                      : -2
            Magic_symbol                   : 'CUBRID/LogActive'
            Magic_symbol_location          : 16
            Creation_time                  : 04:42:28.000 PM 12/11/2013
            Release                        : '10.0.0'
            Compatibility_disk_version     : '9.2'
            Db_page_size                   : 16384
            Log_page_size                  : 16384
            Shutdown                       : 0
            Next_trans_id                  : 607146
            Num_avg_trans                  : 0
            Num_avg_locks                  : 0
            Num_active_log_pages           : 1279
            Db_charset                     : 5
            First_active_log_page          : 66508
            Current_append                 : '(66637|14280)'
            Checkpoint                     : '(66637|14280)'
            Next_archive_page_id           : 66456
            Active_physical_page_id        : 1228
            Next_archive_num               : 52
            Last_archive_num_for_syscrashes: 52
            Last_deleted_archive_num       : -1
            Backup_lsa_level0              : '(66636|5240)'
            Backup_lsa_level1              : '(-1|-1)'
            Backup_lsa_level2              : '(-1|-1)'
            Log_prefix                     : 'demodb'
            Has_logging_been_skipped       : 0
            Perm_status                    : 'LOG_PSTAT_CLEAR'
            Backup_info_level0             : 'time: Mon Dec 16 14:33:17 2013'
            Backup_info_level1             : 'time: none'
            Backup_info_level2             : 'time: none'
            Ha_server_state                : 'idle'
            Ha_file                        : 'unknown'
            Eof_lsa                        : '(66637|14280)'
            Smallest_lsa_at_last_checkpoint: '(66637|14280)'

SHOW ARCHIVE LOG HEADER
=======================

It shows the header information of an archive log file.

::

    SHOW ARCHIVE LOG HEADER OF file_name;

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Volume_id                           INT             Identifier of log volume
Magic_symbol                        VARCHAR(32)     Magic value for file/magic Unix utility
Magic_symbol_location               INT             Magic symbol location from log page
Creation_time                       DATETIME        Database creation time
Next_trans_id                       BIGINT          Next transaction identifier
Num_pages                           INT             Number of pages in the archive log
First_page_id                       BIGINT          Logical page id at physical location 1 in archive log
Archive_num                         INT             The archive log number
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- csql> ;line on
    SHOW ARCHIVE LOG HEADER OF 'demodb_lgar001';
    
::

    <00001> Volume_id            : -20
            Magic_symbol         : 'CUBRID/LogArchive'
            Magic_symbol_location: 16
            Creation_time        : 04:42:28.000 PM 12/11/2013
            Next_trans_id        : 22695
            Num_pages            : 1278
            First_page_id        : 1278
            Archive_num          : 1

SHOW HEAP HEADER
================

It shows shows the header page of the table. 

::

    SHOW [ALL] HEAP HEADER OF table_name;

*   ALL: If "ALL" is given in syntax in the partition table, the basic table and its partitioned tables are shown.

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Class_name                          VARCHAR(256)    Table name
Class_oid                           VARCHAR(64)     Format: (volid|pageid|slotid)
Volume_id                           INT             Volume identifier where the file reside
File_id                             INT             File identifier
Header_page_id                      INT             First page identifier (the header page)
Overflow_vfid                       VARCHAR(64)     Overflow file identifier (if any)
Next_vpid                           VARCHAR(64)     Next page (i.e., the 2nd page of heap file)
Unfill_space                        INT             Stop inserting when page has run below this. leave it for updates
Estimates_num_pages                 BIGINT          Estimation of number of heap pages.
Estimates_num_recs                  BIGINT          Estimation of number of objects in heap
Estimates_avg_rec_len               INT             Estimation total length of records
Estimates_num_high_best             INT             Number of pages in the best array that we believe have at least HEAP_DROP_FREE_SPACE. When this number goes to zero and
                                                    there are at least other HEAP_NUM_BEST_SPACESTATS best pages, we look for them
Estimates_num_others_high_best      INT             Total of other believed known best pages, which are not included in the best array and 
                                                    we believe they have at least HEAP_DROP_FREE_SPACE
Estimates_head                      INT             Head of best circular array
Estimates_best_list                 VARCHAR(512)    Format: '((best[0].vpid.volid|best[0].vpid.pageid), best[0].freespace), ... , ((best[9].vpid.volid|best[9].vpid.pageid), best[9].freespace)'
Estimates_num_second_best           INT             Number of second best hints. The hints are in "second_best" array. They are used when finding new best pages.
Estimates_head_second_best          INT             Index of head of second best hints. A new second best hint will be stored on this index.
Estimates_num_substitutions         INT             Number of page substitutions. This will be used to insert a new second best page into second best hints.
Estimates_second_best_list          VARCHAR(512)    Format: '(second_best[0].vpid.volid|second_best[0].vpid.pageid), ... , (second_best[9].vpid.volid|second_best[9].vpid.pageid)'
Estimates_last_vpid                 VARCHAR(64)     Format: '(volid|pageid)'
Estimates_full_search_vpid          VARCHAR(64)     Format: '(volid|pageid)'
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- csql> ;line on
    SHOW HEAP HEADER OF athlete;
    
::

    <00001> Class_name                    : 'athlete'
            Class_oid                     : '(0|463|8)'
            Volume_id                     : 0
            File_id                       : 147
            Header_page_id                : 590
            Overflow_vfid                 : '(-1|-1)'
            Next_vpid                     : '(0|591)'
            Unfill_space                  : 1635
            Estimates_num_pages           : 27
            Estimates_num_recs            : 6677
            Estimates_avg_rec_len         : 54
            Estimates_num_high_best       : 1
            Estimates_num_others_high_best: 0
            Estimates_head                : 0
            Estimates_best_list           : '((0|826), 14516), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1),0), ((-1|-1), 0)'
            Estimates_num_second_best     : 0
            Estimates_head_second_best    : 0
            Estimates_tail_second_best    : 0
            Estimates_num_substitutions   : 0
            Estimates_second_best_list    : '(-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1)'
            Estimates_last_vpid           : '(0|826)'
            Estimates_full_search_vpid    : '(0|590)'

.. code-block:: sql

    CREATE TABLE participant2 (
        host_year INT,
        nation CHAR(3),
        gold INT,
        silver INT,
        bronze INT
    )
    PARTITION BY RANGE (host_year) (
        PARTITION before_2000 VALUES LESS THAN (2000),
        PARTITION before_2008 VALUES LESS THAN (2008)
    );
    
.. code-block:: sql
    
    SHOW ALL HEAP HEADER OF participant2;
    
::
    
    <00001> Class_name                    : 'participant2'
            Class_oid                     : '(0|467|6)'
            Volume_id                     : 0
            File_id                       : 374
            Header_page_id                : 940
            Overflow_vfid                 : '(-1|-1)'
            Next_vpid                     : '(-1|-1)'
            Unfill_space                  : 1635
            Estimates_num_pages           : 1
            Estimates_num_recs            : 0
            Estimates_avg_rec_len         : 0
            Estimates_num_high_best       : 1
            Estimates_num_others_high_best: 0
            Estimates_head                : 1
            Estimates_best_list           : '((0|940), 16308), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0)'
            Estimates_num_second_best     : 0
            Estimates_head_second_best    : 0
            Estimates_tail_second_best    : 0
            Estimates_num_substitutions   : 0
            Estimates_second_best_list    : '(-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1)'
            Estimates_last_vpid           : '(0|940)'
            Estimates_full_search_vpid    : '(0|940)'
    <00002> Class_name                    : 'participant2__p__before_2000'
            Class_oid                     : '(0|467|7)'
            Volume_id                     : 0
            File_id                       : 376
            Header_page_id                : 950
            Overflow_vfid                 : '(-1|-1)'
            Next_vpid                     : '(-1|-1)'
            Unfill_space                  : 1635
            Estimates_num_pages           : 1
            Estimates_num_recs            : 0
            Estimates_avg_rec_len         : 0
            Estimates_num_high_best       : 1
            Estimates_num_others_high_best: 0
            Estimates_head                : 1
            Estimates_best_list           : '((0|950), 16308), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0)'
            Estimates_num_second_best     : 0
            Estimates_head_second_best    : 0
            Estimates_tail_second_best    : 0
            Estimates_num_substitutions   : 0
            Estimates_second_best_list    : '(-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1)'
            Estimates_last_vpid           : '(0|950)'
            Estimates_full_search_vpid    : '(0|950)'
    <00003> Class_name                    : 'participant2__p__before_2008'
            Class_oid                     : '(0|467|8)'
            Volume_id                     : 0
            File_id                       : 378
            Header_page_id                : 960
            Overflow_vfid                 : '(-1|-1)'
            Next_vpid                     : '(-1|-1)'
            Unfill_space                  : 1635
            Estimates_num_pages           : 1
            Estimates_num_recs            : 0
            Estimates_avg_rec_len         : 0
            Estimates_num_high_best       : 1
            Estimates_num_others_high_best: 0
            Estimates_head                : 1
            Estimates_best_list           : '((0|960), 16308), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0)'
            Estimates_num_second_best     : 0
            Estimates_head_second_best    : 0
            Estimates_tail_second_best    : 0
            Estimates_num_substitutions   : 0
            Estimates_second_best_list    : '(-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1)'
            Estimates_last_vpid           : '(0|960)'
            Estimates_full_search_vpid    : '(0|960)'

.. code-block:: sql

    SHOW HEAP HEADER OF participant2__p__before_2008;
    
::

    <00001> Class_name                    : 'participant2__p__before_2008'
            Class_oid                     : '(0|467|8)'
            Volume_id                     : 0
            File_id                       : 378
            Header_page_id                : 960
            Overflow_vfid                 : '(-1|-1)'
            Next_vpid                     : '(-1|-1)'
            Unfill_space                  : 1635
            Estimates_num_pages           : 1
            Estimates_num_recs            : 0
            Estimates_avg_rec_len         : 0
            Estimates_num_high_best       : 1
            Estimates_num_others_high_best: 0
            Estimates_head                : 1
            Estimates_best_list           : '((0|960), 16308), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0), ((-1|-1), 0)'
            Estimates_num_second_best     : 0
            Estimates_head_second_best    : 0
            Estimates_tail_second_best    : 0
            Estimates_num_substitutions   : 0
            Estimates_second_best_list    : '(-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1), (-1|-1)'
            Estimates_last_vpid           : '(0|960)'
            Estimates_full_search_vpid    : '(0|960)'

SHOW HEAP CAPACITY
==================

It shows the capacity of the table. 

::

    SHOW [ALL] HEAP CAPACITY OF table_name;

*   ALL: If "all" is given in syntax, the basic table and its partition table(s) is shown.

This query has the following columns:

=========================================== =============== ===============================================================================================================================
Column name                                 Type            Description
=========================================== =============== ===============================================================================================================================
Table_name                                  VARCHAR(256)    Table name
Class_oid                                   VARCHAR(64)     Heap file descriptor
Volume_id                                   INT             Volume identifier where the file reside
File_id                                     INT             File identifier
Header_page_id                              INT             First page identifier (the header page)
Num_recs                                    BIGINT          Total Number of objects
Num_relocated_recs                          BIGINT          Number of relocated records
Num_overflowed_recs                         BIGINT          Number of big records
Num_pages                                   BIGINT          Total number of heap pages
Avg_rec_len                                 INT             Average object length
Avg_free_space_per_page                     INT             Average free space per page
Avg_free_space_per_page_without_last_page   INT             Average free space per page without taking in consideration last page
Avg_overhead_per_page                       INT             Average overhead per page
Repr_id                                     INT             Currently cached catalog column info
Num_total_attrs                             INT             total number of columns
Num_fixed_width_attrs                       INT             Number of the fixed width columns
Num_variable_width_attrs                    INT             Number of variable width columns
Num_shared_attrs                            INT             Number of shared columns
Num_class_attrs                             INT             Number of table columns
Total_size_fixed_width_attrs                INT             Total size of the fixed width columns
=========================================== =============== ===============================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- csql> ;line on
    SHOW HEAP CAPACITY OF athlete;
    
::

    <00001> Table_name                              : 'athlete'
            Class_oid                               : '(0|463|8)'
            Volume_id                               : 0
            File_id                                 : 147
            Header_page_id                          : 590
            Num_recs                                : 6677
            Num_relocated_recs                      : 0
            Num_overflowed_recs                     : 0
            Num_pages                               : 27
            Avg_rec_len                             : 53
            Avg_free_space_per_page                 : 2139
            Avg_free_space_per_page_except_last_page: 1663
            Avg_overhead_per_page                   : 993
            Repr_id                                 : 1
            Num_total_attrs                         : 5
            Num_fixed_width_attrs                   : 3
            Num_variable_width_attrs                : 2
            Num_shared_attrs                        : 0
            Num_class_attrs                         : 0
            Total_size_fixed_width_attrs            : 8
    
.. code-block:: sql

    SHOW ALL HEAP CAPACITY OF participant2;
    
::
    
    <00001> Table_name                              : 'participant2'
            Class_oid                               : '(0|467|6)'
            Volume_id                               : 0
            File_id                                 : 374
            Header_page_id                          : 940
            Num_recs                                : 0
            Num_relocated_recs                      : 0
            Num_overflowed_recs                     : 0
            Num_pages                               : 1
            Avg_rec_len                             : 0
            Avg_free_space_per_page                 : 16016
            Avg_free_space_per_page_except_last_page: 0
            Avg_overhead_per_page                   : 4
            Repr_id                                 : 1
            Num_total_attrs                         : 5
            Num_fixed_width_attrs                   : 5
            Num_variable_width_attrs                : 0
            Num_shared_attrs                        : 0
            Num_class_attrs                         : 0
            Total_size_fixed_width_attrs            : 20
    <00002> Table_name                              : 'participant2__p__before_2000'
            Class_oid                               : '(0|467|7)'
            Volume_id                               : 0
            File_id                                 : 376
            Header_page_id                          : 950
            Num_recs                                : 0
            Num_relocated_recs                      : 0
            Num_overflowed_recs                     : 0
            Num_pages                               : 1
            Avg_rec_len                             : 0
            Avg_free_space_per_page                 : 16016
            Avg_free_space_per_page_except_last_page: 0
            Avg_overhead_per_page                   : 4
            Repr_id                                 : 1
            Num_total_attrs                         : 5
            Num_fixed_width_attrs                   : 5
            Num_variable_width_attrs                : 0
            Num_shared_attrs                        : 0
            Num_class_attrs                         : 0
            Total_size_fixed_width_attrs            : 20
    <00003> Table_name                              : 'participant2__p__before_2008'
            Class_oid                               : '(0|467|8)'
            Volume_id                               : 0
            File_id                                 : 378
            Header_page_id                          : 960
            Num_recs                                : 0
            Num_relocated_recs                      : 0
            Num_overflowed_recs                     : 0
            Num_pages                               : 1
            Avg_rec_len                             : 0
            Avg_free_space_per_page                 : 16016
            Avg_free_space_per_page_except_last_page: 0
            Avg_overhead_per_page                   : 4
            Repr_id                                 : 1
            Num_total_attrs                         : 5
            Num_fixed_width_attrs                   : 5
            Num_variable_width_attrs                : 0
            Num_shared_attrs                        : 0
            Num_class_attrs                         : 0
            Total_size_fixed_width_attrs            : 20

SHOW SLOTTED PAGE HEADER
========================

It shows the header information of specified slotted page.

::

    SHOW SLOTTED PAGE HEADER { WHERE|OF } VOLUME = volume_num AND PAGE = page_num;

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Volume_id                           INT             Volume id of the page
Page_id                             INT             page id of the page
Num_slots                           INT             Number of allocated slots for the page
Num_records                         INT             Number of records on page
Anchor_type                         VARCHAR(32)     One of flowing: ANCHORED, ANCHORED_DONT_REUSE_SLOTS, UNANCHORED_ANY_SEQUENCE, UNANCHORED_KEEP_SEQUENCE
Alignment                           VARCHAR(8)      Alignment for records, one of flowing: CHAR, SHORT, INT, DOUBLE
Total_free_area                     INT             Total free space on page
Contiguous_free_area                INT             Contiguous free space on page
Free_space_offset                   INT             Byte offset from the beginning of the page to the first free byte area on the page
Need_update_best_hint               INT             True if saving is need for recovery (undo)
Is_saving                           INT             True if we should update best pages hint for this page.
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- csql> ;line on
    SHOW SLOTTED PAGE HEADER OF VOLUME=0 AND PAGE=140;

::

    <00001> Volume_id            : 0
            Page_id              : 140
            Num_slots            : 3
            Num_records          : 3
            Anchor_type          : 'ANCHORED_DONT_REUSE_SLOTS'
            Alignment            : 'INT'
            Total_free_area      : 15880
            Contiguous_free_area : 15880
            Free_space_offset    : 460
            Need_update_best_hint: 1
            Is_saving            : 0

SHOW SLOTTED PAGE SLOTS
========================

It shows the information of all slots in the specified slotted page.

::

    SHOW SLOTTED PAGE SLOTS { WHERE|OF } VOLUME = volume_num AND PAGE = page_num;
    
This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Volume_id                           INT             Volume id of the page
Page_id                             INT             Page id of the page
Slot_id                             INT             The slot id
Offset                              INT             Byte offset from the beginning of the page to the beginning of the record
Type                                VARCHAR(32)     Record type, one of flowing: REC_UNKNOWN, REC_ASSIGN_ADDRESS, REC_HOME, REC_NEWHOME, REC_RELOCATION, REC_BIGONE, REC_MARKDELETED, REC_DELETED_WILL_REUSE
Length                              INT             Length of record
Waste                               INT             Whether or not wasted
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- csql> ;line on
    SHOW SLOTTED PAGE HEADER OF VOLUME=0 AND PAGE=140;

::

    <00001> Volume_id: 0
            Page_id  : 140
            Slot_id  : 0
            Offset   : 40
            Type     : 'HOME'
            Length   : 292
            Waste    : 0
    <00002> Volume_id: 0
            Page_id  : 140
            Slot_id  : 1
            Offset   : 332
            Type     : 'HOME'
            Length   : 64
            Waste    : 0
    <00003> Volume_id: 0
            Page_id  : 140
            Slot_id  : 2
            Offset   : 396
            Type     : 'HOME'
            Length   : 64
            Waste    : 0

SHOW INDEX HEADER
=================

It shows the index header page of the index of the table.

::

    SHOW INDEX HEADER OF table_name.index_name;

If ALL keyword is used and an index name is omitted, it shows the entire headers of the indexes of the table.

::

    SHOW ALL INDEXES HEADER OF table_name;

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Table_name                          VARCHAR(256)    Table name
Index_name                          VARCHAR(256)    Index name
Btid                                VARCHAR(64)     BTID (volid|fileid|root_pageid)
Prev_vpid                           VARCHAR(32)     VPID (volid|pageid)
Next_vpid                           VARCHAR(32)     VPID (volid|pageid)
Node_type                           VARCHAR(16)     'LEAF' or 'NON_LEAF'
Max_key_len                         INT             Maximum key length for the subtree
Num_oids                            INT             Number of OIDs stored in the Btree
Num_nulls                           INT             Number of NULLs (they aren't stored)
Num_keys                            INT             Number of unique keys in the Btree
Topclass_oid                        VARCHAR(64)     Topclass oid or NULL OID (non unique index)(volid|pageid|slotid)
Unique                              INT             Unique or non-unique
Overflow_vfid                       VARCHAR(32)     VFID (volid|fileid)
Key_type                            VARCHAR(32)     Type name
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- Prepare test environment
    CREATE TABLE tbl1(a INT, b VARCHAR(5));
    CREATE INDEX index_a ON tbl1(a ASC);
    CREATE INDEX index_b ON tbl1(b ASC);  

..  code-block:: sql
    
    -- csql> ;line on
    SHOW INDEX HEADER OF tbl1.index_a;
    
::

    <00001> Table_name   : 'tbl1'
            Index_name   : 'index_a'
            Btid         : '(0|378|950)'
            Prev_vpid    : '(-1|-1)'
            Next_vpid    : '(-1|-1)'
            Node_type    : 'LEAF'
            Max_key_len  : 0
            Num_oids     : -1
            Num_nulls    : -1
            Num_keys     : -1
            Topclass_oid : '(0|469|4)'
            Unique       : 0
            Overflow_vfid: '(-1|-1)'
            Key_type     : 'integer'

.. code-block:: sql
      
    SHOW ALL INDEXES HEADER OF tbl1;
    
::

    <00001> Table_name   : 'tbl1'
            Index_name   : 'index_a'
            Btid         : '(0|378|950)'
            Prev_vpid    : '(-1|-1)'
            Next_vpid    : '(-1|-1)'
            Node_type    : 'LEAF'
            Max_key_len  : 0
            Num_oids     : -1
            Num_nulls    : -1
            Num_keys     : -1
            Topclass_oid : '(0|469|4)'
            Unique       : 0
            Overflow_vfid: '(-1|-1)'
            Key_type     : 'integer'
    <00002> Table_name   : 'tbl1'
            Index_name   : 'index_b'
            Btid         : '(0|381|960)'
            Prev_vpid    : '(-1|-1)'
            Next_vpid    : '(-1|-1)'
            Node_type    : 'LEAF'
            Max_key_len  : 0
            Num_oids     : -1
            Num_nulls    : -1
            Num_keys     : -1
            Topclass_oid : '(0|469|4)'
            Unique       : 0
            Overflow_vfid: '(-1|-1)'
            Key_type     : 'character varying'

SHOW INDEX CAPACITY
===================

It shows the index capacity of the index of the table.

::

    SHOW INDEX CAPACITY OF table_name.index_name;

If ALL keyword is used and an index name is omitted, it shows the entire capacity of the indexes of the table.

::

    SHOW ALL INDEXES CAPACITY OF table_name;

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Table_name                          VARCHAR(256)    Table name
Index_name                          VARCHAR(256)    Index name
Btid                                VARCHAR(64)     BTID (volid|fileid|root_pageid)
Num_distinct_key                    INT             Distinct key count (in leaf pages)
Total_value                         INT             Total number of values stored in tree
Avg_num_value_per_key               INT             Average number of values (OIDs) per key
Num_leaf_page                       INT             Leaf page count
Num_non_leaf_page                   INT             NonLeaf page count
Num_total_page                      INT             Total page count
Height                              INT             Height of the tree
Avg_key_len                         INT             Average key length
Avg_rec_len                         INT             Average page record length
Total_space                         VARCHAR(64)     Total space occupied by index
Total_used_space                    VARCHAR(64)     Total used space in index
Total_free_space                    VARCHAR(64)     Total free space in index
Avg_num_page_key                    INT             Average page key count (in leaf pages)
Avg_page_free_space                 VARCHAR(64)     Average page free space
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- Prepare test environment
    CREATE TABLE tbl1(a INT, b VARCHAR(5));
    CREATE INDEX index_a ON tbl1(a ASC);
    CREATE INDEX index_b ON tbl1(b ASC);  

..  code-block:: sql

    -- csql> ;line on
    SHOW INDEX CAPACITY OF tbl1.index_a;
    
::
    
    <00001> Table_name           : 'tbl1'
            Index_name           : 'index_a'
            Btid                 : '(0|378|950)'
            Num_distinct_key     : 0
            Total_value          : 0
            Avg_num_value_per_key: 0
            Num_leaf_page        : 1
            Num_non_leaf_page    : 0
            Num_total_page       : 1
            Height               : 1
            Avg_key_len          : 0
            Avg_rec_len          : 0
            Total_space          : '16.0K'
            Total_used_space     : '116.0B'
            Total_free_space     : '15.9K'
            Avg_num_page_key     : 0
            Avg_page_free_space  : '15.9K'


.. code-block:: sql
      
    SHOW ALL INDEXES CAPACITY OF tbl1;
    
::

    <00001> Table_name           : 'tbl1'
            Index_name           : 'index_a'
            Btid                 : '(0|378|950)'
            Num_distinct_key     : 0
            Total_value          : 0
            Avg_num_value_per_key: 0
            Num_leaf_page        : 1
            Num_non_leaf_page    : 0
            Num_total_page       : 1
            Height               : 1
            Avg_key_len          : 0
            Avg_rec_len          : 0
            Total_space          : '16.0K'
            Total_used_space     : '116.0B'
            Total_free_space     : '15.9K'
            Avg_num_page_key     : 0
            Avg_page_free_space  : '15.9K'
    <00002> Table_name           : 'tbl1'
            Index_name           : 'index_b'
            Btid                 : '(0|381|960)'
            Num_distinct_key     : 0
            Total_value          : 0
            Avg_num_value_per_key: 0
            Num_leaf_page        : 1
            Num_non_leaf_page    : 0
            Num_total_page       : 1
            Height               : 1
            Avg_key_len          : 0
            Avg_rec_len          : 0
            Total_space          : '16.0K'
            Total_used_space     : '120.0B'
            Total_free_space     : '15.9K'
            Avg_num_page_key     : 0
            Avg_page_free_space  : '15.9K'
