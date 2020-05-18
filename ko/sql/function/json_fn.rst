:meta-keywords: cubrid json, json functions, database json, JSON_TABLE
:meta-description: CUBRID functions that create, query and modify JSON data.

:tocdepth: 3

*********************************
JSON functions
*********************************

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
  - *json path/pointer*: :ref:`json-path`과 :ref:`json-pointer`에 설명된 규칙을 따르는 문자열

.. note::

  JSON 함수 문자열 인자의 코드셋은 UTF8을 기준으로 한다. 다른 코드셋의 입력 문자열은 UTF8로 변환된다.
  UTF8이 아닌 코드셋 문자열에 대한 대소문자 구별 없는 검색은 기대와 다른 결과가 나올 수 있다.

다음의 표는 입력 인자를 해석하는데 있어서 *json_doc*와 *val*의 차이를 보여주고 있다:

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
  *json 경로*가 존재하지 않으면 오류가 발생하고 *json_doc* 인자가 **NULL**이면 **NULL**을 반환한다.

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
  인자가 **NULL**이면 **NULL**을 반환한다.

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
  인자가 **NULL**이거나 해당 경로에 어떤 요소도 존재하지 않으면 **NULL**이 반환된다.

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
  해당 인자가 **NULL**인 경우 **NULL**을 반환한다.

.. code-block:: sql

    SELECT JSON_VALID('[{"a":4}, 2]');
    1
    SELECT JSON_VALID('{"wrong json object":');
    0

.. _fn-json-type:

JSON_TYPE
===================================

.. function:: JSON_TYPE (json_doc)

  **JSON_TYPE** 함수는 문자열 인자인 *json_doc*의 타입을 반환한다.

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
  *str* 인자가 **NULL**인 경우 **NULL**을 반환한다.

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
  *json_doc* 인자가 **NULL**이면 **NULL**을 반환한다.

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

  **JSON_PRETTY**는 *json_doc* 보기좋게 출력된 문자열을 반환한다.
  *json_doc* 인자가 **NULL**이면 **NULL**을 반환한다.

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

  **JSON_SEARCH** 함수는 해당 *search_str*과 일치하는 json 문자열을 포함한 하나의 json 경로 혹은 복수의 json 경로를 반환한다.
  일치 여부 검사는 내부의 json 문자열과 *search_str*에 **LIKE** 연산자를 적용하여 수행된다. **JSON_SEARCH**의 *escape_char* 및 *search_str*에 대해 **LIKE** 연산자의 대응 부분과 동일한 규칙이 적용된다.
  **LIKE**관련 규칙에 대한 추가 설명은 :ref:`like-expr`을 참고한다.

  one/all에서 'one'을 사용하면 **JSON_SEARCH** 첫번째 일치가 나타났을 때 탐색이 멈추게 된다.
  반면에 'all'을 사용하면 *search_str*과 일치하는 모든 경로를 탐색하게 된다.

  주어진 json 경로는 반환 된 경로의 필터를 결정하므로 결과로 나온 json 경로의 접두사(prefix)는 적어도 하나의 주어진 json 경로 인자와 일치해야 한다.
  json 경로 인자가 누락된 경우, **JSON_SEARCH**는 루트 요소로 부터 탐색을 시작한다.

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

  해당 경로로 지정된 *json_doc*로부터 json 요소를 반환한다.
  json 경로 인자가 와일드카드를 포함하는 경우 와일드카드에 의해 포함될 수 있는 모든 경로의 지정된 json 요소가 json 배열 결과로 반환된다.
  와일드카드를 사용하지 않고 json 경로에서 하나의 요소만 발견된 경우 하나의 json 요소만 반환되며, 그렇지 않은 경우 발견된 json 요소는 json 배열로 구성하여 반환된다.
  json 경로가 **NULL**이거나 유효하지 않은 경우 혹은 *json_doc* 인자가 유효하지 않은 경우 에러가 반환된다.
  json 요소가 발견되지 않거나 json_doc이 **NULL**인 경우 **NULL**을 반환한다.

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

  *json_doc* 인자가 하나의 컬럼으로 제한된 두 개의 인자를 가지는 **JSON_EXTRACT**의 별칭 연산자.
  json 경로가 **NULL**이거나 유효하지 않은 경우 오류를 반환한다.
  **NULL** *josn_doc* 인자가 적용된 경우에는 **NULL**을 반환한다.

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

   **JSON_UNQUOTE**의 별칭 (json_doc->json 경로). 본 연산자는 컬럼인 *json_doc* 인자에만 적용 할 수 있다.
   json 경로가 **NULL**이거나 유효하지 않은 경우 오류가 발생한다.
   **NULL** *json_doc* 인자에 적용된 경우 **NULL**을 반환한다.

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

  해당 인자가 **NULL**이면 **NULL**을 반환한다.
  Returns **NULL** if any argument is **NULL**.
  해당 인자가 유효하지 않으면 오류가 발생한다.
  An error occurs if any argument is invalid.

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

  **JSON_CONTAINS** 함수는 *doc2*가 옵션으로 지정된 경로의 *doc1*에 포함되는지를 검사한다.
  다음과 같이 재귀 규칙이 충족되는 경우 json 요소에 다른 json 요소가 포함된다.

  - 타입이 같고 (**JSON_TYPE** ()이 일치하고) 스칼라도 같은 경우 json 스칼라에 다른 json 스칼라가 포함된다. 예외적으로, json integer는 **JSON_TYPE** ()이 다른 경우에도 json double과 비교를 통해 동일한 것으로 간주될 수 있다.
  - json 배열 요소에 json_nonarray가 포함되어 있으면 json 배열에 json 스칼라 또는 json 객체가 포함된다.
  - 두 번째 json 배열의 모든 요소가 첫 번째 json 배열에 포함되어 있으면 json 배열에 다른 json 배열이 포함된다.
  - 두 번째 객체의 모든 (*key2*, *value2*) 쌍에 대해 첫 번째 객체에 *key1* = *key2* 이고 *value2*가 *value1*을 포함하는 (*key1*, *value1*) 쌍이 있는 경우 json 개체에는 다른 json 오브젝트가 포함된다.
  - 이 외에는 json 요소가 포함되지 않는다.

  json 경로 인자가 제공되지 않은 경우 *doc2*가 *doc1*의 루트 json 요소에 포함되는지 여부를 리턴한다.
  인자가 **NULL**이면 **NULL**을 반환한다.
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

