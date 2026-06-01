
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

    CREATE TABLE [schema_name.]table_name (
       ...
    )
    PARTITION BY RANGE ( <partitioning_key> ) (
        PARTITION partition_name VALUES LESS THAN ( <range_value> ) [COMMENT 'comment_string'] ,
        PARTITION partition_name VALUES LESS THAN ( <range_value> ) [COMMENT 'comment_string'] ,
        ... 
    )
    
    ALTER TABLE [schema_name.]table_name 
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

    CREATE TABLE [schema_name.]table_name (
       ...
    )
    PARTITION BY HASH ( <partitioning_key> )
    PARTITIONS ( number_of_partitions )

    ALTER TABLE [schema_name.]table_name 
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

    CREATE TABLE [schema_name.]table_name (
      ...
    )
    PARTITION BY LIST ( <partitioning_key> ) (
      PARTITION partition_name VALUES IN ( <values_list> ) [COMMENT 'comment_string'],
      PARTITION partition_name VALUES IN ( <values_list> ) [COMMENT 'comment_string'],
      ... 
    )
    
    ALTER TABLE [schema_name.]table_name
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

    SHOW CREATE TABLE [schema_name.]table_name;
    SELECT class_name, partition_name, COMMENT FROM db_partition WHERE class_name ='table_name';

또는 CSQL 인터프리터에서 테이블의 스키마를 출력하는 ;sc 명령으로 인덱스의 커멘트를 확인할 수 있다.

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc tbl

.. _partition-pruning:

분할 프루닝
========================================

분할 프루닝(Partition Pruning)은 분할 테이블 조회 시 분할 키 조건을 평가하여, 접근해야 할 분할(Partition) 범위를 최소화하는 최적화 기법이다.
결과에 포함될 가능성이 없는 분할을 미리 제외함으로써 디스크 I/O와 스캔 비용을 줄이고 질의 성능을 향상시킬 수 있다.

분할 프루닝의 적용 여부는 질의 컴파일 단계에서 결정하지 않으며, 질의 실행을 준비하는 단계에서 분할 키 조건으로 사용되는 값을 분석하여 결정한다.
따라서 동일한 형태의 질의라도 분할 키 조건으로 사용되는 값이 달라지면 분할 프루닝 적용 여부도 달라질 수 있다.

분할 프루닝 적용 여부는 질의 실행 계획에 표시되지 않지만, 질의 실행 후 프로파일링 결과를 통해 이를 확인할 수 있다.
질의 프로파일링에 대한 자세한 내용은 :ref:`질의 프로파일링 <query-profiling>`\을 참고한다.

.. note::

  CUBRID 9.0 미만 버전에서는 분할 프루닝 여부를 질의 컴파일 단계에서 결정되지만,
  9.0 이상 버전부터는 질의 실행 준비 단계에서 결정된다.

.. rubric:: 분할 방식별 분할 프루닝 지원 비교 연산자

.. list-table::
  :header-rows: 1
  :widths: 20 20 60

  * - 분할 방법
    - 동등 조건
    - 범위 조건
  * - 영역 분할
    - ``=``, ``IN``
    - ``<``, ``<=``, ``>``, ``>=``, ``BETWEEN``
  * - 리스트 분할
    - ``=``, ``IN``
    - 미지원
  * - 해시 분할
    - ``=``, ``IN``
    - 미지원

.. rubric:: 분할 프루닝이 적용되지 않는 경우

- **WHERE** 절의 분할 키 조건이 분할 키로 정의한 표현식과 다르거나, 표현식이 동일하더라도 인자 순서가 다른 경우
- 분할 방식에서 분할 프루닝을 지원하지 않는 비교 연산자를 사용하는 경우
- 질의 실행 준비 단계에서 분할 키 조건의 값을 확인할 수 없는 경우

.. rubric:: 분할 직접 접근

.. code-block:: sql

  ... FROM [<schema_name>.]<partition_table_name>__p__<partition_name> [AS <alias_name>] ...

  ... FROM [<schema_name>.]<partition_table_name> PARTITION (<partition_name>) [AS <alias_name>] ...

**WHERE** 절에 분할 키 조건이 없거나 분할 프루닝이 적용되지 않더라도, **PARTITION** 절을 사용하거나 분할 이름을 직접 지정하여 특정 분할만 조회할 수 있다.
이 방식은 일반 테이블을 조회하는 것과 동일하게 동작하므로 분할 테이블에서는 제한되었던 일부 최적화 기법도 적용 가능하다.
다만 질의 범위가 해당 분할로 고정되기 때문에, 조건에 맞는 레코드가 다른 분할에 있더라도 검색 대상에서 제외되므로 잘못된 결과가 반환될 수 있다.

**INSERT**\와 **UPDATE** 문에서도 특정 분할을 지정할 수 있지만, 처리하려는 레코드가 해당 분할에 속하지 않으면 오류가 발생할 수 있으니 주의해야 한다.
특히 **INSERT** 문은 분할을 지정하더라도 별도의 성능 이점이 없으므로 분할을 직접 지정하는 방식을 권장하지 않는다.

또한, 분할에 직접 접근하면 분할 테이블이 제공하는 운영상의 장점을 누리기 어렵다.
테이블 단위로 질의를 작성하면 향후 분할 구성이 변경되어도 응용 프로그램을 수정할 필요가 없지만,
분할을 직접 명시하면 이러한 유연성을 확보할 수 없기 때문이다.
따라서 특별한 목적이 있는 경우가 아니라면 분할 테이블을 통해 데이터에 접근하는 것을 권장한다.

