:tocdepth: 3


**************
수치 연산 함수
**************

.. contents::

ABS
===

.. function:: ABS (number_expr)

    함수는 지정된 인자 값의 절대값을 반환하며, 리턴 값의 타입은 주어진 인자의 타입과 같다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param number_expr: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: number_expr의 타입

.. code-block:: sql

    --it returns the absolute value of the argument
    SELECT ABS(12.3), ABS(-12.3), ABS(-12.3000), ABS(0.0);

::
    
      abs(12.3)             abs(-12.3)            abs(-12.3000)         abs(0.0)
    ================================================================================
      12.3                  12.3                  12.3000               .0

ACOS
====

.. function:: ACOS ( x )

    **ACOS** 함수는 인자의 아크 코사인(arc cosine) 값을 반환한다. 즉, 코사인이 *x* 인 값을 라디안 단위로 반환하며, 리턴 값은 **DOUBLE** 타입이다. *x* 는 -1 이상 1 이하의 값이어야 하며, 그 외의 경우 에러를 반환한다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT ACOS(1), ACOS(0), ACOS(-1);

::
    
      acos(1)                   acos(0)                  acos(-1)
    ==================================================================================
      0.000000000000000e+00     1.570796326794897e+00     3.141592653589793e+00

ASIN
====

.. function:: ASIN ( x )

    **ASIN** 함수는 인자의 아크 사인(arc sine) 값을 반환한다. 즉, 사인이 *x* 인 값을 라디안 단위로 반환하며, 리턴 값은 DOUBLE 타입이다. *x* 는 -1 이상 1 이하의 값이어야 하며, 그 외의 경우 에러를 반환한다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT ASIN(1), ASIN(0), ASIN(-1);

::
    
      asin(1)                   asin(0)                  asin(-1)
    ==============================================================================
      1.570796326794897e+00     0.000000000000000e+00    -1.570796326794897e+00

ATAN
====

.. function:: ATAN ( [y,] x )

    **ATAN** 함수는 탄젠트가 *x* 인 값을 라디안 단위로 반환한다. 인자 *y* 는 생략될 수 있으며, *y* 가 지정되는 경우 함수는 *y* / *x* 의 아크 탄젠트 값을 계산한다. 리턴 값은 **DOUBLE** 타입이다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x,y: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT ATAN(1), ATAN(-1), ATAN(1,-1);

::
    
                       atan(1)                  atan(-1)              atan2(1, -1)
    ==============================================================================
         7.853981633974483e-01    -7.853981633974483e-01     2.356194490192345e+000

ATAN2
=====

.. function:: ATAN2 ( y, x )

    **ATAN2** 함수는 *y* / *x* 의 아크 탄젠트 값을 라디안 단위로 반환하며, :func:`ATAN` 와 유사하게 동작한다. 인자 *x*, *y* 가 모두 지정되어야 한다. 리턴 값은 **DOUBLE** 타입이다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x,y: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT ATAN2(1,1), ATAN2(-1,-1), ATAN2(Pi(),0);

::
    
    atan2(1, 1)             atan2(-1, -1)           atan2( pi(), 0)
    ==============================================================================
     7.853981633974483e-01    -2.356194490192345e+00     1.570796326794897e+00

CEIL
====

.. function:: CEIL( number_operand )

    **CEIL** 함수는 인자보다 크거나 같은 최소 정수 값을 인자의 타입으로 반환한다. 리턴 값은 *number_operand* 인자로 지정된 값의 유효 자릿수를 따른다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param number_operand: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: INT

.. code-block:: sql

    SELECT CEIL(34567.34567), CEIL(-34567.34567);

::
    
      ceil(34567.34567)     ceil(-34567.34567)
    ============================================
      34568.00000           -34567.00000
     
    SELECT CEIL(34567.1), CEIL(-34567.1);

::
    
      ceil(34567.1)         ceil(-34567.1)
    =============================
      34568.0         -34567.0

CONV
====

