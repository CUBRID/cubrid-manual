
:meta-keywords: cubrid cast, cubrid type cast, cubrid date_format, cubrid to_char, cubrid to_date, cubrid to_datetime, cubrid to_datetime_tz, cubrid to_time, cubrid to_timestamp, cubrid to_timestamp_tz

:tocdepth: 3

*****************************************
Data Type Casting Functions and Operators
*****************************************

.. contents::

.. _castfn:

CAST
====

.. function:: CAST (cast_operand AS cast_target)

    The **CAST** operator can be used to explicitly cast one data type to another in the **SELECT** statement. A query list or a value expression in the **WHERE** clause can be cast to another data type.
    
    :param cast_operand: Declares the value to cast to a different data type.
    :param cast_target: Specifies the type to cast to.
    :rtype: cast_target

Depending on the situation, data type can be automatically converted without using the **CAST** operator. For details, see :ref:`implicit-type-conversion`.

See :ref:`cast-string-to-datetime` regarding to convert the string of date/time type into date/time type.

The following table shows a summary of explicit type conversions (casts) using the **CAST** operator in CUBRID.

+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **From \\ To** | **EN** | **AN** | **VC** | **FC** | **VB** | **FB** | **ENUM** | **BLOB** | **CLOB** | **D** | **T** | **UT** | **UTZ** | **DT** | **DTZ** | **S** | **MS** | **SQ** |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **EN**         | Yes    | Yes    | Yes    | Yes    | No     | No     | No       | No       | No       | No    | No    | Yes    | Yes     | No     | No      | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **AN**         | Yes    | Yes    | Yes    | Yes    | No     | No     | No       | No       | No       | No    | No    | Yes    | Yes     | No     | No      | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **VC**         | Yes    | Yes    | Yes(*) | Yes(*) | Yes    | Yes    | Yes      | Yes      | Yes      | Yes   | Yes   | Yes    | Yes     | Yes    | Yes     | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **FC**         | Yes    | Yes    | Yes(*) | Yes(*) | Yes    | Yes    | Yes      | Yes      | Yes      | Yes   | Yes   | Yes    | Yes     | Yes    | Yes     | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **VB**         | No     | No     | Yes    | Yes    | Yes    | Yes    | No       | Yes      | Yes      | No    | No    | No     | No      | No     | No      | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **FB**         | No     | No     | Yes    | Yes    | Yes    | Yes    | No       | Yes      | Yes      | No    | No    | No     | No      | No     | No      | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **ENUM**       | No     | No     | Yes    | Yes    |  No    | No     | Yes      | No       | No       | No    | No    | No     | No      | No     | No      | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **BLOB**       | No     | No     | No     | No     | Yes    | Yes    | Yes      | Yes      | No       | No    | No    | No     | No      | No     | No      | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **CLOB**       | No     | No     | Yes    | Yes    | No     | No     | Yes      | No       | Yes      | No    | No    | No     | No      | No     | No      | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **D**          | No     | No     | Yes    | Yes    | No     | No     | No       | No       | No       | Yes   | No    | Yes    | Yes     | Yes    | Yes     | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **T**          | No     | No     | Yes    | Yes    | No     | No     | No       | No       | No       | No    | Yes   | No     | No      | No     | No      | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **UT**         | No     | No     | Yes    | Yes    | No     | No     | No       | No       | No       | Yes   | Yes   | Yes    | Yes     | Yes    | Yes     | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **UTZ**        | No     | No     | Yes    | Yes    | No     | No     | No       | No       | No       | Yes   | Yes   | Yes    | Yes     | Yes    | Yes     | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **DT**         | No     | No     | Yes    | Yes    | No     | No     | No       | No       | No       | Yes   | Yes   | Yes    | Yes     | Yes    | Yes     | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **DTZ**        | No     | No     | Yes    | Yes    | No     | No     | No       | No       | No       | Yes   | Yes   | Yes    | Yes     | Yes    | Yes     | No    | No     | No     |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **S**          | No     | No     | No     | No     | No     | No     | No       | No       | No       | No    | No    | No     | No      | No     | No      | Yes   | Yes    | Yes    |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **MS**         | No     | No     | No     | No     | No     | No     | No       | No       | No       | No    | No    | No     | No      | No     | No      | Yes   | Yes    | Yes    |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+
| **SQ**         | No     | No     | No     | No     | No     | No     | No       | No       | No       | No    | No    | No     | No      | No     | No      | Yes   | Yes    | Yes    |
+----------------+--------+--------+--------+--------+--------+--------+----------+----------+----------+-------+-------+--------+---------+--------+---------+-------+--------+--------+

(*): The **CAST** operation is allowed only when the value expression and the data type to be cast have the same character set.

* **Data Type Key**

  *   **EN**: Exact numeric data type (**INTEGER**, **SMALLINT**, **BIGINT**, **NUMERIC**, **DECIMAL**)
  *   **AN**: Approximate numeric data type (**FLOAT/REAL**, **DOUBLE**)
  *   **VC**: Variable-length character string (**VARCHAR** (*n*))
  *   **FC**: Fixed-length character string (**CHAR** (*n*))
  *   **VB**: Variable-length bit string (**BIT VARYING** (*n*))
  *   **FB**: Fixed-length bit string (**BIT** (*n*))
  *   **ENUM**: **ENUM** type
  *   **BLOB**: Binary data that is stored outside DB
  *   **CLOB**: String data that is stored inside DB
  *   **D**: **DATE**
  *   **T**: **TIME**
  *   **DT**: **DATETIME**
  *   **DTZ**: **DATETIME WITH TIME ZONE** and **DATETIME WITH LOCAL TIME ZONE** data types
  *   **UT**: **TIMESTAMP**
  *   **UTZ**: **TIMESTAMP WITH TIME ZONE** and **TIMESTAMP WITH LOCAL TIME ZONE** data types
  *   **S**: **SET**
  *   **MS**: **MULTISET**
  *   **SQ**: **LIST** (= **SEQUENCE**)

.. code-block:: sql

    --operation after casting character as INT type returns 2
    SELECT (1+CAST ('1' AS INT));
    
::

    2
     
.. code-block:: sql

    --cannot cast the string which is out of range as SMALLINT
    SELECT (1+CAST('1234567890' AS SMALLINT));
     
::

    ERROR: Cannot coerce value of domain "character" to domain "smallint".
    
.. code-block:: sql

    --operation after casting returns 1+1234567890
    SELECT (1+CAST('1234567890' AS INT));
    
::

    1234567891
     
.. code-block:: sql

    --'1234.567890' is casted to 1235 after rounding up
    SELECT (1+CAST('1234.567890' AS INT));
    
::

    1236
     
.. code-block:: sql

    --'1234.567890' is casted to string containing only first 5 letters.
    SELECT (CAST('1234.567890' AS CHAR(5)));
    
::

    '1234.'
     
.. code-block:: sql

    --numeric type can be casted to CHAR type only when enough length is specified
    SELECT (CAST(1234.567890 AS CHAR(5)));
     
::

    ERROR: Cannot coerce value of domain "numeric" to domain "character".
    
.. code-block:: sql

    --numeric type can be casted to CHAR type only when enough length is specified
    SELECT (CAST(1234.567890 AS CHAR(11)));
    
::

    '1234.567890'
     
.. code-block:: sql

    --numeric type can be casted to CHAR type only when enough length is specified
    SELECT (CAST(1234.567890 AS VARCHAR));
    
::

    '1234.567890'
     
.. code-block:: sql

    --string can be casted to time/date types only when its literal is correctly specified
    SELECT (CAST('2008-12-25 10:30:20' AS TIMESTAMP));
    
::

    10:30:20 AM 12/25/2008
     
.. code-block:: sql

    SELECT (CAST('10:30:20' AS TIME));
    
::

    10:30:20 AM
     
.. code-block:: sql

    --string can be casted to TIME type when its literal is same as TIME's.
    SELECT (CAST('2008-12-25 10:30:20' AS TIME));
    
::

    10:30:20 AM
     
.. code-block:: sql

    --string can be casted to TIME type after specifying its type of the string
    SELECT (CAST(TIMESTAMP'2008-12-25 10:30:20' AS TIME));
    
::

    10:30:20 AM
     
