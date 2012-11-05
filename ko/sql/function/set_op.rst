***********
집합 연산자
***********

집합 산술 연산자
================

컬렉션 타입(**SET**, **MULTISET**, **LIST** (=**SEQUENCE**)) 데이터에 대해 합집합, 차집합, 교집합을 구하기 위해서 각각 +, -, * 연산자를 사용할 수 있다. 다음은 컬렉션 타입이 피연산자인 경우, 연산별 결과 데이터 타입을 나타낸 표이다.

**피연산자의 타입별 결과 데이터 타입**

+-----------------+--------------+--------------+-----------------+
| **?**           | **SET**      | **MULTISET** | **LIST**        |
|                 |              |              | **(=SEQUENCE)** |
|                 |              |              |                 |
+-----------------+--------------+--------------+-----------------+
| **SET**         | **+**        | **+**        | **+**           |
|                 | ,            | ,            | ,               |
|                 | **-**        | **-**        | **-**           |
|                 | ,            | ,            | ,               |
|                 | *****        | *****        | *****           |
|                 | :            | :            | :               |
|                 | **SET**      | **MULTISET** | **MULTISET**    |
|                 |              |              |                 |
+-----------------+--------------+--------------+-----------------+
| **MULTISET**    | **+**        | **+**        | **+**           |
|                 | ,            | ,            | ,               |
|                 | **-**        | **-**        | **-**           |
|                 | ,            | ,            | ,               |
|                 | *****        | *****        | *****           |
|                 | :            | :            | :               |
|                 | **MULTISET** | **MULTISET** | **MULTISET**    |
|                 |              |              |                 |
+-----------------+--------------+--------------+-----------------+
| **LIST**        | **+**        | **+**        | **+**           |
| **(=SEQUENCE)** | ,            | ,            | :               |
|                 | **-**        | **-**        | **LIST**        |
|                 | ,            | ,            | **-**           |
|                 | *****        | *****        | ,               |
|                 | :            | :            | *****           |
|                 | **MULTISET** | **MULTISET** | :               |
|                 |              |              | **MULTISET**    |
|                 |              |              |                 |
+-----------------+--------------+--------------+-----------------+

**구문**

::

	value_expression  set_arithmetic_operator value_expression
	 
	value_expression :
	• collection value
	• NULL
	 
	set_arithmetic_operator :
	• + (합집합)
	• - (차집합)
	• * (교집합)

**예제**

.. code-block:: sql

	SELECT ((CAST ({3,3,3,2,2,1} AS SET))+(CAST ({4,3,3,2} AS MULTISET)));
	 (( cast({3, 3, 3, 2, 2, 1} as set))+( cast({4, 3, 3, 2} as multiset)))
	======================
	  {1, 2, 2, 3, 3, 3, 4}
	 
	SELECT ((CAST ({3,3,3,2,2,1} AS MULTISET))+(CAST ({4,3,3,2} AS MULTISET)));
	 (( cast({3, 3, 3, 2, 2, 1} as multiset))+( cast({4, 3, 3, 2} as multiset)))
	======================
	  {1, 2, 2, 2, 3, 3, 3, 3, 3, 4}
	 
	SELECT ((CAST ({3,3,3,2,2,1} AS LIST))+(CAST ({4,3,3,2} AS MULTISET)));
	 (( cast({3, 3, 3, 2, 2, 1} as sequence))+( cast({4, 3, 3, 2} as multiset)))
	======================
	  {1, 2, 2, 2, 3, 3, 3, 3, 3, 4}
	 
	SELECT ((CAST ({3,3,3,2,2,1} AS SET))-(CAST ({4,3,3,2} AS MULTISET)));
	 (( cast({3, 3, 3, 2, 2, 1} as set))-( cast({4, 3, 3, 2} as multiset)))
	======================
	  {1}
	 
	SELECT ((CAST ({3,3,3,2,2,1} AS MULTISET))-(CAST ({4,3,3,2} AS MULTISET)));
	 (( cast({3, 3, 3, 2, 2, 1} as multiset))-( cast({4, 3, 3, 2} as multiset)))
	======================
	  {1, 2, 3}
	 
	SELECT ((CAST ({3,3,3,2,2,1} AS LIST))-(CAST ({4,3,3,2} AS MULTISET)));
	 (( cast({3, 3, 3, 2, 2, 1} as sequence))-( cast({4, 3, 3, 2} as multiset)))
	======================
	  {1, 2, 3}
	 
	SELECT ((CAST ({3,3,3,2,2,1} AS SET))*(CAST ({4,3,3,2} AS MULTISET)));
	 (( cast({3, 3, 3, 2, 2, 1} as set))*( cast({4, 3, 3, 2} as multiset)))
	======================
	  {2, 3}
	 
	SELECT ((CAST ({3,3,3,2,2,1} AS MULTISET))*(CAST ({4,3,3,2} AS MULTISET)));
	 (( cast({3, 3, 3, 2, 2, 1} as multiset))*( cast({4, 3, 3, 2} as multiset)))
	======================
	  {2, 3, 3}
	 
	SELECT ((CAST ({3,3,3,2,2,1} AS LIST))*(CAST ({4,3,3,2} AS MULTISET)));
	 (( cast({3, 3, 3, 2, 2, 1} as sequence))*( cast({4, 3, 3, 2} as multiset)))
	======================
	{2, 3, 3}

**변수에 컬렉션 값 할당**

컬렉션 값을 변수에 할당하기 위해서는 외부 질의가 하나의 행만을 반환해야 한다.

