************
PHP 드라이버
************

**개요**

CUBRID PHP 드라이버는 PHP로 작성한 응용 프로그램에서 CUBRID 데이터베이스를 사용할 수 있는 API를 제공한다. CUBRID PHP 드라이버가 제공하는 모든 함수는 앞에
**cubrid_**
가 붙는다(예: cubrid_connect(), cubrid_connect_with_url()).

공식 CUBRID PHP 드라이버는 PECL 패키지로 제공한다. PECL은 PHP 확장 저장소로, PHP 확장 개발 및 다운로드를 위한 편의 기능을 제공한다. PECL에 대한 자세한 내용은
`http://pecl.php.net/ <http://pecl.php.net/>`_
을 참고한다.

CUBRID PHP?드라이버는 CCI API를 기반으로 작성되었으므로, CCI API 및 CCI에 적용되는
**CCI_DEFAULT_AUTOCOMMIT**
과 같은 설정 파라미터에 영향을 받는다.

별도로 PHP?드라이버를 다운로드하거나 PHP?드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-php-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-php-driver>`_
에 접속한다.

**PHP**
**설치**
**및**
**설정**

가장 쉽고 빠르게 응용 프로그램을 시스템에 설치하려면 Ubuntu에 CUBRID와 Apache, PHP를 설치한다. 설치 방법은
`http://www.cubrid.org/wiki_apis/entry/install-cubrid-with-apache-and-php-on-ubuntu <http://www.cubrid.org/wiki_apis/entry/install-cubrid-with-apache-and-php-on-ubuntu>`_
를 참고한다.

**Linux**

**기본**
**환경**

*   CUBRID: 2008 R3.0(8.3.0) 이상



*   운영체제: Linux: 32 비트 또는 64비트



*   웹 서버: Apache



*   PHP: 5.2 또는 5.3(
    `http://php.net/downloads.php <http://php.net/downloads.php>`_
    )



**PECL을**
**이용한**
**설치**

**PECL**
이 설치되어 있다면,
**PECL**
이 소스코드 다운로드 및 컴파일을 수행하므로 다음과 같이 간단하게 CUBRID PHP 드라이버를 설치할 수 있다.
**PECL**
이 설치되어있지 않다면
`http://www.cubrid.org/wiki_apis/entry/installing-cubrid-php-driver-using-pecl <http://www.cubrid.org/wiki_apis/entry/installing-cubrid-php-driver-using-pecl>`_
을 참고하여 PECL을 설치한다.

*   다음과 같은 명령어를 입력하여 CUBRID PHP 드라이버 최신 버전을 설치한다.



sudo pecl install cubrid

하위 버전의 드라이버가 필요하면 다음과 같이 설치할 버전을 지정할 수 있다.

sudo pecl install cubrid-8.3.0.0005

설치가 진행되는 중에
**CUBRID base install dir autodetect :**
라는 프롬프트가 표시된다. 설치를 원활하게 진행하기 위해서 CUBRID를 설치한 디렉터리의 전체 경로를 입력한다. 예를 들어 CUBRID가
**/home/cubridtest/CUBRID**
디렉터리에 설치되었다면,
**/home/cubridtest/CUBRID**
를 입력한다.

*   설정 파일을 수정한다.



*   CentOS 6.0 이상 버전이나 Fedora 15 이상 버전을 사용한다면
    **pdo_cubrid.ini**
    파일을 생성하고 내용에
    **extension=pdo_cubrid.so**
    를 입력하여
    **/etc/php.d**
    디렉터리에 저장한다.



*   다른 운영체제를 사용한다면
    **php.ini**
    파일 끝에 다음 두 줄의 내용을 추가한다.
    **php.ini**
    파일의 기본 위치는
    **/etc/php5/apache2**
    또는
    **/etc**
    이다.



[CUBRID]

extension=cubrid.so

*   변경된 내용을 반영하려면 웹 서버를 재시작한다.



**apt-get을**
**이용하여**
**Ubuntu에**
**설치**

*   PHP가 설치되어 있지 않다면, 다음 명령어로 PHP를 설치한다.



sudo apt-get install php5

*   **apt-get**
    를 이용하여 CUBRID PHP 드라이버를 설치하려면, Unbuntu가 패키지 다운로드 위치를 알고 인덱스를 업데이트하도록 CUBRID 저장소를 추가해야 한다.



sudo add-apt-repository ppa:cubrid/cubrid

sudo apt-get update

*   다음과 같이 드라이버를 설치한다.



sudo apt-get install php5-cubrid

최신 버전보다 하위 버전을 설치하려면 다음과 같이 버전을 명시한다.

sudo apt-get install php5-cubrid-8.3.1

위 명령어는
**cubrid.so**
드라이버를
**/usr/lib/php5/2009***
디렉터리에 복사하고 다음과 같은 설정을
**/etc/php5/apache2/php.ini**
파일에 추가한다.

[PHP_CUBRID]

extension=cubrid.so

*   PHP가 모듈을 읽어들이도록 Apache 서버를 재시작한다.



service apache2 restart

**Yum을**
**이용하여**
**Fedora/CentOS에**
**설치**

*   **Yum**
    을 이용하여 CUBRID PHP 드라이버를 설치하려면
    **Yum**
    에 CUBRID 패키지의 위치를 알려야 한다. 사용하는 운영체제에 따라 다음 주소에 접속한다.



*   CentOS:
    `http://www.cubrid.org/?mid=yum_repository&os=centos <http://www.cubrid.org/?mid=yum_repository&os=centos>`_



*   Fedora:
    `http://www.cubrid.org/?mid=yum_repository&os=fedora <http://www.cubrid.org/?mid=yum_repository&os=fedora>`_



*   위 주소에서 운영체제와 CUBRID의 버전에 맞는 명령을 찾아 실행한다. 예를 들어 Fedora 16과 CBURID 9.0에 해당하는 드라이버를 설치하려면 다음 명령을 실행한다.
    **Yum**
    저장소 주소의 fc16은 Fedora 16을 의미한다.



rpm -i http://yumrepository.cubrid.org/cubrid_repo_settings/9.0.0/cubridrepo-9.0.0-1.fc16.noarch.rpm

CentOS의 예는 다음과 같다. el6.2는 CentOS 6.2를 의미한다.

rpm -i http://yumrepository.cubrid.org/cubrid_repo_settings/9.0.0/cubridrepo-9.0.0-1.el6.2.noarch.rpm

위 명령어를 실행하면
**Yum**
은 CUBRID 패키지의 위치를 알게 된다.

*   CUBRID PHP 드라이버를 설치하려면 다음 명령을 실행한다.



yum install php-cubrid

*   웹 서버를 재시작한다.



service httpd restart

**Windows**

**기본**
**환경**

*   CUBRID: 2008 R3.0(8.3.0) 이상



*   운영체제: Windows 32 비트 또는 64비트



*   웹 서버: Apache 또는 IIS



*   PHP: 5.2 또는 5.3(
    `http://windows.php.net/download/ <http://windows.php.net/download/>`_
    )



**CUBRID PHP API Installer를**
**사용한**
**설치**

CUBRID PHP API Installer는 자동으로 CUBRID와 PHP의 버전을 인식하여 해당 버전에 맞는 드라이버를 설치하는 Windows 설치 관리자이다. 드라이버를 기본 PHP 확장 디렉터리(
**C:\Program Files\PHP\ext**
)에 복사하고
**php.ini**
파일을 수정한다. 여기에서는 CUBRID PHP API Installer를 이용하여 Windows에 CUBRID PHP 확장을 설치하는 방법을 설명한다.

CUBRID PHP 드라이버를 제거하려면 CUBRID PHP API Installer를 다시 실행하여 프로그램 제거를 선텍한다. 이 방법으로 CUBRID PHP 드라이버를 제거하면 설치할 때 발생한 모든 변경 사항이 복구된다.

CUBRID PHP 드라이버를 설치하기 전에 PHP와 CUBRID의 경로가 시스템 변수의
**Path**
에 추가되어 있어야 한다.

*   다음 주소에서 CUBRID PHP API Installer를 다운로드한다. 아래 주소에서는 모든 CUBRID 버전에 대한 CUBRID PHP 드라이버를 제공한다.



`http://www.cubrid.org/?mid=downloads&item=php_driver&os=windows <http://www.cubrid.org/?mid=downloads&item=php_driver&os=windows>`_

*   CUBRID PHP API Installer를 실행하고 [다음]을 클릭하여 설치를 진행한다.



*   BSD 라이선스 조항에 동의하고 [다음]을 클릭한다.



*   CUBRID PHP API Installer를 설치할 경로를 지정하고 [다음]을 클릭한다. PHP를 설치한 경로가 아니라 예를 들면
    **C:\Program Files\CUBRID PHP API**
    와 같은 새로운 경로를 입력해야 한다.



*   Windows [시작] 메뉴의 폴더 이름을 지정하고 [설치]를 클릭한다. 설치에 실패하면 아래의
    `환경 변수 설정 <#api_api_php_install_htm_error_me_8941>`_
    을 참고한다.



*   설치를 마치면 [마침]을 클릭한다.



*   변경 내용을 반영하기 위해서 웹 서버를 재시작한다. 제대로 설치되었는지 확인하려면 phpinfo()를 실행한다.



