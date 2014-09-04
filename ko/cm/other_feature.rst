*********
기타 기능
*********

DB 데이터 비교 마법사
=====================

HA 구성된 데이터베이스의 마이그레이션 및 복제후 데이터 정합성을 비교할 수 있습니다. Primary Key가 있는 테이블만 비교 대상이 됩니다.

.. image:: /images/cm-data-compare.png

ERwin XML 가져오기/내보내기
===========================

ERwin의 XML 파일로 CUBRID 스키마를 내보내거나 ERwin의 XML 파일로부터 CUBRID 스키마와 비교하고 DDL을 출력한다.

ERwin에서 XML 파일은 File > Save As를 이용하여 저장할 수 있다. ERwin은 4.1 버전을 지원한다.

.. image:: /images/cm-erwin.png


호스트 그룹 관리
================

관리할 호스트가 많을 때에는 호스트 그룹으로 묶어서 아래 그림과 같이 보기 쉽게 관리할 수 있다.

.. image:: /images/cm-host-manage.png

호스트 그룹 보기/숨기기
-----------------------

사용자는 다음 메뉴를 사용하여 호스트 그룹을 보거나 숨길 수 있다.
 
.. image:: /images/cm-show-hostgroup.png

*   아이템: 호스트만 표시하고 그룹 노드는 숨긴다.
*   그룹: 그룹 노드를 표시한다.

호스트 그룹 설정
-----------------------

.. image:: /images/cm-set-hostgroup.png
 
메뉴에서 "그룹 설정"을 선택하면 다음과 같은 "그룹 설정" 대화상자가 나타난다.
 
.. image:: /images/cm-set-hostgroup-dlgbox.png  
 
"맨위", "위로", "아래로", "맨아래" 버튼을 클릭하여 그룹의 순서를 변경할 수 있다. "추가", "편집" 버튼을 클릭하면 그룹을 추가하거나 편집할 수 있는 대화상자가 나타난다.
 

호스트 그룹 추가/편집
-----------------------

"그룹 설정" 대화상자에서 "추가"를 클릭하면 새로운 그룹의 이름과 그 그룹에 속할 호스트를 입력할 수 있다.

대화 상자의 왼쪽에 해당 호스트 그룹에 속하지 않은 호스트 목록이 보이고, 오른쪽에는 해당 호스트 그룹에 속한 호스트 목록이 보인다. ">" 버튼을 클릭하면 왼쪽 목록에서 선택한 호스트가 오른쪽으로 이동하고, ">>" 버튼을 클릭하면 왼쪽 목록의 모든 호스트가 오른쪽으로 이동한다. "<" 버튼을 클릭하면 오른쪽 목록에서 선택한 호스트가 왼쪽으로 이동하고, "<<" 버튼을 클릭하면 오른쪽 목록의 모든 호스트가 왼쪽으로 이동한다.
 
호스트 그룹 삭제
-----------------------

"그룹 설정" 대화상자에서 삭제할 호스트 그룹을 선택하고 "삭제" 버튼을 클릭한다. "Default Group"은 삭제하거나 편집할 수 없으며, 아무 그룹에도 속하지 않는 호스트를 포함한다.
 

끌어다 놓기
-----------------------

여러 개의 호스트를 끌어다 다른 호스트 그룹으로 옮기거나 현재 그룹 내에서 호스트 노드의 표시 순서를 변경할 수 있다. 그룹 노드를 숨겼을 때에도 호스트 노드의 표시 순서를 변경할 수 있다.

다중 호스트 환경에서의 백그라운드 운영
======================================

"호스트 추가"를 이용하여 여러 호스트의 데이터베이스를 관리하는 경우, 데이터베이스 운영 작업이 백그라운드에서 실행되도록 설정할 수 있다. 예를 들어, A 호스트의 데이터베이스를 백업하는 도중에 B 호스트의 데이터베이스에 질의를 실행하고자 한다면, 시간이 오래 걸리는 데이터베이스 백업 작업이 백그라운드에서 실행되도록 설정할 수 있다. 다음은 백그라운드에서 실행할 수 있는 데이터베이스 운영 작업이다.

