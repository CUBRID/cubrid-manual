***
CTE
***

Common Table Expressions (CTEs) are temporary tables (list of results) associated with a statement. A CTE can be referenced multiple times within the statement, and is visible only within the statement scope. It enables better separation of statement logic and may enhance execution performance. Moreover, recursive CTEs can be used to generate a hierarchical statement based on parent-child relationships, being able to reproduce CONNECT BY statements and other more complex queries. 

A CTE is introduced using the WITH clause. A list of sub-queries is expected, and final query which uses the sub-queries. Each sub-query (table expression) has a name and a query definition. A table expression may refer another table expression.
The general syntax is: ::

    WITH
      [RECURSIVE <recursive_cte_name> [ (<recursive_arguments>) ] AS <recursive_sub-query>]
      <cte_name1> [ (<arguments1>) ] AS <sub-query1>
      <cte_name2> [ (<arguments2>) ] AS <sub-query2>
      ...
    <final_query>
    

*  *recursive_cte_name*, *cte_name1*, *cte_name2* :  identifiers for the table expressions (sub-queries)
*  *recursive_arguments*, *arguments1*, *arguments2* : identifiers for the columns of the results of each table expression
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

Recursive CTEs may fall into an infinite loop. CUBRID means to avoid such case is by setting the system parameter **cte_max_recursions**. Its default value is 2000 recursive iterations, maximum is 1000000 and minimum 2.

.. code-block:: sql

    SET SYSTEM PARAMETERS 'cte_max_recursions=3';
    WITH
      RECURSIVE cte1(x) AS (SELECT c FROM t1 UNION ALL SELECT cte1.x + 1 FROM cte1 WHERE cte1.x < 5)
    SELECT * FROM cte1;

::

    In the command from line 3,
    Maximum recursions 3 reached executing CTE.

