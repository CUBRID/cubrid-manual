------------------
Declarations
------------------

Stored procedure/function declarations and block statements contain a declaration section called *seq_of_declare_specs*.
Within this section, you can declare variables, constants, exceptions, cursors, and local procedures/functions as defined by the grammar below.
Each declared item can be referenced within the *body* that follows the declaration section.

::

    <seq_of_declare_specs> ::= <declare_spec> { <declare_spec> }...
    <declare_spec> ::=
          <variable_def>
        | <constant_def>
        | <exception_decl>
        | <cursor_def>
        | <procedure_def>
        | <function_def>

See the sections below for descriptions of each declaration type.

:ref:`Local procedure/function declarations <local_routine_decl>` and :ref:`Block statements <block_stmt>`
have their own declaration and execution blocks, forming nested scopes.
When an inner scope declares a new item with the same name as one declared in an outer scope,
the name in the inner scope refers to the newly defined item, hiding the outer declaration.
However, outside the inner scope, the name still refers to the outer item.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE hidden_variable
    AS
        a INT := 3;
        b VARCHAR(10);

        -- Inner procedure
        PROCEDURE inner_proc
        AS
            a INT := 5;
            b FLOAT;
        BEGIN
            -- Here, a = 5, b is of type FLOAT
            ...
        END;

    BEGIN
        -- Here, a = 3, b is of type VARCHAR(10)
        ...

        -- Block statement
        DECLARE
            a INT := 7;
            b DATETIME;
        BEGIN
            -- Here, a = 7, b is of type DATETIME
            ...
        END;

        -- Back to a = 3, b is VARCHAR(10)
        ...
    END;

This kind of "name hiding" also applies to other types of declarations,
such as constants, procedure/function parameters, exceptions, cursors, and local procedures/functions.

However, if the hidden name is referenced earlier in the same declaration block, such as in an initializer expression,
a compile-time error will occur. Here's a simple example: in procedure `poo`, the parameter `a` is referenced
in the initializer of variable `i` inside the inner procedure. Then, a new variable with the same name `a` is declared below.
This results in a compile error message stating that the name has already been used in the same declaration block.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE poo(a INT) AS

        PROCEDURE inner AS
            i INT := a;
            a NUMERIC;
        BEGIN
            ...
        END;

    BEGIN
        ...
    END;

    ERROR: In line 5, column 9
    Stored procedure compile error: name A has already been used at line 4 and column 18 in the same declaration block

Variable Definitions
=====================

::

    <variable_def> ::=
        <identifier> <type_spec> [ [ NOT NULL ] <initial_value_part> ] ;

    <type_spec> ::=
          <builtin_type>
        | <table>.<column>%TYPE
        | <variable>%TYPE
        | <table>%ROWTYPE
        | <cursor>%ROWTYPE
    <initial_value_part> ::= { := | DEFAULT } <expression>

* *builtin_type*: system-defined types as described in :ref:`Data Types <types>`

When defining variables, you may optionally specify a `NOT NULL` constraint and an initial value.
If `NOT NULL` is specified, a non-null initial value must also be provided.
If no initializer is provided, the variable will implicitly be assigned `NULL`.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_variable
    AS
        a INT NOT NULL := 3;
        b VARCHAR(1) := 's';
        c FLOAT;        -- c = NULL
    BEGIN
        ...
    END;

Constant Definitions
=====================
::

    <constant_def> ::=
        <identifier> CONSTANT <type_spec> [ NOT_NULL ] <value_part> ;

    <type_spec> ::=
          <builtin_type>
        | <table>.<column>%TYPE
        | <variable>%TYPE
        | <table>%ROWTYPE
        | <cursor>%ROWTYPE
    <value_part> ::= { := | DEFAULT } <expression>

* *builtin_type*: system-defined types as described in :ref:`Data Types <types>`

Constant definitions must always include a value.
If `NOT NULL` is specified, the value must not be `NULL`.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_constant
    AS
        a CONSTANT INT NOT NULL := 3;
        b CONSTANT VARCHAR := 's';
        --c CONSTANT FLOAT;        -- Error
    BEGIN
        ...
    END;

.. _exception_decl:

Exception Declarations
======================

::

    <exception_decl> ::=
        <identifier> EXCEPTION ;

You can declare exceptions with any identifier name you choose.
The declared exceptions can be used in the :ref:`RAISE <raise>` statement and in the :ref:`WHEN <block_stmt>` clause of exception handling.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION text_exception(n INT) RETURN INT
    AS
        negative_argument EXCEPTION;
        too_big_argument EXCEPTION;
    BEGIN
        IF n < 0 THEN
            RAISE negative_argument;
        ELSIF n > 100 THEN
            RAISE too_big_argument;
        ELSIF n = 0 THEN
            RETURN 0;
        END IF;
        ...
    EXCEPTION
        WHEN negative_argument THEN
            DBMS_OUTPUT.put_line('error: negative argument ' || n);
            RETURN -1;
        WHEN too_big_argument THEN
            DBMS_OUTPUT.put_line('error: too big argument ' || n);
            RETURN -2;
    END;

.. _cursor_def:

Cursor Definitions
===================
::

    <cursor_def> ::=
        CURSOR <identifier> [ ( <seq_of_cursor_parameters> ) ] IS <select_statement> ;

    <seq_of_cursor_parameters> ::= <cursor_parameter> { , <cursor_parameter> }...
    <cursor_parameter> ::= <identifier> [ IN ] <type_spec>
    <type_spec> ::=
          <builtin_type>
        | <table>.<column>%TYPE
        | <variable>%TYPE
        | <table>%ROWTYPE
        | <cursor>%ROWTYPE

