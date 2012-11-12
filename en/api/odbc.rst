*************
ODBC 드라이버
*************

**ODBC Driver**

**ODBC Overview**

**Description**

CUBRID ODBC driver supports ODBC version 3.52. It also ODBC core and some parts of Level 1 and Level 2 API. Because CUBRID ODBC driver has been developed based on the ODBC Spec 3.x, backward compatibility is not completely ensured for programs written based on the ODBC Spec 2.x. Only 32-bit CUBRID is supported so you must install 32-bit CUBRID even on the 64-bit Windows environment. To verify the 32-bit CUBRID ODBC driver on the 64-bit Windows environment, execute
**C:\Windows\SysWOW64\odbcad32.exe**
.

CUBRID ODBC driver is written based on CCI API so affected by CCI configurations such as
**CCI_DEFAULT_AUTOCOMMIT**
.

To download ODBC driver or get the latest information, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-odbc-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-odbc-driver>`_
.

**Data Type Mapping Between CUBRID and ODBC**

The following table shows the data type mapping relationship between CUBRID and ODBC.

+-------------------------+--------------------------------+
| **CUBRID Data Type**    | **ODBC Data Type**             |
|                         |                                |
+-------------------------+--------------------------------+
| Char                    | SQL_CHAR                       |
|                         |                                |
+-------------------------+--------------------------------+
| Varchar                 | SQL_VARCHAR                    |
|                         |                                |
+-------------------------+--------------------------------+
| String                  | SQL_LONGVARCHAR                |
|                         |                                |
+-------------------------+--------------------------------+
| Nchar                   | SQL_CHAR                       |
|                         |                                |
+-------------------------+--------------------------------+
| Varnchar                | SQL_VARCHAR                    |
|                         |                                |
+-------------------------+--------------------------------+
| Bit                     | SQL_BINARY                     |
|                         |                                |
+-------------------------+--------------------------------+
| varying bit             | SQL_VARBINARY                  |
|                         |                                |
+-------------------------+--------------------------------+
| Numeric                 | SQL_NUMERIC                    |
|                         |                                |
+-------------------------+--------------------------------+
| Int                     | SQL_INTEGER                    |
|                         |                                |
+-------------------------+--------------------------------+
| Short                   | SQL_SMALLINT                   |
|                         |                                |
+-------------------------+--------------------------------+
| Float                   | SQL_FLOAT                      |
|                         |                                |
+-------------------------+--------------------------------+
| Double                  | SQL_DOUBLE                     |
|                         |                                |
+-------------------------+--------------------------------+
| Bigint                  | SQL_BIGINT                     |
|                         |                                |
+-------------------------+--------------------------------+
| Date                    | SQL_TYPE_DATE                  |
|                         |                                |
+-------------------------+--------------------------------+
| Time                    | SQL_TYPE_TIME                  |
|                         |                                |
+-------------------------+--------------------------------+
| Timestamp               | SQL_TYPE_TIMESTAMP             |
|                         |                                |
+-------------------------+--------------------------------+
| Datetime                | SQL_TYPE_TIMESTAMP             |
|                         |                                |
+-------------------------+--------------------------------+
| Monetary                | SQL_DOUBLE                     |
|                         |                                |
+-------------------------+--------------------------------+
| Oid                     | SQL_CHAR(32)                   |
|                         |                                |
+-------------------------+--------------------------------+
| set, multiset, sequence | SQL_VARCHAR(MAX_STRING_LENGTH) |
|                         |                                |
+-------------------------+--------------------------------+

**Configuring and Environment ODBC and ASP**

**Requirements**

*   CUBRID 2008 R3.1 (8.3.1) or later (32-bit version)



**Configuring CUBRID ODBC Driver**

CUBRID ODBC driver is automatically installed upon CUBRID installation. You can check whether it is properly installed in the [Control Panel] > [Administrative Tools] > [Data Source (ODBC)] > [Drivers] tab.

|image77_png|

**Configuring DNS**

After you check the CUBRID ODBC driver installed, configure DSN as a database where the applications are trying to connect. To configure, click the [Add] button in the ODBC Data Source Administrator dialog box. Then, the following dialog box will appear. Select "CUBRID Driver" and then click the [Finish] button.

|image78_png|

In the [Config CUBRID Data Sources] dialog box, enter information as follows:

|image79_png|

*   **DSN**
    : The name of a source data



*   **DB Name**
    : The name of a database to be connected



*   **DB User**
    : The name of a database user



*   **Password**
    : The password of a database user



*   **Server Address**
    : The host address of a database. The value should be either localhost or the IP address of other server.



*   **Server Port**
    : The number of a broker port. You can check the CUBRID broker port number in the
    **cubrid_broker.conf**
    file. The default value is 33,000. To verify the port number, check the BROKER_PORT value in the
    **cubrid_broker.conf**
    file or enter the
    **cubrid service status**
    in the command prompt. The result will be displayed as follows:



|image80_png|

*   **FETCH_SIZE**
    : A value configures the number of records fetched from server whenever the
    **cci_fetch**
    () function of CCI library (which CUBRID ODBC driver internally uses) is called.



After you filled out every field, click the [OK] button. You will notice that data source is added in the [User Data Sources] as shown below.

|image81_png|

**Connecting to a Database Directly without DSN**

It is also possible to connect to a CUBRID database directly in the application source code by using the connecting string. Below shows the example of connection string. For more information, see
`Configuring Connection String <#api_api_odbc_programming_htm_con_5232>`_
.

