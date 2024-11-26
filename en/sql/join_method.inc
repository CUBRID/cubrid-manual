.. _join-method:

Join Method
========================================

.. Background Explanation

In relational databases, data is divided and stored across multiple tables through normalization.
This process is effective for reducing data redundancy and maintaining integrity,
but joining operations are required to retrieve separated data by combining relationships between tables.
Joins are a core function of DBMS that allow dispersed data to be queried as if it were a single table, playing a crucial role in SQL syntax.

.. Basic Explanation

Join queries are used to combine related data from two or more tables, views, or derived tables to extract the desired information.
The records included in the join result vary depending on the type of join selected.
Detailed information can be found in :ref:`join-query`.

.. note::

    From this point onward, the term **table** includes not only actual tables but also views (virtual tables) and derived tables as a comprehensive concept.

.. Join Method

The way records are combined between tables is determined by the join method, which is automatically selected by the query optimizer.

CUBRID supports the following join methods:

- :ref:`join-method_nested-loop`
- :ref:`join-method_sort-merge`
- :ref:`join-method_hash`

A single query can involve multiple joins, and different methods may be used for each join.
All join methods process only the combination of two tables at a time, and the join operations proceed sequentially.
That is, the next join is performed after the previous one is completed.

The query optimizer calculates the cost of each method based on statistics collected by the database and selects the most efficient method (minimum cost).
However, in CUBRID, the query optimizer does not consider sort merge joins and hash joins by default.
To use these methods, **SQL hints** must be explicitly added, as described in :ref:`sql-hint`.

Additionally, by setting the **optimizer_enable_merge_join** parameter to **yes** (default: **no**), sort merge joins can be performed without using the **USE_MERGE** hint.
Further details can be found in :ref:`other-parameters`.

.. _join-method_nested-loop:

Nested Loop Join
----------------------------------------

.. Basic Explanation

A nested loop join is a method in which records are read one by one from an outer table, and matching records are found and joined from an inner table.
This method is similar to a nested loop in programming.
The table used in the outer loop is called the driving table, and the table used in the inner loop is called the driven table.

.. Driving Table

If either table can be accessed randomly, it is more efficient to choose the table with fewer records as the driving table.
This is because the cost of random access is independent of the number of records in the table, and having fewer records in the driving table reduces the number of join operations.
The query optimizer automatically selects the driving table by comparing costs, but users can also specify it explicitly using **ORDERED** or **LEADING** hints.

.. Driven Table

A nested loop join is efficient when random access to the driven table is possible.
To enable this, an index that matches the join condition must be pre-created.
Otherwise, all records in the driven table will be read sequentially, leading to performance degradation.
When the driven table is accessed randomly through an index, the execution plan displays the nested loop join (**nl-join**) as an index join (**idx-join**).

.. Partial Range Processing

A nested loop join does not read all table records at once but processes one record at a time and immediately returns the result.
This allows partial range processing, meaning the join can be terminated early when only the first N results are required, enabling efficient queries.

.. Hash List Scan

Even when random access to the driven table is not possible, CUBRID supports random access via hash list scan, provided all join conditions are equality conditions.
The process of building and probing the hash table is similar to a hash join, as detailed in :ref:`join-method_hash`.
While hash joins only support full range processing, hash list scans can be combined with nested loop joins to enable partial range processing.
Whether a hash list scan is used can be verified through query profiling, as described in :ref:`query-profiling`.

.. _example_join-method_nested-loop_no-index:

.. rubric:: Example 1. Nested Loop Join with a Driven Table Without an Index

This example demonstrates the process of a nested loop join when the driven table lacks an index.
Using the **ORDERED** hint, the join order is fixed, with the nation table as the driving table and the athlete table as the driven table.
Since there is no index matching the join condition, all records in the athlete table are read sequentially for each record in the nation table, and the join condition is evaluated.

