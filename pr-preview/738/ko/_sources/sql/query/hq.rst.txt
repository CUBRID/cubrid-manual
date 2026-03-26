
:meta-keywords: start with clause, connect by clause, hierarchical pseudo column, hierarchical query example, hierarchical query function
:meta-description: Hierarchical Query is used to obtain a set of data organized in a hierarchy. The START WITH ... CONNECT BY clause is used in combination with the SELECT clause in the following fo rm.

***********
계층적 질의
***********

계층적 질의란 테이블에 포함된 행(row)간에 수직적 계층 관계가 성립되는 데이터에 대하여 계층 관계에 따라 각 행을 출력하는 질의이다. **START WITH ... CONNECT BY** 절은 **SELECT** 구문과 결합하여 사용된다.

**CONNECT BY ... START WITH** 로 두 절의 순서를 바꿔서 사용할 수도 있다. ::

    SELECT column_list
    FROM table_joins | tables
    [WHERE join_conditions and/or filtering_conditions]
    [hierarchical_clause]
     
    hierarchical_clause :
        [START WITH condition] CONNECT BY [NOCYCLE] condition
        | CONNECT BY [NOCYCLE] condition [START WITH condition]

START WITH 절
=============

**START WITH** 절은 계층 관계가 시작되는 루트 행(root row)을 지정하기 위한 것으로, **START WITH** 절 다음에 계층 관계를 검색하기 위한 조건식을 포함한다. 만약, **START WITH** 절에 다음에 위치하는 조건식이 생략되면 대상 테이블 내에 존재하는 모든 행을 루트 행으로 간주하여 계층 관계를 검색할 것이다.

.. note::

    **START WITH** 절이 생략되거나, **START WITH** 조건식을 만족하는 결과 행이 존재하지 않는 경우, 테이블 내의 모든 행을 루트 행으로 간주하여 각 루트 행에 속하는 하위 자식 행들 간 계층 관계를 검색하므로 결과 행들 중 일부는 중복되어 출력될 수 있다.

CONNECT BY 절
=============

*   **PRIOR** : **CONNECT BY** 조건식은 한 쌍의 행에 대한 상-하 계층 관계(부모-자식 관계)를 정의하기 위한 것으로, 조건식 내에서 하나는 부모(parent)로 지정되고, 다른 하나는 자식(child)으로 지정된다. 이처럼 행 간의 부모-자식 간 계층 관계를 정의하기 위하여 **CONNECT BY** 조건식 내에 **PRIOR** 연산자를 이용하여 부모 행의 칼럼 값을 지정한다. 즉, 부모 행의 칼럼 값과 같은 칼럼 값을 가지는 모든 행은 자식 행이 된다.

*   **NOCYCLE** : **CONNECT BY** 절의 조건식에 따른 계층 질의 결과는 루프를 포함할 수 있으며, 이것은 계층 트리를 생성할 때 무한 루프를 발생시키는 원인이 될 수 있다. 따라서, CUBRID는 루프를 발견하면 기본적으로 오류를 반환하고, 특수 연산자인 **NOCYCLE** 이 **CONNECT BY** 절에 명시된 경우에는 오류를 발생시키지 않고 해당 루프에 의해 검색된 결과를 출력한다. 

    만약, **CONNECT BY** 절에서 **NOCYCLE** 이 명시되지 않은 계층 질의문을 수행 중에 루프가 감지되는 경우, CUBRID는 오류를 반환하고 해당 질의문을 취소한다. 반면, **NOCYCLE** 이 명시된 계층 질의문에서 루프가 감지되는 경우, CUBRID는 오류를 반환하지는 않지만 루프가 감지된 행에 대해 **CONNECT_BY_ISCYCLE** 값을 1로 설정하고, 더 이상 계층 트리의 검색을 확장하지 않을 것이다.

다음은 계층 질의문을 수행하는 예제이다.

