
.. _parallel-query:

병렬 질의 처리
==============

CUBRID는 대량의 데이터를 효율적으로 처리하기 위해 병렬 쿼리 실행 기능을 제공한다. 병렬 쿼리 실행은 여러 워커 스레드를 사용하여 작업을 동시에 수행함으로써 쿼리 성능을 크게 향상시킬 수 있다.

개요
----

병렬 쿼리는 다음과 같은 주요 기능을 제공한다:

*   **병렬 힙 스캔(Parallel Heap Scan)**: 대량의 데이터를 스캔할 때 여러 워커 스레드를 사용하여 힙 테이블의 스캔 성능을 향상시킨다.
*   **병렬 비상관부질의 실행(Parallel Uncorrelated Subquery Execution)**: 서로 독립적으로 실행 가능한 비상관 부질의들을 여러 워커 스레드를 사용하여 동시에 실행한다.
*   **병렬 해시 조인(Parallel Hash Join)**: 해시 조인 작업을 병렬로 수행한다.
*   **병렬 정렬(Parallel Sort)**: 정렬 작업을 병렬로 수행한다.

설정 방법
^^^^^^^^^

병렬 쿼리 실행은 다음과 같이 설정할 수 있다:

*   :ref:`parallelism <parallelism>` 파라미터를 2 이상으로 설정하면 조건을 만족하는 쿼리에서 병렬 쿼리 실행이 자동으로 활성화될 수 있다.
*   **PARALLEL** ( *degree* ) 힌트를 사용하여 쿼리별로 병렬 처리 정도를 명시적으로 지정할 수 있다. *degree* 는 2 이상의 정수 값이어야 하며, 사용할 워커 스레드의 수를 의미한다.
*   :ref:`max_parallel_workers <max_parallel_workers>` 파라미터는 서버 전역에서 동시에 실행 가능한 병렬 워커 스레드의 최대 개수를 설정한다(기본값: 100).

.. note::

    max_parallel_workers와 parallelism 파라미터는 기본값이 각각 100과 4로 설정되어 있어 별도 설정 없이도 병렬 쿼리를 사용할 수 있다.

.. _parallel-heap-scan:

병렬 힙 스캔
------------

병렬 힙 스캔(Parallel Heap Scan)은 대량의 데이터를 스캔할 때 여러 워커 스레드를 사용하여 힙 테이블의 스캔 성능을 향상시키는 기능이다. 
특히 selectivity가 낮은(약 0.05 보다 낮은) 대량의 데이터를 처리할 때 단일 스레드 기반의 힙 스캔보다 성능이 크게 향상될 수 있다.

힙 스캔 개요
^^^^^^^^^^^^

병렬 힙 스캔은 테이블의 페이지를 여러 워커 스레드에 분산하여 동시에 스캔하고, 각 워커 스레드는 자신에게 할당된 페이지를 독립적으로 처리한다. 
처리된 결과는 결과 큐(result queue)를 통해 수집되며, 메인 스레드는 이 결과를 통합하여 최종 결과를 생성한다.

**NO_PARALLEL_HEAP_SCAN** 힌트를 사용하면 병렬 힙 스캔을 비활성화할 수 있다.

.. note::

    병렬 힙 스캔의 실제 병렬 처리 수준은 처리량 규칙에 따라 자동으로 결정된다. 자세한 내용은 :ref:`parallel-query-throughput-rules`\ 를 참고한다.

제약 조건
^^^^^^^^^^^^^^^^^^

다음 중 하나라도 해당되면 병렬 힙 스캔이 지원되지 않는다:

*   동시성 처리를 지원하지 않는 구문이 포함된 경우 

    *    JavaSP, Serial, 세션 변수, Recursive CTE, Connect By, 오브젝트 DBMS 기능 사용 등

*   update 등 lock(X-LOCK) 획득을 필요로 하는 경우

    *    for update, incr(), update 문, merge 문

*   JOIN 첫번째 테이블이 아닌 경우
*   상관 부질의인 경우
*   조건절에 부질의가 있는 경우

.. note::

    max_parallel_workers와 parallelism 파라미터는 기본값이 각각 100과 4로 설정되어 있어 별도 설정 없이도 병렬 쿼리를 사용할 수 있다. 필요에 따라 cubrid.conf 파일에서 값을 조정할 수 있다. ::

        # cubrid.conf
        max_parallel_workers=200  # 기본값: 100
        parallelism=8             # 기본값: 4

