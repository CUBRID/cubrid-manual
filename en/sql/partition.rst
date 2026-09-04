
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
*   **TIMESTAMPTZ**
*   **TIMESTAMPLTZ**
*   **DATETIME**
*   **DATETIMETZ**
*   **DATETIMELTZ**

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
    *   :c:macro:`SYS_DATE`
    *   :c:macro:`SYSDATE`
    *   :c:macro:`SYS_TIME`
    *   :c:macro:`SYSTIME`
    *   :c:macro:`SYS_DATETIME`
    *   :c:macro:`SYSDATETIME`
    *   :c:macro:`SYS_TIMESTAMP`
    *   :c:macro:`SYSTIMESTAMP`
    *   :func:`CURDATE`
    *   :c:macro:`CURRENT_DATE`
    *   :func:`CURTIME`
    *   :c:macro:`CURRENT_TIME`
    *   :c:macro:`CURRENT_TIMESTAMP`
    *   :c:macro:`LOCALTIME`
    *   :c:macro:`LOCALTIMESTAMP`
    *   :c:macro:`CURRENT_DATETIME`
    *   :func:`NOW`
    *   :func:`UTC_TIME`
    *   :func:`UTC_DATE`
    *   :func:`UTC_TIMESTAMP`
    *   :func:`TZ_OFFSET`
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
========================================

Partition pruning is an optimization technique used when accessing a partitioned table to minimize the scope of partitions to be processed.
By excluding partitions that are guaranteed not to contain records satisfying the predicate,
disk I/O and scan costs are reduced, leading to improved query performance.

Partition pruning is not performed at the query compilation stage.
Instead, it is performed at the open stage of query execution by evaluating the partition key predicate.
Consequently, the applicability of partition pruning may vary even for the same query statement,
depending on the bound values included in the partition key predicate.

Whether partition pruning was performed is not displayed in the query execution plan.
Instead, it can be verified through the query profiling output after query execution.
For more details on query profiling, see :ref:`Query Profiling <query-profiling>`.

.. note::

  In versions older than CUBRID 9.0, partition pruning was performed at the query compilation stage.
  Starting with CUBRID 9.0, it is performed at the open stage of query execution.

.. rubric:: Supported Comparison Operators for Partition Pruning by Partitioning Method

.. list-table::
  :header-rows: 1
  :widths: 20 20 60

  * - Partitioning Method
    - Equality Predicate
    - Range Predicate
  * - Range Partitioning
    - ``=``, ``IN``
    - ``<``, ``<=``, ``>``, ``>=``, ``BETWEEN``
  * - List Partitioning
    - ``=``, ``IN``
    - Not supported
  * - Hash Partitioning
    - ``=``, ``IN``
    - Not supported

.. rubric:: Cases Where Partition Pruning Is Not Performed

- If the partition key predicate included in the **WHERE** clause differs from the expression defined as the partitioning key,
  or if the order of arguments does not match even when the expression is identical.
- If comparison operators not supported for partition pruning by the partitioning method are used.
- If the value of the partition key predicate cannot be determined at the open stage of query execution.

.. rubric:: Accessing Specific Partitions Directly

.. code-block:: sql

  ... FROM [<schema_name>.]<partition_table_name>__p__<partition_name> [AS <alias_name>] ...

  ... FROM [<schema_name>.]<partition_table_name> PARTITION (<partition_name>) [AS <alias_name>] ...

Individual partitions can be accessed by using the **PARTITION** clause or by specifying partition names directly.
This method is available even if a partition key predicate is not included in the **WHERE** clause or partition pruning is not performed.
In such cases, the executor handles the partition as a non-partitioned table,
allowing the application of certain optimization techniques that are otherwise restricted for partitioned tables.

However, caution is required because the access scope is fixed to the designated partition.
Records satisfying the predicate are excluded from retrieval even if they exist in other partitions,
potentially resulting in incorrect results.

A target partition can also be specified in **INSERT** and **UPDATE** statements.
Be aware that an error occurs if the record to be processed does not meet the mapping criteria of the specified partition,
potentially resulting in a statement failure.
In particular, explicitly specifying a partition for **INSERT** statements is not recommended because it provides no additional performance advantage.

Directly accessing a partition prevents the utilization of the operational benefits provided by partitioned tables.
Accessing data at the table level ensures that applications do not require modification even if the partition configuration changes,
allowing the system to maintain long-term flexibility.
Because explicit partition specification eliminates this flexibility,
we recommend accessing data through the partitioned table unless there is a particular requirement.

For details on optimization techniques otherwise restricted for partitioned tables, see :ref:`Restrictions on Partitioned Tables <partitioning-notes>`.

