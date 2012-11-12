**
DO
**


**DO**

**Description**

The
**DO**
statement executes the specified expression, but does not return the result. This can be used to determine whether or not the syntax of the expression is correct because an error is returned when a specified expression does not comply with the syntax. In general, the execution speed of the
**DO**
statement is higher than that of the
**SELECT**
statement because the database server does not return the operation result or errors.

**Syntax**

**DO**
*expression*

*   *expression*
    : Specifies an expression.



**Example**

DO 1+1;

DO SYSDATE + 1;

DO (SELECT count(*) FROM athlete);

***
SET
***


**SET**

**Description**

The
**SET**
statement is the syntax that specifies user-defined variables and the method that you can use to store values.

You can create user-defined variables in two ways. One is to use the
**SET**
statement and the other is to use the assignment statement of user-defined variables within SQL statements. You can delete the user-defined variables that you defined with the
**DEALLOCATE**
or the
**DROP**
statements.

The user-defined variables are also called session variables as they are used for maintaining connections within one application. The user-defined variables are used within the part of a connection session, and the user-defined variables defined by an application cannot be accessed by other applications. When an application terminates connections, all variables will be removed automatically. The user-defined variables are limited to twenty per connection session for an application. If you already have twenty user-defined variables and want to define a new user-defined variable, you must remove some variables with the
**DROP VARIABLE**
statement.

You can use user-defined variables in most SQL statements. If you define user-defined variables and refer to them in one statement, the sequence is not guaranteed. That is, if you refer to the variables specified in the
**SELECT**
list of the
**HAVING**
,
**GROUP BY**
or
**ORDER BY**
clause, you may not get the values in the sequence you expect. You cannot also use user-defined variables as identifiers, such as column names or table names within SQL statements

The user-defined variables are not case-sensitive. The user-defined variable type can be one of the
**SHORT**
,
**INTEGER**
,
**BIGINT**
,
**FLOAT**
,
**DOUBLE**
,
**NUMERIC**
,
**CHAR**
,
**VARCHAR**
,
**NCHAR**
,
**VARNCHAR**
,
**BIT**
and
**BIT VARYING**
. Other types will be converted to the
**VARCHAR**
type.

SET @v1 = 1, @v2=CAST(1 AS BIGINT), @v3 = '123', @v4 = DATE'2010-01-01';

 

SELECT typeof(@v1), typeof(@v2), typeof(@v3), typeof(@v4);

 

   typeof(@v1)         typeof(@v2)         typeof(@v3)         typeof(@v4)

======================================================================================

  'integer'           'bigint'            'character var'     'character varying (10)'

The user-defined variables can be changed when you define values.

SELECT @v := 1, typeof(@v1), @v1:='1', typeof(@v1);

 

  @v := 1                typeof(@v1)          @v1 := '1'             typeof(@v1)

======================================================================================

  1                     'integer'             '1'                   'character (1)'

**Syntax**

<
*set_statement*
>

        : <
*set_statement*
>, <
*udf_assignment*
>

        |
**SET**
<
*udv_assignment*
>

        ;

 

<
*udv_assignment*
>

        : @<
*name*
> = <
*expression*
>

        | @<
*name*
> := <
*expression*
>

        ;

 

{
**DEALLOCATE**
|
**DROP**
}
**VARIABLE**
<
*variable_name_list*
>

<
*variable_name_list*
>

       : <
*variable_name_list*
> ',' @<
*name*
>

*   You must define the variable names with alphanumeric characters and underscores (_).



*   When you define the variables within SQL statements, you should use the ':=' operator.



**Example**

The following example shows how to define the variable 'a' and assign a value 1 to it.

SET @a = 1;

 

SELECT @a;

 

  @a

======================

  1

The following example shows how to count the number of rows in the
**SELECT**
statement by using the user-defined variable.

CREATE TABLE t (i INTEGER);

INSERT INTO t(i) VALUES(2),(4),(6),(8);

 

SET @a = 0;

 

SELECT @a := @a+1 AS row_no, i FROM t;

 

  row_no                          i

 ===================================

  1                               2

  2                               4

  3                               6

  4                               8

4 ROWS selected.

The following example shows how to use the user-defined variable as the input of bind parameter specified in the prepared statement.

SET @a:=3;

 

PREPARE stmt FROM 'SELECT i FROM t WHERE i < ?';

EXECUTE stmt USING @a;

 

            i

=============

            2

The following example shows how to declare the user-defined variable by using the ':=' operator.

SELECT @a := 1, @user_defined_variable := 'user defined variable';

UPDATE t SET i = (@var := 1);

The following example shows how to delete the user-defined variable
*a*
and
*user_defined_variable*
.

DEALLOCATE VARIABLE @a, @user_defined_variable;

DROP VARIABLE @a, @user_defined_variable;

**Remark**

The user-defined variables that are defined by the
**SET**
statement start by connecting an application to a server and will be maintained until the application terminates the connection. The connection maintained during this period is called a session. When an application terminates the connection or when there are no requests for a certain period of time, the session will expire, and the user-defined variables will be deleted as a result. You can set the session time with the
**session_state_timeout**
parameter of
**cubrid.conf**
; the default value is
**21600**
seconds (=6 hours).

