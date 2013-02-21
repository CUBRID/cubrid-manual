************
Partitioning
************

Partitioning is a method by which a table is divided into multiple independent logical units. Each logical unit used in partitioning is called a partition. Partitioning can enhance manageability, performance and availability. Some advantages of partitioning are as follows:

*   Improved management of large capacity tables
*   Improved performance by narrowing the range of access when retrieving data
*   Improved performance and decreased physical loads by distributing disk I/O
*   Decreased possibility of data corruption and improved availability by partitioning a table into multiple chunks
*   Optimized storage cost

Three types of partitioning methods are supported by CUBRID: range partition, hash partition, and list partition.

The maximum number of partitions cannot exceed 1,024. Each partition of a table is created as its subtable. The subtables created by the partitioning process cannot be altered or deleted by users. The name of the subtable is stored in the system table in a '*class_name__p__partition_name*' format. Database users can check the partitioning information in the **db_class** and **db_partition** views. They can also check the information by using the ;sc <table name> command in the CUBRID Manager or the CSQL Interpreter.

.. _partition-data-type:

Partitioning key
================

The data types of columns allowed as partitioning expressions are as follows:

*   **CHAR**
*   **VARCHAR**
*   **NCHAR**
*   **VARNCHAR**
*   **SMALLINT**
*   **INT**
*   **BIGINT**
*   **DATE**
*   **TIME**
*   **TIMESTAMP**
*   **ENUM**

The following shows operators and functions that can be used in partitioning expressions.

*   Operator functions associated with number

    +, -, \*, /, :func:`MOD`, :func:`FLOOR`, :func:`CEIL`, :func:`POWER`, :func:`ROUND`, :func:`ABS`, :func:`TRUNC`

*   Operator functions associated with literal

    :func:`POSITION`, :func:`SUBSTRING`, :func:`OCTET_LENGTH`, :func:`BIT_LENGTH`, :func:`CHAR_LENGTH`, :func:`LOWER`, :func:`UPPER`, :func:`TRIM`, :func:`LTRIM`, :func:`RTRIM`, :func:`LPAD`, :func:`RPAD`, :func:`REPLACE`, :func:`TRANSLATE`

*   Operator functions associated with date

    :func:`ADD_MONTHS`, :func:`LAST_DAY`, :func:`MONTHS_BETWEEN`, :func:`SYS_DATE`, :func:`SYS_TIME`, :func:`SYS_TIMESTAMP`, :func:`TO_DATE`, :func:`TO_NUMBER`, :func:`TO_TIME`, :func:`TO_TIMESTAMP`, :func:`TO_CHAR`

*   Others

    :func:`EXTRACT`, :func:`CAST`


Range Partitioning
==================

.. _defining-range-partitions:

Range Partitioning Definition
-----------------------------

You can define a range partition by using the **PARTITION BY RANGE** clause. ::

    CREATE TABLE table_name (
       ...
    )
    PARTITION BY RANGE ( <partition_expression> ) (
       PARTITION partition_name VALUES LESS THAN ( <range_value> ),
       PARTITION partition_name VALUES LESS THAN ( <range_value> ),
       ... 
    )

*   *partition_expression* : Specifies the partition expression. The expression can be specified by the name of the column to be partitioned or by a function. For details of the data types and functions available, see Data Types Available for Partition Expression.
*   *partition_name* : Specifies the partition name.
*   *range_value* : Specifies the partition-by value.

The following example shows how to create the *participant2* table with the participating countries, and insert data that partitions the years into before and after the 2000 Olympic Games. When inserting data, the countries that participated in the 1988 and 1996 Olympic Games are stored in *before_2000*; the rest of them are stored in *before_2008*.

.. code-block:: sql

    CREATE TABLE participant2 (host_year INT, nation CHAR(3), gold INT, silver INT, bronze INT)
    PARTITION BY RANGE (host_year) (
      PARTITION before_2000 VALUES LESS THAN (2000),
      PARTITION before_2008 VALUES LESS THAN (2008)
    );
     
    INSERT INTO participant2 VALUES (1988, 'NZL', 3, 2, 8);
    INSERT INTO participant2 VALUES (1988, 'CAN', 3, 2, 5);
    INSERT INTO participant2 VALUES (1996, 'KOR', 7, 15, 5);
    INSERT INTO participant2 VALUES (2000, 'RUS', 32, 28, 28);
    INSERT INTO participant2 VALUES (2004, 'JPN', 16, 9, 12);