.. function:: CONV (number,from_base,to_base)

    **CONV** 함수는 숫자의 진수를 변환하는 함수이며, 진수가 변환된 숫자를 문자열로 반환한다. 진수의 최소값은 2, 최대값은 36이다. 반환할 숫자의 진수를 나타내는 *to_base* 가 음수이면 입력 숫자인 *number* 가 부호 있는(signed) 숫자로 간주되고, 그 외의 경우에는 부호 없는(unsigned) 숫자로 간주된다. *from_base* 또는 *to_base*\ 에 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param number: 입력 숫자
    :param from_base: 입력 숫자의 진수
    :param to_base: 반환할 숫자의 진수
    :rtype: STRING

.. code-block:: sql

    SELECT CONV('f',16,2);

::    

    '1111'

.. code-block:: sql

    SELECT CONV('6H',20,8);
    
::    

    '211'

.. code-block:: sql

    SELECT CONV(-30,10,-20);
    
::    

    '-1A'

COS
===

.. function:: COS ( x )

    **COS** 함수는 인자의 코사인(cosine) 값을 반환하며, 인자 *x* 는 라디안 값이어야 한다. 리턴 값은 **DOUBLE** 타입이다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT COS(pi()/6), COS(pi()/3), COS(pi());

::
    
      cos( pi()/6)              cos( pi()/3)                cos( pi())
    ==============================================================================
      8.660254037844387e-01     5.000000000000001e-01    -1.000000000000000e+00

COT
===

.. function:: COT ( x )

    **COT** 함수는 인자 *x* 의 코탄젠트(cotangent) 값을 반환한다. 즉, 탄젠트가 *x* 인 값을 라디안 단위로 반환하며, 리턴 값은 **DOUBLE** 타입이다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT COT(1), COT(-1), COT(0);

::
    
      cot(1)                   cot(-1)   cot(0)
    ==========================================================================
      6.420926159343306e-01    -6.420926159343306e-01  NULL

CRC32
=====

.. function:: CRC32 ( string )

    **CRC32** 함수는 32비트 정수로 순환 중복 검사 값을 반환한다. NULL을 입력하면 NULL을 반환한다.
    :param string: 문자열 값을 반환하는 표현식
    :rtype: INTEGER

.. code-block:: sql

    SELECT CRC32('cubrid');

::
    
       crc32('cubrid')
    ==================
             908740081


DEGREES
=======

.. function:: DEGREES ( x )

    **DEGREES** 함수는 라디안 단위로 지정된 인자 *x* 를 각도로 환산하여 반환한다. 리턴 값은 **DOUBLE** 타입이다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT DEGREES(pi()/6), DEGREES(pi()/3), DEGREES (pi());

::
    
      degrees( pi()/6)          degrees( pi()/3)            degrees( pi())
    ==============================================================================
      3.000000000000000e+01     5.999999999999999e+01     1.800000000000000e+02

DRANDOM, DRAND
==============

.. function:: DRANDOM ( [seed] )
.. function:: DRAND ( [seed] )

    **DRANDOM** / **DRAND** 함수는 구간 0.0 이상 1.0 미만의 구간에서 임의의 이중 정밀도(double-precision) 부동 소수점 값을 반환하며, *seed* 인자를 지정할 수 있다. *seed* 인자의 타입은 **INTEGER** 이며, 실수가 지정되면 반올림하고, **INTEGER** 범위를 초과하면 에러를 반환한다.

    *seed* 값이 주어지지 않은 경우 **DRAND()**\는 연산을 출력하는 행(row)의 개수와 관계없이 한 문장 내에서 1회만 연산을 수행하여 오직 한 개의 임의값만 생성하는 반면, **DRANDOM()**\는 함수가 호출될 때마다 매번 연산을 수행하므로 한 문장 내에서 여러 개의 다른 임의 값을 생성한다. 따라서, 무작위 순서로 행을 출력하기 위해서는 **ORDER BY** 절에 **DRANDOM()**\을 이용해야 한다. 무작위 정수값을 구하기 위해서는 :func:`RANDOM`\ 를 사용한다.

    :param seed: seed 값
    :rtype: DOUBLE

.. code-block:: sql

    SELECT DRAND(), DRAND(1), DRAND(1.4);

