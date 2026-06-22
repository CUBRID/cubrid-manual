*****************
Logical Operators
*****************

For logical operators, boolean expressions are specified as operands; **TRUE**, **FALSE** or **NULL** is returned as a result. If a boolean value is used in an arithmetic expression, **TRUE** is evaluated to 1 and **FALSE** is evaluated to 0.

.. note::

    As of version 11.2, using a non-logical value (such as an integer or column) as an operand raises the error "operand must be logical expression." Such a condition must be rewritten with an explicit comparison: a constant condition like ``WHERE 1`` as ``WHERE 1=1``, and a column condition like ``WHERE column_name`` as ``WHERE column_name <> 0``.

The following table shows the logical operators.

**Logical Operators**

+------------------+-----------------------------------------------------------------------------------+---------------+
| Logical Operator | Description                                                                       | Condition     |
+==================+===================================================================================+===============+
| **AND**, **&&**  | If all operands are **TRUE**, it returns **TRUE**.                                | a **AND** b   |
+------------------+-----------------------------------------------------------------------------------+---------------+
| **OR, ||**       | If none of operands is **NULL** and one or more operands are **TRUE**,            | a **OR** b    |
|                  | it returns **TRUE**. If **pipes_as_concat** is no that is a parameter related to  |               |
|                  | SQL statements, a double pipe symbol can be used as **OR** operator.              |               |
+------------------+-----------------------------------------------------------------------------------+---------------+
| **XOR**          | If none of operand is **NULL** and each of operand has a different value,         | a **XOR** b   |
|                  | it returns **TRUE**.                                                              |               |
+------------------+-----------------------------------------------------------------------------------+---------------+
| **NOT**, **!**   | A unary operator. If a operand is **FALSE** , it returns **TRUE**.                | **NOT** a     |
|                  | If it is **TRUE** , returns **FALSE**.                                            |               |
+------------------+-----------------------------------------------------------------------------------+---------------+

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
