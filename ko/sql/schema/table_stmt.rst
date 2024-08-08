
:meta-keywords: table definition, create table, drop table, alter table, column definition, constraint definition, create table like, create table as select, rename table
:meta-description: Define tables in CUBRID database using create table, alter table, drop table and rename table statements.

*************
테이블 정의문
*************

CREATE TABLE
============

테이블 정의
-----------

**CREATE TABLE** 문을 사용하여 새로운 테이블을 생성한다. 기존 뷰 또는 동의어와 이름이 같은 테이블은 생성할 수 없다. 테이블 이름 작성 원칙은 :doc:`/sql/identifier`\ 를 참고한다.

.. CUBRIDSUS-12366: from 10.0, create table if not exists ...

::

    CREATE {TABLE | CLASS} [IF NOT EXISTS] [schema_name.]table_name
    [<subclass_definition>]
    [CLASS ATTRIBUTE (<column_definition>, ...)]
    [([{<table_constraint>}... ,] <column_definition> [{, {<column_definition> | <table_constraint>}}...])]
    [INHERIT <resolution>, ...]
    [<table_options>]

        <subclass_definition> ::= {UNDER | AS SUBCLASS OF} [schema_name.]superclass_name, ...
        
        <column_definition> ::= 
            column_name <data_type> [{<default_or_shared_or_ai> | <on_update> | <column_constraint>}] [COMMENT 'column_comment_string']
        
            <data_type> ::= <column_type> [<charset_modifier_clause>] [<collation_modifier_clause>]

                <charset_modifier_clause> ::= {CHARACTER_SET|CHARSET} {<char_string_literal>|<identifier>}

                <collation_modifier_clause> ::= COLLATE {<char_string_literal>|<identifier>}
            
            <default_or_shared_or_ai> ::=
                SHARED <value_specification> | 
                DEFAULT <value_specification>  |
                AUTO_INCREMENT [(seed, increment)]

                <on_update> ::= [ON UPDATE <value_specification>]
         
            <column_constraint> ::= [CONSTRAINT constraint_name] { NOT NULL | UNIQUE | PRIMARY KEY | [FOREIGN KEY] [WITH <index_with_option>] <referential_definition> }
                        
        <table_constraint> ::=             
            { 
                {KEY|INDEX} index_name  <index_col_desc> |
                [CONSTRAINT [constraint_name]]
                   {
                      UNIQUE [KEY|INDEX](column_name, ...) |
                      PRIMARY KEY (column_name, ...) |
                      <referential_constraint>
                   }
            } COMMENT 'index_comment_string'

          <index_col_desc> ::=
              { ( {column_name | function_name (argument_list)} [ASC | DESC] [{, {column_name | function_name (argument_list)} [ASC | DESC]} ...] ) }
              [WHERE <filter_predicate>]
              [WITH <index_with_option>]
              [INVISIBLE]
              [COMMENT 'index_comment_string’]
         
            <referential_constraint> ::= FOREIGN KEY [<foreign_key_name>](column_name, ...) [WITH <index_with_option>] <referential_definition>
         
                <referential_definition> ::=
                    REFERENCES [schema_name.]referenced_table_name (column_name, ...) [<referential_triggered_action> ...]
         
                    <referential_triggered_action> ::=
                        ON UPDATE <referential_action> |
                        ON DELETE <referential_action> 
        
                        <referential_action> ::= CASCADE | RESTRICT | NO ACTION | SET NULL
     
        <resolution> ::= [CLASS] {column_name} OF [schema_name.]superclass_name [AS alias]
        <table_options> ::= <table_option> [[,] <table_option> ...] 
            <table_option> ::= REUSE_OID | DONT_REUSE_OID |
                               COMMENT [=] 'table_comment_string' |
                               [CHARSET charset_name] [COLLATE collation_name] |
                               ENCRYPT [=] [AES | ARIA] | 
                               AUTO_INCREMENT = initial_value

      <index_with_option> ::= {DEDUPLICATE ‘=‘ deduplicate_level }

*   **IF NOT EXISTS**: 생성하려는 테이블이 존재하는 경우 에러 없이 테이블을 생성하지 않는다.
*   *schema_name*: 스키마 이름을 지정한다(최대 31바이트). 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 생성할 테이블의 이름을 지정한다(최대 222바이트).
*   *column_name*: 생성할 칼럼의 이름을 지정한다(최대 254바이트).
*   *column_type*: 칼럼의 데이터 타입을 지정한다.
*   [**SHARED** *value* | **DEFAULT** *value*]: 칼럼의 초기값을 지정한다.
*   **ON UPDATE**: 레코드의 필드가 수정되었을 때 갱신될 필드에 대한 수식을 지정한다. 자세한 내용은 :ref:`constraint-definition` 을 참고한다.
*   <*column_constraint*>: 칼럼의 제약 조건을 지정하며 제약 조건의 종류에는 **NOT NULL**, **UNIQUE**, **PRIMARY KEY**, **FOREIGN KEY** 가 있다.
*   <*default_or_shared_or_ai*>: DEFAULT, SHARED, AUTO_INCREMENT 중 하나만 사용될 수 있다.
    AUTO_INCREMENT이 지정될 때 "(seed, increment)"와 "AUTO_INCREMENT = initial_value"는 동시에 정의될 수 없다.
*   *table_comment_string*: 테이블의 커멘트를 지정한다.
*   *column_comment_string*: 칼럼의 커멘트를 지정한다.
*   *index_comment_string*: 인덱스의 커멘트를 지정한다.
*   *deduplicate_level*: deduplicate 레벨을 지정한다(0 ~ 14). 자세한 내용은 :ref:`deduplicate_overview`\를 참고한다.

.. note::

    *   **DBA**\와 **DBA** 멤버는 다른 스키마에 테이블을 생성할 수 있다. 사용자가 **DBA**\도 아니고 **DBA** 멤버도 아니면 해당 사용자의 스키마에서만 테이블을 생성할 수 있다.

.. note::

    *deduplicate_level*\은 0부터 14까지의 정수이다. 0은 **DEDUPLICATE** 옵션이 없었던 CUBRID 11.2 또는 이하 버전과 동일한 구성의 인덱스를 의미한다.

.. code-block:: sql

    CREATE TABLE olympic2 (
        host_year        INT    NOT NULL PRIMARY KEY,
        host_nation      VARCHAR(40) NOT NULL,
        host_city        VARCHAR(20) NOT NULL,
        opening_date     DATE        NOT NULL,
        closing_date     DATE        NOT NULL,
        mascot           VARCHAR(20),
        slogan           VARCHAR(40),
        introduction     VARCHAR(1500)
    );

다음은 ALTER 문을 사용하여 테이블 커멘트를 추가하는 예제이다.

.. code-block:: sql
    
    ALTER TABLE olympic2 COMMENT = 'this is new comment for olympic2';

다음은 테이블 생성 시 인덱스 커멘트를 포함하는 예제이다.

.. code-block:: sql

    CREATE TABLE tbl (a INT, index i_t_a (a) COMMENT 'index comment');

.. note:: **테이블 스키마의 CHECK 제약 조건**

    테이블 스키마에 정의된 CHECK 제약 조건은 파싱되지만, 실제 동작은 무시된다. 파싱되는 이유는 타 DBMS로부터 마이그레이션을 진행하는 경우 호환성을 제공하기 위해서이다.
    
    .. code-block:: sql
    
        CREATE TABLE tbl (
            id INT PRIMARY KEY,
            CHECK (id > 0)
        )

.. _column-definition:

칼럼 정의
---------

칼럼은 테이블에서 각 열에 해당하는 항목이며, 칼럼은 칼럼 이름과 데이터 타입을 명시하여 정의한다. 

::

    <column_definition> ::= 
        column_name <data_type> [[<default_or_shared_or_ai>] | [<on_update>] | [<column_constraint>]] ... [COMMENT 'comment_string']
    
        <data_type> ::= <column_type> [<charset_modifier_clause>] [<collation_modifier_clause>]

            <charset_modifier_clause> ::= {CHARACTER_SET|CHARSET} {<char_string_literal>|<identifier>}

            <collation_modifier_clause> ::= COLLATE {<char_string_literal>|<identifier>}
        
        <default_or_shared_or_ai> ::=
            SHARED <value_specification> | 
            DEFAULT <value_specification>  |
            AUTO_INCREMENT [(seed, increment)]

        <on_update> ::= [ON UPDATE <value_specification>]

        <column_constraint> ::= [CONSTRAINT constraint_name] {NOT NULL | UNIQUE | PRIMARY KEY | [FOREIGN KEY] [WITH <index_with_option>] <referential_definition>}

칼럼 이름
^^^^^^^^^

칼럼 이름 작성 원칙은 :doc:`/sql/identifier` 절을 참고한다. 생성한 칼럼의 이름은 **ALTER TABLE** 문의 :ref:`rename-column` 을 사용하여 변경할 수 있다. 

다음은 *full_name* 과 *age*, 2개의 칼럼을 가지는 *manager2* 테이블을 생성하는 예제이다.

.. code-block:: sql

    CREATE TABLE manager2 (full_name VARCHAR(40), age INT );

.. note::

    *   칼럼 이름의 첫 글자는 반드시 알파벳이어야 한다.
    *   칼럼 이름은 테이블 내에서 고유해야 한다.

칼럼의 초기 값 설정(SHARED, DEFAULT)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

테이블의 칼럼의 초기값을 **SHARED** 또는 **DEFAULT** 값을 통해 정의할 수 있다. **SHARED**, **DEFAULT** 값은 **ALTER TABLE** 문에서 변경할 수 있다.

*   **SHARED** : 칼럼 값은 모든 행에서 동일하다. 따라서 **SHARED** 속성은 **UNIQUE** 제약 조건과 동시에 정의할 수 없다. 초기에 설정한 값과 다른 새로운 값을 **INSERT** 하면, 해당 칼럼 값은 모든 행에서 새로운 값으로 갱신된다.
*   **DEFAULT** : 새로운 행을 삽입할 때 칼럼 값을 지정하지 않으면 **DEFAULT** 속성으로 설정한 값이 저장된다.

**DEFAULT** 의 값으로 허용되는 의사 칼럼(pseudocolumn)과 함수는 다음과 같다.

+-------------------------------+---------------+
| DEFAULT 값                    | 데이터 타입   |
+===============================+===============+
| SYS_TIMESTAMP                 | TIMESTAMP     |
+-------------------------------+---------------+
| UNIX_TIMESTAMP()              | INTEGER       |
+-------------------------------+---------------+
| CURRENT_TIMESTAMP             | TIMESTAMP     |
+-------------------------------+---------------+
| SYS_DATETIME                  | DATETIME      |
+-------------------------------+---------------+
| CURRENT_DATETIME              | DATETIME      |
+-------------------------------+---------------+
| SYS_DATE                      | DATE          |
+-------------------------------+---------------+
| CURRENT_DATE                  | DATE          |
+-------------------------------+---------------+
| SYS_TIME                      | TIME          |
+-------------------------------+---------------+
| CURRENT_TIME                  | TIME          |
+-------------------------------+---------------+
| USER, USER()                  | STRING        |
+-------------------------------+---------------+
| TO_CHAR(date_time[, format])  | STRING        |
+-------------------------------+---------------+
| TO_CHAR(number[, format])     | STRING        |
+-------------------------------+---------------+

.. note::

    CUBRID 9.0 미만 버전에서는 테이블 생성 시 **DATE**, **DATETIME**, **TIME**, **TIMESTAMP** 칼럼의 **DEFAULT** 값을 **SYS_DATE**, **SYS_DATETIME**, **SYS_TIME**, **SYS_TIMESTAMP** 로 지정하면, **CREATE TABLE** 시점의 값이 저장되었다. 따라서 CUBRID 9.0 미만 버전에서 데이터가 **INSERT** 되는 시점의 값을 입력하려면 **INSERT** 구문의 **VALUES** 절에 해당 함수를 입력해야 한다.

.. code-block:: sql

    CREATE TABLE colval_tbl
    (id INT, name VARCHAR SHARED 'AAA', phone VARCHAR DEFAULT '000-0000');
    INSERT INTO colval_tbl (id) VALUES (1), (2);
    SELECT * FROM colval_tbl;
    
::
     
               id  name                  phone
    =========================================================
                1  'AAA'                 '000-0000'
                2  'AAA'                 '000-0000'
     
.. code-block:: sql

    --updating column values on every row
    INSERT INTO colval_tbl(id, name) VALUES (3,'BBB');
    INSERT INTO colval_tbl(id) VALUES (4),(5);
    SELECT * FROM colval_tbl;
     
::

               id  name                  phone
    =========================================================
                1  'BBB'                 '000-0000'
                2  'BBB'                 '000-0000'
                3  'BBB'                 '000-0000'
                4  'BBB'                 '000-0000'
                5  'BBB'                 '000-0000'
     
.. code-block:: sql

    --changing DEFAULT value in the ALTER TABLE statement
    ALTER TABLE colval_tbl MODIFY phone VARCHAR DEFAULT '111-1111';
    INSERT INTO colval_tbl (id) VALUES (6);
    SELECT * FROM colval_tbl;
     
::

               id  name                  phone
    =========================================================
                1  'BBB'                 '000-0000'
                2  'BBB'                 '000-0000'
                3  'BBB'                 '000-0000'
                4  'BBB'                 '000-0000'
                5  'BBB'                 '000-0000'
                6  'BBB'                 '111-1111'

.. code-block:: sql

    --use DEFAULT TO_CHAR in CREATE TABLE statement
    CREATE TABLE t1(id1 INT, id2 VARCHAR(20) DEFAULT TO_CHAR(12345,'S999999'));
    INSERT INTO t1 (id1) VALUES (1);
    SELECT * FROM t1;

::

              id1  id2
    ===================================
                1  ' +12345'

하나 이상의 칼럼에 의사 칼럼의 **DEFAULT** 값 지정이 가능하다.

.. code-block:: sql

    CREATE TABLE tbl (date1 DATE DEFAULT SYSDATE, date2 DATE DEFAULT SYSDATE);
    CREATE TABLE tbl (date1 DATE DEFAULT SYSDATE,
                      ts1   TIMESTAMP DEFAULT CURRENT_TIMESTAMP);
    CREATE TABLE t1(id1 INT, id2 VARCHAR(20) DEFAULT TO_CHAR(12345,'S999999'), id3 VARCHAR(20) DEFAULT TO_CHAR(SYS_TIME, 'HH24:MI:SS'));
    ALTER TABLE t1 add column id4 varchar (20) default TO_CHAR(SYS_DATETIME, 'yyyy/mm/dd hh:mi:ss'), id5 DATE DEFAULT SYSDATE;

자동 증가
^^^^^^^^^

칼럼 값에 자동으로 일련 번호를 부여하기 위해 칼럼에 **AUTO_INCREMENT** 속성을 정의할 수 있다. **SMALLINT**, **INTEGER**, **BIGINT**, **NUMERIC** ( *p* , 0) 타입에 한정하여 정의할 수 있다.

동일한 칼럼에 **AUTO_INCREMENT** 속성과 **SHARED** 또는 **DEFAULT** 속성을 동시에 정의할 수 없으며, 사용자가 직접 입력한 값과 자동 증가 특성에 의해 입력된 값이 서로 충돌되지 않도록 주의해야 한다.

**AUTO_INCREMENT** 의 초기값은 **ALTER TABLE** 문을 이용하여 바꿀 수 있다. 자세한 내용은 **ALTER TABLE** 의 :ref:`alter-auto-increment` 을 참고한다.

::

    CREATE TABLE [schema_name.]table_name (id INT AUTO_INCREMENT[(seed, increment)]);

    CREATE TABLE [schema_name.]table_name (id INT AUTO_INCREMENT) AUTO_INCREMENT = seed ;

*   *seed*: 번호가 시작하는 초기값이다. 모든 정수가 허용되며 기본값은 **1** 이다.
*   *increment*: 행마다 증가되는 증가값이다. 양의 정수만 허용되며 기본값은 **1** 이다.

**CREATE TABLE** *[schema_name.]table_name* (id int **AUTO_INCREMENT**) **AUTO_INCREMENT** = *seed*; 구문을 사용할 때에는 다음과 같은 제약 사항이 있다.

*   **AUTO_INCREMENT** 속성을 갖는 칼럼은 하나만 정의해야 한다.
*   (*seed*, *increment*)와 **AUTO_INCREMENT** = *seed* 는 같이 사용하지 않는다.

.. code-block:: sql

    CREATE TABLE auto_tbl (id INT AUTO_INCREMENT, name VARCHAR);
    INSERT INTO auto_tbl VALUES (NULL, 'AAA'), (NULL, 'BBB'), (NULL, 'CCC');
    INSERT INTO auto_tbl (name) VALUES ('DDD'), ('EEE');
    SELECT * FROM auto_tbl;
     
::

               id  name
    ===================================
                1  'AAA'
                2  'BBB'
                3  'CCC'
                4  'DDD'
                5  'EEE'
     
.. code-block:: sql

    CREATE TABLE tbl (id INT AUTO_INCREMENT, val string) AUTO_INCREMENT = 3;
    INSERT INTO tbl VALUES (NULL, 'cubrid');
     
    SELECT * FROM tbl;
    
::

               id  val
    ===================================
                3  'cubrid'
     
.. code-block:: sql

    CREATE TABLE t (id INT AUTO_INCREMENT, id2 int AUTO_INCREMENT) AUTO_INCREMENT = 5;
    
::
    
    ERROR: To avoid ambiguity, the AUTO_INCREMENT table option requires the table to  have exactly one AUTO_INCREMENT column and no seed/increment specification.
     
.. code-block:: sql

    CREATE TABLE t (i INT AUTO_INCREMENT(100, 2)) AUTO_INCREMENT = 3;
    
::

    ERROR: To avoid ambiguity, the AUTO_INCREMENT table option requires the table to  have exactly one AUTO_INCREMENT column and no seed/increment specification.

.. note::

    *   자동 증가 특성만으로는 **UNIQUE** 제약 조건을 가지지 않는다.
    *   자동 증가 특성이 정의된 칼럼에 **NULL** 을 입력하면 자동 증가된 값이 저장된다.
    *   자동 증가 특성이 정의된 칼럼에 값을 직접 입력해도 AUTO_INCREMENT 값은 변하지 않는다.
    *   자동 증가 특성이 정의된 칼럼에 **SHARED** 또는 **DEFAULT** 속성을 설정할 수 없다.
    *   초기값 및 자동 증가 특성에 의해 증가된 최종 값은 해당 타입에서 허용되는 최소/최대값을 넘을 수 없다.
    *   자동 증가 특성은 순환되지 않으므로 타입의 최대값을 넘어갈 경우 오류가 발생하며, 이에 대한 롤백이 일어나지 않는다. 따라서 이와 같은 경우 해당 칼럼을 삭제 후 다시 생성해야 한다. 

        예를 들어, 아래와 같이 테이블을 생성했다면, A의 최대값은 32767이다. 32767이 넘어가는 경우 에러가 발생하므로, 초기 테이블 생성시에 칼럼 A의 최대값이 해당 타입의 최대값을 넘지 않는다는 것을 감안해야 한다.

        .. code-block:: sql
          
            CREATE TABLE tb1(A SMALLINT AUTO_INCREMENT, B CHAR(5));

.. _constraint-definition:

ON UPDATE
---------

특정 테이블의 해당 열의 다른 속성값이 변경되면 자동으로 변경되는 속성을 추가할 수있다. **ON UPDATE** 의 값은 **ALTER** 문을 통해서 변경할 수있다. 의사 컬럼은 다음과 같은 방법으로 **ON UPDATE** 의 값을 허용한다. 갱신되는 필드의 목록에 의사 컬럼이 포함되는 경우, 정해진 **ON UPDATE** 값으로의 수정되지 않는다.

+-------------------------------+---------------+
| 기본값                        | 데이터 타입   |
+===============================+===============+
| SYS_TIMESTAMP                 | TIMESTAMP     |
+-------------------------------+---------------+
| UNIX_TIMESTAMP()              | INTEGER       |
+-------------------------------+---------------+
| CURRENT_TIMESTAMP             | TIMESTAMP     |
+-------------------------------+---------------+
| SYS_DATETIME                  | DATETIME      |
+-------------------------------+---------------+
| CURRENT_DATETIME              | DATETIME      |
+-------------------------------+---------------+
| SYS_DATE                      | DATE          |
+-------------------------------+---------------+
| CURRENT_DATE                  | DATE          |
+-------------------------------+---------------+

.. code-block:: sql

     CREATE TABLE sales (sales_cnt INTEGER, last_sale TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, product VARCHAR(100), product_id INTEGER);
     INSERT INTO sales VALUES (0, NULL, 'bicycle', 1);

     UPDATE sales set sales_cnt = sales_cnt + 1
     WHERE product_id = 1;

.. code-block:: sql

    ALTER TABLE sales MODIFY last_sale TIMESTAMP; -- removes ON UPDATE
    UPDATE sales set sales_cnt = sales_cnt + 1
    WHERE product_id = 1; -- last_sale will remain unupdated

제약 조건 정의
--------------

제약 조건으로 **NOT NULL**, **UNIQUE**, **PRIMARY KEY**, **FOREIGN KEY** 를 정의할 수 있다. 또한 제약 조건은 아니지만 **INDEX** 또는 **KEY** 를 사용하여 인덱스를 생성할 수도 있다. 

::

    <column_constraint> ::= [CONSTRAINT constraint_name] { NOT NULL | UNIQUE | PRIMARY KEY | [FOREIGN KEY] [WITH <index_with_option>] <referential_definition> }

    <table_constraint> ::=         
        { 
            {KEY|INDEX} index_name <index_col_desc>  |
            [CONSTRAINT [constraint_name]]
               {
                      UNIQUE [KEY|INDEX](column_name, ...) |
                      PRIMARY KEY (column_name, ...) |
                      <referential_constraint>
               }
        }
     
        <referential_constraint> ::= FOREIGN KEY [<foreign_key_name>](column_name, ...) [WITH <index_with_option>] <referential_definition>
     
            <referential_definition> ::=
                REFERENCES [schema_name.]referenced_table_name (column_name, ...) [<referential_triggered_action> ...]
     
                <referential_triggered_action> ::=
                    ON UPDATE <referential_action> |
                    ON DELETE <referential_action> 
    
                    <referential_action> ::= CASCADE | RESTRICT | NO ACTION | SET NULL
                    
        <index_with_option> ::= {DEDUPLICATE ‘=‘ deduplicate_level}

NOT NULL 제약
^^^^^^^^^^^^^

**NOT NULL** 제약 조건이 정의된 칼럼은 반드시 **NULL** 이 아닌 값을 가져야 한다. 모든 칼럼에 대해 **NOT NULL** 제약 조건을 정의할 수 있다. **INSERT**, **UPDATE** 구문을 통해 **NOT NULL** 속성 칼럼에 **NULL** 값을 입력하거나 갱신하면 에러가 발생한다.

아래 예에서 *id* 칼럼은 NULL 값을 가질 수 없으므로, INSERT 문에서 *id* 칼럼에 NULL을 입력하면 오류가 발생한다.

.. code-block:: sql

    CREATE TABLE const_tbl1(id INT NOT NULL, INDEX i_index(id ASC), phone VARCHAR);
     
    CREATE TABLE const_tbl2(id INT NOT NULL PRIMARY KEY, phone VARCHAR);
    INSERT INTO const_tbl2 VALUES (NULL,'000-0000');

::

    Putting value 'null' into attribute 'id' returned: Attribute "id" cannot be made NULL.

UNIQUE 제약
^^^^^^^^^^^

**UNIQUE** 제약 조건은 정의된 칼럼이 고유한 값을 갖도록 하는 제약 조건이다. 기존 레코드와 동일한 칼럼 값을 갖는 레코드가 추가되면 에러가 발생한다.

**UNIQUE** 제약 조건은 단일 칼럼뿐만 아니라 하나 이상의 다중 칼럼에 대해서도 정의가 가능하다. **UNIQUE** 제약 조건이 다중 칼럼에 대해 정의되면 각 칼럼 값에 대해 고유성이 보장되는 것이 아니라, 다중 칼럼 값의 조합에 대해 고유성이 보장된다.

아래 예에서 두번째 INSERT 문의 *id* 칼럼의 값은 첫번째 INSERT 문의 *id* 칼럼 값과 동일한 1이므로 오류가 발생한다.

.. code-block:: sql

    -- UNIQUE constraint is defined on a single column only
    CREATE TABLE const_tbl5(id INT UNIQUE, phone VARCHAR);
    INSERT INTO const_tbl5(id) VALUES (NULL), (NULL);
    INSERT INTO const_tbl5 VALUES (1, '000-0000');
    SELECT * FROM const_tbl5;

::

       id  phone
    =================
     NULL  NULL
     NULL  NULL
        1  '000-0000'
     
.. code-block:: sql

    INSERT INTO const_tbl5 VALUES (1, '111-1111');
     
::

    ERROR: Operation would have caused one or more unique constraint violations.

아래 예에서 **UNIQUE** 제약 조건이 다중 칼럼에 대해 정의되면 칼럼 전체 값의 조합에 대해 고유성이 보장된다. 

.. code-block:: sql
     
    -- UNIQUE constraint is defined on several columns
    CREATE TABLE const_tbl6(id INT, phone VARCHAR, CONSTRAINT UNIQUE (id, phone));
    INSERT INTO const_tbl6 VALUES (1, NULL), (2, NULL), (1, '000-0000'), (1, '111-1111');
    SELECT * FROM const_tbl6;

::
    
       id  phone
    ====================
        1  NULL
        2  NULL
        1  '000-0000'
        1  '111-1111'

PRIMARY KEY 제약
^^^^^^^^^^^^^^^^

테이블에서 키(key)란 각 행을 고유하게 식별할 수 있는 하나 이상의 칼럼들의 집합을 말한다. 후보키(candidate key)는 테이블 내의 각 행을 고유하게 식별하는 칼럼들의 집합을 의미하며, 사용자는 이러한 후보 키 중 하나를 기본키(primary key)로 정의할 수 있다. 즉, 기본키로 정의된 칼럼 값은 각 행에서 고유하게 식별된다.

기본키를 정의하여 생성되는 인덱스는 기본적으로 오름차순으로 생성되며, 칼럼 뒤에 **ASC** 또는 **DESC** 키워드를 명시하여 키의 순서를 지정할 수 있다. 

.. code-block:: sql

    CREATE TABLE pk_tbl (a INT, b INT, PRIMARY KEY (a, b DESC));

    CREATE TABLE const_tbl7 (
        id INT NOT NULL,
        phone VARCHAR,
        CONSTRAINT pk_id PRIMARY KEY (id)
    );
     
    -- CONSTRAINT keyword
    CREATE TABLE const_tbl8 (
        id INT NOT NULL PRIMARY KEY,
        phone VARCHAR
    );
     
    -- primary key is defined on multiple columns
    CREATE TABLE const_tbl8 (
        host_year    INT NOT NULL,
        event_code   INT NOT NULL,
        athlete_code INT NOT NULL,
        medal        CHAR (1)  NOT NULL,
        score        VARCHAR (20),
        unit         VARCHAR (5),
        PRIMARY KEY (host_year, event_code, athlete_code, medal)
    );

.. _foreign-key-constraint:

FOREIGN KEY 제약
^^^^^^^^^^^^^^^^

외래키(foreign key)란 참조 관계에 있는 다른 테이블의 기본키를 참조하는 칼럼 또는 칼럼들의 집합을 말한다. 외래키와 참조되는 기본키는 동일한 데이터 타입을 가져야 한다. 외래키가 기본키를 참조함에 따라 연관되는 두 테이블 사이에는 일관성이 유지되는데, 이를 참조 무결성(referential integrity)이라 한다. ::

    [CONSTRAINT constraint_name] FOREIGN KEY [foreign_key_name] (<column_name_comma_list1>) [WITH <index_with_option>] REFERENCES [schema_name.]referenced_table_name (<column_name_comma_list2>) [<referential_triggered_action> ...]
     
        <referential_triggered_action> ::= ON UPDATE <referential_action> | ON DELETE <referential_action>

            <referential_action> ::= CASCADE | RESTRICT | NO ACTION  | SET NULL
          
        <index_with_option> ::= {DEDUPLICATE ‘=‘ deduplicate_level}

*   *constraint_name*: 제약 조건의 이름을 지정한다.
*   *foreign_key_name*: **FOREIGN KEY** 제약 조건의 이름을 지정한다. 생략할 수 있으며, 이 값을 지정하면 *constraint_name* 을 무시하고 이 이름을 사용한다.

*   <*column_name_comma_list1*>: **FOREIGN KEY** 키워드 뒤에 외래키로 정의하고자 하는 칼럼 이름을 명시한다. 정의되는 외래키의 칼럼 개수는 참조되는 기본키의 칼럼 개수와 동일해야 한다.
*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *referenced_table_name*: 참조되는 테이블의 이름을 지정한다.
*   <*column_name_comma_list2*>: **REFERENCES** 키워드 뒤에 참조되는 기본키 칼럼 이름을 지정한다.
*   <*referential_triggered_action*>: 참조 무결성이 유지되도록 특정 연산에 따라 대응하는 트리거 동작을 정의하는 것이며, **ON UPDATE**, **ON DELETE** 가 올 수 있다. 각각의 동작은 중복하여 정의 가능하며, 정의 순서는 무관하다.

    *   **ON UPDATE**: 외래키가 참조하는 기본키 값을 갱신하려 할 때 수행할 작업을 정의한다. 사용자는 **NO ACTION**, **RESTRICT**, **SET NULL** 중 하나의 옵션을 지정할 수 있으며, 기본은 **RESTRICT** 이다.
    *   **ON DELETE**: 외래키가 참조하는 기본키 값을 삭제하려 할 때 수행할 작업을 정의한다. 사용자는 **NO ACTION**, **RESTRICT**, **CASCADE**, **SET NULL** 중 하나의 옵션을 지정할 수 있으며, 기본은 **RESTRICT** 이다.

*   <*referential_action*>: 기본키 값이 삭제 또는 갱신될 때 이를 참조하는 외래키의 값을 유지할 것인지 또는 변경할 것인지 지정할 수 있다.

    *   **CASCADE**: 기본키가 삭제되면 외래키도 삭제한다. **ON DELETE** 연산에 대해서만 지원된다.
    *   **RESTRICT**: 기본키 값이 삭제되거나 업데이트되지 않도록 제한한다. 삭제 또는 업데이트를 시도하는 트랜잭션은 롤백된다.
    *   **SET NULL**: 기본키가 삭제되거나 업데이트되면, 이를 참조하는 외래키 칼럼 값을 **NULL** 로 업데이트한다.
    *   **NO ACTION**: **RESTRICT** 옵션과 동일하게 동작한다.

*   *deduplicate_level*: deduplicate 레벨을 지정한다(0 ~ 14). 자세한 내용은 :ref:`deduplicate_overview`\를 참고한다.

참조하는 테이블의 각 R1 행에 대해 참조되는 테이블의 R2 행이 있어야 하며, R1의 참조하는 각 컬럼의 값이 **NULL** 이거나 R2의 참조되는 해당 컬럼의 값과 동일해야 한다.

.. code-block:: sql

    -- creating two tables where one is referencing the other
    CREATE TABLE a_tbl (
        id INT NOT NULL DEFAULT 0 PRIMARY KEY,
        phone VARCHAR(10)
    );
     
    CREATE TABLE b_tbl (
        id INT NOT NULL,
        name VARCHAR (10) NOT NULL,
        CONSTRAINT pk_id PRIMARY KEY (id),
        CONSTRAINT fk_id FOREIGN KEY (id) REFERENCES a_tbl (id)
        ON DELETE CASCADE ON UPDATE RESTRICT
    );
     
    INSERT INTO a_tbl VALUES (1,'111-1111'), (2,'222-2222'), (3, '333-3333');
    INSERT INTO b_tbl VALUES (1,'George'),(2,'Laura'), (3,'Max');
    SELECT a.id, b.id, a.phone, b.name FROM a_tbl a, b_tbl b WHERE a.id = b.id;
     
::

       id           id                   phone                 name
    ======================================================================
        1            1                   '111-1111'            'George'
        2            2                   '222-2222'            'Laura'
        3            3                   '333-3333'            'Max'
     
.. code-block:: sql

    -- when deleting primary key value, it cascades foreign key value  
    DELETE FROM a_tbl WHERE id=3;
     
::

    1 row affected.
     
.. code-block:: sql

    SELECT a.id, b.id, a.phone, b.name FROM a_tbl a, b_tbl b WHERE a.id = b.id;
     
::

       id           id                   phone                 name
    ======================================================================
        1            1                   '111-1111'            'George'
        2            2                   '222-2222'            'Laura'

.. code-block:: sql

    -- when attempting to update primary key value, it restricts the operation
    UPDATE  a_tbl SET id = 10 WHERE phone = '111-1111';
     
::

    ERROR: Update/Delete operations are restricted by the foreign key 'fk_id'.

.. note::

    *   참조 제약 조건에는 참조 대상이 되는 기본키 테이블의 이름 및 기본키와 일치하는 칼럼명들이 정의된다. 만약, 칼럼명 목록을 지정하지 않을 경우에는 기본키 테이블의 기본키가 원래 지정된 순서대로 지정된다.
    *   참조 제약 조건의 기본키의 개수는 외래키의 개수와 동일해야 한다. 참조 제약 조건의 기본키는 동일한 칼럼명이 중복될 수 없다.
    *   참조 제약 조건에 의해 CASCADE되는 작업은 트리거의 동작을 활성화하지 않는다.
    *   CUBRID HA 환경에서는 *referential_triggered_action* 을 사용하지 않는 것을 권장한다. CUBRID HA 환경에서는 트리거를 지원하지 않으므로, *referential_triggered_action* 을 사용하면 마스터 데이터베이스와 슬레이브 데이터베이스의 데이터가 일치하지 않을 수 있다. 자세한 내용은 :doc:`/ha` 를 참고한다.

KEY 또는 INDEX
^^^^^^^^^^^^^^

**KEY** 와 **INDEX** 는 동일하며, 해당 칼럼을 키로 하는 인덱스를 생성한다.

.. code-block:: sql

    CREATE TABLE const_tbl4(id INT, phone VARCHAR, KEY i_key(id DESC, phone ASC));

.. note:: CUBRID 9.0 미만 버전에서는 인덱스 이름을 생략할 수 있었으나, CUBRID 9.0 버전부터는 인덱스 이름을 생략할 수 없다.

칼럼 옵션
---------

특정 칼럼에 **UNIQUE** 또는 **INDEX** 를 정의할 때, 해당 칼럼 이름 뒤에 **ASC** 또는 **DESC** 옵션을 명시할 수 있다. 이 키워드는 오름차순 또는 내림차순 인덱스 값 저장을 위해 명시된다. 

.. code-block:: sql

    column_name [ASC | DESC]

.. code-block:: sql

    CREATE TABLE const_tbl(
        id VARCHAR,
        name VARCHAR,
        CONSTRAINT UNIQUE INDEX(id DESC, name ASC)
    );
     
    INSERT INTO const_tbl VALUES('1000', 'john'), ('1000','johnny'), ('1000', 'jone');
    INSERT INTO const_tbl VALUES('1001', 'johnny'), ('1001','john'), ('1001', 'jone');
     
    SELECT * FROM const_tbl WHERE id > '100';
    
::

      id    name    
    =================
      1001     john     
      1001     johnny     
      1001     jone     
      1000     john     
      1000     johnny     
      1000     jone

테이블 옵션
-----------

테이블 옵션 중 **REUSE_OID** 와 **DONT_REUSE_OID** 은 생성하는 테이블이 참조 가능한 테이블인지 아닌지를 지정하는 옵션이다. 두개의 옵션은 함께 사용할 수 없으며 다른 옵션들과는 함께 사용할 수 있다. 테이블 생성시 옵션을 생략한 경우에는 **REUSE_OID** 테이블 옵션을 사용한다. 기본 옵션을 **DONT_REUSE_OID** 로 변경하려면, 시스템 파라미터인 **create_table_reuseoid** 값을 **no** 로 변경하면 된다. 자세한 내용은 :ref:`stmt-type-parameters` 를 참조하면 된다.

::

        <table_options> ::= <table_option> [[,] <table_option> ...]
            <table_option> ::= REUSE_OID | DONT_REUSE_OID |
                               COMMENT [=] 'table_comment_string' |
                               [CHARSET charset_name] [COLLATE collation_name]

.. _reuse-oid:

REUSE_OID
^^^^^^^^^

테이블 생성 시 **REUSE_OID** 옵션을 명시하면, 레코드 삭제(**DELETE**)로 인해 삭제된 OID를 새로운 레코드 삽입(**INSERT**) 시 재사용할 수 있다. **REUSE_OID** 옵션을 명시하여 생성된 테이블을 OID 재사용 테이블 또는 참조 불가능(non-referable)한 테이블이라고 한다.

OID(Object Identifier)는 볼륨 번호, 페이지 번호, 슬롯 번호와 같은 물리적 위치 정보로 표현되는 객체 식별자이다. CUBRID는 OID를 이용하여 객체의 참조 관계를 관리하고, 객체 조회, 저장, 삭제를 수행한다. OID를 이용하면 테이블을 참조하지 않고도 힙 파일 내의 해당 오브젝트에 직접 접근할 수 있어 접근성이 향상되지만, 객체가 삭제되더라도 참조 관계를 유지하기 위해 해당 객체의 OID를 보존하기 때문에 **DELETE** / **INSERT** 연산이 많은 경우 저장 공간 재사용률이 저하되는 문제가 있다.

테이블 생성 시 **REUSE_OID** 옵션을 명시하면, 해당 테이블 내의 데이터 삭제 시 해당 OID가 함께 삭제되며, **INSERT** 된 다른 데이터가 해당 OID를 재사용할 수 있다. 단, OID 재사용 테이블을 다른 테이블이 참조할 수 없고, OID 재사용 테이블 내 객체들의 OID 값을 조회할 수 없다.

.. code-block:: sql

    -- creating table with REUSE_OID option specified
    CREATE TABLE reuse_tbl (a INT PRIMARY KEY) REUSE_OID, COMMENT = 'reuse oid table';
    INSERT INTO reuse_tbl VALUES (1);
    INSERT INTO reuse_tbl VALUES (2);
    INSERT INTO reuse_tbl VALUES (3);
     
    -- an error occurs when column type is a OID reusable table itself
    CREATE TABLE tbl_1 (a reuse_tbl);

::
    
    ERROR: The class 'reuse_tbl' is marked as REUSE_OID and is non-referable. Non-referable classes can't be the domain of an attribute and their instances' OIDs cannot be returned.

테이블의 콜레이션과 같이 지정하는 경우 REUSE_OID를 콜레이션 앞 또는 뒤에 지정할 수 있다. 
     
.. code-block:: sql
    
    CREATE TABLE t3(a VARCHAR(20)) REUSE_OID, COMMENT = 'reuse oid table', COLLATE euckr_bin;
    CREATE TABLE t4(a VARCHAR(20)) COLLATE euckr_bin REUSE_OID;

.. note::

    *   다른 테이블이 OID 재사용 테이블을 참조할 수 없다.
    *   OID 재사용 테이블에 대해 갱신 가능한(updatable) 뷰를 생성할 수 없다.
    *   테이블의 칼럼 타입으로 OID 재사용 테이블을 지정할 수 없다.
    *   OID 재사용 테이블 객체들의 OID 값을 읽을 수 없다.
    *   OID 재사용 테이블에서 인스턴스 메서드를 호출할 수 없다. 메서드가 정의된 클래스를 상속받은 서브클래스가 OID 재사용 테이블로 정의되어도 마찬가지로 인스턴스 메서드를 호출할 수 없다.
    *   OID 재사용 테이블은 CUBRID 2008 R2.2 버전 이상에서만 지원되며, 하위 호환성을 보장하지 않는다. 즉, 더 낮은 버전의 데이터베이스 서버에서 OID 재사용 테이블이 존재하는 데이터베이스에 접근할 수 없다.
    *   OID 재사용 테이블은 분할 테이블로 관리될 수 있으며, 복제될 수 있다.
    *   OID 재사용 테이블이 포함된 뷰의 업데이트 질의는 아래와 같은 오류가 발생한다.

.. code-block:: sql

   CREATE TABLE t1 (c1 INT) REUSE_OID;
   CREATE VIEW v3 AS SELECT c1 FROM t1;

   insert into v3(c1) values (1);

::

   ERROR: Vclass v3 is not updatable. Please check if any of its related classes are marked as REUSE_OID.

.. _dont-reuse-oid:

DONT_REUSE_OID
^^^^^^^^^^^^^^

테이블 생성시 **DONT_REUSE_OID** 옵션을 명시하면, **REUSE_OID** 와 상반된 참조 가능(referable)한 테이블을 생성한다. 

문자셋과 콜레이션
^^^^^^^^^^^^^^^^^

해당 테이블과 컬럼에 적용할 문자셋과 콜레이션을 **CREATE TABLE** 문에 명시할 수 있다.
다음 예시는 CREATE TABLE시 문자셋과 콜레이션을 지정하는 예문이다.
 
.. code-block:: sql
    
    CREATE TABLE tblutf8(a int, b int) charset utf8;
    CREATE TABLE tblutf8enci(a int, b int) collate utf8_en_ci;
 
    CREATE TABLE tbleuckr(a int, b int) charset euckr collate euckr_bin;
    CREATE TABLE tblcoleuckr(a int, c varchar(100) collate euckr_bin) charset euckr;
  
    CREATE TABLE tblcol (s1 STRING COLLATE utf8_en_cs,s2 STRING COLLATE utf8_bin);
    CREATE TABLE tblcharcol (col STRING CHARSET utf8) COLLATE utf8_en_ci;

이에 관한 자세한 내용은 :ref:`collation-charset-table` 또는 :ref:`collation-charset-string` 절을 참조하면 된다.

테이블의 커멘트
^^^^^^^^^^^^^^^

테이블의 커멘트를 다음과 같이 명시할 수 있다. 

.. code-block:: sql

    CREATE TABLE tbl (a INT, b INT) COMMENT = 'this is comment for table tbl';

테이블의 커멘트는 다음 구문에서 확인할 수 있다.

.. code-block:: sql

    SHOW CREATE TABLE table_name;
    SELECT class_name, comment from db_class;
    SELECT class_name, comment from _db_class;

또는 CSQL 인터프리터에서 테이블의 스키마를 출력하는 ;sc 명령으로 테이블의 커멘트를 확인할 수 있다.

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc tbl

.. _create-tde-table:

테이블 암호화 (TDE)
^^^^^^^^^^^^^^^^^^^

다음과 같이 테이블을 암호화할 수 있다. TDE 암호화에 관한 자세한 내용은 :ref:`tde` 절을 참고한다. 

.. code-block:: sql

    CREATE TABLE enc_tbl (a INT, b INT) ENCRYPT = AES;

암호화 알고리즘으로 **AES**, **ARIA** 를 지정할 수 있다. 다음과 같이 생략할 경우 시스템 파라미터 **tde_default_algorithm** 으로 지정된 암호화 알고리즘이 사용 된다. 기본 값은 **AES** 이다.

.. code-block:: sql

    CREATE TABLE enc_tbl (a INT, b INT) ENCRYPT;

암호화 여부는 상속되지 않는다.

CREATE TABLE LIKE
-----------------

**CREATE TABLE ... LIKE** 문을 사용하면, 이미 존재하는 테이블의 스키마와 동일한 스키마를 갖는 테이블을 생성할 수 있다. 기존 테이블에서 정의된 칼럼 속성, 테이블 제약 조건, 암호화 여부, 인덱스도 그대로 복제된다. 원본 테이블에서 자동 생성된 인덱스의 이름은 새로 생성된 테이블의 이름에 맞게 새로 생성되지만, 사용자에 의해 지어진 인덱스 이름은 그대로 복제된다. 그러므로 인덱스 힌트 구문(:ref:`index-hint-syntax` 참고)으로 특정 인덱스를 사용하도록 작성된 질의문이 있다면 주의해야 한다.

**CREATE TABLE ... LIKE** 문은 스키마만 복제하므로 칼럼 정의문을 작성할 수 없다. 

::

    CREATE {TABLE | CLASS} [schema_name.]new_table_name LIKE [schema_name.]source_table_name;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *new_table_name*: 새로 생성할 테이블 이름이다.
*   *source_table_name*: 데이터베이스에 이미 존재하는 원본 테이블 이름이다. **CREATE TABLE ... LIKE** 문에서 아래의 테이블은 원본 테이블로 지정될 수 없다.

    *   분할 테이블
    *   **AUTO_INCREMENT** 칼럼이 포함된 테이블
    *   상속 또는 메서드를 사용하는 테이블

.. code-block:: sql

    CREATE TABLE a_tbl (
      id INT NOT NULL DEFAULT 0 PRIMARY KEY,
      phone VARCHAR(10)
    );
    INSERT INTO a_tbl VALUES (1,'111-1111'), (2,'222-2222'), (3, '333-3333');
     
    -- creating an empty table with the same schema as a_tbl
    CREATE TABLE new_tbl LIKE a_tbl;
    SELECT * FROM new_tbl;

::

    There are no results.
     
    csql> ;schema a_tbl
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         a_tbl
     
     <Attributes>
     
         id                   INTEGER DEFAULT 0 NOT NULL
         phone                CHARACTER VARYING(10)
     
     <Constraints>
     
         PRIMARY KEY pk_a_tbl_id ON a_tbl (id)
     
    csql> ;schema new_tbl
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         new_tbl
     
     <Attributes>
     
         id                   INTEGER DEFAULT 0 NOT NULL
         phone                CHARACTER VARYING(10)
     
     <Constraints>
     
         PRIMARY KEY pk_new_tbl_id ON new_tbl (id)

CREATE TABLE AS SELECT
----------------------

**CREATE TABLE ... AS SELECT** 문을 사용하여 **SELECT** 문의 결과 레코드를 포함하는 새로운 테이블을 생성할 수 있다. 새로운 테이블에 대해 칼럼 및 테이블 제약 조건을 정의할 수 있으며, 다음의 규칙을 적용하여 **SELECT** 결과 레코드를 반영한다.

*   새로운 테이블에 칼럼 *col_1* 이 정의되고, *select_statement* 에 동일한 칼럼 *col_1* 이 명시된 경우, **SELECT** 결과 레코드가 새로운 테이블 *col_1* 값으로 저장된다. 칼럼 이름은 같고 칼럼 타입이 다르면 타입 변환을 시도한다.

*   새로운 테이블에 칼럼 *col_1*, *col_2* 가 정의되고, *select_statement* 의 칼럼 리스트에 *col_1*, *col_2*, *col_3* 이 명시되어 모두 포함 관계가 성립하는 경우, 새로 생성되는 테이블에는 *col_1*, *col_2*, *col_3* 이 생성되고, **SELECT** 결과 데이터가 모든 칼럼 값으로 저장된다. 칼럼 이름은 같고 칼럼 타입이 다르면 타입 변환을 시도한다.

*   새로운 테이블에 칼럼 *col_1*, *col_2* 가 정의되고, *select_statement* 의 칼럼 리스트에 *col_1*, *col_3* 이 명시되어 포함 관계가 성립하지 않는 경우, 새로 생성되는 테이블에는 *col_1*, *col_2*, *col_3* 이 생성되고, *select_statement* 에 명시된 칼럼 *col_1*, *col_3* 에 대해서만 **SELECT** 결과 데이터가 저장되고, *col_2* 에는 NULL이 저장된다.

*   *select_statement* 의 칼럼 리스트에는 칼럼 별칭(alias)이 포함될 수 있으며, 이 경우 칼럼 별칭이 새로운 테이블 칼럼 이름으로 사용된다. 함수 호출이나 표현식이 사용된 경우 별칭이 없으면 유효하지 않은 칼럼 이름이 생성되므로, 이 경우에는 별칭을 사용하는 것이 좋다.

*   **REPLACE** 옵션은 새로운 테이블의 칼럼(*col_1*)에 **UNIQUE** 제약 조건이 정의된 경우에만 유효하다. *select_statement* 의 결과 레코드에 중복된 값이 존재하는 경우, **REPLACE** 옵션이 명시되면 칼럼 *col_1* 에는 고유한 값이 저장되고, **REPLACE** 옵션이 생략되면 **UNIQUE** 제약 조건에 위배되므로 에러 메시지가 출력된다.

::

    CREATE {TABLE | CLASS} [schema_name.]table_name [([{<table_constraint>}... ,] <column_definition> [{ ,{<column_definition> | <table_constraint>}}...])] [COMMENT [=] 'comment_string'] [REPLACE] AS <select_statement>;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 새로 생성할 테이블 이름이다.
*   <*column_definition*>: 칼럼을 정의한다. 생략하면 **SELECT** 문의 칼럼 스키마가 복제된다. **SELECT** 문의 칼럼 제약 조건이나 **AUTO_INCREMENT** 속성, 테이블/칼럼의 커멘트는 복제되지 않는다.
*   <*table_constraint*>: 테이블 제약 조건을 정의한다.
*   <*select_statement*>: 데이터베이스에 이미 존재하는 원본 테이블을 대상으로 하는 **SELECT** 문이다.
*   <*select statement*>은 tbl@srver1처럼 원격 테이블을 포함할 수 있다.

.. code-block:: sql

    CREATE TABLE a_tbl (
      id INT NOT NULL DEFAULT 0 PRIMARY KEY,
      phone VARCHAR(10)
    );
    INSERT INTO a_tbl VALUES (1,'111-1111'), (2,'222-2222'), (3, '333-3333');
     
    -- creating a table without column definition
    CREATE TABLE new_tbl1 AS SELECT * FROM a_tbl;
    SELECT * FROM new_tbl1;
     
::

       id  phone
    ===================================
        1  '111-1111'
        2  '222-2222'
        3  '333-3333'
     
.. code-block:: sql

    -- all of column values are replicated from a_tbl
    CREATE TABLE new_tbl2 (
      id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
      phone VARCHAR
    ) AS SELECT * FROM a_tbl;
    
    SELECT * FROM new_tbl2;
     
::

       id  phone
    ===================================
        1  '111-1111'
        2  '222-2222'
        3  '333-3333'
     
.. code-block:: sql

    -- some of column values are replicated from a_tbl and the rest is NULL
    CREATE TABLE new_tbl3 (
      id INT, 
      name VARCHAR
    ) AS SELECT id, phone FROM a_tbl;
    
    SELECT * FROM new_tbl3
     
::

      name                           id  phone
    =========================================================
      NULL                            1  '111-1111'
      NULL                            2  '222-2222'
      NULL                            3  '333-3333'
     
.. code-block:: sql

    -- column alias in the select statement should be used in the column definition
    CREATE TABLE new_tbl4 (
      id1 INT, 
      id2 INT
    ) AS SELECT t1.id id1, t2.id id2 FROM new_tbl1 t1, new_tbl2 t2;
    
    SELECT * FROM new_tbl4;
     
::

      id1          id2
    ==========================
        1            1
        1            2
        1            3
        2            1
        2            2
        2            3
        3            1
        3            2
        3            3
     
.. code-block:: sql

    -- REPLACE is used on the UNIQUE column
    CREATE TABLE new_tbl5 (id1 int UNIQUE) REPLACE AS SELECT * FROM new_tbl4;
    
    SELECT * FROM new_tbl5;
     
::

      id1          id2
    ==========================
        1            3
        2            3
        3            3


아래와 같이 원격 테이블로 부터 데이터를 읽어서 테이블을 만들 수 있다.

.. code-block:: sql

    -- column values are replicated from remote_tbl
    CREATE TABLE new_tbl_rem (
      id INT NON NULL;
      phone VARCHAR;
      localtion VARCHAR;
    ) AS SELECT id, phone, 'remote' FROM a_tbl@server1 WHERE id < 3;

    SELECT * FROM new_tbl_rem;

::

      id  phone       location
    ===================================
       1  '111-1111'  'remote'
       2  '222-2222'  'remote'


ALTER TABLE
===========

**ALTER** 구문을 이용하여 테이블의 구조를 변경할 수 있다. 대상 테이블에 칼럼 추가/삭제, 인덱스 생성/삭제, 기존 칼럼의 타입 변경, 테이블 이름 변경, 칼럼 이름 변경 등을 수행하거나 테이블 제약 조건을 변경한다. 또한 **AUTO_INCREMENT** 의 초기값을 변경할 수 있다. **TABLE** 은 **CLASS** 와 동의어이다. **COLUMN** 은 **ATTRIBUTE** 와 동의어이다. 

::

    ALTER [TABLE | CLASS] [schema_name.]table_name <alter_clause> [, <alter_clause>] ... ;
     
        <alter_clause> ::= 
            ADD <alter_add> [INHERIT <resolution>, ...]  | 
            ADD {KEY | INDEX} <index_name> <index_col_desc> |
            ALTER [COLUMN] column_name SET DEFAULT <value_specification> |
            DROP <alter_drop> [INHERIT <resolution>, ...] |
            DROP {KEY | INDEX} index_name |
            DROP FOREIGN KEY constraint_name |
            DROP PRIMARY KEY |                   
            RENAME <alter_rename> [INHERIT <resolution>, ...] |
            CHANGE <alter_change> |
            MODIFY <alter_modify> |            
            INHERIT <resolution>, ... |
            AUTO_INCREMENT = <initial_value> |
            COMMENT [=] 'table_comment_string' |
            COMMENT ON {COLUMN | CLASS ATTRIBUTE} <column_comment_definition> [, <column_comment_definition>] ;
                           
            <alter_add> ::= 
                [ATTRIBUTE|COLUMN] [(]<class_element>, ...[)] [FIRST|AFTER old_column_name] |
                CLASS ATTRIBUTE <column_definition>, ... |
                CONSTRAINT <constraint_name> <column_constraint> (column_name) |
                QUERY <select_statement> |
                SUPERCLASS [schema_name.]superclass_name, ...
                            
                <class_element> ::= <column_definition> | <table_constraint>
     
                <column_constraint> ::= UNIQUE [KEY] | PRIMARY KEY | FOREIGN KEY
     
            <alter_drop> ::= 
                [ATTRIBUTE | COLUMN]
                {
                    column_name, ... |
                    QUERY [<unsigned_integer_literal>] |
                    SUPERCLASS [schema_name.]superclass_name, ... |
                    CONSTRAINT constraint_name
                }
                             
            <alter_rename> ::= 
                [ATTRIBUTE | COLUMN]
                {
                    old_column_name AS new_column_name |
                    FUNCTION OF column_name AS function_name
                }
                
            <alter_change> ::= 
                [COLUMN | CLASS ATTRIBUTE] old_col_name new_col_name <column_definition>
                    [FIRST | AFTER col_name]

            <alter_modify> ::= 
                [COLUMN | CLASS ATTRIBUTE] col_name <column_definition>
                    [FIRST | AFTER col_name2]
                    
            <table_option> ::=
                CHANGE [COLUMN | CLASS ATTRIBUTE] old_col_name new_col_name <column_definition>
                    [FIRST | AFTER col_name2]
              | MODIFY [COLUMN | CLASS ATTRIBUTE] col_name <column_definition>
                    [FIRST | AFTER col_name2]

            <resolution> ::= column_name OF [schema_name.]superclass_name [AS alias]

            <index_col_name> ::= column_name [(length)] [ASC | DESC]

            <column_comment_definition> ::= column_name [=] 'column_comment_string'

.. note::

    칼럼의 커멘트는 <column_definition>에서 지정하거나 <column_comment_definition>에서 지정한다. <column_definition>은 위의 :ref:`CREATE TABLE 문법<column-definition>`\을 참고한다.

.. warning::

    테이블의 소유자, **DBA**, **DBA** 의 멤버만이 테이블 스키마를 변경할 수 있으며, 그 밖의 사용자는 소유자나 **DBA** 로부터 이름을 변경할 수 있는 권한을 받아야 한다(권한 관련 사항은 :ref:`granting-authorization` 참조)

ADD COLUMN 절
-------------

**ADD COLUMN** 절을 사용하여 새로운 칼럼을 추가할 수 있다. **FIRST** 또는 **AFTER** 키워드를 사용하여 새로 추가할 칼럼의 위치를 지정할 수 있다.

::

    ALTER [TABLE | CLASS] [schema_name.]table_name
    ADD [COLUMN | ATTRIBUTE] [(] <column_definition> [FIRST | AFTER old_column_name] [)];

        <column_definition> ::= 
            column_name <data_type> [[<default_or_shared_or_ai>] | [<on_update>] | [<column_constraint>]] [COMMENT 'comment_string']
        
            <data_type> ::= <column_type> [<charset_modifier_clause>] [<collation_modifier_clause>]

                <charset_modifier_clause> ::= {CHARACTER_SET|CHARSET} {<char_string_literal>|<identifier>}

                <collation_modifier_clause> ::= COLLATE {<char_string_literal>|<identifier>}
            
            <default_or_shared_or_ai> ::=
                SHARED <value_specification> | 
                DEFAULT <value_specification>  |
                AUTO_INCREMENT [(seed, increment)]

            <on_update> ::= [ON UPDATE <value_specification>]
            
            <column_constraint> ::= [CONSTRAINT constraint_name] {NOT NULL | UNIQUE | PRIMARY KEY | [FOREIGN KEY] [WITH <index_with_option>] <referential_definition>}

                <referential_definition> ::=
                    REFERENCES [schema_name.]referenced_table_name (column_name, ...) [<referential_triggered_action> ...]
         
                    <referential_triggered_action> ::=
                        ON UPDATE <referential_action> |
                        ON DELETE <referential_action> 

                        <referential_action> ::= CASCADE | RESTRICT | NO ACTION | SET NULL

                <index_with_option> ::= {DEDUPLICATE ‘=‘ deduplicate_level}

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 칼럼을 추가할 테이블의 이름을 지정한다.
*   <*column_definition*>: 새로 추가할 칼럼의 이름(최대 254 바이트), 데이터 타입, 제약 조건을 정의한다.
*   **AFTER** *old_column_name*: 새로 추가할 칼럼 앞에 위치하는 기존 칼럼 이름을 명시한다.
*   *comment_string*: 칼럼의 커멘트를 지정한다.
*   *deduplicate_level*: deduplicate 레벨을 지정한다(0 ~ 14). 자세한 내용은 :ref:`deduplicate_overview`\를 참고한다.

.. code-block:: sql

    CREATE TABLE a_tbl;
    ALTER TABLE a_tbl ADD COLUMN age INT DEFAULT 0 NOT NULL COMMENT 'age comment';
    ALTER TABLE a_tbl ADD COLUMN name VARCHAR FIRST;
    ALTER TABLE a_tbl ADD COLUMN id INT NOT NULL AUTO_INCREMENT UNIQUE FIRST;
    INSERT INTO a_tbl(age) VALUES(20),(30),(40);

    ALTER TABLE a_tbl ADD COLUMN phone VARCHAR(13) DEFAULT '000-0000-0000' AFTER name;
    ALTER TABLE a_tbl ADD COLUMN birthday VARCHAR(20) DEFAULT TO_CHAR(SYSDATE,'YYYY-MM-DD'); 
    SELECT * FROM a_tbl;
     
::

      id  name                  phone                         age  birthday
    ============================================================================================
       1  NULL                  '000-0000-0000'                20  '2017-05-24'
       2  NULL                  '000-0000-0000'                30  '2017-05-24'
       3  NULL                  '000-0000-0000'                40  '2017-05-24'

     
    --adding multiple columns
    ALTER TABLE a_tbl ADD COLUMN (age1 int, age2 int, age3 int);

새로 추가되는 칼럼에 어떤 제약 조건이 오느냐에 따라 다른 결과를 보여준다.

*   새로 추가되는 칼럼에 **DEFAULT** 제약 조건이 있으면 **DEFAULT** 값이 입력된다.
*   새로 추가되는 칼럼에 **DEFAULT** 제약 조건이 없고 **NOT NULL** 제약 조건이 있는 경우, 시스템 파라미터 **add_column_update_hard_default** 가 **yes** 이면 고정 기본값(hard default)을 갖게 되고, **no** 이면 에러를 반환한다. 
 
**add_column_update_hard_default** 의 기본값은 **no** 이다.
 
**DEFAULT** 제약 조건 및 **add_column_update_hard_default** 값의 설정에 따라 해당 제약 조건을 위배하지 않는 한도 내에서 **PRIMARY KEY** 혹은 **UNIQUE** 제약 조건의 추가가 가능하다.
 
*   테이블에 데이터가 없거나 **NOT NULL** 이고 **UNIQUE** 인 값을 가지는 기존 칼럼에 **PRIMARY KEY** 제약 조건을 지정할 수 있다.
*   테이블에 데이터가 있고 새로 추가되는 칼럼에 **PRIMARY KEY** 제약 조건을 지정하는 경우, 에러를 반환한다. 
 
    .. code-block:: sql
    
        CREATE TABLE tbl (a INT);
        INSERT INTO tbl VALUES (1), (2);
        ALTER TABLE tbl ADD COLUMN (b int PRIMARY KEY);
 
    ::
    
        ERROR: NOT NULL constraints do not allow NULL value.
 
*   테이블에 데이터가 있고 새로 추가되는 칼럼에 UNIQUE 제약 조건을 지정하는 경우, DEFAULT 제약 조건이 없으면 NULL이 입력된다.
 
    .. code-block:: sql
 
        ALTER TABLE tbl ADD COLUMN (b int UNIQUE);
        SELECT * FROM tbl;
 
    ::
    
            a            b
        ==================
            1         NULL
            2         NULL
 
*   테이블에 데이터가 있고 새로 추가되는 칼럼에 UNIQUE 제약 조건을 지정하는 경우, DEFAULT 제약 조건이 있으면 고유 키 위반 에러를 반환한다.
 
    .. code-block:: sql
    
        ALTER TABLE tbl ADD COLUMN (c int UNIQUE DEFAULT 10);
        
    ::
    
        ERROR: Operation would have caused one or more unique constraint violations.
 
*   테이블에 데이터가 있고 새로 추가되는 칼럼에 UNIQUE 제약 조건을 지정하는 경우, NOT NULL 제약 조건이 있고 add_column_update_hard_default가 yes이면 고유 키 위반 에러를 반환한다.
 
    .. code-block:: sql
 
        SET SYSTEM PARAMETERS 'add_column_update_hard_default=yes';
        ALTER TABLE tbl ADD COLUMN (c int UNIQUE NOT NULL);
 
    ::
    
        ERROR: Operation would have caused one or more unique constraint violations.
        
**add_column_update_hard_default** 및 고정 기본값에 대해서는 :ref:`change-column` 을 참고한다. 

ADD CONSTRAINT 절
-----------------

**ADD CONSTRAINT** 절을 사용하여 새로운 제약 조건을 추가할 수 있다.

**PRIMARY KEY** 제약 조건을 추가할 때 생성되는 인덱스는 기본적으로 오름차순으로 생성되며, 칼럼 이름 뒤에 **ASC** 또는 **DESC** 키워드를 명시하여 키의 정렬 순서를 지정할 수 있다. ::

    ALTER [TABLE | CLASS | VCLASS | VIEW] [schema_name.]table_name
    ADD <table_constraint> ;
    
        <table_constraint> ::=             
            { 
                {KEY|INDEX} index_name (column_name, ...) |
                [CONSTRAINT [constraint_name]]
                   {
                      UNIQUE [KEY|INDEX](column_name, ...) |
                      PRIMARY KEY (column_name, ...) |
                      <referential_constraint>
                   }
            }
     
            <referential_constraint> ::= FOREIGN KEY [foreign_key_name](column_name, ...) <referential_definition>
         
                <referential_definition> ::=
                    REFERENCES [schema_name.]referenced_table_name (column_name, ...) [<referential_triggered_action> ...]
         
                    <referential_triggered_action> ::=
                        ON UPDATE <referential_action> |
                        ON DELETE <referential_action> 

                        <referential_action> ::= CASCADE | RESTRICT | NO ACTION | SET NULL

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 제약 조건을 추가할 테이블의 이름을 지정한다.
*   *constraint_name*: 새로 추가할 제약 조건의 이름(최대 254 바이트)을 지정할 수 있으며, 생략할 수 있다. 생략하면 자동으로 부여된다.
*   *foreign_key_name*: **FOREIGN KEY** 제약 조건의 이름을 지정할 수 있다. 생략할 수 있으며, 지정하면 *constraint_name* 을 무시하고 이 이름을 사용한다.
*   <*table_constraint*>: 지정된 테이블에 대해 제약 조건을 정의한다. 제약 조건에 대한 자세한 설명은 :ref:`constraint-definition` 를 참고한다.

.. code-block:: sql

    ALTER TABLE a_tbl ADD CONSTRAINT pk_a_tbl_id PRIMARY KEY(id); 
    ALTER TABLE a_tbl DROP CONSTRAINT pk_a_tbl_id;
    ALTER TABLE a_tbl ADD CONSTRAINT pk_a_tbl_id PRIMARY KEY(id, name DESC);
    ALTER TABLE a_tbl ADD CONSTRAINT u_key1 UNIQUE (id);

ADD INDEX 절
------------

**ADD INDEX** 절은 특정 칼럼에 대해 인덱스 속성을 추가로 정의할 수 있다. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name ADD {KEY | INDEX} index_name (<index_col_name>) ;
     
        <index_col_name> ::= column_name [(length)] [ ASC | DESC ]

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 변경하고자 하는 테이블의 이름을 지정한다.
*   *index_name*: 인덱스의 이름을 지정한다(최대 254 바이트).
*   *index_col_name*: 인덱스를 정의할 대상 칼럼을 지정하며, 이때 칼럼 옵션으로 **ASC** 또는 **DESC** 을 함께 지정할 수 있다.

.. code-block:: sql

    ALTER TABLE a_tbl ADD INDEX i1(age ASC), ADD INDEX i2(phone DESC);
    
::

    csql> ;schema a_tbl
     
    === <Help: Schema of a Class> ===
     
     <Class Name>
     
         a_tbl
     
    <Attributes>
     
         name                 CHARACTER VARYING(1073741823) DEFAULT ''
         phone                CHARACTER VARYING(13) DEFAULT '111-1111'
         age                  INTEGER
         id                   INTEGER AUTO_INCREMENT  NOT NULL
     
     <Constraints>
     
         UNIQUE u_a_tbl_id ON a_tbl (id)
         INDEX i1 ON a_tbl (age)
         INDEX i2 ON a_tbl (phone DESC)

다음은 ALTER 문으로 인덱스 추가 시 인덱스 커멘트를 포함하는 예제이다.

.. code-block:: sql

    ALTER TABLE tbl ADD index i_t_c (c) COMMENT 'index comment c';

ALTER COLUMN ... SET DEFAULT 절
-------------------------------

**ALTER COLUMN** ... **SET DEFAULT** 절은 기본값이 없는 칼럼에 기본값을 지정하거나 기존의 기본값을 변경할 수 있다. :ref:`change-column` 을 이용하면, 단일 구문으로 여러 칼럼의 기본값을 변경할 수 있다.

::

    ALTER [TABLE | CLASS] [schema_name.]table_name ALTER [COLUMN] column_name SET DEFAULT value ;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 기본값을 변경할 칼럼이 속한 테이블의 이름을 지정한다.
*   *column_name*: 새로운 기본값을 적용할 칼럼의 이름을 지정한다.
*   *value*: 새로운 기본값을 지정한다.

::

    csql> ;schema a_tbl
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         a_tbl
     
     <Attributes>
     
         name                 CHARACTER VARYING(1073741823)
         phone                CHARACTER VARYING(13) DEFAULT '000-0000-0000'
         age                  INTEGER
         id                   INTEGER AUTO_INCREMENT  NOT NULL
     
     <Constraints>
     
         UNIQUE u_a_tbl_id ON a_tbl (id)
     
     
.. code-block:: sql
     
    ALTER TABLE a_tbl ALTER COLUMN name SET DEFAULT '';
    ALTER TABLE a_tbl ALTER COLUMN phone SET DEFAULT '111-1111';
     
::

    csql> ;schema a_tbl
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         a_tbl
     
     <Attributes>
     
         name                 CHARACTER VARYING(1073741823) DEFAULT ''
         phone                CHARACTER VARYING(13) DEFAULT '111-1111'
         age                  INTEGER
         id                   INTEGER AUTO_INCREMENT  NOT NULL
     
     <Constraints>
     
         UNIQUE u_a_tbl_id ON a_tbl (id)

.. code-block:: sql

    CREATE TABLE t1(id1 VARCHAR(20), id2 VARCHAR(20) DEFAULT '');
    ALTER TABLE t1 ALTER COLUMN id1 SET DEFAULT TO_CHAR(SYS_DATETIME, 'yyyy/mm/dd hh:mi:ss');

::

     csql> ;schema t1

     === <Help: Schema of a Class> ===


     <Class Name>

         t1

     <Attributes>

         id1                  CHARACTER VARYING(20) DEFAULT TO_CHAR(SYS_DATETIME, 'yyyy/mm/dd hh:mi:ss')
         id2                  CHARACTER VARYING(20) DEFAULT ''

.. _alter-auto-increment:

AUTO_INCREMENT 절
-----------------

**AUTO_INCREMENT** 절은 기존에 정의한 자동 증가값의 초기값을 변경할 수 있다. 단, 테이블 내에 **AUTO_INCREMENT** 칼럼이 한 개만 정의되어 있어야 한다. ::

    ALTER TABLE [schema_name.]table_name AUTO_INCREMENT = initial_value ;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 테이블 이름
*   *initial_value*: 새로 변경할 초기값

.. code-block:: sql

    CREATE TABLE t (i int AUTO_INCREMENT);
    ALTER TABLE t AUTO_INCREMENT = 5;
     
    CREATE TABLE t (i int AUTO_INCREMENT, j int AUTO_INCREMENT);
    
    -- when 2 AUTO_INCREMENT constraints are defined on one table, below query returns an error.
    ALTER TABLE t AUTO_INCREMENT = 5;

::
    
    ERROR: To avoid ambiguity, the AUTO_INCREMENT table option requires the table to have exactly one AUTO_INCREMENT column and no seed/increment specification.

.. warning:: **AUTO_INCREMENT** 의 초기값 변경으로 인해 **PRIMARY KEY** 나 **UNIQUE** 와 같은 제약 조건에 위배되는 경우가 발생하지 않도록 주의한다.

.. note:: **AUTO_INCREMENT** 칼럼의 타입을 변경하면 최대값도 변경된다. 예를 들어, INT 타입을 BIGINT 타입으로 변경하면 **AUTO_INCREMENT** 최대값이 INT의 최대값에서 BIGINT의 최대값으로 변경된다.

.. _change-column:

CHANGE/MODIFY 절
----------------

**CHANGE** 절은 칼럼의 이름, 타입, 크기 및 속성을 변경한다. 기존 칼럼의 이름과 새 칼럼의 이름이 같으면 타입, 크기 및 속성만 변경한다.

**MODIFY** 절은 칼럼의 타입, 크기 및 속성을 변경할 수 있으며, 칼럼의 이름은 변경할 수 없다.

**CHANGE** 절이나 **MODIFY** 절로 새 칼럼에 적용할 타입, 크기 및 속성을 설정할 때 기존에 정의된 속성은 새 칼럼의 속성에 전달되지 않는다.

**CHANGE** 절이나 **MODIFY** 절로 칼럼에 데이터 타입을 변경할 때, 기존의 칼럼 값이 변경되면서 데이터가 변형될 수 있다. 예를 들어 문자열 칼럼의 길이를 줄이면 문자열이 잘릴 수 있으므로 주의해야 한다. 단, **alter_table_change_type_strict** 설정 값이 **yes** 인 경우 에러가 발생한다. 마찬가지로 **allow_truncated_string** 설정 값이 **no** 인 경우에도 에러가 발생한다.

AUTO_INCREMENT 속성의 컬럼 타입을 변경할 경우, AUTO_INCREMENT 속성으로 사용할 수 없는 타입으로 변경할 수 없다. 예를 들면 아래와 같이 ALTER 구문으로 AUTO_INCREMENT 속성 컬럼인 a를 int에서 varchar 타입으로 변경시 에러가 발생한다.

.. code-block:: sql

    CREATE a_tbl (a int AUTO_INCREMENT, b VARCHAR);
    ALTER TABLE a_tbl MODIFY COLUMN a VARCHAR;

::

    ERROR: before '  varchar; '
    The domain of the attribute 'a' having an auto increment constraint is invalid.

default값이 지정된 칼럼의 타입을 변경할 때, 지정된 default값이 변경된 타입으로 형변환이 불가능한 경우 아래의 예처럼 에러가 발생한다. 

.. code-block:: sql

    CREATE TABLE t_def (a bigint default 123456789012, b varchar(20));
    ALTER TABLE t_def a a int;

::

    ERROR: A domain conflict exists on attribute "a".

.. warning::

    *   CUBRID 2008 R3.1 이하 버전에서 사용되었던 **ALTER TABLE** *[schema_name.]table_name* **CHANGE** *column_name* **DEFAULT** *default_value* 구문은 더 이상 지원하지 않는다.
    *   숫자를 문자 타입으로 변환할 때, alter_table_change_type_strict=no이고 해당 문자열의 길이가 숫자의 길이보다 짧으면 변환되는 문자 타입의 길이에 맞추어 문자열이 잘린 상태로 저장된다. alter_table_change_type_strict=yes이면 오류를 발생한다.
    *   테이블의 칼럼 타입, 콜레이션 등 칼럼 속성을 변경하는 경우 변경된 속성이 변경 전의 테이블을 이용하여 생성한 뷰에 반영되지는 않는다. 따라서 테이블의 칼럼 속성을 변경하는 경우 뷰를 재생성할 것을 권장한다.

::

    ALTER [/*+ SKIP_UPDATE_NULL */] TABLE [schema_name.]tbl_name <table_options> ;
     
        <table_options> ::=
            <table_option>[, <table_option>, ...]
     
            <table_option> ::=
                CHANGE [COLUMN | CLASS ATTRIBUTE] old_col_name new_col_name <column_definition>
                         [FIRST | AFTER col_name]
              | MODIFY [COLUMN | CLASS ATTRIBUTE] col_name <column_definition>
                         [FIRST | AFTER col_name]

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *tbl_name*: 변경할 칼럼이 속한 테이블의 이름을 지정한다.
*   *old_col_name*: 기존 칼럼의 이름을 지정한다.
*   *new_col_name*: 변경할 칼럼의 이름을 지정한다.
*   <*column_definition*>: 변경할 칼럼의 타입, 크기 및 속성, 커멘트를 지정한다.
*   *col_name*: 변경할 칼럼이 어느 칼럼 뒤에 위치할지를 지정한다.
*   **SKIP_UPDATE_NULL**: 이 힌트가 추가되면 NOT NULL 제약 조건을 추가할 때 기존의 NULL 값을 검사하지 않는다. :ref:`SKIP_UPDATE_NULL <skip-update-null>` 을 참고한다.

.. code-block:: sql

    CREATE TABLE t1 (a INTEGER);
     
    -- changing column a's name into a1
    ALTER TABLE t1 CHANGE a a1 INTEGER;
     
    -- changing column a1's constraint
    ALTER TABLE t1 CHANGE a1 a1 INTEGER NOT NULL;
    ---- or
    ALTER TABLE t1 MODIFY a1 INTEGER NOT NULL;
     
    -- changing column col1's type - "DEFAULT 1" constraint is removed.
    CREATE TABLE t1 (col1 INT DEFAULT 1);
    ALTER TABLE t1 MODIFY col1 BIGINT;
     
    -- changing column col1's type - "DEFAULT 1" constraint is kept.
    CREATE TABLE t1 (col1 INT DEFAULT 1, b VARCHAR(10));
    ALTER TABLE t1 MODIFY col1 BIGINT DEFAULT 1;
     
    -- changing column b's size
    ALTER TABLE t1 MODIFY b VARCHAR(20);

    -- changing the name and position of a column  
    CREATE TABLE t1 (i1 INT, i2 INT);  
    INSERT INTO t1 VALUES (1,11), (2,22), (3,33);
    
    SELECT * FROM t1 ORDER BY 1;
    
::

                i1           i2
    ==========================
                 1           11
                 2           22
                 3           33
     
.. code-block:: sql

    ALTER TABLE t1 CHANGE i2 i0 INTEGER FIRST;  
    SELECT * FROM t1 ORDER BY 1;
    
::

                i0           i1
    ==========================
                11            1
                22            2
                33            3

.. code-block:: sql

    ALTER TABLE t1 MODIFY i1 VARCHAR (200) DEFAULT TO_CHAR (SYS_DATE);
    INSERT INTO t1(i0) VALUES (17);
    SELECT * FROM t1 ORDER BY 1;

::

              i0  i1
    ===================================
              11  '1'
              17  '05/24/2017'
              22  '2'
              33  '3'

.. code-block:: sql

    -- adding NOT NULL constraint (strict)
    SET SYSTEM PARAMETERS 'alter_table_change_type_strict=yes';
     
    CREATE TABLE t1 (i INT);
    INSERT INTO t1 VALUES (11), (NULL), (22);
     
    ALTER TABLE t1 CHANGE i i1 INTEGER NOT NULL;
     
::
     
    ERROR: Cannot add NOT NULL constraint for attribute "i1": there are existing NULL values for this attribute.

.. code-block:: sql

    -- adding NOT NULL constraint
    SET SYSTEM PARAMETERS 'alter_table_change_type_strict=no';
     
    CREATE TABLE t1 (i INT);
    INSERT INTO t1 VALUES (11), (NULL), (22);
     
    ALTER TABLE t1 CHANGE i i1 INTEGER NOT NULL;
     
    SELECT * FROM t1;
     
::

               i1
    =============
               22
                0
               11

.. code-block:: sql

    -- change the column's data type (no errors)
     
    CREATE TABLE t1 (i1 INT);
    INSERT INTO t1 VALUES (1), (-2147483648), (2147483647);
     
    ALTER TABLE t1 CHANGE i1 s1 CHAR(11);
    SELECT * FROM t1;
     
::

      s1
    ======================
      '2147483647 '
      '-2147483648'
      '1          '

.. code-block:: sql

    -- change the column's data type (errors), strict mode
    SET SYSTEM PARAMETERS 'alter_table_change_type_strict=yes';
     
    CREATE TABLE t1 (i1 INT);
    INSERT INTO t1 VALUES (1), (-2147483648), (2147483647);
     
    ALTER TABLE t1 CHANGE i1 s1 CHAR(4);

::

    ERROR: ALTER TABLE .. CHANGE : changing to new domain : cast failed, current configuration doesn't allow truncation or overflow.

.. code-block:: sql

    -- change the column's data type (errors)
    SET SYSTEM PARAMETERS 'alter_table_change_type_strict=no';
     
    CREATE TABLE t1 (i1 INT);
    INSERT INTO t1 VALUES (1), (-2147483648), (2147483647);
     
    ALTER TABLE t1 CHANGE i1 s1 CHAR(4);
    SELECT * FROM t1;
     
::

    -- hard default values have been placed instead of signaling overflow

      s1
    ======================
      '1   '
      '-214'
      '2147'

.. _skip-update-null:

.. note:: 
  
    NULL을 NOT NULL로 제약 조건을 변경하는 경우 hard default로 값을 업데이트하는 과정으로 인해 많은 시간이 소요되는데, 이를 해소하기 위한 방법으로 이미 존재하는 NULL 값의 UPDATE는 생략하는 **SKIP_UPDATE_NULL** 힌트를 사용할 수 있다. 단, 이 힌트 사용 이후 사용자는 제약 조건과 불일치되는 NULL 값이 존재할 수 있음을 인지해야 한다. 
  
    .. code-block:: sql 
  
        ALTER /*+ SKIP_UPDATE_NULL */ TABLE foo MODIFY col INT NOT NULL; 

칼럼의 타입 변경에 따른 테이블 속성의 변경
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*   타입 변경: 시스템 파라미터 **alter_table_change_type_strict**\의 값이 no이면 다른 타입으로 값 변경을 허용하고, yes이면 허용하지 않는다. 기본값은 **yes**\이며, **CAST** 연산자로 허용되는 모든 타입으로 변경이 허용된다. 객체 타입의 변경은 객체의 상위 클래스(테이블)에 의해서만 허용된다. 또한 **char**, **varchar**\처럼 스트링 타입으로 변경 시 시스템 파라미터 **allow_truncated_string**\이 **no**\인 경우 오버플로된 스트링은 허용하지 않는다. 기본값은 **no**\이다.

*   **NOT NULL**

    *   변경할 칼럼에 **NOT NULL** 제약 조건이 지정되지 않더라도 새 테이블에서 해당 제약 조건이 제거되지 않고 그대로 남아있게 된다. 새 테이블에서 해당 제약 조건을 제거하려면 구문에서 **NULL**\을 지정하면 된다.
    *   변경할 칼럼에 **NOT NULL** 제약 조건이 지정되면 시스템 파라미터 **alter_table_change_type_strict** 의 설정에 따라 결과가 달라진다.

        *   **alter_table_change_type_strict** 가 yes이면 해당 칼럼의 값을 검사하여 **NULL** 이 존재하면 오류가 발생하고 변경을 수행하지 않는다.
        *   **alter_table_change_type_strict** 가 no이면 존재하는 모든 **NULL** 값을 변경할 타입의 고정 기본값(hard default value)으로 변경한다.

*   **DEFAULT**: 변경할 칼럼에 **DEFAULT** 속성이 지정되지 않더라도 새 테이블에서 해당 속성이 제거되지 않고 그대로 남아있게 된다. 새 테이블에서 해당 속성을 제거하려면 구문에서 **DEFAULT NULL**\을 지정하면 된다.

*   **COMMENT**: 변경할 칼럼에 **COMMENT** 속성이 지정되지 않더라도 새 테이블에서 해당 속성이 제거되지 않고 그대로 남아있게 된다. 새 테이블에서 해당 속성을 제거하려면 구문에서 **COMMENT ''**\을 지정하면 된다.

*   **AUTO_INCREMENT**: 변경할 칼럼에 **AUTO_INCREMENT** 속성이 지정되지 않더라도 새 테이블에서 해당 속성이 제거되지 않고 남아있게 된다.
        *   주의) **AUTO_INCREMENT** 속성은 CREATE나 ALTER로 한 번 설정한 후에는 제거할 수 없다.

