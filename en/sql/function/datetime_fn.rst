***********************
날짜/시간 함수와 연산자
***********************


**Date/Time Functions and Operators**

**ADDDATE/DATE_ADD Functions**

**Description**

The
**ADDDATE**
function performs an addition or subtraction operation on a specific
**DATE**
value;
**ADDDATE**
and
**DATE_ADD**
are used interchangeably. The return value is a
**DATE**
or
**DATETIME**
type. The
**DATETIME**
type is returned in the following cases.

*   The first argument is a
    **DATETIME**
    or
    **TIMESTAMP**
    type



*   The first argument is a
    **DATE**
    type and the unit of
    **INTERVAL**
    value specified is less than the unit of day



Therefore, to return value of
**DATETIME**
type, you should convert the value of first argument by using the
**CAST**
function. Even though the date resulting from the operation exceeds the last day of the month, the function returns a valid
**DATE**
value considering the last date of the month.

If every input argument value of date and time is 0, the return value is determined by the
**return_null_on_function_errors**
system parameter; if it is set to yes, then
**NULL**
is returned; if it is set to no, an error is returned. The default value is
**no**
.

If the calculated value is between '0000-00-00 00:00:00' and '0001-01-01 00:00:00', a value having 0 for all arguments is returned in
**DATE**
or
**DATETIME**
type. Note that operation in JDBC program is determined by the configuration of zeroDateTimeBehavior, connection URL property (see "API Reference > JDBC API > JDBC Programming > Connection Configuration").

**Syntax**

**ADDDATE**
(
*date*
,
**INTERVAL**
*expr*
*unit*
)

**DATE_ADD**
(
*date*
,
**INTERVAL**
*expr*
*unit*
)

**ADDDATE**
(
*date*
,
*days*
)

*   *date*
    : It is a
    **DATE**
    ,
    **TIMETIME**
    , or
    **TIMESTAMP**
    expression that represents the start date. If an invalid
    **DATE**
    value such as '2006-07-00' is specified, an error is returned.



*   *expr*
    : It represents the interval value to be added to the start date. If a negative number is specified next to the
    **INTERVAL**
    keyword, the interval value is subtracted from the start date.



*   *unit*
    : It represents the unit of the interval value specified in the
    *expr*
    expression. See the following table to specify the format for the interpretation of the interval value. If the value of
    *expr*
    unit is less than the number requested in the
    *unit*
    , it is specified from the smallest unit. For example, if it is HOUR_SECOND, three values such as 'HOURS:MINUTES:SECONDS' are required. In the case, if only two values such as "1:1" are given, it is regarded as 'MINUTES:SECONDS'.



**Format of**
*expr*
**for**
*unit*

+--------------------+-------------------------------------------+--------------------------------------------------------------+
| **unit Value**     | **expr Format**                           | **Example**                                                  |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| MILLISECOND        | MILLISECONDS                              | ADDDATE(SYSDATE, INTERVAL 123 MILLISECOND)                   |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| SECOND             | SECONDS                                   | ADDDATE(SYSDATE, INTERVAL 123 SECOND)                        |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| MINUTE             | MINUTES                                   | ADDDATE(SYSDATE, INTERVAL 123 MINUTE)                        |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| HOUR               | HOURS                                     | ADDDATE(SYSDATE, INTERVAL 123 HOUR)                          |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| DAY                | DAYS                                      | ADDDATE(SYSDATE, INTERVAL 123 DAYS)                          |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| WEEK               | WEEKS                                     | ADDDATE(SYSDATE, INTERVAL 123 WEEKS)                         |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| MONTH              | MONTHS                                    | ADDDATE(SYSDATE, INTERVAL 12 MONTH)                          |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| QUARTER            | QUARTERS                                  | ADDDATE(SYSDATE, INTERVAL 12 QUARTER)                        |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| YEAR               | YEARS                                     | ADDDATE(SYSDATE, INTERVAL 12 YEAR)                           |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| SECOND_MILLISECOND | 'SECONDS.MILLISECONDS'                    | ADDDATE(SYSDATE, INTERVAL '12.123' SECOND_MILLISECOND)       |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| MINUTE_MILLISECOND | 'MINUTES:SECONDS.MILLISECONDS'            | ADDDATE(SYSDATE, INTERVAL '12:12.123' MINUTE_MILLISECOND)    |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| MINUTE_SECOND      | 'MINUTES:SECONDS'                         | ADDDATE(SYSDATE, INTERVAL '12:12' MINUTE_SECOND)             |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| HOUR_MILLISECOND   | 'HOURS:MINUTES:SECONDS.MILLISECONDS'      | ADDDATE(SYSDATE, INTERVAL '12:12:12.123' HOUR_MILLISECOND)   |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| HOUR_SECOND        | 'HOURS:MINUTES:SECONDS'                   | ADDDATE(SYSDATE, INTERVAL '12:12:12' HOUR_SECOND)            |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| HOUR_MINUTE        | 'HOURS:MINUTES'                           | ADDDATE(SYSDATE, INTERVAL '12:12' HOUR_MINUTE)               |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| DAY_MILLISECOND    | 'DAYS HOURS:MINUTES:SECONDS.MILLISECONDS' | ADDDATE(SYSDATE, INTERVAL '12 12:12:12.123' DAY_MILLISECOND) |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| DAY_SECOND         | 'DAYS HOURS:MINUTES:SECONDS'              | ADDDATE(SYSDATE,                                             |
|                    |                                           | INTERVAL '12 12:12:12' DAY_SECOND)                           |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| DAY_MINUTE         | 'DAYS HOURS:MINUTES'                      | ADDDATE(SYSDATE, INTERVAL '12 12:12' DAY_MINUTE)             |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| DAY_HOUR           | 'DAYS HOURS'                              | ADDDATE(SYSDATE, INTERVAL '12 12' DAY_HOUR)                  |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+
| YEAR_MONTH         | 'YEARS-MONTHS'                            | ADDDATE(SYSDATE, INTERVAL '12-13' YEAR_MONTH)                |
|                    |                                           |                                                              |
+--------------------+-------------------------------------------+--------------------------------------------------------------+

**Example**

SELECT SYSDATE, ADDDATE(SYSDATE,INTERVAL 24 HOUR), ADDDATE(SYSDATE, 1);

 

   SYS_DATE    date_add( SYS_DATE , INTERVAL 24 HOUR)   adddate( SYS_DATE , 1)

==============================================================================

  03/30/2010  12:00:00.000 AM 03/31/2010               03/31/2010

 

--it substracts days when argument < 0

