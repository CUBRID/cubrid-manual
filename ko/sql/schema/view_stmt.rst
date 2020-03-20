
:meta-keywords: view definition, create view, drop view, alter view, rename view, create view with check, create updatable view
:meta-description: Define views in CUBRID database using create view, alter view, drop view and rename view statements.

*********
뷰 정의문
*********

CREATE VIEW
===========

뷰(가상 테이블)는 물리적으로 존재하지 않는 가상의 테이블이며, 기존의 테이블이나 뷰에 대한 질의문을 이용하여 뷰를 생성할 수 있다. **VIEW** 와 **VCLASS** 는 동의어로 사용된다. 

**CREATE VIEW** 문을 이용하여 뷰를 생성한다. 뷰 이름 작성 원칙은 :doc:`/sql/identifier`\ 를 참고한다.

::

    CREATE [OR REPLACE] {VIEW | VCLASS} view_name
    [<subclass_definition>]
    [(view_column_name [COMMENT 'column_comment_string'], ...)]
    [INHERIT <resolution>, ...]
    [AS <select_statement>]
    [WITH CHECK OPTION] 
    [COMMENT [=] 'view_comment_string'];
                                    
        <subclass_definition> ::= {UNDER | AS SUBCLASS OF} table_name, ...
        <resolution> ::= [CLASS | TABLE] {column_name} OF superclass_name [AS alias]

*   **OR REPLACE**: **CREATE** 뒤에 **OR REPLACE** 키워드가 명시되면, *view_name*\ 이 기존의 뷰와 이름이 중복되더라도 에러를 출력하지 않고 기존의 뷰를 새로운 뷰로 대체한다.

*   *view_name*: 생성하려는 뷰의 이름을 지정한다. 뷰의 이름은 데이터베이스 내에서 고유해야 한다.
*   *view_column_name*: 생성하려는 뷰의 칼럼 이름을 지정한다.
*   **AS** <*select_statement*>: 유효한 **SELECT** 문이 명시되어야 한다. 이를 기반으로 뷰가 생성된다.
*   **WITH CHECK OPTION**: 이 옵션이 명시되면 <*select_statement*> 내 **WHERE** 절에 명시된 조건식을 만족하는 경우에만 업데이트 또는 삽입이 가능하다. 조건식을 위반하는 가상 테이블에 대한 갱신을 허용하지 않기 위해서 사용한다.
*   *view_comment_string*: 뷰의 커멘트를 지정한다.
*   *column_comment_string*: 칼럼의 커멘트를 지정한다.

.. code-block:: sql

    CREATE TABLE a_tbl (
        id INT NOT NULL,
        phone VARCHAR(10)
    );
    INSERT INTO a_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL), (5, NULL);
    
    --creating a new view based on AS select_statement from a_tbl
    CREATE VIEW b_view AS SELECT * FROM a_tbl WHERE phone IS NOT NULL WITH CHECK OPTION;
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
     
.. code-block:: sql

    --WITH CHECK OPTION doesn't allow updating column value which violates WHERE clause
    UPDATE b_view SET phone=NULL;
     
::

    ERROR: Check option exception on view b_view.

다음은 기존 뷰의 정의를 갱신한다. 이와 함께 뷰에 커멘트를 추가하고 있다.

.. code-block:: sql

    --creating view which name is as same as existing view name
    CREATE OR REPLACE VIEW b_view AS SELECT * FROM a_tbl ORDER BY id DESC COMMENT 'changed view';
     
    --the existing view has been replaced as a new view by OR REPLACE keyword
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                5  NULL
                4  NULL
                3  '333-3333'
                2  '222-2222'
                1  '111-1111'

다음은 뷰의 칼럼에 커멘트를 추가한다.

.. code-block:: sql

    CREATE OR REPLACE VIEW b_view(a COMMENT 'column id', b COMMENT 'column phone') AS SELECT * FROM a_tbl ORDER BY id DESC;

업데이트 가능한 VIEW의 생성 조건
--------------------------------

다음의 조건을 만족한다면 해당 뷰를 업데이트할 수 있다.

