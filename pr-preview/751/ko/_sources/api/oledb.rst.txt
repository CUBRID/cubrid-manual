
:meta-keywords: cubrid oledb driver, cubrid ole db driver, cubrid oledb api, cubrid ole db api, oledb programming
:meta-description: CUBRID OLE DB driver is based on CCI API and provides an interface over .NET Framework using the Component Object Model (COM).

***************
OLE DB 드라이버
***************

OLE DB(Object Linking and Embedding, Database)는 Microsoft에서 개발한 COM(Component Object Model) 기반 인터페이스로, 데이터가 저장된 형식에 상관없이 데이터에 접근할 수 있는 일반적인 방법을 제공한다.

.NET 프레임워크는 Windows 응용 프로그램 개발을 위한 프레임워크로, 언어 간 상호 운용성을 지원한다. .NET이 지원하는 모든 프로그래밍 언어는 .NET 라이브러리를 사용할 수 있다. .NET 프레임워크의 데이터 공급자는 데이터베이스에 연결하고 명령을 실행하며 결과를 검색하는 데 사용된다.

CUBRID OLE DB 드라이버는 CCI API를 기반으로 작성되었으므로, CCI API 및 CCI에 적용되는 **CCI_DEFAULT_AUTOCOMMIT** 과 같은 설정 파라미터에 영향을 받는다.

.. FIXME: 별도로 OLE DB 드라이버를 다운로드하거나 OLE DB 드라이버에 대한 최신 정보를 확인하려면 http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver\ 에 접속한다.

.. note:: 

    *   CUBRID OLEDB 드라이버 버전이 9.1.0.p1 이상이면, 32비트와 64비트 통합 설치 패키지 하나만 설치하면 된다. 이 인스톨러는 CUBRID DB 엔진 2008 R4.1 이상 버전을 지원한다.
    *   CUBRID OLEDB 드라이버 버전이 9.1.0 이하이면, 64비트 OS에서 문제가 발생할 수 있다. 

.. FIXME: Please see our installation tutorial for an old version: http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver-installation-instructions-old

OLE DB 설치 및 설정
===================

**CUBRID OLE DB 공급자**

CUBRID를 이용하는 응용 프로그램을 개발하려면 CUBRID OLE DB 공급자 드라이버( **CUBRIDProvider.dll** )가 필요하다. 드라이버 파일을 얻으려면 다음 중 하나를 수행한다.

*   **드라이버 설치하기**: CUBRID OLED DB Data Provider Installer의 .exe 파일을  http://ftp.cubrid.org/CUBRID_Drivers/OLEDB_Driver/ 또는 http://www.cubrid.org/?mid=downloads&item=oledb_driver 위치에서 내려받아 실행한다. OLE DB 드라이버 9.1.0.p1 이상 버전(CUBRID 서버 2008 R4.1부터 이 드라이버 사용 가능)부터는 다운받은 파일을 실행하면 32비트와 64비트 둘 다 설치된다.

    .. image:: /images/oledb_install.jpg

    *   설치된 디렉터리에는 다음 파일이 존재한다.
    
        *   CUBRIDProvider32.dll
        *   CUBRIDProvider64.dll
        *   README.txt
        *   uninstall.exe    

*   **소스 코드에서 빌드하기**: CUBRID OLED DB Data Provider Installer를 변경하고 싶으면 소스 코드를 컴파일하여 직접 CUBRID OLED DB Data Provider Installer를 빌드할 수 있다. 자세한 내용은 다음 주소를 참고한다.

.. FIXME: For details, see below:

.. FIXME:    http://www.cubrid.org/wiki_apis/entry/compiling-the-cubrid-ole-db-installer

    CUBRID OLED DB Data Provider Installer를 이용하지 않는 경우에는 운영체제 버전에 따라 다음 명령어를 실행하여 드라이버를 등록해야 한다. 이때 32비트 Windows에서는 **C:\Windows\system32** 디렉터리의 regsvr32가 실행되어야 하고, 64비트 Windows에서는 **C:\Windows\SysWOW64** 디렉터리의 regsvr32가 실행되어야 한다. ::

        regsvr32 CUBRIDProvider.dll

