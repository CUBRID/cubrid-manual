************
CUBRID SHARD
************

Database Sharding
=================

**수평 분할**

수평 분할(horizontal partitioning)이란 스키마가 동일한 데이터를 행을 기준으로 두 개 이상의 테이블에 나누어 저장하는 디자인을 말한다. 예를 들어 ‘User Table’을 동일 스키마의 13세 미만의 유저를 저장하는 ‘User Table #0’과 13세 이상의 유저를 저장하는 ‘User Table #1’로 분할하여 사용할수 있다. 수평 분할로 인해 각 테이블의 데이터와 인덱스의 크기가 감소하고 작업 동시성이 늘어 성능 향상을 기대할 수 있다. 수평 분할은 일반적으로 하나의 테이터베이스 안에서 이루어진다. 수평 분할로 인해 각 테이블의 데이터와 인덱스의 크기가 감소하기 때문에 성능 향상을 기대할 수 있다.

.. image:: /images/image38.png

**database sharding**

database sharding은 물리적으로 다른 데이터베이스에 데이터를 수평 분할(horizontal partitioning) 방식으로 분산 저장하고 조회하는 방법을 말한다. 예를 들어 ‘User Table’이 여러 데이터베이스에 있을 때 13세 미만의 유저를 0번 데이터베이스에 13세 이상의 유저를 1번 데이터베이스에 저장되도록 하는 방식이다. database sharding은 성능상 이유뿐 아니라 하나의 데이터베이스 인스턴스에 넣을 수 없는 큰 데이터를 분산하여 처리하기 위해 사용된다.

분할된 각 데이터베이스를 shard 또는 database shard라고 부른다.

.. image:: /images/image39.png

**CUBRID SHARD**

CUBRID SHARD는 database sharding을 위한 미들웨어로, 다음과 같은 특징을 갖는다.

*   기존 응용의 변경을 최소화하기 위한 미들웨어 형태로서, 흔히 사용되는 JDBC(Java Database Connectivity)나 CUBRID C API인 CCI(CUBRID C Interface)를 이용하여 투명하게 sharding된 데이터에 접근할 수 있다.
*   힌트를 이용하여 실제 질의 수행할 shard를 선택하는 방식으로, 기존 사용하던 질의에 힌트를 추가하여 사용할 수 있다.
*   CUBRID 외에 MySQL을 backend shard DB로 구성할 수 있다.
*   일부 트랜잭션의 특성을 보장한다.

CUBRID SHARD 기본 용어
======================

다음은 앞으로 CUBRID SHARD를 설명하기 위해 사용되는 용어들로, 각 의미는 다음과 같다.

*   **shard DB** : 분할된 테이블과 데이터를 포함하고 있으며, 실제로 사용자 요청을 처리하는 데이터베이스

*   **shard metadata** : CUBRID SHARD의 동작을 위한 설정 정보. 요청된 질의를 분석하여 실제 질의를 수행할 shard DB를 선택하기 위한 정보 및 shard DB와의 데이터베이스 세션을 생성하기 위한 정보를 포함한다.

*   **shard key (column)** : sharding된 테이블에서 shard를 선택하기 위한 식별자로 사용되는 칼럼

*   **shard key data** : 질의 중 shard를 식별하기 위한 힌트에 해당하는 shard key의 값

*   **shard ID** : shard DB를 식별하기 위한 식별자

*   **shard proxy** : 사용자 질의에 포함된 힌트를 해석하고, 해석된 힌트와 shard metadata를 이용하여 실제 질의 처리할 shard DB로 요청을 전달하는 역할을 하는 CUBRID 미들웨어 프로세스

CUBRID SHARD 주요 기능
======================

미들웨어 구조
-------------

CUBRID SHARD는 응용 프로그램과 물리적 또는 논리적으로 분할된 shard의 중간에 위치하는 미들웨어(middleware)로서, 동시에 다수의 응용 프로그램과의 연결을 유지하며, 응용의 요청이 있는 경우 적절한 shard로 전달하여 처리하고 결과를 응용에 반환하는 기능을 수행한다.

.. image:: /images/image40.png

일반적으로 사용되는 JDBC(Java Database Connectivity)나 CUBRID C 인터페이스인 CCI(CUBRID C Interface)를 이용하여 CUBRID SHARD로 연결하여 응용의 요청을 처리할 수 있으며, 별도의 드라이버나 프레임워크가 필요 없기 때문에 기존 응용의 변경을 최소화할 수 있다.

CUBRID SHARD middleware는 broker/proxy/cas 세 개의 프로세스로 구성되며, 각 프로세스의 간략한 기능은 다음과 같다.

.. image:: /images/image41.png

*   **shard broker**

    *   JDBC/CCI 등 드라이버로부터의 최초 연결 요청을 수신하고, 수신된 연결 요청을 부하 분산 정책에 따라 shard proxy로 전달
    *   shard proxy 프로세스와 shard CAS 프로세스의 상태 감시 및 복구

*   **shard proxy**

    *   드라이버로부터의 사용자 요청을 shard CAS로 전달하고, 처리한 결과를 응용에 반환
    *   드라이버 및 CAS와의 연결 상태 관리 및 트랜잭션 처리

*   **shard CAS**

    *   분할된 shard DB와 연결을 생성하고, 그 연결을 이용하여 shard proxy로부터 수신한 사용자 요청(질의)를 처리
    *   트랜잭션 처리


shard SQL 힌트를 이용한 shard DB 선택
-------------------------------------

**shard SQL 힌트**

CUBRID SHARD는 SQL 힌트 구문에 포함된 힌트와 설정 정보를 이용하여, 응용으로부터 요청된 질의를 실제로 처리할 shard DB를 선택한다. 사용 가능한 SQL 힌트의 종류는 다음과 같다.

+----------------------+------------------------------------------------------------------------+
| SQL 힌트             | 설명                                                                   |
+======================+========================================================================+
| **/*+ shard_key */** | shard key 칼럼에 해당하는 바인드 변수 또는 리터럴 값의 위치를 지정하기 |
|                      | 위한 힌트                                                              |
+----------------------+------------------------------------------------------------------------+
| **/*+ shard_val(**   | 질의 내에 shard key에 해당하는 칼럼이 존재하지 않는 경우 힌트 내에     |
| *value*              | shard key를 명시적으로 지정하기 위한 힌트                              |
| **) */**             |                                                                        |
+----------------------+------------------------------------------------------------------------+
| **/*+ shard_id(**    | 사용자가 특정 shard DB를 지정하여 질의를 처리하고자 할 때 사용하는     |
| *shard_id*           | 힌트                                                                   |
| **) */**             |                                                                        |
+----------------------+------------------------------------------------------------------------+

설명을 위해 간략하게 용어를 정리하면 다음과 같다. 용어에 대한 보다 자세한 설명은 `CUBRID SHARD기본 용어 <#admin_admin_shard_glossary_htm>`_ 를 참고한다.

*   **shard key** : shard DB를 식별할 수 있는 칼럼. 일반적으로 shard DB 내의 모든 혹은 대부분의 테이블에 존재하는 칼럼으로서, DB 내에서 유일한 값을 갖는다.

*   **shard id** : shard를 논리적으로 구분할 수 있는 식별자. 예를 들어, 하나의 DB가 4개의 shard DB로 분할되면 4개의 shard id가 존재한다.

힌트와 설정 정보를 이용한 자세한 질의 처리 절차는 `shard SQL 힌트를 이용하여 질의가 수행되는 일반적인 절차 <#admin_admin_shard_feature_hint_h_8545>`_ 를 참고한다.

**shard_key 힌트**

**shard_key** 힌트는 바인드 변수나 리터럴 값의 위치를 지정하기 위한 힌트로서, 반드시 바인드 변수나 리터럴 값의 앞에 위치해야 한다.

예) 바인드 변수 위치 지정. 실행 시 바인딩되는 student_no 값에 해당하는 shard DB에서 질의를 수행.

.. code-block:: sql

	SELECT name FROM student WHERE student_no = /*+ shard_key */ ?

예) 리터럴 값 위치 지정. 실행 시 리터럴 값인 student_no가 123에 해당하는 shard DB에서 질의를 수행

.. code-block:: sql

	SELECT name FROM student WHERE student_no = /*+ shard_key */ 123

**shard_val 힌트**

**shard_val** 힌트는 질의 내에 shard DB를 식별할 수 있는 shard key 칼럼이 존재하지 않는 경우 사용하며, 실제 질의 처리 시 무시되는 shard key 칼럼을 **shard_val** 힌트의 값으로 설정한다. **shard_val** 힌트는 SQL 구문의 어느 곳에나 위치할 수 있다.

예) shard key가 student_no이나 질의 내에 포함되지 않은 경우. shard key인 student_no가 123에 해당하는 shard DB에서 질의를 수행

.. code-block:: sql

	SELECT age FROM student WHERE name =? /*+ shard_val(123) */

**shard_id 힌트**

**shard_id** 힌트는 shard key 칼럼의 값과 무관하게 사용자가 특정 shard를 지정하여 질의를 수행하고자 할 때 사용한다. **shard_id** 힌트는 SQL 구문의 어느 곳에나 위치할 수 있다.

예) shard DB #3 에서 질의를 수행해야 하는 경우. shard DB #3에서 age가 17보다 큰 학생을 조회

.. code-block:: sql

	SELECT * FROM student WHERE age > 17 /*+ shard_id(3) */

**shard SQL 힌트를 이용하여 질의가 수행되는 일반적인 절차**

**질의 수행**

다음은 사용자 질의 요청이 수행되는 과정이다.

.. image:: /images/image42.png

*   응용 프로그램은 JDBC 인터페이스를 통해 CUBRID SHARD로 질의 처리를 요청하며, 실제로 질의가 수행될 shard DB를 지정하기 위해 SQL 구문 내에 **shard_key** 힌트를 추가한다.

*   SQL 힌트는 SQL 구문 내에서 위 예에서와 마찬가지로 shard key로 설정된 칼럼의 바인드 또는 리터럴 값 바로 앞에 위치해야 한다.

