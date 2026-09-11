-----------------------------
Performance Tuning
-----------------------------

Since stored procedures and stored functions are written by users, performance tuning may be necessary.
This chapter provides various guides to optimize the performance of stored procedures and stored functions.

.. contents::

SQL Query Optimization
==============================

The most common performance overhead in stored procedures and stored functions can occur from inefficient SQL statement execution.
You should sufficiently tune the SQL statements used in internal routines to prevent the following inefficiencies:

* **Table Full Scan**: Performance can degrade if the entire table is scanned without using an index.
* **Communication Overhead**: To reduce communication overhead between the CUBRID database server and the CUBRID PL server, you should write queries to return the minimum number of records.
* **Inefficient Query Calls within Loops**: When using loops within stored procedures and stored functions, it is recommended to fetch data with a single query instead of calling multiple queries within the loop.

To tune inefficient queries used in stored procedures and stored functions, refer to the :doc:`/sql/tuning` document for query optimization.

Optimization of Stored Function Calls in Queries
==================================================

Unnecessary repeated calls to stored functions executed in queries can degrade performance. Therefore, consider the following methods to optimize stored function calls:

* **Minimize Unnecessary Repeated Calls**: Reducing the number of calls to stored functions is the best way to improve performance.
   * Use indexes to reduce the number of records for which the stored function is called.
   * Group duplicate data to handle it with a single call for avoiding repeated calls for the same arguments.
   * If the function logic is deterministic, use :ref:`pl-deterministic`\ to improve performance by caching the results of correlated subqueries.


* **Minimize the Size of Function Arguments and Return Values**: Design the function to return only the necessary values, avoiding the return of unnecessarily large data.

* **Use Parallel Execution**: Stored functions that are safe to execute concurrently can be declared with :ref:`pl-parallel-enable` to allow parallel execution.

.. _pl-use-builtin:

Use Built-in Functions
===============================

Built-in functions provided by CUBRID (:doc:`/sql/function/index`) are optimized to work efficiently with CUBRID's query execution behavior.
On the other hand, user-written stored functions are **black boxes** whose internal routines are unknown, and thus may perform worse than built-in functions.
Therefore, it is recommended to use built-in functions rather than implementing stored functions that can be achieved with simple combinations of built-in functions.

Below is an example of a stored function that performs the same function as the built-in CONCAT function.
Using the built-in function concat() has performance advantages over the user-defined stored function my_concat().

.. code-block:: sql

        CREATE OR REPLACE FUNCTION my_concat (a STRING, b STRING) RETURN STRING AS
        BEGIN
          RETURN a || b;
        END;

        SELECT COUNT(*) FROM (SELECT /*+ NO_MERGE */ concat (name, event) FROM athlete);

        SELECT COUNT(*) FROM (SELECT /*+ NO_MERGE */ my_concat (name, event) FROM athlete);

::

        -- Using concat function
                      count(*)
        ======================
                          6677

        1 row selected. (0.019853 sec) Committed. (0.000000 sec)

        
        -- Using my_concat function
                      count(*)
        ======================
                          6677

        1 row selected. (0.302333 sec) Committed. (0.000000 sec)

.. _pl-deterministic:

Use Deterministic Functions
==============================

A deterministic function is a function that always returns the same result for the same arguments during a transaction. If a stored function is deterministic, its results can be reused to improve performance.

*   To make a stored function deterministic, you can specify the **DETERMINISTIC** option in the **CREATE FUNCTION** statement. For more details, refer to :ref:`create-function`.
*   When the **DETERMINISTIC** attribute is specified, the stored function can be used for correlated subquery result caching optimization and will always return the same result for the same arguments. For more details on correlated subquery cache behavior, refer to :ref:`correlated-subquery-cache`.

Below is an example of a stored function created as a deterministic function with **DETERMINISTIC**. This example shows the process of optimizing performance by caching results when using correlated subqueries.

.. code-block:: sql

    CREATE TABLE dummy_tbl (col1 INTEGER);
    INSERT INTO dummy_tbl VALUES (1), (2), (1), (2);

    CREATE OR REPLACE FUNCTION pl_csql_not_deterministic (n INTEGER) RETURN INTEGER AS
    BEGIN
      return n + 1;
    END;

    CREATE OR REPLACE FUNCTION pl_csql_deterministic (n INTEGER) RETURN INTEGER DETERMINISTIC AS
    BEGIN
      return n + 1;
    END;

    SELECT sp_name, owner, sp_type, is_deterministic from db_stored_procedure;

::
    
    sp_name                      owner           sp_type               is_deterministic    
 ========================================================================================
    'pl_csql_not_deterministic'  'DBA'           'FUNCTION'            'NO'                
    'pl_csql_deterministic'      'DBA'           'FUNCTION'            'YES' 

