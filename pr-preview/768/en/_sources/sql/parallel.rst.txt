
.. _parallel-query:

Parallel Execution
==================

CUBRID provides parallel query execution capabilities to efficiently process large amounts of data. Parallel query execution divides a single query into multiple work units, which are then processed by multiple worker threads simultaneously, dramatically reducing response time.

Overview
--------

Parallel queries provide the following key features:

*   **Parallel Scan**: Multiple worker threads divide and scan the input data (heap, temporary list, or index), improving large input scanning performance. Three scan flavors are supported:

    *   **Parallel Heap Scan**: Heap pages of a table are partitioned by **sector** and scanned in parallel.
    *   **Parallel List Scan**: A temporary result list (list file) produced by a subquery or other intermediate operation is partitioned by **sector** and scanned in parallel.
    *   **Parallel Index Scan**: Workers cooperate through a shared cursor to walk the leaf pages of a B+tree index from left to right (or right to left).

*   **Parallel Subquery Execution**: Independent subqueries (uncorrelated subqueries) are processed simultaneously by individual workers, improving query response time.
*   **Parallel Hash Join**: Parallelizes both the build and probe phases, improving response time during hash join operations.
*   **Parallel Sort**: Divides data to be sorted among multiple worker threads, sorts in parallel, then merges the results, improving sort response time.

Configuration
^^^^^^^^^^^^^

Parallel query execution can be controlled through system parameters and SQL hints.

*   Setting the :ref:`parallelism <parallelism>` parameter to 2 or higher enables the optimizer to determine parallel query execution during query processing.
*   Use the **PARALLEL** ( *degree* ) hint to explicitly specify the degree of parallelism for each query. *degree* is the number of workers to use and must be an integer value of 0 or higher. A value of **0** or **1** disables parallel execution; a value of **2 or higher** takes precedence over the parallelism parameter setting. A value larger than the number of system CPU cores is lowered to the core count.
*   The :ref:`max_parallel_workers <max_parallel_workers>` parameter sets the maximum number of parallel worker threads that can be executed simultaneously across the entire server (default: 100).
*   The **NO_PARALLEL_SCAN** hint disables every parallel scan flavor (heap, list, and index) within the query block. When used together with the **PARALLEL** hint, **NO_PARALLEL_SCAN** takes precedence.
*   Parallel hash join and parallel subquery execution can be disabled with the **NO_PARALLEL_HASH_JOIN** and **NO_PARALLEL_SUBQUERY** hints, respectively. See the corresponding sections for details.

.. note::

    The max_parallel_workers and parallelism parameters are set to default values of 100 and 4 respectively, so you can use parallel queries without additional configuration. However, parallel query execution is disabled entirely when the system has 2 or fewer CPU cores.

.. note::

    Under parallel execution, the order of result rows can change from run to run depending on the order in which workers finish. Specify an ORDER BY clause whenever the result order matters.

.. _parallel-scan:

Parallel Scan
-------------

Parallel Scan splits a single scan input across multiple worker threads that process it concurrently. CUBRID supports parallel scan over three input kinds, all sharing the same parallel execution framework:

*   **Heap**: heap pages of a table — pre-partitioned across workers by **sector**
*   **List**: pages of a temporary result list file — pre-partitioned across workers by **sector**
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

*   The scan is over a table other than the driving (first) table of a nested loop join.
    Parallel processing of a nested loop join enters through the parallel scan of the driving table,
    so if the driving table does not qualify for parallel scan, the entire join runs single-threaded
    even when the subsequent tables are large. In that case, you can induce parallel execution by
    adjusting the join order so that a large table drives the join (the **ORDERED** hint) or by
    forcing a hash join (the **USE_HASH** hint).
*   Scans inside a correlated subquery. Scans inside an uncorrelated subquery can still be parallelized.
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
    WHERE id <= 10
    FOR UPDATE;

    -- Using session variables
    SET @user_id = 123;
    SELECT /*+ PARALLEL(4) */ *
    FROM orders
    WHERE cust_id = @user_id;

    -- Using SERIAL
    SELECT /*+ PARALLEL(4) */ id, order_seq.NEXT_VALUE
    FROM large_table
    WHERE id <= 3;

.. _parallel-heap-scan:

Parallel Heap Scan
^^^^^^^^^^^^^^^^^^

Parallel Heap Scan statically partitions the heap pages of a table by sector and lets workers scan their partitions concurrently. It can yield a large speedup over single-threaded heap scan, especially when selectivity is low (typically 0.05 or less).

Heap scan has no additional flavor-specific restrictions beyond the :ref:`common constraints <parallel-scan>` listed above.