.. _example_partition-pruning_query-profiling:

.. rubric:: Example 1. Verifying Partition Pruning via Query Profiling

In this example, we explore whether partition pruning is applied by checking the query profiling output.

In the following query,
a partitioning key predicate is included in the **WHERE** clause, and it is identical to the expression defined in the **CREATE TABLE** statement.
Additionally, partition pruning can be applied because range partitioning supports pruning for range predicates.
However, its application cannot be verified through the execution plan alone because partition pruning is determined at the open stage of query execution.

.. code-block:: sql

  drop table if exists olympic_range;

  create table olympic_range
  partition by range (host_year) (
      partition before_1920 values less than (1920),
      partition before_1940 values less than (1940),
      partition before_1960 values less than (1960),
      partition before_1980 values less than (1980),
      partition before_2000 values less than (2000),
      partition latest values less than maxvalue
    )
  as (select * from olympic);

  create index i_olympic_range_host_nation on olympic_range (host_nation);

  update statistics on olympic_range;

.. code-block:: sql

  set optimization level 513;
  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_range as o
  where
      o.host_year > 1990
      and o.host_nation = 'USA';

.. code-block:: text

  Join graph segments (f indicates final):
  seg[0]: [0]
  seg[1]: host_year[0] (f)
  seg[2]: host_nation[0] (f)
  seg[3]: host_city[0] (f)
  seg[4]: mascot[0] (f)
  Join graph nodes:
  node[0]: public.olympic_range o(25/6) (sargs 0 1) (loc 0)
  Join graph terms:
  term[0]: o.host_nation='USA' (sel 0.0555556) (sarg term) (not-join eligible) (indexable host_nation[0]) (loc 0)
  term[1]: o.host_year range (1990 gt_inf max) (sel 0.1) (rank 2) (sarg term) (not-join eligible) (loc 0)

  Query plan:

  iscan
      class: o node[0]
      index: i_olympic_range_host_nation term[0]
      sargs: term[1]
      cost:  3 card 1

  Query stmt:

  select o.host_year, o.host_nation, o.host_city, o.mascot from olympic_range o where (o.host_year> ?:0 ) and o.host_nation= ?:1

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           1996  'USA'                 'Atlanta'             'Izzy'

Upon checking the profiling output after query execution,
we can observe that scans for individual partitions are displayed as **PARTITION** entries under the **SCAN** entry.
The **SCAN** entry displays scan information for the entire partitioned table,
while each **PARTITION** entry shows scan information for its respective partition.
Any partition that does not appear as a **PARTITION** entry has been excluded from the scan target.

In this example, only the ``before_2000`` and ``latest`` partitions were scanned
because they satisfy the ``o.host_year > 1990`` predicate.
All other partitions were successfully excluded from the scan target.

.. code-block:: sql

  show trace;

.. code-block:: text

  Query Plan:
    INDEX SCAN (o.i_olympic_range_host_nation) (key range: o.host_nation= ?:1 )

    rewritten query: select o.host_year, o.host_nation, o.host_city, o.mascot from [public.olympic_range] o where (o.host_year> ?:0 ) and o.host_nation= ?:1

  Trace Statistics:
    SELECT (time: 0, fetch: 10, fetch_time: 0, ioread: 0)
      SCAN (index: public.olympic_range.i_olympic_range_host_nation), (btree time: 0, fetch: 6, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 2) (lookup time: 0, rows: 1)
             PARTITION (index: public.olympic_range__p__before_2000.i_olympic_range_host_nation), (btree time: 0, fetch: 4, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 2) (lookup time: 0, rows: 1)
             PARTITION (index: public.olympic_range__p__latest.i_olympic_range_host_nation), (btree time: 0, fetch: 2, ioread: 0, readkeys: 0, filteredkeys: 0, rows: 0) (lookup time: 0, rows: 0)

In the following query,
partition pruning is not applied because no partitioning key predicate is included in the **WHERE** clause.

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_range as o
  where
      o.opening_date > '1990-12-31'
      and o.host_nation = 'USA';

  show trace;

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           1996  'USA'                 'Atlanta'             'Izzy'

