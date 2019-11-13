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

  Alias operator for JSON_EXTRACT with two arguments, having the json_doc argument constrained to be a column.
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

  The **JSON_CONTAINS_PATH** function verifies whether the given paths exist inside the json_doc.
  When one/all argument is 'all', all given paths must exist to return 1. Returns 0 otherwise.
  When one/all argument is 'one', it returns 1 if any given path exists. Returns 0 otherwise.
  Returns NULL if any argument is NULL.
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

The JSON_CONTAINS_PATH function supports wildcards inside json paths.

.. code-block:: sql

    SELECT JSON_CONTAINS_PATH ('[{"0":0},1,"2",{"three":3}]', 'one', '$.inexistent', '$[*]."three"');

::

     json_contains_path('[{"0":0},1,"2",{"three":3}]', 'one', '$.inexistent', '$[*]."three"')
    ==========================================================================
                                                                             1

JSON_CONTAINS
===================================

.. function:: JSON_CONTAINS (json_doc doc1, json_doc doc2 [, json path])

  The **JSON_CONTAINS** function verifies whether the doc2 is contained inside the doc1 at the optionally specified path.
  A json element contains another json element if the following recursive rules are satisfied:

- A json scalar contains another json scalar if they have the same type (their JSON_TYPE () are equal) and are equal. As an exception, json integer can be compared and equal to json double (even if their JSON_TYPE () evaluation are different).
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

Note that json objects do not check containment the same way json arrays do. It is impossible to have a json element that is not a descendent of a json object contained in a sub-element of a json object.

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

  The **JSON_MERGE_PRESERVE** function merges two or more json docs and returns the resulting merged json. **JSON_MERGE_PRESERVE** differs from **JSON_MERGE_PATCH** in that it preserves both json elements on merging conflicts.

  The merging of two json documents is performed after the following rules, recursively:
  
- when two json arrays are merged, they are concatenated.
- when two non-array (scalar/object) json elements are merged and at most one of them is a json object, the result is an array containing the two json elements.
- when a non-array json element is merged with a json array, the non-array is wrapped as a single element json array and then merged with the json array according to json array merging rules.
- when two json objects are merged, all pairs that do not have a corresponding pair in the other json object are preserved. For matching keys, the values are always merged by applying the rules recursively.

  Merge operations are executed serially when there are more than two arguments: the result of merging first two arguments is merged with third, this result is then merged with fourth and so on.

  Returns NULL if any argument is NULL.
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

JSON_MERGE
===================================

.. function:: JSON_MERGE (json_doc, json_doc [, json_doc] ...)

  **JSON_MERGE** is an alias for **JSON_MERGE_PRESERVE**.

JSON_ARRAY_APPEND
===================================

.. function:: JSON_ARRAY_APPEND (json_doc, json path, json_val [, json path, json_val] ...)

  The **JSON_ARRAY_APPEND** function returns a modified copy of first argument. For each given <json path, json_val> pair, the function appends the value to the json array addressed by the corresponding path.

  The (json path, json_val) pairs are evaluated one by one, from left to right. The document produced by evaluating one pair becomes the new value against which the next pair is evaluated.

  If the json path points to an json array inside the json_doc, the json_val is appended at the end of the array. 
  If the json path points to a non-array json element, the non-array gets wrapped as a single element json array containing the referred non-array element followed by the appending of the given json_val.

  Returns NULL if any argument is NULL.
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


JSON_ARRAY_INSERT
===================================

