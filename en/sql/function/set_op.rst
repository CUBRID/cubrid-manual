*************
Set Operators
*************

Set Arithmetic Operators
========================

To compute union, difference or intersection of collections types (**SET**, **MULTISET**, and **LIST (SEQUENCE)**), you can use +, -, or * operators, respectively. The following table shows a result data type by the operator if collection type is an operand.

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

::

    value_expression  set_arithmetic_operator value_expression
     
    value_expression :
    • collection value
    • NULL
     
    set_arithmetic_operator :
    • + (union)
    • - (difference)
    • * (intersection)

**Example**

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

**Assigning Collection Value to Variable**

For a collection value to be assigned to a variable, the outer query must return a single row as a result. The following example shows how to assign a collection value to a variable. The outer query must return only a single row as follows:

.. code-block:: sql

    SELECT SET(SELECT name
    FROM people
    WHERE ssn in {'1234', '5678'})
    TO :"names"
    FROM TABLE people;

Statement Set Operators
=======================

Statement set operators are used to get union, difference or intersection on the result of more than one query statement specified as an operand. Note that the data types of the data to be retrieved from the target tables of the two query statements must be identical or implicitly castable.

The following table shows statement set operators supported by CUBRID and their examples.

**Statement Set Operators Supported by CUBRID**

+----------------------------+-----------------------------+---------------------------------------------------------+
| Statement Set Operator     | Description                 | Note                                                    |
+============================+=============================+=========================================================+
| **UNION**                  | Union                       | Outputs all instance results containing duplicates with |
|                            | Duplicates are not allowed. | **UNION ALL**                                           |
+----------------------------+-----------------------------+---------------------------------------------------------+
| **DIFFERENCE**             | Difference                  | Same as the                                             |
|                            | Duplicates are not allowed. | **EXCEPT**                                              |
|                            |                             | operator                                                |
|                            |                             | Outputs all instance results containing duplicates with |
|                            |                             | **DIFFERENCE ALL**                                      |
+----------------------------+-----------------------------+---------------------------------------------------------+
| **INTERSECTION**           | Intersection                | Same as the                                             |
|                            | Duplicates are not allowed. | **INTERSECTION**                                        |
|                            |                             | operator                                                |
|                            |                             | Outputs all instance results containing duplicates with |
|                            |                             | **INTERSECTION ALL**                                    |
+----------------------------+-----------------------------+---------------------------------------------------------+

::

    query_term statement_set_operator [qualifier] query_term
    [{statement_set_operator [qualifier] query_term}];  
     
    query_term :
    • query_specification
    • subquery
     
    qualifier :
    • DISTINCT, DISTINCTROW 또는 UNIQUE(A returned instance is a distinct value.)
    • ALL (All instances are returned. Duplicates are allowed.)
     
    statement_set_operator :
    • UNION (union)
    • DIFFERENCE (difference)
    • INTERSECT | INTERSECTION (intersection)

**Example**

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