.. code-block:: sql

    SELECT CAST('abcde' AS BLOB);
    
::

    file:/home1/user1/db/tdb/lob/ces_743/ces_temp.00001283232024309172_1342
     
.. code-block:: sql

    SELECT CAST(B'11010000' as varchar(10));
    
::

    'd0'
     
.. code-block:: sql

    SELECT CAST('1A' AS BLOB);
    
::

    X'1a00'
    
.. code-block:: sql

    --numbers can be casted to TIMESTAMP type
    SELECT CAST (1 AS TIMESTAMP), CAST (1.2F AS TIMESTAMP);
    
::

    09:00:01 AM 01/01/1970     09:00:01 AM 01/01/1970   

.. code-block:: sql

    --numbers cannot be casted to DATETIME type
    SELECT CAST (1 AS DATETIME);
    
::

    Cannot coerce 1 to type datetime

.. code-block:: sql

    --TIMESTAMP cannot be casted to numbers
    SELECT CAST (TIMESTAMP'09:00:01 AM 01/01/1970' AS INT)
    
::

    Cannot coerce timestamp '09:00:01 AM 01/01/1970' to type integer.
    
    
.. note::

    *   **CAST** is allowed only between data types having the same character set.
    *   If you cast an approximate data type(FLOAT, DOUBLE) to integer type, the number is rounded to zero decimal places.
    *   If you cast an exact numeric data type(NUMERIC) to integer type, the number is rounded to zero decimal places.
    *   If you cast a numeric data type to string character type, it should be longer than the length of significant figures + decimal point. An error occurs otherwise.
    *   If you cast a character string type *A* to a character string type *B*, B should be longer than the *A*. The end of character string is truncated otherwise.
    *   If you cast a character string type *A*    to a date-time date type *B*, it is converted only when literal of *A* and *B* type match one another. An error occurs otherwise.
    *   You must explicitly do type casting for numeric data stored in a character string so that an arithmetic operation can be performed.

DATE_FORMAT
===========

.. function:: DATE_FORMAT (date, format)

    The **DATE_FORMAT** function converts the value of date/time data type which include a date to specified date/time format and then return the value with the **VARCHAR** data type. For the format parameter to assign, refer to :ref:`Date/Time Format 2 <datetime-format2>` table of the :func:`DATE_FORMAT`. The :ref:`Date/Time Format 2 <datetime-format2>` table is used in :func:`DATE_FORMAT`, :func:`TIME_FORMAT`, and :func:`STR_TO_DATE` functions.

    :param date: A value of DATE, TIMESTAMP, DATETIME, DATETIMETZ, DATETIMELTZ, TIMESTAMPTZ, or TIMESTAMP.
    :param format: Specifies the output format. The format specifier starting with '%' is used.
    :rtype: STRING

When the *format* argument is assigned, the string is interpreted according to the specified language. At that time, the language specified as the **intl_date_lang** system parameter is applied. If the **intl_date_lang** value is not set, the language specified when creating DB is applied. 

For example, when the language is "de_DE" and the format is "%d %M %Y", the string "3 Oktober 2009" is interpreted as the DATE type of "2009-10-03". When the specified *format* argument does not correspond to the given string, an error is returned.

In the following :ref:`Date/Time Format 2 <datetime-format2>` table, the month/day, date, and AM/PM in characters are different by language.

.. _datetime-format2:

**Date/Time Format 2**

+------------------+-------------------------------------------------------------------------------------------------------------------+
| format Value     | Meaning                                                                                                           |
+==================+===================================================================================================================+
| %a               | Weekday, English abbreviation (Sun, ... , Sat)                                                                    |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %b               | Month, English abbreviation (Jan, ... , Dec)                                                                      |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %c               | Month (1, ... , 12)                                                                                               |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %D               | Day of the month, English ordinal number (1st, 2nd, 3rd, ...)                                                     |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %d               | Day of the month, two-digit number (01, ... , 31)                                                                 |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %e               | Day of the month (1, ... , 31)                                                                                    |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %f               | Microseconds, three-digit number (000, ... , 999)                                                                 |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %H               | Hour, 24-hour based, number with at least two-digit (00, ... , 23, ... , 100, ... )                               |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %h               | Hour, 12-hour based two-digit number (01, ... , 12)                                                               |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %I               | Hour, 12-hour based two-digit number (01, ... , 12)                                                               |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %i               | Minutes, two-digit number (00, ... , 59)                                                                          |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %j               | Day of year, three-digit number (001, ... , 366)                                                                  |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %k               | Hour, 24-hour based, number with at least one-digit (0, ... , 23, ... , 100, ... )                                |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %l               | Hour, 12-hour based (1, ... , 12)                                                                                 |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %M               | Month, English string (January, ... , December)                                                                   |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %m               | Month, two-digit number (01, ... , 12)                                                                            |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %p               | AM or PM                                                                                                          |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %r               | Time, 12-hour based, hour:minute:second (hh:mi:ss AM or hh:mi:ss PM)                                              |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %S               | Seconds, two-digit number (00, ... , 59)                                                                          |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %s               | Seconds, two-digit number (00, ... , 59)                                                                          |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %T               | Time, 24-hour based, hour:minute:second (hh:mi:ss)                                                                |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %U               | Week, two-digit number, week number of the year with Sunday being the first day Week (00, ... , 53)               |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %u               | Week, two-digit number, week number of the year with Monday being the first day (00, ... , 53)                    |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %V               | Week, two-digit number, week number of the year with Sunday being the first day Week (00, ... , 53)               |
|                  | (Available to use in combination with %X)                                                                         |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %v               | Week, two-digit number, week number of the year with Monday being the first day (00, ... , 53)                    |
|                  | (Available to use in combination with %x)                                                                         |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %W               | Weekday, English string (Sunday, ... , Saturday)                                                                  |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %w               | Day of the week, number index (0=Sunday, ... , 6=Saturday)                                                        |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %X               | Year, four-digit number calculated as the week number with Sunday being the first day of the week                 |
|                  | (0000, ... , 9999) (Available to use in combination with %V)                                                      |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %x               | Year, four-digit number calculated as the week number with Monday being the first day of the week                 |
|                  | (0000, ... , 9999) (Available to use in combination with %v)                                                      |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %Y               | Year, four-digit number (0001, ... , 9999)                                                                        |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %y               | Year, two-digit number (00, 01, ... , 99)                                                                         |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %%               | Output the special character "%" as a string                                                                      |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %x               | Output an arbitrary character x as a string out of English letters that are not used as format specifiers.        |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %TZR             | Time zone region information.  e.g. US/Pacific.                                                                   |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %TZD             | Daylight saving information. e.g. KST, KT, EET                                                                    |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %TZH             | Timezone hour offset. e.g. +09, -09                                                                               |
+------------------+-------------------------------------------------------------------------------------------------------------------+
| %TZM             | Timezone minute offset. e.g. +00, +30                                                                             |
+------------------+-------------------------------------------------------------------------------------------------------------------+

.. note::

    %TZR, %TZD, %TZH, %TZM can be used only in timezone types.
    
.. note:: **A format specifying a number after TZD**

    See :ref:`A format specifying a number after TZD <tzd-and-a-following-number>`.

The following example shows the case when the system parameter **intl_date_lang** is "en_US".

.. code-block:: sql

    SELECT DATE_FORMAT(datetime'2009-10-04 22:23:00', '%W %M %Y');
    
::

    'Sunday October 2009'
     
.. code-block:: sql

    SELECT DATE_FORMAT(datetime'2007-10-04 22:23:00', '%H:%i:%s');
    
::

    '22:23:00'
     
.. code-block:: sql

    SELECT DATE_FORMAT(datetime'1900-10-04 22:23:00', '%D %y %a %d %m %b %j');
    
::

    '4th 00 Thu 04 10 Oct 277'
     
.. code-block:: sql

    SELECT DATE_FORMAT(date'1999-01-01', '%X %V');
    
::

    '1998 52'

The following example shows the case when the system parameter **intl_date_lang** is "de_DE".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="de_DE"';
    SELECT DATE_FORMAT(datetime'2009-10-04 22:23:00', '%W %M %Y');
    
::

    'Sonntag Oktober 2009'
     
