******************
비트 함수와 연산자
******************


**BIT Functions and Operators**

**Bitwise Operator**

A
**Bitwise**
operator performs operations in bits, and can be used in arithmetic operations. An integer type is specified as the operand and the
**BIT**
type cannot be specified. An integer of
**BIGINT**
type (64-bit integer) is returned as a result of the operation. If one or more operands are
**NULL**
,
**NULL**
is returned.

The following table shows the bitwise operators supported by CUBRID.

**The bitwise operators supported by CUBRID**

+----------------------+------------------------------------------------------------------------------------------------------------------------------------------------+----------------+------------------+
| **Bitwise operator** | **Description**                                                                                                                                | **Expression** | **Return Value** |
|                      |                                                                                                                                                |                |                  |
+----------------------+------------------------------------------------------------------------------------------------------------------------------------------------+----------------+------------------+
| &                    | Performs AND operation in bits and returns a BIGINT integer.                                                                                   | 17 & 3         | 1                |
|                      |                                                                                                                                                |                |                  |
+----------------------+------------------------------------------------------------------------------------------------------------------------------------------------+----------------+------------------+
| |                    | Performs OR operation in bits and returns a BIGINT integer.                                                                                    | 17 | 3         | 19               |
|                      |                                                                                                                                                |                |                  |
+----------------------+------------------------------------------------------------------------------------------------------------------------------------------------+----------------+------------------+
| ^                    | Performs XOR operation in bits and returns a BIGINT integer.                                                                                   | 17 ^ 3         | 18               |
|                      |                                                                                                                                                |                |                  |
+----------------------+------------------------------------------------------------------------------------------------------------------------------------------------+----------------+------------------+
| ~                    | A unary operator. It performs complementary operation that reverses (INVERT) the bit order of the operand and returns a BIGINT integer.        | ~17            | -18              |
|                      |                                                                                                                                                |                |                  |
+----------------------+------------------------------------------------------------------------------------------------------------------------------------------------+----------------+------------------+
| <<                   | Performs the operation of moving bits of the left operand as far to the left as the value of the right operand, and returns a BIGINT integer.  | 17 << 3        | 136              |
|                      |                                                                                                                                                |                |                  |
+----------------------+------------------------------------------------------------------------------------------------------------------------------------------------+----------------+------------------+
| >>                   | Performs the operation of moving bits of the left operand as far to the right as the value of the right operand, and returns a BIGINT integer. | 17 >> 3        | 2                |
|                      |                                                                                                                                                |                |                  |
+----------------------+------------------------------------------------------------------------------------------------------------------------------------------------+----------------+------------------+

**BIT_AND Function**

**Description**

As an aggregate function, it performs
**AND**
operations in bits on every bit of
*expr*
. The return value is a
**BIGINT**
type. If there is no row that satisfies the expression,
**NULL**
is returned. 

**Syntax**

**BIT_AND**
(
*expr*
)

*   *expr*
    : An expression of integer type



**Example**

CREATE TABLE bit_tbl(id int);

INSERT INTO bit_tbl VALUES (1), (2), (3), (4), (5);

SELECT 1&3&5, BIT_AND(id) FROM bit_tbl WHERE id in(1,3,5);

                 1&3&5           bit_and(id)

============================================

                     1                     1

**BIT_OR Function**

**Description**

As an aggregate function, it performs
**OR**
operations in bits on every bit of
*expr*
. The return value is a
**BIGINT**
type. If there is no row that satisfies the expression,
**NULL**
is returned. 

**Syntax**

**BIT_OR**
(
*expr*
)

*   *expr*
    : An expression of integer type



**Example**

SELECT 1|3|5, BIT_OR(id) FROM bit_tbl WHERE id in(1,3,5);

                 1|3|5            bit_or(id)

============================================

                     7                     7

**BIT_XOR Function**

**Description**

As an aggregate function, it performs
**XOR**
operations in bits on every bit of
*expr*
. The return value is a
**BIGINT**
type. If there is no row that satisfies the expression,
**NULL**
is returned.

**Syntax**

**BIT_XOR**
(
*expr*
)

*   *expr*
    : An expression of integer type



**Example**

SELECT 1^2^3, BIT_XOR(id) FROM bit_tbl WHERE id in(1,3,5);

                 1^3^5           bit_xor(id)

============================================

                     7                     7

**BIT_COUNT Function**

**Description**

The
**BIT_COUNT**
function returns the number of bits of
*expr*
that have been set to 1; it is not an aggregate function. The return value is a
**BIGINT**
type.

**Syntax**

**BIT_COUNT**
(
*expr*
)

*   *expr*
    : An expression of integer type



**Example**

SELECT BIT_COUNT(id) FROM bit_tbl WHERE id in(1,3,5);

   bit_count(id)

================

               1

               2

               2
