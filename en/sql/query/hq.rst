
:meta-keywords: start with clause, connect by clause, hierarchical pseudo column, hierarchical query example, hierarchical query function
:meta-description: Hierarchical Query is used to obtain a set of data organized in a hierarchy. The START WITH ... CONNECT BY clause is used in combination with the SELECT clause in the following form.

******************
Hierarchical Query
******************

Hierarchical Query is used to obtain a set of data organized in a hierarchy. The **START WITH ... CONNECT BY** clause is used in combination with the **SELECT** clause in the following form.

You can execute the queries by changing the order of two clauses like **CONNECT BY ... START WITH**. ::

    SELECT column_list
    FROM table_joins | tables
    [WHERE join_conditions and/or filtering_conditions]
    [hierarchical_clause]
     
    hierarchical_clause :
        [START WITH condition] CONNECT BY [NOCYCLE] condition
        | CONNECT BY [NOCYCLE] condition [START WITH condition]

START WITH Clause
=================

The **START WITH** clause will filter the rows from which the hierarchy will start. The rows that satisfy the **START WITH** condition will be the root nodes of the hierarchy. If **START WITH** is omitted, then all the rows will be considered as root nodes.

.. note::

    If **START WITH** clause is omitted or the rows that satisfy the **START WITH** condition does not exist, all of rows in the table are considered as root nodes; which means that hierarchy relationship of sub rows which belong each root is searched. Therefore, some of results can be duplicate.

CONNECT BY Clause
=================

*   **PRIOR** : The **CONNECT BY** condition is tested for a pair of rows. If it evaluates to true, the two rows satisfy the parent-child relationship of the hierarchy. We need to specify the columns that are used from the parent row and the columns that are used from the child row. We can use the **PRIOR** operator when applied to a column, which will refer to the value of the parent row for that column. If **PRIOR** is not used for a column, the value in the child row is used.

*   **NOCYCLE** : In some cases, the resulting rows of the table joins may contain cycles, depending on the **CONNECT BY** condition. Because cycles cause an infinite loop in the result tree construction, CUBRID detects them and either returns an error doesn't expand the branches beyond the point where a cycle is found (if the **NOCYCLE** keyword is specified). This keyword may be specified after the **CONNECT BY** keywords. It makes CUBRID run a statement even if the processed data contains cycles.

    If a **CONNECT BY** statement causes a cycle at runtime and the **NOCYCLE** keyword is not specified, CUBRID will return an error and the statement will be canceled. When specifying the **NOCYCLE** keyword, if CUBRID detects a cycle while processing a hierarchy node, it will set the **CONNECT_BY_ISCYCLE** attribute for that node to the value of 1 and it will stop further expansion of that branch.

The following example shows how to execute hierarchical query.

.. code-block:: sql

    -- Creating tree table and then inserting data
    CREATE TABLE tree(ID INT, MgrID INT, Name VARCHAR(32), BirthYear INT);
     
    INSERT INTO tree VALUES (1,NULL,'Kim', 1963);
    INSERT INTO tree VALUES (2,NULL,'Moy', 1958);
    INSERT INTO tree VALUES (3,1,'Jonas', 1976);
    INSERT INTO tree VALUES (4,1,'Smith', 1974);
    INSERT INTO tree VALUES (5,2,'Verma', 1973);
    INSERT INTO tree VALUES (6,2,'Foster', 1972);
    INSERT INTO tree VALUES (7,6,'Brown', 1981);
     
    -- Executing a hierarchical query with CONNECT BY clause
    SELECT id, mgrid, name
    FROM tree
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;

::
    
       id        mgrid  name
    ========================================
        1         NULL  'Kim'
        2         NULL  'Moy'
        3            1  'Jonas'
        3            1  'Jonas'
        4            1  'Smith'
        4            1  'Smith'
        5            2  'Verma'
        5            2  'Verma'
        6            2  'Foster'
        6            2  'Foster'
        7            6  'Brown'
        7            6  'Brown'
        7            6  'Brown'

.. code-block:: sql

    -- Executing a hierarchical query with START WITH clause
    SELECT id, mgrid, name
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY prior id=mgrid
    ORDER BY id;

