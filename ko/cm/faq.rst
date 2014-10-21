***
FAQ
***

Q1: 오류가 있는 SQL 쿼리를 실행하면 한글로 출력되는 오류 메시지가 깨져 보임
===========================================================================

오류 메시지가 깨어지는 경우 대부분이 리눅스 서버를 윈도우즈 클라이언트가 접속했을 경우이다.

우선 CUBRID DBMS의 환경 변수를 확인한다.

아마도 $CUBRID_LANG 값과 클라이언트의 환경이 맞지 않을 경우가 대부분일 것이며 이 경우 오류 메시지가 깨어질 수 있다.

다음 4가지 경우와 같이 서버와 클라이언트의 환경을 맞추어주면 오류 메시지는 깨어지지 않는다.

1.  서버(리눅스) + 클라이언트(윈도우즈) : 서버의 $CUBRID_LANG=ko_KR.euckr 또는 $CUBRID_LANG=en_US.utf-8로 설정
2.  서버(리눅스) + 클라이언트(리눅스) : 서버의 $CUBRID_LANG과 클라이언트의 $LANG을 동일하게 설정
3.  서버(윈도우즈) + 클라이언트(리눅스) : 서버의 $CUBRID_LANG과 클라이언트의 $LANG을 ko_KR.euc-kr로 설정
4.  서버(윈도우즈) + 클라이언트(윈도우즈) : 문제 발생하지 않음

만약, 영문으로만 출력을 원하면 $CUBRID_LANG=en_US.UTF-8로 변경하면 된다.
 
Q2: CUBRID 매니저 서버를 최신 버전으로 업그레이드
=================================================

CUBRID 매니저 (이하 CM) 서버는 CUBRID 엔진 패키지에 함께 배포되지만, 내부적으로는 독립된 프로세스로서 CM 클라이언트와 통신한다. 간혹 CM 서버에 버그가 있는 경우 CM 서버만 별도 릴리스하게 되는데, http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Manager_Server/\ 에서 다운받을 수 있다.

1)  압축을 풀면, CM Server와 관련된 바이너리 4개(cub_auto, cub_ftproc, cub_job, cub_js)가 포함되어 있다. 
2)  아래 명령어를 사용하여 현재 구동 중인 CM 서버를 종료한다. DB서버와는 독립되어 있으므로 현재 운영 중인 서비스에 영향을 끼치지 않는다. 

    ::

        C:\> cubrid manager stop 

3)  CUBRID/bin 디렉토리에서 현재 버전의 바이너리 4개(cub_auto, cub_ftproc, cub_job, cub_js)가 존재하는지 확인하고, 필요하다면 백업한다. 이후 1)에서 받은 신 버전의 바이너리 4개로 덮어쓰기(overwrite)하면 된다.
4)  아래 명령어로 CM 서버를 시작한다. 

    ::
    
        C:\> cubrid manager start 
 
Q3: csql에서는 Java Procedure 실행시 한글 깨지지 않는데, CUBRID 매니저에서는 한글이 깨질 때
===========================================================================================

CUBRID 매니저와 데이터 및 Java Procedure의 문자집합을 동일하게 맞춰주어야 깨어지지 않는다. 연결 시 문자집합에 UTF-8 또는 EUC-KR 등 서로 동일하게 맞춰주면 이상없이 결과가 출력된다.
 
Q4: CUBRID 매니저로 demodb를 시작하자마자 종료가 되면서 standalone이라는 메시지가 출력될 때
===========================================================================================

서버에서 standalone이 나오는 경우는 서버가 완전히 종료되지 않은 상태에서 다시 시작을 했을 경우 이 현상이 발생한다. 완전히 종료한 후에 다시 시작하면 된다.
 
Q5: 기존 8.2.2 버전 CUBRID 매니저에서 8.4.1 CUBRID 매니저로 기존 Host 접속 정보를 마이그레이션하는 방법은 없는지?
=================================================================================================================

CUBRID 매니저 8.2.2와 CUBRID 매니저 8.4.1은 Host와 같이 일부분의 정보는 마이그레이션이 가능하다.

File > Import Hosts를 선택하면 나오는 대화창에서 기존 CUBRID 매니저의 workspace를 선택하면 Host 정보를 마이그레이션할 수 있다.
 
만약, 필요한 Host를 수작업으로 마이그레이션하려면 Host 설정 파일을 직접 편집할 수도 있다.

CUBRID 매니저의 Host 정보는 아래의 경로에 있다.


::

    cubridmanager\workspace\.metadata\.plugins\org.eclipse.core.runtime\.settings\com.cubrid.cubridmanager.ui.prefs
    
CUBRID_SERVERS 부분을 편집해서 복사하고 붙여넣으면 정상적으로 사용할 수 있다.
 
Q6: CUBRID 매니저의 admin 비밀번호를 분실했을 때
================================================

{CUBRID 설치 폴더}/conf/cm.pass 파일을 편집하여 admin에 해당하는 줄을 아래의 값으로 교체한 다음 저장하고 CUBRID 매니저로 다시 접속하면 기본 비밀번호인 admin으로 접속할 수 있게 된다. 접속 후 바로 비밀번호 변경 대화창이 뜨면 변경하여 사용하면 된다.

