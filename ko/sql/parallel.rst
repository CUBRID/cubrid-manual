
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
*   **PARALLEL** ( *degree* ) 힌트를 사용하여 질의별로 병렬 처리 수준을 명시적으로 지정할 수 있다. *degree* 는 사용할 워커 수이며 0 이상의 정수이다. **0** 또는 **1**\ 로 지정하면 병렬 실행이 비활성화되고, **2 이상**\ 으로 지정하면 힌트 값이 parallelism 파라미터 설정보다 우선한다. 시스템의 CPU 코어 수를 초과하는 값은 코어 수로 낮추어 적용된다.
*   :ref:`max_parallel_workers <max_parallel_workers>` 파라미터는 서버 전체에서 동시에 실행 가능한 병렬 워커 스레드의 최대 개수를 설정한다(기본값: 100).
*   **NO_PARALLEL_SCAN** 힌트는 해당 질의 블록의 모든 병렬 스캔(힙/리스트/인덱스)을 비활성화한다. **PARALLEL** 힌트와 같이 사용하는 경우에는 **NO_PARALLEL_SCAN** 이 우선 적용된다.
*   병렬 해시 조인과 부질의 병렬 실행은 각각 **NO_PARALLEL_HASH_JOIN**, **NO_PARALLEL_SUBQUERY** 힌트로 비활성화할 수 있다. 자세한 내용은 각 절을 참고한다.

.. note::

    max_parallel_workers와 parallelism 파라미터는 기본값이 각각 100과 4로 설정되어 있어 별도 설정 없이도 병렬 질의를 사용할 수 있다. 단, 시스템의 CPU 코어 수가 2 이하인 경우에는 병렬 질의 실행이 전면 비활성화된다.

.. note::

    병렬 실행에서는 워커들의 처리 완료 순서에 따라 결과 행의 순서가 실행할 때마다 달라질 수 있다. 결과 순서가 중요한 경우에는 ORDER BY 절을 명시해야 한다.

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
*   상관 부질의(correlated subquery) 내부의 스캔인 경우. 비상관(uncorrelated) 부질의 내부의 스캔은 병렬 실행이 가능하다.
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
    WHERE id <= 10
    FOR UPDATE;

    -- 세션 변수 사용
    SET @user_id = 123;
    SELECT /*+ PARALLEL(4) */ *
    FROM orders
    WHERE cust_id = @user_id;

    -- SERIAL 사용
    SELECT /*+ PARALLEL(4) */ id, order_seq.NEXT_VALUE
    FROM large_table
    WHERE id <= 3;

.. _parallel-heap-scan:

병렬 힙 스캔
^^^^^^^^^^^^

병렬 힙 스캔(Parallel Heap Scan)은 테이블의 힙 페이지를 섹터(sector) 단위로 워커들에게 정적으로 분배하여 동시 스캔하는 기능이다. 특히 선택도(selectivity)가 낮은 경우(일반적으로 0.05 이하) 단일 스레드 방식의 힙 스캔보다 응답 속도가 크게 향상될 수 있다.

힙 스캔에 한정되는 추가 제약은 없다. 위에서 설명한 :ref:`공통 제약 <parallel-scan>`\ 만 적용된다.

.. code-block:: sql

    -- 병렬 힙 스캔 예
    SELECT /*+ PARALLEL(4) */ id, category
    FROM large_table
    WHERE status = 'active';

    -- 파티션 테이블의 병렬 힙 스캔
    SELECT /*+ PARALLEL(4) */ *
    FROM sales_partitioned
    WHERE sale_date BETWEEN '2024-01-01' AND '2024-12-31';

    -- INSERT SELECT 문 (대용량 데이터 복사)
    INSERT INTO archive_orders
    SELECT /*+ PARALLEL(4) */ *
    FROM orders
    WHERE amount < 100;

.. note::

    파티션 테이블의 스캔은 파티션별로 병렬화가 결정되며, 활성화 조건(:ref:`parallel-query-throughput-rules`)도 개별 파티션의 페이지 수로 평가된다. 페이지 수가 활성화 조건에 미달하는 파티션은 단일 스레드로 스캔된다.

.. _parallel-list-scan:

병렬 리스트 스캔
^^^^^^^^^^^^^^^^

병렬 리스트 스캔(Parallel List Scan)은 부질의나 derived table 등 중간 단계에서 디스크 임시 파일(temp file)로 떨어진 결과 리스트를 섹터(sector) 단위로 워커들에게 정적으로 분배하여 동시 스캔하는 기능이다. 분할 메커니즘 자체는 힙 스캔과 동일하며, 입력이 테이블 힙 대신 임시 파일이라는 점만 다르다. 상위 연산이 큰 임시 결과를 다시 스캔해야 할 때 효과적이다.

