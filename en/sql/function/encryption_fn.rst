***********
암호화 함수
***********




**Encryption Function**

**MD5 Function**

**Description**

The
**MD5**
function function returns the MD5 128-bit checksum for the input character string. The result value is displayed as a character string that is expressed in 32 hexadecimals, which you can use to create hash keys, for example.

The return value is a
**VARCHAR**
(32) type and if an input parameter is
**NULL**
,
**NULL**
will be returned.

**Syntax**

**MD5**
(
*string*
)

*   *string*
    : Input string. If a value that is not a
    **VARCHAR**
    type is entered, it will be converted to
    **VARCHAR**
    .



**Example**

SELECT MD5('cubrid');

   md5('cubrid')

======================

  '685c62385ce717a04f909047d0a55a16'

 

SELECT MD5(255);

   md5(255)

======================

  'fe131d7f5a6b38b23cc967316c13dae2'

SELECT MD5('01/01/2010');

 

   md5('01/01/2010')

======================

  '4a2f373c30426a1b8e9cf002ef0d4a58'

 

SELECT MD5(CAST('2010-01-01' as DATE));

   md5( cast('2010-01-01' as date))

======================

  '4a2f373c30426a1b8e9cf002ef0d4a58'
