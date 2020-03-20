
:meta-keywords: cubrid seteq, cubrid setneq, cubrid superset, cubrid superseteq, cubrid subset, cubrid subseteq
:meta-description: Containment operators are used to check the containment relationship by performing comparison operation on operands of the collection data type.

:tocdepth: 3

***********
포함 연산자
***********

.. contents::

컬렉션 타입인 피연산자 간 비교 연산을 수행하여 포함(containment) 관계를 확인하기 위해 포함 연산자가 사용된다. 피연산자로 컬렉션 타입 또는 부질의(subquery)를 지정할 수 있으며, 두 피연산자의 포함 관계(동일하다/다르다/부분집합이다/진부분집합이다)에 따라 **TRUE** 또는 **FALSE** 를 반환한다.

::

    <collection_operand> <containment_operator> <collection_operand>
     
        <collection_operand> ::=
            <set> |
            <multiset> |
            <sequence> (또는 <list>) |
            <subquery> |
            NULL
     
        <containment_operator> ::=
            SETEQ |
            SETNEQ |
            SUPERSET |
            SUBSET |
            SUPERSETEQ |
            SUBSETEQ

*   <*collection_operand*>: 피연산자로 지정될 수 있는 수식은 하나의 집합 값 속성(SET-valued attribute)이거나, 집합 연산자(SET operator)를 지닌 산술 수식(arithmetic expression)이거나, 중괄호로 둘러싸인 집합 값이다. 이때, 중괄호로 둘러싸인 집합 값은 타입을 명시하지 않을 경우 기본적으로 **LIST** 타입으로 처리한다.

    피연산자로 부질의가 지정될 수 있으며, 컬렉션 타입이 아닌 칼럼을 조회하는 경우에는 **SET** (*subquery*)과 같이 해당 부질의에 컬렉션 타입 키워드를 붙여야 한다. 부질의에서 조회하는 칼럼은 하나의 집합만 결과로 반환해야 나머지 피연산자 집합과 비교할 수 있다.

    컬렉션 원소의 타입이 오브젝트이면, 오브젝트의 내용이 아닌 객체 식별자(OID, object identifier)에 대해 비교한다. 예를 들어, 같은 속성 값을 갖고 OID가 다른 두 오브젝트는 서로 다른 것으로 간주한다.

    *   **NULL**: 비교 대상이 되는 피연산자 중 어느 하나가 **NULL** 인 경우, **NULL** 이 반환된다.

다음은 CUBRID가 지원하는 포함 연산자에 관한 설명 및 리턴 값을 나타낸 표이다.

**CUBRID가 지원하는 포함 연산자**

+----------------+-------------------------------------+--------------------------+----------+
| 포함 연산자    | 설명                                | 조건식                   | 리턴 값  |
+================+=====================================+==========================+==========+
| A              | A = B:                              | {1,2} SETEQ {1,2,2}      | 0        |
| **SETEQ**      | 집합 A와 집합 B의 원소가 서로 같다. |                          |          |
| B              |                                     |                          |          |
+----------------+-------------------------------------+--------------------------+----------+
| A              | A <> B:                             | {1,2} SETNEQ {1,2,3}     | 1        |
| **SETNEQ**     | 집합 A와 집합 B의 원소가 같지 않다. |                          |          |
| B              |                                     |                          |          |
+----------------+-------------------------------------+--------------------------+----------+
| A              | A > B:                              | {1,2} SUPERSET {1,2,3}   | 0        |
| **SUPERSET**   | 집합 B는 집합 A의 진 부분집합이다.  |                          |          |
| B              |                                     |                          |          |
+----------------+-------------------------------------+--------------------------+----------+
| A              | A < B:                              | {1,2} SUBSET {1,2,3}     | 1        |
| **SUBSET**     | 집합 A는 집합 B의 진 부분집합이다.  |                          |          |
| B              |                                     |                          |          |
+----------------+-------------------------------------+--------------------------+----------+
| A              | A >= B:                             | {1,2} SUPERSETEQ {1,2,3} | 0        |
| **SUPERSETEQ** | 집합 B는 집합 A의 부분 집합이다.    |                          |          |
| B              |                                     |                          |          |
+----------------+-------------------------------------+--------------------------+----------+
| A              | A <= B:                             | {1,2} SUBSETEQ {1,2,3}   | 1        |
| **SUBSETEQ**   | 집합 A는 집합 B의 부분 집합이다.    |                          |          |
| B              |                                     |                          |          |
+----------------+-------------------------------------+--------------------------+----------+

