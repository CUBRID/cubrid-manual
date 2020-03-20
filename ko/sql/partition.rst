
:meta-keywords: cubrid partition, partitioning key, range partition, hash partition, list partition, partition pruning
:meta-description: Partitioning is a method by which a table is divided into multiple independent physical units called partitions. In CUBRID, each partition is a table implemented as a subclass of the partitioned table.


****
분할
****

.. _partitioning-key:

분할 키
=======

분할 키는 정의된 분할들에 데이터를 분배하기 위해 분할 방식에서 사용하는 표현식이다. 분할 키로 사용할 수 있는 칼럼의 데이터 타입은 다음과 같다.

*   **CHAR**
*   **VARCHAR**
*   **SMALLINT**
*   **INT**
*   **BIGINT**
*   **DATE**
*   **TIME**
*   **TIMESTAMP**
*   **DATETIME**

분할 키에는 다음과 같은 제약 사항이 적용된다.

*   분할 키는 분할 테이블에서 하나의 칼럼만을 사용해야 한다.
*   :doc:`집계 함수 및 분석 함수<function/analysis_fn>`, :doc:`논리 연산자<function/logical_op>`, :doc:`비교 연산자 <function/comparison_op>`\ 는 분할 키 표현식에 사용할 수 없다.
*   다음 함수 및 표현식은 분할 키 표현식에서 허용되지 않는다.

    *   :ref:`CASE <case-expr>` 
    *   :func:`CHARSET` 
    *   :func:`CHR` 
    *   :func:`COALESCE` 
    *   :func:`SERIAL_CURRENT_VALUE` 
    *   :func:`SERIAL_NEXT_VALUE` 
    *   :func:`DECODE`
    *   :func:`DECR` 
    *   :func:`INCR`
    *   :func:`DRAND` 
    *   :func:`DRANDOM` 
    *   :func:`GREATEST` 
    *   :func:`LEAST` 
    *   :func:`IF` 
    *   :func:`IFNULL` 
    *   :func:`INSTR` 
    *   :func:`NVL` 
    *   :func:`NVL2` 
    *   :c:macro:`ROWNUM` 
    *   :func:`INST_NUM` 
    *   :c:macro:`USER` 
    *   :ref:`PRIOR <prior-operator>` 
    *   :func:`WIDTH_BUCKET`
*       각각의 고유 인덱스 키 또는  기본 키는 분할 키를 포함해야 한다.  이에 대한 자세한 내용은 :ref:`여기<index-partitions>` 를 참고한다.
*       분할 표현식의 길이는 1024바이트를 초과하면 안 된다.

.. _range-partitioning:

영역 분할
=========

영역 분할(range partitioning)은 각 분할에 대해 지정된 값의 영역으로 테이블을 분할하는 방법이다. 범위는 겹치지 않는 연속된 구간으로 정의된다. 이 분할 방법은 테이블의 데이터가 영역 구간으로 나누어질 수 있을 때 가장 유용한 방법이다. 예를 들면, 주문 정보 테이블에서 주문 날짜 또는 사용자 테이블에서 나이 영역으로 분할하는 경우이다. 영역 분할은 거의 모든 검색 조건이 영역을 매칭하는데 사용될 수 있기 때문에 :ref:`partition-pruning` 측면에서 가장 다양하게 활용되는 분할 기법이다.

테이블은 **CREATE** 또는 **ALTER** 문에서 **PARTITION BY RANGE** 절을 사용하여 분할될 수 있다. ::

    CREATE TABLE table_name (
       ...
    )
    PARTITION BY RANGE ( <partitioning_key> ) (
        PARTITION partition_name VALUES LESS THAN ( <range_value> ) [COMMENT 'comment_string'] ,
        PARTITION partition_name VALUES LESS THAN ( <range_value> ) [COMMENT 'comment_string'] ,
        ... 
    )
    
    ALTER TABLE table_name 
    PARTITION BY RANGE ( <partitioning_key> ) (
        PARTITION partition_name VALUES LESS THAN ( <range_value> ) [COMMENT 'comment_string'] ,
        PARTITION partition_name VALUES LESS THAN ( <range_value> ) [COMMENT 'comment_string'] ,
        ... 
    )

*   *partitioning_key*: :ref:`partitioning-key`\ 를 지정한다.
*   *partition_name*: 분할 이름을 지정한다.
*   *range_value*: 분할 키의 최대 값을 지정한다. *range_value* 보다 작은 분할 키 값을 가지는 레코드들은 모두 해당 분할에 저장된다.
*   *comment_string*: 각 분할의 커멘트를 지정한다.