.. code-block:: sql

    SELECT DATE_FORMAT(datetime'2007-10-04 22:23:00', '%H:%i:%s %p');
    
::

    '22:23:00 Nachm.'
     
.. code-block:: sql

    SELECT DATE_FORMAT(datetime'1900-10-04 22:23:00', '%D %y %a %d %m %b %j');
    
::

    '4 00 Do. 04 10 Okt 277'

.. note::

    When the charset is ISO-8859-1, the language that can be changed by the system parameter **intl_date_lang** is "ko_KR" and "tr_TR" except "en_US". If the charset is UTF-8, it can be changed to any language supported by CUBRID. For details, see :ref:`Note <tochar-remark>` in the :func:`TO_CHAR`).

The following example outputs the value of DATETIMETZ type which includes timezone information as the desired format.

.. code-block:: sql

    SELECT DATE_FORMAT(datetimetz'2012-02-02 10:10:10 Europe/Zurich CET', '%TZR %TZD %TZH %TZM');

::
    'Europe/Zurich CET 01 00'

FORMAT
======

.. function:: FORMAT ( x , dec )

    The **FORMAT** function displays the number *x* by using digit grouping symbol as a thousands separator, so that its format becomes '#,###,###.#####' and performs rounding after the decimal point symbol to express as many as *dec* digits after it. The return value is a **VARCHAR** type.

    :param x: An expression that returns a numeric value
    :param dec: the number of digits of fractional parts
    :rtype: STRING

Thousands separator symbol and decimal point symbol is output in the format according to the specified language. The language is specified by the **intl_number_lang** system parameter. If the value of **intl_number_lang** is not set, the language specified when creating DB is applied. 

For example, when the language is one of the European languages, such as "de_DE" or "fr_FR", "." is interpreted as the thousands separator and "," as the decimal point symbol (see :ref:`Default output of number by language <tochar-default-number-format>` of the :func:`TO_CHAR`).

The following example shows command execution by setting the value of the **intl_number_lang system** parameter to "en_US".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_number_lang="en_US"';
    SELECT FORMAT(12000.123456,3), FORMAT(12000.123456,0);
    
::

    '12,000.123'          '12,000'

The following example shows command execution on the database by setting the value of the **intl_number_lang** system parameter to "de_DE". In the number output format of most European countries, such as Germany and France, "." is the cipher identifier and "," is the decimal point symbol.

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_number_lang="de_DE"';
    SELECT FORMAT(12000.123456,3), FORMAT(12000.123456,0);
    
::

    '12.000,123'          '12.000'

STR_TO_DATE
===========

.. function:: STR_TO_DATE (string, format)

    The **STR_TO_DATE** function converts the given character string to a date/time value by interpreting it according to the specified format and operates in the opposite way to the :func:`DATE_FORMAT` function. The return value is determined by the date/time part included in the character string.

    :param string: String.
    :param format: Specifies the format to interpret the character string. You should use character strings including % for the format specifiers. See :ref:`Date/Time Format 2 <datetime-format2>` table of :func:`DATE_FORMAT` function.
    :rtype: DATETIME, DATE, TIME, DATETIMETZ

For the *format* argument to assign, see :ref:`Date/Time Format 2 <datetime-format2>`  table of the :func:`DATE_FORMAT`.

If *string* is invalid date/time value or *format* is invalid, it returns an error.

When the *format* argument is assigned, the *string* is interpreted according to the specified language. At that time, the language specified as the **intl_date_lang** system parameter is applied. If the **intl_date_lang** value is not set, the language specified when creating DB is applied.

For example, when the language is "de_DE" and the *format* is "%d %M %Y", the string "3 Oktober 2009" is interpreted as the **DATE** type of "2009-10-03". If the *format* argument does not correspond to the given *string*, an error is returned.

0 is not allowed in the argument value corresponding to year, month, and day; however, if 0 is inputted in every argument value corresponding to date and time, the value of **DATE** or **DATETIME** type that has 0 for every date and time value is returned as an exception. Note that operation in JDBC program is determined by the configuration of zeroDateTimeBehavior, connection URL property. For more information about zeroDateTimeBehavior, please see :ref:`jdbc-connection-conf`.

The following example shows the case when the system parameter **intl_date_lang** is "en_US".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="en_US"';
    SELECT STR_TO_DATE('01,5,2013','%d,%m,%Y');
    
::

    05/01/2013
     
.. code-block:: sql

    SELECT STR_TO_DATE('May 1, 2013','%M %d,%Y');
    
::

    05/01/2013
     
.. code-block:: sql

    SELECT STR_TO_DATE('13:30:17','%H:%i');
    
::

    01:30:00 PM
     
.. code-block:: sql

    SELECT STR_TO_DATE('09:30:17 PM','%r');
    
::

    09:30:17 PM
     
.. code-block:: sql

    SELECT STR_TO_DATE('0,0,0000','%d,%m,%Y');
    
::

    00/00/0000

The following example shows the case when the system parameter **intl_date_lang** is "de_DE". The German Oktober is interpreted to 10.

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="de_DE"';
    SELECT STR_TO_DATE('3 Oktober 2009', '%d %M %Y');
    
::

    10/03/2009

.. note::

    When the charset is ISO-8859-1, the language that can be changed by the system parameter **intl_date_lang** is "ko_KR" and "tr_TR" except "en_US". If the charset is UTF-8, it can be changed to any language supported by CUBRID. For details, see :ref:`Note <tochar-remark>` in the :func:`TO_CHAR`).

The following example shows which converts a date/time string with timezone information into DATETIMETZ type value.

.. code-block:: sql

    SELECT STR_TO_DATE('2001-10-11 02:03:04 AM Europe/Bucharest EEST', '%Y-%m-%d %h:%i:%s %p %TZR %TZD');

::

    02:03:04.000 AM 10/11/2001 Europe/Bucharest EEST


TIME_FORMAT
===========

.. function:: TIME_FORMAT (time, format)

    The **TIME_FORMAT** function converts the date/time data type value including time value into a string of specified date/time format, and returns the value with the **VARCHAR** data type.

    :param time: A value of a type with time. (TIME, TIMESTAMP, DATETIME, TIMESTAMPTZ or DATETIMETZ)
    :param format: Specifies the output format. Use a string that contains '%' as a specifier. See the table :ref:`Date/Time Format 2 <datetime-format2>` of :func:`DATE_FORMAT` function.
    :rtype: STRING

When the *format* argument is assigned, the time is output according to the specified language. At this time, the language specified as the **intl_date_lang** system parameter is applied. If **intl_date_lang** system parameter is not set, the language specified when creating DB is applied.

For example, when the language is set to "de_DE" and the format is "%h:%i:%s %p", "08:46:53 PM" is output as "08:46:53 Nachm.". When the *format* argument specified does not correspond to the given string, an error is returned.

The following example shows the case when the system parameter **intl_date_lang** is "en_US".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="en_US"';
    SELECT TIME_FORMAT(time'22:23:00', '%H %i %s');
    
::

    '22 23 00'
     
.. code-block:: sql

    SELECT TIME_FORMAT(time'23:59:00', '%H %h %i %s %f');
    
::

    '23 11 59 00 000'
     
.. code-block:: sql

    SELECT SYSTIME, TIME_FORMAT(SYSTIME, '%p');
    
::

    08:46:53 PM  'PM'

The following example shows the case when the system parameter **intl_date_lang** is "de_DE".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="de_DE"';
    SELECT SYSTIME, TIME_FORMAT(SYSTIME, '%p');
     
::

    08:46:53 PM  'Nachm.'

.. note::

    When the charset is ISO-8859-1, the language that can be changed by the system parameter **intl_date_lang** is "ko_KR" and "tr_TR" except "en_US". If the charset is UTF-8, it can be changed to any language supported by CUBRID. For details, see :ref:`Note <tochar-remark>` in the :func:`TO_CHAR`).

The following outputs the value with a timezone information into a specified format string.

.. code-block:: sql

    SELECT TIME_FORMAT(datetimetz'2001-10-11 02:03:04 AM Europe/Bucharest EEST', '%h:%i:%s %p %TZR %TZD');

::

    '02:03:04 AM Europe/Bucharest EEST'


TO_CHAR(date_time)
==================