.. code-block:: sql

    -- 병렬 힙 스캔이 적용되지 않는 예

    -- 힌트로 비활성화
    SELECT /*+ NO_PARALLEL_HEAP_SCAN */ * 
    FROM large_table;

    -- 인덱스 스캔 사용 시
    SELECT /*+ PARALLEL(4) USE_IDX */ * 
    FROM large_table 
    WHERE indexed_column = 100;

    -- SELECT FOR UPDATE
    SELECT /*+ PARALLEL(4) */ * 
    FROM large_table 
    FOR UPDATE;

    -- 상관 부질의
    SELECT * 
    FROM orders o
    WHERE EXISTS (
        SELECT 1 
        FROM order_items oi 
        WHERE oi.order_id = o.id AND oi.quantity > 10
    );

    -- 세션 변수 사용
    SET @user_id = 123;
    SELECT /*+ PARALLEL(4) */ * 
    FROM orders 
    WHERE customer_id = @user_id;

    -- SERIAL 사용
    SELECT /*+ PARALLEL(4) */ *, order_seq.NEXT_VALUE 
    FROM orders;

힙 스캔 성능 고려사항
^^^^^^^^^^^^^^^^^^^^^

병렬 힙 스캔은 다음과 같은 경우에 성능 향상 효과가 크다:

*   대량의 데이터를 스캔해야 하는 경우 (테이블의 페이지 수가 많을수록 효과적)
*   selectivity가 낮은 경우 (약 0.05 이하)
*   CPU 코어가 충분히 사용 가능한 경우
*   디스크 I/O보다 CPU 처리가 병목인 경우

반면, 다음과 같은 경우에는 오히려 성능이 저하될 수 있다:

*   소량의 데이터를 스캔하는 경우
*   인덱스 스캔이 더 효율적인 경우
*   시스템 리소스(CPU, 메모리)가 부족한 경우

병렬 쿼리 사용 시에는 :ref:`max_parallel_workers <max_parallel_workers>` 파라미터를 적절히 설정하여 시스템 리소스 경쟁을 방지해야 한다. 일반적으로 CPU 코어 수 정도로 설정하는 것을 권장한다.

힙 스캔 최적화 (Mergeable List)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

병렬 힙 스캔은 특정 조건을 만족하는 경우 "mergeable list" 방식으로 최적화된다. 이 방식에서는 각 워커 스레드가 생성한 임시 결과 파일을 별도의 병합 과정 없이 직접 최종 결과로 반환하여 성능을 크게 향상시킨다.

특히 약 1,000만 건 이상의 대용량 데이터를 8개 이상의 코어로 처리할 때, 기존 방식(각 스레드로부터 결과를 한 건씩 받아 처리, row-by-row)보다 훨씬 빠른 성능을 보인다.

**제약 조건**

다음 조건을 만족하는 경우 mergeable list 최적화가 적용되지 않고 row-by-row 방식으로 처리된다:

*   대상 테이블을 스캔하며 평가할 수 없는 조건절이 힙스캔에 포함되는 경우
*   해시 집계(hash group by)를 수행하는 경우
*   select-list에 JavaSP가 있는 경우
*   ROWNUM을 사용한 경우
*   topn_sort를 수행하는 경우
*   LIMIT 절이 있는 경우
*   result_cache가 활성화되어 있는 경우

**대표적인 적용 예시**

.. code-block:: sql

    -- 조인이 없는 단순 테이블 full scan
    SELECT /*+ PARALLEL(8) */ *
    FROM large_table
    WHERE status = 'active';

    -- 테이블 full scan 후 ORDER BY
    SELECT /*+ PARALLEL(8) */ *
    FROM large_table
    WHERE created_date > '2024-01-01'
    ORDER BY id;

    -- 비상관 부질의에서의 병렬 힙 스캔
    SELECT *
    FROM orders
    WHERE customer_id IN (
        SELECT /*+ PARALLEL(8) */ customer_id
        FROM customers
        WHERE region = 'Asia'
    );

    -- UNION 문의 각 하위 SELECT에 병렬 힙 스캔 적용
    SELECT /*+ PARALLEL(8) */ order_id, customer_id, order_date
    FROM orders_2023
    WHERE status = 'completed'
    UNION
    SELECT /*+ PARALLEL(8) */ order_id, customer_id, order_date
    FROM orders_2024
    WHERE status = 'completed';

    -- 파티션 테이블의 병렬 힙 스캔
    SELECT /*+ PARALLEL(8) */ *
    FROM sales_partitioned
    WHERE sale_date BETWEEN '2024-01-01' AND '2024-12-31';

    -- INSERT SELECT 문 (대용량 데이터 복사)
    INSERT INTO archive_orders
    SELECT /*+ PARALLEL(8) */ *
    FROM orders
    WHERE order_date < '2023-01-01';

