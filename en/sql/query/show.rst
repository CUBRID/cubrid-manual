****
SHOW
****


**SHOW**

**SHOW TABLES Statement**

**Description**

Displays the list of all the table names within a database. The name of the result column will be
*tables_in_<database name>*
and it will have one column. If you use the
**LIKE**
clause, you can search the table names matching this and if you use the
**WHERE**
clause, you can search table names with more general terms.
**SHOW FULL TABLES**
displays the second column,
*table_type*
together. The table must have the value,
**BASE TABLE**
and the view has the value,
**VIEW**
.

**Syntax**

**SHOW**
[
**FULL**
]
**TABLES**
[
**LIKE**
'
*pattern*
' |
**WHERE**
*expr*
]

**Example**

The following is the result of executing the query in the
*demodb*
database.

SHOW TABLES;

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

 

SHOW FULL TABLES;

  Tables_in_demodb     Table_type

============================================

  'athlete'             'BASE TABLE'

  'code'                'BASE TABLE'

  'event'               'BASE TABLE'

  'game'                'BASE TABLE'

  'history'             'BASE TABLE'

  'nation'              'BASE TABLE'

  'olympic'             'BASE TABLE'

  'participant'         'BASE TABLE'

  'record'              'BASE TABLE'

  'stadium'             'BASE TABLE'

 

SHOW FULL TABLES LIKE '%c%';

  Tables_in_demodb      Table_type

============================================

  'code'                'BASE TABLE'

  'olympic'             'BASE TABLE'

  'participant'         'BASE TABLE'

  'record'              'BASE TABLE'

 

SHOW FULL TABLES  WHERE table_type = 'BASE TABLE' and TABLES_IN_demodb LIKE '%co%';   Tables_in_demodb      Table_type

============================================

  'code'                'BASE TABLE'

  'record'              'BASE TABLE'

**SHOW COLUMN Statement**

**Description**

Displays the column information of a table. You can use the
**LIKE**
clause to search the column names matching it. If you use the
**WHERE**
clause, you can search column names with more general terms like, "General Considerations for All
**SHOW**
Statements."  If you use the
**FULL**
keyword, the additional information of a column will be displayed as follows:

*   Field: Column name



*   Type: Column data type



*   Null: If you can store
    **NULL**
    , the value is YES and if not, it is NO



*   Key: Whether a column has an index or not. If there is more than one key value in the given column of a table, this displays only the one that appears first in the order of PRI, UNI and MUL.



*   If the key is a space, the column doesn't have an index, it is not the first column in the multiple column index or the index is non-unique.



*   If the value is PRI, it is a primary key or the primary key of multiple columns.



*   If the value is UNI, it is a unique index. (The unique index allows multiple NULL values but you can also set a NOT NULL constraint.)



*   If the value is MUL, it is the first column of the non-unique index that allows the given value to be displayed in the column several times. If the column composes a composite unique index, the value will be MUL. The combination of column values can be unique but the value of each column can appear several times.



*   Default : Default value defined in the column



*   Extra : Additional information available about the given column.
    **AUTO_INCREMENT**
    The column attribute must have the auto_increment value.



**SHOW FIELDS**
is the same command as
**SHOW COLUMNS**
.

The
**DESCRIBE**
(abbreviated
**DESC**
) statement and the
**EXPLAIN**
statement provide similar information to
**SHOW COLUMNS**
.

**Syntax**

**SHOW COLUMNS**
{
**FROM**
|
**IN**
}
*tbl_name*
[
**LIKE**
'
*pattern*
' |
**WHERE**
*expr*
]

**Example**

The following is the result of a query in the
*demodb*
database.

SHOW COLUMNS FROM athlete;

  Field                 Type                  Null                  Key                   Default               Extra

====================================================================================================================================

  'code'                'INTEGER'             'NO'                  'PRI'                 NULL                  'auto_increment'

  'name'                'VARCHAR(40)'          'NO'                  ''                    NULL                  ''

  'gender'              'CHAR(1)'             'YES'                 ''                    NULL                  ''

  'nation_code'         'CHAR(3)'             'YES'                 ''                    NULL                  ''

  'event'               'VARCHAR(30)'          'YES'                 ''                    NULL                  ''

 

