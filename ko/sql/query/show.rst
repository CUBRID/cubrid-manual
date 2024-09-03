
:meta-keywords: show statement, show tables, show columns, show index, show collation, show timezones, show grants

:tocdepth: 3


****
SHOW
****

.. contents::

DESC, DESCRIBE
==============

테이블의 칼럼 정보를 출력하며 **SHOW COLUMNS** 문과 같다. 보다 자세한 사항은 :ref:`show-columns-statement`\ 를 참고한다.

::

    DESC [schema_name.]table_name;
    DESCRIBE [schema_name.]table_name;

EXPLAIN
=======

테이블의 칼럼 정보를 출력하며 **SHOW COLUMNS** 문과 같다. 보다 자세한 사항은 :ref:`show-columns-statement`\ 를 참고한다.

::

    EXPLAIN [schema_name.]table_name;

.. _show-tables-statement:

SHOW TABLES
===========

데이터베이스의 전체 테이블 이름 목록을 출력한다. 결과 칼럼의 이름은 *tables_in_<데이터베이스 이름>* 이 되며 하나의 칼럼을 지닌다. **LIKE** 절을 사용하면 이와 매칭되는 테이블 이름을 검색할 수 있으며, **WHERE** 절을 사용하면 좀더 일반적인 조건으로 테이블 이름을 검색할 수 있다. **SHOW FULL TABLES** 는 *owner* 칼럼과 *table_type* 칼럼을 함께 출력한다. *owner* 컬럼은 소유자 이름을 값으로 가진다. *table_type* 칼럼은 테이블이면 **BASE TABLE**, 뷰이면 **VIEW**\를 값으로 가진다.

::

    SHOW [ FULL ] TABLES [ LIKE 'pattern' | WHERE expr ];

다음은 이 구문을 수행한 예이다.

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

테이블의 칼럼 정보를 출력한다. **LIKE** 절을 사용하면 이와 매칭되는 칼럼 이름을 검색할 수 있다. **WHERE** 절을 사용하면 "모든 **SHOW** 문에 대한 일반적인 고려 사항"과 같이 좀 더 일반적인 조건으로 칼럼 이름을 검색할 수 있다. 

::

    SHOW [FULL] COLUMNS (FROM | IN) [schema_name.]table_name [LIKE 'pattern' | WHERE expr];

**FULL** 키워드를 사용하면  **collation** 및 **comment** 를 추가로 출력한다.

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

다음은 이 구문을 수행한 예이다.

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
     'code'                 'INTEGER'             NULL                  'NO'                  'PRI'                 NULL                  'auto_increment'      NULL                
     'nation_code'          'CHAR(3)'             'iso88591_bin'        'YES'                 ''                    NULL                  ''                    NULL                

.. _show-index-statement:

SHOW INDEX
==========

인덱스 정보를 출력한다. 

::

    SHOW (INDEX | INDEXES | KEYS) (FROM | IN) [schema_name.]table_name;

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
Cardinality                         INTEGER         인덱스에서 유일한 값의 개수를 측정한 수치. 카디널리티가 높을수록 인덱스를 이용할 기회가 높아진다. 
                                                    이 값은 **SHOW INDEX** 가 실행되면 매번 업데이트된다. 이 값은 근사치임에 유의한다.
Sub_part                            INTEGER         칼럼의 일부만 인덱스된 경우 인덱스된 문자의 바이트 수. 칼럼 전체가 인덱스되면 **NULL** 이다.
Packed                                              키가 어떻게 팩되었는지(packed)를 나타냄. 팩되지 않은 경우 **NULL**. 현재 지원 안 함.
Null                                VARCHAR         칼럼이 **NULL** 을 포함할 수 있으면 YES, 그렇지 않으면 NO.
Index_type                          VARCHAR         사용되는 인덱스(현재 BTREE만 지원한다).
Func                                VARCHAR         함수 인덱스에서 사용되는 함수
Comment                             VARCHAR         인덱스를 설명하기 위한 주석
Visible                             VARCHAR         인덱스의 가시성을 보여준다 (YES/NO)
=================================== =============== ======================================================================================================================================

다음은 이 구문을 수행한 예이다.

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

데이터베이스에서 지원하는 콜레이션 리스트를 출력한다. LIKE 절은 콜레이션 이름이 매칭되는 정보를 출력한다. 

::

    SHOW COLLATION [LIKE 'pattern'];

해당 질의는 다음과 같은 칼럼을 가진다.

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
=================================== =============== ======================================================================================================================================
Collation                           VARCHAR         콜레이션 이름
Charset                             CHAR(1)         문자셋 이름
Id                                  INTEGER         콜레이션 ID
Built_in                            CHAR(1)         내장 콜레이션 여부. 내장 콜레이션들은 하드-코딩되어 있어 추가 혹은 삭제가 불가능하다.
Expansions                          CHAR(1)         확장이 있는 콜레이션인지 여부. 자세한 내용은 :ref:`expansion`\ 을 참조한다.
Strength                            CHAR(1)         문자 간 비교를 위한 기준. 이 기준에 따라 문자 순서가 달라질 수 있다. 
                                                    이에 대한 설명은 :ref:`collation-properties`\ 를 참고한다.
=================================== =============== ======================================================================================================================================

다음은 이 구문을 수행한 예이다.

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

현재 CUBRID에 설정된 타임 존 정보를 출력한다.

::

    SHOW [FULL] TIMEZONES [LIKE 'pattern'];

FULL이 명시되지 않으면 타임 존의 영역 이름을 가진 하나의 칼럼을 출력한다. 칼럼의 이름은 timezone_region이다.

FULL이 명시되면 4개의 칼럼을 가진 타임존 정보를 출력한다.

LIKE 절을 사용하면 이와 매칭되는  timezone_region 을 검색할 수 있다.

=================== =============== ===================================================
칼럼 이름           타입            설명
=================== =============== ===================================================
timezone_region     VARCHAR(32)     타임존 영역 이름
region_offset       VARCHAR(32)     일광 절약 시간을 고려하지 않은 타임존 영역의 오프셋
dst_offset          VARCHAR(32)     일광 절약 시간을 고려한 타임존 영역의 오프셋
dst_abbreviation    VARCHAR(32)     일광 절약 시간이 적용된 영역의 약어
=================== =============== ===================================================

두 번째, 세 번째, 네 번째 칼럼에서 출력되는 정보는 현재 날짜와 시간에 관한 것이다.

타임 존 영역이 일광 절약 시간(daylight saving time) 규칙을 적용하지 않는다면, dst_offset과 dst_abbreviation 값은 NULL 값이 된다.
 