COUNT 최적화
""""""""""""

병렬 힙 스캔은 **COUNT(\*)**, **COUNT(column)**, **COUNT(DISTINCT column)** 집계 함수에 대해 특별한 최적화를 제공한다. 
이 최적화는 각 워커 스레드가 로컬 카운트를 수행하고, 최종적으로 결과를 병합하는 방식으로 동작한다.

**COUNT 최적화가 적용되는 조건**

다음 조건을 모두 만족하는 경우 COUNT 전용 최적화가 적용된다:

*   **COUNT(\*)**, **COUNT(column)**, **COUNT(DISTINCT column)** 집계 함수만 포함
*   조건절에 ROWNUM, JavaSP가 없는 경우
*   다른 조인이나 부질의가 없는 단순 쿼리

**COUNT 최적화 동작 방식**

*   **COUNT(\*)**: 각 워커가 간단한 카운터를 증가시키고, 최종적으로 모든 카운트를 합산
*   **COUNT(column)**: 각 워커가 NULL이 아닌 값만 카운트하고, 최종적으로 합산
*   **COUNT(DISTINCT column)**: 각 워커가 별도의 리스트 파일에 값을 저장하여 중복을 제거하고, 최종적으로 모든 리스트를 연결하여 전체 DISTINCT 개수 계산

**COUNT 최적화 예제**

.. code-block:: sql

    -- COUNT(*) 최적화
    SELECT /*+ PARALLEL(8) */ COUNT(*)
    FROM large_table
    WHERE status = 'active';

    -- COUNT(column) 최적화
    SELECT /*+ PARALLEL(8) */ COUNT(customer_id)
    FROM orders
    WHERE order_date > '2024-01-01';

    -- COUNT(DISTINCT) 최적화
    SELECT /*+ PARALLEL(8) */ COUNT(DISTINCT customer_id)
    FROM orders;

    -- UPDATE STATISTICS에서의 활용
    UPDATE STATISTICS ON large_table WITH FULLSCAN;

.. note::

    COUNT 최적화는 단순한 COUNT 쿼리에 적용되며, 다른 집계 함수(SUM, AVG 등)와 함께 사용되거나 복잡한 조인이 포함된 경우에는 일반적인 병렬 힙 스캔 방식(mergeable list 또는 row-by-row)이 사용된다.

힙 스캔 SQL 트레이스
^^^^^^^^^^^^^^^^^^^^

병렬 힙 스캔이 사용되면 :ref:`SQL 트레이스 <query-profiling>`\에 병렬 처리 정보가 출력된다.

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

병렬 힙 스캔 SQL 트레이스는 다음과 같은 형식으로 출력된다:

*   **parallel workers**: 사용된 워커 스레드의 수
*   **heap time**: 각 워커의 힙 스캔 소요 시간 범위 (최소..최대, 밀리초)
*   **readrows**: 각 워커가 읽은 행 수 범위 (최소..최대)
*   **rows**: 각 워커가 반환한 행 수 범위 (최소..최대)
*   **gather**: 결과 수집 방식
    
    * **mergeable list**: 최적화된 방식으로, 각 워커의 결과를 별도 병합 없이 직접 사용
    * **row-by-row**: 기본 방식으로, 각 워커의 결과를 한 건씩 수집하여 병합
    * **count**: COUNT 전용 최적화 방식으로, 각 워커가 로컬 카운트를 수행하고 최종 결과를 병합

**gather: mergeable list** 또는 **gather: count** 가 표시되면 병렬 힙 스캔 최적화가 적용되어 더 나은 성능을 보인다는 의미이다.

.. note::

    병렬 워커들의 시간과 행 수가 범위(최소..최대)로 표시되며, 이상적으로는 모든 워커가 비슷한 양의 작업을 수행해야 한다. 범위가 크게 벌어진다면 데이터 분포나 시스템 리소스 경합 문제를 의심해볼 수 있다.