As shown below, the partition key value in a range partition is **NULL**, the data are stored in the first partition.

.. code-block:: sql

    INSERT INTO participant2 VALUES (NULL, 'AAA', 0, 0, 0);
    
.. note::

    *   The maximum number of partitions possible for a given table is 1024.
    
    *   If the partition key value is **NULL**, the data is stored in the first partition (see Example 2).

.. _range-partitioning-redefinition:

Range Partitioning Redefinition
-------------------------------

You can redefine a partition by using the **REORGANIZE PARTITION** clause of the **ALTER** statement. By redefining partitions, you can combine multiple partitions into one or divide one into multiple. ::

    ALTER {TABLE | CLASS} table_name
    REORGANIZE PARTITION <alter_partition_name_comma_list>
    INTO ( <partition_definition_comma_list> )
     
    partition_definition_comma_list ::=
    PARTITION partition_name VALUES LESS THAN ( <range_value> ), ... 

*   *table_name* : Specifies the name of the table to be redefined.
*   *alter_partition_name_comma_list* : Specifies the partition to be redefined. Multiple partitions are separated by commas (,).
*   *partition_definition_comma_list* : Specifies the redefined partitions. Multiple partitions are separated by commas (,).

The following example shows how to perform repartitioning the *before_2000* partition into the *before_1996* and *before_2000* partitions.

.. code-block:: sql

    CREATE TABLE participant2 (host_year INT, nation CHAR(3), gold INT, silver INT, bronze INT)
    PARTITION BY RANGE (host_year) (
      PARTITION before_2000 VALUES LESS THAN (2000),
      PARTITION before_2008 VALUES LESS THAN (2008)
    );
     
    ALTER TABLE participant2 
    REORGANIZE PARTITION before_2000 INTO (
      PARTITION before_1996 VALUES LESS THAN (1996),
      PARTITION before_2000 VALUES LESS THAN (2000)
    );

The following example shows how to combine two partitions redefined in the above example back into a single *before_2000* partition.

.. code-block:: sql

    ALTER TABLE participant2 
    REORGANIZE PARTITION before_1996, before_2000 INTO (
      PARTITION before_2000 VALUES LESS THAN (2000)
    );

.. note::

    *   When redefining a range or list partition, duplicate ranges or values are not allowed.
    
    *   The **REORGANIZE PARTITION** clause cannot be used to change the partition table type. For example, a range partition cannot be changed to a hash partition, or vice versa.
    
    *   The maximum number of partitions cannot exceed 1,024. There must be at least one partition remaining after deleting partitions. In a range-partitioned table, only adjacent partitions can be redefined.

Adding Range Partitioning
-------------------------

You can add range partitions by using the **ADD PARTITION** clause of the **ALTER** statement. ::

    ALTER {TABLE | CLASS} table_name
    ADD PARTITION <partition_definitions_comma_list>
    
    partition definition_comma_list ::=
    PARTITION partition_name VALUES LESS THAN ( <range_value> ), ...

*   *table_name* : Specifies the name of the table to which partitions are added.
*   *partition_definition_comma_list* : Specifies the partitions to be added. Multiple partitions are separated by commas (,).

Currently, the partition before the 2008 Olympic Games is defined in the *participant2* table. The following example shows how to add the *before_2012* and *last_one* partitions; the former will contain the information about the 2012 Olympic Games and the latter will have the information about the later ones.

.. code-block:: sql

    ALTER TABLE participant2 ADD PARTITION (
      PARTITION before_2012 VALUES LESS THAN (2012),
      PARTITION last_one VALUES LESS THAN MAXVALUE
    );

.. note::

    *   When a range partition is added, only the partition by value greater than the existing partition value can be added. Therefore, as shown in the above example, if the maximum value is specified by **MAXVALUE**, no more partitions can be added (you can add partitions by changing the **MAXVALUE** value by redefining the partition).

    *   To add the partition by value smaller than the existing partition value, use the redefining partitions (see :ref:`range-partitioning-redefinition`).

Dropping Range Partitioning
---------------------------

You can drop a partition by using the **DROP PARTITION** clause of the **ALTER** statement. ::

    ALTER {TABLE | CLASS} table_name
    DROP PARTITION partition_name
    
*   *table_name* : Specifies the name of the partitioned table.
*   *partition_name* : Specifies the name of the partition to be dropped.