분할 테이블에 적용할 수 없는 최적화 기법에 대한 자세한 내용은 :ref:`분할된 테이블에 대한 제약들 <partitioning-notes>`\을 참고한다.

.. _example_partition-pruning_query-profiling:

.. rubric:: 예제 1. 질의 프로파일링 결과를 통해 분할 프루닝 확인

이번 예제에서는 질의 프로파일링 결과를 통해 분할 프루닝의 적용 여부를 확인한다.

아래 질의는 **WHERE** 절에 분할 키 조건이 존재하며, 해당 조건은 **CREATE TABLE** 문에서 정의한 분할 키 표현식과 동일하다.
또한 범위 분할은 범위 조건에 대한 분할 프루닝을 지원하므로 분할 프루닝이 적용될 수 있다.
다만 분할 프루닝은 질의 실행 준비 단계에서 결정하므로 실행 계획만으로는 적용 여부를 확인할 수 없다.

.. code-block:: sql

  drop table if exists olympic_range;

  create table olympic_range
  partition by range (host_year) (
      partition before_1920 values less than (1920),
      partition before_1940 values less than (1940),
      partition before_1960 values less than (1960),
      partition before_1980 values less than (1980),
      partition before_2000 values less than (2000),
      partition latest values less than maxvalue
    )
  as (select * from olympic);

  create index i_olympic_range_host_nation on olympic_range (host_nation);

  update statistics on olympic_range;

.. code-block:: sql

  set optimization level 513;
  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_range as o
  where
      o.host_year > 1990
      and o.host_nation = 'USA';

.. code-block:: text

  Join graph segments (f indicates final):
  seg[0]: [0]
  seg[1]: host_year[0] (f)
  seg[2]: host_nation[0] (f)
  seg[3]: host_city[0] (f)
  seg[4]: mascot[0] (f)
  Join graph nodes:
  node[0]: public.olympic_range o(25/6) (sargs 0 1) (loc 0)
  Join graph terms:
  term[0]: o.host_nation='USA' (sel 0.0555556) (sarg term) (not-join eligible) (indexable host_nation[0]) (loc 0)
  term[1]: o.host_year range (1990 gt_inf max) (sel 0.1) (rank 2) (sarg term) (not-join eligible) (loc 0)

  Query plan:

  iscan
      class: o node[0]
      index: i_olympic_range_host_nation term[0]
      sargs: term[1]
      cost:  3 card 1

  Query stmt:

  select o.host_year, o.host_nation, o.host_city, o.mascot from olympic_range o where (o.host_year> ?:0 ) and o.host_nation= ?:1

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           1996  'USA'                 'Atlanta'             'Izzy'

질의 실행 후 프로파일링 결과를 확인하면,
분할 테이블의 각 분할에 대한 스캔이 **SCAN** 하위에 **PARTITION**\으로 출력되는 것을 확인할 수 있다.
**SCAN**\에는 분할 테이블 전체에 대한 스캔 정보가 표시되고, **PARTITION**\에는 각 분할에 대한 스캔 정보가 표시된다.
**PARTITION**\으로 출력되지 않은 분할은 분할 프루닝이 적용되어 스캔 대상에서 제외된 것이다.

``o.host_year > 1990`` 조건을 만족하는 ``before_2000``\과 ``latest`` 분할만 스캔되었으며,
그 외 나머지 분할은 스캔 대상에서 제외되었다.

.. code-block:: sql

  show trace;

.. code-block:: text

  Query Plan:
    INDEX SCAN (o.i_olympic_range_host_nation) (key range: o.host_nation= ?:1 )

    rewritten query: select o.host_year, o.host_nation, o.host_city, o.mascot from [public.olympic_range] o where (o.host_year> ?:0 ) and o.host_nation= ?:1

  Trace Statistics:
    SELECT (time: 0, fetch: 10, fetch_time: 0, ioread: 0)
      SCAN (index: public.olympic_range.i_olympic_range_host_nation), (btree time: 0, fetch: 6, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 2) (lookup time: 0, rows: 1)
             PARTITION (index: public.olympic_range__p__before_2000.i_olympic_range_host_nation), (btree time: 0, fetch: 4, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 2) (lookup time: 0, rows: 1)
             PARTITION (index: public.olympic_range__p__latest.i_olympic_range_host_nation), (btree time: 0, fetch: 2, ioread: 0, readkeys: 0, filteredkeys: 0, rows: 0) (lookup time: 0, rows: 0)

아래 질의는 **WHERE** 절에 분할 키 조건이 없으므로 분할 프루닝이 적용되지 않는다.

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_range as o
  where
      o.opening_date > '1990-12-31'
      and o.host_nation = 'USA';

  show trace;

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           1996  'USA'                 'Atlanta'             'Izzy'

