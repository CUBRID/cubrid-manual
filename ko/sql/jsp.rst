
:meta-keywords: cubrid jsp, loadjava utility
:meta-description: CUBRID supports to develop stored functions and procedures in Java. Java stored functions/procedures are executed on the JVM (Java Virtual Machine) hosted by CUBRID.

***********************
Java 저장 함수/프로시저
***********************

저장 함수와 저장 프로시저를 사용하면 SQL로 구현하지 못하는 복잡한 프로그램의 로직을 구현할 수 있으며, 사용자가 보다 쉽게 데이터를 조작하게 할 수 있다. 함수와 프로시저는 데이터를 조작하기 위해 실행 명령의 흐름이 있고, 쉽게 조작할 수 있고, 관리할 수 있는 블록 단위라고 할 수 있다.

CUBRID는 Java로 저장 함수와 프로시저를 개발할 수 있도록 지원한다. Java 저장 함수와 프로시저는 CUBRID에서 호스팅한 Java 가상 머신(JVM, Java Virtual Machine)에서 실행된다.

Java 저장 함수/프로시저는 SQL에서도 호출할 수 있으며, JDBC를 사용하여 쉽게 Java 응용 프로그램에서 호출할 수 있다.

Java 저장 함수/프로시저를 사용할 때 얻을 수 있는 이점은 다음과 같다.

*   **생산성과 사용성**: Java 저장 함수/프로시저는 한번 만들어 놓으면 계속해서 사용할 수 있다. 사용자가 저장 함수와 저장 프로시저를 SQL에서도 호출하여 사용할 수 있고, JDBC를 사용하여 쉽게 Java 응용 프로그램에서 호출할 수 있다.
*   **뛰어난 상호 운용성, 이식성**: Java 저장 함수/프로시저는 Java 가상 머신을 사용하므로, 시스템에 Java 가상 머신을 사용할 수만 있다면 언제 어디서나 사용할 수 있다.

.. note::

    *   Java를 제외한 다른 언어에서는 저장 함수/프로시저를 지원하지 않는다. CUBRID에서 저장 함수/프로시저는 오직 Java로만 구현 가능하다.

.. _jsp-environment-configuration:

Java 저장 함수/프로시저 환경 설정
=================================

CUBRID에서 Java 저장 함수/프로시저를 사용하기 위해서는 CUBRID 서버가 설치되는 환경에 Java Runtime Environment (JRE) 1.6 이상 버전이 설치되어야 한다. JRE는 Developer Resources for Java Technology 사이트(`http://java.sun.com <http://java.sun.com>`_)에서 다운로드할 수 있다.

CUBRID 환경 설정 파일(cubrid.conf)에 java_stored_procedure 파라미터가 yes로 설정되어 있으면, CUBRID 64비트 버전에는 JRE 64비트 버전이 필요하고, CUBRID 32비트 버전에는 JRE 32비트 버전이 필요하다. JRE 32비트 버전이 설치된 컴퓨터에서 CUBRID 64비트 버전을 실행하면 아래와 같은 에러 메시지가 출력된다. ::

    % cubrid server start demodb
     
    This may take a long time depending on the amount of recovery works to do.
    WARNING: Java VM library is not found : /usr/java/jdk1.6.0_15/jre/lib/amd64/server/libjvm.so: cannot open shared object file: No such file or directory.
    Consequently, calling java stored procedure is not allowed

JRE가 이미 설치되어 있다면, 아래와 같은 명령으로 버전을 확인한다. ::

    % java -version Java(TM) SE Runtime Environment (build 1.6.0_05-b13)
    Java HotSpot(TM) 64-Bit Server VM (build 10.0-b19, mixed mode)

Windows 환경
------------

CUBRID는 Windows 환경에서 **jvm.dll** 파일을 로딩하여 Java 가상 머신을 실행시킨다. CUBRID는 먼저 시스템의 **Path** 환경 변수에서 **jvm.dll** 을 찾아 로딩한다. 만약 찾지 못하면 시스템 레지스트리에 등록된 Java 런타임 정보를 이용한다.

아래와 같이 명령어를 실행하여 **JAVA_HOME** 환경 변수를 설정하고 Java 실행 파일이 있는 디렉터리를 **Path** 환경 변수에 추가할 수 있다. GUI를 이용해서 환경 변수를 설정하는 방법은 JDBC 설치 및 설정을 참고한다.

* JDK 1.6 64비트 버전을 설치하고, 환경 변수를 설정한 예 ::

    % set JAVA_HOME=C:\jdk1.6.0
    % set PATH=%PATH%;%JAVA_HOME%\jre\bin\server

* JDK 1.6 32비트 버전을 설치하고, 환경 변수를 설정한 예 ::
  
    % set JAVA_HOME=C:\jdk1.6.0
    % set PATH=%PATH%;%JAVA_HOME%\jre\bin\client

SUN의 Java 가상 머신을 사용하지 않고 다른 벤더의 구현을 사용하려면 해당 벤더의 설치에서 **jvm.dll** 파일의 경로를 **PATH** 에 추가해 주어야 한다.

