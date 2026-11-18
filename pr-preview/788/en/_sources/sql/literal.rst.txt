
:meta-keywords: cubrid number, cubrid date time, cubrid string, cubrid character, cubrid collection, cubrid null, database literal
:meta-description: This section describes how to write a literal value in CUBRID.

*******
Literal
*******

This section describes how to write a literal value in CUBRID.

Number
======

There are two ways of writing a number; how to write an exact value and how to write an approximate value.

*   An exact number is written as a serial numbers and a dot (.); this input literal is translated as an INT, BIGINT or NUMERIC typed value based on its range.

    ::
    
        10, 123456789012, 1234234324234.23

*   An approximate number is written as a serial numbers, a dot (.) and E (scientific notation, multiples of 10); this input literal is translated as a DOUBLE typed value.

    ::
    
        1.2345E15, 12345E5

*   + or - symbol can be written in front of a number, and this can be written between E which indicates multiples of 10 and a number.

    ::
    
        +10.2345, -1.2345E-15

.. _date-time-literal:

Date/Time
=========

For representing data and time, there are DATE, TIME, DATETIME and TIMESTAMP types; these values can be represented as adding date, time, datetime and timestamp literals (case-insensitive) in front of their strings.

If you use date/time literals, you don't need to use converting functions such as :func:`TO_DATE`, :func:`TO_TIME`, :func:`TO_DATETIME` and :func:`TO_TIMESTAMP`.
However, the writing order of a string which indicates date or time.

*   The date literal only allows 'YYYY-MM-DD' or 'MM/DD/YYYY'.

    ::
    
        date'1974-12-31', date'12/31/1974'


*   The time literal only allows 'HH:MI:SS', 'HH:MI:SS AM' or 'HH:MI:SS PM'.

    ::
        
        time'12:13:25', time'12:13:25 AM', time'12:13:25 PM'

*   The date/time literal used in DATETIME type allows 'YYYY-MM-DD HH:MI:SS[.msec AM|PM]' or 'MM/DD/YYYY HH:MI:SS[.msec AM|PM]'. msec means milliseconds, which can be written until 3 digits.

    ::
    
        datetime'1974-12-31 12:13:25.123 AM', datetime'12/31/1974 12:13:25.123 AM'

*   The date/time literal used in TIMESTAMP type allows 'YYYY-MM-DD HH:MI:SS[ AM|PM]' or 'MM/DD/YYYY HH:MI:SS[ AM|PM]'.

    ::
    
        timestamp'1974-12-31 12:13:25 AM', timestamp'12/31/1974 12:13:25 AM'
        
*   The literal of date/time with timezone type has the same format as the above, and add an offset or a region name which indicates a timezone information. 

    *   Add datetimetz, datetimeltz, timestamptz or timestampltz literal at the front of a string to represent each type's value.

        ::
        
            datetimetz'10/15/1986 5:45:15.135 am +02:30:20';
            datetimetz'10/15/1986 5:45:15.135 am +02:30';
            datetimetz'10/15/1986 5:45:15.135 am +02';
            datetimeltz'10/15/1986 5:45:15.135 am Europe/Bucharest'
            datetimetz'2001-10-11 02:03:04 AM Europe/Bucharest EEST';
            timestampltz'10/15/1986 5:45:15 am Europe/Bucharest'
            timestamptz'10/15/1986 5:45:15 am Europe/Bucharest'
 
    *   The literal at the front of a string can be replaced with "<date/time type> WITH TIMEZONE" or <date/time type> WITH LOCAL TIME ZONE.

        ::
            DATETIME WITH TIMEZONE = datetimetz
            DATETIME WITH LOCAL TIMEZONE = datetimeltz
            TIMESTAMP WITH TIMEZONE = timestamptz
            TIMESTAMP WITH LOCAL TIMEZONE = timestampltz
    
        ::
        
            DATETIME WITH TIME ZONE'10/15/1986 5:45:15.135 am +02';
            DATETIME WITH LOCAL TIME ZONE'10/15/1986 5:45:15.135 am +02';

    .. note::
    
        *   <date/time type> WITH LOCAL TIME ZONE: internally stores UTC time; it is converted as a local (current session) timezone when it is output.
        *   <date/time type> WITH TIME ZONE: internally stores UTC time and timezone information (decided by a user or a session timezone) when this value is created.

Bit String
==========

Bit string uses two formats of binary format and hexadecimal format.

Binary format is written as adding B or 0b in front of a number; the input value is a string with 0 and 1 after B, and a number with 0 and 1 after 0b.

::

    B'10100000'
    0b10100000
    
Binary number is written by 8 digits; if the input digits are not divided into 8, this value is saved as 0s are attached. For example, B'1' is saved as B'10000000'.

Hexadecimal format is written as adding X or 0x in front of a number; the input value is a string with hexadecimal after X, and a number with hexadecimal after 0x.

::

    X'a0'
    0xA0

Hexadecimal number is written by 2 digits; if the input digits are not divided into 2, this value is saved as 0s are attached. For example, X'a' is saved as X'a0'.

Character String
================

Character string is written as wrapped in single quotes.

*   If you want to include a single quote in a string, input it twice serially.

    .. code-block:: sql
    
        SELECT 'You''re welcome.';

*   An escape using a backslash can be used if you set **no_backslash_escapes** in  **cubrid.conf** as no. But this default value is yes.

    For details, see :ref:`escape-characters`.

*   Charset introducer can be located in front of a string, and COLLATE modifier can be located after a string.

    For details, see :ref:`charset-introducer`.

Collection
==========

In collection types, there are SET, MULTISET and LIST; their values are written as elements are wrapped in braces ({, }).

::

    {'c','c','c','b','b','a'}

For details, see :ref:`collection-data-type`.

NULL
====

NULL value means there is no data. NULL is case-insensitive, so it also can be written as null.
Please note that NULL value is not 0 in a number type or an empty string ('') in a string type.