The data managed by the session includes
**PREPARE**
statements, the user-defined variables, the last ID inserted (
**LAST_INSERT_ID**
) and the number of rows affected by the statement that you execute at the end (
**ROW_COUNT**
).

**SET**

**Description**

The
**SET**
statement is the syntax that specifies user-defined variables and the method that you can use to store values.

You can create user-defined variables in two ways. One is to use the
**SET**
statement and the other is to use the assignment statement of user-defined variables within SQL statements. You can delete the user-defined variables that you defined with the
**DEALLOCATE**
or the
**DROP**
statements.

The user-defined variables are also called session variables as they are used for maintaining connections within one application. The user-defined variables are used within the part of a connection session, and the user-defined variables defined by an application cannot be accessed by other applications. When an application terminates connections, all variables will be removed automatically. The user-defined variables are limited to twenty per connection session for an application. If you already have twenty user-defined variables and want to define a new user-defined variable, you must remove some variables with the
**DROP VARIABLE**
statement.

You can use user-defined variables in most SQL statements. If you define user-defined variables and refer to them in one statement, the sequence is not guaranteed. That is, if you refer to the variables specified in the
**SELECT**
list of the
**HAVING**
,
**GROUP BY**
or
**ORDER BY**
clause, you may not get the values in the sequence you expect. You cannot also use user-defined variables as identifiers, such as column names or table names within SQL statements

The user-defined variables are not case-sensitive. The user-defined variable type can be one of the
**SHORT**
,
**INTEGER**
,
**BIGINT**
,
**FLOAT**
,
**DOUBLE**
,
**NUMERIC**
,
**CHAR**
,
**VARCHAR**
,
**NCHAR**
,
**VARNCHAR**
,
**BIT**
and
**BIT VARYING**
. Other types will be converted to the
**VARCHAR**
type.

SET @v1 = 1, @v2=CAST(1 AS BIGINT), @v3 = '123', @v4 = DATE'2010-01-01';

 

SELECT typeof(@v1), typeof(@v2), typeof(@v3), typeof(@v4);

 

   typeof(@v1)         typeof(@v2)         typeof(@v3)         typeof(@v4)

======================================================================================

  'integer'           'bigint'            'character var'     'character varying (10)'

The user-defined variables can be changed when you define values.

SELECT @v := 1, typeof(@v1), @v1:='1', typeof(@v1);

 

  @v := 1                typeof(@v1)          @v1 := '1'             typeof(@v1)

======================================================================================

  1                     'integer'             '1'                   'character (1)'

**Syntax**

<
*set_statement*
>

        : <
*set_statement*
>, <
*udf_assignment*
>

        |
**SET**
<
*udv_assignment*
>

        ;

 

<
*udv_assignment*
>

        : @<
*name*
> = <
*expression*
>

        | @<
*name*
> := <
*expression*
>

        ;

 

{
**DEALLOCATE**
|
**DROP**
}
**VARIABLE**
<
*variable_name_list*
>

<
*variable_name_list*
>

       : <
*variable_name_list*
> ',' @<
*name*
>

*   You must define the variable names with alphanumeric characters and underscores (_).



*   When you define the variables within SQL statements, you should use the ':=' operator.



**Example**

The following example shows how to define the variable 'a' and assign a value 1 to it.

SET @a = 1;

 

SELECT @a;

 

  @a

======================

  1

The following example shows how to count the number of rows in the
**SELECT**
statement by using the user-defined variable.

CREATE TABLE t (i INTEGER);

INSERT INTO t(i) VALUES(2),(4),(6),(8);

 

SET @a = 0;

 

SELECT @a := @a+1 AS row_no, i FROM t;

 

  row_no                          i

 ===================================

  1                               2

  2                               4

  3                               6

  4                               8

4 ROWS selected.

The following example shows how to use the user-defined variable as the input of bind parameter specified in the prepared statement.

SET @a:=3;

 

PREPARE stmt FROM 'SELECT i FROM t WHERE i < ?';

EXECUTE stmt USING @a;

 

            i

=============

            2

The following example shows how to declare the user-defined variable by using the ':=' operator.

SELECT @a := 1, @user_defined_variable := 'user defined variable';

UPDATE t SET i = (@var := 1);

The following example shows how to delete the user-defined variable
*a*
and
*user_defined_variable*
.

DEALLOCATE VARIABLE @a, @user_defined_variable;

DROP VARIABLE @a, @user_defined_variable;

**Remark**

The user-defined variables that are defined by the
**SET**
statement start by connecting an application to a server and will be maintained until the application terminates the connection. The connection maintained during this period is called a session. When an application terminates the connection or when there are no requests for a certain period of time, the session will expire, and the user-defined variables will be deleted as a result. You can set the session time with the
**session_state_timeout**
parameter of
**cubrid.conf**
; the default value is
**21600**
seconds (=6 hours).

The data managed by the session includes
**PREPARE**
statements, the user-defined variables, the last ID inserted (
**LAST_INSERT_ID**
) and the number of rows affected by the statement that you execute at the end (
**ROW_COUNT**
).