Linux/Unix 환경
---------------

CUBRID는 Linux/Unix 환경에서 **libjvm.so** 파일을 로딩하여 Java 가상 머신을 실행시킨다. CUBRID는 먼저 **LD_LIBRARY_PATH** 환경 변수에서 **libjvm.so** 파일을 찾아 로딩한다. 만약 찾지 못하면 **JAVA_HOME** 환경 변수를 이용하여 찾는다. 리눅스의 경우 glibc 2.3.4 이상만 지원되며, 아래는 리눅스 환경 설정 파일(예: **.profile**, **.cshrc**, **.bashrc**, **.bash_profile** 등)에 환경 변수를 설정하는 예이다.

*   JDK 1.6 64비트 버전을 설치하고, bash 셸에서 환경 변수를 설정한 예 ::

    % JAVA_HOME=/usr/java/jdk1.6.0_10
    % LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/amd64:$JAVA_HOME/jre/lib/amd64/server:$LD_LIBRARY_PATH
    % export JAVA_HOME
    % export LD_LIBRARY_PATH

*   JDK 1.6 32비트 버전을 설치하고, bash 셸에서 환경 변수를 설정한 예 ::

    % JAVA_HOME=/usr/java/jdk1.6.0_10
    % LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/i386/:$JAVA_HOME/jre/lib/i386/client:$LD_LIBRARY_PATH
    % export JAVA_HOME
    % export LD_LIBRARY_PATH

*   JDK 1.6 64비트 버전을 설치하고, csh 셸에서 환경 변수를 설정한 예 ::

    % setenv JAVA_HOME /usr/java/jdk1.6.0_10
    % setenv LD_LIBRARY_PATH $JAVA_HOME/jre/lib/amd64:$JAVA_HOME/jre/lib/amd64/server:$LD_LIBRARY_PATH
    % set path=($path $JAVA_HOME/bin .)

*   JDK 1.6 32비트 버전을 설치하고, csh 셸에서 환경 변수를 설정한 예 ::

    % setenv JAVA_HOME /usr/java/jdk1.6.0_10
    % setenv LD_LIBRARY_PATH $JAVA_HOME/jre/lib/i386:$JAVA_HOME/jre/lib/i386/client:$LD_LIBRARY_PATH
    % set path=($path $JAVA_HOME/bin .)

SUN 이외의 다른 벤더가 제공하는 Java 가상 머신을 사용하는 경우, Library Path에 Java VM( **libjvm.so** )이 있는 경로를 추가해 주어야 한다. 이때, **libjvm.so** 파일의 경로는 OS 플랫폼, 지원 비트마다 다르므로 주의하여 설정한다. 예를 들어 SUN Sparc 머신에서 **libjvm.so** 파일의 경로는 **$JAVA_HOME/jre/lib/sparc** 이다.

함수/프로시저 작성
==================

다음은 Java 저장 함수/프로시저를 작성하는 예이다.

cubrid.conf 확인
----------------

**cubrid.conf** 에 있는 **java_stored_procedure** 의 설정값은 **no** 가 기본이다. Java 저장함수/프로시저를 사용하기 위해서는 이 값을 **yes** 로 변경해야 한다. 이 값과 관련한 자세한 설명은 데이터베이스 서버 설정의 `기타 파라미터 <#pm_pm_db_classify_etc_htm>`_ 를 참조한다.

Java 소스 작성 및 컴파일
------------------------

다음과 같이 SpCubrid.java를 컴파일 한다.

.. code-block:: java

    public class SpCubrid{
        public static String HelloCubrid() {
            return "Hello, Cubrid !!";
        }
        
        public static int SpInt(int i) {
            return i + 1;
        }
        
        public static void outTest(String[] o) {
            o[0] = "Hello, CUBRID";
        }
    }

::

    javac SpCubrid.java

이 때, Java 클래스의 메서드는 반드시 public static이어야 한다.

컴파일된 Java 클래스 로드
-------------------------

컴파일된 Java 클래스를 CUBRID로 로딩한다. ::

    % loadjava demodb SpCubrid.class

로딩한 Java 클래스 등록
-----------------------

다음과 같이 CUBRID 저장 함수를 생성하여 Java 클래스를 등록한다.

.. code-block:: sql

    CREATE FUNCTION hello() RETURN STRING 
    AS LANGUAGE JAVA 
    NAME 'SpCubrid.HelloCubrid() return java.lang.String';

.. CREATE OR REPLACE FUNCTION is allowed from 10.0: CUBRIDSUS-6542

또는 **OR REPLACE** 구문을 사용하여 현재의 저장 함수/프로시저를 대체 혹은 새로 생성하는 문장을 작성할 수 있다.

.. code-block:: java

    CREATE OR REPLACE FUNCTION hello() RETURN STRING
    AS LANGUAGE JAVA
    NAME 'SpCubrid.HelloCubrid() return java.lang.String';    

Java 저장 함수/프로시저 호출
----------------------------

