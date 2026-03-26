
:meta-keywords: index definition, unique index, create index, alter index, drop index
:meta-description: Define table indexes using create index, alter index, drop index statements.


*************
인덱스 정의문
*************

CREATE INDEX
============

**CREATE INDEX** 구문을 이용하여 지정한 테이블에 인덱스를 생성한다. 인덱스 이름 작성 원칙은 :doc:`/sql/identifier` 를 참고한다.

인덱스 힌트 구문, 내림차순 인덱스, 커버링 인덱스, 인덱스 스킵 스캔, **ORDER BY** 최적화, **GROUP BY** 최적화 등 인덱스를 이용하는 방법과 필터링된 인덱스, 함수 인덱스를 생성하는 방법에 대해서는 :doc:`/sql/tuning` 을 참고한다.

::

    CREATE [UNIQUE] INDEX index_name ON table_name <index_col_desc> ;
     
        <index_col_desc> ::=
            { ( column_name [ASC | DESC] [ {, column_name [ASC | DESC]} ...] ) [ WHERE <filter_predicate> ] |
            (function_name (argument_list) ) }
                { [[WITH ONLINE [PARALLEL parallel_count]] | [INVISIBLE] | [VISIBLE]] }
                [COMMENT 'index_comment_string']

*   **UNIQUE**: 유일한 값을 갖는 고유 인덱스를 생성한다.
*   *index_name*: 생성하려는 인덱스의 이름을 명시한다. 인덱스 이름은 테이블 안에서 고유한 값이어야 한다.

*   *table_name*: 인덱스를 생성할 테이블의 이름을 명시한다.
*   *column_name*: 인덱스를 적용할 칼럼의 이름을 명시한다. 다중 칼럼 인덱스를 생성할 경우 둘 이상의 칼럼 이름을 명시한다.
*   **ASC** | **DESC**: 칼럼의 정렬 방향을 설정한다.

*   <*filter_predicate*>: 필터링된 인덱스를 만드는 조건을 명시한다. 컬럼과 상수 간 비교 조건이 여러 개인 경우 **AND** 로 연결된 경우에만 필터링이 될 수 있다. 자세한 내용은 :ref:`filtered-index` 를 참고한다.

*   *function_name* (*argument_list*): 함수 기반 인덱스를 만드는 조건을 명시한다. 이와 관련하여 :ref:`function-index`\ 를 반드시 참고한다.
*   **WITH ONLINE**: 다른 트랜잭션들에 의해 테이블 데이터가 변경중에 인덱스의 생성을 허용한다. **PARALLEL** 이 선언되지 않은 경우, 인덱스는 동일 트랜잭션 스레드에서 생성된다. <parallel_count>는 인덱스를 생성하기 위해 사용되는 스레드의 갯수이며 1부터 16사이의 정수이다.
*   **INVISIBLE**: 인덱스를 생성할 때 인덱스의 상태를 **INVISIBLE** 로 설정한다. 이것은 질의의 실행이 인덱스의 생성과 무관하게 실행된다는 것이다. **INVISIBLE** 이 생략된 경우 생성되는 인덱스의 상태는 **NORNAL_INDEX** 로 설정된다.

*   *index_comment_string*: 인덱스의 커멘트를 지정한다.

