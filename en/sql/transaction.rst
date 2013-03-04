********************
Transaction and Lock
********************

This chapter covers issues relating to concurrency and restore, as well as how to commit or rollback transactions.

In multi-user environment, controlling access and update is essential to protect database integrity and ensure that a user’s transaction will have accurate and consistent data. Without appropriate control, data could be updated incorrectly in the wrong order.

To control parallel operations on the same data, data must be locked during transaction, and unacceptable access to the data by another transaction must be blocked until the end of the transaction. In addition, any updates to a certain class must not be seen by other users before they are committed. If updates are not committed, all queries entered after the last commit or rollback of the update can be invalidated.

All examples introduced here were executed by csql.

Database Transaction
====================

A database transaction groups CUBRID queries into a unit of consistency (for ensuring valid results in multi-user environment) and restore (for making the results of committed transactions permanent and ensuring that the aborted transactions are canceled in the database despite any failure, such as system failure). A transaction is a collection of one or more queries that access and update the database.

CUBRID allows multiple users to access the database simultaneously and manages accesses and updates to prevent inconsistency of the database. For example, if data is updated by one user, the changes made by this transaction are not seen to other users or the database until the updates are committed. This principle is important because the transaction can be rolled back without being committed.

You can delay permanent updates to the database until you are confident of the transaction result. Also, you can remove (**ROLLBACK**) all updates in the database if an unsatisfactory result or failure occurs in the application or computer system during the transaction. The end of the transaction is determined by the **COMMIT WORK** or **ROLLBACK WORK** statement. The **COMMIT WORK** statement makes all updates permanent while the **ROLLBACK WORK** statement cancels all updates entered in the transaction.

Transaction Commit
------------------

Updates that occurred in the database are not permanently stored until the **COMMIT WORK** statement is executed. "Permanently stored" means that storing the updates in the disk is completed; The **WORK** keyword can be omitted. In addition, other users of the database cannot see the updates until they are permanently applied. For example, when a new row is inserted into a class, only the user who inserted the row can access it until the database transaction is committed. (If the **UNCOMMITTED INSTANCES** isolation level is used, other users can see inconsistent uncommitted updates.)

All locks obtained by the transaction are released after the transaction is committed. ::

    COMMIT [ WORK ]

The database transaction in the following example consists of three **UPDATE** statements and changes three column values of seats from the stadium. To compare the results, check the current values and names before the update is made. Since, by default, csql runs in an autocommit mode, the following example is executed after setting the autocommit mode to off.

.. code-block:: sql

    ;autocommit off
    AUTOCOMMIT IS OFF
    
    SELECT name, seats
    FROM stadium WHERE code IN (30138, 30139, 30140);
       name                        seats
    ==================================
        'Athens Olympic Tennis Centre'         3200
        'Goudi Olympic Hall'         5000
        'Vouliagmeni Olympic Centre'         3400

Let each **UPDATE** statement have the current seats of each stadium. To verify whether the command is correctly executed, you can retrieve the columns related to the seats table.

.. code-block:: sql

    UPDATE stadium
    SET seats = seats + 1000
    WHERE code IN (30138, 30139, 30140);
     
    SELECT name, seats FROM stadium WHERE code in (30138, 30139, 30140);
        name                        seats
    ===================================
        'Athens Olympic Tennis Centre'         4200
        'Goudi Olympic Hall'         6000
        'Vouliagmeni Olympic Centre'         4400

If the update is properly done, the changes can be semi-permanently fixed. In this time, use the **COMMIT WORK**  as below:

.. code-block:: sql

    COMMIT WORK;

.. note:: In CUBRID, an auto-commit mode is set by default for transaction management.

An auto-commit mode is a mode that commits or rolls back all SQL statements. The transaction is committed automatically if the SQL is executed successfully, or is rolled back automatically if an error occurs.Such auto commit modes are supported in any interfaces.

In CCI, PHP, ODBC and OLE DB interfaces, you can configure auto-commit mode by using **CCI_DEFAULT_AUTOCOMMIT** upon startup of an application. If configuration on broker parameter is omitted, the default value is set to **ON**. To change auto-commit mode, use the following functions by interface: **cci_set_autocommit** () for CCI interface and **cubrid_set_autocommit** () for PHP interface.

For session command (**;AUtocommit**) which enables auto-commit configuration in CSQL Interpreter, see :ref:`csql-session-commands`.

Transaction Rollback
--------------------

The **ROLLBACK WORK** statement removes all updates to the database since the last transaction. The **WORK** keyword can be omitted. By using this statement, you can cancel incorrect or unnecessary updates before they are permanently applied to the database. All locks obtained during the transaction are released. ::

    ROLLBACK [ WORK ]

The following example shows two commands that modify the definition and the row of the same table.

.. code-block:: sql

    ALTER TABLE code DROP s_name;
    INSERT INTO code (s_name, f_name) VALUES ('D','Diamond');
     
    ERROR: s_name is not defined.

The **INSERT** statement fails because the *s_name* column has been dropped in the definition of *code*. The data intended to be entered to the *code* table is correct, but the *s_name* column is wrongly removed. At this point, you can use the **ROLLBACK WORK** statement to restore the original definition of the *code* table.

.. code-block:: sql

    ROLLBACK WORK;

Later, remove the *s_name* column by entering the **ALTER TABLE** again and modify the **INSERT** statement. The **INSERT** command must be entered again because the transaction has been aborted. If the database update has been done as intended, commit the transaction to make the changes permanent.

.. code-block:: sql

    ALTER TABLE code drop s_name;
    INSERT INTO code (f_name) VALUES ('Diamond');

    COMMIT WORK;

Savepoint and Partial Rollback
------------------------------

A savepoint is established during the transaction so that database changes made by the transaction are rolled back to the specified savepoint. Such operation is called a partial rollback. In a partial rollback, database operations (insert, update, delete, etc.) after the savepoint are rolled back, and transaction operations before it are not rolled back. The transaction can proceed with other operations after the partial rollback is executed. Or the transaction can be terminated with the **COMMIT WORK** or **ROLLBACK WORK** statement. Note that the savepoint does not commit the changes made by the transaction.

A savepoint can be created at a certain point of the transaction, and multiple savepoints can be used for a certain point. If a partial rollback is executed to a savepoint before the specified savepoint or the transaction is terminated with the **COMMIT WORK** or **ROLLBACK WORK** statement, the specified savepoint is removed. The partial rollback after the specified savepoint can be performed multiple times.

Savepoints are useful because intermediate steps can be created and named to control long and complicated utilities. For example, if you use a savepoint during the update operation, you don't need to perform all statements again when you made a mistake. ::

    SAVEPOINT mark;
    mark:
    _ a SQL identifier
    _ a host variable (starting with :)

If you make *mark* all the same value when you specify multiple savepoints in a single transaction, only the latest savepoint appears in the partial rollback. The previous savepoints remain hidden until the rollback to the latest savepoint is performed and then appears when the latest savepoint disappears after being used. ::

    ROLLBACK [ WORK ] [ TO [ SAVEPOINT ] mark ] [ ; ]
    mark:
    _ a SQL identifier
    _ a host variable (starting with :)

Previously, the **ROLLBACK WORK** statement canceled all database changes added since the latest transaction. The **ROLLBACK WORK** statement is also used for the partial rollback that rolls back the transaction changes after the specified savepoint.

If *mark* value is not given, the transaction terminates canceling all changes including all savepoints created in the transaction. If *mark* value is given, changes after the specified savepoint are canceled and the ones before it are remained.

.. code-block:: sql

    CREATE TABLE athlete2 (name VARCHAR(40), gender CHAR(1), nation_code CHAR(3), event VARCHAR(30));
    INSERT INTO athlete2(name, gender, nation_code, event)
    VALUES ('Lim Kye-Sook', 'W', 'KOR', 'Hockey');
    SAVEPOINT SP1;
     
    SELECT * from athlete2;
    INSERT INTO athlete2(name, gender, nation_code, event)
    VALUES ('Lim Jin-Suk', 'M', 'KOR', 'Handball');
     
    SELECT * FROM athlete2;
    SAVEPOINT SP2;
     
    RENAME TABLE athlete2 AS sportsman;
    SELECT * FROM sportsman;
    ROLLBACK WORK TO SP2;

In the example above, the name change of the *athlete2* table is rolled back by the partial rollback. The following example shows how to execute the query with the original name and examining the result.

.. code-block:: sql

    SELECT * FROM athlete2;
    DELETE FROM athlete2 WHERE name = 'Lim Jin-Suk';
    SELECT * FROM athlete2;
    ROLLBACK WORK TO SP2;

In the example above, deleting 'Lim Jin-Suk' is discarded by rollback work to SP2 command.
The following example shows how to roll back to SP1.

.. code-block:: sql

    SELECT * FROM athlete2;
    ROLLBACK WORK TO SP1;
    SELECT * FROM athlete2;
    COMMIT WORK;

.. _cursor-holding:

Cursor Holdability
==================

Cursor holdability is when an application holds the record set of the **SELECT** query result to fetch the next record even after performing an explicit commit or an automatic commit. In each application, cursor holdability can be specified to Connection level or Statement level. If it is not specified, the cursor is held by default. Therefore, **HOLD_CURSORS_OVER_COMMIT** is the default setting.

The following code shows how to set cursor holdability in JDBC:

.. code-block:: java

    // set cursor holdability at the connection level
    conn.setHoldability(ResultSet.HOLD_CURSORS_OVER_COMMIT);
     
    // set cursor holdability at the statement level which can override the connection’s
    PreparedStatement pStmt = conn.prepareStatement(sql,
                                         ResultSet.TYPE_SCROLL_SENSITIVE,
                                         ResultSet.CONCUR_UPDATABLE,
     ResultSet.HOLD_CURSORS_OVER_COMMIT);

To set cursor holdability to close the cursor when a transaction is committed, set **ResultSet.CLOSE_CURSORS_AT_COMMIT**, instead of **ResultSet.HOLD_CURSORS_OVER_COMMIT**, in the above example.

The default setting for applications that were developed based on CCI is to hold the cursor. If the cursor is set to 'not to hold a cursor' at connection level and you want to hold the cursor, define the **CCI_PREPARE_HOLDABLE** flag while preparing a query. The default setting for CCI drivers (PHP, PDO, ODBC, OLE DB, ADO.NET, Perl, Python, Ruby) is to hold the cursor. To check whether a driver supports the cursor holdability setting, refer to the **PREPARE** function of the driver.

.. note:: \

    * Note that versions lower than CUBRID 9.0 do not support cursor holdability. The default setting of those versions is to close all cursors at commit.
    * CUBRID currently does not support ResultSet.HOLD_CURSORS_OVER_COMMIT in XAConnection interface. It will be supported later.
    
**Cursor-related Operation at Transaction Commit**

When a transaction is committed, all statements and result sets that are closed are released even if you have set cursor holdability. After that, when the result sets are used for another transaction, some or all of the result sets should be closed as required.

When a transaction is rolled back, all result sets are closed. This means that all result sets held in the previous transaction are closed because you have set cursor holdability.

**When the Result Sets are Closed**

