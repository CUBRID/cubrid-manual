*********
CUBRID HA
*********

High Availability(HA)란, 하드웨어, 소프트웨어, 네트워크 등에 장애가 발생해도 지속적인 서비스를 제공하는 기능이다. 이 기능은 하루 24시간 1년 내내 서비스를 제공해야 하는 네트워킹 컴퓨팅 부분에서 필수적인 요소이다. HA 시스템은 두 대 이상의 서버 시스템으로 구성하여 시스템 구성 요소 중의 한 요소에 장애가 발생해 서비스를 중단 없이 제공할 수 있다.

High Availability 기능을 CUBRID에 적용한 것이 CUBRID HA 기능이다. CUBRID HA 기능은 여러 서버 시스템에서 데이터베이스를 항상 동기화된 상태로 유지하여 서비스를 제공한다. 서비스를 수행 중인 시스템에 예상치 못한 장애가 발생하면 자동으로 다른 시스템이 서비스를 수행하도록 하여 서비스 중단 시간을 최소화한다.

CUBRID의 HA 기능은 shared-nothing 구조이며, 액티브 서버(active server)에서 스탠바이 서버(standby server)로 데이터를 동기화하기 위해 다음 두 단계를 수행한다.

#.  트랜잭션 로그 다중화: 액티브 서버에서 생성되는 트랜잭션 로그를 실시간으로 다른 노드에 복제한다.
#.  트랜잭션 로그 반영: 실시간으로 복제된 트랜잭션 로그를 분석하여 스탠바이 서버에 데이터를 반영한다.

CUBRID HA 기능은 위 두 단계를 수행하여 액티브 서버와 스탠바이 서버에 항상 동기화된 데이터를 유지한다. 따라서, 서비스를 제공 중이던 마스터 노드(master node)에 예상치 못한 장애가 발생하여 액티브 서버가 정상적으로 동작하지 못하면 슬레이브 노드(slave node)의 스탠바이 서버가 액티브 서버를 대신하여 중단 없는 서비스를 제공할 수 있다. CUBRID HA 기능은 시스템과 CUBRID의 상태를 실시간으로 감시하고 장애가 발생하면 자동 failover를 수행하기 위해 heartbeat 메시지를 사용한다.

.. image:: /images/image13.png

CUBRID HA 기본 개념
===================

노드와 그룹
-----------

노드는 CUBRID HA를 구성하는 논리적인 단위로, 노드는 상태에 따라 마스터 노드(master node), 슬레이브 노드(slave node), 레플리카 노드(replica node) 등으로 나눈다.

*   **마스터 노드** : 복제의 대상이 되는 노드로, 액티브 서버를 사용한 읽기, 쓰기 등 모든 서비스를 제공한다.

*   **슬레이브 노드**: 마스터 노드와 동일한 내용을 갖는 노드로, 마스터 노드의 변경이 자동으로 반영된다. 스탠바이 서버를 사용한 읽기 서비스를 제공하며 마스터 노드 장애 시 failover가 일어난다.

*   **레플리카 노드** : 마스터 노드와 동일한 내용을 갖는 노드로, 마스터 노드의 변경이 자동으로 반영된다. 스탠바이 서버를 사용한 읽기 서비스를 제공하며 마스터 노드 장애 시 failover가 일어나지 않는다.

CUBRID HA 그룹은 위와 같은 노드들로 이루어지며, 그룹의 멤버는 **cubrid_ha.conf** 의 **ha_node_list** 및 **ha_replica_list** 로 설정할 수 있다. 그룹 내의 노드들은 동일한 내용을 가지며, 주기적으로 상태 확인 메시지를 주고 받고 마스터 노드에 장애가 발생하면 failover가 일어난다.

노드에는 마스터 프로세스(cub_master), 데이터베이스 서버 프로세스(cub_server), 복제 로그 복사 프로세스(copylogdb) 및 복제 로그 반영 프로세스(applylogdb) 등이 포함된다.

.. image:: /images/image14.png

프로세스
--------

CUBRID HA 노드는 하나의 마스터 프로세스(cub_master), 하나 이상의 데이터베이스 서버 프로세스(cub_server), 하나 이상의 복제 로그 복사 프로세스(copylogdb), 하나 이상의 복제 로그 반영 프로세스(applylogdb)로 이루어져 있다. 하나의 데이터베이스를 설정하면 데이터베이스 서버 프로세스, 복제 로그 복사 프로세스, 복제 로그 반영 프로세스가 구동된다. 복제 로그의 복사와 반영은 서로 다른 프로세스에 의해 수행되므로 복제 반영의 지연은 실행 중인 트랜잭션에 영향을 주지 않는다.

*   **마스터 프로세스(cub_master)** : heartbeat 메시지를 주고 받으며 CUBRID HA 내부 관리 프로세스들을 제어한다.

*   **데이터베이스 서버 프로세스(cub_server)** : 사용자에게 읽기, 쓰기 등의 서비스를 제공한다. 자세한 내용은 :ref:`ha-server` 를 참고한다.

*   **복제 로그 복사 프로세스(copylogdb)** : 그룹 내의 모든 트랜잭션 로그를 복사한다. 복제 로그 복사 프로세스가 대상 노드의 데이터베이스 서버 프로세스에 트랜잭션 로그를 요청하면, 해당 데이터베이스 서버 프로세스는 적절한 로그를 전달한다. 트랜잭션 로그가 복사되는 위치는 **cubrid-ha** 의 **REPL_LOG_HOME** 으로 설정할 수 있다. 복사된 복제 로그의 정보는 :ref:`cubrid-applyinfo` 유틸리티로 확인할 수 있다. 복제 로그 복사는 SYNC, SEMISYNC, ASYNC의 세 가지 모드가 있으며, 모드는 **cubrid-ha** 의 **LW_SYNC_MODE** 로 설정할 수 있다. 모드에 대한 자세한 내용은 :ref:`log-multiplexing` 를 참고한다.

.. image:: /images/image15.png

*   **복제 로그 반영 프로세스(applylogdb)** : 복제 로그 복사 프로세스에 의해 복사된 로그를 노드에 반영한다. 반영한 복제 정보는 내부 카탈로그(db_ha_apply_info)에 저장하며, 이 정보는 :ref:`cubrid-applyinfo` 유틸리티로 확인할 수 있다.

.. image:: /images/image16.png

.. _ha-server:

서버
----

서버란 데이터베이스 서버 프로세스를 논리적으로 표현하는 단어로, 상태에 따라 액티브 서버(active server), 스탠바이 서버(standby server)로 나눈다.

*   **액티브 서버**  : 마스터 노드에 속하는 서버로, active 상태이다. 액티브 서버는 사용자에게 읽기, 쓰기 등 모든 서비스를 제공한다.
*   **스탠바이 서버** : 마스터 노드 외의 노드에 속하는 서버로, standby 상태이다. 스탠바이 서버는 사용자에게 읽기 서비스만을 제공한다.

서버 상태는 노드 상태에 따라 변경된다. :ref:`cubrid-changemode` 유틸리티를 이용하면 서버 상태를 조회할 수 있다. maintenance 모드는 운영 편의를 위한 것으로, **cubrid changemode** 유틸리티를 통해 변경할 수 있다.

.. image:: /images/image17.png

*   **active** : 일반적으로 마스터 노드에서 실행 중인 서버들은 active 상태이다. 읽기, 쓰기 등 모든 서비스를 제공한다.

*   **standby** : 슬레이브 노드 또는 레플리카 노드에서 실행 중인 서버들은 standby 상태이다. 읽기 서비스만을 제공한다.

*   **maintenance** : 운영 편의를 위해 수동으로 변경 가능한 상태로, 로컬 호스트의 csql만 접속할 수 있으며, 사용자에게는 서비스를 제공할 수 없다.

*   **to-be-active** : 스탠바이 서버가 failover 등의 이유로 인해 액티브 서버가 되기 전의 상태이다. 기존의 마스터 노드로부터 받은 트랜잭션 로그를 자신의 서버에 반영하는 등 액티브 서버가 되기 위한 준비를 한다. 해당 상태의 노드에는 SELECT 질의만 수행할 수 있다.

*   기타: 내부적으로 사용하는 상태이다.

노드 상태가 변경되면 cub_master 프로세스 로그와 cub_server 프로세스 로그에 각각 다음과 같은 에러 메시지가 저장된다. 단, cubrid.conf의 error_log_level의 값이 error 이하인 경우에 저장된다.

* cub_master 프로세스의 로그 정보는 $CUBRID/log/<hostname>_master.err 파일에 저장되며 다음의 내용이 기록된다. ::

    HA generic: Send changemode request to the server. (state:1[active], args:[cub_server demodb ], pid:25728).
    HA generic: Receive changemode response from the server. (state:1[active], args:[cub_server demodb ], pid:25728).

* cub_server 프로세스의 로그 정보는 $CUBRID/log/server/<db_name>_<date>_<time>.err 파일에 저장되며 다음의 내용이 기록된다. ::

    Server HA mode is changed from 'to-be-active' to 'active'.



heartbeat 메시지
----------------

HA 기능을 제공하기 위한 핵심 구성 요소로, 마스터 노드, 슬레이브 노드, 레플리카 노드가 다른 노드의 상태를 감시하기 위해 주고 받는 메시지이다. 마스터 프로세스는 그룹 내의 모든 마스터 프로세스와 주기적으로 heartbeat 메시지를 주고 받는다. heartbeat 메시지는 **cubrid_ha.conf** 의 **ha_port_id** 파라미터에 설정된 UDP 포트로 주고 받는다. heartbeat 메시지 주기는 내부적으로 설정된 값을 따른다.

마스터 노드의 장애가 감지되면 슬레이브 노드로 failover가 이루어진다.

.. image:: /images/image18.png

failover와 failback
-------------------

failover란, 마스터 노드에 장애가 발생하여 서비스를 제공할 수 없는 상태가 되면 우선순위가 가장 높은 슬레이브 노드가 자동으로 마스터 노드가 되는 것이다. 마스터 프로세스는 수집한 CUBRID HA 그룹 내의 노드들의 정보를 바탕으로 스코어를 계산하여 적절한 시점에 해당 프로세스가 속한 노드의 상태를 마스터 노드로 변경하고, 관리 프로세스에 변경된 상태를 전파한다.

failback은 마스터 노드였던 장애 노드가 복구되면 자동으로 다시 마스터 노드가 되는 것이며, CUBRID HA는 서버의 failback을 지원하지 않는다.

.. image:: /images/image19.png

heartbeat 메시지가 정상적으로 전달되지 않으면 failover가 일어나므로, 네트워크가 불안정한 환경에서는 장애가 발생하지 않아도 failover가 일어날 수 있다. 이와 같은 상황에서 failover가 일어나는 것을 막기 위해 **ha_ping_hosts** 를 설정할 수 있다. **ha_ping_hosts** 를 설정하면, heartbeat 메시지가 정상적으로 전달되지 못했을 때 **ha_ping_hosts** 로 설정한 노드로 ping 메시지를 보내서 원인이 네트워크 불안정인지 확인하는 절차를 거친다. **ha_ping_hosts** 설정에 대한 좀 더 자세한 설명은 :ref:`cubrid-ha-conf` 를 참고한다.

.. _broker-mode:

브로커 모드
-----------

브로커는 서버에 **Read Write**, **Read Only**, **Slave Only**, **Preferred Host Read Only** 네 가지 모드 중 한 가지로 접속할 수 있으며, 사용자가 브로커 모드를 설정할 수 있다.

브로커는 서버 연결 순서에 의해 연결을 시도하여 자신의 모드에 맞는 서버를 선택하여 연결한다. 조건이 맞지 않아 연결되지 않으면 다음 순서의 연결을 시도하고, 모든 순서를 수행해도 적절한 서버를 찾지 못하면 해당 브로커는 서버 연결에 실패한다.

브로커 모드 설정 방법은 :ref:`ha-cubrid-broker-conf` 를 참고한다.

**Read Write**

읽기, 쓰기 서비스를 제공하는 브로커이다. 이 브로커는 일반적으로 액티브 서버에 연결하며, 연결 가능한 액티브 서버가 없으면 스탠바이 서버에 연결한다. 따라서 Read Write 브로커는 일시적으로 스탠바이 서버와 연결될 수 있다.

일시적으로 스탠바이 서버와 연결되면 트랜잭션이 끝날 때마다 스탠바이 서버와 연결을 끊고, 다음 트랜잭션이 시작되면 다시 액티브 서버와 연결을 시도한다. 스탠바이 서버와 연결되면 읽기 서비스만 가능하며, 쓰기 요청에 대해서는 서버에서 오류가 발생한다.

서버 연결 순서는 다음과 같다.

#.   연결되어 있던 서버가 있으면 해당 서버와 연결을 시도하고, 해당 서버의 상태가 active이면 연결 완료
#.   **databases.txt** 에 설정된 호스트에 순차적으로 연결을 시도하여 서버의 상태가 active이면 연결 완료
#.   **databases.txt** 에 설정된 호스트에 순차적으로 연결을 시도하여 최초 연결 가능한 서버와 연결 완료

.. image:: /images/image20.png

**Read Only**

읽기 서비스를 제공하는 브로커이다. 이 브로커는 가능한 스탠바이 서버에 연결하며, 스탠바이 서버가 없으면 액티브 서버에 연결한다. 따라서 Read Only 브로커는 일시적으로 액티브 서버와 연결될 수 있다.

액티브 서버와 연결된 후에는 스탠바이 서버가 있어도 연결은 끊기지 않으며, **cubrid_broker reset** 명령을 실행해야만 기존 연결을 끊고 새롭게 스탠바이 서버에 연결할 수 있다. Read Only 브로커에 쓰기 요청이 전달되면 브로커에서 오류가 발생하므로, 액티브 서버와 연결되어도 읽기 서비스만 가능하다.

서버 연결 순서는 다음과 같다.

#.   연결되어 있던 서버가 있으면 해당 서버와 연결을 시도하고, 해당 서버의 상태가 standby이면 연결 완료
#.   **databases.txt** 에 설정된 호스트에 순차적으로 연결을 시도하여 서버의 상태가 standby이면 연결 완료
#.   **databases.txt** 에 설정된 호스트에 순차적으로 연결을 시도하여 최초 연결 가능한 서버와 연결 완료

.. image:: /images/image21.png

**Slave Only**

읽기 서비스를 제공하는 브로커이다. 이 브로커는 스탠바이 서버에 연결하며, 스탠바이 서버가 없으면 서비스를 제공하지 않는다.

서버 연결 순서는 다음과 같다.

#.   연결되어 있던 서버가 있으면 해당 서버와 연결을 시도하고, 해당 서버의 상태가 standby이면 연결 완료
#.   **databases.txt** 에 설정된 호스트에 순차적으로 연결을 시도하여 서버의 상태가 standby이면 연결 완료

.. image:: /images/image22.png

**Preferred Host Read Only**

읽기 서비스를 제공하는 브로커이다. Read Only 브로커와 동일하고, 서버의 접속 순서 및 서버 선정 기준만 다르다. 서버의 접속 순서 및 서버 선정 기준은 **PREFERRED_HOSTS** 로 설정할 수 있으며, 설정 방법은 :ref:`ha-cubrid-broker-conf` 를 참고한다.

서버 연결 순서는 다음과 같다.

#.   PREFERRED_HOSTS에 설정된 호스트에 순차적으로 연결 시도하여 최초 연결 가능한 서버와 연결 완료
#.   **databases.txt** 에 설정된 호스트에 순차적으로 연결을 시도하여 서버의 상태가 standby이면 연결 완료
#.   **databases.txt** 에 설정된 호스트에 순차적으로 연결을 시도하여 최초 연결 가능한 서버와 연결 완료

.. image:: /images/image23.png

CUBRID HA기능
=============

서버 이중화
-----------

서버 이중화란 CUBRID HA 기능을 제공하기 위해 물리적인 하드웨어 장비를 중복으로 구성하여 시스템을 구축하는 것이다. 이러한 구성을 통해 하나의 장비에 장애가 발생해도 응용 프로그램에서는 지속적인 서비스를 제공할 수 있다.

**서버 failover**

브로커는 서버의 접속 순서를 정의하고 그 순서에 따라 서버에 접속한다. 접속한 서버에 장애가 발생하면 브로커는 다음 순위로 설정된 서버에 접속하며, 응용 프로그램에서는 별도의 처리가 필요 없다. 브로커가 다음 서버에 접속할 때의 동작은 브로커의 모드에 따라 다를 수 있다. 서버의 접속 순서 및 브로커의 모드의 설정 방법은 :ref:`ha-cubrid-broker-conf` 를 참고한다.

.. image:: /images/image24.png

**서버 failback**

CUBRID HA는 자동으로 서버 failback을 지원하지 않는다. 따라서 failback을 수동으로 적용하려면 비정상 종료되었던 마스터 노드를 복구하여 슬레이브 노드로 구동한 후, failover로 인해 슬레이브에서 마스터로 역할이 바뀐 노드를 의도적으로 종료하여 다시 각 노드의 역할을 서로 바꾼다.

예를 들어 *nodeA* 가 마스터, *nodeB* 가 슬레이브일 때 failover 이후에는 역할이 바뀌어 *nodeB* 가 마스터, *nodeA* 가 슬레이브가 된다. *nodeB* 를 종료(**cubrid heartbeat stop**)한 후, *nodeA* 가 마스터, 즉 노드 상태가 active로 바뀌었는지 확인(**cubrid heartbeat status**) 한다. 그리고 나서 *nodeB* 를 시작(**cubrid heartbeat start**) 하면, *nodeB* 는 슬레이브가 된다.

.. _duplexing-brokers:

브로커 이중화
-------------

CUBRID는 3-tier DBMS로, 응용 프로그램과 데이터베이스 서버를 중계하는 역할을 수행하는 브로커라는 미들웨어가 있다. CUBRID HA 기능을 제공하기 위해 브로커도 물리적인 하드웨어를 중복으로 구성하여, 하나의 브로커에 장애가 발생해도 응용 프로그램에서는 지속적인 서비스를 제공할 수 있다.

브로커 이중화의 구성은 서버 이중화의 구성에 따라 결정되는 것이 아니며, 사용자의 선호에 맞게 변형이 가능하다. 또한, 별도의 장비로 분리가 가능하다.

브로커의 failover, failback 기능을 사용하려면 JDBC, CCI 또는 PHP의 접속 URL에 **altHosts** 속성을 추가해야 한다. 이에 대한 설명은 JDBC 설정, CCI 설정 또는 PHP 설정을 참고한다.

브로커를 설정하려면 **cubrid_broker.conf** 파일을 설정해야 하고, 데이터베이스 서버의 failover 순서를 설정하려면 **databases.txt** 파일을 설정해야 한다. 이에 대한 설명은 브로커 설정을 참고한다.

다음은 2개의 Read Write(RW) 브로커를 구성한 예이다. application URL의 첫 번째 접속 브로커를 *broker B1* 으로 하고 두 번째 접속 브로커를 *broker B2* 로 설정하면, application이 *broker B1* 에 접속할 수 없는 경우 *broker B2* 에 접속하게 된다. 이후 *broker B1* 이 다시 접속 가능해지면 application은 *broker B1* 에 재접속하게 된다.

.. image:: /images/image25.png

다음은 마스터 노드, 슬레이브 노드의 각 장비 내에 Read Write(RW) 브로커와 Read Only(RO) 브로커를 구성한 예이다. app1과 app2 URL의 첫 번째 접속은 각각 *broker A1* (RW), *broker B2* (RO) 이고, 두 번째 접속(**altHosts**)은 각각 *broker A2* (RO), *broker B1* (RW)이다. *nodeA* 를 포함한 장비가 고장나면, app1과 app2는 *nodeB* 를 포함한 장비의 브로커에 접속한다.

.. image:: /images/image26.png

다음은 브로커 장비를 별도로 구성하여 Read Write 브로커 한 개, Preferred Host Read Only 브로커 두 개를 두고, 한 개의 마스터 노드와 두 개의 슬레이브 노드를 구성한 예이다. Preferred Host Read Only 브로커들은 각각 *nodeB* 와 *nodeC* 에 연결함으로써 읽기 부하를 분산하였다.

.. image:: /images/image27.png

**브로커 failover**

