
:meta-keywords: cubrid date, cubrid time, cubrid datetime, datetime functions, datetime operators

:tocdepth: 3

*********************************
Date/Time Functions and Operators
*********************************

.. contents::

ADDDATE, DATE_ADD
=================

.. function:: ADDDATE (date, INTERVAL expr unit)
.. function:: ADDDATE (date, days)
   :noindex:
.. function:: DATE_ADD (date, INTERVAL expr unit)

    The **ADDDATE** function performs an addition or subtraction operation on a specific **DATE** value; **ADDDATE** and **DATE_ADD** are used interchangeably. The return value is a **DATE** or **DATETIME** type. The **DATETIME** type is returned in the following cases.

    *   The first argument is a **DATETIME** or **TIMESTAMP** type
    *   The first argument is a **DATE** type and the unit of **INTERVAL** value specified is less than the unit of day

    Therefore, to return value of **DATETIME** type, you should convert the value of first argument by using the **CAST** function. Even though the date resulting from the operation exceeds the last day of the month, the function returns a valid **DATE** value considering the last date of the month.

    If every input argument value of date and time is 0, the return value is determined by the **return_null_on_function_errors** system parameter; if it is set to yes, then **NULL** is returned; if it is set to no, an error is returned. The default value is **no**.

    If the calculated value is between '0000-00-00 00:00:00' and '0001-01-01 00:00:00', a value having 0 for all arguments is returned in **DATE** or **DATETIME** type. Note that operation in JDBC program is determined by the configuration of zeroDateTimeBehavior, connection URL property. For more information about JDBC connection URL, please see :ref:`jdbc-connection-conf`\ .

    :param date: It is a **DATE**, **TIMETIME**, or **TIMESTAMP** expression that represents the start date. If an invalid **DATE** value such as '2006-07-00' is specified, an error is returned.
    :param expr: It represents the interval value to be added to the start date. If a negative number is specified next to the **INTERVAL** keyword, the interval value is subtracted from the start date.
    :param unit: It represents the unit of the interval value specified in the *expr* expression. See the following table to specify the format for the interpretation of the interval value. If the value of *expr* unit is less than the number requested in the *unit*, it is specified from the smallest unit. For example, if it is HOUR_SECOND, three values such as 'HOURS:MINUTES:SECONDS' are required. In the case, if only two values such as "1:1" are given, it is regarded as 'MINUTES:SECONDS'.
    :rtype: **DATE** or **DATETIME** 

**Format of expr for unit**

+--------------------+-------------------------------------------+--------------------------------------------------------------+
| unit Value         | expr Format                               | Example                                                      |
+====================+===========================================+==============================================================+
| MILLISECOND        | MILLISECONDS                              | ADDDATE(SYSDATE, INTERVAL 123 MILLISECOND)                   |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| SECOND             | SECONDS                                   | ADDDATE(SYSDATE, INTERVAL 123 SECOND)                        |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| MINUTE             | MINUTES                                   | ADDDATE(SYSDATE, INTERVAL 123 MINUTE)                        |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| HOUR               | HOURS                                     | ADDDATE(SYSDATE, INTERVAL 123 HOUR)                          |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| DAY                | DAYS                                      | ADDDATE(SYSDATE, INTERVAL 123 DAY)                           |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| WEEK               | WEEKS                                     | ADDDATE(SYSDATE, INTERVAL 123 WEEK)                          |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| MONTH              | MONTHS                                    | ADDDATE(SYSDATE, INTERVAL 12 MONTH)                          |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| QUARTER            | QUARTERS                                  | ADDDATE(SYSDATE, INTERVAL 12 QUARTER)                        |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| YEAR               | YEARS                                     | ADDDATE(SYSDATE, INTERVAL 12 YEAR)                           |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| SECOND_MILLISECOND | 'SECONDS.MILLISECONDS'                    | ADDDATE(SYSDATE, INTERVAL '12.123' SECOND_MILLISECOND)       |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| MINUTE_MILLISECOND | 'MINUTES:SECONDS.MILLISECONDS'            | ADDDATE(SYSDATE, INTERVAL '12:12.123' MINUTE_MILLISECOND)    |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| MINUTE_SECOND      | 'MINUTES:SECONDS'                         | ADDDATE(SYSDATE, INTERVAL '12:12' MINUTE_SECOND)             |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| HOUR_MILLISECOND   | 'HOURS:MINUTES:SECONDS.MILLISECONDS'      | ADDDATE(SYSDATE, INTERVAL '12:12:12.123' HOUR_MILLISECOND)   |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| HOUR_SECOND        | 'HOURS:MINUTES:SECONDS'                   | ADDDATE(SYSDATE, INTERVAL '12:12:12' HOUR_SECOND)            |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| HOUR_MINUTE        | 'HOURS:MINUTES'                           | ADDDATE(SYSDATE, INTERVAL '12:12' HOUR_MINUTE)               |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| DAY_MILLISECOND    | 'DAYS HOURS:MINUTES:SECONDS.MILLISECONDS' | ADDDATE(SYSDATE, INTERVAL '12 12:12:12.123' DAY_MILLISECOND) |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| DAY_SECOND         | 'DAYS HOURS:MINUTES:SECONDS'              | ADDDATE(SYSDATE, INTERVAL '12 12:12:12' DAY_SECOND)          |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| DAY_MINUTE         | 'DAYS HOURS:MINUTES'                      | ADDDATE(SYSDATE, INTERVAL '12 12:12' DAY_MINUTE)             |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| DAY_HOUR           | 'DAYS HOURS'                              | ADDDATE(SYSDATE, INTERVAL '12 12' DAY_HOUR)                  |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| YEAR_MONTH         | 'YEARS-MONTHS'                            | ADDDATE(SYSDATE, INTERVAL '12-13' YEAR_MONTH)                |
+--------------------+-------------------------------------------+--------------------------------------------------------------+

.. code-block:: sql

    SELECT SYSDATE, ADDDATE(SYSDATE,INTERVAL 24 HOUR), ADDDATE(SYSDATE, 1);
     
::

    03/30/2010  12:00:00.000 AM 03/31/2010               03/31/2010
     
.. code-block:: sql

    --it subtracts days when argument < 0
    SELECT SYSDATE, ADDDATE(SYSDATE,INTERVAL -24 HOUR), ADDDATE(SYSDATE, -1);
     
::

     03/30/2010  12:00:00.000 AM 03/29/2010               03/29/2010
     
.. code-block:: sql

    --when expr is not fully specified for unit
    SELECT SYS_DATETIME, ADDDATE(SYS_DATETIME, INTERVAL '1:20' HOUR_SECOND);
     
::

    06:18:24.149 PM 06/28/2010     06:19:44.149 PM 06/28/2010                            
     
.. code-block:: sql

    SELECT ADDDATE('0000-00-00', 1 );
     
::

    ERROR: Conversion error in date format.
     
.. code-block:: sql

    SELECT ADDDATE('0001-01-01 00:00:00', -1);
     
::

    '12:00:00.000 AM 00/00/0000'

ADDTIME
=======

.. function:: ADDTIME(expr1, expr2)

    The **ADDTIME** function adds or subtracts a value of specific time. The first argument is **DATE**, **DATETIME**, **TIMESTAMP**, or **TIME** type and the second argument is **TIME**, **DATETIME**, or **TIMESTAMP** type. Time should be included in the second argument, and the date of the second argument is ignored. The return type for each argument type is follows:

    +-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+
    | First Argument Type     | Second Argument Type                     | Return Type     | Note                                                     |
    +=========================+==========================================+=================+==========================================================+
    | TIME                    | TIME, DATETIME, TIMESTAMP                | TIME            | The result value must be equal to or less than 24 hours. |
    +-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+
    | DATE                    | TIME, DATETIME, TIMESTAMP                | DATETIME        |                                                          |
    +-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+
    | DATETIME                | TIME, DATETIME, TIMESTAMP                | DATETIME        |                                                          |
    +-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+
    | date/time string        | TIME, DATETIME, TIMESTAMP or time string | VARCHAR         | The result string includes time.                         |
    +-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+

    :param expr1: **DATE**, **DATETIME**, **TIME** or **TIMESTAMP** type
    :param expr2: **DATETIME**, **TIMESTAMP**, **TIME** type or date/time string

.. code-block:: sql

    SELECT ADDTIME(datetime'2007-12-31 23:59:59', time'1:1:2');
    
::

    01:01:01.000 AM 01/01/2008
     
.. code-block:: sql

    SELECT ADDTIME(time'01:00:00', time'02:00:01');
    
::

    03:00:01 AM



ADD_MONTHS
==========

.. function:: ADD_MONTHS ( date_argument , month )

    The **ADD_MONTHS** function adds a *month* value to the expression *date_argument* of **DATE** type, and it returns a **DATE** type value. If the day (*dd*) of the value specified as an argument exists within the month of the result value of the operation, it returns the given day (*dd*); otherwise returns the last day of the given month (*dd*). If the result value of the operation exceeds the expression range of the **DATE** type, it returns an error.

    :param date_argument: Specifies an expression of **DATE** type. To specify a **TIMESTAMP** or **DATETIME** value, an explicit casting to **DATE** type is required. If the value is **NULL**, **NULL** is returned.
    :param month: Specifies the number of the months to be added to the *date_argument*. Both positive and negative values can be specified. If the given value is not an integer type, conversion to an integer type by an implicit casting (rounding to the first place after the decimal point) is performed. If the value is **NULL**, **NULL** is returned.

