
:meta-keywords: cubrid ascii, cubrid concatenation, cubrid lower, cubrid upper, cubrid chr, cubrid find_in_set, cubrid repeat, cubrid replace, cubrid substr

:tocdepth: 3

******************************
String Functions and Operators
******************************

.. contents::

.. note:: 

    In the string functions, if the value of **oracle_style_empty_string** parameter is yes, CUBRID does not separate an empty string and NULL; according to each function, CUBRID regards all of them as NULL or an empty string. For the detail description, see :ref:`oracle_style_empty_string <oracle_style_empty_string>`.

Concatenation Operator
======================

A concatenation operator gets a character string or bit string data type as an operand and returns a concatenated string. The plus sign (+) and double pipe symbol (||) are provided as concatenation operators for character string data. If **NULL** is specified as an operand, a **NULL** value is returned.

If **pipes_as_concat** that is a parameter related to SQL statement is set to **no** (default value: yes), a double pipe (||) symbol is interpreted as an **OR** operator. If plus_as_concat is set to no (default value: yes), a plus (+) symbol is interpreted as a plus (+) operator. In such case, It is recommended to concatenate strings or bit strings, by using the **CONCAT** function. ::

    <concat_operand1> +  <concat_operand1>
    <concat_operand2> || <concat_operand2>
    
        <concat_operand1> ::=
            bit string |
            NULL
         
        <concat_operand2> ::=
            bit string |
            character string
            NULL

*   <*concat_operand1*>: Left string after concatenation. String or bit string can be specified.
*   <*concat_operand2*>: Right string after concatenation. String or bit string can be specified.

.. code-block:: sql

    SELECT 'CUBRID' || ',' + '2008';
    
::

     'CUBRID'||','+'2008'
    ======================
      'CUBRID,2008'
     
.. code-block:: sql

    SELECT 'cubrid' || ',' || B'0010' ||B'0000' ||B'0000' ||B'1000';
    
::

     'cubrid'||','||B'0010'||B'0000'||B'0000'||B'1000'
    ======================
      'cubrid,2008'
     
.. code-block:: sql

    SELECT ((EXTRACT(YEAR FROM SYS_TIMESTAMP))||(EXTRACT(MONTH FROM SYS_TIMESTAMP)));
    
::

     (( extract(year  from  SYS_TIMESTAMP ))||( extract(month  from  SYS_TIMESTAMP )))
    ======================
      '200812'
     
.. code-block:: sql

    SELECT 'CUBRID' || ',' + NULL;
    
::

     'CUBRID'||','+null
    ======================
      NULL

ASCII
=====

.. function:: ASCII (str)

    The **ASCII** function returns the ASCII code of the most left character in numeric value. If an input string is **NULL**, **NULL** is returned. This **ASCII** function supports single-byte character sets only. If a numeric value is entered, it is converted into character string and then the ASCII code of the most left character is returned.

    :param str: Input string
    :rtype: STRING

.. code-block:: sql

    SELECT ASCII('5');
    
::

    53
    
.. code-block:: sql

    SELECT ASCII('ab');
    
::

    97

BIN
===

.. function:: BIN (n)

    The **BIN** function converts a **BIGINT** type number into binary string. If an input string is **NULL**, **NULL** is returned. When you input the string which cannot be transformed into **BIGINT**, it returns an error if the value of **return_null_on_function_errors** in **cubrid.conf** is no(the default), or returns NULL if it is yes.

    :param n: A **BIGINT** type number
    :rtype: STRING

.. code-block:: sql

    SELECT BIN(12);
    
::

    '1100'

BIT_LENGTH
==========

.. function:: BIT_LENGTH (string)

    The **BIT_LENGTH** function returns the length (bits) of a character string or bit string as an integer value. The return value of the **BIT_LENGTH** function may depend on the character set, because for the character string, the number of bytes taken up by a single character is different depending on the character set of the data input environment (e.g., UTF-8 Korean characters: one Korean character is 3*8 bits). For details about character sets supported by CUBRID, see :ref:`char-data-type`. When you input the invalid value, it returns an error if the value of **return_null_on_function_errors** in **cubrid.conf** is no(the default), or returns NULL if it is yes.

    :param string: Specifies the character string or bit string whose number of bits is to be calculated. If this value is **NULL**, **NULL** is returned. 
    :rtype: INT

.. code-block:: sql

    SELECT BIT_LENGTH('');
    
::

       bit_length('')
    =================
                    0
     
.. code-block:: sql

    SELECT BIT_LENGTH('CUBRID');
    
::

       bit_length('CUBRID')
    =======================
                         48
     
.. code-block:: sql

    -- UTF-8 Korean character
    SELECT BIT_LENGTH('큐브리드');
    
::

         bit_length('큐브리드')
    =========================
                           96
     
.. code-block:: sql

    SELECT BIT_LENGTH(B'010101010');
    
::

       bit_length(B'010101010')
    ===========================
                              9
     
.. code-block:: sql

    CREATE TABLE bit_length_tbl (char_1 CHAR, char_2 CHAR(5), varchar_1 VARCHAR, bit_var_1 BIT VARYING);
    INSERT INTO bit_length_tbl VALUES('', '', '', B''); --Length of empty string
    INSERT INTO bit_length_tbl VALUES('a', 'a', 'a', B'010101010'); --English character
    INSERT INTO bit_length_tbl VALUES(NULL, '큐', '큐', B'010101010'); --UTF-8 Korean character and NULL
    INSERT INTO bit_length_tbl VALUES(' ', ' 큐', ' 큐', B'010101010'); --UTF-8 Korean character and space
     
    SELECT BIT_LENGTH(char_1), BIT_LENGTH(char_2), BIT_LENGTH(varchar_1), BIT_LENGTH(bit_var_1) FROM bit_length_tbl;
     
::

    bit_length(char_1)  bit_length(char_2)      bit_length(varchar_1)   bit_length(bit_var_1)
    ================================================================================
    8                   40                       0                       0
    8                   40                       8                       9
    NULL                56                      24                       9
    8                   40                      32                       9

CHAR_LENGTH, CHARACTER_LENGTH, LENGTHB, LENGTH
==============================================

.. function:: CHAR_LENGTH (string)
.. function:: CHARACTER_LENGTH (string)
.. function:: LENGTHB (string)
.. function:: LENGTH (string)

    **CHAR_LENGTH**, **CHARACTER_LENGTH**, **LENGTHB**, and **LENGTH** are used interchangeably.
    The number of characters is returned as an integer. For details on character set supported by CUBRID, see :doc:`/sql/i18n`.

    :param string: Specifies the string whose length will be calculated according to the number of characters. If the character string is **NULL**, **NULL** is returned.
    :rtype: INT