다음은 올림픽 참가국 정보를 담은 *participant2* 테이블을 참가한 올림픽의 개최연도를 기준으로 2000년도 전의 참가국(*before_2000* 분할)과 2008년도 전의 참가국(*before_2008* 분할)로 나누는 영역 분할을 생성하는 예제이다. 

.. _range-participant2-table:

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

분할을 생성할 때, 사용자가 제공한 영역을 가장 작은 값부터 가장 큰 값까지 정렬하고 정렬된 리스트에서 겹치지 않는 간격을 생성한다. 위 예에서 생성된 영역의 간격은 [-inf, 2000)와 [2000, 2008)이다. 분할에 대한 무제한의 최대값을 지정하고 싶으면 **MAXVALUE** 식별자를 사용한다.

.. code-block:: sql

    ALTER TABLE participant2 ADD PARTITION (
      PARTITION before_2012 VALUES LESS THAN (2012),
      PARTITION last_one VALUES LESS THAN MAXVALUE
    );

투플을 영역 분할 테이블에 삽입할 때, 시스템은 분할 키를 평가하여 해당 투플이 어느 분할 영역에 속하게 될 것인가를 식별한다. 분할 키 값이 **NULL**\ 이면, 해당 투플은 가장 작은 영역의 분할에 저장된다. 분할 키 값에 해당하는 영역이 없으면 오류를 반환한다. 또한 투플을 업데이트할 때도 새로운 분할 키 값에 해당하는 영역이 존재하지 않으면 오류를 반환한다. 

다음은 각 분할에 커멘트를 추가하는 예제이다.

.. code-block:: sql

    CREATE TABLE tbl (a int, b int) PARTITION BY RANGE(a) (
        PARTITION less_1000 VALUES LESS THAN (1000) COMMENT 'less 1000 comment', 
        PARTITION less_2000 VALUES LESS THAN (2000) COMMENT 'less 2000 comment'
    );

    ALTER TABLE tbl PARTITION BY RANGE(a) (
        PARTITION less_1000 VALUES LESS THAN (1000) COMMENT 'new partition comment');

분할 커멘트를 확인하는 방법은 :ref:`show-partition-comment`\를 참고한다.

.. _hash-partitioning:

해시 분할
=========

해시 분할은 지정된 개수의 분할로 데이터를 분배하기 위해 사용되는 분할 기법이다. 이 분할 기법은 테이블 데이터의 영역이나 리스트가 의미 없는 값을 포함할 때 유용하다. 예를 들어, 키워드 테이블이나 user_id가 가장 관심 있는 값인 사용자 테이블과 같은 경우에 해당된다. 분할 키 값이 테이블 데이터를 고르게 분배한다면, 해시 분할 기법은 정의된 분할들에 테이블 데이터를 고르게 배분해준다. 해시 분할에 대한 :ref:`partition-pruning` 최적화는 동등 조건(**=**\과 :ref:`IN <in-expr>` 조건)에만 적용될 수 있는데, 대부분의 질의가 분할 키에 대한 동등 조건으로 주어질 때에 해시 분할이 유용하다.

**CREATE** 또는 **ALTER** 문에서 **PARTITION BY HASH** 절을 사용하여 해시 분할을 할 수 있다. ::

    CREATE TABLE table_name (
       ...
    )
    PARTITION BY HASH ( <partitioning_key> )
    PARTITIONS ( number_of_partitions )

    ALTER TABLE table_name 
    PARTITION BY HASH (<partitioning_key>)
    PARTITIONS (number_of_partitions)

*   *partitioning_key*: :ref:`partitioning-key`\ 를 지정한다.
*   *number_of_partitions*: 생성할 분할의 개수를 지정한다.

다음은 국가 코드와 국가 이름의 정보를 담은 *nation2* 테이블을 생성하고 *code* 값을 기준으로 4개의 해시 분할을 정의하는 예제이다. 해시 분할은 분할의 개수만 지정하고 이름은 지정하지 않는다.

.. _hash-nation2-table:

.. code-block:: sql

    CREATE TABLE nation2 (
      code CHAR (3),
      name VARCHAR (50)
    )
    PARTITION BY HASH (code) PARTITIONS 4;

해시 분할 테이블에 삽입될 때 데이터를 저장할 분할은 분할 키의 해시 값에 의해 결정된다. 분할 키 값이 **NULL**\이면, 해당 레코드는 첫번째 분할에 저장된다.

.. _list-partitioning:

리스트 분할
===========