.. code-block:: sql

    -- Creating tree table and then inserting data
    CREATE TABLE tree(ID INT, MgrID INT, Name VARCHAR(32), BirthYear INT);
     
    INSERT INTO tree VALUES (1,NULL,'Kim', 1963);
    INSERT INTO tree VALUES (2,NULL,'Moy', 1958);
    INSERT INTO tree VALUES (3,1,'Jonas', 1976);
    INSERT INTO tree VALUES (4,1,'Smith', 1974);
    INSERT INTO tree VALUES (5,2,'Verma', 1973);
    INSERT INTO tree VALUES (6,2,'Foster', 1972);
    INSERT INTO tree VALUES (7,6,'Brown', 1981);
     
    -- Executing a hierarchical query with CONNECT BY clause
    SELECT id, mgrid, name
    FROM tree
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;

::
    
       id        mgrid  name
    ========================================
        1         NULL  'Kim'
        2         NULL  'Moy'
        3            1  'Jonas'
        3            1  'Jonas'
        4            1  'Smith'
        4            1  'Smith'
        5            2  'Verma'
        5            2  'Verma'
        6            2  'Foster'
        6            2  'Foster'
        7            6  'Brown'
        7            6  'Brown'
        7            6  'Brown'

.. code-block:: sql

    -- Executing a hierarchical query with START WITH clause
    SELECT id, mgrid, name
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY prior id=mgrid
    ORDER BY id;

::
    
       id        mgrid  name
    ========================================
        1         NULL  'Kim'
        2         NULL  'Moy'
        3            1  'Jonas'
        4            1  'Smith'
        5            2  'Verma'
        6            2  'Foster'
        7            6  'Brown'


계층 질의 실행
==============

조인 테이블에 대한 계층 질의
----------------------------

**SELECT** 문에서 대상 테이블이 조인된 경우, **WHERE** 절에는 검색 조건식 외에 테이블 조인 조건을 포함할 수 있다. 이때, CUBRID는 제일 먼저 **WHERE** 절의 조인 조건을 적용하여 테이블 조인 연산을 수행한 후, **CONNECT BY** 절의 조건식을 적용하고, 마지막으로 **WHERE** 절 내의 나머지 검색 조건식을 적용하여 연산을 처리한다.

**WHERE** 절 내에 조인 조건식과 검색 조건식을 함께 명시하는 경우, 내부적으로 조인 조건식이 검색 조건식으로 분류되어 의도하지 않게 연산 순서가 달라질 수 있으므로, **WHERE** 절보다는 **FROM** 절 내에 테이블 조인 조건을 명시하는 것을 권장한다.

계층 질의 결과
--------------

조인 테이블에 대한 계층 질의 결과는 **START WITH** 절의 조건식에 따라 루트 행으로부터 출력된다. 만약 **START WITH** 절이 생략되면 조인된 테이블의 모든 행들을 루트 행으로 간주하여 계층 관계를 출력한다. 이를 위해 CUBRID는 하나의 루트 행에 대하여 모든 자식 행을 검색한 후, 각 자식 행 하위에 속하는 모든 자식 행을 재귀적으로 검색한다. 이러한 검색은 더 이상의 자식 행이 발견되지 않을 때까지 반복된다.

또한, 계층 질의문은 **CONNECT BY** 절의 조건식을 먼저 적용하여 결과 행들을 검색한 후, **WHERE** 절에 명시된 검색 조건식을 적용하여 최종 결과 행들을 출력한다.

다음은 두 개의 조인된 테이블에 대하여 계층 질의문을 수행하는 예제이다.

.. code-block:: sql

    -- Creating tree2 table and then inserting data
    CREATE TABLE tree2(id int, treeid int, job varchar(32));
     
    INSERT INTO tree2 VALUES(1,1,'Partner');
    INSERT INTO tree2 VALUES(2,2,'Partner');
    INSERT INTO tree2 VALUES(3,3,'Developer');
    INSERT INTO tree2 VALUES(4,4,'Developer');
    INSERT INTO tree2 VALUES(5,5,'Sales Exec.');
    INSERT INTO tree2 VALUES(6,6,'Sales Exec.');
    INSERT INTO tree2 VALUES(7,7,'Assistant');
    INSERT INTO tree2 VALUES(8,null,'Secretary');
     
    -- Executing a hierarchical query onto table joins
    SELECT t.id,t.name,t2.job,level
    FROM tree t INNER JOIN tree2 t2 ON t.id=t2.treeid
    START WITH t.mgrid is null
    CONNECT BY prior t.id=t.mgrid
    ORDER BY t.id;

