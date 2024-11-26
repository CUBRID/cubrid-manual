.. _join-method:

조인 방법
========================================

.. 배경 설명

관계형 데이터베이스에서는 정규화를 통해 데이터를 여러 테이블로 분할하여 저장한다.
이 과정은 데이터 중복을 줄이고 무결성을 유지하는 데 효과적이지만, 분리된 데이터를 조회하려면 테이블 간의 관계를 결합하는 연산이 필요하다.
조인은 분산된 데이터를 하나의 테이블처럼 조회할 수 있도록 지원하는 DBMS의 핵심 기능이며, SQL 문법에서 중요한 역할을 한다.

.. 기본 설명

조인 질의는 두 개 이상의 테이블, 뷰, 또는 파생 테이블에서 관련된 데이터를 결합하여 원하는 정보를 추출하는 데 사용된다.
조인 결과에 포함될 레코드는 사용자가 선택한 조인 유형에 따라 달라지며, 자세한 내용은 :ref:`join-query`\에서 확인할 수 있다.

.. note::

    여기서부터 **테이블**\이라는 용어는 실제 테이블뿐만 아니라 뷰(가상 테이블)와 파생 테이블을 포함하는 포괄적인 의미로 사용된다.

.. 조인 방법

테이블 간의 레코드를 결합하는 방법은 조인 방법에 의해 결정되며, 이는 질의 최적화기가 자동으로 선택한다.

CUBRID는 다음과 같은 조인 방법을 지원한다:

- :ref:`join-method_nested-loop`
- :ref:`join-method_sort-merge`
- :ref:`join-method_hash`

하나의 질의에서 여러 조인이 수행될 수 있으며, 각 조인에 대해 서로 다른 방법이 사용될 수 있다.
모든 조인 방법은 두 개의 테이블 간 결합만 처리하며, 조인 연산은 순차적으로 진행된다. 즉, 이전 조인이 완료된 후에 다음 조인이 수행된다.

질의 최적화기는 데이터베이스가 수집한 통계 정보를 바탕으로 각 방법의 비용을 계산하고, 가장 효율적인 방법(최소 비용)을 선택한다.
그러나 CUBRID에서는 기본적으로 질의 최적화기가 **정렬 병합 조인**\과 **해시 조인**\을 고려하지 않는다.
이러한 방법을 사용하려면 **SQL 힌트**\를 명시적으로 추가해야 하며, 자세한 내용은 :ref:`sql-hint`\에서 확인할 수 있다.
**optimizer_enable_merge_join** 파라미터를 **yes**(기본값: **no**)로 설정하면, **USE_MERGE** 힌트를 사용하지 않고도 정렬 병합 조인을 수행할 수 있다.
자세한 내용은 :ref:`other-parameters`\에서 확인할 수 있다.

.. _join-method_nested-loop:

중첩 반복(Nested Loop) 조인
----------------------------------------

.. 기본 설명

중첩 반복 조인은 외부 테이블에서 레코드를 하나씩 읽고, 내부 테이블에서 해당 레코드와 결합될 수 있는 레코드를 찾아 결합하는 방법이다.
이 방법은 프로그래밍에서 사용하는 중첩 반복문과 유사하다.
외부 반복에서 사용되는 테이블은 드라이빙 테이블, 내부 반복에서 사용되는 테이블은 드리븐 테이블이라고 한다.

.. 드라이빙 테이블

두 테이블 중 어느 하나가 드리븐 테이블이 되더라도 임의 접근이 가능하다면, 드라이빙 테이블은 레코드 수가 적은 테이블이 되는 것이 더 효율적이다.
이는 임의 접근의 처리 범위가 테이블의 레코드 수에 영향을 받지 않고 일정하며, 드라이빙 테이블의 레코드 수가 적을수록 조인 연산 횟수를 줄일 수 있기 때문이다.
드라이빙 테이블은 질의 최적화기가 비용을 비교하여 최적의 테이블을 선택한다.
**ORDERED** 또는 **LEADING** 힌트를 사용하면 사용자가 직접 드라이빙 테이블을 지정할 수 있다.