.. function:: JSON_ARRAY_INSERT (json_doc, json path, json_val [, json path, json_val] ...)

  The **JSON_ARRAY_INSERT** function returns a modified copy of first argument. For each given <json path, json_val> pair, the function inserts the value in the json array addressed by the corresponding path.

  The (json path, json_val) pairs are evaluated one by one, from left to right. The document produced by evaluating one pair becomes the new value against which the next pair is evaluated.

  The rules of the **JSON_ARRAY_INSERT** operation are the following:

  - if a json path addresses an element of a json_array, the given json_val is inserted at the specified index, shifting any following elements to the right.
  - if the json path points to an array index after the end of an array, the array is filled with nulls after end of the array until the specified index and the json_val is inserted at the specified index.
  - if the json path does not exist inside the json_doc, the last token of the json path is an array index and the json path without the last array index token would have pointed to an element inside the json_doc, the element found by the stripped json path is replaced with single element json array and the **JSON_ARRAY_INSERT** operation is performed with the original json path.
 
  Returns NULL if any argument is NULL.
  An error occurs if any argument is invalid or if a json_path does not address a cell of an array inside the json_doc.

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

JSON_INSERT
===================================

.. function:: JSON_INSERT (json_doc, json path, json_val [, json path, json_val] ...)

  The **JSON_INSERT** function inserts json_val arguments inside the json_doc at the given paths.
  Returns the resulting json after insertion.

  The insertion rules for **JSON_INSERT** are the following:

  For each (json path, json_val) pair, from left to right, if the json path addresses an existing element inside the json_doc, the argument pair is ignored and does not overwrite the existing json element.
  Otherwise, the json_val is inserted if the json path addresses one of the following json values inside the json_doc:
  
  - An inexistent object member of an existing json object. A (key, value) pair is added to the json object with the key being json path's last element and the value being the json_val.
  - An array index past of an existing json array's end. The array is filled with nulls after the initial end of the array and the json_val is inserted at the specified index.

  The document produced by evaluating one pair becomes the new value against which the next pair is evaluated. 

  Returns NULL if any argument is NULL.
  An error occurs if any argument is invalid.


Paths to existing elements inside the json_doc are ignored:

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

JSON_SET
===================================

.. function:: JSON_SET (json_doc, json path, json_val [, json path, json_val] ...)

  The **JSON_SET** function returns a modified copy of first argument. For each given <json path, json_val> pair, the function inserts or replaces the value at the corresponding path.


  For each (json path, json_val) pair, from left to right, an existing value is searched inside the json_doc at the json path and is replaced with the given json_val.
  Otherwise, the json_val is inserted if the json path addresses one of the following json values inside the json_doc:

  - An inexistent object member of an existing json object. A (key, value) pair is added to the json object with the key deduced from the json path and the value being the json_val.
  - An array index past of an existing json array's end. The array is filled with nulls after the initial end of the array and the json_val is inserted at the specified index.

  The document produced by evaluating one pair becomes the new value against which the next pair is evaluated. 

  Returns the resulting json after insertion.
  Returns NULL if any argument is NULL.
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


JSON_REPLACE
===================================

.. function:: JSON_REPLACE (json_doc, json path, json_val [, json path, json_val] ...)

 The **JSON_REPLACE** function returns a modified copy of first argument. For each given <json path, json_val> pair, the function replaces the value only if another value is found at the corresponding path.

  For each (json path, json_val) pair, from left to right, an existing value is searched inside the json_doc at the json path and is replaced with json_val.
  Otherwise, if the json_path does not exist inside the json_doc, the (json path, json_val) pair is ignored and has no effect.

  The document produced by evaluating one pair becomes the new value against which the next pair is evaluated. 

  Returns the resulting json after replacements.
  Returns NULL if any argument is NULL.
  An error occurs if any argument is invalid.

.. code-block:: sql

    SELECT JSON_REPLACE ('{"a":1}','$.a','b');

::

      json_replace('{"a":1}', '$.a', 'b')
    ======================
      {"a":"b"}

No replacement is done if the json path does not exist inside the json_doc. 

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

JSON_REMOVE
===================================

.. function:: JSON_REMOVE (json_doc, json path [, json path] ...)

The **JSON_REMOVE** function returns a modified copy of first argument, by removing values from all given paths.

The json path arguments are evaluated one by one, from left to right. The result produced by evaluating a json path becomes the value against which the next json path is evaluated.

  Returns the resulting json after removal.
  Returns NULL if any argument is NULL.
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
