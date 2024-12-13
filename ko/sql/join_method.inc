.. _join-method:

조인 방식
========================================

관계형 데이터베이스는 정규화를 통해 데이터를 여러 테이블로 나누어 저장한다.
이 방법은 데이터 중복을 줄이고 데이터 무결성을 보장하는 데 효과적이다.
그러나 나누어진 데이터를 통합적으로 질의하려면 테이블 간의 관계를 결합해야 한다.
조인 연산은 이러한 관계를 결합하여 마치 단일 테이블에서 가져온 것처럼 결과를 반환한다.

.. rubric:: 조인 유형

조인 결과에 포함되는 레코드는 지정된 조인 유형에 따라 달라진다.
자세한 내용은 :ref:`join-query`\에서 확인할 수 있다.

.. rubric:: 조인 방식

테이블 간 레코드를 결합하는 방식은 조인 방식에 따라 달라진다. CUBRID는 다음과 같은 조인 방식을 지원한다:

  - :ref:`join-method_nested-loop`
  - :ref:`join-method_sort-merge`
  - :ref:`join-method_hash`

조인 방식은 질의 최적화기가 비용 비교를 통해 결정하며, 사용자가 직접 지정할 수도 있다.
질의 최적화기는 데이터베이스가 수집한 통계 정보를 바탕으로 각 조인 방식의 비용을 계산한 뒤
가장 낮은 비용을 가진 방식을 선택한다.
질의에 두 개 이상의 조인이 포함된 경우, 각 조인에 다른 조인 방식을 적용할 수 있다.
각 조인 방식은 한 번에 두 개의 테이블만 처리하며, 순차적으로 수행된다.
즉, 다음 조인은 이전 조인이 완료된 후에만 수행된다.

CUBRID에서는 기본적으로 **정렬 병합 조인** 또는 **해시 조인**\에 대한 실행 계획이 생성되지 않는다.
질의 최적화기가 이 두 조인 방식을 고려하도록 하려면 :ref:`SQL 힌트 <sql-hint>`\를 사용해야 한다.
**optimizer_enable_merge_join** 파라미터를 **yes**\(기본값은 **no**)로 설정하면 **USE_MERGE** 힌트를 사용하지 않고도 **정렬 병합 조인**\을 수행할 수 있다.
자세한 내용은 :ref:`other-parameters`\에서 확인할 수 있다.

.. note::

  여기서 **테이블**\은 물리적 테이블뿐만 아니라 뷰(가상 테이블) 및 파생 테이블을 포함하는 넓은 개념으로 사용된다.

.. _join-method_nested-loop:

중첩 반복(Nested Loop) 조인
----------------------------------------

중첩 반복 조인은 외부 테이블에서 레코드를 하나씩 읽은 후, 해당 레코드와 일치하는 내부 테이블의 레코드를 찾아 결합하는 방식이다.
이 방식은 프로그래밍에서 사용하는 중첩 반복문의 개념과 유사하다.
외부 반복에 사용되는 테이블을 **드라이빙 테이블(driving table)**\이라고 하며, 내부 반복에 사용되는 테이블은 **드리븐 테이블(driven table)**\이라고 한다.

.. rubric:: 드라이빙 테이블

두 테이블 중 하나를 드라이빙 테이블로 설정할 때는 일반적으로 레코드 수가 적은 테이블을 선택하는 것이 더 효율적이다.
드라이빙 테이블의 레코드 수가 적을수록 조인 연산 횟수가 줄어들어 전체 조인 비용이 감소한다.
랜덤 액세스 비용은 대체로 일정하므로, 드리븐 테이블에 대한 조인 연산 횟수가 줄어들면 성능이 더욱 향상된다.
질의 최적화기는 이러한 조인 비용을 비교하여 드라이빙 테이블을 자동으로 선택한다.
사용자가 **ORDERED** 또는 **LEADING** 힌트를 사용해 드라이빙 테이블을 직접 지정할 수도 있다.

.. rubric:: 드리븐 테이블

중첩 반복 조인은 드리븐 테이블에 랜덤 액세스가 가능한 경우 가장 효율적으로 작동한다.
이를 위해서는 조인 조건에 적합한 인덱스가 드리븐 테이블에 생성되어 있어야 한다.
만약 조인 조건에 적합한 인덱스가 없다면, 드리븐 테이블의 모든 레코드를 순차적으로 읽어야 하므로 조인 성능이 저하된다.
조인 조건에 적합한 인덱스를 통해 드리븐 테이블에 랜덤 액세스할 수 있으면,
실행 계획에서 중첩 반복 조인은 **인덱스 조인(idx-join)**\으로 표시된다.

.. rubric:: 부분 범위 처리

중첩 반복 조인은 두 테이블에서 각각 하나의 레코드를 읽은 상태에서도 조인 연산을 수행할 수 있으며,
모든 레코드에 대해서 조인이 완료되지 않은 상태에서도 즉시 반환할 수 있는 조인 결과를 유지한다.
이러한 동작은 부분 범위 처리를 가능하게 하므로, 처음 N개의 결과만 필요한 경우 N개의 결과가 준비되는 즉시 반환할 수 있다.
부분 범위 처리를 통해 불필요한 조인 연산을 피함으로써 전체적인 조인 성능이 향상된다.

.. rubric:: 해시 리스트 스캔

