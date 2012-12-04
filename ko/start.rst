***********
CUBRID 시작
***********

CUBRID를 처음 사용하는데 참고할 수 있는 간략한 사용법을 설명한다. 시스템 설치 및 실행 방법, CSQL 인터프리터의 간단한 사용법을 찾아볼 수 있다. 또한, JDBC나 PHP, ODBC, 그리고 CCI 등을 이용한 응용 프로그램 작성 예제도 포함되어 있다.

이 장에서 설명하는 주요 내용은 다음과 같다.

* 설치와 실행
* 환경 변수 설정 및 CUBRID 서비스 시작
* CSQL 인터프리터 사용

CUBRID가 사용하는 포트에 대해서는 "Windows에서의 설치와 실행"의 :ref:`환경 설정 <Configuring-Environment-on-Windows>` 을 참고한다. 
Linux 환경에서는 **APPL_SERVER_PORT** 를 제외한 나머지 포트 설정이 Windows 환경에서의 포트 설정과 동일하다.

CUBRID의 다양한 도구 및 드라이버는 `http://www.cubrid.org/downloads <http://www.cubrid.org/downloads>`_ 에서 다운로드할 수 있다. 
CUBRID가 제공하는 다음 도구를 사용하면 GUI를 통해 편리하게 CUBRID의 기능을 이용할 수 있다.

* CUBRID 쿼리 브라우저 : `http://www.cubrid.org/wiki_tools/entry/cubrid-query-browser <http://www.cubrid.org/wiki_tools/entry/cubrid-query-browser>`_

* CUBRID 매니저 : `http://www.cubrid.org/wiki_tools/entry/cubrid-manager <http://www.cubrid.org/wiki_tools/entry/cubrid-manager>`_

* CUBRID 웹 매니저: `http://www.cubrid.org/wiki_tools/entry/cubrid-web-manager <http://www.cubrid.org/wiki_tools/entry/cubrid-web-manager>`_

* CUBRID 마이그레이션 툴킷 : `http://www.cubrid.org/wiki_tools/entry/cubrid-migration-toolkit <http://www.cubrid.org/wiki_tools/entry/cubrid-migration-toolkit>`_

CUBRID는 JDBC, CCI, PHP, PDO, ODBC, OLE DB, ADO.NET, Perl, Python, Ruby 등 다양한 드라이버를 제공한다. 드라이버에 대한 자세한 내용은 API 레퍼런스를 참고한다.

.. toctree::

	install.rst
	env.rst
	qrytool.rst
