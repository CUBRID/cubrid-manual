질의 도구
=========

CSQL 인터프리터
---------------

CSQL 인터프리터는 CUBRID에서 명령어 방식으로 SQL 질의를 수행하고 수행 결과를 조회할 수 있는 프로그램이다. 입력된 SQL 문장과 그 결과는 나중에 사용하기 위해서 파일에 저장될 수도 있다. 자세한 내용은 `CSQL 인터프리터 소개 <#csql_csql_intro_htm>`_ 및 `CSQL 실행 <#csql_csql_exec_mode_htm>`_ 을 참고한다.

CUBRID는 CSQL 인터프리터 이외에도 편리한 GUI 방식의 "CUBRID 매니저"나 "CUBRID 쿼리 브라우저"를 제공하며, 이 도구들을 이용하여 CUBRID 매니저의 질의 편집기에서도 모든 SQL 문을 수행하고 결과를 조회할 수 있다. CUBRID 매니저와 쿼리 브라우저에 대한 자세한 내용은 `CUBRID 매니저, 쿼리 브라우저 사용 <#gs_gs_manager_htm>`_ 을 참고한다.

본 장에서는 Linux 환경에서 CSQL 인터프리터를 사용하는 경우를 설명한다.

**CSQL 인터프리터 시작**

CSQL 인터프리터는 셸에서 다음과 같이 시작할 수 있다. 처음 설치한 상태에서는 **PUBLIC** 과 **DBA** 사용자가 제공되며, 이들의 비밀번호는 설정되어 있지 않다. CSQL 인터프리터 실행 시 사용자를 지정하지 않으면 **PUBLIC** 으로 로그인된다. ::

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


**CSQL에서 SQL 실행**

csql을 실행하고 난 후에는 csql> 프롬프트에서 원하는 SQL문을 입력해서 실행할 수 있다. 하나의 SQL 문은 세미콜론(;)으로 끝나도록 입력하며, 여러 개의 SQL문을 한 줄에 입력할 수도 있다. 세션 명령어는 ;help 명령으로 간단한 사용법을 찾아 볼 수 있으며 상세한 내용은 `세션 명령어 <#csql_csql_sessioncommand_htm>`_ 를 참고한다. ::

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

CUBRID 매니저, 쿼리 브라우저 사용
---------------------------------

**CUBRID 매니저 서버 시작**

CUBRID 매니저 클라이언트를 사용하기 위해서는 CUBRID 매니저 서버가 반드시 구동되어야 하며, 이를 위해서는 CUBRID가 설치된 상태에서 **cubrid service start** 를 실행하면 된다. CUBRID 매니저 서버의 실행 및 설정에 대한 자세한 내용은 `CUBRID 매니저 구동 및 종료 <#admin_admin_service_manager_star_151>`_ 를 참고한다.

**CUBRID 매니저 클라이언트 시작**

CUBRID 매니저는 CUBRID 데이터베이스 관리 및 질의 기능을 GUI 환경에서 제공하는 CUBRID 데이터베이스 전용 관리 도구로, 데이터베이스 서버 쪽에 존재하는 매니저 서버와 구분하기 위해 CUBRID 매니저 클라이언트라고도 한다. CUBRID 매니저 클라이언트는 Java 응용 프로그램으로 JRE 혹은 JDK 1.6 이상 버전에서만 실행된다.

CUBRID 매니저는 기본 사용자로 **admin** 이 설정되어 있으며, 비밀번호는 **admin** 이다. CUBRID 매니저의 사용자와 데이터베이스의 사용자는 분리되어 있음을 주의한다. CUBRID 매니저 사용자는 여러 개의 데이터베이스들을 구동/종료하거나, 브로커를 관리하는 등의 작업을 수행할 수 있다. 데이터베이스 사용자는 특정 데이터베이스에 대해 질의를 수행할 수 있다. 데이터베이스를 처음 설치하면 기본 사용자로 **PUBLIC** 과 **DBA** 가 설정되어 있으며, 둘 다 처음에는 비밀번호가 설정되어 있지 않다.

