
:meta-keywords: select statement, select from clause, select where clause, group by, having clause, limit clause, join query, subquery, select for update
:meta-description: The SELECT statement specifies columns that you want to retrieve from a table.


******
SELECT
******

**SELECT** 문은 지정된 테이블에서 원하는 칼럼을 조회한다. ::

    SELECT [ <qualifier> ] <select_expressions>
        [{TO | INTO} <variable_comma_list>]
        [FROM <extended_table_specification_comma_list>]
        [WHERE <search_condition>]
        [GROUP BY {col_name | expr} [ASC | DESC], ...[WITH ROLLUP]]
        [HAVING  <search_condition> ]
        [ORDER BY {col_name | expr} [ASC | DESC], ... [NULLS {FIRST | LAST}]
        [LIMIT [offset,] row_count]
        [USING INDEX { index_name [,index_name, ...] | NONE }]
        [FOR UPDATE [OF <spec_name_comma_list>]]
        
        <qualifier> ::= ALL | DISTINCT | DISTINCTROW | UNIQUE
    
        <select_expressions> ::= * | <expression_comma_list> | *, <expression_comma_list>
     
        <variable_comma_list> ::= [:] identifier, [:] identifier, ...
    
        <extended_table_specification_comma_list> ::=
            <table_specification>   [   
                                        {, <table_specification> } ... |
                                        <join_table_specification> ... |
                                        <join_table_specification2> ...
                                    ]
     
    <table_specification> ::=
        <single_table_spec> [<correlation>] |
        <metaclass_specification> [ <correlation> ] |
        <subquery> <correlation> |
        TABLE ( <expression> ) <correlation>

    <correlation> ::= [AS] <identifier> [(<identifier_comma_list>)]
     
    <single_table_spec> ::= [ONLY] <table_name> |
                          ALL <table_name> [ EXCEPT <table_name> ]
     
    <metaclass_specification> ::= CLASS <class_name>
     
    <join_table_specification> ::=
        {
            [INNER | {LEFT | RIGHT} [OUTER]] JOIN 

        } <table_specification> ON <search_condition>
     
    <join_table_specification2> ::= 
        { 
            CROSS JOIN | 
            NATURAL [ LEFT | RIGHT ] JOIN 
        } <table_specification>
    

*   *qualifier*: 한정어. 생략이 가능하며 지정하지 않을 경우에는 **ALL** 로 지정된다.

    *   **ALL**: 테이블의 모든 레코드를 조회한다.
    *   **DISTINCT**: 중복을 허용하지 않고 유일한 값을 갖는 레코드에 대해서만 조회한다. **DISTINCTROW**, **UNIQUE** 와 동일하다.

*   <*select_expressions*>

    *   \*: **SELECT** * 구문을 사용하면 **FROM** 절에서 명시한 테이블에 대한 모든 칼럼을 조회할 수 있다.

    *   *expression_comma_list* : *expression* 은 칼럼 이름이나 경로 표현식(예: *tbl_name.col_name*), 변수, 테이블 이름이 될 수 있으며 산술 연산을 포함하는 일반적인 표현식도 모두 사용될 수 있다. 쉼표(,)는 리스트에서 개별 표현식을 구분하는데 사용된다. 조회하고자 하는 칼럼 또는 연산식에 대해 **AS** 키워드를 사용하여 별칭(alias)를 지정할 수 있으며, 지정된 별칭은 칼럼 이름으로 사용되어 **GROUP BY**, **HAVING**, **ORDER BY** 절 내에서 사용될 수 있다. 칼럼의 위치 인덱스(position)는 칼럼이 명시된 순서대로 부여되며, 시작 값은 1이다.

        *expression*\ 에는 **AVG**, **COUNT**, **MAX**, **MIN**, **SUM** 과 같이 조회된 데이터를 조작하는 집계 함수가 사용될 수 있다. 

*   *table_name*.\*: 테이블 이름을 지정한다. \*을 사용하면 명시한 테이블의 모든 칼럼을 지정하는 것과 같다.

*   *variable_comma_list*: *select_expressions* 이 조회하는 데이터는 하나 이상의 변수에 저장될 수 있다.

*   [:]\ *identifier*: **TO** (또는 **INTO**) 다음에 *:identifier*\를  조회하는 데이터를 ':identifier'의 변수에 저장할 수 있다.

*   <*single_table_spec*>

    *   **ONLY** 키워드 뒤에 수퍼클래스 이름이 명시되면, 해당 수퍼클래스만 선택하고 이를 상속받는 서브클래스는 선택하지 않는다.
    *   **ALL** 키워드 뒤에 수퍼클래스 이름이 지정되면, 해당 수퍼클래스 및 이를 상속받는 서브클래스를 모두 선택한다.
    *   **EXCEPT** 키워드 뒤에 선택하지 않을 서브클래스 리스트를 명시할 수 있다.

다음은 역대 올림픽이 개최된 국가를 중복 없이 조회한 예제이다. 이 예제는 *demodb* 의 *olympic* 테이블을 대상으로 수행하였다. **DISTINCT** 또는 **UNIQUE** 키워드는 질의 결과가 유일한 값만을 갖도록 만든다. 예를 들어 *host_nation* 값이 'Greece'인 *olympic* 인스턴스가 여러 개일 때 질의 결과에는 하나의 값만 나타나도록 할 경우에 사용된다.

.. code-block:: sql

    SELECT DISTINCT host_nation 
    FROM olympic;

::

      host_nation
    ======================
      'Australia'
      'Belgium'
      'Canada'
      'Finland'
      'France'
    ...

다음은 조회하고자 하는 칼럼에 칼럼 별칭을 부여하고, **ORDER BY** 절에서 칼럼 별칭을 이용하여 결과 레코드를 정렬하는 예제이다. 이때, **LIMIT** 절을 사용하여 결과 레코드 수를 5개로 제한한다.

.. code-block:: sql

    SELECT host_year as col1, host_nation as col2 
    FROM olympic 
    ORDER BY col2 LIMIT 5;
    
::
    
             col1  col2
    ===================================
             2000  'Australia'
             1956  'Australia'
             1920  'Belgium'
             1976  'Canada'
             1948  'England'
     
.. code-block:: sql

    SELECT CONCAT(host_nation, ', ', host_city) AS host_place 
    FROM olympic
    ORDER BY host_place LIMIT 5;
    
::
    
      host_place
    ======================
      'Australia,  Melbourne'
      'Australia,  Sydney'
      'Belgium,  Antwerp'
      'Canada,  Montreal'
      'England,  London'

FROM 절
=======

**FROM** 절은 질의에서 데이터를 조회하고자 하는 테이블을 지정한다. 어떤 테이블도 참조하지 않는 경우에는 **FROM** 절을 생략할 수도 있다. 조회할 수 있는 경로는 다음과 같다.

*   개별 테이블(single table)
*   부질의(subquery)
*   유도 테이블(derived table)

::

    SELECT [<qualifier>] <select_expressions>
    [
        FROM <table_specification> [ {, <table_specification> | <join_table_specification> }... ]
    ]
     
    <select_expressions> ::= * | <expression_comma_list> | *, <expression_comma_list>
     
    <table_specification> ::=
        <single_table_spec> [<correlation>] |
        <metaclass_specification> [<correlation>] |
        <subquery> <correlation> |
        TABLE (<expression>) <correlation>
     
    <correlation> ::= [AS] <identifier> [(<identifier_comma_list>)]
     
    <single_table_spec> ::= [ONLY] <table_name> |
                          ALL <table_name> [EXCEPT <table_name>]
     
    <metaclass_specification> ::= CLASS <class_name>
     

*   <*select_expressions*>: 조회하고자 하는 칼럼 또는 연산식을 하나 이상 지정할 수 있으며, 테이블 내 모든 칼럼을 조회할 때에는 * 를 지정한다. 조회하고자 하는 칼럼 또는 연산식에 대해 **AS** 키워드를 사용하여 별칭(alias)를 지정할 수 있으며, 지정된 별칭은 칼럼 이름으로 사용되어 **GROUP BY**, **HAVING**, **ORDER BY** 절 내에서 사용될 수 있다. 칼럼의 위치 인덱스(position)는 칼럼이 명시된 순서대로 부여되며, 시작 값은 1이다.

*   <*table_specification*>: **FROM** 절 뒤에 하나 이상의 테이블 이름이 명시되며, 부질의와 유도 테이블도 지정될 수 있다. 부질의 유도 테이블에 대한 설명은 :ref:`subquery-derived-table`\ 을 참고한다.

.. code-block:: sql

    --FROM clause can be omitted in the statement
    SELECT 1+1 AS sum_value;
    
::

        sum_value
    =============
                2
     
.. code-block:: sql

    SELECT CONCAT('CUBRID', '2008' , 'R3.0') AS db_version;
    
::

      db_version
    ======================
      'CUBRID2008R3.0'

유도 테이블
-----------

질의문에서 **FROM** 절의 테이블 명세 부분에 부질의가 사용될 수 있다. 이런 형태의 부질의는 부질의 결과가 테이블로 취급되는 유도 테이블(derived table)을 만든다. 

또한 유도 테이블은 집합 값을 갖는 속성의 개별 원소를 접근하는데 사용된다. 이 경우 집합 값의 한 원소는 유도 테이블에서 하나의 레코드로 생성된다.

.. _subquery-derived-table:

부질의 유도 테이블
------------------

유도 테이블의 각 레코드는 **FROM** 절에 주어진 부질의의 결과로부터 만들어진다. 부질의로부터 생성되는 유도 테이블은 임의의 개수의 칼럼과 레코드를 가질 수 있다. 

::

    FROM (subquery) [AS] [derived_table_name [(column_name [{, column_name } ... ])]]

*   *column_name* 파라미터의 개수와 *subquery* 에서 만들어지는 칼럼의 개수는 일치해야 한다.
*   *derived_table_name*\ 을 생략할 수 있다.

다음은 한국이 획득한 금메달 개수와 일본이 획득한 은메달 개수를 더한 값을 조회하는 예제이다. 이 예제는 유도 테이블을 이용하여 부질의의 중간 결과를 모으고 하나의 결과로 처리하는 방법을 보여준다. 이 질의는 *nation_code* 칼럼이 'KOR'인 *gold* 값과 *nation_code* 칼럼이 'JPN'인 *silver* 값의 전체 합을 반환한다.

.. code-block:: sql

    SELECT SUM (n) 
    FROM (SELECT gold FROM participant WHERE nation_code = 'KOR'
          UNION ALL 
          SELECT silver FROM participant WHERE nation_code = 'JPN') AS t(n);

부질의 유도 테이블은 외부 질의와 연관되어 있을 때 유용하게 사용할 수 있다. 예를 들어 **WHERE** 절에서 사용된 부질의의 **FROM** 절에 유도 테이블이 사용될 수 있다. 
다음은 은메달 및 동메달을 하나 이상 획득한 경우, 해당 은메달과 동메달의 합의 평균보다 많은 수의 금메달을 획득한 *nation_code*, *host_year*, *gold* 필드를 보여주는 질의 예제이다. 이 예제에서는 질의(외부 **SELECT** 절)와 부질의(내부 **SELECT** 절)가 *nation_code* 속성으로 연결되어 있다.

.. code-block:: sql

    SELECT nation_code, host_year, gold
    FROM participant p
    WHERE gold > (SELECT AVG(s)
                  FROM (SELECT silver + bronze
                        FROM participant
                        WHERE nation_code = p.nation_code
                        AND silver > 0
                        AND bronze > 0)
                       AS t(s));
              
::

      nation_code      host_year      gold
    =========================================
      'JPN'                2004         16
      'CHN'                2004         32
      'DEN'                1996          4
      'ESP'                1992         13

.. _where-clause:

WHERE 절
========

질의에서 칼럼은 조건에 따라 처리될 수 있다. **WHERE** 절은 조회하려는 데이터의 조건을 명시한다. ::

    WHERE <search_condition>

        <search_condition> ::=
            <comparison_predicate>
            <between_predicate>
            <exists_predicate>
            <in_predicate>
            <null_predicate>
            <like_predicate>
            <quantified_predicate>
            <set_predicate>

**WHERE** 절은 *search_condition* 또는 질의에서 조회되는 데이터를 결정하는 조건식을 지정한다. 조건식이 참인 데이터만 질의 결과로 조회된다(**NULL** 값은 알 수 없는 값으로서 질의 결과로 조회되지 않는다).

*   *search_condition*: 자세한 내용은 다음의 항목을 참고한다.

    *   :ref:`basic-cond-expr`
    *   :ref:`between-expr`
    *   :ref:`exists-expr`
    *   :ref:`in-expr`
    *   :ref:`is-null-expr`
    *   :ref:`like-expr`
    *   :ref:`any-some-all-expr`

복수의 조건은 논리연산자 **AND**, **OR** 를 사용할 수 있다. **AND** 가 지정된 경우 모든 조건이 참이어야 하고, **OR** 로 지정된 경우에는 하나의 조건만 참이어도 된다. 만약 키워드 **NOT** 이 조건 앞에 붙는다면 조건은 반대의 의미를 갖는다. 논리 연산이 평가되는 순서는 다음 표와 같다.

+----------+---------+-----------------------------------------------------------+
| 우선순위 | 연산자  | 기능                                                      |
+==========+=========+===========================================================+
| 1        | **( )** | 괄호 내에 포함된 논리 표현식은 첫 번째로 평가된다.        |
+----------+---------+-----------------------------------------------------------+
| 2        | **NOT** | 논리 표현식의 결과를 부정한다.                            |
+----------+---------+-----------------------------------------------------------+
| 3        | **AND** | 논리 표현식에 포함된 모든 조건이 참이어야 한다.           |
+----------+---------+-----------------------------------------------------------+
| 4        | **OR**  | 논리 표현식에 포함된 조건 중 하나의 조건은 참이어야 한다. |
+----------+---------+-----------------------------------------------------------+

.. _group-by-clause:

GROUP BY ... HAVING 절
======================

**SELECT** 문으로 검색한 결과를 특정 칼럼을 기준으로 그룹화하기 위해 **GROUP BY** 절을 사용하며, 그룹별로 정렬을 수행하거나 집계 함수를 사용하여 그룹별 집계를 구할 때 사용한다. 그룹이란 **GROUP BY** 절에 명시된 칼럼에 대해 동일한 칼럼 값을 가지는 레코드들을 의미한다.

**GROUP BY** 절 뒤에 **HAVING** 절을 결합하여 그룹 선택을 위한 조건식을 설정할 수 있다. 즉, **GROUP BY** 절로 구성되는 모든 그룹 중 **HAVING** 절에 명시된 조건식을 만족하는 그룹만 조회한다.

SQL 표준에서는 **GROUP BY** 절에서 명시되지 않은 칼럼(hidden column)을 **SELECT** 칼럼 리스트에 명시할 수 없지만, CUBRID는 문법을 확장하여 **GROUP BY** 절에서 명시되지 않은 칼럼도 **SELECT** 칼럼 리스트에 명시할 수 있다. 확장된 문법을 사용하지 않으려면 **only_full_group_by** 파라미터 값을 yes로 설정해야 한다. 이에 대한 자세한 내용은 :ref:`stmt-type-parameters` 를 참고한다. 

::

    SELECT ...
    GROUP BY {col_name | expr | position} [ASC | DESC], ...
              [WITH ROLLUP] [HAVING <search_condition>]

*   *col_name* | *expr* | *position*: 하나 이상의 칼럼 이름, 표현식, 별칭 또는 칼럼 위치가 지정될 수 있으며, 각 항목은 쉼표로 구분된다. 이를 기준으로 칼럼들이 정렬된다.

*   [**ASC** | **DESC**]: **GROUP BY** 절 내에 명시된 칼럼 뒤에 **ASC** 또는 **DESC** 의 정렬 옵션을 명시할 수 있다. 정렬 옵션이 명시되지 않으면 기본 옵션은 **ASC** 가 된다.

*   <*search_condition*>: **HAVING** 절에 검색 조건식을 명시한다. **HAVING** 절에서는 **GROUP BY** 절 내에 명시된 칼럼과 별칭, 또는 집계 함수에서 사용되는 칼럼을 참조할 수 있다.

    .. note:: cubrid.conf의 only_full_group_by 파라미터의 값이 yes인 경우 **GROUP BY** 절에서 명시되지 않은 칼럼(hidden columns)을 참조할 수도 있는데, 이때 HAVING 조건은 질의 결과에 영향을 끼치지 않는다.

*   **WITH ROLLUP**: **GROUP BY** 절에 **WITH ROLLUP** 수정자를 명시하면, **GROUP BY** 된 칼럼 각각에 대한 결과 값이 그룹별로 집계되고 나서, 해당 그룹 행의 전체를 집계한 결과 값이 추가로 출력된다. 즉, 그룹별로 집계한 값에 대해 다시 전체 집계를 수행한다. 그룹 대상 칼럼이 두 개 이상일 경우 앞의 그룹을 큰 단위, 뒤의 그룹을 작은 단위로 간주하여 작은 단위 별 전체 집계 행과 큰 단위의 전체 집계 행이 추가된다. 예를 들어 부서별, 사람별 영업 실적의 집계를 하나의 질의문으로 확인할 수 있다.

.. code-block:: sql

    -- creating a new table
    CREATE TABLE sales_tbl
    (dept_no INT, name VARCHAR(20), sales_month INT, sales_amount INT DEFAULT 100, PRIMARY KEY (dept_no, name, sales_month));
    
    INSERT INTO sales_tbl VALUES
    (201, 'George' , 1, 450), (201, 'George' , 2, 250), (201, 'Laura'  , 1, 100), (201, 'Laura'  , 2, 500),
    (301, 'Max'    , 1, 300), (301, 'Max'    , 2, 300),
    (501, 'Stephan', 1, 300), (501, 'Stephan', 2, DEFAULT), (501, 'Chang'  , 1, 150),(501, 'Chang'  , 2, 150),
    (501, 'Sue'    , 1, 150), (501, 'Sue'    , 2, 200);
     
    -- selecting rows grouped by dept_no
    SELECT dept_no, avg(sales_amount) 
    FROM sales_tbl
    GROUP BY dept_no;
    
::

          dept_no         avg(sales_amount)
    =======================================
              201     3.250000000000000e+02
              301     3.000000000000000e+02
              501     1.750000000000000e+02
    
.. code-block:: sql

    -- conditions in WHERE clause operate first before GROUP BY
    SELECT dept_no, avg(sales_amount) 
    FROM sales_tbl
    WHERE sales_amount > 100 
    GROUP BY dept_no;
    
::

          dept_no         avg(sales_amount)
    =======================================
              201     4.000000000000000e+02
              301     3.000000000000000e+02
              501     1.900000000000000e+02
     
.. code-block:: sql

    -- conditions in HAVING clause operate last after GROUP BY
    SELECT dept_no, avg(sales_amount) 
    FROM sales_tbl
    WHERE sales_amount > 100 
    GROUP BY dept_no HAVING avg(sales_amount) > 200;
    
::

          dept_no         avg(sales_amount)
    =======================================
              201     4.000000000000000e+02
              301     3.000000000000000e+02
     
.. code-block:: sql

    -- selecting and sorting rows with using column alias
    SELECT dept_no AS a1, avg(sales_amount) AS a2 
    FROM sales_tbl
    WHERE sales_amount > 200 GROUP 
    BY a1 HAVING a2 > 200 
    ORDER BY a2;
    
::

               a1                        a2
    =======================================
              301     3.000000000000000e+02
              501     3.000000000000000e+02
              201     4.000000000000000e+02
     
.. code-block:: sql

    -- selecting rows grouped by dept_no, name with WITH ROLLUP modifier
    SELECT dept_no AS a1, name AS a2, avg(sales_amount) AS a3 
    FROM sales_tbl
    WHERE sales_amount > 100 
    GROUP BY a1, a2 WITH ROLLUP;
    
::

               a1  a2                                          a3
    =============================================================
              201  'George'                 3.500000000000000e+02
              201  'Laura'                  5.000000000000000e+02
              201  NULL                     4.000000000000000e+02
              301  'Max'                    3.000000000000000e+02
              301  NULL                     3.000000000000000e+02
              501  'Chang'                  1.500000000000000e+02
              501  'Stephan'                3.000000000000000e+02
              501  'Sue'                    1.750000000000000e+02
              501  NULL                     1.900000000000000e+02
             NULL  NULL                     2.750000000000000e+02

.. _order-by-clause:

ORDER BY 절
===========

**ORDER BY** 절은 질의 결과를 오름차순 또는 내림차순으로 정렬하며, **ASC** 또는 **DESC** 와 같은 정렬 옵션을 명시하지 않으면 오름차순으로 정렬한다. **ORDER BY** 절을 지정하지 않으면, 조회되는 레코드의 순서는 질의에 따라 다르다. ::

    SELECT ...
    ORDER BY {col_name | expr | position} [ASC | DESC], ...] [NULLS {FIRST | LAST}]