SELECT SYSDATE, ADDDATE(SYSDATE,INTERVAL -24 HOUR), ADDDATE(SYSDATE, -1);

 

   SYS_DATE    date_add( SYS_DATE , INTERVAL -24 HOUR)   adddate( SYS_DATE , -1)

==============================================================================

  03/30/2010  12:00:00.000 AM 03/29/2010               03/29/2010

 

--when expr is not fully specified for unit

SELECT SYS_DATETIME, ADDDATE(SYS_DATETIME, INTERVAL '1:20' HOUR_SECOND);

 

   SYS_DATETIME                   date_add( SYS_DATETIME , INTERVAL '1:20' HOUR_SECOND)

=======================================================================================

  06:18:24.149 PM 06/28/2010     06:19:44.149 PM 06/28/2010                            

 

SELECT ADDDATE('0000-00-00', 1 );

 

ERROR: Conversion error in date format.

 

SELECT ADDDATE('0001-01-01 00:00:00', -1);

 

adddate('0001-01-01 00:00:00', -1)

======================

'12:00:00.000 AM 00/00/0000'

**ADDTIME Function**

**Description**

The
**ADDTIME**
function adds or subtracts a value of specific time.

The first argument is
**DATE**
,
**DATETIME**
,
**TIMESTAMP**
, or
**TIME**
type and the second argument is
**TIME**
,
**DATETIME**
, or
**TIMESTAMP**
type. Time should be include in the second argument, and the date of the second argument is ignored. The return type for each argument type is follows:

+-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+
| **First Argument Type** | **Second Argument Type**                 | **Return Type** | **Note**                                                 |
|                         |                                          |                 |                                                          |
+-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+
| TIME                    | TIME, DATETIME, TIMESTAMP                | TIME            | The result value must be equal to or less than 24 hours. |
|                         |                                          |                 |                                                          |
+-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+
| DATE                    | TIME, DATETIME, TIMESTAMP                | DATETIME        |                                                          |
|                         |                                          |                 |                                                          |
+-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+
| DATETIME                | TIME, DATETIME, TIMESTAMP                | DATETIME        |                                                          |
|                         |                                          |                 |                                                          |
+-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+
| date/time string        | TIME, DATETIME, TIMESTAMP or time string | VARCHAR         | The result string includes time.                         |
|                         |                                          |                 |                                                          |
+-------------------------+------------------------------------------+-----------------+----------------------------------------------------------+

**Syntax**

**ADDTIME**
(
*expr1*
,
*expr2*
)

*   *expr1*
    :
    **DATE**
    ,
    **DATETIME**
    ,
    **TIME**
    or
    **TIMESTAMP**
    type



*   *expr2*
    :
    **DATETIME**
    ,
    **TIMESTAMP**
    ,
    **TIME**
    type or date/time string



**Example**

SELECT ADDTIME(datetime'2007-12-31 23:59:59', time'1:1:2');

 addtime(datetime '2007-12-31 23:59:59', time '1:1:2')

========================================================

01:01:01.000 AM 01/01/2008

 

SELECT ADDTIME(time'01:00:00', time'02:00:01');

 addtime(time '01:00:00', time '02:00:01')

============================================

03:00:01 AM

**ADD_MONTHS Function**

The
**ADD_MONTHS**
function adds a
*month*
value to the expression
*date_argument*
of
**DATE**
type, and it returns a
**DATE**
type value. If the day (
*dd*
) of the value specified as an argument exists within the month of the result value of the operation, it returns the given day (
*dd*
); otherwise returns the last day of the given month (
*dd*
). If the result value of the operation exceeds the expression range of the
**DATE**
type, it returns an error.

**Syntax**

**ADD_MONTHS**
(
*date_argument*
,
*month*
)

 

*date_argument*
:

•
*date*

•
**NULL**

 

*month*
:

•
*integer*

•
**NULL**

*   *date_argument*
    : Specifies an expression of
    **DATE**
    type. To specify a
    **TIMESTAMP**
    or
    **DATETIME**
    value, an explicit casting to
    **DATE**
    type is required. If the value is
    **NULL**
    ,
    **NULL**
    is returned.



*   *month*
    : Specifies the number of the months to be added to the
    *date_argument*
    . Both positive and negative values can be specified. If the given value is not an integer type, conversion to an integer type by an implicit casting (rounding to the first place after the decimal point) is performed. If the value is
    **NULL**
    ,
    **NULL**
    is returned.



**Example**

--it returns DATE type value by adding month to the first argument

 

SELECT ADD_MONTHS(DATE '2008-12-25', 5), ADD_MONTHS(DATE '2008-12-25', -5);

  add_months(date '2008-12-25', 5)   add_months(date '2008-12-25', -5)

=======================================================================

  05/25/2009                         07/25/2008

 

 

SELECT ADD_MONTHS(DATE '2008-12-31', 5.5), ADD_MONTHS(DATE '2008-12-31', -5.5);

  add_months(date '2008-12-31', 5.5)   add_months(date '2008-12-31', -5.5)

===========================================================================

  06/30/2009                           06/30/2008

 

SELECT ADD_MONTHS(CAST (SYS_DATETIME AS DATE), 5), ADD_MONTHS(CAST (SYS_TIMESTAMP AS DATE), 5);

  add_months( cast( SYS_DATETIME  as date), 5)   add_months( cast( SYS_TIMESTAMP  as date), 5)

================================================================================

  07/03/2010                                     07/03/2010

**CURDATE/CURRENT_DATE/CURRENT_DATE()/SYS_DATE/SYSDATE**

**Description**

**CURDATE**
(),
**CURRENT_DATE**
,
**CURRENT_DATE**
,
**SYS_DATE**
, and
**SYSDATE**
are used interchangeably and they return the current date as the
**DATE**
type (
*MM*
/
*DD*
/
*YYYY*
or
*YYYY*
-
*MM*
-
*DD*
). The unit is day.

If input every argument value of year, month, and day is 0, the return value is determined by the
**return_null_on_function_errors**
system parameter; if it is set to yes, then
**NULL**
is returned; if it is set to no, an error is returned. The default value is
**no**
.

**Syntax**

**CURDATE**
()

**CURRENT_DATE**
()

**CURRENT_DATE**

**SYS_DATE**

**SYSDATE**

**Example**

--it returns the current date in DATE type

SELECT CURDATE(), CURRENT_DATE(), CURRENT_DATE, SYS_DATE, SYSDATE;

 

   SYS_DATE    SYS_DATE    SYS_DATE    SYS_DATE    SYS_DATE

============================================================

  04/01/2010  04/01/2010  04/01/2010  04/01/2010  04/01/2010

 

--it returns the date 60 days added to the current date

SELECT CURDATE()+60;

 

   SYS_DATE +60

===============

   05/31/2010

 

