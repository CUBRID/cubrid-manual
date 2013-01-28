**********
Data Types
**********

Numeric Types
=============

CUBRID supports the following numeric data types to store integers or real numbers.

**Numeric Types Supported by CUBRID**

+------------------+-----------+---------------------------------------------------------+---------------------------------------------------------+---------------------+
| Type             | Bytes     | Mix                                                     | Max                                                     | Exact/approx.       |
+==================+===========+=========================================================+=========================================================+=====================+
| SHORT            | 2         | -32,768                                                 | 32,767                                                  | exact numeric       |
| SMALLINT         |           |                                                         |                                                         |                     |
+------------------+-----------+---------------------------------------------------------+---------------------------------------------------------+---------------------+
| INTEGER          | 4         | -2,147,483,648                                          | +2,147,483,647                                          | exact numeric       |
| INT              |           |                                                         |                                                         |                     |
+------------------+-----------+---------------------------------------------------------+---------------------------------------------------------+---------------------+
| BIGINT           | 8         | -9,223,372,036,854,775,808                              | +9,223,372,036,854,775,807                              | exact numeric       |
+------------------+-----------+---------------------------------------------------------+---------------------------------------------------------+---------------------+
| NUMERIC          | 16        | precision                                               | precision                                               | exact numeric       |
| DECIMAL          |           | *p*                                                     | *p*                                                     |                     |
|                  |           | : 1                                                     | : 38                                                    |                     |
|                  |           | scale                                                   | scale                                                   |                     |
|                  |           | *s*                                                     | *s*                                                     |                     |
|                  |           | : 0                                                     | : 38                                                    |                     |
+------------------+-----------+---------------------------------------------------------+---------------------------------------------------------+---------------------+
| FLOAT            | 4         | -3.402823466E+38 (ANSI/IEEE 754-1985 standard)          | +3.402823466E+38                                        | approximate numeric |
| REAL             |           |                                                         | (ANSI/IEEE 754-1985 standard)                           | floating point : 7  |
+------------------+-----------+---------------------------------------------------------+---------------------------------------------------------+---------------------+
| DOUBLE           | 8         | -1.7976931348623157E+308 ANSI/IEEE 754-1985 standard)   | +1.7976931348623157E+308(ANSI/IEEE 754-1985 standard)   | approximate numeric |
| DOUBLE PRECISION |           |                                                         |                                                         | floating point : 15 |
+------------------+-----------+---------------------------------------------------------+---------------------------------------------------------+---------------------+
| MONETARY         | 12        | -3.402823466E+38                                        | +3.402823466E+38                                        | approximate numeric |
+------------------+-----------+---------------------------------------------------------+---------------------------------------------------------+---------------------+

Numeric data types are divided into exact and approximate types. Exact numeric data types (**SMALLINT**, **INT**, **BIGINT**, **NUMERIC**) are used for numbers whose values must be precise and consistent, such as the numbers used in financial accounting. Note that even when the literal values are equal, approximate numeric data types (**FLOAT**, **DOUBLE**, **MONETARY**) can be interpreted differently depending on the system.

CUBRID does not support the UNSIGNED type for numeric data types.

On the above table, two types on the same cell are identical types but it always prints the above type name when you execute **SHOW COLUMNS** statement. For example, you can use both **SHORT** and **SMALLINT** when you create a table, but it prints "SHORT" when you execute **SHOW COLUMNS** statement.

**Precision and Scale**

The precision of numeric data types is defined as the number of significant figures. This applies to both exact and approximate numeric data types.

The scale represents the number of digits following the decimal point. It is significant only in exact numeric data types. Attributes declared as exact numeric data types always have fixed precision and scale. **NUMERIC** (or **DECIMAL**) data type always has at least one-digit precision, and the scale should be between 0 and the precision declared. Scale
cannot be greater than precision. For **INTEGER**, **SMALLINT**, or **BIGINT** data types, the scale is 0 (i.e. no digits following the decimal point), and the precision is fixed by the system.

**Numeric Literals**

Special signs can be used to input numeric values. The plus sign (+) and minus sign (-) are used to represent positive and negative numbers respectively. You can also use scientific notations. In addition, you can use currency signs specified in the system to represent currency values. The maximum precision that can be expressed by a numeric literal is 255.

**Numeric Coercions**

All numeric data type values can be compared with each other. To do this, automatic coercion to the common numeric data type is performed. For explicit coercion, use the **CAST** operator. When different data types are sorted or calculated in a numerical expression, the system performs automatic coercion. For example, when adding a **FLOAT** attribute value to an **INTEGER** attribute value, the system automatically coerces the **INTEGER** value to the most approximate **FLOAT** value before it performs the addition operation.

.. warning:: 

	Earlier version than CUBRID 2008 R2.0, the input constant value exceeds **INTEGER**, it is handled as **NUMERIC**. However, 2008 R2.0 or later versions, it is handled as **BIGINT** .

INT/INTEGER
-----------

The **INTEGER** data type is used to represent integers. The value range is available is from -2,147,483,648 to +2,147,483,647. **SMALLINT** is used for small integers, and **BIGINT** is used for big integers.

*   If a real number is entered for an **INT** type, the number is rounded to zero decimal place and the integer value is stored.
*   **INTEGER** and **INT** are used interchangeably.

**Example** ::

	If you specify 8934 as INTEGER, 8934 is stored.
	If you specify 7823467 as INTEGER, 7823467 is stored.
	If you specify 89.8 to an INTEGER, 90 is stored (all digits after the decimal point are rounded).
	If you specify 3458901122 as INTEGER, an error occurs (if the allowable limit is exceeded).

SHORT/SMALLINT
--------------

The **SMALLINT** data type is used to represent a small integer type. The value range is available is from -32,768 to +32,767.

*   If a real number is entered for an **SMALLINT** type, the number is rounded to zero decimal place and the integer value is stored.
*   **SMALLINT** and **SHORT** are used interchangeably.

**Example** ::

	If you specify 8934 as SMALLINT, 8934 is stored.
	If you specify 34.5 as SMALLINT, 35 is stored (all digits after the decimal point are rounded).
	If you specify 23467 as SMALLINT, 23467 is stored.
	If you specify 89354 as SMALLINT, an error occurs (if the allowable limit is exceeded).

BIGINT
------

The **BIGINT** data type is used to represent big integers. The value range is available from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807.

* If a real number is entered for a **BIG** type, the number is rounded to zero decimal place and the integer value is stored.
* Based on the precision and the range of representation, the following order applies.

  **SMALLINT** ⊂ **INTEGER** ⊂ **BIGINT** ⊂ **NUMERIC** 

**Example** ::

	If you specify 8934 as BIGINT, 8934 is stored.
	If you specify 89.1 as BIGINT, 89 is stored.
	If you specify 89.8 as BIGINT, 90 is stored (all digits after the decimal point are rounded).
	If you specify 3458901122 as BIGINT, 3458901122 is stored.

NUMERIC/DECIMAL
---------------

**NUMERIC** or **DECIMAL** data types are used to represent fixed-point numbers. As an option, the total number of digits (precision) and the number of digits after the decimal point (scale) can be specified for definition. The minimum value for the precision *p* is 1. When the precision *p* is omitted, you cannot enter data whose integer part exceeds 15 digits because the default value is 15. If the scale *s* is omitted, an integer rounded to the first digit after the decimal point is returned because the default value is 0. ::

	NUMERIC [(p[, s])]
	
*   Precision must be equal to or greater than scale.
*   Precision must be equal to or greater than the number of integer digits + scale.
*   **NUMERIC**, **DECIMAL**, and **DEC** are used interchangeably.
*   To check how the precision and the scale became changed when you operate with **NUMERIC** typed values, see :ref:`numeric-data-type-op-and-conversion`.

**Example** ::

	If you specify 12345.6789 as NUMERIC, 12346 is stored (it rounds to the first place after the decimal point since 0 is the default value of scale).
	If you specify 12345.6789 as NUMERIC(4), an error occurs (precision must be equal to or greater than the number of integer digits).
	If you declare NUMERIC(3,4), an error occurs (precision must be equal to or greater than the scale).
	If you specify 0.12345678 as NUMERIC(4,4), .1235 is stored (it rounds to the fifth place after the decimal point).
	If you specify -0.123456789 as NUMERIC(4,4), -.1235 is stored (it rounds to the fifth place after decimal point and then prefixes a minus (-) sign).

FLOAT/REAL
----------

The **FLOAT** (or **REAL**) data type represents floating point numbers.

The ranges of values that can be described as normalized values are from -3.402823466E+38 to -1.175494351E-38, 0, and from +1.175494351E-38 to +3.402823466E+38, whereas the values other than normalized values, which are closer to 0, are described as de-normalized values. It conforms to the ANSI/IEEE 754-1985 standard.

The minimum value for the precision *p* is 1 and the maximum value is 38. When the precision *p* is omitted or it is specified as seven or less, it is represented as single precision (in 7 significant figures). If the precision *p* is greater than 7 and equal to or less than 38, it is represented as double precision (in 15 significant figures) and it is converted into **DOUBLE** data type.

**FLOAT** data types must not be used if you want to store a precise value that exceeds the number of significant figures, as they only store the approximate value of any input value over 7 significant figures. ::

	FLOAT[(p)]
	
*   **FLOAT** is in 7 significant figures.
*   Extra cautions are required when comparing data because the **FLOAT** type stores approximate numeric.
*   **FLOAT** and **REAL** are used interchangeably.

**Example** ::

	If you specify 16777217 as FLOAT, 16777216 is stored and 1.677722e+07 is displayed (if precision is omitted, 8-th digit is rounded up because it is represented as 7 significant figures).
	If you specify 16777217 as FLOAT(5), 16777216 is stored and 1.677722e+07 is displayed (if precision is in seven or less, 8-th digit is rounded up because it is represented as 7 significant figures).
	If you specify 16777.217 as FLOAT(5), 16777.216 is stored and 1.677722e+04 is displayed (if precision is in seven or less, 8-th digit is rounded up because it is represented as 7 significant figures).
	If you specify 16777.217 as FLOAT(10), 16777.217 is stored and 1.677721700000000e+04 is displayed (if precision is greater than 7 and less than or equal to 38, zeroes are added because it is represented as 15 significant figures).

DOUBLE/DOUBLE PRECISION
-----------------------

The **DOUBLE** data type is used to represent floating point numbers.

The ranges of values that can be described as normalized values are from -1.7976931348623157E+308 to -2.2250738585072014E-308, 0, and from 2.2250738585072014E-308 to 1.7976931348623157E+308, whereas the values other than normalized values, which are closer to 0, are described as de-normalized values. It conforms to the ANSI/IEEE 754-1985 standard.

The precision *p* is not specified. The data specified as this data type is represented as double precision (in 15 significant figures).

**DOUBLE** data types must not be used if you want to store a precise value that exceeds the number of significant figures, as they only store the approximate value of any input value over 15 significant figures.

*   **DOUBLE** is in 15 significant figures.
*   Extra caution is required when comparing data because the **DOUBLE** type stores approximate numeric.
*   **DOUBLE** and **DOUBLE PRECISION** are used interchangeably.

**Example** ::

	If you specify 1234.56789 as DOUBLE, 1234.56789 is stored and 1.234567890000000e+03 is displayed.
	If you specify 9007199254740993 as DOUBLE, 9007199254740992 is stored and 9.007199254740992e+15 is displayed.

MONETARY
--------

The **MONETARY** data type is an approximate numeric data type. The range of valid value is the same as **DOUBLE**, which is represented to two decimal places; the value range can be different based on system. A comma is appended to every 1000th place.

You can use a dollar sign or a decimal point, but a comma is not allowed.

**Example** ::

	If you specify 12345.67898934 as MONETARY, $12,345.68 is stored (it is rounded to third decimal place).
	If you specify 123456789 as MONETARY, $123,456.789.00 is stored.

.. _date-time-type:

Date/Time Types
===============

Date/time data types are used to represent the date or time (or both together). CUBRID supports the following data types:

**Date-Time Types Supported by CUBRID**

+---------------+-----------+---------------------------+---------------------------+---------------------------------------------------------------------+
| Type          | bytes     | Min.                      | Max.                      | Note                                                                |
+===============+===========+===========================+===========================+=====================================================================+
| **DATE**      | 4         | 0001-01-01                | 9999-12-31                | As an exception, DATE '0000-00-00' format is allowed.               |
+---------------+-----------+---------------------------+---------------------------+---------------------------------------------------------------------+
| **TIME**      | 4         | 00:00:00                  | 23:59:59                  |                                                                     |
+---------------+-----------+---------------------------+---------------------------+---------------------------------------------------------------------+
| **TIMESTAMP** | 4         | 1970-01-01 00:00:01 (GMT) | 2038-01-19 03:14:07 (GMT) | As an exception, TIMESTAMP '0000-00-00 00:00:00' format is allowed. |
|               |           | 1970-01-01 09:00:01 (KST) | 2038-01-19 12:14:07 (KST) |                                                                     |
+---------------+-----------+---------------------------+---------------------------+---------------------------------------------------------------------+
| **DATETIME**  | 8         | 0001-01-01 00:00:0.000    | 9999-12-31 23:59:59.999   | As an exception, DATETIME '0000-00-00 00:00:00' format is allowed.  |
+---------------+-----------+---------------------------+---------------------------+---------------------------------------------------------------------+