Upon checking the profiling output after query execution,
we can observe that scans for all partitions are displayed as **PARTITION** entries under the **SCAN** entry because partition pruning was not applied.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 24, fetch_time: 1, ioread: 0)
      SCAN (index: public.olympic_range.i_olympic_range_host_nation), (btree time: 1, fetch: 16, ioread: 0, readkeys: 3, filteredkeys: 3, rows: 4) (lookup time: 1, rows: 1)
             PARTITION (index: public.olympic_range__p__before_1920.i_olympic_range_host_nation), (btree time: 0, fetch: 3, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 1) (lookup time: 0, rows: 0)
             PARTITION (index: public.olympic_range__p__before_1940.i_olympic_range_host_nation), (btree time: 0, fetch: 3, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 1) (lookup time: 0, rows: 0)
             PARTITION (index: public.olympic_range__p__before_1960.i_olympic_range_host_nation), (btree time: 0, fetch: 2, ioread: 0, readkeys: 0, filteredkeys: 0, rows: 0) (lookup time: 0, rows: 0)
             PARTITION (index: public.olympic_range__p__before_1980.i_olympic_range_host_nation), (btree time: 0, fetch: 2, ioread: 0, readkeys: 0, filteredkeys: 0, rows: 0) (lookup time: 0, rows: 0)
             PARTITION (index: public.olympic_range__p__before_2000.i_olympic_range_host_nation), (btree time: 1, fetch: 4, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 2) (lookup time: 1, rows: 1)
             PARTITION (index: public.olympic_range__p__latest.i_olympic_range_host_nation), (btree time: 0, fetch: 2, ioread: 0, readkeys: 0, filteredkeys: 0, rows: 0) (lookup time: 0, rows: 0)

.. _example_partition-pruning_arithmetic-expression:

.. rubric:: Example 2. Partition Pruning for Arithmetic Expression Partition Keys

In this example, we explore whether partition pruning is applied to a partition key defined by an arithmetic expression,
focusing on two cases: using the identical expression in the **WHERE** clause, and reordering the arguments within that expression.

In the following query,
a partitioning key predicate using an arithmetic expression is included in the **WHERE** clause,
and it is identical to the expression defined in the **CREATE TABLE** statement.
Additionally, partition pruning can be applied because range partitioning supports pruning for range predicates.

.. code-block:: sql

  drop table if exists olympic_arith_range;

  create table olympic_arith_range
  partition by range (host_year + 5) (
      partition before_1925 values less than (1925),
      partition before_1945 values less than (1945),
      partition before_1965 values less than (1965),
      partition before_1985 values less than (1985),
      partition before_2005 values less than (2005),
      partition latest values less than maxvalue
    )
  as (select * from olympic);

  update statistics on olympic_arith_range;

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_arith_range as o
  where
      o.host_year + 5 between 1975 and 1995
  order by
      o.host_year;

  show trace;

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           1972  'Germany'             'Munich'              'Waldi'
           1976  'Canada'              'Montreal'            'Amik'
           1980  'USSR'                'Moscow'              'Misha'
           1984  'USA'                 'Los Angeles'         'Sam'
           1988  'Korea'               'Seoul'               'HODORI'

Upon checking the profiling output after query execution,
we can observe that only the ``before_1985`` and ``before_2005`` partitions were scanned
because they satisfy the ``o.host_year + 5 between 1975 and 1995`` predicate.
All other partitions were successfully excluded from the scan target.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 4, fetch: 4, fetch_time: 0, ioread: 0)
      SCAN (table: public.olympic_arith_range), (heap time: 0, fetch: 2, ioread: 0, readrows: 10, rows: 5)
             PARTITION (table: public.olympic_arith_range__p__before_1985), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 2)
             PARTITION (table: public.olympic_arith_range__p__before_2005), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 3)
      ORDERBY (time: 4, sort: true, page: 0, ioread: 0)

Even if the arithmetic expression has an identical structure,
partition pruning cannot be applied because the order of the arguments does not match.

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_arith_range as o
  where
      5 + o.host_year between 1975 and 1995
  order by
      o.host_year;

  show trace;

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           1972  'Germany'             'Munich'              'Waldi'
           1976  'Canada'              'Montreal'            'Amik'
           1980  'USSR'                'Moscow'              'Misha'
           1984  'USA'                 'Los Angeles'         'Sam'
           1988  'Korea'               'Seoul'               'HODORI'

Upon checking the profiling output after query execution,
we can observe that scans for all partitions are displayed as **PARTITION** entries under the **SCAN** entry because partition pruning was not applied.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 0, fetch: 12, fetch_time: 0, ioread: 0)
      SCAN (table: public.olympic_arith_range), (heap time: 0, fetch: 6, ioread: 0, readrows: 25, rows: 5)
             PARTITION (table: public.olympic_arith_range__p__before_1925), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 0)
             PARTITION (table: public.olympic_arith_range__p__before_1945), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 0)
             PARTITION (table: public.olympic_arith_range__p__before_1965), (heap time: 0, fetch: 1, ioread: 0, readrows: 3, rows: 0)
             PARTITION (table: public.olympic_arith_range__p__before_1985), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 2)
             PARTITION (table: public.olympic_arith_range__p__before_2005), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 3)
             PARTITION (table: public.olympic_arith_range__p__latest), (heap time: 0, fetch: 1, ioread: 0, readrows: 2, rows: 0)
      ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

