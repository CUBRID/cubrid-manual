:tocdepth: 3

.. contents::

***************
Other functions
***************

SLEEP
=====

.. function:: SLEEP ( sec )

    This function pauses for the specified time then resumes the operations.

    :param sec: sleep time. The unit is second and inputs double type value.
    :rtype: INT

    .. code-block:: sql

        SELECT SLEEP(3);

    It pauses for 3 seconds.

    .. code-block:: sql

        CREATE TABLE tbl (a float);
        INSERT INTO tbl VALUES(0.5),(1.0),(1.3);

        SELECT a, SLEEP(a) FROM tbl;

    When you run the above query, it pauses for 2.8 seconds (0.5 + 1.0 + 1.3) then outputs the results.

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

    It returns the unique hexadecimal string of 32 characters randomly.
     
     
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
