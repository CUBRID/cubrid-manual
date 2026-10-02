:meta-keywords: cubrid regex, cubrid regexp, cubrid rlike, regular expression functions, regex
:meta-description: CUBRID functions related to regular expressions.

:tocdepth: 3

*********************************************
Regular Expressions Functions and Operators
*********************************************

A regular expression is a powerful way to specify a pattern for a complex search.
The functions and operators described in this section performs regular expression matching on a string.

.. contents::

.. _regex-syntax:

Regular Expressions Pattern Syntax
==============================================

To implement regular expressions, CUBRID uses the Google RE2 library and the C++ <regex> standard library.
You can select the regular expression library to use by setting the **regexp_engine** system parameter, and the Google RE2 library is used as a default.

The following sub-sections describes supported regular expression grammars with several examples.

.. note::

  **Compatibility Considerations**

  - In the prior version of CUBRID 11, CUBRID used Henry Spencer’s implementation of regular expressions.
  - From the CUBRID 11.0, CUBRID removes the Henry Spencer library and uses C++ <regex> standard library to support regular expression functions and operators.
  - From the CUBRID 11.2, CUBRID has added the Google RE2 library. Either C++ <regex> or Google RE2 can be used by setting the system parameter.

  \1. The Henry Spencer’s implementation of regular expressions operates in byte-wise fashion. So the REGEXP and RLIKE were not multibyte safe, 
  they only worked as ASCII encoding without considering the collation of operands.
  
  \2. The Spencer library supports the POSIX *collating sequence* expressions (*[.character.]*). But it does not support anymore.
  Also, *character equivalents* (*[=word=]*) does not support. CUBRID occurs an error when these collating element syntax is given.
  
  \3. The Spencer library matches line-terminator characters for the dot operator (.). But it does not.
  
  \4. The word-beginning and word-end boundary ([[:<:]] and [[:>:]]) doesn't support anymore. Instead, the word boundary notation (\\b) can be used.

.. warning::

  **C++ <regex> library caveats**

  There is an issue with the C++ <regex> library that can cause excessive recursive calls when the input string is long or the regular expression pattern is complex.
  Therefore, it is recommended to use Google RE2 instead of C++ <regex>. 
  C++ <regex> is reserved for backwards compatibility and has been deprecated.

.. note::

  **Multibyte Character Comparision Considerations**

  C++ <regex> performs multibyte comparision by C++ <locale> standard dependent on system-supplied locales. Therefore, system locale should be installed on your system for locale-sensitive functions.

Special Pattern Characters
---------------------------

Special pattern characters are characters (or sequences of characters) that have a special meaning when they appear in a regular expression pattern, either to represent a character that is difficult to express in a string, or to represent a category of characters. 
Each of these special pattern characters is matched in a string against a single character (unless a quantifier specifies otherwise).

+----------------+------------------------------------------------------------------------------------------------------------------------------+
| Characters     | Description                                                                                                                  |
+================+==============================================================================================================================+
| .              | Any character except line terminators (LF, CR, LS, PS).                                                                      |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\t            | A horizontal tab character (same as \\u0009).                                                                                |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\n            | A newline (line feed) character (same as \\u000A).                                                                           |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\v            | A vertical tab character (same as \\u000B).                                                                                  |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\f            | A form feed character (same as \\u000C).                                                                                     |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\r            | A carriage return character (same as \\u000D)                                                                                |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\c\ *letter*  | A control code character whose code unit value is the same as the remainder of dividing                                      |
|                | the code unit value of *letter* by 32.                                                                                       |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\x\ *hh*      | A a character whose code unit value has an hex value equivalent to the two hex digits *hh*.                                  |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\u\ *hhhh*    | A character whose code unit value has an hex value equivalent to the four hex digits *hhhh*. *Only supported in C++ <regex>* |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\x\ {*digits*}| A hex character code corresponds to the value of *digits* *Only supported in Google RE2*                                     |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\0            | A null character (same as \\u0000). *Only supported in C++ <regex>*                                                          |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\\ *num*      | The result of the submatch whose opening parenthesis is the *num*-th. See groups below for more info.                        |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\d            | A decimal digit character (same as [[:digit:]]).                                                                             |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\D            | Any character that is not a decimal digit character (same as [^[:digit:]]).                                                  |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\s            | A whitespace character (same as [[:space:]]).                                                                                |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\S            | Any character that is not a whitespace character (same as [^[:space:]]).                                                     |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\w            | An alphanumeric or underscore character (same as [_[:alnum:]]).                                                              |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\W            | Any character that is not an alphanumeric or underscore character (same as [^_[:alnum:]]).                                   |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
|                | | The *character* character as it is, without interpreting its special meaning within a regex expression.                    |
| \\\ *character*| | Any character can be escaped except those which form any of the special character sequences above.                         |
|                | | Needed for: ^ $ \\ . * + ? ( ) [ ] { } \|                                                                                  |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \[\ *class*\]  | A string is part of the *class*. see :ref:`regex-posix-character-class`.                                                     |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \[^\ *class*\] | A string is not part of the *class*. see :ref:`regex-posix-character-class`.                                                 |
+----------------+------------------------------------------------------------------------------------------------------------------------------+
| \\p{*class*}   | A string is range of Unicode range of *class*. see :ref:`regex-unicode-character-class`.                                     |
+----------------+------------------------------------------------------------------------------------------------------------------------------+

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
      0

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

