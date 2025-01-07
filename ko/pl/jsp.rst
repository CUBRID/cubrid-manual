
:meta-keywords: cubrid jsp, loadjava utility
:meta-description: CUBRID supports to develop stored functions and procedures in Java. Java stored functions/procedures are executed on the JVM (Java Virtual Machine) hosted by CUBRID.

.. _jsp-prerequisites:

기능 사용을 위한 준비
==============================================

Java 저장함수/프로시저를 사용하기 위해서 다음의 사항이 준비되어 있어야 한다.

*   **cubrid.conf**\에 있는 **java_stored_procedure** 값을 **yes** 로 설정해야한다.
*   Java 저장 프로시저/함수를 사용하려는 데이터베이스에 대해 Java 저장 프로시저 서버 (Java SP 서버) 를 시작해야한다.

.. _jsp-system-prm:

cubrid.conf 확인
----------------

**cubrid.conf** 에 있는 **java_stored_procedure** 의 설정값은 **no** 가 기본이다.     
Java 저장함수/프로시저를 사용하기 위해서는 이 값을 **yes** 로 변경해야 한다. 이 값과 관련한 자세한 설명은 데이터베이스 서버 설정의 :ref:`other-parameters` 를 참조한다.

.. _jsp-starting-javasp:

자바 저장 프로시저 서버 구동
-------------------------------

Java 저장 프로시저/함수를 사용하려는 데이터베이스에 대해 Java 저장 프로시저 서버 (Java SP 서버)를 시작해야 한다.

**cubrid javasp** **start** *db_name* 을 실행한다 ::

    % cubrid javasp start demodb

    @ cubrid javasp start: demodb
    ++ cubrid javasp start: success

Java SP 서버가 성공적으로 시작되었는지 다음의 명령어로 확인할 수 있다.

**cubrid javasp** **status** *db_name* 을 실행한다 ::

    % cubrid javasp status demodb

    @ cubrid javasp status: demodb
    Java Stored Procedure Server (demodb, pid 9220, port 38408)
    Java VM arguments :
    -------------------------------------------------
    -Djava.util.logging.config.file=/path/to/CUBRID/java/logging.properties
    -Xrs
    -------------------------------------------------

자바 저장 프로시저 서버에 대한 자세한 내용은 :ref:`cubrid-javasp-server` 와 :ref:`cubrid-javasp-server-config` 을 참고한다.

자바 저장 함수/프로시저 작성과 로드
=======================================

자바 저장 함수/프로시저를 사용하기 위해서는 다음의 순서에 따라 자바 저장 함수/프로시저를 작성하고 등록한다.

    *   **Step 1: Java 소스 작성**
    *   **Step 2: Java 소스 컴파일**
    *   **Step 3: Java 클래스 로드**
    *   **Step 4: 저장 함수/프로시저 등록**

Java 소스 작성
------------------

다음은 Java 저장 함수/프로시저를 작성하는 예이다.
구현하는 Java 클래스의 메서드는 반드시 **public static**\이어야 한다.

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

Java 저장 함수/프로시저에서 데이터베이스에 접근하기 위해서는 서버 측 JDBC 드라이버를 사용해야한다.
:ref:`jsp-server-side-jdbc`\를 참조하여 서버 측 JDBC 드라이버 사용한다.

Java 소스 컴파일
------------------

다음과 같이 SpCubrid.java를 컴파일 한다.

::

    javac SpCubrid.java

서버 측 JDBC 드라이버를 사용하는 경우 **classpath(cp)** 옵션을 사용하여 JDBC의 경로를 지정하여 다음과 같이 컴파일 해야한다.
이때 로드 할 데이터베이스 서버의 최신 JDBC 드라이버를 사용해야 한다.

::

    javac SpCubrid.java -cp $CUBRID/jdbc/cubrid_jdbc.jar

.. _jsp-loadjava:

Java 클래스 로드
-------------------

컴파일된 Java 클래스를 loadjava 유틸리티를 사용해 CUBRID로 로딩한다.
:ref:`jsp-load-java`\를 참조하여 사용한다.

::

    % loadjava demodb SpCubrid.class


저장 함수/프로시저 등록
-----------------------

CUBRID는 SQL 문이나 Java 응용 프로그램에서 Java 메서드를 호출할 수 있도록 Java 클래스를 등록(publish)하는 과정이 필요하다.
다음과 같이 CUBRID 저장 함수를 생성하여 Java 클래스를 등록한다.
자세한 내용은 :ref:`call-specification`\을 참조한다.

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
============================

등록된 Java 저장 함수/프로시저는 **CALL** 문을 사용하거나, SQL 문에서 호출하거나, Java 응용프로그램에서 호출될 수 있다.
Java 저장 함수/프로시저를 호출하여 수행 중 exception이 발생하면 *dbname*\ **_java.log** 파일에 exception 내용이 기록되어 저장된다. 만약 화면으로 exception 내용을 확인하고자 할 경우는 **$CUBRID/java/logging.properties** 파일의 handlers 값을 "java.lang.logging.ConsoleHandler"로 수정하면 화면으로 exception 내용을 출력한다.

CALL 문
-------

CALL 문으로 다음과 같이 Java 저장 프로시저/함수를 호출하여 사용할 수 있다. 
자세한 내용은 :doc:`/sql/query/call`\을 참조한다.

.. code-block:: sql

    CALL Hello() INTO :HELLO;