바인드 변수에 설정된 shard SQL 힌트는 다음과 같다.

.. image:: /images/image43.png

리터럴 값에 지정된 shard SQL 힌트는 다음과 같다.

.. image:: /images/image44.png

**질의 분석 및 실제 요청을 처리할 shard DB 선택**

질의를 분석하고 실제로 요청을 처리할 shard DB를 선택하는 과정은 다음과 같다.

.. image:: /images/image45.png

*   사용자로부터 수신한 SQL 질의를 내부에서 처리하기 위한 형태로 다시 작성된다(query rewrite).

*   사용자가 요청한 SQL 구문과 힌트를 이용하여 실제 질의를 수행한 shard DB를 선택한다.

    *   바인드 변수에 SQL 힌트가 설정된 경우, execute 시 shard_key 바인드 변수에 대입된 값을 해시한 결과와 설정 정보를 이용하여 실제 질의가 수행될 shard DB를 선택한다.

    *   해시 함수는 사용자가 별도로 지정할 수 있으며, 지정하지 않은 경우 기본 내장된 해시 함수를 이용하여 shard_key 값을 해싱한다. 기본 내장된 해시 함수는 다음과 같다.

    *   shard_key가 정수인 경우 ::

		기본 해시 함수(shard_key) = shard_key mod SHARD_KEY_MODULAR 파라미터(기본값 256)
   
    *   shard_key가 문자열인 경우 ::

		기본 해시 함수(shard_key) = shard_key[0] mod  SHARD_KEY_MODULAR 파라미터(기본값 256)
	
.. note::

	shard_key 바인드 변수의 값이 100인 경우, "기본 hash 함수(shard_key) = 100 % 256 = 100"이므로, 설정에 의해 해시 결과 100에 해당하는 shard DB #1이 선택되며, 선택된 shard DB #1으로 사용자 요청을 전달하게 된다.

**질의 수행 결과 반환**

질의 수행 결과를 반환하는 과정은 다음과 같다.

.. image:: /images/image46.png

*   shard DB #1 에서 수행한 처리 결과를 수신하여, 요청한 응용으로 결과를 반환한다.

다양한 DBMS 사용 가능
---------------------

CUBRID SHARD는 CUBRID와 MySQL에서 사용할 수 있다.

**CUBRID SHARD with CUBRID**

아래의 그림은 3개의 CUBRID SHARD DB를 사용하는 경우 CUBRID SHARD 의 구조이다.

.. image:: /images/image47.png

**CUBRID SHARD with MySQL**

아래의 그림은 3개의 MySQL shard DB를 사용하는 경우 CUBRID SHARD 의 구조이다.

.. image:: /images/image48.png

**제약 사항**

하나의 CUBRID SHARD를 통해 다른 종류의 DBMS를 동시에 사용하는 것은 불가능하며, 필요하다면 각 DBMS별로 CUBRID SHARD 인스턴스를 분리하여 구성할 수는 있다.

트랜잭션 지원
-------------

**트랜잭션 처리**

CUBRID SHARD는 ACID 중 Atomicity(원자성)을 보장하기 위한 내부적인 처리 절차를 수행한다. 예를 들어, 트랜잭션 중 응용이 비정상 종료하는 등의 예외가 발생하면 해당 응용의 질의를 처리하던 shard DB로 롤백 요청을 전달하여 해당 트랜잭션 중 변경된 내용을 모두 무효화한다.

그 외 일반적인 트랜잭션의 특성인 ACID는 backend DBMS의 특성과 설정에 따라 보장된다.

**제약 사항**

2PC(2 Phase commit)는 불가능하며, 이 때문에 하나의 트랜잭션 중 여러 개의 shard DB로 질의를 수행하는 경우 에러 처리된다.

빠른 시작
=========

구성 예
-------

예로 설명될 CUBRID SHARD는 아래 그림과 같이 4개의 CUBRID SHARD DB로 구성되었으며, 응용은 JDBC 인터페이스를 사용하여 사용자 요청을 처리한다.

.. image:: /images/image49.png

**shard DB 및 사용자 계정 생성 후 시작**

위 구성의 예와 같이 각 shard DB 노드에서 shard DB 및 사용자 계정을 생성한 후 데이터베이스를 인스턴스를 시작한다.

*   shard DB 이름 : *shard1*
*   shard DB 사용자 계정 : *shard*
*   shard DB 사용자 비밀번호 : *shard123*

::

	sh> # CUBRID SHARD DB 생성
	sh> cubrid createdb shard1

	sh> # CUBRID SHARD 사용자 계정 생성
	sh> csql -S -u dba shard1 -c "create user shard password 'shard123'"

	sh> # CUBRID SHARD DB 시작
	sh> cubrid server start shard1


shard 설정 변경
---------------

**shard.conf**

기본 설정 파일인 **shard.conf** 를 아래와 같이 변경한다.

.. warning:: 포트 번호 및 공유 메모리 식별자는 현재 시스템에서 사용하지 않는 값으로 적절히 변경해야 한다.

::

	[shard]
	MASTER_SHM_ID           =45501
	ADMIN_LOG_FILE          =log/broker/cubrid_broker.log
	 
	[%shard1]
	SERVICE                 =ON
	BROKER_PORT             =45511
	MIN_NUM_APPL_SERVER     =1  
	MAX_NUM_APPL_SERVER     =1  
	APPL_SERVER_SHM_ID      =45511
	LOG_DIR                 =log/broker/sql_log
	ERROR_LOG_DIR           =log/broker/error_log
	SQL_LOG                 =ON
	TIME_TO_KILL            =120
	SESSION_TIMEOUT         =300
	KEEP_CONNECTION         =ON
	MAX_PREPARED_STMT_COUNT =1024
	SHARD_DB_NAME           =shard1
	SHARD_DB_USER           =shard
	SHARD_DB_PASSWORD       =shard123
	NUM_PROXY_MIN           =1  
	NUM_PROXY_MAX           =1  
	PROXY_LOG_FILE          =log/broker/proxy_log
	PROXY_LOG               =ALL
	MAX_CLIENT              =10
	METADATA_SHM_ID         =45591
	SHARD_CONNECTION_FILE   =shard_connection.txt
	SHARD_KEY_FILE          =shard_key.txt


CUBRID의 경우 **shard_connection.txt** 에 서버의 포트 번호를 별도로 설정하지 않고 **cubrid.conf** 설정 파일의 **cubrid_port_id** 파라미터를 사용하므로, **cubrid.conf** 의 **cubrid_port_id** 파라미터를 서버와 동일하게 설정한다. ::

	# TCP port id for the CUBRID programs (used by all clients).
	cubrid_port_id=41523

**shard_key.txt**

shard key 해시 값에 대한 shard DB 매핑 설정 파일인 **shard_key.txt** 파일을 아래와 같이 설정한다.

*   [%shard_key] : shard key 섹션 설정
*   기본 해시 함수에 의한 shard key 해시 결과가 0~63인 경우 shard #0 에서 질의 수행
*   기본 해시 함수에 의한 shard key 해시 결과가 64~127인 경우 shard #1 에서 질의 수행
*   기본 해시 함수에 의한 shard key 해시 결과가 128~191인 경우 shard #2 에서 질의 수행
*   기본 해시 함수에 의한 shard key 해시 결과가 192~255인 경우 shard #3 에서 질의 수행

::

	[%shard_key]
	#min    max     shard_id
	0       63      0
	64      127     1
	128     191     2
	192     255     3

**shard_connection.txt**

shard 구성 데이터베이스 설정 파일인 **shard_connection.txt** 파일을 아래와 같이 설정한다.

*   shard #0의 실제 데이터베이스 이름과 connection 정보
*   shard #1의 실제 데이터베이스 이름과 connection 정보
*   shard #2의 실제 데이터베이스 이름과 connection 정보
*   shard #3의 실제 데이터베이스 이름과 connection 정보

::

	# shard-id  real-db-name  connection-info
	#                         * cubrid : hostname, hostname, ...
	#                         * mysql  : hostname:port
	0           shard1        HostA
	1           shard1        HostB
	2           shard1        HostC
	3           shard1        HostD

서비스 시작 및 모니터링
-----------------------

**CUBRID SHARD 시작**

아래와 같이 CUBRID SHARD를 시작한다. ::

	sh> cubrid shard start
	@ cubrid shard start
	++ cubrid shard start: success

**CUBRID SHARD 상태 조회**

아래와 같이 CUBRID SHARD의 상태를 조회하여, 설정된 파라미터와 프로세스의 상태를 확인한다. ::

	sh> cubrid shard status
	@ cubrid shard status
	% shard1  - shard_cas [21265,45511] /home1/cubrid_user/SHARD/log/broker//shard1.err
	 JOB QUEUE:0, AUTO_ADD_APPL_SERVER:ON, SQL_LOG_MODE:ALL:100000, SLOW_LOG:ON
	 LONG_TRANSACTION_TIME:60.00, LONG_QUERY_TIME:60.00, SESSION_TIMEOUT:300
	 KEEP_CONNECTION:ON, ACCESS_MODE:RW, MAX_QUERY_TIMEOUT:0
	----------------------------------------------------------------
	PROXY_ID SHARD_ID   CAS_ID   PID   QPS   LQS PSIZE STATUS       
	----------------------------------------------------------------
		   1        0        1 21272     0     0 53292 IDLE         
		   1        1        1 21273     0     0 53292 IDLE         
		   1        2        1 21274     0     0 53292 IDLE         
		   1        3        1 21275     0     0 53292 IDLE
	 
	sh> cubrid shard status -f
	@ cubrid shard status
	% shard1  - shard_cas [21265,45511] /home1/cubrid_user/SHARD/log/broker//shard1.err
	 JOB QUEUE:0, AUTO_ADD_APPL_SERVER:ON, SQL_LOG_MODE:ALL:100000, SLOW_LOG:ON
	 LONG_TRANSACTION_TIME:60.00, LONG_QUERY_TIME:60.00, SESSION_TIMEOUT:300
	 KEEP_CONNECTION:ON, ACCESS_MODE:RW, MAX_QUERY_TIMEOUT:0
	----------------------------------------------------------------------------------------------------------------------------------------------------------
	PROXY_ID SHARD_ID   CAS_ID   PID   QPS   LQS PSIZE STATUS          LAST ACCESS TIME               DB             HOST   LAST CONNECT TIME    SQL_LOG_MODE
	----------------------------------------------------------------------------------------------------------------------------------------------------------
		   1        0        1 21272     0     0 53292 IDLE         2012/02/29 15:00:24    shard1@HostA           HostA 2012/02/29 15:00:25               -
		   1        1        1 21273     0     0 53292 IDLE         2012/02/29 15:00:24    shard1@HostB           HostB 2012/02/29 15:00:25               -
		   1        2        1 21274     0     0 53292 IDLE         2012/02/29 15:00:24    shard1@HostC           HostC 2012/02/29 15:00:25               -
		   1        3        1 21275     0     0 53292 IDLE         2012/02/29 15:00:24    shard1@HostD           HostD 2012/02/29 15:00:25               -
	   
