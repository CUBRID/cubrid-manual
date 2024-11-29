.. _join-method:

Join Methods
========================================

.. Background

Relational databases store data by dividing it into multiple tables through normalization.
Normalization is effective in reducing data redundancy and ensuring data integrity.
However, querying the integrated results of separated data requires combining relationships between tables.
Joins combine multiple tables through these relationships and return the results as if they were a single table.

.. Overview & Join Type

Join queries are used to combine data from multiple tables, views, or derived tables to retrieve the desired information.
The records included in the result may vary depending on the join type used.
For more details, refer to :ref:`join-query`.

.. note::

    Here, **Table** is used as a broad concept that includes not only actual tables but also views (virtual tables) and derived tables.

.. Join Method

The way records are combined between tables varies depending on the join method.
The join method is determined by the query optimizer through cost comparison, and can also be specified manually by the user.

CUBRID supports the following join methods:

- :ref:`join-method_nested-loop`
- :ref:`join-method_sort-merge`
- :ref:`join-method_hash`

If a query includes two or more joins, each join method can be applied differently.
All join methods perform only a join between two tables, and each join method is performed sequentially.
That is, the next join is performed after the previous one is completed.

The query optimizer calculates the cost of each join method based on statistics collected by the database.
It then selects the method with the lowest cost.
However, in CUBRID, generating execution plans for sort merge join and hash join is not considered by default.
To consider generating execution plans for both join methods, **SQL Hint** must explicitly be specified.
For more details, refer to :ref:`sql-hint`.

By setting the **optimizer_enable_merge_join** parameter to **yes** (default is **no**),
sort merge join can be performed without using the **USE_MERGE** hint.
For more details, refer to :ref:`other-parameters`.

.. _join-method_nested-loop:

Nested Loop Join
----------------------------------------

.. Basic Explanation

A nested loop join is a method of reading records from an outer table one by one,
finding records in the inner table that can be combined with records from the outer table, and combining them.
This method is similar to nested loops in programming.
The table used in the outer loop is called a driving table, and the table in the inner loop is called a driven table.

.. Driving Table

When setting one of the two tables as the driven table, it is more efficient to choose the table with fewer records.
This is because the fewer records there are in the driving table, the fewer join operations are required.
Since the random access cost is generally constant, when random access to a driven table is possible, the join cost decreases as the number of join operations is reduced.
As join costs decrease, join performance improves.
The query optimizer automatically selects the driving table by comparing join costs.
Users can also specify the driving table directly using the **ORDERED** or **LEADING** hint.

.. Driven Table

A nested loop join is efficient when random access to the driven table is possible.
For random access to be possible, an index suitable for the join condition must be created in advance in the driven table.
When an index is not created in advance, all records in the driven table are read sequentially, which degrades join performance.
When random access to a driven table is possible through an index,
a nested loop join (``nl-join``) is output as an index join (``idx-join``) in the execution plan.

.. Partial Range Processing

A nested iterative join does not perform the join operation after reading all records from both tables.
The join operation is performed by reading one record from each of the two tables, and the join result can be returned immediately.
This approach allows for partial range processing,
and returns results when N results are filled, in cases where only the first N results are needed.
Improves join performance by not performing unnecessary join operations.

.. Hash List Scan

Even when random access to driven tables is not possible,
CUBRID supports random access through hash list scan if all join conditions are equal comparison conditions.
A hash list scan is performed in the same way as a hash join, except that it builds the hash table as a driven table.
Additionally, hash list scan can be used with nested loop join to allow partial range processing,
whereas hash joindoes not support this functionality.
For more details, refer to :ref:`join-method_hash`.

Whether hash list scans were used can be confirmed through query profiling.
For more details, refer to :ref:`query-profiling`.

.. _example_join-method_nested-loop_no-index:

.. rubric:: Example 1. Nested loop join without index scans

