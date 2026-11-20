
:meta-keywords: cubrid ascii, cubrid concatenation, cubrid lower, cubrid upper, cubrid chr, cubrid find_in_set, cubrid repeat, cubrid replace, cubrid substr
      
:tocdepth: 3

********************
문자열 함수와 연산자
********************

.. contents::

.. note:: 

    문자열 함수에서 **oracle_style_empty_string** 파라미터의 설정 값이 yes이면, 빈 문자열("")과 NULL을 구분하지 않고 함수에 따라 모두 NULL로 취급하거나 모두 빈 문자열로 취급한다. 이와 관련한 자세한 설명은 :ref:`oracle_style_empty_string <oracle_style_empty_string>`\ 을 참고한다.

병합 연산자
===========

병합 연산자는 피연산자로 문자열 또는 비트열 데이터 타입이 지정되며, 병합(concatenation)된 문자열 또는 비트열을 반환한다. 문자열 데이터의 병합 연산자로 덧셈 기호(**+**)와 두 개의 파이프 기호(**||**)가 제공된다. 피연산자로 **NULL** 이 지정된 경우는 **NULL** 값이 반환된다.

SQL 구문 관련 파라미터인 **pipes_as_concat** 파라미터(기본값: yes)가 no이면 이중 파이프 기호(||)가 불리언(Boolean) OR 연산자로 해석되며 **plus_as_concat** 파라미터(기본값: yes)가 no이면 덧셈 기호가 + 연산자로 해석되므로, 이러한 경우 **CONCAT** 함수를 사용하여 문자열 또는 비트열을 병합하는 것이 좋다. ::

    <concat_operand1> +  <concat_operand1>
    <concat_operand2> || <concat_operand2>
    
        <concat_operand1> ::=
            bit string |
            NULL
         
        <concat_operand2> ::=
            bit string |
            character string
            NULL

*   <*concat_operand1*>: 병합 후 왼쪽에 위치할 문자열 또는 비트열이다.
*   <*concat_operand2*>: 병합 후 오른쪽에 위치할 문자열 또는 비트열이다.

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

    **ASCII** 함수는 인자로 지정된 문자열의 가장 좌측 문자에 대한 ASCII 코드 값을 숫자로 반환한다. 입력 문자열이 **NULL** 이면 **NULL** 을 반환한다. **ASCII** 함수는 1바이트 문자에 대해 동작한다. 숫자가 입력되면 문자열로 변환한 후 가장 왼쪽 문자의 ASCII 코드 값을 반환한다.

    :param str: 입력 문자열
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

    **BIN** 함수는 **BIGINT** 타입의 숫자를 이진 문자열로 표현한다. 입력 인자가 **NULL** 이면 **NULL** 을 반환한다. **BIGNIT**\ 로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param n: **BIGINT** 타입의 숫자
    :rtype: STRING

.. code-block:: sql

    SELECT BIN(12);
    
::

    '1100'

BIT_LENGTH
==========

.. function:: BIT_LENGTH (string)

    **BIT_LENGTH** 함수는 문자열 또는 비트열의 길이(bit)를 정수값으로 반환한다. 단, 문자열의 경우 데이터 입력 환경의 문자셋(character set)에 따라 한 문자가 차지하는 바이트 수가 다르므로, **BIT_LENGTH** 함수의 리턴 값 역시 문자셋에 따라 다를 수 있다(예: UTF-8 한글: 한 글자에 3*8비트). CUBRID가 지원하는 문자셋에 관한 상세한 설명은 :ref:`char-data-type` 을 참고한다. 유효하지 않은 값을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param string: 비트 단위로 길이를 구할 문자열 또는 비트열을 지정한다. **NULL** 이 지정된 경우는 **NULL** 값이 반환된다. 
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

    문자의 개수를 정수 값으로 반환한다. CUBRID가 지원하는 문자셋에 관한 상세한 설명은 :doc:`/sql/i18n`\ 을 참고한다.
    **CHAR_LENGTH**, **CHARACTER_LENGTH**, **LENGTHB**, **LENGTH** 함수는 동일하다.

    :param string: 문자 개수 단위로 길이를 구할 문자열을 지정한다. **NULL** 이 지정된 경우는 **NULL** 값이 반환된다.
    :rtype: INT