브로커 failover는 시스템 파라미터의 설정에 의해 자동으로 failover되는 것이 아니며, JDBC, CCI, PHP 응용 프로그램에서는 접속 URL의 **altHosts** 에 브로커 호스트들을 설정해야 브로커 failover가 가능하다. 설정한 우선순위가 가장 높은 브로커에 접속하고, 접속한 브로커에 장애가 발생하면 접속 URL에 다음 순위로 설정한 브로커에 접속한다. 응용 프로그램에서는 접속 URL의 **altHosts** 를 설정하는 것 외에는 별도의 처리가 필요 없으며, JDBC, CCI, PHP 드라이버 내부에서 처리한다.

**브로커 failback**

브로커 failover 이후 장애 브로커가 복구되면 기존 브로커와 접속을 끊고 이전에 연결했던 우선순위가 가장 높은 브로커에 다시 접속한다. 응용 프로그램에서는 별도의 처리가 필요 없으며, JDBC, CCI, PHP 드라이버 내부에서 처리한다. 브로커 failback을 수행하는 시간은 JDBC 접속 URL에 설정한 값을 따른다. 이에 대한 설명은 :ref:`ha-jdbc-conf` 을 참고한다.

.. _log-multiplexing:

로그 다중화
-----------

CUBRID HA는 CUBRID HA 그룹에 포함된 모든 노드에 트랜잭션 로그를 복사하고 이를 반영함으로써 CUBRID HA 그룹 내의 모든 노드를 동일한 DB로 유지한다. CUBRID HA의 로그 복사 구조는 마스터 노드와 슬레이브 노드 사이의 상호 복사 형태로, 전체 로그의 양이 많아지는 단점이 있으나 체인 형태의 복사 구조보다 구성 및 장애 처리 측면에서 유연하다는 장점이 있다.

.. image:: /images/image28.png

트랜잭션 로그를 복사하는 모드는 **SYNC**, **SEMISYNC**, **ASYNC** 의 세 가지가 있으며, 사용자가 :ref:`cubrid-ha-conf` 로 설정할 수 있다.

**SYNC 모드**

트랜잭션이 커밋되면, 발생한 트랜잭션 로그가 슬레이브 노드에 복사되어 파일에 저장되고 이에 대한 성공 여부를 전달받은 후에 트랜잭션 커밋이 완료된다. 따라서 다른 모드에 비해 커밋 수행 시간이 길어질 수 있지만, failover가 발생해도 복사된 트랜잭션 로그는 스탠바이 서버에 반영되어 있음을 보장할 수 있으므로 가장 안전하다.

**SEMISYNC 모드**

트랜잭션이 커밋되면, 발생한 트랜잭션 로그가 슬레이브 노드에 복사되어 내부 메커니즘에 의해 최적화된 주기에 따라 저장되고 이에 대한 성공 여부를 전달받은 후에 트랜잭션 커밋이 완료된다. 커밋된 트랜잭션은 언젠가는 슬레이브 노드에 반영될 것이 보장된다.

SEMISYNC 모드는 복제 로그를 매번 파일에 저장하지 않기 때문에 SYNC 모드에 비해 커밋 수행 시간은 줄일 수 있다. 그러나 파일에 기록되기 전까지는 복제 로그가 반영되지 않으므로, 노드 간 데이터 동기화가 지연될 수 있다.

**ASYNC 모드**

트랜잭션이 커밋되면, 슬레이브 노드로 트랜잭션 로그가 전송 완료되었는지 확인하지 않고 커밋이 완료된다. 따라서 마스터 노드에서 커밋이 완료된 트랜잭션이 슬레이브 노드에 반영되지 못하는 경우가 발생할 수 있다.

ASYNC 모드는 로그 복제로 인한 커밋 수행 시간 지연은 거의 없으므로 성능상 유리하지만, 노드 간의 데이터가 완전히 일치하지 않을 수 있다.

빠른 시작
=========

준비
----

**구성도**

CUBRID HA를 처음 접하는 사용자가 CUBRID HA를 쉽게 사용할 수 있도록 아래 그림과 같이 간단하게 구성된 CUBRID HA를 설정하는 과정을 설명한다.

.. image:: /images/image29.png

**사양**

마스터 노드와 슬레이브 노드로 사용할 장비에는 Linux와 CUBRID 2008 R2.2 이상 버전이 설치되어 있어야 한다. CUBRID HA는 Windows를 지원하지 않는다.

**CUBRID HA 구성 장비 사양**

+----------------------+-----------------------+--------+
|                      | CUBRID 버전           | OS     |
+======================+=======================+========+
| 마스터 노드용 장비   | CUBRID 2008 R2.2 이상 | Linux  |
+----------------------+-----------------------+--------+
| 슬레이브 노드용 장비 | CUBRID 2008 R2.2 이상 | Linux  |
+----------------------+-----------------------+--------+

.. note:: 

    이 문서는 2008 R4.1 Patch 2 이상 버전의 HA 구성에 대해 설명하고 있으며, 그 이전 버전과는 설정 방법이 조금 다르므로 주의한다. 예를 들어, **cubrid_ha.conf** 는 2008 R4.0 이상 버전에서 도입되었다. **ha_make_slavedb.sh** 는 2008 R4.1 Patch 2 이상 버전에 대해 설명하고 있다.

.. _quick-server-config:

데이터베이스 생성 및 서버 설정
------------------------------

**데이터베이스 생성**

CUBRID HA에 포함할 데이터베이스를 모든 CUBRID HA 노드에서 동일하게 생성한다. 데이터베이스 생성 옵션은 필요에 따라 적절히 변경한다. ::

    [nodeA]$ cd $CUBRID_DATABASES
    [nodeA]$ mkdir testdb
    [nodeA]$ cd testdb
    [nodeA]$ mkdir log
    [nodeA]$ cubrid createdb -L ./log testdb
    Creating database with 512.0M size. The total amount of disk space needed is 1.5G.
     
    CUBRID 9.0
     
    [nodeA]$

**cubrid.conf**

**$CUBRID/conf/cubrid.conf** 의 **ha_mode** 를 모든 HA 노드에 동일하게 설정한다. 특히, 로깅 관련 파라미터인 **log_max_archives** 와 **force_remove_log_archives**, HA 관련 파라미터인 **ha_mode** 의 설정에 주의한다. ::

    # Service parameters
    [service]
    service=server,broker,manager
     
    # Common section
    [common]
    service=server,broker,manager
     
    # Server parameters
    server=testdb
    data_buffer_size=512M
    log_buffer_size=4M
    sort_buffer_size=2M
    max_clients=100
    cubrid_port_id=1523
    db_volume_size=512M
    log_volume_size=512M
     
    # HA 구성 시 추가 (Logging parameters)
    log_max_archives=100
    force_remove_log_archives=no
     
    # HA 구성 시 추가 (HA 모드)
    ha_mode=on

**cubrid_ha.conf**

**$CUBRID/conf/cubrid_ha.conf** 의 **ha_port_id**, **ha_node_list**, **ha_db_list** 를 모든 HA 노드에 동일하게 설정한다. 다음 예에서 마스터 노드의 호스트 이름은 *nodeA*, 슬레이브 노드의 호스트 이름은 *nodeB*\라고 가정한다.::

    [common]
    ha_port_id=59901
    ha_node_list=cubrid@nodeA:nodeB
    ha_db_list=testdb
    ha_copy_sync_mode=sync:sync
    ha_apply_max_mem_size=500

**databases.txt**

**$CUBRID_DATABASES/databases.txt** (**$CUBRID_DATABASES** 가 설정 안 된 경우 **$CUBRID/databases/databases.txt**)의 db-host에 마스터 노드와 슬레이브 노드의 호스트 이름을 설정(*nodeA*:*nodeB*)한다. ::

    #db-name vol-path db-host log-path lob-base-path
    testdb /home/cubrid/DB/testdb nodeA:nodeB /home/cubrid/DB/testdb/log file:/home/cubrid/DB/testdb/lob

CUBRID HA 시작 및 확인
----------------------

**CUBRID HA 시작**

CUBRID HA 그룹 내의 각 노드에서 **cubrid heartbeat start** 를 수행한다. **cubrid heartbeat start** 를 가장 먼저 수행한 노드가 마스터 노드가 되므로 유의해야 한다. 이하의 예에서 마스터 노드의 호스트 이름은 *nodeA*, 슬레이브 노드의 호스트 이름은 *nodeB*\라고 가정한다.

*   마스터 노드 ::

    [nodeA]$ cubrid heartbeat start

*   슬레이브 노드 ::

    [nodeB]$ cubrid heartbeat start

**CUBRID HA 상태 확인**

CUBRID HA 그룹 내의 각 노드에서 **cubrid heartbeat status** 를 수행하여 구성 상태를 확인한다. ::

    [nodeA]$ cubrid heartbeat status
    @ cubrid heartbeat list
     HA-Node Info (current nodeA-node-name, state master)
       Node nodeB-node-name (priority 2, state slave)
       Node nodeA-node-name (priority 1, state master)
     HA-Process Info (nodeA 9289, state nodeA)
       Applylogdb testdb@localhost:/home1/cubrid1/DB/testdb_nodeB.cub (pid 9423, state registered)
       Copylogdb testdb@nodeB-node-name:/home1/cubrid1/DB/testdb_nodeB.cub (pid 9418, state registered)
       Server testdb (pid 9306, state registered_and_active)
     
    [nodeA]$

CUBRID HA 그룹 내의 각 노드에서 **cubrid changemode** 유틸리티를 이용하여 서버의 상태를 확인한다.

* 마스터 노드 ::

    [nodeA]$ cubrid changemode testdb@localhost
    The server 'testdb@localhost''s current HA running mode is active.

* 슬레이브 노드 ::

    [nodeB]$ cubrid changemode testdb@localhost
    The server 'testdb@localhost''s current HA running mode is standby.

**CUBRID HA 동작 여부 확인**

마스터 노드의 액티브 서버에서 쓰기를 수행한 후 슬레이브 노드의 스탠바이 서버에 정상적으로 반영되었는지 확인한다. HA 환경에서 CSQL 인터프리터로 각 노드에 접속하려면, 데이터베이스 이름 뒤에 접속 대상 호스트 이름을 반드시 지정해야 한다("@<호스트 이름>"). 호스트 이름을 localhost로 지정하면, 로컬 노드에 접속하게 된다.

.. warning:: 복제가 정상적으로 수행되기 위해서는 테이블을 생성할 때 기본키(primary key)가 반드시 존재해야 한다는 점을 주의한다

* 마스터 노드 ::

    [nodeA]$ csql -u dba testdb@localhost -c "create table abc(a int, b int, c int, primary key(a));"
    [nodeA]$ csql -u dba testdb@localhost -c "insert into abc values (1,1,1);"
    [nodeA]$

* 슬레이브 노드 ::

    [nodeB]$ csql -u dba testdb@localhost -l -c "select * from abc;"
    === <Result of SELECT Command in Line 1> ===
    <00001> a: 1
            b: 1
            c: 1
    [nodeB]$

.. _quick-broker-config:

브로커 설정, 시작 및 확인
-------------------------

**브로커 설정**

데이터베이스 failover 시 정상적인 서비스를 위해서 **databases.txt** 의 **db-host** 항목에 데이터베이스의 가용 노드를 설정해야 한다. 그리고 **cubrid_broker.conf** 의 **ACCESS_MODE** 를 설정하는데, 이를 생략하면 기본값인 Read Write 모드로 설정된다. 브로커를 별도의 장비로 분리하는 경우 브로커 장비에 **cubrid_broker.conf** 와 **databases.txt** 를 반드시 설정해야 한다.

* databases.txt ::

    #db-name        vol-path                db-host         log-path        lob-base-path
    testdb          /home1/cubrid1/CUBRID/testdb  nodeA:nodeB        /home1/cubrid1/CUBRID/testdb/log file:/home1/cubrid1/CUBRID/testdb/lob

* cubrid_broker.conf ::

    [%testdb_RWbroker]
    SERVICE                 =ON
    BROKER_PORT             =33000
    MIN_NUM_APPL_SERVER     =5
    MAX_NUM_APPL_SERVER     =40
    APPL_SERVER_SHM_ID      =33000
    LOG_DIR                 =log/broker/sql_log
    ERROR_LOG_DIR           =log/broker/error_log
    SQL_LOG                 =ON
    TIME_TO_KILL            =120
    SESSION_TIMEOUT         =300
    KEEP_CONNECTION         =AUTO
    CCI_DEFAULT_AUTOCOMMIT  =ON
     
    # broker mode parameter
    ACCESS_MODE             =RW


**브로커 시작 및 상태 확인**

브로커는 JDBC나 CCI, PHP 등의 응용에서 접근하기 위해 사용하는 것이다. 따라서 간단한 서버 이중화 동작을 시험하고 싶다면 브로커를 시작할 필요 없이 서버 프로세스에 직접 접속하는 CSQL 인터프리터만 실행해서 확인할 수 있다. 브로커는 **cubrid broker start** 를 실행하여 시작하고 **cubrid broker stop** 을 실행하여 정지한다.

다음은 브로커를 마스터 노드에서 실행한 예이다. ::

    [nodeA]$ cubrid broker start
    @ cubrid broker start
    ++ cubrid broker start: success
    [nodeA]$ cubrid broker status
    @ cubrid broker status
    % testdb_RWbroker  - cub_cas [9531,33000] /home1/cubrid1/CUBRID/log/broker//testdb.access /home1/cubrid1/CUBRID/log/broker//testdb.err
     JOB QUEUE:0, AUTO_ADD_APPL_SERVER:ON, SQL_LOG_MODE:ALL:100000
     LONG_TRANSACTION_TIME:60.00, LONG_QUERY_TIME:60.00, SESSION_TIMEOUT:300
     KEEP_CONNECTION:AUTO, ACCESS_MODE:RW
    ---------------------------------------------------------
    ID   PID   QPS   LQS PSIZE STATUS
    ---------------------------------------------------------
     1  9532     0     0  48120  IDLE
 

**응용 프로그램 설정**

응용 프로그램이 연결할 브로커의 호스트 이름(*nodeA_broker*, *nodeB_broker*)과 포트를 연결 URL에 명시한다. 브로커와의 연결 장애가 발생한 경우 다음으로 연결을 시도할 브로커는 **altHosts** 속성에 명시한다. 아래는 JDBC 프로그램의 예이며, CCI, PHP에 대한 예와 자세한 설명은 :ref:`ha-cci-conf`, :ref:`ha-php-conf` 을 참고한다. 

.. code-block:: java

    Connection connection = DriverManager.getConnection("jdbc:CUBRID:nodeA_broker:33000:testdb:::?charSet=utf-8&altHosts=nodeB_broker:33000", "dba", "");

.. _ha-configuration:

환경 설정
=========

cubrid.conf
-----------

**cubrid.conf** 파일은 **$CUBRID/conf** 디렉터리에 위치하며, CUBRID의 전반적인 설정 정보를 담고 있다. 여기에서는 **cubrid.conf** 중 CUBRID HA가 사용하는 파라미터를 설명한다.

**ha_mode**

CUBRID HA 기능을 설정하는 파라미터이다. 기본값은 **off** 이다. CUBRID HA 기능은 Windows를 지원하지 않고 Linux에서만 사용할 수 있으므로 이 값은 Linux용 CUBRID에서만 의미가 있다.

*   **off** : CUBRID HA 기능을 사용하지 않는다.
*   **on** : CUBRID HA 기능을 사용하며, 해당 노드는 failover의 대상이 된다.
*   **replica** : CUBRID HA 기능을 사용하며, 해당 노드는 failover의 대상이 되지 않는다.

**ha_mode** 파라미터는 **[@<database>]** 섹션에서 재설정할 수 있으나, **off** 만 입력할 수 있다. **[@<database>]** 섹션에 **off** 가 아닌 값을 입력하면 오류가 출력된다.

**ha_mode** 가 **on** 이면 **cubrid_ha.conf** 를 읽어 CUBRID HA를 설정한다.

이 파라미터는 동적으로 변경할 수 없으며, 변경하면 해당 노드를 다시 시작해야 한다.

**log_max_archives**

보존할 보관 로그 파일의 최소 개수를 설정하는 파라미터이다. 최소값은 0이며 기본값은 **INT_MAX** (2147483647)이다. CUBRID 설치 시 **cubrid.conf** 에는 0으로 설정되어 있다. 이 파라미터의 동작은 **force_remove_log_archives** 의 영향을 받는다.

활성화된 트랜잭션이 참조하고 있는 기존 보관 로그 파일이나, HA 환경에서 슬레이브 노드에 반영되지 않은 마스터 노드의 보관 로그 파일은 삭제되지 않는다. 이에 대한 자세한 내용은 아래의 **force_remove_log_archives** 를 참고한다.

**log_max_archives** 에 대한 자세한 내용은 :ref:`logging-parameters` 를 참고한다.

**force_remove_log_archives**

**ha_mode** 를 on으로 설정하여 HA 환경을 구축하려면 **force_remove_log_archives** 를 no로 설정하여 HA 관련 프로세스에 의해 사용할 보관 로그(archive log)를 항상 유지하는 것을 권장한다.

**force_remove_log_archives** 를 yes로 설정하면 HA 관련 프로세스가 사용할 보관 로그 파일까지 삭제될 수 있고, 이로 인해 데이터베이스 복제 노드 간 데이터 불일치가 발생할 수 있다. 이러한 위험성을 감수하더라도 디스크의 여유 공간을 유지하고 싶다면 **force_remove_log_archives** 를 yes로 설정한다.

**force_remove_log_archives** 에 대한 자세한 내용은 :ref:`logging-parameters` 를 참고한다.

.. note::

    2008 R4.3 버전부터, 레플리카 노드에서는 **force_remove_log_archives** 값의 설정과 무관하게 **log_max_archives** 파라미터에 설정된 개수의 보관 로그 파일을 제외하고는 항상 삭제한다.

**max_clients**

데이터베이스 서버에 동시에 연결할 수 있는 클라이언트의 최대 수를 지정하는 파라미터이다. 기본값은 **100** 이다.

CUBRID HA 기능을 사용하면 기본적으로 복제 로그 복사 프로세스와 복제 로그 반영 프로세스가 구동되므로, 해당 노드를 제외한 CUBRID HA 그룹 내 노드 수의 두 배를 고려하여 설정해야 한다. 또한 failover가 일어날 때 다른 노드에 접속하고 있던 클라이언트가 해당 노드에 접속할 수 있으므로 이를 고려해야 한다. **max_clients** 에 대한 자세한 내용은 :ref:`connection-parameters` 를 참고한다.

**노드 간 반드시 값이 동일해야 하는 시스템 파라미터**

*   **log_buffer_size** : 로그 버퍼 크기. 서버와 로그를 복사하는 **copylogdb** 간 프로토콜에 영향을 주는 부분이므로 반드시 동일해야 한다.

*   **log_volume_size** : 로그 볼륨 크기. CUBRID HA는 원본 트랜잭션 로그와 복제 로그의 형태와 내용이 동일하므로 반드시 동일해야 한다. 그 외 각 노드에서 별도로 DB를 생성하는 경우 **cubrid createdb** 옵션(**--db-volume-size**, **--db-page-size**, **--log-volume-size**, **--log-page-size** 등)이 동일해야 한다.

*   **cubrid_port_id** : 서버와의 연결 생성을 위한 TCP 포트 번호. 서버와 로그를 복사하는 **copylogdb** 의 연결을 위해 반드시 동일해야 한다.

*   **HA 관련 파라미터** : **cubrid_ha.conf** 에 포함된 HA 관련 파라미터는 기본적으로 동일해야 하며, 다음 파라미터는 예외적으로 노드에 따라 다르게 설정할 수 있다.

    *   레플리카 노드의 **ha_mode** 파라미터
    *   **ha_copy_sync_mode** 파라미터
    *   **ha_ping_hosts** 파라미터

**예시**

다음은 **cubrid.conf** 설정의 예이다. 특히, 로깅 관련 파라미터인 **log_max_archives** 와 **force_remove_log_archives**, HA 관련 파라미터인 **ha_mode** 의 설정에 주의한다. ::

    # Service Parameters
    [service]
    service=server,broker,manager
     
     
    # Server Parameters
    server=testdb
    data_buffer_size=512M
    log_buffer_size=4M
    sort_buffer_size=2M
    max_clients=200
    cubrid_port_id=1523
    db_volume_size=512M
    log_volume_size=512M
     
    # HA 구성 시 추가 (Logging parameters)
    log_max_archives=100
    force_remove_log_archives=no
     
    # HA 구성 시 추가 (HA 모드)
    ha_mode=on
    log_max_archives=100

.. _cubrid-ha-conf:

cubrid_ha.conf
--------------