In this example, an index scan is not performed on the driven table when performing a nested loop join.
**ORDERED** hint is used to fix the join order,
as a result, the nation table becomes the driving table, and the athlete table becomes the driven table.
Since the driven table does not have an index created that is suitable for the join condition,
all records in the athlete table must be read sequentially for each record in the nation table.

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
    :emphasize-lines: 4

    Trace Statistics:
      SELECT (time: 92, fetch: 1720, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (table: public.athlete), (heap time: 92, fetch: 1504, ioread: 0, readrows: 313819, rows: 103)

**수행 과정:**

1. Read a record from the nation table that satisfies the filter condition (``n.continent = 'Asia'``).
2. Read a record from the athlete table that satisfies the filter condition (``a.event = 'Baseball'``).
3. Compare the join condition (``n.code = a.nation_code``) for the current records from both tables. If the condition is satisfied, combine the two records and add them to the result.
4. Read the next record from the athlete table that satisfies the filter condition and repeat step 3.
5. Read the next record from the nation table that satisfies the filter condition and repeat steps 2-4.
6. Continue this process until all join operations are completed.

.. _example_join-method_nested-loop_index:

.. rubric:: Example 2.Nested loop join with index scans

In this example, an index scan is performed on the driven table when performing a nested loop join.
Add an index suitable for the join condition to the driven table.
As a result, there is no need to read all the records in the athlete table for each record in the nation table, and random access is possible.

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
    :emphasize-lines: 4

    Trace Statistics:
      SELECT (time: 1, fetch: 1265, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 1, fetch: 216, ioread: 0, readrows: 215, rows: 47)
          SCAN (index: public.athlete.i_athlete_nation_code), (btree time: 0, fetch: 1045, ioread: 0, readkeys: 24, filteredkeys: 0, rows: 948) (lookup time: 0, rows: 103)

Compared to the query profiling results from :ref:`Example 1 <example_join-method_nested-loop_no-index>`,
the number of records read from the athlete table has significantly decreased before comparing the join conditions.. (313,819 -> 948)

.. _example_join-method_nested-loop_driving:

.. rubric:: Example 3. Performance differences depending on driving table

This example uses the athlete table as the driving table, unlike the previous two examples which used the nation table as the driving table.
**LEADING** hint is used to specify the join order,
as a result, the athlete table becomes the driving table, and the athlete table becomes the driven table.

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

Compared to the query profiling results in `:ref:Example 2 <example_join-method_nested-loop_index>`,
using the athlete table as the driving table results in decreased performance due to the higher record count.

.. code-block:: none
    :emphasize-lines: 2

    Trace Statistics:
      SELECT (time: 5, fetch: 7168, fetch_time: 0, ioread: 0)
        SCAN (table: public.athlete), (heap time: 5, fetch: 6709, ioread: 0, readrows: 6677, rows: 229)
          SCAN (index: public.nation.pk_nation_code), (btree time: 0, fetch: 458, ioread: 0, readkeys: 229, filteredkeys: 0, rows: 229) (lookup time: 0, rows: 103)

.. _example_join-method_nested-loop_with-hash-list:

.. rubric:: Example 4. Nested Loop Join with Hash List Scan

This example shows a hash list scan being performed on a driven table.
By using the **ORDERED** and **NO_MERGE** hints together, an inline view that cannot perform an index scan becomes the driven table.
Then, only the first 20 results are retrieved so that hash list scan and partial range processing can be performed together.

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
    :emphasize-lines: 4

    Trace Statistics:
      SELECT (time: 2, fetch: 33, fetch_time: 0, ioread: 0)
        SCAN (table: public.nation), (heap time: 0, fetch: 1, ioread: 0, readrows: 26, rows: 8)
          SCAN (hash temp(m), build time: 0, time: 0, fetch: 0, ioread: 0, readrows: 249, rows: 20)
        SUBQUERY (uncorrelated)
          SELECT (time: 2, fetch: 32, fetch_time: 0, ioread: 0)
            SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 229)

Whether a hash list scan was performed on the driven table cannot be confirmed from the execution plan,
but can be known through the query profiling results.

.. code-block:: none

    SCAN (hash temp(m), build time: 0, time: 0, fetch: 0, ioread: 0, readrows: 249, rows: 20)
    
- ``hash temp(m)``: The hash table was built using a memory(m) hash method.
- ``rows: 20``: The join operation was performed only on the first 20 results and ended
    
.. code-block:: none

    SUBQUERY (uncorrelated)
      SELECT (time: 2, fetch: 32, fetch_time: 0, ioread: 0)
        SCAN (table: public.athlete), (heap time: 2, fetch: 32, ioread: 0, readrows: 6677, rows: 229)
    
To build the hash table, all records that satisfy the filter condition (``a.event = 'Baseball'``) were read from the athlete table.