*   *col_name* | *expr* | *position*: 정렬 기준이 되는 칼럼 이름, 표현식, 별칭 또는 칼럼 위치를 지정한다. 하나 이상의 값을 지정할 수 있으며 각 항목은 쉼표로 구분한다. **SELECT** 칼럼 리스트에 명시되지 않은 칼럼도 지정할 수 있다.

*   [**ASC** | **DESC**]: **ASC** 은 오름차순, **DESC** 은 내림차순으로 정렬하며, 정렬 옵션이 명시되지 않으면 오름차순으로 정렬한다.

*   [**NULLS** {**FIRST** | **LAST**}]: **NULLS FIRST**\ 는 NULL을 앞에 정렬하며, **NULLS LAST**\ 는 NULL을 뒤에 정렬한다. 이 구문이 생략될 경우 **ASC**\ 는 NULL을 앞에 정렬하며, **DESC**\ 는 NULL을 뒤에 정렬한다.

.. code-block:: sql

    -- selecting rows sorted by ORDER BY clause
    SELECT * 
    FROM sales_tbl
    ORDER BY dept_no DESC, name ASC;
    
::

          dept_no  name                  sales_month  sales_amount
    ==============================================================
              501  'Chang'                         1           150
              501  'Chang'                         2           150
              501  'Stephan'                       1           300
              501  'Stephan'                       2           100
              501  'Sue'                           1           150
              501  'Sue'                           2           200
              301  'Max'                           1           300
              301  'Max'                           2           300
              201  'George'                        1           450
              201  'George'                        2           250
              201  'Laura'                         1           100
              201  'Laura'                         2           500
     
