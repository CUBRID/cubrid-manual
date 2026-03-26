.. _join-method:

Join Methods
========================================

A Relational database stores data by dividing it into multiple tables through normalization.
This approach effectively reduces data redundancy and ensures data integrity.
However, to query the divided data in an integrated manner, relationships between tables must be combined.
A join operation combines these relationships and returns results as if they were from a single table.

.. rubric:: Join Type

The records included in the join results depend on the specified type of join.
For more details, refer to :ref:`join-query`.

.. rubric:: Join Method

The way records are combined between tables depends on the join method used. CUBRID supports the following join methods:

  - :ref:`join-method_nested-loop`
  - :ref:`join-method_sort-merge`
  - :ref:`join-method_hash`

Join methods are determined by the query optimizer based on a cost comparison, though users can also specify them manually.
The query optimizer calculates the cost of each join method using statistics collected by the database.
It then selects the method with the lowest cost.
When a query includes two or more joins, different join methods can be applied to each join.
Each join method operates on only two tables at a time and is executed sequentially.
In other words, the next join is performed only after the previous one has been completed.

By default, CUBRID does not generate execution plans for **Sort-Merge Joins** or **Hash Joins**.
:ref:`SQL hints <sql-hint>` are required for the optimizer to consider these join methods.
The **optimizer_enable_merge_join** parameter can be set to **yes** (default is **no**) to enable a **Sort-Merge Join** without requiring the **USE_MERGE** hint.
For more details, refer to :ref:`other-parameters`.

.. note::

  The term **table** here is used broadly to include not only physical tables but also views (virtual tables) and derived tables.

.. _join-method_nested-loop:

Nested Loop Join
----------------------------------------

A nested loop join reads records from the outer table one at a time and combines each record with matching records from the inner table.
This approach is similar to the concept of nested loops in programming.
The table used in the outer loop is referred to as the **driving table**, while the table in the inner loop is called the **driven table**.

.. rubric:: Driving Table

When selecting a driving table from two tables, it is generally more efficient to choose the table with fewer records.
Fewer records in the driving table result in fewer join operations, reducing the overall cost of the join.
Since random access costs are typically constant, fewer join operations on the driven table lead to lower join costs and better performance.
The query optimizer automatically selects the driving table by comparing join costs,
but users can specify the driving table explicitly using **ORDERED** or **LEADING** hints.

.. rubric:: Driven Table

A nested loop join performs efficiently when random access to the driven table is possible.
For random access, a join-compatible index must be created on the driven table.
If no such index exists, all records in the driven table must be read sequentially, which degrades join performance.
When random access to the driven table is enabled through a join-compatible index,
the nested loop join is displayed as an **Index Join (idx-join)** in the execution plan.

.. rubric:: Partial Range Processing

Nested loop joins can perform join operations by reading just one record from each table,
and they maintain join results that can be returned immediately, even if the join is not completed for all records.
This behavior enables partial range processing: if only the first N results are needed, they are returned as soon as those N results are ready.
Partial range processing improves overall join performance by avoiding unnecessary join operations.

.. rubric:: Hash List Scan

If the driven table is a view or a derived table, the index cannot be used,
so the join is performed using the list scan method, which sequentially reads records from the temporary result.
Hash list scan is a type of list scan that allows random access to records in the temporary result that satisfy the join condition.
Since list scan reads records from a temporary result, it cannot be used for physical tables.
Therefore, hash list scan can only be used when the driven table is a view or a derived table.
Hash list scan operates similarly to a :ref:`hash join <join-method_hash>` and can be used only when all join conditions are equality conditions.
The use of hash list scan can be verified through :ref:`query profiling <query-profiling>`.

The differences between hash list scan and hash join are as follows:

  1. Hash list scan builds the hash table using only the driven table.
  2. Hash list scan is part of a nested loop join, enabling partial range processing.

The query optimizer does not consider whether a hash list scan is performed when calculating the cost of a join.

.. _example_join-method_nested-loop_no-index:

.. rubric:: Example 1. Nested Loop Join without Index Scans