The result sets that hold the cursor are closed in the following cases:

*   The result set is closed by driver (ex. rs.close(), etc)
*   The statement is closed by driver (ex. stmt.close(), etc)
*   Driver is disconnected

**Relationship with CAS**

When the connection between an application and the CAS is closed, all result sets are automatically closed even if you have set cursor holdability in the application. The setting value of
**KEEP_CONNECTION**, the broker parameter, affects cursor holdability of the result set.

*   KEEP_CONNECTION = ON: Cursor holdability is not affected.
*   KEEP_CONNECTION = AUTO: The CAS cannot be restarted while the result set with cursor holdability is open.

.. warning:: Usage of memory will increase in the status of result set opened. Thus, you should close the result set after completion.

.. note::

    Note that CUBRID versions lower than 9.0 do not support cursor holdability and the cursor is automatically closed when a transaction is committed. Therefore, the recordset of the **SELECT** query result is not kept. To keep the recordset of the **SELECT** query result in CUBRID versions lower than 9.0, set the auto commit mode to false and the record should be fetched before the transaction is committed.

.. _database-concurrency:

Database Concurrency
====================

If there are multiple users with read and write authorization to a database, possibility exists that more than one user will access the database simultaneously. Controlling access and update in multi-user environment is essential to protect database integrity and ensure that users and transactions should have accurate and consistent data. Without appropriate control, data could be updated incorrectly in the wrong order.

Like most commercial database systems, CUBRID adopts serializability, an element that is essential to maintaining data concurrency within the database. Serializability ensures no interference between transactions when multiple transactions are executed at once. It is guaranteed more with the higher isolation level. This principle is based on the assumption that database consistency is guaranteed as long as transaction is executed automatically.

The transaction must ensure database concurrency, and each transaction must guarantee appropriate results. When multiple transactions are being executed at once, an event in transaction T1 should not affect an event in transaction T2. This means isolation. Transaction isolation level is the degree to which a transaction is separated from all other concurrent transactions. The higher isolation level means the lower interference from other transactions. The lower isolation level means the higher the concurrency. A database determines whether which lock is applied to tables and records based on these isolation levels. Therefore, can control the level of consistency and concurrency specific to a service by setting appropriate isolation level.

You can set an isolation level by using the :ref:`set-transaction-isolation-level` statement or system parameters provided by CUBRID. For details, see :ref:`lock-parameters`.

The read operations that allow interference between transactions with isolation levels are as follows:

*   **Dirty read** : A transaction T2 can read D' before a transaction T1 updates data D to D' and commits it.
*   **Non-repeatable read** : A transaction T1 can read other value, if a transaction T2 updates data while data is retrieved in the transaction T2 multiple times.
*   **Phantom read** : A transaction T1 can read E, if a transaction T2 inserts new record E while data is retrieved in the transaction T1 multiple times.

The default value of CUBRID isolation level is :ref:`isolation-level-3`.

**Isolation Levels Provided by CUBRID**

+------------------------------------------+-----------------------------+--------+---------------+----------+------------------------+
| CUBRID Isolation Level(isolation_level)  | Other DBMS Isolation Level  | DIRTY  | UNREPEATABLE  | PHANTOM  | Schema Changes of the  |
|                                          | (isolation_level)           | READ   | READ          | READ     | Table Being Retrieved  |
+==========================================+=============================+========+===============+==========+========================+
| :ref:`isolation-level-6` (6)             | SERIALIZABLE (4)            | N      | N             | N        | N                      |
+------------------------------------------+-----------------------------+--------+---------------+----------+------------------------+
| :ref:`isolation-level-5` (5)             | REPEATABLE READ (3)         | N      | N             | Y        | N                      |
+------------------------------------------+-----------------------------+--------+---------------+----------+------------------------+
| :ref:`isolation-level-4` (4)             | READ COMMITTED (2)          | N      | Y             | Y        | N                      |
+------------------------------------------+-----------------------------+--------+---------------+----------+------------------------+
| :ref:`isolation-level-3` (3)             | READ UNCOMMITTED (1)        | Y      | Y             | Y        | N                      |
+------------------------------------------+-----------------------------+--------+---------------+----------+------------------------+
| :ref:`isolation-level-2` (2)             |                             | N      | Y             | Y        | Y                      |
+------------------------------------------+-----------------------------+--------+---------------+----------+------------------------+
| :ref:`isolation-level-1` (1)             |                             | Y      | Y             | Y        | Y                      |
+------------------------------------------+-----------------------------+--------+---------------+----------+------------------------+

Lock Protocol
=============

In the two-phase locking protocol used by CUBRID, a transaction obtains a shared lock before it reads an object, and an exclusive lock before it updates the object so that conflicting operations are not executed simultaneously.

If transaction T1 requires a lock, CUBRID checks if the requested lock conflicts with the existing one. If it does, transaction T1 enters a standby state and delays the lock. If another transaction T2 releases the lock, transaction T1 resumes and obtains it. Once the lock is released, the transaction do not require any more new locks.

Granularity Locking
-------------------

CUBRID uses a granularity locking protocol to decrease the number of locks. In the granularity locking protocol, a database can be modeled as a hierarchy of lockable units: bigger locks have more granular locks.

For example, suppose that a database consists of multiple tables and each table consists of multiple instances. If the database is locked, all tables and instances are implicitly considered to be locked. A lock on a big unit results in less overhead, because only one lock needs to be managed. However, it leads to decreased concurrency because almost all concurrent transactions conflict with each other. The finer the granularity, the better the concurrency; it causes more overhead because more locks need to be managed. CUBRID selects a locking granularity level based on the operation being executed. For example, if a transaction retrieves all instances of a table, the entire tables will be locked, rather than each instance. If the transaction accesses a few instances of the table, the instances are locked individually.

If the locking granularities overlap, effects of a finer granularity are propagated in order to prevent conflicts. That is, if a shared lock is required on an instance of a table, an intention shared lock will be set on the table. If an exclusive lock is required on an instance of a table, an intention exclusive lock will be set on the table. An intention shared lock on a table means that a shared lock can be set on an instance of the table. An intention exclusive lock on a table means that a shared/exclusive lock can be set on an instance of the table. That is, if an intention shared lock on a table is allowed in one transaction, another transaction cannot obtain an exclusive lock on the table (for example, to add a new column). However, the second transaction may obtain a shared lock on the table. If an intention exclusive lock on the table is allowed in one transaction, another transaction cannot obtain a shared lock on the table (for example, a query on an instance of the tables cannot be executed because it is being changed).

A mechanism called lock escalation is used to limit the number of locks being managed. If a transaction has more than a certain number of locks (a number which can be changed by the **lock_escalation** system parameter), the system begins to require locks at the next higher level of granularity. This escalates the locks to a coarser level of granularity. CUBRID performs lock escalation when no transactions have a higher level of granularity in order to avoid a deadlock caused by lock conversion.

.. _lock-mode:

Lock Mode Types And Compatibility
---------------------------------

CUBRID determines the lock mode depending on the type of operation to be performed by the transaction, and determines whether or not to share the lock depending on the mode of the lock preoccupied by another transaction. Such decisions concerning the lock are made by the system automatically. Manual assignment by the user is not allowed. To check the lock information of CUBRID, use the **cubrid lockdb** *db_name* command. For details, see :ref:`lockdb`.

*   **Shared lock (shared lock, S_LOCK)**
    : This lock is obtained before the read operation is executed on the object. It can be obtained by multiple transactions for the same object.
    Transaction T1 obtains the shared lock first before it performs the read operation on a certain object X, and releases it immediately after it completes the operation even before transaction T1 is committed. Here, transaction T2 and T3 can perform the read operation on  X concurrently, but not the update operation.

*   **Exclusive lock (exclusive lock, X_LOCK)**
    : This lock is obtained before the update operation is executed on the object. It can only be obtained by one transaction.
    Transaction T1 obtains the exclusive lock first before it performs the update operation on a certain object X, and does not release it until transaction T1 is committed even after the update operation is completed. Therefore, transaction T2 and T3 cannot perform the read operation as well on X before transaction T1 releases the exclusive lock.

*   **Update lock (update lock, U_LOCK)**
    : This lock is obtained when the read operation is executed in the expression before the update operation is performed.
    For example, when an UPDATE statement combined with a **WHERE** clause is executed, execute the operation by obtaining the update lock for each row and the exclusive lock only for the result rows that satisfy the condition when performing index search or full scan search in the **WHERE** clause. The update lock is converted to an exclusive lock when the actual update operation is performed. It can be called a quasi-exclusive lock because it does not allow read lock on the same object for another transaction.

*   **Intention lock (intention lock)**
    : A lock that is set inherently in a higher-level object than X to protect the lock on the object X of a certain level.
    For example, when a shared lock is requested for a certain row, prevent a situation from occurring in which the table is locked by another transaction by setting the intention shared lock as well on the table at the higher level in hierarchy. Therefore, the intention lock is not set on rows at the lowest level, but is set on higher-level objects. The types of intention locks are as follows:

*   **Intention shared lock (intention shared lock, IS_LOCK)**
    : If the intention shared lock is set on the table, which is the higher-level object, as a result of the shared lock set on a certain row, another transaction cannot perform operations such as changing the schema of the table (e.g. adding a column or changing the table name) or updating all rows. However updating some rows or viewing all rows is allowed.

*   **Intention exclusive lock (intention exclusive lock, IX_LOCK)**
    : If the intention exclusive lock is set on the table, which is the higher-level object, as a result of the exclusive lock set on a certain row, another transaction cannot perform operations such as changing the schema of the table, updating or viewing all rows. However updating some rows is allowed.

*   **Shared with intent exclusive (shared with intent exclusive, SIX_LOCK)**
    : This lock is set on the higher-level object inherently to protect the shared lock set on all objects at the lower hierarchical level and the intention exclusive lock on some object at the lower hierarchical level.
    Once the shared intention exclusive lock is set on a table, another transaction cannot change the schema of the table, update all/some rows or view all rows. However, viewing some rows is allowed.

The following table briefly shows the lock compatibility between the locks described below. Compatibility means that the lock requester can obtain a lock while the lock holder is keeping the lock obtained for the object X. N/a means 'not applicable'.

**Lock Compatibility**

