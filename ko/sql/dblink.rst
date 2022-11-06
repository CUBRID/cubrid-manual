
:meta-keywords: cubrid dblink
:meta-description: CUBRID supports DBLink, which can connect to an external server and search information.

***********************
CUBRID DBLink
***********************

.. _dblink-introduction:

CUBRID DBLink 소개
==============================================

데이터베이스에서 정보를 조회하다 보면 종종 외부 데이타베이스의 정보 조회가 필요한 경우가 있다. 이렇게 외부 데이터베이스의 정보를 조회하기 위해서 CUBRID DBLink를 이용하면 타 데이터베이스의 정보를 조회할 수 있다.

CUBRID DBLink는 동일 기종인 CUBRID와 이기종인 Oracle, MySQL의 데이타베이스의 정보를 조회할 수 있도록 기능을 제공하고 있다.
외부 데이타베이스의 정보를 마치 하나의 데이터베이스에서 조회하는 것과 같은 효과를 발휘한다. 단 외부 데이타베이스를 여러 개 설정은 가능 하나, 정보를 조회할 때는 한 개의 타 데이타베이스의 정보만 조회가 가능하다.


.. _dblink-diagram:

CUBRID DBLink 구성도
==============================================

CUBRID DBLink는 동일기종 간에 DBLink와 이기종 간의 DBLink를 지원한다.

동일기종 간의 DBLink 구성도 
-----------------------------

동일기종의 외부 데이터베이스의 정보를 조회하기 위한 구성도를 보면 Database Server에서 CCI를 이용하여 동일기종의 Brokers에 접속하여 외부 데이터베이스의 정보를 조회할 수 있다.

.. image:: /images/dblink_homo.png

이기종 간의 DBLink 구성도
-----------------------------

이기종 데이터베이스의 정보를 조회하기 위한 구성도를 보면 게이트웨이를 통해서 이기종 데이터베이스의 정보를 조회할 수 있다. 
게이트웨이는 ODBC(Open DataBase Connectivity)를 이용하고 있다.

.. image:: /images/dblink_heter.png


.. _gateway-info:

DBLink를 위한 게이트웨이
==============================================

게이트웨이는 외부의 데이터베이스 서버에 연결할 수 있도록 중개하는 미들웨어로 Broker와 유사하다. 게이트웨이는 CUBRID Database Server에서 외부의 서버에 즉 Oracle/MySQL에 연결하여 외부 서버의 정보를 조회하여 CUBRID Database Server에 전달하는 역할을 한다.

게이트웨이를 포함하는 큐브리드 시스템은 아래 그림과 같이 cubrid_gateway, cub_gateway, cub_cas_cgw를 포함한 다중 계층 구조를 가진다.

.. image:: /images/gateway.png

cub_cas_cgw
----------------

cub_cas_cgw(CAS Gateway)는 CUBRID Database Server에서 외부의 Database의 연결을 요청하는 공용 서버 역할을 한다. 또한, cub_cas_cgw는 데이터베이스 서버의 클라이언트로 동작하여 CUBRID Database Server의 요청에 의해 외부 데이터베이스 서버와 연결을 제공한다. 서비스 풀(service pool) 내에서 구동되는 cub_cas_cgw의 개수는 cubrid_gateway.conf 설정 파일에 지정할 수 있으며, cub_gateway에 의해 동적으로 조정된다.

cub_gateway
----------------

cub_gateway는 CUBRID Database Server와 cub_cas_cgw 사이의 연결을 중계하는 기능을 수행한다. 즉, CUBRID Database Server가 접근을 요청하면, cub_gateway는 공유 메모리(shared memory)를 통해 cub_cas_cgw의 상태를 파악하여 접근 가능한 cub_cas_cgw에게 요청을 전달하고, 해당 cub_cas_cgw로부터 전달받은 요청에 대한 처리 결과를 CUBRID Database Server에게 반환한다.
또한, cub_gateway는 서비스 풀 내의 cub_cas_cgw 개수를 조정하여 서버 부하를 관리하고, cub_cas_cgw의 구동 상태를 모니터링 및 관리한다. 만약, CUBRID Database Server의 요청을 cub_cas_cgw 1에게 전달하였는데, 비정상적인 종료로 인해 cub_cas_cgw 1과의 연결이 실패하면, cub_gateway는 CUBRID Database Server에게 연결 실패에 관한 에러 메시지를 전송하고 cub_cas_cgw 1을 재구동한다. 새롭게 구동된 cub_cas_cgw 1은 정상적인 대기 상태가 되어, 새로운 응용 클라이언트의 요청에 의해 재연결된다.

