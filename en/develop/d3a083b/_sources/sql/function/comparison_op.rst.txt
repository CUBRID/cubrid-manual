:tocdepth: 3

********************
Comparison Operators
********************

.. contents::

The comparison operators compare the operand on the left and on the right, and they return 1 or 0. The operands of comparison operations must have the same data type. Therefore, implicit type casting by the system or implicit type casting by the user is required.

**Syntax 1**

::

    <expression> <comparison_operator> <expression>
     
        <expression> ::=
            bit_string |
            character_string |
            numeric_value |
            date-time_value |
            collection_value |
            NULL
     
        <comparison_operator> ::=
            = |
            <=> |
            <> |
            != |
            > |
            < |
            >= |
            <=

**Syntax 2**

::

    <expression> IS [NOT] <boolean_value>
     
        <expression> ::=
            bit_string |
            character_string |
            numeric_value |
            date-time_value |
            collection_value |
            NULL
     
    <boolean_value> ::=
        { UNKNOWN | NULL } |
        TRUE |
        FALSE

*   <*expression*>: Declares an expression to be compared.

    *   *bit_string*: A Boolean operation can be performed on bit strings, and all comparison operators can be used for comparison between bit strings. If you compare two expressions with different lengths, 0s are padded at the end of the shorter one.

    *   *character_string*: When compared by a comparison operator, two character strings must have the same character sets. The comparison is determined by the collation sequence of the character code set. If you compare two character strings with different lengths, blanks are padded at the end of the shorter one before comparison so that they have the same length.

    *   *numeric_value*: The Boolean operator can be performed for all numeric values and any types of comparison operator can be used. When two different numeric types are compared, the system implicitly performs type casting. For example, when an **INTEGER** value is compared with a **DECIMAL** value, the system first casts **INTEGER** to **DECIMAL** before it performs comparison. When you compare a **FLOAT** value, you must specify the range instead of an exact value because the processing of **FLOAT** is dependent on the system.

    *   *date-time_value*: If two date-time values with the same type are compared, the order is determined in time order. That is, when comparing two date-time values, the earlier date is considered to be smaller than the later date. You cannot compare date-time values with different type by using a comparison operator; therefore, you must explicitly convert it. However, comparison operation can be performed between DATE, TIMESTAMP, and DATETIME because they are implicitly converted.

    *   *collection_value*: When comparing two LISTs (SEQUENCE), comparison is performed between the two elements by user-specified order when LIST was created. Comparison including SET and MULTISET is overloaded to an appropriate operator. You can perform comparison operations on SET, MULTISET, LIST, or SEQUENCE by using a containment operator explained later in this chapter. For details, see :doc:`containment_op`.

    *   **NULL**: The **NULL** value is not included in the value range of any data type. Therefore, comparison between **NULL** values is only allowed to determine if the given value is **NULL** or not. An implicit type cast does not take place when a **NULL** value is assigned to a different data type. For example, when an attribute of **INTEGER** type has a **NULL** and is compared with a floating point type, the **NULL** value is not coerced to **FLOAT** before comparison is made. A comparison operation on the **NULL** value does not return a result.

The following table shows the comparison operators supported by CUBRID and their return values.

**Comparison Operators**

+-------------------------+---------------------------------------------------------------------------------------------+----------------+----------------+
| Comparison Operator     | Description                                                                                 | Predicate      | Return Value   |
+=========================+=============================================================================================+================+================+
| **=**                   | A general equal sign. It compares whether the values of the left and right operands         | 1=2            | 0              |
|                         | are the same. Returns **NULL**  if one or more operands are NULL.                           | 1=NULL         | NULL           |
+-------------------------+---------------------------------------------------------------------------------------------+----------------+----------------+
| **<=>**                 | A NULL-safe equal sign. It compares whether the values of the left and right operands       | 1<=>2          | 0              |
|                         | are the same including **NULL**. Returns 1 if both operands are **NULL**.                   | 1<=> NULL      | 0              |
+-------------------------+---------------------------------------------------------------------------------------------+----------------+----------------+
| **<>, !=**              | The value of left operand is not equal to that of right operand.                            | 1<>2           | 1              |
|                         | If any operand value is **NULL**, **NULL** is returned.                                     |                |                |
+-------------------------+---------------------------------------------------------------------------------------------+----------------+----------------+
| **>**                   | The value of left operand is greater than that of right operand.                            | 1>2            | 0              |
|                         | If any operand value is **NULL**, **NULL** is returned.                                     |                |                |
+-------------------------+---------------------------------------------------------------------------------------------+----------------+----------------+
| **<**                   | The value of left operand is less than that of right operand.                               | 1<2            | 1              |
|                         | If any operand value is **NULL**, **NULL** is returned.                                     |                |                |
+-------------------------+---------------------------------------------------------------------------------------------+----------------+----------------+
| **>=**                  | The value of left operand is greater than or equal to that of right operand.                | 1>=2           | 0              |
|                         | If any operand value is **NULL**, **NULL** is returned.                                     |                |                |
+-------------------------+---------------------------------------------------------------------------------------------+----------------+----------------+
| **<=**                  | The value of left operand is less than or equal to that of right operand.                   | 1<=2           | 1              |
|                         | If any operand value is  **NULL**, **NULL** is returned.                                    |                |                |
+-------------------------+---------------------------------------------------------------------------------------------+----------------+----------------+
| **IS**                  | Compares whether the value of the left operand is the same as boolean value of the right.   | 1 IS FALSE     | 0              |
| *boolean_value*         | The boolean value may be **TRUE**, **FALSE** (or **NULL**).                                 |                |                |
+-------------------------+---------------------------------------------------------------------------------------------+----------------+----------------+
| **IS NOT**              | Compares whether the value of the left operand is the same as boolean value of the right.   | 1 IS NOT FALSE | 1              |
| *boolean_value*         | The boolean value may be **TRUE**, **FALSE** (or **NULL**).                                 |                |                |
+-------------------------+---------------------------------------------------------------------------------------------+----------------+----------------+

The following are the examples which use comparison operators.

.. code-block:: sql

    SELECT (1 <> 0); -- 1 is displayed because it is TRUE.
    SELECT (1 != 0); -- 1 is displayed because it is TRUE.  
    SELECT (0.01 = '0.01'); -- An error occurs because a numeric data type is compared with a character string type.
    SELECT (1 = NULL); -- NULL is displayed.
    SELECT (1 <=> NULL); -- 0 is displayed because it is FALSE. 
    SELECT (1.000 = 1); -- 1 is displayed because it is TRUE.
    SELECT ('cubrid' = 'CUBRID'); -- 0 is displayed because it is case sensitive.
    SELECT ('cubrid' = 'cubrid'); -- 1 is displayed because it is TRUE.
    SELECT (SYSTIMESTAMP = CAST(SYSDATETIME AS TIMESTAMP)); -- 1 is displayed after casting the type explicitly and then performing comparison operator. 
    SELECT (SYSTIMESTAMP = SYSDATETIME); --0 is displayed after casting the type implicitly and then performing comparison operator. 
    SELECT (SYSTIMESTAMP <> NULL); -- NULL is returned without performing comparison operator.
    SELECT (SYSTIMESTAMP IS NOT NULL); -- 1 is returned because it is not NULL.
    