.. warning::

    a backreference syntax using $int is supported only in C++ <regex>.

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
These submatches can be used in the regular expression itself to specify that the entire subpattern should appear again somewhere else (see \int in the special characters list). They can also be used in the replacement string in the REGEXP_REPLACE function.

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

.. note::
    
    The following syntax is supported only in C++ <regex>.

+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| Characters      | Description                                                                                                           |
+=================+=======================================================================================================================+
| \\b             | The previous character is a word character and the next is a non-word character (or vice-versa).                      |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| \\B             | The previous and next characters are both word characters or both are non-word characters.                            |
+-----------------+-----------------------------------------------------------------------------------------------------------------------+
| (?=subpattern)  | Positive lookahead. The characters following the character must match subpattern, but no characters are consumed.     |
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

Individual characters
~~~~~~~~~~~~~~~~~~~~~

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

Ranges
~~~~~~~~~~~~~~~~~~~~~~~ 

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

.. _regex-posix-character-class:

POSIX-based character classes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The POSIX-based character class (*[:classname:]*) defines categories of characters as shown below.

.. note::

    Google RE2 matches only ASCII characters, and C++ <regex> matches Unicode characters as well.

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

[:d:], [:w:] and [:s:] are an extension to the ECMAScript grammar. Only available in C++ <regex>.

+------------+-----------------------------------------+
| Class      | Description                             |
+============+=========================================+
| [:d:]      | Decimal digit character (0-9)           |
+------------+-----------------------------------------+
| [:w:]      | Word character                          |
+------------+-----------------------------------------+
| [:s:]      | Whitespace character                    |
+------------+-----------------------------------------+

.. code-block:: sql

    SELECT REGEXP_SUBSTR ('Samseong-ro 86-gil, Gangnam-gu, Seoul 06178', '[[:digit:]]{5}');
    
::

    regexp_substr('Samseong-ro 86-gil, Gangnam-gu, Seoul 06178', '[[:digit:]]{5}')
    ======================
      '06178'

.. code-block:: sql

    -- ;set regexp_engine=cppstd
    SET NAMES utf8 COLLATE utf8_ko_cs;
    SELECT REGEXP_REPLACE ('가나다 가나 가나다라', '\b([[:alpha:]]{2})\b', '#');
    
::

    regexp_replace('가나다 가나 가나다라' collate utf8_ko_cs, '\b([[:alpha:]]{2})\b' collate utf8_ko_cs, '#' collate utf8_ko_cs)
    ======================
    '가나다 # 가나다라'

.. _regex-unicode-character-class:

Unicode Character Class
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Unicode character classes (*\\p{classname}*) are only supported by Google RE2. The Unicode class name can be specified as shown in the table below.
For example, to match Korean characters, you can use **\\p{Hangul}**.

