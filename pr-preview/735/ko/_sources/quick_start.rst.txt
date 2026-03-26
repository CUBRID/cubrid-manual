
:meta-keywords: cubrid service, cubrid shell, cubrid create database, cubrid start database
:meta-description: CUBRID quick start guide. How to launch CUBRID service, create and start a database.

CUBRID 서비스 시작
==================

환경 변수 및 언어 설정을 완료한 후, CUBRID 서비스를 시작한다. 이에 대한 자세한 설명은 :ref:`control-cubrid-services` 를 참고한다.

셸 명령어
---------

Linux 환경 또는 Windows 환경에서 아래와 같은 셸 명령어로 CUBRID 서비스를 시작하고, 설치 패키지에 포함된 demodb를 구동할 수 있다. ::

    % cubrid service start

    @ cubrid master start
    ++ cubrid master start: success

    @ cubrid broker start
    ++ cubrid broker start: success

    @ cubrid manager server start
    ++ cubrid manager server start: success

    % cubrid server start demodb

    @ cubrid server start: demodb

    This may take a long time depending on the amount of recovery works to do.

    CUBRID 10.1 

    ++ cubrid server start: success

    @ cubrid server status

    Server demodb (rel 10.1, pid 31322)

CUBRIDService 또는 CUBRID Service Tray
--------------------------------------

Windows 환경에서는 다음과 같은 방법으로 CUBRID 서비스를 시작하거나 중지할 수 있다.

*   [제어판] > [성능 및 유지 관리] > [관리도구] > [서비스]에 등록된 CUBRIDService를 선택하여 시작하거나 중지한다.

    .. image:: /images/image5.jpg

*   시스템 트레이에서 CUBRID Service Tray를 마우스 오른쪽 버튼으로 클릭한 후, CUBRID를 시작하려면 [Service Start]를 선택하고 중지하려면 [Service Stop]을 선택한다. 

    시스템 트레이에서 [Service Start]/[Service Stop] 메뉴를 선택하면, 명령어 프롬프트 창에서 **cubrid service start** / **cubrid service stop** 을 실행했을 때와 같은 동작을 수행하며, **cubrid.conf**\의 **service** 파라미터에 설정한 프로세스들을 구동/중지한다.

*   CUBRID가 실행 중일 때 CUBRID 서비스 트레이에서 [Exit]를 선택하면, 해당 서버에서 실행 중인 모든 서비스와 프로세스가 중지되므로 주의한다.

.. note::

    CUBRID 서비스 트레이를 통해 CUBRID 관련 프로세스를 시작/종료하는 작업은 관리자 권한(SYSTEM)으로 수행되고, 셸 명령어로 시작/종료하는 작업은 로그인한 사용자 권한으로 수행된다. Windows Vista 이상 버전의 환경에서 셸 명령어로 CUBRID 프로세스가 제어되지 않는 경우, 명령 프롬프트 창을 관리자 권한으로 실행([시작] > [모든 프로그램] > [보조 프로그램] > [명령 프롬프트]를 마우스 오른쪽 버튼으로 클릭하여 [관리자 권한으로 실행] 선택)하거나 CUBRID 서비스 트레이를 이용해서 해당 작업을 수행할 수 있다.
    CUBRID 서버 프로세스가 모두 중단되면, CUBRID Service Tray 아이콘이 회색으로 변한다.

데이터베이스 생성
-----------------

데이터베이스 볼륨 및 로그 볼륨이 위치할 디렉터리에서 **cubrid createdb** 유틸리티를 실행하여 데이터베이스를 생성할 수 있다. **--db-volume-size** 또는 **--log-volume-size** 와 같은 추가 옵션을 지정하지 않으면 기본적으로 1.5GB 크기의 볼륨 파일이 생성된다(데이터 볼륨 512MB, 활성 로그 512MB, 백그라운드 보관 로그 512MB로 설정됨). ::

    % cd testdb
    % cubrid createdb testdb en_US
    % ls -l

    -rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb
    -rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb_lgar_t
    -rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb_lgat
    -rw------- 1 cubrid dbms       176 Jan 11 15:04 testdb_lginf
    -rw------- 1 cubrid dbms       183 Jan 11 15:04 testdb_vinf

위에서 *testdb* 는 데이터 볼륨 파일을, testdb_lgar_t는 백그라운드 보관 로그 파일을, testdb_lgat는 활성 로그 파일을, testdb_lginf는 로그 정보 파일을, 그리고 testdb_vinf는 볼륨 정보 파일을 나타낸다.

볼륨에 대한 자세한 내용은 :ref:`database-volume-structure` 를, 볼륨 생성에 대한 자세한 내용은 :ref:`creating-database` 를 참고한다. **cubrid addvoldb** 유틸리티를 사용해 용도에 따라 볼륨을 분류해 추가하도록 권장한다. 자세한 내용은 :ref:`adding-database-volume` 을 참고한다.

데이터베이스 시작
-----------------

데이터베이스 프로세스를 시작하려면 **cubrid** 명령어를 이용한다. ::

    % cubrid server start testdb

앞에서 설명한 CUBRID 서비스 시작(**cubrid service start**) 시 *testdb* 가 같이 시작되게 하려면, **cubrid.conf** 파일의 **server** 파라미터에 *testdb* 를 설정한다. ::

    % vi cubrid.conf

    [service]

    service=server,broker,manager
    server=testdb

    ...
