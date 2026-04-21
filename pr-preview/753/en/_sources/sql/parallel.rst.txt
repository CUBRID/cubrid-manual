
.. _parallel-query:

Parallel Execution
==================

CUBRID provides parallel query execution capabilities to efficiently process large amounts of data. Parallel query execution divides a single query into multiple work units, which are then processed by multiple worker threads simultaneously, dramatically reducing response time.

Overview
--------

Parallel queries provide the following key features:

*   **Parallel Scan**: Multiple worker threads divide and scan the input data (heap, temporary list, or index), improving large input scanning performance. Three scan flavors are supported:

    *   **Parallel Heap Scan**: Heap pages of a table are partitioned by **sector** and scanned in parallel.
    *   **Parallel List Scan**: A temporary result list (list file) that has spilled to disk is partitioned by **sector** and scanned in parallel.
    *   **Parallel Index Scan**: Workers cooperate through a shared cursor to walk the leaf pages of a B+tree index from left to right (or right to left).

*   **Parallel Subquery Execution**: Independent subqueries (uncorrelated subqueries) are processed simultaneously by individual workers, improving query response time.
*   **Parallel Hash Join**: Parallelizes both the build and probe phases, improving response time during hash join operations.
*   **Parallel Sort**: Divides data to be sorted among multiple worker threads, sorts in parallel, then merges the results, improving sort response time.

Configuration
^^^^^^^^^^^^^

Parallel query execution can be controlled through system parameters and SQL hints.

*   Setting the :ref:`parallelism <parallelism>` parameter to 2 or higher enables the optimizer to determine parallel query execution during query processing.
*   Use the **PARALLEL** ( *degree* ) hint to explicitly specify the degree of parallelism for each query. *degree* is the number of workers to use and must be an integer value of 2 or higher. Hint-specified values take precedence over the parallelism parameter setting.
*   The :ref:`max_parallel_workers <max_parallel_workers>` parameter sets the maximum number of parallel worker threads that can be executed simultaneously across the entire server (default: 100).
*   The **NO_PARALLEL_SCAN** hint disables every parallel scan flavor (heap, list, and index) within the query block. When used together with the **PARALLEL** hint, **NO_PARALLEL_SCAN** takes precedence.

.. note::

    The max_parallel_workers and parallelism parameters are set to default values of 100 and 4 respectively, so you can use parallel queries without additional configuration.

.. _parallel-scan:

Parallel Scan
-------------

Parallel Scan splits a single scan input across multiple worker threads that process it concurrently. CUBRID supports parallel scan over three input kinds, all sharing the same parallel execution framework:

*   **Heap**: heap pages of a table — pre-partitioned across workers by **sector**
*   **List**: pages of a temporary (on-disk) result list file — pre-partitioned across workers by **sector**
*   **Index**: leaf pages of a B+tree index — workers cooperate through a shared **cursor**, walking left to right (or right to left)

Each worker thread independently scans its assigned region while evaluating filter predicates, and the processed results are passed to the main thread through a result queue and integrated into the final result.

.. note::

    The actual degree of parallelism is automatically optimized by throughput rules within the user-configured upper limit. For more details, see :ref:`parallel-query-throughput-rules`.

Common Constraints
^^^^^^^^^^^^^^^^^^

Regardless of the scan flavor, parallel scan is not applied and falls back to single-threaded execution if any of the following conditions hold:

*   Statements that do not support concurrent processing

    *    Stored procedures (JavaSP, PL/CSQL) or Serial usage
    *    References to session variables
    *    Recursive CTE or Connect By clauses
    *    CUBRID object DBMS specific features

*   Operations requiring exclusive lock (X-LOCK) acquisition

    *    SELECT ... FOR UPDATE clause
    *    Use of the incr() function
    *    update, delete, merge statements

*   The scan is not the first (driving) table in a JOIN
*   Correlated subqueries
*   The scan is the direct outer/inner input of a sort-merge join (applies to every scan flavor)
*   The **NO_PARALLEL_SCAN** hint is specified

Each scan flavor has additional, flavor-specific constraints. See the corresponding subsections below.

