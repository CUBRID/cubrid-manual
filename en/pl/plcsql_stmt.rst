------------------
Statements
------------------

Currently, PL/CSQL provides the following 15 types of statements:

::

    <statement> ::=
          <block>
        | <sql_statement>
        | <cursor_manipulation>
        | <raise_application_error>
        | <execute_immediate>
        | <assignment_statement>
        | <continue_statement>
        | <exit_statement>
        | <null_statement>
        | <raise_statement>
        | <return_statement>
        | <procedure_call>
        | <if_statement>
        | <loop_statement>
        | <case_statement>

.. _block_stmt:

BLOCK
=====

The BLOCK statement creates a nested scope within the list of executable statements,
allowing new variables, constants, etc. to be declared and used inside it.
Like procedures/functions, a BLOCK can have an exception handling structure.

::

    <block> ::=
        [ DECLARE <seq_of_declare_specs> ] <body>

    <body> ::= BEGIN <seq_of_statements> [ EXCEPTION <seq_of_handlers> ] END [ <label_name> ]
    <seq_of_declare_specs> ::= <declare_spec> { <declare_spec> }...
    <seq_of_statements> ::= <statement> ; { <statement> ; }...
    <seq_of_handlers> ::= <handler> { <handler> }...
    <handler> ::= WHEN <exception_name> [ OR <exeption_name> OR ... ] THEN <seq_of_statements>
    <exception_name> ::= identifier | OTHERS

* *body*: must consist of one or more statements, optionally followed by exception handlers.
* *declare_spec*: variable, constant, exception, cursor, or inner procedure/function declarations (see: :doc:`Declarations <plcsql_decl>`)
* *handler*: statements to be executed when the specified exception occurs.
* *exception_name*: the *identifier* must be either a :ref:`system exception <exception>` or a :ref:`user-declared exception <exception_decl>`.
  `OTHERS` matches all exceptions not otherwise matched, and cannot be combined with other exception names using `OR`.

Items declared inside a BLOCK cannot be accessed from outside the BLOCK.
If an item declared in a BLOCK has the same name as one in an outer scope,
the outer item is hidden within that BLOCK.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_block
    IS
        a INT := 3;
        b INT := 3;
        c INT := 3;
    BEGIN
        DECLARE
            a INT := 5;
            b INT := 5;
        BEGIN
            DECLARE
                a INT := 7;
            BEGIN
                DBMS_OUTPUT.put_line(a || b || c);  -- '753'
            END;

            DBMS_OUTPUT.put_line(a || b || c);      -- '553'
        END;

        DBMS_OUTPUT.put_line(a || b || c);          -- '333'
    END;

If any statements inside the body are unreachable during execution,
a compile-time error will occur.
The following is a simple example containing an unreachable statement:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_unreachable_statement
    AS
    BEGIN
        RETURN;
        DBMS_OUTPUT.put_line('Hello world');
    END;

    ERROR: In line 5, column 5
    Stored procedure compile error: unreachable statement

Static SQL
==========

As explained in the :ref:`Static SQL <static_sql>` section,
the following SQL statements (SELECT, INSERT, UPDATE, DELETE, MERGE, REPLACE, COMMIT, ROLLBACK, and TRUNCATE)
can be used directly as executable statements within a program.

.. _cursor_manipulation:

Cursor Manipulation
===================

Cursor manipulation statements come in the following four forms:

::

    <cursor_manipulation> ::=
          <open_statement>
        | <fetch_statement>
        | <close_statement>
        | <open_for_statement>

    <open_statement> ::= OPEN <cursor> [ <function_argument> ]
    <fetch_statement> ::= FETCH <cursor_expression> INTO <identifier> [ , <identifier>, ... ]
    <close_statement> ::= CLOSE <cursor_expression>

    <open_for_statement> ::= OPEN <identifier> FOR <select_statement>

* *cursor_expression*: an expression that evaluates to a cursor or SYS_REFCURSOR variable
* *open_statement*: opens a cursor. Note that this can only be used with explicitly declared cursors, not SYS_REFCURSOR variables.
  For cursors declared with parameters, the actual values provided must match the number and types of parameters.
  If you attempt to open an already open cursor, a `CURSOR_ALREADY_OPEN` exception will be raised.
