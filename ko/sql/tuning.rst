***********
질의 최적화
***********

통계 정보 갱신
==============

**UPDATE STATISTICS ON** 문은 질의 처리기에서 사용되는 내부 통계 정보를 생성한다. 이러한 통계 정보는 데이터베이스 시스템이 질의를 처리하는데 효과적인 방법을 사용할 수 있게 한다. ::

    UPDATE STATISTICS ON { table_spec [ {, table_spec } ] | ALL CLASSES | CATALOG CLASSES } [ ; ]
    
    table_spec ::=
    single_table_spec
    | ( single_table_spec [ {, single_table_spec } ] )
    
    single_table_spec ::=
    [ ONLY ] table_name
    | ALL table_name [ ( EXCEPT table_name ) ]

*   **ALL CLASSES** : 키워드 **ALL CLASSES** 를 지정하였을 경우 데이터베이스 안에 존재하는 모든 테이블에 대한 통계 정보가 갱신된다.

통계 정보 확인
==============

CSQL 인터프리터의 세션 명령어로 지정한 테이블의 통계 정보를 확인한다. ::

    csql> ;info stats table_name

*   *table_name* : 통계 정보를 확인할 테이블 이름

다음은 CSQL 인터프리터에서 *t1* 테이블의 통계 정보를 출력하는 예제이다.

.. code-block:: sql

    CREATE TABLE t1 (code INT);
    INSERT INTO t1 VALUES(1),(2),(3),(4),(5);
    CREATE INDEX i_t1_code ON t1(code);
    UPDATE STATISTICS ON t1;

::

    ;info stats t1
    CLASS STATISTICS
    ****************
     Class name: t1 Timestamp: Mon Mar 14 16:26:40 2011
     Total pages in class heap: 1
     Total objects: 5
     Number of attributes: 1
     Atrribute: code
        id: 0
        Type: DB_TYPE_INTEGER
        Minimum value: 1
        Maximum value: 5
        B+tree statistics:
            BTID: { 0 , 1049 }
            Cardinality: 5 (5) , Total pages: 2 , Leaf pages: 1 , Height: 2

질의 실행 계획 보기
===================

CUBRID SQL 질의에 대한 실행 계획(query plan)을 보기 위해서는 **SET OPTIMIZATION** 구문을 이용해서 최적화 수준(optimization level) 값을 변경시킨다. 현재의 최적화 수준 값은 **GET OPTIMIZATION** 구문으로 얻을 수 있다.

CUBRID 질의 최적화기는 사용자에 의해 설정된 최적화 수준 값을 참조하여 최적화 여부와 질의 실행 계획의 출력 여부를 결정한다. 질의 실행 계획은 표준 출력으로 표시되므로 CSQL 인터프리터와 같은 터미널 기반의 프로그램에서 사용하는 것을 가정하고 설명한다. CSQL 질의 편집기에서는 **;plan** 명령어를 통해 질의 실행 계획을 볼 수 있다. 자세한 내용은 :ref:`csql-session-commands` 를 참고한다. CUBRID 매니저를 이용해서 질의 실행 계획을 보는 방법에 대해서는 `cubrid 매니저 매뉴얼 <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual_kr>`_ 을 참고한다. ::

    SET OPTIMIZATION LEVEL opt-level [;]
    GET OPTIMIZATION LEVEL [ { TO | INTO } variable ] [;]

*   *opt-level* : 최적화 수준을 지정하는 값으로 다음과 같은 의미를 갖는다.

    *   0 : 질의 최적화를 수행하지 않는다. 실행하는 질의는 가장 단순한 형태의 실행 계획을 가지고 실행된다. 디버깅의 용도 이외에는 사용되지 않는다.
    
    *   1 : 질의 최적화를 수행한다. CUBRID에서 사용되는 기본 설정 값으로 대부분의 경우 변경할 필요가 없다.
    
    *   2: 질의 최적화를 수행하여 실행 계획을 생성하나 질의 자체는 수행되지 않는다. 일반적으로 사용되지 않고 다음 질의 실행 계획 보기를 위한 설정값과 같이 설정되어 사용된다.
    
    *   257 : 질의 최적화를 수행하여 생성된 질의 실행 계획(플랜)을 출력한다. 256+1의 값으로 해석하여 값을 1로 설정하고 질의 실행 계획 출력을 지정한 것과 같다.
    
    *   258 : 질의 최적화를 수행하여 생성된 질의 실행 계획을 출력하나 질의를 수행하지는 않는다. 256+2의 값으로 해석하여 2로 설정하고 질의 실행 계획 출력을 지정한 것과 같다. 질의 실행 계획을 살펴보고자 하나 실행 결과에는 관심이 없을 경우 유용한 설정이다.
    
    *   513 : 질의 최적화를 수행하고 상세 질의 실행 계획을 출력한다. 512+1의 의미이다.
    
    *   514 : 질의 최적화를 수행하고 상세 질의 실행 계획을 출력하나 질의는 실행하지는 않는다. 512+2의 의미이다.

    .. note:: 2, 258, 514와 같이 질의를 실행하지 않게 최적화 수준을 설정하는 경우 SELECT 문 뿐만 아니라 INSERT, UPDATE, DELETE, REPLACE,  TRIGGER, SERIAL  문 등 모든 질의문이 실행되지 않는다.
    
다음은 심권호 선수가 메달을 획득한 연도와 메달 종류를 구하는 예제를 이용해 질의 실행 계획 보기를 수행한 것이다.

.. code-block:: sql

    GET OPTIMIZATION LEVEL;
    
          Result
    =============
                1

    SET OPTIMIZATION LEVEL 258;

    SELECT a.name, b.host_year, b.medal
    FROM athlete a, game b 
    WHERE a.name = 'Sim Kwon Ho' AND a.code = b.athlete_code;
    
    Query plan:
      Nested loops
            Sequential scan(game b)
            Index scan(athlete a, pk_athlete_code, a.code=b.athlete_code)
    There are no results.
    0 rows selected.

.. _sql-hint:

SQL 힌트
========

사용자는 질의문에 힌트를 주어 해당 질의 성능을 높일 수 있다. 질의 최적화기는 질의문에 대한 최적화 작업을 수행할 때 SQL 힌트를 참고하여 효율적인 실행 계획을 생성한다. CUBRID에서 지원하는 SQL 힌트는 테이블 조인 관련 힌트, 인덱스 관련 힌트, 통계 정보 관련 힌트가 있다. ::

    { CREATE | ALTER } /*+ NO_STATS */ { TABLE | CLASS } ...;
        
    { CREATE | ALTER | DROP } /*+ NO_STATS */ INDEX ...;
     
    { SELECT | UPDATE | DELETE } /*+ <hint> [ { <hint> } ... ] */ ...;

    MERGE /*+ <merge_statement_hint> [ { <merge_statement_hint> } ... ] */ INTO ...;
    
    <hint> ::=
    USE_NL [ (spec_name_comma_list) ] |
    USE_IDX [ (spec_name_comma_list) ] |
    USE_MERGE [ (spec_name_comma_list) ] |
    ORDERED |
    USE_DESC_IDX |
    NO_DESC_IDX |
    NO_COVERING_IDX |
    NO_MULTI_RANGE_OPT |
    RECOMPILE
    
    <merge_statement_hint> ::=
    USE_UPDATE_INDEX (<update_index_list>) |
    USE_DELETE_INDEX (<insert_index_list>) |
    RECOMPILE

SQL 힌트는 주석에 더하기 기호(+)를 함께 사용하여 지정한다. 힌트를 사용하는 방법은 :doc:`comment` 절에 소개된 바와 같이 3 가지 방식이 있다. 따라서 SQL 힌트도 다음과 같이 3 가지 방식으로 사용할 수 있다.

* /\*+ hint \*/
* --+ hint
* //+ hint

힌트 주석은 반드시 키워드 **SELECT**, **CREATE**, **ALTER** 등의 예약어 다음에 나타나야 하고, 더하기 기호(+)가 주석에서 첫 번째 문자로 시작되어야 한다. 여러 개의 힌트를 지정할 때는 공백이 구분자로 사용된다.

CREATE/ALTER TABLE 문과 CREATE/ALTER/DROP INDEX 문에는 다음 힌트가 지정될 수 있다.

*   **NO_STATS** : 통계 정보 관련 힌트로서 해당 DDL 수행 후에 통계 정보를 갱신하지 않는다. 해당 DDL 문의 성능은 향상되나 통계 정보를 갱신하지 않으므로 질의 계획이 최적화되지 않음에 유의한다.