응용 예제 프로그램 작성
-----------------------

간단한 Java 프로그램을 이용하여 CUBRID SHARD가 정상 동작함을 확인한다.

**예제 테이블 생성**

모든 shard DB에서 예제 프로그램을 위한 임시 테이블을 아래와 같이 생성한다. ::

	sh> csql -C -u shard -p 'shard123' shard1@localhost -c "create table student (s_no int, s_name varchar, s_age int, primary key(s_no))"	

**예제 프로그램 작성**

다음은 0~1023번의 학생 정보를 shard DB로 입력하는 예제 프로그램이다. 이전 절차에서 수정한 **shard.conf** 를 확인하여 주소/포트 및 사용자 정보를 connection url에 설정한다.

.. code-block:: java

	import java.sql.DriverManager;
	import java.sql.Connection;
	import java.sql.SQLException;
	import java.sql.Statement;
	import java.sql.ResultSet;
	import java.sql.ResultSetMetaData;
	import java.sql.PreparedStatement;
	import java.sql.Date;
	import java.sql.*;
	import cubrid.jdbc.driver.*;
	 
	public class TestInsert {
	 
			static  {
					try {
							Class.forName("cubrid.jdbc.driver.CUBRIDDriver");
					} catch (ClassNotFoundException e) {
							throw new RuntimeException(e);
					}
			}
	 
			public static void DoTest(int thread_id) throws SQLException {
					Connection connection = null;
	 
					try {
							connection = DriverManager.getConnection("jdbc:cubrid:localhost:45511:shard1:::?charset=utf8", "shard", "shard123");
							connection.setAutoCommit(false);
	 
							for (int i=0; i < 1024; i++) {
									String query = "INSERT INTO student VALUES (/*+ shard_key */ ?, ?, ?)";
									PreparedStatement query_stmt = connection.prepareStatement(query);
	 
									String name="name_" + i;
									query_stmt.setInt(1, i);
									query_stmt.setString(2, name);
									query_stmt.setInt(3, (i%64)+10);
	 
									query_stmt.executeUpdate();
									System.out.print(".");
	 
									query_stmt.close();
									connection.commit();
							}
	 
							connection.close();
					} catch(SQLException e) {
							System.out.print("exception occurs : " + e.getErrorCode() + " - " + e.getMessage());
							System.out.println();
							connection.close();
					}
			}
	 
	 
			/**
			 * @param args
			 */
			public static void main(String[] args) {
					// TODO Auto-generated method stub
	 
					try {
							DoTest(1);
					} catch(Exception e){
							e.printStackTrace();
					}
			}
	}

**예제 프로그램 수행**

위에서 작성한 예제 프로그램을 다음과 같이 수행한다. ::

	sh> javac -cp ".:$CUBRID/jdbc/cubrid_jdbc.jar" *.java
	sh> java -cp ".:$CUBRID/jdbc/cubrid_jdbc.jar" TestInsert

**결과 확인**

각 shard DB에서 질의를 수행하여 의도한 대로 분할된 정보가 정확하게 입력되었는지 확인한다.

*   shard #0 ::

	sh> csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
	 
	=== <Result of SELECT Command in Line 1> ===
	 
			 s_no  s_name                      s_age
	================================================
				0  'name_0'                       10
				1  'name_1'                       11
				2  'name_2'                       12
				3  'name_3'                       13
				...

*   shard #1 ::

	sh> $ csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
	 
	=== <Result of SELECT Command in Line 1> ===
	 
			 s_no  s_name                      s_age
	================================================
			   64  'name_64'                      10
			   65  'name_65'                      11
			   66  'name_66'                      12
			   67  'name_67'                      13  
			   ...

*   shard #2 ::

	sh> $ csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
	 
	=== <Result of SELECT Command in Line 1> ===
	 
			 s_no  s_name                      s_age
	================================================
	128  'name_128'                     10
	129  'name_129'                     11
	130  'name_130'                     12
	131  'name_131'                     13
	...

*   shard #3 ::

	sh> $ csql -C -u shard -p 'shard123' shard1@localhost -c "select * from student order by s_no"
	 
	=== <Result of SELECT Command in Line 1> ===
	 
			 s_no  s_name                      s_age
	================================================
	192  'name_192'                     10
	193  'name_193'                     11
	194  'name_194'                     12
	195  'name_195'                     13
	...


구성 및 설정
============

구성
----

CUBRID SHARD는 미들웨어로서 아래의 그림과 같이 shard broker, shard proxy, shard CAS 프로세스로 구성된다.

.. image:: /images/image50.png

CUBRID SHARD의 모든 프로세스의 실행에 필요한 기본적인 설정은 **shard.conf** 라는 파일을 이용하며, 이 설정 파일은 **$CUBRID/conf** 디렉터리에 위치한다.

기본 설정 파일 shard.conf
-------------------------

**shard.conf** 는 CUBRID SHARD의 기본 설정 파일로서, 기존 CUBRID 의 Broker/CAS의 설정 파일인 **cubrid_broker.conf** 와 형식과 내용 면에서 매우 유사하다.

**shard.conf** 는 **cubrid_broker.conf** 의 파라미터 설정 내용을 모두 동일하게 포함하고 있으며, 이 문서에서는 **shard.conf** 에서 추가된 내용만을 설명한다. **cubrid_broker.conf** 에 대한 자세한 내용은 "성능 튜닝"의 `브로커 설정 <#pm_pm_broker_setting_htm>`_ 을 참조한다.

+-------------------------------+--------+----------------------+-----------+
| 파라미터 이름                 | 타입   | 기본값               | 동적 변경 |
+===============================+========+======================+===========+
| IGNORE_SHARD_HINT             | string | OFF                  |           |
+-------------------------------+--------+----------------------+-----------+
| MIN_NUM_PROXY                 | int    | 1                    |           |
+-------------------------------+--------+----------------------+-----------+
| MAX_NUM_PROXY                 | int    | 1                    |           |
+-------------------------------+--------+----------------------+-----------+
| PROXY_LOG                     | string | ERROR                | 가능      |
+-------------------------------+--------+----------------------+-----------+
| PROXY_LOG_DIR                 | string | log/broker/proxy_log |           |
+-------------------------------+--------+----------------------+-----------+
| PROXY_LOG_MAX_SIZE            | int    | 100000               | 가능      |
+-------------------------------+--------+----------------------+-----------+
| PROXY_MAX_PREPARED_STMT_COUNT | int    | 2000                 |           |
+-------------------------------+--------+----------------------+-----------+
| MAX_CLIENT                    | int    | 10                   |           |
+-------------------------------+--------+----------------------+-----------+
| METADATA_SHM_ID               | int    | -                    |           |
+-------------------------------+--------+----------------------+-----------+
| SHARD_CONNECTION_FILE         | string | shard_connection.txt |           |
+-------------------------------+--------+----------------------+-----------+
| SHARD_DB_NAME                 | string | -                    | 가능      |
+-------------------------------+--------+----------------------+-----------+
| SHARD_DB_USER                 | string | -                    | 가능      |
+-------------------------------+--------+----------------------+-----------+
| SHARD_DB_PASSWORD             | string | -                    | 가능      |
+-------------------------------+--------+----------------------+-----------+
| SHARD_KEY_FILE                | string | shard_key.txt        |           |
+-------------------------------+--------+----------------------+-----------+
| SHARD_KEY_MODULAR             | int    | 256                  |           |
+-------------------------------+--------+----------------------+-----------+
| SHARD_KEY_LIBRARY_NAME        | string | -                    |           |
+-------------------------------+--------+----------------------+-----------+
| SHARD_KEY_FUNCTION_NAME       | string | -                    |           |
+-------------------------------+--------+----------------------+-----------+

*   **SHARD_DB_NAME** : shard DB의 이름으로서 응용의 연결 요청이 유효한지 검사하는 데에도 사용된다. 동적으로 값을 변경하면 변경된 값은 CAS가 데이터베이스에 다시 접속할 때 적용된다. **cubrid shard reset** 으로 강제로 재접속할 수 있다.

*   **SHARD_DB_USER** : backend shard DB의 사용자 이름으로서, shard CAS 프로세스에서 backend DBMS와 연결을 수행하는데 사용되며, 응용의 연결 요청이 유효한지 검사하는 데에도 사용된다. 모든 shard DB의 사용자 이름은 동일해야 한다. 동적으로 값을 변경하면 변경된 값은 CAS가 데이터베이스에 다시 접속할 때 적용된다. **cubrid shard reset** 으로 강제로 재접속할 수 있다.

*   **SHARD_DB_PASSWORD** : backend shard DB의 사용자 비밀번호로서, shard CAS 프로세스에서 backend DBMS와 연결을 수행하는데 사용되며, 응용의 연결 요청이 유효한지 검사하는 데에도 사용된다. 모든 shard DB의 사용자 비밀번호는 동일해야 한다. 동적으로 값을 변경하면 변경된 값은 CAS가 데이터베이스에 다시 접속할 때 적용된다. **cubrid shard reset** 으로 강제로 재접속할 수 있다.