.. 드리븐 테이블

중첩 반복 조인은 드리븐 테이블에 대해 임의 접근이 가능할 때 효율적이다.
드리븐 테이블을 임의 접근하기 위해서는 조인 조건에 맞는 인덱스가 미리 생성되어 있어야 한다.
그렇지 않으면 드리븐 테이블의 모든 레코드를 순차적으로 읽게 되어 성능이 저하될 수 있다.
인덱스를 사용하여 드리븐 테이블을 임의로 접근하는 경우, 실행 계획에는 중첩 반복 조인(**nl-join**\)이 인덱스 조인(**idx-join**\)으로 표시된다.

.. 부분 범위 처리

중첩 반복 조인은 테이블 레코드를 한 번에 모두 읽지 않고, 한 레코드씩 조인 연산을 수행하며 즉시 결과를 반환한다.
이 방법은 부분 범위 처리가 가능하며, 처음 N개의 결과만 조회하는 경우 조인을 조기에 종료하여 효율적인 조회를 지원한다.

.. 해시 리스트 스캔

CUBRID는 드리븐 테이블에 임의 접근이 불가능한 경우에도, 조인 조건이 모두 동등 조건이면 해시 리스트 스캔을 통해 임의 접근을 지원한다.
해시 테이블을 빌드하고 프로브하는 과정은 해시 조인과 동일하며, 자세한 내용은 :ref:`join-method_hash`\에서 확인할 수 있다.
해시 조인은 전체 범위만 처리할 수 있지만, 해시 리스트 스캔은 중첩 반복 조인과 함께 수행되어 반쪽이라도 부분 범위 처리가 가능하다는 점에서 차이가 있다.
해시 리스트 스캔이 사용되었는지는 질의 프로파일링을 통해 확인할 수 있으며, 자세한 내용은 :ref:`query-profiling`\에서 확인할 수 있다.

.. _example_join-method_nested-loop_no-index:

.. rubric:: 예시 1. 인덱스가 없는 드리븐 테이블에 대한 중첩 반복 조인

이 예시는 인덱스가 없는 드리븐 테이블에 대해 중첩 반복 조인이 수행되는 과정을 설명한다.
**ORDERED** 힌트에 의해 조인 순서가 고정되며, nation 테이블이 드라이빙 테이블, athlete 테이블이 드리븐 테이블로 사용된다.
조인 조건에 맞는 인덱스가 없기 때문에, nation 테이블의 각 레코드에 대해 athlete 테이블의 모든 레코드를 순차적으로 읽어 조인 조건을 비교하는 방법으로 수행된다.