.. code-block:: sql

    --it returns DATE type value by adding month to the first argument
    SELECT ADD_MONTHS(DATE '2008-12-25', 5), ADD_MONTHS(DATE '2008-12-25', -5);
    
::

      05/25/2009                         07/25/2008
     
     
.. code-block:: sql

    SELECT ADD_MONTHS(DATE '2008-12-31', 5.5), ADD_MONTHS(DATE '2008-12-31', -5.5);
    
::

      06/30/2009                           06/30/2008
     
.. code-block:: sql

    SELECT ADD_MONTHS(CAST (SYS_DATETIME AS DATE), 5), ADD_MONTHS(CAST (SYS_TIMESTAMP AS DATE), 5);

::

      07/03/2010                                     07/03/2010

The following are examples of using timezone type values. For timezone related description, see :ref:`timezone-type`.

.. code-block:: sql

    SELECT ADD_MONTHS (datetimeltz'2001-10-11 10:11:12', 1);

::

    11/11/2001

.. code-block:: sql

    SELECT ADD_MONTHS (datetimetz'2001-10-11 10:11:12 Europe/Paris', 1);

::

    11/11/2001

.. code-block:: sql

    SELECT ADD_MONTHS (timestampltz'2001-10-11 10:11:12', 1);

::

    11/11/2001

.. code-block:: sql

    SELECT ADD_MONTHS (timestamptz'2001-10-11 10:11:12 Europe/Paris', 1);

::

    11/11/2001

CURDATE, CURRENT_DATE
=====================

.. function:: CURDATE ()
.. function:: CURRENT_DATE ()
.. c:macro:: CURRENT_DATE

    **CURDATE** (), **CURRENT_DATE** and **CURRENT_DATE** () are used interchangeably and they return the current date of session as the **DATE** type (*MM*/*DD*/*YYYY* or *YYYY*-*MM*-*DD*). The unit is day.
    When the time zone of the current session is same as that of server, these functions are same as :c:macro:`SYS_DATE`, :c:macro:`SYSDATE`. Please refer :c:macro:`SYS_DATE`, :c:macro:`SYSDATE` and the following examples to find a difference and :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE` for details of the functions.
    
    If input every argument value of year, month, and day is 0, the return value is determined by the **return_null_on_function_errors** system parameter; if it is set to yes, then **NULL** is returned; if it is set to no, an error is returned. The default value is **no**.

    :rtype: DATE
    
.. code-block:: sql

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'Asia/Seoul'        

    -- it returns the current date in DATE type
    
    SELECT CURDATE(), CURRENT_DATE(), CURRENT_DATE, SYS_DATE, SYSDATE;

       CURRENT_DATE    CURRENT_DATE    CURRENT_DATE    SYS_DATE    SYS_DATE 
    ========================================================================
      02/05/2016      02/05/2016      02/05/2016      02/05/2016  02/05/2016

.. code-block:: sql

    -- it returns the date 60 days added to the current date
    
    SELECT CURDATE()+60;

       CURRENT_DATE +60
    ===================
      04/05/2016    


.. code-block:: sql

    -- change session time from 'Asia/Seoul' to 'America/Los_Angeles'
    
    SET TIME ZONE 'America/Los_Angeles';

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'America/Los_Angeles'

    -- Note that CURDATE() and SYS_DATE returns different results
    
    SELECT CURDATE(), CURRENT_DATE(), CURRENT_DATE, SYS_DATE, SYSDATE;

       CURRENT_DATE    CURRENT_DATE    CURRENT_DATE    SYS_DATE    SYS_DATE 
    ========================================================================
      02/04/2016      02/04/2016      02/04/2016      02/05/2016  02/05/2016

.. warning::
    
    As 10.0, **CURDATE** (), **CURRENT_DATE**, **CURRENT_DATE** () are different from **SYS_DATE** and **SYSDATE**. They are synonym for 9.x and lower. 
    
CURRENT_DATETIME, NOW
=====================

.. function:: CURRENT_DATETIME ()
.. c:macro:: CURRENT_DATETIME
.. function:: NOW ()

    **CURRENT_DATETIME**, **CURRENT_DATETIME** () and **NOW** () are used interchangeably, and they return the current date and time of session in **DATETIME** type. The unit is millisecond.
    When the time zone of the current session is same as that of server, these functions are same as :c:macro:`SYS_DATETIME`, :c:macro:`SYSDATETIME`. Please also refer :c:macro:`SYS_DATETIME`, :c:macro:`SYSDATETIME` to find a difference and :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE` for details of the functions.
    
    :rtype: DATETIME
    
.. code-block:: sql

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'Asia/Seoul'        

    -- it returns the current date and time in DATETIME type

    SELECT NOW(), SYS_DATETIME;                                                                                                                                                

       CURRENT_DATETIME               SYS_DATETIME                
    ==============================================================
      04:05:09.292 PM 02/05/2016     04:05:09.292 PM 02/05/2016   

.. code-block:: sql

    -- it returns the timestamp value 1 hour added to the current sys_datetime value
    
    SELECT TO_CHAR(SYSDATETIME+3600*1000, 'YYYY-MM-DD HH24:MI');

       to_char( SYS_DATETIME +3600*1000, 'YYYY-MM-DD HH24:MI')
    ======================
      '2016-02-05 17:05'  

.. code-block:: sql

    -- change session time from 'Asia/Seoul' to 'America/Los_Angeles'
    
    set time zone 'America/Los_Angeles';

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'America/Los_Angeles'

    -- Note that NOW() and SYS_DATETIME return different results
    
    SELECT NOW(), SYS_DATETIME;

       CURRENT_DATETIME               SYS_DATETIME                
    ==============================================================
      11:08:57.041 PM 02/04/2016     04:08:57.041 PM 02/05/2016   
  
.. warning::

    As 10.0, **CURRENT_DATETIME** (), **NOW** () are different from **SYS_DATEIME**, **SYSDATETIME**. They are synonym for 9.x and lower. 
    
CURTIME, CURRENT_TIME
=====================

.. function:: CURTIME ()
.. c:macro:: CURRENT_TIME
.. function:: CURRENT_TIME ()

    **CURTIME** (), **CURRENT_TIME** and **CURRENT_TIME** () are used interchangeably and they return the current time of session as **TIME** type (*HH*:*MI*:*SS*). The unit is second.
    When the time zone of the current session is same as that of server, these functions are same as :c:macro:`SYS_TIME`, :c:macro:`SYSTIME`. Please also refer :c:macro:`SYS_TIME`, :c:macro:`SYSTIME` to find a difference and :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE` for details of the functions.
    
    :rtype: TIME
    
.. code-block:: sql

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'Asia/Seoul'        

    -- it returns the current time in TIME type
    
    SELECT CURTIME(), CURRENT_TIME(), CURRENT_TIME, SYS_TIME, SYSTIME;

       CURRENT_TIME    CURRENT_TIME    CURRENT_TIME    SYS_TIME     SYS_TIME  
    ==========================================================================
      04:22:54 PM     04:22:54 PM     04:22:54 PM     04:22:54 PM  04:22:54 PM


.. code-block:: sql

    -- change session time from 'Asia/Seoul' to 'America/Los_Angeles'
    
    SET TIME ZONE 'AMERICA/LOS_ANGELES';

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'America/Los_Angeles'

    -- Note that CURTIME() and SYS_TIME return different results
    
    SELECT CURTIME(), CURRENT_TIME(), CURRENT_TIME, SYS_TIME, SYSTIME;

       CURRENT_TIME    CURRENT_TIME    CURRENT_TIME    SYS_TIME     SYS_TIME  
    ==========================================================================
      11:23:16 PM     11:23:16 PM     11:23:16 PM     04:23:16 PM  04:23:16 PM

.. warning::

    As 10.0, **CURTIME** (), **CURRENT_TIME** () are different from **SYS_TIME**, **SYSTIME**.  They are synonym for 9.x and lower. 

CURRENT_TIMESTAMP, LOCALTIME, LOCALTIMESTAMP
============================================

.. c:macro:: CURRENT_TIMESTAMP
.. function:: CURRENT_TIMESTAMP ()
.. c:macro:: LOCALTIME
.. function:: LOCALTIME ()
.. c:macro:: LOCALTIMESTAMP
.. function:: LOCALTIMESTAMP ()

    **CURRENT_TIMESTAMP**, **CURRENT_TIMESTAMP** (), **LOCALTIME**, **LOCALTIME** (), **LOCALTIMESTAMP** and **LOCALTIMESTAMP** () are used interchangeably and they return the current date and time of session as **TIMESTAMP** type. The unit is second.
    When the time zone of the current session is same as that of server, these functions are same as :c:macro:`SYS_TIMESTAMP`, :c:macro:`SYSTIMESTAMP`.  Please also refer :c:macro:`SYS_TIMESTAMP`, :c:macro:`SYSTIMESTAMP` to find a difference and :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE` for details of the functions.
    
    :rtype: TIMESTAMP
    
