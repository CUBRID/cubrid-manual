
:meta-keywords: truncate statement
:meta-description: You can delete all records in the specified table by using the TRUNCATE statement.

********
TRUNCATE
********

**TRUNCATE** 문은 명시된 테이블의 모든 레코드들을 삭제한다.

내부적으로 테이블에 정의된 모든 인덱스와 제약 조건을 먼저 삭제한 후 레코드를 삭제하기 때문에, **WHERE** 조건이 없는 **DELETE FROM** *table_name* 문을 사용하는 것보다 빠르다. **TRUNCATE** 문을 사용해서 삭제하면 **ON DELETE** 트리거가 활성화되지 않는다.

대상 테이블에 **PRIMARY KEY** 제약 조건이 정의되어 있고, 이 **PRIMARY KEY** 를 하나 이상의 **FOREIGN KEY** 가 참조하고 있는 경우에는 **FOREIGN KEY ACTION** 을 따른다. **FOREIGN KEY** 의 **ON DELETE** 액션이 **RESTRICT** 나 **NO ACTION** 이면 **TRUNCATE** 문은 에러를 반환하고, **CASCADE** 이면 **FOREIGN KEY** 도 함께 삭제한다. **TRUNCATE** 문은 해당 테이블의 **AUTO INCREMENT** 칼럼을 초기화하여, 다시 데이터가 입력되면 **AUTO INCREMENT** 칼럼의 초기값부터 생성된다.

.. note:: 

    **TRUNCATE** 문을 수행하려면 해당 테이블에 **ALTER**, **INDEX**, **DELETE** 권한이 필요하다. 권한을 부여하는 방법은 :ref:`granting-authorization` 를 참고한다.

::

    TRUNCATE [ TABLE ] <table_name>

*   *table_name* : 삭제할 데이터가 포함되어 있는 테이블의 이름을 지정한다.

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
