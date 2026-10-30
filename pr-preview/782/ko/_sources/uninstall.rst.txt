
:meta-keywords: uninstall cubrid linux, uninstall cubrid windows.
:meta-description: Uninstalling CUBRID in Linux and Windows.

.. _uninstall:

CUBRID 제거
===========

Linux에서 CUBRID 제거
---------------------

**SH 패키지 또는 압축 패키지의 CUBRID 제거**

SH 패키지(설치 패키지의 파일 확장자 명이 .sh) 또는 압축 패키지(설치 패키지의 파일 확장자 명이 .tar.gz)를 제거하려면 다음의 절차대로 수행한다.

*   서비스 종료 후 데이터베이스 제거

    CUBRID 서비스를 종료한 후 기존에 생성된 데이터베이스를 모두 제거한다. 
    
    예를 들어 기존에 생성된 데이터베이스가 testdb1, testdb2라면 다음과 같이 명령을 수행한다.
    
    ::
    
        $ cubrid service stop
        $ cubrid deletedb testdb1
        $ cubrid deletedb testdb2
        
*   CUBRID 엔진 제거

    $CUBRID 디렉터리 이하를 모두 제거한다.
    
    ::
    
        $ echo $CUBRID
            /home1/user1/CUBRID
            
        $ rm -rf /home1/user1/CUBRID

*   자동 구동 스크립트 제거

    /etc/init.d 디렉터리 이하에 cubrid 스크립트를 저장했다면 이 파일을 제거한다.

    ::
    
        cd /etc/init.d
        rm cubrid

**RPM 패키지의 CUBRID 제거**

RPM 패키지로 설치했다면 rpm 명령을 통해 제거가 가능하다.
    
*   서비스 종료 후 데이터베이스 제거

    CUBRID 서비스를 종료한 후 기존에 생성된 데이터베이스를 모두 제거한다. 
    
    예를 들어 기존에 생성된 데이터베이스가 testdb1, testdb2라면 다음과 같이 명령을 수행한다.
    
    ::
    
        $ cubrid service stop
        $ cubrid deletedb testdb1
        $ cubrid deletedb testdb2

*   CUBRID 엔진 제거

    ::
    
        $ rpm -q cubrid
        cubrid-10.1.0.7663-1ca0ab8-Linux.x86_64

        $ rpm -e cubrid
        warning: /opt/cubrid/conf/cubrid.conf saved as /opt/cubrid/conf/cubrid.conf.rpmsave
        
*   자동 구동 스크립트 제거

    /etc/init.d 디렉터리 이하에 cubrid 스크립트를 저장했다면 이 파일을 제거한다.

    ::
    
        cd /etc/init.d
        rm cubrid

Windows에서 CUBRID 제거
-----------------------

"제어판 > 프로그램 제거"에서 CUBRID를 선택하여 제거한다.