드리븐 테이블이 뷰 또는 파생 테이블인 경우, 인덱스를 사용할 수 없으므로
임시 결과의 레코드를 순차적으로 읽는 리스트 스캔 방식으로 조인을 수행한다.
해시 리스트 스캔은 리스트 스캔의 한 종류로, 임시 결과에서 조인 조건을 만족하는 레코드를 랜덤 액세스하기 위해 사용된다.
리스트 스캔은 임시 결과의 레코드를 읽는 방식이므로 물리적 테이블에는 사용할 수 없다.
따라서 해시 리스트 스캔도 드리븐 테이블이 뷰 또는 파생 테이블인 경우에만 사용된다.
해시 리스트 스캔은 :ref:`해시 조인 <join-method_hash>`\과 동작 방식이 유사하며, 조인 조건이 모두 동등 비교 조건일 때만 사용할 수 있다.
해시 리스트 스캔이 사용되었는지는 :ref:`질의 프로파일링 <query-profiling>`\을 통해 확인할 수 있다.

해시 리스트 스캔과 해시 조인의 차이점은 다음 두 가지이다:

  1. 해시 리스트 스캔은 해시 테이블 빌드에 드리븐 테이블만 사용한다.
  2. 해시 리스트 스캔은 중첩 반복 조인의 일부로 사용되므로 때문에 부분 범위 처리가 가능하다.

질의 최적화기는 조인 비용을 계산할 때 해시 리스트 스캔의 수행 여부는 반영하지 않는다.

.. _example_join-method_nested-loop_no-index:

.. rubric:: 예시 1. 인덱스 스캔을 수행하지 않는 중첩 반복 조인

이 예시에서는 중첩 반복 조인 수행 시 드리븐 테이블에서 인덱스 스캔이 수행되지 않는다.
**ORDERED** 힌트를 사용하여 조인 순서를 고정했으며, 그 결과 nation 테이블이 드라이빙 테이블이 되고, athlete 테이블이 드리븐 테이블이 된다.
드리븐 테이블에 조인 조건에 적합한 인덱스가 없기 때문에, nation 테이블의 각 레코드에 대해 athlete 테이블의 모든 레코드를 순차적으로 읽어야 한다.

  .. code-block:: sql

    select /*+ recompile ordered use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

  .. code-block:: none

    Query plan:

    nl-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   sargs: term[2]
                   cost:  2 card 31
        inner: sscan
                   class: a node[1]
                   sargs: term[0] AND term[1]
                   cost:  49 card 202
        cost:  4697 card 29

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 92, fetch: 1720, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (table: public.athlete), (heap time: 92, fetch: 1504, ioread: 0, readrows: 313819, rows: 103)

**수행 과정:**

  1. nation 테이블에서 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드를 읽는다.
  2. athlete 테이블에서 필터 조건(``a.event = 'Baseball'``)과 조인 조건(``n.code = a.nation_code``)을 만족하는 레코드를 읽는다.
  3. 조건이 만족되면 두 레코드를 결합해 결과에 추가한다.
  4. athlete 테이블에서 다음 레코드를 읽은 후 **2단계**\를 반복한다.
  5. nation 테이블에서 다음 레코드를 읽은 후 **1~3단계**\를 반복한다.
  6. 모든 레코드를 처리할 때까지 이 과정을 반복한다.

.. _example_join-method_nested-loop_index:

.. rubric:: 예시 2. 인덱스를 사용한 중첩 반복 조인

이 예시에서는 드리븐 테이블에서 인덱스 스캔을 사용하는 중첩 반복 조인을 수행한다.
드리븐 테이블(athlete)에 조인 조건에 적합한 인덱스를 생성한다.
그 결과, nation 테이블의 각 레코드에 대해 athlete 테이블의 모든 레코드를 읽을 필요 없이 랜덤 액세스가 가능하다.

  .. code-block:: sql

    create index i_athlete_nation_code on athlete (nation_code);

    select /*+ recompile ordered use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

    drop index i_athlete_nation_code on athlete;

  .. code-block:: none

    Query plan:

    idx-join (inner join)
        outer: sscan
                   class: n node[0]
                   sargs: term[2]
                   cost:  2 card 31
        inner: iscan
                   class: a node[1]
                   index: i_athlete_nation_code term[0]
                   sargs: term[1]
                   cost:  4 card 202
        cost:  68 card 29

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 1, fetch: 1265, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 1, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (index: public.athlete.i_athlete_nation_code), (btree time: 0, fetch: 1045, ioread: 0, readkeys: 24, filteredkeys: 0, rows: 948) (lookup time: 0, rows: 103)

:ref:`예시 1 <example_join-method_nested-loop_no-index>`\과 비교했을 때,
조인 조건을 비교하기 전 athlete 테이블에서 읽는 레코드 수가 크게 감소했다 (``readrows: 313,819`` -> ``rows: 948``).

.. _example_join-method_nested-loop_driving:

.. rubric:: 예시 3. 드라이빙 테이블에 따른 성능 차이

이 예시에서는 이전 두 예시와 달리 드라이빙 테이블을 athlete로 변경한다.
**LEADING** 힌트를 사용해 조인 순서를 지정했으며, 결과적으로 athlete 테이블이 드라이빙 테이블, nation 테이블이 드리븐 테이블이 된다.

  .. code-block:: sql

    select /*+ recompile leading(a, n) use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

  .. code-block:: none

    Query plan:

    idx-join (inner join)
        outer: sscan
                   class: a node[0]
                   sargs: term[1]
                   cost:  49 card 202
        inner: iscan
                   class: n node[1]
                   index: pk_nation_code term[0]
                   sargs: term[2]
                   cost:  2 card 31
        cost:  161 card 29

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 5, fetch: 7168, fetch_time: 0, ioread: 0)
        SCAN (table: public.athlete), (heap time: 5, fetch: 6709, ioread: 0, readrows: 6677, rows: 229)
          SCAN (index: public.nation.pk_nation_code), (btree time: 0, fetch: 458, ioread: 0, readkeys: 229, filteredkeys: 0, rows: 229) (lookup time: 0, rows: 103)

athlete 테이블을 드라이빙 테이블로 사용하면 레코드 수가 많아 성능이 저하될 수 있다.

.. _example_join-method_nested-loop_with-hash-list:

.. rubric:: 예시 4. 해시 리스트 스캔이 포함된 중첩 반복 조인

이 예시에서는 드리븐 테이블에서 해시 리스트 스캔이 수행되는 중첩 반복 조인을 보여준다.
**ORDERED** 및 **NO_MERGE** 힌트를 사용해 인덱스 스캔이 불가능한 인라인 뷰를 드리븐 테이블로 설정한다.
또한, 질의는 상위 20개의 결과만 조회해 해시 리스트 스캔과 부분 범위 처리를 함께 수행한다.

  .. code-block:: sql

    select /*+ recompile ordered use_nl */
        n.name, v.name
    from
        nation as n
        inner join (
                select /*+ no_merge */
                    a.name, a.nation_code
                from
                    athlete as a
                where
                    a.event = 'Baseball'
            ) as v on n.code = v.nation_code
    where
        n.continent = 'Asia'
    limit 20;

  .. code-block:: none

    Query plan:

    nl-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   sargs: term[1] AND term[2]
                   cost:  2 card 31
        inner: sscan
                   class: v node[1]
                   sargs: term[0]
                   cost:  2 card 202
        sargs: term[1]
        cost:  148 card 29

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 26, rows: 8)
          SCAN (hash temp(m), build time: 0, time: 0, fetch: 0, ioread: 0, readrows: 249, rows: 20)
        SUBQUERY (uncorrelated)
          SELECT (time: 2, fetch: 32, fetch_time: 0, ioread: 0)
            SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 229)

