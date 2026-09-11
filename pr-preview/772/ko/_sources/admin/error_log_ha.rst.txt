CUBRID HA 오류
=================



.. _ERROR-898:

**ERROR CODE: -898, 'Replication error: "%1$s"'**

- 이 메시지는 CUBRID 데이터베이스의 HA(High Availability) 복제 환경에서 복제 작업 중에 다양한 문제가 발생할 때 나타납니다, 즉, 데이터베이스 복제 과정에서 메모리 할당, 클래스 정보 처리, 복제 로그 생성 시 발생합니다.


.. _ERROR-970:

**ERROR CODE: -970, 'Server HA mode is changed from '%1$s' to '%2$s'.'**

- 이 메시지는 CUBRID HA(High Availability) 복제 환경에서 모드가 변경될 때 발생하는 시스템 알림입니다. 서버의 상태가 active, standby, maintenance 등의 상태 간에 전환될 때 발생하며, 이는 시스템의 정상적인 HA 운영 과정의 일부입니다.


.. _ERROR-975:

**ERROR CODE: -975, 'Current version of replication does not allow changing multiple rows with a single UPDATE statement which can violate the UNIQUE constraint.'**

- 이 메시지는 CUBRID HA(High Availability) 복제 환경에서 발생하는 특별한 제한사항과 관련이 있습니다. 일반적으로 단일 UPDATE 문으로 여러 행을 변경하는 것은 허용되지만, HA 복제가 활성화된 환경에서는 UNIQUE 제약 조건을 위반할 수 있는 다중 행 UPDATE 작업이 제한됩니다, 이는 복제 환경에서 데이터 일관성을 보장하고, 마스터와 슬레이브 간의 동기화 문제를 방지하기 위한 보호성 오류입니다. HA 모드가 활성화된 경우(`ha_mode`가 `off`가 아닌 경우), 시스템은 UNIQUE 제약 조건 위반 가능성이 있는 다중 행 UPDATE 작업을 거부합니다.


.. _ERROR-986:

**ERROR CODE: -986, 'CUBRID heartbeat feature started.'**

- 이 메시지는 CUBRID HA(High Availability) 복제 환경에서 heartbeat 서브시스템이 성공적으로 시작 되었음을 알리는 정보성 알림입니다. 하트비트 기능은 CUBRID 클러스터 환경에서 각 노드의 상태를 주기적으로 확인하고, 장애 발생 시 이를 감지하여 자동 전환(Failover)을 수행하는 데 필수적인 역할을 합니다. 이 메시지는 일반적으로 CUBRID 마스터 프로세스 시작 시, cubrid heartbeat start 명령 실행 시, 또는 HA 모드가 활성화될 때 발생하며, 시스템이 고가용성 모드로 정상 작동하기 시작했음을 의미합니다.


.. _ERROR-987:

**ERROR CODE: -987, 'CUBRID heartbeat feature stopped.'**

- 이 메시지는 CUBRID HA(High Availability) 복제 환경에서 HA heartbeat 서브시스템이 비활성화 되었음을 알리는 알림성 이벤트입니다. 관리자 중지 명령, 마스터 종료/재시작, HA 모드 OFF 전환, 또는 비활성화 요청 처리 흐름에서 발생하며, 이후 장애 감지와 자동 전환 기능이 중단됩니다.


.. _ERROR-988:

**ERROR CODE: -988, 'Node event: %1$s.'**

- 이 메시지는 CUBRID HA(High Availability) 시스템에서 노드의 상태 변경이나 이벤트 발생을 알리는 정보성 메시지입니다. 노드의 상태 모니터링과 관련된 다양한 이벤트(연결, 연결 해제, 상태 변경 등)를 기록합니다.


.. _ERROR-989:

**ERROR CODE: -989, 'Process event: %1$s. %2$s'**

- 이 메시지는 CUBRID HA(High Availability) 시스템에서 프로세스 이벤트 발생을 알리는 시스템 메시지입니다. 서버의 상태 변화나 중요 이벤트를 추적하고 기록하는 데 사용되는 핵심 모니터링 메시지입니다.


.. _ERROR-990:

**ERROR CODE: -990, 'Command execution: %1$s. %2$s'**