**리스트 스캔 추가 제약**

다음 조건 중 하나라도 해당되면 병렬 리스트 스캔이 적용되지 않으며, 단일 스레드 리스트 스캔으로 실행된다.

*   임시 리스트가 메모리 버퍼에만 존재하고 디스크 임시 파일로 떨어지지 않은 경우 (분할할 섹터가 없어 작은 리스트는 자동으로 단일 스레드로 실행)
*   상위 연산이 결과를 한 행씩 받아가는 row-by-row 모드로 동작하는 경우 (mergeable list 와 BUILDVALUE 모두 적용 불가한 형태의 질의)
*   ROWNUM 또는 LIMIT 조건절이 리스트 스캔에 걸려 있는 경우
*   소트 머지 조인의 보조 입력 트리(부질의/CTE 등) 안에 위치한 리스트 스캔

.. code-block:: sql

    -- 병렬 리스트 스캔이 적용되는 전형적인 패턴
    -- DISTINCT로 실체화된 derived table 결과 리스트를 외부에서 다시 집계
    SELECT /*+ PARALLEL(4) */ COUNT(*)
    FROM (SELECT DISTINCT id, pad FROM large_table) t;

.. note::

    단순 프로젝션만 있는 derived table은 옵티마이저가 상위 질의로 병합(flatten)하므로 임시 리스트 자체가 만들어지지 않는다. 리스트 스캔은 DISTINCT, GROUP BY, UNION 등으로 실체화(materialization)가 강제된 임시 결과에 대해 나타난다. 해시 조인의 입력 리스트를 상위에서 다시 스캔하는 경우도 대표적인 병렬 리스트 스캔 대상이다.

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
    *   ORDERBY_SKIP / GROUPBY_SKIP 계열 최적화 (ORDER BY / GROUP BY를 인덱스 순서로 대체하는 최적화)
    *   MIN/MAX 단일 키 조회 (min_max scan)

*   ROWNUM 조건을 워커별로 다시 계산할 수 없는 형태로 사용한 경우, 또는 분석 함수(analytic function)의 SKIP SORT/LIMIT 최적화가 적용된 경우
*   상위 연산이 결과를 한 행씩 받아가는 row-by-row 모드로 동작하는 경우 (mergeable list 와 BUILDVALUE 모두 적용 불가한 형태의 질의)
*   소트 머지 조인의 보조 입력 트리(부질의/CTE 등) 안에 위치한 인덱스 스캔

.. code-block:: sql

    -- 병렬 인덱스 스캔이 적용되는 전형적인 예
    -- (넓은 범위 조건의 커버링 인덱스 스캔)
    CREATE INDEX idx_large_id_pad ON large_table(id, pad);

    SELECT /*+ PARALLEL(4) */ COUNT(pad)
    FROM large_table
    WHERE id BETWEEN 1 AND 900000 USING INDEX idx_large_id_pad;

    -- 병렬 인덱스 스캔이 적용되지 않는 예
    -- (KEYLIMIT 절 → 단일 스레드 인덱스 스캔)
    SELECT /*+ PARALLEL(4) */ id, category
    FROM large_table
    WHERE id BETWEEN 1 AND 900000 USING INDEX idx_large_id_pad KEYLIMIT 100;

.. note::

    병렬 인덱스 스캔은 메인 스레드의 인덱스 진입과 워커들의 리프 동기화 비용이 존재하기 때문에, 인덱스의 리프 페이지 수가 충분히 많을 때 효과가 크다. 작은 인덱스에서는 동기화 비용이 절감 효과를 상쇄할 수 있으며, 이를 막기 위해 처리량 규칙(:ref:`parallel-query-throughput-rules`)이 함께 적용된다.

.. note::

    **PARALLEL** 힌트 없이 인덱스 스캔이 자동으로 병렬화되려면 옵티마이저 단계에서 다음 두 조건을 추가로 만족해야 한다.

    *   모든 키 범위(key range) 조건의 선택도가 **히스토그램 통계**\로부터 산출될 수 있어야 한다. 대상 테이블의 히스토그램 통계가 없으면 옵티마이저는 병렬 인덱스 스캔을 선택하지 않는다. 통계는 **UPDATE STATISTICS** 문으로 갱신할 수 있다.
    *   선택도와 인덱스 페이지 수의 곱이 일정 기준(기본 32페이지) 이상이어야 한다.

    **PARALLEL** 힌트를 지정하면 위 옵티마이저 조건은 우회하지만, 서버가 실측한 인덱스 페이지 수가 활성화 조건(:ref:`parallel-query-throughput-rules`)에 미달하면 단일 스레드로 실행된다.

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