**cubrid_ha.conf** 파일은 **$CUBRID/conf** 디렉터리에 위치하며, CUBRID의 HA 기능의 전반적인 설정 정보를 담고 있다. CUBRID HA 기능은 Windows를 지원하지 않고 Linux에서만 사용할 수 있으므로 이 값은 Linux용 CUBRID에서만 의미가 있다.

**ha_node_list**

CUBRID HA 그룹 내에서 사용할 그룹 이름과 failover의 대상이 되는 멤버 노드들의 호스트 이름을 명시한다. @ 구분자로 나누어 @ 앞이 그룹 이름, @ 뒤가 멤버 노드들의 호스트 이름이다. 여러 개의 호스트 이름은 콜론(:)으로 구분한다. 기본값은 **localhost@localhost** 이다.

이 파라미터에서 명시한 멤버 노드들의 호스트 이름은 IP로 대체할 수 없으며, 사용자는 반드시 **/etc/hosts** 에 등록되어 있는 것을 사용해야 한다. **ha_mode** 를 **on** 으로 설정한 노드는 **ha_node_list** 에 해당 노드가 반드시 포함되어 있어야 한다. CUBRID HA 그룹 내의 모든 노드는 **ha_node_list** 의 값이 동일해야 한다. failover가 일어날 때 이 파라미터에 설정된 순서에 따라 마스터 노드가 된다.

이 파라미터는 동적으로 변경할 수 있으며, 변경하면 :ref:`cubrid heartbeat reload <cubrid-heartbeat>` 를 실행해야 한다.

**ha_replica_list**

CUBRID HA 그룹 내에서 사용할 그룹 이름과 failover의 대상이 되지 않는 멤버 노드들의 호스트 이름을 명시한다. @ 구분자로 나누어 @ 앞이 그룹 이름, @ 뒤가 멤버 노드들의 호스트 이름이다. 여러 개의 호스트 이름은 콜론(:)으로 구분한다. 기본값은 **NULL** 이다.

그룹 이름은 **ha_node_list** 에서 명시한 이름과 같아야 한다. 이 파라미터에서 명시하는 멤버 노드들의 호스트 이름 및 해당 노드의 호스트 이름을 지정할 때는 반드시 **/etc/hosts** 에 등록되어 있는 것을 사용해야 한다. **ha_mode** 를 **replica** 로 설정한 노드는 **ha_replica_list** 에 해당 노드가 반드시 포함되어 있어야 한다. CUBRID HA 그룹 내의 모든 노드는 **ha_replica_list** 의 값이 동일해야 한다.

이 파라미터는 동적으로 변경할 수 있으며, 변경하면 :ref:`cubrid heartbeat reload <cubrid-heartbeat>` 를 실행해야 한다.

**ha_port_id**

CUBRID HA 그룹 내의 노드들이 heartbeat 메시지를 주고 받으며 노드 장애를 감지할 때 사용할 UDP 포트 번호를 명시한다. 기본값은 **59901** 이다.

서비스 환경에 방화벽이 있으면, 설정한 포트 값이 방화벽을 통과하도록 방화벽을 설정해야 한다.

**ha_ping_hosts**

슬레이브 노드에서 failover가 시작되는 순간 연결을 확인하여 네트워크에 의한 failover인지 확인할 때 사용할 호스트를 명시한다. 기본값은 **NULL** 이다.

이 파라미터에서 명시한 멤버 노드들의 호스트 이름은 IP로 대체할 수 있으며, 호스트 이름을 사용하는 경우에는 반드시 **/etc/hosts** 에 등록되어 있어야 한다.

이 파라미터를 설정하면 불안정한 네트워크로 인해 상대 마스터 노드가 비정상 종료된 것으로 오인한 슬레이브 노드가 마스터 노드로 역할이 변경되면서 동시에 두 개의 마스터 노드가 존재하게 되는 split-brain 현상을 방지할 수 있다. 여러 개의 호스트를 콜론(:)으로 구분하여 지정할 수 있다.

**ha_copy_sync_mode**

트랜잭션 로그의 복사본을 저장하는 모드를 설정한다. 기본값은 **SYNC** 이다.

**SYNC**, **SEMISYNC**, **ASYNC** 를 값으로 설정할 수 있다. **ha_node_list** 에 지정한 노드의 수만큼 설정해야 하고 순서가 같아야 한다. 콜론(:)으로 구분한다. 레플리카 노드는 이 값의 설정과 관계없이 항상 ASNYC 모드로 동작한다.

자세한 내용은 :ref:`log-multiplexing` 를 참고한다.

**ha_copy_log_base**

트랜잭션 로그의 복사본을 저장할 위치를 지정한다. 기본값은 **$CUBRID_DATABASES** 이다.

자세한 내용은 :ref:`log-multiplexing` 를 참고한다.

**ha_db_list**

CUBRID HA 모드로 구동할 데이터베이스 이름을 명시한다. 기본값은 **NULL** 이다. 여러 개의 데이터베이스 이름은 쉼표(,)로 구분한다.

**ha_apply_max_mem_size**

CUBRID HA의 복제 로그 반영 프로세스가 사용할 수 있는 최대 메모리를 설정한다. 기본값과 최대값은 **500** 이며, 단위는 MB이다. 이 값을 시스템이 허용하는 크기보다 너무 크게 설정하면 메모리 할당에 실패하면서 HA 복제 반영 프로세스가 오동작을 일으킬 수 있으므로, 메모리 자원이 설정한 값을 충분히 사용할 수 있는지 확인한 후 설정하도록 한다.

**ha_applylogdb_ignore_error_list**

CUBRID HA의 복제 로그 반영 프로세스에서 에러가 발생해도 이를 무시하고 계속 복제를 진행하기 위해 이 값을 설정한다. 쉼표(,)로 구분하여 무시할 에러 코드를 나열한다. 이 설정 값은 높은 우선순위를 가지므로, **ha_applylogdb_retry_error_list** 파라미터나 "재시도 에러 리스트"에 의해 설정된 에러 코드와 값이 겹치면 이들을 무시하고 해당 에러를 유발한 작업을 재시도하지 않는다. "재시도 에러 리스트"는 아래 **ha_applylogdb_retry_error_list** 의 설명을 참고한다.

**ha_applylogdb_retry_error_list**

CUBRID HA의 복제 로그 반영 프로세스에서 에러가 발생하면 해당 에러를 유발한 작업이 성공할 때까지 반복적으로 재시도하기 위해 이 값을 설정한다. 쉼표(,)로 구분하여 재시도할 에러 코드를 나열한다. 이 값을 설정하지 않아도 기본으로 설정된 "재시도 에러 리스트"는 다음 표와 같다. 하지만 이 값들이 **ha_applylogdb_ignore_error_list** 에 존재하면 에러를 무시하고 계속 복제를 진행한다.

**재시도 에러 리스트**

+-------------------------------------+-----------+
| 에러 코드 이름                      | 에러 코드 |
+=====================================+===========+
| ER_LK_UNILATERALLY_ABORTED          | -72       |
+-------------------------------------+-----------+
| ER_LK_OBJECT_TIMEOUT_SIMPLE_MSG     | -73       |
+-------------------------------------+-----------+
| ER_LK_OBJECT_TIMEOUT_CLASS_MSG      | -74       |
+-------------------------------------+-----------+
| ER_LK_OBJECT_TIMEOUT_CLASSOF_MSG    | -75       |
+-------------------------------------+-----------+
| ER_LK_PAGE_TIMEOUT                  | -76       |
+-------------------------------------+-----------+
| ER_PAGE_LATCH_TIMEDOUT              | -836      |
+-------------------------------------+-----------+
| ER_PAGE_LATCH_ABORTED               | -859      |
+-------------------------------------+-----------+
| ER_LK_OBJECT_DL_TIMEOUT_SIMPLE_MSG  | -966      |
+-------------------------------------+-----------+
| ER_LK_OBJECT_DL_TIMEOUT_CLASS_MSG   | -967      |
+-------------------------------------+-----------+
| ER_LK_OBJECT_DL_TIMEOUT_CLASSOF_MSG | -968      |
+-------------------------------------+-----------+
| ER_LK_DEADLOCK_CYCLE_DETECTED       | -1021     |
+-------------------------------------+-----------+

다음은 **cubrid_ha.conf** 설정의 예이다. ::

    [common]
    ha_node_list=cubrid@nodeA:nodeB
    ha_db_list=testdb
    ha_copy_sync_mode=sync:sync
    ha_apply_max_mem_size=500

**참고 사항**

다음은 멤버 노드의 호스트 이름이 *nodeA* 이고 IP 주소가 192.168.0.1일 때 /etc/hosts를 설정한 예이다. ::

    127.0.0.1 localhost.localdomain localhost
    192.168.0.1 nodeA

.. _ha-cubrid-broker-conf:

cubrid_broker.conf
------------------

**cubrid_broker.conf** 파일은 **$CUBRID/conf** 디렉터리에 위치하며, 브로커의 전반적인 설정 정보를 담고 있다. 여기에서는 **cubrid_broker.conf** 중 CUBRID HA가 사용하는 파라미터를 설명한다.

**ACCESS_MODE**

브로커의 모드를 설정한다. 기본값은 **RW** 이다.

**RW** (Read Write), **RO** (Read Only), **SO** (Slave Only), **PHRO** (Preferred Host Read Only)를 값으로 설정할 수 있다. 자세한 내용은 :ref:`broker-mode` 를 참고한다.

**PREFERRED_HOSTS**

**ACCESS_MODE** 파라미터의 값이 **PHRO** 일 때만 사용되는 파라미터이다. 기본값은 **NULL** 이다.

여러 노드를 지정할 수 있으며 콜론(:)으로 구분한다. 먼저 **PREFERRED_HOSTS** 파라미터에 설정된 호스트 순서대로 연결을 시도한 후 **$CUBRID_DATABASES/databases.txt** 에 설정된 호스트 순서대로 연결을 시도한다. 자세한 내용은 :ref:`broker-mode` 를 참고한다.

다음은 **cubrid_broker.conf** 설정의 예이다. ::

    [%PHRO_broker]
    SERVICE                 =ON
    BROKER_PORT             =33000
    MIN_NUM_APPL_SERVER     =5
    MAX_NUM_APPL_SERVER     =40
    APPL_SERVER_SHM_ID      =33000
    LOG_DIR                 =log/broker/sql_log
    ERROR_LOG_DIR           =log/broker/error_log
    SQL_LOG                 =ON
    TIME_TO_KILL            =120
    SESSION_TIMEOUT         =300
    KEEP_CONNECTION         =AUTO
    CCI_DEFAULT_AUTOCOMMIT  =ON
     
    # Broker mode setting parameter
    ACCESS_MODE             =PHRO
    PREFERRED_HOSTS         =nodeA:nodeB:nodeC

databases.txt
-------------

**databases.txt** 파일은 **$CUBRID_DATABASES** (설정되어 있지 않은 경우 $CUBRID/databases) 디렉터리에 위치하며, **db_hosts** 값을 설정하여 브로커가 접속하는 서버의 순서를 결정할 수 있다. 여러 노드를 설정하려면 콜론(:)으로 구분한다.

다음은 **databases.txt** 설정의 예이다. ::

    #db-name    vol-path        db-host     log-path     lob-base-path
    testdb       /home/cubrid/DB/testdb nodeA:nodeB   /home/cubrid/DB/testdb/log  file:/home/cubrid/DB/testdb/lob

.. _ha-jdbc-conf:

JDBC 설정
---------

JDBC에서 CUBRID HA 기능을 사용하려면 브로커(*nodeA_broker*)에 장애가 발생했을 때 다음으로 연결할 브로커(*nodeB_broker*)의 연결 정보를 연결 URL에 추가로 지정해야 한다. CUBRID HA를 위해 지정되는 속성은 장애가 발생했을 때 연결할 하나 이상의 브로커 노드 정보인 **altHosts** 이다. 이에 대한 자세한 설명은 :ref:`jdbc-connection-conf` 를 참고한다.

다음은 JDBC 설정의 예이다.

.. code-block:: java

    Connection connection = DriverManager.getConnection("jdbc:CUBRID:nodeA_broker:33000:testdb:::?charSet=utf-8&altHosts=nodeB_broker:33000", "dba", "");

.. _ha-cci-conf:

CCI 설정
--------

CCI에서 CUBRID HA 기능을 사용하려면 브로커에 장애가 발생했을 때 연결할 브로커의 연결 정보를 연결 URL에 추가로 지정할 수 있는 :c:func:`cci_connect_with_url` 함수를 사용하여 브로커와 연결해야 한다. CUBRID HA를 위해 지정되는 속성은 장애가 발생했을 때 연결할 하나 이상의 브로커 노드 정보인 **altHosts** 이다.

다음은 CCI 설정의 예이다.

.. code-block:: c

    con = cci_connect_with_url ("cci:CUBRID:nodeA_broker:33000:testdb:::?altHosts=nodeB_broker:33000", "dba", NULL);
    if (con < 0)
    {
          printf ("cannot connect to database\n");
          return 1;
    }

.. _ha-php-conf:

PHP 설정
--------

PHP에서 CUBRID HA 기능을 사용하려면 브로커에 장애가 발생했을 때 연결할 브로커의 연결 정보를 연결 URL에 추가로 지정할 수 있는 `cubrid_connect_with_url <http://www.php.net/manual/en/function.cubrid-connect-with-url.php>`_ 함수를 사용하여 브로커와 연결해야 한다. CUBRID HA를 위해 지정되는 속성은 장애가 발생했을 때 연결할 하나 이상의 브로커 노드 정보인 **altHosts** 이다.

다음은 PHP 설정의 예이다.

.. code-block:: php

    <?php
    $con = cubrid_connect_with_url ("cci:CUBRID:nodeA_broker:33000:testdb:::?altHosts=nodeB_broker:33000", "dba", NULL);
    if ($con < 0)
    {
          printf ("cannot connect to database\n");
          return 1;
    }
    ?>

.. note:: altHosts를 설정하여 브로커 절체(failover)가 가능하도록 설정한 환경에서, 브로커 절체가 원활하게 되려면 URL에 **disconnectOnQueryTimeout** 값을 **true** 로 설정해야 한다.
    이 값이 true면 질의 타임아웃 발생 시 응용 프로그램은 즉시 기존에 접속되었던 브로커와의 접속을 해제하고 altHosts에 지정한 브로커로 접속한다.

구동 및 모니터링
================

.. _cubrid-heartbeat:

cubrid heartbeat 유틸리티
-------------------------

**start**

해당 노드의 CUBRID HA 기능을 활성화하고 구성 프로세스(데이터베이스 서버 프로세스, 복제 로그 복사 프로세스, 복제 로그 반영 프로세스)를 모두 구동한다. **cubrid heartbeat start** 를 실행하는 순서에 따라 마스터 노드와 슬레이브 노드가 결정되므로, 순서를 주의해야 한다.

사용법은 다음과 같다. ::

    $ cubrid heartbeat start

HA 모드로 설정된 데이터베이스 서버 프로세스는 **cubrid server start** 명령으로 시작할 수 없다.

노드 내에서 특정 데이터베이스의 HA 구성 프로세스들(데이터베이스 서버 프로세스, 복제 로그 복사 프로세스, 복제 로그 반영 프로세스)만 구동하려면 명령의 마지막에 데이터베이스 이름을 지정한다. 예를 들어, 데이터베이스 *testdb* 만 구동하려면 다음 명령을 사용한다. ::

    $ cubrid heartbeat start testdb

**stop**

해당 노드의 CUBRID HA 기능을 비활성화하고 구성 프로세스(데이터베이스 서버 프로세스, 복제 로그 복사 프로세스, 복제 로그 반영 프로세스)를 모두 종료한다. 이 명령을 실행한 노드의 HA 기능은 종료되고 HA 구성에 있는 다음 순위의 슬레이브 노드로 failover가 일어난다.

사용법은 다음과 같다. ::

    $ cubrid heartbeat stop

HA 모드로 설정된 데이터베이스 서버 프로세스는 **cubrid server stop** 명령으로 정지할 수 없다.

노드 내에서 특정 데이터베이스의 HA 구성 프로세스들(데이터베이스 서버 프로세스, 복제 로그 복사 프로세스, 복제 로그 반영 프로세스)만 정지하려면 명령의 마지막에 데이터베이스 이름을 지정한다. 예를 들어, 데이터베이스 *testdb* 를 정지하려면 다음 명령을 사용한다. ::

    $ cubrid heartbeat stop testdb
    
**copylogdb**

CUBRID HA 구성에서 특정 peer_node의 db_name에 대한 트랜잭션 로그를 복사하는 **copylogdb** 프로세스를 시작 또는 정지한다. 운영 도중 복제 재구축을 위해 로그 복사를 일시 정지했다가 재구동하고 싶은 경우 사용할 수 있다.

**cubrid heartbeat copylogdb start** 명령만 성공한 경우에도 노드 간 장애 감지 및 복구 기능이 수행되며, failover의 대상이 되어 슬레이브 노드인 경우 마스터 노드로 역할이 변경될 수 있다.

사용법은 다음과 같다. ::

    $ cubrid heartbeat copylogdb <start|stop> db_name peer_node

**copylogdb** 프로세스의 시작/정지 시 **cubrid_ha.conf** 의 설정 정보를 사용하므로 한 번 정한 설정은 가급적 바꾸지 않을 것을 권장하며, 바꾸어야만 하는 경우 노드 전체를 재구동할 것을 권장한다.

**applylogdb**

CUBRID HA 구성에서 특정 peer_node의 db_name에 대한 트랜잭션 로그를 반영하는 **applylogdb** 프로세스를 시작 또는 정지한다. 운영 도중 복제 재구축을 위해 로그 반영을 일시 정지했다가 재구동하고 싶은 경우 사용할 수 있다.

**cubrid heartbeat applylogdb start** 명령만 성공한 경우에도 노드 간 장애 감지 및 복구 기능이 수행되며, failover의 대상이 되어 슬레이브 노드인 경우 마스터 노드로 역할이 변경될 수 있다.

사용법은 다음과 같다. ::

    $ cubrid heartbeat applylogdb <start|stop> db_name peer_node
    
**applylogdb** 프로세스의 시작/정지 시 **cubrid_ha.conf** 의 설정 정보를 사용하므로 한 번 정한 설정은 가급적 바꾸지 않을 것을 권장하며, 바꾸어야만 하는 경우 노드 전체를 재구동할 것을 권장한다.

**reload**

**cubrid_ha.conf** 에서 CUBRID HA 구성 정보를 다시 읽고 새로운 구성에 맞는 CUBRID HA의 구성 요소들을 구동 및 종료한다. 노드를 추가하거나 삭제하는 경우 사용하며, 수정 이전에 비해 추가된 노드에 해당하는 HA 프로세스들을 시작하거나, 삭제된 노드에 해당하는 HA 프로세스들을 정지한다.

사용법은 다음과 같다. ::

    $ cubrid heartbeat reload

변경할 수 있는 구성 정보는 ha_node_list와 ha_replica_list이다. 이 명령을 실행 중에 특정 노드에서 오류가 발생하더라도 남은 작업을 계속 진행한다. **reload** 명령이 종료된 후 **status** 명령으로 노드의 재구성이 잘 반영되었는지 확인하여, 재구성에 실패한 경우 원인을 찾아 해소하도록 한다.

**status**

CUBRID HA 그룹 정보와 CUBRID HA 구성 요소의 정보를 확인할 수 있다. 사용법은 다음과 같다. ::

    $ cubrid heartbeat status
    @ cubrid heartbeat status
     
     HA-Node Info (current nodeB, state slave)
       Node nodeB (priority 2, state slave)
       Node nodeA (priority 1, state master)
     
     
     HA-Process Info (master 2143, state slave)
       Applylogdb testdb@localhost:/home/cubrid/DB/testdb_nodeB (pid 2510, state registered)
       Copylogdb testdb@nodeA:/home/cubrid/DB/testdb_nodeA (pid 2505, state registered)
       Server testdb (pid 2393, state registered_and_standby)

.. note:: CUBRID 9.0 미만 버전에서 사용되었던 **act**, **deact**, **deregister** 명령은 더 이상 사용되지 않는다.

.. _cubrid-service-util:

cubrid service 유틸리티
-----------------------

CUBRID 서비스에 heartbeat를 등록하면 **cubrid service** 유틸리티를 사용하여 한 번에 관련된 프로세스들을 모두 구동/정지하거나 상태를 알아볼 수 있어 편리하다. CUBRID 서비스 등록은 **cubrid.conf** 파일의 [**service**] 섹션에 있는 **service** 파라미터에 설정할 수 있다. 이 파라미터에 **heartbeat** 를 포함하면 **cubrid service start** / **stop** 명령을 사용하여 서비스의 프로세스 및 HA 관련 프로세스를 모두 한 번에 구동/중지할 수 있다.