.. code-block:: sql

    select /*+ recompile ordered use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

::

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

::

    Trace Statistics:
      SELECT (time: 92, fetch: 1720, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (table: public.athlete), (heap time: 92, fetch: 1504, ioread: 0, readrows: 313819, rows: 103)

수행 과정:

1. nation 테이블에서 필터 조건(n.continent = 'Asia')을 만족하는 레코드를 하나씩 읽는다.
2. athlete 테이블에서 필터 조건(a.event = 'Baseball')을 만족하는 레코드를 하나씩 읽는다.
3. nation 테이블의 현재 레코드와 athlete 테이블의 현재 레코드에 대해 조인 조건(n.code = a.nation_code)을 비교하고, 만족하면 두 레코드를 결합하여 결과에 포함시킨다.
4. athlete 테이블에서 필터 조건을 만족하는 다음 레코드에 대해 3단계를 반복한다.
5. nation 테이블에서 필터 조건을 만족하는 다음 레코드에 대해 2~4단계를 반복한다.
6. 모든 조인 연산이 완료될 때까지 위 과정을 반복한다.

.. _example_join-method_nested-loop_index:

.. rubric:: 예시 2. 인덱스가 있는 드리븐 테이블에 대한 중첩 반복 조인

이 예시에서는 드리븐 테이블에 임의 접근이 가능하도록 인덱스를 추가한 후, 중첩 반복 조인을 수행한다.

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

::

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

:ref:`예시 1 <example_join-method_nested-loop_no-index>`\의 질의 프로파일링 결과와 비교하면 athlete 테이블을 조회할 때 읽은 레코드 수가 크게 감소한 것을 확인할 수 있다. (readrows: 313,819 → rows: 948)

::

    Trace Statistics:
      SELECT (time: 1, fetch: 1265, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 1, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (index: public.athlete.i_athlete_nation_code), (btree time: 0, fetch: 1045, ioread: 0, readkeys: 24, filteredkeys: 0, rows: 948) (lookup time: 0, rows: 103)

.. _example_join-method_nested-loop_with-leading:

.. rubric:: 예시 3. 드라이빙 테이블의 레코드 수의 차이에 따른 성능 차이

실행 계획에 따르면, nation 테이블에서 필터 조건(n.continent = 'Asia')을 만족하는 레코드는 31개, athlete 테이블에서 필터 조건(a.event = 'Baseball')을 만족하는 레코드는 202개로 예측된다.

:ref:`예시 1 <example_join-method_nested-loop_no-index>`\과 :ref:`예시 2 <example_join-method_nested-loop_index>`\에서는 nation 테이블이 드라이빙 테이블로 사용되지만,
이번 예시에서는 **LEADING** 힌트에 의해 athlete 테이블이 드라이빙 테이블로 사용된다.
이 예시는 레코드 수가 적은 테이블과 많은 테이블이 각각 드라이빙 테이블로 사용될 때 성능 차이가 어떻게 나타나는지 보여준다.

.. code-block:: sql

    select /*+ recompile leading(a, n) use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

::

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

:ref:`예시 2 <example_join-method_nested-loop_index>`\의 질의 프로파일링 결과와 비교하면 레코드 수가 더 많은 athlete 테이블이 드라이빙 테이블로 사용되었기 때문에, 성능이 감소한 것을 확인할 수 있다.

::

    Trace Statistics:
      SELECT (time: 5, fetch: 7168, fetch_time: 0, ioread: 0)
        SCAN (table: public.athlete), (heap time: 5, fetch: 6709, ioread: 0, readrows: 6677, rows: 229)
          SCAN (index: public.nation.pk_nation_code), (btree time: 0, fetch: 458, ioread: 0, readkeys: 229, filteredkeys: 0, rows: 229) (lookup time: 0, rows: 103)

.. rubric:: 예시 4. 해시 리스트 스캔을 수행하는 드리븐 테이블에 대한 중첩 반복 조인 (부분 범위 처리)

이 예시는 해시 리스트 스캔이 수행되는 드리븐 테이블에 대해 중첩 반복 조인이 수행되는 과정을 설명한다.
해시 리스트 스캔은 뷰 또는 파생 테이블을 대상으로 수행되므로, 파생 테이블에 대한 뷰 머징 최적화가 수행되지 않도록 **NO_MERGE** 힌트를 사용한다.
처음 20개의 결과만 조회하기 때문에 부분 범위 처리로 수행된다.

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

::

    Query plan:

    sscan
        class: a node[0]
        sargs: term[0]
        cost:  49 card 202

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

해시 테이블을 생성하기 위해서 athlete 테이블에서 필터 조건(a.event = 'Baseball')을 만족하는 모든 레코드를 읽은 것을 확인할 수 있다.
이후 nation 테이블의 각 레코드에 대해 athlete 테이블에서는 해시 테이블을 통해 임의 접근을 수행하며, 처음 20개의 결과만 조회된다.

::

    Trace Statistics:
      SELECT (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 26, rows: 8)
          SCAN (hash temp(m), build time: 0, time: 0, fetch: 0, ioread: 0, readrows: 249, rows: 20)
        SUBQUERY (uncorrelated)
          SELECT (time: 2, fetch: 32, fetch_time: 0, ioread: 0)
            SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 229)

.. _join-method_sort-merge:

정렬 병합(Sort Merge) 조인
----------------------------------------

.. 기본 설명

정렬 병합 조인은 두 테이블의 레코드를 조인 조건에 따라 정렬한 후, 정렬된 레코드를 순차적으로 비교하며 결합하는 방법이다.
정렬 병합 조인은 조인 조건이 동등 조건일 때만 수행할 수 있다.

.. 정렬 과정

조인 전에 두 테이블의 레코드를 정렬하는 과정은 실시간으로 인덱스를 생성하는 것과 유사하며, 정렬된 레코드를 순차적으로 결합하는 과정은 중첩 반복 조인의 동작 방법과 유사하다.
조인 컬럼과 동일한 정렬 순서를 가진 인덱스가 이미 존재하더라도 정렬과정을 생략하지 않는다.
정렬 과정에서 사용되는 버퍼의 크기는 **sort_buffer_size** 파라미터로 설정되며, 자세한 내용은 :ref:`memory-parameters`\에서 확인할 수 있다.

정렬 병합 조인은 데이터 정렬 과정에서 비용이 많이 발생할 경우 비효율적일 수 있다.
반면, 중첩 반복 조인은 별도의 선행 작업이 없고, 해시 조인은 한쪽 테이블의 레코드만 모두 읽어서 해시 테이블을 생성하면 되므로, 정렬 병합 조인보다 낮은 비용으로 평가된다.

.. 부분 범위 처리

정렬 병합 조인은 두 테이블의 정렬이 모두 완료된 상태에서 조인을 수행하기 때문에, 부분 범위 처리가 불가능하다.
따라서 처음 N개의 결과만 필요한 경우에도, 조인이 모두 완료된 후에 N개의 결과를 조회할 수 있다.

.. _example_join-method_sort-merge:

.. rubric:: 예시 1. 정렬 병합 조인의 수행 과정

이 예시는 정렬 병합 조인의 수행 과정을 설명한다.
두 테이블 중 어느 테이블이 먼저 조회되는지는 성능에 영향을 주지 않지만, 설명과 동일하게 수행되도록 하기 위해 **ORDERED** 힌트를 사용한다.

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

::

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

::

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

수행 과정:

1. nation 테이블에서 필터 조건(n.continent = 'Asia')을 만족하는 레코드만 조회하여 code 컬럼 값을 기준으로 정렬한다.

    .. code-block:: sql

        select n.code, n.name from nation as n where n.continent = 'Asia' order by n.code;

2. athlete 테이블에서 필터 조건(a.event = 'Baseball')을 만족하는 레코드만 조회하여 natione_code 컬럼 값을 기준으로 정렬한다.

    .. code-block:: sql

        select a.nation_code, a.name from athlete as a where a.event = 'Baseball' order by a.nation_code;

3. 두 조회 결과의 레코드를 순차적으로 읽어서 조인 조건(n.code = a.nation_code)을 비교하고, 만족하면 두 레코드를 결합하여 결과에 포함시킨다.

.. _join-method_hash:

해시(Hash) 조인
----------------------------------------

.. 기본 설명

해시 조인은 해시 함수를 사용하여 동일한 해시 값을 가지는 두 테이블의 레코드를 결합하는 방법이다.
이 과정에서 해시 테이블은 동일한 해시 값을 가지는 레코드를 모아 효율적으로 비교하기 위해 사용된다.
두 테이블 중 하나의 데이터는 해시 테이블을 빌드하기 위해 사용되고, 다른 테이블의 데이터는 해시 테이블을 탐색하기 위해 사용된다.
해시 값은 조인 컬럼 값을 입력으로 해시 함수에서 생성된다.
서로 다른 입력값에서 동일한 해시 값이 생성되는 상황을 해시 충돌이라고 하며, 충돌이 발생한 데이터는 순차적으로 탐색하여 조인 조건을 비교한다.
해시 조인은 조인 조건이 동등 조건일 때만 수행할 수 있다.

.. 비용 계산

해시 조인의 비용은 해시 테이블 빌드에 사용되는 테이블과 해시 테이블 탐색을 수행하는 테이블을 각각 다른 테이블로 설정했을 때의 비용을 모두 계산하여, 최소 비용을 선택한다.

.. 빌드 테이블

해시 테이블 빌드에 사용되는 테이블은  **ORDERED** 또는 **LEADING** 힌트를 사용하여 지정할 수 없다.
일반적으로 두 테이블 중 레코드 수가 적은 테이블이 해시 테이블 빌드에 사용된다.
단, 외부 조인의 경우 조인 방향에 따라 해시 테이블 빌드에 사용되는 테이블이 고정된다.
왼쪽 외부 조인에서는 오른쪽 테이블이, 오른쪽 외부 조인에서는 왼쪽 테이블이 해시 테이블 빌드에 사용된다.

.. 해시 방법

해시 방법은 임시 결과 크기와 해시 테이블의 최대 크기에 따라 결정된다.

CUBRID는 다음과 같은 해시 방법을 지원한다:

- 메모리 해시: 임시 결과 전체를 해시 테이블에 저장할 수 있을 때 사용한다. **(임시 결과의 페이지 수 * 페이지 크기 <= 해시 테이블 최대 크기)**
- 하이브리드 해시: **메모리 해시**\가 불가능하고, 해시 테이블에 임시 결과 크기 만큼의 레코드 위치 정보를 저장할 수 있을 때 사용한다. **(임시 결과의 레코드 수 * (해시 엔트리 크기 + 위치 정보 크기) <= 해시 테이블 최대 크기)**
- 파일 해시: **메모리 해시**\와 **하이브리드 해시**\가 모두 불가능할 때 사용한다.

해시 테이블의 최대 크기는 **max_hash_list_scan_size** 파라미터로 설정할 수 있으며, 자세한 내용은 :ref:`memory-parameters`\에서 확인할 수 있다.

**하이브리드 해시**\와 **파일 해시**\는 해시 테이블에 임시 결과에 대한 레코드의 위치 정보가 저장되므로, 임시 결과의 레코드를 찾아가는 과정이 추가로 필요하다.
**메모리 해시**\와 **하이브리드 해시**\는 메모리에 해시 테이블을 생성하지만, **파일 해시**\는 임시 볼륨의 페이지를 사용하여 해시 테이블을 생성한다.
임시 볼륨의 페이지는 데이터 버퍼를 통해 접근할 있으므로 래치가 발생하여 성능 저하가 발생할 수 있다.
실제로 어떤 해시 방법이 사용되었는지는 질의 프로파일링을 통해 확인할 수 있으며, 자세한 내용은 :ref:`query-profiling`\에서 확인할 수 있다.

.. 부분 범위 처리

해시 조인은 해시 테이블 빌드가 완료된 상태에서 조인을 수행하기 때문에, 부분 범위 처리가 불가능하다.

.. _example_join-method_hash:

.. rubric:: 예시 1. 해시 조인의 수행 과정

이 예시는 해시 조인의 수행 과정을 설명한다.

**ORDERED** 힌트에 의해 조인 순서가 고정되었지만, 해시 테이블 빌드에 사용되는 테이블은 고정되지 않는다.

.. code-block:: sql

    select /*+ recompile ordered use_hash */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