.. code-block:: sql

    -- Examples where parallel scan is not applied

    -- Disabled by hint (covers every scan flavor)
    SELECT /*+ NO_PARALLEL_SCAN */ *
    FROM large_table;

    -- SELECT FOR UPDATE
    SELECT /*+ PARALLEL(4) */ *
    FROM large_table
    FOR UPDATE;

    -- Using session variables
    SET @user_id = 123;
    SELECT /*+ PARALLEL(4) */ *
    FROM orders
    WHERE customer_id = @user_id;

    -- Using SERIAL
    SELECT /*+ PARALLEL(4) */ *, order_seq.NEXT_VALUE
    FROM orders;

.. _parallel-heap-scan:

Parallel Heap Scan
^^^^^^^^^^^^^^^^^^

Parallel Heap Scan statically partitions the heap pages of a table by sector and lets workers scan their partitions concurrently. It can yield a large speedup over single-threaded heap scan, especially when selectivity is low (typically 0.05 or less).

Heap scan has no additional flavor-specific restrictions beyond the :ref:`common constraints <parallel-scan>` listed above.

.. code-block:: sql

    -- Parallel heap scan
    SELECT /*+ PARALLEL(8) */ *
    FROM large_table
    WHERE status = 'active';

    -- Parallel heap scan over a partitioned table
    SELECT /*+ PARALLEL(8) */ *
    FROM sales_partitioned
    WHERE sale_date BETWEEN '2024-01-01' AND '2024-12-31';

    -- INSERT SELECT (bulk copy)
    INSERT INTO archive_orders
    SELECT /*+ PARALLEL(8) */ *
    FROM orders
    WHERE order_date < '2023-01-01';

.. _parallel-list-scan:

Parallel List Scan
^^^^^^^^^^^^^^^^^^

Parallel List Scan statically partitions a temporary on-disk result list (list file) — produced by a subquery, derived table, or other intermediate operator — across workers by **sector**, and they read it concurrently. The partitioning mechanism itself is identical to parallel heap scan; the only difference is that the input is a temporary file rather than a table heap. It is effective when an upper operator must rescan a large intermediate result.

**Additional list-scan constraints**

Parallel list scan is not applied — and falls back to a single-threaded list scan — if any of the following hold:

*   The temporary list resides only in the in-memory buffer and has not spilled to a disk temp file (no sectors to partition — small lists fall back automatically).
*   The upper XASL consumes results in row-by-row mode (a query shape that admits neither mergeable list nor BUILDVALUE; see :ref:`result-collection-modes`).
*   The list scan sits inside the auxiliary input subtree (subquery, CTE, etc.) of a sort-merge join.

.. code-block:: sql

    -- Typical pattern that benefits from parallel list scan:
    -- the inner subquery materialises a list, which the outer
    -- query then re-aggregates.
    SELECT /*+ PARALLEL(8) */ region, COUNT(*)
    FROM (
        SELECT region, customer_id
        FROM orders o, customers c
        WHERE o.customer_id = c.id
    ) t
    GROUP BY region;

.. _parallel-index-scan:

Parallel Index Scan
^^^^^^^^^^^^^^^^^^^

Parallel Index Scan lets multiple workers cooperatively walk the leaf pages of a B+tree index through a shared cursor. The vertical descent (root → leaf entry) is performed serially by the main thread; the subsequent leaf traversal, OID fetching, and predicate evaluation are parallelised across workers. Each worker grabs one leaf page, processes its keys independently, and only briefly synchronises to obtain the next leaf.

**Additional index-scan constraints**

Parallel index scan is not applied — and falls back to a single-threaded index scan — if any of the following hold:

*   The scan uses an index-driven traversal optimisation that changes how the tree is entered or walked:

    *   ISS (Index Skip Scan)
    *   ILS (Index Loose Scan)
    *   KEYLIMIT clause
    *   ORDERBY_SKIP / GROUPBY_SKIP / ORDERBY_DESC / GROUPBY_DESC
    *   USE_DESC_INDEX hint
    *   **filtered index** (a *function index*, however, is unaffected)
    *   MIN/MAX single-key scan (min_max scan)

*   The upper XASL imposes row-by-row semantics on the index scan through ROWNUM, ANALYTIC SKIP SORT, or ANALYTIC LIMIT OPT.
*   The upper XASL consumes results in row-by-row mode (a query shape that admits neither mergeable list nor BUILDVALUE; see :ref:`result-collection-modes`).
*   The index scan sits inside the auxiliary input subtree (subquery, CTE, etc.) of a sort-merge join.