+-------------------------+
| Unicode Character Class |
+=========================+
| Adlam                   |
+-------------------------+
| Ahom                    |
+-------------------------+
| Anatolian_Hieroglyphs   |
+-------------------------+
| Arabic                  |
+-------------------------+
| Armenian                |
+-------------------------+
| Avestan                 |
+-------------------------+
| Balinese                |
+-------------------------+
| Bamum                   |
+-------------------------+
| Bassa_Vah               |
+-------------------------+
| Batak                   |
+-------------------------+
| Bengali                 |
+-------------------------+
| Bhaiksuki               |
+-------------------------+
| Bopomofo                |
+-------------------------+
| Brahmi                  |
+-------------------------+
| Braille                 |
+-------------------------+
| Buginese                |
+-------------------------+
| Buhid                   |
+-------------------------+
| Canadian_Aboriginal     |
+-------------------------+
| Carian                  |
+-------------------------+
| Caucasian_Albanian      |
+-------------------------+
| Chakma                  |
+-------------------------+
| Cham                    |
+-------------------------+
| Cherokee                |
+-------------------------+
| Chorasmian              |
+-------------------------+
| Common                  |
+-------------------------+
| Coptic                  |
+-------------------------+
| Cuneiform               |
+-------------------------+
| Cypriot                 |
+-------------------------+
| Cypro_Minoan            |
+-------------------------+
| Cyrillic                |
+-------------------------+
| Deseret                 |
+-------------------------+
| Devanagari              |
+-------------------------+
| Dives_Akuru             |
+-------------------------+
| Dogra                   |
+-------------------------+
| Duployan                |
+-------------------------+
| Egyptian_Hieroglyphs    |
+-------------------------+
| Elbasan                 |
+-------------------------+
| Elymaic                 |
+-------------------------+
| Ethiopic                |
+-------------------------+
| Georgian                |
+-------------------------+
| Glagolitic              |
+-------------------------+
| Gothic                  |
+-------------------------+
| Grantha                 |
+-------------------------+
| Greek                   |
+-------------------------+
| Gujarati                |
+-------------------------+
| Gunjala_Gondi           |
+-------------------------+
| Gurmukhi                |
+-------------------------+
| Han                     |
+-------------------------+
| Hangul                  |
+-------------------------+
| Hanifi_Rohingya         |
+-------------------------+
| Hanunoo                 |
+-------------------------+
| Hatran                  |
+-------------------------+
| Hebrew                  |
+-------------------------+
| Hiragana                |
+-------------------------+
| Imperial_Aramaic        |
+-------------------------+
| Inherited               |
+-------------------------+
| Inscriptional_Pahlavi   |
+-------------------------+
| Inscriptional_Parthian  |
+-------------------------+
| Javanese                |
+-------------------------+
| Kaithi                  |
+-------------------------+
| Kannada                 |
+-------------------------+
| Katakana                |
+-------------------------+
| Kawi                    |
+-------------------------+
| Kayah_Li                |
+-------------------------+
| Kharoshthi              |
+-------------------------+
| Khitan_Small_Script     |
+-------------------------+
| Khmer                   |
+-------------------------+
| Khojki                  |
+-------------------------+
| Khudawadi               |
+-------------------------+
| Lao                     |
+-------------------------+
| Latin                   |
+-------------------------+
| Lepcha                  |
+-------------------------+
| Limbu                   |
+-------------------------+
| Linear_A                |
+-------------------------+
| Linear_B                |
+-------------------------+
| Lisu                    |
+-------------------------+
| Lycian                  |
+-------------------------+
| Lydian                  |
+-------------------------+
| Mahajani                |
+-------------------------+
| Makasar                 |
+-------------------------+
| Malayalam               |
+-------------------------+
| Mandaic                 |
+-------------------------+
| Manichaean              |
+-------------------------+
| Marchen                 |
+-------------------------+
| Masaram_Gondi           |
+-------------------------+
| Medefaidrin             |
+-------------------------+
| Meetei_Mayek            |
+-------------------------+
| Mende_Kikakui           |
+-------------------------+
| Meroitic_Cursive        |
+-------------------------+
| Meroitic_Hieroglyphs    |
+-------------------------+
| Miao                    |
+-------------------------+
| Modi                    |
+-------------------------+
| Mongolian               |
+-------------------------+
| Mro                     |
+-------------------------+
| Multani                 |
+-------------------------+
| Myanmar                 |
+-------------------------+
| Nabataean               |
+-------------------------+
| Nag_Mundari             |
+-------------------------+
| Nandinagari             |
+-------------------------+
| New_Tai_Lue             |
+-------------------------+
| Newa                    |
+-------------------------+
| Nko                     |
+-------------------------+
| Nushu                   |
+-------------------------+
| Nyiakeng_Puachue_Hmong  |
+-------------------------+
| Ogham                   |
+-------------------------+
| Ol_Chiki                |
+-------------------------+
| Old_Hungarian           |
+-------------------------+
| Old_Italic              |
+-------------------------+
| Old_North_Arabian       |
+-------------------------+
| Old_Permic              |
+-------------------------+
| Old_Persian             |
+-------------------------+
| Old_Sogdian             |
+-------------------------+
| Old_South_Arabian       |
+-------------------------+
| Old_Turkic              |
+-------------------------+
| Old_Uyghur              |
+-------------------------+
| Oriya                   |
+-------------------------+
| Osage                   |
+-------------------------+
| Osmanya                 |
+-------------------------+
| Pahawh_Hmong            |
+-------------------------+
| Palmyrene               |
+-------------------------+
| Pau_Cin_Hau             |
+-------------------------+
| Phags_Pa                |
+-------------------------+
| Phoenician              |
+-------------------------+
| Psalter_Pahlavi         |
+-------------------------+
| Rejang                  |
+-------------------------+
| Runic                   |
+-------------------------+
| Samaritan               |
+-------------------------+
| Saurashtra              |
+-------------------------+
| Sharada                 |
+-------------------------+
| Shavian                 |
+-------------------------+
| Siddham                 |
+-------------------------+
| SignWriting             |
+-------------------------+
| Sinhala                 |
+-------------------------+
| Sogdian                 |
+-------------------------+
| Sora_Sompeng            |
+-------------------------+
| Soyombo                 |
+-------------------------+
| Sundanese               |
+-------------------------+
| Syloti_Nagri            |
+-------------------------+
| Syriac                  |
+-------------------------+
| Tagalog                 |
+-------------------------+
| Tagbanwa                |
+-------------------------+
| Tai_Le                  |
+-------------------------+
| Tai_Tham                |
+-------------------------+
| Tai_Viet                |
+-------------------------+
| Takri                   |
+-------------------------+
| Tamil                   |
+-------------------------+
| Tangsa                  |
+-------------------------+
| Tangut                  |
+-------------------------+
| Telugu                  |
+-------------------------+
| Thaana                  |
+-------------------------+
| Thai                    |
+-------------------------+
| Tibetan                 |
+-------------------------+
| Tifinagh                |
+-------------------------+
| Tirhuta                 |
+-------------------------+
| Toto                    |
+-------------------------+
| Ugaritic                |
+-------------------------+
| Vai                     |
+-------------------------+
| Vithkuqi                |
+-------------------------+
| Wancho                  |
+-------------------------+
| Warang_Citi             |
+-------------------------+
| Yezidi                  |
+-------------------------+
| Yi                      |
+-------------------------+
| Zanabazar_Square        |
+-------------------------+

