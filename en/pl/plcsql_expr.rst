------------------
Expressions
------------------

The types of expressions in PL/CSQL are summarized by the following syntax:
::

    <expression> ::=
          <literal>                                 # literal
        | <identifier>                              # identifier
        | SQL%ROWCOUNT                              # result size of Static SQL
        | <cursor_expression> <cursor_attribute>    # cursor attribute
        | <expression> <binary_op> <expression>     # binary operation
        | <unary_op> <expression>                   # unary operation
        | ( <expression> )                          # parentheses
        | <identifier>.<identifier>                 # record field reference
        | <identifier> <function_argument>          # function call
        | <case_expression>                         # CASE expression
        | SQLCODE                                   # exception code
        | SQLERRM                                   # exception message
        | <expression> IS [ NOT ] NULL              # IS NULL expression
        | <expression> [ NOT ] BETWEEN <expression> AND <expression>        # BETWEEN expression
        | <expression> [ NOT ] IN ( <expression> [ , <expression>, ... ] )  # IN expression
        | <expression> [ NOT ] LIKE <expression> [ ESCAPE <expression> ]    # LIKE expression

    <literal> ::=
          DATE <quoted_string>
        | TIME <quoted_string>
        | DATETIME <quoted_string>
        | TIMESTAMP <quoted_string>
        | <numeric>
        | <quoted_string>
        | NULL
        | TRUE
        | FALSE

    <numeric> ::= UNSIGNED_INTEGER | FLOATING_POINT_NUM

    <cursor_attribute> ::= { %ISOPEN | %FOUND | %NOTFOUND | %ROWCOUNT }

    <binary_op> ::=
          AND | XOR | OR
        | = | <=> | != | <> | <= | >= | < | >
        | * | / | + | -
        | >> | << | & | ^ | '|'
        | ||

    <unary_op> ::= + | - | NOT | ~

    <case_expression> ::=
          CASE <expression> <case_expression_when_part>... [ ELSE <expression> ] END
        | CASE <case_expression_when_part>... [ ELSE <expression> ] END

    <case_expression_when_part> ::= WHEN <expression> THEN <expression>

Literals
=================
Literals include date/time, numeric, string, NULL, TRUE, and FALSE values.
Except that bit strings and collections are not supported, literal-related rules are the same as described in :ref:`SQL Literals <sql_literal>`.

Identifiers
=================
In PL/CSQL statements other than Static/Dynamic SQL, the following kinds of identifiers can be used:

* Variables, constants, cursors, exceptions, and local procedures/functions declared in the declaration section
* Parameters of procedures/functions
* Implicitly declared iterators of :ref:`FOR loops <loop>` of type integer or record

Using an identifier without explicit or implicit declaration results in a compile-time error.

Result Count for Static SQL
============================
`SQL%ROWCOUNT` is an expression that represents the result count immediately after executing a Static SQL statement.

* For SELECT statements not associated with cursors, the INTO clause must be used, and the result must contain exactly one row. Therefore, when the SELECT executes successfully, `SQL%ROWCOUNT` will be 1. If the result has 0 or more than 1 row and causes a runtime error, the value of `SQL%ROWCOUNT` is undefined.
* For INSERT, UPDATE, DELETE, MERGE, REPLACE, and TRUNCATE statements, it returns the number of affected rows.
* For COMMIT and ROLLBACK, it returns 0.

Cursor Attributes
=================

By appending %ISOPEN, %FOUND, %NOTFOUND, or %ROWCOUNT to a *cursor_expression* that evaluates to a cursor or SYS_REFCURSOR variable, you can check one of four cursor attributes.

* %ISOPEN: Whether the cursor is open (BOOLEAN)
* %FOUND: NULL before the first FETCH. Otherwise, whether the last FETCH returned 1 row (BOOLEAN). Raises INVALID_CURSOR Exception if the cursor is not open.
* %NOTFOUND: NULL before the first FETCH. Otherwise, whether the last FETCH returned 0 rows (BOOLEAN). Raises INVALID_CURSOR Exception if the cursor is not open.
* %ROWCOUNT: 0 before the first FETCH. Otherwise, the number of rows fetched so far (BIGINT). Raises INVALID_CURSOR Exception if the cursor is not open.