.. code-block:: sql

    select /*+ recompile ordered use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

::

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

::

    Trace Statistics:
      SELECT (time: 92, fetch: 1720, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (table: public.athlete), (heap time: 92, fetch: 1504, ioread: 0, readrows: 313819, rows: 103)

**수행 과정:**

1. Read records from the nation table that satisfy the filter condition (n.continent = 'Asia').
2. Read records from the athlete table that satisfy the filter condition (a.event = 'Baseball').
3. Compare the current records from both tables against the join condition (n.code = a.nation_code). If the condition is satisfied, combine the two records and add them to the result.
4. Read the next record from the athlete table that satisfies the filter condition and repeat step 3.
5. Read the next record from the nation table that satisfies the filter condition and repeat steps 2~4.
6. Repeat the process until all join operations are complete.

.. _example_join-method_nested-loop_index:

.. rubric:: Example 2. Nested Loop Join with an Indexed Driven Table

This example demonstrates the result of performing a nested loop join after adding an index to the driven table.

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

::

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

Compared to the profiling results from :ref:`Example 1 <example_join-method_nested-loop_no-index>`,
the number of records read from the athlete table has significantly decreased. (readrows: 313,819 -> rows: 948)

::

    Trace Statistics:
      SELECT (time: 1, fetch: 1265, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 1, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (index: public.athlete.i_athlete_nation_code), (btree time: 0, fetch: 1045, ioread: 0, readkeys: 24, filteredkeys: 0, rows: 948) (lookup time: 0, rows: 103)

.. _example_join-method_nested-loop_with-leading:

.. rubric:: Example 3. Performance Difference Based on the Record Count of the Driving Table

This example demonstrates the performance difference when using the athlete table as the driving table, specified with the **LEADING** hint.

.. code-block:: sql

    select /*+ recompile leading(a, n) use_nl */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

According to the execution plan, 31 records in the nation table satisfy the filter condition (n.continent = 'Asia'),
and 202 records in the athlete table satisfy the filter condition (a.event = 'Baseball').

::

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

Compared to the profiling results in :ref:`예시 2 <example_join-method_nested-loop_index>`,
using the athlete table as the driving table results in decreased performance due to the higher record count.

::

    Trace Statistics:
      SELECT (time: 5, fetch: 7168, fetch_time: 0, ioread: 0)
        SCAN (table: public.athlete), (heap time: 5, fetch: 6709, ioread: 0, readrows: 6677, rows: 229)
          SCAN (index: public.nation.pk_nation_code), (btree time: 0, fetch: 458, ioread: 0, readkeys: 229, filteredkeys: 0, rows: 229) (lookup time: 0, rows: 103)

.. _example_join-method_nested-loop_with-hash-list:

.. rubric:: Example 4. Nested Loop Join with Hash List Scan (for Partial Range Processing)

This example demonstrates the process of a nested loop join when the driven table uses hash list scan.
Hash list scans are performed on views or derived tables. To prevent view merge optimization, the **NO_MERGE** hint is used.
In this query, only the first 20 results are retrieved, allowing partial range processing.

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

::

    Query plan:

    sscan
        class: a node[0]
        sargs: term[0]
        cost:  49 card 202

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

The athlete table is fully scanned to create the hash table for records satisfying the filter condition (a.event = 'Baseball').
Subsequently, random access through the hash table is performed for each record in the nation table, and only the first 20 results are retrieved.

::

    Trace Statistics:
      SELECT (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 26, rows: 8)
          SCAN (hash temp(m), build time: 0, time: 0, fetch: 0, ioread: 0, readrows: 249, rows: 20)
        SUBQUERY (uncorrelated)
          SELECT (time: 2, fetch: 32, fetch_time: 0, ioread: 0)
            SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 229)

.. _join-method_sort-merge:

Sort Merge Join
----------------------------------------

.. Basic Explanation

A sort merge join combines records by first sorting the records of two tables based on the join condition and then sequentially comparing the sorted records.
This method can only be used when the join condition involves equality.

.. Sorting Process

Before performing the join, the records of both tables are sorted.
This process is similar to creating an index in real-time.
The subsequent process of sequentially combining sorted records resembles the operation of a nested loop join.
Even if an index with the same sorting order as the join column already exists, the sorting step is not skipped.

