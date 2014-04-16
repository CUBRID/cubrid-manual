******************
PREPARED STATEMENT
******************

In general, the prepared statement is executed through the interface functions of JDBC, PHP, or ODBC; it can also be executed in the SQL level. The following SQL statements are provided for execution of prepared statement.

*   Prepare the SQL statement to execute. 

    ::

        PREPARE stmt_name FROM preparable_stmt

*   Execute the prepared statement. 

    ::

        EXECUTE stmt_name [USING value [, value] ...]

*   Drop the prepared statement. 

    ::

        {DEALLOCATE | DROP} PREPARE stmt_name

.. note::

    *   In SQL level, PREPARE statement is recommended to use only in CSQL interpreter. If it is used in the application program, it is not guaranteed to work normally.
    *   In SQL level, the number of PREPARE statements is limited to 20 per DB connection. It is limited to protect abusing DB server memory, because PREPARE statement in SQL level uses the memory of DB server.
    *   In the interface function, the number of prepared statements is limited to :ref:`MAX_PREPARED_STMT_COUNT <max-prepared-stmt-count>` of broker parameter per DB connection. If you use CUBRID SHARD, the number of prepared statements per shard proxy is limited to :ref:`SHARD_MAX_PREPARED_STMT_COUNT <shard-max-prepared-stmt-count>`, shard parameter.

PREPARE Statement
=================

The **PREPARE** statement prepares the query specified in *preparable_stmt* of the **FROM** clause and assigns the name to be used later when the SQL statement is referenced to *stmt_name*. See :ref:`execute-statement` for example. ::

    PREPARE stmt_name FROM preparable_stmt

*   *stmt_name* : The prepared statement is specified. If an SQL statement with the same *stmt_name* exists in the given client session, clear the existing prepared statement and prepare a new SQL statement. If the **PREPARE** statement is not executed properly due to an error in the given SQL statement, it is processed as if the *stmt_name* assigned to the SQL statement does not exist.

*   *preparable_stmt* : You must use only one SQL statement. Multiple SQL statements cannot be specified. You can use a question mark (?) as a bind parameter in the *preparable_stmt* statement and it should not be enclosed with quotes.

**Remark**

The **PREPARE** statement starts by connecting an application to a server and will be maintained until the application terminates the connection. The connection maintained during this period is called a session. You can set the session time with the **session_state_timeout** parameter of **cubrid.conf**; the default value is **21600** seconds (=6 hours).

The data managed by the session includes the **PREPARE** statement, user-defined variables, the last ID inserted (**LAST_INSERT_ID**), and the number of rows affected by the statement (**ROW_COUNT**) that you execute at the end.

.. _execute-statement:

EXECUTE Statement
=================

The **EXECUTE** statement executes the prepared statement. You can bind the data value after the **USING** clause if a bind parameter (?) is included in the prepared statement. You cannot specify user-defined variables like an attribute in the **USING** clause. A value such as literal and an input parameter only can be specified. ::

    EXECUTE stmt_name [USING value [, value] ...]

*   *stmt_name* : The name given to the prepared statement to be executed is specified. An error message is displayed if the *stmt_name* is not valid, or if the prepared statement does not exist.

*   *value* : The data to bind is specified if there is a bind parameter in the prepared statement. The number and the order of the data must correspond to that of the bind parameter. If it does not, an error message is displayed.

.. code-block:: sql

    PREPARE st FROM 'SELECT 1 + ?';
    EXECUTE st USING 4;
    
::

       1+ ?:0
    ==========================
       5
     
.. code-block:: sql

    PREPARE st FROM 'SELECT 1 + ?';
    SET @a=3;
    EXECUTE st USING @a;
    
::

       1+ ?:0
    ==========================
       4
     
.. code-block:: sql

    PREPARE st FROM 'SELECT ? + ?';
    EXECUTE st USING 1,3;
    
::

       ?:0 + ?:1
    ==========================
       4
     
.. code-block:: sql

    PREPARE st FROM 'SELECT ? + ?';
    EXECUTE st USING 'a','b';
    
::

       ?:0 + ?:1
    ==========================
       'ab'
     
.. code-block:: sql

    PREPARE st FROM 'SELECT FLOOR(?)';
    EXECUTE st USING '3.2';
    
::

       floor( ?:0 )
    ==========================
       3.000000000000000e+000

DEALLOCATE PREPARE/DROP PREPARE Statements
==========================================

The statements **DEALLOCATE PREPARE** and **DROP PREPARE** are used interchangeably and they clear the prepared statement. All prepared statements are cleared automatically by the server when the client session is terminated even if the **DEALLOCATE PREPARE** or **DROP PREPARE** statement is not executed. ::

    {DEALLOCATE | DROP} PREPARE stmt_name

*   *stmt_name* : The name given to the prepared statement to be cleared is specified. An error message is displayed if the *stmt_name* is not valid, or if the prepared statement does not exist.

.. code-block:: sql

    DEALLOCATE PREPARE stmt1;

***
SET
***

The **SET** statement is the syntax that specifies user-defined variables and the method that you can use to store values.

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

    SELECT @v := 1, typeof(@v1), @v1:='1', typeof(@v1);
     
::

      @v := 1                typeof(@v1)          @v1 := '1'             typeof(@v1)
    ======================================================================================
      1                     'integer'             '1'                    'character (-1)'

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

.. note:: \

    The user-defined variables that are defined by the **SET** statement start by connecting an application to a server and will be maintained until the application terminates the connection. The connection maintained during this period is called a session. When an application terminates the connection or when there are no requests for a certain period of time, the session will expire, and the user-defined variables will be deleted as a result. You can set the session time with the **session_state_timeout** parameter of **cubrid.conf**; the default value is **21600** seconds (=6 hours).

    The data managed by the session includes **PREPARE** statements, the user-defined variables, the last ID inserted (**LAST_INSERT_ID**) and the number of rows affected by the statement that you execute at the end (**ROW_COUNT**).

**
DO
**

The **DO** statement executes the specified expression, but does not return the result. This can be used to determine whether or not the syntax of the expression is correct because an error is returned when a specified expression does not comply with the syntax. In general, the execution speed of the **DO** statement is higher than that of the **SELECT** statement because the database server does not return the operation result or errors. ::

    DO expression

*   *expression* : Specifies an expression.

.. code-block:: sql

    DO 1+1;
    DO SYSDATE + 1;
    DO (SELECT count(*) FROM athlete);