In this example, the driven table does not utilize an index scan during the nested loop join.
The **ORDERED** hint is applied to enforce the join order, making the nation table the driving table and the athlete table the driven table.
Since the driven table lacks an index appropriate for the join condition, all records in the athlete table are read sequentially for each record in the nation table.

  .. code-block:: sql

    select /*+ recompile ordered use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

  .. code-block:: none

    Query plan:

    nl-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   sargs: term[2]
                   cost:  2 card 31
        inner: sscan
                   class: a node[1]
                   sargs: term[0] AND term[1]
                   cost:  49 card 202
        cost:  4697 card 29

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 92, fetch: 1720, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (table: public.athlete), (heap time: 92, fetch: 1504, ioread: 0, readrows: 313819, rows: 103)

**Execution Steps:**

  1. Read a record from the nation table that satisfies the filter condition (``n.continent = 'Asia'``).
  2. Read a record from the athlete table that satisfies both the filter condition (``a.event = 'Baseball'``) and join condition (``n.code = a.nation_code``).
  3. If the condition is met, combine the two records and add them to the result.
  4. Repeat **step 2** for the next record in the athlete table.
  5. Repeat **steps 1-3** for the next record in the nation table.
  6. Continue until all records from both tables are processed.

.. _example_join-method_nested-loop_index:

.. rubric:: Example 2. Nested Loop Join with Index Scans

This example demonstrates a nested loop join where the driven table uses an index scan.
An index suitable for the join condition is created on the driven table (athlete).
As a result, not all records in the athlete table need to be scanned for each record in the nation table, enabling random access.

  .. code-block:: sql

    create index i_athlete_nation_code on athlete (nation_code);

    select /*+ recompile ordered use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

    drop index i_athlete_nation_code on athlete;

  .. code-block:: none

    Query plan:

    idx-join (inner join)
        outer: sscan
                   class: n node[0]
                   sargs: term[2]
                   cost:  2 card 31
        inner: iscan
                   class: a node[1]
                   index: i_athlete_nation_code term[0]
                   sargs: term[1]
                   cost:  4 card 202
        cost:  68 card 29

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 1, fetch: 1265, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 1, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (index: public.athlete.i_athlete_nation_code), (btree time: 0, fetch: 1045, ioread: 0, readkeys: 24, filteredkeys: 0, rows: 948) (lookup time: 0, rows: 103)

Compared to :ref:`Example 1 <example_join-method_nested-loop_no-index>`,
the number of records read from the athlete table before comparing the join conditions is significantly reduced (``readrows: 313,819`` -> ``rows: 948``).

.. _example_join-method_nested-loop_driving:

.. rubric:: Example 3. Performance Differences Depending on the Driving Table

This example changes the driving table to athlete, unlike previous examples where nation was the driving table.
The **LEADING** hint is used to specify the join order, making athlete the driving table and nation the driven table.

  .. code-block:: sql

    select /*+ recompile leading(a, n) use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

  .. code-block:: none

    Query plan:

    idx-join (inner join)
        outer: sscan
                   class: a node[0]
                   sargs: term[1]
                   cost:  49 card 202
        inner: iscan
                   class: n node[1]
                   index: pk_nation_code term[0]
                   sargs: term[2]
                   cost:  2 card 31
        cost:  161 card 29

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 5, fetch: 7168, fetch_time: 0, ioread: 0)
        SCAN (table: public.athlete), (heap time: 5, fetch: 6709, ioread: 0, readrows: 6677, rows: 229)
          SCAN (index: public.nation.pk_nation_code), (btree time: 0, fetch: 458, ioread: 0, readkeys: 229, filteredkeys: 0, rows: 229) (lookup time: 0, rows: 103)

Using athlete as the driving table leads to a decrease in performance due to its higher number of records.

.. _example_join-method_nested-loop_with-hash-list:

.. rubric:: Example 4. Nested Loop Join with Hash List Scan

This example demonstrates a nested loop join where a hash list scan is performed on the driven table.
By combining the **ORDERED** and **NO_MERGE** hints, an inline view that cannot perform an index scan is used as the driven table.
Additionally, the query retrieves only the first 20 results, allowing partial range processing to be combined with the hash list scan.

  .. code-block:: sql

    select /*+ recompile ordered use_nl */
        n.name, v.name
    from
        nation as n
        inner join (
                select /*+ no_merge */
                    a.name, a.nation_code
                from
                    athlete as a
                where
                    a.event = 'Baseball'
            ) as v on n.code = v.nation_code
    where
        n.continent = 'Asia'
    limit 20;

  .. code-block:: none

    Query plan:

    nl-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   sargs: term[1] AND term[2]
                   cost:  2 card 31
        inner: sscan
                   class: v node[1]
                   sargs: term[0]
                   cost:  2 card 202
        sargs: term[1]
        cost:  148 card 29

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 26, rows: 8)
          SCAN (hash temp(m), build time: 0, time: 0, fetch: 0, ioread: 0, readrows: 249, rows: 20)
        SUBQUERY (uncorrelated)
          SELECT (time: 2, fetch: 32, fetch_time: 0, ioread: 0)
            SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 229)