실제로 해시 리스트 스캔이 드리븐 테이블에서 수행되었는지는 실행 계획에서 확인할 수 없지만, 질의 프로파일링을 통해 확인할 수 있다.

.. _join-method_sort-merge:

정렬 병합(Sort-Merge) 조인
----------------------------------------

정렬 병합 조인은 두 테이블의 데이터를 조인 조건에 따라 정렬한 뒤, 정렬된 데이터를 순차적으로 비교하며 결합하는 방식이다.
이 방식은 조인 조건이 동등 조건일 때만 수행할 수 있다.

조인 전에 두 테이블의 데이터를 정렬하는 과정은 실시간으로 인덱스를 생성하는 것과 유사하다.
하지만 조인 조건에 적합한 인덱스가 존재하더라도 정렬 과정은 생략되지 않는다.
정렬된 데이터를 결합하는 과정은,
정렬된 두 임시 결과에서 각각 하나의 레코드를 읽으며 조인 연산을 수행한다는 점에서 중첩 반복 조인과 동작 방식이 비슷하다.
정렬 과정에서 사용되는 버퍼 크기는 **sort_buffer_size** 파라미터로 설정할 수 있으며,
자세한 내용은 :ref:`memory-parameters`\에서 확인할 수 있다.

.. rubric:: 조인 비용

정렬 병합 조인은 정렬 단계에서 비용이 크게 발생할 경우 비효율적일 수 있다.
질의 최적화기가 조인 비용을 비교할 때, 중첩 반복 조인은 사전 작업이 필요하지 않고,
해시 조인은 두 테이블 중 하나의 데이터만 사용해 해시 테이블을 빌드하므로 상대적으로 비용이 낮게 평가될 수 있다.
반면, 정렬 병합 조인은 두 테이블의 데이터를 모두 정렬해야 하므로 더 높은 비용으로 평가된다.

.. rubric:: 부분 범위 처리

정렬 병합 조인은 두 테이블의 정렬이 완료된 후에만 조인을 수행할 수 있으므로 부분 범위 처리가 불가능하다.
따라서 처음 N개의 결과만 필요한 경우에도 조인 작업이 모두 완료된 후에야 결과를 조회할 수 있다.

.. _example_join-method_sort-merge:

.. rubric:: 예시 1. 정렬 병합 조인의 수행 과정

이 예시는 정렬 병합 조인을 수행하는 과정을 보여준다.
**ORDERED** 힌트는 조인 성능에 직접적인 영향을 미치지 않지만, 여기서는 조인이 아래 지정된 순서대로 실행되도록 하기 위해 사용되었다.

  .. code-block:: sql

    create index i_nation_continent_code on nation (continent, code);
    create index i_athlete_event_nation_code on athlete (event, nation_code);

    select /*+ recompile ordered use_merge */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

    drop index i_nation_continent_code on nation;
    drop index i_athlete_event_nation_code on athlete;

  .. code-block:: none

    Query plan:

    temp
        order: UNORDERED
        subplan: m-join (inner join)
                     edge:  term[0]
                     outer: temp
                                order: code[0]
                                subplan: iscan
                                             class: n node[0]
                                             index: i_nation_continent_code term[1]
                                             cost:  2 card 1
                                cost:  8 card 1
                     inner: temp
                                order: nation_code[1]
                                subplan: iscan
                                             class: a node[1]
                                             index: i_athlete_event_nation_code term[2]
                                             cost:  2 card 7
                                cost:  8 card 7
                     cost:  17 card 1
        cost:  23 card 1

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 1, fetch: 287, fetch_time: 1, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 103, rows: 103)
          MERGELIST
            SELECT (time: 0, fetch: 52, fetch_time: 0, ioread: 0)
              SCAN (index: public.nation.i_nation_continent_code), (btree time: 0, fetch: 49, ioread: 0, readkeys: 47, filteredkeys: 0, rows: 47) (lookup time: 0, rows: 47)
              ORDERBY (time: 0, sort: true, page: 0, ioread: 0)
            SELECT (time: 1, fetch: 235, fetch_time: 1, ioread: 0)
              SCAN (index: public.athlete.i_athlete_event_nation_code), (btree time: 1, fetch: 231, ioread: 0, readkeys: 6, filteredkeys: 0, rows: 229) (lookup time: 1, rows: 229)
              ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

