:meta-keywords: cubrid json, json functions, database json, JSON_TABLE
:meta-description: CUBRID functions that create, query and modify JSON data.

:tocdepth: 3

.. _json-fn:

*********
JSON 함수
*********

.. _fn-json-intro:

JSON 함수 소개
===================================

이 섹션에서는 JSON 데이터 관련 동작에 대해 기술한다.
지원하는 JSON 함수들은 다음 표와 같다:

+------------------+------------------+------------------+------------------+
| :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  |
| \arrow`          | \contains-path`  | \merge-patch`    | \replace`        |
+------------------+------------------+------------------+------------------+
| :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  |
| \double-arrow`   | \depth`          | \merge-preserve` | \search`         |
+------------------+------------------+------------------+------------------+
| :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  |
| \array`          | \extract`        | \object`         | \set`            |
+------------------+------------------+------------------+------------------+
| :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  |
| \arrayagg`       | \insert`         | \objectagg`      | \table`          |
+------------------+------------------+------------------+------------------+
| :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  |
| \array-append`   | \keys`           | \pretty`         | \type`           |
+------------------+------------------+------------------+------------------+
| :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  |
| \array-insert`   | \length`         | \quote`          | \unquote`        |
+------------------+------------------+------------------+------------------+
| :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  | :ref:`fn-json-\  |
| \contains`       | \merge`          | \remove`         | \valid`          |
+------------------+------------------+------------------+------------------+

함수의 입력 인자는 아래와 같은 몇가지 유형을 가진다.

  - *json_doc*: JSON이나 JSON으로 파싱되는 문자열
  - *val*: JSON이나 JSON 지원 스칼라 타입 중 하나로 해석될 수 있는 값
  - *json key*: 키 이름으로서의 문자열
  - *json path/pointer*: :ref:`json-path` 와 :ref:`json-pointer` 에 설명된 규칙을 따르는 문자열

.. note::

  JSON 함수 문자열 인자의 코드셋은 UTF8을 기준으로 한다. 다른 코드셋의 입력 문자열은 UTF8로 변환된다.
  UTF8이 아닌 코드셋 문자열에 대한 대소문자 구별 없는 검색은 기대와 다른 결과가 나올 수 있다.

다음의 표는 입력 인자를 해석하는데 있어서 *json_doc* 와 *val* 의 차이를 보여주고 있다:

+-------------------+-----------------------------+----------------------------------+
| 입력 타입         | *json_doc*                  | *val*                            |
+===================+=============================+==================================+
| JSON              | 입력 값이 변하지 않음       | 입력 값이 변하지 않음            |
+-------------------+-----------------------------+----------------------------------+
| String            | JSON 입력 값이 파싱됨       | 입력 값이 JSON STRING으로 변환됨 |
+-------------------+-----------------------------+----------------------------------+
| Short, Integer    | 변환 오류                   | 입력 값이 JSON INTEGER로 변환됨  |
+-------------------+-----------------------------+----------------------------------+
| Bigint            | 변환 오류                   | 입력 값이 JSON BIGINT로 변환됨   |
+-------------------+-----------------------------+----------------------------------+
| Float, Double,    | 변환 오류                   | 입력 값이 JSON DOUBLE로 변환됨   |
+-------------------+-----------------------------+----------------------------------+
| NULL              | NULL                        | 입력 값이 JSON_NULL로 변환됨     |
+-------------------+-----------------------------+----------------------------------+
| Other             | 변환 오류                   | 변환 오류                        |
+-------------------+-----------------------------+----------------------------------+

.. _fn-json-array:

JSON_ARRAY
===================================

.. function:: JSON_ARRAY ([val1 [ , val2] ...])

  **JSON_ARRAY** 함수는 해당 값들(val, val2, ..)을 가진 리스트(텅빈 리스트도 가능)가 포함된 json 배열을 반환한다.

.. code-block:: sql

    SELECT JSON_ARRAY();

::

      json_array()
    ======================
      []

.. code-block:: sql

    SELECT JSON_ARRAY(1, '1', json '{"a":4}', json '[1,2,3]');

::

      json_array(1, '1', json '{"a":4}', json '[1,2,3]')
    ======================
      [1,"1",{"a":4},[1,2,3]]

.. _fn-json-object:

JSON_OBJECT
===================================

.. function:: JSON_OBJECT ([key1, val1 [ , key2, val2] ...])

  **JSON_OBJECT** 함수는 해당 키/값(key, val1, key, val2,...)쌍을 가진 리스트(텅빈 리스트도 가능)가 포함된 json 객체를 반환한다.

.. code-block:: sql

    SELECT JSON_OBJECT();

::

      json_object()
    ======================
      {}

.. code-block:: sql

    SELECT JSON_OBJECT('a', 1, 'b', '1', 'c', json '{"a":4}', 'd', json '[1,2,3]');

::

      json_object('a', 1, 'b', '1', 'c', json '{"a":4}', 'd', json '[1,2,3]')
    ======================
      {"a":1,"b":"1","c":{"a":4},"d":[1,2,3]}

.. _fn-json-keys:

JSON_KEYS
===================================

.. function:: JSON_KEYS (json_doc [ , json path])

  **JSON_KEYS** 함수는 해당 패스로 주어진 json 객체의 모든 키값을 가진 json 배열을 반환한다.
  해당 경로가 json 객체가 아닌 json 요소를 지정하면 json null이 반환된다.
  json 경로 인자가 누락되면 키(key)는 json 루트 요소로부터 가져온다.
  *json 경로* 가 존재하지 않으면 오류가 발생하고 *json_doc* 인자가 **NULL** 이면 **NULL** 을 반환한다.

.. code-block:: sql

    SELECT JSON_KEYS('{}');

::

      json_keys('{}')
    ======================
      []

.. code-block:: sql

    SELECT JSON_KEYS('"non-object"');

::

      json_keys('"non-object"')
    ======================
      null

.. code-block:: sql

    SELECT JSON_KEYS('{"a":1, "b":2, "c":{"d":1}}');

::

      json_keys('{"a":1, "b":2, "c":{"d":1}}')
    ======================
      ["a","b","c"]

.. _fn-json-depth:

JSON_DEPTH
===================================

.. function:: JSON_DEPTH (json_doc)

  **JSON_DEPTH** 함수는 json의 최대 깊이를 반환한다.
  깊이는 1부터 시작하며 깊이 레벨은 비어있지 않은 json 배열이나 비어있지 않은 json 객체에서 1씩 증가한다. 
  인자가 **NULL** 이면 **NULL** 을 반환한다.

.. code-block:: sql

    SELECT JSON_DEPTH('"scalar"');

::

      json_depth('"scalar"')
    ======================
      1

.. code-block:: sql

    SELECT JSON_DEPTH('[{"a":4}, 2]');

::

      json_depth('[{"a":4}, 2]')
    ======================
      3

[예제] deeper json:

.. code-block:: sql

    SELECT JSON_DEPTH('[{"a":[1,2,3,{"k":[4,5]}]},2,3,4,5,6,7]');

::

      json_depth('[{"a":[1,2,3,{"k":[4,5]}]},2,3,4,5,6,7]')
    ======================
      6

.. _fn-json-length:

JSON_LENGTH
===================================

.. function:: JSON_LENGTH (json_doc [ , json path])

  **JSON_LENGTH** 함수는 주어진 경로에 있는 json 요소의 길이를 반환한다.
  경로 인자가 주어지지 않으면 josn 루트 요소의 길이가 반환된다.
  인자가 **NULL** 이거나 해당 경로에 어떤 요소도 존재하지 않으면 **NULL** 이 반환된다.

.. code-block:: sql

    SELECT JSON_LENGTH('"scalar"');

::

      json_length('"scalar"')
    ======================
      1

.. code-block:: sql

    SELECT JSON_LENGTH('[{"a":4}, 2]', '$.a');

::

      json_length('[{"a":4}, 2]', '$.a')
    ======================
      NULL

.. code-block:: sql

    SELECT JSON_LENGTH('[2, {"a":4, "b":4, "c":4}]', '$[1]');

::

      json_length('[2, {"a":4, "b":4, "c":4}]', '$[1]')
    ======================
      3

.. code-block:: sql

    SELECT JSON_LENGTH('[{"a":[1,2,3,{"k":[4,5,6,7,8]}]},2]');

::

      json_length('[{"a":[1,2,3,{"k":[4,5,6,7,8]}]},2]')
    ======================
      2

.. _fn-json-valid:

JSON_VALID
===================================

.. function:: JSON_VALID (val)

  **JSON_VALID** 함수는 해당 *val* 인자가 유효한 json_doc일 경우에 1을 그렇지 않은 경우에 0을 반환한다.
  해당 인자가 **NULL** 인 경우 **NULL** 을 반환한다.

.. code-block:: sql

    SELECT JSON_VALID('[{"a":4}, 2]');
    1
    SELECT JSON_VALID('{"wrong json object":');
    0

.. _fn-json-type:

JSON_TYPE
===================================

.. function:: JSON_TYPE (json_doc)

  **JSON_TYPE** 함수는 문자열 인자인 *json_doc* 의 타입을 반환한다.

.. code-block:: sql

    SELECT JSON_TYPE ('[{"a":4}, 2]');
    'JSON_ARRAY'
    SELECT JSON_TYPE ('{"a":4}');
    'JSON_OBJECT'
    SELECT JSON_TYPE ('"aaa"');
    'STRING'

.. _fn-json-quote:

JSON_QUOTE
===================================

.. function:: JSON_QUOTE (str)

  **JSON_QUOTE** 함수는 문자열과 이스케이프된 특수 문자들을 큰따옴표로 묶은 json_string을 결과로 반환한다.
  *str* 인자가 **NULL** 인 경우 **NULL** 을 반환한다.

.. code-block:: sql

    SELECT JSON_QUOTE ('simple');

::

      json_unquote('simple')
    ======================
      '"simple"'

.. code-block:: sql

    SELECT JSON_QUOTE ('"');

::

      json_unquote('"')
    ======================
      '"\""'

.. _fn-json-unquote:

JSON_UNQUOTE
===================================

.. function:: JSON_UNQUOTE (json_doc)

  **JSON_UNQUOTE** 함수는 따옴표로 묶이지 않은 json_value 문자열을 반환한다.
  *json_doc* 인자가 **NULL** 이면 **NULL** 을 반환한다.

.. code-block:: sql

    SELECT JSON_UNQUOTE ('"\\u0032"');

::

      json_unquote('"\u0032"')
    ======================
      '2'

.. code-block:: sql

    SELECT JSON_UNQUOTE ('"\\""');

::

      json_unquote('"\""')
    ======================
      '"'

.. _fn-json-pretty:

JSON_PRETTY
===================================

.. function:: JSON_PRETTY (json_doc)

  **JSON_PRETTY** 는 *json_doc* 보기좋게 출력된 문자열을 반환한다.
  *json_doc* 인자가 **NULL** 이면 **NULL** 을 반환한다.

.. code-block:: sql

    SELECT JSON_PRETTY('[{"a":"val1", "b":"val2", "c": [1, "elem2", 3, 4, {"key":"val"}]}]');

::

      json_pretty('[{"a":"val1", "b":"val2", "c": [1, "elem2", 3, 4, {"key":"val"}]}]')
    ======================
      '[
      {
        "a": "val1",
        "b": "val2",
        "c": [
          1,
          "elem2",
          3,
          4,
          {
            "key": "val"
          }
        ]
      }
    ]'

.. _fn-json-search:

JSON_SEARCH
===================================

.. function:: JSON_SEARCH (json_doc, one/all, search_str [, escape_char [, json path] ...])

  **JSON_SEARCH** 함수는 해당 *search_str* 과 일치하는 json 문자열을 포함한 하나의 json 경로 혹은 복수의 json 경로를 반환한다.
  일치 여부 검사는 내부의 json 문자열과 *search_str* 에 **LIKE** 연산자를 적용하여 수행된다. **JSON_SEARCH** 의 *escape_char* 및 *search_str* 에 대해 **LIKE** 연산자의 대응 부분과 동일한 규칙이 적용된다.
  **LIKE** 관련 규칙에 대한 추가 설명은 :ref:`like-expr` 을 참고한다.

  one/all에서 'one'을 사용하면 **JSON_SEARCH** 첫번째 일치가 나타났을 때 탐색이 멈추게 된다.
  반면에 'all'을 사용하면 *search_str* 과 일치하는 모든 경로를 탐색하게 된다.

  주어진 json 경로는 반환 된 경로의 필터를 결정하므로 결과로 나온 json 경로의 접두사(prefix)는 적어도 하나의 주어진 json 경로 인자와 일치해야 한다.
  json 경로 인자가 누락된 경우, **JSON_SEARCH** 는 루트 요소로 부터 탐색을 시작한다.

.. code-block:: sql

    SELECT JSON_SEARCH('{"a":["a","b"],"b":"a","c":"a"}', 'one', 'a');

::

      json_search('{"a":["a","b"],"b":"a","c":"a"}', 'one', 'a')
    ======================
      "$.a[0]"

.. code-block:: sql

    SELECT JSON_SEARCH('{"a":["a","b"],"b":"a","c":"a"}', 'all', 'a');

::

      json_search('{"a":["a","b"],"b":"a","c":"a"}', 'all', 'a')
    ======================
      "["$.a[0]","$.b","$.c"]"

.. code-block:: sql

    SELECT JSON_SEARCH('{"a":["a","b"],"b":"a","c":"a"}', 'all', 'a', NULL, '$.a', '$.b');

::

      json_search('{"a":["a","b"],"b":"a","c":"a"}', 'all', 'a', null, '$.a', '$.b')
    ======================
      "["$.a[0]","$.b"]"

와일드카드는 좀더 일반적인 형식의 경로 필터로 사용될 수 있다.
json 경로는 객체 키 식별자로 시작하는 것만 허용된다.

.. code-block:: sql

    SELECT JSON_SEARCH('{"a":["a","b"],"b":"a","c":"a"}', 'all', 'a', NULL, '$.*');

::

      json_search('{"a":["a","b"],"b":"a","c":"a"}', 'all', 'a', null, '$.*')
    ======================
      "["$.a[0]","$.b","$.c"]"

객체 키(key) 식별자로 시작하고 json 배열 인덱스를 따르는 json 경로만 허용함으로써 '$.b', '$.d.e[0]' 일치 항목이 필터링 된다:

.. code-block:: sql

    SELECT JSON_SEARCH('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', 'all', 'a', NULL, '$.*[*]');

::

      json_search('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', 'all', 'a', null, '$.*[*]')
    ======================
      "["$.a[0]","$.c[0]"]"

json 배열 인덱스를 포함하는 json 경로만 허용함으로써 '$.b'가 필터링 된다.

.. code-block:: sql

    SELECT JSON_SEARCH('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', 'all', 'a', NULL, '$**[*]');

::

      json_search('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', 'all', 'a', null, '$**[*]')
    ======================
      "["$.a[0]","$.c[0]","$.d.e[0]"]"

.. _fn-json-extract:

JSON_EXTRACT
===================================

.. function:: JSON_EXTRACT (json_doc, json path [, json path] ...)

  해당 경로로 지정된 *json_doc* 로부터 json 요소를 반환한다.
  json 경로 인자가 와일드카드를 포함하는 경우 와일드카드에 의해 포함될 수 있는 모든 경로의 지정된 json 요소가 json 배열 결과로 반환된다.
  와일드카드를 사용하지 않고 json 경로에서 하나의 요소만 발견된 경우 하나의 json 요소만 반환되며, 그렇지 않은 경우 발견된 json 요소는 json 배열로 구성하여 반환된다.
  json 경로가 **NULL** 이거나 유효하지 않은 경우 혹은 *json_doc* 인자가 유효하지 않은 경우 에러가 반환된다.
  json 요소가 발견되지 않거나 json_doc이 **NULL** 인 경우 **NULL** 을 반환한다.

.. code-block:: sql

    SELECT JSON_EXTRACT('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$.a');

::

      json_extract('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$.a')
    ======================
      "["a","b"]" -- at '$.a' we have the json array ["a","b"] 

.. code-block:: sql

    SELECT JSON_EXTRACT('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$.a[*]');

::

      json_extract('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$.a[*]')
    ======================
      "["a","b"]" -- '$.a[0]'와 '$.a[1]'는 json 배열로 구성하여, ["a","b"]를 형성한다.

와일드 카드'.*'를 포함한 이전의 쿼리를 '.a'로 바꾸면 '$.c[0]'가 일치할 것인데, 이것은 정확히 객체 키(key) 식별자와 배열 인덱스가 있는 모든 json 경로와 일치할 것이다.

.. code-block:: sql

    SELECT JSON_EXTRACT('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$.*[*]');

::

      json_extract('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$.*[*]')
    ======================
      "["a","b","a"]"

다음 json 경로는 json 배열 인덱스로 끝나는 모든 json 경로와 일치할 것이다 (이전의 일치하는 모든 경로 및 '$ .d.e [0]'과 일치):

.. code-block:: sql

    SELECT JSON_EXTRACT('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$**[*]');

::

      json_extract('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$**[*]')
    ======================
      "["a","b","a","a"]"

.. code-block:: sql

    SELECT JSON_EXTRACT('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$.d**[*]');

::

      json_extract('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$d**[*]')
    ======================
	  "["a"]" -- '$.d.e[0]'은 해당 인자의 경로 패밀리와 일치하는 유일한 경로이며, .d'로 시작하고 배열 인덱스로 끝나는 경로이다.

.. _fn-json-arrow:

->
===================================

.. function:: json_doc -> json path

  *json_doc* 인자가 하나의 컬럼으로 제한된 두 개의 인자를 가지는 **JSON_EXTRACT** 의 별칭 연산자.
  json 경로가 **NULL** 이거나 유효하지 않은 경우 오류를 반환한다.
  **NULL** *josn_doc* 인자가 적용된 경우에는 **NULL** 을 반환한다.

.. code-block:: sql

    CREATE TABLE tj (a json);
    INSERT INTO tj values ('{"a":1}'), ('{"a":2}'), ('{"a":3}'), (NULL);

    SELECT a->'$.a' from tj;

::

      json_extract(a, '$.a')
    ======================
      1
      2
      3
      NULL

.. _fn-json-double-arrow:

->>
===================================

.. function:: json_doc ->> json path

   **JSON_UNQUOTE** 의 별칭 (json_doc->json 경로). 본 연산자는 컬럼인 *json_doc* 인자에만 적용 할 수 있다.
   json 경로가 **NULL** 이거나 유효하지 않은 경우 오류가 발생한다.
   **NULL** *json_doc* 인자에 적용된 경우 **NULL** 을 반환한다.

.. code-block:: sql

    CREATE TABLE tj (a json);
    INSERT INTO tj values ('{"a":1}'), ('{"a":2}'), ('{"a":3}'), (NULL);

    SELECT a->>'$.a' from tj;

::

      json_unquote(json_extract(a, '$.a'))
    ======================
      '1'
      '2'
      '3'
      NULL

.. _fn-json-contains-path:

JSON_CONTAINS_PATH
===================================

.. function:: JSON_CONTAINS_PATH (json_doc, one/all, json path [, json path] ...)

  **JSON_CONTAINS_PATH** 함수는 해당 경로가 *json_doc* 내에 존재하는지를 검사한다.

  one/all 인자 중 'all'이 적용된 경우 모든 경로가 존재하면 1을 반환하고 그렇지 않으면 0을 반환한다.
  
  one/all 인자 중 'one'이 적용된 경우 하나의 경로라도 존재하면 1을 반환하고 그렇지 않으면 0을 반환한다.

  해당 인자가 **NULL** 이면 **NULL** 을 반환한다.
  해당 인자가 유효하지 않으면 오류가 발생한다.

.. code-block:: sql

    SELECT JSON_CONTAINS_PATH ('[{"0":0},1,"2",{"three":3}]', 'all', '$[0]', '$[0]."0"', '$[1]', '$[2]', '$[3]');

::

      json_contains_path('[{"0":0},1,"2",{"three":3}]', 'all', '$[0]', '$[0]."0"', '$[1]', '$[2]', '$[3]')
    ======================================================================================================
                                                                                                         1

.. code-block:: sql

    SELECT JSON_CONTAINS_PATH ('[{"0":0},1,"2",{"three":3}]', 'all', '$[0]', '$[0]."0"', '$[1]', '$[2]', '$[3]', '$.inexistent');

::

      json_contains_path('[{"0":0},1,"2",{"three":3}]', 'all', '$[0]', '$[0]."0"', '$[1]', '$[2]', '$[3]', '$.inexistent')
    ======================================================================================================================
                                                                                                                         0

**JSON_CONTAINS_PATH** 함수는 json 경로 내에 와일드카드를 지원한다.

.. code-block:: sql

    SELECT JSON_CONTAINS_PATH ('[{"0":0},1,"2",{"three":3}]', 'one', '$.inexistent', '$[*]."three"');

::

     json_contains_path('[{"0":0},1,"2",{"three":3}]', 'one', '$.inexistent', '$[*]."three"')
    ==========================================================================
                                                                             1

.. _fn-json-contains:

JSON_CONTAINS
===================================

.. function:: JSON_CONTAINS (json_doc doc1, json_doc doc2 [, json path])

  **JSON_CONTAINS** 함수는 *doc2* 가 옵션으로 지정된 경로의 *doc1* 에 포함되는지를 검사한다.
  다음과 같이 재귀 규칙이 충족되는 경우 json 요소에 다른 json 요소가 포함된다.

  - 타입이 같고 (**JSON_TYPE** ()이 일치하고) 스칼라도 같은 경우 json 스칼라에 다른 json 스칼라가 포함된다. 예외적으로, json integer는 **JSON_TYPE** ()이 다른 경우에도 json double과 비교를 통해 동일한 것으로 간주될 수 있다.
  - json 배열 요소에 json_nonarray가 포함되어 있으면 json 배열에 json 스칼라 또는 json 객체가 포함된다.
  - 두 번째 json 배열의 모든 요소가 첫 번째 json 배열에 포함되어 있으면 json 배열에 다른 json 배열이 포함된다.
  - 두 번째 객체의 모든 (*key2*, *value2*) 쌍에 대해 첫 번째 객체에 *key1* = *key2* 이고 *value2* 가 *value1* 을 포함하는 (*key1*, *value1*) 쌍이 있는 경우 json 객체에는 다른 json 오브젝트가 포함된다.
  - 이 외에는 json 요소가 포함되지 않는다.

  json 경로 인자를 입력하지 않은 경우 *doc2* 가 *doc1* 의 루트 json 요소에 포함되는지 여부를 반환한다.
  인자가 **NULL** 이면 **NULL** 을 반환한다.
  인자가 유효하지 않은 경우 오류가 발생한다.

.. code-block:: sql

    SELECT JSON_CONTAINS ('"simple"','"simple"');

::

      json_contains('"simple"', '"simple"')
    =======================================
                                          1

.. code-block:: sql

    SELECT JSON_CONTAINS ('["a", "b"]','"b"');

::

      json_contains('["a", "b"]', '"b"')
    ====================================
                                       1

.. code-block:: sql

    SELECT JSON_CONTAINS ('["a", "b1", ["a", "b2"]]','["b1", "b2"]');

::

      json_contains('["a", "b1", ["a", "b2"]]','["b1", "b2"]')
    ==========================================================
                                                             1

.. code-block:: sql

    SELECT JSON_CONTAINS ('{"k1":["a", "b1"], "k2": ["a", "b2"]}','{"k1":"b1", "k2":"b2"}');

::

      json_contains('{"k1":["a", "b1"], "k2": ["a", "b2"]}','{"k1":"b1", "k2":"b2"}')
    =================================================================================
                                                                                    1

json 객체는 json 배열과 같은 방식으로 포함을 검사하지 않으며, json 객체의 하위 요소에 포함된 json 객체의 자손이 아닌 json 요소를 가질 수 없다.

.. code-block:: sql

    SELECT JSON_CONTAINS ('["a", "b1", ["a", {"k":"b2"}]]','["b1", "b2"]');

::

      json_contains('["a", "b1", ["a", {"k":"b2"}]]','["b1", "b2"]')
    ================================================================
                                                                   0

.. code-block:: sql

    SELECT JSON_CONTAINS ('["a", "b1", ["a", {"k":["b2"]}]]','["b1", {"k":"b2"}]');

::

      json_contains('["a", "b1", ["a", {"k":["b2"]}]]','["b1", {"k":"b2"}]')
    ========================================================================
                                                                           1

.. _fn-json-merge-patch:

JSON_MERGE_PATCH
===================================

.. function:: JSON_MERGE_PATCH (json_doc, json_doc [, json_doc] ...)

**JSON_MERGE_PATCH** 함수는 둘 이상의 json 문서를 병합하고 병합된 결과 json을 반환한다. **JSON_MERGE_PATCH** 는 병합 충돌 시 두 번째 인자를 사용하는 점에서 **JSON_MERGE_PRESERVE** 와 다르다. 
**JSON_MERGE_PATCH** 함수는 `RFC 7396 <https://www.rfc-editor.org/info/rfc7396>` 을 준수한다.

