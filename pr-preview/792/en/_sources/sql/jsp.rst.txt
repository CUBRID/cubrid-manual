
:meta-keywords: cubrid jsp, cubrid javasp, loadjava utility
:meta-description: CUBRID supports to develop stored functions and procedures in Java. Java stored functions/procedures are executed on the JVM (Java Virtual Machine) hosted by CUBRID.

******************************
Java Stored Function/Procedure
******************************

.. _jsp-introduction:

Introduction to Java Stored Function/Procedure
==============================================

Stored functions and procedures are used to implement complicated program logic that is not possible with SQL. They allow users to manipulate data more easily. Stored functions/procedures are blocks of code that have a flow of commands for data manipulation and are easy to manipulate and administer.

CUBRID supports to develop stored functions and procedures in Java. Java stored functions/procedures are executed on the JVM (Java Virtual Machine) hosted by CUBRID.

You can call Java stored functions/procedures from SQL statements or from Java applications using JDBC.

The advantages of using Java stored functions/procedures are as follows:

*   **Productivity and usability**: Java stored functions/procedures, once created, can be reused anytime. They can be called from SQL statements or from Java applications using JDBC.
*   **Excellent interoperability and portability**: Java stored functions/procedures use the Java Virtual Machine. Therefore, they can be used on any system where the Java Virtual Machine is available.

.. note::

    *   The other languages except Java do not support stored function/procedure. In CUBRID, only Java can implement stored function/procedure.

.. _jsp-prerequisites:

Prerequisites
==============================================

To use Java stored function/procedure, the following must be ready

*   **java_stored_procedure** must be set to **yes** in the **cubrid.conf** file.
*   Java Stored Procedure server (Java SP server) must be started for the database that you want to use Java stored function/procedures.

.. _jsp-system-prm:

Check the cubrid.conf file
--------------------------

By default, the **java_stored_procedure** is set to **no** in the **cubrid.conf** file.   
To use a Java stored function/procedure, this value must be changed to **yes**. For details on this value, see :ref:`other-parameters` in Database Server Configuration.

.. _jsp-starting-javasp:

Start Java SP Server
---------------------------------

You need to start a Java Stored Procedure server (Java SP server) for the database you want to use Java-stored procedures/functions.

Execute the **cubrid javasp** **start** *db_name*. ::

    % cubrid javasp start demodb

    @ cubrid javasp start: demodb
    ++ cubrid javasp start: success

You can verify that the Java SP server is successfully started.

Execute the **cubrid javasp** **status** *db_name*. ::

    % cubrid javasp status demodb

    @ cubrid javasp status: demodb
    Java Stored Procedure Server (demodb, pid 9220, port 38408)
    Java VM arguments :
    -------------------------------------------------
    -Djava.util.logging.config.file=/path/to/CUBRID/java/logging.properties
    -Xrs
    -------------------------------------------------

For more details on javasp utility, see :ref:`cubrid-javasp-server` and :ref:`cubrid-javasp-server-config`.

How to Write and Load Java Stored Function/Procedure
======================================================

To use a Java stored function/procedure, you can write and publish Java stored function/procedure as follows.

    *   **Step 1: Write the Java source code**
    *   **Step 2: Compile the Java source code**
    *   **Step 3: Load Java Class**
    *   **Step 4: Publish Stored function/procedure**

Write the Java source code
--------------------------------------

The following example shows how to write Java stored function/procedure.
Here, the Java class method must be **public static**.

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

To access the database from a Java stored function/procedure, you must create a Connection object.
See details on how to use the server-side JDBC driver, refer to the :ref:`jsp-server-side-jdbc`.

Compile the Java source code
------------------------------

Compile the SpCubrid.java file as follows:

::

    javac SpCubrid.java

When using the server-side JDBC driver, you must compile as follows by specifying the path of JDBC using the **classpath(cp)** option.
Note that you must use the latest JDBC driver of the database server to be loaded.

::

    javac SpCubrid.java -cp $CUBRID/jdbc/cubrid_jdbc.jar

.. _jsp-loadjava:

Load the compiled Java class into CUBRID
----------------------------------------

Load the compiled Java class into CUBRID. 
You can refer to the :ref:`jsp-load-java`.

::

    % loadjava demodb SpCubrid.class


Publish the loaded Java class
-----------------------------

In CUBRID, it is required to publish Java classes to call Java methods from SQL statements or Java applications.
Create a CUBRID stored function and publish the Java class as shown below.
For more details, see :ref:`call-specification`.

.. code-block:: sql

    CREATE FUNCTION hello() RETURN STRING 
    AS LANGUAGE JAVA 
    NAME 'SpCubrid.HelloCubrid() return java.lang.String';

