Transaction-Related Errors
==================


.. _ERROR-72:

**ERROR CODE: -72, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) has been unilaterally aborted by the system.'**

- This message is an error indicating that, in the CUBRID system, a specific transaction has been unilaterally aborted by the system; here, “unilaterally aborted” means that the system automatically rolled back the transaction regardless of the intent of the user or application that was executing it; this mainly occurs due to deadlock resolution, insufficient system resources, server failures, or other system-level problems.


.. _ERROR-73:

**ERROR CODE: -73, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on object %6$d|%7$d|%8$d. You are waiting for user(s) %9$s to finish.'**

- This message is a lock timeout (lock timeout) error indicating that, in the CUBRID system, a specific transaction timed out while waiting to acquire a lock on a specific object (record); the current transaction is waiting for another user to release the lock on that object (record), but the lock was not released within the configured time and a timeout occurred; this mainly occurs due to record-level concurrency-control problems, deadlock situations, or long-running transactions.


.. _ERROR-74:

**ERROR CODE: -74, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on class %6$s. You are waiting for user(s) %7$s to finish.'**

- This message is a lock timeout (lock timeout) error indicating that, in the CUBRID system, a specific transaction timed out while waiting to acquire a lock on a table; the current transaction is waiting for another user to release the lock on that table, but the lock was not released within the configured time and a timeout occurred; this mainly occurs due to table-level concurrency-control problems, deadlock situations, or long-running transactions.


.. _ERROR-75:

**ERROR CODE: -75, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on instance %6$d|%7$d|%8$d of class %9$s. You are waiting for user(s) %10$s to finish.'**

- This message is a lock timeout (lock timeout) error indicating that, in the CUBRID system, a specific transaction timed out while waiting for a lock on an instance of a table; this mainly occurs due to record-level concurrency-control problems, deadlock situations, or long-running transactions.


.. _ERROR-76:

**ERROR CODE: -76, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s on page %6$d|%7$d. You are waiting for user(s) %8$s to release the page lock.'**

- This message is a latch timeout (latch timeout) error indicating that, in the CUBRID system, a specific transaction timed out while waiting for a page lock in order to perform work on a page; this mainly occurs due to concurrency-control problems, deadlock situations, or long-running transactions.


.. _ERROR-106:

**ERROR CODE: -106, 'Cannot prepare to commit the current transaction with global transaction identifier %1$d because that transaction identifier is in use by another transaction.'**

- This message is displayed when the CUBRID system tries to prepare-commit the current transaction in a 2PC (Two-Phase Commit) distributed transaction, but cannot proceed with the prepare operation because the specified global transaction identifier is already in use by another active transaction.


.. _ERROR-107:

**ERROR CODE: -107, 'There is no global distributed transaction associated with transaction identifier %1$d.'**

- This message is displayed when the CUBRID system cannot find a global distributed transaction associated with a specific transaction identifier; that is, in a 2PC (Two-Phase Commit) distributed-transaction environment, the client attempted an operation on a distributed transaction using a specific transaction ID, but the global distributed transaction for that ID does not exist or has already completed.


.. _ERROR-108:

**ERROR CODE: -108, 'Current transaction %1$d must be committed or aborted before attempting to join to the distributed transaction with global identifier %2$d.'**

- This message is displayed when the CUBRID system tries to participate in a 2PC (Two-Phase Commit) distributed transaction but cannot participate because there is already an active transaction; it means that the current transaction must be completed (committed or aborted) before participating in the distributed transaction.


.. _ERROR-198:

**ERROR CODE: -198, 'Receive buffer too small, data truncated.'**

- This message indicates that, while receiving data in the CUBRID database, the allocated buffer size was smaller than the size of the data to be received, so part of the data was truncated; it mainly occurs when processing large data in network communication or file I/O, and it is an error that can affect data integrity.


.. _ERROR-440:

**ERROR CODE: -440, 'Invalid cursor position.'**