.. function:: TO_CHAR ( date_time [, format[, date_lang_string_literal ]] )

    The **TO_CHAR** (date_time) function converts the value of date/time types (**TIME**, **DATE**, **TIMESTAMP**, **DATETIME**) to a string depending on the table :ref:`Date/Time Format 1 <datetime-format1>` and then returns the value. The type of the return value is **VARCHAR**.

    :param date_time: A value of date/time type. (TIME, DATE, TIMESTAMP, DATETIME, DATETIMETZ, DATETIMELTZ, TIMESTAMPTZ, TIMESTAMPLTZ)
    :param format: A format of return value.
    :param date_lang_string_literal: Specifies a language applied to a return value.
    :rtype: STRING

When the *format* argument is specified, the *date_time* is output according to the specified language (see the :ref:`Date/Time Format 1 <datetime-format1>` table). A language is defined by the *date_lang_string_literal*. If *date_lang_string_literal* is omitted, the language specified by the *intl_date_lang* parameter is applied; if the value of *intl_date_lang* is not specified, the language specified when creating DB is applied.

For example, when the language is set to "de_DE" and the format is "HH:MI:SS:AM", "08:46:53 PM" is output as "08:46:53 Nachm.". When the *format* argument specified does not correspond to the given *string*, an error is returned.

When the *format* argument is omitted, the *date_time* is output as a string according to the default output format of the "en_US"(see the following table :ref:`Default Date/Time Output Format for Each Language <tochar-default-datetime-format>`).

.. note:: The **CUBRID_DATE_LANG** environment used in earlier version of CUBRID 9.0 is no longer supported.

.. _tochar-default-datetime-format:

**Default Date/Time Output Format for Each Language**

+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| LANG  | DATE           | TIME          | TIMESTAMP                 | DATETIME                     | TIMESTAMP WITH TIME ZONE     | DATETIME WITH TIME ZONE          |
+=======+================+===============+===========================+==============================+==============================+==================================+
| en_US | 'MM/DD/YYYY'   | 'HH:MI:SS AM' | 'HH:MI:SS AM MM/DD/YYYY'  | 'HH:MI:SS.FF AM MM/DD/YYYY'  | 'HH:MI:SS AM MM/DD/YYYY TZR' | 'HH:MI:SS.FF AM MM/DD/YYYY TZR'  |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| de_DE | 'DD.MM.YYYY'   | 'HH24:MI:SS'  | 'HH24:MI:SS DD.MM.YYYY'   | 'HH24:MI:SS.FF DD.MM.YYYY'   | 'HH24:MI:SS DD.MM.YYYY TZR'  | 'HH24:MI:SS.FF DD.MM.YYYY TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| es_ES | 'DD.MM.YYYY'   | 'HH24:MI:SS'  | 'HH24:MI:SS DD.MM.YYYY'   | 'HH24:MI:SS.FF DD.MM.YYYY'   | 'HH24:MI:SS DD/MM/YYYY TZR'  | 'HH24:MI:SS.FF DD/MM/YYYY TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| fr_FR | 'DD.MM.YYYY'   | 'HH24:MI:SS'  | 'HH24:MI:SS DD.MM.YYYY'   | 'HH24:MI:SS.FF DD.MM.YYYY'   | 'HH24:MI:SS DD/MM/YYYY TZR'  | 'HH24:MI:SS.FF DD/MM/YYYY TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| it_IT | 'DD.MM.YYYY'   | 'HH24:MI:SS'  | 'HH24:MI:SS DD.MM.YYYY'   | 'HH24:MI:SS.FF DD.MM.YYYY'   | 'HH24:MI:SS DD/MM/YYYY TZR'  | 'HH24:MI:SS.FF DD/MM/YYYY TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| ja_JP | 'YYYY/MM/DD'   | 'HH24:MI:SS'  | 'HH24:MI:SS YYYY/MM/DD'   | 'HH24:MI:SS.FF YYYY/MM/DD'   | 'HH24:MI:SS YYYY/MM/DD TZR'  | 'HH24:MI:SS.FF YYYY/MM/DD TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| km_KH | 'DD/MM/YYYY'   | 'HH24:MI:SS'  | 'HH24:MI:SS DD/MM/YYYY'   | 'HH24:MI:SS.FF DD/MM/YYYY'   | 'HH24:MI:SS DD/MM/YYYY TZR'  | 'HH24:MI:SS.FF DD/MM/YYYY TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| ko_KR | 'YYYY.MM.DD'   | 'HH24:MI:SS'  | 'HH24:MI:SS YYYY.MM.DD'   | 'HH24:MI:SS.FF YYYY.MM.DD'   | 'HH24:MI:SS YYYY.MM.DD TZR'  | 'HH24:MI:SS.FF YYYY.MM.DD TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| tr_TR | 'DD.MM.YYYY'   | 'HH24:MI:SS'  | 'HH24:MI:SS DD.MM.YYYY'   | 'HH24:MI:SS.FF DD.MM.YYYY'   | 'HH24:MI:SS DD.MM.YYYY TZR'  | 'HH24:MI:SS.FF DD.MM.YYYY TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| vi_VN | 'DD/MM/YYYY'   | 'HH24:MI:SS'  | 'HH24:MI:SS DD/MM/YYYY'   | 'HH24:MI:SS.FF DD/MM/YYYY'   | 'HH24:MI:SS DD/MM/YYYY TZR'  | 'HH24:MI:SS.FF DD/MM/YYYY TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| zh_CN | 'YYYY-MM-DD'   | 'HH24:MI:SS'  | 'HH24:MI:SS YYYY-MM-DD'   | 'HH24:MI:SS.FF YYYY-MM-DD'   | 'HH24:MI:SS YYYY-MM-DD TZR'  | 'HH24:MI:SS.FF YYYY-MM-DD TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+
| ro_RO | 'DD.MM.YYYY'   | 'HH24:MI:SS'  | 'HH24:MI:SS DD.MM.YYYY'   | 'HH24:MI:SS.FF DD.MM.YYYY'   | 'HH24:MI:SS DD.MM.YYYY TZR'  | 'HH24:MI:SS.FF DD.MM.YYYY TZR'   |
+-------+----------------+---------------+---------------------------+------------------------------+------------------------------+----------------------------------+

.. _datetime-format1:

**Date/Time Format 1**

+--------------------+---------------------------------------------------------------------------+
| Format Element     | Description                                                               |
+====================+===========================================================================+
| **CC**             | Century                                                                   |
+--------------------+---------------------------------------------------------------------------+
| **YYYY**           | Year with 4 numbers, Year with 2 numbers                                  |
| , **YY**           |                                                                           |
+--------------------+---------------------------------------------------------------------------+
| **Q**              | Quarter (1, 2, 3, 4; January - March = 1)                                 |
+--------------------+---------------------------------------------------------------------------+
| **MM**             | Month (01-12; January = 01)                                               |
|                    | *Note: MI represents the minute of hour.*                                 |
+--------------------+---------------------------------------------------------------------------+
| **MONTH**          | Month in characters                                                       |
+--------------------+---------------------------------------------------------------------------+
| **MON**            | Abbreviated month name                                                    |
+--------------------+---------------------------------------------------------------------------+
| **DD**             | Day (1 - 31)                                                              |
+--------------------+---------------------------------------------------------------------------+
| **DAY**            | Day of the week in characters                                             |
+--------------------+---------------------------------------------------------------------------+
| **DY**             | Abbreviated day of the week                                               |
+--------------------+---------------------------------------------------------------------------+
| **D** or **d**     | Day of the week in numbers (1 - 7)                                        |
+--------------------+---------------------------------------------------------------------------+
| **AM** or **PM**   | AM/PM                                                                     |
+--------------------+---------------------------------------------------------------------------+
| **A.M.**           | AM/PM with periods                                                        |
| or **P.M.**        |                                                                           |
+--------------------+---------------------------------------------------------------------------+
| **HH**             | Hour (1 -12)                                                              |
| or **HH12**        |                                                                           |
+--------------------+---------------------------------------------------------------------------+
| **HH24**           | Hour (0 - 23)                                                             |
+--------------------+---------------------------------------------------------------------------+
| **MI**             | Minute (0 - 59)                                                           |
+--------------------+---------------------------------------------------------------------------+
| **SS**             | Second (0 - 59)                                                           |
+--------------------+---------------------------------------------------------------------------+
| **FF**             | Millisecond (0-999)                                                       |
+--------------------+---------------------------------------------------------------------------+
| \- / , . ; : "text"| Punctuation and quotation marks are represented as they are in the result |
+--------------------+---------------------------------------------------------------------------+
| **TZD**            | Daylight saving information. e.g. KST, KT, EET                            |
+--------------------+---------------------------------------------------------------------------+
| **TZH**            | Timezone hour offset. e.g. +09, -09                                       |
+--------------------+---------------------------------------------------------------------------+
| **TZM**            | Timezone minute offset. e.g. +00, +30                                     |
+--------------------+---------------------------------------------------------------------------+