*   **ON UPDATE**: 변경할 칼럼에 **ON UPDATE** 속성이 지정되지 않더라도 새 테이블에서 해당 속성이 제거되지 않고 남아있게 된다.
        *   주의) **ON UPDATE** 속성은 CREATE나 ALTER로 한 번 설정한 후에는 제거할 수 없다.

*   **FOREIGN KEY**: 참조되고 있거나 참조하고 있는 외래키(foreign key) 제약 조건을 지닌 칼럼은 변경할 수 없다.

*   단일 칼럼 **PRIMARY KEY**

    *   변경할 칼럼에 **PRIMARY KEY** 제약 조건이 지정되면, 기존 칼럼에 **PRIMARY KEY** 제약 조건이 존재하고 타입이 업그레이드되는 경우에만 **PRIMARY KEY** 가 재생성된다.
    *   변경할 칼럼에 **PRIMARY KEY** 제약 조건이 지정되었으나 기존 칼럼에는 존재하지 않으면 **PRIMARY KEY** 가 생성된다.
    *   기존 칼럼에는 **PRIMARY KEY** 제약 조건이 존재하나 변경할 칼럼에는 지정되지 않으면 **PRIMARY KEY** 는 유지된다.

*   멀티 칼럼 **PRIMARY KEY**: 변경할 칼럼에 **PRIMARY KEY** 제약 조건이 지정되고 타입이 업그레이드되면 **PRIMARY KEY** 가 재생성된다.