.. code-block:: sql

    -- Parallel heap scan
    SELECT /*+ PARALLEL(4) */ id, category
    FROM large_table
    WHERE status = 'active';

    -- Parallel heap scan over a partitioned table
    SELECT /*+ PARALLEL(4) */ *
    FROM sales_partitioned
    WHERE sale_date BETWEEN '2024-01-01' AND '2024-12-31';

    -- INSERT SELECT (bulk copy)
    INSERT INTO archive_orders
    SELECT /*+ PARALLEL(4) */ *
    FROM orders
    WHERE amount < 100;

.. note::

    For a partitioned table, parallelization is decided per partition, and the activation condition (:ref:`parallel-query-throughput-rules`) is also evaluated against each partition's own page count. A partition below the activation condition is scanned single-threaded.

.. _parallel-list-scan:

Parallel List Scan
^^^^^^^^^^^^^^^^^^

Parallel List Scan statically partitions a temporary on-disk result list (list file) — produced by a subquery, derived table, or other intermediate operator — across workers by **sector**, and each worker reads its share concurrently. The partitioning mechanism itself is identical to parallel heap scan; the only difference is that the input is a temporary file rather than a table heap. It is effective when an upper operator must rescan a large intermediate result.

**Additional list-scan constraints**

Parallel list scan is not applied — and falls back to a single-threaded list scan — if any of the following hold:

*   The temporary list resides only in the in-memory buffer and has not spilled to a disk temp file (no sectors to partition — small lists fall back to single-threaded execution automatically).
*   The upper operator consumes results in row-by-row mode (a query shape that admits neither mergeable list nor BUILDVALUE; see :ref:`result-collection-modes`).
*   A ROWNUM or LIMIT predicate is attached to the list scan.
*   The list scan sits inside the auxiliary input subtree (subquery, CTE, etc.) of a sort-merge join.

.. code-block:: sql

    -- Typical pattern that benefits from parallel list scan:
    -- the temporary result list of a derived table on which
    -- View Merging is not performed is
    -- re-aggregated by the outer query.
    SELECT /*+ PARALLEL(4) */ COUNT(*)
    FROM (SELECT DISTINCT id, pad FROM large_table) t;

.. note::

    A derived table that is a simple projection is merged into the outer query by the **View Merging** optimization, so no temporary result list is created in the first place. A temporary result list is created for a derived table on which **View Merging** cannot be performed because it contains DISTINCT, GROUP BY, UNION, and the like; the **NO_MERGE** hint can also be used to block **View Merging** and force the list to be created. Rescanning a hash join's input list from an upper operator is another typical target of parallel list scan. For more details on **View Merging**, see :ref:`view_merge`.

.. _parallel-index-scan:

Parallel Index Scan
^^^^^^^^^^^^^^^^^^^

Parallel Index Scan lets multiple workers cooperatively walk the leaf pages of a B+tree index through a shared cursor. The vertical descent (root → leaf entry) is performed serially by the main thread; the subsequent leaf traversal, OID fetching, and predicate evaluation are parallelized across workers. Each worker grabs one leaf page, processes its keys independently, and only briefly synchronizes to obtain the next leaf.

**Additional index-scan constraints**

Parallel index scan is not applied — and falls back to a single-threaded index scan — if any of the following hold:

*   The scan uses an index-driven traversal optimization that changes how the tree is entered or walked:

    *   ISS (Index Skip Scan)
    *   ILS (Index Loose Scan)
    *   KEYLIMIT clause
    *   ORDERBY_SKIP / GROUPBY_SKIP family optimizations (replacing ORDER BY / GROUP BY with the index order)
    *   MIN/MAX single-key scan (min_max scan)

*   ROWNUM is used in a form that cannot be recomputed per worker, or an analytic-function SKIP SORT / LIMIT optimization is applied.
*   The upper operator consumes results in row-by-row mode (a query shape that admits neither mergeable list nor BUILDVALUE; see :ref:`result-collection-modes`).
*   The index scan sits inside the auxiliary input subtree (subquery, CTE, etc.) of a sort-merge join.

.. code-block:: sql

    -- Typical case where parallel index scan applies
    -- (covering index scan over a wide range)
    CREATE INDEX idx_large_id_pad ON large_table(id, pad);

    SELECT /*+ PARALLEL(4) */ COUNT(pad)
    FROM large_table
    WHERE id BETWEEN 1 AND 900000 USING INDEX idx_large_id_pad;

    -- Parallel index scan is NOT applied
    -- (KEYLIMIT clause forces single-threaded index scan)
    SELECT /*+ PARALLEL(4) */ id, category
    FROM large_table
    WHERE id BETWEEN 1 AND 900000 USING INDEX idx_large_id_pad KEYLIMIT 100;