+--------------------------------------+---------------------------------------------------------------------------------------------------+
|                                      | **Lock Holder**                                                                                   |
|                                      +-----------------+-------------+------------+-------------+--------------+------------+------------+
|                                      | **NULL_LOCK**   | **IS_LOCK** | **S_LOCK** | **IX_LOCK** | **SIX_LOCK** | **U_LOCK** | **X_LOCK** |
+----------------------+---------------+-----------------+-------------+------------+-------------+--------------+------------+------------+
| **Lock Requester**   | **NULL_LOCK** | TRUE            | TRUE        | TRUE       | TRUE        | TRUE         | TRUE       | TRUE       |
| **(lock requester)** |               |                 |             |            |             |              |            |            |
|                      +---------------+-----------------+-------------+------------+-------------+--------------+------------+------------+
|                      | **IS_LOCK**   | TRUE            | TRUE        | TRUE       | TRUE        | TRUE         | N/A        | FALSE      |
|                      +---------------+-----------------+-------------+------------+-------------+--------------+------------+------------+
|                      | **S_LOCK**    | TRUE            | TRUE        | TRUE       | FALSE       | FALSE        | FALSE      | FALSE      |
|                      +---------------+-----------------+-------------+------------+-------------+--------------+------------+------------+
|                      | **IX_LOCK**   | TRUE            | TRUE        | FALSE      | TRUE        | FALSE        | N/A        | FALSE      |
|                      +---------------+-----------------+-------------+------------+-------------+--------------+------------+------------+
|                      | **SIX_LOCK**  | TRUE            | TRUE        | FALSE      | FALSE       | FALSE        | N/A        | FALSE      |
|                      +---------------+-----------------+-------------+------------+-------------+--------------+------------+------------+
|                      | **U_LOCK**    | TRUE            | N/A         | TRUE       | N/A         | N/A          | FALSE      | FALSE      |
|                      +---------------+-----------------+-------------+------------+-------------+--------------+------------+------------+
|                      | **X_LOCK**    | TRUE            | FALSE       | FALSE      | FALSE       | FALSE        | FALSE      | FALSE      |
+----------------------+---------------+-----------------+-------------+------------+-------------+--------------+------------+------------+

*   **NULL_LOCK** : No lock


**Example**

+-------------------------------------------------------------------------------+----------------------------------------------------------------------------+
| session 1                                                                     | session 2                                                                  |
+===============================================================================+============================================================================+
| ::                                                                            | ::                                                                         |
|                                                                               |                                                                            |
|   csql> ;autocommit off                                                       |   csql> ;autocommit off                                                    |
|                                                                               |                                                                            |
|   AUTOCOMMIT IS OFF                                                           |   AUTOCOMMIT IS OFF                                                        |
|                                                                               |                                                                            |
|   csql> set transaction isolation level 4;                                    |   csql> set transaction isolation level 4;                                 |
|                                                                               |                                                                            |
|   Isolation level set to:                                                     |   Isolation level set to:                                                  |
|   REPEATABLE READ SCHEMA, READ COMMITTED INSTANCES.                           |   REPEATABLE READ SCHEMA, READ COMMITTED INSTANCES.                        |
|                                                                               |                                                                            |
|                                                                               | ::                                                                         |
|                                                                               |                                                                            |
|                                                                               |   $ cubrid lockdb demodb                                                   |
|                                                                               |                                                                            |
|                                                                               |   *** Lock Table Dump ***                                                  |
|                                                                               |                                                                            |
|                                                                               |   ...                                                                      |
|                                                                               |                                                                            |
|                                                                               |   Object Lock Table:                                                       |
|                                                                               |         Current number of objects which are locked    = 0                  |
|                                                                               |         Maximum number of objects which can be locked = 10000              |
|                                                                               |                                                                            |
|                                                                               |   ...                                                                      |
+-------------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                            |                                                                            |
|                                                                               |                                                                            |
|   csql> SELECT nation_code, gold FROM participant WHERE nation_code='USA';    |                                                                            |
|                                                                               |                                                                            |
|    nation_code                  gold                                          |                                                                            |
|   ======================================                                      |                                                                            |
|   'USA'                          36                                           |                                                                            |
|   'USA'                          37                                           |                                                                            |
|   'USA'                          44                                           |                                                                            |
|   'USA'                          37                                           |                                                                            |
|   'USA'                          36                                           |                                                                            |
|                                                                               |                                                                            |
| ::                                                                            |                                                                            |
|                                                                               |                                                                            |
|   $ cubrid lockdb demodb                                                      |                                                                            |
|                                                                               |                                                                            |
|   *** Lock Table Dump ***                                                     |                                                                            |
|                                                                               |                                                                            |
|   ...                                                                         |                                                                            |
|                                                                               |                                                                            |
|   Object type: Root class.                                                    |                                                                            |
|   LOCK HOLDERS:                                                               |                                                                            |
|     Tran_index =   2, Granted_mode =  IS_LOCK, Count =   1, Nsubgranules =  1 |                                                                            |
|                                                                               |                                                                            |
|   Object type: Class = participant.                                           |                                                                            |
|   LOCK HOLDERS:                                                               |                                                                            |
|     Tran_index =   2, Granted_mode =  IS_LOCK, Count =   2, Nsubgranules =  0 |                                                                            |
+-------------------------------------------------------------------------------+----------------------------------------------------------------------------+
|                                                                               | ::                                                                         |
|                                                                               |                                                                            |
|                                                                               |   csql> UPDATE participant SET gold = 11 WHERE nation_code = 'USA';        |
+-------------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                            |                                                                            |
|                                                                               |                                                                            |
|   csql> SELECT nation_code, gold FROM participant WHERE nation_code='USA';    |                                                                            |
|                                                                               |                                                                            |
|   /* no results until transaction 2 releases a lock                           |                                                                            |
|                                                                               |                                                                            |
| ::                                                                            |                                                                            |
|                                                                               |                                                                            |
|   $ cubrid lockdb demodb                                                      |                                                                            |
|                                                                               |                                                                            |
|   *** Lock Table Dump ***                                                     |                                                                            |
|                                                                               |                                                                            |
|   ...                                                                         |                                                                            |
|                                                                               |                                                                            |
|   Object type: Instance of class ( 0|   551|   7) = participant.              |                                                                            |
|   LOCK HOLDERS:                                                               |                                                                            |
|       Tran_index =   3, Granted_mode =   X_LOCK, Count =   2                  |                                                                            |
|                                                                               |                                                                            |
|   ...                                                                         |                                                                            |
|                                                                               |                                                                            |
|   Object type: Root class.                                                    |                                                                            |
|   LOCK HOLDERS:                                                               |                                                                            |
|     Tran_index =   3, Granted_mode =  IX_LOCK, Count =   1, Nsubgranules =  3 |                                                                            |
|                                                                               |                                                                            |
|   NON_2PL_RELEASED:                                                           |                                                                            |
|     Tran_index =   2, Non_2_phase_lock =  IS_LOCK                             |                                                                            |
|                                                                               |                                                                            |
|   ...                                                                         |                                                                            |
|                                                                               |                                                                            |
|   Object type: Class = participant.                                           |                                                                            |
|   LOCK HOLDERS:                                                               |                                                                            |
|     Tran_index =   3, Granted_mode =  IX_LOCK, Count =   3, Nsubgranules =  5 |                                                                            |
|     Tran_index =   2, Granted_mode =  IS_LOCK, Count =   2, Nsubgranules =  0 |                                                                            |
+-------------------------------------------------------------------------------+----------------------------------------------------------------------------+
|                                                                               | ::                                                                         |
|                                                                               |                                                                            |
|                                                                               |   csql> COMMIT;                                                            |
|                                                                               |                                                                            |
|                                                                               |   Current transaction has been committed.                                  |
+-------------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                            |                                                                            |
|                                                                               |                                                                            |
|   nation_code                  gold                                           |                                                                            |
|   =================================                                           |                                                                            |
|   'USA'                          11                                           |                                                                            |
|   'USA'                          11                                           |                                                                            |
|   'USA'                          11                                           |                                                                            |
|   'USA'                          11                                           |                                                                            |
|   'USA'                          11                                           |                                                                            |
|                                                                               |                                                                            |
| ::                                                                            |                                                                            |
|                                                                               |                                                                            |
|   $ cubrid lockdb demodb                                                      |                                                                            |
|                                                                               |                                                                            |
|   ...                                                                         |                                                                            |
|                                                                               |                                                                            |
|   Object type: Root class.                                                    |                                                                            |
|   LOCK HOLDERS:                                                               |                                                                            |
|     Tran_index =   2, Granted_mode =  IS_LOCK, Count =   1, Nsubgranules =  1 |                                                                            |
|                                                                               |                                                                            |
|   Object type: Class = participant.                                           |                                                                            |
|   LOCK HOLDERS:                                                               |                                                                            |
|     Tran_index =   2, Granted_mode =  IS_LOCK, Count =   3, Nsubgranules =  0 |                                                                            |
|                                                                               |                                                                            |
|   ...                                                                         |                                                                            |
+-------------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                            |                                                                            |
|                                                                               |                                                                            |
|   csql> COMMIT;                                                               |                                                                            |
|                                                                               |                                                                            |
|   Current transaction has been committed.                                     |                                                                            |
|                                                                               |                                                                            |
| ::                                                                            |                                                                            |
|                                                                               |                                                                            |
|   $ cubrid lockdb demodb                                                      |                                                                            |
|                                                                               |                                                                            |
|   ...                                                                         |                                                                            |
|                                                                               |                                                                            |
|   Object Lock Table:                                                          |                                                                            |
|           Current number of objects which are locked    = 0                   |                                                                            |
|           Maximum number of objects which can be locked = 10000               |                                                                            |
+-------------------------------------------------------------------------------+----------------------------------------------------------------------------+

Transaction Deadlock
--------------------

A deadlock  is a state in which two or more transactions wait at once for another transaction's lock to be released. CUBRID resolves the problem by rolling back one of the transactions because transactions in a deadlock state will hinder the work of another transaction. The transaction to be rolled back is usually the transaction which has made the least updates; it is usually the one that started more recently. As soon as a transaction is rolled back, the lock held by the transaction is released and other transactions in a deadlock are permitted to proceed.

It is impossible to predict such deadlocks, but it is recommended that you reduce the range to which lock is applied by setting the index, shortening the transaction, or setting the transaction isolation level as low in order to decrease such occurrences.

Note that if you configure the value of **error_log_level**, which indicates the severity level, to NOTIFICATION, information on lock is stored in error log file of server upon deadlock occurrences.

In the following error log file, (1) indicates a table name which causes deadlock state and (2) indicates an index name. ::

    demodb_20111102_1811.err
        ...
        OID = -532| 520| 1
    (1) Object type: Index key of class ( 0| 417| 7) = tbl.
        BTID = 0| 123| 530
    (2) Index Name : i_tbl_col1
        Total mode of holders = NS_LOCK, Total mode of waiters = NULL_LOCK.
        Num holders= 1, Num blocked-holders= 0, Num waiters= 0
        LOCK HOLDERS:
        Tran_index = 2, Granted_mode = NS_LOCK, Count = 1
    ...

**Example**

+-----------------------------------------------------------------------------------------------------+------------------------------------------------------+
| session 1                                                                                           | session 2                                            |
+=====================================================================================================+======================================================+
| ::                                                                                                  | ::                                                   |
|                                                                                                     |                                                      |
|   csql> ;autocommit off                                                                             |   csql> ;autocommit off                              |
|                                                                                                     |                                                      |
|   AUTOCOMMIT IS OFF                                                                                 |   AUTOCOMMIT IS OFF                                  |
|                                                                                                     |                                                      |
|   csql> set transaction isolation level 6;                                                          |   csql> set transaction isolation level 6;           |
|                                                                                                     |                                                      |
|   Isolation level set to:                                                                           |   Isolation level set to:                            |
|   SERIALIZABLE                                                                                      |   SERIALIZABLE                                       |
+-----------------------------------------------------------------------------------------------------+------------------------------------------------------+
| ::                                                                                                  |                                                      |
|                                                                                                     |                                                      |
|   csql> CREATE TABLE lock_tbl(host_year integer, nation_code char(3));                              |                                                      |
|   csql> INSERT INTO lock_tbl VALUES (2004, 'KOR');                                                  |                                                      |
|   csql> INSERT INTO lock_tbl VALUES (2004, 'USA');                                                  |                                                      |
|   csql> INSERT INTO lock_tbl VALUES (2004, 'GER');                                                  |                                                      |
|   csql> INSERT INTO lock_tbl VALUES (2008, 'GER');                                                  |                                                      |
|   csql> COMMIT;                                                                                     |                                                      |
|                                                                                                     |                                                      |
|   csql> SELECT * FROM lock_tbl;                                                                     |                                                      |
|                                                                                                     |                                                      |
|       host_year  nation_code                                                                        |                                                      |
|   ===================================                                                               |                                                      |
|            2004  'KOR'                                                                              |                                                      |
|            2004  'USA'                                                                              |                                                      |
|            2004  'GER'                                                                              |                                                      |
|            2008  'GER'                                                                              |                                                      |
+-----------------------------------------------------------------------------------------------------+------------------------------------------------------+
|                                                                                                     | ::                                                   |
|                                                                                                     |                                                      |
|                                                                                                     |   csql> SELECT * FROM lock_tbl;                      |
|                                                                                                     |                                                      |
|                                                                                                     |       host_year  nation_code                         |
|                                                                                                     |   ===================================                |
|                                                                                                     |            2004  'KOR'                               |
|                                                                                                     |            2004  'USA'                               |
|                                                                                                     |            2004  'GER'                               |
|                                                                                                     |            2008  'GER'                               |
+-----------------------------------------------------------------------------------------------------+------------------------------------------------------+
| ::                                                                                                  |                                                      |
|                                                                                                     |                                                      |
|   csql> DELETE FROM lock_tbl WHERE host_year=2008;                                                  |                                                      |
|                                                                                                     |                                                      |
|   /* no result until transaction 2 releases a lock                                                  |                                                      |
|                                                                                                     |                                                      |
| ::                                                                                                  |                                                      |
|                                                                                                     |                                                      |
|   $ cubrid lockdb demodb                                                                            |                                                      |
|                                                                                                     |                                                      |
|   *** Lock Table Dump ***                                                                           |                                                      |
|                                                                                                     |                                                      |
|   ...                                                                                               |                                                      |
|                                                                                                     |                                                      |
|                                                                                                     |                                                      |
|   Object type: Class = lock_tbl.                                                                    |                                                      |
|   LOCK HOLDERS:                                                                                     |                                                      |
|       Tran_index =   2, Granted_mode =   S_LOCK, Count =   2, Nsubgranules =  0                     |                                                      |
|                                                                                                     |                                                      |
|   BLOCKED LOCK HOLDERS:                                                                             |                                                      |
|       Tran_index =   1, Granted_mode =   S_LOCK, Count =   3, Nsubgranules =  0                     |                                                      |
|       Blocked_mode = SIX_LOCK                                                                       |                                                      |
|       Start_waiting_at = Fri Feb 12 14:22:58 2010                                                   |                                                      |
|       Wait_for_nsecs = -1                                                                           |                                                      |
+-----------------------------------------------------------------------------------------------------+------------------------------------------------------+
|                                                                                                     | ::                                                   |
|                                                                                                     |                                                      |
|                                                                                                     |   csql> INSERT INTO lock_tbl VALUES (2004, 'AUS');   |
+-----------------------------------------------------------------------------------------------------+------------------------------------------------------+
| ::                                                                                                  |                                                      |
|                                                                                                     |                                                      |
|   ERROR: Your transaction (index 1, dba@ 090205|4760) has been unilaterally aborted by the system.  |                                                      |
|                                                                                                     |                                                      |
|   /* System rolled back the transaction 1 to resolve a deadlock */                                  |                                                      |
|                                                                                                     |                                                      |
| ::                                                                                                  |                                                      |
|                                                                                                     |                                                      |
|   $ cubrid lockdb demodb                                                                            |                                                      |
|                                                                                                     |                                                      |
|   *** Lock Table Dump ***                                                                           |                                                      |
|                                                                                                     |                                                      |
|   Object type: Class = lock_tbl.                                                                    |                                                      |
|   LOCK HOLDERS:                                                                                     |                                                      |
|       Tran_index =   2, Granted_mode = SIX_LOCK, Count =   3, Nsubgranules =  0                     |                                                      |
+-----------------------------------------------------------------------------------------------------+------------------------------------------------------+