Hash list scans can be confirmed through query profiling but are not explicitly shown in the execution plan.

.. _join-method_sort-merge:

Sort-Merge Join
----------------------------------------

A sort merge join combines records from two tables by first sorting them based on the join condition and then sequentially comparing the sorted records.
This method can only be performed when the join condition is an equality condition.

The process of sorting data from two tables before joining is similar to creating an index in real time.
However, the sorting process is not skipped even if a join-compatible index exists.
The process of combining the sorted data is similar to a nested loop join
in that it performs the join operation by reading one record from each of the two sorted temporary results.
The buffer size used during the sorting process can be set with the **sort_buffer_size** parameter.
For more details, refer to :ref:`memory-parameters`.

.. rubric:: Join Cost

Sort-merge joins can become inefficient when the sorting step incurs a high cost.
When the query optimizer compares the costs of join methods, nested loop joins are often evaluated as having relatively low cost because they do not require preprocessing,
Hash joins also tend to have lower cost since they build a hash table using data from only one of the two tables.
In contrast, sort-merge joins are evaluated as having higher cost because they need to sort the data from both tables.

.. rubric:: Partial Range Processing

Sort-merge joins cannot perform partial range processing because they can only proceed after the sorting of both tables is complete.
As a result, even if only the first N results are needed, the query will return results only after the entire join process has finished.

.. _example_join-method_sort-merge:

.. rubric:: Example 1. Execution Process of a Sort-Merge Join

This example demonstrates the steps involved in executing a sort-merge join.
Although the **ORDERED** hint does not directly impact join performance, it is included here to ensure the join is performed in the specified order.

  .. code-block:: sql

    create index i_nation_continent_code on nation (continent, code);
    create index i_athlete_event_nation_code on athlete (event, nation_code);

    select /*+ recompile ordered use_merge */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

    drop index i_nation_continent_code on nation;
    drop index i_athlete_event_nation_code on athlete;

  .. code-block:: none

    Query plan:

    temp
        order: UNORDERED
        subplan: m-join (inner join)
                     edge:  term[0]
                     outer: temp
                                order: code[0]
                                subplan: iscan
                                             class: n node[0]
                                             index: i_nation_continent_code term[1]
                                             cost:  2 card 1
                                cost:  8 card 1
                     inner: temp
                                order: nation_code[1]
                                subplan: iscan
                                             class: a node[1]
                                             index: i_athlete_event_nation_code term[2]
                                             cost:  2 card 7
                                cost:  8 card 7
                     cost:  17 card 1
        cost:  23 card 1

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 1, fetch: 287, fetch_time: 1, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 103, rows: 103)
          MERGELIST
            SELECT (time: 0, fetch: 52, fetch_time: 0, ioread: 0)
              SCAN (index: public.nation.i_nation_continent_code), (btree time: 0, fetch: 49, ioread: 0, readkeys: 47, filteredkeys: 0, rows: 47) (lookup time: 0, rows: 47)
              ORDERBY (time: 0, sort: true, page: 0, ioread: 0)
            SELECT (time: 1, fetch: 235, fetch_time: 1, ioread: 0)
              SCAN (index: public.athlete.i_athlete_event_nation_code), (btree time: 1, fetch: 231, ioread: 0, readkeys: 6, filteredkeys: 0, rows: 229) (lookup time: 1, rows: 229)
              ORDERBY (time: 0, sort: true, page: 0, ioread: 0)

**Execution Steps:**

1. Retrieve records from the nation table that meet the filter condition (``n.continent = 'Asia'``) and sort them by the code column.

  .. code-block:: sql

    select n.code, n.name from nation as n where n.continent = 'Asia' order by n.code;

