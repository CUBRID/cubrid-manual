***********
암호화 함수
***********

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
