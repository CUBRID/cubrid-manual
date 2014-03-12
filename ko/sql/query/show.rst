****
SHOW
****

DESC, DESCRIBE
==============

**DESC** 또는 **DESCRIBE** 문은 테이블의 칼럼 정보를 출력하며 **SHOW COLUMNS** 문과 같다. 보다 자세한 사항은 :ref:`show-columns-statement`\ 를 참고한다.

::

    DESC tbl_name
    DESCRIBE tbl_name
    
EXPLAIN
=======

**EXPLAIN** 문은 테이블의 칼럼 정보를 출력하며 **SHOW COLUMNS** 문과 같다. 보다 자세한 사항은 :ref:`show-columns-statement`\ 를 참고한다.

::

    EXPLAIN tbl_name

.. _show-tables-statement:

SHOW TABLES
===========

데이터베이스의 전체 테이블 이름 목록을 출력한다. 결과 칼럼의 이름은 *tables_in_<데이터베이스 이름>* 이 되며 하나의 칼럼을 지닌다. **LIKE** 절을 사용하면 이와 매칭되는 테이블 이름을 검색할 수 있으며, **WHERE** 절을 사용하면 좀더 일반적인 조건으로 테이블 이름을 검색할 수 있다. **SHOW FULL TABLES** 는 *table_type* 이라는 이름의 두 번째 칼럼을 함께 출력하며, 테이블은 **BASE TABLE**, 뷰는 **VIEW** 라는 값을 가진다. ::

    SHOW [FULL] TABLES [LIKE 'pattern' | WHERE expr]

다음은 *demodb* 를 가지고 해당 질의를 실행한 결과이다.

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

테이블의 칼럼 정보를 출력한다. **LIKE** 절을 사용하면 이와 매칭되는 칼럼 이름을 검색할 수 있다. **WHERE** 절을 사용하면 "모든 **SHOW** 문에 대한 일반적인 고려 사항"과 같이 좀 더 일반적인 조건으로 칼럼 이름을 검색할 수 있다. 

::

    SHOW [FULL] COLUMNS {FROM | IN} tbl_name [LIKE 'pattern' | WHERE expr]

**FULL** 키워드가 사용되면 Collation 정보를 추가로 출력한다.

**SHOW FIELDS** 는 **SHOW COLUMNS** 와 같은 구문이다.

**DESCRIBE** (또는 줄여서 **DESC**) 문과 **EXPLAIN** 문은 **SHOW COLUMNS**\ 와 같은 정보를 제공하지만, LIKE 절 또는 WHERE 절은 지원하지 않는다.

해당 구문은 다음과 같은 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
=================================== =============== ======================================================================================================================================
Field                               VARCHAR         칼럼 이름
Type                                VARCHAR         칼럼의 데이터 타입
Null                                VARCHAR         **NULL** 을 저장할 수 있으면 YES, 불가능하면 NO
Key                                 VARCHAR         칼럼에 인덱스가 걸려있는지 여부. 테이블의 주어진 칼럼에 하나 이상의 키 값이 존재하면 PRI, UNI, MUL의 순서 중 가장 먼저 나타나는 것 하나만 출력한다.
                                                        * 공백이면 인덱스를 타지 않거나 다중 칼럼 인덱스에서 첫번째 칼럼이 아니거나, 비고유(non-unique) 인덱스이다.
                                                        * PRI 값이면 기본 키이거나 다중 칼럼 기본 키이다.
                                                        * UNI 값이면 고유(unique) 인덱스이다. (고유 인덱스는 여러 개의 NULL값을 허용하지만, NOT NULL 제약 조건을 설정할 수도 있다.)
                                                        * MUL 값이면 주어진 값이 칼럼 내에서 여러 번 나타나는 것을 허용하는 비고유 인덱스의 첫번째 칼럼이다. 복합 고유 인덱스를 구성하는 칼럼이면 MUL 값이 된다. 칼럼 값들의 결합은 고유일 수 있으나 각 칼럼의 값은 여러 번 나타날 수 있기 때문이다.
Default                             VARCHAR         칼럼에 정의된 기본값
Extra                               VARCHAR         주어진 칼럼에 대해 가능한 추가 정보. **AUTO_INCREMENT** 속성인 칼럼은 'auto_increment'라는 값을 갖는다.
=================================== =============== ======================================================================================================================================