SHOW COLUMNS FROM athlete WHERE field LIKE '%c%';

  Field                 Type                  Null                  Key                   Default               Extra

====================================================================================================================================

  'code'                'INTEGER'             'NO'                  'PRI'                 NULL                  'auto_increment'

  'nation_code'         'CHAR(3)'             'YES'                 ''                    NULL                  ''

 

SHOW COLUMNS FROM athlete  WHERE "type" = 'INTEGER' and "key"='PRI' AND extra='auto_increment';

  Field                 Type                  Null                  Key                   Default               Extra

====================================================================================================================================

  'code'                'INTEGER'             'NO'                  'PRI'                 NULL                  'auto_increment'

**SHOW INDEX Statement**

**Description**

The
**SHOW INDEX**
statement displays the index information. The query must have the following columns:

*   Table: Table Name



*   Non_unique



*   0: Duplicate data are not allowed



*   1: Duplicate data are allowed



*   Key_name: Index name



*   Seq_in_index: Serial number of the column in the index. Starts from 1.



*   Column_name: Column name



*   Collation: Method of sorting columns in the index. 'A' means ascending and
    **NULL**
    means not sorted.



*   Cardinality: The number of values measuring the unique values in the index. Higher cardinality increases the opportunity of using an index. This value is updated every time
    **SHOW INDEX**
    is executed.



*   Sub_part: The number of bytes of the indexed characters if the columns are indexed partially.
    **NULL**
    if all columns are indexed.



*   Packed: Shows how keys are packed. If they are not packed, it will be
    **NULL**
    .



*   Null: YES if a column can include
    **NULL**
    , NO if not.



*   Index_type: Index to be used (currently, only the BTREE is supported.)



**Syntax**

**SHOW**
{
**INDEX**
|
**INDEXES**
|
**KEYS**
} {
**FROM**
|
**IN**
}
*tbl_name*

**Example**

The following is the result of a query in the
*demodb*
database.

SHOW INDEX IN athlete;

   Table     Non_unique   Key_name       Seq_in_index  Column_name    Collation     Cardinality   Sub_part  Packed   Null   Index_type

==========================================================================================================================================

 'athlete'     0      'pk_athlete_code'     1          'code'           'A'           6677         NULL     NULL    'NO'      'BTREE'

 

CREATE TABLE t1( i1 INTEGER , i2 INTEGER NOT NULL, i3 INTEGER UNIQUE, s1 VARCHAR(10), s2 VARCHAR(10), s3 VARCHAR(10) UNIQUE);

 

CREATE INDEX i_t1_i1 ON t1(i1 desc);

CREATE INDEX i_t1_s1 ON t1(s1(7));

CREATE INDEX i_t1_i1_s1 ON t1(i1,s1);

CREATE UNIQUE INDEX i_t1_i2_s2 ON t1(i2,s2);

 

SHOW INDEXES FROM t1;

  Table  Non_unique  Key_name          Seq_in_index  Column_name   Collation   Cardinality     Sub_part    Packed   Null    Index_type

==========================================================================================================================================

  't1'           0  'i_t1_i2_s2'              1      'i2'          'A'            0               NULL        NULL     'NO'    'BTREE'

  't1'           0  'i_t1_i2_s2'              2      's2'          'A'            0               NULL        NULL     'YES'   'BTREE'

  't1'           0  'u_t1_i3'                 1      'i3'          'A'            0               NULL        NULL     'YES'   'BTREE'

  't1'           0  'u_t1_s3'                 1      's3'          'A'            0               NULL        NULL     'YES'   'BTREE'

  't1'           1  'i_t1_i1'                 1      'i1'          NULL           0               NULL        NULL     'YES'   'BTREE'

  't1'           1  'i_t1_i1_s1'              1      'i1'          'A'            0               NULL        NULL     'YES'   'BTREE'

  't1'           1  'i_t1_i1_s1'              2      's1'          'A'            0               NULL        NULL     'YES'   'BTREE'

  't1'           1  'i_t1_s1'                 1      's1'          'A'            0                  7        NULL     'YES'   'BTREE'

**SHOW GRANTS Statement**

**Description**

The
**SHOW GRANT**
statement displays the permissions associated with the database user accounts.

**Syntax**

**SHOW GRANTS FOR**
'
*user*
'

**Example**

CREATE TABLE testgrant (id int);

