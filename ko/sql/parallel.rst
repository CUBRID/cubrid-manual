
.. _parallel-query:

병렬 실행
==============

CUBRID는 대량의 데이터를 효율적으로 처리하기 위해 병렬 질의 실행 기능을 제공한다. 병렬 질의 실행은 하나의 질의를 여러 개의 작업 단위로 분할하고, 이를 여러 워커 스레드가 동시에 수행함으로써 응답 시간을 획기적으로 단축한다.

개요
----

병렬 질의는 다음과 같은 주요 기능을 제공한다:

*   **병렬 스캔(Parallel Scan)**: 여러 워커 스레드가 입력 데이터(힙, 임시 리스트, 인덱스)를 나누어 스캔하여 대용량 탐색의 성능을 향상시킨다. 스캔 종류에 따라 다음 세 가지로 세분화된다.

    *   **병렬 힙 스캔(Parallel Heap Scan)**: 테이블의 힙 페이지를 섹터(sector) 단위로 분할하여 워커들이 동시에 스캔한다.
    *   **병렬 리스트 스캔(Parallel List Scan)**: 부질의 등으로 생성되어 디스크에 떨어진 임시 결과 리스트(list file)를 섹터(sector) 단위로 분할하여 워커들이 동시에 스캔한다.
    *   **병렬 인덱스 스캔(Parallel Index Scan)**: B+트리 인덱스의 리프 페이지 체인을 워커들이 공유 커서로 협력하여 좌→우(또는 우→좌)로 진행하면서 스캔한다.

*   **병렬 부질의 실행(Parallel Uncorrelated Subquery Execution)**: 서로 독립적인 부질의(uncorrelated subquery)들을 워커들이 각자 맡아 동시에 처리하여 질의의 응답 속도를 개선한다.
*   **병렬 해시 조인(Parallel Hash Join)**: 빌드(build) 단계와 프로브(probe) 단계를 병렬화하여, 해시 조인 연산시 응답 속도를 개선한다.
*   **병렬 정렬(Parallel Sort)**: 정렬할 데이터를 분할하여 여러 워커 스레드를 통해 정렬한 후 병합하는 과정을 병렬로 수행하여 정렬 응답 속도를 개선한다.

설정 방법
^^^^^^^^^

병렬 질의 실행은 시스템 파라미터와 SQL 힌트를 통해 제어할 수 있다.

*   :ref:`parallelism <parallelism>` 파라미터를 2 이상으로 설정하면, 옵티마이저가 질의 수행시 병렬 질의 실행 여부를 판단할 수 있게 활성화된다.
*   **PARALLEL** ( *degree* ) 힌트를 사용하여 쿼리별로 병렬 처리 정도를 명시적으로 지정할 수 있다. *degree* 는 사용할 워커수이며, 2 이상의 정수 값이어야 한다. 힌트로 지정한 값은 parallelism 파라미터 설정보다 우선한다.
*   :ref:`max_parallel_workers <max_parallel_workers>` 파라미터는 서버 전체에서 동시에 실행 가능한 병렬 워커 스레드의 최대 개수를 설정한다(기본값: 100).
*   **NO_PARALLEL_SCAN** 힌트는 해당 쿼리 블록의 모든 병렬 스캔(힙/리스트/인덱스)을 비활성화한다. **PARALLEL** 힌트와 같이 사용하는 경우에는 **NO_PARALLEL_SCAN** 이 우선 적용된다.

.. note::

    max_parallel_workers와 parallelism 파라미터는 기본값이 각각 100과 4로 설정되어 있어 별도 설정 없이도 병렬 질의를 사용할 수 있다.

.. _parallel-scan:

병렬 스캔
---------

병렬 스캔(Parallel Scan)은 단일 스캔 입력을 여러 워커 스레드가 분할하여 동시에 처리하는 기능이다. CUBRID는 다음 세 가지 입력에 대해 병렬 스캔을 지원하며, 모두 동일한 병렬 실행 프레임워크를 공유한다.

*   **힙(Heap)**: 테이블의 힙 페이지 — **섹터** 단위로 워커들에게 사전 분할
*   **리스트(List)**: 디스크로 떨어진 임시 결과 리스트 파일 — **섹터** 단위로 워커들에게 사전 분할
*   **인덱스(Index)**: B+트리 리프 페이지 — 공유 **커서**\를 통해 워커들이 협력적으로 좌→우(또는 우→좌) 진행

각 워커 스레드는 할당된 영역을 독립적으로 스캔하면서 필터링 조건(predicate)을 평가하고, 처리한 결과는 결과 큐를 통해 메인 스레드에 전달되어 최종 결과로 통합된다.

.. note::

    실제 병렬 처리 수준은 사용자가 설정한 상한값 내에서 처리량 규칙에 의해 자동으로 최적화된다. 자세한 내용은 :ref:`parallel-query-throughput-rules`\ 를 참고한다.

