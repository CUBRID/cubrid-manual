
:meta-keywords: union statement, difference statement, intersection statement
:meta-description: Statement set operators are used to get union, difference or intersection on the result of more than one query statement specified as an operand.

***********************
Statement Set Operators
***********************

UNION, DIFFERENCE, INTERSECTION
===============================

Statement set operators are used to get union, difference or intersection on the result of more than one query statement specified as an operand. Note that the data types of the data to be retrieved from the target tables of the two query statements must be identical or implicitly castable.

::

    query_term statement_set_operator [qualifier] <query_term>
    [{statement_set_operator [qualifier] <query_term>}];  
     
        <query_term> ::=
            query_specification
            subquery
     
*   *qualifier*

    *   DISTINCT, DISTINCTROW or UNIQUE(A returned instance is a distinct value.)
    *   ALL (All instances are returned. Duplicates are allowed.)
     
*   *statement_set_operator*

    *   UNION (union)
    *   DIFFERENCE (difference)
    *   INTERSECT | INTERSECTION (intersection)

The following table shows statement set operators supported by CUBRID.

**Statement Set Operators**

+------------------------+-----------------------------+---------------------------------------------------------+
| Statement Set Operator | Description                 | Note                                                    |
+========================+=============================+=========================================================+
| **UNION**              | Union                       | Outputs all instance results containing duplicates with |
|                        | Duplicates are not allowed. | **UNION ALL**                                           |
+------------------------+-----------------------------+---------------------------------------------------------+
| **DIFFERENCE**         | Difference                  | Same as the                                             |
|                        | Duplicates are not allowed. | **EXCEPT** operator.                                    |
|                        |                             | Outputs all instance results containing duplicates with |
|                        |                             | **DIFFERENCE ALL**.                                     |
+------------------------+-----------------------------+---------------------------------------------------------+
| **INTERSECTION**       | Intersection                | Same as the                                             |
|                        | Duplicates are not allowed. | **INTERSECTION** operator.                              |
|                        |                             | Outputs all instance results containing duplicates with |
|                        |                             | **INTERSECTION ALL**.                                   |
+------------------------+-----------------------------+---------------------------------------------------------+

The following are the examples which execute queries with statement set operators.

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