..  note::

    *   CUBRID 9.0 버전부터는 인덱스 이름을 생략할 수 없다.

    *   prefix 인덱스 기능은 제거될 예정(deprecated)이므로, 더 이상 사용을 권장하지 않는다.

    *   데이터베이스의 TIMESTAMP, TIMESTAMP WITH LOCAL TIME ZONE 또는 DATETIME WITH LOCAL TIME ZONE 타입 컬럼에 인덱스 또는 함수 인덱스가 포함되어 있는 경우 세션 및 서버 타임존(:ref:`timezone-parameters`)을 변경하면 안 된다.
    
    *   데이터베이스의 TIMESTAMP 또는 TIMESTAMP WITH LOCAL TIME ZONE 타입 컬럼에 인덱스 또는 함수 인덱스가 포함되어 있는 경우 윤초를 지원하는 파라미터(:ref:`timezone-parameters`)를 변경하면 안 된다.

    *   **PARALLEL** 옵션에 설정된 스레드의 개수는 인덱스 로드 과정에서만 적용된다. 수집 과정은 해당 트랜잭션에서 단일 스레드로 실행된다. 이 과정에서 레코드들은 특정 단위의 묶음으로 수집되고, 묶음들의 크기가 16M에 도달한 경우 인덱스 로딩 스레드에 전달하고 (또는 큐에 입력한다), 수집 프로세스는 계속 진행한다. 한 묶음의 크기는 인덱스 길이와 열의 식별자 길이로 결정한다.
    
    *   stand-alone 모드에서 WITH ONLINE 옵션은 무시된다 (정상적인 인덱스가 생성된다).

    *   온라인 인덱스 생성은 다음 3 단계로 실행된다:

        * INDEX IS IN ONLINE BUILDING 상태로 스키마에 인덱스를 추가한다. 이 과정은 테이블에 SCH_M_LOCK (다른 모든 스키마 변경 때와 같이)을 확보한 상태에서 실행된다. 이후, 잠금은 IX_LOCK으로 강등된다.

        * 인덱스 채우기: 힙파일을 묶음 단위로 검사, 묶음들을 정렬하고 인덱스에 키를 추가한다. 이 과정 중에 다른 트랜잭션들은 테이블 데이터를 수정할 수있다 (인덱스도 커밋된 다른 트랜잭션에 의해서 수정된 데이터와 함께 변경된다).

        * 인덱스 상태를 NORMAL INDEX로 변경; 이 과정은 테이블 잠금을 다시 SCH_M_LOCK으로 승격한 후에 수행된다.

    * 구성중인 온라인 인덱스는 다른 트랜잭션에서 SHOW문으로 표시된다. 다른 트랜잭션에서는 MVCC 스냅샷 때문에  :ref:`_db_index <db-index>`  시스템 테이블에서는 보이지 않는다 (다른 트랜잭션은 이 테이블에서 커밋된 항목만 볼 수있다).

    * 온라인 인덱스 구성과 병렬로 실행중인 트랜잭션이 인덱스에 대하여 고유 키 위반을 유발하는 명령을 실행하는 경우, 그 트랜잭션은 커밋이 허용된다. 온라인 인덱스 구성은 계속 진행되고 최종 단계 (스키마에 NORMAL INDEX 상태로 설정)에 이르기 전에 고유키 제약의 유효성을 검사한다. 고유 위반인 경우 인덱스 생성이 중단된다. 사용자는 고유 제약 조건이 보장된 후에 이 작업을 다시 시작해야한다.

다음은 내림차순으로 정렬된 인덱스를 생성하는 예제이다.

.. code-block:: sql

    CREATE INDEX gold_index ON participant(gold DESC);

다음은 다중 칼럼 인덱스를 생성하는 예제이다.

.. code-block:: sql

    CREATE INDEX name_nation_idx ON athlete(name, nation_code) COMMENT 'index comment';

인덱스의 커멘트
---------------

인덱스의 커멘트를 다음과 같이 지정할 수 있다. 

.. code-block:: sql

    CREATE TABLE tbl (a int default 0, b int, c int);

    CREATE INDEX i_tbl_b on tbl (b) COMMENT 'index comment for i_tbl_b';

    CREATE TABLE tbl2 (a INT, index i_tbl_a (a) COMMENT 'index comment', b INT);

    ALTER TABLE tbl2 ADD INDEX i_tbl2_b (b) COMMENT 'index comment b';

지정된 인덱스의 커멘트는 다음 구문에서 확인할 수 있다.

.. code-block:: sql

    SHOW CREATE TABLE table_name;
    SELECT index_name, class_name, comment from db_index WHERE class_name ='classname';
    SHOW INDEX FROM table_name;

또는 CSQL 인터프리터에서 테이블의 스키마를 출력하는 ;sc 명령으로 인덱스의 커멘트를 확인할 수 있다.

