-----------------------------
성능 최적화
-----------------------------

저장 프로시저와 저장 함수는 사용자가 직접 작성한 코드이므로, 성능 최적화가 필요할 수 있다. 이 장에서는 저장 프로시저와 저장 함수의 성능을 최적화하기 위한 다양한 가이드를 제공한다.

.. contents::

SQL 질의 최적화
==============================

저장 프로시저와 저장 함수의 가장 흔한 성능 오버헤드는 비효율적인 SQL 구문 수행에서 발생할 수 있다.
내부 루틴에서 사용되는 SQL 문에 대해서 충분히 튜닝하여 다음의 비효율적인 문제를 방지해야 한다.

* **테이블 풀 스캔**: 인덱스를 사용하지 않고 전체 테이블을 스캔하는 경우 성능이 저하될 수 있다.
* **통신 오버헤드**: CUBRID 데이터베이스 서버와 CUBRID PL 서버 간의 통신 오버헤드를 줄이기 위해서는 최소한의 개수의 레코드를 반환하도록 쿼리를 작성해야 한다.
* **비효율적인 반복문 내의 질의 호출**: 저장 프로시저와 저장 함수 내부에서 반복문을 사용할 때, 반복문 내에서 다수의 질의를 호출하지 않고 한번의 질의로 데이터를 가져오는 것을 권장한다.

:doc:`/sql/tuning` 문서에서 SQL 쿼리 최적화에 대한 자세한 내용을 확인하여 비효율적인 질의를 최적화할 수 있다.

질의에서의 저장 함수 호출 최적화
===============================

질의에서 실행하는 저장 함수의 불필요한 반복 호출은 성능을 저하시킬 수 있다. 따라서 저장 함수 호출을 최적화하기 위해 다음과 같은 방법을 고려해야 한다.

* **불필요한 반복 호출 최소화**: 저장 함수의 호출 횟수를 줄이는 것이 성능을 향상시키는 가장 좋은 방법이다.
   * 인덱스를 활용하여 저장 함수를 호출하는 레코드의 수를 줄인다.
   * 동일한 인수에 대해 반복 호출되지 않도록, 중복되는 데이터를 그룹화 하여 한번의 호출로 처리할 수 있도록 한다.
   * 만약 함수의 로직이 결정적이라면 :ref:`pl-deterministic`\를 활용한 상관 부질의 결과 캐시를 사용하여 성능을 향상시킬 수 있다.

* **함수의 인수와 반환 값의 크기를 최소화**: 저장 함수의 인수와 반환 값을 필요한 값만 반환하도록 설계하여 불필요하게 큰 데이터를 반환하지 않게 한다.

.. _pl-use-builtin:

내장 함수 사용
===============================

CUBRID에서 기본적으로 제공하는 내장 함수는 (:doc:`/sql/function/index`) CUBRID의 쿼리 실행 동작에 맞춰 최적화되어 효율적으로 동작한다.
반면, 사용자가 작성하는 저장 함수는 그 내부 루틴을 모르는 **블랙박스** 이므로 내장 함수에 비해 성능이 떨어질 수 있다.
따라서 내장 함수의 단순한 조합으로 구현이 가능한 저장 함수보다 내장 함수를 사용하는 것을 것을 권장한다.

아래는 CONCAT 내장 함수와 동일한 기능을 수행하는 저장 함수의 예시이다.

.. code-block:: sql

        CREATE OR REPLACE FUNCTION my_concat (a STRING, b STRING) RETURN STRING AS
        BEGIN
          RETURN a || b;
        END;

        SELECT COUNT(*) FROM (SELECT /*+ NO_MERGE */ concat (name, event) FROM athlete);

        SELECT COUNT(*) FROM (SELECT /*+ NO_MERGE */ my_concat (name, event) FROM athlete);

