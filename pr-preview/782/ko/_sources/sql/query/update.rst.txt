
:meta-keywords: update statement, update multiple table
:meta-description: You can update the column value of a record stored in the target table or view to a new one by using the UPDATE statement.


******
UPDATE
******

**UPDATE** 문을 사용하면 대상 테이블 또는 뷰에 저장된 레코드의 칼럼 값을 새로운 값으로 업데이트할 수 있다.
**SET** 절에는 업데이트할 칼럼 이름과 새로운 값을 명시하며, :ref:`where-clause`\ 에는 업데이트할 레코드를 추출하기 위한 조건을 명시한다.
하나의 **UPDATE** 문으로 하나 이상의 테이블 또는 뷰를 업데이트할 수 있다.

::

    <UPDATE single table>
    UPDATE [schema_name.]table_name | <remote_table_spec> | view_name SET column_name = {<expr> | DEFAULT} [, column_name = {<expr> | DEFAULT} ...]
        [WHERE <search_condition>]
        [ORDER BY {col_name | <expr>}]
        [LIMIT row_count]
     
    <remote_table_spec> ::= [schema_name.]table_name@[schema.name.]server_name [correlation>]
    <UPDATE multiple tables>
    UPDATE <table_specifications> SET column_name = {<expr> | DEFAULT} [, column_name = {<expr> | DEFAULT} ...]
        [WHERE <search_condition>]

*   <*table_specifications*> : **SELECT** 문의 **FROM** 절과 같은 형태의 구문을 지정할 수 있으며, 하나 이상의 테이블을 지정할 수 있다.

*   *server_name*: 현재 서버가 아닌 dblink로 연결된 원격 서버의 테이블을 지정할 때 사용한다.

*   *column_name*: 업데이트할 칼럼 이름을 지정한다. 하나 이상의 테이블에 대한 칼럼들을 지정할 수 있다.

*   <*expr*> | **DEFAULT**: 해당 칼럼의 새로운 값을 지정하며, 표현식 또는 **DEFAULT** 키워드를 값으로 지정할 수 있다. 단일 결과 레코드를 반환하는 **SELECT** 질의를 지정할 수도 있다.

*   <*search_condition*>: :ref:`where-clause`\ 에 조건식을 명시하면, 조건식을 만족하는 레코드에 대해서만 칼럼 값을 업데이트한다.

*   *col_name* | <*expr*>: 업데이트할 순서의 기준이 되는 칼럼을 지정한다.

*   *row_count*: :ref:`limit-clause` 이후 갱신할 레코드 수를 지정한다. 부호 없는 정수, 호스트 변수 또는 간단한 표현식 중 하나일 수 있다.

<*table_specifications*>에 하나의 테이블이 지정된 경우에만 다음이 허용된다:

* :ref:`order-by-clause`\ 을 지정할 수 있다.
  :ref:`order-by-clause`\ 을 명시하면 해당 칼럼의 순서로 레코드를 업데이트할 수 있다.
  이것은 트리거의 실행 순서나 잠금 순서를 유지하고자 할 때 유용하다.

* :ref:`limit-clause`\ 을 지정할 수 있다.
  :ref:`limit-clause`\ 을 명시하면 업데이트할 레코드 수를 제한할 수 있다.

* **SET** 절의 <*expr*>에 분석 함수를 사용할 수 있다.
  그러나 <*expr*>에 **SELECT** 질의를 지정한 경우, <*table_specifications*>에 지정된 테이블 수와 관계없이 **SELECT** 질의에서 분석 함수를 사용할 수 있다.

.. note::

    CUBRID 9.0 미만 버전에서는 <*table_specifications*>에 하나의 테이블만 지정할 수 있다.

.. note::

    CUBRID 10.0 버전부터는 **JOIN** 구문을 포함하는 뷰에 대한 업데이트가 가능하다.

.. _example_single_table_update_using_order_by:

.. rubric:: 예시 1. ORDER BY 절을 이용한 단일 테이블 업데이트

.. code-block:: sql

    DROP TABLE IF EXISTS t1;

    CREATE TABLE t1 (id INT);
    INSERT INTO t1 VALUES (11), (1), (12), (13), (2), (3), (14), (4);

    CREATE TRIGGER trigger1 BEFORE UPDATE ON t1 IF NEW.id < 10 EXECUTE PRINT 'trigger1 executed';
    CREATE TRIGGER trigger2 BEFORE UPDATE ON t1 IF NEW.id > 10 EXECUTE PRINT 'trigger2 executed';

    UPDATE t1  SET id = id + 1;

	trigger2 executed
	trigger1 executed
	trigger2 executed
	trigger2 executed
	trigger1 executed
	trigger1 executed
	trigger2 executed
	trigger1 executed

:ref:`order-by-clause`\을 이용하면 레코드가 업데이트되는 순서를 변경할 수 있다.