OLE DB 프로그래밍
=================

데이터 연결 속성 대화 상자 사용
-------------------------------

Visual Studio .NET에서 대화 상자에 접근하기 위해, "도구" 메뉴에서 "데이터베이스 연결"을 선택하거나 Server Explorer에 있는 "데이터베이스 연결" 아이콘을 클릭한다.

*   먼저 Visual Studio를 설치한 후, "데이터베이스 연결"을 클릭한다.

    .. image:: /images/oledb_1_connect.jpg

*   Data source로 <other>를 선택하고, Data Provider로 .Net Framework Data Provider for OLE DB를 선택한다. 그리고 나서 "계속" 버튼을 클릭한다.

    .. image:: /images/oledb_2_select.jpg

*   CUBRID OLEDB Provider를 선택하고 "데이터 연결" 버튼을 클릭한다.

   .. image:: /images/oledb_3_datalink.jpg

*   정보를 채우고 "연결 테스트" 버튼을 클릭한다. 연결에 성공하면, 성공했다는 대화 상자가 팝업된다.

    보다 자세한 설명은 MSDN에 있는 http://msdn.microsoft.com/en-us/library/79t8s5dk(v=vs.71).aspx 을 참고한다.

   .. image:: /images/oledb_4_confconn.jpg

또는 윈도 탐색기에서 universal data link(.udl) 파일을 더블 클릭하여 해당 대화 상자를 열 수 있다.

*   먼저 임의의 텍스트 파일을 만들고 확장자를 .udl로 변경한다(1.txt -> 1.udl). 다음으로, 1.udl을 클릭하면 아래의 대화 상자가 팝업된다.

    이 때, 공급자(Provider)를 CUBRID OLE DB Provider로 변경한다.

   .. image:: /images/oledb_confbox.jpg

*   문자셋 설정

    universal data link(.udl) 파일을 텍스트 편집기로 열면 아래와 같이 나타나는데, "Charset=utf-8;"이 문자셋을 설정하는 부분이다.
    
    "Provider=CUBRIDProvider;Data Source=demodb;Location=127.0.0.1;User ID=dba;Password=;Port=33000;Fetch Size=100;Charset=utf-8;"

*   격리 수준 설정

    아래 문자열에서 "Autocommit Isolation Levels=256;"이 격리 수준(isolation level)을 설정하는 부분이다. 이 기능은 드라이버 버전 9.1.0.p2 이상에서만 지원하며, 연결 문자열에서 지정하지 않으면 4096을 기본으로 지정한다.

    ::
    
        "Provider=CUBRIDProvider;Data Source=demodb;Location=10.34.64.104;User ID=dba;Password=;Port=30000;Fetch Size=100;Charset=utf-8;Autocommit Isolation Levels=256;"

    +--------------------------------+-------------------------------------+---------+
    | OLE DB	                     | CUBRID                              | Value   |
    +================================+=====================================+=========+
    | ISOLATIONLEVEL_READUNCOMMITTED | TRAN_COMMIT_CLASS_UNCOMMIT_INSTANCE | 256     |
    +--------------------------------+-------------------------------------+---------+
    | ISOLATIONLEVEL_READCOMMITTED   | TRAN_COMMIT_CLASS_COMMIT_INSTANCE   | 4096    |
    +--------------------------------+-------------------------------------+---------+
    | ISOLATIONLEVEL_REPEATABLEREAD  | TRAN_REP_CLASS_REP_INSTANCE         | 65536   |
    +--------------------------------+-------------------------------------+---------+
    | ISOLATIONLEVEL_SERIALIZABLE    | TRAN_SERIALIZABLE                   | 1048576 |
    +--------------------------------+-------------------------------------+---------+

    note:: CUBRID OLE DB에서 "Autocommit Isolation Levels"은 연결 문자열에서만 지정 가능하며, 트랜잭션에서는 지정 불가하다. 즉, OleDbConnection.BeginTransaction() 함수에서 isolation level을 변경해도 적용되지 않는다.

연결 문자열(connection string) 구성
-----------------------------------