*   단일 칼럼 **UNIQUE KEY**

    *   타입이 업그레이드되면 **UNIQUE KEY** 가 재생성된다.
    *   기존 칼럼에 존재하고 변경할 칼럼에 지정되지 않으면 **UNIQUE KEY** 가 유지된다.
    *   기존 칼럼에 존재하지 않고 변경할 칼럼에 지정되면 **UNIQUE KEY** 가 생성된다.

*   멀티 칼럼 **UNIQUE KEY**: 해당 칼럼의 타입이 변경되면 인덱스가 재생성된다.

*   유일하지 않은(non-unique) 인덱스가 있는 칼럼: 해당 칼럼의 타입이 변경되면 인덱스가 재생성된다.

*   파티션 기준 칼럼: 테이블이 해당 칼럼에 의해 파티션되어 있으면, 칼럼을 변경할 수 없다. 파티션을 추가할 수 없다.

*   클래스 계층이 있는 테이블의 칼럼: 하위 클래스가 없는 테이블만 변경할 수 있다. 상위 클래스에서 상속받은 하위 클래스는 변경할 수 없다. 상속받은 속성은 변경할 수 없다.

*   트리거와 뷰: 트리거와 뷰는 변경할 칼럼의 정의에 따라 변경되지 않으므로 사용자가 직접 재정의해야 한다.