::
    
       id        mgrid  name
    ========================================
        1         NULL  'Kim'
        2         NULL  'Moy'
        3            1  'Jonas'
        4            1  'Smith'
        5            2  'Verma'
        6            2  'Foster'
        7            6  'Brown'


Hierarchical Query Execution
============================

Hierarchical Query for Table Join
---------------------------------

When target table is joined in **SELECT** statement, **WHERE** clause can include not only searching conditions but also joining conditions. At this time, CUBRID applies the joining conditions in **WHERE** clause at first, then conditions in **CONNECT BY** clause; at last, the left searching conditions.

When specifying joining conditions and searching conditions together in **WHERE** clause, joining conditions can be applied as searching conditions even if there was no intention; so the operating order can be different; therefore, we recommend that you specify the table joining conditions in **FROM** clause, not in **WHERE** conditions.

Query Results
-------------

The resulting rows of the table joins are filtered according to the **START WITH** condition to obtain the root nodes for the hierarchy. If no **START WITH** condition is specified, then all the rows resulting from the table joins will be considered as root nodes. After the root nodes are obtained, CUBRID will select the child rows for the root nodes. These are all nodes from the table joins that respect the **CONNECT BY** condition. This step will be repeated for the child nodes to determine their child nodes and so on until no more child nodes can be added.

In addition, CUBRID evaluates the **CONNECT BY** clause first and all the rows of the resulting hierarchy tress by using the filtering condition in the **WHERE** clause.

The example illustrates how joins can be used in **CONNECT BY** queries.

.. code-block:: sql

    -- Creating tree2 table and then inserting data
    CREATE TABLE tree2(id int, treeid int, job varchar(32));
     
    INSERT INTO tree2 VALUES(1,1,'Partner');
    INSERT INTO tree2 VALUES(2,2,'Partner');
    INSERT INTO tree2 VALUES(3,3,'Developer');
    INSERT INTO tree2 VALUES(4,4,'Developer');
    INSERT INTO tree2 VALUES(5,5,'Sales Exec.');
    INSERT INTO tree2 VALUES(6,6,'Sales Exec.');
    INSERT INTO tree2 VALUES(7,7,'Assistant');
    INSERT INTO tree2 VALUES(8,null,'Secretary');
     
    -- Executing a hierarchical query onto table joins
    SELECT t.id,t.name,t2.job,level
    FROM tree t INNER JOIN tree2 t2 ON t.id=t2.treeid
    START WITH t.mgrid is null
    CONNECT BY prior t.id=t.mgrid
    ORDER BY t.id;

::
    
       id  name                  job                         level
    ==============================================================
        1  'Kim'                 'Partner'                       1
        2  'Moy'                 'Partner'                       1
        3  'Jonas'               'Developer'                     2
        4  'Smith'               'Developer'                     2
        5  'Verma'               'Sales Exec.'                   2
        6  'Foster'              'Sales Exec.'                   2
        7  'Brown'               'Assistant'                     3

Ordering Data with the Hierarchical Query
-----------------------------------------

The **ORDER SIBLINGS BY** clause will cause the ordering of the rows while preserving the hierarchy ordering so that the child nodes with the same parent will be stored according to the column list. ::

    ORDER SIBLINGS BY col_1 [ASC|DESC] [, col_2 [ASC|DESC] [...[, col_n [ASC|DESC]]...]]

The following example shows how to display information about seniors and subordinates in a company in the order of birth year.

The result with hierarchical query shows parent and child nodes in a row according to the column list specified in **ORDER SIBLINGS BY** statement by default. Sibling nodes that share the same parent node have outputted in a specified order.

.. code-block:: sql

    -- Outputting a parent node and its child nodes, which sibling nodes that share the same parent are sorted in the order of birthyear.
    SELECT id, mgrid, name, birthyear, level
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER SIBLINGS BY birthyear;