질의 실행 후 프로파일링 결과를 확인하면,
분할 프루닝이 적용되지 않아 모든 분할에 대한 스캔 정보가 **SCAN** 하위에 **PARTITION**\으로 출력된 것을 확인할 수 있다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 24, fetch_time: 1, ioread: 0)
      SCAN (index: public.olympic_range.i_olympic_range_host_nation), (btree time: 1, fetch: 16, ioread: 0, readkeys: 3, filteredkeys: 3, rows: 4) (lookup time: 1, rows: 1)
             PARTITION (index: public.olympic_range__p__before_1920.i_olympic_range_host_nation), (btree time: 0, fetch: 3, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 1) (lookup time: 0, rows: 0)
             PARTITION (index: public.olympic_range__p__before_1940.i_olympic_range_host_nation), (btree time: 0, fetch: 3, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 1) (lookup time: 0, rows: 0)
             PARTITION (index: public.olympic_range__p__before_1960.i_olympic_range_host_nation), (btree time: 0, fetch: 2, ioread: 0, readkeys: 0, filteredkeys: 0, rows: 0) (lookup time: 0, rows: 0)
             PARTITION (index: public.olympic_range__p__before_1980.i_olympic_range_host_nation), (btree time: 0, fetch: 2, ioread: 0, readkeys: 0, filteredkeys: 0, rows: 0) (lookup time: 0, rows: 0)
             PARTITION (index: public.olympic_range__p__before_2000.i_olympic_range_host_nation), (btree time: 1, fetch: 4, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 2) (lookup time: 1, rows: 1)
             PARTITION (index: public.olympic_range__p__latest.i_olympic_range_host_nation), (btree time: 0, fetch: 2, ioread: 0, readkeys: 0, filteredkeys: 0, rows: 0) (lookup time: 0, rows: 0)

.. _example_partition-pruning_arithmetic-expression:

.. rubric:: 예제 2. 산술 표현식 분할 키의 분할 프루닝

이번 예제에서는 산술 표현식으로 정의한 분할 키를 **WHERE** 절에서 동일한 표현식으로 사용하는 경우와,
인자 순서만 달라진 경우에 대해, 분할 프루닝의 적용 여부를 확인한다.

아래 질의는 **WHERE** 절에 산술 표현식 분할 키 조건이 존재하며, 해당 조건은 **CREATE TABLE** 문에서 정의한 분할 키 표현식과 동일하다.
또한 범위 분할은 범위 조건에 대한 분할 프루닝을 지원하므로 분할 프루닝이 적용될 수 있다.

.. code-block:: sql

  drop table if exists olympic_arith_range;

  create table olympic_arith_range
  partition by range (host_year + 5) (
      partition before_1925 values less than (1925),
      partition before_1945 values less than (1945),
      partition before_1965 values less than (1965),
      partition before_1985 values less than (1985),
      partition before_2005 values less than (2005),
      partition latest values less than maxvalue
    )
  as (select * from olympic);

  update statistics on olympic_arith_range;

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_arith_range as o
  where
      o.host_year + 5 between 1975 and 1995
  order by
      o.host_year;

  show trace;

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           1972  'Germany'             'Munich'              'Waldi'
           1976  'Canada'              'Montreal'            'Amik'
           1980  'USSR'                'Moscow'              'Misha'
           1984  'USA'                 'Los Angeles'         'Sam'
           1988  'Korea'               'Seoul'               'HODORI'

질의 실행 후 프로파일링 결과를 확인하면,
``o.host_year + 5 between 1975 and 1995`` 조건을 만족하는 ``before_1985``\와 ``before_2005`` 분할만 스캔되었으며,
그 외 나머지 분할은 스캔 대상에서 제외되었다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 4, fetch: 4, fetch_time: 0, ioread: 0)
      SCAN (table: public.olympic_arith_range), (heap time: 0, fetch: 2, ioread: 0, readrows: 10, rows: 5)
             PARTITION (table: public.olympic_arith_range__p__before_1985), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 2)
             PARTITION (table: public.olympic_arith_range__p__before_2005), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 3)
      ORDERBY (time: 4, sort: true, page: 0, ioread: 0)

산술 표현식의 결과는 같지만 인자 순서가 다르면 분할 프루닝이 적용되지 않는다.

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_arith_range as o
  where
      5 + o.host_year between 1975 and 1995
  order by
      o.host_year;

  show trace;

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           1972  'Germany'             'Munich'              'Waldi'
           1976  'Canada'              'Montreal'            'Amik'
           1980  'USSR'                'Moscow'              'Misha'
           1984  'USA'                 'Los Angeles'         'Sam'
           1988  'Korea'               'Seoul'               'HODORI'

질의 실행 후 프로파일링 결과를 확인하면,
분할 프루닝이 적용되지 않아 모든 분할에 대한 스캔 정보가 **SCAN** 하위에 **PARTITION**\으로 출력된 것을 확인할 수 있다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 0, fetch: 12, fetch_time: 0, ioread: 0)
      SCAN (table: public.olympic_arith_range), (heap time: 0, fetch: 6, ioread: 0, readrows: 25, rows: 5)
             PARTITION (table: public.olympic_arith_range__p__before_1925), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 0)
             PARTITION (table: public.olympic_arith_range__p__before_1945), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 0)
             PARTITION (table: public.olympic_arith_range__p__before_1965), (heap time: 0, fetch: 1, ioread: 0, readrows: 3, rows: 0)
             PARTITION (table: public.olympic_arith_range__p__before_1985), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 2)
             PARTITION (table: public.olympic_arith_range__p__before_2005), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 3)
             PARTITION (table: public.olympic_arith_range__p__latest), (heap time: 0, fetch: 1, ioread: 0, readrows: 2, rows: 0)
      ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

.. _example_partition-pruning_function-expression:

.. rubric:: 예제 3. 함수 표현식 분할 키의 분할 프루닝