|image56_png|

**시스템 환경 변수 설정**

설치 중에 오류가 발생하면 시스템 환경 변수가 제대로 설정되었는지 확인해야 한다. CUBRID를 설치하면 자동으로 설치 경로가 시스템 환경 변수
**Path**
에 추가된다. 시스템 환경 변수가 제대로 설치되었는지 확인하려면, Windows의 [시작] > [모든 프로그램] > [보조프로그램] > [명령 프롬프트]를 실행하고 다음 작업을 수행한다.

*   다음 명령을 입력한다.



php --version

시스템 환경 변수가 제대로 설정되었다면 아래와 같이 PHP 버전을 확인할 수 있다.

C:\Users\Administrator>php --version

PHP 5.2.9 <cli> <built: Feb 25 2009 15:52:24>

*   다음 명령을 입력한다.



php --version

시스템 환경 변수가 제대로 설정되었다면 아래와 같이 CUBRID 버전을 확인할 수 있다.

C:\Users\Administrator>cubrid --version

cubrid <cubrid utilities> R2.1

위와 같은 결과가 출력되지 않는다면 PHP와 CUBRID가 설치되지 않았을 가능성이 높으므로 PHP와 CUBRID를 다시 설치한다. 만약 다시 설치해도 시스템 환경 변수가 제대로 설정되지 않는다면, 다음과 같이 수동으로 시스템 환경 변수를 설정한다.

*   [내 컴퓨터]를 마우스 오른쪽 버튼으로 클릭하여 [속성]을 선택하면 [시스템 속성] 대화 상자가 나타난다.



*   [고급] 탭을 선택하고 [환경 변수]를 클릭한다.



*   [시스템 변수]에서
    **Path**
    를 선택하고 [편집]을 클릭한다.



*   변수 값에 CUBRID와 PHP의 설치 경로를 추가한다. 각 경로는 세미콜론(;)으로 구분한다. 만약 PHP를
    **C:\Program Files\PHP**
    디렉터리에 설치하고 CUBRID를
    **C:\CUBRID\bin**
    디렉터리에 설치했다면, 변수 값의 끝에
    **C:\CUBRID\bin;C:\Program Files\PHP**
    를 덧붙인다.



*   [확인]을 클릭한다.



*   앞에서 설명한 방법으로 시스템 환경 변수가 제대로 설정되었는지 확인한다.



**빌드된**
**드라이버**
**다운로드**
**및**
**설치**

운영체제와 PHP 버전에 맞는 Windows용 CUBRID PHP/PDO 드라이버를
`http://www.cubrid.org/?mid=downloads&item=php_driver&os=windows&ostype=any&php=any&driver_type=phpdr <http://www.cubrid.org/?mid=downloads&item=php_driver&os=windows&ostype=any&php=any&driver_type=phpdr>`_
에서 다운로드한다.

PHP 드라이버를 다운로드하면
**php_cubrid.dll**
파일을 볼 수 있으며, PDO 드라이버를 다운로드하면
**php_pdo_cubrid.dll**
파일을 볼 수 있다. 드라이버를 설치하는 방법은 다음과 같다.

*   드라이버 파일을 기본 PHP 확장 디렉터리(
    **C:\Program Files\PHP\ext**
    )에 복사한다.



*   시스템 환경 변수를 설정한다. 시스템 환경 변수
    **PHPRC**
    의 값으로
    **C:\Program Files\PHP**
    가 설정되고,
    **Path**
    에
    **%PHPRC%**
    와
    **%PHPRC\ext**
    가 추가되었는지 확인한다.



*   **php.ini**
    (
    **C:\Program Files\PHP\php.ini**
    ) 파일을 열어 끝에 다음 두 줄을 추가한다.



[PHP_CUBRID]

extension=php_cubrid.dll

PDO 드라이버의 경우에는 다음 내용을 추가한다.

[PHP_PDO_CUBRID]

extension = php_pdo_cubrid.dll

*   웹 서버를 재시작한다.



**참고**
PHP 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-php-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-php-driver>`_
에 접속한다.

**PHP**
**드라이버**
**빌드**

**Linux**

여기에서는 Linux에서 CUBRID PHP 드라이버를 빌드하는 방법을 설명한다.

**환경 설정**

*   CUBRID: CUBRID를
    설치한다. 시스템에 환경 변수
    **%CUBRID%**
    가 정의되어 있는지 확인한다.



*   PHP 5.3 소스코드: PHP 5.3 소스코드를 다음 주소에서 다운로드한다.



`http://php.net/downloads.php <http://php.net/downloads.php>`_

*   Apache 2: PHP 테스트에 Apache 2를 사용할 수 있다.



*   CUBRID PHP 드라이버 소스코드:
    `http://www.cubrid.org/?mid=downloads&item=php_driver <http://www.cubrid.org/?mid=downloads&item=php_driver>`_
    에서 CUBRID 버전에 맞는 CUBRID PHP 드라이버의 소스코드를 다운로드한다.



**CUBRID PHP 드라이브 빌드**

*   PHP 소스코드를 압축 해제하여 해당 디렉터리로 이동한다.



$> tar zxvf php-<version>.tar.gz (or tar jxvf php-<version>.tar.bz2)

$> cd php-<version>/ext?

*   phpize를 실행한다. phpize에 대한 내용은
    `참고 사항 <#api_api_php_build_htm_remark>`_
    을 참고한다..



cubrid-php> /usr/bin/phpize

*   프로젝트를 설정한다. 설정을 실행하기 전에 먼저
    **./configure**
    **?**
    **h**
    를 실행하여 설정 옵션을 확인하는 것을 권장한다. 설정 방법은 다음과 같다(Apache 2가
    **/usr/local**
    에 설치되어 있다고 가정한다).



cubrid-php>./configure --with-cubrid --with-php-config=/usr/local/bin/php-config

*   --with-cubrid=shared: CUBRID 지원을 포함한다.



*   --with-php-config=PATH: 절대 경로를 포함한 php-config의 파일 이름을 입력한다.



*   프로젝트를 빌드한다. 프로젝트가 성공적으로 빌드되면
    **/modules**
    디렉터리에
    **cubrid.so**
    파일이 생성된다.



*   **cubrid.so**
    파일을
    **/usr/local/php/lib/php/extensions**
    디렉터리에 복사한다.



cubrid-php> mkdir /usr/local/php/lib/php/extensions

cubrid-php> cp modules/cubrid.so /usr/local/php/lib/php/extensions

*   **php.ini**
    파일에
    **extension_dir**
    변수에 PHP 확장의 경로를 입력하고
    **extension**
    변수에 CUBRID PHP 드라이버 파일 이름을 입력한다.



extension_dir = "/usr/local/php/lib/php/extension/no-debug-zts-xxx"

extension = cubrid.so

**CUBRID PHP 드라이버 설치 확인**

*   다음과 같은 내용의
    **test.php**
    파일을 생성한다.



<?php phpinfo(); ?>

*   웹 브라우저로
    http://localhost/test.php에 접속하여 다음 내용이 보이는지 확인한다. 다음 내용이 보이면 설치가 완료된 것이다.



+---------+------------+
| CUBRID  | Value      |
|         |            |
+---------+------------+
| Version | 9.0.0.XXXX |
|         |            |
+---------+------------+

**참고 사항**

phpize는 PHP 확장의 컴파일을 준비하는 셸 스크립트로, 일반적으로 PHP를 설치할 때 자동으로 설치된다. 만약 phpize가 설치되어 있지 않으면 다음과 같은 방법으로 설치할 수 있다.

*   PHP 소스코드를 다운로드한다. PHP 확장을 사용할 버전과 일치하는 버전을 다운로드해야 한다. 다운로드한 PHP 소스코드를 압축 해제하고 소스코드의 최상위 디렉터리로 이동한다.



$> tar zxvf php-<version>.tar.gz (or tar jxvf php-<version>.tar.bz2)

$> cd php-<version>

*   프로젝트를 설정하고, 빌드한 후 설치한다.
    **prefix**
    옵션으로 PHP를 설치할 디렉터리를 지정할 수 있다.



php-root> ./configure --prefix=prefix_dir; make; make install

*   phpize는
    **prefix_dir/bin**
    디렉터리에 위치한다.



**Windows**

여기에서는 Windows에서 CUBRID PHP 드라이버를 빌드하는 방법을 설명한다.

어떤 버전을 선택해야 할지 알 수 없는 경우 다음 내용을 참고한다.

*   Apache 1 또는 Apache 2에서 PHP를 사용하는 경우 PHP VC6 버전을 사용해야 한다.



*   IIS에서 PHP를 사용하는 경우 PHP VC9 버전을 사용해야 한다.



VC6 버전은 기존 Visual Studio 6 컴파일러로 컴파일된다. VC9 버전은 Visual Studio 2008 컴파일러로 컴파일되며, 성능과 안정성이 개선되었다.

VC9 버전을 컴파일하려면 Visual C++ 2008 Runtime이 필요하다. VC9 버전은 Apache Software Foundation(
`http://www.apache.org/ <http://www.apache.org/>`_
)에서 제공하는 바이너리와 함께 사용해선 안 된다.

**VC9를**
**이용하여**
**PHP 5.3용**
**CUBRID PHP**
**드라이버**
**빌드**