.. note::

    Because the tree descent is serial on the main thread and the leaf traversal is cooperative, parallel index scan pays off when the index has enough leaf pages. On small indexes the synchronization cost can outweigh the speedup; the throughput rules (:ref:`parallel-query-throughput-rules`) guard against this.

.. note::

    For an index scan to be parallelized automatically without the **PARALLEL** hint, the optimizer additionally requires both of the following.

    *   The selectivity of every key range predicate must be derivable from **histogram statistics**. If the target table has no histogram statistics, the optimizer never chooses parallel index scan. Statistics can be refreshed with the **UPDATE STATISTICS** statement.
    *   The product of the selectivity and the number of index pages must reach a threshold (32 pages by default).

    The **PARALLEL** hint bypasses these optimizer conditions, but the scan still runs single-threaded if the measured number of index pages does not meet the activation condition (:ref:`parallel-query-throughput-rules`).

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

*   **mergeable list**: each worker builds its own temporary result list and the main thread uses those lists directly without merging. This has the lowest synchronization cost and is usually the fastest mode.
*   **buildvalue**: each worker computes a partial aggregate and the main thread combines the partials into the final aggregate. Used for simple aggregate queries (see :ref:`buildvalue-optimization`).
*   **row-by-row**: the main thread receives one row at a time. Applies when neither of the other two modes can be used. It has the broadest applicability but the highest synchronization cost.

.. note::

    The row-by-row mode is observed **only with parallel heap scan**. Parallel list scan and parallel index scan fall back to single-threaded execution for query shapes that would require row-by-row (see the additional constraints in their respective sections), so ``gather: row by row`` only appears in heap-scan traces.

**When mergeable list is not chosen**

Mergeable list is replaced by another mode (buildvalue or row-by-row) if any of the following hold:

*   The scan carries predicates that cannot be evaluated while scanning (deferred to an upper operator).
*   The select-list contains a stored procedure (JavaSP or PL/CSQL).
*   ROWNUM or LIMIT is used in a form that cannot be recomputed per worker.
*   The scan does not output any table column directly (e.g., a query selecting only constants).
*   result_cache is enabled.

.. note::

    A top-N query combining ORDER BY with LIMIT can also run in parallel with mergeable list. In that case each worker keeps its own top N while scanning, and **topnsort: true** appears in the parallel processing details of the trace. When GROUP BY is combined, the workers perform partial hash aggregation and the **GROUPBY** entry of the trace shows **hash: partial**.

.. _buildvalue-optimization:

BUILDVALUE Optimization
^^^^^^^^^^^^^^^^^^^^^^^

When a query computes aggregate functions without GROUP BY and every aggregate function it uses is on the supported list, parallel scan applies the **BUILDVALUE optimization**. In this mode, each worker computes a partial aggregate over its scanned region and ships it to the main thread, which then combines the partials into the final result. Because workers exchange the smallest possible amount of data, this is the fastest mode for aggregate queries.

**Supported aggregate functions**

The BUILDVALUE optimization applies to queries using the following aggregate functions. The SELECT list may also combine these aggregate functions in arithmetic expressions.

*   **COUNT(\*)**, **COUNT(column)**, **COUNT(DISTINCT column)**
*   **MIN**, **MAX**
*   **SUM**, **AVG**
*   **STDDEV**, **STDDEV_POP**, **STDDEV_SAMP**
*   **VARIANCE**, **VAR_POP**, **VAR_SAMP**
*   **BIT_AND**, **BIT_OR**, **BIT_XOR**
*   **GROUP_CONCAT**, **JSON_ARRAYAGG**, **JSON_OBJECTAGG**
*   **MEDIAN**, **PERCENTILE_CONT**, **PERCENTILE_DISC**
*   **CUME_DIST**, **PERCENT_RANK**

**Conditions**

In addition to using only the supported aggregate functions, all of the following must hold:

*   The query has no GROUP BY clause.
*   The query does not use ROWNUM.

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

    If any unsupported aggregate function is included or GROUP BY is combined, the BUILDVALUE optimization is not applied and the query is processed in the mergeable list or row-by-row mode instead.

Scan SQL Trace
^^^^^^^^^^^^^^

When parallel scan is performed, parallel processing details are added to the :ref:`SQL trace <query-profiling>` output.

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

The parallel scan trace fields are:

*   **parallel workers**: number of worker threads used.
*   **heap time / temp time / index time**: per-worker scan time range (min..max, milliseconds). The label is **heap time** for heap scan, **temp time** for list scan (the input is a temporary list file), and **index time** for index scan.
*   **readrows**: per-worker range of rows read (min..max).
*   **rows**: per-worker range of rows produced (min..max).
*   **gather**: how worker results were collected.

    *   **mergeable list**: per-worker lists are used directly without merging.
    *   **buildvalue**: per-worker partial aggregates are combined (replaces the legacy ``count`` label).
    *   **row by row**: rows are collected one at a time (heap scan only).

When **gather** shows **mergeable list** or **buildvalue**, the query took the lowest-synchronization path.

.. note::

    Per-worker times and row counts appear as min..max ranges. Ideally all workers do similar amounts of work; a wide range hints at uneven data distribution or system resource contention.

**BUILDVALUE optimization trace example**

When BUILDVALUE optimization is applied, **gather: buildvalue** is shown.

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

**Parallel index scan trace example**

The parallel processing details of a parallel index scan report **index time** along with the per-worker ranges of **readkeys** (keys read) and **filteredkeys** (keys passing the key filter). A covering index scan additionally shows **covered: true**.

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

For a non-covering index scan, the parallel processing details are followed by data-page lookup statistics of the form **(lookup time: min..max, rows: min..max)**.

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

.. _parallel-hash-join:

Parallel Hash Join
------------------

Parallel Hash Join improves join response time by parallelizing the build and probe phases of a hash join across multiple worker threads. A hash join first loads both inputs into temporary result lists, builds a hash table from one input (build), and searches it with the other input (probe). Parallel hash join takes one of the following two forms depending on the input sizes.

*   **Partitioned parallel**: both inputs are split into multiple partitions by the join key (SPLIT), and workers run the per-partition build and probe concurrently. Used when the build input is large enough to require partitioning.
*   **Parallel probe**: when the build input is small enough to build the hash table in memory at once, the build runs single-threaded and only the probe input is divided among workers.

**Activation conditions**

*   The larger of the two input lists must satisfy the activation condition (:ref:`parallel-query-throughput-rules`) in page count. Below it, the hash join runs single-threaded even with the **PARALLEL** hint.
*   When the **NO_PARALLEL_HASH_JOIN** hint is specified, the hash join is not parallelized. The hash join itself is kept; only its parallelization is disabled. See :ref:`NO_PARALLEL_HASH_JOIN <no-parallel-hash-join>` for details.

Hash Join Trace
^^^^^^^^^^^^^^^

When a hash join is performed, a **HASHJOIN** tree is printed in the :ref:`SQL trace <query-profiling>` output. The tree shows per-phase statistics for partitioning (**SPLIT**), build (**BUILD**), and probe (**PROBE**), and this tree structure is always printed for a hash join regardless of parallelism. When parallel hash join is applied, a **parallel workers** field is added to the **HASHJOIN** entry, and the per-partition parallel phase is shown as a **PARALLEL** entry.

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

The parallelism-related fields of the hash join trace are:

*   **parallel workers** on **HASHJOIN**: number of worker threads used by the hash join. If absent, the hash join ran single-threaded.
*   **SPLIT**: the phase that splits both inputs into partitions by the join key. **partitions** is the number of partitions created.
*   **PARALLEL**: the phase in which workers run per-partition build and probe concurrently. The **time** of the **BUILD** and **PROBE** entries below it is reported as a per-worker range (min..max).
*   **SUBQUERY (uncorrelated)**: the phase that loads the two join inputs into temporary result lists. Loading the inputs is itself subject to parallel subquery execution and parallel scan.