이번 예제에서는 함수 표현식으로 정의한 분할 키를 **WHERE** 절에서 동일한 표현식으로 사용하는 경우와,
함수 표현식을 사용하지 않고 원본 컬럼만 사용하는 경우에 대해, 분할 프루닝의 적용 여부를 확인한다.

아래 질의는 **WHERE** 절에 함수 표현식 분할 키 조건이 존재하며, 해당 조건은 **CREATE TABLE** 문에서 정의한 분할 키 표현식과 동일하다.
또한 범위 분할은 범위 조건에 대한 분할 프루닝을 지원하므로 분할 프루닝이 적용될 수 있다.

.. code-block:: sql

  drop table if exists olympic_func_range;

  create table olympic_func_range
  partition by range (YEAR (opening_date)) (
      partition before_1920 values less than (1920),
      partition before_1940 values less than (1940),
      partition before_1960 values less than (1960),
      partition before_1980 values less than (1980),
      partition before_2000 values less than (2000),
      partition latest values less than maxvalue
    )
  as (select * from olympic);

  update statistics on olympic_func_range;

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      YEAR (o.opening_date) as opening_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_func_range as o
  where
      YEAR (o.opening_date) between 1970 and 1990
  order by
      opening_year;

  show trace;

.. code-block:: text

    opening_year  host_nation           host_city             mascot
  ================================================================================
            1972  'Germany'             'Munich'              'Waldi'
            1976  'Canada'              'Montreal'            'Amik'
            1980  'USSR'                'Moscow'              'Misha'
            1984  'USA'                 'Los Angeles'         'Sam'
            1988  'Korea'               'Seoul'               'HODORI'

질의 실행 후 프로파일링 결과를 확인하면,
``YEAR (o.opening_date) between 1970 and 1990`` 조건을 만족하는 ``before_1980``\과 ``before_2000`` 분할만 스캔되었으며,
그 외 나머지 분할은 스캔 대상에서 제외되었다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 0, fetch: 4, fetch_time: 0, ioread: 0)
      SCAN (table: public.olympic_func_range), (heap time: 0, fetch: 2, ioread: 0, readrows: 10, rows: 5)
             PARTITION (table: public.olympic_func_range__p__before_1980), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 2)
             PARTITION (table: public.olympic_func_range__p__before_2000), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 3)
      ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

분할 키로 정의한 함수 표현식이 원본 컬럼과 동일한 정렬 순서를 보장하는 경우에는,
함수 표현식 대신 원본 컬럼을 사용하더라도 분할 프루닝이 적용될 수 있다.

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      YEAR (o.opening_date) as opening_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_func_range as o
  where
      o.opening_date between '1970-01-01' and '1990-12-31'
  order by
      opening_year;

  show trace;

.. code-block:: text

    opening_year  host_nation           host_city             mascot
  ================================================================================
            1972  'Germany'             'Munich'              'Waldi'
            1976  'Canada'              'Montreal'            'Amik'
            1980  'USSR'                'Moscow'              'Misha'
            1984  'USA'                 'Los Angeles'         'Sam'
            1988  'Korea'               'Seoul'               'HODORI'

질의 실행 후 프로파일링 결과를 확인하면,
함수 표현식 분할 키 조건을 직접 사용하지 않았음에도 **YEAR** 함수가 원본 컬럼과 동일한 정렬 순서를 보장하므로 분할 프루닝이 적용되었음을 확인할 수 있다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 0, fetch: 4, fetch_time: 0, ioread: 0)
      SCAN (table: public.olympic_func_range), (heap time: 0, fetch: 2, ioread: 0, readrows: 10, rows: 5)
             PARTITION (table: public.olympic_func_range__p__before_1980), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 2)
             PARTITION (table: public.olympic_func_range__p__before_2000), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 3)
      ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

조건절에는 ``o.opening_date``\가 사용되었지만 ``YEAR(o.opening_date)``\와 정렬 순서가 동일하기 때문에,
``YEAR(o.opening_date) between YEAR('1970-01-01') and YEAR('1990-12-31')`` 조건을 만족하는 ``before_1980``\과 ``before_2000`` 분할만 스캔되었으며,
그 외 나머지 분할은 스캔 대상에서 제외되었다.

.. _example_partition-pruning_list:

.. rubric:: 예제 4. 리스트 분할의 분할 프루닝

이번 예제에서는 리스트 분할 방식으로 분할 테이블을 생성한 뒤,
**WHERE** 절에 동등 조건을 사용하는 경우와 범위 조건을 사용하는 경우에 대해, 분할 프루닝의 적용 여부를 확인한다.

아래 질의는 **WHERE** 절에 분할 키 조건이 존재하며, 해당 조건은 **CREATE TABLE** 문에서 정의한 분할 키 표현식과 동일하다.
또한 리스트 분할은 동등 조건에 대한 분할 프루닝을 지원하므로 분할 프루닝이 적용될 수 있다.

.. code-block:: sql

  drop table if exists participant_list;

  create table participant_list
  partition by list (host_year) (
      partition p1988 values IN (1988),
      partition p1992 values IN (1992),
      partition p1996 values IN (1996),
      partition p2000 values IN (2000),
      partition p2004 values IN (2004)
    )
  as (select * from participant);

  update statistics on participant_list;

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      p.host_year, p.nation_code, p.gold
  from
      participant_list as p
  where
      p.host_year in (1988, 1996)
      and p.gold > 40
  order by
      p.host_year, p.gold, p.nation_code;

  show trace;

