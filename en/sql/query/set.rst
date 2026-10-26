
:meta-keywords: set statement, set system parameter, set user variable
:meta-description: The SET statement is the syntax that specifies a system parameter's value or user-defined variables.

***
SET
***

The **SET** statement is the syntax that specifies a system parameter's value or user-defined variables.

System Parameter
================

You can define a value of a system parameter with SQL syntax by CSQL interpreter or a query editor of CUBRID Manager. However, be cautious that updatable parameters are limited. For the information of updatable parameters, see :ref:`cubrid-conf`.

::

    SET SYSTEM PARAMETERS 'parameter_name=value [{; name=value}...]'

**DEFAULT** for *value* will reset the parameter to its default value with an exception of **call_stack_dump_activation_list** parameter. 

.. code-block:: sql

    SET SYSTEM PARAMETERS 'lock_timeout=DEFAULT';
    
User Variables
==============

You can create user-defined variables in two ways. One is to use the **SET** statement and the other is to use the assignment statement of user-defined variables within SQL statements. You can delete the user-defined variables that you defined with the **DEALLOCATE** or the **DROP** statements.

The user-defined variables are also called session variables as they are used for maintaining connections within one application. The user-defined variables are used within the part of a connection session, and the user-defined variables defined by an application cannot be accessed by other applications. When an application terminates connections, all variables will be removed automatically. The user-defined variables are limited to twenty per connection session for an application. If you already have twenty user-defined variables and want to define a new user-defined variable, you must remove some variables with the **DROP VARIABLE** statement.

You can use user-defined variables in most SQL statements. If you define user-defined variables and refer to them in one statement, the sequence is not guaranteed. That is, if you refer to the variables specified in the **SELECT** list of the **HAVING**, **GROUP BY** or **ORDER BY** clause, you may not get the values in the sequence you expect. You cannot also use user-defined variables as identifiers, such as column names or table names within SQL statements

The user-defined variables are not case-sensitive. The user-defined variable type can be one of the **SHORT**, **INTEGER**, **BIGINT**, **FLOAT**, **DOUBLE**, **NUMERIC**, **CHAR**, **VARCHAR**, **BIT** and **BIT VARYING**. Other types will be converted to the **VARCHAR** type.

.. code-block:: sql

    SET @v1 = 1, @v2=CAST(1 AS BIGINT), @v3 = '123', @v4 = DATE'2010-01-01';
     
    SELECT typeof(@v1), typeof(@v2), typeof(@v3), typeof(@v4);
     
::

       typeof(@v1)         typeof(@v2)         typeof(@v3)         typeof(@v4)
    ======================================================================================
      'integer'           'bigint'            'character (-1)'    'character varying (1073741823)

The user-defined variables can be changed when you define values.

.. code-block:: sql

    SET @v = 'a'; 
    SET @v1 = 10;

    SELECT @v := 1, typeof(@v), typeof(@v1);
     
::

      @v := 1                typeof(@v)            typeof(@v1)
    ==================================================================
      1                     'integer'             'integer'

.. code-block:: sql

    SET @v1 = '1';

    SELECT typeof(@v1);

::

       typeof(@v1)
    ======================
      'character (-1)'

However, a user-defined variable has a single type within one SQL statement. If the SQL statement assigns a value to the variable with the ':=' operator, the type of the assigned expression is the type of the variable in that SQL statement; otherwise, it is the type of the value that the variable holds when the SQL statement is executed. An error is returned if one SQL statement assigns expressions of different types to the same variable, or if it reads a value whose type differs from the variable's type in that SQL statement. Strings with different character sets are regarded as different types. Assigning a value of another type with a separate **SET** statement is allowed.

.. code-block:: sql

    SET @c = 1;

    -- The type of @c in this SQL statement is VARCHAR, so reading the INTEGER value 1 returns an error.
    SELECT @c, @c := 'x' FROM db_root;

    -- An INTEGER expression and a VARCHAR expression are assigned to the same variable, so an error is returned.
    SELECT @c := @c + 1, @c := 'x' FROM db_root;