CREATE USER user1;

GRANT INSERT,SELECT ON testgrant TO user1;

 

SHOW GRANTS FOR user1;

  Grants for USER1

======================

  'GRANT INSERT, SELECT ON testgrant TO USER1'

**SHOW CREATE TABLE Statement**

**Description**

When a table name is specified, the
**SHOW CREATE TABLE**
statement outputs the
**CREATE TABLE**
statement of the table.

**Syntax**

**SHOW CREATE TABLE**
*table_name*

**Example**

SHOW CREATE TABLE nation;

 

  TABLE                 CREATE TABLE

============================================

  'nation'              'CREATE TABLE [nation] ([code] CHARACTER(3) NOT NULL, [name] CHARACTER VARYING(40) NO

T NULL, [continent] CHARACTER VARYING(10), [capital] CHARACTER VARYING(30),  CONSTRAINT [pk_nation_code] PRIM

ARY KEY  ([code]))'

**SHOW CREATE VIEW Statement**

**Description**

The
**SHOW CREATE VIEW**
statement outputs the corresponding
**CREATE VIEW**
statement if view name is specified.

**Syntax**

**SHOW CREATE VIEW**
*view_name*

**Example**

The following example shows the result of executing query in the
*demodb*
database.

SHOW CREATE VIEW db_class;

 

  View              Create View

========================================

  'db_class'       'SELECT c.class_name, CAST(c.owner.name AS VARCHAR(255)), CASE c.class_type WHEN 0 THEN 'CLASS' WHEN 1 THEN 'VCLASS' ELSE

                   'UNKNOW' END, CASE WHEN MOD(c.is_system_class, 2) = 1 THEN 'YES' ELSE 'NO' END, CASE WHEN c.sub_classes IS NULL THEN 'NO'

                   ELSE NVL((SELECT 'YES' FROM _db_partition p WHERE p.class_of = c and p.pname IS NULL), 'NO') END, CASE WHEN

                   MOD(c.is_system_class / 8, 2) = 1 THEN 'YES' ELSE 'NO' END FROM _db_class c WHERE CURRENT_USER = 'DBA' OR {c.owner.name}

                   SUBSETEQ (  SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{t.g.name}), SET{})  FROM db_user u, TABLE(groups) AS t(g)  WHERE

                   u.name = CURRENT_USER) OR {c} SUBSETEQ (  SELECT SUM(SET{au.class_of})  FROM _db_auth au  WHERE {au.grantee.name} SUBSETEQ

                   (  SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{t.g.name}), SET{})  FROM db_user u, TABLE(groups) AS t(g)  WHERE u.name =

                   CURRENT_USER) AND  au.auth_type = 'SELECT')'

**SHOW EXEC STATISTICS Statement**

**Description**

The
**SHOW EXEC STATISTICS**
statement outputs statistics information of executing query.

*   To start collecting
    **@collect_exec_stats**
    statistics information, configure the value of session variable (
    **@collect_exec_stats)**
    to 1; to stop, configure it to 0.



*   It outputs the result of collecting statistics information.



*   The
    **SHOW EXEC STATISTICS**
    statement outputs four part of data page statistics information; data_page_fetches, data_page_dirties, data_page_ioreads, and data_page_iowrites. The result columns consist of variable column (name of statistics name) and value column (value of statistics value). Once the
    **SHOW EXEC STATISTICS**
    statement is executed, the statistics information which has been accumulated is initialized.



*   The
    **SHOW EXEC STATISTICS ALL**
    statement outputs all items of statistics information.



For details, see
`Outputting Statistics Information of Server <#admin_admin_db_statdump_htm>`_
.

**Syntax**

**SHOW EXEC STATISTICS[ ALL]**

**Example**

The following example shows the result of exeucting query in the
*demodb*
database.

-- set session variable @collect_exec_stats as 1 to start collecting the statistical information.

SET @collect_exec_stats = 1;

SELECT * FROM db_class;

...

 

-- print the statistical information of the data pages.

SHOW EXEC STATISTICS;

variable value

============================================

'data_page_fetches' 332

'data_page_dirties' 85

'data_page_ioreads' 18

'data_page_iowrites' 28

 

SELECT * FROM db_index;

...

 

-- print all of the statistical information.

SHOW EXEC STATISTICS ALL;

 

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
