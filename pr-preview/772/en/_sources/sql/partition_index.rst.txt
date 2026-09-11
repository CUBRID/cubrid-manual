
:meta-keywords: cubrid partition, partitioning key, range partition, hash partition, list partition, partition pruning
:meta-description: Partitioning is a method by which a table is divided into multiple independent physical units called partitions. In CUBRID, each partition is a table implemented as a subclass of the partitioned table.

************
Partitioning
************

Partitioning is a method by which a table is divided into multiple independent physical units called partitions. In CUBRID, each partition is a table implemented as a subclass of the partitioned table. Each partition holds a subset of the partitioned table data defined by a :ref:`partitioning-key` and a partitioning method. Users can access data stored in partitions by executing statements on the partitioned table. This means that users can partition tables without modifying statements or code that is used to access these tables (benefiting from the advantages of partitioning almost without modifying the user application).

Partitioning can enhance manageability, performance and availability. Some advantages of partitioning a table are:

*   Improved management of large capacity tables
*   Improved performance by narrowing the range of access when retrieving data
*   Improved performance and decreased physical loads by distributing disk I/O
*   Decreased possibility of data corruption and improved availability by partitioning a table into multiple chunks
*   Optimized storage cost

Partitioned data is auto-managed by CUBRID. :doc:`INSERT<query/insert>` and :doc:`UPDATE<query/update>` statements executed on partitioned tables perform an additional step during execution to identify the partition in which a tuple must be placed. During UPDATE statement execution, CUBRID identifies situations in which the modified tuple should be moved to another partition and performs this operation keeping the partitioning definition consistent. Inserting tuples for which there is no valid partition will return an error.

When executing :doc:`SELECT<query/select>` statements, CUBRID applies a procedure called :ref:`partition-pruning` to narrow the search space to only those partitions for which the search predicates will produce results. Pruning (eliminating) most of the partitions during a SELECT statement greatly improves performance.

Table partitioning is most effective when applied to large tables. Exactly what a "large" table means is dependent on the user application and on the way in which the table is used in queries. Which is the best partitioning method (:ref:`range<range-partitioning>`, :ref:`list<list-partitioning>` or :ref:`hash<hash-partitioning>`) for a table, also depends on how the table is used in queries and how data will be distributed between partitions. Even though partitioned tables can be used just like normal tables, there are some :ref:`partitioning-notes` which should be taken into consideration.

.. toctree::
    :maxdepth: 2

    partition.rst