.. code-block:: sql

    -- sorting reversely and limiting result rows by LIMIT clause
    SELECT dept_no AS a1, avg(sales_amount) AS a2 
    FROM sales_tbl
    GROUP BY a1
    ORDER BY a2 DESC
    LIMIT 3;
    
::

               a1           a2
    =======================================
              201     3.250000000000000e+02
              301     3.000000000000000e+02
              501     1.750000000000000e+02

다음은 ORDER BY 절 뒤에 NULLS FIRST, NULLS LAST 구문을 지정하는 예제이다.

.. code-block:: sql

    CREATE TABLE tbl (a INT, b VARCHAR);

    INSERT INTO tbl VALUES
    (1,NULL), (2,NULL), (3,'AB'), (4,NULL), (5,'AB'), 
    (6,NULL), (7,'ABCD'), (8,NULL), (9,'ABCD'), (10,NULL);

.. code-block:: sql
    
    SELECT * FROM tbl ORDER BY b NULLS FIRST;

::

                a  b
    ===================================
                1  NULL
                2  NULL
                4  NULL
                6  NULL
                8  NULL
               10  NULL
                3  'ab'
                5  'ab'
                7  'abcd'
                9  'abcd'
    
.. code-block:: sql

    SELECT * FROM tbl ORDER BY b NULLS LAST;