다음과 같이 등록된 Java 저장 함수를 호출한다.

.. code-block:: sql

    CALL hello() INTO :Hello;

::

      Result
    ======================
    'Hello, Cubrid !!'

서버 내부 JDBC 드라이버 사용
============================

Java 저장 함수/프로시저에서 데이터베이스에 접근하기 위해서는 서버 측 JDBC 드라이버(Server-Side JDBC Driver)를 사용해야 한다. Java 저장 함수/프로시저가 데이터베이스 내에서 실행되기 때문에 서버 측 JDBC 드라이버는 다시 연결을 설정할 필요가 없다. 서버 측 JDBC 드라이버로 해당 데이터베이스의 Connection을 얻는 방법은 아래와 같다. 첫 번째 방법은 JDBC 연결 URL로 "**jdbc:default:connection:**" 을 사용하는 것이고, 두 번째는 **cubrid.jdbc.driver.CUBRIDDriver** 클래스의 **getDefaultConnection** () 메서드를 호출하는 것이다.

.. code-block:: java

    Class.forName("cubrid.jdbc.driver.CUBRIDDriver");
    Connection conn = DriverManager.getConnection("jdbc:default:connection:");

또는

.. code-block:: java

    cubrid.jdbc.driver.CUBRIDDriver.getDefaultConnection();

서버 측 JDBC Driver에서 위와 같은 방법으로 데이터베이스에 연결하면 Java 저장 함수/프로시저 내에 존재하는 트랜잭션 관련 사항이 무시된다. 즉, Java 저장 함수/프로시저에서 수행되는 데이터베이스 연산은 Java 저장 함수/프로시저를 호출한 트랜잭션에 포함된다는 것을 의미한다. 아래의 Athlete 클래스에서 conn.commit()은 무시된다.

.. code-block:: java

    import java.sql.*;

    public class Athlete{
        public static void Athlete(String name, String gender, String nation_code, String event) throws SQLException{
            String sql="INSERT INTO ATHLETE(NAME, GENDER, NATION_CODE, EVENT)" + "VALUES (?, ?, ?, ?)";
            
            try{
                Class.forName("cubrid.jdbc.driver.CUBRIDDriver");
                Connection conn = DriverManager.getConnection("jdbc:default:connection:");
                PreparedStatement pstmt = conn.prepareStatement(sql);
           
                pstmt.setString(1, name);
                pstmt.setString(2, gender);
                pstmt.setString(3, nation_code);
                pstmt.setString(4, event);;
                pstmt.executeUpdate();
     
                pstmt.close();
                conn.commit();
                conn.close();
            } catch (Exception e) {
                System.err.println(e.getMessage());
            }
        }
    }

다른 데이터베이스 연결
======================

서버 측 JDBC 드라이버를 사용하더라도 현재 연결된 데이터베이스를 사용하지 않고, 외부의 다른 데이터베이스에 연결할 수도 있다. 외부의 데이터베이스에 대한 Connection을 얻는 것은 일반적인 JDBC Connection과 다르지 않다. 이에 대한 자세한 내용은 JDBC API를 참조한다.

다른 데이터베이스에 연결하는 경우, Java 메서드의 수행이 종료되더라도 CUBRID 데이터베이스와의 Connection이 자동으로 종료되지 않는다. 따라서, 반드시 Connection 종료를 명시해주어야 **COMMIT**, **ROLLBACK** 과 같은 트랜잭션 연산이 해당 데이터베이스에 반영된다. 즉, Java 저장 함수/프로시저를 호출한 데이터베이스와 실제 연결된 데이터베이스가 다르기 때문에 별도의 트랜잭션으로 수행되는 것이다.

.. code-block:: java

    import java.sql.*;

    public class SelectData {
        public static void SearchSubway(String[] args) throws Exception {

            Connection conn = null;
            Statement stmt = null;
            ResultSet rs = null;

            try {
                Class.forName("cubrid.jdbc.driver.CUBRIDDriver");
                conn = DriverManager.getConnection("jdbc:CUBRID:localhost:33000:demodb:::","","");

                String sql = "select line_id, line from line";
                stmt = conn.createStatement();
                rs = stmt.executeQuery(sql);
                
                while(rs.next()) {
                    int host_year = rs.getString("host_year");
                    String host_nation = rs.getString("host_nation");
                    
                    System.out.println("Host Year ==> " + host_year);
                    System.out.println(" Host Nation==> " + host_nation);
                    System.out.println("\n=========\n");
                }
                
                rs.close();
                stmt.close();
                conn.close();
            } catch ( SQLException e ) {
                System.err.println(e.getMessage());
            } catch ( Exception e ) {
                System.err.println(e.getMessage());
            } finally {
                if ( conn != null ) conn.close();
            }
        }
    }

수행 중인 Java 저장 함수/프로시저가 데이터베이스 서버의 JVM에서만 구동되어야 할 때, Java 프로그램 소스에서 System.getProperty("cubrid.server.version")를 호출함으로써 어디서 수행되는 지를 점검할 수 있다. 결과 값은 데이터베이스에서 호출하면 데이터베이스 버전이 되고, 그 외는 **NULL** 이 된다.

