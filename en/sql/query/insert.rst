******
INSERT
******


**INSERT**

**Overview**

**Description**

You can insert a new record into a table in a database by using the
**INSERT**
statement. CUBRID supports
**INSERT...VALUES**
,
**INSERT...SET**
and
**INSERT...SELECT**
statements.

**INSERT ... VALUES**
and
**INSERT...SET**
statements are used to insert a new record based on the value that is explicitly specified while the
**INSERT ... SELECT**
statement is used to insert query result records obtained from different tables. Use the
**INSERT VALUES**
or
**INSERT ... SELECT**
statement to insert multiple rows by using the single
**INSERT**
statement. 

**Syntax**

<
*INSERT … VALUES statement*
>

**INSERT**
[
**INTO**
]
*table_name*
[(
*column_name*
, ...)]

    {
**VALUES**
|
**VALUE**
}({
*expr*
|
**DEFAULT**
}, ...)[,({
*expr*
|
**DEFAULT**
}, ...),...]

    [
**ON DUPLICATE KEY UPDATE**
*column_name*
 =
*expr*
, ... ]

**INSERT**
[
**INTO**
]
*table_name*
**DEFAULT**
[
**VALUES**
]

**INSERT**
[
**INTO**
]
*table_name*
**VALUES**
()

 

<
*INSERT … SET statement*
>

**INSERT**
[
**INTO**
]
*table_name*

    
**SET**
*column_name*
 = {
*expr*
|
**DEFAULT**
}[,
*column_name*
 = {
*expr*
|
**DEFAULT**
},...]

    [
**ON DUPLICATE KEY UPDATE**
*column_name*
 =
*expr*
, ... ]

 

<
*INSERT … SELECT statement*
>

**INSERT**
[
**INTO**
]
*table_name*
[(
*column_name*
, ...)]

    
**SELECT**
...

    [
**ON DUPLICATE KEY UPDATE**
*column_name*
 =
*expr*
, ... ]

*   *table_name*
    : Specifies the name of the target table into which you want to insert a new record.



*   *column_name*
    : Specifies the name of the column into which you want to insert the value. If you omit to specify the column name, it is considered that all columns defined in the table have been specified. Therefore, you must specify the values for all columns next to the
    **VALUES**
    keyword. If you do not specify all the columns defined in the table, a
    **DEFAULT**
    value is assigned to the non-specified columns; if the
    **DEFAULT**
    value is not defined, a
    **NULL**
    value is assigned.



*   *expr*
    |
    **DEFAULT**
    : Specifies values that correspond to the columns next to the
    **VALUES**
    keyword. Expressions or the
    **DEFAULT**
    keyword can be specified as a value. At this time, the order and number of the specified column list must correspond to the column value list. The column value list for a single record is described in parentheses.



*   **DEFAULT**
    : You can use the
    **DEFAULT**
    keyword to specify a default value as the column value. If you specify
    **DEFAULT**
    in the column value list next to the
    **VALUES**
    keyword, a default value column is stored for the given column: if you specify
    **DEFAUL**
    T before the
    **VALUES**
    keyword, default values are stored for all columns in the table.
    **NULL**
    is stored for the column whose default value has not been defined.



*   **ON DUPLICATE KEY UPDATE**
    : In case constraints are violated because a duplicated value for a column where
    **PRIMARY KEY**
    or
    **UNIQUE**
    attribute is defined is inserted, the value that makes constraints violated is changed into a specific value by performing the action specified in the
    **ON DUPLICATE KEY UPDATE**
    statement.



**Example**

CREATE TABLE a_tbl1(

id INT UNIQUE,

name VARCHAR,

phone VARCHAR DEFAULT '000-0000');

 

--insert default values with DEFAULT keyword before VALUES

INSERT INTO a_tbl1 DEFAULT VALUES;

 

--insert multiple rows

INSERT INTO a_tbl1 VALUES (1,'aaa', DEFAULT),(2,'bbb', DEFAULT);

 

--insert a single row specifying column values for all

INSERT INTO a_tbl1 VALUES (3,'ccc', '333-3333');

 

--insert two rows specifying column values for only

INSERT INTO a_tbl1(id) VALUES (4), (5);

 

--insert a single row with SET clauses

INSERT INTO a_tbl1 SET id=6, name='eee';

INSERT INTO a_tbl1 SET id=7, phone='777-7777';

 

SELECT * FROM a_tbl1;

           id  name                  phone

=========================================================

         NULL  NULL                  '000-0000'

            1  'aaa'                 '000-0000'

            2  'bbb'                 '000-0000'

            3  'ccc'                 '333-3333'

            4  NULL                  '000-0000'

            5  NULL                  '000-0000'

            6  'eee'                 '000-0000'

            7  NULL                  '777-7777' 

 