**CURRENT_DATETIME/CURRENT_DATETIME()/NOW()/SYS_DATETIME/SYSDATETIME**

**Description**

**CURRENT_DATETIME**
,
**CURRENT_DATETIME**
(),
**NOW**
() 
**SYS_DATETIME**
, and
**SYSDATETIME**
are used interchangeably, and they return the current date and time in
**DATETIME**
type. The unit is millisecond.

**Syntax**

**CURRENT_DATETIME**

**CURRENT_DATETIME**
()

**NOW**
()

**SYS_DATETIME**

**SYSDATETIME**

**Example**

--it returns the current date and time in DATETIME type

SELECT NOW(), SYS_DATETIME;

 

   SYS_DATETIME                   SYS_DATETIME

==============================================================

  04:08:09.829 PM 02/04/2010     04:08:09.829 PM 02/04/2010

 

--it returns the timestamp value 1 hour added to the current sys_datetime value

SELECT TO_CHAR(SYSDATETIME+3600*1000, 'YYYY-MM-DD HH:MI');

  to_char( SYS_DATETIME +3600*1000, 'YYYY-MM-DD HH:MI', 'en_US')

======================

  '2010-02-04 04:08'

**CURTIME()/CURRENT_TIME/CURRENT_TIME()/SYS_TIME/SYSTIME**

**Description**

**CURTIME**
(),
**CURRENT_TIME**
,
**CURRENT_TIME**
(),
**SYS_TIME**
, and
**SYSTIME**
are used interchangeably and they return the current time as
**TIME**
type (
*HH*
:
*MI*
:
*SS*
). The unit is second.

**Syntax**

**CURTIME**
()

**CURRENT_TIME**

**CURRENT_TIME**
()

**SYS_TIME**

**SYSTIME**

**Example**

--it returns the current time in TIME type

SELECT CURTIME(), CURRENT_TIME(), CURRENT_TIME, SYS_TIME, SYSTIME;

   SYS_TIME     SYS_TIME     SYS_TIME     SYS_TIME     SYS_TIME

=================================================================

  04:37:34 PM  04:37:34 PM  04:37:34 PM  04:37:34 PM  04:37:34 PM

 

--it returns the time value 1 hour added to the current sys_time

SELECT CURTIME()+3600;

   SYS_TIME +3600

=================

   05:37:34 PM

**CURRENT_TIMESTAMP/CURRENT_TIMESTAMP()/SYS_TIMESTAMP/SYSTIMESTAMP/LOCALTIME/LOCATIME()/LOCALTIMESTAMP/LOCALTIMESTAMP()**

**Description**

**CURRENT_TIMESTAMP**
,
**CURRENT_TIMESTAMP**
(),
**SYS_TIMESTAMP**
,
**SYSTIMESTAMP**
,
**LOCALTIME**
,
**LOCALTIME**
(),
**LOCALTIMESTAMP**
, and
**LOCALTIMESTAMP**
() are used interchangeably and they return the current date and time as
**TIMESTAMP**
type. The unit is second.

If you define
**DEFAULT**
value for column initial value and specify the initial value to
**SYS_DATETIME**
, the default value is specified to the timestamp at the time of creating a table, not inserting a table. Note that the default value is not specified in case of INSERT. Therefore, you must specify
**SYS_DATETIME**
in the
**VALUES**
of
**INSERT**
statement upon inserting data.

**Syntax**

**CURRENT_TIMESTAMP**

**CURRENT_TIMESTAMP**
()

**SYS_TIMESTAMP**

**SYSTIMESTAMP**

**LOCALTIME**

**LOCALTIME()**

**LOCALTIMESTAMP**

**LOCALTIMESTAMP()**

**Example**

--it returns the current date and time in TIMESTAMP type

SELECT LOCALTIME, SYS_TIMESTAMP;

 SYS_TIMESTAMP              SYS_TIMESTAMP

==============================================================================

  07:00:48 PM 04/01/2010     07:00:48 PM 04/01/2010

 

--it returns the timestamp value 1 hour added to the current sys_timestamp value

SELECT CURRENT_TIMESTAMP()+3600;

 SYS_TIMESTAMP +3600

===========================

  08:02:42 PM 04/01/2010

**DATE Function**

**Description**

The
**DATE**
function extracts the date part from specified argument, and returns it as
*MM*
/
*DD*
/
*YYYY*
' format string. Arguments that can be specified are
**DATE**
,
**TIMESTAMP**
and
**DATETIME**
types. The return value is a
**VARCHAR**
type.

0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to date and time, string where 0 is specified for year, month, and day is returned.

**Syntax**

**DATE**
(
*date*
)

*   *date*
    : The
    **DATE**
    ,
    **TIMESTAMP**
    or
    **DATETIME**
    can be specified.



**Example**

SELECT DATE('2010-02-27 15:10:23');

 date('2010-02-27 15:10:23')

==============================

  '02/27/2010'

 

SELECT DATE(NOW());

 date( SYS_DATETIME )

======================

  '04/01/2010'

 

SELECT DATE('0000-00-00 00:00:00');

 date('0000-00-00 00:00:00')

===============================

 '00/00/0000'

**DATEDIFF Function**

**Description**

The
**DATEDIFF**
function returns the difference between two arguments as an integer representing the number of days. Arguments that can be specified are
**DATE**
,
**TIMESTAMP**
and
**DATETIME**
types and it return value is only
**INTEGER**
type.

If every input argument value of date and time is 0, the return value is determined by the
**return_null_on_function_errors**
system parameter; if it is set to yes, then
**NULL**
is returned; if it is set to no, an error is returned. The default value is
**no**
.

**Syntax**

**DATEDIFF**
(
*date1*
,
*date2*
)

*   *date1*
    ,
    *date2*
    : Specifies the types that include date (
    **DATE**
    ,
    **TIMESTAMP**
    or
    **DATETIME**
    ) type or string that represents the value of corresponding type. If invalid string is specified, an error is returned.



**Example**

SELECT DATEDIFF('2010-2-28 23:59:59','2010-03-02');

 datediff('2010-2-28 23:59:59', '2010-03-02')

===============================================

                                             -2

 

SELECT DATEDIFF('0000-00-00 00:00:00', '2010-2-28 23:59:59');

ERROR: Conversion error in date format.

**DATE_SUB()/SUBDATE() Functions**

**Description**

The functions
**DATE_SUB**
and
**SUBDATE**
() are used interchangeably and they perform an addition or subtraction operation on a specific
**DATE**
value. The value is returned in
**DATE**
or
**DATETIME**
type. If the date resulting from the operation exceeds the last day of the month, the function returns a valid
**DATE**
value considering the last date of the month.