현재의 날짜에 일광 절약 시간이 적용되지 않는다면 dst_offset 값은 0이 되고 dst_abbreviation 값은 빈 문자열이 된다.

WHERE 조건 없는 LIKE 조건은 첫 번째 칼럼에 적용된다. WHERE 조건은 결과를 필터링하기 위해 사용될 수 있다.

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

데이터베이스의 사용자 계정에 부여된 권한을 출력한다.

::

    SHOW GRANTS FOR user_name;
    
다음은 이 구문을 수행한 예이다.

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

테이블 이름을 지정하면 해당 테이블의 **CREATE TABLE** 문을 출력한다. ::

    SHOW CREATE TABLE [schema_name.]table_name;

.. code-block:: sql

    SHOW CREATE TABLE nation;
     
::

      TABLE                 CREATE TABLE
    ============================================
      'public.nation'       'CREATE TABLE [nation] ([code] CHARACTER(3) NOT NULL, [name] CHARACTER VARYING(40) NOT NULL, [continent] CHARACTER VARYING(10), [capital] CHARACTER VARYING(30), CONSTRAINT [pk_nation_code] PRIMARY KEY  ([code])) DONT_REUSE_OID, COLLATE iso88591_bin'

**SHOW CREATE TABLE** 문은 사용자가 입력한 구문을 그대로 출력하지는 않는다. 예를 들어, 사용자가 입력한 커멘트를 출력하지 않으며, 테이블 명이나 칼럼 명은 항상 소문자로 출력한다.
    
.. _show-create-view-statement:

SHOW CREATE VIEW
================

뷰 이름을 지정하면 해당 **CREATE VIEW** 문을 출력한다. ::

    SHOW CREATE VIEW view_name;

다음은 이 구문을 수행한 예이다.

.. code-block:: sql

    SHOW CREATE VIEW db_class;
     
::

      View                  Create View
    ============================================
      'db_class'            'SELECT [c].[class_name], CAST([c].[owner].[name] AS VARCHAR(255)), CASE [c].[class_type] WHEN 0 THEN 'CLASS' WHEN 1 THEN 'VCLASS' ELSE 'UNKNOW' END, CASE WHEN MOD([c].[is_system_class], 2) = 1 THEN 'YES' ELSE 'NO' END, CASE [c].[tde_algorithm] WHEN 0 THEN 'NONE' WHEN 1 THEN 'AES' WHEN 2 THEN 'ARIA' END, CASE WHEN [c].[sub_classes] IS NULL THEN 'NO' ELSE NVL((SELECT 'YES' FROM [_db_partition] [p] WHERE [p].[class_of] = [c] and [p].[pname] IS NULL), 'NO') END, CASE WHEN MOD([c].[is_system_class] / 8, 2) = 1 THEN 'YES' ELSE 'NO' END, [coll].[coll_name], [c].[comment] FROM [_db_class] [c], [_db_collation] [coll] WHERE [c].[collation_id] = [coll].[coll_id] AND (CURRENT_USER = 'DBA' OR {[c].[owner].[name]} SUBSETEQ(SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{[t].[g].[name]}), SET{}) FROM [db_user] [u], TABLE([groups]) AS [t]([g]) WHERE [u].[name] = CURRENT_USER) OR {[c]} SUBSETEQ ( SELECT SUM(SET{[au].[class_of]}) FROM [_db_auth] [au] WHERE {[au].[grantee].[name]} SUBSETEQ ( SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{[t].[g].[name]}), SET{}) FROM [db_user] [u], TABLE([groups]) AS [t]([g]) WHERE [u].[name] = CURRENT_USER) AND [au].[auth_type] = 'SELECT'))'

SHOW ACCESS STATUS 
==================
 
**SHOW ACCESS STATUS** 문은 데이터베이스 계정에 대한 로그인 정보를 출력한다. 이 명령은 데이터베이스 계정이 DBA인 사용자만 사용할 수 있다. 

:: 
  
    SHOW ACCESS STATUS [LIKE 'pattern' | WHERE expr]; 

해당 구문은 다음과 같은 칼럼을 출력한다.

=================== =========== =================================================================== 
칼럼 이름           타입          설명 
=================== =========== =================================================================== 
user_name           VARCHAR(32) DB 사용자 계정
last_access_time    DATETIME    DB 사용자가 마지막으로 접속한 시간 
last_access_host    VARCHAR(32) 마지막으로 접속한 호스트 
program_name        VARCHAR(32) 클라이언트 프로그램 이름(broker_cub_cas_1, csql ..) 
=================== =========== =================================================================== 
  
다음은 해당 구문을 실행한 결과이다. 
  
.. code-block:: sql 
  
    SHOW ACCESS STATUS; 
  
:: 
  
      user_name last_access_time last_access_host program_name 
    ============================================================================= 
      'DBA' 08:19:31.000 PM 02/10/2014 127.0.0.1 'csql' 
      'PUBLIC' NULL NULL NULL

.. note::

    SHOW ACCESS STATUS가 보여주는 로그인 정보는 데이터베이스가 재시작되면 초기화되며, HA 환경에서 복제되지 않으므로 각 노드마다 다른 결과를 보여준다.

.. _show-exec-statistics-statement:

SHOW EXEC STATISTICS
====================

실행한 질의의 실행 통계 정보를 출력한다.

*   통계 정보 수집을 시작하려면 세션 변수 **@collect_exec_stats** 의 값을 1로 설정하며, 종료하려면 0으로 설정한다.

*   통계 정보 수집 결과를 출력한다.

    *   **SHOW EXEC STATISTICS**\ 는 data_page_fetches, data_page_dirties, data_page_ioreads, data_page_iowrites 이렇게 4가지 항목의 데이터 페이지 통계 정보를 출력하며, 결과 칼럼은 통계 정보 이름과 값에 해당하는 variable 칼럼과 value 칼럼으로 구성된다. **SHOW EXEC STATISTICS** 문을 실행하고 나면 그동안 누적되었던 통계 정보가 초기화된다.

    *   **SHOW EXEC STATISTICS ALL**\ 은 모든 항목의 통계 정보를 출력한다.

통계 정보 각 항목에 대한 자세한 설명은 :ref:`statdump`\ 을 참고한다.

::

    SHOW EXEC STATISTICS [ALL];

다음은 이 구문을 수행한 예이다.

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

진단(Diagnostics)
=================

SHOW VOLUME HEADER
------------------

명시한 볼륨의 헤더 정보를 출력한다.

::

    SHOW VOLUME HEADER OF volume_id;
    