**Range and Resolution**

*   By default, the range of a time value is represented by the 24-hour system. Dates follow the Gregorian calendar. An error occurs if a value that does not meet these two constraints is entered as a date or time.

*   The range of year in  **DATE** is 0001 - 9999 AD.

*   From the CUBRID 2008 R3.0 version, if time value is represented with two-digit numbers, a number from 00 to 69 is converted into a number from 2000 to 2069; a number from 70 to 99 is converted into a number from 1970 to 1999. In earlier than CUBRID 2008 R3.0 version, if time value is represented with two-digit numbers, a number from 01 to 99 is converted into a number from 0001 to 0099.

*   The range of **TIMESTAMP** is between 1970-01-01 00:00:01 - 2038-01-19 03 03:14:07 (GMT). For KST (GMT+9), values from 1970-01-01 00:00:01 to 2038-01-19 12:14:07 can be stored. timestamp'1970-01-01 00:00:00' (GMT) is the same as timestamp'0000-00-00 00:00:00'.

*   The results of date, time and timestamp operations may depend on the rounding mode. In these cases, for Time and Timestamp, the most approximate second is used as the minimum resolution; for Date, the most approximate date is used as the minimum resolution.

**Coercions**

The **Date** / **Time** types can be cast explicitly using the **CAST** operator only when they have the same field. For implicit coercion, see :ref:`implicit-type-conversion`. The following table shows types that allows explicit coercions. For implicit coercion, see :ref:`arithmetic-op-type-casting`.

**Explicit Coercions**

+----------+------------------------------------------------+
|          | TO                                             |
+==========+===========+======+======+==========+===========+
| FROM     |           | DATE | TIME | DATETIME | TIMESTAMP |
|          +-----------+------+------+----------+-----------+
|          | DATE      | -    | X    | O        | O         |
|          +-----------+------+------+----------+-----------+
|          | TIME      | X    | -    | X        | X         |
|          +-----------+------+------+----------+-----------+
|          | DATETIME  | O    | O    | -        | O         |
|          +-----------+------+------+----------+-----------+
|          | TIMESTAMP | O    | O    | O        | -         |
+----------+-----------+------+------+----------+-----------+

In general, zero is not allowed in **DATE**, **DATETIME**, and **TIMESTAMP** types. However, if both date and time values are 0, it is allowed as an exception. This is useful in terms that this value can be used if an index exists upon query execution of a column corresponding to the type.

*   Some functions in which the **DATE**, **DATETIME**, and **TIMESTAMP** types are specified as an argument return different value based on the **return_null_on_function_errors** system parameter if every input argument value for date and time is 0. If **return_null_on_function_errors** is yes, **NULL** is returned; if no, an error is returned. The default value is **no**.

*   The functions that return **DATE**, **DATETIME**, and **TIMESTAMP** types can return a value of 0 for date and time. However, these values cannot be stored in Date objects in Java applications. Therefore, it will be processed with one of the followings based on the configuration of zeroDateTimeBehavior, the connection URL property: being handled as an exception, returning **NULL**, or returning a minimum value (see "API Reference > JDBC API > JDBC Programming > Connection Configuration").

*   If the **intl_date_lang** system is configured, input string of :func:`TO_DATE`, :func:`TO_DATETIME`, and :func:`TO_TIMESTAMP` functions follows the corresponding locale date format. For details, see :ref:`stmt-type-parameters`.

For details, see the description of each function.

DATE
----

The **DATE** data type is used to represent the year (yyyy), month (mm) and day (dd). Supported range is "01/01/0001" to "12/31/9999." The year can be omitted. If it is, the year value of the current system is specified automatically. The specified input/output types are as follows: ::

	date'mm/dd[/yyyy]'
	date'[yyyy-]mm-dd'

*   All fields must be entered as integer.
*   The date value is displayed in the type of 'MM/DD/YYYY' in CSQL, and it is displayed in the type of 'YYYY-MM-DD' in JDBC application programs and the CUBRID Manager.
*   The :func:`TO_DATE` function is used to convert a character string type into a **DATE** type. 
*   0 is not allowed to input in year, month, and day; however, '0000-00-00', which every digit consisting of year, month, and day is 0, is allowed as an exception.

**Example** ::

	DATE '2008-10-31' is displayed as '10/31/2008'.
	DATE '10/31' is displayed as '10/31/2011'(if a value for year is omitted, the current year is automatically specified).
	DATE '00-10-31' is displayed as '10/31/2000'.
	DATE '0000-10-31' is displayed as an error (a year value should be at least 1).
	DATE '70-10-31' is displayed as '10/31/1970'.
	DATE '0070-10-31' displayed as '10/31/0070'.

TIME
----

The **TIME** data type is used to represent the hour (hh), minute (mm) and second (ss). Supported range is "00:00:00" to "23:59:59." Second can be omitted; if it is, 0 seconds is specified. Both 12-hour and 24-hour notations are allowed as an input format.

The input format of **TIME** is as follows: ::

	time'hh:mi [:ss] [am | pm]'
	
*   All items must be entered as integer.
*   AM/PM time notation is used to display time in the CSQL; while the 24-hour notation is used in the CUBRID Manager.
*   AM/PM can be specified in the 24-hour notation. An error occurs if the time specified does not follow the AM/PM format.
*   Every time value is stored in the 24-hour notation. **db_time_decode**, one of C API functions, is used to return a value in the 24-hour notation.
*   The :func:`TO_TIME` function is used to return a character string type into a TIME type.

**Example** ::

	TIME '00:00:00’ is outputted as '12:00:00 AM'.
	TIME '1:15' is regarded as '01:15:00 AM'.
	TIME '13:15:45' is regarded as '01:15:45 PM'.
	TIME '13:15:45 pm' is stored normally.
	TIME '13:15:45 am' is an error (an input value does not match the AM/PM format).

TIMESTAMP
---------

The **TIMESTAMP** data type is used to represent a data value in which the date (year, month, date) and time (hour, minute, second) are combined. The range of representable value is between GMT '1970-01-01 00:00:01' and '2038-01-19 03:14:07'. The **DATETIME** type can be used if the value is out of range or data in milliseconds is stored. The input format of **TIMESTAMP** is as follows: ::
 
	timestamp'hh:mi [:ss] [am|pm] mm/dd [/yyyy]'
	timestamp'hh:mi [:ss] [am|pm] [yyyy-]mm-dd'
	 
	timestamp'mm/dd [/yyyy] hh:mi [:ss] [am|pm]'
	timestamp'[yyyy-]mm-dd hh:mi [:ss] [am|pm]'

*   All fields must be entered in integer format.
*   If the year is omitted, the current year is specified by default. If the time value (hour/minute/second) is omitted, 12:00:00 AM is specified.

*   You can store the timestamp value of the system in the **TIMESTAMP** type by using the :func:`SYS_TIMESTAMP` (or :func:`SYSTIMESTAMP`, :func:`CURRENT_TIMESTAMP`) function. Note that the timestamp value is specified as a default value at the time of creating the table, not at the time of **INSERT** the data, if :func:`SYS_TIMESTAMP` is specified as a **DEFAULT** value for a **TIMESTAMP** column when creating a table.

*   The :func:`TIMESTAMP` or :func:`TO_TIMESTAMP` function is used to cast a character string type into a **TIMESTAMP** type.
*   0 is not allowed to input in year, month, and day; however, '0000-00-00 00:00:00', which every digit consisting of year, month, day, hour, minute, and second is 0, is allowed as an exception. GMT timestamp'1970-01-01 12:00:00 AM' or KST timestamp'1970-01-01 09:00:00 AM' is translated into timestamp'0000-00-00 00:00:00'.

**Example** ::

	TIMESTAMP '10/31' is outputted as '12:00:00 AM 10/31/2011' (if the value for year/time is omitted, a default value is outputted ).
	TIMESTAMP '10/31/2008' is outputted as '12:00:00 AM 10/31/2008' (if the value for time is omitted, a default value is outputted ).
	TIMESTAMP '13:15:45 10/31/2008' is outputted as '01:15:45 PM 10/31/2008'.
	TIMESTAMP '01:15:45 PM 2008-10-31' is outputted as '01:15:45 PM 10/31/2008'.
	TIMESTAMP '13:15:45 2008-10-31' is outputted as '01:15:45 PM 10/31/2008'.
	TIMESTAMP '10/31/2008 01:15:45 PM' is outputted as '01:15:45 PM 10/31/2008'.
	TIMESTAMP '10/31/2008 13:15:45' is outputted as '01:15:45 PM 10/31/2008'.
	TIMESTAMP '2008-10-31 01:15:45 PM' is outputted as '01:15:45 PM 10/31/2008'.
	TIMESTAMP '2008-10-31 13:15:45' is outputted as '01:15:45 PM 10/31/2008'.

An error occurs on TIMESTAMP '2099-10-31 01:15:45 PM' (out of range to represent TIMESTAMP).

DATETIME
--------

The **DATETIME** data type is used to represent a data value in which the data (year, month, date) and time (hour, minute, second) are combined. The range of representable value is between 0001-01-01 00:00:00.000 and 9999-12-31 23:59:59.999 (GMT).

The input format of **TIMESTAMP** is as follows: ::

	datetime'hh:mi [:ss[.msec]] [am|pm] mm/dd [/yyyy]'
	datetime'hh:mi [:ss[.msec]] [am|pm] [yyyy-]mm-dd'
	datetime'mm/dd[/yyyy] hh:mi[:ss[.ff]] [am|pm]'
	datetime'[yyyy-]mm-dd hh:mi[:ss[.ff]] [am|pm]'

*   All fields must be entered as integer.
*   If you year is omitted, the current year is specified by default. If the value (hour, minute/second) is omitted, 12:00:00.000 AM is specified.

*   You can store the timestamp value of the system in the **DATETIME** type by using the :func:`SYS_DATETIME` (or :func:`SYSDATETIME`, :func:`CURRENT_DATETIME`, :func:`CURRENT_DATETIME`, :func:`NOW`) function. Note that the timestamp value is specified as a default value at the time of creating the table, not at the time of **INSERT** the data, if :func:`SYS_DATETIME` is specified as a **DEFAULT** value for a **DATETIME** column when creating a table.

*   The :func:`TO_DATETIME:func:` function is used to convert a string type into a **DATETIME** type.
*   0 is not allowed to input in year, month, and day; however, '0000-00-00 00:00:00', which every digit consisting of year, month, day, hour, minute, and second is 0, is allowed as an exception.

**Example** ::

	DATETIME '10/31' is outputted as '12:00:00.000 AM 10/31/2011' (if the value for year/time is omitted, a default value is outputted).
	DATETIME '10/31/2008' is outputted as '12:00:00.000 AM 10/31/2008'.
	DATETIME '13:15:45 10/31/2008' is outputted as '01:15:45.000 PM 10/31/2008'.
	DATETIME '01:15:45 PM 2008-10-31' is outputted as '01:15:45.000 PM 10/31/2008'.
	DATETIME '13:15:45 2008-10-31' is outputted as '01:15:45.000 PM 10/31/2008'.
	DATETIME '10/31/2008 01:15:45 PM' is outputted as '01:15:45.000 PM 10/31/2008'.
	DATETIME '10/31/2008 13:15:45' is outputted as '01:15:45.000 PM 10/31/2008'.
	DATETIME '2008-10-31 01:15:45 PM' is outputted as '01:15:45.000 PM 10/31/2008'.
	DATETIME '2008-10-31 13:15:45' is outputted as '01:15:45.000 PM 10/31/2008'.
	DATETIME '2099-10-31 01:15:45 PM' is outputted as '01:15:45.000 PM 10/31/2099'.

.. _cast-string-to-datetime:

CASTing a String to Date/Time Type
----------------------------------

**Recommended Format for Strings in Date/Time Type**

When you casting a string to Date/Time type by using the :func:`CAST` function, it is recommended to write the string in the following format: Note that date/time string formats used in the :func:`CAST` function are not affected by locale (which is specified as the **CUBRID_CHARSET** environment variable).

*   **DATE** Type ::

	YYYY-MM-DD
	MM/DD/YYYY

*   **TIME** Type ::

	HH:MM:SS ["AM"|"PM"]

*   **DATETIME** Type ::

	YYYY-MM-DD HH:MM:SS[.msec] ["AM"|"PM"]

*   **TIMESTAMP** Type ::

	YYYY-MM-DD HH:MM:SS ["AM"|"PM"]

