:meta-keywords: cubrid regex, cubrid regexp, cubrid rlike, regular expression functions, regex
:meta-description: CUBRID functions related to regular expressions.

:tocdepth: 3

*********************************************
Regular Expressions Functions and Operators
*********************************************

The functions described in this section performs regular expression matching on a string. A regular expression is a powerful way to specify a pattern for a complex search.
CUBRID uses the standard C++ <regex> library, which conforms the ECMA-262 RegExp grammar.

.. contents::

.. _regex-rlike:

REGEXP, RLIKE
=============

The **REGEXP** and **RLIKE** are used interchangeably. It performs a regular expression matcinh of a string. In the below syntax, if *expression* matches *pattern*, 1 is returned; otherwise, 0 is returned. If either *expression* or *pattern* is **NULL**, **NULL** is returned.
The second syntax has the same meaning as the third syntax, which both syntaxes are using **NOT**.

::

    expression REGEXP | RLIKE [BINARY] pattern
    expression NOT REGEXP | RLIKE pattern
    NOT (expression REGEXP | RLIKE pattern)

*   *expression* : Column or input expression
*   *pattern* : Pattern used in regular expressions

The difference between **REGEXP** and **LIKE** are as follows:

*  The **LIKE** operator succeeds only if the pattern matches the entire value.
*  The **REGEXP** operator succeeds if the pattern matches anywhere in the value. To match the entire value, you should use "^" at the beginning and "$" at the end.
*  The **LIKE** operator is case sensitive, but patterns of regular expressions in **REGEXP** is not case sensitive. To enable case sensitive, you should use **REGEXP BINARY** statement.

.. code-block:: sql

    -- When REGEXP is used in SELECT list, enclosing this with parentheses is required. 
    -- But used in WHERE clause, no need parentheses.
    -- case insensitive, except when used with BINARY.
    SELECT name FROM athlete where name REGEXP '^[a-d]';

::
    
    name
    ======================
    'Dziouba Irina'
    'Dzieciol Iwona'
    'Dzamalutdinov Kamil'
    'Crucq Maurits'
    'Crosta Daniele'
    'Bukovec Brigita'
    'Bukic Perica'
    'Abdullayev Namik'

.. code-block:: sql

    -- ^ : match the beginning of a string
    SELECT ('cubrid dbms' REGEXP '^cub');
    
::

    ('cubrid dbms' regexp '^cub')
    ===============================
    1

.. code-block:: sql

    -- $ : match the end of a string
    SELECT ('this is cubrid dbms' REGEXP 'dbms$');
    
::

    ('this is cubrid dbms' regexp 'dbms$')
    ========================================
    1

.. _regex-count:

REGEXP_COUNT
============

.. function:: REGEXP_COUNT (string, pattern_string [, position [, match_type]])

    The **REGEXP_COUNT** function returns the number of occurrences of the regular expression pattern, *pattern_string*, within a given character string, *string*. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param pattern_string: Specifies the regular expression pattern string to be searched. If the value is **NULL**, **NULL** is returned.
    :param position: Specifies the position of the *string* to start the search. If the value is ommitted, the default value 1 is applied. If the value is negative or zero, an error will be returned. If the value is **NULL**, **NULL** is returned
    :param match_type: Specifies the string to change default matching behavior of the function. If the value is ommitted, the default value 'i' is applied. If the value is other than 'c' or 'i', an error will be returned. If the value is **NULL**, **NULL** is returned.
    :rtype: INT

.. code-block:: sql

    -- it returns NULL when an argument is specified with NULL value
    SELECT REGEXP_COUNT('ab123ab111a','[a-d]+',NULL);
    
::

    regexp_count('ab123ab111a','[a-d]+',NULL)
    ======================
      NULL

.. code-block:: sql

    -- an empty string pattern doesn't match with any string
    SELECT REGEXP_COUNT('ab123ab111a','');
    
::

    regexp_count('ab123ab111a','')
    ======================
      0

.. code-block:: sql

    SELECT REGEXP_COUNT('ab123ab111a','[a-d]+',3);
    
::

    regexp_count('ab123ab111a','[a-d]+',3)
    ======================
      2

.. code-block:: sql

    SELECT REGEXP_COUNT('가나123abc가다abc가가','[가-나]+');
    
::

    regexp_count('가나123abc가다abc가가','[가-나]+')
    ======================
      2


.. _regex-instr:

REGEXP_INSTR
============