conn = "driver={CUBRID Driver};server=localhost;port=33000;uid=dba;pwd=;db_name=demodb;"

**Note**
Make sure that your database is running before you try to connect to a CUBRID database. Otherwise, you will receive an error indicating that ODBC call has failed. To start the database called demodb, enter
**cubrid server start demodb**
 in the command prompt.

**Note**
To get the latest information about ODBC driver, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-odbc-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-odbc-driver>`_
.

**ODBC Programming**

**Configuring Connection String**

When you are programming CUBRID ODBC, write the connection strings as follows:

+--------------+----------------+-----------------------------------------------------------+
| **Category** | **Example**    | **Description**                                           |
|              |                |                                                           |
+--------------+----------------+-----------------------------------------------------------+
| Driver       | CUBRID  Driver | Driver name                                               |
|              |                |                                                           |
+--------------+----------------+-----------------------------------------------------------+
| UID          | PUBLIC         | User ID                                                   |
|              |                |                                                           |
+--------------+----------------+-----------------------------------------------------------+
| PWD          | xxx            | Password                                                  |
|              |                |                                                           |
+--------------+----------------+-----------------------------------------------------------+
| FETCH_SIZE   | 100            | Fetch size                                                |
|              |                |                                                           |
+--------------+----------------+-----------------------------------------------------------+
| PORT         | 33000          | The broker port number                                    |
|              |                |                                                           |
+--------------+----------------+-----------------------------------------------------------+
| SERVER       | 127.0.0.1      | The IP address or the host name of a CUBRID broker server |
|              |                |                                                           |
+--------------+----------------+-----------------------------------------------------------+
| DB_NAME      | demodb         | Database name                                             |
|              |                |                                                           |
+--------------+----------------+-----------------------------------------------------------+
| DESCRIPTION  | cubrid_test    | Description                                               |
|              |                |                                                           |
+--------------+----------------+-----------------------------------------------------------+
| CHARSET      | utf-8          | Character set                                             |
|              |                |                                                           |
+--------------+----------------+-----------------------------------------------------------+

The following shows the result of using connection strings above.

"DRIVER=CUBRID Driver;UID=PUBLIC;PWD=xxx;FETCH_SIZE=100;PORT=33000;SERVER=127.0.0.1;DB_NAME=demodb;DESCRIPTION=cubrid_test;CHARSET=utf-8"

**Remark**

*   Because a semi-colon (;) is used as a separator in URL string, it is not allowed to use a semi-colon as parts of a password (PWD) when specifying the password in connection strings.



*   The database connection in thread-based programming must be used independently each other.



**Using Object Identifiers (OIDs) and Collections**

CUBRID ODBC driver is designed for relational DBMS so it does not support all characteristics of object-oriented DBMS. It is because CUBRID is an object-relational DBMS integrating relational and object-oriented data models.

**Using OIDs**

Because CUBRID ODBC driver regards an OID as a string (char(32)), you can use
**INSERT**
,
**UPDATE**
, and
**DELETE**
statements containing OIDs as follows. The OID string should be used with single quotes (''). In the example below, the domain of the member attribute is treated as an object (OID).

insert into foo(member) values('@12|34|56')

delete from foo where member = '@12|34|56'

update foo set age = age + 1 where member = '@12|34|56'

**Using Collections**

There are three types of collections which are
**SET**
,
**MULTISET**
, and
**SEQUENCE**
. Because CUBRID ODBC driver regards a collection as a string (longvarchar), you can obtain collections with commas separated within braces such as "{value_1, value_2, ... value_n}" in the
**SELECT**
statement.

**Note**
If a string longer than defined max length is inserted (
**INSERT**
) or updated (
**UPDATE**
), the string will be truncated.

**Note**
To get the latest information about ODBC driver, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-odbc-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-odbc-driver>`_
.