*   칼럼 순서: 칼럼 순서를 변경할 수 있다.

*   이름 변경: 이름이 충돌하지 않는 한 이름을 변경할 수 있다.

칼럼의 타입 변경에 따른 값의 변경
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**alter_table_change_type_strict** 파라미터는 타입 변경에 따른 값의 변환을 허용하는지 여부를 결정한다. 값이 no이면 칼럼의 타입을 변경하거나 **NOT NULL** 제약 조건을 추가할 때 값이 변경될 수 있다. 기본값은 **yes** 이다.

**alter_table_change_type_strict** 파라미터의 값이 no이면 상황에 따라 다음과 같이 동작한다. 

*   숫자 또는 문자열을 숫자로 변환 중 오버플로우 발생: 결과 타입의 부호에 따라 음수면 최소값, 양수면 최대값으로 정해지고 오버플로우가 발생한 레코드에 대한 경고 메시지가 로그에 기록된다. 문자열은 **DOUBLE** 타입으로 변환한 후 같은 법칙을 따른다. 다만, **allow_truncated_string** 설정 값이 **no** 인 경우 오버플로우 에러가 반환될 수 있다.

*   문자열을 더 짧은 문자열로 변환: 레코드는 정의한 타입의 고정 기본값(hard default value)으로 업데이트되고 경고 메시지가 로그에 기록된다. 단, **allow_truncated_string** 설정 값이 **no**\인 경우 허용되지 않을 수 있다.

