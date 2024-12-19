
:meta-keywords: cubrid pdo driver, cubrid php data objects, cubrid pdo api, pdo programming, pdo sample
:meta-description: The official CUBRID PHP Data Objects (PDO) driver is available as a PECL package and it implements the PDO interface to enable access from PDO to CUBRID.PDO is available with PHP 5.1. CUBRID PDO driver is based on CCI API.

************
PDO 드라이버
************

공식 CUBRID PDO(PHP Data Objects) 드라이버는 PECL 패키지로 제공되며, PDO에서 CUBRID 데이터베이스에 접근할 수 있도록 PDO 인터페이스를 제공한다. PDO는 PHP 5.1과 함께 배포되며, PHP 5.0에서는 PECL 확장으로 사용할 수 있다. PDO를 사용하려면 PHP 5 코어의 OO 기능이 필요하므로, PHP 5.0 미만 버전에서는 사용할 수 없다.

PDO는 어떤 데이터베이스를 사용하든 같은 함수를 사용할 수 있게 하는 데이터 액세스 추상화 계층(data-access abstraction layer)을 제공하며, 데이터베이스 추상화를 제공하는 것은 아니다. PDO를 데이터베이스 인터페이스 계층으로 사용하면 PHP 데이터베이스 드라이버를 바로 사용하는 경우에 비해 다음과 같은 이점이 있다.

*   작성한 PHP 코드를 다양한 데이터베이스와 함께 사용할 수 있다.
*   SQL 파라미터와 바인딩을 지원한다.
*   SQL을 더 안전하게 사용할 수 있다(구문 검사, 이스케이프, SQL 인젝션 방지 등).
*   프로그래밍 모델이 간결해진다.

따라서 CUBRID PDO 드라이버를 사용하면, 데이터베이스 인터페이스 계층으로 PDO를 사용하는 모든 응용 프로그램은 CUBRID와 함께 사용할 수 있다.

CUBRID PDO 드라이버는 CCI API를 기반으로 작성되었으므로, CCI API 및 CCI에 적용되는 **CCI_DEFAULT_AUTOCOMMIT**\ 과 같은 설정 파라미터에 영향을 받는다.

.. FIXME: 별도로 PDO 드라이버를 다운로드하거나 PDO 드라이버에 대한 최신 정보를 확인하려면 http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver\ 에 접속한다.

PDO 설치 및 설정
================

Linux
-----

**기본 환경**