병렬 스캔이 활성화된 경우, 워커가 처리한 결과를 메인 스레드가 수집하는 방식은 질의 형태에 따라 다음 세 가지 중 하나로 결정된다. 모드는 SQL 트레이스의 **gather** 항목으로 확인할 수 있다.

*   **mergeable list**: 각 워커가 자신의 임시 결과 리스트를 만들고, 메인 스레드는 워커별 리스트를 병합 없이 그대로 출력에 사용한다. 워커 간 동기화 비용이 가장 적어 일반적으로 가장 빠르다.
*   **buildvalue**: 워커들이 부분 집계값을 계산하여 메인 스레드에 전달하면, 메인 스레드가 최종 집계값을 결합하여 반환한다. 단순 집계 질의에 특화된 모드이다(:ref:`buildvalue-optimization` 참고).
*   **row-by-row**: 메인 스레드가 한 행씩 순서대로 받아 처리한다. 다른 두 모드를 적용할 수 없는 경우에 사용된다. 적용 가능 범위가 가장 넓지만 동기화 비용이 가장 크다.

.. note::

    row-by-row 모드는 **힙 스캔에서만** 나타난다. 리스트 스캔과 인덱스 스캔은 row-by-row 모드가 필요한 질의 형태에서는 단일 스레드로 회귀하므로(위 추가 제약 참고), 트레이스의 ``gather: row by row`` 표기는 힙 스캔에서만 관찰된다.

**mergeable list 가 적용되지 않는 조건**

다음 조건 중 하나라도 해당되면 mergeable list 가 아닌 다른 모드(buildvalue 또는 row-by-row)로 처리된다.

*   대상 입력을 스캔하면서 평가할 수 없는 조건절(상위 단계로 미루어진 predicate)이 포함되는 경우
*   select-list에 저장프로시저(JavaSP 또는 PL/CSQL)가 있는 경우
*   ROWNUM 또는 LIMIT 조건을 워커별로 다시 계산할 수 없는 형태로 사용한 경우
*   스캔이 테이블의 컬럼을 직접 출력하지 않는 경우(상수만 출력하는 질의 등)
*   result_cache가 활성화되어 있는 경우

.. note::

    ORDER BY와 LIMIT을 함께 사용하는 상위 N개 추출 질의도 mergeable list 로 병렬 실행될 수 있다. 이때는 각 워커가 스캔 중에 자신의 상위 N개를 유지하며, 트레이스의 병렬 처리 상세 정보에 **topnsort: true** 가 표시된다. GROUP BY가 결합된 질의에서는 워커들이 부분 해시 집계를 수행하며, 트레이스의 **GROUPBY** 항목에 **hash: partial** 이 표시된다.

.. _buildvalue-optimization:

BUILDVALUE 최적화
^^^^^^^^^^^^^^^^^

GROUP BY 없이 집계 함수를 계산하는 질의에서 사용된 집계 함수가 모두 지원 목록에 포함되면, 병렬 스캔은 **BUILDVALUE 최적화**\를 적용한다. 이 모드에서는 각 워커가 자신이 스캔한 범위의 부분 집계값을 계산하여 메인 스레드에 전달하고, 메인 스레드는 부분 집계들을 결합하여 최종값을 만든다. 워커 간 데이터 전달량이 가장 적어 집계 질의에서 가장 빠른 동작 모드이다.

**지원 집계 함수**

다음 집계 함수를 사용한 질의에 BUILDVALUE 최적화가 적용된다. SELECT 리스트에서 집계 함수들을 산술식 등으로 조합하는 것도 가능하다.

*   **COUNT(\*)**, **COUNT(column)**, **COUNT(DISTINCT column)**
*   **MIN**, **MAX**
*   **SUM**, **AVG**
*   **STDDEV**, **STDDEV_POP**, **STDDEV_SAMP**
*   **VARIANCE**, **VAR_POP**, **VAR_SAMP**
*   **BIT_AND**, **BIT_OR**, **BIT_XOR**
*   **GROUP_CONCAT**, **JSON_ARRAYAGG**, **JSON_OBJECTAGG**
*   **MEDIAN**, **PERCENTILE_CONT**, **PERCENTILE_DISC**
*   **CUME_DIST**, **PERCENT_RANK**

**적용 조건**

위 집계 함수 사용에 더해 다음 조건을 모두 만족해야 한다.

*   GROUP BY 절이 없을 것
*   ROWNUM을 사용하지 않을 것

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

    미지원 집계 함수가 하나라도 포함되거나 GROUP BY가 결합되면 BUILDVALUE 최적화는 적용되지 않으며, mergeable list 또는 row-by-row 모드로 처리된다.

스캔 SQL 트레이스
^^^^^^^^^^^^^^^^^