**수행 과정:**

1. nation 테이블에서 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드를 가져와 code 컬럼 값으로 정렬한다.

  .. code-block:: sql

    select n.code, n.name from nation as n where n.continent = 'Asia' order by n.code;

2. athlete 테이블에서 필터 조건(``a.event = 'Baseball'``)을 만족하는 레코드를 가져와 natione_code 컬럼 값으로 정렬한다.

  .. code-block:: sql

    select a.nation_code, a.name from athlete as a where a.event = 'Baseball' order by a.nation_code;

3. 두 정렬된 결과에서 레코드를 하나씩 순차적으로 읽고, 조인 조건(``n.code = a.nation_code``)을 비교한다. 조건이 만족되면 두 레코드를 결합하여 결과에 추가한다.
4. 모든 레코드가 처리될 때까지 이 과정을 반복하여 조인을 완료한다.

.. _join-method_hash:

해시(Hash) 조인
----------------------------------------

해시 조인은 해시 함수를 사용해 동일한 해시 값을 가지는 두 테이블의 레코드를 결합하는 방식이다.
동일한 해시 값을 가지는 레코드를 효율적으로 비교하기 위해 해시 테이블이 사용된다.
두 테이블 중 하나는 해시 테이블을 **빌드(Build)**\하는 데 사용되고, 다른 하나는 해시 테이블을 **프로브(Probe)**\하는 데 사용된다.
해시 값은 조인 컬럼 값을 입력으로 사용하는 해시 함수에 의해 생성된다.
다른 입력 값에서 동일한 해시 값이 생성될 경우 이를 해시 충돌이라고 한다.
해시 충돌이 발생하면 동일한 해시 값을 가진 레코드를 순차적으로 조사하며 조인 조건을 확인한다.
해시 조인은 모든 조인 조건이 동등 비교 조건인 경우에만 수행할 수 있다.

.. rubric:: SQL 힌트

해시 조인과 관련된 힌트는 두 가지가 있다:

  - **USE_HASH**: 질의 최적화기가 해시 조인에 대한 실행 계획을 생성할 수 있도록 허용한다.
  - **NO_USE_HASH**: 질의 최적화기가 해시 조인에 대한 실행 계획을 생성하지 못하도록 방지한다.

두 힌트 모두 하나 이상의 테이블을 지정해서 사용할 수 있다.
힌트에 특정 테이블이 명시된 경우, 해당 힌트는 내부 테이블(드리븐 테이블)에 적용된다.
테이블이 명시되지 않은 경우, 힌트는 모든 테이블에 적용된다.
동일한 테이블에 대해 **USE_HASH**\와 **NO_USE_HASH** 힌트가 모두 사용된 경우, **NO_USE_HASH** 힌트가 우선 적용된다.

.. rubric:: 비용 계산

해시 조인에 참여하는 각 테이블은 해시 테이블을 빌드하거나 프로브하는 데 사용될 수 있다.
따라서 두 가지 경우의 비용을 계산하여 비교한 뒤, 더 낮은 비용을 해시 조인의 최종 비용으로 결정한다.
예를 들어, 다음 두 가지 경우를 비교하여 비용이 낮은 방식을 선택한다:

  - 외부 테이블(드라이빙 테이블)로 해시 테이블을 빌드하고, 내부 테이블(드리븐 테이블)로 이를 프로브하는 경우.
  - 내부 테이블(드리븐 테이블)로 해시 테이블을 빌드하고, 외부 테이블(드라이빙 테이블)로 이를 프로브하는 경우.

.. rubric:: 해시 테이블을 빌드하는 데 사용되는 테이블

해시 테이블을 빌드할 테이블은 **ORDERED** 또는 **LEADING** 힌트를 사용해 지정할 수 없다.
일반적으로 레코드 수가 적은 테이블이 해시 테이블 빌드에 사용된다.
그러나 외부 조인의 경우, 조인 방향에 따라 해시 테이블을 빌드할 테이블이 고정된다:

  - **왼쪽 외부 조인**: 오른쪽 테이블이 해시 테이블 빌드에 사용된다.
  - **오른쪽 외부 조인**: 왼쪽 테이블이 해시 테이블 빌드에 사용된다.

.. rubric:: 해시 테이블을 빌드하는 방식

해시 테이블을 빌드하는 방식은 임시 결과 크기와 해시 테이블의 최대 크기에 따라 결정된다.
해시 테이블의 최대 크기는 **max_hash_list_scan_size** 파라미터를 통해 설정할 수 있다.
자세한 내용은 :ref:`memory-parameters`\에서 확인할 수 있다.