.. note::

    *   In versions lower than CUBRID 9.0, the multibyte string returns the number of bytes in the string. Therefore, the length of one character is calculated as 2- or 3-bytes according to the charset.
    *   The length of each space character that is included in a character string is one byte.
    *   The length of empty quotes (") to represent a space character is 0. Note that in a  **CHAR** (*n*) type, the length of a space character is *n*, and it is specified as 1 if n is omitted.

.. code-block:: sql

    --character set is UTF-8 for Korean characters
    SELECT LENGTH('');
    
::

    char length('')
    ==================
                     0
     
.. code-block:: sql

    SELECT LENGTH('CUBRID');
    
::

    char length('CUBRID')
    ==================
                     6
     
.. code-block:: sql

    SELECT LENGTH('큐브리드');
    
::

    char length('큐브리드')
    ==================
                     4
     
.. code-block:: sql

    CREATE TABLE length_tbl (char_1 CHAR, char_2 CHAR(5), varchar_1 VARCHAR, varchar_2 VARCHAR);
    INSERT INTO length_tbl VALUES('', '', '', ''); --Length of empty string
    INSERT INTO length_tbl VALUES('a', 'a', 'a', 'a'); --English character
    INSERT INTO length_tbl VALUES(NULL, '큐', '큐', '큐'); --Korean character and NULL
    INSERT INTO length_tbl VALUES(' ', ' 큐', ' 큐', ' 큐'); --Korean character and space
     
    SELECT LENGTH(char_1), LENGTH(char_2), LENGTH(varchar_1), LENGTH(varchar_2) FROM length_tbl;
     
::

    char_length(char_1) char_length(char_2) char_length(varchar_1) char_length(varchar_2)
    ================================================================================
    1                     5                        0             0
    1                     5                        1             1
    NULL                  5                        1             1
    1                     5                        2             2

CHR
===

.. function:: CHR (number_operand [USING charset_name])

    The **CHR** function returns a character that corresponds to the return value of the expression specified as an argument. When you input the code value within invalid ranges, it returns an error if the value of **return_null_on_function_errors** in **cubrid.conf** is no(the default), or returns NULL if it is yes.

    :param number_operand: Specifies an expression that returns a numeric value.
    :param charset_name: Characterset name. It supports utf8 and iso88591.
    :rtype: STRING

.. code-block:: sql

    SELECT CHR(68) || CHR(68-2);
    
::

       chr(68)|| chr(68-2)
    ======================
      'DB'

If you want to get a multibyte character with the **CHR** function, input a number with the valid range of the charset.

.. code-block:: sql

    SELECT CHR(14909886 USING utf8); 
    -- Below query's result is the same as above.
    SET NAMES utf8; 
    SELECT CHR(14909886); 
    
::

       chr(14909886 using utf8) 
    ====================== 
      'ま' 

If you want to get the hexadecimal string from a character, use **HEX** function.

.. code-block:: sql

    SET NAMES utf8; 
    SELECT HEX('ま');

::

       hex(_utf8'ま')
    ======================
      'E381BE'

If you want to get the decimal string from a hexadecimal string, use **CONV** function.

.. code-block:: sql

    SET NAMES utf8; 
    SELECT CONV('E381BE',16,10);
    
::

       conv(_utf8'E381BE', 16, 10)
    ======================
      '14909886'

CONCAT
======

.. function:: CONCAT (string1, string2 [,string3 [, ... [, stringN]...]])

    The     **CONCAT** function has at least one argument specified for it and returns a string as a result of concatenating all argument values. The number of parameters that can be specified is unlimited. Automatic type casting takes place if a non-string type is specified as the argument. If any of the arguments is specified as **NULL**, **NULL** is returned.

    If you want to insert separators between strings specified as arguments for concatenation, use the :func:`CONCAT_WS` Function.

    :param strings: character string
    :rtype: STRING

.. code-block:: sql

    SELECT CONCAT('CUBRID', '2008' , 'R3.0');
    
::

       concat('CUBRID', '2008', 'R3.0')
    ======================
    'CUBRID2008R3.0'
     
.. code-block:: sql

    --it returns null when null is specified for one of parameters
    SELECT CONCAT('CUBRID', '2008' , 'R3.0', NULL);
    
::

       concat('CUBRID', '2008', 'R3.0', null)
    ======================
      NULL
     
     
.. code-block:: sql

    --it converts number types and then returns concatenated strings
    SELECT CONCAT(2008, 3.0);
    
::

       concat(2008, 3.0)
    ======================
      '20083.0'
      
CONCAT_WS
=========

.. function:: CONCAT_WS (string1, string2 [,string3 [, ... [, stringN]...]])

    The **CONCAT_WS** function has at least two arguments specified for it. The function uses the first argument value as the separator and returns the result.

    :param strings: character string
    :rtype: STRING

.. code-block:: sql

    SELECT CONCAT_WS(' ', 'CUBRID', '2008' , 'R3.0');
    
::

    concat_ws(' ', 'CUBRID', '2008', 'R3.0')
    ======================
      'CUBRID 2008 R3.0'
     
.. code-block:: sql

    --it returns strings even if null is specified for one of parameters
    SELECT CONCAT_WS(' ', 'CUBRID', '2008', NULL, 'R3.0');
    
::

    concat_ws(' ', 'CUBRID', '2008', null, 'R3.0')
    ======================
      'CUBRID 2008 R3.0'
     
.. code-block:: sql

    --it converts number types and then returns concatenated strings with separator
    SELECT CONCAT_WS(' ',2008, 3.0);
    
::

    concat_ws(' ', 2008, 3.0)
    ======================
      '2008 3.0'

ELT
===

.. function:: ELT (N, string1, string2, ... )

    If *N* is 1, the **ELT** function returns *string1* and if *N* is 2, it returns *string2*. The return value is a **VARCHAR** type. You can add conditional expressions as needed.

    The maximum byte length of the character string is 33,554,432 and if this length is exceeded, **NULL** will be returned.

    If *N* is 0 or a negative number, an empty string will be returned. If *N* is greater than the number of this input character string, **NULL** will be returned as it is out of range. If *N* is a type that cannot be converted to an integer, an error will be returned.

    :param N: A position of a string to return among the list of strings 
    :param strings: The list of strings
    :rtype: STRING

.. code-block:: sql

    SELECT ELT(3,'string1','string2','string3');
    
::

      elt(3, 'string1', 'string2', 'string3')
    ======================
      'string3'
     
.. code-block:: sql

    SELECT ELT('3','1/1/1','23:00:00','2001-03-04');
    
::

      elt('3', '1/1/1', '23:00:00', '2001-03-04')
    ======================
      '2001-03-04'
     
.. code-block:: sql

    SELECT ELT(-1, 'string1','string2','string3');
    
::

      elt(-1, 'string1','string2','string3')
    ======================
      NULL
     
.. code-block:: sql

    SELECT ELT(4,'string1','string2','string3');
    
::

      elt(4, 'string1', 'string2', 'string3')
    ======================
      NULL
     
.. code-block:: sql

    SELECT ELT(3.2,'string1','string2','string3');
    
::

      elt(3.2, 'string1', 'string2', 'string3')
    ======================
      'string3'
     
.. code-block:: sql

    SELECT ELT('a','string1','string2','string3');
     
::

    ERROR: Cannot coerce 'a' to type bigint.

FIELD
=====

.. function:: FIELD ( search_string, string1 [,string2 [, ... [, stringN]...]])

    The **FIELD** function returns the location index value (position) of a string of *string1*, *string2*. The function returns 0 if it does not have a parameter value which is the same as *search_string*. It returns 0 if *search_string* is **NULL** because it cannot perform the comparison operation with the other arguments.

    If all arguments specified for **FIELD** function are of string type, string comparison operation is performed: if all of them are of number type, numeric comparison operation is performed. If the type of one argument is different from that of another, a comparison operation is performed by casting each argument to the type of the first argument. If type casting fails during the comparison operation with each argument, the function considers the result of the comparison operation as **FALSE** and resumes the other operations.

    :param search_string: A string pattern to search
    :param strings: The list of strings to be searched
    :rtype: INT

.. code-block:: sql

    SELECT FIELD('abc', 'a', 'ab', 'abc', 'abcd', 'abcde');
    
::

       field('abc', 'a', 'ab', 'abc', 'abcd', 'abcde')
    ==================================================
                                                     3
     
.. code-block:: sql

    --it returns 0 when no same string is found in the list
    SELECT FIELD('abc', 'a', 'ab', NULL);
    
::

       field('abc', 'a', 'ab', null)
    ================================
                                   0
     
.. code-block:: sql

    --it returns 0 when null is specified in the first parameter
    SELECT FIELD(NULL, 'a', 'ab', NULL);
    
::

       field(null, 'a', 'ab', null)
    ===============================
                                  0
     
.. code-block:: sql

    SELECT FIELD('123', 1, 12, 123.0, 1234, 12345);
    
::

       field('123', 1, 12, 123.0, 1234, 12345)
    ==========================================
                                             0
     
.. code-block:: sql

    SELECT FIELD(123, 1, 12, '123.0', 1234, 12345);
    
::

       field(123, 1, 12, '123.0', 1234, 12345)
    ==============================================
                                                 3

FIND_IN_SET
===========

.. function:: FIND_IN_SET (str, strlist)

    The **FIND_IN_SET** function looks for the string *str* in the string list *strlist* and returns a position of *str* if it exists. A string list is a string composed of substrings separated by a comma (,). If *str* is not in *strlist* or *strlist* is an empty string, 0 is returned. If either argument is **NULL**, **NULL** is returned. This function does not work properly if *str* contains a comma (,).

    :param str: A string to be searched
    :param strlist: A group of strings separated by a comma
    :rtype: INT

.. code-block:: sql

    SELECT FIND_IN_SET('b','a,b,c,d');
    
::

    2

FROM_BASE64 
=========== 

.. function:: FROM_BASE64(str) 

    **FROM_BASE64** function returns the the decoded result as binary string from the input string encoded as base-64 rule, which is used in **TO_BASE64** function. If the input value is **NULL**, it returns **NULL. When you input the invalid base-64 string, it returns an error if the value of **return_null_on_function_errors** in **cubrid.conf** is no(the default); NULL if this value is yes.
    See :func:`TO_BASE64` for more details on base-64 encoding rules.
     
    :param str: Input string
    :rtype: STRING 

.. code-block:: sql 

    SELECT TO_BASE64('abcd'), FROM_BASE64(TO_BASE64('abcd')); 
     
:: 

       to_base64('abcd') from_base64( to_base64('abcd')) 
    ============================================ 
      'YWJjZA==' 'abcd' 

.. seealso::

    :func:`TO_BASE64`

INSERT
======

.. function:: INSERT ( str, pos, len, string )

    The **INSERT** function inserts a partial character string as long as the length from the specific location of the input character string. The return value is a **VARCHAR** type. The maximum length of the character string is 33,554,432 and if this length is exceeded, **NULL** will be returned.

    :param str: Input character string
    :param pos: *str* location. Starts from 1. If *pos* is smaller than 1 or greater than the length of *string* + 1, the *string* will not be inserted and the *str* will be returned instead.
    :param len: Length of *string* to insert *pos* of *str*. If *len* exceeds the length of the partial character string, insert as many values as *string* in the *pos* of the *str* . If *len* is a negative number, *str* will be the end of the character string.
    :param string: Partial character string to insert to *str*
    :rtype: STRING
    
.. code-block:: sql

    SELECT INSERT('cubrid',2,2,'dbsql');
    
::

      insert('cubrid', 2, 2, 'dbsql')
    ======================
      'cdbsqlrid'
     
.. code-block:: sql

    SELECT INSERT('cubrid',0,3,'db');
    
::

      insert('cubrid', 0, 3, 'db')
    ======================
      'cubrid'
     
.. code-block:: sql

    SELECT INSERT('cubrid',-3,3,'db');
    
::

      insert('cubrid', -3, 3, 'db')
    ======================
      'cubrid'
     
.. code-block:: sql

    SELECT INSERT('cubrid',3,100,'db');
    
::

      insert('cubrid', 3, 100, 'db')
    ======================
      'cudb'
     
.. code-block:: sql

    SELECT INSERT('cubrid',7,100,'db');
    
::

      insert('cubrid', 7, 100, 'db')
    ======================
      'cubriddb'
     
.. code-block:: sql

    SELECT INSERT('cubrid',3,-1,'db');
    
::

      insert('cubrid', 3, -1, 'db')
    ======================
      'cudb'

INSTR
=====

.. function:: INSTR ( string , substring [, position] )

    The **INSTR** function, similarly to the **POSITION**, returns the position of a *substring* within *string*; the position. For the **INSTR** function, you can specify the starting position of the search for *substring* to make it possible to search for duplicate *substring*.

    :param string: Specifies the input character string.
    :param substring: Specifies the character string whose position is to be returned.
    :param position: Optional. Represents the position of a *string* where the search begins in character unit. If omitted, the default value 1 is applied. The first position of the *string* is specified as 1. If the value is negative, the system counts backward from the end of the *string*.
    :rtype: INT
    
.. note::

    In the earlier versions of CUBRID 9.0, position value is returned in byte unit, not character unit. When a multi-byte character set is used, the number of bytes representing one character is different; so the return value may not the same.

.. code-block:: sql

    --character set is UTF-8 for Korean characters
    --it returns position of the first 'b'
    SELECT INSTR ('12345abcdeabcde','b');
    
::

       instr('12345abcdeabcde', 'b', 1)
    ===================================
                                      7
     
.. code-block:: sql

    -- it returns position of the first '나' on UTF-8 Korean charset
    SELECT INSTR ('12345가나다라마가나다라마', '나' );
    
::

       instr('12345가나다라마가나다라마', '나', 1)
    =================================
                                    7
     
.. code-block:: sql

    -- it returns position of the second '나' on UTF-8 Korean charset
    SELECT INSTR ('12345가나다라마가나다라마', '나', 11 );
    
::

       instr('12345가나다라마가나다라마', '나', 11)
    =================================
                                   12
     
.. code-block:: sql

    --it returns position of the 'b' searching from the 8th position
    SELECT INSTR ('12345abcdeabcde','b', 8);
    
::

       instr('12345abcdeabcde', 'b', 8)
    ===================================
                                     12
     
.. code-block:: sql

    --it returns position of the 'b' searching backwardly from the end
    SELECT INSTR ('12345abcdeabcde','b', -1);
    
::

       instr('12345abcdeabcde', 'b', -1)
    ====================================
                                      12
     
.. code-block:: sql

    --it returns position of the 'b' searching backwardly from a specified position
    SELECT INSTR ('12345abcdeabcde','b', -8);
    
::

       instr('12345abcdeabcde', 'b', -8)
    ====================================
                                       7

LCASE, LOWER
============

.. function:: LCASE (string)
.. function:: LOWER (string)

    The functions **LCASE** and **LOWER** are used interchangeably. They convert uppercase characters included in string to lowercase characters.

    :param string: Specifies the string in which uppercase characters are to be converted to lowercase. If the value is **NULL**, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    SELECT LOWER('');
    
::

      lower('')
    ======================
      ''
     
.. code-block:: sql

    SELECT LOWER(NULL);
    
::

      lower(null)
    ======================
      NULL
     
.. code-block:: sql

    SELECT LOWER('Cubrid');
    
::

      lower('Cubrid')
    ======================
      'cubrid'

Note that the **LOWER** function may not work properly by specified collation. For example, when you try to change character Ă used in Romanian as lower character, this function works as follows by collation.

If collation is utf8_bin, this character is not changed.

.. code-block:: sql
    
    SET NAMES utf8 COLLATE utf8_bin;
    SELECT LOWER('Ă');

       lower(_utf8'Ă')
    ======================
      'Ă'

If collation is utf8_ro_RO, 'Ă' can be changed.

.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ro_cs;
    SELECT LOWER('Ă');
    
       lower(_utf8'Ă' COLLATE utf8_ro_cs)
    ======================
      'ă'

For supporting collations in CUBRID, see :ref:`cubrid-all-collation`.

LEFT
====

.. function:: LEFT ( string , length )

    The **LEFT** function returns a length number of characters from the leftmost *string*. If any of the arguments is **NULL**, **NULL** is returned. If a value greater than the *length* of the *string* or a negative number is specified for a length, the entire string is returned. To extract a length number of characters from the rightmost string, use the :func:`RIGHT`.

    :param string: Input string
    :param length: The length of a string to be returned
    :rtype: STRING

.. code-block:: sql

    SELECT LEFT('CUBRID', 3);
    
::

     left('CUBRID', 3)
    ======================
      'CUB'
     
.. code-block:: sql

    SELECT LEFT('CUBRID', 10);
    
::

      left('CUBRID', 10)
    ======================
      'CUBRID'

LOCATE
======

.. function:: LOCATE ( substring, string [, position] )

    The **LOCATE** function returns the location index value of a *substring* within a character string. The third argument *position* can be omitted. If this argument is specified, the function searches for *substring* from the given position and returns the location index value of the first occurrence. If the *substring* cannot be found within the string, 0 is returned. The **LOCATE** function behaves like the :func:`POSITION`, but you cannot use **LOCATE** for bit strings.

    :param substring: A string pattern to search
    :param string: A whole string to be searched
    :param position: Starting position of a whole string to be searched
    :rtype: INT
    
.. code-block:: sql

    --it returns 1 when substring is empty space
    SELECT LOCATE ('', '12345abcdeabcde');
    
::

     locate('', '12345abcdeabcde')
    ===============================
                                 1
     
.. code-block:: sql

    --it returns position of the first 'abc'
    SELECT LOCATE ('abc', '12345abcdeabcde');
    
::

     locate('abc', '12345abcdeabcde')
    ================================
                                   6
     
.. code-block:: sql

    --it returns position of the second 'abc'
    SELECT LOCATE ('abc', '12345abcdeabcde', 8);
    
::

     locate('abc', '12345abcdeabcde', 8)
    ======================================
                                      11
     
.. code-block:: sql

    --it returns 0 when no substring found in the string
    SELECT LOCATE ('ABC', '12345abcdeabcde');
    
::

     locate('ABC', '12345abcdeabcde')
    =================================
                                    0

LPAD
====

.. function:: LPAD ( char1, n, [, char2 ] )

    The **LPAD** function pads the left side of a string until the string length reaches the specified value.

    :param char1: Specifies the string to pad characters to. If *n* is smaller than the length of *char1*, padding is not performed, and *char1* is truncated to length n and then returned. If the value is **NULL**, **NULL** is returned.
    :param n: Specifies the total length of *char1* in bytes. If the value is **NULL**, **NULL** is returned.
    :param char2: Specifies the string to pad to the left until the length of *char1* reaches *n*. If it is not specified, empty characters (' ') are used as a default. If the value is **NULL**, **NULL** is returned.
    :rtype: STRING

.. note::

    In versions lower than CUBRID 9.0, a single character is processed as 2 or 3 bytes in a multi-byte character set environment. If *n* is truncated up to the first byte representing a character according to a value of *char1*, the last byte is removed and a space character (1 byte) is added to the left because the last character cannot be represented normally. When the value is **NULL**, **NULL** is returned as its result.

.. code-block:: sql

    --character set is UTF-8 for Korean characters
     
    --it returns only 3 characters if not enough length is specified
    SELECT LPAD ('CUBRID', 3, '?');
    
::

      lpad('CUBRID', 3, '?')
    ======================
      'CUB'
     
    SELECT LPAD ('큐브리드', 3, '?');
    
::

     lpad('큐브리드', 3, '?')
    ======================
      '큐브리'
     
.. code-block:: sql

    --padding spaces on the left till char_length is 10
    SELECT LPAD ('CUBRID', 10);
    
::

     lpad('CUBRID', 10)
    ======================
      '    CUBRID'
     
.. code-block:: sql

    --padding specific characters on the left till char_length is 10
    SELECT LPAD ('CUBRID', 10, '?');
    
::

     lpad('CUBRID', 10, '?')
    ======================
      '????CUBRID'
     
.. code-block:: sql

    --padding specific characters on the left till char_length is 10
    SELECT LPAD ('큐브리드', 10, '?');
    
::

     lpad('큐브리드', 10, '?')
    ======================
      '??????큐브리드'
     
.. code-block:: sql

    --padding 4 characters on the left
    SELECT LPAD ('큐브리드', LENGTH('큐브리드')+4, '?');
    
::

     lpad('큐브리드',  char_length('큐브리드')+4, '?')
    ======================
      '????큐브리드'

LTRIM
=====

.. function:: LTRIM ( string [, trim_string])

    The **LTRIM** function removes all specified characters from the left-hand side of a string.

    :param string: Enters a string or string-type column to trim. If this value is **NULL**, **NULL** is returned.
    :param trim_string: You can specify a specific string to be removed in the left side of *string*. If it is not specified, empty characters (' ') is automatically specified so that the empty characters in the left side are removed.
    :rtype: STRING

.. code-block:: sql

    --trimming spaces on the left
    SELECT LTRIM ('     Olympic     ');
    
::

      ltrim('     Olympic     ')
    ======================
      'Olympic     '
     
.. code-block:: sql

    --If NULL is specified, it returns NULL
    SELECT LTRIM ('iiiiiOlympiciiiii', NULL);
    
::

      ltrim('iiiiiOlympiciiiii', null)
    ======================
      NULL
     
.. code-block:: sql

    -- trimming specific strings on the left
    SELECT LTRIM ('iiiiiOlympiciiiii', 'i');
    
::

      ltrim('iiiiiOlympiciiiii', 'i')
    ======================
      'Olympiciiiii'

MID
===

.. function:: MID ( string, position, substring_length )

    The **MID** function extracts a string with the length of *substring_length* from a *position* within the *string* and then returns it. If a negative number is specified as a *position* value, the *position* is calculated in a reverse direction from the end of the *string*. **substring_length** cannot be omitted. If a negative value is specified, the function considers this as 0 and returns an empty string.

    The **MID** function is working like the :func:`SUBSTR`, but there are differences in that it cannot be used for bit strings, that the *substring_length* argument must be specified, and that it returns an empty string if a negative number is specified for *substring_length*.

    :param string: Specifies an input character string. If this value is **NULL**, **NULL** is returned.
    :param position: Specifies the starting position from which the string is to be extracted. The position of the first character is 1. It is considered to be 1 even if it is specified as 0. If the input value is **NULL**, **NULL** is returned.
    :param substring_length: Specifies the length of the string to be extracted. If 0 or a negative number is specified, an empty string is returned; if **NULL** is specified, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    CREATE TABLE mid_tbl(a VARCHAR);
    INSERT INTO mid_tbl VALUES('12345abcdeabcde');
     
    --it returns empty string when substring_length is 0
    SELECT MID(a, 6, 0), SUBSTR(a, 6, 0), SUBSTRING(a, 6, 0) FROM mid_tbl;
    
::

      mid(a, 6, 0)          substr(a, 6, 0)       substring(a from 6 for 0)
    ==================================================================
      ''                    ''                    ''
     
.. code-block:: sql

    --it returns 4-length substrings counting from the 6th position
    SELECT MID(a, 6, 4), SUBSTR(a, 6, 4), SUBSTRING(a, 6, 4) FROM mid_tbl;
    
::

      mid(a, 6, 4)          substr(a, 6, 4)       substring(a from 6 for 4)
    ==================================================================
      'abcd'                'abcd'                'abcd'
     
.. code-block:: sql

    --it returns an empty string when substring_length < 0
    SELECT MID(a, 6, -4), SUBSTR(a, 6, -4), SUBSTRING(a, 6, -4) FROM mid_tbl;
    
::

      mid(a, 6, -4)         substr(a, 6, -4)      substring(a from 6 for -4)
    ==================================================================
      ''                    NULL                  'abcdeabcde'
     
.. code-block:: sql

    --it returns 4-length substrings at 6th position counting backward from the end
    SELECT MID(a, -6, 4), SUBSTR(a, -6, 4), SUBSTRING(a, -6, 4) FROM mid_tbl;
    
::

      mid(a, -6, 4)         substr(a, -6, 4)      substring(a from -6 for 4)
    ==================================================================
      'eabc'                'eabc'                '1234'

.. _octet_length:
	  
OCTET_LENGTH
============

.. function:: OCTET_LENGTH ( string )

    The **OCTET_LENGTH** function returns the length (byte) of a character string or bit string as an integer. Therefore, it returns 1 (byte) if the length of the bit string is 8 bits, but 2 (bytes) if the length is 9 bits.

    :param string: Specifies the character or bit string whose length is to be returned in bytes. If the value is **NULL**, **NULL** is returned.
    :rtype: INT

.. code-block:: sql

    --character set is UTF-8 for Korean characters
     
    SELECT OCTET_LENGTH('');
    
::

     octet_length('')
    ==================
                     0
     
.. code-block:: sql

    SELECT OCTET_LENGTH('CUBRID');
    
::

     octet_length('CUBRID')
    ==================
                     6
     
.. code-block:: sql

    SELECT OCTET_LENGTH('큐브리드');
    
::

     octet_length('큐브리드')
    ==================
                     12
     
.. code-block:: sql

    SELECT OCTET_LENGTH(B'010101010');
    
::

     octet_length(B'010101010')
    ==================
                     2
     
.. code-block:: sql

    CREATE TABLE octet_length_tbl (char_1 CHAR, char_2 CHAR(5), varchar_1 VARCHAR, bit_var_1 BIT VARYING);
    INSERT INTO octet_length_tbl VALUES('', '', '', B''); --Length of empty string
    INSERT INTO octet_length_tbl VALUES('a', 'a', 'a', B'010101010'); --English character
    INSERT INTO octet_length_tbl VALUES(NULL, '큐', '큐', B'010101010'); --Korean character and NULL
    INSERT INTO octet_length_tbl VALUES(' ', ' 큐', ' 큐', B'010101010'); --Korean character and space
     
    SELECT OCTET_LENGTH(char_1), OCTET_LENGTH(char_2), OCTET_LENGTH(varchar_1), OCTET_LENGTH(bit_var_1) FROM octet_length_tbl;
    
::

    octet_length(char_1) octet_length(char_2) octet_length(varchar_1) octet_length(bit_var_1)
    ================================================================================
    1                      5                         0                       0
    1                      5                         1                       2
    NULL                   7                         3                       2
    1                      7                         4                       2

POSITION
========

.. function:: POSITION ( substring IN string )

    The **POSITION** function returns the position of a character string corresponding to *substring* within a character string corresponding to *string*.

    An expression that returns a character string or a bit string can be specified as an argument of this function. The return value is an integer greater than or equal to 0. This function returns the position value in character unit for a character string, and in bits for a bit string.

    The **POSITION** function is occasionally used in combination with other functions. For example, if you want to extract a certain string from another string, you can use the result of the **POSITION** function as an input to the **SUBSTRING** function.

    .. note::
    
        The location is returned in the unit of byte, not the character, in version lower than CUBRID 9.0. The multi-byte charset uses different numbers of bytes to express one character, so the result value may differ.

    :param substring: Specifies the character string whose position is to be returned. If the value is an empty character, 1 is returned. If the value is **NULL**, **NULL** is returned.
    :rtype: INT

.. code-block:: sql

    --character set is UTF-8 for Korean characters
     
    --it returns 1 when substring is empty space
    SELECT POSITION ('' IN '12345abcdeabcde');
    
::

      position('' in '12345abcdeabcde')
    ===============================
                                  1
     
.. code-block:: sql

    --it returns position of the first 'b'
    SELECT POSITION ('b' IN '12345abcdeabcde');
    
::

      position('b' in '12345abcdeabcde')
    ================================
                                   7
     
.. code-block:: sql

    -- it returns position of the first '나'
    SELECT POSITION ('나' IN '12345가나다라마가나다라마');
    
::

      position('나' in '12345가나다라마가나다라마')
    =================================
                                    7
     
.. code-block:: sql

    --it returns 0 when no substring found in the string
    SELECT POSITION ('f' IN '12345abcdeabcde');
    
::

      position('f' in '12345abcdeabcde')
    =================================
                                    0
     
.. code-block:: sql

    SELECT POSITION (B'1' IN B'000011110000');
    
::

      position(B'1' in B'000011110000')
    =================================
                                    5

REPEAT
======

.. function:: REPEAT( string, count )

    The **REPEAT** function returns the character string with a length equal to the number of repeated input character strings. The return value is a **VARCHAR** type. The maximum length of the character string is 33,554,432 and if it this length is exceeded, **NULL** will be returned. If one of the parameters is **NULL**, **NULL** will be returned.

    :param substring: Character string
    :param count: Repeat count. If you enter 0 or a negative number, an empty string will be returned and if you enter a non-numeric data type, an error will be returned.
    :rtype: STRING

.. code-block:: sql

    SELECT REPEAT('cubrid',3);
    
::

       repeat('cubrid', 3)
    ======================
      'cubridcubridcubrid'
     
.. code-block:: sql

    SELECT REPEAT('cubrid',32000000);
    
::

       repeat('cubrid', 32000000)
    ======================
      NULL
     
.. code-block:: sql

    SELECT REPEAT('cubrid',-1);
    
::

       repeat('cubrid', -1)
    ======================
      ''
     
.. code-block:: sql

    SELECT REPEAT('cubrid','a');
    
::

    ERROR: Cannot coerce 'a' to type integer.

REPLACE
=======

.. function:: REPLACE ( string, search_string [, replacement_string ] )

    The **REPLACE** function searches for a character string, *search_string*, within a given character string, *string*, and replaces it with a character string, *replacement_string*. If the string to be replaced, *replacement_string* is omitted, all *search_strings* retrieved from *string* are removed. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param search_string: Specifies the string to be searched. If the value is **NULL**, **NULL** is returned
    :param replacement_string: Specifies the string to replace the *search_string*. If this value is omitted, *string* is returned with the *search_string* removed. If the value is **NULL**, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    --it returns NULL when an argument is specified with NULL value
    SELECT REPLACE('12345abcdeabcde','abcde',NULL);
    
::

    replace('12345abcdeabcde', 'abcde', null)
    ======================
      NULL
     
.. code-block:: sql

    --not only the first substring but all substrings into 'ABCDE' are replaced
    SELECT REPLACE('12345abcdeabcde','abcde','ABCDE');
    
::

    replace('12345abcdeabcde', 'abcde', 'ABCDE')
    ======================
      '12345ABCDEABCDE'
     
.. code-block:: sql

    --it removes all of substrings when replace_string is omitted
    SELECT REPLACE('12345abcdeabcde','abcde');
    
::

    replace('12345abcdeabcde', 'abcde')
    ======================
      '12345'

The following shows how to print out the newline as "\\n".
    
.. code-block:: sql

    -- no_backslash_escapes=yes (default)

    CREATE TABLE tbl (cmt_no INT PRIMARY KEY, cmt VARCHAR(1024));
    INSERT INTO tbl VALUES (1234,
    'This is a test for

     new line.');

    SELECT REPLACE(cmt, CHR(10), '\n')
    FROM tbl
    WHERE cmt_no=1234;

::

    This is a test for\n\n new line.

REVERSE
=======

.. function:: REVERSE( string )

    The **REVERSE** function returns *string* converted in the reverse order.

    :param string: Specifies an input character string. If the value is an empty string, empty value is returned. If the value is NULL, NULL is returned.
    :rtype: STRING

.. code-block:: sql

    SELECT REVERSE('CUBRID');
    
::

     reverse('CUBRID')
    ======================
      'DIRBUC'

RIGHT
=====

.. function:: RIGHT ( string , length )

    The **RIGHT** function returns a *length* number of characters from the rightmost *string*. If any of the arguments is **NULL**, **NULL** is returned. If a value greater than the length of the *string* or a negative number is specified for a *length*, the entire string is returned. To extract a length number of characters from the leftmost string, use the :func:`LEFT`.

    :param string: Input string
    :param length: The length of a string to be returned
    :rtype: STRING

.. code-block:: sql

    SELECT RIGHT('CUBRID', 3);
    
::

     right('CUBRID', 3)
    ======================
      'RID'
     
.. code-block:: sql

    SELECT RIGHT ('CUBRID', 10);

::
    
     right('CUBRID', 10)
    ======================
      'CUBRID'

RPAD
====

.. function:: RPAD( char1, n, [, char2 ] ) 

    The **RPAD** function pads the right side of a string until the string length reaches the specified value. 

    :param char1: Specifies the string to pad characters to. If *n* is smaller than the length of *char1*, padding is not performed, and *char1* is truncated to length *n* and then returned. If the value is **NULL**, **NULL** is specified.
    :param n: Specifies the total length of *char1*. If the value is **NULL**, **NULL** is specified.
    :param char2: Specifies the string to pad to the right until the length of *char1* reaches *n*. If it is not specified, empty characters (' ') are used as a default. If the value is **NULL**, **NULL** is returned.
    :rtype: STRING

.. note::

    In versions lower than CUBRID 9.0, a single character is processed as 2 or 3 bytes in a multi-byte character set environment. If *n* is truncated up to the first byte representing a character according to a value of *char1*, the last byte is removed and a space character (1 byte) is added to the right because the last character cannot be represented normally. When the value is **NULL**, **NULL** is returned as its result.

.. code-block:: sql

    --character set is UTF-8 for Korean characters
     
    --it returns only 3 characters if not enough length is specified
    SELECT RPAD ('CUBRID', 3, '?');
    
::

     rpad('CUBRID', 3, '?')
    ======================
      'CUB'
     
.. code-block:: sql

    --on multi-byte charset, it returns the first character only with a right-padded space
    SELECT RPAD ('큐브리드', 3, '?');
    
::

     rpad('큐브리드', 3, '?')
    ======================
      '큐브리'
     
.. code-block:: sql

    --padding spaces on the right till char_length is 10
    SELECT RPAD ('CUBRID', 10);
    
::

     rpad('CUBRID', 10)
    ======================
      'CUBRID    '
     
.. code-block:: sql

    --padding specific characters on the right till char_length is 10
    SELECT RPAD ('CUBRID', 10, '?');
    
::

     rpad('CUBRID', 10, '?')
    ======================
      'CUBRID????'
     
.. code-block:: sql

    --padding specific characters on the right till char_length is 10
    SELECT RPAD ('큐브리드', 10, '?');
    
::

     rpad('큐브리드', 10, '?')
    ======================
      '큐브리드??????'
     
.. code-block:: sql

    --padding 4 characters on the right
    SELECT RPAD ('큐브리드', LENGTH('큐브리드')+4, '?');
    
::

     rpad('',  char_length('')+4, '?')
    ======================
      '큐브리드????'

RTRIM
=====

.. function:: RTRIM ( string [, trim_string])

    The **RTRIM** function removes specified characters from the right-hand side of a string.

    :param string: Enters a string or string-type column to trim. If this value is **NULL**, **NULL** is returned.
    :param trim_string: You can specify a specific string to be removed in the right side of *string*. If it is not specified, empty characters (' ') is automatically specified so that the empty characters in the right side are removed.
    :rtype: STRING

.. code-block:: sql

    SELECT RTRIM ('     Olympic     ');
    
::

     rtrim('     Olympic     ')
    ======================
      '     Olympic'
     
.. code-block:: sql

    --If NULL is specified, it returns NULL
    SELECT RTRIM ('iiiiiOlympiciiiii', NULL);
    
::

     rtrim('iiiiiOlympiciiiii', null)
    ======================
      NULL
     
.. code-block:: sql

    -- trimming specific strings on the right
    SELECT RTRIM ('iiiiiOlympiciiiii', 'i');
    
::

     rtrim('iiiiiOlympiciiiii', 'i')
    ======================
      'iiiiiOlympic'

SPACE
=====

.. function:: SPACE (N)

    The **SPACE** function returns as many empty strings as the number specified. The return value is a **VARCHAR** type.

    :param N: Space count. It cannot be greater than the value specified in the system parameter, **string_max_size_bytes** (default 1048576). If it exceeds the specified value, **NULL** will be returned. The maximum value is 33,554,432; if this length is exceeded, **NULL** will be returned. If you enter 0 or a negative number, an empty string will be returned; if you enter a type that can't be converted to a numeric value, an error will be returned.
    :rtype: STRING

.. code-block:: sql

    SELECT SPACE(8);
    
::

       space(8)
    ======================
      '        '
     
.. code-block:: sql

    SELECT LENGTH(space(1048576));
    
::

       char_length( space(1048576))
    ===============================
                            1048576
     
.. code-block:: sql

    SELECT LENGTH(space(1048577));
    
::

       char_length( space(1048577))
    ===============================
                               NULL
     
.. code-block:: sql

    -- string_max_size_bytes=33554432
    SELECT LENGTH(space('33554432'));
    
::

       char_length( space('33554432'))
    ==================================
                              33554432
     
.. code-block:: sql

    SELECT SPACE('aaa');
     
::

    ERROR: Cannot coerce 'aaa' to type bigint.

STRCMP
======

.. function:: STRCMP( string1 , string2 )

    The **STRCMP** function compares two strings, *string1* and *string2*, and returns 0 if they are identical, 1 if *string1* is greater, or -1 if *string1* is smaller. If any of the parameters is **NULL**, **NULL** is returned.

    :param string1: A string to be compared
    :param string2: A string to be compared
    :rtype: INT

.. code-block:: sql

    SELECT STRCMP('abc', 'abc');

::

    0

.. code-block:: sql

    SELECT STRCMP ('acc', 'abc');

::

    1

.. note::

    Until the previous version of 9.0, STRCMP did not distinguish an uppercase and a lowercase. From 9.0, it compares the strings case-sensitively.    
    To make STRCMP case-insensitive, you should use case-insensitive collation(e.g.: utf8_en_ci).
    
    .. code-block:: sql
    
        -- In previous version of 9.0 STRCMP works case-insensitively
        SELECT STRCMP ('ABC','abc');
        
    ::
        
        0
        
    .. code-block:: sql
    
        -- From 9.0 version, STRCMP distinguish the uppercase and the lowercase when the collation is case-sensitive.
        -- charset is en_US.iso88591
        
        SELECT STRCMP ('ABC','abc');
        
    ::
    
        -1
        
    .. code-block:: sql
    
        -- If the collation is case-insensitive, it does not distinguish the uppercase and the lowercase.
        -- charset is en_US.iso88591

        SELECT STRCMP ('ABC' COLLATE utf8_en_ci ,'abc' COLLATE utf8_en_ci);
        
    ::
    
        0

SUBSTR
======

.. function:: SUBSTR ( string, position [, substring_length])

    The **SUBSTR** function extracts a character string with the length of *substring_length* from a position, *position*, within character string, *string*, and then returns it.

    .. note::
    
        In the previous versions of CUBRID 9.0, the starting position and string length are calculated in byte unit, not in character unit; therefore, in a multi-byte character set, you must specify the parameter in consideration of the number of bytes representing a single character.

    :param string: Specifies the input character string. If the input value is **NULL**, **NULL** is returned.
    :param position: Specifies the position from where the string is to be extracted in bytes. Even though the position of the first character is specified as 1 or a negative number, it is considered as 1. If a value greater than the string length or **NULL** is specified, **NULL** is returned.
    :param substring_length: Specifies the length of the string to be extracted in bytes. If this argument is omitted, character strings between the given position, *position*, and the end of them are extracted. **NULL** cannot be specified as an argument value of this function. If 0 is specified, an empty string is returned; if a negative value is specified, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    --character set is UTF-8 for Korean characters
     
    --it returns empty string when substring_length is 0
    SELECT SUBSTR('12345abcdeabcde',6, 0);
    
::

     substr('12345abcdeabcde', 6, 0)
    ======================
      ''
     
.. code-block:: sql

    --it returns 4-length substrings counting from the position
    SELECT SUBSTR('12345abcdeabcde', 6, 4), SUBSTR('12345abcdeabcde', -6, 4);
    
::

     substr('12345abcdeabcde', 6, 4)   substr('12345abcdeabcde', -6, 4)
    ============================================
      'abcd'                'eabc'
     
.. code-block:: sql

    --it returns substrings counting from the position to the end
    SELECT SUBSTR('12345abcdeabcde', 6), SUBSTR('12345abcdeabcde', -6);
    
::

     substr('12345abcdeabcde', 6)   substr('12345abcdeabcde', -6)
    ============================================
      'abcdeabcde'          'eabcde'
     
.. code-block:: sql

    -- it returns 4-length substrings counting from 11th position
    SELECT SUBSTR ('12345가나다라마가나다라마', 11 , 4);
    
::

     substr('12345가나다라마가나다라마', 11 , 4)
    ======================
      '가나다라'

SUBSTRING
=========

.. function:: SUBSTRING ( string, position [, substring_length]), 
.. function:: SUBSTRING ( string FROM position [FOR substring_length] )

    The **SUBSTRING** function, operating like **SUBSTR**, extracts a character string having the length of *substring_length* from a position, *position*, within character string, *string*, and returns it. If a negative number is specified as the *position* value, the **SUBSTRING** function calculates the position from the beginning of the string. And **SUBSTR** function calculates the position from the end of the string. If a negative number is specified as the *substring_length* value, the **SUBSTRING** function handles the argument is omitted, but the **SUBSTR** function returns **NULL**.

    :param string: Specifies the input character string. If the input value is **NULL**, **NULL** is returned.
    :param position: Specifies the position from where the string is to be extracted. If the position of the first character is specified as 0 or a negative number, it is considered as 1. If a value greater than the string length is specified, an empty string is returned. If **NULL**, **NULL** is returned.
    :param substring_length: Specifies the length of the string to be extracted. If this argument is omitted, character strings between the given position, *position*, and the end of them are extracted. **NULL** cannot be specified as an argument value of this function. If 0 is specified, an empty string is returned; if a negative value is specified, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    SELECT SUBSTRING('12345abcdeabcde', -6 ,4), SUBSTR('12345abcdeabcde', -6 ,4);
    
::

      substring('12345abcdeabcde' from -6 for 4)   substr('12345abcdeabcde', -6, 4)
    ============================================
      '1234'                'eabc'
     
.. code-block:: sql

    SELECT SUBSTRING('12345abcdeabcde', 16), SUBSTR('12345abcdeabcde', 16);
    
::

      substring('12345abcdeabcde' from 16)   substr('12345abcdeabcde', 16)
    ============================================
      ''                    NULL
     
.. code-block:: sql

    SELECT SUBSTRING('12345abcdeabcde', 6, -4), SUBSTR('12345abcdeabcde', 6, -4);
    
::

      substring('12345abcdeabcde' from 6 for -4)   substr('12345abcdeabcde', 6, -4)
    ============================================
      'abcdeabcde'          NULL

SUBSTRING_INDEX
===============

.. function:: SUBSTRING_INDEX (string, delim, count)

    The **SUBSTRING_INDEX** function counts the separators included in the partial character string and will return the partial character string before the *count*\ -th separator. The return value is a **VARCHAR** type.

    :param string: Input character string. The maximum length is 33,554,432 and if this length is exceeded, **NULL** will be returned.
    :param delim: Delimiter. It is case-sensitive.
    :param count: Delimiter occurrence count. If you enter a positive number, it counts the character string from the left and if you enter a negative number, it counts it from the right. If it is 0, an empty string will be returned. If the type cannot be converted, an error will be returned.
    :rtype: STRING

.. code-block:: sql

    SELECT SUBSTRING_INDEX('www.cubrid.org','.','2');
    
::

      substring_index('www.cubrid.org', '.', '2')
    ======================
      'www.cubrid'
     
.. code-block:: sql

    SELECT SUBSTRING_INDEX('www.cubrid.org','.','2.3');
    
::

      substring_index('www.cubrid.org', '.', '2.3')
    ======================
      'www.cubrid'
     
.. code-block:: sql

    SELECT SUBSTRING_INDEX('www.cubrid.org',':','2.3');
    
::

      substring_index('www.cubrid.org', ':', '2.3')
    ======================
      'www.cubrid.org'
     
.. code-block:: sql

    SELECT SUBSTRING_INDEX('www.cubrid.org','cubrid',1);
    
::

      substring_index('www.cubrid.org', 'cubrid', 1)
    ======================
      'www.'
     
.. code-block:: sql

    SELECT SUBSTRING_INDEX('www.cubrid.org','.',100);
    
::

      substring_index('www.cubrid.org', '.', 100)
    ======================
      'www.cubrid.org'

TO_BASE64 
=========

.. function:: TO_BASE64(str) 

    Returns the result as the transformed base-64 string. If the input argument is not a string, it is changed into a string before it is transformed. If the input argument is **NULL**, it returns **NULL**. The base-64 encoded string can be decoded with :func:`FROM_BASE64` function.
     
    :param str: Input string
    :rtype: STRING 

.. code-block:: sql 

    SELECT TO_BASE64('abcd'), FROM_BASE64(TO_BASE64('abcd')); 
     
:: 

       to_base64('abcd') from_base64( to_base64('abcd')) 
    ============================================ 
      'YWJjZA==' 'abcd' 

The following is rules for :func:`TO_BASE64` function and :func:`FROM_BASE64`.

*   The encoded character for the alphabet value 62 is '+'.
*   The encoded character for the alphabet value 63 is '/'.
*   The encoded result consists of character groups, and each group has 4 characters which can be printed out. The 3 bytes of the input data are encoded into 4 bytes. If the last group are not filled with 4 characters, '=' character is padded into that group and 4 characters are made.
*   To divide the long output into the several lines, a newline is added into each 76 encoded output characters.
*   Decoding process indicates newline, carriage return, tab, and space and ignore them.

.. seealso::

    :func:`FROM_BASE64`

TRANSLATE
=========

.. function:: TRANSLATE ( string, from_substring, to_substring )

    The **TRANSLATE** function replaces a character into the character specified in *to_substring* if the character exists in the specified *string*. Correspondence relationship is determined based on the order of characters specified in *from_substring* and *to_substring*. Any characters in *from_substring* that do not have one on one relationship to *to_substring* are all removed. This function is working like the :func:`REPLACE` but the argument of *to_substring* cannot be omitted in this function.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param from_substring: Specifies the string to be retrieved. If the value is **NULL**, **NULL** is returned.
    :param to_substring: Specifies the character string in the *from_substring* to be replaced. It cannot be omitted. If the value is **NULL**, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    --it returns NULL when an argument is specified with NULL value
    SELECT TRANSLATE('12345abcdeabcde','abcde', NULL);

::
    
      translate('12345abcdeabcde', 'abcde', null)
    ======================
      NULL
     
.. code-block:: sql

    --it translates 'a','b','c','d','e' into '1', '2', '3', '4', '5' respectively
    SELECT TRANSLATE('12345abcdeabcde', 'abcde', '12345');
    
::

      translate('12345abcdeabcde', 'abcde', '12345')
    ======================
      '123451234512345'
     
.. code-block:: sql

    --it translates 'a','b','c' into '1', '2', '3' respectively and removes 'd's and 'e's
    SELECT TRANSLATE('12345abcdeabcde','abcde', '123');
    
::

      translate('12345abcdeabcde', 'abcde', '123')
    ======================
      '12345123123'
     
.. code-block:: sql

    --it removes 'a's,'b's,'c's,'d's, and 'e's in the string
    SELECT TRANSLATE('12345abcdeabcde','abcde', '');
    
::

      translate('12345abcdeabcde', 'abcde', '')
    ======================
      '12345'
     
.. code-block:: sql

    --it only translates 'a','b','c' into '3', '4', '5' respectively
    SELECT TRANSLATE('12345abcdeabcde','ABabc', '12345');
    
::

      translate('12345abcdeabcde', 'ABabc', '12345')
    ======================
      '12345345de345de'

TRIM
====

.. function:: TRIM ( [ [ LEADING | TRAILING | BOTH ] [ trim_string ] FROM ] string )

    The **TRIM** function removes specific characters which are located before and after the string.

    :param trim_string: Specifies a specific string to be removed that is in front of or at the back of the target string. If it is not specified, an empty character (' ') is automatically specified so that spaces in front of or at the back of the target string are removed.
    :param string: Enters a string or string-type column to trim. If this value is **NULL**, **NULL** is returned.
    :rtype: STRING

*   **[LEADING|TRAILING|BOTH]** : You can specify an option to trim a specified string that is in a certain position of the target string. If it is **LEADING**, trimming is performed in front of a character string if it is **TRAILING**, trimming is performed at the back of a character string if it is **BOTH**, trimming is performed in front and at the back of a character string. If the option is not specified, **BOTH** is specified by default.

*   The character string of *trim_string* and *string* should have the same character set.

.. code-block:: sql

    --trimming NULL returns NULL
    SELECT TRIM (NULL);
    
::

     trim(both  from null)
    ======================
      NULL
     
.. code-block:: sql

    --trimming spaces on both leading and trailing parts
    SELECT TRIM ('     Olympic     ');
    
::

     trim(both  from '     Olympic     ')
    ======================
      'Olympic'
     
.. code-block:: sql

    --trimming specific strings on both leading and trailing parts
    SELECT TRIM ('i' FROM 'iiiiiOlympiciiiii');
    
::

     trim(both 'i' from 'iiiiiOlympiciiiii')
    ======================
      'Olympic'
     
.. code-block:: sql

    --trimming specific strings on the leading part
    SELECT TRIM (LEADING 'i' FROM 'iiiiiOlympiciiiii');
    
::

     trim(leading 'i' from 'iiiiiOlympiciiiii')
    ======================
      'Olympiciiiii'
     
.. code-block:: sql

    --trimming specific strings on the trailing part
    SELECT TRIM (TRAILING 'i' FROM 'iiiiiOlympiciiiii');
    
::

     trim(trailing 'i' from 'iiiiiOlympiciiiii')
    ======================
      'iiiiiOlympic'

UCASE, UPPER
============

.. function:: UCASE ( string )
.. function:: UPPER ( string )

    The function **UCASE** or **UPPER** converts lowercase characters that are included in a character string to uppercase characters.

    :param string: Specifies the string in which lowercase characters are to be converted to uppercase. If the value is **NULL**, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    SELECT UPPER('');
    
::

     upper('')
    ======================
      ''
     
.. code-block:: sql

    SELECT UPPER(NULL);
    
::

     upper(null)
    ======================
      NULL
     
.. code-block:: sql

    SELECT UPPER('Cubrid');
    
::

     upper('Cubrid')
    ======================
      'CUBRID'

Note that the **UPPER** function may not work properly by specified collation. For example, when you try to change character 'ă' used in Romanian as upper character, this function works as follows by collation.

If collation is utf8_bin, it is not changed.

.. code-block:: sql
    
    SET NAMES utf8 COLLATE utf8_bin;
    SELECT UPPER('ă');
    
       upper(_utf8'ă')
    ======================
      'ă'

If collation is utf8_ro_RO, this can be changed.

.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ro_cs;
    SELECT UPPER('ă');
    
       upper(_utf8'ă' COLLATE utf8_ro_cs)
    ======================
      'Ă'

Regarding collations which CUBRID supports, see :ref:`cubrid-all-collation`.
