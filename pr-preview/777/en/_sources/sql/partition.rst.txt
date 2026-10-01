
:meta-keywords: cubrid partition, partitioning key, range partition, hash partition, list partition, partition pruning
:meta-description: Partitioning is a method by which a table is divided into multiple independent physical units called partitions. In CUBRID, each partition is a table implemented as a subclass of the partitioned table.

************
Partitioning
************

.. _partitioning-key:

Partitioning key
================

The partitioning key is an expression which is used by the partitioning method to distribute data across defined partitions. The following data types are supported for the partitioning key:

*   **CHAR**
*   **VARCHAR**
*   **SMALLINT**
*   **INT**
*   **BIGINT**
*   **DATE**
*   **TIME**
*   **TIMESTAMP**
*   **DATETIME**

The following restrictions apply to the partitioning key:

*   The partitioning key must use exactly one column from the partitioned table.
*   :doc:`Aggregate functions, analytic functions<function/analysis_fn>`, :doc:`logical operators<function/logical_op>` and :doc:`comparison operators<function/comparison_op>` are not allowed in the partitioning key expression.
*   The following functions and expressions are not allowed in the partitioning key expression:

    *   :ref:`CASE <case-expr>` 
    *   :func:`CHARSET` 
    *   :func:`CHR` 
    *   :func:`COALESCE` 
    *   :func:`SERIAL_CURRENT_VALUE` 
    *   :func:`SERIAL_NEXT_VALUE` 
    *   :func:`DECODE`
    *   :func:`DECR` 
    *   :func:`INCR`
    *   :func:`DRAND` 
    *   :func:`DRANDOM` 
    *   :func:`GREATEST` 
    *   :func:`LEAST` 
    *   :func:`IF` 
    *   :func:`IFNULL` 
    *   :func:`INSTR` 
    *   :func:`NVL` 
    *   :func:`NVL2` 
    *   :c:macro:`ROWNUM` 
    *   :func:`INST_NUM` 
    *   :c:macro:`USER` 
    *   :ref:`PRIOR <prior-operator>` 
    *   :func:`WIDTH_BUCKET`
*	The partitioning key needs to be present in the key of each unique index (including primary keys). For more information on this aspect, please see :ref:`here<index-partitions>`.
*	The partitioning expression's length must not exceed 1024 bytes.

.. _range-partitioning:

Range Partitioning
==================