다음은 포함 연산자를 이용하는 경우, 피연산자의 타입별 연산 가능 여부 및 타입 변환 여부를 나타낸 표이다.

**포함 연산자의 피연산자 타입별 연산 가능 여부**

+---------------------+-----------+------------------+-------------------------+
|                     | SET       | MULTISET         | LIST(=SEQUENCE)         |
+=====================+===========+==================+=========================+
| **SET**             | 연산 가능 | 연산 가능        | 연산 가능               |
+---------------------+-----------+------------------+-------------------------+
| **MULTISET**        | 연산 가능 | 연산 가능        | 연산 가능               |
|                     |           |                  | (**LIST** 타입은        |
|                     |           |                  | **MULTISET**            |
|                     |           |                  | 타입으로 변환됨)        |
+---------------------+-----------+------------------+-------------------------+
| **LIST(=SEQUENCE)** | 연산 가능 | 연산 가능        | 일부 연산만 가능        |
|                     |           | (**LIST** 타입은 | (**SETEQ**, **SETNEQ**) |
|                     |           | **MULTISET**     | 나머지 연산은           |
|                     |           | 타입으로 변환됨) | 에러 발생               |
+---------------------+-----------+------------------+-------------------------+

.. code-block:: sql

    --empty set is a subset of any set
    SELECT ({} SUBSETEQ (CAST ({3,1,2} AS SET)));
    
::

           Result
    =============
                1
     
.. code-block:: sql

    --operation between set type and null returns null
    SELECT ((CAST ({3,1,2} AS SET)) SUBSETEQ NULL);
    
::

           Result
    =============
            NULL
     
.. code-block:: sql

    --{1,2,3} seteq {1,2,3} returns true
    SELECT ((CAST ({3,1,2} AS SET)) SETEQ (CAST ({1,2,3,3} AS SET)));
    
::

           Result
    =============
                1
     
.. code-block:: sql

    --{1,2,3} seteq {1,2,3,3} returns false
    SELECT ((CAST ({3,1,2} AS SET)) SETEQ (CAST ({1,2,3,3} AS MULTISET)));
    
::

           Result
    =============
                0
     
.. code-block:: sql

    --{1,2,3} setneq {1,2,3,3} returns true
    SELECT ((CAST ({3,1,2} AS SET)) SETNEQ (CAST ({1,2,3,3} AS MULTISET)));
    
::

           Result
    =============
                1
     
.. code-block:: sql

    --{1,2,3} subseteq {1,2,3,4} returns true
    SELECT ((CAST ({3,1,2} AS SET)) SUBSETEQ (CAST ({1,2,4,4,3} AS SET)));
    
::

           Result
    =============
                1
     
.. code-block:: sql

    --{1,2,3} subseteq {1,2,3,4,4} returns true
    SELECT ((CAST ({3,1,2} AS SET)) SUBSETEQ (CAST ({1,2,4,4,3} AS MULTISET)));
    
::

           Result
    =============
                1
     
.. code-block:: sql

    --{1,2,3} subseteq {1,2,4,4,3} returns true
    SELECT ((CAST ({3,1,2} AS SET)) SUBSETEQ (CAST ({1,2,4,4,3} AS LIST)));
    
::

           Result
    =============
                0
     