다음은 해당 질의들을 수행한 예이다.

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

      Field                 Type                  Collation             Null      Key         Default               Extra
    ====================================================================================================================================
      'code'                'INTEGER'             NULL                  'NO'      'PRI'       NULL                  'auto_increment'
      'nation_code'         'CHAR(3)'             'iso88591_bin'        'YES'     ''          NULL                  ''

.. _show-index-statement:

SHOW INDEX
==========

**SHOW INDEX** 문은 인덱스 정보를 출력한다. 

::

    SHOW {INDEX | INDEXES | KEYS } {FROM | IN} tbl_name

해당 질의는 다음과 같은 칼럼을 가진다. 

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
=================================== =============== ======================================================================================================================================
Table                               VARCHAR         테이블 이름
Non_unique                          INTEGER         중복 가능 여부
                                                        * 0: 데이터 중복 불가능
                                                        * 1: 데이터 중복 가능
Key_name                            VARCHAR         인덱스 이름
Seq_in_index                        INTEGER         인덱스에 있는 칼럼의 일련번호. 1부터 시작한다.
Column_name                         VARCHAR         칼럼 이름
Collation                           VARCHAR         칼럼이 인덱스에서 정렬되는 방법. 'A'는 오름차순(Ascending), **NULL** 은 비정렬을 의미한다.
Cardinality                         INTEGER         인덱스에서 유일한 값의 개수를 측정하는 수치. 카디널리티가 높을수록 인덱스를 이용할 기회가 높아진다. 
                                                    이 값은 **SHOW INDEX** 가 실행되면 매번 업데이트된다.
Sub_part                            INTEGER         칼럼의 일부만 인덱스된 경우 인덱스된 문자의 바이트 수. 칼럼 전체가 인덱스되면 **NULL** 이다.
Packed                                              키가 어떻게 팩되었는지(packed)를 나타냄. 팩되지 않은 경우 **NULL**. 현재 지원 안 함.
Null                                VARCHAR         칼럼이 **NULL** 을 포함할 수 있으면 YES, 그렇지 않으면 NO.
Index_type                          VARCHAR         사용되는 인덱스(현재 BTREE만 지원한다).
Func                                VARCHAR         함수 인덱스에서 사용되는 함수
=================================== =============== ======================================================================================================================================

다음은 해당 질의를 실행한 결과이다.

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

**SHOW COLLATION** 문은 데이터베이스에서 지원하는 콜레이션 리스트를 출력한다. LIKE 절은 콜레이션 이름이 매칭되는 정보를 출력한다. 

::

    SHOW COLLATION [ LIKE 'pattern' ]

해당 질의는 다음과 같은 칼럼을 가진다.

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
=================================== =============== ======================================================================================================================================
Collation                           VARCHAR         콜레이션 이름
Charset                             CHAR(1)         문자셋 이름
Id                                  INTEGER         콜레이션 ID
Built_in                            CHAR(1)         내장 콜레이션 여부. 내장 콜레이션들은 하드-코딩되어 있어 추가 혹은 삭제가 불가능하다.
Expansions                          CHAR(1)         확장이 있는 콜레이션인지 여부. 자세한 내용은 :ref:`expansion`\ 을 참조한다.
Strength                            CHAR(1)         문자 간 비교를 위한 기준. 이 기준에 따라 문자 순서가 달라질 수 있다. 이에 대한 설명은 :ref:`collation-properties`\ 를 참고한다.
=================================== =============== ======================================================================================================================================
    
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

**SHOW GRANT** 문은 데이터베이스의 사용자 계정에 부여된 권한을 출력한다. ::

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

**SHOW CREATE TABLE** 문은 테이블 이름을 지정하면 해당 테이블의 **CREATE TABLE** 문을 출력한다. ::

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

**SHOW CREATE TABLE** 문은 사용자가 입력한 구문을 그대로 출력하지는 않는다. 예를 들어, 사용자가 입력한 커멘트를 출력하지 않으며, 테이블 명이나 칼럼 명은 항상 소문자로 출력한다.
    
.. _show-create-view-statement:

SHOW CREATE VIEW
================

**SHOW CREATE VIEW** 문은 뷰 이름을 지정하면 해당 **CREATE VIEW** 문을 출력한다. ::

    SHOW CREATE VIEW view_name

다음은 해당 질의를 실행한 결과이다.

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