.. note::

    TZR, TZD, TZH, TZM can be used only in timezone types.

..  _tzd-and-a-following-number:
    
.. note:: **A format to specify a number after "TZD"**

    A number can be added after "TZD". This format is from TZD2 to TZD11; When you use a general character as a separator of a string, this format can be used.
    
        .. code-block:: sql
        
            SELECT STR_TO_DATE('09:30:17 20140307XEESTXEurope/Bucharest','%h:%i:%s %Y%d%mX%TZD4X%TZR');
            
        ::
        
            09:30:17.000 AM 07/03/2014 Europe/Bucharest EEST

        When you use a general character, 'X', as a separator to separate each value, TZD value's string length is variable; therefore, it is confused to separate TZD value and a separator. In this case, TZD values' length should be specified.

**Example of date_lang_string_literal**

+--------------+--------------------------------------------+
| **Format     | **date_lang_string_literal**               |
| Element**    +------------------------------+-------------+
|              | **'en_US'**                  | **'ko_KR'** |
+==============+==============================+=============+
| **MONTH**    | JANUARY                      | 1월         |
+--------------+------------------------------+-------------+
| **MON**      | JAN                          | 1           |
+--------------+------------------------------+-------------+
| **DAY**      | MONDAY                       | 월요일      |
+--------------+------------------------------+-------------+
| **DY**       | MON                          | 월          |
+--------------+------------------------------+-------------+
| **Month**    | January                      | 1월         |
+--------------+------------------------------+-------------+
| **Mon**      | Jan                          | 1           |
+--------------+------------------------------+-------------+
| **Day**      | Monday                       | 월요일      |
+--------------+------------------------------+-------------+
| **Dy**       | Mon                          | 월          |
+--------------+------------------------------+-------------+
| **month**    | january                      | 1월         |
+--------------+------------------------------+-------------+
| **mon**      | jan                          | 1           |
+--------------+------------------------------+-------------+
| **day**      | monday                       | 월요일      |
+--------------+------------------------------+-------------+
| **Dy**       | mon                          | 월          |
+--------------+------------------------------+-------------+
| **AM**       | AM                           | 오전        |
+--------------+------------------------------+-------------+
| **Am**       | Am                           | 오전        |
+--------------+------------------------------+-------------+
| **am**       | am                           | 오전        |
+--------------+------------------------------+-------------+
| **A.M.**     | A.M.                         | 오후        |
+--------------+------------------------------+-------------+
| **A.m.**     | A.m.                         | 오전        |
+--------------+------------------------------+-------------+
| **a.m.**     | a.m.                         | 오전        |
+--------------+------------------------------+-------------+
| **PM**       | PM                           | 오후        |
+--------------+------------------------------+-------------+
| **Pm**       | Pm                           | 오후        |
+--------------+------------------------------+-------------+
| **pm**       | pm                           | 오후        |
+--------------+------------------------------+-------------+
| **P.M.**     | P.M.                         | 오후        |
+--------------+------------------------------+-------------+
| **P.m.**     | P.m.                         | 오후        |
+--------------+------------------------------+-------------+
| **p.m.**     | p.m.                         | 오후        |
+--------------+------------------------------+-------------+

**Example of Format Digits of Return Value**

+-------------------------+---------------------------------------------------------------------+
|   Format Element        | en_US   Digits                   | ko_KR   Digits                   |
+=========================+==================================+==================================+
| **MONTH(Month, month)** | 9                                | 4                                |
+-------------------------+----------------------------------+----------------------------------+
| **MON(Mon, mon)**       | 3                                | 2                                |
+-------------------------+----------------------------------+----------------------------------+
| **DAY(Day, day)**       | 9                                | 6                                |
+-------------------------+----------------------------------+----------------------------------+
| **DY(Dy, dy)**          | 3                                | 2                                |
+-------------------------+----------------------------------+----------------------------------+
| **HH12, HH24**          | 2                                | 2                                |
+-------------------------+----------------------------------+----------------------------------+
| "text"                  | The length of the text           | The length of the text           |
+-------------------------+----------------------------------+----------------------------------+
| Other formats           | Same as the length of the format | Same as the length of the format |
+-------------------------+----------------------------------+----------------------------------+

The following example shows the query executed by setting the language and charset to "en_US.iso88591".

.. code-block:: sql

    -- create database testdb en_US.iso88591
     
    --creating a table having date/time type columns
    CREATE TABLE datetime_tbl(a TIME, b DATE, c TIMESTAMP, d DATETIME);
    INSERT INTO datetime_tbl VALUES(SYSTIME, SYSDATE, SYSTIMESTAMP, SYSDATETIME);
     
    --selecting a VARCHAR type string from the data in the specified format
    SELECT TO_CHAR(b, 'DD, DY , MON, YYYY') FROM datetime_tbl;
    
::

    '20, TUE , AUG, 2013'

.. code-block:: sql

    SELECT TO_CHAR(c, 'HH24:MI, DD, MONTH, YYYY') FROM datetime_tbl;
    
::

    '17:00, 20, AUGUST   , 2013'
     
.. code-block:: sql

    SELECT TO_CHAR(d, 'HH12:MI:SS:FF pm, YYYY-MM-DD-DAY') FROM datetime_tbl;
    
::

    '05:00:58:358 pm, 2013-08-20-TUESDAY  '
     
.. code-block:: sql

    SELECT TO_CHAR(TIMESTAMP'2009-10-04 22:23:00', 'Day Month yyyy');
    
::

    'Sunday    October   2009'

The following example shows an additional language parameter given to the **TO_CHAR** function in the database created above. When the charset is ISO-8859-1, setting the language parameter of the **TO_CHAR** function to "tr_TR" or "ko_KR" is allowed, but the other languages are not allowed. To use all languages by setting the language parameter of **TO_CHAR**, the charset when creating DB should be UTF-8.

.. code-block:: sql

    SELECT TO_CHAR(TIMESTAMP'2009-10-04 22:23:00', 'Day Month yyyy','ko_KR');
    
::

    'Iryoil    10wol 2009'
     
.. code-block:: sql

    SELECT TO_CHAR(TIMESTAMP'2009-10-04 22:23:00', 'Day Month yyyy','tr_TR');
    
::

    'Pazar     Ekim    2009'

.. _tochar-remark:

.. note::

    *   In the function that interprets the month/day in characters and AM/PM differently by language, if the charset is ISO-8859-1, the language can be changed to "ko_KR" or "tr_TR" only by using the **intl_date_lang** except "en_US" (see the above example). If the charset is UTF-8, the language can be changed to any language supported by CUBRID. By setting the intl_date_lang system parameter or by specifying the language parameter of the **TO_CHAR** function, the language can be changed to one of all the languages supported by CUBRID (see *date_lang_string_literal* of "Syntax" above). For a list of functions that interpret the date/time differently by language, see the description of the **intl_date_lang** system parameter.

        .. code-block:: sql

            -- change date locale as "de_DE" and run the below query.
            -- This case is failed because database locale, en_US's charset is ISO-8859-1
            -- and 'de_DE' only supports UTF-8 charset.
             
            SELECT TO_CHAR(TIMESTAMP'2009-10-04 22:23:00', 'Day Month yyyy','de_DE');
        
        ::
        
            ERROR: before ' , 'Day Month yyyy','de_DE'); '
            Locales for language 'de_DE' are not available with charset 'iso8859-1'.

        The following example shows how to set the language parameter of the **TO_CHAR** function to "de_DE" when you created DB with the locale "en_US.utf8".

        .. code-block:: sql

            SELECT TO_CHAR(TIMESTAMP'2009-10-04 22:23:00', 'Day Month yyyy','de_DE');
        
        ::
        
            'Sonntag   Oktober 2009'

    *   If the first argument is zerodate and the second argument has a literal like 'Month', 'Day', then  TO_CHAR function returns NULL.
    
        .. code-block:: sql
        
            SELECT TO_CHAR(timestamp '0000-00-00 00:00:00', 'Month Day YYYY');
            
        ::
        
            NULL

