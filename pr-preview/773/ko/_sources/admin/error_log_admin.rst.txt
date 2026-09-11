데이터베이스 관리 오류
======================


.. _ERROR-1:

**ERROR CODE: -1, 'Missing message for error code %1$d.'**

- 이 메시지는 CUBRID 시스템이 규정할 수 없는 오류인 경우에 사용하는 오류로 발생한 원인에 대한 오류 메세지를 함께 출력되며 특정하게 규정할 수 없는 경우에 다양하게 사용하는 오류입니다.


.. _ERROR-2:

**ERROR CODE: -2, 'Internal system failure: no more specific information is available.'**

- 이 메시지는 CUBRID 시스템 내부에서 예측하지 못한 오류가 발생했거나, 특정 오류 상황에 대한 더 구체적인 오류 코드를 설정할 수 없을 때 발생하는 일반적인(Generic) 시스템 오류입니다, CUBRID는 다양한 내부 컴포넌트와 모듈로 구성되어 있으며, 각 컴포넌트는 특정 실패 상황에 대해 고유한 오류 코드를 반환하도록 설계되어 있습니다. 그러나 때로는 예외적인 상황이나 복잡한 내부 상태로 인해 특정 오류를 명확히 식별하기 어려울 때 이 오류가 사용됩니다, 이 메시지는 심각한 내부 문제의 징후일 수 있으며, 데이터베이스의 안정성과 일관성에 영향을 미칠 가능성이 매우 높습니다.


.. _ERROR-4:

**ERROR CODE: -4, 'Has been interrupted.'**

- 이 메시지는 CUBRID 시스템이 실행 중인 작업이 외부 신호나 시스템 이벤트에 의해 중단되었음을 나타내는 오류입니다, CUBRID는 다양한 작업(쿼리 실행, 데이터 로딩, 백업, 복구 등)을 수행하는 동안 중단 신호를 받을 수 있으며, 이 경우 현재 작업을 안전하게 중단하고 이 오류를 반환합니다.


.. _ERROR-83:

**ERROR CODE: -83, 'Length of path "%1$s" plus length of prefix logname "%2$s" is too long; the combined length must be less than %3$d.'**

- 이 메시지는 CUBRID 시스템이 로그 파일의 경로와 로그 이름을 합친 전체 길이가 허용된 최대 길이를 초과했을 때 발생하는 오류입니다, CUBRID는 로그 파일의 전체 경로 길이에 대한 제한하며 이 메시지는 주로 데이터베이스 경로나 로그 파일 이름이 너무 길어서 전체 경로가 시스템 제한을 초과할 때 발생합니다.


.. _ERROR-84:

**ERROR CODE: -84, 'Length of prefix logname "%1$s" is too long; the length must be less than %2$d.'**

- 이 메시지는 CUBRID 시스템이 로그 파일의 이름의 길이가 허용된 최대 길이를 초과했을 때 발생하는 오류입니다, CUBRID는 로그 파일의 이름에 대한 길이 제한하며 이 메시지는 주로 데이터베이스 이름이 너무 길거나, 로그 파일 경로가 시스템 제한을 초과할 때 발생합니다.


.. _ERROR-85:

**ERROR CODE: -85, 'The prefix name "%1$s" is not the same as "%2$s" on the log disk. The log may have been renamed outside the database domain.'**

- 이 메시지는 CUBRID 시스템이 로그 디스크에서 발견한 로그 파일의 이름이 데이터베이스가 예상하는 이름과 일치하지 않을 때 발생하는 오류입니다, 이는 데이터베이스의 트랜잭션 로그 파일이 데이터베이스 시스템 외부에서 수동으로 이름이 변경되었거나, 로그 파일이 잘못된 데이터베이스에 연결되었을 때 발생할 수 있습니다.


.. _ERROR-86:

**ERROR CODE: -86, 'Database is incompatible with current "%1$s" release "%2$s".'**

- 이 메시지는 현재 사용 중인 CUBRID 릴리스(엔진) 버전이 데이터베이스와 호환되지 않음을 나타냅니다, 데이터베이스가 다른 버전의 CUBRID로 생성되었거나, 현재 CUBRID 버전이 데이터베이스의 형식과 호환되지 않는 경우 발생합니다, 이는 주로 CUBRID 버전 업그레이드/다운그레이드 과정에서 발생하는 호환성 문제입니다.


.. _ERROR-87:

**ERROR CODE: -87, 'There are recovery actions that must recovered using "%1$s" release "%2$s" instead of release "%3$s". After the recovery, the database can be run on release "%4$s".'**

- 이 메시지는 데이터베이스 복구 작업이 현재 사용 중인 CUBRID 릴리스와 호환되지 않는 로그 레코드를 포함하고 있음을 나타냅니다, CUBRID 버전 호환성 문제로 주로 데이터베이스 업그레이드/다운그레이드 과정이나, 다른 버전의 CUBRID로 생성된 로그 파일을 복구하려 할 때 발생합니다.


.. _ERROR-88:

**ERROR CODE: -88, 'Internal error: Release string "%1$s" cannot exceed %2$d. Must change header log.'**

- 이 메시지는 CUBRID 시스템이 로그 헤더에 저장된 "릴리스 문자열"의 길이가 허용된 최대 길이를 초과했음을 감지했을 때 발생하는 내부 오류입니다, 즉, 로그 헤더 저장시 릴리스 문자열 길이가 허용된 길이를 초과 했을떄 발생합니다.


.. _ERROR-89:

**ERROR CODE: -89, 'Log "%1$s" does not belong to the given database.'**

- 이 메시지는 CUBRID 시스템이 특정 로그 파일이 현재 작업 중인 데이터베이스에 속하지 않는다고 판단할 때 발생하는 오류입니다, 로그 파일과 데이터베이스 간의 일치성 검증 과정에서 발생하는 오류로, 이는 주로 로그 파일이 잘못된 데이터베이스와 연결되었거나, 로그 파일이 손상되었거나, 데이터베이스 설정이 잘못되었을 때 발생합니다.


.. _ERROR-99:

**ERROR CODE: -99, 'Unable to create backup directory information file "%1$s".'**

- 이 메시지는 CUBRID 시스템이 데이터베이스 백업 작업을 수행할 때, 백업 디렉터리 정보를 저장하는 백업 정보 파일(DBname_bkvinf)을 생성하려고 할 때 실패할 경우 발생하는 오류입니다.


.. _ERROR-100:

**ERROR CODE: -100, 'Unable to backup database "%1$s".'**

- 이 메시지는 CUBRID 시스템이 데이터베이스 백업 작업을 수행하려고 할 때, 백업 과정에서 오류가 발생하여 백업이 실패했을 때 발생하는 오류입니다, 백업 실패는 파일 시스템 권한 문제, 디스크 공간 부족, 파일 접근 오류, 또는 시스템 리소스 부족 등 다양한 원인으로 발생할 수 있습니다.


.. _ERROR-115:

**ERROR CODE: -115, 'Database "%1$s" already exists.'**

- 이 메시지는 CUBRID 시스템이 새로운 데이터베이스를 생성하려고 할 때, 동일한 이름의 데이터베이스가 이미 존재할 때 발생하는 오류입니다, CUBRID 데이터베이스 이름에 대/소문자를 구분하며 이 메시지는 이미 존재하는 데이터베이스와 같은 이름으로 새 데이터베이스를 생성할 수 없음을 의미합니다.


.. _ERROR-116:

**ERROR CODE: -116, 'Database "%1$s" is unknown, or the file "databases.txt" cannot be accessed.'**

- 이 메시지는 CUBRID 시스템이 지정된 데이터베이스를 찾을 수 없거나, 데이터베이스 목록을 관리하는 databases.txt 파일에 접근할 수 없을 때 발생하는 오류입니다, 주로 데이터베이스 이름이 잘못되었거나, databases.txt 파일이 손상되었거나, 파일 시스템 권한 문제로 인해 접근이 불가능할 때 발생하는 오류입니다.


.. _ERROR-117:

**ERROR CODE: -117, 'Absolute pathname for the database is too long. The combined length of the path "%1$s" plus the name "%2$s" is %3$d; the combined length must be less than %4$d.'**

- 이 메시지는 CUBRID 시스템이 데이터베이스의 전체 경로명(절대 경로 + 데이터베이스 이름)을 처리하려고 할 때, 그 길이가 시스템에서 허용하는 최대 길이를 초과했을 때 발생하는 오류입니다, 파일 시스템이나 CUBRID 내부적인 제한으로 인해 너무 긴 경로명은 처리할 수 없음을 의미합니다.


.. _ERROR-119:

**ERROR CODE: -119, 'Cannot find hostname ("%1$s") in "%2$s". Please check permissions of the file or if there is the hostname.'**

- 이 메시지는 CUBRID 시스템이 특정 호스트 이름을 찾으려고 시도했지만 실패했을 때 발생하는 오류입니다, 주로 네트워크 통신을 설정하거나 호스트 정보를 조회하는 과정에서 발생하며, 지정된 호스트 이름이 유효하지 않거나, 호스트 정보를 담고 있는 파일(예: /etc/hosts)에 접근할 수 없을 때 나타납니다.


.. _ERROR-120:

**ERROR CODE: -120, 'The maximum number of volumes (%1$d) has been exceeded.'**

- 이 메시지는 CUBRID 시스템이 새로운 볼륨을 추가하려고 할 때, 시스템에서 허용하는 최대 볼륨 개수를 초과했을 때 발생하는 오류입니다., CUBRID는 시스템 안정성과 성능을 위해 데이터베이스당 최대 볼륨 개수를 제한하고 있습니다. 이 제한을 초과하면 볼륨 추가가 실패합니다.


.. _ERROR-121:

**ERROR CODE: -121, 'Trying to remove "%1$s" a permanent volume from the database.'**

- 이 메시지는 CUBRID 시스템이 데이터베이스에서 임시볼륨(temp temp volume)을 제거하려고 시도할 때 발생하는 오류입니다, 임시볼륨은 데이터베이스에서 쿼리를 정려하는 공간으로, 데이터베이스 구동(운영) 중 삭제가 허용되지 않습니다. 이 오류는 영구 볼륨을 제거하려는 시도가 감지되었음을 사용자에게 안내하는 이벤트 메시지입니다.


.. _ERROR-122:

**ERROR CODE: -122, 'Unable to access system message catalog.'**

- 이 메시지는 CUBRID 시스템이 메시지 카탈로그 파일들을 초기화하려고 할 때, 메시지 카탈로그 파일에 접근할 수 없을 때 발생하는 오류입니다, 메시지 카탈로그는 CUBRID의 모든 에러 메시지와 로케일별 메시지를 포함하는 파일들로, 시스템 초기화 시 필수적으로 로드되어야 합니다.


.. _ERROR-124:

**ERROR CODE: -124, 'Volume "%1$s" already exists.'**

- 이 메시지는 CUBRID 시스템이 새로운 볼륨을 생성하려고 할 때, 동일한 이름의 볼륨이 이미 존재할 때 발생하는 오류입니다, 볼륨 확장이나 새 볼륨 생성 시 중복된 볼륨 이름으로 인해 생성이 실패했음을 의미합니다.


.. _ERROR-126:

**ERROR CODE: -126, 'Unknown purpose "%1$s" given on line %2$d.'**

- 이 메시지는 CUBRID 시스템이 데이터베이스 볼륨 추가 설정 파일을 파싱하는 과정에서 특정 라인에 지정된 '볼륨 용도(PURPOSE)' 값이 유효하지 않거나 인식할 수 없을 때 발생하는 오류입니다, 주로 `createdb 명령어에서 --more-volume-file 옵션 사용 시 발생하며 이는 볼륨 확장 설정 파일에서 볼륨의 용도를 지정하는 PURPOSE 파라미터에 허용되지 않는 문자열이 입력되었음을 의미합니다.


.. _ERROR-127:

**ERROR CODE: -127, 'Incorrect value %1$d for number of pages given on line %2$d.'**

- 이 메시지는 CUBRID 시스템이 데이터베이스 볼륨 추가 설정 파일을 파싱하는 과정에서 특정 라인에 지정된 '페이지 수' 값이 유효하지 않을 때 발생하는 오류입니다, 주로 `createdb 명령어에서 --more-volume-file 옵션 사용 시 발생하며 이는 볼륨 확장 설정 파일에서 볼륨의 크기를 지정하는 페이지 수가 허용되지 않는 값으로 지정되었음을 의미합니다.


.. _ERROR-128:

**ERROR CODE: -128, 'Number of pages was not given on line %1$d.'**

