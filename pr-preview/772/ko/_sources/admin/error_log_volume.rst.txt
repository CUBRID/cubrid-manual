데이터베이스 파일 오류
======================


.. _ERROR-17:

**ERROR CODE: -17, 'Internal error: fetching deallocated pageid %1$d of volume "%2$s".'**

- 이 메시지는 CUBRID 시스템이 이미 할당 해제된(deallocated) 페이지 ID를 가져오려고 시도했을 때 발생하는 내부 오류입니다, 이 메시지는 페이지 할당/해제 관리, 메모리 관리, 또는 페이지 접근 로직에 문제가 발생했음을 의미하며, 데이터베이스의 일관성 손상 및 데이터 손실로 이어질 수 있습니다.


.. _ERROR-19:

**ERROR CODE: -19, 'Internal error: pageptr = %1$p of page %2$d of volume "%3$s" is not fixed.'**

- 이 메시지는 CUBRID 시스템이 특정 볼륨의 특정 페이지에 대한 페이지 포인터가 메모리에서 "고정(fixed)"되지 않은 상태에서 접근하려고 시도했을 때 발생하는 내부 오류입니다, 이 메시지는 페이지 버퍼 관리, 메모리 보호, 또는 페이지 접근 로직에 문제가 발생했음을 의미하며, 데이터베이스의 일관성 손상 및 데이터 손실로 이어질 수 있습니다.


.. _ERROR-35:

**ERROR CODE: -35, 'Internal error: Unknown volume identifier %1$d.'**

- 이 메시지는 CUBRID 시스템이 특정 볼륨 ID(Volume Identifier)를 가진 볼륨을 참조하려고 시도했지만, 해당 볼륨 ID에 해당하는 볼륨을 찾을 수 없거나 유효하지 않은 볼륨으로 인식했을 때 발생하는 내부 오류입니다, 주로 임시 볼륨(temp temp volume)을 삭제 시 발생할 수 있습니다. 


.. _ERROR-38:

**ERROR CODE: -38, 'Internal error: Unknown file VFID %1$d|%2$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 loaddb 명령어 수행 시 오브젝트파일(object file)을 찾을 수 없거나 파일 권한 문제등으로 파일을 열지 못하는 경우 발생합니다.


.. _ERROR-43:

**ERROR CODE: -43, 'Internal error: The page %1$d of volume "%2$s" may be corrupted. %3$d records was found when %4$d was expected.'**

- 이 메시지는 CUBRID 시스템이 특정 볼륨의 슬롯 페이지를 검사하는 동안, 페이지 내에서 발견된 실제 레코드의 수가 페이지 헤더에 기록된 예상 레코드의 수와 일치하지 않음을 감지했을 때 발생하는 내부 오류입니다, 이 불일치는 슬롯 페이지의 내부 구조가 손상되었음을 강력히 시사하며, 데이터베이스의 일관성이 깨졌음을 나타냅니다, 이 메시지는 주로 페이지의 슬롯(slot) 정리 작업 수행 시 발생됩니다.


.. _ERROR-44:

**ERROR CODE: -44, 'No space available in slotted page %1$d of volume "%2$s".'**

- 이 메시지는 CUBRID 시스템이 특정 볼륨의 슬롯 페이지에 새로운 데이터를 삽입하거나 기존 데이터를 확장하려고 시도했지만, 해당 페이지에 더 이상 사용 가능한 공간이 없음을 나타내는 오류입니다, 주로 페이지가 가득 찼을 때 발생하며, 새로운 레코드 삽입, 기존 레코드 업데이트(크기 증가), 또는 인덱스 엔트리 추가와 같은 작업에 실패할 수 있습니다.


.. _ERROR-45:

**ERROR CODE: -45, 'Slot %1$d on page %2$d of volume "%3$s" is allocated to an anchored record. A new record cannot be inserted here.'**

- 이 메시지는 CUBRID 시스템이 특정 볼륨의 특정 페이지에 있는 슬롯에 새로운 레코드를 삽입하려고 시도했지만, 해당 슬롯이 이미 '고정된 레코드(anchored record)'에 할당되어 있어 삽입할 수 없음을 나타내는 내부 오류입니다, 데이터베이스의 내부 구조(특히 슬롯 페이지 구조)에 문제가 있거나, 레코드 삽입 로직에 오류가 있음을 나타내며, 데이터 삽입 실패, 쿼리 오류, 또는 데이터 손실로 이어질 수 있습니다.


.. _ERROR-46:

**ERROR CODE: -46, 'Internal error: slot %1$d on page %2$d of volume "%3$s" is not allocated.'**

- 이 메시지는 CUBRID 시스템이 특정 볼륨의 특정 페이지에 있는 슬롯에 접근하려고 시도했지만, 해당 슬롯이 할당되지 않은 상태임을 감지했을 때 발생하는 내부 오류입니다,  데이터베이스의 내부 구조(특히 슬롯 페이지 구조)에 문제가 있음을 나타내며, 데이터 접근 실패, 쿼리 오류, 또는 데이터 손실로 이어질 수 있습니다, 이는 주로 데이터베이스 페이지에서 슬롯 ID로 접근시 해당 슬롯이 없는 경우 발생합니다.


.. _ERROR-47:

**ERROR CODE: -47, 'Unable to create a heap file in volume "%1$s".'**

- 이 메시지는 CUBRID 시스템이 지정된 볼륨에 새로운 힙 파일(heap file)을 생성하려고 시도했지만 실패했음을 나타내는 오류입니다, 주로 디스크 공간 부족, 파일 시스템 권한 문제, 볼륨 손상, 또는 CUBRID 내부의 파일 시스템 관리 문제로 인해 발생할 수 있습니다, 이는 주로 새로운 테이블 생성, 인덱스 생성, 또는 기존 테이블에 데이터 추가와 같은 작업에 직접적인 영향을 미칩니다.


.. _ERROR-48:

**ERROR CODE: -48, 'Accessing deleted object %1$d|%2$d|%3$d.'**

- 이 메시지는 CUBRID 시스템이 이미 삭제되었거나 유효하지 않은 객체에 접근하려고 시도했을 때 발생하는 내부 오류입니다, 객체에 대한 참조가 객체 자체의 삭제 이후에도 남아있어, 시스템이 유효하지 않은 객체에 접근하려 할 때 발생할 수 있는 내부적인 불일치를 나타냅니다, 결과적으로 데이터 접근 실패, 쿼리 오류, 또는 시스템 불안정으로 이어질 수 있습니다.


.. _ERROR-49:

**ERROR CODE: -49, 'Internal error: class of object %1$d|%2$d|%3$d is unknown.'**

