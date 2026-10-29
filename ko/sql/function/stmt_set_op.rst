
:meta-keywords: union statement, difference statement, intersection statement
:meta-description: Statement set operators are used to get union, difference or intersection on the result of more than one query statement specified as an operand.

****************
문장 집합 연산자
****************

UNION, DIFFERENCE, INTERSECTION
===============================

피연산자로 지정된 하나 이상의 질의문의 결과에 대해 합집합(**UNION**), 차집합(**DIFFERENCE**), 교집합(**INTERSECTION**)을 구하기 위하여 문장 집합 연산자(Statement Set Operator)를 이용한다. 단, 두 질의문의 대상 테이블에서 조회하고자 하는 데이터 타입이 동일하거나, 묵시적으로 변환 가능해야 한다. 

피연산자 질의문의 호스트 변수는 바인딩한 값과 같은 타입의 상수와 같은 규칙으로 변환된다. 예를 들어 **TIMESTAMP** 타입과 문자열 호스트 변수의 합집합은 **TIMESTAMP** 와 문자열 상수의 합집합처럼 **TIMESTAMP** 타입이다.

.. code-block:: sql

    PREPARE st FROM 'SELECT TIMESTAMP''2009-10-31 23:15:45'' AS ts UNION ALL SELECT ?';
    EXECUTE st USING '2010-01-01 10:00:00';

::

      ts                  
    ======================
      11:15:45 PM 10/31/2009
      10:00:00 AM 01/01/2010

단, 바인딩한 값의 타입을 서로 변환할 수 없으면 질의를 실행하기 전에 오류가 반환된다. 상수로 썼을 때 오류가 나는 조합은 호스트 변수로 써도 오류가 난다. 예를 들어 한쪽에 **DATE** 값, 다른 쪽에 **INTEGER** 값을 바인딩하면 두 타입을 하나로 맞출 수 없으므로 오류이다. **VALUES** 절의 여러 행도 같은 규칙을 따르며, 첫 행에 NULL을 바인딩하고 뒤 행에 타입이 있는 값을 바인딩하면 첫 행이 칼럼 타입을 NULL로 정하므로 오류이다.

.. code-block:: sql

    PREPARE st FROM 'SELECT ? FROM db_root UNION ALL SELECT ? FROM db_root';
    EXECUTE st USING DATE'2024-01-01', 1;

::

    ERROR: Semantic: before '  FROM db_root UNION ALL SELECT ? FROM db_root'
    ' ?:0 ' is not union compatible with ' ?:1 '.

::

    query_term statement_set_operator [qualifier] <query_term>
    [{statement_set_operator [qualifier] <query_term>}];  
     
        <query_term> ::=
            query_specification
            subquery
     
*   *qualifier*

    *   DISTINCT, DISTINCTROW 또는 UNIQUE(결과로 반환되는 인스턴스가 서로 다르다는 것을 보장)
    *   ALL (모든 인스턴스가 반환, 중복 허용)
     
*   *statement_set_operator*

    *   UNION (합집합)
    *   DIFFERENCE (차집합)
    *   INTERSECT | INTERSECTION (교집합)

다음은 CUBRID가 지원하는 문장 집합 연산자를 나타낸 표이다.

**문장 집합 연산자**

+------------------------+-----------------------------+---------------------------------------------------------+
| 문장 집합 연산자       | 설명                        | 비고                                                    |
+========================+=============================+=========================================================+
| **UNION**              | 합집합                      | **UNION ALL**                                           |
|                        | 중복을 허용하지 않음        | 이면 중복된 값을 포함한 모든 결과 인스턴스 출력         |
+------------------------+-----------------------------+---------------------------------------------------------+
| **DIFFERENCE**         | 차집합                      | **EXCEPT**                                              |
|                        | 중복을 허용하지 않음        | 연산자와 동일.                                          |
|                        |                             | **DIFFERENCE ALL**                                      |
|                        |                             | 이면 중복된 값을 포함한 모든 결과 인스턴스 출력         |
+------------------------+-----------------------------+---------------------------------------------------------+
| **INTERSECTION**       | 교집합                      | **INTERSECT**                                           |
|                        | 중복을 허용하지 않음        | 연산자와 동일.                                          |
|                        |                             | **INTERSECTION ALL**                                    |
|                        |                             | 이면 중복된 값을 포함한 모든 결과 인스턴스 출력         |
+------------------------+-----------------------------+---------------------------------------------------------+

다음은 문장 집합 연산자를 가지고 질의를 수행하는 예이다.

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
     
    --Using UNION to get only distinct rows
    SELECT id, name FROM nojoin_tbl_1
    UNION
    SELECT id, name FROM nojoin_tbl_2;

::
    
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
     
.. code-block:: sql

    --Using UNION ALL not eliminating duplicate selected rows
    SELECT id, name FROM nojoin_tbl_1
    UNION ALL
    SELECT id, name FROM nojoin_tbl_2;
     
::
    
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
     
.. code-block:: sql

    --Using DEFFERENCE to get only rows returned by the first query but not by the second
    SELECT id, name FROM nojoin_tbl_1
    DIFFERENCE
    SELECT id, name FROM nojoin_tbl_2;
     
::
    
               id  name
    ===================================
                1  'Kim'
                2  'Moy'
                3  'Jonas'
                4  'Smith'
     
.. code-block:: sql

    --Using INTERSECTION to get only those rows returned by both queries
    SELECT id, name FROM nojoin_tbl_1
    INTERSECT
    SELECT id, name FROM nojoin_tbl_2;
     
::
    
               id  name
    ===================================
                5  'Kim'
                6  'Smith'
                7  'Brown'
            