*   그 밖의 이유로 인한 변환 실패: 레코드는 정의한 타입의 고정 기본값(hard default value)으로 업데이트되고 경고 메시지가 로그에 기록된다.

**alter_table_change_type_strict** 파라미터의 값이 **yes** 혹은 **allow_truncated_string** 파라미터 값이 **no**\이면 에러 메시지를 출력하고 변경 내용이 롤백될 수 있다.

**ALTER CHANGE** 문은 레코드를 업데이트하기 전에 해당 타입 변환이 가능한지 검사하지만, 특정 값은 타입 변환에 실패할 수도 있다. 예를 들어, **VARCHAR** 를 **DATE** 로 변환할 때 값의 형식이 올바르지 않으면 변환에 실패할 수 있으며, 이때에는 **DATE** 타입의 고정 기본값(hard default value)이 지정된다.

고정 기본값(hard default value)은 **ALTER TABLE ... ADD COLUMN** 문에 의한 칼럼 추가 혹은 **ALTER TABLE ... CHANGE/MODIFY** 문에 의한 타입 변환으로 인해 값이 추가되거나 변경될 때 사용되는 값이다. **ADD COLUMN** 문에서는 **add_column_update_hard_default** 시스템 파라미터에 따라 동작이 달라진다.

**타입별 고정 기본값**

