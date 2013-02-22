******************************
Numeric and Operator Functions
******************************

ABS
===

.. function:: ABS (number_expr)

    The **ABS** function returns the absolute value of a given number. The data type of the return value is the same as that of the argument.

    :param number_expr: An operator which returns a numeric value
    :rtype: same as that of the argument

.. code-block:: sql

    --it returns the absolute value of the argument
    SELECT ABS(12.3), ABS(-12.3), ABS(-12.3000), ABS(0.0);
    
      abs(12.3)             abs(-12.3)            abs(-12.3000)         abs(0.0)
    ================================================================================
      12.3                  12.3                  12.3000               .0

ACOS
====

.. function:: ACOS ( x )

    The **ACOS** function returns an arc cosine value of the argument. That is, it returns a value whose cosine is *x* in radian. The return value is a **DOUBLE** type. x must be a value between -1 and 1, inclusive. Otherwise, **NULL** is returned.

    :param x: An expression that returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT ACOS(1), ACOS(0), ACOS(-1);
      acos(1)                   acos(0)                  acos(-1)
    ==================================================================================
      0.000000000000000e+00     1.570796326794897e+00     3.141592653589793e+00

ASIN
====

.. function:: ASIN ( x )

    The **ASIN** function returns an arc sine value of the argument. That is, it returns a value whose sine is *x* in radian. The return value is a **DOUBLE** type. x must be a value between -1 and 1, inclusive. Otherwise, **NULL** is returned.

    :param x: An expression that returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT ASIN(1), ASIN(0), ASIN(-1);
      asin(1)                   asin(0)                  asin(-1)
    ==============================================================================
      1.570796326794897e+00     0.000000000000000e+00    -1.570796326794897e+00

ATAN
====

.. function:: ATAN ( [y,] x )

    The **ATAN** function returns a value whose tangent is *x* in radian. The argument *y* can be omitted. If *y* is specified, the function calculates the arc tangent value of *y/x*. The return value is a **DOUBLE** type.

    :param x,y: An expression that returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT ATAN(1), ATAN(-1), ATAN(1,-1);
     
                       atan(1)                  atan(-1)              atan2(1, -1)
    ==============================================================================
         7.853981633974483e-01    -7.853981633974483e-01     2.356194490192345e+000

ATAN2
=====
     
.. function:: ATAN2 ( y, x )

    The **ATAN2** function returns the arc tangent value of *y/x* in radian. This function is working like the :func:`ATAN`. Arguments *x* and *y* must be specified. The return value is a **DOUBLE** type.

    :param x,y: An expression that returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT ATAN2(1,1), ATAN2(-1,-1), ATAN2(Pi(),0);
     
    atan2(1, 1)             atan2(-1, -1)           atan2( pi(), 0)
    ==============================================================================
     7.853981633974483e-01    -2.356194490192345e+00     1.570796326794897e+00

CEIL
====

.. function:: CEIL( number_operand )

    The **CEIL** function returns the smallest integer that is not less than its argument. The return value is determined based on the valid number of digits that are specified as the *number_operand* argument.

    :param number_operand: An expression that returns a numeric value
    :rtype: INT

.. code-block:: sql

    SELECT CEIL(34567.34567), CEIL(-34567.34567);
      ceil(34567.34567)     ceil(-34567.34567)
    ============================================
      34568.00000           -34567.00000
     
    SELECT CEIL(34567.1), CEIL(-34567.1);
      ceil(34567.1)         ceil(-34567.1)
    =============================
      34568.0         -34567.0

CONV
====

.. function:: CONV (number,from_base,to_base)

    The **CONV** function converts numbers between different number bases. This function returns a string representation of a converted number. The minimum value is 2 and the maximum value is 36. If *to_base* (representing the base to be returned) is negative, *number* is regarded as a signed number. Otherwise, it regarded as a unsigned number.

    :param number: An input number
    :param from_base: The base of an input number
    :param to_base: The base of an returned value
    :rtype: STRING