The **JSON_MERGE_PATCH** function merges two or more json docs and returns the resulting merged json. **JSON_MERGE_PATCH** differs from **JSON_MERGE_PRESERVE** in that it will take the second argument when encountering merging conflicts. **JSON_MERGE_PATCH** is compliant with
`RFC 7396 <https://tools.ietf.org/html/rfc7396/>`_.

The merging of two json documents is performed with the following rules, recursively:

- when two non-object jsons are merged, the result of the merge is the second value.
- when a non-object json is merged with a json object, the result is the merge of an empty object with the second merging argument.
- when two objects are merged, the resulting object consists of the following members:

  - All members from the first object that have no corresponding member with the same key in the second object.
  - All members from the second object that have no corresponding members with equal keys in the first object, having values not null. Members with null values from second object are ignored.
  - One member for each member in the first object that has a corresponding non-null valued member in the second object with the same key. Same key members that appear in both objects and the second object's member value is null, are ignored. The values of these pairs become the results of merging operations performed on the values of the members from the first and second object.

Merge operations are executed serially when there are more than two arguments: the result of merging first two arguments is merged with third, this result is then merged with fourth and so on.

Returns **NULL** if any argument is **NULL**.
An error occurs if any argument is not valid.

.. code-block:: sql

    SELECT JSON_MERGE_PATCH ('["a","b","c"]', '"scalar"');

::

      json_merge_patch('["a","b","c"]', '"scalar"')
    ======================
      "scalar"


The exception to the merge-patching, when the first argument is non-object and the second is an object. A merge operation is performed between an empty object and the second object argument.

.. code-block:: sql

    SELECT JSON_MERGE_PATCH ('["a"]', '{"a":null}');

::

      json_merge_patch('["a"]', '{"a":null}')
    ======================
      {}