리스트 분할은 사용자가 지정한 분할 키 값의 리스트에 따라 테이블을 분할하는 기법이다. 분할을 위한 값의 리스트는 겹치는 값이 없어야 한다. 이 분할 기법은 사원 테이블의 부서 ID, 사용자 테이블의 국가 코드와 같은 경우처럼 테이블 데이터가 의미 있는 값의 리스트로 나누어질 때 유용하다. 해시 분할과 마찬가지로, 리스트 분할에 대한 :ref:`partition-pruning` 최적화는 동등 조건(**=**\과 :ref:`IN <in-expr>` 조건)에만 적용된다. 

**CREATE** 또는 **ALTER** 문에서 **PARTITION BY LIST** 절을 사용하여 리스트 분할을 할 수 있다. ::

    CREATE TABLE table_name (
      ...
    )
    PARTITION BY LIST ( <partitioning_key> ) (
      PARTITION partition_name VALUES IN ( <values_list> ) [COMMENT 'comment_string'],
      PARTITION partition_name VALUES IN ( <values_list> ) [COMMENT 'comment_string'],
      ... 
    )
    
    ALTER TABLE table_name
    PARTITION BY LIST ( <partitioning_key> ) (
      PARTITION partition_name VALUES IN ( <values_list> ) [COMMENT 'comment_string'],
      PARTITION partition_name VALUES IN ( <values_list> ) [COMMENT 'comment_string'],
      ... 
    )

*   *partitioning_key* : :ref:`partitioning-key`\를 지정한다.
*   *partition_name* : 분할 명을 지정한다.
*   *partition_value_list* : 분할의 기준이 되는 값의 목록을 지정한다.
*   *comment_string*: 각 분할의 커멘트를 지정한다.

다음은 선수의 이름과 종목 정보를 담고 있는 *athlete2* 테이블을 생성하고 종목에 따른 리스트 분할을 정의하는 예제이다.

.. _list-athlete2-table:

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR (30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics'),
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing'),
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball')
    );

리스트 분할 테이블에 투플을 삽입할 때 분할 키 값은 분할에 정의된 리스트 값 중 하나에 속해야 한다. 리스트 분할의 경우 분할 키 값이 **NULL**\일 때 자동으로 특정 분할을 할당하지 않고 오류가 발생된다. **NULL** 값을 저장하려면 다음의 예와 같이 **NULL**\을 포함하는 분할을 생성해야 한다.

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR (30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics' ),
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing'),
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball', NULL)
    );

