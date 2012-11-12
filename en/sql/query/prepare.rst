******************
PREPARED STATEMENT
******************


**PREPARED STATEMENT**

**Overview**

In general, the prepared statement is executed through the interface functions of JDBC, PHP, or ODBC; it can also be executed in the SQL level. The following SQL statements are provided for execution of prepared statement.

*   Prepare the SQL statement to execute.



PREPARE stmt_name FROM preparable_stmt

*   Execute the prepared statement.



EXECUTE stmt_name [USING value [, value] …]

*   Drop the prepared statement.



{DEALLOCATE | DROP} PREPARE stmt_name

**PREPARE Statement**

**Description**

The
**PREPARE**
statement prepares the query specified in
*preparable_stmt*
of the
**FROM**
clause and assigns the name to be used later when the SQL statement is referenced to
*stmt_name*
. See
`EXECUTE Statement <#syntax_syntax_prepared_execute_h_5018>`_
for example.

**Syntax**

**PREPARE**
*stmt_name*
**FROM**
*preparable_stmt*

*   *stmt_name*
    : The prepared statement is specified. If an SQL statement with the same
    *stmt_name*
    exists in the given client session, clear the existing prepared statement and prepare a new SQL statement. If the
    **PREPARE**
    statement is not executed properly due to an error in the given SQL statement, it is processed as if the
    *stmt_name*
    assigned to the SQL statement does not exist.



*   *preparable_stmt*
    : You must use only one SQL statement. Multiple SQL statements cannot be specified. You can use a question mark (?) as a bind parameter in the
    *preparable_stmt*
    statement and it should not be enclosed with quotes.



**Remark**

The
**PREPARE**
statement starts by connecting an application to a server and will be maintained until the application terminates the connection. The connection maintained during this period is called a session. You can set the session time with the
**session_state_timeout**
parameter of
**cubrid.conf**
; the default value is
**21600**
seconds (=6 hours).

The data managed by the session includes the
**PREPARE**
statement, user-defined variables, the last ID inserted (
**LAST_INSERT_ID**
), and the number of rows affected by the statement (
**ROW_COUNT**
) that you execute at the end.

**EXECUTE Statement**

**Description**

The
**EXECUTE**
statement executes the prepared statement. You can bind the data value after the
**USING**
clause if a bind parameter (?) is included in the prepared statement. You cannot specify user-defined variables like an attribute in the
**USING**
clause. An value such as literal and an input parameter only can be specified.

**Syntax**

**EXECUTE**
*stmt_name*
[
**USING**
*value*
[,
*value*
] …]

*   *stmt_name*
    : The name given to the prepared statement to be executed is specified. An error message is displayed if the
    *stmt_name*
    is not valid, or if the prepared statement does not exist.



*   *value*
    : The data to bind is specified if there is a bind parameter in the prepared statement. The number and the order of the data must correspond to that of the bind parameter. If it does not, an error message is displayed.



**Example**

PREPARE st FROM 'SELECT 1 + ?';

EXECUTE st USING 4;

   1+ ?:0

==========================

   5

 

SET @a=3;

EXECUTE st USING @a;

   1+ ?:0

==========================

   4

 

PREPARE st FROM 'SELECT ? + ?';

EXECUTE st USING 1,3;

   ?:0 + ?:1

==========================

   4

 

PREPARE st FROM 'SELECT ? + ?';

EXECUTE st USING 'a','b';

   ?:0 + ?:1

==========================

   'ab'

 

PREPARE st FROM 'SELECT FLOOR(?)';

EXECUTE st USING '3.2';

   floor( ?:0 )

==========================

   3.000000000000000e+000

 

PREPARE st FROM 'SELECT FLOOR(?)';

EXECUTE st USING 3.2;

   floor( ?:0 )

==========================

   3.0

**DEALLOCATE PREPARE/DROP PREPARE Statements**

**Description**

The statements
**DEALLOCATE PREPARE**
and
**DROP PREPARE**
are used interchangeably and they clear the prepared statement. All prepared statements are cleared automatically by the server when the client session is terminated even if the
**DEALLOCATE PREPARE**
or
**DROP PREPARE**
statement is not executed.

**Syntax**

{
**DEALLOCATE**
|
**DROP**
}
**PREPARE**
*stmt_name*

*   *stmt_name*
    : The name given to the prepared statement to be cleared is specified. An error message is displayed if the
    *stmt_name*
    is not valid, or if the prepared statement does not exist.



**Example**

DEALLOCATE PREPARE stmt1;