In the above example, the pl_csql_not_deterministic function does not use query caching in correlated subqueries because it is **NOT DETERMINISTIC**.
On the other hand, the pl_csql_deterministic function uses query caching in correlated subqueries because it is specified with the **DETERMINISTIC** keyword, optimizing performance.

.. code-block:: sql
    
    ;trace on
    SELECT (SELECT pl_csql_not_deterministic (t1.col1) FROM dual) AS results FROM dummy_tbl t1;

::

      results
 =============
            2
            3
            2
            3
 
 === Auto Trace ===
    ...
    Trace Statistics:
      SELECT (time: 4, fetch: 11, fetch_time: 0, ioread: 0)
        FUNC (time: 4, fetch: 2, ioread: 0, calls: 4)
        SCAN (table: dba.dummy_tbl), (heap time: 0, fetch: 5, ioread: 0, readrows: 4, rows: 4)
        SUBQUERY (correlated)
          SELECT (time: 4, fetch: 6, fetch_time: 0, ioread: 0)
            FUNC (time: 4, fetch: 2, ioread: 0, calls: 4)
            SCAN (table: dual), (heap time: 0, fetch: 4, ioread: 0, readrows: 4, rows: 4)

The pl_csql_not_deterministic function does not cache the results of correlated subqueries because it is **NOT DETERMINISTIC**.

.. code-block:: sql
    
    ;trace on
    SELECT (SELECT pl_csql_deterministic (t1.col1) FROM dual) AS results FROM dummy_tbl t1;

::

      results
 =============
            2
            3
            2
            3

 === Auto Trace ===
    ...
    Trace Statistics:
      SELECT (time: 2, fetch: 9, fetch_time: 0, ioread: 0)
        FUNC (time: 2, fetch: 2, ioread: 0, calls: 2)
        SCAN (table: dba.dummy_tbl), (heap time: 0, fetch: 5, ioread: 0, readrows: 4, rows: 4)
        SUBQUERY (correlated)
          SELECT (time: 2, fetch: 4, fetch_time: 0, ioread: 0)
            FUNC (time: 2, fetch: 2, ioread: 0, calls: 2)
            SCAN (table: dual), (heap time: 0, fetch: 2, ioread: 0, readrows: 2, rows: 2)
            SUBQUERY_CACHE (hit: 2, miss: 2, size: 150808, status: enabled)

In the trace results of the pl_csql_deterministic function, the **SUBQUERY_CACHE** item is displayed (hit: 2, miss: 2, size: 150808, status: enabled), and the number of records read (**readrows**) in the **SCAN (table: dual)** at the top is reduced compared to the **NOT DETERMINISTIC** example.

.. warning::

        * The **DETERMINISTIC** attribute is not supported in stored procedures.
        * If a function that returns non-deterministic results is used the **DETERMINISTIC** option, it may not return the expected results.

.. code-block:: sql

        CREATE TABLE test_table (
          id INT AUTO_INCREMENT PRIMARY KEY,
          name VARCHAR(100)
        );

        INSERT INTO test_table (name) VALUES 
        ('Alice'),
        ('Bob'),
        ('Charlie'),
        ('Alice'),
        ('Bob');

        CREATE SERIAL my_serial;
        CREATE OR REPLACE FUNCTION cnt_name (name VARCHAR) RETURN VARCHAR DETERMINISTIC AS BEGIN RETURN name || my_serial.NEXT_VALUE; END;

        SELECT 
        id,
        name,
          (SELECT cnt_name(name) FROM DUAL) AS result
        FROM test_table;

::

                id  name                  result              
        =========================================================
                1  'Alice'               'Alice1'            
                2  'Bob'                 'Bob2'              
                3  'Charlie'             'Charlie3'          
                4  'Alice'               'Alice1'            
                5  'Bob'                 'Bob2
        
In the above example, the my_serial.NEXT_VALUE inside the cnt_name function returns non-deterministic results, so it returns unexpected results due to the correlated subquery cache.
It is recommended to specify the **DETERMINISTIC** option considering the implementation of the stored function.

.. _pl-parallel-enable:

Use Functions That Can Run in Parallel
-----------------------------------------

**PARALLEL_ENABLE** is a property that allows parallel execution of Java and PL/CSQL stored functions. For the declaration syntax, refer to :ref:`create-function-parallel-enable`.

Functions declared with this property can run in :ref:`parallel-scan`, :ref:`parallel-subquery-execution`, and :ref:`parallel-hash-join`. The declaration alone does not guarantee parallel execution. The applicability conditions of each feature and :ref:`parallel-query-throughput-rules` must also be satisfied. If the part to be executed in parallel also contains a stored function without this property or a method that does not support parallel execution, parallel execution of that part is restricted.