두 개의 json 문서 병합은 다음 규칙에 따라 재귀적으로 수행된다:

- 두 개의 비 객체 JSON이 병합되면 병합 결과는 두 번째 값이다.
- 객체가 아닌 json이 json 객체와 병합되면 빈 객체와 두 번째 병합 인자가 병합된다.
- 두 객체가 병합되면 결과 객체는 다음 멤버로 구성된다:

  - 두 번째 객체와 동일한 키를 가진 멤버를 제외한 첫 번째 객체의 모든 멤버.
  - 첫 번째 객체에 동일한 키를 가진 멤버와 모든 멤버가 null인 값을 제외한 두번째 객체의 모든 멤버. 두 번째 객체의 null 값을 가진 멤버는 무시된다.
  - 두 번째 객체와 동일한 키를 가진 null이 아닌 첫 번째 객체의 멤버. 두 객체 모두에 나타나는 동일한 키 멤버일 때 두 번째 객체의 멤버 값이 null이면 무시된다. 이 두 개의 값은 첫 번째 및 두 번째 객체의 멤버 값에 대해 수행된 병합의 결과가 된다.

병합 작업은 두 개 이상의 인자가 있을 때 연속적으로 실행된다. 처음 두 인자를 병합한 결과는 세 번째와 병합되고 이 결과는 네 번째와 병합된다.

