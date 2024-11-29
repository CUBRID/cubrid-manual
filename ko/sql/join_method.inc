.. _join-method:

조인 방식
========================================

.. 배경 설명

관계형 데이터베이스는 정규화를 통해 데이터를 여러 개의 테이블로 나누어 저장한다.
정규화는 데이터 중복을 줄이고 데이터 무결성을 보장하는 데 효과적이다.
그러나 분리된 데이터의 통합된 결과에 대해 질의하려면 테이블 간의 관계를 결합해야 한다.
조인은 이런 관계를 통해 여러 테이블을 결합하고, 마치 단일 테이블인 것처럼 결과를 반환한다.

.. 기본 설명과 조인 타입

조인 질의는 여러 테이블, 뷰, 또는 파생 테이블의 데이터를 결합하여 원하는 정보를 조회하는 데 사용된다.
결과에 포함되는 레코드는 사용된 조인 유형에 따라 달라질 수 있다.
자세한 내용은 :ref:`join-query`\에서 확인할 수 있다.

.. note::

    여기서 **테이블**\은 실제 테이블뿐만 아니라 뷰(가상 테이블)와 파생 테이블까지 포함하는 넓은 개념으로 사용된다.

.. 조인 방식

테이블 간 레코드를 결합하는 방식은 조인 방식에 따라 달라진다.
조인 방식은 질의 최적화기가 비용 비교를 통해 결정하며, 사용자가 직접 지정할 수도 있다.

CUBRID는 다음과 같은 조인 방식을 지원한다:

- :ref:`join-method_nested-loop`
- :ref:`join-method_sort-merge`
- :ref:`join-method_hash`

질의에 두 개 이상의 조인이 포함된 경우에는 각 조인 방식이 서로 다르게 적용될 수 있다.
모든 조인 방식은 두 테이블 간의 결합만 수행하며, 각 조인 방식은 순차적으로 수행된다.
즉, 이전 조인이 완료된 후에 다음 조인이 수행된다.

질의 최적화기는 데이터베이스가 수집한 통계 정보를 바탕으로 각 조인 방식의 비용을 계산한다.
그런 다음, 가장 낮은 비용을 갖는 방식을 선택한다.
그러나 CUBRID에서는 기본적으로 정렬 병합 조인과 해시 조인에 대한 실행 계획을 생성하는 것은 고려되지 않는다.
이 두 방식을 사용하려면 **SQL 힌트**\를 명시적으로 지정해야 한다.  
자세한 내용은 :ref:`sql-hint`\에서 확인할 수 있다.

**optimizer_enable_merge_join** 파라미터를 **yes**(기본값은 **no**)로 설정하면,
**USE_MERGE** 힌트를 사용하지 않고도 정렬 병합 조인을 수행할 수 있다.
자세한 내용은 :ref:`other-parameters`\에서 확인할 수 있다.

.. _join-method_nested-loop:

중첩 반복(Nested Loop) 조인
----------------------------------------

.. 기본 설명

중첩 반복 조인은 외부 테이블에서 레코드를 하나씩 읽고,
외부 테이블의 레코드와 결합할 수 있는 내부 테이블의 레코드를 찾아 결합하는 방식이다.
이 방식은 프로그래밍에서 사용하는 중첩 반복문과 유사하다.
외부 반복에서 사용되는 테이블을 드라이빙 테이블이라고 하며, 내부 반복에서 사용되는 테이블을 드리븐 테이블이라고 한다.

.. 드라이빙 테이블

두 테이블 중 하나를 드리븐 테이블로 설정할 때, 레코드 수가 적은 테이블을 선택하는 것이 더 효율적이다.
드라이빙 테이블의 레코드 수가 적을수록 조인 연산 횟수가 줄어들기 때문이다.
랜던 액세스 비용은 대체로 일정하므로, 드리븐 테이블에 대해 랜덤 액세스가 가능할 때 조인 연산 횟수가 줄어들면 조인 비용이 감소한다.
조인 비용이 감소함에 따라 조인 성능이 향상된다.
질의 최적화기는 조인 비용을 비교해 드라이빙 테이블을 자동으로 선택한다.
사용자가 **ORDERED** 또는 **LEADING** 힌트를 사용하여 직접 드라이빙 테이블을 지정할 수도 있다.