다음은 **cubrid.conf** 파일을 설정하는 예이다. ::

    # cubrid.conf

    ...

    [service]

    ...

    service=broker,heartbeat

    ...

    [common]

    ...

    ha_mode=on

.. _cubrid-applyinfo:

cubrid applyinfo
----------------

CUBRID HA의 복제 로그 복사 및 반영 상태를 확인한다. ::

    cubrid applyinfo [options] <database-name>
    
*   *database-name* : 확인하려는 서버의 데이터베이스 이름을 명시한다. 노드 이름은 입력하지 않는다.

**cubrid applyinfo**\에서 사용하는 [options]는 다음과 같다.

.. program:: applyinfo

.. option:: -r, --remote-host-name=HOSTNAME

    트랜잭션 로그를 복사하는 대상 노드의 호스트 이름을 설정한다. 이 옵션을 설정하면 대상 노드의 액티브 로그 정보(Active Info.)를 출력한다.

.. option:: -a, --applied-info

    cubrid applyinfo를 수행한 노드(localhost)의 복제 반영 정보(Applied Info.)를 출력한다. 이 옵션을 사용하기 위해서는 반드시 **-L** 옵션이 필요하다.

.. option:: -L, --copied-log-path=PATH

    상대 노드의 트랜잭션 로그를 복사해 온 위치를 설정한다. 이 옵션이 설정된 경우 상대 노드에서 복사해 온 트랜잭션 로그의 정보(Copied Active Info.)를 출력한다.

.. option:: -p, --pageid=ID

    **-L** 옵션을 설정한 경우 설정 가능하며, 복사해 온 로그의 특정 페이지 정보를 출력한다. 기본값은 0으로, 활성 페이지(active page)를 의미한다.
    
.. option:: -v

    더 자세한 내용을 출력한다.
    
.. option:: -i, --interval=SECOND

    트랜잭션 로그 복사 또는 반영 상태 정보를 지정한 초마다 주기적으로 출력한다. 복제가 지연되는 상태를 확인하려면 이 옵션을 반드시 지정해야 한다
    
**예시**

다음은 슬레이브 노드에서 **applyinfo** 를 실행하여 마스터 노드의 트랜잭션 로그 정보(Active Info.), 슬레이브 노드의 로그 복사 상태 정보(Copied Active Info.)와 로그 반영 상태 정보(Applied Info.)를 확인하는 예이다.

*   Applied Info. : 슬레이브 노드가 복제 로그를 반영한 상태 정보를 나타낸다.
*   Copied Active Info. : 슬레이브 노드가 복제 로그를 복사한 상태 정보를 나타낸다.
*   Active Info. : 마스터 노드가 트랜잭션 로그를 기록한 상태 정보를 나타낸다.
*   Delay in Copying Active Log: 트랜잭션 로그 복사 지연 상태를 나타낸다. 
*    Delay in Applying Copied Log: 트랜잭션 로그 반영 지연 상태를 나타낸다. 

::

    [nodeB] $ cubrid applyinfo -L /home/cubrid/DB/testdb_nodeA -r nodeA -a -i 3 testdb
     
     *** Applied Info. *** 
    Insert count                   : 289492
    Update count                   : 71192
    Delete count                   : 280312
    Schema count                   : 20
    Commit count                   : 124917
    Fail count                     : 0

     *** Copied Active Info. *** 
    DB name                        : testdb
    DB creation time               : 04:29:00.000 PM 11/04/2012 (1352014140)
    EOF LSA                        : 27722 | 10088
    Append LSA                     : 27722 | 10088
    HA server state                : active

     ***  Active Info. *** 
    DB name                        : testdb
    DB creation time               : 04:29:00.000 PM 11/04/2012 (1352014140)
    EOF LSA                        : 27726 | 2512
    Append LSA                     : 27726 | 2512
    HA server state                : active

     *** Delay in Copying Active Log *** 
    Delayed log page count         : 4
    Estimated Delay                : 0 second(s)

     *** Delay in Applying Copied Log *** 
    Delayed log page count         : 1459
    Estimated Delay                : 22 second(s)

각 상태 정보가 나타내는 항목을 살펴보면 다음과 같다.

* Applied Info.

    *   Committed page : 복제 로그 반영 프로세스에 의해 마지막으로 반영된 트랜잭션의 커밋된 pageid와 offset 정보. 이 값과 "Copied Active Info."의 EOF LSA 값의 차이만큼 복제 반영의 지연이 있다.
    *    Insert Count : 복제 로그 반영 프로세스가 반영한 Insert 쿼리의 개수
    *    Update Count : 복제 로그 반영 프로세스가 반영한 Update 쿼리의 개수
    *    Delete Count : 복제 로그 반영 프로세스가 반영한 Delete 쿼리의 개수
    *    Schema Count : 복제 로그 반영 프로세스가 반영한 DDL 문의 개수
    *    Commit Count : 복제 로그 반영 프로세스가 반영한 트랜잭션의 개수
    *    Fail Count : 복제 로그 반영 프로세스가 반영에 실패한 DML 및 DDL 문의 개수

* Copied Active Info.

    *    DB name : 복제 로그 복사 프로세스가 로그를 복사하는 대상 데이터베이스의 이름
    *    DB creation time : 복제 로그 복사 프로세스가 복사하는 데이터베이스의 생성 시간
        
    *    EOF LSA : 복제 로그 복사 프로세스가 대상 노드에서 복사한 로그의 마지막 pageid와 offset 정보. 이 값과 "Active Info."의 EOF LSA 값의 차이 및 "Copied Active Info."의 Append LSA 값의 차이만큼 로그 복사의 지연이 있다.
    
    *    Append LSA : 복제 로그 복사 프로세스가 디스크에 실제로 쓴 로그의 마지막 pageid와 offset 정보. 이는 EOF LSA보다 작거나 같을 수 있다. 이 값과 "Copied Active Info"의 EOF LSA 값의 차이 만큼 로그 복사의 지연이 있다.
    
    *    HA server state : 복제 로그 복사 프로세스가 로그를 받아오는 데이터베이스 서버 프로세스의 상태. 상태에 대한 자세한 설명은 :ref:`ha-server` 를 참고하도록 한다.

* Active Info.

    *    DB name : **-r** 옵션에 설정한 노드의 데이터베이스의 이름
    *    DB creation time : **-r** 옵션에 설정한 노드의 데이터베이스 생성 시간
    *    EOF LSA : **-r** 옵션에 설정한 노드의 데이터베이스 트랜잭션 로그의 마지막 pageid와 offset 정보. 이 값과 "Copied Active Info."의 EOF LSA 값의 차이 만큼 복제 로그 복사의 지연이 있다.
    
    *    Append LSA : **-r** 옵션에 설정한 노드의 데이터베이스 서버가 디스크에 실제로 쓴 트랜잭션 로그의 마지막 pageid와 offset 정보
    
    *    HA server state : **-r** 옵션에 설정한 노드의 데이터베이스 서버 상태
    
* Delay in Copying Active Log
    *    Delayed log page count: 복사가 지연된 트랜잭션 로그 페이지 개수
    *    Estimated Delay: 트랜잭션 로그 복사 예상 완료 시간
* Delay in Applying Copied Log
    *    Delayed log page count:  반영이 지연된 트랜잭션 로그 페이지 개수
    *    Estimated Delay: 트랜잭션 로그 반영 예상 완료 시간

    
.. _cubrid-changemode:

cubrid changemode
-----------------

CUBRID HA의 서버 상태를 확인하고 변경한다. ::

    cubrid changemode [option] <database-name@node-name>

*   *database-name@node-name* : 확인 또는 변경하고자 하는 서버의 이름을 명시하고 @으로 구분하여 노드 이름을 명시한다.

**cubrid changemode**\에서 사용하는 [options]는 다음과 같다.

.. program:: changemode

.. option:: -m, --mode=MODE

    서버 상태를 변경한다. 
    
    옵션 값으로 **standby**, **maintenance**,  **active** 중 하나를 입력할 수 있다.
    
.. option:: -f, --force

    서버의 상태를 강제로 변경할지 여부를 설정한다. 
    
    현재 서버가 to-be-active 상태일 때 active 상태로 강제 변경하려고 하는 경우에는 반드시 사용하며, 이를 설정하지 않으면 active 상태로 변경되지 않는다. 
    강제 변경 시 복제 노드 간 데이터 불일치가 발생할 수 있으므로 사용하지 않는 것을 권장한다. 
    
.. option:: -t, --timeout=SECOND
    
    기본값 5(초). 노드 상태를 **standby**\에서 **maintenance**\로 변경할 때 진행 중이던 트랜잭션이 정상 종료되기까지 대기하는 시간을 설정한다. 
    
    설정한 시간이 지나도 트랜잭션이 진행 중이면 강제 종료 후 **maintenance**  상태로 변경하고, 설정한 시간 이내에 모든 트랜잭션이 정상 종료되면 즉시 **maintenance** 상태로 변경한다. 

**상태 변경 가능 표**

다음은 현재 상태에 따라 변경할 수 있는 상태를 표시한 표이다.

+-----------------------+--------------------------------+
|                       | 변경할 상태                    |
|                       +--------+---------+-------------+
|                       | active | standby | maintenance |
+-------+---------------+--------+---------+-------------+
| 현재  | standby       | X      | O       | O           |
| 상태  |               |        |         |             |
|       +---------------+--------+---------+-------------+
|       | to-be-standby | X      | X       | X           |
|       +---------------+--------+---------+-------------+
|       | active        | O      | X       | X           |
|       +---------------+--------+---------+-------------+
|       | to-be-active  | O*     | X       | X           |
|       +---------------+--------+---------+-------------+
|       | maintenance   | X      | O       | O           |
+-------+---------------+--------+---------+-------------+

* 서버가 to-be-active 상태일 때 active 상태로 강제 변경하면 복제 노드 간 불일치가 발생할 수 있으므로 관련 내용을 충분히 숙지한 사용자가 아니라면 사용하지 않는 것을 권장한다.

**예시**

다음 예는 localhost 노드의 *testdb* 서버 상태를 maintenance 상태로 변경한다. 이때 진행 중이던 모든 트랜잭션이 정상 종료하기까지 대기하는 시간은 -t 옵션의 기본값인 5초이다. 이 시간 이내에 모든 트랜잭션이 종료되면 즉시 상태를 변경하며, 이 시간이 지나도 진행 중인 트랜잭션이 존재하면 이를 롤백한 후 상태를 변경한다. ::

    $ cubrid changemode -m maintenance testdb@localhost
    The server 'testdb@localhost''s current HA running mode is maintenance.

다음 예는 localhost 노드의 *testdb* 서버의 상태를 조회한다. ::

    $ cubrid changemode testdb@localhost
    The server 'testdb@localhost''s current HA running mode is active.

CUBRID 매니저 HA 모니터링
-------------------------

CUBRID 매니저는 CUBRID 데이터베이스 관리 및 질의 기능을 GUI 환경에서 제공하는 CUBRID 데이터베이스 전용 관리 도구이다. CUBRID 매니저는 CUBRID HA 그룹에 대한 관계도와 서버 상태를 확인할 수 있는 HA 대시보드를 제공한다. 자세한 설명은 `cubrid 매니저 매뉴얼 <http://www.cubrid.org/wiki_tools/entry/cubrid-manager-manual_kr>`_ 을 참고한다.

HA 구성 형태
============

CUBRID HA 구성에는 HA 기본 구성, 다중 슬레이브 노드 구성, 부하 분산 구성, 다중 스탠바이 서버 구성의 네 가지 형태가 있다. 다음 표에서 M은 마스터 노드, S는 슬레이브 노드, R은 레플리카 노드를 의미한다.

+---------------+------------------+-----------------------------------------------------------------------------------------------------------------------+
| 구성          | 노드 구성(M:S:R) | 특징                                                                                                                  |
+===============+==================+=======================================================================================================================+
| HA 기본 구성  | 1:1:0            | CUBRID HA의 가장 기본적인 구성으로, 하나의 마스터 노드와 하나의 슬레이브 노드로 구성되어 CUBRID HA 고유의 기능인      |
|               |                  | 가용성을 제공한다.                                                                                                    |
+---------------+------------------+-----------------------------------------------------------------------------------------------------------------------+
| 다중 슬레이브 | 1:N:0            | 슬레이브 노드를 여러 개 두어 가용성을 높인 구성이다. 단, 다중 장애 상황에서 CUBRID HA 그룹 내의 데이터가              |
| 노드 구성     |                  | 동일하지 않은 상황이 발생할 수 있으므로 주의해야 한다.                                                                |
+---------------+------------------+-----------------------------------------------------------------------------------------------------------------------+
| 부하 분산     | 1:1:N            | HA 기본 구성에 레플리카 노드를 여러 개 둔다. 읽기 서비스의 부하를 분산할 수 있으며, 다중 슬레이브 노드 구성에 비해    |
| 구성          |                  | HA로 인한 부담이 적다. 레플리카 노드는 failover되지 않으므로 주의해야 한다.                                           |
+---------------+------------------+-----------------------------------------------------------------------------------------------------------------------+
| 다중 스탠바이 | 1:1:0            | HA 기본 구성과 노드 구성은 같으나 여러 서비스의 슬레이브 노드가 하나의 물리적인 서버에 설치되어 서비스된다.           |
| 서버 구성     |                  |                                                                                                                       |
+---------------+------------------+-----------------------------------------------------------------------------------------------------------------------+

HA 기본 구성
------------

CUBRID HA의 가장 기본적인 구성으로, 하나의 마스터 노드와 하나의 슬레이브 노드로 구성된다.

CUBRID HA 고유의 기능인 장애 시 무중단(nonstop) 서비스 기능에 초점을 맞춘 구성으로, 작은 서비스에서 적은 리소스를 투입하여 구성할 수 있다. HA 기본 구성은 하나의 마스터 노드와 하나의 슬레이브 노드로 서비스를 제공하므로, 읽기 부하를 분산하려면 다중 슬레이브 노드 구성 또는 부하 분산 구성이 좋다. 또한, 슬레이브 노드 또는 레플리카 노드 등의 특정 노드에 읽기 전용으로 접속하려면 Read Only 브로커 또는 Preferred Host Read Only 브로커를 구성한다. 브로커 구성에 대한 설명은 :ref:`duplexing-brokers` 를 참고한다.

**노드 설정 예시**

.. image:: /images/image30.png

HA 기본 구성의 각 노드는 다음과 같이 설정한다.

*   **node A** (마스터 노드)

    * **cubrid.conf** 파일의 **ha_mode** 를 **on** 으로 설정한다. ::

        ha_mode=on

    * 다음은 **cubrid_ha.conf** 파일의 설정 예이다. ::

        ha_port_id=59901
        ha_node_list=cubrid@nodeA:nodeB
        ha_db_list=testdb

*   **node B** (슬레이브 노드) : *node A* 와 동일하게 설정한다.

브로커 노드의 **databases.txt** 파일에는 **db-host** 에 HA로 구성된 호스트의 목록을 우선순위에 따라 순서대로 설정해야 한다. 다음은 **databases.txt** 파일의 예이다. ::

    #db-name    vol-path                  db-host       log-path       lob-base-path
    testdb     /home/cubrid/DB/testdb1   nodeA:nodeB   /home/cubrid/DB/testdb/log file:/home/cubrid/DB/testdb/lob

**cubrid_broker.conf** 파일은 브로커를 어떻게 구성하느냐에 따라 다양하게 설정할 수 있으며 **databases.txt** 파일과 함께 별도의 장비로 구성하여 설정할 수도 있다.

다음 예는 각 노드에 RW 브로커를 설정한 경우이며 *node A*, *node B* 둘 다 같은 값으로 구성한다. ::

    [%RW_broker]
    ...
     
    # Broker mode setting parameter
        ACCESS_MODE             =RW
    
**응용 프로그램 연결 설정**

환경 설정의 :ref:`ha-jdbc-conf`, :ref:`ha-cci-conf`, :ref:`ha-php-conf` 을 참고한다.

**참고**

이와 같은 구성에서 트랜잭션 로그의 이동 경로를 중심으로 살펴보면 다음과 같다.

.. image:: /images/image31.png

다중 슬레이브 노드 구성
-----------------------

다중 슬레이브 노드 구성은 한 개의 마스터 노드와 여러 개의 슬레이브 노드를 두어 CUBRID의 서비스 가용성을 높인 구성이다.

CUBRID HA 그룹 내의 모든 노드에서 복제 로그 복사 프로세스와 복제 로그 반영 프로세스가 구동되므로 복제 로그를 복사하는 부하가 생긴다. 따라서 CUBRID HA 그룹 내의 모든 노드는 네트워크 및 디스크 사용률이 높다.

HA로 구성된 노드 수가 많으므로 CUBRID HA 그룹 내의 여러 노드에 장애가 발생해도 하나의 노드만 있으면 읽기 쓰기 서비스를 제공할 수 있다.

다중 슬레이브 노드 구성에서 failover가 일어날 때 마스터 노드가 될 노드는 **ha_node_list** 에 정의한 순서에 따라 지정된다. 만약 **ha_node_list** 값이 nodeA:nodeB:nodeC이고 마스터 노드가 *node A* 이면, 마스터 노드에 장애가 발생했을 때 *node B* 가 마스터 노드가 된다.

**노드 설정 예시**

.. image:: /images/image32.png

다중 슬레이브 구성의 각 노드는 다음과 같이 설정한다.

* **node A** (마스터 노드)

    * **cubrid.conf** 파일의 **ha_mode** 를 **on** 으로 설정한다. ::

        ha_mode=on

    * 다음은 **cubrid_ha.conf** 파일의 설정 예이다. ::

        ha_port_id=59901
        ha_node_list=cubrid@nodeA:nodeB:nodeC
        ha_db_list=testdb

* **node B** (슬레이브 노드): *node A* 와 동일하게 설정한다.

* **node C** (슬레이브 노드): *node A* 와 동일하게 설정한다.

브로커 노드의 **databases.txt** 파일에는 **db-host** 에 HA 구성된 호스트의 목록을 우선순위에 따라 순서대로 설정해야 한다. 다음은 **databases.txt** 파일의 예이다. ::

    #db-name    vol-path                  db-host             log-path       lob-base-path
    testdb     /home/cubrid/DB/testdb1   nodeA:nodeB:nodeC   /home/cubrid/DB/testdb/log file:/home/cubrid/DB/testdb/lob

**cubrid_broker.conf** 파일은 브로커를 어떻게 구성하느냐에 따라 다양하게 설정할 수 있으며 **databases.txt** 파일과 함께 별도의 장비로 구성하여 설정할 수도 있다. 예시에서는 *node A*, *node B*, *node C* 에 RW 브로커를 설정하였다.

다음은 *node A*, *node B*, *node C* 의 **cubrid_broker.conf** 의 예이다. ::

    [%RW_broker]
    ...
     
    # Broker mode setting parameter
    ACCESS_MODE             =RW

**응용 프로그램 연결 설정**

*node A*, *node B* 또는 *node C* 에 있는 브로커 중 하나와 연결한다.

.. code-block:: java

    Connection connection = DriverManager.getConnection(
        "jdbc:CUBRID:nodeA:33000:testdb:::?charSet=utf-8&altHosts=nodeB:33000,nodeC:33000", "dba", "");

기타 자세한 사항은 환경 설정의 :ref:`ha-jdbc-conf`, :ref:`ha-cci-conf`, :ref:`ha-php-conf` 을 참고한다.

**주의 사항**

이 구성은 다중 장애 시 CUBRID HA 그룹 내의 데이터가 동일하지 않은 상황이 발생할 수 있으며, 그 예는 다음과 같다.

*   두 번째 슬레이브 노드가 재시작으로 인해 복제가 지연될 때 첫 번째 슬레이브로 failover되는 상황
*   빈번한 failover로 인해 새로운 마스터 노드의 복제 반영이 완료되지 않았을 때 다시 failover가 일어나는 상황

이외에 복제 로그 복사 프로세스의 모드가 ASYNC이면 CUBRID HA 그룹 내의 데이터가 동일하지 않은 상황이 발생할 수 있다.

이와 같이 CUBRID HA 그룹 내의 데이터가 동일하지 않은 상황이 발생하면, :ref:`rebuilding-replication` 을 통해 CUBRID HA 그룹 내의 데이터를 동일하게 맞춰야 한다.

**참고**