loadjava 유틸리티
=================

컴파일된 Java 파일이나 JAR(Java Archive) 파일을 CUBRID로 로드하기 위해서 **loadjava** 유틸리티를 사용한다. **loadjava** 유틸리티를 사용하여 Java \*.class 파일이나 \*.jar 파일을 로드하면 해당 파일이 해당 데이터베이스 경로로 이동한다. ::

    loadjava [option] database-name java-class-file

*   *database-name*: Java 파일을 로드하려고 하는 데이터베이스 이름
*   *java-class-file*: 로드하려는 Java 클래스 파일 이름 또는 jar 파일 이름
*   [*option*]

    *   **-y**: 이름이 같은 클래스 파일이 존재하면 자동으로 덮어쓰기 한다. 기본값은 **no** 이다. 만약 **-y** 옵션을 명시하지 않고 로드할 때 이름이 같은 클래스 파일이 존재하면 덮어쓰기를 할 것인지 묻는다.

로딩한 Java 클래스 등록
=======================

CUBRID는 클라이언트나 SQL 문이나 Java 응용 프로그램에서 Java 메서드를 호출할 수 있도록 Java 클래스를 등록(publish)하는 과정이 필요하다. Java 클래스를 로딩했을 때 SQL 문이나 Java 응용 프로그램에서 클래스 내의 함수를 어떻게 호출할지 모르기 때문에 Call Specifications를 사용하여 등록해야 한다.

Call Specifications
-------------------

CUBRID에서 Java 저장 함수/프로시저를 사용하기 위해서는 Call Specifications를 작성해야 한다. Call Specifications는 Java 함수 이름과 인자 타입 그리고 리턴 값과 리턴 값의 타입을 SQL 문이나 Java 응용프로그램에서 접근할 수 있도록 해주는 역할을 한다. Call Specifications를 작성하는 구문은 **CREATE FUNCTION** 또는 **CREATE PROCEDURE** 구문을 사용하여 작성한다. Java 저장 함수/프로시저의 이름은 대소문자를 구별하지 않는다. Java 저장 함수/프로시저 이름의 최대 길이는 254바이트이다. 또한 하나의 Java 저장 함수/프로시저가 가질 수 있는 인자의 최대 개수는 64개이다. 

리턴 값이 있으면 함수, 없으면 프로시저로 구분한다.

.. CREATE OR REPLACE FUNCTION is allowed from 10.0: CUBRIDSUS-6542

::

    CREATE [OR REPLACE] FUNCTION function_name[(param [COMMENT 'param_comment_string'] [, param [COMMENT 'param_comment_string']]...)] RETURN sql_type
    {IS | AS} LANGUAGE JAVA
    NAME 'method_fullname (java_type_fullname [,java_type_fullname]...) [return java_type_fullname]'
    COMMENT 'sp_comment';

    CREATE [OR REPLACE] PROCEDURE procedure_name[(param [COMMENT 'param_comment_string'][, param [COMMENT 'param_comment_string']] ...)]
    {IS | AS} LANGUAGE JAVA
    NAME 'method_fullname (java_type_fullname [,java_type_fullname]...) [return java_type_fullname]';
    COMMENT 'sp_comment_string';

    parameter_name [IN|OUT|IN OUT|INOUT] sql_type
       (default IN)

*   *param_comment_string*: 인자 커멘트 문자열을 지정한다.
*   *sp_comment_string*: 자바 저장 함수/프로시저의 커멘트 문자열을 지정한다.

Java 저장 함수/프로시저의 인자를 **OUT** 으로 설정한 경우 길이가 1인 1차원 배열로 전달된다. 그러므로 Java 메서드는 배열의 첫번째 공간에 전달할 값을 저장해야 한다.

.. code-block:: sql

    CREATE FUNCTION Hello() RETURN VARCHAR
    AS LANGUAGE JAVA
    NAME 'SpCubrid.HelloCubrid() return java.lang.String';

    CREATE FUNCTION Sp_int(i int) RETURN int
    AS LANGUAGE JAVA
    NAME 'SpCubrid.SpInt(int) return int';

    CREATE PROCEDURE Athlete_Add(name varchar,gender varchar, nation_code varchar, event varchar)
    AS LANGUAGE JAVA
    NAME 'Athlete.Athlete(java.lang.String, java.lang.String, java.lang.String, java.lang.String)';

    CREATE PROCEDURE test_out(x OUT STRING)
    AS LANGUAGE JAVA
    NAME 'SpCubrid.outTest(java.lang.String[] o)';

Java 저장 함수/프로시저를 등록할 때, Java 저장 함수/프로시저의 반환 정의와 Java 파일의 선언부의 반환 정의가 일치하는지에 대해서는 검사하지 않는다. 따라서, Java 저장 함수/프로시저의 경우 등록할 때의 *sql_type* 반환 정의를 따르고, Java 파일 선언부의 반환 정의는 사용자 정의 정보로서만 의미를 가지게 된다.