- This message is an error that appears when the cursor position is invalid while processing query results in the CUBRID database; it occurs during cursor-position validation in the query processor when the cursor is at an invalid position or the cursor state is not correct.


.. _ERROR-441:

**ERROR CODE: -441, 'Invalid cursor operation.'**

- This message is displayed when trying to extract a record’s OID from a result (cursor) that does not include OIDs in the CUBRID database; it can occur during cursor-operation validation in the query processor when an operation that the cursor does not support is attempted, or when the cursor state does not allow that operation.


.. _ERROR-442:

**ERROR CODE: -442, 'Unknown cursor position.'**

- This message is an error that appears when the cursor position is in an unexpected or undefined state in the CUBRID database; it can occur during cursor-position validation in the query processor when the cursor position has an unexpected value.


.. _ERROR-443:

**ERROR CODE: -443, 'Tuple value index %1$d is out of range.'**

- This message is an error that appears when, in a tuple of query results in the CUBRID database, an attempt is made to fetch a value by a specific index, but that index is out of the valid range; it occurs during tuple-value index validation in the query processor when an index larger than the number of tuple columns or a negative index is used.


.. _ERROR-444:

**ERROR CODE: -444, 'Invalid attribute name: "%1$s".'**

- This message is an error that appears when a column name or attribute name in query results is invalid in the CUBRID database; it occurs during column-name validation in the query processor when a non-existent column name or attribute name is referenced.


.. _ERROR-447:

**ERROR CODE: -447, 'Attempted to operate a query result structure already closed.'**

- This message is an error that appears when trying to perform an operation on a query result structure that has already been closed in the CUBRID database; it occurs during validation of the query result structure’s state in the query processor; it can happen when an application has explicitly closed the query result, or when it tries to access a query result that has been closed internally due to transaction end, and so on.


.. _ERROR-449:

**ERROR CODE: -449, 'Unknown query identifier: %1$d.'**

- This message is an error that appears when a non-existent query identifier is referenced in the CUBRID database; it occurs during query-identifier validation in the query layer; that is, when an undefined query ID or an invalid query identifier is used.


.. _ERROR-452:

**ERROR CODE: -452, 'Invalid XASL tree node content.'**

- This message is an error that appears when the content of an XASL (eXtensible Abstract Syntax Language) tree node is invalid in the CUBRID database; it occurs during validation of XASL tree nodes in a query; that is, when the structure or contents of an XASL tree node are not correct.


.. _ERROR-460:

**ERROR CODE: -460, 'Too few input host variables provided.'**

- This message is an error that appears when fewer host variables are provided than the number of host variables required to execute a query in the CUBRID database.


.. _ERROR-550:

**ERROR CODE: -550, 'Unknown savepoint name %1$s'**

- This message is an error that appears when referencing a savepoint name that does not exist in the CUBRID database; it mainly occurs when performing a partial rollback of a transaction and the specified savepoint does not exist.


.. _ERROR-609:

**ERROR CODE: -609, 'Your transaction cannot be rolled back since logging was ignored. Your database may be corrupted.'**

- This message occurs when attempting a transaction rollback in the CUBRID database; when logging is ignored, it is not possible to attempt a rollback or perform transaction recovery, and there is a possibility that the database may be damaged.


.. _ERROR-640:

**ERROR CODE: -640, 'System Error: a savepoint cannot be added when the transaction is not active (e.g., during commit or rollback).'**

- This message is a system error that occurs when trying to add a savepoint when no transaction is active in the CUBRID database; a savepoint cannot be created when a transaction is committing or rolling back, has not yet started, or has already ended; it indicates an internal system error related to transaction lifecycle management.


.. _ERROR-641:

**ERROR CODE: -641, 'A name must be given for a savepoint.'**

- This message is displayed when creating a savepoint in the CUBRID database but no name is provided; a savepoint is a feature that marks a specific point within a transaction, and it must require a unique name; this is a validation error that occurs when the required parameter (the name) is missing when creating a savepoint.