.. note::

    *   CUBRID 9.0 미만 버전에서 멀티바이트 문자열의 경우 문자열의 바이트 수를 반환한다. 즉, 문자셋에 따라 문자 한 개당 길이가 2바이트 또는 3바이트로 계산된다.
    *   문자열 내에 포함된 공백 문자(space)의 길이는 1바이트이다.
    *   공백 문자를 표현하기 위한 빈 따옴표('')의 길이는 0이다. 단, **CHAR** (*n*) 타입에서는 공백 문자의 길이가 *n* 이고, *n* 이 생략되는 경우 1로 처리되므로 주의한다.

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

    **CHR** 함수는 인자로 지정된 연산식의 리턴 값에 대응하는 문자를 반환하는 함수이다. 유효하지 않은 범위의 코드 값을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param number_operand: 수치값을 반환하는 임의의 연산식을 지정한다. 
    :param charset_name: 문자셋 이름. 지원하는 문자셋은 utf8과 iso88591이다.
    :rtype: STRING

.. code-block:: sql

    SELECT CHR(68) || CHR(68-2);
    
::

       chr(68)|| chr(68-2)
    ======================
      'DB'

**CHR** 함수를 사용해서 멀티바이트 문자를 반환하려면 해당 문자셋에 대해 유효한 범위의 숫자를 입력한다. 

.. code-block:: sql

    SELECT CHR(14909886 USING utf8); 
    -- Below query's result is the same as above.
    SET NAMES utf8; 
    SELECT CHR(14909886); 
    
::

       chr(14909886 using utf8) 
    ====================== 
      'ま' 

문자를 16진수 문자열로 반환하려면 **HEX** 함수를 사용한다.

.. code-block:: sql

    SET NAMES utf8; 
    SELECT HEX('ま');

::

       hex(_utf8'ま')
    ======================
      'E381BE'

16진수 문자열을 10진수로 반환하려면 **CONV** 함수를 사용한다.

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

    **CONCAT** 함수는 두 개 이상의 인자가 지정되며, 모든 인자 값을 연결한 문자열을 결과로 반환한다. 지정 가능한 인자의 개수는 제한이 없으며, 문자열 타입이 아닌 인자가 지정되는 경우 자동으로 타입 변환이 수행된다. 인자 중에 **NULL** 이 포함되면 결과로 **NULL** 을 반환한다.

    인자로 지정된 문자열 사이에 구분자(separator)를 삽입하여 연결하려면, :func:`CONCAT_WS` 함수를 사용한다.

    :param strings: 연결할 문자열들
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

    **CONCAT_WS** 함수는 두 개 이상의 인자가 지정되며, 첫 번째 인자 값을 구분자로 이용하여 나머지 인자 값을 연결한 문자열을 결과로 반환한다. 지정 가능한 인자의 개수에는 제한이 없으며, 문자열 타입이 아닌 인자가 지정되는 경우 자동으로 타입 변환이 수행된다. 만약, 구분자로 **NULL** 이 지정되면 **NULL** 을 반환하고, 구분자 다음에 위치하는 나머지 인자에 **NULL** 이 지정되면 이를 무시하고 문자열을 반환한다.

    :param strings: 연결할 문자열들
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

    **ELT** 함수는 *N*\ 이 1이면 *string1*\ 을 반환하고, *N*\ 이 2이면 *string2*\ 를 반환한다. 리턴 값은 **VARCHAR** 타입이다. 조건식은 필요에 따라 늘릴 수 있다.

    문자열의 최대 바이트 길이는 33,554,432이며 이를 초과하면 **NULL**\ 을 반환한다.

    *N*\ 이 0 또는 음수이면 빈 문자열을 반환한다. *N*\ 이 입력 문자열의 개수보다 크면 범위를 벗어나므로 **NULL**\ 을 반환한다. *N*\ 이 정수로 변환할 수 없는 타입이면 에러를 반환한다.

    :param N: 문자열 리스트 중 반환할 문자열의 위치
    :param strings: 문자열 리스트
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

    **FIELD** 함수는 *string1* , *string2* 등의 인자 중 *search_string*\ 과 동일한 인자의 위치 인덱스 값(포지션)을 반환한다. *search_string*\ 과 동일한 인자가 없으면 0을 반환한다. *search_string*\ 이 **NULL**\ 이면 다른 인자와 비교 연산을 수행할 수 없으므로 0을 반환한다.

    **FIELD** 함수에서 지정된 모든 인자가 문자열 타입이면 문자열 비교 연산을 수행하고, 모두 수치 타입이면 수치 비교 연산을 수행한다. 어느 한 인자의 타입이 나머지와 다른 경우, 모든 인자를 첫 번째 인자의 타입으로 변환하여 비교 연산을 수행한다. 각 인자와의 비교 연산 도중 타입 변환에 실패하면 비교 연산의 결과를 **FALSE**\ 로 간주하고, 나머지 연산을 계속 진행한다.

    :param search_string: 검색할 문자열 패턴
    :param strings: 검색되는 문자열들의 리스트
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

    **FIND_IN_SET** 함수는 여러 개의 문자열을 쉼표(,)로 연결하여 구성한 문자열 리스트 *strlist* 에서 특정 문자열 *str* 이 존재하면 *str* 의 위치를 반환한다. *strlist* 에 *str* 이 존재하지 않거나 *strlist* 가 빈 문자열이면 0을 반환한다. 둘 중 하나의 인자가 **NULL** 이면 **NULL** 을 반환한다. *str* 이 쉼표를 포함하면 제대로 동작하지 않는다.

    :param str: 검색 대상 문자열
    :param strlist: 쉼표로 구분한 문자열의 집합
    :rtype: INT