*   **FROM** 절은 반드시 업데이트 가능한 테이블이나 뷰만 포함해야 한다.

    CUBRID 9.0 미만 버전에서는 **FROM** 절에 업데이트 가능한 테이블을 포함할 경우 반드시 하나의 테이블만 포함해야 했다. 단, FROM (class_x, class_y)와 같이 괄호에 포함된 두 테이블은 하나의 테이블로 표현되므로 업데이트할 수 있었다. CUBRID 9.0 이상 버전에서는 업데이트 가능한 두 개 이상의 테이블을 허용한다.

*   **JOIN** 구문을 포함할 수 있다.

    .. note:: CUBRID 10.0 미만 버전에서는 뷰에 **JOIN** 구문을 포함한 뷰를 업데이트 할 수 없다.

*   **DISTINCT**, **UNIQUE** 구문을 포함하지 않는다.
*   **GROUP BY ... HAVING** 구문을 포함하지 않는다.
*   **SUM** ( ), **AVG** ( )와 같은 집계 함수를 포함하지 않는다.
*   **UNION** 이 아닌 **UNION ALL** 을 사용하여 업데이트 가능한 질의만으로 질의를 구성한 경우 업데이트할 수 있다. 단, 테이블은 **UNION ALL** 을 구성하는 질의 중 어느 한 질의에만 존재해야 한다.
*   **UNION ALL** 구문을 사용하여 생성된 뷰에 레코드를 입력하는 경우, 레코드가 입력될 테이블은 시스템이 결정한다. 레코드가 입력될 테이블을 사용자가 제어하는 것은 불가능하므로 사용자가 제어하기 원한다면 테이블에 직접 입력하거나 입력을 위한 별도의 뷰를 생성해야 한다.

뷰가 위의 규칙을 모두 충족해도, 해당 뷰의 다음과 같은 칼럼은 업데이트할 수 없다.

*   경로 표현식(예: *tbl_name.col_name*)
*   산술 연산자가 포함된 숫자 타입의 칼럼

뷰에 정의된 칼럼이 업데이트 가능하더라도 **FROM** 구문에 포함된 테이블에 대해 업데이트를 위한 적절한 권한이 있어야 하며 뷰에 대한 접근 권한이 있어야 한다. 뷰에 접근 권한을 부여하는 방법은 테이블에 접근 권한을 부여하는 방식과 동일하다. 권한 부여에 대한 자세한 내용은 :ref:`granting-authorization` 를 참조한다.

뷰의 커멘트
-----------

뷰의 커멘트를 다음과 같이 명시할 수 있다. 

.. code-block:: sql

    CREATE OR REPLACE VIEW b_view AS SELECT * FROM a_tbl ORDER BY id DESC COMMENT 'changed view';

명시된 뷰의 커멘트는 다음 구문에서 확인할 수 있다.

.. code-block:: sql

    SHOW CREATE VIEW view_name;
    SELECT vclass_name, comment from db_vclass;

또는 CSQL 인터프리터에서 스키마를 출력하는 ;sc 명령으로 뷰의 커멘트를 확인할 수 있다.

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc b_view

뷰의 각 칼럼에도 커멘트 추가가 가능하다.

.. code-block:: sql

    CREATE OR REPLACE VIEW b_view (a COMMENT 'a comment', b COMMENT 'b comment') 
    AS SELECT * FROM a_tbl ORDER BY id DESC COMMENT 'view comment';
    
뷰 커멘트의 변경은 아래의 ALTER VIEW 구문을 참고한다.

ALTER VIEW
==========

ADD QUERY 절
------------

**ALTER VIEW** 문에 **ADD QUERY** 절을 사용하여 뷰의 질의 명세부에 질의를 추가할 수 있다. 뷰 생성 시 정의된 질의문에는 1이 부여되고, **ADD QUERY** 절에서 추가한 질의문에는 2가 부여된다. ::

    ALTER [VIEW | VCLASS] view_name
    ADD QUERY <select_statement>
    [INHERIT <resolution> , ...] ;
     
        <resolution> ::= {column_name} OF superclass_name [AS alias]

*   *view_name*: 질의를 추가할 뷰의 이름 명시한다.
*   <*select_statement*>: 추가할 질의를 명시한다.

.. code-block:: sql

    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                4  NULL
                5  NULL
     