**환경 설정**

*   CUBRID: CUBRID를
    설치한다. 시스템에 환경 변수
    **%CUBRID%**
    가 정의되어 있는지 확인한다.



*   Visual Studio 2008: makefile을 잘 다룰 수 있는 사용자라면, Visual Studio 2008 대신에 무료인 Visual C++ Express Edition이나 Windows SDK v6.1에 포함된 VC++ 9 컴파일러를 사용할 수 있다. Windows에서 CUBRID PHP VC9 드라이버를 사용하려면 Visual C++ 2008 Redistributable Package가 설치되어 있어야 한다.



*   PHP 5.3 바이너리: VC9 x86 Non Thread Safe 또는 VC9 x86 Thread Safe를 사용할 수 있다. 시스템 환경 변수
    **%PHPRC%**
    가 제대로 정의되어 있어야 한다.



VC9 프로젝트 속성에서 [Linker] > [General]을 선택하면 [Additional Library Directories]에서
**$(PHPRC)**
가 사용되는 것을 볼 수 있다.

|image57_png|

*   PHP 5.3 소스코드: 바이너리 버전에 맞는 소스코드를 다운로드해야 한다. PHP 5.3 소스코드를 다운로드한 후 압축 해제하고, 시스템 환경 변수
    **%PHP5_SRC%**
    를 추가하여 PHP 5.3 소스코드의 경로를 값으로 설정한다.



VC9 프로젝트 속성에서 [C/C++] > [General]을 선택하면 [Additional Library Directories]에서
**$(PHP5_SRC)**
가 사용되는 것을 볼 수 있다.

|image58_png|

*   CUBRID PHP 드라이버 소스코드:
    `http://www.cubrid.org/?mid=downloads&item=php_driver <http://www.cubrid.org/?mid=downloads&item=php_driver>`_
    에서 CUBRID 버전에 맞는 CUBRID PHP 드라이버의 소스코드를 다운로드한다.



**참고**
PHP 5.3을 소스코드에서 빌드할 필요는 없지만 PHP 5.3 프로젝트를 설정해야 한다. PHP 5.3 프로젝트를 설정하지 않으면 VC9에서 config.w32.h 헤더 파일을 찾을 수 없다는 메시지가 출력된다. 설정 방법은 다음 주소를 참고한다.
`http://wiki.php.net/internals/windows/stepbystepbuild <http://wiki.php.net/internals/windows/stepbystepbuild>`_

**CUBRID PHP 드라이버 빌드**

*   다운로드한 CUBRID PHP 드라이버 소스코드의
    **\win**
    디렉터리에 있는
    **php_cubrid.vcproj**
    파일을 열고, 왼쪽의 [Solution Explorer] 창에서
    **php_cubrid**
    를 마우스 오른쪽 버튼으로 클릭하여 [Properties]를 선택한다.



|image59_png|

*   [Property Page] 대화 상자에서 [Configuration Manager]을 클릭한다. [Project context]의 [Configuration]에서 네 가지 설정(Release_TS, Release_NTS, Debug_TS and Debug_NTS) 중 원하는 값을 선택하고 [닫기]를 클릭한다.



|image60_png|

*   설정을 마친 후에는 [OK]를 클릭한 후, <F7> 키를 눌러 컴파일한다.



*   **php_cubrid.dll**
    파일을 빌드한 후에는 PHP가
    **php_cubrid.dll**
    파일을 PHP 확장으로 인식하도록 다음 작업을 수행한다.



*   PHP를 설치한 폴더에
    **cubrid**
    폴더를 생성하고 해당 폴더에
    **php_cubrid.dll**
    파일을 복사한다.
    **%PHPRC%\ext**
    디렉터리가 있다면 이 디렉터리에
    **php_cubrid.dll**
    파일을 복사해도 된다.



*   In
    **php.ini**
    파일의
    **extension_dir**
    변수의 값으로
    **php_cubrid.dll**
    파일의 경로를 입력하고,
    **extension**
    변수의 값으로
    **php_cubrid.dll**
    을 입력한다.



**VC6을**
**이용하여**
**PHP 5.2/5.3용**
**CUBRID PHP**
**드라이버**
**빌드**

**환경 설정**

*   CUBRID 2008 R3.1: CUBRID 2008 R3.1을 설치한다. 시스템에 환경 변수
    **%CUBRID%**
    가 정의되어 있는지 확인한다.



*   Visual C++ 6.0 SP6



*   Windows Server Feb. 2003 SDK: 모든 공식 릴리스와 스냅숏은 Visual C++ 6.0 SP6와 Windows Server Feb. 2003 SDK로 빌드되므로, 이 SDK를 사용하는 것을 권장한다.



이 SDK를 사용하지 않고 VC6의 기본 설정을 사용할 수도 있지만 드라이버를 빌드할 때 오류가 발생할 수 있으며, 오류를 직접 수정해야 한다.

*   PHP 5.2/5.3 바이너리: VC6 x86 Non Thread Safe 또는 VC6 x86 Thread Safe를 사용할 수 있다. 시스템 환경 변수
    **%PHPRC%**
    가 제대로 정의되어 있어야 한다.



VC6 프로젝트의 [Project Settings]을 열면 [Link] 탭의 [Additional library path]에서
**$(PHPRC)**
가 사용되는 것을 볼 수 있다.

|image61_png|

*   PHP 5.2/5.3 소스코드: 바이너리 버전에 맞는 소스코드를 다운로드해야 한다. PHP 소스코드를 다운로드한 후 압축 해제하고, 시스템 환경 변수
    **%PHP5_SRC%**
    를 추가하여 PHP 소스코드의 경로를 값으로 설정한다.



VC6 프로젝트의 [Project Settings]을 열면 [C/C++] 탭의 [Additional include directories]에서
**$(PHP5_SRC)**
가 사용되는 것을 볼 수 있다.

|image62_png|

*   CUBRID PHP 드라이버 소스코드:
    `http://www.cubrid.org/?mid=downloads&item=php_driver <http://www.cubrid.org/?mid=downloads&item=php_driver>`_
    에서 CUBRID 버전에 맞는 CUBRID PHP 드라이버의 소스코드를 다운로드한다.



**참고**
PHP 5.3 소스코드로 CUBRID PHP 드라이버를 빌드한다면, Windows에서 PHP 5.3를 설정해야 한다. PHP 5.3 프로젝트를 설정하지 않으면 VC9에서 config.w32.h 헤더 파일을 찾을 수 없다는 메시지가 출력된다. 설정 방법은 다음 주소를 참고한다.
`http://wiki.php.net/internals/windows/stepbystepbuild <http://wiki.php.net/internals/windows/stepbystepbuild>`_

**CUBRID PHP 드라이버 빌드**

*   다운로드한 CUBRID PHP 드라이버 소스코드에서
    **php_cubrid.dsp**
    파일을 열고, 메뉴에서 [Build] > [Set Active Configuration]를 선택한다. There are four configurations (Win32 Release_TS, Win32 Release, Win32 Debug_TS and Win32 Debug). Choose what you want, then close the [Set Active Project Configuration].



|image63_png|

*   네 가지 프로젝트 설정(Win32 Release_TS, Win32 Release, Win32 Debug_TS and Win32 Debug) 중에서 원하는 설정을 선택하고 [OK]를 클릭한다.



|image64_png|

*   <F7> 키를 눌러 소스코드를 컴파일한다.



*   **php_cubrid.dll**
    파일을 빌드한 후에는 PHP가
    **php_cubrid.dll**
    파일을 PHP 확장으로 인식하도록 다음 작업을 수행한다.



*   PHP를 설치한 폴더에
    **cubrid**
    폴더를 생성하고 해당 폴더에
    **php_cubrid.dll**
    파일을 복사한다.
    **%PHPRC%\ext**
    디렉터리가 있다면 이 디렉터리에
    **php_cubrid.dll**
    파일을 복사해도 된다.



*   In
    **php.ini**
    파일의
    **extension_dir**
    변수의 값으로
    **php_cubrid.dll**
    파일의 경로를 입력하고,
    **extension**
    변수의 값으로
    **php_cubrid.dll**
    을 입력한다.



**Windows x64 CUBRID PHP**
**드라이버**
**빌드**

**x64 PHP**

Windows x64 CUBRID PHP 드라이버는 제공되지 않는다. windows.php.net에도 Windows 32비트용 PHP만 있고 공식적인 Windows x64 PHP는 없지만, Windows x64 PHP가 필요하다면 직접 소스코드를 컴파일할 수 있다(다른 사용자가 빌드한 비공식 PHP는
`http://www.anindya.com/ <http://www.anindya.com/>`_
에서 다운로드할 수 있다). 여기에서는 x64 PHP를 빌드하는 방법은 자세히 설명하지 않는다.

Windows에서 PHP 빌드를 지원하는 컴파일러 목록은
`https://wiki.php.net/internals/windows/compiler <https://wiki.php.net/internals/windows/compiler>`_
에서 제공하며, x64 PHP를 빌드할 때에는 Visual C++ 8(2005)와 Visual C++ 9(2008 SP1 only)을 사용할 수 있다는 것을 확인할 수 있다. Visual C++ 2005 미만 버전에서 x64 PHP를 빌드하려면 Windows Server Feb. 2003 SDK를 사용해야 한다.