.. function:: REGEXP_INSTR (string, pattern_string [, position [, occurrence [, return_option [, match_type]]]])

    The **REGEXP_INSTR** function returns the beginning or ending position by searching for a regular expression pattern, *pattern_string*, within a given character string, *string*, and replaces it with a character string. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param pattern_string: Specifies the regular expression pattern string to be searched. If the value is **NULL**, **NULL** is returned.
    :param position: Specifies the position of the *string* to start the search. If the value is ommitted, the default value 1 is applied. If the value is negative or zero, an error will be returned. If the value is **NULL**, **NULL** is returned
    :param occurrence: Specifies the occurrence of replacement. If the value is ommitted, the default value 1 is applied. If the value is negative, an error will be returned. If the value is **NULL**, **NULL** is returned.
    :param return_option: Specifies whether to return the position of the match. If the value is 0, the position of the first character of the match is returned. If the value is 1, the position of the character following the match is returned. If the value is ommitted, the default value 0 is applied. If the value is other than 0 or 1, an error will be returned. If the value is **NULL**, **NULL** is returned.
    :param match_type: Specifies the string to change default matching behavior of the function. If the value is ommitted, the default value 'i' is applied. If the value is other than 'c' or 'i', an error will be returned. If the value is **NULL**, **NULL** is returned.
    :rtype: INT

.. code-block:: sql

    -- it returns NULL when an argument is specified with NULL value
    SELECT REGEXP_INSTR('12345abcdeabcde','[abc]',NULL);
    
::

    regexp_instr('12345abcdeabcde', '[abc]', null)
    ======================
      NULL
     
.. code-block:: sql

    SELECT REGEXP_INSTR('12345가나다라마가나다라마바','[가-다]+');
    
::

    regexp_instr('12345가나다라마가나다라마바','[가-다]+');
    ======================
      6

.. code-block:: sql

    -- it returns the position of the first character of the match.
    SELECT REGEXP_INSTR('12354abc5','[:alpha:]+',1,1,0);
    
::

    regexp_instr('12354abc5','[:alpha:]+', 1, 1, 0);
    ======================
      6


.. code-block:: sql

    -- it returns the position of the character following the match.
    SELECT REGEXP_INSTR('12354abc5','[:alpha:]+',1,1,1);
    
::

    regexp_instr('12354abc5','[:alpha:]+', 1, 1, 1);
    ======================
      9

.. _regex-like:

REGEXP_LIKE
===========

.. function:: REGEXP_LIKE (string, pattern_string [, match_type])

    The **REGEXP_LIKE** function searches for a regular expression pattern, *pattern_string*, within a given character string, *string*. If the pattern matched anywhere in the *string*, 1 is returned. Otherwise, 0 is returned. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param pattern_string: Specifies the regular expression pattern string to be searched. If the value is **NULL**, **NULL** is returned.
    :param match_type: Specifies the string to change default matching behavior of the function. If the value is ommitted, the default value 'i' is applied. If the value is other than 'c' or 'i', an error will be returned. If the value is **NULL**, **NULL** is returned.
    :rtype: INT

.. code-block:: sql

    SELECT REGEXP_LIKE('abbbbc','ab+c');
    
::

    regexp_like('abbbbc', 'ab+c');
    ======================
      1

.. code-block:: sql

    SELECT REGEXP_LIKE('abefd123','가나?다');
    
::

    regexp_like('가나나다','가나?다');
    ======================
      0

.. code-block:: sql

    SELECT REGEXP_LIKE('abbbbc','AB+C', 'c');
    
::

    regexp_like('abbbbc', 'AB+C');
    ======================
      0

.. _regex-replace:

REGEXP_REPLACE
==============

.. function:: REGEXP_REPLACE (string, pattern_string, replacement_string [, position [, occurrence [, match_type]]])

    The **REGEXP_REPLACE** function searches for a regular expression pattern, *pattern_string*, within a given character string, *string*, and replaces it with a character string, *replacement_string*. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param pattern_string: Specifies the regular expression pattern string to be searched. If the value is **NULL**, **NULL** is returned.
    :param replacement_string: Specifies the string to replace the matched string by *pattern_string*. If the value is **NULL**, **NULL** is returned.
    :param position: Specifies the position of the *string* to start the search. If the value is ommitted, the default value 1 is applied. If the value is negative or zero, an error will be returned. If the value is **NULL**, **NULL** is returned
    :param occurrence: Specifies the occurrence of replacement. If the value is ommitted, the default value 0 is applied. If the value is negative, an error will be returned. If the value is **NULL**, **NULL** is returned.
    :param match_type: Specifies the string to change default matching behavior of the function. If the value is ommitted, the default value 'i' is applied. If the value is other than 'c' or 'i', an error will be returned. If the value is **NULL**, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    -- it returns NULL when an argument is specified with NULL value
    SELECT REGEXP_REPLACE('12345abcdeabcde','[a-d]',NULL);
    