**ASP Sample Program**

In the virtual directory where the ASP sample program runs, right-click "Default Web Site" and click [Properties].

|image82_png|

In the picture above, if you select
**(All Unassigned)**
from the [IP Address] dropdown list under [Web Site Identification], it is recognized as localhost. If you want to see the sample program through a specific IP address, make an IP address recognize a directory as a virtual directory and register the IP address in the registration information.

The following example shows that an IP address is configured as localhost.

**Example**

The following example shows how to create cubrid.asp and store it in a virtual directory.

<HTML>

    <HEAD>

     <meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">

       <title>CUBRID Query Test Page</title>

  </HEAD>

 

 <BODY topmargin="0" leftmargin="0">

 

 <table border="0" width="748" cellspacing="0" cellpadding="0">

   <tr>

     <td width="200"></td>

     <td width="287">

       <p align="center"><font size="3" face="Times New Roman"><b><font color="#FF0000">CUBRID</font>Query Test</b></font></td>

     <td width="200"></td>

   </tr>

 </table>

 

 <form action="cubrid.asp" method="post" >

 <table border="1" width="700" cellspacing="0" cellpadding="0" height="45">

   <tr>

     <td width="113" valign="bottom" height="16" bgcolor="#DBD7BD" bordercolorlight="#FFFFCC"><font size="2">SERVER IP</font></td>

     <td width="78"  valign="bottom" height="16" bgcolor="#DBD7BD" bordercolorlight="#FFFFCC"><font size="2">Broker PORT</font></td>

     <td width="148" valign="bottom" height="16" bgcolor="#DBD7BD" bordercolorlight="#FFFFCC"><font size="2">DB NAME</font></td>

     <td width="113" valign="bottom" height="16" bgcolor="#DBD7BD" bordercolorlight="#FFFFCC"><font size="2">DB USER</font></td>

     <td width="113" valign="bottom" height="16" bgcolor="#DBD7BD" bordercolorlight="#FFFFCC"><font size="2">DB PASS</font></td>

     <td width="80" height="37" rowspan="4" bordercolorlight="#FFFFCC" bgcolor="#F5F5ED">

       <p><input type="submit" value="execute" name="B1" tabindex="7"></p></td>

   </tr>

   <tr>

     <td width="113" height="1" bordercolorlight="#FFFFCC" bgcolor="#F5F5ED"><font size="2"><input type="text" name="server_ip" size="20" tabindex="1" maxlength="15" value="<%=Request("server_ip")%>"></font></td>

     <td width="78"  height="1" bordercolorlight="#FFFFCC" bgcolor="#F5F5ED"><font size="2"><input type="text" name="cas_port" size="15" tabindex="2" maxlength="6" value="<%=Request("cas_port")%>"></font></td>

     <td width="148" height="1" bordercolorlight="#FFFFCC" bgcolor="#F5F5ED"><font size="2"><input type="text" name="db_name" size="20" tabindex="3" maxlength="20" value="<%=Request("db_name")%>"></font></td>

     <td width="113" height="1" bordercolorlight="#FFFFCC" bgcolor="#F5F5ED"><font size="2"><input type="text" name="db_user" size="15" tabindex="4" value="<%=Request("db_user")%>"></font></td>

     <td width="113" height="1" bordercolorlight="#FFFFCC" bgcolor="#F5F5ED"><font size="2"><input type="password" name="db_pass" size="15" tabindex="5" value="<%=Request("db_pass")%>"></font></td>

   </tr>

   <tr>

     <td width="573" colspan="5" valign="bottom" height="18" bordercolorlight="#FFFFCC" bgcolor="#DBD7BD"><font size="2">QUERY</font></td>

   </tr>

   <tr>

     <td width="573" colspan="5" height="25" bordercolorlight="#FFFFCC" bgcolor="#F5F5ED"><textarea rows="3" name="query" cols="92" tabindex="6"><%=Request("query")%></textarea></td>

   </tr>

 </table>

 </form>

 <hr>

 