SELECT, UPDATE, DELETE 문에는 다음 힌트가 지정될 수 있다.

*   **USE_NL** : 테이블 조인과 관련한 힌트로서, 질의 최적화기 중첩 루프 조인 실행 계획을 만든다.
*   **USE_MERGE** : 테이블 조인과 관련한 힌트로서, 질의 최적화기는 정렬 병합 조인 실행 계획을 만든다.
*   **ORDERED** : 테이블 조인과 관련한 힌트로서, 질의 최적화기는 **FROM** 절에 명시된 테이블의 순서대로 조인하는 실행 계획을 만든다. **FROM** 절에서 왼쪽 테이블은 조인의 외부 테이블이 되고, 오른쪽 테이블은 내부 테이블이 된다.
*   **USE_IDX** : 인덱스 관련한 힌트로서, 질의 최적화기는 명시된 테이블에 대해 인덱스 조인 실행 계획을 만든다.
*   **USE_DESC_IDX** : 내림차순 스캔을 위한 힌트이다. 자세한 내용은 :ref:`index-descending-scan` 을 참고한다.
*   **NO_DESC_IDX** : 내림차순 스캔을 사용하지 않도록 하는 힌트이다.
*   **NO_COVERING_IDX** : 커버링 인덱스 기능을 사용하지 않도록 하는 힌트이다. 자세한 내용은 :ref:`covering-index` 를 참고한다.
*   **NO_MULTI_RANGE_OPT** : 다중 키 범위 최적화 기능을 사용하지 않도록 하는 힌트이다. 자세한 내용은 :ref:`multi-key-range-opt` 를 참고한다.
*   **RECOMPILE** : 질의 실행 계획을 리컴파일한다. 캐시에 저장된 기존 질의 실행 계획을 삭제하고 새로운 질의 실행 계획을 수립하기 위해 이 힌트를 사용한다.

    .. note:: *spec_name* 이 **USE_NL**, **USE_IDX**, **USE_MERGE** 와 함께 지정될 경우 해당 조인 방법은 *spec_name* 에 대해서만 적용된다. 만약 **USE_NL** 과 **USE_MERGE** 가 함께 지정된 경우 주어진 힌트는 무시된다. 일부 경우에 질의 최적화기는 주어진 힌트에 따라 질의 실행 계획을 만들지 못할 수 있다. 예를 들어 오른쪽 외부 조인에 대해 **USE_NL** 을 지정한 경우 이 질의는 내부적으로 왼쪽 외부 조인 질의로 변환이 되어 조인 순서는 보장되지 않을 수 있다.

MERGE 문에는 다음과 같은 힌트를 사용할 수 있다. 

*   **USE_INSERT_INDEX** (<*insert_index_list*>) : MERGE 문의 INSERT 절에서 사용되는 인덱스 힌트. *insert_index_list*\ 에 INSERT 절을 수행할 때 사용할 인덱스 이름을 나열한다. MERGE 문의 <*join_condition*>에 해당 힌트가 적용된다.
*   **USE_UPDATE_INDEX** (<*update_index_list*>) : MERGE 문의 UPDATE 절에서 사용되는 인덱스 힌트. *update_index_list*\ 에 UPDATE 절을 수행할 때 사용할 인덱스 이름을 나열한다. MERGE 문의 <*join_condition*>과 <*update_condition*>에 해당 힌트가 적용된다.
*   **RECOMPILE** : 질의 실행 계획을 리컴파일한다. 캐시에 저장된 기존 질의 실행 계획을 삭제하고 새로운 질의 실행 계획을 수립하기 위해 이 힌트를 사용한다.

다음은 심권호 선수가 메달을 획득한 연도와 메달 종류를 구하는 예제이다. 단, *athlete* 테이블을 외부 테이블로 하고 *game* 테이블을 내부 테이블로 하는 중첩 루프 조인 실행 계획을 만들어야 한다. 다음과 같은 질의로 표현이 되는데, 질의최적화기는 *game* 테이블을 외부 테이블로 하고, *athlete* 테이블을 내부 테이블로 하는 중첩 루프 조인 실행 계획을 만든다.

.. code-block:: sql

    SELECT /*+ USE_NL ORDERED  */ a.name, b.host_year, b.medal
    FROM athlete a, game b 
    WHERE a.name = 'Sim Kwon Ho' AND a.code = b.athlete_code;
    
      name                    host_year  medal
    =========================================================
      'Sim Kwon Ho'                2000  'G'
      'Sim Kwon Ho'                1996  'G'
      
    2 rows selected.

다음은 데이터가 없는 분할 테이블(*before_2008*)의 삭제 성능을 높이기 위해 **NO_STATS** 힌트를 사용하여 질의 실행 시간을 확인하는 예제이다. *participant2* 테이블에는 100만 건 이상의 데이터가 있는 것으로 가정한다. 아래 실행 시간의 차이는 시스템 성능 및 데이터베이스 구성 방법에 따라 다를 수 있다.

.. code-block:: sql

    -- NO_STATS 힌트 미사용
    ALTER TABLE participant2 DROP partition before_2008;

    SQL statement execution time:      31.684550 sec

    -- NO_STATS 힌트 사용
    ALTER /*+ NO_STATS */ TABLE participant2 DROP partition before_2008;

    SQL statement execution time:      0.025773 sec

.. _index-hint-syntax:

인덱스 힌트
===========

인덱스 힌트 구문은 질의에서 인덱스를 지정할 수 있도록 해서 질의 처리기가 적절한 인덱스를 선택할 수 있게 한다. 이와 같은 인덱스 힌트 구문은 USING INDEX 절을 사용하는 방식과 FROM 절에 { USE | FORCE | IGNORE } INDEX 구문을 사용하는 방식이 있다.

USING INDEX
-----------

**USING INDEX** 절은 **SELECT**, **DELETE**, **UPDATE** 문의 **WHERE** 절 다음에 지정되어야 한다. **USING INDEX** 절에 강제로 순차 스캔 또는 인덱스 스캔이 사용되게 하거나, 성능에 유리한 인덱스가 포함되도록 한다.

**USING INDEX** 절에 인덱스 이름의 리스트가 지정되면 질의 최적화기는 지정된 인덱스만을 대상으로 질의 실행 비용을 계산하고, 지정된 인덱스의 인덱스 스캔 비용과 순차 스캔 비용을 비교하여 최적의 실행 계획을 만든다(CUBRID는 실행 계획을 선택할 때 비용 기반의 질의 최적화를 수행한다).

**USING INDEX** 절은 **ORDER BY** 없이 원하는 순서로 결과를 얻고자 할 때 유용하게 사용될 수 있다. CUBRID는 인덱스 스캔을 하면 인덱스에 저장된 순서로 결과가 생성되는데, 한 테이블에 여러 인덱스가 있을 경우 특정 인덱스의 순서로 질의 결과를 얻고자 할 때 **USING INDEX** 를 사용할 수 있다. 

::

    SELECT ... WHERE ...
    [USING INDEX { NONE | [ ALL EXCEPT ] <index_spec> [ {, <index_spec> } ...] } ] [ ; ]
    
    DELETE ... WHERE ...
    [USING INDEX { NONE | [ ALL EXCEPT ] <index_spec> [ {, <index_spec> } ...] } ] [ ; ]
    
    UPDATE ... WHERE ...
    [USING INDEX { NONE | [ ALL EXCEPT ] <index_spec> [ {, <index_spec> } ...] } ] [ ; ] 
    
    <index_spec> ::=
      [table_spec.]index_name [(+) | (-)] |
      table_spec.NONE
 

*   **NONE** : **NONE** 을 지정한 경우 모든 테이블에 대해서 순차 스캔이 사용된다.
*   **ALL EXCEPT** : 질의 수행 시 지정한 인덱스를 제외한 모든 인덱스가 사용될 수 있다.
*   *index_name*\ (+) : 인덱스 이름 뒤에 (+)를 지정하면 해당 인덱스 선택이 우선시 된다. 해당 인덱스가 해당 질의를 수행하는데 적합하지 않으면 선택하지 않는다. 
*   *index_name*\ (-) : 인덱스 이름 뒤에 (-)를 지정하면 해당 인덱스가 선택에서 제외된다.
*   *table_spec*.\ **NONE** : 해당 테이블의 모든 인덱스가 선택에서 제외되어 순차 스캔이 사용된다.

USE, FORCE, IGNORE INDEX
------------------------