인자가 **NULL** 이면 **NULL** 을 반환한다.
인자가 유효하지 않은 경우 오류가 발생한다.

.. code-block:: sql

    SELECT JSON_MERGE_PATCH ('["a","b","c"]', '"scalar"');

::

      json_merge_patch('["a","b","c"]', '"scalar"')
    ======================
      "scalar"


첫 번째 인자가 객체가 아니고 두 번째 인자가 객체 인 경우 병합에 대한 예외. 빈 객체와 두 번째 객체 인자 간에 병합 작업이 수행된다.

.. code-block:: sql

    SELECT JSON_MERGE_PATCH ('["a"]', '{"a":null}');

::

      json_merge_patch('["a"]', '{"a":null}')
    ======================
      {}

기술된 객체 병합 예시:

.. code-block:: sql

    SELECT JSON_MERGE_PATCH ('{"a":null,"c":["elem"]}','{"b":null,"c":{"k":null},"d":"elem"}');

::

      json_merge_patch('{"a":null,"c":["elem"]}', '{"b":null,"c":{"k":null},"d":"elem"}')
    ======================
      {"a":null,"c":{},"d":"elem"}

.. _fn-json-merge-preserve:

JSON_MERGE_PRESERVE
===================================

.. function:: JSON_MERGE_PRESERVE (json_doc, json_doc [, json_doc] ...)

  **JSON_MERGE_PRESERVE** 함수는 둘 이상의 json 문서를 병합하고 병합된 결과 json을 반환한다. **JSON_MERGE_PRESERVE** 는 병합 충돌 시 두 json 요소를 모두 보존한다는 점에서 **JSON_MERGE_PATCH** 와 다르다.

  두 json 문서의 병합은 다음 규칙에 따라 재귀적으로 수행된다:
  
  - 두 개의 json 배열이 병합되면 연결된다.
  - 두 개의 비 배열 (스칼라 / 오브젝트) json 요소가 병합되고 그 중 하나만 json 객체인 경우 결과는 두 개의 json 요소를 포함하는 배열이다.
  - 비 배열 json 요소가 json 배열과 병합되면 비 배열은 단일 요소 json 배열로 변경된 다음 json 배열 병합 규칙에 따라 json 배열과 병합된다.
  - 두 개의 json 객체가 병합되면 다른 json 객체와 비교해 없는 모든 멤버가 유지된다. 일치하는 키의 경우 규칙을 재귀적으로 적용하여 값이 항상 병합된다.

  병합 작업은 두 개 이상의 인자가 있을 때 연속적으로 실행된다. 처음 두 인자를 병합 한 결과는 세 번째와 병합되고 이 결과는 네 번째와 병합된다.

  인자가 **NULL** 이면 **NULL** 을 반환한다.
  인자가 유효하지 않은 경우 오류가 발생한다.

