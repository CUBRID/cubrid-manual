****
SHOW
****

SHOW TABLES 문
==============

데이터베이스의 전체 테이블 이름 목록을 출력한다. 결과 칼럼의 이름은 *tables_in_<데이터베이스 이름>* 이 되며 하나의 칼럼을 지닌다. **LIKE** 절을 사용하면 이와 매칭되는 테이블 이름을 검색할 수 있으며, **WHERE** 절을 사용하면 좀더 일반적인 조건으로 테이블 이름을 검색할 수 있다. **SHOW FULL TABLES** 는 *table_type* 이라는 이름의 두 번째 칼럼을 함께 출력하며, 테이블은 **BASE TABLE**, 뷰는 **VIEW** 라는 값을 가진다. ::

    SHOW [FULL] TABLES [LIKE 'pattern' | WHERE expr]

다음은 *demodb* 를 가지고 해당 질의를 실행한 결과이다.

.. code-block:: sql

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
     
    SHOW FULL TABLES LIKE '%c%';
    
      Tables_in_demodb      Table_type
    ============================================
      'code'                'BASE TABLE'
      'olympic'             'BASE TABLE'
      'participant'         'BASE TABLE'
      'record'              'BASE TABLE'
     
    SHOW FULL TABLES WHERE table_type = 'BASE TABLE' and TABLES_IN_demodb LIKE '%co%';
    
      Tables_in_demodb      Table_type
    ============================================
      'code'                'BASE TABLE'
      'record'              'BASE TABLE'

SHOW COLUMNS 문
===============

테이블의 칼럼 정보를 출력한다. **LIKE** 절을 사용하면 이와 매칭되는 칼럼 이름을 검색할 수 있다. **WHERE** 절을 사용하면 "모든 **SHOW** 문에 대한 일반적인 고려 사항"과 같이 좀 더 일반적인 조건으로 칼럼 이름을 검색할 수 있다. 

* Field : 칼럼 이름
* Type : 칼럼의 데이터 타입.
* Null : **NULL** 을 저장할 수 있으면 YES, 불가능하면 NO
* Key : 칼럼에 인덱스가 걸려있는지 여부. 테이블의 주어진 칼럼에 하나 이상의 키 값이 존재하면 PRI, UNI, MUL의 순서 중 가장 먼저 나타나는 것 하나만 출력한다.
   * 공백이면 인덱스를 타지 않거나 다중 칼럼 인덱스에서 첫번째 칼럼이 아니거나, 비고유(non-unique) 인덱스이다.
   * PRI 값이면 기본 키이거나 다중 칼럼 기본 키이다.
   * UNI 값이면 고유(unique) 인덱스이다. (고유 인덱스는 여러 개의 NULL값을 허용하지만, NOT NULL 제약 조건을 설정할 수도 있다.)
   * MUL 값이면 주어진 값이 칼럼 내에서 여러 번 나타나는 것을 허용하는 비고유 인덱스의 첫번째 칼럼이다. 복합 고유 인덱스를 구성하는 칼럼이면 MUL 값이 된다. 칼럼 값들의 결합은 고유일 수 있으나 각 칼럼의 값은 여러 번 나타날 수 있기 때문이다.
* Default : 칼럼에 정의된 기본값
* Extra : 주어진 칼럼에 대해 가능한 추가 정보. **AUTO_INCREMENT** 속성인 칼럼은 auto_increment라는 값을 갖는다.

**FULL** 키워드가 사용되면 Collation 정보를 추가로 출력한다.

**SHOW FIELDS** 는 **SHOW COLUMNS** 와 같은 구문이다.

**DESCRIBE** (또는 줄여서 **DESC**) 문과 **EXPLAIN** 문은 **SHOW COLUMNS** 와 비슷한 정보를 제공한다.

::

    SHOW [FULL] COLUMNS {FROM | IN} tbl_name [LIKE 'pattern' | WHERE expr]


다음은 해당 질의들을 수행한 예이다.

