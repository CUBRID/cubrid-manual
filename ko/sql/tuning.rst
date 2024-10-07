
:meta-keywords: cubrid update statistics, cubrid check statistics, query plan, query profiling, sql hint, cubrid index hint, cubrid special index, cubrid using index
:meta-description: How to optimize query execution in CUBRID database.


통계 정보 갱신
==============

테이블과 인덱스에 대한 통계 정보는 데이터베이스 시스템이 질의를 효과적으로 처리할 수 있게 한다. 통계 정보는 인덱스의 생성, 테이블의 생성등 DDL 문과 INSERT, DELETE 등 DML 문에 대해서 자동으로 갱신되지 않는다. **UPDATE STATISTICS** 문만이 통계정보를 갱신하는 유일한 방법이다. 필요한 경우 사용자가 직접 **UPDATE STATISTICS** 문을 수행하여 통계 정보를 갱신해야 한다(:ref:`info-stats` 참고)

**UPDATE STATISTICS** 문은 주기적으로 수행할 것을 권장한다. 또한 새로운 인덱스가 추가되거나, 대량의 **INSERT**, 혹은 **DELETE** 문이 수행되어 실제 정보와 통계 정보 사이에 차이가 커질 때 수행할 것을 권장한다.

::

    UPDATE STATISTICS ON [schema_name.]class-name [{, [schema_name.]class-name}] [WITH FULLSCAN]; 
     
    UPDATE STATISTICS ON ALL CLASSES [WITH FULLSCAN]; 
  
    UPDATE STATISTICS ON CATALOG CLASSES [WITH FULLSCAN]; 

*   **WITH FULLSCAN**: 지정된 테이블의 전체 데이터를 가지고 통계 정보를 업데이트한다. 생략 시 샘플링한 데이터를 가지고 통계 정보를 업데이트한다. 데이터 샘플은 테이블 전체 페이지 수와 상관없이 5000페이지이다.

*   **ALL CLASSES**: 모든 테이블의 통계 정보를 업데이트한다. 

*   **CATALOG CLASSES**: 카탈로그 테이블에 대한 통계 정보를 업데이트한다.

.. code-block:: sql 
  
    CREATE TABLE foo (a INT, b INT); 
    CREATE INDEX idx1 ON foo (a); 
    CREATE INDEX idx2 ON foo (b); 
  
    UPDATE STATISTICS ON foo; 
    UPDATE STATISTICS ON foo WITH FULLSCAN; 
  
    UPDATE STATISTICS ON ALL CLASSES; 
    UPDATE STATISTICS ON ALL CLASSES WITH FULLSCAN; 
  
    UPDATE STATISTICS ON CATALOG CLASSES; 
    UPDATE STATISTICS ON CATALOG CLASSES WITH FULLSCAN; 

통계 정보 갱신 시작과 종료 시 서버 에러 로그에 NOTIFICATION 메시지를 출력하며, 이를 통해 통계 정보 갱신에 걸리는 시간을 확인할 수 있다.

::
    
    Time: 05/07/13 15:06:25.052 - NOTIFICATION *** file ../../src/storage/statistics_sr.c, line 123  CODE = -1114 Tran = 1, CLIENT = testhost:csql(21060), EID = 4
    Started to update statistics (class "code", oid : 0|522|3).

    Time: 05/07/13 15:06:25.053 - NOTIFICATION *** file ../../src/storage/statistics_sr.c, line 330  CODE = -1115 Tran = 1, CLIENT = testhost:csql(21060), EID = 5
    Finished to update statistics (class "code", oid : 0|522|3, error code : 0).

.. note:: 

    CUBRID 10.0 부터는 HA 환경의 마스터에서 수행한 **UPDATE STATISTICS** 문이 슬레이브/레플리카에 복제된다.

.. note:: 

    CUBRID 11.3부터는 **UPDATE STATISTICS** 문을 실행할 때 동의어를 사용할 수 없다.

    .. code-block:: sql
    
        /* CURRENT_USER: PUBLIC */
        CREATE TABLE t (c int);
        CREATE SYNONYM s for t;

	UPDATE STATISTICS ON t;
        /* Execute OK. */

	UPDATE STATISTICS ON s;
	/* ERROR: before ' ; '
         * Class public.s does not exist. */

.. note::

    CUBRID 11.4 부터는  **UPDATE STATISTICS** 문을 실행할 때 **SELECT** 권한이 필요하다.

.. _info-stats:

통계 정보 확인
==============

CSQL 인터프리터의 세션 명령어로 지정한 테이블의 통계 정보를 확인한다.

::

    csql> ;info stats [schema_name.]table_name

*   *schema_name*: 테이블의 스키마를 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *table_name*: 통계 정보를 확인할 테이블 이름

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
     Class name: t1 Timestamp: Mon Mar 25 17:56:10 2024
     Total pages in class heap: 1
     Total objects: 5
     Number of attributes: 1
     Attribute: code (integer)
        Number of Distinct Values: 5
        B+tree statistics:
            BTID: { 1 , 832 }
            Cardinality: 5 (5) , Total pages: 3 , Leaf pages: 1 , Height: 2

*   *Number of Distinct Values*: 중복이 제거된 값의 개수이다. 옵티마이저에서 선택도를 산정하는데 사용된다.
*   *B+tree Cardinality*: 인덱스 key값의 누적된 중복이 제거된 값의 개수이다. 옵티마이저에서 최소 선택도로 사용된다.

.. _viewing-query-plan:

질의 실행 계획 보기
===================

CUBRID SQL 질의에 대한 실행 계획(query plan)을 보기 위해서는 다음의 방법을 사용할 수 있다.

*   CUBRID Manager에서 "플랜 보기" 버튼을 누른다.

    .. image:: /images/query_plan_on_CM.png

*   CSQL 인터프리터에서 ;plan simple 또는 ;plan detail 명령을 실행하거나 **SET OPTIMIZATION** 구문을 이용해서 최적화 수준(optimization level) 값을 변경시킨다. 현재의 최적화 수준 값은 **GET OPTIMIZATION** 구문으로 얻을 수 있다. CSQL 인터프리터에 대한 자세한 내용은 :ref:`csql-session-commands`\ 를 참고한다.

**SET OPTIMIZATION** 또는 **GET OPTIMIZATION LEVEL** 구문은 다음과 같다.

::

    SET OPTIMIZATION LEVEL opt-level [;]
    GET OPTIMIZATION LEVEL [ { TO | INTO } variable ] [;]

*   *opt-level* : 최적화 수준을 지정하는 값으로 다음과 같은 의미를 갖으며, 이 외에 값을 설정할 경우 에러를 발생한다.

    *   0 : 질의 최적화를 수행하지 않는다. 실행하는 질의는 가장 단순한 형태의 실행 계획을 가지고 실행된다. 디버깅의 용도 이외에는 사용되지 않는다.
    
    *   1 : 질의 최적화를 수행한다. CUBRID에서 사용되는 기본 설정 값으로 대부분의 경우 변경할 필요가 없다.
    
    *   2: 질의 최적화를 수행하여 실행 계획을 생성하나 질의 자체는 수행되지 않는다. 일반적으로 사용되지 않고 다음 질의 실행 계획 보기를 위한 설정값과 같이 설정되어 사용된다.
    
    *   257 : 질의 최적화를 수행하여 생성된 질의 실행 계획(플랜)을 출력한다. 256+1의 값으로 해석하여 값을 1로 설정하고 질의 실행 계획 출력을 지정한 것과 같다.
    
    *   258 : 질의 최적화를 수행하여 생성된 질의 실행 계획을 출력하나 질의를 수행하지는 않는다. 256+2의 값으로 해석하여 2로 설정하고 질의 실행 계획 출력을 지정한 것과 같다. 질의 실행 계획을 살펴보고자 하나 실행 결과에는 관심이 없을 경우 유용한 설정이다.
    
    *   513 : 질의 최적화를 수행하고 상세 질의 실행 계획을 출력한다. 512+1의 의미이다.
    
    *   514 : 질의 최적화를 수행하고 상세 질의 실행 계획을 출력하나 질의는 실행하지는 않는다. 512+2의 의미이다.

    .. note:: 2, 258, 514와 같이 질의를 실행하지 않게 최적화 수준을 설정하는 경우 SELECT 문 뿐만 아니라 INSERT, UPDATE, DELETE, REPLACE,  TRIGGER, SERIAL  문 등 모든 질의문이 실행되지 않는다.

CUBRID 질의 최적화기는 사용자에 의해 설정된 최적화 수준 값을 참조하여 최적화 여부와 질의 실행 계획의 출력 여부를 결정한다. 

다음은 CSQL에서 ";plan simple" 명령 입력 또는 "SET OPTIMIZATION LEVEL 257;"을 입력 후 질의를 수행한 결과이다.

.. code-block:: sql

    SET OPTIMIZATION LEVEL 257;
    --  csql> ;plan simple
    SELECT /*+ RECOMPILE */  DISTINCT h.host_year, o.host_nation
    FROM history h INNER JOIN olympic o 
    ON h.host_year = o.host_year AND o.host_year > 1950;

::    
     
    Query plan:

     Sort(distinct)
        Nested-loop join(h.host_year=o.host_year)
            Index scan(olympic o, pk_olympic_host_year, (o.host_year> ?:0 ))
            Sequential scan(history h)

*   Sort(distinct): DISTINCT를 수행한다.
*   Nested-loop join: 조인 방식이 Nested-loop이다.
*   Index scan: olympic 테이블에 대해 pk_olympic_host_year를 사용하여 index scan. 이때 인덱스를 사용한 조건은 "o.host_year> ?"이다.

CSQL에서 ";plan detail" 명령 입력 또는 "SET OPTIMIZATION LEVEL 513;"을 입력 후 질의를 수행하면 상세 내용을 출력한다.

.. code-block:: sql

    SET OPTIMIZATION LEVEL 513;
    --  csql> ;plan detail
    
    SELECT /*+ RECOMPILE */  DISTINCT h.host_year, o.host_nation
    FROM history h INNER JOIN olympic o 
    ON h.host_year = o.host_year AND o.host_year > 1950;

::

    Join graph segments (f indicates final):
    seg[0]: [0]
    seg[1]: host_year[0] (f)
    seg[2]: [1]
    seg[3]: host_nation[1] (f)
    seg[4]: host_year[1]
    Join graph nodes:
    node[0]: history h(147/1)
    node[1]: olympic o(25/1) (sargs 1)
    Join graph equivalence classes:
    eqclass[0]: host_year[0] host_year[1]
    Join graph edges:
    term[0]: h.host_year=o.host_year (sel 0.04) (join term) (mergeable) (inner-join) (indexable host_year[1]) (loc 0)
    Join graph terms:
    term[1]: o.host_year range (1950 gt_inf max) (sel 0.1) (rank 2) (sarg term) (not-join eligible) (indexable host_year[1]) (loc 0)

    Query plan:

    temp(distinct)
        subplan: nl-join (inner join)
                     edge:  term[0]
                     outer: iscan
                                class: o node[1]
                                index: pk_olympic_host_year term[1]
                                cost:  1 card 2
                     inner: sscan
                                class: h node[0]
                                sargs: term[0]
                                cost:  1 card 147
                     cost:  3 card 15
        cost:  9 card 15

    Query stmt:

    select distinct h.host_year, o.host_nation from history h, olympic o where h.host_year=o.host_year and (o.host_year> ?:0 )

위의 출력 결과에서 질의 계획과 관련하여 봐야 할 정보는 "Query plan:"이며, 가장 안쪽의 윗줄부터 순서대로 실행된다. 즉, outer: iscan -> inner:scan이 nl-join에서 반복 수행되고, 마지막으로 temp(distinct)가 수행된다. "Join graph segments"는 "Query plan:"에서 필요한 정보를 좀더 확인하는 용도로 사용한다. 예를 들어 "Query plan:"에서 "term[0]"는  "Join graph segments"에서 "term[0]: h.host_year=o.host_year (sel 0.04) (join term) (mergeable) (inner-join) (indexable host_year[1]) (loc 0)"로 표현됨을 확인할 수 있다.

위의 "Query plan:" 각 항목에 대한 설명은 다음과 같다.

*   temp(distinct): (distinct)는 DISTINCT를 실행함을 의미한다. temp는 실행 결과를 임시 공간에 저장했음을 의미한다.

    *   nl-join: "nl-join"은 조인 방식이 중첩 루프 조인(Nested loop join)임을 의미한다. 
    *   (inner join): 조인 종류가 "inner join"임을 의미한다. 
    
        *   outer: iscan: outer 테이블에서는 iscan(index scan)을 수행한다.
        
            *   class: o node[1]: o라는 테이블을 사용하며 상세 정보는 Join graph segments의 node[1]을 확인한다.
            *   index: pk_olympic_host_year term[1]: pk_olympic_host_year 인덱스를 사용하며 상세 정보는 Join graph segments의 term[1]을 확인한다.
            *   cost: 해당 구문을 수행하는데 드는 비용이다. 
            
                *   card: 카디널리티(cardinality)를 의미한다. 이 값은 근사치임에 유의한다.
                
        *   inner: sscan: inner 테이블에 sscan(sequential scan)을 수행한다.
        
            *   class: h node[0]: h라는 테이블을 사용하며 상세 정보는 Join graph segments의 node[0]을 확인한다.
            *   sargs: term[0]: sargs는 데이터 필터(인덱스를 사용하지 않는 WHERE 조건)를 나타내며, term[0]는 데이터 필터로 사용된 조건을 의미한다.
            *   cost: 해당 구문을 수행하는데 드는 비용이다.
            
                *   card: 카디널리티(cardinality)를 의미한다. 이 값은 근사치임에 유의한다.
                
    *   cost: 전체 구문을 수행하는데 드는 비용이다. 앞서 수행된 모든 비용을 포함한다.
    
        *   card: 카디널리티(cardinality)를 의미한다. 이 값은 근사치임에 유의한다.

**질의 계획 관련 용어**

다음은 질의 계획으로 출력되는 각 용어에 대한 의미를 정리한 것이다.

*   조인 방식: 질의 계획에서 출력되는 조인 방식은 위에서 "nl-join" 부분으로 다음과 같다.

    *   nl-join: 중첩 루프 조인, Nested loop join
    *   m-join: 정렬 병합 조인, Sort merge join
    *   idx_join: 중첩 루프 조인인데 outer 테이블의 행(row)을 읽으면서 inner 테이블에서 인덱스를 사용하는 조인
    
*   조인 종류: 위에서 (inner join) 부분으로, 질의 계획에서 출력되는 조인 종류는 다음과 같다.
    
    *   inner join
    *   left outer join
    *   right outer join: 질의 계획에서는 질의문의 "outer" 방향과 다른 방향이 출력될 수도 있다. 예를 들어, 질의문에서는 "right outer"로 지정했는데 질의 계획에는 "left outer"로 출력될 수도 있다.
    *   cross join

*   조인 테이블의 종류: 위에서 outer/inner 부분으로, 중첩 루프 조인에서 루프의 어느 쪽에 위치하는가를 기준으로 outer 테이블과 inner 테이블로 나뉜다.

    *   outer 테이블: 조인할 때 가장 처음에 읽을 기준 테이블
    *   inner 테이블: 조인할 때 나중에 읽을 대상 테이블

*   스캔 방식: 위에서 iscan/sscan 부분으로, 해당 질의가 인덱스를 사용하는지 여부를 판단할 수 있다.
    
    *   sscan: 순차 스캔(sequential scan). 풀 테이블 스캔(full table scan)이라고도 하며 인덱스를 사용하지 않고 테이블 전체를 스캔한다.
    *   iscan: 인덱스 스캔(index scan). 인덱스를 사용하여 스캔할 데이터의 범위를 한정한다.
    
*   cost: CPU, IO 등 주로 리소스의 사용과 관련하여 비용을 내부적으로 산정한다. 

*   card: 카디널리티(cardinality)를 의미하며, 선택될 것으로 예측되는 행의 개수이다.
    
다음은 USE_MERGE 힌트를 명시하여 m-join(정렬 병합 조인, sort merge join)이 적용되는 경우의 예이다. 일반적으로 정렬 병합 조인은 outer 테이블과 inner 테이블을 정렬하여 병합하는 것이 인덱스를 사용하여 중첩 루프 조인(nested loop join)을 수행하는 것보다 유리하다고 판단될 때만 사용해야 하며, 조인되는 두 테이블 모두 행의 개수가 매우 많은 경우 유리할 수 있다. 대부분의 경우 정렬 병합 조인을 수행하지 않는 것이 바람직하다.

.. note::

    9.3 버전부터 질의문에 USE_MERGE 힌트를 명시하거나 cubrid.conf의 **optimizer_enable_merge_join** 값을 yes로 설정해야 정렬 병합 조인의 적용이 고려된다.

.. CUBRIDSUS-13186: merge join은 스펙아웃될 예정임. 현재는 USE_MERGE 힌트를 제공해야만 적용됨.

.. code-block:: sql

    SET OPTIMIZATION LEVEL 513;
    -- csql> ;plan detail

    SELECT /*+ RECOMPILE USE_MERGE*/  DISTINCT h.host_year, o.host_nation
    FROM history h LEFT OUTER JOIN olympic o ON h.host_year = o.host_year AND o.host_year > 1950;
    
::

    Query plan:
    
    temp(distinct)
        subplan: temp
                     order: host_year[0]
                     subplan: m-join (left outer join)
                                  edge:  term[0]
                                  outer: temp
                                             order: host_year[0]
                                             subplan: sscan
                                                          class: h node[0]
                                                          cost:  1 card 147
                                             cost:  10 card 147
                                  inner: temp
                                             order: host_year[1]
                                             subplan: iscan
                                                          class: o node[1]
                                                          index: pk_olympic_host_year term[1]
                                                          cost:  1 card 2
                                             cost:  7 card 2
                                  cost:  18 card 147
                     cost:  24 card 147
        cost:  30 card 147

다음은 idx-join(인덱스 조인, index join)을 수행하는 경우의 예이다. inner 테이블의 조인 조건 칼럼에 인덱스가 있는 경우 inner 테이블의 인덱스를 사용하여 조인을 수행하는 것이 유리하다고 판단되면 **USE_IDX** 힌트를 명시하여 idx-join의 실행을 보장할 수 있다.

.. code-block:: sql

    SET OPTIMIZATION LEVEL 513;
    -- csql> ;plan detail

    CREATE INDEX i_history_host_year ON history(host_year);
    
    SELECT /*+ RECOMPILE */  DISTINCT h.host_year, o.host_nation
    FROM history h INNER JOIN olympic o ON h.host_year = o.host_year;

::

    Query plan:

    temp(distinct)
        subplan: idx-join (inner join)
                     outer: sscan
                                class: o node[1]
                                cost:  1 card 25
                     inner: iscan
                                class: h node[0]
                                index: i_history_host_year term[0] (covers)
                                cost:  1 card 147
                     cost:  2 card 147
        cost:  9 card 147

위의 질의 계획에서 "inner: iscan"의 "index: i_history_host_year term[0]"에 "(covers)"가 출력되는데, 이는 :ref:`covering-index` 기능이 적용된다는 의미이다. 즉, inner 테이블에서 인덱스 내에 필요한 데이터가 있어서 데이터 저장소를 추가로 검색할 필요가 없게 된다.

조인 테이블 중 왼쪽 테이블이 오른쪽 테이블보다 행의 개수가 훨씬 작음을 확신할 때 **ORDERED** 힌트를 명시하여 왼쪽 테이블을 outer 테이블로, 오른쪽 테이블을 inner 테이블로 지정할 수 있다.

.. code-block:: sql

    SELECT /*+ RECOMPILE ORDERED */  DISTINCT h.host_year, o.host_nation
    FROM history h INNER JOIN olympic o ON h.host_year = o.host_year;

.. _query-profiling:
 
질의 프로파일링
===============
 
SQL에 대한 성능 분석을 위해서는 질의 프로파일링(profiling) 기능을 사용할 수 있다. 
질의 프로파일링을 위해서는 **SET TRACE ON** 구문으로 SQL 트레이스를 설정해야 하며, 프로파일링 결과를 출력하려면 **SHOW TRACE** 구문을 수행해야 한다.
 
또한 **SHOW TRACE** 결과 출력 시 질의 실행 계획을 항상 포함하려면 /\*+ RECOMPILE \*/ 힌트를 추가해야 한다.

**SET TRACE ON** 구문의 형식은 다음과 같다.
 
::
 
    SET TRACE {ON | OFF} [OUTPUT {TEXT | JSON}]
 
*   ON: SQL 트레이스를 on한다.
*   OFF: SQL 트레이스를 off한다.
*   OUTPUT TEXT: 일반 TEXT 형식으로 출력한다. OUTPUT 이하 절을 생략하면 TEXT 형식으로 출력한다.
*   OUTPUT JSON: JSON 형식으로 출력한다.
    
