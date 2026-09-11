
:meta-keywords: kill statement
:meta-description: The KILL statement terminates transactions with an option TRANSACTION or QUERY modifier.

****
KILL
****

The **KILL** statement terminates transactions with an option **TRANSACTION** or **QUERY** modifier. 

::

    KILL [TRANSACTION | QUERY] tran_index, ... ;

\

* **KILL TRANSACTION** is default of **KILL** statement. It is the same as **KILL** without a modifier. It terminates the connection associated with the given *tran_index*.
* **KILL QUERY** terminates the statement that the transaction is executing.
    
DBA and a user of DBA group are able to terminate all the transactions of the system, while non-DBA users are only allowed to terminate their own transaction.

::

    KILL TRANSACTION 1;
    