.. code-block:: sql

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'Asia/Seoul'        

    -- it returns the current date and time in TIMESTAMP type of session and server timezones.
    
    SELECT LOCALTIME, SYS_TIMESTAMP;

       CURRENT_TIMESTAMP          SYS_TIMESTAMP           
    ======================================================
      04:34:16 PM 02/05/2016     04:34:16 PM 02/05/2016   

.. code-block:: sql
 
    -- change session time from 'Asia/Seoul' to 'America/Los_Angeles'
    
    SET TIME ZONE 'America/Los_Angeles';

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();                                                                                                                                    

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'America/Los_Angeles'

    -- Note that LOCALTIME() and SYS_TIMESTAMP return different results
    
    SELECT LOCALTIME, SYS_TIMESTAMP;                                                                                                                                           

       CURRENT_TIMESTAMP          SYS_TIMESTAMP           
    ======================================================
      11:34:37 PM 02/04/2016     04:34:37 PM 02/05/2016   

.. warning::

    As 10.0, **CURRENT_TIMESTAMP**, **CURRENT_TIMESTAMP** (), **LOCALTIME**, **LOCALTIME** (), **LOCALTIMESTAMP** and **LOCALTIMESTAMP** () are different from **SYS_TIMESTAMP** (), **SYSTIMESTAMP**.  They are synonym for 9.x and lower. 


DATE
====

.. function:: DATE (date)

    The **DATE** function extracts the date part from specified argument, and returns it as '*MM*/*DD*/*YYYY*' format string. Arguments that can be specified are **DATE**, **TIMESTAMP** and **DATETIME** types. The return value is a **VARCHAR** type.

    0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to date and time, string where 0 is specified for year, month, and day is returned.

    :param date: **DATE**, **TIMESTAMP** or **DATETIME** can be specified.
    :rtype: STRING

.. code-block:: sql

    SELECT DATE('2010-02-27 15:10:23');
    
::

    '02/27/2010'
     
.. code-block:: sql

    SELECT DATE(NOW());
    
::

    '04/01/2010'
     
.. code-block:: sql

    SELECT DATE('0000-00-00 00:00:00');
    
::

   '00/00/0000'

DATEDIFF
========

.. function:: DATEDIFF (date1, date2)

    The **DATEDIFF** function returns the difference between two arguments as an integer representing the number of days. Arguments that can be specified are **DATE**, **TIMESTAMP** and **DATETIME** types and its return value is only **INTEGER** type.

    If every input argument value of date and time is 0, the return value is determined by the **return_null_on_function_errors** system parameter; if it is set to yes, then **NULL** is returned; if it is set to no, an error is returned. The default value is **no**.

    :param date1,date2: Specifies the types that include date (**DATE**, **TIMESTAMP** or **DATETIME**) type or string that represents the value of corresponding type. If invalid string is specified, an error is returned.
    :rtype: INT

.. code-block:: sql

    SELECT DATEDIFF('2010-2-28 23:59:59','2010-03-02');
    
::

    -2
     
.. code-block:: sql

    SELECT DATEDIFF('0000-00-00 00:00:00', '2010-2-28 23:59:59');

::
    
    ERROR: Conversion error in date format.

The following are examples of using timezone type values. For timezone related description, see :ref:`timezone-type`.

.. code-block:: sql

    SELECT IF(DATEDIFF('2002-03-03 12:00:00 AM','1990-01-01 11:59:59 PM') = DATEDIFF(timestampltz'2002-03-03 12:00:00 AM',timestampltz'1990-01-01 11:59:59 PM'),'ok','nok');

::

    'ok'

DATE_SUB, SUBDATE
=================

.. function:: DATE_SUB (date, INTERVAL expr unit)
.. function:: SUBDATE(date, INTERVAL expr unit)
.. function:: SUBDATE(date, days)
   :noindex:

    The functions **DATE_SUB** and **SUBDATE** () are used interchangeably and they perform an addition or subtraction operation on a specific **DATE** value. The value is returned in **DATE** or **DATETIME** type. If the date resulting from the operation exceeds the last day of the month, the function returns a valid **DATE** value considering the last date of the month.

    If every input argument value of date and time is 0, the return value is determined by the **return_null_on_function_errors** system parameter; if it is set to yes, then **NULL** is returned; if it is set to no, an error is returned. The default value is **no**.

    If the calculated value is between '0000-00-00 00:00:00' and '0001-01-01 00:00:00', a value having 0 for all arguments is returned in **DATE** or **DATETIME** type. Note that operation in JDBC program is determined by the configuration of zeroDateTimeBehavior, connection URL property (see :ref:`jdbc-connection-conf` for details).

    :param date: It is a **DATE** or **TIMESTAMP** expression that represents the start date. If an invalid **DATE** value such as '2006-07-00' is specified, **NULL** is returned.
    :param expr: It represents the interval value to be subtracted from the start date. If a negative number is specified next to the **INTERVAL** keyword, the interval value is added to the start date.
    :param unit: It represents the unit of the interval value specified in the *exp* expression. To check the expr argument for the unit value, see the table of :func:`ADDDATE`.
    :rtype: DATE or DATETIME

.. code-block:: sql

    SELECT SYSDATE, SUBDATE(SYSDATE,INTERVAL 24 HOUR), SUBDATE(SYSDATE, 1);
    
::

      03/30/2010  12:00:00.000 AM 03/29/2010               03/29/2010
     
.. code-block:: sql

    --it adds days when argument < 0
    SELECT SYSDATE, SUBDATE(SYSDATE,INTERVAL -24 HOUR), SUBDATE(SYSDATE, -1);
    
::

      03/30/2010  12:00:00.000 AM 03/31/2010               03/31/2010
     
.. code-block:: sql

    SELECT SUBDATE('0000-00-00 00:00:00', -50);
    
::

    ERROR: Conversion error in date format.
     
.. code-block:: sql

    SELECT SUBDATE('0001-01-01 00:00:00', 10);
    
::

     '12:00:00.000 AM 00/00/0000'

DAY, DAYOFMONTH
===============

.. function:: DAY (date)
.. function:: DAYOFMONTH (date)

    The function **DAY** or **DAYOFMONTH** returns day in the range of 1 to 31 from the specified parameter. You can specify the **DATE**, **TIMESTAMP** or **DATETIME** type; the value is returned in **INTEGER** type. 
    
    0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to year, month, and day, 0 is returned as an exception.

    :param date: Date
    :rtype: INT

.. code-block:: sql

    SELECT DAYOFMONTH('2010-09-09');
    
::

    9
     
.. code-block:: sql

    SELECT DAY('2010-09-09 19:49:29');
    
::

    9
     
.. code-block:: sql

    SELECT DAYOFMONTH('0000-00-00 00:00:00');
    
::

    0

DAYOFWEEK
=========

.. function:: DAYOFWEEK (date)

    The **DAYOFWEEK** function returns a day in the range of 1 to 7 (1: Sunday, 2: Monday, ..., 7: Saturday) from the specified parameters. The day index is same as the ODBC standards. You can specify the **DATE**, **TIMESTAMP** or **DATETIME** type; the value is returned in **INTEGER** type.

    If every input argument value of year, month, and day is 0, the return value is determined by the **return_null_on_function_errors** system parameter; if it is set to yes, then **NULL** is returned; if it is set to no, an error is returned. The default value is **no**.

    :param date: Date
    :rtype: INT

.. code-block:: sql

    SELECT DAYOFWEEK('2010-09-09');
    
::

    5
     
.. code-block:: sql

    SELECT DAYOFWEEK('2010-09-09 19:49:29');
    
::

    5
     
.. code-block:: sql

    SELECT DAYOFWEEK('0000-00-00');
    
::

    ERROR: Conversion error in date format.

DAYOFYEAR
=========

.. function:: DAYOFYEAR (date)

    The **DAYOFYEAR** function returns the day of a year in the range of 1 to 366. You can specify the **DATE**, **TIMESTAMP** or **DATETIME** types; the value is returned in **INTEGER** type.

    If every input argument value of year, month, and day is 0, the return value is determined by the **return_null_on_function_errors** system parameter; if it is set to yes, then **NULL** is returned; if it is set to no, an error is returned. The default value is **no**.

    :param date: Date
    :rtype: INT

.. code-block:: sql

    SELECT DAYOFYEAR('2010-09-09');
    
::

    252
     
.. code-block:: sql

    SELECT DAYOFYEAR('2010-09-09 19:49:29');
    
::

    252
     
.. code-block:: sql

    SELECT DAYOFYEAR('0000-00-00');
    
::

    ERROR: Conversion error in date format.

EXTRACT
=======