::

    In the command from line 4,

    ERROR: Session variable @c has the type character varying charset utf8 in this statement, not integer.

    In line 9, column 32,

    ERROR: before '  FROM db_root; '
    Session variable @c has the type integer in this statement, not character varying charset utf8.

::

    <set_statement>
            : <set_statement>, <udf_assignment>
            | SET <udv_assignment>
            ;
     
    <udv_assignment>
            : @<name> = <expression>
            | @<name> := <expression>
            ;
     
    {DEALLOCATE|DROP} VARIABLE <variable_name_list>
    <variable_name_list>
           : <variable_name_list> ',' @<name>

*   You must define the variable names with alphanumeric characters and underscores (_).
*   When you define the variables within SQL statements, you should use the ':=' operator.

The following example shows how to define the variable a and assign a value 1 to it.

.. code-block:: sql

    SET @a = 1;
    SELECT @a;

::

      @a
    ======================
      1

The following example shows how to count the number of rows in the **SELECT** statement by using the user-defined variable.

.. code-block:: sql

    CREATE TABLE t (i INTEGER);
    INSERT INTO t(i) VALUES(2),(4),(6),(8);
     
    SET @a = 0;
     
    SELECT @a := @a+1 AS row_no, i FROM t;

::

      row_no                          i
     ===================================
      1                               2
      2                               4
      3                               6
      4                               8
      
    4 rows selected.

The following example shows how to use the user-defined variable as the input of bind parameter specified in the prepared statement.

.. code-block:: sql

    SET @a:=3;
     
    PREPARE stmt FROM 'SELECT i FROM t WHERE i < ?';
    EXECUTE stmt USING @a;

::

                i
    =============
                2

The following example shows how to declare the user-defined variable by using the ':=' operator.

.. code-block:: sql

    SELECT @a := 1, @user_defined_variable := 'user defined variable';
    UPDATE t SET i = (@var := 1);

The following example shows how to delete the user-defined variable *a* and *user_defined_variable*.

.. code-block:: sql

    DEALLOCATE VARIABLE @a, @user_defined_variable;
    DROP VARIABLE @a, @user_defined_variable;

.. note::

    The user-defined variables that are defined by the **SET** statement start by connecting an application to a server and will be maintained until the application terminates the connection. The connection maintained during this period is called a session. When an application terminates the connection or when there are no requests for a certain period of time, the session will expire, and the user-defined variables will be deleted as a result. You can set the session time with the **session_state_timeout** parameter of **cubrid.conf**; the default value is **21600** seconds (=6 hours).

    The data managed by the session includes **PREPARE** statements, the user-defined variables, the last ID inserted (**LAST_INSERT_ID**) and the number of rows affected by the statement that you execute at the end (**ROW_COUNT**).

.. note::

    An SQL statement that reads or assigns a user-defined variable (except the **SET** statement and the **DROP VARIABLE** statement) is neither stored in nor looked up from the plan cache, and is compiled at every execution, because the type of the variable can change between executions. Such a statement does not replace its constants with ?:N (the replacement that lets the same query with different constant values share one query plan), so the constants appear unchanged in the rewritten query shown in the trace.

    .. code-block:: sql

        SET @c = 0;
        SET TRACE ON;
        SELECT COUNT(*) FROM t WHERE i > 2 AND @c = 0;
        SHOW TRACE;

    ::

          trace
        ======================
          '
        Query Plan:
          TABLE SCAN (dba.t)

          rewritten query: select count(*) from [dba.t] [dba.t] where ([dba.t].i>2) and @c=0

        Trace Statistics:
          SELECT (time: 0, fetch: 2, fetch_time: 0, ioread: 0)
            SCAN (table: dba.t), (heap time: 0, fetch: 2, ioread: 0, readrows: 4, rows: 3)
        '
