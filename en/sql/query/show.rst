
:meta-keywords: show statement, show tables, show columns, show index, show collation, show timezones, show grants

:tocdepth: 3

****
SHOW
****

.. contents::

DESC, DESCRIBE
==============

It shows the column information of a table, and it's like a **SHOW COLUMNS** statement. For more details, see :ref:`show-columns-statement`.

::

    DESC [schema_name.]table_name;
    DESCRIBE [schema_name.]table_name;
    
EXPLAIN
=======

It shows the column information of a table, and it's like a **SHOW COLUMNS** statement. For more details, see :ref:`show-columns-statement`.

::

    EXPLAIN [schema_name.]table_name;

.. _show-tables-statement:

SHOW TABLES
===========

It shows the list of all table names within a database. The name of the result column will be *tables_in_<database name>* and it will have one column. If you use the **LIKE** clause, you can search the table names matching this and if you use the **WHERE** clause, you can search table names with more general terms. **SHOW FULL TABLES** displays the *owner* column and the *table_type* column together. The *owner* column has the owner name as a value. *table_type* columns have **BASE TABLE** values for tables and **VIEW** for views.

::

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

      Tables_in_demodb      Owner                 Table_type
    ==================================================================
      'athlete'             'PUBLIC'              'BASE TABLE'
      'code'                'PUBLIC'              'BASE TABLE'
      'event'               'PUBLIC'              'BASE TABLE'
      'game'                'PUBLIC'              'BASE TABLE'
      'history'             'PUBLIC'              'BASE TABLE'
      'nation'              'PUBLIC'              'BASE TABLE'
      'olympic'             'PUBLIC'              'BASE TABLE'
      'participant'         'PUBLIC'              'BASE TABLE'
      'record'              'PUBLIC'              'BASE TABLE'
      'stadium'             'PUBLIC'              'BASE TABLE'

.. code-block:: sql

    SHOW FULL TABLES LIKE '%c%';

::

      Tables_in_demodb      Owner                 Table_type
    ==================================================================
      'code'                'PUBLIC'              'BASE TABLE'
      'olympic'             'PUBLIC'              'BASE TABLE'
      'participant'         'PUBLIC'              'BASE TABLE'
      'record'              'PUBLIC'              'BASE TABLE'

.. code-block:: sql

    SHOW FULL TABLES WHERE table_type = 'BASE TABLE' and TABLES_IN_demodb LIKE '%co%';
    
::

      Tables_in_demodb      Owner                 Table_type
    ==================================================================
      'code'                'PUBLIC'              'BASE TABLE'
      'record'              'PUBLIC'              'BASE TABLE'

.. _show-columns-statement:

SHOW COLUMNS
============

It shows the column information of a table. You can use the **LIKE** clause to search the column names matching it. If you use the **WHERE** clause, you can search column names with more general terms like, "General Considerations for All **SHOW** Statements.".

::

        SHOW [FULL] COLUMNS (FROM | IN) [schema_name.]table_name [LIKE 'pattern' | WHERE expr];

If a **FULL** keyword is used, it shows the additional information, **collation** and **comment**.

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

    SHOW FULL COLUMNS FROM athlete WHERE field LIKE '%c%';
    
::

	  Field                 Type                  Collation             Null                  Key                   Default               Extra                 Comment             
	================================================================================================================================================================================
	  'code'                'INTEGER'             NULL                  'NO'                  'PRI'                 NULL                  'auto_increment'      NULL                
	  'nation_code'         'CHAR(3)'             'iso88591_bin'        'YES'                 ''                    NULL                  ''                    NULL                

.. _show-index-statement:

SHOW INDEX
==========

It shows the index information. 

::

    SHOW (INDEX | INDEXES | KEYS) (FROM | IN) [schema_name.]table_name;

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
Comment                             VARCHAR         Comment to describe the index
Visible                             VARCHAR         Shows the Visibility of an index (YES/NO)
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    SHOW INDEX IN athlete;
    
::

      Table             Non_unique  Key_name           Seq_in_index  Column_name  Collation  Cardinality  Sub_part  Packed  Null  Index_type  func  Comment  Visible
    =================================================================================================================================================================
      'public.athlete'           0  'pk_athlete_code'             1  'code'       'A'               6677      NULL  NULL    'NO'  'BTREE'     NULL  NULL     'YES'

.. code-block:: sql

    CREATE TABLE tbl1 (i1 INTEGER , i2 INTEGER NOT NULL, i3 INTEGER UNIQUE, s1 VARCHAR(10), s2 VARCHAR(10), s3 VARCHAR(10) UNIQUE);
     
    CREATE INDEX i_tbl1_i1 ON tbl1 (i1 DESC);
    CREATE INDEX i_tbl1_s1 ON tbl1 (s1 (7));
    CREATE INDEX i_tbl1_i1_s1 ON tbl1 (i1, s1);
    CREATE UNIQUE INDEX i_tbl1_i2_s2 ON tbl1 (i2, s2);
    
    ALTER INDEX i_tbl1_s1 ON tbl1 INVISIBLE;
     
    SHOW INDEXES FROM tbl1;
  
::

      Table          Non_unique  Key_name        Seq_in_index  Column_name  Collation  Cardinality  Sub_part  Packed  Null   Index_type  Func  Comment  Visible
    =============================================================================================================================================================
      'public.tbl1'           1  'i_tbl1_i1'                1  'i1'         'D'                  0      NULL  NULL    'YES'  'BTREE'     NULL  NULL     'YES'
      'public.tbl1'           1  'i_tbl1_i1_s1'             1  'i1'         'A'                  0      NULL  NULL    'YES'  'BTREE'     NULL  NULL     'YES'
      'public.tbl1'           1  'i_tbl1_i1_s1'             2  's1'         'A'                  0      NULL  NULL    'YES'  'BTREE'     NULL  NULL     'YES'
      'public.tbl1'           0  'i_tbl1_i2_s2'             1  'i2'         'A'                  0      NULL  NULL    'NO'   'BTREE'     NULL  NULL     'YES'
      'public.tbl1'           0  'i_tbl1_i2_s2'             2  's2'         'A'                  0      NULL  NULL    'YES'  'BTREE'     NULL  NULL     'YES'
      'public.tbl1'           1  'i_tbl1_s1'                1  's1'         'A'                  0         7  NULL    'YES'  'BTREE'     NULL  NULL     'NO'
      'public.tbl1'           0  'u_tbl1_i3'                1  'i3'         'A'                  0      NULL  NULL    'YES'  'BTREE'     NULL  NULL     'YES'
      'public.tbl1'           0  'u_tbl1_s3'                1  's3'         'A'                  0      NULL  NULL    'YES'  'BTREE'     NULL  NULL     'YES'