.. _example_partition-pruning_function-expression:

.. rubric:: Example 3. Partition Pruning for Functional Expression Partition Keys

In this example, we explore whether partition pruning is applied to a partition key defined by a functional expression,
focusing on two cases: using the identical expression in the **WHERE** clause, and using only the source column without the functional expression.

In the following query,
a partitioning key predicate using a functional expression is included in the **WHERE** clause,
and it is identical to the expression defined in the **CREATE TABLE** statement.
Additionally, partition pruning can be applied because range partitioning supports pruning for range predicates.

.. code-block:: sql

  drop table if exists olympic_func_range;

  create table olympic_func_range
  partition by range (YEAR (opening_date)) (
      partition before_1920 values less than (1920),
      partition before_1940 values less than (1940),
      partition before_1960 values less than (1960),
      partition before_1980 values less than (1980),
      partition before_2000 values less than (2000),
      partition latest values less than maxvalue
    )
  as (select * from olympic);

  update statistics on olympic_func_range;

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      YEAR (o.opening_date) as opening_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_func_range as o
  where
      YEAR (o.opening_date) between 1970 and 1990
  order by
      opening_year;

  show trace;

.. code-block:: text

    opening_year  host_nation           host_city             mascot
  ================================================================================
            1972  'Germany'             'Munich'              'Waldi'
            1976  'Canada'              'Montreal'            'Amik'
            1980  'USSR'                'Moscow'              'Misha'
            1984  'USA'                 'Los Angeles'         'Sam'
            1988  'Korea'               'Seoul'               'HODORI'

Upon checking the profiling output after query execution,
we can observe that only the ``before_1980`` and ``before_2000`` partitions were scanned
because they satisfy the ``YEAR (o.opening_date) between 1970 and 1990`` predicate.
All other partitions were successfully excluded from the scan target.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 0, fetch: 4, fetch_time: 0, ioread: 0)
      SCAN (table: public.olympic_func_range), (heap time: 0, fetch: 2, ioread: 0, readrows: 10, rows: 5)
             PARTITION (table: public.olympic_func_range__p__before_1980), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 2)
             PARTITION (table: public.olympic_func_range__p__before_2000), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 3)
      ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

Partition pruning can be applied even if the source column is used instead of the functional expression,
because the functional expression defined as the partition key guarantees the same sort order as the source column.

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      YEAR (o.opening_date) as opening_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_func_range as o
  where
      o.opening_date between '1970-01-01' and '1990-12-31'
  order by
      opening_year;

  show trace;

.. code-block:: text

    opening_year  host_nation           host_city             mascot
  ================================================================================
            1972  'Germany'             'Munich'              'Waldi'
            1976  'Canada'              'Montreal'            'Amik'
            1980  'USSR'                'Moscow'              'Misha'
            1984  'USA'                 'Los Angeles'         'Sam'
            1988  'Korea'               'Seoul'               'HODORI'

Upon checking the profiling output after query execution,
we can observe that partition pruning was applied because the **YEAR** function guarantees the same sort order as the source column,
even though the functional expression defining the partition key was not directly included in the **WHERE** clause.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 0, fetch: 4, fetch_time: 0, ioread: 0)
      SCAN (table: public.olympic_func_range), (heap time: 0, fetch: 2, ioread: 0, readrows: 10, rows: 5)
             PARTITION (table: public.olympic_func_range__p__before_1980), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 2)
             PARTITION (table: public.olympic_func_range__p__before_2000), (heap time: 0, fetch: 1, ioread: 0, readrows: 5, rows: 3)
      ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

we can observe that only the ``before_1980`` and ``before_2000`` partitions were scanned
because ``o.opening_date`` guarantees the same sort order as ``YEAR(o.opening_date)``, even though ``o.opening_date`` is included.
These partitions satisfy the ``YEAR(o.opening_date) between YEAR('1970-01-01') and YEAR('1990-12-31')`` predicate.
All other partitions were successfully excluded from the scan target.

.. _example_partition-pruning_list:

.. rubric:: Example 4. Partition Pruning for List Partitioning

In this example, we explore whether partition pruning is applied to a table created using list partitioning,
focusing on two cases: using an equality predicate in the **WHERE** clause, and using a range predicate.

In the following query,
a partitioning key predicate is included in the **WHERE** clause, and it is identical to the expression defined in the **CREATE TABLE** statement.
Additionally, partition pruning can be applied because list partitioning supports pruning for equality predicates.