CUBRID는 다음과 같은 해시 방식을 지원한다:

  - **메모리 해시**: 임시 결과를 모두 해시 테이블에 저장할 수 있는 경우 사용된다.

    .. code-block:: none

      임시 결과 페이지 수 * 페이지 크기 <= 해시 테이블 최대 크기

  - **하이브리드 해시**: 해시 테이블이 임시 결과 레코드의 위치를 저장할 수 있는 경우 사용된다.

    .. code-block:: none

      임시 결과 레코드 수 * (해시 엔트리 크기 + 위치 정보 크기) <= 해시 테이블 최대 크기

  - **파일 해시**: 위 두 방식 모두 사용할 수 없는 경우 사용된다.
  - **스킵 해시**: 두 임시 결과 중 하나가 비어 있거나 불필요한 경우(외부 조인 시), 해시 테이블 빌드는 생략된다.

**하이브리드 해시**\와 **파일 해시**\는 레코드 위치를 해시 테이블에 저장하므로, 레코드를 읽기 전에 위치를 찾는 과정이 추가로 필요하다.

**메모리 해시**\와 **하이브리드 해시**\는 메모리 페이지를 사용해 해시 테이블을 빌드하지만, **파일 해시**\는 임시 볼륨의 페이지를 사용해 해시 테이블을 빌드한다.
임시 볼륨의 페이지 접근 시 논리적 I/O가 추가로 발생하며, 이는 성능 저하를 초래할 수 있다.

질의를 실행하는 동안, 조인이 완료되면 해시 테이블은 삭제되고 사용된 메모리는 반환된다.
해시 조인을 수행할 때 실제로 사용된 방식은 질의 프로파일링을 통해 확인할 수 있으며,
자세한 내용은 :ref:`query-profiling`\에서 확인할 수 있다.

.. rubric:: 부분 범위 처리

해시 조인은 해시 테이블 빌드가 완료된 이후에만 조인이 진행되므로, 부분 범위 처리는 불가능하다.

.. _example_join-method_hash:

.. rubric:: 예시 1. 해시 조인의 수행 과정

이 예시는 해시 조인을 수행하는 과정을 설명한다.
**ORDERED** 힌트를 사용하더라도 해시 테이블을 빌드할 테이블은 지정할 수 없다.

  .. code-block:: sql

    select /*+ recompile ordered use_hash */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

  .. code-block:: none

    Query plan:

    hash-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   sargs: term[2]
                   cost:  2 card 31
        inner: sscan
                   class: a node[1]
                   sargs: term[1]
                   cost:  49 card 202
        cost:  63 card 29

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 103, rows: 103)
          HASHJOIN (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
            BUILD (time: 0, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: memory)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 47)
            PROBE (time: 2, probe_time: 0, fetch: 0, fetch_time: 0, ioread: 0, readkeys: 103, rows: 103, max_collisions: 1)
              SELECT (time: 2, fetch: 32, fetch_time: 0, ioread: 0)
                SCAN (table: public.athlete), (heap time: 1, fetch: 32, ioread: 0, readrows: 6677, rows: 229)

nation 테이블에서는 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드가 47개이고,
athlete 테이블에서는 필터 조건(``a.event = 'Baseball'``)을 만족하는 레코드가 229개다.
따라서 더 적은 레코드를 가진 nation 테이블이 해시 테이블을 빌드하는 데 사용된다.

**질의 프로파일링 결과:**

여기서는 해시 조인과 관련된 항목들만 설명한다. 다른 항목들에 대해서는 :ref:`query-profiling`\에서 확인할 수 있다.

  1. **HASHJOIN**

    - **time**: 해시 조인 전체 실행 시간(ms)
    - **fetch**: 해시 조인에서 fetch한 페이지 수
    - **fetch_time**: 해시 조인에서 페이지를 fetch하는 데 걸린 시간(ms)
    - **ioread**: 해시 조인에서 페이지를 fetch하는 동안 발생한 물리적 I/O 읽기 횟수

  2. **BUILD**: **time**\을 제외하고, 나머지 항목들은 임시 결과를 조회할 때 누적된 정보를 포함하지 않는다.

    - **time**: 임시 결과 조회를 포함한 해시 테이블 빌드 시간(ms)
    - **build_time**: 해시 테이블 빌드 시간(ms)
    - **fetch**: 해시 테이블 빌드를 위해 fetch한 페이지 수
    - **fetch_time**: 해시 테이블 빌드에서 페이지를 fetch하는 데 걸린 시간(ms)
    - **ioread**: 해시 테이블 빌드에서 발생한 물리적 I/O 읽기 횟수
    - **hash_method**: 해시 테이블 빌드에 사용된 방식 (**memory**, **hybrid**, **file**, 또는 **skip**)

      - **memory**: 메모리 해시 방식 사용
      - **hybrid**: 하이브리드 해시 방식 사용
      - **file**: 파일 해시 방식 사용
      - **skip**: 해시 테이블 빌드를 생략

  3. **PROBE**: **time**\을 제외하고, 나머지 항목들은 임시 결과를 조회할 때 누적된 정보를 포함하지 않는다.

    - **time**: 임시 결과 조회를 포함한 해시 테이블 프로브 시간(ms)
    - **probe_time**: 해시 테이블 프로브 시간(ms)
    - **fetch**: 해시 테이블 프로브를 위해 fetch한 페이지 수
    - **fetch_time**: 해시 테이블 프로브에서 페이지를 fetch하는 데 걸린 시간(ms)
    - **ioread**: 해시 테이블 프로브에서 발생한 물리적 I/O 읽기 횟수
    - **readkeys**: 해시 테이블 프로브 중 해시 값이 일치한 레코드 수
    - **rows**: 해시 테이블 프로브 중 조인 조건을 만족하는 레코드 수 (**readkeys >= rows**)
    - **max_collisions**: 해시 테이블 프로브 중 하나의 해시 값에 대해 읽은 최대 레코드 수

