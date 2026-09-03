
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

재귀적 CTE는 무한 루프에 빠질 수 있다. 이런 경우를 피하기 위해서 시스템 파라미터 **cte_max_recursions** 를 원하는 임계치로 설정해야 한다. 이 파라미터의 기본값은 2,000번 재귀 반복이며, 최대값은 1,000,000 최소값은 2이다.

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

    *    CTE 부질의의 복잡도에 따라, 많은 량의 데이타가 생산되며, 심지어 **cte_max_recursions** 의 기본값만으로도 디스크 공간 부족을 발생할 수 있다.


재귀적 CTE의 실행 알고리즘은 다음과 같이 요약될 수 있다:
 * CTE의 비재귀적 부분을 수행하고 결과를 최종 결과 셋에 추가
 * 비재귀적 부분에서 얻은 결과 셋을 사용하여 재귀적 부분을 수행하고, 결과를 최종 결과 셋에 추가한 후, 결과 셋 내에서 현재 반복의 시작과 끝을 기억한다
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

.. _cte-inline-materialize:

CTE 실행 방식(Materialize와 Inline)
===================================

CTE는 실행 방식에 따라 materialize 방식과 inline 방식으로 나뉜다.

*   **Materialize** 방식: CTE의 부질의를 미리 수행하여 결과를 임시 파일로 저장하고, CTE를 참조하는 위치에서 저장된 결과를 공유한다. 동일한 데이터를 한 번만 스캔하는 장점이 있으나, 임시 파일로 저장된 결과에는 인덱스를 사용할 수 없으며 :ref:`View Merging <view_merge>`, :ref:`Predicate Push <pred-push>` 등의 질의 재작성이 불가능하다.

*   **Inline** 방식: CTE를 참조하는 위치에 CTE의 부질의를 인라인 뷰로 재작성하여 수행한다. :ref:`View Merging <view_merge>`, :ref:`Predicate Push <pred-push>` 등의 질의 재작성이 가능하여 인덱스를 활용할 수 있으나, CTE가 여러 번 참조되는 경우 동일한 데이터를 여러 번 스캔할 수 있다.

11.5 버전부터 inline 방식을 지원하며, 이전 버전에서는 CTE가 항상 materialize 방식으로 수행된다.

힌트를 지정하지 않으면 질의에서 CTE가 참조된 횟수에 따라 실행 방식이 결정된다. 참조 횟수에는 다른 CTE에서 참조된 횟수도 포함된다.

*   한 번 참조된 CTE는 inline 방식으로 수행된다.
*   두 번 이상 참조된 CTE는 materialize 방식으로 수행된다.
*   참조되지 않은 CTE는 수행되지 않고 질의에서 제거된다.

CTE의 부질의에 **INLINE** 또는 **MATERIALIZE** 힌트를 지정하여 실행 방식을 직접 결정할 수 있다. 힌트를 사용하는 방법은 :ref:`sql-hint`\를 참고한다.

*   **INLINE**: 참조 횟수와 관계없이 CTE를 inline 방식으로 수행한다.
*   **MATERIALIZE**: 참조 횟수와 관계없이 CTE를 materialize 방식으로 수행한다.

단, 다음의 경우에는 **INLINE** 힌트를 지정하더라도 materialize 방식으로 수행된다.

*   재귀적 CTE인 경우
*   **WITH RECURSIVE** 절에 정의된 CTE인 경우(재귀적이지 않은 CTE도 포함)
*   CTE의 부질의에 **QUERY_CACHE** 힌트가 지정된 경우

.. note::

    CTE의 부질의가 **UNION ALL** 등의 집합 연산으로 구성된 경우, 가장 왼쪽 **SELECT** 문에 지정된 힌트가 CTE 전체에 적용된다.

다음은 CTE의 실행 방식에 따른 수행 차이를 보여주는 예제이다.

.. code-block:: sql

    -- 예제에 사용할 테이블과 데이터 생성
    CREATE TABLE tbl (c1 INT, c2 INT, c3 INT, c4 INT, INDEX i1 (c1));

    INSERT INTO tbl
    SELECT ROWNUM, MOD (ROWNUM, 10), MOD (ROWNUM, 100), MOD (ROWNUM, 1000)
    FROM db_class a, db_class b, db_class c LIMIT 10000;

    UPDATE STATISTICS ON tbl;

아래 질의에서 CTE는 한 번만 참조되었으므로 inline 방식으로 수행된다. CTE가 인라인 뷰로 재작성된 후 View Merging과 Predicate Push가 적용되어 인덱스 *i1*\을 사용할 수 있으므로 조건에 맞는 100건만 읽는다.