- 이 메시지는 CUBRID HA(High Availability) 시스템 내부에서 특정 명령을 실행하는 과정에서 문제가 발생 했음을 나타냅니다. 특히, 이 에러는 CUBRID 마스터 프로세스의 핵심 기능인 heartbeat 관리 또는 요청 처리 중에 명령 실행 실패가 발생 했음을 시사합니다, 실제 오류 발생 시 구체적인 명령 내용이나 실패 원인에 대한 추가 정보를 제공되며, 이는 시스템 내부의 중요한 프로세스가 정상적으로 작동하지 못했음을 의미하고, 데이터베이스의 안정성과 가용성에 직접적인 영향을 미칠 가능성이 있습니다.


.. _ERROR-1023:

**ERROR CODE: -1023, 'log applier filter: %1$s.'**

- 이 메시지는 CUBRID HA(High Availability) 복제 환경에서, 액티브(Active) 서버(마스터)의 변경 로그를 스탠바이(Standby) 서버(슬레이브/리플리카)에 적용하는 로그 적용기(Log Applier)가 사용자가 정의한 복제 필터 규칙을 처리하던 중 일반적인(Generic) 오류를 만났을 때 발생합니다, 복제 필터는 특정 테이블이나 데이터 변경 연산(INSERT, UPDATE, DELETE)을 복제 대상에서 제외하거나 포함시키는 규칙을 정의하는데, 이 규칙의 처리 과정에서 문제가 생겼음을 의미합니다.


.. _ERROR-1024:

**ERROR CODE: -1024, 'log writer: failed to get log page(s) starting from page id %1$lld.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 copylogdb 프로세스가 복제 대상 server로 부터 복제 로그를 요청시 발생합니다.


.. _ERROR-1025:

**ERROR CODE: -1025, 'HA delay info: The replication delay (%1$d secs) has exceeded the limit (%2$d secs).'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 마스터 서버와 스탠바이 서버 간의 복제 지연 시간이 사용자가 설정한 임계값(ha_delay_limit, ha_delay_limit_delta)을 초과했음을 나타냅니다. 즉, 스탠바이 서버가 마스터 서버의 최신 변경 사항을 따라잡는 데 설정된 시간보다 오래 걸리고 있다는 의미입니다. 이는 마스터와 스탠바이 간의 데이터 동기화 상태가 양호하지 않음을 알리는 경고성 또는 정보성 메시지일 수 있으며, 설정에 따라 특정 동작(예: 마스터의 추가 트랜잭션 지연)을 유발할 수도 있습니다.


.. _ERROR-1026:

**ERROR CODE: -1026, 'HA delay info: The replication delay (%1$d secs) is decreased below the acceptable level (%2$d secs).'**

- 이 메시지는 이전에 발생했던 CUBRID HA(High Availability) 환경에서 복제 지연 상태가 해소되었음을 알리는 정보성 로그입니다. 즉, 액티브(마스터) 서버와 스탠바이 서버 간의 복제 지연 시간이 다시 사용자가 설정한 임계값(ha_delay_limit, ha_delay_limit_delta) 이하로 줄어들어, 데이터 동기화 상태가 정상 범위로 복귀했음을 의미합니다.


.. _ERROR-1027:

**ERROR CODE: -1027, 'log applier: failed to change apply state from '%1$s' to '%2$s'.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 내부 시스템로 부터 applylogdb의 동작 상태 변경 요청시 변경할 수 없는 경우 발생하는 오류 입니다.


.. _ERROR-1028:

**ERROR CODE: -1028, 'log applier: unexpected EOF record in archive log. LSA: %1$lld|%2$d.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 복제된 archive log에서 예상치 않은 로그(END of LOG)을 만나는 경우 발생하는 오류 메세지로 applylogdb 프로세스는 해당 로그 레코드를 적용하지 않고 진행됩니다.


.. _ERROR-1029:

**ERROR CODE: -1029, 'log applier: invalid replication log page/offset. page HDR: %1$1lld|%2$d, final: %3$lld|%4$d, append LSA: %5$lld|%6$d, EOF LSA: %7$lld|%8$d, ha file status: %9$d, is end-of-log: %10$d.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 applylogdb 프로세스가 복제 로그를 처리하는 과정에서 유효하지 않은 페이지 ID나 오프셋을 발견했을 때 발생합니다. 로그 레코드의 위치 정보가 올바르지 않아 복제 작업을 진행할 수 없는 상태를 나타냅니다.


.. _ERROR-1030:

**ERROR CODE: -1030, 'log applier: invalid replication record. LSA: %1$lld|%2$d, forw LSA: %3$lld|%4$d, backw LSA: %5$lld|%6$d, Trid: %7$d, prev tran LSA: %8$lld|%9$d, type: %10$d.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 applylogdb 프로세스가 복제 로그 레코드를 검증하는 과정에서 발생합니다, 검증 중 로그 레코드의 LSA 체인(순방향·역방향 연결)이나 트랜잭션 정보가 올바르지 않은 것으로 판단될 경우 해당 오류가 발생됩니다.