데이터 타입 매핑
----------------

Call Specifications에는 SQL의 데이터 타입과 Java의 매개변수와 리턴 값의 데이터 타입이 맞게 대응되어야 한다. CUBRID에서 허용되는 SQL과 Java의 데이터 타입의 관계는 다음의 표와 같다.

**데이터 타입 매핑**

+-----------------+------------------------------------------------------------------------------------------------------------------------------------------+
| SQL Type        | Java Type                                                                                                                                |
+=================+==========================================================================================================================================+
| CHAR, VARCHAR   | java.lang.String, java.sql.Date, java.sql.Time, java.sql.Timestamp, java.lang.Byte, java.lang.Short, java.lang.Integer, java.lang.Long,  |
|                 | java.lang.Float, java.lang.Double, java.math.BigDecimal, byte, short, int, long, float, double                                           |
+-----------------+------------------------------------------------------------------------------------------------------------------------------------------+
| NUMERIC, SHORT, | java.lang.Byte, java.lang.Short, java.lang.Integer, java.lang.Long, java.lang.Float, java.lang.Double, java.math.BigDecimal,             |
| INT, FLOAT,     | java.lang.String, byte, short, int, long, float, double                                                                                  |
| DOUBLE,         |                                                                                                                                          |
| CURRENCY        |                                                                                                                                          |
+-----------------+------------------------------------------------------------------------------------------------------------------------------------------+
| DATE, TIME,     | java.sql.Date, java.sql.Time, java.sql.Timestamp, java.lang.String                                                                       |
| TIMESTAMP       |                                                                                                                                          |
+-----------------+------------------------------------------------------------------------------------------------------------------------------------------+
| SET, MULTISET,  | java.lang.Object[], java primitive type array, java.lang.Integer[] ...                                                                   |
| SEQUENCE        |                                                                                                                                          |
+-----------------+------------------------------------------------------------------------------------------------------------------------------------------+
| OBJECT          | cubrid.sql.CUBRIDOID                                                                                                                     |
+-----------------+------------------------------------------------------------------------------------------------------------------------------------------+
| CURSOR          | cubrid.jdbc.driver.CUBRIDResultSet                                                                                                       |
+-----------------+------------------------------------------------------------------------------------------------------------------------------------------+

등록된 Java 저장 함수/프로시저의 정보 확인
------------------------------------------

등록된 Java 저장 함수/프로시저의 정보는 **db_stored_procedure** 시스템 가상 클래스와 **db_stored_procedure_args** 시스템 가상 클래스에서 확인할 수 있다. **db_stored_procedure** 시스템 가상 클래스에서는 저장 함수/프로시저의 이름과 타입, 반환 타입, 인자의 수, Java 클래스에 대한 명세, Java 저장 함수/프로시저의 소유자에 대한 정보를 확인할 수 있다. **db_stored_procedure_args** 시스템 가상 클래스에서는 저장 함수/프로시저에서 사용하는 인자에 대한 정보를 확인할 수 있다.

.. code-block:: sql

    SELECT * FROM db_stored_procedure;
    
::
    
    sp_name     sp_type   return_type    arg_count
    sp_name               sp_type               return_type             arg_count  lang target                owner
    ================================================================================
    'hello'               'FUNCTION'            'STRING'                        0  'JAVA''SpCubrid.HelloCubrid() return java.lang.String'  'DBA'
     
    'sp_int'              'FUNCTION'            'INTEGER'                       1  'JAVA''SpCubrid.SpInt(int) return int'  'DBA'
     
    'athlete_add'         'PROCEDURE'           'void'                          4  'JAVA''Athlete.Athlete(java.lang.String, java.lang.String, java.lang.String, java.lang.String)'  'DBA'

.. code-block:: sql
    
    SELECT * FROM db_stored_procedure_args;
    
::
    
    sp_name   index_of  arg_name  data_type      mode
    =================================================
     'sp_int'                        0  'i'                   'INTEGER'             'IN'
     'athlete_add'                   0  'name'                'STRING'              'IN'
     'athlete_add'                   1  'gender'              'STRING'              'IN'
     'athlete_add'                   2  'nation_code'         'STRING'              'IN'
     'athlete_add'                   3  'event'               'STRING'              'IN'

Java 저장 함수/프로시저의 삭제 
------------------------------

CUBRID에서는 등록한 Java 함수/저장 프로시저를 삭제할 수 있다. **DROP FUNCTION** *function_name* 또는 **DROP PROCEDURE** *procedure_name* 구문을 사용하여 Java 저장 프로시저를 삭제할 수 있다. 또한 여러 개의 *function_name* 이나 *procedure_name* 을 콤마(,)로 구분하여 한꺼번에 여러 개의 Java 저장 함수/프로시저를 삭제할 수 있다.