.. _ERROR-642:

**ERROR CODE: -642, ' System Error: there is not an active top system operation for current transaction.'**

- This message is a system error that occurs when there is no active top-level system operation for the current transaction in the CUBRID database; it occurs when trying to end a system operation but that operation is not active; it indicates an internal system error related to system-operation state management.


.. _ERROR-643:

**ERROR CODE: -643, 'May be a system error: Committing/Aborting transaction = %1$d (index = %2$d) which has permanent operations\n attached to it. Will attach those system operations to the transaction'**

- This message is a warning that occurs when there is a permanent system operation linked while trying to commit or abort a transaction in the CUBRID database; it occurs when attempting to commit or abort while system operations are linked to the transaction; it may indicate a system error, but CUBRID automatically attaches and processes those system operations with the transaction.


.. _ERROR-674:

**ERROR CODE: -674, 'There is not enough active threads to continue current server execution. Current number of active threads is %1$d. You may need up to %2$d threads for clean server execution in your environment. Transaction index %3$d has been timed out to continue server execution.'**

- This message is a warning from the CUBRID database server meaning that all worker threads are tied up in a “waiting (suspend)” state and, as a result, there appear to be not enough active threads available to process work immediately.


.. _ERROR-735:

**ERROR CODE: -735, 'Unknown transaction index %1$d.'**

- This message is an error that appears when an invalid transaction index is used in the CUBRID database; the transaction index is an internal identifier used by CUBRID to uniquely identify each transaction; this message occurs when the system attempts to reference a non-existent or incorrect transaction index; that is, it can occur due to an internal state inconsistency in the transaction-management system or due to memory corruption.


.. _ERROR-830:

**ERROR CODE: -830, 'Cannot allocate query entry any more. Maximum allocatable entries are %1$d.'**

- This message occurs when, in the query management module of the CUBRID database, allocating a query entry to execute a new query fails; that is, it is an error message that occurs when a new query entry cannot be allocated because the maximum number of query entries per transaction has been exceeded.


.. _ERROR-836:

**ERROR CODE: -836, 'LATCH ON PAGE(%1$d|%2$d) TIMEDOUT'**

- This message appears when an attempt to acquire a latch (synchronization lock) on a specific page in the CUBRID database does not complete within the specified time and times out; a latch (LATCH) is a lightweight lock mechanism used internally to control concurrent access to pages, and it can occur when contention is heavy because many transactions access the same page at the same time, or when acquisition is delayed due to long holding; it mainly appears when latch contention becomes severe due to a large number of concurrent transactions, inefficient queries, system load, internal deadlocks, bugs, and so on; the impact includes cases where an I/O bottleneck prevents the desired page from being fetched from the file, or where a dead-latch occurs while performing a B-tree split or merge.


.. _ERROR-842:

**ERROR CODE: -842, 'Cannot cache lock of object %1$d|%2$d|%3$d.'**

- This message is an error message that occurs when, while checking locks (LOCK) cached in the client module, the lock is NULL LOCK or there is a lock change error and, during the process of acquiring locks for multiple objects (records), acquiring the lock for a specific object (record) fails or a lock-cache operation fails.


.. _ERROR-843:

**ERROR CODE: -843, 'Wrong arguments in %1$s, a %2$s was passed.'**

- This message occurs when an invalid argument is passed to lock manager functions in the CUBRID database; it mainly occurs when, while calling lock manager functions internally from application code, a parameter is NULL.


.. _ERROR-844:

**ERROR CODE: -844, 'Unknown isolation(%1$d) is found. The transaction index is %2$d.'**

- This message occurs when, in the lock manager of the CUBRID database, the system finds an invalid isolation level value that it cannot recognize while checking a transaction’s isolation level; CUBRID supports defined isolation levels such as `TRAN_READ_COMMITTED`, `TRAN_REPEATABLE_READ`, and `TRAN_SERIALIZABLE`; if a value outside this range is found, this error occurs; it is an error that generally occurs due to memory corruption, concurrency-control problems, or an internal state inconsistency in the transaction manager.