.. code-block:: sql

    SELECT JSON_MERGE_PRESERVE ('"a"', '"b"');

::

      json_merge('"a"', '"b"')
    ======================
      ["a","b"]

.. code-block:: sql

    SELECT JSON_MERGE_PRESERVE ('["a","b","c"]', '"scalar"');

::

      json_merge('["a","b","c"]', '"scalar"')
    ======================
      ["a","b","c","scalar"]


**JSON_MERGE_PATCH** 와 달리 **JSON_MERGE_PRESERVE** 는 병합하는 동안 첫 번째 인자의 요소를 삭제 및 수정하지 않고 합쳐서 가져온다.

.. code-block:: sql

    SELECT JSON_MERGE_PRESERVE ('{"a":null,"c":["elem"]}','{"b":null,"c":{"k":null},"d":"elem"}');

::

      json_merge('{"a":null,"c":["elem"]}','{"b":null,"c":{"k":null},"d":"elem"}')
    ======================
      {"a":null,"c":["elem",{"k":null}],"b":null,"d":"elem"}

.. _fn-json-merge:

JSON_MERGE
===================================

.. function:: JSON_MERGE (json_doc, json_doc [, json_doc] ...)

  **JSON_MERGE** 는 **JSON_MERGE_PRESERVE** 의 별칭이다.