If every input argument value of date and time is 0, the return value is determined by the
**return_null_on_function_errors**
system parameter; if it is set to yes, then
**NULL**
is returned; if it is set to no, an error is returned. The default value is
**no**
.

If the calculated value is between '0000-00-00 00:00:00' and '0001-01-01 00:00:00', a value having 0 for all arguments is returned in
**DATE**
or
**DATETIME**
type. Note that operation in JDBC program is determined by the configuration of zeroDateTimeBehavior, connection URL property (see "API Reference > JDBC API > JDBC Programming > Connection Configuration").

**Syntax**

**DATE_SUB**
(
*date*
,
**INTERVAL**
*expr*
*unit*
)

**SUBDATE**
(
*date*
,
**INTERVAL**
*expr*
 
*unit*
)

**SUBDATE**
(
*date*
,
*days*
)

*   *date*
    : It is a
    **DATE**
    or
    **TIMESTAMP**
    expression that represents the start date. If an invalid
    **DATE**
    value such as '2006-07-00' is specified,
    **NULL**
    is returned.



*   *expr*
    : It represents the interval value to be subtracted from the start date. If a negative number is specified next to the
    **INTERVAL**
    keyword, the interval value is added to the start date.



*   *unit*
    : It represents the unit of the interval value specified in the
    *exp*
    expression. To check the expr argument for the unit value, see the table of
    `ADDDATE/DATE_ADD Functions <#syntax_syntax_operator_datefunc__9348>`_
    .



**Example**

SELECT SYSDATE, SUBDATE(SYSDATE,INTERVAL 24 HOUR), SUBDATE(SYSDATE, 1);

   SYS_DATE    date_sub( SYS_DATE , INTERVAL 24 HOUR)   subdate( SYS_DATE , 1)

==============================================================================

  03/30/2010  12:00:00.000 AM 03/29/2010               03/29/2010

 

--it adds days when argument < 0

SELECT SYSDATE, SUBDATE(SYSDATE,INTERVAL -24 HOUR), SUBDATE(SYSDATE, -1);

   SYS_DATE    date_sub( SYS_DATE , INTERVAL -24 HOUR)   subdate( SYS_DATE , -1)

==============================================================================

  03/30/2010  12:00:00.000 AM 03/31/2010               03/31/2010

 

SELECT SUBDATE('0000-00-00 00:00:00', -50);

ERROR: Conversion error in date format.

 

SELECT SUBDATE('0001-01-01 00:00:00', 10);

 subdate('0001-01-01 00:00:00', 10)

==============================

 '12:00:00.000 AM 00/00/0000'

**DAY/DAYOFMONTH Functions**

**Description**

The function
**DAY**
or
**DAYOFMONTH**
returns day in the range of 1 to 31 from the specified parameter. You can specify the 
**DATE**
,
**TIMESTAMP**
, or
**DATETIME**
type; the value is returned in
**INTEGER**
type.

0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to year, month, and day, 0 is returned as an exception.

**Syntax**

**DAY**
(
*date*
)

**DAYOFMONTH**
(
*date*
)

*   *date*
    : Date



**Example**

SELECT DAYOFMONTH('2010-09-09');

   dayofmonth('2010-09-09')

===========================

                          9

 

SELECT DAY('2010-09-09 19:49:29');

   day('2010-09-09 19:49:29')

=============================

                           9

 

SELECT DAYOFMONTH('0000-00-00 00:00:00');
   dayofmonth('0000-00-00 00:00:00')

====================================

                                   0

**DAYOFWEEK Function**

**Description**

The
**DAYOFWEEK**
function returns a day in the range of 1 to 7 (1: Sunday, 2: Monday, ..., 7: Saturday) from the specified parameters. The day index is same as the ODBC standards. You can specify the 
**DATE**
,
**TIMESTAMP**
, or
**DATETIME**
type; the value is returned in
**INTEGER**
type.

If every input argument value of year, month, and day is 0, the return value is determined by the
**return_null_on_function_errors**
system parameter; if it is set to yes, then
**NULL**
is returned; if it is set to no, an error is returned. The default value is
**no**
.

**Syntax**

**DAYOFWEEK**
(
*date*
)

*   *date*
    : Date



**Example**

SELECT DAYOFWEEK('2010-09-09');

   dayofweek('2010-09-09')

==========================

                         5

 

SELECT DAYOFWEEK('2010-09-09 19:49:29');

 dayofweek('2010-09-09 19:49:29')

=================================

                                5

SELECT DAYOFWEEK('0000-00-00');

ERROR: Conversion error in date format.

**DAYOFYEAR Function**

**Description**

The
**DAYOFYEAR**
function returns the day of a year in the range of 1 to 366. You can specify the 
**DATE**
,
**TIMESTAMP**
, or
**DATETIME**
types; the value is returned in
**INTEGER**
type.

If every input argument value of year, month, and day is 0, the return value is determined by the
**return_null_on_function_errors**
system parameter; if it is set to yes, then
**NULL**
is returned; if it is set to no, an error is returned. The default value is
**no**
.

**Syntax**

**DAYOFYEAR**
(
*date*
)

*   *date*
    : Date



**Example**

SELECT DAYOFYEAR('2010-09-09');

   dayofyear('2010-09-09')

==========================

                       252

 

SELECT DAYOFYEAR('2010-09-09 19:49:29');

dayofyear('2010-09-09 19:49:29')

=================================

                            252

 

 

SELECT DAYOFYEAR('0000-00-00');

ERROR: Conversion error in date format.

**EXTRACT Operator**

**Description**

The
**EXTRACT**
operator extracts the values from
*date-time_argument*
and then converts the value type into
**INTEGER**
.

0 is not allowed in the input argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to date and time, 0 is returned as an exception.

**Syntax**

**EXTRACT**
(
*field*
**FROM**
*date-time_argument*
)

 

*field*
:

•
**YEAR**

•
**MONTH**

•
**DAY**

•
**HOUR**

•
**MINUTE**

•
**SECOND**

•
**MILLISECOND**

 

*date-time_argument*
:

•
*expression*

*   *field*
    : Specifies a value to be extracted from date-time expression.



*   *date-time argument*
    : An expression that returns a value of date-time. This expression must be one of
    **TIME**
    ,
    **DATE**
    ,
    **TIMESTAMP**
    , or 
    **DATETIME**
    types. If the value is
    **NULL**
    ,
    **NULL**
    is returned.



**Example**

SELECT EXTRACT(MONTH FROM DATETIME '2008-12-25 10:30:20.123' );

  extract(month  from datetime '2008-12-25 10:30:20.123')

=========================================================

                                                       12

 