*   운영체제: Linux: 32 비트 또는 64비트
*   웹 서버: Apache
*   PHP: 5.6.x, 7.1.x, 또는 7.4.x (https://www.php.net/downloads.php)

**PECL을 이용한 설치**

**PECL**\ 이 설치되어 있다면, **PECL**\ 이 소스코드 다운로드 및 컴파일을 수행하므로 다음과 같이 간단하게 CUBRID PDO 드라이버를 설치할 수 있다. 

.. FIXME: **PECL**\ 이 설치되어있지 않다면 http://www.cubrid.org/wiki_apis/entry/installing-cubrid-php-driver-using-pecl\ 을 참고하여 PECL을 설치한다.

#.  다음과 같은 명령어를 입력하여 CUBRID PDO 드라이버 최신 버전을 설치한다. ::
    
        sudo pecl install pdo_cubrid
    
    하위 버전의 드라이버가 필요하면 다음과 같이 설치할 버전을 지정할 수 있다. ::
    
        sudo pecl install pdo_cubrid-8.3.1.0003
    
    설치가 진행되는 중에 **CUBRID base install dir autodetect :**\ 라는 프롬프트가 표시된다. 설치를 원활하게 진행하기 위해서 CUBRID를 설치한 디렉터리의 전체 경로를 입력한다. 예를 들어 CUBRID가 **/home/cubridtest/CUBRID** 디렉터리에 설치되었다면, **/home/cubridtest/CUBRID**\ 를 입력한다.
    
#.  설정 파일을 수정한다.
    
    * CentOS 6.0 이상 버전이나 Fedora 15 이상 버전을 사용한다면 **pdo_cubrid.ini** 파일을 생성하고 내용에 **extension=pdo_cubrid.so** 를 입력하여 **/etc/php.d** 디렉터리에 저장한다.
    
    * 다른 운영체제를 사용한다면 **php.ini** 파일 끝에 다음 두 줄의 내용을 추가한다. **php.ini** 파일의 기본 위치는 **/etc/php5/apache2** 또는 **/etc** 이다. ::
    
        [CUBRID]
        extension=pdo_cubrid.so
    
#.  변경된 내용을 반영하려면 웹 서버를 재시작한다.

Windows
-------

**기본 환경**

*   운영체제: Windows 32 비트 또는 64비트
*   웹 서버: Apache 또는 IIS
*   PHP: 5.6.x, 7.1.x 또는 7.4.x(https://windows.php.net/download/)
*   PHP 7.1.x 또는 7.4.x 의 경우 32bit 또는 64bit용 Microsoft Visual C ++ 2015 재배포 가능 패키지를 설치해야 한다.

**빌드된 드라이버 다운로드 및 설치**

운영체제와 PHP 버전에 맞는 Windows용 CUBRID PHP/PDO 드라이버를 https://www.cubrid.org/downloads#pdo 에서 다운로드한다.

PDO 드라이버를 다운로드하면 **php_cubrid.dll** 파일을 볼 수 있으며, PDO 드라이버를 다운로드하면 **php_pdo_cubrid.dll** 파일을 볼 수 있다. 드라이버를 설치하는 방법은 다음과 같다.

#.  드라이버 파일을 기본 PHP 확장 디렉터리( **C:\\Program Files\\PHP\\ext** )에 복사한다.

#.  시스템 환경 변수를 설정한다. 시스템 환경 변수 **PHPRC**\ 의 값으로 **C:\\Program Files\\PHP**\ 가 설정되고, **Path**\ 에 **%PHPRC%**\ 와 **%PHPRC%\\ext**\ 가 추가되었는지 확인한다.

#.  **php.ini** ( **C:\\Program Files\\PHP\\php.ini** ) 파일을 열어 끝에 다음 두 줄을 추가한다. 

    ::

        [PHP_CUBRID]
        extension=php_cubrid.dll

    PDO 드라이버의 경우에는 다음 내용을 추가한다. 

    ::

        [PHP_PDO_CUBRID]
        extension = php_pdo_cubrid.dll

#.  웹 서버를 재시작한다.

PHP 드라이버 빌드
=================

CUBRID PDO 드라이버를 빌드하는 방법은 :ref:`CUBRID PHP 드라이버를 빌드하는 방법<how-to-php-driver-build>`\과 동일하므로 참고하여 진행한다.

PDO 프로그래밍
==============

.. _pdo-dsn:

데이터 원본 이름(DSN)
---------------------

PDO_CUBRID 데이터 원본 이름(DSN)은 다음과 같은 요소로 구성된다.

+-------------+-------------------------------------------------------+
| 요소        | 설명                                                  |
+=============+=======================================================+
| DSN 접두어  | DSN 접두어(prefix)는 **cubrid** 이다.                 |
+-------------+-------------------------------------------------------+
| host        | 데이터베이스 서버가 위치한 호스트의 이름을 입력한다.  |
+-------------+-------------------------------------------------------+
| port        | 데이터베이스 서버의 수신 대기 포트 번호를 입력한다.   |
+-------------+-------------------------------------------------------+
| dbname      | 데이터베이스의 이름을 입력한다.                       |
+-------------+-------------------------------------------------------+

**예제** ::

    "cubrid:host=127.0.0.1;port=33000;dbname=demodb"

미리 정의된 상수
----------------

CUBRID PDO 드라이버에 의해 정의되는 상수(predefined constants)는 CUBRID PDO 드라이버가 PHP와 함께 컴파일되거나 런타임에 동적으로 로드되는 경우에만 사용할 수 있다. 이처럼 특정 드라이버에 의해 정의된 상수를 다른 드라이버와 함께 사용하면 예상과 다르게 동작할 수도 있다.

코드가 여러 개의 드라이버와 함께 실행될 수 있다면, **PDO_ATTR_DRIVER_NAME** 속성 값을 얻어 드라이버를 확인하기 위해 `PDO::getAttribute() <http://www.php.net/manual/en/pdo.getattribute.php>`_ 함수를 사용할 수 있다.

다음 상수는 `PDO::cubrid_schema <https://www.php.net/manual/en/pdo.cubrid-schema.php>`_ () 함수를 이용하여 스키마 정보를 얻을 때 사용할 수 있다.

+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| 상수                               | 타입    | 설명                                                                                              |
+====================================+=========+===================================================================================================+
| PDO::CUBRID_SCH_TABLE              | integer | CUBRID 테이블의 이름과 타입을 얻는다.                                                             |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_VIEW               | integer | CUBRID 뷰의 이름과 타입을 얻는다.                                                                 |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_QUERY_SPEC         | integer | 뷰의 쿼리 정의를 얻는다.                                                                          |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_ATTRIBUTE          | integer | 테이블 칼럼의 속성을 얻는다.                                                                      |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_TABLE_ATTRIBUTE    | integer | 테이블의 속성을 얻는다.                                                                           |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_TABLE_METHOD       | integer | 인스턴스 메서드를 얻는다. 인스턴스 메서드는 클래스 인스턴스가 호출하는 메서드이다.                |
|                                    |         | 대부분의 작업은 인스턴스에서 실행되기 때문에 인스턴스 메서드는 클래스 메서드보다 자주 사용된다.   |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_METHOD_FILE        | integer | 테이블의 메서드가 정의된 파일의 정보를 얻는다.                                                    |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_SUPER_TABLE        | integer | 현재 테이블에 속성을 상속한 테이블의 이름과 타입을 얻는다.                                        |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_SUB_TABLE          | integer | 현재 테이블로부터 속성을 상속받은 테이블의 이름과 타입을 얻는다.                                  |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_CONSTRAINT         | integer | 테이블의 제약 조건을 얻는다.                                                                      |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_TRIGGER            | integer | 테이블의 트리거를 얻는다.                                                                         |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_TABLE_PRIVILEGE    | integer | 테이블의 권한 정보를 얻는다.                                                                      |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_COL_PRIVILEGE      | integer | 칼럼의 권한 정보를 얻는다.                                                                        |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_DIRECT_SUPER_TABLE | integer | 현재 테이블의 바로 상위 테이블을 얻는다.                                                          |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_DIRECT_PRIMARY_KEY | integer | 테이블의 기본키를 얻는다.                                                                         |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_IMPORTED_KEYS      | integer | 테이블의 외래키가 참조하는 기본키를 얻는다.                                                       |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_EXPORTED_KEYS      | integer | 테이블의 기본키를 참조하는 외래키를 얻는다.                                                       |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+
| PDO::CUBRID_SCH_CROSS_REFERENCE    | integer | 두 테이블 간의 상호 참조 관계를 얻는다.                                                           |
+------------------------------------+---------+---------------------------------------------------------------------------------------------------+

PDO 예제 프로그램
=================

CUBRID PDO 드라이버 확인
------------------------

사용 가능한 PDO 드라이버를 확인하려면 다음과 같이 `PDO::getAvailableDrivers <http://www.php.net/manual/en/pdo.getavailabledrivers.php>`_ () 함수를 사용한다.

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

위 스크립트는 다음과 같이 설치된 PDO 드라이버를 출력한다. ::

    PDO Drivers available:
    - Driver: mysql
    - Driver: pgsql
    - Driver: sqlite
    - Driver: sqlite2
    - Driver: cubrid

CUBRID 연결
-----------

데이터 원본 이름(DSN)을 사용하여 데이터베이스에 연결한다. 데이터 원본 이름에 대한 자세한 설명은 :ref:`pdo-dsn`\ 을 참고한다.

다음은 *demodb*\ 라는 CUBRID 데이터베이스에 PDO 연결을 수행하는 간단한 PHP 스크립트이다. PDO에서는 try-catch로 오류를 처리하며, 연결을 해제할 때에는 연결 객체에 **NULL**\ 을 할당한다는 것을 알 수 있다.

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

연결에 성공하면 다음과 같은 스크립트가 출력된다. ::

    PDO connect string: cubrid:dbname=demodb;host=localhost;port=30000
    PDO connection created ok!

SELECT 실행
-----------

PDO에서 SQL 질의를 수행하려면 질의나 응용 프로그램의 성격에 따라 다음 중 하나의 방법을 사용할 수 있다.

*   `query <http://www.php.net/manual/en/pdo.query.php>`_ () 함수 사용
*   prepared statements( `prepare <http://www.php.net/manual/en/pdo.prepare.php>`_ ()/ `execute <http://www.php.net/manual/en/pdostatement.execute.php>`_ ()) 함수 사용
*   `exec <http://www.php.net/manual/en/pdo.exec.php>`_ () 함수 사용

다음 예제에서는 가장 간단한 `query <http://www.php.net/manual/en/pdo.query.php>`_ () 함수를 사용한다. 리턴 값은 PDOStatement 객체인 resultset에서 $rs["column_name"]와 같이 칼럼 이름을 이용하여 얻을 수 있다.

`query <http://www.php.net/manual/en/pdo.query.php>`_ () 함수를 사용할 때에는 질의 코드가 제대로 이스케이프되었는지 확인해야 한다. 이스케이프에 대한 내용은 `PDO::quote <https://www.php.net/manual/en/pdo.quote.php>`_ ()를 참고한다.

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

위 스크립트의 결과는 다음과 같이 출력된다. ::

    Executing SQL: SELECT * FROM code
     
    X - Mixed
    W - Woman
    M - Man
    B - Bronze
    S - Silver
    G - Gold

UPDATE 실행
-----------

다음은 prepared statement와 파라미터를 사용하여 UPDATE를 실행하는 예제이다. prepared statement 대신 `exec <http://www.php.net/manual/en/pdo.exec.php>`_ () 함수를 사용할 수도 있다.

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

위 스크립트의 실행 결과는 다음과 같이 출력된다. ::

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

prepare와 bind
--------------

prepared statement는 PDO가 제공하는 유용한 기능 중 하나로, 사용하면 다음과 같은 이점이 있다.

*   SQL prepared statement는 다양한 파라미터와 함께 여러 번 실행되어도 한 번만 파싱하면 된다. 따라서 여러 번 실행되는 SQL문에 prepared statement를 사용하면 CUBRID 응용 프로그램의 성능을 높일 수 있다.
*   수동으로 파라미터를 이스케이프할 필요가 없으므로 SQL 인젝션 공격을 방지할 수 있다(그러나 SQL 질의의 다른 부분이 이스케이프되지 않은 입력으로 구성된다면 SQL 인젝션을 완전히 막을 수는 없다).

다음은 prepared statement를 이용하여 데이터를 조회하는 예이다.

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

위 스크립트의 결과는 다음과 같이 출력된다. ::

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

PDO::getAttribute() 사용
------------------------

`PDO::getAttribute <http://www.php.net/manual/en/pdo.getattribute.php>`_ () 함수는 다음과 같은 데이터베이스 연결 속성을 조회할 때 유용하다.

*   드라이버 이름
*   데이터베이스 버전
*   자동 커밋 모드 여부
*   오류 모드

속성을 쓸 수 있다면 `PDO::setAttribute <http://www.php.net/manual/en/pdo.setattribute.php>`_ () 함수를 사용하여 속성을 설정할 수 있다.

다음은 `PDO::getAttribute <http://www.php.net/manual/en/pdo.getattribute.php>`_ () 함수를 사용하여 클라이언트와 서버의 현재 버전을 조회하는 PHP PDO 스크립트이다.

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

위 스크립트의 결과는 다음과 같이 출력된다. ::

    Driver name: cubrid
    Client version: 8.3.0
    Server version: 8.3.0.0337

CUBRID PDO 확장
---------------

CUBRID PDO 확장은 데이터베이스 스키마와 메타데이터 정보를 조회하는 데 사용할 수 있는 PDO::cubrid_schema() 함수를 제공한다. 다음은 이 함수를 이용하여 *nation* 테이블의 기본키를 반환하는 스크립트이다.

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

위 스크립트의 결과는 다음과 같이 출력된다. ::

    Get PRIMARY KEY for table: nation:
    Array ( [0] => Array ( [CLASS_NAME] => nation [ATTR_NAME] => code [KEY_SEQ] => 1 [KEY_NAME] => pk_nation_code ) )

PDO API
=======

PDO API와 관련하여 http://www.php.net/manual/en/book.pdo.php\를 참고한다.

CUBRID PDO 드라이버가 제공하는 PDO API는 http://ftp.cubrid.org/CUBRID_Docs/Drivers/PDO/\를 참고한다.