.. code-block:: sql

    SHOW COLUMNS FROM athlete;
    
      Field                 Type                  Null       Key          Default               Extra
    ================================================================================================================
      'code'                'INTEGER'             'NO'       'PRI'        NULL                  'auto_increment'
      'name'                'VARCHAR(40)'         'NO'       ''           NULL                  ''
      'gender'              'CHAR(1)'             'YES'      ''           NULL                  ''
      'nation_code'         'CHAR(3)'             'YES'      ''           NULL                  ''
      'event'               'VARCHAR(30)'         'YES'      ''           NULL                  ''
     
    SHOW COLUMNS FROM athlete WHERE field LIKE '%c%';
    
      Field                 Type                  Null       Key          Default               Extra
    ================================================================================================================
      'code'                'INTEGER'             'NO'       'PRI'        NULL                  'auto_increment'
      'nation_code'         'CHAR(3)'             'YES'      ''           NULL                  ''
     
    SHOW COLUMNS FROM athlete  WHERE "type" = 'INTEGER' and "key"='PRI' AND extra='auto_increment';
    
      Field                 Type                  Null       Key          Default               Extra
    ================================================================================================================
      'code'                'INTEGER'             'NO'       'PRI'        NULL                  'auto_increment'
    
    SHOW FULL COLUMNS FROM athlete WHERE field LIKE '%c%';
    
      Field                 Type                  Collation             Null      Key         Default               Extra
    ====================================================================================================================================
      'code'                'INTEGER'             NULL                  'NO'      'PRI'       NULL                  'auto_increment'
      'nation_code'         'CHAR(3)'             'iso88591_bin'        'YES'     ''          NULL                  ''

SHOW INDEX 문
=============

**SHOW INDEX** 문은 인덱스 정보를 출력한다. 해당 질의는 다음과 같은 칼럼을 가진다.

* Table : 테이블 이름
* Non_unique
   * 0 : 데이터 중복 불가능
   * 1 : 데이터 중복 가능
*   Key_name : 인덱스 이름
*   Seq_in_index : 인덱스에 있는 칼럼의 일련번호. 1부터 시작한다.
*   Column_name : 칼럼 이름
*   Collation :칼럼이 인덱스에서 정렬되는 방법. 'A'는 오름차순(Ascending), **NULL** 은 비정렬을 의미한다.
*   Cardinality : 인덱스에서 유일한 값의 개수를 측정하는 수치. 카디널리티가 높을수록 인덱스를 이용할 기회가 높아진다. 이 값은 **SHOW INDEX** 가 실행되면 매번 업데이트된다.
*   Sub_part : 칼럼의 일부만 인덱스된 경우 인덱스된 문자의 바이트 수. 칼럼 전체가 인덱스되면 **NULL** 이다.
*   Packed : 키가 어떻게 팩되었는지(packed)를 나타냄. 팩되지 않은 경우 **NULL** .
*   Null : 칼럼이 **NULL** 을 포함할 수 있으면 YES, 그렇지 않으면 NO.
*   Index_type : 사용되는 인덱스(현재 BTREE만 지원한다).

::

    SHOW {INDEX | INDEXES | KEYS } {FROM | IN} tbl_name

다음은 해당 질의를 실행한 결과이다.

.. code-block:: sql

    SHOW INDEX IN athlete;
    
       Table     Non_unique   Key_name       Seq_in_index  Column_name    Collation     Cardinality   Sub_part  Packed   Null   Index_type
    ==========================================================================================================================================
     'athlete'     0      'pk_athlete_code'     1          'code'           'A'           6677         NULL     NULL    'NO'      'BTREE'
     
    CREATE TABLE t1 (i1 INTEGER , i2 INTEGER NOT NULL, i3 INTEGER UNIQUE, s1 VARCHAR(10), s2 VARCHAR(10), s3 VARCHAR(10) UNIQUE);
     
    CREATE INDEX i_t1_i1 ON t1 (i1 DESC);
    CREATE INDEX i_t1_s1 ON t1 (s1 (7));
    CREATE INDEX i_t1_i1_s1 ON t1 (i1, s1);
    CREATE UNIQUE INDEX i_t1_i2_s2 ON t1 (i2, s2);
     
    SHOW INDEXES FROM t1;
    
      Table  Non_unique  Key_name          Seq_in_index  Column_name   Collation   Cardinality     Sub_part    Packed   Null    Index_type
    ==========================================================================================================================================
      't1'           0  'i_t1_i2_s2'              1      'i2'          'A'            0               NULL        NULL     'NO'    'BTREE'
      't1'           0  'i_t1_i2_s2'              2      's2'          'A'            0               NULL        NULL     'YES'   'BTREE'
      't1'           0  'u_t1_i3'                 1      'i3'          'A'            0               NULL        NULL     'YES'   'BTREE'
      't1'           0  'u_t1_s3'                 1      's3'          'A'            0               NULL        NULL     'YES'   'BTREE'
      't1'           1  'i_t1_i1'                 1      'i1'          NULL           0               NULL        NULL     'YES'   'BTREE'
      't1'           1  'i_t1_i1_s1'              1      'i1'          'A'            0               NULL        NULL     'YES'   'BTREE'
      't1'           1  'i_t1_i1_s1'              2      's1'          'A'            0               NULL        NULL     'YES'   'BTREE'
      't1'           1  'i_t1_s1'                 1      's1'          'A'            0                  7        NULL     'YES'   'BTREE'