.. code-block:: sql

    SELECT CONV('f',16,2);
    '1111'

    SELECT CONV('6H',20,8);
    '211'

    SELECT CONV(-30,10,-20);
    '-1A'

COS
===

.. function:: COS ( x )

    The **COS** function returns a cosine value of the argument. The argument *x* must be a radian value. The return value is a **DOUBLE** type.

    :param x: An expression that returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT COS(pi()/6), COS(pi()/3), COS(pi());
      cos( pi()/6)              cos( pi()/3)                cos( pi())
    ==============================================================================
      8.660254037844387e-01     5.000000000000001e-01    -1.000000000000000e+00

COT
===

.. function:: COT ( x )

    The **COT** function returns the cotangent value of the argument *x*. That is, it returns a value whose tangent is *x* in radian. The return value is a **DOUBLE** type.

    :param x: An expression that returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT COT(1), COT(-1), COT(0);
      cot(1)                   cot(-1)   cot(0)
    ==========================================================================
      6.420926159343306e-01    -6.420926159343306e-01  NULL

DEGREES
=======

.. function:: DEGREES ( x )

    The **DEGREES** function returns the argument *x* specified in radian converted to a degree value. The return value is a **DOUBLE** type.

    :param x: An expression that returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT DEGREES(pi()/6), DEGREES(pi()/3), DEGREES (pi());
      degrees( pi()/6)          degrees( pi()/3)            degrees( pi())
    ==============================================================================
      3.000000000000000e+01     5.999999999999999e+01     1.800000000000000e+02

DRANDOM, DRAND
==============

.. function:: DRANDOM ( [seed] )
.. function:: DRAND ( [seed] )

    The function **DRANDOM** or **DRAND** returns a random double-precision floating point value in the range of between 0.0 and 1.0. A *seed* argument that is **INTEGER** type can be specified. It rounds up real numbers and an error is returned when it exceeds the range of **INTEGER**.

    The **DRAND** function performs the operation only once to produce only one random number regardless of the number of rows where the operation is output, but the **DRANDOM** function performs the operation every time the statement is repeated to produce a different random value for each row. Therefore, to output rows in a random order, you must use the **DRANDOM** function in the **ORDER BY** clause. To obtain a random integer value, use the :func:`RANDOM`.

    :param seed: 
    :rtype: DOUBLE

.. code-block:: sql

    SELECT DRAND(), DRAND(1), DRAND(1.4);
                       drand()                  drand(1)                drand(1.4)
    ==============================================================================
        2.849646518006921e-001    4.163034446537495e-002    4.163034446537495e-002
     
    SELECT * FROM rand_tbl;
               id  name
    ===================================
                1  'a'
                2  'b'
                3  'c'
                4  'd'
                5  'e'
                6  'f'
                7  'g'
                8  'h'
                9  'i'
               10  'j'
     
    --drandom() returns random values on every row
    SELECT DRAND(), DRANDOM() FROM rand_tbl;
       drand()                 drandom()
    ==============================================================================
       7.638782921842098e-001    1.018707846308786e-001
       7.638782921842098e-001    3.191320535905026e-001
       7.638782921842098e-001    3.461714529862361e-001
       7.638782921842098e-001    6.791894283883175e-001
       7.638782921842098e-001    4.533829767754143e-001
       7.638782921842098e-001    1.714224677266762e-001
       7.638782921842098e-001    1.698049867244484e-001
       7.638782921842098e-001    4.507583849604786e-002
       7.638782921842098e-001    5.279091769157994e-001
       7.638782921842098e-001    7.021088290047914e-001
     
    --selecting rows in random order
    SELECT * FROM rand_tbl ORDER BY DRANDOM();
               id  name
    ===================================
                6  'f'
                2  'b'
                7  'g'
                8  'h'
                1  'a'
                4  'd'
               10  'j'
                9  'i'
                5  'e'
                3  'c'

