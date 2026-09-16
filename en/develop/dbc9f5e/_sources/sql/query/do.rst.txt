
:meta-keywords: do statement
:meta-description: The DO statement executes the specified expression, but does not return the result.

**
DO
**

The **DO** statement executes the specified expression, but does not return the result. This can be used to determine whether or not the syntax of the expression is correct because an error is returned when a specified expression does not comply with the syntax. In general, the execution speed of the **DO** statement is higher than that of the **SELECT** statement because the database server does not return the operation result or errors. ::

    DO expression

*   *expression* : Specifies an expression.

.. code-block:: sql

    DO 1+1;
    DO SYSDATE + 1;
    DO (SELECT count(*) FROM athlete);