해당 구문은 다음과 같은 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
=================================== =============== ======================================================================================================================================
Volume_id                           INT             볼륨 식별자 
Magic_symbol                        VARCHAR(100)    볼륨 파일의 매직 값
Io_page_size                        INT             DB 볼륨의 페이지
Purpose                             VARCHAR(32)     볼륨 사용 목적 : '영구적 데이터 목적' 또는 '일시적 데이터 목적'
Type                                VARCHAR(32)     볼륨 타입, '영구적 볼륨' 또는 '일시적 볼륨'
Sector_size_in_pages                INT             페이지 내 섹터의 크기
Num_total_sectors                   INT             섹터 전체 개수
Num_free_sectors                    INT             여유 섹터 개수
Num_max_sectors                     INT             섹터 수의 최대값
Hint_alloc_sector                   INT             할당될 다음 섹터에 대한 힌트
Sector_alloc_table_size_in_pages    INT             페이지 내 섹터 할당 테이브 크기
Sector_alloc_table_first_page       INT             섹터 할당 테이블의 첫번째 페이지 
Page_alloc_table_size_in_pages      INT             페이지 내 페이지 할당 테이블의 크기
Page_alloc_table_first_page         INT             페이지 할당 테이블의 첫번째 페이지
Last_system_page                    INT             마지막 시스템 페이지
Creation_time                       DATETIME        데이터베이스 생성 시간
Db_charset                          INT             데이터베이스 문자셋번호 
Checkpoint_lsa                      VARCHAR(64)     이 볼륨의 복구 절차를 시작하는 가장 작은 로그 일련 주소
Boot_hfid                           VARCHAR(64)     다중 볼륨과 데이터베이스 기동을 위한 시스템 힙 파일ID
Full_name                           VARCHAR(255)    볼륨의 전체 경로
Next_volume_id                      INT             다음 볼륨의 ID
Next_vol_full_name                  VARCHAR(255)    다음 볼륨의 전체 경로
Remarks                             VARCHAR(64)     볼륨에 대한 설명
=================================== =============== ======================================================================================================================================

다음은 이 구문을 수행한 예이다.

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

활성 로그(active log) 파일의 헤더 정보를 출력한다.

::

    SHOW LOG HEADER [OF file_name];
    
OF file_name을 생략하면 메모리의 헤더 정보를 출력하며, OF file_name을 포함하면 file_name의 헤더 정보를 출력한다.

해당 구문은 다음의 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Volume_id                           INT             볼륨 식별자
Magic_symbol                        VARCHAR(32)     로그 파일의 매직 값
Magic_symbol_location               INT             로그 페이지의 매직 심볼 위치
Creation_time                       DATETIME        데이터베이스 생성 시간
Release                             VARCHAR(32)     CUBRID 릴리즈 버전
Compatibility_disk_version          VARCHAR(32)     현재 릴리즈 버전에 대한 DB의 호환성
Db_page_size                        INT             DB 페이지의 크기
Log_page_size                       INT             로그 페이지의 크기
Shutdown                            INT             로그 셧다운의 여부
Next_trans_id                       INT             다음 트랜잭션 ID
Num_avg_trans                       INT             평균 트랜잭션 개수
Num_avg_locks                       INT             평균 객체 잠금 개수
Num_active_log_pages                INT             활성로그 부분에서 페이지 개수
Db_charset                          INT             DB의 문자셋 번호
First_active_log_page               BIGINT          활성 로그에서 물리적 위치 1에 대한 논리 페이지 
Current_append                      VARCHAR(64)     현재의 추가된 위치 
Checkpoint                          VARCHAR(64)     복구 프로세스를 시작하는 가장 작은 로그 일련 주소 
Next_archive_page_id                BIGINT          보관할 다음 논리 페이지 
Active_physical_page_id             INT             보관할 논리 페이지의 물리직 위치 
Next_archive_num                    INT             다음 보관 로그 번호 
Last_archive_num_for_syscrashes     INT             시스템 비정상 종료 대비하여 필요한 최종 보관 로그 번호 
Last_deleted_archive_num            INT             최종 삭제된 보관 로그 번호 
Backup_lsa_level0                   VARCHAR(64)     백업 수준 0의 LSA(log sequence number)
Backup_lsa_level1                   VARCHAR(64)     백업 수준 1의 LSA 
Backup_lsa_level2                   VARCHAR(64)     백업 수준 2의 LSA
Log_prefix                          VARCHAR(256)    로그 prefix 이름
Has_logging_been_skipped            INT             로깅의 생략 여부
Perm_status                         VARCHAR(64)     현재 사용하지 않음
Backup_info_level0                  VARCHAR(128)    백업 수준 0의 상세 정보. 현재는 백업 시작 시간만 저장됨
Backup_info_level1                  VARCHAR(128)    백업 수준 1의 상세 정보. 현재는 백업 시작 시간만 저장됨
Backup_info_level2                  VARCHAR(128)    백업 수준 2의 상세 정보. 현재는 백업 시작 시간만 저장됨
Ha_server_state                     VARCHAR(32)     HA 서버 상태. 다음 값 중 하나: na, idle, active, to-be-active, standby, to-be-standby, maintenance, dead
Ha_file                             VARCHAR(32)     HA 복제 상태. 다음 값 중 하나: clear, archived, sync
Eof_lsa                             VARCHAR(64)     LSA 파일의 끝
Smallest_lsa_at_last_checkpoint     VARCHAR(64)     맨 마지막 체크포인트의 가장 작은 LSA, NULL 값이 될 수 있음
Next_mvcc_id                        BIGINT          다음 트랜잭션에서 사용될 다음 MVCCID 값 
Mvcc_op_log_lsa                     VARCHAR(32)     MVCC 작업을 위한 로그 항목을 연결하는 데 사용되는 LSA
Last_block_oldest_mvcc_id           BIGINT          로그 데이터 블록에서 가장 오래된 MVCC 를 찾기 위한 ID 값, NULL 값이 될 수 있음 
Last_block_newest_mvcc_id           BIGINT          로그 데이터 블록에서 가장 최신의 MVCC 를 찾기 위한 ID 값, NULL 값이 될 수 있음
=================================== =============== ======================================================================================================================================

다음은 이 구문을 수행한 예이다.

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

보관 로그(archive log) 파일의 헤더 정보를 출력한다.

::

    SHOW ARCHIVE LOG HEADER OF file_name;

해당 구문은 다음의 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
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

다음은 이 구문을 수행한 예이다.

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

명시한 테이블의 헤더 페이지를 출력한다. 

::

    SHOW  [ALL] HEAP HEADER OF [schema_name.]table_name;

*   ALL: 분할 테이블에서 "ALL" 키워드가 주어지면 기반 테이블과 분할 테이블이 같이 출력된다.