Transaction Lock Timeout
------------------------

CUBRID provides the  lock timeout feature, which sets the waiting time for the lock until the transaction lock setting is allowed.

If the lock is allowed within the lock timeout, CUBRID rolls back the transaction and outputs an error message when the timeout has passed. If a transaction deadlock occurs within the lock timeout, CUBRID rolls back the transaction whose waiting time is closest to the timeout.

**Setting the Lock Timeout**

The system parameter **lock_timeout_in_secs** in the **$CUBRID/conf/cubrid.conf** file or the **SET TRANSACTION** statement sets the timeout (in seconds) during which the application will wait for the lock and rolls back the transaction and outputs an error message when the specified time has passed. The default value of the **lock_timeout_in_secs** parameter is **-1**, which means the application will wait indefinitely until the transaction lock is allowed. Therefore, the user can change this value depending on the transaction pattern of the application. If the lock timeout value has been set to 0, an error message will be displayed as soon as a lock occurs. ::

    SET TRANSACTION LOCK TIMEOUT timeout_spec [ ; ]
    timeout_spec:
    - INFINITE
    - OFF
    - unsigned_integer
    - variable

*   **INFINITE** : Wait indefinitely until the transaction lock is allowed. Has the same effect as setting the system parameter **lock_timeout_in_secs** to -1.
*   **OFF** : Do not wait for the lock, but roll back the transaction and display an error message. Has the same effect as setting the system parameter **lock_timeout_in_secs** to 0.
*   *unsigned_integer* : Set in seconds. Wait for the transaction lock for the specified time period.  
*   *variable* : A variable can be specified. Wait for the transaction lock for the value stored by the variable.

**Example 1** ::


    vi $CUBRID/conf/cubrid.conf
    ...
    
    lock_timeout_in_secs = 10
    ...

**Example 2** ::

    SET TRANSACTION LOCK TIMEOUT 10;

**Checking the Lock Timeout**

You can check the lock timeout set for the current application by using the **GET TRANSACTION** statement, or store this value in a variable. ::

    GET TRANSACTION LOCK TIMEOUT [ { INTO | TO } variable ] [ ; ]

**Example** ::

    GET TRANSACTION LOCK TIMEOUT;
             Result
    ===============
      1.000000e+001

**Checking and Handling Lock Timeout Error Message**

The following message is displayed if lock timeout occurs in a transaction that has been waiting for another transaction's lock to be released. ::

    Your transaction (index 2, user1@host1|9808) timed out waiting on IX_LOCK lock on class tbl. You are waiting for
    user(s) user1@host1|csql(9807), user1@host1|csql(9805) to finish.
    
*   Your transaction(index 2 ...): This means that the index of the transaction that was rolled back due to timeout while waiting for the lock is 2. The transaction index is a number that is sequentially assigned when the client connects to the database server. You can also check this number by executing the **cubrid lockdb** utility.

*   (... user1\@host1|9808): *cub_user* is the login ID of the client and the part after @ is the name of the host where the client was running. The part after| is the process ID (PID) of the client.

*   IX_LOCK: This means the exclusive lock set on the object to perform data update. For details, see :ref:`lock-mode`.
*   user1@host1|csql(9807), user1@host1|csql(9805): Another transactions waiting for termination to lock **IX_LOCK**

That is, the above lock error message can be interpreted as meaning that "Because another client is holding **X_LOCK** on a specific row in the *participant* table, transaction 3 which running on the host *cdbs006.cub* waited for the lock and was rolled back as the timeout has passed." 

If you want to check the lock information of the transaction specified in the error message, you can do so by using the **cubrid lockdb** utility to search for the OID value (ex: 0|636|34) of a specific row where the **X_LOCK** is set currently to find the transaction ID currently holding the lock, the client program name and the process ID (PID). For details, see :ref:`lockdb`. You can also check the transaction lock information in the CUBRID Manager.

You can organize the transactions by checking uncommitted queries through the SQL log after checking the transaction lock information in the manner described above. For information on checking the SQL log, see :ref:`broker-logs`.

Also, you can forcefully stop problematic transactions by using the **cubrid killtran** utility. For details, see :ref:`killtran`.

.. _transaction-isolation-level:

Transaction Isolation Level
===========================

The transaction isolation level is determined based on how much interference occurs. The more isolation means the less interference from other transactions and more serializable. The less isolation means the more interference from other transactions and higher level of concurrency. You can control the level of consistency and concurrency specific to a service by setting appropriate isolation level.

.. note:: A transaction can be restored in all supported isolation levels because updates are not committed before the end of the transaction.

.. _set-transaction-isolation-level:

SET TRANSACTION ISOLATION LEVEL
-------------------------------

You can set the level of transaction isolation by using **isolation_level** and the **SET TRANSACTION** statement in the **$CUBRID/conf/cubrid.conf**. The level of **REPEATABLE READ CLASS** and **READ UNCOMMITTED INSTANCES** are set by default, which indicates the level 3 through level 1 to 6. For details, see :ref:`database-concurrency`. ::

    SET TRANSACTION ISOLATION LEVEL isolation_level_spec [ ; ]
    isolation_level_spec:
    _ SERIALIZABLE
    _ CURSOR STABILITY
    _ isolation_level [ { CLASS | SCHEMA } [ , isolation_level INSTANCES ] ]
    _ isolation_level [ INSTANCES [ , isolation_level { CLASS | SCHEMA } ] ]
    _ variable
    isolation_level:
    _ REPEATABLE READ
    _ READ COMMITTED
    _ READ UNCOMMITTED

**Example 1** ::

    vi $CUBRID/conf/cubrid.conf
    ...

    isolation_level = 1
    ...
     
    or
     
    isolation_level = "TRAN_COMMIT_CLASS_UNCOMMIT_INSTANCE"

**Example 2** ::

    SET TRANSACTION ISOLATION LEVEL 1;
    -- or
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED CLASS,READ UNCOMMITTED INSTANCES;

The following table shows the isolation levels from 1 to 6. It consists of table schema (row) and isolation level. For the unsupported isolation level, see :ref:`unsupported-isolation-level`.

**Levels of Isolation Supported by CUBRID**

