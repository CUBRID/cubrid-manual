
---------------
Overview
---------------

.. _stored_proc:

Creating Stored Procedures/Functions
=====================================

PL/CSQL is used to create stored procedures and stored functions.
The behavior of the stored procedure or function to be created is described by
writing PL/CSQL code after the AS (or IS) keyword in the CREATE PROCEDURE or CREATE FUNCTION statement.

::

    <create_procedure> ::=
        CREATE [ OR REPLACE ] PROCEDURE [schema_name.]<identifier> [ ( <seq_of_parameters> ) ]
        { IS | AS } [ LANGUAGE PLCSQL ] [ <seq_of_declare_specs> ] <body> ;
    <create_function> ::=
        CREATE [ OR REPLACE ] FUNCTION [schema_name.]<identifier> [ ( <seq_of_parameters> ) ] RETURN <type_spec>
        { IS | AS } [ LANGUAGE PLCSQL ] [ <seq_of_declare_specs> ] <body> ;

In the above syntax, the *body* of a stored procedure/function contains PL/CSQL statements.
The declaration section, *seq_of_declare_specs*, declares variables, constants, exceptions, etc.,
that will be used within the execution statements.
For more details on these syntax elements, refer to :doc:`Declarations <plcsql_decl>` and
:doc:`Statements <plcsql_stmt>`.

Stored procedures/functions are always executed with auto-commit disabled.
This applies even if the auto-commit feature is enabled in the calling session.

Stored procedures/functions cannot have the same name as any
:ref:`CUBRID built-in functions <operators-and-functions>`.
Declaring a procedure or function with the same name as a built-in function will result in
a compilation error during the execution of the **CREATE** statement.

The following are examples of stored procedures/functions written using PL/CSQL.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE insert_athlete(
        p_name VARCHAR,
        p_gender VARCHAR,
        p_nation_code VARCHAR,
        p_event VARCHAR)
    AS
    BEGIN
        INSERT INTO athlete (name, gender, nation_code, event)
        VALUES (p_name, p_gender, p_nation_code, p_event);

        COMMIT;
    EXCEPTION
        WHEN OTHERS THEN
            ROLLBACK;
    END;

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE delete_athlete(c INTEGER)
    AS
        n_deleted INTEGER;
    BEGIN
        DELETE FROM athlete
        WHERE code = c;

        n_deleted := SQL%ROWCOUNT;   -- number of deleted rows
        DBMS_OUTPUT.put_line(n_deleted || ' rows deleted');
    EXCEPTION
        WHEN OTHERS THEN
            DBMS_OUTPUT.put_line('exception occurred');
    END;

.. code-block:: sql

    CREATE OR REPLACE FUNCTION fibonacci(n INTEGER) RETURN INTEGER
    IS
        invalid_input EXCEPTION;
    BEGIN
        IF n <= 0 THEN
            RAISE invalid_input;
        END IF;

        IF n = 1 OR n = 2 THEN
            RETURN 1;
        ELSE
            RETURN fibonacci(n-1) + fibonacci(n-2);
        END IF;
    EXCEPTION
        WHEN invalid_input THEN
            DBMS_OUTPUT.put_line('invalid input: ' || n);
            RETURN -1;
        WHEN OTHERS THEN
            DBMS_OUTPUT.put_line('unknown exception');
            RETURN -1;
    END;

In the above examples, the `DBMS_OUTPUT.put_line()` statement stores the given string argument in the server's
DBMS_OUTPUT buffer.
If the argument is not of a string type, it is converted to a string before being stored.
The string messages stored in the DBMS_OUTPUT buffer can be viewed in CSQL by executing
the session command `;server-output on`.
For more details, refer to :ref:`CSQL session command server-output <server-output>`.

When executing a `CREATE PROCEDURE/FUNCTION` statement, various rules related to the syntax and execution
semantics of the stored procedure/function are checked.
If any errors are found during this process, an error message is displayed indicating the location and
cause of the error.
The following is an example of a stored procedure containing errors and compiling it emits an error message.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE athlete_code(p_name VARCHAR) AS
    BEGIN
         -- Error: Static SQL SELECT statement must have an INTO clause
        SELECT code
        FROM athlete a
        WHERE a.name = p_name;
    END;

    ERROR: In line 4, column 5
    Stored procedure compile error: SELECT statement must have an INTO clause

    0 command(s) successfully processed.

.. _static_sql:

Static SQL
==================

Static SQL refers to SQL statements that are written directly within the code rather than being written in a string.
It is a type of SQL where the access method is predefined, allowing syntax and semantics checks to be performed at
compile time.

Although it has the drawback of being inflexible, it has the advantage of being optimized at compile time,
making it faster and more efficient than Dynamic SQL, where the access method is determined at runtime.

