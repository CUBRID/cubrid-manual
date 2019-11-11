:meta-keywords: cubrid json, json functions, database json
:meta-description: CUBRID functions that create, query and modify JSON data.

:tocdepth: 3

*********************************
JSON functions
*********************************

.. contents::

JSON_ARRAY
===================================

.. function:: JSON_ARRAY ([val1 [ , val2] ...])

  The **JSON_ARRAY** function returns a json array containing the given list (possibly empty) of values.

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

JSON_OBJECT
===================================

.. function:: JSON_OBJECT ([key1, val1 [ , key2, val2] ...])

  The **JSON_OBJECT** function returns a json object containing the given list (possibly empty) of key-value pairs.

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

JSON_KEYS
===================================

.. function:: JSON_KEYS (json_doc [ , json path])

  The **JSON_KEYS** function returns a json array of all the object keys of the json object at the given path.
  Json null is returned if the path addresses a json element that is not a json object.
  If json path argument is missing, the keys are gathered from json root element.
  An error occurs if json path does not exist. Returns NULL if json_doc argument is NULL.

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

JSON_DEPTH
===================================

.. function:: JSON_DEPTH (json_doc)

  The **JSON_DEPTH** function returns the maximum depth of the json.
  Depth count starts at 1. The depth level is increased by one by non-empty json arrays or by non-empty json objects.
  Returns NULL if argument is NULL.

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

Example of a deeper json:

.. code-block:: sql

    SELECT JSON_DEPTH('[{"a":[1,2,3,{"k":[4,5]}]},2,3,4,5,6,7]');
::

      json_depth('[{"a":[1,2,3,{"k":[4,5]}]},2,3,4,5,6,7]')
    ======================
      6

JSON_LENGTH
===================================

.. function:: JSON_LENGTH (json_doc [ , json path])

  The **JSON_LENGTH** function returns the length of the json element at the given path.
  If no path argument is given, the returned value is the length of the root json element.
  Returns NULL if any argument is NULL or if no element exists at the given path.

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

JSON_VALID
===================================

.. function:: JSON_VALID (val)

  The **JSON_VALID** function returns 1 if the given val argument is a valid json_doc, 0 otherwise.
  Returns NULL if argument is NULL.

.. code-block:: sql

    SELECT JSON_VALID('[{"a":4}, 2]');
    1
    SELECT JSON_VALID('{"wrong json object":');
    0

JSON_TYPE
===================================

.. function:: JSON_TYPE (json_doc)

  The **JSON_TYPE** function returns the type of the json_doc argument as a string.

.. code-block:: sql

    SELECT JSON_TYPE ('[{"a":4}, 2]');
    'JSON_ARRAY'
    SELECT JSON_TYPE ('{"a":4}');
    'JSON_OBJECT'
    SELECT JSON_TYPE ('"aaa"');
    'STRING'

JSON_QUOTE
===================================

.. function:: JSON_QUOTE (str)

  Escapes quotes and special characters and surrounds the resulting string in quotes. Returns result as a json_string.
  Returns NULL if str argument is NULL.

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

JSON_UNQUOTE
===================================

.. function:: JSON_UNQUOTE (json_doc)

  Unquotes a json_value's json string and returns the resulting string.
  Returns NULL if json_doc argument is NULL.

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

JSON_PRETTY
===================================

.. function:: JSON_PRETTY (json_doc)

  Returns a string containing the json_doc pretty-printed.
  Returns NULL if json_doc argument is NULL.

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

JSON_SEARCH
===================================

.. function:: JSON_SEARCH (json_doc, one/all, search_str [, escape_char [, json path] ...])

  Returns a json array of json paths or a single json path which contain json strings matching the given search_str.
  The matching is performed by applying the LIKE operator on internal json strings and search_str. Same rules apply for the escape_char and search_str of JSON_SEARCH as for their counter-parts from the LIKE operator.
  For further description of LIKE-related arguments rules refer to :ref:`like-expr`.

  Using 'one' as one/all argument will cause the json_search to stop after the first match is found.
  On the other hand, 'all' will force json_search to gather all paths matching the given search_str.

  The given json paths determine filters on the returned paths, the resulting json paths's prefixes need to match at least one given json path argument.
  If no json path argument is given, json_search will execute the search starting from the root element.

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

Wildcards can be used to define path filters as more general formats.
Accepting only json paths that start with object key identifier:

.. code-block:: sql

    SELECT JSON_SEARCH('{"a":["a","b"],"b":"a","c":"a"}', 'all', 'a', NULL, '$.*');
::

      json_search('{"a":["a","b"],"b":"a","c":"a"}', 'all', 'a', null, '$.*')
    ======================
      "["$.a[0]","$.b","$.c"]"

Accepting only json paths that start with object key identifier and follow immediately with a json array index will filter out '$.b', '$.d.e[0]' matches:

.. code-block:: sql

    SELECT JSON_SEARCH('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', 'all', 'a', NULL, '$.*[*]');
::

      json_search('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', 'all', 'a', null, '$.*[*]')
    ======================
      "["$.a[0]","$.c[0]"]"
::

Accepting any paths that contain json array indexes will filter out '$.b'

.. code-block:: sql

    SELECT JSON_SEARCH('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', 'all', 'a', NULL, '$**[*]');
::

      json_search('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', 'all', 'a', null, '$**[*]')
    ======================
      "["$.a[0]","$.c[0]","$.d.e[0]"]"

JSON_EXTRACT
===================================