.. _ERROR-845:

**ERROR CODE: -845, 'Invalid object type(%1$d) is found. The lockable object is %2$d|%3$d|%4$d.'**

- This message occurs when, in the lock manager of the CUBRID database, deleting a lock entry from a transaction hold list; if a value outside the range supported by CUBRID is found, this error occurs; it is an error that generally occurs due to memory corruption, concurrency-control problems, or an internal state inconsistency in the lock manager.


.. _ERROR-846:

**ERROR CODE: -846, '%1$s of tran(%2$d) is not found in the holder list of an object(%3$d|%4$d|%5$d).'**

- This message occurs during table-lock demotion in the lock manager of the CUBRID database; that is, when trying to find a transaction’s lock entry in the holder list for a specific object (record), it occurs when an abnormal state is found where the entry does not exist in the holder list; this typically appears when there is a problem in CUBRID’s internal lock-management logic, or when memory corruption or concurrency-control problems make the state information of lock entries inconsistent; if this error repeatedly occurs at the same time, it can affect database service stability.


.. _ERROR-847:

**ERROR CODE: -847, '%1$s on a %2$s(%3$d|%4$d|%5$d) is not found in the transaction(%6$d) hold list(cnt=%7$d).'**

- This message occurs during hold-list management in the lock manager of the CUBRID database; that is, when trying to delete a lock entry from a specific transaction’s hold list, it occurs when an abnormal state is found where the entry does not exist in the list; this typically appears when there is a problem in CUBRID’s internal lock-management logic, or when memory corruption or concurrency-control problems make the state information of lock entries inconsistent; if this error repeatedly occurs at the same time, it can affect database service stability.


.. _ERROR-849:

**ERROR CODE: -849, 'Trying to abort a transaction(%1$d) twice.'**

- This message occurs during deadlock resolution in the lock manager of the CUBRID database; that is, when trying to abort a transaction because of a deadlock, it occurs when an abnormal situation is found where an abort is attempted again while the transaction is already in the process of aborting; this generally occurs due to concurrency-control problems in the deadlock detection/resolution process or inconsistencies in transaction state management.


.. _ERROR-850:

**ERROR CODE: -850, 'Tran(%1$d) is neither lock holder nor lock waiter of an object(%2$d|%3$d|%4$d).'**

- This message occurs when releasing a lock in the lock manager of the CUBRID database; that is, when trying to release a lock on a specific object (record), it occurs when an abnormal state is found where the transaction is neither a holder (owner) of that object’s lock nor a waiter waiting for it; this typically appears when there is a problem in CUBRID’s internal lock-management logic, or when memory corruption or concurrency-control problems make the state information of lock resources inconsistent.


.. _ERROR-851:

**ERROR CODE: -851, 'Failed to allocate lock resource (%1$s).'**

- This message occurs when allocating a lock entry in the lock manager of the CUBRID database; that is, it is an error message that occurs when allocation of the memory resources needed to acquire a new lock or manage lock state fails; this generally occurs due to insufficient system memory, exhaustion of the lock manager’s internal resources, or problems with the memory allocator; it is an error that prevents the database’s lock-management function from operating normally.


.. _ERROR-852:

**ERROR CODE: -852, 'Total holders mode(%1$d) of an empty lock holder list is not NULL_LOCK.'**

- This message occurs during deadlock detection in the lock manager of the CUBRID database; that is, while traversing lock resources to detect a deadlock, it occurs when an abnormal state is found where there is no holder but the lock is not NULL_LOCK.


.. _ERROR-854:

**ERROR CODE: -854, 'WARNING: No lock holder, but lock waiters exist. refer to %1$s.'**