FROM 절의 테이블 명세 뒤에 **USE**, **FORCE**, **IGNORE INDEX** 구문을 통해서 인덱스 힌트를 지정할 수 있다. 

::

    FROM table_spec [ <index_hint_clause> ] ...
    
    <index_hint_clause> ::=
      { USE | FORCE | IGNORE } INDEX  ( <index_spec> [, <index_spec>  ...] )
    
    <index_spec> ::=
      [table_spec.]index_name

*    **USE INDEX** ( <*index_spec*> ): 지정한 인덱스들만 선택 시에 고려한다. 
*    **FORCE INDEX** ( <*index_spec*> ): 해당 인덱스 선택이 우선시 된다. 
*    **IGNORE INDEX** ( <*index_spec*> ): 지정한 인덱스들은 선택에서 제외된다. 

USE, FORCE, IGNORE INDEX 구문은 시스템에 의해 자동적으로 적절한 USING INDEX 구문으로 질의 재작성된다.

인덱스 힌트 사용 예
-------------------

.. code-block:: sql

    CREATE TABLE athlete (
       code             SMALLINT PRIMARY KEY,
       name             VARCHAR(40) NOT NULL,
       gender           CHAR(1),
       nation_code      CHAR(3),
       event            VARCHAR(30)
    );
    CREATE UNIQUE INDEX athlete_idx1 ON athlete (code, nation_code);
    CREATE INDEX athlete_idx2 ON athlete (gender, nation_code);

아래 2개의 질의는 같은 동작을 수행하며, 지정된 *athlete_idx2* 인덱스 스캔 비용이 순차 스캔 비용보다 작을 경우 해당 인덱스 스캔을 선택하게 된다. 

.. code-block:: sql

    SELECT /*+ RECOMPILE */ * 
    FROM athlete USE INDEX (athlete_idx2) 
    WHERE gender='M' AND nation_code='USA';

    SELECT /*+ RECOMPILE */ * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete_idx2;

아래 2개의 질의는 같은 동작을 수행하며, 항상 *athlete_idx2*\ 를 사용한다.

.. code-block:: sql
    
    SELECT /*+ RECOMPILE */ * 
    FROM athlete FORCE INDEX (athlete_idx2) 
    WHERE gender='M' AND nation_code='USA';

    SELECT /*+ RECOMPILE */ * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete_idx2(+);

아래 2개의 질의는 같은 동작을 수행하며, 질의 수행 시 *athlete_idx2*\ 를 사용하지 않는다.

.. code-block:: sql
    
    SELECT /*+ RECOMPILE */ * 
    FROM athlete IGNORE INDEX (athlete_idx2) 
    WHERE gender='M' AND nation_code='USA';

    SELECT /*+ RECOMPILE */ * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete_idx2(-);

다음 질의는 수행 시 항상 순차 스캔을 선택한다.

.. code-block:: sql

    SELECT * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX NONE;

    SELECT * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete.NONE;

다음 질의는 수행 시 *athlete_idx2*\ 를 제외한 모든 인덱스의 사용이 가능하도록 한다.

.. code-block:: sql

    SELECT * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX ALL EXCEPT athlete_idx2;

다음과 같이 **USE INDEX** 구문 또는 **USING INDEX** 구문에서 여러 인덱스를 지정한 경우 질의 최적화기는 지정된 인덱스 중 하나를 선택한다.

.. code-block:: sql

    SELECT * 
    FROM athlete USE INDEX (athlete_idx2, athlete_idx1) 
    WHERE gender='M' AND nation_code='USA';

    SELECT * 
    FROM athlete 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete_idx2, athlete_idx1;

여러 개의 테이블에 대해 질의를 수행하는 경우, 한 테이블에서는 특정 인덱스를 사용하여 인덱스 스캔을 하고 다른 테이블에서는 순차 스캔을 하도록 지정할 수 있다. 이러한 질의는 다음과 같은 형태가 된다.

.. code-block:: sql

    SELECT *
    FROM tab1, tab2 
    WHERE ... 
    USING INDEX tab1.idx1, tab2.NONE;

인덱스 힌트 구문이 있는 질의를 수행할 때 질의 최적화기는 인덱스가 지정되지 않는 테이블에 대해서는 해당 테이블의 사용 가능한 모든 인덱스를 고려한다. 예를 들어, *tab1* 테이블에는 인덱스 *idx1*, *idx2* 이 있고 *tab2* 테이블에는 인덱스 *idx3*, *idx4*, *idx5* 가 있는 경우, *tab1* 에 대한 인덱스만 지정하고 *tab2* 에 대한 인덱스를 지정하지 않으면 질의 최적화기는 *tab2* 의 인덱스도 고려하여 동작한다.

.. code-block:: sql

    SELECT ... 
    FROM tab1, tab2 USE INDEX (tab1.idx1) 
    WHERE ... ;
    
    SELECT ... 
    FROM tab1, tab2 
    WHERE ... 
    USING INDEX tab1.idx1;

위의 예제의 경우에 테이블 *tab1*\ 의 순차 스캔과 *idx1* 인덱스 스캔을 비교하여 테이블 *tab1*\ 의 스캔 방법을 선택하며, 테이블 *tab2*\ 의 순차 스캔과 *idx3*, *idx4*, *idx5* 인덱스 스캔을 비교하여 테이블 *tab2*\ 의 스캔 방법을 선택하게 된다.

특별한 인덱스
=============

.. _filtered-index:

필터링된 인덱스
---------------

필터링된 인덱스(filtered index)는 한 테이블에 대해 잘 정의된 부분 집합을 정렬하거나 찾거나 연산해야 할 때 사용되며, 전체 데이터에서 조건에 부합하는 일부 데이터만 인덱스에 유지하므로 부분 인덱스(partial index)라고도 한다. ::

    CREATE /*+ hints */ INDEX index_name
    ON table_name (col1, col2, ...) 
    WHERE <filter_predicate>;
     
    ALTER  /*+ hints */ INDEX index_name
    [ ON table_name (col1, col2, ...) 
    [ WHERE <filter_predicate> ] ]
    REBUILD;
     
    <filter_predicate> ::= <filter_predicate> AND <expression> | <expression>

*   <*filter_predicate*>: 칼럼과 상수 간 비교 조건. 조건이 여러 개인 경우 **AND** 로 연결된 경우에만 필터가 될 수 있다. 필터 조건으로 CUBRID에서 지원하는 대부분의 연산자와 함수가 포함될 수 있다. 그러나 현재 날짜/시간을 출력하는 날짜/시간 함수(예: :func:`SYS_DATETIME`), 랜덤 함수(예: :func:`RAND`)와 같이 같은 입력에 대해 다른 결과를 출력하는 함수는 허용되지 않는다.

필터링된 인덱스를 적용하여 질의를 처리하려면 **USING INDEX** 절 또는 **USE INDEX** 구문을 통해 해당 필터링된 인덱스를 반드시 명시해야 한다.

.. code-block:: sql

    SELECT * 
    FROM blogtopic 
    WHERE postDate >'2010-01-01' 
    USING INDEX my_filter_index;

다음은 버그/이슈를 유지하는 버그 트래킹 시스템의 예이다. 일정 기간의 개발 활동 이후 bugs 테이블에는 버그들이 기록되어 있는데, 이들 대부분은 오래 전에 종료된 상태이다. 버그 트래킹 시스템은 여전히 열린(open) 상태의 새로운 버그를 찾기 위해 해당 테이블에 질의를 한다. 이 경우 버그 테이블의 인덱스는 닫힌(closed) 버그의 레코드들에 대해 알 필요가 없다. 이런 경우 필터링된 인덱스는 열린 버그만 인덱싱하는 것을 허용한다.

.. code-block:: sql

    CREATE TABLE bugs
    (
        bugID BIGINT NOT NULL,
        CreationDate TIMESTAMP,
        Author VARCHAR(255),
        Subject VARCHAR(255),
        Description VARCHAR(255),
        CurrentStatus INTEGER,
        Closed SMALLINT
    );

열린 상태의 버그만을 위한 인덱스는 다음 문장으로 생성될 수 있다.

.. code-block:: sql

    CREATE INDEX idx_open_bugs ON bugs (bugID) WHERE Closed = 0;

열린 상태의 버그에만 관심있는 질의 처리를 위해 해당 인덱스를 인덱스 힌트로 지정하면, 필터링된 인덱스를 통하여 더 적은 인덱스 페이지를 접근하여 질의 결과를 생성할 수 있게 된다.