+-----------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Name                                                      | Description                                                                                                                                                                         |
+===========================================================+=====================================================================================================================================================================================+
| SERIALIZABLE (6)                                          | In this isolation level, problems concerning concurrency (e.g. dirty read, non-repeatable read, phantom read, etc.) do not occur.                                                   |
+-----------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| REPEATABLE READ CLASS with REPEATABLE READ INSTANCES (5)  | Another transaction T2 cannot update the schema of table A while transaction T1 is viewing table A.                                                                                 |
|                                                           | Transaction T1 may experience phantom read for the record R that was inserted by another transaction T2 when it is repeatedly retrieving a specific record.                         |
+-----------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| REPEATABLE READ CLASS with READ COMMITTED INSTANCES (4)   | Another transaction T2 cannot update the schema of table A while transaction T1 is viewing table A.                                                                                 |
| (or CURSOR STABILITY)                                     | Transaction T1 may experience R read (non-repeatable read) that was updated and committed by another transaction T2 when it is repeatedly retrieving the record R.                  |
+-----------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| REPEATABLE READ CLASS with READ UNCOMMITTED INSTANCES (3) | Default isolation level.                                                                                                                                                            |
|                                                           | Another transaction T2 cannot update the schema of table A  while transaction T1 is viewing table A.                                                                                |
|                                                           | Transaction T1 may experience R' read (dirty read) for the record that was updated but not committed by another transaction T2.                                                     |
+-----------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| READ COMMITTED CLASS with READ COMMITTED INSTANCES (2)    | Transaction T1 may experience A' read (non-repeatable read) for the table that was updated and committed by another transaction  T2 while it is viewing table A repeatedly.         |
|                                                           | Transaction T1 may experience R' read (non-repeatable read) for the record that was updated and committed by another transaction T2 while it is retrieving the record R repeatedly. |
+-----------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| READ COMMITTED CLASS with READ UNCOMMITTED INSTANCES (1)  | Transaction T1 may experience A' read (non-repeatable read) for the table that was updated and committed by another transaction T2 while it is repeatedly viewing table A.          |
|                                                           | Transaction T1 may experience R' read (dirty read) for the record that was updated but not committed by another transaction T2.                                                     |
+-----------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

If the transaction level is changed in an application while a transaction is executed, the new level is applied to the rest of the transaction being executed. Therefore, some object locks that have already been obtained may be released during the transaction while the new isolation level is applied. For this reason, it is recommended that the transaction isolation level be modified when the transaction starts (after commit, rollback or system restart) because an isolation level which has already been set does not apply to the entire transaction, but can be changed during the transaction.

GET TRANSACTION ISOLATION LEVEL
-------------------------------

You can assign the current isolation level to *variable* by using the **GET TRANSACTION** statement. The following is a statement that verifies the isolation level. ::

    GET TRANSACTION ISOLATION LEVEL [ { INTO | TO } variable ] [ ; ]

.. code-block:: sql

    GET TRANSACTION ISOLATION LEVEL;
           Result
    =============
      READ COMMITTED SCHEMA, READ UNCOMMITTED INSTANCES

.. _isolation-level-6:

SERIALIZABLE
------------

The highest isolation level (6). Problems concerning concurrency (e.g. dirty read, non-repeatable read, phantom read, etc.) do not occur.

The following are the rules of this isolation level:

*   Transaction T1 cannot read or modify the record being updated by another transaction T2.
*   Transaction T1 cannot read or modify the record being viewed by another transaction T2.
*   Another transaction T2 cannot insert a new record into table A while transaction T1 is retrieving the records of table A.

This isolation level uses a two-phase locking protocol for shared and exclusive lock: the lock is held until the transaction ends even after the operation has been executed.

**Example**

The following example shows that another transaction cannot access the table or record while one transaction is reading or updating the object when the transaction level of the concurrent transactions is **SERIALIZABLE**.

+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
| session 1                                                               | session 2                                                                  |
+=========================================================================+============================================================================+
| ::                                                                      | ::                                                                         |
|                                                                         |                                                                            |
|   csql> ;autocommit off                                                 |   csql> ;autocommit off                                                    |
|                                                                         |                                                                            |
|   AUTOCOMMIT IS OFF                                                     |   AUTOCOMMIT IS OFF                                                        |
|                                                                         |                                                                            |
|   csql> SET TRANSACTION ISOLATION LEVEL 6;                              |   csql> SET TRANSACTION ISOLATION LEVEL 6;                                 |
|                                                                         |                                                                            |
|   Isolation level set to:                                               |   Isolation level set to:                                                  |
|   SERIALIZABLE                                                          |   SERIALIZABLE                                                             |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                      |                                                                            |
|                                                                         |                                                                            |
|   csql> CREATE TABLE isol6_tbl(host_year integer, nation_code char(3)); |                                                                            |
|                                                                         |                                                                            |
|   csql> INSERT INTO isol6_tbl VALUES (2008, 'AUS');                     |                                                                            |
|                                                                         |                                                                            |
|   csql> COMMIT;                                                         |                                                                            |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
|                                                                         | ::                                                                         |
|                                                                         |                                                                            |
|                                                                         |   csql> SELECT * FROM isol6_tbl WHERE nation_code = 'AUS';                 |
|                                                                         |                                                                            |
|                                                                         |       host_year  nation_code                                               |
|                                                                         |   ===================================                                      |
|                                                                         |            2008  'AUS'                                                     |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                      |                                                                            |
|                                                                         |                                                                            |
|   csql> INSERT INTO isol6_tbl VALUES (2004, 'AUS');                     |                                                                            |
|                                                                         |                                                                            |
|   /* unable to insert a row until the tran 2 committed */               |                                                                            |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
|                                                                         | ::                                                                         |
|                                                                         |                                                                            |
|                                                                         |   csql> COMMIT;                                                            |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
|                                                                         | ::                                                                         |
|                                                                         |                                                                            |
|                                                                         |   csql> SELECT * FROM isol6_tbl WHERE nation_code = 'AUS';                 |
|                                                                         |                                                                            |
|                                                                         |   /* unable to select rows until tran 1 committed */                       |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                      | ::                                                                         |
|                                                                         |                                                                            |
|   csql> COMMIT;                                                         |       host_year  nation_code                                               |
|                                                                         |   ===================================                                      |
|                                                                         |            2008  'AUS'                                                     |
|                                                                         |            2004  'AUS'                                                     |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                      |                                                                            |
|                                                                         |                                                                            |
|   csql> DELETE FROM isol6_tbl                                           |                                                                            |
|   csql> WHERE nation_code = 'AUS' and                                   |                                                                            |
|   csql> host_year=2008;                                                 |                                                                            |
|                                                                         |                                                                            |
|   /* unable to delete rows until tran 2 committed */                    |                                                                            |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
|                                                                         | ::                                                                         |
|                                                                         |                                                                            |
|                                                                         |   csql> COMMIT;                                                            |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
|                                                                         | ::                                                                         |
|                                                                         |                                                                            |
|                                                                         |   csql> SELECT * FROM isol6_tbl WHERE nation_code = 'AUS';                 |
|                                                                         |                                                                            |
|                                                                         |   /* unable to select rows until tran 1 committed */                       |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                      | ::                                                                         |
|                                                                         |                                                                            |
|   csql> COMMIT;                                                         |       host_year  nation_code                                               |
|                                                                         |   ===================================                                      |
|                                                                         |            2004  'AUS'                                                     |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                      | ::                                                                         |
|                                                                         |                                                                            |
|   csql> ALTER TABLE isol6_tbl                                           |   /* repeatable read is ensured while tran_1 is altering table schema */   |
|                                                                         |                                                                            |
|   /* unable to alter the table schema until tran 2 committed */         |       host_year  nation_code                                               |
|                                                                         |   ===================================                                      |
|                                                                         |            2004  'AUS'                                                     |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
|                                                                         | ::                                                                         |
|                                                                         |                                                                            |
|                                                                         |   csql> COMMIT;                                                            |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
|                                                                         | ::                                                                         |
|                                                                         |                                                                            |
|                                                                         |   csql> SELECT * FROM isol6_tbl WHERE nation_code = 'AUS';                 |
|                                                                         |                                                                            |
|                                                                         |   /* unable to access the table until tran_1 committed */                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+
| ::                                                                      | ::                                                                         |
|                                                                         |                                                                            |
|   csql> COMMIT;                                                         |   host_year  nation_code  gold                                             |
|                                                                         |   ===================================                                      |
|                                                                         |     2004  'AUS'           NULL                                             |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------+

.. _isolation-level-5:

REPEATABLE READ CLASS with REPEATABLE READ INSTANCES
----------------------------------------------------

A relatively high isolation level (5). A dirty or non-repeatable read does not occur, but a phantom read may.

The following are the rules of this isolation level:

*   Transaction T1 cannot read or modify the record being updated by another transaction T2.
*   Transaction T1 cannot read or modify the record being viewed by another transaction T2.
*   Another transaction T2 can insert a new record into table A while transaction T1 is retrieving records of table A. However, transaction T1 and T2 cannot set the lock on the same record.

This isolation level uses a two-phase locking protocol.

**Example**

The following example shows that phantom read may occur because another transaction can add a new record while one transaction is performing the object read when the transaction level of the concurrent transactions is **REPEATABLE READ CLASS** with **REPEATABLE READ INSTANCES**.