.. code-block:: sql

  drop table if exists participant_list;

  create table participant_list
  partition by list (host_year) (
      partition p1988 values IN (1988),
      partition p1992 values IN (1992),
      partition p1996 values IN (1996),
      partition p2000 values IN (2000),
      partition p2004 values IN (2004)
    )
  as (select * from participant);

  update statistics on participant_list;

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      p.host_year, p.nation_code, p.gold
  from
      participant_list as p
  where
      p.host_year in (1988, 1996)
      and p.gold > 40
  order by
      p.host_year, p.gold, p.nation_code;

  show trace;

.. code-block:: text

      host_year  nation_code                  gold
  ================================================
           1988  'URS'                          55
           1996  'USA'                          44

Upon checking the profiling output after query execution,
we can observe that only the ``p1988`` and ``p1996`` partitions were scanned
because they satisfy the ``p.host_year in (1988, 1996)`` predicate.
All other partitions were successfully excluded from the scan target.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 4, fetch_time: 0, ioread: 0)
      SCAN (table: public.participant_list), (heap time: 1, fetch: 2, ioread: 0, readrows: 352, rows: 2)
             PARTITION (table: public.participant_list__p__p1988), (heap time: 1, fetch: 1, ioread: 0, readrows: 156, rows: 1)
             PARTITION (table: public.participant_list__p__p1996), (heap time: 0, fetch: 1, ioread: 0, readrows: 196, rows: 1)
      ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

Partition pruning is not applied when non-equality predicates, such as range predicates, are used
because list partitioning supports pruning only for equality predicates.

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      p.host_year, p.nation_code, p.gold
  from
      participant_list as p
  where
      p.host_year < 1996
      and p.gold > 40
  order by
      p.host_year;

  show trace;

.. code-block:: text

      host_year  nation_code                  gold
  ================================================
           1988  'URS'                          55
           1992  'EUN'                          45

Upon checking the profiling output after query execution,
we can observe that scans for all partitions are displayed as **PARTITION** entries under the **SCAN** entry because partition pruning was not applied.

Based on the values of each partition, the ``p1996``, ``p2000``, and ``p2004`` partitions appear to be targets for exclusion from the scan.
However, specific partitions cannot be excluded from the scan target by a range predicate
because list partitioning is not structured to arrange values in a sorted order.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 10, fetch_time: 0, ioread: 0)
      SCAN (table: public.participant_list), (heap time: 0, fetch: 5, ioread: 0, readrows: 916, rows: 2)
             PARTITION (table: public.participant_list__p__p1988), (heap time: 0, fetch: 1, ioread: 0, readrows: 156, rows: 1)
             PARTITION (table: public.participant_list__p__p1992), (heap time: 0, fetch: 1, ioread: 0, readrows: 165, rows: 1)
             PARTITION (table: public.participant_list__p__p1996), (heap time: 0, fetch: 1, ioread: 0, readrows: 196, rows: 0)
             PARTITION (table: public.participant_list__p__p2000), (heap time: 0, fetch: 1, ioread: 0, readrows: 197, rows: 0)
             PARTITION (table: public.participant_list__p__p2004), (heap time: 0, fetch: 1, ioread: 0, readrows: 202, rows: 0)
      ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

.. _example_partition-pruning_hash:

.. rubric:: Example 5. Partition Pruning for Hash Partitioning

In this example, we explore whether partition pruning is applied to a table created using hash partitioning,
focusing on two cases: using an equality predicate in the **WHERE** clause, and using an inequality predicate.

In the following query,
a partitioning key predicate is included in the **WHERE** clause, and it is identical to the expression defined in the **CREATE TABLE** statement.
Additionally, partition pruning can be applied because hash partitioning supports pruning for equality predicates.

.. code-block:: sql

  drop table if exists stadium_hash;

  create table stadium_hash
  partition by hash (nation_code) partitions 4
  as (select * from stadium);

  update statistics on stadium_hash;

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      s.nation_code, s.name, s.seats
  from
      stadium_hash as s
  where
      s.nation_code = 'KOR'
      and s.seats >= 100000
  order by
      s.name;

  show trace;

.. code-block:: text

    nation_code           name                        seats
  =========================================================
    'KOR'                 'Seoul Olympic Stadium'       100000

Upon checking the profiling output after query execution,
we can observe that only the ``p2`` partition was scanned
because it satisfies the ``s.nation_code = 'KOR'`` predicate.
All other partitions were successfully excluded from the scan target.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 0, fetch: 2, fetch_time: 0, ioread: 0)
      SCAN (table: public.stadium_hash), (heap time: 0, fetch: 1, ioread: 0, readrows: 32, rows: 1)
             PARTITION (table: public.stadium_hash__p__p2), (heap time: 0, fetch: 1, ioread: 0, readrows: 32, rows: 1)