.. code-block:: sql

    SELECT * 
    FROM bugs
    WHERE Author = 'madden' AND Subject LIKE '%fopen%' AND Closed = 0
    USING INDEX idx_open_bugs;
     
    SELECT * 
    FROM bugs USE INDEX (idx_open_bugs)
    WHERE CreationDate > CURRENT_DATE - 10 AND Closed = 0;
    
.. warning::

    필터링된 인덱스 생성 조건과 질의 조건이 부합되지 않음에도 불구하고 인덱스 힌트 구문으로 인덱스를 명시하여 질의를 수행하면 잘못된 질의 결과를 출력할 수 있음에 주의한다.

**제약 사항**

필터링된 인덱스는 일반 인덱스만 허용된다. 예를 들어, 필터링된 유일한(unique) 인덱스는 허용되지 않는다. 

다음은 인덱스 필터 조건으로 허용하지 않는 경우이다.

*   날짜/시간 함수 또는 랜덤 함수와 같이 입력이 같은데 결과가 매번 다른 함수
    
    .. code-block:: sql
    
        CREATE INDEX idx ON bugs (creationdate) WHERE creationdate > SYS_DATETIME;
            
        ERROR: before ' ; '
        'sys_datetime ' is not allowed in a filter expression for index.
            
        CREATE INDEX idx ON bugs (bugID) WHERE bugID > RAND();
            
        ERROR: before ' ; '
        'rand ' is not allowed in a filter expression for index.
    
*   **OR** 연산자를 사용하는 경우

    .. code-block:: sql

        CREATE INDEX IDX ON bugs (bugID) WHERE bugID > 10 OR bugID = 3;
         
        In line 1, column 62,
         
        ERROR: before ' ; '
        ' or ' is not allowed in a filter expression for index.

*   :func:`INCR`, :func:`DECR` 함수와 같이 테이블의 데이터를 수정하는 함수를 포함한 경우

*   시리얼 관련 함수와 의사 칼럼을 포함한 경우

*   :func:`MIN`, :func:`MAX`, :func:`STDDEV` 등 집계 함수를 포함한 경우

*   인덱스를 생성할 수 없는 타입을 사용하는 함수

    -   SET 타입을 인자로 받는 연산자와 함수
    -   LOB 파일을 생성하는 함수 (:func:`CHAR_TO_BLOB`, :func:`CHAR_TO_CLOB`, :func:`BIT_TO_BLOB`, :func:`BLOB_FROM_FILE`, :func:`CLOB_FROM_FILE`)

*   **IS NULL** 연산자는 인덱스를 구성하는 칼럼들 중 적어도 하나가 **NULL** 이 아닐 경우에만 사용 가능

    .. code-block:: sql
    
        CREATE TABLE t (a INT, b INT);
        
        -- IS NULL cannot be used with expressions
        CREATE INDEX idx ON t (a) WHERE (not a) IS NULL;
        
        ERROR: before ' ; '
        Invalid filter expression (( not t.a<>0) is null ) for index.
         
        CREATE INDEX idx ON t (a) WHERE (a+1) IS NULL;
        
        ERROR: before ' ; '
        Invalid filter expression ((t.a+1) is null ) for index.
         
        -- At least one attribute must not be used with IS NULL
        CREATE INDEX idx ON t(a,b) WHERE a IS NULL ;
        
        ERROR: before '  ; '
        Invalid filter expression (t.a is null ) for index.
         
        CREATE INDEX idx ON t(a,b) WHERE a IS NULL and b IS NULL;
        
        ERROR: before ' ; '
        Invalid filter expression (t.a is null  and t.b is null ) for index.
         
        CREATE INDEX idx ON t(a,b) WHERE a IS NULL and b IS NOT NULL;

*   필터링된 인덱스에 대한 인덱스 스킵 스캔(ISS)은 지원되지 않는다.
*   필터링된 인덱스에서 사용되는 조건 문자열의 길이는 128자로 제한한다.

    .. code-block:: sql

        CREATE TABLE t(VeryLongColumnNameOfTypeInteger INT);
            
        CREATE INDEX idx ON t(VeryLongColumnNameOfTypeInteger) 
        WHERE VeryLongColumnNameOfTypeInteger > 3 AND VeryLongColumnNameOfTypeInteger < 10 AND 
        sqrt(VeryLongColumnNameOfTypeInteger) < 3 AND SQRT(VeryLongColumnNameOfTypeInteger) < 10;
        
        ERROR: before ' ; '
        The maximum length of filter predicate string must be 128.

.. _function-index:

함수 기반 인덱스
----------------

함수 기반 인덱스(function-based index)는 특정 함수를 이용하여 테이블 행들로부터 값의 조합에 기반한 데이터를 정렬하거나 찾고 싶을 때 사용한다. 예를 들어, 공백을 무시한 문자열을 찾는 작업을 하고 싶을 때 이러한 기능을 수행하는 함수를 이용하게 되는데, 함수를 통해 칼럼 값을 변경하게 되면 일반 인덱스를 통해서 인덱스 스캔을 할 수 없다. 이러한 경우에 함수 기반 인덱스를 생성하면 이를 통해 해당 질의 처리를 최적화할 수 있다. 다른 예로, 대소문자를 구분하지 않는 이름을 검색할 때 활용할 수 있다. ::

    CREATE /*+ hints */ INDEX index_name
    ON table_name (function_name (argument_list));
    
    ALTER /*+ hints */ INDEX index_name
    [ ON table_name (function_name (argument_list)) ]
    REBUILD;

다음 인덱스가 생성된 이후 **SELECT** 질의는 자동으로 함수 기반 인덱스를 사용한다.

.. code-block:: sql
  
    CREATE INDEX idx_trim_post ON posts_table (TRIM (keyword));
    
    SELECT * 
    FROM posts_table 
    WHERE TRIM (keyword) = 'SQL';

**LOWER** 함수로 함수 기반 인덱스를 생성하면, 대소문자 구분을 안 하는 이름을 검색할 때 사용될 수 있다.

.. code-block:: sql

    CREATE INDEX idx_last_name_lower ON clients_table (LOWER (LastName));
    
    SELECT * 
    FROM clients_table 
    WHERE LOWER (LastName) = LOWER ('Timothy');

질의 계획을 생성할 때 인덱스가 선택되게 하기 위해서는, 이 인덱스에서 사용되는 함수가 질의 조건에서 같은 방법으로 사용되어야 한다. 위의 **SELECT** 질의는 위에서 생성된 last_name_lower 인덱스를 사용한다. 하지만 다음과 같은 조건에서는 함수 기반 인덱스 형태와 다른 표현식이 주어졌기 때문에 인덱스가 사용되지 않는다.

.. code-block:: sql

    SELECT * 
    FROM clients_table
    WHERE LOWER (CONCAT ('Mr. ', LastName)) = LOWER ('Mr. Timothy');

함수 기반 인덱스의 사용을 강제하려면 **USING INDEX** 구문을 사용할 수 있다.

.. code-block:: sql

    CREATE INDEX i_tbl_first_four ON tbl (LEFT (col, 4));
    SELECT * 
    FROM clients_table 
    WHERE LEFT (col, 4) = 'CAT5' 
    USING INDEX i_tbl_first_four;

.. _allowed-function-in-function-index:

함수 기반 인덱스로 사용할 수 있는 함수는 다음과 같다. 

    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | ABS               | ACOS              | ADD_MONTHS        | ADDDATE           | ASIN              |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | ATAN              | ATAN2             | BIT_COUNT         | BIT_LENGTH        | CEIL              |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | CHAR_LENGTH       | CHR               | COS               | COT               | DATE              |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | DATE_ADD          | DATE_FORMAT       | DATE_SUB          | DATEDIFF          | DAY               |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | DAYOFMONTH        | DAYOFWEEK         | DAYOFYEAR         | DEGREES           | EXP               |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | FLOOR             | FORMAT            | FROM_DAYS         | FROM_UNIXTIME     | GREATEST          |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | HOUR              | IFNULL            | INET_ATON         | INET_NTOA         | INSTR             |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | LAST_DAY          | LEAST             | LEFT              | LN                | LOCATE            |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | LOG               | LOG10             | LOG2              | LOWER             | LPAD              |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | LTRIM             | MAKEDATE          | MAKETIME          | MD5               | MID               |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | MINUTE            | MOD               | MONTH             | MONTHS_BETWEEN    | NULLIF            |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | NVL               | NVL2              | OCTET_LENGTH      | POSITION          | POWER             |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | QUARTER           | RADIANS           | REPEAT            | REPLACE           | REVERSE           |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | RIGHT             | ROUND             | RPAD              | RTRIM             | SECOND            |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | SECTOTIME         | SIN               | SQRT              | STR_TO_DATE       | STRCMP            |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | SUBDATE           | SUBSTR            | SUBSTRING         | SUBSTRING_INDEX   | TAN               |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | TIME              | TIME_FORMAT       | TIMEDIFF          | TIMESTAMP         | TIMETOSEC         |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | TO_CHAR           | TO_DATE           | TO_DATETIME       | TO_DAYS           | TO_NUMBER         |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | TO_TIME           | TO_TIMESTAMP      | TRANSLATE         | TRIM              | TRUNC             |
    +-------------------+-------------------+-------------------+-------------------+-------------------+
    | UNIX_TIMESTAMP    | UPPER             | WEEK              | WEEKDAY           | YEAR              |
    +-------------------+-------------------+-------------------+-------------------+-------------------+