::
    
       id  name                  job                         level
    ==============================================================
        1  'Kim'                 'Partner'                       1
        2  'Moy'                 'Partner'                       1
        3  'Jonas'               'Developer'                     2
        4  'Smith'               'Developer'                     2
        5  'Verma'               'Sales Exec.'                   2
        6  'Foster'              'Sales Exec.'                   2
        7  'Brown'               'Assistant'                     3

계층 질의문에서의 데이터 정렬
-----------------------------

**ORDER SIBLINGS BY** 절은 계층 질의 결과 값들의 계층 정보를 유지하면서 특정 칼럼을 기준으로 오름차순 또는 내림차순으로 데이터를 정렬하며, 동일한 부모를 가진 자식 행들을 정렬할 수 있다. 계층적 질의문에서 데이터의 계층적 순서를 파악하기 위해 사용한다. ::

    ORDER SIBLINGS BY col_1 [ASC|DESC] [, col_2 [ASC|DESC] [...[, col_n [ASC|DESC]]...]]

다음은 상사와 그의 부하 직원을 출력하되, 출생 연도가 앞서는 사람부터 출력하는 예제이다.

계층 질의 결과는 기본적으로 **ORDER SIBLINGS BY** 절에 명시된 칼럼 리스트에 따라 정렬된 부모와 그 부모의 자식 노드들이 연속으로 출력된다. 부모가 같은 형제 노드는 명시된 정렬 순서에 따라 정렬되어 출력된다.

.. code-block:: sql

    -- Outputting a parent node and its child nodes, which sibling nodes that share the same parent are sorted in the order of birthyear.
    SELECT id, mgrid, name, birthyear, level
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER SIBLINGS BY birthyear;

::
    
       id        mgrid  name                    birthyear        level
    ==================================================================
        2         NULL  'Moy'                        1958            1
        6            2  'Foster'                     1972            2
        7            6  'Brown'                      1981            3
        5            2  'Verma'                      1973            2
        1         NULL  'Kim'                        1963            1
        4            1  'Smith'                      1974            2
        3            1  'Jonas'                      1976            2

다음은 상사와 그의 부하 직원을 출력하되, 같은 레벨 간에는 우선 입사한 순서로 정렬시키는 예제이다. *id* 는 입사한 순서로 부여된다. *id* 는 직원의 입사번호이며, *mgrid* 는 상사의 입사번호이다.

.. code-block:: sql

    -- Outputting a parent node and its child nodes, which sibling nodes that share the same parent are sorted in the order of id.
    SELECT id, mgrid, name, LEVEL
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER SIBLINGS BY id;

::
    
       id        mgrid  name                        level
    =====================================================
        1         NULL  'Kim'                           1
        3            1  'Jonas'                         2
        4            1  'Smith'                         2
        2         NULL  'Moy'                           1
        5            2  'Verma'                         2
        6            2  'Foster'                        2
        7            6  'Brown'                         3


계층 질의 의사 칼럼
===================

LEVEL
-----

**LEVEL** 은 계층 질의 결과 행의 깊이 레벨(depth)을 나타내는 의사 칼럼(pseudocolumn)이다. 루트 노드의 **LEVEL** 은 1이며, 하위 자식 노드의 **LEVEL** 은 2가 된다.

**LEVEL** 의사 칼럼은 **SELECT** 문 내의 **WHERE** 절, **ORDER BY** 절, **GROUP BY ... HAVING** 절, **CONNECT BY** 절에서 사용 가능하며, 집계 함수를 이용하는 구문에서도 사용 가능하다.

다음은 노드의 레벨을 확인하기 위하여 **LEVEL** 값을 조회하는 예제이다.

.. code-block:: sql

    -- Checking the LEVEL value
    SELECT id, mgrid, name, LEVEL
    FROM tree
    WHERE LEVEL=2
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;

::

       id        mgrid  name                        level
    =====================================================
        3            1  'Jonas'                         2
        4            1  'Smith'                         2
        5            2  'Verma'                         2
        6            2  'Foster'                        2

