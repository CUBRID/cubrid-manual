
:meta-keywords: cubrid set, cubrid multiset, cubrid list, cubrid sequence, cubrid collection type

************************
Set Arithmetic Operators
************************

SET, MULTISET, LIST
===================

To compute union, difference or intersection of collections types (**SET**, **MULTISET**, and **LIST (SEQUENCE)**), you can use +, -, or * operators, respectively. 

::

    <value_expression>  <set_arithmetic_operator> <value_expression>
     
        <value_expression> ::=
            collection_value |
            NULL
     
        <set_arithmetic_operator> ::=
            + (union) |
            - (difference) |
            * (intersection)

The following table shows a result data type by the operator if collection type is an operand.

**Result Data Type by Operand Type**

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

The following are the examples which execute arithmetic operations with collection types.

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

Assigning Collection Value to Variable
--------------------------------------

For a collection value to be assigned to a variable, the outer query must return a single row as a result. 

The following example shows how to assign a collection value to a variable. The outer query must return only a single row as follows:

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