The following is an example to output date/time type with timezone in TO_CHAR function.

If you don't define a format, it outputs as the following format 

.. code-block:: sql

    SELECT TO_CHAR(datetimetz'2001-10-11 02:03:04 AM Europe/Bucharest EEST');

::

    '02:03:04.000 AM 10/11/2001 Europe/Bucharest EEST'

If you define a format, it outputs as the defined format.

.. code-block:: sql

    SELECT TO_CHAR(datetimetz'2001-10-11 02:03:04 AM Europe/Bucharest EEST', 'MM/DD/YYYY HH24:MI TZR TZD TZH TZM');

::

    '10/11/2001 02:03 Europe/Bucharest EEST +03 +00'

TO_CHAR(number)
===============

.. function:: TO_CHAR(number[, format[, number_lang_string_literal ] ])

    The **TO_CHAR** function converts a numeric data type to a character string according to :ref:`Number Format <tochar-number-format>` and returns it. The type of the return value is **VARCHAR** .
    
    :param number: Specifies an expression that returns numeric data type string. If the input value is **NULL**, **NULL** is returned. If the input value is character type, the character itself is returned.
    :param format: Specifies a format of return value. If format is not specified, all significant figures are returned as character string by default. If the value is **NULL**, **NULL** is returned.
    :param number_lang_string_literal: Specifies the language to be applied to the input value.
    :rtype: STRING

If *format* argument is specified, *number* is converted into a character string according to a specified language. At this time, the language is defined by the *number_lang_string_literal* argument. If *number_lang_string_literal* is omitted, the language specified by **intl_number_lang** system parameter is applied; if **intl_number_lang** is not set, the language specified when creating DB is applied. 

For example, if the language is one of the European languages, such as "de_DE" or "fr_FR", "." is printed out as a thousands separator and "," is printed out as a decimal point. If the *format* argument does not correspond to the given string, the function returns an error.

If *format* argument is omitted, *number* is converted into a character string according to the default format of a specified language(see the table :ref:`Default Output of Number for Each Language <tochar-default-number-format>`).

.. _tochar-number-format:

**Number Format**

+--------------------+-------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Format Element     | Example     | Description                                                                                                                                                                              |
+====================+=============+==========================================================================================================================================================================================+
| **9**              | 9999        | The number of 9's represents the number of significant figures to be returned.                                                                                                           |
|                    |             | If the number of significant figures specified in the format is not sufficient, only the decimal part is rounded. If it is less than the number of digits in an integer, # is outputted. |
|                    |             | If the number of significant figures specified in the format is sufficient, the part preceding the integer part is filled with space characters and the decimal part is filled with 0.   |
+--------------------+-------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **0**              | 0999        | If the number of significant figures specified in the format is sufficient, the part preceding the integer part is filled with 0, not space characters before the value is returned.     |
+--------------------+-------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **S**              | S9999       | Outputs the negative/positive sign in the specified position. These signs can be used only at the beginning of character string.                                                         |
+--------------------+-------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **C**              | C9999       | Returns the ISO currency code at the specified position.                                                                                                                                 |
+--------------------+-------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **,**              | 9,999       | Returns a comma (",") at the specified position. Multiple commas are allowed in the format.                                                                                              |
| (comma)            |             |                                                                                                                                                                                          |
+--------------------+-------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **.**              | 9.999       | Returns a decimal point (".") which distinguishes between a decimal and an at the specified position. Only one decimal point is allowed in the format.                                   |
| (decimal point)    |             | see the table, :ref:`Default Output of Number for Each Language <tochar-default-number-format>`                                                                                          |
+--------------------+-------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **EEEE**           | 9.99EEEE    | Returns a scientific notation number.                                                                                                                                                    |
+--------------------+-------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

.. _tochar-default-number-format:    

**Default Output of Number for Each Language**

+--------------+------------+-------------------+-----------------+--------------------------+
| Language     | Locale     | Number of Digits  | Decimal Symbol  | Example of Number Usage  |
+==============+============+===================+=================+==========================+
| English      | en_US      | ,(comma)          | .(period)       | 123,456,789.012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| German       | de_DE      | .(period)         | ,(comma)        | 123.456.789,012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| Spanish      | es_ES      | .(period)         | ,(comma)        | 123.456.789,012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| French       | fr_FR      | .(period)         | ,(comma)        | 123.456.789,012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| Italian      | it_IT      | .(period)         | ,(comma)        | 123.456.789,012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| Japanese     | ja_JP      | ,(comma)          | .(period)       | 123,456,789.012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| Cambodian    | km_KH      | .(period)         | ,(comma)        | 123.456.789,012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| Korean       | ko_KR      | ,(comma)          | .(period)       | 123,456,789.012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| Turkish      | tr_TR      | .(period)         | ,(comma)        | 123.456.789,012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| Vietnamese   | vi_VN      | .(period)         | ,(comma)        | 123.456.789,012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| Chinese      | zh_CN      | ,(comma)          | .(period)       | 123,456,789.012          |
+--------------+------------+-------------------+-----------------+--------------------------+
| Romanian     | ro_RO      | .(period)         | ,(comma)        | 123.456.789,012          |
+--------------+------------+-------------------+---------------+-+--------------------------+

The following example shows execution of the database by the locale specified when creating DB to "en_US.utf8".

.. code-block:: sql

    --selecting a string casted from a number in the specified format
    SELECT TO_CHAR(12345,'S999999'), TO_CHAR(12345,'S099999');
    
::

    ' +12345'             '+012345'
     
     
.. code-block:: sql

    SELECT TO_CHAR(1234567,'9,999,999,999');
    
::

    '    1,234,567'
     
.. code-block:: sql

    SELECT TO_CHAR(1234567,'9.999.999.999');
    
::

    '##############'
     
.. code-block:: sql

    SELECT TO_CHAR(123.4567,'99'), TO_CHAR(123.4567,'999.99999'), TO_CHAR(123.4567,'99999.999');
    
::

    '##'                  '123.45670'           '  123.457'
      
The following example shows command execution by setting the value of the **intl_number_lang** system parameter to "de_DE". In the number output format of most European countries such as Germany and France, "." is the cipher identifier and "," is the decimal point symbol.

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_number_lang="de_DE"';
     
    --selecting a string casted from a number in the specified format
    SELECT TO_CHAR(12345,'S999999'), TO_CHAR(12345,'S099999');

::
    
    ' +12345'             '+012345'
     
.. code-block:: sql
     
    SELECT TO_CHAR(1234567,'9,999,999,999');
    
::

    '##############'
     
.. code-block:: sql
     
    SELECT TO_CHAR(1234567,'9.999.999.999');
    
::

    '    1.234.567'
     
.. code-block:: sql

    SELECT TO_CHAR(123.4567,'99'), TO_CHAR(123.4567,'999,99999'), TO_CHAR(123.4567,'99999,999');
     
::

    '##'                  '123,45670'           '  123,457'
     
.. code-block:: sql

    SELECT TO_CHAR(123.4567,'99','en_US'), TO_CHAR(123.4567,'999.99999','en_US'), TO_CHAR(123.4567,'99999.999','en_US');
    
::

    '##'                  '123.45670'           '  123.457'
     
.. code-block:: sql

    SELECT TO_CHAR(1.234567,'99.999EEEE','en_US'), TO_CHAR(1.234567,'99,999EEEE','de_DE'), to_char(123.4567);
     
::

    '1.235E+00'           '1,235E+00'           '123,4567'

TO_DATE
=======