.. _fn-json-array-append:

JSON_ARRAY_APPEND
===================================

.. function:: JSON_ARRAY_APPEND (json_doc, json path, json_val [, json path, json_val] ...)

  **JSON_ARRAY_APPEND** 함수는 첫 번째 인자의 수정된 사본을 반환한다. 주어진 각 <*json path*, *json_val*> 에 대해 함수는 해당 경로로 지정된 json 배열에 값을 추가한다.

  (*json path*, *json_val*) 은 왼쪽에서 오른쪽으로 하나씩 평가한다. 한 쌍을 평가하여 작성된 문서는 다음 쌍을 평가하는 새로운 값이 된다.

  json 경로가 *json_doc* 내부의 json 배열을 가리키는 경우 *json_val* 이 배열의 끝에 추가된다.
  json 경로가 비 배열 json 요소를 지정하는 경우 비 배열 요소를 포함하는 단일 요소 json 배열로 변경되고 *json_val* 을 추가한다.

  인자가 **NULL** 이면 **NULL** 을 반환한다.
  인자가 유효하지 않은 경우 오류가 발생한다.

.. code-block:: sql

    SELECT JSON_ARRAY_APPEND ('{"a":[1,2]}','$.a','b');

::

      json_array_append('{"a":[1,2]}', '$.a', 'b')
    ======================
      {"a":[1,2,"b"]}


.. code-block:: sql

    SELECT JSON_ARRAY_APPEND ('{"a":1}','$.a','b');

::

      json_array_append('{"a":1}', '$.a', 'b')
    ======================
      {"a":[1,"b"]}

.. code-block:: sql

    SELECT JSON_ARRAY_APPEND ('{"a":[1,2]}', '$.a[0]', '1');

::

      json_array_append('{"a":[1,2]}', '$.a[0]', '1')
    ======================
      {"a":[[1,"1"],2]}

.. _fn-json-array-insert:

JSON_ARRAY_INSERT
===================================

