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

JSON_MERGE_PATCH
===================================

.. function:: JSON_MERGE_PATCH (json_doc, json_doc [, json_doc] ...)
The **JSON_MERGE_PATCH** function merges 2 or more json docs and returns the resulting merged json. **JSON_MERGE_PATCH** differs from **JSON_MERGE_PRESERVE** in that it will take the second argument when encountering merging conflicts. **JSON_MERGE_PATCH** is compliant with 
`RFC 7396 <https://tools.ietf.org/html/rfc7396/>`_.

The merging of 2 json docuemnts is performed after the following rules, recursively:

- when first argument is not an object, the result of the merge is the second object. As an exception, when the second argument is an object, the result of the merge is the merge result of the second argument with an empty object.
- when 2 objects are merged, the resulting object consists of the following (key, value) pairs:

  - All (key, value) pairs from the first object that have no corresponding (key, value) pairs in the second object.
  - All (key, value) pairs from the second object that have no corresponding (key, value) pairs in the first object, having values not null.
  - All (key, value) pairs with same keys that exist in both objects, the second object's pair having the value not null. The values of these pairs become the results of the merging operations performed according to their json type merging rules.

The JSON_MERGE_PATCH of k>=3 json documents is equivalent to JSON_MERGE_PATCH applied on first k-1 arguments and then applying a JSON_MERGE_PATCH on the result of the first JSON_MERGE_PATCH and the kth argument. 

Returns NULL if any argument is NULL.
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

JSON_MERGE_PRESERVE
===================================

.. function:: JSON_MERGE_PRESERVE (json_doc, json_doc [, json_doc] ...)

  The **JSON_MERGE_PRESERVE** function merges 2 or more json docs and returns the resulting merged json. **JSON_MERGE_PRESERVE** differs from **JSON_MERGE_PATCH** in that it preserves both json elements on merging conflicts.

  The merging of 2 json docuemnts is performed after the following rules, recursively:
- when 2 json arrays are merged, they are concatenated.
- when 2 json objects are merged, all pairs are kept that do not have a corresponding pair in the other json object. The pairs, that have a corresponding pair in the other json object are combined in a pair with the same key and with the two values being merged according to their json type merging rules.
- when 2 non-array (scalar/object) json elements are merged and at most one of them is a json object, they are wrapped as singletons and merged as 2 json arrays.
- when a non-array json element is merged with a json array, the non-array is wrapped as a singleton json array and then merged with the json array according to json array merging rules.

  The JSON_MERGE_PRESERVE operation on k>=3 json documents is equivalent to JSON_MERGE_PRESERVE applied on first k-1 arguments and then applying a JSON_MERGE_PRESERVE on the result of the first JSON_MERGE_PRESERVER and the kth argument.

  Returns NULL if any argument is NULL.
  An error occurs if any argument is not valid.

.. code-block:: sql

    SELECT JSON_MERGE_PATCH ('"a"', '"b"');
::

      json_merge('"a"', '"b"')
    ======================
      ["a","b"]

.. code-block:: sql

    SELECT JSON_MERGE_PATCH ('["a","b","c"]', '"scalar"');
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

JSON_MERGE
===================================

.. function:: JSON_MERGE (json_doc, json_doc [, json_doc] ...)

  **JSON_MERGE** is an alias for **JSON_MERGE_PRESERVE**.