해당 구문은 다음의 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
=================================== =============== ======================================================================================================================================
Class_name                          VARCHAR(256)    테이블 이름
Class_oid                           VARCHAR(64)     포맷: (volid|pageid|slotid)
Volume_id                           INT             파일이 위치해 있는 볼륨의 식별자
File_id                             INT             파일 식별자
Header_page_id                      INT             첫 페이지 식별자(헤더 페이지)
Overflow_vfid                       VARCHAR(64)     오버플로우 파일 식별자(존재하는 경우)
Next_vpid                           VARCHAR(64)     다음 페이지 (예: 힙 파일의 두번째 페이지)
Unfill_space                        INT             페이지 공간이 이 값보다 작을 때 INSERT 중지. UPDATE 시에는 이 값을 사용 안 함
Estimates_num_pages                 BIGINT          힙 페이지 개수의 추정치
Estimates_num_recs                  BIGINT          힙 내 객체 개수의 추정치
Estimates_avg_rec_len               INT             레코드 전체 길이의 추정치
Estimates_num_high_best             INT             최소의 HEAP_DROP_FREE_SPACE를 가진 것으로 추정되는 베스트 페이지의 배열에 있는 페이지 개수. 

                                                    이 숫자가 0이고 최소한 다른 HEAP_NUM_BEST_SPACESTATS 개수만큼의 베스트 페이지가 있으면, 그것을 찾는다.
Estimates_num_others_high_best      INT             베스트 페이지로 알려진 것으로 추정되는 전체 개수. 

                                                    이 베스트 페이지는 베스트 배열에는 포함되어 있지 않고 최소한 HEAP_DROP_FREE_SPACE를 가진 것으로 추정한다.
Estimates_head                      INT             베스트 순환 배열의 헤드
Estimates_best_list                 VARCHAR(512)    포맷: '((best[0].vpid.volid|best[0].vpid.pageid), best[0].freespace), ... , ((best[9].vpid.volid|best[9].vpid.pageid), best[9].freespace)'
Estimates_num_second_best           INT             두번째 베스트 힌트의 개수. 이 힌트는 두번째 베스트 배열에 존재한다. 이들은 새로운 베스트 페이지를 찾을 때 사용됨.
Estimates_head_second_best          INT             두번째 베스트 힌트의 헤드의 인덱스. 새로운 두번째 베스트 힌트는 이 인덱스에 저장된다.
Estimates_num_substitutions         INT             페이지 대체(substitution) 개수. 새로운 두번째 베스트 페이지를 두번째 베스트 힌트로 입력하기 위해 사용된다.
Estimates_second_best_list          VARCHAR(512)    포맷: '(second_best[0].vpid.volid|second_best[0].vpid.pageid), ... , (second_best[9].vpid.volid|second_best[9].vpid.pageid)'
Estimates_last_vpid                 VARCHAR(64)     포맷: '(volid|pageid)'
Estimates_full_search_vpid          VARCHAR(64)     포맷: '(volid|pageid)'
=================================== =============== ======================================================================================================================================

다음은 이 구문을 수행한 예이다.

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

명시한 테이블의 용량을 출력한다. 

::

    SHOW [ALL] HEAP CAPACITY OF [schema_name.] table_name;

*   ALL: 분할 테이블에서 "ALL" 키워드가 주어지면 기반 테이블과 분할된 테이블이 같이 출력된다.

해당 구문은 다음의 칼럼을 출력한다.

=========================================== =============== ===============================================================================================================================
칼럼 이름                                   타입            설명                                                                                                                           
=========================================== =============== ===============================================================================================================================
Table_name                                  VARCHAR(256)    테이블 이름
Class_oid                                   VARCHAR(64)     힙 파일 식별자
Volume_id                                   INT             파일이 존재하는 볼륨 식별자       
File_id                                     INT             파일 식별자
Header_page_id                              INT             첫번째 페이지 식별자(헤더 페이지)                                                                                        
Num_recs                                    BIGINT          객체의 전체 개수
Num_relocated_recs                          BIGINT          재할당된 레코드의 개수                         
Num_overflowed_recs                         BIGINT          큰 레코드의 개수
Num_pages                                   BIGINT          힙 페이지의 전체 개수
Avg_rec_len                                 INT             평균 객체 길이
Avg_free_space_per_page                     INT             페이지 당 평균 여유 공간                    
Avg_free_space_per_page_without_last_page   INT             마지막 페이지를 고려하지 않은 페이지 당 평균 여유 공간
Avg_overhead_per_page                       INT             페이지 당 평균 오버헤드                       
Repr_id                                     INT             현재 캐시된 카탈로그 칼럼 정보  
Num_total_attrs                             INT             칼럼의 전체 개수
Num_fixed_width_attrs                       INT             고정 길이 칼럼의 개수                    
Num_variable_width_attrs                    INT             가변 길이 칼럼의 개수                     
Num_shared_attrs                            INT             공유(shared) 칼럼의 개수                          
Num_class_attrs                             INT             테이블 칼럼 개수 
Total_size_fixed_width_attrs                INT             고정 길이 칼럼의 전체 크기           
=========================================== =============== ===============================================================================================================================

다음은 이 구문을 수행한 예이다.

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

명시한 슬롯 페이지의 헤더 정보를 출력한다.

::

    SHOW SLOTTED PAGE HEADER (WHERE | OF) VOLUME = volume_num AND PAGE = page_num;

해당 구문은 다음의 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
=================================== =============== ======================================================================================================================================
Volume_id                           INT             페이지의 볼륨 식별자
Page_id                             INT             페이지 식별자
Num_slots                           INT             페이지에 할당된 슬롯 개수
Num_records                         INT             페이지에 대한 레코드 개수
Anchor_type                         VARCHAR(32)     다음 값 중 하나: 
                                                    
                                                    ANCHORED, ANCHORED_DONT_REUSE_SLOTS, UNANCHORED_ANY_SEQUENCE, UNANCHORED_KEEP_SEQUENCE
Alignment                           VARCHAR(8)      레코드에 대한 정렬(alignment), 다음 값 중 하나: CHAR, SHORT, INT, DOUBLE
Total_free_area                     INT             페이지 전체 여유 공간
Contiguous_free_area                INT             페이지 내 연속된 여유 공간
Free_space_offset                   INT             페이지의 처음부터 페이지 내 첫번째 여유 공간 바이트 영역까지의 바이트 오프셋
Need_update_best_hint               INT             undo 복구를 위해 저장이 필요하면 true
Is_saving                           INT             이 페이지를 위해 베스트 페이지를 업데이트해야 되면 true
Flags                               INT             페이지의 플래그 값 
=================================== =============== ======================================================================================================================================

