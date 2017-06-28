
:meta-keywords: cubrid seteq, cubrid setneq, cubrid superset, cubrid superseteq, cubrid subset, cubrid subseteq
:meta-description: Containment operators are used to check the containment relationship by performing comparison operation on operands of the collection data type.

:tocdepth: 3

*********************
Containment Operators
*********************

.. contents::

Containment operators are used to check the containment relationship by performing comparison operation on operands of the collection data type. Collection data types or subqueries can be specified as operands. The operation returns **TRUE** or **FALSE** if there is a containment relationship between the two operands of identical/different/subset/proper subset.

::

    <collection_operand> <containment_operator> <collection_operand>
     
        <collection_operand> ::=
            <set> |
            <multiset> |
            <sequence> (or <list>) |
            <subquery> |
            NULL
     
        <containment_operator> ::=
            SETEQ |
            SETNEQ |
            SUPERSET |
            SUBSET |
            SUPERSETEQ |
            SUBSETEQ

*   <*collection_operand*>: This expression that can be specified as an operand is a single SET-valued attribute, an arithmetic expression containing a SET operator or a SET value enclosed in braces. If the type is not specified, the SET value enclosed in braces is treated as a **LIST** type by default.

    Subqueries can be specified as operands. If a column which is not a collection type is searched, a collection data type keyword is required for the subquery like **SET** (*subquery*). The column retrieved by a subquery must return a single set so that it can be compared with the set of the other operands.

    If the element type of collection is an object, the OIDs, not its contents, are compared. For example, two objects with different OIDs are considered to be different even though they have the same attribute values.

    *   **NULL**: Any of operands to be compared is **NULL**, **NULL** is returned.

The description and return values about the containment operators supported by CUBRID are as follows:

**Containment Operators Supported by CUBRID**

+--------------------------+----------------------------------------------+--------------------------+------------------+
| Containment Operator     | Description                                  | Predicates               | Return Value     |
+==========================+==============================================+==========================+==================+
| A                        | A = B:                                       | {1,2} SETEQ {1,2,2}      | 0                |
| **SETEQ**                | Elements in A and B are same each other.     |                          |                  |
| B                        |                                              |                          |                  |
+--------------------------+----------------------------------------------+--------------------------+------------------+
| A                        | A <> B:                                      | {1,2} SETNEQ {1,2,3}     | 1                |
| **SETNEQ**               | Elements in A and B are not same each other. |                          |                  |
| B                        |                                              |                          |                  |
+--------------------------+----------------------------------------------+--------------------------+------------------+
| A                        | A > B:                                       | {1,2} SUPERSET {1,2,3}   | 0                |
| **SUPERSET**             | B is a proper subset of A.                   |                          |                  |
| B                        |                                              |                          |                  |
+--------------------------+----------------------------------------------+--------------------------+------------------+
| A                        | A < B:                                       | {1,2} SUBSET {1,2,3}     | 1                |
| **SUBSET**               | A is a proper subset of B.                   |                          |                  |
| B                        |                                              |                          |                  |
+--------------------------+----------------------------------------------+--------------------------+------------------+
| A                        | A >= B:                                      | {1,2} SUPERSETEQ {1,2,3} | 0                |
| **SUPERSETEQ**           | B is a subset of A.                          |                          |                  |
| B                        |                                              |                          |                  |
+--------------------------+----------------------------------------------+--------------------------+------------------+
| A                        | A <= B:                                      | {1,2} SUBSETEQ {1,2,3}   | 1                |
| **SUBSETEQ**             | A is a subset of B.                          |                          |                  |
| B                        |                                              |                          |                  |
+--------------------------+----------------------------------------------+--------------------------+------------------+

The following table shows than possibility of operation by operand and type conversion if a containment operator is used.

**Possibility of Operation by Operand**

+---------------------+--------------------+--------------------+------------------------------+
|                     | SET                | MULTISET           | LIST(=SEQUENCE)              |
+=====================+====================+====================+==============================+
| **SET**             | Operation possible | Operation possible | Operation possible           |
+---------------------+--------------------+--------------------+------------------------------+
| **MULTISET**        | Operation possible | Operation possible | Operation possible           |
|                     |                    |                    | (**LIST**                    |
|                     |                    |                    | is converted into            |
|                     |                    |                    | **MULTISET**)                |
+---------------------+--------------------+--------------------+------------------------------+
| **LIST(=SEQUENCE)** | Operation possible | Operation possible | Some operation possible      |
|                     |                    | (**LIST**          | (**SETEQ**, **SETNEQ**)      |
|                     |                    | is converted into  | Error occurs for the rest of |
|                     |                    | **MULTISET**)      | operators.                   |
+---------------------+--------------------+--------------------+------------------------------+

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

The **SETEQ** operator returns **TRUE** if the first operand is the same as the second one. It can perform comparison operator for all collection data type. ::

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

The **SETNEQ** operator returns **TRUE** (1) if a first operand is different from a second operand. A comparable operation can be performed for all collection data types. ::

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

The **SUPERSET** operator returns **TRUE** (1) when a second operand is a proper subset of a first operand; that is, the first one is larger than the second one. If two operands are identical, **FALSE** (0) is returned. Note that **SUPERSET** is not supported if all operands are **LIST** type. ::

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

The **SUPERSETEQ** operator returns **TRUE** (1) when a second operand is a subset of a first operand; that is, the first one is identical to or larger than the second one. Note that **SUPERSETEQ** is not supported if an operand is **LIST** type. ::

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

The **SUBSET** operator returns **TRUE** (1) if the second operand contains all elements of the first operand. If the first and the second collection have the same elements, **FALSE** (0) is returned. Note that both operands are the **LIST** type, the **SUBSET** operation is not supported. ::

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

The **SUBSETEQ** operator returns **TRUE** (1) when a first operand is a subset of a second operand; that is, the second one is identical to or larger than the first one. Note that **SUBSETEQ** is not supported if an operand is **LIST** type. ::

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