병렬 스캔이 수행되면 :ref:`SQL 트레이스 <query-profiling>` 결과에 병렬 처리 상세 정보가 추가로 출력된다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(4) RECOMPILE */ id, category
    FROM large_table
    WHERE status = 'active';

::

    Trace Statistics:
      SELECT (time: 110, fetch: 19717, fetch_time: 15, ioread: 2101)
        SCAN (table: dba.large_table), (heap time: 110, fetch: 19714, ioread: 2100, readrows: 1000000, rows: 1000000)
             (parallel workers: 4, heap time: 108..110, readrows: 248704..252416, rows: 248704..252416, gather: mergeable list)

병렬 스캔의 트레이스 출력 항목은 다음과 같다.

*   **parallel workers**: 사용된 워커 스레드의 수
*   **heap time / temp time / index time**: 각 워커의 스캔 소요 시간 범위 (최소..최대, 밀리초). 항목 이름이 스캔 종류(힙/리스트/인덱스)에 따라 달라진다.
*   **readrows**: 각 워커가 읽은 행 수 범위 (최소..최대)
*   **rows**: 각 워커가 반환한 행 수 범위 (최소..최대)
*   **gather**: 결과 수집 방식

    *   **mergeable list**: 워커별 리스트를 병합 없이 직접 사용
    *   **buildvalue**: 워커별 부분 집계를 결합 (구 ``count`` 표시를 대체)
    *   **row by row**: 한 건씩 수집하여 병합 (힙 스캔에서만 나타남)

**gather** 항목에 **mergeable list** 또는 **buildvalue** 가 표시된 경우, 동기화 비용이 적은 최적 경로로 실행되었음을 의미한다.

.. note::

    병렬 워커들의 시간과 행 수가 범위(최소..최대)로 표시되며, 이상적으로는 모든 워커가 비슷한 양의 작업을 수행해야 한다. 범위가 크게 벌어진다면 데이터 분포나 시스템 리소스 경합 문제를 의심해볼 수 있다.

**BUILDVALUE 최적화 추적 정보 예제**

BUILDVALUE 최적화가 적용되면 **gather: buildvalue** 가 표시된다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(4) RECOMPILE */ COUNT(*)
    FROM large_table
    WHERE status = 'active';

::

    Trace Statistics:
      SELECT (time: 86, fetch: 17249, fetch_time: 93, ioread: 13377)
        SCAN (table: dba.large_table), (heap time: 86, fetch: 17246, ioread: 13377, readrows: 1000000, rows: 1000000)
             (parallel workers: 4, heap time: 81..86, readrows: 248704..252416, rows: 248704..252416, gather: buildvalue)

**병렬 인덱스 스캔 트레이스 예제**

병렬 인덱스 스캔의 병렬 처리 상세 정보에는 **index time** 과 함께 워커별 **readkeys**\ (읽은 키 수)와 **filteredkeys**\ (키 필터 통과 수) 범위가 출력된다. 커버링 인덱스 스캔이면 **covered: true** 가 표시된다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(4) RECOMPILE */ COUNT(pad)
    FROM large_table
    WHERE id BETWEEN 1 AND 900000 USING INDEX idx_large_id_pad;

::

    Trace Statistics:
      SELECT (time: 136, fetch: 13247, fetch_time: 77, ioread: 13239)
        SCAN (index: dba.large_table.idx_large_id_pad), (btree time: 136, fetch: 13244, ioread: 13239, readkeys: 900003, filteredkeys: 900000, rows: 900000, covered: true)
             (parallel workers: 4, index time: 136..136, readkeys: 224557..225285, filteredkeys: 224556..225284, rows: 224556..225284, covered: true, gather: buildvalue)

커버링이 아닌 인덱스 스캔에서는 병렬 처리 상세 정보 뒤에 데이터 페이지 조회 통계인 **(lookup time: 최소..최대, rows: 최소..최대)** 가 덧붙는다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(4) RECOMPILE */ category
    FROM large_table
    WHERE id BETWEEN 1 AND 900000 USING INDEX idx_large_id_pad;

::

    Trace Statistics:
      SELECT (time: 330, fetch: 915916, fetch_time: 244, ioread: 129)
        SCAN (index: dba.large_table.idx_large_id_pad), (btree time: 330, fetch: 915913, ioread: 129, readkeys: 900002, filteredkeys: 900000, rows: 900000) (lookup time: 0, rows: 900000)
             (parallel workers: 4, index time: 330..330, readkeys: 223469..227732, filteredkeys: 223468..227732, rows: 223468..227732, gather: mergeable list) (lookup time: 0..0, rows: 223468..227732)

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

.. _parallel-hash-join:

병렬 해시 조인
--------------