2. Retrieve records from the athlete table that meet the filter condition (``a.event = 'Baseball'``) and sort them by the nation_code column.

  .. code-block:: sql

    select a.nation_code, a.name from athlete as a where a.event = 'Baseball' order by a.nation_code;

3. Sequentially read records from the two sorted results, compare the join condition (``n.code = a.nation_code``), and if the condition is satisfied, combine the two records and add them to the result.
4. Repeat this process until all records have been processed and the join is complete.

.. _join-method_hash:

Hash Join
----------------------------------------

A hash join combines records from two tables by using a hash function to match records with the same hash value.
To efficiently compare records with the same hash value, a hash table is used.
One of the two tables is used to **build** the hash table, while the other is used to **probe** it.
The hash value is generated by applying a hash function to the join column values.
When different input values produce the same hash value, a hash collision occurs.
In such cases, records with the same hash value are sequentially examined, and the join condition is checked.
A hash join can only be performed when all join conditions are equality conditions.

.. rubric:: SQL Hints

There are two hints associated with hash joins:

  - **USE_HASH**: Allows the query optimizer to generate an execution plan using a hash join.
  - **NO_USE_HASH**: Prevents the query optimizer from generating an execution plan using a hash join.

Both hints can specify one or more tables.
The hint applies to the inner table (driven table) if a table is explicitly specified.
If no table is specified, the hint applies to all tables.
If the same table is specified in both the **USE_HASH** and **NO_USE_HASH** hints, the **NO_USE_HASH** hint takes precedence.

.. rubric:: Cost Calculation

Each table in a hash join can either be used to build the hash table or to probe it.
To determine the most efficient approach, the costs of both options are calculated and compared, with the lower cost being selected as the hash join cost.
For example, the following scenarios are compared, and the one with the lower cost is selected:

  - Building the hash table with the outer table (driving table) and probing it with the inner table (driven table).
  - Building the hash table with the inner table (driven table) and probing it with the outer table (driving table).

.. rubric:: Table used to build the hash table

The table used to build the hash table cannot be explicitly specified using **ORDERED** or **LEADING** hints.
The table with fewer records is typically chosen for building the hash table.
However, in the case of outer joins, the table used to build the hash table is fixed based on the join direction:

  - **Left Outer Join**: The right table is used to build the hash table.
  - **Right Outer Join**: The left table is used to build the hash table.

.. rubric:: How to build the hash table

The method used to build the hash table depends on the size of the temporary results and the maximum size of the hash table.
The maximum hash table size can be configured using the **max_hash_list_scan_size** parameter.
For more details, refer to :ref:`memory-parameters`.

CUBRID supports the following hashing methods:

  - **Memory Hash**: Used when all temporary results can fit in the hash table.

    .. code-block:: none

      Number of temporary result pages * Page size <= Maximum hash table size

  - **Hybrid Hash**: Used when the hash table can store the positions of all temporary result records.

    .. code-block:: none

      Number of temporary result records * (Hash entry size + Position information size <= Maximum hash table size

  - **File Hash**:  Used when neither of the above methods is applicable.
  - **Skip Hash**: If either of the two temporary results is empty or unnecessary (in the case of an outer join), building the hash table is skipped.

**Hybrid Hash** and **File Hash** store record positions in the hash table, requiring an additional step to locate records before reading them.

**Memory Hash** and **Hybrid Hash** use memory pages to build the hash table, whereas **File Hash** uses temporary volume pages.
Accessing pages in a temporary volume incurs additional logical I/O, which can lead to performance degradation.

During query execution, once the join is complete, the hash table is deleted, and the memory it occupied is released.
The specific hash method used during a hash join can be identified through query profiling.
For details, refer to :ref:`query-profiling`.

.. rubric:: Partial Range Processing

Hash joins cannot perform partial range processing because the join can only proceed after the hash table has been fully built.

.. _example_join-method_hash:

.. rubric:: Example 1. Execution Process of a Hash Join

This example explains the steps involved in performing a hash join.
Even with the **ORDERED** hint, the table used to build the hash table cannot be explicitly specified.

  .. code-block:: sql

    select /*+ recompile ordered use_hash */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

  .. code-block:: none

    Query plan:

    hash-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   sargs: term[2]
                   cost:  2 card 31
        inner: sscan
                   class: a node[1]
                   sargs: term[1]
                   cost:  49 card 202
        cost:  63 card 29

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 103, rows: 103)
          HASHJOIN (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
            BUILD (time: 0, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: memory)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 47)
            PROBE (time: 2, probe_time: 0, fetch: 0, fetch_time: 0, ioread: 0, readkeys: 103, rows: 103, max_collisions: 1)
              SELECT (time: 2, fetch: 32, fetch_time: 0, ioread: 0)
                SCAN (table: public.athlete), (heap time: 1, fetch: 32, ioread: 0, readrows: 6677, rows: 229)