**x64 Apache**

http://www.apachelounge.com/에서는 VC9 x86 버전 Apache만 있고 공식 Windows x64 Apache는 없다. 대신에 64비트 Windows를 사용하는 Windows 서버에서는 IIS를 사용할 수 있다. 반드시 VC9 x64 버전 Apache를 사용하고 싶다면,
`http://www.anindya.com/ <http://www.anindya.com/>`_
에서 다운로드할 수 있다.

**환경 설정**

*   CUBRID x64 버전: CUBRID x64의 최신 버전을 설치한다.시스템에 환경 변수
    **%CUBRID%**
    가 정의되어 있는지 확인한다.



*   Visual Studio 2008: makefile을 잘 다룰 수 있는 사용자라면, Visual Studio 2008 대신에 무료인 Visual C++ Express Edition이나 Windows SDK v6.1에 포함된 VC++ 9 컴파일러를 사용할 수 있다. Windows에서 CUBRID PHP VC9 드라이버를 사용하려면 Visual C++ 2008 Redistributable Package가 설치되어 있어야 한다.



*   SDK 6.1: VC9을 사용한다면 Windows SDK for Windows Server 2008 and .NET Framework 3.5(또는 SDK 6.1)가 필요하다.



*   PHP 5.3 x64 바이너리: SDK 6.1을 이용하여 VC9 x64 PHP를 직접 빌드하거나,
    `http://www.anindya.com/ <http://www.anindya.com/>`_
    에서 VC9 x64 Non Thread Safe 또는 VC9 x64 Thread Safe 버전을 다운로드할 수 있다. 시스템 환경 변수
    **%PHPRC%**
    가 제대로 정의되어 있어야 한다.



*   PHP 5.3 소스코드: 바이너리 버전에 맞는 소스코드를 다운로드해야 한다. PHP 5.3 소스코드를 다운로드한 후 압축 해제하고, 시스템 환경 변수
    **%PHP5_SRC%**
    를 추가하여 PHP 5.3 소스코드의 경로를 값으로 설정한다. VC9 프로젝트 속성에서 [C/C++] > [General]을 선택하면 [Additional Library Directories]에서
    **$(PHP5_SRC)**
    가 사용되는 것을 볼 수 있다.



*   CUBRID PHP 드라이버 소스코드:
    `http://www.cubrid.org/?mid=downloads&item=php_driver <http://www.cubrid.org/?mid=downloads&item=php_driver>`_
    에서 CUBRID 버전에 맞는 CUBRID PHP 드라이버의 소스코드를 다운로드한다.



**참고**
PHP 5.3을 소스코드에서 빌드할 필요는 없지만 PHP 5.3 프로젝트를 설정해야 한다.PHP 5.3 프로젝트를 설정하지 않으면 VC9에서 config.w32.h 헤더 파일을 찾을 수 없다는 메시지가 출력된다. 설정 방법은 다음 주소를 참고한다.
`http://wiki.php.net/internals/windows/stepbystepbuild <http://wiki.php.net/internals/windows/stepbystepbuild>`_

**PHP 5.3 설정**

*   SDK 6.1를 설치한 후에는 Windows [시작] 메뉴에서 [Microsoft Windows SDK v6.1] > [CMD Shell]을 선택하여 명령 셸을 시작한다.



|image65_png|

*   **setenv /x64 /release**
    을 실행한다.



|image66_png|

*   PHP 5.3 소스코드 디렉터리로 이동한 후
    **buildconf**
    을 실행하여
    **configure.js**
    파일을 생성한다.



|image67_png|

또는 PHP 5.3 소스코드에서
**buildconf.bat**
파일을 실행해도 같은 동작을 수행한다.

|image68_png|

*   PHP 프로젝트를 설정하기 위해서
    **configure**
    를 실행한다.



|image69_png|

|image70_png|

**CUBRID PHP 드라이버 빌드**

*   다운로드한 CUBRID PHP 드라이버 소스코드의
    **\win**
    디렉터리에 있는
    **php_cubrid.vcproj**
    파일을 열고, 왼쪽의 [Solution Explorer] 창에서
    **php_cubrid**
    를 마우스 오른쪽 버튼으로 클릭하여 [Properties]를 선택한다.



*   [Property Page] 대화 상자에서 [Configuration Manager]을 클릭한다.



|image71_png|

*   [Configuration Manager] 대화 상자의 [Active solution configuration]에는 네 가지 설정(Release_TS, Release_NTS, Debug_TS and Debug_NTS)만 보인다. x64 CUBRID PHP 드라이버를 빌드하려면 새로운 설정을 생성해야 하므로
    **New**
    를 선택한다.



|image72_png|

*   [New Solution Configuration] 대화상자에서 새로운 설정의 이름(예: Release_TS_x64)을 입력하고 [Copy settings from]에서 사용할 PHP와 같은 설정을 선택한다. 여기에서는
    **Release_TS**
    를 선택했다. 선택한 후에 [OK]를 클릭한다.



|image73_png|

*   [Configuration Manager] 대화 상자에서 해당 프로젝트의 [Platform] 항목을 열어서
    **x64**
    가 있다면
    **x64**
    를 선택하고, 없으면
    **New**
    를 선택한다.



|image74_png|

**New**
를 선택하면 [New Project Platform] 대화 상자가 나타난다.
**x64**
를 선택하고 [OK]를 클릭한다.

|image75_png|

*   [php_cubrid Property Pages] 대화 상자에서 [C/C++] > [Preprocessor]를 선택하고, [Preprocessor Definitions]에서
    **_USE_32BIT_TIME_T**
    를 삭제한 후 [OK]를 클릭한다.



|image76_png|

*   <F7> 키를 눌러 소스코드를 컴파일하면 x64 PHP 드라이버 파일이 생성된다.



**참고**
PHP 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-php-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-php-driver>`_
에 접속한다.

**PHP**
**프로그래밍**

**일반 특징**

**데이터베이스**
**연결**

데이터베이스 응용에서 첫 단계는
`cubrid_connect <http://www.php.net/manual/en/function.cubrid-connect.php>`_
() 함수 또는
`cubrid_connect_with_url <http://www.php.net/manual/en/function.cubrid-connect-with-url.php>`_
() 함수를 사용하는 것으로 데이터베이스 연결을 제공한다.
`cubrid_connect <http://www.php.net/manual/en/function.cubrid-connect.php>`_
함수 또는
`cubrid_connect_with_url <http://www.php.net/manual/en/function.cubrid-connect-with-url.php>`_
() 함수가 성공적으로 수행되면, 데이터베이스를 사용할 수 있는 모든 함수를 사용할 수 있다. 응용을 완전히 끝내기 전에
`cubrid_disconnect <http://www.php.net/manual/en/function.cubrid-disconnect.php>`_
() 함수를 호출하는 것은 매우 중요하다.
`cubrid_disconnect <http://www.php.net/manual/en/function.cubrid-disconnect.php>`_
() 함수는 현재 발생한 트랜잭션을 끝마치고
`cubrid_connect <http://www.php.net/manual/en/function.cubrid-connect.php>`_
() 함수에 의해 생성된 연결 핸들과 모든 요청 핸들을 종료한다.

**주의**
스레드 기반 프로그램에서 데이터베이스 연결은 각 스레드마다 독립적으로 사용해야 한다.

**트랜잭션과**
**자동**
**커밋**

CUBRID PHP는 트랜잭션과 자동 커밋 모드를 지원한다. 자동 커밋 모드에서는 하나의 질의마다 하나의 트랜잭션이 이루어진다.
`cubrid_get_autocommit <http://www.php.net/manual/en/function.cubrid-get-autocommit.php>`_
() 함수를 사용하면 현재 연결의 자동 커밋 모드 여부를 확인할 수 있다.
`cubrid_set_autocommit <http://www.php.net/manual/en/function.cubrid-set-autocommit.php>`_
() 함수를 사용하면 현재 연결의 자동 커밋 모드 여부를 설정할 수 있으며, 진행 중이던 트랜잭션은 모드 설정과 상관없이 커밋된다.

응용 프로그램 시작 시 자동 커밋 모드의 기본값은 브로커 파라미터인
**CCI_DEFAULT_AUTOCOMMIT**
으로 설정한다. 브로커 파라미터 설정을 생략하면 기본값은
**ON**
이다. 다음 예와 같이
`cubrid_connect_with_url <http://www.php.net/manual/en/function.cubrid-connect-with-url.php>`_
() 함수를 사용해도 자동 커밋 모드 여부를 설정할 수 있다.

$con = cubrid_connect_with_url("cci:CUBRID:localhost:33000:demodb:dba::?autocommit=true");

`cubrid_set_autocommit <http://www.php.net/manual/en/function.cubrid-set-autocommit.php>`_
() 함수에서 자동 커밋 모드를 OFF로 설정하면 커밋 또는 롤백을 명시하여 트랜잭션을 처리할 수 있다. 트랜잭션을 커밋하려면
`cubrid_commit <http://www.php.net/manual/en/function.cubrid-commit.php>`_
() 함수를 사용하고 트랜잭션을 롤백하려면
`cubrid_rollback <http://www.php.net/manual/en/function.cubrid-rollback.php>`_
() 함수를 사용한다.
`cubrid_disconnect <http://www.php.net/manual/en/function.cubrid-disconnect.php>`_
() 함수는 트랜잭션을 종료하고 커밋되지 않은 작업을 롤백한다.