.. code-block:: shell

    $ csql -u dba demodb

    csql> ;sc tbl

온라인 인덱스 생성
--------------------

다른 트랜잭션이 테이블에 데이터를 추가하거나 갱신을 허용하면서 인덱스를 생성할 수있다.

.. code-block:: sql

    CREATE TABLE t1 (i1 int, i2 int);

    CREATE INDEX i_t1_i1 on t1 (i1) WITH ONLINE PARALLEL 10;

다른 트랜잭션에서 온라인 인덱스 출력
------------------------------------------------

다른 트랜잭션은 스키마 관련 문장으로 온라인 인덱스를 볼 수있다:

.. code-block:: sql

       csql> show index in t1;

       === <Result of SELECT Command in Line 1> ===

         Table                  Non_unique  Key_name              Seq_in_index  Column_name           Collation             Cardinality     Sub_part  Packed                Null                  Index_type            Func                  Comment               Visible
       =================================================================================================================================================================================================================================================================================
         't1'                            1  'i_t1'                           1  'i1'                  'A'                             0         NULL  NULL                  'YES'                 'BTREE'               NULL                  NULL                  'NO'

       1 row selected. (0.020779 sec) Committed.

       1 command(s) successfully processed.
       csql> desc t1;

       === <Result of SELECT Command in Line 1> ===

         Field                 Type                  Null                  Key                   Default               Extra
       ====================================================================================================================================
         'i1'                  'INTEGER'             'YES'                 'MUL'                 NULL                  ''
         'i2'                  'INTEGER'             'YES'                 ''                    NULL                  ''

       csql> ;schema t1

       === <Help: Schema of a Class> ===


        <Class Name>

            t1

        <Attributes>

            i1                   INTEGER
            i2                   INTEGER

        <Constraints>

            INDEX i_t1 ON t1 (i1) IN PROGRESS

다른 트랜잭션이 고유키 위반을 유발하는 삽입을 실행하는 도중의 온라인 인덱스
-------------------------------------------------------------------------------------------------

+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| session 1                                                         | session 2                                                                         |
+===================================================================+===================================================================================+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   CREATE TABLE t1 (i1 int, i2 int);                               |                                                                                   |
|                                                                   |                                                                                   |
|   COMMIT WORK;                                                    |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|   INSERT INTO t1 VALUES (1, 10);                                  |                                                                                   |
|                                                                   |                                                                                   |
|   CREATE UNIQUE INDEX i_t1_i1 on t1 (i1) WITH ONLINE;             |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                   | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|                                                                   |    csql> ;schema t1                                                               |
|                                                                   |                                                                                   |
|                                                                   |    === <Help: Schema of a Class> ===                                              |
|                                                                   |                                                                                   |
|                                                                   |                                                                                   |
|                                                                   |     <Class Name>                                                                  |
|                                                                   |                                                                                   |
|                                                                   |         t1                                                                        |
|                                                                   |                                                                                   |
|                                                                   |     <Attributes>                                                                  |
|                                                                   |                                                                                   |
|                                                                   |         i1                   INTEGER                                              |
|                                                                   |         i2                   INTEGER                                              |
|                                                                   |                                                                                   |
|                                                                   |     <Constraints>                                                                 |
|                                                                   |                                                                                   |
|                                                                   |         UNIQUE i_t1 ON t1 (i1) IN PROGRESS                                        |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                   | .. code-block:: sql                                                               |
|                                                                   |                                                                                   |
|                                                                   |  INSERT INTO t1 VALUES (1, 20);                                                   |
|                                                                   |                                                                                   |
|                                                                   |  COMMIT WORK;                                                                     |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| .. code-block:: sql                                               |                                                                                   |
|                                                                   |                                                                                   |
|  COMMIT WORK;                                                     |                                                                                   |
|                                                                   |                                                                                   |
|   ERROR: Operation would have caused one or more unique constraint|                                                                                   |
|                                                                   |                                                                                   |
|   violations. INDEX i_t1(B+tree: 0|3456|3457) ON                  |                                                                                   |
|                                                                   |                                                                                   |
|   CLASS t1(CLASS_OID: 0|202|7). key: *UNKNOWN-KEY*.               |                                                                                   |
|                                                                   |                                                                                   |
+-------------------------------------------------------------------+-----------------------------------------------------------------------------------+

