# CUBRID 영문 매뉴얼 리뷰 기준 (`en`)

저장소 공통 기준은 `../.greptile/rules.md`, 정본은 `../docs/STYLE_GUIDE.ko.md`입니다.
이 파일은 영문 매뉴얼에만 추가로 적용되는 기준입니다.

---

## 1. 비활성화된 공통 규칙

이 디렉터리에는 한국어 서술 문장이 없습니다. 다음 규칙은 `disabledRules`로 껐습니다.

| 규칙 ID | 이유 |
|:---|:---|
| `ko-notation-auto` | 한국어 표기 대상 문장 없음 |
| `ko-notation-manual` | 위와 동일 |
| `ko-honorific-consistency` | 한국어 문체 규칙 |

적용되는 공통 규칙은 다음입니다.

| 규칙 ID | 심각도 |
|:---|:---:|
| `en-notation-auto` | high |
| `en-notation-word` | high |
| `en-notation-manual` | low |
| `encoding-notation-context` | medium |
| `ko-en-parallel-sync` | medium |

---

## 2. 영문 표기 규칙 현황

`develop` 기준 검출 건수입니다. **검출 건수와 수정 건수는 다릅니다** — 상당수가 옵션명·식별자입니다.

| 표기 | 검출 | 판정 |
|:---|:---:|:---|
| host name | 117 | 서술 문장만 `hostname`. `--host-name` 옵션명은 유지 |
| user name | 95 | 서술 문장만 `username`. `user_name` 파라미터·칼럼명은 유지 |
| time zone | 54 | `timezone`. `timezone` 파라미터명과 구분 |
| auto-commit / auto commit | 52 / 12 | `autocommit`. `;autocommit`, `--no-auto-commit`, API 상수는 유지 |
| file name | 51 | `filename` |
| fail-over | 36 | `failover` |
| datatype | 15 | WORD 판정. SQL 타입 표기·API 식별자는 유지 |
| can not | 14 | `cannot` |
| can't | 13 | `cannot` |
| sub-query | 13 | `subquery` |
| meta-data | 8 | `metadata` |
| upper case / upper-case | 6 / 3 | `uppercase` |
| white space | 6 | `whitespace` |
| multi-byte | 6 | `multibyte` |
| look-up | 4 | `lookup` |
| centre | 4 | MANUAL. 예제 데이터 값은 유지 |
| indices | 3 | WORD. 수학적 지수 문맥은 유지 |
| on-line | 3 | `online` |
| run-time / start-up / log-in / type-cast | 2 each | `runtime` / `startup` / `login` / `typecast` |
| back-up / data base | 1 each | `backup` / `database` |
| Cubrid | 8 | MANUAL. 클래스명·SQL 예제는 유지 |

---

## 3. 축약형 사용 금지

```rst
The user can't access the table.   (X)
The user cannot access the table.  (O)
```

`can't`, `don't`, `won't`, `isn't`, `doesn't`, `it's` 를 풀어 씁니다.
제품 출력 오류 메시지 원문, 코드 블록, 예제 데이터에 포함된 축약형은 유지합니다.

---

## 4. 옵션명·식별자 판정

가장 오탐이 많은 지점입니다. **서술 문장에만** 적용합니다.

```rst
Specify the host name of the target server.
  → Specify the hostname of the target server.        (서술 문장 → 수정)

cubrid broker start --host-name=node1
  → 원문 유지                                          (옵션명 → 유지)

The ha_mode parameter enables fail-over.
  → The ha_mode parameter enables failover.            (서술 → 수정, 파라미터명은 유지)

CCI_PROP_AUTOCOMMIT
  → 원문 유지                                          (API 상수 → 유지)
```

---

## 5. 오류 메시지 원문은 수정하지 않습니다

`admin/error_log*.rst`의 메시지와 오류 코드는 제품이 출력하는 문자열입니다.

```rst
-- 유지
ERROR: Can't open the file name specified.

-- 수정
This error occurs when the specified file name does not exist.
  → ... when the specified filename does not exist.
```

**판정이 애매하면 메시지 원문으로 간주해 유지합니다.**

---

## 6. 번역 품질

한국어 원문 직역으로 의미가 불명확한 문장을 지적합니다.

| 확인 항목 |
|:---|
| 주어 누락 |
| 관사 누락으로 의미가 달라지는 경우 |
| 조건절과 주절의 관계가 어긋난 문장 |
| 한국어 어순을 그대로 옮겨 수식 관계가 모호해진 문장 |

문장 표현 취향, 동의어 선택은 지적하지 않습니다.
이 PR에서 변경되지 않은 문장은 대상이 아닙니다.

---

## 7. ko 대응 파일

기술 사양이 바뀌었다면 `ko`의 대응 파일도 함께 갱신되어야 합니다.
경로는 `en/` ↔ `ko/` 만 다르고 나머지는 동일합니다.

```
en/sql/query/select.rst  ↔  ko/sql/query/select.rst
en/admin/config.rst      ↔  ko/admin/config.rst
```

자세한 기준은 공통 기준 5절(`../.greptile/rules.md`)을 따릅니다.