::

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

nation 테이블에서 필터 조건(n.continent = 'Asia')을 만족하는 레코드는 47개이고,
athlete 테이블에서 필터 조건(a.event = 'Baseball')을 만족하는 레코드는 229개이기 때문에
레코드 수가 적은 nation 테이블이 해시 테이블 빌드에 사용된다.

::

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

수행 과정:

1. nation 테이블에서 필터 조건(n.continent = 'Asia')을 만족하는 레코드를 조회하여 임시 결과를 만든다.

    .. code-block:: sql

        select n.code, n.name from nation as n where n.continent = 'Asia';

2. athlete 테이블에서 필터 조건(a.event = 'Baseball')을 만족하는 레코드를 조회하여 임시 결과를 만든다.

    .. code-block:: sql

        select a.nation_code, a.name from athlete as a where a.event = 'Baseball';

3. nation 테이블의 임시 결과의 모든 레코드를 읽어서 해시 테이블을 빌드한다.
4. athlete 테이블의 임시 결과에서 레코드를 하나씩 읽어서 해시 테이블을 탐색한다.
5. 해시 값이 동일한 레코드를 읽어서 조인 조건(n.code = a.nation_code)을 비교하고, 만족하면 두 레코드를 결합하여 결과에 포함시킨다.
6. 해시 값이 동일한 다음 레코드에 대해 5단계를 반복한다.
7. athlete 테이블의 임시 결과의 다음 레코드에 대해 4~6단계를 반복한다.
8. 모든 조인 연산이 완료될 때까지 위 과정을 반복한다.