.. code-block:: sql

    SELECT FIND_IN_SET('b','a,b,c,d');
    
::

    2

FROM_BASE64 
=========== 

.. function:: FROM_BASE64(str) 

    **FROM_BASE64** 함수는 **TO_BASE64** 함수에서 사용되는 base-64 암호화 규칙으로 암호화된 문자열을 인자로 입력받아 복호화된 결과를 바이너리 문자열로 반환한다. 입력 인자가 **NULL**\이면 **NULL**\을 반환한다. 유효하지 않은 base-64 문자열일 때 **cubrid.conf**\의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다. 
    암호화 규칙에 대한 상세 내용은 :func:`TO_BASE64`\를 참고한다. 
     
    :param str: 입력 문자열 
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

    **INSERT** 함수는 입력 문자열의 특정 위치부터 정해진 길이만큼 부분 문자열을 삽입한다. 리턴 값은 **VARCHAR** 타입이다. 문자열의 최대 길이는 33,554,432이며 이를 초과하면 **NULL** 을 반환한다.

    :param str: 입력 문자열
    :param pos: *str* 의 위치. 1부터 시작한다. *pos* 가 1보다 작거나 *string* 의 길이+1보다 크면, *string* 을 삽입하지 않고 *str* 을 리턴한다.
    :param len: *str* 의 *pos* 에 삽입할 *string* 의 길이. *len* 이 부분 문자열의 길이를 초과하면, *str* 의 *pos* 에서 *string* 만큼 삽입한다. *len* 이 음수이면 *str* 이 문자열의 끝이 된다.
    :param string: *str* 에 삽입할 부분 문자열
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

    **INSTR** 함수는 **POSITION** 함수와 유사하게 문자열 *string* 내에서 문자열 *substring* 의 위치를 반환한다. 단, **INSTR** 함수는 *substring* 의 검색을 시작할 위치를 지정할 수 있으므로 중복된 *substring* 을 검색할 수 있다.

    :param string: 입력 문자열을 지정한다.
    :param substring: 위치를 반환할 문자열을 지정한다.
    :param position: 선택 사항으로 탐색을 시작할 *string* 의 위치를 나타내며, 문자 개수 단위로 지정된다. 이 인자가 생략되면 기본값인 **1** 이 적용된다. *string* 의 첫 번째 위치는 1로 지정된다. 값이 음수이면 *string* 의 끝에서부터 지정된 값만큼 떨어진 위치에서 역방향으로 *string* 을 탐색한다.
    :rtype: INT
    
.. note::

    CUBRID 9.0 미만 버전에서는 문자 단위가 아닌 바이트 단위로 위치를 반환한다는 점을 주의한다. CUBRID 9.0 미만 버전에서 멀티바이트 문자셋이면 한 문자를 표현하는 바이트 수가 다르므로 반환되는 결과 값이 다를 수 있다.

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

    **LCASE** 함수와 **LOWER** 함수는 동일하며, 문자열에 포함된 대문자를 소문자로 변환한다.

    :param string: 소문자로 변환할 문자열을 지정한다. 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.
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