아래와 같이 **SHOW TRACE** 구문을 실행하면 SQL을 트레이스한 결과를 문자열로 출력한다.
 
::

    SHOW TRACE;
    
다음은 SQL 트레이스를 ON으로 설정하고 질의를 수행한 후, 해당 질의에 대해 트레이스 결과를 출력하는 예이다.

::

    csql> SET TRACE ON;
    csql> SELECT /*+ RECOMPILE */ o.host_year, o.host_nation, o.host_city, SUM(p.gold) 
            FROM OLYMPIC o, PARTICIPANT p  
            WHERE o.host_year = p.host_year AND p.gold > 20
            GROUP BY o.host_nation;
    csql> SHOW TRACE;

::

    === <Result of SELECT Command in Line 2> ===

      trace
    ======================
      '
    Query Plan:
      SORT (group by)
        NESTED LOOPS (inner join)
          TABLE SCAN (o)
          INDEX SCAN (p.fk_participant_host_year) (key range: o.host_year=p.host_year)

      rewritten query: select o.host_year, o.host_nation, o.host_city, sum(p.gold) from OLYMPIC o, PARTICIPANT p where o.host_year=p.host_year and (p.gold> ?:0 ) group by o.host_nation

    Trace Statistics:
      SELECT (time: 2, fetch: 975, fetch_time: 1, ioread: 2)
        SCAN (table: olympic), (heap time: 0, fetch: 26, ioread: 0, readrows: 25, rows: 25)
          SCAN (index: participant.fk_participant_host_year), (btree time: 1, fetch: 941, ioread: 2, readkeys: 5, filteredkeys: 5, rows: 916) (lookup time: 0, rows: 14)
        GROUPBY (time: 0, sort: true, page: 0, ioread: 0, rows: 5)
    ' 

위에서 "Trace Statistics:" 이하가 트레이스 결과를 출력한 것이며 트레이스 결과의 각 항목을 설명하면 다음과 같다.

*   **SELECT** (time: 2, fetch: 975, fetch_time: 1, ioread: 2)
    
    *   time: 2 => 전체 질의 시간 2ms 소요.
    *   fetch: 975 => 페이지에 대해 975회 fetch(개수가 아닌 접근 회수임. 같은 페이지를 다시 fetch하더라도 회수가 증가함). 
    *   fetch_time: 1 => fetch 시간 1ms 소요.
    *   ioread: 2회 디스크 접근.

    : SELECT 질의에 대한 전체 통계이다. fetch 회수와 ioread 회수는 질의를 재실행하면 질의 결과의 일부를 버퍼에서 가져오게 되면서 줄어들 수 있다.
       
    *   **SCAN** (table: olympic), (heap time: 0, fetch: 26, ioread: 0, readrows: 25, rows: 25)
        
        *   heap time: 0 => 소요 시간은 1ms 미만. millisecond보다 작은 값은 버림하기 때문에 1ms 미만의 소요 시간은 0으로 표시된다.
        *   fetch: 26 => 페이지를 fetch한 회수는 26건.
        *   ioread: 0 => 디스크에 접근한 회수는 0.
        *   readrows: 25 => 스캔 시 읽은 행의 개수는 25.
        *   rows: 25 => 결과 행의 개수는 25.
        
        : olympic 테이블에 대한 힙 스캔 통계이다.
    
        *   **SCAN** (index: participant.fk_participant_host_year), (btree time: 1, fetch: 941, ioread: 2, readkeys: 5, filteredkeys: 5, rows: 916) (lookup time: 0, rows: 14)
            
            *   btree time: 1 => 소요 시간은 1ms.
            *   fetch: 941 => 페이지를 fetch한 회수는 941. 
            *   ioread: 2 => 디스크에 접근한 회수는 2회.
            *   readkeys: 5 => 읽은 키의 개수는 5.
            *   filteredkeys: 5 => 키 필터가 적용된 키의 개수는 5.
            *   rows: 916 => 스캔한 행 개수는 916.
            *   lookup time: 0 => 인덱스 스캔 후 데이터에 접근하는데 소요된 시간은 1ms 미만.
            *   rows: 14 => 데이터 필터까지 적용한 이후의 행 개수로, 이 질의문에서는 데이터 필터인 "p.gold > 20"을 적용했을 때 행의 개수는 14.

            : participant.fk_participant_host_year 인덱스에 대한 인덱스 스캔 통계이다.

    *   **GROUPBY** (time: 0, sort: true, page: 0, ioread: 0, rows: 5)
        
        *   time: 0 => group by 적용 시 소요된 시간은 1ms 미만.
        *   sort: true => 정렬이 적용되므로 true.
        *   page: 0 => 정렬에 사용된 임시 페이지 개수가 0.
        *   ioread: 0 => 디스크 접근에 소요된 시간은 1ms 미만.
        *   rows: 5 => group by에 대한 결과 행의 개수는 5개.

        : group by에 대한 통계이다.

다음은 3개의 테이블을 조인한 예이다.

::
 
    csql> SET TRACE ON;
    csql> SELECT /*+ RECOMPILE ORDERED */ o.host_year, o.host_nation, o.host_city, n.name, SUM(p.gold), SUM(p.silver), SUM(p.bronze)
            FROM OLYMPIC o,
                 (select /*+ NO_MERGE */ * from PARTICIPANT p where p.gold > 10) p,
                 NATION n
          WHERE o.host_year = p.host_year AND p.nation_code = n.code
          GROUP BY o.host_nation;
    csql> SHOW TRACE;
 
      trace
    ======================
    '
    Query Plan:
      TABLE SCAN (p)
    
      rewritten query: (select p.host_year, p.nation_code, p.gold, p.silver, p.bronze from PARTICIPANT p where (p.gold> ?:0 ))
    
      SORT (group by)
        NESTED LOOPS (inner join)
          NESTED LOOPS (inner join)
            TABLE SCAN (o)
            TABLE SCAN (p)
          INDEX SCAN (n.pk_nation_code) (key range: p.nation_code=n.code)
    
      rewritten query: select /*+ ORDERED */ o.host_year, o.host_nation, o.host_city, n.[name], sum(p.gold), sum(p.silver), sum(p.bronze) from OLYMPIC o, (select p.host_year, p.nation_code, p.gold, p.silver, p.bronze from PARTICIPANT p where (p.gold> ?:0 )) p (host_year, nation_code, gold,
    silver, bronze), NATION n where o.host_year=p.host_year and p.nation_code=n.code group by o.host_nation
    
    
    Trace Statistics:
      SELECT (time: 6, fetch: 880, fetch_time: 2, ioread: 0)
        SCAN (table: olympic), (heap time: 0, fetch: 104, ioread: 0, readrows: 25, rows: 25)
          SCAN (hash temp(m), buildtime : 0, time: 0, fetch: 0, ioread: 0, readrows: 76, rows: 38)
            SCAN (index: nation.pk_nation_code), (btree time: 2, fetch: 760, ioread: 0, readkeys: 38, filteredkeys: 0, rows: 38) (lookup time: 0, rows: 38)
        GROUPBY (time: 0, hash: true, sort: true, page: 0, ioread: 0, rows: 5)
        SUBQUERY (uncorrelated)
          SELECT (time: 2, fetch: 12, ioread: 0)
            SCAN (table: participant), (heap time: 2, fetch: 12, ioread: 0, readrows: 916, rows: 38)
    '

다음은 트레이스 항목에 대한 설명이다.

**SELECT**
 
*   time: 해당 질의에 대한 전체 수행 시간(ms)
*   fetch: 해당 질의에 대해 페이지를 fetch한 회수
*   fetch_time : 해당 질의에 대해 페이지 fetch 수행 시간(ms)
*   ioread: 해당 질의에 대한 전체 I/O 읽기 회수. 데이터를 읽을 때 물리적으로 디스크에 접근한 회수

**SCAN**

*   heap: 인덱스 없이 데이터를 스캔하는 작업

    *   time, fetch, ioread: heap에서 해당 연산 수행 시 소요된 시간(ms), fetch 회수, I/O 읽기 회수
    *   readrows: 해당 연산 수행 시 읽은 행의 개수
    *   rows: 해당 연산에 대한 결과 행의 개수
    
*   btree: 인덱스 스캔하는 작업

    *   time, fetch, ioread: btree에서 해당 연산 수행 시 소요된 시간(ms), fetch 회수, I/O 읽기 회수
    *   readkeys: btree에서 해당 연산 수행 시 읽은 키의 개수
    *   filteredkeys: 읽은 키 중에 키 필터가 적용된 키의 개수
    *   rows: 해당 연산에 대한 결과 행의 개수로, 키 필터가 적용된 결과 행의 개수

*   temp: 템프 파일에서 데이터를 스캔하는 작업

    *   hash temp(m): 해시 리스트 스캔 사용 여부. 데이터 양에 따라서, IN-MEMORY(m), HYBRID(h), FILE(f) 해시 자료구조를 사용한다.
    *   buildtime: 해시 테이블 빌드 수행 시 소요된 시간(ms)
    *   time: 해시 테이블 조사 수행 시 소요된 시간(ms)
    *   fetch, ioread: temp file에서 해당 연산 수행 시 소요된 fetch 회수, I/O 읽기 회수
    *   readrows: 해당 연산 수행 시 읽은 행의 개수
    *   rows: 해당 연산에 대한 결과 행의 개수

*   lookup: 인덱스 스캔 후 데이터에 접근하는 작업

    *   time: 해당 연산 수행 시 소요된 시간(ms)
    *   rows: 해당 연산에 대한 결과 행의 개수로, 데이터 필터가 적용된 결과 행의 개수

*   noscan: aggregate 연산 시 스캔 없이 인덱스 헤더의 통계 정보를 이용하는 작업 (aggregate: count, min, max)

    *   agl: aggregate lookup을 의미하며, aggreate 연산을 위해 사용된 인덱스 리스트

        다음은 noscan, agl에대한 예제이다.

::

        SET TRACE ON;
        CREATE TABLE agl_tbl (id INTEGER PRIMARY KEY, phone VARCHAR(20));
        INSERT INTO agl_tbl VALUES (1, '123-456-789');
        INSERT INTO agl_tbl VALUES (999, '999-999-999');

        SELECT count(*), min(id), max(id) FROM agl_tbl;

        SHOW TRACE;

::

        Trace Statistics:
          SELECT (time: 0, fetch: 16, fetch_time: 0, ioread: 0)
            SCAN (table: agl_tbl), (noscan time: 0, fetch: 0, ioread: 0, readrows: 0, rows: 0, agl: pk_agl_tbl_id)

**GROUPBY**    

*   time: 해당 연산 수행 시 소요된 시간(ms)
*   sort: 정렬 여부
*   page: 정렬에 사용된 임시 페이지 개수로, 내부 정렬 버퍼 외에 사용한 페이지 개수.
*   rows: 해당 연산에 대한 결과 행의 개수
*   hash: 집계 함수에서 투플 정렬 시 해시 집계 방식 적용 여부(true/false). :ref:`NO_HASH_AGGREGATE <no-hash-aggregate>` 힌트를 참고한다.

**INDEX SCAN**

*   key range: 키의 범위
*   covered: 커버링 인덱스 적용 여부(true/false)
*   loose: loose index scan 적용 여부(true/false)

위의 예는 JSON 형식으로도 출력할 수 있다.
 
::
 
    csql> SET TRACE ON OUTPUT JSON;
    csql> SELECT n.name, a.name FROM athlete a, nation n WHERE n.code=a.nation_code;
    csql> SHOW TRACE;
    
      trace
    ======================
      '{
      "Trace Statistics": {
        "SELECT": {
          "time": 29,
          "fetch": 5836,
          "ioread": 3,
          "SCAN": {
            "access": "temp",
            "temp": {
              "time": 5,
              "fetch": 34,
              "ioread": 0,
              "readrows": 6677,
              "rows": 6677
            }
          },
          "MERGELIST": {
            "outer": {
              "SELECT": {
                "time": 0,
                "fetch": 2,
                "ioread": 0,
                "SCAN": {
                  "access": "table (nation)",
                  "heap": {
                    "time": 0,
                    "fetch": 1,
                    "ioread": 0,
                    "readrows": 215,
                    "rows": 215
                  }
                },
                "ORDERBY": {
                  "time": 0,
                  "sort": true,
                  "page": 21,
                  "ioread": 3
                }
              }
            }
          }
        }
      }
    }'

CSQL 인터프리터에서 트레이스를 자동으로 설정하는 명령을 사용하면 **SHOW TRACE;** 구문을 별도로 실행하지 않아도 질의 실행 결과를 출력한 후 자동으로 트레이스 결과를 출력한다.

CSQL 인터프리터에서 트레이스를 자동으로 설정하는 방법은 :ref:`SQL 트레이스 설정 <set-autotrace>`\ 을 참고한다.

.. note::

    *   독립 모드(-S 옵션 사용)로 실행한 CSQL 인터프리터는 SQL 트레이스 기능을 지원하지 않는다.

    *   여러 개의 SQL을 한 번에 처리하는 경우(batch query, array query) 질의는 프로파일링되지 않는다.

.. _sql-hint:

SQL 힌트
========

사용자는 질의문에 힌트를 주어 해당 질의 성능을 높일 수 있다. 질의 최적화기는 질의문에 대한 최적화 작업을 수행할 때 SQL 힌트를 참고하여 효율적인 실행 계획을 생성한다. CUBRID에서 지원하는 SQL 힌트는 테이블 조인 관련 힌트, 인덱스 관련 힌트가 있다. 

::

    { SELECT | UPDATE | DELETE } /*+ <hint> [ { <hint> } ... ] */ ...;

    MERGE /*+ <merge_statement_hint> [ { <merge_statement_hint> } ... ] */ INTO ...;
    
    <hint> ::=
    USE_NL [ (<spec_name_comma_list>) ] |
    USE_IDX [ (<spec_name_comma_list>) ] |
    USE_MERGE [ (<spec_name_comma_list>) ] |
    ORDERED |
    LEADING |
    USE_DESC_IDX |
    USE_SBR |
    INDEX_SS [ (<spec_name_comma_list>) ] |
    INDEX_LS |
    NO_DESC_IDX |
    NO_COVERING_IDX |
    NO_MULTI_RANGE_OPT |
    NO_SORT_LIMIT |
    NO_SUBQUERY_CACHE |
    NO_PUSH_PRED |
    NO_MERGE |
    NO_ELIMINATE_JOIN |
    NO_HASH_AGGREGATE |
    NO_HASH_LIST_SCAN |
    NO_LOGGING |
    RECOMPILE

    <spec_name_comma_list> ::= <spec_name> [, <spec_name>, ... ]
        <spec_name> ::= [schema_name.]table_name | [schema_name.]view_name
    
    <merge_statement_hint> ::=
    USE_UPDATE_INDEX (<update_index_list>) |
    USE_DELETE_INDEX (<insert_index_list>) |
    RECOMPILE |
    QUERY_CACHE

SQL 힌트는 주석에 더하기 기호(+)를 함께 사용하여 지정한다. 힌트를 사용하는 방법은 :doc:`comment` 절에 소개된 바와 같이 세 가지 방식이 있다. 따라서 SQL 힌트도 다음과 같이 세 가지 방식으로 사용할 수 있다.

*  /\*+ hint \*/
*   \-\-+ hint
*   //+ hint

힌트 주석은 반드시 키워드 **SELECT**, **UPDATE** or **DELETE** 등의 예약어 다음에 나타나야 하고, 더하기 기호(+)가 주석에서 첫 번째 문자로 시작되어야 한다. 여러 개의 힌트를 지정할 때는 공백이 구분자로 사용된다. 여러 개의 힌트를 지정할 때는 공백이 구분자로 사용된다.

**SELECT**, **UPDATE**, **DELETE** 문에는 다음 힌트가 지정될 수 있다.

*   **USE_NL**: 테이블 조인과 관련한 힌트로서, 질의 최적화기 중첩 루프 조인 실행 계획을 만든다.
*   **USE_MERGE**: 테이블 조인과 관련한 힌트로서, 질의 최적화기는 정렬 병합 조인 실행 계획을 만든다.
*   **ORDERED**: 테이블 조인과 관련한 힌트로서, 질의 최적화기는 **FROM** 절에 명시된 테이블의 순서대로 조인하는 실행 계획을 만든다. **FROM** 절에서 왼쪽 테이블은 조인의 외부 테이블이 되고, 오른쪽 테이블은 내부 테이블이 된다.
*   **LEADING**: 테이블 조인과 관련한 힌트로서, 질의 최적화기는 LEADING 힌트에 명시된 테이블의 순서대로 조인하는 실행 계획을 만든다.

*   **USE_IDX**: 인덱스 관련한 힌트로서, 질의 최적화기는 명시된 테이블에 대해 인덱스 조인 실행 계획을 만든다.
*   **USE_DESC_IDX**: 내림차순 스캔을 위한 힌트이다. 자세한 내용은 :ref:`index-descending-scan`\을 참고한다.
*   **USE_SBR**: 구문 기반 복제(statement-based replication)를 위한 힌트로서, 기본키가 설정되지 않은 테이블에 대한 데이터 복제도 지원한다.

    .. note::

        슬레이브 노드에서 트랜잭션 로그가 반영되는 시점에 해당 구문을 다시 실행하기 때문에 노드 간 반영된 데이터의 불일치가 발생할 수 있다.

*   **INDEX_SS**: index skip scan 실행 계획을 고려한다. 자세한 내용은 :ref:`index-skip-scan`\을 참고한다.
*   **INDEX_LS**: loose index scan 실행 계획을 고려한다. 자세한 내용은 :ref:`loose-index-scan`\을 참고한다.
*   **NO_DESC_IDX**: 내림차순 스캔을 사용하지 않도록 하는 힌트이다.
*   **NO_COVERING_IDX**: 커버링 인덱스 기능을 사용하지 않도록 하는 힌트이다. 자세한 내용은 :ref:`covering-index`\를 참고한다.
*   **NO_MULTI_RANGE_OPT**: 다중 키 범위 최적화 기능을 사용하지 않도록 하는 힌트이다. 자세한 내용은 :ref:`multi-key-range-opt`\를 참고한다.
*   **NO_SORT_LIMIT**: SORT-LIMIT 최적화를 사용하지 않기 위한 힌트이다. 자세한 내용은 :ref:`sort-limit-optimization`\를 참고한다.
*   **NO_SUBQUERY_CACHE**: SUBQUERY_CACHE 최적화를 사용하지 않기 위한 힌트이다. 자세한 내용은 :ref:`correlated-subquery-cache`\를 참고한다.
*   **NO_PUSH_PRED**: PREDICATE-PUSH 최적화를 사용하지 않기 위한 힌트이다.
*   **NO_MERGE**: VIEW-MERGE 최적화를 사용하지 않기 위한 힌트이다.
*   **NO_ELIMINATE_JOIN**: 조인 제거 최적화를 사용하지 않기 위한 힌트이다. 자세한 내용은 :ref:`join-elimination-optimization`\를 참고한다.

.. _no-hash-aggregate:

*   **NO_HASH_AGGREGATE**: 집계 함수에서 투플을 정렬할 때 해싱을 사용하지 않도록 하는 힌트이다. 그 대신, 외부 정렬(external sorting)이 집계 함수에서 사용된다. 인-메모리(in-memory) 해시 테이블을 사용하여, CUBRID는 정렬할 때 필요로 하는 데이터의 양을 줄이거나 심지어는 제거할 수 있다. 그러나, 어떤 경우에는 해시 집계 방식이 실패할 것이라는 것을 미리 알고 전체적으로 해시 집계 과정을 생략하기 위해 이 힌트를 사용할 수 있다. 해시 집계 방식의 메모리 크기를 설정하기 위해서는 :ref:`max_agg_hash_size <max_agg_hash_size>`\ 를 참고한다.

    .. note::
    
        DISTINCT한 값을 계산하는 함수들(예. AVG(DISTINCT x))과 GROUP_CONCAT, MEDIAN 함수들은 각 그룹의 투플들에 대해 외부 정렬(external sorting) 과정을 요구하므로 해시 집계 방식이 동작하지 않을 것이다.

.. _no-hash-list-scan:

*   **NO_HASH_LIST_SCAN**: 부질의 스캔 시 해시 리스트 스캔을 사용하지 않도록 하는 힌트이다. 그 대신, 템프 파일 스캔을 위해 리스트 스캔이 사용된다. 해시 테이블을 빌드 및 조사 함으로써, CUBRID는 조회할 때 필요로 하는 데이터의 양을 줄일 수 있다. 그러나, 어떤 경우에는 외부 데이터양이 매우 적다는 것을 미리 알고 전체적으로 해시 리스트 스캔 과정을 생략하기 위해 이 힌트를 사용할 수 있다. 해시 리스트 스캔의 메모리 크기를 설정하기 위해서는 :ref:`max_hash_list_scan_size <max_hash_list_scan_size>`\ 를 참고한다.

    .. note::
    
        해시 리스트 스캔은 오직 동등 연산자를 가지고 있는 조회조건에서 동작하며, OID 타입을 가지고 있는 조회 조건에서는 동작하지 않는다.

