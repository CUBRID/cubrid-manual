
:meta-keywords: common table expression, recursive query, recursive cte
:meta-description: Common Table Expressions (CTEs) are temporary tables (list of results) associated with a statement.


***
CTE
***

CTE(Common Table Expressions)는 질의문과 관련된 임시 테이블(결과 목록)이다. 질의문 내에서 CTE를 여러 번 참조할 수 있으며 질의문 범위 내에서만 표시된다. CTE를 사용하면 질의문 로직을 보다 효과적으로 분리하여 수행 성능을 개선할 수 있으며  계층 질의문을 생성할 때 **CONNECT BY** 질의문 또는  복잡한 질의 대신 재귀적 CTE를 사용할 수 있다.

CTE는 **WITH** 절로 시작한다. 부질의 목록과 부질의를 사용하는 최종 질의가 있어야 한다. 각 부질의(테이블 표현식)는 이름과 질의 정의를 포함한다. 테이블 표현식은 이전에 동일한 질의문에 정의된 다른 테이블 표현식을 참조할 수 있다.
구문은 다음과 같다. ::

    WITH
      [RECURSIVE <recursive_cte_name> [ (<recursive_column_names>) ] AS <recursive_sub-query>]
      <cte_name1> [ (<cte1_column_names>) ] AS <sub-query1>
      <cte_name2> [ (<cte2_column_names>) ] AS <sub-query2>
      ...
    <final_query>
    

*  *recursive_cte_name*, *cte_name1*, *cte_name2* : 테이블 표현식(부질의)의 식별자
*  *recursive_column_names*, *cte1_column_names*, *cte2_column_names* : 각 테이블 표현식 결과 컬럼에 대한 식별자
*  *sub-query1*, *sub-query2* : 각 테이블 표현식을 정의하는 부질의
*  *final_query* : 이전에 정의된 테이블 표현식을 사용하는 질의. 일반적으로 **FROM** 절은 CTE 식별자를 포함한다.

가장 단순한 사용법은 테이블 표현식의 결과 목록을 결합하는 것이다.

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
            
한 CTE의 부질의가 다른 CTE의 부질의에 참조될 수 있다(참조되는 CTE가 미리 정의되어 있어야 함) :

.. code-block:: sql

    WITH
     of_drones AS (SELECT item FROM products WHERE parent_id = 1),
     filter_common_with_cars AS (SELECT * FROM of_drones INTERSECT SELECT item FROM products WHERE parent_id = 5)
    SELECT * FROM filter_common_with_cars ORDER BY 1;

::

      item
    ======================
      'Frame'

다음과 같은 경우 오류가 발생한다. :
 * 둘 이상의 CTE에서 동일한 식별자명 사용.
 * 중첩된 **WITH** 절 사용.
 
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

CTE 컬럼명 
==========

각 CTE 결과의 컬럼명은 CTE 이름 다음에 지정할 수 있다. CTE 컬럼 목록의 요소 수는 CTE 부질의의 컬럼 수와 일치해야 한다.

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

CTE에 컬럼명이 없으면 CTE의 첫 번째 내부 Select 문에서 컬럼명을 가져온다. 원본 구문에 따라 표현식 결과 컬럼명이 결정된다.

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

                    
재귀절
======

**RECURSIVE** 키워드를 사용하여 반복되는 질의를 구성할 수 있다(테이블 표현식 부질의 정의 자체 이름 포함). 재귀 테이블 표현식은 비재귀적 부분과 재귀적 부분(CTE 이름으로 부질의 참조)으로 구성된다. **UNION ALL** 질의 연산자를 사용하여 재귀적 부분과 비재귀적 부분을 **결합 해야 한다** .
무한 반복하지 않도록 재귀적 부분을 정의해야 한다. 또한 재귀적 부분에 집계 함수를 포함하는 경우 집계 함수가 항상 튜플을 반환하고 재귀 반복이 계속되므로 **GROUP BY** 절도 포함해야 한다. **WHERE** 절의 조건을 더 이상 만족하지 않고 현재 수행된 반복의 결과가 없을 경우 재귀 반복이 중단된다.

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

DML에서 CTE 사용(**UPDATE** 또는 **DELETE** 데이터) :
      
.. code-block:: sql

    UPDATE products SET price = 
        (WITH
         RECURSIVE cars (id, parent_id, item, price) AS (
                            SELECT id, parent_id, item, price 
                                FROM products  WHERE item LIKE 'Car%' 
                            UNION ALL 
                            SELECT p.id, p.parent_id, p.item, p.price 
                                FROM products p 
                            INNER JOIN cars rec_cars ON p.parent_id = rec_cars.id)
        SELECT SUM(price) - MAX(price) FROM cars ORDER BY 1) 
    WHERE item='Car';    

    select item, price from products where item='Car';

::
    
      item                        price
    ===================================
      'Car'                        8800 
  

재귀적 CTE는 무한 루프에 빠질 수 있다. 이러한 경우를 방지하려면 시스템 파라미터 **cte_max_recursions** 를 원하는 임계값으로 설정한다. 기본값은 2000번 재귀 반복이고, 최대값은 1000000, 최소값은 2이다.

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

    *   CTE 부질의의 복잡도에 따라 부질의에 대한 결과 셋이 매우 크게 증가하여 대용량의 데이터가 생성될 수 있으므로 디스크 공간 부족을 방지하기 위해  **cte_max_recursions** 설정값 조정을 고려해야 한다.

재귀적 CTE의 수행 알고리즘은 다음과 같이 요약할 수 있다.
 * CTE의 비재귀적 부분을 수행하고 결과를 최종 결과 셋에 추가
 * 비재귀적 부분에서 얻은 결과 셋을 사용하여 재귀적 부분을 수행하고, 결과를 최종 결과 셋에 추가한 후, 결과 셋 내에서 현재 반복의 시작과 끝을 기억한다.
 * 이전 반복의 결과 셋을 사용하여 비재귀적 부분의 수행을 반복하고 해당 결과를 최종 결과 셋에 추가
 * 재귀 반복에서 결과가 생성되지 않으면 중지
 * 설정된 최대 반복 횟수에 도달하는 경우에도 중지
 
재귀적 CTE를 **FROM** 절에서 바로 참조해야 한다. 부질의에서 참조하면 오류가 발생한다.

.. code-block:: sql

    WITH
     RECURSIVE cte1(x) AS SELECT c FROM t1 UNION ALL SELECT * FROM (SELECT cte1.x + 1 FROM cte1 WHERE cte1.x < 5)
    SELECT * FROM cte1;

::

    before '
    SELECT * FROM cte1;
    '
    Recursive CTE 'cte1' must be referenced directly in its recursive query.

