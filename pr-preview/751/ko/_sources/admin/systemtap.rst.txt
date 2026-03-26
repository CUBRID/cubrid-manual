
:meta-keywords: install systemtap, systemtap marker, systemtap probe, systemtap event, systemtap script, connection markers, query markers, object operation markers, index operation markers, locking markers, transaction markers, I/O markers
:meta-description: SystemTap is a tool that can be used to dynamically monitor and track the process of running, to find and diagnose performance bottlenecks; learn how to use CUBRID markers in SystemTap scripts.

*********
SystemTap
*********

개요
====

SystemTap은 실행 중인 프로세스를 동적으로 모니터링하고 추적할 수 있는 도구이다. CUBRID는 SystemTap을 지원하며, 이를 통해 성능 병목 현상의 원인을 찾아낼 수 있다.

SystemTap 스크립트의 기본 아이디어는 이벤트를 명명하고, 거기에 핸들러를 부여하는 것이다. 핸들러는 이벤트가 발생할 때마다 행해져야할 작업을 명시하는 스크립트 문장이다.

SystemTap을 사용하여 CUBRID의 성능을 모니터링하려면 먼저 SystemTap을 설치해야 한다. 설치 이후 C 언어와 비슷한 SystemTap 스크립트를 작성, 수행하여 시스템의 성능을 모니터링할 수 있다.

SystemTap은 Linux OS에서만 지원한다.

SystemTap에 대한 자세한 내용 및 설치 방법에 대해서는 http://sourceware.org/systemtap/index.html\ 을 참고한다.

SystemTap 설치하기
==================

설치 확인
---------

1.  /etc/group 파일에 stapusr, stapdev 그룹 계정이 있는지 확인한다.
    이 계정이 없으면 설치가 되지 않은 상태일 것이다.

2.  stapusr, stapdev 그룹 계정에 CUBRID를 설치할 때 사용한 사용자 계정을 추가한다. 여기서는 cubrid라고 가정한다.

    ::
    
        $ vi /etc/group
        
        stapusr:x:156:cubrid
        stapdev:x:158:cubrid

3.  SystemTap의 수행 가능 여부는 간단히 다음 명령을 실행하여 확인할 수 있다.

    ::

        $ stap -ve 'probe begin { log("hello world") exit() }'

버전
----

CUBRID에서 SystemTap 스크립트를 실행하려면 SystemTap 2.2 이상 버전을 사용해야 한다.

다음은 CentOS 6.3에서 SystemTap을 설치한 예이다. 버전 확인 및 설치 방법은 각 Linux 배포판마다 다를 수 있다.

1.  현재 설치된 SystemTap 버전을 확인한다. 

    ::

        $ sudo yum list|grep  systemtap
        systemtap.x86_64                       1.7-5.el6_3.1                 @update
        systemtap-client.x86_64                1.7-5.el6_3.1                 @update
        systemtap-devel.x86_64                 1.7-5.el6_3.1                 @update
        systemtap-runtime.x86_64               1.7-5.el6_3.1                 @update
        systemtap-grapher.x86_64               1.7-5.el6_3.1                 update
        systemtap-initscript.x86_64            1.7-5.el6_3.1                 update
        systemtap-sdt-devel.i686               1.7-5.el6_3.1                 update
        systemtap-sdt-devel.x86_64             1.7-5.el6_3.1                 update
        systemtap-server.x86_64                1.7-5.el6_3.1                 update
        systemtap-testsuite.x86_64             1.7-5.el6_3.1                 update

2.  SystemTap 2.2 미만 버전이 설치되어 있다면 삭제한다.

    ::

        $ sudo yum remove systemtap-runtime
        $ sudo yum remove systemtap-devel
        $ sudo yum remove systemtap

