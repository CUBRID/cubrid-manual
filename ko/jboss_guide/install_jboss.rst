

.. _install_jboss:

***********
JBoss 설치
***********

앞서 설명한 대로 JBoss는 WildFly 17버전을 기준으로 설치를 진행한다.

본문에 들어가기에 앞서 JBoss는 EAP 5 버전 이후부터 두 가지 서버 구동 모드(standalone, domain)가 있다. 

standalone모드는 하나의 서버에 하나의 관리 프로세스가 생성되어 서버를 운영하는 방식이다. 이 방식은 서버가 많아질수록 관리사항이 같이 늘기 때문에 관리가 까다로워지지만, 서버를 각각 따로 설정, 제어가 가능하고 대규모 환경에서의 배포도 편리하다.

domain모드는 관리자가 하나의 관리 프로세스를 통해 여러 서버를 동시에 제어할 수 있다. 하나를 통해 N개의 서버 관리가 가능하기 때문에 관리가 용이하지만, 그만큼 중앙서버 구성의 어려움과 각각의 서버에 대한 설정, 제어가 어려워진다는 단점이 있다.

이 장에서는 standalone모드를 사용하여 서버를 설치, 시작, 종료하는 과정을 설명한다.

JBoss 설치, 구동
----------------------

Linux
^^^^^^^

#. WildFly 17버전 파일을 다운받는다. ::

    [wildfly@localhost]$ wget https://download.jboss.org/wildfly/17.0.0.Final/wildfly-17.0.0.Final.zip

#. 다운받은 파일의 압축을 해제한다. ::

    [wildfly@localhost]$ unzip wildfly-17.0.1.Final.zip

#. [UNZIP_FILE_ROOT]/bin 으로 이동하여 add-user.sh를 실행한다

    [wildfly@localhost]$ cd [UNZIP_FILE_ROOT]/bin
    [wildfly@localhost]$ sh add-user.sh

#. Management User를 선택한다.

#. 등록할 관리자의 이름과 패스워드를 입력한다.

#. 해당 사용자가 속할 그룹을 선택한다. 없다면 공백인 상태로 <Enter>를 눌러 진행한다

#. 등록이 완료되었다면 standalone.sh를 실행하여 서버가 구동되는지 확인한다. ::
    
    ...
    [org.jboss.as] (Controller Boot Thread) WFLYSRV0025 WildFly Full 17.0.1.Final (WildFly Core 6.0.2.Final) started in 8727ms - Started 306 of 527 services ...

위와 같은 문구가 마지막 줄에 출력되었다면 서버가 제대로 가동된 것이다. 웹 브라우저에서 http://[server_ip]:8080 로 접속하여 확인한다.

Windows
^^^^^^^^

#. WildFly 17버전 파일을 다운받는다. ::

    https://www.wildfly.org/downloads/

#. 원하는 경로에 압축을 해제한다.

#. $[JBOSS_HOME]\\bin의 add-user.bat을 실행한다.

#. Management User를 선택한다.

#. 등록할 관리자의 이름과 패스워드를 입력한다.

#. 해당 사용자가 속할 그룹을 선택한다. 없다면 공백인 상태로 <Enter>를 눌러 진행한다.

#. 등록이 완료되었다면 standalone.bat을 실행하여 서버가 구동되는지 확인한다. ::

    ...
    [org.jboss.as] (Controller Boot Thread) WFLYSRV0025: WildFly Full 17.0.0.Final (WildFly Core 6.0.2.Final) started in 6506ms - Started 306 of 527 services ...

위와 비슷한 메시지가 출력되었다면 http://127.0.0.1:8080 으로 접속하여 서버가 성공적으로 기동 되었는지 확인한다.

Linux와 Windows 공통으로 로컬 환경에서 웹 브라우저를 통해 접속하는 게 아닌 외부에서 브라우저를 통해 접근, 확인해야 하는 환경이라면 다음 절차를 추가로 시행한다.

외부 브라우저에서 접속
-----------------------

#. $[JBOSS_HOME]/standalone/configuration/의 standalone.xml을 편집기로 실행한다.

#. interface라고 적힌 부분을 찾아 아래와 같이 변경한다. ::

    <interfaces>
        <interface name="management">
            <inet-address value="${jboss.bind.address.management:0.0.0.0}"/>
        </interface>
        <interface name="public">
            <any-address/>
        </interface>
    </interfaces>

#. 외부에서 브라우저를 통해 http://[server_ip]:8080 으로 접속하여 확인한다.

JBoss 종료
-----------

종료 방법은 운영체제에 상관없이 비슷하다.

#. 서버 실행 콘솔 화면에서 <Ctrl> + <c> 로 종료한다

#. jboss-cli.sh (Windows의 경우 jboss-cli.bat)을 실행하고 명령어를 다음과 같이 입력한다. ::

    [disconnected /] connect
    [standalone@localhost:9990] shutdown
    [disconnected /] exit

    