함수 기반 인덱스에서 사용할 함수의 인자는 테이블의 칼럼 이름 혹은 상수인 경우만 허용하며, 복잡한 중첩된 표현식은 허용하지 않는다. 예를 들어 아래의 문장은 오류를 발생한다.

.. code-block:: sql

    CREATE INDEX my_idx ON tbl (TRIM (LEFT (col, 3)));
    CREATE INDEX my_idx ON tbl (LEFT (col1, col2 + 3));

묵시적인 타입 변환(implicit type cast)은 허용된다. 아래의 예에서 :func:`LEFT` 함수는 첫 번째 인자 타입이 **VARCHAR** 이고 두 번째 인자 타입이 **INTEGER** 여야 하지만 정상 동작한다.

.. code-block:: sql

    CREATE INDEX my_idx ON tbl (LEFT (int_col, str_col));

함수 기반 인덱스는 필터링된 인덱스와 함께 사용될 수 없다. 아래의 예는 오류를 발생한다.

.. code-block:: sql

    CREATE INDEX my_idx ON tbl (TRIM (col)) WHERE col > 'SQL';

함수 기반 인덱스는 다중 칼럼 인덱스가 될 수 없다. 아래의 예는 오류를 발생한다.

.. code-block:: sql

    CREATE INDEX my_idx ON tbl (TRIM (col1), col2, LEFT (col3, 5));


.. _tuning-index:

인덱스를 활용한 최적화
======================

.. _covering-index:

커버링 인덱스
-------------

질의 수행 시 **SELECT** 리스트, **WHERE**, **HAVING**, **GROUP BY**, **ORDER BY** 절에 있는 모든 칼럼의 데이터를 포함하는 인덱스를 커버링 인덱스(covering index)라고 한다.

커버링 인덱스는 질의 수행 시 인덱스 내에 필요한 모든 데이터를 지니고 있어서 인덱스 페이지만 검색하면 되며, 데이터 저장소를 추가로 검색할 필요가 없어 데이터 저장소 접근을 위한 I/O 비용을 줄일 수 있다. 데이터 검색 속도를 향상시키기 위해 커버링 인덱스로 생성하는 것을 고려할 수 있지만, 인덱스의 크기가 커지면 **INSERT** 와 **DELETE** 작업은 느려질 수 있다는 점을 감안해야 한다.

커버링 인덱스의 적용 여부에 대한 규칙은 다음과 같다.

*   CUBRID 질의 최적화기는 커버링 인덱스의 적용이 가능하면 이를 가장 먼저 사용한다.
*   조인 질의의 경우 인덱스가 **SELECT** 리스트에 있는 테이블의 칼럼을 포함하면, 이 인덱스를 사용한다.
*   인덱스를 사용할 수 있는 조건이 아닌 경우 커버링 인덱스를 사용할 수 없다.

.. code-block:: sql

    CREATE TABLE t (col1 INT, col2 INT, col3 INT);
    CREATE INDEX i_t_col1_col2_col3 ON t (col1,col2,col3);
    INSERT INTO t VALUES (1,2,3),(4,5,6),(10,8,9);

다음의 예는 **SELECT** 하는 칼럼과 **WHERE** 조건의 칼럼이 모두 인덱스 내에 존재하므로, 해당 인덱스가 커버링 인덱스로 사용된다.

.. code-block:: sql

    csql> ;plan simple
    SELECT * FROM t WHERE col1 < 6;
     
    Query plan:
     Index scan(t t, i_t_col1_col2_col3, [(t.col1 range (min inf_lt t.col3))] (covers))
     
             col1         col2         col3
    =======================================
                1            2            3
                4            5            6

.. warning::

    **VARCHAR** 타입의 칼럼에서 값을 가져올 때 커버링 인덱스가 적용되는 경우, 뒤에 따라오는 공백 문자열은 잘리게 된다. 질의 최적화 수행 시 커버링 인덱스가 적용되면 질의 결과 값을 인덱스에서 가져오는데, 인덱스에는 뒤이어 나타나는 공백 문자열을 제거한 채로 값을 저장하기 때문이다.

    이러한 현상을 원하지 않는다면 커버링 인덱스 기능을 사용하지 않도록 하는 **NO_COVERING_IDX** 힌트를 사용한다. 이 힌트를 사용하면 결과값을 인덱스 영역이 아닌 데이터 영역에서 가져오도록 한다.

    다음은 위의 상황의 자세한 예이다. 먼저 **VARCHAR** 타입의 칼럼을 갖는 테이블을 생성하고, 여기에 시작 문자열의 값이 같고 문자열 뒤에 따르는 공백 문자의 개수가 다른 값을 **INSERT** 한다. 그리고 해당 칼럼에 인덱스를 생성한다.

    .. code-block:: sql

        CREATE TABLE tab (c VARCHAR(32));
        INSERT INTO tab VALUES ('abcd'), ('abcd    '), ('abcd ');
        CREATE INDEX i_tab_c ON tab (c);

    인덱스를 반드시 사용하도록(커버링 인덱스가 적용되도록) 했을 때의 질의 결과는 다음과 같다.

    .. code-block:: sql

        csql>;plan simple
        SELECT * FROM tab WHERE c='abcd    ' USING INDEX i_tab_c(+);
         
        Query plan:
         Index scan(tab tab, i_tab_c, (tab.c='abcd    ') (covers))
         
         c
        ======================
        'abcd'
        'abcd'
        'abcd'

    다음은 인덱스를 사용하지 않도록 했을 때의 질의 결과이다.

    .. code-block:: sql

        SELECT * FROM tab WHERE c='abcd    ' USING INDEX tab.NONE;
         
        Query plan:
         Sequential scan(tab tab)
         
         c
        ======================
        'abcd'
        'abcd    '
        'abcd '

    위의 두 결과 비교에서 알 수 있듯이, 커버링 인덱스가 적용되면 **VARCHAR** 타입에서는 인덱스로부터 값을 가져오면서 뒤이어 나타나는 공백 문자열이 잘린 채로 나타난다.

.. note:: 커버링 인덱스 최적화가 적용될 수 있으면 디스크 입출력을 상당히 줄일 수 있기 때문에 성능 향상을 기대할 수 있다. 하지만 특정한 상황에서 커버링 인덱스 스캔 최적화를 원하지 않는다면, 질의에 **NO_COVERING_IDX** 힌트를 명시하면 된다. 힌트를 지정하는 방법은 :ref:`sql-hint`\ 를 참고하면 된다.

.. _order-by-skip-optimization:

ORDER BY 절 최적화
------------------

**ORDER BY** 절에 있는 모든 칼럼을 포함하는 인덱스를 정렬된 인덱스(ordered index)라고 한다. **ORDER BY** 절이 있는 질의를 최적화하면 정렬된 인덱스를 통해 질의 결과를 탐색하므로 별도의 정렬 과정을 거치지 않는다(skip order by). 정렬된 인덱스가 되기 위한 일반적인 조건은 **ORDER BY** 절에 있는 칼럼들이 인덱스의 가장 앞부분에 위치하는 경우이다.

.. code-block:: sql

    SELECT * 
    FROM tab 
    WHERE col1 > 0 
    ORDER BY col1, col2;