.. code-block:: text

      host_year  nation_code                  gold
  ================================================
           1988  'URS'                          55
           1996  'USA'                          44

질의 실행 후 프로파일링 결과를 확인하면,
``p.host_year in (1988, 1996)`` 조건을 만족하는 ``p1988``\과 ``p1996`` 분할만 스캔되었으며,
그 외 나머지 분할은 스캔 대상에서 제외되었다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 4, fetch_time: 0, ioread: 0)
      SCAN (table: public.participant_list), (heap time: 1, fetch: 2, ioread: 0, readrows: 352, rows: 2)
             PARTITION (table: public.participant_list__p__p1988), (heap time: 1, fetch: 1, ioread: 0, readrows: 156, rows: 1)
             PARTITION (table: public.participant_list__p__p1996), (heap time: 0, fetch: 1, ioread: 0, readrows: 196, rows: 1)
      ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

리스트 분할은 동등 조건에서만 분할 프루닝을 지원하므로,
범위 조건 등 동등 조건이 아닌 경우에는 분할 프루닝이 적용되지 않는다.

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      p.host_year, p.nation_code, p.gold
  from
      participant_list as p
  where
      p.host_year < 1996
      and p.gold > 40
  order by
      p.host_year;

  show trace;

.. code-block:: text

      host_year  nation_code                  gold
  ================================================
           1988  'URS'                          55
           1992  'EUN'                          45

질의 실행 후 프로파일링 결과를 확인하면,
분할 프루닝이 적용되지 않아 모든 분할에 대한 스캔 정보가 **SCAN** 하위에 **PARTITION**\으로 출력된 것을 확인할 수 있다.

각 분할의 값을 기준으로 보면 ``p1996``, ``p2000``, ``p2004`` 분할은 스캔 대상에서 제외될 것처럼 보이지만,
리스트 분할은 정렬된 순서대로 값이 배치되는 구조가 아니므로 범위 조건만으로는 특정 분할이 스캔 대상에서 제외될 수 없다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 10, fetch_time: 0, ioread: 0)
      SCAN (table: public.participant_list), (heap time: 0, fetch: 5, ioread: 0, readrows: 916, rows: 2)
             PARTITION (table: public.participant_list__p__p1988), (heap time: 0, fetch: 1, ioread: 0, readrows: 156, rows: 1)
             PARTITION (table: public.participant_list__p__p1992), (heap time: 0, fetch: 1, ioread: 0, readrows: 165, rows: 1)
             PARTITION (table: public.participant_list__p__p1996), (heap time: 0, fetch: 1, ioread: 0, readrows: 196, rows: 0)
             PARTITION (table: public.participant_list__p__p2000), (heap time: 0, fetch: 1, ioread: 0, readrows: 197, rows: 0)
             PARTITION (table: public.participant_list__p__p2004), (heap time: 0, fetch: 1, ioread: 0, readrows: 202, rows: 0)
      ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

.. _example_partition-pruning_hash:

.. rubric:: 예제 5. 해시 분할의 분할 프루닝

이번 예제에서는 해시 분할 방식으로 분할 테이블을 생성한 뒤,
**WHERE** 절에 동등 조건을 사용하는 경우와 부정 조건을 사용하는 경우에 대해, 분할 프루닝의 적용 여부를 확인한다.

아래 질의는 **WHERE** 절에 분할 키 조건이 존재하며, 해당 조건은 **CREATE TABLE** 문에서 정의한 분할 키 표현식과 동일하다.
또한 해시 분할은 동등 조건에 대한 분할 프루닝을 지원하므로 분할 프루닝이 적용될 수 있다.

.. code-block:: sql

  drop table if exists stadium_hash;

  create table stadium_hash
  partition by hash (nation_code) partitions 4
  as (select * from stadium);

  update statistics on stadium_hash;

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      s.nation_code, s.name, s.seats
  from
      stadium_hash as s
  where
      s.nation_code = 'KOR'
      and s.seats >= 100000
  order by
      s.name;

  show trace;

.. code-block:: text

    nation_code           name                        seats
  =========================================================
    'KOR'                 'Seoul Olympic Stadium'       100000

질의 실행 후 프로파일링 결과를 확인하면,
``s.nation_code = 'KOR'`` 조건을 만족하는 ``p2`` 분할만 스캔되었으며,
그 외 나머지 분할은 스캔 대상에서 제외되었다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 0, fetch: 2, fetch_time: 0, ioread: 0)
      SCAN (table: public.stadium_hash), (heap time: 0, fetch: 1, ioread: 0, readrows: 32, rows: 1)
             PARTITION (table: public.stadium_hash__p__p2), (heap time: 0, fetch: 1, ioread: 0, readrows: 32, rows: 1)

해시 분할은 동등 조건에서만 분할 프루닝을 지원하므로,
부정 조건 등 동등 조건이 아닌 경우에는 분할 프루닝이 적용되지 않는다.

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      s.nation_code, s.name, s.seats
  from
      stadium_hash as s
  where
      s.nation_code != 'KOR'
      and s.seats > 100000
  order by
      s.name;

  show trace;

.. code-block:: text

    nation_code           name                        seats
  =========================================================
    'AUS'                 'Olympic Stadium'          115600