In the example below, the internal function `iterate_cursor()` iterates over records using cursor attributes and returns the total row count. If the passed cursor is not open (`%ISOPEN` is False), the function returns -1. Whether there are more records is determined using the `%NOTFOUND` attribute after FETCH. The `%ROWCOUNT` attribute increments with each FETCH and reflects the total number of rows fetched after the loop ends.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE cursor_attributes AS
        ...

        FUNCTION iterate_cursor(rc SYS_REFCURSOR) RETURN INT
        AS
            v VARCHAR;
        BEGIN
            IF rc%ISOPEN THEN
                LOOP
                    FETCH rc INTO v;
                    EXIT WHEN rc%NOTFOUND;

                    -- do something with v
                    ...

                END LOOP;

                RETURN rc%ROWCOUNT;     -- number of records
            ELSE
                RETURN -1;              -- error
            END IF;
        END;
    begin
        ...

    end;

If the *cursor_expression* evaluates to NULL, an INVALID_CURSOR Exception is raised.

Binary Ops, Unary Ops, Parentheses
==================================

PL/CSQL uses the following precedence for operators:

+--------------------------------------------------------------------+-------------------------------------+
| Operator                                                           | Description                         |
+====================================================================+=====================================+
| +, -, ~                                                            | Unary sign, bitwise NOT             |
+--------------------------------------------------------------------+-------------------------------------+
| \*, /, DIV, MOD                                                    | Multiplication, division,           |
|                                                                    | integer division, modulus           |
+--------------------------------------------------------------------+-------------------------------------+
| +, -                                                               | Addition, subtraction               |
+--------------------------------------------------------------------+-------------------------------------+
| ||                                                                 | String concatenation                |
+--------------------------------------------------------------------+-------------------------------------+
| <<, >>                                                             | Bitwise shift                       |
+--------------------------------------------------------------------+-------------------------------------+
| &                                                                  | Bitwise AND                         |
+--------------------------------------------------------------------+-------------------------------------+
| ^                                                                  | Bitwise XOR                         |
+--------------------------------------------------------------------+-------------------------------------+
| \|                                                                 | Bitwise OR                          |
+--------------------------------------------------------------------+-------------------------------------+
| IS NULL                                                            | NULL test                           |
+--------------------------------------------------------------------+-------------------------------------+
| LIKE                                                               | String pattern test                 |
+--------------------------------------------------------------------+-------------------------------------+
| BETWEEN                                                            | Range test                          |
+--------------------------------------------------------------------+-------------------------------------+
| IN                                                                 | Inclusion test                      |
+--------------------------------------------------------------------+-------------------------------------+
| =, <=>, <, >, <=, >=, <>, !=                                       | Comparison                          |
+--------------------------------------------------------------------+-------------------------------------+
| NOT                                                                | Logical NOT                         |
+--------------------------------------------------------------------+-------------------------------------+
| AND                                                                | Logical AND                         |
+--------------------------------------------------------------------+-------------------------------------+
| XOR                                                                | Logical XOR                         |
+--------------------------------------------------------------------+-------------------------------------+
| OR                                                                 | Logical OR                          |
+--------------------------------------------------------------------+-------------------------------------+

* In Non-static/dynamic SQL statements, the `%` operator cannot be used for modulo, so use MOD instead.
* In Non-static/dynamic SQL statements, logical operators `&&`, `||`, and `!` cannot be used, so use AND, OR, and NOT instead.
* In Non-static/dynamic SQL statements, string comparisons follow UTF8 encoding regardless of DB settings, 
  using lexicographical order of Unicode values.
  In Static/Dynamic SQL statements, encoding and comparison rules follow the database and table settings.

Parentheses can be used to explicitly specify evaluation precedence.