.. CREATE OR REPLACE FUNCTION is allowed from 10.0: CUBRIDSUS-6542

Or with **OR REPLACE** syntax, you can replace the current stored function/procedure or create the new stored function/procedure.

.. code-block:: java

    CREATE OR REPLACE FUNCTION hello() RETURN STRING
    AS LANGUAGE JAVA
    NAME 'SpCubrid.HelloCubrid() return java.lang.String';    
    
Call the Java stored function/procedure
========================================

You can call the Java stored functions/procedures by using a **CALL** statement, from SQL statements or Java applications.

|  If an exception occurs during the execution of a Java stored function/procedure, the exception is logged and stored in the *dbname*\ **_java.log** file. To display the exception on the screen, change a handler value of the **$CUBRID/java/logging.properties** file to "java.lang.logging.ConsoleHandler" Then, the exception details are displayed on the screen.

Using CALL Statement
----------------------

You can call Java stored procedure/functions by using **CALL** statement as follows.
For more details, see :doc:`/sql/query/call`.

.. code-block:: sql

    CALL Hello() INTO :HELLO;

::

      Result
    ======================
    'Hello, Cubrid !!'

Calling from SQL Statement
--------------------------

You can call a Java stored function from a SQL statement as shown below.

.. code-block:: sql

    SELECT Hello() FROM db_root;
    SELECT sp_int(99) FROM db_root;

You can use a host variable for the IN/OUT data type when you call a Java stored function/procedure as follows:

.. code-block:: sql

    SELECT 'Hi' INTO :out_data FROM db_root;
    CALL test_out(:out_data);
    SELECT :out_data FROM db_root;

The first clause calls a Java stored procedure in out mode by using a parameter variable; the second is a query clause retrieving the assigned host variable out_data.

Calling from Java Application
-----------------------------

To call a Java stored function/procedure from a Java application, use a **CallableStatement** object.

Create a phone class in the CUBRID database.

.. code-block:: sql

    CREATE TABLE phone(
         name VARCHAR(20),
         phoneno VARCHAR(20)
    );

Compile the following **PhoneNumber.java** file, load the Java class file into CUBRID, and publish it.