::

    regexp_replace('12345abcdeabcde', '[a-d]', null)
    ======================
      NULL
     
.. code-block:: sql

    SELECT REGEXP_REPLACE('12345abcdeabcde','[a-d]+','#');
    
::

    regexp_replace('12345abcdeabcde', '[a-d]+', '#');
    ======================
      '12345#e#e'

.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ko_cs;
    SELECT REGEXP_REPLACE('a1가b2나다라','[가-다]','#',6);
    
::

    regexp_replace('a1가b2나다라', '[가-다]', '#', 6);
    ======================
      'a1가b2##라'

.. _regex-substr:

REGEXP_SUBSTR
=============

.. function:: REGEXP_SUBSTR (string, pattern_string [, position [, occurrence [, match_type]]])

    The **REGEXP_SUBSTR** function extracts a character string matched for a regular expression pattern, *pattern_string*, within a given character string, *string*. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param pattern_string: Specifies the regular expression pattern string to be searched. If the value is **NULL**, **NULL** is returned.
    :param position: Specifies the position of the *string* to start the search. If the value is ommitted, the default value 1 is applied. If the value is negative or zero, an error will be returned. If the value is **NULL**, **NULL** is returned
    :param occurrence: Specifies the occurrence of replacement. If the value is ommitted, the default value 0 is applied. If the value is negative, an error will be returned. If the value is **NULL**, **NULL** is returned.
    :param match_type: Specifies the string to change default matching behavior of the function. If the value is ommitted, the default value 'i' is applied. If the value is other than 'c' or 'i', an error will be returned. If the value is **NULL**, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    -- if pattern is not matched, null is returned
    SELECT REGEXP_SUBSTR('12345abcdeabcde','[k-z]+');
    
::

    regexp_substr('12345abcdeabcde','[k-z]+');
    ======================
      NULL

.. code-block:: sql

    SELECT REGEXP_SUBSTR('Samseong-ro, Gangnam-gu, Seoul',',[^,]+,');
    
::

    regexp_substr('Samseong-ro, Gangnam-gu, Seoul', ',[^,]+,')
    ======================
      ', Gangnam-gu,'
     
.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ko_cs;
    SELECT REGEXP_SUBSTR('삼성로, 강남구, 서울특별시','[[:alpha:]]+',1,2);
    