* *fetch_statement*: fetches one row from the cursor into the specified variables or OUT parameters.
  The number of columns in the fetched row must match the number of target variables, and their types must be compatible.
  Attempting to fetch from a cursor that is not open will raise an `INVALID_CURSOR` exception.
* *close_statement*: closes a cursor. Attempting to close a cursor that is not open will raise an `INVALID_CURSOR` exception.
* *open_for_statement*: the *identifier* must be a variable declared with type SYS_REFCURSOR.
  Internally, this statement opens a cursor for the given *select_statement* and assigns it to the variable.
  If the *select_statement* includes an INTO clause, a compile-time error will occur.

The following example shows how to use the OPEN, FETCH, and CLOSE statements:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_cursor(p_name VARCHAR, p_year INTEGER)
    AS
        CURSOR my_cursor(a VARCHAR, y INTEGER) IS
        SELECT host_year, score
        FROM history
        WHERE athlete = a AND host_year >= y;

        target_year INT;
        target_score VARCHAR(10);
    BEGIN

        OPEN my_cursor(p_name, p_year);
        LOOP
            FETCH my_cursor INTO target_year, target_score;
            EXIT WHEN my_cursor%NOTFOUND;
            DBMS_OUTPUT.put_line('host_year: ' || target_year || ' score: ' || target_score);
        END LOOP;
        CLOSE my_cursor;
    END;

The following example demonstrates how to use a local procedure with a SYS_REFCURSOR OUT parameter
along with the OPEN-FOR statement to bind a specific SELECT statement to a SYS_REFCURSOR variable and fetch its result:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_ref_cursor(p_name VARCHAR)
    AS
        my_refcursor SYS_REFCURSOR;

        target_year INT;
        target_score VARCHAR(10);

        PROCEDURE open_refcursor(athlete_name VARCHAR, rc OUT SYS_REFCURSOR)
        AS
            refcursor SYS_REFCURSOR;
        BEGIN
            OPEN refcursor FOR SELECT host_year, score FROM history WHERE athlete = athlete_name;
            rc := refcursor;
        END;
    BEGIN
        open_refcursor(p_name, my_refcursor);
        LOOP
            FETCH my_refcursor INTO target_year, target_score;
            EXIT WHEN my_refcursor%NOTFOUND;
            DBMS_OUTPUT.put_line('host_year: ' || target_year || ' score: ' || target_score);
        END LOOP;
        CLOSE my_refcursor;
    END;

.. _raise_application_error:

RAISE_APPLICATION_ERROR
=========================

