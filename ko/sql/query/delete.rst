
:meta-keywords: delete statement
:meta-description: You can delete records in the table by using the DELETE statement.


******
DELETE
******

**DELETE** 문을 사용하여 테이블 내에 레코드를 삭제할 수 있으며, :ref:`where-clause`\ 과 결합하여 삭제 조건을 명시할 수 있다. 하나의 **DELETE** 문으로 하나 이상의 테이블을 삭제할 수 있다. 

::
 
    <DELETE single table>
    DELETE [FROM] table_name [ WHERE <search_condition> ] [LIMIT row_count]
     
    <DELETE multiple tables FROM ...>
    DELETE table_name[, table_name] ... FROM <table_specifications> [ WHERE <search_condition> ]
     
    <DELETE FROM multiple tables USING ...>
    DELETE FROM table_name[, table_name] ... USING <table_specifications> [ WHERE <search_condition> ]

*   <*table_specifications*>: **SELECT** 문의 **FROM** 절과 같은 형태의 구문을 지정할 수 있으며, 하나 이상의 테이블을 지정할 수 있다.

*   *table_name*: 삭제할 데이터가 포함되어 있는 테이블의 이름을 지정한다. 테이블의 개수가 한 개일 경우 앞의 **FROM** 키워드를 생략할 수 있다.

*   *search_condition*: :ref:`where-clause`\ 을 이용하여 *search_condition*\ 을 만족하는 데이터만 삭제한다. 생략할 경우 지정된 테이블의 모든 데이터를 삭제한다.

*   *row_count*: :ref:`limit-clause` 에서 삭제할 레코드 수를 지정한다. 부호 없는 정수, 호스트 변수 또는 간단한 표현식 중 하나일 수 있다.

삭제할 테이블이 한 개인 경우에 한하여, :ref:`limit-clause`\ 을 지정할 수 있다. :ref:`limit-clause`\ 을 명시하면 삭제할 레코드 수를 한정할 수 있다. :ref:`where-clause`\ 을 만족하는 레코드 개수가 *row_count*\ 를 초과하면 *row_count* 개의 레코드만 삭제된다.

.. note:: \

    *   여러 개의 테이블이 있는(multiple table) **DELETE** 문에서는 <*table_specifications*> 내에서만 테이블 별칭(alias)을 정의할 수 있고, <*table_specifications*> 밖에서는 <*table_specifications*> 내에서 정의한 테이블 별칭만 사용할 수 있다.

    *   CUBRID 9.0 미만 버전에서는 <*table_specifications*>에 한 개의 테이블만 입력할 수 있다.

.. code-block:: sql

    CREATE TABLE a_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));
    INSERT INTO a_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL), (5, NULL);
     
    --delete one record only from a_tbl
    DELETE FROM a_tbl WHERE phone IS NULL LIMIT 1;
    SELECT * FROM a_tbl;
    
::
    
               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                5  NULL
     
.. code-block:: sql

    --delete all records from a_tbl
    DELETE FROM a_tbl;

아래 테이블들은 **DELETE JOIN**\ 을 설명하기 위해 생성한 것이다.
    
.. code-block:: sql

    CREATE TABLE a_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));
    CREATE TABLE b_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));
    CREATE TABLE c_tbl(
        id INT NOT NULL,
        phone VARCHAR(10));
     
    INSERT INTO a_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL), (5, NULL);
    INSERT INTO b_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL);
    INSERT INTO c_tbl VALUES(1,'111-1111'), (2,'222-2222'), (10, '333-3333'), (11, NULL), (12, NULL);

다음 질의들은 여러 개의 테이블들을 조인한 후 삭제를 수행하며, 모두 같은 결과를 보여준다.

.. code-block:: sql

    -- Below four queries show the same result.
    --  <DELETE multiple tables FROM ...>
     
    DELETE a, b FROM a_tbl a, b_tbl b, c_tbl c
    WHERE a.id=b.id AND b.id=c.id;
     
    DELETE a, b FROM a_tbl a INNER JOIN b_tbl b ON a.id=b.id
    INNER JOIN c_tbl c ON b.id=c.id;
     
    -- <DELETE FROM multiple tables USING ...>
     
    DELETE FROM a, b USING a_tbl a, b_tbl b, c_tbl c
    WHERE a.id=b.id AND b.id=c.id;
     
    DELETE FROM a, b USING a_tbl a INNER JOIN b_tbl b ON a.id=b.id
    INNER JOIN c_tbl c ON b.id=c.id;

조인 구문에 대한 자세한 설명은 :ref:`join-query`\ 를 참고한다.