::
    
       id        mgrid  name                    birthyear        level
    ==================================================================
        2         NULL  'Moy'                        1958            1
        6            2  'Foster'                     1972            2
        7            6  'Brown'                      1981            3
        5            2  'Verma'                      1973            2
        1         NULL  'Kim'                        1963            1
        4            1  'Smith'                      1974            2
        3            1  'Jonas'                      1976            2

The following example shows how to display information about seniors and subordinates in a company in the order of joining. For the same level, the employee ID numbers are assigned in the order of joining. *id* indicates employee ID numbers (parent and child nodes) and *mgrid* indicates the employee ID numbers of their seniors.

.. code-block:: sql

    -- Outputting a parent node and its child nodes, which sibling nodes that share the same parent are sorted in the order of id.
    SELECT id, mgrid, name, LEVEL
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER SIBLINGS BY id;

::
    
       id        mgrid  name                        level
    =====================================================
        1         NULL  'Kim'                           1
        3            1  'Jonas'                         2
        4            1  'Smith'                         2
        2         NULL  'Moy'                           1
        5            2  'Verma'                         2
        6            2  'Foster'                        2
        7            6  'Brown'                         3

Pseudo Columns for Hierarchical Query
=====================================

LEVEL
-----

**LEVEL** is a pseudocolumn representing depth of hierarchical queries. The **LEVEL** of root node is 1 and the LEVEL of its child node is 2.

The **LEVEL** (pseudocolumn) can be used in the **WHERE** clause, **ORDER BY** clause, and **GROUP BY ... HAVING** clause of the **SELECT** statement. And it can also be used in the statement using aggregate functions.

The following example shows how to retrieve the **LEVEL** value to check level of node.

.. code-block:: sql

    -- Checking the LEVEL value
    SELECT id, mgrid, name, LEVEL
    FROM tree
    WHERE LEVEL=2
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;

::

       id        mgrid  name                        level
    =====================================================
        3            1  'Jonas'                         2
        4            1  'Smith'                         2
        5            2  'Verma'                         2
        6            2  'Foster'                        2

The following example shows how to add **LEVEL** conditions after the **CONNECT BY** statement.

.. code-block:: sql

    SELECT LEVEL FROM db_root CONNECT BY LEVEL <= 10;

::

            level
    =============
                1
                2
                3
                4
                5
                6
                7
                8
                9
               10

Note that the format of "CONNECT BY expr(LEVEL) < expr", for example "CONNECT BY LEVEL +1 < 5") is not supported.

CONNECT_BY_ISLEAF
-----------------

**CONNECT_BY_ISLEAF** is a pseudocolumn representing that the result of hierarchical query is leaf node. If the current row is a leaf node, it returns 1; otherwise, it returns 0.

The following example shows how to retrieve the **CONNECT_BY_ISLEAF** value to check whether it is a leaf node or not.

.. code-block:: sql

    -- Checking a CONNECT_BY_ISLEAF value
    SELECT id, mgrid, name, CONNECT_BY_ISLEAF
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;
     
::

      id        mgrid  name                  connect_by_isleaf
    =============================================================
       1         NULL  'Kim'                                 0
       2         NULL  'Moy'                                 0
       3            1  'Jonas'                               1
       4            1  'Smith'                               1
       5            2  'Verma'                               1
       6            2  'Foster'                              0
       7            6  'Brown'                               1

CONNECT_BY_ISCYCLE
------------------

**CONNECT_BY_ISCYCLE** is a pseudocolumn representing that a cycle was detected while processing the node, meaning that a child was also found to be an ancestor. A value of 1 for a row means a cycle was detected; the pseudo-column's value is 0, otherwise.

The **CONNECT_BY_ISCYCLE** pseudo-column can be used in the **WHERE**, **ORDER BY** and **GROUP BY** ... **HAVING** clauses of the **SELECT** statement. It can also be used in aggregate functions.

.. note:: This pseudocolumn is available only when the **NOCYCLE** keyword is used in the statement.

The following example shows how to retrieve the **CONNECT_BY_ISCYCE** value to check a row that occurs loop.