The buffer size used during the sorting process can be configured with the **sort_buffer_size** parameter. For more details, refer to :ref:`memory-parameters`.

While sort merge joins can become inefficient if the sorting process incurs high costs,
nested loop joins do not require any preparatory steps, and hash joins create a hash table by reading only one table's records.
Thus, these methods may be evaluated as less costly compared to sort merge joins.

.. Partial Range Processing

Sort merge joins require that both tables are fully sorted before the join can be performed. As a result, partial range processing is not possible.
Even if only the first N results are needed, the join must be fully completed before the results can be retrieved.

.. _example_join-method_sort-merge:

.. rubric:: Example 1. Execution Process of a Sort Merge Join

This example explains the execution process of a sort merge join.
Although the **ORDERED** hint does not affect join performance, it is used here to ensure the join is executed in the same order as described.

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

::

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

::

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

**수행 과정:**

1. Retrieve records from the nation table that satisfy the filter condition (n.continent = 'Asia'), and sort them based on the code column.

    .. code-block:: sql

        select n.code, n.name from nation as n where n.continent = 'Asia' order by n.code;

2. Retrieve records from the athlete table that satisfy the filter condition (a.event = 'Baseball'), and sort them based on the nation_code column.

    .. code-block:: sql

        select a.nation_code, a.name from athlete as a where a.event = 'Baseball' order by a.nation_code;

3. Sequentially read the sorted results from both tables and compare them against the join condition (n.code = a.nation_code). If the condition is satisfied, combine the two records and add them to the result.

.. _join-method_hash:

Hash Join
----------------------------------------

.. Basic Explanation

A hash join combines records from two tables by using a hash function to assign the same hash value to records that meet the join condition.
During this process, a hash table is created to group records with the same hash value, enabling efficient comparisons.
One table is used to build the hash table, and the other is used to probe the created hash table.
Hash values are generated by the hash function based on the values of the join columns.

If different input values generate the same hash value, this is called a hash collision.
When collisions occur, the records are sequentially checked against the join condition to ensure correctness.
Hash joins can only be performed when the join condition is based on equality.

.. Cost Calculation

The cost of a hash join is calculated for both scenarios:
Using one table to build the hash table and the other to probe it, and vice versa.
The optimizer selects the configuration with the minimum cost.

.. Hash Table Build

The table used to build the hash table cannot be explicitly specified using **ORDERED** or **LEADING** hints.
Typically, the table with fewer records is selected.
However, for outer joins, the table used for hash table creation is fixed based on the join direction:

- Left outer join: The right table builds the hash table.
- Right outer join: The left table builds the hash table.

.. Hash Method

The method for creating a hash table depends on the size of the temporary results and the maximum hash table size.
The maximum hash table size is controlled by the **max_hash_list_scan_size** parameter. For more details, refer to :ref:`memory-parameters`.

CUBRID supports the following hashing methods:

- **Memory Hash**: Used when the entire temporary result can fit in the hash table. **(Number of pages in the temporary result * Page size ≤ Maximum hash table size)**
- **Hybrid Hash**: Used when Memory Hash is not feasible, but the hash table can store location information for all records in the temporary result. **(Number of records in the temporary result * (Hash entry size + Location info size) ≤ Maximum hash table size)**
- **File Hash**: Used when neither Memory Hash nor Hybrid Hash is feasible.

**Hybrid Hash** and **File Hash** store location information for records in the hash table, requiring an additional step to fetch and read the actual records.
While **Memory Hash** and **Hybrid Hash** create the hash table in memory, **File Hash** uses pages from temporary volumes.
Accessing temporary volume pages may result in latching, leading to performance degradation.

The actual hashing method used can be verified through query profiling. For details, refer to :ref:`query-profiling`.
Even during query execution, once the join is complete, the hash table is discarded, and the memory used is released.

.. Partial Range Processing

Hash joins cannot support partial range processing because the join operation can only be performed after the hash table is fully built.

.. _example_join-method_hash:

.. rubric:: Example 1. Execution Process of a Hash Join

