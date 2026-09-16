# CUBRID 매뉴얼 작성 표기 표준

> 적용 대상: `CUBRID/cubrid-manual` 저장소의 모든 `.rst` / `.inc` 문서 (`ko`, `en`)
> 근거: [CUBRIDMAN-343](http://jira.cubrid.org/browse/CUBRIDMAN-343)
> 이 문서는 표기 표준의 **정본**입니다. Greptile 리뷰 규칙(`.greptile/rules.md`)과 내용이 어긋나면 이 문서를 우선합니다.

---

## 목차

1. [이 문서를 읽는 이유](#1-이-문서를-읽는-이유)
2. [모드 구분](#2-모드-구분)
3. [한국어 표기 규칙 33개](#3-한국어-표기-규칙-33개)
4. [영문 표기 규칙 34개](#4-영문-표기-규칙-34개)
5. [문자 인코딩 표기 기준](#5-문자-인코딩-표기-기준)
6. [표기 규칙 적용 제외 대상](#6-표기-규칙-적용-제외-대상)
7. [reStructuredText 작성 규칙](#7-restructuredtext-작성-규칙)
8. [ko--en-번역-쌍-동기화](#8-ko--en-번역-쌍-동기화)
9. [고객 리스크 표현](#9-고객-리스크-표현)
10. [작성자 셀프 체크 절차](#10-작성자-셀프-체크-절차)
11. [Greptile 자동 리뷰 연동](#11-greptile-자동-리뷰-연동)
12. [표준 변경 절차](#12-표준-변경-절차)

---

## 1. 이 문서를 읽는 이유

매뉴얼은 고객사와 평가위원이 읽는 문서입니다. 같은 개념을 문서마다 다르게 부르면
독자는 서로 다른 기능으로 오해합니다. 표기 통일은 문체 취향이 아니라 **기술 정합성 문제**입니다.

이 표준은 사후 일괄 수정 PR을 줄이기 위한 것입니다.
작성 시점에 이 문서를 참조하면 리뷰 왕복이 사라집니다.

이 저장소는 `ko` 132개, `en` 132개의 번역 쌍으로 구성됩니다.
한쪽만 수정하면 두 언어 매뉴얼의 기술 사양이 어긋나므로 8절 기준을 함께 확인합니다.

---

## 2. 모드 구분

| 모드 | 의미 | Greptile 심각도 | 작성자 대응 |
|:---|:---|:---:|:---|
| `AUTO` | 문맥과 무관하게 항상 틀린 표기 | `high` | **반드시 수정** |
| `WORD` | 단어 경계 기준으로 판단 | `high` | 단어 경계 확인 후 수정 |
| `MANUAL` | 문맥에 따라 유지 가능 | `low` | 예외 조건 확인 후 판단 |

`MANUAL` 항목에서 원문을 유지하기로 판단했다면, PR 코멘트에 **유지 이유를 남깁니다.**
같은 판단이 반복되면 12절 절차로 예외를 표준에 반영합니다.

---

## 3. 한국어 표기 규칙 33개

적용 범위: `ko/**/*.rst`, `ko/**/*.inc`

| 번호 | 오류 표기 | 권장 표기 | 모드 | 예외 및 비고 |
|:---:|:---|:---|:---:|:---|
| 1 | 에러 | 오류 | AUTO | 제품 출력 오류 메시지 원문은 제외 |
| 2 | 컬럼 | 칼럼 | AUTO | SQL 식별자, 이미지 파일명, 카탈로그 칼럼명은 제외 |
| 3 | 매개 변수 | 파라미터 | AUTO | |
| 4 | 서브 쿼리 | 부질의 | AUTO | |
| 5 | 슬로우 쿼리 | 슬로우쿼리 | AUTO | 붙여 씀 |
| 6 | 쿼리 | 질의 | MANUAL | **`슬로우쿼리`, `부질의`는 유지.** 제품 UI 명칭·옵션명의 query는 유지 |
| 7 | 리턴 | 반환 | AUTO | API 절 제목 `Return Value`, `return` 키워드는 제외 |
| 8 | 커멘트 | 주석 | AUTO | SQL `COMMENT` 구문은 제외 |
| 9 | 쓰레드 | 스레드 | AUTO | |
| 10 | 리소스 | 자원 | MANUAL | 옵션값·경로·API 이름의 resource는 유지 |
| 11 | 메소드 | 메서드 | AUTO | 메서드 시그니처·코드 식별자는 제외 |
| 12 | 디렉토리 | 디렉터리 | AUTO | 경로 문자열은 제외 |
| 13 | 메세지 | 메시지 | AUTO | |
| 14 | 유저 | 사용자 | MANUAL | DB 계정명, 옵션명 `user`/`username`은 유지 |
| 15 | 파라메터 | 파라미터 | AUTO | |
| 16 | 레퍼런스 | 참조 | MANUAL | `CUBRID 레퍼런스 매뉴얼`처럼 공식 문서 제목은 유지 |
| 17 | 데이타 | 데이터 | AUTO | |
| 18 | 툴킷 | 도구 모음 | MANUAL | **`CUBRID Migration Toolkit`은 제품 정식 명칭이므로 유지** |
| 19 | 스토리지 | 저장소 | AUTO | |
| 20 | 디폴트 | 기본값 | AUTO | |
| 21 | 커넥션 | 연결 | AUTO | |
| 22 | 캐쉬 | 캐시 | AUTO | |
| 23 | 어플리케이션 | 애플리케이션 | AUTO | |
| 24 | 셧다운 | 종료 | AUTO | |
| 25 | 엔클러저 | 인클로저 | AUTO | |
| 26 | 운영체제 | 운영 체제 | AUTO | 띄어 씀 |
| 27 | 소스코드 | 소스 코드 | AUTO | 띄어 씀 |
| 28 | 데이터 베이스 | 데이터베이스 | AUTO | 붙여 씀 |
| 29 | 서브쿼리 | 부질의 | AUTO | |
| 30 | 하위 질의 | 부질의 | AUTO | |
| 31 | 느린 질의 | 슬로우쿼리 | AUTO | |
| 32 | 매개변수 | 파라미터 | AUTO | |
| 33 | 리스트 | 목록 | MANUAL | UI 컨트롤명(List Box), 자료형 `list`, SQL 문법 용어(SELECT 리스트)는 유지 |

### 규칙 간 우선순위

규칙이 서로 충돌할 때의 적용 순서입니다.

1. **제품 정식 명칭** — 모든 규칙보다 우선합니다.
2. **복합어 규칙** — 5·31번(`슬로우쿼리`), 4·29·30번(`부질의`)이 6번(`쿼리 → 질의`)보다 우선합니다.
3. **단일어 규칙** — 6번 등 나머지.

```text
슬로우 쿼리   → 슬로우쿼리      (O)  5번 적용
슬로우쿼리    → 슬로우질의      (X)  6번을 잘못 적용
서브 쿼리     → 부질의          (O)  4번 적용
서브 쿼리     → 서브 질의       (X)  6번을 잘못 적용
```

---

## 4. 영문 표기 규칙 34개

적용 범위: `en/**/*.rst`, `en/**/*.inc`, 그리고 **`ko`의 영문 서술 부분**

| 번호 | 오류 표기 | 권장 표기 | 모드 | 예외 및 비고 |
|:---:|:---|:---|:---:|:---|
| 1 | host name | hostname | AUTO | 옵션명 `--host-name`, 파라미터명은 제외 |
| 2 | user name | username | AUTO | 칼럼명 `user_name`, 옵션명은 제외 |
| 3 | file name | filename | AUTO | |
| 4 | auto-commit | autocommit | MANUAL | `;autocommit`, `--no-auto-commit`, API 상수명 제외 |
| 5 | auto commit | autocommit | MANUAL | 위와 동일 |
| 6 | time zone | timezone | AUTO | 파라미터명 `timezone`과 구분 |
| 7 | fail-over | failover | AUTO | |
| 8 | sub-query | subquery | AUTO | |
| 9 | upper case | uppercase | AUTO | |
| 10 | upper-case | uppercase | AUTO | |
| 11 | white space | whitespace | AUTO | |
| 12 | multi-byte | multibyte | AUTO | |
| 13 | look-up | lookup | AUTO | |
| 14 | run-time | runtime | AUTO | |
| 15 | start-up | startup | AUTO | |
| 16 | log-in | login | AUTO | |
| 17 | type-cast | typecast | AUTO | |
| 18 | datatype | data type | WORD | 코드·API 식별자, SQL 타입 표기는 유지 |
| 19 | data base | database | AUTO | |
| 20 | meta-data | metadata | AUTO | |
| 21 | on-line | online | AUTO | |
| 22 | back-up | backup | AUTO | |
| 23 | can not | cannot | WORD | |
| 24 | can't | cannot | AUTO | 축약형 사용 금지 |
| 25 | centre | center | MANUAL | 예제 데이터 값은 제외 |
| 26 | indices | indexes | WORD | 수학적 지수 문맥은 `indices` 유지 |
| 27 | Cubrid | CUBRID | MANUAL | 클래스명(`SpCubrid`, `CUBRIDDriver`), 패키지 경로, SQL 예제는 유지 |
| 28 | Id | ID | MANUAL | 코드 식별자·필드명의 `Id`는 유지 |
| 29 | UTF8 | UTF-8 | MANUAL | 5절 참조 |
| 30 | utf-8 | UTF-8 | MANUAL | 5절 참조 |
| 31 | euckr | EUC-KR | MANUAL | 5절 참조 (옵션값은 유지) |
| 32 | eucKR | euckr | MANUAL | 5절 참조 |
| 33 | EUCKR | EUC-KR | MANUAL | 5절 참조 |
| 34 | UTF-8 | utf8 | MANUAL | 5절 참조 (옵션값 자리에서만) |

### 축약형 금지 확장

24번은 `can't`만 명시하지만, 영문 매뉴얼에서는 축약형 전체를 사용하지 않습니다.
`don't → do not`, `won't → will not`, `isn't → is not`, `doesn't → does not`, `it's → it is`.
제품 출력 오류 메시지 원문에 포함된 축약형은 유지합니다.

---

## 5. 문자 인코딩 표기 기준

29~34번은 **서로 반대 방향의 규칙**을 포함합니다.
`UTF8 → UTF-8`(29번)과 `UTF-8 → utf8`(34번)이 동시에 존재하므로,
찾아 바꾸기로 일괄 치환하면 반드시 문서가 깨집니다.

현재 저장소 검출 현황입니다. 절대적인 수가 많으므로 특히 주의합니다.

| 표기 | 건수 | 판정 |
|:---|:---:|:---|
| `utf8` | 780 | 대부분 옵션값·콜레이션 이름 → 유지 |
| `UTF-8` | 212 | 대부분 설명 문장 → 유지. 옵션값 자리라면 `utf8`로 수정 |
| `euckr` | 80 | 대부분 옵션값 → 유지 |
| `EUC-KR` | 52 | 설명 문장 → 유지 |
| `utf-8` | 26 | 설명 문장이면 `UTF-8`, 옵션값이면 `utf8` |
| `UTF8` | 24 | 설명 문장이면 `UTF-8`, 옵션값이면 `utf8` |
| `eucKR` | 2 | 항상 `euckr` |

### 판정 순서

```
① 이 문자열이 명령어·옵션값·설정 파일 예제·SQL 문자셋 지정 안에 있는가?
     └ YES → 소문자, 하이픈 없음
              utf8 / euckr / ko_KR.euckr / ko_KR.utf8
② 이 문자열이 일반 설명 문장 안에 있는가?
     └ YES → IANA 표준 명칭
              UTF-8 / EUC-KR
③ 옵션값 자리에는 하이픈 표기를 절대 쓰지 않는다.
```

CUBRID는 인코딩 이름의 대소문자를 구분하지 않지만, **문서 표기는 위 기준 하나로 통일**합니다.

> **이 규칙은 6절 제외 대상의 유일한 예외입니다.**
> 옵션값 예제가 코드 블록이나 인라인 리터럴 안에 있어도 하이픈 표기라면 수정 대상입니다.
> 다른 표기 규칙은 코드 블록 안에서 적용하지 않습니다.

### 콜레이션 이름은 대상이 아닙니다

`utf8_bin`, `utf8_ko_cs`, `euckr_bin` 같은 콜레이션 이름과
`CHARSET` / `COLLATE` 절의 값은 **SQL 식별자**입니다.
소문자 표기를 그대로 유지하며 하이픈 표기로 바꾸지 않습니다.

### 적용 예

| 위치 | 잘못된 예 | 올바른 예 |
|:---|:---|:---|
| 설명 문장 | 문자셋을 UTF8로 지정한다. | 문자셋을 UTF-8로 지정한다. |
| 설명 문장 | euckr 인코딩을 지원한다. | EUC-KR 인코딩을 지원한다. |
| 옵션값 | `--db-locale=ko_KR.eucKR` | `--db-locale=ko_KR.euckr` |
| 옵션값 | `--db-locale=ko_KR.EUC-KR` | `--db-locale=ko_KR.euckr` |
| 옵션값 | `charset=UTF-8` | `charset=utf8` |
| SQL 식별자 | (유지) | `COLLATE utf8_bin` |

### 한 문장 안에 둘 다 나오는 경우

```rst
EUC-KR 인코딩을 사용하려면 ``--db-locale=ko_KR.euckr`` 옵션을 지정한다.
```

설명 부분은 `EUC-KR`, 옵션값은 `euckr`입니다. **둘을 통일하려 하지 마십시오.**

---

## 6. 표기 규칙 적용 제외 대상

다음 위치의 문자열은 **표기 규칙 대상이 아닙니다.** 원문을 그대로 유지합니다.

| 구분 | 내용 |
|:---|:---|
| 코드 블록 | `.. code-block::`, `.. literalinclude::`, `::` 리터럴 블록 내부 |
| 인라인 리터럴 | 이중 백틱으로 감싼 부분 |
| SQL 문법 정의 | BNF 표기, `<expression>` 문법 요소, 대괄호 선택 표기, 중괄호 필수 표기 |
| SQL 식별자 | 예약어, 스키마/테이블/칼럼/인덱스/트리거 이름, 예제 데이터, 질의 결과 출력 |
| 타입·함수 | 데이터 타입명, 함수·연산자 이름, 콜레이션 이름 |
| 설정 | 시스템 파라미터명과 값, 설정 파일 키, 환경 변수명과 값 |
| 명령어 | `cubrid` 유틸리티 명령, 하위 명령, 실행 옵션명과 옵션값 |
| API | 클래스명, 인터페이스명, 메서드명, 함수명, 상수명, 프로퍼티명, 접속 URL, 반환 타입 |
| 제품 출력 | 오류 메시지, 오류 코드, 로그, 모니터링 지표명(`statdump` 등), 리포트 출력 |
| 경로 | 파일명, 디렉터리 경로, URL, 이미지 파일 경로, 포트 번호 |
| Sphinx 문법 | `:ref:`, `:option:`, `:file:`, `:c:func:`, `:menuselection:` 등의 타깃 값 |
| 제품 명칭 | 정식 제품명과 상표. `CUBRID`, `CUBRID Manager`, `CUBRID Migration Toolkit` |
| 릴리스 노트 | 이슈 번호(CBRD-xxxxx, APIS-xxxx), 변경 이력 원문, 이슈 제목 인용 |
| 변경 안 된 줄 | 이 PR에서 수정하지 않은 줄 |

### 오류 메시지 원문 처리

`admin/error_log*.rst`는 제품이 출력하는 메시지 카탈로그입니다.
메시지 안에 `에러`, `컬럼`, `can't` 같은 표기가 있어도 **수정하지 않습니다.**
수정하면 사용자가 실제 로그와 문서를 대조할 수 없습니다.

```rst
-- 유지: 제품 출력 원문
ERROR: Column 'a' cannot be null.

-- 수정: 이를 설명하는 서술 문장
해당 컬럼이 NULL을 허용하지 않을 때 발생한다.   → 해당 칼럼이 ...
```

### API 식별자 처리

`api`, `pl` 디렉터리에서 가장 오탐이 많은 지점입니다.

```rst
getConnection() 메소드는 연결을 리턴한다.
  → getConnection() 메서드는 연결을 반환한다.        (서술 문장 → 수정)

**Return Value**
  → 원문 유지                                       (API 절 제목 → 유지)

CUBRIDStatement.getMethod()
  → 원문 유지                                       (코드 식별자 → 유지)
```

### 판정이 애매할 때

이 저장소는 SQL 구문과 API 시그니처 비중이 높습니다.
**판정이 애매하면 제외 대상으로 간주해 원문을 유지합니다.**
잘못 바꾸는 손해가 안 바꾸는 손해보다 큽니다.

---

## 7. reStructuredText 작성 규칙

| 항목 | 규칙 |
|:---|:---|
| 제목 밑줄 | 제목 텍스트 길이 이상. 한글은 2바이트 폭으로 계산 |
| 제목 계층 | 파일 전체에서 일관된 문자 사용 |
| 지시자 본문 | 3칸 들여쓰기 |
| 표 | 셀 내용을 수정하면 구분선 길이를 함께 조정 |
| 상호 참조 | `:ref:` 타깃은 실제 존재하는 라벨 |
| 이미지 | `.. image::` / `.. figure::` 경로는 실제 파일을 가리킴 |
| `toctree` | 새 문서를 추가하면 `toc.rst`에 등록 |
| `.inc` 파일 | `backup.inc`, `migration.inc`, `join_method.inc`는 `.. include::` 대상이며 `.rst`와 동일한 기준 적용 |

### 문체

한국어 매뉴얼의 기준 문체는 **`~한다`체(평서체)** 입니다.

| 종결 | 건수 |
|:---|:---:|
| `한다.` | 6,093 |
| `된다.` | 1,979 |
| `있다.` | 1,905 |
| **평서체 합계** | **9,977** |
| `합니다.` | 423 |
| `입니다.` | 304 |
| `됩니다.` | 35 |
| **경어체 합계** | **762** |

새로 추가·수정하는 문장은 `~한다`, `~된다`, `~있다`로 종결합니다.
표·그림 캡션, 목록 항목의 명사형 종결, 오류 메시지 원문 인용, 릴리스 노트 공지 문장은 예외입니다.

> 이 문서(`STYLE_GUIDE.ko.md`)는 매뉴얼 본문이 아니므로 경어체를 사용합니다.

영문 매뉴얼은 이 규칙의 대상이 아닙니다(`en/.greptile/config.json`의 `disabledRules`).
영문은 축약형을 쓰지 않고, 새로 작성하는 절차 설명은 명령형 현재 시제로 통일합니다.

### 제품 명칭

| 제품 | 표기 | 비고 |
|:---|:---|:---|
| CUBRID | `CUBRID` | 전부 대문자. `Cubrid`는 오표기(ko 8건, en 8건) |
| CUBRID Manager | `CUBRID Manager` | 약어 `CM` |
| CUBRID Migration Toolkit | `CUBRID Migration Toolkit` | 약어 `CMT`. 규칙 18번을 적용하지 않음 |

`Cubrid`가 클래스명(`CUBRIDDriver`, `SpCubrid`), 패키지 경로, SQL 예제 식별자에 쓰인 경우는 유지합니다.

### 이미지 파일명 변경 시

파일명을 바꾸면 이를 참조하는 **모든** 지시자를 함께 갱신합니다.
누락하면 Sphinx 빌드 경고와 깨진 이미지가 발생합니다.

```bash
# 저장소 루트에서 참조 누락 확인
grep -rn "변경한파일명" --include='*.rst' --include='*.inc' ko en
```

---

## 8. ko / en 번역 쌍 동기화

`ko`와 `en`은 같은 내용의 번역 쌍이며 각각 132개의 `.rst` 파일을 가집니다.
경로는 최상위 디렉터리만 다르고 나머지는 동일합니다.

```
ko/sql/query/select.rst  ↔  en/sql/query/select.rst
ko/admin/config.rst      ↔  en/admin/config.rst
```

### 반대쪽도 함께 수정해야 하는 변경

- SQL 구문 정의, 옵션 목록, 기본값, 지원 범위, 제약 사항
- 새로 추가·삭제된 절이나 표
- `:ref:` 라벨의 신규 추가·삭제
- `.. versionadded::` / `.. deprecated::` 버전 표기

한쪽만 고치면 두 언어 매뉴얼이 서로 다른 제품을 설명하게 됩니다.

### 한쪽만 수정해도 되는 변경

- 문장 표현 다듬기, 오타 수정
- 해당 언어의 표기 규칙 적용

### 확인 명령

```bash
# 이 PR이 건드린 파일의 반대쪽 대응 파일 확인
git diff --name-only develop...HEAD | sed -e 's|^ko/|en/|' -e 's|^en/|ko/|' | sort -u
```

출력된 경로가 변경 목록에 없다면, 기술 사양 변경인지 판정합니다.

---

## 9. 고객 리스크 표현

매뉴얼은 고객사에 전달되는 문서입니다. 보증성 문구는 법적 리스크가 됩니다.

### 사용 금지

| 유형 | 금지 표현 |
|:---|:---|
| 보증성 | `완벽한`, `완벽하게`, `무결한`, `100% 보장`, `절대`, `손실 없이`, `모든 경우에` |
| 비교 우위 단정 | 경쟁 제품과의 우열을 조건 없이 단정하는 표현 |
| 조건 없는 성능 수치 | 하드웨어·데이터량·버전 조건 없이 제시하는 처리량·응답 시간 |

### 대안 문구

| 금지 | 대안 |
|:---|:---|
| 완벽하게 복구한다 | 백업 시점까지 복구한다 |
| 손실 없이 복구한다 | 아카이브 로그가 보존된 범위까지 복구한다 |
| 100% 호환된다 | 지원 타입 매핑 표에 정의된 범위에서 호환된다 |
| 절대 실패하지 않는다 | 사전 조건을 충족하면 정상 동작한다 |
| 모든 질의를 최적화한다 | 옵티마이저가 통계 정보를 갖춘 질의에서 최적화한다 |

### 위험 절차 경고

백업/복구, `deletedb`, 볼륨 조작, HA 전환, 스키마 변경 절차에는
사전 조건과 되돌릴 수 없는 동작에 대한 경고가 있어야 합니다.
경고 없이 명령어만 제시하지 않습니다.

```rst
.. warning::

   deletedb는 데이터베이스 볼륨과 로그를 삭제하며 되돌릴 수 없다.
   실행 전 백업 여부를 확인한다.
```

---

## 10. 작성자 셀프 체크 절차

PR을 올리기 전에 다음 순서로 확인합니다.

### 1단계 — AUTO 항목 검색

```bash
# 한국어 AUTO 항목 (저장소 루트에서 실행)
grep -rn -E "에러|컬럼|매개 ?변수|파라메터|서브 ?쿼리|하위 질의|느린 질의|슬로우 쿼리|리턴|커멘트|쓰레드|메소드|디렉토리|메세지|데이타|스토리지|디폴트|커넥션|캐쉬|어플리케이션|셧다운|엔클러저|운영체제|소스코드|데이터 베이스" \
  --include='*.rst' --include='*.inc' ko

# 영문 AUTO 항목
grep -rn -E "host name|user name|file name|time zone|fail-over|sub-query|upper[ -]case|white space|multi-byte|look-up|run-time|start-up|log-in|type-cast|data base|meta-data|on-line|back-up|can't|can not|datatype" \
  --include='*.rst' --include='*.inc' ko en
```

검색 결과가 나오면 6절의 **제외 대상인지 먼저 판정**한 뒤 수정합니다.

> `develop` 브랜치 기준 검출 건수입니다.
>
> | 대상 | 검출 |
> |:---|:---:|
> | `ko` 한국어 AUTO | 1,676 |
> | `en` 영문 AUTO | 356 |
> | `ko` 안의 영문 AUTO | 58 |
> | `ko` MANUAL 후보 | 497 |
>
> 이 중 상당수는 오류 메시지 원문, SQL 식별자, API 시그니처, 옵션명이므로
> **검출 건수와 수정 건수는 다릅니다.**
> 자신이 변경한 줄만 책임 범위입니다. 기존 오류의 전수 수정은 별도 표준화 작업으로 처리합니다.

### 1-1단계 — 제외 대상 오검출 걸러내기

이 저장소는 오류 메시지 카탈로그와 SQL·API 참조 비중이 높습니다. 이를 먼저 분리합니다.

```bash
# 오류 메시지 카탈로그 (전체가 제품 출력 원문)
grep -rln --include='*.rst' -E "(에러|컬럼|can't)" ko/admin/error_log*.rst en/admin/error_log*.rst

# 코드 블록 안의 검출 (들여쓰기된 줄)
grep -rn --include='*.rst' --include='*.inc' -E "^\s{3,}.*(에러|컬럼|디렉토리)" ko
```

여기 걸린 항목은 **원문을 유지**합니다.

```rst
ERROR: Column 'a' cannot be null.                    ← 유지 (제품 출력)
해당 컬럼이 NULL을 허용하지 않을 때 발생한다.          ← 칼럼으로 수정 (서술 문장)
```

판정이 애매하면 제외 대상으로 간주해 원문을 유지합니다.
잘못 바꾸는 손해가 안 바꾸는 손해보다 큽니다.

### 2단계 — MANUAL 항목 확인

```bash
grep -rn -E "쿼리|리소스|유저|레퍼런스|툴킷|리스트" --include='*.rst' --include='*.inc' ko
grep -rn -E "UTF-?8|utf-?8|EUC-?KR|euc-?kr|eucKR|Cubrid|centre|indices|auto[- ]commit" --include='*.rst' --include='*.inc' ko en
```

각 건마다 문맥을 판정합니다. 유지하기로 결정했다면 PR 코멘트에 이유를 남깁니다.

### 3단계 — 문체 확인 (`ko`만)

```bash
git diff develop...HEAD -- ko | grep '^+' | grep -E "(합니다|입니다|됩니다)\."
```

검출되면 평서체로 바꿉니다. 캡션·명사형 종결·메시지 인용·릴리스 노트 공지는 예외입니다.

### 4단계 — 빌드 검증

```bash
# 자신이 수정한 부분에서 새 경고가 나오는지 확인
cd ko && make html
```

제목 밑줄 길이, 깨진 `:ref:` 참조, 누락된 이미지, `toctree` 누락은 Sphinx **경고**로 출력됩니다.
**자신이 수정한 파일에서 새로 생긴 경고는 0건이어야 합니다.**

> **경고는 CI가 강제하지 않습니다.**
> `ko/Makefile`과 `en/Makefile`의 `SPHINXOPTS`가 비어 있어 `-W`(경고를 오류로 처리)가 적용되지 않습니다.
> `.github/workflows/check.yml`은 `make html` → `make pdf` → `make linkcheck`를 `ko`/`en` 병렬로 실행하지만,
> **빌드 실패와 깨진 외부 링크만 CI를 실패시키고 Sphinx 경고는 통과합니다.**
> 경고 0건은 작성자가 지켜야 하는 기준이며, CI 초록불이 경고 0건을 뜻하지 않습니다.

수정한 파일의 경고만 걸러 보려면 다음과 같이 확인합니다.

```bash
cd ko && make html 2>&1 | grep -E "WARNING|ERROR" | grep -F "수정한파일명.rst"
```

전체를 경고 없이 빌드하려면 `-W`를 직접 붙입니다.
단 기존 문서에 이미 경고가 있어 저장소 전체 빌드는 실패할 수 있으므로, 자신이 만든 경고와 구분해야 합니다.

```bash
cd ko && make html SPHINXOPTS="-W --keep-going"
```

> `SPHINXOPTS="-W"`를 워크플로 기본값으로 넣을지는 아직 결정되지 않았습니다.
> 기존 문서의 경고를 먼저 정리해야 하므로, 필요하다고 판단되면 그때 Jira 이슈를
> 등록해 진행합니다(12절 절차). 표기 수정 PR에 섞지 않습니다.

### 5단계 — 번역 쌍 확인

```bash
git diff --name-only develop...HEAD | sed -e 's|^ko/|en/|' -e 's|^en/|ko/|' | sort -u
```

기술 사양을 바꿨다면 반대쪽 대응 파일도 함께 수정합니다(8절).

### 6단계 — 변경 범위 확인

```bash
git diff --stat develop...HEAD
```

표기 수정 PR에 기능 설명 변경을 섞지 않습니다. **PR 하나에 목적 하나**입니다.

### 7단계 — PR 본문 작성

표기 수정이 포함된 PR은 다음 형식으로 정리합니다.

```markdown
| 번호 | 오류 표기 | 수정 표기 | 모드 | 건수 |
|:---:|:---:|:---:|:---:|:---:|
| 12 | 디렉토리 | 디렉터리 | AUTO | 23 |
```

MANUAL 항목을 유지했다면 유지 이유를 함께 적습니다.

---

## 11. Greptile 자동 리뷰 연동

이 저장소에는 Greptile 리뷰 봇이 연결되어 있습니다.
PR을 올리면 이 표준을 근거로 자동 리뷰 코멘트가 달립니다.

### 설정 파일 위치

```
.greptile/
├── config.json                  공통 설정 + 공통 표기 규칙 8개
├── rules.md                     공통 표기 기준
└── files.json                   이 문서와 toc.rst를 리뷰 근거로 참조
ko/.greptile/
├── config.json                  한국어 매뉴얼 전용 규칙 + 제외 대상
└── rules.md                     한국어 매뉴얼 전용 기준
en/.greptile/
├── config.json                  영문 매뉴얼 전용 규칙 + 한국어 규칙 비활성화
└── rules.md                     영문 매뉴얼 전용 기준
```

상위 디렉터리 규칙과 하위 디렉터리 규칙은 **덮어쓰지 않고 합쳐집니다.**
`ko/`의 파일을 리뷰할 때는 루트 규칙 + `ko` 규칙이 모두 적용됩니다.

> **`ko/.greptile`, `en/.greptile`는 Sphinx 빌드에서 제외되어 있습니다.**
> `conf.py`의 `source_suffix`가 `.md`를 마크다운 소스로 선언하지만 마크다운 파서는
> 설치되어 있지 않습니다. 두 디렉터리가 Sphinx 소스 트리 안에 있으므로
> `ko/conf.py`·`en/conf.py`의 `exclude_patterns`에 `.greptile`을 추가했습니다.
> 이 항목을 지우면 `make html`이 `.greptile/rules.md`를 소스로 읽으려다 실패합니다.
> 같은 이유로 `ko/`·`en/` 아래에 `.md` 파일을 새로 추가할 때는 제외 여부를 먼저 확인합니다.

`en/`에서는 한국어 규칙 3개(`ko-notation-auto`, `ko-notation-manual`,
`ko-honorific-consistency`)를 `disabledRules`로 껐습니다. 오탐 소음을 줄이기 위한 것입니다.

### 코멘트 형식

```
ASIS: 디렉토리를 생성한다.
TOBE: 디렉터리를 생성한다.
근거: ko-notation-auto (표준 12번)
```

### 봇 지적이 틀렸을 때

제외 대상(6절)을 잘못 지적했다면 **그냥 무시하지 말고** 이유를 답글로 남깁니다.

```
@greptileai 이 부분은 error_log 카탈로그의 제품 출력 메시지라 원문을 유지해야 합니다.
```

봇이 같은 오탐을 반복하면 해당 디렉터리의 `.greptile/config.json`
`instructions`에 제외 조건을 추가하거나, `disabledRules`에 규칙 ID를 넣습니다.

특정 하위 디렉터리에서만 오탐이 반복되면 그 디렉터리에 `.greptile/config.json`을
새로 만드는 것이 더 정확합니다(예: `ko/api/.greptile/`).
처음부터 만들지 않고 **오탐이 실제로 나온 뒤에** 추가합니다.

### 재리뷰

PR에 커밋을 추가하면 자동으로 재리뷰됩니다(`triggerOnUpdates: true`).
수동으로 다시 돌리려면 PR에 `@greptileai` 를 코멘트하거나
리뷰 코멘트 하단의 `Re-trigger Greptile` 링크를 사용합니다.

### 한계

Greptile은 **PR에서 변경된 부분**을 검토합니다.
기존 문서에 이미 들어 있는 오류는 지적되지 않습니다.
전수 수정은 별도 표준화 작업(CUBRIDMAN-343 같은)으로 처리합니다.

또한 LLM 기반이므로 판정이 100% 일정하지 않습니다.
**AUTO 항목의 최종 책임은 작성자에게 있습니다.** 10절 셀프 체크를 생략하지 마십시오.

---

## 12. 표준 변경 절차

표기 기준을 바꾸거나 예외를 추가할 때의 절차입니다.

1. Jira에 이슈를 등록하고 변경 근거를 남깁니다(국립국어원 기준, 제품 UI 변경, 상표 정책 등).
2. 이 문서(`docs/STYLE_GUIDE.ko.md`)를 먼저 수정합니다.
3. `.greptile/rules.md`와 관련 `config.json`의 규칙 문장을 동일하게 반영합니다.
4. 기존 문서의 전수 수정이 필요한지 판단하고, 필요하면 별도 PR로 분리합니다.
5. 하나의 PR에서 3~4를 함께 처리하지 않습니다.

### 규칙 ID 목록

`disabledRules`로 예외 처리할 때 사용하는 ID입니다.

| 위치 | 규칙 ID | 심각도 | 적용 범위 |
|:---|:---|:---:|:---|
| 루트 | `ko-notation-auto` | high | `ko` |
| 루트 | `ko-notation-manual` | low | `ko` |
| 루트 | `en-notation-auto` | high | `ko`, `en` |
| 루트 | `en-notation-word` | high | `ko`, `en` |
| 루트 | `en-notation-manual` | low | `ko`, `en` |
| 루트 | `encoding-notation-context` | medium | `ko`, `en` |
| 루트 | `ko-honorific-consistency` | low | `ko` |
| 루트 | `ko-en-parallel-sync` | medium | `ko`, `en` |
| ko | `ko-error-message-verbatim` | high | `ko` |
| ko | `ko-sql-syntax-guard` | medium | `ko/sql` |
| ko | `ko-api-identifier-guard` | medium | `ko/api`, `ko/pl` |
| ko | `ko-admin-config-guard` | medium | `ko/admin` |
| ko | `ko-procedure-safety` | medium | `ko/admin`, `ko/ha.rst`, `ko/shard.rst` |
| ko | `ko-release-note-verbatim` | low | `ko/release_note` |
| en | `en-no-contractions` | high | `en` |
| en | `en-sql-syntax-guard` | medium | `en/sql` |
| en | `en-option-name-guard` | high | `en` |
| en | `en-error-message-verbatim` | high | `en` |
| en | `en-translation-clarity` | low | `en` |

특정 디렉터리에서만 규칙을 끄려면 그 디렉터리의 `.greptile/config.json`에 다음을 추가합니다.

```json
{
  "disabledRules": ["en-notation-word"]
}
```