.. _ERROR-1031:

**ERROR CODE: -1031, 'log applier: failed to apply the following statement. class: "%1$s", statement: "%2$s", server error: %3$d, %4$s.'**

- 이 메시지는 CUBRID HA(High Availability) 환경의 스탠바이 서버에서 applylogdb 프로세스가 마스터 서버로부터 받은 변경 로그(SQL 문장 형태)를 스탠바이 데이터베이스에 실제로 적용(실행)하는 과정에서 실패했을 때 발생합니다. 즉, 로그 자체는 정상적으로 수신하고 해석했지만, 스탠바이 서버의 현재 데이터베이스 상태에서 해당 SQL(주로 DDL ) 구문을 실행할 수 없었음을 의미합니다. 실패 원인은 스키마 불일치, 데이터 충돌(예: 고유 키 위반), 제약 조건 위반, 리소스 부족 등 다양하며, 메시지에 포함된 '서버 오류' 상세 오류 정보가 구체적인 실패 사유를 알려줍니다.


.. _ERROR-1032:

**ERROR CODE: -1032, 'log applier: failed to apply insert replication log. class: "%1$s", key: "%2$s", server error: %3$d, %4$s'**

- 이 메시지는 CUBRID HA(High Availability) 환경의 스탠바이 서버에서 applylogdb 프로세스가 마스터 서버로부터 복제된 `INSERT` 로그를 스탠바이 데이터베이스에 적용(실행)하려다 실패했을 때 발생합니다. 가장 흔한 원인은 스탠바이 서버에 해당 키를 가진 레코드가 이미 존재하여 기본 키 또는 고유 키 제약 조건 위반(Duplicate Key)이 발생하는 경우입니다. 그 외에도 스키마 불일치, 다른 제약 조건 위반 등 INSERT 문 실행을 막는 다양한 원인이 있을 수 있으며, 포함된 '서버 오류' 정보가 실패의 직접적인 이유를 알려줍니다.


.. _ERROR-1033:

**ERROR CODE: -1033, 'log applier: failed to apply update replication log. class: "%1$s", key: "%2$s", server error: %3$d, %4$s'**

- 이 메시지는 CUBRID HA(High Availability) 환경의 스탠바이 서버에서 applylogdb 프로세스가 마스터 서버로부터 복제된 `UPDATE` 로그를 스탠바이 데이터베이스에 적용(실행)하려다 실패했을 때 발생합니다. 이는 특정 사례로, UPDATE 작업이 스탠바이 서버에서 거부되었음을 의미합니다. 주요 원인으로는 업데이트 대상 레코드가 스탠바이 서버에 존재하지 않거나, 업데이트 결과가 고유 키/기본 키 제약 조건 또는 다른 제약 조건(NOT NULL, FK 등)을 위반하거나, 스키마 불일치 등이 있고, 메시지에 포함된 '서버 오류' 정보가 실패의 구체적인 이유입니다.


.. _ERROR-1034:

**ERROR CODE: -1034, 'log applier: failed to apply delete replication log. class: "%1$s", key: "%2$s", server error: %3$d, %4$s'**

- 이 메시지는 CUBRID HA(High Availability) 환경의 스탠바이 서버에서 applylogdb 프로세스가 마스터 서버로부터 복제된 `DELETE` 로그를 스탠바이 데이터베이스에 적용(실행)하려다 실패했을 때 발생합니다. 이는 특정 사례로, DELETE 작업이 스탠바이 서버에서 거부되었음을 의미합니다. 주요 원인으로는 업데이트 대상 레코드가 스탠바이 서버에 존재하지 않거나, 업데이트 결과가 고유 키/기본 키 제약 조건 또는 다른 제약 조건(NOT NULL, FK 등)을 위반하거나, 스키마 불일치 등이 있고, 메시지에 포함된 '서버 오류' 정보가 실패의 구체적인 이유입니다.


.. _ERROR-1035:

**ERROR CODE: -1035, 'log applier: mem size(%1$d MB) of log applier is greater than max mem size (%2$d MB) or has been grow more than 2 times (%3$d MB). required LSA: %4$lld|%5$d. last committed LSA: %6$lld|%7$d.'**