.. 드리븐 테이블

중첩 반복 조인은 드리븐 테이블에 대해 랜덤 액세스가 가능한 경우에 효율적으로 동작한다.
랜덤 액세스가 가능하려면 조인 조건에 적합한 인덱스가 드리븐 테이블에 미리 생성되어 있어야 한다.
인덱스가 미리 생성되어 있지 않으면 드리븐 테이블의 모든 레코드를 순차적으로 읽으므로 조인 성능이 저하된다.
인덱스를 통해 드리븐 테이블에 랜덤 액세스가 가능한 경우,
실행 계획에서 중첩 반복 조인(``nl-join``)이 인덱스 조인(``idx-join``)으로 표시된다.

.. 부분 범위 처리

중첩 반복 조인은 두 테이블의 모든 레코드를 읽은 후 조인 연산을 수행하지 않는다.
조인 연산은 두 테이블에서 각각 하나의 레코드를 읽어서 수행되며, 조인 결과는 즉시 반환할 수 있다.
이러한 동작은 부분 범위 처리를 가능하게 하며,
처음 N개의 결과만 필요한 경우 N개의 결과가 채워지면 결과를 반환한다.
불필요한 조인 연산을 수행하지 않음으로써 조인 성능을 향상시킨다.

.. 해시 리스트 스캔

드리븐 테이블에 대한 랜덤 액세스가 불가능한 경우에도,
CUBRID는 조인 조건이 모두 동등 비교 조건이라면 해시 리스트 스캔을 통해 랜덤 액세스를 지원한다.
해시 리스트 스캔은 해시 조인과 같은 방식으로 수행되지만, 해시 테이블을 드리븐 테이블로 빌드한다는 점이 다르다.
또한, 해시 리스트 스캔은 중첩 반복 조인과 함께 사용되어 부분 범위 처리가 가능하지만,
해시 조인은 불가능하다.
자세한 내용은 :ref:`join-method_hash`\에서 확인할 수 있다.

해시 리스트 스캔이 사용되었는지는 질의 프로파일링을 통해 확인할 수 있다.
자세한 내용은 :ref:`query-profiling`\에서 확인할 수 있다.

.. _example_join-method_nested-loop_no-index:

.. rubric:: 예시 1. 인덱스 스캔을 수행하지 않는 중첩 반복 조인