공유 메모리
-----------------

공유 메모리에는 cub_cas_cgw의 상태 정보가 저장되며, cub_gateway는 공유 메모리에 저장된 cub_cas_cgw의 상태 정보를 참조하여 CUBRID Database Server와의 연결을 중개한다. 공유 메모리에 저장된 cub_cas_cgw의 상태 정보를 통해 시스템 관리자는 어떤 cub_cas_cgw가 현재 작업을 수행 중인지 확인할 수 있다.



게이트웨이 구동
---------------

게이트웨이를 구동하기 위하여 다음과 같이 입력한다. 

::

    $ cubrid gateway start

이미 게이트웨이가 구동 중이라면 다음과 같은 메시지가 출력된다.

::

    cubrid gateway start

게이트웨이 종료
---------------

게이트웨이가를 종료하기 위하여 다음과 같이 입력한다. 

::

    $ cubrid gateway stop

이미 게이트웨이가 종료되었다면 다음과 같은 메시지가 출력된다.

::

    $ cubrid gateway stop
    @ cubrid gateway stop
    ++ cubrid gateway is not running.

게이트웨이 재시작
-----------------------------

전체 게이트웨이를 재시작하기 위하여 다음과 같이 입력한다.

::

    $ cubrid gateway restart

.. _gateway-status-command:

게이트웨이 상태 확인
-------------------------------

**cubrid gateway status**  는 여러 옵션을 제공하여, 각 게이트웨이의 처리 완료된 작업 수, 처리 대기중인 작업 수를 포함한 게이트웨이 상태 정보를 확인할 수 있도록 한다. 
게이트웨이 상태는 브로커와 동일 하므로 :ref:`broker-status`\을 참조 한다.

::

    cubrid gateway status [options] [expr]


CUBRID 서비스 시작시 게이트웨이 함께 시작
----------------------------------------------

CUBRID 서비스 시작(**cubrid service start**) 시 *게이트웨* 를 같이 시작되게 하려면, **cubrid.conf** 파일의 **service** 파라메터에 *gateway* 를 설정한다. ::

    # cubrid.conf

    [service]

    service=server,broker,gateway,manager

    ...


CUBRID DBLINK 설정
==============================================

CUBRID DBLink를 사용하기 위한 설정은 동일기종 DBLink와 이기종 DBLink의 설정이 다르다.

동일기종 DBLink 설정
-----------------------

위의 동일기종 구성도를 보면 원격지 데이터베이스의 Broker에 연결을 해야 하므로 원격지 데이터베이스의 Broker 설정이 필요 하다. 
이 설정은 일반적인 Broker 설정과 동일하다.

이기종 DBLink 설정
------------------------

이기종(Oracle/MySQL)에 접속하기 위한 정보 설정이 필요 하며, 이기종 DBLink 설정을 하기 위해서는 cubrid_gataway.conf 와 unixODBC 설치, ODBC Driver 정보 설정이 필요 하다.


게이트웨이 설정 파일
------------------------------------------------

CUBRID 설치 시 생성되는 기본 게이트웨이 설정 파일인 cubrid_gataway.conf 에서 사용되는 파라미터는 브로커와 거의 동일 하며, 게이트웨이에서 반드시 변경해야 할 일부 파라미터가 기본으로 포함된다. 기본으로 포함되지 않는 파라미터의 설정값을 변경하기 원할 경우 직접 추가/편집해서 사용하면 된다. 다음은 설치 시 기본으로 제공되는 cubrid_gateway.conf 파일 내용이다.