.. function:: EXTRACT ( field FROM date-time_argument )

    The **EXTRACT** operator extracts the values from *date-time_argument* and then converts the value type into **INTEGER**. 
    
    0 is not allowed in the input argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to date and time, 0 is returned as an exception.

    :param field: Specifies a value to be extracted from date-time expression.
    :param date-time_argument: An expression that returns a value of date-time. This expression must be one of **TIME**, **DATE**, **TIMESTAMP**, or **DATETIME** types. If the value is **NULL**, **NULL** is returned.
    :rtype: INT

.. code-block:: sql

    SELECT EXTRACT(MONTH FROM DATETIME '2008-12-25 10:30:20.123' );
    
::

    12
     
.. code-block:: sql

    SELECT EXTRACT(HOUR FROM DATETIME '2008-12-25 10:30:20.123' );
    
::

    10
     
.. code-block:: sql

    SELECT EXTRACT(MILLISECOND FROM DATETIME '2008-12-25 10:30:20.123' );
    
::

    123
     
.. code-block:: sql

    SELECT EXTRACT(MONTH FROM '0000-00-00 00:00:00');
    
::

    0

The following are examples of using timezone type values. For timezone related description, see :ref:`timezone-type`.

.. code-block:: sql

    SELECT EXTRACT (MONTH FROM datetimetz'10/15/1986 5:45:15.135 am Europe/Bucharest');

::

    10
    
.. code-block:: sql

    SELECT EXTRACT (MONTH FROM datetimeltz'10/15/1986 5:45:15.135 am Europe/Bucharest');

::

    10

.. code-block:: sql

    SELECT EXTRACT (MONTH FROM timestampltz'10/15/1986 5:45:15 am Europe/Bucharest');

::

    10

.. code-block:: sql

    SELECT EXTRACT (MONTH FROM timestamptz'10/15/1986 5:45:15 am Europe/Bucharest');

::

    10

FROM_DAYS
=========

.. function:: FROM_DAYS (N)

    The **FROM_DAYS** function returns a date value in **DATE** type if **INTEGER** type is inputted as an argument.

    It is not recommended to use the **FROM_DAYS** function for dates prior to the year 1582 because the function does not take dates prior to the introduction of the Gregorian Calendar into account.

    If a value in the range of 0 to 3,652,424 can be inputted as an argument. If a value in the range of 0 to 365 is inputted, 0 is returned. 3,652,424, which is the maximum value, means the last day of year 9999.

    :param N: Integer in the range of 0 to 3,652,424
    :rtype: DATE

.. code-block:: sql

    SELECT FROM_DAYS(719528);
    
::

    01/01/1970
     
.. code-block:: sql

    SELECT FROM_DAYS('366');
    
::

    01/03/0001
     
.. code-block:: sql

    SELECT FROM_DAYS(3652424);
    
::

    12/31/9999
     
.. code-block:: sql

    SELECT FROM_DAYS(0);
    
::

    00/00/0000

FROM_TZ
=======
      
.. function:: FROM_TZ(datetime, timezone_string)

    Converts date/time type without timezone as date/time type with timezone by adding timezone to DATETIME typed value. Input value's type is DATETIME, and the result value's type is DATETIMETZ.

    :param datetime: DATETIME
    :param timezone_string: String representing a timezone name or and offset '+05:00', 'Asia/Seoul'.
    :rtype: DATETIMETZ
    
.. code-block:: sql

    SELECT FROM_TZ(datetime '10/10/2014 00:00:00 AM', 'Europe/Vienna');

::

    12:00:00.000 AM 10/10/2014 Europe/Vienna CEST

.. code-block:: sql

    SELECT FROM_TZ(datetime '10/10/2014 23:59:59 PM', '+03:25:25');

::

    11:59:59.000 PM 10/10/2014 +03:25:25


For timezone related description, see :ref:`timezone-type`.

