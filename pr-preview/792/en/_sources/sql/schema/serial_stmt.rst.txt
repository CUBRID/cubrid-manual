
:meta-keywords: serial definition, create serial, alter serial, drop serial
:meta-description: Define serials in CUBRID database using create serial, alter serial and drop serial statements.

****************************
SERIAL DEFINITION STATEMENTS
****************************

CREATE SERIAL
=============

Serial is an object that creates a unique sequence number, and has the following characteristics.

*   The serial is useful in creating a unique sequence number in multi-user environment.
*   Generated serial numbers are not related with table so, you can use the same serial in multiple tables.
*   All users including **PUBLIC** can create a serial object. Once it is created, all users can get the number by using **CURRENT_VALUE(equivalent to CURRVAL)** and **NEXT_VALUE(equivalent to NEXTVAL)**.
*   Only owner of a created serial object and **DBA** can update or delete a serial object. If an owner is **PUBLIC**, all users can update or delete it.

You can create a serial object in the database by using the **CREATE SERIAL** statement. Regarding writing serial name, :doc:`/sql/identifier`. 

::

    CREATE SERIAL [schema_name.]serial_name
    [START WITH initial]
    [INCREMENT BY interval]
    [MINVALUE min | NOMINVALUE]
    [MAXVALUE max | NOMAXVALUE]
    [CYCLE | NOCYCLE]
    [CACHE cached_num | NOCACHE]
    [COMMENT 'comment_string'];

*   *schema_name*: Specifies the schema name of the serial(maximum: 31 bytes). If omitted, the schema name of the current session is used.
*   *serial_name*: specifies the name of the serial to be generated(maximum: 222 bytes).

*   **START WITH** *initial*: Specifies the initial value of serial. The range of this value is between -1,000,000,000,000,000,000,000,000,000,000,000,000(-10^36) and    9,999,999,999,999,999,999,999,999,999,999,999,999(10^37-1). The default value of ascending serial is 1 and that of descending serial is -1.

*   **INCREMENT BY** *interval*: Specifies the increment of the serial. You can specify any integer between -9,999,999,999,999,999,999,999,999,999,999,999,999(-10^37+1) and  9,999,999,999,999,999,999,999,999,999,999,999,999(10^37-1) except zero at *interval*. The absolute value of the *interval* must be smaller than the difference between **MAXVALUE** and **MINVALUE**. If a negative number is specified, the serial is in descending order otherwise, it is in ascending order. The default value is **1**.

*   **MINVALUE**: Specifies the minimum value of the serial. The range of this value is between -1,000,000,000,000,000,000,000,000,000,000,000,000(-10^36) and  9,999,999,999,999,999,999,999,999,999,999,999,999(10^37-1). **MINVALUE** must be smaller than or equal to the initial value and smaller than the maximum value.

*   **NOMINVALUE**: 1 is set automatically as a minimum value for the ascending serial, -1,000,000,000,000,000,000,000,000,000,000,000,000(-10^36) for the descending serial.

*   **MAXVALUE**: Specifies the maximum number of the serial. The range of this value is between -999,999,999,999,999,999,999,999,999,999,999,999(-10^36+1) and  10,000,000,000,000,000,000,000,000,000,000,000,000(10^37). **MAXVALUE** must be greater than or equal to the initial value and greater than the minimum value.

*   **NOMAXVALUE**: 10,000,000,000,000,000,000,000,000,000,000,000,000(10^37) is set automatically as a maximum value for the ascending serial, -1 for the descending serial.

*   **CYCLE**: Specifies that the serial will be generated continuously after reaching the maximum or minimum value. When a serial in ascending order reaches the maximum value, the minimum value is created as the next value; when a serial in descending order reaches the minimum value, the maximum value is created as the next value.

*   **NOCYCLE**: Specifies that the serial will not be generated anymore after reaching the maximum or minimum value. The default value is **NOCYCLE**.