.. _alter-index:

ALTER INDEX
===========

**ALTER INDEX** 문은 인덱스의 특성을 변경한다. 주석 또는 상태만 변경된 경우를 제외하고 인덱스가 재구성된다. 인덱스 재구성은 인덱스를 제거하고 다시 생성하는 작업이다.

다음은 인덱스를 재생성하는 구문이다.

::

    ALTER INDEX index_name ON table_name REBUILD;

*   *index_name*: 재생성하려는 인덱스의 이름을 명시한다. 인덱스 이름은 테이블 안에서 고유한 값이어야 한다.
*   *table_name*: 인덱스를 재생성할 테이블의 이름을 명시한다.
*   **REBUILD**: 이미 생성된 것과 같은 구조의 인덱스를 재생성한다.
*   *index_comment_string*: 인덱스의 커멘트를 지정한다.

.. note::

    *   CUBRID 9.0 버전부터는 인덱스 이름을 생략할 수 없다.

    *   CUBRID 10.0 버전부터는 테이블 이름을 생략할 수 없다.
    
    *   CUBRID 10.0 버전부터는 테이블 이름 뒤에 칼럼 이름을 추가하더라도 이는 무시되며, 예전 인덱스와 동일한 칼럼으로 재생성된다.

    *   prefix 인덱스 기능은 제거될 예정(deprecated)이므로, 더 이상 사용을 권장하지 않는다.

다음은 인덱스를 재생성하는 구문이다.

.. code-block:: sql

    CREATE INDEX i_game_medal ON game(medal);
    ALTER INDEX i_game_medal ON game COMMENT 'rebuild index comment' REBUILD ;

인덱스를 재생성하지 않고 인덱스의 커멘트를 추가하거나 변경하려는 경우 다음과 같이 **COMMENT** 절을 추가하고 **REBUILD** 키워드를 제거한다.

.. code-block:: sql

    ALTER INDEX index_name ON table_name COMMENT 'index_comment_string' ;
    
다음은 인덱스 재생성 없이 커멘트만 추가 또는 변경하는 구문이다.

.. code-block:: sql
    
    ALTER INDEX i_game_medal ON game COMMENT 'change index comment' ;

다음은 인덱스 이름을 바꾸는 구문이다. 

.. code-block:: sql

    ALTER INDEX old_index_name ON table_name RENAME TO new_index_name [COMMENT 'index_comment_string'] ;

다음은 인덱스의 상태를 **INVISIBLE**/**VISIBLE** 로 변경하기 위한 구문이다. 인덱스의 상태가 **INVISIBLE** 인 경우, 질의 실행은 인덱스가 없는 것처럼 수행된다. 이 방법으로 인덱스의 성능 측정이 가능하며, 실제로 인덱스를 제거하지 않고 인덱스 제거에 따른 영향도를 측정할 수있다.

.. code-block:: sql

    CREATE INDEX i_game_medal ON game(medal);
    ALTER INDEX i_game_medal ON game VISIBLE;
    ALTER INDEX i_game_medal ON game INVISIBLE;

DROP INDEX
==========

**DROP INDEX** 문을 사용하여 인덱스를 삭제할 수 있다. 고유 인덱스는 **DROP CONSTRAINT** 절로도 삭제할 수 있다.

::

    DROP INDEX index_name ON table_name ;

*   *index_name*: 삭제할 인덱스의 이름을 지정한다.
*   *table_name*: 삭제할 인덱스가 지정된 테이블 이름을 지정한다.

.. warning::

    CUBRID 10.0 버전부터는 테이블 이름을 생략할 수 없다.

다음은 인덱스를 삭제하는 예제이다.

.. code-block:: sql

    DROP INDEX i_game_medal ON game;
