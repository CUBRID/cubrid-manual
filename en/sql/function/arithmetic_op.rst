********************
Arithmetic Operators
********************

Arithmetic Operators
====================

For arithmetic operators, there are binary operators for addition, subtraction, multiplication, or division, and unary operators to represent whether the number is positive or negative. The unary operators to represent the numbers' positive/negative status have higher priority over the binary operators.

::

    <expression>  <mathematical_operator>  <expression>
     
        <expression> ::=
            bit_string |
            character_string |
            numeric_value |
            date-time_value |
            collection_value |
            NULL
     
        <mathematical_operator> ::=
            <set_arithmetic_operator> |
            <arithmetic_operator>
     
                <arithmetic_operator> ::=
                    + |
                    - |
                    * |
                    { / | DIV } |
                    { % | MOD }
         
                <set_arithmetic_operator> ::=
                    UNION |
                    DIFFERENCE |
                    { INTERSECT | INTERSECTION }

*   <*expression*>: Declares the mathematical operation to be calculated.
*   <*mathematical_operator*>: A operator that performs an operation the arithmetic and the set operators are applicable.

    *   <*set_arithmetic_operator*>: A set arithmetic operator that performs operations such as union, difference and intersection on collection type operands.
    *   <*arithmetic_operator*>: An operator to perform the four fundamental arithmetic operations.

The following table shows the arithmetic operators supported by CUBRID and their return values.

**Arithmetic Operators**

