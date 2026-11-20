
:meta-keywords: common table expression, recursive query, recursive cte
:meta-description: Common Table Expressions (CTEs) are temporary tables (list of results) associated with a statement.

***
CTE
***

Common Table Expressions (CTEs) are temporary tables (list of results) associated with a statement. A CTE can be referenced multiple times within the statement, and is visible only within the statement scope. It enables better separation of statement logic and may enhance execution performance. Moreover, recursive CTEs can be used to generate a hierarchical statement based on parent-child relationships, being able to reproduce **CONNECT BY** statements and other more complex queries. 

A CTE is introduced using the **WITH** clause. A list of sub-queries is expected, and final query which uses the sub-queries. Each sub-query (table expression) has a name and a query definition. A table expression may refer another table expression which previously defined in the same statement.
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
*  *final_query* : query using table expression previously defined. Usually, the **FROM** clause of this will contain the CTEs identifiers.

Simplest usage is to combine result lists of table expressions:

.. code-block:: sql

    CREATE TABLE products (id INTEGER PRIMARY KEY, parent_id INTEGER, item VARCHAR(100), price INTEGER);
    INSERT INTO products VALUES (1, -1, 'Drone', 2000);
    INSERT INTO products VALUES (2, 1, 'Blade', 10);
    INSERT INTO products VALUES (3, 1, 'Brushless motor', 20);
    INSERT INTO products VALUES (4, 1, 'Frame', 50);
    INSERT INTO products VALUES (5, -1, 'Car', 20000);
    INSERT INTO products VALUES (6, 5, 'Wheel', 100);
    INSERT INTO products VALUES (7, 5, 'Engine', 4000);
    INSERT INTO products VALUES (8, 5, 'Frame', 4700);
    
    WITH
     of_drones AS (SELECT item, 'drones' FROM products WHERE parent_id = 1),
     of_cars AS (SELECT item, 'cars' FROM products WHERE parent_id = 5)
    SELECT * FROM of_drones UNION ALL SELECT * FROM of_cars ORDER BY 1;

::

      item                  'drones'
    ============================================
      'Blade'               'drones'
      'Brushless motor'     'drones'
      'Car'                 'cars'
      'Drone'               'drones'
      'Engine'              'cars'
      'Frame'               'drones'
      'Frame'               'cars'
      'Wheel'               'cars'            
            
A sub-query of one CTE may be referenced by other sub-query of another CTE (the referenced CTE needs to be defined before):

.. code-block:: sql

    WITH
     of_drones AS (SELECT item FROM products WHERE parent_id = 1),
     filter_common_with_cars AS (SELECT * FROM of_drones INTERSECT SELECT item FROM products WHERE parent_id = 5)
    SELECT * FROM filter_common_with_cars ORDER BY 1;

::

      item
    ======================
      'Frame'

Error will be prompted if:
 * More than one CTE uses the same identifier name.
 * using nested **WITH** clauses.
 
.. code-block:: sql

    WITH
     my_cte AS (SELECT item FROM products WHERE parent_id = 1),
     my_cte AS (SELECT * FROM my_cte INTERSECT SELECT item FROM products WHERE parent_id = 5)
    SELECT * FROM my_cte ORDER BY 1;

::

    before '
        SELECT * FROM my_cte ORDER BY 1;
    '
    CTE name ambiguity, there are more than one CTEs with the same name: 'my_cte'.
    
.. code-block:: sql

    WITH
     of_drones AS (SELECT item FROM products WHERE parent_id = 1),
     of_cars1 AS (WITH 
                    of_cars2 AS (SELECT item FROM products WHERE parent_id = 5)
                  SELECT * FROM of_cars2
                  )
    SELECT * FROM of_drones, of_cars1 ORDER BY 1;

::

    before '
        SELECT * FROM of_drones, of_cars1 ORDER BY 1;
    '
    Nested WITH clauses are not supported.

CTE column names
================

The column names of each CTE result may be specified after the CTE name. The number of elements in the CTE column list must match the number of columns in the CTE sub-query.

.. code-block:: sql

    WITH
     of_drones (product_name, product_type, price) AS (SELECT item, 'drones', price FROM products WHERE parent_id = 1),
     of_cars (product_name, product_type, price) AS (SELECT item, 'cars', price FROM products WHERE parent_id = 5)
    SELECT * FROM of_drones UNION ALL SELECT * FROM of_cars ORDER BY product_type, price;
    
    WITH
     of_drones (product_name, product_type, price) AS (SELECT item, 'drones' as type, MAX(price) FROM products WHERE parent_id = 1 GROUP BY type),
     of_cars (product_name, product_type, price) AS (SELECT item, 'cars'  as type, MAX (price) FROM products WHERE parent_id = 5 GROUP BY type)
    SELECT * FROM of_drones UNION ALL SELECT * FROM of_cars ORDER BY product_type, price;

::

      product_name          product_type                price
    =========================================================
      'Wheel'               'cars'                        100
      'Engine'              'cars'                       4000
      'Frame'               'cars'                       4700
      'Blade'               'drones'                       10
      'Brushless motor'     'drones'                       20
      'Frame'               'drones'                       50

     product_name          product_type                price
    ========================================================
     'Wheel'               'cars'                       4700
     'Blade'               'drones'                       50

If no column names are given in the CTE, the column names are extracted from the first inner select list of the CTE. The expressions result columns will be named according to their original text.