+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| session 1                                                                  | session 2                                                                   |
+============================================================================+=============================================================================+
| ::                                                                         | ::                                                                          |
|                                                                            |                                                                             |
|   csql> ;autocommit off                                                    |   csql> ;autocommit off                                                     |
|                                                                            |                                                                             |
|   AUTOCOMMIT IS OFF                                                        |   AUTOCOMMIT IS OFF                                                         |
|                                                                            |                                                                             |
|   csql> SET TRANSACTION ISOLATION LEVEL 5;                                 |   csql> SET TRANSACTION ISOLATION LEVEL 5;                                  |
|                                                                            |                                                                             |
|   Isolation level set to:                                                  |   Isolation level set to:                                                   |
|   REPEATABLE READ SCHEMA, REPEATABLE READ INSTANCES.                       |   REPEATABLE READ SCHEMA, REPEATABLE READ INSTANCES.                        |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| ::                                                                         |                                                                             |
|                                                                            |                                                                             |
|   csql> CREATE TABLE isol5_tbl(host_year integer, nation_code char(3));    |                                                                             |
|   csql> CREATE UNIQUE INDEX on isol5_tbl(nation_code, host_year);          |                                                                             |
|                                                                            |                                                                             |
|   csql> INSERT INTO isol5_tbl VALUES (2008, 'AUS');                        |                                                                             |
|   csql> INSERT INTO isol5_tbl VALUES (2004, 'AUS');                        |                                                                             |
|                                                                            |                                                                             |
|   csql> COMMIT;                                                            |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | ::                                                                          |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code='AUS';                    |
|                                                                            |                                                                             |
|                                                                            |       host_year  nation_code                                                |
|                                                                            |   ===================================                                       |
|                                                                            |            2004  'AUS'                                                      |
|                                                                            |            2008  'AUS'                                                      |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| ::                                                                         |                                                                             |
|                                                                            |                                                                             |
|   csql> INSERT INTO isol5_tbl VALUES (2004, 'KOR');                        |                                                                             |
|   csql> INSERT INTO isol5_tbl VALUES (2000, 'AUS');                        |                                                                             |
|                                                                            |                                                                             |
|   /* able to insert new rows only when locks are not conflicted */         |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | ::                                                                          |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code='AUS';                    |
|                                                                            |                                                                             |
|                                                                            |   /* phantom read may occur when tran 1 committed */                        |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| ::                                                                         | ::                                                                          |
|                                                                            |                                                                             |
|   csql> COMMIT;                                                            |       host_year  nation_code                                                |
|                                                                            |   ===================================                                       |
|                                                                            |            2000  'AUS'                                                      |
|                                                                            |            2004  'AUS'                                                      |
|                                                                            |            2008  'AUS'                                                      |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| ::                                                                         |                                                                             |
|                                                                            |                                                                             |
|   csql> DELETE FROM isol5_tbl                                              |                                                                             |
|   csql> WHERE nation_code = 'AUS' and                                      |                                                                             |
|   csql> host_year=2008;                                                    |                                                                             |
|                                                                            |                                                                             |
|   /* unable to delete rows until tran 2 committed */                       |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | ::                                                                          |
|                                                                            |                                                                             |
|                                                                            |   csql> COMMIT;                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | ::                                                                          |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code = 'AUS';                  |
|                                                                            |                                                                             |
|                                                                            |   /* unable to select rows until tran 1 committed */                        |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| ::                                                                         | ::                                                                          |
|                                                                            |                                                                             |
|   csql> COMMIT;                                                            |       host_year  nation_code                                                |
|                                                                            |   ===================================                                       |
|                                                                            |            2000  'AUS'                                                      |
|                                                                            |            2004  'AUS'                                                      |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| ::                                                                         |                                                                             |
|                                                                            |                                                                             |
|   csql> ALTER TABLE isol5_tbl ADD COLUMN gold INT;                         |                                                                             |
|                                                                            |                                                                             |
|   /* unable to alter the table schema until tran 2 committed */            |                                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | ::                                                                          |
|                                                                            |                                                                             |
|                                                                            |   /* repeatable read is ensured while tran_1 is altering table schema */    |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code = 'AUS';                  |
|                                                                            |                                                                             |
|                                                                            |       host_year  nation_code                                                |
|                                                                            |   ===================================                                       |
|                                                                            |            2000  'AUS'                                                      |
|                                                                            |            2004  'AUS'                                                      |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | ::                                                                          |
|                                                                            |                                                                             |
|                                                                            |   csql> COMMIT;                                                             |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
|                                                                            | ::                                                                          |
|                                                                            |                                                                             |
|                                                                            |   csql> SELECT * FROM isol5_tbl WHERE nation_code = 'AUS';                  |
|                                                                            |                                                                             |
|                                                                            |   /* unable to access the table until tran_1 committed */                   |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+
| ::                                                                         | ::                                                                          |
|                                                                            |                                                                             |
|   csql> COMMIT;                                                            |   host_year  nation_code  gold                                              |
|                                                                            |   ===================================                                       |
|                                                                            |     2000  'AUS'           NULL                                              |
|                                                                            |     2004  'AUS'           NULL                                              |
+----------------------------------------------------------------------------+-----------------------------------------------------------------------------+

.. _isolation-level-4:

REPEATABLE READ CLASS with READ COMMITTED INSTANCES
---------------------------------------------------

A relatively low isolation level (4). A dirty read does not occur, but non-repeatable or phantom read may. That is, transaction T1 can read another value because insert or update by transaction T2 is allowed while transaction T1 is repeatedly retrieving one object.

The following are the rules of this isolation level:

*   Transaction T1 cannot read the record being updated by another transaction T2.
*   Transaction T1 can update/insert record to the table being viewed by another transaction T2.
*   Transaction T1 cannot change the schema of the table being viewed by another transaction T2.

This isolation level uses a two-phase locking protocol for an exclusive lock. A shared lock on a row is released immediately after it is read; however, an intention lock on a table is released when a transaction terminates to ensure repeatable read on the schema.

**Example**

The following example shows that a phantom or non-repeatable read may occur because another transaction can add or update a record while one transaction is performing the object read but repeatable read for the table schema update is ensured when the transaction level of the concurrent transactions is **REPEATABLE READ CLASS** with **READ COMMITTED INSTANCES**.

+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| session 1                                                               | session 2                                                                        |
+=========================================================================+==================================================================================+
| ::                                                                      | ::                                                                               |
|                                                                         |                                                                                  |
|   csql> ;autocommit off                                                 |   csql> ;autocommit off                                                          |
|                                                                         |                                                                                  |
|   AUTOCOMMIT IS OFF                                                     |   AUTOCOMMIT IS OFF                                                              |
|                                                                         |                                                                                  |
|   csql> SET TRANSACTION ISOLATION LEVEL 4;                              |   csql> SET TRANSACTION ISOLATION LEVEL 4;                                       |
|                                                                         |                                                                                  |
|   Isolation level set to:                                               |   Isolation level set to:                                                        |
|   REPEATABLE READ SCHEMA, READ COMMITTED INSTANCES.                     |   REPEATABLE READ SCHEMA, READ COMMITTED INSTANCES.                              |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| ::                                                                      |                                                                                  |
|                                                                         |                                                                                  |
|   csql> CREATE TABLE isol4_tbl(host_year integer, nation_code char(3)); |                                                                                  |
|                                                                         |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (2008, 'AUS');                     |                                                                                  |
|                                                                         |                                                                                  |
|   csql> COMMIT;                                                         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | ::                                                                               |
|                                                                         |                                                                                  |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |       host_year  nation_code                                                     |
|                                                                         |   ===================================                                            |
|                                                                         |            2008  'AUS'                                                           |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| ::                                                                      |                                                                                  |
|                                                                         |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (2004, 'AUS');                     |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (2000, 'NED');                     |                                                                                  |
|                                                                         |                                                                                  |
|   /* able to insert new rows even if tran 2 uncommitted */              |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | ::                                                                               |
|                                                                         |                                                                                  |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |   /* phantom read may occur when tran 1 committed */                             |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| ::                                                                      | ::                                                                               |
|                                                                         |                                                                                  |
|   csql> COMMIT;                                                         |       host_year  nation_code                                                     |
|                                                                         |   ===================================                                            |
|                                                                         |            2008  'AUS'                                                           |
|                                                                         |            2004  'AUS'                                                           |
|                                                                         |            2000  'NED'                                                           |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| ::                                                                      |                                                                                  |
|                                                                         |                                                                                  |
|   csql> INSERT INTO isol4_tbl VALUES (1994, 'FRA');                     |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | ::                                                                               |
|                                                                         |                                                                                  |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |   /* unrepeatable read may occur when tran 1 committed */                        |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| ::                                                                      |                                                                                  |
|                                                                         |                                                                                  |
|   csql> DELETE FROM isol4_tbl                                           |                                                                                  |
|   csql> WHERE nation_code = 'AUS' and                                   |                                                                                  |
|   csql> host_year=2008;                                                 |                                                                                  |
|                                                                         |                                                                                  |
|   /* able to delete rows while tran 2 is selecting rows*/               |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| ::                                                                      | ::                                                                               |
|                                                                         |                                                                                  |
|   csql> COMMIT;                                                         |       host_year  nation_code                                                     |
|                                                                         |   ===================================                                            |
|                                                                         |            2004  'AUS'                                                           |
|                                                                         |            2000  'NED'                                                           |
|                                                                         |            1994  'FRA'                                                           |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| ::                                                                      |                                                                                  |
|                                                                         |                                                                                  |
|   csql> ALTER TABLE isol4_tbl ADD COLUMN gold INT;                      |                                                                                  |
|                                                                         |                                                                                  |
|   /* unable to alter the table schema until tran 2 committed */         |                                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | ::                                                                               |
|                                                                         |                                                                                  |
|                                                                         |   /* repeatable read is ensured while tran_1 is altering table schema */         |
|                                                                         |                                                                                  |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |       host_year  nation_code                                                     |
|                                                                         |   ===================================                                            |
|                                                                         |            2004  'AUS'                                                           |
|                                                                         |            2000  'NED'                                                           |
|                                                                         |            1994  'FRA'                                                           |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | ::                                                                               |
|                                                                         |                                                                                  |
|                                                                         |   csql> COMMIT;                                                                  |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
|                                                                         | ::                                                                               |
|                                                                         |                                                                                  |
|                                                                         |   csql> SELECT * FROM isol4_tbl;                                                 |
|                                                                         |                                                                                  |
|                                                                         |   /* unable to access the table until tran_1 committed */                        |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+
| ::                                                                      | ::                                                                               |
|                                                                         |                                                                                  |
|   csql> COMMIT;                                                         |   host_year  nation_code  gold                                                   |
|                                                                         |   ===================================                                            |
|                                                                         |     2004  'AUS'           NULL                                                   |
|                                                                         |     2000  'NED'           NULL                                                   |
|                                                                         |     1994  'FRA'           NULL                                                   |
+-------------------------------------------------------------------------+----------------------------------------------------------------------------------+

.. _isolation-level-3:

REPEATABLE READ CLASS with READ UNCOMMITTED INSTANCES
-----------------------------------------------------

The default isolation of CUBRID (3). The concurrency level is high. A dirty, non-repeatable or phantom read may occur for the rows, but repeatable read is ensured for the table. That is, transaction T2 can read an object while transaction T1 is updating one.

The following are the rules of this isolation level:

*   Transaction T1 can read the record being updated by another transaction T2.
*   Transaction T1 can update/insert record to the table being viewed by another transaction T2.
*   Transaction T1 cannot change the schema of the table being viewed by another transaction T2.

This isolation level uses a two-phase locking protocol for an exclusive and update lock. However, the shared lock on the rows is released immediately after it is retrieved. The intention lock on the table is released when the transaction ends to ensure repeatable reads.

**Example**

The following example shows that another transaction can read dirty data uncommitted by one transaction but repeatable reads are ensured for table schema update when the transaction level of the concurrent transactions is **REPEATABLE READ CLASS** with **READ UNCOMMITTED INSTANCES**.