.. code-block:: sql

    TRUNCATE TABLE t1;
    INSERT INTO t1 VALUES (11), (1), (12), (13), (2), (3), (14), (4);

    UPDATE t1  SET id = id + 1 ORDER BY id;

	trigger1 executed
	trigger1 executed
	trigger1 executed
	trigger1 executed
	trigger2 executed
	trigger2 executed
	trigger2 executed
	trigger2 executed

.. _example_single_table_update_using_limit:

.. rubric:: 예시 2. LIMIT 절을 활용한 단일 테이블 업데이트

이 예시는 *LIMIT 3* 절을 이용하여 *name IS NULL*\인 레코드 중 최대 3개만 업데이트한다.

.. code-block:: sql

    DROP TABLE IF EXISTS t1;

    CREATE TABLE t1 (id int, name varchar, phone varchar);
    INSERT INTO t1 VALUES (NULL,  NULL, '000-0000');
    INSERT INTO t1 VALUES (   1, 'aaa', '111-1111');
    INSERT INTO t1 VALUES (   2, 'bbb', '222-2222');
    INSERT INTO t1 VALUES (   3, 'ccc', '333-3333');
    INSERT INTO t1 VALUES (   4,  NULL, '000-0000');
    INSERT INTO t1 VALUES (   5,  NULL, '000-0000');
    INSERT INTO t1 VALUES (   6, 'ddd', '000-0000');
    INSERT INTO t1 VALUES (   7,  NULL, '777-7777');

    SELECT * FROM t1 WHERE name IS NULL;

	           id  name                  phone
	=========================================================
	         NULL  NULL                  '000-0000'
	            4  NULL                  '000-0000'
	            5  NULL                  '000-0000'
	            7  NULL                  '777-7777'

    UPDATE t1 SET name='update', phone='999-9999' WHERE name IS NULL LIMIT 3;

    SELECT * FROM t1;

	           id  name                  phone
	=========================================================
	         NULL  'update'              '999-9999'
	            1  'aaa'                 '111-1111'
	            2  'bbb'                 '222-2222'
	            3  'ccc'                 '333-3333'
	            4  'update'              '999-9999'
	            5  'update'              '999-9999'
	            6  'ddd'                 '000-0000'
	            7  NULL                  '777-7777'

.. _example_update_with_joins:

.. rubric:: 예시 3. 조인을 활용한 업데이트

**UPDATE** 문에서 테이블 **A**\의 레코드가 테이블 **B**\의 여러 레코드와 조인될 때,
**A**\의 레코드는 **B**\에서 처음으로 일치하는 레코드의 값만 사용하여 업데이트된다.

이 예시에서, *t1* 테이블의 *id*\가 3인 레코드는 *t2* 테이블에서 *rate_id*\가 3인 두 개의 레코드와 조인된다.
그러나 *t1* 테이블의 *charge* 칼럼은 *t2* 테이블에서 첫 번째로 일치하는 레코드의 *rate* 칼럼 값만 사용하여 업데이트된다.

.. code-block:: sql

    DROP TABLE IF EXISTS t1, t2;

    CREATE TABLE t1 (id INT PRIMARY KEY, charge DOUBLE);
    INSERT INTO t1 VALUES (1, 100.0);
    INSERT INTO t1 VALUES (2, 100.0);
    INSERT INTO t1 VALUES (3, 100.0);

    CREATE TABLE t2 (rate_id INT, rate DOUBLE);
    INSERT INTO t2 VALUES (1, 0.1);
    INSERT INTO t2 VALUES (2, 0.2);
    INSERT INTO t2 VALUES (3, 0.3);
    INSERT INTO t2 VALUES (3, 0.5);
    
    UPDATE t1 a INNER JOIN t2 b ON a.id = b.rate_id
    SET a.charge = a.charge * (1 + b.rate);

    SELECT id, TO_CHAR (charge) AS charge FROM t1;

	           id  charge
	===================================
	            1  '110'
	            2  '120'
	            3  '150'

조인 구문에 대한 자세한 설명은 :ref:`join-query`\를 참고한다.

.. _example_view_update:

.. rubric:: 예시 4. 뷰 업데이트

