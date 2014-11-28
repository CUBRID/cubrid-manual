****
설치
****

다운로드
========

CUBRID 매니저는 Java 실행 환경에서만 실행이 가능하기 때문에 우선 JRE (Java Runtime Environment)를 설치해야 한다. JRE는 아래의 URL에서 받을 수 있다.

*   JRE 다운로드 URL: http://www.oracle.com/technetwork/java/javase/downloads/index.html

    .. note::
    
        *   Java SE 7(2014년 9월 현재 기준)에 해당하는 JRE를 받아 설치한다.
        *   64bit용 CUBRID 매니저를 사용하려면 JRE 역시 64bit용으로 설치되어야 한다.그러나 64비트용 CUBRID를 사용한다고 해서 CUBRID 매니저도 64bit용으로 설치할 필요는 없다.
        *   간혹 시스템은 64bit이면서 32bit JRE를 설치해서 사용하는 경우가 있는데, 이러한 경우 CUBRID 매니저는 64bit를 설치한 후 CUBRID 매니저가 설치된 경로 아래에 64bit JRE를 jre 경로명으로 복사해 넣으면 시스템에 설치된 JRE의 영향을 받지 않고 CUBRID 매니저를 실행할 수 있다.

CUBRID 매니저는 아래의 URL에서 받을 수 있다. 

*   CUBRID 매니저 다운로드 URL: http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Manager/

설치
====

*   Windows: 32/64bit OS에 해당하는 exe 파일을 다운로드 후 실행하여 설치한다.
*   Linux: 32/64bit OS에 해당하는 tar.gz 파일을 다운로드하고 압축을 해제하여 cubridmanager를 실행한다.
*   Max: tar.gz 파일을 다운로드 하고 압축을 해제하여 cubridmanager.app을 실행한다.

.. _upgrade-cm:

업그레이드
==========

CUBRID 매니저는 자동 업데이트 기능을 내장하고 있어서 신규 버전이 등록되면 CUBRID 매니저 실행 시 사용자를 확인한 후 자동으로 다운로드하고 업그레이드를 수행한다.

자동 업데이트를 사용하지 않고 최신 설치 파일을 다운로드하여 설치도 가능하나 방법은 Windows와 Linux/Mac이 서로 조금씩 다르다.

*   Windows: "시작 > CUBRID > Uninstall CUBRID Manager"를 선택하여 설치를 해제한 후 다시 설치한다.
*   Linux/Mac: 삭제하지 않고 sh로 설치하거나 tar.gz를 압축 해제하여 덮어쓰기한다.

수동으로 업그레이드할 경우 호스트 연결 정보 등이 포함된 설정 폴더를 백업하는 것이 좋다. 

.. note:: **백업 방법**

    CUBRID 매니저가 설치된 디렉토리에는 workspace 폴더가 있는데, 이를 다른 곳으로 백업해두면 된다. 단, "파일 > 워크스페이스 전환"으로 이미 다른 곳으로 워크스페이스를 변경하였다면 변경한 곳의 폴더를 복사해두는 것이 안전하다.

삭제
====

CUBRID 매니저를 삭제하려면 :ref:`upgrade-cm`\ 에서 설명한 것처럼 동일한 방식으로 삭제한다. 호스트 정보까지 모두 삭제하려면 CUBRID 매니저를 Uninstall을 이용하여 삭제한 후 CUBRID 매니저 설치 폴더 (Windows에서는 보통 C:\\CUBRID\\cubridmanager)를 완전히 삭제한다.

Manager Server 패치
===================

CUBRID 매니저 서버는 CUBRID 데이터베이스 설치 시 함께 설치된다. 그러나, CUBRID의 다음 버전이 릴리스되기 전에 CUBRID 매니저, CUBRID 마이그레이션 툴킷 또는 CUBRID 웹 매니저 릴리스와 함께 CUBRID 매니저 서버만 별도로 단독 릴리스될 수 있다. CUBRID 매니저, CUBRID 마이그레이션 툴킷, CUBRID 웹 매니저 릴리스 노트에 CUBRID 매니저 서버의 패치를 요구하는 내용이 명시되어 있을 경우 필히 패치를 받아서 설치하여야 정상적인 기능을 지원받을 수 있다.

참고로, 패치 파일은 아래 경로에서 받을 수 있으며, 현재 운영 중인 CUBRID 데이터베이스 버전에 맞는 서버를 선택하면 된다.

::

    http://ftp.cubrid.org/CUBRID_Tools/CUBRID_Manager_Server

**CUBRID 매니저 서버 패치 절차**

1.  CUBRID 매니저 서버가 현재 실행 중인지 확인한다.

    ::
    
        cubrid service status
        
2.  만약 실행 중이면 CUBRID 매니저 서버를 중지한다.

    ::
    
        cubrid manager stop

3.  패치를 위해 받은 CUBRID 매니저 서버 압축파일을 압축해제한다.

4.  cub_auto, cub_js, cub_job, cub_ftproc를 $CUBRID/bin에 복사한다. 이미 있을 것이므로 겹쳐쓰기를 한다. Windows에서는 cub_auto.exe, cub_js.exe, cub_job.exe, cub_ftproc.exe 이다.

5.  패치가 완료되었으므로 CUBRID 매니저 서버를 실행한다.

    ::
    
        cubrid manager start
        