</BODY>

</HTML>

 

<%

    ' Fetch the DSN and SQL statement.

    strIP = Request( "server_ip" )

    strPort = Request( "cas_port" )

    strUser = Request( "db_user" )

    strPass = Request( "db_pass" )

    strName = Request( "db_name" )

    strQuery = Request( "query" )

   

if strIP = "" then

   Response.Write "Please enter the SERVER_IP"

        Response.End ' If no IP entered, end the page

    end if

    if strPort = "" then

       Response.Write "Please enter the port number"

        Response.End ' If no port entered, end the page

    end if

    if strUser = "" then

       Response.Write "Please enter the DB_USER"

        Response.End ' If no DB_User entered, end the page

    end if

    if strName = "" then

       Response.Write "Please enter the DB_NAME"

        Response.End ' If no DB_NAME entered, end the page

    end if

    if strQuery = "" then

       Response.Write "Please enter the query you want to check"

        Response.End ' If no Query entered, end the page

    end if

 ' Create the connection object

  strDsn = "driver={CUBRID Driver};server=" & strIP & ";port=" & strPort & ";uid=" & strUser & ";pwd=" & strPass & ";db_name=" & strName & ";"

' Connect to DB

Set DBConn = Server.CreateObject("ADODB.Connection")

       DBConn.Open strDsn

    ' Execute SQL

    Set rs = DBConn.Execute( strQuery )

    ' Show message depending on the SQL statement

    if InStr(Ucase(strQuery),"INSERT")>0 then

        Response.Write "The record has been added."

        Response.End

    end if

    

    if InStr(Ucase(strQuery),"DELETE")>0  then

        Response.Write "The record has been deleted."

        Response.End

    end if

 

    if InStr(Ucase(strQuery),"UPDATE")>0  then

        Response.Write "The record has been modified."

        Response.End

    end if   

%>

<table>

<%   

    ' Show the field name

    Response.Write "<tr bgColor=#f3f3f3>"

    For index =0 to ( rs.fields.count-1 )

        Response.Write "<td><b>" & rs.fields(index).name & "</b></td>"

    Next

    Response.Write "</tr>"

    ' Show the field value

    Do While Not rs.EOF

        Response.Write "<tr bgColor=#f3f3f3>"

        For index =0 to ( rs.fields.count-1 )

            Response.Write "<td>" & rs(index) & "</td>"

        Next

        Response.Write "</tr>"

        

        rs.MoveNext

    Loop

%>

<% 

    set  rs = nothing

%>

</table>

You can check the result of the sample program by connecting to http://localhost/cubrid.asp. When you execute the ASP sample code above, you will get the following output. Enter an appropriate value in each field, enter the query statement in the Query field, and click [Run]. The query result will be displayed at the lower part of the page.

|image83_png|
 

**Note**
To get the latest information about ODBC driver, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-odbc-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-odbc-driver>`_
.

**ODBC API**

For ODBC API, see ODBC API Reference (
`http://msdn.microsoft.com/en-us/library/windows/desktop/ms714562%28v=vs.85%29.aspx <http://msdn.microsoft.com/en-us/library/windows/desktop/ms714562%28v=vs.85%29.aspx>`_
) on the MSDN page. See the table below to get information about the list of functions, ODBC Spec version, and compatibility that CUBRID supports.