.. code-block:: sql

    -- Creating a tree_cycle table and inserting data
    CREATE TABLE tree_cycle(ID INT, MgrID INT, Name VARCHAR(32));
     
    INSERT INTO tree_cycle VALUES (1,NULL,'Kim');
    INSERT INTO tree_cycle VALUES (2,11,'Moy');
    INSERT INTO tree_cycle VALUES (3,1,'Jonas');
    INSERT INTO tree_cycle VALUES (4,1,'Smith');
    INSERT INTO tree_cycle VALUES (5,3,'Verma');
    INSERT INTO tree_cycle VALUES (6,3,'Foster');
    INSERT INTO tree_cycle VALUES (7,4,'Brown');
    INSERT INTO tree_cycle VALUES (8,4,'Lin');
    INSERT INTO tree_cycle VALUES (9,2,'Edwin');
    INSERT INTO tree_cycle VALUES (10,9,'Audrey');
    INSERT INTO tree_cycle VALUES (11,10,'Stone');
     
    -- Checking a CONNECT_BY_ISCYCLE value
    SELECT id, mgrid, name, CONNECT_BY_ISCYCLE
    FROM tree_cycle
    START WITH name in ('Kim', 'Moy')
    CONNECT BY NOCYCLE PRIOR id=mgrid
    ORDER BY id;
     
::

    id        mgrid  name        connect_by_iscycle
    ==================================================
     1         NULL  'Kim'                        0
     2           11  'Moy'                        0
     3            1  'Jonas'                      0
     4            1  'Smith'                      0
     5            3  'Verma'                      0
     6            3  'Foster'                     0
     7            4  'Brown'                      0
     8            4  'Lin'                        0
     9            2  'Edwin'                      0
    10            9  'Audrey'                     0
    11           10  'Stone'                      1

Operators for Hierarchical Query
================================

CONNECT_BY_ROOT
---------------

The **CONNECTION_BY_ROOT** operator returns the value of a root row as a column value. This operator can be used in the **WHERE** and **ORDER BY** clauses of the **SELECT** statement.

The following example shows how to retrieve the root row's *id* value.

.. code-block:: sql

    -- Checking the id value of a root row for each row
    SELECT id, mgrid, name, CONNECT_BY_ROOT id
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;
     
::

       id        mgrid  name                   connect_by_root id
    =============================================================
        1         NULL  'Kim'                                   1
        2         NULL  'Moy'                                   2
        3            1  'Jonas'                                 1
        4            1  'Smith'                                 1
        5            2  'Verma'                                 2
        6            2  'Foster'                                2
        7            6  'Brown'                                 2

.. _prior-operator:

PRIOR
-----

The PRIOR operator returns the value of a parent row as a column value and returns NULL for the root row. This operator can be used in the **WHERE**, **ORDER BY** and **CONNECT BY** clauses of the **SELECT** statement.

The following example shows how to retrieve the parent row's *id* value.

.. code-block:: sql

    -- Checking the id value of a parent row for each row
    SELECT id, mgrid, name, PRIOR id as "prior_id"
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;
     
::

       id        mgrid  name                     prior_id
    =====================================================
        1         NULL  'Kim'                        NULL
        2         NULL  'Moy'                        NULL
        3            1  'Jonas'                         1
        4            1  'Smith'                         1
        5            2  'Verma'                         2
        6            2  'Foster'                        2
        7            6  'Brown'                         6

Functions for Hierarchical Query
================================

SYS_CONNECT_BY_PATH
-------------------

The **SYS_CONNECT_BY_PATH** function returns the hierarchical path from a root to the specified row in string. The column and separator specified as an argument must be a character type. Each path separated by specified separator will be displayed consecutively. This function can be used in the **WHERE** and **ORDER BY** clauses of the **SELECT** statement. ::

    SYS_CONNECT_BY_PATH (column_name, separator_char)

The following example shows how to retrieve path from a root to the specified row.

.. code-block:: sql

    -- Executing a hierarchical query with SYS_CONNECT_BY_PATH function
    SELECT id, mgrid, name, SYS_CONNECT_BY_PATH(name,'/') as [hierarchy]
    FROM tree
    START WITH mgrid IS NULL
    CONNECT BY PRIOR id=mgrid
    ORDER BY id;
     