.. code-block:: sql

    -- Typical case where parallel index scan applies
    -- (covering / simple range over a large index)
    CREATE INDEX idx_orders_status ON orders(status, order_date);

    SELECT /*+ PARALLEL(8) */ order_id, order_date
    FROM orders
    WHERE status = 'completed' USING INDEX idx_orders_status;

    -- Parallel index scan is NOT applied
    -- (USE_DESC_INDEX hint forces single-threaded index scan)
    SELECT /*+ PARALLEL(8) USE_DESC_INDEX */ *
    FROM orders
    WHERE status = 'completed';

.. note::

    Because the tree descent is serial on the main thread and the leaf traversal is cooperative, parallel index scan pays off when the index has enough leaf pages. On small indexes the synchronisation cost can outweigh the speedup; the throughput rules (:ref:`parallel-query-throughput-rules`) guard against this.

Performance Considerations
^^^^^^^^^^^^^^^^^^^^^^^^^^

Parallel scan delivers the largest gains in the following cases:

*   Large inputs (tables, lists, indexes) — the more pages, the better
*   Low selectivity (≈ 0.05 or less) for heap and index scans
*   Sufficient CPU cores are available
*   CPU processing — not disk I/O — is the bottleneck

Conversely, performance can regress in these cases:

*   The input has only a small number of pages
*   A single-threaded index scan is already fast enough (e.g., short range / point lookup)
*   System resources (CPU, memory) are scarce

When using parallel queries, set the :ref:`max_parallel_workers <max_parallel_workers>` parameter so that workers do not contend excessively for system resources. A value close to the number of physical CPU cores is usually a good starting point.

.. _result-collection-modes:

Result Collection Modes
^^^^^^^^^^^^^^^^^^^^^^^

Once parallel scan is enabled, the way the main thread collects worker results depends on the query shape and is one of the three modes below. The mode appears as the **gather** field in the SQL trace.

*   **mergeable list**: each worker builds its own temporary result list and the main thread uses those lists directly without merging. This has the lowest synchronisation cost and is usually the fastest mode.
*   **buildvalue**: each worker computes a partial aggregate and the main thread combines the partials into the final aggregate. Used for simple aggregate queries (see :ref:`buildvalue-optimization`).
*   **row-by-row**: the main thread receives one row at a time. Applies when neither of the other two modes can be used. It has the broadest applicability but the highest synchronisation cost.

.. note::

    The row-by-row mode is observed **only with parallel heap scan**. Parallel list scan and parallel index scan fall back to single-threaded execution for query shapes that would require row-by-row (see the additional constraints in their respective sections), so ``gather: row-by-row`` only appears in heap-scan traces.

**When mergeable list is not chosen**

Mergeable list is replaced by another mode if any of the following hold:

*   The scan carries predicates that cannot be evaluated while scanning (deferred to an upper operator).
*   Hash group-by is performed.
*   The select-list contains a stored procedure (JavaSP or PL/CSQL).
*   ROWNUM is used.
*   topn_sort (sort to extract the top N) is performed.
*   There is a LIMIT clause.
*   result_cache is enabled.

.. _buildvalue-optimization:

BUILDVALUE Optimization
^^^^^^^^^^^^^^^^^^^^^^^

When the SELECT list consists solely of supported aggregate functions and there are no per-row semantics such as ROWNUM, parallel scan applies the **BUILDVALUE optimization**. In this mode, each worker computes a partial aggregate over its scanned region and ships it to the main thread, which then combines the partials into the final result. Because workers exchange the smallest possible amount of data, this is the fastest mode for simple aggregate queries.

**Supported aggregate functions**

The BUILDVALUE optimization applies when the SELECT list uses only the following aggregate functions:

*   **COUNT(\*)**, **COUNT(column)**, **COUNT(DISTINCT column)**
*   **MIN(column)**, **MAX(column)**
*   **SUM(column)**, **AVG(column)**
*   **STDDEV(column)**, **STDDEV_POP(column)**, **STDDEV_SAMP(column)**
*   **VARIANCE(column)**, **VAR_POP(column)**, **VAR_SAMP(column)**

**Conditions**

In addition to using only the supported aggregates, all of the following must hold:

*   The SELECT list contains only the supported aggregate functions (no non-aggregate output columns).
*   The query has no ROWNUM and no stored procedures in its predicates.
*   The query is simple — no joins or subqueries combined with the aggregate.

**Scope**

The BUILDVALUE optimization is independent of the scan flavor and can be applied to:

*   Parallel heap scan
*   Parallel list scan
*   Parallel index scan

**Examples**

.. code-block:: sql

    -- COUNT family
    SELECT /*+ PARALLEL(8) */ COUNT(*)
    FROM large_table
    WHERE status = 'active';

    SELECT /*+ PARALLEL(8) */ COUNT(DISTINCT customer_id)
    FROM orders;

    -- Arithmetic aggregates (heap, list, or index scan)
    SELECT /*+ PARALLEL(8) */ SUM(amount), AVG(amount), MAX(amount)
    FROM orders
    WHERE order_date > '2024-01-01';

    -- Variance / standard deviation
    SELECT /*+ PARALLEL(8) */ STDDEV(price), VARIANCE(price)
    FROM products;

    -- UPDATE STATISTICS internally benefits from BUILDVALUE optimization as well
    UPDATE STATISTICS ON large_table WITH FULLSCAN;

.. note::

    If the SELECT list mixes the supported aggregates with other expressions (e.g., plain columns, unsupported aggregate functions) or is combined with GROUP BY, the BUILDVALUE optimization is not applied and the query is processed in the mergeable list or row-by-row mode instead.

Scan SQL Trace
^^^^^^^^^^^^^^

When parallel scan is performed, parallel processing details are added to the :ref:`SQL trace <query-profiling>` output.

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

The parallel scan trace fields are:

*   **parallel workers**: number of worker threads used.
*   **heap time / list time / index time**: per-worker scan time range (min..max, milliseconds). The label changes with the scan flavor.
*   **readrows**: per-worker range of rows read (min..max).
*   **rows**: per-worker range of rows produced (min..max).
*   **gather**: how worker results were collected.

    *   **mergeable list**: per-worker lists are used directly without merging.
    *   **buildvalue**: per-worker partial aggregates are combined (replaces the legacy ``count`` label).
    *   **row-by-row**: rows are collected one at a time (heap scan only).

When **gather** shows **mergeable list** or **buildvalue**, the query took the lowest-synchronisation path.

.. note::

    Per-worker times and row counts appear as min..max ranges. Ideally all workers do similar amounts of work; a wide range hints at uneven data distribution or system resource contention.

**BUILDVALUE optimization trace example**

When BUILDVALUE optimization is applied, **gather: buildvalue** is shown. Because only one aggregate row is produced overall, per-worker ``rows`` is reported as 0.

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

**Parallel index scan trace example**

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

Parallel Subquery Execution
----------------------------

Parallel Subquery Execution is a feature that improves query performance by using multiple worker threads to simultaneously execute subqueries that can run independently.

Subquery Execution Overview
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Since subqueries can be executed independently of other subqueries, when there are multiple subqueries, executing them in parallel can reduce overall query response time. Each subquery is executed in an independent worker thread, and when all subqueries are completed, the results are merged to generate the final result.

Parallel execution of subqueries is possible if the :ref:`parallelism <parallelism>` parameter is set to 2 or higher, or if the degree of parallelism is specified to 2 or higher using the **PARALLEL** ( *degree* ) hint.

The **NO_PARALLEL_SUBQUERY** hint can be used to disable parallel execution of subqueries. When used together with the **PARALLEL** hint, the **NO_PARALLEL_SUBQUERY** hint takes precedence.

Execution Conditions
^^^^^^^^^^^^^^^^^^^^

Parallel execution of subqueries is possible when all of the following conditions are met:

*   The :ref:`max_parallel_workers <max_parallel_workers>` parameter is set to 2 or higher, and available worker threads exist
*   The :ref:`parallelism <parallelism>` parameter is set to 2 or higher, or a **PARALLEL** (2) or higher hint is specified
*   The subquery is directly connected to the top-level query (top-level XASL)
*   The **NO_PARALLEL_SUBQUERY** hint is not used

.. code-block:: sql

    -- parallelism parameter setting (cubrid.conf)
    -- parallelism=4

    -- Subquery parallel execution example
    SELECT *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

    -- Example using hint
    SELECT /*+ PARALLEL(4) */ *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

Cases Not Applied
^^^^^^^^^^^^^^^^^

Parallel execution of subqueries is not applied if any of the following conditions apply:

*   The subquery is not directly connected to the top-level query (such as subqueries inside nested subqueries)
*   CTE (Common Table Expression) recursive part exists or there are references between CTEs
*   References between subqueries exist due to derived tables (inline views), etc.
*   Object DBMS features are used (such as path expressions, etc.)
*   **JSON_TABLE** or SET type table scans are included
*   The subquery condition clause contains stored procedures
*   When it is a correlated subquery