This example illustrates the execution process of a hash join.

Although the **ORDERED** hint fixes the join order, the table used to build the hash table is not predetermined.

.. code-block:: sql

    select /*+ recompile ordered use_hash */
        n.name, a.name
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code
    where
        n.continent = 'Asia'
        and a.event = 'Baseball';

::

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

The nation table contains 47 records that satisfy the filter condition (n.continent = 'Asia'),
while the athlete table contains 229 records that meet the filter condition (a.event = 'Baseball').
As a result, the nation table, which has fewer records, is used to build the hash table.

::

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

**Query Profiling Results:**

1. **HASHJOIN**

    - time: The total time taken to perform the hash join (in milliseconds).
    - fetch: The number of page fetches during the hash join.
    - fetch_time : The time spent fetching pages during the hash join (in milliseconds).
    - ioread: The number of I/O read operations where the disk was physically accessed during the hash join.

2. **BUILD**

    - time: The time taken to scan the table used for hash table creation, including the time spent building the hash table (build_time).
    - build_time: The time taken to construct the hash table (in milliseconds).
    - fetch: The number of page fetches during the hash table build phase.
    - fetch_time : The time spent fetching pages during the hash table build phase (in milliseconds).
    - ioread: The number of I/O read operations where the disk was physically accessed during the hash table build phase.
    - hash_method: The method used to create the hash table (e.g., memory, hybrid, file, or skip).

3. **PROBE**

    - time: The time taken to scan the table used for probing the hash table, including the time spent probing (probe_time).
    - probe_time: The time taken to search the hash table for matching records (in milliseconds).
    - fetch: The number of page fetches during the hash table probe phase.
    - fetch_time : The time spent fetching pages during the hash table probe phase (in milliseconds).
    - ioread: The number of I/O read operations where the disk was physically accessed during the hash table probe phase.
    - readkeys: The number of records with matching hash values found during the hash table probe.
    - rows: The number of records satisfying the join condition during the hash table probe phase. (rows <= readkeys)
    - max_collisions: The maximum number of records with the same hash value during the hash table probe phase.

**수행 과정:**

1. Retrieve records from the nation table that satisfy the filter condition (n.continent = 'Asia') and create a temporary result.

    .. code-block:: sql

        select n.code, n.name from nation as n where n.continent = 'Asia';

2. Retrieve records from the athlete table that satisfy the filter condition (a.event = 'Baseball') and create a temporary result.

    .. code-block:: sql

        select a.nation_code, a.name from athlete as a where a.event = 'Baseball';

3. Read the temporary result from the nation table and build a hash table.
4. For each record in the temporary result from the athlete table, probe the hash table.
5. Find records in the hash table with the same hash value, and compare them against the join condition (n.code = a.nation_code). If the condition is satisfied, combine the records and add them to the result.
6. Repeat step 5 for the next record with the same hash value.
7. Repeat steps 4~6 for the next record in the athlete table.
8. Continue until all join operations are complete.

.. _example_join-method_hash_index:

.. rubric:: Example 2. Hash Join with Index Design Differing from Nested Loop Join

This example illustrates how index design can differ between a nested loop join and a hash join.

Nested Loop Join: Requires an index on the driven table that matches the join condition to allow random access.
Hash Join: Retrieves a temporary result filtered by the filter condition before the join, independent of the join condition, requiring a different indexing approach compared to a nested loop join.

While designing indexes that support both join methods is ideal, if this is not feasible, the **INDEX_SS** hint can be used to consider performing an index skip scan.
An index skip scan allows skipping over leading columns of an index, but its usage is limited when the join condition involves multiple columns.

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

::

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

::

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

.. rubric:: Example 3. Right Outer Hash Join

This example demonstrates how the table used to create the hash table is determined based on the join direction in an outer join.
In a right outer join, the left table is always used to build the hash table.

.. code-block:: sql

    select /*+ recompile use_hash */
        n.name, p.host_year, p.gold
    from
        participant as p
        right outer join nation as n on n.code = p.nation_code and p.gold > 0
    where
        n.continent = 'Asia';