*   데이터베이스 관련: 데이터베이스 생성, 데이터베이스 언로드/로드, 데이터베이스 백업/복구, 데이터베이스 이름 변경, 데이터베이스 복사, 데이터베이스 최적화, 데이터베이스 공간 정리, 데이터베이스 검사

*   테이블 관련: 데이터 올리기/내려 받기, 모든 데이터 삭제, 제약 조건을 NULL에서 NOT NULL로 변경하면서 데이터 업데이트

*   볼륨 관련: 볼륨 추가

OID 탐색기
==========

OID로 데이터를 탐색할 때 사용한다.

해당 데이터베이스 또는 질의 편집기의 결과 창에서 마우스 오른쪽 버튼 클릭하고 "OID 탐색기"를 선택하여 실행할 수 있다. 이 기능은 데이터베이스 서버가 구동 중일 때에만 수행할 수 있으며, 서버가 구동하지 않을 때에는 해당 메뉴가 비활성화된다.


 : 단, REUSE_OID 쓰면 이 기능 사용 불가? => 아닌 거 같다. 실험해 볼 것.REUSE_OID 쓰는 테이블은 다른 테이블을 컬럼 타입으로 쓸 수 없다는 부분만.
 왜냐면 테이블이 drop되고 재생성되면 기존의 테이블 레퍼런스가 다른 엉뚱한 테이블의 인스턴스를 가리킬 수 있으므로.


매니저 로그
===========

CUBRID 매니저 클라이언트를 수행 중에 발생한 오류 로그는 $CUBRID/cubridmanager/logs에 cubridmanager.log라는 파일로 생성된다.

사용자는 CUBRID 매니저 개선 및 오류 분석 시 해당 오류 로그와 현상을 CUBRID 매니저 개발 사이트 이슈로 등록하여 CUBRID 매니저 개선 활동에 참여할 수 있다.

단축키
======

기본 단축키
-----------

=============== =========================
단축키          동작
=============== =========================
Ctrl+T          Open query editor
Ctrl+W          Close the active query editor
Ctrl+Shift+W    Close all query editors
Alt+F           Open file menu
F1              Open help document page
=============== =========================

질의 편집기 단축키
------------------

=============== =========================
단축키          동작
=============== =========================
F5              Run the queries
F6              Show the query plan
F7              Show the query history
F8              Switch to multiple database query
F11             Change query plan display model
Ctrl+Shift+X    Change to upper letters(capitals)
Ctrl+Shift+Y    Change to lower letters
F5/Ctrl+Enter   Run the queries
Ctrl+Shift+F    Format the selected queries
Tab             Indent
Shift+Tab       Outdent
Ctrl+/          Add Comment
Ctrl+/          Delete Comment
Ctrl+Z          Undo
Ctrl+Y          Redo
Ctrl+F          Find/Replace
Ctrl+C          Copy
Ctrl+X          Cut
Ctrl+V          Paste
Ctrl+G          Go to line
=============== =========================

테이블/컬럼 설명 기능 사용
==========================

테이블/컬럼 설명 기능은 UI를 이용하여 테이블을 생성하거나 편집할 때, 테이블/컬럼의 설명을 등록하여 각 테이블과 컬럼의 용도를 UI에서 쉽게 확인하고 관리하기 위해 지원한다.

.. image:: /images/cm-table-column-comment.png

단, 이 기능은 CUBRID에서 기본 지원되는 기능이 아니며, CUBRID Manager, CUBRID Query Browser에서 자체 지원되는 기능이며 사용상 기능 한계가 있을 수 있습니다. 또한, 처음 사용시 설치를 해야 사용할 수 있는 기능이며, DBA 권한이 있을 경우만 생성 가능하다.

DBA 사용자일 경우 아래와 같이 안내후 _cub_schema_comments을 자동 생성합니다. 네비게이션에서는 일반 유저들이 이 테이블을 UI에서 편집하지 못하도록 시스템 테이블 영역에 출력이 됩니다만 실제 시스템 테이블은 아니다.