+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| session 1                                                                 | session 2                                                                           |
+===========================================================================+=====================================================================================+
| ::                                                                        | ::                                                                                  |
|                                                                           |                                                                                     |
|   csql> ;autocommit off                                                   |   csql> ;autocommit off                                                             |
|                                                                           |                                                                                     |
|   AUTOCOMMIT IS OFF                                                       |   AUTOCOMMIT IS OFF                                                                 |
|                                                                           |                                                                                     |
|   csql> SET TRANSACTION ISOLATION LEVEL 3;                                |   csql> SET TRANSACTION ISOLATION LEVEL 3;                                          |
|                                                                           |                                                                                     |
|   Isolation level set to:                                                 |   Isolation level set to:                                                           |
|   REPEATABLE READ SCHEMA, READ UNCOMMITTED INSTANCES.                     |   REPEATABLE READ SCHEMA, READ UNCOMMITTED INSTANCES.                               |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                        |                                                                                     |
|                                                                           |                                                                                     |
|   csql> CREATE TABLE isol3_tbl(host_year integer, nation_code char(3));   |                                                                                     |
|   csql> CREATE UNIQUE INDEX on isol3_tbl(nation_code, host_year);         |                                                                                     |
|                                                                           |                                                                                     |
|   csql> INSERT INTO isol3_tbl VALUES (2008, 'AUS');                       |                                                                                     |
|                                                                           |                                                                                     |
|   csql> COMMIT;                                                           |                                                                                     |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                           | ::                                                                                  |
|                                                                           |                                                                                     |
|                                                                           |   csql> SELECT * FROM isol3_tbl;                                                    |
|                                                                           |                                                                                     |
|                                                                           |       host_year  nation_code                                                        |
|                                                                           |   ===================================                                               |
|                                                                           |            2008  'AUS'                                                              |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                        |                                                                                     |
|                                                                           |                                                                                     |
|   csql> INSERT INTO isol3_tbl VALUES (2004, 'AUS');                       |                                                                                     |
|   csql> INSERT INTO isol3_tbl VALUES (2000, 'NED');                       |                                                                                     |
|                                                                           |                                                                                     |
|   /* able to insert new rows even if tran 2 uncommitted */                |                                                                                     |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                           | ::                                                                                  |
|                                                                           |                                                                                     |
|                                                                           |   csql> SELECT * FROM isol3_tbl;                                                    |
|                                                                           |                                                                                     |
|                                                                           |       host_year  nation_code                                                        |
|                                                                           |   ===================================                                               |
|                                                                           |            2008  'AUS'                                                              |
|                                                                           |            2004  'AUS'                                                              |
|                                                                           |            2000  'NED'                                                              |
|                                                                           |                                                                                     |
|                                                                           |   /* dirty read may occur so that tran_2 can select new rows                        |
|                                                                           |      uncommitted by tran_1 */                                                       |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                        |                                                                                     |
|                                                                           |                                                                                     |
|   csql> ROLLBACK;                                                         |                                                                                     |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                           | ::                                                                                  |
|                                                                           |                                                                                     |
|                                                                           |   csql> SELECT * FROM isol3_tbl;                                                    |
|                                                                           |                                                                                     |
|                                                                           |       host_year  nation_code                                                        |
|                                                                           |   ===================================                                               |
|                                                                           |            2008  'AUS'                                                              |
|                                                                           |                                                                                     |
|                                                                           |   /* unrepeatable read may occur so that selected results are different */          |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                        |                                                                                     |
|                                                                           |                                                                                     |
|   csql> INSERT INTO isol3_tbl VALUES (1994, 'FRA');                       |                                                                                     |
|                                                                           |                                                                                     |
|   csql> DELETE FROM isol3_tbl                                             |                                                                                     |
|   csql> WHERE nation_code = 'AUS' and                                     |                                                                                     |
|   csql> host_year=2008;                                                   |                                                                                     |
|                                                                           |                                                                                     |
|   /* able to delete rows even if tran 2 uncommitted */                    |                                                                                     |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                           | ::                                                                                  |
|                                                                           |                                                                                     |
|                                                                           |   csql> SELECT * FROM isol3_tbl;                                                    |
|                                                                           |                                                                                     |
|                                                                           |       host_year  nation_code                                                        |
|                                                                           |   ===================================                                               |
|                                                                           |            1994  'FRA'                                                              |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                        |                                                                                     |
|                                                                           |                                                                                     |
|   csql> ALTER TABLE isol3_tbl ADD COLUMN gold INT;                        |                                                                                     |
|                                                                           |                                                                                     |
|   /* unable to alter the table schema until tran 2 committed */           |                                                                                     |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                           | ::                                                                                  |
|                                                                           |                                                                                     |
|                                                                           |   /* repeatable read is ensured while tran_1 is altering table schema */            |
|                                                                           |                                                                                     |
|                                                                           |   csql> SELECT * FROM isol3_tbl;                                                    |
|                                                                           |                                                                                     |
|                                                                           |       host_year  nation_code                                                        |
|                                                                           |   ===================================                                               |
|                                                                           |            1994  'FRA'                                                              |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                           | ::                                                                                  |
|                                                                           |                                                                                     |
|                                                                           |   csql> COMMIT;                                                                     |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                           | ::                                                                                  |
|                                                                           |                                                                                     |
|                                                                           |   csql> SELECT * FROM isol3_tbl;                                                    |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                        | ::                                                                                  |
|                                                                           |                                                                                     |
|   csql> COMMIT;                                                           |   host_year  nation_code  gold                                                      |
|                                                                           |   ===================================                                               |
|                                                                           |     1994  'FRA'           NULL                                                      |
+---------------------------------------------------------------------------+-------------------------------------------------------------------------------------+

.. note::

    CUBRID flushes dirty data (or dirty records) in the client buffers to the database (server) such as the following situations. For details, see :ref:`dirty-record-flush`.

.. _isolation-level-2:

READ COMMITTED CLASS with READ COMMITTED INSTANCES
--------------------------------------------------

A relatively low isolation level (2). A dirty read does not occur, but non-repeatable or phantom read may occur. That is, this level is similar to **REPEATABLE READ CLASS** with **READ COMMITTED INSTANCES** (level 4) described above, but works differently for table schema. Non-repeatable read due to a table schema update may occur because another transaction T2 can change the schema of the table being viewed by the transaction T1.

The following are the rules of this isolation level:

*   Transaction T1 cannot read the record being updated by another transaction T2.
*   Transaction T1 can update/insert a record to the table being viewed by another transaction T2.
*   Transaction T1 can change the schema of the table being viewed by another transaction T2.

This isolation level uses a two-phase locking protocol for an exclusive lock. However, non-repeatable read may occur because the shared lock on the rows is released immediately after it is retrieved and the intention lock on the table is released immediately as well.

**Example**

The following example shows that phantom or non-repeatable read for the record as well as for the table schema may occur because another transaction can add or update a new record while one transaction is performing the object read when the transaction level of the concurrent transactions is **READ COMMITTED CLASS** with **READ COMMITTED INSTANCES**.

+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| session 1                                                                       | session 2                                                                         |
+=================================================================================+===================================================================================+
| ::                                                                              | ::                                                                                |
|                                                                                 |                                                                                   |
|   csql> ;autocommit off                                                         |   csql> ;autocommit off                                                           |
|                                                                                 |                                                                                   |
|   AUTOCOMMIT IS OFF                                                             |   AUTOCOMMIT IS OFF                                                               |
|                                                                                 |                                                                                   |
|   csql> SET TRANSACTION ISOLATION LEVEL 2;                                      |   csql> SET TRANSACTION ISOLATION LEVEL 2;                                        |
|                                                                                 |                                                                                   |
|   Isolation level set to:                                                       |   Isolation level set to:                                                         |
|   READ COMMITTED SCHEMA, READ COMMITTED INSTANCES.                              |   READ COMMITTED SCHEMA, READ COMMITTED INSTANCES.                                |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| ::                                                                              |                                                                                   |
|                                                                                 |                                                                                   |
|   csql> CREATE TABLE isol2_tbl(host_year integer, nation_code char(3));         |                                                                                   |
|   csql> CREATE UNIQUE INDEX on isol2_tbl(nation_code, host_year);               |                                                                                   |
|                                                                                 |                                                                                   |
|   csql> INSERT INTO isol2_tbl VALUES (2008, 'AUS');                             |                                                                                   |
|                                                                                 |                                                                                   |
|   csql> COMMIT;                                                                 |                                                                                   |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                                 | ::                                                                                |
|                                                                                 |                                                                                   |
|                                                                                 |   csql> SELECT * FROM isol2_tbl;                                                  |
|                                                                                 |                                                                                   |
|                                                                                 |       host_year  nation_code                                                      |
|                                                                                 |   ===================================                                             |
|                                                                                 |            2008  'AUS'                                                            |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| ::                                                                              |                                                                                   |
|                                                                                 |                                                                                   |
|   csql> INSERT INTO isol2_tbl VALUES (2004, 'AUS');                             |                                                                                   |
|   csql> INSERT INTO isol2_tbl VALUES (2000, 'NED');                             |                                                                                   |
|                                                                                 |                                                                                   |
|   /* able to insert new rows even if tran 2 uncommitted */                      |                                                                                   |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                                 | ::                                                                                |
|                                                                                 |                                                                                   |
|                                                                                 |   csql> SELECT * FROM isol2_tbl;                                                  |
|                                                                                 |                                                                                   |
|                                                                                 |   /* phantom read may occur when tran 1 committed */                              |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| ::                                                                              | ::                                                                                |
|                                                                                 |                                                                                   |
|   csql> COMMIT;                                                                 |       host_year  nation_code                                                      |
|                                                                                 |   ===================================                                             |
|                                                                                 |            2008  'AUS'                                                            |
|                                                                                 |            2004  'AUS'                                                            |
|                                                                                 |            2000  'NED'                                                            |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| ::                                                                              |                                                                                   |
|                                                                                 |                                                                                   |
|   csql> INSERT INTO isol2_tbl VALUES (1994, 'FRA');                             |                                                                                   |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                                 | ::                                                                                |
|                                                                                 |                                                                                   |
|                                                                                 |   csql> SELECT * FROM isol2_tbl;                                                  |
|                                                                                 |                                                                                   |
|                                                                                 |   /* unrepeatable read may occur when tran 1 committed */                         |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| ::                                                                              |                                                                                   |
|                                                                                 |                                                                                   |
|   csql> DELETE FROM isol2_tbl                                                   |                                                                                   |
|   csql> WHERE nation_code = 'AUS' and                                           |                                                                                   |
|   csql> host_year=2008;                                                         |                                                                                   |
|                                                                                 |                                                                                   |
|   /* able to delete rows even if tran 2 uncommitted */                          |                                                                                   |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| ::                                                                              | ::                                                                                |
|                                                                                 |                                                                                   |
|   csql> COMMIT;                                                                 |       host_year  nation_code                                                      |
|                                                                                 |   ===================================                                             |
|                                                                                 |            2004  'AUS'                                                            |
|                                                                                 |            2000  'NED'                                                            |
|                                                                                 |            1994  'FRA'                                                            |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| ::                                                                              |                                                                                   |
|                                                                                 |                                                                                   |
|   csql> ALTER TABLE isol2_tbl ADD COLUMN gold INT;                              |                                                                                   |
|                                                                                 |                                                                                   |
|   /* able to alter the table schema even if tran 2 is uncommitted yet*/         |                                                                                   |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
|                                                                                 | ::                                                                                |
|                                                                                 |                                                                                   |
|                                                                                 |   /* unrepeatable read may occur so that result shows different schema */         |
|                                                                                 |                                                                                   |
|                                                                                 |   csql> SELECT * FROM isol2_tbl;                                                  |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+
| ::                                                                              | ::                                                                                |
|                                                                                 |                                                                                   |
|   csql> COMMIT;                                                                 |   host_year  nation_code  gold                                                    |
|                                                                                 |   ===================================                                             |
|                                                                                 |     2004  'AUS'           NULL                                                    |
|                                                                                 |     2000  'NED'           NULL                                                    |
|                                                                                 |     1994  'FRA'           NULL                                                    |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+

.. _isolation-level-1:

READ COMMITTED CLASS with READ UNCOMMITTED INSTANCES
----------------------------------------------------