**수행 과정:**

  1. nation 테이블에서 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드를 가져와 임시 결과를 생성한다.

    .. code-block:: sql

      select n.code, n.name from nation as n where n.continent = 'Asia';

  2. athlete 테이블에서 필터 조건(``a.event = 'Baseball'``)을 만족하는 레코드를 가져와 임시 결과를 생성한다.

    .. code-block:: sql

      select a.nation_code, a.name from athlete as a where a.event = 'Baseball';

  3. nation 테이블의 임시 결과를 읽어 해시 테이블을 빌드한다.
  4. athlete 테이블의 임시 결과에서 각 레코드에 대해 nation 테이블의 해시 테이블을 프로브한다.
  5. 동일한 해시 값을 가진 레코드를 찾아(``readkeys``) 조인 조건(``n.code = a.nation_code``)을 비교한다. 조건이 만족(``rows``)되면 두 레코드를 결합하여 결과에 추가한다.
  6. 동일한 해시 값을 가진 다음 레코드에 대해 **5단계**\를 반복한다.
  7. athlete 테이블의 다음 레코드에 대해 **4~6단계**\를 반복한다.
  8. 모든 레코드가 처리될 때까지 이 과정을 계속 반복하여 조인을 완료한다.

.. _example_join-method_hash_hint:

.. rubric:: 예시 2. 힌트를 사용한 해시 조인

이 예시는 **USE_HASH** 및 **NO_USE_HASH** 힌트를 사용해 특정 테이블에 대해 해시 조인을 수행하거나 수행하지 않는 방법을 보여준다.
**ORDERED** 힌트를 사용하여 내부 테이블(드리븐 테이블)을 고정한다.

  .. code-block:: sql

    select /*+ recompile ordered use_hash no_use_hash(r) */
        n.name, a.name, a.code, r.medal
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
        inner join record as r on a.code = r.athlete_code
    where
        n.continent = 'Asia'
        and a.event = 'Shooting'
        and r.host_year = 1992;

  .. code-block:: none

    Query plan:

    idx-join (inner join)
        outer: hash-join (inner join)
                   edge:  term[0]
                   outer: sscan
                              class: n node[0]
                              sargs: term[3]
                              cost:  2 card 31
                   inner: sscan
                              class: a node[1]
                              sargs: term[2]
                              cost:  49 card 202
                   cost:  63 card 29
        inner: iscan
                   class: r node[2]
                   index: pk_record_host_year_event_code_athlete_code_medal term[4] (covers)
                   filtr: term[1]
                   cost:  5 card 400
        cost:  109 card 2

**USE_HASH** 힌트를 통해 nation과 athlete 테이블에 대해 해시 조인이 수행된다.

**NO_USE_HASH** 힌트를 사용하여 record 테이블에서는 해시 조인이 수행되지 않는다.

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 5, fetch: 139, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 35, rows: 35)
          SCAN (index: public.record.pk_record_host_year_event_code_athlete_code_medal), (btree time: 3, fetch: 105, ioread: 0, readkeys: 7, filteredkeys: 0, rows: 7, covered: true)
          HASHJOIN (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
            BUILD (time: 1, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: memory)
              SELECT (time: 1, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 1, fetch: 1, ioread: 0, readrows: 215, rows: 47)
            PROBE (time: 1, probe_time: 0, fetch: 0, fetch_time: 0, ioread: 0, readkeys: 35, rows: 35, max_collisions: 1)
              SELECT (time: 1, fetch: 32, fetch_time: 0, ioread: 0)
                SCAN (table: public.athlete), (heap time: 1, fetch: 32, ioread: 0, readrows: 6677, rows: 173)

.. _example_join-method_hash_index_skip_scan:

.. rubric:: 예시 3. 인덱스 스킵 스캔을 수행하는 해시 조인

이 예시는 해시 조인을 수행할 때 인덱스 스킵 스캔을 사용하여 임시 결과를 가져오는 방법을 보여준다.

  .. code-block:: sql

    /* for nested loop join */
    create index i_athlete_nation_code_event on athlete (nation_code, event);

    /* for hash join */
    create index i_nation_continent_code on nation (continent, code);

    select /*+ recompile use_hash index_ss */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

    drop index i_athlete_nation_code_event on athlete;
    drop index i_nation_continent_code on nation;

  .. code-block:: none

    Query plan:

    hash-join (inner join)
        edge:  term[0]
        outer: iscan
                   class: n node[0]
                   index: i_nation_continent_code term[1]
                   cost:  2 card 1
        inner: iscan
                   class: a node[1]
                   index: i_athlete_nation_code_event term[2] (index skip scan)
                   cost:  13 card 202
        cost:  25 card 1

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 1, fetch: 748, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 103, rows: 103)
          HASHJOIN (time: 1, fetch: 748, fetch_time: 0, ioread: 0)
            BUILD (time: 0, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: memory)
              SELECT (time: 0, fetch: 52, fetch_time: 0, ioread: 0)
                SCAN (index: public.nation.i_nation_continent_code), (btree time: 0, fetch: 49, ioread: 0, readkeys: 47, filteredkeys: 0, rows: 47) (lookup time: 0, rows: 47)
            PROBE (time: 1, probe_time: 0, fetch: 0, fetch_time: 0, ioread: 0, readkeys: 103, rows: 103, max_collisions: 1)
              SELECT (time: 1, fetch: 696, fetch_time: 0, ioread: 0)
                SCAN (index: public.athlete.i_athlete_nation_code_event), (btree time: 1, fetch: 692, ioread: 0, readkeys: 121, filteredkeys: 0, rows: 229) (lookup time: 0, rows: 229)