::
    
                       drand()                  drand(1)                drand(1.4)
    ==============================================================================
        2.849646518006921e-001    4.163034446537495e-002    4.163034446537495e-002
     
.. code-block:: sql

    CREATE TABLE rand_tbl (
        id INT,
        name VARCHAR(255)
    );
    
    INSERT INTO rand_tbl VALUES 
        (1, 'a'), (2, 'b'), (3, 'c'), (4, 'd'), (5, 'e'), 
        (6, 'f'), (7, 'g'), (8, 'h'), (9, 'i'), (10, 'j');

    SELECT * FROM rand_tbl;

::
    
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
     
.. code-block:: sql

    --drandom() returns random values on every row
    SELECT DRAND(), DRANDOM() FROM rand_tbl;
    
::
    
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
     
.. code-block:: sql

    --selecting rows in random order
    SELECT * FROM rand_tbl ORDER BY DRANDOM();
    
::
    
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

    **EXP** 함수는 자연로그의 밑수인 e를 *x* 제곱한 값을 **DOUBLE** 타입으로 반환한다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT EXP(1), EXP(0);

::
    
      exp(1)                    exp(0)
    ====================================================
      2.718281828459045e+000 1.000000000000000e+000
     
.. code-block:: sql

    SELECT EXP(-1), EXP(2.00);

::
    
      exp(-1)                 exp(2.00)
    ====================================================
      3.678794411714423e-001 7.389056098930650e+000

FLOOR
=====

.. function:: FLOOR( number_operand )

    **FLOOR** 함수는 인자보다 작거나 같은 최대 정수 값을 반환하며, 리턴 값의 타입은 인자의 타입과 같다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param number_operand: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: number_operand의 타입

.. code-block:: sql

    --it returns the largest integer less than or equal to the arguments
    SELECT FLOOR(34567.34567), FLOOR(-34567.34567);
    
::
    
      floor(34567.34567)    floor(-34567.34567)
    ============================================
      34567.00000           -34568.00000
     
.. code-block:: sql

    SELECT FLOOR(34567), FLOOR(-34567);
    
::
    
      floor(34567)   floor(-34567)
    =============================
             34567         -34567

HEX
===

.. function:: HEX(n)

    **HEX** 함수는 문자열을 인자로 지정하면 해당 문자열에 대한 16진수 문자열을 반환하고, 숫자를 인자로 지정하면 해당 숫자에 대한 16진수 문자열을 반환한다. 숫자를 인자로 지정하면 CONV(num, 10, 16)과 같은 값을 반환한다.

    :param n: 문자열 또는 숫자
    :rtype: STRING

.. code-block:: sql

    SELECT HEX('ab'), HEX(128), CONV(HEX(128), 16, 10);

::    

    hex('ab')             hex(128)              conv(hex(128), 16, 10)
    ==================================================================
      '6162'                '80'                  '128'

LN
==

.. function:: LN ( x )

    **LN** 함수는 진수 *x* 의 자연 로그(밑수가 e인 로그) 값을 반환하며, 리턴 값은 **DOUBLE** 타입이다. 진수 *x* 가 0이거나 음수인 경우, 에러를 반환한다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 양수 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT ln(1), ln(2.72);

::
    
         ln(1)                     ln(2.72)
    =====================================================
         0.000000000000000e+00     1.000631880307906e+00

LOG2
====

.. function:: LOG2 ( x )

    **LOG2** 함수는 진수가 *x* 이고, 밑수가 2인 로그 값을 반환하며, 리턴 값은 **DOUBLE** 타입이다. 진수 *x* 가 0이거나 음수인 경우, 에러를 반환한다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 양수 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT log2(1), log2(8);
    
::

         log2(1)                   log2(8)
    ======================================================
         0.000000000000000e+00     3.000000000000000e+00  

LOG10
=====

.. function:: LOG10 ( x )

    **LOG10** 함수는 진수 *x* 의 상용 로그 값을 반환하며, 리턴 값은 **DOUBLE** 타입이다. 진수 *x* 가 0이거나 음수인 경우, 에러를 반환한다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 양수 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT log10(1), log10(1000);
    