The nation table contains 47 records that satisfy the filter condition (``n.continent = 'Asia'``),
and the athlete table contains 229 records that satisfy the filter condition (``a.event = 'Baseball'``).
Thus, the nation table with fewer records is selected to build the hash table.

**Query Profiling Results:**

This section describes only the query profiling items related to hash joins. For details on other items, refer to :ref:`query-profiling`.

  1. **HASHJOIN**

    - **time**: Total execution time for hash join (ms)
    - **fetch**: Pages fetched during the hash join
    - **fetch_time**: Time spent fetching pages for the hash join (ms)
    - **ioread**: Number of physical I/O reads during page fetches for the hash join

  2. **BUILD**: Except for **time**, the remaining items do not include cumulative information when querying temporary results.

    - **time**: Total time for building the hash table, including temporary results (ms)
    - **build_time**: : Time specifically spent building the hash table (ms)
    - **fetch**: Pages fetched during the hash table build
    - **fetch_time**: Time spent fetching pages for the hash table build (ms)
    - **ioread**: Physical I/O reads during the hash table build
    - **hash_method**: Method used to build the hash table (**memory**, **hybrid**, **file**, or **skip**)

      - **memory**: Use the memory hashing method
      - **hybrid**: Use the hybrid hashing method
      - **file**:  Use the file hashing method
      - **skip**: Skip building the hash table

  3. **PROBE**: Except for **time**, the remaining items do not include cumulative information when querying temporary results.

    - **time**: Total time for probing the hash table, including temporary results (ms)
    - **probe_time**: Time specifically spent probing the hash table (ms)
    - **fetch**: Pages fetched during the hash table probe
    - **fetch_time**: Time spent fetching pages for the hash table probe (ms)
    - **ioread**: Physical I/O reads during the hash table probe
    - **readkeys**: Number of records with matching hash values during the probe
    - **rows**: Number of records satisfying the join condition (**readkeys** >= **rows**)
    - **max_collisions**: Maximum number of records read for a single hash value during probing

**Execution Steps:**

  1. Retrieve records from the nation table that satisfy the filter condition (``n.continent = 'Asia'``) and create a temporary result.

    .. code-block:: sql

      select n.code, n.name from nation as n where n.continent = 'Asia';

  2. Retrieve records from the athlete table that satisfy the filter condition (``a.event = 'Baseball'``) and create a temporary result.

    .. code-block:: sql

      select a.nation_code, a.name from athlete as a where a.event = 'Baseball';

  3. Read the temporary result from the nation table and build a hash table.
  4. For each record in the temporary result from the athlete table, probe the hash table of the nation table.
  5. Match records(``readkeys``) in the hash table with the same hash value and compare them against the join condition (``n.code = a.nation_code``). If the condition is satisfied(``rows``), combine the records and add them to the result.
  6. Repeat **step 5** for subsequent records with the same hash value.
  7. Repeat **steps 4-6** for the next record in the athlete table.
  8. Continue until all join operations are complete.

.. _example_join-method_hash_hint:

.. rubric:: 예시 2. Hash join using hints

This example demonstrates how to use the **USE_HASH** and **NO_USE_HASH** hints to control which tables perform a hash join.
The **ORDERED** hint is used to fix the inner table (driven table) for applying the hint.

  .. code-block:: sql

    select /*+ recompile ordered use_hash no_use_hash(r) */
        n.name, a.name, a.code, r.medal
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
        inner join record as r on a.code = r.athlete_code
    where
        n.continent = 'Asia'
        and a.event = 'Shooting'
        and r.host_year = 1992;

  .. code-block:: none

    Query plan:

    idx-join (inner join)
        outer: hash-join (inner join)
                   edge:  term[0]
                   outer: sscan
                              class: n node[0]
                              sargs: term[3]
                              cost:  2 card 31
                   inner: sscan
                              class: a node[1]
                              sargs: term[2]
                              cost:  49 card 202
                   cost:  63 card 29
        inner: iscan
                   class: r node[2]
                   index: pk_record_host_year_event_code_athlete_code_medal term[4] (covers)
                   filtr: term[1]
                   cost:  5 card 400
        cost:  109 card 2

