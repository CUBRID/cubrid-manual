:meta-keywords: cubrid json, json functions, database json
:meta-description: CUBRID functions that create, query and modify JSON data.

:tocdepth: 3

*********************************
JSON functions
*********************************

.. contents::

JSON_ARRAY
===================================

.. function:: JSON_ARRAY ([val1 [, val2] ...])

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

.. function:: JSON_OBJECT ([key1, val1[ , key2, val2] ...])

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

  The **JSON _KEYS** function returns a json array of all the object keys of the json object at the given path. Json null is returned if the path addresses a json element that is not a json object.
  If no argument is given, the keys are gathered from the root path ('$'). Returns NULL if json_doc argument is NULL.

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

  The **JSON_DEPTH** function returns the maximum depth of the json. Depth count starts at 1. The depth level is increased by one by non-empty json arrays or by non-empty json objects. Returns NULL if argument is NULL.

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

.. function:: JSON_LENGTH (json_doc [, json path])

  The **JSON_LENGTH** function returns the length of the json element at the given path. If no path argument is given, the returned value is the length of the root json element. Returns NULL if any argument is NULL or if no element exists at the given path.

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

  The **JSON_VALID** function returns 1 if the given val argument is a valid json_doc, 0 otherwise. Returns NULL if argument is NULL.

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
  //TODO: NO_BACKSLASH_ESCAPES, escape explainations 

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