공통 제약 조건
^^^^^^^^^^^^^^

다음 조건 중 하나라도 해당되면 스캔 종류와 무관하게 병렬 스캔이 적용되지 않으며, 단일 스레드 방식으로 실행된다.

*   동시성 처리를 지원하지 않는 구문이 포함된 경우

    *    저장프로시저(JavaSP, PL/CSQL), Serial 사용시
    *    세션 변수를 참조시
    *    Recursive CTE 또는 Connect By 구문 사용시
    *    CUBRID 오브젝트 DBMS 전용 기능 사용시

*   배타적 잠금(X-LOCK) 획득이 필요한 경우

    *    SELECT ... FOR UPDATE 구문
    *    incr() 함수 사용시
    *    update, delete, merge 구문

*   JOIN문에서 첫번째로 드라이빙되는 테이블이 아닌 경우
*   상관 부질의(correlated subquery)인 경우
*   소트 머지 조인의 외부/내부 입력으로 사용되는 스캔 (모든 스캔 종류)
*   **NO_PARALLEL_SCAN** 힌트가 명시된 경우

스캔 종류별로 추가 제약이 있다. 아래 각 절을 참고한다.

.. code-block:: sql

    -- 병렬 스캔이 적용되지 않는 예

    -- 힌트로 비활성화 (모든 스캔 종류)
    SELECT /*+ NO_PARALLEL_SCAN */ *
    FROM large_table;

    -- SELECT FOR UPDATE
    SELECT /*+ PARALLEL(4) */ *
    FROM large_table
    FOR UPDATE;

    -- 세션 변수 사용
    SET @user_id = 123;
    SELECT /*+ PARALLEL(4) */ *
    FROM orders
    WHERE customer_id = @user_id;

    -- SERIAL 사용
    SELECT /*+ PARALLEL(4) */ *, order_seq.NEXT_VALUE
    FROM orders;

.. _parallel-heap-scan:

병렬 힙 스캔
^^^^^^^^^^^^

병렬 힙 스캔(Parallel Heap Scan)은 테이블의 힙 페이지를 섹터(sector) 단위로 워커들에게 정적으로 분배하여 동시 스캔하는 기능이다. 특히 선택도(selectivity)가 낮은 경우(일반적으로 0.05 이하) 단일 스레드 방식의 힙 스캔보다 응답 속도가 크게 향상될 수 있다.

힙 스캔에 한정되는 추가 제약은 없다. 위에서 설명한 :ref:`공통 제약 <parallel-scan>`\ 만 적용된다.

.. code-block:: sql

    -- 병렬 힙 스캔 예
    SELECT /*+ PARALLEL(8) */ *
    FROM large_table
    WHERE status = 'active';

    -- 파티션 테이블의 병렬 힙 스캔
    SELECT /*+ PARALLEL(8) */ *
    FROM sales_partitioned
    WHERE sale_date BETWEEN '2024-01-01' AND '2024-12-31';

    -- INSERT SELECT 문 (대용량 데이터 복사)
    INSERT INTO archive_orders
    SELECT /*+ PARALLEL(8) */ *
    FROM orders
    WHERE order_date < '2023-01-01';

.. _parallel-list-scan:

병렬 리스트 스캔
^^^^^^^^^^^^^^^^

병렬 리스트 스캔(Parallel List Scan)은 부질의나 derived table 등 중간 단계에서 디스크 임시 파일(temp file)로 떨어진 결과 리스트를 섹터(sector) 단위로 워커들에게 정적으로 분배하여 동시 스캔하는 기능이다. 분할 메커니즘 자체는 힙 스캔과 동일하며, 입력이 테이블 힙 대신 임시 파일이라는 점만 다르다. 상위 연산이 큰 임시 결과를 다시 스캔해야 할 때 효과적이다.

**리스트 스캔 추가 제약**

다음 조건 중 하나라도 해당되면 병렬 리스트 스캔이 적용되지 않으며, 단일 스레드 리스트 스캔으로 실행된다.

*   임시 리스트가 메모리 버퍼에만 존재하고 디스크 임시 파일로 떨어지지 않은 경우 (분할할 섹터가 없음 — small list 자동 fallback)
*   상위 XASL이 결과를 한 행씩 받아가는 row-by-row 모드로 동작하는 경우 (mergeable list 와 BUILDVALUE 모두 적용 불가한 형태의 쿼리)
*   소트 머지 조인의 보조 입력 트리(서브쿼리/CTE 등) 안에 위치한 리스트 스캔

.. code-block:: sql

    -- 병렬 리스트 스캔이 적용되는 전형적인 패턴
    -- 내부 derived table 결과 리스트를 외부에서 다시 집계
    SELECT /*+ PARALLEL(8) */ region, COUNT(*)
    FROM (
        SELECT region, customer_id
        FROM orders o, customers c
        WHERE o.customer_id = c.id
    ) t
    GROUP BY region;