**질의**
**처리**

**질의 실행**

다음은 질의 실행을 위한 기본 단계이다.

*   연결 핸들 생성



*   SQL 질의 요청에 대한 요청 핸들 생성



*   결과 가져오기



*   요청 핸들 종료



$con = cubrid_connect("192.168.0.10", 33000, "demodb");

if($con) {

????$req = cubrid_execute($con, "select * from code");

????if($req) {

????????while ($row = cubrid_fetch($req)) {

????????????echo $row["s_name"];

????????????echo $row["f_name"];

????????}

????????cubrid_close_request($req);

????}

????cubrid_disconnect($con);

}

**질의 결과의 열 타입과 이름**

`cubrid_column_types <http://www.php.net/manual/en/function.cubrid-column-types.php>`_
() 함수를 사용하여 열 타입이 들어있는 배열을 얻을 수 있고,
`cubrid_column_types <http://www.php.net/manual/en/function.cubrid-column-types.php>`_
() 함수를 사용하여?열의 이름이 들어있는 배열을 얻을 수 있다.

$req = cubrid_execute($con, "select host_year, host_city from olympic");

if($req) {

????$col_types = cubrid_column_types($req);

????$col_names = cubrid_column_names($req);

?

????while (list($key, $col_type) = each($col_types)) {

????????echo $col_type;

????}

????while (list($key, $col_name) = each($col_names))

????????echo $col_name;

????}

????cubrid_close_request($req);

}

**커서 조정**

질의 결과의 위치를 설정할 수 있다.
`cubrid_move_cursor <http://www.php.net/manual/en/function.cubrid-move-cursor.php>`_
() 함수를 사용하여?커서를 세 가지 포인트(질의 결과의 처음, 현재 커서 위치, 질의 결과의 끝) 중 한 포인트로부터 일정한 위치로 이동할 수 있다.

$req = cubrid_execute($con, "select host_year, host_city from olympic order by host_year");

if($req) {

????cubrid_move_cursor($req, 20, CUBRID_CURSOR_CURRENT)

????while ($row = cubrid_fetch($req, CUBRID_ASSOC)) {

????????echo $row["host_year"].” “;

????????echo $row["host_city"].”\n”;

????}

}

**결과 배열 타입**

`cubrid_fetch <http://www.php.net/manual/en/function.cubrid-fetch.php>`_
() 함수의 결과에는 세가지 종류의 배열 타입 중 하나가 사용된다.
`cubrid_fetch <http://www.php.net/manual/en/function.cubrid-fetch.php>`_
() 함수가 호출될 때 배열의 타입을 결정할 수 있다. 그 중 하나인 연관배열은 문자열 색인을 사용한다. 두 번째로 수치배열은 숫자 순서 색인을 사용한다. 마지막 배열은 연관배열과 수치배열을 둘 다 포함한다.

*   수치배열



while (list($id, $name) = cubrid_fetch($req, CUBRID_NUM)) {

????echo $id;

????echo $name;

}

*   연관배열



while ($row = cubrid_fetch($req, CUBRID_ASSOC)) {

????echo $row["id"];

????echo $row["name"];

}

**카탈로그**
**연산**

클래스, 가상 클래스, 속성, 메서드, 트리거, 제약 조건 등 데이터베이스의 스키마 정보는
`cubrid_schema <http://www.php.net/manual/en/function.cubrid-schema.php>`_
() 함수를 호출하여 얻을 수 있다.
`cubrid_schema <http://www.php.net/manual/en/function.cubrid-schema.php>`_
() 함수의 리턴 값은 2차원 배열이다.

$pk = cubrid_schema($con, CUBRID_SCH_PRIMARY_KEY, "game");

if ($pk) {

????print_r($pk);

}

?

$fk = cubrid_schema($con, CUBRID_SCH_IMPORTED_KEYS, "game");

if ($fk) {

????print_r($fk);

}

**에러**
**처리**

에러가 발생하면 대부분의 PHP 인터페이스 함수는 에러 메시지를 출력하고 false나 -1을 반환한다.
`cubrid_error_msg <http://www.php.net/manual/en/function.cubrid-error-msg.php>`_
(),
`cubrid_error_code <http://www.php.net/manual/en/function.cubrid-error-code.php>`_
() 그리고
`cubrid_error_code_facility <http://www.php.net/manual/en/function.cubrid-error-code-facility.php>`_
() 함수를 사용하면?각각 에러 메시지, 에러 코드, 에러 기능 코드를 확인할 수 있다.

`cubrid_error_code_facility <http://www.php.net/manual/en/function.cubrid-error-code-facility.php>`_
() 함수의 결과 값은
**CUBRID_FACILITY_DBMS**
(DBMS 에러),
**CUBRID_FACILITY_CAS**
(CAS 서버 에러),
**CUBRID_FACILITY_CCI**
(CCI 에러),
**CUBRID_FACILITY_CLIENT**
(PHP 모듈 에러) 중 하나이다.

**CUBRID 특징**

**OID**
**사용**

`cubrid_execute <http://www.php.net/manual/en/function.cubrid-execute.php>`_
() 함수에서 CUBRID_INCLUDE_OID 옵션을 업데이트할 수 있는 질의를 함께 사용하면
`cubrid_current_oid <http://www.php.net/manual/en/function.cubrid-current-oid.php>`_
함수를 통해 업데이트된 현재 f 레코드의 OID 값을 가져올 수 있다.

$req = cubrid_execute($con, "select * from person where id = 1", CUBRID_INCLUDE_OID);

if ($req) {

????while ($row = cubrid_fetch($req)) {

????????echo cubrid_current_oid($req);

????????echo $row["id"];

????????echo $row["name"];

????}

????cubrid_close_request($req);

}

OID를 사용하여 인스턴스의 모든 속성, 지정한 속성 또는 한 속성의 값을 얻을 수 있다.

만약
`cubrid_get <http://www.php.net/manual/en/function.cubrid-get.php>`_
() 함수에 속성을 명시하지 않으면 모든 속성의 값을 반환한다(a). 만약 배열 데이터 타입으로 속성을 명시하면 지정한 속성 값이 들어있는 배열은 연관배열로 반환된다(b). 만약 문자열 타입으로 한 속성을 명시하면 속성의 값이 반환된다(c).

$attrarray = cubrid_get ($con, $oid); // (a)

$attrarray = cubrid_get ($con, $oid, array("id", "name")); // (b)

$attrarray = cubrid_get ($con, $oid, "id"); // (c)

OID를 사용하여 인스턴스의 속성 값을 갱신할 수도 있다. 하나의 속성의 값을 갱신하려면 속성 이름을 문자열 타입으로 명시하고 값을 명시한다(a). 다중 속성의 값을 설정하려면 속성 명과 값을 연관배열로 명시해야 한다(b).

$cubrid_put ($con, $oid, "id", 1); // (a)
$cubrid_put ($con, $oid, array("id"=>1, "name"=>"Tomas")); // (b)

**컬렉션**
**사용**

컬렉션 데이터 타입은 PHP 배열 데이터 타입을 통해 사용할 수 있고 배열 데이터 타입을 지원하는 PHP 함수를 사용할 수 있다. 다음은
`cubrid_fetch <http://www.php.net/manual/en/function.cubrid-fetch.php>`_
() 함수를 사용하여 질의 결과를 가져오는 예제이다.

$row = cubrid_fetch ($req);

$col = $row["customer"];

while (list ($key, $cust) = each ($col)) {

???echo $cust;

}

컬렉션 속성의 값도 얻을 수 있다. 다음은
`cubrid_col_get <http://www.php.net/manual/en/function.cubrid-col-get.php>`_
() 함수를 사용하여 컬렉션 속성 값을 가져오는 예제이다.

$tels = cubrid_col_get ($con, $oid, "tels");

while (list ($key, $tel) = each ($tels)) {

???echo $tel."\n";

}

cubrid_set_add() 함수와 cubrid_set_drop() 함수를 사용하면 컬렉션 타입의 값을 직접적으로 갱신할 수 있다.

$tels = cubrid_col_get ($con, $oid, "tels");

while (list ($key, $tel) = each ($tels)) {

???$res = cubrid_set_drop ($con, $oid, "tel", $tel);

}

cubrid_commit ($con);

**참고**
칼럼에서 정의한 크기보다 큰 문자열을
**INSERT**
/
**UPDATE**
하면 문자열이 잘려서 입력된다.

**참고**
PHP 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-php-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-php-driver>`_
에 접속한다.

**PHP API**

PHP API에 대한 자세한 내용은 PHP CUBRID Functions 문서(
`http://www.php.net/manual/en/ref.cubrid.php <http://www.php.net/manual/en/ref.cubrid.php>`_
)를 참고한다.

*   `cubrid_bind <http://www.php.net/manual/en/function.cubrid-bind.php>`_



*   `cubrid_close_prepare <http://www.php.net/manual/en/function.cubrid-close-prepare.php>`_



*   `cubrid_close_request <http://www.php.net/manual/en/function.cubrid-close-request.php>`_



*   `cubrid_col_get <http://www.php.net/manual/en/function.cubrid-col-get.php>`_