Partition pruning is not applied when non-equality predicates, such as inequality predicates, are used
because hash partitioning supports pruning only for equality predicates.

.. code-block:: sql

  set trace on;

  select /*+ recompile */
      s.nation_code, s.name, s.seats
  from
      stadium_hash as s
  where
      s.nation_code != 'KOR'
      and s.seats > 100000
  order by
      s.name;

  show trace;

.. code-block:: text

    nation_code           name                        seats
  =========================================================
    'AUS'                 'Olympic Stadium'          115600

Upon checking the profiling output after query execution,
we can observe that scans for all partitions are displayed as **PARTITION** entries under the **SCAN** entry because partition pruning was not applied.

In the previous query, only the ``p2`` partition was scanned because it satisfies the ``s.nation_code = 'KOR'`` predicate.
Although the ``p2`` partition might appear to be a candidate for exclusion under an inequality predicate,
the ``p2`` partition cannot be excluded because records satisfying the ``s.nation_code != 'KOR'`` predicate may be included in that partition.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 0, fetch: 8, fetch_time: 0, ioread: 0)
      SCAN (table: public.stadium_hash), (heap time: 0, fetch: 4, ioread: 0, readrows: 141, rows: 1)
             PARTITION (table: public.stadium_hash__p__p0), (heap time: 0, fetch: 1, ioread: 0, readrows: 27, rows: 0)
             PARTITION (table: public.stadium_hash__p__p1), (heap time: 0, fetch: 1, ioread: 0, readrows: 53, rows: 0)
             PARTITION (table: public.stadium_hash__p__p2), (heap time: 0, fetch: 1, ioread: 0, readrows: 32, rows: 0)
             PARTITION (table: public.stadium_hash__p__p3), (heap time: 0, fetch: 1, ioread: 0, readrows: 29, rows: 1)

.. _example_partition-pruning_join:

.. rubric:: Example 6. Partition Pruning in Joins

In this example, we explore whether partition pruning is applied when a partitioned table is used as a driven table in a join query.

In the following query,
hints are used to fix the join order so that the partitioned table is used as the driven table.
Accordingly, the join predicate is expected to be utilized as a partitioning key predicate.
However, partition pruning is determined by analyzing values identifiable at the open stage of query execution.
Therefore, partition pruning is not applied to join predicates where column values are bound during join execution
because those values are not determinable at the open stage of query execution.

.. code-block:: sql

  drop table if exists olympic_range;

  create table olympic_range
  partition by range (host_year) (
      partition before_1920 values less than (1920),
      partition before_1940 values less than (1940),
      partition before_1960 values less than (1960),
      partition before_1980 values less than (1980),
      partition before_2000 values less than (2000),
      partition latest values less than maxvalue
    )
  as (select * from olympic);

  update statistics on olympic_range;

.. code-block:: sql

  set trace on;

  select /*+ recompile ordered */
      p.host_year, p.nation_code, p.gold,
      o.host_nation, o.slogan
  from
      participant as p
      inner join olympic_range as o on p.host_year = o.host_year
  where
      p.host_year in (1988, 2004)
      and p.gold > 40
  order by
      p.host_year, p.gold, p.nation_code;

  show trace;

.. code-block:: text

      host_year  nation_code                  gold  host_nation           slogan
  ============================================================================================
           1988  'URS'                          55  'Korea'               'Harmony and progress'

Upon checking the profiling output after query execution,
we can observe that scans for all partitions are displayed as **PARTITION** entries under the **SCAN** entry because partition pruning was not applied.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 2204, fetch_time: 0, ioread: 0)
      SCAN (index: public.participant.pk_participant_host_year_nation_code), (btree time: 1, fetch: 2172, ioread: 0, readkeys: 2154, filteredkeys: 2148, rows: 2148) (lookup time: 1, rows: 6)
        SCAN (table: public.olympic_range), (heap time: 0, fetch: 31, ioread: 0, readrows: 25, rows: 1)
               PARTITION (table: public.olympic_range__p__before_1920), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 0)
               PARTITION (table: public.olympic_range__p__before_1940), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 0)
               PARTITION (table: public.olympic_range__p__before_1960), (heap time: 0, fetch: 4, ioread: 0, readrows: 3, rows: 0)
               PARTITION (table: public.olympic_range__p__before_1980), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 0)
               PARTITION (table: public.olympic_range__p__before_2000), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 1)
               PARTITION (table: public.olympic_range__p__latest), (heap time: 0, fetch: 3, ioread: 0, readrows: 2, rows: 0)
        MEMOIZE (time: 0, hit: 0, miss: 1, size: 0KB, enabled: true)