SELECT EXTRACT(HOUR FROM DATETIME '2008-12-25 10:30:20.123' );

 extract(hour  from datetime '2008-12-25 10:30:20.123')

=========================================================

                                                       10

 

SELECT EXTRACT(MILLISECOND FROM DATETIME '2008-12-25 10:30:20.123' );

 extract(millisecond  from datetime '2008-12-25 10:30:20.123')

=========================================================

                                                      123

 

SELECT EXTRACT(MONTH FROM '0000-00-00 00:00:00');

 extract(month from '0000-00-00 00:00:00')

==========================================

                                         0

**FROM_DAYS Function**

**Description**

The
**FROM_DAYS**
function returns a date value in
**DATE**
type if
**INTEGER**
type is inputted as an argument.

It is not recommended to use the
**FROM_DAYS**
function for dates prior to the year 1582 because the function does not take dates prior to the introduction of the Gregorian Calendar into account.

If a value in the range of 0 to 3,652,424 can be inputted as an argument. If a value in the range of 0 to 365 is inputted, 0 is returned. 3,652,424, which is the maximum value, means the last day of year 9999.

**Syntax**

**FROM_DAYS**
(
*N*
)

*   *N*
    : Integer in the range of 0 to 3,652,424



**Example**

SELECT FROM_DAYS(719528);

   from_days(719528)

====================

  01/01/1970

 

SELECT FROM_DAYS('366');

  from_days('366')

=================

  01/03/0001

 

SELECT FROM_DAYS(3652424);

   from_days(3652424)

=====================

  12/31/9999

 

SELECT FROM_DAYS(0);

   from_days(0)

===============

    00/00/0000

**FROM_UNIXTIME Function**

**Description**

The
**FROM_UNIXTIME**
function returns the date and time in the format of 'YYYY-MM-DD HH:MM:SS.' You can specify
**INTEGER**
type that corresponds to the UNIX timestamp; the value is returned in
**VARCHAR**
type and is displayed in the current time zone.

It displays the result according to the format that you specified, and the time
*format*
format follows the Date/Time Format 2 table of
`DATE_FORMAT Function <#syntax_syntax_operator_to_datefo_6449>`_
.

The relationship is not one of one-to-one correspondence between
**TIMESTAMP**
and UNIX timestamp so if you use
**UNIX_TIMESTAMP**
or 
**FROM_UNIXTIME**
function, partial value could be lost. For details, see
`UNIX_TIMESTAMP Function <#syntax_syntax_operator_datefunc__6995>`_
.

0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to date and time, string where 0 is specified for every date and time value is returned. Note that operation in JDBC program is determined by the configuration of zeroDateTimeBehavior, connection URL property (see "API Reference > JDBC API > JDBC Programming > Connection Configuration").

**Syntax**

**FROM_UNIXTIME**
(
*unix_timestamp*
[,
*format*
] )

*   *unix_timestamp*
    : Positive integer



*   *format*
    : Time format. Follows the date/time format of the
    `DATE_FORMAT Function <#syntax_syntax_operator_to_datefo_6449>`_
    .



**Example**

SELECT FROM_UNIXTIME(1234567890);

   from_unixtime(1234567890)

============================

  01:31:30 AM 02/14/2009

 

SELECT FROM_UNIXTIME('1000000000');

   from_unixtime('1000000000')

==============================

  04:46:40 AM 09/09/2001

 

SELECT FROM_UNIXTIME(1234567890,'%M %Y %W');

   from_unixtime(1234567890, '%M %Y %W')

======================

  'February 2009 Saturday'

 

SELECT FROM_UNIXTIME('1234567890','%M %Y %W');

   from_unixtime('1234567890', '%M %Y %W')

======================

  'February 2009 Saturday'

 

SELECT FROM_UNIXTIME(0);

   from_unixtime(0)

===========================

   12:00:00 AM 00/00/0000

**HOUR Function**

**Description**

The
**HOUR**
function extracts the hour from the specified parameter and then returns the value in integer. The type
**TIME**
,
**TIMESTAMP**
, or
**DATETIME**
can be specified and a value is returned in the
**INTEGER**
type.

**Syntax**

**HOUR**
(
*time*
)

*   *time*
    : Time



**Example**

SELECT HOUR('12:34:56');

   hour('12:34:56')

======================

                 12

 

SELECT HOUR('2010-01-01 12:34:56');

   hour('2010-01-01 12:34:56')

======================

                 12

 

SELECT HOUR(datetime'2010-01-01 12:34:56');

   time(datetime '2010-01-01 12:34:56')

======================

                 12

**LAST_DAY Function**

**Description**

The
**LAST_DAY**
function returns the last day of the given month as
**DATE**
type.

If every input argument value of year, month, and day is 0, the return value is determined by the
**return_null_on_function_errors**
system parameter; if it is set to yes, then
**NULL**
is returned; if it is set to no, an error is returned. The default value is
**no**
.

**Syntax**

**LAST_DAY**
(
*date_argument*
)

 

*date_argument*
:

• date

•
**NULL**

*   *date_argument*
    : Specifies an expression of
    **DATE**
    type. To specify a
    **TIMESTAMP**
    or
    **DATETIME**
    value, explicit casting to
    **DATE**
    is required. If the value is
    **NULL**
    ,
    **NULL**
    is returned.



**Example**

--it returns last day of the momth in DATE type

SELECT LAST_DAY(DATE '1980-02-01'), LAST_DAY(DATE '2010-02-01');

  last_day(date '1980-02-01')   last_day(date '2010-02-01')

============================================================

  02/28/1980                    02/28/2010

 

--it returns last day of the momth when explicitly casted to DATE type

SELECT LAST_DAY(CAST (SYS_TIMESTAMP AS DATE)), LAST_DAY(CAST (SYS_DATETIME AS DATE));

  last_day( cast( SYS_TIMESTAMP  as date))   last_day( cast( SYS_DATETIME  as date))

================================================================================

  02/28/2010                                 02/28/2010

 

SELECT LAST_DAY('0000-00-00');

ERROR: Conversion error in date format.

**MAKEDATE Function**

**Description**

The
**MAKEDATE**
function returns a date from the specified parameter. You can specify an
**INTEGER**
type corresponding to the day of the year in the range of 1 to 9999 as an argument; the value in the range of 1/1/1 to 12/31/9999 is returned in
**DATE**
type. If the day of the year has passed the corresponding year, it will become the next year. For example, MAKEDATE(1999, 366) will return 2000-01-01.

However, if you input a value in the range of 0 to 69 as the year, it will be processed as the year 2000-2069, if it is a value in the range of 70 to 99, it will be processed as the year 1970-1999.