.. seealso::

    :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE`, :func:`NEW_TIME`, :func:`TZ_OFFSET`

FROM_UNIXTIME
=============

.. function:: FROM_UNIXTIME ( unix_timestamp[, format] )

    The **FROM_UNIXTIME** function returns the string of the specified format in **VARCHAR** type if the argument *format* is specified; if the argument *format* is omitted, it returns a value of **TIMESTAMP** type. Specify the arguement *unix_timestamp* as an **INTEGER** type that corresponds to the UNIX timestamp. The returned value is displayed in the current time zone.
    
    It displays the result according to the format that you specified, and the date/time format, *format* follows the Date/Time Format 2 table of :func:`DATE_FORMAT`.

    The relation is not one of one-to-one correspondence between **TIMESTAMP** and UNIX timestamp so if you use :func:`UNIX_TIMESTAMP` or **FROM_UNIXTIME** function, partial value could be lost. For details, see :func:`UNIX_TIMESTAMP`.

    0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to date and time, string where 0 is specified for every date and time value is returned. Note that operation in JDBC program is determined by the configuration of zeroDateTimeBehavior, connection URL property (see :ref:`jdbc-connection-conf` for details).

    :param unix_timestamp: Positive integer
    :param format: Time format. Follows the date/time format of the :func:`DATE_FORMAT`.
    :rtype: STRING, INT

.. code-block:: sql

    SELECT FROM_UNIXTIME(1234567890);
    
::

    01:31:30 AM 02/14/2009
     
.. code-block:: sql

    SELECT FROM_UNIXTIME('1000000000');
    
::

    04:46:40 AM 09/09/2001
     
.. code-block:: sql

    SELECT FROM_UNIXTIME(1234567890,'%M %Y %W');
    
::

    'February 2009 Saturday'
     
.. code-block:: sql

    SELECT FROM_UNIXTIME('1234567890','%M %Y %W');
    
::

    'February 2009 Saturday'
     
.. code-block:: sql

    SELECT FROM_UNIXTIME(0);
    
::

    12:00:00 AM 00/00/0000

HOUR
====

.. function:: HOUR (time)

    The **HOUR** function extracts the hour from the specified parameter and then returns the value in integer. The type **TIME**, **TIMESTAMP** or **DATETIME** can be specified and a value is returned in the **INTEGER** type.

    :param time: Time
    :rtype: INT

.. code-block:: sql

    SELECT HOUR('12:34:56');
    
::

    12
     
.. code-block:: sql

    SELECT HOUR('2010-01-01 12:34:56');
    
::

    12
     
.. code-block:: sql

    SELECT HOUR(datetime'2010-01-01 12:34:56');
    
::

    12

LAST_DAY
========

.. function:: LAST_DAY ( date_argument )

    The **LAST_DAY** function returns the last day of the given month as **DATE** type.

    If every input argument value of year, month, and day is 0, the return value is determined by the **return_null_on_function_errors** system parameter; if it is set to yes, then **NULL** is returned; if it is set to no, an error is returned. The default value is **no**.

    :param date_argument: Specifies an expression of **DATE** type. To specify a **TIMESTAMP** or **DATETIME** value, explicit casting to **DATE** is required. If the value is **NULL**, **NULL** is returned.
    :rtype: DATE

.. code-block:: sql

    --it returns last day of the month in DATE type
    SELECT LAST_DAY(DATE '1980-02-01'), LAST_DAY(DATE '2010-02-01');
    
::

    02/28/1980                    02/28/2010
     
.. code-block:: sql

    --it returns last day of the month when explicitly casted to DATE type
    SELECT LAST_DAY(CAST (SYS_TIMESTAMP AS DATE)), LAST_DAY(CAST (SYS_DATETIME AS DATE));
    
::

    02/28/2010                                 02/28/2010
     
.. code-block:: sql

    SELECT LAST_DAY('0000-00-00');
    
::
    
    ERROR: Conversion error in date format.

MAKEDATE
========

.. function:: MAKEDATE (year, dayofyear)

    The **MAKEDATE** function returns a date from the specified parameter. You can specify an **INTEGER** type corresponding to the day of the year in the range of 1 to 9999 as an argument; the value in the range of 1/1/1 to 12/31/9999 is returned in **DATE** type. If the day of the year has passed the corresponding year, it will become the next year. For example, MAKEDATE(1999, 366) will return 2000-01-01. However, if you input a value in the range of 0 to 69 as the year, it will be processed as the year 2000-2069, if it is a value in the range of 70 to 99, it will be processed as the year 1970-1999.

    If every value specified in *year* and *dayofyear* is 0, the return value is determined by the **return_null_on_function_errors** system parameter; if it is set to yes, then **NULL** is returned; if it is set to no, an error is returned. The default value is **no**.

    :param year: Year in the range of 1 to 9999
    :param dayofyear: If you input a value in the range of 0 to 99 in the argument, it is handled as an exception; *dayofyear* must be equal to or less than 3,615,902 and the return value of MAKEDATE(100, 3615902) is 9999/12/31.
    :rtype: DATE

.. code-block:: sql

    SELECT MAKEDATE(2010,277);

::

    10/04/2010
     
.. code-block:: sql

    SELECT MAKEDATE(10,277);
    
::
    
    10/04/2010
     
.. code-block:: sql

    SELECT MAKEDATE(70,277);
    
::
    
    10/04/1970
     
.. code-block:: sql

    SELECT MAKEDATE(100,3615902);
    
::
    
    12/31/9999
     
.. code-block:: sql

    SELECT MAKEDATE(9999,365);
    
::
    
    12/31/9999
     
.. code-block:: sql

    SELECT MAKEDATE(0,0);
    
::
    
    ERROR: Conversion error in date format.

MAKETIME
========

.. function:: MAKETIME(hour, min, sec)

    The **MAKETIME** function returns the hour from specified argument in the AM/PM format. You can specify the **INTEGER** types corresponding hours, minutes and seconds as arguments; the value is returned in **DATETIME**.

    :param hour: An integer representing the hours in the range of 0 to 23
    :param min: An integer representing the minutes in the range of 0 to 59
    :param sec: An integer representing the minutes in the range of 0 to 59
    :rtype: DATETIME

.. code-block:: sql

    SELECT MAKETIME(13,34,4);
    
::

    01:34:04 PM
     
.. code-block:: sql

    SELECT MAKETIME('1','34','4');
    
::

    01:34:04 AM
     
.. code-block:: sql

    SELECT MAKETIME(24,0,0);
     
::
    
    ERROR: Conversion error in time format.

MINUTE
======

.. function:: MINUTE (time)

    The **MINUTE** function returns the minutes in the range of 0 to 59 from specified argument. You can specify the **TIME** , **TIMESTAMP** or **DATETIME** type; the value is returned in **INTEGER** type.

    :param time: Time
    :rtype: INT

.. code-block:: sql

    SELECT MINUTE('12:34:56');
    
::

    34
     
.. code-block:: sql

    SELECT MINUTE('2010-01-01 12:34:56');
    
::

    34
     
.. code-block:: sql

    SELECT MINUTE('2010-01-01 12:34:56.7890');
    
::

    34

MONTH
=====

.. function:: MONTH (date)

    The **MONTH** function returns the month in the range of 1 to 12 from specified argument. You can specify the **DATE**, **TIMESTAMP** or **DATETIME** type; the value is returned in **INTEGER** type. 

    0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to date, 0 is returned as an exception.

    :param date: Date
    :rtype: INT

.. code-block:: sql

    SELECT MONTH('2010-01-02');
    
::

    1
     
.. code-block:: sql

    SELECT MONTH('2010-01-02 12:34:56');
    
::

    1
     
.. code-block:: sql

    SELECT MONTH('2010-01-02 12:34:56.7890');
    
::

    1
     
.. code-block:: sql

    SELECT MONTH('0000-00-00');
    
::

    0

MONTHS_BETWEEN
==============

.. function:: MONTHS_BETWEEN (date_argument, date_argument)

    The **MONTHS_BETWEEN** function returns the difference between the given **DATE** value. The return value is **DOUBLE** type. An integer value is returned if the two dates specified as arguments are identical or are the last day of the given month; otherwise, a value obtained by dividing the day difference by 31 is returned.

    :param date_argument: Specifies an expression of **DATE** type. **TIMESTAMP** or **DATETIME** value also can be used. If the value is **NULL**, **NULL** is returned.
    :rtype: DOUBLE

.. code-block:: sql

    --it returns the negative months when the first argument is the previous date
    SELECT MONTHS_BETWEEN(DATE '2008-12-31', DATE '2010-6-30');
    
::

    -1.800000000000000e+001
     
.. code-block:: sql

    --it returns integer values when each date is the last date of the month
    SELECT MONTHS_BETWEEN(DATE '2010-6-30', DATE '2008-12-31');
    
::

    1.800000000000000e+001
     
.. code-block:: sql

    --it returns months between two arguments when explicitly casted to DATE type
    SELECT MONTHS_BETWEEN(CAST (SYS_TIMESTAMP AS DATE), DATE '2008-12-25');
    
::

    1.332258064516129e+001
     
.. code-block:: sql

    --it returns months between two arguments when explicitly casted to DATE type
    SELECT MONTHS_BETWEEN(CAST (SYS_DATETIME AS DATE), DATE '2008-12-25');
    
::

    1.332258064516129e+001

The following are examples of using timezone type values. For timezone related description, see :ref:`timezone-type`.

.. code-block:: sql

    SELECT MONTHS_BETWEEN(datetimetz'2001-10-11 10:11:12 +02:00', datetimetz'2001-05-11 10:11:12 +03:00');

::

    5.000000000000000e+00

NEW_TIME
========
      
.. function:: NEW_TIME(src_datetime, src_timezone, dst_timezone)

    Moves a date value from a timezone to the other timezone. The type of *src_datetime* is DATETIME or TIME, and the return value's type is the same with the *src_datetime*\'s value.
    
    :param src_datetime: input value of DATETIME or TIME
    :param src_timezone: a region name of a source timezone
    :param dst_timezion: a region name of a target timezone
    :rtype: the same type with a *src_datetime*\s type

.. code-block:: sql

    SELECT NEW_TIME(datetime '10/10/2014 10:10:10 AM', 'Europe/Vienna', 'Europe/Bucharest');

::

    11:10:10.000 AM 10/10/2014

TIME type only accept an offset timezone as an input argument; a region name is not allowed.

.. code-block:: sql

    SELECT NEW_TIME(time '10:10:10 PM', '+03:00', '+10:00');

::

    05:10:10 AM

To see the timezone related description, see :ref:`timezone-type`.

.. seealso:: 

    :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE`, :func:`FROM_TZ`, :func:`TZ_OFFSET`

QUARTER
=======

.. function:: QUARTER (date)

    The **QUARTER** function returns the quarter in the range of 1 to 4 from specified argument. You can specify the **DATE**, **TIMESTAMP** or **DATETIME** type; the value is returned in **INTEGER** type.

    :param date: Date
    :rtype: INT

.. code-block:: sql

    SELECT QUARTER('2010-05-05');

::

    2
     
.. code-block:: sql

    SELECT QUARTER('2010-05-05 12:34:56');
    
::

    2
     
.. code-block:: sql

    SELECT QUARTER('2010-05-05 12:34:56.7890');
    
::

    2

The following are examples of using timezone type values. For timezone related description, see :ref:`timezone-type`.

.. code-block:: sql

    SELECT QUARTER('2008-04-01 01:02:03 Asia/Seoul');

::

    2

.. code-block:: sql

    SELECT QUARTER(datetimetz'2003-12-31 01:02:03.1234 Europe/Paris');

::

    4

.. _round-date:

ROUND
=====

.. function:: ROUND(date, fmt)

    This function rounds date to the unit specified by the format string, *fmt*. It returns a value of DATE type.
    
    :param date: The value of **DATE**, **TIMESTAMP** or **DATETIME**
    :param fmt: Specifies the format for the truncating unit. If omitted, "dd" is default.
    :rtype: DATE

    The format and its unit and the return value are as follows:
    
    +-------------------+----------+-----------------------------------------------------------------------+
    | Format            | Unit     | Return value                                                          |
    +===================+==========+=======================================================================+
    | 'yyyy' or 'yy'    | year     | a value rounded to year                                               |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'mm' or 'month'   | month    | a value rounded to month                                              |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'q'               | quarter  | a value rounded to quarter, one of 1/1, 4/1, 7/1, 10/1                |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'day'             | week     | a value rounded to week, this Sunday of *date* week                   | 
    |                   |          | or the next Sunday of *date* week                                     |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'dd'              | day      | a value rounded to day                                                |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'hh'              | hour     | a value rounded to hour                                               |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'mi'              | minute   | a value rounded to minute                                             |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'ss'              | second   | a value rounded to second                                             |
    +-------------------+----------+-----------------------------------------------------------------------+

.. code-block:: sql

    SELECT ROUND(date'2012-10-26', 'yyyy');

::

    01/01/2013

.. code-block:: sql

    SELECT ROUND(timestamp'2012-10-26 12:10:10', 'mm');

::

    11/01/2012
    
.. code-block:: sql

    SELECT ROUND(datetime'2012-12-26 12:10:10', 'dd');

::

    12/27/2012
    
.. code-block:: sql

    SELECT ROUND(datetime'2012-12-26 12:10:10', 'day');

::

    12/30/2012

.. code-block:: sql

    SELECT ROUND(datetime'2012-08-26 12:10:10', 'q');

::

    10/01/2012
    
.. code-block:: sql

    SELECT TRUNC(datetime'2012-08-26 12:10:10', 'q');

::

    07/01/2012
    
.. code-block:: sql

    SELECT ROUND(datetime'2012-02-28 23:10:00', 'hh');

::

    02/28/2012
    
.. code-block:: sql

    SELECT ROUND(datetime'2012-02-28 23:58:59', 'hh');

::

    02/29/2012
    
.. code-block:: sql

    SELECT ROUND(datetime'2012-02-28 23:59:59', 'mi');

::

    02/29/2012
    
.. code-block:: sql

    SELECT ROUND(datetime'2012-02-28 23:59:59.500', 'ss');

::

    02/29/2012
    
In order to truncate date instead of rounding, please see :ref:`TRUNC(date, fmt) <trunc-date>`.