이와 같은 구성에서 트랜잭션 로그의 이동 경로를 중심으로 살펴보면 다음과 같다.

.. image:: /images/image33.png

부하 분산 구성
--------------

부하 분산 구성은 HA 구성(한 개의 마스터 노드와 한 개의 슬레이브 노드)에 여러 개의 레플리카 노드를 두어 CUBRID 서비스의 가용성을 높이고, 많은 읽기 부하를 분산하여 처리할 수 있는 구성이다.

레플리카 노드들은 HA 구성에 포함된 노드들로부터 복제 로그를 받아 데이터를 동일하게 유지하고, HA 구성에 포함된 노드들은 레플리카 노드에서 복제 로그를 받지 않으므로 다중 슬레이브 구성에 비해 네트워크 및 디스크 사용률이 낮다.

레플리카 노드는 HA 구성에 포함되지 않으므로 HA 구성 내의 모든 노드에 장애가 발생해도 failover되지 않고 읽기 서비스만 제공한다.

**노드 설정 예시**

.. image:: /images/image34.png

부하 분산 구성의 각 노드는 다음과 같이 설정한다.

* **node A** (마스터 노드)

    * **cubrid.conf** 파일의 **ha_mode** 를 **on** 으로 설정한다. ::

        ha_mode=on

    * 다음은 **cubrid_ha.conf** 파일의 설정 예이다. ::

        ha_port_id=12345
        ha_node_list=cubrid@nodeA:nodeB 
        ha_replica_list=cubrid@nodeC:nodeD
        ha_db_list=testdb

* **node B** (슬레이브 노드): *node A* 와 동일하게 설정한다.

* **node C** (레플리카 노드)

    * **cubrid.conf** 파일의 **ha_mode** 를 **replica** 로 설정한다. ::

        ha_mode=replica

    * **cubrid_ha.conf** 파일은 *node A* 와 동일하게 설정한다.

*   **node D** (레플리카 노드): *node C* 와 동일하게 설정한다.

브로커 노드의 **databases.txt** 파일에는 브로커의 용도에 맞게 HA 또는 부하 분산 서버와 연결될 수 있도록 DB 서버 호스트의 목록을 순서대로 설정해야 한다.

다음은*node A* 와 *node B* 의 **databases.txt** 파일의 예이다. ::

    #db-name    vol-path                  db-host       log-path             lob-base-path
    testdb     /home/cubrid/DB/testdb1   nodeA:nodeB   /home/cubrid/DB/testdb/log file:/home/cubrid/CUBRID/testdb/lob

다음은 *node C* 의 **databases.txt** 파일의 예이다. ::

    #db-name    vol-path                  db-host       log-path             lob-base-path
    testdb     /home/cubrid/DB/testdb   nodeC   /home/cubrid/DB/testdb/log        file:/home/cubrid/CUBRID/testdb/lob

다음은 *node D* 의 **databases.txt** 파일의 예이다. ::

    #db-name    vol-path                  db-host       log-path             lob-base-path
    testdb     /home/cubrid/DB/testdb   nodeD   /home/cubrid/DB/testdb/log file:/home/cubrid/CUBRID/testdb/lob

**cubrid_broker.conf** 파일은 브로커를 어떻게 구성하느냐에 따라 다양하게 설정할 수 있으며 **databases.txt** 파일과 함께 별도의 장비로 구성하여 설정할 수도 있다.

예시에서는 *node A*, *node B* 에 RW 브로커를 설정하고, *node C*, *node D* 에 PHRO 브로커를 설정하였다.

다음은 *node A* 와 *node B* 의 **cubrid_broker.conf** 의 예이다. ::

    [%RW_broker]
    ...
     
    # Broker mode setting parameter
    ACCESS_MODE             =RW

다음은 *node C* 의 **cubrid_broker.conf** 의 예이다.  ::

    [%PHRO_broker]
    ...
     
    # Broker mode setting parameter
    ACCESS_MODE             =PHRO
    PREFERRED_HOSTS         =nodeC:nodeD


다음은 *node D* 의 **cubrid_broker.conf** 의 예이다. ::

    [%PHRO_broker]
    ...
     
    # Broker mode setting parameter
    ACCESS_MODE             =PHRO
    PREFERRED_HOSTS         =nodeD:nodeC


**응용 프로그램 연결 설정**

읽기 쓰기로 접속하기 위한 응용 프로그램은 *node A* 또는 *node B* 에 있는 브로커에 연결한다. 다음은 JDBC 응용 프로그램의 예이다.

.. code-block:: java

    Connection connection = DriverManager.getConnection(
        "jdbc:CUBRID:nodeA:33000:testdb:::?charSet=utf-8&altHosts=nodeB:33000", "dba", "");

읽기 전용으로 접속하기 위한 응용 프로그램은 *node C* 또는 *node D* 에 있는 브로커에 연결한다. 다음은 JDBC 응용 프로그램의 예이다.

.. code-block:: java

    Connection connection = DriverManager.getConnection(
        "jdbc:CUBRID:nodeC:33000:testdb:::?charSet=utf-8&altHosts=nodeD:33000", "dba", "");

기타 자세한 사항은 환경 설정의 :ref:`ha-jdbc-conf`, :ref:`ha-cci-conf`, :ref:`ha-php-conf` 을 참고한다.

**참고**

이와 같은 구성에서 트랜잭션 로그의 이동 경로를 중심으로 살펴보면 다음과 같다.

.. image:: /images/image35.png

다중 스탠바이 서버 구성
-----------------------

한 개의 마스터 노드와 한 개의 슬레이브 노드로 구성되나, 여러 서비스의 슬레이브 노드를 하나의 물리적인 서버에 구성한다.

매우 작은 서비스에서 슬레이브 노드로 읽기 부하를 받지 않아도 되는 경우를 위한 것으로, CUBRID 서비스의 가용성만을 위한 구성이다. 따라서 failover 후 장애가 발생했던 마스터 노드가 복구되면 부하를 다시 마스터 노드로 옮겨 오도록 하여 슬레이브 노드들이 들어있는 서버의 부하를 최소화해야 한다.

.. image:: /images/image36.png

**노드 설정 예시**

HA 기본 구성의 각 노드는 다음과 같이 설정한다.

* **node AM**, **node AS** : 두 노드는 동일하게 설정한다.

    * **cubrid.conf** 파일의 **ha_mode** 를 **on** 으로 설정한다. ::

        ha_mode=on

    * 다음은 **cubrid_ha.conf** 파일의 설정 예이다. ::

        ha_port_id=10000
        ha_node_list=cubridA@Host1:Host5
        ha_db_list=testdbA1,testdbA2

* **node BM**, **node BS** : 두 노드는 동일하게 설정한다.

    * **cubrid.conf** 파일의 **ha_mode** 를 **on** 으로 설정한다. ::

        ha_mode=on

    * 다음은 **cubrid_ha.conf** 파일의 설정 예이다. ::

        ha_port_id=10001
        ha_node_list=cubridB@Host2:Host5
        ha_db_list=testdbB1,testdbB2

* **node CM**, **node CS** : 두 노드는 동일하게 설정한다.

    * **cubrid.conf** 파일의 **ha_mode** 를 **on** 으로 설정한다. ::

        ha_mode=on

    * 다음은 **cubrid_ha.conf** 파일의 설정 예이다. ::

        ha_port_id=10002
        ha_node_list=cubridC@Host3:Host5
        ha_db_list=testdbC1,testdbC2

* **node DM**, **node DS** : 두 노드는 동일하게 설정한다.

    * **cubrid.conf** 파일의 **ha_mode** 를 **on** 으로 설정한다.

        ha_mode=on

    * 다음은 **cubrid_ha.conf** 파일의 설정 예이다. ::

        ha_port_id=10003
        ha_node_list=cubridD@Host4:Host5
        ha_db_list=testdbD1,testdbD2

HA 제약 사항
============

**지원 플랫폼 및 기타**

현재 CUBRID HA 기능은 Linux 계열에서만 사용할 수 있다. CUBRID HA 그룹의 모든 노드들은 반드시 동일한 플랫폼으로 구성해야 한다.

**테이블 기본키(primary key)**

CUBRID HA는 마스터 노드의 서버에서 생성되는 기본키 기반의 복제 로그를 슬레이브 노드에 복제 후 반영하는 방식(transaction log shipping)으로 노드 간 데이터를 동기화하므로 기본키가 설정된 테이블에 대해서만 CUBRID HA 그룹 내의 노드 간 데이터 동기화가 가능하다.

CUBRID HA 그룹 내의 노드 간 특정 테이블의 데이터가 동기화되지 않는다면 해당 테이블에 적절한 기본키가 설정되어 있는지 확인해야 한다.

분할 테이블에서 **PROMOTE** 문에 의해 일부 분할이 승격된 테이블은 모든 데이터를 슬레이브에 복제하지만, 기본 키를 가지지 않게 되므로 이후 마스터에서 해당 테이블의 데이터를 수정해도 슬레이브에 반영되지 않음에 주의한다.

**테이블 트리거(trigger), 자바 저장 프로시저(java stored procedure)**

CUBRID HA에서 트리거 및 자바 저장 프로시저를 사용할 경우 마스터 노드에서 이미 수행된 트리거 또는 자바 저장 프로시저를 슬레이브 노드에서 중복 수행하므로 CUBRID HA 그룹 내의 노드 간 데이터 불일치가 발생할 수 있다.

따라서 CUBRID HA에서는 트리거 및 자바 저장 프로시저를 사용하지 않도록 한다.

**메서드 및 CUBRID 매니저**

CUBRID HA는 복제 로그를 기반으로 CUBRID HA 그룹 내의 노드 간 데이터를 동기화하므로 복제 로그를 생성하지 않는 메서드를 사용하거나 CUBRID 매니저를 통해 **NOT NULL** 옵션 설정 작업 수행 시 CUBRID HA 그룹 내 노드 간 데이터 불일치가 발생할 수 있다.

따라서 CUBRID HA 환경에서는 메서드 사용을 권장하지 않으며, CUBRID 매니저에서 질의 처리기 외의 다른 메뉴 사용 또한 권장하지 않는다.

**UPDATE STATISTICS 문**

통계 정보를 갱신하는 **UPDATE STATISTICS** 문은 슬레이브 노드에 복제되지 않는다.

**stand-alone 모드**

CUBRID의 stand-alone 모드에서 수행한 작업에 대해서는 복제 로그가 생성되지 않는다. 따라서 stand-alone 모드로 csql 등을 통해 작업 수행 시 CUBRID HA 그룹 내 노드 간 데이터 불일치가 발생할 수 있다.

**시리얼 캐시(serial cache)**

시리얼 캐시는 성능 향상을 위해 시리얼 정보를 조회하거나 갱신할 때 Heap에 접근하지 않고 복제 로그를 생성하지 않는다. 따라서 시리얼 캐시를 사용하는 경우 CUBRID HA 그룹 내 노드 간 시리얼의 현재 값이 일치하지 않는다.

**cubrid backupdb -r**

이는 지정한 데이터베이스를 백업하는 명령으로 **-r** 옵션을 사용하면 백업을 수행한 후 복구에 필요하지 않은 로그를 삭제한다. 하지만 이 옵션으로 인해 로그가 사라지는 경우 CUBRID HA 그룹 내의 노드 간 데이터 불일치가 발생할 수 있으므로 **-r** 옵션을 사용하지 않아야 한다

**INCR/DECR 함수**

HA 구성의 슬레이브 노드에서 클릭 카운터 함수인 ::func:`INCR` / :func:`DECR` 함수를 사용하면 오류를 반환한다.

**LOB(BLOB/CLOB) 타입**

CUBRID HA에서 **LOB** 칼럼 메타 데이터(Locator)는 복제되고, **LOB** 데이터는 복제되지 않는다. 따라서 **LOB** 타입 저장소가 로컬에 위치할 경우, 슬레이브 노드 또는 failover 이후 마스터 노드에서 해당 칼럼에 대한 작업을 허용하지 않는다.

운영 시나리오
=============

슬레이브 노드 신규 구축 시나리오
--------------------------------

이 시나리오는 마스터 노드 한 대로만 운영하는 도중 슬레이브 노드를 새로 구축하여 마스터 노드와 슬레이브 노드를 1:1로 구성하는 시나리오이다. 기본 키가 있는 테이블만 복제된다는 점에 반드시 주의한다. 그리고, 마스터 노드와 슬레이브 노드의 볼륨 디렉터리들은 모두 일치해야 한다는 점에 주의한다.

데이터베이스는 **cubrid createdb testdb -L $CUBRID_DATABASES/testdb/log** 명령으로 생성되었다고 가정한다. 이때 백업 파일의 저장 위치는 별도의 옵션으로 지정하지 않으면 $CUBRID_DATABASES/testdb 디렉터리가 기본이 된다.

위의 사항들을 염두에 두고 다음의 순서로 작업한다.

#. 마스터 노드 서비스 중지 ::

    [nodeA]$ cubrid service stop

#. 마스터 노드 HA 설정, 슬레이브 노드 HA 설정

    * 마스터 노드와 슬레이브 노드가 동일하게 **$CUBRID/conf/cubrid.conf** 설정 ::

        …
        [common]
        service=server,broker,manager
        # 서비스 시작 시 구동될 데이터베이스 이름 추가
        server=testdb
        …
        # HA 구성 시 추가 (Logging parameters)
        log_max_archives=100
        force_remove_log_archives=no
         
        # HA 구성 시 추가 (HA 모드)
        ha_mode=on


    * 마스터 노드와 슬레이브 노드가 동일하게 **$CUBRID/conf/cubrid_ha.conf** 설정 ::

        [common]
        ha_port_id=59901
        ha_node_list=cubrid@nodeA:nodeB
        ha_db_list=testdb
        ha_copy_sync_mode=sync:sync
        ha_apply_max_mem_size=500

    * 마스터 노드와 슬레이브 노드가 동일하게 **$CUBRID_DATABASES/databases.txt** 설정 ::

        #db-name    vol-path        db-host     log-path     lob-base-path
        testdb       /home/cubrid/DB/testdb nodeA:nodeB   /home/cubrid/DB/testdb/log  file:/home/cubrid/DB/testdb/lob

    * 슬레이브 노드에 데이터베이스 디렉터리 생성 ::
    
        [nodeB]$ cd $CUBRID_DATABASES
        [nodeB]$ mkdir testdb
    
    * 슬레이브 노드에 로그 디렉터리 생성(마스터 노드와 같은 위치에 생성) ::

        [nodeB]$ cd $CUBRID_DATABASES/testdb
        [nodeB]$ mkdir log

#. 마스터 노드의 데이터베이스를 백업하고, 슬레이브 노드에 백업 파일을 복사. 마스터 노드에서 백업 파일의 저장 위치는 별도의 지정이 없으면 *testdb* 의 로그 디렉터리가 되며, 슬레이브 노드에도 마스터 노드와 같은 위치에 백업 파일을 복사한다. 아래에서 *testdb* _bk0v000은 백업 볼륨 파일, *testdb* _bkvinf는 백업 볼륨 정보 파일이다. ::

    [nodeA]$ cubrid backupdb -z -S testdb
    Backup Volume Label: Level: 0, Unit: 0, Database testdb, Backup Time: Thu Apr 19 16:05:18 2012
    [nodeA]$ cd $CUBRID_DATABASES/testdb/log
    [nodeA]$ scp testdb_bk* cubrid_usr@nodeB:/home/cubrid_usr/CUBRID/databases/testdb/log
    cubrid_usr@nodeB's password:
    testdb_bk0v000                            100% 6157KB   6.0MB/s   00:00
    testdb_bkvinf                             100%   66     0.1KB/s   00:00

#. 슬레이브 노드에서 데이터베이스 복구. 이때, 마스터 노드와 슬레이브 노드의 볼륨 경로가 반드시 같아야 한다. ::

    [nodeB]$ cubrid restoredb -B $CUBRID_DATABASES/testdb/log demodb

#. 마스터 노드 시작 ::

    [nodeA]$ cubrid heartbeat start

#. 마스터 노드가 시작 완료되었음을 확인한 후, 슬레이브 노드 시작. 아래에서 *nodeA* 가 to-be-master에서 master로 변경되면 마스터 노드가 정상 구동된 것이다. ::

    [nodeA]$ cubrid heartbeat status
    @ cubrid heartbeat status
     
     HA-Node Info (current nodeA, state master)
       Node nodeB (priority 2, state unknown)
       Node nodeA (priority 1, state master)
     
     HA-Process Info (master 123, state master)
     
       Applylogdb testdb@localhost:/home1/cubrid/DB/tdb01_nodeB (pid 234, state registered)
       Copylogdb testdb@nodeB:/home1/cubrid/DB/tdb01_nodeB (pid 345, state registered)
       Server tdb01 (pid 456, state registered_and_to_be_active)
     
    [nodeB]$ cubrid heartbeat start

#. 마스터 노드, 슬레이브 노드의 HA 구성이 정상 동작하는지 확인 ::

    [nodeA]$ csql -u dba testdb@localhost -c"create table tbl(i int primary key);insert into tbl values (1),(2),(3)"
     
    [nodeB]$ csql -u dba testdb@localhost -c"select * from tbl"
     
    === <Result of SELECT Command in Line 1> ===
     
                i
    =============
                1
                2
                3

읽기 쓰기 서비스 중 운영 시나리오
---------------------------------

이 운영 시나리오는 서비스의 읽기 쓰기에 영향을 받지 않으므로, CUBRID 운영으로 인해 서비스에 미치는 영향이 매우 작다. 읽기 쓰기 서비스 중의 운영 시나리오는 failover가 일어나는 경우와 그렇지 않은 경우로 나눌 수 있다.

**failover가 필요 없는 운영 시나리오**

다음 작업은 CUBRID HA 그룹 내의 노드를 종료하고 다시 구동하지 않고 바로 수행할 수 있다.

+-------------------------------------+--------------------------------------------------------------------+---------------------------------------------------------------------------------------+
| 대표적인 운영 작업                  | 시나리오                                                           | 고려 사항                                                                             |
+=====================================+====================================================================+=======================================================================================+
| 온라인 백업                         | 운영 중 마스터 노드와 슬레이브 노드에서 각각 운영 작업을 수행한다. | 운영 작업으로 인해 마스터 노드의 트랜잭션이 지연될 수 있으므로 주의해야 한다.         |
+-------------------------------------+--------------------------------------------------------------------+---------------------------------------------------------------------------------------+
| 스키마 변경(기본키 변경 작업 제외), | 마스터 노드에서만 운영 작업하면 자동으로 슬레이브 노드로           | 운영 작업이 마스터 노드에서 완료된 후 슬레이브 노드로 복제 로그가 복사되고 그 후부터  |
| 인덱스 변경, 권한 변경              | 복제 반영한다.                                                     | 슬레이브 노드에 반영이 되므로 운영 작업 시간이 2배 소요 된다.                         |
|                                     |                                                                    | 스키마 변경은 반드시 중간에 failover 없이 진행해야 한다.                              |
|                                     |                                                                    | 스키마 변경을 제외한 인덱스 변경, 권한 변경은 운영 작업 소요 시간이 문제가 되는 경우, |
|                                     |                                                                    | 각 노드를 정지한 후 독립 모드(예:**csql** 유틸리티의 **-S**                           |
|                                     |                                                                    | 옵션)를 통해 수행할 수 있다.                                                          |
+-------------------------------------+--------------------------------------------------------------------+---------------------------------------------------------------------------------------+
| 볼륨 추가                           | HA 구성과 별개로 각 DB에서 운영 작업을 수행한다.                   | 운영 작업으로 인해 마스터 노드의 트랜잭션이 지연될 수 있으므로 주의해야 한다.         |
|                                     |                                                                    | 운영 작업 소요 시간이 문제가 되는 경우 각 노드를 정지한 후 독립 모드(예:              |
|                                     |                                                                    | **cubrid addvoldb** 유틸리티의 **-S** 옵션)를 통해 수행할 수 있다.                    |
+-------------------------------------+--------------------------------------------------------------------+---------------------------------------------------------------------------------------+
| 장애 노드 서버 교체                 | 장애 발생 후 실행 중인 CUBRID HA 그룹의 재시작 없이 교체한다.      | CUBRID HA 그룹 내 설정의 ha_node_list에 장애 노드가 등록되어 있는 경우로, 교체 시     |
|                                     |                                                                    | 노드명 등이 변경되지 않아야 한다.                                                     |
+-------------------------------------+--------------------------------------------------------------------+---------------------------------------------------------------------------------------+
| 장애 브로커 서버 교체               | 장애 발생 후 실행 중인 브로커의 재시작 없이 교체한다.              | 클라이언트에서 교체된 브로커로의 연결은 URL 문자열에 설정된 rcTime 값에 의한다.       |
+-------------------------------------+--------------------------------------------------------------------+---------------------------------------------------------------------------------------+
| DB 서버 증설                        | 기존에 구성된 CUBRID HA 그룹의 재시작 없이 설정 변경               | 변경된 설정 정보를 로딩하여 추가/삭제된 노드에 해당하는                               |
|                                     | (ha_node_list, ha_replica_list) 후 **cubrid heartbeat reload**     | **copylogdb/applylogdb**                                                              |
|                                     | 를 각 노드에서 수행한다.                                           | 프로세스를 시작 또는 정지한다.                                                        |
+-------------------------------------+--------------------------------------------------------------------+---------------------------------------------------------------------------------------+
| 브로커 서버 증설                    | 기존 브로커들의 재시작 없이 추가된 브로커를 구동한다.              | 클라이언트가 추가된 브로커로 연결되기 위해서는 URL 문자열을 수정해야 한다.            |
+-------------------------------------+--------------------------------------------------------------------+---------------------------------------------------------------------------------------+