::

         log10(1)                  log10(1000)
    ====================================================
         0.000000000000000e+00     3.000000000000000e+00

MOD
===

.. function:: MOD (m, n)

    **MOD** 함수는 첫 번째 인자 *m* 을 두 번째 인자 *n* 으로 나눈 나머지 값을 정수로 반환하며, 만약 *n* 이 0이면, 나누기 연산을 수행하지 않고 *m* 값을 그대로 반환한다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.
        
    주의할 점은 피제수, 즉 **MOD** 함수의 인자 *m* 이 음수인 경우, 전형적인 연산(classical modulus) 방식과 다르게 동작한다는 점이다. 아래의 표를 참고한다.

    **MOD 함수의 결과**

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
    | 11    | 0     | 11            | 0으로 나누기 에러     |
    +-------+-------+---------------+-----------------------+

    :param m: 피제수를 나타내며, 수치 값을 반환하는 연산식이다.
    :param n: 제수를 나타내며, 수치 값을 반환하는 연산식이다.
    :rtype: INT

.. code-block:: sql

    --it returns the reminder of m divided by n
    SELECT MOD(11, 4), MOD(11, -4), MOD(-11, 4), MOD(-11, -4), MOD(11,0);
    
::

        mod(11, 4)   mod(11, -4)   mod(-11, 4)   mod(-11, -4)   mod(11, 0)
    =====================================================================
                3             3            -3             -3           11

.. code-block:: sql
     
    SELECT MOD(11.0, 4), MOD(11.000, 4), MOD(11, 4.0), MOD(11, 4.000);
    
::

      mod(11.0, 4)          mod(11.000, 4)        mod(11, 4.0)          mod(11, 4.000)
    =========================================================================
      3.0                   3.000                 3.0                   3.000

PI
==

.. function:: PI ()

    **PI** 함수는 π 값을 반환하며, 리턴 값은 DOUBLE 타입이다.

    :rtype: DOUBLE

.. code-block:: sql

    SELECT PI(), PI()/2;
    
::

         pi()                      pi()/2
    ====================================================
         3.141592653589793e+00     1.570796326794897e+00

POW, POWER
==========

.. function:: POW( x, y )
.. function:: POWER( x, y )

    **POW** 함수와 **POWER** 함수는 동일하며, 지정된 밑수 *x* 를 지수 *y* 만큼 거듭제곱한 값을 반환한다. 리턴 값은 **DOUBLE** 타입이다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 밑수를 나타내며, 수치 값을 반환하는 연산식이다.
    :param y: 지수를 나타내며, 수치 값을 반환하는 연산식이다. 밑수가 음수인 경우, 지수는 반드시 정수가 지정되어야 한다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT POWER(2, 5), POWER(-2, 5), POWER(0, 0), POWER(1,0);
    
::

     power(2, 5)              power(-2, 5)               power(0, 0)               power(1, 0)
    ====================================================================================================
     3.200000000000000e+01    -3.200000000000000e+01     1.000000000000000e+00     1.000000000000000e+00
     
.. code-block:: sql

    --it returns an error when the negative base is powered by a non-int exponent
    SELECT POWER(-2, -5.1), POWER(-2, -5.1);
    
::
     
    ERROR: Argument of power() is out of range.

RADIANS
=======

.. function:: RADIANS ( x )

    **RADIANS** 함수는 각도 단위로 지정된 인자 *x* 를 라디안 단위로 환산하여 리턴한다. 리턴 값은 **DOUBLE** 타입이다. 리턴 값은 **DOUBLE** 타입이다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT RADIANS(90), RADIANS(180), RADIANS(360);
    
::

         radians(90)               radians(180)              radians(360)
    ==============================================================================
         1.570796326794897e+00     3.141592653589793e+00     6.283185307179586e+00

RANDOM, RAND
============