The **USE_HASH** hint specifies that a hash join is performed for the nation and athlete tables.

The **NO_USE_HASH** hint ensures that a hash join is not performed for the record table.

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 5, fetch: 139, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 35, rows: 35)
          SCAN (index: public.record.pk_record_host_year_event_code_athlete_code_medal), (btree time: 3, fetch: 105, ioread: 0, readkeys: 7, filteredkeys: 0, rows: 7, covered: true)
          HASHJOIN (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
            BUILD (time: 1, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: memory)
              SELECT (time: 1, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 1, fetch: 1, ioread: 0, readrows: 215, rows: 47)
            PROBE (time: 1, probe_time: 0, fetch: 0, fetch_time: 0, ioread: 0, readkeys: 35, rows: 35, max_collisions: 1)
              SELECT (time: 1, fetch: 32, fetch_time: 0, ioread: 0)
                SCAN (table: public.athlete), (heap time: 1, fetch: 32, ioread: 0, readrows: 6677, rows: 173)

.. _example_join-method_hash_index_skip_scan:

.. rubric:: Example 3. Hash join with index skip scan

This example demonstrates using an index skip scan to retrieve temporary results during a hash join.

  .. code-block:: sql

    /* for nested loop join */
    create index i_athlete_nation_code_event on athlete (nation_code, event);

    /* for hash join */
    create index i_nation_continent_code on nation (continent, code);

    select /*+ recompile use_hash index_ss */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

    drop index i_athlete_nation_code_event on athlete;
    drop index i_nation_continent_code on nation;

  .. code-block:: none

    Query plan:

    hash-join (inner join)
        edge:  term[0]
        outer: iscan
                   class: n node[0]
                   index: i_nation_continent_code term[1]
                   cost:  2 card 1
        inner: iscan
                   class: a node[1]
                   index: i_athlete_nation_code_event term[2] (index skip scan)
                   cost:  13 card 202
        cost:  25 card 1

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 1, fetch: 748, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 103, rows: 103)
          HASHJOIN (time: 1, fetch: 748, fetch_time: 0, ioread: 0)
            BUILD (time: 0, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: memory)
              SELECT (time: 0, fetch: 52, fetch_time: 0, ioread: 0)
                SCAN (index: public.nation.i_nation_continent_code), (btree time: 0, fetch: 49, ioread: 0, readkeys: 47, filteredkeys: 0, rows: 47) (lookup time: 0, rows: 47)
            PROBE (time: 1, probe_time: 0, fetch: 0, fetch_time: 0, ioread: 0, readkeys: 103, rows: 103, max_collisions: 1)
              SELECT (time: 1, fetch: 696, fetch_time: 0, ioread: 0)
                SCAN (index: public.athlete.i_athlete_nation_code_event), (btree time: 1, fetch: 692, ioread: 0, readkeys: 121, filteredkeys: 0, rows: 229) (lookup time: 0, rows: 229)

.. _example_join-method_hash-right-outer:

.. rubric:: Example 4. Hash Right Outer Join

This example illustrates how the direction of an outer join determines which table is used to build the hash table.

  .. code-block:: sql

    select /*+ recompile use_hash */
        n.name, p.host_year, p.gold
    from
        participant as p
        right outer join nation as n on n.code = p.nation_code and p.gold > 0
    where
        n.continent = 'Asia';

  .. code-block:: none

    Query plan:

    hash-join (right outer join)
        edge:  term[0]
        outer: sscan
                   class: p node[0]
                   sargs: term[1]
                   cost:  5 card 92
        inner: sscan
                   class: n node[1]
                   sargs: term[2]
                   cost:  2 card 31
        cost:  15 card 31

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 1, fetch: 4, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 70, rows: 70)
          HASHJOIN (time: 0, fetch: 4, fetch_time: 0, ioread: 0)
            BUILD (time: 0, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: memory)
              SELECT (time: 0, fetch: 3, fetch_time: 0, ioread: 0)
                SCAN (table: public.participant), (heap time: 0, fetch: 3, ioread: 0, readrows: 916, rows: 228)
            PROBE (time: 0, probe_time: 0, fetch: 0, fetch_time: 0, ioread: 0, readkeys: 36, rows: 70, max_collisions: 5)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 47)