.. _show-collation-statement:
 
SHOW COLLATION
==============

It lists collations supported by the database. If LIKE clause is present, it indicates which collation names to match. 

::

    SHOW COLLATION [LIKE 'pattern'];

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

SHOW TIMEZONES
==============

It shows the timezone information which the current CUBRID supports.

::

    SHOW [FULL] TIMEZONES [LIKE 'pattern'];

If FULL is not specified, one column which has timezone's region names is displayed. The name of this column is timezone_region.

If FULL is specified, four columns which have timezone information are displayed.

If LIKE clause is present, it indicates which timezone_region names to match.

=================== =============== ===================================================
Column name         Type            Description
=================== =============== ===================================================
timezone_region     VARCHAR(32)     Timezone region name
region_offset       VARCHAR(32)     Offset of timezone (daylight saving time is not considered)
dst_offset          VARCHAR(32)     Offset of daylight saving time (applied to timezone region) which is currently considered
dst_abbreviation    VARCHAR(32)     An abbreviation of the daylight saving time which is currently applied for the region
=================== =============== ===================================================

The information listed for the second, third and fourth columns is for the current date and time.

If a timezone region doesn't have daylight saving time rules at all then the dst_offset and dst_abbreviation columns will contain NULL values. 

If at the current date, there aren't daylight saving time rules that apply, then dst_offset will be set to 0 and dst_abbreviation will be the empty string.

The LIKE condition without the WHERE condition is applied on the first column. The WHERE condition may be used to filter the output.

.. code-block:: sql

    SHOW TIMEZONES;

::

    timezone_region
    ======================
    'Africa/Abidjan'
    'Africa/Accra'
    'Africa/Addis_Ababa'
    'Africa/Algiers'
    'Africa/Asmara'
    'Africa/Asmera'
    ...
    'US/Michigan'
    'US/Mountain'
    'US/Pacific'
    'US/Samoa'
    'UTC'
    'Universal'
    'W-SU'
    'WET'
    'Zulu'

.. code-block:: sql

    SHOW FULL TIMEZONES;

::

    timezone_region       region_offset         dst_offset            dst_abbreviation
    ===================================================================================
    'Africa/Abidjan'      '+00:00'              '+00:00'              'GMT'
    'Africa/Accra'        '+00:00'              NULL                  NULL
    'Africa/Addis_Ababa'  '+03:00'              '+00:00'              'EAT'
    'Africa/Algiers'      '+01:00'              '+00:00'              'CET'
    'Africa/Asmara'       '+03:00'              '+00:00'              'EAT'
    'Africa/Asmera'       '+03:00'              '+00:00'              'EAT'
    ...
    'US/Michigan'         '-05:00'              '+00:00'              'EST'
    'US/Mountain'         '-07:00'              '+00:00'              'MST'
    'US/Pacific'          '-08:00'              '+00:00'              'PST'
    'US/Samoa'            '-11:00'              '+00:00'              'SST'
    'UTC'                 '+00:00'              '+00:00'              'UTC'
    'Universal'           '+00:00'              '+00:00'              'UTC'
    'W-SU'                '+04:00'              '+00:00'              'MSK'
    'WET'                 '+00:00'              '+00:00'              'WET'
    'Zulu'                '+00:00'              '+00:00'              'UTC'


.. code-block:: sql

    SHOW FULL TIMEZONES LIKE '%Paris%';

::
    
   timezone_region       region_offset         dst_offset            dst_abbreviation
   ========================================================================================
   'Europe/Paris'        '+01:00'              '+00:00'              'CET'

	
.. _show-grants-statement:

SHOW GRANTS
===========

It shows the permissions associated with the database user accounts.

::

    SHOW GRANTS FOR user_name;

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

    SHOW CREATE TABLE [schema_name.]table_name;
    
.. code-block:: sql

    SHOW CREATE TABLE nation;
     
::

      TABLE                 CREATE TABLE
    ============================================
      'public.nation'       'CREATE TABLE [nation] ([code] CHARACTER(3) NOT NULL, [name] CHARACTER VARYING(40) NOT NULL, [continent] CHARACTER VARYING(10), [capital] CHARACTER VARYING(30), CONSTRAINT [pk_nation_code] PRIMARY KEY  ([code])) DONT_REUSE_OID, COLLATE iso88591_bin'

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

      View                  Create View
    ============================================
      'db_class'            'SELECT [c].[class_name], CAST([c].[owner].[name] AS VARCHAR(255)), CASE [c].[class_type] WHEN 0 THEN 'CLASS' WHEN 1 THEN 'VCLASS' ELSE 'UNKNOW' END, CASE WHEN MOD([c].[is_system_class], 2) = 1 THEN 'YES' ELSE 'NO' END, CASE [c].[tde_algorithm] WHEN 0 THEN 'NONE' WHEN 1 THEN 'AES' WHEN 2 THEN 'ARIA' END, CASE WHEN [c].[sub_classes] IS NULL THEN 'NO' ELSE NVL((SELECT 'YES' FROM [_db_partition] [p] WHERE [p].[class_of] = [c] and [p].[pname] IS NULL), 'NO') END, CASE WHEN MOD([c].[is_system_class] / 8, 2) = 1 THEN 'YES' ELSE 'NO' END, [coll].[coll_name], [c].[comment] FROM [_db_class] [c], [_db_collation] [coll] WHERE [c].[collation_id] = [coll].[coll_id] AND (CURRENT_USER = 'DBA' OR {[c].[owner].[name]} SUBSETEQ(SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{[t].[g].[name]}), SET{}) FROM [db_user] [u], TABLE([groups]) AS [t]([g]) WHERE [u].[name] = CURRENT_USER) OR {[c]} SUBSETEQ ( SELECT SUM(SET{[au].[class_of]}) FROM [_db_auth] [au] WHERE {[au].[grantee].[name]} SUBSETEQ ( SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{[t].[g].[name]}), SET{}) FROM [db_user] [u], TABLE([groups]) AS [t]([g]) WHERE [u].[name] = CURRENT_USER) AND [au].[auth_type] = 'SELECT'))'

SHOW ACCESS STATUS 
================== 
  
**SHOW ACCESS STATUS** statement displays login information regarding database accounts. Only database's DBA account can use this statement.
  
