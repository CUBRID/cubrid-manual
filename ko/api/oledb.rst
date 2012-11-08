***************
OLE DB 드라이버
***************

**개요**

OLE DB(Object Linking and Embedding, Database)는 Microsoft에서 개발한 COM(Component Object Model) 기반 인터페이스로, 데이터가 저장된 형식에 상관없이 데이터에 접근할 수 있는 일반적인 방법을 제공한다.

.NET 프레임워크는 Windows 응용 프로그램 개발을 위한 프레임워크로, 언어 간 상호 운용성을 지원한다. .NET이 지원하는 모든 프로그래밍 언어는 .NET 라이브러리를 사용할 수 있다. .NET 프레임워크의 데이터 공급자는 데이터베이스에 연결하고 명령을 실행하며 결과를 검색하는 데 사용된다.

CUBRID OLE DB?드라이버는 CCI API를 기반으로 작성되었으므로, CCI API 및 CCI에 적용되는
**CCI_DEFAULT_AUTOCOMMIT**
과 같은 설정 파라미터에 영향을 받는다.

별도로 OLE DB?드라이버를 다운로드하거나 OLE DB?드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver>`_
에 접속한다.

**OLE DB**
**설치**
**및**
**설정**

**기본**
**환경**

*   Windows Vista 또는 Windows 7, 32비트 버전



*   CUBRID 2008 R4.1(8.4.1) 이상, 32비트 버전



*   Visual Studio 2010 Express Edition(
    `http://www.microsoft.com/visualstudio/en-us/products/2010-editions/express <http://www.microsoft.com/visualstudio/en-us/products/2010-editions/express>`_
    )



*   .NET framework 3.5 이상(
    `http://www.microsoft.com/download/en/details.aspx?id=21 <http://www.microsoft.com/download/en/details.aspx?id=21>`_
    )



**CUBRID OLE DB**
**공급자**

CUBRID를 이용하는 응용 프로그램을 개발하려면 CUBRID OLE DB 공급자 드라이버(
**CUBRIDProvider.dll**
)가 필요하다. 드라이버 파일을 얻으려면 다음 중 하나를 수행한다.

*   CUBRID OLED DB Data Provider Installer를 다음 위치에서 내려받아 실행한다. 운영체제에 맞는 버전(32비트/64비트)을 다운로드해야 한다.



`http://www.cubrid.org/?mid=downloads&item=oledb_driver <http://www.cubrid.org/?mid=downloads&item=oledb_driver>`_

설치 방법은 다음 동영상을 참고한다.

`http://www.youtube.com/watch?v=FN_6c9x9UOA <http://www.youtube.com/watch?v=FN_6c9x9UOA>`_

*   CUBRID OLED DB Data Provider Installer를 변경하고 싶으면 소스코드를 컴파일하여 직접 CUBRID OLED DB Data Provider Installer를 빌드할 수 있다. 자세한 내용은 다음 주소를 참고한다.



`http://www.cubrid.org/wiki_apis/entry/compiling-the-cubrid-ole-db-installer <http://www.cubrid.org/wiki_apis/entry/compiling-the-cubrid-ole-db-installer>`_

*   다음 위치에서 드라이버를 다운로드한다. 운영체제에 맞는 버전(32비트/64비트)을 다운로드해야 한다.



`http://www.cubrid.org/?mid=downloads&item=oledb_driver <http://www.cubrid.org/?mid=downloads&item=oledb_driver>`_

*   드라이버 소스코드를 컴파일한다. 소스코드는 다음 SVN 저장소에서 체크아웃한다. 소스코드를 컴파일하려면 Visual Studio Express edition(
    `http://www.microsoft.com/express/Downloads/#2010-Visual-CPP <http://www.microsoft.com/express/Downloads/#2010-Visual-CPP>`_
    )을 사용한다. CUBRID 버전이 9.0.0이라면
    *<CUBRID 버전>*
    에는 9.0.0을 입력한다.



http://svn.cubrid.org/cubridapis/oledb/branches/RB-
*<CUBRID 버전>*
/Source/

CUBRID OLED DB Data Provider Installer를 이용하지 않는 경우에는 운영체제 버전에 따라 다음 명령어를 실행하여 드라이버를 등록해야 한다. 이때 32비트 Windows에서는
**C:\Windows\system32**
디렉터리의 regsvr32가 실행되어야 하고, 64비트 Windows에서는
**C:\Windows\SysWOW64**
디렉터리의 regsvr32가 실행되어야 한다.

regsvr32 CUBRIDProvider.dll