- 이 메시지는 CUBRID 시스템이 데이터베이스 볼륨 추가 설정 파일을 파싱하는 과정에서 특정 라인에 필요한 '페이지 수' 정보가 누락되었거나 유효하지 않을 때 발생하는 오류입니다, 주로 `createdb 명령어에서 --more-volume-file 옵션 사용 시 발생하며 이는 볼륨 확장 설정 파일에서 볼륨의 크기를 지정하는 페이지 수가 정의되지 않았거나 올바르지 않은 형식으로 지정되었음을 의미합니다.


.. _ERROR-129:

**ERROR CODE: -129, 'Unknown token "%1$s" was found on line %2$d.'**

- 이 메시지는 CUBRID 시스템이 데이터베이스 볼륨 추가 설정 파일을 파싱하는 과정에서 예상치 못한, 정의되지 않은 토큰(키워드)을 발견했을 때 발생하는 오류입니다, 볼륨 확장 설정 파일에서 지원되지 않는 키워드나 잘못된 구문이 사용되었음을 의미합니다.


.. _ERROR-131:

**ERROR CODE: -131, 'Database system error.'**

- 이 메시지는 CUBRID 데이터베이스 시스템 내부에서 예측하지 못한 오류가 발생했음을 나타냅니다, 일반적으로 데이터베이스 엔진의 핵심 구성 요소에서 문제가 발생했을 때 출력되며 주로 관련 또는 질의 수행 요청시 관련 질의 또는 결과 정보가 없는 경우 발생합니다.


.. _ERROR-137:

**ERROR CODE: -137, 'Corrupted authorization objects.'**

- 이 메시지는 CUBRID 데이터베이스에서 권한 정보가 맞지 않거나 컬럼이 없는 경우 발생합니다.


.. _ERROR-138:

**ERROR CODE: -138, 'Authorization class "%1$s" not found.'**

- 이 메시지는 CUBRID 데이터베이스에서 권한 문제로 테이블를 찾을 수 없는 경우 발생합니다.


.. _ERROR-139:

**ERROR CODE: -139, 'Access error on authorization attribute "%2$s" of class "%1$s".'**

- 이 메시지는 CUBRID 데이터베이스에서 권한 문제로 관련 컬럼을 접근할 수 없는 경우 발생합니다.


.. _ERROR-140:

**ERROR CODE: -140, 'Operation "%1$s" can only be performed by the DBA or a DBA group member.'**

- 이 메시지는 CUBRID 시스템에서 특정 작업을 수행하려고 할 때, 해당 작업이 DBA 또는 DBA 그룹의 멤버만 수행할 수 있는 권한이 필요한 작업일 경우 발생하는 오류입니다, 일반 사용자가 시스템 관리자 권한이 필요한 작업을 시도했을 때 발생하며, 보안상의 이유로 접근이 거부되었음을 의미합니다.


.. _ERROR-141:

**ERROR CODE: -141, 'Cannot add "%1$s" as a member of "%2$s".'**

- 이 메시지는 CUBRID 데이터베이스에서 user 생성시 public의 member로 등록하는 과정에서 발생하는 오류로 경고성 메시지입니다.


.. _ERROR-142:

**ERROR CODE: -142, 'Adding member causes the user hierarchy to become a cyclic graph.'**

- 이 메시지는 CUBRID 시스템에서 사용자 또는 그룹에 멤버를 추가하는 작업이 사용자 계층 구조에 순환 참조(cyclic dependency)를 발생시킬 때 발생하는 오류입니다, 예를 들어, 그룹 A에 그룹 B를 멤버로 추가하고, 동시에 그룹 B에 그룹 A를 멤버로 추가하려고 할 때 발생할 수 있습니다. 이는 무한 루프나 논리적 모순을 야기할 수 있으므로 시스템에서 허용되지 않습니다.


.. _ERROR-143:

**ERROR CODE: -143, 'Encountered a class with no owner.'**

- 이 메시지는 CUBRID 데이터베이스에서 권한을 수정하려는 객체의 owner가 없는 경우 발생하는 오류입니다.


.. _ERROR-146:

**ERROR CODE: -146, 'Cannot issue GRANT/REVOKE to owner of a %1$s.'**

- 이 메시지는 CUBRID 데이터베이스에서 owner의 object를 grantt 수행 시도시 발생하는 오류입니다.


.. _ERROR-147:

**ERROR CODE: -147, 'No GRANT option.'**

- 이 메시지는 CUBRID 데이터베이스에서 grant 수행시 grant option을 주지 않아서 발생하는 오류입니다.


.. _ERROR-148:

**ERROR CODE: -148, 'Cannot obtain write lock on authorization object.'**

- 이 메시지는 CUBRID 데이터베이스에서 grant 또는 revoke 처리시 write lock을 얻지 못해서 update를 할 수 없는 경우 발생하는 오류입니다.


.. _ERROR-150:

**ERROR CODE: -150, 'Cannot revoke privileges from self.'**

- 이 메시지는 CUBRID 데이터베이스에서 자신의 object를 revoke 수행 시도시 발생하는 오류입니다.


.. _ERROR-151:

**ERROR CODE: -151, 'Cannot revoke privileges from owner of a %1$s.'**

- 이 메시지는 CUBRID 데이터베이스에서 owner의 object를 revoke 수행 시도시 발생하는 오류입니다.


.. _ERROR-152:

**ERROR CODE: -152, 'GRANT not found.'**

- 이 메시지는 CUBRID 데이터베이스에서 revoke 실행시 관련 grant 정보를 찾지 못할 경우 발생하는 오류입니다.


.. _ERROR-153:

**ERROR CODE: -153, 'No authorization privileges in effect for the database.'**

- 이 메시지는 CUBRID 데이터베이스에서 해당 DB에 권한관련 catalog class가 존재하지 않을 경우 발생하는 오류입니다.


.. _ERROR-154:

**ERROR CODE: -154, 'Incomplete authorization installation no authorization privileges in effect for the database.'**

- 이 메시지는 CUBRID 데이터베이스에서 해당 DB에 dba, public user가 존재하지 않은 경우 발생하는 오류입니다.


.. _ERROR-156:

**ERROR CODE: -156, 'Authorization failure.'**

- 이 메시지는 CUBRID 데이터베이스에서 알수 없는 권한이 요청되었을때 발생하는 오류입니다.


.. _ERROR-157:

**ERROR CODE: -157, 'SELECT authorization failure.'**

- 이 메시지는 CUBRID 데이터베이스에서 select 권한이 해당 테이블에 없는 경우 발생하는 오류입니다.


.. _ERROR-158:

**ERROR CODE: -158, 'ALTER authorization failure.'**

- 이 메시지는 CUBRID 데이터베이스에서 alter 권한이 해당 테이블에 없는 경우 발생하는 오류입니다.


.. _ERROR-159:

**ERROR CODE: -159, 'UPDATE authorization failure.'**

- 이 메시지는 CUBRID 데이터베이스에서 update 권한이 해당 테이블에 없는 경우 발생하는 오류입니다.


.. _ERROR-160:

**ERROR CODE: -160, 'INSERT authorization failure.'**

- 이 메시지는 CUBRID 데이터베이스에서 insert 권한이 해당 테이블에 없는 경우 발생하는 오류입니다.


.. _ERROR-161:

**ERROR CODE: -161, 'DELETE authorization failure.'**

- 이 메시지는 CUBRID 데이터베이스에서 delete 권한이 해당 테이블에 없는 경우 발생하는 오류입니다.


.. _ERROR-162:

**ERROR CODE: -162, 'INDEX authorization failure.'**

- 이 메시지는 CUBRID 데이터베이스에서 index 처리 권한이 해당 테이블에 없는 경우 발생하는 오류입니다.


.. _ERROR-163:

**ERROR CODE: -163, 'EXECUTE authorization failure.'**

- 이 메시지는 CUBRID 데이터베이스에서 execute 권한이 해당 테이블에 없는 경우 발생하는 오류입니다.


.. _ERROR-164:

**ERROR CODE: -164, 'User "%1$s" already exists.'**

- 이 메시지는 CUBRID 시스템에서 새로운 사용자를 생성하려고 할 때, 동일한 이름의 사용자가 이미 시스템에 존재하는 경우 발생하는 오류입니다, CREATE USER 문을 실행할 때 중복된 사용자 이름으로 인해 사용자 생성이 실패했음을 의미합니다.


.. _ERROR-165:

**ERROR CODE: -165, 'User "%1$s" is invalid.'**

- 이 메시지는 CUBRID 시스템에서 특정 작업을 수행하려고 할 때, 지정된 사용자 이름이 유효하지 않거나 시스템에 존재하지 않는 사용자일 경우 발생하는 인증 관련 오류입니다, 데이터베이스에 접속하거나, 사용자 권한을 변경하거나, 특정 객체의 소유자를 지정하는 등의 작업에서 잘못된 사용자 이름이 사용되었음을 의미합니다.


.. _ERROR-166:

**ERROR CODE: -166, 'Invalid user specified.'**

- 이 메시지는 CUBRID 시스템에서 사용자 권한(GRANT) 작업을 수행할 때, 지정된 사용자가 유효하지 않거나 존재하지 않을 때 발생하는 오류입니다, 주로 사용자 이름 오류나 존재하지 않는 사용자를 참조할 때 발생합니다.


.. _ERROR-167:

**ERROR CODE: -167, 'DBA, members of DBA group and %1$s owner can perform the operation.'**

- 이 메시지는 CUBRID 시스템에서 특정 작업을 수행하려고 할 때, 해당 작업을 수행할 권한이 없는 사용자에게 발생하는 인증 관련 오류입니다, DBA나 DBA 그룹의 멤버, 또는 특정 객체의 소유자만이 해당 작업을 수행할 수 있음을 알려줍니다.


.. _ERROR-168:

**ERROR CODE: -168, '168 Member not found.'**

- 이 메시지는 CUBRID 데이터베이스에서 drop member 시 member가 아닌 user를 삭제 요청할때 발생하는 오류입니다.


.. _ERROR-169:

**ERROR CODE: -169, '169 Cannot remove user %1$s from the database.'**

- 이 메시지는 CUBRID 데이터베이스에서 drop user시 현재 login된 사용자를 삭제 요청할때 발생하는 오류입니다.


.. _ERROR-171:

**ERROR CODE: -171, 'Incorrect or missing password.'**

- 이 메시지는 CUBRID 시스템에서 사용자 인증을 시도할 때 제공된 비밀번호가 올바르지 않거나, 비밀번호가 아예 제공되지 않았을 때 발생하는 오류입니다, 데이터베이스에 접속하거나 큐브리드 명령어 작업을 수행하기 위해 필요한 인증 과정에서 비밀번호 불일치 또는 누락이 감지되었음을 의미합니다.


.. _ERROR-172:

**ERROR CODE: -172, '172 Password string cannot have more than 31 bytes.'**

- 이 메시지는 CUBRID 데이터베이스 db user의 암호 길이가 31자를 넘을 경우 발생하는 오류입니다.


.. _ERROR-173:

**ERROR CODE: -173, 'Could not locate database file "%1$s".'**

- 이 메시지는 CUBRID 시스템이 특정 databases.txt 파일을 찾을 수 없을 때 발생하는 오류입니다.


.. _ERROR-174:

**ERROR CODE: -174, 'Could not obtain write access to database file "%1$s".'**

- 이 메시지는 CUBRID 시스템이 특정 databases.txt 파일에 대한 쓰기 권한을 얻지 못했을 때 발생하는 오류입니다.


.. _ERROR-183:

**ERROR CODE: -183, 'Unexpected amount of received data; %1$d expected, %2$d received.'**

- 이 메시지는 CUBRID 클라이언트가 서버로부터 데이터를 수신할 때, 예상했던 데이터의 크기와 실제로 수신된 데이터의 크기가 일치하지 않을 때 발생하는 오류입니다, 네트워크 통신 중 데이터 무결성 문제, 프로토콜 불일치, 또는 데이터 전송 오류로 인해 발생할 수 있습니다, 클라이언트가 서버로부터 받은 데이터의 양이 예상과 다른 잘못된 통신을 의미할 수 있습니다.


.. _ERROR-184:

**ERROR CODE: -184, 'Cannot allocate communications buffer.'**

- 이 메시지는 CUBRID 시스템이 네트워크 통신을 위한 버퍼를 할당할 수 없을 때 발생하는 오류입니다, 메모리 부족, 시스템 리소스 한계, 또는 버퍼 할당 실패로 인해 발생합니다, 클라이언트와 서버 간의 데이터 전송을 위한 통신 버퍼를 생성하지 못했을 때 발생합니다.


.. _ERROR-185:

**ERROR CODE: -185, 'Error receiving data from client.'**

- 이 메시지는 CUBRID 서버가 클라이언트로부터 데이터를 수신하는 과정에서 오류가 발생했을 때 나타나는 오류입니다, 네트워크 통신 중 클라이언트 데이터 수신 실패, 프로토콜 불일치, 클라이언트 응답 오류 등으로 인해 발생합니다, 서버가 클라이언트로부터 예상한 데이터를 받지 못했거나, 받은 데이터가 올바르지 않을 때 발생합니다.


.. _ERROR-186:

**ERROR CODE: -186, 'Error receiving data from server.'**

- 이 메시지는 CUBRID 클라이언트가 서버로부터 데이터를 수신하는 과정에서 오류가 발생했을 때 나타나는 오류입니다, 네트워크 통신 중 데이터 수신 실패, 프로토콜 불일치, 서버 응답 오류 등으로 인해 발생합니다, 클라이언트가 서버로부터 예상한 데이터를 받지 못했거나, 받은 데이터가 올바르지 않을 때 발생합니다.


.. _ERROR-187:

**ERROR CODE: -187, 'Communications buffer not used.'**

- 이 메시지는 CUBRID 클라이언트가 서버로부터 데이터를 수신할 때, 예상했던 버퍼와 실제로 받은 버퍼가 다를 때 발생하는 오류입니다, 네트워크 통신 과정에서 버퍼 관리에 문제가 있거나 예상과 다른 데이터가 수신되었을 때 발생합니다.


.. _ERROR-188:

**ERROR CODE: -188, 'Unknown database "%1$s".'**

- 이 메시지는 CUBRID 시스템이 지정된 데이터베이스 이름이 NULL 또는 최대 길이를 초과한 경우 발생합니다.


.. _ERROR-189:

**ERROR CODE: -189, 'Invalid host name "%1$s".'**

- 이 메시지는 CUBRID 클라이언트가 서버에 연결할 때 제공된 호스트 이름이 유효하지 않거나 잘못된 형식일 때 발생하는 오류입니다, 호스트 이름이 NULL이거나, 최대 허용 길이를 초과하거나, 올바른 형식이 아닐 때 발생합니다, 클라이언트 초기화 과정에서 서버 호스트 정보의 유효성을 검증할 때 발생합니다.


.. _ERROR-190:

**ERROR CODE: -190, 'Server host not identified.'**

- 이 메시지는 CUBRID 클라이언트가 서버 호스트 정보를 찾을 수 없을 때 발생하는 오류입니다, 클라이언트 초기화 과정에서 서버 호스트명이나 서버 이름이 설정되지 않았거나 비어있는 상태에서 발생합니다, 클라이언트가 어떤 서버에 연결해야 하는지 알 수 없는 상태를 나타냅니다.


.. _ERROR-191:

**ERROR CODE: -191, 'Cannot connect to server "%1$s" on "%2$s".'**

- 이 메시지는 CUBRID 클라이언트가 지정된 호스트의 데이터베이스 서버 프로세스(cub_server)에 연결할 수 없을 때 발생하는 오류입니다, 네트워크 연결 실패, 서버가 실행되지 않음, 방화벽 차단, 포트 문제 등으로 인해 발생할 수 있습니다, 클라이언트가 서버와의 통신을 설정할 수 없는 상태를 나타냅니다.


.. _ERROR-193:

**ERROR CODE: -193, 'Server received shutdown command from client.'**

- 이 메시지는 CUBRID 데이터베이스가 클라이언트로부터 종료(shutdown) 명령을 받았을 때 발생하는 오류입니다. 데이터베이스 서버가 클라이언트의 요청에 따라 정상적으로 종료 프로세스를 시작했음을 알립니다, 이는 오류가 아니라 서버의 정상적인 종료를 알리는 이벤트 메시지입니다.


.. _ERROR-194:

**ERROR CODE: -194, 'Unknown server request id %1$d.'**

- 이 메시지는 CUBRID 서버가 클라이언트로부터 받은 요청 ID (서비스 ID) 가 유효하지 않거나 정의되지 않은 요청일 때 발생하는 오류입니다, 서버가 처리할 수 없는 요청 ID를 받았을 때 발생하며, 이는 주로 클라이언트와 서버 간의 프로토콜 버전 불일치나 잘못된 요청 형식으로 인해 발생합니다.


.. _ERROR-195:

**ERROR CODE: -195, 'Server communications error: %1$s.'**

- 이 메시지는 CUBRID 데이터베이스에서 내/외부 클라이언트와 서버 간의 통신 과정에서 발생하는 오류를 나타냅니다, 서버와의 통신이 실패하거나 연결이 끊어졌을 때, 또는 서버 통신 프로토콜에 문제가 있을 때 발생합니다, 주로 네트워크 연결 문제, 서버 상태 문제, 또는 통신 프로토콜 오류와 관련이 있습니다.


.. _ERROR-196:

**ERROR CODE: -196, 'Server name not identified.'**

- 이 메시지는 CUBRID 데이터베이스 서버에 연결을 시도할 때, 서버 이름이 설정되지 않았거나 유효하지 않을 때 발생하는 오류입니다, 주로 클라이언트가 연결하려는 서버의 이름이나 호스트 정보가 올바르게 설정되지 않은 상태를 나타냅니다.


.. _ERROR-197:

**ERROR CODE: -197, 'Could not make contact with master server.'**

- 이 메시지는 CUBRID 데이터베이스가 마스터(cub_master) 프로세스로 연결을 시도했지만 연결에 실패했음을 나타냅니다, 마스터(cub_master)는 CUBRID 시스템에서 여러 데이터베이스를 관리 역할을 하는 핵심 프로세스로 데이터베이스 작업을 수행하기 위해서는 마스터 서버와의 연결이 필요합니다, 주로 마스터 프로세스가 비정상적인 동작 상황이나 환경(방화벽등)에 있을 때 발생합니다.


.. _ERROR-199:

**ERROR CODE: -199, 'Server no longer responding.'**

- 이 메시지는 CUBRID 데이터베이스 서버가 클라이언트의 요청에 응답하지 않거나, 서버 프로세스가 예기치 않게 종료되었을 때 발생하는 오류입니다, 서버가 더 이상 클라이언트와 통신할 수 없는 상태를 나타내며, 이는 DB와 접속 문제, 연결 끊김, 또는 서버 프로세스 중단 등의 원인으로 발생할 수 있습니다.


.. _ERROR-204:

**ERROR CODE: -204, 'Function called with missing or invalid arguments.'**

- 이 메시지는 CUBRID 데이터베이스 내부에서 함수를 호출할 때, 필수 인자가 누락되었거나 전달된 인자의 값이 유효하지 않을 때 발생하는 오류입니다, 일반적으로 사용자 직접적인 SQL 오류라기보다는 CUBRID 엔진 내부의 문제나 개발자가 CUBRID API를 잘못 사용했을 때 발생할 가능성이 높습니다.


.. _ERROR-210:

**ERROR CODE: -210, 'Internal error: processing object template.'**

- 이 메시지는 CUBRID 데이터베이스에서 객체 템플릿 처리 중 내부 오류가 발생했을 때 나타나는 오류입니다, 주로 테이블 캐시 불일치, 락 업그레이드 실패, 또는 템플릿 상태 검증 오류로 인해 발생할 수 있습니다.


.. _ERROR-217:

**ERROR CODE: -217, 'Operation can only be performed on class objects.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 작업이 테이블 객체에 대해서만 유효할 때, 해당 작업이 테이블 객체가 아닌 다른 유형의 객체에 대해 시도되었음을 나타냅니다, 주로 스키마 관리, 메타데이터 조회, 또는 객체 템플릿 처리와 같은 테이블 정의와 관련된 작업에서 발생될 수 있습니다.


.. _ERROR-224:

**ERROR CODE: -224, 'A database has not been restarted.'**

- 이 메시지는 CUBRID 데이터베이스에서 데이터베이스 연결이 설정되지 않았거나, 데이터베이스가 재시작되지 않은 상태에서 데이터베이스 작업을 시도할 때 발생하는 오류입니다, 주로 데이터베이스 서버와의 연결이 끊어졌거나, 데이터베이스가 정상적으로 시작되지 않은 상태에서 SQL 문을 실행하거나 데이터베이스 작업을 수행하려 할 때 발생합니다.


.. _ERROR-231:

**ERROR CODE: -231, 'Invalid API operation attempted on a temporary object.'**

- 이 메시지는 CUBRID 데이터베이스에서 임시 객체(temporary object)에 대해 허용되지 않는 API 작업을 시도할 때 발생하는 오류입니다, CUBRID에서 임시 객체는 주로 스키마 템플릿이나 트리거와 관련된 임시적인 객체로, 일반적인 데이터베이스 객체와는 다른 생명주기와 제약사항을 가집니다, 주로 임시 객체에 대해 일반 객체에서만 허용되는 작업(예: 데이터 접근, 수정, 삭제 등)을 시도할 때 발생합니다.


.. _ERROR-232:

**ERROR CODE: -232, 'Not allowed to use object templates over multiple transactions.'**

- 이 메시지는 CUBRID 데이터베이스에서 객체 템플릿(object template)이 여러 트랜잭션에 걸쳐 사용되려고 할 때 발생하는 오류입니다, 객체 템플릿은 일반적으로 단일 트랜잭션 내에서 생성 및 사용되어야 하는 임시적인 스키마 정의 객체입니다. 여러 트랜잭션에 걸쳐 사용될 경우, 데이터 일관성 문제나 예상치 못한 동작을 유발할 수 있어 시스템에서 이를 금지합니다, 주로 객체 템플릿의 생명주기 관리와 관련된 로직에서 발생하며, 템플릿이 생성된 트랜잭션 범위를 벗어나 사용될 때 나타납니다.


.. _ERROR-247:

**ERROR CODE: -247, 'Invalid arguments to schema manager internal function.'**

- 이 메시지는 CUBRID 데이터베이스의 스키마 관리자 내부 함수에 잘못된 인자(NULL, 빈 문자열, 또는 유효하지 않은 값)가 전달되었을 때 발생하는 내부 오류입니다, 주로 스키마 생성, 수정, 조회 등의 작업 중에 발생하며, 개발자나 시스템 레벨에서의 문제를 나타냅니다.


.. _ERROR-263:

**ERROR CODE: -263, 'Schema manager internal corruption detected.'**

- 이 메시지는 CUBRID 데이터베이스의 스키마 관리자(Schema Manager) 내부에서 데이터 손상이 감지되었음을 나타내는 시스템 오류입니다, 주로 객체 표현(object representation), 테이블 정의, 또는 메타데이터 처리 과정에서 내부 일관성 검사가 실패했을 때 발생합니다, 이는 데이터베이스 파일 손상, 메모리 오류, 또는 시스템 비정상 종료 등으로 인해 발생할 수 있습니다.


.. _ERROR-314:

**ERROR CODE: -314, 'Object buffer overflow while writing.'**

- 이 메시지는 CUBRID 데이터베이스에서 객체(테이블 레코드 등)의 데이터를 저장(쓰기)할 때, 내부적으로 할당된 버퍼의 크기를 초과하는 데이터가 기록되려고 할 때 발생합니다, 주로 대용량 데이터 처리, 비정상적으로 큰 객체, 버퍼 크기 설정 오류, 내부 버그 등으로 인해 나타날 수 있으며, 데이터 손실, 트랜잭션 실패, 시스템 오류로 이어질 수 있습니다.


.. _ERROR-321:

**ERROR CODE: -321, 'Possible corruption in the workspace detected.'**

- 이 메시지는 CUBRID 데이터베이스에서 workspace에서 처리시 원하지 않는 정보가 존재할 경우 발생하는 오류입니다, workspace 관리 시스템에서 데이터 무결성 검증 과정에서 발생합니다, 일반적으로 workspace는 메모리 내의 객체 관리 구조가 일관성을 유지해야 하는데, 이러한 일관성이 깨졌을 때 이 오류가 보고됩니다.


.. _ERROR-322:

**ERROR CODE: -322, 'MOP not found in the workspace.'**

- 이 메시지는 CUBRID 데이터베이스에서 workspace 내의 해시 테이블에서 특정 MOP를 찾을 수 없을 때 나타나는 오류입니다, CUBRID 내부의 MOP 관리 로직에 문제가 있거나, 동시성 제어 메커니즘에 예기치 않은 문제가 발생했을 때 나타날 수 있습니다


.. _ERROR-324:

**ERROR CODE: -324, 'Object found in workspace without class.'**

- 이 메시지는 CUBRID 데이터베이스에서 workspace 내의 객체(MOP)가 테이블 정보 없이 존재할 때 나타나는 오류입니다, CUBRID 내부의 객체-테이블 관리 로직에 문제가 있거나, 동시성 제어 메커니즘에 예기치 않은 문제가 발생했을 때 나타날 수 있습니다.


.. _ERROR-325:

**ERROR CODE: -325, 'Attempt to garbage collect a MOP in the dirty list.'**

- 이 메시지는 CUBRID 데이터베이스에서 workspace의 가비지 컬렉션(Garbage Collection) 과정 중, 'dirty list'에 있는 MOP를 정리하려고 시도했을 때 나타나는 오류입니다, workspace 관리 시스템에서 MOP의 상태 일관성 검증 과정에서 발생합니다, 일반적으로 MOP는 'dirty list'에 있으면 가비지 컬렉션 대상이 아니어야 하는데, 이러한 불일치가 발생했을 때 이 오류가 보고됩니다.


.. _ERROR-326:

**ERROR CODE: -326, 'Changing class pointer for object in workspace.'**

- 이 메시지는 CUBRID 데이터베이스에서 workspace에 instance 캐싱시 class MOP되어 발생하는 오류입니다, workspace 관리 시스템에서 객체의 테이블 할당 과정에서 발생합니다, 주로 이미 다른 테이블에 속한 객체를 다른 테이블로 재할당하려고 할 때 발생합니다.


.. _ERROR-327:

**ERROR CODE: -327, 'Cannot create MOP with NULL OID.'**

- 이 메시지는 CUBRID 데이터베이스에서 Managed Object Pointer (MOP)를 생성할 때 Object ID (OID)가 NULL인 경우 나타나는 오류입니다, workspace 관리 시스템에서 MOP 생성 시 OID 유효성 검증 과정에서 발생합니다, 주로 OID 할당 실패하거나 OID가 포함된 메모리 구조가 손상되었을 때 발생할 수 있습니다.


.. _ERROR-328:

**ERROR CODE: -328, 'Instance encountered in workspace without class.'**

- 이 메시지는 CUBRID 데이터베이스에서 workspace에 테이블 정의가 없는 레코드가 발견되었을 때 나타나는 오류입니다, 스키마 관리 시스템에서 레코드와 테이블 간의 관계 검증 과정에서 발생합니다, 주로 테이블를 가져오지 못하거나 테이블를 찾을 수 없을 때 발생합니다.


.. _ERROR-335:

**ERROR CODE: -335, 'Ignoring attempt to free object not allocated within the workspace.'**

- 이 메시지는 CUBRID 데이터베이스에서 workspace(작업 공간) 내에 할당되지 않은 메모리 오브젝트를 해제하려고 시도했을 때 나타나는 오류입니다, 메모리 관리 시스템에서 포인터 유효성 검증 과정에서 발생하는데 주로 메모리 포인터가 유효한 area 블록 내에 있지 않을 때 발생합니다.


.. _ERROR-344:

**ERROR CODE: -344, '%1$s.'**

- 이 메시지는 CUBRID 데이터베이스 프로세스(cub_server)가 shutdown 진행 중이라는 경고성 메세지입니다.


.. _ERROR-345:

**ERROR CODE: -345, 'Error occurred during reading a shutdown message from master.'**

- 이 메시지는 CUBRID 데이터베이스 프로세스(cub_server)가 마스터 프로세스(cub_master)로 부터 shutdown 요청 메세지를 확인시 발생합니다.


.. _ERROR-347:

**ERROR CODE: -347, 'Could not establish a pipe to the master server.'**

- 이 메시지는 CUBRID 데이터베이스 서버 프로세스(cub_server)에서 마스터 프로세스(cub_master)와 통신 수행하려는데, invalid socket인 경우 발생하는 오류입니다.


.. _ERROR-350:

**ERROR CODE: -350, 'Cannot find hostname ("%1$s") in "%2$s". Please check permissions of the file or if there is the hostname.'**

- 이 메시지는 CUBRID 시스템에서 hostname으로 ip address 가져오는 발생하는 오류입니다, /etc/hosts의 권한 오류 또는 hostname 정보가 없는 경우, use_user_host = yes인 경우, $CUBRID/conf/cubrid_hosts.conf 에 hostname 정보가 없는 경우에서 발생됩니다.


.. _ERROR-363:

**ERROR CODE: -363, 'Recvmsg from master socket failed.'**

- 이 메시지는 CUBRID 데이터베이스 프로세스(cub_server)와 마스터 프로세스(cub_master) 연결 후 메시지가 수신할 경우 발생하는 오류입니다.


.. _ERROR-364:

**ERROR CODE: -364, 'Sending a new request to server.'**

- 이 메시지는 CUBRID 데이터베이스 프로세스(cub_server)와 마스터 프로세스(cub_master) 연결이 종료된 경우 발생합니다.


.. _ERROR-366:

**ERROR CODE: -366, '366 Error from server: %1$s'**

- 이 메시지는 CUBRID HA 환경에서 change mode시 발생하거나 CAS 프로세스가 standy server에 연결되었을 경우 데이터베이스 서버 프로세스(cub_server)가 연결 종료시킬때 발생하는 오류입니다.


.. _ERROR-367:

**ERROR CODE: -367, 'Server %1$s already exists.'**

- 이 메시지는 CUBRID 시스템에서 데이터베이스 생성 시 동일한 데이터베이스가 이미 존재하는 경우 발생합니다.

.. _ERROR-368:

**ERROR CODE: -368, 'Communication error during connect for %1$s.'**

- 이 메시지는 CUBRID 시스템에서 데이터베이스 프로세스(cub_server)구동 시 마스터 프로세스(cub_master)와 연결 중 발생합니다.


.. _ERROR-423:

**ERROR CODE: -423, 'Invalid session.'**

- 이 메시지는 CUBRID 데이터베이스에서 유효하지 않은 세션이 사용되었을 때 나타나는 오류입니다, 세션 검증 과정에서 세션이 NULL이거나 파서(parser)가 초기화되지 않은 상태일 때입니다.


.. _ERROR-430:

**ERROR CODE: -430, 'Unknown variable "%1$s".'**

- 이 메시지는 CUBRID 데이터베이스에서 정의되지 않은 변수가 참조되었을 때 나타나는 오류입니다, 변수 검증 과정에서 호스트 변수(host variable) 처리 과정에서 변수를 찾을 수 없을 때입니다.


.. _ERROR-456:

**ERROR CODE: -456, 'Data type references are incompatible.'**

- 이 메시지는 CUBRID 데이터베이스에서 서로 다른 데이터 타입 간의 참조나 연산이 호환되지 않을 때 나타나는 오류입니다, 쿼리에서 데이터 타입 호환성 검증 과정에서 발생합니다, 즉, 서로 다른 데이터 타입 간의 비교, 연산, 할당 등이 불가능한 경우 발생합니다.


.. _ERROR-469:

**ERROR CODE: -469, 'Invalid data type used.'**

- 이 메시지는 CUBRID 데이터베이스에서 지원하지 않는 또는 잘못된 데이터 타입이 사용되었을 때 나타나는 오류입니다.


.. _ERROR-547:

**ERROR CODE: -547, 'Server release %1$s is different from client release %2$s.'**

- 이 메시지는 CUBRID 데이터베이스에서 서버와 클라이언트의 릴리즈 버전이 다를 때 나타나는 오류입니다, 주로 네트워크 연결 과정에서 서버와 클라이언트 간의 버전 호환성 검사에서 발생합니다, 서버와 클라이언트의 릴리즈 버전이 호환되지 않을 때 연결이 거부됩니다.


.. _ERROR-554:

**ERROR CODE: -554, 'The loader has entered an invalid state. No further operations are possible.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 잘못된 상태에 진입했을 때 발생하는 오류입니다, 이는 주로 loaddb 명령어 세션이 유효하지 않거나, 로더가 예상치 못한 상태에 있을 때 발생합니다, 로더(loaddb)가 이 상태에 있으면 더 이상의 데이터 로드 작업을 수행할 수 없습니다.


.. _ERROR-555:

**ERROR CODE: -555, 'Memory allocation error during database loading.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 필요한 메모리를 할당하지 못했을 때 발생합니다, 로딩 작업 중 대량의 데이터를 처리하거나, 시스템의 가용 메모리가 부족한 경우, 또는 메모리 파편화, 운영체제의 제한 등으로 인해 할당 요청이 실패할 수 있습니다.


.. _ERROR-556:

**ERROR CODE: -556, 'Too many values supplied. Expecting %1$d.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 데이터를 로드할 때, 제공된 값의 개수가 예상된 개수보다 많을 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 속성 값을 처리할 때 발생합니다, 테이블에 정의된 속성의 개수보다 많은 값이 제공되었을 때 발생합니다.


.. _ERROR-559:

**ERROR CODE: -559, 'For attribute %1$s of %2$s, expected type %3$s, got %4$s.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 데이터를 로드할 때, 속성의 도메인 타입이 예상된 타입과 일치하지 않을 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 도메인 타입을 검증할 때 발생합니다, 특정 속성에 대해 예상된 타입과 실제로 받은 타입이 다를 때 발생합니다.


.. _ERROR-560:

**ERROR CODE: -560, 'For attribute %1$s of %2$s, domain is not specific enough to support\n unqualified object references.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 데이터를 로드할 때, 객체 참조의 도메인이 모호할 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 객체 도메인을 검증할 때 발생합니다, 특정 속성의 도메인이 unqualified 객체 참조를 지원하기에 충분히 구체적이지 않을 때 발생합니다.


.. _ERROR-561:

**ERROR CODE: -561, 'Nested sets are not allowed.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 데이터를 로드할 때, 중첩된 집합(collection)을 처리하려고 할 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 컬렉션 요소를 처리할 때 발생합니다, CUBRID는 중첩된 집합을 허용하지 않으므로, 집합 내에 또 다른 집합이 포함되려고 하면 이 오류가 발생합니다.


.. _ERROR-562:

**ERROR CODE: -562, 'System class %1$s cannot be populated using the database loader.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 데이터를 로드할 때, 시스템 테이블를 로드하려고 할 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 테이블 컨텍스트를 초기화할 때 발생합니다, 시스템 테이블는 CUBRID 내부에서 사용되는 특별한 테이블로, 일반적인 데이터 loaddb를 통해 로드할 수 없습니다.


.. _ERROR-563:

**ERROR CODE: -563, 'Reference to internal class %1$s converted to NULL.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 데이터를 로드할 때, 내부 테이블에 대한 참조를 처리할 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 테이블 레코드를 생성할 때 발생합니다, 내부 테이블에 대한 참조가 발견되면, 이를 NULL로 변환하여 처리합니다.


.. _ERROR-565:

**ERROR CODE: -565, 'Constructor method %1$s is not defined on class %2$s.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 데이터를 로드할 때, 지정된 생성자 메소드가 테이블에 정의되어 있지 않을 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 테이블 레코드를 생성할 때 발생합니다, 생성자 메소드가 테이블에 존재하지 않거나 정의되지 않았을 때 발생합니다.


.. _ERROR-566:

**ERROR CODE: -566, 'No class specified or no attribute in class.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 데이터를 로드할 때, 테이블가 지정되지 않았거나 테이블에 속성이 없을 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 테이블 레코드를 생성할 때 발생합니다, 테이블가 존재하지 않거나, 테이블는 존재하지만 속성이 정의되지 않았을 때 발생합니다.


.. _ERROR-567:

**ERROR CODE: -567, 'Too many constructor method arguments supplied. Expected %1$d.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 예상했던 인자의 개수보다 많은 수의 인자가 제공되었을 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 테이블 레코드를 생성할 때 발생합니다, 생성자 메소드에 필요한 인자의 개수보다 많은 수의 인자가 제공되었을 때 발생합니다.


.. _ERROR-568:

**ERROR CODE: -568, 'Missing constructor method parameters. Expected %1$d, found %2$d.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 예상했던 인자의 개수와 실제로 제공된 인자의 개수가 일치하지 않을 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 테이블 레코드를 생성할 때 발생합니다, 생성자 메소드에 필요한 인자의 개수보다 적은 수의 인자가 제공되었을 때 발생합니다.


.. _ERROR-569:

**ERROR CODE: -569, 'Missing attribute values.  Expected %1$d, found %2$d.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 데이터를 로드하는 과정에서 예상했던 속성 값의 개수와 실제로 발견된 속성 값의 개수가 일치하지 않을 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 테이블 레코드를 생성할 때 발생합니다, 테이블에 정의된 속성의 개수보다 적은 수의 속성 값이 제공되었을 때 발생합니다.


.. _ERROR-570:

**ERROR CODE: -570, 'Could not access Glo data file "%1$s".'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 GLO(Generalized Large Object) 데이터 파일에 액세스할 수 없을 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 BLOB(Binary Large Object) 또는 CLOB(Character Large Object) 데이터를 처리할 때 발생합니다, LOB 데이터 파일의 경로가 잘못되었거나, 파일이 존재하지 않거나, 권한이 없을 때 발생합니다.


.. _ERROR-571:

**ERROR CODE: -571, 'Instance was previously created through a forward reference.\nThis is not allowed for instances created using constructor methods.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 forward reference를 통해 이미 생성된 레코드를 constructor 메소드를 사용하여 다시 생성하려고 할 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 테이블 레코드를 생성할 때 발생합니다.


.. _ERROR-576:

**ERROR CODE: -576, 'Reference to class %3$s is not compatible with the domain of %1$s of %2$s.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 테이블 객체를 참조할 때, 해당 참조가 특정 속성의 도메인과 호환되지 않을 때 발생하는 오류입니다, 이는 주로 데이터 로드 과정에서 테이블 레코드를 생성하거나 업데이트할 때 발생합니다, 테이블 참조가 속성의 도메인과 일치하지 않을 때 발생합니다.


.. _ERROR-579:

**ERROR CODE: -579, 'Target domain must include type "object" to allow references to classes.'**

- 이 메시지는 CUBRID 데이터베이스 loaddb 명령어에서 테이블 객체를 참조할 때 도메인 타입이 올바르지 않을 때 발생하는 오류입니다, 테이블 객체를 참조하려면 대상 도메인이 "object" 타입을 포함해야 합니다, 이는 CUBRID의 타입 시스템에서 테이블 참조를 위한 특별한 요구사항입니다.


.. _ERROR-581:

**ERROR CODE: -581, 'Attempted to update the database when updates are disabled.'**

- 이 메시지는 CUBRID 데이터베이스 서버가 standby 또는 to_be_active 상태로 데이터 수정이 비활성화된 상태에서 데이터베이스를 수정하려고 할 때 발생하는 오류입니다, 데이터베이스 수정이 비활성화된 상태는 읽기 전용 모드나 특별한 상황에서 발생할 수 있습니다.


.. _ERROR-582:

**ERROR CODE: -582, 'Unknown storage purpose %1$d. Valid range is from %2$d to %3$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 볼륨의 사용 용도(purpose)가 유효하지 않을 때 발생하는 디스크 관리 오류입니다, 볼륨 사용 용도는 데이터가 영구적인지(permanent) 또는 임시적인지(temporary)를 나타내는 값입니다, 유효한 범위를 벗어난 값이 전달되어 볼륨을 생성하거나 관리할 수 없습니다.


.. _ERROR-588:

**ERROR CODE: -588, 'INTERNAL ERROR: Assertion '%1$s' failed.'**

- 이 메시지는 CUBRID 데이터베이스에서 내부적인 오류로 특정 조건이 실패했을 때 발생하는 내부 오류입니다, 이 오류가 발생하면 CUBRID의 내부 로직에 문제가 있음을 의미합니다.


.. _ERROR-589:

**ERROR CODE: -589, 'Invalid user name "%1$s".'**

- 이 메시지는 CUBRID 데이터베이스에서 사용자 이름이 유효하지 않거나 존재하지 않음을 나타냅니다, 사용자 인증, 권한 관리, 저장 프로시저 생성 등에서 사용자 이름을 검증할 때 발생합니다, 이는 존재하지 않는 사용자를 참조할 때 발생할 수 있습니다.


.. _ERROR-595:

**ERROR CODE: -595, 'The database name "%1$s" is too long. Database name should be less than %2$d characters.'**

- 이 메시지는 CUBRID에서 데이터베이스 이름이 최대 길이를 초과해서 데이터베이스 이름과 동일한 볼륨 파일명으로 생성할 때 발생하는 오류입니다.

.. _ERROR-611:

**ERROR CODE: -611, '%1$s cannot be executed in client/server mode.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 기능이 클라이언트/서버 모드에서 실행될 수 없을 때 나타납니다, 해당 기능은 standalone 모드에서만 실행 가능하다는 의미입니다.


.. _ERROR-612:

**ERROR CODE: -612, 'Logging cannot be disabled at this moment since there are pending actions to recover.'**

- 이 메시지는 CUBRID 데이터베이스에서 loaddb -S 에서 --no-logging 사용 시 로깅을 비활성화하려고 할 때 발생합니다, 복구해야 할 pending action이 있는 상태에서는 로깅을 비활성화할 수 없다는 의미입니다.


.. _ERROR-619:

**ERROR CODE: -619, 'Bad source codeset.'**

- 이 메시지는 CUBRID 데이터베이스에서 문자열의 codeset 변환 시 소스(source) 코드 세트가 유효하지 않거나 지원되지 않을 때 나타납니다, 문자열을 한 코드 세트에서 다른 코드 세트로 변환할 때 소스 코드 세트가 잘못 지정되었을 때 발생할 수 있습니다.


.. _ERROR-629:

**ERROR CODE: -629, 'Pagesize %1$d is not a power of 2 or pagesize is too small. Pagesize of %2$d is used instead.'**

- 이 메시지는 CUBRID 데이터베이스에서 페이지 크기를 설정할 때 발생하는 경고입니다, 사용자가 지정한 페이지 크기가 2의 제곱수가 아니거나 최소 크기보다 작을 때 나타납니다, CUBRID 시스템이 자동으로 유효한 페이지 크기로 조정했음을 알립니다.


.. _ERROR-630:

**ERROR CODE: -630, 'Conversion not supported by %1$s.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 데이터 타입 간의 변환을 시도했지만, 해당 변환이 시스템에서 지원되지 않을 때 발생하는 오류입니다, 내부 함수에서 타입 간의 변환을 시도할 때 발생합니다, 예를 들어, 지원되지 않는 데이터 타입 조합이나 복합 타입 간의 변환을 시도할 때 이 오류가 발생할 수 있습니다.


.. _ERROR-632:

**ERROR CODE: -632, '%1$s is not a backup volume.'**

- 이 메시지는 CUBRID 데이터베이스에서 지정된 파일이나 디렉토리가 유효한 백업 볼륨이 아닐 때 발생하는 오류입니다, 백업 파일의 헤더를 읽을 수 없거나, 백업 파일의 매직 넘버가 올바르지 않을 때 나타납니다, CUBRID 백업 파일이 아닌 다른 형식의 파일을 백업 파일로 인식하려고 할 때 발생합니다.


.. _ERROR-633:

**ERROR CODE: -633, '1$s is a backup of database %2$s created on %3$s instead of given database %4$s created on %5$s'**

- 이 메시지는 CUBRID 데이터베이스에서 백업 파일을 복원할 때 발생하는 오류입니다, 백업 파일이 다른 데이터베이스의 백업이거나 다른 시점에 생성된 데이터베이스의 백업일 때 나타납니다, 사용자가 지정한 데이터베이스와 백업 파일에 포함된 데이터베이스 정보가 일치하지 않을 때 발생합니다.


.. _ERROR-634:

**ERROR CODE: -634, 'A database volume/file was expected in backup.'**

- 이 메시지는 CUBRID 데이터베이스에서 백업 파일을 복원하거나 목록을 확인할 때 발생하는 오류입니다, 백업 파일 내에서 데이터베이스 볼륨이나 파일 정보를 찾을 수 없을 때 나타납니다, 백업 파일의 구조가 손상되었거나 예상된 형식과 다를 때 발생합니다.


.. _ERROR-648:

**ERROR CODE: -648, 'Backup is incompatible with current "%1$s" release "%2$s".'**

- 이 메시지는 CUBRID 데이터베이스에서 백업 파일이 현재 CUBRID 버전과 호환되지 않을 때 발생하는 오류입니다, 백업 파일을 생성한 CUBRID 버전과 복구를 시도하는 CUBRID 버전 간의 호환성 문제를 나타냅니다, 이는 백업 파일이 현재 버전에서 지원되지 않을 때 발생하는 오류입니다.


.. _ERROR-669:

**ERROR CODE: -669, 'Server refused client connection: max clients, (%1$d), exceeded.'**

- 이 메시지는 CUBRID 데이터베이스 서버가 설정된 최대 클라이언트(max_clients) 연결 수를 초과하여 새로운 클라이언트 연결을 거부했을 때 발생합니다, 서버의 리소스 보호와 성능 유지를 위해 동시 연결 수를 제한하는 메커니즘입니다, 현재 활성화된 클라이언트 연결 수가 서버에서 허용하는 최대값을 초과했음을 의미합니다.


.. _ERROR-675:

**ERROR CODE: -675, 'Cannot read the database location file, '%1$s'.'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 데이터베이스 위치 파일(예: `databases.txt`)을 읽으려고 시도했지만 실패했을 때 발생하는 오류입니다, 이 파일은 CUBRID가 시스템에 등록된 데이터베이스의 위치와 정보를 파악하는 데 사용됩니다, 파일에 접근할 수 없거나 내용이 손상된 경우 이 오류가 발생합니다. 


.. _ERROR-676:

**ERROR CODE: -676, 'Database '%1$s', cannot be found in the database location file, '%2$s''**

- 이 메시지는 CUBRID 데이터베이스 시스템이 특정 데이터베이스를 시작하거나 연결하려고 할 때, 해당 데이터베이스의 정보가 데이터베이스 위치 파일(예: `databases.txt`)에서 발견되지 않아 발생하는 오류입니다, CUBRID는`databases.txt`와 같은 설정 파일을 통해 시스템에 등록된 데이터베이스의 위치와 정보를 관리합니다, 이 파일에 데이터베이스 정보가 없으면 시스템은 해당 데이터베이스를 인식할 수 없습니다.


.. _ERROR-677:

**ERROR CODE: -677, 'Failed to connect to database server, '%1$s', on the following host(s): %2$s'**

- 이 메시지는 CUBRID 데이터베이스에서 클라이언트가 지정된 호스트(들)에서 데이터베이스 서버에 연결을 시도했지만 실패했을 때 발생하는 오류입니다, 네트워크 연결, 서버 상태, 인증 등 다양한 원인으로 인해 데이터베이스 서버에 연결할 수 없는 상황을 나타냅니다, 자세한 원인은 csql,err 파일 또는 $CUBRID/log/*.err 파일 등을 확인 필요합니다.


.. _ERROR-695:

**ERROR CODE: -695, 'Wrong interface has been used to kill a client.'**

- 이 메시지는 CUBRID 데이터베이스에서 killtran 명령어로 클라이언트 세션을 강제 종료 하려고 할 때 잘못된 인터페이스나 부적절한 매개변수가 사용되었을 때 발생합니다, 클라이언트 종료 작업을 수행하기 위해서는 적절한 트랜잭션 인덱스, 사용자명, 호스트명 등의 정보가 필요하지만, 이러한 정보가 부족하거나 잘못된 경우 이 오류가 발생합니다.


.. _ERROR-696:

**ERROR CODE: -696, 'Unknown transaction (index %1$d, %2$s@%3$s|%4$d).'**

- 이 메시지는 CUBRID 데이터베이스에서 killtran 명령어로 특정 트랜잭션을 강제 종료 하려고 시도했으나, 요청된 트랜잭션 인덱스에 해당하는 트랜잭션이 현재 시스템에 존재하지 않을 때 발생합니다, 주로 클라이언트가 잘못된 트랜잭션 인덱스를 사용하거나, 해당 트랜잭션이 이미 종료되어 시스템에서 제거되었을 때 발생할 수 있습니다.


.. _ERROR-697:

**ERROR CODE: -697, 'Given transaction index (index %1$d, %2$s@%3$s|%4$d) does not match with current transaction index (index %5$d, %6$s@%7$s|%8$d). Likely, the given transaction has finished and its transaction index was re-assigned to another transaction.'**

- 이 메시지는 CUBRID 데이터베이스에서 killtran 명령어로 특정 트랜잭션을 강제 종료 하려고 시도했으나, 요청된 트랜잭션 인덱스와 현재 시스템에서 인식하는 트랜잭션 정보가 일치하지 않을 때 발생합니다, 주로 클라이언트가 오래된(stale) 트랜잭션 인덱스를 가지고 있거나, 서버에서 해당 트랜잭션이 이미 종료되어 인덱스가 다른 트랜잭션에 재할당되었을 때 발생할 수 있습니다.


.. _ERROR-743:

**ERROR CODE: -743, 'Failed on handshake between client and server. (peer host %1$s)'**

- 이 메시지는 CUBRID 데이터베이스에서 클라이언트와 서버 간의 초기 연결 설정 과정(handshake)에서 오류가 발생했을 때 나타나는 오류입니다, Handshake는 클라이언트와 서버가 통신을 시작하기 전에 서로의 버전, 기능, 호환성을 확인하는 과정입니다, 이 과정에서 클라이언트의 버전 정보를 받지 못하거나, 서버와 클라이언트 간의 기능 호환성 검사가 실패할 때 발생합니다, 네트워크 연결 문제나 서버 상태 문제로 인해 발생할 수 있습니다.


.. _ERROR-752:

**ERROR CODE: -752, 'Error restoring backup unit_num %1$d.'**

- 이 메시지는 CUBRID 데이터베이스 백업을 복원하는 과정에서, 백업 파일에서 데이터를 읽는 중에 발생하는 일반적인 I/O 오류입니다, 주로 백업 파일의 물리적 손상, 디스크 I/O 오류, 파일 접근 권한 문제, 또는 백업 파일 형식의 불일치 등 다양한 원인으로 발생할 수 있습니다.


.. _ERROR-753:

**ERROR CODE: -753, 'Error restoring backup unit_num %1$d, pageid %2$d exceeds total number of pages %3$d for volid %4$d.'**

- 이 메시지는 CUBRID 데이터베이스 백업을 복원하는 과정에서, 백업 파일 내의 페이지 ID가 복원 대상 볼륨의 유효한 페이지 범위를 벗어났을 때 발생합니다, CUBRID 백업 파일의 메타데이터가 손상되었거나, 백업이 생성될 때와 다른 환경(예: 더 작은 볼륨 크기)으로 복원하려 할 때 발생할 수 있습니다.


.. _ERROR-755:

**ERROR CODE: -755, '%1$s cannot be executed in stand-alone mode.'**

- 이 메시지는 CUBRID 데이터베이스에서 스탠드얼론 모드로 실행 중일 때, 클라이언트-서버 통신이 필요한 작업을 시도했을 때 발생합니다, CUBRID 데이터베이스에서 checkpoint와 같은 특정 작업이나 명령이 스탠드얼론(stand-alone) 모드에서는 실행할 수 없을 때 발생하는 이 오류를 나타냅니다.


.. _ERROR-757:

**ERROR CODE: -757, 'Internal Error: cannot assign permanent OID during object encode.'**

- 이 메시지는 CUBRID 데이터베이스에서 객체를 인코딩하는 과정 중 영구 OID를 할당하지 못했을 때 발생합니다. OID는 데이터베이스 내에서 각 객체를 고유하게 식별하는 중요한 값입니다. 영구 OID 할당 실패는 객체 생성 또는 영속화 과정에 문제가 있음을 의미합니다, 이는 주로 시스템 내부의 데이터 일관성 문제, 저장 공간 부족, 트랜잭션 문제, 또는 내부 로직 오류 등으로 인해 발생할 수 있습니다.


.. _ERROR-767:

**ERROR CODE: -767, 'Cannot create a level %1$d backup without first creating a level %2$d backup.'**

- 이 메시지는 CUBRID 데이터베이스에서 증분 백업을 수행할 때 백업 레벨의 연속성이 보장되지 않을 때 발생합니다,이는 백업 레벨을 지정할 때 2는 1이, 1은 0 레벨 백업이 없을 때 오류가 발생하고 CUBRID는 다음과 같은 백업 레벨을 지원합니다:
Level 0 (Full Backup): 전체 데이터베이스 백업
Level 1 (Big Incremental): Level 0 백업을 기반으로 한 증분 백업
Level 2 (Small Incremental): Level 1 백업을 기반으로 한 증분 백업


.. _ERROR-769:

**ERROR CODE: -769, 'Attribute "%1$s", of class "%2$s" is not a valid class or shared attribute.'**

- 이 메시지는 CUBRID 데이터베이스에서 loaddb 명령어를 수행 시 테이블 속성을 처리할 때, 지정된 속성이 유효한 테이블 속성 또는 공유 속성이 아닌 상황임을 나타냅니다, CUBRID의 데이터 로더 메커니즘에서 발생하며, 주로 속성의 네임스페이스 검증 실패로 인해 발생할 수 있습니다.


.. _ERROR-771:

**ERROR CODE: -771, 'Invalid parameter for function.'**

- 이 메시지는 CUBRID 데이터베이스에서 함수 호출 시 전달된 파라미터가 유효하지 않거나 예상된 형식을 따르지 않을 때 발생합니다, 이는 CUBRID 내부의 특정 함수가 작업을 수행하기 위해 필요한 파라미터의 유효성을 검사하는 과정에서 실패할 경우 오류가 발생됩니다.


.. _ERROR-778:

**ERROR CODE: -778, 'Cannot create a symbolic link "%1$s" to the file "%2$s".'**

- 이 메시지는 CUBRID 데이터베이스에서 심볼릭 링크를 생성하려고 할 때, 심볼릭 링크 생성에 실패한 상황임을 나타냅니다, 이는 CUBRID의 파일 시스템 관리 메커니즘에서 발생하며, 주로 createdb, copydb, renamedb 명령어 수행 시 발생할 수 있습니다.


.. _ERROR-820:

**ERROR CODE: -820, 'Cannot initialize a connection.'**

- 이 메시지는 CUBRID 시스템이 클라이언트와 서버 간의 연결(connection)을 초기화하는 데 실패했을 때 발생하는 내부 오류입니다, 연결 초기화는 클라이언트가 서버에 접속할 때 연결 객체를 생성하고, 필요한 리소스를 할당하며, 연결 상태를 설정하는 과정입니다, 이 과정에서 실패했다는 것은 메모리 할당 실패, 클라이언트 ID 할당 실패, 뮤텍스 초기화 실패 등이 발생했음을 의미합니다, 연결 초기화 실패는 클라이언트가 서버에 접속할 수 없게 만들며, 데이터베이스 서비스 이용이 불가능해집니다.


.. _ERROR-821:

**ERROR CODE: -821, 'Shutdown a connection.'**

- 이 메시지는 CUBRID 시스템이 클라이언트와 서버 간의 연결(connection)을 강제로 종료할 때 발생하는 내부 오류입니다, 이는 오류라기보다는 시스템이 특정 연결을 종료하고 있다는 정보성 메시지입니다, 주로 트랜잭션 중단(slam transaction) 과정에서 발생하며, 해당 연결과 관련된 모든 리소스를 정리하는 과정입니다.


.. _ERROR-827:

**ERROR CODE: -827, 'Connection list traverse function returned invalid value.'**

- 이 메시지는 CUBRID 시스템이 연결 리스트를 순회하는 과정에서 콜백 함수가 잘못된 반환값을 반환했을 때 발생하는 내부 오류입니다, 잘못된 반환값이 반환되었다는 것은 콜백 함수에 버그가 있거나, 예상치 못한 상황이 발생했음을 의미합니다.


.. _ERROR-829:

**ERROR CODE: -829, 'This mutex lock has been unlocked already.'**

- 이 메시지는 CUBRID 데이터베이스의 스레드 관리 모듈에서 크리티컬 섹션을 해제할 때 발생합니다, 즉, 이미 해제된 크리티컬 섹션을 다시 해제하려 할 때 발생하는 오류 메시지입니다, 이는 프로그래밍 오류나 동시성 제어 로직의 문제를 나타내며, 뮤텍스 락의 중복 해제를 방지하기 위한 안전장치입니다.


.. _ERROR-831:

**ERROR CODE: -831, 'All transaction descriptors(%1$d) are in use. Raise up max_clients parameter.'**

- 이 메시지는 CUBRID 데이터베이스 설정에서 max_clients 설정값을 초과해 연결 요청을 받을 때 발생합니다, 이는 max_clients 파리미터 설정값 보다 데이터베이스에 동시에 너무 많은 클라이언트가 접속하거나, 기존 클라이언트들이 트랜잭션을 종료하지 않고 계속 유지하고 있을 때 발생할 수 있습니다.


.. _ERROR-832:

**ERROR CODE: -832, 'Backup is already running. Backup cannot be run by two processes.'**

- 이 메시지는 CUBRID 데이터베이스의 로그 페이지 버퍼 관리 모듈에서 백업(backupdb) 작업의 중복 실행을 방지하기 위해 발생합니다, 데이터베이스 관리자가 백업 작업이 완료되기 전에 또 다른 백업을 시도하거나, 여러 클라이언트에서 동시에 백업을 요청했을 때 발생합니다. 


.. _ERROR-835:

**ERROR CODE: -835, 'Undefined statistics item.'**

- 이 메시지는 CUBRID 데이터베이스의 통계 관리 모듈에서 정의되지 않은 통계 아이템을 참조하려 할 때 발생합니다, CUBRID는 제한된 수의 통계 아이템만을 지원하며, 이 범위를 벗어나는 요청에 대해 이 오류를 반환합니다.


.. _ERROR-837:

**ERROR CODE: -837, 'Cannot drop the user who owns database objects.'**

- 이 메시지는 CUBRID 데이터베이스의 사용자 관리 모듈에서 사용자를 삭제하려 할 때 발생합니다, 데이터베이스의 무결성과 보안을 보장하기 위한 제약 사항으로, 사용자가 소유한 객체들이 무분별하게 삭제되는 것을 방지합니다. 사용자를 삭제하기 전에 먼저 해당 사용자가 소유한 모든 데이터베이스 객체를 삭제하거나 다른 사용자에게 소유권을 이전해야 합니다.


.. _ERROR-841:

**ERROR CODE: -841, 'Requested operation can be executed by only sole client(transaction).'**

- 이 메시지는 CUBRID 데이터베이스의 시스템 파라미터 관리 모듈에서 특정 작업을 수행하려 할 때, 여러 클라이언트(트랜잭션)가 동시에 연결되어 있는 상황에서 발생합니다, 특정 시스템 파라미터를 변경하거나 특정 관리 작업을 수행할 때, 데이터 일관성과 안정성을 보장하기 위해 단일 클라이언트 환경에서만 수행해야 하는 작업에 대한 제약 사항을 나타냅니다.


.. _ERROR-877:

**ERROR CODE: -877, 'No data to be unloaded.'**

- 이 메시지는 CUBRID 데이터베이스에서 스키마나 데이터를 언로드(unload)하는 과정에서 발생합니다, 이런 상황은 일반적으로 데이터베이스에 스키마 정보가 없거나, 언로드 대상 테이블이나 객체가 존재하지 않을 때 나타납니다.


.. _ERROR-878:

**ERROR CODE: -878, 'Invalid database location file, '%1$s'.'**

- 이 메시지는 CUBRID 데이터베이스에서 `databases.txt` 파일을 읽고 파싱하는 과정에서 발생합니다.
 이런 상황은 일반적으로 데이터베이스 시작 시 `databases.txt` 파일의 형식이 올바르지 않거나, 필수 정보(데이터베이스 이름, 경로, 호스트, 로그 경로 등)가 누락되었을 때 나타납니다.


.. _ERROR-883:

**ERROR CODE: -883, 'Failed to get '%1$s' file status.'**

- 이 메시지는 CUBRID 데이터베이스에서 renamedb 명령어 수행 시 특정 파일이나 디렉터리의 상태 정보를 가져오려고 할 때 발생합니다, 이는 파일 시스템 접근 문제, 파일이 존재하지 않는 경우 등으로 인해 발생할 수 있습니다.


.. _ERROR-884:

**ERROR CODE: -884, 'Volext-pathname '%1$s' is not permitted to write.'**

- 이 메시지는 CUBRID 데이터베이스에서 renamedb 명령어 수행 시 특정 경로에 쓰기 권한이 없을 때 발생합니다, 이는 데이터베이스의 파일 시스템 접근 권한 문제로 인해 데이터베이스 운영에 필수적인 파일 작업이 실패했음을 의미합니다.


.. _ERROR-885:

**ERROR CODE: -885, 'Failure to move from '%1$s' to '%2$s'. Use the same device(disk).'**

- 이 메시지는 CUBRID 데이터베이스에서 renamedb -E 옵션과 함께 명령어 수행 시 확장볼륨 파일들을 다른 위치로 이동하려고 할 때 발생합니다, 이는 데이터베이스의 무결성과 성능을 보호하기 위한 보호성 오류입니다.


.. _ERROR-896:

**ERROR CODE: -896, 'Compression failed. Zip Method: %1$d (%2$s), Zip Level: %3$d (%4$s)'**

- 이 메시지는 CUBRID 데이터베이스에서 LZ4 압축 알고리즘을 사용하여 varchar 데이터 또는 log 데이터를  압축할 때 실패하여 발생합니다. 


.. _ERROR-897:

**ERROR CODE: -897, 'Decompression failed.'**

- 이 메시지는 CUBRID가 LZ4 알고리즘으로 압축된 데이터를 해제하는 과정에서 발생합니다. 객체(varchar) 데이터나 로그 데이터의 압축을 해제할 때 발생할 수 있으며, 데이터 손상이나 메모리 문제로 인한 실패를 나타냅니다.


.. _ERROR-910:

**ERROR CODE: -910, 'Not allowed access to partition.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 파티션에 접근하려는 시도가 권한 부족, 파티션의 상태 문제, 또는 잘못된 접근 방식으로 인해 거부되었을 때 발생합니다, 즉, 데이터베이스 시스템이 사용자가 요청한 작업(예: 데이터 삽입, 조회, 수정, 삭제 또는 스키마 변경)을 수행하기 위해 특정 파티션에 접근해야 하지만, 해당 파티션에 대한 접근 권한이 없거나 파티션 자체가 접근 불가능한 상태(예: 오프라인, 손상)일 때 발생하는 오류입니다.
이런 상황은 일반적으로 파티션 테이블에 대한 DML/DDL 작업을 수행할 때, 사용자 계정에 필요한 권한이 없거나, 파티션이 손상되어 접근할 수 없거나, 시스템 내부적으로 파티션 관리 로직에 문제가 발생했을 때 나타납니다, 이는 데이터베이스의 보안, 데이터 무결성 및 시스템 안정성을 보호하기 위한 보호성 오류입니다.


.. _ERROR-941:

**ERROR CODE: -941, 'The operation is not supported.'**

- 이 메시지는 CUBRID 데이터베이스의 클라이언트 인터페이스에서 다양한 상황에서 발생할 수 있는 범용 오류 코드입니다, 즉, 요청된 작업이 현재 CUBRID 버전이나 구현에서 지원되지 않는 경우에 사용됩니다.


.. _ERROR-962:

**ERROR CODE: -962, 'Out of memory'**

- 이 메시지는 CUBRID 데이터베이스의 DBlink gateway에서 메로리 할당을 하지 못한 경우 발생합니다.


.. _ERROR-969:

**ERROR CODE: -969, 'Server bit platform (%1$d) is different from client bit platform (%2$d).'**

- 이 메시지는 CUBRID 데이터베이스 클라이언트가 서버에 연결을 시도할 때, 클라이언트와 서버가 서로 다른 비트 플랫폼(예: 32비트 클라이언트와 64비트 서버, 또는 그 반대)에서 실행되고 있음을 감지했을 때 발생합니다, CUBRID는 클라이언트와 서버 간의 안정적인 통신과 데이터 일관성을 위해 동일한 비트 플랫폼 환경에서 실행되는 것을 권장하며, 이처럼 비트 플랫폼이 다를 경우 연결을 거부하고 이 오류를 발생시킵니다, 이는 데이터 손상이나 예상치 못한 동작을 방지하기 위한 보호성 오류입니다.


.. _ERROR-971:

**ERROR CODE: -971, 'Program '%1$s' (pid %2$d) connected to database server '%3$s' on the host '%4$s' (port %5$d).'**

- 이 메시지는 클라이언트가 CUBRID 데이터베이스 서버에 성공적으로 연결되었을 때 발생하는 알림입니다, 실제 오류가 아닌 정상적인 연결 상태를 나타내는 알림 메시지로, 클라이언트의 연결 정보와 서버 접속 상태를 상세히 보여줍니다.


.. _ERROR-972:

**ERROR CODE: -972, 'Program '%1$s' (pid %2$d) was connected from the host '%3$s'. (transaction index %4$d)'**

- 이 메시지는 CUBRID 데이터베이스에 클라이언트 프로그램이 성공적으로 연결되었음을 알리는 정보성 로그 메시지입니다, 이는 실제 오류가 아니라, 데이터베이스 서버가 클라이언트의 연결 요청을 수락하고 세션을 설정했음을 기록하는 것입니다. 이 메시지는 시스템 관리자가 데이터베이스에 어떤 클라이언트가 언제, 어디서, 어떤 프로그램으로 연결했는지 추적하고 모니터링하는 데 중요한 정보를 제공합니다, 특히, 비정상적인 연결 시도나 보안 감사 시 유용하게 활용될 수 있습니다.


.. _ERROR-973:

**ERROR CODE: -973, 'Server status is %1$s.'**

- 이 메시지는 CUBRID 데이터베이스에서 서버의 현재 상태를 알려주는 정보성 메시지입니다, 이는 실제로는 오류가 아니라 서버의 부팅 과정이나 HA(High Availability) 상태 변경 시 서버의 현재 상태를 로그에 >기록하는 알림 메시지입니다, 서버는 부팅 과정에서 DOWN 상태에서 시작하여 UP 상태로 전환되거나, HA 환경에서 MAINTENANCE 모드로 전환될 때 이 메시지를 출력합니다, 이는 시스템 관리자가 서버의 현재 상태
를 모니터링하고 문제 발생 시 디버깅할 수 있도록 도와주는 역할을 합니다.


.. _ERROR-974:

**ERROR CODE: -974, 'Archive log "%1$s" is created to archive pages from %2$lld to %3$lld.'**

- 이 메시지는 실제 오류가 아닌 정보성 메시지로, 데이터베이스 시스템이 특정 범위의 페이지들을 아카이브 로그 파일로 성공적으로 생성했음을 나타냅니다. 이는 데이터베이스의 백업과 복구를 위한 정상적인 로
그 아카이빙 프로세스의 일부입니다.


.. _ERROR-977:

**ERROR CODE: -977, 'Checkpoint started (previous checkpoint page id: %1$lld, previous redo page id: %2$lld).'**

- 이 메시지는 CUBRID 데이터베이스에서 체크포인트 작업이 시작되었음을 알리는 시스템 알림입니다. 데이터베이스의 일관성과 복구 가능성을 보장하기 위한 중요한 작업의 시작을 나타냅니다.


.. _ERROR-978:

**ERROR CODE: -978, 'Checkpoint finished (checkpoint page id: %1$lld, checkpoint redo page id: %2$lld, flushed page count: %3$d).'**

- 이 메시지는 CUBRID 데이터베이스에서 체크포인트 작업이 완료되었음을 알리는 시스템 알림입니다. 체크포인트 과정에서 처리된 페이지 정보와 함께 작업의 성공적인 종료를 나타냅니다.


.. _ERROR-982:

**ERROR CODE: -982, 'Set interrupt to the transaction %1$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 트랜잭션에 대해 인터럽트(중단 신호)가 성공적으로 설정되었음을 나타냅니다, 이는 일반적으로 외부 요인(예: 사용자 요청, 시스템 관리자의 강제 종료 명령, 타임아웃 등)에 의해 해당 트랜잭션의 실행을 중단시키려는 시도가 있었고, 그 시도가 시스템 내부적으로 처리되었음을 의미합니다, 이 메시지는 오류라기보다는 트랜잭션 중단 요청이 처리되었음을 알리는 정보성 메시지에 가깝습니다. 인터럽트가 설정되면 해당 트랜잭션은 현재 수행 중인 작업을 중단하고 롤백될 수 있습니다.


.. _ERROR-984:

**ERROR CODE: -984, 'Cannot set %3$s to "%6$s" in the "[%5$s]" section when it is set to "%4$s" in the "[%2$s]" section of the configuration file "%1$s".'**

- 이 메시지는 CUBRID 설정 파일($CUBRID/conf/cubrid.conf)의 ha_mode 파라미터 설정값이 다른 값으로 설정되어 있을 때 발생합니다, 이는 설정 파일의 일관성을 보장하고 파라미터 값의 충돌을 방지하기 위한 보호성 오류입니다. 특히 HA(High Availability) 모드와 같은 중요한 파라미터에서 발생할 수 있습니다.


.. _ERROR-985:

**ERROR CODE: -985, 'The hostname on the database connection string should be specified when multihost is set in "databases.txt". ex) csql demodb@localhost'**

- 이 메시지는 CUBRID 데이터베이스 클라이언트가 `databases.txt` 파일에 `multihost` 옵션이 활성화된 데이터베이스에 연결을 시도할 때 발생합니다. `multihost` 옵션은 데이터베이스가 여러 호스트에 걸쳐 서비스될 수 있음을 나타내며, 이 경우 클라이언트는 연결하려는 특정 서버의 호스트 이름을 명시적으로 지정해야 합니다, 클라이언트가 `csql demodb`와 같이 데이터베이스 이름만으로 연결을 시도하면, CUBRID는 어떤 호스트에 연결해야 할지 알 수 없으므로 이 오류를 발생시킵니다. 이는 클라이언트가 올바른 서버 레코드에 연결되도록 유도하고, 모호한 연결 시도를 방지하기 위한 보호성 오류입니다.


.. _ERROR-991:

**ERROR CODE: -991, 'Flush victim candidates of page buffer started.'**

- 이 메시지는 CUBRID 데이터베이스의 페이지 버퍼에서 희생자(victim) 페이지 플러시 작업이 시작되었음을 알리는 시스템 메시지입니다. 메모리 관리를 위해 더티 페이지를 디스크에 기록하는 중요한 작업의 시작을 나타냅니다.


.. _ERROR-992:

**ERROR CODE: -992, 'Flush victim candidates of page buffer finished (count: %1$d).'**

- 이 메시지는 CUBRID 데이터베이스의 페이지 버퍼에서 희생자(victim) 페이지 플러시 작업이 완료되었음을 알리는 시스템 메시지입니다. 플러시된 페이지 수와 함께 작업의 성공적인 종료를 나타냅니다.


.. _ERROR-994:

**ERROR CODE: -994, 'This statement cannot be a prepared statement.'**

- 이 메시지는 CUBRID 데이터베이스에서 prepare from 구문에서 prepared statement 구문 (prepare, execute, deallocated/drop)을 사용하려는 경우 발생합니다.


.. _ERROR-995:

**ERROR CODE: -995, 'A prepared statement with the name %1$s does not exist.'**

- 이 메시지는 CUBRID 데이터베이스에서 `EXECUTE` 문을 사용하여 특정 이름의 prepared statement를 실행하려고 할 때 발생합니다, Prepared statement는 `PREPARE` 문을 통해 미리 준비된 SQL 문으로, `EXECUTE` 문을 통해 실행됩니다. 하지만 요청한 이름의 prepared statement가 현재 세션에 존재하지 않는 경우 이 오류가 발생합니다. 
이는 다음과 같은 상황에서 발생할 수 있습니다, prepared statement가 아직 생성되지 않았거나, 세션이 종료되어 prepared statement가 삭제되었거나, prepared statement 이름이 잘못 입력되었거나, 다른 세션에서 생성된 prepared statement를 현재 세션에서 실행하려고 할 때입니다,  Prepared statement는 세션별로 관리되므로, 세션이 변경되면 이전에 준비된 statement에 접근할 수 없습니다.


.. _ERROR-1001:

**ERROR CODE: -1001, 'An unexpected condition or state has been reached. '%1$s''**

- 이 메시지는 CUBRID 데이터베이스 시스템이 내부적으로 예상치 못한 상태나 조건을 만났을 때 발생합니다, 개발자가 예측하지 못했거나, 특정 오류 코드를 할당하기 어려운 일반적인 내부 오류 상황에 사용되는 포괄적인 오류 코드입니다, 보통 구체적인 오류 상황에 대한 설명을 제공하여 문제의 원인을 추적하는 데 도움을 줍니다. 이 메시지는 데이터베이스의 논리적 결함, 데이터 손상, 비정상적인 시스템 상태 또는 환경 문제로 인해 발생할 수 있습니다.


.. _ERROR-1009:

**ERROR CODE: -1009, 'COMPACTDB already started.'**

이 오류 코드는 이미 실행 중인 COMPACTDB 프로세스가 있을 때 새로운 COMPACTDB 작업을 시도할 경우 발생합니다.


.. _ERROR-1066:

**ERROR CODE: -1066, 'Session expired.'**

- 이 메시지는 CUBRID 데이터베이스에서 클라이언트 세션이 유효 기간을 초과하여 만료되었을 때 발생합니다. 세션은 클라이언트와 서버 간의 연결 상태를 유지하고 사용자별 정보를 저장하는 논리적인 단위입니다. 세션이 만료되면 해당 세션을 통해 진행 중이던 모든 작업이 중단되고, 클라이언트는 데이터베이스에 대한 접근 권한을 잃게 됩니다. 이는 주로 세션 타임아웃 설정, 클라이언트의 비활성 상태 유지, 또는 네트워크 연결 불안정 등으로 인해 발생할 수 있습니다. 데이터베이스의 보안과 리소스 관리를 위한 보호성 오류입니다.


.. _ERROR-1069:

**ERROR CODE: -1069, 'Too many prepared statements.'**

- 이 메시지는 CUBRID 데이터베이스에서 단일 세션이 허용된 최대 준비된 문장(prepared statements) 개수를 초과하여 문장을 준비하거나 사용하려고 할 때 발생합니다, 이는 PREPARE STMT 구문에서 데이터베이스 시스템은 과도한 메모리 사용이나 리소스 고갈을 방지하기 위해 세션당 준비된 문장의 개수를 제한합니다. 이 메시지는 주로 애플리케이션이나 쿼리가 예상보다 많은 준비된 문장을 사용하거나, 시스템의 `MAX_PREPARED_STMT_COUNT `와 같은 관련 파라미터 설정이 현재 워크로드에 비해 너무 낮게 설정되어 있을 때 나타납니다. 이는 데이터베이스의 안정성과 리소스 관리를 위한 보호성 오류입니다.


.. _ERROR-1070:

**ERROR CODE: -1070, 'Session variable '@%1$s' not defined.'**

- 이 메시지는 CUBRID 데이터베이스에서 현재 세션이 존재하지 않거나 정의되지 않은 세션 변수를 사용하려고 시도할 때 발생합니다. 세션 변수는 특정 사용자 세션 내에서만 유효한 임시 변수이며, 데이터베이스 시스템이 요청된 변수를 찾을 수 없을 때 이 오류를 반환합니다. 이는 주로 애플리케이션이나 쿼리에서 변수 이름의 오타, 변수 미정의, 또는 변수의 유효 범위(스코프)를 벗어난 접근 시도 등으로 인해 발생합니다. 이 메시지는 존재하지 않는 데이터에 대한 작업을 방지하고 데이터 무결성을 보장하기 위한 보호성 오류입니다.


.. _ERROR-1071:

**ERROR CODE: -1071, 'Too many session variables.'**

- 이 메시지는 CUBRID 데이터베이스에서 단일 세션이 허용된 최대 세션 변수 개수를 초과하여 변수를 생성하거나 사용하려고 할 때 발생합니다. 세션 변수는 특정 사용자 세션 내에서만 유효한 임시 변수로, 데이터베이스 시스템은 과도한 메모리 사용이나 리소스 고갈을 방지하기 위해 세션 변수의 개수를 제한합니다. 


.. _ERROR-1072:

**ERROR CODE: -1072, 'IP address(%1$s) is not authorized.'**

- 이 메시지는 CUBRID 데이터베이스 서버가 클라이언트의 IP 주소가 접근 제어 목록(Access Control List, ACL)에 허용되지 않는다고 판단했을 때 발생합니다. 즉, 데이터베이스에 연결을 시도하는 IP 주소가 서버의 보안 정책에 의해 차단되었음을 의미합니다. 이는 데이터베이스의 무단 접근을 방지하고 보안을 강화하기 위한 보호성 오류입니다.


.. _ERROR-1073:

**ERROR CODE: -1073, 'Invalid format in file(%1$s).'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 특정 파일(특히 IP 접근 제어 파일)을 읽고 파싱하는 과정에서, 해당 파일의 내용이 예상된 형식이나 문법을 따르지 않을 때 발생합니다. 이는 주로 파일 내용의 오타, 잘못된 구문, 또는 비정상적인 수정으로 인해 발생할 수 있습니다. 이 오류가 발생하면 CUBRID는 해당 파일의 설정을 올바르게 적용할 수 없으므로, 서비스 시작이 실패하거나 예상치 못한 접근 제어 문제가 발생할 수 있습니다. 이는 시스템의 보안 및 안정적인 운영에 필수적인 설정 파일의 무결성 문제로, 즉각적인 확인과 수정이 필요합니다.


.. _ERROR-1074:

**ERROR CODE: -1074, 'Cannot read access list file(%1$s).'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 특정 접근 목록 파일(Access List File)을 열거나 읽는 데 실패했을 때 발생합니다. 접근 목록 파일은 CUBRID의 보안 및 접근 제어 정책을 정의하는 데 사용되는 중요한 설정 파일입니다. 이 메시지는 주로 파일이 존재하지 않거나, 파일 경로가 잘못되었거나, CUBRID 프로세스를 실행하는 사용자에게 해당 파일에 대한 읽기 권한이 없거나, 파일 자체가 손상되었을 때 나타납니다. 이 오류가 발생하면 CUBRID는 올바른 접근 제어 정책을 적용할 수 없으므로, 서비스 시작이 실패하거나 예상치 못한 접근 문제가 발생할 수 있습니다.


.. _ERROR-1076:

**ERROR CODE: -1076, 'Could not load system parameter.'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 시작 또는 초기화 과정에서 필수적인 시스템 파라미터(예: `cubrid.conf` 파일에 정의된 설정)를 로드하는 데 실패했을 때 발생합니다. 시스템 파라미터는 데이터베이스의 동작 방식, 리소스 할당, 운영 정책 등을 구성하는 데 매우 중요합니다. 이러한 파라미터를 로드하지 못하면 시스템이 제대로 자신을 구성할 수 없어, 시작 실패나 비정상적인 동작으로 이어집니다. 이는 데이터베이스가 올바르게 기능하는 것을 방해하는 치명적인 오류입니다.


.. _ERROR-1077:

**ERROR CODE: -1077, 'The '%1$s' parameter at line %2$d in file '%3$s' : Unknown parameter'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 `cubrid.conf`와 같은 설정 파일을 읽고 파싱하는 과정에서, 시스템이 인식하거나 지원하지 않는 파라미터 이름을 발견했을 때 발생합니다. 이는 일반적으로 파라미터 이름의 오타, 현재 CUBRID 버전에서 더 이상 지원되지 않는 파라미터의 사용, 또는 설정 파일의 비정상적인 수정으로 인해 발생할 수 있습니다. 이 오류가 발생하면 CUBRID는 해당 파라미터를 처리할 수 없으므로, 서버나 관련 서비스가 정상적으로 시작되지 않거나 예상치 못한 방식으로 동작할 수 있습니다. 이는 시스템의 안정적인 운영에 필수적인 설정 파일의 무결성 문제로, 즉각적인 확인과 수정이 필요합니다.


.. _ERROR-1078:

**ERROR CODE: -1078, 'Character at offset %1$d is invalid with current codeset.'**

- 이 메시지는 CUBRID 데이터베이스에서 loaddb 명령어 실행 시 현재 설정된 문자 집합(codeset)으로 처리할 수 없는 유효하지 않은 문자를 발견했을 때 발생합니다. 이는 내부 문자열 처리 과정에서 데이터의 실제 인코딩과 시스템의 `codeset` 설정이 일치하지 않아 발생하며, 데이터 무결성 및 문자열 처리의 정확성을 보장하기 위한 보호성 오류입니다.


.. _ERROR-1087:

**ERROR CODE: -1087, 'Backup active log '%1$s' started.'**

- 이 메시지는 CUBRID 데이터베이스에서 활성 로그 파일의 백업 작업이 시작되었음을 알리는 정보 알림 메시지입니다. 이는 시스템이 로그 백업 프로세스를 시작하여 데이터의 안정성과 복구 가능성을 확보하기 위한 작업을 진행하고 있음을 의미합니다. 일반적으로 데이터베이스 관리 시스템에서 중요한 작업의 시작을 기록하는 정보성 메시지입니다.


.. _ERROR-1088:

**ERROR CODE: -1088, 'Backup active log '%1$s' finished.'**

- 이 메시지는 CUBRID 데이터베이스에서 활성 로그 파일의 백업 작업이 성공적으로 완료되었음을 알리는 성공 알림 메시지입니다. 이는 시스템이 정상적으로 로그 백업을 수행하여 데이터의 안정성과 복구 가능성을 확보했음을 의미합니다. 일반적으로 데이터베이스 관리 시스템에서 중요한 작업이 성공적으로 마무리되었을 때 기록되는 정보성 메시지입니다.


.. _ERROR-1090:

**ERROR CODE: -1090, 'Locale initialization: %1$s.'**

- 이 메시지는 CUBRID 데이터베이스가 로케일(Locale) 관련 정보를 초기화하는 과정에서 문제가 발생했을 때 나타납니다. 로케일은 언어, 국가, 문자 인코딩 규칙 등을 정의하여 데이터의 정렬, 문자열 비교, 문자 집합 변환 등을 올바르게 수행하도록 돕는 중요한 설정입니다. 이 과정에서 오류가 발생하면 시스템의 로케일 설정, CUBRID의 구성, 또는 운영체제의 로케일 지원 기능에 문제가 있음을 의미합니다. 이는 문자 집합 처리, 국제화 기능, 전반적인 데이터 무결성에 영향을 미칠 수 있는 중요한 오류입니다.


.. _ERROR-1091:

**ERROR CODE: -1091, 'Locale generation: %1$s.'**

- 이 메시지는 CUBRID 데이터베이스가 로케일(Locale) 관련 정보를 생성하거나 처리하는 과정에서 문제가 발생했을 때 나타납니다. 로케일은 언어, 국가, 문자 인코딩 규칙 등을 정의하여 데이터의 정렬, 문자열 비교, 문자 집합 변환 등을 올바르게 수행하도록 돕는 중요한 설정입니다. 이 과정에서 오류가 발생하면 시스템의 로케일 설정, CUBRID의 구성, 또는 운영체제의 로케일 지원 기능에 문제가 있음을 의미합니다. 이는 문자 집합 처리, 국제화 기능, 전반적인 데이터 무결성에 영향을 미칠 수 있는 중요한 오류입니다.


.. _ERROR-1093:

**ERROR CODE: -1093, 'The arg '%1$s' for inet_aton is not a valid string formatted ipv4 address.'**

- 이 메시지는 CUBRID 데이터베이스에서 `inet_aton` 함수를 사용하여 문자열 형식의 IP 주소를 숫자 형식으로 변환하려고 할 때, 입력된 인수가 유효한 IPv4 주소 문자열 형식이 아닐 경우 발생합니다. `inet_aton` 함수는 "a.b.c.d" 형식의 문자열 IPv4 주소를 네트워크 바이트 순서의 32비트 정수형으로 변환하는 표준 라이브러리 함수입니다. 이 메시지는 주로 잘못된 데이터 입력, 데이터 타입 불일치, 또는 비정상적인 값으로 인해 발생하며, IP 주소 변환이 필요한 작업(예: 네트워크 관련 함수, 로그 기록, 특정 데이터 처리)이 실패했음을 의미합니다. 이는 데이터 유효성 검사 실패로 인한 보호성 오류입니다.


.. _ERROR-1094:

**ERROR CODE: -1094, 'The arg '%1$lld' for inet_ntoa is not a valid number formatted ipv4 address.'**

- 이 메시지는 CUBRID 데이터베이스에서 `inet_ntoa` 함수를 사용하여 숫자 형식의 IP 주소를 문자열 형식으로 변환하려고 할 때, 입력된 인수가 유효한 IPv4 주소 형식이 아닐 경우 발생합니다. `inet_ntoa` 함수는 네트워크 바이트 순서의 32비트 정수형 IPv4 주소를 "a.b.c.d" 형식의 문자열로 변환하는 표준 라이브러리 함수입니다. 이 메시지는 주로 잘못된 데이터 입력, 데이터 타입 불일치, 또는 비정상적인 값으로 인해 발생하며, IP 주소 변환이 필요한 작업(예: 네트워크 관련 함수, 로그 기록, 특정 데이터 처리)이 실패했음을 의미합니다.


.. _ERROR-1095:

**ERROR CODE: -1095, 'User name is too long.'**

- 이 메시지는 CUBRID 데이터베이스 시스템에서 사용자 이름(User Name)을 생성하거나 변경할 때, 입력된 사용자 이름의 길이가 시스템에서 허용하는 최대 길이를 초과했을 때 발생합니다, 즉, CUBRID 서버가 사용자 관리 작업을 처리하는 과정에서, `CREATE USER` 또는 `ALTER USER`와 같은 명령을 통해 제공된 사용자 이름이 내부적으로 정의된 최대 문자열 길이를 초과하는 경우 이 오류를 반환합니다. 이는 데이터베이스의 스키마 및 시스템 메타데이터의 일관성을 유지하고, 내부 버퍼 오버플로우와 같은 잠재적인 문제를 방지하기 위한 보호성 제약입니다.


.. _ERROR-1099:

**ERROR CODE: -1099, 'DDL statement is not allowed by configuration (block_ddl_statement=yes).\n SQL Text: %1$s'**

- 이 메시지는 CUBRID 데이터베이스에서 DDL(Data Definition Language) 문을 실행하려고 할 때 발생하는 오류입니다, 즉, 시스템 파라미터 `block_ddl_statement`가 `yes`로 설정되어 있을 때, 테이블 생성, 수정, 삭제 등의 스키마 변경 작업을 차단하는 보호 기능입니다, 이는 데이터베이스 스키마의 안정성과 보안을 보장하기 위한 보호성 오류로, 의도하지 않은 스키마 변경을 방지합니다, 주로 CREATE, ALTER, DROP 등의 DDL 문이 실행될 때 `pt_is_ddl_statement` 함수로 DDL 문인지 확인하고, 시스템 파라미터 설정에 따라 실행을 허용하거나 차단합니다.


.. _ERROR-1100:

**ERROR CODE: -1100, 'Statement without WHERE clause is not allowed by configuration (block_nowhere_statement=yes).'**

- 이 메시지는 CUBRID 데이터베이스에서 WHERE 절이 없는 DELETE 문을 실행하려고 할 때 발생하는 오류입니다, 즉, 시스템 파라미터 `block_nowhere_statement`가 `yes`로 설정되어 있을 때, 조건 없이 모든 레코드를 삭제하는 위험한 SQL 문의 실행을 차단하는 보호 기능입니다, 이는 데이터 무결성과 시스템 안정성을 보장하기 위한 보호성 오류로, 실수로 전체 테이블의 데이터가 삭제되는 것을 방지합니다, 주로 DELETE 문에서 `search_cond` (검색 조건)이 NULL인 경우를 감지하여 발생하며, 시스템 파라미터 설정에 따라 실행을 허용하거나 차단합니다.


.. _ERROR-1102:

**ERROR CODE: -1102, 'Unknown system parameter or bad value.'**

- 이 메시지는 CUBRID 데이터베이스에서 시스템 파라미터를 설정하거나 변경하려고 할 때 발생하는 변경값이 없는 경우 오류입니다, 즉, 존재하지 않는 시스템 파라미터를 참조하거나, 파라미터의 값이 허용된 범위나 형식을 벗어났을 때 발생합니다, 이는 시스템 안정성과 데이터 무결성을 보장하기 위한 보호성 오류입니다.


.. _ERROR-1103:

**ERROR CODE: -1103, 'Cannot change system parameter.'**

- 이 메시지는 CUBRID 데이터베이스에서 시스템 파라미터를 변경하려고 할 때 발생하는 변경값이 없는 경우 오류입니다, 즉, 데이터베이스 실행 중에 동적으로 변경할 수 없는 파라미터를 변경하려고 시도했거나, 변경 권한이 없는 파라미터를 수정하려고 했을 때 발생합니다, 이는 시스템 안정성과 데이터 무결성을 보장하기 위한 보호성 오류입니다.


.. _ERROR-1104:

**ERROR CODE: -1104, 'Slow query (%1$d msec)\n%2$s'**

- 이 메시지는 실제로는 오류가 아니라 CUBRID 데이터베이스에서 성능 모니터링을 위한 정보성 메시지입니다, 즉, 설정된 임계값보다 오래 걸린 쿼리가 실행되었을 때 시스템이 이를 감지하여 성능 분석을 위한 상세 정보를 제공하는 것입니다, 이는 데이터베이스 성능 최적화와 문제 진단을 위한 중요한 모니터링 기능입니다, 시스템은 쿼리 실행 시간, SQL 문, 실행 계획, 버퍼 통계, 대기 시간 등의 상세 정보를 수집하여 성능 병목 지점을 파악할 수 있도록 도와줍니다.


.. _ERROR-1115:

**ERROR CODE: -1115, 'Started to update statistics (class "%1$s", oid : %2$d|%3$d|%4$d).'**

- 이 메시지는 실제로는 오류가 아니라 CUBRID 데이터베이스의 통계정보 갱신 `update statistics` 작업이 시작되었음을 알리는 정보성 메시지입니다, 즉, 테이블의 통계정보(인덱스 통계, 컬럼 분포도, 카디널리티 등)를 업데이트하는 작업이 시작되었음을 의미합니다, 이는 쿼리 최적화를 위해 필요한 통계정보를 수집하고 갱신하는 과정이 시작되었음을 나타내는 긍정적인 메시지입니다, 통계정보 갱신은 테이블의 데이터 분포, 인덱스 구조, 컬럼 값의 분포 등을 분석하여 쿼리 실행 계획을 최적화하기 위한 중요한 작업입니다.


.. _ERROR-1116:

**ERROR CODE: -1116, 'Finished to update statistics (class "%1$s", oid : %2$d|%3$d|%4$d, error code : %5$d).'**

- 이 메시지는 실제로는 오류가 아니라 CUBRID 데이터베이스의 통계정보 갱신 `update statistics`  작업이 완료되었음을 알리는 정보성 메시지입니다, 즉, 테이블의 통계정보(인덱스 통계, 컬럼 분포도, 카디널리티 등)를 업데이트하는 작업이 정상적으로 종료되었음을 의미합니다, 이는 쿼리 최적화를 위해 필요한 통계정보를 수집하고 갱신하는 과정이 완료되었음을 나타내는 긍정적인 메시지입니다,


.. _ERROR-1117:

**ERROR CODE: -1117, 'Cannot change attribute "%1$s". CUBRID cannot change an attribute as a SHARED and vice versa.'**

- 이 메시지는 CUBRID에서 이미 정의된 속성의 SHARED 특성을 변경하려고 할 때 발생합니다. SHARED 속성은 테이블 정의 시점에 결정되며, 이후에는 변경이 불가능한 특성입니다.


.. _ERROR-1123:

**ERROR CODE: -1123, 'pthread_cond_timedwait() timed out.'**

- 이 메시지는 CUBRID 데이터베이스 내부에서 사용하는 쓰레드(thread)가 critical section (예 : buffer latch 등)을 기다리는 동안 지정된 시간을 초과했을 때 발생합니다.


.. _ERROR-1130:

**ERROR CODE: -1130, 'Transaction is aborted because transaction lock counts exceeds %1$d.'**

- 이 메시지는 CUBRID 데이터베이스에서 트랜잭션이 보유한 잠금의 개수가 시스템에서 설정된 임계값을 초과했을 때 발생합니다, 즉, $CUBRID/conf/cubrid.conf 환경설정 파라미터에서 rollback_on_lock_escalation 설정값이 true이고,, lock escalation 설정된 임계값을 초과했을 때 발생합니다, 이런 상황은 일반적으로 대량의 데이터를 처리하는 쿼리에서 많은 수의 행 잠금이 필요할 때 발생될 수 있습니다.


.. _ERROR-1132:

**ERROR CODE: -1132, 'Encryption library failure: %1$s'**

- 이 메시지는 CUBRID dblink server에서 암호 관련 암복화 시 발생하는 오류입니다.


.. _ERROR-1135:

**ERROR CODE: -1135, 'A previous link of B+tree(%1$s) is corrupted.: vpid = (%2$d, %3$d)'**

- 이 메시지는 CUBRID 데이터베이스 checkdb --repair-prev-link 명령어 수행 시 b-tree 손상을 감지했다는 경고성 메세지입니다.


.. _ERROR-1136:

**ERROR CODE: -1136, 'A previous link of B+tree(%1$s) is repaired.: vpid = (%2$d, %3$d)'**

- 이 메시지는 CUBRID 데이터베이스  checkdb --repair-prev-link 명령어 수행 시 감지된 b-tree 손상을 수정했다는 경고성 메세지입니다.


.. _ERROR-1138:

**ERROR CODE: -1138, 'Handshake error (peer host %1$s): incompatible interruptibility. (client: %2$s, server: %3$s)'**

- 이 메시지는 CUBRID 데이터베이스 클라이언트와 서버 간의 네트워크 연결을 설정하는 핸드셰이크 과정에서, 양측의 '인터럽트 가능성(interruptibility)' 설정이 서로 호환되지 않을 때 발생합니다, 즉, 클라이언트와 서버가 트랜잭션 또는 쿼리 실행 중 인터럽트(예: Ctrl+C 또는 `KILL TRANSACTION`)를 처리하는 방식에 대한 약속이 일치하지 않아 통신 연결을 수립할 수 없음을 의미합니다, 이는 네트워크 통신의 일관성과 안정성을 보장하기 위한 보호성 오류입니다.


.. _ERROR-1145:

**ERROR CODE: -1145, 'To change the owner of a system class is not allowed.'**

- 이 메시지는 CUBRID 데이터베이스에서 시스템 내부적으로 중요한 역할을 하는 '시스템 테이블'의 소유자를 변경하려고 시도할 때 발생합니다, 즉, 데이터베이스의 핵심 구조와 보안을 유지하기 위해, 일반 사용자나 특정 권한을 가진 사용자라도 시스템 테이블의 소유권을 임의로 변경하는 것을 허용하지 않는 보호성 오류입니다, 이런 상황은 일반적으로 `ALTER CLASS ... CHANGE OWNER TO ...`와 같은 DDL(Data Definition Language) 문을 사용하여 시스템 테이블에 대해 소유자 변경을 시도할 때 발생합니다, 이는 데이터베이스의 안정성과 무결성을 보호하기 위한 중요한 보안 정책입니다.


.. _ERROR-1152:

**ERROR CODE: -1152, 'The owner or a member of DBA group is only allowed to kill the transaction (%1$d).'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 트랜잭션을 강제로 종료(kill)하려고 할 때, 해당 작업을 수행할 권한이 없는 사용자가 시도했을 때 발생합니다, 즉, 트랜잭션 종료는 데이터베이스의 안정성과 보안을 위해 제한된 권한을 가진 사용자만 수행할 수 있도록 설계되어 있습니다, 이런 상황은 일반적으로 일반 사용자가 다른 사용자의 트랜잭션을 종료하려고 시도하거나, 시스템 트랜잭션을 종료하려고 할 때 발생합니다, 이는 데이터베이스의 보안 정책과 무결성을 보호하기 위한 보호성 오류입니다.


.. _ERROR-1158:

**ERROR CODE: -1158, 'Serializable conflict due to concurrent updates'**

- 이 메시지는 CUBRID 데이터베이스에서 `SERIALIZABLE` 격리 수준으로 실행되는 트랜잭션이 다른 동시 트랜잭션의 업데이트 작업과 충돌하여 직렬화 가능성(serializability)이 깨졌을 때 발생합니다. `SERIALIZABLE` 격리 수준은 동시 실행되는 트랜잭션들이 마치 순차적으로 실행된 것처럼 동일한 결과를 보장하는 가장 엄격한 수준입니다, 이 메시지는 현재 트랜잭션이 접근하려는 데이터가 다른 트랜잭션에 의해 변경되었거나, 팬텀 읽기(phantom read)와 같은 직렬화 불가능한 현상이 감지되었을 때 발생하며, 데이터 일관성을 유지하기 위해 충돌하는 트랜잭션 중 하나를 롤백시킵니다. 이는 `SERIALIZABLE` 격리 수준의 엄격한 특성으로 인해 발생하는 정상적인 동작입니다.


.. _ERROR-1159:

**ERROR CODE: -1159, 'Timezone compile error: %s'**

- 이 메시지는 CUBRID 데이터베이스가 시간대(timezone) 데이터를 내부적으로 처리하거나 "컴파일"하는 과정에서 문제가 발생했음을 나타냅니다, 시간대 규칙 및 데이터를 파싱하고 내부적으로 사용할 수 있는 형태로 변환하는 로직을 포함합니다. 이 메시지는 주로 시스템에 설치된 시간대 데이터 파일이 손상되었거나, 형식이 잘못되었거나, CUBRID가 해당 데이터를 해석하는 데 실패했을 때 발생합니다. 이는 데이터베이스가 시간대 관련 연산을 정확하게 수행할 수 없게 만들며, 날짜/시간 데이터의 유효성 및 일관성에 영향을 미칠 수 있는 중요한 문제입니다.


.. _ERROR-1160:

**ERROR CODE: -1160, 'Timezone library loader error: %s'**

- 이 메시지는 CUBRID 데이터베이스가 시간대(timezone) 관련 연산을 수행하기 위해 필요한 시간대 라이브러리 또는 데이터를 로드하는 데 실패했음을 나타냅니다. CUBRID는 날짜/시간 데이터의 정확한 처리, 특히 `DATETIME WITH TIME ZONE` 타입이나 `TIMESTAMP` 값의 시간대 변환을 위해 시스템의 시간대 정보에 의존합니다, 이 메시지는 주로 시간대 데이터 파일이 없거나, 손상되었거나, 접근 권한이 없거나, 또는 시간대 관련 환경 설정이 잘못되었을 때 발생합니다. 시간대 라이브러리 로드 실패는 데이터베이스가 시간대 관련 기능을 정상적으로 수행할 수 없게 만들며, 이는 데이터베이스 시작 실패나 날짜/시간 관련 쿼리 및 연산의 오류로 이어질 수 있는 문제입니다.


.. _ERROR-1161:

**ERROR CODE: -1161, 'Timezone conversion error.'**

- 이 메시지는 CUBRID 데이터베이스에서 날짜/시간 값을 한 시간대에서 다른 시간대로 변환하는 과정에서 내부적으로 예상치 못한 문제가 발생했음을 나타냅니다. CUBRID는 내부적으로 UTC(협정 세계시)를 기준으로 시간을 관리하며, 사용자 세션의 시간대 설정에 따라 표시되는 시간을 변환합니다. 이 메시지는 주로 다음과 같은 상황에서 발생할 수 있습니다:


.. _ERROR-1162:

**ERROR CODE: -1162, 'Invalid or missing timezone.'**

- 이 메시지는 CUBRID 데이터베이스에서 시간대(timezone) 정보가 유효하지 않거나, 날짜/시간 관련 연산을 수행하는 데 필요한 시간대 정보가 누락되었을 때 발생합니다. CUBRID는 날짜/시간 데이터의 정확한 처리와 변환을 위해 유효한 시간대 정보가 필수적입니다.


.. _ERROR-1163:

**ERROR CODE: -1163, 'Invalid or missing daylight saving time.'**

- 이 메시지는 CUBRID 데이터베이스에서 날짜/시간 값을 처리할 때, 일광 절약 시간(Daylight Saving Time, DST) 정보가 유효하지 않거나 필요한 DST 정보가 누락되었을 때 발생합니다. CUBRID는 시간대 변환 및 날짜/시간 계산의 정확성을 위해 DST 규칙을 엄격하게 적용합니다. 이 메시지는 주로 다음과 같은 상황에서 발생할 수 있습니다:


.. _ERROR-1165:

**ERROR CODE: -1165, 'Invalid combination of date, time, timezone and daylight specifier.'**

- 이 메시지는 CUBRID 데이터베이스에서 날짜, 시간, 시간대(timezone) 및 일광 절약 시간(Daylight Saving Time, DST) 지정자들 간의 조합이 논리적으로 유효하지 않을 때 발생합니다, CUBRID는 시간대 관련 연산에서 입력된 날짜/시간과 시간대 정보, 그리고 DST 규칙이 서로 일치하는지 검증합니다. 예를 들어, 특정 날짜와 시간이 주어진 시간대에서 DST 규칙과 충돌하거나, 존재하지 않는 시간 조합(예: DST 전환 구간에서 존재하지 않는 시간)을 지정했을 때 이 오류가 발생합니다. 이는 데이터의 정확성과 일관성을 보장하기 위한 검증 메커니즘으로, 잘못된 시간대 변환이나 계산을 방지합니다.


.. _ERROR-1166:

**ERROR CODE: -1166, 'The specified combination of date and time do not exist (during daylight saving interval).'**

- 이 메시지는 CUBRID 데이터베이스에서 사용자가 지정한 날짜와 시간 조합이 특정 시간대(timezone)의 일광 절약 시간(Daylight Saving Time, DST) 전환 구간으로 인해 실제로는 존재하지 않는 시간일 때 발생합니다. 예를 들어, DST가 시작되어 시계가 새벽 2시에서 새벽 3시로 한 시간 앞으로 점프하는 경우, 새벽 2시 0분부터 2시 59분까지의 시간은 해당 날짜에 존재하지 않습니다. 사용자가 이러한 존재하지 않는 시간을 입력하거나 계산하려고 시도할 때, CUBRID는 데이터의 유효성을 보장하기 위해 이 오류를 발생시킵니다. 이는 주로 날짜/시간 데이터 삽입, 업데이트, 또는 날짜/시간 함수 사용 시 발생할 수 있습니다.


.. _ERROR-1167:

**ERROR CODE: -1167, 'Comment string cannot have more than 1024 bytes.'**

- 이 메시지는 CUBRID 데이터베이스에서 사용자나 객체에 대한 코멘트(주석) 문자열의 길이가 허용된 최대 크기인 1024 바이트를 초과했을 때 발생하는 오류를 나타냅니다. 코멘트는 데이터베이스 사용자 객체(테이블, 컬럼, 사용자 등)에 대한 설명이나 메모를 저장하는 데 사용되며, 시스템의 안정성과 성능을 위해 길이 제한이 있습니다, 즉, 이 메시지는 CUBRID 데이터베이스의 사용자 인증 시스템에서 사용자 코멘트를 설정할 때 발생합니다.


.. _ERROR-1168:

**ERROR CODE: -1168, 'Unable to update statistics on table (%1$s) because a lock is not immediately granted.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 테이블의 통계(statistics)를 업데이트하려고 시도했지만, 필요한 잠금(lock)을 즉시 획득하지 못하여 작업이 실패했음을 나타냅니다. 통계 업데이트는 쿼리 최적화기가 최적의 실행 계획을 수립하는 데 필수적인 정보를 제공합니다, 이 메시지는 주로 통계 업데이트 대상 테이블에 다른 트랜잭션이 배타적인 잠금을 보유하고 있거나, 잠금 경합이 심하여 통계 업데이트 작업이 필요한 잠금을 제때 얻지 못할 때 발생합니다. 이는 통계 업데이트 작업이 지연되거나 실패하여, 이후 실행되는 쿼리의 성능에 부정적인 영향을 미칠 수 있음을 의미합니다.


.. _ERROR-1177:

**ERROR CODE: -1177, 'Incompatible timezone data: %1$s has different checksum from %2$s.'**

- 이 메시지는 CUBRID 데이터베이스에서 클라이언트와 서버 간의 시간대(timezone) 데이터가 서로 호환되지 않음을 나타내는 오류입니다.  CUBRID는 시간대 관련 연산의 정확성을 보장하기 위해 클라이언트와 서버가 동일한 시간대 데이터를 사용해야 합니다. 이 메시지는 클라이언트와 서버의 시간대 데이터 체크섬이 다를 때 발생하며, 이는 서로 다른 버전의 시간대 데이터를 사용하고 있거나, 시간대 데이터가 손상되었음을 의미합니다. 시간대 데이터의 불일치는 날짜/시간 변환 함수의 부정확한 결과를 초래할 수 있으므로, 데이터베이스는 이러한 상황에서 오류를 발생시켜 데이터 무결성을 보호합니다.


.. _ERROR-1179:

**ERROR CODE: -1179, 'Found not vacuumed entries in heap.'**

- 이 메시지는 CUBRID checkdb 명령어 수행 시 발생할 수 있는 오류로 힙 파일(테이블의 실제 데이터가 저장되는 공간) 내에 진공(vacuum) 처리되지 않은 엔트리(레코드)가 존재함을 나타내는 정보성 또는 경고성 메시지입니다. 데이터베이스에서 레코드가 삭제되거나 업데이트될 때, 해당 공간은 즉시 재사용되지 않고 "죽은(dead)" 공간으로 표시됩니다. 진공 작업은 이러한 죽은 공간을 회수하고 재사용 가능하게 정리하는 역할을 합니다, 이 메시지는 진공(vacuum) 작업이 필요하거나, 특정 검사 과정에서 아직 정리되지 않은 공간이 발견되었음을 알려줍니다. 이는 데이터베이스의 일관성 문제라기보다는 공간 효율성 및 성능 유지보수와 관련된 정보입니다.


.. _ERROR-1180:

**ERROR CODE: -1180, 'Found not vacuumed OIDs. INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d).'**

- 이 메시지는 CUBRID checkdb 명령어 수행 시 발생할 수 있는 오류로 인덱스에 진공(vacuum) 처리되지 않은(즉, 삭제되었지만 아직 공간이 회수되지 않은) 객체 ID(OID)가 남아 있음을 나타내는 정보성 또는 경고성 메시지입니다. 데이터베이스에서 레코드가 삭제되면 해당 레코드의 OID는 즉시 재사용되지 않고, 진공 작업을 통해 비활성화되거나 재사용 가능한 상태로 정리됩니다, 이 메시지는 특정 인덱스에 이러한 "정리되지 않은" OID가 존재함을 알려주며, 이는 인덱스 공간의 비효율적인 사용이나 잠재적인 성능 저하를 의미할 수 있습니다. 일반적으로는 데이터베이스의 일관성 문제라기보다는 유지보수와 관련된 정보입니다.


.. _ERROR-1186:

**ERROR CODE: -1186, 'checksumdb: %1$s (error code: %2$d).'**

- 이 메시지는 CUBRID 데이터베이스의 `checksumdb` 유틸리티를 실행하는 동안 내부적으로 문제가 발생했음을 나타냅니다. `checksumdb`는 데이터베이스 파일의 체크섬을 검사하여 데이터 손상 여부를 확인하는 도구입니다, 이 메시지는 체크섬 검사 과정에서 파일 접근 문제, 데이터 불일치, 내부 로직 오류 등 다양한 원인으로 인해 유틸리티가 정상적으로 작업을 완료하지 못했음을 의미합니다.


.. _ERROR-1187:

**ERROR CODE: -1187, 'Invalid access on page %1$d,%2$d which was part of a deleted heap.'**

- 이 메시지는 CUBRID 데이터베이스에서 이미 삭제(해제)된 힙 파일(테이블의 실제 데이터 저장소)에 속했던 페이지에 접근하려고 할 때 발생합니다. , 즉, 논리적으로나 물리적으로 삭제된 테이블(혹은 오브젝트)의 데이터 페이지를 참조하거나 읽으려 할 때 시스템이 이를 감지하여 차단하는 보호성 오류입니다, 이런 상황은 일반적으로 테이블 DROP, TRUNCATE, 또는 내부적으로 힙 파일이 삭제된 후, 해당 페이지에 대한 잘못된 참조가 남아있을 때 발생합니다.


.. _ERROR-1188:

**ERROR CODE: -1188, 'Dropping an active user '%1$s' is not allowed.'**

- 이 메시지는 CUBRID 데이터베이스에서 `DROP USER` 수행시 트랜잭션 수행 중인 USER를 DROP 할 수 없을 때 발생합니다.


.. _ERROR-1189:

**ERROR CODE: -1189, 'The specified combination of date and time do not exist (during transition of timezone offset rules).'**

- 이 메시지는 CUBRID 데이터베이스에서 날짜 및 시간 값을 처리할 때, 특정 시간대(timezone)의 오프셋 규칙이 변경되는 전환 기간(예: 일광 절약 시간제(DST) 시작 또는 종료 시점)에 존재하지 않는 날짜와 시간 조합이 입력되었을 때 발생합니다. 예를 들어, DST가 시작되어 시계가 새벽 2시에서 새벽 3시로 건너뛰는 경우, 새벽 2시 30분과 같은 시간은 해당 날짜에 물리적으로 존재하지 않게 됩니다. 이 메시지는 이러한 비존재 시간을 감지하여 데이터의 유효성을 보호하기 위해 발생합니다.


.. _ERROR-1190:

**ERROR CODE: -1190, 'Stand-alone vacuum execution is started.'**

- 이 메시지는 CUBRID 데이터베이스에서 독립 실행형(stand-alone) 진공(vacuum) 작업이 시작되었음을 알리는 정보성 메시지입니다. 이는 오류가 아니라, 데이터베이스의 공간을 회수하고 성능을 최적화하는 진공 작업이 정상적으로 시작되었음을 사용자에게 알려주는 로그 메시지입니다.


.. _ERROR-1191:

**ERROR CODE: -1191, 'Stand-alone vacuum execution is ended.'**

- 이 메시지는 CUBRID 데이터베이스에서 독립 실행형(stand-alone) 진공(vacuum) 작업이 성공적으로 완료되었음을 알리는 정보성 메시지입니다. 이는 오류가 아니라, 데이터베이스의 공간을 회수하고 성능을 최적화하는 진공 작업이 정상적으로 종료되었음을 사용자에게 알려주는 로그 메시지입니다.


.. _ERROR-1194:

**ERROR CODE: -1194, 'Unable to check file %1$d|%2$d because lock on class %3$d|%4$d|%5$d is not immediately granted.'**

- 이 메시지는 CUBRID checkdb 명령어에서 object (heap, btree 등) 검사 중 해당 object의 lock 획득 오류로 특정 파일(일반적으로 데이터 페이지 파일)을 검사하거나 접근하려 할 때, 해당 파일이 속한 테이블(테이블)에 대한 잠금(lock)을 즉시 획득하지 못하여 발생하는 문제입니다. 이는 주로 동시성 제어와 관련된 상황에서 발생하며, 다른 트랜잭션이 해당 테이블에 대한 배타적 잠금을 보유하고 있거나, 잠금 요청이 너무 많아 즉시 처리되지 못할 때 나타날 수 있습니다. 시스템이 파일의 무결성을 확인하거나 안전하게 접근하기 위해 잠금이 필요하지만, 이를 얻지 못해 작업을 진행할 수 없음을 의미합니다.


.. _ERROR-1241:

**ERROR CODE: -1241, 'XASL tree needs recompile.'**

- 이 메시지는 CUBRID 데이터베이스 시스템의 쿼리 처리 과정에서 XASL(eXtensible Abstract Syntax Language) 트리가 재컴파일되어야 함을 나타내는 내부적인 요청 또는 오류입니다, XASL은 CUBRID의 내부 쿼리 표현 형식으로, SQL 쿼리가 파싱되고 최적화된 후 실행 계획으로 변환되는 중간 단계입니다. 이 실행 계획은 성능 향상을 위해 캐시될 수 있습니다, 이 메시지는 캐시된 XASL 트리가 더 이상 유효하지 않거나, 최적화된 실행 계획을 다시 생성해야 하는 상황이 발생했음을 의미합니다. 이는 주로 테이블 스키마 변경, 통계 정보 업데이트, 또는 시스템 내부 상태 변화와 같은 이유로 발생합니다, 일반적으로 사용자에게 직접적으로 노출되는 심각한 오류라기보다는, CUBRID 내부에서 쿼리 처리의 일관성과 최적화를 유지하기 위한 메커니즘의 일부로 발생합니다.


.. _ERROR-1247:

**ERROR CODE: -1247, 'The time(%1$s) specified must be after the time(%2$s) of the specified backup.'**

- 이 메시지는 CUBRID 시스템이 데이터베이스 복구 작업을 수행하는 동안, 사용자가 지정한 복구 시점이 사용하려는 백업의 생성 시점 보다 이전이거나 같을 때 발생하는 오류입니다, 데이터베이스 복구는 특정 백업 시점부터 시작하여 이후의 트랜잭션 로그를 적용하여 데이터베이스를 특정 시점(Point-In-Time Recovery)으로 되돌리거나 최신 상태로 복원하는 과정입니다, 복구 시점은 항상 백업 시점보다 미래여야 합니다. 백업 시점 이전으로 복구하는 것은 논리적으로 불가능하며, 백업 시점과 동일한 시점으로 복구하는 것은 백업 자체를 사용하는 것과 같으므로, 일반적으로 백업 시점 이후의 특정 시점으로 복구할 때 이 오류가 발생할 수 있습니다. 


.. _ERROR-1248:

**ERROR CODE: -1248, 'Invalid Key file : %1$s'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption)에 사용되는 키 파일을 읽거나 처리하는 과정에서 해당 키 파일이 유효하지 않거나 손상되었음을 감지했을 때 발생하는 오류입니다.


.. _ERROR-1249:

**ERROR CODE: -1249, 'Cannot find the key (index: %1$d).'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하는 과정에서 특정 인덱스에 해당하는 마스터 키를 찾으려고 시도했지만, 해당 키를 찾을 수 없어 실패했음을 나타내는 오류입니다.


.. _ERROR-1250:

**ERROR CODE: -1250, 'The master key set on the database and what is given don't match. The key index set on the database: %1$d'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하는 과정에서, 데이터베이스에 설정된 마스터 키 정보(특히 키 인덱스)와 현재 시스템에 제공된 마스터 키 정보가 일치하지 않을 때 발생하는 오류입니다, TDE는 데이터베이스의 데이터를 암호화하여 보안을 강화하는 기능이며, 마스터 키는 데이터 암호화 키(Data Encryption Key, DEK)를 암호화하는 데 사용되는 최상위 키입니다. 이 마스터 키가 일치하지 않으면 암호화된 데이터에 접근할 수 없습니다, 이 메시지는 주로 TDE 키 파일의 내용이 변경되었거나, 잘못된 키 파일이 사용되었거나, 데이터베이스의 메타데이터에 기록된 키 인덱스와 현재 로드된 키의 인덱스가 다를 때 발생합니다.


.. _ERROR-1251:

**ERROR CODE: -1251, 'Error while TDE-encrypting.'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하여 데이터를 암호화하는 과정에서 예상치 못한 오류가 발생하여 암호화 작업에 실패했음을 나타내는 오류입니다, TDE는 데이터베이스의 데이터를 암호화하여 보안을 강화하는 기능으로, 데이터가 디스크에 기록되기 전에 암호화되고 읽힐 때 복호화됩니다. 이 메시지는 이러한 암호화 과정에서 문제가 발생했음을 의미합니다, 암호화 실패는 데이터가 제대로 저장되지 않거나, 데이터 무결성이 손상되거나, 데이터베이스의 정상적인 작동이 방해될 수 있음을 시사합니다.


.. _ERROR-1252:

**ERROR CODE: -1252, 'Error while TDE-decrypting.'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하여 암호화된 데이터를 복호화하는 과정에서 예상치 못한 오류가 발생하여 복호화 작업에 실패했음을 나타내는 오류입니다, TDE는 데이터베이스의 데이터를 암호화하여 보안을 강화하는 기능으로, 데이터가 디스크에 기록되기 전에 암호화되고 읽힐 때 복호화됩니다. 이 메시지는 이러한 복호화 과정에서 문제가 발생했음을 의미합니다, 복호화 실패는 암호화된 데이터에 접근할 수 없거나, 데이터 무결성이 손상되거나, 데이터베이스의 정상적인 작동이 방해될 수 있음을 시사합니다.


.. _ERROR-1253:

**ERROR CODE: -1253, 'TDE Module is not loaded.'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하려고 할 때, TDE 모듈이 제대로 로드되지 않았거나 초기화되지 않았음을 나타내는 오류입니다, 즉, TDE KEY 파일 없이 데이터베이스가 구동되었거나 잘못된 TDE KEY 파일을 가지고 구동되어 TDE 모듈이 제대로 동작되지 않는 상태에서, TDE 관련 작업을 수행하는 경우 발생합니다.


.. _ERROR-1254:

**ERROR CODE: -1254, 'Cannot create a key.'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하여 암호화 키를 생성하는 과정에서 예상치 못한 오류가 발생하여 키 생성 작업에 실패했음을 나타내는 오류입니다, TDE는 데이터베이스의 데이터를 암호화하여 보안을 강화하는 기능으로, 이 과정에서 마스터 키나 데이터 암호화 키(DEK) 등의 키가 필요합니다. 이 메시지는 이러한 키 생성 과정에서 문제가 발생했음을 의미합니다, 키 생성 실패는 TDE 기능이 정상적으로 작동하지 않거나, 데이터 보안에 문제가 발생할 수 있음을 시사합니다.


.. _ERROR-1255:

**ERROR CODE: -1255, 'Cannot load TDE module. You can't use TDE feature.\n\'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하려고 할 때, 데이터베이스 시작 시 TDE KEY 파일이 없어서 TDE 기능을 사용할 수 없다는 정보를 알려주는 메세지입니다.


.. _ERROR-1256:

**ERROR CODE: -1256, 'Cannot copy key file (_keys). You can't use TDE feature on the copied database.'**

- 이 메시지는 CUBRID 데이터베이스를 copydb 명령어로 복사하는 과정에서 TDE(Transparent Data Encryption)에 사용되는 키 파일(_keys)을 복사하는데 실패했음을 나타내는 오류입니다,


.. _ERROR-1258:

**ERROR CODE: -1258, 'Cannot find the key set on the database from %1$s. The key file from backup volume is going to be used.'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하는 데이터베이스를 시작하거나 접근할 때, 데이터베이스에 설정된 마스터 키 파일(_keys 파일)을 지정된 경로에서 찾을 수 없음을 나타냅니다, 하지만, 시스템은 다행히 백업 볼륨 내에서 해당 키 파일을 발견했으며, 이 백업 키 파일을 사용하여 데이터베이스를 계속 운영할 것임을 알리는 경고성 메시지입니다.


.. _ERROR-1259:

**ERROR CODE: -1259, 'Rename the key file from "%1$s" to "%2$s".'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하는 데이터베이스의 키 파일을 복구하거나 복원하는 과정에서, 이전 키 파일로 변경한다는 경고성 메시지입니다.


.. _ERROR-1260:

**ERROR CODE: -1260, 'Copy the key file "%1$s" to "%2$s".'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하는 데이터베이스를 복원하는 과정에서, 백업에 있던 키 파일로 복원했다는 경고성 메세지입니다.


.. _ERROR-1261:

**ERROR CODE: -1261, 'The first key (key index: %1$d, created time: %2$s) on the restored key file has been set.'**

- 이 메시지는 CUBRID 데이터베이스 복원 과정에서 TDE(Transparent Data Encryption) 키 파일이 성공적으로 복원되었고, 해당 키 파일 내의 첫 번째 마스터 키(지정된 인덱스와 생성 시간을 가짐)가 데이터베이스에 설정되었음을 나타내는 정보성 또는 경고성 메시지입니다, TDE는 데이터베이스의 데이터를 암호화하여 보안을 강화하는 기능이며, 마스터 키는 데이터 암호화 키(Data Encryption Key, DEK)를 암호화하는 데 사용되는 최상위 키입니다. 이 메시지는 복원된 데이터베이스가 TDE 기능을 사용하여 암호화된 데이터에 접근할 준비가 되었음을 의미합니다, 일반적으로 긍정적인 결과이지만, 사용자가 의도한 키가 맞는지 확인하는 것이 중요합니다.


.. _ERROR-1262:

**ERROR CODE: -1262, 'The key file is full.'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하는 과정에서, TDE KEY를 새롭게 추가할때 최대 키 저장 개수를 넘을떄 발생합니다.


.. _ERROR-1263:

**ERROR CODE: -1263, 'It fails to TDE-encrypt the log page (pageid: %1$lld). It won't be tried to encrypt this page any longer.'**

- 이 메시지는 CUBRID 시스템이 TDE(Transparent Data Encryption) 기능을 사용하여 데이터베이스의 로그 페이지를 암호화하는 과정에서 예상치 못한 오류가 발생하여 암호화 작업에 실패했음을 나타내는 오류입니다, 로그 페이지는 데이터베이스의 모든 변경 사항을 기록하는 매우 중요한 부분이며, TDE가 활성화된 환경에서는 이 로그 페이지도 보안을 위해 암호화되어야 합니다.


.. _ERROR-1286:

**ERROR CODE: -1286, 'Cannot find the log lsa at the time to extract. (input time : %1$s).'**

- 이 메시지는 CUBRID 시스템이 CDC(Change Data Capture) 기능을 사용하여 특정 시간의 로그 LSA(Log Sequence Address)를 찾으려고 할 때, 해당 시간에 해당하는 로그 LSA를 찾을 수 없음을 나타내는 오류입니다, CDC는 데이터베이스의 변경 사항을 실시간으로 캡처하고 추적하는 기능이며, LSA는 로그 시퀀스 주소로 데이터베이스의 특정 시점을 식별하는 데 사용됩니다.


.. _ERROR-1287:

**ERROR CODE: -1287, 'Timed out attempting to extract log info. (elapsed time : %1$d sec, extraction timeout : %2$d sec)'**

- 이 메시지는 CUBRID 시스템이 CDC(Change Data Capture) 기능을 사용하여 로그 정보를 추출하는 과정에서 설정된 시간 초과 값을 초과하여 작업이 중단되었음을 나타내는 오류입니다.


.. _ERROR-1289:

**ERROR CODE: -1289, 'Skip producing log info for invalid transaction. (trid : %1$d)'**

- 이 메시지는 CUBRID 시스템이 CDC(Change Data Capture) 기능을 사용하여 로그 정보를 처리하는 과정에서, 유효하지 않은 트랜잭션을 발견하여 해당 트랜잭션에 대한 로그 정보 생성을 건너뛴다는 의미입니다.


.. _ERROR-1290:

**ERROR CODE: -1290, 'Invalid log lsa (%1$lld|%2$d) to extract log info.'**

- 이 메시지는 CUBRID 시스템이 CDC(Change Data Capture) 기능을 사용하여 로그 정보를 추출하려고 할 때, 유효하지 않은 로그 LSA(Log Sequence Address)를 만났음을 나타내는 오류입니다, CDC는 데이터베이스의 변경 사항을 실시간으로 캡처하고 추적하는 기능이며, LSA는 로그 시퀀스 주소로 데이터베이스의 특정 시점을 식별하는 데 사용됩니다, 이 메시지는 주로 지정된 LSA가 데이터베이스의 로그 범위를 벗어났거나, 로그 파일이 손상되었거나, LSA 형식이 잘못되었을 때 발생합니다.


.. _ERROR-1291:

**ERROR CODE: -1291, 'Cannot connect to server for extracting log info. Try again with setting 'supplemental_log' parameter.'**

- 이 메시지는 CUBRID 시스템이 CDC(Change Data Capture) 기능을 사용하여 로그 정보를 추출하려고 할 때, 서버에 연결할 수 없음을 나타내는 오류입니다, CDC는 데이터베이스의 변경 사항을 실시간으로 캡처하고 추적하는 기능이며, 이 기능을 사용하려면 서버와의 연결이 필요합니다.


.. _ERROR-1292:

**ERROR CODE: -1292, 'Cannot find the log lsa at the time (%1$s). Log lsa is adjusted to the time (%2$s).'**

- 이 메시지는 CUBRID 시스템이 CDC(Change Data Capture) 기능을 사용하여 특정 시간의 로그 LSA(Log Sequence Address)를 찾으려고 할 때, 해당 시간에 정확히 일치하는 LSA를 찾을 수 없어서 가장 가까운 유효한 시간의 LSA로 조정했음을 나타내는 경고성 메시지입니다, CDC는 데이터베이스의 변경 사항을 실시간으로 캡처하고 추적하는 기능이며, LSA는 로그 시퀀스 주소로 데이터베이스의 특정 시점을 식별하는 데 사용됩니다, 이 메시지는 시스템이 요청된 시간에 정확히 일치하는 로그를 찾지 못했지만, 가장 가까운 유효한 시간의 로그를 찾아서 작업을 계속할 수 있도록 조정했음을 의미합니다.


.. _ERROR-1293:

**ERROR CODE: -1293, 'Cannot get log page while producing log infos. Log lsa to generate log info indicates NULL.'**

- 이 메시지는 CUBRID 시스템이 CDC(Change Data Capture) 기능을 사용하여 로그 정보를 생성하는 과정에서, 로그 페이지를 가져오려고 할 때 로그 LSA(Log Sequence Address)가 NULL 값을 가지고 있어서 작업을 수행할 수 없음을 나타내는 오류입니다, 


.. _ERROR-1294:

**ERROR CODE: -1294, 'Log info has been generated. (type : "%1$s").'**

- 이 메시지는 CUBRID 시스템이 CDC(Change Data Capture) 기능을 사용하여 로그 정보를 성공적으로 생성했음을 나타내는 정보성 메시지입니다, CDC는 데이터베이스의 변경 사항을 실시간으로 캡처하고 추적하는 기능이며, 로그 정보는 이러한 변경 사항을 기록한 데이터입니다, 이 메시지는 일반적으로 성공적인 작업을 나타내며, 생성된 로그 정보의 유형을 포함하여 어떤 종류의 로그 정보가 생성되었는지를 알려줍니다.


.. _ERROR-1296:

**ERROR CODE: -1296, 'Log recovery: ANALYSIS Phase is started.'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 비정상적으로 종료된 후 재시작되거나, 특정 복구 작업이 시작될 때, 트랜잭션 로그 복구 과정의 첫 번째 단계인 '분석(ANALYSIS) 단계'가 시작되었음을 알리는 정보성 메시지입니다, 로그 복구는 데이터베이스의 일관성과 무결성을 보장하기 위해 필수적인 과정으로, 시스템 충돌 등으로 인해 커밋되지 않은 트랜잭션을 롤백하고, 커밋되었지만 디스크에 반영되지 않은 변경 사항을 재반영하는 역할을 합니다, 분석 단계'에서는 로그 파일을 스캔하여 마지막 체크포인트 이후의 모든 트랜잭션 활동을 파악하고, 어떤 트랜잭션이 커밋되었고 어떤 트랜잭션이 롤백되어야 하는지 등의 정보를 수집합니다.


.. _ERROR-1297:

**ERROR CODE: -1297, 'Log recovery: REDO Phase is started. Log pages to redo: %1$lld, Log records to redo: %2$lld.'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 비정상적으로 종료된 후 재시작되거나, 특정 복구 작업이 시작될 때, 트랜잭션 로그 복구 과정의 두 번째 단계인 '재실행(REDO) 단계'가 시작되었음을 알리는 정보성 메시지입니다, 로그 복구는 데이터베이스의 일관성과 무결성을 보장하기 위해 필수적인 과정으로, 시스템 충돌 등으로 인해 커밋되었지만 디스크에 반영되지 않은 변경 사항들을 로그에 기록된 순서대로 데이터베이스에 재반영하는 역할을 합니, 재실행 단계'에서는 이전 '분석 단계'에서 파악된 정보를 바탕으로, 커밋된 트랜잭션의 변경 사항들을 데이터 파일에 적용합니다, 이 메시지는 재실행할 로그 페이지의 수와 로그 레코드의 수를 함께 제공하여 복구 작업의 규모를 알려줍니다.


.. _ERROR-1298:

**ERROR CODE: -1298, 'Log recovery: UNDO Phase is started. Log pages to undo: %1$lld, transactions to undo: %2$d.'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 비정상적으로 종료된 후 재시작되거나, 특정 복구 작업이 시작될 때, 트랜잭션 로그 복구 과정의 세 번째 단계인 '취소(UNDO) 단계'가 시작되었음을 알리는 정보성 메시지입니다, 로그 복구는 데이터베이스의 일관성과 무결성을 보장하기 위해 필수적인 과정으로, 시스템 충돌 등으로 인해 커밋되지 않은 트랜잭션을 롤백하는 역할을 합니다, 취소 단계'에서는 이전 '분석 단계'에서 파악된 정보를 바탕으로, 커밋되지 않은 트랜잭션의 변경 사항들을 되돌립니다, 메시지는 취소할 로그 페이지의 수와 취소할 트랜잭션의 수를 함께 제공하여 복구 작업의 규모를 알려줍니다.


.. _ERROR-1299:

**ERROR CODE: -1299, 'Log recovery: %1$s Phase is being finished up.'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 비정상적으로 종료된 후 재시작되거나, 특정 복구 작업이 시작될 때, 트랜잭션 로그 복구 과정의 특정 단계(예: ANALYSIS, REDO, UNDO)가 성공적으로 마무리되고 있음을 알리는 정보성 메시지입니다, 로그 복구는 데이터베이스의 일관성과 무결성을 보장하기 위해 필수적인 과정으로, 시스템 충돌 등으로 인해 커밋되지 않은 트랜잭션을 롤백하고, 커밋되었지만 디스크에 반영되지 않은 변경 사항을 재반영하는 역할을 합니다, 이 메시지는 각 복구 단계가 성공적으로 진행되고 있음을 나타내며, 전체 복구 프로세스가 순조롭게 진행되고 있음을 시사합니다.


.. _ERROR-1300:

**ERROR CODE: -1300, 'Log recovery: %1$s Phase is finished.'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 비정상적으로 종료된 후 재시작되거나, 특정 복구 작업이 시작될 때, 트랜잭션 로그 복구 과정의 특정 단계(예: ANALYSIS, REDO, UNDO)가 성공적으로 완료되었음을 알리는 정보성 메시지입니다, 로그 복구는 데이터베이스의 일관성과 무결성을 보장하기 위해 필수적인 과정으로, 시스템 충돌 등으로 인해 커밋되지 않은 트랜잭션을 롤백하고, 커밋되었지만 디스크에 반영되지 않은 변경 사항을 재반영하는 역할을 합니다, 이 메시지는 각 복구 단계가 성공적으로 완료되었음을 나타내며, 전체 복구 프로세스가 순조롭게 진행되고 있음을 시사합니다.


.. _ERROR-1301:

**ERROR CODE: -1301, 'Log recovery: %1$s progress: (%2$lld/%3$lld), %4$.2f percent, elapsed time: %5$.2f (s), estimated remaining time: %6$.2f (s).'**

- 이 메시지는 CUBRID 데이터베이스 시스템이 비정상적으로 종료된 후 재시작되거나, 특정 복구 작업이 시작될 때, 트랜잭션 로그 복구 과정의 특정 단계(ANALYSIS, REDO, UNDO)의 현재 진행 상황을 실시간으로 알려주는 정보성 메시지입니다, 로그 복구는 데이터베이스의 일관성과 무결성을 보장하기 위해 필수적인 과정으로, 시스템 충돌 등으로 인해 커밋되지 않은 트랜잭션을 롤백하고, 커밋되었지만 디스크에 반영되지 않은 변경 사항을 재반영하는 역할을 합니다, 이 메시지는 복구 작업의 현재 상태, 완료된 작업량, 전체 작업량, 진행률, 경과 시간 및 예상 남은 시간을 상세하게 제공하여 관리자가 복구 진행 상황을 모니터링할 수 있도록 돕습니다.


.. _ERROR-1302:

**ERROR CODE: -1302, 'dblink %1$s'**

- 이 메시지는 CUBRID의 DBLink 기능에서 발생하는 일반적인(포괄적) 에러 메시지로, 다양한 하위 오류 상황에서 사용됩니다, 이는 parser 등에서 dblink 관련 오류 발생시 사용하는 오류코드로 실제 오류의 원인은 여러 메시지가 치환되어 전달되는 상세 메시지에 따라 달라지며, 네트워크 통신, 인증, 파라미터, 쿼리 실행 등 DBLink 관련 모든 계층에서 발생할 수 있습니다.


.. _ERROR-1303:

**ERROR CODE: -1303, 'dblink invalid number of columns specified.'**

- 이 메시지는 CUBRID에서 DBLink를 통해 외부 DB에 쿼리를 실행할 때, 지정한 컬럼 개수가 실제 쿼리 결과의 컬럼 개수와 일치하지 않거나, 시스템에서 요구하는 조건을 만족하지 않을 때 발생합니다, 예를 들어, SELECT 쿼리에서 반환되는 컬럼 개수와 DBLink 객체에 지정된 컬럼 개수가 다르거나, 컬럼 매핑이 잘못된 경우에 발생할 수 있습니다. 


.. _ERROR-1304:

**ERROR CODE: -1304, 'dblink catalog _db_server class not found.'**

- 이 메시지는 CUBRID에서 DBLink 관련 작업(서버 객체 생성, 조회, 수정 등)을 수행할 때, 내부적으로 참조하는 시스템 카탈로그 테이블(_db_server 테이블)이 데이터베이스에 존재하지 않을 경우 발생합니다, 즉, DBLink 기능의 핵심 정보 저장소가 손상되었거나, 제품 버그로 비정상적인 경우입니다. 


.. _ERROR-1305:

**ERROR CODE: -1305, 'dblink server "%1$s" not found.'**

- 이 메시지는 CUBRID에서 DBLink 서버 객체를 참조(예: 연결, 수정, 삭제 등)하려고 할 때, 지정한 이름의 서버 객체가 데이터베이스에 존재하지 않을 경우 발생합니다, 즉, 서버명이 잘못 입력되었거나, 이미 삭제되었거나, 아직 생성되지 않은 경우입니다.


.. _ERROR-1306:

**ERROR CODE: -1306, 'dblink server "%1$s" already exists.'**

- 이 메시지는 CUBRID에서 DBLink 서버 객체를 생성(CREATE SERVER)하려고 할 때, 지정한 이름의 서버 객체가 이미 존재할 경우 발생합니다, 즉, 동일한 서버명이 이미 등록되어 있어 중복 생성이 불가능한 상황입니다.


.. _ERROR-1307:

**ERROR CODE: -1307, 'dblink Cannot update server object.'**

- 이 메시지는 CUBRID에서 DBLink 서버 객체의 정보를 수정(UPDATE)하려고 할 때, 시스템 정책 또는 권한 제한 등으로 인해 해당 작업이 허용되지 않을 때 발생합니다, 예를 들어, onwer, dba 및 dba member가 아닌 경우 권한 부족, 또는 서버 객체의 일관성 보장 등의 이유로 UPDATE가 거부될 수 있습니다.


.. _ERROR-1308:

**ERROR CODE: -1308, 'dblink not supported type %1$s.'**

- 이 메시지는 CUBRID에서 DBLink를 통해 외부 DB에 쿼리를 실행할 때, 사용된 데이터 타입이 CUBRID DBLink 기능에서 지원하지 않는 타입일 경우 발생합니다, 예를 들어, 외부 DB의 특정 데이터 타입(예: 대용량 바이너리, 특수 타입 등)이 CUBRID DBLink에서 매핑/변환/처리가 불가능할 때 발생합니다.


.. _ERROR-1309:

**ERROR CODE: -1309, 'dblink invalid bind param.'**

- 이 메시지는 CUBRID에서 DBLink를 통해 외부 DB에 쿼리를 실행할 때, 전달된 바인드 파라미터가 올바르지 않을 경우 발생합니다.  
예를 들어, 파라미터 개수가 맞지 않거나, 타입이 다르거나, 값이 NULL/비정상 등 허용되지 않는 경우에 발생할 수 있습니다. 


.. _ERROR-1310:

**ERROR CODE: -1310, 'dblink password length exceeds max size.'**

- 이 메시지는 CUBRID에서 DBLink 서버 객체를 생성하거나 수정할 때, 입력한 비밀번호의 길이가 시스템에서 허용하는 최대 크기를 초과할 경우 발생합니다, 이는 비밀번호가 너무 길게 입력되었거나, 인코딩 문제로 인해 실제 바이트 수가 초과된 경우에도 발생할 수 있습니다.  


.. _ERROR-1311:

**ERROR CODE: -1311, 'dblink encrypted password length is incorrect.'**

- 이 메시지는 CUBRID에서 DBLink 서버 객체의 암호화된 비밀번호를 복호화하거나 검증하는 과정에서, 암호화된 문자열의 길이가 시스템이 요구하는 길이와 다를 때 발생합니다, 이는 암호화된 비밀번호가 손상되었거나, 잘못된 방식으로 생성/저장된 경우, 또는 DBLink 객체 생성 시 잘못된 암호문이 입력된 경우에 발생할 수 있습니다. 


.. _ERROR-1312:

**ERROR CODE: -1312, 'dblink the checksum of the encrypted password does not match.'**

- 이 메시지는 CUBRID에서 DBLink 서버 객체의 암호화된 비밀번호를 복호화하거나 검증하는 과정에서, 암호화된 문자열의 체크섬 값이 기대값과 다를 때 발생합니다, 이는 암호화된 비밀번호가 손상되었거나, 암호화/복호화 환경(알고리즘, 키 등)이 일치하지 않거나, 데이터가 잘못 저장/전송된 경우에 발생할 수 있습니다. 


.. _ERROR-1313:

**ERROR CODE: -1313, 'dblink invalid cipher string format.'**

- 이 메시지는 CUBRID에서 DBLink 서버 객체의 비밀번호를 복호화하거나 검증하는 과정에서, 암호화된 문자열(암호문)의 형식이 올바르지 않을 때 발생합니다, 암호화된 문자열이 손상되었거나, 포맷이 맞지 않거나, 예상과 다른 방식으로 저장된 경우(예: 길이 부족, 구분자 누락, 인코딩 오류 등)에 발생할 수 있습니다. 


.. _ERROR-1314:

**ERROR CODE: -1314, 'dblink Failed to decryption password. error=%1$d".'**

- 이 메시지는 CUBRID에서 DBLink 서버 객체를 통해 외부 DB에 접속하려 할 때, 저장된 비밀번호를 복호화하는 과정에서 내부적으로 실패했을 때 발생합니다, 이는 암호화 라이브러리의 문제, 암호화/복호화 키 불일치, 암호화된 데이터 손상, 시스템 환경 문제, 내부 버그 등 다양한 원인으로 인해 복호화가 정상적으로 수행되지 않을 때 반환됩니다.  


.. _ERROR-1315:

**ERROR CODE: -1315, 'dblink Failed to encryption password. error=%1$d".'**

- 이 메시지는 CUBRID에서 DBLink 서버 객체를 생성하거나 수정할 때, 외부 DB 접속용 비밀번호를 암호화하는 과정에서 내부적으로 실패했을 때 발생합니다, 이는 암호화 라이브러리의 문제, 입력값 오류, 시스템 리소스 부족, 암호화 알고리즘 미지원, 내부 버그 등 다양한 원인으로 인해 암호화가 정상적으로 수행되지 않을 때 반환됩니다.  


.. _ERROR-1317:

**ERROR CODE: -1317, 'dblink Not allowed "%1$s" Server.'**

- 이 메시지는 CUBRID에서 DBlink 서버 객체에 대해 허용되지 않은 조작(ALTER 등)을 시도할 때 발생합니다, 이는 특정 시스템 정책, 권한, 또는 DBLink 서버의 속성에 따라 해당 서버에 대한 변경(ALTER, DROP 등)이 금지되어 있을 수 있습니다.  
예를 들어, 시스템에서 보호되는 서버이거나, 현재 세션/사용자에게 해당 서버를 변경할 권한이 없는 경우에 발생합니다.


.. _ERROR-1318:

**ERROR CODE: -1318, '[%1$5.5s][%2$d] %3$s'**

- 이 메시지는 CUBRID의 Gateway(DBlink  외부 DB 연동, ODBC 등) 기능에서 외부 데이터베이스(ODBC 드라이버 등)로부터 에러가 발생했을 때, 해당 에러의 상세 정보를 그대로 포맷팅하여 전달하는 메시지입니다. 즉, CUBRID 내부가 아닌 외부 DBMS 또는 ODBC 드라이버에서 발생한 에러의 SQLSTATE, 에러 코드, 상세 메시지를 그대로 보여줍니다.  
- 이 메시지는 CUBRID가 DBlink 외부 DB 연동 중 발생한 문제의 원인을 직접 해석하지 않고, 외부 시스템의 에러 정보를 그대로 전달하는 역할을 합니다.


.. _ERROR-1319:

**ERROR CODE: -1319, 'Parameter binding error.'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 기능에서 SQL 쿼리 실행 시, 파라미터(입력 변수) 바인딩 과정에서 문제가 발생했을 때 출력됩니다. 주로 바인딩할 파라미터의 개수가 맞지 않거나, 타입이 일치하지 않거나, 내부적으로 바인딩 핸들(구조체 등)이 올바르게 생성되지 않은 경우에 발생합니다. 예를 들어, 쿼리에서 3개의 파라미터가 필요하지만 2개만 바인딩했거나, 바인딩 값의 타입이 DB 컬럼 타입과 맞지 않을 때 발생할 수 있습니다.


.. _ERROR-1320:

**ERROR CODE: -1320, 'Invalid ODBC handle.'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 기능에서 ODBC 핸들(식별자, 포인터 등)이 유효하지 않을 때 발생합니다. 즉, 이미 닫힌(해제된) 핸들을 참조하거나, 올바르게 생성되지 않은 핸들을 사용할 때, 또는 내부적으로 핸들이 손상되었을 때 발생할 수 있습니다. 주로 연결 해제 후 재사용, 핸들 누락, 메모리 손상, 드라이버 버그 등에서 발생합니다.


.. _ERROR-1322:

**ERROR CODE: -1322, 'not supported type %1$s(%2$d).'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 기능에서 외부 데이터베이스와의 연동 또는 쿼리 실행 시, CUBRID가 지원하지 않는 데이터 타입이 사용되었을 때 발생합니다. 예를 들어, 외부 DB의 테이블에 CUBRID에서 인식할 수 없는 타입(예: BLOB, CLOB, 사용자 정의 타입 등)이 포함되어 있거나, 타입 매핑이 정의되어 있지 않은 경우입니다. 이 메시지는 주로 데이터 타입 호환성 문제, 타입 변환 로직 미구현, 또는 드라이버/연동 모듈의 한계로 인해 발생합니다.


.. _ERROR-1323:

**ERROR CODE: -1323, 'Invalid Statement handle.'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 기능에서 SQL 문을 관리하는 Statement 핸들(식별자, 포인터 등)이 유효하지 않을 때 발생합니다. 즉, 이미 닫힌 Statement를 참조하거나, 올바르게 생성되지 않은 핸들을 사용할 때, 또는 내부적으로 핸들이 손상되었을 때 발생할 수 있습니다. 주로 Statement 해제 후 재사용, 핸들 누락, 메모리 손상, 드라이버 버그 등에서 발생합니다.


.. _ERROR-1324:

**ERROR CODE: -1324, 'Invalid db connection handle.'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 기능에서 데이터베이스 연결을 관리하는 핸들(식별자, 포인터 등)이 유효하지 않을 때 발생합니다. 즉, 이미 닫힌 연결을 참조하거나, 올바르게 생성되지 않은 핸들을 사용할 때, 또는 내부적으로 핸들이 손상되었을 때 발생할 수 있습니다. 주로 연결 해제 후 재사용, 핸들 누락, 메모리 손상, 드라이버 버그 등에서 발생합니다.


.. _ERROR-1325:

**ERROR CODE: -1325, 'Link server connection url do not exist.'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 기능에서 외부 데이터베이스에 연결을 시도할 때, 연결에 필요한 URL(접속 정보)이 정의되어 있지 않거나, 시스템에서 해당 정보를 찾을 수 없을 때 발생합니다. 즉, DBLink 또는 Gateway 서버 객체를 생성할 때 연결 URL을 누락했거나, 내부적으로 해당 정보가 정상적으로 등록되지 않은 경우입니다.


.. _ERROR-1326:

**ERROR CODE: -1326, 'Invalid numeric value.'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 질의에 잘못된 numeric 타입 값 처리 시 발생합니다.


.. _ERROR-1327:

**ERROR CODE: -1327, 'Invalid precision value of "%1$s" column.'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 기능에서 외부 DB의 테이블 또는 쿼리 결과를 처리할 때, 특정 컬럼의 정밀도(precision) 값이 허용 범위를 벗어나거나, 잘못된 값(음수, 0, 너무 큰 값 등)으로 지정된 경우에 발생합니다. 주로 외부 DB의 스키마 정의가 잘못되었거나, ODBC 드라이버가 컬럼의 정밀도 정보를 잘못 반환할 때 발생할 수 있습니다.


.. _ERROR-1329:

**ERROR CODE: -1329, 'Not supported dbms.'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 기능에서 연결을 시도한 외부 DBMS가 CUBRID에서 공식적으로 지원하지 않는 DBMS일 때 발생합니다. 즉, CUBRID Gateway가 인식할 수 없는 DBMS 유형이거나, 지원 목록에 포함되지 않은 DBMS에 연결을 시도할 때 발생합니다. 예를 들어, CUBRID Gateway가 Oracle, MySQL, MSSQL 등 일부 DBMS만 지원하는데, 지원하지 않는 DBMS(예: SQLite, MariaDB, 특수 목적 DB 등)에 연결을 시도하면 이 오류가 반환됩니다.


.. _ERROR-1330:

**ERROR CODE: -1330, 'Unable to allocate an environment handle.'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 기능에서 외부 데이터베이스와의 연결을 시작할 때, 환경 핸들(ODBC 환경 핸들 등)을 생성(할당)하는 과정에서 실패했을 때 발생합니다. 환경 핸들은 외부 DB와의 세션, 연결, 트랜잭션 등 모든 작업의 기반이 되는 리소스이므로, 할당에 실패하면 이후의 모든 DB 연동 작업이 불가능해집니다. 주로 시스템 메모리 부족, ODBC 드라이버/라이브러리 문제, 내부 리소스 고갈 등으로 인해 발생할 수 있습니다.


.. _ERROR-1331:

**ERROR CODE: -1331, 'A null column binder was referenced.'**

- 이 메시지는 CUBRID의 Gateway(DBlink 외부 DB 연동, ODBC 등) 기능에서 SQL 쿼리 실행 결과를 애플리케이션 변수에 바인딩하는 과정에서, 컬럼 바인더(바인딩 핸들)가 NULL(초기화되지 않음) 상태로 참조될 때 발생합니다. 즉, 컬럼 바인더가 올바르게 생성/할당되지 않았거나, 이미 해제된(Free) 상태에서 접근하려고 할 때 발생하는 프로그래밍 오류입니다. 주로 DBlike 외부 DB 연동, ODBC 드라이버, 또는 내부 바인딩 로직의 문제로 인해 발생할 수 있습니다.


.. _ERROR-1332:

**ERROR CODE: -1332, 'The active log volume (%1$s) is too sane to recreate. If you remove the active log, it could cause an unexpected and irreversible problem.'**

- 이 메시지는 CUBRID에서 `cubrid emergencylog -r` 명령어를 사용 시 active log가 정상인 경우 재생성할 수 없다고 발생합니다, 이는 활성 로그(active log) 볼륨을 재생성(recreate)하려고 할 때, 해당 로그 파일이 손상되지 않고 정상(sane) 상태이기 때문에 시스템이 재생성을 허용하지 않을 때 발생합니다.


.. _ERROR-1333:

**ERROR CODE: -1333, ' Invalid time (%1$s) to start flashback. Time is required to be set between (%2$s) and (%3$s).'**

- 이 메시지는 CUBRID에서 플래시백(Flashback) 기능을 실행할 때, 사용자가 지정한 복구 시점(시간)이 시스템에서 허용하는 범위를 벗어난 경우 발생합니다. 플래시백은 로그 보관, 데이터베이스 상태, 백업 시점 등에 따라 복구 가능한 시간 범위가 제한되며, 이 범위를 벗어난 시각으로 요청하면 데이터 일관성 및 무결성 보장을 위해 작업이 거부됩니다.


.. _ERROR-1334:

**ERROR CODE: -1334, 'Can not find the class to flashback (%1$s).'**

- 이 메시지는 CUBRID에서 플래시백(Flashback) 기능을 실행할 때, 복구 대상이 되는 테이블(테이블)가 데이터베이스에 존재하지 않거나, 시스템에서 해당 테이블를 찾을 수 없는 경우 발생합니다. 주로 테이블가 삭제되었거나, 이름이 잘못 지정되었거나, 시스템 카탈로그에 문제가 있을 때 발생할 수 있습니다.


.. _ERROR-1335:

**ERROR CODE: -1335, 'Too many transactions to flashback. Less than %1$d transactions can be flashback.'**

- 이 메시지는 CUBRID에서 플래시백(Flashback) 기능을 실행할 때, 복구 대상이 되는 트랜잭션의 개수가 시스템에서 허용하는 최대값을 초과한 경우 발생합니다. 플래시백 작업은 데이터베이스의 일관성과 성능을 위해 한 번에 처리할 수 있는 트랜잭션 수에 제한이 있습니다. 이 제한을 초과하면 시스템 과부하, 성능 저하, 복구 실패 등의 문제가 발생할 수 있으므로, 정책적으로 제한을 두고 있습니다.


.. _ERROR-1336:

**ERROR CODE: -1336, 'Cannot support flashback for this class (name : %1$s , OID : %2$d|%3$d|%4$d). Class schema has been changed.'**

- 이 메시지는 CUBRID에서 플래시백(Flashback) 기능을 실행할 때, 복구 대상이 되는 테이블(테이블)의 스키마(구조)가 플래시백 대상 시점 이후에 변경된 경우 발생합니다. 예를 들어, 컬럼 추가/삭제, 타입 변경, 제약조건 변경 등 스키마가 변경되면, 플래시백 시점의 데이터와 현재 구조가 일치하지 않아 데이터 복구가 불가능해집니다. 데이터 무결성과 일관성을 보장하기 위해, 스키마가 변경된 테이블에는 플래시백을 지원하지 않습니다.


.. _ERROR-1337:

**ERROR CODE: -1337, 'Log record does not exist (lsa : %1$lld|%2$d). The archive log volume has been deleted.'**

- 이 메시지는 CUBRID 데이터베이스에서 특정 로그 레코드(트랜잭션 복구, 플래시백 등에서 필요)가 존재하지 않거나, 해당 로그가 저장된 아카이브 로그 볼륨이 이미 삭제되어 더 이상 접근할 수 없을 때 발생합니다, 주로 오래된 로그가 자동/수동으로 삭제되었거나, 백업/복구/아카이브 정책에 따라 로그 파일이 정리된 경우, 또는 로그 파일 손상, 잘못된 LSA 접근 등으로 인해 발생할 수 있습니다.


.. _ERROR-1338:

**ERROR CODE: -1338, 'Flashback is already running. Flashback cannot be run by more than one user at the same time.'**

- 이 메시지는 CUBRID에서 플래시백(Flashback) 기능을 실행하려고 할 때, 이미 다른 세션 또는 사용자가 플래시백 작업을 진행 중인 경우 발생합니다. 플래시백은 데이터베이스의 일관성과 무결성을 보장하기 위해 단일 사용자만이 동시에 실행할 수 있도록 제한되어 있습니다. 따라서, 한 번에 하나의 플래시백 작업만 허용되며, 중복 요청이 들어오면 해당 오류가 반환됩니다.


.. _ERROR-1341:

**ERROR CODE: -1341, 'Invalid arguments to authorization internal function.'**

- 이 메시지는 CUBRID 데이터베이스에서 class 또는 method의 owner 변경 시 원하는 정보가 없는 경우 발생합니다.


.. _ERROR-1342:

**ERROR CODE: -1342, 'AUTO_INCREMENT does not allow change of owner.'**

- 이 메시지는 CUBRID 데이터베이스에서 auto increment의 serial 객체의 owner를 변경 시도할 때  발생합니다.


.. _ERROR-1345:

**ERROR CODE: -1345, 'DBA, members of DBA group, and owner can perform CREATE TRIGGER.'**

- 이 메시지는 CUBRID에서 트리거(CREATE TRIGGER)를 생성하려고 할 때, 해당 명령을 실행하는 사용자가 DBA, DBA 그룹의 멤버, 또는 트리거 대상 객체의 소유자가 아닐 경우 발생합니다. 즉, 트리거 생성 권한이 없는 사용자가 CREATE TRIGGER를 시도할 때 발생하는 권한(Privilege) 관련 오류입니다.


.. _ERROR-1350:

**ERROR CODE: -1350, 'Invalid synonym value.'**

- 이 메시지는 CUBRID에서 umloaddb 명령어 수행 시 잘 못된 정보를 가지고 있는 시노님(SYNONYM) 객체에서 발생합니다. 


.. _ERROR-1356:

**ERROR CODE: -1356, 'There is no hint in the DBLink query, or the statement type is incorrect. %1$s'**

- 이 메시지는 CUBRID에서 DBlink 질의는 내부적으로 statement type을 hint로 추가해서 dblink gateway로 전달해서 어떤 종류의 질의를 판단하는데, 알수 없는 statement type이 전달되어서 발생합니다.


.. _ERROR-1357:

**ERROR CODE: -1357, 'Converting SQL string to wide string failed.'**

- 이 메시지는 CUBRID가 DBlink Gateway에서 외부 데이터베이스와 통신할 때, SQL 쿼리 문자열을 UTF8에서 유니코드로 변경할 수 없을 때, 주로 문자 인코딩이 맞지 않거나, 입력 문자열에 유효하지 않은(깨진) 문자가 포함되어 있을 때 발생합니다.


.. _ERROR-1358:

**ERROR CODE: -1358, 'Number of rows affected is unknown.'**

- 이 메시지는 CUBRID가 DML(INSERT, UPDATE, DELETE)을 실행 시 실제로 영향을 받은 행의 개수를 정상적으로 파악하지 못했을 때 발생합니다. DBlink 질의에서 데이터베이스 간의 연동 작업하면서 외부 시스템이 영향을 받은 행의 수를 반환하지 않거나, 반환값이 명확하지 않을 때 발생할 수 있습니다.


.. _ERROR-1362:

**ERROR CODE: -1362, 'Locale '%1$s' is unavailable.'**

- 이 메시지는 CUBRID가 문자열을 날짜/시간로 변경시 서버에 로딩되지 않은 로케일 정보를 사용해 발생합니다,