**SHOW EXEC STATISTICS** 문은 실행한 질의들의 실행 통계 정보를 출력한다.

*   통계 정보 수집을 시작하려면 세션 변수 **@collect_exec_stats** 의 값을 1로 설정하며, 종료하려면 0으로 설정한다.

*   통계 정보 수집 결과를 출력한다.

    *   **SHOW EXEC STATISTICS**\ 는 data_page_fetches, data_page_dirties, data_page_ioreads, data_page_iowrites 이렇게 4가지 항목의 데이터 페이지 통계 정보를 출력하며, 결과 칼럼은 통계 정보 이름과 값에 해당하는 variable 칼럼과 value 칼럼으로 구성된다. **SHOW EXEC STATISTICS** 문을 실행하고 나면 그동안 누적되었던 통계 정보가 초기화된다.

    *   **SHOW EXEC STATISTICS ALL**\ 은 모든 항목의 통계 정보를 출력한다.

통계 정보 각 항목에 대한 자세한 설명은 :ref:`statdump`\ 을 참고한다.

::

    SHOW EXEC STATISTICS [ALL]

다음은 해당 질의를 실행한 결과이다.

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

**SHOW VOLUME HEADER OF** *volume_id* 문은 명시한 볼륨의 헤더 정보를 하나의 행으로 출력한다.

::

    SHOW VOLUME HEADER OF volume_id;
    
해당 구문은 다음과 같은 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Volume_id                           INT             볼륨 식별자
Magic_symbol                        VARCHAR(100)    볼륨 파일의 매직 값
Io_page_size                        INT             DB 볼륨의 페이지 크기
Purpose                             VARCHAR(32)     볼륨 사용 목적, 목적 타입: DATA, INDEX, GENERIC, TEMP TEMP, TEMP
Sector_size_in_pages                INT             페이지 내 섹터의 크기
Num_total_sectors                   INT             섹터 전체 개수
Num_free_sectors                    INT             여유 섹터 개수
Hint_alloc_sector                   INT             할당될 다음 섹터에 대한 힌트
Num_total_pages                     INT             페이지의 전체 개수
Num_free_pages                      INT             여유 페이지 개수
Sector_alloc_table_size_in_pages    INT             페이지 내 섹터 할당 테이블 크기
Sector_alloc_table_first_page       INT             섹터 할당 테이블의 첫번째 페이지
Page_alloc_table_size_in_pages      INT             페이지 내 페이지 할당 테이블의 크기
Page_alloc_table_first_page         INT             페이지 할당 테이블의 첫번째 페이지
Last_system_page                    INT             마지막 시스템 페이지
Creation_time                       DATETIME        데이터베이스 생성 시간
Num_max_pages                       INT             이 볼륨의 최대 페이지 카운트. 자동 확장된 볼륨인 경우 이 값은 total_pages와는 다르다. 
Num_used_data_pages                 INT             DATA 목적으로 할당된 페이지
Num_used_index_pages                INT             INDEX 목적으로 할당된 페이지
Checkpoint_lsa                      VARCHAR(64)     이 볼륨의 복구 절차를 시작하는 가장 작은 로그 일련 주소
Boot_hfid                           VARCHAR(64)     부팅 및 다중 볼륨을 위한 시스템 힙 파일 ID
Full_name                           VARCHAR(255)    볼륨의 전체 경로
Next_vol_full_name                  VARCHAR(255)    다음 볼륨의 전체 경로
Remarks                             VARCHAR(64)     
=================================== =============== ======================================================================================================================================

다음은 해당 질의를 실행한 결과이다.

.. code-block:: sql

    SHOW VOLUME HEADER OF 1;
    
    csql> ;line on
    csql> SHOW VOLUME HEADER OF 0;

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

**SHOW LOG HEADER OF** *file_name* 구문은 활성 로그(active log) 파일의 헤더 정보를 출력한다.

::

    SHOW LOG HEADER [OF file_name]
    
OF file_name을 생략하면 메모리의 헤더 정보를 출력하며, OF file_name을 포함하면 file_name의 헤더 정보를 출력한다.