.. code-block:: java

    import java.sql.*;
    import java.io.*;

    public class PhoneNumber{
        public static void Phone(String name, String phoneno) throws Exception{
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

Create and run the following Java application.

.. code-block:: java

    import java.sql.*;

    public class StoredJDBC{
        public static void main(){
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

Retrieve the phone class after executing the program above; the following result would be displayed.

.. code-block:: sql

    SELECT * FROM phone;
    
::

    name                  phoneno
    ============================================
        'Jane'                '010-111-1111'

.. _jsp-server-side-jdbc:

Using Server-side Internal JDBC Driver
======================================

To access the database from a Java stored function/procedure, you must use the server-side JDBC driver.
The following are possible with the server-side JDBC driver.


*    **Executing SQL Statements**
*    **Processing Query Result**

The following classes are supported by the server-side JDBC driver. For details on JDBC API support, refer to :ref:`jsp-appendix`.

*    **java.sql.Connection**
*    **java.sql.Statement**
*    **java.sql.PreparedStatement**
*    **java.sql.CallableStatement**
*    **java.sql.ResultSet**
*    **java.sql.ResultSetMetaData**

.. warning::
    
    **java.sql.DatabaseMetaData** is not supported yet.

Database operations using the server-side JDBC have the following characteristics.

* Database operations executed in the Java stored function/procedure belongs to the transaction that is called the Java stored function/procedure.
* Transaction-related APIs are ignored.
* There is no need to make the connection to the server-side JDBC driver again. 

.. _jsp-server-side-jdbc-connection:

Creating Connection
---------------------

To access the database from a Java stored function/procedure, you must use the server-side JDBC driver.
To acquire a connection to the database using the server-side JDBC driver, you can use "**jdbc:default:connection:**" as the URL for JDBC connection.

.. code-block:: java

    Connection conn = DriverManager.getConnection("jdbc:default:connection:");

.. note::

    From the CUBRID 11.2, Acquiring a connection in the following way is no longer supported:
    
        Connection conn = cubrid.jdbc.driver.CUBRIDDriver.getDefaultConnection();

.. note::

    The server-side JDBC is already registered, and you do not need to call "Class.forName("cubrid.jdbc.driver.CUBRIDDriver")"

.. _jsp-execute-statement:

Executing SQL Statements
----------------------------

When implementing Java stored functions/procedures, queries can be executed using the following JDBC interface in the same way as developing Java applications.

*    **java.sql.Statement**
*    **java.sql.PreparedStatement**
*    **java.sql.CallableStatement**

The following are the queries that can be executed using the above class.

*    **DML (Data Manipulation Language)**: :doc:`/sql/query/index`
*    **DDL (Data Definition Language)**: :doc:`/sql/schema/index`

.. note::

    The JDBC Statement objects must contain only one SQL statement.
    Therefore, an error occurs in the following cases:

    ::

        stmt = new Statement ("select * from t1;select * from t2;");

The following statements are not supported.

* **TCL (Transaction Control Language)**: :ref:`database-transaction`

.. note::

     * *commit()*, *rollback()* JDBC API methods corresponding to **COMMIT** and **ROLLBACK** statements respectively are ignored.
     * JDBC API methods corresponding to **SAVEPOINT** statement are not supported.

The example of executing statements
-------------------------------------

**Execute a query that returns a result set and process the query result set**

The following example shows how to execute a **SELECT** statement that returns a result set.
**SELECT** statement can be executed by creating a **java.sql.Statement** or **java.sql.PreparedStatement** object.
The query result can be processed using the result set (**java.sql.ResultSet**).

.. note::

     * java.sql.ResultSet is forward-only and read-only.
     * In the case of the client-side JDBC driver, when a query result set is created, :ref:`cursor holdability <cursor-holding>` is performed by default.
       In the server-side JDBC driver, resources are managed by the server, so the query result set is internally closed at the end of the stored function/procedure without maintaining a cursor.

Also, result set metadata (**java.sql.ResultSetMetaData**) can be created from the query result set by using the **getMetaData()** function.


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

The following is an example of executing the **INSERT** statement. **INSERT**, **UPDATE**, **DELETE** statements can be executed through the **executeUpdate()** function.

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

    conn.commit() is ignored at the Athlete class example above.


OUT Parameters of Primitive Types
--------------------------------------------------------------

When changing an argument value in Java in a Java stored function/procedure of CUBRID, the changed value must be passed when an argument is passed as an OUT argument as a one-dimensional array.

.. code-block:: sql

    CREATE PROCEDURE sp_increment_me(x IN OUT INT) AS LANGUAGE JAVA NAME 'OutTest.incrementInt(int[])';

.. code-block:: java

    public class OutTest {
        public static void incrementInt(int[] arg) {
            arg[0] = arg[0] + 1;
        }
    }

OUT Parameters of Set Types
----------------------------------------------------

Parameters of Java methods corresponding to an OUT (or IN OUT) parameter of an SQL set type must be declared as an two-dimensional array of an appropriate type.

.. code-block:: sql

    CREATE PROCEDURE setoid(x in out set, z object) AS LANGUAGE JAVA 
    NAME 'SetOIDTest.SetOID(cubrid.sql.CUBRIDOID[][], cubrid.sql.CUBRIDOID)';

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


OUT Parameters of CUBRID OID type
-------------------------------------------

In case of using an OUT (or IN OUT) parameter of CUBRID OID type, declare the corresponding parameter of the Java method as an array of CUBRIDOID class (cubrid.sql.CUBRIDOID).

.. code-block:: sql

    CREATE PROCEDURE tOID(i inout object, q string) AS LANGUAGE JAVA
    NAME 'OIDtest.tOID(cubrid.sql.CUBRIDOID[], java.lang.String)';

.. code-block:: java

    import java.sql.*;
    import cubrid.sql.CUBRIDOID;

    public class OIDtest {
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

Returning java.sql.ResultSet in Java Stored Procedure
-----------------------------------------------------

In CUBRID, a query result set (**java.sql.ResultSet**) can be returned, and **CURSOR** is used as the returned data type when declared.

.. note::

     * **java.sql.ResultSet** cannot be used as an input argument of a function, and an error occurs if it is passed as an IN argument.
     * An error also occurs when calling a function that returns **ResultSet** in a non-Java environment.

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

In the calling block, you must set the OUT argument with **Types.JAVA_OBJECT**, get the argument to the **getObject** () function, and then cast it to the **java.sql.ResultSet** type before you use it. In addition, the **java.sql.ResultSet** is only available to use in **CallableStatement** of JDBC.

.. code-block:: java

    import java.sql.*;
     
    public class TestResultSet{
        public static void main(String[] args) {
            Connection conn = null;
     
            try {
                conn = DriverManager.getConnection("jdbc:default:connection:");
     
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

Getting information about connection client
---------------------------------------------

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

Connecting to Other Databases
==============================

You can connect to another outside database instead of the currently connected one even when the server-side JDBC driver is being used. Acquiring a connection to an outside database is not different from a generic JDBC connection. For details, see JDBC API.

.. warning::

    If you connect to other databases, the connection to the CUBRID database does not terminate automatically even when the execution of the Java method ends. 
    Therefore, the connection must be explicitly closed so that the result of transaction operations such as **COMMIT** or **ROLLBACK** will be reflected in the database.
    That is, a separate transaction will be performed because the database that called the Java stored function/procedure is different from the one where the actual connection is made.

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

When Java stored functions/procedures are executed, they should run only on a JVM located in the database server. You can check where they are running by calling System.getProperty ("cubrid.server.version") from the Java programs. The result is the database version if it is called from the database; otherwise, it is **NULL**.


.. _jsp-jni:

Java Native Interface (JNI) Support
===================================

Using the Java Native Interface (JNI), you can invoke functions in native languages like C/C++ from the Java Virtual Machine (JVM). 
Java Stored Procedures (SP) in CUBRID provide support for JNI functionality, but you should be cautious when using it because issues in native code can have unexpected impacts on the stored routine server (cub_javasp) process and its operation.
Java Classes used for loading native libraries should be registered using the **-j** option of **loadjava** to prevent them from being dynamically loaded. For more details, refer to :ref:jsp-load-java.

The following is an example of invoking a native function through JNI in a CUBRID Java stored function:

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

    Registering and executing Java stored procedures/functions that invoke JNI without the **-j** option may result in a java.lang.UnsatisfiedLinkError.
    To address this issue, please consider the following:

    * If you are loading multiple Java class files that call System.load() for the same native library path:
       * Modify the Java class files to load the native library from only one class file.
       * Register the class that loads the native library using the **-j** option of loadjava.
       * Restart the javasp utility.

    * If you are overwriting a previously loaded Java class file using loadjava:
       * If the class was registered without the -j option, remove that class from the java directory at the respective database path.
       * Re-register the class using the -j option with loadjava.
       * Restart the javasp utility.

.. _jsp-load-java:

loadjava Utility
================

You can load a Java \*.class or \*.jar file using **loadjava** utility. The file is moved to a database internal path.

    loadjava [option] database-name java-class-file

*   *database-name*: The name of the database where the Java file to be loaded.
*   *java-class-file*: The name of the Java class or jar file to be loaded.
*   [*option*]

    *   **-y**: automatically overwrites a file with the same name, if any. If you do not use this option, you will get a prompt asking if you want to overwrite the file with the same name, if any.
    *   **-j(or --jni)**: moves to the path for statically loading Java \*.class files or \*.jar files. This option should be used to prevent native library loading errors when loading classes that include JNI (native libraries).

.. _jsp-caution:

Caution
=======

* java.sql.DatabaseMetaData is not supported.
* JDBC API related to BLOB/CLOB type is not supported.
* Functions not related to query execution and used only in client-side JDBC are not supported. For details, refer to :ref:`jsp-appendix`\.
* Multiple SQL statements are not supported when executing a query with one JDBC object.
* ResultSet created by query execution is non-updatable, non-scrollable, and non-sensitive.
* Java ignores precision, scale, and length parts of SQL types of IN/OUT parameters, matches only the type name parts, and delivers values as they are.
* A stored procedure can call another stored procedure or call itself recursively. The maximum nesting depth is 16.

Limitations on the precision of IN/OUT parameters and a return value
-----------------------------------------------------------------------------------

To limit the return value of Java stored function/procedure and precision type on IN/OUT, CUBRID processes as follows:

*   Checks the SQL type of the Java stored function/procedure.
*   Passes the value returned by Java to the database with only the type converted if necessary, ignoring the number of digits defined during creating the Java stored function/procedure. 
*   In principle, the user should directly manipulates the data which is passed to the database.

Take a look at the following **typestring** () Java stored function.

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

Appendix
========================

Table of Supproting JDBC API 
------------------------------

=========================== =========================================================
JDBC Interface              Support/Unsupport                                               
=========================== =========================================================
java.sql.CallableStatement  Support                                               
java.sql.Connection         Support                                               
java.sql.Driver             Support (:ref:`jsp-server-side-jdbc-connection`)
java.sql.PreparedStatement  Support                                               
java.sql.ResultSet          Support                                               
java.sql.ResultSetMetaData  Support                                               
CUBRIDOID                   Support                                               
java.sql.Statement          Support
java.sql.DriverManager      Support                                               
Java.sql.SQLException       Support                                               
java.sql.Array              Unsupport                                           
java.sql.Blob               Unsupport                                               
java.sql.Clob               Unsupport                                               
java.sql.DatabaseMetaData   Unsupport                                               
java.sql.ParameterMetaData  Unsupport                                           
java.sql.Ref                Unsupport                                           
java.sql.Savepoint          Unsupport                                           
java.sql.SQLData            Unsupport                                           
java.sql.SQLInput           Unsupport                                           
java.sql.Struct             Unsupport                                           
=========================== =========================================================

.. note::

    JDBC APIs not specified in the table below are not supported and return SQLException.

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