*   **NO_LOGGING**: 테이블에 레코드 삽입, 갱신, 삭제 시 생성되는 로그에 리두(redo)가 포함되지 않도록 하는 힌트이다.

    .. note::

        현재 레코드 삽입, 갱신, 삭제 시 힙 파일에서 생성되는 로그에만 영향을 준다. 따라서 복구 후 테이블과 인덱스의 데이터가 불일치하는 문제, 커밋된 레코드가 복구되지 않는 문제 등이 발생할 수 있다. 반드시 주의하여 사용하여야 한다.

.. _recompile:

*   **RECOMPILE** : 질의 실행 계획을 리컴파일한다. 캐시에 저장된 기존 질의 실행 계획을 삭제하고 새로운 질의 실행 계획을 수립하기 위해 이 힌트를 사용한다.

*   **QUERY_CACHE** : 질의와 그 결과를 캐시한다. 이 힌트는 **SELECT** 질의에서만 사용할 수 있으며, 자세한 내용은 :ref:`query-cache` 를 참고한다.

.. note::

    <*spec_name*>\ 이 **USE_NL**, **USE_IDX**, **USE_MERGE**\ 와 함께 지정될 경우 해당 조인 방법은 <*spec_name*>\ 에 대해서만 적용된다. 

    .. code-block:: sql
    
        SELECT /*+ ORDERED USE_NL(b) USE_NL(c) USE_MERGE(d) */ * 
        FROM a INNER JOIN b ON a.col=b.col 
        INNER JOIN c ON b.col=c.col INNER JOIN d ON c.col=d.col;
        
    위와 같은 질의를 수행한다면 테이블 a와 b가 조인될 때는 **USE_NL**\ 이 적용되고 테이블 c가 조인될 때도 **USE_NL**\ 이 적용되며, 테이블 d가 조인될 때는 **USE_MERGE**\ 가 적용된다.

    <*spec_name*>\ 이 주어지지 않고 **USE_NL**\ 과 **USE_MERGE**\ 가 함께 지정된 경우 주어진 힌트는 무시된다. 일부 경우에 질의 최적화기는 주어진 힌트에 따라 질의 실행 계획을 만들지 못할 수 있다. 예를 들어 오른쪽 외부 조인에 대해 **USE_NL**\ 을 지정한 경우 이 질의는 내부적으로 왼쪽 외부 조인 질의로 변환이 되어 조인 순서는 보장되지 않을 수 있다.

.. note::

    **ORDERED**\가 **LEADING**\과 함께 지정될 경우 **LEADING** 힌트는 무시된다.
    **LEADING** 힌트가 여러게 지정될 경우 첫번째 **LEADING** 힌트만 적용된다.

    .. code-block:: sql

        SELECT /*+ ORDERED LEADING(b, d) */ *
        FROM a INNER JOIN b ON a.col=b.col
        INNER JOIN c ON b.col=c.col INNER JOIN d ON c.col=d.col;

    위와 같은 질의를 수행한다면 **LEADING** 힌트는 무시되며, **ORDERED** 힌트에 따라서 **FROM** 절의 순서인, 테이블 a, b, c, d의 순서로 조인된다.

    .. code-block:: sql

        SELECT /*+ LEADING(b, d) LEADING(c, d) */ *
        FROM a INNER JOIN b ON a.col=b.col
        INNER JOIN c ON b.col=c.col INNER JOIN d ON c.col=d.col;

    위와 같은 질의를 수행한다면 두번째 **LEADING** 힌트는 무시되며, 테이블 b와 d가 첫번째로 조인되는 조인순서가 생성된다.

MERGE 문에는 다음과 같은 힌트를 사용할 수 있다. 

*   **USE_INSERT_IDX** (<*insert_index_list*>): MERGE 문의 INSERT 절에서 사용되는 인덱스 힌트. *insert_index_list*\ 에 INSERT 절을 수행할 때 사용할 인덱스 이름을 나열한다. MERGE 문의 <*join_condition*>에 해당 힌트가 적용된다.
*   **USE_UPDATE_IDX** (<*update_index_list*>): MERGE 문의 UPDATE 절에서 사용되는 인덱스 힌트. *update_index_list*\ 에 UPDATE 절을 수행할 때 사용할 인덱스 이름을 나열한다. MERGE 문의 <*join_condition*>과 <*update_condition*>에 해당 힌트가 적용된다.
*   **RECOMPILE**: 위의 :ref:`RECOMPILE <recompile>`\ 을 참고한다.

조인 시 사용하는 힌트의 경우 힌트 안에 조인할 테이블이나 뷰 이름을 명시할 수 있는데, 이때 테이블 이름/뷰 이름은 ","로 구분한다.

.. code-block:: sql

    SELECT /*+ USE_NL(a, b) */ * 
    FROM a INNER JOIN b ON a.col=b.col;

다음은 *'심권호'* 선수가 메달을 획득한 연도와 메달 종류를 구하는 예제이다. 다음과 같은 질의로 표현이 되는데, 질의최적화기는 *athlete* 테이블을 외부 테이블로 하고, *game* 테이블을 내부 테이블로 하는 중첩 루프 조인 실행 계획을 만든다.

.. code-block:: sql

    -- csql> ;plan_detail
    
    SELECT /*+ USE_NL ORDERED  */ a.name, b.host_year, b.medal
    FROM athlete a, game b 
    WHERE a.name = 'Sim Kwon Ho' AND a.code = b.athlete_code;

::

    Query plan:

    idx-join (inner join)
        outer: sscan
                   class: a node[0]
                   sargs: term[1]
                   cost:  44 card 7
        inner: iscan
                   class: b node[1]
                   index: fk_game_athlete_code term[0]
                   cost:  3 card 8653
        cost:  73 card 9

다음은 **USE_NL** 힌트 사용 시 사용하는 테이블을 명시하는 예이다.

.. code-block:: sql

    -- csql> ;plan_detail
    
    SELECT /*+ USE_NL(a,b)  */ a.name, b.host_year, b.medal
    FROM athlete a, game b 
    WHERE a.name = 'Sim Kwon Ho' AND a.code = b.athlete_code;

.. _index-hint-syntax:

인덱스 힌트
===========

인덱스 힌트 구문은 질의에서 인덱스를 지정할 수 있도록 해서 질의 처리기가 적절한 인덱스를 선택할 수 있게 한다. 이와 같은 인덱스 힌트 구문은 **USING INDEX** 절을 사용하는 방식과 FROM 절에 { **USE** | **FORCE** | **IGNORE** } **INDEX** 구문을 사용하는 방식이 있다.

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

*   **NONE**: **NONE** 을 지정한 경우 모든 테이블에 대해서 순차 스캔이 사용된다.
*   **ALL EXCEPT**: 질의 수행 시 지정한 인덱스를 제외한 모든 인덱스가 사용될 수 있다.
*   *index_name*\ (+): 인덱스 이름 뒤에 (+)를 지정하면 해당 인덱스 선택이 우선시 된다. 해당 인덱스가 해당 질의를 수행하는데 적합하지 않으면 선택하지 않는다. 
*   *index_name*\ (-): 인덱스 이름 뒤에 (-)를 지정하면 해당 인덱스가 선택에서 제외된다.
*   *table_spec*.\ **NONE**: 해당 테이블의 모든 인덱스가 선택에서 제외되어 순차 스캔이 사용된다.

USE, FORCE, IGNORE INDEX
------------------------

FROM 절의 테이블 명세 뒤에 **USE**, **FORCE**, **IGNORE INDEX** 구문을 통해서 인덱스 힌트를 지정할 수 있다. 

::

    FROM table_spec [ <index_hint_clause> ] ...
    
    <index_hint_clause> ::=
      { USE | FORCE | IGNORE } INDEX  ( <index_spec> [, <index_spec>  ...] )
    
    <index_spec> ::=
      [table_spec.]index_name

*   **USE INDEX** ( <*index_spec*> ): 지정한 인덱스들만 선택 시에 고려한다. 
*   **FORCE INDEX** ( <*index_spec*> ): 해당 인덱스 선택이 우선시 된다. 
*   **IGNORE INDEX** ( <*index_spec*> ): 지정한 인덱스들은 선택에서 제외된다. 

USE, FORCE, IGNORE INDEX 구문은 시스템에 의해 자동적으로 적절한 USING INDEX 구문으로 재작성된다.

인덱스 힌트 사용 예
-------------------

.. code-block:: sql

    CREATE TABLE athlete2 (
       code             SMALLINT PRIMARY KEY,
       name             VARCHAR(40) NOT NULL,
       gender           CHAR(1),
       nation_code      CHAR(3),
       event            VARCHAR(30)
    );
    CREATE UNIQUE INDEX athlete2_idx1 ON athlete2 (code, nation_code);
    CREATE INDEX athlete2_idx2 ON athlete2 (gender, nation_code);

아래 2개의 질의는 같은 동작을 수행하며, 지정된 *athlete2_idx2* 인덱스 스캔 비용이 순차 스캔 비용보다 작을 경우 해당 인덱스 스캔을 선택하게 된다. 

.. code-block:: sql

    SELECT /*+ RECOMPILE */ * 
    FROM athlete2 USE INDEX (athlete2_idx2) 
    WHERE gender='M' AND nation_code='USA';

    SELECT /*+ RECOMPILE */ * 
    FROM athlete2 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete2_idx2;

아래 2개의 질의는 같은 동작을 수행하며, 항상 *athlete2_idx2*\ 를 사용한다.

.. code-block:: sql
    
    SELECT /*+ RECOMPILE */ * 
    FROM athlete2 FORCE INDEX (athlete2_idx2) 
    WHERE gender='M' AND nation_code='USA';

    SELECT /*+ RECOMPILE */ * 
    FROM athlete2 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete2_idx2(+);

아래 2개의 질의는 같은 동작을 수행하며, 질의 수행 시 *athlete2_idx2*\ 를 사용하지 않는다.

.. code-block:: sql
    
    SELECT /*+ RECOMPILE */ * 
    FROM athlete2 IGNORE INDEX (athlete2_idx2) 
    WHERE gender='M' AND nation_code='USA';

    SELECT /*+ RECOMPILE */ * 
    FROM athlete2 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete2_idx2(-);

다음 질의는 수행 시 항상 순차 스캔을 선택한다.

.. code-block:: sql

    SELECT * 
    FROM athlete2 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX NONE;

    SELECT * 
    FROM athlete2
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete2.NONE;

다음 질의는 수행 시 *athlete2_idx2*\ 를 제외한 모든 인덱스의 사용이 가능하도록 한다.

.. code-block:: sql

    SELECT * 
    FROM athlete2 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX ALL EXCEPT athlete2_idx2;

다음과 같이 **USE INDEX** 구문 또는 **USING INDEX** 구문에서 여러 인덱스를 지정한 경우 질의 최적화기는 지정된 인덱스 중 하나를 선택한다.

.. code-block:: sql

    SELECT * 
    FROM athlete2 USE INDEX (athlete2_idx2, athlete2_idx1) 
    WHERE gender='M' AND nation_code='USA';

    SELECT * 
    FROM athlete2 
    WHERE gender='M' AND nation_code='USA'
    USING INDEX athlete2_idx2, athlete2_idx1;

여러 개의 테이블에 대해 질의를 수행하는 경우, 한 테이블에서는 특정 인덱스를 사용하여 인덱스 스캔을 하고 다른 테이블에서는 순차 스캔을 하도록 지정할 수 있다. 이러한 질의는 다음과 같은 형태가 된다.

.. code-block:: sql

    SELECT *
    FROM tab1, tab2 
    WHERE ... 
    USING INDEX tab1.idx1, tab2.NONE;

인덱스 힌트 구문이 있는 질의를 수행할 때 질의 최적화기는 인덱스가 지정되지 않는 테이블에 대해서는 해당 테이블의 사용 가능한 모든 인덱스를 고려한다. 예를 들어, *tab1* 테이블에는 인덱스 *idx1*, *idx2* 이 있고 *tab2* 테이블에는 인덱스 *idx3*, *idx4*, *idx5* 가 있는 경우, *tab1* 에 대한 인덱스만 지정하고 *tab2* 에 대한 인덱스를 지정하지 않으면 질의 최적화기는 *tab2* 의 인덱스도 고려하여 동작한다.

.. code-block:: sql

    SELECT ... 
    FROM tab1, tab2 USE INDEX(tab1.idx1) 
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
    ON [schema_name.]table_name (col1, col2, ...) 
    WHERE <filter_predicate>;
     
    ALTER  /*+ hints */ INDEX index_name
    [ ON [schema_name.]table_name (col1, col2, ...) 
    [ WHERE <filter_predicate> ] ]
    REBUILD;
     
    <filter_predicate> ::= <filter_predicate> AND <expression> | <expression>

*   <*filter_predicate*>: 칼럼과 상수 간 비교 조건. 조건이 여러 개인 경우 **AND** 로 연결된 경우에만 필터가 될 수 있다. 필터 조건으로 CUBRID에서 지원하는 대부분의 연산자와 함수가 포함될 수 있다. 그러나 현재 날짜/시간을 출력하는 날짜/시간 함수(예: :func:`SYS_DATETIME`), 랜덤 함수(예: :func:`RAND`)와 같이 같은 입력에 대해 다른 결과를 출력하는 함수는 허용되지 않는다.

필터링된 인덱스를 적용하여 질의를 처리하려면 **USE INDEX** 구문 또는 **FORCE INDEX** 구문을 통해 해당 필터링된 인덱스를 반드시 명시해야 한다.

*   **USING INDEX** 절 또는 **USE INDEX** 구문을 통해 필터링된 인덱스를 명시하는 경우: 

    인덱스를 구성하는 칼럼이 **WHERE** 절의 조건에 포함되어 있지 않으면 필터링된 인덱스를 사용하지 않는다. 

    .. code-block:: sql

        CREATE TABLE blogtopic 
        (
            blogID BIGINT NOT NULL, 
            title VARCHAR(128),
            author VARCHAR(128),
            content VARCHAR(8096),
            postDate TIMESTAMP NOT NULL,
            deleted SMALLINT DEFAULT 0
        );
   
        CREATE INDEX my_filter_index ON blogtopic(postDate) WHERE deleted=0;

    아래 질의에서 my_filter_index를 구성하는 칼럼인 *postDate* 가 **WHERE** 조건에 포함되어 있으므로, **USE INDEX** 구문으로도 인덱스를 사용할 수 있다.
        
    .. code-block:: sql
        
        SELECT * 
        FROM blogtopic USE INDEX (my_filter_index)
        WHERE postDate>'2010-01-01' AND deleted=0;
    
*   **USING INDEX** <index_name>(+) 절 또는 **FORCE INDEX** 구문을 통해 필터링된 인덱스를 명시하는 경우: 

    인덱스를 구성하는 칼럼이 **WHERE** 절의 조건에 포함되어 있지 않더라도 필터링된 인덱스를 사용한다.

    아래 질의에서는 *my_filter_index* 의 인덱스를 구성하는 칼럼이 **WHERE** 조건에 포함되어 있지 않으므로, **USE INDEX** 구문으로는 인덱스를 사용할 수 없다.

    .. code-block:: sql
        
        SELECT * 
        FROM blogtopic USE INDEX (my_filter_index)
        WHERE author = 'David' AND deleted=0;

    따라서, my_filter_index 인덱스를 사용하려면 다음과 같이 **FORCE INDEX** 구문을 사용하여 인덱스 사용을 강제해야 한다.
    
    .. code-block:: sql
        
        SELECT * 
        FROM blogtopic FORCE INDEX (my_filter_index)
        WHERE author = 'David' AND deleted=0;
    
다음은 버그/이슈를 유지하는 버그 트래킹 시스템의 예이다. 일정 기간의 개발 활동 이후 bugs 테이블에는 버그들이 기록되어 있는데, 이들 대부분은 오래 전에 종료된 상태이다. 버그 트래킹 시스템은 여전히 열린(open) 상태의 새로운 버그를 찾기 위해 해당 테이블에 질의를 한다. 이 경우 버그 테이블의 인덱스는 닫힌(closed) 버그의 레코드들에 대해 알 필요가 없다. 이런 경우 필터링된 인덱스는 열린 버그만 인덱싱하는 것을 허용한다.

.. code-block:: sql

    CREATE TABLE bugs
    (
        bugID BIGINT NOT NULL,
        CreationDate TIMESTAMP NOT NULL,
        Author VARCHAR(255),
        Subject VARCHAR(255),
        Description VARCHAR(255),
        CurrentStatus INTEGER,
        Closed SMALLINT
    );

열린 상태의 버그만을 위한 인덱스는 다음 문장으로 생성될 수 있다.

.. code-block:: sql

    CREATE INDEX idx_open_bugs ON bugs(bugID) WHERE Closed = 0;

열린 상태의 버그에만 관심있는 질의 처리를 위해 해당 인덱스를 인덱스 힌트로 지정하면, 필터링된 인덱스를 통하여 더 적은 인덱스 페이지를 접근하여 질의 결과를 생성할 수 있게 된다.

.. code-block:: sql

    SELECT * 
    FROM bugs
    WHERE Author = 'madden' AND Subject LIKE '%fopen%' AND Closed = 0
    USING INDEX idx_open_bugs(+);
     
    SELECT * 
    FROM bugs FORCE INDEX (idx_open_bugs)
    WHERE CreationDate > CURRENT_DATE - 10 AND Closed = 0;

위의 예에서 "**USING INDEX** *idx_open_bugs*" 또는 "**USE INDEX** (*idx_open_bugs*)" 를 사용하는 경우, *idx_open_bugs* 인덱스를 사용하지 않고 질의를 수행하게 된다.
    
.. warning::

    필터링된 인덱스 생성 조건과 질의 조건이 부합되지 않음에도 불구하고 인덱스 힌트 구문으로 인덱스를 명시하여 질의를 수행하면 명시된 인덱스를 선택하여 수행하므로, 주어진 검색 조건에 부합하지 않는 질의 결과를 출력할 수 있음에 주의한다.

.. note:: **제약 사항**

    필터링된 인덱스는 일반 인덱스만 허용한다. 예를 들어, 필터링된 유일한(unique) 인덱스는 허용하지 않는다. 또한, 필터링된 인덱스를 구성하는 칼럼 값이 모두 NULL이 가능한 경우는 허용하지 않는다. 
    예를 들어, 아래의 경우는 Author 값이 NULL일 수 있으므로 허용하지 않는다.

    .. code-block:: sql

        CREATE INDEX idx_open_bugs ON bugs (Author) WHERE Closed = 0;

    ::
        
        ERROR: before ' ; '
        Invalid filter expression (bugs.Closed=0) for index.
        
    하지만 아래의 경우는 Author 값이 NULL이더라도 CreationDate 값이 NULL일 수 없으므로 허용한다.

    .. code-block:: sql
        
        CREATE INDEX idx_open_bugs ON bugs (Author, CreationDate) WHERE Closed = 0;

    다음은 인덱스 필터 조건으로 허용하지 않는 경우이다.

    *   날짜/시간 함수 또는 랜덤 함수와 같이 입력이 같은데 결과가 매번 다른 함수

        .. code-block:: sql

            CREATE INDEX idx ON bugs(creationdate) WHERE creationdate > SYS_DATETIME;

        ::

            ERROR: before ' ; '
            'sys_datetime ' is not allowed in a filter expression for index.

        .. code-block:: sql

            CREATE INDEX idx ON bugs(bugID) WHERE bugID > RAND();

        ::
        
            ERROR: before ' ; '
            'rand ' is not allowed in a filter expression for index.
        
    *   **OR** 연산자를 사용하는 경우

        .. code-block:: sql

            CREATE INDEX IDX ON bugs (bugID) WHERE bugID > 10 OR bugID = 3;
        
        ::     
             
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

        ::
        
            ERROR: before ' ; '
            Invalid filter expression (( not t.a<>0) is null ) for index.
             
        .. code-block:: sql

            CREATE INDEX idx ON t (a) WHERE (a+1) IS NULL;
            
        ::
        
            ERROR: before ' ; '
            Invalid filter expression ((t.a+1) is null ) for index.

        .. code-block:: sql
             
            -- At least one attribute must not be used with IS NULL
            CREATE INDEX idx ON t(a,b) WHERE a IS NULL ;
            
        ::
        
            ERROR: before '  ; '
            Invalid filter expression (t.a is null ) for index.

        .. code-block:: sql
            
            CREATE INDEX idx ON t(a,b) WHERE a IS NULL and b IS NULL;
            
        ::
        
            ERROR: before ' ; '
            Invalid filter expression (t.a is null  and t.b is null ) for index.

        .. code-block:: sql
            
            CREATE INDEX idx ON t(a,b) WHERE a IS NULL and b IS NOT NULL;

    *   필터링된 인덱스에 대한 index skip scan(ISS)은 지원되지 않는다.
    *   필터링된 인덱스에서 사용되는 조건 문자열의 길이는 128자로 제한한다.

        .. code-block:: sql

            CREATE TABLE t(VeryLongColumnNameOfTypeInteger INT);
                
            CREATE INDEX idx ON t(VeryLongColumnNameOfTypeInteger) 
            WHERE VeryLongColumnNameOfTypeInteger > 3 AND VeryLongColumnNameOfTypeInteger < 10 AND 
            SQRT(VeryLongColumnNameOfTypeInteger) < 3 AND SQRT(VeryLongColumnNameOfTypeInteger) < 10;
            
        ::
        
            ERROR: before ' ; '
            The maximum length of filter predicate string must be 128.

