
:meta-keywords: cubrid lob, cubrid blob, cubrid clob, bit_to_blob, char_to_clob, clob_to_char, blob_from_file

:tocdepth: 3

*************
LOB Functions
*************

.. contents::

BIT_TO_BLOB
===========

.. function:: BIT_TO_BLOB ( blob_type_column_or_value )

    This function converts **BIT** or **VARYING BIT** type into **BLOB** type.

    :param blob_type_column_or_value: Target column or value to convert
    :rtype: BLOB

BLOB_FROM_FILE
==============

.. function:: BLOB_FROM_FILE ( file_pathname )

    This function read the contents from the file with **VARCHAR** type data and returns **BLOB** type data.

    :param file_pathname: the path on the server which DB clients like CAS or CSQL are started
    :rtype: BLOB

.. code-block:: sql

    SELECT CAST(BLOB_FROM_FILE('local:/home/cubrid/demo/lob/ces_622/image_t.00001352246696287352_4131') 
       AS BIT VARYING) result; 

    SELECT CAST(BLOB_FROM_FILE('file:/home/cubrid/demo/lob/ces_622/image_t.00001352246696287352_4131') 
       AS BIT VARYING) result; 

BLOB_LENGTH
===========

.. function:: BLOB_LENGTH ( blob_column )                                            
 
    The length of **LOB** data stored in **BLOB** file is returned.
    
    :param clob_column: The column to get the length of **BLOB**
    :rtype: INT

BLOB_TO_BIT
===========

.. function:: BLOB_TO_BIT ( blob_type_column )

    This function converts **BLOB** type to **VARYING BIT** type.           

    :param blob_type_column: Target column to convert
    :rtype: VARYING BIT
    
CHAR_TO_BLOB
============

.. function:: CHAR_TO_BLOB ( char_type_column_or_value )

    This function converts **CHAR** or **VARCHAR** type into **BLOB**  type.           

    :param char_type_column_or_value: Target column or value to convert
    :rtype: BLOB

CHAR_TO_CLOB
============

.. function:: CHAR_TO_CLOB ( char_type_column_or_value )

    This function converts **CHAR** or **VARCHAR** type into **CLOB** type.

    :param char_type_column_or_value: Target column or value to convert
    :rtype: CLOB

CLOB_FROM_FILE
==============

.. function:: CLOB_FROM_FILE ( file_pathname )

    This function read the contents from the file with **VARCHAR** type data and returns **CLOB** type data.

    :param file_pathname: the path on the server which DB clients like CAS or CSQL are started
    :rtype: CLOB

If you specify the *file_pathname* as the relative path, the parent path will be the current working directory. 

For the statement including this function, the query plan is not cached.

.. code-block:: sql

    SELECT CAST(CLOB_FROM_FILE('local:/home/cubrid/demo/lob/ces_622/image_t.00001352246696287352_4131') 
       AS VARCHAR) result; 
    
    SELECT CAST(CLOB_FROM_FILE('file:/home/cubrid/demo/lob/ces_622/image_t.00001352246696287352_4131') 
       AS VARCHAR) result; 

CLOB_LENGTH
===========

.. function:: CLOB_LENGTH ( clob_column )
 
    The length of **LOB** data stored in **CLOB** file is returned.
    
    :param clob_column: The column to get the length of **CLOB**
    :rtype: INT

CLOB_TO_CHAR
============

.. function:: CLOB_TO_CHAR ( clob_type_column [USING charset] )

    This function converts **CLOB** type into **VARCHAR** type.

    :param clob_type_column: Target column to convert
    :param charset: The character set of string to convert. It can be utf8, euckr or iso88591.
    :rtype: STRING