::
    
 	[gateway]
	MASTER_SHM_ID           =50001
	ADMIN_LOG_FILE          =log/gateway/cubrid_gateway.log

	[%oracle_gateway]
	SERVICE                 =OFF
	SSL			=OFF
	APPL_SERVER             =CAS_CGW
	BROKER_PORT             =53000
	MIN_NUM_APPL_SERVER     =5
	MAX_NUM_APPL_SERVER     =40
	APPL_SERVER_SHM_ID      =53000
	LOG_DIR                 =log/gateway/sql_log
	ERROR_LOG_DIR           =log/gateway/error_log
	SQL_LOG                 =ON
	TIME_TO_KILL            =120
	SESSION_TIMEOUT         =300
	KEEP_CONNECTION         =AUTO
	CCI_DEFAULT_AUTOCOMMIT  =ON
	APPL_SERVER_MAX_SIZE    =256
	CGW_LINK_SERVER		=ORACLE
	CGW_LINK_SERVER_IP      =localhost
	CGW_LINK_SERVER_PORT    =1521
	CGW_LINK_ODBC_DRIVER_NAME   =Oracle_ODBC_Driver
	CGW_LINK_CONNECT_URL_PROPERTY       =


	[%mysql_gateway]
	SERVICE                 =OFF
	SSL			=OFF
	APPL_SERVER             =CAS_CGW
	BROKER_PORT             =56000
	MIN_NUM_APPL_SERVER     =5
	MAX_NUM_APPL_SERVER     =40
	APPL_SERVER_SHM_ID      =56000
	LOG_DIR                 =log/gateway/sql_log
	ERROR_LOG_DIR           =log/gateway/error_log
	SQL_LOG                 =ON
	TIME_TO_KILL            =120
	SESSION_TIMEOUT         =300
	KEEP_CONNECTION         =AUTO
	CCI_DEFAULT_AUTOCOMMIT  =ON
	APPL_SERVER_MAX_SIZE    =256
	CGW_LINK_SERVER		=MYSQL
	CGW_LINK_SERVER_IP      =localhost
	CGW_LINK_SERVER_PORT    =3306 
	CGW_LINK_ODBC_DRIVER_NAME   =MySQL_ODBC_Driver
	CGW_LINK_CONNECT_URL_PROPERTY       ="charset=utf8;PREFETCH=100;NO_CACHE=1"


게이트웨이 파라메터
------------------------

이기종 Server와 DBLink를 하기위해서 설정을 해야 하는 파라메터이다.
각각의 파라메터 의미는 이기종 Server에 따라 조금 다르다.

+-------------------------------+-------------+------------------------------------------------------------+
| Parameter Name                | Type        | Value                                                      |
+===============================+=============+============================================================+
| APPL_SERVER                   | string      |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
| CGW_LINK_SERVER               | string      |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
| CGW_LINK_SERVER_IP            | string      |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
| CGW_LINK_SERVER_PORT          | int         |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
| CGW_LINK_ODBC_DRIVER_NAME     | string      |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
| CGW_LINK_CONNECT_URL_PROPERTY | string      |                                                            |
+-------------------------------+-------------+------------------------------------------------------------+
     
  
**APPL_SERVER**

    **APPL_SERVER** 는 게이트웨이의 응용 서버 이름을 설정하는 부분으로 외부 서버와 연결을 하기 위해서는 CAS_CGW 로 설정해야 한다.

**CGW_LINK_SERVER**

    **CGW_LINK_SERVER** 는 CAS_CGW로 연결하여 사용할 외부 DBMS의 이름을 설정해야 한다. 현재 지원하는 데이타베이스는 Oracle, MySQL이다.

**CGW_LINK_SERVER_IP**

    **CGW_LINK_SERVER_IP** 는 CAS_CGW와 연결할 외부 DBMS의 IP 주소를 설정해야 한다.

.. note::
    
    *   Oracle의 경우, tnsnames.ora의 net_service_name을 이용하므로 해당 파라메터는 사용하지 않는다.
    *   자세한 내용은 :ref:`Oracle Database에 연결을 위한 연결정보 설정 <tnsnames-info>`\ 을 참고한다.
        

**CGW_LINK_SERVER_PORT**

    **CGW_LINK_SERVER_PORT** 는 CAS_CGW와 연결할 DBMS의 Port 번호를 설정해야 한다.
	
.. note::

    *   Oracle의 경우, tnsnames.ora의 net_service_name을 이용하므로 해당 파라메터는 사용하지 않는다.
    *   자세한 내용은 :ref:`Oracle Database에 연결을 위한 연결정보 설정 <tnsnames-info>`\ 을 참고한다.


