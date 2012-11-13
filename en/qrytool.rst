***********
Query Tools
***********

CSQL Interpreter
----------------

**Starting the CSQL Interpreter**

The CSQL Interpreter is a program used to execute the SQL statements and retrieve results in a way that CUBRID supports. The entered SQL statements and the results can be stored as a file. For more information, see `Introduction to the CSQL Interpreter <#csql_csql_intro_htm>`_ and `CSQL Execution Mode <#csql_csql_exec_mode_htm>`_ .

CUBRID also provides a GUI-based program called "CUBRID Manager" or "CUBRID Query Browser." By using these tools, you can execute all SQL statements and retrieve results in the query editor of CUBRID Manager. For more information, see `Using CUBRID Manager or Query Browser <#gs_gs_manager_htm>`_ .

This section describes how to use the CSQL Interpreter on the Linux environment

**Starting the CSQL Interpreter**

You can start the CSQL program in the shell as shown below. At the initial installation, **PUBLIC** and **DBA** users are provided and the passwords of the users not set. If no user is specified while the CSQL Interpreter is executed, **PUBLIC** is used for log-in. ::

	% csql demodb

			CUBRID SQL Interpreter


	Type `;help' for help messages.

	csql> ;help

	=== <Help: Session Command Summary> ===


	   All session commands should be prefixed by `;' and only blanks/tabs
	   can precede the prefix. Capitalized characters represent the minimum
	   abbreviation that you need to enter to execute the specified command.

	   ;REAd   [<file-name>]       - read a file into command buffer.
	   ;Write  [<file-name>]       - (over)write command buffer into a file.
	   ;APpend [<file-name>]       - append command buffer into a file.
	   ;PRINT                      - print command buffer.
	   ;SHELL                      - invoke shell.
	   ;CD                         - change current working directory.
	   ;EXit                       - exit program.

	   ;CLear                      - clear command buffer.
	   ;EDIT                       - invoke system editor with command buffer.
	   ;List                       - display the content of command buffer.

	   ;RUn                        - execute sql in command buffer.
	   ;Xrun                       - execute sql in command buffer,
									 and clear the command buffer.
	   ;COmmit                     - commit the current transaction.
	   ;ROllback                   - roll back the current transaction.
	   ;AUtocommit [ON|OFF]        - enable/disable auto commit mode.
	   ;REStart                    - restart database.

	   ;SHELL_Cmd  [shell-cmd]     - set default shell, editor, print and pager
	   ;EDITOR_Cmd [editor-cmd]      command to new one, or display the current
	   ;PRINT_Cmd  [print-cmd]       one, respectively.
	   ;PAger_cmd  [pager-cmd]

	   ;DATE                       - display the local time, date.
	   ;DATAbase                   - display the name of database being accessed.
	   ;SChema class-name          - display schema information of a class.
	   ;TRigger [`*'|trigger-name] - display trigger definition.
	   ;Get system_parameter       - get the value of a system parameter.
	   ;SEt system_parameter=value - set the value of a system parameter.
	   ;PLan [simple/detail/off]   - show query execution plan.
	   ;Info <command>             - display internal information.
	   ;TIme [ON/OFF]              - enable/disable to display the query
									 execution time.
	   ;HISTORYList                - display list of the executed queries.
	   ;HISTORYRead <history_num>  - read entry on the history number into command buffer.
	   ;HElp                       - display this help message.

**Executing the SQL with CSQL**

After the CSQL has been executed, you can enter the SQL into the CSQL prompt. Each SQL statement must end with a semicolon (;). Multiple SQL statements can be entered in a single line. You can find the simple usage of the session commands with the ;help command. For more information, see `Session Commands <#csql_csql_sessioncommand_htm>`_ . ::

	csql> select host_year, host_nation, host_city, opening_date, closing_date, mascot, slogan from olympic;

	=== <Result of SELECT Command in Line 1> ===

		host_year  host_nation           host_city             opening_date  closing_date  mascot                slogan              
	=================================================================================================================================
			 2004  'Greece'              'Athens'              08/13/2004    08/29/2004    'Athena  Phevos'      'Welcome Home'      
			 2000  'Australia'           'Sydney'              09/15/2000    10/01/2000    'Olly Syd Millie'     'Share the Spirit'  
			 1996  'USA'                 'Atlanta'             07/19/1996    08/09/1996    'Izzy'                'The Celebration of the Century'
			 1992  'Spain'               'Barcelona'           07/25/1992    08/09/1992    'Cobi'                'Friends Forever'   
			 1988  'Korea'               'Seoul'               09/17/1988    10/02/1988    'HODORI'              'Harmony and progress'
			 1984  'USA'                 'Los Angeles'         07/28/1984    08/12/1984    'Sam'                 'Play part in History'
			 1980  'USSR'                'Moscow'              07/19/1980    08/03/1980    'Misha'               NULL                
			 1976  'Canada'              'Montreal'            07/17/1976    08/01/1976    'Amik'                NULL                
			 1972  'Germany'             'Munich'              08/26/1972    09/10/1972    'Waldi'               NULL                
			 1968  'Mexico'              'Mexico City'         10/12/1968    10/27/1968    NULL                  NULL                
			 1964  'Japan'               'Tokyo'               10/10/1964    10/24/1964    NULL                  NULL                
			 1960  'Italy'               'Rome'                08/25/1960    09/11/1960    NULL                  NULL                
			 1956  'Australia'           'Melbourne'           11/22/1956    12/08/1956    NULL                  NULL                
			 1952  'Finland'             'Helsinki'            07/19/1952    08/03/1952    NULL                  NULL                
			 1948  'England'             'London'              07/29/1948    08/14/1948    NULL                  NULL                
			 1936  'Germany'             'Berlin'              08/01/1936    08/16/1936    NULL                  NULL                
			 1932  'USA'                 'Los Angeles'         07/30/1932    08/14/1932    NULL                  NULL                
			 1928  'Netherlands'         'Amsterdam'           07/28/1928    08/12/1928    NULL                  NULL                
			 1924  'France'              'Paris'               05/04/1924    07/27/1924    NULL                  NULL                
			 1920  'Belgium'             'Antwerp'             04/20/1920    08/12/1920    NULL                  NULL                
			 1912  'Sweden'              'Stockholm'           05/05/1912    07/22/1912    NULL                  NULL                
			 1908  'United Kingdom'      'London'              04/27/1908    10/31/1908    NULL                  NULL                
			 1904  'USA'                 'St. Louis'           07/01/1904    11/23/1904    NULL                  NULL                
			 1900  'France'              'Paris'               05/14/1900    10/28/1900    NULL                  NULL                
			 1896  'Greece'              'Athens'              04/06/1896    04/15/1896    NULL                  NULL                


	25 rows selected.

	Current transaction has been committed.

	1 command(s) successfully processed.

	csql> SELECT SUM(n) FROM (SELECT gold FROM participant WHERE nation_code='KOR'
	csql> UNION ALL SELECT silver FROM participant WHERE nation_code='JPN') AS t(n);

	=== <Result of SELECT Command in Line 2> ===

		   sum(n)
	=============
			   82


	1 row selected.

	Current transaction has been committed.

	1 command(s) successfully processed.

	csql> ;exit

CUBRID Manager and Query Browser
--------------------------------

**Starting the CUBRID Manager Server**

If you want to use the CUBRID Manager client, you should first run the CUBRID Manager server. To run the server, you just need to execute **cubrid service start** once you have CUBRID installed in your system. For more information, see `Starting and Stopping CUBRID Manager Server <#admin_admin_service_manager_star_151>`_ .

**Starting the CUBRID Manager Client**

CUBRID Manager is an exclusive CUBRID database management tool that provides features to manage CUBRID and execute queries vai a GUI environment. It is called the CUBRID Manager client to distinguish it from the CUBRID Manager server located on the server side. It can be run only on the Java Runtime Environment (JRE) or the Java Development Kit 1.6 or later because it is written in Java.

The default user of CUBRID Manager is **admin** and its password is also **admin**. Please note that a CUBRID Manager user is different from a database user. A CUBRID Manager user can performs tasks such as starting/terminating one or more databases and managing brokers. A database user can performs tasks such as executing queries on specific databases. When you install a database for the first time, **PUBLIC** and **DBA** users are configured by default and no password is specified for either.

To download the CUBRID Manager client or get more information, click `http://www.cubrid.org/wiki_tools/entry/cubrid-manager <http://www.cubrid.org/wiki_tools/entry/cubrid-manager>`_
.

**Starting the CUBRID Query Browser**

CUBRID Query Browser is a light version of the CUBRID Manager client. It eliminates database management related features, and provides only query execution related features; with CUBRID Query Browser, you can execute query statements or execute a database definition language (DDL) by using menus.

A big difference between CUBRID Query Browser and the CUBRID Manager client is that the CUBRID Query Browser can be used regardless of whether the CUBRID Manager server is running. Therefore, it is not required to have a user account for CUBRID Manager when using CUBIRD Query Browser. However, you cannot start/terminate a database and monitor database/broker/HA in CUBRID Query Browser.

To download the CUBRID Query Browser program or get more information, click `http://www.cubrid.org/wiki_tools/entry/cubrid-query-browser <http://www.cubrid.org/wiki_tools/entry/cubrid-query-browser>`_ .

CUBRID Drivers
--------------

The drivers supported by CUBRID are as follows:

*   CUBRID JDBC driver (Installing and Configuring JDBC, Programming, API References, `Downloads <http://www.cubrid.org/?mid=downloads&item=jdbc_driver>`_ )

*   CUBRID CCI driver (CCI Overview, Programming, API References, `Downloads <http://www.cubrid.org?mid=downloads&item=cci_driver>`_ )

*   CUBRID PHP driver (Installing and Configuring PHP, Programming, API References, `Downloads <http://www.cubrid.org/?mid=downloads&item=php_driver&driver_type=phpdr>`_ )

*   CUBRID PDO driver (Installing and Configuring PDO, Programming, API References, `Downloads <http://www.cubrid.org/?mid=downloads&item=php_driver&driver_type=pdo>`_ )

*   CUBRID ODBC driver (Installing and Configuring ODBC, Programming, API References, `Downloads <http://www.cubrid.org/?mid=downloads&item=odbc_driver>`_ )

*   CUBRID OLE DB driver (Installing and Configuring OLE DB, Programming, API References, `Downloads <http://www.cubrid.org/?mid=downloads&item=oledb_driver>`_ )

*   CUBRID ADO.NET driver (Installing and Configuring ADO.NET, Programming, API References, `Downloads <http://www.cubrid.org/?mid=downloads&item=ado_dot_net_driver>`_ )

*   CUBRID Perl driver (Installing and Configuring Perl, API References, `Downloads <http://www.cubrid.org/?mid=downloads&item=perl_driver>`_ )

*   CUBRID Python driver (Installing and Configuring Python, Programming, API References, `Downloads <http://www.cubrid.org/?mid=downloads&item=python_driver>`_ )

*   CUBRID Ruby driver (Installing and Configuring Ruby, API References, `Downloads <http://www.cubrid.org/?mid=downloads&item=ruby_driver>`_ )

Among those of drivers, JDBC, ODBC, and CCI drivers are automatically downloaded while CUBRID is being installed. Thus, you do not have to download them manually.