::

      Result
    ======================
    'Hello, Cubrid !!'

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
        public static void Phone(String name, String phoneno) throws Exception {
            String sql="INSERT INTO PHONE(NAME, PHONENO)"+ "VALUES (?, ?)";
            try{
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

    public class StoredJDBC {
        public static void main() {
            Connection conn = null;
            Statement stmt= null;
            int result;
            int i;

            try{
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

.. _jsp-server-side-jdbc:

서버 내부 JDBC 드라이버 사용
============================

Java 저장 함수/프로시저에서 데이터베이스에 접근하기 위해서는 서버 측 JDBC 드라이버(Server-Side JDBC Driver)를 사용해야 한다.
서버 측 JDBC 드라이버로 다음의 기능이 가능하다.

*    **질의문 수행**
*    **질의 결과셋 처리**

다음은 서버측 JDBC 드라이버에서 지원하는 클래스이다. JDBC API 지원에 대한 자세한 내용은 :ref:`jsp-appendix`\를 참고한다.

*    **java.sql.Connection**
*    **java.sql.Statement**
*    **java.sql.PreparedStatement**
*    **java.sql.CallableStatement**
*    **java.sql.ResultSet**
*    **java.sql.ResultSetMetaData**

.. warning::

    **java.sql.DatabaseMetaData** 의 기능은 지원하지 않는다.

서버측 JDBC 드라이버를 사용하는 데이터베이스 연산은 다음의 특징을 가진다.

*    수행되는 데이터베이스 연산은 Java 저장 함수/프로시저를 호출한 트랜잭션에 포함된다
*    트랜잭션 관련 API는 무시된다.
*    서버 측 JDBC 드라이버의 연결을 다시 설정할 필요가 없다

.. _jsp-server-side-jdbc-connection:

Connection 생성
----------------

데이터베이스에 접근하기 위해서 서버 측 JDBC Connection을 생성해야한다.
서버 측 JDBC 드라이버로 해당 데이터베이스의 Connection을 얻는 방법은 아래와 같이
JDBC 연결 URL로 "**jdbc:default:connection:**" 을 사용한다.

.. code-block:: java

    Connection conn = DriverManager.getConnection("jdbc:default:connection:");

.. note::

    CUBRID 11.2 버전부터 다음과 같은 방법으로 Connection을 얻는 방법은 더이상 지원하지 않는다.

        Connection conn = cubrid.jdbc.driver.CUBRIDDriver.getDefaultConnection();

.. note::

    서버 측 JDBC 드라이버는 이미 등록되어 있기 때문에 Class.forName("cubrid.jdbc.driver.CUBRIDDriver")\를 호출하지 않아도 된다

.. _jsp-execute-statement:

질의문 수행
--------------

자바 저장/프로시저를 구현할 때 자바 어플리케이션을 개발하는 것과 동일하게 다음의 JDBC 인터페이스를 이용하여 질의문을 수행할 수 있다.

    *    **java.sql.Statement**
    *    **java.sql.PreparedStatement**
    *    **java.sql.CallableStatement**

다음은 위의 클래스를 사용하여 수행할 수 있는 질의문이다.

*   **DML (Data Manipulation Language)**: :doc:`/sql/query/index`
*   **DDL (Data Definition Language)**: :doc:`/sql/schema/index`

.. note::

    질의를 수행할 때 생성하는 JDBC 객체는 하나의 SQL 구문만 포함해야한다.
    따라서 다음의 경우에 에러가 발생한다.
    
    ::

        stmt = new Statement ("select * from t1;select * from t2;");

다음의 구문에 해당하는 기능은 지원하지 않는다.

*   **TCL (Transaction Control Language)**: :ref:`database-transaction`

.. note::

    *    **COMMIT**, **ROLLBACK** 구문에 해당하는 함수인 *commit()*, *rollback()*\은 무시된다.
    *    **SAVEPOINT** 구문에 해당하는 함수는 지원하지 않는다.

질의문 수행 예시
^^^^^^^^^^^^^^^^^

**결과셋을 반환하는 질의 수행과 질의 결과셋 처리**

다음 예시는 결과셋을 반환하는 **SELECT** 문을 실행하는 방법이다.
**SELECT** 문은 **java.sql.Statement** 또는 **java.sql.PreparedStatement** 객체를 생성하여 수행할 수 있다.
수행한 질의 결과셋 (**java.sql.ResultSet**) 을 사용하여 수행한 질의의 결과를 처리할 수 있다.

.. note::

    *    java.sql.ResultSet은 forward-only, read-only 이다.
    *    클라이언트 측 JDBC 드라이버의 경우 질의 결과셋을 생성하면 기본적으로 :ref:`커서 유지(cursor holdability) <cursor-holding>`\를 한다.
         서버 측 JDBC 드라이버에서는 자원을 서버에서 관리하므로 질의 결과셋은 커서를 유지하지 않고 저장 함수/프로시저 종료 시에 내부적으로 정리한다.

또한 질의 결과셋으로부터 **getMetaData()** 함수를 이용하여 결과셋 메타 데이터(**java.sql.ResultSetMetaData**)를 생성할 수 있다.

.. code-block:: sql

    CREATE OR REPLACE FUNCTION sp_get_athlete_by_ncode (nc STRING) RETURN STRING as language java name 'TestQuery.printAthelete(java.lang.String) return java.lang.String'; 

.. code-block:: java
    
    import java.sql.*;

    public class TestQuery {
        public static String printAthelete(String nation_code_filter) throws SQLException {
            String sql = "SELECT * FROM public.athlete WHERE nation_code = ?";

            StringBuilder builder = new StringBuilder();
            Connection conn = null;
            PreparedStatement pstmt = null;

            try {
                conn = DriverManager.getConnection("jdbc:default:connection:");
                pstmt = conn.prepareStatement(sql);

                pstmt.setString(1, nation_code_filter);

                ResultSet rs = pstmt.executeQuery();
                ResultSetMetaData rsmd = rs.getMetaData();

                builder.append("<Column Details>:\n");
                int colCount = rsmd.getColumnCount();
                for (int i = 1; i <= colCount; i++) {
                    String colName = rsmd.getColumnName(i);
                    String colType = rsmd.getColumnTypeName(i);
                    builder.append(colName + "," + colType);

                    if (i != colCount) builder.append("|");
                }
                
                builder.append("\n<Rows>:\n");
                while (rs.next()) {
                    for (int i = 1; i <= rsmd.getColumnCount(); i++) {
                        Object object = rs.getObject(i);
                        if (object != null) {
                            readColumn(i, rsmd, rs, builder);
                        }
                        
                        if (i != rsmd.getColumnCount()) builder.append ("|");
                    }
                    builder.append("\n");
                }

                rs.close();
            } catch (Exception e) {
                builder.append(e.getMessage());
            } finally {
                if (pstmt != null) pstmt.close();
                if (conn != null) conn.close();
            }

            return builder.toString();
        }

        private static void readColumn(int idx, ResultSetMetaData rsmd, ResultSet rs, StringBuilder stringBuilder) throws SQLException {
            switch (rsmd.getColumnType(idx)) {
                case java.sql.Types.DOUBLE:
                    stringBuilder.append(rs.getDouble(idx));
                    break;
                case java.sql.Types.FLOAT:
                    stringBuilder.append(rs.getFloat(idx));
                    break;
                case java.sql.Types.VARCHAR:
                    stringBuilder.append("\"").append(rs.getString(idx)).append("\"");
                    break;
                case java.sql.Types.INTEGER:
                case java.sql.Types.TINYINT:
                case java.sql.Types.SMALLINT:
                case java.sql.Types.BIGINT:
                    stringBuilder.append(rs.getInt(idx));
                    break;
                case java.sql.Types.DATE:
                    stringBuilder.append("\"").append(rs.getDate(idx)).append("\"");
                    break;
                case java.sql.Types.TIMESTAMP:
                    stringBuilder.append("\"").append(rs.getTimestamp(idx)).append("\"");
                    break;
                default:
                    stringBuilder.append(rs.getObject(idx));
                    break;
            }
        }
    }

.. code-block:: sql

    SELECT sp_get_athlete_by_ncode ('ESP');

    sp_get_athlete_by_ncode('ESP')
    ======================
    '<Column Details>:
    code,INTEGER|name,VARCHAR|gender,CHAR|nation_code,CHAR|event,VARCHAR
    <Rows>:
    10999|"Fernandez Jesus"|M|ESP|"Handball"
    10997|"Fernandez Isabel"|W|ESP|"Judo"
    10994|"Fernandez Abelardo"|M|ESP|"Football"
    10948|"Etxaburu Aitor"|M|ESP|"Handball"
    10941|"Estiarte Manuel"|M|ESP|"Water Polo"
    ...

**INSERT, UPDATE, DELETE**

다음은 **INSERT** 문을 수행하는 예시이다. **INSERT**, **UPDATE**, **DELETE** 문은 **executeUpdate()** 함수를 통해 수행한다.

.. code-block:: java

    import java.sql.*;

    public class Athlete {
        public static void insertAthlete(String name, String gender, String nation_code, String event) throws SQLException {
            String sql = "INSERT INTO ATHLETE(NAME, GENDER, NATION_CODE, EVENT)" + "VALUES (?, ?, ?, ?)";
            
            Connection conn = null;
            PreparedStatement pstmt = null;

            try{
                conn = DriverManager.getConnection("jdbc:default:connection:");
                pstmt = conn.prepareStatement(sql);
           
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
            } finally {
                if (pstmt != null) pstmt.close();
                if (conn != null) conn.close();
            }
        }
    }

.. note::

    위의 Athlete 클래스 예시에서 conn.commit()은 무시된다.

OUT, IN/OUT 정의
---------------------------------------------

CUBRID의 Java 저장 함수/프로시저에서 Java에서 인자 값을 변경할 경우 변경 값이 전달이 되도록 인자가 OUT 인자로 전달될 때는 1차원 배열로 전달해야 한다.

.. code-block:: sql

    CREATE PROCEDURE sp_increment_me(x IN OUT INT) AS LANGUAGE JAVA NAME 'OutTest.incrementInt(int[])';

.. code-block:: java

    public class OutTest {
        public static void incrementInt(int[] arg) {
            arg[0] = arg[0] + 1;
        }
    }

Set 타입의 IN/OUT 정의
---------------------------------------------

CUBRID의 Java 저장 함수/프로시저에서 Set 타입이 IN OUT인 경우 Java에서 인자 값을 변경할 경우 변경 값이 전달이 되도록 Set 타입이 OUT 인자로 전달될 때는 2차원 배열로 전달해야 한다.

.. code-block:: sql

    CREATE PROCEDURE setoid(x in out set, z object) AS LANGUAGE JAVA 
    NAME 'SetOIDTest.SetOID(cubrid.sql.CUBRIDOID[][], cubrid.sql.CUBRIDOID';

.. code-block:: java

    import cubrid.sql.CUBRIDOID;

    public class SetOIDTest {
        public static void SetOID(CUBRIDOID[][] set, CUBRIDOID aoid) {
            String ret="";
            Vector v = new Vector();

            CUBRIDOID[] set1 = set[0];

            try {
                if(set1 != null) {
                    int len = set1.length;
                    int i = 0;
                    
                    for (i = 0; i < len; i++)
                        v.add(set1[i]);
                }
                
                v.add(aoid);
                set[0] = (CUBRIDOID[]) v.toArray(new CUBRIDOID[]{});
                
            } catch(Exception e) {
                e.printStackTrace();
                System.err.println("SQLException:"+e.getMessage());
            }
        }
    }

Java 저장 함수/프로시저에서 OID 사용
------------------------------------

CUBRID 저장 프로시저에서 OID 타입의 값을 IN/OUT으로 사용할 경우 서버의 값을 전달 받아 사용한다.

.. code-block:: sql

    CREATE PROCEDURE tOID(i inout object, q string) AS LANGUAGE JAVA
    NAME 'OIDtest.tOID(cubrid.sql.CUBRIDOID[], java.lang.String)';

.. code-block:: java

    import java.sql.*;
    import cubrid.sql.CUBRIDOID;

    public class OIDTest {
        public static void tOID(CUBRIDOID[] oid, String query)
        {
            Connection conn = null;
            Statement stmt = null;
            String ret = "";

            try {
                conn = DriverManager.getConnection("jdbc:default:connection:");

                conn.setAutoCommit(false);
                stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery(query);
                System.out.println("query:"+ query);

                while(rs.next()) {
                    oid[0] = (CUBRIDOID) rs.getObject(1);
                    System.out.println("oid:" + oid[0].getTableName());
                }
                
                stmt.close();
                conn.close();
                
            } catch (SQLException e1) {
                e1.printStackTrace();
                System.err.println("SQLException:" + e1.getMessage());
            } catch (Exception e2) {
                e2.printStackTrace();
                system.err.println("Exception:" + e2.getMessage());
            }
        }
    }


질의 결과셋 반환
-----------------

CUBRID에서는 질의 결과셋 (**java.sql.ResultSet**)을 반환할 수 있고, 선언 시 반환하는 데이터 타입으로 **CURSOR** 를 사용한다.

.. note::

    *    **java.sql.ResultSet** 은 함수의 입력 인자로 사용할 수 없으며, 이를 IN 인자로 전달할 경우에는 에러가 발생한다.
    *    Java가 아닌 환경에서 **ResultSet** 을 반환하는 함수를 호출할 경우에도 에러가 발생한다.

.. code-block:: sql

    CREATE FUNCTION rset() RETURN CURSOR AS LANGUAGE JAVA
    NAME 'JavaSP2.TResultSet() return java.sql.ResultSet'

.. code-block:: java

    import java.sql.*;

    public class JavaSP2 {
        public static ResultSet TResultSet(){
            try {
                Connection conn = DriverManager.getConnection("jdbc:default:connection:");
                    
                String sql = "select * from station";
                Statement stmt=conn.createStatement();
                ResultSet rs = stmt.executeQuery(sql);
                    
                return rs;
            } catch (Exception e) {
                e.printStackTrace();
            }
            
            return null;
        }
    }

호출하는 쪽에서는 **Types.JAVA_OBJECT** 로 OUT 인자를 설정하고 **getObject** () 함수로 가져온 후 **java.sql.ResultSet** 으로 변환(Casting)하여 사용해야 한다. 또한, **java.sql.ResultSet** 은 JDBC의 **CallableStatement** 에서만 사용할 수 있다.

.. code-block:: java

    import java.sql.*;
     
    public class TestResultSet{
        public static void main(String[] args) {
            Connection conn = null;
     
            try {
                conn = DriverManager.getConnection("jdbc:CUBRID:localhost:33000:demodb:::","","");
     
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

.. _jsp-get-client-info:

연결 중인 클라이언트 정보 획득
--------------------------------

.. code-block:: sql

    CREATE OR REPLACE FUNCTION sp_client_info () RETURN STRING as language java name 'SpTestClientInfo.getClientInfo() return java.lang.String'; 

.. code-block:: java

    import java.util.Properties;
    import java.sql.*;
     
    public class SpTestClientInfo {
        public static String getClientInfo() {
            Connection conn = null;
            String result = "";
     
            try {
                conn = DriverManager.getConnection("jdbc:default:connection:");
     
                Properties props = conn.getClientInfo();

                // How to get from the Properties
                // String user = props.getProperty ("user");

                result = props.toString ();
            } catch (Exception e) {
                result = e.getMessage ();
            }
            return result;
        }
    }

.. code-block:: sql

    SELECT sp_client_info ();

    sp_client_info()
    ======================
    '{pid=200270, user=DBA, login=cubrid, program=csql, type=2, host=cubrid, ip=192.168.2.201}'

다른 데이터베이스 연결
======================

서버 측 JDBC 드라이버를 사용하더라도 현재 연결된 데이터베이스를 사용하지 않고, 외부의 다른 데이터베이스에 연결할 수도 있다. 
외부의 데이터베이스에 대한 Connection을 얻는 것은 일반적인 JDBC Connection과 다르지 않다. 이에 대한 자세한 내용은 JDBC API를 참조한다.

.. warning::

    다른 데이터베이스에 연결하는 경우, Java 메서드의 수행이 종료되더라도 CUBRID 데이터베이스와의 Connection이 자동으로 종료되지 않는다.
    따라서, 반드시 Connection 종료를 명시해주어야 **COMMIT**, **ROLLBACK** 과 같은 트랜잭션 연산이 해당 데이터베이스에 반영된다. 
    즉, Java 저장 함수/프로시저를 호출한 데이터베이스와 실제 연결된 데이터베이스가 다르기 때문에 **별도의 트랜잭션**\으로 수행되는 것이다.

.. code-block:: java

    import java.sql.*;

    public class SelectData {
        public static void SearchSubway(String[] args) throws Exception {
            Connection conn = null;
            Statement stmt = null;
            ResultSet rs = null;

            try {
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
            } catch (SQLException e1) {
                System.err.println(e1.getMessage());
            } catch (Exception e2) {
                System.err.println(e2.getMessage());
            } finally {
                if (stmt != null) stmt.close();
                if (conn != null) conn.close();
            }
        }
    }

수행 중인 Java 저장 함수/프로시저가 데이터베이스 서버의 JVM에서만 구동되어야 할 때, Java 프로그램 소스에서 System.getProperty("cubrid.server.version")를 호출함으로써 어디서 수행되는 지를 점검할 수 있다. 결과 값은 데이터베이스에서 호출하면 데이터베이스 버전이 되고, 그 외는 **NULL** 이 된다.

.. _jsp-jni:

Java Native Interface (JNI) 지원
==================================

Java Native Interface (JNI)를 사용하여 JVM\에서 C/C++ 와 같은 네이티브 언어의 함수를 호출할 수 있다.
CUBRID의 Java SP에서는 JNI 기능을 사용할 수 있도록 제공하고 있지만 네이티브 코드의 문제가 저장 프로시저 서버 (cub_javasp) 프로세스 및 그 동작에 예상하지 못한 영향을 줄 수 있으므로 주의해서 사용해야 한다.
네이티브 라이브러리를 로드하는 클래스는 동적으로 로드되지 않도록 loadjava의 **-j** 옵션을 사용하여 등록한다. 자세한 내용은 :ref:`jsp-load-java`\를 참조한다.

다음은 Java 저장 함수에서 JNI를 통해 네이티브 함수를 호출하는 예제이다.

.. code-block:: cpp
    :caption: HelloJNI.h

    #include <jni.h>
    
    #ifndef _Included_HelloJNI
    #define _Included_HelloJNI
    #ifdef __cplusplus
    extern "C" {
    #endif
    
    /*
    * Class:     HelloJNI
    * Method:    sayHello
    * Signature: ()V
    */
    JNIEXPORT jstring JNICALL Java_HelloJNI_sayHello(JNIEnv *, jobject, jstring);
    
    #ifdef __cplusplus
    }
    #endif
    #endif

.. code-block:: cpp
    :caption: HelloJNI.c

    #include <jni.h>
    #include <stdio.h>
    #include <string.h>
    #include "HelloJNI.h"

    // Implementation of native method sayHello() of HelloJNI class
    JNIEXPORT jstring JNICALL Java_HelloJNI_sayHello(JNIEnv *env, jobject thisObj, jstring javaString) {
        const char *nativeString = (*env)->GetStringUTFChars(env, javaString, 0);
        // printf("Java_HelloJNI_sayHello : %sn", nativeString);
        const char *greeting = " Hello!";
        char cap[1024];
        strcpy(cap, nativeString);
        strcat(cap, greeting);
        (*env)->ReleaseStringUTFChars(env, javaString, nativeString);
        return (*env)->NewStringUTF(env, cap);
    }

.. code-block:: java
    :caption: HelloJNI.java

    import java.io.File;

    public class HelloJNI {
        static {
            try {
                String cubridPath = System.getenv("CUBRID"); // get $CUBRID
                System.load(
                    cubridPath 
                    + File.separator 
                    + "jni" 
                    + File.separator 
                    + "libhello.so"); // $CUBRID/jni/libhello.so
            } catch(UnsatisfiedLinkError e) {
                e.printStackTrace();
            }
        }

        // Declare
        private native String sayHello(String string);

        // CUBRID
        public static String cubridSayHello(String string) {
            return new HelloJNI().sayHello(string); // invoke the native method
        }
    }

.. code-block:: bash

    -- compile and copy HelloJNI.c
    gcc -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -shared -o libhello.so HelloJNI.c
    mkdir -p $CUBRID/jni
    cp libhello.so $CUBRID/jni

    -- loadjava
    javac HelloJNI.java
    loadjava -j demodb HelloJNI.class


.. code-block:: sql

    CREATE FUNCTION hello(str VARCHAR) RETURN VARCHAR AS LANGUAGE JAVA NAME 'HelloJNI.cubridSayHello(java.lang.String) return java.lang.String';
    
    SELECT hello ('CUBRID');

::

    hello('CUBRID')     
    ======================
    'CUBRID Hello!'

.. warning::

    JNI를 호출하는 자바 저장 프로시저/함수를 **-j** 옵션 없이 등록한 후 실행 시 java.lang.UnsatisfiedLinkError가 발생할 수 있다.
    다음의 사항을 확인한다.

    * 동일한 네이티브 라이브러리 경로에 대해 System.load () 를 호출하는 자바 클래스 파일을 여러 개 로드하는 경우
       * 하나의 자바 클래스 파일만 네이티브 라이브러리를 로드하도록 수정한다
       * 네이티브 라이브러리를 로드하는 클래스를 loadjava의 **-j** 옵션을 사용하여 등록한다
       * javasp 유틸리티를 재시작한다

    * 이미 로드된 자바 클래스 파일을 loadjava로 다시 덮어쓰는 경우
       * **-j** 옵션 없이 loadjava로 클래스를 등록한 경우 해당 데이터베이스 경로의 **java** 디렉토리에서 해당 클래스를 제거한다
       * **-j** 옵션을 사용하여 loadjava로 해당 클래스를 다시 등록한다
       * javasp 유틸리티를 재시작한다

.. _jsp-load-java:

loadjava 유틸리티
=================

컴파일된 Java 파일이나 JAR(Java Archive) 파일을 CUBRID로 로드하기 위해서 **loadjava** 유틸리티를 사용한다. **loadjava** 유틸리티를 사용하여 Java \*.class 파일이나 \*.jar 파일을 로드하면 해당 파일이 해당 데이터베이스 경로로 이동한다. ::

    loadjava [option] database-name java-class-file

*   *database-name*: Java 파일을 로드하려고 하는 데이터베이스 이름
*   *java-class-file*: 로드하려는 Java 클래스 파일 이름 또는 jar 파일 이름
*   [*option*]

    *   **-y**: 이름이 같은 클래스 파일이 존재하면 자동으로 덮어쓰기 한다. 기본값은 **no** 이다. 만약 **-y** 옵션을 명시하지 않고 로드할 때 이름이 같은 클래스 파일이 존재하면 덮어쓰기를 할 것인지 묻는다.
    *   **-j(또는 --jin)**: Java \*.class 파일이나 \*.jar 파일을 정적으로 로드하기 위한 경로로 이동한다. 네이티브 라이브러리의 로딩 오류를 방지하기 위해서 JNI(네이티브 라이브러리)를 사용한 class는 반드시 이 옵션을 사용하여 로드해야 한다.

.. _jsp-caution:

주의 사항
=========

* java.sql.DatabaseMetaData는 지원하지 않는다.
* BLOB/CLOB 타입과 관련한 JDBC API 지원하지 않는다.
* 질의 수행과 관련 없고 클라이언트 측 JDBC에서만 사용하는 기능은 지원하지 않는다. 자세한 내용은 :ref:`jsp-appendix`\를 참조한다.
* 하나의 JDBC 객체로 질의 수행 시 여러 SQL 구문을 지원하지 않는다.
* 질의 수행으로 만들어지는 ResultSet은 non-updatable, non-scrollable, non-sensitive이다.
* 리턴 값 및 IN/OUT 파라미터의 타입 자릿수는 Java에서 무시하고 타입만 맞추어 그대로 데이터베이스에 전달한다.
* 저장 프로시저는 다른 저장 프로시저를 호출하거나 재귀적으로 자신을 호출할 수 있다. 최대 중첩 깊이는 16이다.

리턴 값 및 IN/OUT 파라미터에 대한 타입 자릿수 제한사항
--------------------------------------------------------

리턴 값과 IN/OUT의 데이터 타입에 자릿수를 한정하는 경우, 
Java 저장 함수/프로시저 생성 시 정의한 자릿수는 무시하고 타입만 맞추어 Java에서 반환하는 값을 그대로 데이터베이스에 전달한다. 
전달한 데이터에 대한 조작은 사용자가 데이터베이스에서 직접 처리하는 것을 원칙으로 한다.

다음과 같은 **typestring** () Java 저장 함수를 살펴보자.

.. code-block:: java

    public class JavaSP1 {
        public static String typestring() {
            String temp = " ";
            for(int i = 0; i < 1; i++) {
                temp = temp + "1234567890";
            }
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

.. _jsp-appendix:

부록
========================

JDBC API 지원표
----------------------------

=========================== =========================================================
JDBC 인터페이스              지원 여부                                               
=========================== =========================================================
java.sql.CallableStatement  지원                                               
java.sql.Connection         지원                                               
java.sql.Driver             지원 (:ref:`jsp-server-side-jdbc-connection`)
java.sql.PreparedStatement  지원                                               
java.sql.ResultSet          지원                                               
java.sql.ResultSetMetaData  지원                                               
CUBRIDOID                   지원                                               
java.sql.Statement          지원
java.sql.DriverManager      지원                                               
Java.sql.SQLException       지원                                               
java.sql.Array              미지원                                           
java.sql.Blob               미지원                                               
java.sql.Clob               미지원                                               
java.sql.DatabaseMetaData   미지원                                               
java.sql.ParameterMetaData  미지원                                           
java.sql.Ref                미지원                                           
java.sql.Savepoint          미지원                                           
java.sql.SQLData            미지원                                           
java.sql.SQLInput           미지원                                           
java.sql.Struct             미지원                                           
=========================== =========================================================

.. note::

    다음의 표에서 지정하지 않은 JDBC API는 지원하지 않고 SQLException을 반환한다.

java.sql.Connection
^^^^^^^^^^^^^^^^^^^^^^

.. csv-table::
   :header: "Method", "Description"
   :widths: auto

    "Properties getClientInfo()", :ref:`jsp-get-client-info`
    "void rollback()", "do nothing"
    "Statement createStatement()", :ref:`jsp-execute-statement`
    "Statement createStatement(int resultSetType, int resultSetConcurrency)", :ref:`jsp-execute-statement`
    "Statement createStatement(int resultSetType, int resultSetConcurrency, int resultSetHoldability)", :ref:`jsp-execute-statement`
    "CallableStatement prepareCall(String sql)", :ref:`jsp-execute-statement`
    "CallableStatement prepareCall(String sql, int resultSetType, int resultSetConcurrency)", :ref:`jsp-execute-statement`
    "CallableStatement prepareCall(String sql, int resultSetType, int resultSetConcurrency, int resultSetHoldability)", :ref:`jsp-execute-statement`
    "PreparedStatement prepareStatement(String sql)", :ref:`jsp-execute-statement`
    "PreparedStatement prepareStatement(String sql, int autoGeneratedKeys)", :ref:`jsp-execute-statement`
    "PreparedStatement prepareStatement(String sql, int[] columnIndexes)", :ref:`jsp-execute-statement`
    "PreparedStatement prepareStatement(String sql, int resultSetType, int resultSetConcurrency)", :ref:`jsp-execute-statement`
    "PreparedStatement prepareStatement(String sql, int resultSetType, int resultSetConcurrency, int resultSetHoldability)", :ref:`jsp-execute-statement`
    "PreparedStatement prepareStatement(String sql, String[] columnNames)", :ref:`jsp-execute-statement`
    "void clearWarnings()", "do nothing"
    "void close()", "close all statements"
    "void commit()", "do nothing"
    "boolean getAutoCommit()", "return false"
    "String getCatalog()", "return "
    "int getHoldability()", "return ResultSet.HOLD_CURSORS_OVER_COMMIT;"
    "int getTransactionIsolation()", ""
    "SQLWarning getWarnings()", "return null"
    "boolean isClosed()", "return false"
    "boolean isReadOnly()", "return false"
    "boolean isValid(int timeout)", "return true"
    "void setAutoCommit(boolean autoCommit)", "do nothing"
    "void setCatalog(String catalog)", "do nothing"
    "void setHoldability(int holdability)", "do nothing"
    "void setReadOnly(boolean readOnly)", "do nothing"
    "void setTransactionIsolation(int level)", "do nothing"

java.sql.Statement
^^^^^^^^^^^^^^^^^^^^^^

.. csv-table::
   :header: "Method", "Description"
   :widths: auto

    "Connection getConnection()", ""
    "int getFetchDirection()", "retruns ResultSet.FETCH_FORWARD"
    "int getFetchSize()", ""
    "int getMaxFieldSize()", ""
    "int getMaxRows()", ""
    "int getQueryTimeout()", "retruns 0"
    "int getResultSetConcurrency()", "retruns ResultSet.CONCUR_UPDATABLE"
    "int getResultSetHoldability()", "return ResultSet.HOLD_CURSORS_OVER_COMMIT or ResultSet.CLOSE_CURSORS_AT_COMMIT"
    "int getResultSetType()", "return ResultSet.TYPE_FORWARD_ONLY"
    "int getUpdateCount()", "return -1"
    "boolean isClosed()", ""
    "void setFetchDirection(int direction)", ""
    "void setFetchSize(int rows)", ""
    "void setMaxFieldSize(int max)", ""
    "void setMaxRows(int max)", ""
    "void setQueryTimeout(int seconds)", ""
    "void close()", ""
    "boolean execute(String sql)", ""
    "boolean execute(String sql, int autoGeneratedKeys)", ""
    "boolean execute(String sql, int[] columnIndexes)", ""
    "boolean execute(String sql, String[] columnNames)", ""
    "executeBatch()", "throws SQLException"
    "ResultSet executeQuery(String sql)", ""
    "int executeUpdate(String sql)", ""
    "int executeUpdate(String sql, int autoGeneratedKeys)", ""
    "int executeUpdate(String sql, int[] columnIndexes)", ""
    "int executeUpdate(String sql, String[] columnNames)", ""
    "ResultSet getGeneratedKeys()", ""
    "boolean getMoreResults()", ""
    "ResultSet getResultSet()", ""
    "void cancel()", "do nothing"
    "void clearWarnings()", ""
    "SQLWarning getWarnings()", ""
    "void setCursorName(String name)", ""
    "void setEscapeProcessing(boolean enable)", ""

java.sql.PreparedStatement
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. csv-table::
   :header: "Method", "Description"
   :widths: auto

    "boolean execute()", ""
    "ResultSet executeQuery()", ""
    "int executeUpdate()", ""
    "ResultSetMetaData getMetaData()", ""
    "void setBigDecimal(int parameterIndex, BigDecimal x)", ""
    "void setBoolean(int parameterIndex, boolean x)", ""
    "void setByte(int parameterIndex, byte x)", ""
    "void setBytes(int parameterIndex, byte[] x)", ""
    "void setDate(int parameterIndex, Date x)", ""
    "void setDate(int parameterIndex, Date x, Calendar cal)", ""
    "void setDouble(int parameterIndex, double x)", ""
    "void setFloat(int parameterIndex, float x)", ""
    "void setInt(int parameterIndex, int x)", ""
    "void setLong(int parameterIndex, long x)", ""
    "void setNull(int parameterIndex, int sqlType)", ""
    "void setNull(int parameterIndex, int sqlType, String typeName)", ""
    "void setObject(int parameterIndex, Object x)", ""
    "void setObject(int parameterIndex, Object x, int targetSqlType)", ""
    "void setObject(int parameterIndex, Object x, int targetSqlType, int scaleOrLength)", ""
    "void setShort(int parameterIndex, short x)", ""
    "void setString(int parameterIndex, String x)", ""
    "void setTime(int parameterIndex, Time x)", ""
    "void setTime(int parameterIndex, Time x, Calendar cal)", ""
    "void setTimestamp(int parameterIndex, Timestamp x)", ""
    "void setTimestamp(int parameterIndex, Timestamp x, Calendar cal)", ""


java.sql.CallableStatement
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. csv-table::
   :header: "Method", "Description"
   :widths: auto

    "BigDecimal getBigDecimal(int parameterIndex)", ""
    "boolean getBoolean(int parameterIndex)", ""
    "byte getByte(int parameterIndex)", ""
    "byte[] getBytes(int parameterIndex)", ""
    "Date getDate(int parameterIndex)", ""
    "Date getDate(int parameterIndex, Calendar cal)", ""
    "double getDouble(int parameterIndex)", ""
    "getFloat(int parameterIndex)", ""
    "getInt(int parameterIndex)", ""
    "getLong(int parameterIndex)", ""
    "getObject(int parameterIndex)", ""
    "getShort(int parameterIndex)", ""
    "getString(int parameterIndex)", ""
    "getTime(int parameterIndex)", ""
    "getTime(int parameterIndex, Calendar cal)", ""
    "getTimestamp(int parameterIndex)", ""
    "getTimestamp(int parameterIndex, Calendar cal)", ""
    "registerOutParameter(int parameterIndex, int sqlType)", ""
    "registerOutParameter(int parameterIndex, int sqlType, int scale)", ""
    "registerOutParameter(int parameterIndex, int sqlType, String typeName)", ""
    "wasNull()", ""

java.sql.ResultSet
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. csv-table::
   :header: "Method", "Description"
   :widths: auto

    "clearWarnings()", ""
    "close()", ""
    "deleteRow()", "throws SQLException"
    "findColumn(String columnLabel)", ""
    "first()", "throws SQLException"
    "getBoolean(int columnIndex)", ""
    "getBoolean(String columnLabel)", ""
    "getByte(int columnIndex)", ""
    "getByte(String columnLabel)", ""
    "getBytes(int columnIndex)", ""
    "getBytes(String columnLabel)", ""
    "getConcurrency()", "return ResultSet.CONCUR_READ_ONLY;"
    "getDate(int columnIndex)", ""
    "getDate(int columnIndex, Calendar cal)", ""
    "getDate(String columnLabel)", ""
    "getDate(String columnLabel, Calendar cal)", ""
    "getDouble(int columnIndex)", ""
    "getDouble(String columnLabel)", ""
    "getFetchDirection()", ""
    "getFetchSize()", ""
    "getFloat(int columnIndex)", ""
    "getFloat(String columnLabel)", ""
    "getHoldability()", ""
    "getInt(int columnIndex)", ""
    "getInt(String columnLabel)", ""
    "getLong(int columnIndex)", ""
    "getLong(String columnLabel)", ""
    "getMetaData()", ""
    "getObject(int columnIndex)", ""
    "getObject(String columnLabel)", ""
    "getRow()", ""
    "getShort(int columnIndex)", ""
    "getShort(String columnLabel)", ""
    "getStatement()", ""
    "getString(int columnIndex)", ""
    "getString(String columnLabel)", ""
    "getTime(int columnIndex)", ""
    "getTime(int columnIndex, Calendar cal)", ""
    "getTime(String columnLabel)", ""
    "getTime(String columnLabel, Calendar cal)", ""
    "getTimestamp(int columnIndex)", ""
    "getTimestamp(int columnIndex, Calendar cal)", ""
    "getTimestamp(String columnLabel)", ""
    "getTimestamp(String columnLabel, Calendar cal)", ""
    "getType()", "retruns ResultSet.TYPE_FORWARD_ONLY"
    "isAfterLast()", ""
    "isBeforeFirst()", ""
    "isClosed()", "return false"
    "isFirst()", ""
    "isLast()", ""
    "wasNull()", ""
    "getCursorName()", "return "
    "getWarnings()", "return null"


java.sql.ResultSetMetaData
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. csv-table::
   :header: "Method", "Description"
   :widths: auto

    "getCatalogName (int column)", "return"
    "getColumnClassName(int column)", ""
    "getColumnCount()", ""
    "getColumnDisplaySize(int column)", ""
    "getColumnLabel(int column)", ""
    "getColumnName(int column)", ""
    "getColumnType(int column)", ""
    "getColumnTypeName(int column)", ""
    "getPrecision(int column)", ""
    "getScale(int column)", ""
    "getSchemaName(int column)", "return "
    "getTableName(int column)", ""
    "isAutoIncrement(int column)", ""
    "isCaseSensitive(int column)", ""
    "isCurrency(int column)", ""
    "isDefinitelyWritable(int column)", "return false"
    "isNullable(int column)", ""
    "isReadOnly(int column)", "return false"
    "isSearchable(int column)", "return true"
    "isSigned(int column)", ""
    "isWritable(int column)", "return true"