병렬 해시 조인(Parallel Hash Join)은 해시 조인의 빌드(build)·프로브(probe) 단계를 여러 워커 스레드로 병렬화하여 조인 응답 속도를 개선하는 기능이다. 해시 조인은 두 입력을 임시 결과 리스트로 적재한 뒤 한쪽 입력으로 해시 테이블을 만들고(빌드) 다른 쪽 입력으로 해시 테이블을 탐색(프로브)하는데, 병렬 해시 조인은 입력 크기에 따라 다음 두 형태로 수행된다.

*   **파티션 병렬**: 두 입력을 조인 키 기준으로 여러 파티션으로 분할(SPLIT)한 뒤, 워커들이 파티션별 빌드와 프로브를 동시에 수행한다. 빌드 입력이 커서 파티셔닝이 필요한 경우에 사용된다.
*   **프로브 병렬**: 빌드 입력이 작아 해시 테이블을 메모리에 한 번에 만들 수 있는 경우, 빌드는 단일 스레드로 수행하고 프로브 입력만 워커들이 분할하여 동시에 탐색한다.

**활성화 조건**

*   두 입력 리스트 중 큰 쪽의 페이지 수가 활성화 조건(:ref:`parallel-query-throughput-rules`)을 만족해야 한다. 미달이면 **PARALLEL** 힌트가 있어도 단일 스레드 해시 조인으로 실행된다.
*   **NO_PARALLEL_HASH_JOIN** 힌트가 지정되면 병렬화되지 않는다. 이때 해시 조인 자체는 유지되고 병렬화만 비활성화된다. 자세한 내용은 :ref:`NO_PARALLEL_HASH_JOIN <no-parallel-hash-join>`\ 을 참고한다.

해시 조인 트레이스
^^^^^^^^^^^^^^^^^^

해시 조인이 수행되면 :ref:`SQL 트레이스 <query-profiling>` 결과에 **HASHJOIN** 트리가 출력된다. 트리는 파티션 분할(**SPLIT**), 빌드(**BUILD**), 프로브(**PROBE**) 단계별 통계를 보여주며, 이 트리 구조 자체는 병렬 여부와 무관하게 해시 조인이 수행되면 항상 출력된다. 병렬 해시 조인이 적용되면 **HASHJOIN** 항목에 **parallel workers** 가 추가되고, 파티션별 병렬 수행 구간이 **PARALLEL** 항목으로 표시된다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ RECOMPILE PARALLEL(4) USE_HASH */ COUNT(*)
    FROM orders o JOIN large_table t ON o.order_id = t.id;

::

    Trace Statistics:
      SELECT (time: 915, fetch: 76794, fetch_time: 134, ioread: 497)
        SCAN (temp time: 28, fetch: 2465, ioread: 142, readrows: 1000000, rows: 1000000)
             (parallel workers: 4, temp time: 24..28, readrows: 181482..327609, rows: 181482..327609, gather: buildvalue)
          HASHJOIN (time: 886, fetch: 74305, fetch_time: 133, ioread: 349, parallel workers: 4)
            SPLIT (time: 198, fetch: 16086, ioread: 338, partitions: 8)
            PARALLEL (time: 326, fetch: 21073, ioread: 2)
              BUILD (time: 83..96, fetch: 2437, ioread: 0, rows: 1000000, method: hybrid)
              PROBE (time: 180..226, fetch: 18604, ioread: 1, readrows: 1000000, readkeys: 1000000, rows: 1000000)
            SUBQUERY (uncorrelated)
                     (parallel workers: 2, time: 357, fetch: 37116, fetch_time: 86, ioread: 2)
              SELECT (time: 351, fetch: 14869, fetch_time: 40, ioread: 2)
                SCAN (table: dba.orders), (heap time: 348, fetch: 14866, ioread: 2, readrows: 1000000, rows: 1000000)
                     (parallel workers: 4, heap time: 237..348, readrows: 245632..252096, rows: 245632..252096, gather: mergeable list)
              SELECT (time: 356, fetch: 22247, fetch_time: 46, ioread: 0)
                SCAN (table: dba.large_table), (heap time: 356, fetch: 22244, ioread: 0, readrows: 1000000, rows: 1000000)
                     (parallel workers: 4, heap time: 300..354, readrows: 248704..252416, rows: 248704..252416, gather: mergeable list)

해시 조인 트레이스의 병렬 관련 출력 항목은 다음과 같다.

*   **HASHJOIN**\ 의 **parallel workers**: 해시 조인에 사용된 워커 스레드의 수. 이 항목이 없으면 해시 조인이 단일 스레드로 수행된 것이다.
*   **SPLIT**: 두 입력을 조인 키 기준으로 파티션으로 분할하는 단계. **partitions** 는 생성된 파티션 개수이다.
*   **PARALLEL**: 워커들이 파티션별 빌드·프로브를 병렬로 수행한 구간. 하위 **BUILD** 와 **PROBE** 의 **time** 이 워커별 범위(최소..최대)로 표시된다.
*   **SUBQUERY (uncorrelated)**: 조인의 두 입력을 임시 결과 리스트로 적재하는 단계. 두 입력의 적재 자체도 부질의 병렬 실행과 병렬 스캔의 적용 대상이다.