다음은 이 구문을 수행한 예이다.

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
-----------------------

명시한 슬롯 페이지의 모든 슬롯 정보를 출력한다.

::

    SHOW SLOTTED PAGE SLOTS (WHERE | OF) VOLUME = volume_num AND PAGE = page_num;
    
해당 구문은 다음의 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
=================================== =============== ======================================================================================================================================
Volume_id                           INT             페이지의 볼륨 식별자
Page_id                             INT             페이지 식별자
Slot_id                             INT             슬롯 식별자
Offset                              INT             페이지의 시작부터 레코드의 시작까지의 바이트 오프셋
Type                                VARCHAR(32)     레코드 타입, 다음 값 중 하나: 

                                                    REC_UNKNOWN, REC_ASSIGN_ADDRESS, REC_HOME, REC_NEWHOME, REC_RELOCATION, REC_BIGONE, REC_MARKDELETED, REC_DELETED_WILL_REUSE
Length                              INT             레코드 길이
Waste                               INT             버릴 것인지 여부
=================================== =============== ======================================================================================================================================

다음은 이 구문을 수행한 예이다.

.. code-block:: sql

    -- csql> ;line on
    SHOW SLOTTED PAGE SLOTS OF VOLUME=0 AND PAGE=140;
    
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

특정 테이블 내 인덱스의 헤더 페이지 정보를 출력한다.

::

    SHOW INDEX HEADER OF [schema_name.]table_name.index_name;

ALL 키워드를 사용하고 인덱스 이름을 생략하면 해당 테이블의 전체 인덱스의 헤더 정보를 출력한다.

::

    SHOW ALL INDEXES HEADER OF [schema_name.]table_name;

해당 구문은 다음의 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
Column name                         Type            Description
=================================== =============== ======================================================================================================================================
Table_name                          VARCHAR(256)    테이블명 
Index_name                          VARCHAR(256)    인덱스명 
Btid                                VARCHAR(64)     BTID (volid|fileid|root_pageid)
Node_level                          INT             노드 수준 (1 은 단말, 2 이상은 비단말)
Max_key_len                         INT             서브트리의 최대 키 길이
Num_oids                            INT             B트리에 저당된 OID 개수 
Num_nulls                           INT             NULL 의 개수 
Num_keys                            INT             B트리에 있는 고유 키의 개수 
Topclass_oid                        VARCHAR(64)     최상위 클래스의 oid 또는  NULL OID (고유 인덱스가 아님)(volid|pageid|slotid)
Unique                              INT             고유값 유무 
Overflow_vfid                       VARCHAR(32)     VFID (volid|fileid)
Key_type                            VARCHAR(256)    타입명
Columns                             VARCHAR(256)    인덱스를 구성하는 칼럼 리스트 
=================================== =============== ======================================================================================================================================

다음은 이 구문을 수행한 예이다.

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

테이블의 인덱스 용량 정보를 출력한다.

::

    SHOW INDEX CAPACITY OF [schema_name.]table_name.index_name;

ALL 키워드를 사용하고 인덱스 이름을 생략하면 해당 테이블의 전체 인덱스의 용량 정보를 출력한다.

::

    SHOW ALL INDEXES CAPACITY OF [schema_name.]table_name;

해당 구문은 다음의 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
=================================== =============== ======================================================================================================================================
Table_name                          VARCHAR(256)    테이블 이름
Index_name                          VARCHAR(256)    인덱스 이름
Btid                                VARCHAR(64)     BTID (volid|fileid|root_pageid)
Num_distinct_key                    INT             단말 노드(leaf) 페이지의 Distinct key 개수
Total_value                         INT             트리에 저장된 값의 총 개수
Deduplicate_distinct_key            INT             단말 노드(leaf) 페이지의 Deduplicated Distinct key 개수
Num_fence_key                       INT             펜스키(Fence-key)의 개수
Avg_num_value_per_key               INT             키당 OID 값의 평균 개수
Avg_num_value_per_deduplicate_key   INT             Deduplicated된 키당 OID 값의 평균 개수
Num_leaf_page                       INT             단말 노드(leaf) 페이지 개수
Num_non_leaf_page                   INT             비단말(NonLeaf) 노드 페이지 개수
Num_ovf_page                        INT             단말 노드의 오버플로우 페이지 개수
Num_total_page                      INT             전체 페이지 개수
Height                              INT             트리의 높이
Avg_key_len                         INT             평균 키 길이
Avg_rec_len                         INT             평균 페이지 레코드 길이
Total_space                         VARCHAR(64)     인덱스에 의해 점유되는 전체 공간
Total_used_space_non_ovf            VARCHAR(64)     할당된 페이지에서 사용된 총 공간(단말 노드의 오버플로우 페이지는 제외)
Total_free_space_non_ovf            VARCHAR(64)     할당된 페이지에서 사용되지 않은 총 공간(단말 노드의 오버플로우 페이지는 제외)
Total_used_space_ovf                VARCHAR(64)     단말 노드의 할당된 오버플로우 페이지에서 사용된 총 공간
Total_free_space_ovf                VARCHAR(64)     단말 노드의 할당된 오버플로우 페이지에서 사용되지 않은 총 공간
Avg_num_key_per_page_non_ovf        INT             단말 노드 페이지에서 페이지 당 평균 키 개수
Avg_free_space_per_page_non_ovf     VARCHAR(64)     단말 노드 페이지에서 페이지 당 평균 여유 공간
Avg_num_key_per_page_ovf            INT             단말 노드의 오버플로우 페이지에서 페이지 당 평균 키 개수
Avg_free_space_per_page_ovf         VARCHAR(64)     단말 노드의 오버플로우 페이지 당 평균 여유 공간
Max_num_ovf_page_a_key              INT             하나의 키에 대해 연결된 단말 노드의 오버플로우 페이지의 최대 개수
=================================== =============== ======================================================================================================================================

.. note::

    Fence key는 B-tree 인덱스의 운용을 돕기 위해 단말 노드(leaf)에 추가되는 가상의 키이다.


다음은 이 구문을 수행한 예이다.

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

특정 데이터베이스의 전체 크리티컬 섹션(critical section, 이하 CS) 정보를 출력한다.

.. code-block:: sql

    SHOW CRITICAL SECTIONS;

해당 구문은 다음의 칼럼을 출력한다.

