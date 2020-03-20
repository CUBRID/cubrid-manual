
:meta-keywords: cubrid administrator, cubrid dba, cubrid tools, cubrid utilities, cubrid services, cubrid processes, cubrid config, cubrid troubleshoot
:meta-description: This manual chapter covers everything needed by database administrators how to use cubrid utilities, how to monitor cubrid systems, how to troubleshoot and how to fix problems.

***********
CUBRID 운영
***********

이 장에서는 데이터베이스 관리자(**DBA**)가 CUBRID 시스템을 사용하는데 필요한 작업 방법을 설명한다.

*   CUBRID 서버, 브로커 및 매니저 서버 등의 다양한 프로세스들을 구동하고 정지하는 방법을 설명한다. :doc:`/admin/control`\를 참조한다.

*   **cubrid** 유틸리티를 통해 데이터베이스 생성 및 삭제, 볼륨 추가와 같은 데이터베이스 관리 작업, 데이터베이스를 다른 곳으로 이동하거나 시스템 버전에 맞춰서 변경하는 마이그레이션 작업, 장애 대비를 위한 데이터베이스의 백업 및 복구 작업 등에 대해 설명한다. :doc:`/admin/admin_utils`\를 참조한다.

*   시스템 설정 방법에 대해 설명한다. :doc:`/admin/config` 을 참조한다.

*   실행 중인 프로세스를 동적으로 모니터링하고 추적할 수 있는 SystemTap 사용 방법에 대해 설명한다. :doc:`/admin/systemtap`\을 참조한다.
   
*   트러블슈팅 방법에 대해 설명한다. :doc:`/admin/troubleshoot`\을 참조한다.

**cubrid** 유틸리티는 CUBRID 서비스를 통합 관리할 수 있는 기능을 제공하며, CUBRID 서비스 프로세스를 관리하는 서비스 관리 유틸리티와 데이터베이스를 관리하는 데이터베이스 관리 유틸리티로 구분된다.

서비스 관리 유틸리티는 다음과 같다.

*   서비스 유틸리티: 마스터 프로세스를 구동 및 관리한다.

    *   :ref:`cubrid service <control-cubrid-services>`

*   서버 유틸리티: 서버 프로세스를 구동 및 관리한다.

    *   :ref:`cubrid server <control-cubrid-server>`

*   브로커 유틸리티: 브로커 프로세스 및 응용서버(CAS) 프로세스를 구동 및 관리한다.

    *   :ref:`cubrid broker <broker>`

*   매니저 유틸리티: 매니저 서버 프로세스를 구동 및 관리한다.

    *   :ref:`cubrid manager <cubrid-manager-server>`

*   HA 유틸리티: HA 관련 프로세스를 구동 및 관리한다.

    *   :ref:`cubrid heartbeat <cubrid-heartbeat>`

자세한 설명은 :ref:`control-cubrid-processes` 절을 참조한다.

데이터베이스 관리 유틸리티는 다음과 같다.

*   데이터베이스 생성, 볼륨 추가, 삭제 

    *   :ref:`cubrid createdb <createdb>`
    *   :ref:`cubrid addvoldb <addvoldb>`
    *   :ref:`cubrid deletedb <deletedb>`

*   데이터베이스 이름 변경, 호스트 변경, 복사/이동, 등록 

    *   :ref:`cubrid renamedb <renamedb>`
    *   :ref:`cubrid alterdbhost <alterdbhost>`
    *   :ref:`cubrid copydb <copydb>`
    *   :ref:`cubrid installdb <installdb>`

*   데이터베이스 백업

    *   :ref:`cubrid backupdb <backupdb>`

*   데이터베이스 복구

    *   :ref:`cubrid restoredb <restoredb>`

*   내보내기와 가져오기

    *   :ref:`cubrid unloaddb <unloaddb>`
    *   :ref:`cubrid loaddb <loaddb>`
    
*   데이터베이스 공간 확인, 공간 정리 

    *   :ref:`cubrid spacedb <spacedb>`
    *   :ref:`cubrid compactdb <compactdb>`

*   통계 정보 갱신, 질의 계획 확인 

    *   :ref:`cubrid plandump <plandump>`
    *   :ref:`cubrid optimizedb <optimizedb>`
    *   :ref:`cubrid statdump <statdump>`

*   잠금 확인, 트랜잭션 확인, 트랜잭션 제거 

    *   :ref:`cubrid lockdb <lockdb>`
    *   :ref:`cubrid tranlist <tranlist>`
    *   :ref:`cubrid killtran <killtran>`

*   데이터베이스 진단/파라미터 출력 

    *   :ref:`cubrid checkdb <checkdb>`
    *   :ref:`cubrid diagdb <diagdb>`
    *   :ref:`cubrid paramdump <paramdump>`

*   HA 모드 변경,로그 복제/반영

    *   :ref:`cubrid changemode <cubrid-changemode>`
    *   :ref:`cubrid applyinfo <cubrid-applyinfo>`

*   로캘 컴파일/출력

    *   :ref:`cubrid genlocale <locale-command>`
    *   :ref:`cubrid dumplocale <dumplocale>`

자세한 설명은 :ref:`cubrid-utilities` 를 참조한다.

.. _utility-on-windows:

.. note::

    Windows Vista 이상 버전에서 **cubrid** 유틸리티를 사용하여 서비스를 제어하려면 명령 프롬프트 창을 관리자 권한으로 실행하여 사용하는 것을 권장한다. 명령 프롬프트 창을 관리자 권한으로 구동하지 않고 **cubrid** 유틸리티를 사용하면 UAC(User Account Control) 대화 상자를 통하여 관리자 권한으로 수행할 수는 있으나 수행 결과 메시지를 확인할 수 없다.

    Windows Vista 이상 버전에서 명령 프롬프트 창을 관리자 권한으로 실행하려면 [시작] > [모든 프로그램] > [보조 프로그램] > [명령 프롬프트]를 마우스 오른쪽 버튼으로 클릭하여 [관리자 권한으로 실행]을 선택한다. 권한 상승을 확인하는 대화 상자가 나타났을 때 [예]를 클릭하면 명령 프롬프트가 관리자 권한으로 실행된다.

.. toctree::
    :maxdepth: 3

    control.rst
    db_manage.rst
    admin_utils.rst
    config.rst
    systemtap.rst
    troubleshoot.rst
