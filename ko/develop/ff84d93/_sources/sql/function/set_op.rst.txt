
:meta-keywords: cubrid set, cubrid multiset, cubrid list, cubrid sequence, cubrid collection type

****************
집합 산술 연산자
****************

SET, MULTISET, LIST
===================

컬렉션 타입(**SET**, **MULTISET**, **LIST** (= **SEQUENCE**)) 데이터에 대해 합집합, 차집합, 교집합을 구하기 위해서 각각 +, -, * 연산자를 사용할 수 있다. 

::

    <value_expression>  <set_arithmetic_operator> <value_expression>
     
        <value_expression> ::=
            collection_value |
            NULL
     
        <set_arithmetic_operator> ::=
            + (합집합) |
            - (차집합) |
            * (교집합)

다음은 컬렉션 타입이 피연산자인 경우, 연산별 결과 데이터 타입을 나타낸 표이다.

**피연산자의 타입별 결과 데이터 타입**

+-----------------+--------------+--------------+-----------------+
|                 | SET          | MULTISET     | LIST            |
+=================+==============+==============+=================+
| **SET**         | **+**        | **+**        | **+**           |
|                 | ,            | ,            | ,               |
|                 | **-**        | **-**        | **-**           |
|                 | ,            | ,            | ,               |
|                 | **\***       | **\***       | **\***          |
|                 | :            | :            | :               |
|                 | **SET**      | **MULTISET** | **MULTISET**    |
+-----------------+--------------+--------------+-----------------+
| **MULTISET**    | **+**        | **+**        | **+**           |
|                 | ,            | ,            | ,               |
|                 | **-**        | **-**        | **-**           |
|                 | ,            | ,            | ,               |
|                 | **\***       | **\***       | **\***          |
|                 | :            | :            | :               |
|                 | **MULTISET** | **MULTISET** | **MULTISET**    |
+-----------------+--------------+--------------+-----------------+
| **LIST**        | **+, -, ***  | **+, -, ***  | **+**           |
| **(=SEQUENCE)** | :            | :            | :               |
|                 | **MULTISET** | **MULTISET** | **LIST**        |
|                 |              |              | **-, ***        |
|                 |              |              | :               |
|                 |              |              | **MULTISET**    |
+-----------------+--------------+--------------+-----------------+

다음은 컬렉션 타입을 가지고 산술 연산을 수행하는 예이다.

.. code-block:: sql

    SELECT ((CAST ({3,3,3,2,2,1} AS SET))+(CAST ({4,3,3,2} AS MULTISET)));

::
    
     (( cast({3, 3, 3, 2, 2, 1} as set))+( cast({4, 3, 3, 2} as multiset)))
    ======================
      {1, 2, 2, 3, 3, 3, 4}
     
.. code-block:: sql

    SELECT ((CAST ({3,3,3,2,2,1} AS MULTISET))+(CAST ({4,3,3,2} AS MULTISET)));
    
::
    
     (( cast({3, 3, 3, 2, 2, 1} as multiset))+( cast({4, 3, 3, 2} as multiset)))
    ======================
      {1, 2, 2, 2, 3, 3, 3, 3, 3, 4}
     
.. code-block:: sql

    SELECT ((CAST ({3,3,3,2,2,1} AS LIST))+(CAST ({4,3,3,2} AS MULTISET)));
    
::
    
     (( cast({3, 3, 3, 2, 2, 1} as sequence))+( cast({4, 3, 3, 2} as multiset)))
    ======================
      {1, 2, 2, 2, 3, 3, 3, 3, 3, 4}
     
.. code-block:: sql

    SELECT ((CAST ({3,3,3,2,2,1} AS SET))-(CAST ({4,3,3,2} AS MULTISET)));
    
::
    
     (( cast({3, 3, 3, 2, 2, 1} as set))-( cast({4, 3, 3, 2} as multiset)))
    ======================
      {1}
     
.. code-block:: sql

    SELECT ((CAST ({3,3,3,2,2,1} AS MULTISET))-(CAST ({4,3,3,2} AS MULTISET)));
    
::
    
     (( cast({3, 3, 3, 2, 2, 1} as multiset))-( cast({4, 3, 3, 2} as multiset)))
    ======================
      {1, 2, 3}
     
.. code-block:: sql

    SELECT ((CAST ({3,3,3,2,2,1} AS LIST))-(CAST ({4,3,3,2} AS MULTISET)));
    
::
    
     (( cast({3, 3, 3, 2, 2, 1} as sequence))-( cast({4, 3, 3, 2} as multiset)))
    ======================
      {1, 2, 3}
     
.. code-block:: sql

    SELECT ((CAST ({3,3,3,2,2,1} AS SET))*(CAST ({4,3,3,2} AS MULTISET)));
    
::
    
     (( cast({3, 3, 3, 2, 2, 1} as set))*( cast({4, 3, 3, 2} as multiset)))
    ======================
      {2, 3}
     
.. code-block:: sql

    SELECT ((CAST ({3,3,3,2,2,1} AS MULTISET))*(CAST ({4,3,3,2} AS MULTISET)));
    
::
    
     (( cast({3, 3, 3, 2, 2, 1} as multiset))*( cast({4, 3, 3, 2} as multiset)))
    ======================
      {2, 3, 3}
     
.. code-block:: sql

    SELECT ((CAST ({3,3,3,2,2,1} AS LIST))*(CAST ({4,3,3,2} AS MULTISET)));
    
::
    
     (( cast({3, 3, 3, 2, 2, 1} as sequence))*( cast({4, 3, 3, 2} as multiset)))
    ======================
    {2, 3, 3}

변수에 컬렉션 값 할당
---------------------

컬렉션 값을 변수에 할당하기 위해서는 외부 질의가 하나의 행만을 반환해야 한다.

다음은 컬렉션 값을 변수에 할당하는 방법을 나타내는 예제이다. 다음과 같이 외부 질의는 하나의 행만을 반환해야 한다.

.. code-block:: sql

    CREATE TABLE people (
        ssn VARCHAR(10),
        name VARCHAR(255)
    );
    
    INSERT INTO people 
    VALUES ('1234', 'Ken'), ('5678', 'Dan'), ('9123', 'Jones');
    
    SELECT SET(SELECT name
    FROM people
    WHERE ssn in {'1234', '5678'})
    TO :name_group;