*   `cubrid_col_size <http://www.php.net/manual/en/function.cubrid-col-size.php>`_



*   `cubrid_column_names <http://www.php.net/manual/en/function.cubrid-column-names.php>`_



*   `cubrid_column_types <http://www.php.net/manual/en/function.cubrid-column-types.php>`_



*   `cubrid_commit <http://www.php.net/manual/en/function.cubrid-commit.php>`_



*   `cubrid_connect_with_url <http://www.php.net/manual/en/function.cubrid-connect-with-url.php>`_



*   `cubrid_connect <http://www.php.net/manual/en/function.cubrid-connect.php>`_



*   `cubrid_current_oid <http://www.php.net/manual/en/function.cubrid-current-oid.php>`_



*   `cubrid_disconnect <http://www.php.net/manual/en/function.cubrid-disconnect.php>`_



*   `cubrid_drop <http://www.php.net/manual/en/function.cubrid-drop.php>`_



*   `cubrid_error_code_facility <http://www.php.net/manual/en/function.cubrid-error-code-facility.php>`_



*   `cubrid_error_code <http://www.php.net/manual/en/function.cubrid-error-code.php>`_



*   `cubrid_error_msg <http://www.php.net/manual/en/function.cubrid-error-msg.php>`_



*   `cubrid_execute <http://www.php.net/manual/en/function.cubrid-execute.php>`_



*   `cubrid_fetch <http://www.php.net/manual/en/function.cubrid-fetch.php>`_



*   `cubrid_free_result <http://www.php.net/manual/en/function.cubrid-free-result.php>`_



*   `cubrid_get_autocommit <http://www.php.net/manual/en/function.cubrid-get-autocommit.php>`_



*   `cubrid_get_charset <http://www.php.net/manual/en/function.cubrid-get-charset.php>`_



*   `cubrid_get_class_name <http://www.php.net/manual/en/function.cubrid-get-class-name.php>`_



*   `cubrid_get_client_info <http://www.php.net/manual/en/function.cubrid-get-client-info.php>`_



*   `cubrid_get_db_parameter <http://www.php.net/manual/en/function.cubrid-get-db-parameter.php>`_



*   `cubrid_get_query_timeout <http://www.php.net/manual/en/function.cubrid-get-query-timeout.php>`_



*   `cubrid_get_server_info <http://www.php.net/manual/en/function.cubrid-get-server-info.php>`_



*   `cubrid_get <http://www.php.net/manual/en/function.cubrid-get.php>`_



*   `cubrid_insert_id <http://www.php.net/manual/en/function.cubrid-insert-id.php>`_



*   `cubrid_is_instance <http://www.php.net/manual/en/function.cubrid-is-instance.php>`_



*   `cubrid_lob_close <http://www.php.net/manual/en/function.cubrid-lob-close.php>`_



*   `cubrid_lob_export <http://www.php.net/manual/en/function.cubrid-lob-export.php>`_



*   `cubrid_lob_get <http://www.php.net/manual/en/function.cubrid-lob-get.php>`_



*   `cubrid_lob_send <http://www.php.net/manual/en/function.cubrid-lob-send.php>`_



*   `cubrid_lob_size <http://www.php.net/manual/en/function.cubrid-lob-size.php>`_



*   `cubrid_lock_read <http://www.php.net/manual/en/function.cubrid-lock-read.php>`_



*   `cubrid_lock_write <http://www.php.net/manual/en/function.cubrid-lock-write.php>`_



*   `cubrid_move_cursor <http://www.php.net/manual/en/function.cubrid-move-cursor.php>`_



*   `cubrid_next_result <http://www.php.net/manual/en/function.cubrid-next-result.php>`_



*   `cubrid_num_cols <http://www.php.net/manual/en/function.cubrid-num-cols.php>`_



*   `cubrid_num_rows <http://www.php.net/manual/en/function.cubrid-num-rows.php>`_



*   `cubrid_pconnect_with_url <http://www.php.net/manual/en/function.cubrid-pconnect-with-url.php>`_



*   `cubrid_pconnect <http://www.php.net/manual/en/function.cubrid-pconnect.php>`_



*   `cubrid_prepare <http://www.php.net/manual/en/function.cubrid-prepare.php>`_



*   `cubrid_put <http://www.php.net/manual/en/function.cubrid-put.php>`_



*   `cubrid_rollback <http://www.php.net/manual/en/function.cubrid-rollback.php>`_



*   `cubrid_schema <http://www.php.net/manual/en/function.cubrid-schema.php>`_



*   `cubrid_seq_drop <http://www.php.net/manual/en/function.cubrid-seq-drop.php>`_



*   `cubrid_seq_insert <http://www.php.net/manual/en/function.cubrid-seq-insert.php>`_



*   `cubrid_seq_put <http://www.php.net/manual/en/function.cubrid-seq-put.php>`_



*   `cubrid_set_add <http://www.php.net/manual/en/function.cubrid-set-add.php>`_



*   `cubrid_set_autocommit <http://www.php.net/manual/en/function.cubrid-set-autocommit.php>`_



*   `cubrid_set_db_parameter <http://www.php.net/manual/en/function.cubrid-set-db-parameter.php>`_



*   `cubrid_set_drop <http://www.php.net/manual/en/function.cubrid-set-drop.php>`_



*   `cubrid_set_query_timeout <http://www.php.net/manual/en/function.cubrid-set-query-timeout.php>`_



*   `cubrid_version <http://www.php.net/manual/en/function.cubrid-version.php>`_



**참고**
PHP 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-php-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-php-driver>`_
에 접속한다.

**PDO**
**드라이버**

**개요**

공식 CUBRID PDO(PHP Data Objects) 드라이버는 PECL 패키지로 제공되며, PDO에서 CUBRID 데이터베이스에 접근할 수 있도록 PDO 인터페이스를 제공한다. PDO는 PHP 5.1과 함께 배포되며, PHP 5.0에서는 PECL 확장으로 사용할 수 있다. PDO를 사용하려면 PHP 5 코어의 OO 기능이 필요하므로, PHP 5.0 미만 버전에서는 사용할 수 없다.

PDO는 어떤 데이터베이스를 사용하든 같은 함수를 사용할 수 있게 하는 데이터 액세스 추상화 레이어(data-access abstraction layer)를 제공하며, 데이터베이스 추상화를 제공하는 것은 아니다. PDO를 데이터베이스 인터페이스 레이어로 사용하면 PHP 데이터베이스 드라이버를 바로 사용하는 경우에 비해 다음과 같은 이점이 있다.

*   작성한 PHP 코드를 다양한 데이터베이스와 함께 사용할 수 있다.



*   SQL 파라미터와 바인딩을 지원한다.



*   SQL을 더 안전하게 사용할 수 있다(구문 검사, 이스케이프, SQL 인젝션 방지 등).



*   프로그래밍 모델이 간결해진다.



따라서 CUBRID PDO 드라이버를 사용하면, 데이터베이스 인터페이스 레이어로 PDO를 사용하는 모든 응용 프로그램은 CUBRID와 함께 사용할 수 있다.

CUBRID PDO 드라이버는 CCI API를 기반으로 작성되었으므로, CCI API 및 CCI에 적용되는
**CCI_DEFAULT_AUTOCOMMIT**
과 같은 설정 파라미터에 영향을 받는다.

별도로 PDO?드라이버를 다운로드하거나 PDO?드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver>`_
에 접속한다.

**PDO**
**설치**
**및**
**설정**

**Linux**

**기본**
**환경**

*   운영체제: Linux: 32 비트 또는 64비트



*   웹 서버: Apache



*   PHP: 5.2 또는 5.3(
    `http://php.net/downloads.php <http://php.net/downloads.php>`_
    )



**PECL을**
**이용한**
**설치**

**PECL**
이 설치되어 있다면,
**PECL**
이 소스코드 다운로드 및 컴파일을 수행하므로 다음과 같이 간단하게 CUBRID PDO 드라이버를 설치할 수 있다.
**PECL**
이 설치되어있지 않다면
`http://www.cubrid.org/wiki_apis/entry/installing-cubrid-php-driver-using-pecl <http://www.cubrid.org/wiki_apis/entry/installing-cubrid-php-driver-using-pecl>`_
을 참고하여 PECL을 설치한다.

*   다음과 같은 명령어를 입력하여 CUBRID PDO 드라이버 최신 버전을 설치한다.



sudo pecl install pdo_cubrid

하위 버전의 드라이버가 필요하면 다음과 같이 설치할 버전을 지정할 수 있다.

sudo pecl install pdo_cubrid-8.3.1.0003

설치가 진행되는 중에
**CUBRID base install dir autodetect :**
라는 프롬프트가 표시된다. 설치를 원활하게 진행하기 위해서 CUBRID를 설치한 디렉터리의 전체 경로를 입력한다. 예를 들어 CUBRID가
**/home/cubridtest/CUBRID**
디렉터리에 설치되었다면,
**/home/cubridtest/CUBRID**
를 입력한다.

*   설정 파일을 수정한다.



*   CentOS 6.0 이상 버전이나 Fedora 15 이상 버전을 사용한다면
    **pdo_cubrid.ini**
    파일을 생성하고 내용에
    **extension=pdo_cubrid.so**
    를 입력하여
    **/etc/php.d**
    디렉터리에 저장한다.



