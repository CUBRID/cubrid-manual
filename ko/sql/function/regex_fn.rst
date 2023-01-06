:meta-keywords: cubrid regex, cubrid regexp, cubrid rlike, regular expression functions, regex
:meta-description: CUBRID functions related to regular expressions.

:tocdepth: 3

*********************************************
정규 표현식 함수와 연산자
*********************************************

정규 표현식은 복잡한 검색 패턴을 표현하는 강력한 방법이다.
이 섹션에서는 정규 표현식을 이용한 패턴을 매칭하는 함수 및 연산자를 설명한다.

.. contents::

.. _regex-ecmascript:

ECMAScript 정규 표현식 문법
==============================================

정규 표현식을 구현하기 위해 CUBRID는 `ECMA-262 정규식 문법 <http://ecma-international.org/ecma-262/5.1/#sec-15.10>`_ 을 따르는 C++ <regex> 표준 라이브러리르 사용한다.
다음 하위 섹션에서 지원되는 정규식 문법을 예시와 함께 설명한다.

.. note::

  **호환성 고려사항**
  
  CUBRID 11 이전 버전에서 CUBRID는 Henry Spencer의 정규식 구현을 사용하였다.
  CUBRID 11부터 CUBRID는 C++ <regex> 표준 라이브러리를 사용하여 정규식 함수와 연산자를 지원한다.

  \1. Henry Spencer의 정규식 구현은 바이트 방식으로 작동한다. 따라서 REGEXP 및 RLIKE는 멀티바이트를 지원하지 않았다.
  따라서 인자의 콜레이션을 고려하지 않고 ASCII 인코딩으로만 작동했다.
  
  \2. Henry Spencer 라이브러리는 POSIX의 *collating sequence* (*[.character.]*) 표현식을 지원했지만 더 이상 지원하지 않는다.
  또한 *character equivalents* (*[=word=]*) 문법도 지워낳지 않는다. 이러한 문법을 가진 표현식이 주어지면 CUBRID는 에러를 반환한다.
  
  \3. Henry Spencer 라이브러리는 점 연산자 (.)로 line-terminator를 매치한다. 그러나 C++ <regex>는 매치되지 않는다.

  \4. word-beginning boundary 와 word-end boundary (각각 [[:<:]] 와 [[:>:]]) 문법을 지원하지 않는다. 대신, word boundary notation (\\b) 을 사용할 수 있다.

.. note::

  **멀티바이트 문자열 비교 고려사항**

  C++ <regex>는 시스템 로케일에 따라 C++ <locale>에 의해 멀티바이트 비교를 수행한다. 따라서 로케일에 민감한 기능을 사용하려면 해당 시스템 로케일이 설치되어 있어야 합니다.

Special Pattern Characters
---------------------------

Special pattern characters are characters (or sequences of characters) that have a special meaning when they appear in a regular expression pattern, 
either to represent a character that is difficult to express in a string, or to represent a category of characters. 
Each of these special pattern characters is matched in a string against a single character (unless a quantifier specifies otherwise).

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
| \\c\ *letter*  | A control code character whose code unit value is the same as the remainder of dividing                  |
|                | the code unit value of *letter* by 32.                                                                   |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\x\ *hh*      | A a character whose code unit value has an hex value equivalent to the two hex digits *hh*.              |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\u\ *hhhh*    | A character whose code unit value has an hex value equivalent to the four hex digits *hhhh*.             |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\0            | A null character (same as \\u0000).                                                                      |
+----------------+----------------------------------------------------------------------------------------------------------+
| \\\ *num*      | The result of the submatch whose opening parenthesis is the *num*-th. See groups below for more info.    |
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
|                | | The *character* character as it is, without interpreting its special meaning within a regex expression.|
| \\\ *character*| | Any character can be escaped except those which form any of the special character sequences above.     |
|                | | Needed for: ^ $ \\ . * + ? ( ) [ ] { } \|                                                              |
+----------------+----------------------------------------------------------------------------------------------------------+
| \[\ *class*\]  | A string is part of the *class*. see **POSIX-based character classes** below.                            |
+----------------+----------------------------------------------------------------------------------------------------------+
| \[^\ *class*\] | A string is not part of the *class*. see **POSIX-based character classes** below.                        |
+----------------+----------------------------------------------------------------------------------------------------------+