**failover가 필요한 운영 시나리오**

다음 작업은 CUBRID HA 그룹 내의 노드를 종료하고 운영 작업을 완료한 후 구동해야 한다.

+---------------------------+--------------------------------------+----------------------------------------------+
| 대표적인 운영 작업        | 시나리오                             | 고려 사항                                    |
+===========================+======================================+==============================================+
| DB 서버 설정 변경         | **cubrid.conf** 의 설정이 변경되면   |                                              |
|                           | 설정 변경된 노드를 재시작 한다.      |                                              |
+---------------------------+--------------------------------------+----------------------------------------------+
| 브로커 설정 변경,         | **cubrid_broker.conf** 의 설정이     |                                              |
| 브로커 추가, 브로커 삭제  | 변경되면 설정 변경돤 브로커를        |                                              |
|                           | 재시작 한다.                         |                                              |
+---------------------------+--------------------------------------+----------------------------------------------+
| DBMS 버전 패치            | HA 그룹 내 노드와 브로커들을 각각    | 버전 패치는 CUBRID의 내부 프로토콜, 볼륨 및  |
|                           | 버전 패치 후 재시작 한다.            | 로그의 변경이 없는 것이다.                   |
+---------------------------+--------------------------------------+----------------------------------------------+

읽기 서비스 중 운영 시나리오
----------------------------

이 운영 시나리오는 읽기 서비스만 가능하도록 하여 운영 작업을 수행한다. 서비스의 읽기 서비스만을 허용하거나 브로커의 모드 설정을 Read Only로 동적 변경해야 한다. 읽기 서비스 중의 운영 시나리오는 failover가 일어나는 경우와 그렇지 않은 경우로 나눌 수 있다.

**failover가 필요 없는 운영 시나리오**

다음 작업은 CUBRID HA 그룹 내의 노드를 종료하고 다시 구동하지 않고 바로 수행할 수 있다.

+-------------------------------------+------------------------------------------+--------------------------------------------------------------------------------------------------------------------------+
| 대표적인 운영 작업                  | 시나리오                                 | 고려 사항                                                                                                                |
+=====================================+==========================================+==========================================================================================================================+
| 스키마 변경(기본키 변경)            | 마스터 노드에서만 운영 작업하면 자동으로 | 기본키를 변경하려면 기본키를 삭제하고 다시 추가해야 한다. 따라서 기본키 기반의 복제 로그를 반영하는 HA 내부 구조 상      |
|                                     | 슬레이브 노드로 복제 반영한다.           | 복제 반영이 일어나지 않을 수 있으므로, 반드시 읽기 서비스 중에 운영 작업을 수행해야 한다.                                |
+-------------------------------------+------------------------------------------+--------------------------------------------------------------------------------------------------------------------------+
| 스키마 변경(기본키 변경 작업 제외), | 마스터 노드에서만 운영 작업하면 자동으로 | 운영 작업이 마스터 노드에서 완료된 후 슬레이브 노드로 복제 로그가 복사되고 그 후부터 슬레이브 노드에 반영이 되므로       |
| 인덱스 변경, 권한 변경              | 슬레이브 노드로 복제 반영한다.           | 운영 작업 시간이 2배 소요 된다. 스키마 변경은 반드시 중간에 failover 없이 진행해야 한다.                                 |
|                                     |                                          | 스키마 변경을 제외한 인덱스 변경, 권한 변경은 운영 작업 소요 시간이 문제가 되는 경우, 각 노드를 정지한 후 독립 모드      |
|                                     |                                          | (예: **csql** 유틸리티의 **-S** 옵션)를 통해 수행할 수 있다.                                                             |
+-------------------------------------+------------------------------------------+--------------------------------------------------------------------------------------------------------------------------+

**failover가 필요한 운영 시나리오**

다음 작업은 CUBRID HA 그룹 내의 노드를 종료하고 운영 작업을 완료한 후 구동해야 한다.

+----------------------------------+------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------+
| 대표적인 운영 작업               | 시나리오                                       | 고려 사항                                                                                                                     |
+==================================+================================================+===============================================================================================================================+
| DBMS 버전 업그레이드             | CUBRID HA 그룹 내 노드와 브로커들을 각각 버전  | 버전 업그레이드는 CUBRID의 내부 프로토콜, 볼륨 및 로그의 변경이 있는 것이다.                                                  |
|                                  | 업그레이드 후 재시작 한다.                     | 업그레이드 중의 브로커 및 서버는 프로토콜, 볼륨 및 로그 등이 서로 맞지 않는 두 버전이 존재하게 되므로 업그레이드 전후의       |
|                                  |                                                | 클라이언트 및 브로커는 각각의 버전에 맞는 브로커 및 서버에 연결되도록 운영 작업을 수행해야 한다.                              |
+----------------------------------+------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------+
| 대량의 데이터 작업               | 작업할 노드를 정지하고 운영 작업을 수행한 후   | 분할하여 작업할 수 없는 대량의 데이터 작업이 이에 해당한다.                                                                   |
| (INSERT/UPDATE/DELETE)           | 노드를 구동한다.                               |                                                                                                                               |
+----------------------------------+------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------+


서비스 정지 후 운영 시나리오
----------------------------

이 운영 시나리오는 CUBRID HA 그룹 내의 모든 노드들을 정지 후 운영 작업을 수행해야 한다.

+---------------------+-------------------------------------------+--------------------------------+
| 대표적인 운영 작업  | 시나리오                                  | 고려 사항                      |
+=====================+===========================================+================================+
| DB 서버의 호스트명  | CUBRID HA 그룹 내의 모든 노드를 정지하고  | 호스트명 변경 시 각 브로커의   |
| 및 IP 변경          | 운영 작업 후 구동한다.                    | **databases.txt**              |
|                     |                                           | 도 변경한 후                   |
|                     |                                           | **cubrid broker reset**        |
|                     |                                           | 으로 브로커의 연결을 리셋한다. |
+---------------------+-------------------------------------------+--------------------------------+

복제 불일치 감지 및 재구축
==========================

복제 불일치 감지
----------------

마스터 노드와 슬레이브 노드의 데이터가 일치하지 않는 복제 노드 간 데이터 불일치 현상은 다음과 같은 과정을 통해 어느 정도 감지할 수 있다. 그러나, 마스터 노드와 슬레이브 노드의 데이터를 서로 직접 비교해보는 방법보다 더 정확한 확인 방법은 없음에 주의해야 한다. 복제 불일치 상태라는 판단이 서면, 마스터 노드의 데이터베이스를 슬레이브 노드에 새로 구축해야 한다(:ref:`rebuilding-replication` 참고).

* 슬레이브 노드에서 **cubrid applyinfo** 를 실행하여 "Fail count" 값을 확인한다. "Fail count"가 0이면, 복제에 실패한 트랜잭션이 없다고 볼 수 있다(:ref:`cubrid-applyinfo` 참고). ::

    [nodeB]$ cubrid applyinfo -L /home/cubrid/DB/testdb_nodeA -r nodeA -a testdb
     
     *** Applied Info. ***
    Committed page                 : 1913 | 2904
    Insert count                   : 645
    Update count                   : 0
    Delete count                   : 0
    Schema count                   : 60
    Commit count                   : 15
    Fail count                     : 0
    ...

* 슬레이브 노드에서 복제 로그의 복사 지연 여부를 확인하기 위해, **cubrid applyinfo** 를 실행하여 "Copied Active Info."의 "Append LSA" 값과 "Active Info."의 "Append LSA" 값을 비교한다. 이 값이 큰 차이를 보인다면, 복제 로그가 슬레이브 노드에 복사되는데 지연이 있다는 의미이다(:ref:`cubrid-applyinfo` 참고). ::

    [nodeB]$ cubrid applyinfo -L /home/cubrid/DB/testdb_nodeA -r nodeA -a testdb
 
    ...
     
     *** Copied Active Info. ***
    DB name                        : testdb
    DB creation time               : 11:28:00.000 AM 12/17/2010  (1292552880)
    EOF LSA                        : 1913 | 2976
    Append LSA                     : 1913 | 2976
    HA server state                : active
     
     ***  Active Info. ***
    DB name                        : testdb
    DB creation time               : 11:28:00.000 AM 12/17/2010  (1292552880)
    EOF LSA                        : 1913 | 2976
    Append LSA                     : 1913 | 2976
    HA server state                : active

* 복제 로그 복사 지연이 의심되는 경우 네트워크 회선 속도가 느려졌는지, 디스크 여유 공간이 충분한지, 디스크 I/O에는 이상이 없는지 등을 확인한다.

* 슬레이브 노드에서 복제 로그의 반영 지연 여부를 확인하기 위해, **cubrid applyinfo** 를 실행하여 "Applied Info." 의 "Committed page" 값과 "Copied Active Info."의 "EOF LSA" 값을 비교한다. 이 값이 큰 차이를 보인다면, 복제 로그가 슬레이브 데이터베이스를 반영하는데 지연이 있다는 의미이다(:ref:`cubrid-applyinfo` 참고). ::

    [nodeB]$ cubrid applyinfo -L /home/cubrid/DB/testdb_nodeA -r nodeA -a testdb
 
     *** Applied Info. ***
    Committed page                 : 1913 | 2904
    Insert count                   : 645
    Update count                   : 0
    Delete count                   : 0
    Schema count                   : 60
    Commit count                   : 15
    Fail count                     : 0
     
     *** Copied Active Info. ***
    DB name                        : testdb
    DB creation time               : 11:28:00.000 AM 12/17/2010  (1292552880)
    EOF LSA                        : 1913 | 2976
    Append LSA                     : 1913 | 2976
    HA server state                : active
    ...

* 복제 로그 반영 지연이 심한 경우 수행 시간이 긴 트랜잭션을 의심해 볼 수 있는데, 해당 트랜잭션의 수행이 정상이라면 복제 지연 역시 정상적으로 발생할 수 있다. 정상 여부를 판단하기 위해 **cubrid applyinfo** 를 지속적으로 수행하면서 applylogdb가 복제 로그를 슬레이브 노드에 계속 반영하고 있는지 확인해야 한다.

* copylogdb, applylogdb 프로세스가 생성한 오류 로그의 메시지를 확인한다(오류 메시지 참고).

* 마스터 데이터베이스 테이블의 레코드 개수, 슬레이브 데이터베이스 테이블의 레코드 개수를 비교한다.

오류 메시지 확인
----------------

**복제 로그 복사 프로세스(copylogdb)**

