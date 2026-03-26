
:meta-keywords: cubrid lob, cubrid blob, cubrid clob, bit_to_blob, char_to_clob, clob_to_char, blob_from_file

:tocdepth: 3

********
LOB 함수
********

.. contents::

BIT_TO_BLOB
===========

.. function:: BIT_TO_BLOB ( blob_type_column_or_value )

    **BIT**, **VARYING BIT** 타입을 **BLOB** 타입으로 변환한다.

    :param blob_type_column_or_value: 변환 대상 칼럼 또는 값
    :rtype: BLOB

BLOB_FROM_FILE
==============

.. function:: BLOB_FROM_FILE ( file_pathname )

    **VARCHAR** 타입의 파일 경로에서 파일 내용을 읽어 **BLOB** 타입 데이터로 반환한다. 

    :param file_pathname: CAS나 CSQL과 같은 DB 클라이언트가 구동하는 서버 상의 경로
    :rtype: BLOB

.. code-block:: sql

    SELECT CAST(BLOB_FROM_FILE('local:/home/cubrid/demo/lob/ces_622/image_t.00001352246696287352_4131') 
       AS BIT VARYING) result; 

    SELECT CAST(BLOB_FROM_FILE('file:/home/cubrid/demo/lob/ces_622/image_t.00001352246696287352_4131') 
       AS BIT VARYING) result; 

BLOB_LENGTH
===========

.. function:: BLOB_LENGTH ( blob_column )                                            
 
    **BLOB** 파일에 저장된 **LOB** 데이터의 길이를 바이트 단위로 반환한다. 
    
    :param clob_column: 길이를 구하고자 하는 BLOB 타입의 칼럼
    :rtype: INT

BLOB_TO_BIT
===========

.. function:: BLOB_TO_BIT ( blob_type_column )

    **BLOB** 타입을 **VARYING BIT** 타입으로 변환한다.           

    :param blob_type_column: 변환 대상 칼럼
    :rtype: VARYING BIT
    
CHAR_TO_BLOB
============

.. function:: CHAR_TO_BLOB ( char_type_column_or_value )

    **CHAR**, **VARCHAR** 타입을 **BLOB** 타입으로 변환한다.           

    :param char_type_column_or_value: 변환 대상 칼럼 또는 값
    :rtype: BLOB

CHAR_TO_CLOB
============

.. function:: CHAR_TO_CLOB ( char_type_column_or_value )

    **CHAR**, **VARCHAR** 타입을 **CLOB** 타입으로 변환한다.           

    :param char_type_column_or_value: 변환 대상 칼럼 또는 값
    :rtype: CLOB

CLOB_FROM_FILE
==============

.. function:: CLOB_FROM_FILE ( file_pathname )

    **VARCHAR** 타입의 파일 경로에서 파일 내용을 읽어 **CLOB** 타입 데이터로 반환한다.

    :param file_pathname: CAS나 CSQL과 같은 DB 클라이언트가 구동하는 서버 상의 경로
    :rtype: CLOB

file_pathname을 상대 경로로 명시한 경우, 상위 경로는 프로세스의 현재 작업 디렉터리가 된다. 

이 함수가 호출된 구문에 대해서는 실행 계획을 캐싱하지 않는다.

.. code-block:: sql

    SELECT CAST(CLOB_FROM_FILE('local:/home/cubrid/demo/lob/ces_622/image_t.00001352246696287352_4131') 
       AS VARCHAR) result; 
    
    SELECT CAST(CLOB_FROM_FILE('file:/home/cubrid/demo/lob/ces_622/image_t.00001352246696287352_4131') 
       AS VARCHAR) result; 

CLOB_LENGTH
===========

.. function:: CLOB_LENGTH ( clob_column )
 
    **CLOB** 파일에 저장된 **LOB** 데이터의 길이를 바이트 단위로 반환한다. 
    
    :param clob_column: 길이를 구하고자 하는 **CLOB** 타입의 칼럼
    :rtype: INT

CLOB_TO_CHAR
============

.. function:: CLOB_TO_CHAR ( clob_type_column [USING charset] )

    **CLOB** 타입을 **VARCHAR** 타입으로 변환한다.

    :param clob_type_column: 변환 대상 칼럼
    :param charset: 변환할 문자열의 문자셋. utf8, euckr, iso88591이 올 수 있다.
    :rtype: STRING