*   *tab* (*col1*, *col2*) 으로 구성된 인덱스는 정렬된 인덱스이다.
*   *tab* (*col1*, *col2*, *col3*) 으로 구성된 인덱스도 정렬된 인덱스이다. **ORDER BY** 절에서 참조하지 않는 *col3* 는 *col1*, *col2* 뒤에 오기 때문이다.
*   *tab* (*col1*) 으로 구성된 인덱스는 정렬된 인덱스가 아니다.
*   *tab* (*col3*, *col1*, *col2*) 혹은 *tab* (*col1*, *col3*, *col2*)로 구성된 인덱스는 최적화에 사용할 수 없다. 이는 *col3* 가 **ORDER BY** 절의 칼럼들 뒤에 위치하지 않기 때문이다.

인덱스를 구성하는 칼럼이 **ORDER BY** 절에 없더라도 그 칼럼의 조건이 상수일 때는 정렬된 인덱스의 사용이 가능하다.

.. code-block:: sql

    SELECT * 
    FROM tab 
    WHERE col2=val 
    ORDER BY col1,col3;

*tab* (*col1*, *col2*, *col3*)로 구성된 인덱스가 존재하고 *tab* (*col1*, *col2*)로 구성된 인덱스는 없이 위의 질의를 수행할 때, 질의 최적화기는 *tab* (*col1*, *col2*, *col3*)로 구성된 인덱스를 정렬된 인덱스로 사용한다. 즉, 인덱스 스캔 시 요구하는 순서대로 결과를 가져오므로, 레코드를 정렬할 필요가 없다.

정렬된 인덱스와 커버링 인덱스를 함께 사용할 수 있으면 커버링 인덱스를 먼저 사용한다. 커버링 인덱스를 사용하면 요청한 데이터의 결과가 인덱스 페이지에 모두 들어 있어 추가적인 데이터를 검색할 필요가 없으며, 이 인덱스가 순서까지 만족한다면, 결과를 정렬할 필요가 없기 때문이다.

질의가 조건을 포함하지 않으며 정렬된 인덱스를 사용할 수 있다면, 인덱스의 첫 번째 칼럼이 **NOT NULL** 조건을 만족한다는 전제 하에서는 정렬된 인덱스가 사용될 것이다.

.. code-block:: sql

    CREATE TABLE tab (i INT, j INT, k INT);
    CREATE INDEX i_tab_j_k on tab (j, k);
    INSERT INTO tab VALUES (1,2,3), (6,4,2), (3,4,1), (5,2,1), (1,5,5), (2,6,6), (3,5,4);

다음의 예는 *j*, *k* 칼럼으로 **ORDER BY** 를 수행하므로 *tab* (*j*, *k*)로 구성된 인덱스는 정렬된 인덱스가 되고 별도의 정렬 과정을 거치지 않는다.

.. code-block:: sql

    SELECT i,j,k 
    FROM tab 
    WHERE j > 0 
    ORDER BY j,k;
     
    --  the  selection from the query plan dump shows that the ordering index i_tab_j_k was used and sorting was not necessary
    --  (/* --> skip ORDER BY */)
    Query plan:
    iscan
        class: tab node[0]
        index: i_tab_j_k term[0]
        sort:  2 asc, 3 asc
        cost:  1 card 0
    Query stmt:
    select tab.i, tab.j, tab.k from tab tab where ((tab.j> ?:0 )) order by 2, 3
    /* ---> skip ORDER BY */
     
                i            j            k
    =======================================
                5            2            1
                1            2            3
                3            4            1
                6            4            2
                3            5            4
                1            5            5
                2            6            6

다음의 예는 j, k 칼럼으로 **ORDER BY** 를 수행하며 **SELECT** 하는 칼럼을 모두 포함하는 인덱스가 존재하므로 tab(j,k)로 구성된 인덱스가 커버링 인덱스로서 사용된다. 따라서 인덱스 자체에서 값을 가져오게 되며 별도의 정렬 과정을 거치지 않는다.

.. code-block:: sql

    SELECT /*+ RECOMPILE */ j,k 
    FROM tab 
    WHERE j > 0 
    ORDER BY j,k;
     
    --  in this case the index i_tab_j_k is a covering index and also respects the ordering index property.
    --  Therefore, it is used as a covering index and sorting is not performed.
     
    Query plan:
    iscan
        class: tab node[0]
        index: i_tab_j_k term[0] (covers)
        sort:  1 asc, 2 asc
        cost:  1 card 0
     
    Query stmt: select tab.j, tab.k from tab tab where ((tab.j> ?:0 )) order by 1, 2
    /* ---> skip ORDER BY */
     
                j            k
    ==========================
                2            1
                2            3
                4            1
                4            2
                5            4
                5            5
                6            6

다음의 예는 *i* 칼럼 조건이 있으며 *j*, *k* 칼럼으로 **ORDER BY** 를 수행하고, **SELECT** 하는 칼럼이 *i*, *j*, *k* 이므로 *tab* (*i*, *j*, *k*)로 구성된 인덱스가 커버링 인덱스로서 사용된다. 따라서 인덱스 자체에서 값을 가져오게 되지만, **ORDER BY** *j*, *k* 에 대한 별도의 정렬 과정을 거친다.

.. code-block:: sql

    CREATE INDEX i_tab_j_k ON tab (i,j,k);
    SELECT /*+ RECOMPILE */ i,j,k 
    FROM tab 
    WHERE i > 0 
    ORDER BY j,k;
     
    -- since an index on (i,j,k) is now available, it will be used as covering index. However, sorting the results according to
    -- the ORDER BY  clause is needed.
    Query plan:
    temp(order by)
        subplan: iscan
                     class: tab node[0]
                     index: i_tab_i_j_k term[0] (covers)
                     sort:  1 asc, 2 asc, 3 asc
                     cost:  1 card 1
        sort:  2 asc, 3 asc
        cost:  7 card 1
     
    Query stmt: select tab.i, tab.j, tab.k from tab tab where ((tab.i> ?:0 )) order by 2, 3
     
                i            j            k
    =======================================
                5            2            1
                1            2            3
                3            4            1
                6            4            2
                3            5            4
                1            5            5
                2            6            6

.. note::
    :func:`CAST` 연산자 등을 통해 ORDER BY 절의 칼럼이 타입 변환되더라도, 타입 변환 이전의 정렬 순서와 타입 변환 이후의 정렬 순서가 같다면 ORDER BY 절 최적화가 수행된다.
    
        +----------------+----------------+
        | 변환 이전      | 변환 이후      |
        +================+================+
        | 수치형 타입    | 수치형 타입    |
        +----------------+----------------+
        | 문자열 타입    | 문자열 타입    |
        +----------------+----------------+
        | DATETIME       | TIMESTAMP      |
        +----------------+----------------+
        | TIMESTAMP      | DATETIME       |
        +----------------+----------------+
        | DATETIME       | DATE           |
        +----------------+----------------+
        | TIMESTAMP      | DATE           |
        +----------------+----------------+
        | DATE           | DATETIME       |
        +----------------+----------------+

.. _index-descending-scan:

내림차순 인덱스 스캔
--------------------

다음과 같이 내림차순 정렬이 있는 질의를 수행할 때 일반적으로 내림차순 인덱스를 생성하여 인덱스를 사용하도록 하면 별도의 정렬 과정이 필요 없다.

.. code-block:: sql

    SELECT * 
    FROM tab 
    [WHERE ...] 
    ORDER BY a DESC;

그런데 같은 칼럼에 대해 오름차순 인덱스와 내림차순 인덱스를 생성하면 교착 상태(deadlock)의 발생 가능성이 높아진다. 이러한 경우를 줄이기 위해 CUBRID는 별도의 내림차순 인덱스를 생성하지 않아도, 오름차순 인덱스만으로 내림차순 인덱스 스캔을 사용할 수 있다. 사용자는 **USE_DESC_IDX** 힌트를 사용하여 내림차순 스캔을 사용하도록 명시할 수 있다. 이 힌트가 명시되지 않으면 **ORDER BY** 절에 나열된 칼럼이 인덱스를 사용할 수 있다는 전제 조건 하에서 아래의 3가지 질의 실행 계획을 고려할 수 있다.

*   순차 스캔 + 내림차순 정렬
*   일반적인 오름차순 스캔 + 내림차순 정렬
*   별도의 정렬 작업이 필요없는 내림차순 스캔

내림차순 스캔을 위해 **USE_DESC_IDX** 힌트가 생략된다 하더라도 질의 최적화기는 위에서 나열한 3가지 중 제일 마지막 실행 계획을 최적의 계획으로 결정한다.

.. note:: **USE_DESC_IDX** 힌트는 조인 질의에 대해서는 지원하지 않는다.

.. code-block:: sql

    CREATE TABLE di (i INT);
    CREATE INDEX i_di_i on di (i);
    INSERT INTO di VALUES (5),(3),(1),(4),(3),(5),(2),(5);