.. function:: JSON_ARRAY_INSERT (json_doc, json path, json_val [, json path, json_val] ...)

  **JSON_ARRAY_INSERT** 함수는 첫 번째 인자의 수정된 사본을 반환한다. 주어진 각 <*json path*, *json_val*> 에 대해 함수는 해당 경로로 지정된 json 배열에 값을 삽입한다.

  (*json path*, *json_val*) 은 왼쪽에서 오른쪽으로 하나씩 평가한다. 한 쌍을 평가하여 작성된 문서는 다음 쌍을 평가하는 새로운 값이 된다.

  **JSON_ARRAY_INSERT** 작업의 규칙은 다음과 같다:

  - json 경로가 json_array의 요소를 지정하면 *json_val* 이 지정된 색인에 삽입되어 다음 요소를 오른쪽으로 이동시킨다.
  - json 경로가 배열 끝의 다음을 가리키는 경우, 배열의 끝부터 지정된 색인에 삽입될 때까지 null로 채워진다. 그리고 json_val이 지정된 색인에 삽입된다.
  - json 경로가 *json_doc* 내에 존재하지 않는 경우, json 경로의 마지막 토큰은 배열 인덱스이고 마지막 배열 인덱스 토큰이 없는 json 경로는 *json_doc* 내의 요소를 지정했을 것이다. 마지막 토큰을 제외한 json 경로로 찾은 요소는 단일 요소 json 배열로 대체되고 **JSON_ARRAY_INSERT** 작업은 원래 json 경로로 수행된다.
 
  인자가 **NULL** 이면 **NULL** 을 반환한다.
  인자가 유효하지 않거나 *json_path* 가 *json_doc* 내부의 배열의 장소를 지정하지 않으면 오류가 발생한다.

.. code-block:: sql

    SELECT JSON_ARRAY_INSERT ('[0,1,2]', '$[0]', '1');

::

      json_array_insert('[0,1,2]', '$[0]', '1')
    ======================
      ["1",0,1,2]

.. code-block:: sql

    SELECT JSON_ARRAY_INSERT ('[0,1,2]', '$[5]', '1');

::

      json_array_insert('[0,1,2]', '$[5]', '1')
    ======================
      [0,1,2,null,null,"1"]

Examples for **JSON_ARRAY_INSERT's** third rule. 

.. code-block:: sql

    SELECT JSON_ARRAY_INSERT ('{"a":4}', '$[5]', '1');

::

      json_array_insert('{"a":4}', '$[5]', '1')
    ======================
      [{"a":4},null,null,null,null,"1"]

.. code-block:: sql

    SELECT JSON_ARRAY_INSERT ('"a"', '$[5]', '1');

::

      json_array_insert('"a"', '$[5]', '1')
    ======================
      ["a",null,null,null,null,"1"]

.. _fn-json-insert:

JSON_INSERT
===================================

.. function:: JSON_INSERT (json_doc, json path, json_val [, json path, json_val] ...)

  **JSON_INSERT** 함수는 첫 번째 인자의 수정된 사본을 반환한다. 주어진 각 <*json path*, *json_val*> 에 대해 해당 경로에 다른 값이 없으면 값을 삽입한다.

  **JSON_INSERT** 의 삽입 규칙은 다음과 같다:

  json 경로가 *json_doc* 내의 다음 json 값 중 하나를 처리하는 경우 *json_val* 이 삽입된다:
  
  - 기존 json 객체에 존재하지 않는 객체 멤버이다. 키(key)가 json 경로의 마지막 요소이고 값이 *json_val* 인 (*key*, *value*)이 json 오브젝트에 추가된다.
  - 기존 json 배열 끝을 넘는 배열 색인. 배열은 배열의 끝까지 널로 채워지고 *json_val* 은 지정된 색인에 삽입된다.

  <*json path*, *json_val*> 한 쌍을 평가하여 작성된 json_doc은 다음 <*json path*, *json_val*>이 평가될 때 새로운 값이 된다.

  인자가 **NULL** 이면 **NULL** 을 반환한다.
  인자가 유효하지 않은 경우 오류가 발생한다.


* json_doc * 내의 기존 요소에 대한 경로는 무시된다:

.. code-block:: sql

    SELECT JSON_INSERT ('{"a":1}','$.a','b');

::

      json_insert('{"a":1}', '$.a', 'b')
    ======================
      {"a":1}

.. code-block:: sql

    SELECT JSON_INSERT ('{"a":1}','$.b','1');

::

      json_insert('{"a":1}', '$.b', '1')
    ======================
      {"a":1,"b":"1"}

.. code-block:: sql

    SELECT JSON_INSERT ('[0,1,2]','$[4]','1');

::

      json_insert('[0,1,2]', '$[4]', '1')
    ======================
      [0,1,2,null,"1"]

.. _fn-json-set:

JSON_SET
===================================

.. function:: JSON_SET (json_doc, json path, json_val [, json path, json_val] ...)

  **JSON_SET** 함수는 첫 번째 인자의 수정된 사본을 반환한다. 주어진 각 <*json path*, *json_val*> 에 대해 함수는 해당 경로의 값을 삽입하거나 대체한다.
  json 경로가 *json_doc* 내부에서 아래의 json 값 중 하나를 지정하면 *json_val* 이 삽입된다.

  - 기존 json 객체의 존재하지 않는 객체 멤버. (*key*, *value*) 이 json 경로에서 추론된 키와 *json_val* 값으로 json 객체에 추가된다.
  - 기존 json 배열 끝을 넘는 배열 색인. 배열은 배열의 끝까지 널로 채워지고 *json_val* 은 지정된 색인에 삽입된다.

  <*json path*, *json_val*> 한 쌍을 평가하여 작성된 json_doc은 다음 <*json path*, *json_val*>이 평가될 때 새로운 값이 된다.

  인자가 **NULL** 이면 **NULL** 을 반환한다.
  인자가 유효하지 않은 경우 오류가 발생한다.

.. code-block:: sql

    SELECT JSON_SET ('{"a":1}','$.a','b');

::

      json_set('{"a":1}', '$.a', 'b')
    ======================
      {"a":"b"}

.. code-block:: sql

    SELECT JSON_SET ('{"a":1}','$.b','1');

::

      json_set('{"a":1}', '$.b', '1')
    ======================
      {"a":1,"b":"1"}

.. code-block:: sql

    SELECT JSON_SET ('[0,1,2]','$[4]','1');

::

      json_set('[0,1,2]', '$[4]', '1')
    ======================
      [0,1,2,null,"1"]

.. _fn-json-replace:

JSON_REPLACE
===================================

.. function:: JSON_REPLACE (json_doc, json path, json_val [, json path, json_val] ...)

 **JSON_REPLACE** 함수는 첫 번째 인자의 수정된 사본을 반환한다. 주어진 각 <*json path*, *json_val*> 에 대해 해당 경로에 다른 값이 있는 경우에만 값을 대체한다.

 *json_path* 가 *json_doc* 내에 존재하지 않으면 (*json path*, *json_val*) 이 무시되고 변경되지 않는다.

 <*json path*, *json_val*> 한 쌍을 평가하여 작성된 json_doc은 다음 <*json path*, *json_val*>이 평가될 때 새로운 값이 된다.

 인자가 **NULL** 이면 **NULL** 을 반환한다.
 인자가 유효하지 않은 경우 오류가 발생한다.