Partition pruning can be applied even when a partitioned table is used as the driven table in a join query
because it is applicable if a partitioning key predicate with an identifiable value is included in the **WHERE** clause at the open stage of query execution.

.. code-block:: sql

  set trace on;

  select /*+ recompile ordered */
      p.host_year, p.nation_code, p.gold,
      o.host_nation, o.slogan
  from
      participant as p
      inner join olympic_range as o on p.host_year = o.host_year
  where
      o.host_year in (1988, 2004)
      and p.gold > 40
  order by
      p.host_year, p.gold, p.nation_code;

  show trace;

.. code-block:: text

      host_year  nation_code                  gold  host_nation           slogan
  ============================================================================================
           1988  'URS'                          55  'Korea'               'Harmony and progress'

Upon checking the profiling output after query execution,
we can observe that only the ``before_2000`` and ``latest`` partitions were scanned
because they satisfy the ``o.host_year in (1988, 2004)`` predicate.
All other partitions were successfully excluded from the scan target.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 1845, fetch_time: 1, ioread: 0)
      SCAN (table: public.participant), (heap time: 1, fetch: 1838, ioread: 0, readrows: 1832, rows: 6)
        SCAN (table: public.olympic_range), (heap time: 0, fetch: 6, ioread: 0, readrows: 21, rows: 1)
               PARTITION (table: public.olympic_range__p__before_2000), (heap time: 0, fetch: 3, ioread: 0, readrows: 15, rows: 1)
               PARTITION (table: public.olympic_range__p__latest), (heap time: 0, fetch: 3, ioread: 0, readrows: 6, rows: 0)
        MEMOIZE (time: 0, hit: 0, miss: 3, size: 0KB, enabled: true)

Partition pruning is applicable even when a bind variable is used in a partitioning key predicate
because the bound value is identifiable at the open stage of query execution.

.. code-block:: sql

  set trace on;

  prepare q from '
    select /*+ ordered */
        p.host_year, p.nation_code, p.gold,
        o.host_nation, o.slogan
    from
        participant as p
        inner join olympic_range as o on p.host_year = o.host_year
    where
        o.host_year = year (to_date (?, ''YYYY-MM-DD''))
        and p.gold > 40
    order by
        p.host_year, p.gold, p.nation_code;
  ';

  execute q using '1988-01-01';

  show trace;

.. code-block:: text

      host_year  nation_code                  gold  host_nation           slogan
  ============================================================================================
           1988  'URS'                          55  'Korea'               'Harmony and progress'

Upon checking the profiling output after query execution,
we can observe that only the ``before_2000`` partition was scanned
because it satisfies the ``o.host_year = year (to_date ('1988-01-01', 'YYYY-MM-DD'))`` predicate.
All other partitions were successfully excluded from the scan target.

.. code-block:: text

  Trace Statistics:
    SELECT (time: 1, fetch: 165, fetch_time: 0, ioread: 0)
      SCAN (index: public.participant.pk_participant_host_year_nation_code), (btree time: 1, fetch: 158, ioread: 0, readkeys: 157, filteredkeys: 156, rows: 156) (lookup time: 1, rows: 1)
        SCAN (table: public.olympic_range), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 1)
               PARTITION (table: public.olympic_range__p__before_2000), (heap time: 0, fetch: 6, ioread: 0, readrows: 5, rows: 1)

.. _example_partition-pruning_direct-access:

.. rubric:: Example 7. Accessing Specific Partitions Directly

In this example, we explore how to directly query a specific partition using the **PARTITION** clause, separate from partition pruning.
Through this, we explore whether optimization techniques that were inapplicable to the partitioned table are applicable to the partition.

In the following query,
the **SKIP ORDER BY** is not applied because it targets the partitioned table,
even though the **ORDER BY** and **LIMIT** clauses are included and an appropriate index exists.

.. code-block:: sql

  drop table if exists olympic_range;

  create table olympic_range
  partition by range (host_year) (
      partition before_1920 values less than (1920),
      partition before_1940 values less than (1940),
      partition before_1960 values less than (1960),
      partition before_1980 values less than (1980),
      partition before_2000 values less than (2000),
      partition latest values less than maxvalue
    )
  as (select * from olympic);

  create index i_olympic_range_host_year on olympic_range (host_year);

  update statistics on olympic_range;

.. code-block:: sql

  set optimization level 513;
  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_range as o
  where
      o.host_year > 2000
  order by
      o.host_year desc
  limit 1;

  show trace;