**Available DATE String Format** ::

	[year sep] month sep day
	
*   2011-04-20: April 20th, 2011
*   04-20: April 20th of this year

If a separator (*sep*) is a slash (/), strings are recognized in the following order: ::

	month/day[/year]
	
*   04/20/2011: April 20th, 2011
*   04/20: April 20th of this year

If you do not use a separator (*sep*), strings are recognized in the following format. It is allowed to use 1, 2, and 4 digits for years and 1 and 2 digits for months. For day, you should always enter 2 digits. ::

	YYYYMMDD
	YYMMDD
	YMMDD
	MMDD
	MDD

*   20110420: April 20th, 2011
*   110420: April 20th, 2011
*   420: April 20th of this year

**Available TIME String Format** ::

	[hour]:min[:[sec]] [.[msec]] [am|pm]
	
*   09:10:15.359 am: 9 hours 10 minutes 15 seconds AM (0.359 seconds will be truncated)
*   09:10:15: 9 hours 10 minutes 15 seconds AM
*   09:10: 9 hours 10 minutes AM
*   \:10: 12 hours 10 minutes AM

::

	[[[[[[Y]Y]Y]Y]M]MDD]HHMMSS[.[msec]] [am|pm]
	
*   20110420091015.359 am: 9 hours 10 minutes 15 seconds AM
*   0420091015: 9 hours 10 minutes 15 seconds AM

::

	[H]HMMSS[.[msec]] [am|pm]

*   091015.359 am: 9 hours 10 minutes 15 seconds AM
*   91015: 9 hours 10 minutes 15 seconds AM

::

	[M]MSS[.[msec]] [am|pm]
	
*   1015.359 am: 12 hours 10 minutes 15 seconds AM
*   1015: 12 hours 10 minutes 15 seconds AM

::

	[S]S[.[
	*msec*
	]] [am|pm]

*   15.359 am: 12 hours 15 seconds AM
*   15: 12 hours 15 seconds AM


.. note::

	: The [H]H format was allowed in CUBRID 2008 R3.1 and the earlier versions. That is, the string '10' was converted to **TIME** '10:00:00' in the R3.1 and the earlier versions, and will be converted to **TIME** '00:00:10' in version R4.0 and later.

**Available DATETIME String Format** ::

	[year sep] month sep day [sep] [sep] hour [sep min[sep sec[.[msec]]]]
	
*   04-20 09: April 20th of this year, 9 hours AM

::

	month/day[/year] [sep] hour [sep min [sep sec[.[msec]]]]

*   04/20 09: April 20th of this year, 9 hours AM

::

	year sep month sep day sep hour [sep min[sep sec[.[msec]]]]
	
*   2011-04-20 09: April 20th, 2011, 9 hours AM

::

	month/day/year sep hour [sep min[sep sec [.[msec]]]]

*   04/20/2011 09: April 20th, 2011, 9 hours AM

::

	YYMMDDH (It is allowed only when time format is one digit.)

*   1104209: April 20th, 2011, 9 hours AM

::

	YYMMDDHHMM[SS[.msec]]
	
*   1104200910.359: April 20th, 2011, 9 hours 10 minutes AM (0.359 seconds will be truncated)
*   110420091000.359: April 20th, 2011, 9 hours 10 minutes 0.359 seconds AM

::

	YYYYMMDDHHMMSS[.msec]

*   201104200910.359: November 4th, 2020 8 hours 9 minutes 10.359 seconds PM
*   20110420091000.359: April 20th, 2011, 9 hours 10 minutes 0.359 seconds AM

**Available Time-Date String Format** ::

	[hour]:min[:sec[.msec]] [am|pm] [year-]month-day
	
*   09:10:15.359 am 2011-04-20: April 20th, 2011, 9 hours 10 minutes 15.359 seconds AM
*   \:10 04-20: April 20th of this year, 12 hours 10 minutes AM

::

	[hour]:min[:sec[.msec]] [am|pm] month/day[/[year]]

*   09:10:15.359 am 04/20/2011: April 20th, 2011, 9 hours 10 minutes 15.359 seconds AM
*   \:10 04/20: April 20th of this year, 12 hours 10 minutes AM

::

	hour[:min[:sec[.[msec]]]] [am|pm] [year-]month-day
	
*   09:10:15.359 am 04-20: April 20th of this year, 9 hours 10 minutes 15.359 seconds AM
*   09 04-20: April 20th of this year, 9 hours AM

::

	hour[:min[:sec[.[msec]]]] [am|pm] month/day[/[year]]
	
*   09:10:15.359 am 04/20: April 20th of this year, 9 hours 10 minutes, 15.359 seconds AM
*   09 04/20: April 20th of this year, 9 hours AM

**Rules**

*msec* is a series of numbers representing milliseconds. The numbers after the fourth digit will be ignored.
The rules for the separator string are as follows:

*   You should always use one colon (:) as a separator for the **TIME** separator.

*   **DATE** and **DATETIME** strings can be represented as a series of numbers without the separator sep), and non-alphanumeric characters can be used as separators. The **DATETIME** string can be divided into Time and Date with a space.

*   Separators should be identical in the input string.

*   For the Time-Date string, you can only use colon (:) for a Time separator and hyphen (-) or slash (/) for a Date separator. If you use a hyphen when entering date, you should enter like yyyy-mm-dd; in case of  a slash, enter like mm/dd/yyyy.

The following rules will be applied in the part of date.

*   You can omit the year as long as the syntax allows it.

*   If you enter the year as two digits, it represents the range from 1970-2069. That is, if YY<70, it is treated as 2000+YY; if YY>=70, it is treated as 1900+YY. If you enter one, three or four digit numbers for the year, the numbers will be represented as they are.

*   A space before and after a string and the string next to the space are ignored. The am/pm identifier for the **DATETIME** and **TIME** strings can be recognized as part of TIME value, but are not recognized as the am/pm identifier if non-space characters are added to it.

The **TIMESTAMP** type of CUBRID consists of **DATE** type and **TIME** type, and **DATETIME** type consists of **DATE** type and **TIME** type with milliseconds being added to them. Input strings can include Date (**DATE** string), Time (**TIME** string), or both (**DATETIME** strings). You can convert a string including a specific type of data to another type, and the following rules will be applied for the conversion.

*   If you convert the **DATE** string to the **DATETIME** type, the time value will be '00:00:00.'

*   If you convert the **TIME** string to the **DATETIME** type, colon (:) is recognized as a date separator, so that the **TIME** string can be recognized as a date string and the time value will be '00:00:00.'

*   If you convert the **DATETIME** string to the **DATE** type, the time part will be ignored from the result but the time input value format should be valid.

*   You can covert the **DATETIME** string to the **TIME** type, and you must follow the following rules.

    *   The date and time in the string must be divided by at least one blank.

    *   The date part of the result value is ignored but the date input value format should be valid.

    *   The year in the date part must be over 4 digits (available to start with 0) or the time part must include hours and minutes ([H]H:[M]M) at least. Otherwise the date pate are recognized as the TIME type of the [MM]SS format, and the following string will be ignored.

*   If the one of the units (year, month, date, hour, minute and second) of the **DATETIME** string is greater than 999999, it is not recognized as a number, so the string including the corresponding unit will be ignored. For example, in '2009-10-21 20:9943:10', an error occurs because the value in minutes is out of the range. However, if '2009-10-21 20:1000123:10' is entered,'2009' is recognized as the the **TIME** type of the MMSS format, so that **TIME** '00:20:09' will be returned.

*   If you convert the time-date sting to the **TIME** type, the date part of the string is ignored but the date part format must be valid.

*   All input strings including the time part allow [.*msec*] on conversion, but only the **DATETIME** type can be maintained. If you convert this to a type such as **DATE**, **TIMESTAMP** or **TIME**, the *msec* value is discarded.

*   All conversions in the **DATETIME**, **TIME** string allow English locale following after time value or am/pm specifier written in the current locale of a server.

**Example**

.. code-block:: sql

	SELECT CAST('420' AS DATE);
	 
	   cast('420' as date)
	======================
	  04/20/2012
	 
	SELECT CAST('91015' AS TIME);
	 
	   cast('91015' as time)
	========================
	  09:10:15 AM
	 
	 
	SELECT CAST('110420091035.359' AS DATETIME);
	 
	   cast('110420091035.359' as datetime)
	=======================================
	  09:10:35.359 AM 04/20/2011
	 
	SELECT CAST('110420091035.359' AS TIMESTAMP);
	 
	   cast('110420091035.359' as timestamp)
	========================================
	  09:10:35 AM 04/20/2011

Bit Strings
===========

A bit string is a sequence of bits (1's and 0's). Images (bitmaps) displayed on the computer screen can be stored as bit strings. CUBRID supports the following two types of bit strings:

*   Fixed-length bit string (**BIT**)
*   Variable-length bit string (**BIT VARYING**)

A bit string can be used as a method argument or an attribute type. Bit string literals are represented in a binary or hexadecimal format. For binary format, append the string consisting of 0's and 1's to the letter **B** or append a value to the **0b** as shown example below. ::

	B'1010'
	0b1010

For hexadecimal format, append the string consisting of the numbers 0 - 9 and the letters A - F to the uppercase letter **X** or append a value to the **0x** . The following is hexadecimal representation of the same number that was represented above in binary format. ::

	X'a'
	0xA

The letters used in hexadecimal numbers are not case-sensitive. That is, X'4f' and X'4F' are considered as the same value.

**Length**

If a bit string is used in table attributes or method declarations, you must specify the maximum length. The maximum length for a bit string is 1,073,741,823 bits.

**Bit String Coercion**

Automatic coercion is performed between a fixed-length and a variable-length bit string for comparison. For explicit coercion, use the :func:`CAST` operator.

BIT(n)
------

Fixed-length binary or hexadecimal bit strings are represented as **BIT** (*n*), where *n* is the maximum number of bits. If *n* is not specified, the length is set to 1. If *n* is not specified, the length is set to 1. The bit string is filled with 4-bit values from the left side. For example, the value of B'1' is the same as the value of B'1000'.

*   *n* must be a number greater than 0.
*   If the length of the string exceeds *n*, it is truncated and filled with 0s.
*   If a bit string smaller than *n* is stored, the remainder of the string is filled with 0s.

**Example** 

.. code-block:: sql

	CREATE TABLE bit_tbl(a1 BIT, a2 BIT(1), a3 BIT(8), a4 BIT VARYING);
	INSERT INTO bit_tbl VALUES (B'1', B'1', B'1', B'1');
	INSERT INTO bit_tbl VALUES (0b1, 0b1, 0b1, 0b1);
	INSERT INTO bit_tbl(a3,a4) VALUES (B'1010', B'1010');
	INSERT INTO bit_tbl(a3,a4) VALUES (0xaa, 0xaa);
	SELECT * FROM bit_tbl;

::

	  a1                    a2                    a3                    a4
	 
	=========================================================================
	  X'8'                  X'8'                  X'80'                 X'8'
	  X'8'                  X'8'                  X'80'                 X'8'
	  NULL                  NULL                  X'a0'                 X'a'
	  NULL                  NULL                  X'aa'                 X'aa'

BIT VARYING(n)
--------------

A variable-length bit string is represented as **BIT VARYING** (*n*), where *n* is the maximum number of bits. If *n* is not specified, the length is set to 1,073,741,823 (maximum value). *n* is the maximum number of bits. If *n* is not specified, the maximum length is set to 1,073,741,823. The bit string is filled with 4-bit values from the left side. For example, the value of B'1' is the same as the value of B'1000'.

*   If the length of the string exceeds *n*, it is truncated and filled with 0s.
*   The remainder of the string is not filled with 0s even if a bit string smaller than *n* is stored.
*   *n* must be a number greater than 0.

**Example**

.. code-block:: sql

	CREATE TABLE bitvar_tbl(a1 BIT VARYING, a2 BIT VARYING(8));
	INSERT INTO bitvar_tbl VALUES (B'1', B'1');
	INSERT INTO bitvar_tbl VALUES (0b1010, 0b1010);
	INSERT INTO bitvar_tbl VALUES (0xaa, 0xaa);
	INSERT INTO bitvar_tbl(a1) VALUES (0xaaa);
	SELECT * FROM bitvar_tbl;

::

	  a1                    a2
	============================================
	  X'8'                  X'8'
	  X'a'                  X'a'
	  X'aa'                 X'aa'
	  X'aaa'                NULL
	 
	INSERT INTO bitvar_tbl(a2) VALUES (0xaaa);
	 
	ERROR: Data overflow coercing X'aaa' to type bit varying.

.. _char-data-type:

Character Strings
=================

CUBRID supports the following four types of character strings:

*   Fixed-length character string: **CHAR** (*n*)
*   Variable-length character string: **VARCHAR** (*n*)
*   Fixed-length national character string: **NCHAR** (*n*)
*   Variable-length national character string: **NCHAR VARYING** (*n*)

The followings are the rules that are applied when using the character string types.

