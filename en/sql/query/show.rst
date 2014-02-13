****
SHOW
****

DESC, DESCRIBE
==============

**DESC** or **DESCRIBE** statement displays the column information of a table, and it's like a **SHOW COLUMNS** statement. For more details, see :ref:`show-columns-statement`.

::

    DESC tbl_name
    DESCRIBE tbl_name
    
EXPLAIN
=======

**EXPLAIN** statement displays the column information of a table, and it's like a **SHOW COLUMNS** statement. For more details, see :ref:`show-columns-statement`.

::

    EXPLAIN tbl_name

.. _show-tables-statement:

SHOW TABLES
===========

Displays the list of all table names within a database. The name of the result column will be *tables_in_<database name>* and it will have one column. If you use the **LIKE** clause, you can search the table names matching this and if you use the **WHERE** clause, you can search table names with more general terms. **SHOW FULL TABLES** displays the second column, *table_type* together. The table must have the value, **BASE TABLE** and the view has the value, **VIEW**. ::

    SHOW [FULL] TABLES [LIKE 'pattern' | WHERE expr]

The following is the result of executing the query in the *demodb* database.

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

Displays the column information of a table. You can use the **LIKE** clause to search the column names matching it. If you use the **WHERE** clause, you can search column names with more general terms like, "General Considerations for All **SHOW** Statements."  :

* Field: Column name
* Type: Column data type
* Null: If you can store **NULL**, the value is YES and if not, it is NO
* Key: Whether a column has an index or not. If there is more than one key value in the given column of a table, this displays only the one that appears first in the order of PRI, UNI and MUL.
    *   If the key is a space, the column doesn't have an index, it is not the first column in the multiple column index or the index is non-unique.
    *   If the value is PRI, it is a primary key or the primary key of multiple columns.
    *   If the value is UNI, it is a unique index. (The unique index allows multiple NULL values but you can also set a NOT NULL constraint.)
    *   If the value is MUL, it is the first column of the non-unique index that allows the given value to be displayed in the column several times. If the column composes a composite unique index, the value will be MUL. The combination of column values can be unique but the value of each column can appear several times.
*   Default : Default value defined in the column
*   Extra : Additional information available on the given column. For the column with **AUTO_INCREMENT** constraint, it shows the 'auto_increment'.

If a **FULL** keyword is used, it displays the additional information, collation.

**SHOW FIELDS** is the same statement as **SHOW COLUMNS**.

The **DESCRIBE** (abbreviated **DESC**) statement and the **EXPLAIN** statement provide the same information with **SHOW COLUMNS**, but they don't support LIKE clause or WHERE clause.

::

    SHOW [FULL] COLUMNS {FROM | IN} tbl_name [LIKE 'pattern' | WHERE expr]

The following is the result of a query in the *demodb* database.

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

The **SHOW INDEX** statement displays the index information. This query has the following columns:

*   Table: Table Name
*   Non_unique

    *   0: Duplicate data are not allowed
    *   1: Duplicate data are allowed
    
*   Key_name: Index name
*   Seq_in_index: Serial number of the column in the index. Starts from 1.
*   Column_name: Column name
*   Collation: Method of sorting columns in the index. 'A' means ascending and **NULL** means not sorted.
*   Cardinality: The number of values measuring the unique values in the index. Higher cardinality increases the opportunity of using an index. This value is updated every time **SHOW INDEX** is executed.
*   Sub_part: The number of bytes of the indexed characters if the columns are indexed partially. **NULL** if all columns are indexed.
*   Packed: Shows how keys are packed. If they are not packed, it will be **NULL**.
*   Null: YES if a column can include **NULL**, NO if not.
*   Index_type: Index to be used (currently, only the BTREE is supported.)
*   Func: A function which is used in a function-based index

::

    SHOW {INDEX | INDEXES | KEYS } {FROM | IN} tbl_name

The following is the result of a query in the *demodb* database.

.. code-block:: sql

    SHOW INDEX IN athlete;
    
::

       Table     Non_unique   Key_name       Seq_in_index  Column_name    Collation     Cardinality   Sub_part  Packed   Null   Index_type  Func
    =============================================================================================================================================
     'athlete'     0      'pk_athlete_code'     1          'code'           'A'           6677         NULL     NULL    'NO'      'BTREE'   NULL
     
.. code-block:: sql

    CREATE TABLE t1 (i1 INTEGER , i2 INTEGER NOT NULL, i3 INTEGER UNIQUE, s1 VARCHAR(10), s2 VARCHAR(10), s3 VARCHAR(10) UNIQUE);
     
    CREATE INDEX i_t1_i1 ON t1 (i1 DESC);
    CREATE INDEX i_t1_s1 ON t1 (s1 (7));
    CREATE INDEX i_t1_i1_s1 ON t1 (i1, s1);
    CREATE UNIQUE INDEX i_t1_i2_s2 ON t1 (i2, s2);
     
    SHOW INDEXES FROM t1;
    