**COUNT 최적화 추적 정보 예제**

COUNT 최적화가 적용되면 **gather: count** 가 표시된다:

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ PARALLEL(8) RECOMPILE */ COUNT(*)
    FROM large_table;

::

    Trace Statistics:
        SELECT (time: 1500, fetch: 1, fetch_time: 10, ioread: 100000)
            SCAN (table: dba.large_table), (heap time: 1490, fetch: 100000, ioread: 100000, readrows: 0, rows: 0)
                 (parallel workers: 8, heap time: 1485..1490, readrows: 1250000..1250000,
                  rows: 0..0, gather: count)

COUNT 최적화는 결과 행이 하나이므로 rows가 0으로 표시되며, 실제 카운트 결과는 집계 함수를 통해 반환된다.

.. _parallel-subquery-execution:

비상관 부질의 병렬 실행
------------------------

비상관 부질의 병렬 실행(Parallel Uncorrelated Subquery Execution)은 서로 독립적으로 실행 가능한 비상관 부질의(uncorrelated subquery)들을 여러 워커 스레드를 사용하여 동시에 실행함으로써 쿼리 성능을 향상시키는 기능이다.

부질의 실행 개요
^^^^^^^^^^^^^^^^

비상관 부질의는 외부 쿼리와 독립적으로 실행될 수 있으므로, 여러 부질의가 있는 경우 이들을 병렬로 실행하면 전체 쿼리 실행 시간을 단축할 수 있다. 각 부질의는 독립적인 워커 스레드에서 실행되며, 모든 부질의의 실행이 완료되면 결과가 통합되어 최종 결과를 생성한다.

:ref:`parallelism <parallelism>` 파라미터가 2 이상으로 설정되어 있거나, **PARALLEL** ( *degree* ) 힌트를 사용하여 병렬 정도를 2 이상으로 지정하면 비상관 부질의의 병렬 실행이 가능하다.

**NO_PARALLEL_SUBQUERY** 힌트를 사용하면 비상관 부질의의 병렬 실행을 비활성화할 수 있다.

실행 조건
^^^^^^^^^

다음 조건을 모두 만족할 때 비상관 부질의의 병렬 실행이 가능하다:

*   :ref:`max_parallel_workers <max_parallel_workers>` 파라미터가 1 이상으로 설정되어 있고, 사용 가능한 워커 스레드가 있는 경우
*   :ref:`parallelism <parallelism>` 파라미터가 2 이상으로 설정되어 있거나, **PARALLEL** (2) 이상의 힌트가 명시된 경우
*   비상관 부질의가 top-level XASL에 연결되어 있는 경우
*   **NO_PARALLEL_SUBQUERY** 힌트가 사용되지 않은 경우

.. code-block:: sql

    -- parallelism 파라미터 설정 (cubrid.conf)
    -- parallelism=4

    -- 비상관 부질의 병렬 실행 예제
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
    );

적용되지 않는 경우
^^^^^^^^^^^^^^^^^^

다음 중 하나라도 해당되면 비상관 부질의의 병렬 실행이 적용되지 않는다:

*   비상관 부질의가 최상위 레벨 쿼리에 직접 연결되지 않은 경우 (중첩된 부질의 내부의 부질의 등)
*   CTE(Common Table Expression)의 recursive 부분이 존재하거나 CTE 간 참조가 있는 경우
*   derived table(인라인 뷰) 등에 의해 비상관 부질의 간 참조가 존재하는 경우
*   Object DBMS 기능을 사용하는 경우 (path expression 등)
*   JSON_TABLE이나 SET 타입 테이블의 스캔이 포함된 경우
*   부질의 조건절에 저장 프로시저(Java SP)가 포함된 경우

.. code-block:: sql

    -- 병렬 실행이 적용되지 않는 예

    -- NO_PARALLEL_SUBQUERY 힌트 사용
    -- 2개의 부질의가 있지만 힌트로 병렬 실행 비활성화
    SELECT /*+ NO_PARALLEL_SUBQUERY */ *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

    -- CTE 간 참조가 있는 경우
    -- cte2가 cte1을 참조하므로 독립적이지 않음
    WITH cte1 AS (
        SELECT * FROM table1
    ),
    cte2 AS (
        SELECT * FROM cte1 WHERE id > 100  -- cte1 참조
    )
    SELECT * FROM cte2;

    -- JSON_TABLE 사용
    -- JSON_TABLE이 포함되면 부질의가 2개 이상 있어도 병렬 실행 안 됨
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
    -- 2개의 부질의가 있지만 하나에 저장 프로시저가 있어 병렬 실행 안 됨
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

