****
소개
****

개요
====

.. image:: /images/cm-splash.png

CUBRID Manager는 GUI 환경에서 CUBRID 데이터베이스 관리 및 질의 기능을 제공하는 CUBRID 데이터베이스 전용 관리 도구이다. 
CUBRID Manager의 관리 기능은 CUBRID 데이터베이스의 관리 작업을 GUI를 이용하여 쉽게 할 수 있다. 또한, CUBRID Manager의 질의 기능은 응용 개발에 필요한 질의 편집기 도구를 제공한다.

CUBRID Manager의 관리 기능은 CUBRID DBMS가 설치된 서버에서 동작하는 CUBRID Manager Server가 구동 중인 환경에서만 사용할 수 있지만, 질의 기능은 CUBRID Manager Server의 구동 없이도 수행이 가능하다.

CUBRID Manager 클라이언트는 Java 응용 프로그램으로 JRE 혹은 JDK 1.6 이상 버전에서만 실행되며, 권장하는 JRE 버전은 1.7 이다.
낮은 버전에서 실행할 경우 "지원하지 않는 JRE 버전입니다. CUBRID Manager를 사용하기 위해서는 JRE 1.6 이상의 버전을 사용해야 합니다."라는 오류 메시지가 출력된다.

    
구조
====

아래 그림은 CUBRID Manager가 서버와 통신하는 구조를 보여주고 있다. 

CUBRID Manager는 관리 모드(manage mode)와 질의 모드(query mode)의 두 가지를 제공한다. 

*   관리 모드에서는 CUBRID Manager Server의 8001번, 8002번 포트를 통해 관리에 필요한 데이터를 교환하여 작업을 실행한다.
*   질의 모드에서는 JDBC를 통해 CUBRID 브로커의 포트(설치 시 30000번, 33000번이 기본으로 제공됨)를 통해 질의 및 결과를 주고 받는다.

관리 모드에서는 데이터베이스 백업, 복구, 검사, 최적화, 로드/언로드, 시작, 종료, 생성, 삭제, 복사 등과 같이 시스템 콘솔에서 명령어를 통해 수행할 수 있는 대부분의 작업을 GUI로 제공한다. 

질의 모드에서는 SQL을 실행하고 결과를 확인하는 질의 편집기와, 테이블, 뷰, 시리얼 등 데이터베이스 객체를 생성, 관리, 조회할 수 있는 UI 도구를 제공한다.

.. image:: /images/cm-arch.png

.. note::

    *   타 장비에 CUBRID 서버를 설치한 후 PC에서 CUBRID Manager 관리 모드를 사용하여 데이터베이스 호스트에 접속이 실패하는 경우의 대부분은 8001, 8002번 포트에 대해 방화벽(Firewall)을 해제하지 않은 경우이다. 
    *   시스템 관리자가 별도로 있고 포트 또는 권한이 없을 경우에는 질의 모드를 이용하여 30000, 33000번 포트로 접속하여 데이터를 조회, 편집 작업을 할 수 있다.