.. _example_join-method_hash-right-outer:

.. rubric:: 예시 4. 해시 오른쪽 외부 조인

이 예시는 외부 조인의 방향에 따라 해시 테이블을 빌드하는 테이블이 결정되는 방식을 보여준다.

  .. code-block:: sql

    select /*+ recompile use_hash */
        n.name, p.host_year, p.gold
    from
        participant as p
        right outer join nation as n on n.code = p.nation_code and p.gold > 0
    where
        n.continent = 'Asia';

  .. code-block:: none

    Query plan:

    hash-join (right outer join)
        edge:  term[0]
        outer: sscan
                   class: p node[0]
                   sargs: term[1]
                   cost:  5 card 92
        inner: sscan
                   class: n node[1]
                   sargs: term[2]
                   cost:  2 card 31
        cost:  15 card 31

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 1, fetch: 4, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 70, rows: 70)
          HASHJOIN (time: 0, fetch: 4, fetch_time: 0, ioread: 0)
            BUILD (time: 0, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: memory)
              SELECT (time: 0, fetch: 3, fetch_time: 0, ioread: 0)
                SCAN (table: public.participant), (heap time: 0, fetch: 3, ioread: 0, readrows: 916, rows: 228)
            PROBE (time: 0, probe_time: 0, fetch: 0, fetch_time: 0, ioread: 0, readkeys: 36, rows: 70, max_collisions: 5)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 47)

participant 테이블에는 필터 조건(``p.gold > 0``)을 만족하는 레코드가 228개 있고,
nation 테이블에는 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드가 47개 있다.
participant 테이블의 레코드 수가 더 많지만, 우측 외부 조인의 경우 왼쪽 테이블(participant)이 해시 테이블을 빌드한다.

.. _example_join-method_hash-skip-build:

.. rubric:: 예시 5. 해시 테이블 빌드를 생략한 해시 외부 조인

이 예시는 해시 외부 조인에서 해시 테이블 빌드를 생략하는 경우를 보여준다.
왼쪽 외부 조인의 경우 오른쪽 테이블이 해시 테이블을 빌드한다.
그러나 participant 테이블에 필터 조건(``p.gold > 100``)을 만족하는 레코드가 없으므로 해시 테이블 빌드가 생략된다.
대신 nation 테이블에서 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드가 질의 결과로 반환된다.

  .. code-block:: sql

    select /*+ recompile use_hash */
        n.name, p.host_year, p.gold
    from
        nation as n
        left outer join participant as p on n.code = p.nation_code and p.gold > 100
    where
        n.continent = 'Asia';

  .. code-block:: none

    Query plan:

    hash-join (left outer join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   sargs: term[2]
                   cost:  2 card 31
        inner: sscan
                   class: p node[1]
                   sargs: term[1]
                   cost:  5 card 92
        cost:  15 card 31

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 1, fetch: 4, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 47, rows: 47)
          HASHJOIN (time: 1, fetch: 4, fetch_time: 0, ioread: 0)
            BUILD (time: 1, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: skip)
              SELECT (time: 1, fetch: 3, fetch_time: 0, ioread: 0)
                SCAN (table: public.participant), (heap time: 1, fetch: 3, ioread: 0, readrows: 916, rows: 0)
            PROBE (time: 0, probe_time: 0, fetch: 0, fetch_time: 0, ioread: 0, readkeys: 0, rows: 47, max_collisions: 0)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 47)

**BUILD** 항목에서는 해시 테이블 빌드 방식이 ``hash_method: skip``\으로 표시된다.
``skip``\은 해시 테이블 빌드가 생략되었음을 의미한다.
대신 nation 테이블에서 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드가 질의 결과로 반환된다.

.. _example_join-method_hash-method_hybrid:

.. rubric:: 예시 6. 하이브리드 해시 방식을 사용한 해시 조인

이 예시는 하이브리드 해시 조인을 수행하는 방법을 보여준다.
예시 데이터의 양이 적기 때문에, 하이브리드 해시 방식을 사용하려면 해시 테이블의 최대 크기를 낮춰야 한다.
이를 위해 **max_hash_list_scan_size** 파라미터를 **12K**\(기본값: **8M**)로 설정한다.

  .. code-block:: sql

    set system parameters 'max_hash_list_scan_size=12k';

    select /*+ recompile use_hash */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

  .. code-block:: none

    Query plan:

    hash-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   cost:  2 card 215
        inner: sscan
                   class: a node[1]
                   cost:  49 card 6677
        cost:  400 card 6677

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 5, fetch: 126, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 19, ioread: 0, readrows: 6677, rows: 6677)
          HASHJOIN (time: 5, fetch: 105, fetch_time: 0, ioread: 0)
            BUILD (time: 0, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: hybrid)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 215)
            PROBE (time: 5, probe_time: 2, fetch: 49, fetch_time: 0, ioread: 0, readkeys: 6692, rows: 6677, max_collisions: 2)
              SELECT (time: 3, fetch: 53, fetch_time: 0, ioread: 0)
                SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 6677)