다음 예는 **USE_DESC_IDX** 힌트 없이 오름차순 스캔을 통해 질의를 수행한다.

.. code-block:: sql

    -- The query will be executed with an ascending scan. 
     
    SELECT  * 
    FROM di 
    WHERE i > 0 
    LIMIT 3;
     
    Query plan:
     
    Index scan(di di, i_di_i, (di.i range (0 gt_inf max) and inst_num() range (min inf_le 3)) (covers))
     
                i
    =============
                1
                2
                3

위의 질의에 **USE_DESC_IDX** 힌트를 추가하면 내림차순 스캔을 통해 다른 결과가 나온다.

.. code-block:: sql

    -- We now run the same query, using the 'use_desc_idx' SQL hint:
     
    SELECT /*+ USE_DESC_IDX */ * 
    FROM di 
    WHERE i > 0 
    LIMIT 3;
     
    Query plan:
     Index scan(di di, i_di_i, (di.i range (0 gt_inf max) and inst_num() range (min inf_le 3)) (covers) (desc_index))
     
                i
    =============
                5
                5
                5


다음 예는 **ORDER BY** 절을 통해 내림차순 정렬이 요구되는 경우이다. 이 경우 **USE_DESC_IDX** 힌트가 없지만 내림차순 스캔하게 된다.

.. code-block:: sql

    -- We also run the same query, this time asking that the results are displayed in descending order. 
    -- However, no hint is given. 
    -- Since ORDER BY...DESC clause exists, CUBRID will use descending scan, even though the hint is not given, 
    -- thus avoiding to sort the records.
     
    SELECT * 
    FROM di 
    WHERE i > 0 
    ORDER BY i DESC LIMIT 3;
     
    Query plan:
     Index scan(di di, i_di_i, (di.i range (0 gt_inf max)) (covers) (desc_index))
     
                i
    =============
                5
                5
                5

.. _group-by-skip-optimization:

GROUP BY 절 최적화
------------------

**GROUP BY** 절에 있는 모든 칼럼이 인덱스에 포함되어 질의 수행 시 인덱스를 사용할 수 있어 별도의 정렬 작업을 하지 않는 것을 **GROUP BY** 절 최적화라고 한다. 
이를 위해서는 **GROUP BY** 절에 있는 칼럼들이 인덱스를 구성하는 칼럼들의 제일 앞 쪽에 모두 존재해야 한다.

.. code-block:: sql

    SELECT * 
    FROM tab 
    WHERE col1 > 0 
    GROUP BY col1,col2;

*   *tab* (*col1*, *col2*)로 구성된 인덱스는 최적화에 사용할 수 있다.
*   *tab* (*col1*, *col2*, *col3*)로 구성된 인덱스도 사용될 수 있는데, **GROUP BY** 절에서 참조하지 않는 *col3* 는 *col1*, *col2* 뒤에 오기 때문이다.
*   *tab* (*col1*)로 구성된 인덱스는 최적화에 사용할 수 없다.
*   *tab* (*col3*, *col1*, *col2*) 혹은 *tab* (*col1*, *col3*, *col2*)로 구성된 인덱스도 최적화에 사용할 수 없는데, *col3* 가 **GROUP BY** 절의 칼럼들 뒤에 위치하지 않기 때문이다.

인덱스를 구성하는 칼럼이 **GROUP BY** 절에 없더라도 그 칼럼의 조건이 상수일 때는 인덱스를 사용할 수 있다.

.. code-block:: sql

    SELECT * 
    FROM tab 
    WHERE col2=val 
    GROUP BY col1,col3;

위의 예에서 *tab* (*col1*, *col2*, *col3*)로 구성된 인덱스가 있으면 이 인덱스를 **GROUP BY** 최적화에 사용한다.

이 경우에도 인덱스 스캔 시 요구하는 순서대로 결과를 가져오므로, **GROUP BY** 에 의해서 행에 대한 정렬이 불필요하게 된다.

**WHERE** 절이 없어도 **GROUP BY** 칼럼으로 구성된 인덱스가 있고 그 인덱스의 첫번째 칼럼이 **NOT NULL** 이면 **GROUP BY** 최적화가 적용된다.

집계 함수 사용 시 **GROUP BY** 최적화가 적용되는 경우는 **MIN** ()이나 **MAX** ()를 사용할 때뿐이며, 두 집계 함수가 같이 쓰이려면 같은 칼럼을 사용하는 경우에만 적용된다.

.. code-block:: sql

    CREATE INDEX i_T_a_b_c ON T(a, b, c);
    SELECT a, MIN(b), c, MAX(b) FROM T WHERE a > 18 GROUP BY a, b;

**예제**

.. code-block:: sql

    CREATE TABLE tab (i INT, j INT, k INT);
    CREATE INDEX i_tab_j_k ON tab (j, k);
    INSERT INTO tab VALUES (1,2,3), (6,4,2), (3,4,1), (5,2,1), (1,5,5), (2,6,6), (3,5,4);

다음의 예는 *j*, *k* 칼럼으로 **GROUP BY** 를 수행하므로 *tab* (*j*, *k*)로 구성된 인덱스가 사용되고 별도의 정렬 과정이 필요 없다.

.. code-block:: sql

    SELECT i,j,k 
    FROM tab 
    WHERE j > 0 
    GROUP BY j,k;
     
    --  the  selection from the query plan dump shows that the index i_tab_j_k was used and sorting was not necessary
    --  (/* ---> skip GROUP BY */)
     
    Query plan:
    iscan
        class: tab node[0]
        index: i_tab_j_k term[0]
        sort:  2 asc, 3 asc
        cost:  1 card 0
     
    Query stmt:
    select tab.i, tab.j, tab.k from tab tab where ((tab.j> ?:0 )) group by tab.j, tab.k
    /* ---> skip GROUP BY */
                i            j            k
                5            2            1
                1            2            3
                3            4            1
                6            4            2
                3            5            4
                1            5            5
                2            6            6

다음의 예는 *j*, *k* 칼럼으로 **GROUP BY** 를 수행하며 *j* 에 대한 조건이 없지만 *j* 칼럼은 **NOT NULL** 속성을 지니므로, *tab* (*j*, *k*)로 구성된 인덱스가 사용되고 별도의 정렬 과정이 필요 없다.

.. code-block:: sql

    ALTER TABLE tab CHANGE COLUMN j j INT NOT NULL;
    
    SELECT * 
    FROM tab 
    GROUP BY j,k;
     
    --  the  selection from the query plan dump shows that the index i_tab_j_k was used (since j has the NOT NULL constraint )
    --  and sorting was not necessary (/* ---> skip GROUP BY */)
    Query plan:
    iscan
        class: tab node[0]
        index: i_tab_j_k
        sort:  2 asc, 3 asc
        cost:  1 card 0
     
    Query stmt: select tab.i, tab.j, tab.k from tab tab group by tab.j, tab.k
    /* ---> skip GROUP BY */
    === <Result of SELECT Command in Line 1> ===
                i            j            k
    =======================================
                5            2            1
                1            2            3
                3            4            1
                6            4            2
                3            5            4
                1            5            5
                2            6            6

.. _multi-key-range-opt:

다중 키 범위 최적화
-------------------

대부분의 질의가 **LIMIT** 절을 포함하고 있기 때문에 **LIMIT** 절을 최적화하는 것이 질의 성능에 매우 중요한데, 이에 해당하는 대표적인 최적화가 다중 키 범위 최적화(multiple key range optimization)이다. 

다중 키 범위 최적화는 결과 생성에 필요한 인덱스 범위 전체를 스캔하지 않고, 인덱스 내의 일부 키 범위만 스캔하면서 Top N 정렬 방식을 통해 질의 결과를 생성한다. Top N 정렬은 전체 결과를 생성한 후에 이를 정렬하여 결과를 얻는 것이 아니라, 항상 최적의 N 개의 결과를 유지하는 방식으로 질의를 처리하기 때문에 매우 뛰어난 성능을 보인다.

예를 들어 내 친구들이 쓴 글 중에서 가장 최근 글을 10 개만 검색하는 경우, 다중 키 범위 최적화가 적용되면 내 전체 친구가 쓴 글을 모두 찾아서 정렬한 후에 결과를 찾지 않고 각 친구가 쓴 최근 글 10 개씩만을 찾아서 정렬을 유지하고 있는 인덱스를 스캔하여 결과를 찾는다.

다중 키 범위 최적화를 사용할 수 있는 예는 다음과 같다. 