:: 
  
    SHOW ACCESS STATUS [LIKE 'pattern' | WHERE expr]; 

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
    'log_page_ioreads'                      0
    'log_page_iowrites'                     0
    'log_append_records'                    0
    'log_archives'                          0
    'log_start_checkpoints'                 0
    'log_end_checkpoints'                   0
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
    'query_holdable_cursors'                0
    'sort_io_pages'                         0
    'sort_data_pages'                       0
    'network_requests'                      88
    'adaptive_flush_pages'                  0
    'adaptive_flush_log_pages'              0
    'adaptive_flush_max_pages'              0
    'prior_lsa_list_size'                   0
    'prior_lsa_list_maxed'                  0
    'prior_lsa_list_removed'                0
    'heap_stats_bestspace_entries'          0
    'heap_stats_bestspace_maxed'            0

Diagnostics
===========

SHOW VOLUME HEADER
------------------

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
Purpose                             VARCHAR(32)     Volume purposes, 'Permanent data purpose' or 'Temporary data purpose'
Type                                VARCHAR(32)     Volume type, 'Permanent Volume' or 'Temporary Volume'
Sector_size_in_pages                INT             Size of sector in pages
Num_total_sectors                   INT             Total number of sectors
Num_free_sectors                    INT             Number of free sectors
Num_max_sectors                     INT             Maximum number of sectors
Hint_alloc_sector                   INT             Hint for next sector to be allocated
Sector_alloc_table_size_in_pages    INT             Size of sector allocation table in page
Sector_alloc_table_first_page       INT             First page of sector allocation table
Page_alloc_table_size_in_pages      INT             Size of page allocation table in page
Page_alloc_table_first_page         INT             First page of page allocation table
Last_system_page                    INT             Last system page
Creation_time                       DATETIME        Database creation time
Db_charset                          INT             Charset number of database
Checkpoint_lsa                      VARCHAR(64)     Lowest log sequence address to start the recovery process of this volume
Boot_hfid                           VARCHAR(64)     System Heap file for booting purposes and multi volumes
Full_name                           VARCHAR(255)    The full path of volume
Next_volume_id                      INT             Next volume identifier
Next_vol_full_name                  VARCHAR(255)    The full path of next volume
Remarks                             VARCHAR(64)     Volume remarks
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- csql> ;line on
    SHOW VOLUME HEADER OF 0;
    
::

    <00001> Volume_id                       : 0
            Magic_symbol                    : 'MAGIC SYMBOL = CUBRID/Volume at disk location = 32'
            Io_page_size                    : 16384
            Purpose                         : 'Permanent data purpose'
            Type                            : 'Permanent Volume'
            Sector_size_in_pages            : 64
            Num_total_sectors               : 512
            Num_free_sectors                : 459
            Num_max_sectors                 : 512
            Hint_alloc_sector               : 0
            Sector_alloc_table_size_in_pages: 1
            Sector_alloc_table_first_page   : 1
            Last_system_page                : 1
            Creation_time                   : 09:46:41.000 PM 05/23/2017
            Db_charset                      : 3
            Checkpoint_lsa                  : '(0|12832)'
            Boot_hfid                       : '(0|41|50)'
            Full_name                       : '/home1/brightest/CUBRID/databases/demodb/demodb'
            Next_volume_id                  : -1
            Next_vol_full_name              : ''
            Remarks                         : ''

SHOW LOG HEADER
---------------

It shows the header information of an active log file.

::

    SHOW LOG HEADER [OF file_name];
    
If you omit **OF** *file_name*, it shows the header information of a memory; if you include **OF** *file_name*, it shows the header information of *file_name*.

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Volume_id                           INT             Volume identifier
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
Db_charset                          INT             Charset number of database
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
Eof_lsa                             VARCHAR(64)     EOF LSA
Smallest_lsa_at_last_checkpoint     VARCHAR(64)     The smallest LSA of the last checkpoint, can be NULL LSA
Next_mvcc_id                        BIGINT          The next MVCCID will be used for the next transaction
Mvcc_op_log_lsa                     VARCHAR(32)     The LSA used to link log entries for MVCC operation
Last_block_oldest_mvcc_id           BIGINT          Used to find the oldest MVCCID in a block of log data, can be NULL
Last_block_newest_mvcc_id           BIGINT          Used to find the newest MVCCID in a block of log data, can be NULL
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- csql> ;line on
    SHOW LOG HEADER;
    
::

    <00001> Volume_id                      : -2
            Magic_symbol                   : 'CUBRID/LogActive'
            Magic_symbol_location          : 16
            Creation_time                  : 09:46:41.000 PM 05/23/2017
            Release                        : '10.0.0'
            Compatibility_disk_version     : '10'
            Db_page_size                   : 16384
            Log_page_size                  : 16384
            Shutdown                       : 0
            Next_trans_id                  : 17
            Num_avg_trans                  : 3
            Num_avg_locks                  : 30
            Num_active_log_pages           : 1279
            Db_charset                     : 3
            First_active_log_page          : 0
            Current_append                 : '(102|5776)'
            Checkpoint                     : '(101|7936)'
            Next_archive_page_id           : 0
            Active_physical_page_id        : 1
            Next_archive_num               : 0
            Last_archive_num_for_syscrashes: -1
            Last_deleted_archive_num       : -1
            Backup_lsa_level0              : '(-1|-1)'
            Backup_lsa_level1              : '(-1|-1)'
            Backup_lsa_level2              : '(-1|-1)'
            Log_prefix                     : 'mvccdb'
            Has_logging_been_skipped       : 0
            Perm_status                    : 'LOG_PSTAT_CLEAR'
            Backup_info_level0             : 'time: N/A'
            Backup_info_level1             : 'time: N/A'
            Backup_info_level2             : 'time: N/A'
            Ha_server_state                : 'idle'
            Ha_file                        : 'UNKNOWN'
            Eof_lsa                        : '(102|5776)'
            Smallest_lsa_at_last_checkpoint: '(101|7936)'
            Next_mvcc_id                   : 6
            Mvcc_op_log_lsa                : '(102|5488)'
            Last_block_oldest_mvcc_id      : 4
            Last_block_newest_mvcc_id      : 5



.. code-block:: sql
            
    SHOW LOG HEADER OF 'demodb_lgat';