::

       id        mgrid  name                  hierarchy
    ==============================================================
        1         NULL  'Kim'                 '/Kim'
        2         NULL  'Moy'                 '/Moy'
        3            1  'Jonas'               '/Kim/Jonas'
        4            1  'Smith'               '/Kim/Smith'
        5            2  'Verma'               '/Moy/Verma'
        6            2  'Foster'              '/Moy/Foster'
        7            6  'Brown'               '/Moy/Foster/Brown'

Examples of Hierarchical Query
==============================

The examples in this page shows how to write hierarchical queries by specifying the **CONNECT BY** clause within the **SELECT** statement.

A table that have relationship with recursive reference is create and the table consists of two columns named *ID* and *ParentID*; assume that *ID* is a primary key for the table and *ParentID* is a foreign key for the same table. In this context, the root node will have a *ParentID* value of **NULL**.

Once a table is create, you can get the entire data with hierarchical structure and a value of **LEVEL** by using the **UNION ALL** as shown below.

.. code-block:: sql

    CREATE TABLE tree_table (ID int PRIMARY KEY, ParentID int, name VARCHAR(128));
    
    INSERT INTO tree_table VALUES (1,NULL,'Kim');
    INSERT INTO tree_table VALUES (2,1,'Moy');
    INSERT INTO tree_table VALUES (3,1,'Jonas');
    INSERT INTO tree_table VALUES (4,1,'Smith');
    INSERT INTO tree_table VALUES (5,3,'Verma');
    INSERT INTO tree_table VALUES (6,3,'Foster');
    INSERT INTO tree_table VALUES (7,4,'Brown');
    INSERT INTO tree_table VALUES (8,4,'Lin');
    INSERT INTO tree_table VALUES (9,2,'Edwin');
    INSERT INTO tree_table VALUES (10,9,'Audrey');
    INSERT INTO tree_table VALUES (11,10,'Stone');
    
    SELECT L1.ID, L1.ParentID, L1.name, 1 AS [Level]
        FROM tree_table AS L1
        WHERE L1.ParentID IS NULL
    UNION ALL
    SELECT L2.ID, L2.ParentID, L2.name, 2 AS [Level]
        FROM tree_table AS L1
            INNER JOIN tree_table AS L2 ON L1.ID=L2.ParentID
        WHERE L1.ParentID IS NULL
    UNION ALL
    SELECT L3.ID, L3.ParentID, L3.name, 3 AS [Level]
        FROM tree_table AS L1
            INNER JOIN tree_table AS L2 ON L1.ID=L2.ParentID
            INNER JOIN tree_table AS L3 ON L2.ID=L3.ParentID
        WHERE L1.ParentID IS NULL
    UNION ALL
    SELECT L4.ID, L4.ParentID, L4.name, 4 AS [Level]
        FROM tree_table AS L1
            INNER JOIN tree_table AS L2 ON L1.ID=L2.ParentID
            INNER JOIN tree_table AS L3 ON L2.ID=L3.ParentID
            INNER JOIN tree_table AS L4 ON L3.ID=L4.ParentID
        WHERE L1.ParentID IS NULL;

::

       ID     ParentID  name                        Level
    =====================================================
        1         NULL  'Kim'                           1
        2            1  'Moy'                           2
        3            1  'Jonas'                         2
        4            1  'Smith'                         2
        9            2  'Edwin'                         3
        5            3  'Verma'                         3
        6            3  'Foster'                        3
        7            4  'Brown'                         3
        8            4  'Lin'                           3
       10            9  'Audrey'                        4

Because you do not know how many levels exist in the data, you can rewrite the query above as a stored procedure that loops until no new row is retrieved.

However, the hierarchical structure should be checked every step while looping, specify the **CONNECT BY** clause within the **SELECT** statement as follows; the example below shows how to get the entire data with hierarchical structure and the level of each row in the hierarchy.

.. code-block:: sql

    SELECT ID, ParentID, name, Level
    FROM tree_table
    START WITH ParentID IS NULL
    CONNECT BY ParentID=PRIOR ID;