.. function:: JSON_EXTRACT (json_doc, json path [, json path] ...)

  Returns json elements from the json_doc, that are addressed by the given paths.
  If json path arguments contain wildcards, all elements that are addressed by a path compatible with the wildcards-containing json path are gathered in a resulting json array. 
  A single json element is returned if no wildcards are used in the given json paths and a single element is found, otherwise the json elements found are wrapped in a json array.
  Raises an error if a json path is NULL or invalid or if json_doc argument is invalid.
  Returns NULL if no elements are found or if json_doc is NULL.

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
      "["a","b"]" -- '$.a[0]' and '$.a[1]' wrapped in a json array, forming ["a","b"] 

Changing '.a' from previous query with '.*' wildcards will also match '$.c[0]'. This will match any json path that is exactly an object key identifier followed by an array index.

.. code-block:: sql

    SELECT JSON_EXTRACT('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$.*[*]');
::

      json_extract('{"a":["a","b"],"b":"a","c":["a"], "d":{"e":["a"]}}', '$.*[*]')
    ======================
      "["a","b","a"]"

The following json path will match all json paths that end with a json array index (matches all previous matched paths and, in addition, '$.d.e[0]') :

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
      "["a"]" -- '$.d.e[0]' is the only path matching the given argument path family - paths that start with '.d' and end with an array index

->
===================================

.. function:: json_doc -> json path

  Alias operator for JSON_EXTRACT with 2 arguments, having the json_doc argument constrained to be a column.
  Raises an error if the json path is NULL or invalid.
  Returns NULL if it is applied on a NULL json_doc argument.

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

->>
===================================

.. function:: json_doc ->> json path

  Alias for JSON_UNQUOTE(json_doc->json path). Operator can be applied only on json_doc arguments that are columns.
  Raises an error if the json path is NULL or invalid.
  Returns NULL if it is applied on a NULL json_doc argument.

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

JSON_CONTAINS_PATH
===================================

.. function:: JSON_CONTAINS_PATH (json_doc, one/all, json path [, json path] ...)

  The **JSON_CONTAINS_PATH** function verifies whether the the given paths exist inside the json_doc.
  1 is returned with one/all argument given as 'all' only if all given paths exist inside the json_doc, 0 is returned otherwise. In case one/all argument is 'one', 1 is returned if at least one json path exists inside the json_doc, 0 otherwise.
  Returns NULL if any argument is NULL.
  An error occurs if any argument is invalid.

.. code-block:: sql

    SELECT JSON_CONTAINS_PATH ('[{"0":0},1,"2",{"three":3}]', 'all', '$[0]', '$[0]."0"', '$[1]', '$[2]', '$[3]') ;
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

The JSON_CONTAINS_PATH function supports wildcards inside json paths.

.. code-block:: sql

    SELECT JSON_CONTAINS_PATH ('[{"0":0},1,"2",{"three":3}]', 'one', '$.inexistent', '$[*]."three"');
::

      json_contains_path('[{"0":0},1,"2",{"three":3}]', 'one', '$[*]."three"')
    ==========================================================================
                                                                             1

JSON_CONTAINS
===================================

.. function:: JSON_CONTAINS (json_doc doc1, json_doc doc2 [, json path])

  The **JSON_CONTAINS** function verifies whether the doc2 is contained inside the doc1 at the optionally specified path.
  A json element contains another json element if the following recursive rules are satisfied:
- A json scalar contains another json scalar if they have the same type (their JSON_TYPE () is equal) and are equal. As an exception, json integer can be compared and equal to json double (even if their JSON_TYPE () evaluation are different).
- A json array contains a json scalar or a json object if any of json array's elements contains the json_nonarray.
- A json array contains another json array if all the second json array's elements are contained in the first json array.
- A json object contains another json object if, for every (key2, value2) pair in the second object, there exists a (key1, value1) pair in the first object with key1=key2 and value2 contained in value1.
  Otherwise the json element is not contained.

  Returns whether doc2 is contained in root json element of doc1 if no json path argument is given.
  Returns NULL if any argument is NULL.
  An error occurs if any argument is invalid.

.. code-block:: sql

    SELECT JSON_CONTAINS ('"simple"','"simple"');
::

      json_contains('"simple"', '"simple"')
    =======================================
                                          1

.. code-block:: sql

    SELECT JSON_CONTAINS ('["not important", "important"]','"important"');
::

      json_contains('["not important", "important"]', '"important"')
    ================================================================
                                                                   1

.. code-block:: sql

    SELECT JSON_CONTAINS ('["not important", "important1", ["not important", "important2"]]','["important1", "important2"]');
::

      json_contains('["not important", "important1", ["not important", "important2"]]', '["important1", "important2"]')
    ===================================================================================================================
                                                                                                                      1

.. code-block:: sql

    SELECT JSON_CONTAINS ('{"k1":["not important", "important1"], "k2": ["not important", "important2"]}','{"k1":"important1", "k2":"important2"}');
::

      json_contains('{"k1":["not important", "important1"], "k2": ["not important", "important2"]}', '{"k1":"important1", "k2":"important2"}')
    ==========================================================================================================================================
                                                                                                                                             1

Note that json objects do not check containment the same way json array do. It is impossible to have a json element that is not a descendent of a json object contained in a sub-element of a json object.

.. code-block:: sql

    SELECT JSON_CONTAINS ('["not important", "important1", ["not important", {"k":"important2"}]]','["important1", "important2"]');
::

      json_contains('["not important", "important1", ["not important", {"k":"important2"}]]', '["important1", "important2"]')
    =========================================================================================================================
                                                                                                                            0

.. code-block:: sql

    SELECT JSON_CONTAINS ('["not important", "important1", ["not important", {"k_neccessary":["important2"]}]]','["important1", {"k_neccessary":"important2"}]');
::

      json_contains('["not important", "important1", ["not important", {"k_neccessary":["important2"]}]]', '["important1", {"k_neccessary":"important2"}]')
    =====================================================================================================================================================
                                                                                                                                                        1