::

                count(*)
        ======================
                        6677

        1 row selected. (0.224946 sec) Committed. (0.000000 sec) 

                count(*)
        ======================
                        6677

        1 row selected. (0.014289 sec) Committed. (0.000000 sec)

.. _pl-deterministic:

결정적 함수 사용
==============================

결정적 함수는 동일한 인수에 대해 항상 동일한 결과를 반환하는 함수를 의미한다. 저장 함수가 결정적 함수이면, 저장 함수의 결과를 재사용하여 성능을 향상시킬 수 있다.

*   저장 함수를 결정적 함수로 만들기 위해 생성 시 **CREATE FUNCTION** 구문에서 **DETERMINISTIC** 속성을 지정할 수 있다. 자세한 내용은 :ref:`create-function`\를 참고한다.
*   **DETERMINISTIC** 속성을 지정하면 저장 함수는 상관 부질의 결과 캐시의 최적화에 사용될 수 있으며 동일한 인수에 대해 항상 동일한 결과를 반환한다. 상관 부질의 캐시 동작 방식에 대한 자세한 내용은 :ref:`correlated-subquery-cache`\을 참고한다.

다음은 **DETERMINISTIC**\으로 결정적 함수로 생성한 저장 함수의 예시이다. 이 예시에서는 상관 부질의를 사용할 때 결과를 캐시하여 성능을 최적화하는 과정을 보여준다.

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

위 예시에서 pl_csql_not_deterministic 함수는 **NOT DETERMINISTIC**\이므로 상관 부질의에서 쿼리 캐시를 사용하지 않는다.
반면, pl_csql_deterministic 함수는 **DETERMINISTIC** 키워드가 지정되어 있으므로 상관 부질의에서 쿼리 캐시를 사용하여 성능을 최적화할 수 있다.

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
      SELECT (time: 3, fetch: 44, fetch_time: 0, ioread: 0)
        SCAN (table: dba.dummy_tbl), (heap time: 0, fetch: 20, ioread: 0, readrows: 4, rows: 4)
        SUBQUERY (correlated)
          SELECT (time: 3, fetch: 24, fetch_time: 0, ioread: 0)
            SCAN (table: dual), (heap time: 0, fetch: 16, ioread: 0, readrows: 4, rows: 4)

pl_csql_not_deterministic 함수는 **NOT DETERMINISTIC** 이므로 상관 부질의 결과를 캐시하지 않는다.

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
      SELECT (time: 3, fetch: 36, fetch_time: 0, ioread: 0)
        SCAN (table: dba.dummy_tbl), (heap time: 0, fetch: 20, ioread: 0, readrows: 4, rows: 4)
        SUBQUERY (correlated)
          SELECT (time: 3, fetch: 16, fetch_time: 0, ioread: 0)
            SCAN (table: dual), (heap time: 0, fetch: 8, ioread: 0, readrows: 2, rows: 2)
            SUBQUERY_CACHE (hit: 2, miss: 2, size: 150808, status: enabled)

pl_csql_deterministic 함수의 Trace 결과에서는 **SUBQUERY_CACHE** 항목이 표시되며(hit: 2, miss: 2, size: 150808, status: enabled), 상단의 **SCAN (table: dual)** 에서 읽은 레코드 수(**readrows**)가 **NOT DETERMINISTIC** 예시와 비교해 감소한 것을 확인할 수 있다.

.. warning::

        * 저장 프로시저에서는 **DETERMINISTIC** 속성을 지원하지 않는다.
        * 결정적이지 않은 결과를 반환하는 내부 구현에서 **DETERMINISTIC** 속성을 사용하는 경우에는 기대한 결과를 반환하지 않을 수 있다.

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
        
위 예시에서 cnt_name 함수 내부의 my_serial.NEXT_VALUE는 결정적이지 않은 결과를 반환하므로 상관 부질의 캐시에 의해 기대하지 않은 결과를 반환한다.
저장 함수의 구현을 고려하여 **DETERMINISTIC** 속성을 지정 할 것을 권장한다.