::

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

The participant table contains 228 records that satisfy the filter condition (p.gold > 0),
while the nation table contains 47 records that meet the filter condition (n.continent = 'Asia').
Since a right outer join always uses the left table to build the hash table,
the participant table is used to create the hash table in this case.

::

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

.. _example_join-method_hash-left-outer:

.. rubric:: Example 4. Hash Outer Join Without Hash Table Creation

This example illustrates a scenario where hash table creation is skipped during an outer join.
In a left outer join, the right table is typically used to build the hash table.

If either table contains no data, the hash table is not created.
In this example, the participant table contains no records satisfying the filter condition (p.gold > 100), so the hash table creation is skipped.
As a result, only the filtered records from the left table (nation) satisfying the condition (n.continent = 'Asia') are retrieved as the query result.

.. code-block:: sql

    select /*+ recompile use_hash */
        n.name, p.host_year, p.gold
    from
        nation as n
        left outer join participant as p on n.code = p.nation_code and p.gold > 100
    where
        n.continent = 'Asia';

::

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

In the **BUILD** section, the hash method used for hash table creation is displayed as hash_method: skip.
The value **skip** indicates that hash table creation was omitted.

::

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

.. _example_join-method_hash-method_hybrid:

.. rubric:: Example 5. Hash Join Using Hybrid Hash Method

This example demonstrates the execution results of a hash join that uses the hybrid hash method.

To enable the hybrid hash method, the **max_hash_list_scan_size** parameter is set to **12K** (default: **8M**).

.. code-block:: sql

    set system parameters 'max_hash_list_scan_size=12k';

    select /*+ recompile use_hash */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

::

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

::

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

The following demonstrates how the query optimizer selects the join method when both **USE_NL** and **USE_HASH** hints are provided in the same query.
In this case, a hash join is still performed.

.. code-block:: sql

    set system parameters 'max_hash_list_scan_size=12k';

    select /*+ recompile use_hash use_nl */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

::

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

::

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

.. rubric:: Example 6. Hash Join Using File Hash Method

This example demonstrates the execution results of a hash join that uses the file hash method.
To enable the file hash method, the **max_hash_list_scan_size** parameter is set to **8K** (default: **8M**).

.. code-block:: sql

    set system parameters 'max_hash_list_scan_size=8k';

    select /*+ recompile use_hash */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

The query optimizer determines, based on database statistics, that neither the memory hash nor the hybrid hash method can be used.

::

    Temporary result record count * (Hash entry size + Location info size) <= Maximum hash table size
        => 215 * (32 byte * 16 byte) <= 8,192 byte
	=> 10,320 <= 8192 byte
	=> false

Even when the nation table, which has the fewest records, is used to build the hash table, the maximum hash table size is exceeded.
As a result, the file hash method is used. Since this method relies on temporary volume pages, its cost is higher than the hybrid hash method (400 vs. 3,846).

::

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

Compared to the query profiling results from :ref:`Example 5 <example_join-method_hash-method_hybrid>`,
it is evident that performance decreases when the file hash method is used.

::

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

The following demonstrates how the query optimizer selects the join method when both **USE_NL** and **USE_HASH** hints are provided in the same query.
Due to the high cost of the file hash method, a nested loop join is performed in this case.

.. code-block:: sql

    set system parameters 'max_hash_list_scan_size=8k';

    select /*+ recompile use_hash use_nl */
        count (*)
    from
        nation as n
        inner join athlete as a on n.code = a.nation_code;

    set system parameters 'max_hash_list_scan_size=8M';

::

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

::

    Trace Statistics:
      SELECT (time: 19, fetch: 13387, fetch_time: 1, ioread: 0)
        SCAN (table: public.athlete), (heap time: 4, fetch: 6709, ioread: 0, readrows: 6677, rows: 6677)
          SCAN (index: public.nation.pk_nation_code), (btree time: 10, fetch: 6677, ioread: 0, readkeys: 6677, filteredkeys: 0, rows: 0, covered: true, count_only: true)