Java 저장 함수/프로시저의 삭제는 Java 저장 함수/프로시저를 등록한 사용자와 DBA의 구성원만 삭제할 수 있다. 예를 들어 'sp_int' Java 저장 함수를 **PUBLIC** 이 등록했다면, **PUBLIC** 또는 **DBA** 의 구성원만이 'sp_int' Java 저장 함수를 삭제할 수 있다.

.. code-block:: sql

    DROP FUNCTION hello, sp_int;
    DROP PROCEDURE Athlete_Add;

Java 저장 함수/프로시저의 커멘트
--------------------------------

저장 함수 또는 프로시저의 커멘트를 다음과 같이 제일 뒤에 지정할 수 있다. 

.. code-block:: sql


    CREATE FUNCTION Hello() RETURN VARCHAR
    AS LANGUAGE JAVA
    NAME 'SpCubrid.HelloCubrid() return java.lang.String'
    COMMENT 'function comment';

저장 함수의 인자 뒤에는 다음과 같이 지정할 수 있다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION test(i in number COMMENT 'arg i') 
    RETURN NUMBER AS LANGUAGE JAVA NAME 'SpTest.testInt(int) return int' COMMENT 'function test';

저장 함수 또는 프로시저의 커멘트는 다음 구문을 실행하여 확인할 수 있다.

.. code-block:: sql

    SELECT sp_name, comment FROM db_stored_procedure; 

함수 인자의 커멘트는 다음 구문을 실행하여 확인할 수 있다.

.. code-block:: sql
          
    SELECT sp_name, arg_name, comment FROM db_stored_procedure_args;

Java 저장 함수/프로시저 호출
============================

CALL 문
-------