If every value specified in
*year*
and
*dayofyear*
is 0, the return value is determined by the
**return_null_on_function_errors**
system parameter; if it is set to yes, then
**NULL**
is returned; if it is set to no, an error is returned. The default value is
**no**
.

**Syntax**

**MAKEDATE**
(
*year*
,
*dayofyear*
)

*   *year*
    : Year in the range of 1 to 9999



*   *dayofyear*
    : If you input a value in the range of 0 to 99 in the argument, it is handled as an exception;
    *dayofyear*
    must be equal to or less than 3,615,902 and the return value of MAKEDATE(100, 3615902) is 9999/12/31.



**Example**

SELECT MAKEDATE(2010,277);

   makedate(2010, 277)

======================

  10/04/2010

 

SELECT MAKEDATE(10,277);

   makedate(10, 277)

====================

  10/04/2010

 

SELECT MAKEDATE(70,277);

   makedate(70, 277)

====================

  10/04/1970

 

SELECT MAKEDATE(100,3615902);

   makedate(100, 3615902)

=========================

  12/31/9999

 

SELECT MAKEDATE(9999,365);

   makedate(9999, 365)

======================

  12/31/9999

 

SELECT MAKEDATE(0,0);

ERROR: Conversion error in date format.

**MAKETIME Function**

**Description**

The
**MAKETIME**
function returns the hour from specified argument in the AM/PM format. You can specify the
**INTEGER**
types corresponding hours, minutes and seconds as arguments; the value is returned in
**DATETIME**
.

**Syntax**

**MAKETIME**
(
*hour*
,
*min*
,
*sec*
)

*   *hour*
    : Integers representing the hours in the range of 0 to 23



*   *min*
    : Integers representing the minutes in the range of 0 to 59



*   *sec*
    : Integers representing the seconds in the range of 0 to 59



**Example**

SELECT MAKETIME(13,34,4);

   maketime(13, 34, 4)

======================

  01:34:04 PM

 

SELECT MAKETIME('1','34','4');

   maketime('1', '34', '4')

===========================

  01:34:04 AM

 

SELECT MAKETIME(24,0,0);

 

ERROR: Conversion error in time format.

**MINUTE Function**

**Description**

The
**MINUTE**
function returns the minutes in the range of 0 to 59 from specified argument. You can specify the
**TIME**
,
**TIMESTAMP**
, or
**DATETIME**
type; the value is returned in
**INTEGER**
type.

**Syntax**

**MINUTE**
(
*time*
)

*   *time*
    : Time



**Example**

SELECT MINUTE('12:34:56');

   minute('12:34:56')

=====================

                   34

 

SELECT MINUTE('2010-01-01 12:34:56');

   minute('2010-01-01 12:34:56')

================================

                              34

 

SELECT MINUTE('2010-01-01 12:34:56.7890');

   minute('2010-01-01 12:34:56.7890')

=====================================

                                   34

**MONTH Function**

**Description**

The
**MONTH**
function returns the month in the range of 1 to 12 from specified argument. You can specify the
**DATE**
,
**TIMESTAMP**
, or
**DATETIME**
type; the value is returned in
**INTEGER**
type.

0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to date, 0 is returned as an exception.

**Syntax**

**MONTH**
(
*date*
)

*   *date*
    : Date



**Example**

SELECT MONTH('2010-01-02');

   month('2010-01-02')

======================

                     1

 

SELECT MONTH('2010-01-02 12:34:56');

   month('2010-01-02 12:34:56')

===============================

                              1

 

SELECT MONTH('2010-01-02 12:34:56.7890');

   month('2010-01-02 12:34:56.7890')

====================================

                                   1

 

SELECT MONTH('0000-00-00');

   month('0000-00-00')

======================

                     0

**MONTHS_BETWEEN Function**

**Description**

The
**MONTHS_BETWEEN**
function returns the difference between the given
**DATE**
value. The return value is
**DOUBLE**
type. An integer value is returned if the two dates specified as arguments are identical or are the last day of the given month; otherwise, a value obtained by dividing the day difference by 31 is returned.

**Syntax**

**MONTHS_BETWEEN**
(
*date_argument*
,
*date_argument*
)

 

*date_argument*
:

•
*date*

•
**NULL**

*   *date_argument*
    : Specifies an expression of
    **DATE**
    type. To specify a
    **TIMESTAMP**
    or
    **DATETIME**
    value, explicit casting to
    **DATE**
    is required. If the value is
    **NULL**
    ,
    **NULL**
    is returned.



**Example**

--it returns the negative months when the first argument is the previous date

SELECT MONTHS_BETWEEN(DATE '2008-12-31', DATE '2010-6-30');

 months_between(date '2008-12-31', date '2010-6-30')

======================================================

                               -1.800000000000000e+001

 

--it returns integer values when each date is the last dat of the month

SELECT MONTHS_BETWEEN(DATE '2010-6-30', DATE '2008-12-31');

 months_between(date '2010-6-30', date '2008-12-31')

======================================================

                                1.800000000000000e+001

 

--it returns months between two arguments when explicitly casted to DATE type

SELECT MONTHS_BETWEEN(CAST (SYS_TIMESTAMP AS DATE), DATE '2008-12-25');

 months_between( cast( SYS_TIMESTAMP  as date), date '2008-12-25')

====================================================================

                                              1.332258064516129e+001

 

--it returns months between two arguments when explicitly casted to DATE type

SELECT MONTHS_BETWEEN(CAST (SYS_DATETIME AS DATE), DATE '2008-12-25');

 months_between( cast( SYS_DATETIME  as date), date '2008-12-25')

===================================================================

                                             1.332258064516129e+001

**QUARTER Function**

**Description**

The
**QUARTER**
function returns the quarter in the range of 1 to 4 from specified argument. You can specify the
**DATE**
,
**TIMESTAMP**
, or
**DATETIME**
type; the value is returned in
**INTEGER**
type.

**Syntax**

**QUARTER**
(
*date*
)

*   *date*
    : Date



**Example**

SELECT QUARTER('2010-05-05');

   quarter('2010-05-05')

========================

                       2

 

SELECT QUARTER('2010-05-05 12:34:56');

  quarter('2010-05-05 12:34:56')

===============================

                              2

 

SELECT QUARTER('2010-05-05 12:34:56.7890');

  quarter('2010-05-05 12:34:56.7890')

==================================

                              2

**SEC_TO_TIME Function**

**Description**

The
**SEC_TO_TIME**
function returns the time including hours, minutes and seconds from specified argument. You can specify the
**INTEGER**
type in the range of 0 to 86,399; the value is returned in
**TIME**
type.

**Syntax**

**SEC_TO_TIME**
(
*second*
)

*   *second*
    : Seconds in the range of 0 to 86,399



**Example**

