
:meta-keywords: truncate statement
:meta-description: You can delete all records in the specified table by using the TRUNCATE statement.

********
TRUNCATE
********

**TRUNCATE** 문은 명시된 테이블의 모든 레코드들을 삭제한다.

**WHERE** 조건이 없는 **DELETE FROM** *[schema_name.]table_name* 문을 사용하는 것에 비해 일반적으로 다음의 이점을 지닌다.

* 인덱스와 제약사항 사전 처리, 레코드들 일괄 삭제로 성능면에서 훨씬 뛰어나다.
* 레코드들을 개별 삭제하는 것이 아니기 때문에 각 레코드에 대한 VACUUM 비용이 없다.
* 로그양이 훨씬 적기 때문에 HA 복제, 복구 그리고 롤백(rollback) 등에서 더 좋은 성능을 보인다.

.. note:: 

    * **TRUNCATE** 문을 사용해서 삭제하면 **DELETE** 트리거가 활성화되지 않는다.
    * **TRUNCATE** 문은 해당 테이블의 **AUTO INCREMENT** 칼럼을 초기화하여, 다시 데이터가 입력되면 **AUTO INCREMENT** 칼럼의 초기값부터 생성된다.
    * **TRUNCATE** 문을 수행하려면 해당 테이블에 **ALTER**, **INDEX**, **DELETE** 권한이 필요하다. 권한을 부여하는 방법은 :ref:`granting-authorization` 를 참고한다.
    * 테이블이 **DONT_REUSE_OID** 옵션(:ref:`dont-reuse-oid`)으로 생성된 경우 다른 테이블의 컬럼이 해당 테이블을 참조할 수 있다. 이렇게 외부 참조가 있을 경우 일괄 삭제가 불가능하며 내부적으로 DELETE FROM 연산을 통해 TRUNCATE가 수행된다.

::

    TRUNCATE [ TABLE ] [schema_name.]table_name [ CASCADE ]

*   *schema_name*: 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name* : 삭제할 데이터가 포함되어 있는 테이블의 이름을 지정한다.
*   **CASCADE** : 외래 키 (Foreign Key) 참조 관계에 있는 모든 테이블들의 데이터를 삭제한다. 대상 테이블에 **PRIMARY KEY** 제약 조건이 정의되어 있고, 이 **PRIMARY KEY** 를 하나 이상의 외래 키가 참조하고 있어야 한다. 이 때 외래 키의 **ON DELETE** 동작은 **CASCADE** 로 정의되어 있어야 한다. 외래 키 참조가 있는 경우 **CASCADE** 옵션을 사용하지 않으면, **TRUNCATE** 는 실패한다. 또한, 여러 테이블이 외래 키 참조로 연결되어 있고, 하나라도 **ON DELETE** 동작이 **CASCADE** 가 아닌 경우 역시 실패한다. 외래 키 제약 조건에 대한 자세한 내용은 :ref:`foreign-key-constraint` 을 참고한다.

.. code-block:: sql

    CREATE TABLE a_tbl(A INT AUTO_INCREMENT(3,10) PRIMARY KEY);
    INSERT INTO a_tbl VALUES (NULL),(NULL),(NULL);
    SELECT * FROM a_tbl;
    
::

                a
    =============
                3
                13
                23

.. code-block:: sql

    --AUTO_INCREMENT column value increases from the initial value after truncating the table
    TRUNCATE TABLE a_tbl;
    INSERT INTO a_tbl VALUES (NULL);
    SELECT * FROM a_tbl;
    
::

                a
    =============
                3

다음은 **CASCADE** 옵션을 사용하는 예이다. 일부 레코드의 외래 키 컬럼이 NULL로 어떠한 레코드를 가르키지 않더라도 CASCADE로 전파된 TRUNCATE는 모든 레코드를 제거한다.

.. code-block:: sql
    
    -- a_tbl <- b_tbl <- c_tbl
    --       <- d_tbl
    CREATE TABLE a_tbl(a1 INT PRIMARY KEY);
    CREATE TABLE b_tbl(b1 INT PRIMARY KEY, b2 INT FOREIGN KEY REFERENCES a_tbl (a1) ON DELETE CASCADE);
    CREATE TABLE c_tbl(c1 INT PRIMARY KEY, c2 INT FOREIGN KEY REFERENCES b_tbl (b1) ON DELETE CASCADE);
    CREATE TABLE d_tbl(d1 INT PRIMARY KEY, d2 INT FOREIGN KEY REFERENCES a_tbl (a1) ON DELETE CASCADE);
    INSERT INTO a_tbl VALUES (1);
    INSERT INTO b_tbl VALUES (2, 1);
    INSERT INTO c_tbl VALUES (3, 2);
    INSERT INTO d_tbl VALUES (5, 1);
    INSERT INTO d_tbl VALUES (4, NULL); -- not refer to any record in a_tbl, but will be deleted.
    SELECT * FROM a_tbl;
    SELECT * FROM b_tbl;
    SELECT * FROM c_tbl;
    SELECT * FROM d_tbl;

    TRUNCATE a_tbl CASCADE;
    SELECT * FROM a_tbl, b_tbl, c_tbl, d_tbl;

::

               a1
    =============
                1

               b1           b2
    ==========================
                2            1

               c1           c2
    ==========================
                3            2

               d1           d2
    ==========================
                5            1
                4         NULL
    
    -- after TRUNCATE a_tbl CASCADE;
    There are no results.
