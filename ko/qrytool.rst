
:meta-keywords: cubrid query, csql interpreter, running sql, cubrid management tools, cubrid manager, query browser, migration toolkit
:meta-description: CSQL Interpreter, Management Tools, CUBRID Manager and Query Browser, Migrating schema/data with CUBRID Migration Toolkit.

질의 도구
=========

CSQL 인터프리터
---------------

CSQL 인터프리터는 CUBRID에서 명령어 방식으로 SQL 질의를 수행하고 수행 결과를 조회할 수 있는 프로그램이다. 입력된 SQL 문장과 그 결과는 나중에 사용하기 위해서 파일에 저장될 수도 있다. 자세한 내용은 :ref:`csql-intro` 및 :ref:`csql-exec-mode`\ 를 참고한다.

CUBRID는 CSQL 인터프리터 이외에도 편리한 GUI 방식의 "CUBRID 매니저" 제공하며, 이 도구들을 이용하여 CUBRID 매니저의 질의 편집기에서도 모든 SQL 문을 수행하고 결과를 조회할 수 있다. CUBRID 매니저와 쿼리 브라우저에 대한 자세한 내용은 :ref:`cm-cqb`\ 를 참고한다.

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
       ;EDIT   [format/fmt]        - invoke system editor [after formatter] with command buffer.
       ;LISt                       - display the content of command buffer.

       ;RUn                        - execute sql in command buffer.
       ;Xrun                       - execute sql in command buffer,
                                     and clear the command buffer.
       ;COMmit                     - commit the current transaction.
       ;ROllback                   - roll back the current transaction.
       ;AUtocommit [ON|OFF]        - enable/disable auto commit mode.
       ;REStart                    - reconnect to the current database in a CSQL session.
       ;CHeckpoint                 - execute the checkpoint(CSQL with --sysadm only).
       ;Killtran                   - check transaction status information or end a specific transaction.(CSQL with --sysadm only).

       ;SHELL_Cmd  [shell-cmd]     - set default shell, editor, print, pager and formatter
       ;EDITOR_Cmd [editor-cmd]      command to new one, or display the current
       ;PRINT_Cmd  [print-cmd]       one, respectively.
       ;PAger_cmd  [pager-cmd]
       ;FOrmatter_cmd [formatter-cmd]

       ;DATE                       - display the local time, date.
       ;DATAbase                   - display the name of database being accessed.
       ;SChema class-name          - display schema information of a class.
       ;TRIgger [`*'|trigger-name] - display trigger definition.
       ;Get system_parameter       - get the value of a system parameter.
       ;SET system_parameter=value - set the value of a system parameter.
       ;STring-width [width]       - set width that each column which is a string type is displayed.
       ;COLumn-width [name]=[width]- set width that a specific column is displayed.
       ;PLan [simple/detail/off]   - show query execution plan.
       ;Info <command>             - display internal information.
       ;TIme [ON/OFF]              - enable/disable to display the query
                                     execution time.
       ;LINe-output [ON/OFF]       - enable/disable to display each value in a line
       ;HISTORYList                - display list of the executed queries.
       ;HISTORYRead <history_num>  - read entry on the history number into command buffer.
       ;TRAce [ON/OFF] [text/json] - enable/disable sql auto trace.
       ;SIngleline [ON|OFF]        - enable/disable single-line mode.
       ;CONnect username [dbname | dbname@hostname]
                                   - connect to the current or other databases as a username.
       ;.Hist [ON/OFF]             - start/stop collecting statistics information in CSQL(available DBA only).
       ;.Clear_hist                - clear the CSQL statistics information in the buffer.
       ;.Dump_hist                 - display the CSQL statistics information in CSQL.
       ;.X_hist                    - display the CSQL statistics information in CSQL with statistics data initialized.
       ;HElp                       - display this help message.       

**CSQL에서 SQL 실행**

csql을 실행하고 난 후에는 csql> 프롬프트에서 원하는 SQL문을 입력해서 실행할 수 있다. 하나의 SQL 문은 세미콜론(;)으로 끝나도록 입력하며, 여러 개의 SQL문을 한 줄에 입력할 수도 있다. 세션 명령어는 ;help 명령으로 간단한 사용법을 찾아 볼 수 있으며 상세한 내용은 :ref:`csql-session-commands`\ 를 참고한다. ::

    % csql demodb
    
    csql> SELECT SUM(n) FROM (SELECT gold FROM participant WHERE nation_code='KOR'
    csql> UNION ALL SELECT silver FROM participant WHERE nation_code='JPN') AS t(n);

    === <Result of SELECT Command in Line 2> ===

           sum(n)
    =============
               82

    1 rows selected. (0.106504 sec) Committed.

    csql> ;exit

.. _cm-cqb:

관리 도구
=========

+--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+
|                          | 특징 요약                                                                   | 최신 파일 다운로드                                              |
+==========================+=============================================================================+=================================================================+
| CUBRID Manager           | SQL 실행 및 DB 운영을 위한 Java 클라이언트 도구이다                         | `CUBRID Manager Download                                        |
|                          |                                                                             | <http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Manager>`_           |
|                          | 1) JAVA 기반 관리 도구(JRE 1.6이상 요구)                                    |                                                                 |
|                          |                                                                             |                                                                 |
|                          | 2) 최초 다운로드 후 이후 버전 업데이트는 자동 실행                          |                                                                 |
|                          |                                                                             |                                                                 |
|                          | 3) 멀티 호스트 관리에 적합                                                  |                                                                 |
|                          |                                                                             |                                                                 |
|                          | 4) CUBRID Manager 서버를 통해 DB 접속                                       |                                                                 |
+--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+
| CUBRID Migration Toolkit | 소스 DB(MySQL, Oracle, CUBRID)에서 CUBRID로 데이터 및 스키마를 이전하는     | `CUBRID Migration Toolkit Download                              |
|                          | Java 기반 클라이언트 도구이다.                                              | <http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Migration_Toolkit>`_ |
|                          |                                                                             |                                                                 |
|                          | 1) JAVA 기반 관리 도구(JRE 1.6 이상 요구)                                   |                                                                 |
|                          |                                                                             |                                                                 |
|                          | 2) 최초 다운로드 후 이후 버전 업데이트는 자동 실행                          |                                                                 |
|                          |                                                                             |                                                                 |
|                          | 3) 다중 SQL문 실행 결과만 이전 가능,                                        |                                                                 |
|                          |    작업 시나리오 재사용 가능하여 배치 작업에 유리                           |                                                                 |
|                          |                                                                             |                                                                 |
|                          | 4) JDBC로 DB에 직접 접속                                                    |                                                                 |
+--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+

.. +--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+---------------------------------------------------------------------+
.. |                          | Summary of features                                                         | Downloads of the recent files                                   | Links to the latest documents                                       |
.. +==========================+=============================================================================+=================================================================+=====================================================================+
.. | CUBRID Manager           | Java client tool for SQL execution & DB operation.                          | `CUBRID Manager Download                                        | `CUBRID Manager Documents                                           |
.. |                          |                                                                             | <http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Manager>`_           | <http://www.cubrid.org/wiki_tools/entry/cubrid-manager>`_           |
.. |                          | 1) Java-based management tool (JRE 1.6 or higher required)                  |                                                                 |                                                                     |
.. |                          |                                                                             |                                                                 |                                                                     |
.. |                          | 2) Auto upgrade after the initial download                                  |                                                                 |                                                                     |
.. |                          |                                                                             |                                                                 |                                                                     |
.. |                          | 3) Useful to manage multiple hosts                                          |                                                                 |                                                                     |
.. |                          |                                                                             |                                                                 |                                                                     |
.. |                          | 4) DB access via CUBRID Manager server                                      |                                                                 |                                                                     |
.. +--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+---------------------------------------------------------------------+
.. | CUBRID Migration Toolkit | Java-based client tool to migrate schema and data from source DB            | `CUBRID Migration Toolkit Download                              | `CUBRID Migration Toolkit Documents                                 |
.. |                          | (MySQL, Oracle, CUBRID) to CUBRID.                                          | <http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Migration_Toolkit>`_ | <http://www.cubrid.org/wiki_tools/entry/cubrid-migration-toolkit>`_ |
.. |                          |                                                                             |                                                                 |                                                                     |
.. |                          | 1) Java-based management tool (JRE 1.6 or higher required)                  |                                                                 |                                                                     |
.. |                          |                                                                             |                                                                 |                                                                     |
.. |                          | 2) Auto upgrade after the initial download                                  |                                                                 |                                                                     |
.. |                          |                                                                             |                                                                 |                                                                     |
.. |                          | 3) Available migration only for multiple queries results,                   |                                                                 |                                                                     |
.. |                          |    the reuse of migration scenario; good to batch job                       |                                                                 |                                                                     |
.. |                          |                                                                             |                                                                 |                                                                     |
.. |                          | 4) Direct DB access with JDBC                                               |                                                                 |                                                                     |
.. +--------------------------+-----------------------------------------------------------------------------+-----------------------------------------------------------------+---------------------------------------------------------------------+