3.  SystemTap 2.2 이상 버전의 RPM 배포 패키지를 설치한다. RPM 배포 패키지는 (http://rpmfind.net/linux/rpm2html/)에서 찾을 수 있다.

    ::

        $ sudo rpm -ivh systemtap-devel-2.3-3.el6.x86_64.rpm
        $ sudo rpm -ivh systemtap-runtime-2.3-3.el6.x86_64.rpm
        $ sudo rpm -ivh systemtap-client-2.3-3.el6.x86_64.rpm
        $ sudo rpm -ivh systemtap-2.3-3.el6.x86_64.rpm

관련 용어
=========

.. https://sourceware.org/systemtap/wiki/UsingMarkers

마커(Marker)
------------

코드 안에 위치하는 마커는 실행 중에 제공할 수 있는 함수(프로브)를 호출하기 위한 훅(hook)을 제공한다. 마커가 발동될 때마다 사용자가 제공한 함수가 호출되고, 해당 함수가 종료되면 호출자에게 돌아온다.

마커 발동 시 사용자가 정의하는 함수, 즉 프로브는 추적 및 성능 측정을 위해 사용될 수 있다.

프로브(Probe)
-------------

프로브(probe)는 어떤 이벤트가 발생했을 때의 동작을 정의하는 일종의 함수로서, 이벤트와 핸들러로 나뉜다.

SystemTap 스크립트에는 특정 이벤트, 즉 마커가 발생할 때의 동작을 정의한다.

SystemTap 스크립트는 여러 개의 프로브를 가질 수 있으며, 프로브의 핸들러는 프로브 바디(probe body)라고 불린다.

SystemTap 스크립트는 코드의 재컴파일 없이 계측 코드의 삽입을 허용하며 핸들러에 관해 더 많은 유연성을 제공한다. 이벤트는 핸들러가 실행하도록 트리거로 동작한다. 핸들러는 데이터를 기록하고 그것을 출력하도록 명시될 수 있다. 

CUBRID가 제공하는 마커는 :ref:`cubrid-marker`\ 를 참고한다.

.. https://access.redhat.com/site/documentation/en-US/Red_Hat_Enterprise_Linux/5/html-single/SystemTap_Beginners_Guide/#systemtapscript-events

비동기 이벤트
-------------

비동기 이벤트는 내부에서 정의된 것으로, 코드에서 특정 작업이나 위치에 의존적이지 않다. 이러한 계열의 프로브 이벤트는 주로 카운터, 타이머 등이다.

비동기 이벤트의 예는 다음과 같다.

*   begin
    
    SystemTap 세션의 시작. 
    
    예) SystemTap이 시작하는 순간.
    
    
*   end

    SystemTap 세션의 끝.
    
*   timer events

    핸들러가 주기적으로 실행되는 것을 명시하는 이벤트. 
    
    예) 5초마다 "hello world"를 출력한다.
    
    ::
    
        probe timer.s(5)
        {
          printf("hello world\n")
        }

CUBRID에서 SystemTap 사용하기
=============================

CUBRID 소스 빌드
----------------

SystemTap 은 리눅스에서만 사용할 수 있다.

CUBRID 소스를 빌드해 SystemTap을 사용하려면 **ENABLE_SYSTEMTAP** 을 **ON** (기본값)으로 설정한다.

이 옵션은 릴리즈 빌드에 포함되어 있으며, CUBRID 소스 파일을 빌드하지 않고 인스톨 패키지를 이용하여 설치한 사용자라도 SystemTap 스크립트를 이용할 수 있다.

다음은 CUBRID의 소스를 빌드하는 예이다.

::

    build.sh -m release

SystemTap 스크립트 실행
-----------------------

CUBRID에서 SystemTap 스크립트 예제는 $CUBRID/share/systemtap 이하 디렉터리에 제공하고 있다.

다음은 buffer_access.stp 파일을 수행하는 명령의 예이다. 

::

    cd $CUBRID/share/systemtap/scripts
    stap -k buffer_access.stp -o result.txt