.. code-block:: sql

    -- ;set regexp_engine=re2
    SELECT REGEXP_COUNT('가나 가나다라 마바사아 자차카타 파하', '\p{Hangul}+');

::

    regexp_count(_utf8'가나 가나다라 마바사아 자차카타 파하' collate utf8_ko_cs, _utf8'\p{Hangul}+' collate utf8_ko_cs)
    ==============================
    5

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

    -- [a-dX] : matches any character that is either a, b, c, d or X.
    SELECT ('aXbc' REGEXP '[a-dX]');

::
    
    ('aXbc' regexp '[a-dX]')
    ==============================
    1

.. code-block:: sql

    -- When REGEXP is used in SELECT list, enclosing this with parentheses is required. 
    -- But used in WHERE clause, no need parentheses.
    -- case insensitive, except when used with BINARY.
    SELECT name FROM public.athlete where name REGEXP '^[a-d]';

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

    The **REGEXP_COUNT** function returns the number of occurrences of the regular expression pattern, *pattern_string*, within a given character string, *string*. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param pattern_string: Specifies the regular expression pattern string to be searched. If the value is **NULL**, **NULL** is returned.
    :param position: Specifies the position of the *string* to start the search. If the value is omitted, the default value 1 is applied. If the value is negative or zero, an error will be returned. If the value is **NULL**, **NULL** is returned
    :param match_type: Specifies the string to change default matching behavior of the function. If the value is omitted, the default value 'i' is applied. If the value is other than 'c' or 'i', an error will be returned. If the value is **NULL**, **NULL** is returned.
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

    SELECT REGEXP_COUNT('ab123Ab111aAA','[a-d]', 3);
    