The following SQL statements can be used directly as PL/CSQL statements:

* SELECT (including CTE, UNION, INTERSECT, MINUS)
* INSERT, UPDATE, DELETE, MERGE, REPLACE
* COMMIT, ROLLBACK
* TRUNCATE

For detailed syntax and meanings, refer to :ref:`CUBRID SQL <cubrid_sql>`.
SQL statements not included in the above list cannot be used directly,
but they can be executed using the Dynamic SQL statements described below.

The SELECT statement can be used not only as an execution statement
but also when :ref:`declaring a cursor <cursor_decl>`
or in the :ref:`OPEN-FOR <cursor_manipulation>` statement.
The INTO clause of a SELECT statement can be used to store query results in program variables or OUT parameters.
In this case, the number of retrieved values must match the number of variables or OUT parameters in the INTO clause,
and the values must have types that can be assigned to the corresponding variables or OUT parameters.

When using a SELECT statement as an execution statement, the INTO clause must be included.
However, when using a SELECT statement in a :ref:`cursor declaration <cursor_decl>`
or an :ref:`OPEN-FOR <cursor_manipulation>` statement, the INTO clause may not be included.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_into_clause_1
    AS
        h int;
        s varchar(10);
        CURSOR c IS SELECT host_year, score INTO h, s FROM history;     -- Error: INTO clause
    BEGIN
        ...
    END;

    CREATE OR REPLACE PROCEDURE test_into_clause_2
    AS
        h int;
        s varchar(10);
        r SYS_REFCURSOR;
    BEGIN
        OPEN r FOR SELECT host_year, score INTO h, s FROM history;      -- Error: INTO clause
        ...
    END;

    CREATE OR REPLACE PROCEDURE test_into_clause_3
    AS
    BEGIN
        SELECT host_year, score FROM history WHERE event_code = 20023;  -- Error: no INTO clause
        ...
    END;

The query result of a SELECT statement without an INTO clause must be a single record.
If more than one record is returned, a `TOO_MANY_ROWS` exception occurs.
If no records are found, a `NO_DATA_FOUND` exception occurs.

In places where values are required, such as in the WHERE clause or VALUES clause of a Static SQL statement,
variables, constants, and procedure/function parameters declared in PL/CSQL can be used.
However, these must not have the `BOOLEAN` or `SYS_REFCURSOR` type,
as they are not included in :ref:`SQL Data Types <datatype_index>`.

The following is an example of using Static SQL.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION get_medal_count(p_name VARCHAR, p_medal CHAR) RETURN INTEGER
    AS
        n INTEGER;
    BEGIN
        -- SELECT statement as a regular execution statement
        SELECT COUNT(medal)
        INTO n
        FROM athlete a, record r
        WHERE a.code = r.athlete_code   /* Join condition */
        AND a.name = p_name AND r.medal = p_medal;    /* Filter condition */

        RETURN n;
    END;

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE athlete_history(p_name VARCHAR)
    AS
    BEGIN
        -- SELECT statement inside a FOR loop
        FOR r IN (SELECT host_year, score FROM history WHERE athlete = p_name) LOOP
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);
        END LOOP;
    END;

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE athlete_history(p_name VARCHAR)
    AS
        -- SELECT statement in cursor definition
        CURSOR my_cursor IS
        SELECT host_year, score
        FROM history
        WHERE athlete = p_name;
    BEGIN
        FOR r IN my_cursor LOOP
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);
        END LOOP;
    END;

If an error occurs during the execution of Static SQL, an `SQL_ERROR` exception is raised.

.. _dyn_sql:

Dynamic SQL
==================

Dynamic SQL refers to SQL statements that are stored in strings and executed
by constructing the corresponding SQL string at runtime and executing it using
the :ref:`EXECUTE IMMEDIATE <exec_imme>` statement.

Dynamic SQL is mainly required in the following two cases:

* When it is difficult or impossible to determine the SQL statement at the time of program development
* When executing statements that are not supported by Static SQL, such as DDL statements

In the example below, the new table name includes a procedure parameter,
which means it cannot be determined at the time of program development
and is only decided at runtime.
Additionally, the `DROP TABLE` and `CREATE TABLE` statements are DDL statements,
which are not supported by Static SQL.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE collect_athlete_history(p_name VARCHAR)
    AS
        new_table VARCHAR := p_name || '_history';
    BEGIN
        EXECUTE IMMEDIATE 'drop table if exists ' || new_table;
        EXECUTE IMMEDIATE 'create table ' || new_table || ' like history';
        EXECUTE IMMEDIATE 'insert into ' || new_table || ' select * from history where athlete = ?'
        USING p_name;
    END;