CUBRID 매니저로 SQL 실행하기
----------------------------

CUBRID 매니저는 별도로 다운로드 한 후 실행해야 하는 클라이언트 도구이며, JRE 혹은 JDK 1.6 이상 버전에서 실행되는 Java 애플리케이션이다.

#.  CUBRID 매니저 최신 파일을 다운로드한 후 설치한다. CUBRID 매니저는 CUBRID 엔진 버전 2008 R2.2 이상부터 호환된다. 또한, 자동 업데이트 기능을 지원하므로 주기적으로 최신 버전을 유지하는 것이 좋다. 
    (CUBRID FTP: http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Manager )

#.  서버에서 CUBRID Service를 시작한다. CUBRID 매니저 서버가 구동되어야 CUBRID 매니저 클라이언트가 접속할 수 있다. CUBRID 매니저 서버의 실행 및 설정에 대한 자세한 내용은 :ref:`cubrid-manager-server` 를 참고한다.

    ::

        C:\CUBRID>cubrid service start
        ++ cubrid service is running.
    
#.  CUBRID 매니저를 설치한 후 [파일 > 호스트 추가] 메뉴에서 호스트 정보를 등록한다. 호스트 등록 시에는 호스트 주소, 연결 포트(기본: 8001), CM 사용자 및 비밀번호를 입력해야 하며, 해당 서버의 엔진과 버전이 동일한 JDBC 드라이버를 설치해야 한다(자동 드라이버 검색/자동 업데이트 지원).

