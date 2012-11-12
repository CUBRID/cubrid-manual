***********
Query Tools
***********

**CSQL Interpreter**

**Starting the CSQL Interpreter**

The CSQL Interpreter is a program used to execute the SQL statements and retrieve results in a way that CUBRID supports. The entered SQL statements and the results can be stored as a file. For more information, see
`Introduction to the CSQL Interpreter <#csql_csql_intro_htm>`_
and
`CSQL Execution Mode <#csql_csql_exec_mode_htm>`_
.

CUBRID also provides a GUI-based program called "CUBRID Manager" or "CUBRID Query Browser." By using these tools, you can execute all SQL statements and retrieve results in the query editor of CUBRID Manager. For more information, see
`Using CUBRID Manager or Query Browser <#gs_gs_manager_htm>`_
.

This section describes how to use the CSQL Interpreter on the Linux environment

**Starting the CSQL Interpreter**

You can start the CSQL program in the shell as shown below. At the initial installation,
**PUBLIC**
and
**DBA**
users are provided and the passwords of the users not set. If no user is specified while the CSQL Interpreter is executed,
**PUBLIC**
is used for log-in.

% csql demodb

          CUBRID SQL Interpreter

Type ';help' for help messages.

csql> ;help

=== <Help: Session Command Summary> ===

    All session commands should be prefixed by ';' and only blanks/tabs

    can precede the prefix. Capitalized characters represent the minimum

    abbreviation that should be entered to execute the specified command.

 

    ;REAd   [<file-name>]       - reads a file into command buffer.

    ;Write  [<file-name>]       - (over)writes command buffer into a file.

    ;APpend [<file-name>]       - appends command buffer into a file.

    ;PRINT                      - prints command buffer.

    ;SHELL                      - executes shell.

    ;CD                         - changes current working directory.

    ;EXit                       - exits program.

 

    ;CLear                      - clears the content of command buffer.

    ;EDIT                       - Edits the content of command buffer.

    ;List                       - displays the content of command buffer.

 

    ;RUn                        - executes command buffer.

    ;Xrun                       - executes command buffer and clear the content

    ;COmmit                     - commits the current transaction.

    ;ROllback                   - roll backs the current transaction.

    ;AUtocommit [ON|OFF]        - enables/disables auto commit mode.

    ;REStart                    - restarts database.

 

    ;SHELL_Cmd [shell-cmd]      - configures or displays specified shell.

    ;EDITOR_Cmd [editor-cmd]    - configures or displays specified editor.

    ;PRINT_Cmd [print-cmd]      - configures or displays specified print command.

    ;PAger_Cmd [pager-cmd]      - configures or display specified pager.

  

    ;DATE                       - displays the local time and date.

    ;DATAbase                   - displays the name of database being accessed.

    ;SChema class-name          - displays schema information of a class.

    ;TRigger [`*'|trigger-name] - displays trigger definition.

    ;Get system_parameter       - gets the value of a system parameter.

    ;SEt system_parameter=value - configures the value of a system parameter.

    ;PLan [simple|detail|off]   - displays query execution plan.

    ;Info <command>             - displays internal information.

    ;TIme [ON/OFF]              - enables/disables to display the query execution time.

    ;HISTORYList                - displays list of the executed queries.

    ;HISTORYRead <history_num>  - reads query corresponding to the history number from command buffer.

    ;HElp                       - displays this help message.

 

csql>

**Executing the SQL with CSQL**

After the CSQL has been executed, you can enter the SQL into the CSQL prompt. Each SQL statement must end with a semicolon (;). Multiple SQL statements can be entered in a single line. You can find the simple usage of the session commands with the ;help command. For more information, see
`Session Commands <#csql_csql_sessioncommand_htm>`_
.

% csql demodb

CUBRID SQL Interpreter

Type ';help' for help messages.

csql> select * from olympic;

=== <Result of SELECT Command in Line 1> ===

 

    host_year  host_nation           host_city             opening_date  closing

_date  mascot                slogan                introduction

================================================================================

=======================================================================

         2004  'Greece'              'Athens'              08/13/2004    08/29/2

004    'Athena  Phevos'      'Welcome Home'        'In 2004 the Olympic Games returned to Greece, the home of both the ancient Olympics and the first modern Olympics...'

 

<
omitted
>

25 rows selected.

 

Current transaction has been committed.

 

1 command(s) successfully processed.

csql> SELECT SUM(n) FROM (SELECT gold FROM participant WHERE nation_code='KOR'

csql> UNION ALL SELECT silver FROM participant WHERE nation_code='JPN') AS t(n);

 

=== <Result of SELECT Command in Line 1> ===

 

       sum(n)

=============

           82

 

1 rows selected.

 

Current transaction has been committed.

 

1 command(s) successfully processed.

csql> ;exit

**CUBRID Manager and Query Browser**

**Starting the CUBRID Manager Server**

If you want to use the CUBRID Manager client, you should first run the CUBRID Manager server. To run the server, you just need to execute
**cubrid service start**
once you have CUBRID installed in your system. For more information, see
`Starting and Stopping CUBRID Manager Server <#admin_admin_service_manager_star_151>`_
.

**Starting the CUBRID Manager Client**

CUBRID Manager is an exclusive CUBRID database management tool that provides features to manage CUBRID and execute queries vai a GUI environment. It is called the CUBRID Manager client to distinguish it from the CUBRID Manager server located on the server side. It can be run only on the Java Runtime Environment (JRE) or the Java Development Kit 1.6 or later because it is written in Java.

The default user of CUBRID Manager is
**admin**
and its password is also
**admin**
. Please note that a CUBRID Manager user is different from a database user. A CUBRID Manager user can performs tasks such as starting/terminating one or more databases and managing brokers. A database user can performs tasks such as executing queries on specific databases. When you install a database for the first time,
**PUBLIC**
and
**DBA**
users are configured by default and no password is specified for either.

To download the CUBRID Manager client or get more information, click
`http://www.cubrid.org/wiki_tools/entry/cubrid-manager <http://www.cubrid.org/wiki_tools/entry/cubrid-manager>`_
.

**Starting the CUBRID Query Browser**

CUBRID Query Browser is a light version of the CUBRID Manager client. It eliminates database management related features, and provides only query execution related features; with CUBRID Query Browser, you can execute query statements or execute a database definition language (DDL) by using menus.

A big difference between CUBRID Query Browser and the CUBRID Manager client is that the CUBRID Query Browser can be used regardless of whether the CUBRID Manager server is running. Therefore, it is not required to have a user account for CUBRID Manager when using CUBIRD Query Browser. However, you cannot start/terminate a database and monitor database/broker/HA in CUBRID Query Browser.

To download the CUBRID Query Browser program or get more information, click 
`http://www.cubrid.org/wiki_tools/entry/cubrid-query-browser <http://www.cubrid.org/wiki_tools/entry/cubrid-query-browser>`_
.

**CUBRID Drivers**

The drivers supported by CUBRID are as follows:

*   CUBRID JDBC driver (Installing and Configuring JDBC, Programming, API References,
    `Downloads <http://www.cubrid.org/?mid=downloads&item=jdbc_driver>`_
    )



*   CUBRID CCI driver (CCI Overview, Programming, API References,
    `Downloads <http://www.cubrid.org?mid=downloads&item=cci_driver>`_
    )



*   CUBRID PHP driver (Installing and Configuring PHP, Programming, API References,
    `Downloads <http://www.cubrid.org/?mid=downloads&item=php_driver&driver_type=phpdr>`_
    )



*   CUBRID PDO driver (Installing and Configuring PDO, Programming, API References,
    `Downloads <http://www.cubrid.org/?mid=downloads&item=php_driver&driver_type=pdo>`_
    )



*   CUBRID ODBC driver (Installing and Configuring ODBC, Programming, API References,
    `Downloads <http://www.cubrid.org/?mid=downloads&item=odbc_driver>`_
    )



*   CUBRID OLE DB driver (Installing and Configuring OLE DB, Programming, API References,
    `Downloads <http://www.cubrid.org/?mid=downloads&item=oledb_driver>`_
    )



*   CUBRID ADO.NET driver (Installing and Configuring ADO.NET, Programming, API References,
    `Downloads <http://www.cubrid.org/?mid=downloads&item=ado_dot_net_driver>`_
    )



*   CUBRID Perl driver (Installing and Configuring Perl, API References,
    `Downloads <http://www.cubrid.org/?mid=downloads&item=perl_driver>`_
    )



*   CUBRID Python driver (Installing and Configuring Python, Programming, API References,
    `Downloads <http://www.cubrid.org/?mid=downloads&item=python_driver>`_
    )



*   CUBRID Ruby driver (Installing and Configuring Ruby, API References,
    `Downloads <http://www.cubrid.org/?mid=downloads&item=ruby_driver>`_
    )



Among those of drivers, JDBC, ODBC, and CCI drivers are automatically downloaded while CUBRID is being installed. Thus, you do not have to download them manually.