- 이 메시지는 CUBRID 시스템이 특정 객체(OID로 식별되는)의 테이블 정보를 찾을 수 없을 때 발생하는 내부 오류입니다, 객체와 테이블 간의 연결이 손상되었거나, 테이블 메타데이터가 유효하지 않거나, 데이터베이스의 내부 일관성이 깨졌음을 나타냅니다, 결과적으로 데이터 접근 실패, 쿼리 오류, 또는 데이터 손실로 이어질 수 있습니다.


.. _ERROR-52:

**ERROR CODE: -52, 'Internal error: object overflow address %1$d|%2$d|%3$d may be corrupted.'**

- 이 메시지는 CUBRID 시스템이 객체의 오버플로우 주소가 손상되었을 가능성을 감지했을 때 발생하는 내부 오류입니다, 데이터베이스의 내부 구조 관리에 문제가 있음을 나타내며, 데이터 접근 실패, 쿼리 오류, 또는 데이터 손실로 이어질 수 있습니다.


.. _ERROR-53:

**ERROR CODE: -53, 'Fetching object %1$d|%2$d|%3$d when only its OID has been assigned.'**

- 이 메시지는 CUBRID 시스템이 특정 객체의 데이터를 가져오려고 시도했지만, 해당 객체에는 OID(Object Identifier)만 할당되어 있고 실제 데이터가 저장되지 않은 상태임을 나타내는 오류입니다, 객체 생성과 데이터 저장 사이의 타이밍 문제나, 트랜잭션 처리 과정에서 발생하는 일관성 문제를 나타냅니다.


.. _ERROR-55:

**ERROR CODE: -55, 'Internal error: A page cycle reference was detected on page %1$d|%2$d of heap file %3$d|%4$d|%5$d.'**

- 이 메시지는 CUBRID 시스템이 힙 파일(heap file)에서 페이지 간의 순환 참조를 감지했을 때 발생하는 내부 오류입니다, 데이터베이스의 내부 구조 관리에 문제가 있음을 나타내며, 데이터 접근 실패, 쿼리 오류, 또는 데이터 손실로 이어질 수 있습니다.


.. _ERROR-56:

**ERROR CODE: -56, 'Internal error: unknown extendible hashing file (Volid: %1$d Fileid: %2$d) page (Volid: %3$d Pageid: %4$d) was specified.'**

- 이 메시지는 CUBRID 시스템이 확장 가능 해싱(extendible hashing) 구조에서 알 수 없거나 유효하지 않은 파일과 페이지를 참조하려고 할 때 발생하는 내부 오류입니다, 데이터베이스의 내부 구조 관리에 문제가 있음을 나타내며, 데이터 접근 실패, 쿼리 오류, 또는 데이터 손실로 이어질 수 있습니다.


.. _ERROR-57:

**ERROR CODE: -57, 'Key does not exist in the extendible hashing structure.'**

- 이 메시지는 CUBRID 시스템이 확장 가능 해싱(extendible hashing) 구조에서 특정 키를 찾으려고 시도했지만, 해당 키가 구조 내에 존재하지 않음을 나타내는 오류입니다, 주로 존재하지 않는 키로 데이터를 조회하거나, 키가 잘못되었거나, 해싱 구조 자체에 문제가 발생했을 때 발생할 수 있습니다.


.. _ERROR-59:

**ERROR CODE: -59, 'Internal error: the specified key type %1$d is not valid for the extendible hashing structure.'**

- 이 메시지는 CUBRID 시스템이 확장 가능 해싱(extendible hashing) 구조에서 사용하려는 키 타입이 해당 구조에서 지원되지 않는 유효하지 않은 타입임을 감지했을 때 발생하는 내부 오류입니다, 이는 데이터베이스의 내부 구조 관리에 문제가 있음을 나타내며, 쿼리 실행 실패나 데이터 접근 오류로 이어질 수 있습니다.


.. _ERROR-60:

**ERROR CODE: -60, 'Internal error: the extendible hashing structure has been corrupted.'**

- 이 메시지는 CUBRID 시스템이 데이터베이스의 확장 가능 해싱(extendible hashing) 구조에 손상이 발생했음을 감지했을 때 발생하는 내부 오류입니다, 이 구조의 손상은 데이터베이스의 무결성에 치명적인 영향을 미치며, 데이터 접근 실패, 쿼리 오류, 또는 데이터 손실로 이어질 가능성도 있습니다.


.. _ERROR-61:

**ERROR CODE: -61, 'Internal error: the directory root page of the extendible hashing structure (Volid: %1$d Fileid: %2$d Pageid: %3$d) has been corrupted.'**

- 이 메시지는 CUBRID 시스템에서 확장 가능 해싱(Extendible Hashing)의 최상위 디렉토리 페이지 무결성 검사에서 손상이 감지됨을 의미합니다, 인덱스/해시 디렉토리의 시작점이 깨졌으므로 해당 구조 전체 접근이 불가능하거나 결과가 신뢰될 수 없습니다.


.. _ERROR-63:

**ERROR CODE: -63, 'Internal error: a temporary page was corrupted during sorting.'**

- 이 메시지는 CUBRID 시스템이 데이터 정렬 작업을 수행하는 동안 사용되는 임시 페이지(temporary)가 손상되었음을 감지했을 때 발생하는 내부 오류입니다, CUBRID는 대량의 데이터를 정렬할 때 메모리 부족 시 디스크에 임시 파일을 생성하여 사용하는데, 이 임시 파일의 페이지가 읽기/쓰기 과정에서 손상된 것입니다, 이는 데이터베이스의 일관성과 정렬 작업의 정확성에 영향을 미칠 수 있으며, 쿼리 실패로 이어질 수 있습니다.


.. _ERROR-66:

**ERROR CODE: -66, 'Internal error: unknown force operation %1$d for object %2$d|%3$d|%4$d.'**

- 이 메시지는 CUBRID 시스템이 특정 객체에 대해 알 수 없거나 유효하지 않은 "강제 작업"을 수행하려고 시도했을 때 발생하는 내부 오류입니다, CUBRID 엔진의 내부 로직에 문제가 있거나, 데이터베이스의 내부 일관성이 심각하게 손상되었을 때 발생할 수 있습니다.


.. _ERROR-67:

**ERROR CODE: -67, 'Internal error: a heap file has not been allocated to store object %1$d|%2$d|%3$d.'**

- 이 메시지는 CUBRID 시스템이 특정 객체(OID)를 저장하기 위한 힙 파일이 할당되지 않았을 때 발생하는 내부 오류입니다, 이는 데이터베이스의 내부 구조 관리에 문제가 있음을 의미하며, 객체 저장 및 접근에 치명적인 영향을 미칠 수 있습니다.


.. _ERROR-68:

**ERROR CODE: -68, 'Internal error: different classnames for class with oid = %1$d|%2$d|%3$d were found. Found classnames are "%4$s", "%5$s" using classname hash table and heap, respectively.'**

- 이 메시지는 CUBRID 시스템이 특정 객체(OID)를 가진 테이블에 대해 두 가지 다른 테이블 이름을 발견했을 때 발생하는 내부 오류입니다, 이 불일치는 데이터베이스의 내부 메타데이터 구조가 심각하게 손상되었음을 의미하며, 데이터베이스의 일관성과 무결성에 치명적인 문제가 발생했음을 나타냅니다.


.. _ERROR-69:

**ERROR CODE: -69, 'Internal error: different class object identifiers were found for class with name "%1$s". Found OIDS are %2$d|%3$d|%4$d, %5$d|%6$d|%7$d using classname hash table and heap, respectively.'**

- 이 메시지는 CUBRID 시스템이 특정 테이블에 대해 두 가지 다른 객체 식별자(OID)를 발견했을 때 발생하는 내부 오류입니다, 이 불일치는 데이터베이스의 내부 메타데이터 구조가 심각하게 손상되었음을 의미하며, 데이터베이스의 일관성과 무결성에 치명적인 문제가 발생했음을 나타냅니다.


.. _ERROR-70:

**ERROR CODE: -70, 'Internal error: Class with name "%1$s" and oid = %2$d|%3$d|%4$d does not exist in classname hash table.'**

- 이 메시지는 CUBRID 시스템이 특정 테이블(OID)를 테이블명 해시 테이블에서 찾으려고 시도했지만 실패했을 때 발생하는 내부 오류입니다, 테이블명 해시 테이블은 CUBRID가 테이블 이름을 빠르게 조회하기 위해 사용하는 내부 데이터 구조입니다. 이 메시지는 데이터베이스의 메타데이터 관리 시스템에 불일치 또는 손상이 발생했음을 나타냅니다, 이는 데이터베이스의 내부 구조에 문제가 있음을 의미합니다.


.. _ERROR-71:

**ERROR CODE: -71, 'Internal error: Class with name "%1$s" and oid = %2$d|%3$d|%4$d does not exist in its heap.'**

- 이 메시지는 CUBRID 시스템이 특정 테이블(OID정보)를 데이터베이스의 힙(heap) 영역에서 찾으려고 시도했지만 실패했을 때 발생하는 내부 오류입니다, 힙은 실제 데이터 객체들이 저장되는 공간을 의미합니다. 이 메시지는 데이터베이스의 메타데이터(테이블 정의)와 실제 저장된 데이터 간에 불일치 또는 손상이 발생했음을 나타냅니다, 이는 데이터베이스의 무결성에 문제가 있음을 의미합니다.


.. _ERROR-80:

**ERROR CODE: -80, 'Insufficient space in operating system device when writing logical log page %1$lld (physical page %2$lld) of "%3$s". Could not write more than %4$d bytes.'**

- 이 메시지는 CUBRID 시스템이 논리적 로그 페이지를 물리적 페이지에 쓰려고 할 때, 운영체제 장치에 충분한 공간이 없어서 쓰기 작업이 실패했음을 나타내는 오류입니다, 주로 디스크 공간 부족, 파일 시스템 제한, 또는 I/O 오류로 인해 발생합니다.


.. _ERROR-81:

**ERROR CODE: -81, 'Internal error: logical log page %1$lld may be corrupted.'**

- 이 메시지는 CUBRID 시스템이 데이터베이스의 트랜잭션 로그 파일 내에 있는 특정 논리적 로그 페이지가 손상되었을 가능성을 감지했을 때 발생하는 내부 오류입니다, 데이터베이스의 안정성과 데이터 손실 위험을 나타내는 매우 중요한 경고입니다.


.. _ERROR-96:

**ERROR CODE: -96, 'Media recovery may be needed on volume "%1$s".'**

- 이 메시지는 CUBRID 시스템이 데이터베이스 볼륨의 일관성 또는 무결성 문제를 감지하여 미디어 복구(Media Recovery) 작업이 필요할 수 있음을 알리는 오류입니다, 주로 비정상적인 종료, 디스크 손상, 또는 로그 파일의 불일치 등으로 인해 데이터베이스의 특정 볼륨이 손상되었을 가능성이 있을 때 발생합니다.


.. _ERROR-97:

**ERROR CODE: -97, 'Internal error: unable to find log page %1$lld in log archives.'**

- 이 메시지는 CUBRID 시스템이 특정 로그 페이지를 로그 아카이브에서 찾으려고 시도했지만 실패했을 때 발생하는 내부 오류입니다, 이는 데이터베이스의 트랜잭션 로그를 처리하는 과정에서 필요한 로그 페이지가 아카이브 파일에 존재하지 않거나, 아카이브 파일이 손상되었거나, 아카이브 파일에 접근할 수 없을 때 발생합니다.


.. _ERROR-98:

**ERROR CODE: -98, 'Unable to create archive log "%1$s" to archive pages from %2$lld to %3$lld.'**

- 이 메시지는 CUBRID 시스템이 로그 아카이브 작업을 수행할 때, 아카이브 로그 파일을 생성하는데 실패했을 때 발생하는 오류입니다, 데이터베이스의 일관성 및 복구 가능성에 영향을 미칠 수 있는 중요한 오류입니다.


.. _ERROR-296:

**ERROR CODE: -296, 'Invalid property list encountered.'**

- 이 메시지는 CUBRID 데이터베이스에서 객체의 프로퍼티(속성) 리스트가 유효하지 않거나 손상되었을 때 나타나는 오류입니다, 객체 표현(object representation) 또는 스키마 관리 시스템에서 프로퍼티 리스트의 유효성을 검증하는 과정에서 발생합니다, CUBRID 데이터베이스 내부의 메타데이터나 객체 정의가 잘못되었거나 손상되었을 가능성을 시사합니다.


.. _ERROR-315:

**ERROR CODE: -315, 'Illegal metaclass definition encountered.'**

- 이 메시지는 CUBRID 데이터베이스에서 메타테이블 정의가 잘못되었을 때 나타나는 오류입니다, 메타테이블는 CUBRID의 시스템 카탈로그 테이블들을 정의하는 구조체를 의미합니다, 이 메시지는 메타테이블의 속성 정의가 올바르지 않을 때 발생합니다.


.. _ERROR-316:

**ERROR CODE: -316, 'Transformer size calculation mismatch, expected %1$d calculated %2$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 객체 변환 시 예상 크기와 실제 계산된 크기가 일치하지 않을 때 나타나는 오류입니다, 객체를 디스크에 저장하기 전에 크기를 계산하고, 실제 저장 후 크기를 검증합니다, 크기 불일치는 내부 로직 오류나 데이터 손상을 나타낼 수 있습니다.