Record Field Reference
======================

PL/CSQL supports record variables in the following two cases:

* Implicitly declared record variables used for iterating over SELECT results in a FOR loop
* Record variables declared using %ROWTYPE

To reference a field in a record variable, append a dot and the field name to the record variable.

.. code-block:: sql

    CREATE PROCEDURE athlete_history(p_name VARCHAR)
    AS
        CURSOR my_cursor IS
        SELECT host_year, score
        FROM history
        WHERE athlete = p_name;
    BEGIN
        FOR r IN my_cursor LOOP     -- r: implicitly declared
            DBMS_OUTPUT.put_line('host_year: ' || r.host_year || ' score: ' || r.score);    -- r.<column-name>
        END LOOP;
    END;

Function Call
=============

In a function call expression, the number and types of arguments must match the function's declaration.
Arguments passed to OUT parameters of the function must be assignable variables or other OUT parameters, as their values will be changed by the function call.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION callee(o OUT INT) RETURN INT
    AS
    BEGIN
        ...
    END;

    CREATE OR REPLACE PROCEDURE proc(i INT, o OUT INT)
    AS
        v INT;
        c CONSTANT INT := 0;
    BEGIN
        ... callee(i) ...   -- Error: IN argument
        ... callee(o) ...   -- OK: OUT argument
        ... callee(v) ...   -- OK: variable
        ... callee(c) ...   -- Error: constant
    END;

Callable functions may be stored functions, local functions, or built-in functions.
Among these, built-in functions refer to the CUBRID built-in functions listed in :ref:`Operators and Functions <operators-and-functions>`.
However, the built-in function :ref:`IF <func_if>` cannot be used because it conflicts with PL/CSQL syntax.

If an error occurs while calling another stored function or built-in function, an SQL_ERROR Exception is raised.

CASE Expression
===============

A CASE expression sequentially evaluates conditions and returns the value associated with the first condition that is satisfied.

Like the :ref:`CASE statement <case_stmt>`, CASE expressions come in two forms:

* The first form includes an expression immediately after the CASE keyword. That expression is evaluated once, then compared against each WHEN clause’s expression. When a match is found, the corresponding THEN expression is evaluated and becomes the result of the CASE expression.
* The second form does not have an expression after the CASE keyword. In this case, each WHEN clause must be a BOOLEAN expression. The first one that evaluates to TRUE determines which THEN expression is returned as the CASE result.

Both forms may include an optional ELSE clause, whose expression becomes the CASE value if no conditions match. If no conditions match and no ELSE is present, the CASE expression evaluates to NULL.

The following is an example of the first form:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE print_even_odd(i INTEGER)
    AS
        s VARCHAR(5);
    BEGIN
        s := CASE i MOD 2
            WHEN 0 THEN 'Even'
            WHEN 1 THEN 'Odd'
            ELSE 'NULL'
        END;

        DBMS_OUTPUT.put_line(s);
    END;

The following shows the second form with equivalent behavior:

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE print_even_odd(i INTEGER)
    AS
        s VARCHAR(5);
    BEGIN
        s := CASE
            WHEN i MOD 2 = 0 THEN 'Even'
            WHEN i MOD 2 = 1 THEN 'Odd'
            ELSE 'NULL'
        END;

        DBMS_OUTPUT.put_line(s);
    END;

.. _sqlcode:

SQLCODE, SQLERRM
================

Inside an exception handling block, SQLCODE and SQLERRM represent the error code (INTEGER) and error message (STRING) of the current Exception.
Outside an exception block, SQLCODE is 0 and SQLERRM is 'no error'.

.. code-block:: sql

    CREATE OR REPLACE PROCEDURE test_sql_code_errm
    AS
    BEGIN
        ...
    EXCEPTION
        WHEN OTHERS THEN
            DBMS_OUTPUT.put_line('code=' || SQLCODE);
            DBMS_OUTPUT.put_line('error message' || SQLERRM);
    END;

