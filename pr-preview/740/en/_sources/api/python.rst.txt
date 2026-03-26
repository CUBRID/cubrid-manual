
:meta-keywords: cubrid python driver, cubrid python api, database python driver, python database sample
:meta-description: CUBRIDdb is a Python extension package that implements Python Database API 2.0 compliant support for CUBRID. CUBRID Python driver is written based on CCI API.

*************
Python Driver
*************

**CUBRIDdb** is a Python extension package that implements Python Database API 2.0 compliant support for CUBRID. In additional to the minimal feature set of the standard Python DB API, CUBRID Python API also exposes nearly the entire native client API of the database engine in **_cubrid**.

CUBRID Python driver is written based on CCI API so affected by CCI configurations such as **CCI_DEFAULT_AUTOCOMMIT**.

.. FIXME: If you want to download Python driver or get the latest information, click http://www.cubrid.org/wiki_apis/entry/cubrid-python-driver .

Installing and Configuring Python
=================================

Linux/UNIX
----------

There are three ways to install CUBRID Python driver on Linux, UNIX, and UNIX-like operating systems. You can find instructions for each of them below.

**Requirements**

*   Operating system: 32-bit or 64-bit Linux, UNIX, or UNIX-like operating systems
*   Python: 2.4 or later (https://www.python.org/downloads/)

**Building CUBRID Python Driver from Source Code (Linux)**

To install CUBRID Python driver by compiling source code, you should have Python Development Package installed on your system. 

.. FIXME: If you do not have the package, follow the instructions stated at http://www.cubrid.org/wiki_apis/entry/install-python-development-package .

#.  For building CCI Driver, GNU Developer Toolset 8 or higher is required.

#.  Download the source code from https://www.cubrid.org/downloads#python.

#.  Extract the archive to the desired location. ::

        tar xvfz cubrid-python-11.2-latest.tar.gz

#.  Navigate to the directory where you have extracted the source code. ::

        cd RB-11.0.0

#.  Build the driver. At this and next step, make sure you are still under the root user. ::

        python setup.py build

#.  Install the driver. Here you also need root privileges. ::

        python setup.py install

**Using a Package Manager (EasyInstall) of CUBRID Python Driver (Linux)**

EasyInstall is a Python module (**easy_install**) bundled with **setuptools** that lets you automatically download, build, install, and manage Python packages. It gives you a quick way to install packages remotely by connecting to other websites via HTTP as well as connecting to the Package Index. It is somewhat analogous to the CPAN and PEAR tools for Perl and PHP, respectively. For more information about EasyInstall, see https://setuptools.readthedocs.io/en/latest/deprecated/easy_install.html.

Enter the command below to install CUBRID Python driver by using EasyInstall. ::

    easy_install CUBRID-Python

Windows
-------

To install CUBRID Python driver on Windows, first download CUBRID Python driver as follows:

*   Visit the website below to download the driver. You will be given to select your operating system and Python version installed on your system.

    https://www.cubrid.org/downloads#python

*   Extract the archive you downloaded. You should see a folder and two files in the folder. Copy these files to the **Lib** folder where your Python has been installed; by default, it is **C:\\Program Files\\Python\\Lib**.

Python Programming
==================

The CUBRIDdb package is supposed to have the following constants according to Python Database API 2.0.

+--------------+-----------+
| Name         | Value     |
+==============+===========+
| threadsafety | 2         |
+--------------+-----------+
| apilevel     | 2.0       |
+--------------+-----------+
| paramstyle   | qmark     |
+--------------+-----------+

Python Sample Program
=====================

This sample program will show steps that you need to perform in order to connect to the CUBRID database and run SQL statements from Python programming language. Enter the command line below to create a new table in your database. ::

    csql -u dba -c "CREATE TABLE posts( id integer, title varchar(255), body string, last_updated timestamp );" demodb
    csql -u dba -c "grant ALL PRIVILEGES on posts to public;" demodb

**Connecting to demodb from Python**

#.  Open a new Python console and enter the command line below to import CUBRID Python driver. 

    .. code-block:: python
    
        import CUBRIDdb
    
#.  Establish a connection to the *demodb* database located on localhost.
    
    .. code-block:: python
    
        conn = CUBRIDdb.connect('CUBRID:localhost:33000:demodb:::', 'dba', '')

For the *demodb* database, it is not required to enter any password. In a real-world scenario, you will have to provide the password to successfully connect. 
The syntax to use the `connect <https://pythonhosted.org/CUBRID-Python/_cubrid-module.html#connect>`_ () function is as follows: ::

    connect (url[,user[password]])

If the database has not started and you try to connect to it, you will receive an error such as this: ::

    Traceback (most recent call last):
      File "tutorial.py", line 3, in <module>
        conn = CUBRIDdb.connect('CUBRID:localhost:30000:demodb:dba::')
      File "/usr/local/lib/python3.5/site-packages/CUBRIDdb/__init__.py", line 61, in Connect
        return Connection(*args, **kwargs)
      File "/usr/local/lib/python3.5/site-packages/CUBRIDdb/connections.py", line 22, in __init__
        self.connection = _cubrid.connect(*args, **kwargs2)
    _cubrid.OperationalError: (-677, "ERROR: DBMS, -677, Failed to connect to database server, 'demodb', on the following host(s): localhost:localhost[CAS INFO-127.0.0.1:30000,0,0].")

If you provide wrong credentials, you will receive an error such as this: ::

    Traceback (most recent call last):
      File "tutorial.py", line 3, in <module>
        con = CUBRIDdb.connect('CUBRID:localhost:33000:demodb:::','a','b')
      File "/usr/local/lib/python3.5/site-packages/CUBRIDdb/__init__.py", line 61, in Connect
        return Connection(*args, **kwargs)
      File "/usr/local/lib/python3.5/site-packages/CUBRIDdb/connections.py", line 22, in __init__
        self.connection = _cubrid.connect(*args, **kwargs2)
    _cubrid.DatabaseError: (-165, 'ERROR: DBMS, -165, User "a" is invalid.[CAS INFO-127.0.0.1:33000,0,0].')

**Executing an INSERT Statement**

Now that the table is empty, insert data for the test. First, you have to obtain a cursor and then execute the **INSERT** statement.

.. code-block:: python

    cur = conn.cursor()
    cur.execute("INSERT INTO posts (id, title, body, last_updated) VALUES (1, 'Title 1', 'Test body #1', CURRENT_TIMESTAMP)")
    conn.commit()

The auto-commit in CUBRID Python driver is disabled by default. Therefore, you have to manually perform commit by using the `commit <https://pythonhosted.org/CUBRID-Python/_cubrid.connection-class.html#commit>`_ () function after executing any SQL statement. This is equivalent to executing **cur.execute("COMMIT")** . The opposite to executing commit() is executing `rollback <https://pythonhosted.org/CUBRID-Python/_cubrid.connection-class.html#rollback>`_ (), which aborts the current transaction.

Another way to insert data is to use prepared statements. You can safely insert data into the database by defining a row that contains the parameters and passing it to the `execute <https://pythonhosted.org/CUBRID-Python/CUBRIDdb.cursors.BaseCursor-class.html#execute>`_ () function.

.. code-block:: python

    args = (2, 'Title 2', 'Test body #2')
    cur.execute("INSERT INTO posts (id, title, body, last_updated) VALUES (?, ?, ?, CURRENT_TIMESTAMP)", args)

The entire script up to now looks like this:

.. code-block:: python

    import CUBRIDdb
    conn = CUBRIDdb.connect('CUBRID:localhost:33000:demodb:::', 'dba', '')
    cur = conn.cursor()
     
    # Plain insert statement
    cur.execute("INSERT INTO posts (id, title, body, last_updated) VALUES (1, 'Title 1', 'Test body #1', CURRENT_TIMESTAMP)")
     
    # Parameterized insert statement
    args = (2, 'Title 2', 'Test body #2')
    cur.execute("INSERT INTO posts (id, title, body, last_updated) VALUES (?, ?, ?, CURRENT_TIMESTAMP)", args)
     
    conn.commit()

**Fetching all records at a time**

You can fetch entire records at a time by using the `fetchall <https://pythonhosted.org/CUBRID-Python/CUBRIDdb.cursors.BaseCursor-class.html#fetchall>`_ () function.

.. code-block:: python

    cur.execute("SELECT * FROM posts ORDER BY last_updated")
    rows = cur.fetchall()
    for row in rows:
        print (row)

This will return the two rows inserted earlier in the following form: ::

    [1, 'Title 1', 'Test body #1', '2011-4-7 14:34:46']
    [2, 'Title 2', 'Test body #2', '2010-4-7 14:34:46']

**Fetching a single record at a time**

In a scenario where a lot of data must be returned into the cursor, you can fetch only one row at a time by using the `fetchone <https://pythonhosted.org/CUBRID-Python/CUBRIDdb.cursors.BaseCursor-class.html#fetchone>`_ () function.

.. code-block:: python

    cur.execute("SELECT * FROM posts")
    row = cur.fetchone()
    while row:
        print (row)
        row = cur.fetchone()

**Fetching as many as records desired at a time**

You can fetch a specified number of records at a time by using the `fetchmany <https://pythonhosted.org/CUBRID-Python/CUBRIDdb.cursors.BaseCursor-class.html#fetchmany>`_ () function.

.. code-block:: python

    cur.execute("SELECT * FROM posts")
    rows = cur.fetchmany(3)
    for row in rows:
        print (row)

**Accessing Metadata on the Returned Data**

If it is necessary to get information about column attributes of the obtained records, you should call the `description <https://pythonhosted.org/CUBRID-Python/_cubrid.cursor-class.html#description>`_ method.

.. code-block:: python

    for description in cur.description:
        print (description)

The output of the script is as follows: ::

    ('id', 8, 0, 0, 0, 0, 0)
    ('title', 2, 0, 0, 255, 0, 0)
    ('body', 2, 0, 0, 1073741823, 0, 0)
    ('last_updated', 15, 0, 0, 0, 0, 0)

Each of row has the following information. ::

    (column_name, data_type, display_size, internal_size, precision, scale, nullable)

For more information about numbers representing data types, see https://pythonhosted.org/CUBRID-Python/toc-CUBRIDdb.FIELD_TYPE-module.html .

**Releasing Resource**

After you have done using any cursor or connection to the database, you must release the resource by calling both object's `close <https://pythonhosted.org/CUBRID-Python/CUBRIDdb.cursors.BaseCursor-class.html#close>`_ () function.

.. code-block:: python

    cur.close()
    conn.close()

Python API
==========

Python Database API is composed of connect() module class, Connection object, Cursor object, and many other auxiliary functions. For more information, see Python DB API 2.0 Official Documentation at  https://www.python.org/dev/peps/pep-0249/.

You can find the information about CUBRID Python API at http://ftp.cubrid.org/CUBRID_Docs/Drivers/Python/.