.. _parallel-index-scan:

병렬 인덱스 스캔
^^^^^^^^^^^^^^^^

병렬 인덱스 스캔(Parallel Index Scan)은 B+트리 인덱스의 리프 페이지 체인을 여러 워커가 공유 커서를 통해 협력적으로 진행하면서 읽는 기능이다. 인덱스 진입(루트→리프 수직 탐색)은 메인 스레드가 단일 스레드로 수행하며, 그 이후의 리프 순회와 OID 페치/필터 평가가 워커들에 의해 병렬로 수행된다. 워커는 리프 한 페이지를 잡으면 그 안의 키들을 독립적으로 처리하고, 다음 리프를 얻기 위해서만 짧게 동기화한다.

**인덱스 스캔 추가 제약**

다음 조건 중 하나라도 해당되면 병렬 인덱스 스캔이 적용되지 않으며, 단일 스레드 인덱스 스캔으로 실행된다.

*   리프 순서·진입 방식에 의존하는 인덱스 최적화가 적용된 경우

    *   ISS(Index Skip Scan)
    *   ILS(Index Loose Scan)
    *   KEYLIMIT 절
    *   ORDERBY_SKIP / GROUPBY_SKIP / ORDERBY_DESC / GROUPBY_DESC
    *   USE_DESC_INDEX 힌트
    *   **filtered index** 사용 (단, *function index*\는 영향을 받지 않음)
    *   MIN/MAX 단일 키 조회 (min_max scan)

*   상위 XASL이 인덱스 스캔에 ROWNUM, ANALYTIC SKIP SORT, ANALYTIC LIMIT OPT 등 row-by-row 의미를 강제하는 경우
*   상위 XASL이 결과를 한 행씩 받아가는 row-by-row 모드로 동작하는 경우 (mergeable list 와 BUILDVALUE 모두 적용 불가한 형태의 쿼리)
*   소트 머지 조인의 보조 입력 트리(서브쿼리/CTE 등) 안에 위치한 인덱스 스캔

.. code-block:: sql

    -- 병렬 인덱스 스캔이 적용되는 전형적인 예
    -- (커버링 또는 단순 범위 조건의 인덱스 풀 스캔)
    CREATE INDEX idx_orders_status ON orders(status, order_date);

    SELECT /*+ PARALLEL(8) */ order_id, order_date
    FROM orders
    WHERE status = 'completed' USING INDEX idx_orders_status;

    -- 병렬 인덱스 스캔이 적용되지 않는 예
    -- (USE_DESC_INDEX 힌트 → 단일 스레드 인덱스 스캔)
    SELECT /*+ PARALLEL(8) USE_DESC_INDEX */ *
    FROM orders
    WHERE status = 'completed';

.. note::

    병렬 인덱스 스캔은 메인 스레드의 인덱스 진입과 워커들의 리프 동기화 비용이 존재하기 때문에, 인덱스의 리프 페이지 수가 충분히 많을 때 효과가 크다. 작은 인덱스에서는 동기화 비용이 절감 효과를 상쇄할 수 있으며, 이를 막기 위해 처리량 규칙(:ref:`parallel-query-throughput-rules`)이 함께 적용된다.

성능 고려사항
^^^^^^^^^^^^^

병렬 스캔은 다음과 같은 경우에 성능 향상 효과가 크다.

*   대용량 입력(테이블/리스트/인덱스)을 스캔해야 하는 경우 (페이지 수가 많을수록 효과적)
*   힙/인덱스 스캔에서 선택도가 낮은 경우 (약 0.05 이하)
*   CPU 코어가 충분히 사용 가능한 경우
*   디스크 I/O보다 CPU 처리가 병목인 경우

반면, 다음과 같은 경우에는 오히려 성능이 저하될 수 있다.

*   소량의 입력만 스캔하는 경우
*   인덱스 스캔이 단일 스레드로 충분히 효율적인 경우(예: 짧은 범위 점 조회)
*   시스템 리소스(CPU, 메모리)가 부족한 경우

병렬 질의 사용 시에는 :ref:`max_parallel_workers <max_parallel_workers>` 파라미터를 적절히 설정하여 시스템 리소스 경쟁을 방지해야 한다. 일반적으로 실제 물리 CPU 코어 수 수준으로 설정하는 것을 권장한다.

.. _result-collection-modes:

스캔 결과 수집 모드
^^^^^^^^^^^^^^^^^^^

병렬 스캔이 활성화된 경우, 워커가 처리한 결과를 메인 스레드가 수집하는 방식은 쿼리 형태에 따라 다음 세 가지 중 하나로 결정된다. 모드는 SQL 트레이스의 **gather** 항목으로 확인할 수 있다.