복제 로그 복사 프로세스의 오류 메시지는 **$CUBRID/log/** *db-name* **@** *remote-node-name* **_copylogdb.err** 에 남는다. 복제 로그 복사 프로세스에서 남을 수 있는 오류 메시지의 severity는 fatal, error, notification이며 기본 severity는 error이다. 따라서 notification 오류 메시지를 남기려면 **cubrid.conf** 의 **error_log_level** 값을 변경해야 한다. 이에 대한 자세한 설명은 :ref:`error-parameters` 를 참고한다.

**초기화 오류 메시지**

복제 로그 복사 프로세스의 초기화 단계에서 남을 수 있는 오류 메시지는 아래와 같다.

+-----------+-----------------------------------------------------------------+--------------+-----------------------------------------------------+-----------------------------------------------------------------------------+
| 오류 코드 | 오류 메시지                                                     | severity     | 설명                                                | 조치 사항                                                                   |
+===========+=================================================================+==============+=====================================================+=============================================================================+
| 10        | ? 디스크 볼륨을 마운트할 수 없습니다.                           | error        | 복제 로그 파일 열기 실패                            | 복제 로그 존재 유무를 확인한다. 복제 로그의 위치는                          |
|           |                                                                 |              |                                                     | `기본 환경 설정 <#admin_admin_ha_conf_ha_htm>`_                             |
|           |                                                                 |              |                                                     | 을 참고한다.                                                                |
+-----------+-----------------------------------------------------------------+--------------+-----------------------------------------------------+-----------------------------------------------------------------------------+
| 78        | 내부 에러: an I/O error occurred while reading logical log page | fatal        | 복제 로그 읽기 실패                                 | cubrid applyinfo 유틸리티를 통해 복제 로그를 확인한다.                      |
|           | ? (physical page ?) of ?                                        |              |                                                     |                                                                             |
+-----------+-----------------------------------------------------------------+--------------+-----------------------------------------------------+-----------------------------------------------------------------------------+
| 81        | 내부 에러: logical log page ? may be corrupted.                 | fatal        | 복제 로그 복사 프로세스가 연결된 데이터베이스       | 복제 로그 복사 프로세스가 연결된 데이터베이스 서버 프로세스의 오류 로그를   |
|           |                                                                 |              | 서버 프로세스로부터 복사한 복제 로그 페이지의 오류  | 확인한다. 이 오류 로그는 $CUBRID/log/server에 위치한다.                     |
+-----------+-----------------------------------------------------------------+--------------+-----------------------------------------------------+-----------------------------------------------------------------------------+
| 1039      | log writer: log writer가 시작되었습니다. mode: ?                | error        | 복제 로그 복사 프로세스가 초기화 성공하여 정상 시작 | 이 오류 메시지는 복제 로그 복사 프로세스의 시작 정보를 나타내기 위해        |
|           |                                                                 |              |                                                     | 기록되는 것이므로 조치 사항은 없다. 복제 로그 복사 프로세스가 시작한 후     |
|           |                                                                 |              |                                                     | 이 오류 메시지가 나오기 전까지의 오류 메시지는 정상 상황에서 발생할 수      |
|           |                                                                 |              |                                                     | 있는 것이므로 무시한다.                                                     |
+-----------+-----------------------------------------------------------------+--------------+-----------------------------------------------------+-----------------------------------------------------------------------------+

**복제 로그 요청 및 수신 오류 메시지**

복제 로그 복사 프로세스는 연결된 데이터베이스 서버 프로세스에 복제 로그를 요청하고 적절한 복제 로그를 수신한다. 이때 발생하는 오류 메시지는 아래와 같다.

+-----------+--------------------------------+--------------+--------------------------------------------+------------------------------------------------------------------------------------------------------+
| 오류 코드 | 오류 메시지                    | severity     | 설명                                       | 조치 사항                                                                                            |
+===========+================================+==============+============================================+======================================================================================================+
| 89        | 로그 ?는 주어진 데이터베이스에 | error        | 기존에 복제되었던 로그와 현재 복제하려는   | 복제 로그 복사 프로세스가 연결한 데이터베이스 서버/호스트 정보를 확인한다. 연결하려는 데이터베이스   |
|           | 속하지 않습니다.               |              | 로그가 다름                                | 서버/호스트 정보를 변경해야 하는 경우 기존 복제 로그를 삭제하여 초기화하고 재시작한다.               |
+-----------+--------------------------------+--------------+--------------------------------------------+------------------------------------------------------------------------------------------------------+
| 186       | 서버로부터의 데이터 수신 에러. | error        | 복제 로그 복사 프로세스가 연결된           | 내부적으로 복구된다.                                                                                 |
|           |                                |              | 데이터베이스 서버로부터 잘못된 정보를 수신 |                                                                                                      |
+-----------+--------------------------------+--------------+--------------------------------------------+------------------------------------------------------------------------------------------------------+
| 199       | 서버가 응답하지 않습니다.      | error        | 복제 로그 복사 프로세스가 연결된           | 내부적으로 복구된다.                                                                                 |
|           |                                |              | 데이터베이스 서버로부터 연결 종료          |                                                                                                      |
+-----------+--------------------------------+--------------+--------------------------------------------+------------------------------------------------------------------------------------------------------+

**복제 로그 쓰기 오류 메시지**

복제 로그 복사 프로세스는 연결된 데이터베이스 서버 프로세스로부터 수신한 복제 로그를 **cubrid_ha.conf** 에서 지정한 위치(**ha_copy_log_base**)에 복사한다. 이때 발생하는 오류 메시지는 아래와 같다.

+-----------+-----------------------------------------------------------------------------+--------------+----------------------------+----------------------------+
| 오류 코드 | 오류 메시지                                                                 | severity     | 설명                       | 조치 사항                  |
+===========+=============================================================================+==============+============================+============================+
| 10        | ? 디스크 볼륨을 마운트할 수 없습니다.                                       | error        | 복제 로그 파일 열기 실패   | 복제 로그 유무를 확인한다. |
+-----------+-----------------------------------------------------------------------------+--------------+----------------------------+----------------------------+
| 79        | 내부 에러: an I/O error occurred while writing logical log page ?           | fatal        | 복제 로그 쓰기 실패        | 내부적으로 복구 된다.      |
|           | (physical page ?) of ?.                                                     |              |                            |                            |
+-----------+-----------------------------------------------------------------------------+--------------+----------------------------+----------------------------+
| 80        | ?의 logical log page ? (physical page ?) 쓰는 도중 시스템 디바이스의 공간이 | fatal        | 파일 시스템 공간 부족으로  | 디스크 파티션 내 여유      |
|           | 부족합니다. ? 바이트 이상은 쓸 수 없습니다.                                 |              | 복제 로그 쓰기 실패        | 공간이 있는지 확인한다.    |
+-----------+-----------------------------------------------------------------------------+--------------+----------------------------+----------------------------+

**복제 로그 아카이브 오류 메시지**

복제 로그 복사 프로세스는 연결된 데이터베이스 서버 프로세스로부터 받은 복제 로그를 일정한 주기마다 아카이브(archive)하여 보관하게 된다. 이때 발생하는 오류 메시지는 아래와 같다.

+-----------+--------------------------------------------------------------------------------+--------------+--------------------+---------------------------------------------------+
| 오류 코드 | 오류 메시지                                                                    | severity     | 설명               | 조치 사항                                         |
+===========+================================================================================+==============+====================+===================================================+
| 78        | 내부 에러: an I/O error occurred while reading logical log page ?              | fatal        | 아카이브 중 복제   | cubrid applyinfo 유틸리티를 통해 복제 로그를      |
|           | (physical page ?) of ?.                                                        |              | 로그 읽기 실패     | 확인한다.                                         |
+-----------+--------------------------------------------------------------------------------+--------------+--------------------+---------------------------------------------------+
| 79        | 내부 에러: an I/O error occurred while writing logical log page ?              | fatal        | 아카이브 로그      | 내부적으로 복구된다.                              |
|           | (physical page ?) of ?.                                                        |              | 쓰기 실패          |                                                   |
+-----------+--------------------------------------------------------------------------------+--------------+--------------------+---------------------------------------------------+
| 81        | 내부 에러: logical log page ? may be corrupted.                                | fatal        | 아카이브 중 복제   | cubrid applyinfo 유틸리티를 통해 복제 로그를      |
|           |                                                                                |              | 로그 오류 발견     | 확인한다.                                         |
+-----------+--------------------------------------------------------------------------------+--------------+--------------------+---------------------------------------------------+
| 98        | ?에서 ?까지의 페이지들을 archive하기 위한 archive 로그 ?를 생성할 수 없습니다. | fatal        | 아카이브 로그 파일 | 디스크 파티션 내 여유 공간이 있는지 확인한다.     |
|           |                                                                                |              | 생성 실패          |                                                   |
+-----------+--------------------------------------------------------------------------------+--------------+--------------------+---------------------------------------------------+
| 974       | ?에서 ?까지의 페이지들을 archive하기 위한 archive 로그 ?를 생성했습니다.       | notification | 아카이브 로그 파일 | 이 오류 메시지는 생성된 아카이브 로그 정보를 위해 |
|           |                                                                                |              | 정보               | 기록되는 것이므로 조치 사항은 없다.               |
+-----------+--------------------------------------------------------------------------------+--------------+--------------------+---------------------------------------------------+

**종료 및 재시작 오류 메시지**

복제 로그 복사 프로세스가 종료 및 재시작 시에 발생하는 오류 메시지는 다음과 같다.

+-----------+----------------------------------------------------+--------------+----------------------------------------------+----------------------+
| 오류 코드 | 오류 메시지                                        | severity     | 설명                                         | 조치 사항            |
+===========+====================================================+==============+==============================================+======================+
| 1037      | log writer: log writer가 시그널에 의해 종료됩니다. | error        | 지정된 시그널에 의해 copylogdb 프로세스 종료 | 내부적으로 복구된다. |
+-----------+----------------------------------------------------+--------------+----------------------------------------------+----------------------+

**복제 로그 반영 프로세스(applylogdb)**

복제 로그 반영 프로세스의 오류 메시지는 **$CUBRID/log/** *db-name* **@** *local-node-name* **_applylogdb_** *db-name* **_** *remote-node-name* **.err** 에 남는다. 복제 로그 반영 프로세스에서 남을 수 있는 오류 메시지의 severity는 fatal, error, notification이며 기본 severity는 error이다. 따라서 notification 오류 메시지를 남기려면 **cubrid.conf** 의 **error_log_level** 값을 변경해야 한다. 이에 대한 자세한 설명은 :ref:`error-parameters` 를 참고한다.

**초기화 오류 메시지**

복제 로그 반영 프로세스의 초기화 단계에서 남을 수 있는 오류 메시지는 아래와 같다.

+-----------+--------------------------------------------------------------------------------+--------------+--------------------------------------+--------------------------------------------------------------+
| 오류 코드 | 오류 메시지                                                                    | severity     | 설명                                 | 조치 사항                                                    |
+===========+================================================================================+==============+======================================+==============================================================+
| 10        | ? 디스크 볼륨을 마운트할 수 없습니다.                                          | error        | 동일한 복제 로그를 반영하려는        | 동일한 복제 로그를 반영하려는 applylogdb 프로세스가          |
|           |                                                                                |              | applylogdb가 이미 실행 중            | 있는지 확인한다.                                             |
+-----------+--------------------------------------------------------------------------------+--------------+--------------------------------------+--------------------------------------------------------------+
| 1038      | log applier: log applier가 시작되었습니다. required LSA:                       | error        | applylogdb 초기화 성공 후 정상 시작  | 이 오류 메시지는 복제 로그 반영 프로세스의 시작 정보를       |
|           | ?|?. last committed LSA: ?|?.                                                  |              |                                      | 나타내기 위해 기록되는 것이므로 조치 사항은 없다.            |
+-----------+--------------------------------------------------------------------------------+--------------+--------------------------------------+--------------------------------------------------------------+

**로그 분석 오류 메시지**

복제 로그 반영 프로세스는 복제 로그 복사 프로세스에 의해 복사된 복제 로그를 읽어 분석하고 이를 반영한다. 복제 로그를 분석할 때 발생하는 오류 메시지는 아래와 같다.

+-----------+---------------------------------------------------------------------------------+--------------+---------------------------------------------+----------------------------------------+
| 오류 코드 | 오류 메시지                                                                     | severity     | 설명                                        | 조치 사항                              |
+===========+=================================================================================+==============+=============================================+========================================+
| 13        | 볼륨 ?의 ? 페이지를 읽는 도중에 I/O 에러 발생.                                  | error        | 복제 반영할 로그 페이지 읽기 실패           | cubrid applyinfo 유틸리티를 통해 복제  |
|           |                                                                                 |              |                                             | 로그를 확인한다.                       |
+-----------+---------------------------------------------------------------------------------+--------------+---------------------------------------------+----------------------------------------+
| 17        | 내부 에러: 이미 해제된 볼륨 ?의 ? 페이지에 대한 읽기 시도.                      | fatal        | 복제 로그에 포함되지 않은 로그 페이지를     | cubrid applyinfo 유틸리티를 통해 복제  |
|           |                                                                                 |              | 읽기 시도                                   | 로그를 확인한다.                       |
+-----------+---------------------------------------------------------------------------------+--------------+---------------------------------------------+----------------------------------------+
| 81        | 내부 에러: logical log page ? may be corrupted.                                 | fatal        | 기존 복제 반영 중이던 로그와 현재 로그가    | cubrid applyinfo 유틸리티를 통해 복제  |
|           |                                                                                 |              | 불일치 또는 복제 로그 레코드 오류           | 로그를 확인한다.                       |
+-----------+---------------------------------------------------------------------------------+--------------+---------------------------------------------+----------------------------------------+
| 82        | 로그 디스크 볼륨/파일 ?을(를) 마운트할 수 없습니다.                             | error        | 복제 로그 파일이 존재하지 않음              | 복제 로그 존재 유무를 확인한다.        |
|           |                                                                                 |              |                                             | cubrid applyinfo 유틸리티를 통해 복제  |
|           |                                                                                 |              |                                             | 로그를 확인한다.                       |
+-----------+---------------------------------------------------------------------------------+--------------+---------------------------------------------+----------------------------------------+
| 97        | 내부 에러: unable to find log page ? in log archives.                           | error        | 로그 페이지가 복제 로그에 존재하지 않음     | cubrid applyinfo 유틸리티를 통해 복제  |
|           |                                                                                 |              |                                             | 로그를 확인한다.                       |
+-----------+---------------------------------------------------------------------------------+--------------+---------------------------------------------+----------------------------------------+
| 897       | 압축 해제 오류입니다.                                                           | error        | 로그 레코드 압축 해제 실패                  | cubrid applyinfo 유틸리티를 통해 복제  |
|           |                                                                                 |              |                                             | 로그를 확인한다.                       |
+-----------+---------------------------------------------------------------------------------+--------------+---------------------------------------------+----------------------------------------+
| 1028      | log applier: Archive 로그에 예상치 못한 EOF 로그 레코드가 있습니다. LSA: ?|?.   | error        | 아카이브 로그에 잘못된 로그 레코드가 포함   | cubrid applyinfo 유틸리티를 통해 복제  |
|           |                                                                                 |              |                                             | 로그를 확인한다.                       |
+-----------+---------------------------------------------------------------------------------+--------------+---------------------------------------------+----------------------------------------+
| 1029      | log applier: 잘못된 로그 페이지/오프셋. page HDR: ?|?, final: ?|?,              | error        | 잘못된 로그 레코드가 포함                   | cubrid applyinfo 유틸리티를 통해 복제  |
|           | append LSA: ?|?, EOF LSA: ?|?, ha file status: ?, is end-of-log: ?.             |              |                                             | 로그를 확인한다.                       |
+-----------+---------------------------------------------------------------------------------+--------------+---------------------------------------------+----------------------------------------+
| 1030      | log applier: 잘못된 로그 레코드. LSA: ?|?, forw LSA: ?|?,                       | error        | 로그 레코드 헤더 오류                       | cubrid applyinfo 유틸리티를 통해 복제  |
|           | backw LSA: ?|?, Trid: ?, prev tran LSA: ?|?, type: ?.                           |              |                                             | 로그를 확인한다.                       |
+-----------+---------------------------------------------------------------------------------+--------------+---------------------------------------------+----------------------------------------+

**복제 로그 반영 오류 메시지**

복제 로그 반영 프로세스는 복제 로그 복사 프로세스에 의해 복사된 복제 로그를 읽어 분석하고 이를 반영한다. 분석한 복제 로그를 반영할 때 발생하는 오류 메시지는 아래와 같다.

+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 오류 코드 | 오류 메시지                                                             | severity     | 설명                                              | 조치 사항                                 |
+===========+=========================================================================+==============+===================================================+===========================================+
| 72        | 트랜잭션이(인덱스 ?, ?@?|?) 시스템에 의해 취소되었습니다.               | error        | 데드락 등에 의해 복제 반영 실패                   | 내부적으로 복구된다.                      |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 111       | 당신의 트랜잭션은 서버 failure 혹은 모드 변경으로 인해 취소되었습니다.  | error        | 복제를 반영하려는 데이터베이스 서버 프로세스 종료 | 내부적으로 복구된다.                      |
|           |                                                                         |              | 또는 모드 변경에 의해 복제 반영 실패              |                                           |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 191       | ? 상의 서버 ?에 접속할 수 없습니다.                                     | error        | 복제를 반영하려는 데이터베이스 서버 프로세스와의  | 내부적으로 복구된다.                      |
|           |                                                                         |              | 연결 종료                                         |                                           |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 195       | 서버 통신 에러: ?.                                                      | error        | 복제를 반영하려는 데이터베이스 서버 프로세스와의  | 내부적으로 복구된다.                      |
|           |                                                                         |              | 연결 종료                                         |                                           |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 224       | 데이터베이스가 다시 시작되지 않았습니다.                                | error        | 복제를 반영하려는 데이터베이스 서버 프로세스와의  | 내부적으로 복구된다.                      |
|           |                                                                         |              | 연결 종료                                         |                                           |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 1027      | log applier: ?에서 ?로 복제 반영 상태를 변경하지 못하였습니다.          | error        | 복제 반영 상태 변경 실패                          | 내부적으로 복구된다.                      |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 1031      | log applier: Schema 복제 로그 반영에 실패하였습니다.                    | error        | SCHEMA 복제 반영 실패                             | 복제 불일치 여부를 확인하고 불일치 시     |
|           | class: ?, schema: ?, internal error: ?.                                 |              |                                                   | HA 복제 재구성을 실행한다.                |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 1032      | log applier: Insert 복제 로그 반영에 실패하였습니다.                    | error        | INSERT 복제 반영 실패                             | 복제 불일치 여부를 확인하고 불일치 시     |
|           | class: ?, schema: ?, internal error: ?.                                 |              |                                                   | HA 복제 재구성을 실행한다.                |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 1033      | log applier: Update 복제 로그 반영에 실패하였습니다.                    | error        | UPDATE 복제 반영 실패                             | 복제 불일치 여부를 확인하고 불일치 시     |
|           | class: ?, schema: ?, internal error: ?.                                 |              |                                                   | HA 복제 재구성을 실행한다.                |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 1034      | log applier: Delete 복제 로그 반영에 실패하였습니다.                    | error        | DELETE 복제 반영 실패                             | 복제 불일치 여부를 확인하고 불일치 시     |
|           | class: ?, schema: ?, internal error: ?.                                 |              |                                                   | HA 복제 재구성을 실행한다.                |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+
| 1040      | HA generic: ?.                                                          | notification | 아카이브 로그의 마지막 레코드를 반영하거나 복제   | 이 에러 메시지는 일반적인 정보를 위해     |
|           |                                                                         |              | 반영 상태 변경                                    | 기록되는 로그로 조치 사항은 없다.         |
+-----------+-------------------------------------------------------------------------+--------------+---------------------------------------------------+-------------------------------------------+

**종료 및 재시작 오류 메시지**

복제 로그 반영 프로세스가 종료 및 재시작 시에 발생하는 오류 메시지는 다음과 같다.

+-----------+--------------------------------------------------------------------------------------------+--------------+------------------------------------+----------------------+
| 오류 코드 | 오류 메시지                                                                                | severity     | 설명                               | 조치 사항            |
+===========+============================================================================================+==============+====================================+======================+
| 1035      | log applier: log applier의 메모리 크기(? MB)가 최대 메모리 크기(? MB)보다 크거나 시작 시   | error        | 최대 메모리 크기 제한에 의해 복제  | 내부적으로 복구된다. |
|           | 메모리 크기(? MB)보다 2배 이상 증가하였습니다. required LSA: ?|?. last committed LSA: ?|?. |              | 로그 반영 프로세스 재시작          |                      |
+-----------+--------------------------------------------------------------------------------------------+--------------+------------------------------------+----------------------+
| 1036      | log applier: log applier가 시그널에 의해 종료됩니다.                                       | error        | 지정된 시그널에 의해 복제 로그     | 내부적으로 복구된다. |
|           |                                                                                            |              | 반영 프로세스 종료                 |                      |
+-----------+--------------------------------------------------------------------------------------------+--------------+------------------------------------+----------------------+

.. _rebuilding-replication:

복제 재구축
-----------

CUBRID HA 환경에서의 복제 재구축은 다중 슬레이브 노드의 다중 장애 상황이나 일반적인 오류 상황으로 인해 CUBRID HA 그룹 내의 데이터가 동일하지 않은 경우에 필요하다. CUBRID HA 환경에서의 복제 재구축은 **ha_make_slavedb.sh** 스크립트를 통해 제공된다. **cubrid applyinfo** 유틸리티는 복제 진행 상태를 확인할 수는 있지만 이를 통해 복제 불일치 여부를 직접 판단할 수는 없으므로, 복제 불일치 여부를 정확하게 판단하려면 마스터 노드와 슬레이브 노드의 데이터를 직접 확인해야 한다.

복제 재구축을 위해서는 슬레이브 노드와 마스터 노드, 레플리카 노드에서 아래 환경이 동일해야 한다.

*   CUBRID 버전
*   환경 변수(**$CUBRID**, **$CUBRID_DATABASES**, **$LD_LIBRARY_PATH**, **$PATH**)
*   데이터베이스 볼륨, 로그 및 복제 로그 경로
*   리눅스 서버의 사용자 아이디 및 비밀번호
*   **ha_mode**, **ha_copy_sync_mode**, **ha_ping_hosts** 를 제외한 모든 HA 관련 파라미터

**ha_make_slavedb.sh 스크립트**

**ha_make_slavedb.sh** 스크립트를 이용하여 복제 재구축을 수행할 수 있다. 이 스크립트는 **$CUBRID/share/scripts/ha** 에 위치하며, 복제 재구축에 들어가기 전에 다음의 항목을 사용자 환경에 맞게 설정해야 한다. 이 스크립트는 2008 R2.2 Patch 9 버전부터 지원하지만 2008 R4.1 Patch 2 미만 버전과는 일부 설정 방법이 다르며, 이 문서에서는 2008 R4.1 Patch 2 이상 버전에서의 설정 방법에 대해 설명한다.

*   **target_host** : 복제 재구축을 위한 원본 노드(주로 마스터 노드)의 호스트명으로, **/etc/hosts** 에 등록되어 있어야 한다. 슬레이브 노드는 마스터 노드 또는 레플리카 노드로 복제 재구축이 가능하며, 레플리카 노드는 슬레이브 노드 또는 또 다른 레플리카 노드로 복제 재구축이 가능하다.

*   **repl_log_home** : 마스터 노드의 복제 로그의 홈 디렉터리를 설정한다. 일반적으로 **$CUBRID_DATABASES** 와 동일하다. 반드시 절대 경로를 입력해야 하며, 심볼릭 링크를 사용하면 안 된다. 경로 뒤에 슬래시(/)를 붙이면 안 된다.

다음은 필요에 따라 선택적으로 설정하는 항목이다.

*   **db_name** : 복제 재구축할 데이터베이스 이름을 설정한다. 설정하지 않으면 **$CUBRID/conf/cubrid_ha.conf** 내 **ha_db_list** 의 가장 처음에 위치한 이름을 사용한다.

*   **backup_dest_path** : 복제 재구축 원본 노드에서 **backupdb** 수행 시 백업 볼륨을 생성할 경로를 설정한다.

*   **backup_option** : 복제 재구축 원본 노드에서 **backupdb** 수행 시 필요한 옵션을 설정한다.

*   **restore_option** : 복제를 재구축할 슬레이브 노드에서 **restoredb** 수행 시 필요한 옵션을 설정한다.

*   **scp_option** : 복제 재구축 원본 노드의 백업 볼륨을 슬레이브 노드로 복사해 오기 위한 **scp** 옵션을 설정할 수 있는 항목으로 기본값은 복제 재구축 원본 노드의 네트워크 부하를 주지 않기 위해 **-l 131072** 옵션을 사용한다(전송 속도를 16M로 제한).

스크립트의 설정이 끝나면 **ha_make_slavedb.sh** 스크립트를 복제 재구축할 슬레이브 노드에서 수행한다. 스크립트 수행 시 여러 단계에 의해 복제 재구축이 이루어지며 각 단계의 진행을 위해서 사용자가 적절한 값을 입력해야 한다. 다음은 입력할 수 있는 값에 대한 설명이다.

*   **yes** : 계속 진행한다.

*   **no** : 현재 단계를 포함하여 이후 과정을 진행하지 않는다.

*   **skip** : 현재 단계를 수행하지 않고 다음 단계를 진행한다. 이 입력 값은 이전 스크립트 수행에 실패하여 재시도할 때 다시 수행할 필요가 없는 단계를 무시하기 위해 사용한다.

**제약 사항**

*   해당 스크립트는 expect와 ssh를 이용하여 원격 노드에 접속 명령을 수행하므로 원격 ssh 접속이 가능해야 한다.

*   **복제 재구축 노드의 온라인 백업** : 복제 재구축을 위해서는 복제 재구축 노드나 슬레이브 노드의 기존 백업을 이용할 수 없다. 따라서 스크립트 내부에서 자동으로 수행하는 마스터 노드의 온라인 백업을 이용해야 한다.

*   **복제 재구축 스크립트 수행 중 오류 발생** : 복제 재구축 스크립트는 수행 도중 오류가 발생해도 이전 상황으로 자동 롤백되지 않는다. 이는 복제 재구축 스크립트를 수행하기 전에도 슬레이브 노드가 이미 정상적으로 서비스하기 힘든 상황이기 때문이다. 복제 재구축 스크립트를 수행하기 전 상황으로 돌아가려면, 복제 재구축 스크립트를 수행하기 전에 마스터 노드와 슬레이브 노드의 내부 카탈로그인 **db_ha_apply_info** 정보와 기존의 복제 로그를 백업해야 한다.

**주의 사항**

복제 재구축을 수행하려면 원본 노드에 있는 데이터베이스 볼륨의 물리적 이미지를 복제 대상 노드의 데이터베이스에 복사해야 한다. 그런데 **cubrid unloaddb** 는 논리적인 이미지를 백업하므로  **cubrid unloaddb** 와 **cubrid loaddb** 를 이용해서는 복제 재구축을 할 수 없다. **cubrid backupdb** 는 물리적 이미지를 백업하므로 이를 이용한 복제 재구축이 가능하며, **ha_make_slavedb.sh** 스크립트는 **cubrid backupdb** 를 이용하여 복제 재구축을 수행한다.

**예시**

다음은 복제 재구축 원본 노드를 마스터 노드로 하여, 마스터 노드로부터 슬레이브 노드를 재구축하는 예이다.

.. image:: /images/image37.png

*   마스터 노드 호스트 명: *nodeA*
*   슬레이브 노드 호스트 명: *nodeB*

복제 재구축은 마스터 노드가 운영 중인 상태에서도 수행할 수 있으나, 복제 지연을 최소화하기 위해 시간당 트랜잭션 수가 적을 때 수행하는 것을 권장한다.

**ha_make_slavedb.sh** 스크립트를 실행하여 복제 재구축을 수행하기 전에 다음과 같이 슬레이브 노드의 HA 서비스를 종료하고 **ha_make_slavedb.sh** 스크립트를 설정해야 한다. target_host에는 복사할 대상, 즉 마스터 노드의 호스트 이름(*nodeA*)을 설정하고, repl_log_home에는 복제로그의 홈 디렉터리(기본값: $CUBRID_DATABASES)를 설정한다. ::

    [nodeB]$ cubrid heartbeat stop
     
    [nodeB]$ cd $CUBRID/share/scripts/ha
    [nodeB]$ vi ha_make_slavedb.sh
    target_host=nodeA

설정 후에는 다음과 같이 슬레이브 노드에서 **ha_make_slavedb.sh** 스크립트를 실행한다 ::

    [nodeB]$ cd $CUBRID/share/scripts/ha
    [nodeB]$ ./ha_make_slavedb.sh

스크립트를 각 단계의 순서대로 실행하는 도중 오류가 발생하거나 n을 입력하여 실행을 중단한 이후에 재실행할 경우, 그 이전까지 실행에 성공한 단계에 대해서는 s를 입력하여 다음 단계로 넘어가도 된다.

