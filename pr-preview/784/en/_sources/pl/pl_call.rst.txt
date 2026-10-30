-----------------------------
Call a Stored Procedure
-----------------------------

Registered stored procedures and stored functions can be called using the :doc:`/sql/query/call` statement or in SQL statements.
In general, stored procedures are called using the :doc:`/sql/query/call` statement, and stored functions are used in SQL statements where a return value is needed.

This chapter provides a detailed explanation of the considerations when calling stored procedures and stored functions.

.. _pl_call_stmt:

CALL Statement
==============

You can call a stored procedure using the :doc:`/sql/query/call` statement as follows in the CSQL interpreter.

.. code-block:: sql

    -- Execute in csql
    CREATE PROCEDURE hello ()
    AS
    BEGIN
        DBMS_OUTPUT.put_line('Hello, CUBRID!');
    END;

    ;server-output on

    -- Call
    CALL hello();

::
    
      Result              
    ======================
      NULL                

    <DBMS_OUTPUT>
    ====
    Hello, CUBRID!

.. _pl_call_sql:

Calling in SQL Statements
===========================

In the case of calling in SQL statements, stored functions are typically used to return values. 
The following example shows how to calculate the number of medals for a specific country using a stored function.

.. code-block:: sql

    CREATE FUNCTION count_medals(nation STRING) RETURN INT
    AS
    cnt INT;
    BEGIN
        SELECT COUNT(*) INTO cnt
        FROM game
        WHERE nation_code = nation;
        RETURN cnt;
    END;

    SELECT count_medals('USA');
    SELECT count_medals('KOR');

::

      count_medals('USA')
    =====================
                     1118

      count_medals('KOR')
    =====================
                      316

.. _pl_nested_call_limits:

Limitations on Nested Procedure Calls
=======================================

The maximum allowed nested procedure call limit is **16**.
Nested procedure call refers to calling another procedure from within a procedure.
In the case of recursive calls, the limit is not applied.

The following is an example of the limitations on nested procedure calls.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_factorial_query(n BIGINT) RETURN BIGINT
    AS
    k BIGINT;
    BEGIN
        IF n = 0 THEN
            RETURN 1;
        ELSE
            SELECT test_factorial_query(n - 1) INTO k;
            RETURN n * k;
        END IF;
    END;

    SELECT test_factorial_query(15);
    SELECT test_factorial_query(16);

::

      test_factorial_query(15)
    ==========================
                 1307674368000


    -- SELECT test_factorial_query(16);
    ERROR: Stored procedure execute error:
      ...
      (line 8, column 13) Stored procedure execute error:
      (line 8, column 13) Stored procedure execute error: Too many nested stored procedure call.

The following is an example of a recursive call that does not go through a query. In this case, the nested call limit is not applied.
Recursively calling the following example may result in an overflow of the BIGINT value or excessive use of system resources, so be careful.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test_factorial_constant(n BIGINT) RETURN BIGINT
    AS
    k BIGINT;
    BEGIN
        IF n = 0 THEN
            RETURN 1;
        ELSE
            RETURN n * test_factorial_constant(n - 1);
        END IF;
    END;

    SELECT test_factorial_constant(16);
    SELECT test_factorial_constant(25);

::

      test_factorial_constant(16)
    =============================
                   20922789888000

    -- SELECT test_factorial_constant(25);
    ERROR: Stored procedure execute error: 
      (line 8, column 20) data overflow in multiplication of BIGINT values