#.  왼쪽에 노드 트리에서 호스트를 선택하고 CM 사용자(=호스트 사용자) 인증을 수행한다. 기본 사용자 계정은 admin/admin이다.

#.  데이터베이스 노드에서 마우스 우클릭을 하여 새로운 데이터베이스를 생성하거나, 호스트 노드 하위에 있는 기존 데이터베이스를 선택하여 접속을 시도한다. 이때에는 DB 사용자 인증을 수행한다. 기본 사용자 이름은 dba이며 암호는 없다.
    
#.  접속한 DB에서 SQL을 실행하고, 결과를 확인한다. 왼쪽에는 호스트, 데이터베이스, 테이블 목록이 출력되고, 오른쪽에는 질의 편집기와 질의 결과 창이 있다. [SQL 실행 이력] 탭에서는 DB별로 실행 성공한 SQL 리스트를 재사용할 수 있으며, [질의 다중 실행] 탭에서 결과 비교를 위한 DB를 추가하여 여러 데이터베이스에서 결과값을 쉽게 비교할 수 있다.

    .. image:: /images/gs_manager_sql.png

.. 보다 자세한 정보는 http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual_kr 를 참고한다.

CUBRID 마이그레이션 툴킷으로 스키마/데이터 이전하기
---------------------------------------------------

