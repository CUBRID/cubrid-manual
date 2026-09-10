
:meta-keywords: cubrid transaction, database transaction, cubrid locking, database locking, cubrid concurrency, multiversion concurrency control, mvcc, isolation level, database recovery
:meta-description: This chapter covers issues relating to concurrency (MVCC) and restore, as well as how to commit or rollback transactions in CUBRID database.

********************
Transaction and Lock
********************

This chapter covers issues relating to concurrency and restore, as well as how to commit or rollback transactions.

In multi-user environment, controlling access and update is essential to protect database integrity and ensure that a user's transaction will have accurate and consistent data. Without appropriate control, data could be updated incorrectly in the wrong order.

To control parallel operations on the same data, data must be locked during transaction, and unacceptable access to the data by another transaction must be blocked until the end of the transaction. In addition, any updates to a certain class must not be seen by other users before they are committed. If updates are not committed, all queries entered after the last commit or rollback of the update can be invalidated.

CUBRID uses Multiversion Concurrency Control to optimize data access. Data being modified is not overwritten, old data is instead marked as deleted and a newer version is added elsewhere. Other transactions can continue to read the old version without locking it. Each transaction sees its own snapshot of the database that is defined at the start of transaction or statement execution. Furthermore, the snapshot is only affected by own changes and remains consistent throughout the execution.

All examples introduced here were executed by csql.

.. toctree::
    :maxdepth: 2

    transaction.rst