결과 출력
---------

특정 스크립트를 수행하면, 스크립트에 기록한 코드에 의해 필요한 정보를 콘솔에 출력한다. -o *filename* 옵션을 명시하는 경우 해당 옵션에 명시한 *filename*\ 에 결과를 기록한다.

다음은 앞서 보인 예제의 출력 결과이다.

::

    Page buffer hit count: 172
    Page buffer miss count: 172
    Miss ratio: 50

.. _cubrid-marker:
    
CUBRID 마커
===========

SystemTap의 가장 유용한 기능은 마커를 사용자 소스 코드(CUBRID 코드) 안에 삽입할 수 있다는 점과 이 마커에 다다를 때 발동하는 프로브를 스크립트에서 작성할 수 있다는 점이다. 

연결 마커
---------

일정 기간 동안 연결 활동(연결 개수, 연결 지속 시간, 평균 연결 지속 시간, 최대 연결 획득  개수 등)과 관련된 분석에 도움이 되는 정보를 수집하는 것은 관심이 가는 일이다. 이러한 모니터링 스크립트를 작성하기 위해서는 연결 시작 마커와 연결 끝 마커가 필요하다.

.. function:: conn_start(connection_id, user)

    서버에서 질의 실행이 시작되면 이 마커가 발동된다.

    :param connection_id: 연결 ID를 포함한 정수값
    :param user: 연결의 사용자 이름.
    
.. function:: conn_end(connection_id, user)

    서버에서 질의 실행이 끝나면 이 마커가 발동된다.
    
    :param connection_id: 연결 ID를 포함한 정수값
    :param user: 연결의 사용자 이름

질의 마커
---------

이벤트 관련 질의 실행을 위한 마커로서, 비록 전체 시스템에 관련된 전체(global) 정보를 포함하지는 않지만 모니터링 작업에서 매우 유용하다. 적어도 아래 두 개의 마커는 가장 기본이 되는 것이다. 질의 실행의 시작과 종료 시에 각각 해당 마커가 발동된다.

.. function:: query_exec_start(query_string, query_id, connection_id, user)

    서버에서 질의 실행이 시작되면 이 마커가 발동된다.
    
    :param query_string: 실행할 질의를 나타내는 문자열
    :param query_id: 질의 식별자
    :param connection_id: 연결 식별자
    :param user: 연결할 때 사용하는 사용자 이름

.. function:: query_exec_end(query_string, query_id, connection_id, user, status)

    서버에서 질의 실행이 끝나면 이 마커가 발동된다.
    
    :param query_string: 실행할 질의를 나타내는 문자열
    :param query_id: 질의 식별자
    :param connection_id: 연결 식별자
    :param user: 연결할 때 사용하는 사용자 이름
    :param status: 질의 실행 시 반환 상태(Success, Error)

객체 연산 마커
--------------

저장 엔진을 포함하는 연산들은 치명적이며 테이블이나 객체 수준에서 업데이트를 조사하는 것(probing)은 데이터베이스의 동작을 모니터링하는 데 큰 도움이 된다. 객체가 매번 INSERT/UPDATE/DELETE될 때마다 마커가 발동되는데, 이 점은 모니터링 스크립트와 서버 둘 다에 성능 상 약점이 될 수 있다.

.. function:: obj_insert_start(table)

    이 마커는 객체가 삽입되기 전에 발동된다.

    :param table: 이 연산의 대상 테이블
    
.. function:: obj_insert_end(table, status)

    이 마커는 객체가 삽입된 이후에 발동된다.
    
    :param table: 이 연산의 대상 테이블
    :param status: 이 연산의 성공 여부를 나타내는 값
    
.. function:: obj_update_start(table)

    이 마커는 객체가 갱신되기 전에 발동된다.
    
    :param table: 이 연산의 대상 테이블