The following example shows how to drop the *before_2000* partition in the *participant2* table.

.. code-block:: sql

    ALTER TABLE participant2 DROP PARTITION before_2000;

.. note::

    *   When dropping a partitioned table, all stored data in the partition are also dropped.
    
    *   If you want to change the partitioning of a table without losing data, use the **ALTER TABLE** ... **REORGANIZE PARTITION** statement.
    
    *   The number of rows deleted is not returned when a partition is dropped. If you want to delete the data, but want to maintain the table and partitions, use the **DELETE** statement.

Hash Partitioning
=================

Hash Partitioning Definition
----------------------------

You can define a hash partition by using the **PARTITION BY HASH** clause. ::

    CREATE TABLE table_name (
       ...
    )
    PARTITION BY HASH ( <partition_expression> )
    PARTITIONS ( number_of_partitions )

*   *partition_expression* : Specifies a partition expression. The expression can be specified by the name of the column to be partitioned or by a function.
*   *number_of_partitions* : Specifies the number of partitions.

The following example shows how to create the *nation2* table with country *code* and country names, and define 4 hash partitions based on code values. Only the number of partitions, not the name, is defined in hash partitioning; names such as p0 and p1 are assigned automatically.

.. code-block:: sql

    CREATE TABLE nation2 (
      code CHAR (3),
      name VARCHAR (50)
    )
    PARTITION BY HASH (code) 
    PARTITIONS 4;

The following example shows how to insert data to the hash partition created in the above example. When a value is inserted into a hash partition, the partition to store the data is determined by the hash value of the partition key. If the partition key value is **NULL**, the data is stored in the first partition.

.. code-block:: sql

    INSERT INTO nation2 VALUES ('KOR', 'Korea');
    INSERT INTO nation2 VALUES ('USA', 'USA United States of America');
    INSERT INTO nation2 VALUES ('FRA', 'France');
    INSERT INTO nation2 VALUES ('DEN', 'Denmark');
    INSERT INTO nation2 VALUES ('CHN', 'China');
    INSERT INTO nation2 VALUES (NULL, 'AAA');


.. note::

    The maximum number of partitions cannot exceed 1024.

Hash Partitioning Redefinition
------------------------------

You can redefine a partition by using the **COALESCE PARTITION** clause of the **ALTER** statement. Instances are preserved if the hash partition is redefined. ::

    ALTER {TABLE | CLASS} table_name
    COALESCE PARTITION number_of_shrinking_partitions

*   *table_name* : Specifies the name of the table to be redefined.
*   *number_of_shrinking_partitions* : Specifies the number of partitions to be deleted.

The following example shows how to decrease the number of partitions in the *nation2* table from 4 to 3.

.. code-block:: sql

    ALTER TABLE nation2 COALESCE PARTITION 1;

.. note::

    *   Decreasing the number of partitions is only available.
    
    *   To increase the number of partitions, use the **ALTER TABLE** ... **ADD PARTITION** statement as in range partitioning.
    
    *   There must be at least one partition remaining after redefining partitions.

List Partitioning
=================

List Partitioning Definition
----------------------------

You can define a list partition by using the **PARTITION BY LIST** statement. ::

    CREATE TABLE table_name (
      ...
    )
    PARTITION BY LIST ( <partition_expression> ) (
      PARTITION partition_name VALUES IN ( <partition_value_list> ),
      PARTITION partition_name VALUES IN ( <partition_value_list> ),
      ... 
    )
    
*   *partition_expression* : Specifies a partition expression. The expression can be specified by the name of the column to be partitioned or by a function. For details on the data types and functions available, see :ref:`Data Types Available for Partition Expressions <partition-data-type>`.
*   *partition_name* : Specifies the partition name.
*   *partition_value_list* : Specifies the list of the partition by values.

The following example shows how to create the *athlete2* table with athlete names and sport events, and define list partitions based on event values.

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR (30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics'),
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing'),
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball')
    );

The following example shows how to insert data to the list partition created in the above example. In the last query of the following example, if you insert an argument that has not been specified in the partition expression of the above example, data inserting fails.

.. code-block:: sql

    INSERT INTO athlete2 VALUES ('Hwang Young-Cho', 'Athletics');
    INSERT INTO athlete2 VALUES ('Lee Seung-Yuop', 'Baseball');
    INSERT INTO athlete2 VALUES ('Cho In-Chul', 'Judo');
    INSERT INTO athlete2 VALUES ('Hong Kil-Dong', 'Volleyball');