EXP
===

.. function:: EXP( x )

    The **EXP** function returns e x (the base of natural logarithm) raised to a power.

    :param x: An operator which returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT EXP(1), EXP(0);
      exp(1)                    exp(0)
    ====================================================
      2.718281828459045e+000 1.000000000000000e+000
     
    SELECT EXP(-1), EXP(2.00);
      exp(-1)                 exp(2.00)
    ====================================================
      3.678794411714423e-001 7.389056098930650e+000

FLOOR
=====

.. function:: FLOOR( number_operand )

    The **FLOOR** function returns the largest integer that is not greater than its argument. The data type of the return value is the same as that of the argument.

    :param number_operand: An operator which returns a numeric value
    :rtype: same as that of the argument

.. code-block:: sql

    --it returns the largest integer less than or equal to the arguments
    SELECT FLOOR(34567.34567), FLOOR(-34567.34567);
      floor(34567.34567)    floor(-34567.34567)
    ============================================
      34567.00000           -34568.00000
     
    SELECT FLOOR(34567), FLOOR(-34567);
      floor(34567)   floor(-34567)
    =============================
             34567         -34567

GREATEST
========

.. function:: GREATEST( expression [, expression]* )

    The **GREATEST** function compares more than one expression specified as parameters and returns the greatest value. If only one expression has been specified, the expression is returned because there is no expression to be compared with.

    Therefore, more than one expression that is specified as parameters must be of the type that can be compared with each other. If the types of the specified parameters are identical, so are the types of the return values; if they are different, the type of the return value becomes a convertible common data type.

    That is, the **GREATEST** function compares the values of column 1, column 2 and column 3 in the same row and returns the greatest value while the **MAX** function compares the values of column in all result rows and returns the greatest value.

    :param expression: Specifies more than one expression. Their types must be comparable each other. One of the arguments is **NULL**, **NULL** is returned.
    :rtype: same as that of the argument
    
The following example shows how to retrieve the number of every medals and the highest number that Korea won in the *demodb* database.

.. code-block:: sql

    SELECT gold, silver , bronze, GREATEST (gold, silver, bronze) FROM participant
    WHERE nation_code = 'KOR';
             gold       silver       bronze  greatest(gold, silver, bronze)
    =======================================================================
                9           12            9                              12
                8           10           10                              10
                7           15            5                              15
               12            5           12                              12
               12           10           11                              12

HEX
===

.. function:: HEX(n)

    The **HEX** function returns a decimal string if a hexadecimal string is specified as an argument; it returns a hexadecimal string if a decimal string is specified as an argument. If a number is specified as an argument, it returns a value like CONV(num, 10, 16).

    :param n: A hexadecimal string or A decimal string
    :rtype: STRING
    
.. code-block:: sql

    SELECT HEX('ab'), HEX(128), CONV(HEX(128), 16, 10);
    hex('ab')             hex(128)              conv(hex(128), 16, 10)
    ==================================================================
      '6162'                '80'                  '128'

LEAST
=====

.. function:: LEAST( expression [, expression]* )

    The **LEAST** function compares more than one expression specified as parameters and returns the smallest value. If only one expression has been specified, the expression is returned because there is no expression to be compared with.

    Therefore, more than one expression that is specified as parameters must be of the type that can be compared with each other. If the types of the specified parameters are identical, so are the types of the return values; if they are different, the type of the return value becomes a convertible common data type.

    That is, the **LEAST** function compares the values of column 1, column 2 and column 3 in the same row and returns the smallest value while the :func:`MIN` compares the values of column in all result rows and returns the smallest value. 

    :param expression: Specifies more than one expression. Their types must be comparable each other. One of the arguments is **NULL**, **NULL** is returned.
    :rtype: same as that of the argument

The following example shows how to retrieve the number of every medals and the lowest number that Korea won in the *demodb* database.

