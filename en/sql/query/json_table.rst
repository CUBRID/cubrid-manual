:meta-keywords: cubrid json, cubrid json_table, database json, json table
:meta-description: CUBRID JSON_TABLE description

*********************************
JSON_TABLE
*********************************

**JSON_TABLE** facilitates transforming jsons into a table-like structures that can be queried similarly as regular tables.
  The transformation generates a single row or multiple rows, by expanding for example the elements of a JSON_ARRAY.

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
	|  name type PATH string path <on_empty> <on_error>
	|  name type EXISTS PATH string path
	|  NESTED [PATH] path COLUMNS <column_list>

    <on_empty>::=
        NULL | ERROR | DEFAULT json_value ON EMPTY

    <on_error>::=
        NULL | ERROR | DEFAULT json_value ON ERROR


The json_doc expr must be an expression that results in a json_doc. This can be a constant json, a table's column or the result of a function or operator.
The json path must be a valid path and is used to extract json data to be evaluated in the COLUMNS clause.
The COLUMNS clause defines output column types and operations performed to get the output.  
The [AS] alias clause is required.


**JSON_TABLE** supports four types of columns:

- name FOR ORDINALITY: this type keeps track of a row's number inside a COLUMNS clause. The column's type is INTEGER.
- name type PATH json path [on empty] [on error]: Columns of this type are used to extract json_values from the specified json paths. The extracted json data is then coerced to the specified type.
  If the path does not exist, json value triggers the on empty clause. The on error clause is triggered if the extracted json value is not coercible to the target type.

  - on empty determines the behavior of JSON_TABLE in case the path does not exist. On empty can have one of the following values:

    - NULL ON EMPTY: the column is set to NULL. This is the default behavior.
    - ERROR ON EMPTY: an error is thrown
    - DEFAULT json_string ON EMPTY: json_string will be used instead of the missing value. Column type rules also apply to the default value.

  - on error can have one of the following values:

    - NULL ON ERROR: the column is set to NULL. This is the default behavior.
    - ERROR ON ERROR: an error is thrown.
    - DEFAULT json_string ON ERROR: json_string will be used instead of the array/object/json scalar that failed coercion to desired column type. Column type rules also apply to the default value. 

- name type EXISTS PATH json path: this returns 1 if any data is present at the json path location, 0 otherwise.

- NESTED [PATH] json path COLUMNS (column list):
  The json path is relative to the parent's path. The parent's path is either JSON_TABLE's json path or the path of the parent NESTED [PATH] clause.


.. code-block:: sql

    SELECT * FROM JSON_TABLE ('{"a":[1,[2,3]]}', '$.a[*]' COLUMNS ( col INT PATH '$')) as jt;
::

                       col
    ======================
                         1 -- first value found at '$.a[*]' is 1 json scalar, which is coercible to 1
                      NULL -- second value found at '$.a[*]' is [2,3] json array which cannot be coerced to int, triggering NULL ON ERROR default behavior

Overriding the default on_error behavior, results in a different output from previous example: 

.. code-block:: sql

    SELECT * FROM JSON_TABLE ('{"a":[1,[2,3]]}', '$.a[*]' COLUMNS ( col INT PATH '$' DEFAULT '-1' ON ERROR)) as jt;
::

                       col
    ======================
                         1 -- first value found at '$.a[*]' is '1' json scalar, which is coercible to 1
                        -1 -- second value found at '$.a[*]' is '[2,3]' json array which cannot be coerced to int, triggering ON ERROR

ON EMPTY example:

.. code-block:: sql

    SELECT * FROM JSON_TABLE ('{"a":1}', '$' COLUMNS ( col1 INT PATH '$.a', col2 INT PATH '$.b', col3 INT PATH '$.c' DEFAULT '0' ON EMPTY)) as jt;

::

             col1         col2         col3
    =======================================
                1         NULL            0 
  Column col2 represents the value found at '$.a' in the given json_doc. Since the path does not exist, ON EMPTY is triggered resulting in NULL as a result.
  The '$.c' extraction also results in an empty result, but the triggered ON EMPTY behavior returns 0 as default value. 

In the example below, '$.*' path will be used to make the parent columns receive root json object's member values one by one. Column a shows what is processed. Each member's value of
the root object will then be processed further by the NESTED [PATH] clause. NESTED PATH uses path '$[*]' take each element of the array to be further processed by its columns.
FOR ORDINALITY columns track the count of the current processed element. In the example's result we can see that for each new element in a column, the ord column's value also gets incremented.
FOR ORDINALITY nested_ord column also acts as a counter of the number of elements processed by sibling columns. The nested FOR ORDINALITY column gets reset after finishing each processing batch.
The third member's value, 6 cannot be treated as an array and therefore cannot be processed by the nested columns. Nested columns will yield NULL values. 

.. code-block:: sql

    SELECT * FROM JSON_TABLE ('{"a":[1,2],"b":[3,4,5],"d":6,"c":[7]}', '$.*'
                  COLUMNS ( ord FOR ORDINALITY, 
                            col JSON PATH '$',
                            NESTED PATH '$[*]' COLUMNS (nested_ord FOR ORDINALITY, nested_col JSON PATH '$'))) as jt;

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

The following example showcases how multiple NESTED [PATH] clauses are treated by the JSON_TABLE. The value to be processed gets passed once, in order, to each of the NESTED [PATH] clauses at the same level.
During processing of a value by a NESTED [PATH] clause, any sibling NESTED [PATH] clauses will fill their column with NULL values.

.. code-block:: sql

    SELECT * FROM JSON_TABLE ('{"a":{"key1":[1,2], "key2":[3,4,5]},"b":{"key1":6, "key2":[7]}}', '$.*'
                  COLUMNS ( ord FOR ORDINALITY,
                            col JSON PATH '$',
                            NESTED PATH '$.key1[*]' COLUMNS (nested_ord1 FOR ORDINALITY, nested_col1 JSON PATH '$'),
                            NESTED PATH '$.key2[*]' COLUMNS (nested_ord2 FOR ORDINALITY, nested_col2 JSON PATH '$'))) as jt;
::

              ord  col                            nested_ord1  nested_col1           nested_ord2  nested_col2         
    ===================================================================================================================
                1  {"key1":[1,2],"key2":[3,4,5]}            1  1                            NULL  NULL                
                1  {"key1":[1,2],"key2":[3,4,5]}            2  2                            NULL  NULL                
                1  {"key1":[1,2],"key2":[3,4,5]}         NULL  NULL                            1  3                   
                1  {"key1":[1,2],"key2":[3,4,5]}         NULL  NULL                            2  4                   
                1  {"key1":[1,2],"key2":[3,4,5]}         NULL  NULL                            3  5                   
                2  {"key1":6,"key2":[7]}                 NULL  NULL                            1  7                   