.. code-block:: text

  Join graph segments (f indicates final):
  seg[0]: [0]
  seg[1]: host_year[0] (f)
  seg[2]: host_nation[0] (f)
  seg[3]: host_city[0] (f)
  seg[4]: mascot[0] (f)
  Join graph nodes:
  node[0]: public.olympic_range o(25/6) (sargs 0) (loc 0)
  Join graph terms:
  term[0]: o.host_year range (2000 gt_inf max) (sel 0.1) (rank 2) (sarg term) (not-join eligible) (indexable host_year[0]) (loc 0)

  Query plan:

  temp(order by)
      subplan: iscan
                   class: o node[0]
                   index: i_olympic_range_host_year term[0]
                   cost:  3 card 2
      sort:  1 desc
      cost:  9 card 2

  Query stmt:

  select o.host_year, o.host_nation, o.host_city, o.mascot from olympic_range o where (o.host_year> ?:0 ) order by 1 desc  for orderby_num()<= ?:1

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           2004  'Greece'              'Athens'              'Athena  Phevos'

Upon checking the profiling output after query execution,
we can observe that **TopNSort** was performed for the **ORDER BY** clause because the **SKIP ORDER BY** was not applied.

.. code-block:: text

  Query Plan:
    SORT (order by)
      INDEX SCAN (o.i_olympic_range_host_year) (key range: (o.host_year> ?:0 ))

    rewritten query: select o.host_year, o.host_nation, o.host_city, o.mascot from [public.olympic_range] o where (o.host_year> ?:0 ) order by 1 desc  for orderby_num()<= ?:1

  Trace Statistics:
    SELECT (time: 0, fetch: 6, fetch_time: 0, ioread: 0)
      SCAN (index: public.olympic_range.i_olympic_range_host_year), (btree time: 0, fetch: 3, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 1) (lookup time: 0, rows: 1)
             PARTITION (index: public.olympic_range__p__latest.i_olympic_range_host_year), (btree time: 0, fetch: 3, ioread: 0, readkeys: 1, filteredkeys: 1, rows: 1) (lookup time: 0, rows: 1)
      ORDERBY (time: 0, topnsort: true)

**SKIP ORDER BY** is applicable if we directly query a specific partition using the **PARTITION** clause
because the partition is handled as a non-partitioned table.

.. code-block:: sql

  set optimization level 513;
  set trace on;

  select /*+ recompile */
      o.host_year, o.host_nation, o.host_city, o.mascot
  from
      olympic_range partition (latest) as o
  where
      o.host_year > 2000
  order by
      o.host_year desc
  limit 1;

  show trace;

.. code-block:: text

  Join graph segments (f indicates final):
  seg[0]: [0]
  seg[1]: host_year[0] (f)
  seg[2]: host_nation[0] (f)
  seg[3]: host_city[0] (f)
  seg[4]: mascot[0] (f)
  Join graph nodes:
  node[0]: public.olympic_range__p__latest o(25/1) (sargs 0) (loc 0)
  Join graph terms:
  term[0]: o.host_year range (2000 gt_inf max) (sel 0.1) (rank 2) (sarg term) (not-join eligible) (indexable host_year[0]) (loc 0)

  Query plan:

  iscan
      class: o node[0]
      index: i_olympic_range_host_year term[0] (desc_index)
      sort:  1 desc
      cost:  4 card 2

  Query stmt:

  select o.host_year, o.host_nation, o.host_city, o.mascot from olympic_range__p__latest o where (o.host_year> ?:0 ) order by 1 desc  for orderby_num()<= ?:1

  /* ---> skip ORDER BY */

.. code-block:: text

      host_year  host_nation           host_city             mascot
  ===============================================================================
           2004  'Greece'              'Athens'              'Athena  Phevos'

Upon checking the profiling output after query execution,
we can observe that **SKIP ORDER BY** was applied.

.. code-block:: text

  Query Plan:
    INDEX SCAN (o.i_olympic_range_host_year) (key range: (o.host_year> ?:0 ), desc_index: true)
    skip order by: true

    rewritten query: select o.host_year, o.host_nation, o.host_city, o.mascot from [public.olympic_range__p__latest] o where (o.host_year> ?:0 ) order by 1 desc  for orderby_num()<= ?:1

  Trace Statistics:
    SELECT (time: 0, fetch: 4, fetch_time: 0, ioread: 0)
      SCAN (index: public.olympic_range__p__latest.i_olympic_range_host_year), (btree time: 0, fetch: 3, ioread: 0, readkeys: 2, filteredkeys: 1, rows: 1) (lookup time: 0, rows: 1)

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