.. function:: TO_DATE(string [,format [,date_lang_string_literal]])

    The **TO_DATE** function interprets a character string based on the date format given as an argument, converts it to a **DATE** type value, and returns it. For the format, see :ref:`Date/Time Format 1 <datetime-format1>`.

    :param string: A character string
    :param format: Specifies a format of return value to be converted as **DATE** type. See :ref:`Date/Time Format 1 <datetime-format1>`. If the value is **NULL**, **NULL** is returned.
    :param date_lang_string_literal: Specifies the language for the input value to be applied.
    :rtype: DATE

When the *format* argument is specified, the *string* is interpreted according to the specified language. At this time, the language is set by *date_lang_string_literal* argument. If *date_lang_string_literal* argument is not set, the language is specified by the **intl_date_lang** system parameter; if the value of **intl_date_lang** is not set, the language is applied by the language specified when creating DB. 

For example, when a language is "de_DE" and *string* is "12.mai.2012", and *format* is "DD.mon.YYYY", it is interpreted as May 12th, 2012.When the *format* parameter specified does not correspond to the given *string*, an error is returned.

When the *format* argument is omitted, *string* is interpreted as the CUBRID default format (refer to :ref:`cast-to-datetime-recommend`) and if it fails, *string* is interpreted as the language format (see the table :ref:`Default Output Format of Language <tochar-default-datetime-format>` in the :func:`TO_CHAR`) by **intl_date_lang**. If the value of **intl_date_lang** is not set, the language is applied by the language specified when creating DB.

For example, when a language is "de_DE", the acceptable strings for **DATE** type are "MM/DD/YYYY", CUBRID default format and "DD.MM.YYYY", "de_DE" default format.

The following example shows the query executed by the locale specified when creating DB to "en_US.utf8".

.. code-block:: sql

    --selecting a date type value casted from a string in the specified format
     
    SELECT TO_DATE('12/25/2008');
    
::

    12/25/2008
     
.. code-block:: sql

    SELECT TO_DATE('25/12/2008', 'DD/MM/YYYY');
    
::

    12/25/2008
     
.. code-block:: sql

    SELECT TO_DATE('081225', 'YYMMDD');
    
::

    12/25/2008
     
.. code-block:: sql

    SELECT TO_DATE('2008-12-25', 'YYYY-MM-DD');
    
::

    12/25/2008

The following example shows the query executed when the system parameter **intl_date_lang** is "de_DE". 

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="de_DE"';
    SELECT TO_DATE('25.12.2012');
    
::

    12/25/2012
     
.. code-block:: sql

    SELECT TO_DATE('12/mai/2012','dd/mon/yyyy', 'de_DE');
    
::

    05/12/2012

.. note::

    When the charset is ISO-8859-1, the language that can be changed by the system parameter **intl_date_lang** is "ko_KR" and "tr_TR" except "en_US". If the charset is UTF-8, it can be changed to any language supported by CUBRID. For details, see :ref:`Note <tochar-remark>` in the :func:`TO_CHAR`).

TO_DATETIME
===========

.. function:: TO_DATETIME (string [,format [,date_lang_string_literal]])

    The **TO_DATETIME** function interprets a character string based on the date-time format given as an argument, converts it to a **DATETIME** type value, and returns it. For the format, see :ref:`Date/Time Format 1 <datetime-format1>`.

    :param string: A character string
    :param format: Specifies a format of return value to be converted as **DATETIME** type. See the table, :ref:`Date/Time Format 1 <datetime-format1>`. If the value is **NULL**, **NULL** is returned.
    :param date_lang_string_literal: Specifies the language for the input value to be applied.
    :rtype: DATETIME

When the *format* argument is specified, the *string* is interpreted according to the specified language. 

For example, when a language is "de_DE" and *string* is "12/mai/2012 12:10:00 Nachm."  and *format* is "DD/MON/YYYY HH:MI:SS AM", it is interpreted as May 12th, 2012 12:10:00 PM. At this time, the language is set by *date_lang_string_literal* argument. If *date_lang_string_literal* argument is not set, the language is specified by the **intl_date_lang** system parameter; if the value of **intl_date_lang** is not set, the language is specified by the language specified when creating DB. When the *format* parameter specified does not correspond to the given *string*, an error is returned.

When the *format* argument is omitted, *string* is interpreted as the CUBRID default format (refer to :ref:`cast-to-datetime-recommend`) and if it fails, *string* is interpreted as the language format (see the table :ref:`Default Output Format of Language <tochar-default-datetime-format>` in the :func:`TO_CHAR`) by **intl_date_lang**. If the value of **intl_date_lang** is not set, the language is applied by the language specified when creating DB.

For example, when a language is "de_DE", the acceptable strings for **DATETIME** type are "HH:MI:SS.FF AM MM/DD/YYYY", CUBRID default format and "HH24:MI:SS.FF DD.MM.YYYY", "de_DE" default format.

.. note:: The **CUBRID_DATE_LANG** environment used in earlier version of CUBRID 9.0 is no longer supported.

The following example shows execution of the database by setting the environment variable **CUBRID_CHARSET** to "en_US".

.. code-block:: sql

    --selecting a datetime type value casted from a string in the specified format
     
    SELECT TO_DATETIME('13:10:30 12/25/2008');
    
::

    01:10:30.000 PM 12/25/2008
     
.. code-block:: sql

    SELECT TO_DATETIME('08-Dec-25 13:10:30.999', 'YY-Mon-DD HH24:MI:SS.FF');
    
::

    01:10:30.999 PM 12/25/2008
     
.. code-block:: sql

    SELECT TO_DATETIME('DATE: 12-25-2008 TIME: 13:10:30.999', '"DATE:" MM-DD-YYYY "TIME:" HH24:MI:SS.FF');
    
::

    01:10:30.999 PM 12/25/2008

The following example shows the case when the system parameter **intl_date_lang** is "de_DE".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="de_DE"';
    SELECT TO_DATETIME('13:10:30.999 25.12.2012');
    
::

    01:10:30.999 PM 12/25/2012
     
.. code-block:: sql

    SELECT TO_DATETIME('12/mai/2012 12:10:00 Nachm.','DD/MON/YYYY HH:MI:SS AM', 'de_DE');
    
::

    12:10:00.000 PM 05/12/2012

.. note::

    When the charset is ISO-8859-1, the language that can be changed in **TO_DATETIME** function is "ko_KR" and "tr_TR" except "en_US". If the charset is UTF-8, it can be changed to any language supported by CUBRID. For details, see :ref:`Note <tochar-remark>` in the :func:`TO_CHAR`.

TO_DATETIME_TZ
==============

.. function:: TO_DATETIME_TZ (string [,format [,date_lang_string_literal]])

    **TO_DATETIME_TZ** function is the same as :func:`TO_DATETIME` function except that this function can include a timezone information on this input string.

    :rtype: DATETIMETZ

.. code-block:: sql

    SELECT TO_DATETIME_TZ('13:10:30 Asia/Seoul 12/25/2008', 'HH24:MI:SS TZR MM/DD/YYYY');
    
::

    01:10:30.000 PM 12/25/2008 Asia/Seoul

TO_NUMBER
=========

.. function:: TO_NUMBER(string [, format ])

    The **TO_NUMBER** function interprets a character string based on the number format given as an argument, converts it to a **NUMERIC** type value, and returns it.
    
    :param string: Specifies an expression that returns character string. If the value is **NULL**, **NULL** is returned.
    :param format: Specifies a format of return value to be converted as **NUMBER** type. See :ref:`Number Format <tochar-number-format>`. If it is omitted, NUMERIC(38,0) value is returned.
    :rtype: NUMERIC

When the *format* argument is assigned, the string is interpreted according to the specified language. At this time, the language is specified by the **intl_number_lang** system parameter. If the value of **intl_number_lang** is not set, the language specified when creating DB is applied.

For example, when the language is one of the European languages, such as "de_DE" and "fr_FR", "." is interpreted as the cipher identifier and "," as the decimal point symbol. When the format parameter specified does not correspond to the given string, an error is returned.

If the *format* argument is omitted, string is interpreted according to the default output format set by **intl_number_lang** (see :ref:`Default Output of Number for Each Language <tochar-default-number-format>`). When the **intl_number_lang** is not set, the language specified when creating DB is applied. 

The following example shows execution of the database by setting the value of system parameter **intl_number_lang** as "en_US".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_number_lang="en_US"';

    --selecting a number casted from a string in the specified format
    SELECT TO_NUMBER('-1234');
    