Objects merging example, exemplifying the described object merging rules:

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

  The **JSON_MERGE_PRESERVE** function merges two or more json docs and returns the resulting merged json. **JSON_MERGE_PRESERVE** differs from **JSON_MERGE_PATCH** in that it preserves both json elements on merging conflicts.

  The merging of two json documents is performed after the following rules, recursively:
  
  - when two json arrays are merged, they are concatenated.
  - when two non-array (scalar/object) json elements are merged and at most one of them is a json object, the result is an array containing the two json elements.
  - when a non-array json element is merged with a json array, the non-array is wrapped as a single element json array and then merged with the json array according to json array merging rules.
  - when two json objects are merged, all pairs that do not have a corresponding pair in the other json object are preserved. For matching keys, the values are always merged by applying the rules recursively.

  Merge operations are executed serially when there are more than two arguments: the result of merging first two arguments is merged with third, this result is then merged with fourth and so on.

  Returns **NULL** if any argument is **NULL**.
  An error occurs if any argument is not valid.

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


**JSON_MERGE_PRESERVE**, as opposed to **JSON_MERGE_PATCH**, will not drop and patch first argument's elements during merges and will gather them together.

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

  **JSON_MERGE** is an alias for **JSON_MERGE_PRESERVE**.

.. _fn-json-array-append:

JSON_ARRAY_APPEND
===================================

.. function:: JSON_ARRAY_APPEND (json_doc, json path, json_val [, json path, json_val] ...)

  The **JSON_ARRAY_APPEND** function returns a modified copy of the first argument. For each given <*json path*, *json_val*> pair, the function appends the value to the json array addressed by the corresponding path.

  The (*json path*, *json_val*) pairs are evaluated one by one, from left to right. The document produced by evaluating one pair becomes the new value against which the next pair is evaluated.

  If the json path points to a json array inside the *json_doc*, the *json_val* is appended at the end of the array. 
  If the json path points to a non-array json element, the non-array gets wrapped as a single element json array containing the referred non-array element followed by the appending of the given *json_val*.

  Returns **NULL** if any argument is **NULL**.
  An error occurs if any argument is invalid.

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

  The **JSON_ARRAY_INSERT** function returns a modified copy of the first argument. For each given <*json path*, *json_val*> pair, the function inserts the value in the json array addressed by the corresponding path.

  The (*json path*, *json_val*) pairs are evaluated one by one, from left to right. The document produced by evaluating one pair becomes the new value against which the next pair is evaluated.

  The rules of the **JSON_ARRAY_INSERT** operation are the following:

  - if a json path addresses an element of a json_array, the given *json_val* is inserted at the specified index, shifting any following elements to the right.
  - if the json path points to an array index after the end of an array, the array is filled with nulls after end of the array until the specified index and the json_val is inserted at the specified index.
  - if the json path does not exist inside the *json_doc*, the last token of the json path is an array index and the json path without the last array index token would have pointed to an element inside the *json_doc*, the element found by the stripped json path is replaced with single element json array and the **JSON_ARRAY_INSERT** operation is performed with the original json path.
 
  Returns **NULL** if any argument is **NULL**.
  An error occurs if any argument is invalid or if a *json_path* does not address a cell of an array inside the *json_doc*.

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

  The **JSON_INSERT** function returns a modified copy of the first argument. For each given <*json path*, *json_val*> pair, the function inserts the value if no other value exists at the corresponding path.

  The insertion rules for **JSON_INSERT** are the following:

  The *json_val* is inserted if the json path addresses one of the following json values inside the *json_doc*:
  
  - An inexistent object member of an existing json object. A (*key*, *value*) pair is added to the json object with the key being json path's last element and the value being the *json_val*.
  - An array index past of an existing json array's end. The array is filled with nulls after the initial end of the array and the *json_val* is inserted at the specified index.

  The document produced by evaluating one pair becomes the new value against which the next pair is evaluated. 

  Returns **NULL** if any argument is **NULL**.
  An error occurs if any argument is invalid.


Paths to existing elements inside the *json_doc* are ignored:

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

  The **JSON_SET** function returns a modified copy of the first argument. For each given <*json path*, *json_val*> pair, the function inserts or replaces the value at the corresponding path.
  Otherwise, the *json_val* is inserted if the json path addresses one of the following json values inside the *json_doc*:

  - An inexistent object member of an existing json object. A (*key*, *value*) pair is added to the json object with the key deduced from the json path and the value being the *json_val*.
  - An array index past of an existing json array's end. The array is filled with nulls after the initial end of the array and the *json_val* is inserted at the specified index.

  The document produced by evaluating one pair becomes the new value against which the next pair is evaluated. 

  Returns **NULL** if any argument is **NULL**.
  An error occurs if any argument is invalid.

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

 The **JSON_REPLACE** function returns a modified copy of the first argument. For each given <*json path*, *json_val*> pair, the function replaces the value only if another value is found at the corresponding path.

 If the *json_path* does not exist inside the *json_doc*, the (*json path*, *json_val*) pair is ignored and has no effect.

 The document produced by evaluating one pair becomes the new value against which the next pair is evaluated. 

 Returns **NULL** if any argument is **NULL**.
 An error occurs if any argument is invalid.