단, 콜레이션의 지정에 따라 정상 동작하지 않을 수 있으므로 주의한다. 예를 들어, 루마니아어에서 사용되는 문자 Ă을 소문자로 변환하고자 할 때 콜레이션에 따라 다음과 같이 동작한다.

콜레이션이 utf8_bin이면 이 문자는 변환되지 않는다.

.. code-block:: sql
    
    SET NAMES utf8 COLLATE utf8_bin;
    SELECT LOWER('Ă');

       lower(_utf8'Ă')
    ======================
      'Ă'

콜레이션이 utf8_ro_cs이면 'Ă'는 소문자로 변환이 가능하다.

.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ro_cs;
    SELECT LOWER('Ă');
    
       lower(_utf8'Ă' COLLATE utf8_ro_cs)
    ======================
      'ă'

CUBRID가 지원하는 콜레이션에 관한 상세한 설명은 :ref:`cubrid-all-collation`\ 을 참고한다.

LEFT
====

.. function:: LEFT ( string , length )

    **LEFT** 함수는 *string* 의 가장 왼쪽에서부터 *length* 개의 문자를 반환한다. 어느 하나의 인자가 **NULL** 인 경우 **NULL** 이 반환되고, *string* 길이보다 큰 값이나 음수가 *length* 로 지정되면 문자열 전체를 반환한다. 문자열의 가장 오른쪽에서부터 *length* 길이의 문자열을 추출하려면 :func:`RIGHT` 를 사용한다.

    :param string: 입력 문자열
    :param length: 반환할 문자열의 길이
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

    **LOCATE** 함수는 문자열 *string* 내에서 문자열 *substring* 의 위치 인덱스 값을 반환한다. 세 번째 인자 *position* 은 생략할 수 있으며, 이 인자가 지정되면 해당 위치에서부터 *substring* 을 검색하여 처음 검색한 위치 인덱스 값을 반환한다. *substring* 이 *string* 내에서 검색되지 않으면 0을 반환한다. **LOCATE** 함수는 :func:`POSITION` 와 유사하게 동작하지만, 비트열에 대해서는 **LOCATE** 함수를 적용할 수 없다.

    :param substring: 검색 대상 문자열의 패턴
    :param string: 전체 문자열
    :param position: 검색 시작 위치 
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

    **LPAD** 함수는 문자열이 일정 길이가 될 때까지 왼쪽에 특정 문자를 덧붙인다.

    :param char1: 덧붙이는 대상 문자열을 지정한다. *char1* 의 길이보다 작은 *n* 이 지정되면, 패딩을 수행하지 않고 *char1* 을 길이 *n* 으로 잘라내어 반환한다. 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.
    :param n: *char1* 의 전체 문자 개수를 지정한다. 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.
    :param char2:  *char1* 의 길이가 *n* 이 될 때까지 왼쪽에 덧붙일 문자열을 지정한다. 이를 지정하지 않으면 공백 문자(' ')가 *char2* 의 기본값으로 사용된다. 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.
    :rtype: STRING