.. code-block:: sql

    SELECT JSON_REPLACE ('{"a":1}','$.a','b');

::

      json_replace('{"a":1}', '$.a', 'b')
    ======================
      {"a":"b"}

*json_doc* 안에 *json path* 가 없으면 대체 (replace) 동작이 수행되지 않는다.

.. code-block:: sql

    SELECT JSON_REPLACE ('{"a":1}','$.b','1');

::

      json_replace('{"a":1}', '$.b', '1')
    ======================
      {"a":1}

.. code-block:: sql

    SELECT JSON_REPLACE ('[0,1,2]','$[4]','1');

::

      json_replace('[0,1,2]', '$[4]', '1')
    ======================
      [0,1,2]

.. _fn-json-remove:

JSON_REMOVE
===================================

.. function:: JSON_REMOVE (json_doc, json path [, json path] ...)

 **JSON_REMOVE** 함수는 주어진 모든 경로에서 값을 제거하여 첫 번째 인자의 수정된 사본을 반환한다.

 json 경로 인자는 왼쪽에서 오른쪽으로 하나씩 평가된다. json 경로를 평가하여 생성된 결과는 다음 json 경로가 평가되는 값이 된다.

 인자가 **NULL** 이면 **NULL** 을 반환한다.
 인자가 유효하지 않거나 경로가 루트를 지정하거나 경로가 없는 경우 오류가 발생한다.

.. code-block:: sql

    SELECT JSON_REMOVE ('[0,1,2]','$[1]');

::

      json_remove('[0,1,2]','$[1]')
    ======================
      [0,2]

.. code-block:: sql

    SELECT JSON_REMOVE ('{"a":1,"b":2}','$.a');

::

      json_remove('{"a":1,"b":2}','$.a')
    ======================
      {"b":2}

.. _fn-json-table:

JSON_TABLE
=====================

**JSON_TABLE** 함수는 json을 일반 테이블과 유사하게 질의(query)할 수 있는 유사 테이블 구조로 변환하는 것을 용이하게 해준다.
변환은 예를 들면 JSON_ARRAY 요소를 확장함으로써, 하나의 행 또는 복수의 행을 생성한다.

**JSON_TABLE** 의 전체 문법 :
::

    JSON_TABLE(
        expr,
        path COLUMNS (column_list)
    )   [AS] alias


    <column_list>::=
        <column> [, <column>] ...

    <column>::=
        name FOR ORDINALITY
	|  name type PATH string_path <on_empty> <on_error>
	|  name type EXISTS PATH string_path
	|  NESTED [PATH] string_path COLUMNS <column_list>

    <on_empty>::=
        NULL | ERROR | DEFAULT value ON EMPTY

    <on_error>::=
        NULL | ERROR | DEFAULT value ON ERROR


*json_doc* expr은 결과가 json_doc이 되는 표현식이어야 한다. 상수 json, 테이블의 열 또는 함수 또는 연산자의 결과 일 수 있다.
*json path* 는 유효한 경로 이어야 하며 **COLUMNS** 절에서 평가할 json 데이터를 추출하는 데 사용된다.
** COLUMNS ** 절은 열 유형 및 출력을 얻기 위해 수행되는 작업을 정의한다.
[**AS**] *alias* 절이 필요하다.


**JSON_TABLE** 은 네 가지 유형의 열을 지원한다:

- *name* **FOR ORDINALITY** : 이 유형은 **COLUMNS** 절 내에서 행 번호를 추적한다. 열 유형은 **INTEGER** 이다.
- *name* *type* **PATH** *json path* [**on empty**] [**on error**] :이 유형의 열은 지정된 json 경로에서 json_values를 추출하는 데 사용된다. 추출된 json 데이터는 지정된 유형으로 강제 변환된다.
  경로가 존재하지 않으면 **on empty** 절이 자동 수행된다. 추출된 json 값이 대상 유형으로 변환되지 않으면 **on error** 절이 자동 수행된다.

  - **on empty**\은 경로가 존재하지 않는 경우 **JSON_TABLE**\의 동작을 결정한다. **on empty**\은 다음 값 중 하나를 가질 수 있다:

    - **NULL ON EMPTY** : 열이 **NULL** 로 설정된다. 이것이 기본 동작이다.
    - **ERROR ON EMPTY**: 오류가 발생한다.
    - **DEFAULT** *value* **ON EMPTY**: 빈 값 대신에 *value* 가 사용된다.

  - **on error** 는 다음 값 중 하나를 가질 수 있다.:

    - **NULL ON ERROR**: 열이 **NULL** 로 설정된다. 이것이 기본 동작이다.
    - **ERROR ON ERROR**: 오류가 발생한다.
    - **DEFAULT** *value* **ON ERROR**: 원하는 열 유형으로 강제 변환하지 못한 배열 / 객체 / json 스칼라 대신 *value* 가 사용된다.

- *name* *type* **EXISTS PATH** *json path*: json 경로 위치에 데이터가 있으면 1을 반환하고 그렇지 않으면 0을 반환한다.

- **NESTED** [**PATH**] *json path* **COLUMNS** (*column list*) 는 경로에서 찾은 json 데이터에서 부모 결과와 결합 된 행과 열의 개별 서브 세트를 생성한다. \
  \결과는 "for each"루프와 유사하게 결합된다. \
  \json 경로는 부모 경로와 관련이 있다. \
  \ **COLUMNS** 절에 대해 동일한 규칙이 재귀적으로 적용된다.

.. code-block:: sql

    SELECT * FROM JSON_TABLE (
            '{"a":[1,[2,3]]}',
            '$.a[*]' COLUMNS ( col INT PATH '$')
        )   AS jt;

::

                       col
    ======================
                         1 -- first value found at '$.a[*]' is 1 json scalar, which is coercible to 1
                      NULL -- second value found at '$.a[*]' is [2,3] json array which cannot be coerced to int, triggering NULL ON ERROR default behavior

기본 on_error 동작을 재정의하면 이전 예제와 다른 결과가 나타난다: 

.. code-block:: sql

    SELECT * FROM JSON_TABLE (
            '{"a":[1,[2,3]]}',
            '$.a[*]' COLUMNS ( col INT PATH '$' DEFAULT '-1' ON ERROR)
        )   AS jt;