=================================== =============== ======================================================================================================================================
칼럼 이름                           타입            설명
=================================== =============== ======================================================================================================================================
Index                               INT             CS 인덱스 
Name                                VARCHAR(32)     CS 이름
Num_holders                         VARCHAR(16)     해당 CS 보유자의 개수. 다음 값 중 하나: 'N readers', '1 writer', 'none'
Num_waiting_readers                 INT             읽기 대기자의 개수
Num_waiting_writers                 INT             쓰기 대기자의 개수
Owner_thread_index                  INT             CS 쓰기 소유자의 쓰레드 인덱스. 소유자 없으면 NULL
Owner_tran_index                    INT             CS 쓰기 소유자의 트랜잭션 인덱스. 소유자 없으면 NULL
Total_enter_count                   BIGINT          진입자의 전체 개수
Total_waiter_count                  BIGINT          대기자의 전체 개수     
Waiting_promoter_thread_index       INT             승격 대기자의 쓰레드 인덱스. 승격 대기자 없으면 NULL
Max_waiting_msecs                   NUMERIC(10,3)   최대 대기 시간(밀리 초)
Total_waiting_msecs                 NUMERIC(10,3)   전체 대기 시간(밀리초)
=================================== =============== ======================================================================================================================================

다음은 이 구문을 수행한 예이다.

.. code-block:: sql

    SHOW CRITICAL SECTIONS;

::

    Index  Name                       Num_holders           Num_waiting_readers Num_waiting_writers  Owner_thread_index  Owner_tran_index     Total_enter_count Total_waiter_count  Waiting_promoter_thread_index  Max_waiting_msecs Total_waiting_msecs
    ============================================================================================================================================================================================================================================================
        0  'ER_LOG_FILE'              'none' 0                    0                NULL              NULL 217 0                           NULL  0.000                 0.000
        1  'ER_MSG_CACHE'             'none' 0                    0                NULL              NULL 0                     0                           NULL 0.000 0.000
        2  'WFG'                      'none' 0                    0                NULL              NULL 0                     0                           NULL 0.000 0.000
        3  'LOG'                      'none' 0                    0                NULL              NULL 11 0                           NULL  0.000                 0.000
        4  'LOCATOR_CLASSNAME_TABLE'  'none' 0                    0                NULL              NULL 33 0                           NULL  0.000                 0.000
        5  'QPROC_QUERY_TABLE'        'none' 0                    0                NULL              NULL 3                     0                           NULL 0.000 0.000
        6  'QPROC_LIST_CACHE'         'none' 0                    0                NULL              NULL 1                     0                           NULL 0.000 0.000
        7   'DISK_CHECK'              'none' 0                    0                NULL              NULL 3                     0                           NULL 0.000 0.000
        8  'CNV_FMT_LEXER'            'none' 0                    0                NULL              NULL 0                     0                           NULL 0.000 0.000
        9  'HEAP_CHNGUESS'            'none' 0                    0                NULL              NULL 10 0                           NULL  0.000                 0.000
        10  'TRAN_TABLE'              'none' 0                    0                NULL              NULL 7                     0                           NULL 0.000 0.000
        11  'CT_OID_TABLE'            'none' 0                    0                NULL              NULL 0                     0                           NULL 0.000 0.000
        12  'HA_SERVER_STATE'         'none' 0                    0                NULL              NULL 2                     0                           NULL 0.000 0.000
        13  'COMPACTDB_ONE_INSTANCE'  'none' 0                    0                NULL              NULL 0                     0                           NULL 0.000 0.000
        14  'ACL'                     'none' 0                    0                NULL              NULL 0                     0                           NULL 0.000 0.000
        15  'PARTITION_CACHE'         'none' 0                    0                NULL              NULL 1                     0                           NULL 0.000 0.000
        16  'EVENT_LOG_FILE'          'none' 0                    0                NULL              NULL 0                     0                           NULL 0.000 0.000
        17  'LOG_ARCHIVE'             'none' 0                    0                NULL              NULL 0                     0                           NULL 0.000 0.000
        18  'ACCESS_STATUS'           'none' 0                    0                NULL              NULL 1                     0                           NULL 0.000 0.000

SHOW TRANSACTION TABLES
-----------------------

각 트랜잭션을 관리하는 데이터 구조인 트랜잭션 디스크립터(transcation descriptor)의 내부 정보를 출력한다. 유효한 트랜잭션만 출력되므로, 출력되는 트랜잭션 디스크립터의 스냅샷이 일관되지 않을 수도 있다.

.. code-block:: sql

    SHOW { TRAN | TRANSACTION } TABLES [ WHERE expr ];

해당 구문은 다음의 칼럼을 출력한다.

======================== =============== ==============================================================================================================================================================
칼럼 이름                타입            설명
======================== =============== ==============================================================================================================================================================
Tran_index               INT             트랜잭션 테이블의 인덱스 또는 할당되지 않은 트랜잭션 슬롯일 경우 NULL 값 
Tran_id                  INT             트랜잭션 식별자 
Is_loose_end             INT             0 : 완료된 트랜잭션일 경우 , 1 : 완료되지 않은 트랜잭션
State                    VARCHAR(64)     트랜잭션의 상태. 다음 값 중 하나:

                                         'TRAN_RECOVERY', 'TRAN_ACTIVE', 'TRAN_UNACTIVE_COMMITTED', 'TRAN_UNACTIVE_WILL_COMMIT', 'TRAN_UNACTIVE_COMMITTED_WITH_POSTPONE', 

                                         'TRAN_UNACTIVE_ABORTED', 'TRAN_UNACTIVE_UNILATERALLY_ABORTED', 'TRAN_UNACTIVE_2PC_PREPARE', 'TRAN_UNACTIVE_2PC_COLLECTING_PARTICIPANT_VOTES',

                                         'TRAN_UNACTIVE_2PC_ABORT_DECISION', 'TRAN_UNACTIVE_2PC_COMMIT_DECISION', 'TRAN_UNACTIVE_COMMITTED_INFORMING_PARTICIPANTS', 

                                         'TRAN_UNACTIVE_ABORTED_INFORMING_PARTICIPANTS','TRAN_STATE_UNKNOWN'