*   **mergeable list**: 각 워커가 자신의 임시 결과 리스트를 만들고, 메인 스레드는 워커별 리스트를 병합 없이 그대로 출력에 사용한다. 워커 간 동기화 비용이 가장 적어 일반적으로 가장 빠르다.
*   **buildvalue**: 워커들이 부분 집계값을 계산하여 메인 스레드에 전달하면, 메인 스레드가 최종 집계값을 결합하여 반환한다. 단순 집계 질의에 특화된 모드이다(:ref:`buildvalue-optimization` 참고).
*   **row-by-row**: 메인 스레드가 한 행씩 순서대로 받아 처리한다. 다른 두 모드를 적용할 수 없는 경우에 사용된다. 적용 가능 범위가 가장 넓지만 동기화 비용이 가장 크다.

.. note::

    row-by-row 모드는 **힙 스캔에서만** 나타난다. 리스트 스캔과 인덱스 스캔은 row-by-row 모드가 필요한 쿼리 형태에서는 단일 스레드로 회귀하므로(위 추가 제약 참고), 트레이스의 ``gather: row-by-row`` 표기는 힙 스캔에서만 관찰된다.

**mergeable list 가 적용되지 않는 조건**

다음 조건 중 하나라도 해당되면 mergeable list 가 아닌 다른 모드(buildvalue 또는 row-by-row)로 처리된다.

*   대상 입력을 스캔하면서 평가할 수 없는 조건절(상위 단계로 미루어진 predicate)이 포함되는 경우
*   해시 집계(hash group by)를 수행하는 경우
*   select-list에 저장프로시저(JavaSP 또는 PL/CSQL)가 있는 경우
*   ROWNUM을 사용한 경우
*   topn_sort(상위 N개 추출을 위한 정렬)를 수행하는 경우
*   LIMIT 절이 있는 경우
*   result_cache가 활성화되어 있는 경우

.. _buildvalue-optimization:

BUILDVALUE 최적화
^^^^^^^^^^^^^^^^^

SELECT 리스트가 지원되는 집계 함수만으로 구성되고 ROWNUM 등 row 단위 의미가 없는 경우, 병렬 스캔은 **BUILDVALUE 최적화**\를 적용한다. 이 모드에서는 각 워커가 자신이 스캔한 범위의 부분 집계값을 계산하여 메인 스레드에 전달하고, 메인 스레드는 부분 집계들을 결합하여 최종값을 만든다. 워커 간 데이터 전달량이 가장 적어 단순 집계 질의에서 가장 빠른 동작 모드이다.

**지원 집계 함수**

다음 집계 함수만 사용된 SELECT 리스트에 BUILDVALUE 최적화가 적용된다.

*   **COUNT(\*)**, **COUNT(column)**, **COUNT(DISTINCT column)**
*   **MIN(column)**, **MAX(column)**
*   **SUM(column)**, **AVG(column)**
*   **STDDEV(column)**, **STDDEV_POP(column)**, **STDDEV_SAMP(column)**
*   **VARIANCE(column)**, **VAR_POP(column)**, **VAR_SAMP(column)**

**적용 조건**

위 집계 함수 사용에 더해 다음 조건을 모두 만족해야 한다.

*   SELECT 리스트가 위에 나열된 집계 함수만 포함 (집계 외 출력 컬럼이 없을 것)
*   조건절에 ROWNUM, 저장프로시저가 없을 것
*   다른 조인이나 부질의가 결합되지 않은 단순 쿼리

**적용 범위**

BUILDVALUE 최적화는 스캔 종류와 무관하게 적용 가능하다.

*   병렬 힙 스캔
*   병렬 리스트 스캔
*   병렬 인덱스 스캔

**예제**

.. code-block:: sql

    -- COUNT 계열
    SELECT /*+ PARALLEL(8) */ COUNT(*)
    FROM large_table
    WHERE status = 'active';

    SELECT /*+ PARALLEL(8) */ COUNT(DISTINCT customer_id)
    FROM orders;

    -- 산술 집계 (HEAP/LIST/INDEX 모두 가능)
    SELECT /*+ PARALLEL(8) */ SUM(amount), AVG(amount), MAX(amount)
    FROM orders
    WHERE order_date > '2024-01-01';

    -- 분산/표준편차
    SELECT /*+ PARALLEL(8) */ STDDEV(price), VARIANCE(price)
    FROM products;

    -- UPDATE STATISTICS도 내부적으로 BUILDVALUE 최적화의 혜택을 받는다
    UPDATE STATISTICS ON large_table WITH FULLSCAN;

.. note::

    SELECT 리스트에 위 집계 외의 표현식(예: 일반 컬럼, 미지원 집계 함수)이 함께 포함되거나 GROUP BY가 결합되면 BUILDVALUE 최적화는 적용되지 않으며, mergeable list 또는 row-by-row 모드로 처리된다.

스캔 SQL 트레이스
^^^^^^^^^^^^^^^^^