**참고**
OLE DB 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver>`_
에 접속한다.

**OLE DB**
**프로그래밍**

**데이터**
**연결**
**속성**
**대화**
**상자**
**사용**

[데이터 연결 속성] 대화 상자에서는 현재 사용하고 있는 Windows 운영 체제에 있는 각종 OLE DB 공급자를 확인하고 연결 속성을 설정할 수 있다.

Windows용 CUBRID OLE DB Provider를 올바르게 설치하였다면 다음?그림과 같이 [데이터 연결 속성] 대화 상자의 공급자 목록에 'CUBRID OLE DB Provider'가 나타난다.

.. image:: /images/image84.png

'CUBRID OLE DB Provider'를 선택한 뒤 [다음] 버튼을 클릭하면 다음과 같이 [연결] 탭이 나타난다. [연결] 탭에서 원하는 연결 속성을 설정한다.

.. image:: /images/image85.png

*   **데이터 원본**
    : CUBRID 데이터베이스의 이름을 입력한다.



*   **위치**
    : CUBRID 브로커가 구동 중인 서버의 IP 주소 또는 호스트 이름을 입력한다.



*   **사용자 이름**
    : 데이터베이스 서버에 로그온할 때 사용할 사용자 이름을 입력한다.



*   **암호**
    : 데이터베이스 서버에 로그온할 때 사용할 암호를 입력한다.



연결 속성을 모두 설정한 후 [모두] 탭을 누른다.

.. image:: /images/image86.png

[모두] 탭을 클릭하면 현재 설정한 각각의 항목 값을 확인할 수 있다. 설정 값을 편집하려면 값을 편집하려는 항목을 더블 클릭한다. [속성 값 편집] 대화 상자가 나타나면 원하는 값을 입력한 뒤 [확인]을 누른다. 위 그림은 [Port] 번호는 '31000', [Fetch Size]는 '100'으로 설정한 예이다.

모든 설정을 마친 뒤, [연결] 탭에서 [연결 테스트] 버튼을 클릭하면 연결이 제대로 되는지 테스트해 볼 수 있다.

.. image:: /images/image87.png

**연결**
**문자열(connection string)**
**구성**

ADO(ActiveX Data Object) 혹은 ADO.net을 이용하여 CUBRID OLE DB Provider 프로그래밍을 할 때 연결 문자열(connection string)은 다음과 같이 작성한다.

+-------------+----------------+-------------------------------+
| **항목**      | **예**          | **설명**                        |
|             |                |                               |
+-------------+----------------+-------------------------------+
| Provider    | CUBRIDProvider | 공급자 이름                        |
|             |                |                               |
+-------------+----------------+-------------------------------+
| Data Source | demodb         | 데이터베이스 이름                     |
|             |                |                               |
+-------------+----------------+-------------------------------+
| Location    | 127.0.0.1      | CUBRID 브로커 서버 IP 주소 또는 호스트 이름 |
|             |                |                               |
+-------------+----------------+-------------------------------+
| User ID     | PUBLIC         | 사용자 ID                        |
|             |                |                               |
+-------------+----------------+-------------------------------+
| Password    | xxx            | 비밀번호                          |
|             |                |                               |
+-------------+----------------+-------------------------------+
| Port        | 33000          | 브로커 Port 번호                   |
|             |                |                               |
+-------------+----------------+-------------------------------+
| Fetch Size  | 100            | Fetch 크기                      |
|             |                |                               |
+-------------+----------------+-------------------------------+

위의 예를 이용한 연결 문자열은 다음과 같다.

"Provider=CUBRIDProvider;Data Source=demodb;Location=127.0.0.1;User ID=PUBLIC;Password=xxx;Port= 33000;Fetch Size=100"

**주의**
**사항**

*   연결 문자열에서 세미콜론(;)은 구분자로 사용되므로, 연결 문자열에 암호(Password)를 지정할 때 암호의 일부에 세미콜론을 사용할 수 없다.



*   칼럼에서 정의한 크기보다 큰 문자열을
    **INSERT**
    /
    **UPDATE**
    하면 문자열이 잘려서 입력된다.



*   스레드 기반 프로그램에서 데이터베이스 연결은 각 스레드마다 독립적으로 사용해야 한다.



**.NET**
**환경에서의**
**멀티**
**스레드**
**프로그래밍**

Microsoft의 .NET 환경에서 CUBRID OLE DB Provider를 이용하여 프로그래밍할 때 추가로 고려해야 할 사항은 다음과 같다.

관리 환경에서 ADO.NET을 통한 멀티 스레드 프로그래밍을 할 때에는, CUBRID OLE DB Provider가 오직 STA(Single Threaded Apartment) 속성만을 지원하므로, Thread 객체의 ApartmentState 속성 값을 ApartmentState.STA 값으로 변경해야 한다.

만약 아무런 설정을 하지 않는다면 Thread 객체의 이 속성 기본값으로 Unknown 값이 반환되기 때문에 멀티 스레드 프로그래밍 시 비정상적으로 동작할 수 있다.

**주의**
OLE DB의 모든 객체는 COM 객체이다. 현재 CUBRID OLE DB Provider는 COM threading model 중 apartment threading model만을 지원하고 free threading model은 지원하지 않는다. 이는 .NET 환경에만 해당하는 사항은 아니고 모든 multi-threaded 환경에 해당하는 내용이다.

**참고**
OLE DB 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver>`_
에 접속한다.

**OLE DB API**

OLE DB API에 대한 자세한 내용은 Micorosoft OLE DB 문서(
`http://msdn.microsoft.com/en-us/library/ms722784%28VS.85%29.aspx <http://msdn.microsoft.com/en-us/library/ms722784%28VS.85%29.aspx>`_
)를 참고한다.

**참고**
OLE DB 드라이버에 대한 최신 정보를 확인하려면
`http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-oledb-driver>`_
에 접속한다.