The following example shows in which an error occurs with no data inserted when the partition key value is **NULL**. 

.. code-block:: sql

    INSERT INTO athlete2 VALUES ('Hong Kil-Dong', NULL);
     
To define a partition where a **NULL** value can be inserted, define one that has a list including a **NULL** value as in the event3 partition as below.

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR (30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics' ),
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing'),
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball', NULL)
    );

.. note::

    The maximum number of partitions cannot exceed 1,024.

List Partitioning Redefinition
------------------------------

You can redefine a partition by using the **REORGANIZE PARTITION** clause of the **ALTER** statement. By redefining partitions, you can combine multiple partitions into one or divide one into multiple. ::

    ALTER {TABLE | CLASS} table_name
    REORGANIZE PARTITION alter_partition_name_comma_list
    INTO ( <partition_definition_comma_list> )
    
    partition_definition_comma_list ::=
    PARTITION <partition_name> VALUES IN ( <partition_value_list> ), ... 
    
*   *table_name* : Specifies the name of the table to be redefined.
*   *alter_partition_name_comma_list* : Specifies the partition to be redefined. Multiple partitions are separated by commas (,).
*   *partition_definition_comma_list* : Specifies the redefined partitions. Multiple partitions are separated by commas (,).

The following example shows how to create the *athlete2* table partitioned by the list of sport events, and redefine the *event2* partition to be divided into *event2_1* (Judo) and *event2_2* (Taekwondo, Boxing).

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR(30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics'),
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing'),
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball')
    );

    ALTER TABLE athlete2 
    REORGANIZE PARTITION event2 INTO (
        PARTITION event2_1 VALUES IN ('Judo'),
        PARTITION event2_2 VALUES IN ('Taekwondo', 'Boxing')
    );

The following example shows how to combine the *event2_1* and *event2_2* partitions divided in Example 1 back into a single *event2* partition.

.. code-block:: sql

    ALTER TABLE athlete2 
    REORGANIZE PARTITION event2_1, event2_2 INTO (
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing')
    );

Dropping List Partitioning
--------------------------

You can drop a partition by using the **DROP PARTITION** clause of the **ALTER** statement. ::

    ALTER {TABLE | CLASS} table_name
    DROP PARTITION partition_name

*   *table_name* : Specifies the name of the partitioned table.
*   *partition_name* : Specifies the name of the partition to be dropped.

The following example shows how to create the *athlete2* table partitioned by the list of sport events and drop the *event3* partition.

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR (30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics' ),
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing'),
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball')
    );
    
    ALTER TABLE athlete2 DROP PARTITION event3;

Partitioning key and Charset, Collation
=======================================

Partitioning keys of partition table should have the same character set with the column. Therefore, the following case is not allowed.

.. code-block:: sql

    CREATE TABLE t (c CHAR(50) COLLATE utf8_bin) 
    PARTITION BY LIST (c) (
        PARTITION p0 VALUES IN (_utf8'x'),
        PARTITION p1 VALUES IN (_iso88591'y')
    );
    
You can specify the collation on the partition table. The following example shows that *tbl* is defined as the case insensitive utf8_en_ci collation; Since it is considered that partitioning key 'test' and 'TEST' are the same, the table creation fails.

.. code-block:: sql

    CREATE TABLE tbl (str STRING) COLLATE utf8_en_ci 
    PARTITION BY LIST (str) (
        PARTITION p0 VALUES IN ('test'), 
        PARTITION p1 VALUES IN ('TEST')
    );
    
    ERROR: Partition definition is duplicated. 'p1'
 
A hash partition whose partition key has a non-binary collation is not allowed. 

.. code-block:: sql

    CREATE TABLE tbl (code VARCHAR (10)) COLLATE utf8_de_exp_ai_ci 
    PARTITION BY HASH (code) PARTITIONS 4;

    ERROR: before ' ; '
    Unsupported partition column type.


Retrieval and Manipulation data from Partition
==============================================

Local Index and Global Index for Partitioning
---------------------------------------------

Indexes created on a partitioning table are classified into Local Index or Global Index. Global Index defines one index structure that maintains data from all partitions. However, Local Index defines one index for one partition. The operators cannot control the index to be Local Index or Global Index. The index type is automatically determined by the system.