.. note::

    CUBRID 9.0 미만 버전에서 멀티바이트 문자셋이면 한 문자를 2바이트 또는 3바이트로 처리하는데, n 값에 의해 한 문자를 표현하는 첫 번째 바이트까지 char1을 잘라내는 경우, 마지막 문자를 정상적으로 표현할 수 없으므로 마지막 바이트를 제거하고 왼쪽에 공백 문자 하나(1바이트)를 덧붙인다. 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.

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

    **LTRIM** 함수는 문자열의 왼쪽(앞 부분)에 위치한 특정 문자를 제거한다.

    :param string: 트리밍할 문자열 또는 문자열 타입의 칼럼을 입력하며, 이 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.
    :param trim_string: *string* 의 왼쪽에서 제거하고자 하는 특정 문자열을 지정할 수 있으며, 이를 지정하지 않으면 공백 문자(' ')가 자동으로 지정되어 대상 문자열의 왼쪽에 위치한 공백이 제거된다.
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

    **MID** 함수는 문자열 *string* 내의 *position* 위치로부터 *substring_length* 길이의 문자열을 추출하여 반환한다. 만약, *position* 값으로 음수가 지정되면, 문자열의 끝에서부터 역방향으로 위치를 산정한다. *substring_length* 는 생략할 수 없으며, 음수가 지정되는 경우 이를 0으로 간주하여 공백 문자열을 반환한다.

    **MID** 함수는 :func:`SUBSTR` 와 유사하게 동작하나, 비트열에 대해서는 적용할 수 없고, *substring_length* 인자를 생략할 수 없으며, *substring_length* 에 음수가 지정되면 공백 문자열을 반환한다는 차이점이 있다.

    :param string: 입력 문자열을 지정한다. 입력 값이 **NULL** 이면 결과로 **NULL** 이 반환된다.
    :param position: 문자열을 추출할 시작 위치를 지정한다. 첫 번째 문자의 위치는 1이며, 0으로 지정되더라도 1로 간주된다. 입력 값이 **NULL** 이면 결과로 **NULL** 이 반환된다.
    :param substring_length: 추출할 문자열의 길이를 지정한다. 0 또는 음수가 지정되는 경우 공백 문자열이 반환되고, 입력 값이 **NULL** 이면 결과로 **NULL** 이 반환된다.
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

    **OCTET_LENGTH** 함수는 문자열 또는 비트열의 바이트(byte) 길이를 정수로 반환한다. 따라서, 비트열의 길이가 8비트인 경우에는 1(byte)을 반환하지만, 9비트인 경우에는 2(byte)를 반환한다.

    :param string: 바이트 단위로 길이를 구할 문자열 또는 비트열을 지정한다. **NULL** 이 지정된 경우는 **NULL** 값이 반환된다.
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

    **POSITION** 함수는 문자열 *string* 내에서 문자열 *substring* 의 위치를 반환한다.

    이 함수의 인자로 문자열 또는 비트열을 반환하는 임의의 연산식을 지정할 수 있으며, 리턴 값은 0 이상의 정수이다. 문자열에 대해서는 문자 개수 단위로 위치 값을 반환하고, 비트열에 대해서는 비트 단위로 위치 값을 반환한다.

    **POSITION** 함수는 가끔 다른 함수와 연결되어서 사용된다. 예를 들어, 특정 문자열에서 일부 문자열을 추출하고 싶은 경우에 **POSITION** 함수의 결과를 **SUBSTRING** 함수의 입력으로 사용할 수 있다.

    .. note::
    
        CUBRID 9.0 미만 버전에서는 문자 단위가 아닌 바이트 단위로 위치를 반환한다는 점을 주의한다. 멀티바이트 문자셋에서는 한 문자를 표현하는 바이트 수가 다르므로 반환되는 결과 값이 다를 수 있다.

    :param substring: 위치를 반환할 문자열을 지정한다. 값이 공백 문자열이면 1이 반환된다. **NULL** 이면 **NULL** 이 반환된다.
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

    **REPEAT** 함수는 입력 문자열에 대해 반복 횟수만큼의 문자열을 반환한다. 리턴 값은 **VARCHAR** 타입이다. 문자열의 최대 길이는 33,554,432이며, 이를 초과하면 **NULL** 을 반환한다. 입력 인자 중 하나가 **NULL** 이면 **NULL** 을 반환한다.

    :param substring: 문자열
    :param count: 반복 횟수. 0 또는 음수를 입력하면 빈 문자열을 반환하고, 숫자가 아닌 다른 데이터 타입을 입력하면 에러를 반환한다.
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

    **REPLACE** 함수는 주어진 문자열 *string* 내에서 문자열 *search_string* 을 검색하여 이를 문자열 *replacement_string* 으로 대체한다. 이때, 대체할 문자열 *replacement_string* 이 생략되면 *string* 내에서 검색된 *search_string* 이 모두 제거된다. 만약, 인자에 **NULL** 이 지정되면, **NULL** 이 반환된다.

    :param string: 원본 문자열을 지정한다. 값이 **NULL** 이면 결과로 **NULL** 이 반환된다.
    :param search_string: 검색할 문자열을 지정한다. 값이 **NULL** 이면 결과로 **NULL** 이 반환된다.
    :param replacement_string: *search_string* 을 대체할 문자열을 지정한다. 값이 생략되면 *string* 에서 *search_string* 을 제거하여 반환한다. 값이 **NULL** 이면 결과로 **NULL** 이 반환된다.
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