.. code-block:: sql

    SELECT gold, silver , bronze, LEAST(gold, silver, bronze) FROM participant
    WHERE nation_code = 'KOR';
             gold       silver       bronze  least(gold, silver, bronze)
    ====================================================================
                9           12            9                            9
                8           10           10                            8
                7           15            5                            5
               12            5           12                            5
               12           10           11                           10

LN
==

.. function:: LN ( x )

    The **LN** function returns the natural log value (base = e) of an antilogarithm *x*. The return value is a **DOUBLE** type. If the antilogarithm is 0 or a negative number, an error is returned.

    :param x: An expression that returns a positive number
    :rtype: DOUBLE

.. code-block:: sql

    SELECT ln(1), ln(2.72);
         ln(1)                     ln(2.72)
    =====================================================
         0.000000000000000e+00     1.000631880307906e+00

LOG2
====

.. function:: LOG2 ( x )

    The **LOG2** function returns a log value whose antilogarithm is *x* and base is 2. The return value is a **DOUBLE** type. If the antilogarithm is 0 or a negative number, an error is returned.

    :param x: An expression that returns a positive number
    :rtype: DOUBLE

.. code-block:: sql

    SELECT log2(1), log2(8);
         log2(1)                   log2(8)
    ======================================================
         0.000000000000000e+00     3.000000000000000e+00  

LOG10
=====

.. function:: LOG10 ( x )

    The **LOG10** function returns the common log value of an antilogarithm *x*. The return value is a **DOUBLE** type. If the antilogarithm is 0 or a negative number, an error is returned.

    :param x: An expression that returns a positive number
    :rtype: DOUBLE

.. code-block:: sql

    SELECT log10(1), log10(1000);
         log10(1)                  log10(1000)
    ====================================================
         0.000000000000000e+00     3.000000000000000e+00

MOD
===

.. function:: MOD (m, n)

    The **MOD** function returns the remainder of the first parameter *m* divided by the second parameter *n*. If *n* is 0, *m* is returned without the division operation being performed. Note that if the dividend, the parameter m of the **MOD** function, is a negative number, the function operates differently from a typical operation (classical modulus) method.

    **Result of MOD**

    +-------+-------+---------------+-----------------------+
    | m     | n     | MOD(m, n)     | Classical Modulus     |
    |       |       |               | m-n*FLOOR(m/n)        |
    +=======+=======+===============+=======================+
    | 11    | 4     | 3             | 3                     |
    +-------+-------+---------------+-----------------------+
    | 11    | -4    | 3             | -1                    |
    +-------+-------+---------------+-----------------------+
    | -11   | 4     | -3            | 1                     |
    +-------+-------+---------------+-----------------------+
    | -11   | -4    | -3            | -3                    |
    +-------+-------+---------------+-----------------------+
    | 11    | 0     | 11            | Divided by 0 error    |
    +-------+-------+---------------+-----------------------+

    :param m: Represents a dividend. It is an expression that returns a numeric value.
    :param n: Represents a divisor. It is an expression that returns a numeric value.
    :rtype: INT

.. code-block:: sql

    --it returns the reminder of m divided by n
    SELECT MOD(11, 4), MOD(11, -4), MOD(-11, 4), MOD(-11, -4), MOD(11,0);
        mod(11, 4)   mod(11, -4)   mod(-11, 4)   mod(-11, -4)   mod(11, 0)
    =====================================================================
                3             3            -3             -3           11
     
    SELECT MOD(11.0, 4), MOD(11.000, 4), MOD(11, 4.0), MOD(11, 4.000);
      mod(11.0, 4)          mod(11.000, 4)        mod(11, 4.0)          mod(11, 4.000)
    =========================================================================
      3.0                   3.000                 3.0                   3.000

PI
==

.. function:: PI ()

    The **PI** function returns the π value of type **DOUBLE**.

    :rtype: DOUBLE

.. code-block:: sql

    SELECT PI(), PI()/2;
         pi()                      pi()/2
    ====================================================
         3.141592653589793e+00     1.570796326794897e+00