병렬 스캔이 수행되면 :ref:`SQL 트레이스 <query-profiling>` 결과에 병렬 처리 상세 정보가 추가로 출력된다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(4) RECOMPILE */ count(*)
    FROM large_table
    WHERE status = 'active';

::

    Trace Statistics:
        SELECT (time: 2405, fetch: 143277, fetch_time: 1287, ioread: 123467)
            SCAN (table: dba.large_table), (heap time: 2395, fetch: 143277, ioread: 123467, readrows: 0, rows: 0)
                 (parallel workers: 8, heap time: 2390..2395, readrows: 1249989..1250011,
                  rows: 1249989..1250011, gather: mergeable list)

병렬 스캔의 트레이스 출력 항목은 다음과 같다.

*   **parallel workers**: 사용된 워커 스레드의 수
*   **heap time / list time / index time**: 각 워커의 스캔 소요 시간 범위 (최소..최대, 밀리초). 스캔 종류에 따라 항목 이름이 달라진다.
*   **readrows**: 각 워커가 읽은 행 수 범위 (최소..최대)
*   **rows**: 각 워커가 반환한 행 수 범위 (최소..최대)
*   **gather**: 결과 수집 방식

    *   **mergeable list**: 워커별 리스트를 병합 없이 직접 사용
    *   **buildvalue**: 워커별 부분 집계를 결합 (구 ``count`` 표시를 대체)
    *   **row-by-row**: 한 건씩 수집하여 병합 (힙 스캔에서만 나타남)

**gather** 항목에 **mergeable list** 또는 **buildvalue** 가 표시된 경우, 동기화 비용이 적은 최적 경로로 실행되었음을 의미한다.

.. note::

    병렬 워커들의 시간과 행 수가 범위(최소..최대)로 표시되며, 이상적으로는 모든 워커가 비슷한 양의 작업을 수행해야 한다. 범위가 크게 벌어진다면 데이터 분포나 시스템 리소스 경합 문제를 의심해볼 수 있다.

**BUILDVALUE 최적화 추적 정보 예제**

BUILDVALUE 최적화가 적용되면 **gather: buildvalue** 가 표시되며, 단일 집계 결과만 반환되므로 worker별 ``rows`` 는 0으로 출력된다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(8) RECOMPILE */ COUNT(*)
    FROM large_table;

::

    Trace Statistics:
        SELECT (time: 1500, fetch: 1, fetch_time: 10, ioread: 100000)
            SCAN (table: dba.large_table), (heap time: 1490, fetch: 100000, ioread: 100000, readrows: 0, rows: 0)
                 (parallel workers: 8, heap time: 1485..1490, readrows: 1250000..1250000,
                  rows: 0..0, gather: buildvalue)

**병렬 인덱스 스캔 트레이스 예제**

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(4) RECOMPILE */ order_id, order_date
    FROM orders
    WHERE status = 'completed' USING INDEX idx_orders_status;

::

    Trace Statistics:
        SELECT (time: 980, fetch: 51200, fetch_time: 410, ioread: 0)
            SCAN (table: dba.orders, index: idx_orders_status),
                 (key time: 970, fetch: 51200, ioread: 0, readkeys: 1, filteredkeys: 0,
                  rows: 0, parallel workers: 4, key time: 965..970, rows: 312500..312500,
                  gather: mergeable list)

.. _parallel-subquery-execution:

부질의 병렬 실행
------------------------

부질의 병렬 실행(Parallel Subquery Execution)은 서로 독립적으로 실행 가능한 부질의(subquery)들을 여러 워커 스레드를 사용하여 동시에 실행함으로써 쿼리 성능을 향상시키는 기능이다.

부질의 실행 개요
^^^^^^^^^^^^^^^^

부질의는 다른 부질의와 독립적으로 실행될 수 있으므로, 여러 부질의가 있는 경우  병렬로 실행하여 전체 쿼리 응답 시간을 단축할 수 있다. 각 부질의는 독립적인 워커 스레드에서 실행되며, 모든 부질의의 실행이 완료되면 결과가 병합되어 최종 결과를 생성한다.

:ref:`parallelism <parallelism>` 파라미터가 2 이상으로 설정되어 있거나, **PARALLEL** ( *degree* ) 힌트를 사용하여 병렬 정도를 2 이상으로 지정하면 부질의의 병렬 실행이 가능하다.

**NO_PARALLEL_SUBQUERY** 힌트를 사용하면 부질의의 병렬 실행을 비활성화할 수 있다. **PARALLEL** 힌트와 같이 사용하는 경우에는 **NO_PARALLEL_SUBQUERY** 힌트가 우선된다.

실행 조건
^^^^^^^^^

다음 조건을 모두 만족할 때 부질의의 병렬 실행이 가능하다:

*   :ref:`max_parallel_workers <max_parallel_workers>` 파라미터가 2 이상으로 설정되어 있고, 사용 가능한 워커 스레드가 있는 경우
*   :ref:`parallelism <parallelism>` 파라미터가 2 이상으로 설정되어 있거나, **PARALLEL** (2) 이상의 힌트가 명시된 경우
*   부질의가 최상위 레벨 쿼리(top-level XASL)에 직접 연결되는 있는 경우
*   **NO_PARALLEL_SUBQUERY** 힌트가 사용되지 않은 경우

.. code-block:: sql

    -- parallelism 파라미터 설정 (cubrid.conf)
    -- parallelism=4

    -- 부질의 병렬 실행 예제
    SELECT *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

    -- 힌트를 사용한 예제
    SELECT /*+ PARALLEL(4) */ *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

적용되지 않는 경우
^^^^^^^^^^^^^^^^^^

다음 조건 중 하나라도 해당되면 부질의의 병렬 실행이 적용되지 않는다:

*   부질의가 최상위 레벨 쿼리에 직접 연결되지 않은 경우 (중첩된 부질의 내부의 부질의 등)
*   CTE(Common Table Expression)의 recursive 부분이 존재하거나 CTE 간 참조가 있는 경우
*   derived table(인라인 뷰) 등에 의해 부질의 간 참조가 존재하는 경우
*   Object DBMS 기능을 사용하는 경우 (path expression 등)
*   JSON_TABLE이나 SET 타입 테이블의 스캔이 포함된 경우
*   부질의 조건절에 저장 프로시저가 포함된 경우
*   상관 부질의인 경우

.. code-block:: sql

    -- 병렬 실행이 적용되지 않는 예

    -- NO_PARALLEL_SUBQUERY 힌트 사용
    SELECT /*+ NO_PARALLEL_SUBQUERY */ *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

    -- CTE 간 참조가 있는 경우
    WITH cte1 AS (
        SELECT * FROM table1
    ),
    cte2 AS (
        SELECT * FROM cte1 WHERE id > 100  -- cte1 참조
    )
    SELECT * FROM cte2;

    -- JSON_TABLE 사용
    SELECT *
    FROM orders,
    JSON_TABLE(json_column, '$[*]' COLUMNS(id INT PATH '$.id')) AS jt
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

    -- 저장 프로시저가 조건절에 포함된 경우
    SELECT *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE check_region_sp(region) = 1
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

부질의 성능 고려사항
^^^^^^^^^^^^^^^^^^^^

부질의의 병렬 실행은 다음과 같은 경우에 성능 향상 효과가 크다:

*   복수의 독립적인 부질의가 존재하는 경우
*   각 부질의의 실행 시간이 충분히 긴 경우
*   CPU 코어가 충분히 사용 가능한 경우

반면, 다음과 같은 경우에는 오히려 성능이 저하될 수 있다:

*   부질의의 실행 시간이 매우 짧은 경우 (병렬 처리 오버헤드가 더 클 수 있음)
*   부질의가 하나만 존재하는 경우
*   시스템 리소스(CPU, 메모리)가 부족한 경우
*   :ref:`max_parallel_workers <max_parallel_workers>` 설정이 부적절한 경우

관련 파라미터
^^^^^^^^^^^^^

부질의의 병렬 실행을 효과적으로 사용하려면 다음 파라미터들을 적절히 설정해야 한다:

*   :ref:`max_parallel_workers <max_parallel_workers>`: 서버 전역의 최대 병렬 워커 수
*   :ref:`parallelism <parallelism>`: 기본 병렬 처리 정도

.. code-block:: sql

    -- cubrid.conf 설정 예제
    max_parallel_workers=16
    parallelism=4

부질의 추적 정보
^^^^^^^^^^^^^^^^

부질의의 병렬 실행이 수행되면 :ref:`SQL 트레이스 <query-profiling>`\에 결과에 병렬 처리 상세 정보가 추가로 출력된다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(4) RECOMPILE */ *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

::

    Trace Statistics:
        SELECT (time: 1710, fetch: 51619, fetch_time: 5, ioread: 0)
            SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 125, rows: 125)
                SCAN (table: dba.orders), (heap time: 1677, fetch: 51500, ioread: 0, readrows: 12500000, rows: 25000)
                    SCAN (hash temp(m), build time: 0, time: 0, fetch: 0, ioread: 0, readrows: 350, rows: 17)
            SUBQUERY (uncorrelated)
                (parallel workers: 2, time: 0, fetch: 9, fetch_time: 0, ioread: 0)
                SELECT (time: 0, fetch: 5, fetch_time: 0, ioread: 0)
                    SCAN (table: dba.customers), (heap time: 0, fetch: 4, ioread: 0, readrows: 1000, rows: 333)
                    ORDERBY (time: 0, sort: true, page: 0, ioread: 0)
                SELECT (time: 0, fetch: 4, fetch_time: 0, ioread: 0)
                    SCAN (table: dba.products), (heap time: 0, fetch: 3, ioread: 0, readrows: 500, rows: 125)
                    ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