`RAISE_APPLICATION_ERROR` is used to raise an :ref:`Exception <exception>` with a user-defined
:ref:`code and error message <sqlcode>`.
Although it appears to be a built-in procedure call, it is internally handled as a PL/CSQL executable statement.
The first argument must be an `INTEGER` value greater than 1000, as values ≤1000 are reserved for the system.
The second argument can be any string message.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_raise_app_err(i INT)
    AS
    BEGIN
        CASE i
        WHEN 1 THEN
            RAISE_APPLICATION_ERROR(1001, 'my error 1');
        WHEN 2 THEN
            RAISE_APPLICATION_ERROR(1002, 'my error 2');
        WHEN 3 THEN
            RAISE_APPLICATION_ERROR(1003, 'my error 3');
        END CASE;
    EXCEPTION
        WHEN OTHERS THEN
            dbms_output.put_line('code=' || SQLCODE || ', message=''' || SQLERRM || '''');
    END;

    CALL test_raise_app_err(1);     -- DBMS_OUTPUT output: code=1001, message='my error 1'
    CALL test_raise_app_err(2);     -- DBMS_OUTPUT output: code=1002, message='my error 2'
    CALL test_raise_app_err(3);     -- DBMS_OUTPUT output: code=1003, message='my error 3'

.. _exec_imme:

EXECUTE IMMEDIATE
=================

As described in the :ref:`Dynamic SQL <dyn_sql>` section,
you can dynamically build SQL statements at runtime and execute them using the `EXECUTE IMMEDIATE` statement.
The `USING` clause allows you to bind values from the program to host variables in the SQL.
The `INTO` clause allows the program to retrieve result values from a `SELECT` query into variables or OUT parameters.
The number of result columns must match the number of variables or OUT parameters,
and the types must be compatible.

If an error occurs while executing the SQL, a `SQL_ERROR` exception will be raised.
When using the `INTO` clause, the `SELECT` must return exactly one row.
If there are no results, `NO_DATA_FOUND` is raised;
if more than one row is returned, `TOO_MANY_ROWS` is raised.

::

    <execute_immediate> ::=
        EXECUTE IMMEDIATE <dynamic_sql> { [ <into_clause> ] [ <using_clause> ] | <using_clause> <into_clause> }
        <using_clause> ::= USING <using_element> [ , <using_element>, ... ]
        <using_element> ::= [ IN ] <expression>
        <into_clause> ::= INTO <identifier> [ , <identifier>, ... ]

* *dynamic_sql*: an expression of string type. It must evaluate to a syntactically valid SQL string.
  You may use `?` placeholders for values.
  The number of `?` placeholders must match the number of expressions in the *using_clause*.
* *using_clause*: specifies the values to replace `?` placeholders in the SQL string.
  Expressions of type `BOOLEAN` or `SYS_REFCURSOR` are not allowed.
  Record-type values declared with :ref:`%ROWTYPE <percent_rowtype>` or cursors are also not allowed.
* *into_clause*: specifies the variables or OUT parameters to receive result values from a `SELECT` statement.
  If *dynamic_sql* is a `SELECT` but no `INTO` clause is present, or vice versa, then a `SQL_ERROR` exception is raised.

Here is an example usage of `EXECUTE IMMEDIATE`:

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

Assignment
==========

::

    <assignment_statement> ::=
        <identifier> := <expression>

* *identifier*: must be a variable or OUT parameter
* *expression*: an expression that evaluates to the value to be assigned (see the expressions section)

The type of *expression* must be the same as or implicitly convertible to the type of *identifier*.
Otherwise, a compile-time error occurs.

CONTINUE, EXIT
===============

::

    <continue_statement> ::=
        CONTINUE [ <label_name> ] [ WHEN <expression> ]

::

    <exit_statement> ::=
        EXIT [ <label_name> ] [ WHEN <expression> ]

The `CONTINUE` and `EXIT` statements can only be used within loop statements.
The `CONTINUE` statement stops the current execution flow and jumps to the beginning of the loop to start the next iteration.
The `EXIT` statement terminates the current loop and jumps to the statement immediately following the loop.
If *label_name* is omitted, the statement applies to the innermost enclosing loop.
If *label_name* is specified, it must reference a label declared in one of the enclosing loops;
otherwise, a compile-time error occurs.
When multiple loops are nested, *label_name* can be used to specify the target loop.
The `WHEN` clause allows the branch to occur only when the BOOLEAN *expression* evaluates to TRUE.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_continue_exit
    AS
        i INT := 0;
    BEGIN
        LOOP
            DBMS_OUTPUT.put_line(i);            -- 0, 1, 2, 3, 4, 5
            i := i + 1;
            CONTINUE WHEN i < 3;
            DBMS_OUTPUT.put_line(i);            -- 3, 4, 5
            EXIT WHEN i = 5;
        END LOOP;

        DBMS_OUTPUT.put_line(i);                -- 5
    END;

NULL
====

::

    <null_statement> ::=
        NULL

`NULL` can be used to explicitly indicate a no-operation statement,
or as a placeholder for a statement to be implemented later.
It is syntactically treated as a valid placeholder where a statement is required.

.. _raise:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_null(medal CHAR)
    AS
    BEGIN
        CASE medal
            WHEN 'G' THEN
                DBMS_OUTPUT.put_line('Gold');
            WHEN 'S' THEN
                DBMS_OUTPUT.put_line('Silver');
            WHEN 'B' THEN
                DBMS_OUTPUT.put_line('Bronze');
            ELSE
                NULL;
        END CASE;
    END;

RAISE
=====

::

    <raise_statement> ::=
        RAISE [ <identifier> ]

Raises an exception.
The exception name *identifier* must be a :ref:`system exception <exception>`
or a :ref:`user-declared exception <exception_decl>`.
Within an exception handler's `THEN` clause, the exception name may be omitted,
in which case the currently handled exception is raised again.

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
            RAISE;      -- Re-raises the currently handled `invalid_input` exception
        WHEN OTHERS THEN
            DBMS_OUTPUT.put_line('unknown exception');
            RAISE;      -- Re-raises the currently handled exception
    END;

RETURN
======

::

    <return_statement> ::=
        RETURN [ <expression> ]

Branches execution to the point following the call to the current routine.
If the current routine is a function, a return value *expression* must be provided
that is convertible to the function’s declared return type.
If the current routine is a procedure, specifying a return value causes an error.

Procedure Call
==============

::

    <procedure_call> ::=
        <identifier> [ <function_argument> ]
    <function_argument> ::= ( [ <expression> [ , <expression>, ... ] ] )

Calls the procedure identified by *identifier*, optionally passing *function_argument* values.
The number and types of arguments must match those declared in the procedure.
Arguments passed to OUT parameters must be variables or other OUT parameters,
as their values will be modified by the called procedure.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE callee(o OUT INT)
    AS
    BEGIN
        ...
    END;

    CREATE OR REPLACE PROCEDURE proc(i INT, o OUT INT)
    AS
        v INT;
        c CONSTANT INT := 0;
    BEGIN
        callee(i);   -- Error: IN parameter
        callee(o);   -- OK: OUT parameter
        callee(v);   -- OK: variable
        callee(c);   -- Error: constant
    END;

The called procedure may be a stored procedure or a local procedure.
If an error occurs during the execution of a stored procedure call,
a `SQL_ERROR` exception is raised.

IF
==

::

    <if_statement> ::=
        IF <expression> THEN <seq_of_statements>
        [ <elsif_part> [ <elsif_part> ... ] ]
        [ <else_part> ]
        END IF
    <elsif_part> ::= ELSIF <expression> THEN <seq_of_statements>
    <else_part> ::= ELSE <seq_of_statements>

Provides an If-Then-Else construct similar to those in general-purpose programming languages.
The *expression* following `IF` and `ELSIF` must be of type `BOOLEAN`.

.. _loop:

LOOP
====

PL/CSQL provides five forms of loop statements as shown below.
The first three are similar to loops found in general-purpose programming languages.
The last two are used for iterating over the results of a SELECT statement.

::

    <loop_statement> ::=
          <label_declaration>? LOOP <seq_of_statements> END LOOP                          # basic-loop
        | <label_declaration>? WHILE <expression> LOOP <seq_of_statements> END LOOP       # while-loop
        | <label_declaration>? FOR <iterator> LOOP <seq_of_statements> END LOOP           # for-iter-loop
        | <label_declaration>? FOR <for_cursor> LOOP <seq_of_statements> END LOOP         # for-cursor-loop
        | <label_declaration>? FOR <for_static_sql> LOOP <seq_of_statements> END LOOP     # for-static-sql-loop

    <label_declaration> ::= '<<' <identifier> '>>'

    <iterator> ::= <identifier> IN [ REVERSE ] <lower_bound> .. <upper_bound> [ BY <step> ]

    <for_cursor>      ::= <record> IN <cursor> [ <function_argument> ]
    <for_static_sql>  ::= <record> IN ( <select_statement> )

* *label_declaration*: a label can only be declared at the beginning of a loop statement. It is used by CONTINUE or EXIT statements in the loop body to indicate which loop to branch to.
* In the *while-loop*, the condition *expression* must be of type BOOLEAN.
* In the *for-iter-loop*, *lower_bound*, *upper_bound*, and *step* must all be of types convertible to INTEGER. At runtime, if *step* is less than or equal to 0, a VALUE_ERROR exception is raised.
  If REVERSE is not specified, the *identifier* is initialized with *lower_bound* and the loop body is executed if the value is less than or equal to *upper_bound*. After each iteration, it increases by *step* and continues while still satisfying the condition.
  If REVERSE is specified, *identifier* is initialized with *upper_bound* and the loop body is executed if the value is greater than or equal to *lower_bound*. After each iteration, it decreases by *step* and continues while the condition holds.
  The loop variable *identifier* is treated as an INTEGER-type variable within the loop body.
* *for-cursor-loop* and *for-static-sql-loop* forms are used to iterate over the result set of a cursor or SELECT statement. The SELECT statement must not contain an INTO clause or a compile-time error occurs.
  For each iteration, a row from the result set is assigned to *record*, and the loop body executes. Each column of the row can be referenced as *record.column* within the body.

A basic form of LOOP typically includes an internal condition to exit the loop, as shown below:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE print_nation_athletes(nation CHAR)
    AS
        code INT;
        name VARCHAR(40);
        CURSOR c IS SELECT code, name from athlete where nation_code = nation;
    BEGIN
        OPEN c;
        LOOP
            FETCH c INTO code, name;
            EXIT WHEN c%NOTFOUND;
            DBMS_OUTPUT.PUT_LINE('code: ' || code || ' name: ' || name);
        END LOOP;
        CLOSE c;
    END;

The following is a simple example of using a WHILE loop:

.. code-block:: sql

    CREATE OR REPLACE FUNCTION sum_upto(n INT) RETURN INT
    AS
        sum INT := 0;
        i INT := 1;
    BEGIN
        WHILE i <= n LOOP
            sum := sum + i;
            i := i + 1;
        END LOOP;

        RETURN sum;
    END;

The example below demonstrates the usage of a FOR iterator loop:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE mult_tables
    AS
    BEGIN
        FOR i IN 2 .. 9 LOOP
            DBMS_OUTPUT.put_line('table ' || i);

            FOR j IN 1 .. 9 LOOP
                DBMS_OUTPUT.put_line(i || ' x ' || j || ' = ' || i*j);
            END LOOP;

            DBMS_OUTPUT.put_line('');
        END LOOP;
    END;

The following shows an example of querying the same SELECT statement using two different forms of FOR loop.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE athlete_history(p_name VARCHAR)
    AS
        CURSOR my_cursor IS
        SELECT host_year, score
        FROM history
        WHERE athlete = p_name;
    BEGIN
        -- For-Cursor Loop
        FOR r IN my_cursor LOOP
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);
        END LOOP;

        -- For-Select Loop
        FOR r IN (SELECT host_year, score FROM history WHERE athlete = p_name) LOOP
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);
        END LOOP;
    END;

.. _case_stmt:

CASE Statement
==============

The CASE statement sequentially evaluates multiple conditions and executes the statements associated with the first condition that evaluates to true.

::

    <case_statement> ::=
          CASE <expression> { WHEN <expression> THEN <seq_of_statements> }... [ ELSE <seq_of_statements> ] END CASE
        | CASE { WHEN <expression> THEN <seq_of_statements> }... [ ELSE <seq_of_statements> ] END CASE

There are two forms of CASE statements:

* The first form includes an expression immediately after the CASE keyword. It first evaluates this expression once, then compares it sequentially with the expressions in the WHEN clauses to find a match. If a match is found, the corresponding THEN clause statements are executed.
* The second form does not include an expression after the CASE keyword. Each WHEN clause expression must be of BOOLEAN type and is evaluated one by one until the first TRUE expression is found. The associated THEN clause statements are then executed.

Both forms may optionally include an ELSE clause, which specifies statements to execute if no WHEN clause matches.
If no WHEN clause matches and there is no ELSE clause, the system exception CASE_NOT_FOUND is raised.

The following is an example of the first form of the CASE statement:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE print_even_odd(i INTEGER)
    AS
    BEGIN
        CASE i MOD 2
            WHEN 0 THEN
                DBMS_OUTPUT.put_line('Even');
            WHEN 1 THEN
                DBMS_OUTPUT.put_line('Odd');
            ELSE
                DBMS_OUTPUT.put_line('Null');
        END CASE;
    END;

The following is an example of the second form of the CASE statement with similar behavior:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE print_even_odd(i INTEGER)
    AS
    BEGIN
        CASE
            WHEN i MOD 2 = 0 THEN
                DBMS_OUTPUT.put_line('Even');
            WHEN i MOD 2 = 1 THEN
                DBMS_OUTPUT.put_line('Odd');
            ELSE
                DBMS_OUTPUT.put_line('Null');
        END CASE;
    END;