등록된 Java 저장 함수/프로시저는 **CALL** 문을 사용하거나, SQL 문에서 호출하거나, Java 응용프로그램에서 호출될 수 있다. 다음과 같이 **CALL** 문을 사용하여 호출할 수 있다. **CALL** 문에서 호출되는 Java 저장 함수/프로시저의 이름은 대소문자를 구분하지 않는다. ::

    CALL {procedure_name ([param[, param]...]) | function_name ([param[, param]...]) INTO :host_variable
    param {literal | :host_variable}

.. code-block:: sql

    CALL Hello() INTO :HELLO;
    CALL Sp_int(3) INTO :i;
    CALL phone_info('Tom','016-111-1111');

CUBRID에서는 Java 저장 함수/프로시저를 같은 **CALL** 문을 이용해 호출한다. 따라서 다음과 같이 **CALL** 문을 처리하게 된다.

*   **CALL** 문에 대상 클래스가 있는 경우 메서드로 처리한다.
*   **CALL** 문에 대상 클래스가 없는 경우 먼저 Java 저장 함수/프로시저 수행 여부를 검사하고 Java 저장 함수/프로시저가 존재하면 Java 저장 함수/프로시저를 수행한다.
*   2에서 Java 저장 함수/프로시저가 존재하지 않으면 메서드 수행 여부를 검사하여 같은 이름이 존재하면 수행한다.

만약 존재하지 않는 Java 저장 함수/프로시저를 호출하는 경우에는 다음과 같은 에러가 나타난다.

.. code-block:: sql

    CALL deposit();
    
::

    ERROR: Stored procedure/function 'deposit' does not exist.

.. code-block:: sql

    CALL deposit('Tom', 3000000);
    
::

    ERROR: Methods require an object as their target.

**CALL** 문에 인자가 없는 경우는 메서드와 구분되므로 "ERROR: Stored procedure/function 'deposit' does not exist."라는 오류 메시지가 나타난다. 하지만, **CALL** 문에 인자가 있는 경우에는 메서드와 구분할 수 없기 때문에 "ERROR: Methods require an object as their target."이라는 메시지가 나타난다.

그리고, 아래와 같이 Java 저장 함수/프로시저를 호출하는 **CALL** 문 안에 **CALL** 문이 중첩되는 경우와 **CALL** 문을 사용하여 Java 저장 함수/프로시저 호출 시 인자로 서브 질의를 사용할 경우 **CALL** 문은 수행이 되지 않는다.

.. code-block:: sql

    CALL phone_info('Tom', CALL sp_int(999));
    CALL phone_info((SELECT * FROM Phone WHERE id='Tom'));

Java 저장 함수/프로시저를 호출하여 수행 중 exception이 발생하면 *dbname*\ **_java.log** 파일에 exception 내용이 기록되어 저장된다. 만약 화면으로 exception 내용을 확인하고자 할 경우는 **$CUBRID/java/logging.properties** 파일의 handlers 값을 " java.lang.logging.ConsoleHandler" 로 수정하면 화면으로 exception 내용을 출력한다.

SQL 문에서 호출
---------------

다음과 같이 SQL 문에서 Java 저장 함수를 호출하여 사용할 수 있다.

.. code-block:: sql

    SELECT Hello() FROM db_root;
    SELECT sp_int(99) FROM db_root;

Java 저장 함수/프로시저를 호출할 때 IN/OUT의 데이터 타입에 호스트 변수를 사용할 수 있으며, 사용 예는 다음과 같다.

.. code-block:: sql

    SELECT 'Hi' INTO :out_data FROM db_root;
    CALL test_out(:out_data);
    SELECT :out_data FROM db_root;

첫 번째 문장은 파라미터 변수를 이용하여 out 모드의 Java 저장 프로시저를 호출하는 예이고, 두 번째 문장은 할당된 호스트 변수 out_data를 조회하는 질의문이다.

Java 응용 프로그램에서 호출
---------------------------

Java 응용 프로그램에서 Java 저장 함수/프로시저를 호출하기 위해서는 **CallableStatement** 를 사용한다.

CUBRID 데이터베이스에 Phone 클래스를 생성한다.

.. code-block:: sql

    CREATE TABLE phone(
         name VARCHAR(20),
         phoneno VARCHAR(20)
    );

다음의 PhoneNumber.java Java 파일을 컴파일하여 Java 클래스 파일을 CUBRID로 로드하고 등록한다.

.. code-block:: java

    import java.sql.*;
    import java.io.*;

    public class PhoneNumber{
        public static void Phone(String name, String phoneno) throws Exception{
            String sql="INSERT INTO PHONE(NAME, PHONENO)"+ "VALUES (?, ?)";
            try{
                Class.forName("cubrid.jdbc.driver.CUBRIDDriver");
                Connection conn = DriverManager.getConnection("jdbc:default:connection:");
                PreparedStatement pstmt = conn.prepareStatement(sql);
           
                pstmt.setString(1, name);
                pstmt.setString(2, phoneno);
                pstmt.executeUpdate();

                pstmt.close();
                conn.commit();
                conn.close();
            } catch (SQLException e) {
                System.err.println(e.getMessage());
            }
        }
    }

.. code-block:: sql

    create PROCEDURE phone_info(name varchar, phoneno varchar) as language java    
    name 'PhoneNumber.Phone(java.lang.String, java.lang.String)';

다음과 같은 Java 응용 프로그램을 작성하고 실행한다.

.. code-block:: java

    import java.sql.*;

    public class StoredJDBC{
        public static void main(){
            Connection conn = null;
            Statement stmt= null;
            int result;
            int i;

            try{
                Class.forName("cubrid.jdbc.driver.CUBRIDDriver");
                conn = DriverManager.getConnection("jdbc:CUBRID:localhost:33000:demodb:::","","");

                CallableStatement cs;
                cs = conn.prepareCall("CALL PHONE_INFO(?, ?)");

                cs.setString(1, "Jane");
                cs.setString(2, "010-1111-1111");
                cs.executeUpdate();

                conn.commit();
                cs.close();
                conn.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

위의 프로그램 실행한 후 PHONE 클래스 조회를 하면 다음과 같은 결과가 출력된다.

.. code-block:: sql

    SELECT * FROM phone;
    
::

    name                  phoneno
    ============================================
        'Jane'                '010-111-1111'

주의 사항
=========

Java 저장 함수/프로시저의 리턴 값 및 IN/OUT에 대한 타입 자릿수
--------------------------------------------------------------

Java 저장 함수/프로시저의 리턴 값과 IN/OUT의 데이터 타입에 자릿수를 한정하는 경우, CUBRID에서는 다음과 같이 처리한다.

*   Java 저장 함수/프로시저의 sql_type을 기준으로 확인한다.

*   Java 저장 함수/프로시저 생성 시 정의한 자릿수는 무시하고 타입만 맞추어 Java에서 반환하는 값을 그대로 데이터베이스에 전달한다. 전달한 데이터에 대한 조작은 사용자가 데이터베이스에서 직접 처리하는 것을 원칙으로 한다.

다음과 같은 **typestring** () Java 저장 함수를 살펴보자.

.. code-block:: java

    public class JavaSP1{
        public static String typestring(){
            String temp = " ";
            for(int i=0 i< 1 i++)
                temp = temp + "1234567890";
            return temp;
        }
    }

.. code-block:: sql

    CREATE FUNCTION typestring() RETURN CHAR(5) AS LANGUAGE JAVA
    NAME 'JavaSP1.typestring() return java.lang.String';

    CALL typestring();
    
::

      Result
    ======================
      ' 1234567890'

Java 저장 프로시저에서의 java.sql.ResultSet 반환
------------------------------------------------

CUBRID에서는 **java.sql.ResultSet** 을 반환하는 Java 저장 함수/프로시저를 선언할 때는 데이터 타입으로 **CURSOR** 를 사용해야 한다.

.. code-block:: sql

    CREATE FUNCTION rset() RETURN CURSOR AS LANGUAGE JAVA
    NAME 'JavaSP2.TResultSet() return java.sql.ResultSet'

Java 파일에서는 **java.sql.ResultSet** 을 반환하기 전에 **CUBRIDResultSet** 클래스로 캐스팅 후 **setReturnable** () 메서드를 호출해야 한다.

.. code-block:: java

    import java.sql.Connection;
    import java.sql.DriverManager;
    import java.sql.ResultSet;
    import java.sql.Statement;
     
    import cubrid.jdbc.driver.CUBRIDConnection;
    import cubrid.jdbc.driver.CUBRIDResultSet;

    public class JavaSP2 {
        public static ResultSet TResultSet(){
            try {
                Class.forName("cubrid.jdbc.driver.CUBRIDDriver");
                Connection conn = DriverManager.getConnection("jdbc:default:connection:");
                ((CUBRIDConnection)conn).setCharset("euc_kr");
                    
                String sql = "select * from station";
                Statement stmt=conn.createStatement();
                ResultSet rs = stmt.executeQuery(sql);
                ((CUBRIDResultSet)rs).setReturnable();
                    
                return rs;
            } catch (Exception e) {
                e.printStackTrace();
            }
            
            return null;
        }
    }

호출하는 쪽에서는 **Types.JAVA_OBJECT** 로 OUT 인자를 설정하고 **getObject** () 함수로 가져온 후 **java.sql.ResultSet** 으로 변환(Casting)하여 사용해야 한다. 또한, **java.sql.ResultSet** 은 JDBC의 **CallableStatement** 에서만 사용할 수 있다.

.. code-block:: java

    import java.sql.CallableStatement;
    import java.sql.Connection;
    import java.sql.DriverManager;
    import java.sql.ResultSet;
    import java.sql.Types;
     
    public class TestResultSet{
        public static void main(String[] args) {
            Connection conn = null;
     
            try {
                Class.forName("cubrid.jdbc.driver.CUBRIDDriver");
                conn = DriverManager.getConnection("jdbc:CUBRID:localhost:31001:tdemodb:::","","");
     
                CallableStatement cstmt = conn.prepareCall("?=CALL rset()");
                cstmt.registerOutParameter(1, Types.JAVA_OBJECT);
                cstmt.execute();
                ResultSet rs = (ResultSet) cstmt.getObject(1);
     
                while(rs.next()) {
                    System.out.println(rs.getString(1));
                }
     
                rs.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

**ResultSet** 은 입력 인자로 사용할 수 없으며, 이를 IN 인자로 전달할 경우에는 에러가 발생한다. Java가 아닌 환경에서 **ResultSet** 을 반환하는 함수를 호출할 경우에도 에러가 발생한다.

Java 저장 함수/프로시저에서 Set 타입의 IN/OUT
---------------------------------------------

CUBRID의 Java 저장 함수/프로시저에서 Set 타입이 IN OUT인 경우 Java에서 인자 값을 변경할 경우 변경 값이 전달이 되도록 Set 타입이 OUT 인자로 전달될 때는 2차원 배열로 전달하도록 해야 한다.

.. code-block:: sql

    CREATE PROCEDURE setoid(x in out set, z object) AS LANGUAGE JAVA 
    NAME 'SetOIDTest.SetOID(cubrid.sql.CUBRIDOID[][], cubrid.sql.CUBRIDOID';

.. code-block:: java

    public static void SetOID(cubrid.sql.CUBRID[][] set, cubrid.sql.CUBRIDOID aoid){
        Connection conn=null;
        Statement stmt=null;
        String ret="";
        Vector v = new Vector();

        cubrid.sql.CUBRIDOID[] set1 = set[0];

        try {
            if(set1!=null) {
                int len = set1.length;
                int i = 0;
                
                for (i=0 i< len i++)
                    v.add(set1[i]);
            }
            
            v.add(aoid);
            set[0]=(cubrid.sql.CUBRIDOID[]) v.toArray(new cubrid.sql.CUBRIDOID[]{});
            
        } catch(Exception e) {
            e.printStackTrace();
            System.err.pirntln("SQLException:"+e.getMessage());
        }
    }

Java 저장 함수/프로시저에서 OID 사용
------------------------------------

CUBRID 저장 프로시저에서 OID 타입의 값을 IN/OUT으로 사용할 경우 서버의 값을 전달 받아 사용한다.

.. code-block:: sql

    CREATE PROCEDURE tOID(i inout object, q string) AS LANGUAGE JAVA
    NAME 'OIDtest.tOID(cubrid.sql.CUBRIDOID[], java.lang.String)';

.. code-block:: java

    public static void tOID(CUBRIDOID[] oid, String query)
    {
        Connection conn=null;
        Statement stmt=null;
        String ret="";

        try {
            Class.forName("cubrid.jdbc.driver.CUBRIDDriver");
            conn=DriverManager.getConnection("jdbc:default:connection:");

            conn.setAutoCommit(false);
            stmt = conn.createStatement();
            ResultSet rs = stmt.executeQuery(query);
            System.out.println("query:"+ query);

            while(rs.next()) {
                oid[0]=(CUBRIDOID)rs.getObject(1);
                System.out.println("oid:"+oid[0].getTableName());
            }
            
            stmt.close();
            conn.close();
            
        } catch (SQLException e) {
            e.printStackTrace();
            System.err.println("SQLException:"+e.getMessage());
        } catch (Exception e) {
            e.printStackTrace();
            system.err.println("Exception:"+ e.getMessage());
        }
    }