질의 실행 후 프로파일링 결과를 확인하면,
분할 프루닝이 적용되지 않아 모든 분할에 대한 스캔 정보가 **SCAN** 하위에 **PARTITION**\으로 출력된 것을 확인할 수 있다.

앞선 질의에서 ``s.nation_code = 'KOR'`` 조건을 만족하는 경우 ``p2`` 분할만 스캔되었으므로 부정 조건에서는 스캔 대상에서 제외될 것처럼 보이지만,
해당 분할에도 ``s.nation_code != 'KOR'`` 조건을 만족하는 레코드가 존재할 수 있어 스캔 대상에서 제외될 수 없다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 0, fetch: 8, fetch_time: 0, ioread: 0)
      SCAN (table: public.stadium_hash), (heap time: 0, fetch: 4, ioread: 0, readrows: 141, rows: 1)
             PARTITION (table: public.stadium_hash__p__p0), (heap time: 0, fetch: 1, ioread: 0, readrows: 27, rows: 0)
             PARTITION (table: public.stadium_hash__p__p1), (heap time: 0, fetch: 1, ioread: 0, readrows: 53, rows: 0)
             PARTITION (table: public.stadium_hash__p__p2), (heap time: 0, fetch: 1, ioread: 0, readrows: 32, rows: 0)
             PARTITION (table: public.stadium_hash__p__p3), (heap time: 0, fetch: 1, ioread: 0, readrows: 29, rows: 1)

.. _example_partition-pruning_join:

.. rubric:: 예제 6. 조인에서의 분할 프루닝

이번 예제에서는 조인 질의 시 분할 테이블이 드리븐(driven) 테이블로 사용되는 경우, 분할 프루닝의 적용 여부를 확인한다.

아래 질의는 힌트를 사용하여 분할 테이블이 드리븐 테이블로 사용되도록 조인 순서를 고정하며,
이에 따라 조인 조건이 분할 키 조건으로 활용될 것으로 기대할 수 있다.
하지만 분할 프루닝은 질의 실행 준비 단계에서 확인 가능한 값을 분석하여 결정된다.
따라서 조인 실행 시점에 컬럼 값이 확정되는 조인 조건으로는 질의 실행 준비 단계에서 값을 미리 알 수 없으므로, 분할 프루닝이 적용되지 않는다.

.. code-block:: sql

  drop table if exists olympic_range;

  create table olympic_range
  partition by range (host_year) (
      partition before_1920 values less than (1920),
      partition before_1940 values less than (1940),
      partition before_1960 values less than (1960),
      partition before_1980 values less than (1980),
      partition before_2000 values less than (2000),
      partition latest values less than maxvalue
    )
  as (select * from olympic);

  update statistics on olympic_range;

.. code-block:: sql

  set trace on;

  select /*+ recompile ordered */
      p.host_year, p.nation_code, p.gold,
      o.host_nation, o.slogan
  from
      participant as p
      inner join olympic_range as o on p.host_year = o.host_year
  where
      p.host_year in (1988, 2004)
      and p.gold > 40
  order by
      p.host_year, p.gold, p.nation_code;

  show trace;

.. code-block:: text

      host_year  nation_code                  gold  host_nation           slogan
  ============================================================================================
           1988  'URS'                          55  'Korea'               'Harmony and progress'

질의 실행 후 프로파일링 결과를 확인하면,
분할 프루닝이 적용되지 않아 모든 분할에 대한 스캔 정보가 **SCAN** 하위에 **PARTITION**\으로 출력된 것을 확인할 수 있다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 2204, fetch_time: 0, ioread: 0)
      SCAN (index: public.participant.pk_participant_host_year_nation_code), (btree time: 1, fetch: 2172, ioread: 0, readkeys: 2154, filteredkeys: 2148, rows: 2148) (lookup time: 1, rows: 6)
        SCAN (table: public.olympic_range), (heap time: 0, fetch: 31, ioread: 0, readrows: 25, rows: 1)
               PARTITION (table: public.olympic_range__p__before_1920), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 0)
               PARTITION (table: public.olympic_range__p__before_1940), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 0)
               PARTITION (table: public.olympic_range__p__before_1960), (heap time: 0, fetch: 4, ioread: 0, readrows: 3, rows: 0)
               PARTITION (table: public.olympic_range__p__before_1980), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 0)
               PARTITION (table: public.olympic_range__p__before_2000), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 1)
               PARTITION (table: public.olympic_range__p__latest), (heap time: 0, fetch: 3, ioread: 0, readrows: 2, rows: 0)
        MEMOIZE (time: 0, hit: 0, miss: 1, size: 0KB, enabled: true)

분할 테이블이 드리븐 테이블로 사용되는 경우에도,
조건절에 질의 실행 준비 단계에서 확인 가능한 값을 가진 분할 키 조건이 존재한다면 분할 프루닝이 적용될 수 있다.

.. code-block:: sql

  set trace on;

  select /*+ recompile ordered */
      p.host_year, p.nation_code, p.gold,
      o.host_nation, o.slogan
  from
      participant as p
      inner join olympic_range as o on p.host_year = o.host_year
  where
      o.host_year in (1988, 2004)
      and p.gold > 40
  order by
      p.host_year, p.gold, p.nation_code;

  show trace;

.. code-block:: text

      host_year  nation_code                  gold  host_nation           slogan
  ============================================================================================
           1988  'URS'                          55  'Korea'               'Harmony and progress'

