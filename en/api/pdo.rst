
:meta-keywords: cubrid pdo driver, cubrid php data objects, cubrid pdo api, pdo programming, pdo sample
:meta-description: The official CUBRID PHP Data Objects (PDO) driver is available as a PECL package and it implements the PDO interface to enable access from PDO to CUBRID.PDO is available with PHP 5.1. CUBRID PDO driver is based on CCI API.

**********
PDO Driver
**********

The official CUBRID PHP Data Objects (PDO) driver is available as a PECL package and it implements the PDO interface to enable access from PDO to CUBRID.PDO is available with PHP 5.1. For PHP 5.0, you can use it as a PECL extension. PDO cannot run with earlier versions of PHP 5.0 because it requires the new OO features in the core of PHP 5.0.

PDO provides a data-access abstraction layer, which means that, regardless of which database you are using, you use the same functions to issue queries and fetch data; PDO does not provide a database abstraction. Using PDO as a database interface layer can have important advantages over "direct" PHP database drivers as follows:

*   Portable PHP code between different databases and database abstraction.
*   Supports SQL parameters and bind.
*   Safer SQLs (syntax verification, escaping, it helps protect against SQL injections etc.)
*   Cleaner programming model

In particular, having a CUBRID PDO driver means that any application that uses PDO as a database interface should work with CUBRID.

CUBRID PDO driver is based on CCI API so affected by CCI configurations such as **CCI_DEFAULT_AUTOCOMMIT**.

.. FIXME: To download PDO driver or get the latest information, click http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver .

Installing and Configuring PDO
==============================

Linux
-----

**Requirements**