::

       ID     ParentID  name                        level
    =====================================================
        1         NULL  'Kim'                           1
        2            1  'Moy'                           2
        9            2  'Edwin'                         3
       10            9  'Audrey'                        4
       11           10  'Stone'                         5
        3            1  'Jonas'                         2
        5            3  'Verma'                         3
        6            3  'Foster'                        3
        4            1  'Smith'                         2
        7            4  'Brown'                         3
        8            4  'Lin'                           3

You can specify **NOCYCLE** to prevent an error from occurring as follows. When you run the following query, a loop does not appear; therefore, the result is the same as the above.

.. code-block:: sql

    SELECT ID, ParentID, name, Level
    FROM tree_table
    START WITH ParentID IS NULL
    CONNECT BY NOCYCLE ParentID=PRIOR ID;

CUBRID judges that this hierarchical query has a loop if the same row is found during searching process on the query. The below is an example that the loop exists; we define **NOCYCLE** to end the additional searching process if a loop exists.

.. code-block:: sql

    CREATE TABLE tbl(seq INT, id VARCHAR(10), parent VARCHAR(10));
    
    INSERT INTO tbl VALUES (1, 'a', null);
    INSERT INTO tbl VALUES (2, 'b', 'a');
    INSERT INTO tbl VALUES (3, 'b', 'c');
    INSERT INTO tbl VALUES (4, 'c', 'b');
    INSERT INTO tbl VALUES (5, 'c', 'b');
    
    SELECT seq, id, parent, LEVEL,
      CONNECT_BY_ISCYCLE AS iscycle,
      CAST(SYS_CONNECT_BY_PATH(id,'/') AS VARCHAR(10)) AS idpath
    FROM tbl
    START WITH PARENT is NULL
    CONNECT BY NOCYCLE PARENT = PRIOR id;

::

        seq  id           parent       level      iscycle  idpath
    =============================================================================
          1  'a'          NULL             1            0  '/a'
          2  'b'          'a'              2            0  '/a/b'
          4  'c'          'b'              3            0  '/a/b/c'
          3  'b'          'c'              4            1  '/a/b/c/b'
          5  'c'          'b'              5            1  '/a/b/c/b/c'
          5  'c'          'b'              3            0  '/a/b/c'
          3  'b'          'c'              4            1  '/a/b/c/b'
          4  'c'          'b'              5            1  '/a/b/c/b/c'

The below shows to output dates of March, 2013(201303) with a hierarchical query.     

.. code-block:: sql

    SELECT TO_CHAR(base_month + lvl -1, 'YYYYMMDD') h_date
    FROM (
        SELECT LEVEL lvl, base_month
        FROM ( 
                SELECT TO_DATE('201303', 'YYYYMM') base_month FROM db_root
        )
        CONNECT BY LEVEL <= LAST_DAY(base_month) - base_month + 1
    );

::

    h_date
    ======================
      '20130301'
      '20130302'
      '20130303'
      '20130304'
      '20130305'
      '20130306'
      '20130307'
      '20130308'
      '20130309'
      '20130310'
      '20130311'
      '20130312'
      '20130313'
      '20130314'
      '20130315'
      '20130316'
      '20130317'
      '20130318'
      '20130319'
      '20130320'
      '20130321'
      '20130322'
      '20130323'
      '20130324'
      '20130325'
      '20130326'
      '20130327'
      '20130328'
      '20130329'
      '20130330'
      '20130331'

    31 rows selected. (0.066175 sec) Committed.

Performance of Hierarchical Query
=================================

Although this form is shorter and clearer, please keep in mind that it has its limitations regarding speed.

If the result of the query contains all the rows of the table, the **CONNECT BY** form might be slower as it has to do additional processing (such as cycle detection, pseudo-column bookkeeping and others). However, if the result of the query only contains a part of the table rows, the **CONNECT BY** form might be faster.

For example, if we have a table with 20,000 records and we want to retrieve a sub-tree of roughly 1,000 records, a **SELECT** statement with a **START WITH ... CONNECT BY** clause will run up to 30% faster than an equivalent **UNION ALL** with **SELECT** statements.