::

    admin:6e85f0f80f030451dc9e98851098dfb2
 
Q7: CUBRID 매니저로 매니저 서버에 접속되지 않을 때 어떻게 조치하나?
===================================================================

CUBRID 매니저 Server가 정상적으로 실행중인지 확인한다. cub_auto, cub_js 프로세스가 정상적으로 실행 중인지는 다음의 명령어로 확인할 수 있다. 

*   Linux는 ps -ef | grep "cub"로 확인한다.
*   윈도우즈는 작업관리자의 프로세스탭에서 cub_auto.exe, cub_js.exe가 실행 중인지 확인한다.
*   "cubrid manager stop" 후 "cubrid manager start"를 시도한다.

간혹 CUBRID DBMS를 업그레이드하면서 기존 버전이 언인스톨되지 않은 상태에서 설치할 경우 정상적으로 시작되지 않은 경우가 있다. 
이 경우 필히 기존 버전을 언인스톨한 후 설치해야 하며, 언인스톨 시 conf, databases는 백업을 권장한다.
 
Q8: CUBRID 매니저로 서버 접속 시 연결 시간 초과로 접속할 수 없다는 에러가 발생할 때
===================================================================================

아래 문서에 따라 리눅스의 방화벽 설정이 INPUT, OUTPUT 모두 설정되어 있는지 확인한다.

*   `http://www.cubrid.com/zbxe/bbs_developer_qa/62871 <CUBRID 사용 포트와 iptables(방화벽) 설정>`

*   `http://www.cubrid.org/port_iptables_configuration <CUBRID Port and iptables (firewall) Configuration>`
 
Q9: CUBRID 매니저(또는 쿼리 브라우저)의 UI에 출력되는 언어를 변경하고 싶을 때
=============================================================================

CUBRID 매니저 설치 폴더에 있는 cubridmanager.ini를 텍스트 편집기로 "-nl"과 "en_US" 부분을 추가해 넣은 후 재시작하면 다음부터는 영문으로 시작할 수 있다.

::

    -startup
    plugins/org.eclipse.equinox.launcher_1.1.0.v20100507.jar
    --launcher.library
    plugins/org.eclipse.equinox.launcher.win32.win32.x86_64_1.1.0.v20100503
    -nl
    en_US
    -vmargs
    -Xms40m
    -Xmx512m

Q10: CUBRID 매니저(또는 쿼리 브라우저)에서 데이터 크기가 커서 조회/로딩에 실패할 때
===================================================================================

Java의 힙 메모리 부족으로 인해 발생하는 현상이다. 이 경우, 로컬 시스템에 따라 특정 사이즈를 초과하는 경우에만 조회/입력 등의 작업을 실패하게 된다. Migration Toolkit도 동일하다.

CUBRID 매니저 설치 폴더에 있는 cubridmanager.ini를 텍스트 편집기로 제일 마지막 줄의 값을 "-Xmx1024m"(Java 힙 메모리 최대값)으로 변경하고 큐브리드 매니저를 재시작하면 된다. 기본은 512MB인데, 이를 로컬 작업 환경에 맞게 증가시켜주면 된다. (예: 1024MB)

::

    -startup
    plugins/org.eclipse.equinox.launcher_1.1.0.v20100507.jar
    --launcher.library
    plugins/org.eclipse.equinox.launcher.win32.win32.x86_64_1.1.0.v20100503
    -nl
    en_US
    -vmargs
    -Xms40m
    -Xmx1024m

Q11: CUBRID 매니저 admin 접속 후 장시간이 지나면 관리 메뉴가 동작하지 않을 때
=============================================================================

CUBRID 매니저의 admin 접속은 매니저 서버와 접속을 유지하는 것인데, 장시간이 지나면 세션이 타임아웃되는 문제 등으로 인해 관리 메뉴를 사용하지 못할 수 있다. 이러한 경우 다음과 같이 조치한다.

*   호스트 접속을 끊었다가 다시 접속한다.

*   그래도 정상 동작하지 않는 경우, CUBRID 데이터베이스가 설치된 장비에서 매니저 서버의 구동을 종료하고 다시 시작한다.

    ::
    
        $ cubrid manager stop
        $ cubrid manager start

Q12: 데이터베이스 정보가 호스트 탐색 창에서 안 보일 때
======================================================

해당 호스트의 $CUBRID/databases/databases.txt 파일 내에 호스트 이름이 잘못 기록되어 있을 수 있다.

*   /etc/hosts 파일에 호스트 이름이 정상적으로 기록되어 있는지 확인한다.
*   $CUBRID/databases/databases.txt 파일의 호스트 이름이 /etc/hosts 파일의 호스트 이름에 포함되어 있는지 확인한다.

다음은 HA 환경을 구성한 databases.txt 파일의 예이다.

::

    #db-name        vol-path                                db-host                 log-path                                lob-base-path
    testdb          /home/cubrid/CUBRID/databases/testdb   test001.cub:test002.cub /home/cubrid/CUBRID/databases/testdb    file:/home/cubrid/CUBRID/databases/testdb/lob