.. _function-index:

함수 기반 인덱스
----------------

함수 기반 인덱스(function-based index)는 특정 함수를 이용하여 테이블 행들로부터 값의 조합에 기반한 데이터를 정렬하거나 찾고 싶을 때 사용한다. 예를 들어, 공백을 무시한 문자열을 찾는 작업을 하고 싶을 때 이러한 기능을 수행하는 함수를 이용하게 되는데, 함수를 통해 칼럼 값을 변경하게 되면 일반 인덱스를 통해서 인덱스 스캔을 할 수 없다. 이러한 경우에 함수 기반 인덱스를 생성하면 이를 통해 해당 질의 처리를 최적화할 수 있다. 다른 예로, 대소문자를 구분하지 않는 이름을 검색할 때 활용할 수 있다. ::

    CREATE /*+ hints */ INDEX index_name
    ON [schema_name.]table_name (function_name (argument_list));
    
    ALTER /*+ hints */ INDEX index_name
    [ ON [schema_name.]table_name (function_name (argument_list)) ]
    REBUILD;

다음 인덱스가 생성된 이후 **SELECT** 질의는 자동으로 함수 기반 인덱스를 사용한다.

.. code-block:: sql

    CREATE INDEX idx_trim_post ON posts_table(TRIM(keyword));
    
    SELECT * 
    FROM posts_table 
    WHERE TRIM(keyword) = 'SQL';

**LOWER** 함수로 함수 기반 인덱스를 생성하면, 대소문자 구분을 안 하는 이름을 검색할 때 사용될 수 있다.

.. code-block:: sql

    CREATE INDEX idx_last_name_lower ON clients_table(LOWER(LastName));
    
    SELECT * 
    FROM clients_table 
    WHERE LOWER(LastName) = LOWER('Timothy');

질의 계획을 생성할 때 인덱스가 선택되게 하기 위해서는, 이 인덱스에서 사용되는 함수가 질의 조건에서 같은 방법으로 사용되어야 한다. 위의 **SELECT** 질의는 위에서 생성된 last_name_lower 인덱스를 사용한다. 하지만 다음과 같은 조건에서는 함수 기반 인덱스 형태와 다른 표현식이 주어졌기 때문에 인덱스가 사용되지 않는다.

.. code-block:: sql

    SELECT * 
    FROM clients_table
    WHERE LOWER(CONCAT('Mr. ', LastName)) = LOWER('Mr. Timothy');

함수 기반 인덱스의 사용을 강제하려면 **USING INDEX** 구문을 사용할 수 있다.

.. code-block:: sql

    CREATE INDEX i_tbl_first_four ON tbl(LEFT(col, 4));
    SELECT *
    FROM clients_table 
    WHERE LEFT(col, 4) = 'CAT5' 
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

    CREATE INDEX my_idx ON tbl (TRIM(LEFT(col, 3)));
    CREATE INDEX my_idx ON tbl (LEFT(col1, col2 + 3));

묵시적인 타입 변환(implicit type cast)은 허용된다. 아래의 예에서 :func:`LEFT` 함수는 첫 번째 인자 타입이 **VARCHAR** 이고 두 번째 인자 타입이 **INTEGER** 여야 하지만 정상 동작한다.

.. code-block:: sql

    CREATE INDEX my_idx ON tbl (LEFT(int_col, str_col));

함수 기반 인덱스는 필터링된 인덱스와 함께 사용될 수 없다. 아래의 예는 오류를 발생한다.

.. code-block:: sql

    CREATE INDEX my_idx ON tbl (TRIM(col)) WHERE col > 'SQL';

함수 기반 인덱스는 다중 칼럼 인덱스가 될 수 없다. 아래의 예는 오류를 발생한다.

.. code-block:: sql

    CREATE INDEX my_idx ON tbl (TRIM(col1), col2, LEFT(col3, 5));


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

    -- csql> ;plan simple
    SELECT * FROM t WHERE col1 < 6;
    
::
    
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

        CREATE TABLE tab(c VARCHAR(32));
        INSERT INTO tab VALUES('abcd'),('abcd    '),('abcd ');
        CREATE INDEX i_tab_c ON tab(c);

    인덱스를 반드시 사용하도록(커버링 인덱스가 적용되도록) 했을 때의 질의 결과는 다음과 같다.

    .. code-block:: sql

        -- csql>;plan simple
        SELECT * FROM tab WHERE c='abcd    ' USING INDEX i_tab_c(+);
        
    ::
    
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
         
    ::
    
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

::
    
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

::

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

::
    
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

    :func:`CAST` 연산자 등을 통해 **ORDER BY** 절의 칼럼이 타입 변환되더라도, 타입 변환 전의 정렬 순서와 타입 변환 이후의 정렬 순서가 같다면 **ORDER BY** 절 최적화가 수행된다.
    
        +----------------+----------------+
        | 변환 전        | 변환 이후      |
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
*   별도의 정렬 작업이 필요 없는 내림차순 스캔

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

::
    
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

::

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

::
    
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

**GROUP BY** 절에 있는 모든 칼럼이 인덱스에 포함되어 질의 수행 시 인덱스를 사용할 수 있으므로 별도의 정렬 작업을 하지 않는 것을 **GROUP BY** 절 최적화라고 한다. 
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

집계 함수 사용 시에도 **GROUP BY** 칼럼으로 구성된 인덱스가 있으면 **GROUP BY** 최적화가 적용된다. 

.. code-block:: sql

    CREATE INDEX i_T_a_b_c ON T(a, b, c);
    SELECT a, MIN(b), c, MAX(b) FROM T WHERE a > 18 GROUP BY a, b;

.. note::

    GROUP BY 절 또는 DISTINCT의 칼럼이 인덱스 부분 키(subkey)를 포함할 때, 부분 키를 구성하는 칼럼 각각의 고유(unique) 값에 대해 동적으로 범위를 조정하여 B-트리 검색을 시작한다. 이와 관련하여 :ref:`loose-index-scan`\ 을 참고한다.

**예제**

.. code-block:: sql

    CREATE TABLE tab (i INT, j INT, k INT);
    CREATE INDEX i_tab_j_k ON tab (j, k);
    INSERT INTO tab VALUES (1,2,3), (6,4,2), (3,4,1), (5,2,1), (1,5,5), (2,6,6), (3,5,4);

    UPDATE STATISTICS on tab;

다음의 예는 *j*, *k* 칼럼으로 **GROUP BY** 를 수행하므로 *tab* (*j*, *k*)로 구성된 인덱스가 사용되고 별도의 정렬 과정이 필요 없다.

.. code-block:: sql

    SELECT /*+ RECOMPILE */ j,k 
    FROM tab 
    WHERE j > 0 
    GROUP BY j,k;
     
    --  the  selection from the query plan dump shows that the index i_tab_j_k was used and sorting was not necessary
    --  (/* ---> skip GROUP BY */)

::

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

::

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

.. code-block:: sql

    CREATE TABLE tab (k1 int, k2 int, k3 int, v double);
    INSERT INTO tab
        SELECT
            RAND(CAST(UNIX_TIMESTAMP() AS INT)) MOD 5,
            RAND(CAST(UNIX_TIMESTAMP() AS INT)) MOD 10,
            RAND(CAST(UNIX_TIMESTAMP() AS INT)) MOD 100000,
            RAND(CAST(UNIX_TIMESTAMP() AS INT)) MOD 100000
        FROM db_class a, db_class b, db_class c, db_class d LIMIT 20000;
    CREATE INDEX idx ON tab(k1, k2, k3);

위의 테이블과 인덱스를 생성했을 때 다음의 예는 k1, k2 칼럼으로 **GROUP BY**\를 수행하며 k3로 집계 함수를 수행하므로 tab(k1, k2, k3)로 구성된 인덱스가 사용되고 별도의 정렬 과정이 필요 없다. 또한 **SELECT** 리스트에 있는 k1, k2, k3 칼럼이 모두 tab(k1, k2, k3)로 구성된 인덱스 내에 존재하므로 커버링 인덱스가 적용된다.
    
.. code-block:: sql

    SELECT /*+ RECOMPILE INDEX_SS */ k1, k2, SUM(DISTINCT k3)
    FROM tab 
    WHERE k2 > -1 GROUP BY k1, k2;

::

    Query plan:

    iscan
        class: tab node[0]
        index: idx term[0] (covers) (index skip scan)
        sort:  1 asc, 2 asc
        cost:  85 card 2000

    Query stmt:

    select tab.k1, tab.k2, sum(distinct tab.k3) from tab tab where (tab.k2> ?:0 ) group by tab.k1, tab.k2

    /* ---> skip GROUP BY */

다음의 예는 *k1*, *k2* 칼럼으로 **GROUP BY**\를 수행하므로 *tab*(*k1*, *k2*, *k3*)로 구성된 인덱스가 사용되고 별도의 정렬 과정이 필요 없다. 하지만 **SELECT** 리스트에 있는 v 칼럼은 *tab*(*k1*, *k2*, *k3*)로 구성된 인덱스 내에 존재하지 않으므로 커버링 인덱스가 적용되지 않는다.
    
.. code-block:: sql
    
    SELECT /*+ RECOMPILE INDEX_SS */ k1, k2, stddev_samp(v)  
    FROM tab 
    WHERE k2 > -1 GROUP BY k1, k2;

::

    Query plan:

    iscan
        class: tab node[0]
        index: idx term[0] (index skip scan)
        sort:  1 asc, 2 asc
        cost:  85 card 2000

    Query stmt:

    select tab.k1, tab.k2, stddev_samp(tab.v) from tab tab where (tab.k2> ?:0 ) group by tab.k1, tab.k2

    /* ---> skip GROUP BY */

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

::

    Query plan: 
    iscan 
    class: t node[0] 
    index: i_t_a_b term[0] (covers) (multi_range_opt) 
    sort: 1 asc, 2 asc 
    cost: 1 card 0 

단일 테이블에서는 다음과 같은 조건들이 만족되었을 경우에 다중 키 범위 최적화가 수행된다. 

::

    SELECT /*+ hints */ ...
    FROM table
    WHERE col_1 = ? AND col_2 = ? AND ... AND col(j-1) = ?
    AND col_(j) IN (?, ?, ...)
    AND col_(j+1) = ? AND ... AND col_(p-1) = ?
    AND key_filter_terms
    ORDER BY col_(p) [ASC|DESC], col_(p+1) [ASC|DESC], ... col_(p+k-1) [ASC|DESC]
    LIMIT n;

먼저 **LIMIT** 절을 통해서 지정된 최종 결과의 상한 크기(*n*)가 **multi_range_optimization_limit** 시스템 파라미터 값보다 작거나 같아야 한다.

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
    LIMIT n;

JOIN 질의에 대해서 다중 키 범위 최적화가 적용되기 위해서는 다음과 같은 조건을 만족해야 한다.

*   **ORDER BY** 절에 존재하는 칼럼들은 하나의 테이블에만 존재하는 칼럼들이며, 이 테이블은 단일 테이블 질의에서 다중 키 범위 최적화에 의해 요구되는 조건을 모두 만족해야 한다. 이 테이블을 정렬 테이블(sort table)이라고 하자. 
*   정렬 테이블과 외부 테이블들(outer tables) 간의 JOIN 조건에 명시된 정렬 테이블의 칼럼들은 모두 인덱스에 포함되어야 한다. 즉, 데이터 필터링 조건이 없어야 한다. 
*   정렬 테이블과 내부 테이블들(inner tables) 간의 JOIN 조건에 명시된 정렬 테이블의 칼럼들은 범위 조건이 아닌 조건으로 WHERE 절에 포함되어야 한다. 

.. note:: 다중 키 범위 최적화가 적용될 수 있는 대부분의 경우에 다중 키 범위 최적화가 가장 좋은 성능을 보장하지만, 특정한 상황에서 최적화를 원하지 않는다면 질의에 **NO_MULTI_RANGE_OPT** 힌트를 명시하면 된다. 힌트를 지정하는 방법은 :ref:`sql-hint`\ 를 참고하면 된다.

.. _index-skip-scan:

Index Skip Scan
----------------

Index Skip Scan(이하 ISS)은 인덱스의 첫 번째 칼럼이 조건에 명시되지 않아도 뒤따라오는 칼럼이 조건(주로 =)에 명시되면 해당 인덱스를 활용하여 질의를 처리하는 최적화 방식이다. 

질의문 힌트를 통해 특정 테이블에 대한 **INDEX_SS**\가 입력되고 다음의 경우를 만족할 때 ISS의 적용이 고려된다. 

1.  복합 인덱스의 두번째 칼럼부터 조건에 명시된다.
2.  사용되는 인덱스가 필터링된 인덱스가 아니어야 한다.
3.  인덱스의 첫 번째 칼럼이 범위 필터나 키 필터가 아니어야 한다.
4.  계층 질의는 지원하지 않는다.
5.  집계 함수가 포함된 경우는 지원하지 않는다.

**INDEX_SS** 힌트에는 ISS의 적용을 고려할 테이블 리스트를 입력할 수 있으며,
테이블 리스트가 생략되는 경우 모든 테이블에 대해 ISS의 적용이 고려된다.

::

    /*+ INDEX_SS */
    /*+ INDEX_SS(tbl1) */
    /*+ INDEX_SS(tbl1, tbl2) */

.. note::

    "INDEX_SS"를 입력하면 모든 테이블에 ISS 힌트가 적용되지만, "INDEX_SS()"를 입력하면 힌트가 무시된다.

.. code-block:: sql

    CREATE TABLE t1 (id INT PRIMARY KEY, a INT, b INT, c INT);
    CREATE TABLE t2 (id INT PRIMARY KEY, a INT, b INT, c INT);
    CREATE INDEX i_t1_ac ON t1(a,c);
    CREATE INDEX i_t2_ac ON t2(a,c);

    INSERT INTO t1 SELECT rownum, rownum, rownum, rownum 
    FROM db_class x1, db_class x2, db_class LIMIT 10000;
    
    INSERT INTO t2 SELECT id, a%5, b, c FROM t1;


    SELECT /*+ INDEX_SS */ * 
    FROM t1, t2 
    WHERE t1.b<5 AND t1.c<5 AND t2.c<5 
    USING INDEX i_t1_ac, i_t2_ac limit 1;
    
    SELECT /*+ INDEX_SS(t1) */ * 
    FROM t1, t2 
    WHERE t1.b<5 AND t1.c<5 AND t2.c<5 
    USING INDEX i_t1_ac, i_t2_ac LIMIT 1;
    
    SELECT /*+ INDEX_SS(t1, t2) */ * 
    FROM t1, t2 
    WHERE t1.b<5 AND t1.c<5 AND t2.c<5 
    USING INDEX i_t1_ac, i_t2_ac LIMIT 1;

일반적으로 ISS는 여러 개의 칼럼들(C1, C2, ..., Cn) 중에서 고려되어야 하는데, 여기에서 질의는 연속된 칼럼들에 대한 조건을 가지고 있고 이 조건들은 인덱스의 두 번째 칼럼(C2)부터 시작한다.

::

    INDEX (C1, C2, ..., Cn);
     
    SELECT ... WHERE C2 = x AND C3 = y AND ... AND Cp = z; -- p <= n
    SELECT ... WHERE C2 < x AND C3 >= y AND ... AND Cp BETWEEN (z AND w); -- other conditions than equal

질의 최적화기는 궁극적으로 비용에 따라 ISS가 최적의 접근 방식인지 비용을 감안하여 결정한다. ISS는 인덱스의 첫 번째 칼럼이 레코드 개수에 비해 고유한(**DISTINCT**) 값의 개수가 적은 경우와 같이 특정한 상황에서 적용되며, 이 경우 인덱스 전체 검색(index full scan)보다 더 우수한 성능을 발휘한다. 예를 들어, 인덱스 칼럼 중에 첫 번째 칼럼이 남성/여성의 값 또는 수백만 건의 레코드가 1~100 사이의 값을 가지는 것처럼 DISTINCT 값의 개수가 매우 낮고(값의 중복도가 높고), 이 칼럼 조건이 질의 조건에 명시되지 않은 경우에 질의 최적화기는 ISS 적용을 검토하게 된다.

인덱스 전체 검색은 인덱스 리프 전체를 모두 다 읽어야 하지만, ISS는 동적으로 재조정되는 범위 검색(range search)을 사용하여 대부분의 인덱스 페이지 읽기를 생략하면서 질의를 처리한다. 값의 중복도가 높을수록 읽기를 생략할 수 있는 인덱스 페이지가 많아질 수 있기 때문에 ISS의 효율이 높아질 수 있다. 하지만 ISS가 많이 적용된다는 것은 인덱스 생성이 적절하지 않다는 것을 의미하기 때문에, DBA들은 인덱스 재조정이 필요하지 않은지 검토해볼 필요가 있다.

.. code-block:: sql

    CREATE TABLE tbl (name STRING, gender CHAR (1), birthday DATETIME);
    
    INSERT INTO tbl 
    SELECT ROWNUM, CASE (ROWNUM MOD 2) WHEN 1 THEN 'M' ELSE 'F' END, SYSDATETIME  
    FROM db_class a, db_class b, db_class c, db_class d, db_class e LIMIT 360000;
    
    CREATE INDEX idx_tbl_gen_name ON tbl (gender, name);
    -- Note that gender can only have 2 values, 'M' and 'F' (low cardinality)
    
    UPDATE STATISTICS ON ALL CLASSES;
    
.. code-block:: sql

    -- csql>;plan simple
    -- this will qualify to use Index Skip Scanning
    SELECT /*+ RECOMPILE INDEX_SS */ * 
    FROM tbl 
    WHERE name = '1000';

::

    Query plan:

     Index scan(tbl tbl, idx_tbl_gen_name, tbl.[name]= ?:0  (index skip scan))

.. code-block:: sql

    -- csql>;plan simple
    -- this will qualify to use Index Skip Scanning
    SELECT /*+ RECOMPILE INDEX_SS */ * 
    FROM tbl 
    WHERE name between '1000' and '1050';

::

    Query plan:

     Index scan(tbl tbl, idx_tbl_gen_name, (tbl.[name]>= ?:0  and tbl.[name]<= ?:1 ) (index skip scan))


.. _loose-index-scan:

Loose Index Scan
----------------

**GROUP BY** 절 또는 **DISTINCT**\의 칼럼이 인덱스 부분 키(subkey)를 포함할 때, loose index scan은 부분 키를 구성하는 칼럼 각각의 고유(unique) 값에 대해 동적으로 범위를 조정하여 B-트리 검색을 시작한다. 따라서 B-트리의 스캔 영역을 상당 부분 줄일 수 있다.

Loose index scan은 그룹핑되는 칼럼의 카디널리티가 전체 데이터량이 비해 매우 작을 때 적용하는 것이 유리하다.

질의문 힌트를 통해 **INDEX_LS**\가 입력되고 다음의 경우에 loose index scan의 적용이 고려된다.

1.  인덱스가 **SELECT** 리스트의 모든 부분을 커버하는 경우. 즉, 커버링 인덱스가 적용되는 경우
2.  **SELECT DISTINCT**, **SELECT** ... **GROUP BY** 또는 단일 투플 **SELECT** 문인 경우
3.  **MIN**/**MAX** 함수를 제외한 모든 집계 함수가 **DISTINCT** 를 포함하는 경우
4. **COUNT(*)** 가 사용되어선 안 됨
5.  부분 키(subkey)의 카디널리티(고유 값의 개수)가 전체 인덱스의 카디널리티보다 100배 작은 경우

부분 키는 복합 인덱스(composite index)에서 앞 쪽 부분에 해당하는 것으로, 예를 들어 INDEX(a, b, c, d)로 구성되어 있는 경우 (a), (a, b) 또는 (a, b, c)가 부분 키에 해당한다. 

이상과 같이 구성된 테이블에 대해 다음 질의를 수행하는 경우, 

.. code-block:: sql

    SELECT /*+ INDEX_LS */ a, b FROM tbl GROUP BY a;