::

    regexp_count('ab123Ab111aAA', '[a-d]', 3)
    ===========================================
                                            5

.. code-block:: sql

    -- case insensitive ('i') is the default value
    SELECT REGEXP_COUNT('ab123Ab111aAA','[a-d]', 3, 'i');

    -- If case sensitive ('c') is specified as match_type, A is not matched.
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

    regexp_count('가나123abc가다abc가가' collate utf8_ko_cs, '[가-나]+' collate utf8_ko_cs)
    =================================================================================================
                                                                                                    3


.. _regex-instr:

REGEXP_INSTR
============

.. function:: REGEXP_INSTR (string, pattern_string [, position [, occurrence [, return_option [, match_type]]]])

    The **REGEXP_INSTR** function returns the beginning or ending position by searching for a regular expression pattern, *pattern_string*, within a given character string, *string*. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param pattern_string: Specifies the regular expression pattern string to be searched. If the value is **NULL**, **NULL** is returned.
    :param position: Specifies the position of the *string* to start the search. If the value is omitted, the default value 1 is applied. If the value is negative or zero, an error will be returned. If the value is **NULL**, **NULL** is returned
    :param occurrence: Specifies the occurrence of the match to use. If the value is omitted, the default value 1 is applied. If the value is negative, an error will be returned. If the value is **NULL**, **NULL** is returned.
    :param return_option: Specifies whether to return the position of the start or end of the matched string. If the value is 0, the position of the first character of the match is returned. If the value is 0, the starting position of the matched string is returned. If the value is 1, the end position of the matched string is returned. If the value is other than 0 or 1, an error will be returned. If the value is **NULL**, **NULL** is returned.
    :param match_type: Specifies the string to change default matching behavior of the function. If the value is omitted, the default value 'i' is applied. If the value is other than 'c' or 'i', an error will be returned. If the value is **NULL**, **NULL** is returned.
    :rtype: INT

.. code-block:: sql

    -- it returns NULL when an argument is specified with NULL value
    SELECT REGEXP_INSTR('12345abcdeabcde','[abc]',NULL);
    
::

    regexp_instr('12345abcdeabcde', '[abc]', null)
    ======================
      NULL

.. code-block:: sql

    -- an empty string pattern doesn't match with any string
    SELECT REGEXP_INSTR('12345abcdeabcde','');
    
::

    regexp_instr('12345abcdeabcde', '')
    ======================
      0

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
      7

.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ko_cs;
    SELECT REGEXP_INSTR('12345가나다라마가나다라마바','[가-다]+');
    
::

    regexp_instr('12345가나다라마가나다라마바' collate utf8_ko_cs, '[가-다]+' collate utf8_ko_cs)
    ============================================================================================================
                                                                                                            6

.. _regex-like:

REGEXP_LIKE
===========

.. function:: REGEXP_LIKE (string, pattern_string [, match_type])

    The **REGEXP_LIKE** function searches for a regular expression pattern, *pattern_string*, within a given character string, *string*. If the pattern matched anywhere in the *string*, 1 is returned. Otherwise, 0 is returned. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param pattern_string: Specifies the regular expression pattern string to be searched. If the value is **NULL**, **NULL** is returned.
    :param match_type: Specifies the string to change default matching behavior of the function. If the value is omitted, the default value 'i' is applied. If the value is other than 'c' or 'i', an error will be returned. If the value is **NULL**, **NULL** is returned.
    :rtype: INT

.. code-block:: sql

    SELECT REGEXP_LIKE('abbbbc','ab+c');
    
::

    regexp_like('abbbbc', 'ab+c');
    ======================
      1