+-----------+------------------+-----------------------------------------+
| 타입      | 고정 기본값 유무 | 고정 기본값                             |
+===========+==================+=========================================+
| INTEGER   | 유               | 0                                       |
+-----------+------------------+-----------------------------------------+
| FLOAT     | 유               | 0                                       |
+-----------+------------------+-----------------------------------------+
| DOUBLE    | 유               | 0                                       |
+-----------+------------------+-----------------------------------------+
| SMALLINT  | 유               | 0                                       |
+-----------+------------------+-----------------------------------------+
| DATE      | 유               | date'01/01/0001'                        |
+-----------+------------------+-----------------------------------------+
| TIME      | 유               | time'00:00'                             |
+-----------+------------------+-----------------------------------------+
| DATETIME  | 유               | datetime'01/01/0001 00:00'              |
+-----------+------------------+-----------------------------------------+
| TIMESTAMP | 유               | timestamp'00:00:01 AM 01/01/1970' (GMT) |
+-----------+------------------+-----------------------------------------+
| NUMERIC   | 유               | 0                                       |
+-----------+------------------+-----------------------------------------+
| CHAR      | 유               | ''                                      |
+-----------+------------------+-----------------------------------------+
| VARCHAR   | 유               | ''                                      |
+-----------+------------------+-----------------------------------------+
| SET       | 유               | {}                                      |
+-----------+------------------+-----------------------------------------+
| MULTISET  | 유               | {}                                      |
+-----------+------------------+-----------------------------------------+
| SEQUENCE  | 유               | {}                                      |
+-----------+------------------+-----------------------------------------+
| BIGINT    | 유               | 0                                       |
+-----------+------------------+-----------------------------------------+
| BIT       | 무               |                                         |
+-----------+------------------+-----------------------------------------+
| VARBIT    | 무               |                                         |
+-----------+------------------+-----------------------------------------+
| OBJECT    | 무               |                                         |
+-----------+------------------+-----------------------------------------+
| BLOB      | 무               |                                         |
+-----------+------------------+-----------------------------------------+
| CLOB      | 무               |                                         |
+-----------+------------------+-----------------------------------------+