SEC_TO_TIME
===========
  
.. function:: SEC_TO_TIME (second)

    The **SEC_TO_TIME** function returns the time including hours, minutes and seconds from specified argument. You can specify the **INTEGER** type in the range of 0 to 86,399; the value is returned in **TIME** type.

    :param second: Seconds in the range of 0 to 86,399
    :rtype: TIME

.. code-block:: sql

    SELECT SEC_TO_TIME(82800);
    
::

      sec_to_time(82800)
    =====================
      11:00:00 PM
     
.. code-block:: sql

    SELECT SEC_TO_TIME('82800.3');
    
::

      sec_to_time('82800.3')
    =========================
      11:00:00 PM
     
.. code-block:: sql

    SELECT SEC_TO_TIME(86399);
    
::

      sec_to_time(86399)
    =====================
      11:59:59 PM

SECOND
======

.. function:: SECOND (time)

    The **SECOND** function returns the seconds in the range of 0 to 59 from specified argument. You can specify the **TIME**, **TIMESTAMP** or **DATETIME**; the value is returned in **INTEGER** type.

    :param time: Time
    :rtype: INT

.. code-block:: sql

    SELECT SECOND('12:34:56');
    
::

       second('12:34:56')
    =====================
                       56
     
.. code-block:: sql

    SELECT SECOND('2010-01-01 12:34:56');
    
::

       second('2010-01-01 12:34:56')
    ================================
                                  56
     
.. code-block:: sql

    SELECT SECOND('2010-01-01 12:34:56.7890');

::
   
       second('2010-01-01 12:34:56.7890')
    =====================================
                                       56

SYS_DATE, SYSDATE
=================

.. c:macro:: SYS_DATE
.. c:macro:: SYSDATE

    **SYS_DATE** and **SYSDATE** are used interchangeably and they return the current date of server as the **DATE** type (*MM*/*DD*/*YYYY* or *YYYY*-*MM*-*DD*). The unit is day.  
    When the time zone of the current session is same as that of server, these functions are same as :func:`CURDATE`, :func:`CURRENT_DATE` and :c:macro:`CURRENT_DATE`. Please also refer :func:`CURDATE`, :func:`CURRENT_DATE` to find a difference and :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE` for details of the functions. 

    If input every argument value of year, month, and day is 0, the return value is determined by the **return_null_on_function_errors** system parameter; if it is set to yes, then **NULL** is returned; if it is set to no, an error is returned. The default value is **no**.

    :rtype: DATE
    
.. code-block:: sql

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'Asia/Seoul'        

    -- it returns the current date in DATE type
    
    SELECT CURDATE(), CURRENT_DATE(), CURRENT_DATE, SYS_DATE, SYSDATE;

       CURRENT_DATE    CURRENT_DATE    CURRENT_DATE    SYS_DATE    SYS_DATE 
    ========================================================================
      02/05/2016      02/05/2016      02/05/2016      02/05/2016  02/05/2016

.. code-block:: sql

    -- it returns the date 60 days added to the current date
    
    SELECT CURDATE()+60;

       CURRENT_DATE +60
    ===================
      04/05/2016    


.. code-block:: sql

    -- change session time from 'Asia/Seoul' to 'America/Los_Angeles'
    
    SET TIME ZONE 'America/Los_Angeles';

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'America/Los_Angeles'

    -- Note that CURDATE() and SYS_DATE returns different results
    
    SELECT CURDATE(), CURRENT_DATE(), CURRENT_DATE, SYS_DATE, SYSDATE;

       CURRENT_DATE    CURRENT_DATE    CURRENT_DATE    SYS_DATE    SYS_DATE 
    ========================================================================
      02/04/2016      02/04/2016      02/04/2016      02/05/2016  02/05/2016

.. warning::
    
    As 10.0, **SYS_DATE** and **SYSDATE** are different from **CURDATE** (), **CURRENT_DATE**, **CURRENT_DATE** (). They are synonym for 9.x and lower. 

SYS_DATETIME, SYSDATETIME
=========================

.. c:macro:: SYS_DATETIME
.. c:macro:: SYSDATETIME

    **SYS_DATETIME** and **SYSDATETIME** are used interchangeably, and they return the current date and time of server in **DATETIME** type. The unit is millisecond.
    When the time zone of the current session is same as that of server, these functions are same as :func:`CURRENT_DATETIME`, :c:macro:`CURRENT_DATETIME`, :func:`NOW`. Please also refer :func:`CURRENT_DATETIME`, :func:`NOW` to find a difference and :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE` for details of the functions.

    :rtype: DATETIME
    
.. code-block:: sql

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'Asia/Seoul'        

    -- it returns the current date and time in DATETIME type

    SELECT NOW(), SYS_DATETIME;                                                                                                                                                

       CURRENT_DATETIME               SYS_DATETIME                
    ==============================================================
      04:05:09.292 PM 02/05/2016     04:05:09.292 PM 02/05/2016   

.. code-block:: sql

    -- it returns the timestamp value 1 hour added to the current sys_datetime value
    
    SELECT TO_CHAR(SYSDATETIME+3600*1000, 'YYYY-MM-DD HH24:MI');

       to_char( SYS_DATETIME +3600*1000, 'YYYY-MM-DD HH24:MI')
    ======================
      '2016-02-05 17:05'  

.. code-block:: sql

    -- change session time from 'Asia/Seoul' to 'America/Los_Angeles'
    
    SET TIME ZONE 'America/Los_Angeles';

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'America/Los_Angeles'

    -- Note that NOW() and SYS_DATETIME return different results
    
    SELECT NOW(), SYS_DATETIME;

       CURRENT_DATETIME               SYS_DATETIME                
    ==============================================================
      11:08:57.041 PM 02/04/2016     04:08:57.041 PM 02/05/2016   
  
.. warning::

    As 10.0, **SYS_DATEIME**, **SYSDATETIME** are different from **CURRENT_DATETIME** (), **NOW** (). They are synonym for 9.x and lower. 
    
SYS_TIME, SYSTIME
=================

.. c:macro:: SYS_TIME
.. c:macro:: SYSTIME

    **SYS_TIME** and **SYSTIME** are used interchangeably and they return the current time of server as **TIME** type (*HH*:*MI*:*SS*). The unit is second.
    When the time zone of the current session is same as that of server, these functions are same as :func:`CURTIME`, :c:macro:`CURRENT_TIME`, :func:`CURRENT_TIME`. Please also refer :func:`CURTIME`, :func:`CURRENT_TIME` to find a difference and :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE` for details of the functions.
    
    :rtype: TIME
    
.. code-block:: sql

    select dbtimezone(), sessiontimezone();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'Asia/Seoul'        

    -- it returns the current time in TIME type
    
    SELECT CURTIME(), CURRENT_TIME(), CURRENT_TIME, SYS_TIME, SYSTIME;

       CURRENT_TIME    CURRENT_TIME    CURRENT_TIME    SYS_TIME     SYS_TIME  
    ==========================================================================
      04:22:54 PM     04:22:54 PM     04:22:54 PM     04:22:54 PM  04:22:54 PM


.. code-block:: sql

    -- change session time from 'Asia/Seoul' to 'America/Los_Angeles'
    
    SET TIME ZONE 'America/Los_Angeles';

    select dbtimezone(), sessiontimezone();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'America/Los_Angeles'

    -- Note that CURTIME() and SYS_TIME return different results
    
    SELECT CURTIME(), CURRENT_TIME(), CURRENT_TIME, SYS_TIME, SYSTIME;

       CURRENT_TIME    CURRENT_TIME    CURRENT_TIME    SYS_TIME     SYS_TIME  
    ==========================================================================
      11:23:16 PM     11:23:16 PM     11:23:16 PM     04:23:16 PM  04:23:16 PM

.. warning::

    As 10.0, **SYS_TIME**, **SYSTIME** are different from **CURTIME** (), **CURRENT_TIME** ().  They are synonym for 9.x and lower. 
    
SYS_TIMESTAMP, SYSTIMESTAMP
===========================

.. c:macro:: SYS_TIMESTAMP
.. c:macro:: SYSTIMESTAMP

    **SYS_TIMESTAMP** and **SYSTIMESTAMP** are used interchangeably and they return the current date and time of server as **TIMESTAMP** type. The unit is second.
    When the time zone of the current session is same as that of server, these functions are same as :c:macro:`CURRENT_TIMESTAMP`, :func:`CURRENT_TIMESTAMP`, :c:macro:`LOCALTIME`, :func:`LOCALTIME`, :c:macro:`LOCALTIMESTAMP`, :func:`LOCALTIMESTAMP`. Please also refer :c:macro:`CURRENT_TIMESTAMP`, :func:`CURRENT_TIMESTAMP`, :c:macro:`LOCALTIME`, :func:`LOCALTIME`, :c:macro:`LOCALTIMESTAMP`, :func:`LOCALTIMESTAMP` to find a difference and :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE` for details of the functions.
    
    :rtype: TIMESTAMP
    