*   **MIN_NUM_PROXY** : shard proxy 프로세스의 최소 개수

*   **MAX_NUM_PROXY** : shard proxy 프로세스의 최대 개수

*   **PROXY_LOG_DIR** : shard proxy 로그를 저장할 디렉터리 경로

*   **PROXY_LOG** : shard proxy 로그 레벨로서 다음의 값 중 하나로 설정 가능하다.

    *   **ALL** : 모든 로그 기록
    *   **ON**  : 모든 로그 기록
    *   **SHARD** : shard DB 선택과 처리에 대한 로그 기록
    *   **SCHEDULE** : 작업 할당에 대한 로그 기록
    *   **NOTICE** : 주요한 알림에 대한 로그 기록
    *   **TIMEOUT** : 임계 시간 초과에 대한 로그 기록
    *   **ERROR** : 에러 로그 기록
    *   **NONE** : 로그 기록하지 않음
    *   **OFF** : 로그 기록하지 않음

*   **PROXY_MAX_PREPARED_STMT_COUNT** : shard proxy가 관리하는 statement pool의 최대 크기

*   **MAX_CLIENT** : shard proxy로 동시에 연결 가능한 응용의 수

*   **METADATA_SHM_ID** : shard 메타데이터를 저장할 공유 메모리 식별자

*   **SHARD_CONNECTION_FILE** : shard connection 설정 파일의 경로. shard connection 설정 파일은 **$CUBRID/conf** 내에 위치해야 한다. 자세한 설명은 `shard 연결 설정 파일 <#admin_admin_shard_conf_meta_htm__2489>`_ 을 참고한다. :

*   **SHARD_KEY_FILE** : shard key 설정 정보 파일의 경로. shard key 설정 파일은 **$CUBRID/conf** 내에 위치해야 한다. 자세한 설명은 `shard key 설정 파일 <#admin_admin_shard_conf_meta_htm__8339>`_ 을 참고한다. :

*   **SHARD_KEY_MODULAR** : 내장된 shard key 해시 함수 결과의 범위를 지정하기 위한 파라미터로서, 기본 shard key 해시 함수의 결과는 shard_key(정수형) % SHARD_KEY_MODULAR이다. 관련된 내용은 `shard key 설정 파일 <#admin_admin_shard_conf_meta_htm__8339>`_ 과 `사용자 정의 해시 함수 설정 <#admin_admin_shard_conf_hash_htm>`_ 을 참고한다.

*   **SHARD_KEY_LIBRARY_NAME** : shard key에 대한 사용자 해시 함수를 지정하기 위해 실행 시간에 로딩 가능한 라이브러리 경로를 지정한다. **SHARD_KEY_LIBRARY_NAME** 파라미터가 설정된 경우 반드시 **SHARD_KEY_FUNCTION_NAME** 파라미터도 설정되어야 한다. 자세한 내용은 `사용자 정의 해시 함수 설정 <#admin_admin_shard_conf_hash_htm>`_ 을 참고한다.

*   **SHARD_KEY_FUNCTION_NAME** : shard key에 대한 사용자 해시 함수의 이름을 지정하기 위한 파라미터이다. 자세한 내용은 `사용자 정의 해시 함수 설정 <#admin_admin_shard_conf_hash_htm>`_ 를 참고한다.

*   **PROXY_LOG_MAX_SIZE** : shard proxy 로그 파일의 최대크기로 kbyte 단위이다. 최대 1,000,000까지 설정할 수 있다.

*   **IGNORE_SHARD_HINT** : 이 값이 ON이면 특정 shard로 연결하기 위해 제공되는 힌트가 무시되고, 정해진 규칙에 따라 접속할 데이터베이스를 선택한다. 기본값은 **OFF** 이다. 모든 데이터베이스가 같은 데이터로 복제되어 있는 상태에서 읽기 부하를 자동으로 로드 밸런싱하여 처리하고자 할 때 사용할 수 있는 방식이다. 예를 들어 응용 프로그램의 부하를 여러 개의 복제 노드 중 하나에 접속하고자 할 때 특정 shard 하나의 연결만 제공하면 어느 노드(데이터베이스)에 연결할지는 shard proxy가 자동으로 결정한다.

shard 메타데이터 설정
---------------------

CUBRID SHARD는 기본 설정 파일인 **shard.conf** 외에, 실제 shard DB와의 연결을 수행하기 위한 shard 연결 설정 파일과 shard key에 대한 설정 파일이 존재한다.

**shard 연결 설정 파일(SHARD_CONNECTION_FILE)**

CUBRID SHARD는 시작 시 기본 설정 파일인 **shard.conf** 의 **SHARD_CONNECTION_FILE** 파라미터에 지정된 shard 연결 설정 파일을 로딩하여 backend shard DB와의 연결을 수행한다.

**shard.conf** 에 **SHARD_CONNECTION_FILE** 을 별도로 지정하지 않은 경우에는 기본값인 **shard_connection.txt** 파일을 로딩한다.

**형식**

shard 연결 설정 파일의 기본적인 예와 형식은 아래와 같다. ::

	#
	# shard-id      real-db-name    connection-info
	#                               * cubrid : hostname, hostname, ...
	#                               * mysql  : hostname:port
	 
	# CUBRID
	0               shard1          HostA  
	1               shard1          HostB
	2               shard1          HostC
	3               shard1          HostD
	 
	# mysql
	#0              shard1         HostA:3306
	#1              shard1         HostB:3306
	#2              shard1         HostC:3306
	#3              shard1         HostD:3306

.. note:: 일반적인 CUBRID 설정과 마찬가지로 # 이후 내용은 주석으로 처리된다.

**CUBRID**

backend shard DB가 CUBRID인 경우 연결 설정 파일의 형식은 다음과 같다. ::

	# CUBRID
	# shard-id      real-db-name            connection-info
	# shard 식별자( >0 )        각 backend shard DB 의 실제 이름    호스트 이름
	 
	0           shard_db_1          host1
	1           shard_db_2          host2
	2           shard_db_3          host3
	3           shard_db_4          host4

CUBRID의 경우 별도의 backend shard DB의 포트 번호를 위 설정 파일에 지정하지 않고, CUBRID의 기본 설정 파일인 **cubrid.conf** 에 **CUBRID_PORT_ID** 파라미터를 사용한다. **cubrid.conf** 파일은 기본적으로 **$CUBRID/conf** 디렉터리에 위치한다. ::

	$ vi cubrid.conf

	# TCP port id for the CUBRID programs (used by all clients).
	cubrid_port_id=41523

**MySQL**

backend shard DB가 MySQL인 경우 연결 설정 파일의 형식은 다음과 같다. ::

	# mysql
	# shard-id      real-db-name            connection-info
	# shard 식별자( >0 )        각 backend shard DB 의 실제 이름    호스트 이름:포트 번호
	 
	0           shard_db_1          host1:1234
	1           shard_db_2          host2:1234
	2           shard_db_3          host3:1234
	3           shard_db_4          host4:1234


**shard key 설정 파일(SHARD_KEY_FILE)**

CUBRID SHARD는 시작 시 기본 설정 파일인 **shard.conf** 의 **SHARD_KEY_FILE** 파라미터에 지정된 shard key 설정 파일을 로딩하여 사용자 요청을 어떤 backend shard DB에서 처리해야 할지 결정하는 데 사용한다.

**shard.conf** 에 **SHARD_KEY_FILE** 을 별도로 지정하지 않은 경우에는 기본값인 **shard_key.txt** 파일을 로딩한다.

**형식**

shard key 설정 파일의 예와 형식은 다음과 같다. ::

	[%student_no]
	#min    max     shard_id
	0       31      0   
	32      63      1   
	64      95      2   
	96      127     3   
	128     159     0
	160     191     1
	192     223     2
	224     255     3
	 
	#[%another_key_column]
	#min    max     shard_id
	#0      127     0   
	#128    255     1

*   [%shard_key_name] : shard key의 이름을 지정
*   min : shard key 해시 결과의 최소값 범위
*   max : shard key 해시 결과의 최대 범위
*   shard_id : shard 식별자

.. note:: 일반적인 CUBRID 설정과 마찬가지로 # 이후 내용은 주석으로 처리된다.

**주의 사항**

*   shard key의 min은 항상 0부터 시작해야 한다.
*   max는 최대 255까지 설정해야 한다.
*   min~max 사이에는 빈 값이 존재하면 안 된다.
*   내장 해시 함수를 사용하는 경우 **SHARD_KEY_MODULAR** 파라미터 값을 초과할 수 없다.
*   shard key 해시 결과는 0 ~ (**SHARD_KEY_MODULAR** - 1)의 범위에 반드시 포함되어야 한다.

사용자 정의 해시 함수 설정
--------------------------

CUBRID SHARD는 질의를 수행할 shard를 선택하기 위해 shard key를 해싱한 결과와 메타데이터 설정 정보를 이용한다. 이를 위해 기본 내장된 해시 함수를 사용하거나, 또는 사용자가 별도로 해시 함수를 정의할 수 있다.

**내장된 기본 해시 함수**

**shard.conf** 의 **SHARD_KEY_LIBRARY_NAME**, **SHARD_KEY_FUNCTION_NAME** 파라미터를 설정하지 않는 경우 기본 내장된 해시 함수를 이용하여 shard key를 해시하며, 기본 해시 함수의 내용은 아래와 같다.

*   shard_key가 정수인 경우 ::

	기본 해시 함수(shard_key) = shard_key mod SHARD_KEY_MODULAR 파라미터(기본값 256)

*   shard_key가 문자열인 경우 ::

	기본 해시 함수(shard_key) = shard_key[0] mod SHARD_KEY_MODULAR 파라미터(기본값 256)

**사용자 해시 함수 설정**