다음은 컬렉션 값을 변수에 할당하는 방법을 나타내는 예제이다. 다음과 같이 외부 질의는 하나의 행만을 반환해야 한다.

.. code-block:: sql

	SELECT SET(SELECT name
	FROM people
	WHERE ssn in {'1234', '5678'})
	TO :"names"
	FROM TABLE people;

문장 집합 연산자
================

피연산자로 지정된 하나 이상의 질의문의 결과에 대해 합집합(**UNION**), 차집합(**DIFFERENCE**), 교집합(**INTERSECTION**)을 구하기 위하여 문장 집합 연산자(Statement Set Operator)를 이용한다. 단, 두 질의문의 대상 테이블에서 조회하고자 하는 데이터 타입이 동일하거나, 묵시적으로 변환 가능해야 한다. 다음은 CUBRID가 지원하는 문장 집합 연산자와 예제를 나타낸 표이다.

**CUBRID가 지원하는 문장 집합 연산자**

+------------------+----------------------+-------------------------------------------------+
| 문장 집합 연산자 | 설명                 | 비고                                            |
+==================+======================+=================================================+
| **UNION**        | 합집합               | **UNION ALL**                                   |
|                  | 중복을 허용하지 않음 | 이면 중복된 값을 포함한 모든 결과 인스턴스 출력 |
+------------------+----------------------+-------------------------------------------------+
| **DIFFERENCE**   | 차집합               | **EXCEPT**                                      |
|                  | 중복을 허용하지 않음 | 연산자와 동일                                   |
|                  |                      | **DIFFERENCE ALL**                              |
|                  |                      | 이면 중복된 값을 포함한 모든 결과 인스턴스 출력 |
+------------------+----------------------+-------------------------------------------------+
| **INTERSECTION** | 교집합               | **INTERSECT**                                   |
|                  | 중복을 허용하지 않음 | 연산자와 동일                                   |
|                  |                      | **INTERSECTION ALL**                            |
|                  |                      | 이면 중복된 값을 포함한 모든 결과 인스턴스 출력 |
+------------------+----------------------+-------------------------------------------------+

**구문**

::

	query_term statement_set_operator [qualifier] query_term
	[{statement_set_operator [qualifier] query_term}];  
	 
	query_term :
	• query_specification
	• subquery
	 
	qualifier :
	• DISTINCT, DISTINCTROW 또는 UNIQUE(결과로 반환되는 인스턴스가 서로 다르다는 것을 보장)
	• ALL (모든 인스턴스가 반환, 중복 허용)
	 
	statement_set_operator :
	• UNION (합집합)
	• DIFFERENCE (차집합)
	• INTERSECT | INTERSECTION (교집합)

**예제**

.. code-block:: sql

	CREATE TABLE nojoin_tbl_1 (ID INT, Name VARCHAR(32));
	 
	INSERT INTO nojoin_tbl_1 VALUES (1,'Kim');
	INSERT INTO nojoin_tbl_1 VALUES (2,'Moy');
	INSERT INTO nojoin_tbl_1 VALUES (3,'Jonas');
	INSERT INTO nojoin_tbl_1 VALUES (4,'Smith');
	INSERT INTO nojoin_tbl_1 VALUES (5,'Kim');
	INSERT INTO nojoin_tbl_1 VALUES (6,'Smith');
	INSERT INTO nojoin_tbl_1 VALUES (7,'Brown');
	 
	CREATE TABLE nojoin_tbl_2 (id INT, Name VARCHAR(32));
	 
	INSERT INTO nojoin_tbl_2 VALUES (5,'Kim');
	INSERT INTO nojoin_tbl_2 VALUES (6,'Smith');
	INSERT INTO nojoin_tbl_2 VALUES (7,'Brown');
	INSERT INTO nojoin_tbl_2 VALUES (8,'Lin');
	INSERT INTO nojoin_tbl_2 VALUES (9,'Edwin');
	INSERT INTO nojoin_tbl_2 VALUES (10,'Edwin');
	 
	--Using UNION to get only distict rows
	SELECT id, name FROM nojoin_tbl_1
	UNION
	SELECT id,name FROM nojoin_tbl_2;
	 
			   id  name
	===================================
				1  'Kim'
				2  'Moy'
				3  'Jonas'
				4  'Smith'
				5  'Kim'
				6  'Smith'
				7  'Brown'
				8  'Lin'
				9  'Edwin'
			   10  'Edwin'
	 
	--Using UNION ALL not eliminating duplicate selected rows
	SELECT id, name FROM nojoin_tbl_1
	UNION ALL
	SELECT id,name FROM nojoin_tbl_2;
	 
			   id  name
	===================================
				1  'Kim'
				2  'Moy'
				3  'Jonas'
				4  'Smith'
				5  'Kim'
				6  'Smith'
				7  'Brown'
				5  'Kim'
				6  'Smith'
				7  'Brown'
				8  'Lin'
				9  'Edwin'
			   10  'Edwin'
	 
	--Using DEFFERENCE to get only rows returned by the first query but not by the second
	SELECT id, name FROM nojoin_tbl_1
	DIFFERENCE
	SELECT id,name FROM nojoin_tbl_2;
	 
			   id  name
	===================================
				1  'Kim'
				2  'Moy'
				3  'Jonas'
				4  'Smith'
	 
	--Using INTERSECTION to get only those rows returned by both queries
	SELECT id, name FROM nojoin_tbl_1
	INTERSECT
	SELECT id,name FROM nojoin_tbl_2;
	 
			   id  name
	===================================
				5  'Kim'
				6  'Smith'
				7  'Brown'
			