Range partitioning is a partitioning method in which a table is partitioned using a user specified range of values of the partitioning key for each partition. Ranges are defined as continuous non-overlapping intervals. This partitioning method is most useful when table data can be divided into range intervals (e.g. order placement date for an orders table or age intervals for a user's table). Range partitioning is the most versatile partitioning method in terms of :ref:`partition-pruning` because almost all search predicates can be used to identify matching ranges.

Tables can be partitioned by range by using the **PARTITION BY RANGE** clause in **CREATE** or **ALTER** statements. ::

    CREATE TABLE [schema_name.]table_name (
       ...
    )
    PARTITION BY RANGE ( <partitioning_key> ) (
        PARTITION partition_name VALUES LESS THAN ( <range_value> ) [COMMENT 'comment_string'] ,
        PARTITION partition_name VALUES LESS THAN ( <range_value> ) [COMMENT 'comment_string'] ,
        ... 
    )
    
    ALTER TABLE [schema_name.]table_name 
    PARTITION BY RANGE ( <partitioning_key> ) (
        PARTITION partition_name VALUES LESS THAN ( <range_value> ) [COMMENT 'comment_string'] ,
        PARTITION partition_name VALUES LESS THAN ( <range_value> ) [COMMENT 'comment_string'] ,
        ... 
    )

*   *partitioning_key* : specifies the :ref:`partitioning-key`.
*   *partition_name* : specifies the partition name.
*   *range_value* : specifies the upper limit of the partitioning key value. All tuples for which the evaluation of partitioning key is less than (but not equal to) the *range_value* will be stored in this partition. 
*   *comment_string*: specifies a comment for each partition.

The following example shows how to create the *participant2* table which holds countries participating at the Olympics and partition this table into partitions holding participants before year 2000(*before_2000* partition) and participants before year 2008(*before_2008* partition):

.. _range-participant2-table:

.. code-block:: sql

    CREATE TABLE participant2 (
        host_year INT, 
        nation CHAR(3), 
        gold INT, 
        silver INT, 
        bronze INT
    )
    PARTITION BY RANGE (host_year) (
        PARTITION before_2000 VALUES LESS THAN (2000),
        PARTITION before_2008 VALUES LESS THAN (2008)
    );

When creating partitions, CUBRID sorts the user supplied range values from smallest to largest and creates the non-overlapping intervals from the sorted list. In the above example, the created range intervals are [-inf, 2000) and [2000, 2008). The identifier **MAXVALUE** can be used to specify an infinite upper limit for a partition. 

.. code-block:: sql

    ALTER TABLE participant2 ADD PARTITION (
      PARTITION before_2012 VALUES LESS THAN (2012),
      PARTITION last_one VALUES LESS THAN MAXVALUE
    );

When inserting a tuple into a range-partitioned table, CUBRID identifies the range to which the tuple belongs by evaluating the partitioning key. If the partitioning key value is **NULL**, the data is stored in the partition with the smallest specified range value. If there is no range which would accept the partitioning key value, CUBRID returns an error. CUBRID also returns an error when updating a tuple if the new value of the partitioning key does not belong to any of the defined ranges.

The below is an example to add a comment for each partition.

.. code-block:: sql

    CREATE TABLE tbl (a int, b int) PARTITION BY RANGE(a) (
        PARTITION less_1000 VALUES LESS THAN (1000) COMMENT 'less 1000 comment', 
        PARTITION less_2000 VALUES LESS THAN (2000) COMMENT 'less 2000 comment'
    );

    ALTER TABLE tbl PARTITION BY RANGE(a) (
        PARTITION less_1000 VALUES LESS THAN (1000) COMMENT 'new partition comment');

To see a partition comment, refer to :ref:`show-partition-comment`.

.. _hash-partitioning:

Hash Partitioning
=================

Hash partitioning is a partitioning method which is used to distribute data across a specified number of partition. This partitioning method is useful when table data contains values for which ranges or lists would be meaningless (for example, a keywords table or an users table for which user_id is the most interesting value). If the values for the partitioning key are evenly distributed across the table data, hash-partitioning technique divides table data evenly between the defined partitions. For hash partitioning, :ref:`partition-pruning` can only be applied on equality predicates (e.g. predicates using **=** and :ref:`IN <in-expr>` expressions), making hash partitioning useful only if most of the queries specify such a predicate for the partitioning key. 

Tables can be partitioned by hash by using the **PARTITION BY HASH** clause in **CREATE** or **ALTER** statements::

    CREATE TABLE [schema_name.]table_name (
       ...
    )
    PARTITION BY HASH ( <partitioning_key> )
    PARTITIONS ( number_of_partitions )

    ALTER TABLE [schema_name.]table_name 
    PARTITION BY HASH (<partitioning_key>)
    PARTITIONS (number_of_partitions)

*   *partitioning_key* : Specifies the :ref:`partitioning-key`.
*   *number_of_partitions* : Specifies the number of partitions to be created.

The following example shows how to create the *nation2* table with country *code* and country names, and define 4 hash partitions based on code values. Only the number of partitions, not the name, is defined in hash partitioning.

.. _hash-nation2-table:

.. code-block:: sql

    CREATE TABLE nation2 (
      code CHAR (3),
      name VARCHAR (50)
    )
    PARTITION BY HASH (code) PARTITIONS 4;

When a value is inserted into a hash-partitioned table, the partition to store the data is determined by the hash value of the partitioning key. If the partitioning key value is **NULL**, the data is stored in the first partition.

.. _list-partitioning:

List Partitioning
=================

List partitioning is a partitioning method in which a table is divided into partitions according to user specified list of values for the partitioning key. The lists of values for partitions must be disjoint sets. This partitioning method is useful when table data can be divided into lists of possible values which have a certain meaning (e.g. department id for an employees table or country code for a user's table). As for hash partitioning, :ref:`partition-pruning` for list partitioned tables can only be applied on equality predicates (e.g. predicates using **=** and :ref:`IN <in-expr>` expressions). 

Tables can be partitioned by list by using the **PARTITION BY LIST** clause in **CREATE** or **ALTER** statements::

    CREATE TABLE [schema_name.]table_name (
      ...
    )
    PARTITION BY LIST ( <partitioning_key> ) (
      PARTITION partition_name VALUES IN ( <values_list> ) [COMMENT 'comment_string'],
      PARTITION partition_name VALUES IN ( <values_list> ) [COMMENT 'comment_string'],
      ... 
    )
    
    ALTER TABLE [schema_name.]table_name
    PARTITION BY LIST ( <partitioning_key> ) (
      PARTITION partition_name VALUES IN ( <values_list> ) [COMMENT 'comment_string'],
      PARTITION partition_name VALUES IN ( <values_list> ) [COMMENT 'comment_string'],
      ... 
    )

*   *partitioning_key*: specifies the :ref:`partitioning-key`.
*   *partition_name*: specifies the partition name.
*   *value_list*: specifies the list of values for the partitioning key.
*   *comment_string*: specifies a comment for each partition.

The following example shows how to create the *athlete2* table with athlete names and sport events, and define list partitions based on event values.

.. _list-athlete2-table:

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR (30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics'),
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing'),
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball')
    );

When inserting a tuple into a list-partitioned table, the value of the partitioning key must belong to one of the value lists defined for partitions. For this partitioning model, CUBRID does not automatically assign a partition for **NULL** values of the partitioning key. To be able to store **NULL** values into a list-partitioned table, a partition which includes the **NULL** value in the values list must be created:

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR (30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics' ),
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing'),
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball', NULL)
    );

The below is examples of adding comments for each partition.

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR (30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics') COMMENT 'G1',
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing') COMMENT 'G2',
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball') COMMENT 'G3');

    CREATE TABLE athlete3 (name VARCHAR (40), event VARCHAR (30));
    ALTER TABLE athlete3 PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Handball', 'Volleyball', 'Tennis') COMMENT 'G1');


.. _show-partition-comment:

COMMENT of Partition
--------------------

A partition's comment can be written only for the range partition and the list partition. You cannot write the comment about the hash partition. The partition comment can be shown by running this syntax.

.. code-block:: sql

    SHOW CREATE TABLE [schema_name.]table_name;
    SELECT class_name, partition_name, COMMENT FROM db_partition WHERE class_name ='table_name';

Or you can use CSQL interpreter by running ;sc command.

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc tbl

.. _partition-pruning:

Partition Pruning
=================

Partition pruning is an optimization method, limiting the scope of a search on a partitioned table by eliminating partitions. During partition pruning, CUBRID examines the **WHERE** clause of the query to identify partitions for which this clause is always false, as considering the way partitioning was defined. In the following example, the **SELECT** query will only be applied to partitions *before_2008* and *before_2012*, since CUBRID knows that the rest of partitions hold data for which *YEAR (opening_date)* is less than 2004.

.. code-block:: sql

    CREATE TABLE olympic2 (opening_date DATE, host_nation VARCHAR (40))
    PARTITION BY RANGE (YEAR(opening_date)) (
        PARTITION before_1996 VALUES LESS THAN (1996),
        PARTITION before_2000 VALUES LESS THAN (2000),
        PARTITION before_2004 VALUES LESS THAN (2004),
        PARTITION before_2008 VALUES LESS THAN (2008),
        PARTITION before_2012 VALUES LESS THAN (2012)
    );
     
    SELECT opening_date, host_nation 
    FROM olympic2 
    WHERE YEAR(opening_date) > 2004;

Partition pruning greatly reduces the disk I/O and the amount of data which must be processed during query execution. It is important to understand when pruning is performed in order to fully benefit from it. In order for CUBRID to successfully prune partitions, the following conditions have to be met:

*   Partitioning key must be used in the *WHERE* clause directly (without applying other expressions to it)
*   For range-partitioning, the partitioning key must be used in range predicates (**<**, **>**, **BETWEEN**, etc) or equality predicates (**=**, **IN**, etc).
*   For list and hash partitioning, the partitioning key must be used in equality predicates (**=**, **IN**, etc).

The following queries explain how pruning is performed on the *olympic2* table from the example above:

.. code-block:: sql

    -- prune all partitions except before_2012
    SELECT host_nation 
    FROM olympic2 
    WHERE YEAR (opening_date) >= 2008;

    -- prune all partitions except before_2008
    SELECT host_nation 
    FROM olympic2 
    WHERE YEAR(opening_date) BETWEEN 2005 and 2007;

    -- no partition is pruned because partitioning key is not used
    SELECT host_nation 
    FROM olympic2 
    WHERE opening_date = '2008-01-02';

    -- no partition is pruned because partitioning key is not used directly
    SELECT host_nation 
    FROM olympic2 
    WHERE YEAR(opening_date) + 1 = 2008;

    -- no partition is pruned because there is no useful predicate in the WHERE clause
    SELECT host_nation 
    FROM olympic2 
    WHERE YEAR(opening_date) != 2008;

.. note:: In versions older than CUBRID 9.0, partition pruning was performed during query compilation stage. Starting with CUBRID 9.0, partition pruning is performed during the query execution stage, because executing partition pruning during query execution allows CUBRID to apply this optimization on much more complex queries. However, pruning information is not displayed in query planning stage anymore, since query planning happens before query execution and this information is not available at that time.

Users can also access partitions directly (independent of the partitioned table) either by using the table name assigned by CUBRID to a partition or by using the *table PARTITION (name)* clause:

.. code-block:: sql

    -- to specify a partition with its table name
    SELECT * FROM olympic2__p__before_2008;
    
    -- to specify a partition with PARTITION clause
    SELECT * FROM olympic2 PARTITION (before_2008);

Both of the queries above access partition *before_2008* as if it were a normal table (not a partition). This is a very useful feature because it allows certain query optimizations to be used even though they are disabled on partitioned tables (see :ref:`partitioning-notes` for more info). Users should note that, when accessing partitions directly, the scope of the query is limited to that partition. This means that tuples from other partitions are not considered (even though the **WHERE** clause includes them) and, for **INSERT** and **UPDATE** statements, if the tuple inserted/updated does not belong to the specified partition, an error is returned.

By executing queries on a partition rather than the partitioned table, some of the benefits of partitioning are lost. For example, if users only execute queries on the partitioned table, this table can be repartitioned or partitions can be dropped without having to modify the user application. If users access partitions directly, this benefit is lost. Users should also note that, even though using partitions in **INSERT** statements is allowed (for consistency), it is discouraged because there is no performance gain from it.

Partitioning Management
=======================

Partitioned tables can be managed using partition specific clauses of the **ALTER** statement. CUBRID allows several actions to be performed on partitions:

1. :ref:`Modifying a partitioned table into a regular table<remove-partitioning>`.
#. :ref:`Partitions reorganization<reorganize-partitions>`.
#. :ref:`Adding partitions to an already partitioned table<add-partitions>`.
#. :ref:`Dropping partitions<drop-partitions>`.
#. :ref:`Promote partitions to regular tables<promote-partitions>`.

.. _remove-partitioning:

Modifying a Partitioned Table into a Regular Table
--------------------------------------------------

Changing a partitioned table into a regular table can be done using the **REMOVE PARTITIONING** clause of the **ALTER** statement::

    ALTER [TABLE | CLASS] [schema_name.]table_name REMOVE PARTITIONING

*   *schema_name*: Specifies the schema name of the table. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of the table to be altered.

When removing partitioning, CUBRID moves all data from partitions into the partitioned table. This is a costly operation and should be carefully planned.

.. _reorganize-partitions:

Partition Reorganization
------------------------

Partition reorganization is a process through which a partition can be divided into smaller partitions or a group of partitions can be merged into a single partition. For this purpose, CUBRID implements the **REORGANIZE PARTITION** clause of the **ALTER** statement::

    ALTER [TABLE | CLASS] [schema_name.]table_name
    REORGANIZE PARTITION <alter_partition_name_comma_list>
    INTO "(" <partition_definition_comma_list> ")"
     
    partition_definition_comma_list ::=
    PARTITION partition_name VALUES LESS THAN "(" <range_value> ")", ... 

*   *schema_name*: Specifies the schema name of the table. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of the table to be redefined.
*   *alter_partition_name_comma_list* : Specifies the partition to be redefined(current partitions). Multiple partitions are separated by commas (,).
*   *partition_definition_comma_list* : Specifies the redefined partitions(new partitions). Multiple partitions are separated by commas (,).

This clause applies only to range and list partitioning. Since data distribution in hash-partitioning method is semantically different, hash-partitioned tables only allow adding and dropping partitions. See :ref:`hash-reorganization` for details.

The following example shows how to reorganize the *before_2000* partition of the :ref:`participant2<range-participant2-table>` table into the *before_1996* and *before_2000* partitions.

.. code-block:: sql
     
    ALTER TABLE participant2 
    REORGANIZE PARTITION before_2000 INTO (
      PARTITION before_1996 VALUES LESS THAN (1996),
      PARTITION before_2000 VALUES LESS THAN (2000)
    );

The following example shows how to merge the two partitions defined in the above example back into a single *before_2000* partition.

.. code-block:: sql

    ALTER TABLE participant2 
    REORGANIZE PARTITION before_1996, before_2000 INTO (
      PARTITION before_2000 VALUES LESS THAN (2000)
    );

The following example shows how to reorganize partitions defined on the :ref:`athlete2<list-athlete2-table>`, dividing the  *event2* partition into *event2_1* (Judo) and *event2_2* (Taekwondo, Boxing).

.. code-block:: sql

    ALTER TABLE athlete2 
    REORGANIZE PARTITION event2 INTO (
        PARTITION event2_1 VALUES IN ('Judo'),
        PARTITION event2_2 VALUES IN ('Taekwondo', 'Boxing')
    );

The following example shows how to combine the *event2_1* and *event2_2* partitions back into a single *event2* partition.

.. code-block:: sql

    ALTER TABLE athlete2 
    REORGANIZE PARTITION event2_1, event2_2 INTO (
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing')
    );

.. note::

    *   In a range-partitioned table, only adjacent partitions can be reorganized.
    *   During partition reorganization, CUBRID moves data between partitions in order to reflect the new partitioning schema. Depending on the size of the reorganized partitions, this might be a time consuming operations and should be carefully planned.
    *   The **REORGANIZE PARTITION** clause cannot be used to change the partitioning method. For example, a range-partitioned table cannot be changed into a hash-partitioned one.
    *   There must be at least one partition remaining after deleting partitions.

.. _add-partitions:

Adding Partitions
-----------------

Partitions can be added to a partitioned table by using the *ADD PARTITION* clause of the *ALTER* statement. ::

    ALTER [TABLE | CLASS] [schema_name.]able_name
    ADD PARTITION "(" <partition_definitions_comma_list> ")"

*   *schema_name*: Specifies the schema name of the table. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of the table to which partitions are added.
*   *partition_definitions_comma_list* : Specifies the partitions to be added. Multiple partitions are separated by commas (,).

The following example shows how to add the *before_2012* and *last_one* partitions to the :ref:`participant2<range-participant2-table>` table.

.. code-block:: sql

    ALTER TABLE participant2 ADD PARTITION (
      PARTITION before_2012 VALUES LESS THAN (2012),
      PARTITION last_one VALUES LESS THAN MAXVALUE
    );

.. note::

    *   For range-partitioned tables, range values for added partitions must be greater than the largest range value of the existing partitions.
    *   For range-partitioned tables, if the upper limit of the range of one of the existing partitions is specified by **MAXVALUE**, **ADD PARTITION** clause will always return an error (the :ref:`REORGANIZE PARTITION<reorganize-partitions>` clause should be used instead).
    *   The *ADD PARTITION* clause can only be used on already partitioned tables.
    *   This clause has different semantics when executed on hash-partitioned tables. See :ref:`hash-reorganization` for details.

.. _drop-partitions:

Dropping Partitions
-------------------

Partitions can be dropped from a partitioned table by using the **DROP PARTITION** clause of the **ALTER** statement. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name
    DROP PARTITION partition_name_list

*   *schema_name*: Specifies the schema name of the table. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of the partitioned table.
*   <*partition_name_list*> : Specifies the names of the partitions to be dropped, separated by comma(,).

The following example shows how to drop the *before_2000* partition in the :ref:`participant2<range-participant2-table>` table.

.. code-block:: sql

    ALTER TABLE participant2 DROP PARTITION before_2000;

.. note::

    *   When dropping a partition, all stored data in the partition is deleted. If you want to change the partitioning of a table without losing data, use the **ALTER TABLE** ... **REORGANIZE PARTITION** statement.
    
    *   The number of rows deleted is not returned when a partition is dropped. If you want to delete the data, but want to maintain the table and partitions, use the **DELETE** statement.

This statement is not allowed on hash-partitioned tables. To drop partitions of a hash-partitioned table, use the hash partitioning specific :ref:`alter clauses<hash-reorganization>`.

.. _hash-reorganization:

Hash Partitioning Reorganization
--------------------------------

Because data distribution among partitions in a hash-partitioned table is controlled internally by CUBRID, hash-partitioning reorganization behaves differently for hash-partitioned tables than for list or range partitioned tables. CUBRID allows the number of partitions defined on a hash-partitioned table to be increased or reduced. When modifying the number of partitions of a hash-partitioned table, no data is lost. However, because the domain of the hashing function is modified, table data has to be redistributed between the new partitions in order to maintain hash-partitioning consistency.

The number of partitions defined on a hash-partitioned table can be reduced using the  **COALESCE PARTITION** clause of the **ALTER** statement. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name
    COALESCE PARTITION number_of_shrinking_partitions

*   *schema_name*: Specifies the schema name of the table. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of the table to be redefined.
*   *number_of_shrinking_partitions* : Specifies the number of partitions to be deleted.

The following example shows how to decrease the number of partitions in the :ref:`nation2<hash-nation2-table>` table from 4 to 3.

.. code-block:: sql

    ALTER TABLE nation2 COALESCE PARTITION 1;

The number of partitions defined on a hash partitioned table can be increased using the **ADD PARTITION** clause of the **ALTER** statement. ::

    ALTER [TABLE | CLASS] [schema_name.]table_name
    ADD PARTITION PARTITIONS number

*   *schema_name*: Specifies the schema name of the table. If omitted, the schema name of the current session is used.
*   *table_name* : Specifies the name of the table to be redefined.
*   *number* : Specifies the number of partitions to be added.

The following example shows how to add 3 partitions to the :ref:`nation2 <hash-nation2-table>`.

.. code-block:: sql

    ALTER TABLE nation2 ADD PARTITION PARTITIONS 3;

.. _promote-partitions:

Partition Promotion
-------------------

The **PROMOTE** clause of the **ALTER** statement promotes a partition of a partitioned table to a regular table. This feature is useful when a certain partition contains historic data which is almost never used. By promoting the partition to a regular table, performance on the partitioned table is increased and the data removed from this table (contained in the promoted partition) can still be accessed. Promoting a partition is an irreversible process, promoted partitions cannot be added back to the partitioned table.

The partition **PROMOTE** statement is allowed only on range and list-partitioned tables. Since users do not control how data is distributed among hash partitions, promoting such a partition does not make sense.

When the partition is promoted to a standalone table, this table inherits the data and ordinary indexes only. The following constraints are not available on the promoted partition:

*   Primary Key
*   Foreign key
*   Unique index
*   **AUTO_INCREMENT** attribute and serial
*   Triggers
*   Methods
*   Inheritance relationship (super-class and sub-class)

The syntax for promoting partitions is::

    ALTER TABLE [schema_name.]table_name PROMOTE PARTITION <partition_name_list>

*   *partition_name_list*: The user defined names of partitions to promote separated by comma(,)

The following example creates a partitioned table, inserts some tuples into it and then promotes two of its partitions:

.. code-block:: sql
    
    CREATE TABLE t (i INT) PARTITION BY LIST (i) (
        PARTITION p0 VALUES IN (1, 2),
        PARTITION p1 VALUES IN (3, 4),
        PARTITION p2 VALUES IN (5, 6)
    );
    
    INSERT INTO t VALUES(1), (2), (3), (4), (5), (6);
    
Schema and data of table *t* are shown below.

.. code-block:: sql

    csql> ;schema t
    === <Help: Schema of a Class> ===
    ...
     <Partitions>
         PARTITION BY LIST ([i])
         PARTITION p0 VALUES IN (1, 2)
         PARTITION p1 VALUES IN (3, 4)
         PARTITION p2 VALUES IN (5, 6)

    csql> SELECT * FROM t;

    === <Result of SELECT Command in Line 1> ===
                i
    =============
                1
                2
                3
                4
                5
                6

The following statement promotes partitions *p0* and *p2*:

.. code-block:: sql

    ALTER TABLE t PROMOTE PARTITION p0, p2;

After promotion, table *t* has only one partition (*p1*) and contains the following data.

.. code-block:: sql

    csql> ;schema t
    === <Help: Schema of a Class> ===
     <Class Name>
         t
     ...
     <Partitions>
         PARTITION BY LIST ([i])
         PARTITION p1 VALUES IN (3, 4)

    csql> SELECT * FROM t;

    === <Result of SELECT Command in Line 1> ===
                i
    =============
                3
                4         

.. _index-partitions:

Indexes on Partitioned Tables
=============================

All indexes created on a partitioning table are local indexes. With local indexes, data for each partition is stored in a separate(local) index. This increases concurrency on a partitioned table's indexes, since transactions access data from different partitions also do different, local, indexes.

In order to ensure local unique indexes, the following restriction must be satisfied when creating unique indexes on partitions:

*  The partitioning key must be part of the primary key's and the all the unique indexes' definition.

If this is not satisfied, CUBRID will return an error:

.. code-block:: sql
    
	csql> CREATE TABLE t(i INT , j INT) PARTITION BY HASH (i) PARTITIONS 4;
	Execute OK. (0.142929 sec) Committed.

	1 command(s) successfully processed.
	csql> ALTER TABLE t ADD PRIMARY KEY (i);
	Execute OK. (0.123776 sec) Committed.

	1 command(s) successfully processed.
	csql> CREATE UNIQUE INDEX idx2 ON t(j);

	In the command from line 1,

	ERROR: Partition key attributes must be present in the index key.


	0 command(s) successfully processed.

It is important to understand the benefits of local indexes. In a global index scan, for each partition that was not pruned a separate index scan would have been performed. This leads to poorer performance than scanning local indexes because data from other partitions is fetched from disk and then discarded (it belongs to another partition than the one being scanned at the moment). **INSERT** statements also show better performance on local indexes since these indexes are smaller.

.. _partitioning-notes:

Notes on Partitioning
=====================

Partitioned tables normally behave like regular tables. However there are some notes that should be taken into consideration in order to fully benefit from partitioning a table.

Statistics on Partitioning Tables
---------------------------------

Since CUBRID 9.0, the clause **ANALYZE PARTITION** of the **ALTER** statement has been deprecated. Since partition pruning happens during query execution, this statement will not produce any useful results. Since 9.0, CUBRID keeps separated statistics on each partition. The statistics on the partitioned table are computed as a mean value of the statistics of the table partitions. This is done to optimize the usual case in which, for a query, all partitions are pruned except one. 

Restrictions on Partitioned Tables
-------------------------------------

The following restrictions apply to partitioned tables:

*   The maximum number of partitions which can be defined on a table is 1,024.

*   Partitions cannot be a part of the inheritance chain. Classes cannot inherit a partition and partitions cannot inherit other classes than the partitioned class (which it inherits by default).

*   The following query optimizations are not performed on partitioned tables:

    *   ORDER BY skip (for details, see :ref:`order-by-skip-optimization`)
    *   GROUP BY skip (for details, see :ref:`group-by-skip-optimization`)
    *   Multi-key range optimization (for details, see :ref:`multi-key-range-opt`)
    *   INDEX JOIN

Partitioning Key and Charset, Collation
----------------------------------------

Partitioning keys and partition definition must have the same character set. The following query will return an error:

.. code-block:: sql

    CREATE TABLE t (c CHAR(50) COLLATE utf8_bin) 
    PARTITION BY LIST (c) (
        PARTITION p0 VALUES IN (_utf8'x'),
        PARTITION p1 VALUES IN (_iso88591'y')
    );

::

    ERROR: Invalid codeset '_iso88591' for partition value. Expecting '_utf8' codeset.

CUBRID uses the collation defined on the table when performing comparisons on the partitioning key. The following example will return an error because, for utf8_en_ci collation 'test' equals 'TEST'.

.. code-block:: sql

    CREATE TABLE tbl (str STRING) COLLATE utf8_en_ci 
    PARTITION BY LIST (str) (
        PARTITION p0 VALUES IN ('test'), 
        PARTITION p1 VALUES IN ('TEST')
    );
    
::

    ERROR: Partition definition is duplicated. 'p1'

.. CUBRIDSUS-10161 : below constraints of 9.1 was removed from 9.2. (below will be commented)

    For hash-partitioned tables, the collation of the partitioning key must be binary. 
        *   e.g. of binary collation: utf8_bin, iso88591_bin, euckr_bin
        *   e.g. of non-binary collation: utf8_de_exp_ai_ci