1. HA 복제 재구축을 위한 Linux 계정의 암호와, CUBRID DB 계정인 **dba** 의 암호를 입력하는 단계이다. 질문에 y를 입력한다. ::

    ##### step 1 ###################################################################
    #
    # get HA/replica user password and DBA password
    #
    #  * warning !!!
    #   - Because ha_make_slavedb.sh use expect (ssh, scp) to control HA/replica node,
    #     the script has to know these passwords.
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
   
    HA 노드의 Linux 계정 암호와, CUBRID DB 계정인 **dba** 의 암호를 입력한다. 처음 CUBRID 설치 후 **dba** 암호를 변경하지 않았을 경우, **dba** 암호의 입력 없이 <Enter> 키를 누르면 된다. ::
    
    HA/replica cubrid_usr's password :
    HA/replica cubrid_usr's password :
     
    testdb's DBA password :
    Retype testdb's DBA password :

2. 레이브 노드의 환경 변수 설정이 올바른지 확인하는 단계이다. 질문에 y를 입력한다. ::

    ##### step 2 ###################################################################
    #
    #  ha_make_slavedb.sh is the script for making slave database more easily
    #
    #  * environment
    #   - db_name           : testdb
    #
    #   - master_host       : nodeA
    #   - slave_host        : nodeB
    #   - replica_hosts     :
    #
    #   - current_host      : nodeB
    #   - current_state     : slave
    #
    #   - target_host       : nodeA
    #   - target_state      : master
    #
    #   - repl_log_home     : /home/cubrid_usr/CUBRID/databases
    #   - backup_dest_path  : /home/cubrid_usr/.ha/backup
    #   - backup_option     :
    #   - restore_option    :
    #
    #  * warning !!!
    #   - environment on slave must be same as master
    #   - database and replication log on slave will be deleted
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y

3. 슬레이브 노드의 HA 관련 스크립트들을 마스터 노드에 복사하는 단계이다. 질문에 y를 입력한다. 이후 모든 단계에서도 마스터 노드에 접속 시 암호를 요구하게 되며, scp 명령을 이용하여 필요한 파일을 전송할 때에도 암호를 요구한다. ::

    ##### step 3 ###################################################################
    #
    #  copy scripts to master node
    #
    #  * details
    #   - scp scripts to '~/.ha' on nodeA(master).
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    [nodeB]$ tar -zcf ha.tgz ha
    [nodeA]$ rm -rf /home/cubrid_usr/.ha
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.
    [nodeB]$ scp -l 131072 -r ./../ha.tgz cubrid_usr@nodeA:/home1/cubrid_usr
    cubrid_usr@nodeA's password:
    ha.tgz                    100%   10KB  10.4KB/s   00:00
    [nodeA]$ tar -zxf ha.tgz
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.
    [nodeA]$ mv ha /home/cubrid_usr/.ha
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.
    [nodeA]$ mkdir /home/cubrid_usr/.ha/backup
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.

  scp 수행 시 암호 요청 과정을 생략하려면 다음과 같이 scp의 개인키를 슬레이브 노드에, 공개키를 마스터 노드에 설정하면 된다. 보다 자세한 내용은 Linux의 ssh-keygen 사용법을 참고한다.

  #.  **ssh-keygen -t rsa** 를 실행하여 리눅스 사용자 계정의 홈 디렉터리 이하 .ssh/id_rsa와 .ssh/id_rsa.pub 파일이 생성되었음을 확인한다.
  #.  마스터 노드 Linux 사용자 계정의 홈 디렉터리/.ssh 디렉터리 이하에 id_rsa.pub을 authorized_keys라는 파일명으로 복사한다.
  #.  테스트를 통해 암호 요청 없이 파일이 복사되는지 확인한다. (scp test.txt cubrid_usr@:/home/cubrid_usr/.)

4. HA 관련 스크립트들을 레플리카 노드에 복사하는 단계이다. 이 시나리오에서는 레플리카 노드가 없으므로 이 단계는 s를 입력하여 다음 단계로 넘어가도 된다. ::

    ##### step 4 #####################################
    #
    #  copy scripts to replication node
    #
    #  * details
    #   - scp scripts to '~/.ha' on replication node.
    #
    ##################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    There is no replication server to copy scripts.

5. 모든 노드의 환경 변수 설정이 올바른지 확인하는 단계이다. 질문에 y를 입력한다. ::

    ##### step 5 ###################################################################
    #
    #  check environment of all ha node
    #
    #  * details
    #   - test $CUBRID == /home1/cubrid_usr/CUBRID
    #   - test $CUBRID_DATABASES == /home1/cubrid_usr/CUBRID/database
    #   - test -d /home1/cubrid_usr/CUBRID/database/testdb
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y

6. 마스터 노드의 복제 진행을 멈추도록 하는 단계이다. 질문에 y를 입력한다. ::

    ##### step 6 ###################################################################
    #
    #  suspend copylogdb/applylogdb on master if running
    #
    #  * details
    #   - deregister copylogdb/applylogdb on nodeA(master).
    #
    ################################################################################
       continue ? ([y]es / [n]o / [s]kip) : y
     
    [nodeA]$ sh /home/cubrid_usr/.ha/functions/ha_repl_suspend.sh -l /home/cubrid_usr/CUBRID/databases -d testdb -h nodeB -o /home/cubrid_usr/.ha/repl_utils.output
    cubrid_usr@nodeA's password:
    [nodeA]$ cubrid heartbeat deregister 9408
    suspend: (9408) cub_admin copylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB -m sync testdb@nodeB
    [nodeA]$ cubrid heartbeat deregister 9410
    suspend: (9410) cub_admin applylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB --max-mem-size=300 testdb@localhost
     
     
    3. heartbeat status on nodeA(master).
     
    [nodeA]$ cubrid heartbeat list
    @ cubrid heartbeat list
     
     HA-Node Info (current nodeA, state master)
       Node nodeB (priority 2, state unknown)
       Node nodeA (priority 1, state master)
     
     
     HA-Process Info (master 8362, state master)
       Copylogdb testdb@nodeB:/home/cubrid_usr/CUBRID/databases/testdb_nodeB (pid 9408, state deregistered)
       Server testdb (pid 9196, state registered_and_active)
     
    Connection to nodeA closed.
    Wait for 60s to deregister coppylogdb/applylogdb.
    ............................................................


7. 이전에 슬레이브 노드에 존재했던 오래된 복사 로그를 삭제하고 마스터 노드의 HA 메타 정보 테이블을 초기화하는 단계이다. 질문에 y를 입력한다. ::

    ##### step 7 ###################################################################
    #
    #  remove old copy log of slave and init db_ha_apply_info on master
    #
    #  * details
    #   - remove old copy log of slave
    #   - init db_ha_apply_info on master
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    - 1. remove old copy log.
     
    [nodeA]$ rm -rf /home/cubrid_usr/CUBRID/databases/testdb_nodeB/*
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.
     
     - 2. init db_ha_apply_info.
     
    [nodeA]$ csql -C -u dba  --sysadm testdb@localhost -c "delete from db_ha_apply_info where db_name='testdb'"
    cubrid_usr@nodeA's password:
    Connection to nodeA closed.
    [nodeA]$ csql -C -u dba  --sysadm testdb@localhost -c "select * from db_ha_apply_info where db_name='testdb'"
    cubrid_usr@nodeA's password:
     
    === <Result of SELECT Command in Line 1> ===
     
    There are no results.
    Connection to nodeA closed.

8. 레플리카 노드의 HA 메타 정보 테이블을 초기화하는 단계이다. 이 시나리오에서는 레플리카 노드가 없으므로 질문에 s를 입력하여 넘어가도 된다. ::

    ##### step 8 ###################################################################
    #
    #  remove old copy log of slave and init db_ha_apply_info on replications
    #
    #  * details
    #   - remove old copy log of replica
    #   - init db_ha_apply_info on master
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    There is no replication server to init ha_info


9. HA 복제 재구축을 위해 마스터 노드(target_host)로부터 백업 볼륨을 생성하는 단계로서, 기존에 이미 생성한 백업 볼륨이 있다면 s를 입력하여 다음 단계로 넘어갈 수 있다. 기존 백업 볼륨을 사용하여 복제 재구축을 수행하기 위해서는 다음과 같은 제약 조건이 있다.
   
  *   백업 시 실행 중인 트랜잭션을 포함하는 보관 로그(archive log)가 반드시 마스터 노드(target_host) 내에 존재해야 한다(즉, 오래 전에 생성한 백업 볼륨은 사용할 수 없음).

  *   백업 시 -o 옵션을 사용하여 백업 상태 정보 파일을 남겨야 한다. 이 때 저장되는 경로는 백업 볼륨 파일의 경로와 같아야 한다. 파일 이름은 db_name.bkup.output 형식이어야 하며, 파일 이름이 다른 경우 스크립트 수행 전에 형식에 맞게 변경한다.

  *   기존 백업 볼륨 및 상태 정보 파일의 경로는 스크립트 내의 backup_dest_path 파라미터에 지정해야 한다. 즉, 마스터 노드(target_host)에 존재하는 백업 볼륨이 포함된 디렉터리의 절대 경로를 이 파라미터에 지정하면 된다.

  ::

    ##### step 9 ###################################################################
    #
    #  online backup database  on master
    #
    #  * details
    #   - run 'cubrid backupdb -C -D ... -o ... testdb@localhost' on master
    #
    ################################################################################
     
       continue ? ([y]es / [n]o / [s]kip) : y
     
    [nodeA]$ cubrid backupdb  -C -D /home/cubrid_usr/.ha/backup -o /home/cubrid_usr/.ha/backup/testdb.bkup.output testdb@localhost
    cubrid_usr@nodeA's password:
    Backup Volume Label: Level: 0, Unit: 0, Database testdb, Backup Time: Thu Apr 19 18:52:03 2012
    Connection to nodeA closed.
    [nodeA]$ cat /home/cubrid_usr/.ha/backup/testdb.bkup.output
    cubrid_usr@nodeA's password:
    [ Database(testdb) Full Backup start ]
     
    - num-threads: 2
     
    - compression method: NONE
     
    - backup start time: Thu Apr 19 18:52:03 2012
     
    - number of permanent volumes: 1
     
    - HA apply info: testdb 1334739766 715 8680
     
    - backup progress status
     
    -----------------------------------------------------------------------------
     volume name                  | # of pages | backup progress status    | done
    -----------------------------------------------------------------------------
     testdb_vinf                  |          1 | ######################### | done
     testdb                       |       6400 | ######################### | done
     testdb_lgar000               |       6400 | ######################### | done
     testdb_lgar001               |       6400 | ######################### | done
     testdb_lginf                 |          1 | ######################### | done
     testdb_lgat                  |       6400 | ######################### | done
    -----------------------------------------------------------------------------
     
    # backup end time: Thu Apr 19 18:52:06 2012
     
    [ Database(testdb) Full Backup end ]
    Connection to nodeA closed.

10. 마스터 노드의 데이터베이스 백업본을 슬레이브 노드에 복사하는 단계이다. 질문에 y를 입력한다. ::

     ##### step 10 ###################################################################
     #
     #  copy testdb databases backup to current host
     #
     #  * details
     #   - scp databases.txt from target host if there's no testdb info on current host
     #   - remove old database and replication log if exist
     #   - make new database volume and replication path
     #   - scp  database backup to current host
     #
     ################################################################################
      
        continue ? ([y]es / [n]o / [s]kip) : y
      
      
      - 1. check if the databases information is already registered.
      
      
      - thres's already testdb information in /home/cubrid_usr/CUBRID/databases/databases.txt
     [nodeB]$ grep testdb /home/cubrid_usr/CUBRID/databases/databases.txt
     testdb          /home/cubrid_usr/CUBRID/databases/testdb        nodeA:nodeB /home/cubrid_usr/CUBRID/databases/testdb/log file:/home/cubrid_usr/CUBRID/databases/testdb/lob
      
      - 2. get db_vol_path and db_log_path from databases.txt.
      
      
      - 3. remove old database and replication log.
      
     [nodeB]$ rm -rf /home/cubrid_usr/CUBRID/databases/testdb/log
     [nodeB]$ rm -rf /home/cubrid_usr/CUBRID/databases/testdb
     [nodeB]$ rm -rf /home/cubrid_usr/CUBRID/databases/testdb_*
      
      - 4. make new database volume and replication log directory.
      
     [nodeB]$ mkdir -p /home/cubrid_usr/CUBRID/databases/testdb
     [nodeB]$ mkdir -p /home/cubrid_usr/CUBRID/databases/testdb/log
     [nodeB]$ mkdir -p /home/cubrid_usr/.ha
     [nodeB]$ rm -rf /home/cubrid_usr/.ha/backup
     [nodeB]$ mkdir -p /home/cubrid_usr/.ha/backup
      
      - 5. copy backup volume and log from target host
      
     cubrid_usr@nodeA's password:
     testdb_bkvinf              100%   49     0.1KB/s   00:00
     cubrid_usr@nodeA's password:
     testdb_bk0v000             100% 1540MB   7.8MB/s   03:18
     testdb.bkup.output         100% 1023     1.0KB/s   00:00

11. 복사한 데이터베이스 백업본을 슬레이브 노드에 복구하는 단계이다. 질문에 y를 입력한다. ::

     ##### step 11 ###################################################################
     #
     #  restore database testdb on current host
     #
     #  * details
     #   - cubrid restoredb -B ... testdb current host
     #
     ################################################################################
      
        continue ? ([y]es / [n]o / [s]kip) : y
      
     [nodeB]$ cubrid restoredb -B /home/cubrid_usr/.ha/backup  testdb

12. 슬레이브 노드의 HA 메타 정보 테이블 값을 설정하는 단계이다. 질문에 y를 입력한다. ::

     ##### step 12 ###################################################################
     #
     #  set db_ha_apply_info on slave
     #
     #  * details
     #   - insert db_ha_apply_info on slave
     #
     ################################################################################
      
        continue ? ([y]es / [n]o / [s]kip) : y
      
      
      
     1. get db_ha_apply_info from backup output(/home1/cubrid_usr/.ha/backup/testdb.bkup.output).
      
      - dn_name       : testdb
      - db_creation   : 1349426614
      - pageid        : 86
      - offset        : 8800
      - log_path      : /home1/cubrid_usr/CUBRID/databases/testdb_nodeA
      
      
      
     2. select old db_ha_apply_info.
      
     [nodeA]$ csql -u DBA -S testdb -l -c "SELECT db_name, db_creation_time, copied_log_path, committed_lsa_pageid, committed_lsa_offset, committed_rep_pageid, committed_rep_offset, required_lsa_pageid, required_lsa_offset FROM db_ha_apply_info WHERE db_name='testdb'"
      
     === <Result of SELECT Command in Line 1> ===
      
     There are no results.
      
      
      
     3. insert new db_ha_apply_info on slave.
      
     [nodeB]$ csql --sysadm -u dba -S testdb -c "DELETE FROM db_ha_apply_info WHERE db_name='testdb'"
     [nodeB]$ csql --sysadm -u DBA -S testdb -c "INSERT INTO  db_ha_apply_info VALUES (       'testdb',       datetime '10/05/2012 17:43:34',         '/home1/cubrid_usr/DB/testdb_nodeA',         86, 8800,       86, 8800,       86, 8800,       86, 8800,       86, 8800,       86, 8800,       NULL,   NULL,   NULL,   0,      0,      0,      0,      0,      0,      0,      NULL )"
     [nodeB]$ csql -u DBA -S testdb -l -c "SELECT db_name, db_creation_time, copied_log_path, committed_lsa_pageid, committed_lsa_offset, committed_rep_pageid, committed_rep_offset, required_lsa_pageid, required_lsa_offset FROM db_ha_apply_info WHERE db_name='testdb'"
      
     === <Result of SELECT Command in Line 1> ===
      
     <00001> db_name             : 'testdb'
             db_creation_time    : 05:43:34.000 PM 10/05/2012
             copied_log_path     : '/home1/cubrid_usr/CUBRID/databases/testdb_nodeA'
             committed_lsa_pageid: 86
             committed_lsa_offset: 8800
             committed_rep_pageid: 86
             committed_rep_offset: 8800
             required_lsa_pageid : 86
             required_lsa_offset : 8800
 

13. 마스터에 있는 복제 로그를 초기화하고, 마스터 노드의 저장 로그를 슬레이브 노드에 복사하는 단계이다. 질문에 y를 입력한다. ::

     ##### step 13 ###################################################################
     #
     #  make initial replication active log on master, and copy archive logs from
     #  master
     #
     #  * details
     #   - remove old replication log on master if exist
     #   - start copylogdb to make replication active log
     #   - copy archive logs from master
     #
     ################################################################################
      
        continue ? ([y]es / [n]o / [s]kip) : y
      
      
      - 1. remove old replicaton log.
      
     [nodeB]$ rm -rf /home/cubrid_usr/CUBRID/databases/testdb_nodeA
     [nodeB]$ mkdir -p /home/cubrid_usr/CUBRID/databases/testdb_nodeA
      
      - 2. start copylogdb to initiate active log.
      
      
      - cubrid service stop
     [nodeB]$ cubrid service stop >/dev/null 2>&1
      
      - start cub_master
     [nodeB]$ cub_master >/dev/null 2>&1
      
      - start copylogdb and wait until replication active log header to be initialized
     [nodeB]$ cub_admin copylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeA -m 3 testdb@nodeA >/dev/null 2>&1 &
      
     ...
      
      - cubrid service stop
     [nodeB]$ cubrid service stop >/dev/null 2>&1
      
      - check copied active log header
     [nodeB]$  cubrid applyinfo -L /home/cubrid_usr/CUBRID/databases/testdb_nodeA testdb | grep -wqs "DB name"
      
      - 3. copy archive log from target.
      
     cubrid_usr@nodeA's password:
     testdb_lgar000             100%  512MB   3.9MB/s   02:11


14. 마스터 노드의 copylogdb, applylogdb 프로세스를 재시작하는 단계이다. 질문에 y를 입력한다. ::

     ##### step 14 ###################################################################
     #
     #  restart copylogdb/applylogdb on master
     #
     #  * details
     #   - restart copylogdb/applylogdb
     #
     ################################################################################
      
        continue ? ([y]es / [n]o / [s]kip) : y
      
     [nodeA]$ sh /home/cubrid_usr/.ha/functions/ha_repl_resume.sh -i /home/cubrid_usr/.ha/repl_utils.output
     cubrid_usr@nodeA's password:
     [nodeA]$ cub_admin copylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB -m sync testdb@nodeB >/dev/null 2>&1 &
     resume: cub_admin copylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB -m sync testdb@nodeB
     [nodeA]$ cub_admin applylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB --max-mem-size=300 testdb@localhost >/dev/null 2>&1 &
     resume: cub_admin applylogdb -L /home/cubrid_usr/CUBRID/databases/testdb_nodeB --max-mem-size=300 testdb@localhost
      
      - check heartbeat list on (master).
      
     [nodeA]$ cubrid heartbeat status
     @ cubrid heartbeat status
      
      HA-Node Info (current nodeA, state master)
        Node nodeB (priority 2, state unknown)
        Node nodeA (priority 1, state master)
      
      HA-Process Info (master 11847, state master)
        Server testdb (pid 11853, state registered_and_active)
      
      
     Connection to nodeA closed.


15. 슬레이브 노드 구축이 정상적으로 완료되었는지 여부를 출력하는 단계이다. ::

     ##### step 15 ##################################################################
     #
     #  completed
     #
     ################################################################################

**ha_make_slavedb.sh** 스크립트가 종료된 후에는 슬레이브 노드에서 HA 상태를 확인하고, HA를 구동한다. ::

    [NodeB]$ cubrid heartbeat status
    @ cubrid heartbeat status
    ++ cubrid master is not running.
    [NodeB]$ cubrid heartbeat start
    @ cubrid heartbeat start
    @ cubrid master start
    ++ cubrid master start: success
     
    @ HA processes start
    @ cubrid server start: testdb
     
    This may take a long time depending on the amount of recovery works to do.
     
    CUBRID 9.0
     
    ++ cubrid server start: success
    @ copylogdb start
    ++ copylogdb start: success
    @ applylogdb start
    ++ applylogdb start: success
    ++ HA processes start: success
    ++ cubrid heartbeat start: success
    [nodeB ha]$ cubrid heartbeat status
    @ cubrid heartbeat status
     
     HA-Node Info (current nodeB, state slave)
       Node nodeB (priority 2, state slave)
       Node nodeA (priority 1, state master)
     
     HA-Process Info (master 26611, state slave)
       Applylogdb testdb@localhost:/home/cubrid_usr/CUBRID/databases/testdb_nodeA (pid 26831, state registered)
       Copylogdb testdb@nodeA:/home/cubrid_usr/CUBRID/databases/testdb_nodeA (pid 26829, state registered)
       Server testdb (pid 26617, state registered_and_standby)