- This message is a warning that occurs during deadlock detection in the lock manager of the CUBRID database; that is, while traversing lock resources to detect a deadlock, it occurs when an abnormal state is found where there is no holder but there are waiters waiting for the lock; this typically appears when there is a problem in CUBRID’s internal lock-management logic, or when memory corruption or concurrency-control problems make the state information of lock resources inconsistent.


.. _ERROR-855:

**ERROR CODE: -855, 'WARNING: Two or more threads are lock-waiting for one transaction(%1$d).'**

- This message occurs during the process of requesting a lock in the lock manager of the CUBRID database; this situation typically appears when a single transaction is executed concurrently across multiple threads, or when nested lock requests occur within a transaction.



.. _ERROR-856:

**ERROR CODE: -856, 'WARNING: Strange lockwait state(%1$d|%2$d), thread(%3$d|%4$lld) and transaction(%5$d).'**

- This message is a warning that occurs when, while checking the lock-wait (lockwait) state in the lock manager of the CUBRID database, an unexpected abnormal state is detected; this typically appears when there is a problem in CUBRID’s internal lock-management logic, or when memory corruption or concurrency-control problems make lock-wait state information inconsistent.


.. _ERROR-859:

**ERROR CODE: -859, 'LATCH ON PAGE(%1$d|%2$d) ABORTED'**

- This message occurs when, in the CUBRID database, an attempt to acquire a latch (synchronization lock) for a specific page is aborted due to internal policies or exceptional situations (e.g., deadlock, forced abort, transaction rollback, and so on); it mainly appears when, during concurrency control, a deadlock is detected, a forced abort occurs due to long holding, an internal system error occurs, or a transaction is forcibly terminated and latch acquisition cannot be completed normally; the impact includes cases where an I/O bottleneck prevents the desired page from being fetched from the file, or where a dead-latch occurs while performing a B-tree split or merge.


.. _ERROR-860:

**ERROR CODE: -860, 'Cannot make current transaction as a part of a global transaction because its state is '%1$s'.'**

- This message occurs when trying to start a global transaction using the 2PC (Two-Phase Commit) protocol in the CUBRID database; that is, it is an error that occurs when the current transaction is not active or not in an appropriate state, but an attempt is made to make it part of a global transaction; it is a protective error to ensure correct use of the 2PC protocol and to guarantee the integrity of transaction state management.


.. _ERROR-861:

**ERROR CODE: -861, 'Cannot prepare to commit the current transaction with transaction identifier %1$d because that transaction is not made as a part of a global transaction.'**

- This message occurs when trying to prepare a transaction using the 2PC (Two-Phase Commit) protocol in the CUBRID database; that is, it is an error that occurs when the current transaction was not started as part of a global transaction, but a 2PC prepare is attempted; it is a protective error to ensure the correct sequence of using the 2PC protocol and to protect the integrity of the global transaction.


.. _ERROR-862:

**ERROR CODE: -862, 'Cannot set the user information of the global transaction with the identifier %1$d because its state is '%2$s'.'**

- This message occurs when trying to set user information for a global transaction that uses the 2PC (Two-Phase Commit) protocol in the CUBRID database; that is, it is an error that occurs when the global transaction is already in an intermediate phase of the 2PC protocol and user information cannot be changed; this situation generally occurs when using the 2PC protocol to ensure transaction consistency among multiple databases in a distributed-transaction environment; it is a protective error to protect transaction state management and data consistency.


.. _ERROR-966:

**ERROR CODE: -966, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on object %6$d|%7$d|%8$d because of deadlock. You are waiting for user(s) %9$s to finish.'**

- This message indicates that, in the CUBRID database, while a transaction is waiting to acquire a lock on a specific object (record), a deadlock situation occurred and a timeout happened; a deadlock means a situation where two or more transactions fall into an infinite wait state while each waits for locks on resources held by the others, and CUBRID detects and resolves this by aborting some of the transactions.


.. _ERROR-967:

**ERROR CODE: -967, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on class %6$s because of deadlock. You are waiting for user(s) %7$s to finish.'**