.. code-block:: sql

    --{1,2,3} subseteq {1,2,3,4,4} returns true
    SELECT ((CAST ({3,1,2} AS SET)) SUBSETEQ (CAST ({1,2,3,4,4} AS LIST)));
    
::

           Result
    =============
                1
     
.. code-block:: sql

    --{3,1,2} seteq {3,1,2} returns true
    SELECT ((CAST ({3,1,2} AS LIST)) SETEQ (CAST ({3,1,2} AS LIST)));
    
::

           Result
    =============
                1
                
.. code-block:: sql

    --error occurs because LIST subseteq LIST is not supported
    SELECT ((CAST ({3,1,2} AS LIST)) SUBSETEQ (CAST ({3,1,2} AS LIST)));
         
::

    ERROR: ' subseteq ' operator is not defined on types sequence and sequence.

SETEQ
=====

**SETEQ** 연산자는 첫 번째 피연산자와 두 번째 피연산자가 동일한 경우 **TRUE** (1)을 반환한다. 모든 컬렉션 타입에 대해 비교 연산을 수행할 수 있다. ::

    collection_operand SETEQ collection_operand

.. code-block:: sql

    --creating a table with SET type address column and LIST type zip_code column
     
    CREATE TABLE contain_tbl (id INT PRIMARY KEY, name CHAR(10), address SET VARCHAR(20), zip_code LIST INT);
    INSERT INTO contain_tbl VALUES(1, 'Kim', {'country', 'state'},{1, 2, 3});
    INSERT INTO contain_tbl VALUES(2, 'Moy', {'country', 'state'},{3, 2, 1});
    INSERT INTO contain_tbl VALUES(3, 'Jones', {'country', 'state', 'city'},{1,2,3,4});
    INSERT INTO contain_tbl VALUES(4, 'Smith', {'country', 'state', 'city', 'street'},{1,2,3,4});
    INSERT INTO contain_tbl VALUES(5, 'Kim', {'country', 'state', 'city', 'street'},{1,2,3,4});
    INSERT INTO contain_tbl VALUES(6, 'Smith', {'country', 'state', 'city', 'street'},{1,2,3,5});
    INSERT INTO contain_tbl VALUES(7, 'Brown', {'country', 'state', 'city', 'street'},{});
     
    --selecting rows when two collection_operands are same in the WEHRE clause
    SELECT id, name, address, zip_code FROM contain_tbl WHERE address SETEQ {'country','state', 'city'};
    
::

               id  name                  address               zip_code
    ===============================================================================
                3  'Jones     '          {'city', 'country', 'state'}  {1, 2, 3, 4}
     
    1 row selected.
     
.. code-block:: sql

    --selecting rows when two collection_operands are same in the WEHRE clause
    SELECT id, name, address, zip_code FROM contain_tbl WHERE zip_code SETEQ {1,2,3};
    
     
::

               id  name                  address               zip_code
    ===============================================================================
                1  'Kim       '          {'country', 'state'}  {1, 2, 3}
     
    1 rows selected.

SETNEQ
======

**SETNEQ** 연산자는 첫 번째 피연산자와 두 번째 피연산자가 동일하지 않은 경우에 **TRUE** (1)을 반환한다. 모든 컬렉션 타입에 대해 비교 연산을 수행할 수 있다. ::

    collection_operand SETNEQ collection_operand

.. code-block:: sql

    --selecting rows when two collection_operands are not same in the WEHRE clause
    SELECT id, name, address, zip_code FROM contain_tbl WHERE address SETNEQ {'country','state', 'city'};
    
::

               id  name                  address               zip_code
    ===============================================================================
                1  'Kim       '          {'country', 'state'}  {1, 2, 3}
                2  'Moy       '          {'country', 'state'}  {3, 2, 1}
                4  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                5  'Kim       '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                6  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 5}
                7  'Brown     '          {'city', 'country', 'state', 'street'}  {} 
     
    6 rows selected.
     