Isolation                VARCHAR(64)     트랜잭션의 격리 수준. 다음 중 하나: 'SERIALIZABLE', 'REPEATABLE READ', 'COMMITTED READ', 'TRAN_UNKNOWN_ISOLATION'
Wait_msecs               INT             잠금 상태로 대기(milliseconds)
Head_lsa                 VARCHAR(64)     트랜잭션 로그의 처음 주소 
Tail_lsa                 VARCHAR(64)     트랜잭션 로그의 마지막 주소
Undo_next_lsa            VARCHAR(64)     UNDO  트랜잭션의 다음 로그 주소
Postpone_next_lsa        VARCHAR(64)     실행 될 연기된 레코드의 다음 로그 주소
Savepoint_lsa            VARCHAR(64)     마지막 세이브 포인트의 로그 주소
Topop_lsa                VARCHAR(64)     마지막 최상위 동작의 로그 주소 
Tail_top_result_lsa      VARCHAR(64)     마지막 부분 취소 또는 커밋의 로그 주소
Client_id                INT             클라이언트의 트랜잭션 고유 식별자
Client_type              VARCHAR(40)     클라이언트 타입. 다음 중 하나 값 

                                         'SYSTEM_INTERNAL', 'DEFAULT', 'CSQL', 'READ_ONLY_CSQL', 'BROKER', 'READ_ONLY_BROKER', 'SLAVE_ONLY_BROKER',

                                         'ADMIN_UTILITY', 'ADMIN_CSQL', 'LOG_COPIER', 'LOG_APPLIER', 'RW_BROKER_REPLICA_ONLY', 'RO_BROKER_REPLICA_ONLY', 

                                         'SO_BROKER_REPLICA_ONLY','ADMIN_CSQL_WOS', 'UNKNOWN'
Client_info              VARCHAR(256)    클라이언트의 정보 
Client_db_user           VARCHAR(40)     클라이언트의 데이터베이스 로그인 계정
Client_program           VARCHAR(256)    클라이언트의 프로그램명 
Client_login_user        VARCHAR(16)     클라이언트를 수행 중인 OS 로그인 계정 
Client_host              VARCHAR(64)     클라이언트의 호스트명
Client_pid               INT             클라이언트의 프로세스 id 
Topop_depth              INT             최상위 동작의 단계 
Num_unique_btrees        INT             unique_stat_info 배열에 포함된 고유한 btree 의 개수
Max_unique_btrees        INT             unique_stat_info_array 의 크기
Interrupt                INT             수행 중인 트랜잭션의 인터럽트 유무, 0 : 무, 1 : 유 
Num_transient_classnames INT             트랜잭션에 의해 임시 생성되는 클래스의 개수
Repl_max_records         INT             복제 레코드 배열의 크기
Repl_records             VARCHAR(20)     복제 레코드 버퍼 배열, 주소 포인터를 0x12345678 처럼 나타냄, NULL은 0x00000000 을 의미함
Repl_current_index       INT             복제 레코드의 현재 위치 
Repl_append_index        INT             추가 레코드의 현재 위치 
Repl_flush_marked_index  INT             플러시 표시된 복제 레코드의 인덱스
Repl_insert_lsa          VARCHAR(64)     쓰기 복제의 로그 주소
Repl_update_lsa          VARCHAR(64)     갱신 복제의 로그 주소
First_save_entry         VARCHAR(20)     트랜잭션의 처음 세이브 포인트 시작점. 주소 포인터를 0x12345678 처럼 나타냄, NULL은 0x00000000 을 의미함  
Tran_unique_stats        VARCHAR(20)     다중 열에 대한 로컬 통계 정보. 주소 포인터를 0x12345678 처럼 나타냄, NULL은 0x00000000 을 의미함
Modified_class_list      VARCHAR(20)     더티 클래쓰의 목록, 주소 포인터를 0x12345678 처럼 나타냄, NULL은 0x00000000 을 의미함
Num_temp_files           INT             임시 파일의 개수 
Waiting_for_res          VARCHAR(20)     대기 리소스, 주소 포인터를 0x12345678 처럼 나타냄, NULL은 0x00000000 을 의미함
Has_deadlock_priority    INT             데드락 우선순위 유무,  0 : 무, 1 : 유
Suppress_replication     INT             플래그가 세팅 될 때 복제 로그 쓰기를 생략 
Query_timeout            DATETIME        query_timeout 시간 내에 퀴리는 수행되어야 함. NULL일 경우 질의가 끝날 때 까지 기다림.
Query_start_time         DATETIME        질의 시작 시간,  질의 완료시 NULL
Tran_start_time          DATETIME        트랜잭션 시작 시간,  트랜잭션 완료시 NULL 
Xasl_id                  VARCHAR(64)     vpid:(volid|pageid),vfid:(volid|pageid), 질의 완료시 NULL
Disable_modifications    INT             0보다 클 경우 수정을 금지 
Abort_reason             VARCHAR(40)     트랜잭션 중지 사유, 다음 중 하나 

                                         'NORMAL', 'ABORT_DUE_TO_DEADLOCK', 'ABORT_DUE_ROLLBACK_ON_ESCALATION'
======================== =============== ==============================================================================================================================================================

다음은 이 구문을 수행한 예이다.


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

각 스레드의 내부 정보를 출력한다. 반환 결과는 "Index" 칼럼의 오름차순으로 정렬되며, 출력되는 스레드 엔트리의 스냅샷이 일관되지 않을 수도 있다.
SA MODE일 경우 이 구문은 아무런 결과도 출력하지 않는다.

.. code-block:: sql

    SHOW THREADS [ WHERE expr ];

해당 구문은 다음의 칼럼을 출력한다.

=========================== =============== ==============================================================================================================================================================
칼럼명                      타입            설영
=========================== =============== ==============================================================================================================================================================
Index                       INT             쓰레드 시작 인덱스
Jobq_index                  INT             워커 쓰레드의 작업 큐 인덱스.  워커 쓰레드가 아닌 경우 NULL 
Thread_id                   BIGINT          쓰레드 식별자
Tran_index                  INT             쓰레드가 속한 트랜잭션 인덱스. 관련 쓰레드가 없을 경우 NULL
Type                        VARCHAR(8)      쓰레드 종류. 다음 중 하나 'MASTER', 'SERVER', 'WORKER', 'DAEMON', 'VACUUM_MASTER', 'VACUUM_WORKER', 'NONE', 'UNKNOWN'.
Status                      VARCHAR(8)      쓰레드 상태. 다음 중 하나 'FREE', 'RUN', 'WAIT', 'CHECK'.
Resume_status               VARCHAR(32)     재시작 상태. 다음 중 하나 'RESUME_NONE', 'RESUME_DUE_TO_INTERRUPT', 'RESUME_DUE_TO_SHUTDOWN', 'PGBUF_SUSPENDED', 'PGBUF_RESUMED', 
                                            'JOB_QUEUE_SUSPENDED', 'JOB_QUEUE_RESUMED', 'CSECT_READER_SUSPENDED', 'CSECT_READER_RESUMED', 'CSECT_WRITER_SUSPENDED', 'CSECT_WRITER_RESUMED',
                                            'CSECT_PROMOTER_SUSPENDED', 'CSECT_PROMOTER_RESUMED', 'CSS_QUEUE_SUSPENDED', 'CSS_QUEUE_RESUMED', 'QMGR_ACTIVE_QRY_SUSPENDED', 'QMGR_ACTIVE_QRY_RESUMED',
                                            'QMGR_MEMBUF_PAGE_SUSPENDED', 'QMGR_MEMBUF_PAGE_RESUMED', 'HEAP_CLSREPR_SUSPENDED', 'HEAP_CLSREPR_RESUMED', 'LOCK_SUSPENDED', 'LOCK_RESUMED', 
                                            'LOGWR_SUSPENDED', 'LOGWR_RESUMED'