.. function:: obj_update_end(table, status)

    이 마커는 객체가 갱신된 후에 발동된다.
    
    :param table: 이 연산의 대상 테이블
    :param status: 이 연산의 성공 여부를 나타내는 값
    
.. function:: obj_deleted_start(table)

    이 마커는 객체가 삭제되기 전에 발동된다.

    :param table: 이 연산의 대상 테이블

.. function:: obj_delete_end(table, status)

    이 마커는 객체가 삭제된 후에 발동된다.
    
    :param table: 이 연산의 타겟 테이블
    :param status: 이 연산의 성공 여부를 나타내는 값
    
인덱스 연산 마커
----------------

위의 마커는 테이블 기반 마커이고, 다음은 인덱스 기반 마커이다.

잘못된 인덱스의 사용은 시스템에서 많은 문제를 유발하는 원인이 될 수 있으며, 인덱스를 모니터링할 수 있다는 점은 매우 유용하다. 아래 마커들은 테이블에서 사용된 마커와 매우 유사한데, 인덱스가 테이블과 같은 연산을 지원하기 때문이다.

.. function:: idx_insert_start(classname, index_name) 

    이 마커는 B-Tree에 인덱스 노드를 삽입하기 전에 발동된다.

    :param classname: 대상 인덱스의 테이블 이름
    :param index_name: 대상 인덱스 이름
    
.. function:: idx_insert_end(classname, index_name, status)

    이 마커는 B-Tree에 인덱스 노드를 삽입한 이후에 발동된다.

    :param classname: 대상 인덱스의 테이블 이름
    :param index_name: 대상 인덱스 이름
    :param status: 연산의 성공 여부를 나타내는 값
    
.. function:: idx_update_start(classname, index_name)

    이 마커는 B-Tree에서 인덱스 노드를 갱신하기 전에 발동된다.

    :param classname: 대상 인덱스의 테이블 이름
    :param index_name: 대상 인덱스 이름
    
.. function:: idx_update_end(classname, index_name, status)

    이 마커는 B-Tree에서 인덱스 노드를 갱신한 이후에 발동된다.
    
    :param classname: 대상 인덱스의 테이블 이름
    :param index_name:  대상 인덱스 이름
    :param status: 연산의 성공 여부를 나타내는 값
    
.. function:: idx_delete_start(classname, index_name)

    이 마커는 B-Tree에서 인덱스 노드를 삭제하기 전에 발동된다.

    :param classname: 대상 인덱스의 테이블 이름
    :param index_name: 대상 인덱스 이름
    
.. function:: idx_delete_end(classname, index_name, status)

    이 마커는 B-Tree에서 인덱스 노드를 삭제한 후에 발동된다.

    :param classname: 대상 인덱스의 테이블 이름
    :param index_name: 대상 인덱스 이름
    :param status: 연산의 성공 여부를 나타내는 값
    
잠금(locking) 마커
------------------

잠금 이벤트를 포함하는 마커는 아마도 전체를 모니터링하는 작업에서 가장 중요할 것이다. 잠금 시스템은 서버 성능과에 큰 영향을 끼치며, 잠금 대기 시간 및 카운트(교착 상태 및 회피된 트랜잭션 개수)는 문제를 찾는데 매우 유용하다.

.. function:: lock_acquire_start(OID, table, type)

    이 마커는 잠금이 요청되기 전에 발동된다.
    
    :param OID: 잠금 요청 대상 객체 ID
    :param table: 객체를 유지하고 있는 테이블
    :param type: 잠금 타입(X_LOCK, S_LOCK 등)
    
.. function:: lock_acquire_end(OID, table, type)

    이 마커는 잠금 요청이 완료된 이후에 발동된다.

    :param OID: 잠금 요청 대상 객체 ID
    :param table: 객체를 유지하고 있는 테이블
    :param type: 잠금 타입(X_LOCK, S_LOCK etc.)
    :param status: Value showing whether the request has been granted or not.
    