- 이 메시지는 CUBRID HA(High Availability) 환경의 스탠바이 서버에서 applylogdb 프로세스가 사용하는 메모리 양이 설정된 최대 허용치(`ha_log_applier_max_mem_size`)를 초과하거나, 비정상적으로 빠르게 (단기간에 2배 이상) 증가했을 때 발생합니다, 이는 스탠바이 서버의 자원 고갈을 방지하고 시스템 안정성을 유지하기 위한 보호 메커니즘으로 주로 마스터 서버에서 매우 큰 트랜잭션(많은 로그를 생성하는)이 발생했거나, 스탠바이 서버의 처리 성능 부족 또는 네트워크 지연 등으로 인해 로그 적용이 지연되어 처리해야 할 로그가 메모리에 과도하게 쌓일 때 발생할 수 있습니다. 'required LSA'와 'last committed LSA'의 차이가 크다면, 로그 적용이 상당히 지연되고 있음을 나타냅니다.


.. _ERROR-1036:

**ERROR CODE: -1036, 'log applier: log applier shut itself down by signal.'**

- 이 메시지는 CUBRID의 HA(High Availability) 시스템에서 발생하는 applylogdb 프로세스의 정상적인 종료 상황을 나타냅니다, 시스템 시그널을 받아 로그 복제 프로세스가 정상적인 종료 절차를 수행했음을 의미합니다. 이는 일반적으로 계획된 시스템 종료나 관리자의 의도적인 종료 요청에 의해 발생합니다.


.. _ERROR-1037:

**ERROR CODE: -1037, 'log writer: log writer shut itself down by signal.'**

- 이 메시지는 CUBRID의 HA(High Availability) 시스템에서 발생하는 copylogdb 로그 복제 프로세스의 정상적인 종료 상황을 나타냅니다, 시스템 시그널을 받아 로그 복제 프로세스가 정상적인 종료 절차를 수행했음을 의미합니다, 이는 일반적으로 계획된 시스템 종료나 관리자의 의도적인 종료 요청에 의해 발생합니다.


.. _ERROR-1038:

**ERROR CODE: -1038, 'log applier: log applier started. required LSA: %1$lld|%2$d. last committed LSA: %3$lld|%4$d. last committed repl LSA: %5$lld|%6$d.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 applylogdb 프로세스가 시작될 때 출력되는 정보성 메시지입니다. 시작 시점의 LSA 정보를 포함하여 로그 복제의 시작 위치와 마지막 커밋 상태를 나타냅니다. LSA는 로그의 위치를 식별하는 주소값으로, 데이터 동기화의 기준점이 됩니다


.. _ERROR-1039:

**ERROR CODE: -1039, 'log writer: log writer started. mode: %1$d.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 copylogdb 프로세스가 시작될 때 출력되는 정보성 메시지입니다. 시작 시점의 LSA 정보를 포함하여 로그 복제의 시작 위치와 마지막 커밋 상태를 나타냅니다. LSA는 로그의 위치를 식별하는 주소값으로, 데이터 동기화의 기준점이 됩니다, 이는 copylogdb 프로세스의 정상 구동을 알리는 메시지입니다.


.. _ERROR-1040:

**ERROR CODE: -1040, 'HA generic: %1$s.'**

- 이 메시지는 CUBRID HA(High Availability) 시스템 내에서 발생한 문제 중, 특정 오류 코드로 분류되지 않은 예외적이거나 일반적인 오류 상황을 나타냅니다. 즉, HA 관련 기능(heartbeat, copylogdb, applylogdb 등)을 수행하는 도중 예상치 못한 내부 상태나 외부 요인으로 인해 더 이상 진행할 수 없을 때 사용되는 포괄적인 오류 코드입니다. 따라서 에러 메시지에 포함된 여러 상세한 오류 메시지들의 내용이 실제 오류의 원인을 진단하는데 결정적인 단서가 됩니다.


.. _ERROR-1105:

**ERROR CODE: -1105, 'Internal error: partially failed to flush dirty object.'**

- 이 메시지는 CUBRID 데이터베이스에서 메모리에 있는 변경된 데이터(dirty object)를 디스크로 저장하는 과정에서 일부만 성공하고 일부가 실패했을 때 발생합니다, 즉, 트랜잭션의 일부 변경사항은 성공적으로 디스크에 저장되었지만, 나머지 일부는 저장에 실패했음을 의미합니다, 이는 데이터 무결성과 시스템 안정성을 보장하기 위한 내부 오류로, 주로 applylogdb에서 데이터 반영시 발생할 수 있는 오류로 복제(replication) 과정에서 발생할 수 있습니다.


.. _ERROR-1122:

**ERROR CODE: -1122, 'Time out in receiving data.'**

- 이 메시지는 CUBRID HA(High Availability) 시스템 내에서 발생한 문제 중 copylogdb 프로세스에서 발생하는 실행 오류로 cub_server 프로세스로 부터 log page를 요청 후 timeout 까지 요청한 log page를 받지 못한 경우 발생합니다.


.. _ERROR-1133:

**ERROR CODE: -1133, 'Copylogdb for %1$s already exists.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 copylogdb 가 cub_master와 연결 요청시 이미 copylogdb가 구동 중일때 발생합니다.


.. _ERROR-1134:

**ERROR CODE: -1134, 'Applylogdb for %1$s already exists.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 applylogdb가 cub_master와 연결 요청시 이미 applylogdb가 구동 중일때 발생합니다.


.. _ERROR-1139:

**ERROR CODE: -1139, 'Handshake error (peer host %1$s): incompatible read/write mode. (client: %2$s, server: %3$s)'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 데이터베이스 클라이언트와 서버 간의 핸드쉐이크 과정에서 읽기/쓰기 모드의 호환성 문제가 발생할 때 나타납니다, 즉, 데이터베이스 클라이언트와 데이터베이스 서버간 핸드쉐이크 과정에서 클라이언트와 서버의 모드(RW/RO)가 다른 경우 발생합니다.


.. _ERROR-1140:

**ERROR CODE: -1140, 'Handshake error (peer host %1$s): HA replication delayed.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 데이터베이스 서버 간 복제 과정에서 지연이 발생할 때 나타납니다. 마스터-슬레이브 간의 데이터 동기화가 설정된 임계값을 초과하여 지연되는 상황을 나타내는 오류입니다.


.. _ERROR-1141:

**ERROR CODE: -1141, 'Handshake error (peer host %1$s): replica-only client to non-replica server.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 리플리카 전용으로 설정된 서버에 클라이언트가 직접 접근을 시도할 때 발생합니다, 즉,  데이터베이스 클라이언트와 데이터베이스 서버간 핸드쉐이크 과정에서 리플리카 전용 클라이언트(replica broker)가 리플리카 서버가 아닌 마스터, 슬레이브 서버로 연결 요청 시 발생합니다.


.. _ERROR-1142:

**ERROR CODE: -1142, 'Handshake error (peer host %1$s): unidentified server version.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 클라이언트가 데이터베이스 서버에 연결을 시도할 때 서버의 버전 정보를 확인할 수 없는 경우 발생합니다. 클라이언트-서버 간의 핸드쉐이크 과정에서 서버 버전 정보가 올바르게 전달되지 않거나 호환되지 않는 상황을 나타냅니다.


.. _ERROR-1143:

**ERROR CODE: -1143, 'Handshake error (peer host %1$s): remote access to server not allowed.'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 데이터베이스 클라이언트가(csql, cas, copylogdb, applylogdb등..) 서버 간의 버전 정보를 식별할 수 없을 때 발생하는 오류입니다.


.. _ERROR-1144:

**ERROR CODE: -1144, 'Timed out attempting to connect to %1$s. (timeout: %2$d sec(s))'**

- 이 메시지는 CUBRID 클라이언트 또는 구성 요소(예: CSQL, JDBC 드라이버 등)가 지정된 서버에 연결을 시도했으나 타임아웃 설정된 시간 이내에 응답을 받지 못해 연결이 실패했을 때 발생하는 오류입니다, 일반적으로 네트워크 장애, 서버 비정상 상태, 방화벽 차단, 포트 오픈 실패 등의 원인으로 발생할 수 있습니다.

- 참조: 이 메시지는 큐브리드 매뉴얼 CUBRID HA 복제 불일치 감지 부분에 "장비 down"으로 정리되어 있습니다.


.. _ERROR-1170:

**ERROR CODE: -1170, 'Failed to flush replication items (error code : %1$d).'**

- 이 메시지는 CUBRID HA(High Availability) 환경에서 데이터베이스의 복제(replication) 과정에서 복제 항목, 즉 마스터 데이터베이스에서 발생한 변경 사항들을 슬레이브(replica) 데이터베이스에 적용하기 위해 버퍼에서 디스크로 쓰거나 다음 단계로 전달하는 작업이 실패했음을 나타냅니다. 이는 네트워크 문제, 디스크 I/O 문제, 슬레이브 데이터베이스의 내부 상태 문제, 또는 복제 항목 자체의 문제 등 다양한 원인으로 발생할 수 있습니다.

