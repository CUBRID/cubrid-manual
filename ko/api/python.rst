
:meta-keywords: cubrid python driver, cubrid python api, database python driver, python database sample
:meta-description: CUBRIDdb is a Python extension package that implements Python Database API 2.0 compliant support for CUBRID. CUBRID Python driver is written based on CCI API.

***************
Python 드라이버
***************

**CUBRIDdb** 는 Python Database API 2.0을 준수하며 CUBRID 데이터베이를 지원하는 Python 확장 패키지이다. CUBRID Python API는 Python Database API가 제공하는 기본 기능 외에도, CUBRID 데이터베이스 엔진에서 제공하는 기능을 **_cubrid** 모듈에서 제공한다.

CUBRID Python 드라이버는 CCI API를 기반으로 작성되었으므로, CCI API 및 CCI에 적용되는 **CCI_DEFAULT_AUTOCOMMIT** 과 같은 설정 파라미터에 영향을 받는다.

.. FIXME: 별도로 Python 드라이버를 다운로드하거나 Python 드라이버에 대한 최신 정보를 확인하려면 http://www.cubrid.org/wiki_apis/entry/cubrid-python-driver\에 접속한다.

Python 설치 및 설정
===================

Linux/Unix
----------

Linux, Unix 및 유사 운영체제에서는 다음과 같은 세 가지 방법으로 CUBRID Python 드라이버를 설치할 수 있다.

**기본 환경**