::

    regexp_substr('삼성로, 강남구, 서울특별시', [[:alpha:]]+', 1, 2);
    ======================
      '강남구'

.. _regex-ecmascript:

ECMAScript Regular Expressions Pattern Syntax
==============================================

CUBRID uses the standard C++ <regex> library, which conforms the ECMA-262 RegExp grammar. The details on the grammar are not described in this section.

The following sub-sections describes basic characteristics of regular expressions with several examples.

Special Pattern Characters
---------------------------

Special pattern characters are characters (or sequences of characters) that have a special meaning when they appear in a regular expression pattern, 
either to represent a character that is difficult to express in a string, or to represent a category of characters. 
Each of these special pattern characters is matched in the target sequence against a single character (unless a quantifier specifies otherwise).

+----------------+----------------------------------------------------------------------------------------------------------+
| Characters     | Description                                                                                              |
+================+==========================================================================================================+
| .              | Any character except line terminators (LF, CR, LS, PS).                                                  |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\t            | A horizontal tab character (same as \\u0009).                                                            |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\n            | A newline (line feed) character (same as \\u000A).                                                       |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\v            | A vertical tab character (same as \\u000B).                                                              |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\f            | A form feed character (same as \\u000C).                                                                 |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\r            | A carriage return character (same as \\u000D)                                                            |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\cletter      | A a control code character whose code unit value is the same as the remainder of dividing                |
|                | the code unit value of letter by 32.                                                                     |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\xhh          | A a character whose code unit value has an hex value equivalent to the two hex digits *hh*.              |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\uhhhh        | A character whose code unit value has an hex value equivalent to the four hex digits *hhhh*.             |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\0            | A null character (same as \\u0000).                                                                      |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\num          | The result of the submatch whose opening parenthesis is the *num*-th. See groups below for more info.    |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\d            | A decimal digit character (same as [[:digit:]]).                                                         |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\D            | Any character that is not a decimal digit character (same as [^[:digit:]]).                              |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\s            | A whitespace character (same as [[:space:]]).                                                            |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\S            | Any character that is not a whitespace character (same as [^[:space:]]).                                 |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\w            | An alphanumeric or underscore character (same as [_[:alnum:]]).                                          |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\W            | Any character that is not an alphanumeric or underscore character (same as [^_[:alnum:]]).               |
+----------------+----------------------------------------------------------------------------------------------------------+
|                | | The character character as it is, without interpreting its special meaning within a regex expression.  |
| \\character    | | Any character can be escaped except those which form any of the special character sequences above.     |
|                | | Needed for: ^ $ \ . * + ? ( ) [ ] { } \|                                                               |
+----------------+----------------------------------------------------------------------------------------------------------+
| \[class\]      | The target character is part of the *class*. see **Character classes** below.                            |
+----------------+----------------------------------------------------------------------------------------------------------+
| \[^class\]     | The target character is not part of the *class*. see **Character classes** below.                        |
+----------------+----------------------------------------------------------------------------------------------------------+

.. code-block:: sql

    -- .: match any character
    SELECT ('cubrid dbms' REGEXP '^c.*$');
    
::

    ('cubrid dbms' regexp '^c.*$')
    ================================
    1


Quantifiers
------------

Quantifiers follow a character or a special pattern character. They can modify the amount of times that character is repeated in the match:

+----------------+-------------------------------------------------------------------------+
| Characters     | Description                                                             |
+================+=========================================================================+
| \*             | The preceding is matched 0 or more times.                               |
+----------------+-------------------------------------------------------------------------+
| \+             | The preceding is matched 1 or more times.                               |
+----------------+-------------------------------------------------------------------------+
| ?              | The preceding is optional (matched either 0 times or once).             |
+----------------+-------------------------------------------------------------------------+
| {*num*}        | The preceding is matched exactly *num* times.                           |
+----------------+-------------------------------------------------------------------------+
| {*num,*}       | The preceding is matched *num* or more times.                           |
+----------------+-------------------------------------------------------------------------+
| {*min,max*}    | The preceding is matched at least *min* times, but not more than *max*. |
+----------------+-------------------------------------------------------------------------+

.. code-block:: sql

    -- a+ : match any sequence of one or more a characters. case insensitive.
    SELECT ('Aaaapricot' REGEXP '^A+pricot');
    
::

    ('Aaaapricot' regexp '^A+pricot')
    ================================
    1

.. code-block:: sql

    -- a? : match either zero or one a character.
    SELECT ('Apricot' REGEXP '^Aa?pricot');
    
::

    ('Apricot' regexp '^Aa?pricot')
    ==========================
    1
    
.. code-block:: sql

    SELECT ('Aapricot' REGEXP '^Aa?pricot');
    
::

    ('Aapricot' regexp '^Aa?pricot')
    ===========================
    1
     
.. code-block:: sql

    SELECT ('Aaapricot' REGEXP '^Aa?pricot');
    
::

    ('Aaapricot' regexp '^Aa?pricot')
    ============================
    0

.. code-block:: sql

    -- (cub)* : match zero or more instances of the sequence abc.
    SELECT ('cubcub' REGEXP '^(cub)*$');
    
::

    ('cubcub' regexp '^(cub)*$')
    ==========================
    1

By default, all these quantifiers are greedy (i.e., they take as many characters that meet the condition as possible). 
This behavior can be overridden to ungreedy (i.e., take as few characters that meet the condition as possible) by adding a question mark (?) after the quantifier.

.. code-block:: sql

    -- (cub)* : match zero or more instances of the sequence abc.
    SELECT ('cubcub' REGEXP '^(cub)*$');
    
::

    ('cubcub' regexp '^(cub)*$')
    ==========================
    1


Groups
------

Groups allow to apply quantifiers to a sequence of characters (instead of a single character). There are two kinds of groups:

+------------------+-------------------------------------------------------+
| Characters       | Description                                           |
+==================+=======================================================+
| (*subpattern*)   | Group which creates a backreference.                  |
+------------------+-------------------------------------------------------+
| (?:*subpattern*) | Passive group which does not create a backreference.  |
+----------------+---------------------------------------------------------+

When a group creates a backreference, the characters that represent the subpattern in the target sequence are stored as a submatch. Each submatch is numbered after the order of appearance of their opening parenthesis (the first submatch is number 1, the second is number 2, and so on...).
These submatches can be used in the regular expression itself to specify that the entire subpattern should appear again somewhere else (see \int in the special characters list). They can also be used in the replacement string or retrieved in the match_results object filled by some regex operations.

Assertions
----------

Assertions are conditions that do not consume characters in the target sequence: they do not describe a character, but a condition that must be fulfilled before or after a character.

+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| Characters      | Description                                                                                                           |
+=================+=======================================================================================================================+
| ^               | The preceding is matched 0 or more times.                                                                             |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| $               | The preceding is matched 1 or more times.                                                                             |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| \b              | The previous character is a word character and the next is a non-word character (or vice-versa).                      |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| \B              | The previous and next characters are both word characters or both are non-word characters.                            |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| (?=*subpattern) | Positive lookahead. The characters following the charcter must match subpattern, but no characters are consumed.      |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| (?!*subpattern) | Negative lookahead. The characters following the assertion must not match subpattern, but no characters are consumed. |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+

Alternatives
------------

A pattern can include different alternatives:

+-----------------+------------------------------------------------------+
| Characters      | Description                                          | 
+=================+======================================================+
| \|              | Separates two alternative patterns or subpatterns.   |
+-----------------+------------------------------------------------------+

A regular expression can contain multiple alternative patterns simply by separating them with the separator operator (|): The regular expression will match if any of the alternatives match, and as soon as one does.
Subpatterns (in groups or assertions) can also use the separator operator to separate different alternatives.

Character classes
-----------------

Character classes syntax matches a category of characters. The character class can contain any combincation of:

- **Individual characters:** Any character specified is considered part of the class (except the characters \, [, ] and - when they have a special meaning as described in the following paragraphs).
- **Ranges:** They can be specified by using the hyphen character (-) between two valid characters.
- **POSIX-like classes:** A whole set of predefined classes can be added to a custom character class. There are three kinds:

+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| Characters      | Description                                                                                                           |
+=================+=======================================================================================================================+
| [:*classname*:] | The character class                                                                                                   |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| [.*classname*.] | The collating sequence doesn't support. CUBRID occurs an error when this syntax is given                              |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| [=*classname*=] | The character equivalents indicates that contain characters should be considered as identical for sorting.            |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+

.. code-block:: sql

    -- [a-dX], [^a-dX] : matches any character that is (or is not, if ^ is used) either a, b, c, d or X.
    SELECT ('aXbc' REGEXP '^[a-dXYZ]+');
    
::

    ('aXbc' regexp '^[a-dXYZ]+')
    ==============================
    1
     
.. code-block:: sql

    SELECT ('strike' REGEXP '^[^a-dXYZ]+$');
    
::

    ('strike' regexp '^[^a-dXYZ]+$')
    ================================
    1

+------------+-----------------------------------------+
| Class      | Description                             |
+============+=========================================+
| [:alnum:]  | Alpha-numerical character               |
+------------+-----------------------------------------+
| [:alpha:]  | Alphabetic character                    |
+------------+-----------------------------------------+
| [:blank:]  | Blank character                         |
+------------+-----------------------------------------+
| [:cntrl:]  | Control character                       |
+------------+-----------------------------------------+
| [:digit:]  | Decimal digit character                 |
+------------+-----------------------------------------+
| [:graph:]  | Character with graphical representation |
+------------+-----------------------------------------+
| [:lower:]  | Lowercase letter                        |
+------------+-----------------------------------------+
| [:print:]  | Printable character                     |
+------------+-----------------------------------------+
| [:punct:]  | Punctuation mark character              |
+------------+-----------------------------------------+
| [:space:]  | Whitespace character                    |
+------------+-----------------------------------------+
| [:upper:]  | Uppercase letter                        |
+------------+-----------------------------------------+
| [:xdigit:] | Hexadecimal digit character             |
+------------+-----------------------------------------+
| [:d:]      | Decimal digit character                 |
+------------+-----------------------------------------+
| [:w:]      | Word character                          |
+------------+-----------------------------------------+
| [:s:]      | Whitespace character                    |
+------------+-----------------------------------------+

.. note::

  The character classes depends on the 