The participant table contains 228 records that satisfy the filter condition (``p.gold > 0``),
while the nation table contains 47 records that meet the filter condition (``n.continent = 'Asia'``).
Even though the participant table has more records, in a right outer join, the left table (participant) is used to build the hash table.

.. _example_join-method_hash-skip-build:

.. rubric:: Example 5. Hash Outer Join Skipping Hash Table Build

This example demonstrates a hash outer join where the hash table build is skipped.
In a left outer join, the right table is typically used to build the hash table.
However, since no records in the participant table satisfy the filter condition (``p.gold > 100``), the hash table build is skipped.
Instead, records from the nation table that satisfy the filter condition (``n.continent = 'Asia'``) are directly returned as the query result.

  .. code-block:: sql

    select /*+ recompile use_hash */
        n.name, p.host_year, p.gold
    from
        nation as n
        left outer join participant as p on n.code = p.nation_code and p.gold > 100
    where
        n.continent = 'Asia';

  .. code-block:: none

    Query plan:

    hash-join (left outer join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   sargs: term[2]
                   cost:  2 card 31
        inner: sscan
                   class: p node[1]
                   sargs: term[1]
                   cost:  5 card 92
        cost:  15 card 31

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 1, fetch: 4, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 0, ioread: 0, readrows: 47, rows: 47)
          HASHJOIN (time: 1, fetch: 4, fetch_time: 0, ioread: 0)
            BUILD (time: 1, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: skip)
              SELECT (time: 1, fetch: 3, fetch_time: 0, ioread: 0)
                SCAN (table: public.participant), (heap time: 1, fetch: 3, ioread: 0, readrows: 916, rows: 0)
            PROBE (time: 0, probe_time: 0, fetch: 0, fetch_time: 0, ioread: 0, readkeys: 0, rows: 47, max_collisions: 0)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 47)

In the **BUILD** section, the hash table build method is displayed as ``hash_method: skip``.
This indicates that the hash table was not built.
Instead, the records from the nation table satisfying the filter condition (``n.continent = 'Asia'``) constitute the query result.

.. _example_join-method_hash-method_hybrid:

.. rubric:: Example 6. Hash Join Using the Hybrid Hash Method

This example demonstrates how to perform a hash join using the hybrid hash method.
Because the data set is small, the hash table's maximum size must be reduced to use the hybrid hash method.
To do this, set the **max_hash_list_scan_size** parameter to **12K** (default: **8M**).

  .. code-block:: sql

    set system parameters 'max_hash_list_scan_size=12k';

    select /*+ recompile use_hash */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

  .. code-block:: none

    Query plan:

    hash-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   cost:  2 card 215
        inner: sscan
                   class: a node[1]
                   cost:  49 card 6677
        cost:  400 card 6677

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 5, fetch: 126, fetch_time: 0, ioread: 0)
        SCAN (temp time: 0, fetch: 19, ioread: 0, readrows: 6677, rows: 6677)
          HASHJOIN (time: 5, fetch: 105, fetch_time: 0, ioread: 0)
            BUILD (time: 0, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: hybrid)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 215)
            PROBE (time: 5, probe_time: 2, fetch: 49, fetch_time: 0, ioread: 0, readkeys: 6692, rows: 6677, max_collisions: 2)
              SELECT (time: 3, fetch: 53, fetch_time: 0, ioread: 0)
                SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 6677)

When more than one join hint is used on the same table,
the query optimizer compares the costs of the specified join methods and selects the one with the lower cost.
The selected join method can be verified through the generated execution plan.
The following is an example where the **USE_HASH** and **USE_NL** hints are used together,
and the query optimizer selects the hybrid hash method, generating and executing the execution plan as a hash join.

  .. code-block:: sql

    set system parameters 'max_hash_list_scan_size=12k';

    select /*+ recompile use_hash use_nl */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

  .. code-block:: none

    Query plan:

    hash-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   cost:  2 card 215
        inner: sscan
                   class: a node[1]
                   cost:  49 card 6677
        cost:  400 card 6677

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 5, fetch: 126, fetch_time: 0, ioread: 0)
        SCAN (temp time: 1, fetch: 19, ioread: 0, readrows: 6677, rows: 6677)
          HASHJOIN (time: 4, fetch: 105, fetch_time: 0, ioread: 0)
            BUILD (time: 0, build_time: 0, fetch: 0, fetch_time: 0, ioread: 0, hash_method: hybrid)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 215)
            PROBE (time: 4, probe_time: 2, fetch: 49, fetch_time: 0, ioread: 0, readkeys: 6692, rows: 6677, max_collisions: 2)
              SELECT (time: 2, fetch: 53, fetch_time: 0, ioread: 0)
                SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 6677)