*   All primary keys are Global Index.
*   All foreign keys are Local Index.
*   All non-unique indexes are Local Index.
*   A unique index is Local Index or Global Index. If the partition key is a unique index, the index is Local Index; otherwise, it is Global Index.

Partition Pruning
-----------------

Partition pruning is an optimization, limiting the scope of your query according to the criteria you have specified. It is the skipping of unnecessary data partitions in a query. By doing this, you can greatly reduce the amount of data output from the disk and time spent on processing data as well as improve query performance and resource availability.

.. note::

    In versions lower than CUBRID 9.0, partition pruning has been executed at the query compiling stage. However, in version of CUBRID 9.0 or higher, it is executed at the server side at the query execution stage. Therefore, in version of CUBRID 9.0 or higher, partition pruning can be executed for more complex and various queries than existing versions. However, it is not available to print out the query information for a partitioning pruning query and optimization of **ORDER BY SKIP**, and **GROUP BY SKIP** is not supported.

The following example shows how to create the *olympic2* table to be partitioned based on the year the Olympic Games were held, and retrieve the countries that participated in the Olympic Games since the 2000 Sydney Olympic Games. In the **WHERE** clause, partition pruning takes place when equality or range comparison is performed between a partition key and a constant value.

In this example, the *before_1996* partition that has smaller year values than 2000 is not scanned.

.. code-block:: sql

    CREATE TABLE olympic2 (opening_date DATE, host_nation VARCHAR (40))
    PARTITION BY RANGE (EXTRACT (YEAR FROM opening_date)) (
        PARTITION before_1996 VALUES LESS THAN (1996),
        PARTITION before_MAX VALUES LESS THAN MAXVALUE
    );
     
    SELECT opening_date, host_nation 
    FROM olympic2 
    WHERE EXTRACT (YEAR FROM (opening_date)) >= 2000;


The following example shows how to retrieve the method of getting the effects of partition pruning by retrieving data with a specific partition when partition pruning does not occur. In the first query, partition pruning does not occur because the value compared is not in the same format as that of the partition expression. Therefore, you can use the same effect of partition pruning by specifying the appropriate partition as shown in the second query.

.. code-block:: sql

    -- pruning cannot be applied
    SELECT host_nation 
    FROM olympic2 
    WHERE opening_date >= '2000-01-01';

    -- to access a specific partition
    SELECT host_nation 
    FROM olympic2 PARTITION (before_max) 
    WHERE opening_date >= '2000-01-01';

The following example shows how to specify the search condition to make a partition pruning in the hash partitioned table, called the *manager* table. For hash partitioning, partition pruning occurs only when equality comparison is performed between a partition key and a constant value in the **WHERE** clause.

.. code-block:: sql

    CREATE TABLE manager (
        code INT,
        name VARCHAR (50)
    )
    PARTITION BY HASH (code) PARTITIONS 4;
     
    SELECT * FROM manager WHERE code = 10053;

.. note::

    *   The partition expression and the value compared must be in the same format.
    
    *   To enable pruning for hash partitioning and list partitioning, use the following partitioning key expression in the **WHERE** clause. The following constant expression does not include any table columns and any other conditions are not allowed.

        *   <*partitioning key*> = <*constant expression*>
        *   <*partitioning key*> { IN | = SOME | = ANY } ( <*constant expression list*> )

    *   To enable pruning for range partitioning, use the following partitioning key expression in the **WHERE** clause.

        *   <*partitioning key*> { < | > | = | <= | >= } <*constant expression*>
        *   <*partitioning key*> BETWEEN <*constant expression*> AND <*constant expression*>

To access a specific partition
------------------------------

.. (TODO - translation)

.. 데이터를 SELECT/INSERT/UPDATE할 때 특정 분할을 명시적으로 지정하여 접근할 수 있다. 특정 분할을 지정할 때 분할 테이블 이름을 명시하지 않고 분할 이름만 명시하여 지정할 수 있도록 PARTITION 절을 지원한다. PARTITION 절은 분할 테이블 이름 뒤에 명시할 수 있으며, SELECT 문 뿐만 아니라 분할을 사용할 수 있는 모든 SQL에 사용할 수 있다. 

.. When SELECT/UPDATE/DELETE data, it is possible to access for each partition with "**PARTITION** (partition_name)" syntax.