::

    <00001> Volume_id                      : -2
            Magic_symbol                   : 'CUBRID/LogActive'
            Magic_symbol_location          : 16
            Creation_time                  : 09:46:41.000 PM 05/23/2017
            Release                        : '10.0.0'
            Compatibility_disk_version     : '10'
            Db_page_size                   : 16384
            Log_page_size                  : 16384
            Shutdown                       : 0
            Next_trans_id                  : 15
            Num_avg_trans                  : 3
            Num_avg_locks                  : 30
            Num_active_log_pages           : 1279
            Db_charset                     : 3
            First_active_log_page          : 0
            Current_append                 : '(101|8016)'
            Checkpoint                     : '(101|7936)'
            Next_archive_page_id           : 0
            Active_physical_page_id        : 1
            Next_archive_num               : 0
            Last_archive_num_for_syscrashes: -1
            Last_deleted_archive_num       : -1
            Backup_lsa_level0              : '(-1|-1)'
            Backup_lsa_level1              : '(-1|-1)'
            Backup_lsa_level2              : '(-1|-1)'
            Log_prefix                     : 'mvccdb'
            Has_logging_been_skipped       : 0
            Perm_status                    : 'LOG_PSTAT_CLEAR'
            Backup_info_level0             : 'time: N/A'
            Backup_info_level1             : 'time: N/A'
            Backup_info_level2             : 'time: N/A'
            Ha_server_state                : 'idle'
            Ha_file                        : 'UNKNOWN'
            Eof_lsa                        : '(101|8016)'
            Smallest_lsa_at_last_checkpoint: '(101|7936)'
            Next_mvcc_id                   : 4
            Mvcc_op_log_lsa                : '(-1|-1)'
            Last_block_oldest_mvcc_id      : NULL
            Last_block_newest_mvcc_id      : NULL


SHOW ARCHIVE LOG HEADER
-----------------------

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
----------------

It shows shows the header page of the table. 

::

    SHOW  [ALL] HEAP HEADER OF [schema_name.]table_name;

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
------------------

It shows the capacity of the table. 

::

    SHOW [ALL] HEAP CAPACITY OF [schema_name.]table_name;

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
------------------------

It shows the header information of specified slotted page.

::

    SHOW SLOTTED PAGE HEADER (WHERE | OF) VOLUME = volume_num AND PAGE = page_num;

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
Flags                               INT             Flag value of the page
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
            Flags                : 0

SHOW SLOTTED PAGE SLOTS
------------------------

It shows the information of all slots in the specified slotted page.

::

    SHOW SLOTTED PAGE SLOTS (WHERE | OF) VOLUME = volume_num AND PAGE = page_num;
    
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
-----------------

It shows the index header page of the index of the table.

::

    SHOW INDEX HEADER OF [schema_name.]table_name.index_name;

If ALL keyword is used and an index name is omitted, it shows the entire headers of the indexes of the table.

