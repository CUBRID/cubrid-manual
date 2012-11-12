*****
MERGE
*****


**MERGE**

**Description**

The
**MERGE**
statement is used to select rows from one or more sources and to update or to insert the rows onto one table or view. You can specify the condition whether to update or to insert the rows onto the target table or view. The
**MERGE**
statement is a deterministic statement, so you cannot update the same rows on the target table several times within one sentence.

To execute the
**MERGE**
statement, the
**SELECT**
authorization for the source table should be granted. When the
**UPDATE**
clause is included in the target table, the
**UPDATE**
authorization should be granted. When the
**DELETE**
clause is included in the target table, the
**DELETE**
should be granted. When the
**INSERT**
clause is included in the target table, the
**INSERT**
should be granted.

**Syntax**

**MERGE INTO**
<
*target*
> [[
**AS**
] <
*alias*
>]

**USING**
<
*source*
> [[
**AS**
] <
*alias*
>], <
*source*
> [[
**AS**
] <
*alias*
>], ...

**ON**
<
*join_condition*
>

[ <
*merge_update_clause*
> ]

[ <
*merge_insert_clause*
> ]

 

<
*merge_update_clause*
>::=

**WHEN MATCHED THEN UPDATE**

**SET**
<
*col*
=
*expr*
> [,<
*col*
=
*expr*
>,…] [
**WHERE**
<
*update_condition*
>]

[
**DELETE WHERE**
<
*delete_condition*
>]

 

<
*merge_insert_clause*
>::=

**WHEN NOT MATCHED THEN INSERT**
[(<
*attributes_list*
>)]

    
**VALUES**
(<
*expr_list*
>) [
**WHERE**
<
*insert_condition*
>]

*   <
    *target*
    >: Target table to be updated or inserted. Several tables or views are available.



*   <
    *source*
    >: Source table to get the data. Several tables or views are available and sub-query is available, too.



*   <
    *join_condition*
    >: Specifies the updated conditions



*   <
    *merge_update_clause*
    >: If <
    *join_condition*
    > is TRUE, the new column value of a target table will be specified.

    *   When the
        **UPDATE**
        clause is executed, all
        **UPDATE**
        triggers defined in <
        *target*
        > are enabled.



    *   <
        *col*
        >: The column to be updated should be the column of the target table.



    *   <
        *expr*
        >: Allows the expression including columns of <
        *source*
        > and <
        *target*
        >. Or it can be the
        **DEFAULT**
        keyword. <
        *expr*
        > cannot include the aggregate functions.



    *   <
        *update_condition*
        >: Performs the
        **UPDATE**
        operation only when a specific condition is TRUE. For the condition, both of the source tables and the target tables can be referenced. The rows which do not satisfy the condition are not updated.



    *   <
        *delete_condition*
        >: Specifies a target to be deleted after update. At this time, the
        **DELETE**
        condition is executed with the updated result value. When
        **DELETE**
        statement is executed, the
        **DELETE**
        trigger defined in <
        *target*
        > will be enabled.



    *   The column referred by the
        **ON**
        condition clause cannot be updated.



    *   When you update view, you cannot specify
        **DEFAULT**
        .





*   <
    *merge_insert_clause*
    >: If the <
    *join_condition*
    > condition is FALSE, the column value of the target table will be inserted.

    *   When the
        **INSERT**
        clause is executed, all
        **INSERT**
        triggers defined in the target table are enabled.



    *   <
        *insert_condition*
        >: Performs the INSERT operation only when the specified condition is TRUE. <Columns of <
        *source*
        > can be included in the condition.



    *   <
        *attribute_list*
        >: Columns to be inserted in <
        *target*
        >.



    *   <
        *expr_list*
        >: Integer filter condition can be used to insert all source rows to the target table. ON (0=1) is an example of integer filter condition.



    *   This clause can be specified as it is or as <
        *merge_update_clause*
        >. If both of two are defined, the order does not matter.





**Example 1**

The following example shows how to merge the value of source_table to target_table.

-- source_table

CREATE TABLE source_table (a INT, b INT, c INT);

INSERT INTO source_table VALUES (1, 1, 1);

INSERT INTO source_table VALUES (1, 3, 2);

INSERT INTO source_table VALUES (2, 4, 5);

INSERT INTO source_table VALUES (3, 1, 3);

 

-- target_table

CREATE TABLE target_table (a INT, b INT, c INT);

INSERT INTO target_table VALUES (1, 1, 4);