Net_request                 VARCHAR(64)     net_requests 배열의 요청 이름, 예: 'LC_ASSIGN_OID'. 요청 이름이 없을 경우  NULL  
Conn_client_id              INT             쓰레드에 응답하는 클라이언트의 식별자, 클라이언트의 식별자가 없을 경우 NULL 
Conn_request_id             INT             쓰레드가 처리하고 있는 요청의 식별자, 요청 식별자가 없을 경우 NULL 
Conn_index                  INT             연결 인덱스, 없을 경우 NULL
Last_error_code             INT             마지막 에러 코드 
Last_error_msg              VARCHAR(256)    마지막 에러 메세지, 메세지가 256 자 보다 클 경우 일부만 보인다. 에러 메세지가 없을 경우 NULL
Private_heap_id             VARCHAR(20)     쓰레드 내부 메모리 할당자의 주소, 예: 0x12345678. 관련 힙 id 가 없을 경우 NULL
Query_entry                 VARCHAR(20)     QMGR_QUERY_ENTRY의 주소 , 예: 0x12345678,  연관된 QMGR_QUERY_ENTRY 가 없을 경우 NULL.
Interrupted                 INT             요청/트랜잭션의 인터럽트 유/무 0 또는 1
Shutdown                    INT             서버의 중지 진행 여/부, 0 또는 1
Check_interrupt             INT             0 또는 1
Wait_for_latch_promote      INT             0 또는 1, 쓰레드가 래치 프로모션(latch promotion)을 대기하는 여/부 
Lockwait_blocked_mode       VARCHAR(24)     잠금대기 블록 모드, 다음 중 하나. 'NULL_LOCK', 'IS_LOCK', 'S_LOCK', 'IS_LOCK', 'IX_LOCK', 'SIX_LOCK', 'X_LOCK', 'SCH_M_LOCK', 'UNKNOWN'
Lockwait_start_time         DATETIME        차단이 시작된 시간, 차단 상태 아닌 경우 NULL
Lockwait_msecs              INT             차단되었던 시간(milliseconds), 차단된 상태가 아닌 경우 NULL
Lockwait_state              VARCHAR(24)     잠금 대기 상태 예: 'SUSPENDED', 'RESUMED', 'RESUMED_ABORTED_FIRST', 'RESUMED_ABORTED_OTHER', 'RESUMED_DEADLOCK_TIMEOUT', 'RESUMED_TIMEOUT', 
                                            'RESUMED_INTERRUPT'. 블록 된 상태가 없을 경우  NULL
Next_wait_thread_index      INT             다음 대기 쓰레드 인덱스, 없을 경우 NULL
Next_tran_wait_thread_index INT             잠금 매니저의 다음 대기 쓰레드 인덱스, 없을 경우 NULL
Next_worker_thread_index    INT             css_Job_queue.worker_thrd_list 의 다음 워커 쓰레드 인덱스, 없을 경우 NULL
=========================== =============== ==============================================================================================================================================================

다음은 이 구문을 수행한 예이다.

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

작업 큐의 상태를 보여준다. SA MODE일 때에 이 문은 아무 결과도 보여주지 않는다.

.. code-block:: sql

    SHOW JOB QUEUES;

이 질의는 다음의 칼럼들을 출력한다:

=========================== =============== =======================================================
칼럼명                      타입            설명
=========================== =============== =======================================================
Jobq_index                  INT             작업 큐의 인덱스
Num_total_workers           INT             큐의 워커 쓰레드 총 개수 
Num_busy_workers            INT             큐의 활성 워커 쓰레드의 개수 
Num_connection_workers      INT             큐의 연결(connection) 워커 쓰레드의 수
=========================== =============== =======================================================

SHOW PAGE BUFFER STATUS
-----------------------

데이터 페이지 버퍼의 상태를 출력한다.

.. code-block:: sql

    SHOW PAGE BUFFER STATUS;

해당 구문은 다음의 칼럼을 출력한다.

=========================== =============== ===============================================================
칼럼 이름                   타입            설명
=========================== =============== ===============================================================
Hit_rate                    NUMERIC(13,10)  데이터 버퍼의 페이지 적중률 (이전 출력 이후)
Num_hit                     BIGINT          데이터 버퍼의 페이지 적중 수 (이전 출력 이후)
Num_page_request            BIGINT          데이터 버퍼에 페이지 요청 수 (이전 출력 이후)
Pool_size                   INT             데이터 버퍼의 전체 페이지 수
Page_size                   INT             데이터 버퍼의 단일 페이지 크기
Free_pages                  INT             데이터 버퍼의 여유 페이지 수
Victim_candidate_pages      INT             데이터 버퍼의 희생자(victim) 후보 페이지 수
Clean_pages                 INT             데이터 버퍼의 갱신되지 않은 페이지 수
Dirty_pages                 INT             데이터 버퍼의 갱신된 페이지 수
Num_index_pages             INT             데이터 버퍼의 인덱스 페이지 수
Num_data_pages              INT             데이터 버퍼의 데이터 페이지 수
Num_system_pages            INT             데이터 버퍼의 시스템 페이지 수
Num_temp_pages              INT             데이터 버퍼의 임시 페이지 수
Num_pages_created           BIGINT          데이터 버퍼에서 새롭게 생성된 페이지 수 (이전 출력 이후)
Num_pages_written           BIGINT          데이터 버퍼에서 디스크로 쓰여진 페이지 수 (이전 출력 이후)
Pages_written_rate          NUMERIC(20,10)  데이터 버퍼에서 디스크로 초당 쓰여진 페이지 수 (이전 출력 이후)
Num_pages_read              BIGINT          데이터 버퍼로 디스크에서 읽은 페이지 수 (이전 출력 이후)
Pages_read_rate             NUMERIC(20,10)  데이터 버퍼로 디스크에서 초당 읽은 페이지 수 (이전 출력 이후)
Num_flusher_waiting_threads INT             데이터 버퍼의 페이지 할당을 대기하는 쓰레드 수
=========================== =============== ===============================================================

다음은 이 구문을 수행한 예이다.

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