.. code-block:: sql

    CREATE TABLE t (a int, b int); 
    CREATE INDEX i_t_a_b ON t (a,b);
    
    -- Multiple key range optimization
    SELECT * 
    FROM t 
    WHERE a IN (1,2,3) 
    ORDER BY b 
    LIMIT 2; 

    Query plan: 
    iscan 
    class: t node[0] 
    index: i_t_a_b term[0] (covers) (multi_range_opt) 
    sort: 1 asc, 2 asc 
    cost: 1 card 0 

단일 테이블에서는 다음과 같은 조건들이 만족되었을 경우에 다중 키 범위 최적화가 수행된다. 

::

    SELECT /*+ hints */ … 
    FROM table
    WHERE col_1 = ? AND col_2 = ? AND … AND col(j-1) = ?
    AND col_(j) IN (?, ?, … )
    AND col_(j+1) = ? AND … AND col_(p-1) = ?
    AND key_filter_terms
    ORDER BY col_(p) [ASC|DESC], col_(p+1) [ASC|DESC],… col_(p+k-1) [ASC|DESC]
    FOR orderbynum_pred | LIMIT n;

먼저 *orderbynum_pred* 조건이 명시되었다면 이 조건은 유효해야 하고, **ORDERBY_NUM** 또는 **LIMIT**\ 를 통해서 지정된 최종 결과의 상한 크기(*n*)이 **multi_range_optimization_limit** 시스템 파라미터 값보다 작거나 같아야 한다.

또한 다중 키 범위 최적화에 적합한 인덱스가 필요한데, **ORDER BY** 절에 명시된 모든 *k* 개의 칼럼을 커버해야 한다. 즉, 인덱스 상에서 **ORDER BY** 절에 명시된 칼럼들을 모두 포함해야 하고, 칼럼들의 순서와 정렬 방향이 일치해야 한다. 또한 **WHERE** 절에서 사용되는 모든 칼럼을 포함해야 한다.

인덱스를 구성하는 칼럼들 중 

*   범위 조건(예를 들어, IN 조건) 앞의 칼럼들은 동일(=) 조건으로 표현된다.
*   범위 조건을 가진 칼럼이 하나만 존재한다. 
*   범위 조건 이후의 칼럼들은 키 필터로 존재한다. 
*   데이터 필터 조건이 없어야 한다. 다시 말해, 인덱스는 **WHERE** 절에서 사용되는 모든 칼럼을 포함해야 한다.
*   키 필터 이후의 칼럼들은 **ORDER BY** 절에 존재한다. 
*   키 필터 조건의 칼럼들은 반드시 **ORDER BY** 절의 칼럼이 아니어야 한다.
*   상관 부질의(correlated subquery)를 포함한 키 필터 조건이 포함되어 있다면, 이에 연관된 칼럼은 범위 조건이 아닌 조건으로 WHERE 절에 포함되어야 한다. 

다음과 같은 예에 다중 키 범위 최적화가 수행된다. 

.. code-block:: sql

    CREATE TABLE t (a INT, b INT, c INT, d INT, e INT); 
    CREATE INDEX i_t_a_b ON t (a,b,c,d,e); 
    
    SELECT * 
    FROM t 
    WHERE a = 1 AND b = 3 AND c IN (1,2,3) AND d = 3 
    ORDER BY e 
    LIMIT 2; 

다중 테이블을 포함하는 JOIN 질의에서는 다음의 경우 최적화가 수행된다. 

::

    SELECT /*+ hints */ ...
    FROM table_1, table_2, ... table_(sort), ...
    WHERE col_1 = ? AND col_2 = ? AND ...
    AND col_(j) IN (?, ?, ... )
    AND col_(j+1) = ? AND ... AND col_(p-1) = ?
    AND key_filter_terms
    AND join_terms
    ORDER BY col_(p) [ASC|DESC], col_(p+1) [ASC|DESC], ... col_(p+k-1) [ASC|DESC]
    FOR ordbynum_pred | LIMIT n;

JOIN 질의에 대해서 다중 키 범위 최적화가 적용되기 위해서는 다음과 같은 조건을 만족해야 한다.

*    **ORDER BY** 절에 존재하는 칼럼들은 하나의 테이블에만 존재하는 칼럼들이며, 이 테이블은 단일 테이블 질의에서 다중 키 범위 최적화에 의해 요구되는 조건을 모두 만족해야 한다. 이 테이블을 정렬 테이블(sort table)이라고 하자. 
*   정렬 테이블과 외부 테이블들(outer tables) 간의 JOIN 조건에 명시된 정렬 테이블의 칼럼들은 모두 인덱스에 포함되어야 한다. 즉, 데이터 필터링 조건이 없어야 한다. 
*   정렬 테이블과 내부 테이블들(inner tables) 간의 JOIN 조건에 명시된 정렬 테이블의 칼럼들은 범위 조건이 아닌 조건으로 WHERE 절에 포함되어야 한다. 


.. note:: 다중 키 범위 최적화가 적용될 수 있는 대부분의 경우에 다중 키 범위 최적화가 가장 좋은 성능을 보장하지만, 특정한 상황에서 최적화를 원하지 않는다면 질의에 **NO_MULTI_RANGE_OPT** 힌트를 명시하면 된다. 힌트를 지정하는 방법은 :ref:`sql-hint`\ 를 참고하면 된다.

.. _index-skip-scan:

인덱스 스킵 스캔
----------------

인덱스 스킵 스캔(index skip scan, 이하 ISS)은 인덱스의 첫 번째 칼럼이 조건에 명시되지 않아도 뒤따라오는 칼럼이 조건(주로 =)에 명시되면 해당 인덱스를 활용하여 질의를 처리하는 최적화 방식이다. 
일반적으로 ISS는 여러 개의 칼럼들(C1, C2, …, Cn) 중에서 고려되어야 하는데, 여기에서 질의는 연속된 칼럼들에 대한 조건을 가지고 있고 이 조건들은 인덱스의 두 번째 칼럼(C2)부터 시작한다.

.. code-block:: sql

    INDEX (C1, C2, ..., Cn);
     
    SELECT ... WHERE C2 = x AND C3 = y AND ... AND Cp = z; -- p <= n
    SELECT ... WHERE C2 < x AND C3 >= y AND ... AND Cp BETWEEN (z AND w); -- other conditions than equal

질의 최적화기는 궁극적으로 비용에 따라 ISS가 최적의 접근 방식인지 비용을 감안하여 결정한다. ISS는 인덱스의 첫 번째 칼럼이 레코드 개수에 비해 구분되는(**DISTINCT**) 값의 개수가 적은 경우와 같이 특정한 상황에서 적용되며, 이 경우 인덱스 전체 검색(index full scan)보다 더 우수한 성능을 발휘한다. 예를 들어, 인덱스 칼럼 중에 첫 번째 칼럼이 남성/여성의 값 또는 수백만 건의 레코드가 1~100 사이의 값을 가지는 것처럼 매우 낮은 카디널리티(cardinality)를 가지고 있고(값의 중복도가 높고), 이 칼럼 조건이 질의 조건에 명시되지 않은 경우에 질의 최적화기는 ISS 적용을 검토하게 된다.

인덱스 전체 검색은 인덱스 리프 전체를 모두 다 읽어야 하지만, ISS는 동적으로 재조정되는 범위 검색(range search)을 사용하여 대부분의 인덱스 페이지 읽기를 생략하면서 질의를 처리한다. 값의 중복도가 높을수록 읽기를 생략할 수 있는 인덱스 페이지가 많아질 수 있기 때문에 ISS의 효율이 높아질 수 있다. 하지만 ISS가 많이 적용된다는 것은 인덱스 생성이 적절하지 않다는 것을 의미하기 때문에, DBA들은 인덱스 재조정이 필요하지 않은지 검토해볼 필요가 있다.

.. code-block:: sql

    CREATE TABLE t (name STRING, gender CHAR (1), birthday DATETIME);
     
    CREATE INDEX idx_t_gen_name ON t (gender, name);
    -- Note that gender can only have 2 values, 'M' and 'F' (low cardinality)
     
    -- this would qualify to use Index Skip Scanning:
    SELECT * 
    FROM t 
    WHERE name = 'SMITH';

다음과 같은 경우에는 ISS가 적용되지 않는다.

*   필터링된 인덱스
*   인덱스의 첫 번째 칼럼이 범위 필터나 키 필터인 경우
*   계층 질의
*   집계 함수가 포함된 경우
