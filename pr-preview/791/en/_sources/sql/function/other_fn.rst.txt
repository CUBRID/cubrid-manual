
:meta-keywords: cubrid sleep, cubrid sys_guid

:tocdepth: 3

***************
Other functions
***************

.. contents::

SLEEP
=====

.. function:: SLEEP ( sec )

    This function pauses for the specified time then resumes the operations.

    :param sec: sleep time. The unit is second and inputs double type value.
    :rtype: INT

    .. code-block:: sql

        SELECT SLEEP(3);

    It pauses for 3 seconds.


SYS_GUID
========

.. function:: SYS_GUID () 

    It returns the unique hexadecimal string of 32 characters randomly.
     
     
    .. code-block:: sql 
     
        SELECT SYS_GUID(); 

    :: 
     
        sys_guid() 
        ================================== 
        '938210043A7B4455927A8697FB2571FF' 
