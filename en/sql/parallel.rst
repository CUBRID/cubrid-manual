
.. _parallel-query:

Parallel Execution
==================

CUBRID provides parallel query execution capabilities to efficiently process large amounts of data. Parallel query execution divides a single query into multiple work units, which are then processed by multiple worker threads simultaneously, dramatically reducing response time.

Overview
--------

Parallel queries provide the following key features:

*   **Parallel Heap Scan**: Multiple worker threads divide and scan heap regions, improving large table scanning performance.
*   **Parallel Subquery Execution**: Independent subqueries (uncorrelated subqueries) are processed simultaneously by individual workers, improving query response time.
*   **Parallel Hash Join**: Parallelizes both the build and probe phases, improving response time during hash join operations.
*   **Parallel Sort**: Divides data to be sorted among multiple worker threads, sorts in parallel, then merges the results, improving sort response time.

Configuration
^^^^^^^^^^^^^

Parallel query execution can be controlled through system parameters and SQL hints.

*   Setting the :ref:`parallelism <parallelism>` parameter to 2 or higher enables the optimizer to determine parallel query execution during query processing.
*   Use the **PARALLEL** ( *degree* ) hint to explicitly specify the degree of parallelism for each query. *degree* is the number of workers to use and must be an integer value of 2 or higher. Hint-specified values take precedence over the parallelism parameter setting.
*   The :ref:`max_parallel_workers <max_parallel_workers>` parameter sets the maximum number of parallel worker threads that can be executed simultaneously across the entire server (default: 100).

.. note::

    The max_parallel_workers and parallelism parameters are set to default values of 100 and 4 respectively, so you can use parallel queries without additional configuration.

.. _parallel-heap-scan:

Parallel Heap Scan
------------------

Parallel Heap Scan is a feature that improves heap table scanning performance by using multiple worker threads when scanning large amounts of data. Performance can be significantly improved over single-threaded heap scanning, especially when selectivity is low (typically 0.05 or less) and processing large amounts of data.

Heap Scan Overview
^^^^^^^^^^^^^^^^^^

Parallel heap scan divides large tables into logical units for simultaneous scanning by multiple worker threads, with each worker thread independently scanning assigned pages while processing filter conditions (predicates). The processed results are collected through a result queue, and the main thread integrates these results to generate the final result and returns it to the user.

The **NO_PARALLEL_HEAP_SCAN** hint can be used to disable parallel heap scan. When used together with the **PARALLEL** hint, the **NO_PARALLEL_HEAP_SCAN** hint takes precedence.

.. note::

    The actual degree of parallelism for parallel heap scan is automatically optimized by throughput rules within the user-configured upper limit. For more details, see :ref:`parallel-query-throughput-rules`.

Constraints
^^^^^^^^^^^

If any of the following conditions apply, parallel heap scan is not supported and executes in single-threaded mode:

*   Statements that do not support concurrent processing

    *    When using stored procedures (JavaSP, PL/CSQL)

    *    When referencing session variables
  
    *    When using Recursive CTE or Connect By clauses
  
    *    When using CUBRID object DBMS specific features

*   Cases requiring exclusive lock (X-LOCK) acquisition

    *    SELECT ... FOR UPDATE clause
    *    When using incr() function
    *    update, delete, merge statements

*   When not the first (driving) table in a JOIN
*   When it is a correlated subquery
*   When reading data through index scan

.. note::

    The default values of the max_parallel_workers and parallelism parameters provide defaults that allow parallel queries without additional settings. Performance can be further optimized by modifying these values in the cubrid.conf file according to system resources and application workload. ::

        # cubrid.conf
        max_parallel_workers=200  # default: 100
        parallelism=8             # default: 4

.. code-block:: sql

    -- Examples where parallel heap scan is not applied

    -- Disabled by hint
    SELECT /*+ NO_PARALLEL_HEAP_SCAN */ * 
    FROM large_table;

    -- When using index scan
    SELECT /*+ PARALLEL(4) */ * 
    FROM large_table 
    WHERE indexed_column = 100 using index idx_large_table_indexed_column;

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

Heap Scan Performance Considerations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Parallel heap scan has significant performance improvements in the following cases:

*   When large table data needs to be scanned (more effective with more table pages)
*   When selectivity is low (approximately 0.05 or less)
*   When sufficient CPU cores are available
*   When CPU processing is the bottleneck rather than disk I/O

On the other hand, performance may degrade in the following cases:

*   When scanning small table data
*   When index scan is more efficient
*   When system resources (CPU, memory) are insufficient

When using parallel queries, the :ref:`max_parallel_workers <max_parallel_workers>` parameter should be set appropriately to prevent system resource contention. It is generally recommended to set it to the level of the actual physical CPU core count.