- This message indicates that, in the CUBRID database, while a transaction is waiting to acquire a lock on a specific table, a deadlock situation occurred and a timeout happened; a deadlock means a situation where two or more transactions fall into an infinite wait state while each waits for locks on resources held by the others; CUBRID detects the deadlock and resolves it by aborting some transactions, and in this case it informs that the transaction timed out due to a deadlock.



.. _ERROR-968:

**ERROR CODE: -968, 'Your transaction (index %1$d, %2$s@%3$s|%4$d) timed out waiting on %5$s lock on instance %6$d|%7$d|%8$d of class %9$s because of deadlock. You are waiting for user(s) %10$s to finish.'**

- This message appears when, in a deadlock situation, a lock on a specific object (record) cannot be acquired and a timeout occurs.


.. _ERROR-1002:

**ERROR CODE: -1002, 'The current transaction could not acquire a database lock it requires to continue processing.'**

- This message occurs when a transaction could not acquire a lock on a required resource.


.. _ERROR-1021:

**ERROR CODE: -1021, 'A deadlock cycle is detected. %1$s.'**

- This message indicates that a deadlock (Deadlock) occurred in the database system: two or more transactions are in a circular wait state where each cannot proceed because it is waiting for locks held by others to be released; CUBRID’s lock manager (Lock Manager) detects such a deadlock and, to prevent the entire system from stopping, forcibly terminates (rolls back) one of the involved transactions and reports this error.


.. _ERROR-1154:

**ERROR CODE: -1154, 'MVCC row %1$d|%2$d|%3$d does not satisfies reevaluation.'**

- This message occurs when, in the CUBRID database under an MVCC (Multi-Version Concurrency Control) environment, reevaluation of conditions for a specific record fails; that is, when a transaction attempts to access a specific record, the system detects that the record does not satisfy the MVCC conditions for the current transaction and raises this error; this situation generally occurs when the record’s version has changed during concurrency control, or when the visibility conditions required by the transaction isolation level are not satisfied; it is a protective error for database consistency and concurrency control. (However, “reevaluation” varies depending on the isolation level.)


.. _ERROR-1156:

**ERROR CODE: -1156, 'MVCC can't get snapshot'**

- This message indicates an error that occurs when, in the MVCC (Multi-Version Concurrency Control) system of the CUBRID database, a snapshot for a transaction cannot be created; an MVCC snapshot is an important mechanism that captures a consistent state of the database when a transaction starts, and through it the transaction can maintain a consistent data view.


.. _ERROR-1157:

**ERROR CODE: -1157, 'Isolation level value in MVCC must be 'read committed', 'repeatable read' or 'serializable'.'**

- This message occurs when, in the CUBRID database under an MVCC environment, an unsupported value is specified when setting the transaction isolation level (Isolation Level); that is, in CUBRID’s MVCC mode, only three isolation levels are supported: 'read committed', 'repeatable read', and 'serializable', and this validation error occurs when a value outside that range is about to be set; this situation generally occurs when an incorrect value is passed in a SET TRANSACTION ISOLATION LEVEL statement or an internal API call that changes the transaction isolation level; it is a validation error to protect the database’s concurrency-control mechanism.


.. _ERROR-1176:

**ERROR CODE: -1176, 'Latch promotion for page %1$d of volume %2$d failed.'**

- This message indicates that, in the CUBRID database system, a latch (latch) promotion (promote) operation failed for a specific page (B-tree); a latch is a lightweight lock mechanism used internally to control concurrency for in-memory data structures (e.g., pages in the page buffer); “latch promotion” generally means upgrading a shared latch to an exclusive latch, or attempting to acquire a higher-level latch in order to perform a write on the page; this failure can occur during page buffer management, file management, or B-tree operations, and it suggests that there was a problem acquiring the latch necessary to maintain page consistency; it can occur due to concurrency issues, page-state inconsistencies, or internal errors, and it can lead to transaction failures or database instability.