.. code-block:: sql

    -- 한 번 참조된 CTE가 inline 방식으로 수행되는 예
    csql> ;trace on

    WITH cte AS (SELECT * FROM tbl)
    SELECT /*+ RECOMPILE */ c1 FROM cte WHERE c1 <= 100;

::

    Trace Statistics:
      SELECT (time: 0, fetch: 3, fetch_time: 0, ioread: 0)
        SCAN (index: dba.tbl.i1), (btree time: 0, fetch: 2, ioread: 0, readkeys: 100, filteredkeys: 100, rows: 100, covered: true)

아래 질의와 같이 **MATERIALIZE** 힌트를 지정하면 CTE가 한 번만 참조되어도 materialize 방식으로 수행된다. CTE의 부질의가 테이블의 모든 행을 스캔하여 결과를 임시 파일로 저장하고, 메인 질의는 저장된 결과에서 조건에 맞는 행을 찾는다.

.. code-block:: sql

    -- MATERIALIZE 힌트로 materialize 방식을 강제하는 예
    WITH cte AS (SELECT /*+ MATERIALIZE */ * FROM tbl)
    SELECT /*+ RECOMPILE */ c1 FROM cte WHERE c1 <= 100;

::

    Trace Statistics:
      SELECT (time: 6, fetch: 155, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 41, ioread: 0, readrows: 10000, rows: 100)
        SUBQUERY (uncorrelated)
          CTE (non_recursive_part)
            SELECT (time: 5, fetch: 114, fetch_time: 0, ioread: 0)
              SCAN (table: dba.tbl), (heap time: 2, fetch: 30, ioread: 0, readrows: 10000, rows: 10000)

아래 질의에서 CTE는 두 번 참조되었으므로 힌트가 없으면 materialize 방식으로 수행된다. CTE의 부질의는 한 번만 수행되고, 저장된 결과가 두 참조 위치에서 공유된다.

.. code-block:: sql

    -- 두 번 참조된 CTE가 materialize 방식으로 수행되는 예
    WITH cte AS (SELECT * FROM tbl)
    SELECT /*+ RECOMPILE */ a.c1 FROM cte a, cte b WHERE a.c1 <= 100 AND a.c1 = b.c1;

::

    Trace Statistics:
      SELECT (time: 56, fetch: 4255, fetch_time: 1, ioread: 0)
        SCAN (temp time: 0, fetch: 41, ioread: 0, readrows: 10000, rows: 100)
          SCAN (temp time: 51, fetch: 4100, ioread: 0, readrows: 1000000, rows: 100)
        SUBQUERY (uncorrelated)
          CTE (non_recursive_part)
            SELECT (time: 5, fetch: 114, fetch_time: 0, ioread: 0)
              SCAN (table: dba.tbl), (heap time: 5, fetch: 30, ioread: 0, readrows: 10000, rows: 10000)

**INLINE** 힌트를 지정하면 두 번 이상 참조된 CTE도 inline 방식으로 수행된다. 각 참조 위치에서 인덱스를 활용할 수 있으나, 동일한 테이블을 여러 번 스캔한다.

.. code-block:: sql

    -- INLINE 힌트로 두 번 참조된 CTE를 inline 방식으로 강제하는 예
    WITH cte AS (SELECT /*+ INLINE */ * FROM tbl)
    SELECT /*+ RECOMPILE */ a.c1 FROM cte a, cte b WHERE a.c1 <= 100 AND a.c1 = b.c1;

::

    Trace Statistics:
      SELECT (time: 0, fetch: 204, fetch_time: 0, ioread: 0)
        SCAN (index: dba.tbl.i1), (btree time: 0, fetch: 2, ioread: 0, readkeys: 100, filteredkeys: 100, rows: 100, covered: true)
          SCAN (index: dba.tbl.i1), (btree time: 0, fetch: 200, ioread: 0, readkeys: 100, filteredkeys: 100, rows: 100, covered: true)

DML과 CREATE에서 CTE의 사용
============================

**SELECT** 문에 대한 사용 외에도 CTE는 다른 문장에도 사용될 수 있다.
CTE가 **CREATE TABLE** *table_name* **AS SELECT** 에 사용될 수 있다:

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

또한, **INSERT**/**REPLACE INTO** *table_name* **SELECT** 도 CTE 사용이 가능하다:

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

또한 **UPDATE** 의 부질의에서도 사용 가능하다:

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

또한, **DELETE** 의 부질의에서도 사용가능하다:

.. code-block:: sql

    WITH product_removal_th AS (
        SELECT SUM (sales_n) / 20 AS threshold, product_type
        FROM green_products
        GROUP BY product_type
    )
        DELETE
        FROM green_products gp
        WHERE sales_n < (select threshold from product_removal_th WHERE product_type = gp.product_type);