.. function:: lock_release_start(OID, table, type)

    이 마커는 잠금이 해제된 이후에 발동된다.

    :param OID: 잠금 요청 대상 객체 ID
    :param table: 객체를 유지하고 있는 테이블
    :param type: 잠금 타입(X_LOCK, S_LOCK etc.)
    
.. function:: lock_release_end(OID, table, type)

    This marker should be triggered after a lock release operation has been completed.

    :param OID: 잠금 요청 대상 객체 ID
    :param table: 객체를 유지하고 있는 테이블
    :param type: 잠금 타입(X_LOCK, S_LOCK etc.)
    
트랜잭션 마커
-------------

서버 모니터링에서 관심있게 봐야 할 측정 대상은 트랜잭션의 활동이다. 간단한 예로, 트랜잭션이 취소된 개수는 교착 상태가 발생한 개수와 밀접하게 관련되어 있으며, 매우 중요한 성능 식별자라 할 수 있다. 또 다른 직관적인 사용 예는 간단한 SystemTap 스크립트를 사용하여 TPS와 같은 시스템 성능 통계를 수집하는 방법을 단순화하는 것이다.

.. function:: tran_commit(tran_id)

    이 마커는 트랜잭션이 성공적으로 완료된 이후에 발동된다.

    :param tran_id: 트랜잭션 식별자
    
.. function:: tran_abort(tran_id, status)

    이 마커는 트랜잭션이 중단(abort)된 이후에 발동된다.

    :param tran_id: 트랜잭션 식별자
    :param status: 종료 상태

.. function:: tran_start(tran_id)

    이 마커는 트랜잭션이 시작된 이후에 발동된다.

    :param tran_id: 트랜잭션 식별자
    
.. function:: tran_deadlock()

    이 마커는 교착상태가 감지된 이후에 발동된다.

I/O 마커
--------

I/O 액세스는 RDBMS의 주요 병목(bottleneck)이며, I/O 성능을 모니터링하는 마커가 제공되어야 한다.
이 마커를 통해 사용자는 I/O 페이지 액세스 시간을 측정하고, 이 측정에 기반하여 다양하고 복잡한 통계를 집계할 수 있다.

.. function:: pgbuf_hit() 

    이 마커는 페이지 버퍼에서 요청 페이지가 발견되어 디스크에서 그것을 검색할 필요가 없을 때 발동된다.
    
.. function:: pgbuf_miss()

    이 마커는 페이지 버퍼에서 요청 페이지가 발견되지 않아 디스크에서 그것을 검색해야 할 때 발동된다.

.. function:: io_write_start (query_id)

    이 마커는 디스크에 페이지를 기록하는 프로세스가 시작할 때 발동된다.

    :param query_id: 질의 식별자

.. function:: io_write_end(query_id, size, status)

    이 마커는 디스크에 페이지를 기록하는 프로세스가 종료될 때 발동된다.

    :param query_id: 질의 식별자
    :param size: 기록되는 바이트 수
    :param status: 연산이 성공적으로 종료되었는지 여부를 나타내는 값

.. function:: io_read_start(query_id)

    이 마커는 디스크에서 페이지를 읽는 작업이 시작될 때 발동된다.

    :param query_id: 질의 식별자

.. function:: io_read_end (query_id, size, status)

    이 마커는 디스크에서 페이지를 읽는 작업이 종료될 때 발동된다.

    :param query_id: 질의 식별자
    :param size: 읽은 바이트 수
    :param status: 연산이 성공적으로 종료되었는지 여부를 나타내는 값

기타 마커
---------

.. function:: sort_start ()

    이 마커는 정렬 연산이 시작될 때 발동된다.
    
.. function:: sort_end (nr_rows, status)

    이 마커는 정렬 연산이 완료될 때 발동된다.

    :param nr_rows: 정렬되는 행의 개수
    :param status: 연산이 성공적으로 종료되었는지 여부
