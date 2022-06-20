
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

To access the database from a Java stored function/procedure, you must use the server-side JDBC driver.
See details on how to use the server-side JDBC driver, refer to the :ref:`jsp-server-side-jdbc`.

Compile the Java source code
------------------------------

Compile the SpCubrid.java file as follows:

::

    javac SpCubrid.java

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

To access the database from a Java stored function/procedure, you must use the server-side JDBC driver. As Java stored functions/procedures are executed within the database, there is no need to make the connection to the server-side JDBC driver again. 

|  To acquire a connection to the database using the server-side JDBC driver, you can either use "**jdbc:default:connection:**" as the URL for JDBC connection, or call the **getDefaultConnection** () method of the **cubrid.jdbc.driver.CUBRIDDriver** class.

.. code-block:: java

    Connection conn = DriverManager.getConnection("jdbc:default:connection:");

or

.. code-block:: java

    cubrid.jdbc.driver.CUBRIDDriver.getDefaultConnection();

If you connect to the database using the JDBC driver as shown above, the transaction in the Java stored function/procedure is ignored. That is, database operations executed in the Java stored function/procedure belong to the transaction that called the Java stored function/procedure. In the following example, **conn.commit()** method of the **Athlete** class is ignored.

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

Connecting to Other Database
============================

You can connect to another outside database instead of the currently connected one even when the server-side JDBC driver is being used. Acquiring a connection to an outside database is not different from a generic JDBC connection. For details, see JDBC API.

If you connect to other databases, the connection to the CUBRID database does not terminate automatically even when the execution of the Java method ends. Therefore, the connection must be explicitly closed so that the result of transaction operations such as **COMMIT** or **ROLLBACK** will be reflected in the database. That is, a separate transaction will be performed because the database that called the Java stored function/procedure is different from the one where the actual connection is made.

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

When the Java stored function/procedure is executed, it should run only on JVM located in the database server, you can check where it is running by calling System.getProperty ("cubrid.server.version") from the Java program source. The result value is the database version if it is called from the database; otherwise, it is **NULL**.

.. _jsp-load-java:

loadjava Utility
================

To load a compiled Java or JAR (Java Archive) file into CUBRID, use the **loadjava** utility. If you load a Java \*.class or \*.jar file using the **loadjava** utility, the file is moved to the specified database path. ::

    loadjava [option] database-name java-class-file

*   *database-name*: The name of the database where the Java file to be loaded.
*   *java-class-file*: The name of the Java class or jar file to be loaded.
*   [*option*]

    *   **-y**: Automatically overwrites a class file with the same name, if any. The default value is **no**. If you load the file without specifying the **-y** option, you will be prompted to ask if you want to overwrite the class file with the same name (if any).

.. _jsp-caution:

Caution
=======

Returning Value of Java Stored Function/Procedure and Precision Type on IN/OUT
------------------------------------------------------------------------------

To limit the return value of Java stored function/procedure and precision type on IN/OUT, CUBRID processes as follows:

*   Checks the sql_type of the Java stored function/procedure.

*   Passes the value returned by Java to the database with only the type converted if necessary, ignoring the number of digits defined during creating the Java stored function/procedure. In principle, the user manipulates directly the data which is passed to the database.

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

Returning java.sql.ResultSet in Java Stored Procedure
-----------------------------------------------------

In CUBRID, you must use **CURSOR** as the data type when you declare a Java stored function/procedure that returns a **java.sql.ResultSet**.

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

.. note::
    
    You cannot use the **ResultSet** as an input argument. If you pass it to an IN argument, an error occurs. An error also occurs when calling a function that returns **ResultSet** in a non-Java environment.

IN/OUT of Set Type in Java Stored Function/Procedure
----------------------------------------------------

If the set type of the Java stored function/procedure in CUBRID is IN OUT, the value of the argument changed in Java must be applied to IN OUT. When the set type is passed to the OUT argument, it must be passed as a two-dimensional array.

.. code-block:: sql

    CREATE PROCEDURE setoid(x in out set, z object) AS LANGUAGE JAVA 
    NAME 'SetOIDTest.SetOID(cubrid.sql.CUBRIDOID[][], cubrid.sql.CUBRIDOID';

.. code-block:: java

    import cubrid.sql.CUBRIDOID;

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

Using OID in Java Stored Function/Procedure
-------------------------------------------

In case of using the OID type value for IN/OUT in CUBRID, use the value passed from the server.

.. code-block:: sql

    CREATE PROCEDURE tOID(i inout object, q string) AS LANGUAGE JAVA
    NAME 'OIDtest.tOID(cubrid.sql.CUBRIDOID[], java.lang.String)';

.. code-block:: java

    import java.sql.*;
    import cubrid.sql.CUBRIDOID;

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