.. image:: /images/cm-col-comment.png

DBA 권한이 없는 유저의 경우 아래와 같은 오류가 발생한다.

.. image:: /images/cm-col-comment2.png
 
설치 후 아래와 같이 설명 편집과 조회가 가능하다. 

.. image:: /images/cm-table-column-comment2.png

 
테이블 설명과 컬럼 설명을 확인 가능하며 컬럼 설명은 아래의 컬럼 편집 UI 에서 편집 가능하다. 

.. image:: /images/cm-col-comment3.png

HA 마법사
=========

본 기능은 대상 호스트가 Linux인 경우에만 사용할 수 있습니다. Linux에서만 CUBRID HA 구성이 가능하기 때문입니다.  

선행 작업:  

1)  Linux 서버 2대를 확보하고, 각각 CUBRID 2008 R2.2 이상의 버전이 설치되어 있어야 합니다. 

2)  각각의 서버에서 동일한 이름의 DB를 생성하세요. 

3)  CUBRID Manager에서 각 마스터/슬레이브 DB에 로그인합니다. 

4)  마스터 호스트 이름에 마우스를 대고 우클릭> HA 설정 마법사를 선택하십시오. 

5)  왼쪽 마스터 설정 영역에서 마스터의 호스트이름을 적고, 하위 목록에서 마스터 DB를 선택합니다.  오른쪽 슬레이브 설정 영역에서 슬레이브 서버 선택, 호스트 이름 및 슬레이브 DB를 선택합니다.  

    .. image:: /images/cm-ha-1.jpg

6)  cubrid.conf 파라미터를 설정합니다.

    .. image:: /images/cm-ha-2.jpg

7)  cubrid_ha.conf 파라미터를 설정합니다.

    .. image:: /images/cm-ha-3.jpg

8)  파라미터 설정을 확인합니다.

    .. image:: /images/cm-ha-4.jpg

9)  HA 구성 적용을 위한 가이드를 실행합니다.

    Step1,2)에서 출력된 대로 각 호스트에서  /etc/hosts 파일을 수정합니다. 
    Step3) HA 서비스를 구동합니다.

    .. image:: /images/cm-ha-7.jpg

10) Add host info to /etc/hosts files both servers. The content is "host address" or "host name"

    (e.g. 10.34.64.149NC-PL-DEV001). (If host address is "localhost" or "127.0.0.1", please replace it by real address). 

    Stop current service use command "cubrid service stop". 

    Start service use command "cubrid service start". 

    Start heart beat use command "cubrid heartbeat start".
    
HA 프로세스 상태 확인
=====================

Check HA server status use command “cubrid hb status”.

.. image:: /images/cm-ha-8.jpg

As shown in illustration: The master server is “NC-VM-PL-QA-34” and “NC-PL-DEV001” is slave server. 

So far, the simple HA environment configure finished.  

For more detail,  Please see :doc:`/ha`.

오브젝트 대시보드
=================

테이블, 뷰, 시리얼, 트리거, 작업 자동화의 목록을 보면서 추가/편집을 할 수 있는 오브젝트 대시보드를 제공한다.

왼쪽 네비게이션 트리에서 테이블/뷰/시리얼/트리거/작업 자동화 노드를 두번클릭하면 사용할 수 있다.

.. image:: /images/cm-obj-dashboard1.png

.. image:: /images/cm-obj-dashboard2.png

빠른 질의 입력
==============

질의 편집기에서 마우스 없이도 쉽게 질의를 입력할 수 있는 방법을 제공한다.

CTRL + , 를 누르면 빠른 질의 입력 창이 뜨고 테이블명을 입력(자동 완성)한 후,
SELECT, INSERT, UPDATE 문을 바로 질의 편집기에 입력할 수 있다.

.. image:: /images/cm-quick-query.png

다중 호스트 설정 편집기
=======================

복수개의 호스트 설정 (cubrid.conf, broker.conf)을 동시에 비교하면서 편집할 수 있다.

.. image:: /images/cm-multihost.png