.. code-block:: sql

    DROP TABLE IF EXISTS t1, t2;
    DROP VIEW v1;

    CREATE TABLE t1 (id INT, val INT) DONT_REUSE_OID;
    INSERT INTO t1 VALUES (1, 1);
    INSERT INTO t1 VALUES (2, 2);
    INSERT INTO t1 VALUES (3, 3);
    INSERT INTO t1 VALUES (4, 4);
    INSERT INTO t1 VALUES (5, 5);

    CREATE TABLE t2 (id INT, val INT) DONT_REUSE_OID;
    INSERT INTO t2 VALUES (1, 1);
    INSERT INTO t2 VALUES (2, 2);
    INSERT INTO t2 VALUES (3, 3);
    INSERT INTO t2 VALUES (4, 4);
    INSERT INTO t2 VALUES (6, 6);

    CREATE VIEW v1 AS
      SELECT b.id, b.val FROM t2 b LEFT JOIN t1 a ON b.id = a.id WHERE b.id <= 3;

    UPDATE v1 SET val = -1;

    SELECT * from v1;

	           id          val
	==========================
	            1           -1
	            2           -1
	            3           -1

    SELECT * from t2;

	           id          val
	==========================
	            1           -1
	            2           -1
	            3           -1
	            4            4
	            6            6

.. warning::

    REUSE OID 테이블이 포함된 뷰의 레코드는 업데이트할 수 없다.

    **REUSE_OID**\와 **DONT_REUSE_OID**\에 대한 자세한 내용은 :ref:`reuse-oid` (:ref:`dont-reuse-oid`)\를 참고한다.

.. _example_update_using_update_use_attribute_references:

.. rubric:: 예시 5. update_use_attribute_references 파라미터를 활용한 업데이트

이 예시의 결과는 :ref:`update_use_attribute_references <update_use_attribute_references>` 파라미터의 값에 따라 달라진다.

*   이 파라미터의 값이 yes인 경우, *c1 = 10*\의 영향을 받아 *c2*\의 값이 10으로 갱신된다.
*   이 파라미터의 값이 no인 경우, *c2*\의 값은 *c1 = 10*\의 영향을 받지 않고, 해당 레코드에 저장된 *c1* 값에 따라 1로 갱신된다.

.. code-block:: sql 

    DROP TABLE IF EXISTS t1;

    CREATE TABLE t1 (c1 INT, c2 INT); 
    INSERT INTO t1 values (1, NULL);

    SET SYSTEM PARAMETERS 'update_use_attribute_references=yes';

    UPDATE t1 SET c1 = 10, c2 = c1;

    SELECT * FROM t1;

	           c1           c2
	==========================
	           10           10

.. code-block:: sql

    TRUNCATE TABLE t1;
    INSERT INTO t1 values (1, NULL);

    SET SYSTEM PARAMETERS 'update_use_attribute_references=no';

    UPDATE t1 SET c1 = 10, c2 = c1;

    SELECT * FROM t1;

	           c1           c2
	==========================
	           10            1

.. _example_single_table_update_using_analytic_functions:

.. rubric:: 예시 6. 분석 함수를 활용한 단일 테이블 업데이트

**UPDATE** 문에서 단일 테이블만 지정된 경우, **SET** 절에서 분석 함수를 사용할 수 있다.

.. code-block:: sql

    DROP TABLE IF EXISTS t1, t2;

    CREATE TABLE t1 (id INT);
    INSERT INTO t1 VALUES (1), (2), (3), (4), (5);

    CREATE TABLE t2 (id INT, val INT, update_val DOUBLE, join_update_val DOUBLE);
    INSERT INTO t2 (id, val) SELECT a.id, b.id FROM t1 a, t1 b WHERE b.id <= a.id;

    UPDATE t2 SET update_val = AVG (val) OVER (PARTITION BY id);

    SELECT DISTINCT id, TO_CHAR (update_val) AS update_val FROM t2;

	           id  update_val
	===================================
	            1  '1'
	            2  '1.5'
	            3  '2'
	            4  '2.5'
	            5  '3'

.. _example_multiple_tables_update_using_analytic_functions:

.. rubric:: 예시 7. 분석 함수를 활용한 다중 테이블 업데이트

:ref:`example_single_table_update_using_analytic_functions`\에 이어서, **UPDATE** 문에서 여러 개의 테이블이 지정된 경우, **SET** 절에서는 분석 함수를 사용할 수 없다.
하지만, **SET** 절에 **SELECT** 질의를 지정하면, 지정된 테이블 수에 관계없이 **SELECT** 질의 내에서 분석 함수를 사용할 수 있다.

.. code-block:: sql

    UPDATE t1 a, t2 b SET b.join_update_val = AVG (b.val) OVER (PARTITION BY b.id) WHERE a.id = b.id;

	ERROR: before '  where a.id = b.id; '
	Nested analytic functions are not allowed.

.. code-block:: sql

    UPDATE t2 c SET c.join_update_val = (SELECT AVG (b.val) OVER (PARTITION BY b.id) FROM t1 a, t2 b WHERE a.id = b.id AND a.id = c.id LIMIT 1);

    SELECT DISTINCT id, TO_CHAR (join_update_val) AS join_update_val FROM t2;

	           id  join_update_val
	===================================
	            1  '1'
	            2  '1.5'
	            3  '2'
	            4  '2.5'
	            5  '3'