.. _join-method_sort-merge:

Sort Merge Join
----------------------------------------

.. Basic Explanation

Sort merge join is a method that sorts the records of two tables based on the join condition and then sequentially compares the sorted records to combine them.
This method can only be performed if the join conditions are an equality conditions.

.. Sorting Process

The process of sorting the records of two tables before the join is similar to creating an index in real time.
The process of sequentially combining the sorted records is similar to the way a nested loop join works.
The sorting process is not skipped even if an index suitable for the join condition exists.

The buffer size used in the sorting process can be configured with the **sort_buffer_size** parameter.
For more details, refer to :ref:`memory-parameters`.

Sort merge join can become inefficient if the sorting process is expensive.
Nested loop join requires no pre-processing, and hash join requires data from only one of the two tables to build the hash table.
Both methods can be evaluated as having lower costs compared to sort merge join, which requires sorting two tables.

.. Partial Range Processing

Sort merge join is performed only after both tables are sorted, which makes partial range processing impossible.
Therefore, even if only the first N results are needed, the results can only be retrieved after the join is fully completed.

.. _example_join-method_sort-merge:

.. rubric:: Example 1. Execution Process of a Sort Merge Join

This example shows the process of performing a sort merge join.
Although the **ORDERED** hint does not affect join performance, it is used here to ensure the join is executed in the same order as below.

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

**수행 과정:**

1. Read records that satisfy the filter condition (``n.continent = 'Asia'``) from the nation table, and sort them by the code column value.

    .. code-block:: sql

        select n.code, n.name from nation as n where n.continent = 'Asia' order by n.code;

2. Read records that satisfy the filter condition (``a.event = 'Baseball'``) from the athlete table and sort them by the natione_code column value.

    .. code-block:: sql

        select a.nation_code, a.name from athlete as a where a.event = 'Baseball' order by a.nation_code;

3. Read records one by one sequentially from the two sorted results, compare the join condition (``n.code = a.nation_code``), and if the condition is satisfied, combine the two records and add them to the result.
4. Continue this process until all join operations are completed.

.. _join-method_hash:

Hash Join
----------------------------------------

.. Basic Explanation

Hash join is a method of combining records from two tables with the same hash value using a hash function.
A hash table is used to collect records with the same hash value for efficient comparison.
One of the two tables is used to build the hash table, and the other is used to probe the hash table.
The hash value is generated by a hash function using the join column values ​​as input.
When the same hash value is generated from different input values, it is called a hash collision.
When a hash collision occurs, sequentially probe records with the same hash value and compare the join conditions.
Hash join can be performed only when all join conditions are equal comparison conditions.

.. 힌트

There are two hints related to hash joins:

- **USE_HASH**: Enables the query optimizer to generate execution plans for hash joins.
- **NO_USE_HASH**: Disables the query optimizer from generating execution plans for hash joins.

Both hints can be used by specifying one or more tables.
The hint is applied when the table specified in the hint is an inner table (driven table).
If no table is specified in the hint, the hint is applied to all tables.
When **USE_HASH** and **NO_USE_HASH** hints are used together on the same table, only **NO_USE_HASH** hint is applied.

.. Cost Calculation

Each table participating in a hash join can be used to build a hash table or to probe a hash table.
So, the costs of both cases are calculated and compared, and the lowest cost is determined as the hash join cost.

For example, use the lowest cost among the following two cases:

- Cost of building a hash table with the outer table (driving table), and probing the hash table with the inner table (driven table).
- Cost of building a hash table with the inner table (driven table), and probing the hash table with the outer table (driving table).

.. The Table used to build the hash table

The table that builds the hash table cannot be specified using the **ORDERED** or **LEADING** hints.
Build a hash table using a table with few records.
However, in the case of outer joins, the table used to build the hash table is fixed depending on the join direction.

- Left outer join: The right table is used to build the hash table.
- Right outer join: The left table is used to build the hash table.

.. Hash Method

The method for building a hash table depends on the size of the temporary results and the maximum hash table size.
The maximum hash table size can be configured with the **max_hash_list_scan_size** parameter.
For more details, refer to :ref:`memory-parameters`.

CUBRID supports the following hashing methods:

- **Memory Hash**: Used when all temporary results can be stored in a hash table. **(Number of pages in the temporary result * Page size ≤ Maximum hash table size)**
- **Hybrid Hash**: Used when the record position can be stored in a hash table as many times as the number of temporary results. **(Number of records in the temporary result * (Hash entry size + Location info size) ≤ Maximum hash table size)**
- **File Hash**: Used when all other methods are not feasible.

**Hybrid Hash** and **File Hash** store record positions in a hash table, which adds a step to navigate to the record position.

**Memory Hash** and **Hybrid Hash** build the hash table using memory pages,
but **File Hash** builds the hash table using pages from the temporary volume.
**File Hash** may cause latches when accessing pages of a temporary volume, which may result in performance degradation.

The method actually used when performing a hash join can be found out through query profiling.
For details, refer to :ref:`query-profiling`.

Even while executing a query, the hash table is discarded when the join is completed, and the used memory is also returned.

.. Partial Range Processing

Partial range processing is not possible with hash join because the join is performed after the hash table build is completed.

.. _example_join-method_hash:

.. rubric:: Example 1. Execution Process of a Hash Join

This example shows the process of performing a hash join.
Even if the **ORDERED** hint is used, the table on which to build the hash table cannot be specified.

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

There are 47 records that satisfy the filter condition (``n.continent = 'Asia'``) in the nation table,
and 229 records that satisfy the filter condition (``a.event = 'Baseball'``) in the athlete table.
Therefore, the nation table with fewer records is used to build the hash table.

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

1. Retrieve records from the nation table that satisfy the filter condition (``n.continent = 'Asia'``) and create a temporary result.

    .. code-block:: sql

        select n.code, n.name from nation as n where n.continent = 'Asia';

2. Retrieve records from the athlete table that satisfy the filter condition (``a.event = 'Baseball'``) and create a temporary result.

    .. code-block:: sql

        select a.nation_code, a.name from athlete as a where a.event = 'Baseball';

3. Read the temporary result from the nation table and build a hash table.
4. For each record in the temporary result from the athlete table, probe the hash table.
5. Find records in the hash table with the same hash value, and compare them against the join condition (``n.code = a.nation_code``). If the condition is satisfied, combine the records and add them to the result.
6. Repeat step 5 for the next record with the same hash value.
7. Repeat steps 4~6 for the next record in the athlete table.
8. Continue until all join operations are complete.

.. _example_join-method_hash_hint:

.. rubric:: 예시 2. Hash join using hints

This example shows using the **USE_HASH** and **NO_USE_HASH** hints to specify which tables should perform a hash join and which should not.
**ORDERED** hint is used to fix the inner table (driven table) to which the hint is applied.

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

Using the USE_HASH hint, the nation and athlete tables perform a hash join.
The NO_USE_HASH hint is used by specifying the record table, so hash join is not performed with the record table.

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

.. _example_join-method_hash_index:

.. rubric:: Example 3. Hash Join with Index Design Differing from Nested Loop Join

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

.. rubric:: Example 4. Right Outer Hash Join

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

The participant table contains 228 records that satisfy the filter condition (p.gold > 0),
while the nation table contains 47 records that meet the filter condition (n.continent = 'Asia').
Since a right outer join always uses the left table to build the hash table,
the participant table is used to create the hash table in this case.

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

.. _example_join-method_hash-left-outer:

.. rubric:: Example 5. Hash Outer Join Without Hash Table Creation

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

In the **BUILD** section, the hash method used for hash table creation is displayed as hash_method: skip.
The value **skip** indicates that hash table creation was omitted.

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

.. _example_join-method_hash-method_hybrid:

.. rubric:: Example 6. Hash Join Using Hybrid Hash Method

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

.. rubric:: Example 7. Hash Join Using File Hash Method

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

.. code-block:: none

    Temporary result record count * (Hash entry size + Location info size) <= Maximum hash table size
        => 215 * (32 byte * 16 byte) <= 8,192 byte
	=> 10,320 <= 8192 byte
	=> false

Even when the nation table, which has the fewest records, is used to build the hash table, the maximum hash table size is exceeded.
As a result, the file hash method is used. Since this method relies on temporary volume pages, its cost is higher than the hybrid hash method (400 vs. 3,846).

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

Compared to the query profiling results from :ref:`Example 5 <example_join-method_hash-method_hybrid>`,
it is evident that performance decreases when the file hash method is used.

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