다음은 **CONNECT BY** 절 뒤에 **LEVEL** 조건을 추가한 예제이다.

.. code-block:: sql

    SELECT LEVEL FROM db_root CONNECT BY LEVEL <= 10;

::

            level
    =============
                1
                2
                3
                4
                5
                6
                7
                8
                9
               10

단, "CONNECT BY expr(LEVEL) < expr"과 같은 형태, 예를 들어 "CONNECT BY LEVEL +1 < 5"와 같은 형태는 지원하지 않는다.

CONNECT_BY_ISLEAF
-----------------

**CONNECT_BY_ISLEAF** 는 계층 질의 결과 행이 리프 노드(leaf node : 하위에 자식 노드를 가지지 않는 단말 노드)인지 가리키는 의사 칼럼이다. 계층 구조 하에서 현재 행이 리프 노드이면 1을 반환하고, 그렇지 않으면 0을 반환한다.

다음은 리프 노드를 확인하기 위하여 **CONNECT_BY_ISLEAF** 값을 조회하는 예제이다.

.. code-block:: sql

    -- CONNECT_BY_ISLEAF의 값을 확인하기
    SELECT id, mgrid, name, CONNECT_BY_ISLEAF
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;
     
::

      id        mgrid  name                  connect_by_isleaf
    =============================================================
       1         NULL  'Kim'                                 0
       2         NULL  'Moy'                                 0
       3            1  'Jonas'                               1
       4            1  'Smith'                               1
       5            2  'Verma'                               1
       6            2  'Foster'                              0
       7            6  'Brown'                               1

CONNECT_BY_ISCYCLE
------------------

**CONNECT_BY_ISCYCLE** 은 계층 질의 결과 행이 루프를 발생시키는 행인지를 가리키는 의사 칼럼이다. 즉, 현재 행의 자식이 동시에 조상이 되어 루프를 발생시키는 경우 1을 반환하고, 그렇지 않으면 0을 반환한다.

**CONNECT_BY_ISCYCLE** 의사 칼럼은 **SELECT** 문 내의 **WHERE** 절, **ORDER BY** 절, **GROUP BY ... HAVING** 절에서 사용할 수 있으며, 집계 함수를 이용하는 구문에서도 사용 가능하다.

.. note:: **CONNECT_BY_ISCYCLE** 은 **CONNECT BY** 절에 **NOCYCLE** 키워드가 명시되는 경우에만 사용할 수 있다.

다음은 루프를 발생시키는 행을 확인하기 위해 **CONNECT_BY_ISCYCE** 값을 조회하는 예제이다.

.. code-block:: sql

    -- tree_cycle 테이블을 만들고 데이터를 삽입하기
    CREATE TABLE tree_cycle(ID INT, MgrID INT, Name VARCHAR(32));
     
    INSERT INTO tree_cycle VALUES (1,NULL,'Kim');
    INSERT INTO tree_cycle VALUES (2,11,'Moy');
    INSERT INTO tree_cycle VALUES (3,1,'Jonas');
    INSERT INTO tree_cycle VALUES (4,1,'Smith');
    INSERT INTO tree_cycle VALUES (5,3,'Verma');
    INSERT INTO tree_cycle VALUES (6,3,'Foster');
    INSERT INTO tree_cycle VALUES (7,4,'Brown');
    INSERT INTO tree_cycle VALUES (8,4,'Lin');
    INSERT INTO tree_cycle VALUES (9,2,'Edwin');
    INSERT INTO tree_cycle VALUES (10,9,'Audrey');
    INSERT INTO tree_cycle VALUES (11,10,'Stone');
     
    -- CONNECT_BY_ISCYCLE의 값을 확인하기
    SELECT id, mgrid, name, CONNECT_BY_ISCYCLE
    FROM tree_cycle
    START WITH name in ('Kim', 'Moy')
    CONNECT BY NOCYCLE PRIOR id=mgrid
    ORDER BY id;
     