.. _example_remote_table_update:

.. rubric:: 예시 8. 원격 테이블 업데이트

테이블 확장명을 사용하면 로컬 서버뿐만 아니라 원격 서버의 테이블도 업데이트할 수 있다.

.. code-block:: sql

    /**
     * on the remote server (e.g., 192.168.6.21)
     */

    -- DROP SERVER origin;
    CREATE SERVER origin (HOST='localhost', PORT=33000, DBNAME=demodb, USER=public);

    DROP TABLE IF EXISTS t1;

    CREATE TABLE t1 (id int, name varchar, phone varchar);
    INSERT INTO t1@origin VALUES (NULL,  NULL, '000-0000');
    INSERT INTO t1@origin VALUES (   1, 'aaa', '111-1111');
    INSERT INTO t1@origin VALUES (   2, 'bbb', '222-2222');
    INSERT INTO t1@origin VALUES (   3, 'ccc', '333-3333');
    INSERT INTO t1@origin VALUES (   4,  NULL, '000-0000');
    INSERT INTO t1@origin VALUES (   5,  NULL, '000-0000');
    INSERT INTO t1@origin VALUES (   6, 'ddd', '000-0000');
    INSERT INTO t1@origin VALUES (   7,  NULL, '777-7777');

    SELECT * FROM t1;

	           id  name                  phone
	=========================================================
	         NULL  NULL                  '000-0000'
	            1  'aaa'                 '111-1111'
	            2  'bbb'                 '222-2222'
	            3  'ccc'                 '333-3333'
	            4  NULL                  '000-0000'
	            5  NULL                  '000-0000'
	            6  'ddd'                 '000-0000'
	            7  NULL                  '777-7777'

.. code-block:: sql

    /**
     * on the local server (e.g., 192.168.6.22)
     */

    -- DROP SERVER remote;
    CREATE SERVER remote (HOST='192.168.6.21', PORT=33000, DBNAME=demodb, USER=public);

    SELECT * FROM t1@remote WHERE name IS NULL;

	           id  name                  phone
	=========================================================
	         NULL  NULL                  '000-0000'
	            4  NULL                  '000-0000'
	            5  NULL                  '000-0000'
	            7  NULL                  '777-7777'

    UPDATE t1@remote SET name='update', phone='999-9999' WHERE name IS NULL LIMIT 3;

    SELECT * FROM t1@remote;

	           id  name                  phone
	=========================================================
	         NULL  'update'              '999-9999'
	            1  'aaa'                 '111-1111'
	            2  'bbb'                 '222-2222'
	            3  'ccc'                 '333-3333'
	            4  'update'              '999-9999'
	            5  'update'              '999-9999'
	            6  'ddd'                 '000-0000'
	            7  NULL                  '777-7777'

.. _example_local_table_update_using_joins_with_remote_tables:

.. rubric:: 예시 9. 원격 테이블과 조인하여 로컬 테이블 업데이트

.. code-block:: sql

    /**
     * on the remote server (e.g., 192.168.6.21)
     */

    DROP TABLE IF EXISTS t2;

    CREATE TABLE t2 (rate_id INT, rate DOUBLE);

.. code-block:: sql

    /**
     * on the local server (e.g., 192.168.6.22)
     */

    -- DROP SERVER remote;
    CREATE SERVER remote (HOST='192.168.6.21', PORT=33000, DBNAME=demodb, USER=public);

    DROP TABLE IF EXISTS t1;

    CREATE TABLE t1 (id INT PRIMARY KEY, charge DOUBLE);
    INSERT INTO t1 VALUES (1, 100.0);
    INSERT INTO t1 VALUES (2, 100.0);
    INSERT INTO t1 VALUES (3, 100.0);

    INSERT INTO t2@remote VALUES (1, 0.1);
    INSERT INTO t2@remote VALUES (2, 0.2);
    INSERT INTO t2@remote VALUES (3, 0.3);
    INSERT INTO t2@remote VALUES (3, 0.5);

    UPDATE t1 a INNER JOIN t2@remote b ON a.id = b.rate_id
    SET a.charge = a.charge * (1 + b.rate);

    SELECT id, TO_CHAR (charge) AS charge FROM t1;

	           id  charge
	===================================
	            1  '110'
	            2  '120'
	            3  '150'

.. warning::

    **UPDATE** 문에서 로컬 테이블과 원격 테이블을 조인할 때, 원격 테이블의 레코드는 업데이트할 수 없다.

    .. code-block:: sql
    
	UPDATE t1 a INNER JOIN t2@remote b ON a.id = b.rate_id
	SET b.rate = b.rate + 0.1;

	    ERROR: before '  a INNER JOIN t2@remote b ON a.id = b.rate_id
	    SET b.rate = b....'
	    dblink: local mixed remote DML is not allowed