POW, POWER
==========

.. function:: POW( x, y )
.. function:: POWER( x, y )

    The **POW** function returns *x* to the power of *y*. The functions **POW** and **POWER** are used interchangeably. The return value is a **DOUBLE** type.

    :param x: It represents the base. It is an expression that returns a numeric value. An expression that returns a numeric value.
    :param y: It represents the exponent. An expression that returns a numeric value. If the base is a negative number, an integer must specified as the exponent.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT POWER(2, 5), POWER(-2, 5), POWER(0, 0), POWER(1,0);
     power(2, 5)            power(-2, 5)           power(0, 0)           power(1, 0)
    ======================================================================================
     3.200000000000000e+01    -3.200000000000000e+01     1.000000000000000e+00     1.000000000000000e+00
     
    --it returns an error when the negative base is powered by a non-int exponent
    SELECT POWER(-2, -5.1), POWER(-2, -5.1);
     
    ERROR

RADIANS
=======

.. function:: RADIANS ( x )

    The **RADIANS** function returns the argument *x* specified in degrees converted to a radian value. The return value is a **DOUBLE** type.

    :param x: An expression that returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT RADIANS(90), RADIANS(180), RADIANS(360);
         radians(90)               radians(180)              radians(360)
    ==============================================================================
         1.570796326794897e+00     3.141592653589793e+00     6.283185307179586e+00

RANDOM, RAND
============

.. function:: RANDOM ( [seed] )
.. function:: RAND ( [seed] )

    The function **RANDOM** or **RAND** returns any integer value, which is greater than or equal to 0 and less than 2 31, and a *seed* argument that is **INTEGER** type can be specified. It rounds up real numbers and an error is returned when it exceeds the range of **INTEGER**.

    The **RAND** function performs the operation only once to produce only one random number regardless of the number of rows where the operation is output, but the **RANDOM** function performs the operation every time the statement is repeated to produce a different random value for each row. Therefore, to output rows in a random order, you must use the **RANDOM** function. To obtain a random real number, use the :func:`DRANDOM`.

    :param seed: 
    :rtype: INT

.. code-block:: sql

    SELECT RAND(), RAND(1), RAND(1.4);
           rand()      rand(1)    rand(1.4)
    =======================================
       1526981144     89400484     89400484
     
    --creating a new table
    SELECT * FROM rand_tbl;
               id  name
    ===================================
                1  'a'
                2  'b'
                3  'c'
                4  'd'
                5  'e'
                6  'f'
                7  'g'
                8  'h'
                9  'i'
               10  'j'
     
    --random() returns random values on every row
    SELECT RAND(),RANDOM() FROM rand_tbl;
           rand()       random()
    ============================
       2078876566     1753698891
       2078876566     1508854032
       2078876566      625052132
       2078876566      279624236
       2078876566     1449981446
       2078876566     1360529082
       2078876566     1563510619
       2078876566     1598680194
       2078876566     1160177096
       2078876566     2075234419
     
     
    --selecting rows in random order
    SELECT * FROM rand_tbl ORDER BY RANDOM();
               id  name
    ===================================
                6  'f'
                1  'a'
                5  'e'
                4  'd'
                2  'b'
                7  'g'
               10  'j'
                9  'i'
                3  'c'
                8  'h'

ROUND
=====

.. function:: ROUND ( number_operand, integer )

    The **ROUND** function returns the specified argument, *number_operand*, rounded to the number of places after the decimal point specified by the *integer*. If the *integer* argument is a negative number, it rounds to a place before the decimal point, that is, at the integer part.

    :param number_operand: An expression that returns a numeric value
    :param integer: Specifies the place to round to. If a positive integer *n* is specified, the number is represented to the nth place after the decimal point; if a negative integer *n* is specified, the number is rounded to the *n* th place before the decimal point.
    :rtype: same type as the *number_operand*

