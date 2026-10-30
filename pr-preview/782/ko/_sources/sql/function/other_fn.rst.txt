
:meta-keywords: cubrid sleep, cubrid sys_guid

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

    3초간 멈춰있다가 수행한다.
    
    
SYS_GUID
========

.. function:: SYS_GUID () 

    무작위로 고유한 32개 문자의 헥사 문자열(hexadecimal)을 반환한다. 
     
     
    .. code-block:: sql 
     
        SELECT SYS_GUID();

    :: 
     
        sys_guid() 
        ================================== 
        '938210043A7B4455927A8697FB2571FF'