.. code-block:: sql

    -- an empty string pattern doesn't match with any string
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

    regexp_like('가나다' collate utf8_ko_cs, '가나?다' collate utf8_ko_cs)
    ==============================================================================
                                                                                1

    regexp_like('가나라다' collate utf8_ko_cs, '가나?다' collate utf8_ko_cs)
    =================================================================================
                                                                                    0

.. _regex-replace:

REGEXP_REPLACE
==============

.. function:: REGEXP_REPLACE (string, pattern_string, replacement_string [, position [, occurrence [, match_type]]])

    The **REGEXP_REPLACE** function searches for a regular expression pattern, *pattern_string*, within a given character string, *string*, and replaces it with a character string, *replacement_string*. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param pattern_string: Specifies the regular expression pattern string to be searched. If the value is **NULL**, **NULL** is returned.
    :param replacement_string: Specifies the string to replace the matched string by *pattern_string*. If the value is **NULL**, **NULL** is returned.
    :param position: Specifies the position of the *string* to start the search. If the value is omitted, the default value 1 is applied. If the value is negative or zero, an error will be returned. If the value is **NULL**, **NULL** is returned
    :param occurrence: Specifies the occurrence of the pattern to use. If the value is omitted, the default value 0 is applied. If the value is negative, an error will be returned. If the value is **NULL**, **NULL** is returned.
    :param match_type: Specifies the string to change default matching behavior of the function. If the value is omitted, the default value 'i' is applied. If the value is other than 'c' or 'i', an error will be returned. If the value is **NULL**, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    -- it returns NULL when an argument is specified with NULL value
    SELECT REGEXP_REPLACE('12345abcdeabcde','[a-d]',NULL);
    
::

    regexp_replace('12345abcdeabcde', '[a-d]', null)
    ======================
    NULL

.. code-block:: sql

    -- an empty string pattern doesn't match with any string
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

    -- case insensitive ('i') is the default value
    SELECT REGEXP_REPLACE('12345abDEKBcde','[a-d]','#', 1, 0, 'i');

    -- match_type is specified as case sensitive ('c'). 'B' and 'D' are not matched.
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

    regexp_replace('a1가b2나다라' collate utf8_ko_cs, '[가-다]' collate utf8_ko_cs, '#' collate utf8_ko_cs, 6)
    ======================
    'a1가b2##라'

.. _regex-substr:

REGEXP_SUBSTR
=============

.. function:: REGEXP_SUBSTR (string, pattern_string [, position [, occurrence [, match_type]]])

    The **REGEXP_SUBSTR** function extracts a character string matched for a regular expression pattern, *pattern_string*, within a given character string, *string*. If **NULL** is specified as an argument, **NULL** is returned.

    :param string: Specifies the original string. If the value is **NULL**, **NULL** is returned.
    :param pattern_string: Specifies the regular expression pattern string to be searched. If the value is **NULL**, **NULL** is returned.
    :param position: Specifies the position of the *string* to start the search. If the value is omitted, the default value 1 is applied. If the value is negative or zero, an error will be returned. If the value is **NULL**, **NULL** is returned
    :param occurrence: Specifies the occurrence of the pattern to use. If the value is omitted, the default value 0 is applied. If the value is negative, an error will be returned. If the value is **NULL**, **NULL** is returned.
    :param match_type: Specifies the string to change default matching behavior of the function. If the value is omitted, the default value 'i' is applied. If the value is other than 'c' or 'i', an error will be returned. If the value is **NULL**, **NULL** is returned.
    :rtype: STRING

.. code-block:: sql

    -- if pattern is not matched, null is returned
    SELECT REGEXP_SUBSTR('12345abcdeabcde','[k-z]+');
    
::

    regexp_substr('12345abcdeabcde','[k-z]+');
    ======================
      NULL

.. code-block:: sql

    -- an empty string pattern doesn't match with any string
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
    SELECT REGEXP_SUBSTR('삼성로, 강남구, 서울특별시','\p{Hangul}+',1,2);
    
::

    regexp_substr('삼성로, 강남구, 서울특별시' collate utf8_ko_cs, '\p{Hangul}+' collate utf8_ko_cs, 1, 2)
    ======================
    '강남구'