.. code-block:: sql

    --it rounds a number to one decimal point when the second argument is omitted
    SELECT ROUND(34567.34567), ROUND(-34567.34567);
      round(34567.34567, 0)   round(-34567.34567, 0)
    ============================================
      34567.00000           -34567.00000
     
     
    --it rounds a number to three decimal point
    SELECT ROUND(34567.34567, 3), ROUND(-34567.34567, 3)  FROM db_root;
     round(34567.34567, 3)   round(-34567.34567, 3)
    ============================================
      34567.34600           -34567.34600
     
    --it rounds a number three digit to the left of the decimal point
    SELECT ROUND(34567.34567, -3), ROUND(-34567.34567, -3);
     round(34567.34567, -3)   round(-34567.34567, -3)
    ============================================
      35000.00000           -35000.00000

SIGN
====

.. function:: SIGN (number_operand)

    The **SIGN** function returns the sign of a given number. It returns 1 for a positive value, -1 for a negative value, and 0 for zero.

    :param number_operand: An operator which returns a numeric value
    :rtype: INT

.. code-block:: sql

    --it returns the sign of the argument
     
    SELECT SIGN(12.3), SIGN(-12.3), SIGN(0);
        sign(12.3)   sign(-12.3)      sign(0)
    ========================================
                1            -1            0

SIN
===

.. function:: SIN ( x )

    The **SIN** function returns a sine value of the parameter. The argument *x* must be a radian value. The return value is a **DOUBLE** type.

    :param x: An expression that returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT SIN(pi()/6), SIN(pi()/3), SIN(pi());
         sin( pi()/6)              sin( pi()/3)              sin( pi())
    ==============================================================================
         4.999999999999999e-01     8.660254037844386e-01     1.224646799147353e-16

SQRT
====

.. function:: SQRT ( x )

    The **SQRT** function returns the square root of *x* as a **DOUBLE** type.

    :param x: An expression that returns a numeric value. An error is returned if this value is a negative number.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT SQRT(4), SQRT(16.0);
         sqrt(4)                   sqrt(16.0)
    ====================================================
         2.000000000000000e+00     4.000000000000000e+00

TAN
===

.. function:: TAN ( x )

    The **TAN** function returns a tangent value of the argument. The argument *x* must be a radian value. The return value is a **DOUBLE** type.  

    :param x: An expression that returns a numeric value
    :rtype: DOUBLE

.. code-block:: sql

    SELECT TAN(pi()/6), TAN(pi()/3), TAN(pi()/4);
         tan( pi()/6)              tan( pi()/3)              tan( pi()/4)
    ==============================================================================
         5.773502691896257e-01     1.732050807568877e+00     9.999999999999999e-01

TRUNC, TRUNCATE
===============

.. function:: TRUNC ( x[, dec] )
.. function:: TRUNCATE ( x, dec )

    The function **TRUNC** or **TRUNCATE** truncates the numbers of the specified argument *x* to the right of the *dec* position. If the *dec* argument is a negative number, it displays 0s to the *dec-* th position left to the decimal point. Note that the *dec* argument of the **TRUNC** function can be omitted, but that of the **TRUNCATE** function cannot be omitted. If the *dec* argument is a negative number, it displays 0s to the *dec* -th position left to the decimal point. The number of digits of the return value to be represented follows the argument *x*.

    :param x: An expression that returns a numeric value
    :param dec: The place to be truncated is specified. If a positive integer *n* is specified, the number is represented to the *n-*\th place after the decimal point; if a negative integer *n* is specified, the number is truncated to the *n-*\th place before the decimal point. It truncates to the first place after the decimal point if the *dec* argument is 0 or omitted. Note that the *dec* argument cannot be omitted in the **TRUNCATE** function.
    :rtype: same type as the *x*
    