::

    id        mgrid  name        connect_by_iscycle
    ==================================================
     1         NULL  'Kim'                        0
     2           11  'Moy'                        0
     3            1  'Jonas'                      0
     4            1  'Smith'                      0
     5            3  'Verma'                      0
     6            3  'Foster'                     0
     7            4  'Brown'                      0
     8            4  'Lin'                        0
     9            2  'Edwin'                      0
    10            9  'Audrey'                     0
    11           10  'Stone'                      1

계층 질의 연산자
================

CONNECT_BY_ROOT
---------------

**CONNECT_BY_ROOT** 은 칼럼 값으로 루트 행의 값을 반환한다. 이 연산자는 **SELECT** 문 내의 **WHERE** 절 및 **ORDER BY** 절에서 사용할 수 있다.

다음은 계층 질의 결과 행에 대하여 루트 행의 *id* 값을 조회하는 예제이다.

.. code-block:: sql

    -- 각 행마다 루트 행의 id 값을 확인하기
    SELECT id, mgrid, name, CONNECT_BY_ROOT id
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;
     
::

       id        mgrid  name                   connect_by_root id
    =============================================================
        1         NULL  'Kim'                                   1
        2         NULL  'Moy'                                   2
        3            1  'Jonas'                                 1
        4            1  'Smith'                                 1
        5            2  'Verma'                                 2
        6            2  'Foster'                                2
        7            6  'Brown'                                 2

.. _prior-operator:

PRIOR
-----

**PRIOR** 연산자는 칼럼 값으로 부모 행의 값을 반환하고, 루트 행에 대해서는 **NULL** 을 반환한다. 이 연산자는 **SELECT** 문 내의 **WHERE** 절, **ORDER BY** 절 및 **CONNECT BY** 절에서 사용할 수 있다.

다음은 계층 질의 결과 행에 대하여 부모 행의 id 값을 조회하는 예제이다.

.. code-block:: sql

    -- 각 행마다 부모 행의 id 값을 확인하기
    SELECT id, mgrid, name, PRIOR id as "prior_id"
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;
     
::

       id        mgrid  name                     prior_id
    =====================================================
        1         NULL  'Kim'                        NULL
        2         NULL  'Moy'                        NULL
        3            1  'Jonas'                         1
        4            1  'Smith'                         1
        5            2  'Verma'                         2
        6            2  'Foster'                        2
        7            6  'Brown'                         6

계층 질의 함수
==============

SYS_CONNECT_BY_PATH
-------------------

**SYS_CONNECT_BY_PATH** 함수는 루트 행으로부터 해당 행까지의 상-하 관계의 path를 문자열로 반환하는 함수이다. 이때, 함수의 인자로 지정되는 칼럼과 구분자는 문자형 타입이어야 하며, 각 path는 지정된 구분자에 의해 구분되어 연쇄적으로 출력된다. 이 함수는 **SELECT** 문 내의 **WHERE** 절과 **ORDER BY** 절에서 사용할 수 있다. ::

    SYS_CONNECT_BY_PATH (column_name, separator_char)

다음은 루트 행으로부터 해당 행의 path를 확인하는 예제이다.

.. code-block:: sql

    -- 구분자를 이용하여 루트 행으로부터 해당 행까지 path를 확인하기
    SELECT id, mgrid, name, SYS_CONNECT_BY_PATH(name,'/') as [hierarchy]
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;
     
::

       id        mgrid  name                  hierarchy
    ==============================================================
        1         NULL  'Kim'                 '/Kim'
        2         NULL  'Moy'                 '/Moy'
        3            1  'Jonas'               '/Kim/Jonas'
        4            1  'Smith'               '/Kim/Smith'
        5            2  'Verma'               '/Moy/Verma'
        6            2  'Foster'              '/Moy/Foster'
        7            6  'Brown'               '/Moy/Foster/Brown'

계층 질의문 예
==============

**SELECT** 문에 **CONNECT BY** 절을 명시하여 계층 질의문을 작성하는 예이다.

재귀적 참조 관계를 가지는 테이블을 생성했으며, 이 테이블은 *ID* 와 *ParentID* 라는 두 개의 칼럼으로 구성되고, *ID* 와 *ParentID* 는 각각 기본 키와 외래 키로 정의된다고 가정한다. 이때, 루트 노드의 *ParentID* 값은 **NULL** 이 된다.