.. function:: RANDOM ( [seed] )
.. function:: RAND ( [seed] )

    **RANDOM** / **RAND** 함수는 0 이상 2 31 미만 구간에서 임의의 정수 값을 반환하며, *seed* 인자를 지정할 수 있다. *seed* 인자의 타입은 **INTEGER** 이며, 실수가 지정되면 반올림하고 **INTEGER** 범위를 초과하면 에러를 반환한다.

    *seed* 값이 주어지지 않은 경우 **RAND()**\는 연산을 출력하는 행(row)의 개수와 관계없이 한 문장 내에서 1회만 연산을 수행하여 오직 한 개의 임의값만 생성하는 반면, **RANDOM()**\은 함수가 호출될 때마다 매번 연산을 수행하므로 한 문장 내에서 여러 개의 다른 임의 값을 생성한다. 따라서, 무작위 순서로 행을 출력하기 위해서는 **RANDOM()**\을 이용해야 한다.

    무작위 실수 값을 구하기 위해서는 :func:`DRANDOM` 를 사용한다.

    :param seed: 
    :rtype: INT

.. code-block:: sql

    SELECT RAND(), RAND(1), RAND(1.4);
    
::

           rand()      rand(1)    rand(1.4)
    =======================================
       1526981144     89400484     89400484
     
.. code-block:: sql

    --creating a new table
    SELECT * FROM rand_tbl;
    
::

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
     
.. code-block:: sql

    --random() returns random values on every row
    SELECT RAND(),RANDOM() FROM rand_tbl;
    
::

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
     
     
.. code-block:: sql

    --selecting rows in random order
    SELECT * FROM rand_tbl ORDER BY RANDOM();
    
::

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

.. function:: ROUND( number_operand, integer )

    **ROUND** 함수는 지정된 인자 *number_operand* 를 소수점 아래 *integer* 자리까지 반올림한 값을 반환한다. 반올림할 자릿수를 지정하는 *integer* 인자가 생략되거나 0인 경우에는 소수점 아래 첫째 자리에서 반올림한다. 그리고 *integer* 인자가 음수이면, 소수점 위 자리, 즉 정수부에서 반올림한다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param number_operand: 수치 값을 반환하는 임의의 연산식이다.
    :param integer: 반올림 처리할 위치를 지정한다. 양의 정수 *n* 이 지정되면 소수점 아래 *n* 자리까지 표현되고, 음의 정수 *n* 이 지정되면 소수점 위 *n* 자리에서 반올림한다.
    :rtype: number_operand의 타입

.. code-block:: sql

    --it rounds a number to one decimal point when the second argument is omitted
    SELECT ROUND(34567.34567), ROUND(-34567.34567);
    
::

      round(34567.34567, 0)   round(-34567.34567, 0)
    ============================================
      34567.00000           -34567.00000
     
.. code-block:: sql
     
    --it rounds a number to three decimal point
    SELECT ROUND(34567.34567, 3), ROUND(-34567.34567, 3)  FROM db_root;
    
::

     round(34567.34567, 3)   round(-34567.34567, 3)
    ============================================
      34567.34600           -34567.34600
     
.. code-block:: sql

    --it rounds a number three digit to the left of the decimal point
    SELECT ROUND(34567.34567, -3), ROUND(-34567.34567, -3);
    
::

     round(34567.34567, -3)   round(-34567.34567, -3)
    ============================================
      35000.00000           -35000.00000

SIGN
====

.. function:: SIGN (number_operand)

    **SIGN** 함수는 지정된 인자 값의 부호를 반환한다. 양수이면 1을, 음수이면 -1을, 0이면 0을 반환한다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param number_operand: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: INT

.. code-block:: sql

    --it returns the sign of the argument
    SELECT SIGN(12.3), SIGN(-12.3), SIGN(0);
    
::

        sign(12.3)   sign(-12.3)      sign(0)
    ========================================
                1            -1            0

SIN
===

.. function:: SIN ( x )

    **SIN** 함수는 인자의 사인(sine) 값을 반환하며, 인자 *x* 는 라디안 값이어야 한다. 리턴 값은 **DOUBLE** 타입이다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT SIN(pi()/6), SIN(pi()/3), SIN(pi());
    
::

         sin( pi()/6)              sin( pi()/3)              sin( pi())
    ==============================================================================
         4.999999999999999e-01     8.660254037844386e-01     1.224646799147353e-16