.. code-block:: sql

    --it returns a number truncated to 0 places
    SELECT TRUNC(34567.34567), TRUNCATE(34567.34567, 0);
      trunc(34567.34567, 0)   trunc(34567.34567, 0)
    ============================================
      34567.00000            34567.00000
     
    --it returns a number truncated to three decimal places
    SELECT TRUNC(34567.34567, 3), TRUNC(-34567.34567, 3);
      trunc(34567.34567, 3)   trunc(-34567.34567, 3)
    ============================================
      34567.34500           -34567.34500
     
    --it returns a number truncated to three digits left of the decimal point
    SELECT TRUNC(34567.34567, -3), TRUNC(-34567.34567, -3);
      trunc(34567.34567, -3)   trunc(-34567.34567, -3)
    ============================================
      34000.00000           -34000.00000

WIDTH_BUCKET
============
[번역]

.. function:: WIDTH_BUCKET(expression, from, to, num_buckets)

    **WIDTH_BUCKET** 함수는 순차적인 데이터 집합을 균등한 범위로 부여된 일련의 버킷으로 나누며, 각 행에 적당한 버킷 번호를 1부터 할당한다. 반환되는 값은 정수이다.
    
    이 함수는 주어진 버킷 개수로 범위를 균등하게 나누어 버킷 번호를 부여한다. 즉, 버킷마다 각 범위의 넓이는 균등하다.
    ( :func:`NTILE` 함수는 이에 비해 주어진 버킷 개수로 전체 행의 개수를 균등하게 나누어 버킷 번호를 부여한다. 즉, 버킷마다 각 행의 개수는 균등하다.)

    expression은 버킷 번호를 부여받기 위한 입력 데이터이다. *from* 과 *to* 값으로 숫자형 타입과 날짜/시간 타입의 값 또는 날짜/시간 타입으로 변환 가능한 문자열이 입력될 수 있다.
    
    전체 범위에서 *from* 은 범위에 포함되지만 *to* 는 범위 밖에 존재한다. 예를 들어 WIDTH_BUCKET(score, 80, 50, 3)이 반환하는 값은 score가 
    
        * 80보다 크면 0, 
        * [80,70)이면  1, 
        * [70, 60)이면  2, 
        * [60, 50)이면 3, 
        * 50 또는 50보다 작으면 4가 된다.
    
    :param expression: 버킷 번호를 부여받기 위한 입력 값. 수치 값을 반환하는 임의의 연산식을 지정한다.
    :param from: expression이 취할 수 있는 범위의 시작값으로, 이 값은 전체 범위 안에 포함된다. 
    :param to: expression이 취할 수 있는 범위의 마지막 값으로, 이 값은 전체 범위 안에 포함되지 않는다.
    :param num_buckets: 버킷의 개수. 추가로 범위 밖의 내용을 담기 위한 0번 버킷과 (num_buckets + 1)번 버킷이 생성된다.
    :rtype: INT

다음 예제는 80점보다 작거나 같고 50점보다 큰 범위를 1부터 3까지 균등한 점수 범위로 나누어 등급을 부여한다. 해당 범위를 벗어나는 경우 80점보다 크면 0, 50점이거나 50점보다 작으면 4등급을 부여한다.

.. code-block:: sql

    CREATE TABLE t_score(name VARCHAR(10), score INT);
    INSERT INTO t_score VALUES
        ('Amie', 60),
        ('Jane', 80),
        ('Lora', 60),
        ('James', 75),
        ('Peter', 70),
        ('Tom', 50),
        ('Ralph', 99),
        ('David', 55);

    SELECT name, score, WIDTH_BUCKET(score, 80, 50, 3) grade FROM t_score ORDER BY grade ASC, score DESC;
    
      name                        score        grade
    ================================================
      'Ralph'                        99            0
      'Jane'                         80            1
      'James'                        75            1
      'Peter'                        70            2
      'Amie'                         60            3
      'Lora'                         60            3
      'David'                        55            3
      'Tom'                          50            4

NTILE 함수와 비교한 예제는 :func:`NTILE` 함수를 참고한다.