**CGW_LINK_ODBC_DRIVER_NAME**

    **CGW_LINK_ODBC_DRIVER_NAME** 는 CAS_CGW와 연결할 때 외부 DBMS에서 제공하는 ODBC Driver 이름을 설정해야 한다.

.. note::
    
    *   Windows 경우, ODBC Driver를 설치한 경우, ODBC 데이터 원본 관리자를 통해 Driver 이름을 알 수 있다.
    *   Linux의 경우, odbcinit.ini에 직접 Driver 이름을 작성해야 한다.
    *   자세한 내용은 :ref:`ODBC Driver 정보 설정 <odbcdriver-info>`\ 을 참고한다.

**CGW_LINK_CONNECT_URL_PROPERTY**

    **CGW_LINK_CONNECT_URL_PROPERTY** 는 CAS_CGW와 외부 DBMS 연결할 때 Connection String에 사용되는 Property를 작성한다.

.. note::
    
    *   Property는 DBMS별로 각각 다르므로 아래의 사이트를 참조한다.
    *   Oracle : https://docs.oracle.com/cd/B19306_01/server.102/b15658/app_odbc.htm#UNXAR418
    *   MySQL : https://dev.mysql.com/doc/connector-odbc/en/connector-odbc-configuration-connection-parameters.html#codbc-dsn-option-flags


unixODBC 설치
------------------------------------------------

unixODBC 드라이버 관리자는 Linux 및 UNIX 운영 체제에서 ODBC 드라이버 와 함께 사용할 수 있는 오픈 소스 ODBC 드라이버 관리자이다.
게이트웨이에서는 ODBC를 사용하기 위해서 unixODBC를 설치해야 한다.

.. note::
	
	Winodws에서는 Microsoft® ODBC 데이터 원본 관리자 를 사용하면 되며, Windows에는 기본으로 설치가 되어 있다.

unixODBC 설치 방법

::
    
	$ wget http://www.unixodbc.org/unixODBC-2.3.9.tar.gz
	$ tar xvf unixODBC-2.3.9.tar.gz
	$ cd unixODBC-2.3.9
	$ ./configure
	$ make
	$ make install

.. note::

	unixODBC 드라이버 관리자 설치 방법은 아래의 url를 참고 바란다.
	unixODBC 홈페이지 : http://www.unixodbc.org/ 


.. _odbcdriver-info:

ODBC Driver 정보 설정
------------------------------------------------

unixODBC가 설치되어 있다면, ODBC Driver 정보를 등록해야 한다.
ODBC Driver 정보 등록은 odbcinst.ini에 작성한다.

아래의 내용은 MySQL, Oracle ODBC Driver 정보를 설정한 예제이다.

::
		
	[MySQL ODBC 8.0 Unicode Driver]
	Description = MySQL ODBC driver v8.0
	Driver=/usr/lib64/libmyodbc8w.so

	[Oracle 11g ODBC driver]
	Description = Oracle ODBC driver v11g
	Driver = /home/user/oracle/instantclient/libsqora.so.11.1
	

.. note::
    
        참고로, 위의 예제에서 드라이버 이름은 각각 "MySQL ODBC 8.0 Unicode Driver" 와 "Oracle 11g ODBC driver" 이다.


DBLink를 위한 Oracle 설정
==============================================
	
Oracle 환경설정
----------------------------

DBLink에서 Oracle을 사용 하기위해서는 Oracle Database 환경변수 설정과 Oracle Instant Client 설정, 게이트웨이 설정을 해야 한다.

**Oracle Database 환경변수를 설정**

Oracle database server 에 아래의 환경변수를 설정 해야 한다.

::
	
	export ORACLE_SID=XE
	export ORACLE_BASE=/u01/app/oracle
	export ORACLE_HOME=$ORACLE_BASE/product/11.2.0/xe
	export PATH=$ORACLE_HOME/bin:$PATH


* ORACLE_SID는 시스템 식별자이다.
* ORACLE_BASE은 오라클 기본 디렉토리 구조이다.
* ORACLE_HOME은 오라클 데이터베이스가 설치된 경로이다.	

**오라클 인스턴트 클라이언트 ODBC 설치**

Oracle Instant Client 다운로드 사이트에서 ODBC Package와 Basic Package 다운받아 동일한 디렉토리에 압축을 풉니다.