칼럼 a에 대한 조건이 없으므로 부분 키를 사용할 수 없다. 그러나 다음과 같이 부분 키의 조건이 명시되면 loose index scan이 적용될 수 있다.

.. code-block:: sql

    SELECT /*+ INDEX_LS */ a, b FROM tbl WHERE a > 10 GROUP BY a;

다음과 같이 그룹핑 칼럼이 앞에, WHERE 조건 칼럼이 뒤에 오는 경우에도 부분 키를 사용할 수 있으므로 loose index scan이 적용될 수 있다.

.. code-block:: sql

    SELECT /*+ INDEX_LS */ a, b FROM tbl WHERE b > 10 GROUP BY a;

다음은 loose index scan이 적용되는 예이다.

.. code-block:: sql

    CREATE TABLE tbl1 (
        k1 INT, 
        k2 INT, 
        k3 INT, 
        k4 INT
    );
    
    INSERT INTO tbl1 
    SELECT ROWNUM MOD 2, ROWNUM MOD 400, ROWNUM MOD 80000, ROWNUM 
    FROM db_class a, db_class b, db_class c, db_class d, db_class e LIMIT 360000;
    
    CREATE INDEX idx ON tbl1 (k1, k2, k3);

    CREATE TABLE tbl2 (
        k1 INT, 
        k2 INT
    );
    
    INSERT INTO tbl2 VALUES (0, 0), (1, 1), (0, 2), (1, 3), (0, 4), (0, 100), (1000, 1000);

    UPDATE STATISTICS ON ALL CLASSES;

.. code-block:: sql

    -- csql>;plan simple
    -- add a condition to the grouped column, k1 to enable loose index scan
    SELECT /*+ RECOMPILE INDEX_LS */ DISTINCT k1     
    FROM tbl1 
    WHERE k1 > -1000000 LIMIT 20;

::

    Query plan:

     Sort(distinct)
        Index scan(tbl1 tbl1, idx, (tbl1.k1> ?:0 ) (covers) (loose index scan on prefix 1))

.. code-block:: sql
    
    -- csql>;plan simple
    -- different key ranges/filters
    SELECT /*+ RECOMPILE INDEX_LS */ DISTINCT k1 
    FROM tbl1 
    WHERE k1 >= 0 AND k1 <= 1;

::

    Query plan:

     Sort(distinct)
        Index scan(tbl1 tbl1, idx, (tbl1.k1>= ?:0  and tbl1.k1<= ?:1 ) (covers) (loose index scan on prefix 1))

.. code-block:: sql
    
    -- csql>;plan simple
    SELECT /*+ RECOMPILE INDEX_LS */ DISTINCT k1, k2 
    FROM tbl1 
    WHERE k1 >= 0 AND k1 <= 1 AND k2 > 3 AND k2 < 11;
    
::

    Query plan:

     Sort(distinct)
        Index scan(tbl1 tbl1, idx, (tbl1.k1>= ?:0  and tbl1.k1<= ?:1 ), [(tbl1.k2> ?:2  and tbl1.k2< ?:3 )] (covers) (loose index scan on prefix 2))

.. code-block:: sql
    
    -- csql>;plan simple
    SELECT /*+ RECOMPILE INDEX_LS */ DISTINCT k1, k2 
    FROM tbl1 
    WHERE k1 >= 0 AND k1 + k2 <= 10;

::

    Query plan:

     Sort(distinct)
        Index scan(tbl1 tbl1, idx, (tbl1.k1>= ?:0 ), [tbl1.k1+tbl1.k2<=10] (covers) (loose index scan on prefix 2))

.. code-block:: sql
    
    -- csql>;plan simple
    SELECT /*+ RECOMPILE INDEX_LS */ tbl1.k1, tbl1.k2 
    FROM tbl2 INNER JOIN tbl1 
    ON tbl2.k1 = tbl1.k1 AND tbl2.k2 = tbl1.k2 
    GROUP BY tbl1.k1, tbl1.k2;

::

 Sort(group by)
    Nested loops
        Sequential scan(tbl2 tbl2)
        Index scan(tbl1 tbl1, idx, tbl2.k1=tbl1.k1 and tbl2.k2=tbl1.k2 (covers) (loose index scan on prefix 2))

.. code-block:: sql
        
    SELECT /*+ RECOMPILE INDEX_LS */ MIN(k2), MAX(k2) 
    FROM tbl1;

::

    Query plan:

     Index scan(tbl1 tbl1, idx (covers) (loose index scan on prefix 2))

.. code-block:: sql

    -- csql>;plan simple
    SELECT /*+ RECOMPILE INDEX_LS */ SUM(DISTINCT k1), SUM(DISTINCT k2)
    FROM tbl1;

::

    Query plan:

     Index scan(tbl1 tbl1, idx (covers) (loose index scan on prefix 2))

.. code-block:: sql

    -- csql>;plan simple
    SELECT /*+ RECOMPILE INDEX_LS */ DISTINCT k1 
    FROM tbl1 
    WHERE k2 > 0;

::

    Query plan:

     Sort(distinct)
        Index scan(tbl1 tbl1, idx, [(tbl1.k2> ?:0 )] (covers) (loose index scan on prefix 2))

다음은 loose index scan이 적용되지 않는 경우이다.

.. code-block:: sql

    -- csql>;plan simple
    -- not enabled when full key is used
    SELECT /*+ RECOMPILE INDEX_LS */ DISTINCT k1, k2, k3 
    FROM tbl1 
    ORDER BY 1, 2, 3 LIMIT 10;
    
::

    Query plan:

     Sort(distinct)
        Sequential scan(tbl1 tbl1)

.. code-block:: sql

    -- csql>;plan simple
    SELECT /*+ RECOMPILE INDEX_LS */ k1, k2, k3
    FROM tbl1
    WHERE k1 > -10000 GROUP BY k1, k2, k3 LIMIT 10;

::

    Query plan:

     Index scan(tbl1 tbl1, idx, (tbl1.k1> ?:0 ) (covers))
     skip GROUP BY

.. code-block:: sql
    
    -- csql>;plan simple
    -- not enabled when using count star
    SELECT /*+ RECOMPILE INDEX_LS */ COUNT(*), k1 
    FROM tbl1 
    WHERE k1 > -10000 GROUP BY k1;

::

    Query plan:

     Index scan(tbl1 tbl1, idx, (tbl1.k1> ?:0 ) (covers))
     skip GROUP BY
    
.. code-block:: sql

    -- csql>;plan simple
    -- not enabled when index is not covering
    SELECT /*+ RECOMPILE INDEX_LS */ k1, k2, SUM(k4) 
    FROM tbl1 
    WHERE k1 > -1 AND k2 > -1 GROUP BY k1, k2 LIMIT 10;
    
::

    Query plan:

     Index scan(tbl1 tbl1, idx, (tbl1.k1> ?:0 ), [(tbl1.k2> ?:1 )])
     skip GROUP BY


.. code-block:: sql

    -- csql>;plan simple
    -- not enabled for non-distinct aggregates
    SELECT /*+ RECOMPILE INDEX_LS */ k1, SUM(k2) 
    FROM tbl1 
    WHERE k1 > -1 GROUP BY k1;
    
::

    Query plan:

     Index scan(tbl1 tbl1, idx, (tbl1.k1> ?:0 ) (covers))
     skip GROUP BY

.. code-block:: sql

    -- csql>;plan simple
    SELECT /*+ RECOMPILE */ SUM(k1), SUM(k2) 
    FROM tbl1;

::
    
    Query plan:

     Sequential scan(tbl1 tbl1)


.. _in-memory-sort:

인-메모리 정렬
--------------

인-메모리 정렬(in memory sort, IMS) 기능은 **ORDER BY** 절을 명시한 **LIMIT** 질의에 적용되는 최적화이다. 일반적으로, **ORDER BY** 와 **LIMIT** 절을 명시한 질의를 수행할 때, CUBRID는 전체 정렬 결과셋(full sorted resultset) 생성하고 나서 이 결과 셋에 **LIMIT** 연산을 적용한다. 전체 결과셋을 생성하는 대신 IMS 최적화를 사용하면, CUBRID는 **ORDER BY** ... **LIMIT** 절을 만족하는 투플만 정렬 버퍼(sort buffer)에 저장하는 인-메모리 바이너리 힙을 사용한다. 이 최적화는 정렬되지 않은 결과셋 전체를 정렬할 필요가 없게 하여 성능을 향상시킨다.

이 최적화의 적용 여부는 사용자가 제어하는 것이 아니며, CUBRID는 다음 상황에서 IMS를 사용할 것을 결정한다.

*   **ORDER BY** 와 **LIMIT** 절을 명시한 질의
*   **LIMIT** 절 적용 이후 최종 결과의 크기가 외부 정렬(external sort)에 의해 사용되는 메모리 양보다 작을 때(:ref:`memory-parameters`\ 의 **sort_buffer_size** 참고). 

IMS는 결과 행의 개수가 아닌 결과의 실제 크기를 고려함에 주의한다. 예를 들어, 기본 정렬 버퍼 크기(2MB)에 대해, 4바이트 **INTEGER** 하나로 구성된 레코드는 524288개 행의 **LIMIT** 까지 IMS가 적용되지만 **CHAR** (1024) 하나로 구성된 레코드는 2048개 행의 **LIMIT** 까지만 IMS가 적용된다. 이 최적화는 **DISTINCT** 정렬 결과 셋을 요구하는 질의에는 적용되지 않는다.

.. _sort-limit-optimization:

SORT-LIMIT 최적화
-----------------

SORT-LIMIT 최적화는 **ORDER BY** 절과 LIMIT 절을 명시한 질의에 적용되는 최적화이다. 이 최적화는 조인하는 동안의 카디널리티(cardinality)를 줄이기 위해 질의 계획에서 가능한 빨리 **LIMIT** 연산자를 평가하고자 한다. 

다음 조건이 만족될 때 SORT-LIMIT 계획이 고려될 수 있다.

*   **ORDER BY** 절에서 참조되는 모든 테이블은 SORT-LIMIT 계획에 속한다.
*   JOIN 테이블 중 다음 테이블이 SORT-LIMIT 계획에 포함된다.

    *   외래 키/기본 키 관계에서 외래 키를 가지는 테이블
    *   **LEFT JOIN** 시 왼쪽 테이블
    *   **RIGHT JOIN** 시 오른쪽 테이블

*   LIMIT 행은 **sort_limit_max_count** 시스템 파라미터(기본값: 1000) 값보다 작아야 한다. 
*   질의가 CROSS JOIN을 하지 않는다.
*   질의가 최소한 2개의 릴레이션으로 조인한다.
*   질의는 **GROUP BY** 절을 가지지 않는다.
*   질의는 **DISTINCT** 를 명시하지 않는다.
*   **ORDER BY** 표현식이 스캔하는 동안 평가될 수 있다. 

    예를 들어, 아래와 같은 질의는 SORT-LIMIT 최적화가 적용될 수 없는데, **SUM** 은 스캔하는 동안 평가될 수 없기 때문이다.

    .. code-block:: sql
    
        SELECT SUM(u.i) FROM u, t where u.i = t.i ORDER BY 1 LIMIT 5;

다음은 SORT-LIMIT을 계획하는 예이다.

.. code-block:: sql

    CREATE TABLE t(i int PRIMARY KEY, j int, k int);
    CREATE TABLE u(i int, j int, k int);
    ALTER TABLE u ADD constraint fk_t_u_i FOREIGN KEY(i) REFERENCES t(i);
    CREATE INDEX i_u_j ON u(j); 

    INSERT INTO t SELECT ROWNUM, ROWNUM, ROWNUM FROM _DB_CLASS a, _DB_CLASS b LIMIT 1000; 
    INSERT INTO u SELECT 1+(ROWNUM % 1000), RANDOM(1000), RANDOM(1000) FROM _DB_CLASS a, _DB_CLASS b, _DB_CLASS c LIMIT 5000; 

    SELECT /*+ RECOMPILE */ * FROM u, t WHERE u.i = t.i AND u.j > 10 ORDER BY u.j LIMIT 5; 

위의 **SELECT** 질의 계획이 아래와 같이 출력될 때 "(sort limit)"을 확인할 수 있다.

::

    Query plan:

    idx-join (inner join)
        outer: temp(sort limit)
                   subplan: iscan
                                class: u node[0]
                                index: i_u_j term[1]
                                cost:  1 card 0
                   cost:  1 card 0
        inner: iscan
                   class: t node[1]
                   index: pk_t_i term[0]
                   cost:  6 card 1000
        sort:  2 asc
        cost:  7 card 0

.. _tuning-rewrite:

재작성을 활용한 최적화
======================

.. _join-elimination-optimization:

조인 제거 최적화
----------------

조인 제거 최적화는 쿼리 결과에 영향을 주지 않는 테이블과의 조인을 제거하여 조인 연산을 줄이고, 쿼리 성능을 향상시키는 방법이다.

조인 제거 최적화에는 두 가지 동작이 있다:

    #. **INNER JOIN** 제거
    #. **LEFT OUTER JOIN** 제거

조인 제거 최적화를 하지 않으려면 **NO_ELIMINATE_JOIN** 힌트를 사용해야 한다.

.. _eliminate-inner-join:

**INNER JOIN** 제거
^^^^^^^^^^^^^^^^^^^

기본키(**PK**)가 있는 테이블과 그 기본키(**PK**)를 참조하는 외래키(**FK**)가 있는 테이블 간의 관계를 부모-자식 관계라고 한다.
부모-자식 관계의 **INNER JOIN**\에서 조인 조건 외에 부모 테이블에 대한 참조가 없는 경우에는 부모 테이블과의 조인을 제거해도 쿼리 결과에 영향을 주지 않는다.
이러한 경우에는 부모 테이블과의 조인을 제거하여 조인 연산을 줄이고, 쿼리 성능을 향상시킨다.

**INNER JOIN**\을 제거하기 위해서는 다음 조건을 만족해야 한다:

    #. 부모-자식 관계의 **INNER JOIN**\을 한다.
    #. 기본키(**PK**)와 외래키(**FK**)의 모든 컬럼은 조인 조건에 사용되어야 한다.
    #. 기본키(**PK**) 컬럼은 해당 기본키(**PK**) 컬럼을 참조하는 외래키(**FK**) 컬럼과 동일한 조인 조건에 사용되어야 한다.
    #. 모든 조인 조건은 동등(=) 비교를 사용해야 한다.
    #. 조인 조건 외에 부모 테이블에 대한 참조가 없어야 한다.
    #. 상속 관계가 있는 테이블을 조회하는 경우에는 **ALL** 키워드를 사용하지 않아야 한다.

부모 테이블과의 조인을 제거할 때 자식 테이블의 외래키(**FK**) 컬럼에 **NOT NULL** 제약 조건이 없는 경우에는 해당 컬럼에 대한 **IS NOT NULL** 조건이 추가된다.

다음은 **INNER JOIN**\을 제거하는 예제이다.

.. code-block:: sql

    call login ('public') on class db_user;

    /* current_user: public */
    drop table if exists child_tbl, parent_tbl;

    create table parent_tbl (
        id int,
        sub_id int,
        name varchar (100),
        filter int,
        primary key (id, sub_id)
    );

    insert into parent_tbl
    select
        ((rownum - 1) / 10) + 1 as id,
        (((rownum - 1) % 10) + 1) * 10 as sub_id,
        'Parent-' || lpad (rownum, 3) as name,
        ((rownum - 1) % 10) + 1 as filter
    from db_root
    connect by level <= 100;

    create table child_tbl (
        id int,
        name varchar (100),
        filter int,
        parent_id int not null,
        parent_sub_id int,
        primary key (id),
        foreign key (parent_id, parent_sub_id) references parent_tbl (id, sub_id)
    );

    insert into child_tbl
    select
        rownum as id,
        'Child-' || lpad (rownum, 5) as name,
        ((rownum - 1) % 100) + 1 as filter,
        id as parent_id,
        sub_id as parent_sub_id
    from parent_tbl, (select level from db_root connect by level <= 100);

    update statistics on parent_tbl, child_tbl with fullscan;

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id
    from
        parent_tbl p
        inner join child_tbl c on p.id = c.parent_id and p.sub_id = c.parent_sub_id
    where
        c.filter = 1 and c.parent_id = 1
    order by
        c.name;

*parent_tbl* 테이블과의 조인이 제거되었고, *child_tbl* 테이블의 *parent_sub_id* 컬럼에 대한 **IS NOT NULL** 조건이 추가되었다.

::

    Query plan:
    
    temp(order by)
        subplan: iscan
                     class: c node[0]
                     index: fk_child_tbl_parent_id_parent_sub_id term[1]
                     filtr: term[2]
                     sargs: term[0]
                     sort:  5 asc
                     cost:  8 card 1
        sort:  2 asc
        cost:  14 card 1
    
    Query stmt:
    
    select c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id from child_tbl c where c.parent_id= ?:0  and c.filter= ?:1  and c.parent_sub_id is not null  order by 2

::

               id  name                       filter    parent_id  parent_sub_id
    ============================================================================
                1  'Child-    1'                   1            1             10
              101  'Child-  101'                   1            1             20
              201  'Child-  201'                   1            1             30
              301  'Child-  301'                   1            1             40
              401  'Child-  401'                   1            1             50
              501  'Child-  501'                   1            1             60
              601  'Child-  601'                   1            1             70
              701  'Child-  701'                   1            1             80
              801  'Child-  801'                   1            1             90
              901  'Child-  901'                   1            1            100

다음은 **NO_ELIMINATE_JOIN** 힌트를 사용하는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile no_eliminate_join */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id
    from
        parent_tbl p
        inner join child_tbl c on p.id = c.parent_id and p.sub_id = c.parent_sub_id
    where
        c.filter = 1 and c.parent_id = 1
    order by
        c.name;

**NO_ELIMINATE_JOIN** 힌트를 사용했기 때문에 *parent_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (inner join)
                     outer: iscan
                                class: p node[0]
                                index: pk_parent_tbl_id_sub_id term[4] (covers)
                                cost:  1 card 10
                     inner: iscan
                                class: c node[1]
                                index: fk_child_tbl_parent_id_parent_sub_id term[1] AND term[3]
                                sargs: term[2]
                                cost:  3 card 1
                     cost:  4 card 1
        sort:  2 asc
        cost:  10 card 1
    
    Query stmt:
    
    select /*+ NO_ELIMINATE_JOIN */ c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id from parent_tbl p, child_tbl c where p.sub_id=c.parent_sub_id and p.id= ?:0  and c.parent_id= ?:1  and c.filter= ?:2  and p.id=c.parent_id order by 2

::

               id  name                       filter    parent_id  parent_sub_id
    ============================================================================
                1  'Child-    1'                   1            1             10
              101  'Child-  101'                   1            1             20
              201  'Child-  201'                   1            1             30
              301  'Child-  301'                   1            1             40
              401  'Child-  401'                   1            1             50
              501  'Child-  501'                   1            1             60
              601  'Child-  601'                   1            1             70
              701  'Child-  701'                   1            1             80
              801  'Child-  801'                   1            1             90
              901  'Child-  901'                   1            1            100

.. _eliminate-inner-join-1:

기본키(**PK**)와 외래키(**FK**)의 모든 컬럼은 조인 조건에 사용되어야 한다.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

다음은 *parent_tbl* 테이블의 *id* 컬럼과 *child_tbl* 테이블의 *parent_id* 컬럼만 조인 조건으로 사용된 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id
    from
        parent_tbl p
        inner join child_tbl c on p.id = c.parent_id
    where
        c.filter = 2 and c.parent_id = 2 and c.parent_sub_id = 20
    order by
        c.name;

*parent_tbl* 테이블의 *sub_id* 컬럼과 *child_tbl* 테이블의 *parent_sub_id* 컬럼이 조인 조건으로 사용되지 않았기 때문에 *parent_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: nl-join (cross join)
                     outer: iscan
                                class: c node[1]
                                index: fk_child_tbl_parent_id_parent_sub_id term[2] AND term[3]
                                sargs: term[1]
                                cost:  3 card 1
                     inner: iscan
                                class: p node[0]
                                index: pk_parent_tbl_id_sub_id term[4] (covers)
                                cost:  1 card 10
                     cost:  4 card 10
        sort:  2 asc
        cost:  10 card 10
    
    Query stmt:
    
    select c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id from parent_tbl p, child_tbl c where p.id= ?:0  and c.parent_sub_id= ?:1  and c.parent_id= ?:2  and c.filter= ?:3  and p.id=c.parent_id order by 2