.. code-block:: sql

    -- .: match any character
    SELECT ('cubrid dbms' REGEXP '^c.*$');
    
::

    ('cubrid dbms' regexp '^c.*$')
    ================================
      1

To match special characters such as "\\n", "\\t", "\\r", and "\\\\", some must be escaped with the backslash (\\) by specifying the value of **no_backslash_escapes** (default: yes) to **no**. 
For details on **no_backslash_escapes**, see :ref:`escape-characters`.

.. code-block:: sql

    -- \n : match a special character, when no_backslash_escapes=yes (default)
    SELECT ('new\nline' REGEXP 'new\\nline'); 

::
    
    ('new\nline' REGEXP 'new\\nline'); 
    =====================================
      1

.. code-block:: sql

    -- \n : match a special character, when no_backslash_escapes=no
    SELECT ('new\nline' REGEXP 'new
    line');

::
    
    ('new
    line' regexp 'new
    line')
    =====================================
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

By default, all these quantifiers perform in a *greedy* way which takes as many characters that meet the condition as possible. 
And this behavior can be overridden to *non-greedy* by adding a question mark (?) after the quantifier.

.. code-block:: sql

    -- (a+), (a+?) : match with quantifiers performs greedy and ungreedy respectively.
    SELECT REGEXP_SUBSTR ('aardvark', '(a+)'), REGEXP_SUBSTR ('aardvark', '(a+?)');
    
::

    regexp_substr('aardvark', '(a+)')  regexp_substr('aardvark', '(a+?)')
    ============================================
      'aa'                  'a'

Groups
------

Groups allow to apply quantifiers to a sequence of characters (instead of a single character). There are two kinds of groups:

+--------------------+-------------------------------------------------------+
| Characters         | Description                                           |
+====================+=======================================================+
| (\ *subpattern*)   | Group which creates a backreference.                  |
+--------------------+-------------------------------------------------------+
| (?:\ *subpattern*) | Passive group which does not create a backreference.  |
+--------------------+-------------------------------------------------------+

.. code-block:: sql

    -- The captured group can be referenced with $int
    SELECT REGEXP_REPLACE ('hello cubrid','([[:alnum:]]+)','$1!');

::

    regexp_replace('hello cubrid','([[:alnum:]]+)','$1!')
    ==========================
      'hello! cubrid!'

When a group creates a backreference, the characters that represent the subpattern in a string are stored as a submatch. Each submatch is numbered after the order of appearance of their opening parenthesis (the first submatch is number 1, the second is number 2, and so on...).
These submatches can be used in the regular expression itself to specify that the entire subpattern should appear again somewhere else (see \int in the special characters list). They can also be used in the replacement string or retrieved in the match_results object filled by some regex operations.

.. code-block:: sql

    -- performs regexp_substr without groups. the following is the case that fully matched.
    SELECT REGEXP_SUBSTR ('abckabcjabc', '[a-c]{3}k[a-c]{3}j[a-c]{3}');

    -- ([a-c]{3}) creates a backreference, \1
    SELECT REGEXP_SUBSTR ('abckabcjabc', '([a-c]{3})k\1j\1');

::

    regexp_substr('abckabcjabc', '[a-c]{3}k[a-c]{3}j[a-c]{3}')
    ======================
      'abckabcjabc'

    regexp_substr('abckabcjabc', '([a-c]{3})k\1j\1')
    ======================
      'abckabcjabc'

Assertions
----------

Assertions are conditions that do not consume characters in a string: they do not describe a character, but a condition that must be fulfilled before or after a character.

+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| Characters      | Description                                                                                                           |
+=================+=======================================================================================================================+
| ^               | The beginning of a string, or follows a line terminator                                                               |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| $               | The end of a string, or precedes a line terminator                                                                    |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| \\b             | The previous character is a word character and the next is a non-word character (or vice-versa).                      |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| \\B             | The previous and next characters are both word characters or both are non-word characters.                            |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| (?=subpattern)  | Positive lookahead. The characters following the charcter must match subpattern, but no characters are consumed.      |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| (?!subpattern)  | Negative lookahead. The characters following the assertion must not match subpattern, but no characters are consumed. |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+

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

.. code-block:: sql

    -- (?=subpattern): positive lookahead
    SELECT REGEXP_REPLACE ('cubrid dbms cubrid sql cubrid rdbms', 'cubrid(?= sql)', 'CUBRID');

    -- (?!subpattern): nagative lookahead
    SELECT REGEXP_REPLACE ('cubrid dbms cubrid sql cubrid rdbms', 'cubrid(?! sql)', 'CUBRID');
    
::

   regexp_replace('cubrid dbms cubrid sql cubrid rdbms', 'cubrid(?= sql)', 'CUBRID')
   ======================
     'cubrid dbms CUBRID sql cubrid rdbms'

   regexp_replace('cubrid dbms cubrid sql cubrid rdbms', 'cubrid(?! sql)', 'CUBRID')
   ======================
     'CUBRID dbms cubrid sql CUBRID rdbms'

Alternatives
------------

A pattern can include different alternatives:

+-----------------+------------------------------------------------------+
| Characters      | Description                                          | 
+=================+======================================================+
| \|              | Separates two alternative patterns or subpatterns.   |
+-----------------+------------------------------------------------------+

.. code-block:: sql

    -- a|b : matches any character that is either a or b.
    SELECT ('a' REGEXP 'a|b');
    SELECT ('d' REGEXP 'a|b');
    
::

    ('a' regexp 'a|b')
    ==============================
      1

    ('d' regexp 'a|b')
    ==============================
      0

A regular expression can contain multiple alternative patterns simply by separating them with the separator operator (|): The regular expression will match if any of the alternatives match, and as soon as one does.
Subpatterns (in groups or assertions) can also use the separator operator to separate different alternatives.

.. code-block:: sql

    -- a|b|c : matches any character that is either a, b or c.
    SELECT ('a' REGEXP 'a|b|c');
    SELECT ('d' REGEXP 'a|b|c');
    
::

    ('a' regexp 'a|b|c')
    ==============================
      1

    ('d' regexp 'a|b|c')
    ==============================
      0

Character classes
-----------------

Character classes syntax matches one of characters or a category of characters within square brackets.

**Individual characters** 

Any character specified is considered part of the class (except the characters \\, [, ]).

.. code-block:: sql

    -- [abc] : matches any character that is either a, b or c.
    SELECT ('a' REGEXP '[abc]');
    SELECT ('d' REGEXP '[abc]');
    
::

    ('a' regexp '[abc]')
    ==============================
      1

    ('d' regexp '[abc]')
    ==============================
      0

**Ranges** 

To represent a range of characters, use the dash character (-) between two valid characters. 
For example, "[a-z]" matches any alphabet letter whereas "[0-9]" matches any single number.

.. code-block:: sql

    SELECT ('adf' REGEXP '[a-f]');
    SELECT ('adf' REGEXP '[g-z]');
    
::

    ('adf' regexp '[a-f]')
    ================================
      1

    ('adf' regexp '[g-z]')
    ================================
      0

.. code-block:: sql

    -- [0-9]+: matches number sequence in a string
    SELECT REGEXP_SUBSTR ('aas200gjb', '[0-9]+');
    
::

    regexp_substr('aas200gjb', '[0-9]+')
    ======================
      '200'

.. code-block:: sql

    SELECT ('strike' REGEXP '^[^a-dXYZ]+$');
    
::

    ('strike' regexp '^[^a-dXYZ]+$')
    ================================
      1

**POSIX-based character classes**

The POSIX-based character class (*[:classname:]*) defines categories of characters as shown below. [:d:], [:w:] and [:s:] are an extension to the ECMAScript grammar.

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

.. code-block:: sql

    SELECT REGEXP_SUBSTR ('Samseong-ro 86-gil, Gangnam-gu, Seoul 06178', '[[:digit:]]{5}');
    
::

    regexp_substr('Samseong-ro 86-gil, Gangnam-gu, Seoul 06178', '[[:digit:]]{5}')
    ================================
      '06178'

.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ko_cs;
    SELECT REGEXP_REPLACE ('가나다 가나 가나다라', '\b[[:alpha:]]{2}\b', '#');
    
::

    regexp_replace('가나다 가나 가나다라', '\b[[:alpha:]]{2}\b', '#')
    ======================
      '가나다 # 가나다라'

.. _regex-rlike:

REGEXP, RLIKE
=============

**REGEXP**, **RLIKE**\ 는 동일한 의미로 사용되며, 정규 표현식을 이용한 패턴을 매칭한다. 아래의 문법에서, *expression*이 *pattern*과 일치하면 1을 반환한다. 그렇지 않으면 0을 반환한다.
*expression* 또는 *pattern*이 **NULL**이면 **NULL**을 반환한다. 두 번째 구문은 두 구문 모두 **NOT**을 사용하는 세 번째 구문과 동일한 의미를 갖는다.

::

    expression REGEXP | RLIKE [BINARY] pattern
    expression NOT REGEXP | RLIKE pattern
    NOT (expression REGEXP | RLIKE pattern)

*   *expression* : 칼럼 또는 입력 표현식
*   *pattern* : 정규 표현식에 사용될 패턴. 대소문자 구분 없음

**REGEXP**\ 와 **LIKE**\ 의 차이는 다음과 같다.

* **LIKE** 절은 입력값 전체가 패턴과 매칭되어야 성공한다.
* **REGEXP**\ 는 입력값의 일부가 패턴과 매칭되면 성공한다. **REGEXP**\ 에서 전체 값에 대한 패턴 매칭을 하려면, 패턴의 시작에는 "^"을, 끝에는 "$"을 사용해야 한다.
* **LIKE** 절의 패턴은 대소문자를 구분하지만 **REGEXP**\ 에서 정규 표현식의 패턴은 대소문자를 구분하지 않는다. 대소문자를 구분하려면 **REGEXP BINARY** 구문을 사용해야 한다.
* **REGEXP**, **REGEXP BINARY**\ 는 피연산자의 콜레이션을 고려하지 않고 ASCII 인코딩으로 동작한다.

.. code-block:: sql

    -- [a-dX], [^a-dX] : matches any character that is (or is not, if ^ is used) either a, b, c, d or X.
    SELECT ('aXbc' REGEXP '^[a-dXYZ]+');

::
    
    ('aXbc' regexp '^[a-dXYZ]+')
    ==============================
    1

.. code-block:: sql

    -- SELECT 리스트에서 REGEXP를 사용하는 경우 괄호로 묶어야 한다.
    -- 단, WHERE 절에서는 괄호가 필요하지 않다.
    -- BINARY와 함께 사요하는 경우를 제외하고 대소문자를 구분하지 않는다.
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

.. _regex-count:

REGEXP_COUNT
============

.. function:: REGEXP_COUNT (string, pattern_string [, position [, match_type]])

    **REGEXP_COUNT** 함수는 주어진 문자열 *string* 내에서 정규식 패턴 *pattern_string*의 매칭 횟수를 반환한다. **NULL**\이 인수로 지정된 경우 **NULL**\을 반환한다.

    :param string: 입력 문자열을 지정한다. 값이 **NULL**\이면 **NULL**\을 반환한다
    :param pattern_string: 매칭을 수행할 정규식 패턴 문자열을 지정한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :param position: 매칭을 수행할 *string*\의 위치를 지정한다. 값을 생략하면 기본값 1이 적용된다. 값이 음수이거나 0이면 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다
    :param match_type: 함수의 매칭 옵션을 설정할 문자열을 지정한다.  값을 생략하면 기본값 'i'가 적용된다. 값이 'c' 또는 'i'가 아닌 경우 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :rtype: INT

.. code-block:: sql

    -- 인수가 NULL 값으로 지정되면 NULL을 반환한다
    SELECT REGEXP_COUNT('ab123ab111a','[a-d]+',NULL);
    
::

    regexp_count('ab123ab111a','[a-d]+',NULL)
    ======================
      NULL

.. code-block:: sql

    -- 빈 문자열 패턴은 어떤 문자열과도 일치하지 않는다
    SELECT REGEXP_COUNT('ab123ab111a','');
    
::

    regexp_count('ab123ab111a','')
    ======================
      0

.. code-block:: sql

    SELECT REGEXP_COUNT('ab123Ab111aAA','[a-d]', 3);
    
::

    regexp_count('ab123Ab111aAA', '[a-d]', 3)
    ===========================================
                                            5

.. code-block:: sql

    -- 대소문자 구분 안함 옵션('i')이 기본값이다
    SELECT REGEXP_COUNT('ab123Ab111aAA','[a-d]', 3, 'i');

    -- 대소문자 구분 옵션('c')이 match_type으로 지정된 경우 A는 매칭되지 않는다.
    SELECT REGEXP_COUNT('ab123Ab111aAA','[a-d]', 3, 'c');
    
    
::

    regexp_count('ab123Ab111aAA', '[a-d]', 3, 'i')
    ================================================
                                                 5

    regexp_count('ab123Ab111aAA', '[a-d]', 3, 'c')
    ================================================
                                                 2

.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ko_cs;
    SELECT REGEXP_COUNT('가나123abc가다abc가가','[가-나]+');
    
::

    regexp_count('가나123abc가다abc가가','[가-나]+')
    ======================
      2


.. _regex-instr:

REGEXP_INSTR
============

.. function:: REGEXP_INSTR (string, pattern_string [, position [, occurrence [, return_option [, match_type]]]])

    **REGEXP_INSTR** 함수는 주어진 문자열 *string* 내에서 정규식 패턴 *pattern_string*을 검색하여 시작 위치 또는 끝 위치를 반환한다. **NULL**\이 인수로 지정된 경우 **NULL**\을 반환한다.

    :param string: 입력 문자열을 지정한다. 값이 **NULL**\이면 **NULL**\을 반환한다
    :param pattern_string: 매칭을 수행할 정규식 패턴 문자열을 지정한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :param position: 매칭을 수행할 *string*\의 위치를 지정한다. 값을 생략하면 기본값 1이 적용된다. 값이 음수이거나 0이면 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다
    :param occurrence: 몇 번째 매칭을 사용할 지 지정한다. 값을 생략하면 기본값 1을 적용한다. 값을 생략하면 기본값 1이 적용된다. 값이 음수이면 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :param return_option: 일치하는 문자열의 시작 또는 끝 지점 중 어떤 위치를 반환할지 여부를 지정한다. 만약 값이 0이면 일치하는 문자의 시작 위치를 반환한다. 값이 1이면 일치하는 문자의 끝 위치를 반환한다. 값을 생략하면 기본값 0이 적용된다. 값이 0 또는 1이 아닌 경우 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :param match_type: 함수의 매칭 옵션을 설정할 문자열을 지정한다.  값을 생략하면 기본값 'i'가 적용된다. 값이 'c' 또는 'i'가 아닌 경우 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :rtype: INT

.. code-block:: sql

    -- 인수가 NULL 값으로 지정되면 NULL을 반환한다
    SELECT REGEXP_INSTR('12345abcdeabcde','[abc]',NULL);
    
::

    regexp_instr('12345abcdeabcde', '[abc]', null)
    ======================
      NULL

.. code-block:: sql

    -- 빈 문자열 패턴은 어떤 문자열과도 일치하지 않는다
    SELECT REGEXP_INSTR('12345abcdeabcde','');
    
::

    regexp_instr('12345abcdeabcde', '')
    ======================
      0

.. code-block:: sql

    -- 매칭되는 첫 번째 문자의 위치를 반환한다
    SELECT REGEXP_INSTR('12354abc5','[:alpha:]+',1,1,0);
    
::

    regexp_instr('12354abc5','[:alpha:]+', 1, 1, 0);
    ======================
      6


.. code-block:: sql

    -- 매칭 후 일치하는 문자열의 위치를 반환한다
    SELECT REGEXP_INSTR('12354abc5','[:alpha:]+',1,1,1);
    
::

    regexp_instr('12354abc5','[:alpha:]+', 1, 1, 1);
    ======================
      9

.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ko_cs;
    SELECT REGEXP_INSTR('12345가나다라마가나다라마바','[가-다]+');
    
::

    regexp_instr('12345가나다라마가나다라마바','[가-다]+');
    ======================
      6

.. _regex-like:

REGEXP_LIKE
===========

.. function:: REGEXP_LIKE (string, pattern_string [, match_type])

    **REGEXP_LIKE** 함수는 주어진 문자열 *string* 내에서 정규식 패턴 *pattern_string*을 검색한다. 정규식 패턴이 *pattern_string*의 어느 곳에서나 일치하면 1이 반환된다. 그렇지 않으면 0이 반환된다. **NULL**\이 인수로 지정된 경우 **NULL**\을 반환한다.

    :param string: 입력 문자열을 지정한다. 값이 **NULL**\이면 **NULL**\을 반환한다
    :param pattern_string: 매칭을 수행할 정규식 패턴 문자열을 지정한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :param match_type: 함수의 매칭 옵션을 설정할 문자열을 지정한다.  값을 생략하면 기본값 'i'가 적용된다. 값이 'c' 또는 'i'가 아닌 경우 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :rtype: INT

.. code-block:: sql

    SELECT REGEXP_LIKE('abbbbc','ab+c');
    
::

    regexp_like('abbbbc', 'ab+c');
    ======================
      1

.. code-block:: sql

    -- 빈 문자열 패턴은 어떤 문자열과도 일치하지 않는다
    SELECT REGEXP_LIKE('abbbbc','');
    
::

    regexp_like('abbbbc', '');
    ======================
      0

.. code-block:: sql

    SELECT REGEXP_LIKE('abbbbc','AB+C', 'c');
    
::

    regexp_like('abbbbc', 'AB+C');
    ======================
      0

.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ko_cs;
    SELECT REGEXP_LIKE('가나다','가나?다');
    SELECT REGEXP_LIKE('가나라다','가나?다');
    
::

    regexp_like('가나다', '가나?다')
    ===============================
      1

    regexp_like('가나라다, '가나?다')
    ================================
      0

.. _regex-replace:

REGEXP_REPLACE
==============

.. function:: REGEXP_REPLACE (string, pattern_string, replacement_string [, position [, occurrence [, match_type]]])

    The **REGEXP_REPLACE** function searches for a regular expression pattern, *pattern_string*, within a given character string, *string*, and replaces it with a character string, *replacement_string*. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: 입력 문자열을 지정한다. 값이 **NULL**\이면 **NULL**\을 반환한다
    :param pattern_string: 매칭을 수행할 정규식 패턴 문자열을 지정한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :param replacement_string: Specifies the string to replace the matched string by *pattern_string*. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :param position: 매칭을 수행할 *string*\의 위치를 지정한다. 값을 생략하면 기본값 1이 적용된다. 값이 음수이거나 0이면 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다
    :param occurrence: 몇 번째 매칭을 사용할 지 지정한다. 값을 생략하면 기본값 1을 적용한다. If the value is ommitted, the default value 0 is applied. 값이 음수이면 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :param match_type: 함수의 매칭 옵션을 설정할 문자열을 지정한다.  값을 생략하면 기본값 'i'가 적용된다. 값이 'c' 또는 'i'가 아닌 경우 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :rtype: STRING

.. code-block:: sql

    -- 인수가 NULL 값으로 지정되면 NULL을 반환한다
    SELECT REGEXP_REPLACE('12345abcdeabcde','[a-d]',NULL);
    
::

    regexp_replace('12345abcdeabcde', '[a-d]', null)
    ======================
    NULL

.. code-block:: sql

    -- 빈 문자열 패턴은 어떤 문자열과도 일치하지 않는다
    SELECT REGEXP_REPLACE('12345abcdeabcde','','#');
    
::

    regexp_replace('12345abcdeabcde', '', '#')
    ======================
      '12345abcdeabcde'

.. code-block:: sql

    SELECT REGEXP_REPLACE('12345abDEKBcde','[a-d]','#');
    
::

    regexp_replace('12345abDEKBcde', '[a-d]', '#')
    ======================
      '12345###EK###e'

.. code-block:: sql

    -- 대소문자 구분 안함 옵션('i')이 기본값이다
    SELECT REGEXP_REPLACE('12345abDEKBcde','[a-d]','#', 1, 0, 'i');

    -- 대소문자 구분 옵션('c')이 match_type으로 지정된 경우 'B' 와 'D'는 매칭되지 않는다.
    SELECT REGEXP_REPLACE('12345abDEKBcde','[a-d]','#', 1, 0, 'c');
    
::

    regexp_replace('12345abDEKBcde', '[a-d]', '#', 1, 0, 'i')
    ======================
      '12345###EK###e'


    regexp_replace('12345abDEKBcde', '[a-d]', '#', 1, 0, 'c')
    ======================
      '12345##DEKB##e'

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

    **REGEXP_SUBSTR** 함수는 주어진 문자열 *string* 내에서 정규식 패턴 *pattern_string*에 일치하는 문자열을 추출한다. **NULL**\이 인수로 지정된 경우 **NULL**\을 반환한다.

    :param string: 입력 문자열을 지정한다. 값이 **NULL**\이면 **NULL**\을 반환한다
    :param pattern_string: 매칭을 수행할 정규식 패턴 문자열을 지정한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :param position: 매칭을 수행할 *string*\의 위치를 지정한다. 값을 생략하면 기본값 1이 적용된다. 값이 음수이거나 0이면 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다
    :param occurrence: 몇 번째 매칭을 사용할 지 지정한다. 값을 생략하면 기본값 0을 적용한다. 이 의미는 발생하는 모든 매칭을 사용한다는 의미이다. 값이 음수이면 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :param match_type: 함수의 매칭 옵션을 설정할 문자열을 지정한다. 값을 생략하면 기본값 'i'가 적용된다. 값이 'c' 또는 'i'가 아닌 경우 에러를 반환한다. 값이 **NULL**\이면 **NULL**\을 반환한다.
    :rtype: STRING

.. code-block:: sql

    -- 만약 패턴이 매칭되지 않으면, NULL을 반환한다
    SELECT REGEXP_SUBSTR('12345abcdeabcde','[k-z]+');
    
::

    regexp_substr('12345abcdeabcde','[k-z]+');
    ======================
      NULL

.. code-block:: sql

    -- 빈 문자열 패턴은 어떤 문자열과도 일치하지 않는다
    SELECT REGEXP_SUBSTR('12345abcdeabcde','');
    
::

    regexp_substr('12345abcdeabcde', '')
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