SELECT SEC_TO_TIME(82800);

   sec_to_time(82800)

=====================

  11:00:00 PM

 

SELECT SEC_TO_TIME('82800.3');

   sec_to_time('82800.3')

=========================

  11:00:00 PM

 

SELECT SEC_TO_TIME(86399)

   sec_to_time(86399)

=====================

  11:59:59 PM

**SECOND Function**

**Description**

The
**SECOND**
function returns the seconds in the range of 0 to 59 from specified argument. You can specify the
**TIME**
,
**TIMESTAMP**
, or
**DATETIME**
; the value is returned in
**INTEGER**
type.

**Syntax**

**SECOND**
(
*time*
)

*   *time*
    : Time



**Example**

SELECT SECOND('12:34:56');

   second('12:34:56')

=====================

                   56

 

SELECT SECOND('2010-01-01 12:34:56');

   second('2010-01-01 12:34:56')

================================

                              56

 

SELECT SECOND('2010-01-01 12:34:56.7890');

   second('2010-01-01 12:34:56.7890')

=====================================

                                   56

 

**TIME Function**

**Description**

The
**TIME**
function extracts the time part from specified argument and returns the
**VARCHAR**
type string in the 'HH:MM:SS' format. You can specify the
**TIME**
,
**TIMESTAMP**
, and
**DATETIME**
types.

**Syntax**

**TIME**
(
*time*
)

*   *time*
    : Time



**Example**

SELECT TIME('12:34:56');

   time('12:34:56')

======================

  '12:34:56'

 

SELECT TIME('2010-01-01 12:34:56');

   time('2010-01-01 12:34:56')

======================

  '12:34:56'

 

SELECT TIME(datetime'2010-01-01 12:34:56');

   time(datetime '2010-01-01 12:34:56')

======================

  '12:34:56'

**TIME_TO_SEC Function**

**Description**

The
**TIME_TO_SEC**
function returns the seconds in the range of 0 to 86,399 from specified argument. You can specify the
**TIME**
,
**TIMESTAMP**
, or
**DATETIME**
type; the value is returned in
**INTEGER**
type.

**Syntax**

**TIME_TO_SEC**
(
*time*
)

*   *time*
    : Time



**Example**

SELECT TIME_TO_SEC('23:00:00');

   time_to_sec('23:00:00')

==========================

                     82800

 

SELECT TIME_TO_SEC('2010-10-04 23:00:00');

   time_to_sec('2010-10-04 23:00:00')

=====================================

                                82800

 

 SELECT TIME_TO_SEC('2010-10-04 23:00:00.1234');

   time_to_sec('2010-10-04 23:00:00.1234')

==========================================

                                     82800

 

**TIMEDIFF Function**

**Description**

The
**TIMEDIFF**
function returns the time difference between the two specified time arguments.

You can enter a date/time type, the
**TIME**
,
**DATE**
,
**TIMESTAMP**
, or
**DATETIME**
type and the data types of the two arguments must be identical. The
**TIME**
will be returned and the time difference between the two arguments must be in the range of 00:00:00 -23:59:59. If it exceeds the range, an error will be returned.

**Syntax**

**TIMEDIFF**
(
*expr1*
,
*expr2*
)

*   *expr1*
    ,
    *expr2*
    : Time. The data types of the two arguments must be identical.



**Example**

SELECT TIMEDIFF(time '17:18:19', time '12:05:52');

   timediff(time '17:18:19', time '12:05:52')

=============================================

  05:12:27 AM

 

SELECT TIMEDIFF('17:18:19','12:05:52');

   timediff('17:18:19', '12:05:52')

===================================

  05:12:27 AM

 

SELECT TIMEDIFF('2010-01-01 06:53:45', '2010-01-01 03:04:05');

   timediff('2010-01-01 06:53:45', '2010-01-01 03:04:05')

=========================================================

  03:49:40 AM              

 

**TIMESTAMP Function**

**Description**

The
**TIMESTAMP**
function converts a
**DATE**
or
**TIMESTAMP**
type expression to
**DATETIME**
type.