다음은 **NO_PARALLEL_HASH_JOIN** 힌트로 병렬화를 비활성화한 예이다. **HASHJOIN** 항목에 **parallel workers** 가 없고 **PARALLEL** 항목 없이 **BUILD** 와 **PROBE** 가 단일 스레드로 수행되지만, 조인 입력을 적재하는 스캔들은 독립적으로 병렬 수행될 수 있다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ RECOMPILE PARALLEL(4) USE_HASH NO_PARALLEL_HASH_JOIN */ COUNT(*)
    FROM orders o JOIN large_table t ON o.order_id = t.id;

::

    Trace Statistics:
      SELECT (time: 1716, fetch: 74720, fetch_time: 398, ioread: 16587)
        SCAN (temp time: 52, fetch: 2465, ioread: 1346, readrows: 1000000, rows: 1000000)
             (parallel workers: 4, temp time: 46..52, readrows: 223584..302385, rows: 223584..302385, gather: buildvalue)
          HASHJOIN (time: 1663, fetch: 72231, fetch_time: 355, ioread: 15237)
            SPLIT (time: 390, fetch: 15843, ioread: 1029, partitions: 8)
            BUILD (time: 237, fetch: 2431, ioread: 2323, rows: 1000000, method: hybrid)
            PROBE (time: 562, fetch: 16847, ioread: 2136, readrows: 1000000, readkeys: 1000000, rows: 1000000)
            SUBQUERY (uncorrelated)
                     (parallel workers: 2, time: 439, fetch: 37048, fetch_time: 222, ioread: 9733)
              SELECT (time: 360, fetch: 14873, fetch_time: 62, ioread: 2989)
                SCAN (table: dba.orders), (heap time: 360, fetch: 14870, ioread: 2987, readrows: 1000000, rows: 1000000)
                     (parallel workers: 4, heap time: 165..354, readrows: 245632..252096, rows: 245632..252096, gather: mergeable list)
              SELECT (time: 433, fetch: 22175, fetch_time: 160, ioread: 6744)
                SCAN (table: dba.large_table), (heap time: 433, fetch: 22172, ioread: 6742, readrows: 1000000, rows: 1000000)
                     (parallel workers: 4, heap time: 215..433, readrows: 248704..252416, rows: 248704..252416, gather: mergeable list)

다음은 빌드 입력이 작아 **프로브 병렬** 형태로 수행된 예이다. **BUILD** 는 단일 스레드로 수행되고(**method: memory**), **PROBE** 항목 아래에 워커별 범위를 보여주는 병렬 처리 상세 정보가 출력된다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ RECOMPILE PARALLEL(4) USE_HASH */ COUNT(*)
    FROM large_table t JOIN small_table s ON t.category = s.id;

::

    Trace Statistics:
      SELECT (time: 482, fetch: 32028, fetch_time: 76, ioread: 2290)
        SCAN (temp time: 23, fetch: 2441, ioread: 64, readrows: 990000, rows: 990000)
             (parallel workers: 4, temp time: 15..19, readrows: 197807..352823, rows: 197807..352823, gather: buildvalue)
          HASHJOIN (time: 458, fetch: 29575, fetch_time: 72, ioread: 2226)
            BUILD (time: 0, fetch: 0, ioread: 0, rows: 100, method: memory)
            PROBE (time: 123, fetch: 14720, ioread: 0, readrows: 1000000, readkeys: 990000, rows: 990000)
                  (parallel workers: 4, time: 113..119, readrows: 197015..312576, readkeys: 195044..309450, rows: 195044..309450)
            SUBQUERY (uncorrelated)
                     (parallel workers: 2, time: 334, fetch: 22199, fetch_time: 64, ioread: 2226)
              SELECT (time: 334, fetch: 22195, fetch_time: 64, ioread: 2223)
                SCAN (table: dba.large_table), (heap time: 333, fetch: 22192, ioread: 2221, readrows: 1000000, rows: 1000000)
                     (parallel workers: 4, heap time: 288..332, readrows: 248704..252416, rows: 248704..252416, gather: mergeable list)
              SELECT (time: 0, fetch: 4, fetch_time: 0, ioread: 3)
                SCAN (table: dba.small_table), (heap time: 0, fetch: 2, ioread: 1, readrows: 100, rows: 100)

.. _parallel-sort:

병렬 정렬
---------