.. code-block:: sql

    -- Examples where parallel execution is not applied

    -- Using NO_PARALLEL_SUBQUERY hint
    SELECT /*+ NO_PARALLEL_SUBQUERY */ *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

    -- When there are references between CTEs
    WITH cte1 AS (
        SELECT * FROM table1
    ),
    cte2 AS (
        SELECT * FROM cte1 WHERE id > 100  -- references cte1
    )
    SELECT * FROM cte2;

    -- Using JSON_TABLE
    SELECT *
    FROM orders,
    JSON_TABLE(json_column, '$[*]' COLUMNS(id INT PATH '$.id')) AS jt
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

    -- When stored procedure is included in condition clause
    SELECT *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE check_region_sp(region) = 1
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

Subquery Performance Considerations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Parallel execution of subqueries has significant performance improvements in the following cases:

*   When multiple independent subqueries exist
*   When each subquery's execution time is sufficiently long
*   When sufficient CPU cores are available

On the other hand, performance may degrade in the following cases:

*   When subquery execution time is very short (parallel processing overhead may be greater)
*   When only one subquery exists
*   When system resources (CPU, memory) are insufficient
*   When :ref:`max_parallel_workers <max_parallel_workers>` setting is inappropriate

Related Parameters
^^^^^^^^^^^^^^^^^^

To effectively use parallel execution of subqueries, the following parameters should be set appropriately:

*   :ref:`max_parallel_workers <max_parallel_workers>`: Maximum number of parallel workers across the server
*   :ref:`parallelism <parallelism>`: Default degree of parallelism

.. code-block:: sql

    -- cubrid.conf configuration example
    max_parallel_workers=16
    parallelism=4

Subquery Trace Information
^^^^^^^^^^^^^^^^^^^^^^^^^^^

When parallel execution of subqueries is performed, parallel processing details are additionally output in the :ref:`SQL trace <query-profiling>` results.

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

The description of subquery parallel execution SQL trace output items is as follows:

*   **SUBQUERY (uncorrelated)**: Indicates subquery execution
*   **parallel workers**: Number of worker threads used for parallel execution
*   **time**: Time spent on parallel execution (milliseconds)
*   Each subquery is displayed as an independent SELECT, and execution statistics for each are output

In the above example, 2 subqueries (customers table query, products table query) were executed in parallel using 2 worker threads.

.. _parallel-query-throughput-rules:

Parallel Query Throughput Rules
--------------------------------

Throughput Rules Overview
^^^^^^^^^^^^^^^^^^^^^^^^^^

While parallel query execution dramatically reduces query response time, it also consumes significant server resources (CPU, memory, I/O, etc.). If a small number of queries excessively occupy server resources through parallel execution, the performance of other queries may degrade. To prevent this, CUBRID applies **throughput rules** to selectively allow parallel execution only for queries with significant parallel execution benefits.

The actual degree of parallelism for each parallel operation is determined by the following factors:

*   Throughput rules based on the size of the input (table, list, or index)
*   Values explicitly specified by **PARALLEL** hints
*   Upper limit set by the :ref:`parallelism <parallelism>` parameter
*   Global worker pool size set by the :ref:`max_parallel_workers <max_parallel_workers>` parameter

The degree of parallelism calculated by throughput rules cannot exceed the :ref:`parallelism <parallelism>` parameter value. The degree of parallelism specified by hints can exceed the :ref:`parallelism <parallelism>` parameter value but cannot exceed the maximum value (the smaller of 32 or the number of system cores).

Scan Throughput Rules
^^^^^^^^^^^^^^^^^^^^^

The degree of parallelism for parallel scan (heap, list, or index) is determined by the same rule based on the page count of the scan input. Heap scan uses the table's heap page count, list scan uses the temporary list page count, and index scan uses the index leaf page count.

**Activation Condition**

*   Activated when the input has 2,048 or more pages (approximately 32 MB when db_page_size is 16K).
*   If this condition is not met, parallel scan is not activated even if the **PARALLEL** hint is present.

**Degree Determination**

The degree of parallelism is determined according to the page count as follows:

.. csv-table::
   :header: "Number of Pages", "Throughput", "Throughput Rule Calculation"
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