Heap Scan Optimization (Mergeable List)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Parallel heap scan operates with "mergeable list" optimization when certain conditions are met. In this method, each worker thread does not pass temporary results to the main thread but directly returns the final processed results to the main thread, significantly improving processing performance.

Especially when processing large amounts of data (approximately 10 million records or more) with 8 or more cores, it shows much faster performance than the row-by-row method (receiving results one by one from each thread).

**Constraints**

When the following conditions are met, the mergeable list optimization is not applied and row-by-row processing is used:

*   When the heap scan includes conditions that cannot be evaluated while scanning the target table
*   When performing hash aggregation (hash group by)
*   When there is a stored procedure (JavaSP or PL/CSQL) in the select-list
*   When ROWNUM is used
*   When performing topn_sort (sorting to extract top N)
*   When there is a LIMIT clause
*   When result_cache is enabled

**Representative Application Examples**

.. code-block:: sql

    -- Simple table full scan without join
    SELECT /*+ PARALLEL(8) */ *
    FROM large_table
    WHERE status = 'active';

    -- Table full scan followed by ORDER BY
    SELECT /*+ PARALLEL(8) */ *
    FROM large_table
    WHERE created_date > '2024-01-01'
    ORDER BY id;

    -- Parallel heap scan in uncorrelated subquery
    SELECT *
    FROM orders
    WHERE customer_id IN (
        SELECT /*+ PARALLEL(8) */ customer_id
        FROM customers
        WHERE region = 'Asia'
    );

    -- Apply parallel heap scan to each sub-SELECT in UNION statement
    SELECT /*+ PARALLEL(8) */ order_id, customer_id, order_date
    FROM orders_2023
    WHERE status = 'completed'
    UNION
    SELECT /*+ PARALLEL(8) */ order_id, customer_id, order_date
    FROM orders_2024
    WHERE status = 'completed';

    -- Parallel heap scan on partitioned table
    SELECT /*+ PARALLEL(8) */ *
    FROM sales_partitioned
    WHERE sale_date BETWEEN '2024-01-01' AND '2024-12-31';

    -- INSERT SELECT statement (copying large amounts of data)
    INSERT INTO archive_orders
    SELECT /*+ PARALLEL(8) */ *
    FROM orders
    WHERE order_date < '2023-01-01';

COUNT Optimization
""""""""""""""""""

Parallel heap scan provides special optimization mechanisms for **COUNT(\*)**, **COUNT(column)**, and **COUNT(DISTINCT column)** aggregate functions, which are the most frequently used. This method works by having each worker thread first calculate intermediate counts within their scanned range, and then finally summing the results.

**Conditions for COUNT Optimization**

COUNT-specific optimization is applied when all of the following conditions are met:

*   Contains only **COUNT(\*)**, **COUNT(column)**, or **COUNT(DISTINCT column)** aggregate functions
*   No ROWNUM or stored procedures in the condition clause
*   Simple query without other joins or subqueries

**COUNT Optimization Operation**

*   **COUNT(\*)**: Each worker increments a simple counter, and finally the main thread sums all worker counts
*   **COUNT(column)**: Each worker counts only non-NULL values, and finally the main thread sums all worker counts
*   **COUNT(DISTINCT column)**: Each worker stores values in a separate list file to remove duplicates and passes them on, and the main thread merges all lists received from workers to calculate the total DISTINCT count

**COUNT Optimization Examples**

.. code-block:: sql

    -- COUNT(*) optimization
    SELECT /*+ PARALLEL(8) */ COUNT(*)
    FROM large_table
    WHERE status = 'active';

    -- COUNT(column) optimization
    SELECT /*+ PARALLEL(8) */ COUNT(customer_id)
    FROM orders
    WHERE order_date > '2024-01-01';

    -- COUNT(DISTINCT) optimization
    SELECT /*+ PARALLEL(8) */ COUNT(DISTINCT customer_id)
    FROM orders;

    -- Usage in UPDATE STATISTICS
    UPDATE STATISTICS ON large_table WITH FULLSCAN;

.. note::

    COUNT optimization is a specialized optimization for simple aggregation. When used with other aggregate functions (SUM, AVG, etc.) or when complex joins are included, it is not applied and processing uses the general parallel heap scan method (mergeable list or row-by-row).

Heap Scan SQL Trace
^^^^^^^^^^^^^^^^^^^^

When parallel heap scan is performed, parallel processing details are additionally output in the :ref:`SQL trace <query-profiling>` results.

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

The description of parallel heap scan trace output items is as follows:

*   **parallel workers**: Number of worker threads used
*   **heap time**: Range of heap scan time for each worker (min..max, milliseconds)
*   **readrows**: Range of rows read by each worker (min..max)
*   **rows**: Range of rows returned by each worker (min..max)
*   **gather**: Result collection method
    
    * **mergeable list**: Optimized method that directly uses each worker's results without separate merging
    * **row-by-row**: Basic method that collects and merges each worker's results one by one
    * **count**: COUNT-specific optimization method where each worker performs local counting and merges final results

