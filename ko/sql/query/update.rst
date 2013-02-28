******
UPDATE
******

**UPDATE** 문을 사용하면 대상 테이블에 저장된 레코드의 칼럼 값을 새로운 값으로 업데이트할 수 있다. **SET** 절에는 업데이트할 칼럼 이름과 새로운 값을 명시하며, :ref:`where-clause`\ 에는 업데이트할 레코드를 추출하기 위한 조건을 명시한다. 하나의 **UPDATE** 문으로 하나 이상의 테이블을 업데이트할 수 있다.

업데이트할 테이블이 한 개인 경우에 한하여, :ref:`order-by-clause`\ 이나 :ref:`limit-clause`\ 을 지정할 수 있다. :ref:`limit-clause`\ 을 명시하면 업데이트할 레코드 수를 한정할 수 있다. :ref:`order-by-clause`\ 을 명시하면 해당 칼럼의 순서로 레코드를 업데이트한다. :ref:`order-by-clause`\ 에 의한 업데이트는 트리거의 실행 순서나 잠금 순서를 유지하고자 할 때 유용하게 이용할 수 있다. ::

    <UPDATE single table>
    UPDATE table_name SET column_name = {expr | DEFAULT} [, column_name = {expr | DEFAULT} ...]
        [WHERE search_condition]
        [ORDER BY {col_name | expr}]
        [LIMIT row_count]
     
    <UPDATE multiple tables>
    UPDATE <table_specifications> SET column_name = {expr | DEFAULT} [, column_name = {expr | DEFAULT} ...]
        [WHERE search_condition]

*   <*table_specifications*> : **SELECT** 문의 **FROM** 절과 같은 형태의 구문을 지정할 수 있으며, 하나 이상의 테이블을 지정할 수 있다.

*   *column_name*: 업데이트할 칼럼 이름을 지정한다. 하나 이상의 테이블에 대한 칼럼들을 지정할 수 있다.

*   *expr* | **DEFAULT**: 해당 칼럼의 새로운 값을 지정하며, 표현식 또는 **DEFAULT** 키워드를 값으로 지정할 수 있다. 단일 결과 레코드를 반환하는 **SELECT** 질의를 지정할 수도 있다.

*   *search_condition*: :ref:`where-clause`\ 에 조건식을 명시하면, 조건식을 만족하는 레코드에 대해서만 칼럼 값을 업데이트한다.

*   *col_name* | *expr*: 업데이트할 순서의 기준이 되는 칼럼을 지정한다.

*   *row_count*: :ref:`limit-clause`\ 에 업데이트할 레코드 수를 명시하며, 0보다 큰 정수를 지정할 수 있다.

.. note:: CUBRID 9.0 미만 버전에서는 <*table_specifications*>에 한 개의 테이블만 입력할 수 있다.

다음은 하나의 테이블에 대해 업데이트를 수행하는 예이다.

.. code-block:: sql

    --creating a new table having all records copied from a_tbl1
    CREATE TABLE a_tbl5 AS SELECT * FROM a_tbl1;
    SELECT * FROM a_tbl5 WHERE name IS NULL;
     
               id  name                  phone
    =========================================================
             NULL  NULL                  '000-0000'
                4  NULL                  '000-0000'
                5  NULL                  '000-0000'
                7  NULL                  '777-7777'
     
    UPDATE a_tbl5 SET name='yyy', phone='999-9999' WHERE name IS NULL LIMIT 3;
    SELECT * FROM a_tbl5;
     
               id  name                  phone
    =========================================================
             NULL  'yyy'                 '999-9999'
                1  'aaa'                 '000-0000'
                2  'bbb'                 '000-0000'
                3  'ccc'                 '333-3333'
                4  'yyy'                 '999-9999'
                5  'yyy'                 '999-9999'
                6  'eee'                 '000-0000'
                7  NULL                  '777-7777'
     
    -- using triggers, that the order in which the rows are updated is modified by the ORDER BY clause.
     
    CREATE TABLE t (i INT,d INT);
    CREATE TRIGGER trigger1 BEFORE UPDATE ON t IF new.i < 10 EXECUTE PRINT 'trigger1 executed';
    CREATE TRIGGER trigger2 BEFORE UPDATE ON t IF new.i > 10 EXECUTE PRINT 'trigger2 executed';
    INSERT INTO t VALUES (15,1),(8,0),(11,2),(16,1), (6,0),(1311,3),(3,0);
    UPDATE t  SET i = i + 1 WHERE 1 = 1;
     
    trigger2 executed
    trigger1 executed
    trigger2 executed
    trigger2 executed
    trigger1 executed
    trigger2 executed
    trigger1 executed
     
    TRUNCATE TABLE t;
    INSERT INTO t VALUES (15,1),(8,0),(11,2),(16,1), (6,0),(1311,3),(3,0);
    UPDATE t SET i = i + 1 WHERE 1 = 1  ORDER BY i;
     
    trigger1 executed
    trigger1 executed
    trigger1 executed
    trigger2 executed
    trigger2 executed
    trigger2 executed
    trigger2 executed

다음은 여러 개의 테이블들에 대해 조인한 후 업데이트를 수행하는 예이다.

.. code-block:: sql

    CREATE TABLE a_tbl(id INT PRIMARY KEY, charge DOUBLE);
    CREATE TABLE b_tbl(rate_id INT, rate DOUBLE);
    INSERT INTO a_tbl VALUES (1, 100.0), (2, 1000.0), (3, 10000.0);
    INSERT INTO b_tbl VALUES (1, 0.1), (2, 0.0), (3, 0.2), (3, 0.5);
     
    UPDATE
     a_tbl INNER JOIN b_tbl ON a_tbl.id=b_tbl.rate_id
    SET
      a_tbl.charge = a_tbl.charge * (1 + b_tbl.rate)
    WHERE a_tbl.charge > 900.0;

**UPDATE** 문에서 조인하는 테이블 *a_tbl*, *b_tbl* 에 대해 *a_tbl* 의 행 하나당 조인하는 *b_tbl* 의 행의 개수가 두 개 이상이고 갱신 대상 칼럼이 *a_tbl* 에 있으면, *b_tbl* 의 행들 중 첫 번째로 발견되는 행의 값을 사용하여 갱신을 수행한다.

위의 예에서 **JOIN** 조건 칼럼인 *id* = 5 인 행의 개수가 *a_tbl* 에는 한 개 있고 *b_tbl* 에는 두 개 있다면, *a_tbl.id* = 5 인 행의 업데이트 대상 칼럼인 a_tbl.charge는 *b_tbl* 의 첫 번째 행의 *rate* 칼럼 값만 사용한다.