+---------------------+------------------------+--------------------------+--------------------------+
| **API**             | **Version Introduced** | **Standards Compliance** | **Support**              |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLAllocHandle      | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLBindCol          | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLBindParameter    | 2.0                    | ODBC                     | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLBrowseConnect    | 1.0                    | ODBC                     | NO                       |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLBulkOperations   | 3.0                    | ODBC                     | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLCancel           | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLCloseCursor      | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLColAttribute     | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLColumnPrivileges | 1.0                    | ODBC                     | NO                       |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLColumns          | 1.0                    | X/Open                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLConnect          | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLCopyDesc         | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLDescribeCol      | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLDescribeParam    | 1.0                    | ODBC                     | NO                       |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLDisconnect       | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLDriverConnect    | 1.0                    | ODBC                     | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLEndTran          | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLExecDirect       | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLExecute          | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLFetch            | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLFetchScroll      | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLForeignKeys      | 1.0                    | ODBC                     | YES (2008 R3.1 or later) |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLFreeHandle       | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLFreeStmt         | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetConnectAttr   | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetCursorName    | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetData          | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetDescField     | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetDescRec       | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetDiagField     | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetDiagRec       | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetEnvAttr       | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetFunctions     | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetInfo          | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetStmtAttr      | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLGetTypeInfo      | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLMoreResults      | 1.0                    | ODBC                     | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLNativeSql        | 1.0                    | ODBC                     | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLNumParams        | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLNumResultCols    | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLParamData        | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLPrepare          | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLPrimaryKeys      | 1.0                    | ODBC                     | YES (2008 R3.1 or later) |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLProcedureColumns | 1.0                    | ODBC                     | YES (2008 R3.1 or later) |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLProcedures       | 1.0                    | ODBC                     | YES (2008 R3.1 or later) |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLPutData          | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLRowCount         | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLSetConnectAttr   | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLSetCursorName    | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLSetDescField     | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLSetDescRec       | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLSetEnvAttr       | 3.0                    | ISO 92                   | NO                       |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLSetPos           | 1.0                    | ODBC                     | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLSetStmtAttr      | 3.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLSpecialColumns   | 1.0                    | X/Open                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLStatistics       | 1.0                    | ISO 92                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLTablePrivileges  | 1.0                    | ODBC                     | YES (2008 R3.1 or later) |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+
| SQLTables           | 1.0                    | X/Open                   | YES                      |
|                     |                        |                          |                          |
+---------------------+------------------------+--------------------------+--------------------------+

Backward compatibility is not supported for some CUBRID functions. Refer to information in the mapping table below to change unsupported functions into appropriate ones.

+---------------------------+------------------------+
| **ODBC 2.x Functions**    | **ODBC 3.x Functions** |
|                           |                        |
+---------------------------+------------------------+
| SQLAllocConnect           | SQLAllocHandle         |
|                           |                        |
+---------------------------+------------------------+
| SQLAllocEnv               | SQLAllocHandle         |
|                           |                        |
+---------------------------+------------------------+
| SQLAllocStmt              | SQLAllocHandle         |
|                           |                        |
+---------------------------+------------------------+
| SQLBindParam              | SQLBindParameter       |
|                           |                        |
+---------------------------+------------------------+
| SQLColAttributes          | SQLColAttribute        |
|                           |                        |
+---------------------------+------------------------+
| SQLError                  | SQLGetDiagRec          |
|                           |                        |
+---------------------------+------------------------+
| SQLFreeConnect            | SQLFreeHandle          |
|                           |                        |
+---------------------------+------------------------+
| SQLFreeEnv                | SQLFreeHandle          |
|                           |                        |
+---------------------------+------------------------+
| SQLFreeStmt with SQL_DROP | SQLFreeHandle          |
|                           |                        |
+---------------------------+------------------------+
| SQLGetConnectOption       | SQLGetConnectAttr      |
|                           |                        |
+---------------------------+------------------------+
| SQLGetStmtOption          | SQLGetStmtAttr         |
|                           |                        |
+---------------------------+------------------------+
| SQLParamOptions           | SQLSetStmtAttr         |
|                           |                        |
+---------------------------+------------------------+
| SQLSetConnectOption       | SQLSetConnectAttr      |
|                           |                        |
+---------------------------+------------------------+
| SQLSetParam               | SQLBindParameter       |
|                           |                        |
+---------------------------+------------------------+
| SQLSetScrollOption        | SQLSetStmtAttr         |
|                           |                        |
+---------------------------+------------------------+
| SQLSetStmtOption          | SQLSetStmtAttr         |
|                           |                        |
+---------------------------+------------------------+
| SQLTransact               | SQLEndTran             |
|                           |                        |
+---------------------------+------------------------+

**Note**
To get the latest information about ODBC driver, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-odbc-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-odbc-driver>`_
.
