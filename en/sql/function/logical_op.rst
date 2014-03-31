*****************
Logical Operators
*****************

For logical operators, boolean expressions or expressions that evaluates to an **INTEGER** value are specified as operands; **TRUE**, **FALSE** or **NULL** is returned as a result. If the **INTEGER** value is used, 0 is evaluated to **FALSE** and the other values are evaluated to **TRUE**. If a boolean value is used, 1 is evaluated to **TRUE** and 0 is evaluated to **FALSE**.

The following table shows the logical operators.

**Logical Operators**

+----------------------+------------------------------------------------------------------------------------------+---------------+
| Logical Operator     | Description                                                                              | Condition     |
+======================+==========================================================================================+===============+
| **AND**              | If all operands are                                                                      | a             |
| ,                    | **TRUE**                                                                                 | **AND**       |
| **&&**               | , it returns                                                                             | b             |
|                      | **TRUE**                                                                                 |               |
|                      | .                                                                                        |               |
+----------------------+------------------------------------------------------------------------------------------+---------------+
| **OR, ||**           | If none of operands is                                                                   | a             |
|                      | **NULL**                                                                                 | **OR**        |
|                      | and one or more operands are                                                             | b             |
|                      | **TRUE**                                                                                 |               |
|                      | , it returns                                                                             |               |
|                      | **TRUE**                                                                                 |               |
|                      | . If                                                                                     |               |
|                      | **pipes_as_concat**                                                                      |               |
|                      | is no that is a parameter related to SQL statements, a double pipe symbol can be used as |               |
|                      | **OR**                                                                                   |               |
|                      | operator.                                                                                |               |
+----------------------+------------------------------------------------------------------------------------------+---------------+
| **XOR**              | If none of operand is                                                                    | a             |
|                      | **NULL**                                                                                 | **XOR**       |
|                      | and each of operand has a different value, it returns                                    | b             |
|                      | **TRUE**                                                                                 |               |
|                      | .                                                                                        |               |
+----------------------+------------------------------------------------------------------------------------------+---------------+
| **NOT**              | A unary operator. If a operand is                                                        | **NOT**       |
| ,                    | **FALSE**                                                                                | a             |
| **!**                | , it returns                                                                             |               |
|                      | **TRUE**                                                                                 |               |
|                      | . If it is                                                                               |               |
|                      | **TRUE**                                                                                 |               |
|                      | , returns                                                                                |               |
|                      | **FALSE**                                                                                |               |
|                      | .                                                                                        |               |
+----------------------+------------------------------------------------------------------------------------------+---------------+

**Truth Table of Logical Operators**

+-----------+-----------+-------------+------------+-----------+-------------+
| a         | b         | a AND b     | a OR b     | NOT a     | a XOR b     |
+===========+===========+=============+============+===========+=============+
| **TRUE**  | **TRUE**  | TRUE        | TRUE       | FALSE     | FALSE       |
+-----------+-----------+-------------+------------+-----------+-------------+
| **TRUE**  | **FALSE** | FALSE       | TRUE       | FALSE     | TRUE        |
+-----------+-----------+-------------+------------+-----------+-------------+
| **TRUE**  | **NULL**  | NULL        | TRUE       | FALSE     | NULL        |
+-----------+-----------+-------------+------------+-----------+-------------+
| **FALSE** | **TRUE**  | FALSE       | TRUE       | TRUE      | TRUE        |
+-----------+-----------+-------------+------------+-----------+-------------+
| **FALSE** | **FALSE** | FALSE       | FALSE      | TRUE      | FALSE       |
+-----------+-----------+-------------+------------+-----------+-------------+
| **FALSE** | **NULL**  | FALSE       | NULL       | TRUE      | NULL        |
+-----------+-----------+-------------+------------+-----------+-------------+
