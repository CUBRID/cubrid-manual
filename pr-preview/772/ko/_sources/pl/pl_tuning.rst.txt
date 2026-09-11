-----------------------------
성능 최적화
-----------------------------

저장 프로시저와 저장 함수는 사용자가 직접 작성한 코드이므로, 성능 최적화가 필요할 수 있다.
이 장에서는 저장 프로시저와 저장 함수의 성능을 최적화하기 위한 다양한 가이드를 제공한다.

.. contents::

SQL 질의 최적화
==============================

저장 프로시저와 저장 함수의 가장 흔한 성능 오버헤드는 비효율적인 SQL 구문 수행에서 발생할 수 있다.
내부 루틴에서 사용되는 SQL 문에 대해서 충분히 튜닝하여 다음의 비효율적인 문제를 방지해야 한다.

* **테이블 풀 스캔**: 인덱스를 사용하지 않고 전체 테이블을 스캔하는 경우 성능이 저하될 수 있다.
* **통신 오버헤드**: CUBRID 데이터베이스 서버와 CUBRID PL 서버 간의 통신 오버헤드를 줄이기 위해서는 최소한의 개수의 레코드를 반환하도록 쿼리를 작성해야 한다.
* **비효율적인 반복문 내의 질의 호출**: 저장 프로시저와 저장 함수 내부에서 반복문을 사용할 때, 반복문 내에서 다수의 질의를 호출하지 않고 한번의 질의로 데이터를 가져오는 것을 권장한다.

저장 프로시저와 저장함수에서 사용된 비효율적인 질의는 :doc:`/sql/tuning` 문서를 참조하여 질의 최적화하기를 권장한다.

질의에서의 저장 함수 호출 최적화
=======================================

질의에서 실행하는 저장 함수의 불필요한 반복 호출은 성능을 저하시킬 수 있다. 따라서 저장 함수 호출을 최적화하기 위해 다음과 같은 방법을 고려해야 한다.

* **불필요한 반복 호출 최소화**: 저장 함수의 호출 횟수를 줄이는 것이 성능을 향상시키는 가장 좋은 방법이다.
   * 인덱스를 활용하여 저장 함수를 호출하는 레코드의 수를 줄인다.
   * 동일한 인수에 대해 반복 호출되지 않도록, 중복되는 데이터를 그룹화 하여 한번의 호출로 처리할 수 있도록 한다.
   * 만약 함수의 로직이 결정적이라면 :ref:`pl-deterministic`\를 활용한 상관 부질의 결과 캐시를 사용하여 성능을 향상시킬 수 있다.

* **함수의 인수와 반환 값의 크기를 최소화**: 저장 함수의 인수와 반환 값을 필요한 값만 반환하도록 설계하여 불필요하게 큰 데이터를 반환하지 않게 한다.

* **병렬 실행 활용**: 동시에 실행해도 안전한 저장 함수는 :ref:`pl-parallel-enable`\을 지정하여 병렬 실행을 허용할 수 있다.

.. _pl-use-builtin:

내장 함수 사용
===============================

CUBRID에서 기본적으로 제공하는 내장 함수는 (:doc:`/sql/function/index`) CUBRID의 쿼리 실행 동작에 맞춰 최적화되어 효율적으로 동작한다.
반면, 사용자가 작성하는 저장 함수는 그 내부 루틴을 모르는 **블랙박스** 이므로 내장 함수에 비해 성능이 떨어질 수 있다.
따라서 내장 함수의 단순한 조합으로 구현이 가능한 저장 함수보다 내장 함수를 사용하는 것을 것을 권장한다.

아래는 CONCAT 내장 함수와 동일한 기능을 수행하는 저장 함수의 예시이다.
사용자 저장 함수인 my_concat()보다 내장 함수인 concat()사용 시 성능 이점이 있다. 

.. code-block:: sql

        CREATE OR REPLACE FUNCTION my_concat (a STRING, b STRING) RETURN STRING AS
        BEGIN
          RETURN a || b;
        END;

        SELECT COUNT(*) FROM (SELECT /*+ NO_MERGE */ concat (name, event) FROM athlete);

        SELECT COUNT(*) FROM (SELECT /*+ NO_MERGE */ my_concat (name, event) FROM athlete);

::

        -- concat 함수 사용
                      count(*)
        ======================
                          6677

        1 row selected. (0.019853 sec) Committed. (0.000000 sec)

        
        -- my_concat 함수 사용
                      count(*)
        ======================
                          6677

        1 row selected. (0.302333 sec) Committed. (0.000000 sec)

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
      SELECT (time: 4, fetch: 11, fetch_time: 0, ioread: 0)
        FUNC (time: 4, fetch: 2, ioread: 0, calls: 4)
        SCAN (table: dba.dummy_tbl), (heap time: 0, fetch: 5, ioread: 0, readrows: 4, rows: 4)
        SUBQUERY (correlated)
          SELECT (time: 4, fetch: 6, fetch_time: 0, ioread: 0)
            FUNC (time: 4, fetch: 2, ioread: 0, calls: 4)
            SCAN (table: dual), (heap time: 0, fetch: 4, ioread: 0, readrows: 4, rows: 4)

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
      SELECT (time: 2, fetch: 9, fetch_time: 0, ioread: 0)
        FUNC (time: 2, fetch: 2, ioread: 0, calls: 2)
        SCAN (table: dba.dummy_tbl), (heap time: 0, fetch: 5, ioread: 0, readrows: 4, rows: 4)
        SUBQUERY (correlated)
          SELECT (time: 2, fetch: 4, fetch_time: 0, ioread: 0)
            FUNC (time: 2, fetch: 2, ioread: 0, calls: 2)
            SCAN (table: dual), (heap time: 0, fetch: 2, ioread: 0, readrows: 2, rows: 2)
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