*   Operating system: 32-bit or 64-bit Linux
*   Web server: Apache
*   PHP: 5.6.x, 7.1.x, or 7.4.x (https://www.php.net/downloads.php)

**Installing CUBRID PHP Driver using PECL**

If **PECL** package has been installed on your system, the installation of CUBRID PDO driver is straightforward. **PECL** will download and compile the driver for you. 

.. FIXME: If you do not have **PECL** installed, follow the instructions at http://www.cubrid.org/wiki_apis/entry/installing-cubrid-php-driver-using-pecl to get it installed.

#.  Enter the following command to install the latest version of CUBRID PDO driver. ::
    
        sudo pecl install pdo_cubrid
    
    If you need earlier versions of the driver, you can install exact versions as follows: ::
    
        sudo pecl install pdo_cubrid-8.3.1.0003
    
    During the installation, you will be prompted to enter **CUBRID base install dir autodetect :**. Just to make sure your installation goes smoothly, enter the full path to the directory where you have CUBRID installed. For example, if CUBRID has been installed at **/home/cubridtest/CUBRID**, then enter **/home/cubridtest/CUBRID**.
    
#.  Edit the configuration file.

    * If you are using CentOS 6.0 and later or Fedora 15 and later, create a file named **pdo_cubrid.ini**, enter a command line **extension=pdo_cubrid.so**, and store the file in the **/etc/php.d** directory.
    
    * If you are using earlier versions of Cent0S or Fedora 15, edit the **php.ini** file (default location: **/etc/php5/apache2** or **/etc/**) and add the following two command lines at the end of the file. ::
    
        [CUBRID]
        extension=pdo_cubrid.so

#. Restart the web server to apply changes.

Windows
-------

**Requirements**

*   Operating system: 32-bit or 64-bit Windows
*   Web server: Apache or IIS
*   PHP: 5.6.x, 7.1.x, or 7.4.x (https://windows.php.net/download/)
*   For PHP 7.1.x or 7.4.x, you need to install Microsoft Visual C++ 2015 Redistributable Package for 32bit or 64bit.

**Downloading and Installing Compiled CUBRID PDO Driver**

First, download CUBRID PHP/PDO driver of which versions match the versions of your operating system and PHP installed at https://www.cubrid.org/downloads#pdo.

After you download the driver, you will see the **php_cubrid.dll** file for CUBRID PHP driver or the **php_pdo_cubrid.dll** file for CUBRID PDO driver. Follow the steps below to install it.

#.  Copy this driver to the default PHP extensions directory (usually located at **C:\\Program Files\\PHP\\ext**).

#.  Set your system environment. Check if the environment variable **PHPRC** is **C:\\Program Files\\PHP** and system variable path is added with **%PHPRC%** and **%PHPRC%\\ext**.

#.  Edit **php.ini** (**C:\\Program Files\\PHP\\php.ini**) and add the following two lines at the end of the **php.ini** file. 

    ::

        [PHP_PDO_CUBRID]
        extension=php_pdo_cubrid.dll

    For CUBRID PHP driver, add command lines below. 

    ::

        [PHP_PDO_CUBRID]
        extension = php_pdo_cubrid.dll

#.  Restart your web server to apply changes.

Building CUBRID PHP Driver from Source Code
===========================================

Building CUBRID PDO driver and :ref:`Building CUBRID PHP driver from source code<how-to-php-driver-build>` \are the same.


PDO Programming
===============

.. _pdo-dsn:

Data Source Name (DSN)
----------------------

The PDO_CUBRID data source name (DSN) consists of the following elements:

+-------------+--------------------------------------------------------+
| Element     | Description                                            |
+=============+========================================================+
| DSN prefix  | The DSN prefix is **cubrid**.                          |
+-------------+--------------------------------------------------------+
| host        | The hostname on which the database server resides      |
+-------------+--------------------------------------------------------+
| port        | The port number where the database server is listening |
+-------------+--------------------------------------------------------+
| dbname      | The name of the database                               |
+-------------+--------------------------------------------------------+

**Example** ::

    "cubrid:host=127.0.0.1;port=33000;dbname=demodb"

Predefined Constants
--------------------

The constants defined by CUBRID PDO driver are available only when the extension has been either compiled into PHP or dynamically loaded at runtime. In addition, these driver-specific constants should only be used if you are using PDO driver. Using driver-specific attributes with another driver may result in unexpected behavior.

The `PDO::getAttribute() <http://www.php.net/manual/en/pdo.getattribute.php>`_ function may be used to obtain the **PDO_ATTR_DRIVER_NAME** attribute value to check the driver if your code can run.

The constants below can be used with the `PDO::cubrid_schema <https://www.php.net/manual/en/pdo.cubrid-schema.php>`_ function to get schema information.

+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| Constant                           | Type     | Description                                                                                         |
+====================================+==========+=====================================================================================================+
| PDO::CUBRID_SCH_TABLE              | integer  | Gets name and type of table in CUBRID.                                                              |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_VIEW               | integer  | Gets name and type of view in CUBRID.                                                               |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_QUERY_SPEC         | integer  | Get the query definition of view.                                                                   |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_ATTRIBUTE          | integer  | Gets the attributes of table column.                                                                |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_TABLE_ATTRIBUTE    | integer  | Gets the attributes of table.                                                                       |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_TABLE_METHOD       | integer  | Gets the instance method. The instance method is a method called by a class instance.               |
|                                    |          | It is used more often than the class method because most operations are executed in the instance.   |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_METHOD_FILE        | integer  | Gets the information of the file where the method of the table is defined.                          |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_SUPER_TABLE        | integer  | Gets the name and type of table which table inherits attributes from.                               |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_SUB_TABLE          | integer  | Gets the name and type of table which inherits attributes from this table.                          |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_CONSTRAINT         | integer  | Gets the table constraints.                                                                         |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_TRIGGER            | integer  | Gets the table triggers.                                                                            |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_TABLE_PRIVILEGE    | integer  | Gets the privilege information of table.                                                            |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_COL_PRIVILEGE      | integer  | Gets the privilege information of column.                                                           |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_DIRECT_SUPER_TABLE | integer  | Gets the direct super table of table.                                                               |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_DIRECT_PRIMARY_KEY | integer  | Gets the table primary key.                                                                         |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_IMPORTED_KEYS      | integer  | Gets imported keys of table.                                                                        |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_EXPORTED_KEYS      | integer  | Gets exported keys of table.                                                                        |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_CROSS_REFERENCE    | integer  | Gets reference relationship of two tables.                                                          |
+------------------------------------+----------+-----------------------------------------------------------------------------------------------------+

PDO Sample Program
==================

Verifying CUBRID PDO Driver Version
-----------------------------------

If you want to verify that the CUBRID PDO driver is accessible, you can use the `PDO::getAvailableDrivers <http://www.php.net/manual/en/pdo.getavailabledrivers.php>`_ () function.

.. code-block:: php

    <?php
    echo'PDO Drivers available:
    ';
    foreach(PDO::getAvailableDrivers()as $driver)
    {
    if($driver =="cubrid"){
    echo" - Driver: <b>".$driver.'</b>
    ';
    }else{
    echo" - Driver: ".$driver.'
    ';
    }
    }
    ?>

This script will output all the currently installed PDO drivers: ::

    PDO Drivers available:
    - Driver: mysql
    - Driver: pgsql
    - Driver: sqlite
    - Driver: sqlite2
    - Driver: cubrid

Connecting to CUBRID
--------------------

Use the data source name (DSN) to connect to the database server. For details about DSN, see :ref:`pdo-dsn`.

Below is a simple PHP example script which performs a PDO connection to the CUBRID *demodb* database. You can notice that errors are handling in PDO by using a try-catch mechanism and the connection is closed by assigning **NULL** to the connection object.

.. code-block:: php

    <?php
    $database ="demodb";
    $host ="localhost";
    $port ="30000";//use default value
    $username ="dba";
    $password ="";
     
    try{
    //cubrid:host=localhost;port=33000;dbname=demodb
    $conn_str ="cubrid:dbname=".$database.";host=".$host.";port=".$port;
    echo"PDO connect string: ".$conn_str."
    ";
    $db =new PDO($conn_str, $username, $password );
    echo"PDO connection created ok!"."
    ";
    $db = null;//disconnect
    }catch(PDOException $e){
    echo"Error: ".$e->getMessage()."
    ";
    }
    ?>

If connection succeeds, the output of this script is as follows: ::

    PDO connect string: cubrid:dbname=demodb;host=localhost;port=30000
    PDO connection created ok!

Executing a SELECT Statement
----------------------------

In PDO, there is more than one way to execute SQL queries.

*   Using the `query <http://www.php.net/manual/en/pdo.query.php>`_ () function
*   Using prepared statements (see `prepare <http://www.php.net/manual/en/pdo.prepare.php>`_ ()/ `execute <http://www.php.net/manual/en/pdostatement.execute.php>`_ ()) functions)
*   Using the `exec <http://www.php.net/manual/en/pdo.exec.php>`_ () function

The example script below shows the simplest one - using the `query <http://www.php.net/manual/en/pdo.query.php>`_ () function. You can retrieve the return values from the resultset (a PDOStatement object) by using the column names, like $rs["*column_name*"].

Note that when you use the `query <http://www.php.net/manual/en/pdo.query.php>`_ () function, you must ensure that the query code is properly escaped. For information about escaping, see `PDO::quote <https://www.php.net/manual/en/pdo.quote.php>`_ () function.

.. code-block:: php

    <?php
    include("_db_config.php");
    include("_db_connect.php");
     
    $sql ="SELECT * FROM code";
    echo"Executing SQL: <b>".$sql.'</b>
    ';
    echo'
    ';
     
    try{
    foreach($db->query($sql)as $row){
    echo $row['s_name'].' - '. $row['f_name'].'
    ';
    }
    }catch(PDOException $e){
    echo $e->getMessage();
    }
     
    $db = null;//disconnect
    ?>

The output of the script is as follows: ::

    Executing SQL: SELECT * FROM code
     
    X - Mixed
    W - Woman
    M - Man
    B - Bronze
    S - Silver
    G - Gold

Executing an UPDATE Statement
-----------------------------

The following example shows how to execute an UPDATE statement by using a prepared statement and parameters. You can use the `exec <http://www.php.net/manual/en/pdo.exec.php>`_ () function as an alternative.

.. code-block:: php

    <?php
    include("_db_config.php");
    include("_db_connect.php");
     
    $s_name ='X';
    $f_name ='test';
    $sql ="UPDATE code SET f_name=:f_name WHERE s_name=:s_name";
     
    echo"Executing SQL: <b>".$sql.'</b>
    ';
    echo'
    ';
     
    echo":f_name: <b>".$f_name.'</b>
    ';
    echo'
    ';
    echo":s_name: <b>".$s_name.'</b>
    ';
    echo'
    ';
     
    $qe = $db->prepare($sql);
    $qe->execute(array(':s_name'=>$s_name,':f_name'=>$f_name));
     
    $sql ="SELECT * FROM code";
    echo"Executing SQL: <b>".$sql.'</b>
    ';
    echo'
    ';
     
    try{
    foreach($db->query($sql)as $row){
    echo $row['s_name'].' - '. $row['f_name'].'
    ';
    }
    }catch(PDOException $e){
    echo $e->getMessage();
    }
     
    $db = null;//disconnect
    ?>

The output of the script is as follows: ::

    Executing SQL: UPDATE code SET f_name=:f_name WHERE s_name=:s_name
     
    :f_name: test
     
    :s_name: X
     
    Executing SQL: SELECT * FROM code
     
    X - test
    W - Woman
    M - Man
    B - Bronze
    S - Silver
    G - Gold

Using prepare and bind
----------------------

Prepared statements are one of the major features offered by PDO and you can take following benefits by using them.

*   SQL prepared statements need to be parsed only once even if they are executed multiple times with different parameter values. Therefore, using a prepared statement minimizes the resources and ,in general, the prepared statements run faster.
*   It helps to prevent SQL injection attacks by eliminating the need to manually quote the parameters; however, if other parts of the SQL query are being built up with unescaped input, SQL injection is still possible.

The example script below shows how to retrieve data by using a prepared statement.

.. code-block:: php

    <?php
    include("_db_config.php");
    include("_db_connect.php");
     
    $sql ="SELECT * FROM code WHERE s_name NOT LIKE :s_name";
    echo"Executing SQL: <b>".$sql.'</b>
    ';
     
    $s_name ='xyz';
    echo":s_name: <b>".$s_name.'</b>
    ';
     
    echo'
    ';
     
    try{
    $stmt = $db->prepare($sql);
     
    $stmt->bindParam(':s_name', $s_name, PDO::PARAM_STR);
    $stmt->execute();
     
    $result = $stmt->fetchAll();
    foreach($result as $row)
    {
    echo $row['s_name'].' - '. $row['f_name'].'
    ';
    }
    }catch(PDOException $e){
    echo $e->getMessage();
    }
    echo'
    ';
     
    $sql ="SELECT * FROM code WHERE s_name NOT LIKE :s_name";
    echo"Executing SQL: <b>".$sql.'</b>
    ';
     
    $s_name ='X';
    echo":s_name: <b>".$s_name.'</b>
    ';
     
    echo'
    ';
     
    try{
    $stmt = $db->prepare($sql);
     
    $stmt->bindParam(':s_name', $s_name, PDO::PARAM_STR);
    $stmt->execute();
     
    $result = $stmt->fetchAll();
    foreach($result as $row)
    {
    echo $row['s_name'].' - '. $row['f_name'].'
    ';
    }
    $stmt->closeCursor();
    }catch(PDOException $e){
    echo $e->getMessage();
    }
    echo'
    ';
     
    $db = null;//disconnect
    ?>

The output of the script is as follows: ::

    Executing SQL: SELECT * FROM code WHERE s_name NOT LIKE :s_name
    :s_name: xyz
     
    X - Mixed
    W - Woman
    M - Man
    B - Bronze
    S - Silver
    G - Gold
     
    Executing SQL: SELECT * FROM code WHERE s_name NOT LIKE :s_name
    :s_name: X
     
    W - Woman
    M - Man
    B - Bronze
    S - Silver
    G - Gold

Using the PDO::getAttribute() Function
--------------------------------------

The `PDO::getAttribute <http://www.php.net/manual/en/pdo.getattribute.php>`_ () function is very useful to retrieve the database connection attributes. For example,

*   Driver name
*   Database version
*   Auto-commit state
*   Error mode

Note that if you want to set attributes values (assuming that they are writable), you should use the `PDO::setAttribute <http://www.php.net/manual/en/pdo.setattribute.php>`_ function.

The following example script shows how to retrieve the current versions of client and server by using the `PDO::getAttribute <http://www.php.net/manual/en/pdo.getattribute.php>`_ () function.

.. code-block:: php

    <?php
    include("_db_config.php");
    include("_db_connect.php");
     
    echo"Driver name: <b>".$db->getAttribute(PDO::ATTR_DRIVER_NAME)."</b>";
    echo"
    ";
    echo"Client version: <b>".$db->getAttribute(PDO::ATTR_CLIENT_VERSION)."</b>";
    echo"
    ";
    echo"Server version: <b>".$db->getAttribute(PDO::ATTR_SERVER_VERSION)."</b>";
    echo"
    ";
     
    $db = null;//disconnect
    ?>

The output of the script is as follows: ::

    Driver name: cubrid
    Client version: 8.3.0
    Server version: 8.3.0.0337

CUBRID PDO Extensions
---------------------

In CUBRID, the PDO::cubrid_schema() function is offered as an extension; the function is used to retrieve the database schema and metadata information. Below is an example script that returns information about primary key for the *nation* table by using this function.

.. code-block:: php

    <?php
    include("_db_config.php");
    include("_db_connect.php");
    try{
    echo"Get PRIMARY KEY for table: <b>nation</b>:
     
    ";
    $pk_list = $db->cubrid_schema(PDO::CUBRID_SCH_PRIMARY_KEY,"nation");
    print_r($pk_list);
    }catch(PDOException $e){
    echo $e->getMessage();
    }
     
    $db = null;//disconnect
    ?>

The output of the script is as follows: ::

    Get PRIMARY KEY for table: nation:
    Array ( [0] => Array ( [CLASS_NAME] => nation [ATTR_NAME] => code [KEY_SEQ] => 1 [KEY_NAME] => pk_nation_code ) )

PDO API
=======

For more information about PHP Data Objects (PDO) API, see http://www.php.net/manual/en/book.pdo.php. The API provided by CUBRID PDO driver is as follows:

For more information about CUBRID PDO API provides, see http://ftp.cubrid.org/CUBRID_Docs/Drivers/PDO/.