::
    
	unzip instantclient-basic-linux.x64-11.2.0.4.0.zip
	unzip instantclient-odbc-linux.x64-11.2.0.4.0.zip

Oracle Instant Client 다운로드 사이트: https://www.oracle.com/database/technologies/instant-client/downloads.html


**오라클 인스턴트 클라이언트 환경변수 설정**

export ORACLE_INSTANT_CLIENT=/home/user/oracle/instantclient  
export PATH=$ORACLE_INSTANT_CLIENT:$PATH
export LD_LIBRARY_PATH=$ORACLE_INSTANT_CLIENT:$LD_LIBRARY_PATH


.. _tnsnames-info:

**Oracle Database에 연결을 위한 연결정보 설정**

Oracle Database에 연결을 하기 위해서는 연결정보를 가지고 있는 tnsnames.ora 파일을 수정해야 한다.
아래의 기본 형식에 HOST, PORT, SERVICE_NAME 이 세 항목에 연결정보를 작성해야 한다.
연결정보를 작성한 tnsnames.ora 파일은 TNS_ADMIN 환경변수에서 디렉토리 경로를 설정해야 한다. 
TNS_ADMIN설정 방법은 "TNS_ADMIN 환경변수 설정" 참고한다.


tnsnames.ora 파일의 기본 형식

::
	
	net_service_name =
	  (DESCRIPTION=
		(ADDRESS = (PROTOCOL = TCP)(HOST = xxx.xxx.xxx.xxx)(PORT = 1521)
	  )
	  (CONNECT_DATA =
		(SERVICE_NAME=service_name)
	  )
	)


* net_service_name: 데이터베이스 연결을 위한 네트 서비스 이름이며, connection url의 db_name에 사용하는 이름이다.

* HOST: 데이터베이스에 연결하려는 IP 주소 또는 서버 이름이다.

* PORT: 연결에 필요한 포트입니다. 대부분의 경우 기본 포트는 1521이다.
* service_name: 연결하려는 데이터베이스의 이름입니다.


.. note::
    
        참고로, net_service_name 이 중복으로 작성이 되어도 에러가 발생되지 않는다. 하지만 원치않는 서버에 연결이 되므로, net_service_name 이 중복되지 않도록 주의해야 한다.

.. _tns_admin-info:

**TNS_ADMIN 환경변수 설정**

TNS_ADMIN는 tnsnames.ora 파일이 있는 디렉토리 경로를 가리킨다.
만약 /home/user/myconfigs 에 tnsnames.ora 파일이 있다면 아래와 같이 설정 할 수 있다.

::
	
	export TNS_ADMIN=/home/user/myconfigs



**Oracle을 위한 cubrid_gataway.conf 설정**

게이트웨이에서 oracle에 연결하기 위해서는 아래와 같이 몇 가지 설정이 필요 하다.
자세한 cubrid_gataway.conf 설정은 여기를 참고한다.

게이트웨이는 oracle에 연결하기 위해서 tnsnames.ora 의 정보를 이용하기 때문에 CGW_LINK_SERVER_IP, CGW_LINK_SERVER_PORT 는 작성하지 않아도 된다.

::
    
	APPL_SERVER              	=CAS_CGW
			.
			.
			.
	CGW_LINK_SERVER		        =ORACLE
	CGW_LINK_SERVER_IP      	=localhost
	CGW_LINK_SERVER_PORT    	=1521
	CGW_LINK_ODBC_DRIVER_NAME   =Oracle 12c ODBC driver
	CGW_LINK_CONNECT_URL_PROPERTY =


DBLink를 위한 MySQL 설정
=======================================

MySQL 환경설정
-------------------------
 
**MySQL ODBC Driver 설치**

게이트웨이에서 MySQL 연결을 하기위해서는 MySQL ODBC Driver가 필요 하다.
아래의 내용은 MYySQL ODBC Drvier 설치 방법이다.