.. code-block:: sql

    WITH
     of_drones AS (SELECT item, 'drones', MAX(price) FROM products WHERE parent_id = 1 GROUP BY 2),
     of_cars AS (SELECT item, 'cars', MAX (price) FROM products WHERE parent_id = 5 GROUP BY 2)
    SELECT * FROM of_drones UNION ALL SELECT * FROM of_cars ORDER BY 1;
    
::

     item                  'drones'              max(products.price)
    ================================================================
     'Blade'               'drones'                               50
     'Wheel'               'cars'                               4700

                    
RECURSIVE clause
================

The **RECURSIVE** keyword allows construction recurrent queries (the table expression sub-queries definition contains its own name). A recursive table expression is composed of the non-recursive part and a recursive part (which references the sub-queries by its CTE name). The recursive and non-recursive parts **must** be combined using the **UNION ALL** query operator.
The recursive part should be defined in such way, that no cycle will be generated. Also if the recursive part contains aggregate functions, it should also contain a **GROUP BY** clause, because aggregate functions will return always a tuple and the recursive iterations will never stop. The recursive part will stop iterating when the conditions from **WHERE** clause are no longer true, and the current iteration return no results.

.. code-block:: sql

    WITH
     RECURSIVE cars (id, parent_id, item, price) AS (
                        SELECT id, parent_id, item, price 
                            FROM products WHERE item LIKE 'Car%' 
                        UNION ALL 
                        SELECT p.id, p.parent_id, p.item, p.price 
                            FROM products p 
                        INNER JOIN cars rec_cars ON p.parent_id = rec_cars.id)
    SELECT item, price FROM cars ORDER BY 1;

::

      item                        price
    ===================================
      'Car'                       20000
      'Engine'                     4000
      'Frame'                      4700
      'Wheel'                       100  

Recursive CTEs may fall into an infinite loop. To avoid such case, set the system parameter **cte_max_recursions** to a desired threshold. Its default value is 2000 recursive iterations, maximum is 1000000 and minimum 2.

.. code-block:: sql

    SET SYSTEM PARAMETERS 'cte_max_recursions=2';
    WITH
     RECURSIVE cars (id, parent_id, item, price) AS (
                        SELECT id, parent_id, item, price 
                            FROM products  WHERE item LIKE 'Car%' 
                        UNION ALL 
                        SELECT p.id, p.parent_id, p.item, p.price 
                            FROM products p 
                        INNER JOIN cars rec_cars ON p.parent_id = rec_cars.id)
    SELECT item, price FROM cars ORDER BY 1;

::

    In the command from line 9,
    Maximum recursions 2 reached executing CTE.

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
     RECURSIVE cte1(x) AS SELECT c FROM t1 UNION ALL SELECT * FROM (SELECT cte1.x + 1 FROM cte1 WHERE cte1.x < 5)
    SELECT * FROM cte1;

::

    before '
    SELECT * FROM cte1;
    '
    Recursive CTE 'cte1' must be referenced directly in its recursive query.

CTE Usage in DMLs and CREATE
============================

Besides their use for **SELECT** statements, CTEs can also be used for other statements.
CTEs can be used in **CREATE TABLE** *table_name* **AS SELECT**:

.. code-block:: sql

    CREATE TABLE inc AS
        WITH RECURSIVE cte (n) AS (
            SELECT 1
            UNION ALL
            SELECT n + 1
            FROM cte
            WHERE n < 3)
        SELECT n FROM cte;
    
    SELECT * FROM inc;

::

                n
    =============
                1
                2
                3

Also, **INSERT**/**REPLACE INTO** *table_name* **SELECT** can use CTE:

.. code-block:: sql

    INSERT INTO inc
        WITH RECURSIVE cte (n) AS (
            SELECT 1
            UNION ALL
            SELECT n + 1
            FROM cte
            WHERE n < 3)
        SELECT * FROM cte;

    REPLACE INTO inc
       WITH cte AS (SELECT * FROM inc)
       SELECT * FROM cte;

Also, in subclauses of **UPDATE** statement:

.. code-block:: sql

    CREATE TABLE green_products (producer_id INTEGER, sales_n INTEGER, product VARCHAR, product_type INTEGER, price INTEGER);
    INSERT INTO green_products VALUES (1, 99, 'bicycle', 1, 99);
    INSERT INTO green_products VALUES (2, 337, 'bicycle', 1, 129);
    INSERT INTO green_products VALUES (3, 5012, 'bicycle', 1, 199);
    INSERT INTO green_products VALUES (1, 989, 'scooter', 2, 899);
    INSERT INTO green_products VALUES (3, 3211, 'scooter', 2, 599);
    INSERT INTO green_products VALUES (4, 2312, 'scooter', 2, 1009);

    WITH price_increase_th AS (
        SELECT SUM (sales_n) * 7 / 10 AS threshold, product_type 
        FROM green_products
        GROUP BY product_type
    )
        UPDATE green_products gp JOIN price_increase_th th ON gp.product_type = th.product_type 
        SET price = price + (price / 10)
        WHERE sales_n >= threshold;

And also, in subclauses of **DELETE** statement:

.. code-block:: sql

    WITH product_removal_th AS (
        SELECT SUM (sales_n) / 20 AS threshold, product_type 
        FROM green_products
        GROUP BY product_type
    )
        DELETE 
        FROM green_products gp 
        WHERE sales_n < (select threshold from product_removal_th WHERE product_type = gp.product_type);