.. _example_join-method_hash_index:

.. rubric:: 예시 2. 필터 조건이 먼저 적용되는 해시 조인

이 예시는 중첩 반복 조인과 해시 조인 수행을 수행할 때 인덱스 설계가 다를 수 있음을 보여준다.

중첩 반복 조인에서는 드리븐 테이블을 임의 접근하기 위해 조인 조건에 맞는 인덱스가 필요하지만,
해시 조인에서는 조인 조건을 제외하고 필터 조건만 적용된 임시 결과를 먼저 조회하기 때문에 중첩 반복 조인과는 다른 인덱스가 필요하다.
두 가지 조인 방법을 모두 고려하여 인덱스를 설계하면 좋겠지만, 그렇지 않은 경우 **INDEX_SS** 힌트를 사용하여 인덱스 스킵 스캔의 수행을 고려해볼 수 있다.
인덱스 스킵 스캔은 인덱스의 선행 컬럼만 스킵할 수 있기 때문에 조인 조건의 컬럼이 여러 개인 경우에는 수행이 불가능하다.

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

::

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

::

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

.. rubric:: 예시 3. 오른쪽 해시 외부 조인

이 예시는 외부 조인에서 조인 방향에 따라 해시 테이블 빌드에 사용되는 테이블이 고정되는 것을 보여준다.
오른쪽 외부 조인에서는 왼쪽 테이블이 해시 테이블 빌드에 사용된다.