+-------------------------+----------------------------------------------------------------------------------------------------+--------------+------------------+
| Arithmetic Operator     | Description                                                                                        | Operator     | Return Value     |
+=============+================================================================================================================+==============+==================+
| **+**                   | Addition                                                                                           | 1+2          | 3                |
+-------------------------+----------------------------------------------------------------------------------------------------+--------------+------------------+
| **-**                   | Subtraction                                                                                        | 1-2          | -1               |
+-------------------------+----------------------------------------------------------------------------------------------------+--------------+------------------+
| **\***                  | Multiplication                                                                                     | 1*2          | 2                |
+-------------------------+----------------------------------------------------------------------------------------------------+--------------+------------------+
| **/**                   | Division. Returns quotient.                                                                        | 1/2.0        | 0.500000000      |
+-------------------------+----------------------------------------------------------------------------------------------------+--------------+------------------+
| **DIV**                 | Division. Returns quotient.                                                                        | 1 DIV 2      | 0                |
+-------------------------+----------------------------------------------------------------------------------------------------+--------------+------------------+
| **%**                   | Division. Returns quotient. An operator must be an integer type, and it always returns integer.    | 1 % 2        | 1                |
| ,                       | If an operand is real number, the **MOD**                                                          | 1 MOD 2      |                  |
| **MOD**                 | function can be used.                                                                              |              |                  |
+-------------------------+----------------------------------------------------------------------------------------------------+--------------+------------------+

.. _numeric-data-type-op-and-conversion:

Arithmetic Operations and Type Casting of Numeric Data Types
============================================================

All numeric data types can be used for arithmetic operations. The result type of the operation differs depending on the data types of the operands and the type of the operation. The following table shows the result data types of addition/subtraction/multiplication for each operand type.

**Result Data Type by Operand Type**

+--------------+--------------+---------------------+--------------+--------------+
|              | INT          | NUMERIC             | FLOAT        | DOUBLE       |
+==============+==============+=====================+==============+==============+
| **INT**      | INT or       | NUMERIC             | FLOAT        | DOUBLE       |
|              | BIGINT       |                     |              |              |
+--------------+--------------+---------------------+--------------+--------------+
| **NUMERIC**  | NUMERIC      | NUMERIC             | DOUBLE       | DOUBLE       |
|              |              | (p and s are also   |              |              |
|              |              | converted)          |              |              |
+--------------+--------------+---------------------+--------------+--------------+
| **FLOAT**    | FLOAT        | DOUBLE              | FLOAT        | DOUBLE       |
+--------------+--------------+---------------------+--------------+--------------+
| **DOUBLE**   | DOUBLE       | DOUBLE              | DOUBLE       | DOUBLE       |
+--------------+--------------+---------------------+--------------+--------------+

Note that the result type of the operation does not change if all operands are of the same data type but type casting occurs exceptionally in division operations. An error occurs when a denominator, i.e. a divisor, is 0.

The following table shows the total number of digits (*p*) and the number of digits after the decimal point (*s*) of the operation results when all operands are of the **NUMERIC** type. 

**Result of NUMERIC Type Operation**

+-----------------------+--------------------------------------------------------------------------------------------------------------------------+-------------------+
| Operation             | Maximum Precision                                                                                                        | Maximum Scale     |
+=======================+==========================================================================================================================+===================+
| N(p1, s1) + N(p2, s2) | max(p1-s1, p2-s2)+max(s1, s2) +1                                                                                         | max(s1, s2)       |
+-----------------------+--------------------------------------------------------------------------------------------------------------------------+-------------------+
| N(p1, s1) - N(p2, s2) | max(p1-s1, p2-s2)+max(s1, s2)                                                                                            | max(s1, s2)       |
+-----------------------+--------------------------------------------------------------------------------------------------------------------------+-------------------+
| N(p1, s1) * N(p2, s2) | p1+p2+1                                                                                                                  | s1+s2             |
+-----------------------+--------------------------------------------------------------------------------------------------------------------------+-------------------+
| N(p1, s1) / N(p2, s2) | Let Pt = p1+max(s1, s2) + s2 - s1 when s2 > 0 and Pt = p1 in other cases; St = s1 when s1 > s2 and s2 in other cases;    |                   |
|                       | the number of decimal places is min(9-St, 38-Pt) + St when St < 9 and St in other cases.                                 |                   |
+-----------------------+----------------------------------------------------------------------------------------------------------------------------------------------+

**Example**

.. code-block:: sql

    --int * int
    SELECT 123*123;
    
::

          123*123
    =============
            15129
     
.. code-block:: sql

    -- int * int returns overflow error
    SELECT (1234567890123*1234567890123);

::
    
    ERROR: Data overflow on data type bigint.
     
.. code-block:: sql

    -- int * numeric returns numeric type  
    SELECT (1234567890123*CAST(1234567890123 AS NUMERIC(15,2)));
    
::

     (1234567890123* cast(1234567890123 as numeric(15,2)))
    ======================
      1524157875322755800955129.00
     
.. code-block:: sql

    -- int * float returns float type
    SELECT (1234567890123*CAST(1234567890123 AS FLOAT));
    
::

     (1234567890123* cast(1234567890123 as float))
    ===============================================
                                      1.524158e+024
     
.. code-block:: sql

    -- int * double returns double type
    SELECT (1234567890123*CAST(1234567890123 AS DOUBLE));
    
::

     (1234567890123* cast(1234567890123 as double))
    ================================================
                              1.524157875322756e+024
     
.. code-block:: sql

    -- numeric * numeric returns numeric type   
    SELECT (CAST(1234567890123 AS NUMERIC(15,2))*CAST(1234567890123 AS NUMERIC(15,2)));
    
::

     ( cast(1234567890123 as numeric(15,2))* cast(1234567890123 as numeric(15,2)))
    ======================
      1524157875322755800955129.0000
     
.. code-block:: sql

    -- numeric * float returns double type  
    SELECT (CAST(1234567890123 AS NUMERIC(15,2))*CAST(1234567890123 AS FLOAT));
    
::

     ( cast(1234567890123 as numeric(15,2))* cast(1234567890123 as float))
    =======================================================================
                                                     1.524157954716582e+024
     
.. code-block:: sql

    -- numeric * double returns double type  
    SELECT (CAST(1234567890123 AS NUMERIC(15,2))*CAST(1234567890123 AS DOUBLE));
    
::

     ( cast(1234567890123 as numeric(15,2))* cast(1234567890123 as double))
    ========================================================================
                                                      1.524157875322756e+024
     
.. code-block:: sql

    -- float * float returns float type  
    SELECT (CAST(1234567890123 AS FLOAT)*CAST(1234567890123 AS FLOAT));
    
::

     ( cast(1234567890123 as float)* cast(1234567890123 as float))
    ===============================================================
                                                      1.524158e+024

.. code-block:: sql

    -- float * double returns float type  
    SELECT (CAST(1234567890123 AS FLOAT)*CAST(1234567890123 AS DOUBLE));
    
::

     ( cast(1234567890123 as float)* cast(1234567890123 as double))
    ================================================================
                                              1.524157954716582e+024
     
.. code-block:: sql

    -- double * double returns float type  
    SELECT (CAST(1234567890123 AS DOUBLE)*CAST(1234567890123 AS DOUBLE));
    
::

     ( cast(1234567890123 as double)* cast(1234567890123 as double))
    =================================================================
                                               1.524157875322756e+024
     
.. code-block:: sql

    -- int / int returns int type without type conversion or rounding
    SELECT 100100/100000;
    
::

      100100/100000
    ===============
                  1
     
.. code-block:: sql

    -- int / int returns int type without type conversion or rounding
    SELECT 100100/200200;
    
::

      100100/200200
    ===============
                  0
     
.. code-block:: sql

    -- int / zero returns error
    SELECT 100100/(100100-100100);
    
::

    ERROR: Attempt to divide by zero.

.. _arithmetic-op-type-casting:

Arithmetic Operations and Type Casting of DATE/TIME Data Types
==============================================================

If all operands are date/time type, only a subtraction operation is allowed and its return value is **BIGINT** . Note that the unit of the operation differs depending on the types of the operands. Both addition and subtraction operations are allowed in case of date/time and integer types In this case, operation units and return values are date/time data type.

The following table shows operations allowed for each operand type, and their result types.

**Allowable Operation and Result Data Type by Operand Type**

+---------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+
|               | **TIME**                                   | **DATE**                                   | **TIMESTAMP**                              | **DATETIME**                               | **INT**                                    |
|               | **(in seconds)**                           | **(in day)**                               | **(in seconds)**                           | **(in milliseconds)**                      |                                            |
+===============+============================================+============================================+============================================+============================================+============================================+
| **TIME**      | A subtraction is allowed.                  | X                                          | X                                          | X                                          | An addition and a subtraction are allowed. |
|               | **BIGINT**                                 |                                            |                                            |                                            | **TIME**                                   |
+---------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+
| **DATE**      | X                                          | A subtraction is allowed.                  | A subtraction is allowed.                  | A subtraction is allowed.                  | An addition and a subtraction are allowed. |
|               |                                            | **BIGINT**                                 | **BIGINT**                                 | **BIGINT**                                 | **DATE**                                   |
+---------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+
| **TIMESTAMP** | X                                          | A subtraction is allowed.                  | A subtraction is allowed.                  | A subtraction is allowed.                  | An addition and a subtraction are          |
|               |                                            | **BIGINT**                                 | **BIGINT**                                 | **BIGINT**                                 | allowed. **TIMESTAMP**                     |
+---------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+
| **DATETIME**  | X                                          | A subtraction is allowed.                  | A subtraction is allowed.                  | A subtraction is allowed.                  | An addition and a subtraction are allowed. |
|               |                                            | **BIGINT**                                 | **BIGINT**                                 | **BIGINT**                                 | **DATETIME**                               |
+---------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+
| **INT**       | An addition and a subtraction are allowed. | An addition and a subtraction are allowed. | An addition and a subtraction are allowed. | An addition and a subtraction are allowed. | All operations are allowed.                |
|               | **TIME**                                   | **DATE**                                   | **TIMESTAMP**                              | **DATETIME**                               |                                            |
+---------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+--------------------------------------------+

.. note:: 

    If any of the date/time arguments contains **NULL**,  **NULL** is returned.

**Example**

.. code-block:: sql

    -- initial systimestamp value
    SELECT SYSDATETIME;
    
::

      SYSDATETIME
    ===============================
      07:09:52.115 PM 01/14/2010
     
.. code-block:: sql

    -- time type + 10(seconds) returns time type
    SELECT (CAST (SYSDATETIME AS TIME) + 10);
    
::

     ( cast( SYS_DATETIME  as time)+10)
    ====================================
      07:10:02 PM
     
.. code-block:: sql

    -- date type + 10 (days) returns date type
    SELECT (CAST (SYSDATETIME AS DATE) + 10);
    
::

     ( cast( SYS_DATETIME  as date)+10)
    ====================================
      01/24/2010
     
.. code-block:: sql

    -- timestamp type + 10(seconds) returns timestamp type
    SELECT (CAST (SYSDATETIME AS TIMESTAMP) + 10);
    
::

     ( cast( SYS_DATETIME  as timestamp)+10)
    =========================================
      07:10:02 PM 01/14/2010
     
.. code-block:: sql

    -- systimestamp type + 10(milliseconds) returns systimestamp type
    SELECT (SYSDATETIME  + 10);
    
::

     ( SYS_DATETIME +10)
    ===============================
      07:09:52.125 PM 01/14/2010
     
.. code-block:: sql

    SELECT DATETIME '09/01/2009 03:30:30.001 pm'- TIMESTAMP '08/31/2009 03:30:30 pm';
    
::

     datetime '09/01/2009 03:30:30.001 pm'-timestamp '08/31/2009 03:30:30 pm'
    =======================================
      86400001
     
.. code-block:: sql

    SELECT TIMESTAMP '09/01/2009 03:30:30 pm'- TIMESTAMP '08/31/2009 03:30:30 pm';
    
::

     timestamp '09/01/2009 03:30:30 pm'-timestamp '08/31/2009 03:30:30 pm'
    =======================================
      86400