::

      Table  Non_unique  Key_name      Seq_in_index  Column_name  Collation  Cardinality     Sub_part  Packed  Null    Index_type   Func
    =====================================================================================================================================
      't1'            1  'i_t1_i1'                1  'i1'         'D'                  0         NULL  NULL    'YES'   'BTREE'      NULL
      't1'            1  'i_t1_i1_s1'             1  'i1'         'A'                  0         NULL  NULL    'YES'   'BTREE'      NULL
      't1'            1  'i_t1_i1_s1'             2  's1'         'A'                  0         NULL  NULL    'YES'   'BTREE'      NULL
      't1'            0  'i_t1_i2_s2'             1  'i2'         'A'                  0         NULL  NULL    'NO'    'BTREE'      NULL
      't1'            0  'i_t1_i2_s2'             2  's2'         'A'                  0         NULL  NULL    'YES'   'BTREE'      NULL
      't1'            1  'i_t1_s1'                1  's1'         'A'                  0            7  NULL    'YES'   'BTREE'      NULL
      't1'            0  'u_t1_i3'                1  'i3'         'A'                  0         NULL  NULL    'YES'   'BTREE'      NULL
      't1'            0  'u_t1_s3'                1  's3'         'A'                  0         NULL  NULL    'YES'   'BTREE'      NULL

.. _show-collation-statement:
 
SHOW COLLATION
==============

**SHOW COLLATION** statement lists collations supported by the database. If LIKE clause is present, it indicates which collation names to match. 
This query has the following columns:

* Collation: Collation name
* Charset: Charset name
* Id: Collation ID
* Built_in: Built-in collation or not. Built-in collations are impossible to add or remove because they are hard-coded.
* Expansions: Collation with expansion or not. For details, see :ref:`expansion`.
* Strength: The number of levels that are to be considered in comparison, and the character order can be different by this number. For details, see :ref:`collation-properties`.

The following shows **SHOW COLLATION** syntax and examples.

::

    SHOW COLLATION [ LIKE 'pattern' ]

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

The **SHOW GRANT** statement displays the permissions associated with the database user accounts. ::

    SHOW GRANTS FOR 'user'

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

When a table name is specified, the **SHOW CREATE TABLE** statement outputs the **CREATE TABLE** statement of the table. ::

    SHOW CREATE TABLE table_name
    
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

The **SHOW CREATE VIEW** statement outputs the corresponding **CREATE VIEW** statement if view name is specified. ::

    SHOW CREATE VIEW view_name

The following example shows the result of executing query in the *demodb* database.

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

.. _show-exec-statistics-statement:

SHOW EXEC STATISTICS
====================

The **SHOW EXEC STATISTICS** statement outputs statistics information of executing query.

*   To start collecting **@collect_exec_stats** statistics information, configure the value of session variable **@collect_exec_stats** to 1; to stop, configure it to 0.

*   It outputs the result of collecting statistics information.

    *   The **SHOW EXEC STATISTICS** statement outputs four part of data page statistics information; data_page_fetches, data_page_dirties, data_page_ioreads, and data_page_iowrites. The result columns consist of variable column (name of statistics name) and value column (value of statistics value). Once the **SHOW EXEC STATISTICS** statement is executed, the statistics information which has been accumulated is initialized.

    *   The **SHOW EXEC STATISTICS ALL** statement outputs all items of statistics information.

For details, see :ref:`statdump`.

::

    SHOW EXEC STATISTICS [ALL]

The following example shows the result of executing query in the *demodb* database.

.. code-block:: sql

    -- set session variable @collect_exec_stats as 1 to start collecting the statistical information.
    SET @collect_exec_stats = 1;
    SELECT * FROM db_class;
     
    -- print the statistical information of the data pages.
    SHOW EXEC STATISTICS;
    
::

    variable value
    ============================================
    'data_page_fetches' 332
    'data_page_dirties' 85
    'data_page_ioreads' 18
    'data_page_iowrites' 28
     
.. code-block:: sql

    SELECT * FROM db_index;
    
    -- print all of the statistical information.
    SHOW EXEC STATISTICS ALL;

::
    
    variable value
    ============================================
    'file_creates' 0
    'file_removes' 0
    'file_ioreads' 6
    'file_iowrites' 0
    'file_iosynches' 0
    'data_page_fetches' 548
    'data_page_dirties' 34
    'data_page_ioreads' 6
    'data_page_iowrites' 0
    'data_page_victims' 0
    'data_page_iowrites_for_replacement' 0
    'log_page_ioreads' 0
    'log_page_iowrites' 0
    'log_append_records' 0
    'log_checkpoints' 0
    'log_wals' 0
    'page_locks_acquired' 13
    'object_locks_acquired' 9
    'page_locks_converted' 0
    'object_locks_converted' 0
    'page_locks_re-requested' 0
    'object_locks_re-requested' 8
    'page_locks_waits' 0
    'object_locks_waits' 0
    'tran_commits' 3
    'tran_rollbacks' 0
    'tran_savepoints' 0
    'tran_start_topops' 6
    'tran_end_topops' 6
    'tran_interrupts' 0
    'btree_inserts' 0
    'btree_deletes' 0
    'btree_updates' 0
    'btree_covered' 0
    'btree_noncovered' 2
    'btree_resumes' 0
    'btree_multirange_optimization' 0
    'query_selects' 4
    'query_inserts' 0
    'query_deletes' 0
    'query_updates' 0
    'query_sscans' 2
    'query_iscans' 4
    'query_lscans' 0
    'query_setscans' 2
    'query_methscans' 0
    'query_nljoins' 2
    'query_mjoins' 0
    'query_objfetches' 0
    'network_requests' 88
    'adaptive_flush_pages' 0
    'adaptive_flush_log_pages' 0
    'adaptive_flush_max_pages' 0
    'network_requests' 88
    'adaptive_flush_pages' 0
    'adaptive_flush_log_pages' 0
    'adaptive_flush_max_pages' 0