다음은 개행 문자(newline)를 "\\n"으로 출력하도록 하는 예이다.
    
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

    **REVERSE** 함수는 문자열 *string*\ 을 역순으로 변환한 후 반환한다. 

    :param string: 입력 문자열을 지정한다. 입력 값이 공백 문자열이면 공백 문자열을 반환하고, **NULL** 이면 **NULL** 을 반환한다.
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

    **RIGHT** 함수는 *string* 의 가장 오른쪽에서부터 *length* 개의 문자를 반환한다. 어느 하나의 인자가 **NULL** 인 경우 **NULL** 이 반환되고, *string* 길이보다 큰 값이나 음수가 *length* 로 지정되면 문자열 전체를 반환한다. 문자열의 가장 왼쪽에서부터 *length* 길이의 문자열을 추출하려면 :func:`LEFT` 를 사용한다.

    :param string: 입력 문자열
    :param length: 반환할 문자열의 길이
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

    **RPAD** 함수는 문자열이 일정 길이가 될 때까지 오른쪽에 특정 문자를 덧붙인다.

    :param char1: 덧붙이는 대상 문자열을 지정한다. *char1* 의 길이보다 작은 *n* 이 지정되면, 패딩을 수행하지 않고 *char1* 을 길이 *n* 으로 잘라내어 반환한다. 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.
    :param n: *char1* 의 전체 길이를 지정한다. 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.
    :param char2: *char1* 의 길이가 *n* 이 될 때까지 오른쪽에 덧붙일 문자열을 지정한다. 이를 지정하지 않으면 공백 문자(' ')가 *char2* 의 기본값으로 사용된다. 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.
    :rtype: STRING