.. code-block:: sql

    -- to specify a partition with its table name
    SELECT * FROM athlete2__p__event2;
    
    -- to specify a partition with PARTITION clause
    SELECT * FROM athlete2 PARTITION (event2);
    

The following example shows how to create the *athlete2* table to be partitioned by the list of sport events, insert data, and retrieve the *event1* and *event2* partitions.

.. code-block:: sql

    CREATE TABLE athlete2( name VARCHAR(40), event VARCHAR(30) )
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics' ),
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo','Boxing'),
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball')
    );

    INSERT INTO athlete2 VALUES ('Hwang Young-Cho', 'Athletics');
    INSERT INTO athlete2 VALUES ('Lee Seung-Yuop', 'Baseball');
    INSERT INTO athlete2 VALUES ('Lee Sun-Hee','Taekwondo');
    INSERT INTO athlete2 VALUES ('Kim In-Chul', 'Judo');

    SELECT * FROM athlete2 PARTITION (event1);
      name                  event
    ============================================
      'Hwang Young-Cho'     'Athletics'

    SELECT * FROM athlete2 PARTITION (event2);
      name                  event
    ============================================
      'Lee Sun-Hee'         'Taekwondo'
      'Kim In-Chul'         'Judo'

The following shows to INSERT one row on the *event1* partition of the *athlete2* table.

.. code-block:: sql

    INSERT INTO athlete2 PARTITION(event1) VALUES ('Lee Bong-Ju', 'Athletics');

The following shows to UPDATE one row on the *event2* partition of the *athlete2* table.

.. code-block:: sql

    UPDATE athlete2 PARTITION(event2) SET name='Cho In-Chul' WHERE name='Kim In-Chul';

.. (TODO - translation)

.. INSERT 문 등에 PARTITION 절을 명시했을 때 지정된 분할이 정의와 다를 경우에는 오류가 반환된다.

.. code-block:: sql

    CREATE TABLE t (i INTEGER) 
    PARTITION BY RANGE (i) (
      PARTITION p0 VALUES LESS THAN (10), 
      PARTITION p1 VALUES LESS THAN (100)
    );
    
    -- success
    INSERT INTO t PARTITION (p0) VALUES (2);
    
    -- error -1108
    INSERT INTO t PARTITION (p0) VALUES (20);

.. WHERE 절을 가지는 질의에 대해 특정 분할을 직접 참조하면 분할 프루닝 과정을 수행하지 않게 되는 성능상의 (작은) 이점이 있으며, 또한 일반적으로 분할 테이블에는 적용되지 못하는 INDEX JOIN, ORDER BY 및 GROUP BY 생략 최적화, 다중 키 범위 최적화, INDEX SKIP SCAN 등의 질의 처리 기법이 사용될 수 있다.


Moving Data by Changing Partitioning Key Value
----------------------------------------------

If a partition key value is changed, the changed instance can be moved to another partition by the partition expression.

The following example shows how to move the instance to another partition by changing the partition key value. If you change the sport event information of Hwang Young-Cho in the *event1* partition from 'Athletics' to 'Football', the instance is moved to the *event3* partition.

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR (40), event VARCHAR (30))
    PARTITION BY LIST (event) (
        PARTITION event1 VALUES IN ('Swimming', 'Athletics' ),
        PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing'),
        PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball')
    );
    
    INSERT INTO athlete2 VALUES ('Hwang Young-Cho', 'Athletics');
    INSERT INTO athlete2 VALUES ('Lee Seung-Yuop', 'Baseball');

    SELECT * FROM athlete2 PARTITION (event1);
    
      name                  event
    ============================================
      'Hwang Young-Cho'     'Athletics'

    UPDATE athlete2 SET event = 'Football' WHERE name = 'Hwang Young-Cho';

    SELECT * FROM athlete2 PARTITION (event3);
    
      name                  event
    ============================================
      'Lee Seung-Yuop'      'Baseball'
      'Hwang Young-Cho'     'Football'


.. note::

    Be aware that when moving data between partitions by changing a partition key value, it can cause performance degradation due to internal deletions and insertions.

Creating VIEW with Partitioning Table
-------------------------------------

You can define a VIEW by using each partition of a partitioned table.

The following example shows how to create the *participant2* table partitioned based on the participating year, and create and retrieve a virtual table with the *before_2000* partition of the *participant2* table.