*   **CACHE**: Stores as many serials as the number specified by "cached_num" in the cache to improve the performance of the serials and fetches a serial value when one is requested. If all cached values are used up, as many serials as "cached_num" are fetched again from the disk to the memory. If the database server stops accidentally, all cached serial values are deleted. For this reason, the serial values before and after the restart of the database server may be discontinuous. Because the transaction rollback does not affect the cached serial values, the request for the next serial will return the next value of the value used (or fetched) lastly when the transaction is rolled back. The "cached_num" after the **CACHE** keyword cannot be omitted. If the "cached_num" is equal to or smaller than 1, the serial cache is not applied.

*   **NOCACHE**: Specifies that the serial cache is not used, and serial value is updated for each time. The default value is **NOCACHE**.

*   *comment_string*: specifies a comment of a serial.

.. code-block:: sql

    --creating serial with default values
    CREATE SERIAL order_no;
     
    --creating serial within a specific range
    CREATE SERIAL order_no START WITH 10000 INCREMENT BY 2 MAXVALUE 20000;
    
    --creating serial with specifying the number of cached serial values
    CREATE SERIAL order_no START WITH 10000 INCREMENT BY 2 MAXVALUE 20000 CACHE 3;
     
    --selecting serial information from the db_serial class
    SELECT * FROM db_serial;

::

      name            current_val      increment_val         max_val         min_val         cyclic      started       cached_num        att_name
    ====================================================================================================================================================
    'order_no'      10006            2                     20000           10000                0            1                3            NULL

The following example shows how to create the *athlete_idx* table to store athlete codes and names and then create an instance by using the *order_no*. NEXT_VALUE increases the serial number and returns its value.

.. code-block:: sql

    CREATE TABLE athlete_idx( code INT, name VARCHAR(40) );
    CREATE SERIAL order_no START WITH 10000 INCREMENT BY 2 MAXVALUE 20000;
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Park');
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Kim');
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Choo');
    INSERT INTO athlete_idx VALUES (order_no.CURRENT_VALUE, 'Lee');
    
    SELECT * FROM athlete_idx;

::

             code  name
    ===================================
            10000  'Park'
            10002  'Kim'
            10004  'Choo'
            10004  'Lee'

COMMENT of Serial
-----------------

The below adds a comment when you create a serial.

.. code-block:: sql

    CREATE SERIAL order_no 
    START WITH 100 INCREMENT BY 2 MAXVALUE 200 
    COMMENT 'from 100 to 200 by 2';

To see a comment of the serial, run the below syntax.

.. code-block:: sql

    SELECT name, comment FROM db_serial;

To change a comment of a serial, see ALTER SERIAL syntax.

ALTER SERIAL
============

With the **ALTER SERIAL** statement, you can update the increment of the serial value, set or delete its initial or minimum/maximum values, and set its cycle attribute. 

::

    ALTER SERIAL [schema_name.]serial_name
    [INCREMENT BY interval]
    [START WITH initial_value]
    [MINVALUE min | NOMINVALUE]
    [MAXVALUE max | NOMAXVALUE]
    [CYCLE | NOCYCLE]
    [CACHE cached_num | NOCACHE]
    [COMMENT 'comment_string'];

*   *schema_name*: Specifies the schema name of the serial(maximum: 31 bytes). If omitted, the schema name of the current session is used.
*   *serial_name*: specifies the name of the serial to be created(maximum: 222 bytes).

*   **INCREMENT BY** *interval*: specifies the increment of the serial. For the *interval*, you can specify any integer with 38 digits or less except zero. The absolute value of the *interval* must be smaller than the difference between **MAXVALUE** and **MINVALUE**. If a negative number is specified, the serial is in descending order; otherwise, it is in ascending order. The default value is **1**.

*   **START WITH** *initial_value*: changes the initial value of Serial.

*   **MINVALUE**: specifies the minimum value of the serial with 38 digits or less. **MINVALUE** must be smaller than or equal to the initial value and smaller than the maximum value.