.. note::

    CUBRID 9.0 미만 버전에서 멀티바이트 문자셋이면 한 문자를 2바이트 또는 3바이트로 처리하는데, n 값에 의해 한 문자를 표현하는 첫 번째 바이트까지 char1을 잘라내는 경우, 마지막 문자를 정상적으로 표현할 수 없으므로 마지막 바이트를 제거하고 오른쪽에 공백 문자 하나(1바이트)를 덧붙인다. 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.

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

    **RTRIM** 함수는 문자열의 오른쪽(뒷 부분)에 위치한 특정 문자를 제거한다.

    :param string: 트리밍할 문자열 또는 문자열 타입의 칼럼을 입력하며, 이 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.
    :param trim_string: *string* 의 오른쪽에서 제거하고자 하는 특정 문자열을 지정할 수 있으며, 이를 지정하지 않으면 공백 문자(' ')가 자동으로 지정되어 대상 문자열의 오른쪽에 위치한 공백이 제거된다.
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

    **SPACE** 함수는 지정한 숫자만큼의 공백 문자열을 반환한다. 리턴 값은 **VARCHAR** 타입이다.

    :param N: 공백 개수. 시스템 파라미터 **string_max_size_bytes** 에 지정된 값보다 클 수 없으며(기본값 1048576), 이를 초과하면 **NULL** 을 반환한다. 최대값은 33,554,432이며 이를 초과하면 **NULL** 을 반환한다. 0 또는 음수를 입력하면 빈 문자열을 반환하고, 숫자로 변환할 수 없는 타입을 입력하면 에러를 반환한다.
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

    **STRCMP** 함수는 두 개의 문자열 *string1*, *string2* 을 비교하여 동일하면 0을 반환하고, *string1* 이 더 크면 1을 반환하고, *string1* 이 더 작은 경우에는 -1을 반환한다. 어느 하나의 인자가 **NULL** 이면 **NULL** 을 반환한다.

    :param string1: 비교 대상 문자열
    :param string2: 비교 대상 문자열
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

    9.0 미만 버전까지는 STRCMP가 대소문자를 구분하지 않고 문자열을 비교했으나, 
    9.0 버전부터는 대소문자를 구분하여 문자열을 비교한다. 대소문자를 구분하지 않게 동작하려면 문자열에 대소문자를 구분하지 않는 콜레이션(예: utf8_en_ci)을 지정한다.
    
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

    **SUBSTR** 함수는 문자열 *string* 내의 *position* 위치로부터 *substring_length* 길이의 문자열을 추출하여 반환한다. 만약, *position* 값으로 음수가 지정되면, 문자열의 끝에서부터 역방향으로 위치를 산정한다. 또한, *substring_length* 가 생략되는 경우, 주어진 *position* 위치로부터 마지막까지 문자열을 추출하여 반환한다.

    .. note::
    
        CUBRID 9.0 미만 버전에서는 문자 단위가 아닌 바이트 단위로 시작 위치와 문자열의 길이를 산정한다는 점에 주의한다. 따라서, 멀티바이트 문자셋에서는 한 문자를 표현하는 바이트 수를 고려하여 인자를 지정해야 한다.

    :param string: 입력 문자열을 지정한다. 입력 값이 **NULL** 이면 결과로 **NULL** 이 반환된다.
    :param position: 문자열을 추출할 시작 위치를 지정한다. 첫 번째 문자의 위치는 1이며, 0으로 지정되더라도 1로 간주된다. string 길이보다 큰 값을 지정하거나 **NULL** 을 지정하면 결과로 **NULL** 이 반환된다.
    :param substring_length: 추출할 문자열의 길이를 지정한다. 이 인자가 생략되면 *position* 위치로부터 마지막까지 문자열을 추출한다. 이 인자의 값으로 **NULL** 이 지정될 수 없으며, 0이 지정되는 경우 공백 문자열이 반환되고, 음수가 지정되는 경우 **NULL** 이 반환된다.
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

    **SUBSTRING** 함수는 **SUBSTR** 함수와 유사하며, 문자열 *string* 내의 *position* 위치로부터 *substring_length* 길이의 문자열을 추출하여 반환한다. *position* 값에 음수가 지정되면, **SUBSTRING** 함수는 문자열의 처음으로 검색 위치를 산정하고, **SUBSTR** 함수는 문자열의 끝에서부터 역방향으로 위치를 산정한다. *substring_length* 값에 음수가 지정되면, **SUBSTRING** 함수는 해당 인자가 생략된 것으로 처리하지만, **SUBSTR** 함수는 **NULL** 을 반환한다.

    :param string: 입력 문자열을 지정한다. 입력 값이 **NULL** 이면 결과로 **NULL** 이 반환된다.
    :param position: 문자열을 추출할 시작 위치를 지정한다. 0이나 음수가 지정되면, 첫 번째 문자의 위치인 1로 간주된다. *string* 길이보다 큰 값을 지정하면 공백 문자열이 반환되고, **NULL** 을 지정하면 **NULL** 이 반환된다.
    :param substring_length: 추출할 문자열의 길이를 지정한다. 이 인자가 생략되면 *position* 위치로부터 마지막까지 문자열을 추출한다. 이 인자의 값으로 **NULL** 이 지정될 수 없으며, 0이 지정되는 경우 공백 문자열이 반환되고, 음수를 지정하면 무시한다.
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

    **SUBSTRING_INDEX** 함수는 문자열에 포함된 구분자를 세어 *count* 번째 구분자 앞까지의 부분 문자열을 반환한다. 리턴 값은 **VARCHAR** 타입이다.

    :param string: 입력 문자열. 최대 길이는 33,554,432이며, 이를 초과하면 **NULL** 을 반환한다.
    :param delim: 구분자. 대소문자를 구분한다.
    :param count: 구분자가 나타나는 횟수. 양수를 입력하면 문자열의 왼쪽부터 세고, 음수를 입력하면 오른쪽부터 센다. 0이면 빈 문자열을 반환한다. 정수로 변환할 수 없는 타입을 입력하면 에러를 반환한다.
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

    문자열을 base-64 암호화 형식으로 변환하여 결과를 반환한다. 입력 인자가 문자열이 아니면 변환이 발생하기 전에 문자열로 변환된다. 입력 인자가 **NULL**\이면 **NULL**\을 반환한다. Base-64로 암호화된 문자열은 :func:`FROM_BASE64` 함수로 복호화될 수 있다. 
     
    :param str: 입력 문자열 
    :rtype: STRING 

.. code-block:: sql 

    SELECT TO_BASE64('abcd'), FROM_BASE64(TO_BASE64('abcd')); 
     
:: 

       to_base64('abcd') from_base64( to_base64('abcd')) 
    ============================================ 
      'YWJjZA==' 'abcd' 

다음은 :func:`TO_BASE64` 함수와 :func:`FROM_BASE64` 함수에서 사용되는 암호화 및 복호화 규칙이다. 