The lowest isolation level (1). The concurrency level is the highest. A dirty, non-repeatable or phantom read may occur for the rows and a non-repeatable read may occur for the table as well. Similar to **REPEATABLE READ CLASS** with **READ UNCOMMITTED INSTANCES** (level 3) described above, but works differently for the table schema. That is, non-repeatable read due to table schema update may occur because another transaction T2 can change the schema of the table being viewed by the transaction T1.

The following are the rules of this isolation level:

*   Transaction T1 can read the record being updated by another transaction T2.
*   Transaction T1 can update/insert record to the table being viewed by another transaction T2.
*   Transaction T1 can change the schema of the table being viewed by another transaction T2.

This isolation level uses a two-phase locking protocol for an exclusive and update lock. However, the shared lock on the rows is released immediately after it is retrieved. The intention lock on the table is released immediately after the retrieval as well.

**Example**

+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| session 1                                                                       | session 2                                                                           |
+=================================================================================+=====================================================================================+
| ::                                                                              | ::                                                                                  |
|                                                                                 |                                                                                     |
|   csql> ;autocommit off                                                         |   csql> ;autocommit off                                                             |
|                                                                                 |                                                                                     |
|   AUTOCOMMIT IS OFF                                                             |   AUTOCOMMIT IS OFF                                                                 |
|                                                                                 |                                                                                     |
|   csql> SET TRANSACTION ISOLATION LEVEL 1;                                      |   csql> SET TRANSACTION ISOLATION LEVEL 1;                                          |
|                                                                                 |                                                                                     |
|   Isolation level set to:                                                       |   Isolation level set to:                                                           |
|   READ COMMITTED SCHEMA, READ UNCOMMITTED INSTANCES.                            |   READ COMMITTED SCHEMA, READ UNCOMMITTED INSTANCES.                                |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                              |                                                                                     |
|                                                                                 |                                                                                     |
|   csql> CREATE TABLE isol1_tbl(host_year integer, nation_code char(3));         |                                                                                     |
|   csql> CREATE UNIQUE INDEX on isol1_tbl(nation_code, host_year);               |                                                                                     |
|                                                                                 |                                                                                     |
|   csql> INSERT INTO isol1_tbl VALUES (2008, 'AUS');                             |                                                                                     |
|                                                                                 |                                                                                     |
|   csql> COMMIT;                                                                 |                                                                                     |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                                 | ::                                                                                  |
|                                                                                 |                                                                                     |
|                                                                                 |   csql> SELECT * FROM isol1_tbl;                                                    |
|                                                                                 |                                                                                     |
|                                                                                 |       host_year  nation_code                                                        |
|                                                                                 |   ===================================                                               |
|                                                                                 |            2008  'AUS'                                                              |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                              |                                                                                     |
|                                                                                 |                                                                                     |
|   csql> INSERT INTO isol1_tbl VALUES (2004, 'AUS');                             |                                                                                     |
|   csql> INSERT INTO isol1_tbl VALUES (2000, 'NED');                             |                                                                                     |
|                                                                                 |                                                                                     |
|   /* able to insert new rows even if tran 2 uncommitted */                      |                                                                                     |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                                 | ::                                                                                  |
|                                                                                 |                                                                                     |
|                                                                                 |   csql> SELECT * FROM isol1_tbl;                                                    |
|                                                                                 |                                                                                     |
|                                                                                 |       host_year  nation_code                                                        |
|                                                                                 |   ===================================                                               |
|                                                                                 |            2008  'AUS'                                                              |
|                                                                                 |            2004  'AUS'                                                              |
|                                                                                 |            2000  'NED'                                                              |
|                                                                                 |                                                                                     |
|                                                                                 |   /* dirty read may occur so that tran_2 can select new rows                        |
|                                                                                 |      uncommitted by tran_1 */                                                       |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                              |                                                                                     |
|                                                                                 |                                                                                     |
|   csql> ROLLBACK;                                                               |                                                                                     |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                                 | ::                                                                                  |
|                                                                                 |                                                                                     |
|                                                                                 |   csql> SELECT * FROM isol1_tbl;                                                    |
|                                                                                 |                                                                                     |
|                                                                                 |       host_year  nation_code                                                        |
|                                                                                 |   ===================================                                               |
|                                                                                 |            2008  'AUS'                                                              |
|                                                                                 |                                                                                     |
|                                                                                 |   /* unrepeatable read may occur so that selected results are different */          |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                              |                                                                                     |
|                                                                                 |                                                                                     |
|   csql> INSERT INTO isol1_tbl VALUES (1994, 'FRA');                             |                                                                                     |
|                                                                                 |                                                                                     |
|   csql> DELETE FROM isol1_tbl                                                   |                                                                                     |
|   csql> WHERE nation_code = 'AUS' and                                           |                                                                                     |
|   csql> host_year=2008;                                                         |                                                                                     |
|                                                                                 |                                                                                     |
|   /* able to delete rows while tran 2 is selecting rows*/                       |                                                                                     |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                                 | ::                                                                                  |
|                                                                                 |                                                                                     |
|                                                                                 |   csql> SELECT * FROM isol1_tbl;                                                    |
|                                                                                 |                                                                                     |
|                                                                                 |       host_year  nation_code                                                        |
|                                                                                 |   ===================================                                               |
|                                                                                 |            1994  'FRA'                                                              |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                              |                                                                                     |
|                                                                                 |                                                                                     |
|   csql> ALTER TABLE isol1_tbl ADD COLUMN gold INT;                              |                                                                                     |
|                                                                                 |                                                                                     |
|   /* able to alter the table schema even if tran 2 is uncommitted yet*/         |                                                                                     |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
|                                                                                 | ::                                                                                  |
|                                                                                 |                                                                                     |
|                                                                                 |   /* unrepeatable read may occur so that result shows different schema */           |
|                                                                                 |                                                                                     |
|                                                                                 |   csql> SELECT * FROM isol1_tbl;                                                    |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+
| ::                                                                              | ::                                                                                  |
|                                                                                 |                                                                                     |
|   csql> COMMIT;                                                                 |   host_year  nation_code  gold                                                      |
|                                                                                 |   ====================================                                              |
|                                                                                 |     1994  'FRA'           NULL                                                      |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------------------------+

UPDATE INCONSISTENCY
--------------------

In this isolation level, uncommitted updates may be lost, which makes a transaction unrestorable (cannot be rolled back) because the data are committed before the end of the transaction. CUBRID does not support this isolation level because this can cause the updates made by the user to be lost.

The following are the rules of this isolation level:

*   A transaction does not overwrite an object being modified by another transaction.

.. note:: A transaction can be restored in all supported isolation levels because updates are not committed before the end of the transaction.

.. _unsupported-isolation-level:

Combination of Unsupported Isolation Level
------------------------------------------

You can set customized isolation levels by using the **SET TRANSACTION ISOLATION LEVE** statement. However, combinations of isolation levels below are not supported. If they are used, a system error message is shown up and an isolation level closest to the one specified is chosen.

The following are unsupported isolation levels. If table schema is changed while data is selected, unrepeatable read occurs; therefore, the combinations below are not supported.

*   **READ COMMITTED CLASS** with **REPEATABLE READ INSTANCES**
*   **READ UNCOMMITTED CLASS** with **REPEATABLE READ INSTANCES**

Neither are isolation levels below supported because updating a row by a transaction is not allowed while table schema is changed by other transaction.

*   **READ UNCOMMITTED CLASS** with **READ COMMITTED INSTANCES**
*   **READ UNCOMMITTED CLASS** with **READ UNCOMMITTED INSTANCES**

.. _dirty-record-flush:

How to Handle Dirty Record
--------------------------

CUBRID flushes dirty data (or dirty record) in the client buffers to the database (server) such as the following situations. In additions to those, there can be more situations where flushes can be performed.

*   Dirty data can be flushed to server when a transaction is committed.
*   Some of dirty data can be flushed to server when a lot of data is loaded into the client buffers.
*   Dirty data of table *A* can be flushed to server when the schema of table *A* is updated.
*   Dirty data of table *A* can be flushed to server when the table *A* is retrieved (**SELECT**)
*   Some of dirty data can be flushed to server when a server function is called.

Transaction Termination and Restoration
=======================================

The restore process in CUBRID makes it possible that the database is not affected even if a software or hardware error occurs. In CUBRID, all read and update commands that are made during a transaction must be atomic. This means that either all of the transaction's commands are committed to the database or none are. The concept of atomicity is extended to the set of operations that consists of a transaction. The transaction must either commit so that all effects are permanently applied to the database or roll back so that all effects are removed. To ensure transaction atomicity, CUBRID applies the effects of the committed transaction again every time an error occurs without the updates of the transaction being written to the disk. CUBRID also removes the effects of partially committed transactions in the database every time the site fails (some transactions may have not committed or applications may have requested to cancel transactions). This restore feature eases the burden for the applications of maintaining the database consistency depending on the system error. The restore process used in CUBRID is based on the undo/redo logging mechanism.

CUBRID provides an automatic restore method to maintain the transaction atomicity when a hardware or software error occurs. You do not have to take the responsibility for restore since CUBRID's restore feature always returns the database to a consistent state even when an application or computer system error occurs. For this purpose, CUBRID automatically rolls back part of committed transactions when the application fails or the user requests explicitly. For example, a system error that occurred during the execution of the **COMMIT WORK** statement must be stopped if the transaction has not committed yet (it cannot be confirmed that the user's operation has been committed). Automatic stop prevents errors causing undesired changes to the database by canceling uncommitted updates.

Restarting Database
-------------------

CUBRID uses log volumes/files and database backups to restore committed or uncommitted transactions when system or media (disk) error occurs. Logs are also used to support the user-specified rollback. A log consists of a collection of sequential files created by CUBRID. The most recent log is called the active log, and the rest are called archive logs. A log file refers to both the active and archive logs.

All updates of the database are written to the log. Actually, two copies of the updates are logged. The first one is called a before image (UNDO log) and used to restore data during execution of the user-specified **ROLLBACK WORK** statement or during media or system errors. The second copy is an after image (REDO log) and used to re-apply the updates when media or system error occurs.

When the active log is full, CUBRID copies it to an archive log to store in the disk. The archive log is needed to restore the database when a system failure occurs.

**Normal Termination or Error**

CUBRID restores the database if it restarts due to a normal termination or a device error. The restore process re-applies the committed changes that have not been applied to the database and removes the uncommitted changes stored in the database. The general operation of the database resumes after the restore is completed. This restore process does not use any archive logs or database backup.

In a client/server environment, the database can restart by using the **cubrid server** utility.

**Media Error**

The user's intervention is somewhat needed to restart the database after media error occurs. The first step is to restore the database by installing a backup of a known good state. In CUBRID, the most recent log file (the one after the last backup) must be installed. This specific log (archive or active) is applied to a backup copy of the database. As with normal termination, the database can restart after restoration is committed.

.. note::

    To minimize the possibility of losing database updates, it is recommended to create a snapshot and store it in the backup media before it is deleted from the disk. The DBA can backup and restore the database by using the **cubrid backupdb** and **cubrid restoredb** utilities. For details on these utilities, see :ref:`db-backup`.