.. code-block:: sql

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'Asia/Seoul'        

    -- it returns the current date and time in TIMESTAMP type of session and server timezones.
    
    SELECT LOCALTIME, SYS_TIMESTAMP;

       CURRENT_TIMESTAMP          SYS_TIMESTAMP           
    ======================================================
      04:34:16 PM 02/05/2016     04:34:16 PM 02/05/2016   

.. code-block:: sql
 
    -- change session time from 'Asia/Seoul' to 'America/Los_Angeles'
    
    SET TIME ZONE 'America/Los_Angeles';

    SELECT DBTIMEZONE(), SESSIONTIMEZONE();                                                                                                                                    

      dbtimezone            sessiontimezone     
    ============================================
      'Asia/Seoul'          'America/Los_Angeles'

    -- Note that LOCALTIME() and SYS_TIMESTAMP return different results
    
    SELECT LOCALTIME, SYS_TIMESTAMP;                                                                                                                                           

       CURRENT_TIMESTAMP          SYS_TIMESTAMP           
    ======================================================
      11:34:37 PM 02/04/2016     04:34:37 PM 02/05/2016   

.. warning::

    As 10.0, **SYS_TIMESTAMP** (), **SYSTIMESTAMP** are different from **CURRENT_TIMESTAMP**, **CURRENT_TIMESTAMP** (), **LOCALTIME**, **LOCALTIME** (), **LOCALTIMESTAMP** and **LOCALTIMESTAMP** ().  They are synonym for 9.x and lower. 

TIME
====

.. function:: TIME (time)

    The **TIME** function extracts the time part from specified argument and returns the **VARCHAR** type string in the 'HH:MI:SS' format. You can specify the **TIME**, **TIMESTAMP** and **DATETIME** types.

    :param time: Time
    :rtype: STRING

.. code-block:: sql

    SELECT TIME('12:34:56');

::
    
       time('12:34:56')
    ======================
      '12:34:56'
     
.. code-block:: sql

    SELECT TIME('2010-01-01 12:34:56');
    
::

       time('2010-01-01 12:34:56')
    ======================
      '12:34:56'
     
.. code-block:: sql

    SELECT TIME(datetime'2010-01-01 12:34:56');
    
::

       time(datetime '2010-01-01 12:34:56')
    ======================
      '12:34:56'

The following are examples of using timezone type values. For timezone related description, see :ref:`timezone-type`.

.. code-block:: sql

    SELECT TIME(datetimetz'1996-02-03 02:03:04 AM America/Lima PET');

::

    '02:03:04'
    
.. code-block:: sql

    SELECT TIME(datetimeltz'1996-02-03 02:03:04 AM America/Lima PET');

::

    '16:03:04'

.. code-block:: sql

    SELECT TIME(datetimeltz'2000-12-31 17:34:23.1234 -05:00');

::

    '07:34:23.123'

.. code-block:: sql

    SELECT TIME(datetimetz'2000-12-31 17:34:23.1234 -05:00');

::

    '17:34:23.123'

TIME_TO_SEC
===========

.. function:: TIME_TO_SEC (time)

    The **TIME_TO_SEC** function returns the seconds in the range of 0 to 86,399 from specified argument. You can specify the **TIME**, **TIMESTAMP** or **DATETIME** type; the value is returned in **INTEGER** type.

    :param time: Time
    :rtype: INT

.. code-block:: sql

    SELECT TIME_TO_SEC('23:00:00');
    
::

    82800
     
.. code-block:: sql

    SELECT TIME_TO_SEC('2010-10-04 23:00:00');
    
::

    82800
     
.. code-block:: sql

    SELECT TIME_TO_SEC('2010-10-04 23:00:00.1234');
     
::

    82800

The following are examples of using timezone type values. For timezone related description, see :ref:`timezone-type`.

.. code-block:: sql

    SELECT TIME_TO_SEC(datetimeltz'1996-02-03 02:03:04 AM America/Lima PET');

    57784

.. code-block:: sql

    SELECT TIME_TO_SEC(datetimetz'1996-02-03 02:03:04 AM America/Lima PET');

    7384

TIMEDIFF
========

.. function:: TIMEDIFF (expr1, expr2)

    The **TIMEDIFF** function returns the time difference between the two specified time arguments. You can enter a date/time type, the **TIME**, **DATE**, **TIMESTAMP** or **DATETIME** type and the data types of the two arguments must be identical. The **TIME** will be returned and the time difference between the two arguments must be in the range of 00:00:00 -23:59:59. If it exceeds the range, an error will be returned.

    :param expr1, expr2: Time. The data types of the two arguments must be identical.
    :rtype: TIME

.. code-block:: sql

    SELECT TIMEDIFF(time '17:18:19', time '12:05:52');
    
::

    05:12:27 AM
     
.. code-block:: sql

    SELECT TIMEDIFF('17:18:19','12:05:52');
    
::

    05:12:27 AM
     
.. code-block:: sql

    SELECT TIMEDIFF('2010-01-01 06:53:45', '2010-01-01 03:04:05');
    
::

    03:49:40 AM              

The following are examples of using timezone type values. For timezone related description, see :ref:`timezone-type`.

.. code-block:: sql

    SELECT TIMEDIFF (datetimeltz'2013-10-28 03:11:12 AM Asia/Seoul', datetimeltz'2013-10-27 04:11:12 AM Asia/Seoul');

::

    11:00:00 PM

TIMESTAMP
=========

.. function:: TIMESTAMP (date [,time])

    The **TIMESTAMP** function converts a **DATE** or **TIMESTAMP** type expression to **DATETIME** type.

    If the **DATE** format string ('*YYYY-MM-DD*' or '*MM/DD/YYYY*') or **TIMESTAMP** format string ('*YYYY-MM-DD HH:MI:SS*' or '*HH:MI:SS MM/DD/ YYYY*') is specified as the first argument, the function returns it as **DATETIME**.

    If the **TIME** format string ('*HH:MI:SS*.*FF*') is specified as the second, the function adds it to the first argument and returns the result as a **DATETIME** type. If the second argument is not specified, **12:00:00.000 AM** is specified by default.

    :param date: The format strings can be specified as follows: '*YYYY*-*MM*-*DD*', '*MM*/*DD*/*YYYY*', '*YYYY*-*MM*-*DD* *HH*:*MI*:*SS*.*FF*', '*HH*:*MI*:*SS*.*FF* *MM*/*DD*/*YYYY*'.
    :param time: The format string can be specified as follows: '*HH*:*MI*:*SS*[.*FF*]'.
    :rtype: DATETIME

.. code-block:: sql

    SELECT TIMESTAMP('2009-12-31'), TIMESTAMP('2009-12-31','12:00:00');
    
::

    12:00:00.000 AM 12/31/2009     12:00:00.000 PM 12/31/2009
     
.. code-block:: sql

    SELECT TIMESTAMP('2010-12-31 12:00:00','12:00:00');
    
::

    12:00:00.000 AM 01/01/2011
     
.. code-block:: sql

    SELECT TIMESTAMP('13:10:30 12/25/2008');
    
::

    01:10:30.000 PM 12/25/2008

The following are examples of using timezone type values. For timezone related description, see :ref:`timezone-type`.

.. code-block:: sql

    SELECT TIMESTAMP(datetimetz'2010-12-31 12:00:00 America/New_York', '03:00');

::

    03:00:00.000 PM 12/31/2010

.. code-block:: sql

    SELECT TIMESTAMP(datetimeltz'2010-12-31 12:00:00 America/New_York', '03:00');

::

    05:00:00.000 AM 01/01/2011

TO_DAYS
=======

.. function:: TO_DAYS (date)

    The **TO_DAYS** function returns the number of days after year 0 in the rage of 366 to 3652424 from specified argument. You can specify **DATE** type; the value is returned in **INTEGER** type.

    It is not recommended to use the **TO_DAYS** function for dates prior to the year 1582, as the function does not take dates prior to the introduction of the Gregorian Calendar into account.

    :param date: Date
    :rtype: INT

.. code-block:: sql

    SELECT TO_DAYS('2010-10-04');
    
::

       to_days('2010-10-04')
    ========================
                      734414
     
.. code-block:: sql

    SELECT TO_DAYS('2010-10-04 12:34:56');
    
::

       to_days('2010-10-04 12:34:56')
    ================================
                              734414
     
.. code-block:: sql

    SELECT TO_DAYS('2010-10-04 12:34:56.7890');
    
::

       to_days('2010-10-04 12:34:56.7890')
    ======================================
                                    734414
     
.. code-block:: sql

    SELECT TO_DAYS('1-1-1');
    
::

       to_days('1-1-1')
    ===================
                    366
     
.. code-block:: sql

    SELECT TO_DAYS('9999-12-31');
    
::

       to_days('9999-12-31')
    ========================
                     3652424

.. _trunc-date:

TRUNC
=====