.. _example_join-method_hash-method_file:

.. rubric:: Example 7. Hash Join Using the File Hash Method

This example demonstrates how to perform a hash join using the file hash method.
Because the data set is small, the hash table's maximum size must be reduced to below **12K** to use the file hash method.
To do this, set the **max_hash_list_scan_size** parameter to **8K** (default: **8M**).

  .. code-block:: sql

    set system parameters 'max_hash_list_scan_size=8k';

    select /*+ recompile use_hash */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

  .. code-block:: none

    Query plan:

    hash-join (inner join)
        edge:  term[0]
        outer: sscan
                   class: n node[0]
                   cost:  2 card 215
        inner: sscan
                   class: a node[1]
                   cost:  49 card 6677
        cost:  3846 card 6677

The query optimizer evaluates whether memory hashing or hybrid hashing can be used by calculating based on statistics collected by the database:

  .. code-block:: none

    Temporary result record count * (Hash entry size + Location info size) <= Maximum hash table size
                                                 215 * (32 byte * 16 byte) <= 8,192 byte
                                                                    10,320 <= 8192 byte (False)

Since the calculated size exceeds the maximum size of the hash table, the file hash method is chosen instead of the memory or hybrid method.
File hash methods (cost: 3,846) are more expensive than other hash methods (cost: 400) due to the use of temporary volume pages.

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 10, fetch: 27510, fetch_time: 3, ioread: 0)
        SCAN (temp time: 0, fetch: 19, ioread: 0, readrows: 6677, rows: 6677)
          HASHJOIN (time: 10, fetch: 27489, fetch_time: 3, ioread: 0)
            BUILD (time: 1, build_time: 1, fetch: 645, fetch_time: 1, ioread: 0, hash_method: file)
              SELECT (time: 0, fetch: 1, fetch_time: 0, ioread: 0)
                SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 215, rows: 215)
            PROBE (time: 9, probe_time: 7, fetch: 26772, fetch_time: 2, ioread: 0, readkeys: 6692, rows: 6677, max_collisions: 2)
              SELECT (time: 2, fetch: 53, fetch_time: 0, ioread: 0)
                SCAN (table: public.athlete), (heap time: 1, fetch: 32, ioread: 0, readrows: 6677, rows: 6677)

When compared with the query profiling results from :ref:`Example 6 <example_join-method_hash-method_hybrid>`,
It can be seen that the execution time of the file hash method is slow in proportion to the cost.

When more than one join hint is used on the same table,
the query optimizer compares the costs of the specified join methods and selects the one with the lower cost.
The selected join method can be verified through the generated execution plan.
The following is an example where the **USE_HASH** and **USE_NL** hints are used together,
and the query optimizer generates and executes a plan with a nested loop join (cost: 3,405) because the cost of a hash join (cost: 3,846) using the file hash method) is high.

  .. code-block:: sql

    set system parameters 'max_hash_list_scan_size=8k';

    select /*+ recompile use_hash use_nl */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

  .. code-block:: none

    Query plan:

    idx-join (inner join)
        outer: sscan
                   class: a node[1]
                   cost:  49 card 6677
        inner: iscan
                   class: n node[0]
                   index: pk_nation_code term[0] (covers)
                   cost:  2 card 215
        cost:  3405 card 6677

  .. code-block:: none

    Trace Statistics:
      SELECT (time: 19, fetch: 13387, fetch_time: 1, ioread: 0)
        SCAN (table: public.athlete), (heap time: 4, fetch: 6709, ioread: 0, readrows: 6677, rows: 6677)
          SCAN (index: public.nation.pk_nation_code), (btree time: 10, fetch: 6677, ioread: 0, readkeys: 6677, filteredkeys: 0, rows: 0, covered: true, count_only: true)
