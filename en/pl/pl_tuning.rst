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

For inefficient queries used in stored procedures and stored functions, refer to the :doc:`/sql/tuning` document for query optimization.

Optimization of Stored Function Calls in Queries
==================================================

Unnecessary repeated calls to stored functions executed in queries can degrade performance. Therefore, consider the following methods to optimize stored function calls:

* **Minimize Unnecessary Repeated Calls**: Reducing the number of calls to stored functions is the best way to improve performance.
   * Use indexes to reduce the number of records for which the stored function is called.
   * Group duplicate data to handle it with a single call, preventing repeated calls for the same arguments.
   * If the function logic is deterministic, use the :ref:`pl-deterministic`\ to improve performance by caching the results of correlated subqueries.

* **Minimize the Size of Function Arguments and Return Values**: Design the function to return only the necessary values, avoiding the return of unnecessarily large data.

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

                      count(*)
        ======================
                          6677

        1 row selected. (0.019853 sec) Committed. (0.000000 sec)

        SELECT COUNT(*) FROM (SELECT /*+ NO_MERGE */ my_concat (name, event) FROM athlete);
        
                      count(*)
        ======================
                          6677

        1 row selected. (0.302333 sec) Committed. (0.000000 sec)
::


.. _pl-deterministic:

Use Deterministic Functions
==============================

A deterministic function is a function that always returns the same result for the same arguments. If a stored function is deterministic, its results can be reused to improve performance.

*   To make a stored function deterministic, you can specify the **DETERMINISTIC** attribute in the **CREATE FUNCTION** statement. For more details, refer to :ref:`create-function`.
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
        * If the **DETERMINISTIC** attribute is used in internal implementations that return non-deterministic results, it may not return the expected results.

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
It is recommended to specify the **DETERMINISTIC** attribute considering the implementation of the stored function.