CUBRID 마이그레이션 툴킷은 소스 데이터베이스(MySQL, Oracle, CUBRID)에서 타겟 데이터베이스(CUBRID)로 데이터 및 스키마를 이전하는 도구이다. 역시 JRE 혹은 JDK 1.6 이상 버전에서 실행되는 Java 애플리케이션이며, 별도로 다운받아야 한다. 
(CUBRID FTP: http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Migration_Toolkit )

DB를 CUBRID로 전환하는 경우, 장비를 이전하는 경우, 운영 DB로부터 일부 스키마와 일부 데이터를 이전하고자 하는 경우, CUBRID 버전 업그레이드를 하는 경우, 배치 작업을 수행하는 경우 유용하다. 주요 기능은 다음과 같다.

*   전체/일부 스키마 및 데이터 마이그레이션 지원
    
*   여러 개의 SQL을 실행하여 원하는 결과 데이터만 마이그레이션 가능
    
*   JDBC를 통한 온라인 마이그레이션 지원하여 운영 중단 없이 실행 가능
    
*   CSV, SQL, CUBRID loaddb 포맷으로 출력 후 오프라인 마이그레이션 가능
    
*   마이그레이션 실행 스크립트를 추출하여 타겟 서버에서 직접 실행 가능
    
*   마이그레이션 실행 스크립트를 재사용할 수 있어 배치 작업 시간 단축

.. image:: /images/gs_manager_migration.png

.. FIXME: 보다 자세한 정보는 http://www.cubrid.org/wiki_tools/entry/cubrid-migration-toolkit-manual 을 참고한다.

드라이버
========

CUBRID가 지원하는 드라이버는 다음과 같다.

*   :doc:`CUBRID JDBC driver <api/jdbc>` (`Downloads JDBC <http://ftp.cubrid.org/CUBRID_Drivers/JDBC_Driver/>`_)

*   :doc:`CUBRID CCI driver <api/cci>` (`Downloads CCI <http://ftp.cubrid.org/CUBRID_Drivers/CCI_Driver/>`_)

*   :doc:`CUBRID PHP driver <api/php>` (`Downloads PHP <http://ftp.cubrid.org/CUBRID_Drivers/PHP_Driver/>`_)

*   :doc:`CUBRID PDO driver <api/pdo>` (`Downloads PDO <http://ftp.cubrid.org/CUBRID_Drivers/PHP_Driver/PDO/>`_)

*   :doc:`CUBRID ODBC driver <api/odbc>` (`Downloads ODBC <http://ftp.cubrid.org/CUBRID_Drivers/ODBC_Driver/>`_)

*   :doc:`CUBRID OLE DB driver <api/oledb>` (`Downloads OLE DB <http://ftp.cubrid.org/CUBRID_Drivers/OLEDB_Driver/>`_)

*   :doc:`CUBRID ADO.NET driver <api/adodotnet>` (`Downloads ADO.NET <http://ftp.cubrid.org/CUBRID_Drivers/ADO.NET_Driver/>`_)

*   :doc:`CUBRID Perl driver <api/perl>` (`Downloads Perl <http://ftp.cubrid.org/CUBRID_Drivers/Perl_Driver/>`_)

*   :doc:`CUBRID Python driver <api/python>` (`Downloads Python <http://ftp.cubrid.org/CUBRID_Drivers/Python_Driver/>`_)

*   :doc:`CUBRID Ruby driver <api/ruby>` (`Downloads Ruby <http://ftp.cubrid.org/CUBRID_Drivers/Ruby_Driver/>`_)

*   :doc:`CUBRID Node.js driver <api/node_js>` (`Downloads Node.js <http://ftp.cubrid.org/CUBRID_Drivers/Node.JS_Driver/>`_)

위 드라이버 중 JDBC, CCI 드라이버는 CUBRID를 설치할 때 자동으로 다운로드되므로 따로 다운로드하지 않아도 된다.