INSERT INTO target_table VALUES (1, 2, 5);

INSERT INTO target_table VALUES (1, 3, 2);

INSERT INTO target_table VALUES (3, 1, 6);

INSERT INTO target_table VALUES (5, 5, 2);

 

MERGE INTO target_table tt USING source_table st

ON (st.a=tt.a AND st.b=tt.b)

WHEN MATCHED THEN UPDATE SET tt.c=st.c

     DELETE WHERE tt.c = 1

WHEN NOT MATCHED THEN INSERT VALUES (st.a, st.b, st.c);

 

-- the result of above query

SELECT * FROM target_table;

            a            b            c

=======================================

            1            2            5

            1            3            2

            3            1            3

            5            5            2

            2            4            5

In the above example, when column A and B of source_table are identical with the values of column A and B in target_table, column C of target_table is updated with the column C of source_table. Otherwise, the record value in source_table is inserted to target_table. However, if the value of column C in target_table is 1 in the updated record, delete the record.

**Example 2**

The following example shows how to use the
**MERGE**
statement to arrange the bonus score records given to students.CREATE TABLE bonus (std_id int, addscore int);

CREATE INDEX i_scores_std_id on scores (std_id);

 

INSERT INTO bonus VALUES (1,10);

INSERT INTO bonus VALUES (2,10);

INSERT INTO bonus VALUES (3,10);

INSERT INTO bonus VALUES (4,10);

INSERT INTO bonus VALUES (5,10);

INSERT INTO bonus VALUES (6,10);

INSERT INTO bonus VALUES (7,10);

INSERT INTO bonus VALUES (8,10);

INSERT INTO bonus VALUES (9,10);

INSERT INTO bonus VALUES (10,10);

 

CREATE TABLE std (std_id INT, score INT);

CREATE INDEX i_std_std_id  ON std (std_id);

CREATE INDEX i_std_std_id_score ON std (std_id, score);

 

INSERT INTO std VALUES (1,60);

INSERT INTO std VALUES (2,70);

INSERT INTO std VALUES (3,80);

INSERT INTO std VALUES (4,35);

INSERT INTO std VALUES (5,55);

INSERT INTO std VALUES (6,30);

INSERT INTO std VALUES (7,65);

INSERT INTO std VALUES (8,65);

INSERT INTO std VALUES (9,70);

INSERT INTO std VALUES (10,22);

INSERT INTO std VALUES (11,67);

INSERT INTO std VALUES (12,20);

INSERT INTO std VALUES (13,45);

INSERT INTO std VALUES (14,30);

 

MERGE INTO bonus t USING (SELECT * FROM std WHERE score < 40) s

ON t.std_id = s.std_id

WHEN MATCHED THEN

UPDATE SET t.addscore=t.addscore+s.score*0.1

WHEN NOT MATCHED THEN

INSERT (t.std_id, t.addscore) VALUES (s.std_id, 10+s.score*0.1) WHERE s.score<=30;

 

SELECT * FROM bonus ORDER BY 1;

std_id     addscore

==========================

            1           10

            2           10

            3           10

            4           14

            5           10

            6           13

            7           10

            8           10

            9           10

           10           12

           12           12

           14           13

In the above example, the source table is a set of std table records where the score is less than 40 and the target table is bonus. The student numbers (std_id) where the score (std.score) is less than 40 are 4, 6, 10, 12, and 14. Among them, for 4, 6, and 10 on the bonus table, the
**UPDATE**
clause adds 10% of the corresponding student score to the existing bonus. For 12 and 14 which are not on the bonus table, the INSERT clause additionally gives 10 scores and 10% of the corresponding student score.

**Note**
In CUBRID 9.0 Beta, "WITH CHECK OPTION" is not successfully processed and a "Check option exception" error occurs when
**MERGE**
is executed for the view.

CREATE TABLE t1(a int, b int);

INSERT INTO t1 values(1, 100);

INSERT INTO t1 values(2, 200);

CREATE TABLE t2(a int, b int);

INSERT INTO t2 values(1, 99);

INSERT INTO t2 values(2, 999);

CREATE VIEW v AS SELECT * FROM t1 WHERE b < 150 WITH CHECK OPTION;

--should succeed, but check option exception occurs

MERGE into v

USING t2

ON (t2.a=v.a)

WHEN MATCHED THEN

UPDATE

SET v.b=t2.b;

 

ERROR: Check option exception on view v.