동일한 테이블에 하나 이상의 조인 힌트를 사용하면,
질의 최적화기는 지정된 조인 방식들의 비용을 비교해 더 비용이 적은 방식을 선택한다.
생성된 실행 계획을 통해 질의 최적화기가 선택한 조인 방식을 확인할 수 있다.
다음은 **USE_HASH**\와 **USE_NL** 힌트를 함께 사용했을 때,
질의 최적화기가 하이브리드 해시 방식을 선택하여 해시 조인으로 실행 계획을 생성하고 수행한 예시이다.

  .. code-block:: sql

    set system parameters 'max_hash_list_scan_size=12k';

    select /*+ recompile use_hash use_nl */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

  .. code-block:: none

    Query plan:

    hash-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   cost:  2 card 215
        inner: sscan
                   class: a node[1]
                   cost:  49 card 6677
        cost:  400 card 6677

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 5, fetch: 126, fetch_time: 0, ioread: 0)
        SCAN (temp time: 1, fetch: 19, ioread: 0, readrows: 6677, rows: 6677)
          HASHJOIN (time: 4, fetch: 105, fetch_time: 0, ioread: 0)
            BUILD (time: 0, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: hybrid)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 215)
            PROBE (time: 4, probe_time: 2, fetch: 49, fetch_time: 0, ioread: 0, readkeys: 6692, rows: 6677, max_collisions: 2)
              SELECT (time: 2, fetch: 53, fetch_time: 0, ioread: 0)
                SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 6677)

.. _example_join-method_hash-method_file:

.. rubric:: 예시 7. 파일 해시 방식을 사용한 해시 조인

이 예시는 파일 해시 방식을 사용하여 해시 조인을 수행하는 방법을 보여준다.
예시 데이터의 양이 적기 때문에, 파일 해시 방식을 사용하려면 해시 테이블의 최대 크기를 **12K**\보다 더 낮춰야 한다.
이를 위해 **max_hash_list_scan_size** 파라미터를 **8K**\(기본값: **8M**)로 설정한다.

  .. code-block:: sql

    set system parameters 'max_hash_list_scan_size=8k';

    select /*+ recompile use_hash */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

  .. code-block:: none

    Query plan:

    hash-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   cost:  2 card 215
        inner: sscan
                   class: a node[1]
                   cost:  49 card 6677
        cost:  3846 card 6677

질의 최적화기는 데이터베이스가 수집한 통계를 바탕으로, 다음 계산을 통해 메모리 해시 방식 또는 하이브리드 해시 방식의 사용 가능 여부를 평가한다:

  .. code-block:: none

    임시 결과의 레코드 수 * (해시 엔트리 크기 + 위치 정보 크기) <= 해시 테이블 최대 크기
                                  215 * (32 byte * 16 byte) <= 8,192 byte
                                                     10,320 <= 8192 byte (False)

계산 결과가 설정된 메모리 값보다 커서 메모리나 하이브리드 방식 대신 파일 해시 방식이 선택된다.
파일 해시 방식(cost: 3,846)은 임시 볼륨 페이지를 사용하므로 다른 해시 방식(cost: 400)보다 비용이 더 높게 평가된다.

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 10, fetch: 27510, fetch_time: 3, ioread: 0)
        SCAN (temp time: 0, fetch: 19, ioread: 0, readrows: 6677, rows: 6677)
          HASHJOIN (time: 10, fetch: 27489, fetch_time: 3, ioread: 0)
            BUILD (time: 1, build_time: 1, fetch: 645, fetch_time: 1, ioread: 0, hash_method: file)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 215)
            PROBE (time: 9, probe_time: 7, fetch: 26772, fetch_time: 2, ioread: 0, readkeys: 6692, rows: 6677, max_collisions: 2)
              SELECT (time: 2, fetch: 53, fetch_time: 0, ioread: 0)
                SCAN (table: public.athlete), (heap time: 1, fetch: 32, ioread: 0, readrows: 6677, rows: 6677)

:ref:`예시 6 <example_join-method_hash-method_hybrid>`\의 질의 프로파일링 결과와 비교했을 때,
파일 해시 방식이 비용과 비례해서 수행시간이 느린 것을 확인할 수 있다.

동일한 테이블에 하나 이상의 조인 힌트를 사용하면,
질의 최적화기는 지정된 조인 방식들의 비용을 비교해 더 비용이 적은 방식을 선택한다.
생성된 실행 계획을 통해 질의 최적화기가 선택한 조인 방식을 확인할 수 있다.
다음은 **USE_HASH**\와 **USE_NL** 힌트를 함께 사용했을 때,
파일 해시 방식의 해시 조인 비용(cost: 3,846)이 높아 질의 최적화기가 중첩 반복 조인(cost: 3,405)으로 실행 계획을 생성하고 수행한 예시이다.

  .. code-block:: sql

    set system parameters 'max_hash_list_scan_size=8k';

    select /*+ recompile use_hash use_nl */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

  .. code-block:: none

    Query plan:

    idx-join (inner join)
        outer: sscan
                   class: a node[1]
                   cost:  49 card 6677
        inner: iscan
                   class: n node[0]
                   index: pk_nation_code term[0] (covers)
                   cost:  2 card 215
        cost:  3405 card 6677

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 19, fetch: 13387, fetch_time: 1, ioread: 0)
        SCAN (table: public.athlete), (heap time: 4, fetch: 6709, ioread: 0, readrows: 6677, rows: 6677)
          SCAN (index: public.nation.pk_nation_code), (btree time: 10, fetch: 6677, ioread: 0, readkeys: 6677, filteredkeys: 0, rows: 0, covered: true, count_only: true)