Starting from 2,048 pages, the degree of parallelism calculated by the throughput rule increases by 1 each time the page count doubles from the previous threshold.

**The degree of parallelism determined by throughput rules cannot exceed the** :ref:`parallelism <parallelism>` **parameter value:**

*   **MIN (throughput rule calculation, parallelism parameter value)**

For example, when parallelism=4 (default):

*   Page count 2,048 → throughput rule calculates 2 → MIN(2, 4) = **2** applied

*   Page count 65,536 → throughput rule calculates 7 → MIN(7, 4) = **4** applied (cannot exceed parallelism)

.. note::

    Even when the degree of parallelism is explicitly specified using the **PARALLEL** hint, the activation condition (2,048 or more pages) still applies. After activation, the hint value takes precedence in determining the degree of parallelism.

**Example**

.. code-block:: sql

    -- Create table and insert data
    CREATE TABLE large_table (c1 INT);

    INSERT INTO large_table
    WITH RECURSIVE cte (n) AS (
        SELECT 1
        UNION ALL
        SELECT n + 1 FROM cte WHERE n < 2000
    )
    SELECT ROWNUM FROM cte a, cte b, cte c LIMIT 2200000;

    UPDATE STATISTICS ON large_table WITH FULLSCAN;

    -- Check table statistics
    -- Total pages in class heap: 4215 (approximately 66MB when db_page_size is 16K)
    -- Total objects: 2200000

    -- When parallelism parameter is set to 4
    -- Page count 4215 is at least 2,048, so degree of parallelism 3 is automatically applied
    SELECT COUNT(*) FROM large_table;

    -- Explicit specification with hint
    SELECT /*+ PARALLEL(8) */ COUNT(*) FROM large_table;

Hash Join Throughput Rules
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The degree of parallelism for parallel hash join is determined according to throughput rules, and the determined degree of parallelism should be less than or equal to the number of partitions.

.. note::

    Detailed throughput rules for parallel hash join will be added in future versions.

Sort Throughput Rules
^^^^^^^^^^^^^^^^^^^^^

The degree of parallelism for parallel sort is determined according to throughput rules, and the determined degree of parallelism should be less than or equal to the number of input pages.

.. note::

    Detailed throughput rules for parallel sort will be added in future versions.

Subquery Throughput Rules
^^^^^^^^^^^^^^^^^^^^^^^^^^

Parallel execution of subqueries is activated when multiple subqueries have an independent structure where they do not reference each other's results.

*   The degree of parallelism for subquery parallel execution is fixed at 2. For example, even if there are 4 independent subqueries within a query, the system allocates 2 parallel workers for processing.
*   Whether each subquery is executed in parallel is determined by the "throughput rules".
*   When multiple independent subqueries exist, the effect of parallel execution is significant

.. code-block:: sql

    -- Parallel subquery execution example
    -- When parallelism=4, 2 subqueries are executed in parallel
    SELECT *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

Worker Thread Pool Management
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If the parallel thread pool is insufficient, only some operations may be performed in parallel, or parallel execution may not be performed at all.

*   Set the maximum thread count for the global parallel processing worker pool with the :ref:`max_parallel_workers <max_parallel_workers>` parameter
*   Each worker thread reserves the required number of parallel workers from the parallel worker pool before parallel query execution, and returns them after task completion
*   If the reservation fails, the query is executed in the normal single-threaded manner
*   The sum of the degrees of parallelism used in the entire query can exceed the :ref:`parallelism <parallelism>` parameter value, but cannot exceed the :ref:`max_parallel_workers <max_parallel_workers>` value

.. code-block:: sql

    -- cubrid.conf configuration example
    max_parallel_workers=100  # Global worker pool size
    parallelism=4             # Upper limit for single parallel operation

Throughput Performance Considerations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Optimization through parallel query throughput rules:

*   Prevents unnecessary parallel execution on small inputs to reduce overhead
*   Automatically adjusts the degree of parallelism proportional to input size
*   Prevents system resource contention due to excessive parallel execution
*   Concentrates parallel resources on queries with significant benefits

Recommended Settings:

*   **max_parallel_workers**: Set considering the number of simultaneously executable parallel queries and the average degree of parallelism for each query
*   **parallelism**: Set considering the number of physical cores in the system (usually 4~8 is appropriate)
*   In environments with many large tables, set **max_parallel_workers** value high
*   In environments with many small tables, using default values is recommended