::

                a  b
    ===================================
                3  'ab'
                5  'ab'
                7  'abcd'
                9  'abcd'
                1  NULL
                2  NULL
                4  NULL
                6  NULL
                8  NULL
               10  NULL

.. note::

    **GROUP BY 별칭(alias)의 해석**

    .. code-block:: sql

        CREATE TABLE t1(a INT, b INT, c INT);
        INSERT INTO t1 VALUES(1,1,1);
        INSERT INTO t1 VALUES(2,NULL,2);
        INSERT INTO t1 VALUES(2,2,2);

        SELECT a, NVL(b,2) AS b 
        FROM t1 
        GROUP BY a, b;  -- Q1

    위의 SELECT 질의를 수행할 때 "GROUP BY a, b"는

    *   9.2 이하 버전에서 "GROUP BY a, NVL(b, 2)"(별칭 이름 b)로 해석되며, 아래 Q2와 동일한 결과를 출력한다.

        .. code-block:: sql
        
            SELECT a, NVL(b,2) AS bxxx 
            FROM t1 
            GROUP BY a, bxxx;  -- Q2

        ::

                    a            b
            ======================
                    1            1
                    2            2

    *   9.3 이상 버전에서 "GROUP BY a, b"(칼럼 이름 b)로 해석되며, 아래 Q3와 동일한 결과를 출력한다.

        .. code-block:: sql
        
            SELECT a, NVL(b,2) AS bxxx
            FROM t1 
            GROUP BY a, b;  -- Q3

        ::

                    a            b
            ======================
                    1            1
                    2            2
                    2            2

.. _limit-clause:

LIMIT 절
========

**LIMIT** 절은 출력되는 레코드의 개수를 제한할 때 사용한다. **LIMIT** 절은 prepared statement에 포함하여 사용할 수 있으며, 인자로 바인드 파라미터를 사용할 수 있다.

**LIMIT** 절을 포함하는 질의에서는 **WHERE** 절에 **INST_NUM** (), **ROWNUM** 을 포함할 수 없으며, **HAVING GROUPBY_NUM** ()과 함께 사용할 수 없다.

::

    LIMIT {[offset,] row_count | row_count [OFFSET offset]}

    <offset> ::= <limit_expression>
    <row_count> ::= <limit_expression>

    <limit_expression> ::= <limit_term> | <limit_expression> + <limit_term> | <limit_expression> - <limit_term>
    <limit_term> ::= <limit_factor> | <limit_term> * <limit_factor> | <limit_term> / <limit_factor>
    <limit_factor> ::= <unsigned int> | <input_hostvar> | ( <limit_expression> )

