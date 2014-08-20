:tocdepth: 3

*********
기타 함수
*********

.. contents::

SLEEP
=====

.. function:: SLEEP ( sec )

    명시한 시간동안 멈춰있다가 수행한다.

    :param sec: sleep 시간. 단위는 초이며, double 타입 값을 입력한다.
    :rtype: INT

    .. code-block:: sql

        SELECT SLEEP(3);

    위의 질의를 수행하면 3초 동안 멈춰있게 된다.

    .. code-block:: sql

        CREATE TABLE tbl (a float);
        INSERT INTO tbl VALUES(0.5),(1.0),(1.3);

        SELECT a, SLEEP(a) FROM tbl;

    위의 SELECT 문을 수행하면 2.8(0.5 + 1.0 + 1.3) 초 동안 멈춰있다가 결과를 출력한다.

    ::

                   a      sleep(a)    
        ==========================
        5.000000e-01            0
        1.000000e+00            0
        1.300000e+00            0

.. SYS_GUID는 9.4에도 추가됨.

SYS_GUID
========

.. function:: SYS_GUID () 

    무작위로 고유한 32개 문자의 헥사 문자열(hexadecimal)을 반환한다. 
     
     
    .. code-block:: sql 
     
        SELECT rownum, SYS_GUID() FROM code; 

    :: 
     
        rownum sys_guid() 
        =========================================== 
             1 '938210043A7B4455927A8697FB2571FF' 
             2 '7AF83E40414A49F89632D16D8EEA7299' 
             3 '7060596C53B2492384571A6F1336AE87' 
             4 'D1F9F3F51E954D319819690AB52CA3AA' 
             5 '35554A5605F84BA98EDCAC33F23D0DEE' 
             6 '80FEC6306B3D4622A1B416D6F7B59034' 