비상관 부질의의 병렬 실행은 다음과 같은 경우에 성능 향상 효과가 크다:

*   복수의 독립적인 비상관 부질의가 존재하는 경우
*   각 부질의의 실행 시간이 충분히 긴 경우
*   CPU 코어가 충분히 사용 가능한 경우
*   부질의 결과 크기가 적절한 경우

반면, 다음과 같은 경우에는 오히려 성능이 저하될 수 있다:

*   부질의의 실행 시간이 매우 짧은 경우 (병렬 처리 오버헤드가 더 클 수 있음)
*   부질의가 하나만 존재하는 경우
*   시스템 리소스(CPU, 메모리)가 부족한 경우
*   :ref:`max_parallel_workers <max_parallel_workers>` 설정이 부적절한 경우

관련 파라미터
^^^^^^^^^^^^^

비상관 부질의의 병렬 실행을 효과적으로 사용하려면 다음 파라미터들을 적절히 설정해야 한다:

*   :ref:`max_parallel_workers <max_parallel_workers>`: 서버 전역의 최대 병렬 워커 수
*   :ref:`parallelism <parallelism>`: 기본 병렬 처리 정도

.. code-block:: sql

    -- cubrid.conf 설정 예제
    max_parallel_workers=16
    parallelism=4

부질의 추적 정보
^^^^^^^^^^^^^^^^

비상관 부질의의 병렬 실행이 사용되면 :ref:`SQL 트레이스 <query-profiling>`\에 병렬 처리 정보가 출력된다.

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

비상관 부질의 병렬 실행 SQL 트레이스는 다음과 같은 형식으로 출력된다:

*   **SUBQUERY (uncorrelated)**: 비상관 부질의 실행 표시
*   **parallel workers**: 병렬 실행에 사용된 워커 스레드의 수
*   **time**: 병렬 실행에 소요된 시간 (밀리초)
*   각 부질의는 독립적인 SELECT로 표시되며, 각각의 실행 통계가 출력된다

위 예제에서는 2개의 부질의(customers 테이블 조회, products 테이블 조회)가 2개의 워커 스레드를 사용하여 병렬로 실행되었다.

.. _parallel-query-throughput-rules:

병렬 쿼리 처리량 규칙
---------------------

처리량 규칙 개요
^^^^^^^^^^^^^^^^

병렬 쿼리 실행은 높은 성능을 제공하지만, 동시에 서버 자원을 많이 소모한다. 소수의 쿼리가 병렬 실행으로 서버 자원을 과도하게 선점하면 다른 다수의 쿼리 성능이 저하될 수 있다. 이를 방지하기 위해 CUBRID는 병렬 실행의 효과가 큰 쿼리만 선별하여 병렬 실행을 허용하는 처리량 규칙을 적용한다.

각 병렬 연산의 실제 병렬 처리 수준은 다음 요인에 따라 결정된다:

*   테이블 크기, 파티션 개수 등의 처리량 규칙
*   **PARALLEL** 힌트로 명시적으로 지정된 값
*   :ref:`parallelism <parallelism>` 파라미터로 설정된 상한값
*   :ref:`max_parallel_workers <max_parallel_workers>` 파라미터로 설정된 전역 워커 풀 크기

처리량 규칙으로 계산된 병렬 처리 수준은 :ref:`parallelism <parallelism>` 파라미터 값을 초과할 수 없다. 힌트로 지정된 병렬 처리 수준은 :ref:`parallelism <parallelism>` 파라미터 값을 초과할 수 있지만 최대값(32 또는 시스템 코어 수 중 작은 값)은 초과할 수 없다.

힙 스캔 처리량 규칙
^^^^^^^^^^^^^^^^^^^

병렬 힙 스캔의 병렬 처리 수준은 스캔 대상 테이블의 페이지 수에 따라 결정된다.

**활성화 조건**

*   스캔 대상 테이블의 페이지 수가 4,096개 이상일 때 활성화된다 (약 64MB, db_page_size가 16K일 때)
*   이 조건을 만족하지 않으면 **PARALLEL** 힌트가 있어도 병렬 힙 스캔이 활성화되지 않는다