::

               id  name                       filter    parent_id  parent_sub_id
    ============================================================================
             1102  'Child- 1102'                   2            2             20
             1102  'Child- 1102'                   2            2             20
             1102  'Child- 1102'                   2            2             20
             1102  'Child- 1102'                   2            2             20
             1102  'Child- 1102'                   2            2             20
             1102  'Child- 1102'                   2            2             20
             1102  'Child- 1102'                   2            2             20
             1102  'Child- 1102'                   2            2             20
             1102  'Child- 1102'                   2            2             20
             1102  'Child- 1102'                   2            2             20

.. _eliminate-inner-join-2:

기본키(**PK**) 컬럼은 해당 기본키(**PK**)를 참조하는 외래키(**FK**) 컬럼과 동일한 조인 조건에 사용되어야 한다.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

다음은 조인 조건에서 *id* 컬럼을 *parent_sub_id* 컬럼과 비교하고, *sub_id* 컬럼을 *parent_id* 컬럼과 비교하는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id
    from
        parent_tbl p
        inner join child_tbl c on p.id = c.parent_sub_id and p.sub_id = c.parent_id
    where
        c.filter = 3 and c.parent_id = 3
    order by
        c.name;

*id* 컬럼이 *parent_id* 컬럼과 비교되지 않았고, *sub_id* 컬럼이 *parent_sub_id* 컬럼과 비교되지 않았기 때문에 *parent* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (inner join)
                     outer: sscan
                                class: p node[0]
                                sargs: term[3]
                                cost:  1 card 1
                     inner: iscan
                                class: c node[1]
                                index: fk_child_tbl_parent_id_parent_sub_id term[0] AND term[4]
                                sargs: term[2]
                                cost:  3 card 1
                     cost:  5 card 1
        sort:  2 asc
        cost:  11 card 1
    
    Query stmt:
    
    select c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id from parent_tbl p, child_tbl c where p.id=c.parent_sub_id and p.sub_id= ?:0  and c.parent_id= ?:1  and c.filter= ?:2  and p.sub_id=c.parent_id order by 2

::

    There are no results.
    0 row selected.

.. _eliminate-inner-join-3:

모든 조인 조건은 동등(=) 비교를 사용해야 한다.
++++++++++++++++++++++++++++++++++++++++++++++

다음은 조인 조건으로 동등(=) 비교를 사용하지 않는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id
    from
        parent_tbl p
        inner join child_tbl c on p.id = c.parent_id and p.sub_id < c.parent_sub_id
    where
        c.filter = 4 and c.parent_id = 4 and c.parent_sub_id = 40
    order by
        c.name;

조인 조건에서 *parent_tbl* 테이블의 *sub_id* 컬럼과 *child_tbl* 테이블의 *parent_sub_id* 컬럼을 비교할 때 동등(=) 비교를 사용하지 않았기 때문에 *parent* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: nl-join (cross join)
                     outer: iscan
                                class: p node[0]
                                index: pk_parent_tbl_id_sub_id term[5] AND term[6] (covers)
                                cost:  1 card 1
                     inner: iscan
                                class: c node[1]
                                index: fk_child_tbl_parent_id_parent_sub_id term[3] AND term[4]
                                sargs: term[2]
                                cost:  3 card 1
                     cost:  4 card 1
        sort:  2 asc
        cost:  10 card 1
    
    Query stmt:
    
    select c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id from parent_tbl p, child_tbl c where (p.sub_id< ?:0 ) and p.id= ?:1  and c.parent_sub_id= ?:2  and c.parent_id= ?:3  and c.filter= ?:4  and p.id=c.parent_id and (p.sub_id<c.parent_sub_id) order by 2

::

               id  name                       filter    parent_id  parent_sub_id
    ============================================================================
             3304  'Child- 3304'                   4            4             40
             3304  'Child- 3304'                   4            4             40
             3304  'Child- 3304'                   4            4             40

.. _eliminate-inner-join-4:

조인 조건 외에 부모 테이블에 대한 참조가 없어야 한다.
+++++++++++++++++++++++++++++++++++++++++++++++++++++

다음은 *parent_tbl* 테이블의 *name* 컬럼을 조회하는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id, p.name
    from
        parent_tbl p
        inner join child_tbl c on p.id = c.parent_id and p.sub_id = c.parent_sub_id
    where
        c.filter = 5 and c.parent_id = 5
    order by
        c.name;

*parent_tbl* 테이블의 *name* 컬럼의 값을 조회하고 있기 때문에 *parent_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (inner join)
                     outer: iscan
                                class: p node[0]
                                index: pk_parent_tbl_id_sub_id term[4]
                                cost:  1 card 10
                     inner: iscan
                                class: c node[1]
                                index: fk_child_tbl_parent_id_parent_sub_id term[1] AND term[3]
                                sargs: term[2]
                                cost:  3 card 1
                     cost:  4 card 1
        sort:  2 asc
        cost:  10 card 1
    
    Query stmt:
    
    select c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id, p.[name] from parent_tbl p, child_tbl c where p.sub_id=c.parent_sub_id and p.id= ?:0  and c.parent_id= ?:1  and c.filter= ?:2  and p.id=c.parent_id order by 2

::

               id  name                       filter    parent_id  parent_sub_id  name
    ==================================================================================================
             4005  'Child- 4005'                   5            5             10  'Parent- 41'
             4105  'Child- 4105'                   5            5             20  'Parent- 42'
             4205  'Child- 4205'                   5            5             30  'Parent- 43'
             4305  'Child- 4305'                   5            5             40  'Parent- 44'
             4405  'Child- 4405'                   5            5             50  'Parent- 45'
             4505  'Child- 4505'                   5            5             60  'Parent- 46'
             4605  'Child- 4605'                   5            5             70  'Parent- 47'
             4705  'Child- 4705'                   5            5             80  'Parent- 48'
             4805  'Child- 4805'                   5            5             90  'Parent- 49'
             4905  'Child- 4905'                   5            5            100  'Parent- 50'

다음은 *parent* 테이블의 *filter* 컬럼에 대한 조건이 있는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id
    from
        parent_tbl p
        inner join child_tbl c on p.id = c.parent_id and p.sub_id = c.parent_sub_id
    where
        c.filter = 5 and c.parent_id = 5 and p.filter = 5
    order by
        c.name;

*parent_tbl* 테이블의 *filter* 컬럼의 값이 5인 레코드를 조회하고 있기 때문에 *parent_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (inner join)
                     outer: iscan
                                class: p node[0]
                                index: pk_parent_tbl_id_sub_id term[5]
                                sargs: term[3]
                                cost:  1 card 1
                     inner: iscan
                                class: c node[1]
                                index: fk_child_tbl_parent_id_parent_sub_id term[1] AND term[4]
                                sargs: term[2]
                                cost:  3 card 1
                     cost:  4 card 1
        sort:  2 asc
        cost:  10 card 1
    
    Query stmt:
    
    select c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id from parent_tbl p, child_tbl c where p.sub_id=c.parent_sub_id and p.id= ?:0  and p.filter= ?:1  and c.parent_id= ?:2  and c.filter= ?:3  and p.id=c.parent_id order by 2

::

               id  name                       filter    parent_id  parent_sub_id
    ============================================================================
             4405  'Child- 4405'                   5            5             50

다음은 *parent_tbl* 테이블의 *name* 컬럼을 기준으로 결과를 정렬하는 예제이다.

.. code-block:: sql

    select /*+ recompile */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id
    from
        parent_tbl p
        inner join child_tbl c on p.id = c.parent_id and p.sub_id = c.parent_sub_id
    where
        c.filter = 5 and c.parent_id = 5
    order by
        p.name desc;

*parent_tbl* 테이블의 *nmae* 컬럼을 기준으로 결과를 내림차순 정렬하고 있기 때문에 *parent_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (inner join)
                     outer: iscan
                                class: p node[0]
                                index: pk_parent_tbl_id_sub_id term[4]
                                cost:  1 card 10
                     inner: iscan
                                class: c node[1]
                                index: fk_child_tbl_parent_id_parent_sub_id term[1] AND term[3]
                                sargs: term[2]
                                cost:  3 card 1
                     cost:  4 card 1
        sort:  6 desc
        cost:  10 card 1
    
    Query stmt:
    
    select c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id, p.[name] from parent_tbl p, child_tbl c where p.sub_id=c.parent_sub_id and p.id= ?:0  and c.parent_id= ?:1  and c.filter= ?:2  and p.id=c.parent_id order by 6 desc

::

               id  name                       filter    parent_id  parent_sub_id
    ============================================================================
             4905  'Child- 4905'                   5            5            100
             4805  'Child- 4805'                   5            5             90
             4705  'Child- 4705'                   5            5             80
             4605  'Child- 4605'                   5            5             70
             4505  'Child- 4505'                   5            5             60
             4405  'Child- 4405'                   5            5             50
             4305  'Child- 4305'                   5            5             40
             4205  'Child- 4205'                   5            5             30
             4105  'Child- 4105'                   5            5             20
             4005  'Child- 4005'                   5            5             10

.. _eliminate-inner-join-5:

묵시적인 동등(=)
++++++++++++++++

**WHERE**\절에서 조인 조건 외에 부모 테이블에 대한 조건이 있더라도 조인 조건에 사용된 컬럼이 해당 조건에 사용된 경우에는 묵시적인 동등(=)에 의해 부모 테이블과의 조인을 제거할 수 있다.
해당 조건은 동등(=) 비교를 사용해야 하고, 상수와 비교되어야 한다.

다음은 *parent_tbl* 테이블의 *sub_id* 컬럼에 대한 동등(=) 비교 조건이 있는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id
    from
        parent_tbl p
        inner join child_tbl c on p.id = c.parent_id and p.sub_id = c.parent_sub_id
    where
        c.filter = 6 and c.parent_id = 6 and p.sub_id = 60
    order by
        c.name;

*parent_tbl* 테이블의 *sub_id* 컬럼의 값이 60인 레코드를 조회하고 있지만 조인 조건에 의해 해당 조건은 *child_tbl* 테이블의 *parent_sub_id* 컬럼의 값이 60인 조건과 동등하기 때문에 *parent_tbl* 테이블과의 조인이 제거되었다.

::

    Query plan:
    
    temp(order by)
        subplan: iscan
                     class: c node[0]
                     index: fk_child_tbl_parent_id_parent_sub_id term[1] AND term[2]
                     filtr: term[3]
                     sargs: term[0]
                     cost:  3 card 1
        sort:  2 asc
        cost:  9 card 1
    
    Query stmt:
    
    select c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id from child_tbl c where c.parent_sub_id= ?:0  and c.parent_id= ?:1  and c.filter= ?:2  and c.parent_sub_id is not null  order by 2

::

               id  name                       filter    parent_id  parent_sub_id
    ============================================================================
             5506  'Child- 5506'                   6            6             60

.. _eliminate-inner-join-6:

상속 관계가 있는 테이블을 조회하는 경우에는 **ALL** 키워드를 사용하지 않아야 한다.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

상속 관계가 있는 부모 또는 자식 테이블에 **ALL** 키워드를 사용하면 부모 테이블과의 조인을 제거할 수 없다.

다음은 *child_tbl* 테이블에 **ALL** 키워드를 사용하는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id
    from
        parent_tbl p
        inner join all child_tbl c on p.id = c.parent_id and p.sub_id = c.parent_sub_id
    where
        c.filter = 7 and c.parent_id = 7
    order by
        c.name;

*parent_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (inner join)
                     outer: iscan
                                class: p node[0]
                                index: pk_parent_tbl_id_sub_id term[4] (covers)
                                cost:  1 card 10
                     inner: iscan
                                class: c node[1]
                                index: fk_child_tbl_parent_id_parent_sub_id term[1] AND term[3]
                                sargs: term[2]
                                cost:  3 card 1
                     cost:  4 card 1
        sort:  2 asc
        cost:  10 card 1
    
    Query stmt:
    
    select c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id from parent_tbl p,  all child_tbl c where p.sub_id=c.parent_sub_id and p.id= ?:0  and c.parent_id= ?:1  and c.filter= ?:2  and p.id=c.parent_id order by 2

::

               id  name                       filter    parent_id  parent_sub_id
    ============================================================================
             6007  'Child- 6007'                   7            7             10
             6107  'Child- 6107'                   7            7             20
             6207  'Child- 6207'                   7            7             30
             6307  'Child- 6307'                   7            7             40
             6407  'Child- 6407'                   7            7             50
             6507  'Child- 6507'                   7            7             60
             6607  'Child- 6607'                   7            7             70
             6707  'Child- 6707'                   7            7             80
             6807  'Child- 6807'                   7            7             90
             6907  'Child- 6907'                   7            7            100

다음은 *parent_tbl* 테이블과 *child_tbl* 테이블에 **ONLY** 키워드를 사용하는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        c.id, c.name, c.filter, c.parent_id, c.parent_sub_id
    from
        only parent_tbl p
        inner join only child_tbl c on p.id = c.parent_id and p.sub_id = c.parent_sub_id
    where
        c.filter = 7 and c.parent_id = 7
    order by
        c.name;

*parent_tbl* 테이블과의 조인이 제거되었다.

::

    Query plan:
    
    temp(order by)
        subplan: iscan
                     class: c node[0]
                     index: fk_child_tbl_parent_id_parent_sub_id term[1]
                     filtr: term[2]
                     sargs: term[0]
                     sort:  5 asc
                     cost:  8 card 1
        sort:  2 asc
        cost:  14 card 1
    
    Query stmt:
    
    select c.id, c.[name], c.filter, c.parent_id, c.parent_sub_id from child_tbl c where c.parent_id= ?:0  and c.filter= ?:1  and c.parent_sub_id is not null  order by 2

::

               id  name                       filter    parent_id  parent_sub_id
    ============================================================================
             6007  'Child- 6007'                   7            7             10
             6107  'Child- 6107'                   7            7             20
             6207  'Child- 6207'                   7            7             30
             6307  'Child- 6307'                   7            7             40
             6407  'Child- 6407'                   7            7             50
             6507  'Child- 6507'                   7            7             60
             6607  'Child- 6607'                   7            7             70
             6707  'Child- 6707'                   7            7             80
             6807  'Child- 6807'                   7            7             90
             6907  'Child- 6907'                   7            7            100

.. _eliminate-left-outer-join:

**LEFT OUTER JOIN** 제거
^^^^^^^^^^^^^^^^^^^^^^^^

N:1 관계의 **LEFT OUTER JOIN**\에서 조인 조건 외에 오른쪽 테이블에 대한 참조가 없는 경우에는 오른쪽 테이블과의 조인을 제거해도 쿼리 결과에 영향을 주지 않는다.
이러한 경우에는 오른쪽 테이블과의 조인을 제거하여 조인 연산을 줄이고, 쿼리 성능을 향상시킨다.

**LEFT OUTER JOIN** 제거를 하기 위해서는 다음 조건을 만족해야 한다:

    #. **LEFT OUTER JOIN**\을 한다.
    #. 오른쪽 테이블은 기본키(**PK**) 또는 **UNIQUE** 제약조건이 있어야 한다.
    #. 기본키(**PK**) 또는 **UNIQUE** 제약조건의 모든 컬럼은 조인 조건에 사용되어야 한다.
    #. 모든 조인 조건은 동등(=) 비교를 사용해야 한다.
    #. 조인 조건 외에 오른쪽 테이블에 대한 참조가 없어야 한다.
    #. 상속 관계가 있는 테이블을 조회하는 경우에는 **ALL** 키워드를 사용하지 않아야 한다.

다음은 **LEFT OUTER JOIN** 제거를 하는 예제이다.

.. code-block:: sql

    call login ('public') on class db_user;

    /* current_user: public */
    drop table if exists left_tbl, right_tbl;

    create table left_tbl (
        id int,
        sub_id int,
        other_id int,
        name varchar (100),
        filter int,
        primary key (id, sub_id, other_id)
    );

    insert into left_tbl
    select
        ((rownum - 1) / 100) + 1 as id,
        (((rownum - 1) % 10) + 1) * 10 as sub_id,
        (((rownum - 1) % 100) + 1) * 100 as other_id,
        'Left-' || lpad (rownum, 4) as name,
        ((rownum - 1) % 100) + 1 as filter
    from db_root
    connect by level <= 1000;

    create table right_tbl (
        id int,
        sub_id int,
        name varchar (100),
        filter int,
        primary key (id, sub_id)
    );

    insert into right_tbl
    select
        id as id,
        sub_id as sub_id,
        'Right-' || lpad (rownum, 3) as name,
        ((rownum - 1) % 10) + 1 as filter
    from (select distinct id, sub_id from left_tbl);

    update statistics on left_tbl, right_tbl with fullscan;

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        l.id, l.sub_id, l.other_id, l.name, l.filter
    from
        left_tbl as l
        left outer join right_tbl as r on l.id = r.id and l.sub_id = r.sub_id
    where
        l.filter = 1
    order by
        l.name;

*right_tbl* 테이블과의 조인이 제거되었다.

::

    Query plan:
    
    temp(order by)
        subplan: sscan
                     class: l node[0]
                     sargs: term[0]
                     cost:  6 card 1
        sort:  4 asc
        cost:  13 card 1
    
    Query stmt:
    
    select l.id, l.sub_id, l.other_id, l.[name], l.filter from left_tbl l where l.filter= ?:0  order by 4

::

               id       sub_id     other_id  name                       filter
    ==========================================================================
                1           10          100  'Left-   1'                     1
                2           10          100  'Left- 101'                     1
                3           10          100  'Left- 201'                     1
                4           10          100  'Left- 301'                     1
                5           10          100  'Left- 401'                     1
                6           10          100  'Left- 501'                     1
                7           10          100  'Left- 601'                     1
                8           10          100  'Left- 701'                     1
                9           10          100  'Left- 801'                     1
               10           10          100  'Left- 901'                     1

다음은 **NO_ELIMINATE_JOIN** 힌트를 사용하는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile no_eliminate_join */
        l.id, l.sub_id, l.other_id, l.name, l.filter
    from
        left_tbl as l
        left outer join right_tbl as r on l.id = r.id and l.sub_id = r.sub_id
    where
        l.filter = 1
    order by
        l.name;

**NO_ELIMINATE_JOIN** 힌트를 사용했기 때문에 *right_tbl* 테이블과의 조인을 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (left outer join)
                     outer: sscan
                                class: l node[0]
                                sargs: term[2]
                                cost:  6 card 1
                     inner: iscan
                                class: r node[1]
                                index: pk_right_tbl_id_sub_id term[0] AND term[1] (covers)
                                cost:  1 card 100
                     cost:  9 card 1
        sort:  4 asc
        cost:  15 card 1
    
    Query stmt:
    
    select /*+ NO_ELIMINATE_JOIN */ l.id, l.sub_id, l.other_id, l.[name], l.filter from left_tbl l left outer join right_tbl r on l.sub_id=r.sub_id and l.id=r.id where l.filter= ?:0  order by 4

::

               id       sub_id     other_id  name                       filter
    ==========================================================================
                1           10          100  'Left-   1'                     1
                2           10          100  'Left- 101'                     1
                3           10          100  'Left- 201'                     1
                4           10          100  'Left- 301'                     1
                5           10          100  'Left- 401'                     1
                6           10          100  'Left- 501'                     1
                7           10          100  'Left- 601'                     1
                8           10          100  'Left- 701'                     1
                9           10          100  'Left- 801'                     1
               10           10          100  'Left- 901'                     1

.. _eliminate-left-outer-join-1:

오른쪽 테이블은 기본키(**PK**) 또는 **UNIQUE** 제약조건이 있어야 한다.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

오른쪽 테이블에 기본키(**PK**) 또는 **UNIQUE** 제약조건이 없으면 N:1 관계의 **LEFT OUTER JOIN**\이 아니기 때문에 오른쪽 테이블과의 조인을 제거할 수 없다.

다음은 *right_tbl* 테이블의 기본키(**PK**)를 제거하고 *id* 컬럼과 *sub_id* 컬럼으로 구성된 일반 인덱스를 생성해놓은 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    alter table right_tbl drop primary key;
    alter table right_tbl add index i_right_id_sub_id (id, sub_id);
    update statistics on right_tbl with fullscan;

    select /*+ recompile */
        l.id, l.sub_id, l.other_id, l.name, l.filter
    from
        left_tbl as l
        left outer join right_tbl as r on l.id = r.id and l.sub_id = r.sub_id
    where
        l.filter = 2
    order by
        l.name;

    drop index i_right_id_sub_id on right_tbl;
    alter table right_tbl add primary key (id, sub_id);
    update statistics on right_tbl with fullscan;