*   In general, single quotations are used to enclose character string. Double quotations may be used as well depending on the value of **ansi_quotes**, which is a parameter related to SQL statement. If the **ansi_quotes** value is set to **no**, character string enclosed by double quotations is handled as character string, not as an identifier. The default value is **yes**. For details, :ref:`stmt-type-parameters`.

*   If there are characters that can be considered to be blank (e.g. spaces, tabs, or line breaks) between two character strings, these two character strings are treated as one according to ANSI standard. For example, the following example shows that a line break exists between two character string. ::

	'abc'
	'def'

*   The two strings above are considered identical to one string below. ::

	'abcdef'

*   If you want to include a single quote as part of a character string, enter two single quotes in a row. For example, the character string on the left is stored as the one on the right. ::

	''abcde''fghij'            'abcde'fghij

*   The maximum size of the token for all the character strings is 16 KB.

* National character strings are used to store national (except English alphabet) character strings in a multilingual environment. Note that **N** (uppercase) should be followed by a single quote which encloses character strings. ::

	N'Härder'

  However, to enter the language of a specific country, we recommend that you to change the locale by using the **CUBRID_CHARSET** environment variable or introducer **CHARSET** (or **COLLATE** modifier) or by using the general string type (**VARCHAR** or **CHAR**) instead of the country string type. For a more detailed description, see :doc:`/admin/i18n`.

For a **CHAR** or **VARCHAR** type, specify the length (bytes) of a character string for a **NCHAR** or **NCHAR VARYING** type, specify the number of character strings (number of characters).
When the length of the character string entered exceeds the length specified, the characters in excess of the specified length are truncated.

For a fixed-length character string type such as **CHAR** or **NCHAR**, the length is fixed at the declared length. Therefore, the right part (trailing space) of the character string is filled with space characters when the string is stored. For a variable-length character string type such as **VARCHAR** or **NCHAR VARYING**, only the entered character string is stored, and the space is not filled with space characters.

The maximum length of a **CHAR** or **VARCHAR** type to be specified is 1,073,741,823 the maximum length of a **NCHAR** or **NCHAR VARYING** type to be specified is 536,870,911. The maximum length that can be input or output in a CSQL statement is 8,192 KB.

**Character Set, charset**

A character set (charset) is a set in which rules are defined that relate to what kind of codes can be used for encoding when specified characters (symbols) are stored in the computer.

The characted used by CUBRID can be configued as the **CUBRID_CHARSET** environment variable. For details, see :doc:`/admin/i18n`.

**Collating Character Sets**

A collation is a set of rules used for comparing characters to search or sort values stored in the database when a certain character set is specified. For details, see :doc:`/admin/i18n`.

Therefore, such rules are applied only to character string data types such as **CHAR** or **VARCHAR**. For a national character string type such as **NCAHR()** or **NCHAR VARYING()** , the sorting rules are determined according to the encoding algorithm of the specified character set.

**Character String Coercion**

Automatic coercion takes place between a fixed-length and a variable-length character string for the comparison of two characters, applicable only to characters that belong to the same character set.

For example, when you extract a column value from a CHAR(5) data type and insert it into a column with a CHAR(10) data type, the data type is automatically coerced to CHAR(10). If you want to coerce a character string explicitly, use the **CAST** operator (See :func:`CAST`).

CHAR(n)
-------

A fixed-length character string is represented as **CHAR** *(n)*, in which *n* represents the number of characters. If *n* is not specified, the value is specified as 1, default value.

When the length of a character string exceeds *n*, they are truncated. When character string which is shorter than *n* is stored, whitespace characters are used to fill up the trailing space.

**CHAR** (*n*) and **CHARACTER** (*n*) are used interchangeably.

.. note:: In the earlier versions of CUBRID 9.0, *n* represents bite length, not the number of characters.

*   The **CHAR** data type is always based on the ISO 8859-1 Latin character set.

*   *n* is an integer between 1 and 1,073,741,823 (1G).

*   Empty quotes (' ') are used to represent a blank string. In this case, the return value of the **LENGTH** function is not 0, but is the fixed length defined in **CHAR** (*n*). That is, if you enter a blank string into a column with **CHAR** (10), the **LENGTH** is 10; if you enter a blank value into a **CHAR** with no length specified, the **LENGTH** is the default value 1.

*   Space characters used as filling characters are considered to be smaller than any other characters, including special characters.

**Example** ::

	If you specify 'pacesetter' as CHAR(12), 'pacesetter ' is stored (a 10-character string plus two whitespace characters).
	If you specify 'pacesetter ' as CHAR(10), 'pacesetter' is stored (a 10-character string; two whitespace characters are truncated).
	If you specify 'pacesetter' as CHAR(4), 'pace' is stored (truncated as the length of the character string is greater than 4).
	If you specify 'p ' as CHAR, 'p' is stored (if n is not specified, the length is set to the default value 1).

VARCHAR(n)/CHAR VARYING(n)
--------------------------

Variable-length character strings are represented as **VARCHAR** (*n*), where *n* represents the number of characters. If *n* is not specified, the value is specified as 1,073,741,823, the maximum length.

When the length of a character string exceeds *n*, they are truncated. When character string which is shorter than *n* is stored, whitespace characters are used to fill up the trailing space for **VARCHAR** (*n*), the length of string used are stored. 

**VARCHAR** (*n*), **CHARACTER, VARYING** (*n*), and **CHAR VARYING** (*n*) are used interchangeably.

.. note:: In the earlier versions of CUBRID 9.0, *n* represents bite length, not the number of characters.

*   **STRING** is the same as the **VARCHAR** (maximum length).
*   *n* is an integer between 1 and 1,073,741,823 (1G).
*   Empty quotes (' ') are used to represent a blank string. In this case, the return value of the **LENGTH** function is not 0.


**Example 1** ::

	If you specify 'pacesetter' as CHAR(4), 'pace' is stored (truncated as the length of the character string is greater than 4).
	If you specify 'pacesetter' as VARCHAR(12), 'pacesetter' is stored (a 10-character string).
	If you specify 'pacesetter ' as VARCHAR(12), 'pacesetter ' is stored (a 10-character string plus two whitespace characters).
	If you specify 'pacesetter ' as VARCHAR(10), 'pacesetter' is stored (a 10-character string; two whitespace characters are truncated).
	If you specify 'p ' as VARCHAR, 'p' is stored (if n is not specified, the default value 1,073,741,823 is used, and the trailing space is not filled with whitespace characters).

**Example 2** ::

	If you specify '큐브리드' as VARCHAR(10) in the EUC-KR encoding, it is processed normally.
	If you specify '큐브리드' as CHAR(10) and then use CHAR_LENGTH() function in the EUC-KR encoding, 8 is stored.
	If you specify '큐브리드, as CHAR(10) in the utf-8 encoding, the last character is broken (one Korean character takes up three bytes in the UTF-8 encoding so it requires two more bytes).
	If you specify '큐브리드' as VARCHAR(12) in the UTF-8 encoding, it is processed normally.

STRING
------

**STRING** is a variable-length character string data type. **STRING** is the same as the VARCHAR with the length specified to the maximum value. That is, **STRING** and **VARCHAR** (1,073,741,823) have the same value.

NCHAR(n)
--------

**NCHAR** (*n*) is used to store non-English character strings. It can be used only for character sets supported by CUBRID described above. n is the number of characters. If *n* is omitted, the length is specified as the default value 1. When the length of a character string exceeds *n*, they are truncated. When character string which is shorter than *n* is stored, whitespace characters are used to fill up the  space.

To store a Korean character string as a national character string type, you must set the locale of the operating system to Korean, or set the value of the **CUBRID_CHARSET** environment variable to **ko_KR.euckr** before creating the table.

.. note:: In the earlier versions of CUBRID 9.0, *n* represents bite length, not the number of characters.

*   *n* is an integer between 1 and 536,870,911.
*   The number of national character sets that can be used in a single database is set to be one. For example, 8-bit ISO 8889-1 (Latin-1) and EUC code sets cannot be used simultaneously in the same database.

*   An error occurs if a non-national character string (whether it is fixed-length or variable-length) is specified for an attribute declared as a national character string.
*   Using two different character code sets at once also causes an error.

**Example** ::

	If you specify ‘큐브리드’ as NCHAR(5) in the EUC-KR encoding, it is processed normally.
	If you specify '큐브리드' as NCHAR(5) and then use the CHAR_LENGTH() function in the EUC-KR encoding, 5 is stored.
	If you specify '큐브리드' as NCHAR(5) in the utf-8 encoding, an error occurs (utf-8 character set is not supported).

NCHAR VARYING(n)
----------------

**NCHAR VARYING** (*n*) is a variable-length character string type. For details, see description and note of NCHAR(n). The difference is that the right part (trailing space) of the character string is not filled with whitespace characters, even when the number of strings is smaller than n.

**NCHAR VARYING** (*n*), **NATIONAL CHAR VARYING**(*n*), and **NATIONAL CHARACTER VARYING(n)** are used interchangeably.

.. note:: In the earlier versions of CUBRID 9.0, n represents bite length, not the number of characters.

**Example** ::

	If you specify '큐브리드' as NCHAR VARYING(5) in the EUC-KR encoding, it is processed normally.
	If you specify '큐브리드' as NCHAR VARYING(5) and then use CHAR_LENGTH() function in the EUC-KR encoding, 4 is stored.
	If you specify '큐브리드' as HCHAR VARYING(5) in the utf-8 encoding, an error occurs (UTF-8 character set is not supported).

.. _escape-characters:

Escape Special Characters
-------------------------

CUBRID supports two kinds of methods to escape special characters. One is using quotes and the other is using backslash (\).

**Escape with Quotes**

If you set **no** for the system parameter **ansi_quotes** in the **cubrid.conf** file, you can use both double quotes (") and singe quotes (') to wrap strings. The default value for the **ansi_quotes** parameter is **yes**, and you can use only single quotes to wrap the string. The numbers 2 and 3 below are applied only if you set for the **ansi_quotes** parameter to **no**.

*   You should use two single quotes ('') for the single quotes included in the strings wrapped in single quotes.
*   You should use two double quotes ("") for the double quotes included in the strings wrapped in double quotes.
*   You don't need to escape the single quotes included in the string wrapped in double quotes.
*   You don't need to escape the double quotes included in the string wrapped in single quotes.

**Escape with Backslash**

You can use escape using backslash (\) only if you set no for the system parameter **no_backslash_escapes** in the **cubrid.conf** file. The default value for the **no_backslash_escapes** parameter is **yes**. If the value of **no_backslash_escapes** is **no**, the following are the special characters.

*   \\' : Single quotes (')
*   \\" : Double quotes (")
*   \\n : Newline, linefeed character
*   \\r : Carriage return character
*   \\t : Tab character
*   \\\\ : Backslash
*   \\% : Percent sign (%). For details, see the following description.
*   \\_ : Underbar (_). For details, see the following description.

For all other escapes, the backslash will be ignored. For example, "\x" is the same as entering only "x".

**\\%** and **\\_** are used in the pattern matching syntax such as **LIKE** to search percent signs and underbars and are used as a wildcard character if there is no backslash. Outside of the pattern matching syntax, "\\%"and "\\_" are recognized as normal strings not wildcard characters. For details, see :ref:`like-expr`.

**Example 1**

The following is the result of executing Escape if a value for the system parameter **ansi_quotes** in the **cubrid.conf** file is no, and a value for **no_backslash_escapes** is no.