SQRT
====

.. function:: SQRT ( x )

    **SQRT** 함수는 *x* 의 제곱근(square root) 값을 **DOUBLE** 타입으로 반환한다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.
    
    :param x: 수치 값을 반환하는 임의의 연산식이다. 만약, 음수이면 에러를 반환한다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT SQRT(4), SQRT(16.0);
    
::

         sqrt(4)                   sqrt(16.0)
    ====================================================
         2.000000000000000e+00     4.000000000000000e+00

TAN
===

.. function:: TAN ( x )

    **TAN** 함수는 인자의 탄젠트(tangent) 값을 반환하며, 인자 *x* 는 라디안 값이어야 한다. 리턴 값은 **DOUBLE** 타입이다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 수치 값을 반환하는 임의의 연산식이다.
    :rtype: DOUBLE

.. code-block:: sql

    SELECT TAN(pi()/6), TAN(pi()/3), TAN(pi()/4);
    
::

         tan( pi()/6)              tan( pi()/3)              tan( pi()/4)
    ==============================================================================
         5.773502691896257e-01     1.732050807568877e+00     9.999999999999999e-01

TRUNC, TRUNCATE
===============

.. function:: TRUNC ( x[, dec] )
.. function:: TRUNCATE ( x, dec )

    **TRUNC** 함수와 **TRUNCATE** 함수는 지정된 인자 *x* 의 소수점 아래 숫자가 *dec* 자리까지 표현되도록 버림(truncation)한 값을 반환한다. 단, **TRUNC** 함수의 *dec* 인자는 생략할 수 있지만, **TRUNCATE** 함수의 *dec* 인자는 생략할 수 없다. 버림할 위치를 지정하는 *dec* 인자가 음수이면 정수부의 소수점 위 *dec* 번째 자리까지 0으로 표시한다. 리턴 값의 표현 자릿수는 인자 *x* 를 따른다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    :param x: 수치 값을 반환하는 임의의 연산식이다.
    :param dec: 버림할 위치를 지정한다. 양의 정수 *n* 이 지정되면 소수점 아래 *n* 자리까지 표현되고, 음의 정수 *n* 이 지정되면 소수점 위 *n* 자리까지 0으로 표시한다. *dec* 인자가 0이거나 생략되면 소수부를 버림한다. 단, **TRUNCATE** 함수에서는 *dec* 인자를 생략할 수 없다.
    :rtype: x의 타입

.. code-block:: sql

    --it returns a number truncated to 0 places
    SELECT TRUNC(34567.34567), TRUNCATE(34567.34567, 0);
    
::

      trunc(34567.34567, 0)   trunc(34567.34567, 0)
    ============================================
      34567.00000            34567.00000
     
.. code-block:: sql

    --it returns a number truncated to three decimal places
    SELECT TRUNC(34567.34567, 3), TRUNC(-34567.34567, 3);
    
::

      trunc(34567.34567, 3)   trunc(-34567.34567, 3)
    ============================================
      34567.34500           -34567.34500
     
.. code-block:: sql

    --it returns a number truncated to three digits left of the decimal point
    SELECT TRUNC(34567.34567, -3), TRUNC(-34567.34567, -3);
    
::

      trunc(34567.34567, -3)   trunc(-34567.34567, -3)
    ============================================
      34000.00000           -34000.00000

WIDTH_BUCKET
============