*right_tbl* 테이블에 기본키(**PK**) 또는 **UNIQUE** 제약조건이 없기 때문에 *right_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (left outer join)
                     outer: sscan
                                class: l node[0]
                                sargs: term[2]
                                cost:  6 card 1
                     inner: iscan
                                class: r node[1]
                                index: i_right_id_sub_id term[0] AND term[1] (covers)
                                cost:  1 card 100
                     cost:  9 card 1
        sort:  4 asc
        cost:  15 card 1
    
    Query stmt:
    
    select l.id, l.sub_id, l.other_id, l.[name], l.filter from left_tbl l left outer join right_tbl r on l.sub_id=r.sub_id and l.id=r.id where l.filter= ?:0  order by 4

::

               id       sub_id     other_id  name                       filter
    ==========================================================================
                1           20          200  'Left-   2'                     2
                2           20          200  'Left- 102'                     2
                3           20          200  'Left- 202'                     2
                4           20          200  'Left- 302'                     2
                5           20          200  'Left- 402'                     2
                6           20          200  'Left- 502'                     2
                7           20          200  'Left- 602'                     2
                8           20          200  'Left- 702'                     2
                9           20          200  'Left- 802'                     2
               10           20          200  'Left- 902'                     2

다음은 *right_tbl* 테이블의 기본키(**PK**)를 제거하고 *id* 컬럼과 *sub_id* 컬럼으로 구성된 **UNIQUE** 제약조건을 생성해놓은 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    alter table right_tbl drop primary key;
    alter table right_tbl add constraint unique (id, sub_id);
    update statistics on right_tbl with fullscan;

    select /*+ recompile */
        l.id, l.sub_id, l.other_id, l.name, l.filter
    from
        left_tbl as l
        left outer join right_tbl as r on l.id = r.id and l.sub_id = r.sub_id
    where
        l.filter = 2
    order by
        l.name;

    alter table right_tbl drop constraint u_right_tbl_id_sub_id;
    alter table right_tbl add primary key (id, sub_id);
    update statistics on right_tbl with fullscan;

*right_tbl* 테이블에 기본키(**PK**) 또는 **UNIQUE** 제약조건이 없기 때문에 *right_tbl* 테이블과의 조인이 제거되었다.

::

    Query plan:
    
    temp(order by)
        subplan: sscan
                     class: l node[0]
                     sargs: term[0]
                     cost:  6 card 1
        sort:  4 asc
        cost:  13 card 1
    
    Query stmt:
    
    select l.id, l.sub_id, l.other_id, l.[name], l.filter from left_tbl l where l.filter= ?:0  order by 4

::

               id       sub_id     other_id  name                       filter
    ==========================================================================
                1           20          200  'Left-   2'                     2
                2           20          200  'Left- 102'                     2
                3           20          200  'Left- 202'                     2
                4           20          200  'Left- 302'                     2
                5           20          200  'Left- 402'                     2
                6           20          200  'Left- 502'                     2
                7           20          200  'Left- 602'                     2
                8           20          200  'Left- 702'                     2
                9           20          200  'Left- 802'                     2
               10           20          200  'Left- 902'                     2

.. _eliminate-left-outer-join-2:

기본키(**PK**) 또는 **UNIQUE** 제약조건의 모든 컬럼을 조인 조건에 사용해야 한다.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

다음은 *left_tbl* 테이블의 *id* 컬럼과 *right_tbl* 테이블의 *id* 컬럼만 조인 조건으로 사용된 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        l.id, l.sub_id, l.other_id, l.name, l.filter
    from
        left_tbl as l
        left outer join right_tbl as r on l.id = r.id
    where
        l.filter = 3 and l.id = 3
    order by
        l.name;

*left_tbl* 테이블의 *sub_id* 컬럼과 *right_tbl* 테이블의 *sub_id* 컬럼이 조인 조건으로 사용되지 않았기 때문에 *right_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (left outer join)
                     outer: iscan
                                class: l node[0]
                                index: pk_left_tbl_id_sub_id_other_id term[2]
                                sargs: term[1]
                                cost:  3 card 1
                     inner: iscan
                                class: r node[1]
                                index: pk_right_tbl_id_sub_id term[0] (covers)
                                filtr: term[3]
                                cost:  1 card 10
                     sort:  2 asc, 3 asc
                     cost:  4 card 1
        sort:  4 asc
        cost:  10 card 1
    
    Query stmt:
    
    select l.id, l.sub_id, l.other_id, l.[name], l.filter from left_tbl l left outer join right_tbl r on l.id=r.id and r.id= ?:0  where l.id= ?:1  and l.filter= ?:2  order by 4

::

               id       sub_id     other_id  name                       filter
    ==========================================================================
                3           30          300  'Left- 203'                     3
                3           30          300  'Left- 203'                     3
                3           30          300  'Left- 203'                     3
                3           30          300  'Left- 203'                     3
                3           30          300  'Left- 203'                     3
                3           30          300  'Left- 203'                     3
                3           30          300  'Left- 203'                     3
                3           30          300  'Left- 203'                     3
                3           30          300  'Left- 203'                     3
                3           30          300  'Left- 203'                     3

.. _eliminate-left-outer-join-3:

모든 조인 조건은 동등(=) 비교를 사용해야 한다.
++++++++++++++++++++++++++++++++++++++++++++++

다음은 조인 조건으로 동등(=) 비교를 사용하지 않는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        l.id, l.sub_id, l.other_id, l.name, l.filter
    from
        left_tbl as l
        left outer join right_tbl as r on l.id = r.id and l.sub_id < r.sub_id
    where
        l.filter = 4 and l.id = 4
    order by
        l.name;

조인 조건에서 *left_tbl* 테이블의 *sub_id* 컬럼과 *right_tbl* 테이블의 *sub_id* 컬럼을 비교할 때 동등(=) 비교를 사용하지 않았기 때문에 *right_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (left outer join)
                     outer: iscan
                                class: l node[0]
                                index: pk_left_tbl_id_sub_id_other_id term[3]
                                sargs: term[2]
                                cost:  3 card 1
                     inner: iscan
                                class: r node[1]
                                index: pk_right_tbl_id_sub_id term[0] (covers)
                                filtr: term[1] AND term[4]
                                cost:  1 card 10
                     sort:  2 asc, 3 asc
                     cost:  4 card 1
        sort:  4 asc
        cost:  10 card 1
    
    Query stmt:
    
    select l.id, l.sub_id, l.other_id, l.[name], l.filter from left_tbl l left outer join right_tbl r on l.id=r.id and r.id= ?:0  and (l.sub_id<r.sub_id) where l.id= ?:1  and l.filter= ?:2  order by 4

::

               id       sub_id     other_id  name                       filter
    ==========================================================================
                4           40          400  'Left- 304'                     4
                4           40          400  'Left- 304'                     4
                4           40          400  'Left- 304'                     4
                4           40          400  'Left- 304'                     4
                4           40          400  'Left- 304'                     4
                4           40          400  'Left- 304'                     4

.. _eliminate-left-outer-join-4:

조인 조건 외에 오른쪽 테이블에 대한 참조가 없어야 한다.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++

다음은 *right_tbl* 테이블의 *name* 컬럼을 조회하는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        l.id, l.sub_id, l.other_id, l.name, l.filter, r.name
    from
        left_tbl as l
        left outer join right_tbl as r on l.id = r.id and l.sub_id = r.sub_id
    where
        l.filter = 5
    order by
        l.name;

*right_tbl* 테이블의 *name* 컬럼의 값을 조회하고 있기 때문에 *right_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (left outer join)
                     outer: sscan
                                class: l node[0]
                                sargs: term[2]
                                cost:  6 card 1
                     inner: iscan
                                class: r node[1]
                                index: pk_right_tbl_id_sub_id term[0] AND term[1]
                                cost:  1 card 100
                     cost:  9 card 1
        sort:  4 asc
        cost:  15 card 1
    
    Query stmt:
    
    select l.id, l.sub_id, l.other_id, l.[name], l.filter, r.[name] from left_tbl l left outer join right_tbl r on l.sub_id=r.sub_id and l.id=r.id where l.filter= ?:0  order by 4

::

               id       sub_id     other_id  name                       filter  name
    ================================================================================================
                1           50          500  'Left-   5'                     5  'Right-  5'
                2           50          500  'Left- 105'                     5  'Right- 15'
                3           50          500  'Left- 205'                     5  'Right- 25'
                4           50          500  'Left- 305'                     5  'Right- 35'
                5           50          500  'Left- 405'                     5  'Right- 45'
                6           50          500  'Left- 505'                     5  'Right- 55'
                7           50          500  'Left- 605'                     5  'Right- 65'
                8           50          500  'Left- 705'                     5  'Right- 75'
                9           50          500  'Left- 805'                     5  'Right- 85'
               10           50          500  'Left- 905'                     5  'Right- 95'

다음은 **LEFT OUTER JOIN**\을 하기 전에 *right_tbl* 테이블의 *filter* 컬럼에 대한 조건이 있는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        l.id, l.sub_id, l.other_id, l.name, l.filter
    from
        left_tbl as l
        left outer join right_tbl as r on l.id = r.id and l.sub_id = r.sub_id and r.filter = 5
    where
        l.filter = 5
    order by
        l.name;

*right_tbl* 테이블의 *filter* 컬럼의 값이 5인 레코드를 조회하고 있지만 **LEFT OUTER JOIN**\을 하기 전이기 때문에 *right_tbl* 테이블과의 조인이 제거되었다.

::

    Query plan:
    
    temp(order by)
        subplan: sscan
                     class: l node[0]
                     sargs: term[0]
                     cost:  6 card 1
        sort:  4 asc
        cost:  13 card 1
    
    Query stmt:
    
    select l.id, l.sub_id, l.other_id, l.[name], l.filter from left_tbl l where l.filter= ?:0  order by 4

::

               id       sub_id     other_id  name                       filter
    ==========================================================================
                1           50          500  'Left-   5'                     5
                2           50          500  'Left- 105'                     5
                3           50          500  'Left- 205'                     5
                4           50          500  'Left- 305'                     5
                5           50          500  'Left- 405'                     5
                6           50          500  'Left- 505'                     5
                7           50          500  'Left- 605'                     5
                8           50          500  'Left- 705'                     5
                9           50          500  'Left- 805'                     5
               10           50          500  'Left- 905'                     5

다음은 *right_tbl* 테이블의 *name* 컬럼으로 결과를 정렬하는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        l.id, l.sub_id, l.other_id, l.name, l.filter, r.name
    from
        left_tbl as l
        left outer join right_tbl as r on l.id = r.id and l.sub_id = r.sub_id
    where
        l.filter = 5
    order by
        r.name desc;

*right_tbl* 테이블의 *name* 컬럼을 기준으로 결과를 내림차순 정렬하고 있기 때문에 *right_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (left outer join)
                     outer: sscan
                                class: l node[0]
                                sargs: term[2]
                                cost:  6 card 1
                     inner: iscan
                                class: r node[1]
                                index: pk_right_tbl_id_sub_id term[0] AND term[1]
                                cost:  1 card 100
                     cost:  9 card 1
        sort:  6 desc
        cost:  15 card 1
    
    Query stmt:
    
    select l.id, l.sub_id, l.other_id, l.[name], l.filter, r.[name] from left_tbl l left outer join right_tbl r on l.sub_id=r.sub_id and l.id=r.id where l.filter= ?:0  order by 6 desc

::

               id       sub_id     other_id  name                       filter  name
    ================================================================================================
               10           50          500  'Left- 905'                     5  'Right- 95'
                9           50          500  'Left- 805'                     5  'Right- 85'
                8           50          500  'Left- 705'                     5  'Right- 75'
                7           50          500  'Left- 605'                     5  'Right- 65'
                6           50          500  'Left- 505'                     5  'Right- 55'
                5           50          500  'Left- 405'                     5  'Right- 45'
                4           50          500  'Left- 305'                     5  'Right- 35'
                3           50          500  'Left- 205'                     5  'Right- 25'
                2           50          500  'Left- 105'                     5  'Right- 15'
                1           50          500  'Left-   5'                     5  'Right-  5'

.. _eliminate-left-outer-join-5:

상속 관계가 있는 테이블을 조회하는 경우에는 **ALL** 키워드를 사용하지 않아야 한다.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

상속 관계가 있는 오른쪽 테이블에 **ALL** 키워드를 사용하면 부모 테이블과의 조인을 제거할 수 없다.

다음은 *right_tbl* 테이블에 **ALL** 키워드를 사용하는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        l.id, l.sub_id, l.other_id, l.name, l.filter
    from
        left_tbl as l
        left outer join all right_tbl as r on l.id = r.id and l.sub_id = r.sub_id
    where
        l.filter = 6
    order by
        l.name;

*right_tbl* 테이블과의 조인이 제거되지 않았다.

::

    Query plan:
    
    temp(order by)
        subplan: idx-join (left outer join)
                     outer: sscan
                                class: l node[0]
                                sargs: term[2]
                                cost:  6 card 1
                     inner: iscan
                                class: r node[1]
                                index: pk_right_tbl_id_sub_id term[0] AND term[1] (covers)
                                cost:  1 card 100
                     cost:  9 card 1
        sort:  4 asc
        cost:  15 card 1
    
    Query stmt:
    
    select l.id, l.sub_id, l.other_id, l.[name], l.filter from left_tbl l left outer join  all right_tbl r on l.sub_id=r.sub_id and l.id=r.id where l.filter= ?:0  order by 4

::

               id       sub_id     other_id  name                       filter
    ==========================================================================
                1           60          600  'Left-   6'                     6
                2           60          600  'Left- 106'                     6
                3           60          600  'Left- 206'                     6
                4           60          600  'Left- 306'                     6
                5           60          600  'Left- 406'                     6
                6           60          600  'Left- 506'                     6
                7           60          600  'Left- 606'                     6
                8           60          600  'Left- 706'                     6
                9           60          600  'Left- 806'                     6
               10           60          600  'Left- 906'                     6

다음은 *left_tbl* 테이블과 *right_tbl* 테이블에 **ONLY** 키워드를 사용하는 예제이다.

.. code-block:: sql

    /* current_user: public */
    set optimization level 513;

    select /*+ recompile */
        l.id, l.sub_id, l.other_id, l.name, l.filter
    from
        only left_tbl as l
        left outer join only right_tbl as r on l.id = r.id and l.sub_id = r.sub_id
    where
        l.filter = 6
    order by
        l.name;

*right* 테이블과의 조인이 제거되었다.

::

    Query plan:
    
    temp(order by)
        subplan: sscan
                     class: l node[0]
                     sargs: term[0]
                     cost:  6 card 1
        sort:  4 asc
        cost:  13 card 1
    
    Query stmt:
    
    select l.id, l.sub_id, l.other_id, l.[name], l.filter from left_tbl l where l.filter= ?:0  order by 4

::

               id       sub_id     other_id  name                       filter
    ==========================================================================
                1           60          600  'Left-   6'                     6
                2           60          600  'Left- 106'                     6
                3           60          600  'Left- 206'                     6
                4           60          600  'Left- 306'                     6
                5           60          600  'Left- 406'                     6
                6           60          600  'Left- 506'                     6
                7           60          600  'Left- 606'                     6
                8           60          600  'Left- 706'                     6
                9           60          600  'Left- 806'                     6
               10           60          600  'Left- 906'                     6

.. _view_merge:

View Merging 최적화
-----------------------
**View Merging**\은 질의 처리 시간과 오버헤드를 줄이는데 초점을 둔다. 질의에서 뷰(인라인 뷰 포함)를 사용할 때, 시스템은 일반적으로 새로운 임시 테이블을 생성한다. 
이렇게 생성된 임시 테이블은 인덱스를 사용할 수 없을 뿐만아니라, 임시 테이블을 생성하는 과정 자체가 시스템에 불필요한 부하를 발생하게 된다.
따라서 **View Merging**\은 뷰를 원래 테이블로 대체하여 뷰 수행시 발생하는 오버헤드를 피하고, 원래 테이블의 인덱스를 활용하여 더 효율적인 질의 처리를 가능하게 한다.

아래 질의문 1 처럼 인라인 뷰를 사용하면, 질의 내용을 파악하기 쉽다.

.. code-block:: sql


    /* 질의문 1 */
    SELECT *
    FROM (SELECT * FROM athlete WHERE nation_code = 'USA') a,
         (SELECT * FROM record WHERE medal = 'G') b
    WHERE a.code = b.athlete_code;


하지만, View Merging 최적화가 처리 되지 않는다면, 인라인 뷰 *a*\와 인라인 뷰 *b*\를 각각 수행후 결과를 임시 저장 공간에 보관하고, 임시 저장된 결과를 바탕으로 조인을 수행한다.

이 경우, 임시 저장 공간에 저장된 결과는 인덱스 사용이 불가능하여 많은 성능 저하가 발생한다.

.. code-block:: sql


    /* 질의문 2 */
    SELECT *
    FROM emp a, dept b
    WHERE a.code = b.athlete_code
    AND a.nation_code = 'USA'
    AND b.medal = 'G'


따라서 질의문 1의 뷰 질의들은 뷰를 참조하는 질의와의 머지(merge) 과정을 거쳐 질의문 2와 같은 형태로 변환된다.
이를 **View Merging**\이라고 한다.

큐브리드에서는 질의가 다음 조건에 해당하면 **View Merging**\을 수행할 수 없다.

    #. 뷰에 **NO_MERGE** 힌트가 사용된 경우

    #. 뷰가 **CONNECT BY**\를 포함한 경우

    #. 뷰가 **DISTINCT**\ 문을 포함한 경우

    #. 뷰가 **CTE**\(Common Table Expressions)인 경우
    
    #. 뷰를 **OUTER JOIN**\ 하는 경우

    #. 뷰에 집계함수나 분석함수를 사용하는 경우

    #. 뷰에 **ROWNUM, LIMIT**\ 또는 **GROUPBY_NUM (), INST_NUM (), ORDERBY_NUM ()**\ 을 사용하는 경우

    #. **Correlated Subquery**\ 를 사용하여 작성된 경우

    #. 뷰가 메소드를 포함한 경우

    #. 뷰가 **RANDOM (), DRANDOM (), SYS_GUID ()**\를 포함한 경우

다음은 뷰에 **NO_MERGE** 힌트가 사용된 예시이다.

.. code-block:: sql

    SELECT *
    FROM (SELECT /*+ NO_MERGE*/ * FROM athlete WHERE nation_code = 'USA') a,
    (SELECT /*+ NO_MERGE*/ * FROM record WHERE medal = 'G') b
    WHERE a.code = b.athlete_code;

뷰에 **NO_MERGE** 힌트를 사용할 경우, 해당 뷰는 **View Merging**\의 대상이 되지 않는다.

다음은 뷰가 **CONNECT BY**\를 포함한 예시이다.

.. code-block:: sql

    -- Creating tree table and then inserting data
    CREATE TABLE tree(id INT, mgrid INT, name VARCHAR(32), birthyear INT);

    INSERT INTO tree VALUES (1,NULL,'Kim', 1963);
    INSERT INTO tree VALUES (2,NULL,'Moy', 1958);
    INSERT INTO tree VALUES (3,1,'Jonas', 1976);
    INSERT INTO tree VALUES (4,1,'Smith', 1974);
    INSERT INTO tree VALUES (5,2,'Verma', 1973);
    INSERT INTO tree VALUES (6,2,'Foster', 1972);
    INSERT INTO tree VALUES (7,6,'Brown', 1981);

    -- Executing a hierarchical query with CONNECT BY clause
    SELECT *
    FROM (SELECT * FROM tree WHERE birthyear = 1973) t
    CONNECT BY PRIOR t.id=t.mgrid; 


위 질의문에서는 **CONNECT BY**\절을 사용하기 때문에 **View Merging**\을 수행할 수 없다.

다음은 뷰가 **DISTINCT**\문을 포함한 예시이다.

.. code-block:: sql

        SELECT * FROM (SELECT DISTINCT host_year FROM record) T;

위 질의문의 뷰 내부에 **DISTINCT**\문이 사용되어 **View Merging**\을 수행할 수 없다.


다음은 뷰가 **CTE**\(Common Table Expressions)인 예시이다.


.. code-block:: sql

        WITH cte AS (SELECT * FROM athlete WHERE gender = 'M') 
        SELECT * FROM cte WHERE cte.nation_code = 'USA';

위와 같이 **CTE**\를 포함한 질의문은 **View Merging**\을 수행할 수 없다.

다음은 뷰를 **OUTER JOIN**\ 하는 예시이다.

.. code-block:: sql

        SELECT * 
        FROM athlete a 
        LEFT OUTER JOIN (SELECT * FROM record WHERE host_year = 2020) b 
        ON a.code = b.athlete_code;

위와 같이 **OUTER JOIN**\을 수행하는 경우에는 **View Merging**\을 수행할 수 없다.

다음은 뷰에 집계함수나 분석함수를 사용하는 예시이다. 

.. code-block:: sql

        SELECT * 
        FROM (SELECT AVG(host_year) FROM record WHERE medal = 'G') a;

뷰에 집계함수나 분석함수를 포함한 질의문의 경우 **View Merging**\의 대상이 되지 않는다.