Writing Rules
==================

When writing identifiers, reserved words, comments, and literals, follow the rules of
:ref:`SQL writing rules <lexical_rules>` within :ref:`Static <static_sql>`/:ref:`Dynamic <dyn_sql>` SQL.

The rules for writing Non-static/dynamic SQL statements in PL/SQL mostly follow the same rules,
but there are some exceptions:

* Unlike SQL, the `#` symbol cannot be used in identifiers. That is, identifiers must consist only of English letters (uppercase and lowercase), Korean letters, digits, and the underscore (`_`).
* Even if enclosed in double quotes, square brackets, or backticks, special characters cannot be used in identifiers.
  Only English letters (uppercase and lowercase), Korean letters, digits, and underscores (`_`) are allowed.
* Bit string literals cannot be used.

.. rubric:: Examples of Allowed Identifiers

::

    a
    a_b
    athleteName2
    "select"        -- Reserved word enclosed in double quotes

.. rubric:: Examples of Disallowed Identifiers

::

    1a              -- Starts with a digit
    a@b             -- Contains special character
    athlete-name-2  -- Contains special character
    [a@b]           -- Special characters not allowed even within [ ]
    select          -- Reserved word

PL/CSQL reserved words are listed in the table below.
In Non-static/dynamic SQL statements, the words in the table below cannot be used as identifiers
representing variables, constants, exceptions, internal procedures/functions, etc.
However, as in SQL statements, they can be used as identifiers if enclosed in double quotes (" "),
square brackets ([ ]), or backticks (\` \`).
Inside Static/Dynamic SQL, the list below does not apply; instead, the
:ref:`CUBRID reserved word list <reserved_words>` applies, which is used for general SQL statements.

+-------------------+--------------------+--------------------+--------------------+
| ADDDATE           | AND                | AS                 | AUTHID             |
+-------------------+--------------------+--------------------+--------------------+
| AUTONOMOUS_TRANSACTION                 |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| BEGIN             | BETWEEN            | BIGINT             | BOOLEAN            |
+-------------------+--------------------+--------------------+--------------------+
| BOTH              | BY                 |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| CALLER            | CASE               | CAST               | CHAR               |
+-------------------+--------------------+--------------------+--------------------+
| CHARACTER         | CHR                | CLOSE              | COMMENT            |
+-------------------+--------------------+--------------------+--------------------+
| COMMIT            | CONSTANT           | CONTINUE           | CREATE             |
+-------------------+--------------------+--------------------+--------------------+
| CURRENT_USER      | CURSOR             |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| DATE              | DATETIME           | DATETIMELTZ        | DATETIMETZ         |
+-------------------+--------------------+--------------------+--------------------+
| DATE_ADD          | DATE_SUB           | DAY                | DAY_HOUR           |
+-------------------+--------------------+--------------------+--------------------+
| DAY_MILLISECOND   | DAY_MINUTE         | DAY_SECOND         | DBMS_OUTPUT        |
+-------------------+--------------------+--------------------+--------------------+
| DEC               | DECIMAL            | DECLARE            | DEFAULT            |
+-------------------+--------------------+--------------------+--------------------+
| DEFINER           | DELETE             | DETERMINISTIC      | DIV                |
+-------------------+--------------------+--------------------+--------------------+
| DOUBLE            |                    |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| ELSE              | ELSIF              | END                | ESCAPE             |
+-------------------+--------------------+--------------------+--------------------+
| EXCEPTION         | EXECUTE            | EXIT               | EXTRACT            |
+-------------------+--------------------+--------------------+--------------------+
| FALSE             | FETCH              | FLOAT              | FOR                |
+-------------------+--------------------+--------------------+--------------------+
| FROM              | FUNCTION           |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| HOUR              | HOUR_MILLISECOND   | HOUR_MINUTE        | HOUR_SECOND        |
+-------------------+--------------------+--------------------+--------------------+
| IF                | IMMEDIATE          | IN                 | INOUT              |
+-------------------+--------------------+--------------------+--------------------+
| INSERT            | INT                | INTEGER            | INTERNAL           |
+-------------------+--------------------+--------------------+--------------------+
| INTO              | IS                 | ISO88591           | LANGUAGE           |
+-------------------+--------------------+--------------------+--------------------+
| LEADING           | LIKE               | LIST               | LOOP               |
+-------------------+--------------------+--------------------+--------------------+
| MERGE             | MILLISECOND        | MINUTE             | MINUTE_MILLISECOND |
+-------------------+--------------------+--------------------+--------------------+
| MINUTE_SECOND     | MOD                | MONTH              | MULTISET           |
+-------------------+--------------------+--------------------+--------------------+
| NOT               | NULL               | NUMERIC            |                    |
+-------------------+--------------------+--------------------+--------------------+
| OF                | OPEN               | OR                 | OUT                |
+-------------------+--------------------+--------------------+--------------------+
| OWNER             |                    |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| PLCSQL            | POSITION           | PRAGMA             | PRECISION          |
+-------------------+--------------------+--------------------+--------------------+
| PROCEDURE         |                    |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| QUARTER           |                    |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| RAISE             | REAL               | REPLACE            | RETURN             |
+-------------------+--------------------+--------------------+--------------------+
| REVERSE           | ROLLBACK           | RAISE_APPLICATION_ERROR                 |
+-------------------+--------------------+--------------------+--------------------+
| SECOND            | SECOND_MILLISECOND | SEQUENCE           | SELECT             |
+-------------------+--------------------+--------------------+--------------------+
| SECOND_MILLISECOND| SEQUENCE           | SELECT             | SET                |
+-------------------+--------------------+--------------------+--------------------+
| SETEQ             | SETNEQ             | SHORT              | SMALLINT           |
+-------------------+--------------------+--------------------+--------------------+
| SQL               | SQLCODE            | SQLERRM            | STRING             |
+-------------------+--------------------+--------------------+--------------------+
| SUBDATE           | SUBSET             | SUBSETEQ           | SUPERSET           |
+-------------------+--------------------+--------------------+--------------------+
| SUPERSETEQ        | SYS_REFCURSOR      |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| THEN              | TIME               | TIMESTAMP          | TIMESTAMPLTZ       |
+-------------------+--------------------+--------------------+--------------------+
| TIMESTAMPTZ       | TRAILING           | TRIM               | TRUE               |
+-------------------+--------------------+--------------------+--------------------+
| TRUNCATE          |                    |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| UPDATE            | USING              | UTF8               |                    |
+-------------------+--------------------+--------------------+--------------------+

The CREATE PROCEDURE/FUNCTION statement for creating PL/CSQL stored procedures/functions goes through
a separate PL/CSQL syntax check, not the general SQL syntax check,
and a different set of reserved words is applied.
However, up to the AS/IS keywords, the general SQL syntax check also applies, so the
:ref:`CUBRID reserved word list <reserved_words>` is also in effect there.

In the following example, the parameter name `add` is not a PL/CSQL reserved word,
but since it is a CUBRID reserved word, it causes a syntax error.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_cubrid_reserved_word(add INT) AS
    BEGIN
       NULL;
    END;

    ERROR: invalid create procedure
      ... ...

Among the list above, `AUTONOMOUS_TRANSACTION` is a reserved word that is pre-included for functionality
to be added in the future.

.. _types:

Data Types
==================

In Static/Dynamic SQL, all :ref:`data types <datatype_index>` provided by SQL can be used.

On the other hand, in Non-static/dynamic SQL,
only `BOOLEAN`, `SYS_REFCURSOR`, and a subset of SQL-provided data types can be used.

* `BOOLEAN`: Can have values TRUE, FALSE, or NULL.
  Since the `BOOLEAN` type is not supported in SQL, it cannot be used as a parameter or return type
  in the `CREATE PROCEDURE/FUNCTION` statement.
  However, it *can* be used as a parameter or return type when declaring
  :ref:`local procedures/functions <local_routine_decl>`.
* `SYS_REFCURSOR`: Used when declaring cursor variables.
  For usage of cursor variables, see the :ref:`OPEN-FOR <cursor_manipulation>` statement.
  Similar to `BOOLEAN`, `SYS_REFCURSOR` cannot be used as a parameter or return type
  in the `CREATE PROCEDURE/FUNCTION` statement, but it *can* be used in
  :ref:`local procedures/functions <local_routine_decl>`.

Among the data types provided by SQL, some are supported and others are not in PL/CSQL.
(As noted above, all SQL-provided data types *can* be used in Static/Dynamic SQL.)

+----------------+-------------------------------------+----------------------------------+
| Category       | Supported Types                     | Unsupported Types                |
+================+=====================================+==================================+
| Numeric        | SHORT, SMALLINT,                    |                                  |
+                +-------------------------------------+                                  +
|                | INTEGER, INT,                       |                                  |
+                +-------------------------------------+                                  +
|                | BIGINT,                             |                                  |
+                +-------------------------------------+                                  +
|                | NUMERIC, DECIMAL,                   |                                  |
+                +-------------------------------------+                                  +
|                | FLOAT, REAL,                        |                                  |
+                +-------------------------------------+                                  +
|                | DOUBLE, DOUBLE PRECISION,           |                                  |
+----------------+-------------------------------------+----------------------------------+
| Date/Time      | DATE, TIME, TIMESTAMP, DATETIME,    | | TIMESTAMPLTZ, TIMESTAMPTZ,     |
|                |                                     | | DATETIMELTZ, DATETIMETZ        |
+----------------+-------------------------------------+----------------------------------+
| String         | CHAR, VARCHAR, STRING, CHAR VARYING |                                  |
+----------------+-------------------------------------+----------------------------------+
| Collection     |                                     | SET, MULTISET, LIST, SEQUENCE    |
+----------------+-------------------------------------+----------------------------------+
| Others         |                                     | BIT, BIT VARYING,                |
+                +                                     +----------------------------------+
|                |                                     | ENUM,                            |
+                +                                     +----------------------------------+
|                |                                     | BLOB/CLOB,                       |
+                +                                     +----------------------------------+
|                |                                     | JSON                             |
+----------------+-------------------------------------+----------------------------------+

.. _percent_type:

%TYPE
======================

Appending `%TYPE` after a table column name allows you to reference the data type of that column.
Below is an example using `%TYPE`.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION get_athlete_name(p_code athlete.code%TYPE) RETURN athlete.name%TYPE
    AS
        name athlete.name%TYPE;
    BEGIN
        SELECT a.name
        INTO name
        FROM athlete a
        WHERE a.code = p_code;

        RETURN name;
    END;

<table>.<column>%TYPE represents the data type of the specified table column at the time
the `CREATE PROCEDURE/FUNCTION` statement is executed.
However, if the column’s data type is later changed, it will *not* be automatically reflected
in the behavior of stored procedures/functions that use `<table>.<column>%TYPE`.
Therefore, when the data type of a column using `%TYPE` is changed,
you must recompile all stored procedures/functions that use that `%TYPE` by executing
`ALTER PROCEDURE/FUNCTION <name> COMPILE`.

In addition to table columns, `%TYPE` can also be appended to procedure/function parameter or variable names
to represent their data types.

.. code-block:: sql

   ...
   a VARCHAR(10);
   a_like a%TYPE;   -- Declares variable a_like with the same type as variable a
   ...

.. _percent_rowtype:

%ROWTYPE
======================

Appending `%ROWTYPE` after a table name represents a record type composed of fields that have the same
names and types as the columns of that table.
For example, for a table `tbl` declared as follows:

.. code-block:: sql

   CREATE TABLE tbl(a INT, b CHAR, c VARCHAR);

Declaring a variable `r` with the type `tbl%ROWTYPE`:

.. code-block:: sql

   r tbl%ROWTYPE;

The value of `r` becomes a record with fields `a`, `b`, and `c`, and `r.a`, `r.b`, and `r.c` will be of types `INT`, `CHAR`, and `VARCHAR`, respectively.

You can also append `%ROWTYPE` to a cursor name.
In this case, it represents a record type corresponding to the result of the `SELECT` statement defined in the cursor.

.. code-block:: sql

   CURSOR c IS SELECT a, b FROM tbl;
   p c%ROWTYPE;     -- p.a and p.b are of types INT and CHAR, respectively

If no initial value is provided in the declaration of a record variable, it is initialized as an "empty record" in which all fields are `NULL`.

.. code-block:: sql

   r tbl%ROWTYPE;   -- r.a, r.b, r.c are all NULL, but r itself is not NULL

Assigning `NULL` to a record variable initializes each field to `NULL`, but the record variable itself does not become `NULL`.
In other words, a record variable never holds a `NULL` value after being declared.

Records of the same type can be compared using `=` and `!=` operators.
Here, "same type" means not only records derived from the same table, but also records from different tables whose corresponding field names and types match exactly.
The result of a `=` operation between two records is `TRUE` only if the `<=>` operation between all corresponding fields returns `TRUE`; otherwise, it returns `FALSE`.
The result of the `!=` operation is the logical opposite of the `=` operation.
Using `=` or `!=` between records of different types will result in a compile-time error.

.. code-block:: sql

    create table tblA(a INT, b CHAR, c VARCHAR);
    create table tblB(a INT, b CHAR, c VARCHAR);        -- tblA%ROWTYPE and tblB%ROWTYPE are of the same type
    create table tblC(aa INT, bb CHAR, cc VARCHAR);     -- tblA%ROWTYPE and tblC%ROWTYPE are not of the same type

    CREATE OR REPLACE PROCEDURE test_record_equality AS
        r1 tblA%ROWTYPE;
        r2 tblB%ROWTYPE;
        r3 tblC%ROWTYPE;
    BEGIN
        ...
        if (r1 = r2) then       -- OK
        ...
        if (r1 = r3) then       -- Error
        ...
    END;

Comparison operators other than `=` and `!=`, such as `<=>`, `<`, `>`, `<=`, and `>=`,
cannot be used for record comparisons.

Assignment from one record variable `s` to another record variable `t` is allowed when the following conditions are met:

* The number of fields in `s` and `t` are the same.
* For each field position `i`, let the types of the `i`-th fields in `s` and `t` be `S`\ :sub:`i` and `T`\ :sub:`i`,
  respectively. It must be possible to assign a value from `S`\ :sub:`i` to `T`\ :sub:`i`.

The names of corresponding fields do *not* need to match in order for assignment between record variables to be valid.

.. code-block:: sql

    CREATE TABLE tblAA(a NUMERIC, b DATETIME);
    CREATE TABLE tblBB(m INT, n VARCHAR);
    CREATE TABLE tblCC(x INT, y TIME);

    CREATE OR REPLACE PROCEDURE test_record_assign AS
        r1 tblAA%ROWTYPE;
        r2 tblBB%ROWTYPE;
        r3 tblCC%ROWTYPE;
    BEGIN
        ...
        r1 := r2;   -- OK
        r1 := r3;   -- Error: Cannot assign TIME to DATETIME (incompatible types)
    END;

`%ROWTYPE` can be used as parameter types and return types for local procedures/functions.
However, since record types are not supported in SQL statements, `%ROWTYPE` cannot be used as parameter or return types in stored procedures/functions.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE sp(a tbl%ROWTYPE) AS    -- Error

        PROCEDURE inner(b tbl%ROWTYPE) AS               -- OK
        BEGIN
            ...
        END;
    BEGIN
        ...
    END;

Record variables can be used in the `INTO` clause of Static/Dynamic SQL `SELECT` statements and `FETCH` statements.
When using a record variable in an `INTO` clause, other variables cannot be used together.
Also, the column names of the query result do not need to match the field names of the record variable,
but the number of columns retrieved and the number of fields in the record variable must be the same.
Additionally, the column types and record field types must be compatible.

.. code-block:: sql

   CURSOR c IS SELECT a, b FROM tbl;
   whole tbl%ROWTYPE;
   part c%ROWTYPE;

   -- Static SQL
   SELECT * INTO whole FROM tbl;

   -- Dynamic SQL
   EXECUTE IMMEDIATE 'SELECT * FROM tbl' INTO whole;
   EXECUTE IMMEDIATE 'SELECT a, b FROM tbl' INTO part;

   -- Fetch
   FETCH c INTO part;

In the `VALUES` clause of a Static SQL `INSERT`/`REPLACE` statement, a record variable can be used.
When a record variable is used, it cannot be combined with other variables in the same `VALUES` clause.
Also, the names of the target columns and the record fields do not need to match,
but the number of columns and fields must be the same,
and the column types must be compatible with the record field types.

.. code-block:: sql

   INSERT INTO tbl VALUES whole;
   INSERT INTO tbl(a, b) VALUES part;

The following forms are also allowed:

.. code-block:: sql

   INSERT INTO tbl SET ROW = whole;
   INSERT INTO tbl(a, b) SET ROW = part;

In a Static SQL `UPDATE` statement, a record variable can also be used with the
`SET ROW = <record>` syntax as shown below.
This is allowed only for single-table updates, and the field types of the record must be
compatible with the corresponding column types in the same order.

.. code-block:: sql

   UPDATE tbl SET ROW = whole WHERE a % 2 = 0;

Precision and Scale Specification Exceptions
=============================================

Among the :ref:`data types supported in PL/CSQL <datatype_index>`,
`NUMERIC` allows specification of precision and scale,
while `CHAR` and `VARCHAR` allow specification of length.
However, specifying precision and scale is *not allowed* for parameter types and return types of
stored procedures/functions.
This restriction also applies to local procedures/functions.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION sf(a NUMERIC(5, 3)) RETURN VARCHAR(10) AS ...    -- Error
    CREATE OR REPLACE FUNCTION sf(a NUMERIC) RETURN VARCHAR AS ...              -- OK

Generally, `NUMERIC` without precision and scale amounts to `NUMERIC(15, 0)`.
However, in the *parameter type* position, it exceptionally means that any precision and scale are allowed
with precision from 1 to 38 and scale from 0 to the precision,
while in the *return type* position, it is treated as `NUMERIC(p, s)`,
where `p` and `s` are determined by the system configuration parameter
:ref:`STORED_PROCEDURE_RETURN_NUMERIC_SIZE <system_config>`.
The default values for `p` and `s` are 38 and 15, respectively.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_any_precision_scale(a NUMERIC) RETURN NUMERIC
    AS
    BEGIN
        RETURN a;
    END;

    SELECT test_any_precision_scale(1.23);      -- Result: 1.230000000000000
    SELECT test_any_precision_scale(1.234);     -- Result: 1.234000000000000
    SELECT test_any_precision_scale(1.2345);    -- Result: 1.234500000000000

In addition, when `CHAR` and `VARCHAR` are used as parameter or return types,
they do not behave as `CHAR(1)` or `VARCHAR(1073741823)` as in other contexts.
Instead, they indicate that any string length is allowed
with length ≤ 2048 for `CHAR` and length ≤ 1073741823 for `VARCHAR`.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_any_length(a CHAR) RETURN CHAR
    AS
    BEGIN
        RETURN a;
    END;

    SELECT test_any_length('ab');       -- Result: 'ab'
    SELECT test_any_length('abc');      -- Result: 'abc'
    SELECT test_any_length('abcd');     -- Result: 'abcd'

Even when parameter and return types are declared using :ref:`%TYPE <percent_type>`,
the precision, scale, and length of the referenced original types are ignored,
and the behavior follows the rules described above.

.. code-block:: sql

    CREATE TABLE tbl(p NUMERIC(3,2), q CHAR(3));

    CREATE OR REPLACE FUNCTION test_ptype_precision_scale(a tbl.p%TYPE) RETURN tbl.p%TYPE
    AS
    BEGIN
        RETURN a;
    END;

    SELECT test_ptype_precision_scale(1.23);      -- Result: 1.230000000000000
    SELECT test_ptype_precision_scale(1.234);     -- Result: 1.234000000000000
    SELECT test_ptype_precision_scale(1.2345);    -- Result: 1.234500000000000

    CREATE OR REPLACE FUNCTION test_ptype_length(a tbl.q%TYPE) RETURN tbl.q%TYPE
    AS
    BEGIN
        RETURN a;
    END;

    SELECT test_ptype_length('ab');       -- Result: 'ab'
    SELECT test_ptype_length('abc');      -- Result: 'abc'
    SELECT test_ptype_length('abcd');     -- Result: 'abcd'

Operators and Functions
========================

In Static/Dynamic SQL, all operators and functions provided by SQL can be used.
On the other hand, in Non-static/dynamic SQL statements,
most SQL-provided operators and functions can also be used in the same way,
but there are a few exceptions:

* Operators and functions that take or return unsupported types (`BIT`, `ENUM`, `BLOB/CLOB`, `JSON`, etc.)
  cannot be used.
* The modulo operator `%` is not allowed. However, the `MOD` function with the same meaning can be used instead.
* Logical operators `&&`, `||`, and `!` are not allowed. Instead, use their equivalents: `AND`, `OR`, and `NOT`,
  respectively.

The following example shows that string functions `locate` and `substr`,
as well as the string concatenation operator `||`, can be used in PL/CSQL executable statements outside of
Static/Dynamic SQL.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE family_name_to_last
    AS
        delim INTEGER;
        family_name VARCHAR;
        given_name VARCHAR;
    BEGIN
        FOR r IN (SELECT a.name FROM athlete a LIMIT 5,5) LOOP
            delim := locate(' ', r.name);                   -- locate function
            family_name := substr(r.name, 1, delim - 1);    -- substr function
            given_name := substr(r.name, delim + 1);        -- substr function
            DBMS_OUTPUT.put_line(given_name || ' ' || family_name);     -- string concatenation operator ||
        END LOOP;
    END;

.. _exception:

Exception
======================

PL/CSQL, like many other programming languages, supports error handling through exception handlers
(see: :ref:`Block Statements <block_stmt>`).
Users can define their own exceptions in the declarative section and use them in the execution section
(see: :ref:`Exception Declarations <exception_decl>`).
Additionally, for major error situations, the following system-defined exceptions are available:

+---------------------+---------+------------------------------------------------------------------+
| Name                | SQLCODE | Description                                                      |
+=====================+=========+==================================================================+
| CASE_NOT_FOUND      | 0       | No matching WHEN clause and no ELSE clause in a CASE statement   |
+---------------------+---------+------------------------------------------------------------------+
| CURSOR_ALREADY_OPEN | 1       | Attempt to open a cursor that is already open                    |
+---------------------+---------+------------------------------------------------------------------+
| INVALID_CURSOR      | 2       | Invalid cursor operation (e.g., trying to close an unopened one) |
+---------------------+---------+------------------------------------------------------------------+
| NO_DATA_FOUND       | 3       | `SELECT INTO` returned zero rows                                 |
+---------------------+---------+------------------------------------------------------------------+
| PROGRAM_ERROR       | 4       | Internal system error                                            |
+---------------------+---------+------------------------------------------------------------------+
| STORAGE_ERROR       | 5       | Memory allocation failed due to insufficient memory              |
+---------------------+---------+------------------------------------------------------------------+
| SQL_ERROR           | 6       | Static/Dynamic SQL execution failed                              |
+---------------------+---------+------------------------------------------------------------------+
| TOO_MANY_ROWS       | 7       | `SELECT INTO` returned more than one row                         |
+---------------------+---------+------------------------------------------------------------------+
| VALUE_ERROR         | 8       | Error caused by an invalid value                                 |
+---------------------+---------+------------------------------------------------------------------+
| ZERO_DIVIDE         | 9       | Attempted division by zero                                       |
+---------------------+---------+------------------------------------------------------------------+

The `SQLCODE` of each exception above can be used inside an
:ref:`OTHERS exception handler block <block_stmt>` to identify the type of exception.

* `SQLCODE` values up to 999 are reserved for system exceptions.
* :ref:`User-declared exceptions <exception_decl>` have a `SQLCODE` of 1000.
* The first argument to :ref:`RAISE_APPLICATION_ERROR <raise_application_error>` must be greater than 1000.

The following is a simple example that handles the system exceptions `NO_DATA_FOUND` and `TOO_MANY_ROWS`
that can occur when executing a Static SQL `SELECT` statement.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION athlete_code(p_name VARCHAR) RETURN integer
    AS
        c INTEGER;
    BEGIN
        -- The SELECT INTO statement must return exactly one row
        SELECT code
        INTO c
        FROM athlete a
        WHERE a.name = p_name;

        RETURN c;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.put_line('error: no rows found for athlete name ' || p_name);
            RETURN -1;
        WHEN TOO_MANY_ROWS THEN
            DBMS_OUTPUT.put_line('error: more than one rows found for athlete name ' || p_name);
            RETURN -1;
    END;

If an exception is not explicitly handled using a `WHEN ... THEN ...` clause,
the location in the code where the exception occurred and the corresponding error message will be displayed.

For example, if the exception handler clauses are removed from the above `athlete_code()` function:

.. code-block:: sql

    CREATE OR REPLACE FUNCTION athlete_code(p_name VARCHAR) RETURN integer
    AS
        c INTEGER;
    BEGIN
        -- The SELECT INTO statement must return exactly one row
        SELECT code
        INTO c
        FROM athlete a
        WHERE a.name = p_name;

        RETURN c;
    END;

Then, if a name that does not exist in the `athlete` table is passed as an argument in CSQL,
a `NO_DATA_FOUND` exception will be raised as the result.

.. code-block::

   select athlete_code('x');

   In line 1, column 22,

   ERROR: Stored procedure execute error:
     (line 6, column 5) no data found


   0 command(s) successfully processed.

In the example above, position `(1, 22)` indicates the location within the `SELECT` statement,
and `(6, 5)` indicates the location within the `CREATE` statement that declares `athlete_code()`.

System Configuration Parameters Application
============================================

The behavior of Static/Dynamic SQL statements is uniformly affected by all
:ref:`system configuration parameters <system_config>`.

In contrast, in PL/CSQL statements outside of Static/Dynamic SQL,
only the following four system configuration parameters are effective:

* `compat_numeric_division_scale`
* `oracle_compat_number_behavior`
* `oracle_style_empty_string`
* `timezone`

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_system_config
    AS
    BEGIN
        -- When compat_numeric_division_scale is 'no': 0.125000000, when 'yes': 0.1
        dbms_output.put_line(1.0 / 8.0);

        -- When oracle_compat_number_behavior is 'no': 1, when 'yes': 2
        dbms_output.put_line(3 / 2);

        -- When oracle_style_empty_string is 'no': 'false', when 'yes': 'true'
        IF '' IS NULL THEN
            dbms_output.put_line('true');
        ELSE
            dbms_output.put_line('false');
        END IF;
    END;

For details about these parameters, see
:ref:`system configuration parameters <system_config>`.

Other than the four listed above, system settings do not apply to PL/CSQL statements outside Static/Dynamic SQL.
In particular:

* Regardless of the `no_backslash_escapes` setting, the backslash character is *not* treated as an escape character.
* Regardless of the `pipes_as_concat` setting, `||` is *not* used as a logical OR operator.
* Regardless of the `plus_as_concat` setting, `+` is treated as a string concatenation operator when applied to strings.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_system_config_2
    AS
    BEGIN
        -- Regardless of no_backslash_escapes, prints: 'Hello\nworld'
        dbms_output.put_line('Hello\\nworld');

        -- Regardless of pipes_as_concat, prints: 'ab'
        dbms_output.put_line('a' || 'b');

        -- Regardless of plus_as_concat, prints: '12'
        dbms_output.put_line('1' + '2');
    END;