::

                       col
    ======================
                         1 -- first value found at '$.a[*]' is '1' json scalar, which is coercible to 1
                        -1 -- second value found at '$.a[*]' is '[2,3]' json array which cannot be coerced to int, triggering ON ERROR

**ON EMPTY** example:

.. code-block:: sql

    SELECT * FROM JSON_TABLE (
            '{"a":1}',
            '$' COLUMNS ( col1 INT PATH '$.a',
                          col2 INT PATH '$.b',
                          col3 INT PATH '$.c' DEFAULT '0' ON EMPTY)
        )   AS jt;

::

             col1         col2         col3
    =======================================
                1         NULL            0 

아래 예에서 '$. *'경로는 상위 열이 루트 json 객체의 멤버 값을 하나씩 받도록 하는 데 사용된다. 열 a는 처리된 내용을 보여준다.
그런 다음 루트 오브젝트의 각 멤버 값은 **NESTED** [**PATH**] 절에 의해 추가로 처리된다. **NESTED PATH**\는 경로 '$ [*]'를 사용하여 배열의 각 요소를 열에 의해 추가로 처리한다.
**FOR ORDINALITY** 열은 현재 처리된 요소의 수를 추적한다. 예제 결과에서 우리는 열의 각 새로운 요소에 대해 * ord * 열의 값도 증가함을 알 수 있다.
**FOR ORDINALITY** *nested_ord* 열은 형제 열(sibling columns)에 의해 처리되는 요소 수의 카운터 역할도 한다. 중첩된 **FOR ORDINALITY** 열은 각 일괄 처리가 완료된 후 재설정된다.
세 번째 멤버의 값인 6은 배열로 취급될 수 없으므로 중첩된 열로 처리할 수 없다. 중첩된 열은 **NULL** 값을 생성한다.

.. code-block:: sql

    SELECT * FROM JSON_TABLE (
            '{"a":[1,2],"b":[3,4,5],"d":6,"c":[7]}',
            '$.*' COLUMNS ( ord FOR ORDINALITY,
                            col JSON PATH '$',
                            NESTED PATH '$[*]' COLUMNS ( nested_ord FOR ORDINALITY,
                                                         nested_col JSON PATH '$'))
        )   AS jt;

::

             ord  col                    nested_ord  nested_col          
    =====================================================================
               1  [1,2]                           1  1                   
               1  [1,2]                           2  2                   
               2  [3,4,5]                         1  3                   
               2  [3,4,5]                         2  4                   
               2  [3,4,5]                         3  5                   
               3  6                            NULL  NULL                
               4  [7]                             1  7                   

다음 예는 여러 개의 동일한 레벨 **NESTED** [**PATH**] 절이 **JSON_TABLE** 에 의해 처리되는 방법을 보여준다. 처리될 값은 각 **NESTED** [**PATH**] 절에 하나씩 차례로 순서대로 전달된다.
**NESTED** [**PATH**] 절로 값을 처리하는 동안 형제 **NESTED** [**PATH**] 절은 **NULL** 값으로 열을 채운다.

.. code-block:: sql

    SELECT * FROM JSON_TABLE (
            '{"a":{"key1":[1,2], "key2":[3,4,5]},"b":{"key1":6, "key2":[7]}}',
            '$.*' COLUMNS ( ord FOR ORDINALITY,
                            col JSON PATH '$',
                            NESTED PATH '$.key1[*]' COLUMNS ( nested_ord1 FOR ORDINALITY,
                                                              nested_col1 JSON PATH '$'),
                            NESTED PATH '$.key2[*]' COLUMNS ( nested_ord2 FOR ORDINALITY,
                                                              nested_col2 JSON PATH '$'))
        )   AS jt;

::

              ord  col                            nested_ord1  nested_col1           nested_ord2  nested_col2         
    ===================================================================================================================
                1  {"key1":[1,2],"key2":[3,4,5]}            1  1                            NULL  NULL                
                1  {"key1":[1,2],"key2":[3,4,5]}            2  2                            NULL  NULL                
                1  {"key1":[1,2],"key2":[3,4,5]}         NULL  NULL                            1  3                   
                1  {"key1":[1,2],"key2":[3,4,5]}         NULL  NULL                            2  4                   
                1  {"key1":[1,2],"key2":[3,4,5]}         NULL  NULL                            3  5                   
                2  {"key1":6,"key2":[7]}                 NULL  NULL                            1  7                   

An example for multiple layers **NESTED** [**PATH**] clauses:

.. code-block:: sql

    SELECT * FROM JSON_TABLE (
            '{"a":{"key1":[1,2], "key2":[3,4,5]},"b":{"key1":6, "key2":[7]}}',
            '$.*' COLUMNS ( ord FOR ORDINALITY,
                            col JSON PATH '$',
                            NESTED PATH '$.*' COLUMNS ( nested_ord1 FOR ORDINALITY,
                                                        nested_col1 JSON PATH '$',
                                                        NESTED PATH '$[*]' COLUMNS ( nested_ord11 FOR ORDINALITY,
                                                                                     nested_col11 JSON PATH '$')),
                            NESTED PATH '$.key2[*]' COLUMNS ( nested_ord2 FOR ORDINALITY,
                                                              nested_col2 JSON PATH '$'))
        )   AS jt;

::

              ord  col                            nested_ord1  nested_col1           nested_ord11  nested_col11          nested_ord2  nested_col2         
    =======================================================================================================================================================
                1  {"key1":[1,2],"key2":[3,4,5]}            1  [1,2]                            1  1                            NULL  NULL                
                1  {"key1":[1,2],"key2":[3,4,5]}            1  [1,2]                            2  2                            NULL  NULL                
                1  {"key1":[1,2],"key2":[3,4,5]}            2  [3,4,5]                          1  3                            NULL  NULL                
                1  {"key1":[1,2],"key2":[3,4,5]}            2  [3,4,5]                          2  4                            NULL  NULL                
                1  {"key1":[1,2],"key2":[3,4,5]}            2  [3,4,5]                          3  5                            NULL  NULL                
                1  {"key1":[1,2],"key2":[3,4,5]}         NULL  NULL                          NULL  NULL                            1  3                   
                1  {"key1":[1,2],"key2":[3,4,5]}         NULL  NULL                          NULL  NULL                            2  4                   
                1  {"key1":[1,2],"key2":[3,4,5]}         NULL  NULL                          NULL  NULL                            3  5                   
                2  {"key1":6,"key2":[7]}                    1  6                             NULL  NULL                         NULL  NULL                
                2  {"key1":6,"key2":[7]}                    2  [7]                              1  7                            NULL  NULL                
                2  {"key1":6,"key2":[7]}                 NULL  NULL                          NULL  NULL                            1  7                   