*   *offset*: 출력할 레코드의 시작 행의 오프셋을 지정한다. 결과 셋에 있는 시작 행의 오프셋은 0이다. 생략할 수 있으며 기본값은 **0** 이다. 부호 없는 정수, 호스트 변수 또는 간단한 표현식 중 하나일 수 있다.
*   *row_count*: 출력하고자 하는 레코드 개수를 명시한다.  부호 없는 정수, 호스트 변수 또는 간단한 표현식 중 하나일 수 있다.

.. code-block:: sql

    -- LIMIT clause can be used in prepared statement
    PREPARE stmt FROM 'SELECT * FROM sales_tbl LIMIT ?, ?';
    EXECUTE stmt USING 0, 10;

.. code-block:: sql

    -- selecting rows with LIMIT clause
    SELECT * 
    FROM sales_tbl
    WHERE sales_amount > 100
    LIMIT 5;
    
::

          dept_no  name                  sales_month  sales_amount
    ==============================================================
              201  'George'                        1           450
              201  'George'                        2           250
              201  'Laura'                         2           500
              301  'Max'                           1           300
              301  'Max'                           2           300
     
.. code-block:: sql

    -- LIMIT clause can be used in subquery
    SELECT t1.*
    FROM (SELECT * FROM sales_tbl AS t2 WHERE sales_amount > 100 LIMIT 5) AS t1
    LIMIT 1,3;
    
    -- above query and below query shows the same result
    SELECT t1.*
    FROM (SELECT * FROM sales_tbl AS t2 WHERE sales_amount > 100 LIMIT 5) AS t1
    LIMIT 3 OFFSET 1;
    
::

          dept_no  name                  sales_month  sales_amount
    ==============================================================
              201  'George'                        2           250
              201  'Laura'                         2           500
              301  'Max'                           1           300

.. code-block:: sql

    -- LIMIT clause allows simple expressions for both offset and row_count
    SELECT *
    FROM sales_tbl
    WHERE sales_amount > 100
    LIMIT ? * ?, (? * ?) + ?;


.. _join-query:
              
조인 질의
=========

조인은 두 개 이상의 테이블 또는 뷰(view)에 대해 행(row)을 결합하는 질의이다. 조인 질의에서 두 개 이상의 테이블에 공통인 칼럼을 비교하는 조건을 조인 조건이라고 하며, 조인된 각 테이블로부터 행을 가져와 지정된 조인 조건을 만족하는 경우에만 결과 행을 결합한다.

조인 질의에서 동등 연산자( **=** )를 이용한 조인 조건을 포함하는 조인 질의를 동등 조인(equi-join)이라 하고, 조인 조건이 없는 조인 질의를 카티션 곱(cartesian products)이라 한다. 또한, 하나의 테이블을 조인하는 경우를 자체 조인(self join)이라 하는데, 자체 조인에서는 **FROM** 절에 같은 테이블이 두 번 사용되므로 테이블 별칭(alias)을 사용하여 칼럼을 구분한다.

조인된 테이블에 대해 조인 조건을 만족하는 행만 결과를 출력하는 경우를 내부 조인(inner join) 또는 간단 조인(simple join)이라고 하는 반면, 조인된 테이블에 대해 조인 조건을 만족하는 행은 물론 조인 조건을 만족하지 못하는 행도 포함하여 출력하는 경우를 외부 조인(outer join)이라 한다. 

외부 조인은 왼쪽 테이블의 모든 행이 결과로 출력되는(조건과 일치하지 않는 오른쪽 테이블의 칼럼들은 NULL로 출력됨) 왼쪽 외부 조인과(left outer join)과 오른쪽 테이블의 모든 행이 결과로 출력되는(조건과 일치하지 않는 왼쪽 테이블의 칼럼들은 NULL로 출력됨) 오른쪽 외부 조인(right outer join)이 있으며, 양쪽의 행이 모두 출력되는 완전 외부 조인(full outer join)이 있다. 외부 조인 질의 결과에서 한쪽 테이블에 대해 대응되는 칼럼 값이 없는 경우, 이는 모두 **NULL**\ 을 반환된다.