The following example disables parallelization with the **NO_PARALLEL_HASH_JOIN** hint. The **HASHJOIN** entry has no **parallel workers** field and **BUILD**/**PROBE** run single-threaded without a **PARALLEL** entry, while the scans loading the join inputs can still run in parallel independently.

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

The following example runs in the **parallel probe** form because the build input is small. **BUILD** runs single-threaded (**method: memory**), and parallel processing details with per-worker ranges are printed under the **PROBE** entry.

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

Parallel Sort
-------------

Parallel Sort improves sort response time by distributing the sort input among multiple worker threads, letting each sort its share, and merging the results. Parallel sort can be applied to the following sort operations.

*   File sort (filesort) of an ORDER BY clause
*   Partitioning/sorting for analytic functions
*   The sort of a GROUP BY to which hash aggregation cannot be applied (e.g., when a DISTINCT aggregate function is included)
*   The internal sort for DISTINCT deduplication

**Activation conditions**

*   The page count of the sort input must satisfy the activation condition (:ref:`parallel-query-throughput-rules`).
*   If the input has no more rows than the degree of parallelism, the sort runs single-threaded.

.. note::

    A top-N query combining ORDER BY with LIMIT does not go through a separate parallel sort phase; it is parallelized by the parallel scan workers each keeping their own top N while scanning. In the trace, **topnsort: true** appears in the scan's parallel processing details and the **ORDERBY** phase performs only the final merge.

.. note::

    When a GROUP BY query is eligible for hash aggregation, the workers perform partial hash aggregation instead of parallelizing the sort, and the **GROUPBY** entry of the trace shows **hash: partial**. When hash aggregation is disabled with the **NO_HASH_AGGREGATE** hint, the GROUP BY sort is not parallelized.

.. note::

    Parallel sort uses per-worker temporary sort space, so temporary volume usage can be higher than with a single-threaded sort.

Sort Trace
^^^^^^^^^^

When a parallel sort is performed, parallel processing details showing the per-worker ranges of elapsed time, page count, and I/O reads are additionally printed under the corresponding sort entry (**ORDERBY**, **GROUPBY**, or **ANALYTIC**).

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

The trace fields of parallel sort are:

*   **parallel workers**: number of worker threads used for the sort
*   **time**: per-worker sort time range (min..max, milliseconds)
*   **page**, **ioread**: per-worker ranges (min..max) of pages used and I/O reads issued during the sort

The following example shows an analytic function whose sort was parallelized.

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

The following example shows a GROUP BY whose sort was parallelized because a DISTINCT aggregate function makes hash aggregation inapplicable.

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

The degree of parallelism for parallel heap scan and parallel list scan is determined by the same rule based on the page count of the scan input (the table's heap page count for heap scan, the temporary list page count for list scan). Index scan follows a separate rule (see below).

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

**Index scan throughput rule**

The automatic degree of parallelism for parallel index scan is computed by the optimizer from **selectivity × index page count**. The scan becomes a candidate when this value is 32 or more; the degree starts at 2 and increases by 1 each time the value doubles from 32, and it cannot exceed the :ref:`parallelism <parallelism>` parameter value. The **PARALLEL** hint bypasses this optimizer computation, but the activation condition that the measured index page count must be 2,048 or more still applies.

**Example**

.. code-block:: sql

    -- Create table and insert data
    CREATE TABLE large_table (id INT PRIMARY KEY, category INT, status VARCHAR(10), pad VARCHAR(200));

    INSERT INTO large_table
    SELECT ROWNUM, MOD(ROWNUM, 100),
           CASE WHEN MOD(ROWNUM, 2) = 0 THEN 'active' ELSE 'closed' END,
           LPAD('x', 200, 'x')
    FROM db_class a, db_class b, db_class c, db_class d
    LIMIT 1000000;

    UPDATE STATISTICS ON large_table WITH FULLSCAN;

    -- Check table statistics (SHOW HEAP CAPACITY OF large_table)
    -- Num_pages: 17245 (approximately 269MB when db_page_size is 16K)
    -- Num_recs: 1000000

    -- When the parallelism parameter is set to 4
    -- Page count 17245 -> throughput rule calculates 5 -> MIN(5, 4) = 4 applied
    SELECT COUNT(*) FROM large_table WHERE status = 'active';

    -- Explicit specification with hint (the hint value wins once the activation condition is met)
    SELECT /*+ PARALLEL(8) */ COUNT(*) FROM large_table WHERE status = 'active';

Hash Join Throughput Rules
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The degree of parallelism for parallel hash join is computed from the page count of the **larger** of the two input lists, by the same rule as for scans (activation at 2,048 pages, degree +1 per doubling of the page count, capped by :ref:`parallelism <parallelism>`).

*   The computed degree of parallelism cannot exceed the number of partitions.
*   Below the activation condition, the hash join runs single-threaded even with the **PARALLEL** hint.

Sort Throughput Rules
^^^^^^^^^^^^^^^^^^^^^

The degree of parallelism for parallel sort is computed from the page count of the sort input list, by the same rule as for scans (activation at 2,048 pages, degree +1 per doubling of the page count, capped by :ref:`parallelism <parallelism>`).

*   If the input has no more rows than the computed degree of parallelism, the sort runs single-threaded.
*   Below the activation condition, the sort runs single-threaded even with the **PARALLEL** hint.

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
*   Each parallel operation reserves the required number of parallel workers from the parallel worker pool before execution, and returns them after task completion
*   If the pool cannot grant the full request due to contention, the operation runs in parallel with only the workers actually reserved; if no worker is reserved, it runs single-threaded
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