**PARALLEL_ENABLE** and :ref:`DETERMINISTIC <pl-deterministic>` are independent properties and can be specified together. **DETERMINISTIC** alone does not allow parallel execution, and **PARALLEL_ENABLE** alone does not make the function deterministic.

**Declaration Precautions**

CUBRID does not verify that the function body is safe for parallel execution. Users must ensure that the function returns correct results when executed concurrently. Do not specify this property for functions that use mutable Java static variables or shared state, or whose results depend on the number or order of calls.

**Server Access Restrictions**

Functions declared with this property cannot use server-side SQL or **DBMS_OUTPUT**. The same restrictions apply when parallel execution is not selected and when the function is called serially with **CALL**.

*   **PL/CSQL**: Statements requiring a server connection and **DBMS_OUTPUT** calls are rejected at compilation time during **CREATE FUNCTION** or **CREATE OR REPLACE FUNCTION**. Built-in functions evaluated by the server, cursors, serials, static SQL, dynamic SQL, calls to other stored functions and procedures, **COMMIT**, and **ROLLBACK** are not allowed. These constructs are rejected even in branches that are not executed or in exception handlers.
*   **Java**: Obtaining the server-side default connection through ``jdbc:default:connection`` or accessing the server through an OID raises **SQLException**. Calling **DBMS_OUTPUT** directly raises **RuntimeException**. If the exception is not handled, the function call fails. If the function handles the exception and returns a value, that value is used.

Java functions can pass OIDs as arguments or return values and obtain their string representations with ``getOidString()``. However, server access to read or modify the objects referenced by OIDs is not allowed.

External JDBC connections (``jdbc:cubrid://...``) from Java functions are allowed because they use separate sessions and transactions. These connections do not share uncommitted changes or SQL session variables with the query that calls the function.

**Checking the Declaration and Execution**

In :ref:`db-stored-procedure` and :ref:`information-schema-routines`, **is_parallel_enabled** is **YES** for declared functions and **NO** otherwise. This value indicates the declaration, not whether the actual execution plan is parallel.

Use :ref:`query-profiling` to check whether execution is parallel. **FUNC** statistics also include stored function calls executed by parallel workers.

**Example**

The following example calls a stored function that performs a pure calculation in a parallel scan. It was run in an environment with two parallel workers available. The table created in this run has 3,281 heap pages.

.. code-block:: sql

    -- Prepare data for a parallel scan.
    CREATE OR REPLACE FUNCTION parallel_inc (n INTEGER) RETURN INTEGER
    PARALLEL_ENABLE
    AS
    BEGIN
        RETURN n + 1;
    END;
    CREATE TABLE parallel_data (id INTEGER, pad VARCHAR(200));
    INSERT INTO parallel_data
    SELECT ROWNUM, RPAD('x', 200, 'x')
    FROM db_class a, db_class b, db_class c, db_class d
    LIMIT 200000;
    UPDATE STATISTICS ON parallel_data WITH FULLSCAN;

The following query specifies a parallelism degree of 2 with the **PARALLEL(2)** hint.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ RECOMPILE PARALLEL(2) */ COUNT(*) AS total
    FROM parallel_data
    WHERE parallel_inc(id) > 0;

::

    Trace Statistics:
      SELECT (time: 4135, fetch: 3286, fetch_time: 7, ioread: 0)
        FUNC (time: 7502, fetch: 1, ioread: 0, calls: 200000)
        SCAN (table: dba.parallel_data), (heap time: 4134, fetch: 3283, ioread: 0, readrows: 200000, rows: 200000)
             (parallel workers: 2, heap time: 4038..4134, readrows: 98496..101504, rows: 98496..101504, gather: buildvalue)

Running the same query serially with the **PARALLEL(1)** hint produces the following trace.

.. code-block:: sql

    csql> ;trace on

    SELECT /*+ RECOMPILE PARALLEL(1) */ COUNT(*) AS total
    FROM parallel_data
    WHERE parallel_inc(id) > 0;

::

    Trace Statistics:
      SELECT (time: 6619, fetch: 3285, fetch_time: 3, ioread: 0)
        FUNC (time: 6140, fetch: 1, ioread: 0, calls: 200000)
        SCAN (table: dba.parallel_data), (heap time: 6560, fetch: 3283, ioread: 0, readrows: 200000, rows: 200000)

Both queries return **200000**. The parallel trace shows **parallel workers: 2**, and **calls: 200000** in **FUNC** includes the function calls executed by both workers.