.. _ERROR-317:

**ERROR CODE: -317, 'Invalid disk representation encountered for class "%1$s".'**

- 이 메시지는 CUBRID 데이터베이스에서 테이블의 디스크 표현(representation)이 유효하지 않을 때 나타나는 오류입니다, 디스크 표현은 객체가 디스크에 저장되는 방식을 정의하는 구조체로 테이블에서 특정 representation ID에 해당하는 표현을 찾을 수 없을 때 발생합니다.


.. _ERROR-318:

**ERROR CODE: -318, 'Out of sync during object loading. Database is likely corrupted or out of date.'**

- 이 메시지는 CUBRID 데이터베이스에서 객체를 로딩하는 과정에서 동기화(sync) 문제가 발생했을 때 나타나는 오류입니다, 디스크에서 읽은 데이터의 크기와 예상 크기가 다를 때 발생합니다, 일반적으로 데이터베이스 파일 손상, 버전 호환성 문제, 또는 메모리 손상으로 인해 발생할 수 있습니다.


.. _ERROR-406:

**ERROR CODE: -406, 'Invalid B+tree index identifier: (vfid = (%1$d, %2$d), rt_pgid: %3$d).'**

- 이 메시지는 CUBRID 데이터베이스에서 B-tree 인덱스 식별자(BTID)가 유효하지 않을 때 나타나는 오류입니다, B-tree 저장소 관리에서 인덱스 식별자의 유효성 검사 과정에서 주로 B-tree 인덱스 식별자의 vfid(볼륨 파일 ID)나 pageid가 잘못되었을 때 발생합니다.


.. _ERROR-407:

**ERROR CODE: -407, 'Unknown key %1$s referenced in B+tree index {vfid: (%2$d, %3$d), rt_pgid: %4$d, key_type: %5$s}.'**

- 이 메시지는 B+tree 인덱스 구조에서 존재하지 않거나 손상된 키를 참조하려 할 때 발생하는 시스템 오류입니다, 데이터베이스의 인덱스 구조 무결성에 문제가 있음을 나타내는 중요한 오류 메시지로, 인덱스의 일관성이 깨졌거나 데이터 손상이 발생했을 수 있습니다.


.. _ERROR-412:

**ERROR CODE: -412, 'Invalid range search specification.'**

- 이 메시지는 CUBRID 데이터베이스에서 B-tree 인덱스를 사용한 범위 검색(range search)을 수행할 때, 검색 범위 명세가 유효하지 않을 때 나타나는 오류입니다, B-tree 저장소 관리에서 범위 검색 유효성 검사 과정에 지원되지 않는 범위 검색 타입이나 잘못된 범위 검색 조건이 사용되었을 때입니다.


.. _ERROR-415:

**ERROR CODE: -415, 'Invalid class identifier: %1$d|%2$d|%3$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 내부적으로 사용되는 테이블 식별자(OID, Object Identifier)가 유효하지 않거나, 존재하지 않는 테이블를 참조할 때 발생합니다, 주로 시스템 카탈로그, 쿼리 실행, 객체 접근 등에서 잘못된 OID가 전달되었거나, 테이블가 삭제/변경되어 더 이상 유효하지 않은 경우, 또는 내부 데이터 손상 등으로 인해 발생할 수 있습니다.


.. _ERROR-416:

**ERROR CODE: -416, 'Unknown representation identifier: %1$d.'**

- 이 메시지는  CUBRID 데이터베이스에서 객체(테이블, 레코드 등)의 내부 표현식(Representation)을 식별하는 ID(REPRID)가 유효하지 않거나, 시스템에 등록되어 있지 않은 값을 참조할 때 발생합니다, 주로 시스템 카탈로그, 객체 접근, 쿼리 실행 등에서 잘못된 식별자가 전달되었거나, 테이블 구조 변경, 데이터 손상, 복구/이관 과정에서 식별자가 유실/불일치한 경우에 발생할 수 있습니다.


.. _ERROR-417:

**ERROR CODE: -417, 'Invalid representation identifier: %1$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 객체(테이블, 레코드 등)의 내부 표현식(Representation)을 식별하는 ID(REPRID)가 유효하지 않거나, 허용되지 않는 값이 전달될 때 발생합니다, 주로 시스템 카탈로그, 객체 접근, 쿼리 실행 등에서 잘못된 식별자가 사용되었거나, 테이블 구조 변경, 데이터 손상, 복구/이관 과정에서 식별자가 불일치하거나 손상된 경우에 발생할 수 있습니다.


.. _ERROR-421:

**ERROR CODE: -421, 'Representations Directory Missing in Catalog for Class: %1$d|%2$d|%3$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 테이블의 representation 디렉터리가 시스템 카탈로그에서 누락되었을 때 나타나는 오류입니다, 시스템 카탈로그 손상으로 필요한 정보를 찾을 수 없을 때 이 오류가 발생할 가능성이 매우 높습니다.


.. _ERROR-422:

**ERROR CODE: -422, 'Representation Information Record Missing in Catalog for Class Repr_Id: %1$d|%2$d|%3$d %4$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 테이블의 representation 정보 레코드가 시스템 카탈로그에서 누락되었을 때 나타나는 오류입니다, 시스템 카탈로그 손상으로 필요한 정보를 찾을 수 없을 때 이 오류가 발생할 가능성이 매우 높습니다.


.. _ERROR-475:

**ERROR CODE: -475, 'No query specification with index %1$s.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 인덱스 번호에 해당하는 쿼리 명세(Query Specification)를 찾을 수 없을 때 나타나는 오류입니다, 쿼리 명세는 가상 테이블나 뷰에서 사용되는 SQL 쿼리 정의를 의미하며, 각 명세는 인덱스 번호로 식별됩니다, 이 메시지는 존재하지 않는 인덱스 번호로 쿼리 명세를 조회하거나 삭제하려고 할 때 발생합니다.


.. _ERROR-540:

**ERROR CODE: -540, '%1$s.'**

- 이 메시지는 CUBRID 데이터베이스에서 긴급 오류가 발생했을 때 나타나는 일반적인 오류 메시지입니다, 주로 내부 시스템 오류나 예상치 못한 상황이 발생했을 때 사용되는 범용 오류 메시지입니다, 이 오류의 구체적인 내용은 트리거 컴파일 오류, B+Tree 구조 오류, 인덱스 구조의 손상이나 불일치 발생될 수 있습니다.


.. _ERROR-544:

**ERROR CODE: -544, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). B+tree key %6$s entry for object OID: %7$d|%8$d|%9$d was not found on B+tree: %10$d|%11$d|%12$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 B+tree 인덱스에서 특정 키 엔트리를 찾을 수 없을 때 나타나는 내부 오류입니다, 주로 인덱스 일관성 검사 과정에서 힙에 존재하는 객체의 키가 B+tree 인덱스에 존재하지 않을 때 발생합니다, 인덱스와 힙 간의 데이터 불일치를 나타내는 내부 오류입니다.


.. _ERROR-545:

**ERROR CODE: -545, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). Key and OID: %6$d|%7$d|%8$d entry on B+tree: %9$d|%10$d|%11$d is incorrect. The object does not exist.'**

- 이 메시지는 CUBRID 데이터베이스에서 B+tree 인덱스의 키와 OID 엔트리가 올바르지 않을 때 나타나는 내부 오류입니다, 주로 인덱스 스캔 과정에서 B+tree에 존재하는 OID가 실제로는 힙에 존재하지 않을 때 발생합니다, 인덱스와 힙 간의 데이터 불일치를 나타내는 내부 오류입니다.


.. _ERROR-546:

**ERROR CODE: -546, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). Expecting %6$d OID entry values, but %7$d were found on B+tree: %8$d|%9$d|%10$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 B+tree 인덱스의 OID 엔트리 개수가 예상과 다를 때 나타나는 내부 오류입니다, 주로 인덱스 일관성 검사 과정에서 힙(heap)의 OID 개수와 B+tree의 OID 개수가 일치하지 않을 때 발생합니다, 인덱스와 힙 간의 데이터 불일치를 나타내는 내부 오류입니다.


.. _ERROR-551:

**ERROR CODE: -551, 'Unable to locate volume information path file "%1$s"... Continue reading from internal tables'**

- 이 메시지는 CUBRID 데이터베이스에서 볼륨 정보 파일(DB_vinf)을 찾을 수 없을 때 나타나는 오류입니다, 주로 데이터베이스 부팅이나 로그 페이지 버퍼 스캔 과정에서 볼륨 정보 파일에 접근할 수 없을 때 발생합니다.


.. _ERROR-585:

**ERROR CODE: -585, 'Unknown heap %1$s|%2$d|%3$d'**

- 이 메시지는 CUBRID 데이터베이스에서 존재하지 않거나 알 수 없는 heap 파일에 접근하려고 할 때 발생하는 오류입니다, heap 파일은 CUBRID에서 데이터를 저장하는 기본 단위이며, 각 heap 파일은 고유한 식별자(HFID)를 가집니다.


.. _ERROR-587:

**ERROR CODE: -587, 'Entries of permanent volumes are unsorted in your "%1$s" volinfo file. Entry %2$d: %3$d %4$s is out of sequence.'**

- 이 메시지는 CUBRID 데이터베이스의 DBname_vinf 파일에서 영구 볼륨(permanent volume) 엔트리들이 올바른 순서로 정렬되어 있지 않음을 나타냅니다, DBname_vinf 파일은 데이터베이스의 볼륨 정보를 저장하는 파일경로, 파일 정보등 볼륨 ID 순서대로 정렬되어 있어야 합니다.


.. _ERROR-596:

**ERROR CODE: -596, 'The %1$d pages of total temporary space allowed have been exceeded.'**

- 이 메시지는 CUBRID 데이터베이스에서 백엔드(Backend)가 사용할 수 있도록 허용된 총 임시 공간(temporary space)의 페이지 수가 초과되었음을 나타냅니다, CUBRID는 복잡한 쿼리 처리, 정렬, 조인 등의 작업을 수행할 때 임시 데이터를 저장하기 위해 임시 공간을 사용합니다, 이 메시지는 cubrid.conf 파일에 `temp_file_max_size_in_pages`와 같은 설정 파라미터에 의해 제한된 임시 공간을 초과했을 때 발생합니다.


.. _ERROR-597:

**ERROR CODE: -597, 'Number of pages for heap file %1$d|%2$d|%3$d is inconsistent. \n%4$d and %5$d were found according to heap chain and file table, respectively.'**

- 이 메시지는 CUBRID 데이터베이스에서 checkdb 명령어 수행 시 힙 파일(heap file)에서 페이지(page) 개수의 불일치가 발견되었음을 나타내는 내부 오류입니다, 힙 파일은 실제 데이터 레코드가 저장되는 공간이며, 힙 체인(heap chain)은 힙 파일 내 페이지들의 논리적 연결을, 파일 테이블(file table)은 파일 시스템 수준에서 관리되는 페이지 정보를 의미합니다.


.. _ERROR-603:

**ERROR CODE: -603, 'Internal Error: Sector/page table of file VFID %1$d|%2$d seems corrupted.'**

- 이 메시지는 CUBRID 데이터베이스에서 파일 시스템의 내부 구조(섹터/페이지 테이블)에 손상이 감지되었을 때 발생합니다, 데이터베이스 파일의 무결성이 깨져서 정상적인 데이터 접근 및 관리가 불가능할 수 있음을 나타내는 치명적인 내부 오류입니다.


.. _ERROR-610:

**ERROR CODE: -610, 'Your database is likely to be corrupted since logging was turned off when your database crashed.'**

- 이 메시지는 CUBRID 데이터베이스에서 로그 복구 과정에서 발생합니다, 데이터베이스가 크래시된 시점에 로깅이 비활성화되어 있어서 데이터베이스가 손상되었을 가능성이 높다는 의미입니다.


.. _ERROR-614:

**ERROR CODE: -614, 'Number of active log archives has been exceeded the max desired number of %1$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 활성 로그 아카이브의 개수가 설정된 최대 허용 개수를 초과했을 때 나타납니다, 로그 아카이브 관리 시스템에서 자동으로 오래된 로그 아카이브를 삭제하는 과정에서 발생하는 알림입니다.


.. _ERROR-625:

**ERROR CODE: -625, 'Internal Error. Trying to update the wrong instance object %1$d|%2$d|%3$d attribute information template with instance object %4$d|%5$d|%6$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 내부적으로 발생하는 오류입니다, 잘못된 레코드 객체의 속성 정보 OID 불일치로 인해 발생하는 내부 오류입니다, OID(Object Identifier)는 CUBRID에서 데이터베이스 내의 각 객체를 고유하게 식별하는 8바이트 크기의 식별자입니다.


.. _ERROR-626:

**ERROR CODE: -626, 'Internal Error. %1$d requested attributes were not found.'**

- 이 메시지는 CUBRID 데이터베이스에서 내부적으로 발생하는 오류입니다, 요청된 속성(attribute) 중 일부를 찾을 수 없을 때 나타납니다, 테이블의 속성 정보를 캐시하거나 검색하는 과정에서 발생합니다.


.. _ERROR-638:

**ERROR CODE: -638, 'Warning: Flushing a non-updatable log archive pageid = %1$d'**

- 이 메시지는 CUBRID 데이터베이스에서 업데이트할 수 없는 로그 아카이브 페이지를 플러시하려고 할 때 발생하는 경고입니다, 로그 페이지가 아카이브 범위를 벗어나거나 더 이상 업데이트할 수 없는 상태에서 플러시 작업이 시도될 때 발생합니다.


.. _ERROR-644:

**ERROR CODE: -644, 'LOG FATAL ERROR: %1$s'**

- 이 메시지는 CUBRID 데이터베이스에서 로그 시스템에서 발생하는 치명적인 오류를 나타냅니다, 로그 처리 과정에서 복구할 수 없는 오류가 발생했을 때 사용됩니다, 이는 데이터베이스의 무결성과 안정성에 영향을 미칠 수 있는 오류입니다.


.. _ERROR-694:

**ERROR CODE: -694, 'SYSTEM ERROR: Unable to load B+tree.'**

- 이 메시지는 CUBRID 데이터베이스에서 B+tree 인덱스를 로드(생성)하려고 할 때 발생하는 시스템 레벨 오류입니다, 인덱스 생성 과정에서 메모리 부족, 디스크 I/O 오류, 매개변수 검증 실패, 또는 내부 알고리즘 오류 등으로 인해 발생할 수 있습니다.


.. _ERROR-698:

**ERROR CODE: -698, ' Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). the number of OIDs in the unique hierarchy: %6$d does not equal the number of OIDs: %7$d found in the unique B+tree plus the number of NULLs: %8$d found in the unique hierarchy for B+tree: %9$d|%10$d|%11$d.'**

- 이 메시지는 CUBRID 데이터베이스의 내부 일관성 오류를 나타냅니다, 특정 테이블의 인덱스에 대한 고유 제약조건(UNIQUE constraint)이 적용되는 계층구조(hierarchy) 내의 총 OID 개수가 B+tree에서 찾은 OID 개수와 고유 계층구조에서 찾은 NULL 개수의 합과 일치하지 않을 때 발생합니다, 이는 데이터베이스의 메타데이터(스키마 정보)와 실제 B+tree 인덱스 구조 간의 불일치를 의미하며, 데이터 무결성이 손상되었거나 인덱스 구조에 문제가 있을 가능성이 있습니다. 


.. _ERROR-699:

**ERROR CODE: -699, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). the number of OIDs in the unique hierarchy: %6$d does not equal the number of OIDs: %7$d found in the unique B+tree root statistics for B+tree: %8$d|%9$d|%10$d.'**

- 이 메시지는 CUBRID 데이터베이스의 내부 일관성 오류를 나타냅니다, 특정 테이블의 인덱스에 대한 고유 제약조건(UNIQUE constraint)이 적용되는 계층구조(hierarchy) 내의 OID(Object Identifier) 개수가 해당 인덱스의 B+tree 루트 통계(root statistics)에 기록된 OID 개수와 일치하지 않을 때 발생합니다, 이는 데이터베이스의 메타데이터(스키마 정보)와 실제 B+tree 인덱스 구조 간의 불일치를 의미하며, 데이터 무결성이 손상되었거나 인덱스 구조에 문제가 있을 가능성이 있습니다. 


.. _ERROR-700:

**ERROR CODE: -700, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). the number of OIDs: %6$d does not equal the number of NULLs: %7$d plus the number of keys: %8$d in the unique B+tree: %9$d|%10$d|%11$d.'**

- 이 메시지는 CUBRID 데이터베이스의 내부 일관성 오류를 나타냅니다, 특정 테이블의 인덱스에 대한 고유 B+tree에서 총 OID 개수가 NULL 값 개수와 키 개수의 합과 일치하지 않을 때 발생합니다, 이는 B+tree 인덱스의 내부 통계가 일관되지 않음을 의미하며, 데이터 무결성이 손상되었거나 인덱스 구조에 문제가 있을 가능성이 있습니다.


.. _ERROR-702:

**ERROR CODE: -702, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). the number of NULLs in the unique hierarchy: %6$d does not equal the number of NULLs: %7$d found in the unique B+tree root statistics for B+tree: %8$d|%9$d|%10$d.'**

- 이 메시지는 CUBRID 데이터베이스의 내부 일관성 오류를 나타냅니다, 특정 테이블의 인덱스에 대한 고유 제약조건(UNIQUE constraint)이 적용되는 계층구조(hierarchy) 내의 NULL 값 개수가 해당 인덱스의 B+tree 루트 통계(root statistics)에 기록된 NULL 값 개수와 일치하지 않을 때 발생합니다, 이는 데이터베이스의 메타데이터(스키마 정보)와 실제 B+tree 인덱스 구조 간의 불일치를 의미하며, 데이터 무결성이 손상되었거나 인덱스 통계가 잘못되었을 가능성이 있습니다.


.. _ERROR-703:

**ERROR CODE: -703, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). OID: %6$d|%7$d|%8$d found in unique B+tree: %9$d|%10$d|%11$d does not belong to one of the classes for the unique constraint.'**

- 이 메시지는 CUBRID 데이터베이스의 내부 일관성 오류를 나타냅니다, 특정 테이블의 인덱스에 대한 고유 B+tree에서 발견된 객체 OID가 해당 고유 제약조건이 적용되는 테이블 계층구조(UNIQUE hierarchy)에 속하지 않을 때 발생합니다, 이는 데이터베이스의 메타데이터(스키마)와 실제 데이터 저장 구조(B+tree) 간의 불일치를 의미하며, 데이터 무결성이 손상되었을 가능성이 있습니다.


.. _ERROR-714:

**ERROR CODE: -714, 'Query failed due to insufficient temporary file space.'**

- 이 메시지는 CUBRID 데이터베이스에서 쿼리 실행 중 임시 파일(temporary file) 공간이 부족하여 쿼리 처리가 실패했을 때 나타나는 오류입니다, CUBRID는 복잡한 쿼리(예: 정렬, 조인, 그룹화 등)를 처리할 때 중간 결과를 저장하기 위해 임시 파일을 사용합니다.


.. _ERROR-725:

**ERROR CODE: -725, 'Internal error: expected temporary oid and encountered permanent oid.'**

- 이 메시지는 CUBRID 데이터베이스에서 collection data type 처리 시 임시 OID를 기대했지만 영구 OID를 만났을 때 나타나는 내부 오류입니다, OID는 CUBRID에서 데이터베이스 객체를 고유하게 식별하는 식별자입니다, 이 메시지는 객체의 상태와 예상된 OID 타입이 일치하지 않을 때 발생합니다.


.. _ERROR-728:

**ERROR CODE: -728, 'Query failed because temporary file vfid is invalid for transaction %1$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 쿼리 실행 중 임시 파일(temporary file)의 VFID(Volume File ID)가 특정 트랜잭션에 대해 유효하지 않을 때 나타나는 오류입니다, CUBRID는 복잡한 쿼리나 대량의 데이터를 처리할 때 임시 파일을 사용하여 중간 결과를 저장합니다, VFID는 CUBRID에서 파일을 고유하게 식별하는 식별자로, 볼륨 ID와 파일 ID로 구성됩니다, 이 메시지는 임시 파일의 VFID가 손상되었거나, 해당 트랜잭션과 연결된 임시 파일이 예상치 못한 상태에 있을 때 발생합니다.


.. _ERROR-858:

**ERROR CODE: -858, 'Volume "%1$s" is unknown at server restart.'**

- 이 메시지는 데이터베이스 서버가 구동/종료시 불필요한 temp temp volume을 삭제히는데, 시스템 정보에 등록되지 않는 temp temp volume이 존재해서 삭제한다는 경고성 메세지입니다.



.. _ERROR-909:

**ERROR CODE: -909, 'Missing or invalid catalog class/vclass is found.'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 내부적으로 사용하는 카탈로그 테이블 또는 가상 테이블(뷰)를 찾을 수 없거나, 해당 카탈로그의 정의가 유효하지 않을 때 발생합니다, 즉, 데이터베이스의 핵심 메타데이터(테이블, 컬럼, 인덱스, 사용자 정보 등 시스템 운영에 필수적인 정보)를 관리하는 내부 객체에 문제가 발생하여 시스템이 정상적으로 동작할 수 없는 상황을 의미합니다, 이런 상황은 일반적으로 데이터베이스 손상, 비정상적인 시스템 종료, 데이터베이스 업그레이드 실패, 또는 CUBRID 내부의 메타데이터 관리 로직 오류로 인해 발생합니다, 이는 데이터베이스 시스템의 무결성과 안정성을 보호하기 위한 치명적인 보호성 오류입니다.


.. _ERROR-934:

**ERROR CODE: -934, 'Null domain referenced.'**

- 이 메시지는 CUBRID 데이터베이스에서 데이터 타입의 도메인 정보가 NULL이거나 유효하지 정보의 영향으로 일반적으로 잘못된 데이터 타입 정의로 인해 발생합니다.



.. _ERROR-983:

**ERROR CODE: -983, 'Instances of a reusable OID class are non-referable. This operation is not permitted on non-referable instances.'**

- 이 메시지는 CUBRID 데이터베이스에서 `REUSE_OID` 옵션이 설정된 테이블의 레코드를 참조하려고 할 때 발생합니다, `REUSE_OID`는 CUBRID의 특별한 기능으로, 삭제된 레코드의 OID를 재사용하여 데이터베이스의 공간 효율성을 높이는 기능입니다. 하지만 이 기능이 활성화된 테이블의 레코드는 외부에서 참조할 수 없도록 제한됩니다, 이는 데이터 무결성을 보호하고 OID 재사용으로 인한 참조 오류를 방지하기 위한 보호성 오류입니다. 특히 외래 키 제약조건, 객체 참조, 또는 다른 테이블에서의 참조가 시도될 때 발생할 수 있습니다.


.. _ERROR-1016:

**ERROR CODE: -1016, '%1$s external storage error: %2$s'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 외부 저장소(External Storage)를 사용하여 LOB(Large Object) 데이터나 외부 파일을 처리하는 과정에서 발생하는 일반적인 오류입니다, 즉, CUBRID 서버가 외부 저장소 시스템에 접근하려고 할 때, 파일 생성, 읽기, 쓰기, 삭제 등의 작업이 실패했음을 의미합니다, 이는 외부 저장소의 초기화 실패, 파일 시스템 권한 문제, 디스크 공간 부족, 네트워크 연결 문제, 또는 외부 저장소 시스템 자체의 오류로 인해 발생할 수 있습니다, 이 메시지는 CUBRID의 LOB 데이터 관리 시스템에서 외부 저장소와의 연동 과정에서 발생하는 보호성 오류입니다.


.. _ERROR-1017:

**ERROR CODE: -1017, 'Path for external storage '%1$s' is invalid.'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 외부 저장소(External Storage)의 경로를 검증하는 과정에서 해당 경로가 유효하지 않다고 판단했을 때 발생합니다, 즉, CUBRID 서버가 LOB 데이터를 저장하기 위한 외부 저장소 경로를 초기화하거나 사용하려고 할 때, 경로 형식이 올바르지 않거나, 지원되지 않는 외부 저장소 타입을 지정했거나, 경로 구성 요소가 누락되었음을 의미합니다, 이는 CUBRID의 외부 저장소 시스템에서 경로의 유효성을 검증하는 과정에서 발생하는 보호성 오류입니다.


.. _ERROR-1019:

**ERROR CODE: -1019, 'External storage is not initialized because the path is not specified in "databases.txt".'**

- 이 메시지는 CUBRID 데이터베이스의 외부 스토리지(LOB) 경로가 databases.txt 파일에 정의되지 않아 초기화에 실패했을 때 발생합니다. LOB(Large Object) 데이터를 저장하기 위한 외부 스토리지 경로 설정이 누락되었음을 나타냅니다.


.. _ERROR-1020:

**ERROR CODE: -1020, 'External file "%1$s" was not found.'**

- 이 메시지는 CUBRID가 외부 스토리지(LOB 저장소 또는 다른 외부 파일 시스템)에서 특정 파일을 찾으려고 시도했으나 해당 파일이 존재하지 않을 때 발생합니다. 파일 경로가 잘못되었거나, 파일이 실제로 삭제되었거나, 접근 권한 문제 등으로 인해 파일을 찾을 수 없는 상황을 나타냅니다.


.. _ERROR-1075:

**ERROR CODE: -1075, 'Descending index scan aborted because of lower priority on B+tree with index identifier: (vfid = (%1$d, %2$d), rt_pgid: %3$d).'**

- 이 메시지는 CUBRID 데이터베이스에서 B+트리 인덱스의 내림차순(Descending) 스캔 작업이, 시스템 내부에서 해당 작업의 우선순위가 낮다고 판단되어 중단(abort)되었을 때 발생합니다, 주로 내림차순 스캔시 원하는 btree 페이지를 얻지 못하는 경우 발생합니다.


.. _ERROR-1084:

**ERROR CODE: -1084, 'Skip invalid page in checkpoint. (page id: %1$d, "%2$s", oldest_unflush_lsa: %3$lld|%4$d, previous checkpoint redo lsa: %5$lld|%6$d)'**