.. code-block:: sql

    select /*+ recompile use_hash */
        n.name, p.host_year, p.gold
    from
        participant as p
        right outer join nation as n on n.code = p.nation_code and p.gold > 0
    where
        n.continent = 'Asia';

::

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
nation 테이블에서 필터 조건(n.continent = 'Asia')을 만족하는 레코드는 47개이지만,
오른쪽 외부 조인에서는 왼쪽 테이블이 해시 테이블 빌드에 사용되기 때문에 participant 테이블이 해시 테이블 빌드에 사용된다.

::

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

.. rubric:: 예시 4. 해시 테이블 빌드가 생략된 해시 외부 조인

이 예시는 외부 조인에서 해시 테이블 빌드가 생략되는 경우를 보여준다.
왼쪽 외부 조인에서는 오른쪽 테이블이 해시 테이블 빌드에 사용된다.

두 테이블 중 하나라도 데이터가 없으면 해시 테이블은 빌드되지 않는다.
participant 테이블에는 필터 조건(p.gold > 100)을 만족하는 레코드가 존재하기 때문에 해시 테이블 빌드를 생략한다.
필터 조건(n.continent = 'Asia')이 적용된 왼쪽 테이블의 결과가 질의 결과로 조회된다.

.. code-block:: sql

    select /*+ recompile use_hash */
        n.name, p.host_year, p.gold
    from
        nation as n
        left outer join participant as p on n.code = p.nation_code and p.gold > 100
    where
        n.continent = 'Asia';

