***
CTE
***

Common Table Expressions (CTEs) are temporary tables (list of results) associated with a statement. A CTE can be referenced multiple times within the statement, and is visible only within the statement scope. It enables better separation of statement logic and may enhance execution performance. Moreover, recursive CTEs can be used to generate a hierarchical statement based on parent-child relationships, being able to reproduce CONNECT BY statements and other more complex queries. 

A CTE is introduced using the WITH clause. A list of sub-queries is expected, and final query which uses the sub-queries. Each sub-query (table expression) has a name and a query definition. A table expression may refer another table expression which previously defined in the same statement.
The general syntax is: ::

    WITH
      [RECURSIVE <recursive_cte_name> [ (<recursive_column_names>) ] AS <recursive_sub-query>]
      <cte_name1> [ (<cte1_column_names>) ] AS <sub-query1>
      <cte_name2> [ (<cte2_column_names>) ] AS <sub-query2>
      ...
    <final_query>
    

*  *recursive_cte_name*, *cte_name1*, *cte_name2* :  identifiers for the table expressions (sub-queries)
*  *recursive_column_names*, *cte1_column_names*, *cte2_column_names* : identifiers for the columns of the results of each table expression
*  *sub-query1*, *sub-query2* : sub-queries which define each table expression. 
*  *final_query* : query using table expression previously defined. 

Simplest usage is to unify/difference/intersect result list of table expressions:

.. code-block:: sql

    CREATE TABLE t1 (c INT);
    INSERT INTO t1 VALUES (1),(2);
    
    WITH
     cte2 AS (VALUES (2), (3)),
     cte1 AS (SELECT c FROM t1)
    SELECT c FROM cte1 UNION ALL SELECT * FROM cte2;
    
    WITH
     cte2 AS (VALUES (2), (3)),
     cte1 AS (SELECT c FROM t1)
    SELECT c FROM cte1 DIFFERENCE SELECT * FROM cte2;
    
    WITH
     cte2 AS (VALUES (2), (3)),
     cte1 AS (SELECT c FROM t1)
    SELECT c FROM cte1 INTERSECT SELECT * FROM cte2;    

::

                c
    =============
                1
                2
                2
                3
                
                c
    =============
                1

                c
    =============
                2               
            
A sub-query may be referenced by other sub-query:

.. code-block:: sql

    CREATE TABLE t1 (c INT);
    INSERT INTO t1 VALUES (1),(2);

    WITH
     cte2 AS (VALUES (2), (3)),
     cte1 AS (SELECT c FROM t1 UNION ALL SELECT * FROM cte2)
    SELECT c FROM cte1;

::

                c
    =============
                1
                2
                2
                3

Error will be prompted if:
 * More than one CTE uses the same identifier name.
 * using nested WITH clauses.
 
.. code-block:: sql

    WITH
     cte1 AS (VALUES (2), (3)),
     cte1 AS (SELECT c FROM t1)
    SELECT c FROM cte1, cte2;

::

    before '
        SELECT c FROM cte1, cte2;
    '
    CTE name ambiguity, there are more than one CTEs with the same name: 'cte1'.
    
.. code-block:: sql

    WITH
     cte1 AS (VALUES (2), (3)),
     cte2 AS (    WITH
                    cte3 AS (SELECT 1 FROM db_root)
                SELECT * FROM cte4 )
    SELECT c FROM cte1, cte2;

::

    before '
        SELECT c FROM cte1, cte2;
    '
    Nested WITH clauses are not supported.

CTE column names
================

The column names of each CTE result may be specified after the CTE name. The number of elements in the CTE column list must match the number of columns in the CTE sub-query.

.. code-block:: sql

    WITH
     cte1(c_of_cte, c_of_cte_100) AS (SELECT c, c+100 FROM t1)
    SELECT c_of_cte, c_of_cte_100 FROM cte1;
    
    WITH
     cte1(c_of_cte, c_of_cte_100) AS (SELECT c, c+100 FROM t1)
    SELECT c_of_cte FROM cte1; 

::

         c_of_cte  c_of_cte_100
    ===========================
                1           101
                2           102

         c_of_cte
    =============
                1
                2

If no column names are given in the CTE, the column names are extracted from the first inner select list of the CTE. This means that expressions will be named according to their original text.

.. code-block:: sql

    WITH
     cte1 AS (SELECT c, c+100 FROM t1)
    SELECT * FROM cte1;
    
    WITH
     cte1(c_of_cte, c_of_cte_100) AS (SELECT c, c+100 FROM t1)
    SELECT c_of_cte FROM cte1; 

::

                c     t1.c+100
    ==========================
                1          101
                2          102

         c_of_cte
    =============
                1
                2
                    
RECURSIVE clause
================

The **RECURSIVE** keyword allows construction recurrent queries (the table expression sub-queries definition contains its own name). A recursive table expression is composed of the non-recursive part and a recursive part (which references the sub-queries by its CTE name). The recursive and non-recursive parts **must** be combined using the UNION ALL query operator.
The recursive part should be defined in such way, that no cycle will be generated. Also if the recursive part contains aggregate functions, it should also contain a GROUP BY clause, because aggregate functions will return always a tuple and the recursive iterations will never stop. The recursive part will stop iterating when the conditions from WHERE clause are no longer true, and the current iteration return no results.

.. code-block:: sql

    WITH
      RECURSIVE cte1(x) AS (SELECT c FROM t1 UNION ALL SELECT cte1.x + 1 FROM cte1 WHERE cte1.x < 5)
    SELECT * FROM cte1;

::

                x
    =============
                1
                2
                2
                3
                3
                4
                4
                5
                5

Recursive CTEs may fall into an infinite loop. To avoid such case, set the system parameter **cte_max_recursions** to a desired threshold. Its default value is 2000 recursive iterations, maximum is 1000000 and minimum 2.

.. code-block:: sql

    SET SYSTEM PARAMETERS 'cte_max_recursions=3';
    WITH
      RECURSIVE cte1(x) AS (SELECT c FROM t1 UNION ALL SELECT cte1.x + 1 FROM cte1 WHERE cte1.x < 5)
    SELECT * FROM cte1;

::

    In the command from line 3,
    Maximum recursions 3 reached executing CTE.

.. warning::

    *   Depending on the complexity of the CTE sub-queries, the result set can grow very large for sub-queries which produces large amount of data. Even the default value of **cte_max_recursions** may not be enough to avoid starvation of disk space.

The execution algorithm of a recursive CTE may be summarized as:
 * execute the non recursive part of CTE and add its results to then final result set
 * execute the recursive part using the result set obtained by the non recursive part, add its results to the final result set and memorize the start and end of the current iteration within the result set.
 * repeat the non recursive part execution using the result set from previous iteration and add its results to the final result set
 * if a recursive iteration produces no results, then stop
 * if the configured maximum number of iterations is reached, also stop
 
The recursive CTE must be referenced directly in the **FROM** clause, referencing it in sub-query will prompt an error:

.. code-block:: sql

    WITH
     RECURSIVE cte1(x) AS SELECT c FROM t1 UNION ALL SELECT * from ( SELECT cte1.x + 1 FROM cte1 WHERE cte1.x < 5)
    SELECT * FROM cte1;

::

    before '
    SELECT * FROM cte1;
    '
    Recursive CTE 'cte1' must be referenced directly in its recursive query.
     