다음은 각 분할에 커멘트를 추가하는 예제이다.

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR (30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics') COMMENT 'G1',
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing') COMMENT 'G2',
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball') COMMENT 'G3');

    CREATE TABLE athlete3 (name VARCHAR (40), event VARCHAR (30));
    ALTER TABLE athlete3 PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Handball', 'Volleyball', 'Tennis') COMMENT 'G1');


.. _show-partition-comment:

분할 커멘트
-----------

분할 커멘트는 영역 분할과 리스트 분할에 대해서만 지정할 수 있으며, 해시 분할에서는 지정할 수 없다. 분할 커멘트는 다음 구문을 실행하여 확인할 수 있다.

.. code-block:: sql

    SHOW CREATE TABLE table_name;
    SELECT class_name, partition_name, COMMENT FROM db_partition WHERE class_name ='table_name';

또는 CSQL 인터프리터에서 테이블의 스키마를 출력하는 ;sc 명령으로 인덱스의 커멘트를 확인할 수 있다.

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc tbl

.. _partition-pruning:

분할 프루닝
===========

분할 프루닝(partition pruning)은 검색 조건을 통해 데이터 검색 범위를 한정시키는 최적화 기법이다. 분할 프루닝을 수행하는 과정 중에 분할 정의를 고려하여 질의문에 대해 항상 거짓인 분할들을 식별한다. 다음 예의 **SELECT** 문에 대해 *before_2008*\과 *before_2012* 분할을 제외한 나머지 분할들은 모두 *YEAR (opening_date)*\가 2004 보다 작다는 것을 알 수 있기 때문에, *before_2008*\과 *before_2012* 분할에 대해서만 질의가 이루어진다.

.. code-block:: sql

    CREATE TABLE olympic2 (opening_date DATE, host_nation VARCHAR (40))
    PARTITION BY RANGE (YEAR(opening_date)) (
        PARTITION before_1996 VALUES LESS THAN (1996),
        PARTITION before_2000 VALUES LESS THAN (2000),
        PARTITION before_2004 VALUES LESS THAN (2004),
        PARTITION before_2008 VALUES LESS THAN (2008),
        PARTITION before_2012 VALUES LESS THAN (2012)
    );
     
    SELECT opening_date, host_nation 
    FROM olympic2 
    WHERE YEAR(opening_date) > 2004;

분할 프루닝은 디스크 I/O와 질의 수행 중 처리해야 할 데이터 양을 크게 줄여준다. 프루닝의 이점을 최대한 활용하기 위해서 프루닝이 수행되는 시점을 이해하는 것이 중요하다. 분할을 프루닝하려면 다음 조건들을 만족해야 한다.

*   분할 키는 *WHERE* 절에서 다른 표현식을 통하지 않고 직접 사용되어야 한다.
*   영역 분할에서 분할 키는 범위 조건(**<**, **>**, **BETWEEN** 등)이나 동등 조건(**=**, **IN** 등)으로 사용되어야 한다.
*   리스트 분할과 해시 분할에서 분할 키는 동등 조건(**=**, **IN** 등)으로 사용되어야 한다.

다음 예는 위의 *olympic2* 테이블을 가지고 프루닝이 어떻게 수행되는가를 설명한다.  

.. code-block:: sql

    -- prune all partitions except before_2012
    SELECT host_nation 
    FROM olympic2 
    WHERE YEAR (opening_date) >= 2008;

    -- prune all partitions except before_2008
    SELECT host_nation 
    FROM olympic2 
    WHERE YEAR(opening_date) BETWEEN 2005 and 2007;

    -- no partition is pruned because partitioning key is not used
    SELECT host_nation 
    FROM olympic2 
    WHERE opening_date = '2008-01-02';

    -- no partition is pruned because partitioning key is not used directly
    SELECT host_nation 
    FROM olympic2 
    WHERE YEAR(opening_date) + 1 = 2008;

    -- no partition is pruned because there is no useful predicate in the WHERE clause
    SELECT host_nation 
    FROM olympic2 
    WHERE YEAR(opening_date) != 2008;

.. note:: CUBRID 9.0 미만 버전에서 분할 프루닝은 질의 컴파일 단계에서 수행되었다. CUBRID 9.0부터 분할 프루닝은 질의 실행 단계에서 수행되는데, 질의를 실행하는 동안 분할 프루닝을 실행하면 훨씬 복잡한 질의에 대해서도 이 최적화를 적용할 수 있게 되기 때문이다. 그러나 질의 실행 계획은 질의 실행 전에 수행되어 프루닝 정보는 질의 실행 전에는 알 수 없으므로, 프루닝 정보는 더 이상 질의 실행 계획 단계에서 출력되지 않는다.

사용자는 분할 테이블을 접근하는 방법 외에 시스템에 의해 부여된 분할 이름을 직접 명시하거나 *table PARTITION (name)* 절을 사용하여 각 분할에 직접 접근할 수 있다.

.. code-block:: sql

    -- to specify a partition with its table name
    SELECT * FROM olympic2__p__before_2008;
    
    -- to specify a partition with PARTITION clause
    SELECT * FROM olympic2 PARTITION (before_2008);

위의 *before_2008* 분할에 접근하는 두 개의 질의는 분할(partition)이 아닌 일반 테이블인 것처럼 보인다. 분할 테이블(partitioned table)에서는 사용할 수 없는 최적화 기법(이에 대한 자세한 내용은 :ref:`partitioning-notes` 참고)을 이 방법을 통해서 사용할 수 있기 때문에 매우 유용하게 활용될 수 있다. 사용자가 분할을 직접 명시하면 해당 질의는 지정한 분할에만 제한된다는 것을 유의해야 한다. 질의의 **WHERE** 절 조건을 만족하는 레코드를 포함하더라도 명시되지 않은 분할들은 질의 수행 시에 전혀 고려되지 않으며, **INSERT**\와 **UPDATE** 문에 의해 삽입/수정되는 레코드가 지정된 분할에 속하지 않는 경우 오류가 발생된다.

분할 테이블(partitioned table)이 아닌 각 분할(partition)에 대해 질의를 수행하면, 분할 기법의 몇 가지 이점을 잃게 된다. 예를 들어, 사용자가 단지 분할 테이블에 대해서만 질의를 수행하면 사용자의 응용 프로그램을 수정할 필요 없이 추후에 해당 테이블을 재분할하거나 특정 분할을 제거(drop)할 수 있다. 사용자가 분할에 직접 접근하면 이러한 이점을 잃게 된다. 또한, **INSERT** 문에서 특정 분할을 명시하는 것이 허용되기는 하지만 이로 인해 얻을 수 있는 성능 이득이 없으므로 권장되지 않는다.

분할 관리
=========

**ALTER** 문의 분할 지정 절을 사용하여 다음과 같이 분할 테이블을 관리할 수 있다. 

1. :ref:`분할 테이블을 일반 테이블로 변경 <remove-partitioning>`
#. :ref:`분할 재구성 <reorganize-partitions>`
#. :ref:`이미 존재하는 분할 테이블에 분할 추가 <add-partitions>`
#. :ref:`분할 제거하기 <drop-partitions>`
#. :ref:`분할을 일반 테이블로 승격 <promote-partitions>`

.. _remove-partitioning:

분할 테이블을 일반 테이블로 변경
--------------------------------

분할 테이블을 일반 테이블로 변경하려면 **ALTER TABLE** 문을 이용한다. ::

    ALTER {TABLE | CLASS} table_name REMOVE PARTITIONING

*   *table_name*: 변경하고자 하는 테이블의 이름을 지정한다.

분할 설정을 제거하면 각 분할에 있던 모든 데이터가 분할 테이블로 이동된다. 이는 비용이 많이 드는 작업으로 주의해서 계획해야 한다.

.. _reorganize-partitions:

분할 재구성
-----------

분할 재구성은 하나의 분할을 더 작은 분할들로 나누거나 한 그룹의 분할들을 하나의 분할로 병합하는 작업이다. 이를 수행하려면 **ALTER** 문의 **REORGANIZE PARTITION** 절을 사용한다. ::

    ALTER {TABLE | CLASS} table_name
    REORGANIZE PARTITION <alter_partition_name_comma_list>
    INTO ( <partition_definition_comma_list> )
     
    partition_definition_comma_list ::=
    PARTITION partition_name VALUES LESS THAN ( <range_value> ), ... 

*   *table_name*: 재정의할 테이블의 이름을 지정한다.
*   *alter_partition_name_comma_list*: 재정의할 현재 분할들을 지정한다. 여러 개의 분할은 쉼표(,)로 구분된다.
*   *partition_definition_comma_list*: 새 분할들을 지정한다. 여러 개의 분할은 쉼표(,)로 구분된다.

이 절은 영역 분할 및 리스트 분할에만 적용된다. 해시 분할 기법에서 데이터 분배는 영역 분할과 리스트 분할과는 의미적으로 다르므로, 해시 분할 테이블은 분할 추가 및 삭제만 허용한다. 자세한 사항은 :ref:`hash-reorganization` 절을 참고한다.

다음 예는 :ref:`participant2<range-participant2-table>` 테이블의 *before_2000* 분할을 *before_1996* 분할과 *before_2000* 분할로 재구성하는 방법이다.

.. code-block:: sql
     
    ALTER TABLE participant2 
    REORGANIZE PARTITION before_2000 INTO (
      PARTITION before_1996 VALUES LESS THAN (1996),
      PARTITION before_2000 VALUES LESS THAN (2000)
    );

다음 예는 위의 예에서 정의된 두 개의 분할을 다시 하나의 *before_2000*\로 병합하는 방법이다.

.. code-block:: sql

    ALTER TABLE participant2 
    REORGANIZE PARTITION before_1996, before_2000 INTO (
      PARTITION before_2000 VALUES LESS THAN (2000)
    );

다음 예는 :ref:`athlete2<list-athlete2-table>` 테이블에서 정의된  *event2* 분할을 *event2_1* (Judo)와 *event2_2* (Taekwondo, Boxing)으로 재구성하는 방법이다.

.. code-block:: sql

    ALTER TABLE athlete2 
    REORGANIZE PARTITION event2 INTO (
        PARTITION event2_1 VALUES IN ('Judo'),
        PARTITION event2_2 VALUES IN ('Taekwondo', 'Boxing')
    );

다음 예는 *event2_1*\과 *event2_2* 분할을 다시 *event2* 분할로 합치는 방법이다.

.. code-block:: sql

    ALTER TABLE athlete2 
    REORGANIZE PARTITION event2_1, event2_2 INTO (
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing')
    );

.. note::

    *   영역 분할 테이블에서 인접한 분할끼리만 재구성될 수 있다.
    *   분할 재구성을 수행하는 동안, 새로 분할된 스키마에 맞춰 분할 간에 데이터를 이동한다. 재구성되는 분할의 크기에 따라 시간이 많이 소요될 수 있으므로 주의 깊게 해당 작업을 계획할 필요가 있다.
    *   **REORGANIZE PARTITION** 절은 분할 방법을 바꾸기 위해 사용할 수 없다. 예를 들어, 영역 분할 테이블을 해시 분할 테이블로 바꿀 수 없다.
    *   분할을 재구성한 후에 최소한 하나의 분할이 존재해야 한다.

.. _add-partitions:

분할 추가
---------

*ALTER* 문의 *ADD PARTITION* 절을 사용하여 분할 테이블에 분할을 추가할 수 있다. ::

    ALTER {TABLE | CLASS} table_name
    ADD PARTITION (<partition_definitions_comma_list>)

*   *table_name*: 분할이 추가될 테이블 이름을 지정한다. 
*   *partition_definitions_comma_list*: 추가될 분할 이름을 지정한다. 여러 개인 경우 쉼표(,)로 구분한다.

다음 예는 :ref:`participant2<range-participant2-table>` 테이블에 *before_2012* 분할과 *last_one* 분할을 추가하는 방법이다.

.. code-block:: sql

    ALTER TABLE participant2 ADD PARTITION (
      PARTITION before_2012 VALUES LESS THAN (2012),
      PARTITION last_one VALUES LESS THAN MAXVALUE
    );

.. note::

    *   영역 분할 테이블에서 추가할 분할에 대한 영역 값은 기존 분할의 최대 영역 값보다 커야 한다.
    *   영역 분할 테이블에서 **MAXVALUE** 로 최대값이 설정되어 있으면 **ADD PARTITION** 절은 항상 오류를 반환한다. 이 경우에 대신 :ref:`REORGANIZE PARTITION<reorganize-partitions>` 절을 사용해야 한다.
    *   **ADD PARTITION** 절은 이미 존재하는 분할 테이블에 대해서만 사용할 수 있다.
    *   **ADD PARTITION** 절이 해시 분할 테이블에 적용될 때는 다른 의미를 가진다. 이에 대한 자세한 사항은 :ref:`hash-reorganization` 절을 참고한다.

.. _drop-partitions:

분할 제거
---------

**ALTER** 문의 **DROP PARTITION** 절을 이용하여 분할 테이블에서 분할을 제거(drop)할 수 있다. ::

    ALTER {TABLE | CLASS} table_name
    DROP PARTITION partition_name_list

*   *table_name*: 분할 테이블 이름을 지정한다.
*   *partition_name_list*: 제거할 분할 이름을 지정한다. 여러 개인 경우 쉼표(,)로 구분한다.

다음은 :ref:`participant2 <range-participant2-table>` 테이블에서 *before_2000* 분할을 제거하는 방법이다.

.. code-block:: sql

    ALTER TABLE participant2 DROP PARTITION before_2000;

.. note::

    *   분할을 제거하면 해당 분할 내에 저장된 데이터도 모두 삭제된다. 데이터를 유지한 채로 테이블의 분할을 변경하고 싶다면 **ALTER TABLE** ... **REORGANIZE PARTITION** 문을 사용하면 된다.
   
    *   분할을 제거할 경우 삭제된 행의 개수를 반환하지 않는다. 테이블과 분할을 유지한 채로 데이터만 삭제하고 싶은 경우 **DELETE** 문을 사용하면 된다.

해시 분할 테이블에 대해 이 구문을 사용할 수 없다. 해시 분할 테이블의 분할을 제거하려면 해시 분할에서만 사용하는 :ref:`hash-reorganization` 절을 참고한다.
   
.. _hash-reorganization:

해시 분할 재구성
----------------

해시 분할 테이블에서 분할 간의 데이터 분배는 CUBRID에 의해 내부적으로 관리되므로, 해시 분할 재구성은 리스트 분할이나 영역 분할에서의 재구성과 다르게 동작한다. 해시 분할 테이블에 정의된 분할 개수를 증가시키거나 감소시키는 것만 허용된다. 해시 분할 테이블의 분할 개수를 수정하더라도 데이터 손실은 발생되지 않는다. 그러나 해시 함수의 영역이 수정되기 때문에, 해시 분할의 일관성을 유지하기 위해 새로운 분할들 간에 데이터가 재분배되어야 한다.

해시 분할 테이블에 정의된 분할 개수는 **ALTER** 문의 **COALESCE PARTITION** 절을 이용하여 줄일 수 있다. ::

    ALTER {TABLE | CLASS} table_name
    COALESCE PARTITION number_of_shrinking_partitions

*   *table_name* : 재정의할 테이블의 이름을 지정한다.
*   *number_of_shrinking_partitions* : 삭제하려는 분할 개수를 지정한다.

다음은 :ref:`nation2 <hash-nation2-table>` 테이블의 분할 개수를 4 개에서 3 개로 줄이는 예제이다.

.. code-block:: sql

    ALTER TABLE nation2 COALESCE PARTITION 1;

**ALTER** 문의 **ADD PARTITION** 절을 사용하여 **ALTER** 해시 분할 테이블에 정의된 분할 개수를 늘릴 수 있다. ::

    ALTER {TABLE | CLASS} table_name
    ADD PARTITION PARTITIONS number

*   *table_name* : 분할 개수를 재정의할 테이블의 이름을 지정한다.
*   *number* : 추가할 분할 개수를 지정한다.

다음은 :ref:`nation2 <hash-nation2-table>` 테이블에 3 개의 분할을 추가하는 예이다.

.. code-block:: sql

    ALTER TABLE nation2 ADD PARTITION PARTITIONS 3;

.. _promote-partitions:

분할 승격
---------

분할(partition) **PROMOTE** 문은 분할 테이블에서 사용자가 지정한 분할을 일반 테이블로 승격(promote)한다. 이것은 거의 사용하지 않는 오래된 데이터를 보관할(archiving) 목적으로 유지하고자 할 때 유용하다. 해당 분할을 일반 테이블로 승격함으로써 분할 테이블에 대한 접근 부하를 줄일 수 있고, 분할 테이블에서 제거된 데이터는 승격된 테이블에 유지되므로 여전히 해당 데이터를 접근할 수 있다. 분할을 승격(promote)하는 것은 비가역적인 작업으로 승격된 분할을 분할 테이블로 다시 되돌릴 수 없다.

분할 **PROMOTE** 문은 영역 분할 테이블과 리스트 분할 테이블에만 허용된다. 해시 분할 테이블은 사용자가 해시 분할 간에 데이터 분배를 제어할 수 없으므로 승격을 허용하지 않는다.

분할이 일반 테이블로 승격될 때 승격 테이블은 데이터와 일반 인덱스만 상속받는다. 다음의 테이블 속성들은 승격된 테이블에 저장되지 않는다.

*   기본 키
*   외래 키
*   고유 인덱스
*   **AUTO_INCREMENT** 속성 및 시리얼
*   트리거
*   메서드
*   상속 관계(수퍼클래스와 서브클래스)

분할을 승격하는 구문은 다음과 같다. ::

    ALTER TABLE table_name PROMOTE PARTITION <partition_name_list>

*   <*partition_name_list*> :  승격할 분할 이름으로, 여러 개를 쉼표(,)로 구분한다.

다음은 분할 테이블을 생성하고, 일부 투플을 삽입한 후 이들 중 2 개의 분할을 승격하는 예이다.  

.. code-block:: sql
    
    CREATE TABLE t (i INT) PARTITION BY LIST (i) (
        PARTITION p0 VALUES IN (1, 2),
        PARTITION p1 VALUES IN (3, 4),
        PARTITION p2 VALUES IN (5, 6)
    );
    
    INSERT INTO t VALUES(1), (2), (3), (4), (5), (6);
    
테이블 *t* 의 스키마와 데이터는 다음과 같다.

.. code-block:: sql

    csql> ;schema t
    === <Help: Schema of a Class> ===
    ...
     <Partitions>
         PARTITION BY LIST ([i])
         PARTITION p0 VALUES IN (1, 2)
         PARTITION p1 VALUES IN (3, 4)
         PARTITION p2 VALUES IN (5, 6)

    csql> SELECT * FROM t;

    === <Result of SELECT Command in Line 1> ===
                i
    =============
                1
                2
                3
                4
                5
                6

다음 구문은 *p0* 분할과 *p2* 분할을 승격한다.

.. code-block:: sql

    ALTER TABLE t PROMOTE PARTITION p0, p2;

승격(promotion) 이후, 테이블 *t*\는 *p1*\이라는 하나의 분할만 포함하며 다음 데이터를 유지한다.

.. code-block:: sql

    csql> ;schema t
    === <Help: Schema of a Class> ===
     <Class Name>
         t
     ...
     <Partitions>
         PARTITION BY LIST ([i])
         PARTITION p1 VALUES IN (3, 4)

    csql> SELECT * FROM t;

    === <Result of SELECT Command in Line 1> ===
                i
    =============
                3
                4         

.. _index-partitions:

분할 테이블의 인덱스
====================

분할 테이블에서 생성되는 모든 인덱스는 로컬 인덱스이다. 로컬 인덱스의 경우 각 분할에 대한 데이터가 별도의(로컬) 인덱스로 저장된다. 다른 분할의 데이터에 액세스하는 트랜잭션이 다른 로컬 인덱스에도 액세스하므로 분할 테이블 인덱스의 동시성을 향상시킨다.

고유 인덱스를 생성할 때 다음 제약 사항을 충족해야 한다.

*  고유 인덱스 키 또는  기본 키는 분할 키를 포함해야 한다.

이를 충족하지 않으면 CUBRID에서 오류가 반환된다.

.. code-block:: sql

        csql> CREATE TABLE t(i INT , j INT) PARTITION BY HASH (i) PARTITIONS 4;
        Execute OK. (0.142929 sec) Committed.

        1 command(s) successfully processed.
        csql> ALTER TABLE t ADD PRIMARY KEY (i);
        Execute OK. (0.123776 sec) Committed.

        1 command(s) successfully processed.
        csql> CREATE UNIQUE INDEX idx2 ON t(j);

        In the command from line 1,

        ERROR: Partition key attributes must be present in the index key.


        0 command(s) successfully processed.

로컬 인덱스의 이점을 이해하는 것이 중요하다. 글로벌 인덱스 스캔의 경우 프루닝(pruning)되지 않은 분할에 대해 각각 별도의 인덱스 스캔이 수행된다. 디스크에서 다른 분할에 있는 데이터(지금 스캔 중인 분할이 아닌 다른 분할에 속한 데이터)를 가져온 다음 버리기 때문에 로컬 인덱스 스캔보다 성능이 저하된다. **INSERT** 질의문도 글로벌 인덱스보다 크기가 더 작은 로컬 인덱스에서 향상된 성능을 보인다.

.. _partitioning-notes:

분할에 관한 노트
================

분할된 테이블은 일반적인 테이블 처럼 정상적으로 동작한다. 하지만 분할된 테이블의 장점을 충분히 살리기 위해서 적용을 고려해야하는 노트가 있다.

분할 테이블에 관한 통계
-----------------------

CUBRID 9.0에서 부터, **ALTER** 문의 **ANALYZE PARTITION** 절은 더 이상 사용되지 않는다. 질의를 수행하는 동안 분할을 잘라내는 것이 발생하고, 이러한 경우 이 문은 유용한 결과를 생산하지 못한다. 9.0에서 부터, CUBRID는 각 분할에 대한 통계를 분리 유지한다. 분할된 테이블의 통계는 각 분할에 대한 통계의 평균 값으로 계산된다. 이것은 일상적인 경우의 최적화, 하나를 제외하고 모든 분할이 제거된 분할에 대한 질의, 등을 위해서 진행되었다.

분할된 테이블에 대한 제약들
---------------------------

다음의 제약이 분할된 테이블에 적용된다:

*   하나의 테이블에 대해서 최대 1,024 까지의 분할이 정의될 수있다.

*   분할은 상속 체인의 부분이 될 수 없다. 클래스는 하나의 분할을 상속할 수 없고, 분할은 분할된 클래스(기본으로 상속한다)를 제외한 다른 클래스를 상속할 수 없다. 

*   다음의 질의 최적화는 분할된 테이블에 대해서 수행되지 않는다:

    *   ORDER BY skip (for details, see :ref:`order-by-skip-optimization`)
    *   GROUP BY skip (for details, see :ref:`group-by-skip-optimization`)
    *   Multi-key range optimization (for details, see :ref:`multi-key-range-opt`)
    *   INDEX JOIN

분할 키와 문자셋, 콜레이션
--------------------------

분할하는 키들과 분할의 정의는 같은 문자셋이어야 한다. 아래의 질의는 오류를 반환한다:

.. code-block:: sql

    CREATE TABLE t (c CHAR(50) COLLATE utf8_bin)
    PARTITION BY LIST (c) (
        PARTITION p0 VALUES IN (_utf8'x'),
        PARTITION p1 VALUES IN (_iso88591'y')
    );

::

    ERROR: Invalid codeset '_iso88591' for partition value. Expecting '_utf8' codeset.

분할 키에서 비교 작업을 수행할 때 분할 테이블에 정의된 콜레이션을 사용한다. 다음 예제에서 utf8_en_ci 콜레이션의 'test'는 'TEST'와 같으므로 오류를 반환한다.

.. code-block:: sql

    CREATE TABLE tbl (str STRING) COLLATE utf8_en_ci
    PARTITION BY LIST (str) (
        PARTITION p0 VALUES IN ('test'),
        PARTITION p1 VALUES IN ('TEST')
    );

::

    ERROR: Partition definition is duplicated. 'p1'

.. CUBRIDSUS-10161 : below constraints of 9.1 was removed from 9.2. (below will be commented)

    For hash-partitioned tables, the collation of the partitioning key must be binary.
        *   e.g. of binary collation: utf8_bin, iso88591_bin, euckr_bin
        *   e.g. of non-binary collation: utf8_de_exp_ai_ci