CUBRID 매니저 클라이언트 프로그램을 다운로드하거나 자세한 정보를 얻으려면 `http://www.cubrid.org/wiki_tools/entry/cubrid-manager <http://www.cubrid.org/wiki_tools/entry/cubrid-manager>`_ 를 참조한다.

**CUBRID 쿼리 브라우저 시작**

CUBRID 쿼리 브라우저는 CUBRID 매니저의 기능을 경량화하여 데이터베이스 운영 기능을 제외한 질의 기능만 제공하는 도구로, 이를 이용하여 질의문을 실행하거나 DDL을 메뉴로 실행할 수 있다.

CUBRID 쿼리 브라우저와 CUBRID 매니저 클라이언트의 가장 큰 차이점은, CUBRID 쿼리 브라우저는 매니저 서버의 구동 여부와 관계없이 사용할 수 있다는 점이다. 따라서 CUBRID 쿼리 브라우저는 CUBRID 매니저용 사용자 계정이 필요없다. 하지만 CUBRID 쿼리 브라우저는 데이터베이스의 시작/ 종료, 데이터베이스/브로커/HA 모니터링 등을 수행할 수 없다.

CUBRID 쿼리 브라우저 프로그램을 다운로드하거나 자세한 정보를 얻으려면 `http://www.cubrid.org/wiki_tools/entry/cubrid-query-browser <http://www.cubrid.org/wiki_tools/entry/cubrid-query-browser>`_ 를 참조한다.

CUBRID 드라이버의 사용
----------------------

CUBRID가 지원하는 드라이버는 다음과 같다.

* CUBRID JDBC 드라이버(설치 및 설정, 프로그래밍, API 레퍼런스, `드라이버 다운로드 <http://www.cubrid.org/?mid=downloads&item=jdbc_driver>`_ )

* CUBRID CCI 드라이버(개요, 프로그래밍, API 레퍼런스,`드라이버 다운로드 <http://www.cubrid.org?mid=downloads&item=cci_driver>`_ )

* CUBRID PHP 드라이버(설치 및 설정, 프로그래밍, API 레퍼런스, `드라이버 다운로드 <http://www.cubrid.org/?mid=downloads&item=php_driver&driver_type=phpdr>`_ )

* CUBRID PDO 드라이버(설치 및 설정, 프로그래밍, API 레퍼런스, `드라이버 다운로드 <http://www.cubrid.org/?mid=downloads&item=php_driver&driver_type=pdo>`_ )

* CUBRID ODBC 드라이버(설치 및 설정, 프로그래밍, API 레퍼런스, `드라이버 다운로드 <http://www.cubrid.org/?mid=downloads&item=odbc_driver>`_ )

* CUBRID OLE DB 드라이버(설치 및 설정, 프로그래밍, API 레퍼런스, `드라이버 다운로드 <http://www.cubrid.org/?mid=downloads&item=oledb_driver>`_ )

* CUBRID ADO.NET 드라이버(설치 및 설정, 프로그래밍, API 레퍼런스, `드라이버 다운로드 <http://www.cubrid.org/?mid=downloads&item=ado_dot_net_driver>`_ )

* CUBRID Perl 드라이버(설치 및 설정, API 레퍼런스, `드라이버 다운로드 <http://www.cubrid.org/?mid=downloads&item=perl_driver>`_ )

* CUBRID Python 드라이버(설치 및 설정, 프로그래밍, API 레퍼런스, `드라이버 다운로드 <http://www.cubrid.org/?mid=downloads&item=python_driver>`_ )

* CUBRID Ruby 드라이버(설치 및 설정, API 레퍼런스, `드라이버 다운로드 <http://www.cubrid.org/?mid=downloads&item=ruby_driver>`_ )

위 드라이버 중 JDBC, ODBC, CCI 드라이버는 CUBRID를 설치할 때 자동으로 다운로드되므로 따로 다운로드하지 않아도 된다.