CUBRID SHARD는 기본 내장된 해시 함수 외에 사용자 정의 해시 함수를 이용하여 질의에 포함된 shard key를 해싱할 수 있다.

**라이브러리 구현 및 생성**

사용자 정의 해시 함수는 실행 시간에 로딩 가능한 **.so** 형태의 라이브러리로 구현되어야 하며 프로토타입은 아래와 같다.

.. code-block:: c

	94 /*
	95    return value :
	96         success - shard key id(>0)
	97         fail    - invalid argument(ERROR_ON_ARGUMENT), shard key id make fail(ERROR_ON_MAKE_SHARD_KEY)
	98    type         : shard key value type
	99    val          : shard key value
	100 */
	101 typedef int (*FN_GET_SHARD_KEY) (const char *shard_key, T_SHARD_U_TYPE type,
	102                                    const void *val, int val_size);

*   해시 함수의 반환 값은 **shard_key.txt** 설정 파일의 해시 결과 범위에 반드시 포함되어야 한다.
*   라이브러리를 빌드하기 위해서는 반드시 **$CUBRID/include/shard_key.h** 파일을 include해야 한다. 이 파일에서 반환 가능한 에러 코드 등 자세한 내용도 확인할 수 있다.

**shard.conf 설정 파일 변경**

생성한 사용자 정의 해시 함수를 반영하기 위해서는 **shard.conf** 의 **SHARD_KEY_LIBRARY_NAME**, **SHARD_KEY_FUNCTION_NAME** 파라미터를 구현 내용에 맞도록 설정해야 한다.

*   **SHARD_KEY_LIBRARY_NAME** : 사용자 정의 해시 라이브러리의 (절대) 경로
*   **SHARD_KEY_FUNCTION_NAME** : 사용자 정의 해시 함수의 이름

**예제**

다음은 사용자 정의 해시 함수를 사용한 예이다. 먼저 **shard_key.txt** 설정 파일을 확인한다. ::

	[%student_no]
	#min    max     shard_id
	0       31      0   
	32      63      1   
	64      95      2   
	96      127     3   
	128     159     0
	160     191     1
	192     223     2
	224     255     3

사용자 지정 해시 함수를 설정하기 위해서는 실행 시간에 로딩 가능한 **.so** 형태의 공유 라이브러리를 먼저 구현해야 한다. 해시 함수의 결과는 이전 과정에서 확인한 **shard_key.txt** 설정 파일에 정의된 해시 결과의 범위 안에 포함되는 값이어야 한다. 다음은 간단한 구현 예이다.

*   shard_key가 정수인 경우

    *   shard_key가 홀수인 경우 shard #0을 선택
    *   shard_key가 짝수인 경우 shard #1을 선택

*   shard_key가 문자열인 경우

    *   shard_key 문자열이 'a', 'A'로 시작되는 경우 shard #0을 선택
	*   shard_key 문자열이 'b', 'B'로 시작되는 경우 shard #1을 선택
    *   shard_key 문자열이 'c', 'C'로 시작되는 경우 shard #2를 선택
	*   shard_key 문자열이 'd', 'D'로 시작되는 경우 shard #3을 선택

.. code-block:: c
	
	// <shard_key_udf.c>
	 
	1 #include <string.h>
	2 #include <stdio.h>
	3 #include <unistd.h>
	4 #include "shard_key.h"
	5
	6 int
	7 fn_shard_key_udf (const char *shard_key, T_SHARD_U_TYPE type,
	8                   const void *value, int value_len)
	9 {
	10   unsigned int ival;
	11   unsigned char c;
	12
	13   if (value == NULL)
	14     {
	15       return ERROR_ON_ARGUMENT;
	16     }
	17
	18   switch (type)
	19     {
	20     case SHARD_U_TYPE_INT:
	21       ival = (unsigned int) (*(unsigned int *) value);
	22       if (ival % 2)
	23         {
	24           return 32;            // shard #1
	25         }
	26       else
	27         {
	28           return 0;             // shard #0
	29         }
	30       break;
	31
	32     case SHARD_U_TYPE_STRING:
	33       c = (unsigned char) (((unsigned char *) value)[0]);
	34       switch (c)
	36         case 'a':
	37         case 'A':
	38           return 0;             // shard #0
	39         case 'b':
	40         case 'B':
	41           return 32;            // shard #1
	42         case 'c':
	43         case 'C':
	44           return 64;            // shard #2
	45         case 'd':
	46         case 'D':
	47           return 96;            // shard #3
	48         default:
	49           return ERROR_ON_ARGUMENT;
	50         }
	51
	52       break;
	53
	54     default:
	55       return ERROR_ON_ARGUMENT;
	56     }
	57   return ERROR_ON_MAKE_SHARD_KEY;
	58 }

사용자 지정 해시 함수를 공유 라이브러리 형태로 빌드한다. 다음은 해시 함수 빌드를 위한 Makefile의 예이다. ::

	# Makefile
	 
	CC = gcc
	LIBS = $(LIB_FLAG)
	CFLAGS = $(CFLAGS_COMMON) -fPIC -I$(CUBRID)/include –I$(CUBRID_SRC)/src/broker
	 
	SHARD_CC = gcc -g -shared -Wl,-soname,shard_key_udf.so
	SHARD_KEY_UDF_OBJS = shard_key_udf.o
	 
	all:$(SHARD_KEY_UDF_OBJS)
			$(SHARD_CC) $(CFLAGS) -o shard_key_udf.so $(SHARD_KEY_UDF_OBJS) $(LIBS)
	 
	clean:
			-rm -f *.o core shard_key_udf.so

사용자 정의 해시 함수를 포함하기 위해 **SHARD_KEY_LIBRARY_NAME**, **SHARD_KEY_FUNCTION_NAME** 파라미터를 위 구현과 일치하도록 수정한다. ::

	[%student_no]
	SHARD_KEY_LIBRARY_NAME ????=$CUBRID/conf/shard_key_udf.so
	SHARD_KEY_FUNCTION_NAME ?=fn_shard_key_udf

구동 및 모니터링
================

cubrid shard 유틸리티
---------------------

cubrid shard 유틸리티를 이용하여 CUBRID SHARD를 구동하거나 정지할 수 있고, 각종 상태 정보를 조회할 수 있다.

**CUBRID SHARD 구동**

CUBRID SHARD를 구동하기 위해서는 다음과 같이 입력한다. ::

	% cubrid shard start
	@ cubrid shard start
	++ cubrid shard start: success

이미 CUBRID SHARD가 구동 중이면 다음과 같은 메시지가 출력된다. ::

	% cubrid shard start
	@ cubrid shard start
	++ cubrid shard is running.

**cubrid shard start** 수행 시 CUBRID SHARD 환경 설정 파일(**shard.conf**) 의 설정을 읽어 설정 상의 모든 구성 요소를 구동한다. 구동 시 메타데이터 DB 및 shard DB에 접속을 하므로, CUBRID SHARD의 구동 전 메타데이터 DB 및 shard DB 들이 모두 구동되어 있어야 한다.

**CUBRID SHARD 정지**

CUBRID SHARD 를 종료하기 위하여 다음과 같이 입력한다. ::

	% cubrid shard stop
	@ cubrid shard stop
	++ cubrid shard stop: success

이미 CUBRID SHARD 가 종료되었다면 다음과 같은 메시지가 출력된다. ::

	$ cubrid shard stop
	@ cubrid shard stop
	++ cubrid shard is not running.

**CUBRID SHARD 파라미터의 동적 변경**

CUBRID SHARD의 구동과 관련된 파라미터는 CUBRID SHARD 환경 설정 파일(**shard.conf**) 에서 설정할 수 있다. 그 밖에, **shard_broker_changer** 유틸리티를 이용하여 구동 중에만 한시적으로 일부 CUBRID SHARD 파라미터를 동적으로 변경할 수 있다. CUBRID SHARD 파라미터 설정 및 동적으로 변경 가능한 파라미터 등 기타 자세한 내용은 `CUBRID SHARD> 구성 및 설정 <#admin_admin_shard_conf_comp_htm>`_을 참조한다.

**구문**

CUBRID SHARD 구동 중에 파라미터를 변경하기 위한 **shard_broker_changer** 유틸리티의 구문은 다음과 같다. *shard-name* 에는 구동 중인 CUBRID SHARD 이름을 입력하고 *parameter* 에는 동적으로 변경할 수 있는 파라미터를 입력한다. 변경하고자 하는 파라미터에 따라 *value* 가 지정되어야 한다. CUBRID SHARD의 식별 번호를 지정하여 특정 CUBRID SHARD에만 변경을 적용할 수 있다. *proxy-number* 는 **cubrid shard status** 명령에서 출력되는 PROXY-ID이다. ::

	shard_broker_changer shard-name [proxy-number] parameter value

**예제**

구동 중인 CUBRID SHARD에서 SQL 로그가 기록되도록 **SQL_LOG** 파라미터를 ON으로 설정하기 위하여 다음과 같이 입력한다. 이와 같은 파라미터의 동적 변경은 CUBRID SHARD가 구동 중일 때만 한시적으로 효력이 있다. ::

	% shard_broker_changer shard1 sql_log on
	OK

**CUBRID SHARD 상태 확인**

**cubrid shard status** 는 여러 옵션을 제공하며, 각 shard broker 및 shard proxy, shard cas의 상태 정보를 확인할 수 있도록 한다. 또한 메타데이터 정보 및 shard proxy에 접속한 클라이언트의 정보를 확인 가능하다. ::

	cubrid shard status options [<expr>]
	options : [-b | -f [-l sec] | -t | -c | -m | -s <sec>]

<*expr*>이 주어지면 해당 CUBRID SHARD에 대한 상태 모니터링을 수행하고, 생략되면 CUBRID SHARD 환경 설정 파일(**shard.conf**)에 등록된 전체 CUBRID SHARD에 대해 상태 모니터링을 수행한다.

**옵션**

다음은 결합할 수 있는 옵션에 관해 설명한 표이다.