::

    FROM <table_specification> [{, <table_specification> 
        | { <join_table_specification> | <join_table_specification2> } ...]

    <table_specification> ::=
        <single_table_spec> [<correlation>] |
        <metaclass_specification> [<correlation>] |
        <subquery> <correlation> |
        TABLE (<expression>) <correlation>
        
    <join_table_specification> ::=
        {
            [INNER | {LEFT | RIGHT} [OUTER]] JOIN 
           
        } <table_specification> ON <search_condition>
     
    <join_table_specification2> ::= 
        {
            CROSS JOIN | 
            NATURAL [LEFT | RIGHT] JOIN
        } <table_specification>

*   <*join_table_specification*>

    *   [**INNER**] **JOIN**: 내부 조인에 사용되며 조인 조건이 반드시 필요하다.

    *   {**LEFT** | **RIGHT**} [**OUTER**] **JOIN**: **LEFT** 는 왼쪽 외부 조인을 수행하는 질의를 만드는데 사용되고, **RIGHT** 는 오른쪽 외부 조인을 수행하는 질의를 만드는데 사용된다.


     
*   <*join_table_specification2*>

    *   **CROSS JOIN**: 교차 조인에 사용되며, 조인 조건을 사용하지 않는다.
    *   **NATURAL** [ **LEFT** | **RIGHT** ] **JOIN**: 자연 조인에 사용되며, 조인 조건을 사용하지 않는다. 같은 이름의 칼럼끼리 동등 조건을 가지는 것과 같이 동작한다.

내부 조인
---------

내부 조인은 조인을 위한 조건이 반드시 필요하다. **INNER JOIN** 키워드는 생략할 수 있으며, 생략하면 테이블 사이를 쉼표(,)로 구분하고, **ON** 조인 조건을 **WHERE** 조건으로 대체할 수 있다.


다음은 내부 조인을 이용하여 1950년 이후에 열린 올림픽 중에서 신기록이 세워진 올림픽의 개최연도와 개최국가를 조회하는 예제이다. 다음 질의는 *history* 테이블의 *host_year* 가 1950보다 큰 범위에서 값이 존재하는 레코드를 가져온다. 다음 두 개의 질의는 같은 결과를 출력한다.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h INNER JOIN olympic o ON h.host_year = o.host_year AND o.host_year > 1950;
     
    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h, olympic o
    WHERE h.host_year = o.host_year AND o.host_year > 1950;
     
::

        host_year  host_nation
    ===================================
             1968  'Mexico'
             1980  'U.S.S.R.'
             1984  'United States of America'
             1988  'Korea'
             1992  'Spain'
             1996  'United States of America'
             2000  'Australia'
             2004  'Greece'

외부 조인
---------

CUBRID는 외부 조인 중 왼쪽 외부 조인과 오른쪽 외부 조인만 지원하며, 완전 외부 조인(full outer join)을 지원하지 않는다. 또한, 외부 조인에서 조인 조건에 부질의와 하위 칼럼을 포함하는 경로 표현식을 사용할 수 없다.

외부 조인의 경우 조인 조건은 내부 조인의 경우와는 다른 방법으로 지정된다. 내부 조인의 조인 조건은 **WHERE** 절에서도 표현될 수 있지만, 외부 조인의 경우에는 조인 조건이 **FROM** 절 내의 **ON** 키워드 뒤에 나타난다. 다른 검색 조건은 **WHERE** 절이나 **ON** 절에서 사용할 수 있지만 검색 조건이 **WHERE** 절에 있을 때와 **ON** 절에 있을 때 질의 결과가 달라질 수 있다.

**FROM** 절에 명시된 순서대로 테이블 실행 순서가 고정되므로, 외부 조인을 사용하는 경우 테이블 순서에 주의하여 질의문을 작성한다. 외부 조인 연산자 '**(+)**'\ 를 **WHERE** 절에 명시하여 Oracle 스타일의 조인 질의문도 작성 가능하나, 실행 결과나 실행 계획이 원하지 않는 방향으로 유도될 수 있으므로 {**LEFT** | **RIGHT**} [**OUTER**] **JOIN**\ 을 이용한 표준 구문을 사용할 것을 권장한다.

다음은 오른쪽 외부 조인을 이용하여 1950년 이후에 열린 올림픽에서 신기록이 세워진 올림픽의 개최국가와 개최연도를 조회하되, 신기록이 세워지지 않은 올림픽에 대한 정보도 포함하는 예제이다. 이 예제는 오른쪽 외부 조인이므로, *olympic* 테이블의 *host_nation* 의 모든 레코드를 포함하고, 값이 존재하지 않는 *history* 테이블의 *host_year*\ 에 대해서는 칼럼 값으로 **NULL**\ 을 반환한다.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_year, o.host_nation
    FROM history h RIGHT OUTER JOIN olympic o ON h.host_year = o.host_year 
    WHERE o.host_year > 1950;
    
::

        host_year    host_year  host_nation
    ================================================
             NULL         1952  'Finland'
             NULL         1956  'Australia'
             NULL         1960  'Italy'
             NULL         1964  'Japan'
             NULL         1972  'Germany'
             NULL         1976  'Canada'
             1968         1968  'Mexico'
             1980         1980  'USSR'
             1984         1984  'USA'
             1988         1988  'Korea'
             1992         1992  'Spain'
             1996         1996  'USA'
             2000         2000  'Australia'
             2004         2004  'Greece'

다음은 왼쪽 외부 조인을 이용하여 위와 동일한 결과를 출력하는 예제이다. **FROM** 절에서 두 테이블의 순서를 바꾸어 명시한 후, 왼쪽 외부 조인을 수행한다.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_year, o.host_nation
    FROM olympic o LEFT OUTER JOIN history h ON h.host_year = o.host_year 
    WHERE o.host_year > 1950;
     
::

        host_year    host_year  host_nation
    ================================================
             NULL         1952  'Finland'
             NULL         1956  'Australia'
             NULL         1960  'Italy'
             NULL         1964  'Japan'
             NULL         1972  'Germany'
             NULL         1976  'Canada'
             1968         1968  'Mexico'
             1980         1980  'USSR'
             1984         1984  'USA'
             1988         1988  'Korea'
             1992         1992  'Spain'
             1996         1996  'USA'
             2000         2000  'Australia'
             2004         2004  'Greece'

다음은 **WHERE** 절에서 **(+)**\를 사용해서 외부 조인 질의를 작성한 예이며, 위와 같은 결과를 출력한다. 단, **(+)** 연산자를 이용한 Oracle 스타일의 외부 조인 질의문은 ISO/ANSI 표준이 아니며 모호한 상황을 만들어 낼 수 있으므로 가능하면 표준 구문인 **LEFT OUTER JOIN**\(또는 **RIGHT OUTER JOIN**)을 사용할 것을 권장한다.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_year, o.host_nation 
    FROM history h, olympic o
    WHERE o.host_year = h.host_year(+) AND o.host_year > 1950;
     
::

        host_year    host_year  host_nation
    ================================================
             NULL         1952  'Finland'
             NULL         1956  'Australia'
             NULL         1960  'Italy'
             NULL         1964  'Japan'
             NULL         1972  'Germany'
             NULL         1976  'Canada'
             1968         1968  'Mexico'
             1980         1980  'USSR'
             1984         1984  'USA'
             1988         1988  'Korea'
             1992         1992  'Spain'
             1996         1996  'USA'
             2000         2000  'Australia'
             2004         2004  'Greece'

이상의 예에서 *h.host_year* = *o.host_year* 는 외부 조인 조건이고 *o.host_year* > 1950은 검색 조건이다. 만약 검색 조건이 **WHERE** 절이 아닌 **ON** 절에서 조인 조건으로 사용될 경우 질의의 의미와 결과는 달라진다. 다음 질의는 *o.host_year* 가 1950보다 크지 않은 값도 질의 결과에 포함된다. 

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_year, o.host_nation
    FROM olympic o LEFT OUTER JOIN history h ON h.host_year = o.host_year AND o.host_year > 1950;
     
::

        host_year    host_year  host_nation
    ================================================
             NULL         1896  'Greece'
             NULL         1900  'France'
             NULL         1904  'USA'
             NULL         1908  'United Kingdom'
             NULL         1912  'Sweden'
             NULL         1920  'Belgium'
             NULL         1924  'France'
             NULL         1928  'Netherlands'
             NULL         1932  'USA'
             NULL         1936  'Germany'
             NULL         1948  'England'
             NULL         1952  'Finland'
             NULL         1956  'Australia'
             NULL         1960  'Italy'
             NULL         1964  'Japan'
             NULL         1972  'Germany'
             NULL         1976  'Canada'
             1968         1968  'Mexico'
             1980         1980  'USSR'
             1984         1984  'USA'
             1988         1988  'Korea'
             1992         1992  'Spain'
             1996         1996  'USA'
             2000         2000  'Australia'
             2004         2004  'Greece'

위의 예에서 **LEFT OUTER JOIN**\ 은 왼쪽 테이블의 행이 조건에 부합하지 않더라도 모든 행을 결과 행에 결합해야 하므로, 왼쪽 테이블의 칼럼 조건인 "AND o.host_year > 1950"는 이므로 무시된다. 그러나 "WHERE o.host_year > 1950"는 조인이 완료된 이후에 적용된다. **OUTER JOIN**\ 에서는 **ON** 절 뒤의 조건과 **WHERE** 절 뒤의 조건이 다르게 적용될 수 있음에 주의해야 한다.

교차 조인
---------

교차 조인은 아무런 조건 없이 두 개의 테이블을 결합한 것, 즉 카티션 곱(cartesian product)이다. 교차 조인에서 **CROSS JOIN** 키워드는 생략할 수 있으며, 생략하려면 테이블 사이를 쉼표(,)로 구분한다.

다음은 내부 조인을 이용하여 1950년 이후에 열린 올림픽 중에서 신기록이 세워진 올림픽의 개최연도와 개최국가를 조회하는 예제이다. 다음 질의는 *history* 테이블의 *host_year* 가 1950보다 큰 범위에서 값이 존재하는 레코드를 가져온다. 

다음은 교차 조인을 작성한 예이다. 다음 두 개의 질의는 같은 결과를 출력한다.

.. code-block:: sql

    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h CROSS JOIN olympic o;
     
    SELECT DISTINCT h.host_year, o.host_nation 
    FROM history h, olympic o;

::

        host_year  host_nation
    ===================================
             1968  'Australia'
             1968  'Belgium'
             1968  'Canada'
             1968  'England'
             1968  'Finland'
             1968  'France'
             1968  'Germany'
    ...
             2004  'Spain'
             2004  'Sweden'
             2004  'USA'
             2004  'USSR'
             2004  'United Kingdom'

    144 rows selected. (1.283548 sec) Committed.

자연 조인
---------

각 테이블에서 조인할 칼럼 이름이 같은 경우 즉, 해당 칼럼끼리 동등 조건(=)을 부여하고자 하는 경우 내부/외부 조인을 대체하는 자연 조인(natural join)을 사용할 수 있다.

.. code-block:: sql

    CREATE TABLE t1 (a int, b1 int); 
    CREATE TABLE t2 (a int, b2 int);

    INSERT INTO t1 values(1,1);
    INSERT INTO t1 values(3,3);
    INSERT INTO t2 values(1,1);
    INSERT INTO t2 values(2,2);

다음은 **NATURAL JOIN**\ 을 수행하는 예이다.

.. code-block:: sql
    
    SELECT /*+ RECOMPILE*/ * 
    FROM t1 NATURAL JOIN t2;

위의 질의를 수행하는 것은 아래의 질의를 수행하는 것과 동일하며, 같은 결과를 출력한다.

.. code-block:: sql

    SELECT /*+ RECOMPILE*/ * 
    FROM t1 INNER JOIN t2 ON t1.a=t2.a;

::


            a           b1            a           b2
    ================================================
            1            1            1            1

다음은 **NATURAL LEFT JOIN**\ 을 수행하는 예이다.
    
.. code-block:: sql

    SELECT /*+ RECOMPILE*/ * 
    FROM t1 NATURAL LEFT JOIN t2;
    
위의 질의를 수행하는 것은 아래의 질의를 수행하는 것과 동일하며, 같은 결과를 출력한다.

.. code-block:: sql

    SELECT /*+ RECOMPILE*/ * 
    FROM t1 LEFT JOIN t2 ON t1.a=t2.a;

::

                a           b1            a           b2
    ====================================================
                1            1            1            1
                3            3         NULL         NULL

다음은 **NATURAL RIGHT JOIN**\ 을 수행하는 예이다.

.. code-block:: sql

    SELECT /*+ RECOMPILE*/ * 
    FROM t1 NATURAL RIGHT JOIN t2;

위의 질의는 아래의 질의를 수행하는 것과 동일하며, 같은 결과를 출력한다.

.. code-block:: sql

    SELECT /*+ RECOMPILE*/ * 
    FROM t1 RIGHT JOIN t2 ON t1.a=t2.a;
    
::

                a           b1            a           b2
    ====================================================
                1            1            1            1
             NULL         NULL            2            2








부질의
======

부질의는 질의 내에서 **SELECT** 절이나 **WHERE** 절 등 표현식이 가능한 모든 곳에서 사용할 수 있다. 부질의가 표현식으로 사용될 경우에는 반드시 단일 칼럼을 반환해야 하지만, 표현식이 아닌 경우에는 하나 이상의 행이 반환될 수 있다. 부질의가 사용되는 경우에 따라 단일 행 부질의와 다중 행 부질의로 나뉠 수 있다.

단일 행 부질의
--------------

단일 행 부질의는 하나의 칼럼을 갖는 하나의 행을 만든다. 부질의에 의해 행이 반환되지 않을 경우에 부질의 표현식은 **NULL** 을 가진다. 만약 부질의가 두 개 이상의 행을 반환하도록 만들어진 경우에는 에러가 발생한다.

다음은 역대 기록 테이블을 조회하는데, 신기록을 수립한 올림픽이 개최된 국가도 함께 조회하는 예제이다. 이 예제는 표현식으로 사용된 단일 행 부질의를 보여준다. 이 예에서 부질의는 *olympic* 테이블에서 *host_year* 칼럼 값이 *history* 테이블의 *host_year* 칼럼 값과 같은 행에 대해 *host_nation* 값을 반환한다. 조건에 일치되는 값이 없을 경우 부질의 결과는 **NULL** 이 표시된다.

.. code-block:: sql

    SELECT h.host_year, (SELECT host_nation FROM olympic o WHERE o.host_year=h.host_year) AS host_nation,
           h.event_code, h.score, h.unit 
    FROM history h;
    
::

        host_year  host_nation            event_code  score                 unit
    ============================================================================================
        2004       'Greece'               20283       '07:53.0'             'time'
        2004       'Greece'               20283       '07:53.0'             'time'
        2004       'Greece'               20281       '03:57.0'             'time'
        2004       'Greece'               20281       '03:57.0'             'time'
        2004       'Greece'               20281       '03:57.0'             'time'
        2004       'Greece'               20281       '03:57.0'             'time'
        2004       'Greece'               20326       '210'                 'kg'
        2000       'Australia'            20328       '225'                 'kg'
        2004       'Greece'               20331       '237.5'               'kg'
    ...

다중 행 부질의
--------------

다중 행 부질의는 지정된 칼럼을 갖는 하나 이상의 행을 반환한다. 다중 행 부질의 결과는 적절한 키워드를 사용하여 **SET**, **MULTISET**, **LIST** (= **SEQUENCE**)를 만드는데 사용될 수 있다.

다음은 국가 테이블에서 국가 이름과 수도 이름을 조회하되, 올림픽을 개최한 국가는 개최도시를 **LIST** 로 묶어 함께 조회하는 예제이다. 이 예제 같은 경우는 부질의 결과를 이용하여 *olympic* 테이블의 *host_city* 칼럼 값으로 **LIST** 로 만든다. 이 질의는 *nation* 테이블에 대해 *name*, *capital* 값과 *host_nation* 값을 포함하는 *olympic* 테이블의 *host_city* 값에 대한 집합을 반환한다. 질의 결과에서 *name* 값이 공집합인 경우는 제외되고, *name* 과 같은 값을 갖는 *olympic* 테이블이 존재하지 않는 경우에는 공집합이 반환된다.

.. code-block:: sql

    SELECT name, capital, list(SELECT host_city FROM olympic WHERE host_nation = name) AS host_cities
    FROM nation;

::

      name                      capital                 host_cities
    ==================================================================
      'Somalia'                   'Mogadishu'           {}
      'Sri Lanka'                 'Sri Jayewardenepura Kotte' {}
      'Sao Tome & Principe'       'Sao Tome'            {}
      ...
      'U.S.S.R.'                  'Moscow'              {'Moscow'}
      'Uruguay'                   'Montevideo'          {}
      'United States of America'  'Washington.D.C'      {'Atlanta ', 'St. Louis', 'Los Angeles', 'Los Angeles'}
      'Uzbekistan'                'Tashkent'            {}
      'Vanuatu'                   'Port Vila'           {}

이런 형태의 다중 행 부질의 표현식은 컬렉션 타입의 값을 갖는 표현식이 허용되는 모든 곳에서 사용할 수 있다. 단, 클래스 속성 정의에서 **DEFAULT** 명세 부분과 같이 컬렉션 타입의 상수 값이 요구되는 곳에는 사용될 수 없다.

부질의 내에서 **ORDER BY** 절을 명시적으로 사용하지 않는 경우 다중 행 부질의 결과의 순서는 지정되지 않으므로, **LIST** (= **SEQUENCE**)를 생성하는 다중 행 부질의는 **ORDER BY** 절을 사용하여 결과의 순서를 지정해야 한다.

VALUES
======

**VALUES** 절은 표현식에 명시된 행 값들을 출력한다. 대부분 상수 테이블을 생성할 때 사용하지만, **VALUES** 절 자체만으로도 사용될 수 있다. **VALUES** 절에 한 개 이상의 행이 지정되면 모든 행은 같은 개수의 원소를 가져야 한다. ::

    VALUES (expression[, ...])[, ...]

*   *expression*: 괄호로 감싸인 표현식은 테이블에서의 하나의 행을 나타낸다.

**VALUES** 절은 상수 값으로 구성된 **UNION ALL** 질의문을 단순하게 표현하는 방법으로 볼 수 있다. 예를 들면 다음과 같은 질의문을 실행할 수 있다.

.. code-block:: sql

    VALUES (1 AS col1, 'first' AS col2), (2, 'second'), (3, 'third'), (4, 'fourth');

위 질의문은 다음과 같은 결과를 출력한다.

.. code-block:: sql

    SELECT 1 AS col1, 'first' AS col2
    UNION ALL
    SELECT 2, 'second'
    UNION ALL
    SELECT 3, 'third'
    UNION ALL
    SELECT 4, 'fourth';

다음은 **INSERT** 문 안에서 여러 행을 갖는 **VALUES** 절을 사용하는 예이다.

.. code-block:: sql

    INSERT INTO athlete (code, name, gender, nation_code, event)
    VALUES ('21111', 'Jang Mi-Ran ', 'F', 'KOR', 'Weight-lifting'),
           ('21112', 'Son Yeon-Jae ', 'F', 'KOR', 'Rhythmic gymnastics');

다음은 **FROM** 절에서 부질의(subquery)로 사용하는 예이다.

.. code-block:: sql
    
    SELECT a.*
    FROM athlete a, (VALUES ('Jang Mi-Ran', 'F'), ('Son Yeon-Jae', 'F')) AS t(name, gender)
    WHERE a.name=t.name AND a.gender=t.gender;

::

             code  name                gender   nation_code        event
    =====================================================================================================
            21111  'Jang Mi-Ran'       'F'      'KOR'              'Weight-lifting'
            21112  'Son Yeon-Jae'      'F'      'KOR'              'Rhythmic gymnastics'

FOR UPDATE
==========

**FOR UPDATE** 절은 **UPDATE/DELETE** 문을 수행하기 위해 **SELECT** 문에서 반환되는 행들에 잠금을 부여하기 위해 사용될 수 있다.

:: 

    SELECT ... [FOR UPDATE [OF <spec_name_comma_list>]]

        <spec_name_comma_list> ::= <spec_name> [, <spec_name>, ... ]
            <spec_name> ::= table_name | view_name 
         
* <*spec_name_comma_list*>: **FROM** 절에서 참조하는 테이블/뷰들의 목록

<*spec_name_comma_list*>에 참조된 테이블/뷰만 잠긴다. <*spec_name_comma_list*>가 누락되었지만 **FOR UPDATE** 가 있는 경우 **SELECT** 질의문의 **FROM** 절에 있는 모든 테이블/뷰가 참조된다고 가정한다. 행은 **X_LOCK** 을 사용하여 잠근다.

.. note:: 제약 사항 

    *   부질의 안에서 **FOR UPDATE** 절을 사용할 수 없다. 단, **FOR UPDATE** 절이 부질의를 참조할 수는 있다. 
    *   **GROUP BY**, **DISTINCT** 또는 집계 함수를 가진 질의문에서 사용할 수 없다. 
    *   **UNION** 을 참조할 수 없다. 

다음은 **SELECT ... FOR UPDATE** 문을 사용하는 예이다. 

.. code-block:: sql 


    CREATE TABLE t1(i INT); 
    INSERT INTO t1 VALUES (1), (2), (3), (4), (5); 

    CREATE TABLE t2(i INT); 
    INSERT INTO t2 VALUES (1), (2), (3), (4), (5); 
    CREATE INDEX idx_t2_i ON t2(i); 

    CREATE VIEW v12 AS SELECT t1.i AS i1, t2.i AS i2 FROM t1 INNER JOIN t2 ON t1.i=t2.i; 

    SELECT * FROM t1 ORDER BY 1 FOR UPDATE; 
    SELECT * FROM t1 ORDER BY 1 FOR UPDATE OF t1; 
    SELECT * FROM t1 INNER JOIN t2 ON t1.i=t2.i ORDER BY 1 FOR UPDATE OF t1, t2; 

    SELECT * FROM t1 INNER JOIN (SELECT * FROM t2 WHERE t2.i > 0) r ON t1.i=r.i WHERE t1.i > 0 ORDER BY 1 FOR UPDATE; 

    SELECT * FROM v12 ORDER BY 1 FOR UPDATE; 
    SELECT * FROM t1, (SELECT * FROM v12, t2 WHERE t2.i > 0 AND t2.i=v12.i1) r WHERE t1.i > 0 AND t1.i=r.i ORDER BY 1 FOR UPDATE OF r;