.. _show-collation:
 
SHOW COLLATION 문
=================

**SHOW COLLATION** 문은 데이터베이스에서 지원하는 콜레이션 리스트를 출력한다. LIKE 절은 콜레이션 이름이 매칭되는 정보를 출력한다. 
해당 질의는 다음과 같은 칼럼을 가진다.

* Collation: 콜레이션 이름
* Charset: 문자셋 이름
* Id: 콜레이션 ID
* Built_in: 내장 콜레이션 여부. 내장 콜레이션들은 하드-코딩되어 있어 추가 혹은 삭제가 불가능하다.
* Expansions: 확장이 있는 콜레이션인지 여부. 자세한 내용은 :ref:`expansion` 참조하면 된다.
* Strength: 문자 간 비교를 위한 기준인데, 이 기준에 따라 문자 순서가 달라질 수 있다. 이에 대한 설명은 :ref:`collation-properties` 를 참고한다.

**SHOW COLLATION** 문의 구문과 예는 다음과 같다.

::

    SHOW COLLATION [ LIKE 'pattern' ]
    
.. code-block:: sql

    SHOW COLLATION;

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

    SHOW COLLATION LIKE '%_ko_%';
    
      Collation             Charset                        Id  Built_in              Expansions            Strength
    ===========================================================================================================================
      'utf8_ko_cs'          'utf8'                          7  'Yes'                 'No'                  'Not applicable'
      'utf8_ko_cs_uca'      'utf8'                        133  'No'                  'No'                  'Quaternary'

SHOW GRANTS 문
==============

**SHOW GRANT** 문은 데이터베이스의 사용자 계정에 부여된 권한을 출력한다. ::

    SHOW GRANTS FOR 'user'

.. code-block:: sql

    CREATE TABLE testgrant (id int);
    CREATE USER user1;
    GRANT INSERT,SELECT ON testgrant TO user1;
     
    SHOW GRANTS FOR user1;
    
      Grants for USER1
    ======================
      'GRANT INSERT, SELECT ON testgrant TO USER1'

SHOW CREATE TABLE 문
====================

**SHOW CREATE TABLE** 문은 테이블 이름을 지정하면 해당 테이블의 **CREATE TABLE** 문을 출력한다. ::

    SHOW CREATE TABLE table_name

.. code-block:: sql

    SHOW CREATE TABLE nation;
     
      TABLE                 CREATE TABLE
    ============================================
      'nation'              'CREATE TABLE [nation] ([code] CHARACTER(3) NOT NULL, [name] CHARACTER VARYING(40) NO
    T NULL, [continent] CHARACTER VARYING(10), [capital] CHARACTER VARYING(30),  CONSTRAINT [pk_nation_code] PRIM
    ARY KEY  ([code]))'

SHOW CREATE VIEW 문
===================

**SHOW CREATE VIEW** 문은 뷰 이름을 지정하면 해당 **CREATE VIEW** 문을 출력한다. ::

    SHOW CREATE VIEW view_name

다음은 해당 질의를 실행한 결과이다.

.. code-block:: sql

    SHOW CREATE VIEW db_class;
     
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

SHOW EXEC STATISTICS 문
=======================

**SHOW EXEC STATISTICS** 문은 실행한 질의들의 실행 통계 정보를 출력한다.

*   통계 정보 수집을 시작하려면 세션 변수 **@collect_exec_stats** 의 값을 1로 설정하며, 종료하려면 0으로 설정한다.

*   통계 정보 수집 결과를 출력한다.

    *   **SHOW EXEC STATISTICS** 는 data_page_fetches, data_page_dirties, data_page_ioreads, data_page_iowrites 이렇게 4가지 항목의 데이터 페이지 통계 정보를 출력하며, 결과 칼럼은 통계 정보 이름과 값에 해당하는 variable 칼럼과 value 칼럼으로 구성된다. **SHOW EXEC STATISTICS** 문을 실행하고 나면 그동안 누적되었던 통계 정보가 초기화된다.

    *   **SHOW EXEC STATISTICS ALL** 은 모든 항목의 통계 정보를 출력한다.

통계 정보 각 항목에 대한 자세한 설명은 :ref:`statdump` 을 참고한다.

::

    SHOW EXEC STATISTICS [ALL]

다음은 해당 질의를 실행한 결과이다.

.. code-block:: sql

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
