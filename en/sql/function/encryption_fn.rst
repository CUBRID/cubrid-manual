*******************
Encryption Function
*******************

AES_ENCRYPT
===========

.. function:: AES_ENCRYPT(str,key_str)

    The **AES_ENCRYPT** and **AES_DECRYPT** functions enable encryption and decryption of data using the official AES (Advanced Encryption Standard) algorithm(based on "Rijndael" encryption). Encoding with a 128-bit key length is used, and AES_ENCRYPT() encrypts a string and returns a binary string.

    :param string: target string to be encrypted
    :param key_string: key string
    :rtype: STRING 
    

Because AES is a block-level algorithm, padding is used to encode uneven length strings and so the result string length may be calculated using AES/ECB/PKCS7 formula.

Because a return value is binary string, it can be displayed abnormally.

.. code-block:: sql

    SELECT HEX(AES_ENCRYPT('cubrid', 'encr_key'));

::
    
       hex( aes_encrypt('cubrid', 'encr_key'))
    ======================
      'BCE9FDE7AA8D2009248C595CC0C87AD0'

AES_DECRYPT
===========

.. function:: AES_DECRYPT(crypt_str, key_str)

    The **AES_DECRYPT** function decrypts the encrypted string by func:`AES_ENCRYPT`.

    :param crypt_string: encrypted string
    :param key_string: key string
    :rtype: STRING 

The input arguments may be any length. If either argument is NULL, the result of this function is also NULL. 

If AES_DECRYPT() detects invalid data or incorrect padding, it returns NULL. However, it is possible for AES_DECRYPT() to return a non-NULL value (possibly garbage) if the input data or the key is invalid. 
    
    .. code-block:: sql

        SELECT AES_DECRYPT(AES_ENCRYPT('cubrid', 'encr_key'), 'encr_key');

    ::
    
           aes_decrypt( aes_encrypt('cubrid', 'encr_key'), 'encr_key')
        ======================
          'cubrid'
          
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

.. function:: SHA1(str)

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

.. function:: SHA2(str, hash_length) 


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