.. function:: TRUNC( date[, fmt] )

    This function truncates date to the unit specified by the format string, *fmt*. It returns a value of DATE type.
    
    :param date: The value of **DATE**, **TIMESTAMP** or **DATETIME**
    :param fmt: Specifies the format for the truncating unit. If omitted, "dd" is default.
    :rtype: DATE

    The format and its unit and the return value are as follows:
    
    +-------------------+----------+-----------------------------------------------------------------------+
    | Format            | Unit     | Return value                                                          |
    +===================+==========+=======================================================================+
    | 'yyyy' or 'yy'    | year     | the same year with Jan. 1st                                           |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'mm' or 'month'   | month    | the same month with 1st                                               |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'q'               | quarter  | the same quarter with one of Jan. 1st, Apr. 1st, Jul. 1st, Oct. 1st   |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'day'             | week     | Sunday of the same week(starting date of the week including *date*)   |
    +-------------------+----------+-----------------------------------------------------------------------+
    | 'dd'              | day      | the same date with *date*                                             |
    +-------------------+----------+-----------------------------------------------------------------------+

.. code-block:: sql

    SELECT TRUNC(date'2012-12-26', 'yyyy');

::

    01/01/2012

.. code-block:: sql

    SELECT TRUNC(timestamp'2012-12-26 12:10:10', 'mm');

::

    12/01/2012
    
.. code-block:: sql

    SELECT TRUNC(datetime'2012-12-26 12:10:10', 'q');

::

    10/01/2012

.. code-block:: sql

    SELECT TRUNC(datetime'2012-12-26 12:10:10', 'dd');

::

    12/26/2012
    
.. code-block:: sql

    // It returns the date of Sunday of the week which includes date'2012-12-26'
    SELECT TRUNC(datetime'2012-12-26 12:10:10', 'day');

::

    12/23/2012
            
In order to round date instead of truncation, please see :ref:`ROUND(date, fmt) <round-date>`.

TZ_OFFSET
=========
      
.. function:: TZ_OFFSET(timezone_string)

    This returns a timezone offset from a timezone offset or timezone region name (e.g. '-05:00', or 'Europe/Vienna').
    
    :param timezone_string: timezone offset of timezone region name.
    :rtype: STRING

.. code-block:: sql

    SELECT TZ_OFFSET('+05:00');

::

      '+05:00'

.. code-block:: sql

    SELECT TZ_OFFSET('Asia/Seoul');

::

    '+09:00'

For timezone related description, see :ref:`timezone-type`.

.. seealso:: 

    :func:`DBTIMEZONE`, :func:`SESSIONTIMEZONE`, :func:`FROM_TZ`, :func:`NEW_TIME`

UNIX_TIMESTAMP
==============

.. function:: UNIX_TIMESTAMP ( [date] )

    The argument of the **UNIX_TIMESTAMP** function can be omitted. If it is omitted, the function returns the interval between '1970-01-01 00:00:00' UTC and the current system date/time in seconds as **INTEGER** type. If the date argument is specified, the function returns the interval between '1970-01-01 00:00:00' UTC and the specified date/time in seconds. 

    0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is given in every argument value corresponding to date and time, 0 is returned as an exception.

    Argument of DATETIME type is considered in session timezone.

    :param date: **DATE** type, **TIMESTAMP** type, **TIMESTAMPTZ** type, **TIMESTAMPLTZ** type, **DATETIME** type, **DATETIMETZ** type, **DATETIMELTZ** type, **DATE** format string ('*YYYY*-*MM*-*DD*' or '*MM*/*DD*/*YYYY*'), **TIMESTAMP** format string ('*YYYY*-*MM*-*DD* *HH*:*MI*:*SS*', '*HH*:*MI*:*SS* *MM*/*DD*/*YYYY*') or '*YYYYMMDD*' format string can be specified.
    :rtype: INT

.. code-block:: sql

    SELECT UNIX_TIMESTAMP('1970-01-02'), UNIX_TIMESTAMP();

::

       unix_timestamp('1970-01-02')   unix_timestamp()
    ==================================================
                              54000         1270196737
     
.. code-block:: sql

    SELECT UNIX_TIMESTAMP ('0000-00-00 00:00:00');

::

       unix_timestamp('0000-00-00 00:00:00')
    ========================================
                                           0
                                           

.. code-block:: sql

     -- when used without argument, it returns the exact value at the moment of execution of each occurence
     SELECT  UNIX_TIMESTAMP(), SLEEP(1), UNIX_TIMESTAMP();

       unix_timestamp()     sleep(1)   unix_timestamp()
    ===================================================
             1454661297            0         1454661298                                        

UTC_DATE
========

.. function:: UTC_DATE ()

    The **UTC_DATE** function returns the UTC date in 'YYYY-MM-DD' format.

    :rtype: STRING

.. code-block:: sql

    SELECT UTC_DATE();

::

      utc_date()
    ==============
      01/12/2011

UTC_TIME
========

.. function:: UTC_TIME ()

    The **UTC_TIME** function returns the UTC time in 'HH:MI:SS' format.

    :rtype: STRING

.. code-block:: sql

    SELECT UTC_TIME();
    
::

      utc_time()
    ==============
      10:35:52 AM

WEEK
====

.. function:: WEEK (date[, mode])

    The **WEEK** function returns the week in the range of 0 to 53 from specified argument. You can specify the **DATE**, **TIMESTAMP** or **DATETIME** type; the value is returned in **INTEGER** type.

    :param date: Date
    :param mode: Value in the range of 0 to 7
    :rtype: INT

You can omit the second argument, *mode* and must input a value in the range of 0 to 7. You can set that a week starts from Sunday or Monday and the range of the return value is from 0 to 53 or 1 to 53 with this value. If you omit the *mode*, the system parameter, **default_week_format** value(default: 0) will be used. The *mode* value means as follows:

+----------+---------------------------+-----------+-------------------------------------------------------------------+
| mode     | Start Day of the Week     | Range     | The First Week of the Year                                        |
+==========+===========================+===========+===================================================================+
| 0        | Sunday                    | 0~53      | The first week that Sunday is included in the year                |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 1        | Monday                    | 0~53      | The first week that more than three days are included in the year |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 2        | Sunday                    | 1~53      | The first week in the year that includes a Sunday                 |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 3        | Monday                    | 1~53      | The first week in the year that includes more than three days     |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 4        | Sunday                    | 0~53      | The first week in the year that includes more than three days     |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 5        | Monday                    | 0~53      | The first week in the year that includes Monday                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 6        | Sunday                    | 1~53      | The first week in the year that includes more than three days     |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 7        | Monday                    | 1~53      | The first week in the year that includes Monday                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+

If the *mode* value is one of 0, 1, 4 or 5, and the date corresponds to the last week of the previous year, the **WEEK** function will return 0. The purpose is to see what nth of the year the week is so it returns 0 for the 52th week of the year 1999.

.. code-block:: sql
    
    SELECT YEAR('2000-01-01'), WEEK('2000-01-01',0);
    
::

       year('2000-01-01')   week('2000-01-01', 0)
    =============================================
                    2000                       0

To see what n-th the week is based on the year including the start day of the week, use 0, 2, 5 or 7 as the *mode* value.

.. code-block:: sql

    SELECT WEEK('2000-01-01',2);
    
::

        week('2000-01-01', 2)
    ========================
                          52

.. code-block:: sql

    SELECT WEEK('2010-04-05');
    
::

       week('2010-04-05', 0)
    ========================
                          14
     
.. code-block:: sql

    SELECT WEEK('2010-04-05 12:34:56',2);
    
::

       week('2010-04-05 12:34:56',2)
    ===============================
                                  14
     
.. code-block:: sql

    SELECT WEEK('2010-04-05 12:34:56.7890',4);
    
::

       week('2010-04-05 12:34:56.7890',4)
    ====================================
                                      14

WEEKDAY
=======

.. function:: WEEKDAY (date)

    The **WEEKDAY** function returns the day of week in the range of 0 to 6 (0: Monday, 1: Tuesday, ..., 6: Sunday) from the specified parameter. You can specify **DATE**, **TIMESTAMP**, **DATETIME** types as parameters and an **INTEGER** type will be returned.

    :param date: Date
    :rtype: INT

.. code-block:: sql

    SELECT WEEKDAY('2010-09-09');
    
::

       weekday('2010-09-09')
    ========================
                           3
     
.. code-block:: sql

    SELECT WEEKDAY('2010-09-09 13:16:00');
    
::

       weekday('2010-09-09 13:16:00')
    =================================
                                    3

YEAR
====

.. function:: YEAR (date)

    The **YEAR** function returns the year in the range of 1 to 9,999 from the specified parameter. You can specify **DATE**, **TIMESTAMP** or **DATETIME** type; the value is returned in  **INTEGER** type.

    :param date: Date
    :rtype: INT

.. code-block:: sql

    SELECT YEAR('2010-10-04');
    
::

       year('2010-10-04')
    =====================
                     2010
     
.. code-block:: sql

    SELECT YEAR('2010-10-04 12:34:56');
    
::

       year('2010-10-04 12:34:56')
    ==============================
                              2010
     
.. code-block:: sql

    SELECT YEAR('2010-10-04 12:34:56.7890');
    
::

       year('2010-10-04 12:34:56.7890')
    ===================================
                                   2010