*   다른 운영체제를 사용한다면
    **php.ini**
    파일 끝에 다음 두 줄의 내용을 추가한다.
    **php.ini**
    파일의 기본 위치는
    **/etc/php5/apache2**
    또는
    **/etc**
    이다.



[CUBRID]

extension=pdo_cubrid.so

*   변경된 내용을 반영하려면 웹 서버를 재시작한다.



**Windows**

**기본**
**환경**

*   운영체제: Windows 32 비트 또는 64비트



*   웹 서버: Apache 또는 IIS



*   PHP: 5.2 또는 5.3(
    `http://windows.php.net/download/ <http://windows.php.net/download/>`_
    )



**빌드된**
**드라이버**
**다운로드**
**및**
**설치**

운영체제와 PHP 버전에 맞는 Windows용 CUBRID PHP/PDO 드라이버를
`http://www.cubrid.org/?mid=downloads&item=php_driver&os=windows&ostype=any&php=any&driver_type=pdo <http://www.cubrid.org/?mid=downloads&item=php_driver&os=windows&ostype=any&php=any&driver_type=pdo>`_
에서 다운로드한다.

PDO 드라이버를 다운로드하면
**php_cubrid.dll**
파일을 볼 수 있으며, PDO 드라이버를 다운로드하면
**php_pdo_cubrid.dll**
파일을 볼 수 있다. 드라이버를 설치하는 방법은 다음과 같다.

*   드라이버 파일을 기본 PHP 확장 디렉터리(
    **C:\Program Files\PHP\ext**
    )에 복사한다.



*   시스템 환경 변수를 설정한다. 시스템 환경 변수
    **PHPRC**
    의 값으로
    **C:\Program Files\PHP**
    가 설정되고,
    **Path**
    에
    **%PHPRC%**
    와
    **%PHPRC\ext**
    가 추가되었는지 확인한다.



*   **php.ini**
    (
    **C:\Program Files\PHP\php.ini**
    ) 파일을 열어 끝에 다음 두 줄을 추가한다.



[PHP_CUBRID]

extension=php_cubrid.dll

PDO 드라이버의 경우에는 다음 내용을 추가한다.

[PHP_PDO_CUBRID]

extension = php_pdo_cubrid.dll

*   웹 서버를 재시작한다.



**참고**
PDO 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver>`_
에 접속한다.

**PDO**
**프로그래밍**

**데이터**
**원본**
**이름(DSN)**

PDO_CUBRID 데이터 원본 이름(DSN)은 다음과 같은 요소로 구성된다.

+--------+----------+
| 요소     | 설명       |
|        |          |
+--------+----------+
| DSN    | DSN      |
| 접두어    | 접두어      |
|        | (prefix) |
|        | 는        |
|        | cubrid   |
|        | 이다       |
|        | .        |
|        |          |
+--------+----------+
| host   | 데이터베이스   |
|        | 서버가      |
|        | 위치한      |
|        | 호스트의     |
|        | 이름을      |
|        | 입력한다.    |
|        |          |
+--------+----------+
| port   | 데이터베이스   |
|        | 서버의      |
|        | 수신       |
|        | 대기       |
|        | 포트       |
|        | 번호를      |
|        | 입력한다.    |
|        |          |
+--------+----------+
| dbname | 데이터베이스의  |
|        | 이름을      |
|        | 입력한다.    |
|        |          |
+--------+----------+

**예제**

"cubrid:host=127.0.0.1;port=33000;dbname=demodb"

**미리**
**정의된**
**상수**

CUBRID PDO 드라이버에 의해 정의되는 상수(predefined constants)는 CUBRID PDO 드라이버가 PHP와 함께 컴파일되거나 런타임에 동적으로 로드되는 경우에만 사용할 수 있다. 이처럼 특정 드라이버에 의해 정의된 상수를 다른 드라이버와 함께 사용하면 예상과 다르게 동작할 수도 있다.

코드가 여러 개의 드라이버와 함께 실행될 수 있다면,
**PDO_ATTR_DRIVER_NAME**
속성 값을 얻어 드라이버를 확인하기 위해
`PDO::getAttribute() <http://docs.php.net/manual/en/pdo.getattribute.php>`_
함수를 사용할 수 있다.

다음 상수는
`PDO::cubrid_schema <http://www.php.net/manual/en/pdo.cubrid-schema.php>`_
() 함수를 이용하여 스키마 정보를 얻을 때 사용할 수 있다.

+------------------------------------+---------+--------+
| 상수                                 | 타입      | 설명     |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_TABLE              | integer | CUBRID |
|                                    |         | 테이블의   |
|                                    |         | 이름과    |
|                                    |         | 타입을    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_VIEW               | integer | CUBRID |
|                                    |         | 뷰의     |
|                                    |         | 이름과    |
|                                    |         | 타입을    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_QUERY_SPEC         | integer | 뷰의     |
|                                    |         | 쿼리     |
|                                    |         | 정의를    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_ATTRIBUTE          | integer | 테이블    |
|                                    |         | 칼럼의    |
|                                    |         | 속성을    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_TABLE_ATTRIBUTE    | integer | 테이블의   |
|                                    |         | 속성을    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_TABLE_METHOD       | integer | 인스턴스   |
|                                    |         | 메서드를   |
|                                    |         | 얻는다.   |
|                                    |         | 인스턴스   |
|                                    |         | 메서드는   |
|                                    |         | 클래스    |
|                                    |         | 인스턴스가  |
|                                    |         | 호출하는   |
|                                    |         | 메서드이다. |
|                                    |         | 대부분의   |
|                                    |         | 작업은    |
|                                    |         | 인스턴스에서 |
|                                    |         | 실행되기   |
|                                    |         | 때문에    |
|                                    |         | 인스턴스   |
|                                    |         | 메서드는   |
|                                    |         | 클래스    |
|                                    |         | 메서드보다  |
|                                    |         | 자주     |
|                                    |         | 사용된다.  |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_METHOD_FILE        | integer | 테이블의   |
|                                    |         | 메서드가   |
|                                    |         | 정의된    |
|                                    |         | 파일의    |
|                                    |         | 정보를    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_SUPER_TABLE        | integer | 현재     |
|                                    |         | 테이블에   |
|                                    |         | 속성을    |
|                                    |         | 상속한    |
|                                    |         | 테이블의   |
|                                    |         | 이름과    |
|                                    |         | 타입을    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_SUB_TABLE          | integer | 현재     |
|                                    |         | 테이블로부터 |
|                                    |         | 속성을    |
|                                    |         | 상속받은   |
|                                    |         | 테이블의   |
|                                    |         | 이름과    |
|                                    |         | 타입을    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_CONSTRAINT         | integer | 테이블의   |
|                                    |         | 제약     |
|                                    |         | 조건을    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_TRIGGER            | integer | 테이블의   |
|                                    |         | 트리거를   |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_TABLE_PRIVILEGE    | integer | 테이블의   |
|                                    |         | 권한     |
|                                    |         | 정보를    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_COL_PRIVILEGE      | integer | 칼럼의    |
|                                    |         | 권한     |
|                                    |         | 정보를    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_DIRECT_SUPER_TABLE | integer | 현재     |
|                                    |         | 테이블의   |
|                                    |         | 바로     |
|                                    |         | 상위     |
|                                    |         | 테이블을   |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_DIRECT_PRIMARY_KEY | integer | 테이블의   |
|                                    |         | 기본키를   |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_IMPORTED_KEYS      | integer | 테이블의   |
|                                    |         | 외래키가   |
|                                    |         | 참조하는   |
|                                    |         | 기본키를   |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_EXPORTED_KEYS      | integer | 테이블의   |
|                                    |         | 기본키를   |
|                                    |         | 참조하는   |
|                                    |         | 외래키를   |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+
| PDO::CUBRID_SCH_CROSS_REFERENCE    | integer | 두      |
|                                    |         | 테이블    |
|                                    |         | 간의     |
|                                    |         | 상호     |
|                                    |         | 참조     |
|                                    |         | 관계를    |
|                                    |         | 얻는다.   |
|                                    |         |        |
+------------------------------------+---------+--------+

**참고**
PDO 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver>`_
에 접속한다.

**PDO**
**예제**
**프로그램**

**CUBRID PDO**
**드라이버**
**확인**

사용 가능한 PDO 드라이버를 확인하려면 다음과 같이
`PDO::getAvailableDrivers <http://docs.php.net/manual/en/pdo.getavailabledrivers.php>`_
() 함수를 사용한다.

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

위 스크립트는 다음과 같이 설치된 PDO 드라이버를 출력한다.

PDO Drivers available:

- Driver: mysql

- Driver: pgsql

- Driver: sqlite

- Driver: sqlite2

- Driver: cubrid

**CUBRID**
**연결**

데이터 원본 이름(DSN)을 사용하여 데이터베이스에 연결한다. 데이터 원본 이름에 대한 자세한 설명은
`데이터 원본 이름(DSN) <#api_api_pdo_programming_htm_dsn>`_
을 참고한다.

다음은
*demodb*
라는 CUBRID 데이터베이스에 PDO 연결을 수행하는 간단한 PHP 스크립트이다. PDO에서는 try-catch로 오류를 처리하며, 연결을 해제할 때에는 연결 객체에
**NULL**
을 할당한다는 것을 알 수 있다.

