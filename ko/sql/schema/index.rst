******
인덱스
******

CREATE INDEX
============

**CREATE INDEX** 구문을 이용하여 지정한 테이블에 인덱스를 생성한다. 인덱스 이름 작성 원칙은 :doc:`/sql/identifier` 를 참고한다.

인덱스 힌트 구문, 내림차순 인덱스, 커버링 인덱스, 인덱스 스킵 스캔, **ORDER BY** 최적화, **GROUP BY** 최적화 등 인덱스를 이용하는 방법과 필터링된 인덱스, 함수 인덱스를 생성하는 방법에 대해서는 :doc:`/sql/tuning` 을 참고한다.

::

    CREATE [ UNIQUE ] INDEX index_name
    ON table_name <index_col_desc>
     
    <index_col_desc> ::=
        ( column_name[(prefix_length)] [ASC | DESC] [ {, column_name[(prefix_length)] [ASC | DESC]} ...] ) [ WHERE <filter_predicate> ]
        | (function_name (argument_list) )

*   **UNIQUE** : 유일한 값을 갖는 고유 인덱스를 생성한다.
*   *index_name* : 생성하려는 인덱스의 이름을 명시한다. 인덱스 이름은 테이블 안에서 고유한 값이어야 한다.
*   *prefix_length* : 문자열 또는 비트열 타입의 칼럼에 인덱스를 설정하는 경우, 칼럼 값의 앞 부분 일부를 prefix로 지정하여 인덱스를 생성하기 위하여 칼럼 이름 뒤 괄호 안에 문자 개수로 prefix 길이를 지정할 수 있다. 단, *prefix_length* 는 다중 칼럼 인덱스 및 **UNIQUE** 인덱스에는 지정할 수 없다. 또한 *prefix_length* 를 호스트 변수로 지정하여 인덱스를 생성할 수 없다. *prefix_length* 가 지정된 인덱스에서 질의 결과의 순서를 보장하려면 반드시 **ORDER BY** 절을 명시해야 한다.

*   *table_name* : 인덱스를 생성할 테이블의 이름을 명시한다.
*   *column_name* : 인덱스를 적용할 칼럼의 이름을 명시한다. 다중 칼럼 인덱스를 생성할 경우 둘 이상의 칼럼 이름을 명시한다.
*   **ASC** | **DESC** : 칼럼의 정렬 방향을 설정한다.
*   <*filter_predicate*> : 필터링된 인덱스를 만드는 조건을 명시한다. 칼럼과 상수 간 비교 조건이 여러 개인 경우 **AND** 로 연결된 경우에만 필터가 될 수 있다.
*   *function_name* (*argument_list*) : 함수 기반 인덱스를 만드는 조건을 명시한다.

.. warning::

    CUBRID 9.0 미만 버전에서는 인덱스 이름을 생략할 수 있었으나, CUBRID 9.0 버전부터는 인덱스 이름을 생략할 수 없다.

다음은 내림차순으로 정렬된 인덱스를 생성하는 예제이다.

.. code-block:: sql

    CREATE INDEX gold_index ON participant(gold DESC);

다음은 다중 칼럼 인덱스를 생성하는 예제이다.

.. code-block:: sql

    CREATE INDEX name_nation_idx ON athlete(name, nation_code);

다음은 prefix 인덱스를 생성하는 예제이다. 문자열 타입으로 정의한 *nation_code* 칼럼에 대해서 1바이트 길이만큼 prefix를 지정하여 인덱스를 생성한다.

.. code-block:: sql

    CREATE INDEX idx_game_nation_code ON game(nation_code(1));

ALTER INDEX
===========

**ALTER INDEX** 문을 사용하여 인덱스를 재생성한다. 즉, 인덱스를 삭제하고 다시 생성한다. **ON** 절 뒤에 테이블 이름과 칼럼 이름이 추가되면 해당 테이블 이름과 칼럼 이름으로 인덱스를 재생성한다. ::

    ALTER [ UNIQUE ] INDEX index_name
    ON { ONLY } table_name <index_col_desc> REBUILD [ ; ]
     
    <index_col_desc> ::=
        ( column_name[ {, column_name} ...] ) [ WHERE <filter_predicate> ]
        | (function_name (argument_list) )

*   **UNIQUE** : 재생성하려는 인덱스가 고유 인덱스임을 지정한다.
*   *index_name* : 재생성하려는 인덱스의 이름을 명시한다. 인덱스 이름은 테이블 안에서 고유한 값이어야 한다.
*   *table_name* : 인덱스를 재생성할 테이블의 이름을 명시한다.
*   *column_name* : 인덱스를 적용할 칼럼의 이름을 명시한다. 다중 칼럼 인덱스를 생성할 경우 둘 이상의 칼럼 이름을 명시한다.
*   <*filter_predicate*> : 필터링된 인덱스를 만드는 조건을 명시한다. 칼럼과 상수 간 비교 조건이 여러 개인 경우 **AND** 로 연결된 경우에만 필터가 될 수 있다.
*   *function_name* (*argument_list*) : 함수 기반 인덱스를 만드는 조건을 명시한다.

.. warning::

    CUBRID 9.0 미만 버전에서는 인덱스 이름을 생략할 수 있었으나, CUBRID 9.0 버전부터는 인덱스 이름을 생략할 수 없다.

다음은 인덱스를 재생성하는 여러 가지 방법을 보여주는 예제이다.

.. code-block:: sql

    ALTER INDEX i_game_medal ON game(medal) REBUILD;
    ALTER INDEX game_date_idx REBUILD;
    ALTER INDEX char_idx ON athlete(gender, nation_code) WHERE gender='M' AND nation_code='USA' REBUILD;

DROP INDEX
==========

**DROP INDEX** 문을 사용하여 인덱스를 삭제할 수 있다. ::

    DROP [ UNIQUE ] INDEX index_name
    [ON table_name] [ ; ]

*   **UNIQUE** : 삭제하려는 인덱스가 고유 인덱스임을 지정한다. 고유 인덱스는 **DROP CONSTRAINT** 절로도 삭제할 수 있다.
*   *index_name* : 삭제할 인덱스의 이름을 지정한다.
*   *table_name* : 삭제할 인덱스가 지정된 테이블 이름을 지정한다.

다음은 인덱스를 삭제하는 예제이다.

.. code-block:: sql

    DROP INDEX game_date_idx ON game;
