*************
OLE DB Driver
*************

OLE DB (Object Linking and Embedding, Database) is an API designed by Microsoft for accessing data from a variety of sources in a uniform manner so it can be used by all Microsoft platforms. It is a set of interfaces implemented using the Component Object Model (COM).

.NET Framework is a software framework for Microsoft Windows operating systems. It includes a large library and it supports several programming languages which allows language interoperability (each language can utilize code written in other languages). The .NET library is available to all programming languages that .NET supports. A data provider in the .NET Framework serves as a bridge between an application and a data source; a data provider is used to retrieve data from a data source and to reconcile changes to that data back to the data source.

CUBRID OLE DB driver is written based on CCI API so affected by CCI configurations such as **CCI_DEFAULT_AUTOCOMMIT**.

To download OLD DB driver or get the latest information, click http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver .

Installing and Configuring OLE DB
=================================

**Requirements**

*   Windows Vista or Windows 7 (32-bit version)
*   CUBRID 2008 R4.1 (8.4.1.) or later (32-bit version)
*   Visual Studio 2010 Express Edition ( http://www.microsoft.com/visualstudio/en-us/products/2010-editions/express )
*   .NET Framework 3.5 or later ( http://www.microsoft.com/download/en/details.aspx?id=21 )

**CUBRID OLE DB Provider**

Before you start developing applications with CUBRID, you will need the Provider driver (**CUBRIDProvider.dll**). Make sure to download the appropriate DLL according to your CUBRID installation. You can check the CUBRID version by entering the command, **cubrid -- version**. You have four options to get the driver.

*   **Using an official installer** : Use the official CUBRID OLE DB Data Provider installer. You can download it below.

    http://www.cubrid.org/?mid=downloads&item=oledb_driver

    We provide brief video tutorial to see how to install CUBRID Provider using the installer; to watch that video, go to the website below.

    http://www.youtube.com/watch?v=FN_6c9x9UOA

*   **Modifying the official installer** : If you want to modify the official installer, you can build the CUBRID OLE DB Provide installer by compiling source code yourself. For information about modifying an installer, see the link below.

    http://www.cubrid.org/wiki_apis/entry/compiling-the-cubrid-ole-db-installer

*   **Installing the driver** : Download the CUBRID OLE DB driver at the location below. You should choose the appropriate version of the driver depending on your operating system (32 bit or 64 bit).

    http://www.cubrid.org/?mid=downloads&item=oledb_driver

*   **Building from source code** : Compile the driver source code. You can check out the source code at the SVN storage below. To compile the source code, the Visual Studio Express Edition ( http://www.microsoft.com/express/Downloads/#2010-Visual-CPP ) is required. If you are using CUBRID 9.1.0 version, enter 9.1.0 in <*CUBRID version*>.

    ::

        http://svn.cubrid.org/cubridapis/oledb/branches/RB-*<CUBRID version>*/Source/

If you do not use the CUBRID OLED DB Provider installer, you should execute the command below to register the driver. The version of the driver should match the version of your operating system. For 32 bit, the **regsvr32** command should be executed in the **C:\Windows\system32** directory; for 64 bit, the **regsvr32** command should be executed in the **C:\Windows\SysWOW64** directory. ::

    regsvr32 CUBRIDProvider.dll

OLE DB Programming
==================

Using Data Link Properties Dialog Box
-------------------------------------

In the [Data Link Properties] dialog box, you can check and configure various OLE DB providers provided by the Windows operating system that you are currently using.

If you have properly installed the CUBRID OLE DB Provider for Windows, "CUBRID OLE DB Provider" will be displayed in the provider list of the [Data Link Properties] dialog box as shown below.

.. image:: /images/image84.png

If you click the [Next] button after selecting "CUBRID OLE DB Provider," the [Connection] tab will appear as shown below. Set the desired link properties in the [Connection] tab.

.. image:: /images/image85.png

*   **Data Source**: Enter the name of the CUBRID database.
*   **Location**: Enter the IP address or host name of the server where the CUBRID broker is running.
*   **User name**: Enter the user name to be used for logon to the database server.
*   **Password**: Enter the password to be used for logon to the database server.

Set all values of connection properties and then click the [All] tab.

.. image:: /images/image86.png

You can check every value currently configured in the [All] tab; to edit the value within the tab, double-click the item you want to edit. Enter the desired value in the [Edit Property Value] dialog box and then click [OK]. The figure above shows an example which configures the value of [Port] to "31000," and the value of [Fetch Size] is "100."

You can check whether the connection is working properly by clicking the [Test Connection] button in the [Connection] tab after completing all configurations.

.. image:: /images/image87.png

Configuring Connection String
-----------------------------

When you do programming on the CUBRID OLE DB Provider using ActiveX Data Objects (ADO) or ADO.NET, you should write connection string as follows:

+-------------+----------------+---------------------------------------------------------+
| Item        | Example        | Description                                             |
+=============+================+=========================================================+
| Provider    | CUBRIDProvider | Provider name                                           |
+-------------+----------------+---------------------------------------------------------+
| Data Source | demodb         | Database name                                           |
+-------------+----------------+---------------------------------------------------------+
| Location    | 127.0.0.1      | The IP address or host name of the CUBRID broker server |
+-------------+----------------+---------------------------------------------------------+
| User ID     | PUBLIC         | User ID                                                 |
+-------------+----------------+---------------------------------------------------------+
| Password    | xxx            | Password                                                |
+-------------+----------------+---------------------------------------------------------+
| Port        | 33000          | The broker port number                                  |
+-------------+----------------+---------------------------------------------------------+
| Fetch Size  | 100            | Fetch size                                              |
+-------------+----------------+---------------------------------------------------------+

A connection string using the example above is as follows: ::

    "Provider=CUBRIDProvider;Data Source=demodb;Location=127.0.0.1;User ID=PUBLIC;Password=xxx;Port= 33000;Fetch Size=100"

.. note::

    *   Because a semi-colon (;) is used as a separator in URL string, it is not allowed to use a semi-colon as parts of a password (PWD) when specifying the password in connection string.
    *   If a string longer than defined max length is inserted (**INSERT**) or updated (**UPDATE**), the string will be truncated.
    *   The database connection in thread-based programming must be used independently each other.
    *   In autocommit mode, the transaction is not committed if all results are not fetched after running the SELECT statement. Therefore, although in autocommit mode, you should end the transaction by executing COMMIT or ROLLBACK if some error occurs during fetching for the resultset.

Multi-Threaded Programming in .NET Environment
----------------------------------------------

Additional considerations when you do programming with the CUBRID OLE DB Provider in the Microsoft .NET environment are as follows:

If you do multi-threaded programming using ADO.NET in the management environment, you need to change the value of the **ApartmentState** attribute of the Thread object to a **ApartmentState.STA** value because the CUBRID OLE DB Provider supports the Single Threaded Apartment (STA) attribute only.

Without any changes of given values, the default value of the attribute in the Thread object returns Unknown value, causing it to malfunction during multi-threaded programming.

.. warning::

    All OLE DB objects are the Component Object Model. Of COM threading model, the CUBRID OLE DB Provider currently supports the apartment threading model only, which is available in every multi-threaded environment as well as .NET environment.

OLE DB API
==========

For more information about OLE DB API, see Micorosoft OLE DB documentation at  http://msdn.microsoft.com/en-us/library/ms722784%28vs.85%29.aspx .