* *builtin_type*: system-defined types described in :ref:`Data Types <types>`

Cursors can have parameters just like procedures/functions, but only `IN` parameters are allowed.
These parameters can be referenced within the *select_statement*.
When calling :ref:`OPEN <cursor_manipulation>`, the provided arguments must match the declared number and types of the cursor parameters, and the `SELECT` statement will execute accordingly.

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

Cursors can be used explicitly as shown above via `OPEN`, `FETCH`, and `CLOSE` statements.
Alternatively, they can be used with a `FOR-LOOP`, in which the `OPEN`, `FETCH`, and `CLOSE` steps are performed implicitly:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_cursor_loop(p_name VARCHAR, p_year INTEGER)
    AS
        CURSOR my_cursor(a VARCHAR, y INTEGER) IS
        SELECT host_year, score
        FROM history
        WHERE athlete = a AND host_year >= y;
    BEGIN
        FOR r IN my_cursor(p_name, p_year) LOOP
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);
        END LOOP;
    END;

Note that `SELECT` statements used in cursor declarations must not include an `INTO` clause.

.. _local_routine_decl:

Local Procedure/Function Declarations
=====================================

You can define local procedures/functions that are only used inside the currently defined stored procedure/function,
following the syntax below. If a task is somewhat large or repeated,
modularizing it as a local procedure/function improves code readability and reusability.

::

    <procedure_def> ::=
        PROCEDURE <identifier> [ ( <seq_of_parameters> ) ] { IS | AS } [ <seq_of_declare_specs> ] <body> ;
    <function_def> ::=
        FUNCTION <identifier> [ ( <seq_of_parameters> ) ] RETURN <type_spec> { IS | AS } [ <seq_of_declare_specs> ] <body> ;

    <seq_of_parameters> ::= [ <parameter> { , <parameter> }... ]
    <parameter> ::= <identifier> [ { IN | IN OUT | INOUT | OUT } ] <type_spec> [ COMMENT 'param_comment_string' ]
    <type_spec> ::=
          <builtin_type>
        | <table>.<column>%TYPE
        | <variable>%TYPE
        | <table>%ROWTYPE
        | <cursor>%ROWTYPE
    <body> ::= BEGIN <seq_of_statements> [ EXCEPTION <seq_of_handlers> ] END [ <label_name> ]
    <seq_of_declare_specs> ::= <declare_spec> { <declare_spec> }...
    <seq_of_statements> ::= <statement> ; { <statement> ; }...
    <seq_of_handlers> ::= <handler> { <handler> }...
    <handler> ::= WHEN <exception_name> [ OR <exception_name> OR ... ] THEN <seq_of_statements>
    <exception_name> ::= identifier | OTHERS

* *parameter*: parameters can be declared as IN, IN OUT, INOUT, or OUT. IN OUT and INOUT are equivalent.
* *param_comment_string*: a comment string for the parameter.
* *builtin_type*: system-defined types described in :ref:`Data Types <types>`
* *body*: consists of at least one statement and optionally one or more exception handlers.
* *label_name*: must match the name of the procedure/function.
* *declare_spec*: one of variable, constant, exception, cursor, or local procedure/function declarations.
* *statement*: see the :doc:`Statements <plcsql_stmt>` section.
* *handler*: specifies the statements to execute when the designated exception is raised.
* *exception_name*: the exception *identifier* must be a :ref:`System Exception <exception>` or a user-declared one via :ref:`Exception Declaration <exception_decl>`. `OTHERS` matches any unhandled exceptions, and cannot be combined with other exception names using `OR`.

Unlike :ref:`stored procedures/functions <stored_proc>`,
a local procedure/function may have the same name as a :ref:`CUBRID built-in function <operators-and-functions>`.
In this case, the built-in function will be hidden within the scope of the local declaration.

In the case of a function, the *body* must return a value of the declared return type using a `RETURN` statement.
If a control path reaches the end of the function body without a `RETURN`, a compile-time error will occur.
For procedures, `RETURN` statements must not specify a return value.

Procedures/functions can reference themselves, enabling recursive calls.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION choose(m INT, n INT) RETURN INT
    AS
        invalid_argument EXCEPTION;

        -- Local function declaration
        FUNCTION factorial(n INT) RETURN INT
        AS
        BEGIN
            IF n < 0 THEN
                RAISE invalid_argument;
            ELSIF n <= 1 THEN
                RETURN 1;
            ELSE
                RETURN n * factorial(n - 1);    -- Recursive call
            END IF;
        END;
    BEGIN
        IF n > m OR n < 0 THEN
            RAISE invalid_argument;
        ELSE
            RETURN factorial(m) / factorial(n) / factorial(m - n);
        END IF;
    END;

Mutual recursive calls are also possible between inner procedures/functions declared in the same declaration block.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE ping_pong(cnt INT)
    AS
        PROCEDURE ping(n INT)
        AS
        BEGIN
            IF n <= 0 THEN
                DBMS_OUTPUT.put_line('-- end --');
            ELSE
                DBMS_OUTPUT.put_line('ping ->');
                pong(n - 1);     -- mutual recursive call
            END IF;
        END;

        PROCEDURE pong(n INT)
        AS
        BEGIN
            IF n <= 0 THEN
                DBMS_OUTPUT.put_line('-- end --');
            ELSE
                DBMS_OUTPUT.put_line('      <- pong');
                ping(n - 1);     -- mutual recursive call
            END IF;
        END;
    BEGIN
        ping(cnt);
    END;

When using recursion, be sure to include an appropriate termination condition to avoid infinite loops.