.. code-block:: sql

    --selecting rows when two collection_operands are not same in the WEHRE clause
    SELECT id, name, address, zip_code FROM contain_tbl WHERE zip_code SETNEQ {1,2,3};
    
::

               id  name                  address               zip_code
    ===============================================================================
                2  'Moy       '          {'country', 'state'}  {3, 2, 1}
                3  'Jones     '          {'city', 'country', 'state'}  {1, 2, 3, 4}
                4  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                5  'Kim       '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                6  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 5}
                7  'Brown     '          {'city', 'country', 'state', 'street'}  {}

SUPERSET
========

**SUPERSET** 연산자는 첫 번째 피연산자가 두 번째 피연산자의 모든 원소를 포함하는 경우, 즉 두 번째 피연산자가 첫 번째 피연산자의 진부분집합인 경우 **TRUE** (1)을 반환한다. 피연산자 집합이 서로 동일한 경우에는 **FALSE** (0)을 반환한다. 단, 피연산자가 모두 **LIST** 타입인 경우에는 **SUPERSET** 연산을 지원하지 않는다. ::

    collection_operand SUPERSET collection_operand

.. code-block:: sql

    --selecting rows when the first operand is a superset of the second operand and they are not same
    SELECT id, name, address, zip_code FROM contain_tbl WHERE address SUPERSET {'country','state','city'};
    
::

               id  name                  address               zip_code
    ===============================================================================
                4  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                5  'Kim       '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                6  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 5}
                7  'Brown     '          {'city', 'country', 'state', 'street'}  {} 

.. code-block:: sql

    --SUPERSET operator cannot be used for comparison between LIST and LIST type values
    SELECT id, name, address, zip_code FROM contain_tbl WHERE zip_code SUPERSET {1,2,3};
     
::

    ERROR: ' superset ' operator is not defined on types sequence and sequence.
     
.. code-block:: sql

    --Comparing operands with a SUPERSET operator after casting LIST type as SET type
    SELECT id, name, address, zip_code FROM contain_tbl WHERE zip_code SUPERSET (CAST ({1,2,3} AS SET));
    
::

               id  name                  address               zip_code
    ===============================================================================
                3  'Jones     '          {'city', 'country', 'state'}  {1, 2, 3, 4} 
                4  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                5  'Kim       '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                6  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 5}

SUPERSETEQ
==========

**SUPERSETEQ** 연산자는 첫 번째 피연산자가 두 번째 피연산자의 모든 원소를 포함하거나 서로 동일한 경우, 즉 두 번째 피연산자가 첫 번째 피연산자의 부분집합인 경우 **TRUE** (1)를 반환한다. 단, 피연산자가 모두 **LIST** 타입인 경우에는 **SUPERSETEQ** 연산을 지원하지 않는다. ::

    collection_operand SUPERSETEQ collection_operand

.. code-block:: sql

    --selecting rows when the first operand is a superset of the second operand
    SELECT id, name, address, zip_code FROM contain_tbl WHERE address SUPERSETEQ {'country','state','city'};

::

               id  name                  address               zip_code
    ===============================================================================
                3  'Jones     '          {'city', 'country', 'state'}  {1, 2, 3, 4}
                4  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                5  'Kim       '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                6  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 5}
                7  'Brown     '          {'city', 'country', 'state', 'street'}  {}
     
.. code-block:: sql

    --SUPERSETEQ operator cannot be used for comparison between LIST and LIST type values
    SELECT id, name, address, zip_code FROM contain_tbl WHERE zip_code SUPERSETEQ {1,2,3};
     
::

    ERROR: ' superseteq ' operator is not defined on types sequence and sequence.
     
.. code-block:: sql

    --Comparing operands with a SUPERSETEQ operator after casting LIST type as SET type
    SELECT id, name, address, zip_code FROM contain_tbl WHERE zip_code SUPERSETEQ (CAST ({1,2,3} AS SET));
    
