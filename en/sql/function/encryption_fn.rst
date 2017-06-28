
:meta-keywords: cubrid md5, cubrid sha1, cubrid sha2

:tocdepth: 3

*******************
Encryption Function
*******************

.. contents::

MD5
===

.. function:: MD5(string)

    The **MD5** function returns the MD5 128-bit checksum for the input character string. The result value is displayed as a character string that is expressed in 32 hexadecimals, which you can use to create hash keys, for example.

    :param string: Input string. If a value that is not a **VARCHAR** type is entered, it will be converted to **VARCHAR**.
    :rtype: STRING

The return value is a **VARCHAR** (32) type and if an input parameter is **NULL**, **NULL** will be returned.

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

    The **SHA1** function calculates an SHA-1 160-bit checksum for the string, as described in RFC 3174 (Secure Hash Algorithm).

    :param string: target string to be encrypted
    :rtype: STRING

The value is returned as a string of 40 hex digits, or NULL if the argument is NULL. 

.. code-block:: sql

    SELECT SHA1('cubrid');
    
::

          sha1('cubrid')
    ======================
      '0562A8E9C814E660F5FFEB0DAC739ABFBBB1CB69'

SHA2
====

.. function:: SHA2(string, hash_length) 

    The **SHA2** function calculates the SHA-2 family of hash functions (SHA-224, SHA-256, SHA-384, and SHA-512). The first argument is the cleartext string to be hashed. The second argument indicates the desired bit length of the result, which must have a value of 224, 256, 384, 512, or 0 (which is equivalent to 256). 

    :param string: target string to be encrypted
    :rtype: STRING
    
If either argument is NULL or the hash length is not one of the permitted values, the return value is NULL. Otherwise, the function result is a hash value containing the desired number of bits.

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