해당 구문은 다음의 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Magic_symbol                        VARCHAR(32)     로그 파일의 매직 값
Magic_symbol_location               INT             로그 페이지로부터 매직 심볼 위치
Creation_time                       DATETIME        DB 생성 시간
Release                             VARCHAR(32)     CUBRID 릴리스 버전
Compatibility_disk_version          VARCHAR(32)     현재의 릴리스 버전에 대한 DB의 호환성
Db_page_size                        INT             DB 페이지의 크기
Log_page_size                       INT             로그 페이지의 크기
Shutdown                            INT             로그 셧다운(shutdown) 여부
Next_trans_id                       INT             다음 트랜잭션 ID
Num_avg_trans                       INT             평균 트랜잭션 개수
Num_avg_locks                       INT             평균 객체 잠금 개수
Num_active_log_pages                INT             활성 로그 부분에서 페이지 개수
Db_charset                          INT             DB의 문자셋 번호
First_active_log_page               BIGINT          활성 로그에서 물리적 위치 1에 대한 논리 페이지 ID
Current_append                      VARCHAR(64)     현재의 추가된 위치
Checkpoint                          VARCHAR(64)     복구 프로세스를 시작하는 가장 작은 로그 일련 주소
Next_archive_page_id                BIGINT          보관할 다음 논리 페이지
Active_physical_page_id             INT             보관할 논리 페이지의 물리적 위치
Next_archive_num                    INT             다음 보관 로그 번호
Last_archive_num_for_syscrashes     INT             시스템 크래시에 대비하여 필요한 최종 보관 로그 번호
Last_deleted_archive_num            INT             최종 삭제된 보관 로그 번호
Backup_lsa_level0                   VARCHAR(64)     백업 수준 0의 LSA(log sequence number)
Backup_lsa_level1                   VARCHAR(64)     백업 수준 1의 LSA
Backup_lsa_level2                   VARCHAR(64)     백업 수준 2의 LSA
Log_prefix                          VARCHAR(256)    로그 prefix 이름
Has_logging_been_skipped            INT             로깅의 생략 여부
Perm_status                         VARCHAR(64)     현재 사용 안 함
Backup_info_level0                  VARCHAR(128)    백업 수준 0의 상세 정보. 현재는 백업 시작 시간만 저장됨
Backup_info_level1                  VARCHAR(128)    백업 수준 1의 상세 정보. 현재는 백업 시작 시간만 저장됨
Backup_info_level2                  VARCHAR(128)    백업 수준 2의 상세 정보. 현재는 백업 시작 시간만 저장됨
Ha_server_state                     VARCHAR(32)     HA 서버 상태. 다음 값 중 하나: na, idle, active, to-be-active, standby, to-be-standby,  maintenance, dead
Ha_file                             VARCHAR(32)     HA 복제 상태. 다음 값 중 하나: clear, archived, sync
Eof_lsa                             VARCHAR(64)     
Smallest_lsa_at_last_checkpoint     VARCHAR(64)     
=================================== =============== ======================================================================================================================================

다음은 해당 질의를 실행한 결과이다.

::

    csql> ;line on
    csql> SHOW LOG HEADER;
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
            
    csql> SHOW LOG HEADER OF 'demodb_lgat';

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

**SHOW ARCHIVE LOG HEADER OF** *file_name* 구문은 보관 로그(archive log) 파일의 헤더 정보를 출력한다.

::

    SHOW ARCHIVE LOG HEADER OF file_name

해당 구문은 다음의 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Volume_id                           INT             로그 볼륨 ID
Magic_symbol                        VARCHAR(32)     보관 로그 파일의 매직 값
Magic_symbol_location               INT             로그 페이지로부터 매직 심볼 위치
Creation_time                       DATETIME        DB 생성 시간
Next_trans_id                       BIGINT          다음 트랜잭션 ID
Num_pages                           INT             보관 로그에서 페이지의 개수
First_page_id                       BIGINT          보관 로그에서 물리적 위치 1에 대한 논리 페이지 ID
Archive_num                         INT             보관 로그 번호
=================================== =============== ======================================================================================================================================

다음은 해당 질의를 실행한 결과이다.

::

    csql> ;line on
    csql> SHOW ARCHIVE LOG HEADER OF 'demodb_lgar001';

    <00001> Volume_id            : -20
            Magic_symbol         : 'CUBRID/LogArchive'
            Magic_symbol_location: 16
            Creation_time        : 04:42:28.000 PM 12/11/2013
            Next_trans_id        : 22695
            Num_pages            : 1278
            First_page_id        : 1278
            Archive_num          : 1