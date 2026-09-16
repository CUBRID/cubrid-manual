
:meta-keywords: cubrid md5, cubrid sha1, cubrid sha2

:tocdepth: 3

***********
암호화 함수
***********

.. contents::

.. CUBRIDSUS-13314

MD5
===

.. function:: MD5(string)

    입력 문자열에 대해 MD5 128비트 체크섬(checksum) 결과를 반환한다. 결과 값은 32개의 16진수로 표현된 문자열로 나타나며, 이 값은 예를 들면 해시 키를 생성할 때 사용할 수도 있다.

    :param string: 입력 문자열. **VARCHAR** 이 아닌 값이 입력되면 **VARCHAR** 으로 변환한다.
    :rtype: STRING
    
리턴 값은 **VARCHAR** (32) 타입이며, 입력 인자가 **NULL** 이면 **NULL** 을 리턴한다.

.. code-block:: sql

    SELECT MD5('cubrid');
    
::

       md5('cubrid')
    ======================
      '685c62385ce717a04f909047d0a55a16'
     
.. code-block:: sql

    SELECT MD5(255);
    
::

       md5(255)
    ======================
      'fe131d7f5a6b38b23cc967316c13dae2'
      
.. code-block:: sql

    SELECT MD5('01/01/2010');
     
::

       md5('01/01/2010')
    ======================
      '4a2f373c30426a1b8e9cf002ef0d4a58'
     
.. code-block:: sql

    SELECT MD5(CAST('2010-01-01' as DATE));
    
::

       md5( cast('2010-01-01' as date))
    ======================
      '4a2f373c30426a1b8e9cf002ef0d4a58'

SHA1
====

.. function:: SHA1(string)

    **SHA1** 함수는 입력 문자열에 대해 160비트의 체크섬을 계산하는데, 이는 RFC 3174(보안 해시 알고리즘)에 기술되어 있다.

    :param string: 암호화할 대상 문자열
    :rtype: STRING 

40개의 16진수 문자열을 반환하며, 입력 인자가 NULL이면 NULL을 반환한다.

.. code-block:: sql

    SELECT SHA1('cubrid');
    
::

          sha1('cubrid')
    ======================
      '0562A8E9C814E660F5FFEB0DAC739ABFBBB1CB69'

SHA2
====

.. function:: SHA2(string, hash_length) 

    **SHA2** 함수는 SHA-2 계열의 해시 함수들(SHA-224, SHA-256, SHA-384, and SHA-512)을 계산한다. 첫번째 인자는 해싱될 문자열이다. 두번째 인자는 기대하는 결과 비트의 길이를 나타내는데, 224, 256, 384, 512 또는 0(256과 동일) 중 하나여야 한다.

    :param string: 암호화할 대상 문자열
    :rtype: STRING

인자 중 하나 이상이 NULL 이거나 허용된 해시 길이가 아니면 NULL을 반환한다. 정상 범위의 인자를 입력한 경우 원하는 비트 수를 포함하는 해시 값을 반환한다.

.. code-block:: sql

    SELECT SHA2('cubrid', 256);

::
    
       sha2('cubrid', 256)
    ======================
      'D14DA17F2C492114F4A57D9F7BED908FD3A351B40CD59F0F79413687E4CA85A5'
    
.. code-block:: sql

    SELECT SHA2('cubrid', 224);

::
    
       sha2('cubrid', 224)
    ======================
      '8E5E18B5B47646C31CCEA98A87B19CBEF084036716FBD13D723AC9B2'