병렬 정렬(Parallel Sort)은 정렬 입력을 여러 워커 스레드에 분배하여 각자 정렬하게 한 뒤 결과를 병합함으로써 정렬 응답 속도를 개선하는 기능이다. 다음 정렬 연산이 병렬 정렬의 대상이다.

*   ORDER BY 절의 파일 정렬(filesort)
*   분석 함수(analytic function)의 파티션·정렬 처리
*   해시 집계를 적용할 수 없는 GROUP BY의 정렬 (예: DISTINCT 집계 함수가 포함된 경우)
*   DISTINCT 중복 제거를 위한 내부 정렬

**활성화 조건**

*   정렬 입력의 페이지 수가 활성화 조건(:ref:`parallel-query-throughput-rules`)을 만족해야 한다.
*   입력의 행 수가 병렬 처리 수준 이하이면 단일 스레드로 정렬된다.

.. note::

    ORDER BY와 LIMIT을 함께 사용하는 상위 N개 추출 질의는 별도의 병렬 정렬 단계를 거치지 않고, 병렬 스캔 워커들이 스캔 중에 각자 상위 N개를 유지하는 방식으로 병렬화된다. 이때 트레이스에서 스캔의 병렬 처리 상세 정보에 **topnsort: true** 가 표시되고, **ORDERBY** 단계는 최종 병합만 수행한다.

.. note::

    GROUP BY 질의에서 해시 집계가 가능한 형태이면 정렬을 병렬화하는 대신 워커들이 부분 해시 집계를 수행하며, 트레이스의 **GROUPBY** 항목에 **hash: partial** 이 표시된다. **NO_HASH_AGGREGATE** 힌트로 해시 집계를 비활성화한 경우에는 GROUP BY 정렬이 병렬화되지 않는다.

.. note::

    병렬 정렬은 워커별로 임시 정렬 공간을 사용하므로 단일 스레드 정렬보다 임시 볼륨 사용량이 늘어날 수 있다.

정렬 트레이스
^^^^^^^^^^^^^

병렬 정렬이 수행되면 해당 정렬 항목(**ORDERBY**, **GROUPBY**, **ANALYTIC**) 아래에 워커별 소요 시간, 페이지 수, I/O 읽기 수의 범위를 보여주는 병렬 처리 상세 정보가 추가로 출력된다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(4) RECOMPILE */ id, category, pad
    FROM large_table
    ORDER BY category;

::

    Trace Statistics:
      SELECT (time: 3563, fetch: 49462, fetch_time: 235, ioread: 14)
        SCAN (table: dba.large_table), (heap time: 628, fetch: 49418, ioread: 10, readrows: 1000000, rows: 1000000)
             (parallel workers: 4, heap time: 497..628, readrows: 248704..252416, rows: 248704..252416, gather: mergeable list)
        ORDERBY (time: 2934, sort: true, page: 0, ioread: 4)
                (parallel workers: 4, time: 2732..2916, page: 6440..11364, ioread: 46952..58793)

병렬 정렬의 트레이스 출력 항목은 다음과 같다.

*   **parallel workers**: 정렬에 사용된 워커 스레드의 수
*   **time**: 각 워커의 정렬 소요 시간 범위 (최소..최대, 밀리초)
*   **page**, **ioread**: 각 워커가 정렬 중 사용한 페이지 수와 I/O 읽기 수의 범위 (최소..최대)

다음은 분석 함수의 정렬이 병렬화된 예이다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(4) RECOMPILE */ order_id, amount,
           SUM(amount) OVER (PARTITION BY cust_id ORDER BY order_id)
    FROM orders;

::

    Trace Statistics:
      SELECT (time: 1458, fetch: 1059149, fetch_time: 353, ioread: 50971)
        SCAN (table: dba.orders), (heap time: 139, fetch: 19880, ioread: 9910, readrows: 1000000, rows: 1000000)
             (parallel workers: 4, heap time: 135..139, readrows: 245632..252096, rows: 245632..252096, gather: mergeable list)
        ANALYTIC #1 (time: 1317, sort: true, page: 10561, ioread: 33612, rows: 1000000)
                (parallel workers: 4, time: 64..70, page: 3656..4066, ioread: 4..5)

다음은 DISTINCT 집계 함수 때문에 해시 집계를 적용할 수 없어 GROUP BY 정렬이 병렬화된 예이다.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(4) RECOMPILE */ cust_id, COUNT(DISTINCT amount)
    FROM orders
    GROUP BY cust_id;