.. code-block:: sql

    CREATE TABLE participant2 (host_year INT, nation CHAR(3), gold INT, silver INT, bronze INT)
    PARTITION BY RANGE (host_year) (
        PARTITION before_2000 VALUES LESS THAN (2000),
        PARTITION before_2008 VALUES LESS THAN (2008)
    );

    INSERT INTO participant2 VALUES (1988, 'NZL', 3, 2, 8);
    INSERT INTO participant2 VALUES (1988, 'CAN', 3, 2, 5);
    INSERT INTO participant2 VALUES (1996, 'KOR', 7, 15, 5);
    INSERT INTO participant2 VALUES (2000, 'RUS', 32, 28, 28);
    INSERT INTO participant2 VALUES (2004, 'JPN', 16, 9, 12);

    CREATE VIEW v_2000 AS
    SELECT * FROM participant2 PARTITION (before_2000)
    WHERE host_year = 1988;

    SELECT * FROM v_2000;
    
        host_year  nation                       gold       silver       bronze
    ==========================================================================
             1988  'NZL'                           3            2            8
             1988  'CAN'                           3            2            5

    
Partitioning Management
=======================

Altering Regular Table into Partitioning Table
----------------------------------------------

To alter a regular table into a partitioned one, use the **ALTER TABLE** statement. Three partitioning methods can be used with the **ALTER TABLE** statement. The data in the existing table are moved to and stored in each partition according to the partition definition. ::

    ALTER {TABLE | CLASS} table_name
    PARTITION BY RANGE ( <partition_expression> )
    ( PARTITION partition_name VALUES LESS THAN { MAXVALUE | ( <partition_value_option> ) }, ... )

    ALTER {TABLE | CLASS} table_name
    PARTITION BY LIST ( <partition_expression> )
    ( PARTITION partition_name VALUES IN ( <partition_value_option_list> ), ... )
    
    ALTER {TABLE | CLASS} table_name
    PARTITION BY HASH ( <partition_expression> )
    PARTITIONS number_of_hash_partition

    <partition_expression> ::= expression
    <partition_value_option> ::= literal

*   *table_name* : Specifies the name of the table to be altered.
*   *partition_expression* : Specifies a partition expression. The expression can be specified by the name of the column to be partitioned or by a function. For details on the data types and functions available, see :ref:`Data Types Available for Partition Expressions <partition-data-type>`.
*   *partition_name* : Specifies the name of the partition.
*   *partition_value_option* : Specifies the value or the value list on which the partition is based.

The following are examples of altering the record table into a range, list and hash table respectively.

.. code-block:: sql

    ALTER TABLE record PARTITION BY RANGE (host_year) (
        PARTITION before_1996 VALUES LESS THAN (1996),
        PARTITION after_1996 VALUES LESS THAN MAXVALUE
    );

    ALTER TABLE record PARTITION BY LIST (unit) (
        PARTITION time_record VALUES IN ('Time'),
        PARTITION kg_record VALUES IN ('kg'),
        PARTITION meter_record VALUES IN ('Meter'),
        PARTITION score_record VALUES IN ('Score')
    );

    ALTER TABLE record PARTITION BY HASH (score) PARTITIONS 4;


.. note::

    If there is data that does not satisfy the partition condition, partitions cannot be defined.

Altering Partitioning Table into Regular Table
----------------------------------------------

To alter an existing partitioned table into a regular one, use the **ALTER TABLE** statement. Removing partition does not mean that the data of a table will be deleted. ::

    ALTER {TABLE | CLASS} table_name REMOVE PARTITIONING

*   *table_name* : Specifies the name of the table to be altered.

The following example shows how to alter the partitioned table of name *nation2* into a regular one.

.. code-block:: sql

    ALTER TABLE nation2 REMOVE PARTITIONING;

Partition PROMOTE Statement
---------------------------

Partition **PROMOTE** statement promotes the operator-specified partition on the partition table to a general standalone table. This is useful to retain the old data, which is rarely accessed, to archive only. By promoting the partition to a general table, useful data has less partitions, reducing the access load and archiving the old data in a convenient manner.

The partition **PROMOTE** statement is allowed for the range partition table and the list partition table only. Promotion of the hash partition table is not allowed since it cannot be controlled by an operator.

When the partition is promoted to a standalone table, the table inherits the data and local indexes only. It means that the following table attributes are not saved in the promotion table.