+--------+--------------------------------------------------------------------------+
| 옵션   | 설명                                                                     |
|        |                                                                          |
+========+==========================================================================+
| <      | CUBRID SHARD                                                             |
| *expr* | 이름이 <                                                                 |
| >      | *expr*                                                                   |
|        | >을 포함하는                                                             |
|        | CUBRID SHARD에 관한 상태 정보를 출력한다. 지정되지 않으면 전체           |
|        | CUBRID SHARD의 상태 정보를 출력한다.                                     |
|        |                                                                          |
+--------+--------------------------------------------------------------------------+
| **-b** | CUBRID proxy나 CUBRID CAS에 관한 정보는 포함하지 않고, CUBRID broker에   |
|        | 관한 상태 정보만 출력한다.                                               |
+--------+--------------------------------------------------------------------------+
| **-c** | CUBRID proxy에 접속한 클라이언트 정보를 출력한다.                        |
|        |                                                                          |
+--------+--------------------------------------------------------------------------+
| **-m** | 메타데이터 정보를 출력한다.                                              |
|        |                                                                          |
+--------+--------------------------------------------------------------------------+
| **-t** | 화면 출력시 tty mode로 출력한다. 출력 내용을 리다이렉션하여 파일로       |
|        | 쓸 수 있다.                                                              |
+--------+--------------------------------------------------------------------------+
| **-f** | CUBRID SHARD에 대한 좀 더 상세한 정보를 출력한다.                        |
| [      |                                                                          |
| **-l** |                                                                          |
| *secs* |                                                                          |
| ]      |                                                                          |
|        |                                                                          |
+--------+--------------------------------------------------------------------------+
| **-s** | CUBRID SHARD에 대한 상태 정보를 지정된 시간마다 주기적으로 출력한다.     |
| *secs* | **q**                                                                    |
|        | 를 입력하면 명령 프롬프트로 복귀한다.                                    |
|        |                                                                          |
+--------+--------------------------------------------------------------------------+

**예제**

전체 CUBRID SHARD 상태 정보를 확인하기 위하여 옵션 및 인수를 입력하지 않으면 다음과 같이 출력된다. ::

	$ cubrid shard status
	@ cubrid shard status
	% test_shard  - shard_cas [2576,45000] /home/CUBRID/log/broker/test_shard.err
	 JOB QUEUE:0, AUTO_ADD_APPL_SERVER:ON, SQL_LOG_MODE:ALL:100000
	 LONG_TRANSACTION_TIME:60.00, LONG_QUERY_TIME:60.00, SESSION_TIMEOUT:10
	 KEEP_CONNECTION:AUTO, ACCESS_MODE:RW
	----------------------------------------------------------------
	PROXY_ID SHARD_ID   CAS_ID   PID   QPS   LQS PSIZE STATUS
	----------------------------------------------------------------
		   1        1        1  2580     100     3 55968 IDLE
		   1        2        1  2581     200     4 55968 IDLE

*   % test_shard : proxy의 이름
*   shard_cas : 응용 서버의 형태. [shard_cas | shard_cas_myqsl]
*   [2576, 45000] : proxy 프로세스 ID와 proxy 접속 포트 번호
*   /home/CUBRID/log/broker/test_shard.err : test_shard의 에러 로그 파일
*   JOB QUEUE : 작업 큐에 대기 중인 작업 개수

*   SQL_LOG_MODE : 모든 SQL에 대해 로그를 기록하기 위해 **shard.conf** 파일의 **SQL_LOG** 파라미터 값을 **ALL** 로 지정했다.
*   SLOW_LOG : 장기 실행 질의문 또는 에러가 발생한 질의문을 SLOW SQL LOG 파일에 기록하기 위해 **shard.conf** 파일의 **SLOW_LOG** 파라미터 값을 **ON** 으로 지정했다.

*   LONG_TRANSACTION_TIME : 장기 실행(long-duration) 트랜잭션으로 판단하는 트랜잭션의 실행 시간. 트랜잭션의 실행시간이 60초를 넘으면 장기 실행 트랜잭션이다.
*   LONG_QUERY_TIME : 장기 실행 질의(long-duration query)으로 판단하는 질의의 실행 시간. 질의의 실행 시간이 60초를 넘으면 장기 실행 질의이다.

*   SESSION_TIMEOUT : 트랜잭션 시작 이후 커밋 혹은 롤백하지 않은 채로 아무런 요청이 없는 상태의 응용 서버(CAS) 세션을 종료하기 위한 타임아웃 값. 이 상태에서 이 시간을 초과하면 응용 클라이언트와 응용 서버(CAS) 간의 접속이 종료된다. **shard.conf** 의 **SESSION_TIMEOUT** 파라미터 값이 300(초)이다.

*   ACCESS_MODE : shard broker의 동작 모드. RW는 데이터베이스 조회 뿐만 아니라 수정도 가능한 모드이다.

*   PROXY_ID : shard broker 내부에서 순차적으로 부여된 proxy의 일련번호
*   SHARD_ID : proxy에 설정된 shard DB의 일련번호
*   CAS_ID : shard DB에 접속하는 응용 서버(CAS)의 일련번호
*   PID : shard DB에 접속하는 응용 서버(CAS) 프로세스의 ID
*   QPS : 초당 처리된 질의의 수
*   LQS : 초당 처리되는 장기 실행 질의의 수
*   PSIZE : 응용 서버 프로세스 크기
*   STATUS : 응용 서버의 현재 상태로서, BUSY/IDLE/CLIENT_WAIT/CLOSE_WAIT/CON_WAIT가 있다.

shard broker에 관한 상태 정보를 확인하려면 다음과 같이 입력한다. ::

	$ cubrid shard status -b
	@ cubrid shard status
	  NAME           PID  PORT  Active-P  Active-C      REQ  TPS  QPS  K-QPS NK-QPS    LONG-T    LONG-Q  ERR-Q
	==========================================================================================================
	* test_shard    3548 45000         1         2        0    0    0      0      0    0/60.0    0/60.0      0

*   NAME : proxy의 이름
*   PID : proxy의 프로세스 ID
*   PORT : proxy의 포트 번호
*   Active-P : proxy의 개수
*   Active-C : 응용 서버(CAS)의 개수
*   REQ : proxy가 처리한 클라이언트 요청 개수
*   TPS : 초당 처리된 트랜잭션의 수(옵션이 **-b -s** <*sec*>일 때만 계산됨)
*   QPS : 초당 처리된 질의의 수(옵션이 **-b -s** <*sec*>일 때만 계산됨)
*   K-QPS : shard key가 포함된 질의에 대한 QPS
*   NK-QPS : shard key가 포함되지 않은 질의에 대한 QPS
*   LONG-T : **LONG_TRANSACTION_TIME** 시간을 초과한 트랜잭션 수 / **LONG_TRANSACTION_TIME** 파라미터의 값
*   LONG-Q : **LONG_QUERY_TIME** 시간을 초과한 질의의 수 / **LONG_QUERY_TIME** 파라미터의 값
*   ERR-Q : 에러가 발생한 질의의 수


shard broker에 관한 좀 더 상세한 상태 정보를 확인하려면 다음과 같이 입력한다. ::

	$ cubrid shard status -b -f
	@ cubrid shard status
	NAME           PID  PSIZE  PORT  Active-P  Active-C      REQ  TPS  QPS  K-QPS (H-KEY   H-ID H-ALL) NK-QPS    LONG-T    LONG-Q  ERR-Q  CANCELED  ACCESS_MODE  SQL_LOG
	======================================================================================================================================================================
	* test_shard 3548 100644 45000         1         2        0    0    0      0      0      0      0      0    0/60.0    0/60.0      0         0           RW      ALL

*   NAME : proxy의 이름
*   PID : proxy의 프로세스 ID
*   PSIZE : proxy의 프로세스 크기
*   PORT : proxy의 포트 번호
*   Active-P : proxy의 개수
*   Active-C : 응용 서버(CAS)의 개수
*   REQ : proxy가 처리한 클라이언트 요청 개수
*   TPS : 초당 처리된 트랜잭션의 수(옵션이 **-b -s** <*sec*>일 때만 계산됨)
*   QPS : 초당 처리된 질의의 수(옵션이 **-b -s** <*sec*>일 때만 계산됨)
*   K-QPS : shard key가 포함된 질의에 대한 QPS
*   H-KEY : shard_key 힌트가 포함된 질의에 대한 QPS
*   H-ID : shard_id 힌트가 포함된 질의에 대한 QPS
*   H-ALL : shard_all 힌트가 포함된 질의에 대한 QPS
*   NK-QPS : shard key가 포함되지 않은 질의에 대한 QPS
*   LONG-T : **LONG_TRANSACTION_TIME** 시간을 초과한 트랜잭션 수 / **LONG_TRANSACTION_TIME** 파라미터의 값
*   LONG-Q : **LONG_QUERY_TIME** 시간을 초과한 질의의 수 / **LONG_QUERY_TIME** 파라미터의 값
*   ERR-Q : 에러가 발생한 질의의 수
*   CANCELED : shard broker 시작 이후 사용자 인터럽트로 인해 취소된 질의의 개수 (**-l** *N* 옵션과 함께 사용하면 *N* 초 동안 누적된 개수)
*   ACCESS_MODE : shard broker의 동작 모드. RW는 데이터베이스 조회 뿐만 아니라 수정도 가능한 모드이다.
*   SQL_LOG : SQL 로그를 남기도록 **shard.conf** 파일의 **SQL_LOG** 파라미터 값이 ALL이다.

**-s** 옵션을 이용하여 test_shard를 포함하는 이름을 가진 shard broker의 모니터링 주기를 입력하고, 주기적으로 shard broker의 상태를 모니터링하기 위해 다음과 같이 입력한다. 인수로 test_shard를 입력하지 않으면 모든 shard broker에 대하여 상태 모니터링이 주기적으로 수행된다. 또한, **q** 를 입력하면 모니터링 화면에서 명령 프롬프트로 복귀한다. ::

	$ cubrid shard status -b test_shard -s 1 -t
	@ cubrid shard status
	  NAME           PID  PORT  Active-P  Active-C      REQ  TPS  QPS  K-QPS NK-QPS    LONG-T    LONG-Q  ERR-Q
	==========================================================================================================
	* test_shard    3548 45000         1         2        0    0    0      0      0    0/60.0    0/60.0      0