칼럼의 커멘트
-------------

칼럼의 커멘트는 ADD/MODIFY/CHANGE 구문 뒤에 위치하는 <*column_definition*> 에서 지정하거나 COMMENT ON COLUMN 구문 뒤에 위치하는 <column_comment_definition> 에서 지정한다. <*column_definition*>은 위의 :ref:`CREATE TABLE 문법<column-definition>`\을 참고한다.

COMMENT ON COLUMN 구문에서는 하나 이상의 칼럼을 지정하여 칼럼 커멘트를 변경할 수 있다.
다음은 COMMENT ON COLUMN 구문을 이용해서 칼럼의 커멘트를 변경하는 예제이다.

.. code-block:: sql

    ALTER TABLE t1 COMMENT ON COLUMN c1 = 'changed table column c1 comment';
    ALTER TABLE t1 COMMENT ON COLUMN c2 = 'changed table column c2 comment', c3 = 'changed table column c3 comment';

다음은 칼럼의 커멘트를 확인하는 예제이다.

.. code-block:: sql

    SHOW CREATE TABLE t1 /* table_name */ ;

    SELECT attr_name, class_name, comment 
    FROM db_attribute
    WHERE class_name = 't1' /* lowercase_table_name */ ;

    SHOW FULL COLUMNS FROM t1 /* table_name */ ;