::

    Trace Statistics:
      SELECT (time: 848, fetch: 984969, fetch_time: 114, ioread: 1742)
        SCAN (table: dba.orders), (heap time: 99, fetch: 14846, ioread: 34, readrows: 1000000, rows: 1000000)
             (parallel workers: 4, heap time: 96..99, readrows: 245632..252096, rows: 245632..252096, gather: mergeable list)
        GROUPBY (time: 749, hash: false, sort: true, page: 129, ioread: 3, readrows: 1000000, rows: 50000)
                (parallel workers: 4, time: 49..57, page: 2470..2759, ioread: 4..90)

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

병렬 힙 스캔과 병렬 리스트 스캔의 병렬 처리 수준은 스캔 대상의 페이지 수(힙 스캔은 테이블 힙 페이지 수, 리스트 스캔은 임시 리스트 페이지 수)에 따라 동일한 규칙으로 결정된다. 인덱스 스캔은 별도 규칙을 따른다(아래 참고).

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

**인덱스 스캔의 처리량 규칙**

병렬 인덱스 스캔의 자동 병렬 수준은 옵티마이저가 **선택도 × 인덱스 페이지 수** 를 기준으로 계산한다. 이 값이 32 이상일 때 후보가 되며, 32에서 2배가 될 때마다 병렬 처리 수준이 2에서 1씩 증가하고 :ref:`parallelism <parallelism>` 파라미터 값을 초과할 수 없다. **PARALLEL** 힌트를 지정하면 이 옵티마이저 계산은 우회하지만, 서버가 실측한 인덱스 페이지 수가 2,048 이상이어야 한다는 활성화 조건은 동일하게 적용된다.

**예제**

.. code-block:: sql

    -- 테이블 생성 및 데이터 삽입
    CREATE TABLE large_table (id INT PRIMARY KEY, category INT, status VARCHAR(10), pad VARCHAR(200));

    INSERT INTO large_table
    SELECT ROWNUM, MOD(ROWNUM, 100),
           CASE WHEN MOD(ROWNUM, 2) = 0 THEN 'active' ELSE 'closed' END,
           LPAD('x', 200, 'x')
    FROM db_class a, db_class b, db_class c, db_class d
    LIMIT 1000000;

    UPDATE STATISTICS ON large_table WITH FULLSCAN;

    -- 테이블 통계 확인 (SHOW HEAP CAPACITY OF large_table)
    -- Num_pages: 17245 (약 269MB, db_page_size가 16K일 때)
    -- Num_recs: 1000000

    -- parallelism 파라미터가 4로 설정된 경우
    -- 페이지 수 17245 → 처리량 규칙 계산값 5 → MIN(5, 4) = 4 적용
    SELECT COUNT(*) FROM large_table WHERE status = 'active';

    -- 힌트로 명시적 지정 (활성화 조건 충족 시 힌트 값이 우선)
    SELECT /*+ PARALLEL(8) */ COUNT(*) FROM large_table WHERE status = 'active';

해시 조인 처리량 규칙
^^^^^^^^^^^^^^^^^^^^^

병렬 해시 조인은 두 입력 리스트 중 **큰 쪽의 페이지 수**\ 를 기준으로 스캔과 동일한 규칙(활성화 조건 2,048페이지, 페이지 수가 2배가 될 때마다 병렬 처리 수준 1 증가, :ref:`parallelism <parallelism>` 상한)에 따라 병렬 처리 수준이 계산된다.

*   계산된 병렬 처리 수준은 파티션 개수를 초과할 수 없다.
*   활성화 조건 미달이면 **PARALLEL** 힌트가 있어도 단일 스레드 해시 조인으로 실행된다.

정렬 처리량 규칙
^^^^^^^^^^^^^^^^

병렬 정렬은 정렬 입력 리스트의 페이지 수를 기준으로 스캔과 동일한 규칙(활성화 조건 2,048페이지, 페이지 수가 2배가 될 때마다 병렬 처리 수준 1 증가, :ref:`parallelism <parallelism>` 상한)에 따라 병렬 처리 수준이 계산된다.

*   입력의 행 수가 계산된 병렬 처리 수준 이하이면 단일 스레드로 정렬된다.
*   활성화 조건 미달이면 **PARALLEL** 힌트가 있어도 단일 스레드로 정렬된다.

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
*   각 병렬 연산은 실행 전에 필요한 병렬 워커 수를 미리 병렬 워커 풀로부터 예약하고, 작업 완료 후 반환한다
*   워커 풀 경합으로 요청한 수를 모두 예약하지 못하면 실제 예약된 워커 수만큼만 병렬로 실행하며, 예약된 워커가 하나도 없으면 단일 스레드 방식으로 실행된다
*   질의 전체에서 사용되는 병렬 처리 수준의 합은 :ref:`parallelism <parallelism>` 파라미터 값을 넘을 수 있으나, :ref:`max_parallel_workers <max_parallel_workers>` 값을 초과할 수는 없다

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