.. code-block:: sql

	SELECT STRCMP('single quotes test('')', 'single quotes test(\')');
	 
	   strcmp('single quotes test('')', 'single quotes test('')')
	=============================================================
																0
	 
	SELECT STRCMP("\a\b\c\d\e\f\g\h\i\j\k\l\m\n\o\p\q\r\s\t\u\v\w\x\y\z", "a\bcdefghijklm\nopq\rs\tuvwxyz");
	 
	   strcmp('abcdefghijklm
	s       uvwxyz', 'abcdefghijklm
	s       uvwxyz')
	=====================================================================
																		0
	 
	SELECT LENGTH('\\');
	 
	   char_length('\')
	===================
					  1

**Example 2**

The following is the result of executing Escape if a value for the system parameter **ansi_quotes** in the **cubrid.conf** file is yes, and a value for **no_backslash_escapes** is yes.

.. code-block:: sql

	SELECT STRCMP('single quotes test('')', 'single quotes test(\')');
	 
	In the command from line 2,
	ERROR: unterminated string
	 
	In the command from line 2,
	ERROR: syntax error, unexpected UNTERMINATED_STRING
	 
	 
	SELECT STRCMP("\a\b\c\d\e\f\g\h\i\j\k\l\m\n\o\p\q\r\s\t\u\v\w\x\y\z", "a\bcdefghijklm\nopq\rs\tuvwxyz");
	 
	In line 1, column 18,
	ERROR: [\a\b\c\d\e\f\g\h\i\j\k\l\m\n\o\p\q\r\s\t\u\v\w\x\y\z] is not defined.
	 
	In line 1, column 18,
	ERROR: [a\bcdefghijklm\nopq\rs\tuvwxyz] is not defined.
	 
	SELECT LENGTH('\\');
	 
	   char_length('\\')
	====================
					   2

**Example 3**

The following is the result of executing Escape if a value for the system parameter **ansi_quotes** in the **cubrid.conf** file is yes, and a value for **no_backslash_escapes** is no.

.. code-block:: sql

	CREATE TABLE t1 (a varchar(200));
	INSERT INTO t1 VALUES ('aaabbb'), ('aaa%');
	 
	SELECT a FROM t1 WHERE a LIKE 'aaa\%' escape '\\';
	 
	  a
	======================
	  'aaa%'

ENUM Data Type
==============

The **ENUM** type is defined as the enumerated string constants. Only the specified string elements are allowed as the value of the column defined as **ENUM** and the maximum number of the ENUM elements is 65535. In the column of the **ENUM** type, each value is saved as 1 byte when the number of the ENUM elements is less than 256 and 2 bytes when the number is 256 or more. **ENUM** value allows numeric data type or string type.

**ENUM** type column is handled as a number and considered as an index number value, which corresponds to the **ENUM** type when the compared value is **CHAR** / **VARCHAR** in the query. ::

	<enum_type>
		: ENUM '(' <char_string_literal_list> ')'
	<char_string_literal_list>
		: <char_string_literal_list> ',' CHAR_STRING
		| CHAR_STRING

**Example**

The following example shows the definition of the **ENUM** column.

.. code-block:: sql

	CREATE TABLE tbl (
		color ENUM('red', 'yellow', 'blue')
	);

The *color* column can have one of following values:

+-----------+------------------+
| Value     | Index Number     |
+===========+==================+
| NULL      | NULL             |
+-----------+------------------+
| 'red'     | 1                |
+-----------+------------------+
| 'yellow'  | 2                |
+-----------+------------------+
| 'blue'    | 3                |
+-----------+------------------+

The following example shows the insertion of a value into the **ENUM** column.

.. code-block:: sql

	INSERT into tbl values ('yellow'), ('red'), (2), ('blue');

The following example shows the **SELECT** statement that retrieves the **ENUM** column where the value has been inserted in the above. 

.. code-block:: sql

	SELECT color FROM tbl;
	 
	  color
	======================
	  yellow
	  red
	  yellow
	  blue
	 
	SELECT color FROM tbl ORDER BY color ASC;
	 
	  color
	======================
	  red
	  yellow
	  yellow
	  blue
	 
	SELECT color FROM tbl ORDER BY cast(color as char) ASC;
	 
	  color
	======================
	  blue
	  red
	  yellow
	  yellow

* When the string context is used as the **ENUM** value, the string is returned. The following example shows a case of using the string context.

  .. code-block:: sql

	SELECT CONCAT(enum_col, 'color') FROM tbl_name;
 
	  CONCAT(color, '_color')
	======================
	  yellow_color
	  red_color
	  yellow_color
	  blue_color

* When the numeric context is used as the **ENUM** value, the index number is returned. The numeric value can be searched on the **ENUM** column as follows.

  .. code-block:: sql

	SELECT color + 0 FROM tb;
	 
	  color + 0
	======================
	  2
	  1
	  2
	  3

* The result of using the string is different from the result of using the index number. See the following example.

  .. code-block:: sql

	-- will use the ENUM index value because it is compared with a number
	SELECT color FROM tbl WHERE color <= 1;
	 
	  color
	======================
	red
	 
	 
	-- will use the ENUM char literal value because it is compared with a CHAR type
	SELECT color FROM tbl WHERE color <= 'red';
	 
	  color
	======================
	red
	blue

* Index scan of the **ENUM** type column is allowed for **=** and **IN** operators. Index scan cannot process any other comparison operators.

* The value which exceeds the range that the **ENUM** type can express is not converted to the **ENUM** type and an error occurs. For the error data, automatic mapping to the default index value (0) and the default string value (NULL) is not supported.

* When a number inserted in the **ENUM** type column is enclosed within single quotes (' '), if the value is included in the list of the ENUM elements, the value is interpreted as a string value; otherwise, it is interpreted as an index number. Therefore, to avoid confusion, we recommend that you do not use the value similar to the number as the ENUM element value. The following example shows typing an ENUM element value similar to a number in the **ENUM** type column.

  .. code-block:: sql

	CREATE TABLE tb2 (nums enum('0', '1', '2'));
	INSERT INTO tb2 (nums) VALUES(1),('1'),('3');
	SELECT * FROM tb2;
	 
	  nums
	======================
	  0
	  1
	  2

	  
    *   If the entered 1 is not enclosed within single quotes, 0 (corresponds to the Index Number 1) is inserted instead of 1.
    *   When '1' is entered, '1' value is inserted since the corresponding ENUM element value exists.
    *   When '3' is entered, '2' (corresponds to the Index Number 3) is inserted because there is no corresponding ENUM element value and 3 is a valid index number.

* The **ENUM** values are sorted by the index number, not by the string value of the element. **NULL** values are sorted on the front of all strings and blank strings are sorted on the front of any other strings. To sort elements in alphabetic order in the **ENUM** type column, use the **CAST** function as follows.

  .. code-block:: sql

	SELECT color FROM tb ORDER BY cast(color as char) ASC;

* When converting the **ENUM** type to the other type, the index number or the string of the **ENUM** type is converted according to the target type. In the following table, the types with an asterisk (*) can be converted to the **ENUM** type.

  +------------+---------------------------------+
  | Type       | Value (Index Number/String)     |
  +============+=================================+
  | SHORT      | Index Number                    |
  +------------+---------------------------------+
  | INTEGER    | Index Number                    |
  +------------+---------------------------------+
  | BIGINT     | Index Number                    |
  +------------+---------------------------------+
  | FLOAT      | Index Number                    |
  +------------+---------------------------------+
  | DOUBLE     | Index Number                    |
  +------------+---------------------------------+
  | NUMERIC    | Index Number                    |
  +------------+---------------------------------+
  | MONETARY   | Index Number                    |
  +------------+---------------------------------+
  | TIME       | String                          |
  +------------+---------------------------------+
  | DATE       | String                          |
  +------------+---------------------------------+
  | DATETIME   | String                          |
  +------------+---------------------------------+
  | TIMESTAMP  | String                          |
  +------------+---------------------------------+
  | CHAR       | String                          |
  +------------+---------------------------------+
  | VARCHAR    | String                          |
  +------------+---------------------------------+
  | BIT        | String                          |
  +------------+---------------------------------+
  | VARBIT     | String                          |
  +------------+---------------------------------+

**Note**

*   To view all values allowed for the **ENUM** column, use **SHOW COLUMNS**.
*   Each **ENUM** value has its index number based on the order of the sorting elements. The element index number starts at 1.
*   Blank strings can be used as an ENUM element value when operators have explicitly specified a general index number to the blank strings.
* If operators have not specified a general index number to the blank strings, the index number of blank strings is 0. To search the rows with the blank strings, use the following sentence.

  .. code-block:: sql

	SELECT * FROM tb WHERE color=0;

*   In the **ENUM** column declared to allow **NULL**, the index number for **NULL** is **NULL**.
*   The default value of the column, which allows **NULL** is **NULL**. For **NOT NULL**, the default value of the column is the first element of the **ENUM** list specified while defining the column.

*   When a table is created, all trailing blanks of all elements in the **ENUM** column are automatically removed.
*   The cases of the **ENUM** element are not changed but the cases defined while defining the column are maintained as they are.
* For the operation where operands are Type 1 and Type 2, the result type is as follows. The exception of the following rule is the case of comparing the **ENUM** column to the constant value. In this case, the constant value is changed to the **ENUM** value of the same type.

  +------------+------------+-----------------+
  | Type 1     | Type 2     | Result Type     |
  +============+============+=================+
  | SHORT      | ENUM       | SHORT           |
  +------------+------------+-----------------+
  | INTEGER    | ENUM       | INTEGER         |
  +------------+------------+-----------------+
  | BIGINT     | ENUM       | BIGINT          |
  +------------+------------+-----------------+
  | FLOAT      | ENUM       | FLOAT           |
  +------------+------------+-----------------+
  | DOUBLE     | ENUM       | DOUBLE          |
  +------------+------------+-----------------+
  | NUMERIC    | ENUM       | NUMERIC         |
  +------------+------------+-----------------+
  | MONETARY   | ENUM       | MONETARY        |
  +------------+------------+-----------------+
  | TIME       | ENUM       | TIME            |
  +------------+------------+-----------------+
  | DATE       | ENUM       | DATE            |
  +------------+------------+-----------------+
  | DATETIME   | ENUM       | DATETIME        |
  +------------+------------+-----------------+
  | TIMESTAMP  | ENUM       | TIMESTAMP       |
  +------------+------------+-----------------+
  | CHAR       | ENUM       | CHAR            |
  +------------+------------+-----------------+
  | VARCHAR    | ENUM       | VARCHAR         |
  +------------+------------+-----------------+

**Using ENUM Type at the Driver Level**

The **ENUM** type is not specially mapped to various drivers such as JDBC and CCI. Therefore, application developers can use the **STRING** type as they have used. The following example shows the JDBC application.

.. code-block:: java

	Statement stmt = connection.createStatement("SELECT color FROM tbl");
	ResultSet rs = stmt.executeQuery();
	
	while(rs.next()) {
	   System.out.println(rs.getString());
	}

The following example shows the CCI application.

.. code-block:: c

	req_id = cci_prepare (conn, "SELECT color FROM tbl", 0, &err);
	error = cci_execute (req_id, 0, 0, &err);
	if (error < CCI_ER_NO_ERROR)
	{
		/* handle error */
	}
	
	error = cci_cursor (req_id, 1, CCI_CURSOR_CURRENT, &err);
	if (error < CCI_ER_NO_ERROR)
	{
		/* handle error */
	}
	
	error = cci_fetch (req_id, &err);
	if (error < CCI_ER_NO_ERROR)
	{
		/* handle error */
	}
	
	cci_get_data (req, idx, CCI_A_TYPE_STR, &data, 1);

**Constraints**

*   The **ENUM** type column does not allow the **DEFALUT** value.
*   No expressions can be used for the **ENUM** value. For example, the following **CREATE TABLE** statement returns an error.

  .. code-block:: sql

	CREATE TABLE tb (
		color ENUM('red', CONCAT('light ','gray'), 'blue')
	);

BLOB/CLOB Data Types
====================

An External **LOB** type is data to process Large Object, such as text or images. When LOB-type data is created and inserted, it will be stored in a file to an external storage, and the location information of the relevant file (**LOB** Locator) will be stored in the CUBRID database. If the **LOB** Locator is deleted from the database, the relevant file that was stored in the external storage will be deleted as well. CUBRID supports the following two types of **LOB** :

*   Binary Large Object (**BLOB**)
*   Character Large Object (**CLOB**)

**Related Terms**

*   **LOB** (Large Object) : Large-sized objects such as binaries or text.
*   **FBO** (File Based Object) : An object that stores data of the database in an external file.
*   **External LOB** : An object better known as FBO, which stores **LOB** data in a file into an external DB. It is supported by CUBRID. Internal **LOB** is an object that stores **LOB** data inside the DB.
*   **External Storage** : An external storage to store LOB (example : POSIX file system).
*   **LOB Locator** : The path name of a file stored in external storage.
*   **LOB Data** : Details of a file in a specific location of LOB Locator.

**File Names**

When storing LOB data in external storage, the following naming convention will be applied: ::

	{table_name}_{unique_name}
	
*   *table_name* : It is inserted as a prefix and able to store the **LOB** data of many tables in one external storage.
*   *unique_name* : The random name created by the DB server.

**Default Storage**

*   **LOB** data is stored in the local file system of the DB server. LOB data is stored in the path specified in the **-lob-base-path option** value of **cubrid createdb**; if this value is omitted, the data will be stored in the [db-vol path]/lob path where the database volume will be created. For more details, see :ref:`creating-database` and :ref:`lob_storage`.

*   If the relevant path is deleted despite a **LOB** data file path being registered in the database location file (**databases.txt**), please note that the utility that operates in database server (**cub_server**) and standalone will not function normally.

BLOB/CLOB
---------

**BLOB**

*   A type that stores binary data outside the database.
*   The maximum length of **BLOB** data is the maximum file size creatable in an external storage.
*   In SQL statements, the **BLOB** type expresses the input and output value in a bit array. That is, it is compatible with the **BIT** (n) and **BIT VARYING** (n) types, and only an explicit type change is allowed. If data lengths differ from one another, the maximum length is truncated to fit the smaller one.

*   When converting the **BLOB** type value to a binary value, the length of the converted data cannot exceed 1GB. When converting binary data to the **BLOB** type, the size of the converted data cannot exceed the maximum file size provided by the **BLOB** storage.

**CLOB**

*   A type that stores character string data outside the database.
*   The maximum length of **CLOB** data is the maximum file size creatable in an external storage.
*   In SQL statements, the CLOB type expresses the input and output value in a character string. That is, it is compatible with the **CHAR** (n), **VARCHAR** (n), **NCHAR** (n), **NCHAR VARYING** (n) types. However, only an explicit type change is allowed, and if data lengths are different from one another, the maximum length is truncated to fit to the smaller one.

*   When converting the **CLOB** type value to a character string, the length of the converted data cannot exceed 1 GB. When converting a character string to the **CLOB** type, the size of the converted data cannot exceed the maximum file size provided by the **CLOB** storage.

Creating and Altering Columns
-----------------------------

**BLOB** / **CLOB** type columns can be created/added/deleted by using a **CREATE TABLE** statement or an **ALTER TABLE** statement.

*   You cannot create the index file for a **LOB** type column.
*   You cannot define the **PRIMARY KEY**, **FOREIGN KEY**, **UNIQUE**, **NOT NULL** constraints for a **LOB** type column. However, **SHARED** property cannot be defined and **DEFAULT** property can only be defined by the **NULL** value.

*   **LOB** type column/data cannot be the element of collection type.
*   If you are deleting a record containing a **LOB** type column, all files located inside a **LOB** column value (Locator) and the external storage will be deleted. When a record containing a LOB type column is deleted in a basic key table, and a record of a foreign key table that refers to the foregoing details is deleted at once, all **LOB** files located in a **LOB** column value (Locator) and the external storage will be deleted. However, if the relevant table is deleted by using a **DROP TABLE** statement, or a **LOB** column is deleted by using an **ALTER TABLE...DROP** statement, only a **LOB** column value (**LOB** Locator) is deleted, and the **LOB** files inside the external storage which a **LOB** column refers to will not be deleted.

**Example**

.. code-block:: sql

	-- creating a table and CLOB column
	CREATE TABLE doc_t (doc_id VARCHAR(64) PRIMARY KEY, content CLOB);
	 
	-- an error occurs when UNIQUE constraint is defined on CLOB column
	ALTER TABLE doc_t ADD CONSTRAINT content_unique UNIQUE(content);
	 
	-- an error occurs when creating an index on CLOB column
	CREATE INDEX ON doc_t (content);
	 
	-- creating a table and BLOB column
	CREATE TABLE image_t (image_id VARCHAR(36) PRIMARY KEY, doc_id VARCHAR(64) NOT NULL, image BLOB);
	 
	-- an error occurs when adding a BOLB column with NOT NULL constraint
	ALTER TABLE image_t ADD COLUMN thumbnail BLOB NOT NULL;
	 
	-- an error occurs when adding a BLOB column with DEFAULT attribute
	ALTER TABLE image_t ADD COLUMN thumbnail2 BLOB DEFAULT BIT_TO_BLOB(X'010101');	

Storing and Updating Columns
----------------------------

In a **BLOB** / **CLOB** type column, each **BLOB** / **CLOB** type value is stored, and if binary or character string data is input, you must explicitly change the types by using each **BIT_TO_BLOB** / **CHAR_TO_CLOB** function.

If a value is input in a **LOB** column by using an **INSERT** statement, a file is created in an external storage internally and the relevant data is stored; the relevant file path (Locator) is stored in an actual column value.

If a record containing a **LOB** column uses a **DELETE** statement, a file to which the relevant **LOB** column refers will be deleted simultaneously. If a **LOB** column value is changed using an **UPDATE** statement, the column value will be changed following the operation below, according to whether a new value is **NULL** or not.

*   If a **LOB** type column value is changed to a value that is not **NULL** : If a Locator that refers to an external file is already available in a **LOB** column, the relevant file will be deleted. A new file is created afterwards. After storing a value that is not **NULL**, a Locator for a new file will be stored in a **LOB** column value.

*   If changing a **LOB** type column value to **NULL** : If a Locator that refers to an external file is already available in a **LOB** column, the relevant file will be deleted. And then **NULL** is stored in a **LOB** column value.

**Example**

.. code-block:: sql

	-- inserting data after explicit type conversion into CLOB type column
	INSERT INTO doc_t (doc_id, content) VALUES ('doc-1', CHAR_TO_CLOB('This is a Dog'));
	INSERT INTO doc_t (doc_id, content) VALUES ('doc-2', CHAR_TO_CLOB('This is a Cat'));
	 
	-- inserting data after explicit type conversion into BLOB type column
	INSERT INTO image_t VALUES ('image-0', 'doc-0', BIT_TO_BLOB(X'000001'));
	INSERT INTO image_t VALUES ('image-1', 'doc-1', BIT_TO_BLOB(X'000010'));
	INSERT INTO image_t VALUES ('image-2', 'doc-2', BIT_TO_BLOB(X'000100'));
	 
	-- inserting data from a sub-query result
	INSERT INTO image_t SELECT 'image-1010', 'doc-1010', image FROM image_t WHERE image_id = 'image-0';
	 
	-- updating CLOB column value to NULL
	UPDATE doc_t SET content = NULL WHERE doc_id = 'doc-1';
	 
	-- updating CLOB column value
	UPDATE doc_t SET content = CHAR_TO_CLOB('This is a Dog') WHERE doc_id = 'doc-1';
	 
	-- updating BLOB column value
	UPDATE image_t SET image = (SELECT image FROM image_t WHERE image_id = 'image-0') WHERE image_id = 'image-1';
	 
	-- deleting BLOB column value and its referencing files
	DELETE FROM image_t WHERE image_id = 'image-1010';

Getting Column Values
---------------------

When you get a **LOB** type column, the data stored in a file to which the column refers will be displayed. You can execute an explicit type change by using **CAST** operator, **CLOB_TO_CHAR** function, and **BLOB_TO_BIT** function.

*   If the query is executed in CSQL, a column value (Locator) will be displayed, instead of the data stored in a file. To display the data to which a **BLOB** / **CLOB** column refers, it must be changed to strings by using **CLOB_TO_CHAR** function.

*   To use the string process function, the strings need to be converted by using the **CLOB_TO_CHAR** function.
*   You cannot specify a **LOB** column in ** GROUP BY** clause and **ORDER BY** clause.
*   Comparison operators, relational operators, **IN**, **NOT IN** operators cannot be used to compare **LOB** columns. However, **IS NULL** expression can be used to compare whether it is a **LOB** column value (Locator) or **NULL**. This means that **TRUE** will be returned when a column value is **NULL**, and if a column value is **NULL**, there is no file to store **LOB** data.

*   When a **LOB** column is created, and the file is deleted after data input, a **LOB** column value (Locator) will become a state that is referring to an invalid file. As such, using **CLOB_TO_CHAR**, **BLOB_TO_BIT**, **CLOB_LENGTH**, and **BLOB_LENGTH** functions on the columns that have mismatching **LOB** Locator and a **LOB** data file enables them to display **NULL**.

**Example**

.. code-block:: sql

	-- displaying locator value when selecting CLOB and BLOB column in CSQL interpreter
	SELECT doc_t.doc_id, content, image FROM doc_t, image_t WHERE doc_t.doc_id = image_t.doc_id;
	 
	  doc_id                content               image
	==================================================================
	  'doc-1'               file:/home1/data1/ces_658/doc_t.00001282208855807171_7329  file:/ home1/data1/ces_318/image_t.00001282208855809474_7474
	  'doc-2'               file:/home1/data1/ces_180/doc_t.00001282208854194135_5598  file:/
	home1/data1/ces_519/image_t.00001282208854205773_1215
	 
	2 rows selected.
	 
	-- using string functions after coercing its type by CLOB_TO_CHAR( )
	SELECT CLOB_TO_CHAR(content), SUBSTRING(CLOB_TO_CHAR(content), 10) FROM doc_t;
	 
	   clob_to_char(content)  substring( clob_to_char(content) from 10)
	============================================
	  'This is a Dog'       ' Dog'
	  'This is a Cat'       ' Cat'
	 
	2 rows selected.
	 
	SELECT CLOB_TO_CHAR(content) FROM doc_t WHERE CLOB_TO_CHAR(content) LIKE '%Dog%';
	 
	   clob_to_char(content)
	======================
	  'This is a Dog'
	 
	SELECT CLOB_TO_CHAR(content) FROM doc_t ORDER BY CLOB_TO_CHAR(content)
	 
	   clob_to_char(content)
	======================
	  'This is a Cat'
	  'This is a Dog'
	 
	-- an error occurs when LOB column specified in WHERE/ORDER BY/GROUP BY clauses
	SELECT * FROM doc_t WHERE content LIKE 'This%';
	SELECT * FROM doc_t ORDER BY content;

Functions and Operators
-----------------------

**CAST Operator**

By using **CAST** operator, you can execute an explicit type change between **BLOB** / **CLOB** type and binary type/string type. For more details, see :func:`CAST`. ::

	CAST (<bit_type_column_or_value> AS CLOB)
	CAST (<bit_type_column_or_value> AS BLOB)
	CAST (<char_type_column_or_value> AS BLOB)
	CAST (<char_type_column_or_value> AS CLOB)

**LOB Data Process and Type Change Functions**

The next table shows the functions provided to process and change BLOB/CLOB types.

+------------------------------+-----------------------------------------------------------------------------------------------------------------+
| Functional Expression        | Description                                                                                                     |
|                              |                                                                                                                 |
+==============================+=================================================================================================================+
| **CLOB_TO_CHAR**             | Changes number type, date/time type, and                                                                        |
| (<                           | **CLOB**                                                                                                        |
| *clob_type_column*           | type to                                                                                                         |
| >)                           | **VARCHA**                                                                                                      |
|                              | R type.                                                                                                         |
|                              |                                                                                                                 |
+------------------------------+-----------------------------------------------------------------------------------------------------------------+
| **BLOB_TO_BIT**              | Changes                                                                                                         |
| (<                           | **BLOB**                                                                                                        |
| *blob_type_column*           | type to                                                                                                         |
| >)                           | **VARYING BIT**                                                                                                 |
|                              | type.                                                                                                           |
|                              |                                                                                                                 |
+------------------------------+-----------------------------------------------------------------------------------------------------------------+
| **CHAR_TO_CLOB**             | Changes text string type (                                                                                      |
| (<                           | **CHAR**                                                                                                        |
| *char_type_column_or_value*  | ,                                                                                                               |
| >)                           | **VARCHAR**                                                                                                     |
|                              | ,                                                                                                               |
|                              | **NCHAR**                                                                                                       |
|                              | ,                                                                                                               |
|                              | **NVACHAR**                                                                                                     |
|                              | ) to                                                                                                            |
|                              | **CLOB**                                                                                                        |
|                              | type.                                                                                                           |
|                              |                                                                                                                 |
+------------------------------+-----------------------------------------------------------------------------------------------------------------+
| **BIT_TO_BLOB**              | Changes bit array type (                                                                                        |
| (<                           | **BIT**                                                                                                         |
| *blob_type_column_or_value*  | ,                                                                                                               |
| >)                           | **VARYING BIT**                                                                                                 |
|                              | ) to                                                                                                            |
|                              | **BLOB**                                                                                                        |
|                              | type.                                                                                                           |
|                              |                                                                                                                 |
+------------------------------+-----------------------------------------------------------------------------------------------------------------+
| **CHAR_TO_BLOB**             | Changes text string type (                                                                                      |
| (<                           | **CHAR**                                                                                                        |
| *char_type_colulmn_or_value* | ,                                                                                                               |
| >)                           | **VARCHAR**                                                                                                     |
|                              | ,                                                                                                               |
|                              | **NCHAR**                                                                                                       |
|                              | ,                                                                                                               |
|                              | **NVACHAR**                                                                                                     |
|                              | ) to                                                                                                            |
|                              | **BLOB**                                                                                                        |
|                              | type.                                                                                                           |
|                              |                                                                                                                 |
+------------------------------+-----------------------------------------------------------------------------------------------------------------+
| **CLOB_FROM_FILE**           | Reads file details from the file path of                                                                        |
| (<                           | **VARCHAR**                                                                                                     |
| *file_pathname*              | type and changes to                                                                                             |
| >)                           | **CLOB**                                                                                                        |
|                              | type data. <                                                                                                    |
|                              | *file_pathname*                                                                                                 |
|                              | > is analyzed to a path of server which is operated by the DB client, such as CAS or CSQL.                      |
|                              | If a path is specified targeting this, the upper path will be the current work direction of the process.        |
|                              | The statement that calls this function will not cache execution plans.                                          |
+------------------------------+-----------------------------------------------------------------------------------------------------------------+
| **BLOB_FROM_FILE**           | Reads file details from the file path of                                                                        |
| (<                           | **VARCHAR**                                                                                                     |
| *file_pathname*              | type, and changes to BLOB type data. The file path specified in is interpreted using the same method as the     |
| >)                           | **CLOB_FROM_FILE**                                                                                              |
|                              | function.                                                                                                       |
|                              |                                                                                                                 |
+------------------------------+-----------------------------------------------------------------------------------------------------------------+
| **CLOB_LENGTH**              | Returns the length of LOB data stored in a                                                                      |
| (<                           | **CLOB**                                                                                                        |
| *clob_column*                | file in bytes.                                                                                                  |
| >)                           |                                                                                                                 |
|                              |                                                                                                                 |
+------------------------------+-----------------------------------------------------------------------------------------------------------------+
| **BLOB_LENGTH**              | Returns the length of LOB data stored in a                                                                      |
| (<                           | **BLOB**                                                                                                        |
| *blob_column*                | file in bytes.                                                                                                  |
| >)                           |                                                                                                                 |
|                              |                                                                                                                 |
+------------------------------+-----------------------------------------------------------------------------------------------------------------+
| <                            | Use an                                                                                                          |
| *blob_or_clob_column*        | **IS NULL**                                                                                                     |
| >                            | expression to compare whether it is a                                                                           |
| **IS NUL**                   | **LOB**                                                                                                         |
| L                            | column value (Locator) or                                                                                       |
|                              | **NULL**                                                                                                        |
|                              | ; returns                                                                                                       |
|                              | **TRUE**                                                                                                        |
|                              | if                                                                                                              |
|                              | **NULL**                                                                                                        |
|                              | .                                                                                                               |
|                              |                                                                                                                 |
+------------------------------+-----------------------------------------------------------------------------------------------------------------+

.. _lob_storage:

Creating and Managing Storage
-----------------------------

**LOB File Path Specification**

By default, the **LOB** data file is stored in the <db-volumn-path>/lob directory where database volume is created. However, if the **--lob-base-path** option of **cubrid createdb** utility is used when creating the database, a **LOB** data file can be stored in the directory specified by option value. However, if there is no directory specified by option value, attempt to create a directory, and display an error message if it fails to create the directory. For more details, see the **--lob-base-path** option in :option:`createdb -B`. ::

	#image_db volume is created in the current work directory, and a LOB data file will be stored.
	cubrid createdb image_db

	#LOB data file is stored in the "/home1/data1" path within a local file system.
	cubrid createdb --lob-base-path="file:/home1/data1" image_db

**Checking LOB File Store Directory** ::

	#You can check a directory where a LOB file will be stored by executing the cubrid spacedb utility.
	cubrid spacedb image_db

	Space description for database 'image_db' with pagesize 16.0K. (log pagesize: 16.0K)

	Volid  Purpose  total_size  free_size  Vol Name

	    0  GENERIC      512.0M     510.1M  /home1/data1/image_db

	Space description for temporary volumes for database 'image_db' with pagesize 16.0K.

	Volid  Purpose  total_size  free_size  Vol Name

	LOB space description file:/home1/data1

**Changing or Expanding LOB File Store Directory**

Secure disk space to create additional file storage, expand the **lob-base-path** of **databases.txt**, and change to the disk location. Restart the database server to apply the changes made to **databases.txt**. However, even if you change the **lob-base-path** of **databases.txt**, access to the **LOB** data stored in a previous storage is possible. ::

	# You can change to a new directory from the lob-base-path of databases.txt file.
	sh> cat $CUBRID_DATABASES/databases.txt

	#db-name         vol-path             db-host         log-path         lob-base-path    
	image_db         /home1/data1         localhost       /home1/data1     file:/home1/data2

**Backing up and Recovering of LOB Files**

While backup/recovery is not supported for **LOB** type columns, meta data (Locator) of the **LOB** type columns is supported with such service.

**Copying Database with LOB Files**

If you are copying a database by using the **cubrid copydb** utility, you must configure the **databases.txt** additionally, as the **LOB** file directory path will not be copied if the related option is not specified. For more details, see the :option:`copydb -B` and :option:`copydb --copy-lob-path` options.

Supporting and Recovering Transactions
--------------------------------------

Commit/rollback for **LOB** data changes are supported. That is, it ensures the validation of mapping between **LOB** Locator and actual **LOB** data within transactions, and it supports recovery during DB errors. This means that an error will be displayed in case of mapping errors between **LOB** Locator and **LOB** data due to the rollback of the relevant transactions, as the database is terminated during transactions. See the example below.

**Example**

.. code-block:: sql

	;AUTOCOMMIT OFF
	 
	CREATE TABLE doc_t (doc_id VARCHAR(64) PRIMARY KEY, content CLOB);
	INSERT INTO doc_t VALUES ('doc-10', CHAR_TO_CLOB('This is content'));
	COMMIT;
	UPDATE doc_t SET content = CHAR_TO_CLOB('This is content 2') where doc_id = 'doc-10';
	ROLLBACK;
	SELECT doc_id, CLOB_TO_CHAR(content) FROM doc_t WHERE doc_id = 'doc-10';
	  doc_id   content                  
	=========================================================
	  'doc-10'  'This is content '
	 
	INSERT INTO doc_t VALUES ('doc-11', CHAR_TO_CLOB ('This is content'));
	COMMIT;
	UPDATE doc_t SET content = CHAR_TO_CLOB('This is content 3') WHERE doc_id = 'doc-11';
	 
	-- system crash occurred and then restart server
	SELECT doc_id, CLOB_TO_CHAR(content) FROM doc_t WHERE doc_id = 'doc-11';
	 
	-- Error : LOB Locator references to the previous LOB data because only LOB Locator is rollbacked.

**Remark**

*   When selecting **LOB** data in an application through a driver such as JDBC, the driver can get **ResultSet** from DB server and fetch the record while changing the cursor location on **Resultset**. That is, only Locator, the meta data of a **LOB** column, is stored at the time when **ResultSet** is imported, and **LOB** data that is referred by a File Locator will be fetched from the file Locator at the time when a record is fetched. Therefore, if **LOB** data is updated between two different points of time, there could be an error, as the mapping of **LOB** Locator and actual **LOB** data will be invalid.

*   Since backup/recovery is supported only for meta data (Locator) of the **LOB** type columns, an error is likely to occur, as the mapping of **LOB** Locator and LOB data is invalid if recovery is performed based on a specific point of time.

*   TO execute **INSERT** the **LOB** data into other device, LOB data referred by the meta data (Locator) of a **LOB** column must be read.

*   In a CUBRID HA environment, the meta data (Locator) of a  **LOB** column is replicated and data of a **LOB** type is not replicated. Therefore, if storage of a **LOB** type is located on the local machine, no tasks on the columns in a slave node or a master node after failover are allowed.

.. warning::

	Up to CUBRID 2008 R3.0, Large Objects are processed by using **glo** (Generalized Large Object) classes. However, the **glo** classes has been deprecated since the CUBRID 2008 R3.1. Instead of it, **LOB** / **CLOB** data type is supported. Therefore, both DB schema and application must be modified when upgrading CUBRID in an environment using the previous version of **glo** classes.

Collection Types
================

Allowing multiple data values to be stored in a single attribute is an extended feature of relational database. Each element of a collection is possible to have different data type each other except View.

+--------------+-----------------------------------------------------------------------+---------------------------------+----------------------------+----------------------------+
| Type         | Description                                                           | Definition                      | Input Data                 | Stored Data                |
+==============+=======================================================================+=================================+============================+============================+
| **SET**      | A union which does not allow duplicates                               | col_name SET VARCHAR(20)        | {'c','c','c','b','b','a'}  | {'a','b','c'}              |
|              |                                                                       | col_name SET (VARCHAR(20))      | {'c','c','c','b','b', 'a'} | {'a','b','c'}              |
+--------------+-----------------------------------------------------------------------+---------------------------------+----------------------------+----------------------------+
| **MULTISET** | A union which allows duplicates                                       | col_name MULTISET VARCHAR(20)   | {'c','c','c','b','b','a'}  | {'a','b','b','c','c','c'}  |
|              |                                                                       | col_name MULTISET (VARCHAR(20)) | {'c','c','c','b','b','a'}  | {'a','b','b', 'c','c','c'} |
+--------------+-----------------------------------------------------------------------+---------------------------------+----------------------------+----------------------------+
| **LIST** or  | A union which allows duplicates and stores data in the order of input | col_name LIST VARCHAR(20)       | {'c','c','c','b','b','a'}  | {'c','c','c','b','b','a'}  |
| **SEQUENCE** |                                                                       | col_name LIST (VARCHAR(20))     | {'c','c','c','b','b', 'a'} | {'c','c','c','b','b','a'}  |
+--------------+-----------------------------------------------------------------------+---------------------------------+----------------------------+----------------------------+

As you see the table above, the value specified as a collection type can be inputted with curly braces ('{', '}') each value is separated with a comma (,).

**Coercions**

If the specified collection types are identical, the collection types can be cast explicitly by using the **CAST** operator. The following table shows the collection types that allow explicit coercions.

**Explicit Coercions**

+----------+----------------------------------+
|          | TO                               |
+==========+==========+=====+==========+======+
| FROM     |          | SET | MULTISET | LIST |
|          +----------+-----+----------+------+
|          | SET      | -   | O        | O    |
|          +----------+-----+----------+------+
|          | MULTISET | O   | -        | X    |
|          +----------+-----+----------+------+
|          | LIST     | O   | O        | -    |
+----------+----------+-----+----------+------+

SET
---

**SET** is a collection type in which each element has different values. Elements of a **SET** are allowed to have only one data type. It can have records of other tables.

**Example**

.. code-block:: sql

	CREATE TABLE set_tbl ( col_1 set(CHAR(1)));
	INSERT INTO set_tbl VALUES ({'c','c','c','b','b','a'});
	INSERT INTO set_tbl VALUES ({NULL});
	INSERT INTO set_tbl VALUES ({''});
	SELECT * FROM set_tbl;
	  col_1
	======================
	{'a', 'b', 'c'}
	{NULL}
	{' '}
	 
	SELECT CAST(col_1 AS MULTISET), CAST(col_1 AS LIST) FROM set_tbl;
	   cast(col_1 as multiset)   cast(col_1 as sequence)
	============================================
	  {'a', 'b', 'c'}  {'a', 'b', 'c'}
	  {NULL}  {NULL}
	  {' '}  {' '}
	 
	INSERT INTO set_tbl VALUES ('');
	 
	ERROR: Cannot coerce '' to type set.

**MULTISET**

**MULTISET** is a collection type in which duplicated elements are allowed. Elements of a **MULTISET** are allowed to have only one data type. It can have records of other tables.

**Example**

.. code-block:: sql

	CREATE TABLE multiset_tbl ( col_1 multiset(CHAR(1)));
	INSERT INTO multiset_tbl VALUES ({'c','c','c','b','b', 'a'});
	SELECT * FROM multiset_tbl;
	  col_1
	======================
	  {'a', 'b', 'b', 'c', 'c', 'c'}
	 
	SELECT CAST(col_1 AS SET), CAST(col_1 AS LIST) FROM multiset_tbl;
	   cast(col_1 as set)   cast(col_1 as sequence)
	============================================
	  {'a', 'b', 'c'}  {'c', 'c', 'c', 'b', 'b', 'a'}
  
**LIST/SEQUENCE**

**LIST** (= **SEQUENCE**) is a collection type in which the input order of elements is preserved, and duplications are allowed. Elements of a **LIST** are allowed to have only one data type. It can have records of other tables.

**Example**

.. code-block:: sql

	CREATE TABLE list_tbl ( col_1 list(CHAR(1)));
	INSERT INTO list_tbl VALUES ({'c','c','c','b','b', 'a'});
	SELECT * FROM list_tbl;
	  col_1
	======================
	  {'c', 'c', 'c', 'b', 'b', 'a'}
	 
	SELECT CAST(col_1 AS SET), CAST(col_1 AS MULTISET) FROM list_tbl;
	   cast(col_1 as set)  cast(col_1 as multiset)
	============================================
	  {'a', 'b', 'c'}  {'a', 'b', 'b', 'c', 'c', 'c'}

.. _implicit-type-conversion:

Implicit Type Conversion
========================

An implicit type conversion represents an automatic conversion of a type of expression to a corresponding type. 

**SET**, **MULTISET**, **LIST** and **SEQUENCE** should be converted explicitly.

If you convert the **DATETIME** and the **TIMESTAMP** types to the **DATE** type or the **TIME** type, data loss may occur. If you convert the **DATE** type to the **DATETIME** type or the **TIMESTAMP** type, the time will be set to '12:00:00 AM.'

If you convert a string type or an exact numeric type to a floating-point numeric type, the value may not be accurate. Because a string type and an exact type use a decimal precision to represent the value, but a floating-point numeric type uses a binary precision.

The implicit type conversion executed by CUBRID is as follows:

**Implicit Type Conversion Table 1**

+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| From \ To     | DATETIME     | DATE     | TIME     | TIMESTAMP     | DOUBLE     | FLOAT     | NUMERIC     | BIGINT     |
+===============+==============+==========+==========+===============+============+===========+=============+============+
| **DATETIME**  | -            | O        | O        | O             |            |           |             |            |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **DATE**      | O            | -        |          | O             |            |           |             |            |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **TIME**      |              |          | -        |               |            |           |             |            |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **TIMESTAMP** | O            | O        | O        | -             |            |           |             |            |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **DOUBLE**    |              |          |          |               | -          | O         | O           | O          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **FLOAT**     |              |          |          |               | O          | -         | O           | O          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **NUMERIC**   |              |          |          |               | O          | O         | -           | O          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **BIGINT**    |              |          |          |               | O          | O         | O           | -          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **INT**       |              |          |          | O             | O          | O         | O           | O          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **SHORT**     |              |          |          |               | O          | O         | O           | O          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **MONETARY**  |              |          |          |               | O          | O         | O           | O          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **BIT**       |              |          |          |               |            |           |             |            |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **VARBIT**    |              |          |          |               |            |           |             |            |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **CHAR**      | O            | O        | O        | O             | O          | O         | O           | O          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **VARCHAR**   | O            | O        | O        | O             | O          | O         | O           | O          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **NCHAR**     | O            | O        | O        | O             | O          | O         | O           | O          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+
| **VARNCHAR**  | O            | O        | O        | O             | O          | O         | O           | O          |
+---------------+--------------+----------+----------+---------------+------------+-----------+-------------+------------+

**Implicit Type Conversion Table 2**

+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| From \ To     | INT     | SHORT     | MONETARY     | BIT     | VARBIT     | CHAR     | VARCHAR     | NCHAR     | VARNCHAR     |
+===============+=========+===========+==============+=========+============+==========+=============+===========+==============+
| **DATETIME**  |         |           |              |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **DATE**      |         |           |              |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **TIME**      |         |           |              |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **TIMESTAMP** |         |           |              |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **DOUBLE**    | O       | O         | O            |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **FLOAT**     | O       | O         | O            |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **NUMERIC**   | O       | O         | O            |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **BIGINT**    | O       | O         | O            |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **INT**       | -       | O         | O            |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **SHORT**     | O       | -         | O            |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **MONETARY**  | O       | O         | -            |         |            | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **BIT**       |         |           |              | -       | O          | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **VARBIT**    |         |           |              | O       | -          | O        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **CHAR**      | O       | O         | O            | O       | O          | -        | O           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **VARCHAR**   | O       | O         | O            | O       | O          | O        | -           | O         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **NCHAR**     | O       | O         | O            | O       | O          | O        | O           | -         | O            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+
| **VARNCHAR**  | O       | O         | O            | O       | O          | O        | O           | O         | -            |
+---------------+---------+-----------+--------------+---------+------------+----------+-------------+-----------+--------------+

Conversation Rules
------------------

**INSERT and UPDATE**

The type will be converted to the type of the column affected.

.. code-block:: sql

	CREATE TABLE t(i INT);
	INSERT INTO t VALUES('123');
	 
	SELECT * FROM t;
	 
				i
	=============
			  123

**Function**

If the parameter value entered in the function can be converted to the specified type, the parameter type will be converted. The strings are converted to numbers because the input parameter expected in the following function is a number.

.. code-block:: sql

	SELECT MOD('123','2');
	 
			   mod('123', '2')
	==========================
		 1.000000000000000e+00

You can enter multiple type values in the function. If the type value not specified in the function is delivered, the type will be converted depending on the following priority order.

*   Date/Time Type ( **DATETIME** > **TIMESTAMP** > **DATE** > **TIME** )
*   Approximate Numeric Type ( **MONETARY** > **DOUBLE** > **FLOAT** )
*   Exact Numeric Type ( **NUMERIC** > **BIGINT** > **INT** > **SHORT** )
*   String Type ( **CHAR** / **NCHAR** > **VARCHAR** / **VARNCHAR** )

**Comparison Operation**

The following are the conversion rules according to an operand type of the comparison operator.

+-------------------+-------------------+----------------------------------------------+----------------+
| operand1 Type     | operand2 Type     | Conversion                                   | Comparison     |
+===================+===================+==============================================+================+
| Numeric Type      | Numeric Type      | None                                         | NUMERIC        |
|                   +-------------------+----------------------------------------------+----------------+
|                   | String Type       | Converts operand2 to                         | NUMERIC        |
|                   |                   | **DOUBLE**                                   |                |
|                   +-------------------+----------------------------------------------+----------------+
|                   | Date/Time Type    | None                                         | N/a            |
+-------------------+-------------------+----------------------------------------------+----------------+
| String Type       | Numeric Type      | Converts operand1 to                         | NUMERIC        |
|                   |                   | **DOUBLE**                                   |                |
|                   +-------------------+----------------------------------------------+----------------+
|                   | String Type       | None                                         | String         |
|                   +-------------------+----------------------------------------------+----------------+
|                   | Date/Time Type    | Converts operand1 to date/time type          | Date/Time      |
+-------------------+-------------------+----------------------------------------------+----------------+
| Date/Time Type    | Numeric Type      | None                                         | N/A            |
|                   +-------------------+----------------------------------------------+----------------+
|                   | String Type       | Converts operand2 to date/time type          | Date/Time      |
|                   +-------------------+----------------------------------------------+----------------+
|                   | Date/Time Type    | Converts it to the type with higher priority | Date/Time      |
+-------------------+-------------------+----------------------------------------------+----------------+

The following are the exceptions in the conversion rules for comparison operators:

*   COLUMN <operator> value

+-------------------+-------------------+--------------------------------------+----------------+
| operand1 Type     | operand2 Type     | Conversion                           | Comparison     |
+===================+===================+======================================+================+
| String type       | Numeric type      | Converts operand2 to the string type | String         |
|                   +-------------------+--------------------------------------+----------------+
|                   | Date/Time type    | Converts operand2 to the string type | String         |
+-------------------+-------------------+--------------------------------------+----------------+

If operand2 is a set operator( **IS IN**, **IS NOT IN**, **= ALL**, **= ANY**, **< ALL**, **< ANY**, **<= ALL**, **<= ANY**, **>= ALL**, **>= ANY** ), the exception above is not applied.

**Numeric Type & String Type Operands**

The string type operand will be converted to **DOUBLE**.

.. code-block:: sql

	CREATE TABLE t(i INT, s STRING);
	INSERT INTO t VALUES(1,'1'),(2,'2'),(3,'3'),(4,'4'), (12,'12');
	 
	SELECT i FROM t WHERE i < '11.3';
	 
				i
	=============
				1
				2
				3
				4
	 
	SELECT ('2' <= 11);
	 
		 ('2'<11)
	=============
				1

**String Type & Date/Time Type Operands**

The string type operand will be converted to the date/time type.

.. code-block:: sql

	SELECT ('2010-01-01' < date'2010-02-02');
	 
	   ('2010-01-01'<date '2010-02-02')
	==================================
									1
	 
	SELECT (date'2010-02-02' >= '2010-01-01');
	 
	  (date '2010-02-02'>='2010-01-01')
	===================================
									1

**String Type & Numeric Type Host Variable Operands**

The numeric type host variable will be converted to the string type.

.. code-block:: sql

	PREPARE s FROM 'SELECT s FROM t WHERE s < ?';
	EXECUTE s USING 11;
		   s
	===================
		 '1'

**String Type & Numeric Type value Operands**

The numeric type value will be converted to the string type.

.. code-block:: sql

	SELECT s FROM t WHERE s > 11;
		   s
	==================
		 '2'
		 '3'
		 '4'
		 '12'
	 
	SELECT s FROM t WHERE s BETWEEN 11 AND 33;
			s
	======================
		  '2'
		  '3'
		  '12'
	  
**String Type Column & Date/Time Type Value Operands**

The date/time type value will be converted to the string type.

.. code-block:: sql

	SELECT s FROM t;
	 
			   s
	======================
		'01/01/1998'
		'01/01/1999'
		'01/01/2000'
	 
	SELECT s FROM t WHERE s <= date'02/02/1998';
				s
	======================
		'01/01/1998'
		'01/01/1999'
		'01/01/2000'

**Range Operation**

**Numeric Type and String Type Operands**

The string type operand will be converted to **DOUBLE**.

.. code-block:: sql

	SELECT i FROM t WHERE i <= all {'11','12'};
	 
				i
	=============
				1
				2
				3
				4

**String Type and Date/Time Type Operands**

The string type operand will be converted to the date/time type.

.. code-block:: sql

	SELECT s FROM t2;
	 
			  s
	======================
		  '01/01/2000'
		  '01/01/1999'
		  '01/01/1998'
	 
	SELECT s FROM t2 WHERE s <= ALL {date'02/02/1998',date'01/01/2000'};
	 
			  s
	======================
		   '01/01/1998'

An error will be returned if it cannot be converted to the corresponding type.

**Arithmetic Operation**

**Date/Time Type Operand**

If the date/time type operands are given to '-' operator and the types are different from each other, it will be converted to the type with a higher priority. The following example shows that the operand data type on the left is converted from **DATE** to **DATETIME** so that the result of '-' operation of **DATETIME** can be outputted in milliseconds.

.. code-block:: sql

	SELECT date'2002-01-01' - datetime'2001-02-02 12:00:00 am';

	   date '2002-01-01'- datetime '2001-02-02 12:00:00 am'
	=====================================================
											  28771200000

**Numeric Type Operand**

If the numeric type operands are given and the types are different from each other, it will be converted to the type with the higher priority.

**Date/Time Type & Numeric Type Operands**

If the date/time type and the numeric type operands are given to '+' or '-' operator, the numeric type operand is converted to either **BIGINT**, **INT** or **SHORT**.

**Date/Time Type & String Type Operands**

If a date/time type and a string type are operands, only '+' and '-' operators are allowed. If the '+' operator is used, it will be applied according to the following rules.

*   The string type will be converted to **BIGINT** with an interval value. The interval is the smallest unit for operands in the Date/Time type, and the interval for each type is as follows:

    *   **DATE** : Days
    *   **TIME**, **TIMESTAMP** : Seconds
    *   **DATETIME** : Milliseconds

*   Floating-point numbers are rounded.

*   The result type is the type of an date/time operand.

.. code-block:: sql

	SELECT date'2002-01-01' + '10';
	 
	  date '2002-01-01'+'10'
	======================
	  01/11/2002

If the date/time type and a string type are operands and the '-' operator is used, they will be applied according to the following rules.

*   If the date/time type operands are **DATE**, **DATETIME** and **TIMESTAMP**, the string will be converted to **DATETIME**; if the date/time operand is **TIME**, the string is converted to **TIME**.
*   The result type is always **BIGINT**.

.. code-block:: sql

	SELECT date'2002-01-01'-'2001-01-01';
	 
	  date '2002-01-01'-'2001-01-01'
	================================
						31536000000
	 
	-- this causes an error
	 
	SELECT date'2002-01-01'-'10';
	 
	 In line 1, column 13,
	 ERROR: Cannot coerce '10' to type datetime.    
 
**Numeric Type & String Type Operands**

If a numeric type and a string type are operands, they will be applied according to the following rules.

*   Strings will be converted to **DOUBLE** when possible.
*   The result type is **DOUBLE** or **MONETARY** and depends on the type of the numeric operand.

.. code-block:: sql

	SELECT 4 + '5.2';
	 
					4+'5.2'
	==========================
	  9.199999999999999e+00

Unlike CUBRID 2008 R3.1 and the earlier versions, the string in the date/time format, that is, the string such as '2010-09-15' is not converted to the date/time type. You can use a literal (DATE'2010-09-15') with the date/time type for addition and subtraction operations.

.. code-block:: sql

	SELECT '2002-01-01'+1;
	   ERROR: Cannot coerce '2002-01-01' to type double.
	
	SELECT DATE'2002-01-01'+1;
	  date '2002-01-01'+1
	=====================
	  01/02/2002

**String Type Operand**

If you multiply, divide or subtract both strings, the result returns a **DOUBLE** type value.

.. code-block:: sql

	SELECT '3'*'2';
	 
						 '3'*'2'
	============================
		   6.000000000000000e+00

The '+' operator action depends on how to set the system parameter **plus_as_concat** in the **cubrid.conf** file. For details, see :ref:`stmt-type-parameters`.

* If a value for **plus_as_concat** is yes (default value), the concatenation of two strings will be returned.

  .. code-block:: sql

	SELECT '1'+'1';
	 
				   '1'+'1'
	======================
					  '11'

* If a value for **plus_as_concat** is no and two strings can be converted to numbers, the **DOUBLE** type value will be returned by adding the two numbers.

  .. code-block:: sql

	SELECT '1'+'1';
	 
					   '1'+'1'
	==========================
		 2.000000000000000e+00

An error will be returned if it cannot be converted to the corresponding type.