이 예시에서는 중첩 반복 조인을 수행할 때 드리븐 테이블에서 인덱스 스캔이 수행되지 않는다.
**ORDERED** 힌트는 조인 순서를 고정하기 위해 사용되며,
그 결과 nation 테이블은 드라이빙 테이블이 되고, athlete 테이블은 드리븐 테이블이 된다.
드리븐 테이블에는 조인 조건에 적합한 인덱스가 생성되어 있지 않기 때문에
nation 테이블의 각 레코드에 대해 athlete 테이블의 모든 레코드를 순차적으로 읽어야 한다.

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
    :emphasize-lines: 4

    Trace Statistics:
      SELECT (time: 92, fetch: 1720, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (table: public.athlete), (heap time: 92, fetch: 1504, ioread: 0, readrows: 313819, rows: 103)

**수행 과정:**

1. nation 테이블에서 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드를 읽는다.
2. athlete 테이블에서 필터 조건(``a.event = 'Baseball'``)을 만족하는 레코드를 읽는다.
3. 두 테이블의 현재 레코드에 대해 조인 조건(``n.code = a.nation_code``)을 비교하여 조건을 만족하면 두 레코드를 결합하여 결과에 추가한다.
4. athlete 테이블에서 필터 조건을 만족하는 다음 레코드를 읽고, 3단계를 반복한다.
5. nation 테이블에서 필터 조건을 만족하는 다음 레코드를 읽고, 2~4단계를 반복한다.
6. 모든 조인 연산이 완료될 때까지 위 과정을 반복한다.

.. _example_join-method_nested-loop_index:

.. rubric:: 예시 2. 인덱스 스캔을 수행하는 중첩 반복 조인

이 예시는 중첩 반복 조인을 수행할 때 드리븐 테이블에서 인덱스 스캔을 수행한다.
조인 조건에 적합한 인덱스를 드리븐 테이블에 추가한다.
그 결과 nation 테이블의 각 레코드에 대해 athlete 테이블의 모든 레코드를 읽을 필요가 없으며, 랜덤 액세스가 가능하다.

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
    :emphasize-lines: 4

    Trace Statistics:
      SELECT (time: 1, fetch: 1265, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 1, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (index: public.athlete.i_athlete_nation_code), (btree time: 0, fetch: 1045, ioread: 0, readkeys: 24, filteredkeys: 0, rows: 948) (lookup time: 0, rows: 103)

:ref:`예시 1 <example_join-method_nested-loop_no-index>`\의 질의 프로파일링 결과와 비교하면,
athlete 테이블에서 조인 조건을 비교하기 전에 읽는 레코드 수가 크게 감소했다. (313,819 -> 948)

.. _example_join-method_nested-loop_driving:

.. rubric:: 예시 3. 드라이빙 테이블에 따른 성능 차이

이 예시는 이전 두 예시에서 nation 테이블을 드라이빌 테이블로 사용한 것과 달리 athlete 테이블을 드라이빙 테이블로 사용한다.
**LEADING** 힌트는 조인 순서를 지정하기 위해 사용하며,
그 결과 athlete 테이블은 드라이빙 테이블이 되고, athlete 테이블은 드리븐 테이블이 된다.

.. code-block:: sql

    select /*+ recompile leading(a, n) use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

실행 계획에 따르면, nation 테이블에서 필터 조건(n.continent = 'Asia')을 만족하는 레코드는 31개,
athlete 테이블에서 필터 조건(a.event = 'Baseball')을 만족하는 레코드는 202개로 예측된다.

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

:ref:`예시 2 <example_join-method_nested-loop_index>`\의 질의 프로파일링 결과와 비교했을 때,
athlete 테이블을 드라이빙 테이블로 사용하면 레코드 수가 많아 성능이 감소했음을 확인할 수 있다.

.. code-block:: none
    :emphasize-lines: 4

    Trace Statistics:
      SELECT (time: 5, fetch: 7168, fetch_time: 0, ioread: 0)
        SCAN (table: public.athlete), (heap time: 5, fetch: 6709, ioread: 0, readrows: 6677, rows: 229)
          SCAN (index: public.nation.pk_nation_code), (btree time: 0, fetch: 458, ioread: 0, readkeys: 229, filteredkeys: 0, rows: 229) (lookup time: 0, rows: 103)

.. _example_join-method_nested-loop_with-hash-list:

.. rubric:: 예시 4. 해시 리스트 스캔을 수행하는 중첩 반복 조인

이 예시는 드리븐 테이블에서 해시 리스트 스캔이 수행되는 것을 보여준다.
**ORDERED**\와 **NO_MERGE** 힌트를 함께 사용해서 인덱스 스캔을 수행하지 못하는 인라인 뷰가 드리븐 테이블이 되게 한다.
그런 다음, 해시 리스트 스캔과 부분 범위 처리가 함께 수행될 수 있도록 처음 20개의 결과만 조회한다.

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
    :emphasize-lines: 4

    Trace Statistics:
      SELECT (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 26, rows: 8)
          SCAN (hash temp(m), build time: 0, time: 0, fetch: 0, ioread: 0, readrows: 249, rows: 20)
        SUBQUERY (uncorrelated)
          SELECT (time: 2, fetch: 32, fetch_time: 0, ioread: 0)
            SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 229)

드리븐 테이블에서 해시 리스트 스캔이 수행되었는지는 실행 계획으로는 확인할 수 없지만,
질의 프로파일링 결과를 통해 알 수 있다.

.. code-block:: none

    SCAN (hash temp(m), build time: 0, time: 0, fetch: 0, ioread: 0, readrows: 249, rows: 20)

- ``hash temp(m)``: 해시 테이블은 메모리(m) 해시 방식으로 빌드되었다.
- ``rows: 20``: 조인 연산은 처음 20개 결과에 대해서만 수행되고, 종료되었다.

.. code-block:: none

    SUBQUERY (uncorrelated)
      SELECT (time: 2, fetch: 32, fetch_time: 0, ioread: 0)
        SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 229)

해시 테이블을 빌드하기 위해 athlete 테이블에서 필터 조건(``a.event = 'Baseball'``)을 만족하는 레코드를 모두 읽었다는 것을 알 수 있다.

.. _join-method_sort-merge:

정렬 병합(Sort Merge) 조인
----------------------------------------

.. 기본 설명

정렬 병합 조인은 두 테이블의 레코드를 조인 조건에 따라 정렬한 후, 정렬된 레코드를 순차적으로 비교하며 결합하는 방식이다.
이 방식은 조인 조건이 동등 조건인 경우에만 수행될 수 있다.

.. 정렬 과정

조인 전에 두 테이블의 레코드를 정렬하는 과정은 실시간으로 인덱스를 생성하는 것과 유사하다.
정렬된 레코드를 순차적으로 결합하는 과정은 중첩 반복 조인의 동작 방식과 유사하다.
정렬 과정은 조인 조건에 적합한 ㅇ니덱스가 존재하더라도 생략되지 않는다.

정렬 과정에서 사용되는 버퍼의 크기는 **sort_buffer_size** 파라미터로 설정할 수 있다.
자세한 내용은 :ref:`memory-parameters`\에서 확인할 수 있다.

정렬 병합 조인은 데이터 정렬 과정에서 비용이 많이 발생할 경우 비효율적일 수 있다.
중첩 반복 조인은 선행 작업이 필요하지 없고, 해시 조인도 해시 테이블을 빌드하기 위해 두 테이블 중 하나의 데이터만 필요하다.
두 방식은 두 테이블을 정렬해야 하는 정렬 병합 조인보다 비용이 더 낮게 평가될 수 있다.

.. 부분 범위 처리

정렬 병합 조인은 두 테이블의 정렬이 모두 완료된 후에 수행될 수 있으므로 부분 범위 처리가 불가능하다.
따라서 처음 N개의 결과만 필요한 경우에도 조인이 모두 완료된 후에만 결과를 조회할 수 있다.

.. _example_join-method_sort-merge:

.. rubric:: 예시 1. 정렬 병합 조인의 수행 과정

이 예시는 정렬 병합 조인을 수행하는 과정을 보여준다.
조인 성능에 영향을 미치지는 않지만, 여기서는 아래와 동일한 순서로 조인이 수행되도록 **ORDERED** 힌트를 사용한다.

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

1. nation 테이블에서 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드를 읽고, code 컬럼 값을 기준으로 정렬한다.

    .. code-block:: sql

        select n.code, n.name from nation as n where n.continent = 'Asia' order by n.code;

2. athlete 테이블에서 필터 조건(``a.event = 'Baseball'``)을 만족하는 레코드를 읽고, natione_code 컬럼 값을 기준으로 정렬한다.

    .. code-block:: sql

        select a.nation_code, a.name from athlete as a where a.event = 'Baseball' order by a.nation_code;

3. 두 정렬 결과에서 순차적으로 레코드를 하나씩 읽고, 조인 조건(``n.code = a.nation_code``)을 비교하여 조건을 만족하면 두 레코드를 결합하여 결과에 추가한다.
4. 모든 조인 연산이 완료될 때까지 위 과정을 반복한다.

.. _join-method_hash:

해시(Hash) 조인
----------------------------------------

.. 기본 설명

해시 조인은 해시 함수를 사용하여 동일한 해시 값을 가지는 두 테이블의 레코드를 결합하는 방식이다.
동일한 해시 값을 가지는 레코드를 모아서 효율적으로 비교하기 위해서 해시 테이블을 사용한다.
두 테이블 중 하나는 해시 테이블을 빌드하는 데 사용되고, 다른 하나는 해시 테이블을 프로브하는 데 사용된다.
해시 값은 조인 컬럼 값을 입력으로 사용하는 해시 함수에 의해 생성된다.
서로 다른 입력 값에서 동일한 해시 값이 생성되는 경우 이를 해시 충돌이라고 한다.
해시 충돌이 발생하면 해시 값을 가진 레코드를 순차적으로 조사하고 조인 조건을 비교한다.
해시 조인은 조인 조건이 모두 동등 비교 조건인 경우에만 수행할 수 있다.

.. 힌트

해시 조인과 관련된 힌트는 두 가지가 있다.

- **USE_HASH**: 질의 최적화기가 해시 조인에 대한 실행 계획을 생성할 수 있도록 한다.
- **NO_USE_HASH**: 질의 최적화기가 해시 조인에 대한 실행 계획을 생성하지 못하도록 한다.

두 힌트 모두 하나 이상의 테이블을 지정해서 사용할 수 있다. 
힌트에 지정된 테이블이 내부 테이블(드리븐 테이블)일 때 힌트를 적용한다.
힌트에 테이블이 지정되지 않으면 힌트는 모든 테이블에 적용된다.
동일한 테이블에서 **USE_HASH**\와 **NO_USE_HASH** 힌트를 함께 사용하는 경우, **NO_USE_HASH** 힌트만 적용된다.

.. 비용 계산

해시 조인에 참여하는 각 테이블은 해시 테이블을 빌드하거나 해시 테이블을 프로브하는 데 사용될 수 있다.
따라서 두 경우의 비용을 계산하여 비교하고, 가장 낮은 비용을 해시 조인 비용으로 결정한다.

예를 들어, 다음 두 가지 경우 중 가장 낮은 비용을 사용한다:

- outer 테이블(드라이빙 테이블)로 해시 테이블을 빌드하고, inner 테이블(드리븐 테이블)로 해시 테이블을 프로브하는 비용
- inner 테이블(드리븐 테이블)로 해시 테이블을 빌드하고, outer 테이블(드라이빙 테이블)로 해시 테이블을 프로브하는 비용

.. 해시 테이블 빌드에 사용되는 테이블

해시 테이블을 빌드하는 테이블은 **ORDERED** 또는 **LEADING** 힌트를 사용하여 지정할 수 없다.
레코드 수가 적은 테이블을 사용하여 해시 테이블을 빌드한다.
하지만 외부 조인의 경우 조인 방향에 따라 해시 테이블을 빌드에 사용되는 테이블이 고정된다.

- 왼쪽 외부 조인: 해시 테이블 빌드에 오른쪽 테이블이 사용된다.
- 오른쪽 외부 조인: 해시 테이블 빌드에 왼쪽 테이블이 사용된다.

.. 해시 방식

해시 테이블을 빌드하는 방식은 임시 결과 크기와 해시 테이블의 최대 크기에 따라 결정된다.
해시 테이블의 최대 크기는 **max_hash_list_scan_size** 파라미터로 설정할 수 있다.
자세한 내용은 :ref:`memory-parameters`\에서 확인할 수 있다.

CUBRID는 다음과 같은 해시 방식을 지원한다:

- **메모리 해시**: 임시 결과를 모두 해시 테이블에 저장할 수 있는 경우에 사용된다. **(임시 결과의 페이지 수 * 페이지 크기 <= 해시 테이블 최대 크기)**
- **하이브리드 해시**: 임시 결과의 개수만큼 해시 테이블에 레코드 위치를 저장할 수 있는 경우에 사용된다. **(임시 결과의 레코드 수 * (해시 엔트리 크기 + 위치 정보 크기) <= 해시 테이블 최대 크기)**
- **파일 해시**: 다른 모든 방식을 사용할 수 없는 경우에 사용된다.

**하이브리드 해시**\와 **파일 해시**\는 해시 테이블에 레코드의 위치를 저장하므로, 레코드 읽기에 앞서 레코드 위치를 찾는 과정이 추가된다.

**메모리 해시**\와 **하이브리드 해시**\는 메모리 페이지를 사용하여 해시 테이블을 빌드하지만,
**파일 해시**\는 임시 볼륨의 페이지를 사용하여 해시 테이블을 빌드한다.
**파일 해시**\는 임시 볼륨의 페이지에 접근할 때 래치를 발생시킬 수 있으며, 이로 인해 성능이 저하될 수 있다.

해시 조인을 수행할 때 실제로 사용되는 방법은 질의 프로파일링을 통해 알아낼 수 있다.
자세한 내용은 :ref:`query-profiling`\에서 확인할 수 있다.

질의를 실행하는 동안에도 조인이 완료되면 해시 테이블은 삭제되고, 사용된 메모리도 반환된다.

.. 부분 범위 처리

해시 조인은 해시 테이블 빌드가 완료된 후에 수행되므로 부분 범위 처리가 불가능하다.

.. _example_join-method_hash:

.. rubric:: 예시 1. 해시 조인의 수행 과정

이 예시는 해시 조인을 수행하는 과정을 보여준다.
**ORDERED** 힌트를 사용하더라도 헤시 테이블을 만들 테이블을 지정할 수 없다.

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

nation 테이블에는 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드가 47개 있고,
athlete 테이블에는 필터 조건(``a.event = 'Baseball'``)을 만족하는 레코드가 229개 있다.
따라서 레코드 수가 적은 nation 테이블을 사용하여 해시 테이블을 빌드한다.

**질의 프로파일링 결과:**

1. **HASHJOIN**

    - time: 해시 조인 수행 시간(ms)
    - fetch: 해시 조인 중 관련 페이지의 fetch 횟수
    - fetch_time : 해시 조인 중 페이지 fetch 수행 시간(ms)
    - ioread: 해시 조인 동안 물리적으로 디스크에 접근한 I/O 읽기 횟수

2. **BUILD**

    - time: 해시 테이블을 생성하는 테이블을 조회하는 데 걸린 시간(ms)에 build_time이 더해진다.
    - build_time: 해시 테이블을 생성하는 데 걸린 시간(ms)
    - fetch: 해시 테이블 빌드 중 페이지 fetch 횟수
    - fetch_time : 해시 테이블 빌드 중 페이지 fetch 수행 시간(ms)
    - ioread: 해시 테이블 빌드 중 물리적으로 디스크에 접근한 I/O 읽기 횟수
    - hash_method: 해시 테이블을 생성하는 방식(memory, hybrid, file, skip 등)

3. **PROBE**

    - time: 해시 테이블을 탐색하는 테이블을 조회하는 데 걸린 시간(ms)에 probe_time 더해진다.
    - probe_time: 해시 테이블을 탐색하는 데 걸린 시간(ms)
    - fetch: 해시 테이블 탐색 중 페이지 fetch 횟수
    - fetch_time : 해시 테이블 탐색 중 페이지 fetch 수행 시간(ms)
    - ioread: 해시 테이블 탐색 중 물리적으로 디스크에 접근한 I/O 읽기 횟수
    - readkeys: 해시 테이블 탐색 중 해시 값이 동일한 레코드 개수
    - rows: 해시 테이블 탐색 중 조인 조건을 만족하는 레코드 개수. (rows <= readkeys)
    - max_collisions: 해시 테이블 탐색 중 해시 값이 동일한 레코드의 최대 개수

**수행 과정:**

1. nation 테이블에서 필터 조건(``n.continent = 'Asia'``)을 만족하는 레코드를 조회하여 임시 결과를 생성한다.

    .. code-block:: sql

        select n.code, n.name from nation as n where n.continent = 'Asia';

2. athlete 테이블에서 필터 조건(``a.event = 'Baseball'``)을 만족하는 레코드를 조회하여 임시 결과를 생성한다.

    .. code-block:: sql

        select a.nation_code, a.name from athlete as a where a.event = 'Baseball';

3. nation 테이블의 임시 결과를 읽어 해시 테이블을 빌드한다.
4. athlete 테이블의 임시 결과에서 레코드를 하나씩 읽어 해시 테이블을 탐색한다.
5. 해시 값이 동일한 레코드를 찾은 후, 조인 조건(``n.code = a.nation_code``)을 비교하여 조건을 만족하면 두 레코드를 결합하여 결과에 추가한다.
6. 해시 값이 동일한 다음 레코드에 대해 5단계를 반복한다.
7. athlete 테이블의 다음 레코드에 대해 4~6단계를 반복한다.
8. 모든 조인 연산이 완료될 때까지 위 과정을 반복한다.

.. _example_join-method_hash_hint:

.. rubric:: 예시 2. 해시 조인에 대한 힌트 사용

이 예시는 **USE_HASH**\와 **NO_USE_HASH** 힌트를 사용하여 어떤 테이블에서 해시 조인을 수행해야 하고 어떤 테이블에서 수행하지 않아야 하는지 지정하는 방법을 보여준다.
**ORDERED** 힌트를 사용하여 힌트가 적용되는 내부 테이블(드리븐 테이블)을 고정한다.

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

**USE_HASH** 힌트를 사용하여, nation과 athlete 테이블이 해시 조인을 수행한다.
record 테이블을 지정해서 **NO_USE_HASH** 힌트가 사용하여, record 테이블과는 해시 조인을 수행하지 않는다.

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

.. _example_join-method_hash_index:

.. rubric:: 예시 3. 중첩 반복 조인과는 인덱스 설계가 다른 해시 조인

이 예시는 중첩 반복 조인과 해시 조인에서 인덱스 설계가 어떻게 달라질 수 있는지를 보여준다.

중첩 반복 조인은 드리븐 테이블을 랜덤 액세스하기 위해 조인 조건에 맞는 인덱스가 필요하다.
반면, 해시 조인은 조인 조건과 관계없이 필터 조건이 적용된 임시 결과를 먼저 조회하기 때문에 중첩 반복 조인과는 다른 인덱스가 필요하다.

두 조인 방식을 모두 고려하여 인덱스를 설계하는 것이 이상적이지만, 그렇지 않은 경우 **INDEX_SS** 힌트를 사용해 인덱스 스킵 스캔 수행을 고려할 수 있다.
인덱스 스킵 스캔은 인덱스의 선행 컬럼만 스킵할 수 있으므로, 조인 조건의 컬럼이 여러 개인 경우에는 수행이 제한될 수 있다.

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

.. rubric:: 예시 4. 오른쪽 해시 외부 조인

이 예시는 외부 조인에서 조인 방향에 따라 해시 테이블 생성에 사용되는 테이블이 고정되는 것을 보여준다.
오른쪽 외부 조인에서는 왼쪽 테이블이 해시 테이블을 생성하는 데 사용된다.

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

participant 테이블에서 필터 조건(p.gold > 0)을 만족하는 레코드는 228개이고,
nation 테이블에서 필터 조건(n.continent = 'Asia')을 만족하는 레코드는 47개이다.
오른쪽 외부 조인에서는 왼쪽 테이블이 해시 테이블을 생성하는 데 사용되므로,
이 경우 participant 테이블이 해시 테이블을 생성하는 데 사용된다.

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

.. _example_join-method_hash-left-outer:

.. rubric:: 예시 5. 해시 테이블 생성이 생략된 해시 외부 조인

이 예시는 외부 조인에서 해시 테이블 생성이 생략되는 경우를 보여준다.
왼쪽 외부 조인에서는 오른쪽 테이블이 해시 테이블 생성에 사용된다.

만약 두 테이블 중 하나라도 데이터가 없으면 해시 테이블은 생성되지 않는다.
이 예시에서 participant 테이블에는 필터 조건(p.gold > 100)을 만족하는 레코드가 없으므로, 해시 테이블 생성을 생략된다.
필터 조건(n.continent = 'Asia')이 적용된 왼쪽 테이블의 결과가 질의 결과로 조회된다.

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

**BUILD** 항목에서 해시 테이블 생성에 사용된 방식이 hash_method: skip으로 표시된다.
**skip**\은 해시 테이블 생성이 생략되었음을 의미한다.

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

.. _example_join-method_hash-method_hybrid:

.. rubric:: 예시 6. 하이브리드 해시 방식을 사용하는 해시 조인

이 예시는 하이브리드 해시 방식을 사용하는 해시 조인의 수행 결과를 보여준다.

하이브리드 해시 방식을 사용하기 위해 **max_hash_list_scan_size** 파라미터를 **12K**\(기본값: **8M**)로 설정한다.

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

다음은 동일한 예시에서 **USE_NL**\과 **USE_HASH** 힌트를 함께 사용했을 때, 질의 최적화기가 어떤 조인 방식을 선택하는지를 보여준다.
이 경우에도 동일하게 해시 조인이 수행된다.

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

.. rubric:: 예시 7. 파일 해시 방식을 사용하는 해시 조인

이 예시는 파일 해시 방식을 사용하는 해시 조인의 수행 결과를 보여준다.
파일 해시 방식을 사용하기 위해 **max_hash_list_scan_size** 파라미터를 **8K**\(기본값: **8M**)로 설정한다.

.. code-block:: sql

    set system parameters 'max_hash_list_scan_size=8k';

    select /*+ recompile use_hash */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

질의 최적화기는 데이터베이스가 수집한 통계 정보를 바탕으로 메모리 해시와 하이브리드 해시 방식을 모두 사용할 수 없음을 판단한다.

.. code-block:: none

    임시 결과의 레코드 수 * (해시 엔트리 크기 + 위치 정보 크기) <= 해시 테이블 최대 크기
        => 215 * (32 byte * 16 byte) <= 8,192 byte
	=> 10,320 <= 8192 byte
	=> false

임시 결과의 레코드 수가 가장 적은 nation 테이블을 사용하여 해시 테이블을 생성하더라도 해시 테이블의 최대 크기를 초과한다.
따라서, 파일 해시 방식이 사용되며, 이 경우 임시 볼륨의 페이지를 사용하기 때문에 하이브리드 해시 방식보다 비용이 높다. (400 vs 3,846)

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

:ref:`예시 5 <example_join-method_hash-method_hybrid>`\의 질의 프로파일링 결과와 비교하면,
파일 해시 방식을 사용할 때, 성능이 감소했음을 확인할 수 있다.

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

다음은 동일한 예시에서 **USE_NL**과 **USE_HASH** 힌트를 함께 사용했을 때, 질의 최적화기가 어떤 조인 방식을 선택하는지 보여준다.
파일 해시 방식은 비용이 높기 때문에, 이 경우 중첩 반복 조인이 수행된다.

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