테이블이 생성되었다면, 아래와 같이 **UNION ALL** 을 이용하여 계층 구조를 가지는 전체 데이터와 **LEVEL** 값을 조회할 수 있다.

.. code-block:: sql

    CREATE TABLE tree_table (ID int PRIMARY KEY, ParentID int, name VARCHAR(128));
    
    INSERT INTO tree_table VALUES (1,NULL,'Kim');
    INSERT INTO tree_table VALUES (2,1,'Moy');
    INSERT INTO tree_table VALUES (3,1,'Jonas');
    INSERT INTO tree_table VALUES (4,1,'Smith');
    INSERT INTO tree_table VALUES (5,3,'Verma');
    INSERT INTO tree_table VALUES (6,3,'Foster');
    INSERT INTO tree_table VALUES (7,4,'Brown');
    INSERT INTO tree_table VALUES (8,4,'Lin');
    INSERT INTO tree_table VALUES (9,2,'Edwin');
    INSERT INTO tree_table VALUES (10,9,'Audrey');
    INSERT INTO tree_table VALUES (11,10,'Stone');
    
    SELECT L1.ID, L1.ParentID, L1.name, 1 AS [Level]
        FROM tree_table AS L1
        WHERE L1.ParentID IS NULL
    UNION ALL
    SELECT L2.ID, L2.ParentID, L2.name, 2 AS [Level]
        FROM tree_table AS L1
            INNER JOIN tree_table AS L2 ON L1.ID=L2.ParentID
        WHERE L1.ParentID IS NULL
    UNION ALL
    SELECT L3.ID, L3.ParentID, L3.name, 3 AS [Level]
        FROM tree_table AS L1
            INNER JOIN tree_table AS L2 ON L1.ID=L2.ParentID
            INNER JOIN tree_table AS L3 ON L2.ID=L3.ParentID
        WHERE L1.ParentID IS NULL
    UNION ALL
    SELECT L4.ID, L4.ParentID, L4.name, 4 AS [Level]
        FROM tree_table AS L1
            INNER JOIN tree_table AS L2 ON L1.ID=L2.ParentID
            INNER JOIN tree_table AS L3 ON L2.ID=L3.ParentID
            INNER JOIN tree_table AS L4 ON L3.ID=L4.ParentID
        WHERE L1.ParentID IS NULL;

::

       ID     ParentID  name                        Level
    =====================================================
        1         NULL  'Kim'                           1
        2            1  'Moy'                           2
        3            1  'Jonas'                         2
        4            1  'Smith'                         2
        9            2  'Edwin'                         3
        5            3  'Verma'                         3
        6            3  'Foster'                        3
        7            4  'Brown'                         3
        8            4  'Lin'                           3
       10            9  'Audrey'                        4

계층 관계를 가지는 데이터의 레벨이 얼마나 될지 예측할 수 없으므로, 위 질의문은 새로운 행이 검색되지 않을 때까지 루프를 도는 저장 프로시저(stored procedure) 문으로 재작성할 수 있다. 

그러나 루프를 도는 동안 각 단계마다 계층 트리를 확인해야 하므로, 아래와 같이 **SELECT** 문에 **CONNECT BY** 절을 명시하여 계층 질의문을 재작성할 수 있다. 다음의 질의문을 실행하면, 계층 관계를 가지는 데이터 전체와 각 행의 레벨이 출력된다.

.. code-block:: sql

    SELECT ID, ParentID, name, Level
    FROM tree_table
    START WITH ParentID IS NULL
    CONNECT BY ParentID=PRIOR ID;

::

       ID     ParentID  name                        level
    =====================================================
        1         NULL  'Kim'                           1
        2            1  'Moy'                           2
        9            2  'Edwin'                         3
       10            9  'Audrey'                        4
       11           10  'Stone'                         5
        3            1  'Jonas'                         2
        5            3  'Verma'                         3
        6            3  'Foster'                        3
        4            1  'Smith'                         2
        7            4  'Brown'                         3
        8            4  'Lin'                           3

루프로 인한 오류를 발생시키지 않으려면 다음과 같이 **NOCYCLE**\을 명시할 수 있다. 아래의 질의 수행 시 루프가 발생하지 않으므로, 결과는 위와 동일하다.