*   **NOMINVALUE**: 1 is set automatically as a minimum value for the ascending serial; -(10) 36 for the descending serial.

*   **MAXVALUE**: specifies the maximum number of the serial with 38 digits or less. **MAXVALUE** must be larger than or equal to the initial value and greater than the minimum value.

*   **NOMAXVALUE**: (10) 37 is set automatically as a maximum value for the ascending serial; -1 for the descending serial.

*   **CYCLE**: specifies that the serial will be generated continuously after reaching the maximum or minimum value. If the ascending serial reaches the maximum value, the minimum value is generated as the next value. If the descending serial reaches the minimum value, the maximum value is generated as the next value.

*   **NOCYCLE**: specifies that the serial will not be generated anymore after reaching the maximum or minimum value. The default is **NOCYCLE**.

*   **CACHE**: stores as many serials as the number specified by *integer* in the cache to improve the performance of the serials and fetches a serial value when one is requested. The *integer* after the **CACHE** keyword cannot be omitted. If a number equal to or smaller than 1 is specified, the serial cache is not applied.

*   **NOCACHE**: It does not use the serial cache feature. The serial value is updated every time and a new serial value is fetched from the disk upon each request. The default is **NOCACHE**.

*   *comment_string*: specifies a comment of a serial.

.. code-block:: sql

    --altering serial by changing start and incremental values
    ALTER SERIAL order_no START WITH 100 MINVALUE 100 INCREMENT BY 2;
     
    --altering serial to operate in cache mode
    ALTER SERIAL order_no CACHE 5;
     
    --altering serial to operate in common mode
    ALTER SERIAL order_no NOCACHE;
    
.. warning::

     In CUBRID 2008 R1.x version, the serial value can be modified by updating the db_serial table, a system catalog. However, in CUBRID 2008 R2.0 version or above, the modification of the db_serial table is not allowed but use of the **ALTER SERIAL** statement is allowed. Therefore, if an **ALTER SERIAL** statement is included in the data exported (unloaddb) from CUBRID 2008 R2.0 or above, it is not allowed to import (loaddb) the data in CUBRID 2008 R1.x or below.

.. warning::

    When you get the value of **NEXT_VALUE** after running **ALTER SERIAL**, in the version lower than CUBRID 9.0, the next value of the initial value was returned. From CUBRID 9.0, the setting value of **ALTER_SERIAL** is returned.

    ::
    
        CREATE SERIAL s1;
        SELECT s1.NEXTVAL;

        ALTER SERIAL s1 START WITH 10;
        
        SELECT s1.NEXTVAL;
        -- From 9.0, above query returns 10
        -- In the version less than 9.0, above query returns 11

The below changes the comment of the serial.

.. code-block:: sql

    ALTER SERIAL order_no COMMENT 'new comment';

DROP SERIAL
===========

With the **DROP SERIAL** statement, you can drop a serial object from the database. 
If you also specify **IF EXISTS** clause, no error will be happened even if a target serial does not exist.

::

    DROP SERIAL [ IF EXISTS ] [schema_name.]serial_name ;

*   *schema_name*: Specifies the schema name of the serial. If omitted, the schema name of the current session is used.
*   *serial_name*: Specifies the name of the serial to be dropped.

The following example shows how to drop the *order_no* serial.

.. code-block:: sql

    DROP SERIAL order_no;
    DROP SERIAL IF EXISTS order_no;

Accessing Serial
================

Pseudocolumns
-------------

You can access and update a serial by serial name and a pseudocolumn pair. ::

    [schema_name.]serial_name.CURRENT_VALUE
    [schema_name.]serial_name.CURRVAL

    [schema_name.]serial_name.NEXT_VALUE
    [schema_name.]serial_name.NEXTVAL