::

    SHOW ALL INDEXES HEADER OF [schema_name.]table_name;

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Table_name                          VARCHAR(256)    Table name
Index_name                          VARCHAR(256)    Index name
Btid                                VARCHAR(64)     BTID (volid|fileid|root_pageid)
Node_level                          INT             Node level (1 for LEAF, 2 or more for NON_LEAF)
Max_key_len                         INT             Maximum key length for the subtree
Num_oids                            INT             Number of OIDs stored in the Btree
Num_nulls                           INT             Number of NULLs (they aren't stored)
Num_keys                            INT             Number of unique keys in the Btree
Topclass_oid                        VARCHAR(64)     Topclass oid or NULL OID (non unique index)(volid|pageid|slotid)
Unique                              INT             Unique or non-unique
Overflow_vfid                       VARCHAR(32)     VFID (volid|fileid)
Key_type                            VARCHAR(256)    Type name
Columns                             VARCHAR(256)    the list of columns which consists of the index
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- Prepare test environment
    CREATE TABLE tbl1(a INT, b VARCHAR(5));
    CREATE INDEX index_ab ON tbl1(a ASC, b DESC);

..  code-block:: sql
    
    -- csql> ;line on
    SHOW INDEX HEADER OF tbl1.index_ab;
    
::

    <00001> Table_name   : 'tbl1'
            Index_name   : 'index_ab'
            Btid         : '(0|378|950)'
            Node_type    : 'LEAF'
            Max_key_len  : 0
            Num_oids     : -1
            Num_nulls    : -1
            Num_keys     : -1
            Topclass_oid : '(0|469|4)'
            Unique       : 0
            Overflow_vfid: '(-1|-1)'
            Key_type     : 'midxkey(integer,character varying(5))'
            Columns      : 'a,b DESC'

SHOW INDEX CAPACITY
-------------------

It shows the index capacity of the index of the table.

::

    SHOW INDEX CAPACITY OF [schema_name.]table_name.index_name;

If ALL keyword is used and an index name is omitted, it shows the entire capacity of the indexes of the table.

::

    SHOW ALL INDEXES CAPACITY OF [schema_name.]table_name;

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Table_name                          VARCHAR(256)    Table name
Index_name                          VARCHAR(256)    Index name
Btid                                VARCHAR(64)     BTID (volid|fileid|root_pageid)
Num_distinct_key                    INT             Distinct key count (in leaf pages)
Total_value                         INT             Total number of values stored in tree
Deduplicate_distinct_key            INT             Deduplicated distinct key count (in leaf pages)
Num_fence_key                       INT             Number of fence keys
Avg_num_value_per_key               INT             Average number of values (OIDs) per key
Avg_num_value_per_deduplicate_key   INT             Average number of values (OIDs) per deduplicated key
Num_leaf_page                       INT             Leaf page count
Num_non_leaf_page                   INT             NonLeaf page count
Num_ovf_page                        INT             Leaf's overflow page count
Num_total_page                      INT             Total page count
Height                              INT             Height of the tree
Avg_key_len                         INT             Average key length
Avg_rec_len                         INT             Average page record length
Total_space                         VARCHAR(64)     Total space occupied by index
Total_used_space_non_ovf            VARCHAR(64)     Total space used in allocated pages (excluded leaf's overflow pages)
Total_free_space_non_ovf            VARCHAR(64)     Total space unused in allocated pages (excluded leaf's overflow pages)
Total_used_space_ovf                VARCHAR(64)     Total space used in pages allocated to leaf's overflow
Total_free_space_ovf                VARCHAR(64)     Total space unused in pages allocated to leaf's overflow
Avg_num_key_per_page_non_ovf        INT             Average page key count in leaf pages
Avg_free_space_per_page_non_ovf     VARCHAR(64)     Average page free space
Avg_num_ovf_page_per_key            INT             Average page key count in leaf pages
Avg_free_space_per_page_ovf         VARCHAR(64)     Average page free space in leaf's overflow pages
Max_num_ovf_page_a_key              INT             Maximum number of leaf's overflow pages for one key
=================================== =============== ======================================================================================================================================

.. note::

    Fence key is a virtual key added to a leaf node to help operate the B-tree index.


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

    <00001> Table_name                       : 'dba.tbl1'
            Index_name                       : 'index_a'
            Btid                             : '(0|4160|4161)'
            Num_distinct_key                 : 0
            Total_value                      : 0
            Deduplicate_distinct_key         : 0
            Num_fence_key                    : 0
            Avg_num_value_per_key            : 0
            Avg_num_value_per_deduplicate_key: 0
            Num_leaf_page                    : 1
            Num_non_leaf_page                : 0
            Num_ovf_page                     : 0
            Num_total_page                   : 1
            Height                           : 1
            Avg_key_len                      : 0
            Avg_rec_len                      : 0
            Total_space                      : '16.0K'
            Total_used_space_non_ovf         : '120.0B'
            Total_free_space_non_ovf         : '15.8K'
            Total_used_space_ovf             : '0.0B'
            Total_free_space_ovf             : '0.0B'
            Avg_num_key_per_page_non_ovf     : 0
            Avg_free_space_per_page_non_ovf  : '15.8K'
            Avg_num_ovf_page_per_key         : 0
            Avg_free_space_per_page_ovf      : '0.0B'
            Max_num_ovf_page_a_key           : 0

.. code-block:: sql
      
    SHOW ALL INDEXES CAPACITY OF tbl1;
    
::

    <00001> Table_name                       : 'dba.tbl1'
            Index_name                       : 'index_a'
            Btid                             : '(0|4160|4161)'
            Num_distinct_key                 : 0
            Total_value                      : 0
            Deduplicate_distinct_key         : 0
            Num_fence_key                    : 0
            Avg_num_value_per_key            : 0
            Avg_num_value_per_deduplicate_key: 0
            Num_leaf_page                    : 1
            Num_non_leaf_page                : 0
            Num_ovf_page                     : 0
            Num_total_page                   : 1
            Height                           : 1
            Avg_key_len                      : 0
            Avg_rec_len                      : 0
            Total_space                      : '16.0K'
            Total_used_space_non_ovf         : '120.0B'
            Total_free_space_non_ovf         : '15.8K'
            Total_used_space_ovf             : '0.0B'
            Total_free_space_ovf             : '0.0B'
            Avg_num_key_per_page_non_ovf     : 0
            Avg_free_space_per_page_non_ovf  : '15.8K'
            Avg_num_ovf_page_per_key         : 0
            Avg_free_space_per_page_ovf      : '0.0B'
            Max_num_ovf_page_a_key           : 0
    <00002> Table_name                       : 'dba.tbl1'
            Index_name                       : 'index_b'
            Btid                             : '(0|4224|4225)'
            Num_distinct_key                 : 0
            Total_value                      : 0
            Deduplicate_distinct_key         : 0
            Num_fence_key                    : 0
            Avg_num_value_per_key            : 0
            Avg_num_value_per_deduplicate_key: 0
            Num_leaf_page                    : 1
            Num_non_leaf_page                : 0
            Num_ovf_page                     : 0
            Num_total_page                   : 1
            Height                           : 1
            Avg_key_len                      : 0
            Avg_rec_len                      : 0
            Total_space                      : '16.0K'
            Total_used_space_non_ovf         : '124.0B'
            Total_free_space_non_ovf         : '15.8K'
            Total_used_space_ovf             : '0.0B'
            Total_free_space_ovf             : '0.0B'
            Avg_num_key_per_page_non_ovf     : 0
            Avg_free_space_per_page_non_ovf  : '15.8K'
            Avg_num_ovf_page_per_key         : 0
            Avg_free_space_per_page_ovf      : '0.0B'
            Max_num_ovf_page_a_key           : 0

SHOW CRITICAL SECTIONS
----------------------

Total critical section (hereafter CS) information of a database is shown.

.. code-block:: sql

    SHOW CRITICAL SECTIONS;

This query has the following columns:

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Index                               INT             The index of CS
Name                                VARCHAR(32)     The name of CS
Num_holders                         VARCHAR(16)     The number of CS holders. This has one of these values: 'N readers', '1 writer', 'none'
Num_waiting_readers                 INT             The number of waiting readers
Num_waiting_writers                 INT             The number of waiting writers
Owner_thread_index                  INT             The thread index of CS owner writer, NULL if no owner
Owner_tran_index                    INT             Transaction index of CS owner writer, NULL if no owner
Total_enter_count                   BIGINT          Total count of enterers
Total_waiter_count                  BIGINT          Total count of waiters   
Waiting_promoter_thread_index       INT             The thread index of waiting promoter, NULL if no waiting promoter
Max_waiting_msecs                   NUMERIC(10,3)   Maximum waiting time (millisecond)
Total_waiting_msecs                 NUMERIC(10,3)   Total waiting time (millisecond)
=================================== =============== ======================================================================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    SHOW CRITICAL SECTIONS;

::

    Index  Name                       Num_holders           Num_waiting_readers  Num_waiting_writers  Owner_thread_index  Owner_tran_index     Total_enter_count    Total_waiter_count  Waiting_promoter_thread_index  Max_waiting_msecs     Total_waiting_msecs 
    ============================================================================================================================================================================================================================================================
        0  'ER_LOG_FILE'              'none'                                  0                    0                NULL              NULL                   217                     0                           NULL  0.000                 0.000               
        1  'ER_MSG_CACHE'             'none'                                  0                    0                NULL              NULL                     0                     0                           NULL  0.000                 0.000               
        2  'WFG'                      'none'                                  0                    0                NULL              NULL                     0                     0                           NULL  0.000                 0.000               
        3  'LOG'                      'none'                                  0                    0                NULL              NULL                    11                     0                           NULL  0.000                 0.000               
        4  'LOCATOR_CLASSNAME_TABLE'  'none'                                  0                    0                NULL              NULL                    33                     0                           NULL  0.000                 0.000          
        5  'QPROC_QUERY_TABLE'        'none'                                  0                    0                NULL              NULL                     3                     0                           NULL  0.000                 0.000               
        6  'QPROC_LIST_CACHE'         'none'                                  0                    0                NULL              NULL                     1                     0                           NULL  0.000                 0.000               
        7   'DISK_CHECK'              'none'                                  0                    0                NULL              NULL                     3                     0                           NULL  0.000                 0.000               
        8  'CNV_FMT_LEXER'            'none'                                  0                    0                NULL              NULL                     0                     0                           NULL  0.000                 0.000               
        9  'HEAP_CHNGUESS'            'none'                                  0                    0                NULL              NULL                    10                     0                           NULL  0.000                 0.000               
        10  'TRAN_TABLE'              'none'                                  0                    0                NULL              NULL                     7                     0                           NULL  0.000                 0.000               
        11  'CT_OID_TABLE'            'none'                                  0                    0                NULL              NULL                     0                     0                           NULL  0.000                 0.000               
        12  'HA_SERVER_STATE'         'none'                                  0                    0                NULL              NULL                     2                     0                           NULL  0.000                 0.000               
        13  'COMPACTDB_ONE_INSTANCE'  'none'                                  0                    0                NULL              NULL                     0                     0                           NULL  0.000                 0.000           
        14  'ACL'                     'none'                                  0                    0                NULL              NULL                     0                     0                           NULL  0.000                 0.000                    
        15  'PARTITION_CACHE'         'none'                                  0                    0                NULL              NULL                     1                     0                           NULL  0.000                 0.000               
        16  'EVENT_LOG_FILE'          'none'                                  0                    0                NULL              NULL                     0                     0                           NULL  0.000                 0.000               
        17  'LOG_ARCHIVE'             'none'                                  0                    0                NULL              NULL                     0                     0                           NULL  0.000                 0.000               
        18  'ACCESS_STATUS'           'none'                                  0                    0                NULL              NULL                     1                     0                           NULL  0.000                 0.000               

SHOW TRANSACTION TABLES
-----------------------

It shows internal information of transaction descriptors which is internal data structure to manage each transaction. It only shows valid transactions and the result may not be a consistent snapshot of a transaction descriptor.

.. code-block:: sql

    SHOW { TRAN | TRANSACTION } TABLES [ WHERE expr ];

This query has the following columns:

======================== =============== ==============================================================================================================================================================
Column name              Type            Description
======================== =============== ==============================================================================================================================================================
Tran_index               INT             Index on the transaction table or NULL for unassigned transaction descriptor slot
Tran_id                  INT             Transaction Identifier
Is_loose_end             INT             0 for Ordinary transactions, 1 for loose-end transactions
State                    VARCHAR(64)     State of the transaction. Either one of the followings:
                                         'TRAN_RECOVERY', 'TRAN_ACTIVE', 'TRAN_UNACTIVE_COMMITTED', 'TRAN_UNACTIVE_WILL_COMMIT', 'TRAN_UNACTIVE_COMMITTED_WITH_POSTPONE', 
                                         'TRAN_UNACTIVE_ABORTED', 'TRAN_UNACTIVE_UNILATERALLY_ABORTED', 'TRAN_UNACTIVE_2PC_PREPARE', 'TRAN_UNACTIVE_2PC_COLLECTING_PARTICIPANT_VOTES',
                                         'TRAN_UNACTIVE_2PC_ABORT_DECISION', 'TRAN_UNACTIVE_2PC_COMMIT_DECISION', 'TRAN_UNACTIVE_COMMITTED_INFORMING_PARTICIPANTS', 
                                         'TRAN_UNACTIVE_ABORTED_INFORMING_PARTICIPANTS','TRAN_STATE_UNKNOWN'
Isolation                VARCHAR(64)     Isolation level of the transaction. Either one of the followings: 'SERIALIZABLE', 'REPEATABLE READ', 'COMMITTED READ', 'TRAN_UNKNOWN_ISOLATION'
Wait_msecs               INT             Wait until this number of milliseconds for locks.
Head_lsa                 VARCHAR(64)     First log address of transaction.
Tail_lsa                 VARCHAR(64)     Last log record address of transaction.
Undo_next_lsa            VARCHAR(64)     Next log record address of transaction for UNDO purposes.
Postpone_next_lsa        VARCHAR(64)     Next address of a postpone record to be executed.
Savepoint_lsa            VARCHAR(64)     Address of last save-point.
Topop_lsa                VARCHAR(64)     Address of last top operation.
Tail_top_result_lsa      VARCHAR(64)     Address of last partial abort/commit.
Client_id                INT             Unique identifier of client application bind to transaction.
Client_type              VARCHAR(40)     Type of the client. Either one of the followings: 'SYSTEM_INTERNAL', 'DEFAULT', 'CSQL', 'READ_ONLY_CSQL', 'BROKER', 'READ_ONLY_BROKER', 'SLAVE_ONLY_BROKER',
                                         'ADMIN_UTILITY', 'ADMIN_CSQL', 'LOG_COPIER', 'LOG_APPLIER', 'RW_BROKER_REPLICA_ONLY', 'RO_BROKER_REPLICA_ONLY', 'SO_BROKER_REPLICA_ONLY', 
                                         'ADMIN_CSQL_WOS', 'UNKNOWN'
Client_info              VARCHAR(256)    General information of client application.
Client_db_user           VARCHAR(40)     Current login database account from client application.
Client_program           VARCHAR(256)    Program name of client application.
Client_login_user        VARCHAR(16)     Current login user of OS which running the client application.
Client_host              VARCHAR(64)     Host name of client application.
Client_pid               INT             Process id of client application.
Topop_depth              INT             Depth of nested top operation.
Num_unique_btrees        INT             Number of unique btrees contained in unique_stat_info array.
Max_unique_btrees        INT             Size of unique_stat_info_array.
Interrupt                INT             The flag of whether or not interrupt current transaction. 0 for No, 1 for Yes.
Num_transient_classnames INT             Number of transient classnames by this transaction.
Repl_max_records         INT             Capacity of replication record array.
Repl_records             VARCHAR(20)     Replication record buffer array, display address pointer as 0x12345678 or NULL for 0x00000000.
Repl_current_index       INT             Current position of replication record in the array.
Repl_append_index        INT             Current position of appended record in the array.
Repl_flush_marked_index  INT             Index of flush marked replication record at first.
Repl_insert_lsa          VARCHAR(64)     Insert Replication target LSA.
Repl_update_lsa          VARCHAR(64)     Update Replication target LSA.
First_save_entry         VARCHAR(20)     First save entry for the transaction, display address pointer as 0x12345678 or NULL for 0x00000000.
Tran_unique_stats        VARCHAR(20)     Local statistical info for multiple row. display address pointer as 0x12345678 or NULL for 0x00000000.
Modified_class_list      VARCHAR(20)     List of dirty classes, display address pointer as 0x12345678 or NULL for 0x00000000.
Num_temp_files           INT             Number of temporary files.
Waiting_for_res          VARCHAR(20)     Waiting resource. Just display address pointer as 0x12345678 or NULL for 0x00000000.
Has_deadlock_priority    INT             Whether or not have deadlock priority. 0 for No, 1 for Yes.
Suppress_replication     INT             Suppress writing replication logs when flag is set.
Query_timeout            DATETIME        A query should be executed before query_timeout time or NULL for waiting until query complete.
Query_start_time         DATETIME        Current query start time or NULL for query completed.
Tran_start_time          DATETIME        Current transaction start time or NULL for transaction completed.
Xasl_id                  VARCHAR(64)     vpid:(volid|pageid),vfid:(volid|pageid) or NULL for query completed.
Disable_modifications    INT             Disable modification if greater than zero.
Abort_reason             VARCHAR(40)     Reason of transaction aborted. Either one of the followings: 'NORMAL', 'ABORT_DUE_TO_DEADLOCK', 'ABORT_DUE_ROLLBACK_ON_ESCALATION'
======================== =============== ==============================================================================================================================================================

The following shows the examples of the statement.

.. code-block:: sql

    SHOW TRAN TABLES WHERE CLIENT_TYPE = 'CSQL';

::

        === <Result of SELECT Command in Line 1> ===

        <00001> Tran_index              : 1
                Tran_id                 : 58
                Is_loose_end            : 0
                State                   : 'ACTIVE'
                Isolation               : 'COMMITTED READ'
                Wait_msecs              : -1
                Head_lsa                : '(-1|-1)'
                Tail_lsa                : '(-1|-1)'
                Undo_next_lsa           : '(-1|-1)'
                Postpone_next_lsa       : '(-1|-1)'
                Savepoint_lsa           : '(-1|-1)'
                Topop_lsa               : '(-1|-1)'
                Tail_top_result_lsa     : '(-1|-1)'
                Client_id               : 108
                Client_type             : 'CSQL'
                Client_info             : ''
                Client_db_user          : 'PUBLIC'
                Client_program          : 'csql'
                Client_login_user       : 'cubrid'
                Client_host             : 'cubrid001'
                Client_pid              : 13190
                Topop_depth             : 0
                Num_unique_btrees       : 0
                Max_unique_btrees       : 0
                Interrupt               : 0
                Num_transient_classnames: 0
                Repl_max_records        : 0
                Repl_records            : NULL
                Repl_current_index      : 0
                Repl_append_index       : -1
                Repl_flush_marked_index : -1
                Repl_insert_lsa         : '(-1|-1)'
                Repl_update_lsa         : '(-1|-1)'
                First_save_entry        : NULL
                Tran_unique_stats       : NULL
                Modified_class_list     : NULL
                Num_temp_files          : 0
                Waiting_for_res         : NULL
                Has_deadlock_priority   : 0
                Suppress_replication    : 0
                Query_timeout           : NULL
                Query_start_time        : 03:10:11.425 PM 02/04/2016
                Tran_start_time         : 03:10:11.425 PM 02/04/2016
                Xasl_id                 : 'vpid: (32766|50), vfid: (32766|43)'
                Disable_modifications   : 0
                Abort_reason            : 'NORMAL'

SHOW THREADS
------------

It shows internal information of each thread. The results are sorted by "Index" column with ascending order and may not be a consistent snapshot of thread entries.
The statement under SA MODE shows an empty result. 

.. code-block:: sql

    SHOW THREADS [ WHERE EXPR ];

This query has the following columns:

=========================== =============== ==============================================================================================================================================================
Column name                 Type            Description
=========================== =============== ==============================================================================================================================================================
Index                       INT             Thread entry index.
Jobq_index                  INT             Job queue index only for worker threads. NULL for non-worker threads.
Thread_id                   BIGINT          Thread id.
Tran_index                  INT             Transaction index to which this thread belongs. If no related tran index, NULL.
Type                        VARCHAR(8)      Thread type. Either one of the followings: 'MASTER', 'SERVER', 'WORKER', 'DAEMON', 'VACUUM_MASTER', 'VACUUM_WORKER', 'NONE', 'UNKNOWN'.
Status                      VARCHAR(8)      Thread status. Either one of the followings: 'FREE', 'RUN', 'WAIT', 'CHECK'.
Resume_status               VARCHAR(32)     Resume status. Either one of the followings: 'RESUME_NONE', 'RESUME_DUE_TO_INTERRUPT', 'RESUME_DUE_TO_SHUTDOWN', 'PGBUF_SUSPENDED', 'PGBUF_RESUMED', 
                                            'JOB_QUEUE_SUSPENDED', 'JOB_QUEUE_RESUMED', 'CSECT_READER_SUSPENDED', 'CSECT_READER_RESUMED', 'CSECT_WRITER_SUSPENDED', 'CSECT_WRITER_RESUMED',
                                            'CSECT_PROMOTER_SUSPENDED', 'CSECT_PROMOTER_RESUMED', 'CSS_QUEUE_SUSPENDED', 'CSS_QUEUE_RESUMED', 'QMGR_ACTIVE_QRY_SUSPENDED', 'QMGR_ACTIVE_QRY_RESUMED',
                                            'QMGR_MEMBUF_PAGE_SUSPENDED', 'QMGR_MEMBUF_PAGE_RESUMED', 'HEAP_CLSREPR_SUSPENDED', 'HEAP_CLSREPR_RESUMED', 'LOCK_SUSPENDED', 'LOCK_RESUMED', 
                                            'LOGWR_SUSPENDED', 'LOGWR_RESUMED'
Net_request                 VARCHAR(64)     The net request name in net_requests array, such as: 'LC_ASSIGN_OID'. If not request name, shows NULL
Conn_client_id              INT             Client id whom this thread is responding, if no client id, shows NULL
Conn_request_id             INT             Request id which this thread is processing, if no request id, shows NULL
Conn_index                  INT             Connection index, if not connection index, shows NULL
Last_error_code             INT             Last error code
Last_error_msg              VARCHAR(256)    Last error message, if message length is more than 256, it will be truncated, If no error message, shows NULL
Private_heap_id             VARCHAR(20)     The address of id of thread private memory allocator, such as: 0x12345678. If no related heap id, shows NULL.
Query_entry                 VARCHAR(20)     The address of the QMGR_QUERY_ENTRY*, such as: 0x12345678, if no related QMGR_QUERY_ENTRY, shows NULL.
Interrupted                 INT             0 or 1, is this request/transaction interrupted
Shutdown                    INT             0 or 1, is server going down?
Check_interrupt             INT             0 or 1
Wait_for_latch_promote      INT             0 or 1, whether this thread is waiting for latch promotion.
Lockwait_blocked_mode       VARCHAR(24)     Lockwait blocked mode. Either one of the followings: 'NULL_LOCK', 'IS_LOCK', 'S_LOCK', 'IS_LOCK', 'IX_LOCK', 'SIX_LOCK', 'X_LOCK', 'SCH_M_LOCK', 'UNKNOWN'
Lockwait_start_time         DATETIME        Start blocked time, if not in blocked state, shows NULL
Lockwait_msecs              INT             Time in milliseconds, if not in blocked state, shows NULL
Lockwait_state              VARCHAR(24)     The lock wait state such as: 'SUSPENDED', 'RESUMED', 'RESUMED_ABORTED_FIRST', 'RESUMED_ABORTED_OTHER', 'RESUMED_DEADLOCK_TIMEOUT', 'RESUMED_TIMEOUT', 
                                            'RESUMED_INTERRUPT'. If not in blocked state, shows NULL
Next_wait_thread_index      INT             The next wait thread index, if not exist, shows NULL
Next_tran_wait_thread_index INT             The next wait thread index in lock manager, if not exist, shows NULL
Next_worker_thread_index    INT             The next worker thread index in css_Job_queue.worker_thrd_list, if not exist, shows NULL
=========================== =============== ==============================================================================================================================================================

The following shows the examples of the statement.

.. code-block:: sql

    SHOW THREADS WHERE RESUME_STATUS != 'RESUME_NONE' AND STATUS != 'FREE';

::

    === <Result of SELECT Command in Line 1> ===
    <00001> Index                      : 183
            Jobq_index                 : 3
            Thread_id                  : 140077788813056
            Tran_index                 : 3
            Type                       : 'WORKER'
            Status                     : 'RUN'
            Resume_status              : 'JOB_QUEUE_RESUMED'
            Net_request                : 'QM_QUERY_EXECUTE'
            Conn_client_id             : 108
            Conn_request_id            : 196635
            Conn_index                 : 3
            Last_error_code            : 0
            Last_error_msg             : NULL
            Private_heap_id            : '0x02b3de80'
            Query_entry                : '0x7f6638004cb0'
            Interrupted                : 0
            Shutdown                   : 0
            Check_interrupt            : 1
            Wait_for_latch_promote     : 0
            Lockwait_blocked_mode      : NULL
            Lockwait_start_time        : NULL
            Lockwait_msecs             : NULL
            Lockwait_state             : NULL
            Next_wait_thread_index     : NULL
            Next_tran_wait_thread_index: NULL
            Next_worker_thread_index   : NULL
    <00002> Index                      : 192
            Jobq_index                 : 2
            Thread_id                  : 140077779339008
            Tran_index                 : 2
            Type                       : 'WORKER'
            Status                     : 'WAIT'
            Resume_status              : 'LOCK_SUSPENDED'
            Net_request                : 'LC_FIND_LOCKHINT_CLASSOIDS'
            Conn_client_id             : 107
            Conn_request_id            : 131097
            Conn_index                 : 2
            Last_error_code            : 0
            Last_error_msg             : NULL
            Private_heap_id            : '0x02bcdf10'
            Query_entry                : NULL
            Interrupted                : 0
            Shutdown                   : 0
            Check_interrupt            : 1
            Wait_for_latch_promote     : 0
            Lockwait_blocked_mode      : 'SCH_S_LOCK'
            Lockwait_start_time        : 10:47:45.000 AM 02/03/2016
            Lockwait_msecs             : -1
            Lockwait_state             : 'SUSPENDED'
            Next_wait_thread_index     : NULL
            Next_tran_wait_thread_index: NULL
            Next_worker_thread_index   : NULL
            
SHOW JOB QUEUES
---------------

It shows the status of job queue. The statement under SA MODE shows an empty result. 

.. code-block:: sql

    SHOW JOB QUEUES;

This query has the following columns:

=========================== =============== =======================================================
Column name                 Type            Description
=========================== =============== =======================================================
Jobq_index                  INT             The index of job queue
Num_total_workers           INT             Total number of work threads of the queue
Num_busy_workers            INT             The number of busy worker threads of the queue
Num_connection_workers      INT             The number of connection worker threads of the queue
=========================== =============== =======================================================

SHOW PAGE BUFFER STATUS
-----------------------

It shows the status of the data page buffer pool.

.. code-block:: sql

    SHOW PAGE BUFFER STATUS;

This query has the following columns:

=========================== =============== ================================================================================
Column name                 Type            Description
=========================== =============== ================================================================================
Hit_rate                    NUMERIC(13,10)  The buffer pool hit rate (since the last printout)
Num_hit                     BIGINT          The number of buffer hits (since the last printout)
Num_page_request            BIGINT          The number of page requests (since the last printout)
Pool_size                   INT             Buffer pool size in pages
Page_size                   INT             Data page size
Free_pages                  INT             The number of free pages in the buffer pool
Victim_candidate_pages      INT             The number of victim candidate pages in the cold LRU area
Clean_pages                 INT             The number of clean pages in the buffer pool
Dirty_pages                 INT             The number of dirty pages in the buffer pool
Num_index_pages             INT             The number of index pages in the buffer pool
Num_data_pages              INT             The number of data pages in the buffer pool
Num_system_pages            INT             The number of system pages in the buffer pool
Num_temp_pages              INT             The number of temp pages in the buffer pool
Num_pages_created           BIGINT          The number of pages created in the buffer pool (since the last printout)
Num_pages_written           BIGINT          The number of pages written to disk in the buffer pool (since the last printout)
Pages_written_rate          NUMERIC(20,10)  The number of pages written per second (since the last printout)
Num_pages_read              BIGINT          The number of pages read from disk in the buffer pool (since the last printout)
Pages_read_rate             NUMERIC(20,10)  The number of pages read per second (since the last printout)
Num_flusher_waiting_threads INT             The number of waiting threads for flusher
=========================== =============== ================================================================================

The following shows the examples of this syntax.

.. code-block:: sql

    -- csql> ;line on
    SHOW PAGE BUFFER STATUS;

::

    <00001> Hit_rate                   : 0.0000000000
            Num_hit                    : 0
            Num_page_request           : 0
            Pool_size                  : 32768
            Page_size                  : 16392
            Free_pages                 : 32739
            Victim_candidate_pages     : 0
            Clean_pages                : 32767
            Dirty_pages                : 1
            Num_index_pages            : 2
            Num_data_pages             : 15
            Num_system_pages           : 12
            Num_temp_pages             : 0
            Num_pages_created          : 0
            Num_pages_written          : 0
            Pages_written_rate         : 0.0000000000
            Num_pages_read             : 0
            Pages_read_rate            : 0.0000000000
            Num_flusher_waiting_threads: 0