INSERT INTO a_tbl1 SET id=6, phone='000-0000'

ON DUPLICATE KEY UPDATE phone='666-6666';

 

SELECT * FROM a_tbl1 WHERE id=6;

           id  name                  phone

=========================================================

            6  'eee'                 '666-6666'

 

INSERT INTO a_tbl1 SELECT * FROM a_tbl1 WHERE id=7 ON DUPLICATE KEY UPDATE name='ggg';

 

SELECT * FROM a_tbl1 WHERE id=7;

           id  name                  phone

=========================================================

            7  'ggg'                 '777-7777'

**INSERT ... SELECT Statement**

**Description**

If you use the
**SELECT**
query in the
**INSERT**
statement, you can insert query results obtained from at least one table. The
**SELECT**
statement can be used in place of the
**VALUES**
keyword, or be included as a subquery in the column value list next to
**VALUES**
. If you specify the
**SELECT**
statement in place of the
**VALUES**
keyword, you can insert multiple query result records into the column of the table at once. However, there should be only one query result record if the
**SELECT**
statement is specified in the column value list.

In this way, you can extract data from another table that satisfies a certain retrieval condition, and insert it into the target table by combining the
**SELECT**
statement with the
**INSERT**
statement.

**Syntax**

**INSERT**
[
**INTO**
]
*table_name*
[(
*column_name*
, ...)]

    
**SELECT**
...

    [
**ON DUPLICATE KEY UPDATE**
*column_name*
 =
*expr*
, ... ]

**Example**

--creating an empty table which schema replicated from a_tbl1

CREATE TABLE a_tbl2 LIKE a_tbl1;

 

--inserting multiple rows from SELECT query results

INSERT INTO a_tbl2 SELECT * FROM a_tbl1 WHERE id IS NOT NULL;

 

--inserting column value with SELECT subquery specified in the value list

INSERT INTO a_tbl2 VALUES(8, SELECT name FROM a_tbl1 WHERE name <'bbb', DEFAULT);

 

SELECT * FROM a_tbl2;

           id  name                  phone

=========================================================

            1  'aaa'                 '000-0000'

            2  'bbb'                 '000-0000'

            3  'ccc'                 '333-3333'

            4  NULL                  '000-0000'

            5  NULL                  '000-0000'

            6  'eee'                 '000-0000'

            7  NULL                  '777-7777'

            8  'aaa'                 '000-0000'

**ON DUPLICATE KEY UPDATE Statement**

**Description**

In a situation in which a duplicate value is inserted into a column for which the
**UNIQUE**
index or the
**PRIMARY KEY**
constraint has been set, you can update to a new value without outputting the error by specifying the
**ON DUPLICATE KEY UPDATE**
clause in the
**INSERT**
statement.

However, the
**ON DUPLICATE KEY UPDATE**
clause cannot be used in a table in which a trigger for
**INSERT**
or
**UPDATE**
has been activated, or in a nested
**INSERT**
statement.

**Syntax**

<
*INSERT … VALUES statement*
>

<
*INSERT … SET statement*
>

<
*INSERT … SELECT statement*
>

**    INSERT**
...

    [
**ON DUPLICATE KEY UPDATE**
*column_name*
 =
*expr*
, ... ]

*   *column_name*
    =
    *expr*
    : Specifies the name of the column whose value you want to change next to
    **ON DUPLICATE KEY UPDATE**
    and a new column value by using the equal sign.



**Example**

--creating a new table having the same schema as a_tbl1

CREATE TABLE a_tbl3 LIKE a_tbl1;

INSERT INTO a_tbl3 SELECT * FROM a_tbl1 WHERE id IS NOT NULL and name IS NOT NULL;

SELECT * FROM a_tbl3;

           id  name                  phone

=========================================================

            1  'aaa'                 '000-0000'

            2  'bbb'                 '000-0000'

            3  'ccc'                 '333-3333'

            6  'eee'                 '000-0000'

 

--insert duplicated value violating UNIQUE constraint

INSERT INTO a_tbl3 VALUES(2, 'bbb', '222-2222');

 

ERROR: Operation would have caused one or more unique constraint violations.

 

--insert duplicated value with specifying ON DUPLICATED KEY UPDATE clause

INSERT INTO a_tbl3 VALUES(2, 'bbb', '222-2222')

ON DUPLICATE KEY UPDATE phone = '222-2222';

 

SELECT * FROM a_tbl3 WHERE id=2;

           id  name                  phone

=========================================================

            2  'bbb'                 '222-2222'