If the
**DATE**
format string ('
*YYYY-MM-DD*
' or '
*MM/DD/YYYY*
') or
**TIMESTAMP**
format string ('
*YYYY-MM-DD HH:MI:SS*
' or '
*HH:MI:SS MM/DD/ YYYY*
') is specified as the first argument, the function returns it as
**DATETIME**
.

If the
**TIME**
format string ('
*HH:MI:SS*
') is specified as the second, the function adds it to the first argument and returns the result as a
**DATETIME**
type. If the second argument is not specified,
**12:00:00.000 AM**
is specified by default.

**Syntax**

**TIMESTAMP**
(
*date*
[,
*time*
])

*   *date*
    : The format strings can be specified as follows: '
    *YYYY-MM-DD*
    ', '
    *MM/DD/YYYY*
    ', '
    *YYYY-MM-DD HH:MI:SS*
    ', or '
    *HH:MI:SS*
    *MM/DD/YYYY*
    '



*   *time*
    : The format string can be specified as follows: '
    *HH*
    :
    *MI*
    :
    *SS*
    '



**Example**

SELECT TIMESTAMP('2009-12-31'), TIMESTAMP('2009-12-31','12:00:00');

 timestamp('2009-12-31')        timestamp('2009-12-31', '12:00:00')

=====================================================================

  12:00:00.000 AM 12/31/2009     12:00:00.000 PM 12/31/2009

 

SELECT TIMESTAMP('2010-12-31 12:00:00','12:00:00');

 timestamp('2010-12-31 12:00:00', '12:00:00')

===============================================

  12:00:00.000 AM 01/01/2011

 

SELECT TIMESTAMP('13:10:30 12/25/2008');

 timestamp('13:10:30 12/25/2008')

===================================

  01:10:30.000 PM 12/25/2008

**TO_DAYS Function**

**Description**

The
**TO_DAYS**
function returns the number of days after year 0 in the rage of 366 to 3652424 from specified argument. You can specify 
**DATE**
type; the value is returned in
**INTEGER**
type.

It is not recommended to use the
**TO_DAYS**
function for dates prior to the year 1582, as the function does not take dates prior to the introduction of the Gregorian Calendar into account.

**Syntax**

**TO_DAYS**
(
*date*
)

*   *date*
    : Date



**Example**

SELECT TO_DAYS('2010-10-04');

   to_days('2010-10-04')

========================

                  734414

 

SELECT TO_DAYS('2010-10-04 12:34:56');

   to_days('2010-10-04 12:34:56')

================================

                          734414

 

SELECT TO_DAYS('2010-10-04 12:34:56.7890');

   to_days('2010-10-04 12:34:56.7890')

======================================

                                734414

 

SELECT TO_DAYS('1-1-1');

   to_days('1-1-1')

===================

                366

 

SELECT TO_DAYS('9999-12-31');

   to_days('9999-12-31')

========================

                 3652424

 

**UNIX_TIMESTAMP Function**

**Description**

The arguments of the
**UNIX_TIMESTAMP**
function can be omitted. If they are omitted, the function returns the interval between '1970-01-01 00:00:00' UTC and the current system date/time in seconds as
**INTEGER**
type. If the date argument is specified, the function returns the interval between '1970-01-01 00:00:00' UTC and the specified date/time in seconds.

0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to date and time, 0 is returned as an exception.

**Syntax**

**UNIX_TIMESTAMP**
( [
*date*
] )

*   *date*
    :
    **DATE**
    type or
    **TIMESTAMP**
    type,
    **DATE**
    format string ('
    *YYYY*
    -
    *MM*
    -
    *DD*
    ' or '
    *MM*
    /
    *DD*
    /
    *YYYY*
    '),
    **TIMESTAMP**
    format string ('
    *YYYY*
    -
    *MM*
    -
    *DD*
    *HH*
    :
    *MI*
    :
    *SS*
    ' or '
    *HH*
    :
    *MI*
    :
    *SS*
    *MM*
    /
    *DD*
    /
    *YYYY*
    ') or '
    *YYYYMMDD*
    ' string can be specified.



**Example**

SELECT UNIX_TIMESTAMP('1970-01-02'), UNIX_TIMESTAMP();

   unix_timestamp('1970-01-02')   unix_timestamp()

==================================================

                          54000         1270196737

 

SELECT UNIX_TIMESTAMP ('0000-00-00 00:00:00');

   unix_timestamp('0000-00-00 00:00:00')

========================================

                                       0

**UTC_DATE Function**

**Description**

The
**UTC_DATE**
function returns the UTC date in 'YYYY-MM-DD' format.

**Syntax**

**UTC_DATE**
()

**Example**

SELECT UTC_DATE();

  utc_date()

==============

  01/12/2011

**UTC_TIME Function**

**Description**

The
**UTC_TIME**
function returns the UTC time in 'HH:MM:SS' format.

**Syntax**

**UTC_TIME**
()

**Example**

SELECT UTC_TIME();

  utc_time()

==============

  10:35:52 AM

**WEEK Function**

**Description**

The
**WEEK**
function returns the week in the range of 0 to 53 from specified argument. You can specify the
**DATE**
,
**TIMESTAMP**
, or
**DATETIME**
type; the value is returned in
**INTEGER**
type.

You can omit the second argument,
*mode*
and must input a value in the range of 0 to 7. You can set that a week starts from Sunday or Monday and the range of the return value is from 0 to 53 or 1 to 53 with this value. If you omit the
*mode*
, the system parameter,
**default_week_format**
value(default: 0) will be used. The
*mode*
value means as follows:

+----------+---------------------------+-----------+-------------------------------------------------------------------+
| **mode** | **Start Day of the Week** | **Range** | **The First Week of the Year**                                    |
|          |                           |           |                                                                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 0        | Sunday                    | 0~53      | The first week that Sunday is included in the year                |
|          |                           |           |                                                                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 1        | Monday                    | 0~53      | The first week that more than three days are included in the year |
|          |                           |           |                                                                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 2        | Sunday                    | 1~53      | The first week in the year that includes a Sunday                 |
|          |                           |           |                                                                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 3        | Monday                    | 1~53      | The first week in the year that includes more than three days     |
|          |                           |           |                                                                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 4        | Sunday                    | 0~53      | The first week in the year that includes more than three days     |
|          |                           |           |                                                                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 5        | Monday                    | 0~53      | The first week in the year that includes Monday                   |
|          |                           |           |                                                                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 6        | Sunday                    | 1~53      | The first week in the year that includes more than three days     |
|          |                           |           |                                                                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+
| 7        | Monday                    | 1~53      | The first week in the year that includes Monday                   |
|          |                           |           |                                                                   |
+----------+---------------------------+-----------+-------------------------------------------------------------------+

If the
*mode*
value is one of 0, 1, 4 or 5, and the date corresponds to the last week of the previous year, the
**WEEK**
function will return 0. The purpose is to see what nth of the year the week is so it returns 0 for the 52th week of the year 1999.

SELECT YEAR('2000-01-01'), WEEK('2000-01-01',0);

   year('2000-01-01')   week('2000-01-01', 0)

=============================================

                2000                       0

To see what n-th the week is based on the year including the start day of the week, use 0, 2, 5 or 7 as the
*mode*
value.

SELECT WEEK('2000-01-01',2);

    week('2000-01-01', 2)

========================

                      52

**Syntax**

**WEEK**
(
*date*
[,
*mode*
])

*   *date*
    : Date



*   *mode*
    : Value in the range of 0 to 7



**Example**

SELECT WEEK('2010-04-05');

   week('2010-04-05', 0)

========================

                      14

 

SELECT WEEK('2010-04-05 12:34:56',2);

   week('2010-04-05 12:34:56',2)

===============================

                              14

 

SELECT WEEK('2010-04-05 12:34:56.7890',4);

   week('2010-04-05 12:34:56.7890',4)

====================================

                                  14               

 

**WEEKDAY Function**

**Description**

The
**WEEKDAY**
function returns the day of week in the range of 0 to 6 (0: Monday, 1: Tuesday, ..., 6: Sunday) from the specified parameter. You can specify
**DATE**
,
**TIMESTAMP**
,
**DATETIME**
types as parameters and an
**INTEGER**
type will be returned.

**Syntax**

**WEEKDAY**
(
*date*
)

*   *date*
    : Date



**Example**

SELECT WEEKDAY('2010-09-09');

   weekday('2010-09-09')

========================

                       3

 

SELECT WEEKDAY('2010-09-09 13:16:00');

   weekday('2010-09-09 13:16:00')

=================================

                                3

 

**YEAR Function**

**Description**

The
**YEAR**
function returns the year in the range of 1 to 9,999 from the specified parameter. You can specify
**DATE**
,
**TIMESTAMP**
, or
**DATETIME**
type; the value is returned in 
**INTEGER**
type.

**Syntax**

**YEAR**
(
*date*
)

*   *date*
    : Date



**Example**

SELECT YEAR('2010-10-04');

   year('2010-10-04')

=====================

                 2010

 

SELECT YEAR('2010-10-04 12:34:56');

   year('2010-10-04 12:34:56')

==============================

                          2010

 

SELECT YEAR('2010-10-04 12:34:56.7890');

   year('2010-10-04 12:34:56.7890')

===================================

                               2010

 