.. _pl-parallel-enable:

병렬 실행이 가능한 함수 사용
----------------------------------

**PARALLEL_ENABLE**\은 Java 및 PL/CSQL 저장 함수의 병렬 실행을 허용하는 속성이다. 선언 방법은 :ref:`create-function-parallel-enable`\을 참고한다.

이 속성을 지정한 함수는 :ref:`parallel-scan`, :ref:`parallel-subquery-execution`, :ref:`parallel-hash-join`\에서 병렬로 실행할 수 있다. 선언만으로 병렬 실행이 보장되지는 않으며, 각 기능의 적용 조건과 :ref:`parallel-query-throughput-rules`\을 만족해야 한다. 병렬로 실행할 부분에 이 속성이 없는 저장 함수나 병렬 실행을 지원하지 않는 메서드가 함께 있으면 해당 부분의 병렬 실행이 제한된다.

**PARALLEL_ENABLE**\과 :ref:`DETERMINISTIC <pl-deterministic>`\은 독립적인 속성이며 함께 지정할 수 있다. **DETERMINISTIC**\만 지정한 함수는 병렬 실행을 허용하지 않으며, **PARALLEL_ENABLE**\만 지정해도 결정적 함수로 취급하지 않는다.

**선언 시 주의 사항**

CUBRID는 함수 본문이 병렬 실행에 안전한지 검사하지 않는다. 함수가 동시에 실행되어도 올바른 결과를 반환하는지 사용자가 확인해야 한다. Java의 변경 가능한 static 변수나 공유 상태를 사용하거나, 호출 횟수 또는 호출 순서에 따라 결과가 달라지는 함수에는 이 속성을 지정하지 않아야 한다.

**서버 접근 제한**

이 속성을 지정한 함수는 서버측 SQL과 **DBMS_OUTPUT**\을 사용할 수 없다. 실제로 병렬 실행이 선택되지 않은 경우와 **CALL**\로 직렬 호출한 경우에도 동일한 제한이 적용된다.

*   **PL/CSQL**: 서버 연결이 필요한 구문과 **DBMS_OUTPUT** 호출을 **CREATE FUNCTION** 또는 **CREATE OR REPLACE FUNCTION** 컴파일 시점에 거절한다. 서버에서 평가하는 내장 함수, 커서, 시리얼, 정적 SQL, 동적 SQL, 다른 저장 함수 및 프로시저 호출, **COMMIT**, **ROLLBACK**\을 사용할 수 없다. 실행되지 않는 분기나 예외 처리부에 있어도 허용하지 않는다.
*   **Java**: ``jdbc:default:connection``\으로 서버측 기본 연결을 얻거나 OID를 통해 서버에 접근하면 **SQLException**\이 발생한다. **DBMS_OUTPUT**\을 직접 호출하면 **RuntimeException**\이 발생한다. 예외를 처리하지 않으면 함수 호출이 실패한다. 함수가 예외를 처리하고 값을 반환하면 그 반환값을 사용한다.

Java 함수에서 OID를 인자나 반환값으로 전달하거나 ``getOidString()``\으로 문자열을 얻는 것은 허용한다. 그러나 OID가 가리키는 객체를 조회하거나 변경하는 서버 접근은 허용하지 않는다.

Java 함수의 외부 JDBC 연결(``jdbc:cubrid://...``)은 별도 세션과 트랜잭션을 사용하므로 허용한다. 이 연결은 함수를 호출한 질의의 커밋되지 않은 변경이나 SQL 세션 변수를 공유하지 않는다.

**선언 및 실행 확인**

:ref:`db-stored-procedure`\와 :ref:`information-schema-routines`\의 **is_parallel_enabled** 값이 **YES**\이면 선언된 함수이고, **NO**\이면 선언되지 않은 함수이다. 이 값은 선언 여부이며 실제 실행 계획의 병렬 여부를 나타내지 않는다.

실제 병렬 실행 여부는 :ref:`query-profiling`\으로 확인한다. **FUNC** 통계에는 병렬 워커가 실행한 저장 함수 호출도 합산된다.

**예제**

다음은 순수 계산을 수행하는 저장 함수를 병렬 스캔에서 호출하는 예이다. 병렬 워커를 2개 사용할 수 있는 환경에서 실행했으며, 이 실행에서 생성한 테이블의 힙 페이지 수는 3,281개이다.

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

다음 질의는 **PARALLEL(2)** 힌트로 병렬 처리 수준을 2로 지정한다.

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

같은 질의를 **PARALLEL(1)** 힌트로 직렬 실행하면 다음과 같다.

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

두 질의의 결과는 모두 **200000**\이다. 병렬 실행에서는 **parallel workers: 2**\가 표시되고, **FUNC**\의 **calls: 200000**\에는 두 워커가 실행한 함수 호출이 합산된다.