When the **gather** item shows **mergeable list** or **count**, it indicates that parallel heap scan optimization is applied, showing better performance.

.. note::

    The time and number of rows for parallel workers are displayed as ranges (min..max), and ideally all workers should perform similar amounts of work. If the range is wide, you may suspect data distribution or system resource contention issues.

**COUNT Optimization Trace Information Example**

When COUNT optimization is applied, **gather: count** is displayed:

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

COUNT optimization shows rows as 0 because the result is a single row, and the actual count result is returned through the aggregate function.

.. _parallel-subquery-execution:

Parallel Subquery Execution
----------------------------

Parallel Subquery Execution is a feature that improves query performance by using multiple worker threads to simultaneously execute subqueries that can run independently.

Subquery Execution Overview
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Since subqueries can be executed independently of other subqueries, when there are multiple subqueries, executing them in parallel can reduce overall query response time. Each subquery is executed in an independent worker thread, and when all subqueries are completed, the results are merged to generate the final result.

Parallel execution of subqueries is possible if the :ref:`parallelism <parallelism>` parameter is set to 2 or higher, or if the degree of parallelism is specified to 2 or higher using the **PARALLEL** ( *degree* ) hint.

The **NO_PARALLEL_SUBQUERY** hint can be used to disable parallel execution of subqueries.

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
    -- Two subqueries exist, but parallel execution is disabled by hint
    SELECT /*+ NO_PARALLEL_SUBQUERY */ *
    FROM orders
    WHERE customer_id IN (
        SELECT customer_id FROM customers WHERE region = 'Asia'
    )
    AND product_id IN (
        SELECT product_id FROM products WHERE category = 'Electronics'
    );

    -- When there are references between CTEs
    -- cte2 references cte1, so they are not independent
    WITH cte1 AS (
        SELECT * FROM table1
    ),
    cte2 AS (
        SELECT * FROM cte1 WHERE id > 100  -- references cte1
    )
    SELECT * FROM cte2;

    -- Using JSON_TABLE
    -- When JSON_TABLE is included, parallel execution is not applied even with 2+ subqueries
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
    -- Two subqueries exist, but one contains a stored procedure, so no parallel execution
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

*   Throughput rules such as table size and number of partitions
*   Values explicitly specified by **PARALLEL** hints
*   Upper limit set by the :ref:`parallelism <parallelism>` parameter
*   Global worker pool size set by the :ref:`max_parallel_workers <max_parallel_workers>` parameter

The degree of parallelism calculated by throughput rules cannot exceed the :ref:`parallelism <parallelism>` parameter value. The degree of parallelism specified by hints can exceed the :ref:`parallelism <parallelism>` parameter value but cannot exceed the maximum value (the smaller of 32 or the number of system cores).

Heap Scan Throughput Rules
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The degree of parallelism for parallel heap scan is determined by the number of pages in the target table to be scanned.

**Activation Condition**

*   Activated when the target table has 4,096 or more pages (approximately 64MB when db_page_size is 16K)
*   If this condition is not met, parallel heap scan is not activated even if the **PARALLEL** hint is present

**Degree Determination**

The degree of parallelism is determined according to the number of pages in the table as follows:

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

Starting from 2,048 pages, the degree of parallelism calculated by throughput rule increases by 1 each time the number of pages doubles from the previous increase threshold.

**The degree of parallelism determined by throughput rules cannot exceed the :ref:`parallelism <parallelism>` parameter value:**

*   **MIN (throughput rule calculation, parallelism parameter value)**

For example, when parallelism=4 (default):

*   Page count 4,096 → throughput rule calculates 2 → MIN(2, 4) = **2** applied

*   Page count 65,536 → throughput rule calculates 6 → MIN(6, 4) = **4** applied (cannot exceed parallelism)

.. note::

    When the degree of parallelism is explicitly specified using the **PARALLEL** hint, the throughput rules are not applied and the hint value is used.

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
    -- Page count 4215 is 4,096 or more, so degree of parallelism 2 is automatically applied
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

*   Prevents unnecessary parallel execution on small tables to reduce overhead
*   Automatically adjusts the degree of parallelism proportional to table size
*   Prevents system resource contention due to excessive parallel execution
*   Concentrates parallel resources on queries with significant benefits

Recommended Settings:

*   **max_parallel_workers**: Set considering the number of simultaneously executable parallel queries and the average degree of parallelism for each query
*   **parallelism**: Set considering the number of physical cores in the system (usually 4~8 is appropriate)
*   In environments with many large tables, set **max_parallel_workers** value high
*   In environments with many small tables, using default values is recommended