**처리 수준 결정**

병렬 처리 수준은 테이블의 페이지 수에 따라 다음과 같이 결정된다:

.. csv-table::
   :header: "페이지 수", "처리량", "처리량 규칙 계산값"
   :widths: 15, 15, 15

   "4,096", "64 MB", "2"
   "8,192", "128 MB", "3"
   "16,384", "256 MB", "4"
   "32,768", "512 MB", "5"
   "65,536", "1.0 GB", "6"
   "131,072", "2.0 GB", "7"
   "262,144", "4.0 GB", "8"
   "524,288", "8.0 GB", "9"
   "1,048,576", "16.0 GB", "10"
   "2,097,152", "32.0 GB", "11"
   "4,194,304", "64.0 GB", "12"
   "8,388,608", "128.0 GB", "13"

페이지 수가 4,096개를 시작으로, 이전에 증가한 기준 페이지 수의 2배가 될 때마다 처리량 규칙으로 계산된 수준이 1씩 증가한다.

**처리량 규칙에 따라 결정되는 병렬 처리 수준은 :ref:`parallelism <parallelism>` 파라미터 값을 초과할 수 없다:**

*   **MIN (처리량 규칙 계산값, parallelism 파라미터 값)**

예를 들어, parallelism=4 (기본값)로 설정된 경우:
- 페이지 수 4,096개 → 처리량 규칙 계산값 2 → MIN(2, 4) = **2** 적용
- 페이지 수 65,536개 → 처리량 규칙 계산값 6 → MIN(6, 4) = **4** 적용 (parallelism 초과 불가)

.. note::

    **PARALLEL** 힌트로 병렬 수준을 명시적으로 지정한 경우에는 처리량 규칙이 적용되지 않고 힌트 값이 사용된다.

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
    -- 페이지 수 4215는 4,096 이상이므로 병렬 처리 수준 2가 자동 적용됨
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

비상관 부질의(Uncorrelated Subquery)의 병렬 실행은 서로의 결과를 참조하지 않는 복수의 부질의가 존재할 때 활성화된다.

*   병렬 처리 수준은 2로 고정된다
*   각 부질의가 독립적으로 실행 가능하고 서로의 결과를 참조하지 않는 경우 병렬 실행이 가능하다
*   여러 독립적인 비상관 부질의가 존재하는 경우 병렬 실행의 효과가 크다

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

*   :ref:`max_parallel_workers <max_parallel_workers>` 파라미터로 전역 워커 풀의 최대 개수를 설정한다
*   각 서버 스레드는 병렬 쿼리 실행 전에 필요한 워커 수를 미리 예약하고, 작업 완료 후 해제한다
*   예약에 실패할 경우 일반적인 단일 스레드 실행 방식으로 쿼리가 수행된다
*   쿼리 전체에서 사용되는 병렬 처리 수준의 합은 :ref:`parallelism <parallelism>` 파라미터 값을 넘을 수 있으나, :ref:`max_parallel_workers <max_parallel_workers>` 값을 초과할 수는 없다

.. code-block:: sql

    -- cubrid.conf 설정 예제
    max_parallel_workers=100  # 전역 워커 풀 크기
    parallelism=4             # 단일 병렬 연산의 상한값

처리량 성능 고려사항
^^^^^^^^^^^^^^^^^^^^

병렬 쿼리 처리량 규칙을 통한 최적화:

*   작은 테이블에 대한 불필요한 병렬 실행을 방지하여 오버헤드를 줄인다
*   테이블 크기에 비례하여 병렬 처리 수준을 자동으로 조정한다
*   과도한 병렬 실행으로 인한 시스템 자원 경쟁을 방지한다
*   효과가 큰 쿼리에 집중적으로 병렬 자원을 할당한다

권장 설정:

*   **max_parallel_workers**: 동시 실행 가능한 병렬 쿼리 수와 각 쿼리의 평균 병렬 처리 수준을 고려하여 설정
*   **parallelism**: 시스템의 물리 코어 수를 고려하여 설정 (보통 4~8 정도가 적절)
*   대용량 테이블이 많은 환경에서는 **max_parallel_workers** 값을 높게 설정
*   소규모 테이블이 많은 환경에서는 기본값 사용을 권장