다음은 뷰에 **ROWNUM, LIMIT**\ 또는 **GROUPBY_NUM (), INST_NUM (), ORDERBY_NUM ()**\ 을 사용하는 예시이다.

.. code-block:: sql

        SELECT *
        FROM (SELECT gender, rownum FROM athlete WHERE rownum < 15) a
        WHERE gender = 'M';

뷰에 **ROWNUM, LIMIT**\ 또는 **GROUPBY_NUM (), INST_NUM (), ORDERBY_NUM ()**\을 사용한 질의문의 경우 **View Merging**\이 불가능하다.

다음은 **Correlated Subquery**\ 를 사용하여 작성된 예시이다

.. code-block:: sql

        SELECT COUNT(*)
        FROM athlete a,
        (SELECT * FROM record r WHERE a.code = r.athlete_code) b;

**Correlated Subquery**\를 사용한 질의문의 경우 **View Merging**\이 불가능하다.

다음은 뷰가 **RANDOM (), DRANDOM (), SYS_GUID ()**\를 포함한 예시이다.

.. code-block:: sql

        SELECT *
        FROM    (SELECT RANDOM (), code FROM athlete WHERE nation_code = 'USA') a,
                (SELECT SYS_GUID (), athlete_code FROM record WHERE medal = 'G') b
        WHERE a.code = b.athlete_code;

뷰가 **RANDOM (), DRANDOM (), SYS_GUID ()**\를 포함한 질의문의 경우 **View Merging**\이 불가능하다.

.. _pred-push:

Predicate Push
-----------------------
**Predicate Push**\는 외부의 조건절을 뷰 안으로 밀어 넣는 최적화이다.

이를 통해 뷰 수행시 추가된 조건에 의해 더 적은 양의 데이터만 조회할 수 있어 전체 처리량을 줄일 수 있다.


예를 들어 아래 질의 처럼 인라인 뷰와 테이블을 *a.code = r.athlete_code*\의 조인 조건으로 조인을 처리할 때, 조건절 중 인라인 뷰에 해당되는 조건절들을 인라인 뷰 안쪽에 밀어 넣을 수 있다면
조인해야 할 데이터양을 줄일 수 있다. 

.. code-block:: sql


        SELECT a.name, r.score 
        FROM (SELECT name, nation_code, code, count(*) cnt FROM athlete GROUP BY name, nation_code) a, record r
        WHERE a.code = r.athlete_code
        AND a.nation_code = 'KOR';


위 질의 중 인라인 뷰 내부에는 조건절이 없다. 만약 쿼리 변환이 수행되지 않았다면,
*athlete* 테이블을 full scan하여 생성된 임시 결과와 record 테이블을 조인 수행후에 *a.nation_code = 'KOR'* 조건을
필터링했을 것이다. 

하지만 **Predicate Push**\를 통해서 다음과 같이 질의가 변환된다면, 더 적은 양의
데이터만 조회되도록 최적화할 수 있다.

.. code-block:: sql

        SELECT a.name, r.score 
        FROM (SELECT name, nation_code, code, count(*) cnt FROM athlete WHERE nation_code = 'KOR' GROUP BY name, nation_code ) a, record r
        WHERE a.code = r.athlete_code;

다음의 경우에는 **Predicate Push**\가 수행되지 않는다.

    #. 주 질의에 **NO_PUSH_PRED** 힌트가 사용된 경우

    #. 주 질의가 **CONNECT BY**\를 포함한 경우

    #. 뷰에 집계함수나 분석함수를 사용하는 경우

    #. 뷰에 **ROWNUM, LIMIT** 또는 **GROUPBY_NUM (), INST_NUM (), ORDERBY_NUM ()**\ 을 사용하는 경우

    #. **Correlated Subquery**\ 를 사용하여 작성된 경우

    #. 조건절에 부질의가 사용된 경우

    #. 뷰가 메소드를 포함한 경우

    #. 푸시될 조건절이나 뷰 내부의 **Predicate Push** 대상에 **RANDOM (), DRANDOM (), SYS_GUID ()**\를 사용하는 경우

    #. **OUTER JOIN**\을 수행할 때 푸시될 조건절이나 뷰 내부의 **Predicate Push** 대상에 다음을 사용하는 경우:
    
            * **ON**\절에 조건절이 작성된 경우
            * **NULL** 변환 함수 (**COALESCE (), NVL (), NVL2 (), DECODE (), IF (), IFNULL (), CONCAT_WS ()**)
            * **IS NULL, CASE** 문

다음은 주 질의에 **NO_PUSH_PRED** 힌트가 사용된 예시이다.

.. code-block:: sql

        SELECT /*+ NO_PUSH_PRED*/ a.name, r.score 
        FROM (SELECT name, nation_code, code, count(*) cnt FROM athlete GROUP BY name, nation_code) a, record r
        WHERE a.code = r.athlete_code
        AND a.nation_code = 'KOR';

주 질의에 **NO_PUSH_PRED** 힌트가 사용된 경우, **Predicate Push**\가 수행되지 않는다.

다음은 질의가 **OUTER JOIN**\을 수행할 때 **ON**\절의 조건에 푸시될 조건절이 있는 예시이다.

.. code-block:: sql

        SELECT a.name, r.score 
        FROM (SELECT * FROM athlete WHERE gender = 'M') a
            LEFT OUTER JOIN record r ON a.code = r.athlete_code AND a.nation_code = 'KOR';

이 경우, *a.nation_code = 'KOR'*\는 **LEFT OUTER JOIN** 수행 시 **ON** 절에 있는데, 이러한 형태로 **ON** 절의 조건절은 **Predicate Push** 대상이 아니다.

다음 질의는 **OUTER JOIN**\을 수행할 때 푸시될 조건절이나 뷰 내부의 **Predicate Push** 대상에 **NULL** 변환 함수를 사용하는 예시이다.

.. code-block:: sql

        SELECT a.name, r.score 
        FROM athlete a
                LEFT OUTER JOIN (SELECT * FROM record WHERE medal = 'G') r ON a.code = r.athlete_code
        WHERE NVL(r.score, '0') = '0';

**OUTER JOIN**\을 수행할 때 푸시될 조건절이나 뷰 내부의 **Predicate Push** 대상에 **NULL** 변환 함수를 사용한 경우 **Predicate Push** 대상이 아니다.

.. _query-cache:

쿼리 캐시
===========

**QUERY_CACHE** 힌트는 반복적으로 실행되는 쿼리의 성능을 향상시키는 데 사용할 수 있으며, 쿼리는 전용 메모리 영역에 캐시되고 그 결과도 별도의 디스크 공간에 캐시된다. 단, 힌트는 SELECT 쿼리에만 적용된다. 그러나 다음과 같은 경우에는 쿼리에 힌트를 적용 할 수 없으며 힌트는 의미가 없어진다.

* 아래와 같이 질의에 시스템 시간 또는 날짜 관련 속성이 포함된 경우.
    예) SELECT SYSDATE, ADDDATE (SYSDATE, INTERVAL -24 HOUR), ADDDATE (SYSDATE, -1);
* SERIAL 관련 속성이 포함된 경우.
* 컬럼 경로 관련 속성이 포함된 경우.
* 메서드가 포함된 경우.
* 저장 프로 시저 또는 저장 함수가 포함된 경우.
* dual, _db_attribute 등과 같은 시스템 테이블이 포함된 경우.
* sys_guid ()와 같은 시스템 함수가 포함된 경우.

힌트가 설정되고 새 SELECT 질의가 처리될 때 질의가 캐시에서 발견되면 쿼리 캐시를 사용한다. 동일한 데이터베이스에서 동일한 질의 텍스트와 동일한 바인딩 값을 사용하는 경우 질의는 동일한 것으로 간주된다. 캐시된 질의를 찾을 수 없는 경우 질의가 처리된 다음 결과와 함께 캐시가 생성된다. 질의가 캐시에서 발견되면 캐시된 영역에서 결과를 가져온다. CSQL에서는 아래 예제와 같이 COUNT 함수를 사용하여 질의를 반복적으로 실행할 때 개선된 성능을 쉽게 측정 할 수 있다. 첫번째 질의에 대한 결과는 캐시가 되어 있지 않아서 느리지만, 두 번째 질의의 결과는 캐시 된 영역에서 가져오므로 응답 시간이 이전 질의보다 훨씬 빠르다. ::

    csql> SELECT /*+ QUERY_CACHE */ count(*) FROM game;

    === <Result of SELECT Command in Line 1> ===

         count(*)
    =============
         8653

    1 row selected. (0.107082 sec) Committed.

    1 command(s) successfully processed.

    csql> SELECT /*+ QUERY_CACHE */ count(*) FROM game;

    === <Result of SELECT Command in Line 1> ===

         count(*)
    =============
         8653

    1 row selected. (0.003932 sec) Committed.

    1 command(s) successfully processed.

다음과 같이 CSQL에 세션 명령 *;info qcache* 를 입력하여 쿼리가 캐시되는지 여부를 확인할 수 있다. ::

    csql> ;info qcache

    LIST_CACHE {
      n_hts 1010
      n_entries 1  n_pages 1
      lookup_counter 1
      hit_counter 1
      miss_counter 0
      full_counter 0
    }

    list_hts[0] 0x6a74d10
    HTABLE NAME = list file cache (DB_VALUE list), SIZE = 211, REHASH_AT = 147,
    NENTRIES = 1, NPREALLOC = 0, NCOLLISIONS = 0

    HASH AT 0
    LIST_CACHE_ENTRY (0x6c46d18) {
      param_values = [ ]
      list_id = { type_list { 1 integer/1 } tuple_cnt 1 page_cnt 1 first_vpid { 65 32766 } last_vpid { 65 32766 } lasttpl_len 24 query_id 2
      temp_vfid { 64 32766 } }
      uncommitted_marker = false
      tran_isolation = 4
      tran_index_array = [ ]
      last_ta_idx = 0
      query_string = select /*+ QUERY_CACHE */ count(*) from [game] [game]?193="en_US";194="en_US";249="Asia/Seoul";user=0|833|1
      time_created = 11/23/20 16:07:12.779703
      time_last_used = 11/23/20 16:07:22.772330
      ref_count = 1
      deletion_marker = false
    }

캐시 된 질의는 결과 화면 중간에 **query_string** 으로 표시되며 각 **n_entries** 및 **n_pages** 는 캐시된 질의 수와 캐시 된 결과의 페이지 수를 나타낸다. **n_entries** 는 파라미터 **max_query_cache_entries** 의 값으로 제한되고 **n_pages** 는 **query_cache_size_in_pages** 의 값으로 제한된다. **n_entries** 가 초과되거나 **n_pages** 가 초과되면 캐시 항목 중 일부가 삭제될 후보로 선택되어 삭제되고, 삭제되는 캐시는 **max_query_cache_entries** 값과 **query_cache_size_in_pages** 값의 약 20% 이다.

.. _correlated-subquery-cache:

서브 쿼리 캐시 (correlated)
------------------------------------

서브 쿼리 캐시 최적화는 상관 부질의(correlated subquery)를 포함한 질의 성능을 향상시키는데 사용할 수 있으며, 부질의 결과는 부질의마다 독립적인 공간에 캐시된다. 
서브 쿼리 캐시 최적화는 기본적으로 적용되므로 질의 수행시 캐시를 사용하지 않으려면 **NO_SUBQUERY_CACHE** 힌트를 해당 부질의에 사용해야 한다.

서브 쿼리 캐시 최적화는 상관 부질의가 SELECT 절에 있는 경우 적용되며, 처리 절차는 다음과 같다. 
재 실행되는 상관 부질의의 검색 조건 컬럼 값이 동일한 경우 부질의 수행 대신 캐시된 결과를 사용한다.
만약, 캐시된 값을 찾을 수 없는 경우 부질의 수행 후 참조한 컬럼 값과 결과를 서브 쿼리 캐시에 저장한다. 

:ref:`질의 프로파일링 <query-profiling>` 요청과 함께 질의 수행시 서브 쿼리 캐시 최적화가 적용된 부질의의 하위 정보로 서브 쿼리 캐시에 대한 프로파일링 정보가 출력된다.

다음은 상관 부질의 수행시 서브 쿼리 캐시 프로파일링 정보가 표시되는 예시이다

::

    csql> SELECT /*+ RECOMPILE NO_MERGE */
            (SELECT t1_pk FROM t1 b WHERE b.t1_pk = a.c3)
          FROM t1 a
          WHERE a.c2 >= 1;

    Trace Statistics:
      SELECT (time: 108, fetch: 103639, fetch_time: 13, ioread: 0)
        SCAN (table: dba.t1), (heap time: 70, fetch: 100384, ioread: 0, readrows: 100000, rows: 99000)
        SUBQUERY (correlated)
          SELECT (time: 4, fetch: 2970, fetch_time: 0, ioread: 0)
            SCAN (index: dba.t1.pk_t1), (btree time: 2, fetch: 1980, ioread: 0, readkeys: 990, filteredkeys: 0, rows: 990, covered: true)
            SUBQUERY_CACHE (hit: 98010, miss: 990, size: 269384, status: enabled)

서브 쿼리 캐시의 프로파일링 항목에 대한 설명은 다음과 같다.

* **hit** : 질의 실행 대신 캐시된 영역에서 결과를 가져온 횟수.
* **miss** : 질의를 실행한 후 결과를 캐시한 횟수.
* **size** : 서브 쿼리 캐시에 사용된 메모리 크기.
* **status** : 질의 종료 시 서브 쿼리 캐시의 활성화 여부.

**size**\가 설정해둔 값을 초과하는 경우에는 서브 쿼리 캐시가 질의 수행 도중 비활성화 되며 SQL 트레이스 정보에서 **status**\가 disabled로 출력된다. 또한, **miss**/**hit** 가 9 이상일 경우, 서브 쿼리 캐시의 메모리 크기가 설정해둔 값을 초과하지 않더라도 캐시 miss가 높다고 판단해서 질의 수행 도중 캐시가 비활성화 될 수 있다. 

다음은 서브 쿼리 캐시의 사용 유무에 따른 성능 차이를 확인하기 위해 반복 수행되는 상관 부질의 결과를 count하는 예제 질의이다.
다음은 예제 질의를 수행하기 위한 데이터를 준비하는 질의이다. 

::
    
    # Prepare data
    csql> DROP TABLE IF EXISTS t1;
    
    csql> CREATE TABLE t1 AS
            SELECT
                    ROWNUM AS t1_pk,
                    MOD(ROWNUM, 10) AS c1,
                    MOD(ROWNUM, 100) AS c2,
                    MOD(ROWNUM, 1000) AS c3
            FROM 
                    db_class a, db_class b, db_class c, db_class d, db_class e, db_class f
            LIMIT 100000;
    
    csql> ALTER TABLE t1 ADD CONSTRAINT PRIMARY KEY pk_t1 (t1_pk);

    csql> CREATE TABLE t2 AS
            SELECT
                    ROWNUM as c1,
                    1 as c2,
                    TO_CHAR(ROWNUM * 1000, '0999') as code
            FROM 
                    db_class a, db_class b
            LIMIT 10;
    
    csql> update statistics on t1 with fullscan;
    
    csql> ;trace on  

질의 #1는 서브 쿼리 캐시 최적화를 사용하여 질의를 수행했고, 질의 #2는  **NO_SUBQUERY_CACHE** 힌트를 사용하여 서브 쿼리 캐시를 비 활성화하여 질의를 수행했다. 
두 질의의 결과를 비교해보면 서브 쿼리 캐시 최적화를 사용한 것이 응답 수행이 개선된 것을 확인할 수 있다. 

+------------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| **Query #1**                                                           | **Query #2**                                                                                   |
+========================================================================+================================================================================================+
| ::                                                                     | ::                                                                                             |
|                                                                        |                                                                                                |
|     csql> SELECT COUNT(*)                                              |     csql> SELECT COUNT(*)                                                                      |
|            FROM (                                                      |            FROM (                                                                              |
|                   SELECT /*+ RECOMPILE NO_MERGE */                     |                   SELECT /*+ RECOMPILE NO_MERGE */                                             |
|                          (SELECT t1_pk FROM t1 b WHERE b.t1_pk = a.c3) |                         (SELECT /*+ NO_SUBQUERY_CACHE */ t1_pk FROM t1 b WHERE b.t1_pk = a.c3) | 
|                     FROM t1 a                                          |                     FROM t1 a                                                                  |
|                    WHERE a.c2 >= 1                                     |                    WHERE a.c2 >= 1                                                             |
|                 );                                                     |                 );                                                                             |
|                                                                        |                                                                                                |
|     === <Result of SELECT Command in Line 2> ===                       |     === <Result of SELECT Command in Line 2> ===                                               |
|                                                                        |                                                                                                |
|                  count(*)                                              |                  count(*)                                                                      |
|     ======================                                             |     ======================                                                                     |
|                     99000                                              |                     99000                                                                      |
|                                                                        |                                                                                                |
|     1 row selected. (0.128251 sec) Committed. (0.000010 sec)           |     1 row selected. (0.626199 sec) Committed. (0.000011 sec)                                   |
|                                                                        |                                                                                                |
|     1 command(s) successfully processed.                               |     1 command(s) successfully processed.                                                       |
+------------------------------------------------------------------------+------------------------------------------------------------------------------------------------+

서브 쿼리 캐시는 다음과 같은 상황에 동작하지 않는다:

* 상관 부질의가 상관 부질의를 포함한 경우. (단, 다른 상관 하위 쿼리가 포함되지 않은 최 하단의 상관 부질의는 서브 쿼리 캐시 최적화 작업이 수행됨)
* 부질의가 단일 행 부질의가 아닌 경우.
* 부질의가 CONNECT BY 절을 포함한 경우.
* 부질의에 OID 관련 기능이 포함된 경우.
* 부질의가 **NO_SUBQUERY_CACHE** 힌트를 포함한 경우.
* 새로운 결과를 저장할 때 설정한 서브 쿼리 캐시 크기(기본값: 2MB)를 초과하는 경우.
* random (), sys_guid ()와 같이 실행될 때마다 결과가 바뀌는 함수가 포함된 경우.

다음 예시는 상관 부질의가 상관 부질의를 포함한 경우, 최하단의 상관부질의만 서브 쿼리 캐시가 사용되고, 상단의 상관 부질의에는 서브 쿼리 캐시가 적용되지 않는 예시이다. 

::
    
    csql> SELECT /*+ recompile */ 
            (
                SELECT 
                    (
                        SELECT c.code
                        FROM t2 c
                        WHERE c.c1 = b.c1
                    ) 
                FROM t1 b 
                WHERE b.t1_pk = a.c1
            ) s
            FROM t1 a
            WHERE a.c3 = 1;
    
    Trace Statistics:
        SELECT (time: 56, fetch: 100785, fetch_time: 10, ioread: 0)
            SCAN (table: dba.t1), (heap time: 55, fetch: 100384, ioread: 0, readrows: 100000, rows: 100)
            SUBQUERY (correlated)
            SELECT (time: 0, fetch: 401, fetch_time: 0, ioread: 0)
                SCAN (index: dba.t1.pk_t1), (btree time: 0, fetch: 300, ioread: 0, readkeys: 100, filteredkeys: 0, rows: 100) (lookup time: 0, rows: 100)
                SUBQUERY (correlated)
                SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                    SCAN (table: dba.t2), (heap time: 0, fetch: 1, ioread: 0, readrows: 10, rows: 1)
                    SUBQUERY_CACHE (hit: 99, miss: 1, size: 150704, status: enabled)

다음은 상관 부질의에 random () 와 같이 실행될 때마다 결과가 바뀌는 함수가 포함된 경우 서브 쿼리 캐시가 비활성화되는 예시이다.

::

    csql> WITH cte_1 AS 
            (SELECT
                DISTINCT (SELECT random(1) FROM t2 b WHERE b.c1 = a.c1 AND b.c2 = 1) v
                FROM t1 a
                WHERE a.c2 = 1
            ) SELECT count(*) FROM cte_1;
    
    Trace Statistics:
        SELECT (time: 65, fetch: 101384, fetch_time: 9, ioread: 0)
            SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 1000, rows: 1000)
            SUBQUERY (uncorrelated)
            CTE (non_recursive_part)
                SELECT (time: 65, fetch: 101384, fetch_time: 9, ioread: 0)
                SCAN (table: dba.t1), (heap time: 59, fetch: 100384, ioread: 0, readrows: 100000, rows: 1000)
                ORDERBY (time: 0, sort: true, page: 0, ioread: 0)
                SUBQUERY (correlated)
                    SELECT (time: 4, fetch: 1000, fetch_time: 0, ioread: 0)
                    SCAN (table: dba.t2), (heap time: 3, fetch: 1000, ioread: 0, readrows: 10000, rows: 1000)