*   알파벳 값 62에 대한 암호화는 '+'이다. 
*   알파벳 값 63에 대한 암호화는 '/'이다. 
*   암호화된 결과는 4개의 출력 가능한 문자 그룹으로 구성되어 있다. 입력 데이터의 세 바이트는 네 개의 문자로 암호화된다. 마지막 그룹이 네 개의 문자로 채워지지 않으면 '=' 문자를 덧붙여(padding) 네 개 문자의 길이를 만든다. 
*   긴 출력을 여러 개의 라인으로 나누기 위해 76개의 암호화된 출력 문자마다 뉴라인(newline)이 추가된다. 
*   복호화는 뉴 라인(newline), 캐리지 리턴(carriage return), 탭, 공백 문자를 인식하고 이들을 무시한다. 

.. seealso::

    :func:`FROM_BASE64`

TRANSLATE
=========

.. function:: TRANSLATE ( string, from_substring, to_substring )

    **TRANSLATE** 함수는 지정된 문자열 *string* 내에 문자열 *from_substring* 에 지정된 문자가 존재한다면, 이를 *to_substring* 에 지정된 문자로 대체한다. 이때, *from_substring* 과 *to_substring* 에 지정되는 문자의 순서에 따라 대응 관계를 가지며, *to_substring* 과 1:1 대응되지 않는 나머지 *from_substring* 문자는 문자열 *string* 내에서 모두 제거된다. :func:`REPLACE` 함수와 유사하게 동작하나, **TRANSLATE** 함수에서는 *to_substring* 인자를 생략할 수 없다.

    :param string: 입력 문자열. 최대 길이는 33,554,432이며, 이를 초과하면 **NULL** 을 반환한다
    :param from_substring: 검색할 문자열을 지정한다. 값이 **NULL** 이면 결과로 **NULL** 이 반환된다.
    :param to_substring: *from_substring* 에 지정된 문자열을 대체할 문자열을 지정하며, 생략할 수 없다. 값이 **NULL** 이면 결과로 **NULL** 이 반환된다.
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

    **TRIM** 함수는 문자열의 앞, 뒤 또는 앞뒤에 위치한 특정 문자들을 제거한다.

    :param trim_string: 대상 문자열의 앞, 뒤 또는 앞뒤에서 제거하고자 하는 특정 문자열을 지정할 수 있으며, 이를 지정하지 않으면 공백 문자(' ')가 자동으로 지정되어 대상 문자열의 앞, 뒤 또는 앞뒤에 위치한 공백이 제거된다.
    :param string: 트리밍할 문자열 또는 문자열 타입의 칼럼을 입력하며, 이 값이 **NULL** 이면 **NULL** 이 반환된다.
    :rtype: STRING

*   **[LEADING|TRAILING|BOTH]** : 대상 문자열의 어느 위치에서 지정된 문자열을 트리밍할 것인지를 옵션으로 명시할 수 있다. **LEADING** 은 문자열의 앞 부분에서 트리밍을 수행하고, **TRAILING** 은 문자열의 뒷 부분에서 트리밍을 수행하며, **BOTH** 는 앞뒤에서 지정된 문자열을 트리밍한다. 옵션을 명시하지 않으면 기본값은 **BOTH** 이다.

*   *trim_string* 과 *string* 의 문자열은 같은 문자셋을 가져야 한다.

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

    **UCASE** 함수와 **UPPER** 함수는 동일하며, 문자열에 포함된 소문자를 대문자로 변환한다. 
    
    :param string: 대문자로 변환할 문자열을 지정한다. 값이 **NULL** 이면 결과는 **NULL** 이 반환된다.
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

단, 콜레이션의 지정에 따라 정상 동작하지 않을 수 있으므로 주의한다. 예를 들어, 루마니아어에서 사용되는 문자 ă을 대문자로 변환하고자 할 때 콜레이션에 따라 다음과 같이 동작한다.

콜레이션이 utf8_bin이면 변환이 되지 않는다.

.. code-block:: sql
    
    SET NAMES utf8 COLLATE utf8_bin;
    SELECT UPPER('ă');
    
       upper(_utf8'ă')
    ======================
      'ă'

콜레이션이 utf8_ro_cs이면 변환이 가능하다.

.. code-block:: sql

    SET NAMES utf8 COLLATE utf8_ro_cs;
    SELECT UPPER('ă');
    
       upper(_utf8'ă' COLLATE utf8_ro_cs)
    ======================
      'Ă'

CUBRID가 지원하는 콜레이션에 관한 상세한 설명은 :ref:`cubrid-all-collation`\ 을 참고한다.