::

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

**BUILD** 항목에서 해시 테이블 빌드에 사용된 해시 방법이 hash_method: skip으로 표시되어 있다.
**skip**\은 해시 테이블 빌드가 생략되었다는 것을 의미한다.

::

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

.. rubric:: 예시 5. 하이브리드 해시 방법을 사용하는 해시 조인

이 예시는 하이브리드 해시 방법을 사용하는 해시 조인의 수행 결과를 보여준다.

하이브리드 해시 방법을 사용하기 위해 **max_hash_list_scan_size** 파라미터를 **12K**\(기본값: **8M**)로 설정한다.

.. code-block:: sql

    set system parameters 'max_hash_list_scan_size=12k';

    select /*+ recompile use_hash */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

::

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

::

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

같은 예시에서 **USE_NL**\과 **USE_HASH** 힌트를 함께 사용했을 때 질의 최적화기가 어떤 조인 방법을 선택하는지 보여준다.
여기서는 동일하게 해시 조인이 수행된다.

.. code-block:: sql

    set system parameters 'max_hash_list_scan_size=12k';

    select /*+ recompile use_hash use_nl */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

::

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

::

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

.. rubric:: 예시 6. 파일 해시 방법을 사용하는 해시 조인

이 예시는 파일 해시 방법을 사용하는 해시 조인의 수행 결과를 보여준다.
파일 해시 방법을 사용하기 위해 **max_hash_list_scan_size** 파라미터를 **8K**\(기본값: **8M**)로 설정한다.

.. code-block:: sql

    set system parameters 'max_hash_list_scan_size=8k';

    select /*+ recompile use_hash */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

질의 최적화기는 데이터베이스가 수집한 통계 정보를 바탕으로 메모리 해시와 하이브리드 해시 방법 모두 사용이 불가능하다는 것을 확인할 수 있다.

::

    임시 결과의 레코드 수 * (해시 엔트리 크기 + 위치 정보 크기) <= 해시 테이블 최대 크기
        => 215 * (32 byte * 16 byte) <= 8,192 byte
	=> 10,320 <= 8192 byte
	=> false

임시 결과의 레코드 수가 가장 적은 nation 테이블을 사용하여 해시 테이블을 빌드하더라도 해시 테이블의 최대 크기를 초과한다.
파일 해시 방법을 사용하는 경우에는 임시 볼륨의 페이지를 사용하기 때문에 하이브리드 해시 방법을 사용할 때보다 계산된 비용이 높다. (400 vs 3,846)

::

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

:ref:`예시 5 <example_join-method_hash-method_hybrid>`\의 질의 프로파일링 결과와 비교하면 파일 해시 방법을 사용할 때 성능이 감소한 것을 확인할 수 있다.

::

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

같은 예시에서 **USE_NL**과 **USE_HASH** 힌트를 함께 사용했을 때, 질의 최적화기가 어떤 조인 방법을 선택하는지 보여준다.
파일 해시 방법을 사용해야 하는 경우에는 비용이 높기 때문에 중첩 반복 조인이 수행된다.

.. code-block:: sql

    set system parameters 'max_hash_list_scan_size=8k';

    select /*+ recompile use_hash use_nl */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

::

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

::

    Trace Statistics:
      SELECT (time: 19, fetch: 13387, fetch_time: 1, ioread: 0)
        SCAN (table: public.athlete), (heap time: 4, fetch: 6709, ioread: 0, readrows: 6677, rows: 6677)
          SCAN (index: public.nation.pk_nation_code), (btree time: 10, fetch: 6677, ioread: 0, readkeys: 6677, filteredkeys: 0, rows: 0, covered: true, count_only: true)