<?php

$database ="demodb";

$host ="localhost";

$port ="30000";//use default value

$username ="dba";

$password ="";

?

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

연결에 성공하면 다음과 같은 스크립트가 출력된다.

PDO connect string: cubrid:dbname=demodb;host=localhost;port=30000

PDO connection created ok!

**SELECT**
**실행**

PDO에서 SQL 질의를 수행하려면 질의나 응용 프로그램의 성격에 따라 다음 중 하나의 방법을 사용할 수 있다.

*   `query <http://docs.php.net/manual/en/pdo.exec.php>`_
    () 함수 사용



*   prepared statements(
    `prepare <http://docs.php.net/manual/en/pdo.prepare.php>`_
    ()/
    `execute <http://docs.php.net/manual/en/pdostatement.execute.php>`_
    ()) 함수 사용



*   `exec <http://docs.php.net/manual/en/pdo.exec.php>`_
    () 함수 사용



다음 예제에서는 가장 간단한
`query <http://docs.php.net/manual/en/pdo.exec.php>`_
() 함수를 사용한다. 리턴 값은 PDOStatement 객체인 resultset에서 $rs["column_name"]와 같이 칼럼 이름을 이용하여 얻을 수 있다.

`query <http://docs.php.net/manual/en/pdo.exec.php>`_
() 함수를 사용할 때에는 질의 코드가 제대로 이스케이프되었는지 확인해야 한다. 이스케이프에 대한 내용은
`PDO::quote <http://www.php.net/manual/en/pdo.quote.php>`_
()를 참고한다.

<?php

include("_db_config.php");

include("_db_connect.php");

?

$sql ="SELECT * FROM code";

echo"Executing SQL: <b>".$sql.'</b>

';

echo'

';

?

try{

foreach($db->query($sql)as $row){

echo $row['s_name'].' - '. $row['f_name'].'

';

}

}catch(PDOException $e){

echo $e->getMessage();

}

?

$db = null;//disconnect

?>

위 스크립트의 결과는 다음과 같이 출력된다.

Executing SQL: SELECT * FROM code

?

X - Mixed

W - Woman

M - Man

B - Bronze

S - Silver

G - Goldie

**UPDATE**
**실행**

다음은 prepared statement와 파라미터를 사용하여 UPDATE를 실행하는 예제이다. prepared statement 대신
`exec <http://docs.php.net/manual/en/pdo.exec.php>`_
() 함수를 사용할 수도 있다.

<?php

include("_db_config.php");

include("_db_connect.php");

?

$s_name ='X';

$f_name ='test';

$sql ="UPDATE code SET f_name=:f_name WHERE s_name=:s_name";

?

echo"Executing SQL: <b>".$sql.'</b>

';

echo'

';

?

echo":f_name: <b>".$f_name.'</b>

';

echo'

';

echo":s_name: <b>".$s_name.'</b>

';

echo'

';

?

$qe = $db->prepare($sql);

$qe->execute(array(':s_name'=>$s_name,':f_name'=>$f_name));

?

$sql ="SELECT * FROM code";

echo"Executing SQL: <b>".$sql.'</b>

';

echo'

';

?

try{

foreach($db->query($sql)as $row){

echo $row['s_name'].' - '. $row['f_name'].'

';

}

}catch(PDOException $e){

echo $e->getMessage();

}

?

$db = null;//disconnect

?>

위 스크립트의 실행 결과는 다음과 같이 출력된다.

Executing SQL: UPDATE code SET f_name=:f_name WHERE s_name=:s_name

?

:f_name: test

?

:s_name: X

?

Executing SQL: SELECT * FROM code

?

X - test

W - Woman

M - Man

B - Bronze

S - Silver

G ? Goldie

**prepare와**
**bind**

prepared statement는 PDO가 제공하는 유용한 기능 중 하나로, 사용하면 다음과 같은 이점이 있다.

*   SQL prepared statement는 다양한 파라미터와 함께 여러 번 실행되어도 한 번만 파싱하면 된다. 따라서 여러 번 실행되는 SQL문에 prepared statement를 사용하면 CUBRID 응용 프로그램의 성능을 높일 수 있다.



*   수동으로 파라미터를 이스케이프할 필요가 없으므로 SQL 인젝션 공격을 방지할 수 있다(그러나 SQL 질의의 다른 부분이 이스케이프되지 않은 입력으로 구성된다면 SQL 인젝션을 완전히 막을 수는 없다).



다음은 prepared statement를 이용하여 데이터를 조회하는 예이다.

<?php

include("_db_config.php");

include("_db_connect.php");

?

$sql ="SELECT * FROM code WHERE s_name NOT LIKE :s_name";

echo"Executing SQL: <b>".$sql.'</b>

';

?

$s_name ='xyz';

echo":s_name: <b>".$s_name.'</b>

';

?

echo'

';

?

try{

$stmt = $db->prepare($sql);

?

$stmt->bindParam(':s_name', $s_name, PDO::PARAM_STR);

$stmt->execute();

?

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

?

$sql ="SELECT * FROM code WHERE s_name NOT LIKE :s_name";

echo"Executing SQL: <b>".$sql.'</b>

';

?

$s_name ='X';

echo":s_name: <b>".$s_name.'</b>

';

?

echo'

';

?

try{

$stmt = $db->prepare($sql);

?

$stmt->bindParam(':s_name', $s_name, PDO::PARAM_STR);

$stmt->execute();

?

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

?

$db = null;//disconnect

?>

위 스크립트의 결과는 다음과 같이 출력된다.

Executing SQL: SELECT * FROM code WHERE s_name NOT LIKE :s_name

:s_name: xyz

?

X - Mixed

W - Woman

M - Man

B - Bronze

S - Silver

G - Goldie

?

Executing SQL: SELECT * FROM code WHERE s_name NOT LIKE :s_name

:s_name: X

?

W - Woman

M - Man

B - Bronze

S - Silver

G - Goldie

**PDO::getAttribute()**
**사용**

`PDO::getAttribute <http://docs.php.net/manual/en/pdo.getattribute.php>`_
() 함수는 다음과 같은 데이터베이스 연결 속성을 조회할 때 유용하다.

*   드라이버 이름



*   데이터베이스 버전



*   자동 커밋 모드 여부



*   오류 모드



속성을 쓸 수 있다면
`PDO::setAttribute <http://docs.php.net/manual/en/pdo.setattribute.php>`_
() 함수를 사용하여 속성을 설정할 수 있다.

다음은
`PDO::getAttribute <http://docs.php.net/manual/en/pdo.getattribute.php>`_
() 함수를 사용하여 클라이언트와 서버의 현재 버전을 조회하는 PHP PDO 스크립트이다.

<?php

include("_db_config.php");

include("_db_connect.php");

?

echo"Driver name: <b>".$db->getAttribute(PDO::ATTR_DRIVER_NAME)."</b>";

echo"

";

echo"Client version: <b>".$db->getAttribute(PDO::ATTR_CLIENT_VERSION)."</b>";

echo"

";

echo"Server version: <b>".$db->getAttribute(PDO::ATTR_SERVER_VERSION)."</b>";

echo"

";

?

$db = null;//disconnect

?>

위 스크립트의 결과는 다음과 같이 출력된다.

Driver name: cubrid

Client version: 8.3.0

Server version: 8.3.0.0337

**CUBRID PDO**
**확장**

CUBRID PDO 확장은 데이터베이스 스키마와 메타데이터 정보를 조회하는 데 사용할 수 있는
`PDO::cubrid_schema <http://kr.php.net/manual/en/pdo.cubrid-schema.php>`_
() 함수를 제공한다. 다음은
`PDO::cubrid_schema <http://kr.php.net/manual/en/pdo.cubrid-schema.php>`_
()?함수를 이용하여
*nation*
테이블의 기본키를 반환하는 스크립트이다.

<?php

include("_db_config.php");

include("_db_connect.php");

try{

echo"Get PRIMARY KEY for table: <b>nation</b>:

?

";

$pk_list = $db->cubrid_schema(PDO::CUBRID_SCH_PRIMARY_KEY,"nation");

print_r($pk_list);

}catch(PDOException $e){

echo $e->getMessage();

}

?

$db = null;//disconnect

?>

위 스크립트의 결과는 다음과 같이 출력된다.

Get PRIMARY KEY for table: nation:

Array ( [0] => Array ( [CLASS_NAME] => nation [ATTR_NAME] => code [KEY_SEQ] => 1 [KEY_NAME] => pk_nation_code ) )

**참고**
PDO 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver>`_
에 접속한다.

**PDO API**

PDO API에 대한 자세한 내용은
`http://docs.php.net/manual/en/book.pdo.php <http://docs.php.net/manual/en/book.pdo.php>`_
를 참고한다. CUBRID PDO 드라이버가 제공하는 API는 다음과 같다.

*   `PDO_CUBRID DSN <http://www.php.net/manual/en/ref.pdo-cubrid.connection.php>`_



*   `PDO::cubrid_schema <http://www.php.net/manual/en/pdo.cubrid-schema.php>`_



**참고**
PDO 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-pdo-driver>`_
에 접속한다.