*   운영체제: Linux: 32 비트/64비트 또는 유사 Unix 운영체제
*   Python: 2.4 이상(http://www.python.org/download/)

**Yum을 이용한 설치(Fedora, CentOS)**

#.  Yum을 이용하여 CUBRID Python 드라이버를 설치하려면, CUBRID 패키지의 위치를 Yum에 알려야 한다. 운영체제에 따라 다음 주소에 접속하여 사용하는 운영체제와 CUBRID 버전에 해당하는 명령어를 찾아 사용한다.

    *   CentOS: http://www.cubrid.org/?mid=yum_repository&os=centos
    *   Fedora: http://www.cubrid.org/?mid=yum_repository&os=fedora

    예를 들어 Fedora 17에 CUBRID 9.0을 설치한 경우에 해당하는 명령어는 다음과 같다. ::

        rpm -i http://yumrepository.cubrid.org/cubrid_repo_settings/9.0.0/cubridrepo-9.0.0-1.fc17.noarch.rpm

#.  다음 명령어를 실행하여 CUBRID Python 드라이버를 설치한다.

    ::

        yum install python-cubrid

**소스코드로 설치(Linux)**

소스코드를 컴파일하여 CUBRID Python 드라이버를 설치하려면 Python Development Package가 필요하다. 

.. FIXME: Python Development Package가 설치되어 있지 않다면 http://www.cubrid.org/wiki_apis/entry/install-python-development-package\ 를 참고하여 설치한다.

#.  소스 코드를 http://www.cubrid.org/?mid=downloads&item=python_driver\에서 다운로드한다.

#.  다음 명령어를 실행하여 원하는 위치에 다운로드한 파일의 압축을 해제한다. ::

        tar xvfz cubrid-python-src-8.4.0.0001.tar.gz

#.  압축을 해제한 디렉터리로 이동한다. ::

        cd cubrid-python-src

#.  드라이버를 빌드한다. 이 단계와 다음 단계는 루트 사용자 계정으로 실행해야 한다. ::

        python setup.py build

#.  빌드한 드라이버를 설치한다. ::

        python setup.py install

**Easy Install을 이용한 설치(Linux)**

Easy Install은 자동으로 Python 패키지를 다운로드/빌드/설치/관리할 수 있는 Python 모듈로, setuptools에 포함되어 있다. Easy Install을 사용하면 패키지 인덱스뿐만 아니라 다른 웹 사이트에도 HTTP로 연결하여 패키지를 설치할 수 있다. Perl의 CPAN이나 PHP의 PEAR와 유사하다. Easy Install에 대한 더 자세한 설명은 http://packages.python.org/distribute/easy_install.html\ 을 참고한다.

Easy Install을 이용하여 CUBRID Python 드라이버를 설치하려면 다음 명령어를 입력한다. ::

    easy_install CUBRID-Python

Windows
-------

Windows에 CUBRID Python 드라이버를 설치하려면 다음과 같이 CUBRID Python 드라이버를 다운로드하여 설치한다.

*   다음 주소에서 운영체제와 Python의 버전에 맞는 드라이버를 다운로드한다.

    http://www.cubrid.org/?mid=downloads&item=python_driver&os=windows&python=detect

*   다운로드한 파일의 압축을 해제하여 Python이 설치된 경로의 **Lib** 폴더( **C:\\Program Files\\Python\\Lib** ) 안에 복사한다.

Python 프로그래밍
=================

CUBRIDdb 패키지는 Python Database API 2.0에 따라 다음과 같은 상수를 갖는다.

+--------------+-------+
| 이름         | 값    |
+==============+=======+
| threadsafety | 2     |
+--------------+-------+
| apilevel     | 2.0   |
+--------------+-------+
| paramstyle   | qmark |
+--------------+-------+

Python 예제 프로그램
====================

여기에서는 Python으로 CUBRID 데이터베이스에 대한 작업을 수행하는 예제 프로그램을 작성한다. 예제로 다음과 같은 테이블을 생성한다. ::

    csql -u dba -c "CREATE TABLE posts( id integer, title varchar(255), body string, last_updated timestamp );" demodb

**Python에서 demodb에 연결**

#.  새 Python 콘솔을 열어 다음과 같이 Python에 CUBRID Python 드라이버를 import한다. 

    .. code-block:: python
    
        import CUBRIDdb
    
#.  localhost에 위치한 *demodb* 데이터베이스에 연결을 생성한다.
    
    .. code-block:: python
    
        conn = CUBRIDdb.connect('CUBRID:localhost:30000:dba::')

*demodb* 데이터베이스는 비밀번호가 필요하지 않으므로 비밀번호를 입력하지 않았다. 그러나 실제 데이터베이스에 연결할 때에는 비밀번호가 필요하다면 비밀번호를 입력해야 한다.
`connect <http://packages.python.org/CUBRID-Python/_cubrid-module.html#connect>`_ () 함수의 구문은 다음과 같다. ::

    connect (url[,user[password]])

연결하려는 데이터베이스가 시작되지 않았다면 다음과 같은 오류가 발생한다. ::

    Traceback (most recent call last):
      File "tutorial.py", line 3, in <module>
        conn = CUBRIDdb.connect('CUBRID:localhost:30000:dba::')
      File "/usr/local/lib/python2.6/site-packages/CUBRIDdb/__init__.py", line 48, in Connect
        return Connection(*args, **kwargs)
      File "/usr/local/lib/python2.6/site-packages/CUBRIDdb/connections.py", line 19, in __init__
        self._db = _cubrid.connect(*args, **kwargs)
    _cubrid.Error: (-1, 'ERROR: DBMS, 0, Unknown DBMS Error')

자격이 잘못되었다면 다음과 같은 오류가 발생한다. ::

    Traceback (most recent call last):
      File "tutorial.py", line 3, in <module>
        con = CUBRIDdb.connect('CUBRID:localhost:33000:demodb','a','b')
      File "/usr/local/lib/python2.6/site-packages/CUBRIDdb/__init__.py", line 48, in Connect
        return Connection(*args, **kwargs)
      File "/usr/local/lib/python2.6/site-packages/CUBRIDdb/connections.py", line 19, in __init__
        self._db = _cubrid.connect(*args, **kwargs)
    _cubrid.Error: (-1, 'ERROR: DBMS, 0, Unknown DBMS Error')

**INSERT 문 실행**

테이블이 비어있으므로 데이터를 입력한다. 먼저 커서를 얻은 후에 **INSERT** 문을 실행해야 한다.

.. code-block:: python

    cur = conn.cursor()
    cur.execute("INSERT INTO posts (id, title, body, last_updated) VALUES (1, 'Title 1', 'Test body #1', CURRENT_TIMESTAMP)")
    conn.commit()

CUBRID Python 드라이버에서는 기본적으로 자동 커밋 모드가 비활성화되어 있다. 따라서 SQL문을 실행한 후에는 수동으로 `commit <http://packages.python.org/CUBRID-Python/_cubrid.connection-class.html#commit>`_ () 함수를 사용하여 커밋을 수행해야 한다. 이 함수는 **cur.execute("COMMIT")** 와 같은 동작을 수행한다. 반대로 현재 트랜잭션을 중단하고 롤백하려면 `rollback <http://packages.python.org/CUBRID-Python/_cubrid.connection-class.html#rollback>`_ () 함수를 사용한다.

데이터를 입력하는 다른 방법으로 prepared statement를 사용할 수도 있다. 다음과 같이 파라미터를 포함하는 투플을 정의한 후 `execute <http://packages.python.org/CUBRID-Python/CUBRIDdb.cursors.Cursor-class.html#execute>`_ () 함수에 전달하여 안전하게 데이터베이스에 데이터를 입력할 수 있다.

.. code-block:: python

    args = (2, 'Title 2', 'Test body #2')
    cur.execute("INSERT INTO posts (id, title, body, last_updated) VALUES (?, ?, ?, CURRENT_TIMESTAMP)", args)

여기까지 작성한 코드는 다음과 같다.

.. code-block:: python

    import CUBRIDdb
    conn = CUBRIDdb.connect('CUBRID:localhost:33000:demodb', 'public', '')
    cur = conn.cursor()
     
    # Plain insert statement
    cur.execute("INSERT INTO posts (id, title, body, last_updated) VALUES (1, 'Title 1', 'Test body #1', CURRENT_TIMESTAMP)")
     
    # Parameterized insert statement
    args = (2, 'Title 2', 'Test body #2')
    cur.execute("INSERT INTO posts (id, title, body, last_updated) VALUES (?, ?, ?, CURRENT_TIMESTAMP)", args)
     
    conn.commit()

**전체 레코드를 한 번에 조회**

`fetchall <http://packages.python.org/CUBRID-Python/CUBRIDdb.cursors.Cursor-class.html#fetchall>`_ () 함수를 사용하면 전체 레코드를 한 번에 조회할 수 있다.

.. code-block:: python

    cur.execute("SELECT * FROM posts ORDER BY last_updated")
    rows = cur.fetchall()
    for row in rows:
        print row

위 코드는 다음과 같은 내용을 출력한다. ::

    [1, 'Title 1', 'Test body #1', '2011-4-7 14:34:46']
    [2, 'Title 2', 'Test body #2', '2010-4-7 14:34:46']

**하나의 레코드를 조회**

데이터의 양이 많다면 전체 결과를 메모리로 가져오는 대신 다음과 같이 `fetchone <http://packages.python.org/CUBRID-Python/CUBRIDdb.cursors.Cursor-class.html#fetchone>`_ () 함수를 사용하여 레코드를 한 번에 하나씩 조회할 수 있다.

.. code-block:: python

    cur.execute("SELECT * FROM posts")
    row = cur.fetchone()
    while row:
        print row
        row = cur.fetchone()

**레코드 개수를 지정하여 조회**

다음과 같이 `fetchmany <http://packages.python.org/CUBRID-Python/CUBRIDdb.cursors.Cursor-class.html#fetchmany>`_ () 함수를 사용하면 조회할 레코드의 개수를 지정할 수 있다.

.. code-block:: python

    cur.execute("SELECT * FROM posts")
    rows = cur.fetchmany(3)
    for row in rows:
        print row

**반환된 데이터의 메타데이터에 접근**

조회한 레코드의 칼럼 속성에 대한 정보가 필요하면 커서의 `description <http://packages.python.org/CUBRID-Python/_cubrid.cursor-class.html#description>`_ 메서드를 사용한다.

.. code-block:: python

    for description in cur.description:
        print description

위 코드는 다음과 같은 내용을 출력한다. ::

    ('id', 8, 0, 0, 0, 0, 0)
    ('title', 2, 0, 0, 255, 0, 0)
    ('body', 2, 0, 0, 1073741823, 0, 0)
    ('last_updated', 15, 0, 0, 0, 0, 0)

각 투플은 다음과 같은 정보를 포함한다. ::

    (column_name, data_type, display_size, internal_size, precision, scale, nullable)

데이터 타입을 나타내는 숫자에 대한 자세한 내용은 http://packages.python.org/CUBRID-Python/toc-CUBRIDdb.FIELD_TYPE-module.html 을 참고한다.

**자원 해제**

데이터베이스 연결이나 커서를 사용하는 모든 작업을 마친 후에는 객체의 `close <http://packages.python.org/CUBRID-Python/CUBRIDdb.cursors.Cursor-class.html#close>`_ () 함수를 호출하여 자원을 해제해야 한다.

.. code-block:: python

    cur.close()
    conn.close()

Python API
==========

Python Database API는 connect() 모듈 클래스와 Connection 객체, Cursor 객체, 그리고 그 밖의 보조적인 함수들로 이루어진다. 이에 대한 자세한 내용은 http://www.python.org/dev/peps/pep-0249/ 를 참고한다.

CUBRID Python API에 대한 자세한 내용은 http://ftp.cubrid.org/CUBRID_Docs/Drivers/Python/\을 참고한다.