질의 실행 후 프로파일링 결과를 확인하면,
``o.host_year in (1988, 2004)`` 조건을 만족하는 ``before_2000`` 분할과 ``latest`` 분할만 스캔되었으며,
그 외 나머지 분할은 스캔 대상에서 제외되었다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 1845, fetch_time: 1, ioread: 0)
      SCAN (table: public.participant), (heap time: 1, fetch: 1838, ioread: 0, readrows: 1832, rows: 6)
        SCAN (table: public.olympic_range), (heap time: 0, fetch: 6, ioread: 0, readrows: 21, rows: 1)
               PARTITION (table: public.olympic_range__p__before_2000), (heap time: 0, fetch: 3, ioread: 0, readrows: 15, rows: 1)
               PARTITION (table: public.olympic_range__p__latest), (heap time: 0, fetch: 3, ioread: 0, readrows: 6, rows: 0)
        MEMOIZE (time: 0, hit: 0, miss: 3, size: 0KB, enabled: true)

분할 키 조건에 바인드 변수가 사용되는 경우에도,
질의 실행 준비 단계에서 바인딩된 값을 확인할 수 있으므로 분할 프루닝이 적용될 수 있다.
여러 종류의 표현식이 중첩되더라도 질의 실행 준비 단계에서 상수로 평가될 수 있다면 분할 프루닝이 적용될 수 있다.

.. code-block:: sql

  set trace on;

  prepare q from '
    select /*+ ordered */
        p.host_year, p.nation_code, p.gold,
        o.host_nation, o.slogan
    from
        participant as p
        inner join olympic_range as o on p.host_year = o.host_year
    where
        o.host_year = year (to_date (?, ''YYYY-MM-DD''))
        and p.gold > 40
    order by
        p.host_year, p.gold, p.nation_code;
  ';

  execute q using '1988-01-01';

  show trace;

.. code-block:: text

      host_year  nation_code                  gold  host_nation           slogan
  ============================================================================================
           1988  'URS'                          55  'Korea'               'Harmony and progress'

질의 실행 후 프로파일링 결과를 확인하면,
``o.host_year = year (to_date ('1988-01-01', 'YYYY-MM-DD'))`` 조건을 만족하는 ``before_2000`` 분할만 스캔되었으며,
그 외 나머지 분할은 스캔 대상에서 제외되었다.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 165, fetch_time: 0, ioread: 0)
      SCAN (index: public.participant.pk_participant_host_year_nation_code), (btree time: 1, fetch: 158, ioread: 0, readkeys: 157, filteredkeys: 156, rows: 156) (lookup time: 1, rows: 1)
        SCAN (table: public.olympic_range), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 1)
               PARTITION (table: public.olympic_range__p__before_2000), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 1)

.. _example_partition-pruning_direct-access:

.. rubric:: 예제 7. 분할 직접 접근

이번 예제에서는 분할 프루닝 외에 **PARTITION** 절을 사용하여 특정 분할을 직접 조회하는 방법을 확인한다.
이를 통해 분할 테이블에는 적용할 수 없었던 최적화 기법이 분할 단위에서는 적용되는지 그 여부를 확인한다.

아래 질의는 분할 테이블을 대상으로 조회하므로, **ORDER BY** 및 **LIMIT** 절과 적절한 인덱스가 존재하더라도
**SKIP ORDER BY** 최적화는 적용되지 않는다.

.. code-block:: sql

  drop table if exists olympic_range;

  create table olympic_range
  partition by range (host_year) (
      partition before_1920 values less than (1920),
      partition before_1940 values less than (1940),
      partition before_1960 values less than (1960),
      partition before_1980 values less than (1980),
      partition before_2000 values less than (2000),
      partition latest values less than maxvalue
    )
  as (select * from olympic);

  create index i_olympic_range_host_year on olympic_range (host_year);

  update statistics on olympic_range;

.. code-block:: sql

  set optimization level 513;
  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_range as o
  where
      o.host_year > 2000
  order by
      o.host_year desc
  limit 1;

  show trace;

.. code-block:: text

  Join graph segments (f indicates final):
  seg[0]: [0]
  seg[1]: host_year[0] (f)
  seg[2]: host_nation[0] (f)
  seg[3]: host_city[0] (f)
  seg[4]: mascot[0] (f)
  Join graph nodes:
  node[0]: public.olympic_range o(25/6) (sargs 0) (loc 0)
  Join graph terms:
  term[0]: o.host_year range (2000 gt_inf max) (sel 0.1) (rank 2) (sarg term) (not-join eligible) (indexable host_year[0]) (loc 0)

  Query plan:

  temp(order by)
      subplan: iscan
                   class: o node[0]
                   index: i_olympic_range_host_year term[0]
                   cost:  3 card 2
      sort:  1 desc
      cost:  9 card 2

  Query stmt:

  select o.host_year, o.host_nation, o.host_city, o.mascot from olympic_range o where (o.host_year> ?:0 ) order by 1 desc  for orderby_num()<= ?:1

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           2004  'Greece'              'Athens'              'Athena  Phevos'

질의 실행 후 프로파일링 결과를 확인하면,
**SKIP ORDER BY** 최적화가 적용되지 않아 **ORDERBY**\에서 **TopNSort**\가 수행되었음을 확인할 수 있다.