.. function:: WIDTH_BUCKET(expression, from, to, num_buckets)

    **WIDTH_BUCKET** 함수는 순차적인 데이터 집합을 균등한 범위로 부여된 일련의 버킷으로 나누며, 각 행에 적당한 버킷 번호를 1부터 할당한다. 즉, WIDTH_BUCKET 함수는 equi-width histogram을 생성한다. 반환되는 값은 정수이다. 숫자로 변환되지 않는 문자열을 입력할 때 **cubrid.conf**\ 의 **return_null_on_function_errors** 파라미터의 값이 no(기본값)면 에러, yes면 NULL을 반환한다.

    이 함수는 주어진 버킷 개수로 범위를 균등하게 나누어 버킷 번호를 부여한다. 즉, 버킷마다 각 범위의 넓이는 균등하다.

    참고로 :func:`NTILE` 분석 함수는 이에 비해 주어진 버킷 개수로 전체 행의 개수를 균등하게 나누어 버킷 번호를 부여한다. 즉, 버킷마다 각 행의 개수는 균등하다.

    :param expression: 버킷 번호를 부여받기 위한 입력 값. 수치 값을 반환하는 임의의 연산식을 지정한다.
    :param from: *expression*\ 이 취할 수 있는 범위의 시작값으로, 이 값은 전체 범위 안에 포함된다. 
    :param to: *expression*\ 이 취할 수 있는 범위의 마지막 값으로, 이 값은 전체 범위 안에 포함되지 않는다.
    :param num_buckets: 버킷의 개수. 추가로 범위 밖의 내용을 담기 위한 0번 버킷과 (*num_buckets* + 1)번 버킷이 생성된다.
    :rtype: INT

    *expression*\ 은 버킷 번호를 부여받기 위한 입력 데이터이다. *from*\ 과 *to* 값으로 숫자형 타입과 날짜/시간 타입의 값 또는 날짜/시간 타입으로 변환 가능한 문자열이 입력될 수 있다. 전체 범위에서 *from*\ 은 범위에 포함되지만 *to*\ 는 범위 밖에 존재한다. 

    예를 들어 WIDTH_BUCKET (score, 80, 50, 3)이 반환하는 값은 score가 
    
        * 80보다 크면 0, 
        * [80, 70)이면 1, 
        * [70, 60)이면 2, 
        * [60, 50)이면 3, 
        * 50보다 작거나 같으면 4가 된다.

다음 예제는 80점보다 작거나 같고 50점보다 큰 범위를 1부터 3까지 균등한 점수 범위로 나누어 등급을 부여한다. 해당 범위를 벗어나는 경우 80점보다 크면 0, 50점이거나 50점보다 작으면 4등급을 부여한다.

.. code-block:: sql

    CREATE TABLE t_score (name VARCHAR(10), score INT);
    INSERT INTO t_score VALUES
        ('Amie', 60),
        ('Jane', 80),
        ('Lora', 60),
        ('James', 75),
        ('Peter', 70),
        ('Tom', 50),
        ('Ralph', 99),
        ('David', 55);

    SELECT name, score, WIDTH_BUCKET (score, 80, 50, 3) grade 
    FROM t_score 
    ORDER BY grade ASC, score DESC;

::
    
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

다음의 예에서 **WIDTH_BUCKET** 함수는 birthdate의 지정 범위를 균등하게 나누고 이를 기준으로 버킷 번호를 부여한다. 8 명의 고객을 생년월일을 기준으로 '1950-01-01'부터 '1999-12-31'까지의 범위를 5개로 균등 분할하며, birthdate 값이 범위를 벗어나면 0 또는 버킷 개수 + 1인 6을 반환한다.

.. code-block:: sql

    CREATE TABLE t_customer (name VARCHAR(10), birthdate DATE);
    INSERT INTO t_customer VALUES
        ('Amie', date'1978-03-18'),
        ('Jane', date'1983-05-12'),
        ('Lora', date'1987-03-26'),
        ('James', date'1948-12-28'),
        ('Peter', date'1988-10-25'),
        ('Tom', date'1980-07-28'),
        ('Ralph', date'1995-03-17'),
        ('David', date'1986-07-28');
        
    SELECT name, birthdate, WIDTH_BUCKET (birthdate, date'1950-01-01', date'2000-1-1', 5) age_group 
    FROM t_customer 
    ORDER BY birthdate;

::

      name                  birthdate     age_group
    ===============================================
      'James'               12/28/1948            0
      'Amie'                03/18/1978            4
      'Tom'                 07/28/1980            4
      'Jane'                05/12/1983            5
      'David'               07/28/1986            5
      'Lora'                03/26/1987            5
      'Peter'               10/25/1988            5
      'Ralph'               03/17/1995            6