*   Primary Key
*   Foreign key
*   Unique index
*   **AUTO_INCREMENT** attribute and serial
*   Triggers
*   Methods
*   Inheritance relationship (super-class and sub-class)

The following attributes are used as they are on the promoted table:

*   Record attributes (column types)
*   Table attributes
*   Local indexes (general indexes, not the unique indexes and primary keys)

**Constraints**

*   If the partition table includes any foreign key, the partition cannot be promoted.
*   Promoting the hash partition table is not allowed.

::

    ALTER TABLE table_name PROMOTE PARTITION <partition_name_list>

*   <*partition_name_list*>: The name of a partition to promote

The following example shows promotion of list partition:

.. code-block:: sql

    CREATE TABLE t (i INT) PARTITION BY LIST (i) (
        PARTITION p0 VALUES IN (1, 2, 3),
        PARTITION p1 VALUES IN (4, 5, 6),
        PARTITION p2 VALUES IN (7, 8, 9),
        PARTITION p3 VALUES IN (10, 11, 12)
    );
     
    ALTER TABLE t PROMOTE PARTITION p1, p2;

After promotion, the partition of the *t* table has *p0* and *p3* only and *p1* and *p2* can be accessed through the *t__p__p1* table and the *t__p__p2* table, respectively. ::

    csql> ;schema t
    
    === <Help: Schema of a Class> ===
    
     <Class Name>
         t
         
     <Sub Classes>
         t__p__p0
         t__p__p3
         
     <Attributes>
         i                    INTEGER
         
     <Partitions>
         PARTITION BY LIST ([i])
         PARTITION p0 VALUES IN (1, 2, 3)
         PARTITION p3 VALUES IN (10, 11, 12)
     
    csql> ;schema t__p__p1
    
    === <Help: Schema of a Class> ===
    
     <Class Name>
         t__p__p1
         
     <Attributes>
         i                    INTEGER
         
         
The following example shows promotion of range partition.

.. code-block:: sql

    CREATE TABLE t (i INT, j INT) PARTITION BY RANGE (i) (
        PARTITION p0 VALUES LESS THAN (1),
        PARTITION p1 VALUES LESS THAN (10),
        PARTITION p2 VALUES LESS THAN (100),
        PARTITION p3 VALUES LESS THAN MAXVALUE
    );
     
    CREATE UNIQUE INDEX u_t_i ON t (i);
    CREATE INDEX i_t_j ON t (j);
     
    ALTER TABLE t PROMOTE PARTITION p1, p2;

After promotion, the partition of the *t* table has *p0* and *p3* only and *p1* and *p2* can be accessed through the *t__p__p1* table and the *t__p__p2* table, respectively. Note that some attributes or indexes such as the primary keys, foreign keys, and unique keys have been removed from *t__p__p1* and *t__p__p2*, the promoted tables. ::

    csql> ;schema t
    
    === <Help: Schema of a Class> ===
    
     <Class Name>
         t
         
     <Sub Classes>
         t__p__p0
         t__p__p3
         
     <Attributes>
         i                    INTEGER
         j                    INTEGER
         
     <Constraints>
        UNIQUE u_t_i ON t (i)
        INDEX i_t_j ON t (j)
        
     <Partitions>
         PARTITION BY RANGE ([i])
         PARTITION p0 VALUES LESS THAN (1)
         PARTITION p3 VALUES LESS THAN MAXVALUE
     
    csql> ;schema t__p__p1
    
    === <Help: Schema of a Class> ===
    
     <Class Name>
         t__p__p1
         
     <Attributes>
         i                    INTEGER
         j                    INTEGER
         
     <Constraints>
        INDEX idx_t_j ON t (j)


Updating Statistics on Partitioning Tables
------------------------------------------

As the search range is limited by partitioning pruning when a query is executed, the query plan does not include the partitioning information. Therefore, no statistics information update is required.

.. note::

    In versions lower than CUBRID 9.0, statistics information of the partitioning table has been updated by using the **ANALYZE PARTITION** syntax. From the CUBRID 9.0 version, no action is actually made even when this syntax is executed, however, it is not processed as an error for compatibility with the previous versions.

Partitions and Inheritance
--------------------------

Partitions cannot be a part of the inheritance hierarchy chain and CUBRID has a different inheritance relationship for a partitioned table and a subclass. In fact, a partitioned table has superclasses and subclasses. However, in CUBRID, one partition has just one superclass (in other words, a partitioned table) only and does not have several subclasses.


