
:meta-keywords: update statement, update multiple table
:meta-description: You can update the column value of a record stored in the target table or view to a new one by using the UPDATE statement.


******
UPDATE
******

**UPDATE** 문을 사용하면 대상 테이블 또는 뷰에 저장된 레코드의 칼럼 값을 새로운 값으로 업데이트할 수 있다. **SET** 절에는 업데이트할 칼럼 이름과 새로운 값을 명시하며, :ref:`where-clause`\ 에는 업데이트할 레코드를 추출하기 위한 조건을 명시한다. 하나의 **UPDATE** 문으로 하나 이상의 테이블 또는 뷰를 업데이트할 수 있다.

.. note:: **JOIN** 구문을 포함하는 뷰에 대한 업데이트는 10.0 버전부터 가능하다.

::

    <UPDATE single table>
    UPDATE table_name|view_name SET column_name = {<expr> | DEFAULT} [, column_name = {<expr> | DEFAULT} ...]
        [WHERE <search_condition>]
        [ORDER BY {col_name | <expr>}]
        [LIMIT row_count]
     
    <UPDATE multiple tables>
    UPDATE <table_specifications> SET column_name = {<expr> | DEFAULT} [, column_name = {<expr> | DEFAULT} ...]
        [WHERE <search_condition>]

*   <*table_specifications*> : **SELECT** 문의 **FROM** 절과 같은 형태의 구문을 지정할 수 있으며, 하나 이상의 테이블을 지정할 수 있다.

*   *column_name*: 업데이트할 칼럼 이름을 지정한다. 하나 이상의 테이블에 대한 칼럼들을 지정할 수 있다.

*   <*expr*> | **DEFAULT**: 해당 칼럼의 새로운 값을 지정하며, 표현식 또는 **DEFAULT** 키워드를 값으로 지정할 수 있다. 단일 결과 레코드를 반환하는 **SELECT** 질의를 지정할 수도 있다.

*   <*search_condition*>: :ref:`where-clause`\ 에 조건식을 명시하면, 조건식을 만족하는 레코드에 대해서만 칼럼 값을 업데이트한다.

*   *col_name* | <*expr*>: 업데이트할 순서의 기준이 되는 칼럼을 지정한다.

*   *row_count*: :ref:`limit-clause` 이후 갱신할 레코드 수를 지정한다. 부호 없는 정수, 호스트 변수 또는 간단한 표현식 중 하나일 수 있다.

업데이트할 테이블이 한 개인 경우에 한하여, :ref:`order-by-clause`\ 이나 :ref:`limit-clause`\ 을 지정할 수 있다. :ref:`limit-clause`\ 을 명시하면 업데이트할 레코드 수를 한정할 수 있다. :ref:`order-by-clause`\ 을 명시하면 해당 칼럼의 순서로 레코드를 업데이트한다. :ref:`order-by-clause`\ 에 의한 업데이트는 트리거의 실행 순서나 잠금 순서를 유지하고자 할 때 유용하게 이용할 수 있다. 

.. note:: CUBRID 9.0 미만 버전에서는 <*table_specifications*>에 한 개의 테이블만 입력할 수 있다.

다음은 하나의 테이블에 대해 업데이트를 수행하는 예이다.

.. code-block:: sql

    --creating a new table having all records copied from a_tbl1
    CREATE TABLE a_tbl5 AS SELECT * FROM a_tbl1;
    SELECT * FROM a_tbl5 WHERE name IS NULL;

::
    
               id  name                  phone
    =========================================================
             NULL  NULL                  '000-0000'
                4  NULL                  '000-0000'
                5  NULL                  '000-0000'
                7  NULL                  '777-7777'
     
.. code-block:: sql

    UPDATE a_tbl5 SET name='yyy', phone='999-9999' WHERE name IS NULL LIMIT 3;
    SELECT * FROM a_tbl5;
     
::

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
     
.. code-block:: sql

    -- using triggers, that the order in which the rows are updated is modified by the ORDER BY clause.
     
    CREATE TABLE t (i INT,d INT);
    CREATE TRIGGER trigger1 BEFORE UPDATE ON t IF new.i < 10 EXECUTE PRINT 'trigger1 executed';
    CREATE TRIGGER trigger2 BEFORE UPDATE ON t IF new.i > 10 EXECUTE PRINT 'trigger2 executed';
    INSERT INTO t VALUES (15,1),(8,0),(11,2),(16,1), (6,0),(1311,3),(3,0);
    UPDATE t  SET i = i + 1 WHERE 1 = 1;
     
::

    trigger2 executed
    trigger1 executed
    trigger2 executed
    trigger2 executed
    trigger1 executed
    trigger2 executed
    trigger1 executed
     
.. code-block:: sql

    TRUNCATE TABLE t;
    INSERT INTO t VALUES (15,1),(8,0),(11,2),(16,1), (6,0),(1311,3),(3,0);
    UPDATE t SET i = i + 1 WHERE 1 = 1  ORDER BY i;
     
::

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

**UPDATE** 문에서 조인하는 테이블 *a_tbl*, *b_tbl*\ 에 대해 *a_tbl*\ 의 행 하나당 조인하는 *b_tbl*\ 의 행의 개수가 두 개 이상이고 갱신 대상 칼럼이 *a_tbl*\ 에 있으면, *b_tbl*\ 의 행들 중 첫 번째로 발견되는 행의 값을 사용하여 갱신을 수행한다.

위의 예에서 **JOIN** 조건 칼럼인 *id* = 5 인 행의 개수가 *a_tbl* 에는 한 개 있고 *b_tbl* 에는 두 개 있다면, *a_tbl.id* = 5 인 행의 업데이트 대상 칼럼인 *a_tbl.charge*\ 는 *b_tbl*\ 의 첫 번째 행의 *rate* 칼럼 값만 사용한다.

조인 구문에 대한 자세한 설명은 :ref:`join-query`\ 를 참고한다.

다음은 뷰에 대해 업데이트를 수행하는 예이다.

.. code-block:: sql 

    CREATE TABLE tbl1(a INT, b INT); 
    CREATE TABLE tbl2(a INT, b INT); 
    INSERT INTO tbl1 VALUES (5,5),(4,4),(3,3),(2,2),(1,1); 
    INSERT INTO tbl2 VALUES (6,6),(4,4),(3,3),(2,2),(1,1); 
    CREATE VIEW vw AS SELECT tbl2.* FROM tbl2 LEFT JOIN tbl1 ON tbl2.a=tbl1.a WHERE tbl2.a<=3; 

    UPDATE vw SET a=1000; 

아래의 UPDATE 문 결과는 :ref:`update_use_attribute_references <update_use_attribute_references>` 파라미터의 값에 따라 달라진다. 
      
.. code-block:: sql 

    CREATE TABLE tbl(a INT, b INT); 
    INSERT INTO tbl values (10, NULL); 

    UPDATE tbl SET a=1, b=a; 
      
이 파라미터의 값이 yes이면, 위의 UPDATE 질의에서 갱신되는 b의 값은 "a=1"의 영향을 받아 1이 된다. 

.. code-block:: sql 
  
    SELECT * FROM tbl; 

:: 
  
    1, 1 
      
이 파라미터의 값이 no이면, 위의 UPDATE 질의에서 갱신되는 b의 값은 "a=1"의 영향을 받지 않고 해당 레코드에 저장되어 있는 a 값의 영향을 받아 NULL이 된다. 

.. code-block:: sql 
  
    SELECT * FROM tbl; 
      
:: 
  
    1, NULL
    