**-t** 옵션을 사용하여, TPS 와 QPS 정보를 파일로 출력한다. 파일로 출력하는 것을 중단하려면 <Crtl+C> 키를 눌러서 프로그램을 정지시킨다. ::

	% cubrid shard status -b -s 1 -t > log_file

**-m** 옵션을 사용하여 메타데이터 정보를 출력한다. **shard.conf** 의 파라미터에 대한 내용은 `기본 설정 파일 shard.conf <#admin_admin_shard_conf_basic_htm>`_ 을 참고한다. ::

	$ cubrid shard status -m
	@ cubrid shard status
	% test_shard [299009]
	MODULAR : 256, LIBRARY_NAME : NOT DEFINED, FUNCTION_NAME : NOT DEFINED
	SHARD STATISTICS
			   ID  NUM-KEY-Q  NUM-ID-Q   NUM-NO-HINT-Q       SUM
			-----------------------------------------------------
				0          0         0               0         0
				1          0         0               0         0
				2          0         0               0         0
				3          0         0               0         0

*   test_shard : proxy의 이름
*   [299009] : **shard.conf** 의 **METADATA_SHM_ID** 파라미터의 decimal 값
*   MODULAR : **shard.conf** 의 **SHARD_KEY_MODULR** 파라미터 값
*   LIBRARY_NAME : **shard.conf** 의 **SHARD_KEY_LIBRARY_NAME** 파라미터 값
*   FUNCTION_NAME : **shard.conf** 의 **SHARD_KEY_FUNCTION_NAME** 파라미터 값
*   SHARD STATISTICS : shard ID 질의 정보

    *   ID : shard DB 일련번호(shard ID)
    *   NUM-KEY-Q : shard key가 포함된 질의 요청 수
    *   NUM-ID-Q : shard ID가 포함된 질의 요청 수
    *   NUM-NO-HINT-Q : **IGNORE_SHARD_HINT** 가 설정된 경우 hint 없이 load balancing되어 처리된 요청 수
    *   SUM : NUM-KEY-Q + NUM-ID-Q

**-m -f** 옵션을 사용하면 좀 더 상세한 메타데이터 정보를 출력한다. **shard.conf** 의 파라미터에 대한 내용은 `기본 설정 파일 shard.conf <#admin_admin_shard_conf_basic_htm>`_ 을 참고한다. ::

	$ cubrid shard status –m -f
	@ cubrid shard status
	% test_shard [299009]
	MODULAR : 256, LIBRARY_NAME : NOT DEFINED, FUNCTION_NAME : NOT DEFINED
	SHARD : 0 [HostA] [shard1], 1 [HostB] [shard1], 2 [HostC] [shard1], 3 [HostD] [shard1]
	SHARD STATISTICS
			   ID  NUM-KEY-Q  NUM-ID-Q   NUM-NO-HINT-Q       SUM
			-----------------------------------------------------
				0          0         0               0         0
				1          0         0               0         0
				2          0         0               0         0
				3          0         0               0         0
	 
	RANGE STATISTICS : user_no
			  MIN ~   MAX :      SHARD     NUM-Q
			------------------------------------
				0 ~    31 :          0         0
			   32 ~    63 :          1         0
			   64 ~    95 :          2         0
			   96 ~   127 :          3         0
			  128 ~   159 :          0         0
			  160 ~   191 :          1         0
			  192 ~   223 :          2         0
			  224 ~   255 :          3         0
	DB Alias : shard1 [USER : shard, PASSWD : shard123]

*   test_shard : proxy의 이름
*   [299009] : **shard.conf** 의 **METADATA_SHM_ID** 파라미터의 decimal 값
*   MODULAR : **shard.conf** 의 **SHARD_KEY_MODULR** 파라미터 값
*   LIBRARY_NAME : **shard.conf** 의 **SHARD_KEY_LIBRARY_NAME** 파라미터 값
*   FUNCTION_NAME : **shard.conf** 의 **SHARD_KEY_FUNCTION_NAME** 파라미터 값
*   SHARD : proxy 내의 shard DB 정보

    *   0 : shard DB 일련번호(shard ID)
    *   [HostA] : shard 접속 정보
    *   [shard1] : 실제 DB 이름

*   ID : shard DB 일련번호(shard ID)
*   NUM-KEY-Q : shard key가 포함된 질의 요청 수
*   NUM-ID-Q : shard ID가 포함된 질의 요청 수
*   SUM : NUM-KEY-Q + NUM-ID-Q
*   RANGE STATISTICS : shard key 질의 정보

    *   user_no : shard key 이름
    *   MIN : shard key 최소 범위
    *   MAX : shard key 최대 범위
    *   SHARD : shard DB 일련번호(shard ID)
    *   NUM-Q : shard key가 포함된 질의 요청 수

**-c** 옵션을 사용하여 shard proxy에 접속한 클라이언트 정보를 출력한다. ::

	$ cubrid shard status -c
	@ cubrid shard status
	% test_shard(0), MAX-CLIENT : 10000
	------------------------------------------------------------------------------------------------
	 CLIENT-ID           CLIENT-IP             CONN-TIME            L-REQ-TIME            L-RES-TIME
	------------------------------------------------------------------------------------------------
			 0         10.24.18.68   2011/12/15 16:33:31   2011/12/15 16:33:31   2011/12/15 16:33:31

*   CLIENT-ID : proxy 내에서 순차적으로 부여한 클라이언트 일련 번호
*   CLIENT-IP : 클라이언트 IP 주소
*   CONN-TIME : proxy에 접속한 시각
*   L-REQ-TIME : proxy에 마지막으로 질의를 요청한 시각
*   L-RES-TIME : proxy로부터 마지막으로 응답을 받은 시각

**shard proxy 접속 제한**

shard proxy에 접속하는 응용 클라이언트를 제한하려면 **cubrid_shard.conf** 의 **ACCESS_CONTROL** 파라미터 값을 ON으로 설정하고, **ACCESS_CONTROL_FILE** 파라미터 값에 접속을 허용하는 사용자와 데이터베이스 및 IP 목록을 작성한 파일 이름을 입력한다. **ACCESS_CONTROL** 파라미터의 기본값은 OFF이다.

**ACCESS_CONTROL**, **ACCESS_CONTROL_FILE** 파라미터는 공통 적용 파라미터가 위치하는 [shard] 아래에 작성해야 한다.

**ACCESS_CONTROL_FILE의** 형식은 다음과 같다. ::

	[%<shard_name>]
	<db_name>:<db_user>:<ip_list_file>

	...

*   <*shard_name*> : shard proxy 이름. **cubrid_broker.conf** 에서 지정한 shard proxy 이름 중 하나이다.
*   <*db_name*> : 데이터베이스 이름. *로 지정하면 모든 데이터베이스를 허용한다.
*   <*db_user*> : 데이터베이스 사용자 ID. *로 지정하면 모든 데이터베이스 사용자 ID를 허용한다.
*   <*ip_list_file*> : 접속 가능한 IP 목록을 저장한 파일의 이름. ip_list_file1, ip_list_file2, ... 와 같이 파일 여러 개를 쉼표(,)로 구분하여 지정할 수 있다.

shard proxy별로 [%<*broker_name*>]과 <*db_name*>:<*db_user*>:<*ip_list_file*>을 추가로 지정할 수 있으며, 같은 <*db_name*>, 같은 <*db_user*>에 대해 별도의 라인으로 추가 지정할 수 있다.

ip_list_file의 작성 형식은 다음과 같다. ::

	<ip_addr>

	...

*   <*ip_addr*> : 접근을 허용할 IP 명. 뒷자리를 *로 입력하면 뒷자리의 모든 IP를 허용한다.

**ACCESS_CONTROL** 값이 ON인 상태에서 **ACCESS_CONTROL_FILE** 이 지정되지 않으면 shard proxy는 localhost에서의 접속 요청만을 허용한다. shard proxy 구동 시 **ACCESS_CONTROL_FILE** 및 ip_list_file 분석에 실패하면 shard proxy 는 localhost에서의 접속 요청만을 허용한다.

shard proxy 구동 시 **ACCESS_CONTROL_FILE** 및 ip_list_file 분석에 실패하는 경우 shard proxy는 구동되지 않는다. ::

	# cubrid_broker.conf
	[broker]
	MASTER_SHM_ID           =30001
	ADMIN_LOG_FILE          =log/broker/cubrid_broker.log
	ACCESS_CONTROL   =ON
	ACCESS_CONTROL_FILE     =/home1/cubrid/access_file.txt
	[%QUERY_EDITOR]
	SERVICE                 =ON
	BROKER_PORT             =30000
	......

다음은 **ACCESS_CONTROL_FILE** 의 한 예이다. 파일 내에서 사용하는 *는 모든 것을 나타내며, 데이터베이스 이름, 데이터베이스 사용자 ID, 접속을 허용하는 IP 리스트 파일 내의 IP에 대해 지정할 때 사용할 수 있다. ::

	[%QUERY_EDITOR]
	dbname1:dbuser1:READIP.txt
	dbname1:dbuser2:WRITEIP1.txt,WRITEIP2.txt
	*:dba:READIP.txt
	*:dba:WRITEIP1.txt
	*:dba:WRITEIP2.txt
	 
	[%SHARD2]
	dbname:dbuser:iplist2.txt
	 
	[%SHARD3]
	dbname:dbuser:iplist2.txt
	 
	[%SHARD4]
	dbname:dbuser:iplist2.txt


위의 예에서 지정한 shard proxy는 QUERY_EDITOR, SHARD2, SHARD3, SHARD4이다. 위 설정에서 QUERY_EDITOR shard proxy는 다음과 같은 응용의 접속 요청만을 허용한다.