- 이 메시지는 CUBRID 데이터베이스의 체크포인트 (checkpoint ) 시 내부의 시간 정보가 맞지 않아서 발생하는 오류 메세지로, 시스템 운영 및 데이터 정합성에 영향이 없는 오류입니다.



.. _ERROR-1125:

**ERROR CODE: -1125, 'Create the overflow key file. INDEX %1$s%2$s ON CLASS %3$s%4$s. key: %5$s%6$s.'**

- 이 메시지는 CUBRID 데이터베이스 시스템에서 B-tree 인덱스를 생성하거나 업데이트할 때, 인덱스 키의 크기가 페이지에 직접 저장할 수 있는 최대 크기를 초과하여 오버플로우 키 파일을 생성했을 때 발생하는 정보성 알림입니다, 즉, CUBRID 서버가 B-tree 인덱스 작업을 처리하는 과정에서, 페이지 특정 크기 보다 큰 키를 발견하면 자동으로 오버플로우 키 파일을 생성하여 해당 키를 별도의 오버플로우 페이지에 저장합니다.
이는 B-tree의 깊이를 제한하고 인덱스 성능을 유지하기 위한 정상적인 동작으로, 오류가 아닌 시스템의 자동 최적화 기능입니다.


.. _ERROR-1126:

**ERROR CODE: -1126, 'Create a new overflow page. INDEX %1$s%2$s ON CLASS %3$s%4$s. key: %5$s%6$s.'**

- 이 메시지는 CUBRID 데이터베이스 시스템에서 B-tree 인덱스를 생성하거나 업데이트할 때, 인덱스 리프 노드에 저장할 수 있는 OID(Object Identifier)의 개수가 페이지 한계를 초과하여 오버플로우 페이지를 생성했을 때 발생하는 정보성 알림입니다.
즉, CUBRID 서버가 B-tree 인덱스 작업을 처리하는 과정에서, 페이지 특정 크기에 맞는 최대(OID 개수)보다 많은 OID를 하나의 키에 대해 저장해야 할 때 자동으로 오버플로우 페이지를 생성하여 추가 OID를 별도 페이지에 저장합니다. 이는 B-tree의 깊이를 제한하고 인덱스 성능을 유지하기 위한 정상적인 동작으로, 오류가 아닌 시스템의 자동 최적화 기능입니다.


.. _ERROR-1127:

**ERROR CODE: -1127, 'Delete an empty overflow page. INDEX %1$s%2$s ON CLASS %3$s%4$s. key: %5$s%6$s.'**

- 이 메시지는 CUBRID 데이터베이스 시스템에서 B-tree 인덱스를 정리하거나 최적화할 때, 더 이상 사용되지 않는 빈 오버플로우 페이지를 삭제했을 때 발생하는 정보성 알림입니다, 즉, CUBRID 서버가 B-tree 인덱스의 유지보수 작업을 수행하는 과정에서, 특정 키에 대한 OID(Object Identifier)가 삭제되거나 변경되어 해당 오버플로우 페이지가 비어있게 되었을 때, 시스템이 자동으로 해당 페이지를 정리하고 삭제합니다. 이는 저장 공간을 효율적으로 관리하고 인덱스 구조를 최적화하기 위한 정상적인 동작으로, 오류가 아닌 시스템의 자동 정리 기능입니다.


.. _ERROR-1128:

**ERROR CODE: -1128, 'Log recovery is started.'**

- 이 메시지는 실제로는 오류가 아니라 CUBRID 데이터베이스의 로그 복구(recovery) 과정이 시작되었음을 알리는 정보성 메시지입니다.


.. _ERROR-1129:

**ERROR CODE: -1129, 'Log recovery is finished.'**

- 이 메시지는 실제로는 오류가 아니라 CUBRID 데이터베이스의 로그 복구(recovery) 과정이 성공적으로 완료되었음을 알리는 정보성 메시지입니다.


.. _ERROR-1146:

**ERROR CODE: -1146, 'volume identifier %1$d does not exist.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 볼륨 ID를 사용하여 볼륨에 접근하려고 할 때, 해당 볼륨이 시스템에 존재하지 않음을 감지했을 때 발생합니다, 즉, `show volume header` 구문 수행시 발생할 수 있는 오류로 데이터베이스가 참조하려는 물리적 저장 공간(볼륨)을 찾을 수 없을 때 발생하는 진단(diagnostic) 오류입니다, 이런 상황은 일반적으로 볼륨 파일이 삭제되었거나, 경로가 변경되었거나, 데이터베이스 구성 파일에 잘못된 볼륨 정보가 설정되었을 때 발생할 수 있습니다.


.. _ERROR-1149:

**ERROR CODE: -1149, 'Cannot find the page %1$d of volume %2$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 볼륨의 특정 페이지에 접근하려고 할 때, 해당 페이지가 존재하지 않거나 이미 해제(deallocated)된 경우 발생합니다, 즉, `show slotted page header/slots` 구문 수행시 발생할 수 있는 오류로 시스템이 요청된 페이지를 찾을 수 없을 때 발생하는 진단(diagnostic) 오류입니다, 이런 상황은 일반적으로 페이지가 삭제되었거나, 볼륨이 손상되었거나, 잘못된 페이지 참조가 있을 때 발생합니다.


.. _ERROR-1151:

**ERROR CODE: -1151, 'The page %1$d of volume %2$d is not a slotted page.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 볼륨의 특정 페이지에 접근하려고 할 때, 해당 페이지가 예상되는 '슬롯 페이지(Slotted Page)' 형식이 아님을 시스템이 감지했을 때 발생합니다, 즉, show slotted page header/slots 구문 수행시 발생할 수 있는 오류로 데이터베이스가 특정 페이지를 슬롯 페이지로 기대하고 접근했으나, 실제 페이지의 헤더 정보나 내부 구조가 슬롯 페이지의 유효성 검사를 통과하지 못했을 때 발생하는 진단(diagnostic) 오류입니다.


.. _ERROR-1181:

**ERROR CODE: -1181, 'Manual vacuum is disabled for client-server mode.'**

- 이 메시지는 CUBRID 데이터베이스가 클라이언트-서버(Client-Server) 모드로 실행 중일 때, 사용자가 수동으로 진공(vacuum) 작업을 시도할 경우 발생합니다,  CUBRID는 클라이언트-서버 환경에서 데이터베이스의 일관성과 안정성을 유지하기 위해, 수동 진공(vacuum) 작업을 제한하고 내부적으로 자동 진공(auto vacuum) 메커니즘을 통해 공간 회수 및 최적화를 관리합니다. 따라서, 이 메시지는 수동 진공 명령이 현재 운영 모드에서는 허용되지 않음을 알려주는 보호성 오류입니다.