::

               id  name                  address               zip_code
    ===============================================================================
                1  'Kim       '          {'country', 'state'}  {1, 2, 3}
                3  'Jones     '          {'city', 'country', 'state'}  {1, 2, 3, 4} 
                4  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                5  'Kim       '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 4}
                6  'Smith     '          {'city', 'country', 'state', 'street'}  {1, 2, 3, 5}

SUBSET
======

**SUBSET** 연산자는 두 번째 피연산자가 첫 번째 피연산자의 모든 원소를 포함하는 경우, 즉 첫 번째 피연산자가 두 번째 피연산자의 진부분집합인 경우 **TRUE** (1)을 반환한다. 피연산자 집합이 서로 동일한 경우에는 **FALSE** (0)을 반환한다. 단, 피연산자가 모두 **LIST** 타입인 경우에는 **SUBSET** 연산을 지원하지 않는다. ::

    collection_operand SUBSET collection_operand

.. code-block:: sql

    --selecting rows when the first operand is a subset of the second operand and they are not same
    SELECT id, name, address, zip_code FROM contain_tbl WHERE address SUBSET {'country','state','city'};
    
::

               id  name                  address               zip_code
    ===============================================================================
                1  'Kim       '          {'country', 'state'}  {1, 2, 3}
                2  'Moy       '          {'country', 'state'}  {3, 2, 1}
     
    --SUBSET operator cannot be used for comparison between LIST and LIST type values
    SELECT id, name, address, zip_code FROM contain_tbl WHERE zip_code SUBSET {1,2,3};
     
::

    ERROR: ' subset ' operator is not defined on types sequence and sequence.
     
    --Comparing operands with a SUBSET operator after casting LIST type as SET type
    SELECT id, name, address, zip_code FROM contain_tbl WHERE zip_code SUBSET (CAST ({1,2,3} AS SET));
    
::

               id  name                  address               zip_code
    ===============================================================================
                7  'Brown     '          {'city', 'country', 'state', 'street'}  {}

SUBSETEQ
========

**SUBSETEQ** 연산자는 두 번째 피연산자가 첫 번째 피연산자의 모든 원소를 포함하거나 서로 동일한 경우, 즉 첫 번째 피연산자가 두 번째 피연산자의 부분집합인 경우 **TRUE** (1)을 반환한다. 단, 피연산자가 모두 **LIST** 타입인 경우에는 **SUBSETEQ** 연산을 지원하지 않는다. ::

    collection_operand SUBSETEQ collection_operand

.. code-block:: sql

    --selecting rows when the first operand is a subset of the second operand
    SELECT id, name, address, zip_code FROM contain_tbl WHERE address SUBSETEQ {'country','state','city'};
    
::

               id  name                  address               zip_code
    ===============================================================================
                1  'Kim       '          {'country', 'state'}  {1, 2, 3}
                2  'Moy       '          {'country', 'state'}  {3, 2, 1}
                3  'Jones     '          {'city', 'country', 'state'}  {1, 2, 3, 4}

.. code-block:: sql

    --SUBSETEQ operator cannot be used for comparison between LIST and LIST type values
    SELECT id, name, address, zip_code FROM contain_tbl WHERE zip_code SUBSETEQ {1,2,3};
     
::

    ERROR: ' subseteq ' operator is not defined on types sequence and sequence.

.. code-block:: sql

    --Comparing operands with a SUBSETEQ operator after casting LIST type as SET type
    SELECT id, name, address, zip_code FROM contain_tbl WHERE zip_code SUBSETEQ (CAST ({1,2,3} AS SET));

::

               id  name                  address               zip_code
    ===============================================================================
                1  'Kim       '          {'country', 'state'}  {1, 2, 3}
                7  'Brown     '          {'city', 'country', 'state', 'street'}  {}