.. code-block:: text

  Query Plan:
    SORT (order by)
      INDEX SCAN (o.i_olympic_range_host_year) (key range: (o.host_year> ?:0 ))

    rewritten query: select o.host_year, o.host_nation, o.host_city, o.mascot from [public.olympic_range] o where (o.host_year> ?:0 ) order by 1 desc  for orderby_num()<= ?:1

  Trace Statistics:
    SELECT (time: 0, fetch: 6, fetch_time: 0, ioread: 0)
      SCAN (index: public.olympic_range.i_olympic_range_host_year), (btree time: 0, fetch: 3, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 1) (lookup time: 0, rows: 1)
             PARTITION (index: public.olympic_range__p__latest.i_olympic_range_host_year), (btree time: 0, fetch: 3, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 1) (lookup time: 0, rows: 1)
      ORDERBY (time: 0, topnsort: true)

**PARTITION** 절을 통해 특정 분할만 직접 조회하면
일반 테이블과 동일하게 처리되므로, **SKIP ORDER BY** 최적화가 적용될 수 있다.

.. code-block:: sql

  set optimization level 513;
  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_range partition (latest) as o
  where
      o.host_year > 2000
  order by
      o.host_year desc
  limit 1;

  show trace;

.. code-block:: text

  Join graph segments (f indicates final):
  seg[0]: [0]
  seg[1]: host_year[0] (f)
  seg[2]: host_nation[0] (f)
  seg[3]: host_city[0] (f)
  seg[4]: mascot[0] (f)
  Join graph nodes:
  node[0]: public.olympic_range__p__latest o(25/1) (sargs 0) (loc 0)
  Join graph terms:
  term[0]: o.host_year range (2000 gt_inf max) (sel 0.1) (rank 2) (sarg term) (not-join eligible) (indexable host_year[0]) (loc 0)

  Query plan:

  iscan
      class: o node[0]
      index: i_olympic_range_host_year term[0] (desc_index)
      sort:  1 desc
      cost:  4 card 2

  Query stmt:

  select o.host_year, o.host_nation, o.host_city, o.mascot from olympic_range__p__latest o where (o.host_year> ?:0 ) order by 1 desc  for orderby_num()<= ?:1

  /* ---> skip ORDER BY */

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           2004  'Greece'              'Athens'              'Athena  Phevos'

질의 실행 후 프로파일링 결과를 확인하면,
**SKIP ORDER BY** 최적화가 적용되었음을 확인할 수 있다.

.. code-block:: text

  Query Plan:
    INDEX SCAN (o.i_olympic_range_host_year) (key range: (o.host_year> ?:0 ), desc_index: true)
    skip order by: true

    rewritten query: select o.host_year, o.host_nation, o.host_city, o.mascot from [public.olympic_range__p__latest] o where (o.host_year> ?:0 ) order by 1 desc  for orderby_num()<= ?:1

  Trace Statistics:
    SELECT (time: 0, fetch: 4, fetch_time: 0, ioread: 0)
      SCAN (index: public.olympic_range__p__latest.i_olympic_range_host_year), (btree time: 0, fetch: 3, ioread: 0, readkeys: 2, filteredkeys: 1, rows: 1) (lookup time: 0, rows: 1)

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

    ALTER [TABLE | CLASS] [schema_name.]table_name REMOVE PARTITIONING

*   *schema_name*: 테이블의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 변경하고자 하는 테이블의 이름을 지정한다.

분할 설정을 제거하면 각 분할에 있던 모든 데이터가 분할 테이블로 이동된다. 이는 비용이 많이 드는 작업으로 주의해서 계획해야 한다.

.. _reorganize-partitions:

분할 재구성
-----------

분할 재구성은 하나의 분할을 더 작은 분할들로 나누거나 한 그룹의 분할들을 하나의 분할로 병합하는 작업이다. 이를 수행하려면 **ALTER** 문의 **REORGANIZE PARTITION** 절을 사용한다. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name
    REORGANIZE PARTITION <alter_partition_name_comma_list>
    INTO "(" <partition_definition_comma_list> ")"
     
    partition_definition_comma_list ::=
    PARTITION partition_name VALUES LESS THAN "(" <range_value> ")", ... 

*   *schema_name*: 테이블의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
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

    ALTER [TABLE | CLASS] [schema_name.]table_name
    ADD PARTITION "(" <partition_definitions_comma_list> ")"

*   *schema_name*: 테이블의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
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

    ALTER [TABLE | CLASS] [schema_name.]table_name
    DROP PARTITION partition_name_list

*   *schema_name*: 테이블의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
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

    ALTER [TABLE | CLASS] [schema_name.]table_name
    COALESCE PARTITION number_of_shrinking_partitions

*   *schema_name*: 테이블의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name* : 재정의할 테이블의 이름을 지정한다.
*   *number_of_shrinking_partitions* : 삭제하려는 분할 개수를 지정한다.

다음은 :ref:`nation2 <hash-nation2-table>` 테이블의 분할 개수를 4 개에서 3 개로 줄이는 예제이다.

.. code-block:: sql

    ALTER TABLE nation2 COALESCE PARTITION 1;

**ALTER** 문의 **ADD PARTITION** 절을 사용하여 **ALTER** 해시 분할 테이블에 정의된 분할 개수를 늘릴 수 있다. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name
    ADD PARTITION PARTITIONS number

*   *schema_name*: 테이블의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
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

    ALTER TABLE [schema_name.]table_name PROMOTE PARTITION <partition_name_list>

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