::

    -1234
     
.. code-block:: sql
     
    SELECT TO_NUMBER('12345','999999');
    
::

    12345
     
.. code-block:: sql
     
    SELECT TO_NUMBER('12,345.67','99,999.999');
    
::

    12345.670
     
.. code-block:: sql
     
    SELECT TO_NUMBER('12345.67','99999.999');
    
::

    12345.670

The following example shows command execution on the database by setting the value of the **intl_number_lang** system parameter to "de_DE". In the number output format of most European countries, such as Germany and France, "." is the cipher identifier and "," is the decimal point symbol.

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_number_lang="de_DE"';
    SELECT TO_NUMBER('12.345,67','99.999,999');

::

    12345.670

TO_TIME
=======

.. function:: TO_TIME (string [,format [,date_lang_string_literal]])

    The **TO_TIME** function interprets a character string based on the time format given as an argument, converts it to a **TIME** type value, and returns it. For the format, see :ref:`Date/Time Format 1 <datetime-format1>`.

    :param string: Specifies an expression that returns character string. If the value is **NULL**, **NULL** is returned.
    :param format: Specifies a format of return value to be converted as **TIME** type. See :ref:`Date/Time Format 1 <datetime-format1>`. If the value is **NULL**, **NULL** is returned.
    :param date_lang_string_literal: Specifies the language for the input value to be applied.
    :rtype: TIME

When the *format* argument is specified, the *string* is interpreted according to the specified language. At this time, the language is set by *date_lang_string_literal* argument. If *date_lang_string_literal* argument is not set, the language is specified by the **intl_date_lang** system parameter; if the value of **intl_date_lang** is not set, the language specified when creating DB is applied. 
When the *format* parameter does not correspond to the given *string*, an error is returned.

For example, when a language is "de_DE" and *string* is "10:23:00 Nachm.", and *format* is "HH/MI/SS/AM, it is interpreted as 10:23:00 PM.

When the *format* argument is omitted, *string* is interpreted as the CUBRID default format (refer to :ref:`cast-to-datetime-recommend`) and if it fails, *string* is interpreted as the language format (see the table :ref:`Default Output Format of Language <tochar-default-datetime-format>` in the :func:`TO_CHAR`) by **intl_date_lang**. If the value of **intl_date_lang** is not set, the language is applied by the language specified when creating DB.

For example, when a language is "de_DE", the acceptable strings for **TIME** type are "HH:MI:SS AM", CUBRID default format and "HH24:MI:SS", "de_DE" default format.

.. note:: The **CUBRID_DATE_LANG** environment used in earlier version of CUBRID 9.0 is no longer supported.

The following example shows execution of the database by setting the value of system parameter **intl_date_lang** as "en_US".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="en_US"';

    --selecting a time type value casted from a string in the specified format
     
    SELECT TO_TIME ('13:10:30');
    
::

    01:10:30 PM
     
.. code-block:: sql

    SELECT TO_TIME('HOUR: 13 MINUTE: 10 SECOND: 30', '"HOUR:" HH24 "MINUTE:" MI "SECOND:" SS');
    
::

    01:10:30 PM
     
.. code-block:: sql

    SELECT TO_TIME ('13:10:30', 'HH24:MI:SS');
    
::

    01:10:30 PM
     
.. code-block:: sql

    SELECT TO_TIME ('13:10:30', 'HH12:MI:SS');
     
::

    ERROR: Conversion error in date format.

The following example shows the case when the system parameter **intl_date_lang** is "de_DE".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="de_DE"';
    SELECT TO_TIME('13:10:30');
    
::

    01:10:30 PM
     
.. code-block:: sql

    SELECT TO_TIME('10:23:00 Nachm.', 'HH:MI:SS AM');

::
    
    10:23:00 PM

.. note::

    When the charset is ISO-8859-1, the language that can be changed by the system parameter **intl_date_lang** is "ko_KR" and "tr_TR" except "en_US". If the charset is UTF-8, it can be changed to any language supported by CUBRID. For details, see :ref:`Note <tochar-remark>` in the :func:`TO_CHAR`).


TO_TIMESTAMP
============

.. function:: TO_TIMESTAMP(string [, format [,date_lang_string_literal]])

    The **TO_TIMESTAMP** function interprets a character string based on the time format given as an argument, converts it to a **TIMESTAMP** type value, and returns it. For the format, see :ref:`Date/Time Format 1 <datetime-format1>`.

    :param string: Specifies an expression that returns character string. If the value is **NULL**, **NULL** is returned.
    :param format: Specifies a format of return value to be converted as **TIMESTAMP** type. See :ref:`Date/Time Format 1 <datetime-format1>`. If the value is **NULL**, **NULL** is returned.
    :param date_lang_string_literal: Specifies the language for the input value to be applied.
    :rtype: TIMESTAMP

When the *format* argument is specified, the *string* is interpreted according to the specified language. The language is set by *date_lang_string_literal* argument. If *date_lang_string_literal* argument is not set, the language is specified by the **intl_date_lang** system parameter; if the value of **intl_date_lang** is not set, the language specified when creating DB is applied. 

For example, when a language is "de_DE" and *string* is "12/mai/2012 12:10:00 Nachm.", and *format* is "DD/MON/YYYY HH:MI:SS AM", it is interpreted as May 12th, 2012, 12:10:00 AM. When the *format* parameter specified does not correspond to the given string, an error is returned.

When the *format* argument is omitted, *string* is interpreted as the CUBRID default format(refer to :ref:`cast-to-datetime-recommend`) and if it fails, *string* is interpreted as the language format (see the table :ref:`Default Output Format of Language <tochar-default-datetime-format>` in the :func:`TO_CHAR`) by **intl_date_lang**. If the value of **intl_date_lang** is not set, the language is applied by the language specified when creating DB.

For example, when a language is "de_DE", the acceptable strings for **TIMESTAMP** type are "HH:MI:SS AM MM/DD/YYYY", CUBRID default format and "HH24:MI:SS DD.MM.YYYY", "de_DE" default format.

The following example shows execution of the database by setting the value of system parameter **intl_date_lang** as "en_US".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="en_US"';

    --selecting a timestamp type value casted from a string in the specified format
     
    SELECT TO_TIMESTAMP('13:10:30 12/25/2008');
    
::

    01:10:30 PM 12/25/2008
     
.. code-block:: sql

    SELECT TO_TIMESTAMP('08-Dec-25 13:10:30', 'YY-Mon-DD HH24:MI:SS');
    
::

    01:10:30 PM 12/25/2008
     
.. code-block:: sql

    SELECT TO_TIMESTAMP('YEAR: 2008 DATE: 12-25 TIME: 13:10:30', '"YEAR:" YYYY "DATE:" MM-DD "TIME:" HH24:MI:SS');
    
::

    01:10:30 PM 12/25/2008

The following example shows the case when the system parameter **intl_date_lang** is "de_DE".

.. code-block:: sql

    SET SYSTEM PARAMETERS 'intl_date_lang="de_DE"';
    SELECT TO_TIMESTAMP('13:10:30 25.12.2008');
    
::

    01:10:30 PM 12/25/2008
     
.. code-block:: sql

    SELECT TO_TIMESTAMP('10:23:00 Nachm.', 'HH12:MI:SS AM');
    
::

    10:23:00 PM 08/01/2012

.. note::

    When the charset is ISO-8859-1, the language that can be changed by the system parameter **intl_date_lang** is "ko_KR" and "tr_TR" except "en_US". If the charset is UTF-8, it can be changed to any language supported by CUBRID. For details, see :ref:`Note <tochar-remark>` in the :func:`TO_CHAR`).

TO_TIMESTAMP_TZ
===============

.. function:: TO_TIMESTAMP_TZ(string [, format [,date_lang_string_literal]])

    **TO_TIMESTAMP_TZ** function is the same as :func:`TO_TIMESTAMP` function except that this function can include a timezone information on this input string.

    :rtype: TIMESTAMPTZ

 .. code-block:: sql

    SELECT TO_TIMESTAMP_TZ('13:10:30 Asia/Seoul 12/25/2008', 'HH24:MI:SS TZR MM/DD/YYYY');
    
::

    01:10:30 PM 12/25/2008 Asia/Seoul