CSQL 인터프리터에서 ";sc table_name" 명령으로도 확인할 수 있다.

::

    $ csql -u dba demodb
    
    csql> ;sc t1

.. _rename-column:

RENAME COLUMN 절
----------------

**RENAME COLUMN** 절을 사용하여 칼럼의 이름을 변경할 수 있다. ::

    ALTER [TABLE | CLASS | VCLASS | VIEW] [schema_name.]table_name
    RENAME [COLUMN | ATTRIBUTE] old_column_name {AS | TO} new_column_name

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 이름을 변경할 칼럼의 테이블 이름을 지정한다.
*   *old_column_name*: 현재의 칼럼 이름을 지정한다.
*   *new_column_name*: 새로운 칼럼 이름을 **AS** 키워드 뒤에 명시한다(최대 254 바이트).

.. code-block:: sql

    CREATE TABLE a_tbl (id INT, name VARCHAR(50));
    ALTER TABLE a_tbl RENAME COLUMN name AS name1;

DROP COLUMN 절
--------------

**DROP COLUMN** 절을 사용하여 테이블에 존재하는 칼럼을 삭제할 수 있다. 삭제하고자 하는 칼럼들을 쉼표(,)로 구분하여 여러 개의 칼럼을 한 번에 삭제할 수 있다. ::

    ALTER [TABLE | CLASS | VCLASS | VIEW] [schema_name.]table_name
    DROP [COLUMN | ATTRIBUTE] column_name, ... ;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 삭제할 칼럼의 테이블 이름을 명시한다.
*   *column_ name*: 삭제할 칼럼의 이름을 명시한다. 쉼표로 구분하여 여러 개의 칼럼을 지정할 수 있다.

.. code-block:: sql

    ALTER TABLE a_tbl DROP COLUMN age1,age2,age3;

DROP CONSTRAINT 절
------------------

**DROP CONSTRAINT** 절을 사용하여, 테이블에 이미 정의된 **UNIQUE**, **PRIMARY KEY**, **FOREIGN KEY** 제약 조건을 삭제할 수 있다. 삭제할 제약 조건 이름을 지정해야 하며, 이는 CSQL 명령어( **;schema table_name** )를 사용하여 확인할 수 있다. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name
    DROP CONSTRAINT constraint_name ;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 제약 조건을 삭제할 테이블의 이름을 지정한다.
*   *constraint_name*: 삭제할 제약 조건의 이름을 지정한다.

.. code-block:: sql

    CREATE TABLE a_tbl (
      id INT NOT NULL DEFAULT 0 PRIMARY KEY,
      phone VARCHAR(10)
    );
     
    CREATE TABLE b_tbl (
      ID INT NOT NULL,
      name VARCHAR (10) NOT NULL,
      CONSTRAINT u_name UNIQUE (name), 
      CONSTRAINT pk_id PRIMARY KEY (id),
      CONSTRAINT fk_id FOREIGN KEY (id) REFERENCES a_tbl (id)
      ON DELETE CASCADE ON UPDATE RESTRICT
    );
    
    ALTER TABLE b_tbl DROP CONSTRAINT pk_id;
    ALTER TABLE b_tbl DROP CONSTRAINT fk_id;
    ALTER TABLE b_tbl DROP CONSTRAINT u_name;

DROP INDEX 절
-------------


**DROP INDEX** 절을 사용하여 인덱스를 삭제할 수 있다. 고유 인덱스는 **DROP CONSTRAINT** 절로도 삭제할 수 있다.

::

    ALTER [TABLE | CLASS] [schema_name.]table_name DROP INDEX index_name ;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 제약 조건을 삭제할 테이블의 이름을 지정한다.
*   *index_name*: 삭제할 인덱스의 이름을 지정한다.

.. code-block:: sql

    ALTER TABLE a_tbl DROP INDEX i_a_tbl_age;

DROP PRIMARY KEY 절
-------------------

**DROP PRIMARY KEY** 절을 사용하여 테이블에 정의된 기본키 제약 조건을 삭제할 수 있다. 하나의 테이블에는 하나의 기본키만 정의될 수 있으므로 기본키 제약 조건 이름을 지정하지 않아도 된다. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name DROP PRIMARY KEY ;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 기본키 제약 조건을 삭제할 테이블의 이름을 지정한다.

.. code-block:: sql

    ALTER TABLE a_tbl DROP PRIMARY KEY;

DROP FOREIGN KEY 절
-------------------

**DROP FOREIGN KEY** 절을 사용하여 테이블에 정의된 외래키 제약 조건을 모두 삭제할 수 있다. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name DROP FOREIGN KEY constraint_name ;

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 제약 조건을 삭제할 테이블의 이름을 지정한다.
*   *constraint_name*: 삭제할 외래키 제약 조건의 이름을 지정한다.

.. code-block:: sql

    ALTER TABLE b_tbl ADD CONSTRAINT fk_id FOREIGN KEY (id) REFERENCES a_tbl (id);
    ALTER TABLE b_tbl DROP FOREIGN KEY fk_id;

DROP TABLE
==========

**DROP** 구문을 이용하여 기존의 테이블을 삭제할 수 있다. 하나의 **DROP** 구문으로 여러 개의 테이블을 삭제할 수 있으며 테이블이 삭제되면 포함된 행도 모두 삭제된다. **IF EXISTS** 절을 함께 사용하면 해당 테이블이 존재하지 않더라도 에러가 발생하지 않는다. 

::

    DROP [TABLE | CLASS] [IF EXISTS] <table_specification_comma_list> [CASCADE CONSTRAINTS] ;

        <table_specification_comma_list> ::= 
            <single_table_spec> | (<table_specification_comma_list>) 

            <single_table_spec> ::= 
                | [ONLY] [schema_name.]table_name 
                | ALL [schema_name.]table_name [( EXCEPT [schema_name.]table_name, ... )] 

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 삭제할 테이블의 이름을 지정한다. 쉼표로 구분하여 여러 개의 테이블을 한 번에 삭제할 수 있다.
*   **ONLY** 키워드 뒤에 수퍼클래스 이름이 명시되면, 해당 수퍼클래스만 삭제하고 이를 상속받는 서브클래스는 삭제하지 않는다.
*   **ALL** 키워드 뒤에 수퍼클래스 이름이 지정되면, 해당 수퍼클래스 및 이를 상속받는 서브클래스를 모두 삭제한다.
*   **EXCEPT** 키워드 뒤에 삭제하지 않을 서브클래스 리스트를 명시할 수 있다.
*	**CASCADE CONSTRAINTS**: 테이블이 DROP되고 이 테이블을 참조하는 다른 테이블들의 외래 키도 DROP된다.

.. code-block:: sql

    CREATE TABLE b_tbl (i INT);
    CREATE TABLE a_tbl (i INT);
     
    -- DROP TABLE IF EXISTS
    DROP TABLE IF EXISTS b_tbl, a_tbl;
     
    SELECT * FROM a_tbl;
    
::

    ERROR: Unknown class "a_tbl".

*   **CASCADE CONSTRAINTS** 가 명시되면 다른 테이블들이 DROP할 테이블의 기본 키를 참조하더라도 지정된 테이블은 DROP되며, 이 테이블을 참조하는 다른 테이블들의 외래 키 역시 DROP된다. 단, 참조하는 테이블들의 데이터는 삭제되지 않는다. 

다음은 b_child 테이블이 참조하는 a_parent 테이블을 DROP하는 예이다. b_child의 외래 키 역시 DROP되며, b_child의 데이터는 유지된다. 

.. code-block:: sql 

    CREATE TABLE a_parent ( 
        id INTEGER PRIMARY KEY, 
        name VARCHAR(10) 
    ); 
    CREATE TABLE b_child ( 
        id INTEGER PRIMARY KEY, 
        parent_id INTEGER, 
        CONSTRAINT fk_parent_id FOREIGN KEY(parent_id) REFERENCES a_parent(id) ON DELETE CASCADE ON UPDATE RESTRICT 
    ); 

    DROP TABLE a_parent CASCADE CONSTRAINTS;     

RENAME TABLE
============

**RENAME TABLE** 구문을 사용하여 테이블 이름을 변경할 수 있으며, 여러 개의 테이블 이름을 변경하는 경우 테이블 이름 리스트를 명시할 수 있다. ::

    RENAME  [TABLE | CLASS] [schema_name.]old_table_name {AS | TO} [schema_name.]new_table_name [{, [schema_name.]old_table_name {AS | TO} [schema_name.]new_table_name}];

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다. 변경할 테이블의 스키마와 새로운 테이블의 스키마가 동일해야 한다.
*   *old_table_name*: 변경할 테이블의 이름을 지정한다.
*   *new_table_name*: 새로운 테이블 이름을 지정한다(최대 254 바이트).

.. code-block:: sql

    RENAME TABLE a_tbl AS aa_tbl;
    RENAME TABLE aa_tbl TO a1_tbl, b_tbl TO b1_tbl;

.. note::

    테이블의 소유자, **DBA**, **DBA** 의 멤버만이 테이블의 이름을 변경할 수 있으며, 그 밖의 사용자는 소유자나 **DBA** 로부터 이름을 변경할 수 있는 권한을 받아야 한다(권한 관련 사항은 :ref:`granting-authorization` 참조).