.. code-block:: sql

    SELECT ID, ParentID, name, Level
    FROM tree_table
    START WITH ParentID IS NULL
    CONNECT BY NOCYCLE ParentID=PRIOR ID;

계층 질의에 대한 루프 탐색 과정 중에 동일한 행이 발견되면, CUBRID는 그 질의를 루프가 있는 것으로 판단한다. 다음은 루프가 존재하는 예로, **NOCYCLE**\을 명시하여 루프가 존재하는 경우 추가 탐색을 종료하도록 했다.

.. code-block:: sql

    CREATE TABLE tbl(seq INT, id VARCHAR(10), parent VARCHAR(10));
    
    INSERT INTO tbl VALUES (1, 'a', null);
    INSERT INTO tbl VALUES (2, 'b', 'a');
    INSERT INTO tbl VALUES (3, 'b', 'c');
    INSERT INTO tbl VALUES (4, 'c', 'b');
    INSERT INTO tbl VALUES (5, 'c', 'b');
    
    SELECT seq, id, parent, LEVEL,
      CONNECT_BY_ISCYCLE AS iscycle,
      CAST(SYS_CONNECT_BY_PATH(id,'/') AS VARCHAR(10)) AS idpath
    FROM tbl
    START WITH PARENT is NULL
    CONNECT BY NOCYCLE PARENT = PRIOR id;

::

        seq  id           parent       level      iscycle  idpath
    =============================================================================
          1  'a'          NULL             1            0  '/a'
          2  'b'          'a'              2            0  '/a/b'
          4  'c'          'b'              3            0  '/a/b/c'
          3  'b'          'c'              4            1  '/a/b/c/b'
          5  'c'          'b'              5            1  '/a/b/c/b/c'
          5  'c'          'b'              3            0  '/a/b/c'
          3  'b'          'c'              4            1  '/a/b/c/b'
          4  'c'          'b'              5            1  '/a/b/c/b/c'

다음은 계층 질의를 사용하여 2013년 3월(201303)의 날짜들을 출력하는 예제이다.

.. code-block:: sql

    SELECT TO_CHAR(base_month + lvl -1, 'YYYYMMDD') h_date
    FROM (
        SELECT LEVEL lvl, base_month
        FROM ( 
                SELECT TO_DATE('201303', 'YYYYMM') base_month FROM db_root
        )
        CONNECT BY LEVEL <= LAST_DAY(base_month) - base_month + 1
    );

::

    h_date
    ======================
      '20130301'
      '20130302'
      '20130303'
      '20130304'
      '20130305'
      '20130306'
      '20130307'
      '20130308'
      '20130309'
      '20130310'
      '20130311'
      '20130312'
      '20130313'
      '20130314'
      '20130315'
      '20130316'
      '20130317'
      '20130318'
      '20130319'
      '20130320'
      '20130321'
      '20130322'
      '20130323'
      '20130324'
      '20130325'
      '20130326'
      '20130327'
      '20130328'
      '20130329'
      '20130330'
      '20130331'

    31 rows selected. (0.066175 sec) Committed.

계층 질의문의 성능
==================

**CONNECY BY** 절을 이용한 계층 질의문이 짧고 간편하지만 질의 처리 속도 측면에서는 한계를 가지고 있으므로 주의해야 한다.

질의문 수행 결과가 대상 테이블의 모든 행을 출력하는 경우라면, **CONNECT BY** 절을 이용한 계층 질의문은 루프 감지, 의사 칼럼의 예약 등 내부적인 처리로 인해 오히려 일반적인 질의문보다 성능이 낮을 수 있다. 반대로 대상 테이블에 대해 일부 행만 출력하는 경우라면 **CONNECT BY** 절을 이용한 계층 질의문의 성능이 높다.

예를 들어, 2만 개의 레코드를 가지는 테이블에 대하여 약 1000개의 레코드를 포함하는 서브 트리를 검색하는 경우라면, **CONNECT BY** 절을 포함한 **SELECT** 문은 **UNION ALL** 을 결합한 **SELECT** 문보다 약 30%의 성능 향상을 기대할 수 있다.