부질의 병렬 실행 SQL 트레이스 출력 항목에 대한 설명은 다음과 같다:

*   **SUBQUERY (uncorrelated)**: 부질의 실행 표시
*   **parallel workers**: 병렬 실행에 사용된 워커 스레드의 수
*   **time**: 병렬 실행에 소요된 시간 (밀리초)
*   각 부질의는 독립적인 SELECT로 표시되며, 각각의 실행 통계가 출력된다

위 예제에서는 2개의 부질의(customers 테이블 조회, products 테이블 조회)가 2개의 워커 스레드를 사용하여 병렬로 실행되었다.

.. _parallel-query-throughput-rules:

병렬 질의 처리량 규칙
---------------------

처리량 규칙 개요
^^^^^^^^^^^^^^^^

병렬 질의 실행은 질의 응답 시간을 획기적으로 단축하지만, 동시에 서버 자원(CPU, 메모리, I/O 등)을 많이 소모한다. 소수의 쿼리가 병렬 실행으로 서버 자원을 과도하게 선점하면 다른 다수의 쿼리 성능이 저하될 수 있다. 이를 방지하기 위해 CUBRID는 병렬 실행의 효과가 큰 쿼리만을 선별하기 위한 **처리량 규칙**\을 적용한다.

각 병렬 연산의 실제 병렬 처리 수준은 다음 요인에 따라 결정된다:

*   테이블/리스트/인덱스 크기 등의 처리량 규칙
*   **PARALLEL** 힌트로 명시적으로 지정된 값
*   :ref:`parallelism <parallelism>` 파라미터로 설정된 상한값
*   :ref:`max_parallel_workers <max_parallel_workers>` 파라미터로 설정된 전역 워커 풀 크기

처리량 규칙으로 계산된 병렬 처리 수준은 :ref:`parallelism <parallelism>` 파라미터 값을 초과할 수 없다. 힌트로 지정된 병렬 처리 수준은 :ref:`parallelism <parallelism>` 파라미터 값을 초과할 수 있지만 최대값(32 또는 시스템 코어 수 중 작은 값)은 초과할 수 없다.

스캔 처리량 규칙
^^^^^^^^^^^^^^^^

병렬 스캔(힙/리스트/인덱스)의 병렬 처리 수준은 스캔 대상의 페이지 수에 따라 동일한 규칙으로 결정된다. 힙 스캔은 테이블 힙 페이지 수를, 리스트 스캔은 임시 리스트 페이지 수를, 인덱스 스캔은 인덱스 리프 페이지 수를 기준으로 한다.

**활성화 조건**

*   스캔 대상의 페이지 수가 2,048개 이상일 때 활성화된다 (약 32MB, db_page_size가 16K일 때)
*   이 조건을 만족하지 않으면 **PARALLEL** 힌트가 있어도 병렬 스캔이 활성화되지 않는다

**처리 수준 결정**

병렬 처리 수준은 페이지 수에 따라 다음과 같이 결정된다.

.. csv-table::
   :header: "페이지 수", "처리량", "처리량 규칙 계산값"
   :widths: 15, 15, 15

   "2,048", "32 MB", "2"
   "4,096", "64 MB", "3"
   "8,192", "128 MB", "4"
   "16,384", "256 MB", "5"
   "32,768", "512 MB", "6"
   "65,536", "1.0 GB", "7"
   "131,072", "2.0 GB", "8"
   "262,144", "4.0 GB", "9"
   "524,288", "8.0 GB", "10"
   "1,048,576", "16.0 GB", "11"
   "2,097,152", "32.0 GB", "12"
   "4,194,304", "64.0 GB", "13"
   "8,388,608", "128.0 GB", "14"

페이지 수가 2,048개를 시작으로, 이전에 증가한 기준 페이지 수의 2배가 될 때마다 처리량 규칙으로 계산된 병렬 처리 수준이 1씩 증가한다.

**처리량 규칙에 따라 결정되는 병렬 처리 수준은** :ref:`parallelism <parallelism>` **파라미터 값을 초과할 수 없다:**

*   **MIN (처리량 규칙 계산값, parallelism 파라미터 값)**

예를 들어, parallelism=4 (기본값)로 설정된 경우:

*   페이지 수 2,048개 → 처리량 규칙 계산값 2 → MIN(2, 4) = **2** 적용

*   페이지 수 65,536개 → 처리량 규칙 계산값 7 → MIN(7, 4) = **4** 적용 (parallelism 초과 불가)

.. note::

    **PARALLEL** 힌트로 병렬 수준을 명시적으로 지정한 경우에도, 활성화 조건(페이지 수 2,048 이상)은 동일하게 적용된다. 활성화된 이후의 병렬 수준 결정에서는 힌트 값이 우선한다.

