
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
                [COMMENT 'index_comment_string']

*   **UNIQUE**: 유일한 값을 갖는 고유 인덱스를 생성한다.
*   *index_name*: 생성하려는 인덱스의 이름을 명시한다. 인덱스 이름은 테이블 안에서 고유한 값이어야 한다.

*   *table_name*: 인덱스를 생성할 테이블의 이름을 명시한다.
*   *column_name*: 인덱스를 적용할 칼럼의 이름을 명시한다. 다중 칼럼 인덱스를 생성할 경우 둘 이상의 칼럼 이름을 명시한다.
*   **ASC** | **DESC**: 칼럼의 정렬 방향을 설정한다.

*   <*filter_predicate*>: 필터링된 인덱스를 만드는 조건을 명시한다. 컬럼과 상수 간 비교 조건이 여러 개인 경우 **AND** 로 연결된 경우에만 필터링이 될 수 있다. 자세한 내용은 :ref:`filtered-index` 를 참고한다.

*   *function_name* (*argument_list*): 함수 기반 인덱스를 만드는 조건을 명시한다. 이와 관련하여 :ref:`function-index`\ 를 반드시 참고한다.

*   *index_comment_string*: 인덱스의 커멘트를 지정한다.

..  note::

    *   CUBRID 9.0 버전부터는 인덱스 이름을 생략할 수 없다.

    *   prefix 인덱스 기능은 제거될 예정(deprecated)이므로, 더 이상 사용을 권장하지 않는다.

    *   데이터베이스의 TIMESTAMP, TIMESTAMP WITH LOCAL TIME ZONE 또는 DATETIME WITH LOCAL TIME ZONE 타입 컬럼에 인덱스 또는 함수 인덱스가 포함되어 있는 경우 세션 및 서버 타임존(:ref:`timezone-parameters`)을 변경하면 안 된다.
    
    *   데이터베이스의 TIMESTAMP 또는 TIMESTAMP WITH LOCAL TIME ZONE 타입 컬럼에 인덱스 또는 함수 인덱스가 포함되어 있는 경우 윤초를 지원하는 파라미터(:ref:`timezone-parameters`)를 변경하면 안 된다.

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

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc tbl

.. _alter-index:

ALTER INDEX
===========

**ALTER INDEX** 질의문은 인덱스를 재생성하거나 인덱스의 커멘트를 추가/변경한다. 인덱스 재생성은 인덱스를 삭제하고 다시 생성하는 작업이다.

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

::

    ALTER INDEX index_name ON table_name COMMENT 'index_comment_string' ;
    
다음은 인덱스 재생성 없이 커멘트만 추가 또는 변경하는 구문이다.

.. code-block:: sql
    
    ALTER INDEX i_game_medal ON game COMMENT 'change index comment' ;

다음은 인덱스 이름을 바꾸는 구문이다. 

:: 

    ALTER INDEX old_index_name ON table_name RENAME TO new_index_name [COMMENT 'index_comment_string'] ;


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