.. code-block:: sql
     
    ALTER VIEW b_view ADD QUERY SELECT * FROM a_tbl WHERE id IN (1,2);
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                4  NULL
                5  NULL
                1  '111-1111'
                2  '222-2222'

AS SELECT 절
------------

**ALTER VIEW** 문에 **AS SELECT** 절을 사용하여 가상 테이블에 정의된 **SELECT** 질의를 변경할 수 있다. 이는 **CREATE OR REPLACE** 문과 유사하게 동작한다. **ALTER VIEW** 문의 **CHANGE QUERY** 절에 질의 번호 1을 명시하여 질의를 변경할 수도 있다. ::

    ALTER [VIEW | VCLASS] view_name AS <select_statement> ;

*   *view_name*: 변경할 가상 테이블의 이름을 명시한다.
*   <*select_statement*>: 가상 테이블 생성 시 정의된 **SELECT** 문을 대체할 새로운 질의문을 명시한다.

.. code-block:: sql

    ALTER VIEW b_view AS SELECT * FROM a_tbl WHERE phone IS NOT NULL;
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'

CHANGE QUERY 절
---------------

**ALTER VIEW** 문의 **CHANGE QUERY** 절을 사용하여 뷰 질의 명세부에 정의된 질의를 변경할 수 있다. ::

    ALTER [VIEW | VCLASS] view_name
    CHANGE QUERY [integer] <select_statement> ;

*   *view_name*: 변경할 뷰의 이름을 명시한다.
*   *integer*: 변경할 질의의 번호를 명시한다. 기본값은 1이다.
*   <*select_statement*>: 질의 번호가 *integer* 인 질의를 대치할 새로운 질의를 명시한다.

.. code-block:: sql

    --adding select_statement which query number is 2 and 3 for each
    ALTER VIEW b_view ADD QUERY SELECT * FROM a_tbl WHERE id IN (1,2);
    ALTER VIEW b_view ADD QUERY SELECT * FROM a_tbl WHERE id = 3;
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                4  NULL
                5  NULL
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
     
.. code-block:: sql

    --altering view changing query number 2
    ALTER VIEW b_view CHANGE QUERY 2 SELECT * FROM a_tbl WHERE phone IS NULL;
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                4  NULL
                5  NULL
                4  NULL
                5  NULL
                3  '333-3333'

DROP QUERY 절
-------------

**ALTER VIEW** 문의 **DROP QUERY** 예약어를 이용하여 뷰 질의 명세부에 정의된 질의를 삭제할 수 있다.

.. code-block:: sql

    ALTER VIEW b_view DROP QUERY 2,3;
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                4  NULL
                5  NULL

COMMENT 절
----------

**ALTER VIEW** 문의 **COMMENT** 절을 이용하여 뷰의 커멘트를 변경할 수 있다.

::

    ALTER [VIEW | VCLASS] view_name COMMENT [=] 'view_comment';

.. code-block:: sql

    ALTER VIEW b_view COMMENT = 'changed view comment';

DROP VIEW
=========

뷰는 **DROP VIEW** 문을 이용하여 삭제할 수 있다. 뷰를 삭제하는 방법은 일반 테이블을 삭제하는 방법과 동일하다. IF EXISTS 절을 함께 사용하면 해당 뷰가 존재하지 않더라도 에러가 발생하지 않는다. ::

    DROP [VIEW | VCLASS] [IF EXISTS] view_name [{ ,view_name , ... }] ;

*   *view_name* : 삭제하려는 뷰의 이름을 지정한다.

.. code-block:: sql

    DROP VIEW b_view;

RENAME VIEW
===========

뷰의 이름은 **RENAME VIEW** 문을 사용하여 변경할 수 있다. ::

    RENAME [VIEW | VCLASS] old_view_name {AS | TO} new_view_name[, old_view_name {AS | TO} new_view_name, ...] ;

*   *old_view_name* : 변경할 뷰의 이름을 지정한다.
*   *new_view_name* : 뷰의 새로운 이름을 지정한다.

다음은 *game_2004* 뷰의 이름을 *info_2004* 로 변경하는 예제이다.

.. code-block:: sql

    RENAME VIEW game_2004 AS info_2004;