*   *schema_name*: Specifies the schema name of the serial. If omitted, the schema name of the current session is used.
*   *[schema_name.]serial_name*.\ **CURRENT_VALUE**, *[schema_name.]serial_name*.\ **CURRVAL**: Returns the current serial value.
*   *[schema_name.]serial_name*.\ **NEXT_VALUE**, *[schema_name.]serial_name*.\ **NEXTVAL**: Increments the serial value and returns the result.

The following example shows how to create a table *athlete_idx* where athlete numbers and names are stored and how to create the instances by using a serial *order_no*.

.. code-block:: sql

    CREATE TABLE athlete_idx (code INT, name VARCHAR (40));
    CREATE SERIAL order_no START WITH 10000 INCREMENT BY 2 MAXVALUE 20000;
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Park');
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Kim');
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Choo');
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Lee');
    SELECT * FROM athlete_idx;
    SELECT order_no.CURRENT_VALUE;
::
     
             code  name
    ===================================
            10000  'Park'
            10002  'Kim'
            10004  'Choo'
            10006  'Lee'

            serial_current_value(order_no)
    ======================
            10006

.. note:: 

    When you use a serial for the first time after creating it, **NEXT_VALUE** returns the initial value. Subsequently, the sum of the current value and the increment are returned.

Functions
---------

.. function:: SERIAL_CURRENT_VALUE ([schema_name.]serial_name)
.. function:: SERIAL_NEXT_VALUE ([schema_name.]serial_name, number)

    The **Serial** function consists of the **SERIAL_CURRENT_VALUE** and **SERIAL_NEXT_VALUE** functions.
    
    :param schema_name: The schema name of the serial
    :param serial_name: Serial name
    :param number: The number of serials to be obtained
    :rtype:  NUMERIC(38,0)

The **SERIAL_CURRENT_VALUE** function returns the current serial value, which is the same value as *serial_name* **.current_value**.

This function returns as much added value as interval specified. The serial interval is determined by the value of a **CREATE SERIAL ... INCREMENT BY** statement. **SERIAL_NEXT_VALUE** (*serial_name*, 1) returns the same value as *serial_name* **.next_value**.

To get a large amount of serials at once, specify the desired number as an argument to call the **SERIAL_NEXT_VALUE** function only once; which has an advantage over calling repeatedly *serial_name* **.next_value** in terms of performance.

Assume that an application process is trying to get the number of n serials at once. To perform it, call **SERIAL_NEXT_VALUE** (*serial_name*, N) one time to store a return value and calculate a serial value between (a serial start value) and (the return value). (Serial value at the point of function call) is equal to the value of (return value) - (desired number of serials) * (serial interval).

For example, if you create a serial starting 101 and increasing by 1 and call **SERIAL_NEXT_VALUE** (*serial_name*, 10), it returns 110. The start value at the point is 110-(10-1)*1 = 101. Therefore, 10 serial values such as 101, 102, 103, ... 110 can be used by an application process. If **SERIAL_NEXT_VALUE** (*serial_name*, 10) is called in succession, 120 is returned; the start value at this point is 120-(10-1)*1 = 111.

.. code-block:: sql

    CREATE SERIAL order_no START WITH 101 INCREMENT BY 1 MAXVALUE 20000;
    SELECT SERIAL_CURRENT_VALUE(order_no);
    
::

    101
     
.. code-block:: sql

    -- At first, the first serial value starts with the initial serial value, 10000. So the l0th serial value will be 10009.
    SELECT SERIAL_NEXT_VALUE(order_no, 10);
    
::

    110
     
.. code-block:: sql

    SELECT SERIAL_NEXT_VALUE(order_no, 10);
    
::

    120

.. note::

    If you create a serial and calls the **SERIAL_NEXT_VALUE** function for the first time, a value of (serial interval) * (desired number of serials - 1) added to the current value is returned. If you call the **SERIAL_NEXT_VALUE** function in succession, a value of (serial interval) * (desired number of serials) added to the current is returned (see the example above).