**예제**

.. code-block:: sql

    -- 테이블 생성 및 데이터 삽입
    CREATE TABLE large_table (c1 INT);

    INSERT INTO large_table
    WITH RECURSIVE cte (n) AS (
        SELECT 1
        UNION ALL
        SELECT n + 1 FROM cte WHERE n < 2000
    )
    SELECT ROWNUM FROM cte a, cte b, cte c LIMIT 2200000;

    UPDATE STATISTICS ON large_table WITH FULLSCAN;

    -- 테이블 통계 확인
    -- Total pages in class heap: 4215 (약 66MB, db_page_size가 16K일 때)
    -- Total objects: 2200000

    -- parallelism 파라미터가 4로 설정된 경우
    -- 페이지 수 4215는 2,048 이상이므로 병렬 처리 수준 3이 자동 적용됨
    SELECT COUNT(*) FROM large_table;

    -- 힌트로 명시적 지정
    SELECT /*+ PARALLEL(8) */ COUNT(*) FROM large_table;

해시 조인 처리량 규칙
^^^^^^^^^^^^^^^^^^^^^

병렬 해시 조인의 병렬 처리 수준은 처리량 규칙에 따라 결정되며, 결정된 병렬 처리 수준은 파티션 개수 이하이어야 한다.

.. note::

    병렬 해시 조인에 대한 자세한 처리량 규칙은 향후 버전에서 추가될 예정이다.

정렬 처리량 규칙
^^^^^^^^^^^^^^^^

병렬 정렬의 병렬 처리 수준은 처리량 규칙에 따라 결정되며, 결정된 병렬 처리 수준은 입력 페이지 개수 이하이어야 한다.

.. note::

    병렬 정렬에 대한 자세한 처리량 규칙은 향후 버전에서 추가될 예정이다.

부질의 처리량 규칙
^^^^^^^^^^^^^^^^^^

부질의(Subquery)의 병렬 실행은 복수의 부질의가 서로의 결과를 참조하지 않는 독립적인 구조일때 활성화된다.

*   부질의 병렬 실행시 처리 수준은 2로 고정되어 적용된다. 예를 들어, 한 쿼리 내에 독립적인 부질의가 4개가 존재하더라도, 시스템은 2개의 병렬 워커를 할당하여 처리한다.
*   각 부질의의 병렬 실행 여부는 "처리량 규칙"에 의해 결정된다.
*   여러 독립적인 부질의가 존재하는 경우 병렬 실행의 효과가 크다

.. code-block:: sql

    -- 병렬 부질의 실행 예제
    -- parallelism=4로 설정된 경우, 2개의 부질의가 병렬로 실행됨
    SELECT *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

워커 스레드 풀 관리
^^^^^^^^^^^^^^^^^^^

병렬 스레드 풀이 부족하면 일부 연산만 병렬로 수행하거나, 병렬 실행을 전혀 수행하지 못할 수 있다.

*   :ref:`max_parallel_workers <max_parallel_workers>` 파라미터로 전역 병렬 처리 워커 풀의 최대 스레드 개수를 설정한다
*   각 워커 스레드는 병렬 질의 실행 전에 필요한 병렬 워커 수를 미리 병렬 워커 풀로부터 예약하고, 작업 완료 후 반환한다
*   예약에 실패할 경우 일반적인 단일 스레드 실행 방식으로 쿼리가 수행된다
*   쿼리 전체에서 사용되는 병렬 처리 수준의 합은 :ref:`parallelism <parallelism>` 파라미터 값을 넘을 수 있으나, :ref:`max_parallel_workers <max_parallel_workers>` 값을 초과할 수는 없다

.. code-block:: sql

    -- cubrid.conf 설정 예제
    max_parallel_workers=100  # 전역 워커 풀 크기
    parallelism=4             # 단일 병렬 연산의 상한값

처리량 성능 고려사항
^^^^^^^^^^^^^^^^^^^^

병렬 질의 처리량 규칙을 통한 최적화:

*   작은 입력에 대한 불필요한 병렬 실행을 방지하여 오버헤드를 줄인다
*   입력 크기에 비례하여 병렬 처리 수준을 자동으로 조정한다
*   과도한 병렬 실행으로 인한 시스템 자원 경쟁을 방지한다
*   효과가 큰 쿼리에 집중적으로 병렬 자원을 할당한다

권장 설정:

*   **max_parallel_workers**: 동시 실행 가능한 병렬 질의 수와 각 쿼리의 평균 병렬 처리 수준을 고려하여 설정
*   **parallelism**: 시스템의 물리 코어 수를 고려하여 설정 (보통 4~8 정도가 적절)
*   대용량 테이블이 많은 환경에서는 **max_parallel_workers** 값을 높게 설정
*   소규모 테이블이 많은 환경에서는 기본값 사용을 권장