.. code-block:: sql

    SELECT JSON_REPLACE ('{"a":1}','$.a','b');

::

      json_replace('{"a":1}', '$.a', 'b')
    ======================
      {"a":"b"}

No replacement is done if the *json path*` does not exist inside the *json_doc*. 

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

 The **JSON_REMOVE** function returns a modified copy of the first argument, by removing values from all given paths.

 The json path arguments are evaluated one by one, from left to right. The result produced by evaluating a json path becomes the value against which the next json path is evaluated.

 Returns **NULL** if any argument is **NULL**.
 An error occurs if any argument is invalid or if a path points to the root or if a path does not exist.

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

**JSON_TABLE** function facilitates transforming jsons into a table-like structures
that can be queried similarly as regular tables.
The transformation generates a single row or multiple rows, by expanding for
example the elements of a JSON_ARRAY.

The full syntax of **JSON_TABLE**:
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


The *json_doc* expr must be an expression that results in a json_doc. This can be a constant json, a table's column or the result of a function or operator.
The *json path* must be a valid path and is used to extract json data to be evaluated in the **COLUMNS** clause.
The **COLUMNS** clause defines output column types and operations performed to get the output.  
The [**AS**] *alias* clause is required.


**JSON_TABLE** supports four types of columns:

- *name* **FOR ORDINALITY**: this type keeps track of a row's number inside a **COLUMNS** clause. The column's type is **INTEGER**.
- *name* *type* **PATH** *json path* [**on empty**] [**on error**]: Columns of this type are used to extract json_values from the specified json paths. The extracted json data is then coerced to the specified type.
  If the path does not exist, json value triggers the **on empty** clause. The **on error** clause is triggered if the extracted json value is not coercible to the target type.

  - **on empty** determines the behavior of **JSON_TABLE** in case the path does not exist. **on empty** can have one of the following values:

    - **NULL ON EMPTY**: the column is set to **NULL**. This is the default behavior.
    - **ERROR ON EMPTY**: an error is thrown
    - **DEFAULT** *value* **ON EMPTY**: *value* will be used instead of the missing value.

  - **on error** can have one of the following values:

    - **NULL ON ERROR**: the column is set to **NULL**. This is the default behavior.
    - **ERROR ON ERROR**: an error is thrown.
    - **DEFAULT** *value* **ON ERROR**: *value* will be used instead of the array/object/json scalar that failed coercion to desired column type.

- *name* *type* **EXISTS PATH** *json path*: this returns 1 if any data is present at the json path location, 0 otherwise.

- **NESTED** [**PATH**] *json path* **COLUMNS** (*column list*) generates from json data \
  \found at path a separate subset of rows and columns that are combined \
  \with the results of parent. Results are combined similarly as "for each" \
  \ loops. The json path is relative to the parent's path. Same rules for \
  \ **COLUMNS** clause are applied recursively.

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

Overriding the default on_error behavior, results in a different output from previous example: 

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

In the example below, '$.*' path will be used to make the parent columns receive root json object's member values one by one. Column a shows what is processed. Each member's value of
the root object will then be processed further by the **NESTED** [**PATH**] clause. **NESTED PATH** uses path '$[*]' take each element of the array to be further processed by its columns.
**FOR ORDINALITY** columns track the count of the current processed element. In the example's result we can see that for each new element in a column, the *ord* column's value also gets incremented.
**FOR ORDINALITY** *nested_ord* column also acts as a counter of the number of elements processed by sibling columns. The nested **FOR ORDINALITY** column gets reset after finishing each processing batch.
The third member's value, 6 cannot be treated as an array and therefore cannot be processed by the nested columns. Nested columns will yield **NULL** values. 

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

The following example showcases how multiple same-level **NESTED** [**PATH**] clauses are treated by the **JSON_TABLE**. The value to be processed gets passed once, one by one and in order, to each of the **NESTED** [**PATH**] clauses.
During processing of a value by a **NESTED** [**PATH**] clause, any sibling **NESTED** [**PATH**] clauses will fill their column with **NULL** values.

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
