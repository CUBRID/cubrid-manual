-----------------------------
Transaction Commit and Rollback Support
-----------------------------

In stored procedures and stored functions, transactions can be completed or aborted by **COMMIT** or **ROLLBACK**. This is an important feature that maintains data integrity and provides flexibility in transaction processing.
The logic within stored procedures and functions can control transactions based on conditions or exception handling, which greatly aids in managing complex data logic safely and efficiently.

Advantages of Condition and Exception Handling with Transaction Control
---------------------------------------------

1. **Condition-Based Transaction Processing**  
   Within stored procedures and functions, transactions can be committed or rolled back based on conditions. For example, if certain conditions are not met, a rollback can ensure data integrity.

2. **Safe Rollback on Exception Occurrence**  
   If an error (exception) occurs within a stored procedure, the transaction can be rolled back, returning the operation to a safe state. This is useful for preventing data corruption in unexpected situations.

3. **Stability in Handling Complex Logic**  
   Even while processing complex logic involving multiple operations, the success of a transaction can be clearly controlled based on the situation. This enhances the reliability of data processing.


Example: Condition and Exception Handling with Transaction Control
------------------------------------------

The following is an example of controlling transactions using conditions and exception handling in a stored procedure.

.. code-block:: sql

        ;server-output on

        CREATE TABLE orders (
          id INT NOT NULL PRIMARY KEY,
          product VARCHAR(100) NOT NULL,
          quantity INT NOT NULL
        );

        CREATE TABLE inventory (
          product VARCHAR(100) NOT NULL PRIMARY KEY,
          stock INT NOT NULL
        );

        CREATE OR REPLACE PROCEDURE process_transaction() 
        AS
        current_stock INT;
        BEGIN
                INSERT INTO orders (id, product, quantity) VALUES (1, 'Laptop', 5);

                UPDATE inventory SET stock = stock - 5 WHERE product = 'Laptop';

                SELECT stock INTO current_stock FROM inventory WHERE product = 'Laptop';

                IF current_stock < 0 THEN
                  ROLLBACK;
                  DBMS_OUTPUT.PUT_LINE('Transaction failed: Insufficient stock.');
                ELSE
                  COMMIT;
                  DBMS_OUTPUT.PUT_LINE('Transaction successful.');
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                  ROLLBACK;
                  DBMS_OUTPUT.PUT_LINE('Transaction failed: An error occurred.');
        END;

::

        process_transaction ();

        === <Result of CALL Command in Line 2> ===

        Result              
        ======================
        NULL                

        <DBMS_OUTPUT>
        ====
        Transaction successful.

This example covers the following scenarios:
- Rollback the transaction to maintain data integrity if there is insufficient stock.
- Safely rollback the transaction if an exception occurs during the operation.
- Commit the transaction to complete the operation if conditions are met.

Cautions
----------

* In PL/CSQL, **COMMIT** and **ROLLBACK** statements are supported by default, allowing explicit control of transaction processing within stored procedures.
* However, in case of Java Stored Procedures, the **pl_transaction_control** setting is used to maintain backward compatibility, and COMMIT and ROLLBACK statements can only be used when the parameter is set to **yes**.
* The default value of **pl_transaction_control** is **no**, and COMMIT and ROLLBACK are ignored.