MySQL Yum 저장소 를 사용하여 Connector/ODBC RPM 패키지를 제공합니다. 시스템의 리포지토리 목록에 MySQL Yum 저장소가 있어야 하며,
없는경우 MySQL Yum 저장소 다운로드 페이지( https://dev.mysql.com/downloads/repo/yum/ ) 에서 플랫폼에 대한 패키지를 선택하고 다운로드한다.

다운로드한 릴리스 패키지를 설치한다.

::
    
	$ sudo yum install mysql80-community-release-el6-{version-number}.noarch.rpm


Yum을 사용하여 저장소를 업데이트한다.

::
    
	$ su root
	$ yum update mysql-community-release

아래의 명령으로 Connector/ODBC 를 설치한다.

::
    
	$ yum install mysql-connector-odbc

자세한 설치 방법은 https://dev.mysql.com/doc/connector-odbc/en/connector-odbc-installation-binary-yum.html 을 참고한다.


**MySQL을 위한 cubrid_gataway.conf 설정**

게이트웨이에서 MySQL에 연결하기 위해서는 아래와 같이 몇 가지 설정이 필요 하다.
자세한 cubrid_gataway.conf 설정은 여기를 참고한다.

  
::
    
	APPL_SERVER                  =CAS_CGW
			.
			.
			.	
	CGW_LINK_SERVER		         =MYSQL
	CGW_LINK_SERVER_IP           =localhost
	CGW_LINK_SERVER_PORT         =3306 
	CGW_LINK_ODBC_DRIVER_NAME    =MySQL ODBC 8.0 Unicode Driver
	CGW_LINK_CONNECT_URL_PROPERTY ="charset=utf8;PREFETCH=100;NO_CACHE=1"



Cubrid DBLink 사용 방법
==============================================

동일기종의 Brokers와 이기종의 게이트웨이 설정을 했다면, 데이터베이스의 정보를 조회하기 위한 Query문 작성 방법에 대해서 알아본다.

데이터 조회를 위한 DBLINK Query문 작성 방법 두가지가 있다.

**첫번째**, FROM절에 DBLINK 구문을 작성하여 타 데이터베이스의 정보를 조회하는 방법
아래의 Query문은 IP 192.xxx.xxx.xxx의 타 데이터베이스의 remote_t 테이블 정보를 조회하는 Query문이다.

::
    
	SELECT * FROM DBLINK ('192.xxx.xxx.xxx:53000:testdb:user:password:','SELECT col1, col2 FROM remote_t') AS t(col1 int, col2 varchar(32));

.. note::
    
	Oracle의 경우 원격접속 정보중 db_name 항목에 tnsnames.ora의 net_service_name 을 넣어야 한다.
	만약 net_service_name이 ora_test 이라면 아래와 같이 작성하면 된다.
	SELECT * FROM DBLINK ('192.xxx.xxx.xxx:53000:ora_test:user:password:','SELECT col1, col2 FROM remote_t') AS t(col1 int, col2 varchar(32));

**두번째**, 위의 DBLINK Query를 보면 타 데이터베이스에 접속하기 위한 정보는 가장 기본적인 정보다. 그래서 Query를 작성할 때 마다 매번 작성해야 하는 번거로움과 사용자 정보(id, password) 가 외부로 노출될 우려가 있다.
이런 번거로움과 정보 보호를 위해 CREATE SERVER문을 이용하면, Query문 보다 간단하고, 사용자 정보 보호에 도움이 된다.



::
    
    CREATE SERVER remote_srv ( HOST='192.xxx.xxx.xxx', PORT=53000, DBNAME=testdb, USER=user, PASSWORD='password');
    SELECT * FROM DBLINK (remote_srv, 'SELECT col1 FROM remote_t') AS t(col1 int);




.. note::
    
        자세한 DBLink SQL 문법은 :doc:`/sql/query/select` 와 :doc:`/sql/schema/server_stmt` 을 참고한다.


제약사항
==============================================

*   CUBRID Hetergeneous DBLink는 utf-8만 지원한다.
*	게이트웨이에서는 Unicode ODBC Driver만 사용이 가능 하다.
*   1개 컬럼의 문자열 최대 길이는 16M까지만 지원한다.
*	Mysql의 경우, 대용량 테이블인 경우 cache를 사용하는 경우 게이트웨이 CAS의 메모리 사용량이 증가하므로 PREFETCH, NO_CACHE=1 사용을 권장한다.
*	ODBC 미지원 타입은 SQL_INTERVAL,SQL_GUID,SQL_BIT,SQL_BINARY,SQL_VARBINARY,SQL_LONGVARBINARY 이다.