CUBRID OLE DB Provider로 프로그래밍을 할 때 연결 문자열(connection string)의 속성은 다음과 같다.

+-----------------------------+----------------+---------------------------------------------+
| 항목                        | 예             | 설명                                        |
+=============================+================+=============================================+
| Provider                    | CUBRIDProvider | 공급자 이름                                 |
+-----------------------------+----------------+---------------------------------------------+
| Data Source                 | demodb         | 데이터베이스 이름                           |
+-----------------------------+----------------+---------------------------------------------+
| Location                    | 127.0.0.1      | CUBRID 브로커 서버 IP 주소 또는 호스트 이름 |
+-----------------------------+----------------+---------------------------------------------+
| User ID                     | PUBLIC         | 사용자 ID                                   |
+-----------------------------+----------------+---------------------------------------------+
| Password                    | xxx            | 비밀번호                                    |
+-----------------------------+----------------+---------------------------------------------+
| Port                        | 33000          | 브로커 Port 번호                            |
+-----------------------------+----------------+---------------------------------------------+
| Fetch Size                  | 100            | Fetch 크기                                  |
+-----------------------------+----------------+---------------------------------------------+
| Charset                     | utf-8          | 문자셋                                      |
+-----------------------------+----------------+---------------------------------------------+
| Autocommit Isolation Levels | 4096           | isolation level                             |
+-----------------------------+----------------+---------------------------------------------+

위의 예를 이용한 연결 문자열은 다음과 같다. ::

    "Provider=CUBRIDProvider;Data Source=demodb;Location=127.0.0.1;User ID=PUBLIC;Password=xxx;Port= 33000;Fetch Size=100;Charset=utf-8;Autocommit Isolation Levels=256;"

.. note::

    *   연결 문자열에서 세미콜론(;)은 구분자로 사용되므로, 연결 문자열에 암호(Password)를 지정할 때 암호의 일부에 세미콜론을 사용할 수 없다.
    *   칼럼에서 정의한 크기보다 큰 문자열을 **INSERT** / **UPDATE** 하면 문자열이 잘려서 입력된다.
    *   스레드 기반 프로그램에서 데이터베이스 연결은 각 스레드마다 독립적으로 사용해야 한다.
    *   자동 커밋 모드에서 SELECT 문 수행 이후 모든 결과 셋이 fetch되지 않으면 커밋이 되지 않는다. 따라서, 자동 커밋 모드라 하더라도 프로그램 내에서 결과 셋에 대한 fetch 도중 어떠한 오류가 발생한다면 반드시 커밋 또는 롤백을 수행하여 트랜잭션을 종료 처리하도록 한다. 

.NET 환경에서의 멀티 스레드 프로그래밍
--------------------------------------

Microsoft의 .NET 환경에서 CUBRID OLE DB Provider를 이용하여 프로그래밍할 때 추가로 고려해야 할 사항은 다음과 같다.

관리 환경에서 ADO.NET을 통한 멀티 스레드 프로그래밍을 할 때에는, CUBRID OLE DB Provider가 오직 STA(Single Threaded Apartment) 속성만을 지원하므로, Thread 객체의 ApartmentState 속성 값을 ApartmentState.STA 값으로 변경해야 한다.

만약 아무런 설정을 하지 않는다면 Thread 객체의 이 속성 기본값으로 Unknown 값이 반환되기 때문에 멀티 스레드 프로그래밍 시 비정상적으로 동작할 수 있다.

.. warning::

    OLE DB의 모든 객체는 COM 객체이다. 현재 CUBRID OLE DB Provider는 COM threading model 중 apartment threading model만을 지원하고 free threading model은 지원하지 않는다. 이는 .NET 환경에만 해당하는 사항은 아니고 모든 multi-threaded 환경에 해당하는 내용이다.

OLE DB API
==========

OLE DB API에 대한 자세한 내용은 Micorosoft OLE DB 문서( http://msdn.microsoft.com/en-us/library/ms722784%28VS.85%29.aspx )를 참고한다.

CUBRID OLE DB에 대한 자세한 내용은 http://ftp.cubrid.org/CUBRID_Docs/Drivers/OLEDB/\를 참고한다.