*   dbname1에 dbuser1으로 로그인하는 사용자가 READIP.txt에 등록된 IP에서 접속
*   dbname1에 dbuser2로 로그인하는 사용자가 WRITEIP1.txt나 WRITEIP2.txt에 등록된 IP에서 접속
*   모든 데이터베이스에 DBA로 로그인하는 사용자가 READIP.txt나 WRITEIP1.txt 또는 WRITEIP2.txt에 등록된 IP에서 접속

다음은 ip_list_file에서 허용하는 IP를 설정하는 예이다. ::

	192.168.1.25
	192.168.*
	10.*
	*

위의 예에서 지정한 IP를 보면 다음과 같다.

*   첫 번째 줄의 설정은 192.168.1.25을 허용한다.
*   두 번째 줄의 설정은 192.168 로 시작하는 모든 IP를 허용한다.
*   세 번째 줄의 설정은 10으로 시작하는 모든 IP를 허용한다.
*   네 번째 줄의 설정은 모든 IP를 허용한다.

이미 구동되어 있는 shard proxy에 대해서는 다음 명령어를 통해 설정 파일을 다시 적용하거나 현재 적용 상태를 확인할 수 있다.

shard proxy에서 허용하는 데이터베이스, 데이터베이스 사용자 ID, IP를 설정한 후 변경된 내용을 서버에 적용하려면 다음 명령어를 사용한다. ::

	cubrid shard acl reload [<SP_NAME>]

*   *SP_NAME* : shard proxy 이름. 이 값을 지정하면 특정 shard proxy에만 변경 내용을 적용할 수 있으며, 생략하면 전체 shard proxy에 변경 내용을 적용한다.

현재 구동 중인 shard proxy에서 허용하는 데이터베이스, 데이터베이스 사용자 ID, IP의 설정을 화면에 출력하려면 다음 명령어를 사용한다. ::

	cubrid shard acl status [<SP_NAME>]

*   *SP_NAME* : shard proxy 이름. 이 값을 지정하면 특정 shard proxy의 설정을 출력할 수 있으며, 생략하면 전체 shard proxy의 설정을 출력한다.

.. note:: 데이터베이스 서버의 접속 제한에 대한 자세한 내용은 `데이터베이스 서버 접속 제한 <#admin_admin_service_server_acces_3933>`_ 을 참고한다.

**특정 shard 관리**

shard1만 구동하려면 다음과 같이 입력한다. ::

	$ cubrid shard on shard1

만약, shard1이 공유 메모리에 설정되지 않은 상태라면 다음과 같은 메시지가 출력된다. ::

	% cubrid shard on shard1
	Cannot open shared memory

shard1만 종료하려면 다음과 같이 입력한다. ::

	$ cubrid shard off shard1

shard1을 재시작하려면 다음과 같이 입력한다. ::

	$ cubrhd shard restart shard1

shard proxy 리셋 기능은 HA에서 failover 등으로 shard proxy가 원하지 않는 데이터베이스 서버에 연결되었을 때, 기존 연결을 끊고 새로 연결할 수 있도록 한다. 만약 동적으로
**SHARD_DB_NAME**, **SHARD_DB_USER**, **SHARD_DB_PASSWORD** 를 변경했다면, 변경된 값으로 접속을 시도한다. ::

	% cubrid shard reset shard1

CUBRID SHARD 로그
-----------------

shard 구동과 관련된 로그에는 접속 로그, 프록시 로그, SQL 로그, 에러 로그가 있다. 각각 로그의 저장 디렉터리 변경은 shard 환경 설정 파일(**shard.conf**) 의 **LOG_DIR**, **ERROR_LOG_DIR**, **PROXY_LOG_FILE** 파라미터를 통해 설정할 수 있다.

**SHARD PROXY 로그**

**접속 로그**

*   파라미터 : **ACCESS_LOG**
*   설명 : 클라이언트의 접속을 logging한다(기존 broker는 cas에서 로그를 남긴다).
*   기본 저장 디렉터리 : $CUBRID/log/broker/
*   파일 이름 : <broker_name>_<proxy_index>.access
*   로그 형식 : cas에서 남기는 access log와 cas_index 이외의 모든 string 동일

::

	10.24.18.67 - - 1340243427.828 1340243427.828 2012/06/21 10:50:27 ~ 2012/06/21 10:50:27 23377 - -1 shard1     shard1
	10.24.18.67 - - 1340243427.858 1340243427.858 2012/06/21 10:50:27 ~ 2012/06/21 10:50:27 23377 - -1 shard1     shard1
	10.24.18.67 - - 1340243446.791 1340243446.791 2012/06/21 10:50:46 ~ 2012/06/21 10:50:46 23377 - -1 shard1     shard1
	10.24.18.67 - - 1340243446.821 1340243446.821 2012/06/21 10:50:46 ~ 2012/06/21 10:50:46 23377 - -1 shard1     shard1

**프록시 로그**

*   파라미터 : **PROXY_LOG_DIR**
*   설명 : proxy 내부의 동작을 logging한다.
*   기본 저장 디렉터리 : $CUBRID/log/broker/proxy_log
*   파일 이름 : <broker_name>_<proxy_index>log

::

	06/21 10:50:46.822 [SRD] ../../src/broker/shard_proxy_io.c(1045): New socket io created. (fd:50).
	06/21 10:50:46.822 [SRD] ../../src/broker/shard_proxy_io.c(2517): New client connected. client(client_id:3, is_busy:Y, fd:50, ctx_cid:3, ctx_uid:4).
	06/21 10:50:46.825 [DBG] ../../src/broker/shard_proxy_io.c(3298): Shard status. (num_cas_in_tran=1, shard_id=2).
	06/21 10:50:46.827 [DBG] ../../src/broker/shard_proxy_io.c(3385): Shard status. (num_cas_in_tran=0, shard_id=2).

**프록시 로그 레벨**

*   파라미터 : **PROXY_LOG**
*   프록시 로그 레벨 정책 : 상위 level을 설정하면 하위의 모든 로그가 남는다.

    *   예) SCHEDULE을 설정하면, ERROR | TIMEOUT | NOTICE | SHARD | SCHEDULE 로그를 모두 남긴다.

*   프록시 로그 레벨 항목

    *   NONE or OFF : 로그를 남기지 않는다.
    *   ERROR(default) : 내부적으로 에러가 발생하여 정상적으로 처리되지 못하는 경우
    *   TIMEOUT : session timeout이나 query timeout 등의 timeout
    *   NOTICE : 힌트 없는 query 및 기타 에러는 아닌 경우
    *   SHARD : client 의 request가 어떤 shard의 어떤 cas로 갔는지, 그것이 다시 client response 되었는지 등의 scheduling
    *   SCHEDULE : 힌트 parsing 및 hash를 통해 shard key id 가져오는 것 등의 shard processing
    *   ALL : 모든 로그

**SHARD CAS 로그**

**SQL 로그**

*   파라미터 : **SQL_LOG**
*   설명 : prepare/exeucte/fetch 등의 query 및 기타 cas 정보를 logging한다.
*   기본 저장 디렉터리 : $CUBRID/log/broker/sql_log
*   파일 이름 : %broker_name%_%proxy_index%_%shard_index%_%as_index%.sql.log

::

	06/21 10:13:00.005 (0) STATE idle
	06/21 10:13:01.035 (0) CAS TERMINATED pid 31595
	06/21 10:14:20.198 (0) CAS STARTED pid 23378
	06/21 10:14:21.227 (0) connect db shard1@HostA user dba url shard1 session id 3
	06/21 10:14:21.227 (0) DEFAULT isolation_level 3, lock_timeout -1
	06/21 10:50:28.259 (1) prepare srv_h_id 1
	06/21 10:50:28.259 (0) auto_rollback
	06/21 10:50:28.259 (0) auto_rollback 0


**에러 로그**

*   파라미터 : **ERROR_LOG_DIR**
*   설명 : cubrid의 경우 cs library에서 EID 및 error string을 해당 파일에 logging한다. cas4o/m의 경우 cas에서 해당 파일에 error를 logging한다.
*   기본 저장 디렉터리 : $CUBRID/log/broker/error_log
*   파일 이름 : %broker_name%_%proxy_index%_%shard_index%_%cas_index%.err

::

	Time: 06/21/12 10:50:27.776 - DEBUG *** file ../../src/transaction/boot_cl.c, line 1409
	trying to connect 'shard1@localhost'
	Time: 06/21/12 10:50:27.776 - DEBUG *** file ../../src/transaction/boot_cl.c, line 1418
	ping server with handshake
	Time: 06/21/12 10:50:27.777 - DEBUG *** file ../../src/transaction/boot_cl.c, line 966
	boot_restart_client: register client { type 4 db shard1 user dba password (null) program cubrid_cub_cas_1 login cubrid_user host HostA pid 23270 }

제약 사항
=========

**한 트랜잭션 내에서 다수의 shard DB의 데이터 변경 또는 조회**

하나의 트랜잭션은 오직 하나의 shard DB에서만 수행되어야 하며, 따라서 아래와 같은 제약사항이 존재한다.

*   shard key 변경(**UPDATE**)으로 인해 여러 shard DB의 데이터를 변경하는 것은 불가능하며, 필요하다면 **DELETE** / **INSERT** 를 이용한다.

*   여러 shard DB 데이터에 대한 join, sub-query, or, union, group by, between, like, in, exist, any/some/all 등 질의를 수행하면, 의도한 것과 다른 결과가 반환될 수 있다.

**세션**

세션 정보가 각 shard DB 내에서만 유효하므로, :func:`LAST_INSERT_ID` 와 같은 세션 관련 함수의 결과가 의도한 바와 다를 수 있다.

**auto increment**

auto increment 속성 또는 SERIAL 등의 값이 각 shard DB 내에서만 유효하므로, 의도한 것과 다른 값을 반환할 수 있다.
